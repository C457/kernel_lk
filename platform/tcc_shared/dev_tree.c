/* Copyright (c) 2012-2013, The Linux Foundation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * * Redistributions of source code must retain the above copyright
 *  notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided
 *  with the distribution.
 *   * Neither the name of The Linux Foundation nor the names of its
 * contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <libfdt.h>
#include <dev_tree.h>
#include <lib/ptable.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <platform.h>
#include <board.h>
#include <tcc_lcd.h>
#include <daudio_ver.h>

#define min(a,b) ((a) < (b) ? (a) : (b))

extern int target_is_emmc_boot(void);
extern uint32_t target_dev_tree_mem(void *fdt, uint32_t memory_node_offset);

/*
 * Will relocate the DTB to the tags addr if the device tree is found and return
 * its address
 *
 * Arguments:    kernel - Start address of the kernel loaded in RAM
 *               tags - Start address of the tags loaded in RAM
 * Return Value: DTB address : If appended device tree is found
 *               'NULL'         : Otherwise
 */
void *dev_tree_appended(void *kernel, uint32_t kernel_size, void *tags)
{
	void *kernel_end = kernel + kernel_size;
	uint32_t app_dtb_offset = 0;
	void *dtb;

	memcpy((void*) &app_dtb_offset, (void*) (kernel + DTB_OFFSET), sizeof(uint32_t));

	dtb = kernel + app_dtb_offset;
	while (dtb + sizeof(struct fdt_header) < kernel_end) {
		bool compat;
		struct fdt_header dtb_hdr;
		uint32_t dtb_size;

		/* the DTB could be unaligned, so extract the header,
		 * and operate on it separately */
		memcpy(&dtb_hdr, dtb, sizeof(struct fdt_header));
		if (fdt_check_header((const void *)&dtb_hdr) != 0 ||
		    (dtb + fdt_totalsize((const void *)&dtb_hdr) > kernel_end))
			break;
		dtb_size = fdt_totalsize(&dtb_hdr);

		/* now that we know we have a valid DTB, we need to copy
		 * it somewhere aligned, like tags */
		memcpy(tags, dtb, dtb_size);

		/* goto the next device tree if any */
		dtb += dtb_size;
	}

	return NULL;
}

/* Function to return the pointer to the start of the correct device tree
 *  based on the platform data.
 */
struct dt_entry * dev_tree_get_entry_ptr(struct dt_table *table)
{
	uint32_t i;
	struct dt_entry *dt_entry_ptr;
	struct dt_entry *latest_dt_entry = NULL;

	dt_entry_ptr = (struct dt_entry *)((char *)table + DEV_TREE_HEADER_SIZE);

	for(i = 0; i < table->num_entries; i++)
	{
		/* DTBs are stored in the ascending order of soc revision.
		 * For eg: Rev0..Rev1..Rev2 & so on.
		 * we pickup the DTB with highest soc rev number which is less
		 * than or equal to actual hardware
		 */
		if((dt_entry_ptr->platform_id == board_platform_id()) &&
		   (dt_entry_ptr->variant_id == board_hardware_id()) &&
		   (dt_entry_ptr->soc_rev == board_soc_version()))
			{
				return dt_entry_ptr;
			}
		/* if the exact match not found, return the closest match
		 * assuming it to be the nearest soc version
		 */
		if((dt_entry_ptr->platform_id == board_platform_id()) &&
		  (dt_entry_ptr->variant_id == board_hardware_id()) &&
		  (dt_entry_ptr->soc_rev <= board_soc_version())) {
			latest_dt_entry = dt_entry_ptr;
		}
		dt_entry_ptr++;
	}

	if (latest_dt_entry) {
		dprintf(SPEW, "Loading DTB with SOC version:%x\n", latest_dt_entry->soc_rev);
		return latest_dt_entry;
	}

	return NULL;
}

/* Function to add the first RAM partition info to the device tree.
 * Note: The function replaces the reg property in the "/memory" node
 * with the addr and size provided.
 */
int dev_tree_add_first_mem_info(uint32_t *fdt, uint32_t offset, uint32_t addr, uint32_t size)
{
	int ret;

	ret = fdt_setprop_u32(fdt, offset, "reg", addr);

	if (ret)
	{
		dprintf(CRITICAL, "Failed to add the memory information addr: %d\n",
				ret);
	}


	ret = fdt_appendprop_u32(fdt, offset, "reg", size);

	if (ret)
	{
		dprintf(CRITICAL, "Failed to add the memory information size: %d\n",
				ret);
	}

	return ret;
}

/* Function to add the subsequent RAM partition info to the device tree. */
int dev_tree_add_mem_info(void *fdt, uint32_t offset, uint32_t addr, uint32_t size)
{
	static int mem_info_cnt = 0;
	int ret;

	if (!mem_info_cnt)
	{
		/* Replace any other reg prop in the memory node. */
		ret = fdt_setprop_u32(fdt, offset, "reg", addr);
		mem_info_cnt = 1;
	}
	else
	{
		/* Append the mem info to the reg prop for subsequent nodes.  */
		ret = fdt_appendprop_u32(fdt, offset, "reg", addr);
	}

	if (ret)
	{
		dprintf(CRITICAL, "Failed to add the memory information addr: %d\n",
				ret);
	}


	ret = fdt_appendprop_u32(fdt, offset, "reg", size);

	if (ret)
	{
		dprintf(CRITICAL, "Failed to add the memory information size: %d\n",
				ret);
	}

	return ret;
}

static int pinctrl_get_fdt_multi_property(const void *fdt, const int nodeoffset, const char *name, int *property_val, int *property_len)
{
        int lenp, result = -1;
        uint32_t *property;

        property = fdt_getprop(fdt, nodeoffset, name, &lenp);
        if(property < 1) {
                printf("error tcc_fdt_get_multi_property (%d)\r\n", property);
                goto END_PROCESS;
        }

        lenp >>= 2;     // lenp / sizeof(uint32_t)

        if(lenp < 1) {
                result = -1;
                printf("error tcc_fdt_get_multi_property invalid length \r\n");
                goto END_PROCESS;
        }


        if(!property_val || !property_len || lenp != *property_len) {
                result = -1;
                if(property_len) *property_len = lenp;
                printf("error tcc_fdt_get_multi_property invalid parameter\r\n");
                goto END_PROCESS;
        }

        *property_len = lenp;

        while(lenp--) {
                *property_val++ = *property++;

        }
        result = 0;

END_PROCESS:
        return result;

}


/* Top level function that updates the device tree. */
int update_device_tree(void *fdt, const char *cmdline,
					   void *ramdisk, uint32_t ramdisk_size)
{
	int ret = 0;
	uint32_t offset;
	
	void* property;
	uint32_t phandle;
	int property3_val[3]={0,};
	int property1_val[1]={0};
	int property_len, rootnode, subnode, pinctrlnode, nodeoffset, initnode;
	uint32_t len, lenp;


	/* Check the device tree header */
	ret = fdt_check_header(fdt);
	if (ret)
	{
		dprintf(CRITICAL, "Invalid device tree header \n");
		return ret;
	}

	/* Add padding to make space for new nodes and properties. */
	ret = fdt_open_into(fdt, fdt, fdt_totalsize(fdt) + DTB_PAD_SIZE);

	
	if (ret!= 0)
	{
		dprintf(CRITICAL, "Failed to move/resize dtb buffer: %d\n", ret);
		return ret;
	}

	/* Get offset of the memory node */
	ret = fdt_path_offset(fdt, "/memory");
	if (ret < 0)
	{
		dprintf(CRITICAL, "Could not find memory node.\n");
		return ret;
	}

	offset = ret;

	/* Get offset of the chosen node */
	ret = fdt_path_offset(fdt, "/chosen");
	if (ret < 0)
	{
		dprintf(CRITICAL, "Could not find chosen node.\n");
		return ret;
	}

	offset = ret;
	/* Adding the cmdline to the chosen node */
	ret = fdt_setprop_string(fdt, offset, (const char*)"bootargs", (const void*)cmdline);
	if (ret)
	{
		dprintf(CRITICAL, "ERROR: Cannot update chosen node [bootargs]\n");
		return ret;
	}

	/* Adding the initrd-start to the chosen node */
	ret = fdt_setprop_u32(fdt, offset, "linux,initrd-start", (uint32_t)ramdisk);
	if (ret)
	{
		dprintf(CRITICAL, "ERROR: Cannot update chosen node [linux,initrd-start]\n");
		return ret;
	}

	/* Adding the initrd-end to the chosen node */
	ret = fdt_setprop_u32(fdt, offset, "linux,initrd-end", ((uint32_t)ramdisk + ramdisk_size));
	if (ret)
	{
		dprintf(CRITICAL, "ERROR: Cannot update chosen node [linux,initrd-end]\n");
		return ret;
	}
	/*ISDB_T*/
#if defined(INCLUDE_ISDB)
		rootnode = fdt_path_offset(fdt, "/i2c@76320000");
		property_len = 1;
		if(rootnode < 0) {
			dprintf(CRITICAL,"error fdt_path_offset_ISDB_T\n");
			return ret;
		}
		ret = fdt_setprop_u32(fdt, rootnode, "port-mux", 20);
		if(ret < 0){
			dprintf(CRITICAL,"error fdt_setprop_u32_ISDB_T\n");
			return ret;
		}
		ret = pinctrl_get_fdt_multi_property(fdt, rootnode, "port-mux", property1_val, &property_len);
		if(ret < 0){
			printf(CRITICAL,"error_fdt_get_property_isdbt_port_mux\r\n");
			return ret;
		}
		dprintf(INFO, "i2c_isdbt gpio_mux=%d\r\n", fdt32_to_cpu(property1_val[0]));

                //ISDBT_PINCTRL-0
                ret = pinctrl_get_fdt_multi_property(fdt, rootnode, "pinctrl-0", property1_val, &property_len);
                if(ret < 0){
                        printf(CRITICAL,"error_fdt_get_property_pinctrl-0(ISDBT)\r\n");
                        return ret;
               }
/*
dprintf(INFO, "pinctrl-0-ISDBT gpio init[%d]\n",property1_val[0]);
dprintf(INFO, "pinctrl-0-ISDBT gpio val=%d\r\n", fdt32_to_cpu(property1_val[0]));
dprintf(INFO, "pinctrl-0-ISDBT gpio phandle offset=%d\r\n", fdt_node_offset_by_phandle(fdt, fdt32_to_cpu(property1_val[0])));
dprintf(INFO, "pinctrl-0-ISDBT gpio phandle = %d\r\n", fdt_get_phandle(fdt, fdt_node_offset_by_phandle(fdt, fdt32_to_cpu(property1_val[0]))));
*/
               ret = pinctrl_get_fdt_multi_property(fdt, rootnode, "pinctrl-1", property1_val, &property_len);
                if(ret < 0){
                        printf(CRITICAL,"error_fdt_get_property_pinctrl-1(ISDBT)\r\n");
                        return ret;
                }
/*
dprintf(INFO, "pinctrl-0 ISDBT_TEST gpio init[%d]\n",property1_val[0]);
dprintf(INFO, "pinctrl-0 ISDBT_TEST gpio val=%d\r\n", fdt32_to_cpu(property1_val[0]));
dprintf(INFO, "pinctrl-0 ISDBT_TEST gpio phandle offset=%d\r\n", fdt_node_offset_by_phandle(fdt, fdt32_to_cpu(property1_val[0])));
dprintf(INFO, "pinctrl-0 ISDBT_TEST gpio phandle = %d\r\n", fdt_get_phandle(fdt, fdt_node_offset_by_phandle(fdt, fdt32_to_cpu(property1_val[0]))));
*/
	       ret = fdt_setprop(fdt, rootnode, "pinctrl-0", property1_val, sizeof(int)*property_len);
              if(ret < 0){
                         printf(CRITICAL,"error_fdt_set_property_pinctrl-0(ISDBT)\r\n");
                         return ret;
                 }

                ret = pinctrl_get_fdt_multi_property(fdt, rootnode, "pinctrl-0", property1_val, &property_len);
                if(ret < 0){
                        printf(CRITICAL,"error_fdt_get_property_pinctrl-0(ISDBT)\r\n");
                        return ret;
                }
                dprintf(INFO, "pinctrl-0 ISDBT_gpio val=%d\r\n", fdt32_to_cpu(property1_val[0]));
                dprintf(INFO, "pinctrl-0 ISDBT_gpio phandle offset=%d\r\n", fdt_node_offset_by_phandle(fdt, fdt32_to_cpu(property1_val[0])));
                dprintf(INFO, "pinctrl-0 ISDBT_gpio phandle = %d\r\n", fdt_get_phandle(fdt, fdt_node_offset_by_phandle(fdt, fdt32_to_cpu(property1_val[0]))));
#endif
/*
	// Check Platfrom version & Change pin mux
	if((get_daudio_main_ver() == 2) || (get_daudio_main_ver()>=4)) //5th board
	{
		
		
		//TOUCH_I2C		
		rootnode = fdt_path_offset(fdt, "/i2c@76310000");
		if(rootnode < 0) {
			dprintf(CRITICAL,"error fdt_path_offset_I2C_for_TOUCH\n");
			return rootnode;
		}
		//PINCTRL
		pinctrlnode = fdt_path_offset(fdt, "/pinctrl");
		if (pinctrlnode <0) {
			dprintf(CRITICAL,"error fdt_path_offset_pinctrl\n");
			return pinctrlnode;
		}
		property_len = 1;
		//TOUCH PINMUX
		ret = fdt_setprop_u32(fdt, rootnode, "port-mux", 21);
		 if(ret < 0){
                        dprintf(CRITICAL,"error fdt_setprop_u32_TOUCH_PINMUX\n");
                        return ret;
	        }
		ret = pinctrl_get_fdt_multi_property(fdt, rootnode, "port-mux", property1_val, &property_len);
		if(ret < 0){
			printf(CRITICAL,"error_fdt_get_property_touch_port_mux\r\n");
			return ret;
		}
		dprintf(INFO, "i2c_touch gpio_mux=%d\r\n", fdt32_to_cpu(property1_val[0]));

		//TOUCH_PINCTRL-0
		ret = pinctrl_get_fdt_multi_property(fdt, rootnode, "pinctrl-0", property1_val, &property_len);
		if(ret < 0){
	        	printf(CRITICAL,"error_fdt_get_property_pinctrl-0\r\n");
        		return ret;
		}

		dprintf(INFO, "pinctrl-0 i2c_touch_gpio init[%d]\n",property1_val[0]); 
		dprintf(INFO, "pinctrl-0 i2c_touch_gpio val=%d\r\n", fdt32_to_cpu(property1_val[0]));
		dprintf(INFO, "pinctrl-0 i2c_touch_gpio phandle offset=%d\r\n", fdt_node_offset_by_phandle(fdt, fdt32_to_cpu(property1_val[0])));
		dprintf(INFO, "pinctrl-0 i2c_touch_gpio phandle = %d\r\n", fdt_get_phandle(fdt, fdt_node_offset_by_phandle(fdt, fdt32_to_cpu(property1_val[0]))));
		
		ret = pinctrl_get_fdt_multi_property(fdt, rootnode, "pinctrl-1", property1_val, &property_len);
		if(ret < 0){
		        printf(CRITICAL,"error_fdt_get_property_pinctrl-0\r\n");
		        return ret;
		}

		dprintf(INFO, "pinctrl-1 i2c_touch_gpio init[%d]\n",property1_val[0]);
		dprintf(INFO, "pinctrl-1 i2c_tocuh_gpio val=%d\r\n", fdt32_to_cpu(property1_val[0]));
		dprintf(INFO, "pinctrl-1 i2c_touch_gpio phandle offset=%d\r\n", fdt_node_offset_by_phandle(fdt, fdt32_to_cpu(property1_val[0])));
		dprintf(INFO, "pinctrl-1 i2c_touch_gpio phandle = %d\r\n", fdt_get_phandle(fdt, fdt_node_offset_by_phandle(fdt, fdt32_to_cpu(property1_val[0]))));
		
		ret = fdt_setprop(fdt, rootnode, "pinctrl-0", property1_val, sizeof(int)*property_len);
		 if(ret < 0){
			 printf(CRITICAL,"error_fdt_set_property_pinctrl-0(i2c_touch)\r\n");
			 return ret;
		 }	

		ret = pinctrl_get_fdt_multi_property(fdt, rootnode, "pinctrl-0", property1_val, &property_len);
		if(ret < 0){
			printf(CRITICAL,"error_fdt_get_property_pinctrl-0(i2c_touch)\r\n");
			return ret;
		}
		dprintf(INFO, "pinctrl-0 gpio val=%d\r\n", fdt32_to_cpu(property1_val[0]));
		dprintf(INFO, "pinctrl-0 gpio phandle offset=%d\r\n", fdt_node_offset_by_phandle(fdt, fdt32_to_cpu(property1_val[0])));
		dprintf(INFO, "pinctrl-0 gpio phandle = %d\r\n", fdt_get_phandle(fdt, fdt_node_offset_by_phandle(fdt, fdt32_to_cpu(property1_val[0]))));	

		//TOUCH POR-GPIOS
		subnode = fdt_subnode_offset(fdt, rootnode, "mxt336s@4A");
		if(subnode < 0) {
			dprintf(CRITICAL,"error fdt_path_offset_MXT\n");
			return ret;
		}
		property_len = 3;
		ret = pinctrl_get_fdt_multi_property(fdt, subnode, "por-gpios", property3_val, &property_len);
		if(ret < 0){
			printf(CRITICAL,"error_fdt_get_property_touch_por_gpios_MXT\r\n");
			return ret;
		}
		dprintf(INFO, "por-gpios gpio val=%d\r\n", fdt32_to_cpu(property3_val[0]));
		dprintf(INFO, "por-gpios gpio phandle offset=%d\r\n", fdt_node_offset_by_phandle(fdt, fdt32_to_cpu(property3_val[0])));
		dprintf(INFO, "por-gpios gpio phandle = %d\r\n", fdt_get_phandle(fdt, fdt_node_offset_by_phandle(fdt, fdt32_to_cpu(property3_val[0]))));
		dprintf(INFO, "por-gpios gpio port=%d\r\n", fdt32_to_cpu(property3_val[1]));
		dprintf(INFO, "por-gpios gpio initial value=%d\r\n", fdt32_to_cpu(property3_val[2]));

		property3_val[1] = cpu_to_fdt32(18);
		ret = fdt_setprop(fdt, subnode, "por-gpios", property3_val, sizeof(int)*property_len);
		if(ret < 0){
			dprintf(CRITICAL,"error fdt_setprop_TOUCH_POR\n");
			return ret;
		}

		ret = pinctrl_get_fdt_multi_property(fdt, subnode, "por-gpios", property3_val, &property_len);
		if(ret < 0){
			printf(CRITICAL,"error_fdt_get_property_touch_por_gpios\r\n");
			return ret;
		}
		dprintf(INFO, "por-gpios touch_gpio val=%d\r\n", fdt32_to_cpu(property3_val[0]));
		dprintf(INFO, "por-gpios touch_gpio phandle offset=%d\r\n", fdt_node_offset_by_phandle(fdt, fdt32_to_cpu(property3_val[0])));
		dprintf(INFO, "por-gpios touch_gpio phandle = %d\r\n", fdt_get_phandle(fdt, fdt_node_offset_by_phandle(fdt, fdt32_to_cpu(property3_val[0]))));
		dprintf(INFO, "por-gpios touch_gpio port=%d\r\n", fdt32_to_cpu(property3_val[1]));
		dprintf(INFO, "por-gpios touch_gpio initial value=%d\r\n", fdt32_to_cpu(property3_val[2]));
		
		//TOUCH_SCL_GPIOS
		ret = pinctrl_get_fdt_multi_property(fdt, subnode, "scl-gpios", property3_val, &property_len);
		if(ret < 0){
			printf(CRITICAL,"error_fdt_get_property_touch_por_gpios_before\r\n");
			return ret;
		}

		dprintf(INFO, "scl-gpios gpio val=%d\r\n", fdt32_to_cpu(property3_val[0]));
		dprintf(INFO, "scl-gpios gpio phandle offset=%d\r\n", fdt_node_offset_by_phandle(fdt, fdt32_to_cpu(property3_val[0])));
		dprintf(INFO, "scl-gpios gpio phandle = %d\r\n", fdt_get_phandle(fdt, fdt_node_offset_by_phandle(fdt, fdt32_to_cpu(property3_val[0]))));
		dprintf(INFO, "scl-gpios gpio port=%d\r\n", fdt32_to_cpu(property3_val[1]));
		dprintf(INFO, "scl-gpios gpio initial value=%d\r\n", fdt32_to_cpu(property3_val[2]));

		nodeoffset = fdt_subnode_offset(fdt, pinctrlnode, "gpf");
		
		property3_val[0] = cpu_to_fdt32(fdt_get_phandle(fdt,nodeoffset));
                property3_val[1] = cpu_to_fdt32(15);
                ret = fdt_setprop(fdt, subnode, "scl-gpios", property3_val, sizeof(int)*property_len);
                if(ret < 0){
                        dprintf(CRITICAL,"error fdt_setprop_mxt_sc-gpios\n");
                        return ret;
                }

		ret = pinctrl_get_fdt_multi_property(fdt, subnode, "scl-gpios", property3_val, &property_len);
		if(ret < 0){
			printf(CRITICAL,"error_fdt_get_property_scl-gpios(mxt)\r\n");
			return ret;
		}
		dprintf(INFO, "scl-gpios gpio(mxt) val=%d\r\n", fdt32_to_cpu(property3_val[0]));
                dprintf(INFO, "scl-gpios gpio(mxt) phandle offset=%d\r\n", fdt_node_offset_by_phandle(fdt, fdt32_to_cpu(property3_val[0])));
                dprintf(INFO, "scl-gpios gpio(mxt) phandle = %d\r\n", fdt_get_phandle(fdt, fdt_node_offset_by_phandle(fdt, fdt32_to_cpu(property3_val[0]))));
                dprintf(INFO, "scl-gpios gpio(mxt) port=%d\r\n", fdt32_to_cpu(property3_val[1]));
                dprintf(INFO, "scl-gpios gpio(mxt) initial value=%d\r\n", fdt32_to_cpu(property3_val[2]));

		//TOUCH_SDA_GPIOS
		ret = pinctrl_get_fdt_multi_property(fdt, subnode, "sda-gpios", property3_val, &property_len);
		if(ret < 0){
			printf(CRITICAL,"error_fdt_get_property_touch_por_gpios_before\r\n");
			return ret;
		}

		dprintf(INFO, "sda-gpios mxt_gpio val=%d\r\n", fdt32_to_cpu(property3_val[0]));
		dprintf(INFO, "sda-gpios mxt_gpio phandle offset=%d\r\n", fdt_node_offset_by_phandle(fdt, fdt32_to_cpu(property3_val[0])));
		dprintf(INFO, "sda-gpios mxt_gpio phandle = %d\r\n", fdt_get_phandle(fdt, fdt_node_offset_by_phandle(fdt, fdt32_to_cpu(property3_val[0]))));
		dprintf(INFO, "sda-gpios mxt_gpio port=%d\r\n", fdt32_to_cpu(property3_val[1]));
		dprintf(INFO, "sda-gpios mxt_gpio initial value=%d\r\n", fdt32_to_cpu(property3_val[2]));

		nodeoffset = fdt_subnode_offset(fdt, pinctrlnode, "gpf");

		property3_val[0] = cpu_to_fdt32(fdt_get_phandle(fdt,nodeoffset));
		property3_val[1] = cpu_to_fdt32(16);
		ret = fdt_setprop(fdt, subnode, "sda-gpios", property3_val, sizeof(int)*property_len);
		if(ret < 0){
			dprintf(CRITICAL,"error fdt_setprop_sda_gpio(mxt)\n");
			return ret;
		}

		ret = pinctrl_get_fdt_multi_property(fdt, subnode, "sda-gpios", property3_val, &property_len);
		if(ret < 0){
			printf(CRITICAL,"error_fdt_get_property_sda_gpios(mxt)\r\n");
			return ret;
		}
		dprintf(INFO, "sda-gpios touch_gpio(mxt) val=%d\r\n", fdt32_to_cpu(property3_val[0]));
		dprintf(INFO, "sda-gpios touch_gpio(mxt) phandle offset=%d\r\n", fdt_node_offset_by_phandle(fdt, fdt32_to_cpu(property3_val[0])));
		dprintf(INFO, "sda-gpios touch_gpio(mxt) phandle = %d\r\n", fdt_get_phandle(fdt, fdt_node_offset_by_phandle(fdt, fdt32_to_cpu(property3_val[0]))));
		dprintf(INFO, "sda-gpios touch_gpio(mxt) port=%d\r\n", fdt32_to_cpu(property3_val[1]));
		dprintf(INFO, "sda-gpios touch_gpio(mxt) initial value=%d\r\n", fdt32_to_cpu(property3_val[2]));
		
		//IPOD DRM PINMUX
		rootnode = fdt_path_offset(fdt, "/i2c@76300000");
		property_len = 1;
		if(rootnode < 0) {
			dprintf(CRITICAL,"error fdt_path_offset_IPOD\n");
			return ret;
		}
		ret = fdt_setprop_u32(fdt, rootnode, "port-mux", 22);
		if(ret < 0){
			dprintf(CRITICAL,"error fdt_setprop_u32_IPOD\n");
			return ret;
		}
		ret = pinctrl_get_fdt_multi_property(fdt, rootnode, "port-mux", property1_val, &property_len);
		if(ret < 0){
			printf(CRITICAL,"error_fdt_get_property_ipod_port_mux\r\n");
			return ret;
		}
		dprintf(INFO, "i2c_ipod gpio_mux=%d\r\n", fdt32_to_cpu(property1_val[0]));

                //IPOD_PINCTRL-0
                ret = pinctrl_get_fdt_multi_property(fdt, rootnode, "pinctrl-0", property1_val, &property_len);
                if(ret < 0){
                        printf(CRITICAL,"error_fdt_get_property_pinctrl-0(ipod)\r\n");
                        return ret;
                }

                dprintf(INFO, "pinctrl-0-ipod gpio init[%d]\n",property1_val[0]);
                dprintf(INFO, "pinctrl-0-ipod gpio val=%d\r\n", fdt32_to_cpu(property1_val[0]));
                dprintf(INFO, "pinctrl-0-ipod gpio phandle offset=%d\r\n", fdt_node_offset_by_phandle(fdt, fdt32_to_cpu(property1_val[0])));
                dprintf(INFO, "pinctrl-0-ipod gpio phandle = %d\r\n", fdt_get_phandle(fdt, fdt_node_offset_by_phandle(fdt, fdt32_to_cpu(property1_val[0]))));

                ret = pinctrl_get_fdt_multi_property(fdt, rootnode, "pinctrl-1", property1_val, &property_len);
                if(ret < 0){
                        printf(CRITICAL,"error_fdt_get_property_pinctrl-1(ipod)\r\n");
                        return ret;
                }

                dprintf(INFO, "pinctrl-0 IPOD_TEST gpio init[%d]\n",property1_val[0]);
                dprintf(INFO, "pinctrl-0 IPOD_TEST gpio val=%d\r\n", fdt32_to_cpu(property1_val[0]));
                dprintf(INFO, "pinctrl-0 IPOD_TEST gpio phandle offset=%d\r\n", fdt_node_offset_by_phandle(fdt, fdt32_to_cpu(property1_val[0])));
                dprintf(INFO, "pinctrl-0 IPOD_TEST gpio phandle = %d\r\n", fdt_get_phandle(fdt, fdt_node_offset_by_phandle(fdt, fdt32_to_cpu(property1_val[0]))));

                ret = fdt_setprop(fdt, rootnode, "pinctrl-0", property1_val, sizeof(int)*property_len);
                 if(ret < 0){
                         printf(CRITICAL,"error_fdt_set_property_pinctrl-0(ipod_drm)\r\n");
                         return ret;
                 }

                ret = pinctrl_get_fdt_multi_property(fdt, rootnode, "pinctrl-0", property1_val, &property_len);
                if(ret < 0){
                        printf(CRITICAL,"error_fdt_get_property_pinctrl-0(ipod_drm)\r\n");
                        return ret;
                }
                dprintf(INFO, "pinctrl-0 ipod_gpio val=%d\r\n", fdt32_to_cpu(property1_val[0]));
                dprintf(INFO, "pinctrl-0 ipod_gpio phandle offset=%d\r\n", fdt_node_offset_by_phandle(fdt, fdt32_to_cpu(property1_val[0])));
                dprintf(INFO, "pinctrl-0 ipod_gpio phandle = %d\r\n", fdt_get_phandle(fdt, fdt_node_offset_by_phandle(fdt, fdt32_to_cpu(property1_val[0]))));          


		//CAMERA
		property_len = 3;
		rootnode = fdt_path_offset(fdt, "/camera");
		if(rootnode < 0) {
			dprintf(CRITICAL, "error fdt_path_offset_camera\n");
			return ret;
		}
		ret = pinctrl_get_fdt_multi_property(fdt, rootnode, "rst-gpios", property3_val, &property_len);
 		if(ret < 0){
		         printf(CRITICAL,"error_fdt_get_property_CAMERA\r\n");
		         return ret;
 		 }

		 dprintf(INFO, "rst-gpios camera_gpio val=%d\r\n", fdt32_to_cpu(property3_val[0]));
		 dprintf(INFO, "rst-gpios camera_gpio phandle offset=%d\r\n", fdt_node_offset_by_phandle(fdt, fdt32_to_cpu(property3_val[0])));
		 dprintf(INFO, "rst-gpios camera_gpio phandle = %d\r\n", fdt_get_phandle(fdt, fdt_node_offset_by_phandle(fdt, fdt32_to_cpu(property3_val[0]))));
		 dprintf(INFO, "rst-gpios camera_gpio port=%d\r\n", fdt32_to_cpu(property3_val[1]));
		 dprintf(INFO, "rst-gpios camera_gpio initial value=%d\r\n", fdt32_to_cpu(property3_val[2]));

 		 property3_val[1] = cpu_to_fdt32(16);
		 ret = fdt_setprop(fdt, rootnode, "rst-gpios", property3_val, sizeof(int)*property_len);
		 if(ret < 0){
		         dprintf(CRITICAL,"error fdt_setprop_CAMERA\n");
		         return ret;
		 }

	         ret = pinctrl_get_fdt_multi_property(fdt, rootnode, "rst-gpios", property3_val, &property_len);
	         if(ret < 0){
	                 printf(CRITICAL,"error_fdt_get_property_rst-gpios(camera)\r\n");
        	         return ret;
         	}
		 dprintf(INFO, "rst-gpios camera_gpio val=%d\r\n", fdt32_to_cpu(property3_val[0]));
		 dprintf(INFO, "rst-gpios camera_gpio phandle offset=%d\r\n", fdt_node_offset_by_phandle(fdt, fdt32_to_cpu(property3_val[0])));
		 dprintf(INFO, "rst-gpios camera_gpio phandle = %d\r\n", fdt_get_phandle(fdt, fdt_node_offset_by_phandle(fdt, fdt32_to_cpu(property3_val[0]))));
		 dprintf(INFO, "rst-gpios camera_gpio port=%d\r\n", fdt32_to_cpu(property3_val[1]));
		 dprintf(INFO, "rst-gpios camera_gpio initial value=%d\r\n", fdt32_to_cpu(property3_val[2]));
#if defined(DAUDIO_KK_3HW_GPIO)
		 //TW9990_ATV
		 subnode = fdt_subnode_offset(fdt, rootnode, "tw9990_atv");
		 if(subnode < 0) {
		         dprintf(CRITICAL,"error fdt_path_offset_tw9990_atv(camera)\n");
		         return ret;
		 }
	        ret = pinctrl_get_fdt_multi_property(fdt, subnode, "rst-gpios", property3_val, &property_len);
		if(ret < 0){
         		printf(CRITICAL,"error_fdt_get_property_rst-gpios(tw9990_atv_camera)\r\n");
         		return ret;
		 }

 		dprintf(INFO, "rst-gpios tw9990_gpio(camera) val=%d\r\n", fdt32_to_cpu(property3_val[0]));
		 dprintf(INFO, "rst-gpios tw9990_gpio(camera) phandle offset=%d\r\n", fdt_node_offset_by_phandle(fdt, fdt32_to_cpu(property3_val[0])));
		 dprintf(INFO, "rst-gpios tw9990_gpio(camera) phandle = %d\r\n", fdt_get_phandle(fdt, fdt_node_offset_by_phandle(fdt, fdt32_to_cpu(property3_val[0]))));
		 dprintf(INFO, "rst-gpios tw9990_gpio(camera) port=%d\r\n", fdt32_to_cpu(property3_val[1]));
		 dprintf(INFO, "rst-gpios tw9990_gpio(camera) initial value=%d\r\n", fdt32_to_cpu(property3_val[2]));

		 property3_val[1] = cpu_to_fdt32(16);
		 ret = fdt_setprop(fdt, subnode, "rst-gpios", property3_val, sizeof(int)*property_len);
		 if(ret < 0){
		         dprintf(CRITICAL,"error fdt_setprop_CAMERA_TW9990\n");
		         return ret;
		 }

		 ret = pinctrl_get_fdt_multi_property(fdt, subnode, "rst-gpios", property3_val, &property_len);
		 if(ret < 0){
	        	 printf(CRITICAL,"error_fdt_get_property_tw9990_rst_gpios\r\n");
	        	 return ret;
		}
		 dprintf(INFO, "rst-gpios tw9990_gpio(camera) val=%d\r\n", fdt32_to_cpu(property3_val[0]));
		 dprintf(INFO, "rst-gpios tw9990_gpio(camera) phandle offset=%d\r\n", fdt_node_offset_by_phandle(fdt, fdt32_to_cpu(property3_val[0])));
		 dprintf(INFO, "rst-gpios tw9990_gpio(camera) phandle = %d\r\n", fdt_get_phandle(fdt, fdt_node_offset_by_phandle(fdt, fdt32_to_cpu(property3_val[0]))));
		 dprintf(INFO, "rst-gpios tw9990_gpio(camera) port=%d\r\n", fdt32_to_cpu(property3_val[1]));
		 dprintf(INFO, "rst-gpios tw9990_gpio(camera) initial value=%d\r\n", fdt32_to_cpu(property3_val[2])); 	
#endif
		//LVDS_CAMERA
		rootnode = fdt_path_offset(fdt, "/lvds_camera");
		if(rootnode < 0) {
			dprintf(CRITICAL, "error fdt_path_offset_lvds\n");
			return ret;
		}
		ret = pinctrl_get_fdt_multi_property(fdt, rootnode, "rst-gpios", property3_val, &property_len);
		if(ret < 0){
		         printf(CRITICAL,"error_fdt_get_property_rst_gpios_lvds\r\n");
		         return ret;
		 }

		dprintf(INFO, "rst-gpios gpio val=%d\r\n", fdt32_to_cpu(property3_val[0]));
		dprintf(INFO, "rst-gpios gpio phandle offset=%d\r\n", fdt_node_offset_by_phandle(fdt, fdt32_to_cpu(property3_val[0])));
		dprintf(INFO, "rst-gpios gpio phandle = %d\r\n", fdt_get_phandle(fdt, fdt_node_offset_by_phandle(fdt, fdt32_to_cpu(property3_val[0]))));
		dprintf(INFO, "rst-gpios gpio port=%d\r\n", fdt32_to_cpu(property3_val[1]));
		dprintf(INFO, "rst-gpios gpio initial value=%d\r\n", fdt32_to_cpu(property3_val[2]));

		property3_val[1] = cpu_to_fdt32(16);
		ret = fdt_setprop(fdt, rootnode, "rst-gpios", property3_val, sizeof(int)*property_len);
		if(ret < 0){
			dprintf(CRITICAL,"error fdt_setprop_LVDS\n");
			return ret;
		 }	

		ret = pinctrl_get_fdt_multi_property(fdt, rootnode, "rst-gpios", property3_val, &property_len);
		if(ret < 0){
			printf(CRITICAL,"error_fdt_get_property_rst_gpios_lvds\r\n");
			return ret;
		}
		dprintf(INFO, "rst-gpios lvds_gpio val=%d\r\n", fdt32_to_cpu(property3_val[0]));
		dprintf(INFO, "rst-gpios lvds_gpio phandle offset=%d\r\n", fdt_node_offset_by_phandle(fdt, fdt32_to_cpu(property3_val[0])));
		dprintf(INFO, "rst-gpios lvds_gpio phandle = %d\r\n", fdt_get_phandle(fdt, fdt_node_offset_by_phandle(fdt, fdt32_to_cpu(property3_val[0]))));
		dprintf(INFO, "rst-gpios lvds_gpio port=%d\r\n", fdt32_to_cpu(property3_val[1]));
		dprintf(INFO, "rst-gpios lvds_gpio initial value=%d\r\n", fdt32_to_cpu(property3_val[2]));
#if defined(DAUDIO_KK_3HW_GPIO)
		   //TW9990_ATV
                 subnode = fdt_subnode_offset(fdt, rootnode, "tw9990_atv");
                 if(subnode < 0) {
                         dprintf(CRITICAL,"error fdt_path_offset_tw9990_atv(lvds)\n");
                         return ret;
                 }
                ret = pinctrl_get_fdt_multi_property(fdt, subnode, "rst-gpios", property3_val, &property_len);
                if(ret < 0){
                        printf(CRITICAL,"error_fdt_get_property_tw9990_rst_gpios_lvds\r\n");
                        return ret;
                 }

                 dprintf(INFO, "rst-gpios tw9990-gpio(lvds) val=%d\r\n", fdt32_to_cpu(property3_val[0]));
                 dprintf(INFO, "rst-gpios tw9990-gpio(lvds) phandle offset=%d\r\n", fdt_node_offset_by_phandle(fdt, fdt32_to_cpu(property3_val[0])));
                 dprintf(INFO, "rst-gpios tw9990-gpio(lvds) phandle = %d\r\n", fdt_get_phandle(fdt, fdt_node_offset_by_phandle(fdt, fdt32_to_cpu(property3_val[0]))));
                 dprintf(INFO, "rst-gpios tw9990-gpio(lvds) port=%d\r\n", fdt32_to_cpu(property3_val[1]));
                 dprintf(INFO, "rst-gpios tw9990-gpio(lvds) initial value=%d\r\n", fdt32_to_cpu(property3_val[2]));

                 property3_val[1] = cpu_to_fdt32(16);
                 ret = fdt_setprop(fdt, subnode, "rst-gpios", property3_val, sizeof(int)*property_len);
                 if(ret < 0){
                         dprintf(CRITICAL,"error fdt_setprop_LVDS_TW9990\n");
                         return ret;
                 }

                 ret = pinctrl_get_fdt_multi_property(fdt, subnode, "rst-gpios", property3_val, &property_len);
                 if(ret < 0){
                         printf(CRITICAL,"error_fdt_get_property_tw9990_rst_gpios(lvds)\r\n");
                         return ret;
                }
                 dprintf(INFO, "rst-gpios tw9990_gpio(lvds) val=%d\r\n", fdt32_to_cpu(property3_val[0]));
                 dprintf(INFO, "rst-gpios tw9990_gpio(lvds) phandle offset=%d\r\n", fdt_node_offset_by_phandle(fdt, fdt32_to_cpu(property3_val[0])));
                 dprintf(INFO, "rst-gpios tw9990_gpio(lvds) phandle = %d\r\n", fdt_get_phandle(fdt, fdt_node_offset_by_phandle(fdt, fdt32_to_cpu(property3_val[0]))));
                 dprintf(INFO, "rst-gpios tw9990_gpio(lvds) port=%d\r\n", fdt32_to_cpu(property3_val[1]));
                 dprintf(INFO, "rst-gpios tw9990_gpio(lvds) initial value=%d\r\n", fdt32_to_cpu(property3_val[2]));
#endif     
	}
*/

	fdt_pack(fdt);

	return ret;
}




/*
 * Copyright (C) 2016 Telechips Coperation
 * Telechips <telechips@telechips.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <command.h>
#include <memalign.h>
#include <linux/types.h>
#include <compiler.h>
#include <part.h>
#include <image.h>
#include <libfdt_env.h>
#include <fdt.h>

#include <android_image.h>

static unsigned int page_size;


static int do_parse_fdt_header(block_dev_desc_t *dev_desc, disk_partition_t info)
{
	struct fdt_header *h_fdt;
	u32 n;

	unsigned  int dtb_size = 0;

	h_fdt = malloc(BLOCK_CNT(sizeof(struct fdt_header), dev_desc)
			* dev_desc->blksz);

	n = dev_desc->block_read(dev_desc->dev, info.start,
			BLOCK_CNT(sizeof(struct fdt_header),
				dev_desc), (void*)h_fdt);

	debug("Device Tree Blob Size : %x , nblocks : %d \n",
			fdt32_to_cpu(h_fdt->totalsize), n);

	dtb_size = fdt32_to_cpu(h_fdt->totalsize);

	free(h_fdt);

	return dtb_size;

}

static int do_parse_kernel_header(block_dev_desc_t *dev_desc, disk_partition_t info)
{

	unsigned int kernel_size = 0;
	u32 n;
#ifdef CONFIG_ANDROID_BOOT_IMAGE
	struct andr_img_hdr *hdr_t;

	hdr_t = malloc(BLOCK_CNT(sizeof(struct andr_img_hdr), dev_desc)
			* dev_desc->blksz);

	n = dev_desc->block_read(dev_desc->dev, info.start,
			BLOCK_CNT(sizeof(struct andr_img_hdr), dev_desc),
			(void*)hdr_t);

	if(memcmp(hdr_t->magic, ANDR_BOOT_MAGIC, 8)){
		printf(" Not Android Image  \n");
		return -1;
	}


	kernel_size = (unsigned int)hdr_t->kernel_size;
	page_size = (unsigned int)hdr_t->page_size;
#endif

#ifdef CONFIG_U_BOOT_IMAGE
	image_header_t *hdr_t;

	hdr_t = malloc(BLOCK_CNT(sizeof(image_ingo_t), dev_desc)
			* dev_desc->blksz);

	n = dev_desc->block_read(dev_desc->dev, info.start,
			BLOCK_CNT(sizeof(struct andr_img_hdr), dev_desc),
			(void*)hdr_t);

	if(memcmp(hdr_t->ih_magic , IH_MAGIC, sizeof(__be32))){
		printf(" NOt uBoot Image \n");
		return -1;
	}

	kernel_size = (unsigned int)hdr_t->ih_size;
#endif


#ifdef CONFIG_GZIP_BOOT_IMAGE
	unsigned int *hdr_t;
	hdr_t = malloc(sizeof(char) * 512);

	n = dev_desc->block_read(dev_desc->dev, info.start,
			BLOCK_CNT(sizeof(char) * 512 , dev_desc),
			(void*)hdr_t);

	kernel_size = *(hdr_t+11);
#endif

	debug("Kernel Image Size : %x , nBlocks : %d \n",
			kernel_size, n );


	free(hdr_t);

	return kernel_size;

}

static int get_device_and_part(char * const argv[], block_dev_desc_t **dev_desc,
			disk_partition_t *info)
{

	int part, dev;
	const char *part_str;
	const char *dev_str;
	char *dup_str = NULL;

	part_str = strchr(argv[2], ':');
	if(part_str){
		dup_str = strdup(argv[2]);
		dup_str[part_str - argv[2] ] = 0;
		dev_str = dup_str;
		part_str++;
		if(strlen(part_str) < 3)
			return 1;
	}else{
		dev_str = argv[2];
	}

	dev = get_device(argv[1], dev_str, dev_desc);

	if(dev < 0) return -1;

	part = get_partition_info_efi_by_name(*dev_desc, part_str, info);

	if(part < 0) return -2;

	return 0;

}


static int do_load_dtb(cmd_tbl_t *cmdtp, int flag,
			int argc, char * const argv[])
{
	block_dev_desc_t *dev_desc;
	disk_partition_t info;
	u32 addr, size, n;

	int part, ret=0;

	ret = get_device_and_part(argv, &dev_desc, &info);

	if(ret > 0){
		part = get_device_and_partition(argv[1], argv[2], &dev_desc, &info, 1);
		if(part < 0) return 0;
	}
	else if (ret < 0) return 0;


	/* load Device Tree Blob Start */
	addr = simple_strtoul(argv[3], NULL,16);
	size = do_parse_fdt_header(dev_desc, info);

	n = dev_desc->block_read(dev_desc->dev, info.start,
			BLOCK_CNT(size , dev_desc), (void*)addr);
	return n;
}

static int do_load_kernel(cmd_tbl_t *cmdtp, int flag,
			int argc, char * const argv[])
{
	block_dev_desc_t *dev_desc;
	disk_partition_t info;
	u32 addr,  n;
	int part, ret = 0;

	int size;

	ret = get_device_and_part(argv, &dev_desc, &info);

	if(ret > 0){
		part = get_device_and_partition(argv[1], argv[2], &dev_desc, &info, 1);
		if(part < 0) return 0;
	}
	else if (ret < 0) return 0;


	/* load Device Tree Blob Start */
	addr = simple_strtoul(argv[3], NULL,16);
	size = do_parse_kernel_header(dev_desc, info);

	if(size <= 0) return -1;

#ifdef CONFIG_ANDROID_BOOT_IMAGE
	n = dev_desc->block_read(dev_desc->dev, (info.start + BLOCK_CNT(page_size, dev_desc)),
			BLOCK_CNT(size, dev_desc), (void*)addr);
#else
	n = dev_desc->block_read(dev_desc->dev, info.start,
			BLOCK_CNT(size, dev_desc), (void*)addr);
#endif

	return n;

}

static int do_readpart(cmd_tbl_t *cmdtp, int flag,
			int argc, char * const argv[])
{

	block_dev_desc_t *dev_desc;
	disk_partition_t info;
	u32 addr, size, n;
	u32 start_sec;

	if(get_device_and_part(argv, &dev_desc, &info) < 0)
		return 0;

	addr = simple_strtoul(argv[3], NULL,16);

	start_sec = simple_strtoul(argv[4], NULL, 16);

	size = simple_strtoul(argv[5], NULL, 16);

	n = dev_desc->block_read(dev_desc->dev, info.start + start_sec,
			BLOCK_CNT(size, dev_desc), (void*)addr);

	return n;
}


static cmd_tbl_t cmd_tccload_sub[] = {

	U_BOOT_CMD_MKENT(dtb, 4, 0, do_load_dtb, "",""),
	U_BOOT_CMD_MKENT(kernel, 4, 0, do_load_kernel,"",""),
	U_BOOT_CMD_MKENT(part, 6, 0, do_readpart,"",""),
};


static int do_tccload (cmd_tbl_t *cmdtp, int flag, int argc,
			char * const argv[])
{
	cmd_tbl_t *cp;

	cp = find_cmd_tbl(argv[1], cmd_tccload_sub, ARRAY_SIZE(cmd_tccload_sub));

	/* Drop the tcc_load command */
	argc--; argv++;


	if(cp == NULL || argc > cp->maxargs || argc < cp->maxargs)
		return CMD_RET_USAGE;
	if (flag == CMD_FLAG_REPEAT && !cp->repeatable)
		return CMD_RET_SUCCESS;

	return cp->cmd(cmdtp, flag, argc, argv);

}

U_BOOT_CMD(
	tcc_load,	8,	1,	do_tccload,
	"Telechps Image Load SubSystem",
	"load boot image form dev partition\n"
	" - tcc_load kernel <interface> [<dev[:part]>] <addr> <bytes<hex>>\n"
	"load DTB form dev partition\n"
	" - tcc_load dtb <interface> [<dev[:part]>] <addr> <bytes<hex>>\n"
	"read raw data form dev partition\n"
	" - tcc_load part <interface> [<dev[:part]>] <addr> <start_sector> <bytes<hex>>\n"
	"load image  and raw data form dev on interface"
);



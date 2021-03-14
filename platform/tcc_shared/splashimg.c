
#include <debug.h>
#include <string.h>
#include <dev/flash.h>
#include <lib/ptable.h>
#include <dev/fbcon.h>
#include <splash/splashimg.h>

#include <tcc_lcd.h>

#ifdef BOOTSD_INCLUDE
#include <fwdn/Disk.h>
#include <sdmmc/sd_memory.h>
#include <sdmmc/emmc.h>
#endif

#include <partition_parser.h>
#include <sfl.h>
#include <bitmap.h>

static char *splash_image_names[] =
{
	"bootlogo",		//BOOT_LOGO
	"pg001",		//PARKING_GUIDE_LOGO
	"quickboot",	//MAKE_QUICKBOOT_LOGO
	"bluelink",		//BLUELINK BOOT LOGO
	"reserved",		//RESERVED
	"error",
};

#define ROUND_TO_PAGE(x,y) (((x) + (y)) & (~(y)))
#define BYTE_TO_SECTOR(x) (x)/512

extern unsigned int flash_read_tnftl_v8(unsigned long long data_addr , unsigned data_len , void* in);
extern int display_splash_logo(struct fbcon_config *fb_con);

unsigned char *splash[16384];

int jpu_c6_decoder( jpu_dec_inits_t* init_jpu, jpu_dec_outputs_t* out_jpu)
{
       unsigned int ret = 0;
       int dec_handle = 0;

       jpu_dec_init_t dec_init = {0,};
       jpu_dec_initial_info_t dec_initial_info = {0,};
       jpu_dec_input_t dec_input = {0,};
       jpu_dec_output_t dec_output = {0,};
       jpu_dec_buffer_t dec_buffer = { 0 };
       unsigned int frame_buf_addr[2] = {0,};
       unsigned int bitstream_buf_addr[2] = {0,};

       dec_init.m_iBitstreamBufSize = 0x100000;
       dec_init.m_RegBaseVirtualAddr = init_jpu->register_base_virtual_address;
       dec_init.m_BitstreamBufAddr[PHYSICAL_ADDR] = init_jpu->memory_base_physical_address;
       dec_init.m_BitstreamBufAddr[VIRTUAL_ADDR] = init_jpu->memory_base_virtual_address;
       dec_init.m_iCbCrInterleaveMode = 0;
       dec_init.m_uiDecOptFlags = 0;

       dec_init.m_Memcpy        = init_jpu->m_Memcpy;
       dec_init.m_Memset        = init_jpu->m_Memset;
       dec_init.m_Interrupt = init_jpu->m_Interrupt;

       ret = TCC_JPU_DEC( JPU_DEC_INIT, &dec_handle, &dec_init, NULL );
       if( ret != JPG_RET_SUCCESS )
       {
               dprintf(INFO, "[dec_test] JPU_DEC_INIT failed Error code is 0x%x \r\n", ret );
//             dprintf(INFO, "[dec_test] JPU_DEC_INIT failed detail errod  code is 0x%x \r\n", dec_initial_info.m_error_reason);
               goto ERR_DEC_OPEN;
       }
       //PRINTF( "[dec_test] YUV format :  %d \r\n", dec_initial_info.m_iJpg_sourceFormat );
       //PRINTF("TCC_JPU_DEC JPU_DEC_INIT OK!\r\n");

       ret = TCC_JPU_DEC( JPU_DEC_SEQ_HEADER, &dec_handle, (void*)init_jpu->input_stream_size, &dec_initial_info );
       if( ret != JPG_RET_SUCCESS )
       {
               dprintf(INFO, "[dec_test] JPU_DEC_SEQ_HEADER failed Error code is 0x%x, error reason is 0x%x \r\n", ret, dec_initial_info.m_iErrorReason );
               goto ERR_DEC_OPEN;
       }

       out_jpu->m_iYUVFormat = dec_initial_info.m_iSourceFormat;

       frame_buf_addr[PHYSICAL_ADDR] = init_jpu->memory_base_physical_address + 0x100000;
       if( frame_buf_addr[PHYSICAL_ADDR] == 0 )
       {
               dprintf(INFO, "frame_buf_addr[PA] malloc() failed \r\n");
               goto ERR_DEC_OPEN;
       }
       //PRINTF("frame_buf_addr[PA] = 0x%x\r\n", (codec_addr_t)frame_buf_addr[PA]);

       frame_buf_addr[VIRTUAL_ADDR] = init_jpu->memory_base_virtual_address+0x100000;
       if( frame_buf_addr[VIRTUAL_ADDR] == 0 )
       {
               dprintf(INFO, "frame_buf_addr[VIRTUAL_ADDR] malloc() failed \r\n");
               goto ERR_DEC_OPEN;
       }
       //PRINTF("frame_buf_addr[VIRTUAL_ADDR] = 0x%x\r\n", (codec_addr_t)frame_buf_addr[VIRTUAL_ADDR]);

       dec_buffer.m_FrameBufferStartAddr[PHYSICAL_ADDR] = frame_buf_addr[PHYSICAL_ADDR];
       dec_buffer.m_FrameBufferStartAddr[VIRTUAL_ADDR] = frame_buf_addr[VIRTUAL_ADDR];
       dec_buffer.m_iFrameBufferCount = dec_initial_info.m_iMinFrameBufferCount;
       dec_buffer.m_iJPGScaleRatio = 0;

       ret = TCC_JPU_DEC( JPU_DEC_REG_FRAME_BUFFER, &dec_handle, &dec_buffer, 0 );
       if( ret != JPG_RET_SUCCESS )
       {
               dprintf(INFO, "JPU_DEC_REG_FRAME_BUFFER failed Error code is 0x%x \r\n\n", ret);
               goto ERR_DEC_OPEN;
       }
       //PRINTF("TCC_JPU_DEC JPU_DEC_REG_FRAME_BUFFER OK!\r\n");


       dec_input.m_BitstreamDataAddr[PHYSICAL_ADDR] = init_jpu->memory_base_physical_address;
       dec_input.m_BitstreamDataAddr[VIRTUAL_ADDR] = init_jpu->memory_base_virtual_address;
       dec_input.m_iBitstreamDataSize = init_jpu->input_stream_size;

       ret = TCC_JPU_DEC( JPU_DEC_DECODE, &dec_handle, &dec_input, &dec_output );
       if(ret ==JPG_RET_FAILURE) {
               goto ERR_DEC_OPEN;
       }
       if(ret ==JPG_RET_CODEC_EXIT){
               goto ERR_DEC_OPEN;
       }

       out_jpu->m_iWidth = dec_output.m_DecOutInfo.m_iWidth;
       out_jpu->m_iHeight = dec_output.m_DecOutInfo.m_iHeight;
       out_jpu->m_pCurrOut[0][0] = dec_output.m_pCurrOut[0][0];
       out_jpu->m_pCurrOut[0][1] = dec_output.m_pCurrOut[0][1];
       out_jpu->m_pCurrOut[0][2] = dec_output.m_pCurrOut[0][2];
       out_jpu->m_pCurrOut[1][0] = dec_output.m_pCurrOut[1][0];
       out_jpu->m_pCurrOut[1][1] = dec_output.m_pCurrOut[1][1];
       out_jpu->m_pCurrOut[1][2] = dec_output.m_pCurrOut[1][2];

ERR_DEC_OPEN:
       // Now that we are done with decoding, close the open instance.
       ret = TCC_JPU_DEC( JPU_DEC_CLOSE, &dec_handle, 0, 0 );

       return ret;
}

int splash_image_decode_jpeg(unsigned int input_stream_size_, jpu_dec_outputs_t * jpu_out) {

       int ret = 0;

       jpu_dec_inits_t jpu_init = {0,};
//     jpu_dec_outputs_t jpu_out = {0,};
       unsigned int register_base_physical_address = 0x75180000;
       unsigned int register_base_virtual_address = 0x75180000;
       unsigned int memory_base_physical_address = JPEG_DECODE_ADDR;// 0x8cc00000;
       unsigned int memory_base_virtual_address = JPEG_DECODE_ADDR;// 0x8cc00000;
       int input_stream_size = (int)input_stream_size_;        // decoding CO jpeg stream size

//     dprintf(INFO, "==== %s() in \n", __func__);

       memset(jpu_out, 0, sizeof(jpu_dec_outputs_t));

       jpu_init.input_stream_size = input_stream_size;
       jpu_init.register_base_virtual_address = register_base_virtual_address;
       jpu_init.memory_base_physical_address = memory_base_physical_address;
       jpu_init.memory_base_virtual_address = memory_base_virtual_address;
       jpu_init.m_Memset = memset;                             // (void  (*) ( void*, int, unsigned int ))sys_memset;
       jpu_init.m_Memcpy = memcpy;                             // (void* (*) ( void*, const void*, unsigned int ))sys_memcpy;
       jpu_init.m_Interrupt = NULL;                    // (int  (*) ( void ))sys_test;

#if 0
       dprintf(INFO, "==== %s()\n==== input_stream_size(%d) \n==== register_base_virtual_address(0x%x) \n==== memory_base_physical_address(0x%x) \n", \
                       __func__, \
                       jpu_init.input_stream_size, \
                       jpu_init.register_base_virtual_address, \
                       jpu_init.memory_base_physical_address);
#endif

       ret = jpu_c6_decoder(&jpu_init, jpu_out);
       if ( ret != 0 )
               dprintf(INFO, "jpu error : %d\n", ret);

#if 0
       dprintf(INFO, "==== %s() out \n==== width x height(%dx%d), YUV format(%d), m_pCurrOut(0x%x) \n", \
                               __func__, \
                               jpu_out->m_iWidth, jpu_out->m_iHeight, \
                               jpu_out->m_iYUVFormat, \
                               jpu_out->m_pCurrOut[0][0]);
#endif
       return ret;

}

static int get_splash_index(SPLASH_IMAGE_Header_info_t *splash_hdr, char *part)
{
	int idx;

	for (idx = 0; idx < splash_hdr->ulNumber; idx++) {
		dprintf(SPEW, "part name : %s idx : %d\n", splash_hdr->SPLASH_IMAGE[idx].ucImageName, idx);
		if (!strcmp((splash_hdr->SPLASH_IMAGE[idx].ucImageName), part)) {
			return idx;
		}
	}

	return -1;
}

static int splash_image_nand_v8(char *partition){

	SPLASH_IMAGE_Header_info_t *splash_hdr = (void*)splash;
	struct fbcon_config *fb_display = NULL;
	int img_idx = 0;
	unsigned int page_size = flash_page_size();
	//unsigned int page_mask = page_size -1;

	unsigned long long ptn = 0;

    memset(splash_hdr, 0x00, sizeof(SPLASH_IMAGE_Header_info_t));

	dprintf(SPEW, "partition : %s\n", partition);


	ptn = flash_ptn_offset("splash");
	if(ptn == 0){
		dprintf(CRITICAL, "ERROR : No splash partition found !\n");
		return -1;
	}else{

		fb_display = fbcon_display();

		if(fb_display){

			if(!flash_read_tnftl_v8(ptn, page_size, splash_hdr)){
				if(strcmp(splash_hdr->ucPartition, SPLASH_TAG)){
					dprintf(CRITICAL, "Splash TAG Is Mismatched\n");
					return -1;
				}

			}

			img_idx = get_splash_index(splash_hdr, partition);
			dprintf(SPEW, "image idx = %d \n" ,img_idx);


			if(img_idx < 0){
				dprintf(CRITICAL, " there is no image [%s]\n", partition);
				return -1;
			}else{

				if((fb_display->width != splash_hdr->SPLASH_IMAGE[img_idx].ulImageWidth) 
						&& (fb_display->height != splash_hdr->SPLASH_IMAGE[img_idx].ulImageHeight)){

					fb_display->width = splash_hdr->SPLASH_IMAGE[img_idx].ulImageWidth;
					fb_display->height = splash_hdr->SPLASH_IMAGE[img_idx].ulImageHeight;
					display_splash_logo(fb_display);
				}

				if(flash_read_tnftl_v8(ptn+BYTE_TO_SECTOR(splash_hdr->SPLASH_IMAGE[img_idx].ulImageAddr),
							splash_hdr->SPLASH_IMAGE[img_idx].ulImageSize, fb_display->base)){
					fbcon_clear();
				}
			}
		}
	}

	return 0;

}

#if defined(BOOTSD_INCLUDE)
static int splash_image_sdmmc(char *partition){

	SPLASH_IMAGE_Header_info_t *splash_hdr = (void*)splash;
	struct fbcon_config *fb_display = NULL;
	int img_idx = 0;
	unsigned int page_size = flash_page_size();
	//unsigned int page_mask = page_size -1;

	unsigned long long ptn = 0;

	dprintf(SPEW, "partition : %s\n", partition);

	ptn = emmc_ptn_offset("splash");
	if(ptn == 0){
		dprintf(CRITICAL, "ERROR : No splash partition found !\n");
		return -1;
	}else{

		fb_display = fbcon_display();

		if(fb_display){

			if(!emmc_read(ptn, page_size, splash_hdr)){
				if(strcmp(splash_hdr->ucPartition, SPLASH_TAG)){
					dprintf(CRITICAL, "Splash TAG Is Mismatched\n");
					return -1;
				}
			}
			img_idx = get_splash_index(splash_hdr, partition);
			dprintf(SPEW, "image idx = %d \n" ,img_idx);


			if(img_idx < 0){
				dprintf(CRITICAL, " there is no image [%s]\n", partition);
			}else{

				if((fb_display->width != splash_hdr->SPLASH_IMAGE[img_idx].ulImageWidth) 
						&& (fb_display->height != splash_hdr->SPLASH_IMAGE[img_idx].ulImageHeight)){

					fb_display->width = splash_hdr->SPLASH_IMAGE[img_idx].ulImageWidth;
					fb_display->height = splash_hdr->SPLASH_IMAGE[img_idx].ulImageHeight;
					display_splash_logo(fb_display);
				}

				if(emmc_read(ptn + BYTE_TO_SECTOR(splash_hdr->SPLASH_IMAGE[img_idx].ulImageAddr),
							splash_hdr->SPLASH_IMAGE[img_idx].ulImageSize, fb_display->base)){

					fbcon_clear();
				}
			}
		}
	}

	return 0;

}

int splash_image_load_sdmmc(char *partition, struct fbcon_config *fb_cfg)
{

	SPLASH_IMAGE_Header_info_t *splash_hdr = (void*)splash;
	struct fbcon_config *fb_display = NULL;
	int img_idx = 0;
	unsigned int page_size = flash_page_size();
	unsigned int page_mask = page_size -1;

	unsigned long long ptn = 0;
	unsigned int ptn_index;

	dprintf(INFO, "partition : %s\n", partition);

	ptn_index = partition_get_index(partition);
	ptn = partition_get_offset(ptn_index);

	if(ptn == 0){
		dprintf(CRITICAL, "ERROR : No splash partition found !\n");
		return -1;
	}else{

		if(tcc_read(ptn, page_size, splash_hdr)){
			if(strcmp(splash_hdr->ucPartition, SPLASH_TAG)){
				dprintf(CRITICAL, "Splash TAG Is Mismatched\n");
				return -1;
			}
		}

		img_idx = get_splash_index(splash_hdr, partition);
		dprintf(SPEW, "image idx = %d \n" ,img_idx);

		if(img_idx < 0){
			dprintf(CRITICAL, " there is no image from emmc [%s]\n", partition);
		}else{

			fb_cfg->width = splash_hdr->SPLASH_IMAGE[img_idx].ulImageWidth;
			fb_cfg->height = splash_hdr->SPLASH_IMAGE[img_idx].ulImageHeight;
			//fb_display->bpp = splash_hdr->SPLASH_IMAGE[img_idx].fmt

			tcc_read(ptn + BYTE_TO_SECTOR(splash_hdr->SPLASH_IMAGE[img_idx].ulImageAddr),
					splash_hdr->SPLASH_IMAGE[img_idx].ulImageSize, fb_cfg->base);    			    			
		}

	}

	return 0;

}
#endif

#if !defined(TCC_SUPPORT_SPLASH_JPEG)
//for convert D-audio custom splash (bitmap images...)
static int get_splash_image_type(char *name)
{
	int i, size = sizeof(splash_image_names) / sizeof(int);

	for (i = 0; i < size; i++)
	{
		if (strcmp(name, splash_image_names[i]) == 0)
			break;	//success to find image type
	}

	return i;
}

int splash_image_load(char *partition, struct fbcon_config *fb_cfg)
{
	int type = get_splash_image_type(partition);

	dprintf(INFO, "%s image type[%d] %s \n", __func__, type, splash_image_names[type]);

	if (type != IMAGE_TYPE_MAX)
		return bitmap_to_fb(type, fb_cfg);
	else return -1;
}
#else // TCC Original
int splash_image_load(char *partition, struct fbcon_config *fb_cfg) {
	SPLASH_IMAGE_Header_info_t *splash_hdr = (void*)splash;
	//struct fbcon_config *fb_display = NULL;
	int img_idx = 0;
	int ret_val = 0;

	unsigned int page_size = flash_page_size();
	//unsigned int page_mask = page_size -1;

	unsigned long long ptn = 0;
	unsigned int ptn_index;
	unsigned int img_fmt = 0;       // 0 = BMP, 1 = JPEG

	const char* ptn_name = "splash";

	void * temp_addr;

	jpu_dec_outputs_t jpu_out = {0,};

	//dprintf(INFO, "==== %s in\n", __func__);

	ptn_index = partition_get_index(ptn_name);
	ptn = partition_get_offset(ptn_index);

	if(ptn == 0) {
		dprintf(CRITICAL, "ERROR : No splash partition found !\n");
		return -1;
	}
	else{
		if(tcc_read(ptn, splash_hdr, page_size)) {
			dprintf(CRITICAL, "Splash TAG Is Mismatched\n");
			return -1;
		}

		if(splash_hdr->ulNumber < 0) {
		        dprintf(CRITICAL, " there is no data in splash partition \n");
		        return -1;
		}

		img_idx = get_splash_index(splash_hdr, partition);
		dprintf(SPEW, "image idx = %d \n" ,img_idx);


		if(img_idx < 0) {
			dprintf(CRITICAL, " there is no image from [%s]\n", partition);
		}
		else{

			fb_cfg->width = splash_hdr->SPLASH_IMAGE[img_idx].ulImageWidth;
			fb_cfg->height = splash_hdr->SPLASH_IMAGE[img_idx].ulImageHeight;
			fb_cfg->stride = fb_cfg->width;

			if(!strcmp((const char *)splash_hdr->SPLASH_IMAGE[img_idx].ucFmt, "JPEG")) {
			        dprintf(INFO, "Splash image format is JPEG \n");
			        img_fmt = 1;

			        tcc_read(ptn+(splash_hdr->SPLASH_IMAGE[img_idx].ulImageAddr), \
			                                JPEG_DECODE_ADDR, splash_hdr->SPLASH_IMAGE[img_idx].ulImageSize );
			}
			else {
			        dprintf(INFO, "Splash image format is BMP \n");
			        img_fmt = 0;

			        tcc_read(ptn+(splash_hdr->SPLASH_IMAGE[img_idx].ulImageAddr), \
			                                fb_cfg->base, splash_hdr->SPLASH_IMAGE[img_idx].ulImageSize );
			}

			if(img_fmt == 1) {
			        ret_val = splash_image_decode_jpeg(splash_hdr->SPLASH_IMAGE[img_idx].ulImageSize, &jpu_out);
			        if(ret_val != 0)
			                return -1;

			        splash_image_convert_format(&jpu_out, fb_cfg);
			}
		}
	}

	//dprintf(INFO, "==== %s out\n", __func__);
	return 0;

}
#endif

int get_splash_image_early_camera_V2(char *image_name, unsigned int parking_guide, unsigned int *width, unsigned int *height)
{
	unsigned int partition_index;
	unsigned long long partition = 0;
	SPLASH_IMAGE_Header_info_t *splash_header = (void *)splash;
#if _NAND_BOOT
	unsigned int page_size = flash_page_size();
#else
	unsigned int page_size = 512;
#endif
	int image_index = 0;
	int rel = 0;

	const char* partition_name = "splash";

	partition_index = partition_get_index(partition_name);
	partition = partition_get_offset(partition_index);

	if (partition == 0) {
		dprintf(CRITICAL, "ERROR:  No splash partition found. \n");
	}
	else {
		if (tcc_read(partition, splash_header, page_size)) {
			dprintf(CRITICAL, "ERROR:  Splash tag is mismatched. \n");
		}
		else rel =1;

		if(splash_header->ulNumber < 0 || splash_header->ulNumber > 255) {
			dprintf(CRITICAL, "ERROR:  There is no data in splash partition. \n");
			rel = 0;
		}
		else {
			image_index = get_splash_index(splash_header, image_name);
			dprintf(SPEW, "Early-Camera image index = %d. \n", image_index);
			rel = 1;
		}
		if(rel) {
			if (image_index < 0) {
				dprintf(CRITICAL, "ERROR:  There is no image from [%s]. \n", image_name);
				rel = 0;
			}
			else {
				if(width)
					*width = splash_header->SPLASH_IMAGE[image_index].ulImageWidth;
				if(height)
					*height = splash_header->SPLASH_IMAGE[image_index].ulImageHeight;
				tcc_read((partition + splash_header->SPLASH_IMAGE[image_index].ulImageAddr), \
						(void *)parking_guide, splash_header->SPLASH_IMAGE[image_index].ulImageSize);
				dprintf(INFO, "=============OK:  get_splash_image_early_camera_V2 ==================== \n");
			}
		}
	}
	return rel;
}

int splash_image(char *partition)
{
#if defined(BOOTSD_INCLUDE)
	return splash_image_sdmmc(partition);
#elif defined(TNFTL_V8_INCLUDE)
	return splash_image_nand_v8(partition);
#else
	//return splash_image_nand_v7(partition);
#endif

	dprintf(CRITICAL, "Not defined storage device\n");
	return -1;
}
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define FILE_OFFSET_BASE_ADDR			0X60
#define FILE_OFFSET_INIT_CODE			0X70

#define MEMBASE 0X82000000
#define BYTE_TO_SECTOR(X)           ((X + 511) >> 9)
#define BOOTSD_HIDDEN_MAX_COUNT         10
#define CRC32_SIZE  4

#define BOOTCONFIG_HIGH_SPEED   0x00000004
#define BOOTCONFIG_NORMAL_SPEED 0x00000000

#define BOOTCONFIG_1BIT_BUS     0x00000000
#define BOOTCONFIG_4BIT_BUS     0x00000001
#define BOOTCONFIG_8BIT_BUS     0x00000002

//#define DEBUG

#if defined(DEBUG)
#define DBG 	printf
#else 
#define DBG
#endif

static const unsigned int CRC32_TABLE[256] = {     
	0x00000000, 0x90910101, 0x91210201, 0x01B00300,
	0x92410401, 0x02D00500, 0x03600600, 0x93F10701,
	0x94810801, 0x04100900, 0x05A00A00, 0x95310B01,
	0x06C00C00, 0x96510D01, 0x97E10E01, 0x07700F00,
	0x99011001, 0x09901100, 0x08201200, 0x98B11301,
	0x0B401400, 0x9BD11501, 0x9A611601, 0x0AF01700,
	0x0D801800, 0x9D111901, 0x9CA11A01, 0x0C301B00,
	0x9FC11C01, 0x0F501D00, 0x0EE01E00, 0x9E711F01,
	0x82012001, 0x12902100, 0x13202200, 0x83B12301,
	0x10402400, 0x80D12501, 0x81612601, 0x11F02700,
	0x16802800, 0x86112901, 0x87A12A01, 0x17302B00,
	0x84C12C01, 0x14502D00, 0x15E02E00, 0x85712F01,
	0x1B003000, 0x8B913101, 0x8A213201, 0x1AB03300,
	0x89413401, 0x19D03500, 0x18603600, 0x88F13701,
	0x8F813801, 0x1F103900, 0x1EA03A00, 0x8E313B01,
	0x1DC03C00, 0x8D513D01, 0x8CE13E01, 0x1C703F00,
	0xB4014001, 0x24904100, 0x25204200, 0xB5B14301,
	0x26404400, 0xB6D14501, 0xB7614601, 0x27F04700,
	0x20804800, 0xB0114901, 0xB1A14A01, 0x21304B00,
	0xB2C14C01, 0x22504D00, 0x23E04E00, 0xB3714F01,
	0x2D005000, 0xBD915101, 0xBC215201, 0x2CB05300,
	0xBF415401, 0x2FD05500, 0x2E605600, 0xBEF15701,
	0xB9815801, 0x29105900, 0x28A05A00, 0xB8315B01,
	0x2BC05C00, 0xBB515D01, 0xBAE15E01, 0x2A705F00,
	0x36006000, 0xA6916101, 0xA7216201, 0x37B06300,
	0xA4416401, 0x34D06500, 0x35606600, 0xA5F16701,
	0xA2816801, 0x32106900, 0x33A06A00, 0xA3316B01,
	0x30C06C00, 0xA0516D01, 0xA1E16E01, 0x31706F00,
	0xAF017001, 0x3F907100, 0x3E207200, 0xAEB17301,
	0x3D407400, 0xADD17501, 0xAC617601, 0x3CF07700,
	0x3B807800, 0xAB117901, 0xAAA17A01, 0x3A307B00,
	0xA9C17C01, 0x39507D00, 0x38E07E00, 0xA8717F01,
	0xD8018001, 0x48908100, 0x49208200, 0xD9B18301,
	0x4A408400, 0xDAD18501, 0xDB618601, 0x4BF08700,
	0x4C808800, 0xDC118901, 0xDDA18A01, 0x4D308B00,
	0xDEC18C01, 0x4E508D00, 0x4FE08E00, 0xDF718F01,
	0x41009000, 0xD1919101, 0xD0219201, 0x40B09300,
	0xD3419401, 0x43D09500, 0x42609600, 0xD2F19701,
	0xD5819801, 0x45109900, 0x44A09A00, 0xD4319B01,
	0x47C09C00, 0xD7519D01, 0xD6E19E01, 0x46709F00,
	0x5A00A000, 0xCA91A101, 0xCB21A201, 0x5BB0A300,
	0xC841A401, 0x58D0A500, 0x5960A600, 0xC9F1A701,
	0xCE81A801, 0x5E10A900, 0x5FA0AA00, 0xCF31AB01,
	0x5CC0AC00, 0xCC51AD01, 0xCDE1AE01, 0x5D70AF00,
	0xC301B001, 0x5390B100, 0x5220B200, 0xC2B1B301,
	0x5140B400, 0xC1D1B501, 0xC061B601, 0x50F0B700,
	0x5780B800, 0xC711B901, 0xC6A1BA01, 0x5630BB00,
	0xC5C1BC01, 0x5550BD00, 0x54E0BE00, 0xC471BF01,
	0x6C00C000, 0xFC91C101, 0xFD21C201, 0x6DB0C300,
	0xFE41C401, 0x6ED0C500, 0x6F60C600, 0xFFF1C701,
	0xF881C801, 0x6810C900, 0x69A0CA00, 0xF931CB01,
	0x6AC0CC00, 0xFA51CD01, 0xFBE1CE01, 0x6B70CF00,
	0xF501D001, 0x6590D100, 0x6420D200, 0xF4B1D301,
	0x6740D400, 0xF7D1D501, 0xF661D601, 0x66F0D700,
	0x6180D800, 0xF111D901, 0xF0A1DA01, 0x6030DB00,
	0xF3C1DC01, 0x6350DD00, 0x62E0DE00, 0xF271DF01,
	0xEE01E001, 0x7E90E100, 0x7F20E200, 0xEFB1E301,
	0x7C40E400, 0xECD1E501, 0xED61E601, 0x7DF0E700,
	0x7A80E800, 0xEA11E901, 0xEBA1EA01, 0x7B30EB00,
	0xE8C1EC01, 0x7850ED00, 0x79E0EE00, 0xE971EF01,
	0x7700F000, 0xE791F101, 0xE621F201, 0x76B0F300,
	0xE541F401, 0x75D0F500, 0x7460F600, 0xE4F1F701,
	0xE381F801, 0x7310F900, 0x72A0FA00, 0xE231FB01,
	0x71C0FC00, 0xE151FD01, 0xE0E1FE01, 0x7070FF00 
};                                                 

unsigned int FWUG_CalcCrc8(unsigned char *base, unsigned int length)
{                                                                   
	unsigned int crcout = 0;                                        
	unsigned int cnt;                                               
	unsigned char   code, tmp;                                      

	for(cnt=0; cnt<length; cnt++)                                   
	{                                                               
		code = base[cnt];                                           
		tmp = code^crcout;                                          
		crcout = (crcout>>8)^CRC32_TABLE[tmp&0xFF];                 
	}                                                               
	return crcout;                                                  
}                                                                   

typedef struct BOOTSD_Header_info {
	unsigned int ulEntryPoint;
	unsigned int ulBaseAddr;
	unsigned int ulConfSize;
	unsigned int ulROMFileSize;
	unsigned int ulBootConfig;
	unsigned int ulBootPartitionSize;
	unsigned int ulConfigCodeBase;
	unsigned int ulBootloaderBase;
	unsigned char ucSerialNum[64];
	unsigned int ulResv96[8];
	unsigned int ulHiddenStartAddr;
	unsigned int ulHiddenSize[BOOTSD_HIDDEN_MAX_COUNT];
	unsigned int ulResv176[83];
	unsigned char ucHeaderFlag[4];
	unsigned int ulCRC;
}BOOTSD_Header_Info_T;

static unsigned long 
get_file_size(FILE* file)
{
	unsigned long f_len;
	fseek(file , 0L, SEEK_END);
	f_len = ftell(file);
	fseek(file, 0L, SEEK_SET);

	return f_len;
}

static int 
extract_dram_init(char *infile ,char *outfile)
{
	FILE *fd = fopen(infile , "r");
	FILE *wfd = fopen(outfile, "w");
	int sync_wfd = fileno(wfd);
	unsigned int f_len;
	unsigned char *rBuffer ,*dBuffer; 
	unsigned int sInitCode , eInitCode, baseAddr, sizeInitCode;
	unsigned int crc;
	
	f_len = get_file_size(fd);

	rBuffer = malloc(f_len);
	memset(rBuffer , 0x00, f_len);

	if(!fread(rBuffer, sizeof(char), f_len, fd)){
		DBG("ROM FILE READ FAIL\n");
		return -1;
	}

	memcpy((void*)&baseAddr, &rBuffer[FILE_OFFSET_BASE_ADDR],4);
	memcpy((void*)&sInitCode, &rBuffer[FILE_OFFSET_INIT_CODE],4);
	memcpy((void*)&eInitCode, &rBuffer[FILE_OFFSET_INIT_CODE+4],4);

	sizeInitCode = eInitCode - sInitCode;

	DBG("[BASE ADDR : %x] [INIT CODE START : %x] [INIT CODE END : %x] [INIT CODE SIZE : %x]\n", baseAddr, sInitCode, eInitCode, sizeInitCode);

	dBuffer = &rBuffer[sInitCode - baseAddr];

	if(!fwrite(dBuffer, sizeof(char), sizeInitCode, wfd)){
		DBG("DRAM INIT CODE FILE WRITE FAIL\n");
		return -1;
	}

	// CRC
	crc = FWUG_CalcCrc8( (unsigned char*)dBuffer, sizeInitCode ); 
	if(!fwrite(&crc, sizeof(unsigned int), 1, wfd))
	{
		DBG("DRAM INIT CODE CRC WRITE FAIL\n");
		return -1;
	}

	free(rBuffer);
	fflush(wfd);
	fsync(sync_wfd);
	fclose(fd);
	fclose(wfd);

	return 0;

}

int 
make_header(char *romfile ,char *dramfile, char *outfile)
{
	BOOTSD_Header_Info_T header;                                               
	FILE *rfd = fopen(romfile, "r");
	FILE *dfd = fopen(dramfile, "r");
	FILE *ofd = fopen(outfile, "w");
	int sync_ofd = fileno(ofd);

	memset(&header, 0, sizeof(BOOTSD_Header_Info_T));                          

	header.ulEntryPoint = MEMBASE;
	header.ulBaseAddr = MEMBASE;
	header.ulConfSize = get_file_size(dfd);
	header.ulROMFileSize = get_file_size(rfd);
	header.ulBootConfig = (BOOTCONFIG_NORMAL_SPEED | BOOTCONFIG_4BIT_BUS);
	header.ulConfigCodeBase = 1;
	header.ulBootloaderBase = 1 + BYTE_TO_SECTOR(header.ulConfSize);

	memcpy(header.ucHeaderFlag, "HEAD", 4);

	// If the hidden area is required, the relevant information in the boot header must be written in this function.
	// If you want to write the serial number in this tool, you need to do that in this function.

	header.ulCRC = FWUG_CalcCrc8((unsigned char*) &header, 512-4);

    if(!fwrite(&header, sizeof(char), 512, ofd)){
		DBG("Header FILE WRITE FAIL\n");
		return -1;
	}

	//DBG("[EntryPoint : 0x%08X] [BaseAddr : 0x%08X] [ConfSize : %lu] [RomSize : %lu] [BootConfig : %lu] [ConfigBase : %lu] [BootBase : %lu] [CRC : %lu]\n",
	DBG("[EntryPoint : 0x%08X] [BaseAddr : 0x%08X] [ConfSize : %lu] [RomSize : %lu]  [ConfigBase : %lu] [BootBase : %lu] [CRC : %lu]\n",
			header.ulEntryPoint,
			header.ulBaseAddr,
			header.ulConfSize,
			header.ulROMFileSize,
			//header.ulBootConfig,
			header.ulConfigCodeBase,
			header.ulBootloaderBase,
			header.ulCRC);

	DBG("header size : %u\n", sizeof(header));

	fclose(rfd);
	fclose(dfd);
	fflush(ofd);
	fsync(sync_ofd);
	fclose(ofd);

	return 0;
}

int 
main(int argc, char *argv[])
{
	int ret = 0;

	if(argc < 4)
	{
		DBG("[usage] mkimg lk.bin path dram image path header image path\n");
		return -1;
	}

	DBG("LK_PATH : %s, DRAM_PATH :%s, HEADER_PATH :%s\n", argv[1], argv[2], argv[3]);

	ret = extract_dram_init(argv[1], argv[2]);

	ret = make_header(argv[1], argv[2], argv[3]);

	return ret;
}

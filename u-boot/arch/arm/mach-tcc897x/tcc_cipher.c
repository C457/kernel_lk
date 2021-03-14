#include <common.h>
#include <mach/tcc_cipher.h>

typedef struct {
	uint nReg;
} REG;

typedef struct {
	uint        EN          : 1;    //[00]
	uint                    :14;    //[14:01]
	uint        BWRAP       : 1;    //[15]
	uint        RATEEN      : 1;    //[16]
	uint        RATEPAUSE   : 3;    //[19:17]
	uint        RATEWAIT    : 3;    //[22:20]
	uint        IRQEN       : 1;    //[23]
	uint        CHANNEL     : 8;    //[31:24]
} DMA_CTRL;

typedef union {
	uint        nReg;
	DMA_CTRL    bReg;
} DMA_CTRL_U;

typedef struct {
	uint        SIZE        :31;    //[31:00]
} DMA_SIZE;

typedef union {
	uint        nReg;
	DMA_SIZE    bReg;
} DMA_SIZE_U;

typedef struct {
	uint        NEMPTY      : 8;    //[07:00]
	uint                    :21;    //[28:08]
	uint        IDLE        : 1;    //[29]
	uint        REMPTY      : 1;    //[30]
	uint        WFULL       : 1;    //[31]
} DMA_FIFOCTRL;

typedef union {
	uint            nReg;
	DMA_FIFOCTRL    bReg;
} DMA_FIFOCTRL_U;

typedef struct {
	uint        MODE        : 3;    //[02:00]
	uint                    : 1;    //[03]
	uint        ENDIAN      : 4;    //[07:04]
	uint        KEY         : 3;    //[10:08]
	uint                    : 4;    //[14:11]
	uint        IDLE        : 1;    //[15]
	uint                    :16;    //[31:16]
} CIPHER_CTRL;

typedef union {
	uint        nReg;
	CIPHER_CTRL bReg;
} CIPHER_CTRL_U;

typedef struct {
	uint        AESMODE     : 3;    //[02:00]
	uint                    : 1;    //[03]
	uint        DESMODE     : 2;    //[05:04]
	uint        DESPARITY   : 1;    //[06]
	uint                    :17;    //[23:07]
	uint        OPMODE      : 3;    //[26:24]
	uint        ENC         : 1;    //[27]
	uint                    : 2;    //[29:28]
	uint        IVLOAD      : 1;    //[30]
	uint        KEYLOAD     : 1;    //[31]
} CIPHER_ADM;

typedef union {
	uint        nReg;
	CIPHER_ADM  bReg;
} CIPHER_ADM_U;

typedef struct {
	DMA_CTRL_U      uDMACTRL;   //0x000
	uint            nSBASE;     //0x004
	uint            nDBASE;     //0x008
	DMA_SIZE_U      uSIZE;      //0x00c
	DMA_FIFOCTRL_U  uFIFOCTRL;  //0x010
	uint            nFIFOSBASE; //0x014
	uint            nFIFODBASE; //0x018
	uint            nFIFOTSIZE; //0x01C
	uint            nIRQMSK;    //0x020
	uint            nIRQSTS;    //0x024
	uint            _empty0[54];//0x028~0x0FC
	CIPHER_CTRL_U   uCTRL;      //0x100
	CIPHER_ADM_U    uADM;       //0x104
	uint            _empty1[2]; //0x108~0x10C
	uint            nHASH;      //0x110
	uint            nCSA3;      //0x114
	uint            nRSA;       //0x118
	uint            nRSADO;     //0x11C
	uint            nROUND;     //0x120
	uint            _empty2[23];//0x124~0x17C
	uint            nKEY0;      //0x180
	uint            nKEY1;      //0x184
	uint            nKEY2;      //0x188
	uint            nKEY3;      //0x18C
	uint            nKEY4;      //0x190
	uint            nKEY5;      //0x194
	uint            nKEY6;      //0x198
	uint            nKEY7;      //0x19C
	uint            nKEY8;      //0x1A0
	uint            nKEY9;      //0x1A4
	uint            _empty3[6]; //0x1A8~0x1BC
	uint            nIV0;       //0x1C0
	uint            nIV1;       //0x1C4
	uint            nIV2;       //0x1C8
	uint            nIV3;       //0x1CC
	uint            _empty4[12];//0x1D0~0x1FC
} CIPHER;

#define HwHSB_CFG_CLKMASK       ((volatile REG          *)0x71EA0000)
#define HwHSB_CFG_SWRESET       ((volatile REG          *)0x71EA0004)
#define HwCIPHER                ((volatile CIPHER       *)0x71EC0000)

static int gbCipherInit = 0;
static int hwCipherStart(void)
{
	if(gbCipherInit == 0)
	{
		HwHSB_CFG_CLKMASK->nReg |= 0x00040000;
		HwHSB_CFG_SWRESET->nReg &= 0xFFFBFFFF;
		gbCipherInit = 1;
	}

	HwHSB_CFG_SWRESET->nReg |= 0x00040000;

	return gbCipherInit;
}
static void hwCipherEnd(void)
{
	HwCIPHER->uDMACTRL.nReg = 0x00078000;
	HwCIPHER->uCTRL.nReg = 0;
	HwCIPHER->uADM.nReg = 0;
	HwCIPHER->nHASH = 0;
	HwCIPHER->nROUND = 0;

	HwHSB_CFG_SWRESET->nReg &= 0xFFFBFFFF;
}

#define CIPHER_AES_PACKET		(4096)	// Max. 4KB (4096) (0x1000)
#define CIPHER_AES_PACK_MAX	(CIPHER_AES_PACKET*CIPHER_AES_PACKET)	// Max. 16MB (4096*4096)

#define CIPHER_AES_BLOCK		(16)	// 16 bytes (in 128 bits)
#define CIPHER_AES_BLK_MAX		(CIPHER_AES_PACKET*CIPHER_AES_BLOCK)	// Max. 64KB (4096*16) (in 128 bits)


void tcc_cipher_AES(unsigned int encode, unsigned int opmode, unsigned int size, unsigned int sbase, unsigned int dbase, unsigned int *key, unsigned int *iv)
{
	unsigned int bucket = 0;
	unsigned int bOTPKey = opmode & CIPHER_OTPKEY_FLAG;
	unsigned int aligned_sbase = (sbase & 0x3F) ? (sbase - (sbase & 0x3F)) : sbase;
	unsigned int aligned_dbase = (dbase & 0x3F) ? (dbase - (dbase & 0x3F)) : dbase;
	unsigned int aligned_size = (size & 0x3F) ? (size + 0x80 - (size & 0x3F)) : size + 0x40;

	if(size == 0) return;

	opmode &= (~CIPHER_ADDFUNC_FLAG);

	flush_dcache_range(aligned_sbase, aligned_sbase + aligned_size);
	flush_dcache_range(aligned_dbase, aligned_dbase + aligned_size);
	hwCipherStart();

	//HwCIPHER->uCTRL.bReg.ENDIAN = 0;//little endian check!!!!

	HwCIPHER->uCTRL.bReg.MODE = CIPHER_SELECT_AES;
	HwCIPHER->uADM.bReg.OPMODE = opmode; // 0:ECB, 1:CBC

	if(bOTPKey == 0)
	{
		HwCIPHER->uCTRL.bReg.KEY = 0;
		HwCIPHER->nKEY0 = *key++;		//key0
		HwCIPHER->nKEY1 = *key++;		//key1
		HwCIPHER->nKEY2 = *key++;		//key2
		HwCIPHER->nKEY3 = *key++;		//key3
	}
	else
	{
		HwCIPHER->uCTRL.bReg.KEY = 1;	// OTP Key
	}

	if(opmode == CIPHER_AES_CBC)
	{
		HwCIPHER->nIV0 = *iv++;		//iv0
		HwCIPHER->nIV1 = *iv++;		//iv1
		HwCIPHER->nIV2 = *iv++;		//iv2
		HwCIPHER->nIV3 = *iv++;		//iv3
	}

	HwCIPHER->uADM.bReg.ENC = encode;	// 0:Decryption, 1:Encryption
	HwCIPHER->uADM.bReg.AESMODE = 0;	// 0:AES128, 1:AES192, 2,3:AES256
	HwCIPHER->uADM.bReg.KEYLOAD = 1;
	if(opmode == CIPHER_AES_CBC)
	{
		HwCIPHER->uADM.bReg.IVLOAD = 1;
	}

	while(1)
	{
		HwCIPHER->nSBASE = sbase;
		HwCIPHER->nDBASE = dbase;

		if(size <= CIPHER_AES_PACK_MAX)
		{
			bucket = size;
		}
		else //if(size > CIPHER_AES_PACK_MAX)
		{
			bucket = CIPHER_AES_PACK_MAX;
		}

		HwCIPHER->uSIZE.bReg.SIZE = bucket;
		HwCIPHER->uDMACTRL.bReg.EN = 1;

		while(!HwCIPHER->uFIFOCTRL.bReg.IDLE);

		if(size == bucket)
			break;

		size -= bucket;
		sbase += bucket;
		dbase += bucket;
	}

	invalidate_dcache_range(aligned_dbase, aligned_dbase + aligned_size);
	hwCipherEnd();
}


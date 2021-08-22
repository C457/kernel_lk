#ifndef _SNOR_H
#define _SNOR_H

typedef	struct
{
	unsigned short	MID;
	unsigned short	DID;
	unsigned int 	SectorSize;
	unsigned int	BlockSize;
	unsigned int	BlockNum;
	unsigned int	uStart;
	unsigned int	uDest;
	unsigned int	uLength;
	unsigned int 	uDLDV;
	unsigned char	Name[20];
	unsigned char	SerialNumber[64];
	unsigned int	kernel_addr;
	unsigned int	kernel_size;
	unsigned int	data_addr;
	unsigned int	data_size;
	unsigned int	boot_part_size;
	unsigned int	uSF2Header_addr[2];
	unsigned int	uNSKHeader_addr[2];
	unsigned char	Rsv[256-156];
	unsigned int	uCRC;
} sSFBootHeader;

typedef struct
{
	unsigned int	uBaseAddress;  		// Bootloader Dest address (dram addr)
	unsigned int	uBootRomSize;		// Bootloader Rom size
	unsigned int	uBootRomAddress;	//Bootloader Rom address (snor addr)
	unsigned int	uInitCodeSize; 		// DRAM init code size
	unsigned int	uInitCodeAddress; 	// DRAM init code snor address (snor addr)
	unsigned int	uInitCodeBaseAddress; // DRAM init code address (offset: lk.rom)
	unsigned int	uFrequency_MHz;
	unsigned int	uReserved[56];
	unsigned int	uCRC;          		// Header CRC
} sSF2ndBootHeader;

typedef struct
{
	unsigned int	uTcc_start;
	unsigned int	uHardwareID;
	unsigned int	uFirmwareVersion[2];
	unsigned int	uFirmwareCheckSum;	// 128kbyte CRC
	unsigned int	uReserved_1;
	unsigned int	uFirmwareCheckSumEnd;	// 128kbyte ~ end CRC
	unsigned int 	uFirmwareSize;
	unsigned int	uSerialNumber[16];
	unsigned int	uFirmwareBaseAddress;
	unsigned int	uReserved_2;
	unsigned int	uFwdnSig;
	unsigned int	uChipsetName;
	unsigned int	uInitRoutine_StartBase;
	unsigned int	uInitRoutine_StartLimit;
	unsigned int	uSnorSig;
	unsigned int	uSnorLoaderStart;
	unsigned int	uSnorLoaderSize;
	unsigned int	uSnorLoaderBaseAddr;
} sTCCBootRomHeader;

int SNOR_Init(void);
unsigned int SNOR_GetSectorSize(void);
unsigned int SNOR_GetTotalSize(void);
unsigned int SNOR_GetBootSize(void);
unsigned int SNOR_GetBoot1stRomPartSize(void);
unsigned int SNOR_GetKernelSize(void);
unsigned int SNOR_GetKernelAddress(void);
unsigned int SNOR_GetDataAreaSize(void);
unsigned int SNOR_GetFrequency(void);
unsigned int SNOR_GetReadFrequency(void);
void SNOR_WP(int mode);
void SNOR_ChipErase(void);
int SNOR_Write(unsigned int address, const void *pBuffer, unsigned int length);
int SNOR_Read(unsigned int address, void *pBuffer, unsigned int length);
void SNOR_UnitErase(unsigned int address, unsigned int size);
void SNOR_SectorErase(unsigned int address);
int SNOR_DataAreaUpdate_Write(unsigned long ulLBA_addr, unsigned long ulSector, void *buff);
int SNOR_Kernel_Read(unsigned long ulLBA_addr, unsigned long ulSector, void *buff);
int SNOR_Kernel_Write(unsigned long ulLBA_addr, unsigned long ulSector, void *buff);
unsigned int SNOR_GetDataAreaAddress(void);
unsigned int SNOR_GetBootAddress(void);
int FWDN_SNOR_Write_Firmware( unsigned char * pucRomFile_Buffer, unsigned int uiROMFileSize);
int SNOR_GetSerialNumber(char* Serial);


#define HwGPIO_BASE                             (0x14200000)
#define HwGPIOA_BASE                            (0x14200000)
#define HwGPIOB_BASE                            (0x14200040)
#define HwGPIOC_BASE                            (0x14200080)
#define HwGPIOD_BASE                            (0x142000C0)
#define HwGPIOE_BASE                            (0x14200100)
#define HwGPIOF_BASE                            (0x14200140)
#define HwGPIOG_BASE                            (0x14200180)
#define HwGPIOHDMI_BASE                         (0x142001C0)
#define HwGPIOADC_BASE                          (0x14200200)
#define HwGPIOSD_BASE                           (0x14200240)

/*******************************************************************************
GPSB Controller Register Define
********************************************************************************/
#define HwGPSB0_BASE							(0x16900000)
#define HwGPSB1_BASE							(0x16910000)
#define HwGPSB2_BASE							(0x16920000)
#define HwGPSB_PORTCFG_BASE						(0x16960000)
#define HwGPSB_PIDTABLE_BASE					(0x16970000)

typedef struct {
    unsigned CH0            :8;
    unsigned CH1            :8;
    unsigned CH2            :8;
    unsigned CH3            :8;
} GPSB_PCFG0_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    GPSB_PCFG0_IDX_TYPE     bREG;
} GPSB_PCFG0_TYPE;

typedef struct {
    unsigned CH4            :8;
    unsigned CH5            :8;
    unsigned                :16;
} GPSB_PCFG1_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    GPSB_PCFG1_IDX_TYPE     bREG;
} GPSB_PCFG1_TYPE;

typedef struct {
    unsigned ISTC0          :1;
    unsigned ISTD0          :1;
    unsigned ISTC1          :1;
    unsigned ISTD1          :1;
    unsigned ISTC2          :1;
    unsigned ISTD2          :1;
    unsigned ISTC3          :1;
    unsigned                :1;
    unsigned ISTC4          :1;
    unsigned                :1;
    unsigned ISTC5          :1;
    unsigned                :21;
} GPSB_CIRQST_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    GPSB_CIRQST_IDX_TYPE    bREG;
} GPSB_CIRQST_TYPE;

typedef struct _GPSBPORTCFG{
    volatile GPSB_PCFG0_TYPE    PCFG0;          // 0x800  R/W  0x03020100   Port Configuration Register 0
    volatile GPSB_PCFG1_TYPE    PCFG1;          // 0x800  R/W  0x00000504   Port Configuration Register 1
    unsigned :32;
    volatile GPSB_CIRQST_TYPE   CIRQST;         // 0x80C  R    0x00000000   Channel IRQ Status Register
} GPSBPORTCFG, *PGPSBPORTCFG;

typedef struct {
    unsigned DATA           :32;
} GPSB_PORT_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    GPSB_PORT_IDX_TYPE      bREG;
} GPSB_PORT_TYPE;

typedef struct {
    unsigned RTH            :1;
    unsigned WTH            :1;
    unsigned RNE            :1;
    unsigned WE             :1;
    unsigned RF             :1;
    unsigned ROR            :1;
    unsigned WUR            :1;
    unsigned RUR            :1;
    unsigned WOR            :1;
    unsigned                :7;
    unsigned RBVCNT         :4;
    unsigned                :4;
    unsigned WBVCNT         :4;
    unsigned                :4;
} GPSB_STATUS_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    GPSB_STATUS_IDX_TYPE    bREG;
} GPSB_STATUS_TYPE;

typedef struct {
    unsigned IRQEN          :9;
    unsigned                :6;
    unsigned RC             :1;
    unsigned CFGRTH         :3;
    unsigned                :1;
    unsigned CFGWTH         :3;
    unsigned                :1;
    unsigned SBR            :1;
    unsigned SHR            :1;
    unsigned SBT            :1;
    unsigned SHT            :1;
    unsigned                :2;
    unsigned DR             :1;
    unsigned DW             :1;
} GPSB_INTEN_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    GPSB_INTEN_IDX_TYPE     bREG;
} GPSB_INTEN_TYPE;

typedef struct {
    unsigned MD             :2;
    unsigned SLV            :1;
    unsigned EN             :1;
    unsigned CTF            :1;
    unsigned SDO            :1;
    unsigned LB             :1;
    unsigned SD             :1;
    unsigned BWS            :5;
    unsigned                :1;
    unsigned CWF            :1;
    unsigned CRF            :1;
    unsigned PCK            :1;
    unsigned PRD            :1;
    unsigned PWD            :1;
    unsigned PCD            :1;
    unsigned PCS            :1;
    unsigned TSU            :1;
    unsigned THL            :1;
    unsigned TRE            :1;
    unsigned DIVLDV         :8;
} GPSB_MODE_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    GPSB_MODE_IDX_TYPE      bREG;
} GPSB_MODE_TYPE;

typedef struct {
    unsigned PSW            :5;
    unsigned                :2;
    unsigned PLW            :1;
    unsigned RDSTART        :5;
    unsigned                :3;
    unsigned CMDSTART       :5;
    unsigned                :3;
    unsigned CMDEND         :5;
    unsigned                :1;
    unsigned LCR            :1;
    unsigned LCW            :1;
} GPSB_CTRL_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    GPSB_CTRL_IDX_TYPE      bREG;
} GPSB_CTRL_TYPE;

typedef struct {
    unsigned TXCV           :16;
    unsigned                :11;
    unsigned EXTDPOL        :1;
    unsigned EXTDCHK        :1;
    unsigned EXTEN          :1;
    unsigned TXCREP         :1;
    unsigned TXCRX          :1;
} GPSB_EVTCTRL_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    GPSB_EVTCTRL_IDX_TYPE   bREG;
} GPSB_EVTCTRL_TYPE;

typedef struct {
    unsigned TXC            :16;
    unsigned                :15;
    unsigned FSDI           :1;
} GPSB_CCV_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    GPSB_CCV_IDX_TYPE       bREG;
} GPSB_CCV_TYPE;

typedef struct {
    unsigned TX_BASE        :32;
} GPSB_TX_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    GPSB_TX_IDX_TYPE        bREG;
} GPSB_TX_TYPE;

typedef struct {
    unsigned RX_BASE        :32;
} GPSB_RX_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    GPSB_RX_IDX_TYPE        bREG;
} GPSB_RX_TYPE;

typedef struct {
    unsigned SIZE           :13;
    unsigned                :3;
    unsigned COUNT          :13;
    unsigned                :3;
} GPSB_PACKET_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    GPSB_PACKET_IDX_TYPE    bREG;
} GPSB_PACKET_TYPE;

typedef struct {
    unsigned EN             :1;
    unsigned TSIF           :1;
    unsigned PCLR           :1;
    unsigned                :1;
    unsigned MD             :2;
    unsigned                :8;
    unsigned RXAM           :2;
    unsigned TXAM           :2;
    unsigned MS             :1;
    unsigned MP             :1;
    unsigned                :8;
    unsigned END            :1;
    unsigned CT             :1;
    unsigned DRE            :1;
    unsigned DTE            :1;
} GPSB_DMACTRL_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    GPSB_DMACTRL_IDX_TYPE   bREG;
} GPSB_DMACTRL_TYPE;

typedef struct {
    unsigned TXPCNT         :13;
    unsigned                :4;
    unsigned RXPCNT         :13;
    unsigned                :2;
} GPSB_DMASTAT_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    GPSB_DMASTAT_IDX_TYPE   bREG;
} GPSB_DMASTAT_TYPE;

typedef struct {
    unsigned IRQPCNT        :13;
    unsigned                :3;
    unsigned IEP            :1;
    unsigned IED            :1;
    unsigned                :2;
    unsigned IRQS           :1;
    unsigned                :7;
    unsigned ISP            :1;
    unsigned ISD            :1;
    unsigned                :2;
} GPSB_DMAICR_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    GPSB_DMAICR_IDX_TYPE    bREG;
} GPSB_DMAICR_TYPE;

typedef struct _GPSB{
    volatile GPSB_PORT_TYPE     PORT;           // 0x000  R/W  0x00000000   Data port
    volatile GPSB_STATUS_TYPE   STAT;           // 0x004  R/W  0x00000000   Status register
    volatile GPSB_INTEN_TYPE    INTEN;          // 0x008  R/W  0x00000000   Interrupt enable
    volatile GPSB_MODE_TYPE     MODE;           // 0x00C  R/W  0x00000004   Mode register
    volatile GPSB_CTRL_TYPE     CTRL;           // 0x010  R/W  0x00000000   Control register
    volatile GPSB_EVTCTRL_TYPE  EVTCTRL;        // 0x014  R/W  0x00000000   Counter & Ext. Event Control
    volatile GPSB_CCV_TYPE      CCV;            // 0x018  R    0x00000000   Counter Current Value
    unsigned :32;
    volatile GPSB_TX_TYPE       TXBASE;         // 0x020  R/W  0x00000000   TX base address register
    volatile GPSB_RX_TYPE       RXBASE;         // 0x024  R/W  0x00000000   RX base address register
    volatile GPSB_PACKET_TYPE   PACKET;         // 0x028  R/W  0x00000000   Packet register
    volatile GPSB_DMACTRL_TYPE  DMACTR;         // 0x02C  R/W  0x00000000   DMA control register
    volatile GPSB_DMASTAT_TYPE  DMASTR;         // 0x030  R/W  0x00000000   DMA status register
    volatile GPSB_DMAICR_TYPE   DMAICR;         // 0x034  R/W  0x00000000   DMA interrupt control register
} GPSB, *PGPSB, sHwGPSB;

typedef struct {
    unsigned GP00           :1;
    unsigned GP01           :1;
    unsigned GP02           :1;
    unsigned GP03           :1;
    unsigned GP04           :1;
    unsigned GP05           :1;
    unsigned GP06           :1;
    unsigned GP07           :1;
    unsigned GP08           :1;
    unsigned GP09           :1;
    unsigned GP10           :1;
    unsigned GP11           :1;
    unsigned GP12           :1;
    unsigned GP13           :1;
    unsigned GP14           :1;
    unsigned GP15           :1;
    unsigned GP16           :1;
    unsigned GP17           :1;
    unsigned GP18           :1;
    unsigned GP19           :1;
    unsigned GP20           :1;
    unsigned GP21           :1;
    unsigned GP22           :1;
    unsigned GP23           :1;
    unsigned GP24           :1;
    unsigned GP25           :1;
    unsigned GP26           :1;
    unsigned GP27           :1;
    unsigned GP28           :1;
    unsigned GP29           :1;
    unsigned GP30           :1;
    unsigned GP31           :1;
} GPIO_PORT_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    GPIO_PORT_IDX_TYPE      bREG;
} GPIO_PORT_TYPE;

typedef struct {
    unsigned GPCD00         :2;
    unsigned GPCD01         :2;
    unsigned GPCD02         :2;
    unsigned GPCD03         :2;
    unsigned GPCD04         :2;
    unsigned GPCD05         :2;
    unsigned GPCD06         :2;
    unsigned GPCD07         :2;
    unsigned GPCD08         :2;
    unsigned GPCD09         :2;
    unsigned GPCD10         :2;
    unsigned GPCD11         :2;
    unsigned GPCD12         :2;
    unsigned GPCD13         :2;
    unsigned GPCD14         :2;
    unsigned GPCD15         :2;
} GPIO_CD0_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    GPIO_CD0_IDX_TYPE       bREG;
} GPIO_CD0_TYPE;

typedef struct {
    unsigned GPCD16         :2;
    unsigned GPCD17         :2;
    unsigned GPCD18         :2;
    unsigned GPCD19         :2;
    unsigned GPCD20         :2;
    unsigned GPCD21         :2;
    unsigned GPCD22         :2;
    unsigned GPCD23         :2;
    unsigned GPCD24         :2;
    unsigned GPCD25         :2;
    unsigned GPCD26         :2;
    unsigned GPCD27         :2;
    unsigned GPCD28         :2;
    unsigned GPCD29         :2;
    unsigned GPCD30         :2;
    unsigned GPCD31         :2;
} GPIO_CD1_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    GPIO_CD1_IDX_TYPE       bREG;
} GPIO_CD1_TYPE;

typedef struct {
    unsigned GPFN00         :4;
    unsigned GPFN01         :4;
    unsigned GPFN02         :4;
    unsigned GPFN03         :4;
    unsigned GPFN04         :4;
    unsigned GPFN05         :4;
    unsigned GPFN06         :4;
    unsigned GPFN07         :4;
} GPIO_FN0_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    GPIO_FN0_IDX_TYPE       bREG;
} GPIO_FN0_TYPE;

typedef struct {
    unsigned GPFN08         :4;
    unsigned GPFN09         :4;
    unsigned GPFN10         :4;
    unsigned GPFN11         :4;
    unsigned GPFN12         :4;
    unsigned GPFN13         :4;
    unsigned GPFN14         :4;
    unsigned GPFN15         :4;
} GPIO_FN1_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    GPIO_FN1_IDX_TYPE       bREG;
} GPIO_FN1_TYPE;

typedef struct {
    unsigned GPFN16         :4;
    unsigned GPFN17         :4;
    unsigned GPFN18         :4;
    unsigned GPFN19         :4;
    unsigned GPFN20         :4;
    unsigned GPFN21         :4;
    unsigned GPFN22         :4;
    unsigned GPFN23         :4;
} GPIO_FN2_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    GPIO_FN2_IDX_TYPE       bREG;
} GPIO_FN2_TYPE;

typedef struct {
    unsigned GPFN24         :4;
    unsigned GPFN25         :4;
    unsigned GPFN26         :4;
    unsigned GPFN27         :4;
    unsigned GPFN28         :4;
    unsigned GPFN29         :4;
    unsigned GPFN30         :4;
    unsigned GPFN31         :4;
} GPIO_FN3_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    GPIO_FN3_IDX_TYPE       bREG;
} GPIO_FN3_TYPE;

typedef struct {
    unsigned GPFN00         :4;
    unsigned GPFN01         :4;
    unsigned GPFN02         :4;
    unsigned GPFN03         :4;
    unsigned                :16;
} HDMI_FN_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    HDMI_FN_IDX_TYPE        bREG;
} HDMI_FN_TYPE;

typedef struct {
    unsigned                :8;
    unsigned GPFN02         :4;
    unsigned GPFN03         :4;
    unsigned GPFN04         :4;
    unsigned GPFN05         :4;
    unsigned                :8;
} ADC_FN_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    ADC_FN_IDX_TYPE         bREG;
} ADC_FN_TYPE;

typedef struct {
    unsigned EINT00SEL      :8;
    unsigned EINT01SEL      :8;
    unsigned EINT02SEL      :8;
    unsigned EINT03SEL      :8;
} EINTSEL0_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    EINTSEL0_IDX_TYPE       bREG;
} EINTSEL0_TYPE;

typedef struct {
    unsigned EINT04SEL      :8;
    unsigned EINT05SEL      :8;
    unsigned EINT06SEL      :8;
    unsigned EINT07SEL      :8;
} EINTSEL1_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    EINTSEL1_IDX_TYPE       bREG;
} EINTSEL1_TYPE;

typedef struct {
    unsigned EINT08SEL      :8;
    unsigned EINT09SEL      :8;
    unsigned EINT10SEL      :8;
    unsigned EINT11SEL      :8;
} EINTSEL2_IDX_TYPE;

typedef union {
    unsigned long           nREG;
    EINTSEL2_IDX_TYPE       bREG;
} EINTSEL2_TYPE;






typedef struct _GPIO{
    volatile GPIO_PORT_TYPE     GPADAT;         // 0x000  R/W  0x00000000   GPA Data Register
    volatile GPIO_PORT_TYPE     GPAEN;          // 0x004  R/W  0x00000000   GPA Output Enable Register
    volatile GPIO_PORT_TYPE     GPASET;         // 0x008  W    -            OR function on GPA Output Data
    volatile GPIO_PORT_TYPE     GPACLR;         // 0x00C  W    -            BIC function on GPA Output Data
    volatile GPIO_PORT_TYPE     GPAXOR;         // 0x010  W    -            XOR function on GPA Output Data
    volatile GPIO_CD0_TYPE      GPACD0;         // 0x014  R/W  0x00000000   Driver strength Control on GPA
    volatile GPIO_CD1_TYPE      GPACD1;         // 0x018  R/W  0x00000000   Driver strength Control on GPA
    volatile GPIO_PORT_TYPE     GPAPE;          // 0x01C  R/W  0x00000000   Pull-Up/Down Enable function on GPA
    volatile GPIO_PORT_TYPE     GPAPS;          // 0x020  R/W  0x00000000   Pull-Up/Down Selection function on GPA
    volatile GPIO_PORT_TYPE     GPAIEN;         // 0x024  R/W  0xFFFFFFFF   Input Buffer Enable Function on GPA
    volatile GPIO_PORT_TYPE     GPAIS;          // 0x028  R/W  0x00000000   Schimitt input Function on GPA
    volatile GPIO_PORT_TYPE     GPASR;          // 0x02C  R/W  0x00000000   Fast Slew Rate Function GPA
    volatile GPIO_FN0_TYPE      GPAFN0;         // 0x030  R/W  0x00000000   Port Configuration on GPA Output Data
    volatile GPIO_FN1_TYPE      GPAFN1;         // 0x034  R/W  0x00000000   Port Configuration on GPA Output Data
    volatile GPIO_FN2_TYPE      GPAFN2;         // 0x038  R/W  0x00000000   Port Configuration on GPA Output Data
    volatile GPIO_FN3_TYPE      GPAFN3;         // 0x03C  R/W  0x00000000   Port Configuration on GPA Output Data

    volatile GPIO_PORT_TYPE     GPBDAT;         // 0x040  R/W  0x00000000   GPB Data Register
    volatile GPIO_PORT_TYPE     GPBEN;          // 0x044  R/W  0x00000000   GPB Output Enable Register
    volatile GPIO_PORT_TYPE     GPBSET;         // 0x048  W    -            OR function on GPB Output Data
    volatile GPIO_PORT_TYPE     GPBCLR;         // 0x04C  W    -            BIC function on GPB Output Data
    volatile GPIO_PORT_TYPE     GPBXOR;         // 0x050  W    -            XOR function on GPB Output Data
    volatile GPIO_CD0_TYPE      GPBCD0;         // 0x054  R/W  0x00000000   Driver strength Control on GPB
    volatile GPIO_CD1_TYPE      GPBCD1;         // 0x058  R/W  0x00000000   Driver strength Control on GPB
    volatile GPIO_PORT_TYPE     GPBPE;          // 0x05C  R/W  0x00000000   Pull-Up/Down Enable function on GPB
    volatile GPIO_PORT_TYPE     GPBPS;          // 0x060  R/W  0x00000000   Pull-Up/Down Selection function on GPB
    volatile GPIO_PORT_TYPE     GPBIEN;         // 0x064  R/W  0xFFFFFFFF   Input Buffer Enable Function on GPB
    volatile GPIO_PORT_TYPE     GPBIS;          // 0x068  R/W  0x00000000   Schimitt input Function on GPB
    volatile GPIO_PORT_TYPE     GPBSR;          // 0x06C  R/W  0x00000000   Fast Slew Rate Function GPB
    volatile GPIO_FN0_TYPE      GPBFN0;         // 0x070  R/W  0x00000000   Port Configuration on GPB Output Data
    volatile GPIO_FN1_TYPE      GPBFN1;         // 0x074  R/W  0x00000000   Port Configuration on GPB Output Data
    volatile GPIO_FN2_TYPE      GPBFN2;         // 0x078  R/W  0x00000000   Port Configuration on GPB Output Data
    volatile GPIO_FN3_TYPE      GPBFN3;         // 0x07C  R/W  0x00000000   Port Configuration on GPB Output Data

    volatile GPIO_PORT_TYPE     GPCDAT;         // 0x080  R/W  0x00000000   GPC Data Register
    volatile GPIO_PORT_TYPE     GPCEN;          // 0x084  R/W  0x00000000   GPC Output Enable Register
    volatile GPIO_PORT_TYPE     GPCSET;         // 0x088  W    -            OR function on GPC Output Data
    volatile GPIO_PORT_TYPE     GPCCLR;         // 0x08C  W    -            BIC function on GPC Output Data
    volatile GPIO_PORT_TYPE     GPCXOR;         // 0x090  W    -            XOR function on GPC Output Data
    volatile GPIO_CD0_TYPE      GPCCD0;         // 0x094  R/W  0x00000000   Driver strength Control 0 on GPC
    volatile GPIO_CD1_TYPE      GPCCD1;         // 0x098  R/W  0x00000000   Driver strength Control 0 on GPC
    volatile GPIO_PORT_TYPE     GPCPE;          // 0x09C  R/W  0x00000000   Pull-Up/Down Enable function on GPC
    volatile GPIO_PORT_TYPE     GPCPS;          // 0x0A0  R/W  0x00000000   Pull-Up/Down Selection function on GPC
    volatile GPIO_PORT_TYPE     GPCIEN;         // 0x0A4  R/W  0xFFFFFFFF   Input Buffer Enable Function on GPC
    volatile GPIO_PORT_TYPE     GPCIS;          // 0x0A8  R/W  0x00000000   Schimitt input Function on GPC
    volatile GPIO_PORT_TYPE     GPCSR;          // 0x0AC  R/W  0x00000000   Fast Slew Rate Function GPC
    volatile GPIO_FN0_TYPE      GPCFN0;         // 0x0B0  R/W  0x00000000   Port Configuration on GPC Output Data
    volatile GPIO_FN1_TYPE      GPCFN1;         // 0x0B4  R/W  0x00000000   Port Configuration on GPC Output Data
    volatile GPIO_FN2_TYPE      GPCFN2;         // 0x0B8  R/W  0x00000000   Port Configuration on GPC Output Data
    volatile GPIO_FN3_TYPE      GPCFN3;         // 0x0BC  R/W  0x00000000   Port Configuration on GPC Output Data

    volatile GPIO_PORT_TYPE     GPDDAT;         // 0x0C0  R/W  0x00000000   GPD Data Register
    volatile GPIO_PORT_TYPE     GPDEN;          // 0x0C4  R/W  0x00000000   GPD Output Enable Register
    volatile GPIO_PORT_TYPE     GPDSET;         // 0x0C8  W    -            OR function on GPD Output Data
    volatile GPIO_PORT_TYPE     GPDCLR;         // 0x0CC  W    -            BIC function on GPD Output Data
    volatile GPIO_PORT_TYPE     GPDXOR;         // 0x0D0  W    -            XOR function on GPD Output Data
    volatile GPIO_CD0_TYPE      GPDCD0;         // 0x0D4  R/W  0x00000000   Driver strength Control 0 on GPD
    volatile GPIO_CD1_TYPE      GPDCD1;         // 0x0D8  R/W  0x00000000   Driver strength Control 0 on GPD
    volatile GPIO_PORT_TYPE     GPDPE;          // 0x0DC  R/W  0x00000000   Pull-Up/Down Enable function on GPD
    volatile GPIO_PORT_TYPE     GPDPS;          // 0x0E0  R/W  0x00000000   Pull-Up/Down Selection function on GPD
    volatile GPIO_PORT_TYPE     GPDIEN;         // 0x0E4  R/W  0xFFFFFFFF   Input Buffer Enable Function on GPD
    volatile GPIO_PORT_TYPE     GPDIS;          // 0x0E8  R/W  0x00000000   Schimitt input Function on GPD
    volatile GPIO_PORT_TYPE     GPDSR;          // 0x0EC  R/W  0x00000000   Fast Slew Rate Function GPD
    volatile GPIO_FN0_TYPE      GPDFN0;         // 0x0F0  R/W  0x00000000   Port Configuration on GPD Output Data
    volatile GPIO_FN1_TYPE      GPDFN1;         // 0x0F4  R/W  0x00000000   Port Configuration on GPD Output Data
    volatile GPIO_FN2_TYPE      GPDFN2;         // 0x0F8  R/W  0x00000000   Port Configuration on GPD Output Data
    volatile GPIO_FN3_TYPE      GPDFN3;         // 0x0FC  R/W  0x00000000   Port Configuration on GPD Output Data

    volatile GPIO_PORT_TYPE     GPEDAT;         // 0x100  R/W  0x00000000   GPE Data Register
    volatile GPIO_PORT_TYPE     GPEEN;          // 0x104  R/W  0x00000000   GPE Output Enable Register
    volatile GPIO_PORT_TYPE     GPESET;         // 0x108  W    -            OR function on GPE Output Data
    volatile GPIO_PORT_TYPE     GPECLR;         // 0x10C  W    -            BIC function on GPE Output Data
    volatile GPIO_PORT_TYPE     GPEXOR;         // 0x110  W    -            XOR function on GPE Output Data
    volatile GPIO_CD0_TYPE      GPECD0;         // 0x114  R/W  0x00000000   Driver strength Control 0 on GPE
    volatile GPIO_CD1_TYPE      GPECD1;         // 0x118  R/W  0x00000000   Driver strength Control 0 on GPE
    volatile GPIO_PORT_TYPE     GPEPE;          // 0x11C  R/W  0x00000000   Pull-Up/Down Enable function on GPE
    volatile GPIO_PORT_TYPE     GPEPS;          // 0x120  R/W  0x00000000   Pull-Up/Down Selection function on GPE
    volatile GPIO_PORT_TYPE     GPEIEN;         // 0x124  R/W  0xFFFFFFFF   Input Buffer Enable Function on GPE
    volatile GPIO_PORT_TYPE     GPEIS;          // 0x128  R/W  0x00000000   Schimitt input Function on GPE
    volatile GPIO_PORT_TYPE     GPESR;          // 0x12C  R/W  0x00000000   Fast Slew Rate Function GPE
    volatile GPIO_FN0_TYPE      GPEFN0;         // 0x130  R/W  0x00000000   Port Configuration on GPE Output Data
    volatile GPIO_FN1_TYPE      GPEFN1;         // 0x134  R/W  0x00000000   Port Configuration on GPE Output Data
    volatile GPIO_FN2_TYPE      GPEFN2;         // 0x138  R/W  0x00000000   Port Configuration on GPE Output Data
    volatile GPIO_FN3_TYPE      GPEFN3;         // 0x13C  R/W  0x00000000   Port Configuration on GPE Output Data

    volatile GPIO_PORT_TYPE     GPFDAT;         // 0x140  R/W  0x00000000   GPF Data Register
    volatile GPIO_PORT_TYPE     GPFEN;          // 0x144  R/W  0x00000000   GPF Output Enable Register
    volatile GPIO_PORT_TYPE     GPFSET;         // 0x148  W    -            OR function on GPF Output Data
    volatile GPIO_PORT_TYPE     GPFCLR;         // 0x14C  W    -            BIC function on GPF Output Data
    volatile GPIO_PORT_TYPE     GPFXOR;         // 0x150  W    -            XOR function on GPF Output Data
    volatile GPIO_CD0_TYPE      GPFCD0;         // 0x154  R/W  0x00000000   Driver strength Control 0 on GPF
    volatile GPIO_CD1_TYPE      GPFCD1;         // 0x158  R/W  0x00000000   Driver strength Control 0 on GPF
    volatile GPIO_PORT_TYPE     GPFPE;          // 0x15C  R/W  0x00000000   Pull-Up/Down Enable function on GPF
    volatile GPIO_PORT_TYPE     GPFPS;          // 0x160  R/W  0x00000000   Pull-Up/Down Selection function on GPF
    volatile GPIO_PORT_TYPE     GPFIEN;         // 0x164  R/W  0xFFFFFFFF   Input Buffer Enable Function on GPF
    volatile GPIO_PORT_TYPE     GPFIS;          // 0x168  R/W  0x00000000   Schimitt input Function on GPF
    volatile GPIO_PORT_TYPE     GPFSR;          // 0x16C  R/W  0x00000000   Fast Slew Rate Function GPF
    volatile GPIO_FN0_TYPE      GPFFN0;         // 0x170  R/W  0x00000000   Port Configuration on GPF Output Data
    volatile GPIO_FN1_TYPE      GPFFN1;         // 0x174  R/W  0x00000000   Port Configuration on GP Output Data
    volatile GPIO_FN2_TYPE      GPFFN2;         // 0x178  R/W  0x00000000   Port Configuration on GPF Output Data
    volatile GPIO_FN3_TYPE      GPFFN3;         // 0x17C  R/W  0x00000000   Port Configuration on GPF Output Data

    volatile GPIO_PORT_TYPE     GPGDAT;         // 0x180  R/W  0x00000000   GPG Data Register
    volatile GPIO_PORT_TYPE     GPGEN;          // 0x184  R/W  0x00000000   GPG Output Enable Register
    volatile GPIO_PORT_TYPE     GPGSET;         // 0x188  W    -            OR function on GPG Output Data
    volatile GPIO_PORT_TYPE     GPGCLR;         // 0x18C  W    -            BIC function on GPG Output Data
    volatile GPIO_PORT_TYPE     GPGXOR;         // 0x190  W    -            XOR function on GPG Output Data
    volatile GPIO_CD0_TYPE      GPGCD0;         // 0x194  R/W  0x00000000   Driver strength Control 0 on GPG
    volatile GPIO_CD1_TYPE      GPGCD1;         // 0x198  R/W  0x00000000   Driver strength Control 0 on GPG
    volatile GPIO_PORT_TYPE     GPGPE;          // 0x19C  R/W  0x00000000   Pull-Up/Down Enable function on GPG
    volatile GPIO_PORT_TYPE     GPGPS;          // 0x1A0  R/W  0x00000000   Pull-Up/Down Selection function on GPG
    volatile GPIO_PORT_TYPE     GPGIEN;         // 0x1A4  R/W  0xFFFFFFFF   Input Buffer Enable Function on GPG
    volatile GPIO_PORT_TYPE     GPGIS;          // 0x1A8  R/W  0x00000000   Schimitt input Function on GPG
    volatile GPIO_PORT_TYPE     GPGSR;          // 0x1AC  R/W  0x00000000   Fast Slew Rate Function GPG
    volatile GPIO_FN0_TYPE      GPGFN0;         // 0x1B0  R/W  0x00000000   Port Configuration on GPG Output Data
    volatile GPIO_FN1_TYPE      GPGFN1;         // 0x1B4  R/W  0x00000000   Port Configuration on GPG Output Data
    volatile GPIO_FN2_TYPE      GPGFN2;         // 0x1B8  R/W  0x00000000   Port Configuration on GPG Output Data
    volatile GPIO_FN3_TYPE      GPGFN3;         // 0x1BC  R/W  0x00000000   Port Configuration on GPG Output Data

    volatile GPIO_PORT_TYPE     GPHDMIDAT;      // 0x1C0  R/W  0x00000000   GPHDMI Data Register
    volatile GPIO_PORT_TYPE     GPHDMIEN;       // 0x1C4  R/W  0x00000000   GPHDMI Output Enable Register
    volatile GPIO_PORT_TYPE     GPHDMISET;      // 0x1C8  W    -            OR function on GPHDMI Output Data
    volatile GPIO_PORT_TYPE     GPHDMICLR;      // 0x1CC  W    -            BIC function on GPHDMI Output Data
    volatile GPIO_PORT_TYPE     GPHDMIXOR;      // 0x1D0  W    -            XOR function on GPHDMI Output Data
    volatile GPIO_CD0_TYPE      GPHDMICD;       // 0x1D4  R/W  0x00000000   Driver strength Control on GPHDMI
    unsigned :32;
    volatile GPIO_PORT_TYPE     GPHDMIPE;       // 0x1DC  R/W  0x00000000   Pull-Up/Down Enable function on GPHDMI
    volatile GPIO_PORT_TYPE     GPHDMIPS;       // 0x1E0  R/W  0x00000000   Pull-Up/Down Selection function on GPHDMI
    volatile GPIO_PORT_TYPE     GPHDMIIEN;      // 0x1E4  R/W  0xFFFFFFFF   Input Buffer Enable Function on GPHDMI
    volatile GPIO_PORT_TYPE     GPHDMIIS;       // 0x1E8  R/W  0x00000000   Schimitt input Function on GPHDMI
    volatile GPIO_PORT_TYPE     GPHDMISR;       // 0x1EC  R/W  0x00000000   Fast Slew Rate Function GPHDMI
    volatile HDMI_FN_TYPE       GPHDMIFN0;      // 0x1F0  R/W  0x00000000   Port Configuration on GPHDMI Output Data
    unsigned :32; unsigned :32; unsigned :32;

    volatile GPIO_PORT_TYPE     GPADCDAT;       // 0x200  R/W  0x00000000   GPADC Data Register
    volatile GPIO_PORT_TYPE     GPADCEN;        // 0x204  R/W  0x00000000   GPADC Output Enable Register
    volatile GPIO_PORT_TYPE     GPADCSET;       // 0x208  W    -            OR function on GPADC Output Data
    volatile GPIO_PORT_TYPE     GPADCCLR;       // 0x20C  W    -            BIC function on GPADC Output Data
    volatile GPIO_PORT_TYPE     GPADCXOR;       // 0x210  W    -            XOR function on GPADC output Data
    volatile GPIO_CD0_TYPE      GPADCCD;        // 0x214  R/W  0x00000000   Driver strength Control on GPADC
    unsigned :32;
    volatile GPIO_PORT_TYPE     GPADCPE;        // 0x21C  R/W  0x00000000   Pull-Up/Down Enable function on GPADC
    volatile GPIO_PORT_TYPE     GPADCPS;        // 0x220  R/W  0x00000000   Pull-Up/Down Selection function on GPADC
    volatile GPIO_PORT_TYPE     GPADCIEN;       // 0x224  R/W  0xFFFFFFFF   Input Buffer Enable Function on GPADC
    volatile GPIO_PORT_TYPE     GPADCIS;        // 0x228  R/W  0x00000000   Schimitt Input Function on GPADC
    volatile GPIO_PORT_TYPE     GPADCSR;        // 0x22C  R/W  0x00000000   Fast Slew Rate Function on GPADC
    volatile ADC_FN_TYPE        GPADCFN0;       // 0x230  R/W  0x00000000   Port Configuration on GPADC Output Data
    unsigned :32; unsigned :32; unsigned :32;

    volatile GPIO_PORT_TYPE     GPSDDAT;        // 0x240
    volatile GPIO_PORT_TYPE     GPSDEN;         // 0x244
    volatile GPIO_PORT_TYPE     GPSDSET;        // 0x248
    volatile GPIO_PORT_TYPE     GPSDCLR;        // 0x24C
    volatile GPIO_PORT_TYPE     GPSDXOR;        // 0x250
    volatile GPIO_CD0_TYPE      GPSDCD;         // 0x254
    unsigned :32;
    volatile GPIO_PORT_TYPE     GPSDPE;         // 0x25C
    volatile GPIO_PORT_TYPE     GPSDPS;         // 0x260
    volatile GPIO_PORT_TYPE     GPSDIEN;        // 0x264
    volatile GPIO_PORT_TYPE     GPSDIS;         // 0x268
    volatile GPIO_PORT_TYPE     GPSDSR;         // 0x26C
    volatile GPIO_FN0_TYPE      GPSDFN0;        // 0x270
    unsigned :32; unsigned :32; unsigned :32;

    volatile EINTSEL0_TYPE      EINTSEL0;       // 0x280  R/W  -            External Interrupt Select Register 0
    volatile EINTSEL1_TYPE      EINTSEL1;       // 0x284  R/W  -            External Interrupt Select Register 1
    volatile EINTSEL2_TYPE      EINTSEL2;       // 0x288  R/W  -            External Interrupt Select Register 2
    unsigned :32;

    volatile unsigned long      ECID0;          // 0x290  R/W  -            CID Output Register
    volatile unsigned long      ECID1;          // 0x294  R    -            CID Serial output data Register
    volatile unsigned long      ECID2;          // 0x298  R    -            CID Parallel output data 0 Register
    volatile unsigned long      ECID3;          // 0x29C  R    -            CID Parallel output data 1 Register
} GPIO, *PGPIO;






#endif //_SNOR_H

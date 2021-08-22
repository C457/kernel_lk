/*
 * Copyright (c) 2011 Telechips, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef _STRUCTURES_USB_H_
#define _STRUCTURES_USB_H_

/*******************************************************************************
*   5. USB 2.0 OTG Controller                          (Base Addr = 0x71B00000)
********************************************************************************/

typedef struct _USB20OTG{
    // Core Global CSR Map
    volatile unsigned int       GOTGCTL;            // 0x000  R/W   OTG Control and Status Register
    volatile unsigned int       GOTGINT;            // 0x004        OTG Interrupt Register
    volatile unsigned int       GAHBCFG;            // 0x008        Core AHB Configuration Register
    volatile unsigned int       GUSBCFG;            // 0x00C        Core USB Configuration register
    volatile unsigned int       GRSTCTL;            // 0x010        Core Reset Register
    volatile unsigned int       GINTSTS;            // 0x014        Core Interrupt Register
    volatile unsigned int       GINTMSK;            // 0x018        Core Interrupt Mask Register
    volatile unsigned int       GRXSTSR;            // 0x01C        Receive Status Debug Read register(Read Only)
    volatile unsigned int       GRXSTSP;            // 0x020        Receive Status Read /Pop register(Read Only)
    volatile unsigned int       GRXFSIZ;            // 0x024        Receive FIFO Size Register
    volatile unsigned int       GNPTXFSIZ;          // 0x028        Non-periodic Transmit FIFO Size register
    volatile unsigned int       GNPTXSTS;           // 0x02C        Non-periodic Transmit FIFO/Queue Status register (Read Only)
    volatile unsigned int       NOTDEFINE0[3];      // 0x030~       Reserved
    volatile unsigned int       GUID;               // 0x03C        User ID Register
    volatile unsigned int       NOTDEFINE1;         // 0x040        Reserved
    volatile unsigned int       GHWCFG1;            // 0x044        User HW Config1 Register(Read Only)
    volatile unsigned int       GHWCFG2;            // 0x048        User HW Config2 Register(Read Only)
    volatile unsigned int       GHWCFG3;            // 0x04C        User HW Config3 Register(Read Only)
    volatile unsigned int       GHWCFG4;            // 0x050        User HW Config4 Register(Read Only)
    volatile unsigned int       NOTDEFINE2[43];     // 0x054~       Reserved
    volatile unsigned int       HPTXFSIZ;           // 0x100        Host Periodic Transmit FIFO Size Register
    volatile unsigned int       DIEPTXFn[15];       // 0x104~       Device IN Endpoint Transmit FIFO Size register
    volatile unsigned int       NOTDEFINE3[176];    // 0x140~       Reserved
// Host Mode CSR Map
    volatile unsigned int       HCFG;               // 0x400        Host Configuration Register
    volatile unsigned int       HFIR;               // 0x404        Host Frame Interval Register
    volatile unsigned int       HFNUM;              // 0x408        Host Frame Number/Frame Time Remaining register
    volatile unsigned int       NOTDEFINE4;         // 0x40C        Reserved
    volatile unsigned int       HPTXSTS;            // 0x410        Host Periodic Transmit FIFO/Queue Status Register
    volatile unsigned int       HAINT;              // 0x414        Host All Channels Interrupt Register
    volatile unsigned int       HAINTMSK;           // 0x418        Host All Channels Interrupt Mask register
    volatile unsigned int       NOTDEFINE5[9];      // 0x41C~       Not Used
    volatile unsigned int       HPRT;               // 0x440        Host Port Control and Status register
    volatile unsigned int       NOTDEFINE6[47];     // 0x444~       Reserved
    volatile unsigned int       HCCHARn;            // 0x500        Host Channel 0 Characteristics Register
    volatile unsigned int       HCSPLTn;            // 0x504        Host Channel 0 Split Control Register
    volatile unsigned int       HCINTn;             // 0x508        Host Channel 0 Interrupt Register
    volatile unsigned int       HCINTMSKn;          // 0x50C        Host Channel 0 Interrupt Mask Register
    volatile unsigned int       HCTSIZn;            // 0x510        Host Channel 0 Transfer Size Register
    volatile unsigned int       HCDMAn;             // 0x514        Host Channel 0 DMA Address Register
    volatile unsigned int       NOTDEFINE7[2];      // 0x518~       Reserved
    volatile unsigned int       HCH1[8];            // 0x520~       Host Channel 1 Registers
    volatile unsigned int       HCH2[8];            // 0x540~       Host Channel 2 Registers
    volatile unsigned int       HCH3[8];            // 0x560~       Host Channel 3 Registers
    volatile unsigned int       HCH4[8];            // 0x580~       Host Channel 4 Registers
    volatile unsigned int       HCH5[8];            // 0x5A0~       Host Channel 5 Registers
    volatile unsigned int       HCH6[8];            // 0x5C0~       Host Channel 6 Registers
    volatile unsigned int       HCH7[8];            // 0x5E0~       Host Channel 7 Registers
    volatile unsigned int       HCH8[8];            // 0x600~       Host Channel 8 Registers
    volatile unsigned int       HCH9[8];            // 0x620~       Host Channel 9 Registers
    volatile unsigned int       HCH10[8];           // 0x640~       Host Channel 10 Registers
    volatile unsigned int       HCH11[8];           // 0x660~       Host Channel 11 Registers
    volatile unsigned int       HCH12[8];           // 0x680~       Host Channel 12 Registers
    volatile unsigned int       HCH13[8];           // 0x6A0~       Host Channel 13 Registers
    volatile unsigned int       HCH14[8];           // 0x6C0~       Host Channel 14 Registers
    volatile unsigned int       HCH15[8];           // 0x6E0~       Host Channel 15 Registers
    volatile unsigned int       NOTDEFINE8[64];     // 0x700~       Reserved
// Device Mode CSR Map
    volatile unsigned int       DCFG;               // 0x800        Device Configuration Register
    volatile unsigned int       DCTL;               // 0x804        Device Control Register
    volatile unsigned int       DSTS;               // 0x808        Device Status Register (Read Only)
    volatile unsigned int       NOTDEFINE9;         // 0x80C        Reserved
    volatile unsigned int       DIEPMSK;            // 0x810        Device IN Endpoint Common Interrupt Mask Register
    volatile unsigned int       DOEPMSK;            // 0x814        Device OUT Endpoint Common Interrupt Mask register
    volatile unsigned int       DAINT;              // 0x818        Device All Endpoints Interrupt Register
    volatile unsigned int       DAINTMSK;           // 0x81C        Device All Endpoints Interrupt Mask Register
    volatile unsigned int       NOTDEFINE10[2];     // 0x820~       Reserved
    volatile unsigned int       DVBUSDIS;           // 0x828        Device VBUS Discharge Time Register
    volatile unsigned int       DVBUSPULSE;         // 0x82C        Device VBUS Pulsing Time register
    volatile unsigned int       DTHRCTL;            // 0x830        Device Threshold Control register
    volatile unsigned int       DIEPEMPMSK;         // 0x834        Device IN Endpoint FIFO Empty Interrupt Mask register
    volatile unsigned int       NOTDEFINE11[50];    // 0x838~       Reserved

    volatile unsigned int       DIEPCTL0;           // 0x900        Device Control IN Endpoint 0 Control Register
    volatile unsigned int       NOTDEFINE12;        // 0x904        Reserved
    volatile unsigned int       DIEPINT0;           // 0x908        Device IN Endpoint 0 Interrupt Register
    volatile unsigned int       NOTDEFINE13;        // 0x90C        Reserved
    volatile unsigned int       DIEPTSIZ0;          // 0x910        Device IN Endpoint 0 Transfer Size register
    volatile unsigned int       DIEPDMA0;           // 0x914        Device IN Endpoint 0 DMA Address Register
    volatile unsigned int       DTXFSTS0;           // 0x918        Device IN Endpoint Transmit FIFO Status Register
    volatile unsigned int       NOTDEFINE14;        // 0x91C        Reserved

    volatile unsigned int       DEVINENDPT[15][8];  // 0x920~       Device IN Endpoint 1~15 Registers
    volatile unsigned int       DOEPCTL0;           // 0xB00        Device Control OUT Endpoint 0 Control register
    volatile unsigned int       NOTDEFINE15;        // 0xB04        Reserved
    volatile unsigned int       DOEPINT0;           // 0xB08        Device OUT Endpoint 0 Interrupt Register
    volatile unsigned int       NOTDEFINE16;        // 0xB0C        Reserved
    volatile unsigned int       DOEPTSIZ0;          // 0xB10        Device OUT Endpoint 0 Transfer Size Register
    volatile unsigned int       DOEPDMA0;           // 0xB14        Device OUT Endpoint 0 DMA Address register
    volatile unsigned int       NOTDEFINE17[2];     // 0xB18~       Reserved
    volatile unsigned int       DEVOUTENDPT[15][8]; // 0xB20~       Device OUT Endpoint 1~15 Registers
    volatile unsigned int       NOTDEFINE18[64];    // 0xD00~       Reserved
    // Power and Clock Gating CSR Map
    volatile unsigned int       PCGCR;              // 0xE00        Power and Clock Gating Control Register
    volatile unsigned int       NOTDEFINE19[127];   // 0xE04~       Reserved
    // Data FIFO(DFIFO) Access Register Map
    volatile unsigned int       DFIFOENDPT[16][1024];   // 0x1000~      Device IN Endpoint 0~16/Host Out Channel 0~16: DFIFO Write/Read Access
    //volatile unsigned int       DFIFOENDPT0[1024];  // 0x1000~      Device IN Endpoint 0/Host Out Channel 0: DFIFO Write/Read Access
    //volatile unsigned int       DFIFOENDPT1[1024];  // 0x2000~      Device IN Endpoint 1/Host Out Channel 1: DFIFO Write/Read Access
    //volatile unsigned int       DFIFOENDPT2[1024];  // 0x3000~      Device IN Endpoint 2/Host Out Channel 2: DFIFO Write/Read Access
    //volatile unsigned int       DFIFOENDPT3[1024];  // 0x4000~      Device IN Endpoint 3/Host Out Channel 3: DFIFO Write/Read Access
    //volatile unsigned int       DFIFOENDPT4[1024];  // 0x5000~      Device IN Endpoint 4/Host Out Channel 4: DFIFO Write/Read Access
    //volatile unsigned int       DFIFOENDPT5[1024];  // 0x6000~      Device IN Endpoint 5/Host Out Channel 5: DFIFO Write/Read Access
    //volatile unsigned int       DFIFOENDPT6[1024];  // 0x7000~      Device IN Endpoint 6/Host Out Channel 6: DFIFO Write/Read Access
    //volatile unsigned int       DFIFOENDPT7[1024];  // 0x8000~      Device IN Endpoint 7/Host Out Channel 7: DFIFO Write/Read Access
    //volatile unsigned int       DFIFOENDPT8[1024];  // 0x9000~      Device IN Endpoint 8/Host Out Channel 8: DFIFO Write/Read Access
    //volatile unsigned int       DFIFOENDPT9[1024];  // 0xA000~      Device IN Endpoint 9/Host Out Channel 9: DFIFO Write/Read Access
    //volatile unsigned int       DFIFOENDPT10[1024]; // 0xB000~      Device IN Endpoint 10/Host Out Channel 10: DFIFO Write/Read Access
    //volatile unsigned int       DFIFOENDPT11[1024]; // 0xC000~      Device IN Endpoint 11/Host Out Channel 11: DFIFO Write/Read Access
    //volatile unsigned int       DFIFOENDPT12[1024]; // 0xD000~      Device IN Endpoint 12/Host Out Channel 12: DFIFO Write/Read Access
    //volatile unsigned int       DFIFOENDPT13[1024]; // 0xE000~      Device IN Endpoint 13/Host Out Channel 13: DFIFO Write/Read Access
    //volatile unsigned int       DFIFOENDPT14[1024]; // 0xF000~      Device IN Endpoint 14/Host Out Channel 14: DFIFO Write/Read Access
    //volatile unsigned int       DFIFOENDPT15[1024]; // 0x10000~     Device IN Endpoint 15/Host Out Channel 15: DFIFO Write/Read Access
} USB20OTG, *PUSB20OTG;

typedef struct _USBOTG{
    // Core Global CSR Map
    volatile unsigned int       GOTGCTL;            // 0x000  R/W   OTG Control and Status Register
    volatile unsigned int       GOTGINT;            // 0x004        OTG Interrupt Register
    volatile unsigned int       GAHBCFG;            // 0x008        Core AHB Configuration Register
    volatile unsigned int       GUSBCFG;            // 0x00C        Core USB Configuration register
    volatile unsigned int       GRSTCTL;            // 0x010        Core Reset Register
    volatile unsigned int       GINTSTS;            // 0x014        Core Interrupt Register
    volatile unsigned int       GINTMSK;            // 0x018        Core Interrupt Mask Register
    volatile unsigned int       GRXSTSR;            // 0x01C        Receive Status Debug Read register(Read Only)
    volatile unsigned int       GRXSTSP;            // 0x020        Receive Status Read /Pop register(Read Only)
    volatile unsigned int       GRXFSIZ;            // 0x024        Receive FIFO Size Register
    volatile unsigned int       GNPTXFSIZ;          // 0x028        Non-periodic Transmit FIFO Size register
    volatile unsigned int       GNPTXSTS;           // 0x02C        Non-periodic Transmit FIFO/Queue Status register (Read Only)
    volatile unsigned int       NOTDEFINE0[3];      // 0x030~       Reserved
    volatile unsigned int       GUID;               // 0x03C        User ID Register
    volatile unsigned int       NOTDEFINE1;         // 0x040        Reserved
    volatile unsigned int       GHWCFG1;            // 0x044        User HW Config1 Register(Read Only)
    volatile unsigned int       GHWCFG2;            // 0x048        User HW Config2 Register(Read Only)
    volatile unsigned int       GHWCFG3;            // 0x04C        User HW Config3 Register(Read Only)
    volatile unsigned int       GHWCFG4;            // 0x050        User HW Config4 Register(Read Only)
    volatile unsigned int       NOTDEFINE2[43];     // 0x054~       Reserved
    volatile unsigned int       HPTXFSIZ;           // 0x100        Host Periodic Transmit FIFO Size Register
    volatile unsigned int       DIEPTXFn[15];       // 0x104~       Device IN Endpoint Transmit FIFO Size register
    volatile unsigned int       NOTDEFINE3[176];    // 0x140~       Reserved
    // Host Mode CSR Map
    volatile unsigned int       HCFG;               // 0x400        Host Configuration Register
    volatile unsigned int       HFIR;               // 0x404        Host Frame Interval Register
    volatile unsigned int       HFNUM;              // 0x408        Host Frame Number/Frame Time Remaining register
    volatile unsigned int       NOTDEFINE4;         // 0x40C        Reserved
    volatile unsigned int       HPTXSTS;            // 0x410        Host Periodic Transmit FIFO/Queue Status Register
    volatile unsigned int       HAINT;              // 0x414        Host All Channels Interrupt Register
    volatile unsigned int       HAINTMSK;           // 0x418        Host All Channels Interrupt Mask register
    volatile unsigned int       NOTDEFINE5[9];      // 0x41C~       Not Used
    volatile unsigned int       HPRT;               // 0x440        Host Port Control and Status register
    volatile unsigned int       NOTDEFINE6[47];     // 0x444~       Reserved
    volatile unsigned int       HCCHARn;            // 0x500        Host Channel 0 Characteristics Register
    volatile unsigned int       HCSPLTn;            // 0x504        Host Channel 0 Split Control Register
    volatile unsigned int       HCINTn;             // 0x508        Host Channel 0 Interrupt Register
    volatile unsigned int       HCINTMSKn;          // 0x50C        Host Channel 0 Interrupt Mask Register
    volatile unsigned int       HCTSIZn;            // 0x510        Host Channel 0 Transfer Size Register
    volatile unsigned int       HCDMAn;             // 0x514        Host Channel 0 DMA Address Register
    volatile unsigned int       NOTDEFINE7[2];      // 0x518~       Reserved
    volatile unsigned int       HCH1[8];            // 0x520~       Host Channel 1 Registers
    volatile unsigned int       HCH2[8];            // 0x540~       Host Channel 2 Registers
    volatile unsigned int       HCH3[8];            // 0x560~       Host Channel 3 Registers
    volatile unsigned int       HCH4[8];            // 0x580~       Host Channel 4 Registers
    volatile unsigned int       HCH5[8];            // 0x5A0~       Host Channel 5 Registers
    volatile unsigned int       HCH6[8];            // 0x5C0~       Host Channel 6 Registers
    volatile unsigned int       HCH7[8];            // 0x5E0~       Host Channel 7 Registers
    volatile unsigned int       HCH8[8];            // 0x600~       Host Channel 8 Registers
    volatile unsigned int       HCH9[8];            // 0x620~       Host Channel 9 Registers
    volatile unsigned int       HCH10[8];           // 0x640~       Host Channel 10 Registers
    volatile unsigned int       HCH11[8];           // 0x660~       Host Channel 11 Registers
    volatile unsigned int       HCH12[8];           // 0x680~       Host Channel 12 Registers
    volatile unsigned int       HCH13[8];           // 0x6A0~       Host Channel 13 Registers
    volatile unsigned int       HCH14[8];           // 0x6C0~       Host Channel 14 Registers
    volatile unsigned int       HCH15[8];           // 0x6E0~       Host Channel 15 Registers
    volatile unsigned int       NOTDEFINE8[64];     // 0x700~       Reserved
    // Device Mode CSR Map
    volatile unsigned int       DCFG;               // 0x800        Device Configuration Register
    volatile unsigned int       DCTL;               // 0x804        Device Control Register
    volatile unsigned int       DSTS;               // 0x808        Device Status Register (Read Only)
    volatile unsigned int       NOTDEFINE9;         // 0x80C        Reserved
    volatile unsigned int       DIEPMSK;            // 0x810        Device IN Endpoint Common Interrupt Mask Register
    volatile unsigned int       DOEPMSK;            // 0x814        Device OUT Endpoint Common Interrupt Mask register
    volatile unsigned int       DAINT;              // 0x818        Device All Endpoints Interrupt Register
    volatile unsigned int       DAINTMSK;           // 0x81C        Device All Endpoints Interrupt Mask Register
    volatile unsigned int       NOTDEFINE10[2];     // 0x820~       Reserved
    volatile unsigned int       DVBUSDIS;           // 0x828        Device VBUS Discharge Time Register
    volatile unsigned int       DVBUSPULSE;         // 0x82C        Device VBUS Pulsing Time register
    volatile unsigned int       DTHRCTL;            // 0x830        Device Threshold Control register
    volatile unsigned int       DIEPEMPMSK;         // 0x834        Device IN Endpoint FIFO Empty Interrupt Mask register
    volatile unsigned int       NOTDEFINE11[50];    // 0x838~       Reserved

    //volatile unsigned int     DIEPCTL0;           // 0x900        Device Control IN Endpoint 0 Control Register
    //volatile unsigned int     NOTDEFINE12;        // 0x904        Reserved
    //volatile unsigned int     DIEPINT0;           // 0x908        Device IN Endpoint 0 Interrupt Register
    //volatile unsigned int     NOTDEFINE13;        // 0x90C        Reserved
    //volatile unsigned int     DIEPTSIZ0;          // 0x910        Device IN Endpoint 0 Transfer Size register
    //volatile unsigned int     DIEPDMA0;           // 0x914        Device IN Endpoint 0 DMA Address Register
    //volatile unsigned int     DTXFSTS0;           // 0x918        Device IN Endpoint Transmit FIFO Status Register
    //volatile unsigned int     NOTDEFINE14;        // 0x91C        Reserved

    volatile unsigned int       DEVINENDPT[16][8];  // 0x900~       Device IN Endpoint 1~15 Registers

    //volatile unsigned int     DOEPCTL0;           // 0xB00        Device Control OUT Endpoint 0 Control register
    //volatile unsigned int     NOTDEFINE15;        // 0xB04        Reserved
    //volatile unsigned int     DOEPINT0;           // 0xB08        Device OUT Endpoint 0 Interrupt Register
    //volatile unsigned int     NOTDEFINE16;        // 0xB0C        Reserved
    //volatile unsigned int     DOEPTSIZ0;          // 0xB10        Device OUT Endpoint 0 Transfer Size Register
    //volatile unsigned int     DOEPDMA0;           // 0xB14        Device OUT Endpoint 0 DMA Address register
    //volatile unsigned int     NOTDEFINE17[2];     // 0xB18~       Reserved

    volatile unsigned int       DEVOUTENDPT[16][8]; // 0xB00~       Device OUT Endpoint 1~15 Registers
    volatile unsigned int       NOTDEFINE18[64];    // 0xD00~       Reserved

    // Power and Clock Gating CSR Map
    volatile unsigned int       PCGCR;              // 0xE00        Power and Clock Gating Control Register
    volatile unsigned int       NOTDEFINE19[127];   // 0xE04~       Reserved
    // Data FIFO(DFIFO) Access Register Map
    volatile unsigned int       DFIFOENDPT[16][1024];   // 0x1000~      Device IN Endpoint 0~16/Host Out Channel 0~16: DFIFO Write/Read Access
    //volatile unsigned int     DFIFOENDPT0[1024];  // 0x1000~      Device IN Endpoint 0/Host Out Channel 0: DFIFO Write/Read Access
    //volatile unsigned int     DFIFOENDPT1[1024];  // 0x2000~      Device IN Endpoint 1/Host Out Channel 1: DFIFO Write/Read Access
    //volatile unsigned int     DFIFOENDPT2[1024];  // 0x3000~      Device IN Endpoint 2/Host Out Channel 2: DFIFO Write/Read Access
    //volatile unsigned int     DFIFOENDPT3[1024];  // 0x4000~      Device IN Endpoint 3/Host Out Channel 3: DFIFO Write/Read Access
    //volatile unsigned int     DFIFOENDPT4[1024];  // 0x5000~      Device IN Endpoint 4/Host Out Channel 4: DFIFO Write/Read Access
    //volatile unsigned int     DFIFOENDPT5[1024];  // 0x6000~      Device IN Endpoint 5/Host Out Channel 5: DFIFO Write/Read Access
    //volatile unsigned int     DFIFOENDPT6[1024];  // 0x7000~      Device IN Endpoint 6/Host Out Channel 6: DFIFO Write/Read Access
    //volatile unsigned int     DFIFOENDPT7[1024];  // 0x8000~      Device IN Endpoint 7/Host Out Channel 7: DFIFO Write/Read Access
    //volatile unsigned int     DFIFOENDPT8[1024];  // 0x9000~      Device IN Endpoint 8/Host Out Channel 8: DFIFO Write/Read Access
    //volatile unsigned int     DFIFOENDPT9[1024];  // 0xA000~      Device IN Endpoint 9/Host Out Channel 9: DFIFO Write/Read Access
    //volatile unsigned int     DFIFOENDPT10[1024]; // 0xB000~      Device IN Endpoint 10/Host Out Channel 10: DFIFO Write/Read Access
    //volatile unsigned int     DFIFOENDPT11[1024]; // 0xC000~      Device IN Endpoint 11/Host Out Channel 11: DFIFO Write/Read Access
    //volatile unsigned int     DFIFOENDPT12[1024]; // 0xD000~      Device IN Endpoint 12/Host Out Channel 12: DFIFO Write/Read Access
    //volatile unsigned int     DFIFOENDPT13[1024]; // 0xE000~      Device IN Endpoint 13/Host Out Channel 13: DFIFO Write/Read Access
    //volatile unsigned int     DFIFOENDPT14[1024]; // 0xF000~      Device IN Endpoint 14/Host Out Channel 14: DFIFO Write/Read Access
    //volatile unsigned int     DFIFOENDPT15[1024]; // 0x10000~     Device IN Endpoint 15/Host Out Channel 15: DFIFO Write/Read Access
} USBOTG, *PUSBOTG;

typedef struct _USBPHYCFG
{
	volatile unsigned	UPCR0;		// 0x100  R/W    USB PHY Configuration Register0
	volatile unsigned	UPCR1;		// 0x104  R/W    USB PHY Configuration Register1
	volatile unsigned	UPCR2;		// 0x108  R/W    USB PHY Configuration Register2
	volatile unsigned	UPCR3;		// 0x10C  R/W    USB PHY Configuration Register3
	volatile unsigned	UPCR4;		// 0x110  R/W    USB PHY Configuration Register3
	volatile unsigned	LSTS;		// 0x114  R/W    USB PHY Configuration Register3
	volatile unsigned	LCFG;		// 0x118  R/W    USB PHY Configuration Register3
} *PUSBPHYCFG;
#endif /* _STRUCTURES_USB_H_ */

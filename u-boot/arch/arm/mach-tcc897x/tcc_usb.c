
#include <common.h>
#include <asm/arch/iomap.h>
#include <asm/arch/usb/reg_physical.h>
#include <asm/arch/gpio.h>
#include <usb.h>
#include <usb/dwc2_udc.h>

struct dwc2_plat_otg_data tcc_otg_data = {
    .regs_phy   = HwUSBPHYCFG_BASE,
    .regs_otg   = CONFIG_USB_DWC2_REG_ADDR,
};

void tcc_dwc_otg_phy(int on_off)
{
	PUSBPHYCFG USBPHYCFG = (PUSBPHYCFG) HwUSBPHYCFG_BASE;

	if (on_off == ON) {
		debug("[%s] enable\n", __func__);
		USBPHYCFG->UPCR0 = 0x03000015;
		USBPHYCFG->UPCR1 = 0x0330D543;
		USBPHYCFG->UPCR2 = 0xC0;
		USBPHYCFG->UPCR3 = 0x0;
		USBPHYCFG->UPCR4 = 0x0;
		USBPHYCFG->LCFG  = 0x30000000;

		tcc_set_hsiobus_swreset(HSIOBUS_DWC_OTG, 1);

		USBPHYCFG->LCFG = 0x0;							// assert prstn, adp_reset_n
		USBPHYCFG->UPCR0 &= ~(Hw20|Hw25|Hw24|Hw31);		//disable PHYVALID_EN -> no irq, SIDDQ, POR
		udelay(100);
		USBPHYCFG->LCFG |= Hw29;					// prstn

		tcc_set_hsiobus_swreset(HSIOBUS_DWC_OTG, 0);
	} else {
		debug("[%s] disable\n", __func__);
		PUSBPHYCFG USBPHYCFG = (PUSBPHYCFG) HwUSBPHYCFG_BASE;
		BITCSET(USBPHYCFG->UPCR2, Hw10|Hw9/*UPCR2_OPMODE_MASK*/, Hw9/*UPCR2_OPMODE_NON_DRVING*/);
		USBPHYCFG->UPCR0 = 0x4840;
		USBPHYCFG->UPCR0 = 0x6940;
	}
}

void tcc_dwc_otg_vbus_ctrl(int on_off)
{
#ifndef CONFIG_DAUDIO_KK
	gpio_config(GPIO_OTG_VBUS,  GPIO_FN0|GPIO_OUTPUT);
	gpio_set(GPIO_OTG_VBUS, on_off);
#endif
}

int board_usb_init(int index, enum usb_init_type init)
{
    switch (index) {
    case 0:	// dwc_otg
        if (init == USB_INIT_HOST) {
			// USB INIT Host
			tcc_dwc_otg_phy(ON);
			tcc_dwc_otg_vbus_ctrl(ON);

        } else {
			tcc_dwc_otg_vbus_ctrl(OFF);
		#if defined(CONFIG_USB_GADGET_DWC2_OTG)
			dwc2_udc_probe(&tcc_otg_data);
		#endif
			// USB INIT Device
        }

        break;
    case 1:	// xhci
        if (init == USB_INIT_HOST) {
			// USB INIT Host
        } else {
			// USB INIT Device
        }

        break;
    default:
        printf("Invalid Controller Index\n");
    }

    return 0;
}

void otg_phy_init(void)
{
	tcc_dwc_otg_phy(ON);
}

void otg_phy_off(void)
{
	tcc_dwc_otg_phy(OFF);
}

int g_dnl_get_board_bcd_device_number(int gcnum)
{
	return 0x0100;
}

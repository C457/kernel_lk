
#include <common.h>
#include <asm/arch/usb/usb_manager.h>

extern char FirmwareVersion[];
extern void USB_Isr(void);

int check_fwdn_mode(void)
{
	if ( FirmwareVersion[3] != '?')
		return 0;
	else
		return 1;
}

void fwdn_start(void)
{
	printf("start: fwdn mode\n");
	USB_Init(USBDEV_VTC);
	USB_DEVICE_On();

	for(;;)
	{
#if !defined(FEATURE_USB_ISR)
		USB_Isr();
#endif
		USB_DEVICE_Run();
	}
}

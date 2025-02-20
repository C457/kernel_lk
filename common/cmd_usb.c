/*
 * (C) Copyright 2001
 * Denis Peter, MPL AG Switzerland
 *
 * Adapted for U-Boot driver model
 * (C) Copyright 2015 Google, Inc
 *
 * Most of this source has been derived from the Linux USB
 * project.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <command.h>
#include <console.h>
#include <dm.h>
#include <memalign.h>
#include <asm/byteorder.h>
#include <asm/unaligned.h>
#include <part.h>
#include <usb.h>

#include <asm/arch/sdmmc/sd_bus.h>
#ifdef CONFIG_USB_UPDATE
#include <asm/arch/fwdn/Disk.h>
#include <asm/arch/sfl.h>
#include <mobis/verify_update_image.h>

#if defined(UPDATE_DEBUG_LOG_FBCON)
#define fbout(fmt, args...) fbprintf(fmt, ##args)
#else
#define fbout(fmt, args...) do {} while (0)
#endif

#define DEBUG_LOG
#ifdef DEBUG_LOG
#define update_debug(fmt, args...)		\
	do {					\
		printf(fmt, ##args);		\
		fbout(fmt, ##args);		\
	} while (0)
#else
#define update_debug(fmt, args...) do {} while (0)
#endif
int erase_emmc(unsigned long start_addr, unsigned long erase_size, int low_format);
#endif
#ifdef CONFIG_USB_STORAGE
static int usb_stor_curr_dev = -1; /* current device */
#endif
#if defined(CONFIG_USB_HOST_ETHER) && !defined(CONFIG_DM_ETH)
static int __maybe_unused usb_ether_curr_dev = -1; /* current ethernet device */
#endif

/* some display routines (info command) */
static char *usb_get_class_desc(unsigned char dclass)
{
	switch (dclass) {
	case USB_CLASS_PER_INTERFACE:
		return "See Interface";
	case USB_CLASS_AUDIO:
		return "Audio";
	case USB_CLASS_COMM:
		return "Communication";
	case USB_CLASS_HID:
		return "Human Interface";
	case USB_CLASS_PRINTER:
		return "Printer";
	case USB_CLASS_MASS_STORAGE:
		return "Mass Storage";
	case USB_CLASS_HUB:
		return "Hub";
	case USB_CLASS_DATA:
		return "CDC Data";
	case USB_CLASS_VENDOR_SPEC:
		return "Vendor specific";
	default:
		return "";
	}
}

static void usb_display_class_sub(unsigned char dclass, unsigned char subclass,
				  unsigned char proto)
{
	switch (dclass) {
	case USB_CLASS_PER_INTERFACE:
		printf("See Interface");
		break;
	case USB_CLASS_HID:
		printf("Human Interface, Subclass: ");
		switch (subclass) {
		case USB_SUB_HID_NONE:
			printf("None");
			break;
		case USB_SUB_HID_BOOT:
			printf("Boot ");
			switch (proto) {
			case USB_PROT_HID_NONE:
				printf("None");
				break;
			case USB_PROT_HID_KEYBOARD:
				printf("Keyboard");
				break;
			case USB_PROT_HID_MOUSE:
				printf("Mouse");
				break;
			default:
				printf("reserved");
				break;
			}
			break;
		default:
			printf("reserved");
			break;
		}
		break;
	case USB_CLASS_MASS_STORAGE:
		printf("Mass Storage, ");
		switch (subclass) {
		case US_SC_RBC:
			printf("RBC ");
			break;
		case US_SC_8020:
			printf("SFF-8020i (ATAPI)");
			break;
		case US_SC_QIC:
			printf("QIC-157 (Tape)");
			break;
		case US_SC_UFI:
			printf("UFI");
			break;
		case US_SC_8070:
			printf("SFF-8070");
			break;
		case US_SC_SCSI:
			printf("Transp. SCSI");
			break;
		default:
			printf("reserved");
			break;
		}
		printf(", ");
		switch (proto) {
		case US_PR_CB:
			printf("Command/Bulk");
			break;
		case US_PR_CBI:
			printf("Command/Bulk/Int");
			break;
		case US_PR_BULK:
			printf("Bulk only");
			break;
		default:
			printf("reserved");
			break;
		}
		break;
	default:
		printf("%s", usb_get_class_desc(dclass));
		break;
	}
}

static void usb_display_string(struct usb_device *dev, int index)
{
	ALLOC_CACHE_ALIGN_BUFFER(char, buffer, 256);

	if (index != 0) {
		if (usb_string(dev, index, &buffer[0], 256) > 0)
			printf("String: \"%s\"", buffer);
	}
}

static void usb_display_desc(struct usb_device *dev)
{
	if (dev->descriptor.bDescriptorType == USB_DT_DEVICE) {
		printf("%d: %s,  USB Revision %x.%x\n", dev->devnum,
		usb_get_class_desc(dev->config.if_desc[0].desc.bInterfaceClass),
				   (dev->descriptor.bcdUSB>>8) & 0xff,
				   dev->descriptor.bcdUSB & 0xff);

		if (strlen(dev->mf) || strlen(dev->prod) ||
		    strlen(dev->serial))
			printf(" - %s %s %s\n", dev->mf, dev->prod,
				dev->serial);
		if (dev->descriptor.bDeviceClass) {
			printf(" - Class: ");
			usb_display_class_sub(dev->descriptor.bDeviceClass,
					      dev->descriptor.bDeviceSubClass,
					      dev->descriptor.bDeviceProtocol);
			printf("\n");
		} else {
			printf(" - Class: (from Interface) %s\n",
			       usb_get_class_desc(
				dev->config.if_desc[0].desc.bInterfaceClass));
		}
		printf(" - PacketSize: %d  Configurations: %d\n",
			dev->descriptor.bMaxPacketSize0,
			dev->descriptor.bNumConfigurations);
		printf(" - Vendor: 0x%04x  Product 0x%04x Version %d.%d\n",
			dev->descriptor.idVendor, dev->descriptor.idProduct,
			(dev->descriptor.bcdDevice>>8) & 0xff,
			dev->descriptor.bcdDevice & 0xff);
	}

}

static void usb_display_conf_desc(struct usb_config_descriptor *config,
				  struct usb_device *dev)
{
	printf("   Configuration: %d\n", config->bConfigurationValue);
	printf("   - Interfaces: %d %s%s%dmA\n", config->bNumInterfaces,
	       (config->bmAttributes & 0x40) ? "Self Powered " : "Bus Powered ",
	       (config->bmAttributes & 0x20) ? "Remote Wakeup " : "",
		config->bMaxPower*2);
	if (config->iConfiguration) {
		printf("   - ");
		usb_display_string(dev, config->iConfiguration);
		printf("\n");
	}
}

static void usb_display_if_desc(struct usb_interface_descriptor *ifdesc,
				struct usb_device *dev)
{
	printf("     Interface: %d\n", ifdesc->bInterfaceNumber);
	printf("     - Alternate Setting %d, Endpoints: %d\n",
		ifdesc->bAlternateSetting, ifdesc->bNumEndpoints);
	printf("     - Class ");
	usb_display_class_sub(ifdesc->bInterfaceClass,
		ifdesc->bInterfaceSubClass, ifdesc->bInterfaceProtocol);
	printf("\n");
	if (ifdesc->iInterface) {
		printf("     - ");
		usb_display_string(dev, ifdesc->iInterface);
		printf("\n");
	}
}

static void usb_display_ep_desc(struct usb_endpoint_descriptor *epdesc)
{
	printf("     - Endpoint %d %s ", epdesc->bEndpointAddress & 0xf,
		(epdesc->bEndpointAddress & 0x80) ? "In" : "Out");
	switch ((epdesc->bmAttributes & 0x03)) {
	case 0:
		printf("Control");
		break;
	case 1:
		printf("Isochronous");
		break;
	case 2:
		printf("Bulk");
		break;
	case 3:
		printf("Interrupt");
		break;
	}
	printf(" MaxPacket %d", get_unaligned(&epdesc->wMaxPacketSize));
	if ((epdesc->bmAttributes & 0x03) == 0x3)
		printf(" Interval %dms", epdesc->bInterval);
	printf("\n");
}

/* main routine to diasplay the configs, interfaces and endpoints */
static void usb_display_config(struct usb_device *dev)
{
	struct usb_config *config;
	struct usb_interface *ifdesc;
	struct usb_endpoint_descriptor *epdesc;
	int i, ii;

	config = &dev->config;
	usb_display_conf_desc(&config->desc, dev);
	for (i = 0; i < config->no_of_if; i++) {
		ifdesc = &config->if_desc[i];
		usb_display_if_desc(&ifdesc->desc, dev);
		for (ii = 0; ii < ifdesc->no_of_ep; ii++) {
			epdesc = &ifdesc->ep_desc[ii];
			usb_display_ep_desc(epdesc);
		}
	}
	printf("\n");
}

/*
 * With driver model this isn't right since we can have multiple controllers
 * and the device numbering starts at 1 on each bus.
 * TODO(sjg@chromium.org): Add a way to specify the controller/bus.
 */
static struct usb_device *usb_find_device(int devnum)
{
#ifdef CONFIG_DM_USB
	struct usb_device *udev;
	struct udevice *hub;
	struct uclass *uc;
	int ret;

	/* Device addresses start at 1 */
	devnum++;
	ret = uclass_get(UCLASS_USB_HUB, &uc);
	if (ret)
		return NULL;

	uclass_foreach_dev(hub, uc) {
		struct udevice *dev;

		if (!device_active(hub))
			continue;
		udev = dev_get_parent_priv(hub);
		if (udev->devnum == devnum)
			return udev;

		for (device_find_first_child(hub, &dev);
		     dev;
		     device_find_next_child(&dev)) {
			if (!device_active(hub))
				continue;

			udev = dev_get_parent_priv(dev);
			if (udev->devnum == devnum)
				return udev;
		}
	}
#else
	struct usb_device *udev;
	int d;

	for (d = 0; d < USB_MAX_DEVICE; d++) {
		udev = usb_get_dev_index(d);
		if (udev == NULL)
			return NULL;
		if (udev->devnum == devnum)
			return udev;
	}
#endif

	return NULL;
}

static inline char *portspeed(int speed)
{
	char *speed_str;

	switch (speed) {
	case USB_SPEED_SUPER:
		speed_str = "5 Gb/s";
		break;
	case USB_SPEED_HIGH:
		speed_str = "480 Mb/s";
		break;
	case USB_SPEED_LOW:
		speed_str = "1.5 Mb/s";
		break;
	default:
		speed_str = "12 Mb/s";
		break;
	}

	return speed_str;
}

/* shows the device tree recursively */
static void usb_show_tree_graph(struct usb_device *dev, char *pre)
{
	int index;
	int has_child, last_child;

	index = strlen(pre);
	printf(" %s", pre);
#ifdef CONFIG_DM_USB
	has_child = device_has_active_children(dev->dev);
#else
	/* check if the device has connected children */
	int i;

	has_child = 0;
	for (i = 0; i < dev->maxchild; i++) {
		if (dev->children[i] != NULL)
			has_child = 1;
	}
#endif
	/* check if we are the last one */
#ifdef CONFIG_DM_USB
	/* Not the root of the usb tree? */
	if (device_get_uclass_id(dev->dev->parent) != UCLASS_USB) {
		last_child = device_is_last_sibling(dev->dev);
#else
	if (dev->parent != NULL) { /* not root? */
		last_child = 1;
		for (i = 0; i < dev->parent->maxchild; i++) {
			/* search for children */
			if (dev->parent->children[i] == dev) {
				/* found our pointer, see if we have a
				 * little sister
				 */
				while (i++ < dev->parent->maxchild) {
					if (dev->parent->children[i] != NULL) {
						/* found a sister */
						last_child = 0;
						break;
					} /* if */
				} /* while */
			} /* device found */
		} /* for all children of the parent */
#endif
		printf("\b+-");
		/* correct last child */
		if (last_child && index)
			pre[index-1] = ' ';
	} /* if not root hub */
	else
		printf(" ");
	printf("%d ", dev->devnum);
	pre[index++] = ' ';
	pre[index++] = has_child ? '|' : ' ';
	pre[index] = 0;
	printf(" %s (%s, %dmA)\n", usb_get_class_desc(
					dev->config.if_desc[0].desc.bInterfaceClass),
					portspeed(dev->speed),
					dev->config.desc.bMaxPower * 2);
	if (strlen(dev->mf) || strlen(dev->prod) || strlen(dev->serial))
		printf(" %s  %s %s %s\n", pre, dev->mf, dev->prod, dev->serial);
	printf(" %s\n", pre);
#ifdef CONFIG_DM_USB
	struct udevice *child;

	for (device_find_first_child(dev->dev, &child);
	     child;
	     device_find_next_child(&child)) {
		struct usb_device *udev;

		if (!device_active(child))
			continue;

		udev = dev_get_parent_priv(child);

		/* Ignore emulators, we only want real devices */
		if (device_get_uclass_id(child) != UCLASS_USB_EMUL) {
			usb_show_tree_graph(udev, pre);
			pre[index] = 0;
		}
	}
#else
	if (dev->maxchild > 0) {
		for (i = 0; i < dev->maxchild; i++) {
			if (dev->children[i] != NULL) {
				usb_show_tree_graph(dev->children[i], pre);
				pre[index] = 0;
			}
		}
	}
#endif
}

/* main routine for the tree command */
static void usb_show_subtree(struct usb_device *dev)
{
	char preamble[32];

	memset(preamble, '\0', sizeof(preamble));
	usb_show_tree_graph(dev, &preamble[0]);
}

void usb_show_tree(void)
{
#ifdef CONFIG_DM_USB
	struct udevice *bus;

	for (uclass_first_device(UCLASS_USB, &bus);
		bus;
		uclass_next_device(&bus)) {
		struct usb_device *udev;
		struct udevice *dev;

		device_find_first_child(bus, &dev);
		if (dev && device_active(dev)) {
			udev = dev_get_parent_priv(dev);
			usb_show_subtree(udev);
		}
	}
#else
	struct usb_device *udev;
	int i;

	for (i = 0; i < USB_MAX_DEVICE; i++) {
		udev = usb_get_dev_index(i);
		if (udev == NULL)
			break;
		if (udev->parent == NULL)
			usb_show_subtree(udev);
	}
#endif
}

static int usb_test(struct usb_device *dev, int port, char* arg)
{
	int mode;

	if (port > dev->maxchild) {
		printf("Device is no hub or does not have %d ports.\n", port);
		return 1;
	}

	switch (arg[0]) {
	case 'J':
	case 'j':
		printf("Setting Test_J mode");
		mode = USB_TEST_MODE_J;
		break;
	case 'K':
	case 'k':
		printf("Setting Test_K mode");
		mode = USB_TEST_MODE_K;
		break;
	case 'S':
	case 's':
		printf("Setting Test_SE0_NAK mode");
		mode = USB_TEST_MODE_SE0_NAK;
		break;
	case 'P':
	case 'p':
		printf("Setting Test_Packet mode");
		mode = USB_TEST_MODE_PACKET;
		break;
	case 'F':
	case 'f':
		printf("Setting Test_Force_Enable mode");
		mode = USB_TEST_MODE_FORCE_ENABLE;
		break;
	default:
		printf("Unrecognized test mode: %s\nAvailable modes: "
		       "J, K, S[E0_NAK], P[acket], F[orce_Enable]\n", arg);
		return 1;
	}

	if (port)
		printf(" on downstream facing port %d...\n", port);
	else
		printf(" on upstream facing port...\n");

	if (usb_control_msg(dev, usb_sndctrlpipe(dev, 0), USB_REQ_SET_FEATURE,
			    port ? USB_RT_PORT : USB_RECIP_DEVICE,
			    port ? USB_PORT_FEAT_TEST : USB_FEAT_TEST,
			    (mode << 8) | port,
			    NULL, 0, USB_CNTL_TIMEOUT) == -1) {
		printf("Error during SET_FEATURE.\n");
		return 1;
	} else {
		printf("Test mode successfully set. Use 'usb start' "
		       "to return to normal operation.\n");
		return 0;
	}
}


/******************************************************************************
 * usb boot command intepreter. Derived from diskboot
 */
#ifdef CONFIG_USB_STORAGE
static int do_usbboot(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	return common_diskboot(cmdtp, "usb", argc, argv);
}
#endif /* CONFIG_USB_STORAGE */

static int do_usb_stop_keyboard(int force)
{
#ifdef CONFIG_USB_KEYBOARD
	if (usb_kbd_deregister(force) != 0) {
		printf("USB not stopped: usbkbd still using USB\n");
		return 1;
	}
#endif
	return 0;
}

static void do_usb_start(void)
{
	bootstage_mark_name(BOOTSTAGE_ID_USB_START, "usb_start");

	if (usb_init() < 0)
		return;

	/* Driver model will probe the devices as they are found */
#ifndef CONFIG_DM_USB
# ifdef CONFIG_USB_STORAGE
	/* try to recognize storage devices immediately */
	usb_stor_curr_dev = usb_stor_scan(1);
# endif
# ifdef CONFIG_USB_KEYBOARD
	drv_usb_kbd_init();
# endif
#endif /* !CONFIG_DM_USB */
#ifdef CONFIG_USB_HOST_ETHER
# ifdef CONFIG_DM_ETH
#  ifndef CONFIG_DM_USB
#   error "You must use CONFIG_DM_USB if you want to use CONFIG_USB_HOST_ETHER with CONFIG_DM_ETH"
#  endif
# else
	/* try to recognize ethernet devices immediately */
	usb_ether_curr_dev = usb_host_eth_scan(1);
# endif
#endif
}

#ifdef CONFIG_DM_USB
static void show_info(struct udevice *dev)
{
	struct udevice *child;
	struct usb_device *udev;

	udev = dev_get_parent_priv(dev);
	usb_display_desc(udev);
	usb_display_config(udev);
	for (device_find_first_child(dev, &child);
	     child;
	     device_find_next_child(&child)) {
		if (device_active(child))
			show_info(child);
	}
}

static int usb_device_info(void)
{
	struct udevice *bus;

	for (uclass_first_device(UCLASS_USB, &bus);
	     bus;
	     uclass_next_device(&bus)) {
		struct udevice *hub;

		device_find_first_child(bus, &hub);
		if (device_get_uclass_id(hub) == UCLASS_USB_HUB &&
		    device_active(hub)) {
			show_info(hub);
		}
	}

	return 0;
}
#endif

/******************************************************************************
 * usb command intepreter
 */
static int do_usb(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	struct usb_device *udev = NULL;
	int i;
	extern char usb_started;
#ifdef CONFIG_USB_STORAGE
	block_dev_desc_t *stor_dev;
#endif

	if (argc < 2)
		return CMD_RET_USAGE;

	if (strncmp(argv[1], "start", 5) == 0) {
		if (usb_started)
			return 0; /* Already started */
		printf("starting USB...\n");
		do_usb_start();
		return 0;
	}

	if (strncmp(argv[1], "reset", 5) == 0) {
		printf("resetting USB...\n");
		if (do_usb_stop_keyboard(1) != 0)
			return 1;
		usb_stop();
		do_usb_start();
		return 0;
	}
	if (strncmp(argv[1], "stop", 4) == 0) {
		if (argc != 2)
			console_assign(stdin, "serial");
		if (do_usb_stop_keyboard(0) != 0)
			return 1;
		printf("stopping USB..\n");
		usb_stop();
		return 0;
	}
	if (!usb_started) {
		printf("USB is stopped. Please issue 'usb start' first.\n");
		return 1;
	}
	if (strncmp(argv[1], "tree", 4) == 0) {
		puts("USB device tree:\n");
		usb_show_tree();
		return 0;
	}
	if (strncmp(argv[1], "inf", 3) == 0) {
		if (argc == 2) {
#ifdef CONFIG_DM_USB
			usb_device_info();
#else
			int d;
			for (d = 0; d < USB_MAX_DEVICE; d++) {
				udev = usb_get_dev_index(d);
				if (udev == NULL)
					break;
				usb_display_desc(udev);
				usb_display_config(udev);
			}
#endif
			return 0;
		} else {
			/*
			 * With driver model this isn't right since we can
			 * have multiple controllers and the device numbering
			 * starts at 1 on each bus.
			 */
			i = simple_strtoul(argv[2], NULL, 10);
			printf("config for device %d\n", i);
			udev = usb_find_device(i);
			if (udev == NULL) {
				printf("*** No device available ***\n");
				return 0;
			} else {
				usb_display_desc(udev);
				usb_display_config(udev);
			}
		}
		return 0;
	}
	if (strncmp(argv[1], "test", 4) == 0) {
		if (argc < 5)
			return CMD_RET_USAGE;
		i = simple_strtoul(argv[2], NULL, 10);
		udev = usb_find_device(i);
		if (udev == NULL) {
			printf("Device %d does not exist.\n", i);
			return 1;
		}
		i = simple_strtoul(argv[3], NULL, 10);
		return usb_test(udev, i, argv[4]);
	}
#ifdef CONFIG_USB_STORAGE
	if (strncmp(argv[1], "stor", 4) == 0)
		return usb_stor_info();

	if (strncmp(argv[1], "part", 4) == 0) {
		int devno, ok = 0;
		if (argc == 2) {
			for (devno = 0; ; ++devno) {
				stor_dev = usb_stor_get_dev(devno);
				if (stor_dev == NULL)
					break;
				if (stor_dev->type != DEV_TYPE_UNKNOWN) {
					ok++;
					if (devno)
						printf("\n");
					debug("print_part of %x\n", devno);
					print_part(stor_dev);
				}
			}
		} else {
			devno = simple_strtoul(argv[2], NULL, 16);
			stor_dev = usb_stor_get_dev(devno);
			if (stor_dev != NULL &&
			    stor_dev->type != DEV_TYPE_UNKNOWN) {
				ok++;
				debug("print_part of %x\n", devno);
				print_part(stor_dev);
			}
		}
		if (!ok) {
			printf("\nno USB devices available\n");
			return 1;
		}
		return 0;
	}
	if (strcmp(argv[1], "read") == 0) {
		if (usb_stor_curr_dev < 0) {
			printf("no current device selected\n");
			return 1;
		}
		if (argc == 5) {
			unsigned long addr = simple_strtoul(argv[2], NULL, 16);
			unsigned long blk  = simple_strtoul(argv[3], NULL, 16);
			unsigned long cnt  = simple_strtoul(argv[4], NULL, 16);
			unsigned long n;
			printf("\nUSB read: device %d block # %ld, count %ld"
				" ... ", usb_stor_curr_dev, blk, cnt);
			stor_dev = usb_stor_get_dev(usb_stor_curr_dev);
			n = stor_dev->block_read(usb_stor_curr_dev, blk, cnt,
						 (ulong *)addr);
			printf("%ld blocks read: %s\n", n,
				(n == cnt) ? "OK" : "ERROR");
			if (n == cnt)
				return 0;
			return 1;
		}
	}
	if (strcmp(argv[1], "write") == 0) {
		if (usb_stor_curr_dev < 0) {
			printf("no current device selected\n");
			return 1;
		}
		if (argc == 5) {
			unsigned long addr = simple_strtoul(argv[2], NULL, 16);
			unsigned long blk  = simple_strtoul(argv[3], NULL, 16);
			unsigned long cnt  = simple_strtoul(argv[4], NULL, 16);
			unsigned long n;
			printf("\nUSB write: device %d block # %ld, count %ld"
				" ... ", usb_stor_curr_dev, blk, cnt);
			stor_dev = usb_stor_get_dev(usb_stor_curr_dev);
			n = stor_dev->block_write(usb_stor_curr_dev, blk, cnt,
						(ulong *)addr);
			printf("%ld blocks write: %s\n", n,
				(n == cnt) ? "OK" : "ERROR");
			if (n == cnt)
				return 0;
			return 1;
		}
	}
	if (strncmp(argv[1], "dev", 3) == 0) {
		if (argc == 3) {
			int dev = (int)simple_strtoul(argv[2], NULL, 10);
			printf("\nUSB device %d: ", dev);
			stor_dev = usb_stor_get_dev(dev);
			if (stor_dev == NULL) {
				printf("unknown device\n");
				return 1;
			}
			printf("\n    Device %d: ", dev);
			dev_print(stor_dev);
			if (stor_dev->type == DEV_TYPE_UNKNOWN)
				return 1;
			usb_stor_curr_dev = dev;
			printf("... is now current device\n");
			return 0;
		} else {
			printf("\nUSB device %d: ", usb_stor_curr_dev);
			stor_dev = usb_stor_get_dev(usb_stor_curr_dev);
			dev_print(stor_dev);
			if (stor_dev->type == DEV_TYPE_UNKNOWN)
				return 1;
			return 0;
		}
		return 0;
	}
#endif /* CONFIG_USB_STORAGE */
	return CMD_RET_USAGE;
}

#ifdef CONFIG_USB_UPDATE
#include <fs.h>

unsigned long usb_fat_read(unsigned long addr, unsigned long bytes, unsigned long pos)
{
	const char *filename = UPDATE_FILE_NAME;
	int ret = -1;
	loff_t len_read = 0;

	if (fs_set_blk_dev("usb", "0:1", 1))
		return -1;

	ret = fs_read(filename, addr, (loff_t)pos, (loff_t)bytes, (loff_t*)&len_read);
	if (ret < 0) {
		 update_debug("[ERROR:%s] read data ret %d \n",__func__,ret);
		 len_read = -1;
	}

	return (unsigned long)len_read;
}

unsigned long usb_fat_read_bootloader(unsigned long addr, unsigned long bytes, unsigned long pos)
{
	const char *filename = UPDATE_BOOTLOADER_NAME;
	int ret = -1;
	loff_t len_read;

	if (fs_set_blk_dev("usb", "0:1", 1))
		return -1;

	ret = fs_read(filename, addr, pos, bytes, (loff_t*)&len_read);
	if (ret < 0) {
		 update_debug("[ERROR:%s] read data ret %d \n",__func__,ret);
		 len_read = -1;
	}

	return (unsigned long)len_read;
}

#define OVER_FLOW 		1
#define NORM_COMPLETE 	0

int write_DiskImage_to_eMMC(char* fileaddr, int imageSize, unsigned int usb_offset)
{
	char* cur_point = fileaddr;
	unsigned long long partSize, destAddress;
	unsigned int offset = 0;
	unsigned int unit_size = 0;
	unsigned int file_size = getenv_hex("data_fai_size", 0);
	int ret = NORM_COMPLETE;

	while(imageSize > 0)
	{
		destAddress = 0;
		partSize = 0;

		memcpy(&destAddress, cur_point, 8);
		cur_point += 8;
		memcpy(&partSize, cur_point, 8);
		cur_point += 8;

		/*partition data insufficient*/
		if( (offset + partSize + 16) > UPDATE_BUFFER_SIZE ) {
			if(!offset){	//first data overflow
				ret = OVER_FLOW;
				break;
			}
			return offset;
		}

		imageSize -= (partSize+16);

		if(partSize == 0){
			offset += 16;
			continue;
		}

		if(DISK_WriteSector(DISK_DEVICE_TRIFLASH, 0,
			BYTE_TO_SECTOR(destAddress), BYTE_TO_SECTOR(partSize), cur_point))
		{
			update_debug("eMMC write fail\n");
			return -1;
		}

		update_debug("\r%u/%u bytes", usb_offset + offset, file_size);

		cur_point += partSize;
		offset += (partSize + 16);
	}

	if(ret == OVER_FLOW)//offset == 0
	{
		usb_offset += 16;

		while(partSize > 0){
			unit_size = (unsigned int)(partSize > UPDATE_BUFFER_SIZE)?UPDATE_BUFFER_SIZE:partSize;

			if(!usb_fat_read((unsigned long)fileaddr, unit_size, usb_offset)){
				update_debug("\x1b[1;31m [SD DATA UPDATE]FAT read error!!  \x1b[0m\n");
				return -1;
			}

			if(DISK_WriteSector(DISK_DEVICE_TRIFLASH, 0,
				(unsigned long)BYTE_TO_SECTOR(destAddress), (unsigned long)BYTE_TO_SECTOR(unit_size), fileaddr))
			{
				update_debug("eMMC write fail\n");
				return -1;
			}

			offset += unit_size;
			usb_offset += unit_size;
			partSize -= unit_size;
			destAddress += unit_size;

			update_debug("\r%u/%u bytes", usb_offset, file_size);
		}

		return offset+16;
	}
	ret = offset;

	return ret;
}

char *buf_addr_for_bootloader = NULL;
int do_usbupdate(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	char *usb_argv[6];
	ulong loadaddr = simple_strtoul(argv[1], NULL, 16);
	unsigned int file_size = 0;
	unsigned int remain_data = 0;
	unsigned int headersize = 0;
	unsigned int offset = 0;
	unsigned long time = 0;
	int ret = -1;
	int bootloader_size = 0;
	buf_addr_for_bootloader = (char*)loadaddr;
	unsigned long long dummy = 0;
	update_debug("bootloader buffer address(0x%08X)\n", (unsigned int)buf_addr_for_bootloader);
	update_debug("\n=== USB update start ===\n");

	/*usb initailization*/
	usb_argv[0] = "usb";
	usb_argv[1] = "start";

	mdelay(1000); 	//for usb compatibility
	if(do_usb(NULL, 0, 2, usb_argv)){
		update_debug("USB init fail!\n");
		return -1;
	}
	//for usb compatibility
	usb_argv[1] = "part";
	if(do_usb(NULL, 0, 2, usb_argv)){
		update_debug("USB scan fail!\n");
		usb_argv[1] = "start";
		mdelay(1000);
		update_debug("retry scan usb\n");
		if(do_usb(NULL, 0, 2, usb_argv)){
			update_debug("USB init fail!\n");
			return -1;
		}
	}

#ifdef CONFIG_MOBIS_CHECK_RSA
	update_debug("\n[%s][%d]\n",__FUNCTION__,__LINE__);
	ret = mobis_verify_image((unsigned long)loadaddr);
	if(ret!=0)
	{
		update_debug("\n=== RSA signature failed ===\n");
		return -1;
	}
	update_debug("\n[%s][%d], RSA signature success!!!\n",__FUNCTION__,__LINE__);
#endif

	/*get tag strings [0:7]*/
	update_debug("\n=== prepare for update ===\n");
	update_debug("Check update file valid\n");
	
	ret = usb_fat_read((unsigned long)loadaddr , 8, offset);
	if (ret > 0) { 
		if (memcmp((char*)loadaddr, "[HEADER]", 8)){
			//buf_addr[8] = '\0';
			update_debug("[SD DATA UPDATE] Tag Reading Error!\n");
			return -1;
		}
		offset += 8;
	} else {
		update_debug("[DEBUG:%s] File is not find #1 \n",__func__);
	}

	/*get header size [8:11]*/
	ret = usb_fat_read((unsigned long)loadaddr, 4, offset);
	if (ret > 0) { 
		memcpy(&headersize, (char*)loadaddr, 4);
		if(headersize != 0x60)
		{
			update_debug("[SD DATA UPDATE] Headersize is wrong! )size = 0x%x\n",headersize);
			return -1;
		}
	} else {
		update_debug("[DEBUG:%s] File is not find #2 \n",__func__);
	}

	/*check the area name*/
	offset = 0x30;
	ret = usb_fat_read((unsigned long)loadaddr, 7, offset);
	if (ret > 0) { 
		if (memcmp((char*)loadaddr, "SD Data", 7)){
			update_debug("[SD DATA UPDATE] Area name reading Error!\n");
			return -1;
		}
	} else {
		update_debug("[DEBUG:%s] File is not find #3 \n",__func__);
	}

	file_size = getenv_hex("data_fai_size", 0);
	update_debug("[SD DATA UPDATE] %s size = %u\n", UPDATE_FILE_NAME, file_size);
	if(file_size > 0)
	{
		offset = 0x60;
		remain_data = file_size - headersize;
		time = get_timer(0);

		/* remove all emmc area for update.fai update */
		if(remain_data){
			if( !erase_emmc(0, 0, 1) ) {
				update_debug("Erase eMMC finished!\n");
			} else {
				update_debug("Erase eMMC failed...\n");
				goto out;
			}
		}

		//update_debug("eMMC update start !!!\n");
		update_debug("\n=== Write SD Data(%s) ===\n", UPDATE_FILE_NAME);
		while(remain_data>0)
		{
			unsigned int writen_size = 0;
			unsigned int partial_size = (remain_data>UPDATE_BUFFER_SIZE)?UPDATE_BUFFER_SIZE:remain_data;

			if(!usb_fat_read(loadaddr,partial_size,offset)){
				update_debug("\x1b[1;31m [SD DATA UPDATE]FAT read error!!  \x1b[0m\n");
				goto bootloader_update;
			}

			writen_size = write_DiskImage_to_eMMC((char*)loadaddr, partial_size, offset);
			if(writen_size == -1){
				update_debug("\x1b[1;31m [SD DATA UPDATE]failed to write!! \x1b[0m\n");
				goto bootloader_update;
			}

			offset		+=	writen_size;
			remain_data	-=	writen_size;
			update_debug("\r%u/%u bytes",offset, file_size);
		}
		time = get_timer(time);
		update_debug("\r%u/%u bytes\n",file_size, file_size);
		update_debug("[SD DATA UPDATE] %d bytes update in %lu ms\n", file_size, time);
	}
	else
	{
		update_debug("[SD DATA UPDATE] There is no data(%s) file.\n", UPDATE_FILE_NAME);
		return -1;
	}
bootloader_update:

	/* Disable to update Bootloader. 20.07.28 
	bootloader_size = usb_fat_read_bootloader((unsigned long)buf_addr_for_bootloader, bootloader_size, 0);

	if (bootloader_size > 0)
	{
		int res=0;
		update_debug("=== Write bootloader(%s) ===\n", UPDATE_BOOTLOADER_NAME);
		res = tcc_write("bootloader", dummy, bootloader_size, buf_addr_for_bootloader);
		if (res < 0)
		{
			update_debug("[BOOTLOADER UPDATE] failed to write bootloader!\n");
			return -1;
		}
	}
	else
	{
		update_debug("[BOOTLOADER UPDATE] There is no bootloader(%s) file.\n", UPDATE_BOOTLOADER_NAME);
		return -1;
	}
	*/
	update_debug("=== USB update complete ===\n\n");
out:
	return 0;
}

int usbupdate(void)
{
    char * argv[5];
	int ret = 0;
    //dcache_enable();
    argv[1] = UPDATE_BUFFER_ADDR;

	/* display proper color to check update progress */
    if(do_usbupdate((cmd_tbl_t *)NULL,0,0, argv) < 0){

		ret = -1;
    }else{
	}

	/* notification for update succeeded */
	if( ret == 0)
	{
		update_debug("Please disconnect USB connection.\n");
	}

	/* after uboot update and reset cpu */
	do_reset(NULL, 0, 0, NULL);

    return ret;

}

U_BOOT_CMD(
	usbupdate,	3,	1,	do_usbupdate,
	"update from USB device to eMMC",
	"usbupdate [addr] [file-name]"
);
#endif /* CONFIG_USB_UPDATE */

U_BOOT_CMD(
	usb,	5,	1,	do_usb,
	"USB sub-system",
	"start - start (scan) USB controller\n"
	"usb reset - reset (rescan) USB controller\n"
	"usb stop [f] - stop USB [f]=force stop\n"
	"usb tree - show USB device tree\n"
	"usb info [dev] - show available USB devices\n"
	"usb test [dev] [port] [mode] - set USB 2.0 test mode\n"
	"    (specify port 0 to indicate the device's upstream port)\n"
	"    Available modes: J, K, S[E0_NAK], P[acket], F[orce_Enable]\n"
#ifdef CONFIG_USB_STORAGE
	"usb storage - show details of USB storage devices\n"
	"usb dev [dev] - show or set current USB storage device\n"
	"usb part [dev] - print partition table of one or all USB storage"
	"    devices\n"
	"usb read addr blk# cnt - read `cnt' blocks starting at block `blk#'\n"
	"    to memory address `addr'\n"
	"usb write addr blk# cnt - write `cnt' blocks starting at block `blk#'\n"
	"    from memory address `addr'"
#endif /* CONFIG_USB_STORAGE */
);


#ifdef CONFIG_USB_STORAGE
U_BOOT_CMD(
	usbboot,	3,	1,	do_usbboot,
	"boot from USB device",
	"loadAddr dev:part"
);
#endif /* CONFIG_USB_STORAGE */

/*
 * Samsung N130, N220, N230 NC10, Np-Q45, R468/R418, X320/X420/X520, X360,
 * R518, R510/P510, R410, NF110/NF210/NF310, N350 and R470/R420 
 * Laptop Backlight driver
 *
 * Copyright (C) 2011 Peter Savichev (proton) (psavichev@gmail.com)
 * Copyright (C) 2010 xonatius (xonatius@gmail.com)
 * Copyright (C) 2010 elemc, sergem (fedora@conference.jabber.ru)
 * Copyright (C) 2009 Greg Kroah-Hartman (gregkh@suse.de)
 * Copyright (C) 2009 Novell Inc.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 *
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/backlight.h>
#include <linux/fb.h>
#include <linux/dmi.h>

#define MAX_BRIGHT      0x07
#define OFFSET          0xf4

/*
 * HAL/gnome-display-manager really wants us to only set 8 different levels for
 * the brightness control.  And since 256 different levels seems a bit
 * overkill, that's fine.  So let's map the 256 values to 8 different ones:
 *
 * userspace     0    1    2    3    4    5    6    7
 * hardware     31   63   95  127  159  195  223  255
 *
 * or hardware = ((userspace + 1) * 32)-1
 *
 * Note, we keep value 0 at a positive value, otherwise the screen goes
 * blank because HAL likes to set the backlight to 0 at startup when there is
 * no power plugged in.
 */


static int offset = OFFSET;
module_param(offset, int, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(offset, "The offset into the PCI device for the brightness control");

static struct pci_dev *pci_device;
static struct backlight_device *backlight_device;

static u8 read_brightness(void)
{
        u8 kernel_brightness;
        u8 user_brightness = 0;

        pci_read_config_byte(pci_device, offset, &kernel_brightness);
        user_brightness = ((kernel_brightness + 1) / 32) - 1;
        return user_brightness;
}

static void set_brightness(u8 user_brightness)
{
        u16 kernel_brightness = 0;

        kernel_brightness = ((user_brightness + 1) * 32) - 1;
        pci_write_config_byte(pci_device, offset, (u8)kernel_brightness);
}

static int get_brightness(struct backlight_device *bd)
{
        return bd->props.brightness;
}

static int update_status(struct backlight_device *bd)
{
        set_brightness(bd->props.brightness);
        return 0;
}

static struct backlight_ops backlight_ops = {
        .get_brightness = get_brightness,
        .update_status  = update_status,
};

static int __init dmi_check_cb(const struct dmi_system_id *id)
{
        printk(KERN_INFO KBUILD_MODNAME ": found laptop model '%s'\n",
                id->ident);
        return 0;
}

static struct dmi_system_id __initdata samsung_dmi_table[] = {
        {
                .ident = "N120",
                .matches = {
                        DMI_MATCH(DMI_SYS_VENDOR, "SAMSUNG ELECTRONICS CO., LTD."),
                        DMI_MATCH(DMI_PRODUCT_NAME, "N120"),
                        DMI_MATCH(DMI_BOARD_NAME, "N120"),
                },
                .callback = dmi_check_cb,
        },
        {
                .ident = "N130",
                .matches = {
                        DMI_MATCH(DMI_SYS_VENDOR, "SAMSUNG ELECTRONICS CO., LTD."),
                        DMI_MATCH(DMI_PRODUCT_NAME, "N130"),
                        DMI_MATCH(DMI_BOARD_NAME, "N130"),
                },
                .callback = dmi_check_cb,
        },
        {
                .ident = "NC10",
                .matches = {
                        DMI_MATCH(DMI_SYS_VENDOR, "SAMSUNG ELECTRONICS CO., LTD."),
                        DMI_MATCH(DMI_PRODUCT_NAME, "NC10"),
                        DMI_MATCH(DMI_BOARD_NAME, "NC10"),
                },
                .callback = dmi_check_cb,
        },
        {
                .ident = "NP-Q45",
                .matches = {
                        DMI_MATCH(DMI_SYS_VENDOR, "SAMSUNG ELECTRONICS CO., LTD."),
                        DMI_MATCH(DMI_PRODUCT_NAME, "SQ45S70S"),
                        DMI_MATCH(DMI_BOARD_NAME, "SQ45S70S"),
                },
                .callback = dmi_check_cb,
        },
        {
                .ident = "R410 Plus",
                .matches = {
                        DMI_MATCH(DMI_SYS_VENDOR, "SAMSUNG ELECTRONICS CO., LTD."),
                        DMI_MATCH(DMI_PRODUCT_NAME, "R410P"),
                        DMI_MATCH(DMI_BOARD_NAME, "R460"),
                },
                .callback = dmi_check_cb,
        },
        {
                .ident = "R468/R418",
                .matches = {
                        DMI_MATCH(DMI_SYS_VENDOR, "SAMSUNG ELECTRONICS CO., LTD."),
                        DMI_MATCH(DMI_PRODUCT_NAME, "R468/R418"),
                        DMI_MATCH(DMI_BOARD_NAME, "R468/R418"),
                },
                .callback = dmi_check_cb,
        },
        {
                .ident = "X320/X420/X520",
                .matches = {
                        DMI_MATCH(DMI_SYS_VENDOR, "SAMSUNG ELECTRONICS CO., LTD."),
                        DMI_MATCH(DMI_PRODUCT_NAME, "X320/X420/X520"),
                        DMI_MATCH(DMI_BOARD_NAME, "X320/X420/X520"),
                },
                .callback = dmi_check_cb,
        },
        {
                .ident = "X360",
                .matches = {
                        DMI_MATCH(DMI_SYS_VENDOR, "SAMSUNG ELECTRONICS CO., LTD."),
                        DMI_MATCH(DMI_PRODUCT_NAME, "X360"),
                        DMI_MATCH(DMI_BOARD_NAME, "X360"),
                },
                .callback = dmi_check_cb,
        },
        {
                .ident = "R518",
                .matches = {
                        DMI_MATCH(DMI_SYS_VENDOR, "SAMSUNG ELECTRONICS CO., LTD."),
                        DMI_MATCH(DMI_PRODUCT_NAME, "R518"),
                        DMI_MATCH(DMI_BOARD_NAME, "R518"),
                },
                .callback = dmi_check_cb,
        },
        {
                .ident = "R510/P510",
                .matches = {
                        DMI_MATCH(DMI_SYS_VENDOR, "SAMSUNG ELECTRONICS CO., LTD."),
                        DMI_MATCH(DMI_PRODUCT_NAME, "R510/P510"),
                        DMI_MATCH(DMI_BOARD_NAME, "R510/P510"),
                },
                .callback = dmi_check_cb,
        },
        {
                .ident = "NF110/NF210/NF310",
                .matches = {
                         DMI_MATCH(DMI_SYS_VENDOR, "SAMSUNG ELECTRONICS CO., LTD."),
                         DMI_MATCH(DMI_PRODUCT_NAME, "NF110/NF210/NF310"),
                         DMI_MATCH(DMI_BOARD_NAME, "NF110/NF210/NF310"),
                },
                .callback = dmi_check_cb,
        },
        {
                .ident = "N220",
                .matches = {
                         DMI_MATCH(DMI_SYS_VENDOR, "SAMSUNG ELECTRONICS CO., LTD."),
                         DMI_MATCH(DMI_PRODUCT_NAME, "N220"),
                         DMI_MATCH(DMI_BOARD_NAME, "N220"),
                },
                .callback = dmi_check_cb,
        },
        {
                .ident = "N230",
                .matches = {
                         DMI_MATCH(DMI_SYS_VENDOR, "SAMSUNG ELECTRONICS CO., LTD."),
                         DMI_MATCH(DMI_PRODUCT_NAME, "N230"),
                         DMI_MATCH(DMI_BOARD_NAME, "N230"),
                },
                .callback = dmi_check_cb,
        },
        {
                .ident = "N350",
                .matches = {
                         DMI_MATCH(DMI_SYS_VENDOR, "SAMSUNG ELECTRONICS CO., LTD."),
                         DMI_MATCH(DMI_PRODUCT_NAME, "N350"),
                         DMI_MATCH(DMI_BOARD_NAME, "N350"),
                },
                .callback = dmi_check_cb,
        },
        {
                .ident = "R470/R420",
                .matches = {
                         DMI_MATCH(DMI_SYS_VENDOR, "SAMSUNG ELECTRONICS CO., LTD."),
                         DMI_MATCH(DMI_PRODUCT_NAME, "R470/R420"),
                         DMI_MATCH(DMI_BOARD_NAME, "R470/R420"),
                },
                .callback = dmi_check_cb,
        },
        { },
};

static int __init samsung_init(void)
{
        struct backlight_properties props;
        memset(&props, 0, sizeof(struct backlight_properties));

        if (!dmi_check_system(samsung_dmi_table))
                return -ENODEV;

        /*
         * The Samsung N120, N130, and NC10 use pci device id 0x27ae, while the
         * NP-Q45 uses 0x2a02
         * R410P, R468/R418, R518, R510/P510, X320/X420/X520, X360 and R470/R420 
         * uses 0x2a42
         * N220, NF110/NF210/NF310 and N350 uses 0xa011
         * Odds are we might need to add more to the
         * list over time...
         */
        pci_device = pci_get_device(PCI_VENDOR_ID_INTEL, 0x27ae, NULL);
        if (!pci_device) 
                pci_device = pci_get_device(PCI_VENDOR_ID_INTEL, 0x2a02, NULL);
        if (!pci_device)
                pci_device = pci_get_device(PCI_VENDOR_ID_INTEL, 0x2a42, NULL);
        if (!pci_device)
                pci_device = pci_get_device(PCI_VENDOR_ID_INTEL, 0xa011, NULL);
        if (!pci_device)
                return -ENODEV;
        

        /* create a backlight device to talk to this one */
        backlight_device = backlight_device_register("samsung",
                                                     &pci_device->dev,
                                                     NULL, &backlight_ops, &props);
        if (IS_ERR(backlight_device)) {
                pci_dev_put(pci_device);
                return PTR_ERR(backlight_device);
        }

        backlight_device->props.max_brightness = MAX_BRIGHT;
        backlight_device->props.brightness = read_brightness();
        backlight_device->props.power = FB_BLANK_UNBLANK;
        backlight_update_status(backlight_device);

        return 0;
}

static void __exit samsung_exit(void)
{
        backlight_device_unregister(backlight_device);

        /* we are done with the PCI device, put it back */
        pci_dev_put(pci_device);
}

module_init(samsung_init);
module_exit(samsung_exit);

MODULE_AUTHOR("Greg Kroah-Hartman <gregkh@suse.de>");
MODULE_DESCRIPTION("Samsung Backlight driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("dmi:*:svnSAMSUNGELECTRONICSCO.,LTD.:pnN120:*:rnN120:*");
MODULE_ALIAS("dmi:*:svnSAMSUNGELECTRONICSCO.,LTD.:pnN130:*:rnN130:*");
MODULE_ALIAS("dmi:*:svnSAMSUNGELECTRONICSCO.,LTD.:pnNC10:*:rnNC10:*");
MODULE_ALIAS("dmi:*:svnSAMSUNGELECTRONICSCO.,LTD.:pnSQ45S70S:*:rnSQ45S70S:*");
MODULE_ALIAS("dmi:*:svnSAMSUNGELECTRONICSCO.,LTD.:pnR410P:*:rnR410P:*");
MODULE_ALIAS("dmi:*:svnSAMSUNGELECTRONICSCO.,LTD.:pnR468/R418:*:rnR468/R418:*");
MODULE_ALIAS("dmi:*:svnSAMSUNGELECTRONICSCO.,LTD.:pnX320/X420/X520:*:rnX320/X420/X520:*");
MODULE_ALIAS("dmi:*:svnSAMSUNGELECTRONICSCO.,LTD.:pnX360:*:rnX360:*");
MODULE_ALIAS("dmi:*:svnSAMSUNGELECTRONICSCO.,LTD.:pnR518:*:rnR518:*");
MODULE_ALIAS("dmi:*:svnSAMSUNGELECTRONICSCO.,LTD.:pnR510/P510:*:rnR510/P510:*");
MODULE_ALIAS("dmi:*:svnSAMSUNGELECTRONICSCO.,LTD.:pnNF110/NF210/NF310:*:rnNF110/NF210/NF310:*");
MODULE_ALIAS("dmi:*:svnSAMSUNGELECTRONICSCO.,LTD.:pnN220:*:rnN220:*");
MODULE_ALIAS("dmi:*:svnSAMSUNGELECTRONICSCO.,LTD.:pnN230:*:rnN230:*");
MODULE_ALIAS("dmi:*:svnSAMSUNGELECTRONICSCO.,LTD.:pnN350:*:rnN350:*");
MODULE_ALIAS("dmi:*:svnSAMSUNGELECTRONICSCO.,LTD.:pnR470/R420:*:rnR470/R420:*");

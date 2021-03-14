/*
 * tcc_lcd.h - Telechips LCD Controller structures
 *
 * (C) Copyright 2001
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef _TCC_LCD_H_
#define _TCC_LCD_H_

struct lcd_platform_data {
// power control pin
	unsigned power_on;
	unsigned display_on;
	unsigned bl_on;
	unsigned reset;

	unsigned lcdc_num;
};

typedef struct vidinfo {
	const char *name;
	const char *manufacturer;

	struct lcd_platform_data dev;

	int id; 		/* panel ID */
	int xres;		/* x resolution in pixels */
	int yres;		/* y resolution in pixels */
	int width;		/* display width in mm */
	int height; 	/* display height in mm */
	int bpp;		/* bits per pixels */

	int clk_freq;
	int clk_div;
	int bus_width;
	int lpw;		/* line pulse width */
	int lpc;		/* line pulse count */
	int lswc;		/* line start wait clock */
	int lewc;		/* line end wait clock */
	int vdb;		/* back porch vsync delay */
	int vdf;		/* front porch vsync delay */
	int fpw1;		/* frame pulse width 1 */
	int flc1;		/* frame line count 1 */
	int fswc1;		/* frame start wait cycle 1 */
	int fewc1;		/* frame end wait cycle 1 */
	int fpw2;		/* frame pulse width 2 */
	int flc2;		/* frame line count 2 */
	int fswc2;		/* frame start wait cycle 2 */
	int fewc2;		/* frame end wait cycle 2 */
	int sync_invert;

	int (*init)(struct lcd_panel *panel);
	int (*set_power)(struct lcd_panel *panel, int on);
	int (*set_backlight_level)(struct lcd_panel *panel, int level);

} vidinfo_t;

void init_panel_info(vidinfo_t *vid);

#endif

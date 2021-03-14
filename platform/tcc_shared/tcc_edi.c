/****************************************************************************
 *
 * Copyright (C) 2014 Telechips Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions
 * andlimitations under the License.
 ****************************************************************************/

#include <reg.h>
#include <globals.h>
#include <platform/reg_physical.h>

#define EDI_CTRL	0x00
#define EDI_CSNCFG0	0x04
#define EDI_CSNCFG1	0x08
#define EDI_OENCFG	0x0C
#define EDI_WENCFG	0x10
#define EDI_RDYCFG	0x14
#define EDI_REQOFF	0x20

void edi_init( void )
{
	void __iomem *edi_base	= (void __iomem *)HwEDI_BASE;
	unsigned reg_values;

	reg_values = readl(edi_base+EDI_RDYCFG) & ~(0xFFFFF);
	writel(reg_values | 0x32104, edi_base+EDI_RDYCFG);

	reg_values = readl(edi_base+EDI_CSNCFG0) & ~(0xFFFF);
	writel(reg_values | 0x8765, edi_base+EDI_CSNCFG0);

	reg_values = readl(edi_base+EDI_OENCFG) & ~(0xF);
	writel(reg_values | 0x1, edi_base+EDI_OENCFG);

	reg_values = readl(edi_base+EDI_WENCFG) & ~(0xF);
	writel(reg_values | 0x1, edi_base+EDI_WENCFG);
}

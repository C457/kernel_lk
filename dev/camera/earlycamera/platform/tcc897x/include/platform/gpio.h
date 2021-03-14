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

#ifndef __PLATFORM_GPIO_H
#define __PLATFORM_GPIO_H

#define GPIO_REG_SHIFT	6
#define GPIO_REGMASK	(0xF<<GPIO_REG_SHIFT)
#define GPIO_BITMASK	(0x3F<<0)

#define GPIO_PORTA	(0 << GPIO_REG_SHIFT)
#define GPIO_PORTB	(1 << GPIO_REG_SHIFT)
#define GPIO_PORTC	(2 << GPIO_REG_SHIFT)
#define GPIO_PORTD	(3 << GPIO_REG_SHIFT)
#define GPIO_PORTE	(4 << GPIO_REG_SHIFT)
#define GPIO_PORTF	(5 << GPIO_REG_SHIFT)
#define GPIO_PORTG	(6 << GPIO_REG_SHIFT)
#define GPIO_PORTHDMI	(7 << GPIO_REG_SHIFT)
#define GPIO_PORTSD3	(8 << GPIO_REG_SHIFT)
#define GPIO_PORTSD0	(9 << GPIO_REG_SHIFT)
#define GPIO_PORTEXT1	(10 << GPIO_REG_SHIFT)
#define GPIO_PORTEXT2	(11 << GPIO_REG_SHIFT)

#define TCC_GPA(x)	(GPIO_PORTA | (x))
#define TCC_GPB(x)	(GPIO_PORTB | (x))
#define TCC_GPC(x)	(GPIO_PORTC | (x))
#define TCC_GPD(x)	(GPIO_PORTD | (x))
#define TCC_GPE(x)	(GPIO_PORTE | (x))
#define TCC_GPF(x)	(GPIO_PORTF | (x))
#define TCC_GPG(x)	(GPIO_PORTG | (x))
#define TCC_GPHDMI(x)	(GPIO_PORTHDMI | (x))
#define TCC_GPSD3(x)	(GPIO_PORTSD3 | (x))
#define TCC_GPSD0(x)	(GPIO_PORTSD0 | (x))
#define TCC_GPEXT1(x)	(GPIO_PORTEXT1 | (x))
#define TCC_GPEXT2(x)	(GPIO_PORTEXT2 | (x))

#define GPIO_FN_SHIFT	27
#define GPIO_FN_BITMASK	(0x1F<<GPIO_FN_SHIFT)
#define GPIO_FN(x)	((x+1) << GPIO_FN_SHIFT)
#define GPIO_FN0	( 1 << GPIO_FN_SHIFT)
#define GPIO_FN1	( 2 << GPIO_FN_SHIFT)
#define GPIO_FN2	( 3 << GPIO_FN_SHIFT)
#define GPIO_FN3	( 4 << GPIO_FN_SHIFT)
#define GPIO_FN4	( 5 << GPIO_FN_SHIFT)
#define GPIO_FN5	( 6 << GPIO_FN_SHIFT)
#define GPIO_FN6	( 7 << GPIO_FN_SHIFT)
#define GPIO_FN7	( 8 << GPIO_FN_SHIFT)
#define GPIO_FN8	( 9 << GPIO_FN_SHIFT)
#define GPIO_FN9	(10 << GPIO_FN_SHIFT)
#define GPIO_FN10	(11 << GPIO_FN_SHIFT)
#define GPIO_FN11	(12 << GPIO_FN_SHIFT)
#define GPIO_FN12	(13 << GPIO_FN_SHIFT)
#define GPIO_FN13	(14 << GPIO_FN_SHIFT)
#define GPIO_FN14	(15 << GPIO_FN_SHIFT)
#define GPIO_FN15	(16 << GPIO_FN_SHIFT)

#define GPIO_CD_SHIFT	24
#define GPIO_CD_BITMASK	(0x7<<GPIO_CD_SHIFT)
#define GPIO_CD(x)	((x+1) << GPIO_CD_SHIFT)
#define GPIO_CD0	(1 << GPIO_CD_SHIFT)
#define GPIO_CD1	(2 << GPIO_CD_SHIFT)
#define GPIO_CD2	(3 << GPIO_CD_SHIFT)
#define GPIO_CD3	(4 << GPIO_CD_SHIFT)

#define GPIO_INPUT	0x0001
#define GPIO_OUTPUT	0x0002

#define GPIO_LEVEL	0x0000
#define GPIO_EDGE	0x0010

#define GPIO_RISING	0x0020
#define GPIO_FALLING	0x0040

#define GPIO_HIGH	0x0020
#define GPIO_LOW	0x0040

#define GPIO_PULLUP	0x0100
#define GPIO_PULLDOWN	0x0200
#define GPIO_PULLDISABLE 0x0400

int gpio_config(unsigned n, unsigned flags);
void gpio_set(unsigned n, unsigned on);
int gpio_get(unsigned n);

#endif

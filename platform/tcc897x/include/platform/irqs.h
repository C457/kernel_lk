/****************************************************************************
 * Copyright (C) 2015 Telechips Inc.
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

#ifndef __PLATFORM_TCC897X_IRQS__H__
#define __PLATFORM_TCC897X_IRQS__H__

#define INT_GIC_OFFSET	32
#define INT_TIMER1	(1+INT_GIC_OFFSET)
#define INT_USB_OTG	(48+INT_GIC_OFFSET)
#define INT_NUM		(127+INT_GIC_OFFSET)

#define NR_IRQS		(INT_NUM + 1)

#endif /* __PLATFORM_TCC897X_IRQS__H__ */


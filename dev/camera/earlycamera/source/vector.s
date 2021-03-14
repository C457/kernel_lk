
;/****************************************************************************
;  Copyright (C) 2014 Telechips, Inc.
;
;  This software is licensed under the terms of the GNU General Public
;  License version 2, as published by the Free Software Foundation, and
;  may be copied, distributed, and modified under those terms.
; 
;  This program is distributed in the hope that it will be useful,
;  but WITHOUT ANY WARRANTY; without even the implied warranty of
;  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;  GNU General Public License for more details.
;****************************************************************************/

        AREA    Vector, DATA, NOINIT
        ALIGN	4

;        EXPORT top_of_stacks
        EXPORT VectorTableBase

; Create dummy variable used to locate stacks in memory

;top_of_stacks    SPACE   4
VectorTableBase		SPACE 256
VectorTableEnd
MyData1		DCD	0
MyData2		DCD	0

        END


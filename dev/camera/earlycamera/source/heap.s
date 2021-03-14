
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

        AREA    Heap, DATA, NOINIT

        EXPORT bottom_of_heap

; Create dummy variable used to locate bottom of heap

bottom_of_heap    SPACE   1

        END


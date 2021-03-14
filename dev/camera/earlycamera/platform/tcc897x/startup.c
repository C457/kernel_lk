//*****************************************************************************
//
// startup.c - Boot code for Stellaris.
//
// Copyright (c) 2005-2007 Luminary Micro, Inc.  All rights reserved.
//
// Software License Agreement
//
// Luminary Micro, Inc. (LMI) is supplying this software for use solely and
// exclusively on LMI's microcontroller products.
//
// The software is owned by LMI and/or its suppliers, and is protected under
// applicable copyright laws.  All rights are reserved.  Any use in violation
// of the foregoing restrictions may subject the user to criminal sanctions
// under applicable laws, as well as to civil liability for the breach of the
// terms and conditions of this license.
//
// THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
// OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
// LMI SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
// CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
//
// This is part of revision 1049 of the Stellaris Driver Library.
//
//*****************************************************************************

#include <reg_physical.h>

//*****************************************************************************
//
// Forward declaration of the default fault handlers.
//
//*****************************************************************************
void ResetISR(void);
static void NmiSR(void);
static void FaultISR(void);
static void IntDefaultHandler(void);
//extern void xPortPendSVHandler(void);
//extern void xPortSysTickHandler(void);
//extern void vPortSVCHandler( void );
//extern void vIRQ( void );
//extern void vFIQ( void );

//*****************************************************************************
//
// The entry point for the application.
//
//*****************************************************************************
extern int main(void);

//*****************************************************************************
//
// Reserve space for the system stack.
//
//*****************************************************************************
#ifndef STACK_SIZE
#define STACK_SIZE                              1024
#endif
//static unsigned long pulStack[STACK_SIZE];

//*****************************************************************************
//
// The minimal vector table for a Cortex-M3.  Note that the proper constructs
// must be placed on this to ensure that it ends up at physical address
// 0x0000.0000.
//
//*****************************************************************************
__attribute__ ((section(".isr_vector")))
void (* const g_pfnVectors[])(void) =
{
	(void (*)(void))(HwCORTEXM4_CODE_MEM_SIZE + HwCORTEXM4_DATA_MEM_SIZE),		// The initial stack pointer	// ((unsigned long)pulStack + sizeof(pulStack)),
	ResetISR,                               // The reset handler
	NmiSR,                                  // The NMI handler
	FaultISR,                               // The hard fault handler
	IntDefaultHandler,                      // The MPU fault handler
	IntDefaultHandler,                      // The bus fault handler
	IntDefaultHandler,                      // The usage fault handler
	0,                                      // Reserved
	0,                                      // Reserved
	0,                                      // Reserved
	0,                                      // Reserved
//	vPortSVCHandler,                        // SVCall handler
	IntDefaultHandler,                      // IntDefaultHandler
	0,                                      // Reserved
//	xPortPendSVHandler,                     // The PendSV handler
//	xPortSysTickHandler,                    // The SysTick handler
	IntDefaultHandler,                      // IntDefaultHandler
	IntDefaultHandler,                      // IntDefaultHandler
	IntDefaultHandler,                      // IntDefaultHandler
	IntDefaultHandler,                      // IntDefaultHandler
	IntDefaultHandler,                      // IntDefaultHandler
	IntDefaultHandler,                      // IntDefaultHandler
	IntDefaultHandler,                      // IntDefaultHandler
	IntDefaultHandler,                      // IntDefaultHandler
	IntDefaultHandler,                      // IntDefaultHandler
	IntDefaultHandler,                      // IntDefaultHandler
//	vFIQ,                                   // FRQ Handler
//	vIRQ,                                   // IRQ Handler
};

//*****************************************************************************
//
// The following are constructs created by the linker, indicating where the
// the "data" and "bss" segments reside in memory.  The initializers for the
// for the "data" segment resides immediately following the "text" segment.
//
//*****************************************************************************
extern unsigned long _etext;
extern unsigned long _data;
extern unsigned long _edata;
extern unsigned long _bss;
extern unsigned long _ebss;

//*****************************************************************************
//
// This is the code that gets called when the processor first starts execution
// following a reset event.  Only the absolutely necessary set is performed,
// after which the application supplied main() routine is called.  Any fancy
// actions (such as making decisions based on the reset cause register, and
// resetting the bits in that register) are left solely in the hands of the
// application.
//
//*****************************************************************************
void
ResetISR(void)
{
    unsigned long *pulSrc, *pulDest;
    //
    // Copy the data segment initializers from flash to SRAM.
    //
    pulSrc = &_etext;
    pulSrc += 0x100/4; //dummy refer to scatter map
    for(pulDest = &_data; pulDest < &_edata; )
    {
        *pulDest++ = *pulSrc++;
    }

    //
    // Zero fill the bss segment.
    //
    for(pulDest = &_bss; pulDest < &_ebss; )
    {
        *pulDest++ = 0;
    }

    //
    // Call the application's entry point.
    //
    main();
	while(1)
	{
	}
}

//*****************************************************************************
//
// This is the code that gets called when the processor receives a NMI.  This
// simply enters an infinite loop, preserving the system state for examination
// by a debugger.
//
//*****************************************************************************
static void
NmiSR(void)
{
    //
    // Enter an infinite loop.
    //
    while(1)
    {
    }
}

//*****************************************************************************
//
// This is the code that gets called when the processor receives a fault
// interrupt.  This simply enters an infinite loop, preserving the system state
// for examination by a debugger.
//
//*****************************************************************************
static void
FaultISR(void)
{
    //
    // Enter an infinite loop.
    //
    while(1)
    {
    }
}

//*****************************************************************************
//
// This is the code that gets called when the processor receives an unexpected
// interrupt.  This simply enters an infinite loop, preserving the system state
// for examination by a debugger.
//
//*****************************************************************************
static void
IntDefaultHandler(void)
{
    //
    // Go into an infinite loop.
    //
    while(1)
    {
    }
}

/* for caddr_t (typedef char * caddr_t;) */
#include <sys/types.h>

extern int  __HEAP_START;

caddr_t _sbrk ( int incr )
{
//  static unsigned char *heap = NULL;
  static unsigned char *heap = 0;
  unsigned char *prev_heap;

//  if (heap == NULL) {
  if (heap == 0) {
    heap = (unsigned char *)&__HEAP_START;
  }
  prev_heap = heap;
  /* check removed to show basic approach */

  heap += incr;

  return (caddr_t) prev_heap;
}


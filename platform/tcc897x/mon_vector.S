
#include <asm.h>
#include <platform/sram_map.h>

#define MON_BASE_ADDR	RESET_ARM_TIEMR_ADDR
#define MON_STACK_BASE	(RESET_ARM_TIEMR_ADDR+RESET_ARM_TIEMR_SIZE-0x4)

#define CNTVOFF_H	(RESET_ARM_TIEMR_ADDR+0x20)
#define CNTVOFF_L	(RESET_ARM_TIEMR_ADDR+0x24)

.globl mon_vector
mon_vector:
	b	mon_reset
	b	mon_exception_loop		@ undefined
	b	mon_monitor
	b	mon_exception_loop		@ prefetch_abort
	b	mon_exception_loop		@ data_abort
	b	mon_exception_loop		@ reserved
	b	mon_exception_loop		@ irq
	b	mon_exception_loop		@ fiq

	.word	0x00000000			@ CNTVOFF_H
	.word	0x00000000			@ CNTVOFF_L

mon_exception_loop:
	stmfd	sp!, {lr}
	b	.
	ldmfd	sp!, {pc}^

mon_reset:
	stmfd	sp!, {r0-r2, lr}

	@ check cpu id
	mrc	 p15, 0, r2, c0, c0, 5		@ MPIDR
	ubfx	r1, r2, #8, #4			@ cluster
	ubfx	r2, r2, #0, #4			@ cpu
	cmp	r1, #1
	addeq	r2, r2, #2

	cmp	r2, #0
	bne	skip_check_nrcpus
	ldr	r1, =0x40000000
	ldr	r0, [r1, #0x494]
	ubfx	r0, r0, #17, #4
	cmp	r0, #0xd
	moveq	r0, #0x2
	beq	finish_check_nrcpus
	mov	r0, #0xf
finish_check_nrcpus:
	ldr	r1, =SRAM_BOOT_ADDR
	str	r0, [r1, #0x60]

skip_check_nrcpus:
	@ set monitor stack
	mrs	r0, cpsr
	mov	r1, r0
	bic	r1, r1, #0x1f
	orr	r1, r1, #0x16
	msr	cpsr_cf, r1
	ldr	r1, =MON_STACK_BASE
loop_check_cpus:
	cmp	r2, #0
	subne	r2, r2, #1			@ decrease counter
	subne	r1, r1, #0x20			@ each cpus mon stack size is 0x18
	bne	loop_check_cpus
	mov	sp, r1
	msr	cpsr_cf, r0

	@ MVBAR
	ldr	r0, =MON_BASE_ADDR
	MCR	p15, 0, r0, c12, c0, 1

	@ NSACR
	mrc	p15, 0, r0, c1, c1, 2
	orr	r0, r0, #0xC00			@ permission to access coprocessor 11 and 10 in the Non-secure state
	orr	r0, r0, #0x40000		@ permission to access SMP in the Non-secure state
	mcr	p15, 0, r0, c1, c1, 2

	@ VCR
//	mrc	p15, 0, r0, c1, c1, 3
//	orr	r0, r0, #0x100			@ VCR.AMO = 1 (asychronous abort)
//	mcr	p15, 0, r0, c1, c1, 3

	@ SCR
	mrc	p15, 0, r0, c1, c1, 0
//	orr	r0, r0, #0x28			@ SCR.AW=1, SCR.EA=1
	orr	r0, r0, #0x30			@ SCR.AW=1, SCR.FW=1
	mcr	p15, 0, r0, c1, c1, 0
	orr	r0, r0, #0x01			@ SCR.NS=1 (non-secure)
	mcr	p15, 0, r0, c1, c1, 0

	.arch_extension sec
	smc	0

	@ Clear NSACR reg.
	ldr	r0, =0x0;
	mcr	p15, 0, r0, c1, c1, 2

	@ Set ARM Timer rate to 24MHz
	ldr	r0, =24000000
	mcr	p15, 0, r0, c14, c0, 0

	ldmfd	sp!, {r0-r2, pc}


mon_monitor:
	stmfd	sp!, {r0-r2, lr}

	@ set CNTVOFF to 0
	ldr	r2, =CNTVOFF_L
	ldr	r0, [r2]
	ldr	r2, =CNTVOFF_H
	ldr	r1, [r2]
	mcrr	p15, 4, r0, r1, c14

	mrc	p15, 0, r0, c1, c1, 0
	bic	r0, r0, #0x01			@ SCR.NS=0 (secure)
	mcr	p15, 0, r0, c1, c1, 0

	ldmfd	sp!, {r0-r2, pc}^


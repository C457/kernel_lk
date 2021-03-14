
#include <asm.h>
#include <platform/sram_map.h>

#define TCC_PMU_BASE		0x74400000
#define PMU_WDTCTRL		0x008
#define PMU_SYSRST		0x010
#define PMU_CONFIG		0x014

#define MAX_CPU_NUM		4

FUNCTION(SRAM_Boot)
_sram_vectors:
	b		reset
	ldr		pc, _arm_undefined
	ldr		pc, _arm_syscall
	ldr		pc, _arm_prefetch_abort
	ldr		pc, _arm_data_abort
	ldr		pc, _arm_reserved
	ldr		pc, _arm_irq
	ldr		pc, _arm_fiq

_arm_undefined:
	.word		arm_undefined
_arm_syscall:
	.word		arm_syscall
_arm_prefetch_abort:
	.word		arm_prefetch_abort
_arm_data_abort:
	.word		arm_data_abort
_arm_reserved:
	.word		arm_reserved
_arm_irq:
	.word		arm_irq
_arm_fiq:
	.word		arm_fiq

	.word		0x00000000		/* dummy area */
	.word		0x00000000		@ cpu0 secondary start	(offset: 0x40)
	.word		0x00000000		@ cpu1 secondary start
	.word		0x00000000		@ cpu2 secondary start
	.word		0x00000000		@ cpu3 secondary start
	.word		0x00000000		@ cpu0 secondary valid	(offset: 0x50)
	.word		0x00000000		@ cpu1 secondary valid
	.word		0x00000000		@ cpu2 secondary valid
	.word		0x00000000		@ cpu3 secondary valid
	.word		0x00000000		@ nrcpus

reset:
	ldr	r0, =SRAM_BOOT_ADDR
	orr	r0, r0, pc
	mov	pc, r0

	@ disable local coherency (ACTLR.SMP=0)
	mrc	p15, 0, r0, c1, c0, 1
	bic	r0, r0, #(1 << 6)
	mcr	p15, 0, r0, c1, c0, 1

	@ do some cpu setup
	mrc	p15, 0, r0, c1, c0, 0
	bic	r0, r0, #(1<<15| 1<<13 | 1<<12)
	bic	r0, r0, #(1<<2 | 1<<0)
	orr	r0, r0, #(1<<1)
	orr	r0, r0, #(1<<11)		@ Z bit. Branch prediction enable
	mcr	p15, 0, r0, c1, c0, 0

	@ Mask IRQ/FIQ
	mrs	r0, cpsr
	orr	r0, r0, #0xC0
	msr	cpsr_cf, r0

	mrc	p15, 0, r0, c0, c0, 5		@ read MPIDR for get cluster/cpu id
	and	r1, r0, #(0xF<<8)		@ r1 = cluster
	and	r0, r0, #(0xF)			@ r0 = cpu
	add	r0, r0, r1, lsr #7		@ make cpu number likes 0,1,2,3,4,5

	cmp	r0, #0				@ check kernel boot cpu (Cortex-A7 CPU0)
	bne	skip_resume_process
	ldr	r1, =CPU_DATA_REPOSITORY_ADDR
	ldr	r2, [r1]
	ldr	r1, =0x18C818C8
	cmp	r1, r2				@ check resume status
	bne	skip_resume_process

	@ tzasc_ctrl
	ldr	r2, =0x60330040
	ldr	r1, [r2]
	orr	r1, r1, #3			@ IMEM, DMEM BYPASS
	str	r1, [r2]

	@ set CNTVOFF
	ldr	r2, =ARM_TIMER_BACKUP_ADDR
	ldr	r3, =RESET_ARM_TIEMR_ADDR
	ldr	r1, [r2]
	str	r1, [r3, #0x20]
	ldr	r1, [r2, #0x4]
	str	r1, [r3, #0x24]

skip_resume_process:

	ldr	r1, =SRAM_STACK_ADDR
	mov	r2, r0
set_stack_loop:
	cmp	r2, #0x0
	subne	r2, r2, #0x1
	subne	r1, r1, #0x100
	bne	set_stack_loop
	mov	sp, r1

@ check valid flag. if all valid flag is 0 then do hard boot
	ldr	r1, =SRAM_BOOT_ADDR
	add	r1, r1, #SEC_VALID
	ldr	r2, =0x0			@ cpu id (dummy)
	mov	r3, r2				@ cpu valid flags.
chk_cpu_valid_loop:
	ldr	r4, [r1]
	orr	r3, r4
	cmp	r3, #0x0
	bne	chk_sec_valid
	add	r1, r1, #0x4			@ increase secondary ready reg.
	add	r2, r2, #1			@ increase cpu id
	cmp	r2, #MAX_CPU_NUM
	bne	chk_cpu_valid_loop
	b	do_hard_boot

chk_sec_valid:
	ldr	r1, =SRAM_BOOT_ADDR
	ldr	r2, =SEC_VALID
	add	r2, r2, r0, lsl #2
	ldr	r3, [r1, r2]
	cmp	r3, #1
	beq	jump_startaddr
	wfe
	b	chk_sec_valid;

jump_startaddr:
	@ reset ARM Timer
	ldr	r3, =RESET_ARM_TIEMR_ADDR
	mov	r14, pc
	mov	pc, r3

	ldr	r3, =0x0
	str	r3, [r1, r2]
	ldr	r2, =SEC_START
	add	r2, r2, r0, lsl #2
	ldr	r3, [r1, r2]

	@ enable local coherency (ACTLR.SMP=1)
	mrc	p15, 0, r2, c1, c0, 1
	orr	r2, r2, #(1 << 6)
	mcr	p15, 0, r2, c1, c0, 1

	mov	pc, r3

do_hard_boot:
	ldr	r1, =TCC_PMU_BASE		@ read pmu Base Address
	cmp	r0, #0x0
	bne	wdt_reset_loop

	ldr	r2, =0x1
	str	r2, [r1, #0xE8]			@ PWRUP_CMBUS

	ldr	r2, [r1, #PMU_SYSRST]
	bic	r2, r2, #(0x7<<13)		@ powerdown secondary cpus
	str	r2, [r1, #PMU_SYSRST]

	ldr	r2, [r1, #PMU_CONFIG]
	bic	r2, r2, #(0x3<<28)		@ remap ca7 to Boot-Rom
	str	r2, [r1, #PMU_CONFIG]

	ldr	r2, =0x1			@ watchdog cnt = 1
	orr	r2, r2, #(0x1<<31)		@ watchdog en = 1
	str	r2, [r1, #PMU_WDTCTRL]

wdt_reset_loop:
	wfe
	b	wdt_reset_loop


#ifndef _TCC_GLOABAL_DATA_H_
#define _TCC_GLOABAL_DATA_H_

#include <common.h>
#include <asm/arch/clock.h>
#include <asm/arch/tcc_ckc.h>
#include <asm/arch/clock.h>
struct tcc_global_data{
	unsigned int stClockSource[MAX_CLK_SRC];
	const struct tcc_ckc_ops *clock_ops;
};

#endif /*_GLOABAL_DATA_H_*/


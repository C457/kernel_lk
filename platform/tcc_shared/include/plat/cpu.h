#ifndef __ARCH_ARM_TCC_PLAT_CPU_H
#define __ARCH_ARM_TCC_PLAT_CPU_H

/* IDs for TCC893x SoCs */
#define TCC893X_ARCH_ID		0x1009
#define TCC8930_CPU_ID		0x0000
#define TCC8933_CPU_ID		0x0001
#define TCC8935_CPU_ID		0x0002
#define TCC8933S_CPU_ID		0x0003
#define TCC8935S_CPU_ID		0x0004
#define TCC8937S_CPU_ID		0x0005

/* IDs for TCC896x SoCs */
#define TCC896X_ARCH_ID		0x1010
#define TCC8960_CPU_ID		0x0000
#define TCC8963_CPU_ID		0x0001

/* IDs for TCC897x SoCs */
#define TCC897X_ARCH_ID		0x1011
#define TCC8970_CPU_ID		0x0000
#define TCC8975_CPU_ID		0x0001
#define TCC8971_CPU_ID		0x0002

extern unsigned int __arch_id;

#if defined(TCC897X)
#define PKG_ID() (((*(volatile unsigned *)0x74400014) >> 16) & 0x7)
#elif defined(TCC896X)
#define PKG_ID() (((*(volatile unsigned *)0x14400014) >> 16) & 0x7)
#else
extern unsigned int __cpu_id;
#define PKG_ID() (__cpu_id)
#endif

#define DEFINE_CPU_ID(name, arch, cpu)			\
static inline int cpu_is_##name(void)			\
{							\
	if (__arch_id == arch && PKG_ID() == cpu)	\
		return 1;				\
	else						\
		return 0;				\
}

DEFINE_CPU_ID(tcc8930, TCC893X_ARCH_ID, TCC8930_CPU_ID);
DEFINE_CPU_ID(tcc8933, TCC893X_ARCH_ID, TCC8933_CPU_ID);
DEFINE_CPU_ID(tcc8935, TCC893X_ARCH_ID, TCC8935_CPU_ID);
DEFINE_CPU_ID(tcc8933s, TCC893X_ARCH_ID, TCC8933S_CPU_ID);
DEFINE_CPU_ID(tcc8935s, TCC893X_ARCH_ID, TCC8935S_CPU_ID);
DEFINE_CPU_ID(tcc8937s, TCC893X_ARCH_ID, TCC8937S_CPU_ID);

DEFINE_CPU_ID(tcc8960, TCC896X_ARCH_ID, TCC8960_CPU_ID);
DEFINE_CPU_ID(tcc8963, TCC896X_ARCH_ID, TCC8963_CPU_ID);

DEFINE_CPU_ID(tcc8970, TCC897X_ARCH_ID, TCC8970_CPU_ID);
DEFINE_CPU_ID(tcc8975, TCC897X_ARCH_ID, TCC8975_CPU_ID);
DEFINE_CPU_ID(tcc8971, TCC897X_ARCH_ID, TCC8971_CPU_ID);

#endif


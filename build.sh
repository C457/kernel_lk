export ARCH=arm
# This is user specific
export CROSS_COMPILE=~/android/kia/arm-eabi-4.7/bin/arm-eabi-
make clean
make tcc897x-lcn_defconfig
make
mv u-boot.rom u-boot_1920x720.rom

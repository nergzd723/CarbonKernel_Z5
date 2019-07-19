#!/bin/bash
export KERNELDIR=~/CarbonKernel_Z5
cd $KERNELDIR
export ANYKERNEL=$KERNELDIR/AnyKernel2
make clean
TOOLCHAINDIR=~/aarch64-linux-android-4.9-linaro/
export ARCH=arm64
export KBUILD_BUILD_USER="x0r3d"
export KBUILD_BUILD_HOST="L1nux1sX0R1N6"
export CROSS_COMPILE=$TOOLCHAINDIR/bin/aarch64-linux-android-
export USE_CCACHE=1
export CCACHE_DIR=../.ccache
export FINALZIP=kernel.zip

make clean && make mrproper
make kitakami_sumire_defconfig
make  -j$( nproc --all )

cp arch/arm64/boot/Image.gz-dtb $ANYKERNEL
cd $ANYKERNEL
zip -r kernel.zip *

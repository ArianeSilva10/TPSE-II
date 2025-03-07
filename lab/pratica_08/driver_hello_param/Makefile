# Makefile for driver
# if KERNELRELEASE is defined, we’ve been invoked from the
# kernel build system and can use its language.
ifneq (${KERNELRELEASE},)
	obj-m := hello.o
	# Otherwise we were called directly from the command line.
	# Invoke the kernel build system.
else
	KERNEL_SOURCE := /home/ariane/TPSE-II/lab/pratica_08/buildroot/output/build/linux-v6.4
	PWD := $(shell pwd)
	# Linux kernel 6.4 (one line)
	CC=/home/ariane/x-tools/arm-tpse2-linux-musleabi/bin/arm-tpse2-linux-musleabi-
	BUILD=boneblack
	CORES=4
	image=zImage
	PUBLIC_DRIVER_PWD=/home/ariane/TPSE-II/lab/rootfs/lib/modules/6.4.0/kernel/drivers/hello_param
default:
	# Trigger kernel build for this module
	${MAKE} -C ${KERNEL_SOURCE} M=${PWD} -j${CORES} ARCH=arm \
	LOCALVERSION=${BUILD} CROSS_COMPILE=${CC} ${address} \
	${image} modules
	# copy result to public folder
	cp *.ko ${PUBLIC_DRIVER_PWD}
clean:
	${MAKE} -C ${KERNEL_SOURCE} M=${PWD} clean
endif
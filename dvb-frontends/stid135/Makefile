# SPDX-License-Identifier: GPL-2.0

stid135-objs := stid135-fe.o \
		chip.o \
		stfe_utilities.o \
		oxford_anafe_func.o \
		stid135_init.o \
		stid135_drv.o

obj-$(CONFIG_DVB_STID135) += stid135.o

ccflags-y += -Idrivers/media/dvb-frontends -DHOST_PC

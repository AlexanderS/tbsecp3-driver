tbsecp3-objs := tbsecp3/tbsecp3-core.o \
                tbsecp3/tbsecp3-cards.o \
                tbsecp3/tbsecp3-i2c.o \
                tbsecp3/tbsecp3-dma.o \
                tbsecp3/tbsecp3-dvb.o \
                tbsecp3/tbsecp3-ca.o \
                tbsecp3/tbsecp3-asi.o
obj-m += tbsecp3.o

ccflags-y += -I$(M)/dvb-frontends/
ccflags-y += -I$(M)/tuners/

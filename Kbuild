tbsecp3-objs := tbsecp3/tbsecp3-core.o \
                tbsecp3/tbsecp3-cards.o \
                tbsecp3/tbsecp3-i2c.o \
                tbsecp3/tbsecp3-dma.o \
                tbsecp3/tbsecp3-dvb.o \
                tbsecp3/tbsecp3-ca.o \
                tbsecp3/tbsecp3-asi.o
obj-m += tbsecp3.o

tas2101-objs := dvb-frontends/tas2101.o
ccflags-y += -DCONFIG_DVB_TAS2101
obj-m += tas2101.o

mxl58x-objs := dvb-frontends/mxl58x.o
ccflags-y += -DCONFIG_DVB_MXL58X
obj-m += mxl58x.o

stv091x-objs := dvb-frontends/stv091x.o
ccflags-y += -DCONFIG_DVB_STV091X
obj-m += stv091x.o

tas2971-objs := dvb-frontends/tas2971.o
ccflags-y += -DCONFIG_DVB_TAS2971
obj-m += tas2971.o

av201x-objs := tuners/av201x.o
ccflags-y += -DCONFIG_MEDIA_TUNER_AV201X
obj-m += av201x.o

stv6120-objs := tuners/stv6120.o
ccflags-y += -DCONFIG_MEDIA_TUNER_STV6120
obj-m += stv6120.o

stid135-objs := dvb-frontends/stid135/stid135-fe.o \
                dvb-frontends/stid135/chip.o \
                dvb-frontends/stid135/stfe_utilities.o \
                dvb-frontends/stid135/oxford_anafe_func.o \
                dvb-frontends/stid135/stid135_init.o \
                dvb-frontends/stid135/stid135_drv.o
ccflags-y += -DHOST_PC -DCONFIG_DVB_STID135
obj-m += stid135.o

si2168-objs := dvb-frontends/si2168.o
si2183-objs := dvb-frontends/si2183.o
mn88436-objs := dvb-frontends/mn88436.o
mtv23x-objs := dvb-frontends/mtv23x.o
gx1503-objs := dvb-frontends/gx1503.o
obj-m += si2168.o si2183.o mn88436.o mtv23x.o gx1503.o

si2157-objs := tuners/si2157.o
mxl603-objs := tuners/mxl603.o
obj-m += si2157.o mxl603.o

ccflags-y += -I$(M)/dvb-frontends/
ccflags-y += -I$(M)/dvb-frontends/stid135/
ccflags-y += -I$(M)/tuners/
ccflags-y += -DTBS_STANDALONE

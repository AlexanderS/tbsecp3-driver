tbsecp3-objs := tbsecp3/tbsecp3-core.o \
                tbsecp3/tbsecp3-cards.o \
                tbsecp3/tbsecp3-i2c.o \
                tbsecp3/tbsecp3-dma.o \
                tbsecp3/tbsecp3-dvb.o \
                tbsecp3/tbsecp3-ca.o \
                tbsecp3/tbsecp3-asi.o
obj-m += tbsecp3.o
ccflags-y += -I$(M)/dvb-frontends/
ccflags-y += -I$(M)/dvb-frontends/stid135/
ccflags-y += -I$(M)/tuners/
ccflags-y += -I$(srctree)
ccflags-y += -DTBS_STANDALONE


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

stid135-objs := dvb-frontends/stid135/stid135-fe.o \
                dvb-frontends/stid135/chip.o \
                dvb-frontends/stid135/stfe_utilities.o \
                dvb-frontends/stid135/oxford_anafe_func.o \
                dvb-frontends/stid135/stid135_init.o \
                dvb-frontends/stid135/stid135_drv.o
ccflags-y += -DHOST_PC -DCONFIG_DVB_STID135
obj-m += stid135.o

gx1133-objs := dvb-frontends/gx1133.o
ccflags-y += -DCONFIG_DVB_GX1133
obj-m += gx1133.o

mn88436-objs := dvb-frontends/mn88436.o
ccflags-y += -DCONFIG_DVB_MN88436
obj-m += mn88436.o

si2183-objs := dvb-frontends/si2183.o
mtv23x-objs := dvb-frontends/mtv23x.o
gx1503-objs := dvb-frontends/gx1503.o
obj-m += si2183.o mtv23x.o gx1133.o gx1503.o


av201x-objs := tuners/av201x.o
ccflags-y += -DCONFIG_MEDIA_TUNER_AV201X
obj-m += av201x.o

mxl603-objs := tuners/mxl603.o
obj-m += mxl603.o

rda5816-objs := tuners/rda5816.o
ccflags-y += -DCONFIG_MEDIA_TUNER_RDA5816
obj-m += rda5816.o

stv6120-objs := tuners/stv6120.o
ccflags-y += -DCONFIG_MEDIA_TUNER_STV6120
obj-m += stv6120.o

saa716x_core-objs	:= saa716x/saa716x_pci.o	\
			   saa716x/saa716x_i2c.o	\
			   saa716x/saa716x_cgu.o	\
			   saa716x/saa716x_msi.o	\
			   saa716x/saa716x_dma.o	\
			   saa716x/saa716x_vip.o	\
			   saa716x/saa716x_aip.o	\
			   saa716x/saa716x_phi.o	\
			   saa716x/saa716x_boot.o	\
			   saa716x/saa716x_fgpi.o	\
			   saa716x/saa716x_adap.o	\
			   saa716x/saa716x_gpio.o	\
			   saa716x/saa716x_greg.o	\
			   saa716x/saa716x_rom.o	\
			   saa716x/saa716x_spi.o

saa716x_ff-objs		:= saa716x/saa716x_ff_main.o	\
                           saa716x/saa716x_ff_cmd.o	\
			   saa716x/saa716x_ff_ir.o

saa716x_tbs-dvb-objs += saa716x/saa716x_budget.o saa716x/tbsci-i2c.o saa716x/tbs-ci.o
saa716x_hybrid-objs += saa716x/saa716x_hybrid.o

obj-m += saa716x_core.o saa716x_tbs-dvb.o saa716x_hybrid.o saa716x_ff.o

isl6422-objs := dvb-frontends/isl6422.o
ccflags-y += -DCONFIG_DVB_ISL6422
obj-m += isl6422.o

isl6423-objs := dvb-frontends/isl6423.o
obj-m += isl6423.o

mb86a16-objs := dvb-frontends/mb86a16.o
obj-m += mb86a16.o

stb6100-objs := dvb-frontends/stb6100.o
obj-m += stb6100.o

stv090x-objs := dvb-frontends/stv090x.o
obj-m += stv090x.o

stv6110x-objs := dvb-frontends/stv6110x.o
obj-m += stv6110x.o

tda1004x-objs := dvb-frontends/tda1004x.o
obj-m += tda1004x.o

zl10353-objs := dvb-frontends/zl10353.o
obj-m += zl10353.o

tda18212-objs := tuners/tda18212.o
obj-m += tda18212.o

tda827x-objs := tuners/tda827x.o
obj-m += tda827x.o

tda8290-objs := tuners/tda8290.o
obj-m += tda8290.o

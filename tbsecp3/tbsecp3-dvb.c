/*
    TBS ECP3 FPGA based cards PCIe driver

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "tbsecp3.h"

#include "tas2101.h"
#include "av201x.h"

#include "si2168.h"
#include "si2157.h"

#include "mxl58x.h"

#include "si2183.h"

#include "stv091x.h"
#include "stv6120.h"

#include "mn88436.h"
#include "mxl603.h"

#include "mtv23x.h"
#include "gx1503.h"
#include "tas2971.h"

#include "stid135.h"
#include "rda5816.h"

DVB_DEFINE_MOD_OPT_ADAPTER_NR(adapter_nr);

struct sec_priv {
	struct tbsecp3_adapter *adap;
	int (*set_voltage)(struct dvb_frontend *fe,
			   enum fe_sec_voltage voltage);
};
static void ecp3_spi_read(struct i2c_adapter *i2c,u8 reg, u32 *buf)
{	
	struct tbsecp3_i2c *i2c_adap = i2c_get_adapdata(i2c);
	struct tbsecp3_dev *dev = i2c_adap->dev;
	*buf = tbs_read(TBSECP3_GPIO_BASE,reg );

	//printk(" tbsecp3-dvb : ecp3_spi_read **********%x = %x*******\n",reg,*buf);

	return ;
}
static void ecp3_spi_write(struct i2c_adapter *i2c,u8 reg, u32 buf)
{
	struct tbsecp3_i2c *i2c_adap = i2c_get_adapdata(i2c);
	struct tbsecp3_dev *dev = i2c_adap->dev;
	//printk(" tbsecp3-dvb : ecp3_spi_write **********%x = %x*******\n",reg,buf);
	tbs_write(TBSECP3_GPIO_BASE, reg, buf);

	return ;
}

static void mcu_24cxx_read(struct i2c_adapter *i2c,u32 bassaddr, u8 reg, u32 *buf)
{	
	struct tbsecp3_i2c *i2c_adap = i2c_get_adapdata(i2c);
	struct tbsecp3_dev *dev = i2c_adap->dev;
	*buf = tbs_read(bassaddr,reg );

	//printk(" tbsecp3-dvb : mcu_24cxx_read *****bassaddr: %x,  %x = %x*******\n",bassaddr,reg,*buf);

	return ;
}
static void mcu_24cxx_write(struct i2c_adapter *i2c,u32 bassaddr,u8 reg, u32 buf)
{
	struct tbsecp3_i2c *i2c_adap = i2c_get_adapdata(i2c);
	struct tbsecp3_dev *dev = i2c_adap->dev;
	//printk(" tbsecp3-dvb : mcu_24cxx_write ****bassaddr: %x,***%x = %x*******\n",bassaddr,reg,buf);
	tbs_write(bassaddr, reg, buf);

	return ;
}

static int tbs6304_read_mac(struct tbsecp3_adapter *adap)
{
	struct tbsecp3_dev *dev = adap->dev;
	int ret = 1;
	int i =0;
	u32 postaddr;
	unsigned char tmpbuf[4]={0};
	unsigned char rdbuffer[8]={0};
	
	postaddr =  0x80 *(adap->nr) +0x08;

	tmpbuf[0] = 0x31; //24cxx read;
	tmpbuf[1] = 6;  //how many byte;
	tmpbuf[2] =  (postaddr>>8);
	tmpbuf[3] = (postaddr &255); //24cxx sub_address,0x08 for mac

	
	tbs_write( BASE_ADDRESS_24CXX, CMD_24CXX, *(u32 *)&tmpbuf[0] );
	//wait... until the data are received correctly;
	for(i=0;i<100;i++)
	{
		msleep(1);
		*(u32 *)tmpbuf = tbs_read( BASE_ADDRESS_24CXX, STATUS_MAC16_24CXX );
		if((tmpbuf[0]&1) == 0)
			break;
	}
	if(i==100)
	{
		ret = 0;
		//printk(" the receiver always busy !\n");
		//check mcu status
		*(u32 *)tmpbuf = tbs_read( BASE_ADDRESS_24CXX,  STATUS_MAC16_24CXX );
		if((tmpbuf[0]&0x4) == 1) // bit2==1 mcu busy
		{
			//printk("MCU status is busy!!!\n" );
			// release cs;
			tbs_write( BASE_ADDRESS_24CXX,  CS_RELEASE, *(u32 *)&tmpbuf[0] );
			ret = 0;
		}
		
	}
	// release cs;
	tbs_write(  BASE_ADDRESS_24CXX, CS_RELEASE, *(u32 *)&tmpbuf[0] );
	//check the write finished;
	for(i=0;i<100;i++)
	{
		msleep(1);
		*(u32 *)tmpbuf = tbs_read(  BASE_ADDRESS_24CXX, STATUS_MAC16_24CXX );
		if((tmpbuf[0]&1) == 1)
			break;
	}
	if(i==100)
	{
		ret = 0;
		//printk(" wait wt_24cxx_done timeout! \n");
	}
	//read back to host;
	*(u32 *)rdbuffer = tbs_read(  BASE_ADDRESS_24CXX, DATA0_24CXX );
	*(u32 *)&rdbuffer[4] = tbs_read(  BASE_ADDRESS_24CXX, DATA1_24CXX );
	if(ret!=0)
	{
		memcpy(adap->dvb_adapter.proposed_mac, rdbuffer,6);
		printk("adapter %d ,mac address: %x,%x,%x,%x,%x,%x \n",adap->dvb_adapter.num,rdbuffer[0],rdbuffer[1],rdbuffer[2],rdbuffer[3],rdbuffer[4],rdbuffer[5]);
	}

	return ret;
};
static void tbs_write_ext(struct tbsecp3_adapter *adap, u32 baseaddr, u8 address, u32 data)
{
    struct tbsecp3_dev *dev = adap->dev;
    int i = 0;
    u32 uAddr = baseaddr + address;
    u32 tmp = ((uAddr & 0xff00) >> 8) | ((uAddr & 0x00ff) << 8);
    tbs_write(0x1000,4, data);
    tbs_write(0x1000,0, (((tmp << 16) & 0xffff0000) + 0x00000000));

    while (0xff == tbs_read(0x1000,0)) {
	//msleep(1);
        if (10000 == i) {
           //printk("rdReg32_Extern:rdReg32(0x1000) time out\n");
           return;
        }
        i++;
    }
    return;

}

static u32 tbs_read_ext(struct tbsecp3_adapter *adap, u32 baseaddr, u8 address)
{
    struct tbsecp3_dev *dev = adap->dev;
    int i = 0;
    u32 uAddr = baseaddr + address;
    u32 tmp = ((uAddr & 0xff00) >> 8) | ((uAddr & 0x00ff) << 8);
    tbs_write(0x1000,0, (((tmp << 16) & 0xffff0000) + 0x00000080));
    while (0xff == tbs_read(0x1000,0)) {
	//msleep(1);
        if (10000 == i) {
           //printk("rdReg32_Extern:rdReg32(0x1000) time out\n");
           return 0;
        }
        i++;
    }
    return tbs_read(0x1000,4);
}

static int tbs6308_read_mac_ext(struct tbsecp3_adapter *adap)
{
	struct tbsecp3_dev *dev = adap->dev;
	int ret = 1;
	int i =0;
	u32 postaddr;
	unsigned char tmpbuf[4]={0};
	unsigned char rdbuffer[8]={0};
	
	postaddr =  0x80 *((adap->nr)%4) +0x08;

	tmpbuf[0] = 0x31; //24cxx read;
	tmpbuf[1] = 6;  //how many byte;
	tmpbuf[2] =  (postaddr>>8);
	tmpbuf[3] = (postaddr &255); //24cxx sub_address,0x08 for mac

	
	tbs_write_ext( adap, BASE_ADDRESS_24CXX, CMD_24CXX, *(u32 *)&tmpbuf[0] );
	//wait... until the data are received correctly;
	for(i=0;i<200;i++)
	{
		msleep(1);
		*(u32 *)tmpbuf = tbs_read_ext( adap, BASE_ADDRESS_24CXX, STATUS_MAC16_24CXX );
		if((tmpbuf[0]&1) == 0)
			break;
	}
	if(i==200)
	{
		//printk(" the receiver always busy !\n");
		ret = 0;
		//check mcu status
		*(u32 *)tmpbuf = tbs_read_ext(adap, BASE_ADDRESS_24CXX,  STATUS_MAC16_24CXX );
		if((tmpbuf[0]&0x4) == 1) // bit2==1 mcu busy
		{
			//printk("MCU status is busy!!!\n" );
			// release cs;
			tbs_write_ext( adap,BASE_ADDRESS_24CXX,  CS_RELEASE, *(u32 *)&tmpbuf[0] );
			ret = 0;
		}
		
	}
	// release cs;
	tbs_write_ext( adap, BASE_ADDRESS_24CXX, CS_RELEASE, *(u32 *)&tmpbuf[0] );
	//check the write finished;
	for(i=0;i<200;i++)
	{
		msleep(1);
		*(u32 *)tmpbuf = tbs_read_ext( adap, BASE_ADDRESS_24CXX, STATUS_MAC16_24CXX );
		if((tmpbuf[0]&1) == 1)
			break;
	}
	if(i==200)
	{
		//printk(" wait wt_24cxx_done timeout! \n");
		ret=0;
	}
	//read back to host;
	*(u32 *)rdbuffer = tbs_read_ext( adap, BASE_ADDRESS_24CXX, DATA0_24CXX );
	*(u32 *)&rdbuffer[4] = tbs_read_ext( adap, BASE_ADDRESS_24CXX, DATA1_24CXX );
	
	if(ret!=0)
	{
		memcpy(adap->dvb_adapter.proposed_mac, rdbuffer,6);
		printk("adapter %d ,mac address: %x,%x,%x,%x,%x,%x \n",adap->dvb_adapter.num,rdbuffer[0],rdbuffer[1],rdbuffer[2],rdbuffer[3],rdbuffer[4],rdbuffer[5]);
	}
	return ret;
};

static void tbs6301_read_mac(struct tbsecp3_adapter *adap)
{
	struct tbsecp3_dev *dev = adap->dev;

	int i =0;
	unsigned char tmpbuf[4]={0};
	unsigned char rdbuffer[8]={0};

	tmpbuf[0] = 0x31; //24cxx read;
	tmpbuf[1] = 6;  //how many byte;
	tmpbuf[2] = 0;
	tmpbuf[3] = 0x08; //24cxx sub_address,0x08 for mac

	tbs_write( BASE_ADDRESS_24CXX, CMD_24CXX, *(u32 *)&tmpbuf[0] );
	//wait... until the data are received correctly;
	for(i=0;i<100;i++)
	{
		msleep(1);
		*(u32 *)tmpbuf = tbs_read( BASE_ADDRESS_24CXX, STATUS_MAC16_24CXX );
		if((tmpbuf[0]&1) == 0)
			break;
	}
	if(i==100)
	{
		printk(" the receiver always busy !\n");
		//check mcu status
		*(u32 *)tmpbuf = tbs_read( BASE_ADDRESS_24CXX,  STATUS_MAC16_24CXX );
		if((tmpbuf[0]&0x4) == 1) // bit2==1 mcu busy
		{
			printk("MCU status is busy!!!\n" );
			// release cs;
			tbs_write( BASE_ADDRESS_24CXX,  CS_RELEASE, *(u32 *)&tmpbuf[0] );
			return;
		}
		
	}
	// release cs;
	tbs_write(  BASE_ADDRESS_24CXX, CS_RELEASE, *(u32 *)&tmpbuf[0] );
	//check the write finished;
	for(i=0;i<100;i++)
	{
		msleep(1);
		*(u32 *)tmpbuf = tbs_read(  BASE_ADDRESS_24CXX, STATUS_MAC16_24CXX );
		if((tmpbuf[0]&1) == 1)
			break;
	}
	if(i==100)
	{
		printk(" wait wt_24cxx_done timeout! \n");
	}
	//read back to host;
	*(u32 *)rdbuffer = tbs_read(  BASE_ADDRESS_24CXX, DATA0_24CXX );
	*(u32 *)&rdbuffer[4] = tbs_read(  BASE_ADDRESS_24CXX, DATA1_24CXX );
	memcpy(adap->dvb_adapter.proposed_mac, rdbuffer,6);
	printk(" tbs6301 mac address: %x,%x,%x,%x,%x,%x \n",rdbuffer[0],rdbuffer[1],rdbuffer[2],rdbuffer[3],rdbuffer[4],rdbuffer[5]);

	return ;
};


static int tbsecp3_set_voltage(struct dvb_frontend* fe,
		enum fe_sec_voltage voltage)
{
	struct sec_priv *priv = fe->sec_priv;
	struct tbsecp3_gpio_config *cfg = &priv->adap->cfg->gpio;
	struct tbsecp3_dev *dev = priv->adap->dev;

	dev_dbg(&dev->pci_dev->dev, "%s() %s\n", __func__,
		voltage == SEC_VOLTAGE_13 ? "SEC_VOLTAGE_13" :
		voltage == SEC_VOLTAGE_18 ? "SEC_VOLTAGE_18" :
		"SEC_VOLTAGE_OFF");

	switch (voltage) {
		case SEC_VOLTAGE_13:
			tbsecp3_gpio_set_pin(dev, &cfg->lnb_power, 1);
			tbsecp3_gpio_set_pin(dev, &cfg->lnb_voltage, 0);
			break;
		case SEC_VOLTAGE_18:
			tbsecp3_gpio_set_pin(dev, &cfg->lnb_power, 1);
			tbsecp3_gpio_set_pin(dev, &cfg->lnb_voltage, 1);
			break;
		default: /* OFF */
			tbsecp3_gpio_set_pin(dev, &cfg->lnb_power, 0);
			break;
	}

	if (priv->set_voltage)
		return priv->set_voltage(fe, voltage);
	else
		return 0;
}

static void tbsecp3_release_sec(struct dvb_frontend* fe)
{
	struct sec_priv *priv;

	if (fe == NULL)
		return;

	priv = fe->sec_priv;
	if (priv == NULL)
		return;

	fe->ops.set_voltage = priv->set_voltage;
	fe->sec_priv = NULL;
	kfree(priv);
}

static struct dvb_frontend *tbsecp3_attach_sec(struct tbsecp3_adapter *adap, struct dvb_frontend *fe)
{
	struct sec_priv *priv;

	priv = kzalloc(sizeof(struct sec_priv), GFP_KERNEL);
	if (!priv)
		return NULL;

	priv->set_voltage = fe->ops.set_voltage;
	priv->adap = adap;

//	fe->ops.release_sec = tbsecp3_release_sec;
	fe->ops.set_voltage = tbsecp3_set_voltage;
	fe->sec_priv = priv;

	return fe;
}

static int set_mac_address(struct tbsecp3_adapter *adap)
{
	struct tbsecp3_dev *dev = adap->dev;
	u8 eeprom_bus_nr = dev->info->eeprom_i2c;
	struct i2c_adapter *i2c = &dev->i2c_bus[eeprom_bus_nr].i2c_adap;
	u8 eep_addr = 0xa0;
	int ret;

	struct i2c_msg msg[] = {
		{ .addr = 0x50, .flags = 0,
		  .buf = &eep_addr, .len = 1 },
		{ .addr = 0x50, .flags = I2C_M_RD,
		  .buf = adap->dvb_adapter.proposed_mac, .len = 6 }
	};

	if (dev->info->eeprom_addr)
		eep_addr = dev->info->eeprom_addr;
	
	eep_addr += 0x10 * adap->nr;
	
	ret = i2c_transfer(i2c, msg, 2);
	ret = i2c_transfer(i2c, msg, 2);
	if (ret != 2) {
		dev_warn(&dev->pci_dev->dev,
			"error reading MAC address for adapter %d\n",
			adap->nr);
	} else {
		dev_info(&dev->pci_dev->dev,
			"MAC address %pM\n", adap->dvb_adapter.proposed_mac);
	}
	return 0;
};

static int start_feed(struct dvb_demux_feed *dvbdmxfeed)
{
	struct dvb_demux *dvbdmx = dvbdmxfeed->demux;
	struct tbsecp3_adapter *adapter = dvbdmx->priv;

	if (!adapter->feeds)
		tbsecp3_dma_enable(adapter);

	return ++adapter->feeds;
}

static int stop_feed(struct dvb_demux_feed *dvbdmxfeed)
{
	struct dvb_demux *dvbdmx = dvbdmxfeed->demux;
	struct tbsecp3_adapter *adapter = dvbdmx->priv;

	if (--adapter->feeds)
		return adapter->feeds;

	tbsecp3_dma_disable(adapter);
	return 0;
}

static void reset_demod(struct tbsecp3_adapter *adapter)
{
	struct tbsecp3_dev *dev = adapter->dev;
	struct tbsecp3_gpio_pin *reset = &adapter->cfg->gpio.demod_reset;

	tbsecp3_gpio_set_pin(dev, reset, 1);
	usleep_range(10000, 20000);

	tbsecp3_gpio_set_pin(dev, reset, 0);
	usleep_range(50000, 100000);
}


static struct tas2101_config tbs6902_demod_cfg[] = {
	{
		.i2c_address   = 0x60,
		.id            = ID_TAS2101,
		.init          = {0xb0, 0x32, 0x81, 0x57, 0x64, 0x9a, 0x33},
		.init2         = 0,
		.write_properties = ecp3_spi_write,  
		.read_properties = ecp3_spi_read,	

	},
	{
		.i2c_address   = 0x68,
		.id            = ID_TAS2101,
		.init          = {0xb0, 0x32, 0x81, 0x57, 0x64, 0x9a, 0x33}, // 0xb1
		.init2         = 0,
		.write_properties = ecp3_spi_write,  
		.read_properties = ecp3_spi_read,
	}
};

static struct av201x_config tbs6902_av201x_cfg = {
		.i2c_address = 0x62,
		.id 		 = ID_AV2012,
		.xtal_freq	 = 27000,		/* kHz */
};

static struct tas2101_config tbs6308_demod_cfg = {
       		 .i2c_address   = 0x60,
		.id            = ID_TAS2101,
		.init          = {0xb0, 0x32, 0x81, 0x57, 0x64, 0x9a, 0x33}, // 0xb1
		.init2         = 0,
		.write_properties = ecp3_spi_write,  
		.read_properties = ecp3_spi_read,

		.mcuWrite_properties = mcu_24cxx_write,  
		.mcuRead_properties = mcu_24cxx_read,    

};

static struct tas2101_config tbs6304_demod_cfg[] = {
    {
        .i2c_address   = 0x60,
		.id            = ID_TAS2101,
		.init          = {0xb0, 0x32, 0x81, 0x57, 0x64, 0x9a, 0x33}, // 0xb1
		.init2         = 0,
		.write_properties = ecp3_spi_write,  
		.read_properties = ecp3_spi_read,

		.mcuWrite_properties = mcu_24cxx_write,  
		.mcuRead_properties = mcu_24cxx_read,    
    },
    {
        .i2c_address   = 0x68,
		.id            = ID_TAS2101,
		.init          = {0xb0, 0x32, 0x81, 0x57, 0x64, 0x9a, 0x33}, // 0xb1
		.init2         = 0,
		.write_properties = ecp3_spi_write,  
		.read_properties = ecp3_spi_read,

		.mcuWrite_properties = mcu_24cxx_write,  
		.mcuRead_properties = mcu_24cxx_read,
    },    
    {
        .i2c_address   = 0x60,
		.id            = ID_TAS2101,
		.init          = {0xb0, 0x32, 0x81, 0x57, 0x64, 0x9a, 0x33}, // 0xb1
		.init2         = 0,
		.write_properties = ecp3_spi_write,  
		.read_properties = ecp3_spi_read,

		.mcuWrite_properties = mcu_24cxx_write,  
		.mcuRead_properties = mcu_24cxx_read,
    },    
    {
        .i2c_address   = 0x68,
		.id            = ID_TAS2101,
		.init          = {0xb0, 0x32, 0x81, 0x57, 0x64, 0x9a, 0x33}, // 0xb1
		.init2         = 0,
		.write_properties = ecp3_spi_write,  
		.read_properties = ecp3_spi_read,

		.mcuWrite_properties = mcu_24cxx_write,  
		.mcuRead_properties = mcu_24cxx_read,
    } 
};
static struct tas2101_config tbs6301_demod_cfg = {
		.i2c_address   = 0x60,
		.id            = ID_TAS2101,
		.init          = {0xb0, 0x32, 0x81, 0x57, 0x64, 0x9a, 0x33}, // 0xb1
		.init2         = 0,
		.write_properties = ecp3_spi_write,  
		.read_properties = ecp3_spi_read,

		.mcuWrite_properties = mcu_24cxx_write,  
		.mcuRead_properties = mcu_24cxx_read,

};
static struct tas2101_config tbs6904_demod_cfg[] = {
	{
		.i2c_address   = 0x60,
		.id            = ID_TAS2101,
		.init          = {0xb0, 0x32, 0x81, 0x57, 0x64, 0x9a, 0x33}, // 0xb1
		.init2         = 0,
		.write_properties = ecp3_spi_write,  
		.read_properties = ecp3_spi_read,
	},
	{
		.i2c_address   = 0x68,
		.id            = ID_TAS2101,
		.init          = {0xb0, 0x32, 0x81, 0x57, 0x64, 0x9a, 0x33},
		.init2         = 0,
		.write_properties = ecp3_spi_write,  
		.read_properties = ecp3_spi_read,
	},
	{
		.i2c_address   = 0x60,
		.id            = ID_TAS2101,
		.init          = {0xb0, 0x32, 0x81, 0x57, 0x64, 0x9a, 0x33},
		.init2         = 0,
		.write_properties = ecp3_spi_write,  
		.read_properties = ecp3_spi_read,
	},
	{
		.i2c_address   = 0x68,
		.id            = ID_TAS2101,
		.init          = {0xb0, 0x32, 0x81, 0x57, 0x64, 0x9a, 0x33},
		.init2         = 0,
		.write_properties = ecp3_spi_write,  
		.read_properties = ecp3_spi_read,
	}
};

static struct av201x_config tbs6904_av201x_cfg = {
	.i2c_address = 0x63,
	.id          = ID_AV2012,
	.xtal_freq   = 27000,		/* kHz */
};


static struct tas2101_config tbs6910_demod_cfg[] = {
	{
		.i2c_address   = 0x68,
		.id            = ID_TAS2101,
		.init          = {0x21, 0x43, 0x65, 0xb0, 0xa8, 0x97, 0xb1},
		.init2         = 0,
		.write_properties = ecp3_spi_write,  
		.read_properties = ecp3_spi_read,
	},
	{
		.i2c_address   = 0x60,
		.id            = ID_TAS2101,
		.init          = {0xb0, 0xa8, 0x21, 0x43, 0x65, 0x97, 0xb1},
		.init2         = 0,
		.write_properties = ecp3_spi_write,  
		.read_properties = ecp3_spi_read,
	},
};

static struct av201x_config tbs6910_av201x_cfg = {
	.i2c_address = 0x62,
	.id          = ID_AV2018,
	.xtal_freq   = 27000,		/* kHz */
};


static int max_set_voltage(struct i2c_adapter *i2c,
		enum fe_sec_voltage voltage, u8 rf_in)
{
	struct tbsecp3_i2c *i2c_adap = i2c_get_adapdata(i2c);
	struct tbsecp3_dev *dev = i2c_adap->dev;

	u32 val, reg;

	//printk("set voltage on %u = %d\n", rf_in, voltage);
	
	if (rf_in > 3)
		return -EINVAL;

	reg = rf_in * 4;
	val = tbs_read(TBSECP3_GPIO_BASE, reg) & ~4;

	switch (voltage) {
	case SEC_VOLTAGE_13:
		val &= ~2;
		break;
	case SEC_VOLTAGE_18:
		val |= 2;
		break;
	case SEC_VOLTAGE_OFF:
	default:
		//val |= 4;
		break;
	}

	tbs_write(TBSECP3_GPIO_BASE, reg, val);
	return 0;
}

static int max_send_master_cmd(struct dvb_frontend *fe, struct dvb_diseqc_master_cmd *cmd)
{
	//printk("send master cmd\n");
	return 0;
}
static int max_send_burst(struct dvb_frontend *fe, enum fe_sec_mini_cmd burst)
{
	//printk("send burst: %d\n", burst);
	return 0;
}
static void RF_switch(struct i2c_adapter *i2c,u8 rf_in,u8 flag)//flag : 0: dvbs/s2 signal 1:Terrestrial and cable signal 
{
	struct tbsecp3_i2c *i2c_adap = i2c_get_adapdata(i2c);
	struct tbsecp3_dev *dev = i2c_adap->dev;
	u32 val ,reg;

	if (flag)
		tbsecp3_gpio_set_pin(dev, &dev->adapter[rf_in].cfg->gpio.lnb_power, 0);

	reg = 0x8+rf_in*4;
	
	val = tbs_read(TBSECP3_GPIO_BASE, reg);
	if(flag)
		val |= 2;
	else
		val &= ~2;
		
	tbs_write(TBSECP3_GPIO_BASE, reg, val);

}

static struct mxl58x_cfg tbs6909_mxl58x_cfg = {
	.adr		= 0x60,
	.type		= 0x01,
	.clk		= 24000000,
	.cap		= 12,
	.fw_read	= NULL,

	.set_voltage	= max_set_voltage,
	.write_properties = ecp3_spi_write, 
	.read_properties = ecp3_spi_read,
};

static struct stv091x_cfg tbs6903_stv0910_cfg = {
	.adr      = 0x68,
	.parallel = 1,
	.rptlvl   = 3,
	.clk      = 30000000,
	.dual_tuner = 1,
	.write_properties = ecp3_spi_write, 
	.read_properties = ecp3_spi_read,
};

struct stv6120_cfg tbs6903_stv6120_cfg = {
	.adr      = 0x60,
	.Rdiv     = 2,
	.xtal     = 30000,
};


static struct av201x_config tbs6522_av201x_cfg[] = {
	{
		.i2c_address = 0x63,
		.id          = ID_AV2018,
		.xtal_freq   = 27000,
	},
	{
		.i2c_address = 0x62,
		.id          = ID_AV2018,
		.xtal_freq   = 27000,
	},
};

static void Set_TSsampling(struct i2c_adapter *i2c,int tuner,int time)
{
	struct tbsecp3_i2c *i2c_adap = i2c_get_adapdata(i2c);
	struct tbsecp3_dev *dev = i2c_adap->dev;

	u32 Many_cnt;
	u32 iobuffer;
	char tmp[4];

	Many_cnt = 0x3B9ACA00/8*time;
	tmp[0] = (Many_cnt>>24)&0xff;
	tmp[1] =  (Many_cnt>>16)&0xff;
	tmp[2] =  (Many_cnt>>8)&0xff;
	tmp[3] =  Many_cnt&0xff;

	iobuffer = tmp[0]|(tmp[1]<<8)|(tmp[2]<<16)|(tmp[3]<<24);
	
	tbs_write(TBSECP3_GPIO_BASE, (0xc+tuner)*4,iobuffer);
	
}

static u32  Set_TSparam(struct i2c_adapter *i2c,int tuner,int time,bool flag)
{
	struct tbsecp3_i2c *i2c_adap = i2c_get_adapdata(i2c);
	struct tbsecp3_dev *dev = i2c_adap->dev;

	u32 iobuffer;
	u32 Frm_cnt,Bit_rate,Many_cnt;
	int clk_preset;
	u8  tmp[4];

	Many_cnt = 0x3B9ACA00/8*time;

	iobuffer =  tbs_read(TBSECP3_GPIO_BASE, (0x10+tuner)*4);
	tmp[0] = (iobuffer>>24)&0xff;
	tmp[1] =  (iobuffer>>16)&0xff;
	tmp[2] =  (iobuffer>>8)&0xff;
	tmp[3] =  iobuffer&0xff;

	Bit_rate= tmp[0]|(tmp[1]<<8)|(tmp[2]<<16)|(tmp[3]<<24);

	if(!flag)
		return Bit_rate;

	Frm_cnt = Many_cnt/(Bit_rate-50) -100;

	tmp[0] =  (Frm_cnt>>24)&0xff;
	tmp[1] =  (Frm_cnt>>16)&0xff;
	tmp[2] =  (Frm_cnt>>8)&0xff;
	tmp[3] =  Frm_cnt&0xff;

	iobuffer= tmp[0]|(tmp[1]<<8)|(tmp[2]<<16)|(tmp[3]<<24);
	
	tbs_write(TBSECP3_GPIO_BASE, (0xA+tuner)*4,iobuffer);

	if( Frm_cnt==0) return 0;

	//set ci clk preset
	clk_preset = Frm_cnt/408;
	if(clk_preset>0xf)clk_preset=0xf;
	iobuffer =  tbs_read(tuner?0x7000:0x6000,0x04*4);
	iobuffer = (iobuffer&0xF0FFFFFF)|(clk_preset<<24);
	
	tbs_write(tuner?0x7000:0x6000,0x04*4,iobuffer);
		
	return 1;

}

static struct stid135_cfg tbs6903x_stid135_cfg = {
	.adr		= 0x68,
	.clk		= 27,
	.ts_mode	= TS_2PAR,
	.set_voltage	= NULL,
	.write_properties = ecp3_spi_write, 
	.read_properties = ecp3_spi_read,
	.set_TSsampling = NULL,
	.set_TSparam = NULL,
};

static struct stid135_cfg tbs6909x_stid135_cfg = {
	.adr		= 0x68,
	.clk		= 27,
	.ts_mode	= TS_STFE,
	.set_voltage	= max_set_voltage,
	.write_properties = ecp3_spi_write, 
	.read_properties = ecp3_spi_read,
	.set_TSsampling = NULL,
	.set_TSparam = NULL,
};

static struct stid135_cfg tbs6912_stid135_cfg = {
	.adr		= 0x68,
	.clk		= 27,
	.ts_mode	= TS_2PAR,
	.set_voltage	= NULL,
	.write_properties = ecp3_spi_write, 
	.read_properties = ecp3_spi_read,
	.set_TSsampling = Set_TSsampling,
	.set_TSparam = Set_TSparam,
};

static struct rda5816_config rda5816_cfg[] = {
	{
		.i2c_adr = 0x8,
		.xtal   = 1,    //1=27M  0=24M
	},
	{
		.i2c_adr = 0x9,
		.xtal   = 1,
	},
};

static int tbsecp3_frontend_attach(struct tbsecp3_adapter *adapter)
{
	struct tbsecp3_dev *dev = adapter->dev;
	struct pci_dev *pci = dev->pci_dev;

	struct si2168_config si2168_config;
	struct si2183_config si2183_config;
	struct si2157_config si2157_config;
	struct mn88436_config mn88436_config;
	struct mxl603_config mxl603_config;
	struct mtv23x_config mtv23x_config;
	struct gx1503_config gx1503_config;

	struct i2c_board_info info;
	struct i2c_adapter *i2c = &adapter->i2c->i2c_adap;
	struct i2c_client *client_demod, *client_tuner;

	adapter->fe = NULL;
	adapter->fe2 = NULL;
	adapter->i2c_client_demod = NULL;
	adapter->i2c_client_tuner = NULL;

	if((TBSECP3_BOARD_TBS6304 != dev->info->board_id) && (TBSECP3_BOARD_TBS6308 != dev->info->board_id)){
		reset_demod(adapter);
		set_mac_address(adapter);
	}

	switch (dev->info->board_id) {
	   case TBSECP3_BOARD_TBS6508:
		/* attach demod */
		memset(&si2183_config, 0, sizeof(si2183_config));
		si2183_config.i2c_adapter = &i2c;
		si2183_config.fe = &adapter->fe;
		si2183_config.ts_mode = SI2183_TS_PARALLEL;
		si2183_config.ts_clock_gapped = true;
		si2183_config.rf_in = adapter->nr;
		si2183_config.RF_switch = NULL;
		si2183_config.read_properties = ecp3_spi_read;
		si2183_config.write_properties = ecp3_spi_write;
	
		memset(&info, 0, sizeof(struct i2c_board_info));
		strlcpy(info.type, "si2183", I2C_NAME_SIZE);
		info.addr = (adapter->nr %2) ? 0x67 : 0x64;
		si2183_config.agc_mode = (adapter->nr %2)? 0x5 : 0x4;
		info.platform_data = &si2183_config;
		request_module(info.type);
		client_demod = i2c_new_device(i2c, &info);
		if (client_demod == NULL ||
			client_demod->dev.driver == NULL)
			goto frontend_atach_fail;
		if (!try_module_get(client_demod->dev.driver->owner)) {
			i2c_unregister_device(client_demod);
			goto frontend_atach_fail;
		}
		adapter->i2c_client_demod = client_demod;
	
		/* dvb core doesn't support 2 tuners for 1 demod so
		  we split the adapter in 2 frontends */
		adapter->fe2 = &adapter->_fe2;
		memcpy(adapter->fe2, adapter->fe, sizeof(struct dvb_frontend));
	
	
		/* terrestrial tuner */
		memset(adapter->fe->ops.delsys, 0, MAX_DELSYS);
		adapter->fe->ops.delsys[0] = SYS_DVBT;
		adapter->fe->ops.delsys[1] = SYS_DVBT2;
		adapter->fe->ops.delsys[2] = SYS_DVBC_ANNEX_A;
		adapter->fe->ops.delsys[3] = SYS_ISDBT;
		adapter->fe->ops.delsys[4] = SYS_DVBC_ANNEX_B;
	
		/* attach tuner */
		memset(&si2157_config, 0, sizeof(si2157_config));
		si2157_config.fe = adapter->fe;
		si2157_config.if_port = 1;
	
		memset(&info, 0, sizeof(struct i2c_board_info));
		strlcpy(info.type, "si2157", I2C_NAME_SIZE);
		info.addr = (adapter->nr %2) ? 0x60 : 0x61;
		info.platform_data = &si2157_config;
		request_module(info.type);
		client_tuner = i2c_new_device(i2c, &info);
		if (client_tuner == NULL ||
			client_tuner->dev.driver == NULL)
			goto frontend_atach_fail;
	
		if (!try_module_get(client_tuner->dev.driver->owner)) {
			i2c_unregister_device(client_tuner);
			goto frontend_atach_fail;
		}
		adapter->i2c_client_tuner = client_tuner;
	
	
		/* sattelite tuner */
		memset(adapter->fe2->ops.delsys, 0, MAX_DELSYS);
		adapter->fe2->ops.delsys[0] = SYS_DVBS;
		adapter->fe2->ops.delsys[1] = SYS_DVBS2;
		adapter->fe2->ops.delsys[2] = SYS_DSS;
		adapter->fe2->id = 1;
		if (dvb_attach(rda5816_attach, adapter->fe2, &rda5816_cfg[(adapter->nr %2)],
				i2c) == NULL) {
			dev_err(&dev->pci_dev->dev,
				"frontend %d tuner attach failed\n",
				adapter->nr);
			goto frontend_atach_fail;
		}
		if (tbsecp3_attach_sec(adapter, adapter->fe2) == NULL) {
			dev_warn(&dev->pci_dev->dev,
				"error attaching lnb control on adapter %d\n",
				adapter->nr);
		}
		break;		
	case TBSECP3_BOARD_TBS6904X:
		memset(&si2183_config, 0, sizeof(si2183_config));
		si2183_config.i2c_adapter = &i2c;
		si2183_config.fe = &adapter->fe;
		si2183_config.ts_mode =  SI2183_TS_PARALLEL ;
		si2183_config.ts_clock_gapped = true;
		si2183_config.rf_in = adapter->nr;
		si2183_config.RF_switch = NULL;
		si2183_config.start_clk_mode = 1;
		si2183_config.read_properties = ecp3_spi_read;
		si2183_config.write_properties = ecp3_spi_write;
		memset(&info, 0, sizeof(struct i2c_board_info));
		strlcpy(info.type, "si2183", I2C_NAME_SIZE);

		info.addr = (adapter->nr %2)? 0x64 : 0x67;
		 si2183_config.agc_mode = (adapter->nr%2)? 0x4 : 0x5;
	
		info.platform_data = &si2183_config;
		request_module(info.type);
		client_demod = i2c_new_device(i2c, &info);
		if (client_demod == NULL ||
			client_demod->dev.driver == NULL)
		    goto frontend_atach_fail;
		if (!try_module_get(client_demod->dev.driver->owner)) {
		    i2c_unregister_device(client_demod);
		    goto frontend_atach_fail;
		}
		adapter->i2c_client_demod = client_demod;

		memset(adapter->fe->ops.delsys, 0, MAX_DELSYS);
		adapter->fe->ops.delsys[0] = SYS_DVBS;
		adapter->fe->ops.delsys[1] = SYS_DVBS2;
		adapter->fe->ops.delsys[2] = SYS_DSS;
		
		if (dvb_attach(av201x_attach, adapter->fe, &tbs6522_av201x_cfg[(adapter->nr%2)],
			    i2c) == NULL) {
		    dvb_frontend_detach(adapter->fe);
		    adapter->fe = NULL;
		    dev_err(&dev->pci_dev->dev,
			    "frontend %d tuner attach failed\n",
			    adapter->nr);
		    goto frontend_atach_fail;
		}


		if (tbsecp3_attach_sec(adapter, adapter->fe) == NULL) {
		    dev_warn(&dev->pci_dev->dev,
			    "error attaching lnb control on adapter %d\n",
			    adapter->nr);
		}

		break;
	case TBSECP3_BOARD_TBS6308:
		adapter->fe = dvb_attach(tas2971_attach, &tbs6308_demod_cfg, i2c);
		if (adapter->fe == NULL)
		    goto frontend_atach_fail;
		    if(adapter->nr <4)
		    {
			    if(tbs6304_read_mac(adapter)==0)
				    tbs6304_read_mac(adapter);
		    }
		    else
		    {
			    if(tbs6308_read_mac_ext(adapter)==0)
				    tbs6308_read_mac_ext(adapter);//try again
		    }
		break;

	case TBSECP3_BOARD_TBS6304:
		adapter->fe = dvb_attach(tas2971_attach, &tbs6304_demod_cfg[adapter->nr], i2c);
		if (adapter->fe == NULL)
		    goto frontend_atach_fail;
		if(tbs6304_read_mac(adapter)==0)
		    tbs6304_read_mac(adapter);
		break;

	case TBSECP3_BOARD_TBS6301:
		adapter->fe = dvb_attach(tas2971_attach, &tbs6301_demod_cfg, i2c);
		if (adapter->fe == NULL)
		    goto frontend_atach_fail;

		tbs6301_read_mac(adapter);
		break;

	case TBSECP3_BOARD_TBS690a:
		adapter->fe = dvb_attach(tas2971_attach, &tbs6904_demod_cfg[adapter->nr], i2c);
		if (adapter->fe == NULL)
		    goto frontend_atach_fail;

		// init asi
		int regdata;
		u8 mpbuf[4];
		mpbuf[0] = adapter->nr; //0--3 select value
		tbs_write( TBSECP3_GPIO_BASE, 0x34 , *(u32 *)&mpbuf[0]); // select chip : 13*8 =104=0x68 select address
		//u32 mpbuf = adapter->nr;
		//tbs_write( TBSECP3_GPIO_BASE, 0x34 , mpbuf); // select chip : 13*8 =104=0x68 select address
		// ==***********************************************************************

		asi_chip_reset(dev,ASI0_BASEADDRESS);  //asi chip reset;

		mpbuf[0] = 1; //active spi bus from "z"
		tbs_write( ASI0_BASEADDRESS, ASI_SPI_ENABLE, *(u32 *)&mpbuf[0]);

		regdata = asi_read16bit(dev,ASI0_BASEADDRESS,0x24);
		asi_write16bit(dev,ASI0_BASEADDRESS,0x24,3);	 
		regdata = asi_read16bit(dev,ASI0_BASEADDRESS, 0x24);

		mpbuf[0] = 0; //spi disable, enter "z" state;
		tbs_write( ASI0_BASEADDRESS, ASI_SPI_ENABLE, *(u32 *)&mpbuf[0]);

		//==****************************************************************************************
		// ~~init asi
		break;

	case TBSECP3_BOARD_TBS6514:
		memset(&gx1503_config,0,sizeof(gx1503_config));
		gx1503_config.i2c_adapter =&i2c;
		gx1503_config.fe = &adapter->fe;
		gx1503_config.clk_freq = 30400;//KHZ
		gx1503_config.ts_mode = 1;
		gx1503_config.i2c_wr_max = 8;

		memset(&info, 0, sizeof(struct i2c_board_info));
		strlcpy(info.type, "gx1503", I2C_NAME_SIZE);
		info.addr = 0x30;
		info.platform_data = &gx1503_config;
		request_module(info.type);
		client_demod = i2c_new_device(i2c,&info);
		if(client_demod == NULL||
			client_demod->dev.driver == NULL)
		    goto frontend_atach_fail;

		if (!try_module_get(client_demod->dev.driver->owner)) {
		    i2c_unregister_device(client_demod);
		    goto frontend_atach_fail;
		}

		adapter->i2c_client_demod = client_demod;

		/*attach tuner*/
		memset(&si2157_config, 0, sizeof(si2157_config));
		si2157_config.fe = adapter->fe;
		si2157_config.if_port = 0;

		memset(&info, 0, sizeof(struct i2c_board_info));
		strlcpy(info.type, "si2157", I2C_NAME_SIZE);
		info.addr = 0x60;
		info.platform_data = &si2157_config;
		request_module(info.type);
		client_tuner = i2c_new_device(i2c, &info);
		if (client_tuner == NULL ||
			client_tuner->dev.driver == NULL)
		    goto frontend_atach_fail;

		if (!try_module_get(client_tuner->dev.driver->owner)) {
		    i2c_unregister_device(client_tuner);
		    goto frontend_atach_fail;
		}
		adapter->i2c_client_tuner = client_tuner;
		break;

	case TBSECP3_BOARD_TBS6814:
		memset(&mtv23x_config, 0, sizeof(mtv23x_config));
		mtv23x_config.fe = &adapter->fe;
		mtv23x_config.clk_freq = 32000;
		mtv23x_config.ts_mode  = 6;
		mtv23x_config.i2c_wr_max = 32;

		memset(&info, 0, sizeof(struct i2c_board_info));
		strlcpy(info.type, "mtv23x", I2C_NAME_SIZE);
		info.addr = (adapter->nr%2)? 0x44 : 0x43;
		info.platform_data = &mtv23x_config;
		request_module(info.type);
		client_demod = i2c_new_device(i2c, &info);
		if (client_demod == NULL ||
			client_demod->dev.driver == NULL)
		    goto frontend_atach_fail;
		if (!try_module_get(client_demod->dev.driver->owner)) {
		    i2c_unregister_device(client_demod);
		    goto frontend_atach_fail;
		}
		adapter->i2c_client_demod = client_demod;
		break;

	case TBSECP3_BOARD_TBS6209:
		/* attach demod */
		memset(&si2183_config, 0, sizeof(si2183_config));
		si2183_config.i2c_adapter = &i2c;
		si2183_config.fe = &adapter->fe;
		si2183_config.ts_mode = SI2183_TS_SERIAL;
		si2183_config.ts_clock_gapped = true;
		si2183_config.rf_in = adapter->nr;
		si2183_config.RF_switch = NULL;
		si2183_config.read_properties = ecp3_spi_read;
		si2183_config.write_properties = ecp3_spi_write;
			    
		memset(&info, 0, sizeof(struct i2c_board_info));
		strlcpy(info.type, "si2183", I2C_NAME_SIZE);
		info.addr = (adapter->nr%2)? 0x67 : 0x64;
		si2183_config.agc_mode = (adapter->nr%2)? 0x5 : 0x4;
		info.platform_data = &si2183_config;
		request_module(info.type);
		client_demod = i2c_new_device(i2c, &info);
		if (client_demod == NULL ||
			client_demod->dev.driver == NULL)
		    goto frontend_atach_fail;
		if (!try_module_get(client_demod->dev.driver->owner)) {
		    i2c_unregister_device(client_demod);
		    goto frontend_atach_fail;
		}
		adapter->i2c_client_demod = client_demod;

		/* terrestrial tuner */
		memset(adapter->fe->ops.delsys, 0, MAX_DELSYS);
		adapter->fe->ops.delsys[0] = SYS_DVBT;
		adapter->fe->ops.delsys[1] = SYS_DVBT2;
		adapter->fe->ops.delsys[2] = SYS_DVBC_ANNEX_A;
		adapter->fe->ops.delsys[3] = SYS_ISDBT;
		adapter->fe->ops.delsys[4] = SYS_DVBC_ANNEX_B;

		/* attach tuner */
		memset(&si2157_config, 0, sizeof(si2157_config));
		si2157_config.fe = adapter->fe;
		si2157_config.if_port = 1;

		memset(&info, 0, sizeof(struct i2c_board_info));
		strlcpy(info.type, "si2157", I2C_NAME_SIZE);
		info.addr = (adapter->nr %2)? 0x60 : 0x63;
		info.platform_data = &si2157_config;
		request_module(info.type);
		client_tuner = i2c_new_device(i2c, &info);
		if (client_tuner == NULL ||
			client_tuner->dev.driver == NULL)
		    goto frontend_atach_fail;

		if (!try_module_get(client_tuner->dev.driver->owner)) {
		    i2c_unregister_device(client_tuner);
		    goto frontend_atach_fail;
		}
		adapter->i2c_client_tuner = client_tuner;
		break;

	case TBSECP3_BOARD_TBS6704:
		/* attach demod */
		memset(&mn88436_config, 0, sizeof(mn88436_config));
		mn88436_config.fe = &adapter->fe;
		mn88436_config.ts_mode = 0;
		mn88436_config.i2c_wr_max = 32;

		memset(&info, 0, sizeof(struct i2c_board_info));
		strlcpy(info.type, "mn88436", I2C_NAME_SIZE);
		info.addr = 0x18;
		info.platform_data = &mn88436_config;
		request_module(info.type);
		client_demod = i2c_new_device(i2c, &info);
		if (client_demod == NULL ||
			client_demod->dev.driver == NULL)
		    goto frontend_atach_fail;
		if (!try_module_get(client_demod->dev.driver->owner)) {
		    i2c_unregister_device(client_demod);
		    goto frontend_atach_fail;
		}
		adapter->i2c_client_demod = client_demod;

		/* attach tuner */
		memset(&mxl603_config, 0, sizeof(mxl603_config));
		mxl603_config.fe = adapter->fe;
		mxl603_config.xtalFreqSel= 1; //0:16M ,1:24M
		mxl603_config.agcType = 0 ; //0:self 1:external
		mxl603_config.ifOutFreq = MXL603_IF_5MHz;
		mxl603_config.manualIFFreqSet = false;
		mxl603_config.manualIFOutFreqInKHz = 0 ;//if manual set ,input the freq

		memset(&info, 0, sizeof(struct i2c_board_info));
		strlcpy(info.type, "mxl603", I2C_NAME_SIZE);
		info.addr = 0x60;
		info.platform_data = &mxl603_config;
		request_module(info.type);
		client_tuner = i2c_new_device(i2c, &info);
		if (client_tuner == NULL ||
			client_tuner->dev.driver == NULL)
		    goto frontend_atach_fail;

		if (!try_module_get(client_tuner->dev.driver->owner)) {
		    i2c_unregister_device(client_tuner);
		    goto frontend_atach_fail;
		}
		adapter->i2c_client_tuner = client_tuner;
		break;

	case TBSECP3_BOARD_TBS6205:
	case TBSECP3_BOARD_TBS6281SE:
		/* attach demod */
		memset(&si2168_config, 0, sizeof(si2168_config));
		si2168_config.i2c_adapter = &i2c;
		si2168_config.fe = &adapter->fe;
		si2168_config.ts_mode = SI2168_TS_PARALLEL;
		si2168_config.ts_clock_gapped = true;

		memset(&info, 0, sizeof(struct i2c_board_info));
		strlcpy(info.type, "si2168", I2C_NAME_SIZE);
		info.addr = 0x64;
		info.platform_data = &si2168_config;
		request_module(info.type);
		client_demod = i2c_new_device(i2c, &info);
		if (client_demod == NULL ||
			client_demod->dev.driver == NULL)
		    goto frontend_atach_fail;
		if (!try_module_get(client_demod->dev.driver->owner)) {
		    i2c_unregister_device(client_demod);
		    goto frontend_atach_fail;
		}
		adapter->i2c_client_demod = client_demod;

		/* attach tuner */
		memset(&si2157_config, 0, sizeof(si2157_config));
		si2157_config.fe = adapter->fe;
		si2157_config.if_port = 1;

		memset(&info, 0, sizeof(struct i2c_board_info));
		strlcpy(info.type, "si2157", I2C_NAME_SIZE);
		info.addr = 0x60;
		info.platform_data = &si2157_config;
		request_module(info.type);
		client_tuner = i2c_new_device(i2c, &info);
		if (client_tuner == NULL ||
			client_tuner->dev.driver == NULL)
		    goto frontend_atach_fail;

		if (!try_module_get(client_tuner->dev.driver->owner)) {
		    i2c_unregister_device(client_tuner);
		    goto frontend_atach_fail;
		}
		adapter->i2c_client_tuner = client_tuner;
		break;

	case TBSECP3_BOARD_TBS6290SE:
		/* attach demod */
		memset(&si2168_config, 0, sizeof(si2168_config));
		si2168_config.i2c_adapter = &i2c;
		si2168_config.fe = &adapter->fe;
		si2168_config.ts_mode = SI2168_TS_SERIAL;//zc2016/07/20
		si2168_config.ts_clock_gapped = true;
		si2168_config.ts_clock_inv=0;//zc2016/07/20

		memset(&info, 0, sizeof(struct i2c_board_info));
		strlcpy(info.type, "si2168", I2C_NAME_SIZE);
		info.addr = 0x64;
		info.platform_data = &si2168_config;
		request_module(info.type);
		client_demod = i2c_new_device(i2c, &info);
		if (client_demod == NULL ||
			client_demod->dev.driver == NULL)
		    goto frontend_atach_fail;
		if (!try_module_get(client_demod->dev.driver->owner)) {
		    i2c_unregister_device(client_demod);
		    goto frontend_atach_fail;
		}
		adapter->i2c_client_demod = client_demod;

		/* attach tuner */
		memset(&si2157_config, 0, sizeof(si2157_config));
		si2157_config.fe = adapter->fe;
		si2157_config.if_port = 1;

		memset(&info, 0, sizeof(struct i2c_board_info));
		strlcpy(info.type, "si2157", I2C_NAME_SIZE);
		info.addr = 0x60;
		info.platform_data = &si2157_config;
		request_module(info.type);
		client_tuner = i2c_new_device(i2c, &info);
		if (client_tuner == NULL ||
			client_tuner->dev.driver == NULL)
		    goto frontend_atach_fail;

		if (!try_module_get(client_tuner->dev.driver->owner)) {
		    i2c_unregister_device(client_tuner);
		    goto frontend_atach_fail;
		}
		adapter->i2c_client_tuner = client_tuner;
		tbsecp3_ca_init(adapter, adapter->nr);
		break;

	case TBSECP3_BOARD_TBS6522:
	case TBSECP3_BOARD_TBS6504:
		/* attach demod */
		memset(&si2183_config, 0, sizeof(si2183_config));
		si2183_config.i2c_adapter = &i2c;
		si2183_config.fe = &adapter->fe;
		si2183_config.ts_mode = SI2183_TS_PARALLEL;
		si2183_config.ts_clock_gapped = true;
		si2183_config.rf_in = adapter->nr;
		si2183_config.RF_switch = NULL;
		si2183_config.read_properties = ecp3_spi_read;
		si2183_config.write_properties = ecp3_spi_write;

		memset(&info, 0, sizeof(struct i2c_board_info));
		strlcpy(info.type, "si2183", I2C_NAME_SIZE);
		info.addr = (adapter->nr %2) ? 0x64 : 0x67;
		si2183_config.agc_mode = (adapter->nr %2)? 0x4 : 0x5;
		info.platform_data = &si2183_config;
		request_module(info.type);
		client_demod = i2c_new_device(i2c, &info);
		if (client_demod == NULL ||
			client_demod->dev.driver == NULL)
		    goto frontend_atach_fail;
		if (!try_module_get(client_demod->dev.driver->owner)) {
		    i2c_unregister_device(client_demod);
		    goto frontend_atach_fail;
		}
		adapter->i2c_client_demod = client_demod;

		/* dvb core doesn't support 2 tuners for 1 demod so
		  we split the adapter in 2 frontends */
		adapter->fe2 = &adapter->_fe2;
		memcpy(adapter->fe2, adapter->fe, sizeof(struct dvb_frontend));


		/* terrestrial tuner */
		memset(adapter->fe->ops.delsys, 0, MAX_DELSYS);
		adapter->fe->ops.delsys[0] = SYS_DVBT;
		adapter->fe->ops.delsys[1] = SYS_DVBT2;
		adapter->fe->ops.delsys[2] = SYS_DVBC_ANNEX_A;
		adapter->fe->ops.delsys[3] = SYS_ISDBT;
		adapter->fe->ops.delsys[4] = SYS_DVBC_ANNEX_B;

		/* attach tuner */
		memset(&si2157_config, 0, sizeof(si2157_config));
		si2157_config.fe = adapter->fe;
		si2157_config.if_port = 1;

		memset(&info, 0, sizeof(struct i2c_board_info));
		strlcpy(info.type, "si2157", I2C_NAME_SIZE);
		info.addr = (adapter->nr %2) ? 0x61 : 0x60;
		info.platform_data = &si2157_config;
		request_module(info.type);
		client_tuner = i2c_new_device(i2c, &info);
		if (client_tuner == NULL ||
			client_tuner->dev.driver == NULL)
		    goto frontend_atach_fail;

		if (!try_module_get(client_tuner->dev.driver->owner)) {
		    i2c_unregister_device(client_tuner);
		    goto frontend_atach_fail;
		}
		adapter->i2c_client_tuner = client_tuner;


		/* sattelite tuner */
		memset(adapter->fe2->ops.delsys, 0, MAX_DELSYS);
		adapter->fe2->ops.delsys[0] = SYS_DVBS;
		adapter->fe2->ops.delsys[1] = SYS_DVBS2;
		adapter->fe2->ops.delsys[2] = SYS_DSS;
		adapter->fe2->id = 1;
		if (dvb_attach(av201x_attach, adapter->fe2, &tbs6522_av201x_cfg[(adapter->nr %2)],
			    i2c) == NULL) {
		    dev_err(&dev->pci_dev->dev,
			    "frontend %d tuner attach failed\n",
			    adapter->nr);
		    goto frontend_atach_fail;
		}
		if (tbsecp3_attach_sec(adapter, adapter->fe2) == NULL) {
		    dev_warn(&dev->pci_dev->dev,
			    "error attaching lnb control on adapter %d\n",
			    adapter->nr);
		}
		break;

	case TBSECP3_BOARD_TBS6528:
	case TBSECP3_BOARD_TBS6590:
		/* attach demod */
		memset(&si2183_config, 0, sizeof(si2183_config));
		si2183_config.i2c_adapter = &i2c;
		si2183_config.fe = &adapter->fe;
		si2183_config.ts_mode = pci->subsystem_vendor==0x6528 ? SI2183_TS_PARALLEL : SI2183_TS_SERIAL;
		si2183_config.ts_clock_gapped = true;
		si2183_config.rf_in = adapter->nr;
		si2183_config.RF_switch = RF_switch;
		si2183_config.read_properties = ecp3_spi_read;
		si2183_config.write_properties = ecp3_spi_write;

		memset(&info, 0, sizeof(struct i2c_board_info));
		strlcpy(info.type, "si2183", I2C_NAME_SIZE);
		if(pci->subsystem_vendor==0x6528)
		{
		    info.addr = 0x67;
		    si2183_config.agc_mode = 0x5 ;
		}
		else{
		    info.addr = adapter->nr ? 0x67 : 0x64;
		    si2183_config.agc_mode = adapter->nr? 0x5 : 0x4;
		}
		info.platform_data = &si2183_config;
		request_module(info.type);
		client_demod = i2c_new_device(i2c, &info);
		if (client_demod == NULL ||
			client_demod->dev.driver == NULL)
		    goto frontend_atach_fail;
		if (!try_module_get(client_demod->dev.driver->owner)) {
		    i2c_unregister_device(client_demod);
		    goto frontend_atach_fail;
		}
		adapter->i2c_client_demod = client_demod;

		/* dvb core doesn't support 2 tuners for 1 demod so
		  we split the adapter in 2 frontends */
		adapter->fe2 = &adapter->_fe2;
		memcpy(adapter->fe2, adapter->fe, sizeof(struct dvb_frontend));


		/* terrestrial tuner */
		memset(adapter->fe->ops.delsys, 0, MAX_DELSYS);
		adapter->fe->ops.delsys[0] = SYS_DVBT;
		adapter->fe->ops.delsys[1] = SYS_DVBT2;
		adapter->fe->ops.delsys[2] = SYS_DVBC_ANNEX_A;
		adapter->fe->ops.delsys[3] = SYS_ISDBT;
		adapter->fe->ops.delsys[4] = SYS_DVBC_ANNEX_B;

		/* attach tuner */
		memset(&si2157_config, 0, sizeof(si2157_config));
		si2157_config.fe = adapter->fe;
		si2157_config.if_port = 1;

		memset(&info, 0, sizeof(struct i2c_board_info));
		strlcpy(info.type, "si2157", I2C_NAME_SIZE);
		if(pci->subsystem_vendor==0x6528)info.addr = 0x61;
		else
		    info.addr = adapter->nr ? 0x61 : 0x60;

		info.platform_data = &si2157_config;
		request_module(info.type);
		client_tuner = i2c_new_device(i2c, &info);
		if (client_tuner == NULL ||
			client_tuner->dev.driver == NULL)
		    goto frontend_atach_fail;

		if (!try_module_get(client_tuner->dev.driver->owner)) {
		    i2c_unregister_device(client_tuner);
		    goto frontend_atach_fail;
		}
		adapter->i2c_client_tuner = client_tuner;


		/* sattelite tuner */
		memset(adapter->fe2->ops.delsys, 0, MAX_DELSYS);
		adapter->fe2->ops.delsys[0] = SYS_DVBS;
		adapter->fe2->ops.delsys[1] = SYS_DVBS2;
		adapter->fe2->ops.delsys[2] = SYS_DSS;
		adapter->fe2->id = 1;
		if(pci->subsystem_vendor==0x6528)
		{
		    if (dvb_attach(av201x_attach, adapter->fe2, &tbs6522_av201x_cfg[1],
				i2c) == NULL) {
			dev_err(&dev->pci_dev->dev,
				"frontend %d tuner attach failed\n",
				adapter->nr);
			goto frontend_atach_fail;
		    }
		}
		else{
		    if (dvb_attach(av201x_attach, adapter->fe2, &tbs6522_av201x_cfg[adapter->nr],
				i2c) == NULL) {
			dev_err(&dev->pci_dev->dev,
				"frontend %d tuner attach failed\n",
				adapter->nr);
			goto frontend_atach_fail;
		    }
		}
		if (tbsecp3_attach_sec(adapter, adapter->fe2) == NULL) {
		    dev_warn(&dev->pci_dev->dev,
			    "error attaching lnb control on adapter %d\n",
			    adapter->nr);
		}

		tbsecp3_ca_init(adapter, adapter->nr);
		break;

	case TBSECP3_BOARD_TBS6902:
		adapter->fe = dvb_attach(tas2101_attach, &tbs6902_demod_cfg[adapter->nr], i2c);
		if (adapter->fe == NULL)
		    goto frontend_atach_fail;

		if (dvb_attach(av201x_attach, adapter->fe, &tbs6902_av201x_cfg,
			    tas2101_get_i2c_adapter(adapter->fe, 2)) == NULL) {
		    dvb_frontend_detach(adapter->fe);
		    adapter->fe = NULL;
		    dev_err(&dev->pci_dev->dev,
			    "frontend %d tuner attach failed\n",
			    adapter->nr);
		    goto frontend_atach_fail;
		}

		if (tbsecp3_attach_sec(adapter, adapter->fe) == NULL) {
		    dev_warn(&dev->pci_dev->dev,
			    "error attaching lnb control on adapter %d\n",
			    adapter->nr);
		}
		break;

	case TBSECP3_BOARD_TBS6903:
	case TBSECP3_BOARD_TBS6905:
	case TBSECP3_BOARD_TBS6908:
		adapter->fe = dvb_attach(stv091x_attach, i2c,
			&tbs6903_stv0910_cfg, adapter->nr & 1);
		if (adapter->fe == NULL)
		    goto frontend_atach_fail;

		if (dvb_attach(stv6120_attach, adapter->fe, i2c, &tbs6903_stv6120_cfg, 1 - (adapter->nr & 1)) == NULL) {
		    dvb_frontend_detach(adapter->fe);
		    adapter->fe = NULL;
		    dev_err(&dev->pci_dev->dev,
			    "frontend %d tuner attach failed\n",
			    adapter->nr);
		    goto frontend_atach_fail;
		}
		if (tbsecp3_attach_sec(adapter, adapter->fe) == NULL) {
		    dev_warn(&dev->pci_dev->dev,
			    "error attaching lnb control on adapter %d\n",
			    adapter->nr);
		}
		break;

	case TBSECP3_BOARD_TBS6904:
		adapter->fe = dvb_attach(tas2101_attach, &tbs6904_demod_cfg[adapter->nr], i2c);
		if (adapter->fe == NULL)
		    goto frontend_atach_fail;

		if (dvb_attach(av201x_attach, adapter->fe, &tbs6904_av201x_cfg,
			    tas2101_get_i2c_adapter(adapter->fe, 2)) == NULL) {
		    dvb_frontend_detach(adapter->fe);
		    adapter->fe = NULL;
		    dev_err(&dev->pci_dev->dev,
			    "frontend %d tuner attach failed\n",
			    adapter->nr);
		    goto frontend_atach_fail;
		}

		if (tbsecp3_attach_sec(adapter, adapter->fe) == NULL) {
		    dev_warn(&dev->pci_dev->dev,
			    "error attaching lnb control on adapter %d\n",
			    adapter->nr);
		}
	    break;

	case TBSECP3_BOARD_TBS6909:
		/*
		  tmp = tbs_read(TBS_GPIO_BASE, 0x20);
		  printk("RD 0x20 = %x\n", tmp);
		  tbs_write(TBS_GPIO_BASE, 0x20, tmp & 0xfffe);
		  tmp = tbs_read(TBS_GPIO_BASE, 0x20);
		  printk("RD 0x20 = %x\n", tmp);

		  tmp = tbs_read(TBS_GPIO_BASE, 0x24);
		  printk("RD 0x24 = %x\n", tmp);
		  tbs_write(TBS_GPIO_BASE, 0x24, tmp & 0xfffc);
		  tmp = tbs_read(TBS_GPIO_BASE, 0x24);
		  printk("RD 0x24 = %x\n", tmp);
		  */

		adapter->fe = dvb_attach(mxl58x_attach, i2c,
			&tbs6909_mxl58x_cfg, adapter->nr);
		if (adapter->fe == NULL)
		    goto frontend_atach_fail;

		//	adapter->fe->ops.diseqc_send_master_cmd = max_send_master_cmd;
		//	adapter->fe->ops.diseqc_send_burst = max_send_burst;

		if (tbsecp3_attach_sec(adapter, adapter->fe) == NULL) {
		    dev_warn(&dev->pci_dev->dev,
			    "error attaching lnb control on adapter %d\n",
			    adapter->nr);
		}
		break;

	case TBSECP3_BOARD_TBS6910:
		adapter->fe = dvb_attach(tas2101_attach, &tbs6910_demod_cfg[adapter->nr], i2c);
		if (adapter->fe == NULL)
		    goto frontend_atach_fail;

		if (dvb_attach(av201x_attach, adapter->fe, &tbs6910_av201x_cfg,
			    tas2101_get_i2c_adapter(adapter->fe, 2)) == NULL) {
		    dvb_frontend_detach(adapter->fe);
		    adapter->fe = NULL;
		    dev_err(&dev->pci_dev->dev,
			    "frontend %d tuner attach failed\n",
			    adapter->nr);
		    goto frontend_atach_fail;
		}
		if (tbsecp3_attach_sec(adapter, adapter->fe) == NULL) {
		    dev_warn(&dev->pci_dev->dev,
			    "error attaching lnb control on adapter %d\n",
			    adapter->nr);
		}

		tbsecp3_ca_init(adapter, adapter->nr);
		break;

	case TBSECP3_BOARD_TBS6909X:
		adapter->fe = dvb_attach(stid135_attach, i2c,
				&tbs6909x_stid135_cfg, adapter->nr, adapter->nr/2);
		if (adapter->fe == NULL)
			goto frontend_atach_fail;
		break;

	case TBSECP3_BOARD_TBS6903X:
	case TBSECP3_BOARD_TBS6912:
		if(pci->subsystem_vendor==0x6912)
			adapter->fe = dvb_attach(stid135_attach, i2c,
					&tbs6912_stid135_cfg, adapter->nr ? 2 : 0, adapter->nr ? 3 : 0);
		else		
			adapter->fe = dvb_attach(stid135_attach, i2c,
				&tbs6903x_stid135_cfg, adapter->nr ? 2 : 0, adapter->nr ? 3 : 0);
		if (adapter->fe == NULL)
			goto frontend_atach_fail;

		if (tbsecp3_attach_sec(adapter, adapter->fe) == NULL) {
			dev_warn(&dev->pci_dev->dev,
				"error attaching lnb control on adapter %d\n",
				adapter->nr);
		}
		if(pci->subsystem_vendor==0x6912)
			tbsecp3_ca_init(adapter, adapter->nr);
		
		break;

	default:
		dev_warn(&dev->pci_dev->dev, "unknonw card\n");
		return -ENODEV;
		break;
	}
	strlcpy(adapter->fe->ops.info.name,dev->info->name,52);
	if (adapter->fe2)
		strlcpy(adapter->fe2->ops.info.name,dev->info->name,52);
	return 0;

frontend_atach_fail:
	tbsecp3_i2c_remove_clients(adapter);
	if (adapter->fe != NULL)
	    dvb_frontend_detach(adapter->fe);
	adapter->fe = NULL;
	dev_err(&dev->pci_dev->dev, "TBSECP3 frontend %d attach failed\n",
		adapter->nr);

	return -ENODEV;
}

int tbsecp3_dvb_init(struct tbsecp3_adapter *adapter)
{
    struct tbsecp3_dev *dev = adapter->dev;
    struct dvb_adapter *adap = &adapter->dvb_adapter;
    struct dvb_demux *dvbdemux = &adapter->demux;
    struct dmxdev *dmxdev;
    struct dmx_frontend *fe_hw;
    struct dmx_frontend *fe_mem;
    int ret;

    ret = dvb_register_adapter(adap, "TBSECP3 DVB Adapter",
            THIS_MODULE,
            &adapter->dev->pci_dev->dev,
            adapter_nr);
    if (ret < 0) {
        dev_err(&dev->pci_dev->dev, "error registering adapter\n");
        if (ret == -ENFILE)
            dev_err(&dev->pci_dev->dev,
                    "increase DVB_MAX_ADAPTERS (%d)\n",
                    DVB_MAX_ADAPTERS);
        return ret;
    }

    adap->priv = adapter;
    dvbdemux->priv = adapter;
    dvbdemux->filternum = 256;
    dvbdemux->feednum = 256;
    dvbdemux->start_feed = start_feed;
    dvbdemux->stop_feed = stop_feed;
    dvbdemux->write_to_decoder = NULL;
    dvbdemux->dmx.capabilities = (DMX_TS_FILTERING |
            DMX_SECTION_FILTERING |
            DMX_MEMORY_BASED_FILTERING);

    ret = dvb_dmx_init(dvbdemux);
    if (ret < 0) {
        dev_err(&dev->pci_dev->dev, "dvb_dmx_init failed\n");
        goto err0;
    }

    dmxdev = &adapter->dmxdev;

    dmxdev->filternum = 256;
    dmxdev->demux = &dvbdemux->dmx;
    dmxdev->capabilities = 0;

    ret = dvb_dmxdev_init(dmxdev, adap);
    if (ret < 0) {
        dev_err(&dev->pci_dev->dev, "dvb_dmxdev_init failed\n");
        goto err1;
    }

    fe_hw = &adapter->fe_hw;
    fe_mem = &adapter->fe_mem;

    fe_hw->source = DMX_FRONTEND_0;
    ret = dvbdemux->dmx.add_frontend(&dvbdemux->dmx, fe_hw);
    if ( ret < 0) {
        dev_err(&dev->pci_dev->dev, "dvb_dmx_init failed");
        goto err2;
    }

    fe_mem->source = DMX_MEMORY_FE;
    ret = dvbdemux->dmx.add_frontend(&dvbdemux->dmx, fe_mem);
    if (ret  < 0) {
        dev_err(&dev->pci_dev->dev, "dvb_dmx_init failed");
        goto err3;
    }

    ret = dvbdemux->dmx.connect_frontend(&dvbdemux->dmx, fe_hw);
    if (ret < 0) {
        dev_err(&dev->pci_dev->dev, "dvb_dmx_init failed");
        goto err4;
    }

    ret = dvb_net_init(adap, &adapter->dvbnet, adapter->dmxdev.demux);
    if (ret < 0) {
        dev_err(&dev->pci_dev->dev, "dvb_net_init failed");
        goto err5;
    }

    tbsecp3_frontend_attach(adapter);
    if (adapter->fe == NULL) {
        dev_err(&dev->pci_dev->dev, "frontend attach failed\n");
        ret = -ENODEV;
        goto err6;
    }

    ret = dvb_register_frontend(adap, adapter->fe);
    if (ret < 0) {
        dev_err(&dev->pci_dev->dev, "frontend register failed\n");
        goto err7;
    }

    if (adapter->fe2 != NULL) {
        ret = dvb_register_frontend(adap, adapter->fe2);
        if (ret < 0) {
            dev_err(&dev->pci_dev->dev, "frontend2 register failed\n");
        }
    }


    return ret;

err7:
    dvb_frontend_detach(adapter->fe);
err6:
    tbsecp3_release_sec(adapter->fe);

    dvb_net_release(&adapter->dvbnet);
err5:
    dvbdemux->dmx.close(&dvbdemux->dmx);
err4:
    dvbdemux->dmx.remove_frontend(&dvbdemux->dmx, fe_mem);
err3:
    dvbdemux->dmx.remove_frontend(&dvbdemux->dmx, fe_hw);
err2:
    dvb_dmxdev_release(dmxdev);
err1:
    dvb_dmx_release(dvbdemux);
err0:
    dvb_unregister_adapter(adap);
    return ret;
}

void tbsecp3_dvb_exit(struct tbsecp3_adapter *adapter)
{
    struct dvb_adapter *adap = &adapter->dvb_adapter;
    struct dvb_demux *dvbdemux = &adapter->demux;

    if (adapter->fe) {
        tbsecp3_ca_release(adapter);
        dvb_unregister_frontend(adapter->fe);
        tbsecp3_release_sec(adapter->fe);
        dvb_frontend_detach(adapter->fe);
        adapter->fe = NULL;

        if (adapter->fe2 != NULL) {
            dvb_unregister_frontend(adapter->fe2);
            tbsecp3_release_sec(adapter->fe2);
            dvb_frontend_detach(adapter->fe2);
            adapter->fe2 = NULL;
        }
    }
    dvb_net_release(&adapter->dvbnet);
    dvbdemux->dmx.close(&dvbdemux->dmx);
    dvbdemux->dmx.remove_frontend(&dvbdemux->dmx, &adapter->fe_mem);
    dvbdemux->dmx.remove_frontend(&dvbdemux->dmx, &adapter->fe_hw);
    dvb_dmxdev_release(&adapter->dmxdev);
    dvb_dmx_release(&adapter->demux);
    dvb_unregister_adapter(adap);
}

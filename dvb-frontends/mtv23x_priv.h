#ifndef MTV23X_PRIV_H
#define MTV23X_PRIV_H

#include <media/dvb_frontend.h>
#include "mtv23x.h"
#include <linux/regmap.h>

/* Do not modify the order and value! */
enum E_RTV_SERVICE_TYPE {
	RTV_SERVICE_INVALID = -1,
	RTV_SERVICE_UHF_ISDBT_1seg = 0, /* ISDB-T 1seg */
	RTV_SERVICE_UHF_ISDBT_13seg = 1, /* ISDB-T fullseg */
	RTV_SERVICE_VHF_ISDBTmm_1seg = 2, /* ISDB-Tmm 1seg */
	RTV_SERVICE_VHF_ISDBTmm_13seg = 3, /* ISDB-Tmm 13seg */
	RTV_SERVICE_VHF_ISDBTsb_1seg = 4, /* ISDB-Tsb 1seg */
	RTV_SERVICE_VHF_ISDBTsb_3seg = 5, /* ISDB-Tsb 3seg */
	RTV_SERVICE_DVBT = 6, /* DVB-T */
	MAX_NUM_RTV_SERVICE
};


struct mtv23x_dev {
	struct i2c_client *client;
	struct regmap *regmap;
	struct dvb_frontend fe;
	u16 i2c_wr_max;
	
	int ts_mode;
	int clk_freq;//u:KHZ

	bool rtv_1seglpmode;

	enum E_RTV_SERVICE_TYPE svc_type;
};

#define TOP_PAGE	0x00
#define HOST_PAGE	0x00
#define OFDM_PAGE	0x01
#define SHAD_PAGE	0x02
#define FEC_PAGE	0x03
#define DATA_PAGE	0x04
#define FEC2_PAGE	0x06
#define LPOFDM_PAGE	0x07
#define SPI_CTRL_PAGE	0x0E
#define RF_PAGE		0x0F

#define MAP_SEL_REG	0x03

enum E_RTV_BANDWIDTH_TYPE {
	RTV_BW_MODE_5MHZ = 0, /* DVB_T */
	RTV_BW_MODE_6MHZ, /* DVB_T, FULLSEG, ISDB-Tmm */
	RTV_BW_MODE_7MHZ, /* DVB_T, FULLSEG */
	RTV_BW_MODE_8MHZ, /* DVB_T, FULLSEG */
	RTV_BW_MODE_430KHZ, /* 1SEG at 6MHz BW */
	RTV_BW_MODE_500KHZ, /* 1SEG at 7MHz BW */
	RTV_BW_MODE_571KHZ, /* 1SEG at 8MHz BW */
	RTV_BW_MODE_857KHZ, /* DAB */
	RTV_BW_MODE_1290KHZ, /* 3SEG */
	MAX_NUM_RTV_BW_MODE_TYPE
};



struct RTV_REG_INIT_INFO {
	u8	bReg;
	u8	bVal;
};

struct RTV_ADC_CFG_INFO {
	u8 bData2A;
	u8 bData6E;
	u8 bData70;
	u8 bData71;
	u8 bData75;
	u32 dwTNCO;
	u32 dwPNCO;
	u32 dwCFREQGAIN;
	u16 dwGAIN;
};

#define RTV_ISDBT_OFDM_LOCK_MASK		0x1
#define RTV_ISDBT_TMCC_LOCK_MASK		0x2
#define RTV_ISDBT_CHANNEL_LOCK_OK	\
	(RTV_ISDBT_OFDM_LOCK_MASK|RTV_ISDBT_TMCC_LOCK_MASK)


#define RSSI_UINT(val)	(s32)((val)*10)

#define RSSI_RFAGC_VAL(rfagc, coeffi)\
	((rfagc) * RSSI_UINT(coeffi))

#define RSSI_GVBB_VAL(gvbb, coeffi)\
	((gvbb) * RSSI_UINT(coeffi))

#endif

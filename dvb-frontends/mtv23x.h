#ifndef MTV23X_H
#define MTV23X_H
#include <linux/dvb/frontend.h>


/*TS mode*/
//#define RTV_TSIF_FORMAT_0 /* Serial: EN_high, CLK_rising */
//#define RTV_TSIF_FORMAT_1 /*   Serial: EN_high, CLK_falling */ //
//#define RTV_TSIF_FORMAT_2 /* Serial: EN_low, CLK_rising */
//#define RTV_TSIF_FORMAT_3 /* Serial: EN_low, CLK_falling */
//#define RTV_TSIF_FORMAT_4 /* Serial: EN_high, CLK_rising + 1CLK add */
//#define RTV_TSIF_FORMAT_5 /* Serial: EN_high, CLK_falling + 1CLK add */
//#define RTV_TSIF_FORMAT_6 /* Parallel: EN_high, CLK_rising */
//#define RTV_TSIF_FORMAT_7 /* Parallel: EN_high, CLK_falling */

struct mtv23x_config{
	int ts_mode;  // 0:serial 1:parallel
	int clk_freq;  //32000khz , 19200khz
	u16 i2c_wr_max;

	struct dvb_frontend **fe;
};

#endif

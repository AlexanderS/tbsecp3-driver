#ifndef MN88436_H
#define MN88436_H
#include <linux/dvb/frontend.h>

struct mn88436_config{
#define DMD_E_TSOUT_PARALLEL_FIXED_CLOCK 0
#define DMD_E_TSOUT_SERIAL_VARIABLE_CLOCK 1
	int ts_mode;

	u16 i2c_wr_max;
	
	struct dvb_frontend **fe;
};
#endif

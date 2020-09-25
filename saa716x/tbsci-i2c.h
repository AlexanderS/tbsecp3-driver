/*
	TurboSight TBS CI I2C over GPIO driver
    	Copyright (C) 2011 Konstantin Dimitrov <kosio.dimitrov@gmail.com>

    	Copyright (C) 2011 TurboSight.com
*/

#ifndef TBSCI_I2C_H
#define TBSCI_I2C_H

#include <linux/i2c-algo-bit.h>

//#define I2C_HW_B_SAA716x		0x12

struct tbsci_i2c_state {
	struct i2c_adapter adap;
	struct i2c_algo_bit_data algo;
	u8 wscl, wsda, rsda;
	u32 state;
	int nr;
};

extern int tbsci_i2c_probe(struct saa716x_adapter *adap, int tbsci_i2c_nr);
extern void tbsci_i2c_remove(struct saa716x_adapter *adap);

#endif

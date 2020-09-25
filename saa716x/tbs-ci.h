/*
	TurboSight TBS CI driver
    	Copyright (C) 2011 Konstantin Dimitrov <kosio.dimitrov@gmail.com>

    	Copyright (C) 2011 TurboSight.com
*/

#ifndef TBS_CI_H
#define TBS_CI_H

#include "saa716x_priv.h"
#include "saa716x_gpio.h"
#include "tbsci-i2c.h"

#include <media/dvb_ca_en50221.h>

extern int tbsci_read_attribute_mem(struct dvb_ca_en50221 *en50221, 
	int slot, int addr);
extern int tbsci_write_attribute_mem(struct dvb_ca_en50221 *en50221, 
	int slot, int addr, u8 data);
extern int tbsci_read_cam_control(struct dvb_ca_en50221 *en50221, 
	int slot, u8 addr);
extern int tbsci_write_cam_control(struct dvb_ca_en50221 *en50221, 
	int slot, u8 addr, u8 data);
extern int tbsci_slot_reset(struct dvb_ca_en50221 *en50221, 
	int slot);
extern int tbsci_slot_shutdown(struct dvb_ca_en50221 *en50221, 
	int slot);
extern int tbsci_slot_ts_enable(struct dvb_ca_en50221 *en50221, 
	int slot);
extern int tbsci_poll_slot_status(struct dvb_ca_en50221 *en50221, 
	int slot, int open);
extern int tbsci_init(struct saa716x_adapter *adap, int tbsci_nr,
	int tbsci_mode);
extern void tbsci_release(struct saa716x_adapter *adap);

#endif

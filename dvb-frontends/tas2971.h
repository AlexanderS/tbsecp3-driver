/*
    Tmax TAS2101 - DVBS/S2 Satellite demod/tuner driver

    Copyright (C) 2014 Luis Alves <ljalvs@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef TAS2971_H
#define TAS2971_H

#include <linux/kconfig.h>
#include <linux/dvb/frontend.h>
#include "tas2101.h"

#if IS_REACHABLE(CONFIG_DVB_TAS2971)
extern struct dvb_frontend *tas2971_attach(
	const struct tas2101_config *cfg,
	struct i2c_adapter *i2c);
#else
static inline struct dvb_frontend *tas2971_attach(
	const struct tas2101_config *cfg,
	struct i2c_adapter *i2c)
{
	dev_warn(&i2c->dev, "%s: driver disabled by Kconfig\n", __func__);
	return NULL;
}

#endif

#endif /* TAS2101_H */

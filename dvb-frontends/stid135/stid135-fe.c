/*
 * Driver for the STm STiD135 DVB-S/S2/S2X demodulator.
 *
 * Copyright (C) CrazyCat <crazycat69@narod.ru>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 only, as published by the Free Software Foundation.
 *
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA
 * Or, point your browser to http://www.gnu.org/copyleft/gpl.html
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/firmware.h>
#include <linux/i2c.h>
#include <linux/version.h>
#include <asm/div64.h>

#include <media/dvb_frontend.h>
#include "stid135.h"
#include "i2c.h"
#include "stid135_drv.h"

LIST_HEAD(stvlist);

static int mode;
module_param(mode, int, 0444);
MODULE_PARM_DESC(mode,
		"Multi-switch mode: 0=quattro/quad 1=normal direct connection");

static unsigned int rfsource;
module_param(rfsource, int, 0644);
MODULE_PARM_DESC(rfsource, "RF source selection for direct connection mode (default:0 - auto)");

static unsigned int mc_auto;
module_param(mc_auto, int, 0644);
MODULE_PARM_DESC(mc_auto, "Enable auto modcode filtering depend from current C/N (default:0 - disabled)");

struct stv_base {
	struct list_head     stvlist;

	u8                   adr;
	struct i2c_adapter  *i2c;
	struct mutex         i2c_lock;
	struct mutex         status_lock;
	int                  count;
	u32                  extclk;
	u8                   ts_mode;

	int (*set_voltage)(struct i2c_adapter *i2c,
		enum fe_sec_voltage voltage, u8 rf_in);
	u8                   mode;

	fe_stid135_handle_t  handle;

	void (*write_properties) (struct i2c_adapter *i2c,u8 reg, u32 buf);
	void (*read_properties) (struct i2c_adapter *i2c,u8 reg, u32 *buf);
};

struct stv {
	struct stv_base     *base;
	struct dvb_frontend  fe;
	int                  nr;
	int                  rf_in;
	
	unsigned long        tune_time;
	struct fe_sat_signal_info signal_info;
};

I2C_RESULT I2cReadWrite(void *pI2CHost, I2C_MODE mode, u8 ChipAddress, u8 *Data, int NbData)
{
	struct stv_base     *base = (struct stv_base *)pI2CHost;
	struct i2c_msg msg = {.addr = ChipAddress>>1, .flags = 0,
			      .buf = Data, .len = NbData};
	int ret;
	

	if (!base) return I2C_ERR_HANDLE;

	if (mode == I2C_READ)
		msg.flags = I2C_M_RD;

	mutex_lock(&base->i2c_lock);
	ret = i2c_transfer(base->i2c, &msg, 1);
	mutex_unlock(&base->i2c_lock);

	return (ret == 1) ? I2C_ERR_NONE : I2C_ERR_ACK;
}

static int stid135_probe(struct stv *state)
{
	struct fe_stid135_init_param init_params;
	fe_lla_error_t err = FE_LLA_NO_ERROR;
	struct fe_stid135_internal_param *p_params;
	enum device_cut_id cut_id;
	int i;

	//dev_warn(&state->base->i2c->dev, "%s\n", FE_STiD135_GetRevision());

	strcpy(init_params.demod_name,"STiD135");
	init_params.pI2CHost		=	state->base;
	init_params.demod_i2c_adr   	=	state->base->adr ? state->base->adr<<1 : 0xd0;
	init_params.demod_ref_clk  	= 	state->base->extclk ? state->base->extclk : 27;
	init_params.internal_dcdc	=	FALSE;
	init_params.internal_ldo	=	TRUE; // LDO supply is internal on Oxford valid board
	init_params.rf_input_type	=	0xF; // Single ended RF input on Oxford valid board rev2
	init_params.roll_off		=  	FE_SAT_35; // NYQUIST Filter value (used for DVBS1/DSS, DVBS2 is automatic)
	init_params.tuner_iq_inversion	=	FE_SAT_IQ_NORMAL;
	
	err = fe_stid135_init(&init_params,&state->base->handle);
	
	if (err != FE_LLA_NO_ERROR) {
		dev_err(&state->base->i2c->dev, "%s: fe_stid135_init error %d !\n", __func__, err);
		return -EINVAL;
	}

	p_params = state->base->handle;

	err = fe_stid135_get_cut_id(state->base->handle,&cut_id);
	switch(cut_id)
	{
	case STID135_CUT1_0:
		dev_warn(&state->base->i2c->dev, "%s: cut 1.0\n", __func__);
		break;
	case STID135_CUT1_1:
		dev_warn(&state->base->i2c->dev, "%s: cut 1.1\n", __func__);
		break;
	case STID135_CUT1_X:
		dev_warn(&state->base->i2c->dev, "%s: cut 1.x\n", __func__);
		break;
	case STID135_CUT2_0:
		dev_warn(&state->base->i2c->dev, "%s: cut 2.0 \n", __func__);
		break;
	case STID135_CUT2_1:
		dev_warn(&state->base->i2c->dev, "%s: cut 2.1 \n", __func__);
		break;
	case STID135_CUT2_X_UNFUSED:
		dev_warn(&state->base->i2c->dev, "%s: cut 2.x \n", __func__);
		break;
	default:
		dev_warn(&state->base->i2c->dev, "%s: cut ? \n", __func__);
		return -EINVAL;
	}

	if (state->base->ts_mode == TS_STFE) {
		dev_warn(&state->base->i2c->dev, "%s: 8xTS to STFE mode init.\n", __func__);
		for(i=0;i<8;i++) {
			err |= fe_stid135_set_ts_parallel_serial(state->base->handle, i+1, FE_TS_PARALLEL_ON_TSOUT_0);
			err |= fe_stid135_set_maxllr_rate(state->base->handle, i+1, 260);
		}
		err |= fe_stid135_enable_stfe(state->base->handle,FE_STFE_OUTPUT0);
		err |= fe_stid135_set_stfe(state->base->handle, FE_STFE_TAGGING_MERGING_MODE, FE_STFE_INPUT1 |
						FE_STFE_INPUT2 |FE_STFE_INPUT3 |FE_STFE_INPUT4| FE_STFE_INPUT5 |
						FE_STFE_INPUT6 |FE_STFE_INPUT7 |FE_STFE_INPUT8 ,FE_STFE_OUTPUT0, 0xDE);
	} else if (state->base->ts_mode == TS_8SER) {
		dev_warn(&state->base->i2c->dev, "%s: 8xTS serial mode init.\n", __func__);
		for(i=0;i<8;i++) {
			err |= fe_stid135_set_ts_parallel_serial(state->base->handle, i+1, FE_TS_SERIAL_CONT_CLOCK);
			err |= fe_stid135_set_maxllr_rate(state->base->handle, i+1, 90);
		}
	} else {
		dev_warn(&state->base->i2c->dev, "%s: 2xTS parallel mode init.\n", __func__);
		err |= fe_stid135_set_ts_parallel_serial(state->base->handle, FE_SAT_DEMOD_3, FE_TS_PARALLEL_PUNCT_CLOCK);
		err |= fe_stid135_set_maxllr_rate(state->base->handle, FE_SAT_DEMOD_3, 260);
		err |= fe_stid135_set_ts_parallel_serial(state->base->handle, FE_SAT_DEMOD_1, FE_TS_PARALLEL_PUNCT_CLOCK);
		err |= fe_stid135_set_maxllr_rate(state->base->handle, FE_SAT_DEMOD_1, 260);
	}

	if (state->base->mode == 0) {
		dev_warn(&state->base->i2c->dev, "%s: multiswitch mode init.\n", __func__);
		err |= fe_stid135_tuner_enable(p_params->handle_demod, AFE_TUNER1);
		err |= fe_stid135_tuner_enable(p_params->handle_demod, AFE_TUNER2);
		err |= fe_stid135_tuner_enable(p_params->handle_demod, AFE_TUNER3);
		err |= fe_stid135_tuner_enable(p_params->handle_demod, AFE_TUNER4);
		err |= fe_stid135_diseqc_init(state->base->handle,AFE_TUNER1, FE_SAT_DISEQC_2_3_PWM);
		err |= fe_stid135_diseqc_init(state->base->handle,AFE_TUNER2, FE_SAT_22KHZ_Continues);
		err |= fe_stid135_diseqc_init(state->base->handle,AFE_TUNER3, FE_SAT_DISEQC_2_3_PWM);
		err |= fe_stid135_diseqc_init(state->base->handle,AFE_TUNER4, FE_SAT_22KHZ_Continues);
		if (state->base->set_voltage) {		  
			state->base->set_voltage(state->base->i2c, SEC_VOLTAGE_13, 0);
			state->base->set_voltage(state->base->i2c, SEC_VOLTAGE_13, 1);
			state->base->set_voltage(state->base->i2c, SEC_VOLTAGE_18, 2);
			state->base->set_voltage(state->base->i2c, SEC_VOLTAGE_18, 3);
		}
	}

	if (err != FE_LLA_NO_ERROR)
		dev_err(&state->base->i2c->dev, "%s: setup error %d !\n", __func__, err);

	return err != FE_LLA_NO_ERROR ? -1 : 0;
}

static int stid135_init(struct dvb_frontend *fe)
{
	struct stv *state = fe->demodulator_priv;
	fe_lla_error_t err = FE_LLA_NO_ERROR;
	struct fe_stid135_internal_param *p_params = state->base->handle;

	if (state->base->mode == 0)
		return 0;

	dev_dbg(&state->base->i2c->dev, "%s: demod %d + tuner %d\n", __func__, state->nr, state->rf_in);

	mutex_lock(&state->base->status_lock);
	err |= fe_stid135_tuner_enable(p_params->handle_demod, state->rf_in + 1);
	err |= fe_stid135_diseqc_init(state->base->handle, state->rf_in + 1, FE_SAT_DISEQC_2_3_PWM);
	err |= fe_stid135_set_rfmux_path(p_params->handle_demod, state->nr + 1, state->rf_in + 1);
	mutex_unlock(&state->base->status_lock);

	if (err != FE_LLA_NO_ERROR)
		dev_err(&state->base->i2c->dev, "%s: enable tuner %d + demod %d error %d !\n", __func__, state->rf_in, state->nr, err);

	return err != FE_LLA_NO_ERROR ? -1 : 0;
}

static void stid135_release(struct dvb_frontend *fe)
{
	struct stv *state = fe->demodulator_priv;

	dev_dbg(&state->base->i2c->dev, "%s: demod %d\n", __func__, state->nr);
	state->base->count--;
	if (state->base->count == 0) {
		if (state->base->handle)
			FE_STiD135_Term (state->base->handle);
		list_del(&state->base->stvlist);
		kfree(state->base);
	}
	kfree(state);
}

static int stid135_set_parameters(struct dvb_frontend *fe)
{
	struct stv *state = fe->demodulator_priv;
	struct dtv_frontend_properties *p = &fe->dtv_property_cache;
	fe_lla_error_t err = FE_LLA_NO_ERROR;
	struct fe_stid135_internal_param *p_params = state->base->handle;
	struct fe_sat_search_params search_params;
	struct fe_sat_search_result search_results;
	u32 pls_mode, pls_code;
	s32 rf_power;

	dev_warn(&state->base->i2c->dev,
			"delivery_system=%u modulation=%u frequency=%u symbol_rate=%u inversion=%u stream_id=%d\n",
			p->delivery_system, p->modulation, p->frequency,
			p->symbol_rate, p->inversion, p->stream_id);

	/* Search parameters */
	search_params.search_algo 	= FE_SAT_COLD_START;
	search_params.frequency 	= p->frequency*1000;
	search_params.symbol_rate 	= p->symbol_rate;
	search_params.modulation	= FE_SAT_MOD_UNKNOWN;
	search_params.modcode		= FE_SAT_DUMMY_PLF;
	search_params.search_range	= 10000000;
	search_params.puncture_rate	= FE_SAT_PR_UNKNOWN;
	switch (p->delivery_system)
	{
	  case SYS_DSS:
		search_params.standard		= FE_SAT_SEARCH_DSS;
		break;
	  case SYS_DVBS:
		search_params.standard		= FE_SAT_SEARCH_DVBS1;
		break;
	  case SYS_DVBS2:
		search_params.standard		= FE_SAT_SEARCH_DVBS2;
		break;
	  default:
		search_params.standard		= FE_SAT_AUTO_SEARCH;
	}
	search_params.iq_inversion	= FE_SAT_IQ_AUTO;
	search_params.tuner_index_jump	= 0; // ok with narrow band signal

	err = FE_STiD135_GetLoFreqHz(state->base->handle, &(search_params.lo_frequency));
	search_params.lo_frequency *= 1000000;

	mutex_lock(&state->base->status_lock);

	dev_dbg(&state->base->i2c->dev, "%s: demod %d + tuner %d\n", __func__, state->nr, state->rf_in);
	err |= fe_stid135_set_rfmux_path(p_params->handle_demod, state->nr + 1, state->rf_in + 1);

	if (p->stream_id == NO_STREAM_ID_FILTER) {
		pls_mode = 0;
		pls_code = 1;
	}
	else {
		pls_mode = (p->stream_id>>26) & 0x3;
		pls_code = (p->stream_id>>8) & 0x3FFFF;
		if (pls_mode == 0 && pls_code == 0)
			pls_code = 1;
	}
	
	if (p->scrambling_sequence_index) {
		pls_mode = 1;
		pls_code = 0;
	}

	/* Set PLS before search */
	dev_warn(&state->base->i2c->dev, "%s: set pls_mode %d, pls_code %d !\n", __func__, pls_mode, pls_code);
	err |= fe_stid135_set_pls(state->base->handle, state->nr + 1, pls_mode, pls_code);
	
	if (err != FE_LLA_NO_ERROR)
		dev_err(&state->base->i2c->dev, "%s: fe_stid135_set_pls error %d !\n", __func__, err);

	err |= fe_stid135_reset_modcodes_filter(state->base->handle, state->nr + 1);
	if (err != FE_LLA_NO_ERROR)
		dev_err(&state->base->i2c->dev, "%s: fe_stid135_reset_modcodes_filter error %d !\n", __func__, err);

	err |= fe_stid135_search(state->base->handle, state->nr + 1, &search_params, &search_results, 0);
	mutex_unlock(&state->base->status_lock);

	if (err != FE_LLA_NO_ERROR)
	{
		dev_err(&state->base->i2c->dev, "%s: fe_stid135_search error %d !\n", __func__, err);
		return -1;
	}

	if (search_results.locked)
		dev_warn(&state->base->i2c->dev, "%s: locked !\n", __func__);
	else {
		err |= fe_stid135_get_band_power_demod_not_locked(state->base->handle, state->nr + 1, &rf_power);
		dev_warn(&state->base->i2c->dev, "%s: not locked, band rf_power %d dBm !\n", __func__, rf_power / 1000);
	}

	/* Set ISI before search */
	if (p->stream_id != NO_STREAM_ID_FILTER) {
		dev_warn(&state->base->i2c->dev, "%s: set ISI %d !\n", __func__, p->stream_id & 0xFF);
		err |= fe_stid135_set_mis_filtering(state->base->handle, state->nr + 1, TRUE, p->stream_id & 0xFF, 0xFF);
	} else {
		dev_warn(&state->base->i2c->dev, "%s: disable ISI filtering !\n", __func__);
		err |= fe_stid135_set_mis_filtering(state->base->handle, state->nr + 1, FALSE, 0, 0xFF);				
	}
	if (err != FE_LLA_NO_ERROR)
		dev_err(&state->base->i2c->dev, "%s: fe_stid135_set_mis_filtering error %d !\n", __func__, err);

	return err != FE_LLA_NO_ERROR ? -1 : 0;

}

static int stid135_get_frontend(struct dvb_frontend *fe, struct dtv_frontend_properties *p)
{
	struct stv *state = fe->demodulator_priv;

	if (!state->signal_info.locked)
		return 0;

	switch (state->signal_info.standard) {
	case FE_SAT_DSS_STANDARD:
		p->delivery_system = SYS_DSS;
		break;
	case FE_SAT_DVBS2_STANDARD:
		p->delivery_system = SYS_DVBS2;
		break;
	case FE_SAT_DVBS1_STANDARD:
	default:
		p->delivery_system = SYS_DVBS;
    	}

	switch (state->signal_info.modulation) {
	case FE_SAT_MOD_8PSK:
		p->modulation = PSK_8;
		break;
	case FE_SAT_MOD_16APSK:
		p->modulation = APSK_16;
		break;
#ifdef APSK_64
	case FE_SAT_MOD_64APSK:
		p->modulation = APSK_64;
		break;
#endif
#ifdef APSK_128
	case FE_SAT_MOD_128APSK:
		p->modulation = APSK_128;
		break;
#endif
#ifdef APSK_256
	case FE_SAT_MOD_256APSK:
		p->modulation = APSK_256;
		break;
#endif
#ifdef APSK_1024
	case FE_SAT_MOD_1024APSK:
		p->modulation = APSK_1024;
		break;
#endif
#ifdef APSK_8L
	case FE_SAT_MOD_8PSK_L:
		p->modulation = APSK_8L;
		break;
#endif
#ifdef APSK_16L
	case FE_SAT_MOD_16APSK_L:
		p->modulation = APSK_16L;
		break;
#endif
#ifdef APSK_64L
	case FE_SAT_MOD_64APSK_L:
		p->modulation = APSK_64L;
		break;
#endif
#ifdef APSK_256L
	case FE_SAT_MOD_256APSK_L:
		p->modulation = APSK_256L;
		break;
#endif
	case FE_SAT_MOD_QPSK:
	default:
		p->modulation = QPSK;
    	}

    	switch (state->signal_info.roll_off) {
#ifdef ROLLOFF_5
	  case FE_SAT_05:
		p->rolloff = ROLLOFF_5;
		break;
#endif
#ifdef ROLLOFF_10
	  case FE_SAT_10:
		p->rolloff = ROLLOFF_10;
		break;
#endif
#ifdef ROLLOFF_15
	  case FE_SAT_15:
		p->rolloff = ROLLOFF_15;
		break;
#endif
	  case FE_SAT_20:
		p->rolloff = ROLLOFF_20;
		break;
	  case FE_SAT_25:
		p->rolloff = ROLLOFF_25;
		break;
	  case FE_SAT_35:
		p->rolloff = ROLLOFF_35;
		break;
	  default:
		p->rolloff = ROLLOFF_AUTO;
	}

	p->inversion = state->signal_info.spectrum == FE_SAT_IQ_SWAPPED ? INVERSION_ON : INVERSION_OFF;
	if (p->delivery_system == SYS_DVBS2) {
		enum fe_code_rate modcod2fec[0x20] = {
			FEC_NONE,
#ifdef FEC_1_4
			FEC_1_4,
#endif
#ifdef FEC_1_3
			FEC_1_3,
#endif
			FEC_2_5,
			FEC_1_2, FEC_3_5, FEC_2_3, FEC_3_4,
			FEC_4_5, FEC_5_6, FEC_8_9, FEC_9_10,
			FEC_3_5, FEC_2_3, FEC_3_4, FEC_5_6,
			FEC_8_9, FEC_9_10, FEC_2_3, FEC_3_4,
			FEC_4_5, FEC_5_6, FEC_8_9, FEC_9_10,
			FEC_3_4, FEC_4_5, FEC_5_6, FEC_8_9,
			FEC_9_10
		};
		if (state->signal_info.modcode < FE_SAT_MODCODE_UNKNOWN)
			p->fec_inner = modcod2fec[state->signal_info.modcode];
		else
			p->fec_inner = FEC_AUTO;
		p->pilot = state->signal_info.pilots == FE_SAT_PILOTS_ON ? PILOT_ON : PILOT_OFF;
	}
	else {
		switch (state->signal_info.puncture_rate) {
		case FE_SAT_PR_1_2:
			p->fec_inner = FEC_1_2;
			break;
		case FE_SAT_PR_2_3:
			p->fec_inner = FEC_2_3;
			break;
		case FE_SAT_PR_3_4:
			p->fec_inner = FEC_3_4;
			break;
		case FE_SAT_PR_5_6:
			p->fec_inner = FEC_5_6;
			break;
		case FE_SAT_PR_6_7:
			p->fec_inner = FEC_6_7;
			break;
		case FE_SAT_PR_7_8:
			p->fec_inner = FEC_7_8;
			break;
		default:
			p->fec_inner = FEC_NONE;
		}
	}
    	return 0;
}

static int stid135_read_status(struct dvb_frontend *fe, enum fe_status *status)
{
	struct stv *state = fe->demodulator_priv;
	struct dtv_frontend_properties *p = &fe->dtv_property_cache;
	fe_lla_error_t err = FE_LLA_NO_ERROR;

	*status = 0;
	p->strength.len = 1;
	p->strength.stat[0].scale = FE_SCALE_NOT_AVAILABLE;
	p->cnr.len = 1;
	p->cnr.stat[0].scale = FE_SCALE_NOT_AVAILABLE;
	p->pre_bit_error.len =1;
	p->pre_bit_error.stat[0].scale = FE_SCALE_NOT_AVAILABLE;
	p->pre_bit_count.len =1;
	p->pre_bit_count.stat[0].scale = FE_SCALE_NOT_AVAILABLE;

	mutex_lock(&state->base->status_lock);
	err = fe_stid135_get_lock_status(state->base->handle, state->nr + 1, &state->signal_info.locked);
	mutex_unlock(&state->base->status_lock);

	if (err != FE_LLA_NO_ERROR) {
		dev_warn(&state->base->i2c->dev, "%s: fe_stid135_get_lock_status error %d !\n", __func__, err);
		return 0;
	}
	
	if (!state->signal_info.locked) {
		mutex_lock(&state->base->status_lock);
		err = fe_stid135_get_band_power_demod_not_locked(state->base->handle, state->nr + 1, &state->signal_info.power);
		mutex_unlock(&state->base->status_lock);

		if (err != FE_LLA_NO_ERROR) {
			dev_warn(&state->base->i2c->dev, "%s: fe_stid135_get_band_power_demod_not_locked error %d !\n", __func__, err);
			return 0;
		}

		*status |= FE_HAS_SIGNAL;

		p->strength.len = 2;
		p->strength.stat[0].scale = FE_SCALE_DECIBEL;
		p->strength.stat[0].svalue = state->signal_info.power;
		
		p->strength.stat[1].scale = FE_SCALE_RELATIVE;
		p->strength.stat[1].uvalue = (100 + state->signal_info.power/1000) * 656;
		return 0;
	}
	  
	mutex_lock(&state->base->status_lock);
	err = fe_stid135_get_signal_info(state->base->handle, state->nr + 1, &state->signal_info, 0);
	mutex_unlock(&state->base->status_lock);

	if (err != FE_LLA_NO_ERROR) {
		dev_warn(&state->base->i2c->dev, "%s: fe_stid135_get_signal_info error %d !\n", __func__, err);
		return 0;
	}

	*status |= FE_HAS_SIGNAL | FE_HAS_CARRIER
		    | FE_HAS_VITERBI | FE_HAS_SYNC | FE_HAS_LOCK;

	p->strength.len = 2;
	p->strength.stat[0].scale = FE_SCALE_DECIBEL;
	p->strength.stat[0].svalue = state->signal_info.power;
	
	p->strength.stat[1].scale = FE_SCALE_RELATIVE;
	p->strength.stat[1].uvalue = (100 + state->signal_info.power/1000) * 656;

	p->cnr.len = 2;
	p->cnr.stat[0].scale = FE_SCALE_DECIBEL;
	p->cnr.stat[0].svalue = state->signal_info.C_N * 100;

	p->cnr.stat[1].scale = FE_SCALE_RELATIVE;
	p->cnr.stat[1].uvalue = state->signal_info.C_N * 328;
	if (p->cnr.stat[1].uvalue > 0xffff)
		p->cnr.stat[1].uvalue = 0xffff;

	p->post_bit_error.len = 1;
	p->post_bit_error.stat[0].scale = FE_SCALE_COUNTER;
	p->post_bit_error.stat[0].uvalue = state->signal_info.ber;
	
	if (mc_auto && state->signal_info.standard == FE_SAT_DVBS2_STANDARD) {
		mutex_lock(&state->base->status_lock);
		err = fe_stid135_filter_forbidden_modcodes(state->base->handle, state->nr + 1, state->signal_info.C_N * 10);
		mutex_unlock(&state->base->status_lock);

		if (err != FE_LLA_NO_ERROR)
			dev_warn(&state->base->i2c->dev, "%s: fe_stid135_filter_forbidden_modcodes error %d !\n", __func__, err);
	}

	return 0;
}

static int stid135_tune(struct dvb_frontend *fe, bool re_tune,
		unsigned int mode_flags,
		unsigned int *delay, enum fe_status *status)
{
	struct stv *state = fe->demodulator_priv;
	int r;

	if (re_tune) {
		r = stid135_set_parameters(fe);
		if (r)
			return r;
		state->tune_time = jiffies;
	}

	r = stid135_read_status(fe, status);
	if (r)
		return r;

	if (*status & FE_HAS_LOCK)
		return 0;

	*delay = HZ;

	return 0;
}


static enum dvbfe_algo stid135_get_algo(struct dvb_frontend *fe)
{
	return DVBFE_ALGO_HW;
}

static int stid135_set_voltage(struct dvb_frontend *fe, enum fe_sec_voltage voltage)
{
	struct stv *state = fe->demodulator_priv;

	if (state->base->mode == 0)
	{
		if (voltage == SEC_VOLTAGE_18)
			state->rf_in |= 2;
		else
			state->rf_in &= ~2;
		return 0;
	}

	if (state->base->set_voltage)
		state->base->set_voltage(state->base->i2c, voltage, state->rf_in);

	return 0;
}

static int stid135_set_tone(struct dvb_frontend *fe, enum fe_sec_tone_mode tone)
{
	struct stv *state = fe->demodulator_priv;
	fe_lla_error_t err = FE_LLA_NO_ERROR;

	if (state->base->mode == 0)
	{
		if (tone == SEC_TONE_ON)
			state->rf_in |= 1;
		else
			state->rf_in &= ~1;

		return 0;
	}

	mutex_lock(&state->base->status_lock);
	err = fe_stid135_set_22khz_cont(state->base->handle,state->rf_in + 1, tone == SEC_TONE_ON);
	mutex_unlock(&state->base->status_lock);

	if (err != FE_LLA_NO_ERROR)
		dev_err(&state->base->i2c->dev, "%s: fe_stid135_set_22khz_cont error %d !\n", __func__, err);

	return err != FE_LLA_NO_ERROR ? -1 : 0;
}

static int stid135_send_master_cmd(struct dvb_frontend *fe,
			   struct dvb_diseqc_master_cmd *cmd)
{
	struct stv *state = fe->demodulator_priv;
	fe_lla_error_t err = FE_LLA_NO_ERROR;

	if (state->base->mode == 0)
		return 0;

	mutex_lock(&state->base->status_lock);
	err |= fe_stid135_diseqc_init(state->base->handle, state->rf_in + 1, FE_SAT_DISEQC_2_3_PWM);
	err |= fe_stid135_diseqc_send(state->base->handle, state->rf_in + 1, cmd->msg, cmd->msg_len);
	mutex_unlock(&state->base->status_lock);

	if (err != FE_LLA_NO_ERROR)
		dev_err(&state->base->i2c->dev, "%s: fe_stid135_diseqc_send error %d !\n", __func__, err);

	return err != FE_LLA_NO_ERROR ? -1 : 0;
}

static int stid135_recv_slave_reply(struct dvb_frontend *fe,
			    struct dvb_diseqc_slave_reply *reply)
{
	struct stv *state = fe->demodulator_priv;
	fe_lla_error_t err = FE_LLA_NO_ERROR;

	if (state->base->mode == 0)
		return 0;

	mutex_lock(&state->base->status_lock);
	err = fe_stid135_diseqc_receive(state->base->handle, reply->msg, &reply->msg_len);
	mutex_unlock(&state->base->status_lock);

	if (err != FE_LLA_NO_ERROR)
		dev_err(&state->base->i2c->dev, "%s: fe_stid135_diseqc_receive error %d !\n", __func__, err);

	return err != FE_LLA_NO_ERROR ? -1 : 0;
}

static int stid135_send_burst(struct dvb_frontend *fe, enum fe_sec_mini_cmd burst)
{
	struct stv *state = fe->demodulator_priv;
	fe_lla_error_t err = FE_LLA_NO_ERROR;

	if (state->base->mode == 0)
		return 0;

	return err != FE_LLA_NO_ERROR ? -1 : 0;
}

static int stid135_sleep(struct dvb_frontend *fe)
{
	struct stv *state = fe->demodulator_priv;
	fe_lla_error_t err = FE_LLA_NO_ERROR;
	struct fe_stid135_internal_param *p_params = state->base->handle;;

	if (state->base->mode == 0)
		return 0;

	dev_dbg(&state->base->i2c->dev, "%s: tuner %d\n", __func__, state->rf_in);

	err = FE_STiD135_TunerStandby(p_params->handle_demod, state->rf_in + 1, 0);

	if (err != FE_LLA_NO_ERROR)
		dev_warn(&state->base->i2c->dev, "%s: STiD135 standby tuner %d failed!\n", __func__, state->rf_in);

	return err != FE_LLA_NO_ERROR ? -1 : 0;
}

static int stid135_read_signal_strength(struct dvb_frontend *fe, u16 *strength)
{
	struct dtv_frontend_properties *p = &fe->dtv_property_cache;
	int i;

	*strength = 0;
	for (i=0; i < p->strength.len; i++) {
		if (p->strength.stat[i].scale == FE_SCALE_RELATIVE)
			*strength = (u16)p->strength.stat[i].uvalue;
		else if (p->strength.stat[i].scale == FE_SCALE_DECIBEL)
			*strength = ((100000 + (s32)p->strength.stat[i].svalue)/1000) * 656;
	}

	return 0;
}

static int stid135_read_snr(struct dvb_frontend *fe, u16 *snr)
{
	struct dtv_frontend_properties *p = &fe->dtv_property_cache;
	int i;

	*snr = 0;
	for (i=0; i < p->cnr.len; i++)
		if (p->cnr.stat[i].scale == FE_SCALE_RELATIVE)
		  *snr = (u16)p->cnr.stat[i].uvalue;
	return 0;
}

static int stid135_read_ber(struct dvb_frontend *fe, u32 *ber)
{
	struct dtv_frontend_properties *p = &fe->dtv_property_cache;
	int i;

	*ber = 1;
	for (i=0; i < p->post_bit_error.len; i++)
		if ( p->post_bit_error.stat[0].scale == FE_SCALE_COUNTER )	  
			*ber = (u32)p->post_bit_error.stat[0].uvalue;

	return 0;
}

static int stid135_read_ucblocks(struct dvb_frontend *fe, u32 *ucblocks)
{
	*ucblocks = 0;
	return 0;
}

#ifndef TBS_STANDALONE
static void spi_read(struct dvb_frontend *fe, struct ecp3_info *ecp3inf)
{
	struct stv *state = fe->demodulator_priv;
	struct i2c_adapter *adapter = state->base->i2c;

	if (state->base->read_properties)
		state->base->read_properties(adapter,ecp3inf->reg, &(ecp3inf->data));
	return ;
}

static void spi_write(struct dvb_frontend *fe,struct ecp3_info *ecp3inf)
{
	struct stv *state = fe->demodulator_priv;
	struct i2c_adapter *adapter = state->base->i2c;

	if (state->base->write_properties)
		state->base->write_properties(adapter,ecp3inf->reg, ecp3inf->data);
	return ;
}
#endif

static struct dvb_frontend_ops stid135_ops = {
	.delsys = { SYS_DVBS, SYS_DVBS2, SYS_DSS },
	.info = {
		.name			= "STiD135 Multistandard",
#ifdef MHz
		.frequency_min_hz	 = 950 * MHz,
		.frequency_max_hz 	= 2150 * MHz,
#else
		.frequency_min		 = 950000,
		.frequency_max 		= 2150000,
#endif
		.symbol_rate_min	= 100000,
		.symbol_rate_max	= 520000000,
		.caps			= FE_CAN_INVERSION_AUTO |
					  FE_CAN_FEC_AUTO       |
					  FE_CAN_QPSK           |
					  FE_CAN_2G_MODULATION  |
					  FE_CAN_MULTISTREAM
	},
	.init				= stid135_init,
	.sleep				= stid135_sleep,
	.release                        = stid135_release,
	.get_frontend_algo              = stid135_get_algo,
	.get_frontend                   = stid135_get_frontend,
	.tune                           = stid135_tune,
	.set_tone			= stid135_set_tone,
	.set_voltage			= stid135_set_voltage,

	.diseqc_send_master_cmd		= stid135_send_master_cmd,
	.diseqc_send_burst		= stid135_send_burst,
	.diseqc_recv_slave_reply	= stid135_recv_slave_reply,

	.read_status			= stid135_read_status,
	.read_signal_strength		= stid135_read_signal_strength,
	.read_snr			= stid135_read_snr,
	.read_ber			= stid135_read_ber,
	.read_ucblocks			= stid135_read_ucblocks,
#ifndef TBS_STANDALONE
	.spi_read			= spi_read,
	.spi_write			= spi_write,
#endif
};

static struct stv_base *match_base(struct i2c_adapter  *i2c, u8 adr)
{
	struct stv_base *p;

	list_for_each_entry(p, &stvlist, stvlist)
		if (p->i2c == i2c && p->adr == adr)
			return p;
	return NULL;
}

struct dvb_frontend *stid135_attach(struct i2c_adapter *i2c,
				    struct stid135_cfg *cfg,
				    int nr, int rf_in)
{
	struct stv *state;
	struct stv_base *base;

	state = kzalloc(sizeof(struct stv), GFP_KERNEL);
	if (!state)
		return NULL;

	base = match_base(i2c, cfg->adr);
	if (base) {
		base->count++;
		state->base = base;
	} else {
		base = kzalloc(sizeof(struct stv_base), GFP_KERNEL);
		if (!base)
			goto fail;
		base->i2c = i2c;
		base->adr = cfg->adr;
		base->count = 1;
		base->extclk = cfg->clk;
		base->ts_mode = cfg->ts_mode;
		base->set_voltage = cfg->set_voltage;
		base->mode = cfg->set_voltage ? mode : 1;
		base->write_properties = cfg->write_properties;
		base->read_properties = cfg->read_properties;

		mutex_init(&base->i2c_lock);
		mutex_init(&base->status_lock);

		state->base = base;
		if (stid135_probe(state) < 0) {
			dev_warn(&i2c->dev, "No demod found at adr %02X on %s\n",
				 cfg->adr, dev_name(&i2c->dev));
			kfree(base);
			goto fail;
		}
		list_add(&base->stvlist, &stvlist);
	}
	state->fe.ops               = stid135_ops;
	state->fe.demodulator_priv  = state;
	state->nr = nr;

	if (rfsource > 0 && rfsource < 5)
		rf_in = rfsource - 1;
	state->rf_in = base->mode ? rf_in : 0;

	dev_info(&i2c->dev, "%s demod found at adr %02X on %s\n",
		 state->fe.ops.info.name, cfg->adr, dev_name(&i2c->dev));

	return &state->fe;

fail:
	kfree(state);
	return NULL;
}
EXPORT_SYMBOL_GPL(stid135_attach);

MODULE_DESCRIPTION("STiD135 driver");
MODULE_AUTHOR("CrazyCat");
MODULE_LICENSE("GPL");

/*
 * Driver for the ST STV6120 tuner
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 only, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
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

#include "stv6120.h"

#define REG_N0		0
#define REG_N1_F0	1
#define REG_F1		2
#define REG_F2_ICP	3
#define REG_CF_PDIV	4
#define REG_CFHF	5
#define REG_CAL		6

struct SLookup {
	s16 Value;
	u16 RegValue;
};

static struct SLookup Gain_RFAGC_LookUp[] = {                
	/*Gain *100dB*/   /*reg*/
	{	7429		,	0        },  /*	74.61 dB	*/
	{	7368		,	18711	 },  /*	74.43 dB	*/
	{	7214		,	23432	 },  /*	73.36 dB	*/
	{	7090		,	25123	 },  /*	72.4  dB	*/
	{	6988		,	26305	 },  /*	71.47 dB	*/
	{	6897		,	27100	 },  /*	70.47 dB	*/
	{	6809		,	27741	 },  /*	69.5  dB	*/
	{	6728		,	28271	 },  /*	68.52 dB	*/
	{	6645		,	28737	 },  /*	67.52 dB	*/
	{	6571		,	29120	 },  /*	66.53 dB	*/
	{	6494		,	29504	 },  /*	65.54 dB	*/
	{	6416		,	29857	 },  /*	64.55 dB	*/
	{	6341		,	30180	 },  /*	63.55 dB	*/
	{	6263		,	30490	 },  /*	62.56 dB	*/
	{	6179		,	30815	 },  /*	61.57 dB	*/
	{	6101		,	31088	 },  /*	60.58 dB	*/
	{	6028		,	31345	 },  /*	59.57 dB	*/
	{	5956		,	31600	 },  /*	58.59 dB	*/
	{	5883		,	31840	 },  /*	57.6  dB	*/
	{	5801		,	32096	 },  /*	56.6  dB	*/
	{	5730		,	32320	 },  /*	55.59 dB	*/
	{	5659		,	32544	 },  /*	54.59 dB	*/
	{	5582		,	32752	 },  /*	53.61 dB	*/
	{	5498		,	32960	 },  /*	52.61 dB	*/
	{	5414		,	33184	 },  /*	51.61 dB	*/
	{	5340		,	33392	 },  /*	50.62 dB	*/
	{	5271		,	33584	 },  /*	49.61 dB	*/
	{	5198		,	33775	 },  /*	48.61 dB	*/
	{	5125		,	33967	 },  /*	47.62 dB	*/
	{	5048		,	34160	 },  /*	46.62 dB	*/
	{	4963		,	34352	 },  /*	45.62 dB	*/
	{	4884		,	34543	 },  /*	44.62 dB	*/
	{	4820		,	34719	 },  /*	43.62 dB	*/
	{	4740		,	34910	 },  /*	42.62 dB	*/
	{	4666		,	35103	 },  /*	41.62 dB	*/
	{	4582		,	35295	 },  /*	40.62 dB	*/
	{	4500		,	35488	 },  /*	39.62 dB	*/
	{	4426		,	35680	 },  /*	38.62 dB	*/
	{	4365		,	35870	 },  /*	37.62 dB	*/
	{	4279		,	36095	 },  /*	36.62 dB	*/
	{	4113		,	36289	 },  /*	35.62 dB	*/
	{	4020		,	36500	 },  /*	34.62 dB	*/
	{	3930		,	36704	 },  /*	33.62 dB	*/
	{	3838		,	36912	 },  /*	32.62 dB	*/
	{	3738		,	37152	 },  /*	31.62 dB	*/
	{	3648		,	37375	 },  /*	30.62 dB	*/
	{	3544		,	37600	 },  /*	29.62 dB	*/
	{	3458		,	37823	 },  /*	28.62 dB	*/
	{	3358		,	38048	 },  /*	27.62 dB	*/
	{	3281		,	38240	 },  /*	26.62 dB	*/
	{	3191		,	38479	 },  /*	25.62 dB	*/
	{	3091		,	38720	 },  /*	24.62 dB	*/
	{	2993		,	38976	 },  /*	23.63 dB	*/
	{	2900		,	39226	 },  /*	22.63 dB	*/
	{	2792		,	39520	 },  /*	21.62 dB	*/
	{	2692		,	39792	 },  /*	20.62 dB	*/
	{	2592    	,	40064	 },  /*	19.62 dB	*/
	{	2497		,	40351	 },  /*	18.62 dB	*/
	{	2392		,	40640	 },  /*	17.62 dB	*/
	{	2290		,	40976	 },  /*	16.62 dB	*/
	{	2189		,	41295	 },  /*	15.62 dB	*/
	{	2088		,	41631	 },  /*	14.62 dB	*/
	{	1999		,	41934	 },  /*	13.62 dB	*/
	{	1875		,	42354	 },  /*	12.62 dB	*/
	{	1764	   	,	42815	 },  /*	11.62 dB	*/
	{	1637	   	,	43263	 },  /*	10.62 dB	*/	
	{	1537	   	,	43743    },  /*	9.62  dB	*/		
	{	1412		,	44288    },  /*	8.62  dB	*/	
	{	1291		,	44913    },  /*	7.62  dB	*/		
	{	1188	  	,	45712    },  /*	6.62  dB	*/	
	{	1080    	,	46720    },  /*	5.63  dB	*/	
	{	976		,       48164	 },  /*	4.63  dB	*/	
	{	930	    	,	50816    },  /*	3.63  dB	*/	
	{	898	    	,	65534    },  /*	2.94  dB	*/	
	{	880	    	,	65535    }  /*	2.95  dB	*/	
};

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#endif

static unsigned int rfsource;
module_param(rfsource, int, 0644);
MODULE_PARM_DESC(rfsource, "RF source selection  (default:0 - auto)");

static const u8 tuner_init[25] = {
 0x77,
 0x33, 0xce, 0x54, 0x55, 0x0d, 0x32, 0x44, 0x0e,
 0xf9, 0x1b,
 0x33, 0xce, 0x54, 0x55, 0x0d, 0x32, 0x44, 0x0e,
 0x00, 0x00, 0x4c, 0x00, 0x00, 0x4c,
};

LIST_HEAD(stvlist);

static inline u32 MulDiv32(u32 a, u32 b, u32 c)
{
	u64 tmp64;

	tmp64 = (u64)a * (u64)b;
	do_div(tmp64, c);

	return (u32) tmp64;
}


struct stv_base {
	struct list_head     stvlist;

	u8                   adr;
	struct i2c_adapter  *i2c;
	struct mutex         i2c_lock;
	struct mutex         reg_lock;
	int                  count;
};


struct stv {
	struct stv_base     *base;
	struct dvb_frontend *fe;
	int                  nr;

	struct stv6120_cfg *cfg;

	u8 reg[7];
};

static int i2c_read(struct i2c_adapter *adap,
		    u8 adr, u8 *msg, int len, u8 *answ, int alen)
{
	struct i2c_msg msgs[2] = { { .addr = adr, .flags = 0,
				     .buf = msg, .len = len},
				   { .addr = adr, .flags = I2C_M_RD,
				     .buf = answ, .len = alen } };
	if (i2c_transfer(adap, msgs, 2) != 2) {
		pr_err("stv6120: i2c_read error\n");
		return -1;
	}
	return 0;
}

static int i2c_write(struct i2c_adapter *adap, u8 adr, u8 *data, int len)
{
	struct i2c_msg msg = {.addr = adr, .flags = 0,
			      .buf = data, .len = len};

	if (i2c_transfer(adap, &msg, 1) != 1) {
		pr_err("stv6120: i2c_write error\n");
		return -1;
	}
	return 0;
}
#if 0
static int write_regs(struct stv *state, int reg, int len)
{
	u8 d[8];

	u8 base = 0x02 + 0xa * state->nr;

	memcpy(&d[1], &state->base->reg[reg], len);
	d[0] = reg;
	return i2c_write(state->base->i2c, state->base->adr, d, len + 1);
}
#endif

static int write_tuner_regs(struct stv *state)
{
	u8 d[8];
	memcpy(&d[1], state->reg, 7);
	d[0] = 0x02 + 0xa * state->nr;
	return i2c_write(state->base->i2c, state->base->adr, d, 8);
}


static int read_reg(struct stv *state, u8 reg, u8 *val)
{
	return i2c_read(state->base->i2c, state->base->adr, &reg, 1, val, 1);
}

#if 0
static int read_regs(struct stv *state, u8 reg, u8 *val, int len)
{
	return i2c_read(state->base->i2c, state->base->adr, &reg, 1, val, len);
}

static void dump_regs(struct stv *state)
{
	u8 d[25], *c = &state->reg[0];

	read_regs(state, state->nr * 0xa + 2, d, 7);
	pr_info("stv6120_regs = %02x %02x %02x %02x %02x %02x %02x\n",
		d[0], d[1], d[2], d[3], d[4], d[5], d[6]);
	pr_info("reg[] =        %02x %02x %02x %02x %02x %02x %02x\n",
		c[0], c[1], c[2], c[3], c[4], c[5], c[6]);


	read_regs(state, 0, d, 14);
	pr_info("global: 0=%02x a=%02x 1=%02x b=%02x\n", d[0], d[0xa], d[1], d[0xb]);
}
#endif

static int wait_for_call_done(struct stv *state, u8 mask)
{
	int status = 0;
	u32 LockRetryCount = 10;

	while (LockRetryCount > 0) {
		u8 Status;

		status = read_reg(state, state->nr * 0xa + 8, &Status);
		if (status < 0)
			return status;

		if ((Status & mask) == 0)
			break;
		usleep_range(4000, 6000);
		LockRetryCount -= 1;

		status = -1;
	}
	return status;
}

static int probe(struct stv *state)
{
	struct dvb_frontend *fe = state->fe;
	int ret = 0;

	u8 d[26];

	if (fe->ops.i2c_gate_ctrl)
		fe->ops.i2c_gate_ctrl(fe, 1);

	memcpy(&d[1], tuner_init, 25);
	switch (rfsource) {
	case 1:
		d[10] = 0xFA; /* RF C */
		d[11] = 0x13; /* LNAC ON */
		break;	    
	case 2:
		d[10] = 0xF5; /* RF B */
		d[11] = 0x0B; /* LNAB ON */
		break;	    
	}
	d[0] = 0;
	ret = i2c_write(state->base->i2c, state->base->adr, d, 25 + 1);

	if (ret < 0)
		goto err;
#if 0
	pr_info("attach_init OK\n");
	dump_regs(state);
#endif

err:
	if (fe->ops.i2c_gate_ctrl)
		fe->ops.i2c_gate_ctrl(fe, 0);
	return ret;
}

static int sleep(struct dvb_frontend *fe)
{
	/* struct tda_state *state = fe->tuner_priv; */

	//pr_info("tuner sleep\n");
	return 0;
}

static int init(struct dvb_frontend *fe)
{
	/* struct tda_state *state = fe->tuner_priv; */
	//pr_info("init\n");
	return 0;
}

static void release(struct dvb_frontend *fe)
{
	struct stv *state = fe->tuner_priv;

	state->base->count--;
	if (state->base->count == 0) {
		//pr_info("remove STV tuner base\n");
		list_del(&state->base->stvlist);
		kfree(state->base);
	}
	kfree(state);
	fe->tuner_priv = NULL;
}

#if 0
static int set_bandwidth(struct dvb_frontend *fe, u32 CutOffFrequency)
{
	struct stv *state = fe->tuner_priv;
	u32 index = (CutOffFrequency + 999999) / 1000000;

	if (index < 6)
		index = 6;
	if (index > 50)
		index = 50;
	if ((state->base->reg[0x08] & ~0xFC) == ((index-6) << 2))
		return 0;

	state->base->reg[0x08] = (state->base->reg[0x08] & ~0xFC) | ((index-6) << 2);
	state->base->reg[0x09] = (state->base->reg[0x09] & ~0x0C) | 0x08;
	if (fe->ops.i2c_gate_ctrl)
		fe->ops.i2c_gate_ctrl(fe, 1);
	write_regs(state, 0x08, 2);
	wait_for_call_done(state, 0x08);
	if (fe->ops.i2c_gate_ctrl)
		fe->ops.i2c_gate_ctrl(fe, 0);
	return 0;
}

#endif

static int set_lof(struct stv *state, u32 LocalFrequency, u32 CutOffFrequency)
{
	int cf_index = (CutOffFrequency / 1000000) - 5;
	u32 Frequency = (LocalFrequency + 500) / 1000; // Hz -> kHz
	u32 fvco, div, frac;
	u8 Icp, tmp;

	u8 PDIV, P;

	//pr_info("F = %u, CutOff = %u\n", Frequency, CutOffFrequency);


	/* set PDIV and CF */
	if (cf_index < 0)
		cf_index = 0;
	if (cf_index > 31)
		cf_index = 31;

	if (Frequency >= 1191000) {
		PDIV = 0;
		P    = 2;
	} else if (Frequency >= 596000) {
		PDIV = 1;
		P    = 4;
	} else if (Frequency >= 299000) {
		PDIV = 2;
		P    = 8;
	} else {
		PDIV = 3;
		P    = 16;
	}


	fvco = Frequency * P;
	div = (fvco * state->cfg->Rdiv) / state->cfg->xtal;

	/* charge pump current */
	Icp = 0;
	if (fvco < 2472000)
		Icp = 0;
	else if (fvco < 2700000)
		Icp = 1;
	else if (fvco < 3021000)
		Icp = 2;
	else if (fvco < 3387000)
		Icp = 3;
	else if (fvco < 3845000)
		Icp = 5;
	else if (fvco < 4394000)
		Icp = 6;
	else
		Icp = 7;

	frac = (fvco * state->cfg->Rdiv) % state->cfg->xtal;
	frac = MulDiv32(frac, 0x40000, state->cfg->xtal);


	state->reg[REG_N0]    = div & 0xff;
	state->reg[REG_N1_F0] = (((div >> 8) & 0x01) | ((frac & 0x7f) << 1)) & 0xff;
	state->reg[REG_F1]    = (frac >> 7) & 0xff;
	state->reg[REG_F2_ICP] &= 0x88;
	state->reg[REG_F2_ICP] |= (Icp << 4) | ((frac >> 15) & 0x07);
	state->reg[REG_CF_PDIV] &= 0x9f;
	state->reg[REG_CF_PDIV] |= ((PDIV << 5) | cf_index);

	/* Start cal vco,CF */
	state->reg[REG_CAL] &= 0xf8;
	state->reg[REG_CAL] |= 0x06;

	write_tuner_regs(state);

	wait_for_call_done(state, 0x06);

	usleep_range(10000, 12000);

#if 0
	read_reg(state, 0x03, &tmp);
	if (tmp & 0x10)	{
		state->base->reg[0x02] &= ~0x80;   /* LNA NF Mode */
		write_regs(state, 2, 1);
	}
	read_reg(state, 0x08, &tmp);
#endif

//	dump_regs(state);
	return 0;
}

static int set_params(struct dvb_frontend *fe)
{
	struct stv *state = fe->tuner_priv;
	struct dtv_frontend_properties *p = &fe->dtv_property_cache;
	int status;
	u32 freq, symb, cutoff;
	u32 rolloff;

	if (p->delivery_system != SYS_DVBS && p->delivery_system != SYS_DVBS2)
		return -EINVAL;

	switch (p->rolloff) {
	case ROLLOFF_20:
		rolloff = 120;
		break;
	case ROLLOFF_25:
		rolloff = 125;
		break;
	default:
		rolloff = 135;
		break;
	}

	freq = p->frequency * 1000;
	symb = p->symbol_rate;
	cutoff = 5000000 + MulDiv32(p->symbol_rate, rolloff, 200);

	if (fe->ops.i2c_gate_ctrl)
		fe->ops.i2c_gate_ctrl(fe, 1);
	set_lof(state, freq, cutoff);
	if (fe->ops.i2c_gate_ctrl)
		fe->ops.i2c_gate_ctrl(fe, 0);
	return status;
}

static s32 TableLookup(struct SLookup *Table, int TableSize, u16 RegValue)
{
	s32 Gain;
	s32 RegDiff;
	int imin = 0;
	int imax = TableSize - 1;
	int i;
	
	// Assumes Table[0].RegValue < Table[imax].RegValue 
	if( RegValue <= Table[0].RegValue )
		Gain = Table[0].Value;
	else if( RegValue >= Table[imax].RegValue )
		Gain = Table[imax].Value;
	else {
		while(imax-imin > 1) {
			i = (imax + imin) / 2;
			if ((Table[imin].RegValue <= RegValue) &&
			    (RegValue <= Table[i].RegValue) )
				imax = i;
			else
				imin = i;
		}
		RegDiff = Table[imax].RegValue - Table[imin].RegValue;
		Gain = Table[imin].Value;
		if (RegDiff != 0)
			Gain += ((s32) (RegValue - Table[imin].RegValue) *
			    (s32)(Table[imax].Value - Table[imin].Value))/(RegDiff);
	}
	return Gain;
}

static int get_rf_strength(struct dvb_frontend *fe, u16 *agc)
{
	struct dtv_frontend_properties *p = &fe->dtv_property_cache;

	s32 gain = 1, ref_bbgain = 12, tilt = 6;
	s32 freq;

	gain = TableLookup(Gain_RFAGC_LookUp, ARRAY_SIZE(Gain_RFAGC_LookUp), *agc);

	gain += 100 * (6 - ref_bbgain); 	

	freq = p->frequency / 10000;

	if (freq<159)
		gain -= 200; /* HMR filter 2dB gain compensation below freq=1590MHz */	
	
	gain -= (((freq-155)*tilt)/12)*10;
	
	*agc = gain;

	return 0;
}

static struct dvb_tuner_ops tuner_ops = {
	.info = {
		.name = "STV6120",
		.frequency_min_hz  =  250 * MHz,
		.frequency_max_hz  = 2300 * MHz,
	},
	.init              = init,
	.sleep             = sleep,
	.set_params        = set_params,
	.release           = release,
	.get_rf_strength   = get_rf_strength,
};

static struct stv_base *match_base(struct i2c_adapter  *i2c, u8 adr)
{
	struct stv_base *p;

	list_for_each_entry(p, &stvlist, stvlist)
		if (p->i2c == i2c && p->adr == adr)
			return p;
	return NULL;
}

struct dvb_frontend *stv6120_attach(struct dvb_frontend *fe,
		    struct i2c_adapter *i2c, struct stv6120_cfg *cfg, int nr)
{
	struct stv *state;
	struct stv_base *base;

	state = kzalloc(sizeof(struct stv), GFP_KERNEL);
	if (!state)
		return NULL;
	memcpy(&fe->ops.tuner_ops, &tuner_ops, sizeof(struct dvb_tuner_ops));
	state->fe = fe;
	memcpy(state->reg, &tuner_init[2], 7);

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
		mutex_init(&base->i2c_lock);
		mutex_init(&base->reg_lock);
		state->base = base;
		if (probe(state) < 0) {
			kfree(base);
			goto fail;
		}
		list_add(&base->stvlist, &stvlist);
	}


	if (cfg->xtal == 0) {
		printk("xtal=0!!!\n");
		goto fail;
	}

	state->cfg = cfg;
	state->nr = nr;

	fe->tuner_priv = state;
	return fe;
fail:
	kfree(state);
	return NULL;
}
EXPORT_SYMBOL_GPL(stv6120_attach);

MODULE_DESCRIPTION("STV6120 driver");
MODULE_AUTHOR("Luis Alves");
MODULE_LICENSE("GPL");


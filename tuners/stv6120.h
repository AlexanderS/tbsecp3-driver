#ifndef STV6120_H
#define STV6120_H

#include <linux/i2c.h>
#include <media/dvb_frontend.h>

struct stv6120_cfg {
	u8 adr;

	u32 xtal;
	u8 Rdiv;
};


#if IS_REACHABLE(CONFIG_MEDIA_TUNER_STV6120)
extern struct dvb_frontend *stv6120_attach(struct dvb_frontend *fe,
		    struct i2c_adapter *i2c, struct stv6120_cfg *cfg, int nr);
#else
static inline struct dvb_frontend *stv6120_attach(struct dvb_frontend *fe,
			struct i2c_adapter *i2c, struct stv6120_cfg *cfg, int nr)
{
	printk(KERN_WARNING "%s: driver disabled by Kconfig\n", __func__);
	return NULL;
}
#endif


#endif /* STV6120_H */

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

struct tbsecp3_board tbsecp3_boards[] = {
	[TBSECP3_BOARD_TBS6508] = {
		.board_id	= TBSECP3_BOARD_TBS6508,
		.name		= "TurboSight TBS 6508 DVB-S/S2/S2X/T/T2/C(QAM-A/B/C)/C2/ISDB-T ",
		.adapters	= 8,
		.i2c_speed	= 39,
		.eeprom_i2c	= 3,
		.eeprom_addr = 0x10,
		.adap_config	= {
			{
				.ts_in = 4,
				.i2c_bus_nr = 2,
				.gpio.demod_reset.lvl = TBSECP3_GPIODEF_LOW,
				.gpio.demod_reset.nr  = TBSECP3_GPIO_PIN(2, 0),
				.gpio.lnb_power.lvl   = TBSECP3_GPIODEF_LOW,
				.gpio.lnb_power.nr    = TBSECP3_GPIO_PIN(2, 2),
				.gpio.lnb_voltage.lvl = TBSECP3_GPIODEF_HIGH,
				.gpio.lnb_voltage.nr  = TBSECP3_GPIO_PIN(2, 1),
			},
			{
				.ts_in = 5,
				.i2c_bus_nr = 2,
				.gpio.lnb_power.lvl   = TBSECP3_GPIODEF_LOW,
				.gpio.lnb_power.nr    = TBSECP3_GPIO_PIN(2, 2),
				.gpio.lnb_voltage.lvl = TBSECP3_GPIODEF_HIGH,
				.gpio.lnb_voltage.nr  = TBSECP3_GPIO_PIN(2, 1),
			},
			{
				.ts_in = 0,
				.i2c_bus_nr = 0,
				.gpio.demod_reset.lvl = TBSECP3_GPIODEF_LOW,
				.gpio.demod_reset.nr  = TBSECP3_GPIO_PIN(0, 0),
				.gpio.lnb_power.lvl   = TBSECP3_GPIODEF_LOW,
				.gpio.lnb_power.nr	  = TBSECP3_GPIO_PIN(0, 2),
				.gpio.lnb_voltage.lvl = TBSECP3_GPIODEF_HIGH,
				.gpio.lnb_voltage.nr  = TBSECP3_GPIO_PIN(0, 1),
			},
			{
				.ts_in = 1,
				.i2c_bus_nr = 0,
				.gpio.lnb_power.lvl   = TBSECP3_GPIODEF_LOW,
				.gpio.lnb_power.nr	  = TBSECP3_GPIO_PIN(0, 2),
				.gpio.lnb_voltage.lvl = TBSECP3_GPIODEF_HIGH,
				.gpio.lnb_voltage.nr  = TBSECP3_GPIO_PIN(0, 1),
			},
			{
				.ts_in = 2,
				.i2c_bus_nr = 1,
				.gpio.demod_reset.lvl = TBSECP3_GPIODEF_LOW,
				.gpio.demod_reset.nr  = TBSECP3_GPIO_PIN(1, 0),
				.gpio.lnb_power.lvl   = TBSECP3_GPIODEF_LOW,
				.gpio.lnb_power.nr    = TBSECP3_GPIO_PIN(1, 2),
				.gpio.lnb_voltage.lvl = TBSECP3_GPIODEF_HIGH,
				.gpio.lnb_voltage.nr  = TBSECP3_GPIO_PIN(1, 1),
			},
			{
				.ts_in = 3,
				.i2c_bus_nr = 1,
				.gpio.lnb_power.lvl   = TBSECP3_GPIODEF_LOW,
				.gpio.lnb_power.nr    = TBSECP3_GPIO_PIN(1, 2),
				.gpio.lnb_voltage.lvl = TBSECP3_GPIODEF_HIGH,
				.gpio.lnb_voltage.nr  = TBSECP3_GPIO_PIN(1, 1),
			},
			{
				.ts_in = 6,
				.i2c_bus_nr = 3,
				.gpio.demod_reset.lvl = TBSECP3_GPIODEF_LOW,
				.gpio.demod_reset.nr  = TBSECP3_GPIO_PIN(3, 0),
				.gpio.lnb_power.lvl   = TBSECP3_GPIODEF_LOW,
				.gpio.lnb_power.nr	  = TBSECP3_GPIO_PIN(3, 2),
				.gpio.lnb_voltage.lvl = TBSECP3_GPIODEF_HIGH,
				.gpio.lnb_voltage.nr  = TBSECP3_GPIO_PIN(3, 1),
			},
			{
				.ts_in = 7,
				.i2c_bus_nr = 3,
				.gpio.lnb_power.lvl   = TBSECP3_GPIODEF_LOW,
				.gpio.lnb_power.nr	  = TBSECP3_GPIO_PIN(3, 2),
				.gpio.lnb_voltage.lvl = TBSECP3_GPIODEF_HIGH,
				.gpio.lnb_voltage.nr  = TBSECP3_GPIO_PIN(3, 1),
			},
		}
	},

	[TBSECP3_BOARD_TBS6504] = {
		.board_id	= TBSECP3_BOARD_TBS6504,
		.name		= "TurboSight TBS 6504 DVB-S/S2/S2X/T/T2/C/C2/ISDB-T ",
		.adapters	= 4,
		.i2c_speed	= 39,
		.eeprom_i2c	= 0,
		.adap_config	= {
			{
				.ts_in = 3,
				.i2c_bus_nr = 1,
				.gpio.demod_reset.lvl = TBSECP3_GPIODEF_LOW,
				.gpio.demod_reset.nr  = TBSECP3_GPIO_PIN(1, 0),
				.gpio.lnb_power.lvl   = TBSECP3_GPIODEF_LOW,
				.gpio.lnb_power.nr    = TBSECP3_GPIO_PIN(3, 2),
				.gpio.lnb_voltage.lvl = TBSECP3_GPIODEF_HIGH,
				.gpio.lnb_voltage.nr  = TBSECP3_GPIO_PIN(3, 1),
			},
			{
				.ts_in = 2,
				.i2c_bus_nr = 1,
				.gpio.lnb_power.lvl   = TBSECP3_GPIODEF_LOW,
				.gpio.lnb_power.nr    = TBSECP3_GPIO_PIN(2, 2),
				.gpio.lnb_voltage.lvl = TBSECP3_GPIODEF_HIGH,
				.gpio.lnb_voltage.nr  = TBSECP3_GPIO_PIN(2, 1),
			},
			{
					.ts_in = 1,
					.i2c_bus_nr = 0,
					.gpio.demod_reset.lvl = TBSECP3_GPIODEF_LOW,
					.gpio.demod_reset.nr  = TBSECP3_GPIO_PIN(0, 0),
					.gpio.lnb_power.lvl   = TBSECP3_GPIODEF_LOW,
					.gpio.lnb_power.nr	  = TBSECP3_GPIO_PIN(1, 2),
					.gpio.lnb_voltage.lvl = TBSECP3_GPIODEF_HIGH,
					.gpio.lnb_voltage.nr  = TBSECP3_GPIO_PIN(1, 1),
				},
				{
					.ts_in = 0,
					.i2c_bus_nr = 0,
					.gpio.lnb_power.lvl   = TBSECP3_GPIODEF_LOW,
					.gpio.lnb_power.nr	  = TBSECP3_GPIO_PIN(0, 2),
					.gpio.lnb_voltage.lvl = TBSECP3_GPIODEF_HIGH,
					.gpio.lnb_voltage.nr  = TBSECP3_GPIO_PIN(0, 1),
				},
		}
	},

	[TBSECP3_BOARD_TBS6904X] = {
		.board_id	= TBSECP3_BOARD_TBS6904X,
		.name		= "TurboSight TBS 6904x DVB-S/S2/S2X ",
		.adapters	= 4,
		.i2c_speed	= 39,
		.eeprom_i2c	= 0,
		.eeprom_addr = 0x10,
		.adap_config	= {
			{
				.ts_in = 3,
				.i2c_bus_nr = 1,
				.gpio.demod_reset.lvl = TBSECP3_GPIODEF_LOW,
				.gpio.demod_reset.nr  = TBSECP3_GPIO_PIN(1, 0),
				.gpio.lnb_power.lvl   = TBSECP3_GPIODEF_LOW,
				.gpio.lnb_power.nr    = TBSECP3_GPIO_PIN(3, 2),
				.gpio.lnb_voltage.lvl = TBSECP3_GPIODEF_HIGH,
				.gpio.lnb_voltage.nr  = TBSECP3_GPIO_PIN(3, 1),
			},
			{
				.ts_in = 2,
				.i2c_bus_nr = 1,
				.gpio.lnb_power.lvl   = TBSECP3_GPIODEF_LOW,
				.gpio.lnb_power.nr    = TBSECP3_GPIO_PIN(2, 2),
				.gpio.lnb_voltage.lvl = TBSECP3_GPIODEF_HIGH,
				.gpio.lnb_voltage.nr  = TBSECP3_GPIO_PIN(2, 1),
			},
			{
				.ts_in = 1,
				.i2c_bus_nr = 0,
				.gpio.demod_reset.lvl = TBSECP3_GPIODEF_LOW,
				.gpio.demod_reset.nr  = TBSECP3_GPIO_PIN(0, 0),
				.gpio.lnb_power.lvl   = TBSECP3_GPIODEF_LOW,
				.gpio.lnb_power.nr	  = TBSECP3_GPIO_PIN(1, 2),
				.gpio.lnb_voltage.lvl = TBSECP3_GPIODEF_HIGH,
				.gpio.lnb_voltage.nr  = TBSECP3_GPIO_PIN(1, 1),
			},
			{
				.ts_in = 0,
				.i2c_bus_nr = 0,
				.gpio.lnb_power.lvl   = TBSECP3_GPIODEF_LOW,
				.gpio.lnb_power.nr	  = TBSECP3_GPIO_PIN(0, 2),
				.gpio.lnb_voltage.lvl = TBSECP3_GPIODEF_HIGH,
				.gpio.lnb_voltage.nr  = TBSECP3_GPIO_PIN(0, 1),
			},

		}
	},

	[TBSECP3_BOARD_TBS6308] = {
		.board_id	= TBSECP3_BOARD_TBS6308,
		.name		= "TurboSight TBS 6304 HDMI Capture",
		.adapters	= 4,
		.i2c_speed	= 39,
		.eeprom_i2c	= 0,
		.adap_config	= {
			{
				.ts_in = 0,
				.i2c_bus_nr = 0,
			}, 
			{
				.ts_in = 1,
				.i2c_bus_nr = 0,
			},
			{
				.ts_in = 2,
				.i2c_bus_nr = 0,
			},
			{
				.ts_in = 3,
				.i2c_bus_nr = 0,
			},
			{
				.ts_in = 4,
				.i2c_bus_nr = 0,
			},
			{
				.ts_in = 5,
				.i2c_bus_nr = 0,
			},
			{
				.ts_in = 6,
				.i2c_bus_nr = 0,
			},
			{
				.ts_in = 7,
				.i2c_bus_nr = 0,

			}
		}
	},
    // added by pzw in 2017,12,12
	[TBSECP3_BOARD_TBS6304] = {
		.board_id	= TBSECP3_BOARD_TBS6304,
 	 	.name		= "TurboSight TBS 6304 HDMI Capture ",
		.adapters	= 4,
		.i2c_speed	= 39,
		.eeprom_i2c	= 0,
		.adap_config	= {
 			{ 
 			 	.ts_in = 0,
				.i2c_bus_nr = 3,
				.gpio.demod_reset.lvl = TBSECP3_GPIODEF_LOW,
				.gpio.demod_reset.nr  = TBSECP3_GPIO_PIN(3, 0),
				.gpio.lnb_power.lvl   = TBSECP3_GPIODEF_LOW,
				.gpio.lnb_power.nr    = TBSECP3_GPIO_PIN(3, 2),
			}, 
 			{ 
 			 	.ts_in = 1,
				.i2c_bus_nr = 2,
				.gpio.demod_reset.lvl = TBSECP3_GPIODEF_LOW,
				.gpio.demod_reset.nr  = TBSECP3_GPIO_PIN(2, 0),
				.gpio.lnb_power.lvl   = TBSECP3_GPIODEF_LOW,
				.gpio.lnb_power.nr    = TBSECP3_GPIO_PIN(2, 2),
			}, 
 			{ 
 			 	.ts_in = 2,
				.i2c_bus_nr = 1,
				.gpio.demod_reset.lvl = TBSECP3_GPIODEF_LOW,
				.gpio.demod_reset.nr  = TBSECP3_GPIO_PIN(1, 0),
				.gpio.lnb_power.lvl   = TBSECP3_GPIODEF_LOW,
				.gpio.lnb_power.nr    = TBSECP3_GPIO_PIN(1, 2),
			}, 
 			{ 
 			 	.ts_in = 3,
				.i2c_bus_nr = 0,
				.gpio.demod_reset.lvl = TBSECP3_GPIODEF_LOW,
				.gpio.demod_reset.nr  = TBSECP3_GPIO_PIN(0, 0),
				.gpio.lnb_power.lvl   = TBSECP3_GPIODEF_LOW,
				.gpio.lnb_power.nr    = TBSECP3_GPIO_PIN(0, 2),
			}, 
			
		}
	},
	[TBSECP3_BOARD_TBS6301] = {
		.board_id	= TBSECP3_BOARD_TBS6301,
		.name		= "TurboSight TBS 6301 HDMI Capture ",
		.adapters	= 1,
		.i2c_speed	= 39,
		.eeprom_i2c	= 0,
		.adap_config	= {
			{
				.ts_in = 0,
				.i2c_bus_nr = 3,
				.gpio.demod_reset.lvl = TBSECP3_GPIODEF_LOW,
				.gpio.demod_reset.nr  = TBSECP3_GPIO_PIN(3, 0),
				.gpio.lnb_power.lvl   = TBSECP3_GPIODEF_LOW,
				.gpio.lnb_power.nr    = TBSECP3_GPIO_PIN(3, 2),
			},
			
		}
	},
	[TBSECP3_BOARD_TBS690a] = {
		.board_id	= TBSECP3_BOARD_TBS690a,
		.name		= "TurboSight TBS 690a ASI Capture ",
		.adapters	= 4,
		.i2c_speed	= 39,
		.eeprom_i2c	= 1,
		.adap_config	= {
			{
				.ts_in = 0,
				.i2c_bus_nr = 3,
				.gpio.demod_reset.lvl = TBSECP3_GPIODEF_LOW,
				.gpio.demod_reset.nr  = TBSECP3_GPIO_PIN(3, 0),
				.gpio.lnb_power.lvl   = TBSECP3_GPIODEF_LOW,
				.gpio.lnb_power.nr    = TBSECP3_GPIO_PIN(3, 2),
			},
			{
				.ts_in = 1,
				.i2c_bus_nr = 2,
				.gpio.demod_reset.lvl = TBSECP3_GPIODEF_LOW,
				.gpio.demod_reset.nr  = TBSECP3_GPIO_PIN(2, 0),
				.gpio.lnb_power.lvl   = TBSECP3_GPIODEF_LOW,
				.gpio.lnb_power.nr    = TBSECP3_GPIO_PIN(2, 2),
			},
			{
				.ts_in = 2,
				.i2c_bus_nr = 1,
				.gpio.demod_reset.lvl = TBSECP3_GPIODEF_LOW,
				.gpio.demod_reset.nr  = TBSECP3_GPIO_PIN(1, 0),
				.gpio.lnb_power.lvl   = TBSECP3_GPIODEF_LOW,
				.gpio.lnb_power.nr    = TBSECP3_GPIO_PIN(1, 2),
			},
			{
				.ts_in = 3,
				.i2c_bus_nr = 0,
				.gpio.demod_reset.lvl = TBSECP3_GPIODEF_LOW,
				.gpio.demod_reset.nr  = TBSECP3_GPIO_PIN(0, 0),
				.gpio.lnb_power.lvl   = TBSECP3_GPIODEF_LOW,
				.gpio.lnb_power.nr    = TBSECP3_GPIO_PIN(0, 2),
			}, 
		}
	},
	[TBSECP3_BOARD_TBS6514] = {
		.board_id	= TBSECP3_BOARD_TBS6514,
		.name  		= "Turbosight TBS 6514 (Quad DTMB)",
		.i2c_speed  = 39,
		.eeprom_i2c = 1,
		.adapters	= 4,
		.adap_config ={
			
			{
				.ts_in = 0,
				.i2c_bus_nr = 0,
				.gpio.demod_reset.lvl = TBSECP3_GPIODEF_LOW,
				.gpio.demod_reset.nr  = TBSECP3_GPIO_PIN(0, 0),
			}, 
			{
				.ts_in = 1,
				.i2c_bus_nr = 1,
				.gpio.demod_reset.lvl = TBSECP3_GPIODEF_LOW,
				.gpio.demod_reset.nr  = TBSECP3_GPIO_PIN(1, 0),
			},
			{
				.ts_in = 2,
				.i2c_bus_nr = 2,
				.gpio.demod_reset.lvl = TBSECP3_GPIODEF_LOW,
				.gpio.demod_reset.nr  = TBSECP3_GPIO_PIN(2, 0),
			},
			{
				.ts_in = 3,
				.i2c_bus_nr = 3,
				.gpio.demod_reset.lvl = TBSECP3_GPIODEF_LOW,
				.gpio.demod_reset.nr  = TBSECP3_GPIO_PIN(3, 0),
			}
		},
	},

	[TBSECP3_BOARD_TBS6814] = {
		.board_id	= TBSECP3_BOARD_TBS6814,
		.name  		= "Turbosight TBS 6814 (Quad ISDB-T)",
		.i2c_speed  = 39,
		.eeprom_i2c = 1,
		.adapters	= 4,
		.adap_config ={
			
			{
				.ts_in = 0,
				.i2c_bus_nr = 0,
				.gpio.demod_reset.lvl = TBSECP3_GPIODEF_LOW,
				.gpio.demod_reset.nr  = TBSECP3_GPIO_PIN(0, 0),
			}, 
			{
				.ts_in = 1,
				.i2c_bus_nr = 1,
				.gpio.demod_reset.lvl = TBSECP3_GPIODEF_LOW,
				.gpio.demod_reset.nr  = TBSECP3_GPIO_PIN(1, 0),
			},
			{
				.ts_in = 2,
				.i2c_bus_nr = 2,
				.gpio.demod_reset.lvl = TBSECP3_GPIODEF_LOW,
				.gpio.demod_reset.nr  = TBSECP3_GPIO_PIN(2, 0),
			},
			{
				.ts_in = 3,
				.i2c_bus_nr = 3,
				.gpio.demod_reset.lvl = TBSECP3_GPIODEF_LOW,
				.gpio.demod_reset.nr  = TBSECP3_GPIO_PIN(3, 0),
			}
		},
	},
	[TBSECP3_BOARD_TBS6209] = {
		.board_id	= TBSECP3_BOARD_TBS6209,
		.name		= "Turbosight TBS 6209 (Octa DVB-T/T2/C/C2/ISDB-T)",
		.i2c_speed  = 39,
		.eeprom_i2c	= 0,
		.eeprom_addr = 0x10,
		.adapters	= 8,
		.adap_config ={
			{
				.ts_in = 0,
				.i2c_bus_nr = 0,
				.gpio.demod_reset.lvl = TBSECP3_GPIODEF_LOW,
				.gpio.demod_reset.nr  = TBSECP3_GPIO_PIN(0, 0),
			}, 
			{
				.ts_in = 1,
				.i2c_bus_nr = 0,
			},
			{
				.ts_in = 2,
				.i2c_bus_nr = 1,
				.gpio.demod_reset.lvl = TBSECP3_GPIODEF_LOW,
				.gpio.demod_reset.nr  = TBSECP3_GPIO_PIN(1, 0),
			},
			{
				.ts_in = 3,
				.i2c_bus_nr = 1,
			},
			{
				.ts_in = 4,
				.i2c_bus_nr = 2,
				.gpio.demod_reset.lvl = TBSECP3_GPIODEF_LOW,
				.gpio.demod_reset.nr  = TBSECP3_GPIO_PIN(2, 0),
				
			},
			{
				.ts_in = 5,
				.i2c_bus_nr = 2,
			},
			{
				.ts_in = 6,
				.i2c_bus_nr = 3,
				.gpio.demod_reset.lvl = TBSECP3_GPIODEF_LOW,
				.gpio.demod_reset.nr  = TBSECP3_GPIO_PIN(3, 0),
				
			},
			{
				.ts_in = 7,
				.i2c_bus_nr = 3,
			}
		
		}
	},
	[TBSECP3_BOARD_TBS6704] = {
		.board_id	= TBSECP3_BOARD_TBS6704,
		.name		= "TurboSight TBS 6704(Quad ATSC/QAMB)",
		.i2c_speed	= 39,
		.eeprom_i2c	= 1,
		.adapters	= 4,
		.adap_config	= {
			{
				.ts_in = 0,
				.i2c_bus_nr = 0,
				.gpio.demod_reset.lvl = TBSECP3_GPIODEF_LOW,
				.gpio.demod_reset.nr  = TBSECP3_GPIO_PIN(0, 0),
			}, 
			{
				.ts_in = 1,
				.i2c_bus_nr = 1,
				.gpio.demod_reset.lvl = TBSECP3_GPIODEF_LOW,
				.gpio.demod_reset.nr  = TBSECP3_GPIO_PIN(1, 0),
			},
			{
				.ts_in = 2,
				.i2c_bus_nr = 2,
				.gpio.demod_reset.lvl = TBSECP3_GPIODEF_LOW,
				.gpio.demod_reset.nr  = TBSECP3_GPIO_PIN(2, 0),
			},
			{
				.ts_in = 3,
				.i2c_bus_nr = 3,
				.gpio.demod_reset.lvl = TBSECP3_GPIODEF_LOW,
				.gpio.demod_reset.nr  = TBSECP3_GPIO_PIN(3, 0),
			}
		}
	},

	[TBSECP3_BOARD_TBS6205] = {
		.board_id	= TBSECP3_BOARD_TBS6205,
		.name		= "TurboSight TBS 6205 DVB-T/T2/C ",
		.i2c_speed	= 39,
		.eeprom_i2c	= 1,
		.eeprom_addr = 0xa0,
		.adapters	= 4,
		.adap_config	= {
			{
				.ts_in = 0,
				.i2c_bus_nr = 0,
				.gpio.demod_reset.lvl = TBSECP3_GPIODEF_LOW,
				.gpio.demod_reset.nr  = TBSECP3_GPIO_PIN(0, 0),
			}, 
			{
				.ts_in = 1,
				.i2c_bus_nr = 1,
				.gpio.demod_reset.lvl = TBSECP3_GPIODEF_LOW,
				.gpio.demod_reset.nr  = TBSECP3_GPIO_PIN(1, 0),
			},
			{
				.ts_in = 2,
				.i2c_bus_nr = 2,
				.gpio.demod_reset.lvl = TBSECP3_GPIODEF_LOW,
				.gpio.demod_reset.nr  = TBSECP3_GPIO_PIN(2, 0),
			},
			{
				.ts_in = 3,
				.i2c_bus_nr = 3,
				.gpio.demod_reset.lvl = TBSECP3_GPIODEF_LOW,
				.gpio.demod_reset.nr  = TBSECP3_GPIO_PIN(3, 0),
			}
		}
	},
	[TBSECP3_BOARD_TBS6522] = {
		.board_id	= TBSECP3_BOARD_TBS6522,
		.name		= "TurboSight TBS 6522 DVB-S/S2/S2X/T/T2/C/C2/ISDB-T ",
		.adapters	= 2,
		.i2c_speed	= 39,
		.eeprom_i2c	= 0,
		.eeprom_addr = 0xa0,
		.adap_config	= {
			{
				.ts_in = 1,
				.i2c_bus_nr = 0,
				.gpio.demod_reset.lvl = TBSECP3_GPIODEF_LOW,
				.gpio.demod_reset.nr  = TBSECP3_GPIO_PIN(0, 0),
				.gpio.lnb_power.lvl   = TBSECP3_GPIODEF_LOW,
				.gpio.lnb_power.nr    = TBSECP3_GPIO_PIN(0, 2),
				.gpio.lnb_voltage.lvl = TBSECP3_GPIODEF_HIGH,
				.gpio.lnb_voltage.nr  = TBSECP3_GPIO_PIN(0, 1),
			},
			{
				.ts_in = 0,
				.i2c_bus_nr = 0,
				.gpio.lnb_power.lvl   = TBSECP3_GPIODEF_LOW,
				.gpio.lnb_power.nr    = TBSECP3_GPIO_PIN(1, 2),
				.gpio.lnb_voltage.lvl = TBSECP3_GPIODEF_HIGH,
				.gpio.lnb_voltage.nr  = TBSECP3_GPIO_PIN(1, 1),
			},
		}
	},
	[TBSECP3_BOARD_TBS6902] = {
		.board_id	= TBSECP3_BOARD_TBS6902,
		.name		= "TurboSight TBS 6902 DVB-S/S2 ",
		.adapters	= 2,
		.i2c_speed	= 39,
		.eeprom_i2c	= 0,
		.adap_config	= {
			{
				.ts_in = 1,
				.i2c_bus_nr = 1,
				.gpio.demod_reset.lvl = TBSECP3_GPIODEF_LOW,
				.gpio.demod_reset.nr  = TBSECP3_GPIO_PIN(1, 0),
				.gpio.lnb_power.lvl   = TBSECP3_GPIODEF_LOW,
				.gpio.lnb_power.nr    = TBSECP3_GPIO_PIN(1, 2),
			},	
			{
				.ts_in = 0,
				.i2c_bus_nr = 0,
				.gpio.demod_reset.lvl = TBSECP3_GPIODEF_LOW,
				.gpio.demod_reset.nr  = TBSECP3_GPIO_PIN(0, 0),
				.gpio.lnb_power.lvl   = TBSECP3_GPIODEF_LOW,
				.gpio.lnb_power.nr    = TBSECP3_GPIO_PIN(0, 2),
			}
			
		}
	},
	[TBSECP3_BOARD_TBS6903] = {
		.board_id	= TBSECP3_BOARD_TBS6903,
		.name		= "TurboSight TBS 6903 DVB-S/S2 ",
		.adapters	= 2,
		.i2c_speed	= 39,
		.eeprom_i2c	= 1,
		.adap_config	= {
			{
				.ts_in = 0,
				.i2c_bus_nr = 0,
				.gpio.demod_reset.lvl = TBSECP3_GPIODEF_LOW,
				.gpio.demod_reset.nr  = TBSECP3_GPIO_PIN(0, 0),
				.gpio.lnb_power.lvl   = TBSECP3_GPIODEF_LOW,
				.gpio.lnb_power.nr    = TBSECP3_GPIO_PIN(0, 2),
				.gpio.lnb_voltage.lvl = TBSECP3_GPIODEF_HIGH,
				.gpio.lnb_voltage.nr  = TBSECP3_GPIO_PIN(0, 1),
			}, 
			{
				.ts_in = 1,
				.i2c_bus_nr = 0,
				.gpio.lnb_power.lvl   = TBSECP3_GPIODEF_LOW,
				.gpio.lnb_power.nr    = TBSECP3_GPIO_PIN(1, 2),
				.gpio.lnb_voltage.lvl = TBSECP3_GPIODEF_HIGH,
				.gpio.lnb_voltage.nr  = TBSECP3_GPIO_PIN(1, 1),
			},
		}
	},
	[TBSECP3_BOARD_TBS6904] = {
		.board_id	= TBSECP3_BOARD_TBS6904,
		.name		= "TurboSight TBS 6904 DVB-S/S2 ",
		.adapters	= 4,
		.i2c_speed	= 39,
		.eeprom_i2c	= 1,
		.adap_config	= {
			{
				.ts_in = 3,
				.i2c_bus_nr = 3,
				.gpio.demod_reset.lvl = TBSECP3_GPIODEF_LOW,
				.gpio.demod_reset.nr  = TBSECP3_GPIO_PIN(3, 0),
				.gpio.lnb_power.lvl   = TBSECP3_GPIODEF_LOW,
				.gpio.lnb_power.nr    = TBSECP3_GPIO_PIN(3, 2),
			},
			{
				.ts_in = 2,
				.i2c_bus_nr = 2,
				.gpio.demod_reset.lvl = TBSECP3_GPIODEF_LOW,
				.gpio.demod_reset.nr  = TBSECP3_GPIO_PIN(2, 0),
				.gpio.lnb_power.lvl   = TBSECP3_GPIODEF_LOW,
				.gpio.lnb_power.nr    = TBSECP3_GPIO_PIN(2, 2),
			},
			{
				.ts_in = 1,
				.i2c_bus_nr = 1,
				.gpio.demod_reset.lvl = TBSECP3_GPIODEF_LOW,
				.gpio.demod_reset.nr  = TBSECP3_GPIO_PIN(1, 0),
				.gpio.lnb_power.lvl   = TBSECP3_GPIODEF_LOW,
				.gpio.lnb_power.nr    = TBSECP3_GPIO_PIN(1, 2),
			},
			{
				.ts_in = 0,
				.i2c_bus_nr = 0,
				.gpio.demod_reset.lvl = TBSECP3_GPIODEF_LOW,
				.gpio.demod_reset.nr  = TBSECP3_GPIO_PIN(0, 0),
				.gpio.lnb_power.lvl   = TBSECP3_GPIODEF_LOW,
				.gpio.lnb_power.nr    = TBSECP3_GPIO_PIN(0, 2),
			}, 
		}
	},
	[TBSECP3_BOARD_TBS6905] = {
		.board_id	= TBSECP3_BOARD_TBS6905,
		.name		= "TurboSight TBS 6905 DVB-S/S2 ",
		.adapters	= 4,
		.i2c_speed	= 39,
		.eeprom_i2c	= 2,
		.adap_config	= {
			{
				.ts_in = 2,
				.i2c_bus_nr = 2,
				.gpio.demod_reset.lvl = TBSECP3_GPIODEF_LOW,
				.gpio.demod_reset.nr  = TBSECP3_GPIO_PIN(2, 0),
				.gpio.lnb_power.lvl   = TBSECP3_GPIODEF_LOW,
				.gpio.lnb_power.nr    = TBSECP3_GPIO_PIN(2, 2),
				.gpio.lnb_voltage.lvl = TBSECP3_GPIODEF_HIGH,
				.gpio.lnb_voltage.nr  = TBSECP3_GPIO_PIN(2, 1),
			},
			{
				.ts_in = 3,
				.i2c_bus_nr = 2,
				.gpio.lnb_power.lvl   = TBSECP3_GPIODEF_LOW,
				.gpio.lnb_power.nr    = TBSECP3_GPIO_PIN(3, 2),
				.gpio.lnb_voltage.lvl = TBSECP3_GPIODEF_HIGH,
				.gpio.lnb_voltage.nr  = TBSECP3_GPIO_PIN(3, 1),
			},
			{
				.ts_in = 0,
				.i2c_bus_nr = 0,
				.gpio.demod_reset.lvl = TBSECP3_GPIODEF_LOW,
				.gpio.demod_reset.nr  = TBSECP3_GPIO_PIN(0, 0),
				.gpio.lnb_power.lvl   = TBSECP3_GPIODEF_LOW,
				.gpio.lnb_power.nr    = TBSECP3_GPIO_PIN(0, 2),
				.gpio.lnb_voltage.lvl = TBSECP3_GPIODEF_HIGH,
				.gpio.lnb_voltage.nr  = TBSECP3_GPIO_PIN(0, 1),
			},
			{
				.ts_in = 1,
				.i2c_bus_nr = 0,
				.gpio.lnb_power.lvl   = TBSECP3_GPIODEF_LOW,
				.gpio.lnb_power.nr    = TBSECP3_GPIO_PIN(1, 2),
				.gpio.lnb_voltage.lvl = TBSECP3_GPIODEF_HIGH,
				.gpio.lnb_voltage.nr  = TBSECP3_GPIO_PIN(1, 1),
			},
		}
	},
	[TBSECP3_BOARD_TBS6908] = {
		.board_id	= TBSECP3_BOARD_TBS6908,
		.name		= "TurboSight TBS 6908 DVB-S/S2 ",
		.adapters	= 4,
		.i2c_speed	= 39,
		.eeprom_i2c	= 2,
		.adap_config	= {
			{
				.ts_in = 2,
				.i2c_bus_nr = 2,
				.gpio.demod_reset.lvl = TBSECP3_GPIODEF_LOW,
				.gpio.demod_reset.nr  = TBSECP3_GPIO_PIN(2, 0),
				.gpio.lnb_power.lvl   = TBSECP3_GPIODEF_LOW,
				.gpio.lnb_power.nr    = TBSECP3_GPIO_PIN(2, 2),
				.gpio.lnb_voltage.lvl = TBSECP3_GPIODEF_HIGH,
				.gpio.lnb_voltage.nr  = TBSECP3_GPIO_PIN(2, 1),
			},

			{
				.ts_in = 3,
				.i2c_bus_nr = 2,
				.gpio.lnb_power.lvl   = TBSECP3_GPIODEF_LOW,
				.gpio.lnb_power.nr    = TBSECP3_GPIO_PIN(3, 2),
				.gpio.lnb_voltage.lvl = TBSECP3_GPIODEF_HIGH,
				.gpio.lnb_voltage.nr  = TBSECP3_GPIO_PIN(3, 1),
			},
			{
				.ts_in = 0,
				.i2c_bus_nr = 0,
				.gpio.demod_reset.lvl = TBSECP3_GPIODEF_LOW,
				.gpio.demod_reset.nr  = TBSECP3_GPIO_PIN(0, 0),
				.gpio.lnb_power.lvl   = TBSECP3_GPIODEF_LOW,
				.gpio.lnb_power.nr    = TBSECP3_GPIO_PIN(0, 2),
				.gpio.lnb_voltage.lvl = TBSECP3_GPIODEF_HIGH,
				.gpio.lnb_voltage.nr  = TBSECP3_GPIO_PIN(0, 1),
			},
			{
				.ts_in = 1,
				.i2c_bus_nr = 0,
				.gpio.lnb_power.lvl   = TBSECP3_GPIODEF_LOW,
				.gpio.lnb_power.nr    = TBSECP3_GPIO_PIN(1, 2),
				.gpio.lnb_voltage.lvl = TBSECP3_GPIODEF_HIGH,
				.gpio.lnb_voltage.nr  = TBSECP3_GPIO_PIN(1, 1),
			},
		}
	},
	[TBSECP3_BOARD_TBS6909] = {
		.board_id	= TBSECP3_BOARD_TBS6909,
		.name		= "TurboSight TBS 6909 DVB-S/S2 ",
		.adapters	= 8,
		.i2c_speed	= 39,
		.eeprom_i2c	= 0,
		.eeprom_addr	= 0x10,
		.adap_config	= {
			{
				.ts_in = 0,
				.i2c_bus_nr = 0,
				.gpio.demod_reset.lvl = TBSECP3_GPIODEF_LOW,
				.gpio.demod_reset.nr  = TBSECP3_GPIO_PIN(0, 0),
			}, 
			{
				.ts_in = 1,
				.i2c_bus_nr = 0,
			},
			{
				.ts_in = 2,
				.i2c_bus_nr = 0,
			},
			{
				.ts_in = 3,
				.i2c_bus_nr = 0,
			},
			{
				.ts_in = 4,
				.i2c_bus_nr = 0,
			},
			{
				.ts_in = 5,
				.i2c_bus_nr = 0,
			},
			{
				.ts_in = 6,
				.i2c_bus_nr = 0,
			},
			{
				.ts_in = 7,
				.i2c_bus_nr = 0,
			}
		}
	},
	[TBSECP3_BOARD_TBS6910] = {
		.board_id	= TBSECP3_BOARD_TBS6910,
		.name		= "TurboSight TBS 6910 DVB-S/S2 + 2xCI ",
		.adapters	= 2,
		.i2c_speed	= 39,
		.eeprom_i2c	= 1,
		.adap_config	= {
			{
				.ts_in = 0,
				.i2c_bus_nr = 0,
				.gpio.demod_reset.lvl = TBSECP3_GPIODEF_LOW,
				.gpio.demod_reset.nr  = TBSECP3_GPIO_PIN(0, 0),
				.gpio.lnb_power.lvl   = TBSECP3_GPIODEF_LOW,
				.gpio.lnb_power.nr    = TBSECP3_GPIO_PIN(0, 2),
			},
			{
				.ts_in = 1,
				.i2c_bus_nr = 1,
				.gpio.demod_reset.lvl = TBSECP3_GPIODEF_LOW,
				.gpio.demod_reset.nr  = TBSECP3_GPIO_PIN(1, 0),
				.gpio.lnb_power.lvl   = TBSECP3_GPIODEF_LOW,
				.gpio.lnb_power.nr    = TBSECP3_GPIO_PIN(1, 2),
			},
		}
	},
	[TBSECP3_BOARD_TBS6528] = {
		.board_id	= TBSECP3_BOARD_TBS6528,
		.name		= "TurboSight TBS 6528 DVB-S/S2/S2X/T/T2/C/C2/ISDB-T + CI ",
		.adapters	= 1,
		.i2c_speed	= 39,
		.eeprom_i2c	= 0,
		.eeprom_addr = 0xa0,
		.adap_config	= {
			{
				.ts_in = 0,
				.i2c_bus_nr = 0,
				.gpio.demod_reset.lvl = TBSECP3_GPIODEF_LOW,
				.gpio.demod_reset.nr  = TBSECP3_GPIO_PIN(0, 0),
				.gpio.lnb_power.lvl   = TBSECP3_GPIODEF_LOW,
				.gpio.lnb_power.nr    = TBSECP3_GPIO_PIN(0, 2),
				.gpio.lnb_voltage.lvl = TBSECP3_GPIODEF_HIGH,
				.gpio.lnb_voltage.nr  = TBSECP3_GPIO_PIN(0, 1),
			},
		}
	},	
	[TBSECP3_BOARD_TBS6590] = {
		.board_id	= TBSECP3_BOARD_TBS6590,
		.name		= "TurboSight TBS 6590 DVB-S/S2/S2X/T/T2/C/C2/ISDB-T + 2xCI ",
		.adapters	= 2,
		.i2c_speed	= 39,
		.eeprom_i2c	= 0,
		.eeprom_addr = 0xa0,
		.adap_config	= {
			{
				.ts_in = 0,
				.i2c_bus_nr = 0,
				.gpio.demod_reset.lvl = TBSECP3_GPIODEF_LOW,
				.gpio.demod_reset.nr  = TBSECP3_GPIO_PIN(0, 0),
				.gpio.lnb_power.lvl   = TBSECP3_GPIODEF_LOW,
				.gpio.lnb_power.nr    = TBSECP3_GPIO_PIN(0, 2),
				.gpio.lnb_voltage.lvl = TBSECP3_GPIODEF_HIGH,
				.gpio.lnb_voltage.nr  = TBSECP3_GPIO_PIN(0, 1),
			},
			{
				.ts_in = 1,
				.i2c_bus_nr = 0,
				.gpio.lnb_power.lvl   = TBSECP3_GPIODEF_LOW,
				.gpio.lnb_power.nr    = TBSECP3_GPIO_PIN(1, 2),
				.gpio.lnb_voltage.lvl = TBSECP3_GPIODEF_HIGH,
				.gpio.lnb_voltage.nr  = TBSECP3_GPIO_PIN(1, 1),
			},
		}
	},
	[TBSECP3_BOARD_TBS6290SE] = {
		.board_id	= TBSECP3_BOARD_TBS6290SE,
		.name		= "TurboSight TBS 6290SE DVB-T/T2/C + 2xCI ",
		.i2c_speed	= 39,
		.eeprom_i2c	= 1,
		.adapters	= 2,
		.adap_config	= {
			{
				.ts_in = 0,
				.i2c_bus_nr = 0,
				.gpio.demod_reset.lvl = TBSECP3_GPIODEF_NONE,
				.gpio.demod_reset.nr  = TBSECP3_GPIO_PIN(0, 0),

			}, 
			{
				.ts_in = 1,
				.i2c_bus_nr = 1,
				.gpio.demod_reset.lvl = TBSECP3_GPIODEF_NONE,
				.gpio.demod_reset.nr  = TBSECP3_GPIO_PIN(2, 0),

			},
		}
	},	
	[TBSECP3_BOARD_TBS6281SE] = {
		.board_id	= TBSECP3_BOARD_TBS6281SE,
		.name		= "TurboSight TBS 6281SE DVB-T/T2/C ",
		.i2c_speed	= 39,
		.eeprom_i2c	= 1,
		.adapters	= 2,
		.adap_config	= {
			{
				.ts_in = 1,
				.i2c_bus_nr = 1,
				.gpio.demod_reset.lvl = TBSECP3_GPIODEF_LOW,
				.gpio.demod_reset.nr  = TBSECP3_GPIO_PIN(1, 0),
			}, 
			{
				.ts_in = 2,
				.i2c_bus_nr = 2,
				.gpio.demod_reset.lvl = TBSECP3_GPIODEF_LOW,
				.gpio.demod_reset.nr  = TBSECP3_GPIO_PIN(2, 0),
			},
		}
	},
	[TBSECP3_BOARD_TBS6909X] = {
		.board_id	= TBSECP3_BOARD_TBS6909X,
		.name		= "TurboSight TBS 6909x (Octa DVB-S/S2/S2X)",
		.adapters	= 8,
		.eeprom_i2c	= 0,
		.eeprom_addr	= 0x10,
		.adap_config	= {
			{
				.ts_in = 0,
				.i2c_bus_nr = 0,
				.gpio.demod_reset.lvl = TBSECP3_GPIODEF_LOW,
				.gpio.demod_reset.nr  = TBSECP3_GPIO_PIN(0, 0),
			}, 
			{
				.ts_in = 1,
				.i2c_bus_nr = 0,
			},
			{
				.ts_in = 2,
				.i2c_bus_nr = 0,
			},
			{
				.ts_in = 3,
				.i2c_bus_nr = 0,
			},
			{
				.ts_in = 4,
				.i2c_bus_nr = 0,
			},
			{
				.ts_in = 5,
				.i2c_bus_nr = 0,
			},
			{
				.ts_in = 6,
				.i2c_bus_nr = 0,
			},
			{
				.ts_in = 7,
				.i2c_bus_nr = 0,
			}
		}
	},
	[TBSECP3_BOARD_TBS6903X] = {
		.board_id	= TBSECP3_BOARD_TBS6903X,
		.name		= "TurboSight TBS 6903x (Dual DVB-S/S2/S2X)",
		.adapters	= 2,
		.eeprom_i2c	= 0,
		.eeprom_addr	= 0x10,
		.adap_config	= {
			{
				.ts_in = 0,
				.i2c_bus_nr = 0,
				.gpio.demod_reset.lvl = TBSECP3_GPIODEF_LOW,
				.gpio.demod_reset.nr  = TBSECP3_GPIO_PIN(0, 0),
				.gpio.lnb_power.lvl   = TBSECP3_GPIODEF_LOW,
				.gpio.lnb_power.nr    = TBSECP3_GPIO_PIN(0, 2),
				.gpio.lnb_voltage.lvl = TBSECP3_GPIODEF_HIGH,
				.gpio.lnb_voltage.nr  = TBSECP3_GPIO_PIN(0, 1),
			}, 
			{
				.ts_in = 1,
				.i2c_bus_nr = 0,
				.gpio.lnb_power.lvl   = TBSECP3_GPIODEF_LOW,
				.gpio.lnb_power.nr    = TBSECP3_GPIO_PIN(1, 2),
				.gpio.lnb_voltage.lvl = TBSECP3_GPIODEF_HIGH,
				.gpio.lnb_voltage.nr  = TBSECP3_GPIO_PIN(1, 1),
			},
		}
	},
	
	[TBSECP3_BOARD_TBS6912] = {
		.board_id	= TBSECP3_BOARD_TBS6912,
		.name		= "TurboSight TBS 6912 (Dual DVB-S/S2/S2X CI card)",
		.adapters	= 2,
		.eeprom_i2c = 0,
		.eeprom_addr	= 0x10,
		.adap_config	= {
			{
				.ts_in = 0,
				.i2c_bus_nr = 0,
				.gpio.demod_reset.lvl = TBSECP3_GPIODEF_LOW,
				.gpio.demod_reset.nr  = TBSECP3_GPIO_PIN(0, 0),
				.gpio.lnb_power.lvl   = TBSECP3_GPIODEF_LOW,
				.gpio.lnb_power.nr    = TBSECP3_GPIO_PIN(0, 2),
				.gpio.lnb_voltage.lvl = TBSECP3_GPIODEF_HIGH,
				.gpio.lnb_voltage.nr  = TBSECP3_GPIO_PIN(0, 1),
			}, 
			{
				.ts_in = 1,
				.i2c_bus_nr = 0,
				.gpio.lnb_power.lvl   = TBSECP3_GPIODEF_LOW,
				.gpio.lnb_power.nr    = TBSECP3_GPIO_PIN(1, 2),
				.gpio.lnb_voltage.lvl = TBSECP3_GPIODEF_HIGH,
				.gpio.lnb_voltage.nr  = TBSECP3_GPIO_PIN(1, 1),
			},

		}
	},
};

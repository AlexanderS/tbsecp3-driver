/* 
* This file is part of STiD135 OXFORD LLA 
* 
* Copyright (c) <2014>-<2018>, STMicroelectronics - All Rights Reserved 
* Author(s): Mathias Hilaire (mathias.hilaire@st.com), Thierry Delahaye (thierry.delahaye@st.com) for STMicroelectronics. 
* 
* License terms: BSD 3-clause "New" or "Revised" License. 
* 
* Redistribution and use in source and binary forms, with or without 
* modification, are permitted provided that the following conditions are met: 
* 
* 1. Redistributions of source code must retain the above copyright notice, this 
* list of conditions and the following disclaimer. 
* 
* 2. Redistributions in binary form must reproduce the above copyright notice, 
* this list of conditions and the following disclaimer in the documentation 
* and/or other materials provided with the distribution. 
* 
* 3. Neither the name of the copyright holder nor the names of its contributors 
* may be used to endorse or promote products derived from this software 
* without specific prior written permission. 
* 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE 
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
* 
*/ 
/** 
file i2c.h
*/
#ifndef H_I2C
#define H_I2C

	#include "gen_types.h"

	/* exported definitions */
	typedef enum
	{
		I2C_ERR_NONE = 0,	/* I2C transmission OK */
		I2C_ERR_PARAM,                                                                      
		I2C_ERR_HANDLE,
		I2C_ERR_SDA,		/* SDA line is stucked (block)*/
		I2C_ERR_SCL,		/* SCL line is stucked */
		I2C_ERR_BUS,		/* SDA and SCL are stucked */
		I2C_ERR_ACK,		/* SDA and SCL lines are OK but the device does not acknowledge */
		I2C_ERR_COMM
		
	} I2C_RESULT;
	
	typedef enum
	{
		I2C_READ,		/* Read from slave */
		I2C_WRITE,		/* Write to slave */
		I2C_WRITENOSTOP /* Write to slave without stop */   
	} I2C_MODE;

	/*	exported functions	*/
	extern I2C_RESULT	I2cReadWrite(void *pI2CHost, I2C_MODE mode, u8 ChipAddress,unsigned char *Data,int NbData);	/* Read or write bytes */
#endif
	
/* End of i2c.h */

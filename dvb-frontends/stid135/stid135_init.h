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
#ifndef STID135_INIT_H
	#define STID135_INIT_H
	
	#include "chip.h"
	#include "gen_types.h"

	#define DEMOD_NBINSTANCES 76

	#define DEMOD_NBREGS		7953
	#define AFE_NBREGS			88	/* AFE registers */
	#define STiD135_NBREGS		(DEMOD_NBREGS + AFE_NBREGS)	/* Nb of DEMOD+AFE registers */
	#define STiD135_NBSOCREGS	757	/* SOC registers, excluding AFE registers */
	#define STiD135_NBFIELDS	15479	/* not true, to be recalculated */

	#define DEMOD_START_ADDR  0x100
	#define AFE_START_ADDR 0x9000
	#define AFE_END_ADDR 0x90F2
	#define SOC_START_ADDR 0x8000
	#define SOC_END_ADDR 0xF838
	#define DEMOD_END_ADDR 0x5536
	
	/* functions --------------------------------------------------------------- */
	
	/* create instance of STC register mappings */
	STCHIP_Error_t STiD135_Init(Demod_InitParams_t *InitParams,
					STCHIP_Handle_t *hChip);
	STCHIP_Error_t STiD135_SOC_Init(Demod_InitParams_t *InitParams,
					STCHIP_Handle_t *hChipHandle);
#endif


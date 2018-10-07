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
 #ifndef H_FESAT_PRO_ADVANCE
	#define H_FESAT_PRO_ADVANCE
	
	#include "fesat_commlla_str.h"
	
	/*************************************************************************
		This header file contain all SAT advanced applications strcutures and enum
	**************************************************************************/
#define NBTOTMODCODES 232 // 114 in DVBS2 only, not S2X

#define NORMALFRAMESIZE 	64800
#define SHORTFRAMESIZE		16200
#define MAXDATAFLOWSINGLE	135   

/*******************
	Enumerations
********************/

enum fe_sat_str_type
{
	FE_SAT_GENERIC_PACKETIZED_STREAM,
	FE_SAT_GENERIC_CONTINUOUS_STREAM,
	FE_SAT_GENERIC_ENCAPSULATED_STREAM, //before: FE_SAT_RESERVED_STREAM
	FE_SAT_TRANSPORT_STREAM
	
};

enum fe_sat_inp_str
{
	FE_SAT_MULTIPLE_INPUT_STREAM,	 /*MIS*/
	FE_SAT_SINGLE_INPUT_STREAM		 /*SIS*/
	
};

enum fe_sat_coding_modulation
{
	FE_SAT_ACM,						/*Adpatative Coding and Modulation*/
	FE_SAT_CCM						/*Constant Coding and Modulation*/
};
	

enum fe_sat_inp_str_synchro
{
	FE_SAT_ISSYI_DISABLED,						/*ISSYI active*/
	FE_SAT_ISSYI_ENABLED						/*ISSYI activated not active*/
};
	

enum fe_sat_null_pack_del
{
	FE_SAT_NPD_DISABLED,					/*Null-packet deletion active	 */
	FE_SAT_NPD_ENABLED						/*Null-packet deletion not active*/
};

typedef enum
{								
	FE_SAT_LDPC_ASAP,
	FE_SAT_LDPC_BROADCAST
} FE_Sat_LDPCmode_t;

typedef enum
{								
	FE_SAT_SYNCHRO_ENABLED,
	FE_SAT_SYNCHRO_DISABLED
} FE_Sat_Synchro_t;




/*******************
	Struct
********************/

struct fe_sat_dvbs2_matype_info_t
{
	enum fe_sat_str_type 		StreamType;		     /* Transport Stream Input or Generic Stream Input (packetized or continuous).*/
	enum fe_sat_inp_str	 	InputStream;	     /* Single Input Stream or Multiple Input Stream. */
	enum fe_sat_coding_modulation	CodingModulation;	 /* Constant Coding and Modulation or Adaptive Coding and Modulation (VCM is signalled as ACM).*/
	enum fe_sat_inp_str_synchro	InputStreamSynchro;  /* (Input Stream Synchronization Indicator): If ISSYI = 1 = active, the ISSY field is inserted after UPs */
	enum fe_sat_null_pack_del 	NullPacketdeletion;  /* Null-packet deletion active/not active. */ 
	enum fe_sat_rolloff			roll_off;			 /* Transmission Roll-off factor */
	
};
struct fe_sat_dvbs2_mode_t 
{
	enum fe_sat_modcode mod_code;
	enum fe_sat_pilots pilots;
	enum fe_sat_frame frame_length;
};

struct fe_sat_dvbs2_mode_inventory_t
{
	u8 mode_num;
	u32 mode_field; 
	u8 mode_mask;
};
	
	
struct fe_sat_isi_struct_t
{
	u32 nb_isi;
	u8 isi[256];
};


#endif

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
#ifndef H_OXFORD_FUNC
	#define H_OXFORD_FUNC

/*******************************************************************************
   AFE specific define
*******************************************************************************/
#define PLL_FVCO_FREQUENCY	   6200 /*design spec*/

/*******************************************************************************
   AFE specific typedefs declaration
*******************************************************************************/
   typedef enum
   {
	   AFE_TUNER1=1,
	   AFE_TUNER2,
	   AFE_TUNER3,
	   AFE_TUNER4,
	   AFE_TUNER_ALL
   }
   FE_OXFORD_TunerPath_t;
  typedef enum
   {
	   AFE_XTAL_25MHz=0,
	   AFE_XTAL_27MHz,
	   AFE_XTAL_30MHz,
	   AFE_XTAL_50MHz
   }
   FE_OXFORD_XtalValue_t;
   
   typedef enum
   {
	   DISABLE_PATH_I=0,
	   ENABLE_PATH_I,
	   DISABLE_PATH_Q,
	   ENABLE_PATH_Q
   }
   FE_OXFORD_IQ_TEST_PATH_t;
   
   

/*******************************************************************************
   AFE specific Public functions declaration
   These functions should be called from application
*******************************************************************************/
  	STCHIP_Error_t 	Oxford_AfeInit(			STCHIP_Handle_t 		hTuner,
								   			U32					 	Fxtal_MHz,
								 			U8 						InternalDCDC,
								  			U8 						InternalLDO);
	STCHIP_Error_t 	Oxford_AfeTerm(			STCHIP_Handle_t 		hTuner);
	STCHIP_Error_t 	Oxford_EnableLO(		STCHIP_Handle_t 		hTuner, 
											FE_OXFORD_TunerPath_t 	TunerNb);
	STCHIP_Error_t 	Oxford_TunerStartUp(	STCHIP_Handle_t 		hTuner, 
											FE_OXFORD_TunerPath_t 	TunerNb);
	STCHIP_Error_t 	Oxford_TunerDisable(	STCHIP_Handle_t 		hTuner, 
											FE_OXFORD_TunerPath_t 	TunerNb);
	STCHIP_Error_t 	Oxford_AdcStartUp(		STCHIP_Handle_t 		hTuner, 
											FE_OXFORD_TunerPath_t 	TunerNb);
	STCHIP_Error_t 	Oxford_AdcStop(			STCHIP_Handle_t 		hTuner, 
											FE_OXFORD_TunerPath_t 	TunerNb);
	STCHIP_Error_t 	Oxford_Disec_Lnb_FskStartup(	STCHIP_Handle_t hTuner);
	STCHIP_Error_t 	Oxford_SetFxtalDivider(	STCHIP_Handle_t 		hTuner, 
										   	U32						Fxtal_MHz);
	STCHIP_Error_t	Oxford_SetClkDcdcDivider(STCHIP_Handle_t 		hTuner,
											U32					 	Fxtal_MHz);
	
	  /* only gives access to FSK DISEQC LNBCTRL registers. 
	 There is absolutely no FSK, Diseq or LNBCTRL drivers  in this code */
	U8				Oxford_GetFSK(			STCHIP_Handle_t 		hTuner);
	STCHIP_Error_t 	Oxford_SetFSK(			STCHIP_Handle_t 		hTuner,
									  		U8	 					FSKctrl);
	U8	 			Oxford_GetDiseqC(		STCHIP_Handle_t 		hTuner);
	STCHIP_Error_t 	Oxford_SetDiseqC(		STCHIP_Handle_t 		hTuner,
									  		U8 						DiseqC);
	U8	 			Oxford_GetLnbCtrl(		STCHIP_Handle_t 		hTuner);
	STCHIP_Error_t 	Oxford_SetLnbCtrl(		STCHIP_Handle_t 		hTuner,
									  		U8 						LnbCtrl);
	/* BB filter calibration */
	STCHIP_Error_t 	Oxford_StartBBcal   (	STCHIP_Handle_t 		hTuner, 
											FE_OXFORD_TunerPath_t 	TunerNb,
											U8 						IsFromReg);
	STCHIP_Error_t Oxford_StopBBcal   (		STCHIP_Handle_t 		hTuner, 
											FE_OXFORD_TunerPath_t 	TunerNb);

/*******************************************************************************
   AFE specific Private functions declaration
   These functions are internal and must not be called directly from application
*******************************************************************************/
	U32 			RoundU32DividerX10(		U32 					X);
	STCHIP_Error_t 	Oxford_SetFvco_MHz(		STCHIP_Handle_t 		hTuner,
									   		U32 					Fvco_MHz,
									 		U32						Fxtal_MHz);
	U32 			Oxford_GetFvco_MHz(		STCHIP_Handle_t 		hTuner, 
											U32 					Fxtal_MHz);
	U32 			Oxford_GetFadc_MHz(		STCHIP_Handle_t 		hTuner,
											U32 					Fvco_MHz,
							 				U32						Oxford_GetFadc_MHz);
	STCHIP_Error_t 	Oxford_PowerUp(			STCHIP_Handle_t 		hTuner,
								 			U32 					Fxtal_MHz);
	STCHIP_Error_t Oxford_PowerManagement(	STCHIP_Handle_t 		hTuner,
											U8						InternalDCDC,
											U8						InternalLDO);
	STCHIP_Error_t Oxford_CheckPowerStatus(	STCHIP_Handle_t 		hTuner,
											U8						InternalDCDC,
											U8						InternalLDO);
	
	U8 				Oxford_LoopExitCondition(U8 					Internal_DCDC, 
											U8 						Internal_LDO, 
											U8 						DCDCok, 
											U8 						LDOok );
	STCHIP_Error_t 	Oxford_ClockStartUp(	STCHIP_Handle_t 		hTuner,
									  		U32						Fxtal_MHz);
	STCHIP_Error_t 	Oxford_CalVCOfilter(	STCHIP_Handle_t 		hTuner);
	STCHIP_Error_t  Oxford_AfeBiasingCal(	STCHIP_Handle_t 		hTuner);
	U32 			Oxford_GetPLLfrac(		STCHIP_Handle_t 		hTuner);
	STCHIP_Error_t 	Oxford_SetPLLfrac(		STCHIP_Handle_t 		hTuner,
									  		U32 					FracDiv);
	U32 			Oxford_GetPLLinteger(	STCHIP_Handle_t 		hTuner);
	STCHIP_Error_t 	Oxford_SetPLLinteger(	STCHIP_Handle_t 		hTuner,
									 		U32 					IntDiv);
	STCHIP_Error_t 	Oxford_SetPLLtcp( 		STCHIP_Handle_t 		Handle, 
											U8	 					PLLcp);

	U32 			Oxford_GetPLLrefDiv(	STCHIP_Handle_t 		hTuner);
	STCHIP_Error_t 	Oxford_SetPLLrefDiv(	STCHIP_Handle_t 		hTuner,
											U32 					RefDiv);
	U32 			Oxford_GetPLLsdOn(		STCHIP_Handle_t 		hTuner);
	STCHIP_Error_t 	Oxford_SetPLLsdOn(		STCHIP_Handle_t 		hTuner,
									  		U32 					SdOn);
	U8 				Oxford_GetPLLlock(		STCHIP_Handle_t 		hTuner);
	STCHIP_Error_t 	Oxford_SetBBgain(		STCHIP_Handle_t 		hTuner, 
											FE_OXFORD_TunerPath_t 	TunerNb, 
											U8 						Value   );
	STCHIP_Error_t 	Oxford_SetBBgainMode(	STCHIP_Handle_t 		hTuner, 
											FE_OXFORD_TunerPath_t 	TunerNb, 
											U8 						Value   );
	STCHIP_Error_t 	Oxford_SetRFgain(		STCHIP_Handle_t 		hTuner, 
											FE_OXFORD_TunerPath_t 	TunerNb, 
											U8 						Value   );
	STCHIP_Error_t 	Oxford_SetRFgainMode(	STCHIP_Handle_t 		hTuner, 
											FE_OXFORD_TunerPath_t 	TunerNb, 
											U8 						Value   ); 
	STCHIP_Error_t 	Oxford_SetVGLNAgainMode(STCHIP_Handle_t 		hTuner, 
											FE_OXFORD_TunerPath_t 	TunerNb, 
											U8 						Value   );
	U8			 	Oxford_GetVGLNAgainMode(STCHIP_Handle_t 		hTuner, 
											FE_OXFORD_TunerPath_t 	TunerNb );
	STCHIP_Error_t 	Oxford_SetVCOcalMode(	STCHIP_Handle_t 		hTuner, 
											U8 						Value   );
	STCHIP_Error_t 	Oxford_SetVCOcalStart(	STCHIP_Handle_t 		hTuner, 
											U8 						Value   );
	STCHIP_Error_t 	Oxford_SetVCOfreqCalCode(STCHIP_Handle_t 		hTuner, 
											U8 						Value   );
	U8 				Oxford_GetVCOfreqCalCode(STCHIP_Handle_t 		hTuner  );  
	STCHIP_Error_t 	Oxford_BBoutputAccess(	STCHIP_Handle_t 		hTuner, 
											FE_OXFORD_TunerPath_t 	TunerNb );
    STCHIP_Error_t 	Oxford_ResetPll(		STCHIP_Handle_t 		hTuner );
	STCHIP_Error_t Oxford_SetVglnaInputs(	STCHIP_Handle_t 		hTuner,
											U8						IsDifferential);
	U8			 	Oxford_TunerState(		STCHIP_Handle_t 		hTuner, 
											FE_OXFORD_TunerPath_t 	TunerNb );
	STCHIP_Error_t  Oxford_StoreBBcalCode(	STCHIP_Handle_t 		hTuner, 
									 		FE_OXFORD_TunerPath_t 	TunerNb,
									 		U8 						ICalCode,
									 		U8						QCalCode) ;
	STCHIP_Error_t Oxford_GetBBcalCode(		STCHIP_Handle_t 		hTuner, 
									 		FE_OXFORD_TunerPath_t 	TunerNb,
									 		U8 						*ICode,
									 		U8						*QCode,
								     		U8						IsFromReg);
	S32 			Oxford_GetPllEnable(	STCHIP_Handle_t 		hTuner);
	STCHIP_Error_t  Oxford_GetAfeId(		STCHIP_Handle_t 		hTuner, 
									 		U8 						*CutID,
									 		U8						*FixID);
	STCHIP_Error_t Oxford_CalCodeSource(STCHIP_Handle_t	hTuner, 
				FE_OXFORD_TunerPath_t	TunerNb,
				BOOL			cal_code_source);

#endif

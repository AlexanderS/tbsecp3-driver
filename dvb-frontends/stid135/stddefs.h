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
file include <stddefs.h>
*/
/******************************************************************************
Description : Contains a number of generic type declarations and defines.
******************************************************************************/

/* Define to prevent recursive inclusion */
#ifndef __STDDEFS_H
#define __STDDEFS_H


/* Includes ---------------------------------------------------------------- */
#ifdef HOST_PC
	#include <linux/types.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Exported Types ---------------------------------------------------------- */

/* Common unsigned types */
typedef uint8_t /*unsigned char*/  U8;
typedef uint16_t /*unsigned short*/ U16;
typedef uint32_t /*unsigned int*/   U32;
typedef uint64_t /*unsigned long long*/ U64;

typedef uint8_t /*unsigned char*/  u8;
typedef uint16_t /*unsigned short*/ u16;
typedef uint32_t /*unsigned int*/   u32;
typedef uint64_t /*unsigned long long*/ u64;

/* Common signed types */
typedef int8_t /*signed char*/  S8;
typedef int16_t /*signed short*/ S16;
typedef int32_t /*signed int*/   S32;
typedef int64_t /*long long*/    S64;

typedef int8_t /*signed char*/  s8;
typedef int16_t /*signed short*/ s16;
typedef int32_t /*signed int*/   s32;
typedef int64_t /*long long*/    s64;

/* Boolean type (values should be among TRUE and FALSE constants only) */
typedef int BOOL;

/* General purpose string type */
typedef char* ST_String_t;

/* Function return error code */
typedef u32 ST_ErrorCode_t;

/* Revision structure */
typedef const char * ST_Revision_t;

/* Device name type */
#define ST_MAX_DEVICE_NAME 16  /* 15 characters plus '\0' */
typedef char ST_DeviceName_t[ST_MAX_DEVICE_NAME];


/* Exported Constants ------------------------------------------------------ */

/* BOOL type constant values */
#ifndef TRUE
    #define TRUE (1 == 1)
#endif
#ifndef FALSE
    #define FALSE (!TRUE)
#endif
/* Common driver error constants */
#define ST_DRIVER_ID   0
#define ST_DRIVER_BASE (ST_DRIVER_ID << 16)

#ifdef STFRONTEND_NUM_MAX_HANDLES
#define STFRONTEND_MAX_HANDLES  STFRONTEND_NUM_MAX_HANDLES
#else
#define STFRONTEND_MAX_HANDLES 4
#endif

	
enum
{
    ST_NO_ERROR = ST_DRIVER_BASE,
    ST_ERROR_BAD_PARAMETER,             /* Bad parameter passed       */
    ST_ERROR_NO_MEMORY,                 /* Memory allocation failed   */
    ST_ERROR_UNKNOWN_DEVICE,            /* Unknown device name        */
    ST_ERROR_ALREADY_INITIALIZED,       /* Device already initialized */
    ST_ERROR_NO_FREE_HANDLES,           /* Cannot open device again   */
    ST_ERROR_OPEN_HANDLE,               /* At least one open handle   */
    ST_ERROR_INVALID_HANDLE,            /* Handle is not valid        */
    ST_ERROR_FEATURE_NOT_SUPPORTED,     /* Feature unavailable        */
    ST_ERROR_INTERRUPT_INSTALL,         /* Interrupt install failed   */
    ST_ERROR_INTERRUPT_UNINSTALL,       /* Interrupt uninstall failed */
    ST_ERROR_TIMEOUT,                   /* Timeout occured            */
    ST_ERROR_DEVICE_BUSY,               /* Device is currently busy   */
	STFRONTEND_ERROR_I2C
};

typedef enum{
   STFRONTEND_BER_ALGO_DEFAULT ,    /* STFRONTEND_BER_ALGO_INTERNAL_BERWINDOW */
   STFRONTEND_BER_ALGO_TASK_BERWINDOW, /*Corresponds to legacy BER calc  */
   STFRONTEND_BER_ALGO_APPLI_BERWINDOW/*Corresponds to BER calc. based on API call STFxx_GetTunerInfo */
}STFRONTEND_BER_Algo_t;
typedef enum STFRONTEND_Additional_Flags_e
{
  STFRONTEND_FLAG_IF1_AGC1_USED = (1<<0),  /* IF_OUT1 and AGC_IN1 are used for both digital and analog */
  STFRONTEND_FLAG_IF2_AGC2_USED = (1 << 1),  /* IF_OUT2 and AGC_IN 2are used for both digital and analog */
  STFRONTEND_FLAG_TUNER_XTAL_SHARED = (1 << 2),  /* Tuner Xtal is shared with another tuner */
  STFRONTEND_FLAG_TUNER_CLK_EXT_REF = (1 << 3) ,/* External reference clock is used i. e if a 4MHz Xtal is shared with demodulator */
  STFRONTEND_FLAG_TUNER_CLK_XTAL_IN_OUT_ON = (1 << 4),/* XTAL is directly connected to the tuner; CLK_OUT is enabled */
  STFRONTEND_FLAG_TUNER_CLK_IN_OUT_ON = (1 << 5),/* Tuner is driven by CLK_IN and CLK_OUT is enabled */
  STFRONTEND_FLAG_TUNER_CLK_IN_ON_OUT_OFF = (1 << 6) /* Tuner is driven by CLK_IN and CLK_OUT is disabled */
  
}STFRONTEND_Additional_Flags_t;
/*****************************************************************
 TS formatting related structures & enums
*****************************************************************/
/* transport stream output mode (common) */
typedef enum
 {
  STFRONTEND_TS_MODE_DEFAULT,   /*No Action/demod uses default register settings*/
  STFRONTEND_TS_SERIAL_PUNCT_CLOCK,  /*Serial punctured clock : serial STBackend  */
  STFRONTEND_TS_SERIAL_CONT_CLOCK,  /*Serial continues clock       */
  STFRONTEND_TS_PARALLEL_PUNCT_CLOCK,  /*Parallel punctured clock : Parallel STBackend */
  STFRONTEND_TS_DVBCI_CLOCK    /*Parallel continues clock : DVBCI    */
 } STFRONTEND_TS_OutputMode_t;

/*TS SPEED/data rate*/ /*DataRate Control Supported in 900/903*/
 typedef enum
 {
  STFRONTEND_TS_DATARATECONTROL_DEFAULT,/*No Action/demod uses default register settings*/
  STFRONTEND_TS_MANUAL_SPEED,    /* TS Speed manual */
  STFRONTEND_TS_AUTO_SPEED    /* TS Speed automatic */
 } STFRONTEND_TS_DataRateControl_t;

/*TS Clock Polarity*/
 typedef enum
 {
  STFRONTEND_TS_CLOCKPOLARITY_DEFAULT,/*No Action/demod uses default register settings*/
  STFRONTEND_TS_RISINGEDGE_CLOCK,   /* TS clock rising edge  */
  STFRONTEND_TS_FALLINGEDGE_CLOCK   /* TS clock falling edge */
 } STFRONTEND_TS_ClockPolarity_t;

 /*TS Sync byte enable/disable*/
 typedef enum
 {
  STFRONTEND_TS_SYNCBYTE_DEFAULT,/*No Action/demod uses default register settings*/
  STFRONTEND_TS_SYNCBYTE_ON,    /* sync bytes are not stripped */
  STFRONTEND_TS_SYNCBYTE_OFF    /* delete TS sync byte */
 } STFRONTEND_TS_SyncByteEnable_t;


 typedef enum
 {
  STFRONTEND_TS_PARTITYBYTES_DEFAULT,/*No Action/demod uses default register settings*/
  STFRONTEND_TS_PARITYBYTES_ON,   /* All MPEG/TS bits are output*/
  STFRONTEND_TS_PARITYBYTES_OFF   /*delete TS parity bytes */
 } STFRONTEND_TS_ParityBytes_t;

/*Supported in 297E/7141/5197  only*/
 typedef enum
 {
  STFRONTEND_TS_SWAP_DEFAULT,/*No Action/demod uses default register settings*/
  STFRONTEND_TS_SWAP_ON,     /* swap TS bits LSB <-> MSB */
  STFRONTEND_TS_SWAP_OFF     /* don't swap TS bits LSB <-> MSB */
 } STFRONTEND_TS_Swap_t;

 /*Supported in 297E/7141/5197 only*/
 typedef enum
 {
  STFRONTEND_TS_SMOOTHER_DEFAULT,/*No Action/demod uses default register settings*/
  STFRONTEND_TS_SMOOTHER_ON,    /* enable TS smoother or fifo for serial ouput only*/
  STFRONTEND_TS_SMOOTHER_OFF    /* disable TS smoother or fifo for serial output only*/
 } STFRONTEND_TS_Smoother_t;

	
/* Exported Variables ------------------------------------------------------ */


/* Exported Macros --------------------------------------------------------- */


/* Exported Functions ------------------------------------------------------ */

#ifdef __cplusplus
}
#endif

#endif  /* #ifndef __STDDEFS_H */

/* End of stddefs.h */



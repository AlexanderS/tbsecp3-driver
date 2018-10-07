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
/*---------------------------------------------------------------------------- */
/* define to prevent recursive inclusion */
#ifndef H_CHIP
#define H_CHIP



//----------------------------------------  
/*       Slice Mapping Definitions   TEMPORARY WORKAROUND !!!!!!!!!!!!!!!!!!!   */
//----------------------------------------   
#define SLC_PSLICE    0xf730       // Slice Parent (Mapping) Register Adress
#define SLC_MIRROR    0xf720       // Slice Mirror Start Register Adress  
#define SLC_BASE      0xf900       // Slice Base Register Adress, start adress of slices
//#define SLC_PPAGE     0xf731       // Slice Paging (Banking) Parent Register Adress, not implemented    

#define SLC_COUNT    16            // slices;    = number of slices per memory page (=bank)
#define SLC_RSIZE    16            // registers, = size of a slice in number of registers
#define SLC_OFFSET   16            // offset of a slice to the next slice (should be: next slice adress = base adress of current slice + offset)



/* includes ---------------------------------------------------------------- */
#include "globaldefs.h"

#ifdef ST_OSLINUX
	#include "i2c_wrapper.h"
#endif

#ifdef __cplusplus
 extern "C"
 {
#endif                  /* __cplusplus */

	 
/* definitions ------------------------------------------------------------- */

/* maximum number of chips that can be opened */
#define MAXNBCHIP 10

/* maximum number of chars per chip/tuner name */
#define MAXNAMESIZE 30

/* STV0368 specific */
#define STV0368_PAGESHIFT 				0x004C
#define STV0368_HIGHEST_PROGRAM_ADDR 	0x40FFFF	/* cut 2 */


/*
 * I2C->STBus Transaction register parameter byte
 *
 *   ++------------ DATA_NB_BYTES  (number of data bytes to transmit)
 *   || +++-------- STREAM_ADD_INC (streaming address auto increment)
 *   || ||| ++----- ADD_NB_BYTES   (number of address bytes)
 *   || ||| || +--- R_not_W:       (I2C transaction type)
 *   76 543 21 0
 *
 Note: Adress increments are limited to 4K !!!!
 */

#define STBUS_WRITE      0           /*  Write mode   */   
#define STBUS_READ       1           /*  Read mode    */

#define STBUS_ADDR_1_BYTE     (0<<1) /* 1 byte of address or data provided   */
#define STBUS_ADDR_2_BYTE     (1<<1) /* 2 bytes of address or data provided  */ 
#define STBUS_ADDR_DONT_USE   (2<<1) /* do not use, repeat transaction byte  */   
#define STBUS_ADDR_4_BYTE     (3<<1) /* 4 bytes of address or data provided  */   

#define STBUS_STREAM_INCR_0   (0<<3) /*  no streaming mode                    */   
#define STBUS_STREAM_INCR_1   (1<<3) /*  address increment of 1 * DATA_BYTE   */ 
#define STBUS_STREAM_INCR_2   (2<<3) /*  address increment of 2 * DATA_BYTE   */  
#define STBUS_STREAM_INCR_4   (3<<3) /*  address increment of 4 * DATA_BYTE   */
#define STBUS_STREAM_DECR_1   (4<<3) /*  address decrement of 1 * DATA_BYTE   */
#define STBUS_STREAM_DECR_2   (5<<3) /*  address decrement of 2 * DATA_BYTE   */
#define STBUS_STREAM_DECR_4   (6<<3) /*  address decrement of 4 * DATA_BYTE   */
#define STBUS_ERROR_REGISTER  (7<<3) /*  Error register request   */

#define STBUS_DATA_1_BYTE     (0<<6) /* 1 byte  of address or data provided  */
#define STBUS_DATA_2_BYTE     (1<<6) /* 2 bytes of address or data provided  */ 
#define STBUS_DATA_RESERVED   (2<<6) /* do not use, repeat transaction byte  */   
#define STBUS_DATA_4_BYTE     (3<<6) /* 4 bytes of address or data provided  */

// specific STBUS mode definitions for defaults
#define STCHIP_MODE_STBUS_A2D1S0 STBUS_ADDR_2_BYTE+STBUS_STREAM_INCR_0+STBUS_DATA_1_BYTE // 2 AdressBytes, 1 Databyte,  + 0 Adress
#define STCHIP_MODE_STBUS_A2D4S0 STBUS_ADDR_2_BYTE+STBUS_STREAM_INCR_0+STBUS_DATA_4_BYTE // 2 AdressBytes, 4 Databytes, + 0 Adress  
	 
#define STCHIP_MODE_STBUS_A2D1S1 STBUS_ADDR_2_BYTE+STBUS_STREAM_INCR_1+STBUS_DATA_1_BYTE // 2 AdressBytes, 1 Databyte,  + 1 Adress
#define STCHIP_MODE_STBUS_A2D4S1 STBUS_ADDR_2_BYTE+STBUS_STREAM_INCR_1+STBUS_DATA_4_BYTE // 2 AdressBytes, 4 Databytes, + 1 Adress  
#define STCHIP_MODE_STBUS_ERROR STBUS_ERROR_REGISTER                                     // fetch the Error register  	 

/* enumerations------------------------------------------------------------- */

#ifdef HOST_PC
	/* access modes for fields and registers */
	typedef enum
	{
		STCHIP_ACCESS_WR = 0,  	/* can be read and written */
		STCHIP_ACCESS_R,   	/* only be read from */
		STCHIP_ACCESS_W,   	/* only be written to */
		STCHIP_ACCESS_NON 	/* cannot be read or written (guarded register, e.g. register skipped by ChipApplyDefaultValues() etc.) */
	} STCHIP_Access_t;
#endif


/* +gb+ slice stuff, ------------------------------------------- */

#define isREGISTER 0x00
#define isPARENT   0x01
#define isMIRROR   0x02
#define isBASE     0x03

typedef enum
{
	STCHIP_ACCESS_REGISTER	= 0x00,	/* normal register         */
	STCHIP_ACCESS_PARENT	= 0x10, /* parent for hosting bank */  
	STCHIP_ACCESS_MIRROR	= 0x20, /* mirror register         */
	STCHIP_ACCESS_BASE	= 0x30	/* flat Base register, mirrored according to parent */
} STCHIP_SliceAccess_t;

#ifdef HOST_PC
	/* register's available size */
	typedef enum
	{
		STCHIP_REGSIZE_8	=1, /*  8 bits */
		STCHIP_REGSIZE_16	=2, /* 16 bits */
		STCHIP_REGSIZE_24	=3, /* 24 bits */
		STCHIP_REGSIZE_32	=4, /* 32 bits */
		STCHIP_REGSIZE_64	=8  /* 64 bits */
	}
	STCHIP_RegSize_t;
	
	
	/* register field type */
	typedef enum
	{
		CHIP_UNSIGNED,
		CHIP_SIGNED
	}
	STCHIP_FieldType_t;
	
	
	/* error codes */
	
	/* PJ/TA 12/2009: this enum must be in line with TUNER_Error_t in stfe_utilities.h */
	typedef enum
	{
		CHIPERR_NO_ERROR = 0x0,       /* No error encountered */
		CHIPERR_INVALID_HANDLE = 0x1,     /* Using of an invalid chip handle */
		CHIPERR_INVALID_REG_ID = 0x2,     /* Using of an invalid register */
		CHIPERR_INVALID_FIELD_ID = 0x4,   /* Using of an Invalid field */
		CHIPERR_INVALID_FIELD_SIZE = 0x8, /* Using of a field with an invalid size */
		CHIPERR_I2C_NO_ACK = 0x10,         /* No acknowledge from the chip */
		CHIPERR_I2C_BURST = 0x20,          /* Two many registers accessed in burst mode */
		CHIPERR_TYPE_ERR = 0x40,
		CHIPERR_CONFIG_ERR = 0x80
		
	}
	STCHIP_Error_t;
	
	
	/* how to access I2C bus */
	typedef enum
	{
		STCHIP_MODE_I2C2STBUS,      /* <trans><addr><addr...><data><data...>  (e.g. oxford stbus bridge, fixed transaction) */
		STCHIP_MODE_SUBADR_8,       /* <addr><reg8><data><data>        (e.g. demod chip) */
		STCHIP_MODE_SUBADR_16,      /* <addr><reg8><data><data>        (e.g. demod chip) */    
		STCHIP_MODE_NOSUBADR,       /* <addr><data>|<data><data><data> (e.g. tuner chip) */
		STCHIP_MODE_NOSUBADR_RD,	/* <addr><data>|<data><data><data> (e.g. tuner chip) only for read */
	}
	STCHIP_Mode_t;
#endif

/* structures -------------------------------------------------------------- */
#ifdef HOST_PC
	/* register information */
	typedef struct
	{
		u16	Addr;     		 /* Address               */
		//u8	Value;    		 /* Current value         */
		/* for 32-bit SOC registers */
		u32	Value;    		 /* Current value         */
		//u16	Attr;     /* tag for slices */
		STCHIP_RegSize_t Size;	         /* register size         */
	}
	STCHIP_Register_t;
	
	
	/* register field information */
	typedef struct
	{
	u16			Reg;      /* Register index */
	unsigned char	Pos;      /* Bit position */
	unsigned char	Bits;     /* Bit width */
	u32			Mask;     /* Mask compute with width and position */
	STCHIP_FieldType_t	Type;     /* Signed or unsigned */
	char		Name[48]; /* Name */
	}
	STCHIP_Field_t;
	
	typedef struct
	{
		u16	RegIdx;     /* Register reference index */
		u32	FldIdx;     /* Field reference index */
	}
	STCHIP_InstIndex_t;
	
	typedef struct
	{
		u16			Path;           /* Path index               */
		STCHIP_InstIndex_t	Index[16];		/* 1st Register/Field index */
	}
	STCHIP_Instance_t;
	
	/* data about a specific chip */
	typedef struct stchip_Info_t
	{
		void			*pI2CHost;
		unsigned char		I2cAddr;          /* Chip I2C address */
		char			Name[MAXNAMESIZE];/* Name of the chip */
		u32			NbInsts;          /* Number of instances in the chip */  
		int			NbRegs;           /* Number of registers in the chip */
		u32			NbFields;         /* Number of fields in the chip */
		STCHIP_Register_t	*pRegMapImage;     /* Pointer to register map */
		STCHIP_Field_t		*pFieldMapImage;   /* Pointer to field map */
		STCHIP_Instance_t	*pInstMap;         /* Pointer to an Instances Map */
		STCHIP_Error_t		Error;            /* Error state */
		STCHIP_Mode_t		ChipMode;         /* Access bus in demod (SubAdr) or tuner (NoSubAdr) mode */
		u8			ChipID;           /* Chip cut ID */				
	
	#if defined(HOST_PC)					  /* PC specific parameters */
		BOOL                Repeater;         /* Is repeater enabled or not ? */
		struct stchip_Info_t *RepeaterHost;   /* Owner of the repeater */
		STCHIP_Error_t 		(*RepeaterFn)(struct stchip_Info_t *hChip,BOOL State);    /* Pointer to repeater routine */ 
		
		/* Parameters needed for non sub address devices */
		u32	WrStart;		  /* Id of the first writable register */	
		u32	WrSize;           /* Number of writable registers */	
		u16	RdStart;		  /* Id of the first readable register */
		u32	RdSize;			  /* Number of readable registers */
		s32	LastRegIndex;	  /* Last accessed register index in the register map Image */
		BOOL	Abort;			  /* Abort flag when set to on no register access and no wait  are done  */ 
	
	#endif
	
		void	*pData;			  /* pointer to chip data */
		u8	TunerNb;		  /* number of tuner from 0 to 3, field added to match Oxford */
	}
	STCHIP_Info_t;
	
	
	typedef STCHIP_Info_t *STCHIP_Handle_t;  /* Handle to a chip */
	typedef STCHIP_Info_t *tuner_handle;   /* ?? */    
	
	typedef STCHIP_Error_t (*STCHIP_RepeaterFn_t)(STCHIP_Handle_t hChip,BOOL State);    /* Pointer to repeater routine */ 
	
	typedef struct
	{
		STCHIP_Info_t	*Chip;     	/* pointer to parameters to pass to the CHIP API */
		u32		NbDefVal; 	    /* number of default values (must match number of STC registers) */
	} 
	Demod_InitParams_t;
#endif

/* functions --------------------------------------------------------------- */

/* Creation and destruction routines */
STCHIP_Handle_t	ChipOpen(STCHIP_Info_t *hChipOpenParams);
STCHIP_Error_t  ChipAddRegDescription  (STCHIP_Handle_t hChip, u16 RegId, char *Description);
STCHIP_Error_t  ChipAddReg             (STCHIP_Handle_t hChip, u16 RegId, char *Name, u16 Address, u32 Default, STCHIP_Access_t Access, ...);    

STCHIP_Error_t	ChipAddField    (STCHIP_Handle_t hChip, u16 RegId, u32 FieldId, char *Name,   char Pos, char NbBits, STCHIP_FieldType_t Type);
STCHIP_Error_t	ChipClose       (STCHIP_Handle_t hChip);

/* Utility routines */
STCHIP_Handle_t	ChipGetFirst(void);
STCHIP_Handle_t	ChipGetNext(STCHIP_Handle_t hPrevChip);
STCHIP_Handle_t	ChipGetHandleFromName(char *Name);
s32 ChipGetRegisterIndex(STCHIP_Handle_t hChip, u16 RegId);
s32 ChipGetFieldIndex(STCHIP_Handle_t hChip, u32 FieldId);

STCHIP_Error_t	ChipChangeAddress(STCHIP_Handle_t hChip, unsigned char I2cAddr);
STCHIP_Error_t	ChipSetRepeater(STCHIP_Handle_t hChip,BOOL Repeater);

STCHIP_Error_t  ChipUpdateDefaultValues(STCHIP_Handle_t hChip,STCHIP_Register_t  *pRegMap);
STCHIP_Error_t	ChipApplyDefaultValues(STCHIP_Handle_t hChip, u16 RegAddr, u8 RegsVal);  /* reset */
#ifdef HOST_PC
	int	ChipCheckAck(STCHIP_Handle_t hChip);
#endif

STCHIP_Error_t	ChipGetError(STCHIP_Handle_t hChip);
STCHIP_Error_t	ChipResetError(STCHIP_Handle_t hChip);

int		ChipGetFieldIdFromName(STCHIP_Handle_t hChip, char *FieldName);
u16		ChipGetRegIdFromName(STCHIP_Handle_t hChip, char *RegName);
u16		ChipGetRegIdFromAdd(STCHIP_Handle_t hChip,u16 Add);
unsigned char	ChipGetFieldBitsFromId(STCHIP_Handle_t hChip, u32 FieldId);
char *		ChipGetFieldNameFromId(STCHIP_Handle_t hChip, u32 FieldId);
u16 		ChipGetFieldRegisterAddrFromId(STCHIP_Handle_t hChip, u16 FieldId);
u16		ChipGetRegisterAddrFromId(STCHIP_Handle_t hChip, u16 RegId);

#ifdef HOST_PC
	/* Access routines */
	/* FIXME: new for 32-bit access */
	STCHIP_Error_t ChipSetOneRegister(STCHIP_Handle_t hChip,u16 RegAddr,u32 Data);
	STCHIP_Error_t ChipGetOneRegister(STCHIP_Handle_t hChip, u16 RegAddr, u32* data_p );
	
	
	STCHIP_Error_t	ChipSetRegisters(STCHIP_Handle_t hChip, u16 FirstReg, s32 NbRegs);
	STCHIP_Error_t	ChipGetRegisters(STCHIP_Handle_t hChip, u16 FirstReg, s32 NbRegs);
#endif

STCHIP_Error_t	ChipSetField(STCHIP_Handle_t hChip, u32 FieldId, s32 Value);

#ifdef HOST_PC
	STCHIP_Error_t	ChipGetField(STCHIP_Handle_t hChip, u32 FieldId, s32* value_p);
#endif

STCHIP_Error_t	ChipSetFieldImage(STCHIP_Handle_t hChip, u32 FieldId, s32 Value);
s32				ChipGetFieldImage(STCHIP_Handle_t hChip, u32 FieldId);

STCHIP_Error_t	ChipSetRegDefault(STCHIP_Handle_t hChip, u16 RegId, u32 Default);
u32				ChipGetRegDefault(STCHIP_Handle_t hChip, u16 RegId);

STCHIP_Error_t	ChipSetFieldDefault(STCHIP_Handle_t hChip, u16 FieldId,s32 DefValue);
s32				ChipGetFieldDefault(STCHIP_Handle_t hChip, u16 FieldId);

void	ChipWaitOrAbort(STCHIP_Handle_t hChip,u32 delay_ms);
#ifdef HOST_PC
	void	ChipAbort(STCHIP_Handle_t hChip,BOOL Abort);
#endif

/* Added for slice management */
// STCHIP_Error_t  ChipAddParent (STCHIP_Handle_t hChip,u16 RegId, char *Name, u16 Addr, u8 Default, STCHIP_Access_t Access);
STCHIP_Error_t  ChipExpandRegister(STCHIP_Handle_t hChip, u16 RegId, char *Name, u16 BaseAddr,u8 Default, STCHIP_Access_t Access, u16 Fields, char *InstanceName);
STCHIP_Error_t  ChipExpandField   (STCHIP_Handle_t hChip, u16 RegId, u32 FieldId, char * Name, char Pos, char NbBits, STCHIP_FieldType_t Type);

STCHIP_Error_t	ChipSetPagedRegister(STCHIP_Handle_t hChip,u16 RegId, u32 Value, u16 Slice);
STCHIP_Error_t  ChipSetPagedField   (STCHIP_Handle_t hChip,u32 FieldId,int Value, u8 Slice);

u32 CreateMask(char NbBits, char Pos);

u8  ChipGetPagedRegister(STCHIP_Handle_t hChip, u16 RegId, u8 Slice);
int ChipGetPagedFieldImage(STCHIP_Handle_t hChip,u32 FieldId, u16 Slice);
//u16	ChipGetBaseId(STCHIP_Handle_t hChip, u16 RegId, u8 Slice); 
int  ChipGetRegType(STCHIP_Handle_t hChip, int RegId);      	/* slices, used in regmap to distinguish parent, mirror, reg ... */
void ChipSetHierarchyMap   (BOOL Flag);  /* helper function used in _init.c     							 */
void ChipSetMapRegisterSize(STCHIP_RegSize_t Regsize); 			/* helper function to set the registersize w/o the hchip handle  */

#ifdef __cplusplus
 }
#endif                  /* __cplusplus */

#endif          /* H_CHIP */

/* End of chip.h */

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
#ifndef _C8SECTP_FE_TSDMA_TOP_REG_INIT_H
#define _C8SECTP_FE_TSDMA_TOP_REG_INIT_H

/* -------------------------------------------------------------------------
 * File name  : c8sectp_fe_tsdma_top_reg_init.h
 * File type  : C header file
 * -------------------------------------------------------------------------
 * Description:  Register map constants
 * Generated by spirit2regtest v2.24_alpha3
 * -------------------------------------------------------------------------
 */


/* Register map constants */

/* C8SECTPFE_TSDMA_STRM0_BASE */
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM0_BASE          0x00000000
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM0_BASE__DEFAULT 0x0       
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM0_BASE_BUFBASE__MASK 0x00003FC0

/* C8SECTPFE_TSDMA_STRM0_TOP */
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM0_TOP           0x00000004
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM0_TOP__DEFAULT  0x3f      
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM0_TOP_BUFTOP__MASK 0x00003FC0

/* C8SECTPFE_TSDMA_STRM0_PKTSIZE */
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM0_PKTSIZE       0x00000008
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM0_PKTSIZE__DEFAULT 0xc4      
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM0_PKTSIZE_VALUE__MASK 0x000000FF

/* C8SECTPFE_TSDMA_STRM0_FORMAT */
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM0_FORMAT        0x0000000c
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM0_FORMAT__DEFAULT 0x0       
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM0_FORMAT_TAGGED__MASK 0x00000001

/* C8SECTPFE_TSDMA_STRM0_STATUS */
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM0_STATUS        0x00000018
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM0_STATUS__DEFAULT 0x1       
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM0_STATUS_IDLE__MASK 0x00000001

/* C8SECTPFE_TSDMA_STRM1_BASE */
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM1_BASE          0x00000020
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM1_BASE__DEFAULT 0x0       
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM1_BASE_BUFBASE__MASK 0x00003FC0

/* C8SECTPFE_TSDMA_STRM1_TOP */
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM1_TOP           0x00000024
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM1_TOP__DEFAULT  0x3f      
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM1_TOP_BUFTOP__MASK 0x00003FC0

/* C8SECTPFE_TSDMA_STRM1_PKTSIZE */
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM1_PKTSIZE       0x00000028
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM1_PKTSIZE__DEFAULT 0xc4      
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM1_PKTSIZE_VALUE__MASK 0x000000FF

/* C8SECTPFE_TSDMA_STRM1_FORMAT */
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM1_FORMAT        0x0000002c
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM1_FORMAT__DEFAULT 0x0       
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM1_FORMAT_TAGGED__MASK 0x00000001

/* C8SECTPFE_TSDMA_STRM1_STATUS */
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM1_STATUS        0x00000038
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM1_STATUS__DEFAULT 0x1       
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM1_STATUS_IDLE__MASK 0x00000001

/* C8SECTPFE_TSDMA_STRM2_BASE */
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM2_BASE          0x00000040
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM2_BASE__DEFAULT 0x0       
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM2_BASE_BUFBASE__MASK 0x00003FC0

/* C8SECTPFE_TSDMA_STRM2_TOP */
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM2_TOP           0x00000044
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM2_TOP__DEFAULT  0x3f      
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM2_TOP_BUFTOP__MASK 0x00003FC0

/* C8SECTPFE_TSDMA_STRM2_PKTSIZE */
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM2_PKTSIZE       0x00000048
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM2_PKTSIZE__DEFAULT 0xc4      
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM2_PKTSIZE_VALUE__MASK 0x000000FF

/* C8SECTPFE_TSDMA_STRM2_FORMAT */
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM2_FORMAT        0x0000004c
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM2_FORMAT__DEFAULT 0x0       
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM2_FORMAT_TAGGED__MASK 0x00000001

/* C8SECTPFE_TSDMA_STRM2_STATUS */
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM2_STATUS        0x00000058
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM2_STATUS__DEFAULT 0x1       
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM2_STATUS_IDLE__MASK 0x00000001

/* C8SECTPFE_TSDMA_STRM3_BASE */
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM3_BASE          0x00000060
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM3_BASE__DEFAULT 0x0       
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM3_BASE_BUFBASE__MASK 0x00003FC0

/* C8SECTPFE_TSDMA_STRM3_TOP */
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM3_TOP           0x00000064
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM3_TOP__DEFAULT  0x3f      
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM3_TOP_BUFTOP__MASK 0x00003FC0

/* C8SECTPFE_TSDMA_STRM3_PKTSIZE */
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM3_PKTSIZE       0x00000068
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM3_PKTSIZE__DEFAULT 0xc4      
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM3_PKTSIZE_VALUE__MASK 0x000000FF

/* C8SECTPFE_TSDMA_STRM3_FORMAT */
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM3_FORMAT        0x0000006c
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM3_FORMAT__DEFAULT 0x0       
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM3_FORMAT_TAGGED__MASK 0x00000001

/* C8SECTPFE_TSDMA_STRM3_STATUS */
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM3_STATUS        0x00000078
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM3_STATUS__DEFAULT 0x1       
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM3_STATUS_IDLE__MASK 0x00000001

/* C8SECTPFE_TSDMA_STRM4_BASE */
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM4_BASE          0x00000080
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM4_BASE__DEFAULT 0x0       
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM4_BASE_BUFBASE__MASK 0x00003FC0

/* C8SECTPFE_TSDMA_STRM4_TOP */
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM4_TOP           0x00000084
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM4_TOP__DEFAULT  0x3f      
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM4_TOP_BUFTOP__MASK 0x00003FC0

/* C8SECTPFE_TSDMA_STRM4_PKTSIZE */
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM4_PKTSIZE       0x00000088
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM4_PKTSIZE__DEFAULT 0xc4      
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM4_PKTSIZE_VALUE__MASK 0x000000FF

/* C8SECTPFE_TSDMA_STRM4_FORMAT */
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM4_FORMAT        0x0000008c
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM4_FORMAT__DEFAULT 0x0       
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM4_FORMAT_TAGGED__MASK 0x00000001

/* C8SECTPFE_TSDMA_STRM4_STATUS */
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM4_STATUS        0x00000098
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM4_STATUS__DEFAULT 0x1       
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM4_STATUS_IDLE__MASK 0x00000001

/* C8SECTPFE_TSDMA_STRM5_BASE */
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM5_BASE          0x000000a0
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM5_BASE__DEFAULT 0x0       
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM5_BASE_BUFBASE__MASK 0x00003FC0

/* C8SECTPFE_TSDMA_STRM5_TOP */
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM5_TOP           0x000000a4
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM5_TOP__DEFAULT  0x3f      
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM5_TOP_BUFTOP__MASK 0x00003FC0

/* C8SECTPFE_TSDMA_STRM5_PKTSIZE */
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM5_PKTSIZE       0x000000a8
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM5_PKTSIZE__DEFAULT 0xc4      
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM5_PKTSIZE_VALUE__MASK 0x000000FF

/* C8SECTPFE_TSDMA_STRM5_FORMAT */
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM5_FORMAT        0x000000ac
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM5_FORMAT__DEFAULT 0x0       
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM5_FORMAT_TAGGED__MASK 0x00000001

/* C8SECTPFE_TSDMA_STRM5_STATUS */
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM5_STATUS        0x000000b8
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM5_STATUS__DEFAULT 0x1       
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM5_STATUS_IDLE__MASK 0x00000001

/* C8SECTPFE_TSDMA_STRM6_BASE */
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM6_BASE          0x000000c0
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM6_BASE__DEFAULT 0x0       
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM6_BASE_BUFBASE__MASK 0x00003FC0

/* C8SECTPFE_TSDMA_STRM6_TOP */
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM6_TOP           0x000000c4
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM6_TOP__DEFAULT  0x3f      
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM6_TOP_BUFTOP__MASK 0x00003FC0

/* C8SECTPFE_TSDMA_STRM6_PKTSIZE */
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM6_PKTSIZE       0x000000c8
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM6_PKTSIZE__DEFAULT 0xc4      
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM6_PKTSIZE_VALUE__MASK 0x000000FF

/* C8SECTPFE_TSDMA_STRM6_FORMAT */
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM6_FORMAT        0x000000cc
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM6_FORMAT__DEFAULT 0x0       
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM6_FORMAT_TAGGED__MASK 0x00000001

/* C8SECTPFE_TSDMA_STRM6_STATUS */
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM6_STATUS        0x000000d8
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM6_STATUS__DEFAULT 0x1       
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM6_STATUS_IDLE__MASK 0x00000001

/* C8SECTPFE_TSDMA_STRM7_BASE */
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM7_BASE          0x000000e0
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM7_BASE__DEFAULT 0x0       
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM7_BASE_BUFBASE__MASK 0x00003FC0

/* C8SECTPFE_TSDMA_STRM7_TOP */
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM7_TOP           0x000000e4
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM7_TOP__DEFAULT  0x3f      
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM7_TOP_BUFTOP__MASK 0x00003FC0

/* C8SECTPFE_TSDMA_STRM7_PKTSIZE */
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM7_PKTSIZE       0x000000e8
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM7_PKTSIZE__DEFAULT 0xc4      
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM7_PKTSIZE_VALUE__MASK 0x000000FF

/* C8SECTPFE_TSDMA_STRM7_FORMAT */
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM7_FORMAT        0x000000ec
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM7_FORMAT__DEFAULT 0x0       
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM7_FORMAT_TAGGED__MASK 0x00000001

/* C8SECTPFE_TSDMA_STRM7_STATUS */
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM7_STATUS        0x000000f8
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM7_STATUS__DEFAULT 0x1       
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_STRM7_STATUS_IDLE__MASK 0x00000001

/* C8SECTPFE_TSDMA_DEST0_ROUTE */
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_DEST0_ROUTE         0x00000800
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_DEST0_ROUTE__DEFAULT 0x0       
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_DEST0_ROUTE_ENABLE__MASK 0x00000000000000FF

/* C8SECTPFE_TSDMA_DEST0_FIFOTRIG */
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_DEST0_FIFOTRIG      0x00000808
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_DEST0_FIFOTRIG__DEFAULT 0x1b      
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_DEST0_FIFOTRIG_TRIGGER__MASK 0x0000001F

/* C8SECTPFE_TSDMA_DEST0_BYPASS */
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_DEST0_BYPASS        0x0000080c
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_DEST0_BYPASS__DEFAULT 0x0       
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_DEST0_BYPASS_SELECT__MASK 0x0000000F
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_DEST0_BYPASS_ENABLE__MASK 0x80000000

/* C8SECTPFE_TSDMA_DEST0_CLKDIV */
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_DEST0_CLKDIV        0x00000810
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_DEST0_CLKDIV__DEFAULT 0x0       
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_DEST0_CLKDIV_RATIO__MASK 0x000000FF
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_DEST0_CLKDIV_DATA_DELAY__MASK 0x00FF0000

/* C8SECTPFE_TSDMA_DEST0_FORMAT */
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_DEST0_FORMAT        0x00000814
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_DEST0_FORMAT__DEFAULT 0x0       
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_DEST0_FORMAT_SERIAL_NOT_PARALLEL__MASK 0x00000001
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_DEST0_FORMAT_BYTE_ENDIANNESS__MASK 0x00000002
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_DEST0_FORMAT_REMOVE_TAG__MASK 0x00000004
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_DEST0_FORMAT_COMPLETE_BYTE__MASK 0x00000008
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_DEST0_FORMAT_SOP_FIRST_BIT__MASK 0x00000010
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_DEST0_FORMAT_WAIT_WHOLE_PACKET__MASK 0x00000020

/* C8SECTPFE_TSDMA_DEST0_STATUS */
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_DEST0_STATUS        0x00000818
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_DEST0_STATUS__DEFAULT 0x1       
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_DEST0_STATUS_IDLE__MASK 0x00000001
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_DEST0_STATUS_DMA_OVERFLOW__MASK 0x00000002

/* C8SECTPFE_TSDMA_DEST1_ROUTE */
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_DEST1_ROUTE         0x00000820
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_DEST1_ROUTE__DEFAULT 0x0       
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_DEST1_ROUTE_ENABLE__MASK 0x00000000000000FF

/* C8SECTPFE_TSDMA_DEST1_FIFOTRIG */
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_DEST1_FIFOTRIG      0x00000828
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_DEST1_FIFOTRIG__DEFAULT 0x1b      
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_DEST1_FIFOTRIG_TRIGGER__MASK 0x0000001F

/* C8SECTPFE_TSDMA_DEST1_BYPASS */
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_DEST1_BYPASS        0x0000082c
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_DEST1_BYPASS__DEFAULT 0x0       
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_DEST1_BYPASS_SELECT__MASK 0x0000000F
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_DEST1_BYPASS_ENABLE__MASK 0x80000000

/* C8SECTPFE_TSDMA_DEST1_CLKDIV */
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_DEST1_CLKDIV        0x00000830
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_DEST1_CLKDIV__DEFAULT 0x0       
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_DEST1_CLKDIV_RATIO__MASK 0x000000FF
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_DEST1_CLKDIV_DATA_DELAY__MASK 0x00FF0000

/* C8SECTPFE_TSDMA_DEST1_FORMAT */
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_DEST1_FORMAT        0x00000834
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_DEST1_FORMAT__DEFAULT 0x0       
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_DEST1_FORMAT_SERIAL_NOT_PARALLEL__MASK 0x00000001
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_DEST1_FORMAT_BYTE_ENDIANNESS__MASK 0x00000002
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_DEST1_FORMAT_REMOVE_TAG__MASK 0x00000004
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_DEST1_FORMAT_COMPLETE_BYTE__MASK 0x00000008
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_DEST1_FORMAT_SOP_FIRST_BIT__MASK 0x00000010
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_DEST1_FORMAT_WAIT_WHOLE_PACKET__MASK 0x00000020

/* C8SECTPFE_TSDMA_DEST1_STATUS */
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_DEST1_STATUS        0x00000838
#define RSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_DEST1_STATUS__DEFAULT 0x1       
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_DEST1_STATUS_IDLE__MASK 0x00000001
#define FSTID135_C8SECTPFE_TSDMA_C8SECTPFE_TSDMA_DEST1_STATUS_DMA_OVERFLOW__MASK 0x00000002


/* Number of registers */
#define STID135_C8SECTP_FE_TSDMA_TOP_REG_NBREGS                      52        

/* Number of fields */
#define STID135_C8SECTP_FE_TSDMA_TOP_REG_NBFIELDS                    68        



#endif /* #ifndef _C8SECTP_FE_TSDMA_TOP_REG_INIT_H */

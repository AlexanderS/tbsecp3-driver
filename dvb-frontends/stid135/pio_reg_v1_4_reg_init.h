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
#ifndef _PIO_REG_V1_4_REG_INIT_H
#define _PIO_REG_V1_4_REG_INIT_H

/* -------------------------------------------------------------------------
 * File name  : pio_reg_v1_4_reg_init.h
 * File type  : C header file
 * -------------------------------------------------------------------------
 * Description:  Register map constants
 * Generated by spirit2regtest v2.24_alpha3
 * -------------------------------------------------------------------------
 */


/* Register map constants */

/* POUT */
#define RSTID135_PIO_POUT                                            0x00000000
#define RSTID135_PIO_POUT__DEFAULT                                   0x0       
#define FSTID135_PIO_POUT_PIO_OUT_BITS__MASK                         0xFF      

/* POUT_SET */
#define RSTID135_PIO_POUT_SET                                        0x00000004
#define RSTID135_PIO_POUT_SET__DEFAULT                               0x0       
#define FSTID135_PIO_POUT_SET_POUT_SET__MASK                         0xFF      

/* POUT_RESET */
#define RSTID135_PIO_POUT_RESET                                      0x00000008
#define RSTID135_PIO_POUT_RESET__DEFAULT                             0x0       
#define FSTID135_PIO_POUT_RESET_POUT_RST__MASK                       0xFFFF    

/* PIO_IN */
#define RSTID135_PIO_PIO_IN                                          0x00000010
#define RSTID135_PIO_PIO_IN__DEFAULT                                 0x0       
#define FSTID135_PIO_PIO_IN_PIO_IN__MASK                             0xFF      

/* PC0 */
#define RSTID135_PIO_PC0                                             0x00000020
#define RSTID135_PIO_PC0__DEFAULT                                    0x0       
#define FSTID135_PIO_PC0_CONFIG_0__MASK                              0xFF      

/* PC0_SET */
#define RSTID135_PIO_PC0_SET                                         0x00000024
#define RSTID135_PIO_PC0_SET__DEFAULT                                0x0       
#define FSTID135_PIO_PC0_SET_PC0_SET__MASK                           0xFF      

/* PC0_RST */
#define RSTID135_PIO_PC0_RST                                         0x00000028
#define RSTID135_PIO_PC0_RST__DEFAULT                                0x0       
#define FSTID135_PIO_PC0_RST_PC0_RST__MASK                           0xFF      

/* PC1 */
#define RSTID135_PIO_PC1                                             0x00000030
#define RSTID135_PIO_PC1__DEFAULT                                    0x0       
#define FSTID135_PIO_PC1_CONFIG_1__MASK                              0xFF      

/* PC1_SET */
#define RSTID135_PIO_PC1_SET                                         0x00000034
#define RSTID135_PIO_PC1_SET__DEFAULT                                0x0       
#define FSTID135_PIO_PC1_SET_PC1_SET__MASK                           0xFF      

/* PC1_RST */
#define RSTID135_PIO_PC1_RST                                         0x00000038
#define RSTID135_PIO_PC1_RST__DEFAULT                                0x0       
#define FSTID135_PIO_PC1_RST_PC1_RST__MASK                           0xFF      

/* PC2 */
#define RSTID135_PIO_PC2                                             0x00000040
#define RSTID135_PIO_PC2__DEFAULT                                    0x0       
#define FSTID135_PIO_PC2_CONFIG_2__MASK                              0xFF      

/* PC2_SET */
#define RSTID135_PIO_PC2_SET                                         0x00000044
#define RSTID135_PIO_PC2_SET__DEFAULT                                0x0       
#define FSTID135_PIO_PC2_SET_PC2_SET__MASK                           0xFF      

/* PC2_RST */
#define RSTID135_PIO_PC2_RST                                         0x00000048
#define RSTID135_PIO_PC2_RST__DEFAULT                                0x0       
#define FSTID135_PIO_PC2_RST_PC2_RST__MASK                           0xFF      

/* PCOMP */
#define RSTID135_PIO_PCOMP                                           0x00000050
#define RSTID135_PIO_PCOMP__DEFAULT                                  0x0       
#define FSTID135_PIO_PCOMP_PCOMP__MASK                               0xFF      

/* PCOMP_SET */
#define RSTID135_PIO_PCOMP_SET                                       0x00000054
#define RSTID135_PIO_PCOMP_SET__DEFAULT                              0x0       
#define FSTID135_PIO_PCOMP_SET_PCOMP_SET__MASK                       0xFF      

/* PCOMP_RST */
#define RSTID135_PIO_PCOMP_RST                                       0x00000058
#define RSTID135_PIO_PCOMP_RST__DEFAULT                              0x0       
#define FSTID135_PIO_PCOMP_RST_PCOMP_RST__MASK                       0xFF      

/* PMASK */
#define RSTID135_PIO_PMASK                                           0x00000060
#define RSTID135_PIO_PMASK__DEFAULT                                  0x0       
#define FSTID135_PIO_PMASK_MASK__MASK                                0xFF      

/* PMASK_SET */
#define RSTID135_PIO_PMASK_SET                                       0x00000064
#define RSTID135_PIO_PMASK_SET__DEFAULT                              0x0       
#define FSTID135_PIO_PMASK_SET_PMASK_SET__MASK                       0xFF      

/* PMASK_RST */
#define RSTID135_PIO_PMASK_RST                                       0x00000068
#define RSTID135_PIO_PMASK_RST__DEFAULT                              0x0       
#define FSTID135_PIO_PMASK_RST_PMASK_RST__MASK                       0xFF      


/* Number of registers */
#define STID135_PIO_REG_V1_4_REG_NBREGS                              19        

/* Number of fields */
#define STID135_PIO_REG_V1_4_REG_NBFIELDS                            19        



#endif /* #ifndef _PIO_REG_V1_4_REG_INIT_H */

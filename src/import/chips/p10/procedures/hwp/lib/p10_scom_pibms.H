/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/lib/p10_scom_pibms.H $    */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2019,2020                        */
/* [+] International Business Machines Corp.                              */
/*                                                                        */
/*                                                                        */
/* Licensed under the Apache License, Version 2.0 (the "License");        */
/* you may not use this file except in compliance with the License.       */
/* You may obtain a copy of the License at                                */
/*                                                                        */
/*     http://www.apache.org/licenses/LICENSE-2.0                         */
/*                                                                        */
/* Unless required by applicable law or agreed to in writing, software    */
/* distributed under the License is distributed on an "AS IS" BASIS,      */
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or        */
/* implied. See the License for the specific language governing           */
/* permissions and limitations under the License.                         */
/*                                                                        */
/* IBM_PROLOG_END_TAG                                                     */

#ifndef __pibms_H_
#define __pibms_H_

const uint64_t PERV_FSI2PIB_STATUS_FSI = 0x1007;
//const uint64_t PERV_TP_PLL_LOCK_REG = 0x010F0019;

const uint64_t PERV_CBS_CS_FSI = 0x2801;
const uint64_t PERV_SB_CS_FSI  = 0x2808;

const uint8_t  PERV_CBS_CS_SECURE_ACCESS_BIT = 4;
const uint8_t  PERV_SB_CS_SECURE_DEBUG_MODE  = 0;


const uint64_t OTP_STATUS           = 0x00010002;
const uint64_t OTP_SECURITY_SWITCH  = 0x00010005;
const uint64_t OTP_CONTROL          = 0x00010008;

const uint64_t MAILBOX_CBS_CTRL_STATUS           = 0x00050001;
const uint64_t MAILBOX_CBS_TRACE                 = 0x00050002;
const uint64_t MAILBOX_CBS_ENVSTAT               = 0x00050004;
const uint64_t MAILBOX_CBS_SELFBOOT_CTRL_STATUS  = 0x00050008;
const uint64_t MAILBOX_CBS_SELFBOOT_MSG          = 0x00050009;
const uint64_t MAILBOX_CBS_DEBUG_PERV_CLK        = 0x0005000B;
const uint64_t MAILBOX_ROOTCTRL0                 = 0x00050010;
const uint64_t MAILBOX_ROOTCTRL1                 = 0x00050011;
const uint64_t MAILBOX_ROOTCTRL2                 = 0x00050012;
const uint64_t MAILBOX_ROOTCTRL3                 = 0x00050013;
const uint64_t MAILBOX_ROOTCTRL4                 = 0x00050014;
const uint64_t MAILBOX_ROOTCTRL5                 = 0x00050015;
const uint64_t MAILBOX_ROOTCTRL6                 = 0x00050016;
const uint64_t MAILBOX_ROOTCTRL7                 = 0x00050017;
const uint64_t MAILBOX_ROOTCTRL8                 = 0x00050018;
const uint64_t MAILBOX_PERVCTRL0                 = 0x0005001A;
const uint64_t MAILBOX_PERVCTRL1                 = 0x0005001B;
const uint64_t MAILBOX_SCRATCH_REG_1             = 0x00050038;
const uint64_t MAILBOX_SCRATCH_REG_2             = 0x00050039;
const uint64_t MAILBOX_SCRATCH_REG_3             = 0x0005003A;
const uint64_t MAILBOX_SCRATCH_REG_4             = 0x0005003B;
const uint64_t MAILBOX_SCRATCH_REG_5             = 0x0005003C;
const uint64_t MAILBOX_SCRATCH_REG_6             = 0x0005003D;
const uint64_t MAILBOX_SCRATCH_REG_7             = 0x0005003E;
const uint64_t MAILBOX_SCRATCH_REG_8             = 0x0005003F;
const uint64_t MAILBOX_SCRATCH_REG_9             = 0x00050180;
const uint64_t MAILBOX_SCRATCH_REG_10            = 0x00050181;
const uint64_t MAILBOX_SCRATCH_REG_11            = 0x00050182;
const uint64_t MAILBOX_SCRATCH_REG_12            = 0x00050183;
const uint64_t MAILBOX_SCRATCH_REG_13            = 0x00050184;
const uint64_t MAILBOX_SCRATCH_REG_14            = 0x00050185;
const uint64_t MAILBOX_SCRATCH_REG_15            = 0x00050186;
const uint64_t MAILBOX_SCRATCH_REG_16            = 0x00050187;

const uint64_t PIBMEM_CTRL             = 0x0008FFF0;
const uint64_t PIBMEM_ADDR_PIB         = 0x0008FFF1;
const uint64_t PIBMEM_STATUS           = 0x0008FFF5;
const uint64_t PIBMEM_RESET            = 0x0008FFF6;
const uint64_t PIBMEM_ADDR_FACES       = 0x0008FFF7;
const uint64_t PIBMEM_FIR_MASK         = 0x0008FFF8;
const uint64_t PIBMEM_ECC_CAPTURE      = 0x0008FFFB;

const uint64_t  SPIMST0_ERR_INJECT          = 0x000C0000;
const uint64_t  SPIMST0_COUNTER             = 0x000C0001;
const uint64_t  SPIMST0_CONFIG              = 0x000C0002;
const uint64_t  SPIMST0_CLOCK_CONFIG        = 0x000C0003;
const uint64_t  SPIMST0_MEM_MAPPING         = 0x000C0004;
const uint64_t  SPIMST0_TRANSMIT_DATA_REG   = 0x000C0005;
const uint64_t  SPIMST0_RECEIVE_DATA_REG    = 0x000C0006;
const uint64_t  SPIMST0_SEQ_OP_REG          = 0x000C0007;
const uint64_t  SPIMST0_STATUS_REG          = 0x000C0008;

const uint64_t  SPIMST1_ERR_INJECT          = 0x000C0020;
const uint64_t  SPIMST1_COUNTER             = 0x000C0021;
const uint64_t  SPIMST1_CONFIG              = 0x000C0022;
const uint64_t  SPIMST1_CLOCK_CONFIG        = 0x000C0023;
const uint64_t  SPIMST1_MEM_MAPPING         = 0x000C0024;
const uint64_t  SPIMST1_TRANSMIT_DATA_REG   = 0x000C0025;
const uint64_t  SPIMST1_RECEIVE_DATA_REG    = 0x000C0026;
const uint64_t  SPIMST1_SEQ_OP_REG          = 0x000C0027;
const uint64_t  SPIMST1_STATUS_REG          = 0x000C0028;

const uint64_t  SPIMST2_ERR_INJECT          = 0x000C0040;
const uint64_t  SPIMST2_COUNTER             = 0x000C0041;
const uint64_t  SPIMST2_CONFIG              = 0x000C0042;
const uint64_t  SPIMST2_CLOCK_CONFIG        = 0x000C0043;
const uint64_t  SPIMST2_MEM_MAPPING         = 0x000C0044;
const uint64_t  SPIMST2_TRANSMIT_DATA_REG   = 0x000C0045;
const uint64_t  SPIMST2_RECEIVE_DATA_REG    = 0x000C0046;
const uint64_t  SPIMST2_SEQ_OP_REG          = 0x000C0047;
const uint64_t  SPIMST2_STATUS_REG          = 0x000C0048;

const uint64_t  SPIMST3_ERR_INJECT          = 0x000C0060;
const uint64_t  SPIMST3_COUNTER             = 0x000C0061;
const uint64_t  SPIMST3_CONFIG              = 0x000C0062;
const uint64_t  SPIMST3_CLOCK_CONFIG        = 0x000C0063;
const uint64_t  SPIMST3_MEM_MAPPING         = 0x000C0064;
const uint64_t  SPIMST3_TRANSMIT_DATA_REG   = 0x000C0065;
const uint64_t  SPIMST3_RECEIVE_DATA_REG    = 0x000C0066;
const uint64_t  SPIMST3_SEQ_OP_REG          = 0x000C0067;
const uint64_t  SPIMST3_STATUS_REG          = 0x000C0068;

const uint64_t  SPIMST4_ERR_INJECT          = 0x000C0080;
const uint64_t  SPIMST4_COUNTER             = 0x000C0081;
const uint64_t  SPIMST4_CONFIG              = 0x000C0082;
const uint64_t  SPIMST4_CLOCK_CONFIG        = 0x000C0083;
const uint64_t  SPIMST4_MEM_MAPPING          = 0x000C0084;
const uint64_t  SPIMST4_TRANSMIT_DATA_REG   = 0x000C0085;
const uint64_t  SPIMST4_RECEIVE_DATA_REG    = 0x000C0086;
const uint64_t  SPIMST4_SEQ_OP_REG          = 0x000C0087;
const uint64_t  SPIMST4_STATUS_REG          = 0x000C0088;

const uint64_t     SBEFIFO_UPSTREAM_STATUS   = 0x000B0001;
const uint64_t     SBEFIFO_DOWNSTREAM_STATUS = 0x000B0011;
const uint64_t     SBEFIFO2_UPSTREAM_STATUS   = 0x000B0021;
const uint64_t     SBEFIFO2_DOWNSTREAM_STATUS = 0x000B0031;

const uint64_t PSU_PIBHST_CTRL_STATUS            = 0x000D0000;
const uint64_t PSU_PIBHST_FILTER                 = 0x000D0001;
const uint64_t PSU_PIBHST_LAST_ADDR_TRACE        = 0x000D0002;
const uint64_t PSU_PIBHST_LAST_REQDATA_TRACE     = 0x000D0003;
const uint64_t PSU_PIBHST_LAST_RSPDATA_TRACE     = 0x000D0004;
const uint64_t PSU_PIBHST_2NDLAST_ADDR_TRACE     = 0x000D0005;
const uint64_t PSU_PIBHST_2NDLAST_REQDATA_TRACE  = 0x000D0006;
const uint64_t PSU_PIBHST_2NDLAST_RSPDATA_TRACE  = 0x000D0007;
const uint64_t PSU_INSTR_CTRL_STATUS             = 0x000D0010;

const uint64_t PCBM_REC_ERR_REG0         = 0x000F0011;
const uint64_t PCBM_REC_ERR_REG1         = 0x000F0012;
const uint64_t PCBM_REC_ERR_REG2         = 0x000F0013;
const uint64_t PCBM_REC_ERR_REG3         = 0x000F0014;
const uint64_t PCBM_FIRST_ERR            = 0x000F001E;
const uint64_t PCBM_ERR_REG              = 0x000F001F;
const uint64_t PCBM_REC_ERR_MST14_REG0         = 0x000F0078;
const uint64_t PCBM_REC_ERR_MST14_REG1         = 0x000F0079;
const uint64_t PCBM_REC_ERR_MST14_REG2         = 0x000F007A;
const uint64_t PCBM_REC_ERR_MST14_REG3         = 0x000F007B;

#endif

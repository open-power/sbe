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

const uint8_t    I2CM_STATUS_REGISTER_B_ECC_UNCORRECTED_ERROR_0      = 42;
const uint8_t    I2CM_STATUS_REGISTER_B_ECC_CONFIG_ERROR_0           = 43;
const uint8_t    I2CM_STATUS_REGISTER_B_BUS_BUSY_0                   = 44;
const uint8_t    I2CM_STATUS_REGISTER_B_BUS_INVALID_COMMAND_0        = 45;
const uint8_t    I2CM_STATUS_REGISTER_B_BUS_PARITY_ERROR_0           = 46;
const uint8_t    I2CM_STATUS_REGISTER_B_BUS_BACK_END_OVERRUN_ERROR_0 = 47;
const uint8_t    I2CM_STATUS_REGISTER_B_BUS_BACK_END_ACCESS_ERROR_0  = 48;
const uint8_t    I2CM_STATUS_REGISTER_B_BUS_ARBITRATION_LOST_ERROR_0 = 49;
const uint8_t    I2CM_STATUS_REGISTER_B_BUS_NACK_RECEIVED_ERROR_0    = 50;
const uint8_t    I2CM_STATUS_REGISTER_B_BUS_STOP_ERROR_0             = 53;

const uint64_t PERV_FSI2PIB_STATUS_FSI = 0x1007;
//const uint64_t PERV_TP_PLL_LOCK_REG = 0x010F0019;

const uint64_t PERV_CBS_CS_FSI = 0x2801;
const uint64_t PERV_SB_CS_FSI  = 0x2808;

const uint8_t  PERV_CBS_CS_SECURE_ACCESS_BIT = 4;
const uint8_t  PERV_SB_CS_SECURE_DEBUG_MODE  = 0;


const uint64_t OTP_STATUS           = 0x00010002;
const uint64_t OTP_SECURITY_SWITCH  = 0x00010005;

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

const uint64_t PIBMEM_CTRL             = 0x00088000;
const uint64_t PIBMEM_ADDR_PIB         = 0x00088001;
const uint64_t PIBMEM_STATUS           = 0x00088005;
const uint64_t PIBMEM_RESET            = 0x00088006;
const uint64_t PIBMEM_ADDR_FACES       = 0x00088007;
const uint64_t PIBMEM_FIR_MASK         = 0x00088008;
const uint64_t PIBMEM_ECC_CAPTURE      = 0x0008800B;

const uint64_t I2CM_CONTROL_REGISTER_B                   = 0x000A0000;
const uint64_t I2CM_DATA8TO15_REGISTER_B                 = 0x000A0001;
const uint64_t I2CM_RESET_REGISTER_B                     = 0x000A0001;
const uint64_t I2CM_FSTATUS_REGISTER_B                   = 0x000A0002;  //STATUS REGISTER_FAST_MODE
const uint64_t I2CM_STATUS_REGISTER_B                    = 0x000A0002;  //was there
const uint64_t I2CM_DATA0TO7_REGISTER_B                  = 0x000A0003;
const uint64_t I2CM_FIFO1_REGISTER_READ_B                = 0x000A0004;
const uint64_t I2CM_COMMAND_REGISTER_B                   = 0x000A0005;
const uint64_t I2CM_MODE_REGISTER_B                      = 0x000A0006;
const uint64_t I2CM_WATER_MARK_REGISTER_B                = 0x000A0007;
const uint64_t I2CM_INTERRUPT_MASK_REGISTER_READ_B       = 0x000A0008;
const uint64_t I2CM_INTERRUPT_COND_B                     = 0x000A0009;
const uint64_t I2CM_INTERRUPTS_B                         = 0x000A000A;
const uint64_t I2CM_STATUS_REGISTER_ENGINE_B             = 0x000A000B;
const uint64_t I2CM_EXTENDED_STATUS_B                    = 0x000A000C;
const uint64_t I2CM_RESIDUAL_FRONT_END_BACK_END_LENGTH_B = 0x000A000D;
const uint64_t I2CM_I2C_BUSY_REGISTER_B                  = 0x000A000E;
const uint64_t I2CM_FIFO4_REGISTER_READ_B                = 0x000A0012;
const uint64_t I2CM_CONTROL_REGISTER_C                   = 0x000A1000;
const uint64_t I2CM_DATA8TO15_REGISTER_C                 = 0x000A1001;
const uint64_t I2CM_FSTATUS_REGISTER_C                   = 0x000A1002;  //STATUS REGISTER_FAST_MODE
const uint64_t I2CM_DATA0TO7_REGISTER_C                  = 0x000A1003;
const uint64_t I2CM_FIFO1_REGISTER_READ_C                = 0x000A1004;
const uint64_t I2CM_COMMAND_REGISTER_C                   = 0x000A1005;
const uint64_t I2CM_MODE_REGISTER_C                      = 0x000A1006;
const uint64_t I2CM_WATER_MARK_REGISTER_C                = 0x000A1007;
const uint64_t I2CM_INTERRUPT_MASK_REGISTER_READ_C       = 0x000A1008;
const uint64_t I2CM_INTERRUPT_COND_C                     = 0x000A1009;
const uint64_t I2CM_INTERRUPTS_C                         = 0x000A100A;
const uint64_t I2CM_STATUS_REGISTER_ENGINE_C             = 0x000A100B;
const uint64_t I2CM_EXTENDED_STATUS_C                    = 0x000A100C;
const uint64_t I2CM_RESIDUAL_FRONT_END_BACK_END_LENGTH_C = 0x000A100D;
const uint64_t I2CM_I2C_BUSY_REGISTER_C                  = 0x000A100E;

const uint64_t     SBEFIFO_UPSTREAM_STATUS   = 0x000B0001;
const uint64_t     SBEFIFO_DOWNSTREAM_STATUS = 0x000B0011;

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

#endif

/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/common/include/p10_ppe_eq_e.H $          */
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

#ifndef __PPE_HCODE__
    #include "proc_scomt.H"
#endif

#ifndef __p10_ppe_eq_e_H_
#define __p10_ppe_eq_e_H_


#ifndef __PPE_HCODE__
namespace scomt
{
namespace ppe_eq
{
#endif


//>> [QME_CRCR]
//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_CRCR = 0xc0003000ull;

static const uint32_t QME_CRCR_COMPARE_VALUE = 0;
static const uint32_t QME_CRCR_COMPARE_VALUE_LEN = 8;
static const uint32_t P10_20_QME_CRCR_OMPARE_VALUE = 0; // p10:20,
static const uint32_t P10_20_QME_CRCR_OMPARE_VALUE_LEN = 8;
static const uint32_t P10_20_QME_CRCR_R_TOGGLE_STEP = 8; // p10:20,
static const uint32_t P10_20_QME_CRCR_R_TOGGLE_SYNC = 9; // p10:20,
//<< [QME_CRCR]
// ppe_eq/reg00014.H

//>> [QME_DERP]
//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_DERP = 0xc0002500ull;

static const uint32_t QME_DERP_DEND = 0;
static const uint32_t QME_DERP_DEND_LEN = 32;
static const uint32_t QME_DERP_SOR = 32;
static const uint32_t QME_DERP_SOR_LEN = 32;
//<< [QME_DERP]
// ppe_eq/reg00014.H

//>> [QME_EINR]
//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_EINR = 0xc0002740ull;

static const uint32_t QME_EINR_INTERRUPT_INPUT = 0;
static const uint32_t QME_EINR_INTERRUPT_INPUT_LEN = 64;
//<< [QME_EINR]
// ppe_eq/reg00014.H

//>> [QME_LFIR]
//ERRCODE: // Some of the name targets not found in address targets: eq

static const uint32_t QME_LFIR_RW = 0xc0000000ull;
static const uint32_t QME_LFIR_WO_AND = 0xc0000010ull;
static const uint32_t QME_LFIR_WO_OR = 0xc0000020ull;

static const uint32_t QME_LFIR_PPE_HALT_ERROR = 0;
static const uint32_t QME_LFIR_DEBUG_TRIGGER = 1;
static const uint32_t QME_LFIR_SPARE_TRIGGER = 2;
static const uint32_t QME_LFIR_PPE_WATCHDOG = 3;
static const uint32_t QME_LFIR_LOCAL_PCB_TIMEOUT = 4;
static const uint32_t QME_LFIR_FABRIC_ERROR = 5;
static const uint32_t QME_LFIR_SRAM_UE = 6;
static const uint32_t QME_LFIR_SRAM_CE = 7;
static const uint32_t QME_LFIR_RESCLK_ARRAY_PARITY_ERR = 8;
static const uint32_t QME_LFIR_PCB_INTERRUPT_PROTOCOL_ERR = 9;
static const uint32_t QME_LFIR_SRAM_SCRUB_ERR = 10;
static const uint32_t QME_LFIR_CTFS_ERR = 11;
static const uint32_t QME_LFIR_CPMS_ERR = 12;
static const uint32_t QME_LFIR_PGPE_HEARTBEAT_LOST = 13;
static const uint32_t QME_LFIR_BCE_TIMEOUT = 14;
static const uint32_t QME_LFIR_RESCLK_PROTOCOL_ERR = 15;
static const uint32_t QME_LFIR_PCB_RESET_WHEN_ACTIVE = 16;
static const uint32_t QME_LFIR_SPECIAL_WKUP_PROTOCOL_ERR = 17;
static const uint32_t QME_LFIR_SPECIAL_WKUP_DONE_WINDOW = 18;
static const uint32_t QME_LFIR_DISABLED_INTR = 19;
static const uint32_t QME_LFIR_DECONFIGURED_INTR = 20;
static const uint32_t QME_LFIR_RS4_TIMEOUT = 21;
static const uint32_t P10_20_QME_LFIR_MIXED_LPAR_ERROR = 21; // p10:20,
static const uint32_t QME_LFIR_PB_DATA_HANG = 22;
static const uint32_t QME_LFIR_WRITE_PROTECT_FAIL = 23;
static const uint32_t QME_LFIR_DTC_ERROR = 24;
static const uint32_t QME_LFIR_PB_CE = 25;
static const uint32_t QME_LFIR_PB_UE = 26;
static const uint32_t QME_LFIR_PB_SUE = 27;
static const uint32_t QME_LFIR_PB_INVALID_TOPOTABLE_ENTRY = 28;
static const uint32_t QME_LFIR_PB_TAG_PERR = 29;
static const uint32_t QME_LFIR_PIG_PROTOCOL_ERR = 30;
static const uint32_t QME_LFIR_LOCAL_ACCESS_ERR = 31;
static const uint32_t QME_LFIR_PB_SSA_CE = 32;
static const uint32_t QME_LFIR_PB_SSA_UE = 33;
static const uint32_t QME_LFIR_SPARE1 = 34;
static const uint32_t QME_LFIR_SPARE1_LEN = 2;
static const uint32_t P10_20_QME_LFIR_RESCLK_CCFG_PAR_ERR = 34; // p10:20,
static const uint32_t P10_20_QME_LFIR_SPARE1 = 35; // p10:20,
//<< [QME_LFIR]
// ppe_eq/reg00014.H

//>> [QME_PCBSII]
//ERRCODE: // Some of the name targets not found in address targets: eq

static const uint32_t QME_PCBSII = 0xc0000380ull;

static const uint32_t QME_PCBSII_PCB_REQ_ACK = 0;
static const uint32_t QME_PCBSII_PCB_RSP_P = 1;
static const uint32_t QME_PCBSII_PCB_RSP_VLD = 4;
static const uint32_t QME_PCBSII_PCB_RSP_INFO = 5;
static const uint32_t QME_PCBSII_PCB_RSP_INFO_LEN = 3;
static const uint32_t QME_PCBSII_QUAD_ID = 33;
static const uint32_t QME_PCBSII_QUAD_ID_LEN = 3;
//<< [QME_PCBSII]
// ppe_eq/reg00014.H

//>>THE END<<

#ifndef __PPE_HCODE__
}
}
#include "ppe_eq/reg00014.H"
#endif
#endif

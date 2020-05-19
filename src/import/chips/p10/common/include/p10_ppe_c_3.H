/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/common/include/p10_ppe_c_3.H $           */
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

#ifndef __p10_ppe_c_3_H_
#define __p10_ppe_c_3_H_


#ifndef __PPE_HCODE__
namespace scomt
{
namespace ppe_c
{
#endif


//>> [CPMS_CL2_PFETCNTL]
static const uint32_t CPMS_CL2_PFETCNTL = 0xc000e280ull;
static const uint32_t CPMS_CL2_PFETCNTL_WO_CLEAR = 0xc000e2b0ull;
static const uint32_t CPMS_CL2_PFETCNTL_WO_OR = 0xc000e2a0ull;

static const uint32_t CPMS_CL2_PFETCNTL_VDD_PFET_SEQ_STATE = 0;
static const uint32_t CPMS_CL2_PFETCNTL_VDD_PFET_SEQ_STATE_LEN = 2;
static const uint32_t CPMS_CL2_PFETCNTL_VCS_PFET_SEQ_STATE = 2;
static const uint32_t CPMS_CL2_PFETCNTL_VCS_PFET_SEQ_STATE_LEN = 2;
static const uint32_t CPMS_CL2_PFETCNTL_VDD_PFET_VAL_OVERRIDE = 4;
static const uint32_t CPMS_CL2_PFETCNTL_VDD_PFET_SEL_OVERRIDE = 5;
static const uint32_t CPMS_CL2_PFETCNTL_VCS_PFET_VAL_OVERRIDE = 6;
static const uint32_t CPMS_CL2_PFETCNTL_VCS_PFET_SEL_OVERRIDE = 7;
static const uint32_t CPMS_CL2_PFETCNTL_VDD_PFET_REGULATION_FINGER_SET = 8;
static const uint32_t CPMS_CL2_PFETCNTL_VDD_PFET_ENABLE_VALUE = 12;
static const uint32_t CPMS_CL2_PFETCNTL_VDD_PFET_ENABLE_VALUE_LEN = 8;
static const uint32_t CPMS_CL2_PFETCNTL_VDD_PFET_SEL_VALUE = 20;
static const uint32_t CPMS_CL2_PFETCNTL_VDD_PFET_SEL_VALUE_LEN = 4;
static const uint32_t CPMS_CL2_PFETCNTL_VCS_PFET_ENABLE_VALUE = 24;
static const uint32_t CPMS_CL2_PFETCNTL_VCS_PFET_ENABLE_VALUE_LEN = 8;
static const uint32_t CPMS_CL2_PFETCNTL_VCS_PFET_SEL_VALUE = 32;
static const uint32_t CPMS_CL2_PFETCNTL_VCS_PFET_SEL_VALUE_LEN = 4;
static const uint32_t CPMS_CL2_PFETCNTL_VDD_PG_STATE = 42;
static const uint32_t CPMS_CL2_PFETCNTL_VDD_PG_STATE_LEN = 4;
static const uint32_t CPMS_CL2_PFETCNTL_VDD_PG_SEL = 46;
static const uint32_t CPMS_CL2_PFETCNTL_VDD_PG_SEL_LEN = 4;
static const uint32_t CPMS_CL2_PFETCNTL_VCS_PG_STATE = 50;
static const uint32_t CPMS_CL2_PFETCNTL_VCS_PG_STATE_LEN = 4;
static const uint32_t CPMS_CL2_PFETCNTL_VCS_PG_SEL = 54;
static const uint32_t CPMS_CL2_PFETCNTL_VCS_PG_SEL_LEN = 4;
static const uint32_t CPMS_CL2_PFETCNTL_ACS_MINMAX_SEL = 58;
static const uint32_t CPMS_CL2_PFETCNTL_ACS_MINMAX_SEL_LEN = 4;
static const uint32_t CPMS_CL2_PFETCNTL_ACS_ENABLE = 62;
static const uint32_t CPMS_CL2_PFETCNTL_SRAM_ENABLE = 63;
//<< [CPMS_CL2_PFETCNTL]
// ppe_c/reg00003.H

//>> [CPMS_FDCR]
static const uint32_t CPMS_FDCR = 0xc000e400ull;
static const uint32_t CPMS_FDCR_PPE2 = 0xc000e420ull;
static const uint32_t CPMS_FDCR_WO_CLEAR = 0xc000e430ull;

static const uint32_t CPMS_FDCR_DISABLE = 0;
static const uint32_t CPMS_FDCR_FORCE_SAMPLE = 1;
static const uint32_t CPMS_FDCR_STICKY_CONTROL = 2;
static const uint32_t CPMS_FDCR_STICKY_CONTROL_LEN = 2;
static const uint32_t CPMS_FDCR_CONTROL_UPDATE_DISABLE = 4;
static const uint32_t CPMS_FDCR_SUPPRESS_FTC_UPDATE = 5;
static const uint32_t CPMS_FDCR_CAL_ADJUST = 6;
static const uint32_t CPMS_FDCR_CAL_ADJUST_LEN = 2;
static const uint32_t CPMS_FDCR_DELAY = 8;
static const uint32_t CPMS_FDCR_DELAY_LEN = 8;
static const uint32_t CPMS_FDCR_THRESHOLD_UPDATE_DISABLE = 16;
static const uint32_t CPMS_FDCR_TRIP_OFFSET = 17;
static const uint32_t CPMS_FDCR_TRIP_OFFSET_LEN = 3;
static const uint32_t CPMS_FDCR_DATA0_DETECT = 20;
static const uint32_t CPMS_FDCR_DATA0_DETECT_LEN = 4;
static const uint32_t CPMS_FDCR_DATA1_DETECT = 24;
static const uint32_t CPMS_FDCR_DATA1_DETECT_LEN = 4;
static const uint32_t CPMS_FDCR_DATA2_DETECT = 28;
static const uint32_t CPMS_FDCR_DATA2_DETECT_LEN = 4;
static const uint32_t CPMS_FDCR_LARGE_DROOP_DETECT = 32;
static const uint32_t CPMS_FDCR_LARGE_DROOP_DETECT_LEN = 4;
static const uint32_t CPMS_FDCR_SMALL_DROOP_DETECT = 36;
static const uint32_t CPMS_FDCR_SMALL_DROOP_DETECT_LEN = 4;
static const uint32_t CPMS_FDCR_SLOPEA_START_DETECT = 40;
static const uint32_t CPMS_FDCR_SLOPEA_START_DETECT_LEN = 4;
static const uint32_t CPMS_FDCR_SLOPEA_END_DETECT = 44;
static const uint32_t CPMS_FDCR_SLOPEA_END_DETECT_LEN = 4;
static const uint32_t CPMS_FDCR_SLOPEB_START_DETECT = 48;
static const uint32_t CPMS_FDCR_SLOPEB_START_DETECT_LEN = 4;
static const uint32_t CPMS_FDCR_SLOPEB_END_DETECT = 52;
static const uint32_t CPMS_FDCR_SLOPEB_END_DETECT_LEN = 4;
static const uint32_t CPMS_FDCR_SLOPEA_CYCLES = 56;
static const uint32_t CPMS_FDCR_SLOPEA_CYCLES_LEN = 4;
static const uint32_t CPMS_FDCR_SLOPEB_CYCLES = 60;
static const uint32_t CPMS_FDCR_SLOPEB_CYCLES_LEN = 4;
//<< [CPMS_FDCR]
// ppe_c/reg00003.H

//>> [CPMS_CCDR]
static const uint32_t CPMS_CCDR = 0xc000e740ull;

static const uint32_t CPMS_CCDR_CHAR_COUNTA_SCALE = 0;
static const uint32_t CPMS_CCDR_CHAR_COUNTA_SCALE_LEN = 4;
static const uint32_t CPMS_CCDR_CHAR_COUNTA_VALUE = 4;
static const uint32_t CPMS_CCDR_CHAR_COUNTA_VALUE_LEN = 16;
static const uint32_t CPMS_CCDR_CHAR_COUNTB_SCALE = 20;
static const uint32_t CPMS_CCDR_CHAR_COUNTB_SCALE_LEN = 4;
static const uint32_t CPMS_CCDR_CHAR_COUNTB_VALUE = 24;
static const uint32_t CPMS_CCDR_CHAR_COUNTB_VALUE_LEN = 16;
static const uint32_t CPMS_CCDR_CHAR_COUNTC_SCALE = 40;
static const uint32_t CPMS_CCDR_CHAR_COUNTC_SCALE_LEN = 4;
static const uint32_t CPMS_CCDR_CHAR_COUNTC_VALUE = 44;
static const uint32_t CPMS_CCDR_CHAR_COUNTC_VALUE_LEN = 16;
static const uint32_t CPMS_CCDR_CHAR_COUNTA_MOD = 60;
static const uint32_t CPMS_CCDR_CHAR_COUNTB_MOD = 61;
static const uint32_t CPMS_CCDR_CHAR_COUNTC_MOD = 62;
static const uint32_t CPMS_CCDR_SAMPLE_DISABLE_INDICATOR = 63;
//<< [CPMS_CCDR]
// ppe_c/reg00003.H

//>> [CPMS_PRSR]
static const uint32_t CPMS_PRSR = 0xc000eac0ull;

static const uint32_t CPMS_PRSR_CPMS_PPSR_PERIOD_PROXY_REMAIN = 0;
static const uint32_t CPMS_PRSR_CPMS_PPSR_PERIOD_PROXY_REMAIN_LEN = 20;
//<< [CPMS_PRSR]
// ppe_c/reg00003.H

//>> [QME_CDBA]
static const uint32_t QME_CDBA = 0xc0008000ull;

static const uint32_t QME_CDBA_MESSAGE_NUMBER = 0;
static const uint32_t QME_CDBA_MESSAGE_NUMBER_LEN = 8;
//<< [QME_CDBA]
// ppe_c/reg00003.H

//>> [QME_CSIR]
static const uint32_t QME_CSIR = 0xc00087c0ull;

static const uint32_t QME_CSIR_PMSR_INSTR_DATA = 36;
static const uint32_t QME_CSIR_PMSR_INSTR_DATA_LEN = 22;
//<< [QME_CSIR]
// ppe_c/reg00003.H

//>>THE END<<

#ifndef __PPE_HCODE__
}
}
#include "ppe_c/reg00003.H"
#endif
#endif

/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/common/include/p10_scom_c_d_unused.H $   */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2020                             */
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

#ifndef __p10_scom_c_d_H_UNUSED__
#define __p10_scom_c_d_H_UNUSED__


#ifndef __PPE_HCODE__
namespace scomt
{
namespace c
{
#endif


//>> [CPMS_PPSR]
static const uint64_t CPMS_PPSR = 0x200e0e98ull;

static const uint32_t CPMS_PPSR_ENERGY_PROXY_UPPER = 0;
static const uint32_t CPMS_PPSR_ENERGY_PROXY_UPPER_LEN = 32;
static const uint32_t CPMS_PPSR_ENERGY_PROXY_LOWER = 32;
static const uint32_t CPMS_PPSR_ENERGY_PROXY_LOWER_LEN = 8;
static const uint32_t CPMS_PPSR_PERIOD_PROXY_REMAIN = 40;
static const uint32_t CPMS_PPSR_PERIOD_PROXY_REMAIN_LEN = 20;
static const uint32_t CPMS_PPSR_FTX_SATURATED = 60;
static const uint32_t CPMS_PPSR_PITCH_SATURATED = 61;
static const uint32_t CPMS_PPSR_PITCH_ABORTED = 62;
static const uint32_t CPMS_PPSR_PITCH_ACTIVE = 63;
//<< [CPMS_PPSR]
// c/reg00013.H

//>> [EC_LS_LSU_HOLD_OUT_REG3]
static const uint64_t EC_LS_LSU_HOLD_OUT_REG3 = 0x200206c3ull;

static const uint32_t EC_LS_LSU_HOLD_OUT_REG3_0_EADIR_ERR_HOLD_OUT_SET0 = 0;
static const uint32_t EC_LS_LSU_HOLD_OUT_REG3_0_EADIR_ERR_HOLD_OUT_SET1 = 1;
static const uint32_t EC_LS_LSU_HOLD_OUT_REG3_0_EADIR_ERR_HOLD_OUT_SET2 = 2;
static const uint32_t EC_LS_LSU_HOLD_OUT_REG3_0_EADIR_ERR_HOLD_OUT_SET3 = 3;
static const uint32_t EC_LS_LSU_HOLD_OUT_REG3_0_EADIR_ERR_HOLD_OUT_SET4 = 4;
static const uint32_t EC_LS_LSU_HOLD_OUT_REG3_0_EADIR_ERR_HOLD_OUT_SET5 = 5;
static const uint32_t EC_LS_LSU_HOLD_OUT_REG3_0_EADIR_ERR_HOLD_OUT_SET6 = 6;
static const uint32_t EC_LS_LSU_HOLD_OUT_REG3_0_EADIR_ERR_HOLD_OUT_SET7 = 7;
static const uint32_t EC_LS_LSU_HOLD_OUT_REG3_1_EADIR_ERR_HOLD_OUT_SET0 = 8;
static const uint32_t EC_LS_LSU_HOLD_OUT_REG3_1_EADIR_ERR_HOLD_OUT_SET1 = 9;
static const uint32_t EC_LS_LSU_HOLD_OUT_REG3_1_EADIR_ERR_HOLD_OUT_SET2 = 10;
static const uint32_t EC_LS_LSU_HOLD_OUT_REG3_1_EADIR_ERR_HOLD_OUT_SET3 = 11;
static const uint32_t EC_LS_LSU_HOLD_OUT_REG3_1_EADIR_ERR_HOLD_OUT_SET4 = 12;
static const uint32_t EC_LS_LSU_HOLD_OUT_REG3_1_EADIR_ERR_HOLD_OUT_SET5 = 13;
static const uint32_t EC_LS_LSU_HOLD_OUT_REG3_1_EADIR_ERR_HOLD_OUT_SET6 = 14;
static const uint32_t EC_LS_LSU_HOLD_OUT_REG3_1_EADIR_ERR_HOLD_OUT_SET7 = 15;
//<< [EC_LS_LSU_HOLD_OUT_REG3]
// c/reg00013.H

//>> [EC_PC_COMMON_SPR_V2_HMER]
static const uint64_t EC_PC_COMMON_SPR_V2_HMER_RW_WAND = 0x20020494ull;
static const uint64_t EC_PC_COMMON_SPR_V2_HMER_WO_OR = 0x20020490ull;

static const uint32_t EC_PC_COMMON_SPR_V2_HMER_V2_HMER_MALFUNCTION_ALERT = 0;
static const uint32_t EC_PC_COMMON_SPR_V2_HMER_SPARE_1 = 1;
static const uint32_t EC_PC_COMMON_SPR_V2_HMER_V2_HMER_PROC_RCVY_DONE = 2;
static const uint32_t EC_PC_COMMON_SPR_V2_HMER_V2_HMER_SUSPEND_MODE = 3;
static const uint32_t EC_PC_COMMON_SPR_V2_HMER_V2_HMER_TFAC_ERR = 4;
static const uint32_t EC_PC_COMMON_SPR_V2_HMER_V2_HMER_TFX_ERR = 5;
static const uint32_t EC_PC_COMMON_SPR_V2_HMER_SPARE_6 = 6;
static const uint32_t EC_PC_COMMON_SPR_V2_HMER_SPARE_7 = 7;
static const uint32_t EC_PC_COMMON_SPR_V2_HMER_V2_HMER_XSCOM_FAIL = 8;
static const uint32_t EC_PC_COMMON_SPR_V2_HMER_V2_HMER_XSCOM_DONE = 9;
static const uint32_t EC_PC_COMMON_SPR_V2_HMER_SPARE_10 = 10;
static const uint32_t EC_PC_COMMON_SPR_V2_HMER_V2_HMER_PROC_RCVY_AGAIN = 11;
static const uint32_t EC_PC_COMMON_SPR_V2_HMER_SPARE_12 = 12;
static const uint32_t EC_PC_COMMON_SPR_V2_HMER_SPARE_13 = 13;
static const uint32_t EC_PC_COMMON_SPR_V2_HMER_SPARE_14 = 14;
static const uint32_t EC_PC_COMMON_SPR_V2_HMER_SPARE_15 = 15;
static const uint32_t EC_PC_COMMON_SPR_V2_HMER_V2_HMER_SCOM_FIR_HMI = 16;
static const uint32_t EC_PC_COMMON_SPR_V2_HMER_V2_HMER_THD_WAKE_BLOCKED_TM_SUSPEND = 17;
static const uint32_t EC_PC_COMMON_SPR_V2_HMER_V2_HMER_TRIG_FIR_HMI = 18;
static const uint32_t EC_PC_COMMON_SPR_V2_HMER_SPARE_19 = 19;
static const uint32_t EC_PC_COMMON_SPR_V2_HMER_SPARE_20 = 20;
static const uint32_t EC_PC_COMMON_SPR_V2_HMER_V2_HMER_XSCOM_STATUS = 21;
static const uint32_t EC_PC_COMMON_SPR_V2_HMER_V2_HMER_XSCOM_STATUS_LEN = 3;
//<< [EC_PC_COMMON_SPR_V2_HMER]
// c/reg00013.H

//>> [EC_PC_FIR_CORE_ACTION1]
static const uint64_t EC_PC_FIR_CORE_ACTION1 = 0x20020447ull;

static const uint32_t EC_PC_FIR_CORE_ACTION1_IF_SRAM_REC_ERROR = 0;
static const uint32_t EC_PC_FIR_CORE_ACTION1_TC_FIR_XSTOP_ERROR = 1;
static const uint32_t EC_PC_FIR_CORE_ACTION1_IF_RFILE_REC_ERROR = 2;
static const uint32_t EC_PC_FIR_CORE_ACTION1_IF_RFILE_XSTOP_ERROR = 3;
static const uint32_t EC_PC_FIR_CORE_ACTION1_IF_LOG_REC_ERROR = 4;
static const uint32_t EC_PC_FIR_CORE_ACTION1_IF_LOG_XSTOP_ERROR = 5;
static const uint32_t EC_PC_FIR_CORE_ACTION1_UNUSED_6 = 6;
static const uint32_t EC_PC_FIR_CORE_ACTION1_VS_VSMM_ACC_ERROR = 7;
static const uint32_t EC_PC_FIR_CORE_ACTION1_PC_RECOV_XSTOP_ERROR = 8;
static const uint32_t EC_PC_FIR_CORE_ACTION1_VS_STF_ERROR = 9;
static const uint32_t EC_PC_FIR_CORE_ACTION1_UNUSED_10 = 10;
static const uint32_t EC_PC_FIR_CORE_ACTION1_SD_LOG_REC_ERROR = 11;
static const uint32_t EC_PC_FIR_CORE_ACTION1_SD_LOG_XSTOP_ERROR = 12;
static const uint32_t EC_PC_FIR_CORE_ACTION1_SD_NOT_MT_CI_REC_ERROR = 13;
static const uint32_t EC_PC_FIR_CORE_ACTION1_SD_MCHK_AND_ME_EQ_0_ERROR = 14;
static const uint32_t EC_PC_FIR_CORE_ACTION1_SD_L2_UE_ERROR = 15;
static const uint32_t EC_PC_FIR_CORE_ACTION1_SD_L2_UE_OVER_THRES_ERROR = 16;
static const uint32_t EC_PC_FIR_CORE_ACTION1_SD_L2_CI_UE_ERROR = 17;
static const uint32_t EC_PC_FIR_CORE_ACTION1_MU_TLB_P_ERROR = 18;
static const uint32_t EC_PC_FIR_CORE_ACTION1_MU_SLB_P_ERROR = 19;
static const uint32_t EC_PC_FIR_CORE_ACTION1_UNUSED_20 = 20;
static const uint32_t EC_PC_FIR_CORE_ACTION1_MU_CXT_P_ERROR = 21;
static const uint32_t EC_PC_FIR_CORE_ACTION1_MU_LOG_XSTOP_ERROR = 22;
static const uint32_t EC_PC_FIR_CORE_ACTION1_MU_SYS_XSTOP_ERROR = 23;
static const uint32_t EC_PC_FIR_CORE_ACTION1_VS_LOG_REC_ERROR = 24;
static const uint32_t EC_PC_FIR_CORE_ACTION1_VS_LOG_XSTOP_ERROR = 25;
static const uint32_t EC_PC_FIR_CORE_ACTION1_PC_RECOV_IN_MAINT_ERROR = 26;
static const uint32_t EC_PC_FIR_CORE_ACTION1_UNUSED_27 = 27;
static const uint32_t EC_PC_FIR_CORE_ACTION1_PC_SYS_XSTOP_ERROR = 28;
static const uint32_t EC_PC_FIR_CORE_ACTION1_LS_SRAM_PARITY_ERROR = 29;
static const uint32_t EC_PC_FIR_CORE_ACTION1_LS_SETDELETE_ERROR = 30;
static const uint32_t EC_PC_FIR_CORE_ACTION1_LS_RFILE_REC_ERROR = 31;
static const uint32_t EC_PC_FIR_CORE_ACTION1_LS_RFILE_XSTOP_ERROR = 32;
static const uint32_t EC_PC_FIR_CORE_ACTION1_MU_TLB_MULTIHIT_ERROR = 33;
static const uint32_t EC_PC_FIR_CORE_ACTION1_MU_SLB_MULTIHIT_ERROR = 34;
static const uint32_t EC_PC_FIR_CORE_ACTION1_LS_DERAT_MULTIHIT_ERROR = 35;
static const uint32_t EC_PC_FIR_CORE_ACTION1_PC_FWD_PROGRESS_ERROR = 36;
static const uint32_t EC_PC_FIR_CORE_ACTION1_LS_LOG_REC_ERROR = 37;
static const uint32_t EC_PC_FIR_CORE_ACTION1_LS_LOG_XSTOP_ERROR = 38;
static const uint32_t EC_PC_FIR_CORE_ACTION1_UNUSED_39 = 39;
static const uint32_t EC_PC_FIR_CORE_ACTION1_UNUSED_40 = 40;
static const uint32_t EC_PC_FIR_CORE_ACTION1_LS_SYS_XSTOP_ERROR = 41;
static const uint32_t EC_PC_FIR_CORE_ACTION1_UNUSED_42 = 42;
static const uint32_t EC_PC_FIR_CORE_ACTION1_PC_THREAD_HANG_REC_ERROR = 43;
static const uint32_t EC_PC_FIR_CORE_ACTION1_UNUSED_44 = 44;
static const uint32_t EC_PC_FIR_CORE_ACTION1_PC_LOG_XSTOP_ERROR = 45;
static const uint32_t EC_PC_FIR_CORE_ACTION1_PC_TFX_XSTOP_ERROR = 46;
static const uint32_t EC_PC_FIR_CORE_ACTION1_PC_TFAC_XSTOP_ERROR = 47;
static const uint32_t EC_PC_FIR_CORE_ACTION1_UNUSED_48 = 48;
static const uint32_t EC_PC_FIR_CORE_ACTION1_UNUSED_49 = 49;
static const uint32_t EC_PC_FIR_CORE_ACTION1_UNUSED_50 = 50;
static const uint32_t EC_PC_FIR_CORE_ACTION1_UNUSED_51 = 51;
static const uint32_t EC_PC_FIR_CORE_ACTION1_PC_HANG_RECOVERY_FAILED = 52;
static const uint32_t EC_PC_FIR_CORE_ACTION1_PC_CORE_HANG_DETECT_ERROR = 53;
static const uint32_t EC_PC_FIR_CORE_ACTION1_UNUSED_54 = 54;
static const uint32_t EC_PC_FIR_CORE_ACTION1_PC_NEST_HANG_DETECT_ERROR = 55;
static const uint32_t EC_PC_FIR_CORE_ACTION1_PC_OTHER_CORE_CHIPLET_REC_ERROR = 56;
static const uint32_t EC_PC_FIR_CORE_ACTION1_PC_OTHER_CORE_CHIPLET_XSTOP_ERROR = 57;
static const uint32_t EC_PC_FIR_CORE_ACTION1_PC_OTHER_CORE_CHIPLET_SYS_XSTOP_ERROR = 58;
static const uint32_t EC_PC_FIR_CORE_ACTION1_PC_SCOM_ERROR = 59;
static const uint32_t EC_PC_FIR_CORE_ACTION1_PC_XSTOP_ON_DBG_TRIGGER_ERROR = 60;
static const uint32_t EC_PC_FIR_CORE_ACTION1_PC_FW_INJ_REC_ERROR = 61;
static const uint32_t EC_PC_FIR_CORE_ACTION1_PC_FW_INJ_XSTOP_ERROR = 62;
static const uint32_t EC_PC_FIR_CORE_ACTION1_PC_PHYP_XSTOP_ERROR = 63;
//<< [EC_PC_FIR_CORE_ACTION1]
// c/reg00013.H

//>> [EC_PC_PMU_SPRCOR_OCC_SCOMC]
static const uint64_t EC_PC_PMU_SPRCOR_OCC_SCOMC = 0x20020410ull;

static const uint32_t EC_PC_PMU_SPRCOR_OCC_SCOMC_OCC_SCOMD_ADDRESS = 57;
static const uint32_t EC_PC_PMU_SPRCOR_OCC_SCOMC_OCC_SCOMD_ADDRESS_LEN = 4;
//<< [EC_PC_PMU_SPRCOR_OCC_SCOMC]
// c/reg00013.H

//>> [EC_PC_SCR1]
static const uint64_t EC_PC_SCR1 = 0x20020487ull;
//<< [EC_PC_SCR1]
// c/reg00013.H

//>> [EC_PC_TRACE2_TR_TR0_CONFIG_3]
static const uint64_t EC_PC_TRACE2_TR_TR0_CONFIG_3 = 0x20020a86ull;

static const uint32_t EC_PC_TRACE2_TR_TR0_CONFIG_3_C = 0;
static const uint32_t EC_PC_TRACE2_TR_TR0_CONFIG_3_C_LEN = 24;
static const uint32_t EC_PC_TRACE2_TR_TR0_CONFIG_3_D = 24;
static const uint32_t EC_PC_TRACE2_TR_TR0_CONFIG_3_D_LEN = 24;
//<< [EC_PC_TRACE2_TR_TR0_CONFIG_3]
// c/reg00013.H

//>> [EC_SD_SDP_SDCR_UNIT_HOLD_OUT2]
static const uint64_t EC_SD_SDP_SDCR_UNIT_HOLD_OUT2 = 0x20020642ull;

static const uint32_t EC_SD_SDP_SDCR_UNIT_HOLD_OUT2_S_ERR_HOLD_OUT_5 = 0;
static const uint32_t EC_SD_SDP_SDCR_UNIT_HOLD_OUT2_D_T0_SS_LTAG_LTZ_HOLD_OUT = 1;
static const uint32_t EC_SD_SDP_SDCR_UNIT_HOLD_OUT2_D_T1_SS_LTAG_LTZ_HOLD_OUT = 2;
static const uint32_t EC_SD_SDP_SDCR_UNIT_HOLD_OUT2_D_T2_SS_LTAG_LTZ_HOLD_OUT = 3;
static const uint32_t EC_SD_SDP_SDCR_UNIT_HOLD_OUT2_D_T3_SS_LTAG_LTZ_HOLD_OUT = 4;
static const uint32_t EC_SD_SDP_SDCR_UNIT_HOLD_OUT2_D_T0_SS_STAG_LTZ_HOLD_OUT = 5;
static const uint32_t EC_SD_SDP_SDCR_UNIT_HOLD_OUT2_D_T1_SS_STAG_LTZ_HOLD_OUT = 6;
static const uint32_t EC_SD_SDP_SDCR_UNIT_HOLD_OUT2_D_T2_SS_STAG_LTZ_HOLD_OUT = 7;
static const uint32_t EC_SD_SDP_SDCR_UNIT_HOLD_OUT2_D_T3_SS_STAG_LTZ_HOLD_OUT = 8;
static const uint32_t EC_SD_SDP_SDCR_UNIT_HOLD_OUT2_D_T0_CCC_LTAG_LTZ_HOLD_OUT = 9;
static const uint32_t EC_SD_SDP_SDCR_UNIT_HOLD_OUT2_D_T2_CCC_LTAG_LTZ_HOLD_OUT = 10;
static const uint32_t EC_SD_SDP_SDCR_UNIT_HOLD_OUT2_D_T0_CCC_STAG_LTZ_HOLD_OUT = 11;
static const uint32_t EC_SD_SDP_SDCR_UNIT_HOLD_OUT2_D_T2_CCC_STAG_LTZ_HOLD_OUT = 12;
static const uint32_t EC_SD_SDP_SDCR_UNIT_HOLD_OUT2_P_WAT_T0_HOLD_OUT = 13;
static const uint32_t EC_SD_SDP_SDCR_UNIT_HOLD_OUT2_P_WAT_T1_HOLD_OUT = 14;
static const uint32_t EC_SD_SDP_SDCR_UNIT_HOLD_OUT2_P_WAT_T2_HOLD_OUT = 15;
//<< [EC_SD_SDP_SDCR_UNIT_HOLD_OUT2]
// c/reg00013.H

//>> [L2_L2MISC_L2CERRS_FIR_REG]
static const uint64_t L2_L2MISC_L2CERRS_FIR_REG_RW = 0x20020000ull;
static const uint64_t L2_L2MISC_L2CERRS_FIR_REG_WO_AND = 0x20020001ull;
static const uint64_t L2_L2MISC_L2CERRS_FIR_REG_WO_OR = 0x20020002ull;

static const uint32_t L2_L2MISC_L2CERRS_FIR_REG_CACHE_RD_CE = 0;
static const uint32_t L2_L2MISC_L2CERRS_FIR_REG_CACHE_RD_UE = 1;
static const uint32_t L2_L2MISC_L2CERRS_FIR_REG_CACHE_RD_SUE = 2;
static const uint32_t L2_L2MISC_L2CERRS_FIR_REG_HW_DIR_INTIATED_LINE_DELETE_OCCURRED = 3;
static const uint32_t L2_L2MISC_L2CERRS_FIR_REG_CACHE_UE_SUE_DETECTED_ON_MODIFIED_LINE_BY_CO = 4;
static const uint32_t L2_L2MISC_L2CERRS_FIR_REG_CACHE_UE_SUE_DETECTED_ON_NON_MODIFIED_LINE_BY_CO = 5;
static const uint32_t L2_L2MISC_L2CERRS_FIR_REG_DIR_CE_DETECTED = 6;
static const uint32_t L2_L2MISC_L2CERRS_FIR_REG_DIR_UE_DETECTED = 7;
static const uint32_t L2_L2MISC_L2CERRS_FIR_REG_DIR_STUCK_BIT_CE = 8;
static const uint32_t L2_L2MISC_L2CERRS_FIR_REG_DIR_SBCE_REPAIR_FAILED = 9;
static const uint32_t L2_L2MISC_L2CERRS_FIR_REG_MULTIPLE_DIR_ERRORS_DETECTED = 10;
static const uint32_t L2_L2MISC_L2CERRS_FIR_REG_LRU_READ_ERROR_DETECTED = 11;
static const uint32_t L2_L2MISC_L2CERRS_FIR_REG_RC_POWERBUS_DATA_TIMEOUT = 12;
static const uint32_t L2_L2MISC_L2CERRS_FIR_REG_NCU_POWERBUS_DATA_TIMEOUT = 13;
static const uint32_t L2_L2MISC_L2CERRS_FIR_REG_HW_CONTROL_ERROR = 14;
static const uint32_t L2_L2MISC_L2CERRS_FIR_REG_LRU_ALL_MEMBERS_IN_CGC_ARE_LINE_DELETED = 15;
static const uint32_t L2_L2MISC_L2CERRS_FIR_REG_CACHE_INHIBITED_HIT_CACHEABLE_ERROR = 16;
static const uint32_t L2_L2MISC_L2CERRS_FIR_REG_RC_LOAD_RECEIVED_PB_CRESP_ADR_ERR = 17;
static const uint32_t L2_L2MISC_L2CERRS_FIR_REG_RC_STORE_RECEIVED_PB_CRESP_ADR_ERR = 18;
static const uint32_t L2_L2MISC_L2CERRS_FIR_REG_RC_POWBUS_DATA_CE_ERR_FROM_F2CHK = 19;
static const uint32_t L2_L2MISC_L2CERRS_FIR_REG_RC_POWBUS_DATA_UE_ERR_FROM_F2CHK = 20;
static const uint32_t L2_L2MISC_L2CERRS_FIR_REG_RC_POWBUS_DATA_SUE_ERR_FROM_F2CHK = 21;
static const uint32_t L2_L2MISC_L2CERRS_FIR_REG_TGT_NODAL_REQ_DINC_ERR = 22;
static const uint32_t L2_L2MISC_L2CERRS_FIR_REG_RC_LOAD_RECEIVED_PB_CRESP_ADR_ERR_FOR_HYP = 23;
static const uint32_t L2_L2MISC_L2CERRS_FIR_REG_RCDAT_RD_PARITY_ERR = 24;
static const uint32_t L2_L2MISC_L2CERRS_FIR_REG_CO_PSH_RECEIVED_PB_CRESP_ADR_ERR = 25;
static const uint32_t L2_L2MISC_L2CERRS_FIR_REG_LVDIR_PERR = 26;
static const uint32_t L2_L2MISC_L2CERRS_FIR_REG_LPCRD_TOPOTABLE_SW_CFG_ERR = 27;
static const uint32_t L2_L2MISC_L2CERRS_FIR_REG_DARN_DATA_TIMEOUT = 28;
static const uint32_t L2_L2MISC_L2CERRS_FIR_REG_EARLY_HANG_WARNING = 29;
static const uint32_t L2_L2MISC_L2CERRS_FIR_REG_CHIP_CONTAINED_UNEXP_CO_PUSH = 30;
static const uint32_t L2_L2MISC_L2CERRS_FIR_REG_RSVD = 31;
static const uint32_t L2_L2MISC_L2CERRS_FIR_REG_PEC_PH3_TIMEOUT = 32;
static const uint32_t L2_L2MISC_L2CERRS_FIR_REG_SPARE2TO1 = 33;
static const uint32_t L2_L2MISC_L2CERRS_FIR_REG_SPARE2TO1_LEN = 3;
static const uint32_t L2_L2MISC_L2CERRS_FIR_REG_CACHE_RD_CE_AND_UE = 36;
static const uint32_t L2_L2MISC_L2CERRS_FIR_REG_SPARE1TO1 = 37;
static const uint32_t L2_L2MISC_L2CERRS_FIR_REG_SPARE1TO1_LEN = 3;
//<< [L2_L2MISC_L2CERRS_FIR_REG]
// c/reg00013.H

//>> [L3_MISC_L3CERRS_CAC_WAY_DIS_REG]
static const uint64_t L3_MISC_L3CERRS_CAC_WAY_DIS_REG = 0x2001060bull;

static const uint32_t L3_MISC_L3CERRS_CAC_WAY_DIS_REG_L3_CAC_WAY_DIS_CFG = 0;
static const uint32_t L3_MISC_L3CERRS_CAC_WAY_DIS_REG_L3_CAC_WAY_DIS_CFG_LEN = 8;
//<< [L3_MISC_L3CERRS_CAC_WAY_DIS_REG]
// c/reg00013.H

//>> [L3_MISC_L3CERRS_LINE_DELETED_MEMBERS_REG]
static const uint64_t L3_MISC_L3CERRS_LINE_DELETED_MEMBERS_REG = 0x20010615ull;

static const uint32_t L3_MISC_L3CERRS_LINE_DELETED_MEMBERS_REG_L3_LINE_DELETED_MEMBERS = 0;
static const uint32_t L3_MISC_L3CERRS_LINE_DELETED_MEMBERS_REG_L3_LINE_DELETED_MEMBERS_LEN = 5;
//<< [L3_MISC_L3CERRS_LINE_DELETED_MEMBERS_REG]
// c/reg00013.H

//>> [NC_NCMISC_NCSCOMS_INJ_REG]
static const uint64_t NC_NCMISC_NCSCOMS_INJ_REG = 0x2001064dull;

static const uint32_t NC_NCMISC_NCSCOMS_INJ_REG_STQ_ERR_INJ = 0;
static const uint32_t NC_NCMISC_NCSCOMS_INJ_REG_STQ_ERR_INJ_LEN = 2;
//<< [NC_NCMISC_NCSCOMS_INJ_REG]
// c/reg00013.H

//>> [PSCOM_MODE_REG]
static const uint64_t PSCOM_MODE_REG = 0x20010000ull;

static const uint32_t PSCOM_MODE_REG_ABORT_ON_PCB_ADDR_PARITY_ERROR = 0;
static const uint32_t PSCOM_MODE_REG_ABORT_ON_PCB_WDATA_PARITY_ERROR = 1;
static const uint32_t PSCOM_MODE_REG_ABORT_ON_DL_RETURN_WDATA_PARITY_ERROR = 3;
static const uint32_t PSCOM_MODE_REG_WATCHDOG_ENABLE = 4;
static const uint32_t PSCOM_MODE_REG_SCOM_HANG_LIMIT = 5;
static const uint32_t PSCOM_MODE_REG_SCOM_HANG_LIMIT_LEN = 2;
static const uint32_t PSCOM_MODE_REG_FORCE_ALL_RINGS = 7;
static const uint32_t PSCOM_MODE_REG_FSM_SELFRESET_ON_STATEVEC_PARITYERROR_ENABLE = 8;
static const uint32_t PSCOM_MODE_REG_RESERVED_PSCOM_MODE_LT = 9;
static const uint32_t PSCOM_MODE_REG_RESERVED_PSCOM_MODE_LT_LEN = 3;
//<< [PSCOM_MODE_REG]
// c/reg00013.H

//>>THE END<<

#ifndef __PPE_HCODE__
}
}
#include "c/reg00013_unused.H"
#endif
#endif

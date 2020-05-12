/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_ram_core.C $     */
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
///
/// @file p10_ram_core.C
/// @brief Class that implements the base ramming capability
///
/// *HWP HW Maintainer : Doug Holtsinger <Douglas.Holtsinger@ibm.com>
/// *HWP FW Maintainer : Raja Das        <rajadas2@in.ibm.com>
/// *HWP Consumed by   : SBE
///

//-----------------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------------
#include <p10_ram_core.H>
#include <p10_thread_control.H>
#include <p10_scom_c.H>

// shift/mask for inserting SPR num into opcode
const uint32_t SPR_NUM_OPCODE_SHIFT  = 11;
const uint32_t SPR_NUM_MASK_LOWER    = 0x0000001F;
const uint32_t SPR_NUM_MASK_UPPER    = 0x000003E0;
const uint32_t SPR_NUM_MASK          = (SPR_NUM_MASK_UPPER | SPR_NUM_MASK_LOWER);
const uint32_t SPR_NUM_LOWER_SHIFT_L = 5;
const uint32_t SPR_NUM_UPPER_SHIFT_R = 5;

// encode the SPR number suitable for bitwise-OR into an opcode
#define ENCODE_SPR_NUM_OPCODE(_spr_num) \
    ((((_spr_num & SPR_NUM_MASK_LOWER) << SPR_NUM_LOWER_SHIFT_L) | \
      ((_spr_num & SPR_NUM_MASK_UPPER) >> SPR_NUM_UPPER_SHIFT_R)) \
     << SPR_NUM_OPCODE_SHIFT)

// SPR numbers
const uint32_t SPR_NUM_XER             =   1;
const uint32_t SPR_NUM_LR              =   8;
const uint32_t SPR_NUM_CTR             =   9;
const uint32_t SPR_NUM_SPRG2           = 274;
const uint32_t SPR_NUM_HSPRG1          = 305;
const uint32_t SPR_NUM_LPCR            = 318;
const uint32_t SPR_NUM_SMFCTRL         = 511;
const uint32_t SPR_NUM_MMCR1_RU        = 782;
const uint32_t SPR_NUM_MMCR2           = 785;          // MTMMCR2
const uint32_t SPR_NUM_MMCRA           = 786;          // MTMMCRA
const uint32_t SPR_NUM_MMCR1           = 798;          // MTMMCR1
const uint32_t SPR_NUM_TAR             = 815;

// opcode pattern masks
const uint32_t OPCODE_MASK_TYPE = 0xFC000000;
const uint32_t OPCODE_MASK      = 0xFC0007FE;
const uint32_t OPCODE_L         = 0x00010000;
const uint32_t OPCODE_MASK_L    = (OPCODE_MASK | OPCODE_L);
const uint32_t OPCODE_MASK_SPR  = (OPCODE_MASK | (SPR_NUM_MASK << SPR_NUM_OPCODE_SHIFT));

// See PC workbook, supported RAM instruction table footnote
const uint32_t OPCODE_SPECIAL_PPC                  = 0x001E0000;

// opcode for ramming
const uint32_t OPCODE_MTSPR_FROM_GPR0_TO_SPRD      = 0x7C1543A6;
const uint32_t OPCODE_MTSPR_FROM_GPR1_TO_SPRD      = 0x7C3543A6;
const uint32_t OPCODE_MFSPR_FROM_SPRD_TO_GPR0      = 0x7C1542A6;
const uint32_t OPCODE_MFSPR_FROM_SPRD_TO_GPR1      = 0x7C3542A6;
const uint32_t OPCODE_MFSPR_FROM_SPR0_TO_GPR0      = 0x7C0002A6;
const uint32_t OPCODE_MTSPR_FROM_GPR0_TO_SPR0      = 0x7C0003A6;
const uint32_t OPCODE_MFFPRD_FROM_FPR0_TO_GPR0     = 0x7C000066;
const uint32_t OPCODE_MTFPRD_FROM_GPR0_TO_FPR0     = 0x7C000166;
const uint32_t OPCODE_MFVSRD_FROM_VSR0_TO_GPR0     = 0x7C000066;
const uint32_t OPCODE_MFVSRD_FROM_VSR32_TO_GPR0    = 0x7C000067;
const uint32_t OPCODE_MFVSRLD_FROM_VSR0_TO_GPR0    = 0x7C000266;
const uint32_t OPCODE_MFVSRLD_FROM_VSR32_TO_GPR0   = 0x7C000267;
const uint32_t OPCODE_MTVSRDD_FROM_GPR1_0_TO_VSR0  = 0x7C010366;
const uint32_t OPCODE_MTVSRDD_FROM_GPR1_0_TO_VSR32 = 0x7C010367;
const uint32_t OPCODE_MFSPR_FROM_LR_TO_GPR0        = 0x7C0802A6;
const uint32_t OPCODE_MTSPR_FROM_GPR0_TO_LR        = 0x7C0803A6;
const uint32_t OPCODE_MFFS                         = 0xFC00048E;
const uint32_t OPCODE_DCBZ                         = 0x7C0007EC;
const uint32_t OPCODE_DCBF                         = 0x7C0000AC;
const uint32_t OPCODE_LD                           = 0xE8000000;
const uint32_t OPCODE_STD                          = 0xF8000000;
const uint32_t OPCODE_LFD                          = 0xC8000000;
const uint32_t OPCODE_STFD                         = 0xD8000000;
const uint32_t OPCODE_LVX                          = 0x7C0000CE;
const uint32_t OPCODE_STVX                         = 0x7C0001CE;
const uint32_t OPCODE_LXVD2X                       = 0x7C000698;
const uint32_t OPCODE_STXVD2X                      = 0x7C000798;
const uint32_t OPCODE_MFMSR_TO_GPR0                = 0x7C0000A6;
const uint32_t OPCODE_MFMSR                        = 0x7C0000A6;
const uint32_t OPCODE_MFOCRF_TO_GPR0               = 0x7C100026;
const uint32_t OPCODE_MTOCRF_FROM_GPR0             = 0x7C100120;
const uint32_t OPCODE_MTFSF_FROM_FPR0              = 0xFE00058E;
const uint32_t OPCODE_MFVSCR_TO_VR0                = 0x10000604;
const uint32_t OPCODE_MTVSCR_FROM_VR0              = 0x10000644;
// Pseudo PPCs
const uint32_t OPCODE_MTNIA_LR                     = 0x00000002;
const uint32_t OPCODE_MFNIA_RT                     = 0x00000004;
const uint32_t OPCODE_GPR_XOR                      = 0x00000010;
const uint32_t OPCODE_VSR0_XOR                     = 0x00000110;
const uint32_t OPCODE_VSR1_XOR                     = 0x00000210;
const uint32_t OPCODE_FASTSPR_XOR                  = 0x00000310;
const uint32_t OPCODE_GPR_MOVE_RECONFIG            = 0x00000410;
const uint32_t OPCODE_VSR0_MOVE_RECONFIG           = 0x00000510;
const uint32_t OPCODE_VSR1_MOVE_RECONFIG           = 0x00000610;
const uint32_t OPCODE_FASTSPR_MOVE_RECONFIG        = 0x00000710;
const uint32_t OPCODE_XFVC_MOVE                    = 0x00000810;
const uint32_t OPCODE_MMA_DPRIME                   = 0x00000910;
const uint32_t OPCODE_MMM_PRIME                    = 0x00000A10;
const uint32_t OPCODE_STF_REBALANCE                = 0x00000B10;
const uint32_t OPCODE_GPR_MOVE_RECOVERY            = 0x00000C10;
const uint32_t OPCODE_VSR0_MOVE_RECOVERY           = 0x00000D10;
const uint32_t OPCODE_VSR1_MOVE_RECOVERY           = 0x00000E10;
const uint32_t OPCODE_FASTSPR_MOVE_RECOVERY        = 0x00000F10;

const uint32_t OPCODE_MFSPR                        = 0x7C0002A6;
const uint32_t OPCODE_MTSPR                        = 0x7C0003A6;
const uint32_t OPCODE_SLBMFEE                      = 0x7C000726;
const uint32_t OPCODE_SLBMFEV                      = 0x7C0006A6;
const uint32_t OPCODE_MTMSR                        = 0x7C000124;
const uint32_t OPCODE_MTMSR_L1                     = (OPCODE_MTMSR | OPCODE_L);
const uint32_t OPCODE_MTMSRD                       = 0x7C000164;
const uint32_t OPCODE_MTMSRD_L0                    = OPCODE_MTMSRD;
const uint32_t OPCODE_MTMSR_L0                     = OPCODE_MTMSR;
const uint32_t OPCODE_MTMSRD_L1                    = (OPCODE_MTMSRD | OPCODE_L);

const uint32_t OPCODE_MFSPR_SPRG2   = OPCODE_MFSPR | ENCODE_SPR_NUM_OPCODE(SPR_NUM_SPRG2) ; // 0x7C1242A6
const uint32_t OPCODE_MFSPR_XER     = OPCODE_MFSPR | ENCODE_SPR_NUM_OPCODE(SPR_NUM_XER);  // 0x7C0102A6
const uint32_t OPCODE_MFSPR_CTR     = OPCODE_MFSPR | ENCODE_SPR_NUM_OPCODE(SPR_NUM_CTR);
const uint32_t OPCODE_MFSPR_HSPRG1  = OPCODE_MFSPR | ENCODE_SPR_NUM_OPCODE(SPR_NUM_HSPRG1);
const uint32_t OPCODE_MFSPR_LR      = OPCODE_MFSPR | ENCODE_SPR_NUM_OPCODE(SPR_NUM_LR);
const uint32_t OPCODE_MFSPR_TAR     = OPCODE_MFSPR | ENCODE_SPR_NUM_OPCODE(SPR_NUM_TAR);

const uint32_t OPCODE_MTSPR_SPRG2   = OPCODE_MTSPR | ENCODE_SPR_NUM_OPCODE(SPR_NUM_SPRG2) ; // 0x7C1243A6
const uint32_t OPCODE_MTSPR_XER     = OPCODE_MTSPR | ENCODE_SPR_NUM_OPCODE(SPR_NUM_XER);  // 0x7C0103A6
const uint32_t OPCODE_MTSPR_CTR     = OPCODE_MTSPR | ENCODE_SPR_NUM_OPCODE(SPR_NUM_CTR);
const uint32_t OPCODE_MTSPR_LPCR    = OPCODE_MTSPR | ENCODE_SPR_NUM_OPCODE(SPR_NUM_LPCR); // 0x7C1E4BA6
const uint32_t OPCODE_MTSPR_MMCR1   = OPCODE_MTSPR | ENCODE_SPR_NUM_OPCODE(SPR_NUM_MMCR1);
const uint32_t OPCODE_MTSPR_MMCR2   = OPCODE_MTSPR | ENCODE_SPR_NUM_OPCODE(SPR_NUM_MMCR2);
const uint32_t OPCODE_MTSPR_MMCRA   = OPCODE_MTSPR | ENCODE_SPR_NUM_OPCODE(SPR_NUM_MMCRA);
const uint32_t OPCODE_MTSPR_SMFCTRL = OPCODE_MTSPR | ENCODE_SPR_NUM_OPCODE(SPR_NUM_SMFCTRL);
const uint32_t OPCODE_MTSPR_HSPRG1  = OPCODE_MTSPR | ENCODE_SPR_NUM_OPCODE(SPR_NUM_HSPRG1);
const uint32_t OPCODE_MTSPR_LR      = OPCODE_MTSPR | ENCODE_SPR_NUM_OPCODE(SPR_NUM_LR);
const uint32_t OPCODE_MTSPR_TAR     = OPCODE_MTSPR | ENCODE_SPR_NUM_OPCODE(SPR_NUM_TAR);


// poll count for check ram status
const uint32_t RAM_CORE_STAT_POLL_CNT = 10;
// poll count for check ram ready.  This can take about 3000 core clock cycles in P10.
const uint32_t RAM_CORE_READY_POLL_CNT = 100;

// register SPR_COMMON.SCOMC , definition for field MODE_CX_SCOMC
// where field value = SCRATCH xxx SPR
const uint32_t EC_PC_COMMON_SPR_SCOMC_MODE_CX_SCR0 = 0;

//-----------------------------------------------------------------------------------
// Namespace declarations
//-----------------------------------------------------------------------------------

using namespace scomt;
using namespace scomt::c;

//-----------------------------------------------------------------------------------
// Function definitions
// Note: All function doxygen can be found in class definition in header file.
//-----------------------------------------------------------------------------------
RamCore::RamCore(const fapi2::Target<fapi2::TARGET_TYPE_CORE>& i_target,
                 const uint8_t i_thread)
{
    iv_target = i_target;
    iv_thread = i_thread;
    iv_ram_enable    = false;
    iv_ram_scr0_save = false;
    iv_ram_setup     = false;
    iv_ram_err       = false;
    iv_write_gpr0    = false;
    iv_write_gpr1    = false;
    iv_backup_scr0   = 0;
    iv_backup_gpr0   = 0;
    iv_backup_gpr1   = 0;
    iv_thread_activated = false;
}

// Sequence of operations in put_reg and get_reg
// functions follow the below template of a putscom
// before the opcode ramming and a getscom post-ramming,
// or a combination of these. This structure can be
// used to streamline these operations with an array
// of this structure.
// If the pointers are null, corresponding operations are skipped.
typedef struct
{
    // putscom to be done before ramming opcode
    fapi2::buffer<uint64_t>* prePutScom;
    // ramming opcode
    uint32_t opCode;
    // getscom to be done after ramming opcode
    fapi2::buffer<uint64_t>* postGetScom;
} putreg_op_t;

RamCore::~RamCore()
{
    if(iv_ram_setup)
    {
        FAPI_ERR("RamCore Destructor error: Ram is still in active state!!!");
    }
}

// See doxygen comments in header file
fapi2::ReturnCode RamCore::ram_ready(bool& o_ready, fapi2::buffer<uint64_t>& o_thread_info)
{
    FAPI_DBG("Start ram_ready");
    bool l_thread_active = false;
    bool l_thread_action_in_progress = false;

    FAPI_TRY(GET_EC_PC_PMC_THREAD_INFO(iv_target, o_thread_info));
    FAPI_DBG("EC_PC_PMC_THREAD_INFO: 0x%.16llX", o_thread_info());

    FAPI_TRY(o_thread_info.extractToRight(l_thread_active,
                                          EC_PC_PMC_THREAD_INFO_THREAD_INFO_VTID0_V + iv_thread, 1));

    l_thread_action_in_progress = GET_EC_PC_PMC_THREAD_INFO_THREAD_ACTION_IN_PROGRESS(o_thread_info);

    o_ready = l_thread_active && !l_thread_action_in_progress;

fapi_try_exit:
    FAPI_DBG("Exiting ram_ready");
    return fapi2::current_err;
}

// See doxygen comments in header file
fapi2::ReturnCode RamCore::ram_setup()
{
    FAPI_DBG("Start ram setup");
    fapi2::buffer<uint64_t> l_data = 0;
    uint32_t l_opcode = 0;
    ThreadSpecifier l_thread = NO_THREADS;
    fapi2::buffer<uint64_t> l_ras_status;
    uint64_t l_thread_state = 0;
    fapi2::ReturnCode rc_fapi(fapi2::FAPI2_RC_SUCCESS);
    bool l_thread_ready = false;
    uint32_t l_poll_count = RAM_CORE_READY_POLL_CNT;

    switch (iv_thread)
    {
        case 0:
            l_thread = THREAD0;
            break;

        case 1:
            l_thread = THREAD1;
            break;

        case 2:
            l_thread = THREAD2;
            break;

        case 3:
            l_thread = THREAD3;
            break;

        default:
            l_thread = NO_THREADS ;
            break;
    }

    FAPI_ASSERT(l_thread != NO_THREADS,
                fapi2::P10_RAM_THREAD_INVALID_ERR()
                .set_CORE_TARGET(iv_target)
                .set_THREAD(iv_thread),
                "Invalid ram thread specified.");

    // query thread to check the thread is stopped for ramming
    FAPI_EXEC_HWP(rc_fapi,
                  p10_thread_control,
                  iv_target,
                  l_thread,
                  PTC_CMD_QUERY,
                  false,
                  l_ras_status,
                  l_thread_state);

    FAPI_ASSERT((l_thread_state & THREAD_STATE_STOP) == THREAD_STATE_STOP,
                fapi2::P10_RAM_THREAD_NOT_STOP_ERR()
                .set_CORE_TARGET(iv_target)
                .set_THREAD(iv_thread),
                "Thread to perform ram is not stopped. "
                "RAS_STATUS register : 0x%.16llX", l_ras_status());

    // check the thread is ready for RAMing
    FAPI_TRY(ram_ready(l_thread_ready, l_data));

    while (!l_thread_ready && l_poll_count > 0)
    {
        // If thread not ready, then try to make thread ready
        if (!iv_thread_activated)
        {
            FAPI_TRY(l_data.setBit(EC_PC_PMC_THREAD_INFO_RAM_THREAD_ACTIVE + iv_thread));
            FAPI_TRY(PUT_EC_PC_PMC_THREAD_INFO(iv_target, l_data));
            iv_thread_activated = true;
        }

        FAPI_TRY(ram_ready(l_thread_ready, l_data));

        l_poll_count--;
    }

    FAPI_ASSERT(l_thread_ready,
                fapi2::P10_RAM_THREAD_INACTIVE_ERR()
                .set_CORE_TARGET(iv_target)
                .set_THREAD(iv_thread),
                "Thread to perform ram is inactive. "
                "EC_PC_PMC_THREAD_INFO reg 0x%.16llX", l_data);

    // set RAM_MODEREG via Scom to enable RAM mode
    FAPI_TRY(GET_EC_PC_FIR_RAM_MODEREG(iv_target, l_data));
    SET_EC_PC_FIR_RAM_MODEREG_RAM_MODE_ENABLE(l_data);
    FAPI_TRY(PUT_EC_PC_FIR_RAM_MODEREG(iv_target, l_data));

    iv_ram_enable = true;

    //
    // backup Core registers SCR0/GPR0/GPR1 since they can be overwritten during the
    // RAM opcode procedures.
    //

    //
    // backup SCR0
    //
    FAPI_TRY(GET_EC_PC_SCR0(iv_target, iv_backup_scr0));
    iv_ram_scr0_save = true;

    //
    // backup GPR0
    //

    // set Enable bits to allow a single SCOM write to SCOMC to be broadcast
    // to all of the per-logical thread SPRC registers
    FAPI_TRY(GET_EC_PC_COMMON_SPR_MODE(iv_target, l_data));
    SET_EC_PC_COMMON_SPR_MODE_SPRC_LT0_SEL(l_data);
    SET_EC_PC_COMMON_SPR_MODE_SPRC_LT1_SEL(l_data);
    SET_EC_PC_COMMON_SPR_MODE_SPRC_LT2_SEL(l_data);
    SET_EC_PC_COMMON_SPR_MODE_SPRC_LT3_SEL(l_data);
    SET_EC_PC_COMMON_SPR_MODE_SPRC_LT4_SEL(l_data);
    SET_EC_PC_COMMON_SPR_MODE_SPRC_LT5_SEL(l_data);
    SET_EC_PC_COMMON_SPR_MODE_SPRC_LT6_SEL(l_data);
    SET_EC_PC_COMMON_SPR_MODE_SPRC_LT7_SEL(l_data);
    FAPI_TRY(PUT_EC_PC_COMMON_SPR_MODE(iv_target, l_data));

    //1.setup SPRC to use SPRD as an alias to SCR0
    FAPI_TRY(GET_EC_PC_COMMON_SPR_SCOMC(iv_target, l_data));
    SET_EC_PC_COMMON_SPR_SCOMC_MODE_CX_SCOMC(EC_PC_COMMON_SPR_SCOMC_MODE_CX_SCR0, l_data);
    FAPI_TRY(PUT_EC_PC_COMMON_SPR_SCOMC(iv_target, l_data));

    //2.create mtsprd<gpr0> opcode, ram into thread to get GPR0
    l_opcode = OPCODE_MTSPR_FROM_GPR0_TO_SPRD;
    FAPI_TRY(ram_opcode(l_opcode, true));

    //3.get GPR0 from SCR0
    FAPI_TRY(GET_EC_PC_SCR0(iv_target, iv_backup_gpr0));

    //
    // backup GPR1
    //

    //1.create mtsprd<gpr1> opcode, ram into thread to get GPR1
    l_opcode = OPCODE_MTSPR_FROM_GPR1_TO_SPRD;
    FAPI_TRY(ram_opcode(l_opcode, true));

    //2.get GPR1 from SCR0
    FAPI_TRY(GET_EC_PC_SCR0(iv_target, iv_backup_gpr1));

    iv_ram_setup = true;

fapi_try_exit:

    // Error happened and SCR0 saved, to restore SCR0
    // Do not use "FAPI_TRY" to avoid endless loop
    if((fapi2::current_err != fapi2::FAPI2_RC_SUCCESS) && iv_ram_scr0_save)
    {
        PREP_EC_PC_SCR0(iv_target);
        PUT_EC_PC_SCR0(iv_target, iv_backup_scr0);
    }

    FAPI_DBG("Exiting ram setup");
    return fapi2::current_err;
}

// See doxygen comments in header file
fapi2::ReturnCode RamCore::ram_cleanup()
{
    FAPI_DBG("Start ram cleanup");
    uint32_t l_opcode = 0;
    fapi2::buffer<uint64_t> l_data = 0;

    FAPI_ASSERT(iv_ram_setup,
                fapi2::P10_RAM_NOT_SETUP_ERR()
                .set_CORE_TARGET(iv_target),
                "Attempting to call ram_cleanup without calling ram_setup before");

    // set Enable bits to allow a single SCOM write to SCOMC to be broadcast
    // to all of the per-logical thread SPRC registers
    FAPI_TRY(GET_EC_PC_COMMON_SPR_MODE(iv_target, l_data));
    SET_EC_PC_COMMON_SPR_MODE_SPRC_LT0_SEL(l_data);
    SET_EC_PC_COMMON_SPR_MODE_SPRC_LT1_SEL(l_data);
    SET_EC_PC_COMMON_SPR_MODE_SPRC_LT2_SEL(l_data);
    SET_EC_PC_COMMON_SPR_MODE_SPRC_LT3_SEL(l_data);
    SET_EC_PC_COMMON_SPR_MODE_SPRC_LT4_SEL(l_data);
    SET_EC_PC_COMMON_SPR_MODE_SPRC_LT5_SEL(l_data);
    SET_EC_PC_COMMON_SPR_MODE_SPRC_LT6_SEL(l_data);
    SET_EC_PC_COMMON_SPR_MODE_SPRC_LT7_SEL(l_data);
    FAPI_TRY(PUT_EC_PC_COMMON_SPR_MODE(iv_target, l_data));

    // setup SPRC to use SPRD as an alias to SCR0
    FAPI_TRY(GET_EC_PC_COMMON_SPR_SCOMC(iv_target, l_data));
    SET_EC_PC_COMMON_SPR_SCOMC_MODE_CX_SCOMC(EC_PC_COMMON_SPR_SCOMC_MODE_CX_SCR0, l_data);
    FAPI_TRY(PUT_EC_PC_COMMON_SPR_SCOMC(iv_target, l_data));

    //
    // restore Core registers SCR0/GPR0/GPR1 since they can be overwritten during the
    // RAM opcode procedures.
    //

    // restore GPR1
    if(!iv_write_gpr1)
    {
        //iv_backup_gpr1->GPR1
        //1.put restore data into SCR0
        PREP_EC_PC_SCR0(iv_target);
        FAPI_TRY(PUT_EC_PC_SCR0(iv_target, iv_backup_gpr1));

        //2.create mfsprd<gpr1> opcode, ram into thread to restore GPR1
        l_opcode = OPCODE_MFSPR_FROM_SPRD_TO_GPR1;
        FAPI_TRY(ram_opcode(l_opcode, true));
    }

    // restore GPR0
    if(!iv_write_gpr0)
    {
        //iv_backup_gpr0->GPR0
        //1.put restore data into SCR0
        PREP_EC_PC_SCR0(iv_target);
        FAPI_TRY(PUT_EC_PC_SCR0(iv_target, iv_backup_gpr0));

        //2.create mfsprd<gpr0> opcode, ram into thread to restore GPR0
        l_opcode = OPCODE_MFSPR_FROM_SPRD_TO_GPR0;
        FAPI_TRY(ram_opcode(l_opcode, true));
    }

    // restore SCR0
    PREP_EC_PC_SCR0(iv_target);
    FAPI_TRY(PUT_EC_PC_SCR0(iv_target, iv_backup_scr0));

    // set RAM_MODEREG Scom to clear RAM mode
    FAPI_TRY(GET_EC_PC_FIR_RAM_MODEREG(iv_target, l_data));
    CLEAR_EC_PC_FIR_RAM_MODEREG_RAM_MODE_ENABLE(l_data);
    FAPI_TRY(PUT_EC_PC_FIR_RAM_MODEREG(iv_target, l_data));

    // clear RAM thread active
    if (iv_thread_activated)
    {
        FAPI_TRY(GET_EC_PC_PMC_THREAD_INFO(iv_target, l_data));
        FAPI_TRY(l_data.clearBit(EC_PC_PMC_THREAD_INFO_RAM_THREAD_ACTIVE + iv_thread));
        FAPI_TRY(PUT_EC_PC_PMC_THREAD_INFO(iv_target, l_data));
        iv_thread_activated = false;
    }

    iv_ram_enable    = false;
    iv_ram_scr0_save = false;
    iv_ram_setup     = false;
    iv_write_gpr0    = false;
    iv_write_gpr1    = false;

fapi_try_exit:
    FAPI_DBG("Exiting ram cleanup");
    return fapi2::current_err;
}

// See doxygen comments in header file
fapi2::ReturnCode RamCore::ram_opcode(const uint32_t i_opcode,
                                      const bool i_allow_mult)
{
    FAPI_DBG("Start ram opcode");
    fapi2::buffer<uint64_t> l_data = 0;
    uint8_t  l_predecode = 0;
    uint32_t l_poll_count = RAM_CORE_STAT_POLL_CNT;
    bool l_is_load_store = false;

    // check the opcode for load/store
    l_is_load_store = is_load_store(i_opcode);

    // ram_setup
    if(!i_allow_mult)
    {
        FAPI_TRY(ram_setup());
    }

    FAPI_ASSERT(iv_ram_enable,
                fapi2::P10_RAM_NOT_SETUP_ERR()
                .set_CORE_TARGET(iv_target),
                "Attempting to ram opcode without enable RAM mode before");

    // write RAM_CTRL Scom for ramming the opcode
    PREP_EC_PC_FIR_RAM_CTRL(iv_target);
    l_data.flush<0>();
    l_data.insertFromRight<EC_PC_FIR_RAM_CTRL_RAM_VTID, EC_PC_FIR_RAM_CTRL_RAM_VTID_LEN>(iv_thread);
    l_predecode = gen_predecode(i_opcode);
    l_data.insertFromRight<EC_PC_FIR_RAM_CTRL_PPC_PREDCD, EC_PC_FIR_RAM_CTRL_PPC_PREDCD_LEN>(l_predecode);
    l_data.insertFromRight<EC_PC_FIR_RAM_CTRL_PPC_INSTR, EC_PC_FIR_RAM_CTRL_PPC_INSTR_LEN>(i_opcode);
    FAPI_TRY(PUT_EC_PC_FIR_RAM_CTRL(iv_target, l_data));

    // poll RAM_STATUS_REG Scom for the completion
    while(1)
    {
        FAPI_TRY(GET_EC_PC_FIR_RAM_STATUS(iv_target, l_data));

        // attempting to ram during recovery
        FAPI_ASSERT(!l_data.getBit<EC_PC_FIR_RAM_STATUS_RAM_CONTROL_ACCESS_DURING_RECOV>(),
                    fapi2::P10_RAM_STATUS_IN_RECOVERY_ERR()
                    .set_CORE_TARGET(iv_target),
                    "Attempting to ram during recovery. "
                    "EC_PC_FIR_RAM_STATUS reg 0x%.16llX", l_data);

        // exception or interrupt
        FAPI_ASSERT(!l_data.getBit<EC_PC_FIR_RAM_STATUS_RAM_EXCEPTION>(),
                    fapi2::P10_RAM_STATUS_EXCEPTION_ERR()
                    .set_CORE_TARGET(iv_target),
                    "Exception or interrupt happened during ramming. "
                    "EC_PC_FIR_RAM_STATUS reg 0x%.16llX", l_data);

        // load/store opcode need to check LSU empty and PPC complete
        if (l_is_load_store)
        {
            if ( l_data.getBit<EC_PC_FIR_RAM_STATUS_RAM_COMPLETION>() &&
                 l_data.getBit<EC_PC_FIR_RAM_STATUS_LSU_EMPTY>() )
            {
                FAPI_DBG("ram_opcode:: RAM is done");
                break;
            }
        }
        else
        {
            if ( l_data.getBit<EC_PC_FIR_RAM_STATUS_RAM_COMPLETION>() )
            {
                FAPI_DBG("ram_opcode:: RAM is done");
                break;
            }
        }

        --l_poll_count;

        FAPI_ASSERT(l_poll_count > 0,
                    fapi2::P10_RAM_STATUS_POLL_THRESHOLD_ERR()
                    .set_CORE_TARGET(iv_target),
                    "Timeout for ram to complete, poll count expired. "
                    "EC_PC_FIR_RAM_STATUS reg 0x%.16llX", l_data);
    }

    // ram_cleanup
    if(!i_allow_mult)
    {
        FAPI_TRY(ram_cleanup());
    }

fapi_try_exit:

    if(fapi2::current_err != fapi2::FAPI2_RC_SUCCESS)
    {
        iv_ram_err = true;
    }

    FAPI_DBG("Exiting ram opcode");
    return fapi2::current_err;
}


// See doxygen comments in header file
uint8_t RamCore::gen_predecode(const uint32_t i_opcode)
{
    uint8_t  l_predecode = 0;
    const uint32_t l_opcode_pattern    = i_opcode & OPCODE_MASK;
    const uint32_t l_opcode_pattern_L  = i_opcode & OPCODE_MASK_L;
    const uint32_t l_opcode_pattern_spr = i_opcode & OPCODE_MASK_SPR;

    if (l_opcode_pattern_spr == OPCODE_MFSPR_XER ||
        l_opcode_pattern_spr == OPCODE_MTSPR_LPCR ||
        l_opcode_pattern_spr == OPCODE_MTSPR_MMCR1 ||
        l_opcode_pattern_spr == OPCODE_MTSPR_MMCR2 ||
        l_opcode_pattern_spr == OPCODE_MTSPR_MMCRA ||
        l_opcode_pattern_spr == OPCODE_MTSPR_SMFCTRL)
    {
        l_predecode = 6;
    }
    else if (i_opcode == OPCODE_MTNIA_LR)
    {
        l_predecode = 0xE;
    }
    else if (l_opcode_pattern_L == OPCODE_MTMSR ||
             l_opcode_pattern_L == OPCODE_MTMSRD)
    {
        l_predecode = 9;
    }
    else if (l_opcode_pattern_spr == OPCODE_MTSPR_XER)
    {
        l_predecode = 5;
    }
    else if (l_opcode_pattern_spr == OPCODE_MFSPR_SPRG2  ||
             l_opcode_pattern_spr == OPCODE_MTSPR_SPRG2  ||
             l_opcode_pattern_spr == OPCODE_MFSPR_CTR    ||
             l_opcode_pattern_spr == OPCODE_MTSPR_CTR    ||
             l_opcode_pattern_spr == OPCODE_MFSPR_HSPRG1 ||
             l_opcode_pattern_spr == OPCODE_MTSPR_HSPRG1 ||
             l_opcode_pattern_spr == OPCODE_MFSPR_LR     ||
             l_opcode_pattern_spr == OPCODE_MTSPR_LR     ||
             l_opcode_pattern_spr == OPCODE_MFSPR_TAR    ||
             l_opcode_pattern_spr == OPCODE_MTSPR_TAR)
    {
        l_predecode = 0;
    }
    else if (i_opcode           == OPCODE_MFNIA_RT ||
             l_opcode_pattern   == OPCODE_MFSPR ||
             l_opcode_pattern   == OPCODE_MTSPR ||
             l_opcode_pattern_L == OPCODE_MTMSR_L1 ||
             l_opcode_pattern_L == OPCODE_MTMSRD_L1 ||
             l_opcode_pattern   == OPCODE_MFFS ||
             l_opcode_pattern   == OPCODE_MFMSR ||
             l_opcode_pattern   == OPCODE_SLBMFEE ||
             l_opcode_pattern   == OPCODE_SLBMFEV ||
             l_opcode_pattern   == OPCODE_MFVSCR_TO_VR0 ||
             l_opcode_pattern   == OPCODE_MTVSCR_FROM_VR0)
    {
        l_predecode = 1;
    }
    else if (i_opcode == OPCODE_GPR_XOR ||
             i_opcode == OPCODE_VSR0_XOR ||
             i_opcode == OPCODE_VSR1_XOR ||
             i_opcode == OPCODE_FASTSPR_XOR ||
             i_opcode == OPCODE_GPR_MOVE_RECONFIG ||
             i_opcode == OPCODE_VSR0_MOVE_RECONFIG ||
             i_opcode == OPCODE_VSR1_MOVE_RECONFIG ||
             i_opcode == OPCODE_FASTSPR_MOVE_RECONFIG ||
             i_opcode == OPCODE_XFVC_MOVE ||
             i_opcode == OPCODE_MMA_DPRIME ||
             i_opcode == OPCODE_MMM_PRIME ||
             i_opcode == OPCODE_STF_REBALANCE ||
             i_opcode == OPCODE_GPR_MOVE_RECOVERY ||
             i_opcode == OPCODE_VSR0_MOVE_RECOVERY ||
             i_opcode == OPCODE_VSR1_MOVE_RECOVERY ||
             i_opcode == OPCODE_FASTSPR_MOVE_RECOVERY)
    {
        l_predecode = 7;
    }

    return l_predecode;
}


// See doxygen comments in header file
bool RamCore::is_load_store(const uint32_t i_opcode)
{
    bool l_load_store = false;
    uint32_t l_opcode_pattern = i_opcode & OPCODE_MASK;
    uint32_t l_opcode_pattern_type = i_opcode & OPCODE_MASK_TYPE;

    if((l_opcode_pattern      == OPCODE_DCBZ)   ||
       (l_opcode_pattern      == OPCODE_DCBF)   ||
       (l_opcode_pattern_type == OPCODE_LD)     ||
       (l_opcode_pattern_type == OPCODE_LFD)    ||
       (l_opcode_pattern_type == OPCODE_STD)    ||
       (l_opcode_pattern_type == OPCODE_STFD)   ||
       (l_opcode_pattern      == OPCODE_LVX)    ||
       (l_opcode_pattern      == OPCODE_STVX)   ||
       (l_opcode_pattern      == OPCODE_LXVD2X) ||
       (l_opcode_pattern      == OPCODE_STXVD2X))
    {
        l_load_store = true;
    }

    return l_load_store;
}

// See doxygen comments in header file
fapi2::ReturnCode RamCore::get_reg(const Enum_RegType i_type,
                                   const uint32_t i_reg_num,
                                   fapi2::buffer<uint64_t>* o_buffer,
                                   const bool i_allow_mult)
{
    FAPI_DBG("Start get register");
    uint32_t l_opcode = 0;
    fapi2::buffer<uint64_t> l_backup_gpr0 = 0;
    fapi2::buffer<uint64_t> l_backup_fpr0 = 0;
    fapi2::buffer<uint64_t> l_backup_vr0_dw0 = 0;
    fapi2::buffer<uint64_t> l_backup_vr0_dw1 = 0;
    fapi2::buffer<uint64_t> l_crf_data = 0;
    uint64_t l_cr_data = 0;
    uint32_t l_cr_num = 0;
    static const size_t MAX_OPCODES = 10;
    putreg_op_t opcodes[MAX_OPCODES] = {};
    bool array_op = false;

    // ram_setup
    if(!i_allow_mult)
    {
        FAPI_TRY(ram_setup());
    }

    FAPI_ASSERT(iv_ram_setup,
                fapi2::P10_RAM_NOT_SETUP_ERR()
                .set_CORE_TARGET(iv_target)
                .set_REG_TYPE(i_type)
                .set_REG_NUM(i_reg_num)
                .set_ALLOW_MULT(i_allow_mult),
                "Attempting to cleanup ram without setup before. "
                "Reg type: %u, Reg num: %u, Allow Mult: %u",
                i_type, i_reg_num, i_allow_mult);

    //backup GPR0 if it is written
    if(iv_write_gpr0)
    {
        l_opcode = OPCODE_MTSPR_FROM_GPR0_TO_SPRD;
        FAPI_TRY(ram_opcode(l_opcode, true));
        FAPI_TRY(GET_EC_PC_SCR0(iv_target, l_backup_gpr0));
    }

    // get register value
    if(i_type == REG_GPR)
    {
        //1.create mtsprd<i_reg_num> opcode, ram into thread
        //2.get GPR value from SCR0
        opcodes[0] = {NULL, OPCODE_MTSPR_FROM_GPR0_TO_SPRD + (i_reg_num << 21), &o_buffer[0]};

        array_op = true;
    }
    else if(i_type == REG_SPR)
    {
        if(i_reg_num == RAM_REG_CR)
        {
            for(l_cr_num = 0; l_cr_num < 8; l_cr_num++)
            {
                //1.create mfocrf opcode, ram into thread
                l_opcode = OPCODE_MFOCRF_TO_GPR0 | ((0x00080000 >> l_cr_num) & 0x000FF000);
                FAPI_TRY(ram_opcode(l_opcode, true));

                //2.get CR value from SCR0
                l_opcode = OPCODE_MTSPR_FROM_GPR0_TO_SPRD;
                FAPI_TRY(ram_opcode(l_opcode, true));

                FAPI_TRY(GET_EC_PC_SCR0(iv_target, l_crf_data));
                l_cr_data |= l_crf_data();
            }

            o_buffer[0] = l_cr_data;
        }
        else
        {
            if(i_reg_num == RAM_REG_NIA)
            {
                //1.ram MFNIA_RT opcode
                opcodes[0] = {NULL, OPCODE_MFNIA_RT, NULL};
                //2.get NIA from GPR0
                opcodes[1] = {NULL, OPCODE_MTSPR_FROM_GPR0_TO_SPRD, &o_buffer[0]};
            }
            else if(i_reg_num == RAM_REG_MSR ||
                    i_reg_num == RAM_REG_MSRD ||
                    i_reg_num == RAM_REG_MSR_L1 ||
                    i_reg_num == RAM_REG_MSRD_L1)
            {
                //1.create mfmsr opcode, ram into thread
                opcodes[0] = {NULL, OPCODE_MFMSR_TO_GPR0, NULL};
                //2.get MSR value from SCR0
                opcodes[1] = {NULL, OPCODE_MTSPR_FROM_GPR0_TO_SPRD, &o_buffer[0]};
            }
            else if(i_reg_num == RAM_REG_FPSCR)
            {
                //1.backup FPR0
                opcodes[0] = {NULL, OPCODE_MFFPRD_FROM_FPR0_TO_GPR0, NULL};
                opcodes[1] = {NULL, OPCODE_MTSPR_FROM_GPR0_TO_SPRD, &l_backup_fpr0};
                //2.create mffs opcode, ram into thread
                opcodes[2] = {NULL, OPCODE_MFFS, NULL};
                //3.get FPSCR value from SCR0
                opcodes[3] = {NULL, OPCODE_MFFPRD_FROM_FPR0_TO_GPR0, NULL};
                opcodes[4] = {NULL, OPCODE_MTSPR_FROM_GPR0_TO_SPRD, &o_buffer[0]};
                //4.restore FPR0
                opcodes[5] = {&l_backup_fpr0, OPCODE_MFSPR_FROM_SPRD_TO_GPR0, NULL};
                opcodes[6] = {NULL, OPCODE_MTFPRD_FROM_GPR0_TO_FPR0, NULL};
            }
            else if(i_reg_num == RAM_REG_VSCR)
            {
                //1.backup VR0
                opcodes[0] = {NULL, OPCODE_MFVSRD_FROM_VSR32_TO_GPR0, NULL};
                opcodes[1] = {NULL, OPCODE_MTSPR_FROM_GPR0_TO_SPRD, &l_backup_vr0_dw0};
                opcodes[2] = {NULL, OPCODE_MFVSRLD_FROM_VSR32_TO_GPR0, NULL};
                opcodes[3] = {NULL, OPCODE_MTSPR_FROM_GPR0_TO_SPRD, &l_backup_vr0_dw1};
                //2.create mfvscr opcode, ram into thread
                opcodes[4] = {NULL, OPCODE_MFVSCR_TO_VR0, NULL};
                //3.get VSCR value from SCR0
                opcodes[5] = {NULL, OPCODE_MFVSRLD_FROM_VSR32_TO_GPR0, NULL};
                opcodes[6] = {NULL, OPCODE_MTSPR_FROM_GPR0_TO_SPRD, &o_buffer[0]};
                //4.restore VR0
                opcodes[7] = {&l_backup_vr0_dw1, OPCODE_MFSPR_FROM_SPRD_TO_GPR0, NULL};
                opcodes[8] = {&l_backup_vr0_dw0, OPCODE_MFSPR_FROM_SPRD_TO_GPR0 + (1 << 21), NULL};
                opcodes[9] = {NULL, OPCODE_MTVSRDD_FROM_GPR1_0_TO_VSR32, NULL};
            }
            else
            {
                //1.create mfspr<gpr0, i_reg_num> opcode, ram into thread
                opcodes[0] = {NULL, OPCODE_MFSPR_FROM_SPR0_TO_GPR0 | ENCODE_SPR_NUM_OPCODE(i_reg_num), NULL};
                //2.create mtsprd<gpr0> opcode, ram into thread
                //3.get GPR value from SCR0
                opcodes[1] = {NULL, OPCODE_MTSPR_FROM_GPR0_TO_SPRD, &o_buffer[0]};
            }

            array_op = true;
        }
    }
    else if(i_type == REG_FPR)
    {
        //1.create mffprd<gpr0, i_reg_num>#SX=0 opcode, ram into thread
        opcodes[0] = {NULL, OPCODE_MFFPRD_FROM_FPR0_TO_GPR0 + (i_reg_num << 21), NULL};
        //2.create mtsprd<gpr0> opcode, ram into thread
        //3.get GPR value from SCR0
        opcodes[1] = {NULL, OPCODE_MTSPR_FROM_GPR0_TO_SPRD, &o_buffer[0]};

        array_op = true;
    }

#ifndef __PPE__
    else if(i_type == REG_VSR)
    {
        //1.create mfvsrd<gpr0, i_reg_num> opcode, ram into thread to get dw0
        if(i_reg_num < 32)
        {
            l_opcode = OPCODE_MFVSRD_FROM_VSR0_TO_GPR0;
            l_opcode += (i_reg_num << 21);
        }
        else
        {
            l_opcode = OPCODE_MFVSRD_FROM_VSR32_TO_GPR0;
            l_opcode += ((i_reg_num - 32) << 21);
        }

        FAPI_TRY(ram_opcode(l_opcode, true));

        //2.create mtsprd<gpr0> opcode, ram into thread
        l_opcode = OPCODE_MTSPR_FROM_GPR0_TO_SPRD;
        FAPI_TRY(ram_opcode(l_opcode, true));

        //3.get VSR dw0 value from SCR0
        FAPI_TRY(GET_EC_PC_SCR0(iv_target, o_buffer[0]));

        //4.create mfvrld<gpr0, i_reg_num> opcode, ram into thread to get dw1
        if(i_reg_num < 32)
        {
            l_opcode = OPCODE_MFVSRLD_FROM_VSR0_TO_GPR0;
            l_opcode += (i_reg_num << 21);
        }
        else
        {
            l_opcode = OPCODE_MFVSRLD_FROM_VSR32_TO_GPR0;
            l_opcode += ((i_reg_num - 32) << 21);
        }

        FAPI_TRY(ram_opcode(l_opcode, true));

        //5.create mtsprd<gpr0> opcode, ram into thread
        l_opcode = OPCODE_MTSPR_FROM_GPR0_TO_SPRD;
        FAPI_TRY(ram_opcode(l_opcode, true));

        //6.get VSR dw1 value from SCR0
        FAPI_TRY(GET_EC_PC_SCR0(iv_target, o_buffer[1]));
    }

#endif
    else
    {
        FAPI_ASSERT(false,
                    fapi2::P10_RAM_INVALID_REG_TYPE_ACCESS_ERR()
                    .set_REGTYPE(i_type),
                    "Type of reg (0x%.8X) is not supported", i_type);
    }

    if(array_op)
    {
        // Looping through the opcode structures
        for(size_t i = 0; i < MAX_OPCODES; i++)
        {
            if(opcodes[i].prePutScom)
            {
                PREP_EC_PC_SCR0(iv_target);
                FAPI_TRY(PUT_EC_PC_SCR0(iv_target, *opcodes[i].prePutScom));
            }

            if(opcodes[i].opCode)
            {
                FAPI_TRY(ram_opcode(opcodes[i].opCode, true));
            }
            else
            {
                // Will break after encountering first empty opcode,
                // as the array will be empty after this
                break;
            }

            if(opcodes[i].postGetScom)
            {
                fapi2::buffer<uint64_t> scomData;
                FAPI_TRY(GET_EC_PC_SCR0(iv_target, scomData));
                *opcodes[i].postGetScom = scomData;
            }
        }
    }

    //restore GPR0 if necessary
    if(iv_write_gpr0)
    {
        PREP_EC_PC_SCR0(iv_target);
        FAPI_TRY(PUT_EC_PC_SCR0(iv_target, l_backup_gpr0));
        l_opcode = OPCODE_MFSPR_FROM_SPRD_TO_GPR0;
        FAPI_TRY(ram_opcode(l_opcode, true));
    }

    // ram_cleanup
    if(!i_allow_mult)
    {
        FAPI_TRY(ram_cleanup());
    }

fapi_try_exit:
    // Error happened and it's not ram error, call ram_cleanup to restore the backup registers
    // If it is ram error, do not call ram_cleanup, so that no new ramming will be executed
    // Do not use "FAPI_TRY" to avoid endless loop
    fapi2::ReturnCode first_err = fapi2::current_err;

    if((fapi2::current_err != fapi2::FAPI2_RC_SUCCESS) && !iv_ram_err && iv_ram_setup)
    {
        ram_cleanup();
    }

    FAPI_DBG("Exiting get register");
    return first_err;
}

// See doxygen comments in header file
fapi2::ReturnCode RamCore::put_reg(const Enum_RegType i_type,
                                   const uint32_t i_reg_num,
                                   const fapi2::buffer<uint64_t>* i_buffer,
                                   const bool i_allow_mult)
{
    FAPI_DBG("Start put register");
    uint32_t l_opcode = 0;
    bool l_write_gpr0 = false;
    fapi2::buffer<uint64_t> l_backup_lr   = 0;
    fapi2::buffer<uint64_t> l_backup_gpr0 = 0;
    fapi2::buffer<uint64_t> l_backup_gpr1 = 0;
    fapi2::buffer<uint64_t> l_backup_fpr0 = 0;
    fapi2::buffer<uint64_t> l_backup_vr0_dw0 = 0;
    fapi2::buffer<uint64_t> l_backup_vr0_dw1 = 0;
    static const size_t MAX_OPCODES = 10;
    putreg_op_t opcodes[MAX_OPCODES] = {};
    bool array_op = false;

    // ram_setup
    if(!i_allow_mult)
    {
        FAPI_TRY(ram_setup());
    }

    FAPI_ASSERT(iv_ram_setup,
                fapi2::P10_RAM_NOT_SETUP_ERR()
                .set_CORE_TARGET(iv_target),
                "Attempting to cleanup ram without setup before");

    //backup GPR0 if it is written
    if(iv_write_gpr0)
    {
        l_opcode = OPCODE_MTSPR_FROM_GPR0_TO_SPRD;
        FAPI_TRY(ram_opcode(l_opcode, true));
        FAPI_TRY(GET_EC_PC_SCR0(iv_target, l_backup_gpr0));
    }

#ifndef __PPE__

    //backup GPR1 if it is written
    if(iv_write_gpr1 && ((i_type == REG_VSR) || ((i_type == REG_SPR) && (i_reg_num == RAM_REG_VSCR))))
    {
        l_opcode = OPCODE_MTSPR_FROM_GPR1_TO_SPRD;
        FAPI_TRY(ram_opcode(l_opcode, true));
        FAPI_TRY(GET_EC_PC_SCR0(iv_target, l_backup_gpr1));
    }

#endif

    // put register value
    if(i_type == REG_GPR)
    {
        //1.put GPR value into SCR0
        //2.create mfsprd<i_reg_num> opcode, ram into thread
        opcodes[0] = {const_cast<fapi2::buffer<uint64_t>* >(&i_buffer[0]), OPCODE_MFSPR_FROM_SPRD_TO_GPR0 + (i_reg_num << 21), NULL};

        if(i_reg_num == 0)
        {
            iv_write_gpr0 = true;
            l_write_gpr0  = true;
        }

        if(i_reg_num == 1)
        {
            iv_write_gpr1 = true;
        }

        array_op = true;
    }
    else if(i_type == REG_SPR)
    {
        if(i_reg_num == RAM_REG_NIA)
        {
            //1.backup LR
            opcodes[0] = {NULL, OPCODE_MFSPR_FROM_LR_TO_GPR0, NULL};
            opcodes[1] = {NULL, OPCODE_MTSPR_FROM_GPR0_TO_SPRD, &l_backup_lr};
            //2.put NIA value into LR
            opcodes[2] = {const_cast<fapi2::buffer<uint64_t>* >(&i_buffer[0]), OPCODE_MFSPR_FROM_SPRD_TO_GPR0, NULL};
            opcodes[3] = {NULL, OPCODE_MTSPR_FROM_GPR0_TO_LR, NULL};
            //3.ram MTNIA_LR opcode
            opcodes[4] = {NULL, (OPCODE_MTNIA_LR | OPCODE_SPECIAL_PPC), NULL};
            //4.restore LR
            opcodes[5] = {&l_backup_lr, OPCODE_MFSPR_FROM_SPRD_TO_GPR0, NULL};
            opcodes[6] = {NULL, OPCODE_MTSPR_FROM_GPR0_TO_LR, NULL};
        }
        else if(i_reg_num == RAM_REG_MSR)
        {
            //1.put SPR value into SCR0
            //2.create mfsprd<gpr0> opcode, ram into thread
            opcodes[0] = {const_cast<fapi2::buffer<uint64_t>* >(&i_buffer[0]), OPCODE_MFSPR_FROM_SPRD_TO_GPR0, NULL};
            //3.create mtmsr opcode, ram into thread
            opcodes[1] = {NULL, (OPCODE_MTMSR_L0 | OPCODE_SPECIAL_PPC), NULL};
        }
        else if(i_reg_num == RAM_REG_MSR_L1)
        {
            //1.put SPR value into SCR0
            //2.create mfsprd<gpr0> opcode, ram into thread
            opcodes[0] = {const_cast<fapi2::buffer<uint64_t>* >(&i_buffer[0]), OPCODE_MFSPR_FROM_SPRD_TO_GPR0, NULL};
            //3.create mtmsr opcode, ram into thread
            opcodes[1] = {NULL, OPCODE_MTMSR_L1, NULL};
        }
        else if(i_reg_num == RAM_REG_MSRD)
        {
            //1.put SPR value into SCR0
            //2.create mfsprd<gpr0> opcode, ram into thread
            opcodes[0] = {const_cast<fapi2::buffer<uint64_t>* >(&i_buffer[0]), OPCODE_MFSPR_FROM_SPRD_TO_GPR0, NULL};
            //3.create mtmsrd opcode, ram into thread
            opcodes[1] = {NULL, (OPCODE_MTMSRD_L0 | OPCODE_SPECIAL_PPC), NULL};
        }
        else if(i_reg_num == RAM_REG_MSRD_L1)
        {
            //1.put SPR value into SCR0
            //2.create mfsprd<gpr0> opcode, ram into thread
            opcodes[0] = {const_cast<fapi2::buffer<uint64_t>* >(&i_buffer[0]), OPCODE_MFSPR_FROM_SPRD_TO_GPR0, NULL};
            //3.create mtmsrd opcode, ram into thread
            opcodes[1] = {NULL, OPCODE_MTMSRD_L1, NULL};
        }
        else if(i_reg_num == RAM_REG_CR)
        {
            //1.put SPR value into SCR0
            //2.create mfsprd<gpr0> opcode, ram into thread
            opcodes[0] = {const_cast<fapi2::buffer<uint64_t>* >(&i_buffer[0]), OPCODE_MFSPR_FROM_SPRD_TO_GPR0, NULL};
            //3.create mtcrf opcode, ram into thread
            opcodes[1] = {NULL, OPCODE_MTOCRF_FROM_GPR0 | ((0x00080000 >> 0) & 0x000FF000), NULL};
            opcodes[2] = {NULL, OPCODE_MTOCRF_FROM_GPR0 | ((0x00080000 >> 1) & 0x000FF000), NULL};
            opcodes[3] = {NULL, OPCODE_MTOCRF_FROM_GPR0 | ((0x00080000 >> 2) & 0x000FF000), NULL};
            opcodes[4] = {NULL, OPCODE_MTOCRF_FROM_GPR0 | ((0x00080000 >> 3) & 0x000FF000), NULL};
            opcodes[5] = {NULL, OPCODE_MTOCRF_FROM_GPR0 | ((0x00080000 >> 4) & 0x000FF000), NULL};
            opcodes[6] = {NULL, OPCODE_MTOCRF_FROM_GPR0 | ((0x00080000 >> 5) & 0x000FF000), NULL};
            opcodes[7] = {NULL, OPCODE_MTOCRF_FROM_GPR0 | ((0x00080000 >> 6) & 0x000FF000), NULL};
            opcodes[8] = {NULL, OPCODE_MTOCRF_FROM_GPR0 | ((0x00080000 >> 7) & 0x000FF000), NULL};
        }
        else if(i_reg_num == RAM_REG_FPSCR)
        {
            //1.backup FPR0
            opcodes[0] = {NULL, OPCODE_MFFPRD_FROM_FPR0_TO_GPR0, NULL};
            opcodes[1] = {NULL, OPCODE_MTSPR_FROM_GPR0_TO_SPRD, &l_backup_fpr0};
            //2.put SPR value into FPR0
            opcodes[2] = {const_cast<fapi2::buffer<uint64_t>* >(&i_buffer[0]), OPCODE_MFSPR_FROM_SPRD_TO_GPR0, NULL};
            opcodes[3] = {NULL, OPCODE_MTFPRD_FROM_GPR0_TO_FPR0, NULL};
            //3.create mtfsf opcode, ram into thread
            opcodes[4] = {NULL, OPCODE_MTFSF_FROM_FPR0, NULL};
            //4.restore FPR0
            opcodes[5] = {&l_backup_fpr0, OPCODE_MFSPR_FROM_SPRD_TO_GPR0, NULL};
            opcodes[6] = {NULL, OPCODE_MTFPRD_FROM_GPR0_TO_FPR0, NULL};
        }
        else if(i_reg_num == RAM_REG_VSCR)
        {
            //1.backup VR0
            opcodes[0] = {NULL, OPCODE_MFVSRD_FROM_VSR32_TO_GPR0, NULL};
            opcodes[1] = {NULL, OPCODE_MTSPR_FROM_GPR0_TO_SPRD, &l_backup_vr0_dw0};
            opcodes[2] = {NULL, OPCODE_MFVSRLD_FROM_VSR32_TO_GPR0, NULL};
            opcodes[3] = {NULL, OPCODE_MTSPR_FROM_GPR0_TO_SPRD, &l_backup_vr0_dw1};
            //2.put SPR value into VR0
            opcodes[4] = {const_cast<fapi2::buffer<uint64_t>* >(&i_buffer[0]), OPCODE_MFSPR_FROM_SPRD_TO_GPR0, NULL};
            opcodes[5] = {NULL, OPCODE_MTVSRDD_FROM_GPR1_0_TO_VSR32, NULL};
            //3.create mtvscr opcode, ram into thread
            opcodes[6] = {NULL, OPCODE_MTVSCR_FROM_VR0, NULL};
            opcodes[7] = {&l_backup_vr0_dw1, OPCODE_MFSPR_FROM_SPRD_TO_GPR0, NULL};
            opcodes[8] = {&l_backup_vr0_dw0, OPCODE_MFSPR_FROM_SPRD_TO_GPR0 + (1 << 21), NULL};
            opcodes[9] = {NULL, OPCODE_MTVSRDD_FROM_GPR1_0_TO_VSR32, NULL};
        }
        else
        {
            //1.put SPR value into SCR0
            //2.create mfsprd<gpr0> opcode, ram into thread
            opcodes[0] = {const_cast<fapi2::buffer<uint64_t>* >(&i_buffer[0]), OPCODE_MFSPR_FROM_SPRD_TO_GPR0, NULL};

            //3.create mtspr<i_reg_num, gpr0> opcode, ram into thread
            opcodes[1] = {NULL, OPCODE_MTSPR_FROM_GPR0_TO_SPR0 | ENCODE_SPR_NUM_OPCODE(i_reg_num), NULL};
        }

        array_op = true;
    }
    else if(i_type == REG_FPR)
    {
        //1.put FPR value into SCR0
        //2.create mfsprd<gpr0> opcode, ram into thread
        opcodes[0] = {const_cast<fapi2::buffer<uint64_t>* >(&i_buffer[0]), OPCODE_MFSPR_FROM_SPRD_TO_GPR0, NULL};
        //3.create mtfprd<i_reg_num, gpr0>#TX=0 opcode, ram into thread
        opcodes[1] = {NULL, OPCODE_MTFPRD_FROM_GPR0_TO_FPR0 + (i_reg_num << 21), &l_backup_vr0_dw0};

        array_op = true;
    }

#ifndef __PPE__
    else if(i_type == REG_VSR)
    {
        //1.put VSR dw1 value into SCR0
        PREP_EC_PC_SCR0(iv_target);
        FAPI_TRY(PUT_EC_PC_SCR0(iv_target, i_buffer[1]));

        //2.create mfsprd<gpr0> opcode, ram into thread
        l_opcode = OPCODE_MFSPR_FROM_SPRD_TO_GPR0;
        FAPI_TRY(ram_opcode(l_opcode, true));

        //3.put VSR dw0 value into SCR0
        PREP_EC_PC_SCR0(iv_target);
        FAPI_TRY(PUT_EC_PC_SCR0(iv_target, i_buffer[0]));

        //4.create mfsprd<gpr1> opcode, ram into thread
        l_opcode = OPCODE_MFSPR_FROM_SPRD_TO_GPR0;
        l_opcode += (1 << 21);
        FAPI_TRY(ram_opcode(l_opcode, true));

        //5.create mtvsrdd<i_reg_num, gpr0, gpr1> opcode, ram into thread
        if(i_reg_num < 32)
        {
            l_opcode = OPCODE_MTVSRDD_FROM_GPR1_0_TO_VSR0;
            l_opcode += (i_reg_num << 21);
        }
        else
        {
            l_opcode = OPCODE_MTVSRDD_FROM_GPR1_0_TO_VSR32;
            l_opcode += ((i_reg_num - 32) << 21);
        }

        FAPI_TRY(ram_opcode(l_opcode, true));
    }

#endif
    else
    {
        FAPI_ASSERT(false,
                    fapi2::P10_RAM_INVALID_REG_TYPE_ACCESS_ERR()
                    .set_REGTYPE(i_type),
                    "Type of reg (0x%.8X) is not supported", i_type);
    }

    if(array_op)
    {
        // Looping through the opcode structures
        for(size_t i = 0; i < MAX_OPCODES; i++)
        {
            if(opcodes[i].prePutScom)
            {
                PREP_EC_PC_SCR0(iv_target);
                FAPI_TRY(PUT_EC_PC_SCR0(iv_target, *opcodes[i].prePutScom));
            }

            if(opcodes[i].opCode)
            {
                FAPI_TRY(ram_opcode(opcodes[i].opCode, true));
            }
            else
            {
                // Will break after encountering first empty opcode,
                // as the array will be empty after this
                break;
            }

            if(opcodes[i].postGetScom)
            {
                fapi2::buffer<uint64_t> scomData;
                FAPI_TRY(GET_EC_PC_SCR0(iv_target, scomData));
                *opcodes[i].postGetScom = scomData;
            }
        }
    }


    //restore GPR0 if necessary
    if(iv_write_gpr0 && !l_write_gpr0)
    {
        PREP_EC_PC_SCR0(iv_target);
        FAPI_TRY(PUT_EC_PC_SCR0(iv_target, l_backup_gpr0));
        l_opcode = OPCODE_MFSPR_FROM_SPRD_TO_GPR0;
        FAPI_TRY(ram_opcode(l_opcode, true));
    }

#ifndef __PPE__

    //restore GPR1 if necessary
    if(iv_write_gpr1 && ((i_type == REG_VSR) || ((i_type == REG_SPR) && (i_reg_num == RAM_REG_VSCR))))
    {
        PREP_EC_PC_SCR0(iv_target);
        FAPI_TRY(PUT_EC_PC_SCR0(iv_target, l_backup_gpr1));
        l_opcode = OPCODE_MFSPR_FROM_SPRD_TO_GPR1;
        FAPI_TRY(ram_opcode(l_opcode, true));
    }

#endif

    // ram_cleanup
    if(!i_allow_mult)
    {
        FAPI_TRY(ram_cleanup());
    }

fapi_try_exit:
    // Error happened and it's not ram error, call ram_cleanup to restore the backup registers
    // If it is ram error, do not call ram_cleanup, so that no new ramming will be executed
    // Do not use "FAPI_TRY" to avoid endless loop
    fapi2::ReturnCode first_err = fapi2::current_err;

    if((fapi2::current_err != fapi2::FAPI2_RC_SUCCESS) && !iv_ram_err && iv_ram_setup)
    {
        ram_cleanup();
    }

    FAPI_DBG("Exiting put register");
    return first_err;
}

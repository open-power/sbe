/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/core/p10_sbe_core_spr_setup.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2019,2021                        */
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
///----------------------------------------------------------------------------
///
/// @file p10_sbe_core_spr_setup.H
///
/// @brief Initializes core SPRs prior to thread execution start
///
/// *HWP HW Maintainer: Douglas Holtsinger <Douglas.Holtsinger@ibm.com>
/// *HWP FW Maintainer: Raja Das <rajadas2@in.ibm.com>
/// *HWP Consumed by  : SBE
///----------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include <p10_sbe_core_spr_setup.H>
#include <p10_scom_c.H>
#include <p10_scom_eq.H>
#include <p10_ram_core.H>

// RAM constants
const uint8_t THREAD_0 = 0;
const uint8_t THREAD_1 = 1;
const uint8_t THREAD_2 = 2;
const uint8_t THREAD_3 = 3;
const uint32_t MSR_SPR_NUMBER   = 2001;
const uint32_t PSSCR_SPR_NUMBER = 855;
const uint64_t HOSTBOOT_PSSCR_VALUE = 0x00000000003F00FFULL;

// MSR Secure / S bit
//    0 -> The thread is not in Secure state. It may not access Secure memory. The thread is not in ultravisor state.
//    1 -> The thread is in Secure state. If MSRHV=1 and MSRPR=0, the thread is in ultravisor state; otherwise the
//    value does not affect privilege.
const uint8_t MSR_S_BITPOS = 41;

//------------------------------------------------------------------------------
// Function definitions
//------------------------------------------------------------------------------

///
/// @brief Return set of core targets which need SPR programming
///        prior to instruction start.  In fused core mode, both the
///        master core and its partner fused core will be returned; in normal
///        core mode, solely the master core target will be returned.
///
/// @param[in]  i_target          Reference to processor chip target
/// @param[in]  i_fused_core_mode Boolean indicator of fused core configuration
///                               status (true=fused, false=normal)
/// @param[out] o_core_targets    Set of master core targets to process
/// @param[out] o_master_core_num Master core number
/// @return fapi::ReturnCode. FAPI2_RC_SUCCESS if success, else error code.
///
fapi2::ReturnCode p10_sbe_core_spr_setup_select_cores(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
    bool i_fused_core_mode,
    std::vector<fapi2::Target<fapi2::TARGET_TYPE_CORE>>& o_core_targets,
    fapi2::ATTR_MASTER_CORE_Type& o_master_core_num)
{
    FAPI_DBG("Entering...");

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_MASTER_CORE, i_target, o_master_core_num),
             "Error from FAPI_ATTR_GET (ATTR_MASTER_CORE)");

    for (auto l_core_target : i_target.getChildren<fapi2::TARGET_TYPE_CORE>())
    {
        fapi2::ATTR_CHIP_UNIT_POS_Type l_core_num;
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_core_target, l_core_num),
                 "Error from FAPI_ATTR_GET (ATTR_CHIP_UNIT_POS)");

        if (l_core_num == o_master_core_num)
        {
            o_core_targets.push_back(l_core_target);
            break;
        }
    }

    FAPI_ASSERT(o_core_targets.size() == 1,
                fapi2::P10_SBE_CORE_SPR_SETUP_MASTER_CORE_NOT_FOUND()
                .set_CHIP_TARGET(i_target)
                .set_MASTER_CORE_NUM(o_master_core_num)
                .set_FUSED_MODE(i_fused_core_mode),
                "Error finding the master core target!");

    if (i_fused_core_mode)
    {
        fapi2::ATTR_MASTER_CORE_Type l_partner_core_num =
            (o_master_core_num % 2) ? (o_master_core_num - 1) : (o_master_core_num + 1);

        for (auto l_core_target : i_target.getChildren<fapi2::TARGET_TYPE_CORE>())
        {
            fapi2::ATTR_CHIP_UNIT_POS_Type l_core_num;
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_core_target, l_core_num),
                     "Error from FAPI_ATTR_GET (ATTR_CHIP_UNIT_POS)");

            if (l_core_num == l_partner_core_num)
            {
                o_core_targets.push_back(l_core_target);
                break;
            }
        }

        FAPI_ASSERT(o_core_targets.size() == 2,
                    fapi2::P10_SBE_CORE_SPR_SETUP_MASTER_FUSED_CORE_PARTNER_NOT_FOUND()
                    .set_CHIP_TARGET(i_target)
                    .set_MASTER_CORE_NUM(o_master_core_num)
                    .set_FUSED_MODE(i_fused_core_mode),
                    "Error finding the master fused core partner target!");
    }

    for (const auto& l_core_target : o_core_targets)
    {
        fapi2::ATTR_ECO_MODE_Type l_eco_mode;
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_ECO_MODE, l_core_target, l_eco_mode));

        FAPI_ASSERT(l_eco_mode == fapi2::ENUM_ATTR_ECO_MODE_DISABLED,
                    fapi2::P10_SBE_CORE_SPR_SETUP_MASTER_CORE_ECO_MODE()
                    .set_CHIP_TARGET(i_target)
                    .set_FUSED_MODE(i_fused_core_mode)
                    .set_ECO_MODE_TARGET(l_core_target),
                    "Master core target not expected to be in ECO mode!");
    }

fapi_try_exit:
    FAPI_DBG("Exiting...");
    return fapi2::current_err;

}

///
/// @brief Program SPRs on set of master core targets.
///
/// @param[in]  i_core_targets    -- Set of core targets to initialize.
/// @param[in]  i_master_core_num -- Master core number
///
/// @return fapi::ReturnCode. FAPI2_RC_SUCCESS if success, else error code.
///
fapi2::ReturnCode p10_sbe_core_spr_setup_program_sprs(
    const std::vector<fapi2::Target<fapi2::TARGET_TYPE_CORE>>& i_core_targets,
    const fapi2::ATTR_MASTER_CORE_Type& i_master_core_num)
{
    using namespace scomt::c;
    using namespace scomt::eq;

    // RTC 245822
    // remove this to use the auto-generated value once it bit shows up in the headers.
    const uint32_t QME_QMCR_STOP_SHIFTREG_OVERRIDE_EN = 29;

    uint64_t l_master_hrmor = 0;
    uint64_t l_bootloader_offset = 0;
    const fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYSTEM;
    fapi2::buffer<uint64_t> l_scsr_or = 0;

    FAPI_DBG("Entering...");

    l_scsr_or.setBit<QME_SCSR_ASSERT_PM_EXIT>();

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_SBE_BOOTLOADER_OFFSET,
                           FAPI_SYSTEM,
                           l_bootloader_offset),
             "Error from FAPI_ATTR_GET (ATTR_SBE_BOOTLOADER_OFFSET)");

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_SBE_MASTER_HRMOR_ADDRESS,
                           FAPI_SYSTEM,
                           l_master_hrmor),
             "Error from FAPI_ATTR_GET (ATTR_SBE_MASTER_HRMOR_ADDRESS)");

    for (auto l_core_target : i_core_targets)
    {
        auto l_eq = l_core_target.getParent<fapi2::TARGET_TYPE_EQ>();

        RamCore l_ram_t1(l_core_target, THREAD_1);
        RamCore l_ram_t2(l_core_target, THREAD_2);
        RamCore l_ram_t3(l_core_target, THREAD_3);

        fapi2::buffer<uint64_t> l_data = l_master_hrmor;
        fapi2::buffer<uint64_t> l_msr = 0;
        fapi2::buffer<uint64_t> l_qmcr = 0;

        uint8_t l_core_unit_pos = 0;

        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS,
                               l_core_target,
                               l_core_unit_pos),
                 "Error from FAPI_ATTR_GET (ATTR_CHIP_UNIT_POS)!");

        if (l_core_unit_pos != i_master_core_num)
        {
            l_data -= l_bootloader_offset;
        }

        // assert PM_EXIT via SCSR WOR address
        FAPI_TRY(fapi2::putScom(l_core_target, QME_SCSR_SCOM2, l_scsr_or),
                 "Error from putScom (QME_SCSR_SCOM2)");

        // get MSR to determine if need to set URMOR
        FAPI_TRY(l_ram_t2.get_reg(REG_SPR, MSR_SPR_NUMBER, &l_msr),
                 "Error ramming MSR (T2)!");

        // Enable SCOM access to the RMOR registers
        l_qmcr.flush<0>().setBit<QME_QMCR_STOP_SHIFTREG_OVERRIDE_EN>();
        FAPI_TRY(fapi2::putScom(l_eq, QME_QMCR_SCOM2, l_qmcr),
                 "Error during putscom of QME_QMCR_SCOM2 for shiftable regs access");

        // set URMOR to the HRMOR value if SMF is enabled
        if(l_msr.getBit <MSR_S_BITPOS>()) // SMF is on
        {
            PREP_QME_RMOR(l_core_target);
            FAPI_TRY(PUT_QME_RMOR(l_core_target,
                                  l_data.setBit<QME_RMOR_URMOR_SELECT>()),
                     "Error during PUT_QME_RMOR for URMOR");
            FAPI_DBG("Wrote URMOR with 0x%016lX", (uint64_t)l_data );
        }

        // set HRMOR -- core level SPR
        // Must not set bit 12 in HRMOR.  Only applies to URMOR.
        PREP_QME_RMOR(l_core_target);
        l_data.clearBit<12>();
        FAPI_TRY(PUT_QME_RMOR(l_core_target,
                              l_data.clearBit<QME_RMOR_URMOR_SELECT>()),
                 "Error during PUT_QME_RMOR for HRMOR");
        FAPI_DBG("Wrote HRMOR with 0x%016lX", (uint64_t)l_data);

        // set PSSCR via thread specific instances
        l_data = HOSTBOOT_PSSCR_VALUE;
        FAPI_DBG("PSSCR with 0x%16llX", (uint64_t)l_data);
        FAPI_TRY(l_ram_t1.put_reg(REG_SPR, PSSCR_SPR_NUMBER, &l_data),
                 "Error ramming PSSCR (T1)!");
        FAPI_TRY(l_ram_t2.put_reg(REG_SPR, PSSCR_SPR_NUMBER, &l_data),
                 "Error ramming PSSCR (T2)!");
        FAPI_TRY(l_ram_t3.put_reg(REG_SPR, PSSCR_SPR_NUMBER, &l_data),
                 "Error ramming PSSCR (T3)!");

    }

fapi_try_exit:
    FAPI_DBG("Exiting...");
    return fapi2::current_err;
}


// HWP entry point, description in header
fapi2::ReturnCode p10_sbe_core_spr_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
{
    FAPI_DBG("Entering ...");

    fapi2::ATTR_PROC_SBE_MASTER_CHIP_Type l_is_master_sbe;
    fapi2::ATTR_FUSED_CORE_MODE_Type l_fused_core_mode;
    std::vector<fapi2::Target<fapi2::TARGET_TYPE_CORE>> l_core_targets;
    fapi2::ATTR_MASTER_CORE_Type l_master_core_num;

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PROC_SBE_MASTER_CHIP, i_target, l_is_master_sbe),
             "Error from FAPI_ATTR_GET (ATTR_PROC_SBE_MASTER_CHIP)");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_FUSED_CORE_MODE, fapi2::Target<fapi2::TARGET_TYPE_SYSTEM>(), l_fused_core_mode),
             "Error from FAPI_ATTR_GET (ATTR_FUSED_CORE_MODE)");

    FAPI_ASSERT(l_is_master_sbe,
                fapi2::P10_SBE_CORE_SPR_SETUP_NOT_MASTER_CHIP()
                .set_CHIP_TARGET(i_target),
                "HWP execution is supported only on SBE master chip!");

    FAPI_TRY(p10_sbe_core_spr_setup_select_cores(i_target,
             l_fused_core_mode,
             l_core_targets,
             l_master_core_num),
             "Error from p10_sbe_core_spr_setup_select_cores");

    FAPI_TRY(p10_sbe_core_spr_setup_program_sprs(l_core_targets, l_master_core_num),
             "Error from p10_sbe_core_spr_setup_program_sprs");

fapi_try_exit:
    FAPI_DBG("Exiting ...");
    return fapi2::current_err;
}

/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/core/p10_sbe_core_spr_setup.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2019                             */
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
/// *HWP HW Maintainer: Nicholas Landi <nlandi@ibm.com>
/// *HWP FW Maintainer: Raja Das <rajadas2@in.ibm.com>
/// *HWP Consumed by  : SBE
///----------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include <p10_sbe_core_spr_setup.H>
#include <p10_scom_c.H>

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
/// @return fapi::ReturnCode. FAPI2_RC_SUCCESS if success, else error code.
///
fapi2::ReturnCode p10_sbe_core_spr_setup_select_cores(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
    bool i_fused_core_mode,
    std::vector<fapi2::Target<fapi2::TARGET_TYPE_CORE>>& o_core_targets)
{
    FAPI_DBG("Entering...");

    fapi2::ATTR_MASTER_CORE_Type l_master_core_num;
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_MASTER_CORE, i_target, l_master_core_num),
             "Error from FAPI_ATTR_GET (ATTR_MASTER_CORE)");

    for (auto l_core_target : i_target.getChildren<fapi2::TARGET_TYPE_CORE>())
    {
        fapi2::ATTR_CHIP_UNIT_POS_Type l_core_num;
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_core_target, l_core_num),
                 "Error from FAPI_ATTR_GET (ATTR_CHIP_UNIT_POS)");

        if (l_core_num == l_master_core_num)
        {
            o_core_targets.push_back(l_core_target);
            break;
        }
    }

    FAPI_ASSERT(o_core_targets.size() == 1,
                fapi2::P10_SBE_CORE_SPR_SETUP_MASTER_CORE_NOT_FOUND()
                .set_CHIP_TARGET(i_target)
                .set_MASTER_CORE_NUM(l_master_core_num)
                .set_FUSED_MODE(i_fused_core_mode),
                "Error finding the master core target!");

    if (i_fused_core_mode)
    {
        fapi2::ATTR_MASTER_CORE_Type l_partner_core_num =
            (l_master_core_num % 2) ? (l_master_core_num - 1) : (l_master_core_num + 1);

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
                    .set_MASTER_CORE_NUM(l_master_core_num)
                    .set_FUSED_MODE(i_fused_core_mode),
                    "Error finding the master fused core partner target!");
    }

fapi_try_exit:
    FAPI_DBG("Exiting...");
    return fapi2::current_err;

}

///
/// @brief Program SPRs on set of master core targets
///
/// @param[in]  i_targets  Set of core targets to initialize
///
/// @return fapi::ReturnCode. FAPI2_RC_SUCCESS if success, else error code.
///
fapi2::ReturnCode p10_sbe_core_spr_setup_program_sprs(
    const std::vector<fapi2::Target<fapi2::TARGET_TYPE_CORE>>& i_core_targets)
{
    using namespace scomt::c;

    FAPI_DBG("Entering...");

    // assert PM_EXIT via SCSR WOR address
    fapi2::buffer<uint64_t> l_scsr_or = 0;
    l_scsr_or.setBit<QME_SCSR_ASSERT_PM_EXIT>();

    for (auto l_core_target : i_core_targets)
    {
        FAPI_TRY(fapi2::putScom(l_core_target, QME_SCSR_SCOM2, l_scsr_or),
                 "Error from putScom (QME_SCSR_SCOM2)");
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

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PROC_SBE_MASTER_CHIP, i_target, l_is_master_sbe),
             "Error from FAPI_ATTR_GET (ATTR_RPROC_SBE_MASTER_CHIP)");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_FUSED_CORE_MODE, fapi2::Target<fapi2::TARGET_TYPE_SYSTEM>(), l_fused_core_mode),
             "Error from FAPI_ATTR_GET (ATTR_FUSED_CORE_MODE)");

    FAPI_ASSERT(l_is_master_sbe,
                fapi2::P10_SBE_CORE_SPR_SETUP_NOT_MASTER_CHIP()
                .set_CHIP_TARGET(i_target),
                "HWP execution is supported only on SBE master chip!");

    FAPI_TRY(p10_sbe_core_spr_setup_select_cores(i_target,
             l_fused_core_mode,
             l_core_targets),
             "Error from p10_sbe_core_spr_setup_select_cores");

    FAPI_TRY(p10_sbe_core_spr_setup_program_sprs(l_core_targets),
             "Error from p10_sbe_core_spr_setup_program_sprs");

fapi_try_exit:
    FAPI_DBG("Exiting ...");
    return fapi2::current_err;
}

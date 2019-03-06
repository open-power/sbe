/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_sbe_select_ex.C $ */
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
///
/// @file p10_sbe_select_ex.C
/// @brief Select the Hostboot core(s) from the available cores on the chip
///
/// *HWP HWP Owner: Greg Still <stillgs@us.ibm.com>
/// *HWP FW Owner: Prem Jha <premjha1@in.ibm.com>
/// *HWP Team: PM
/// *HWP Consumed by: SBE
///
/// High-level procedure flow:
/// @verbatim
///
///   Prerequisite:  istep 2 will setup the all MC groups with ALL good
///                  elements represented.
///
/// Two modes are supported:
/// - SINGLE: select 1 core based on type (normal or fused) and then 8MB of
///   backing cache (2 normal or fused)
/// - ALL: select all good cores
///
/// In both modes, the OCC registers are written with the valid configuration.
/// Additionally, default PFET controller delays are written into all good
/// cores so that future power-on operations will succeed.
///
/// Parameter indicates single core or all (controlled by Cronus/SBE)
///
/// first_core = true
/// second_core = false
/// first_backing = false
/// second_backing = false
/// Loop over functional cores
///     if (single)
///         if (first_core)
///             set bit c in core_configure buffer
///             set MASTER_CORE attribute
///             first_core = false
///             if (fused)
///                 second_core = true
///             else
///                 first_backing = true
///             next
///
///         if (second_core)
///             check that second one is paired
///             set bit c in core_configure buffer
///             second_core = false
///             first_backing = true;
///             next
///
///         if (first_backing)
///             set bit c in core_configure buffer
///             set BACKING_CACHE[0] attribute
///             first_backing = false;
///             second_backing = true;
///             next
///
///         if (second_backing)
///             check that second one is paired
///             set bit c in core_configure buffer
///             set BACKING_CACHE[1] attribute
///             second_backing = false;
///             next
///      else
///          set bit c in core_configure buffer
///
/// Write resultant scoreboard Core mask into OCC complex
///   - This is the "master record " of the enabled cores/quad in the system
///   - This is only for during the IPL (will be updated later in step 15)
/// Set default PFET controller delay values into Core
/// @endverbatim

// -----------------------------------------------------------------------------
//  Includes
// -----------------------------------------------------------------------------
#include "p10_sbe_select_ex.H"
#if 0
    #include "p10_common_poweronoff.H"
    #include <multicast_group_defs.H>
#endif
// -----------------------------------------------------------------------------
//  Definitions
// -----------------------------------------------------------------------------

const static uint32_t NUM_OF_QUADS = 8;

// -----------------------------------------------------------------------------
//  Function prototypes
// -----------------------------------------------------------------------------

fapi2::ReturnCode select_ex_pfet_delay(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target);

// -----------------------------------------------------------------------------
//  Function definitions
// -----------------------------------------------------------------------------

// See .H for documentation
fapi2::ReturnCode p10_sbe_select_ex(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
    const selectex::MODE i_mode)
{
#if 0
    FAPI_IMP("> p10_sbe_select_ex");

    fapi2::buffer<uint32_t> l_core_config = 0;
    fapi2::buffer<uint64_t> l_data64;

    bool b_skip_hb_checks = false;
    bool b_single = true;
    bool b_fused = false;
    bool b_first_core = true;
    bool b_second_core = false;
    bool b_first_backing = false;
    bool b_second_backing = false;

    uint32_t l_master_core_num = 0xFF; // invalid Core number initialized
    uint32_t l_backing_caches[2] = { 255, 255 };

    const fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYSTEM;
    auto l_core_functional_vector = i_target.getChildren<fapi2::TARGET_TYPE_CORE>
                                    (fapi2::TARGET_STATE_FUNCTIONAL);

    // Read the "FORCE_ALL" attribute
    fapi2::ATTR_SYS_FORCE_ALL_CORES_Type l_attr_force_all;
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_SYS_FORCE_ALL_CORES,
                           FAPI_SYSTEM,
                           l_attr_force_all));

    // Set the flow mode and respect the force mode
    if (l_attr_force_all || i_mode == selectex::ALL)
    {
        b_single = false;
        b_skip_hb_checks = true;
        FAPI_DBG("All cores mode");
    }
    else
    {
        if (i_mode == selectex::SINGLE_NONE_OK)
        {
            b_skip_hb_checks = true;
        }

        FAPI_DBG("Single/Fused core mode:  Number of candidate normal cores = %d",
                 l_core_functional_vector.size());

        fapi2::ATTR_FUSED_CORE_MODE_Type l_attr_fused_mode;
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_FUSED_CORE_MODE,
                               FAPI_SYSTEM,
                               l_attr_fused_mode));

        if (l_attr_fused_mode == fapi2::ENUM_ATTR_FUSED_CORE_MODE_CORE_FUSED)
        {
            b_fused = true;
            FAPI_INF("p10_sbe_select_ex: Fused core mode detected");
        }

    }

    // Check that we're not trying to force fused cores on a chip where the
    // force mechanism is disabled via eFuses AND we are not already in
    // fused mode
//    {
//         fapi2::buffer<uint64_t> l_perv_ctrl0;
//         fapi2::buffer<uint64_t> l_device_id_reg;
//
//         FAPI_TRY(fapi2::getScom(i_target, PERV_PERV_CTRL0_SCOM, l_perv_ctrl0));
//         FAPI_TRY(fapi2::getScom(i_target, PERV_DEVICE_ID_REG, l_device_id_reg));
//
//         FAPI_ASSERT(!(l_perv_ctrl0.getBit<p10N2_PERV_PERV_CTRL0_TP_OTP_SCOM_FUSED_CORE_MODE>()
//                       && l_device_id_reg.getBit<p10N2_PERV_DEVICE_ID_REG_HW_MODE_SEL>() &&
//                       !l_device_id_reg.getBit<p10N2_PERV_DEVICE_ID_REG_TP_EX_FUSE_SMT8_CTYPE_EN>()),
//                     fapi2::SBE_SELECT_EX_FORCE_FUSED_CORES_DISABLED(),
//                     "Failed to force fused core mode because external control has been disabled via eFuses");
//     }

    // Loop through the core functional vector on the view that the FAPI
    // platform code is expected to return the vector elements in acsending
    // order.
    //
    // For non-fused mode, the 3 cores in the vector is going to be the
    // hostboot cores.
    //
    // For fused core mode, the first core must be even numbered and then
    // the companion odd numbered one must also be in the vector. This is repeated
    // for the next 2 core so that we have 2 fused cores to have the necessary
    // back cache.  // If the first or third core in the vector is odd or the
    // companion odd core to the first and second even one is not present, raise
    // errors.
    //
    // A buffer track the core configuration as though "ALL" is the mode chosen.
    // This is done to reduce conditional processing within the vector loop to
    // allow for better prefetch utilization.

    for (auto& core : l_core_functional_vector)
    {
        fapi2::ATTR_CHIP_UNIT_POS_Type l_attr_chip_unit_pos = 0;
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS,
                               core,
                               l_attr_chip_unit_pos));

        uint32_t l_core_num  = (uint32_t)l_attr_chip_unit_pos;

        if (b_single)
        {
            if (b_first_core)
            {
                l_core_config.setBit(l_core_num);
                l_master_core_num = l_core_num;
                FAPI_TRY(FAPI_ATTR_SET( fapi2::ATTR_MASTER_CORE,
                                        i_target,
                                        l_master_core_num));
                FAPI_DBG("MASTER core chiplet %d", l_master_core_num);
                b_first_core = false;

                if (b_fused)
                {
                    // Check that the first normal core is even.
                    FAPI_ASSERT(l_core_num % 2 == 0,
                                fapi2::SBE_SELECT_EX_FUSED_NOT_EVEN_ERROR()
                                .set_CHIP(i_target)
                                .set_CORE_NUM(l_core_num),
                                "The first core found in fused mode was not an even core");
                    b_second_core = true;
                }
                else
                {
                    b_first_backing = true;
                }

                continue;  // next core
            }

            if (b_second_core)
            {
                // Check that the second normal core is functional.
                FAPI_DBG("Odd core check: master core == %d, core num = %d",
                         l_master_core_num, l_core_num);
                FAPI_ASSERT(l_core_num == l_master_core_num + 1,
                            fapi2::SBE_SELECT_EX_FUSED_ODD_ERROR()
                            .set_CHIP(i_target)
                            .set_CORE_NUM(l_core_num)
                            .set_MASTER_NUM(l_master_core_num),
                            "Odd core within master fused set is not functional");
                l_core_config.setBit(l_core_num);
                b_second_core = false;
                b_first_backing = true;
                continue;  // next core
            }

            if (b_first_backing)
            {
                l_backing_caches[0] = l_core_num;
                l_core_config.setBit(l_core_num);
                b_first_backing = false;
                b_second_backing = true;
                continue;  // next core
            }

            if (b_second_backing)
            {
                l_backing_caches[1] = l_core_num;
                l_core_config.setBit(l_core_num);
                b_second_backing = false;
                continue;  // next core
            }
        }
        else  // All cores
        {
            l_core_config.setBit(l_core_num);
        }

        FAPI_DBG("Scoreboard values for OCC: Core 0x%08X", l_core_config);

    } // Core loop

    FAPI_ASSERT(b_skip_hb_checks || l_core_config != 0,
                fapi2::SBE_SELECT_EX_NO_CORE_AVAIL_ERROR()
                .set_CHIP(i_target),
                "No good cores found to boot with");

    if (b_single)
    {
        FAPI_TRY(FAPI_ATTR_SET( fapi2::ATTR_BACKING_CACHES,
                                i_target,
                                l_backing_caches));
        FAPI_DBG("Backing caches = %d", l_backing_caches[0], l_backing_caches[1]);
    }

    // Setup the Core PFET delays with multicast
    FAPI_TRY(select_ex_pfet_delay(i_target));

    // Write to the OCC Core Configuration Status Register
    l_data64.flush<0>().insertFromRight<0, 32>(l_core_config);
    FAPI_TRY(fapi2::putScom(i_target, PU_OCB_OCI_CCSR_SCOM, l_data64));

fapi_try_exit:
    FAPI_INF("< p10_sbe_select_ex");
#endif
    return fapi2::current_err;
} // END p10_sbe_select_ex

///-----------------------------------------------------------------------------
/// @brief Update the QMEs PFET delays
///
/// @return  FAPI2_RC_SUCCESS if success, else error code.
fapi2::ReturnCode select_ex_pfet_delay(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
{
#if 0
    FAPI_DBG("> select_ex_pfet_delay...");

    fapi2::buffer<uint64_t> l_data64;

    fapi2::MulticastGroupMapping l_mapping;
    std::vector< fapi2::MulticastGroupMapping > l_mappings;
    fapi2::Target < fapi2::TARGET_TYPE_CORE | fapi2::TARGET_TYPE_MULTICAST > l_core_mc;

    l_mapping.abstractGroup = fapi2::MCGROUP_GOOD;
    l_mapping.hwValue = 0;
    l_mappings.push_back(l_mapping);

    l_mapping.abstractGroup = fapi2::MCGROUP_GOOD_NO_TP;
    l_mapping.hwValue = 1;
    l_mappings.push_back(l_mapping);

    l_mapping.abstractGroup = fapi2::MCGROUP_GOOD_MEMCTL;
    l_mapping.hwValue = 2;
    l_mappings.push_back(l_mapping);

    l_mapping.abstractGroup = fapi2::MCGROUP_GOOD_OBUS;
    l_mapping.hwValue = 3;
    l_mappings.push_back(l_mapping);

    l_mapping.abstractGroup = fapi2::MCGROUP_NEST;
    l_mapping.hwValue = 4;
    l_mappings.push_back(l_mapping);

    l_mapping.abstractGroup = fapi2::MCGROUP_GOOD_PCI;
    l_mapping.hwValue = 5;
    l_mappings.push_back(l_mapping);

    l_mapping.abstractGroup = fapi2::MCGROUP_GOOD_EQ;
    l_mapping.hwValue = 6;
    l_mappings.push_back(l_mapping);

    l_mapping.abstractGroup = fapi2::MCGROUP_ALL;
    l_mapping.hwValue = 7;
    l_mappings.push_back(l_mapping);
    FAPI_TRY(fapi2::setMulticastGroupMap(i_target, l_mappings));

    l_core_mc = i_target.getMulticast(fapi2::MCGROUP_GOOD_EQ, fapi2::MCCORE_ALL);

    // Write the default PFET Controller Delay values to the Cores
    FAPI_DBG("Setting PFET Delays in all cores");

    // *INDENT-OFF*
    l_data64.flush<0>()
            .insertFromRight<0,  4>(p10power::PFET_DELAY_POWERDOWN)
            .insertFromRight<4,  4>(p10power::PFET_DELAY_POWERUP_L3)
            .insertFromRight<8,  4>(p10power::PFET_DELAY_POWERUP_CL2)
            .insertFromRight<12, 4>(p10power::PFET_DELAY_POWERUP_MMA);
    // *INDENT-ON*

    FAPI_TRY(fapi2::putScom(l_core_mc, CPMS0_CPMS_PFETDLY_SCOM, l_data64));

    // Clear QME Scratch 1[Runtime Wakeup Mode](3) to force SMF enabled systems
    // tp start Hostboot in UV mode


    // @todo This bit should come from p10_pm_hcd_flags.h but it is presently
    // not mirrored.
//    l_data64.flush<0>().setBit<31>();
//     FAPI_TRY(fapi2::putScom(i_target_core, C_CPPM_CPMMR_CLEAR, l_data64));
//     FAPI_DBG("Clearing CPPMR[Runtime Wakeup Mode] in core %d", i_core_num);

fapi_try_exit:
    FAPI_DBG("< select_ex_pfet_delay...");
#endif
    return fapi2::current_err;
}

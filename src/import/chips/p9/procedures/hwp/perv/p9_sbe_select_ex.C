/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/perv/p9_sbe_select_ex.C $  */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2019                        */
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
/// @file p9_sbe_select_ex.C
/// @brief Select the Hostboot core from the available cores on the chip
///
// *HWP HWP Owner: Greg Still <stillgs@us.ibm.com>
// *HWP FW Owner: Sangeetha T S <sangeet2@in.ibm.com>
// *HWP Team: PM
// *HWP Level: 2
// *HWP Consumed by: SBE
///
///
///
/// High-level procedure flow:
/// @verbatim
///   Add the master core to Group 0 (all functional chiplets) and Group 1
///     (all functional cores)
///   Add the master cache to Group 0 (all functional chiplets)
///   Setup MC groups needed for istep 4 use:
///     - MC group 3:  Core(s) (eg ECs);  use EC MC group register 3
///     - MC group 4:  EQ(s);             use EQ MC group register 2
///     - MC group 5:  Even EXs;          use EQ MC group register 3
///     - MC group 6:  Odd Exs;           use EQ MC group register 4
///
///   Prerequisite:  istep 2 will setup the above groups with ALL the good
///                  elements represented.
///
///   This procedure will ADD entities to these groups in SINGLE mode;
///   in ALL mode, the groups are not changed.  In either case, the OCC
///   registers are written with the valid configuration.  Additionally,
///   default PFET controller delays are written into all configured
///   EC and EQ chiplets so that istep 4 power-on operations will
///   succeed.
///
///   Parameter indicates single core or all (controlled by Cronus/SBE)
///
///     host_eq_found = false
///     loop over functional cores {
///         if mode == SINGLE {
///             if !host_eq_found {
///                 Record the master core, EX and EQ number
///                 Add to MC Group 3
///                 host_eq_found = true
///             }
///         }
///         Set bits in core and EX scoreboard for later updating the OCC
///         Set default PFET controller delay values into Core
///         if host_eq_found (only set in single mode)
///             break out of core loop
///       }
///
///     host_eq_found = false
///     loop over functional EQs {
///         if mode == SINGLE {
///             if master EQ
///                 Add to MC Groups 4
///                 for the EXs in the EQ {
///                     if master EX
///                         Add to MC Group 5 if Even (EX0)
///                         Add to MC Group 6 if Odd (EX1)
///                 }
///                 host_eq_found = true
///             }
///         }
///         Set bit in EQ scoreboard for later updating the OCC
///         Set default PFET controller delay values into EQ
///         if host_eq_found (only set in single mode)
///             break out of EQ loop
///     }
///
///   Write resultant scoreboard EQ/Core mask into OCC complex
///     - This is the "master record " of the enabled cores/quad in the system
///         - This is only for during the IPL (will be updated later in step 15)
/// @endverbatim

// -----------------------------------------------------------------------------
//  Includes
// -----------------------------------------------------------------------------
#include "p9_sbe_select_ex.H"
#include "p9_common_poweronoff.H"

// -----------------------------------------------------------------------------
//  Definitions
// -----------------------------------------------------------------------------

static const uint32_t NUM_EX_PER_EQ = 2;

static const uint8_t CORE_CHIPLET_START     = 0x20;
static const uint8_t CORE_CHIPLET_COUNT     = 24;

static const uint8_t ALL_CHIPLETS_MC_GROUP  = 0;
static const uint8_t ALL_CORES_MC_GROUP     = 1;
static const uint8_t CORE_STOP_MC_GROUP     = 3;
static const uint8_t EQ_STOP_MC_GROUP       = 4;
static const uint8_t EX_EVEN_STOP_MC_GROUP  = 5;
static const uint8_t EX_ODD_STOP_MC_GROUP   = 6;
static const uint8_t BROADCAST_GROUP        = 7;

// Use PERV addressses as the accesses to the cores and EQ use PERV targets
static const uint64_t ALL_CHIPLETS_MC_REG   = PERV_MULTICAST_GROUP_1;
static const uint64_t ALL_CORES_MC_REG      = PERV_MULTICAST_GROUP_2;
static const uint64_t CORE_MC_REG           = PERV_MULTICAST_GROUP_3;
static const uint64_t EQ_MC_REG             = PERV_MULTICAST_GROUP_2;
static const uint64_t EX_EVEN_MC_REG        = PERV_MULTICAST_GROUP_3;
static const uint64_t EX_ODD_MC_REG         = PERV_MULTICAST_GROUP_4;

// Note: in the above, the EX MC groups really live in the EQ chiplet, not the
// core!

static const uint8_t PERV_EQ_START      = 0x10;
static const uint8_t PERV_EQ_COUNT      = 6;
static const uint8_t PERV_CORE_START    = 0x20;
static const uint8_t PERV_CORE_COUNT    = 24;

// -----------------------------------------------------------------------------
//  Function prototypes
// -----------------------------------------------------------------------------

fapi2::ReturnCode select_ex_add_core_to_mc_group(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_cplt);

fapi2::ReturnCode select_ex_add_ex_to_mc_groups(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_cplt,
    const uint32_t i_ex_num);

fapi2::ReturnCode select_ex_add_eq_to_mc_group(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_cplt);

// -----------------------------------------------------------------------------
//  Function definitions
// -----------------------------------------------------------------------------

// See .H for documentation
fapi2::ReturnCode p9_sbe_select_ex(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
    const p9selectex::MODE i_mode)
{
    FAPI_IMP("> p9_sbe_select_ex");

    fapi2::buffer<uint64_t> l_core_config = 0;
    fapi2::buffer<uint64_t> l_quad_config = 0;
    fapi2::buffer<uint64_t> l_data64 = 0;
    uint8_t attr_force_all = 0;
    bool b_single = true;
    bool b_host_core_found = false;
    bool b_host_eq_found = false;

    uint32_t l_master_ex_num = 0xFF;  // invalid EX number initialized
    uint32_t l_master_eq_num = 0xFF;  // invalid EQ number initialized
    const fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYSTEM;

    auto l_core_functional_vector = i_target.getChildren<fapi2::TARGET_TYPE_PERV>
                                    (fapi2::TARGET_FILTER_ALL_CORES,
                                     fapi2::TARGET_STATE_FUNCTIONAL );

    auto l_eq_functional_vector = i_target.getChildren<fapi2::TARGET_TYPE_PERV>
                                  (fapi2::TARGET_FILTER_ALL_CACHES,
                                   fapi2::TARGET_STATE_FUNCTIONAL );

    // Read the "FORCE_ALL" attribute
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_SYS_FORCE_ALL_CORES,
                           FAPI_SYSTEM,
                           attr_force_all));

    // Set the flow mode and respect the force mode
    if (attr_force_all || i_mode == p9selectex::ALL)
    {
        b_single = false;
        FAPI_DBG("All cores mode");
    }
    else
    {
        FAPI_DBG("Single core mode:  Number of candidate cores = %d, Number of candidate caches = %d",
                 l_core_functional_vector.size(),
                 l_eq_functional_vector.size());
    }

    // Loop through the core functional vector.  The first core in the vector
    // is going to be the hostboot core as the FAPI platform code is expected
    // to return the vector elements in acsending order; thus, the first vector
    // entry is the lowest numbered, valid core.
    //
    // Two buffers track the core and EX configuration as though "ALL" is the
    // mode chosen.  This is done to reduce conditional processing within the
    // vector loop to allow for better prefetch utilization.

    for (auto& core : l_core_functional_vector)
    {
        uint8_t l_attr_chip_unit_pos = 0; //actual value is read in FAPI_ATTR_GET below
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS,
                               core,
                               l_attr_chip_unit_pos));

        // Needed as core is a PERV target
        uint32_t l_core_num = static_cast<uint32_t>(l_attr_chip_unit_pos - PERV_CORE_START);

        FAPI_DBG("Functional core l_attr_chip_unit_pos 0x%02X, l_core_num =  0x%02X",
                 l_attr_chip_unit_pos, l_core_num);

        uint32_t l_ex_num = l_core_num / 2;
        uint32_t l_eq_num = l_core_num / 4;

        if (b_single)
        {
            if (!b_host_core_found)
            {

                l_master_ex_num = l_ex_num;
                l_master_eq_num = l_eq_num;

                uint8_t l_short_core_num = static_cast<uint8_t>(l_core_num);
                FAPI_TRY(FAPI_ATTR_SET( fapi2::ATTR_MASTER_CORE,
                                        i_target,
                                        l_short_core_num));

                uint8_t l_short_ex_num = static_cast<uint8_t>(l_ex_num);
                FAPI_TRY(FAPI_ATTR_SET( fapi2::ATTR_MASTER_EX,
                                        i_target,
                                        l_short_ex_num));

                FAPI_DBG("MASTER core chiplet %d 0x%02X; EX %d 0x%02X",
                         l_core_num, l_core_num,
                         l_master_ex_num, l_master_ex_num);

                b_host_core_found = true;

                // Add the core to the apppropriate multicast group
                FAPI_TRY(select_ex_add_core_to_mc_group(core));

            } // host_core_found
        } // Single

        // To save code space in the SBE, the assumption is made that if the core
        // is good (eg in the core functional vector), then the EX associated with
        // it is also good.  No checking is performed on the associated the EX
        // targets to check this.
        //
        // Thus, set the bits in the buffers for the OCC configuration register
        // update
        FAPI_DBG("core num = %d, ex num = %d",
                 l_core_num, l_ex_num);
        l_core_config.setBit(l_core_num);
        l_quad_config.setBit(l_ex_num);

        FAPI_DBG("Scoreboard values for OCC: Core 0x%016llX  EX 0x%016llX",
                 l_core_config, l_quad_config);

        // Write the default PFET Controller Delay values for the Core
        // as it will be used for istep 4
        FAPI_DBG("Setting PFET Delays in core %d", l_core_num);

        l_data64.flush<0>()
        .insertFromRight<0, 4>(p9power::PFET_DELAY_POWERDOWN_CORE)
        .insertFromRight<4, 4>(p9power::PFET_DELAY_POWERUP_CORE);

        FAPI_TRY(fapi2::putScom(core,
                                C_PPM_PFDLY - 0x20000000,  // Create chip address base
                                l_data64),
                 "Error: Core PFET Delay register");

        // if b_host_core_found (only set in single mode), break out of core loop
        if (b_host_core_found)
        {
            break;
        }

    } // Core loop

    // Process the good EQs
    for (auto& eq : l_eq_functional_vector)
    {
        uint8_t l_attr_chip_unit_pos = 0; //actual value is read in FAPI_ATTR_GET below
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS,
                               eq,
                               l_attr_chip_unit_pos));

        // Needed as eq is a PERV target
        uint32_t l_eq_num = static_cast<uint32_t>(l_attr_chip_unit_pos - PERV_EQ_START);

        FAPI_DBG("Functional EQ l_attr_chip_unit_pos 0x%02X, l_eq_num =  0x%02X",
                 l_attr_chip_unit_pos, l_eq_num);

        if (b_single)
        {
            if (l_eq_num == l_master_eq_num)
            {
                FAPI_TRY(select_ex_add_eq_to_mc_group(eq));
                b_host_eq_found = true;
            }

            for (auto i = l_eq_num * NUM_EX_PER_EQ; i < (l_eq_num + 1)*NUM_EX_PER_EQ; ++i)
            {
                FAPI_DBG("ex = %d, master ex = %d, quad bit[%d] = %d",
                         i, l_master_ex_num, i, l_quad_config.getBit(i));

                // Add to MC group
                if ((i == l_master_ex_num) && l_quad_config.getBit(i))
                {
                    FAPI_TRY(select_ex_add_ex_to_mc_groups(eq, i));
                }
            }
        } // Single

        FAPI_DBG("Setting PFET Delays in EQ %d", l_eq_num);

        // Write the default PFET Controller Delay values for the EQs
        // that will be used for istep 4
        l_data64.flush<0>()
        .insertFromRight<0, 4>(p9power::PFET_DELAY_POWERDOWN_EQ)
        .insertFromRight<4, 4>(p9power::PFET_DELAY_POWERUP_EQ);

        FAPI_TRY(fapi2::putScom(eq,
                                EQ_PPM_PFDLY - 0x10000000,  // Create chip address base
                                l_data64),
                 "Error: EQ PFET Delay register, rc 0x%.8X",
                 (uint32_t)fapi2::current_err);

        // if b_eq_eq_found (only set in single mode), break out of EQ loop
        if (b_host_eq_found)
        {
            break;
        }

    } // EQ loop


    // Write to the OCC Core Configuration Status Register
    FAPI_TRY(fapi2::putScom(i_target, PU_OCB_OCI_CCSR_SCOM2, l_core_config));

    // Write to the OCC Quad Configuration Status Register
    FAPI_TRY(fapi2::putScom(i_target, PU_OCB_OCI_QCSR_SCOM2, l_quad_config));

    // Write default value the OCC Quad Status Status Register
    l_data64.flush<0>()
    .setBit<0, 12>()       // L2 Stopped
    .setBit<14, 6>();      // Quad Stopped
    FAPI_TRY(fapi2::putScom(i_target, PU_OCB_OCI_QSSR_SCOM2, l_data64));

fapi_try_exit:
    FAPI_INF("< p9_sbe_select_ex");

    return fapi2::current_err;
} // END p9_sbe_select_ex

///-----------------------------------------------------------------------------
/// @brief Add core chiplet to Dynamic cores multicast group
///
/// @param[in]     i_target_cplt   Reference to TARGET_TYPE_PERV target
///                                     that is a core
/// @return  FAPI2_RC_SUCCESS if success, else error code.
fapi2::ReturnCode select_ex_add_core_to_mc_group(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_cplt)
{
    FAPI_INF("> add_to_core_mc_group...");

    fapi2::buffer<uint64_t> l_data64 = 0;

    // Add core to functional Chiplets group

    // Entering group
    l_data64.insertFromRight<0, 3>(0x7);
    l_data64.insertFromRight<3, 3>(ALL_CHIPLETS_MC_GROUP);
    // Removed group
    l_data64.insertFromRight<19, 3>(BROADCAST_GROUP);

#ifndef __PPE__
    uint8_t l_attr_chip_unit_pos = 0;
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS,
                           i_target_cplt,
                           l_attr_chip_unit_pos));

    FAPI_DBG("Adding Core %d to MC group %d",
             l_attr_chip_unit_pos - PERV_CORE_START,
             ALL_CHIPLETS_MC_GROUP );
#endif
    FAPI_TRY(fapi2::putScom(i_target_cplt,
                            ALL_CHIPLETS_MC_REG,
                            l_data64),
             "Error: Core MC group register, rc 0x%.8X",
             (uint32_t)fapi2::current_err);

    // Add core to functional core group
    // Entering group
    l_data64.insertFromRight<0, 3>(0x7);
    l_data64.insertFromRight<3, 3>(ALL_CORES_MC_GROUP);
    // Removed group
    l_data64.insertFromRight<19, 3>(BROADCAST_GROUP);

#ifndef __PPE__
    FAPI_DBG("Adding Core %d to MC group %d",
             l_attr_chip_unit_pos - PERV_CORE_START,
             ALL_CORES_MC_GROUP );
#endif
    FAPI_TRY(fapi2::putScom(i_target_cplt,
                            ALL_CORES_MC_REG,
                            l_data64),
             "Error: Core MC group register, rc 0x%.8X",
             (uint32_t)fapi2::current_err);

    // Entering group
    l_data64.insertFromRight<0, 3>(0x7);
    l_data64.insertFromRight<3, 3>(CORE_STOP_MC_GROUP);
    // Removed group
    l_data64.insertFromRight<19, 3>(BROADCAST_GROUP);

#ifndef __PPE__
    FAPI_DBG("Adding Core %d to MC group %d",
             l_attr_chip_unit_pos - PERV_CORE_START,
             CORE_STOP_MC_GROUP );
#endif

    FAPI_TRY(fapi2::putScom(i_target_cplt,
                            CORE_MC_REG,
                            l_data64),
             "Error: Core MC group register, rc 0x%.8X",
             (uint32_t)fapi2::current_err);

fapi_try_exit:
    FAPI_INF("< add_to_core_mc_group...");
    return fapi2::current_err;

}

///-----------------------------------------------------------------------------
/// @brief Add EX to multicast groups
///
/// @param[in]     i_ex_num  EX number that needs to be added to MC groups
///
/// @return  FAPI2_RC_SUCCESS if success, else error code.
fapi2::ReturnCode select_ex_add_ex_to_mc_groups(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_cplt,
    const uint32_t i_ex_num)
{
    FAPI_INF("> select_ex_add_ex_to_mc_groups...");

    // If the Core is in a even EX, then put the EQ chiplet in the EQ MC group
    // and the EX Even MC group.

    // If the Core is in a odd EX, then put the EQ chiplet in the EQ MC group
    // and the EX Odd MC group.

    fapi2::buffer<uint64_t> l_data64 = 0;

    l_data64.insertFromRight<0, 3>(0x7);
    // Removed group
    l_data64.insertFromRight<19, 3>(BROADCAST_GROUP);

    if (i_ex_num % 2)   // Odd EX
    {

        FAPI_DBG("Add EX %d (Odd) to MC group %d",
                 i_ex_num,
                 EX_ODD_STOP_MC_GROUP);

        // Entering group
        l_data64.insertFromRight<3, 3>(EX_ODD_STOP_MC_GROUP);

        FAPI_TRY(fapi2::putScom(i_target_cplt,
                                EX_ODD_MC_REG,
                                l_data64),
                 "Error: EX Odd MC group register, rc 0x%.8X",
                 (uint32_t)fapi2::current_err);

    }
    else                // Even EX
    {
        FAPI_DBG("Add EX %d (Even) to MC group %d",
                 i_ex_num,
                 EX_EVEN_STOP_MC_GROUP);

        // Entering group
        l_data64.insertFromRight<3, 3>(EX_EVEN_STOP_MC_GROUP);

        FAPI_TRY(fapi2::putScom(i_target_cplt,
                                EX_EVEN_MC_REG,
                                l_data64),
                 "Error: EX Even MC group register, rc 0x%.16X",
                 (uint32_t)fapi2::current_err);
    }

fapi_try_exit:
    FAPI_INF("< select_ex_add_ex_to_mc_groups...");
    return fapi2::current_err;

}

///-----------------------------------------------------------------------------
/// @brief Add EQ to multicast groups
///
/// @return  FAPI2_RC_SUCCESS if success, else error code.
fapi2::ReturnCode select_ex_add_eq_to_mc_group(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_cplt)
{
    FAPI_INF("> select_ex_add_eq_to_mc_group...");

    fapi2::buffer<uint64_t> l_data64;

    // Add EQ to functional chiplets group

    // Entering group
    l_data64.insertFromRight<0, 3>(0x7);
    l_data64.insertFromRight<3, 3>(ALL_CHIPLETS_MC_GROUP);
    // Removed group
    l_data64.insertFromRight<19, 3>(BROADCAST_GROUP);

#ifndef __PPE__
    uint8_t l_attr_chip_unit_pos = 0;
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS,
                           i_target_cplt,
                           l_attr_chip_unit_pos));

    FAPI_DBG("Adding EQ %d to MC group %d",
             l_attr_chip_unit_pos - PERV_EQ_START,
             ALL_CHIPLETS_MC_GROUP );
#endif

    FAPI_TRY(fapi2::putScom(i_target_cplt,
                            ALL_CHIPLETS_MC_REG,
                            l_data64),
             "Error: EQ All chiplets MC group register, rc 0x%.8X",
             (uint32_t)fapi2::current_err);

    // Add EQ to EQ STOP group

    // Entering group
    l_data64.insertFromRight<0, 3>(0x7);
    l_data64.insertFromRight<3, 3>(EQ_STOP_MC_GROUP);
    // Removed group
    l_data64.insertFromRight<19, 3>(BROADCAST_GROUP);

#ifndef __PPE__
    FAPI_DBG("Adding EQ %d to MC group %d",
             l_attr_chip_unit_pos - PERV_EQ_START,
             EQ_STOP_MC_GROUP );
#endif

    FAPI_TRY(fapi2::putScom(i_target_cplt,
                            EQ_MC_REG,
                            l_data64),
             "Error: EQ MC group register, rc 0x%.8X",
             (uint32_t)fapi2::current_err);

fapi_try_exit:
    FAPI_INF("< select_ex_add_eq_to_mc_group...");
    return fapi2::current_err;

}

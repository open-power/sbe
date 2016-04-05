/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: chips/p9/procedures/hwp/perv/p9_sbe_select_ex.C $             */
/*                                                                        */
/* IBM CONFIDENTIAL                                                       */
/*                                                                        */
/* EKB Project                                                            */
/*                                                                        */
/* COPYRIGHT 2015,2016                                                    */
/* [+] International Business Machines Corp.                              */
/*                                                                        */
/*                                                                        */
/* The source code for this program is not published or otherwise         */
/* divested of its trade secrets, irrespective of what has been           */
/* deposited with the U.S. Copyright Office.                              */
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
///   Following MC groups are needed to be setup for istep 4 use:
///     - MC group 3:  Core(s) (eg ECs);  use EC MC group register 3
///     - MC group 4:  EQ(s);             use EQ MC group register 2
///     - MC group 5:  Even EXs;          use EQ MC group register 3
///     - MC group 6:  Odd Exs;           use EQ MC group register 4
///
///   Prerequisite:  istep 2 will setup the above groups with ALL the good
///                  elements represented.
///
///   This procedure will REMOVE entities from these groups in SINGLE mode;
///   in ALL mode, the groups are not changed.  In either case, the OCC
///   registers are written with the valid configuration.  Additionally,
///   default PFET controller delays are written into all configured
///   EC and EQ chiplets so that istep 4 power-on operations will
///   succeed.
///
///   Parameter indicates single core or all (controlled by Cronus/SBE)
///
///     loop over functional cores {
///         if mode == SINGLE {
///             if first one {
///                 Record the master core, EX and EQ number
///             }
///             else {
///                 Remove from MC Group 3
///             }
///         }
///         Set bits in core and EX scoreboard for later updating the OCC
///         Set default PFET controller delay values into Core
///       }
///
///     loop over functional EQs {
///         if mode == SINGLE {
///             if not master EQ {
///                 Remove from MC Groups 4
///             for the EXs in the EQ {
///                 if not master EX && bit is set in EX scoreboard
///                     Remove from MC Group 5 if Even (EX0)
///                     Remove from MC Group 6 if Odd (EX1)
///         }
///         Set default PFET controller delay values into EQ
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

static const uint8_t CORE_STOP_MC_GROUP     = 3;
static const uint8_t EQ_STOP_MC_GROUP       = 4;
static const uint8_t EX_EVEN_STOP_MC_GROUP  = 5;
static const uint8_t EX_ODD_STOP_MC_GROUP   = 6;
static const uint8_t BROADCAST_GROUP        = 7;

// Use PERV addressses as the accesses to the cores and EQ use PERV targets
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

fapi2::ReturnCode select_ex_remove_core_from_mc_group(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_cplt);

fapi2::ReturnCode select_ex_remove_ex_from_mc_group(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_cplt,
    const uint32_t i_ex_num);

fapi2::ReturnCode select_ex_remove_eq_from_mc_group(
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
    bool b_processing_host_core = false;

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

    for (auto core : l_core_functional_vector)
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
            b_processing_host_core = false;

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
                b_processing_host_core = true;

            } // host_core_found

            // Remove the core from the apppropriate multicast group if not
            // the host core
            if (!b_processing_host_core)
            {
                FAPI_TRY(select_ex_remove_core_from_mc_group(core));
            }

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

    } // Core loop

    // Process the good EQs
    for (auto eq : l_eq_functional_vector)
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
            if (l_eq_num != l_master_eq_num)
            {
                FAPI_TRY(select_ex_remove_eq_from_mc_group(eq));
            }

            for (auto i = l_eq_num * NUM_EX_PER_EQ; i < (l_eq_num + 1)*NUM_EX_PER_EQ; ++i)
            {
                FAPI_DBG("ex = %d, master ex = %d, quad bit[%d] = %d",
                         i, l_master_ex_num, i, l_quad_config.getBit(i));

                // Remove from MC group if not master EX and configured
                if ((i != l_master_ex_num) && l_quad_config.getBit(i))
                {
                    FAPI_TRY(select_ex_remove_ex_from_mc_group(eq, i));
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
/// @brief Remve core chiplet from Dynamic cores multicast group
///
/// @param[in]     i_target_cplt   Reference to TARGET_TYPE_PERV target
///                                     that is a core
/// @return  FAPI2_RC_SUCCESS if success, else error code.
fapi2::ReturnCode select_ex_remove_core_from_mc_group(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_cplt)
{
    FAPI_INF("> remove_from_core_mc_group...");

    fapi2::buffer<uint64_t> l_data64 = 0;

    // Entering group
    l_data64.insertFromRight<0, 3>(0x7);
    l_data64.insertFromRight<3, 3>(BROADCAST_GROUP);
    // Removed group
    l_data64.insertFromRight<19, 3>(CORE_STOP_MC_GROUP);

#ifndef __PPE__
    uint8_t l_attr_chip_unit_pos = 0;
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS,
                           i_target_cplt,
                           l_attr_chip_unit_pos));

    FAPI_DBG("Removing Core %d from MC group %d",
             l_attr_chip_unit_pos - PERV_CORE_START,
             CORE_STOP_MC_GROUP );
#endif

    FAPI_TRY(fapi2::putScom(i_target_cplt,
                            CORE_MC_REG,
                            l_data64),
             "Error: Core MC group register, rc 0x%.8X",
             (uint32_t)fapi2::current_err);

fapi_try_exit:
    FAPI_INF("< remove_from_core_mc_group...");
    return fapi2::current_err;

}

///-----------------------------------------------------------------------------
/// @brief Remove EX from multicast group
///
/// @param[in]     i_ex_num  EX number that needs to be removed from an MC group
///
/// @return  FAPI2_RC_SUCCESS if success, else error code.
fapi2::ReturnCode select_ex_remove_ex_from_mc_group(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_cplt,
    const uint32_t i_ex_num)
{
    FAPI_INF("> select_ex_remove_ex_from_mc_group...");

    // If the Core is in a even EX, then put the EQ chiplet in the EQ MC group
    // and the EX Even MC group.

    // If the Core is in a odd EX, then put the EQ chiplet in the EQ MC group
    // and the EX Odd MC group.

    fapi2::buffer<uint64_t> l_data64 = 0;

    // Entering group
    l_data64.insertFromRight<0, 3>(0x7);
    l_data64.insertFromRight<3, 3>(BROADCAST_GROUP);

    if (i_ex_num % 2)   // Odd EX
    {
        FAPI_DBG("Removing EX %d (Odd) from MC group %d",
                 i_ex_num,
                 EX_ODD_STOP_MC_GROUP);

        // Removed group
        l_data64.insertFromRight<19, 3>(EX_ODD_STOP_MC_GROUP);

        FAPI_TRY(fapi2::putScom(i_target_cplt,
                                EX_ODD_MC_REG,
                                l_data64),
                 "Error: EX Odd MC group register, rc 0x%.8X",
                 (uint32_t)fapi2::current_err);

    }
    else                // Even EX
    {
        FAPI_DBG("Removing EX %d (Even) from MC group %d",
                 i_ex_num,
                 EX_EVEN_STOP_MC_GROUP);


        // Removed group
        l_data64.insertFromRight<19, 3>(EX_EVEN_STOP_MC_GROUP);

        FAPI_TRY(fapi2::putScom(i_target_cplt,
                                EX_EVEN_MC_REG,
                                l_data64),
                 "Error: EX Even MC group register, rc 0x%.16X",
                 (uint32_t)fapi2::current_err);
    }

fapi_try_exit:
    FAPI_INF("< select_ex_remove_ex_from_mc_group...");
    return fapi2::current_err;

}

///-----------------------------------------------------------------------------
/// @brief Remove EX from multicast group
///
/// @param[in]     i_ex_num  EX number for which the EQ needs to be in MC group
///
/// @return  FAPI2_RC_SUCCESS if success, else error code.
fapi2::ReturnCode select_ex_remove_eq_from_mc_group(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_cplt)
{
    FAPI_INF("> select_ex_remove_eq_from_mc_group...");

    fapi2::buffer<uint64_t> l_data64;

    // Entering group
    l_data64.insertFromRight<0, 3>(0x7);
    l_data64.insertFromRight<3, 3>(BROADCAST_GROUP);
    // Removed group
    l_data64.insertFromRight<19, 3>(EQ_STOP_MC_GROUP);

#ifndef __PPE__
    uint8_t l_attr_chip_unit_pos = 0;
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS,
                           i_target_cplt,
                           l_attr_chip_unit_pos));

    FAPI_DBG("Removing EQ %d from MC group %d",
             l_attr_chip_unit_pos - PERV_EQ_START,
             EQ_STOP_MC_GROUP );
#endif

    FAPI_TRY(fapi2::putScom(i_target_cplt,
                            EQ_MC_REG,
                            l_data64),
             "Error: EQ MC group register, rc 0x%.8X",
             (uint32_t)fapi2::current_err);

fapi_try_exit:
    FAPI_INF("< select_ex_remove_eq_from_mc_group...");
    return fapi2::current_err;

}

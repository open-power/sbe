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
///   Setup the following MC groups for istep 4 use:
///     - MC group 2:  Core(s) (eg ECs);  use EC MC group register 2
///     - MC group 4:  EQ(s);             use EQ MC group register 2
///     - MC group 5:  Even EXs;          use EQ MC group register 3
///     - MC group 6:  Odd Exs;           use EQ MC group register 4
///   Parameter indicates single core or all (controlled by Cronus/SBE)
///     - For all, all functional EC/EQ are added to respective MC groups 2, 4,
///         5, and 6
///     - For single, the first functional EC/EQ is added to respective MC
///         groups 2, 4, 5 and 6.  Note: for single, only 1 EX will be active;
///         Thus, either MC group 5 or 6 will have no chiplets included.
///   Write selected (single/all) EQ/Core mask into OCC complex
///     - This is the "master record " of the enabled cores/quad in the system
///         - This is only for during the IPL (will be updated later in step 15)
/// @endverbatim

// -----------------------------------------------------------------------------
//  Includes
// -----------------------------------------------------------------------------
#include "p9_sbe_select_ex.H"

// -----------------------------------------------------------------------------
//  Definitions
// -----------------------------------------------------------------------------

namespace p9_selex
{
static const uint8_t CORE_CHIPLET_START     = 0x20;
static const uint8_t CORE_CHIPLET_COUNT     = 24;

static const uint8_t CORE_STOP_MC_GROUP     = 3;
static const uint8_t EQ_STOP_MC_GROUP       = 4;
static const uint8_t EX_EVEN_STOP_MC_GROUP  = 5;
static const uint8_t EX_ODD_STOP_MC_GROUP   = 6;

// Use PERV addressses as the accesses to the cores and EQ use PERV targets
static const uint64_t CORE_MC_REG           = PERV_MULTICAST_GROUP_2;
static const uint64_t EQ_MC_REG             = PERV_MULTICAST_GROUP_2;
static const uint64_t EX_EVEN_MC_REG        = PERV_MULTICAST_GROUP_3;
static const uint64_t EX_ODD_MC_REG         = PERV_MULTICAST_GROUP_4;

// Note: in the above, the EX MC groups really live in the EQ chiplet, not the
// core!

static const uint8_t PERV_EQ_START      = 0x10;
static const uint8_t PERV_EQ_COUNT      = 6;
static const uint8_t PERV_CORE_START    = 0x20;
static const uint8_t PERV_CORE_COUNT    = 24;
};  // namespace p9_selex

// -----------------------------------------------------------------------------
//  Function prototypes
// -----------------------------------------------------------------------------

static fapi2::ReturnCode set_core_mc_group(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_cplt);

static fapi2::ReturnCode set_eq_mc_groups(
    const std::vector<fapi2::Target<fapi2::TARGET_TYPE_PERV> >& i_eq_functional_vector,
    const uint8_t i_ex_num);

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
    uint8_t attr_force_all = 0;
    bool b_single = true;
    bool b_first = true;

    auto l_perv_functional_vector =
        i_target.getChildren<fapi2::TARGET_TYPE_PERV>
        (fapi2::TARGET_STATE_FUNCTIONAL);

    std::vector<fapi2::Target<fapi2::TARGET_TYPE_PERV>> l_core_functional_vector;
    std::vector<fapi2::Target<fapi2::TARGET_TYPE_PERV>> l_eq_functional_vector;

    // Find the good cores
    for (auto it : l_perv_functional_vector)
    {
        uint8_t l_attr_chip_unit_pos = 0; //actual value is read in FAPI_ATTR_GET below
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS,
                               it,
                               l_attr_chip_unit_pos));

        // If not a core, continue to next vector entry
        if (!(l_attr_chip_unit_pos >= p9_selex::PERV_CORE_START &&
              l_attr_chip_unit_pos <= p9_selex::PERV_CORE_START + p9_selex::PERV_CORE_COUNT))
        {
            continue;
        }

        l_core_functional_vector.push_back(it);

    }

    // Find the good EQs
    for (auto it : l_perv_functional_vector)
    {
        uint8_t l_attr_chip_unit_pos = 0; //actual value is read in FAPI_ATTR_GET below
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS,
                               it,
                               l_attr_chip_unit_pos));

        // If not a EQ, continue to next vector entry
        if (!(l_attr_chip_unit_pos >= p9_selex::PERV_EQ_START &&
              l_attr_chip_unit_pos <= p9_selex::PERV_EQ_START + p9_selex::PERV_EQ_COUNT))
        {
            continue;
        }

        l_eq_functional_vector.push_back(it);

    }

    // Check that configuration allows success
    FAPI_ASSERT(l_core_functional_vector.size(),
                fapi2::SBE_SELECT_EX_NO_CORES(),
                "No functional cores found");

    FAPI_ASSERT(l_eq_functional_vector.size(),
                fapi2::SBE_SELECT_EX_NO_EQS(),
                "No functional cache chiplets (EQ) found");

    FAPI_DBG("  Number of candidate cores = %d",
             l_core_functional_vector.size());
    FAPI_DBG("  Number of candidate caches = %d",
             l_eq_functional_vector.size());


    // Read the "FORCE_ALL" attribute
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_SYS_FORCE_ALL_CORES,
                           i_target,
                           attr_force_all));

    // Set the flow mode and respect the force mode
    if (attr_force_all || i_mode == p9selectex::ALL)
    {
        b_single = false;
    }

    // Loop through the core functional vector looking for the lowest numbered
    // core.  This fills out the two buffers tracking the core and EX
    // configuration as though "ALL" is the mode chosen.  This is done to
    // reduce conditional processing within the vector loop to allow for
    // better prefetch utilization.  Also, the FAPI platform code is
    // expected to return the vector elements in acsending order; thus, the
    // first vector entry is the lowest numbered, valid core.

    for (auto it : l_core_functional_vector)
    {
        uint8_t l_attr_chip_unit_pos = 0; //actual value is read in FAPI_ATTR_GET below
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS,
                               it,
                               l_attr_chip_unit_pos));

        uint32_t l_core_num = static_cast<uint32_t>(l_attr_chip_unit_pos - p9_selex::PERV_CORE_START);

        FAPI_DBG("Functional core l_attr_chip_unit_pos 0x%02X, l_core_num =  0x%02X",
                 l_attr_chip_unit_pos, l_core_num);

        uint32_t l_ex_num = l_core_num / 2;
        uint32_t l_eq_num = l_core_num / 4;

        // Determine that the cache associated with the core is also
        // in the  list
        bool b_eq_found = false;

        for (auto it : l_eq_functional_vector)
        {
            FAPI_TRY(FAPI_ATTR_GET( fapi2::ATTR_CHIP_UNIT_POS,
                                    it,
                                    l_attr_chip_unit_pos));

            if ((l_attr_chip_unit_pos - p9_selex::PERV_EQ_START) == static_cast<uint8_t>(l_eq_num))
            {
                b_eq_found = true;
                break;
            }
        }

        FAPI_ASSERT(b_eq_found,
                    fapi2::SBE_SELECT_EX_CORE_EQ_CONFIG_ERROR()
                    .set_CORE_NUM(l_core_num)
                    .set_EQ_NUM(l_eq_num),
                    "Did not find matching EQ for the core");

        // At this point, all the checks have passed so add the core and the EQ/EX
        // to the apppropriate multicast groups and set the bits in the buffers for
        // the OCC configuration registers.

        // Core
        FAPI_TRY(set_core_mc_group(it));
        l_core_config.setBit(l_core_num);

        // EQ/EX
        FAPI_TRY(set_eq_mc_groups(l_eq_functional_vector, l_ex_num));
        l_quad_config.setBit(l_ex_num);

        if (b_first)
        {
            uint8_t l_core_num_short = static_cast<uint8_t>(l_core_num);
            FAPI_TRY(FAPI_ATTR_SET( fapi2::ATTR_MASTER_CORE,
                                    i_target,
                                    l_core_num_short));

            uint8_t l_ex_num_short = static_cast<uint8_t>(l_ex_num);
            FAPI_TRY(FAPI_ATTR_SET( fapi2::ATTR_MASTER_EX,
                                    i_target,
                                    l_ex_num_short));

            FAPI_DBG("MASTER core chiplet %d 0x%02X; EX %d 0x%02X",
                     l_core_num, l_core_num,
                     l_ex_num, l_ex_num);

            b_first = false;
        }

        if (b_single)
        {
            FAPI_DBG("SINGLE mode: chosen core chiplet %d 0x%02X; EX %d 0x%02X",
                     l_core_num, l_core_num,
                     l_ex_num, l_ex_num);
            FAPI_DBG("SINGLE mode: core config: %016llX", l_core_config);

            FAPI_DBG("SINGLE mode:  chiplet id 0x%02X; selected element %d Target Type %X",
                     l_core_num + 0x20, 0, l_core_functional_vector.at(0).getType());

            // We found the core and cache so exit in SINGLE mode
            break;

        }
        else
        {
            FAPI_DBG("ALL mode: core chiplet %d 0x%02X with is chiplet id 0x%02X",
                     l_core_num, l_core_num, l_core_num + 0x20);

            FAPI_DBG("ALL mode: core chiplet %d %02X; EX %d %02X",
                     l_core_num, l_core_num,
                     l_ex_num, l_ex_num);
            FAPI_DBG("ALL mode: core config: %016llX", l_core_config);

        }
    }

    FAPI_ASSERT(l_core_config(),
                fapi2::SBE_SELECT_EX_NO_CORE_AVAIL_ERROR(),
                "No cores are configurable");

    // Write to the OCC Core Configuration Status Register
    FAPI_TRY(fapi2::putScom(i_target, PU_OCB_OCI_CCSR_SCOM2, l_core_config));

    // Write to the OCC Quad Configuration Status Register
    FAPI_TRY(fapi2::putScom(i_target, PU_OCB_OCI_QCSR_SCOM2, l_quad_config));

fapi_try_exit:
    FAPI_INF("< p9_sbe_select_ex");

    return fapi2::current_err;
} // END p9_sbe_select_ex

///-----------------------------------------------------------------------------
/// @brief Set multicast group to core chiplet
///
/// @param[in]     i_core_target_cplt   Reference to TARGET_TYPE_PERV target
///                                     that is a core
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode set_core_mc_group(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_core_target_cplt)
{
    FAPI_INF("> set_core_mc_group...");

    fapi2::buffer<uint64_t> l_data64;
    uint32_t l_core_num;

    FAPI_DBG("Setting Core MC group %d", p9_selex::CORE_STOP_MC_GROUP );
    l_data64.insertFromRight<3, 3>(p9_selex::CORE_STOP_MC_GROUP);

    uint8_t l_attr_chip_unit_pos = 0;
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS,
                           i_core_target_cplt,
                           l_attr_chip_unit_pos));

    l_core_num = l_attr_chip_unit_pos - p9_selex::PERV_CORE_START;
    FAPI_DBG("Setting MC group in core %d", l_core_num);

    FAPI_TRY(fapi2::putScom(i_core_target_cplt,
                            p9_selex::CORE_MC_REG,
                            l_data64),
             "Error: Core MC group register");

fapi_try_exit:
    FAPI_INF("< set_core_mc_group...");
    return fapi2::current_err;

}

///-----------------------------------------------------------------------------
/// @brief Set multicast groups for the EQ chiplet(s)
///
/// @param[in]     i_eq_functional_vector  Vector of functional EQs
/// @param[in]     i_ex_num  EX number for which the EQ needs to be in MC group
///
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode set_eq_mc_groups(
    const std::vector<fapi2::Target<fapi2::TARGET_TYPE_PERV> >& i_eq_functional_vector,
    const uint8_t i_ex_num)
{
    FAPI_INF("> set_eq_mc_groups...");

    // If the Core is in a even EX, then put the EQ chiplet in the EQ MC group
    // and the EX Even MC group.

    // If the Core is in a odd EX, then put the EQ chiplet in the EQ MC group
    // and the EX Odd MC group.

    fapi2::buffer<uint64_t> l_data64;

    uint8_t l_eq_num = i_ex_num / 2;  // Two EX per EQ

    // Search the EQ functional vector for one that corresponds to the input EX number.
    // If the correct EQ is not found, flag an error
    for (auto it : i_eq_functional_vector)
    {
        uint8_t l_attr_chip_unit_pos = 0; //actual value is read in FAPI_ATTR_GET below

        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS,
                               it,
                               l_attr_chip_unit_pos));

        // See if the input EX is in the current EQ
        if (l_eq_num == l_attr_chip_unit_pos - p9_selex::PERV_EQ_START)
        {
            if (l_eq_num % 2)   // Odd EX
            {
                FAPI_DBG("EQ %d associated with EX %d (Odd) put into MC group %d",
                         l_eq_num,
                         i_ex_num,
                         p9_selex::EX_ODD_STOP_MC_GROUP);
                l_data64.insertFromRight<3, 3>(p9_selex::EX_ODD_STOP_MC_GROUP);

                FAPI_TRY(fapi2::putScom(it,
                                        p9_selex::EX_ODD_MC_REG,
                                        l_data64),
                         "Error: EX Odd MC group register");

            }
            else                // Even EX
            {
                FAPI_DBG("EQ %d associated with EX %d (Even) put into MC group %d",
                         l_eq_num,
                         i_ex_num,
                         p9_selex::EX_EVEN_STOP_MC_GROUP);
                l_data64.insertFromRight<3, 3>(p9_selex::EX_EVEN_STOP_MC_GROUP);

                FAPI_TRY(fapi2::putScom(it,
                                        p9_selex::EX_EVEN_MC_REG,
                                        l_data64),
                         "Error: EX Even MC group register z");
            }

            // EQ
            FAPI_DBG("EQ %d put into MC group %d",
                     l_eq_num,
                     p9_selex::EQ_STOP_MC_GROUP);
            l_data64.insertFromRight<3, 3>(p9_selex::EQ_STOP_MC_GROUP);

            FAPI_TRY(fapi2::putScom(it,
                                    p9_selex::EQ_MC_REG,
                                    l_data64),
                     "Error: EQ MC group register");
        }
    }

fapi_try_exit:
    FAPI_INF("< set_eq_mc_groups...");
    return fapi2::current_err;

}

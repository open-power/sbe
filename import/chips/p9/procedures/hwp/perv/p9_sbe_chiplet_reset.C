/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: chips/p9/procedures/hwp/perv/p9_sbe_chiplet_reset.C $         */
/*                                                                        */
/* IBM CONFIDENTIAL                                                       */
/*                                                                        */
/* EKB Project                                                            */
/*                                                                        */
/* COPYRIGHT 2015                                                         */
/* [+] International Business Machines Corp.                              */
/*                                                                        */
/*                                                                        */
/* The source code for this program is not published or otherwise         */
/* divested of its trade secrets, irrespective of what has been           */
/* deposited with the U.S. Copyright Office.                              */
/*                                                                        */
/* IBM_PROLOG_END_TAG                                                     */
//------------------------------------------------------------------------------
/// @file  p9_sbe_chiplet_reset.C
///
/// @brief Steps:-
///     1) Identify Partical good chiplet and configure Multicasting register
///     2) Similar way, Configure hang pulse counter for Nest/MC/OBus/XBus/PCIe
///     3) Similar way,  set fence for Nest and MC chiplet
///     4) Similar way,  Reset sys.config and OPCG setting for Nest and MC chiplet in sync mode
///
/// Done
//------------------------------------------------------------------------------
// *HWP HW Owner        : Abhishek Agarwal <abagarw8@in.ibm.com>
// *HWP HW Backup Owner : Srinivas V. Naga <srinivan@in.ibm.com>
// *HWP FW Owner        : Brian Silver <bsilver@us.ibm.com>
// *HWP Team            : Perv
// *HWP Level           : 2
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------


//## auto_generated
#include "p9_sbe_chiplet_reset.H"

#include "p9_perv_scom_addresses.H"


static fapi2::ReturnCode p9_sbe_chiplet_reset_all_cplt_hang_cnt_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_cplt);

static fapi2::ReturnCode p9_sbe_chiplet_reset_all_cplt_net_cntl_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_cplt);

static fapi2::ReturnCode p9_sbe_chiplet_reset_cache_hang_cnt_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_ep);

static fapi2::ReturnCode p9_sbe_chiplet_reset_cache_mc_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_ep);

static fapi2::ReturnCode p9_sbe_chiplet_reset_core_hang_cnt_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_ec);

static fapi2::ReturnCode p9_sbe_chiplet_reset_core_mc_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_ec);

static fapi2::ReturnCode p9_sbe_chiplet_reset_nest_hang_cnt_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_cplt);

static fapi2::ReturnCode p9_sbe_chiplet_reset_nest_mc_call(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_cplt);

static fapi2::ReturnCode p9_sbe_chiplet_reset_nest_mc_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_cplt);

static fapi2::ReturnCode p9_sbe_chiplet_reset_opcg_cnfg(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_clpt);

static fapi2::ReturnCode p9_sbe_chiplet_reset_opcg_cnfg(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_cplt,
    const uint8_t i_attr_mc_sync_mode);

static fapi2::ReturnCode p9_sbe_chiplet_reset_pg_fence_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet);

fapi2::ReturnCode p9_sbe_chiplet_reset(const
                                       fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    // Local variable
    uint8_t l_mc_sync_mode = 0;
    auto l_perv_functional_vector =
        i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
        (fapi2::TARGET_STATE_FUNCTIONAL);
    std::vector<fapi2::Target<fapi2::TARGET_TYPE_PERV>> l_nestchiplets_vector;
    std::vector<fapi2::Target<fapi2::TARGET_TYPE_PERV>> l_cachechiplets_vector;
    std::vector<fapi2::Target<fapi2::TARGET_TYPE_PERV>> l_corechiplets_vector;
    FAPI_DBG("Entering ...");

    // Collecting MC sync mode attribute...
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_MC_SYNC_MODE, i_target_chip,
                           l_mc_sync_mode));

    // Configuring chiplet multicasting registers..
    FAPI_TRY(p9_sbe_chiplet_reset_nest_mc_call(i_target_chip));

    // Get targets
    for (auto l_target_cplt : l_perv_functional_vector)
    {
        uint8_t l_attr_chip_unit_pos = 0; //actual value is read in FAPI_ATTR_GET below
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_target_cplt,
                               l_attr_chip_unit_pos));

        if ((l_attr_chip_unit_pos >= 0x10
             && l_attr_chip_unit_pos <= 0x15))/* CacheChiplet */
        {
            l_cachechiplets_vector.push_back(l_target_cplt);
        }
    }

    for (auto l_target_cplt : l_cachechiplets_vector)
    {
        // Configuring chiplet multicasting registers..
        FAPI_TRY(p9_sbe_chiplet_reset_cache_mc_setup(l_target_cplt));
    }

    // Get targets
    for (auto l_target_cplt : l_perv_functional_vector)
    {
        uint8_t l_attr_chip_unit_pos = 0; //actual value is read in FAPI_ATTR_GET below
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_target_cplt,
                               l_attr_chip_unit_pos));

        if ((l_attr_chip_unit_pos >= 0x20
             && l_attr_chip_unit_pos <= 0x37))/* CoreChiplet */
        {
            l_corechiplets_vector.push_back(l_target_cplt);
        }
    }

    for (auto l_target_cplt : l_corechiplets_vector)
    {
        // Configuring chiplet multicasting registers..
        FAPI_TRY(p9_sbe_chiplet_reset_core_mc_setup(l_target_cplt));
    }

    for (auto l_target_cplt : l_corechiplets_vector)
    {
        // Configuring chiplet multicasting registers..
        FAPI_TRY(p9_sbe_chiplet_reset_all_cplt_net_cntl_setup(l_target_cplt));
    }

    for (auto l_target_cplt : l_perv_functional_vector)
    {
        uint8_t l_attr_chip_unit_pos = 0; //actual value is read in FAPI_ATTR_GET below
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_target_cplt,
                               l_attr_chip_unit_pos));

        if (!((l_attr_chip_unit_pos == 0x07
               || l_attr_chip_unit_pos == 0x08/* McChiplet */) ||
              (l_attr_chip_unit_pos == 0x09 || l_attr_chip_unit_pos == 0x0A
               || l_attr_chip_unit_pos == 0x0B
               || l_attr_chip_unit_pos == 0x0C/* ObusChiplet */) ||
              (l_attr_chip_unit_pos == 0x0D || l_attr_chip_unit_pos == 0x0E
               || l_attr_chip_unit_pos == 0x0F/* PcieChiplet */) ||
              (l_attr_chip_unit_pos == 0x06/* XbusChiplet */)))
        {
            continue;
        }

        // Setting up hang pulse counter for register 0 and register 6
        FAPI_TRY(p9_sbe_chiplet_reset_all_cplt_hang_cnt_setup(l_target_cplt));
    }

    // Get targets
    for (auto l_target_cplt : l_perv_functional_vector)
    {
        uint8_t l_attr_chip_unit_pos = 0; //actual value is read in FAPI_ATTR_GET below
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_target_cplt,
                               l_attr_chip_unit_pos));

        if ((l_attr_chip_unit_pos == 0x02 || l_attr_chip_unit_pos == 0x03
             || l_attr_chip_unit_pos == 0x04
             || l_attr_chip_unit_pos == 0x05))/* NestChiplet */
        {
            l_nestchiplets_vector.push_back(l_target_cplt);
        }
    }

    for (auto l_target_cplt : l_nestchiplets_vector)
    {
        // Setting up hang pulse counter for register 5
        FAPI_TRY(p9_sbe_chiplet_reset_nest_hang_cnt_setup(l_target_cplt));
    }

    for (auto l_target_cplt : l_corechiplets_vector)
    {
        // Setting up hang pulse counter for register 5
        FAPI_TRY(p9_sbe_chiplet_reset_core_hang_cnt_setup(l_target_cplt));
    }

    for (auto l_target_cplt : l_cachechiplets_vector)
    {
        // Setting up hang pulse counter for register 5
        FAPI_TRY(p9_sbe_chiplet_reset_cache_hang_cnt_setup(l_target_cplt));
    }

    for (auto l_target_cplt : l_perv_functional_vector)
    {
        uint8_t l_attr_chip_unit_pos = 0; //actual value is read in FAPI_ATTR_GET below
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_target_cplt,
                               l_attr_chip_unit_pos));

        if (!((l_attr_chip_unit_pos == 0x07
               || l_attr_chip_unit_pos == 0x08/* McChiplet */) ||
              (l_attr_chip_unit_pos == 0x02 || l_attr_chip_unit_pos == 0x03
               || l_attr_chip_unit_pos == 0x04
               || l_attr_chip_unit_pos == 0x05/* NestChiplet */) ||
              (l_attr_chip_unit_pos == 0x09 || l_attr_chip_unit_pos == 0x0A
               || l_attr_chip_unit_pos == 0x0B
               || l_attr_chip_unit_pos == 0x0C/* ObusChiplet */) ||
              (l_attr_chip_unit_pos == 0x0D || l_attr_chip_unit_pos == 0x0E
               || l_attr_chip_unit_pos == 0x0F/* PcieChiplet */) ||
              (l_attr_chip_unit_pos == 0x06/* XbusChiplet */)))
        {
            continue;
        }

        // Setting up partial good fence drop and resetting chiplet.
        FAPI_TRY(p9_sbe_chiplet_reset_pg_fence_setup(l_target_cplt));
    }

    for (auto l_target_cplt : l_nestchiplets_vector)
    {
        FAPI_TRY(p9_sbe_chiplet_reset_opcg_cnfg(l_target_cplt));
    }

    for (auto l_target_cplt : l_perv_functional_vector)
    {
        uint8_t l_attr_chip_unit_pos = 0; //actual value is read in FAPI_ATTR_GET below
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_target_cplt,
                               l_attr_chip_unit_pos));

        if (!(l_attr_chip_unit_pos == 0x07
              || l_attr_chip_unit_pos == 0x08))/* McChiplet */
        {
            continue;
        }

        FAPI_TRY(p9_sbe_chiplet_reset_opcg_cnfg(l_target_cplt, l_mc_sync_mode));
    }

    FAPI_DBG("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Setting up hang pulse counter for all parital good chiplet except for Tp,nest, core and cache
///
/// @param[in]     i_target_cplt   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_all_cplt_hang_cnt_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_cplt)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_DBG("Entering ...");

    //Setting HANG_PULSE_0_REG register value (Setting all fields)
    //HANG_PULSE_0_REG.HANG_PULSE_REG_0 = p9SbeChipletReset::HANG_PULSE_0X10
    l_data64.insertFromRight<0, 6>(p9SbeChipletReset::HANG_PULSE_0X10);
    l_data64.clearBit<6>();  //HANG_PULSE_0_REG.SUPPRESS_HANG_0 = 0
    FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_HANG_PULSE_0_REG, l_data64));

    FAPI_DBG("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Configuring NET control registers into Default required value
///
/// @param[in]     i_target_cplt   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_all_cplt_net_cntl_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_cplt)
{
    FAPI_DBG("Entering ...");

    //Setting NET_CTRL0 register value
    //NET_CTRL0 = p9SbeChipletReset::NET_CNTL0_HW_INIT_VALUE
    FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_NET_CTRL0,
                            p9SbeChipletReset::NET_CNTL0_HW_INIT_VALUE));
    //Setting NET_CTRL1 register value
    //    //NET_CTRL1 = p9SbeChipletReset::NET_CNTL1_HW_INIT_VALUE
    //        FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_NET_CTRL1,
    //                                    p9SbeChipletReset::NET_CNTL1_HW_INIT_VALUE));
    //

    FAPI_DBG("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Setup Cache hang pulse counter configuration...
///
/// @param[in]     i_target_ep   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_cache_hang_cnt_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_ep)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_DBG("Entering ...");

    //Setting HANG_PULSE_0_REG register value (Setting all fields)
    //HANG_PULSE_0_REG.HANG_PULSE_REG_0 = p9SbeChipletReset::HANG_PULSE_0X10
    l_data64.insertFromRight<0, 6>(p9SbeChipletReset::HANG_PULSE_0X10);
    l_data64.clearBit<6>();  //HANG_PULSE_0_REG.SUPPRESS_HANG_0 = 0
    FAPI_TRY(fapi2::putScom(i_target_ep, PERV_HANG_PULSE_0_REG, l_data64));
    //Setting HANG_PULSE_1_REG register value (Setting all fields)
    //HANG_PULSE_1_REG.HANG_PULSE_REG_1 = p9SbeChipletReset::HANG_PULSE_0X01
    l_data64.insertFromRight<0, 6>(p9SbeChipletReset::HANG_PULSE_0X01);
    l_data64.clearBit<6>();  //HANG_PULSE_1_REG.SUPPRESS_HANG_1 = 0
    FAPI_TRY(fapi2::putScom(i_target_ep, PERV_HANG_PULSE_1_REG, l_data64));
    //Setting HANG_PULSE_2_REG register value (Setting all fields)
    //HANG_PULSE_2_REG.HANG_PULSE_REG_2 = p9SbeChipletReset::HANG_PULSE_0X01
    l_data64.insertFromRight<0, 6>(p9SbeChipletReset::HANG_PULSE_0X01);
    l_data64.clearBit<6>();  //HANG_PULSE_2_REG.SUPPRESS_HANG_2 = 0
    FAPI_TRY(fapi2::putScom(i_target_ep, PERV_HANG_PULSE_2_REG, l_data64));
    //Setting HANG_PULSE_3_REG register value (Setting all fields)
    //HANG_PULSE_3_REG.HANG_PULSE_REG_3 = p9SbeChipletReset::HANG_PULSE_0X04
    l_data64.insertFromRight<0, 6>(p9SbeChipletReset::HANG_PULSE_0X04);
    l_data64.clearBit<6>();  //HANG_PULSE_3_REG.SUPPRESS_HANG_3 = 0
    FAPI_TRY(fapi2::putScom(i_target_ep, PERV_HANG_PULSE_3_REG, l_data64));
    //Setting HANG_PULSE_4_REG register value (Setting all fields)
    //HANG_PULSE_4_REG.HANG_PULSE_REG_4 = p9SbeChipletReset::HANG_PULSE_0X00
    l_data64.insertFromRight<0, 6>(p9SbeChipletReset::HANG_PULSE_0X00);
    l_data64.clearBit<6>();  //HANG_PULSE_4_REG.SUPPRESS_HANG_4 = 0
    FAPI_TRY(fapi2::putScom(i_target_ep, PERV_HANG_PULSE_4_REG, l_data64));
    //Setting HANG_PULSE_5_REG register value (Setting all fields)
    //HANG_PULSE_5_REG.HANG_PULSE_REG_5 = p9SbeChipletReset::HANG_PULSE_0X06
    l_data64.insertFromRight<0, 6>(p9SbeChipletReset::HANG_PULSE_0X06);
    l_data64.clearBit<6>();  //HANG_PULSE_5_REG.SUPPRESS_HANG_5 = 0
    FAPI_TRY(fapi2::putScom(i_target_ep, PERV_HANG_PULSE_5_REG, l_data64));

    FAPI_DBG("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Configuring cache chiplet multcast registers based on partial good
///
/// @param[in]     i_target_ep   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_cache_mc_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_ep)
{
    FAPI_DBG("Entering ...");

    //Setting MULTICAST_GROUP_1 register value
    //MULTICAST_GROUP_1 = p9SbeChipletReset::MCGR0_CNFG_SETTINGS
    FAPI_TRY(fapi2::putScom(i_target_ep, PERV_MULTICAST_GROUP_1,
                            p9SbeChipletReset::MCGR0_CNFG_SETTINGS));
    //Setting MULTICAST_GROUP_2 register value
    //MULTICAST_GROUP_2 = p9SbeChipletReset::MCGR3_CNFG_SETTINGS
    FAPI_TRY(fapi2::putScom(i_target_ep, PERV_MULTICAST_GROUP_2,
                            p9SbeChipletReset::MCGR3_CNFG_SETTINGS));
    //Setting MULTICAST_GROUP_3 register value
    //MULTICAST_GROUP_3 = p9SbeChipletReset::MCGR1_CNFG_SETTINGS
    FAPI_TRY(fapi2::putScom(i_target_ep, PERV_MULTICAST_GROUP_3,
                            p9SbeChipletReset::MCGR1_CNFG_SETTINGS));

    FAPI_DBG("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Setup core hang pulse counter configuration...
///
/// @param[in]     i_target_ec   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_core_hang_cnt_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_ec)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_DBG("Entering ...");

    //Setting HANG_PULSE_0_REG register value (Setting all fields)
    //HANG_PULSE_0_REG.HANG_PULSE_REG_0 = p9SbeChipletReset::HANG_PULSE_0X10
    l_data64.insertFromRight<0, 6>(p9SbeChipletReset::HANG_PULSE_0X10);
    l_data64.clearBit<6>();  //HANG_PULSE_0_REG.SUPPRESS_HANG_0 = 0
    FAPI_TRY(fapi2::putScom(i_target_ec, PERV_HANG_PULSE_0_REG, l_data64));
    //Setting HANG_PULSE_1_REG register value (Setting all fields)
    //HANG_PULSE_1_REG.HANG_PULSE_REG_1 = p9SbeChipletReset::HANG_PULSE_0X1A
    l_data64.insertFromRight<0, 6>(p9SbeChipletReset::HANG_PULSE_0X1A);
    l_data64.clearBit<6>();  //HANG_PULSE_1_REG.SUPPRESS_HANG_1 = 0
    FAPI_TRY(fapi2::putScom(i_target_ec, PERV_HANG_PULSE_1_REG, l_data64));
    //Setting HANG_PULSE_5_REG register value (Setting all fields)
    //HANG_PULSE_5_REG.HANG_PULSE_REG_5 = p9SbeChipletReset::HANG_PULSE_0X06
    l_data64.insertFromRight<0, 6>(p9SbeChipletReset::HANG_PULSE_0X06);
    l_data64.clearBit<6>();  //HANG_PULSE_5_REG.SUPPRESS_HANG_5 = 0
    FAPI_TRY(fapi2::putScom(i_target_ec, PERV_HANG_PULSE_5_REG, l_data64));

    FAPI_DBG("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Configuring core chiplet multcast registers based on partial good
///
/// @param[in]     i_target_ec   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_core_mc_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_ec)
{
    FAPI_DBG("Entering ...");

    //Setting MULTICAST_GROUP_1 register value
    //MULTICAST_GROUP_1 = p9SbeChipletReset::MCGR0_CNFG_SETTINGS
    FAPI_TRY(fapi2::putScom(i_target_ec, PERV_MULTICAST_GROUP_1,
                            p9SbeChipletReset::MCGR0_CNFG_SETTINGS));
    //Setting MULTICAST_GROUP_2 register value
    //MULTICAST_GROUP_2 = p9SbeChipletReset::MCGR3_CNFG_SETTINGS
    FAPI_TRY(fapi2::putScom(i_target_ec, PERV_MULTICAST_GROUP_2,
                            p9SbeChipletReset::MCGR3_CNFG_SETTINGS));
    //Setting MULTICAST_GROUP_3 register value
    //MULTICAST_GROUP_3 = p9SbeChipletReset::MCGR1_CNFG_SETTINGS
    FAPI_TRY(fapi2::putScom(i_target_ec, PERV_MULTICAST_GROUP_3,
                            p9SbeChipletReset::MCGR1_CNFG_SETTINGS));
    //Setting MULTICAST_GROUP_4 register value
    //MULTICAST_GROUP_4 = p9SbeChipletReset::MCGR2_CNFG_SETTINGS
    FAPI_TRY(fapi2::putScom(i_target_ec, PERV_MULTICAST_GROUP_4,
                            p9SbeChipletReset::MCGR2_CNFG_SETTINGS));

    FAPI_DBG("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Setting up hang pulse counter for partial good Nest chiplet
///
/// @param[in]     i_target_cplt   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_nest_hang_cnt_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_cplt)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_DBG("Entering ...");

    // Local variables
    //
    uint8_t   l_attr_chipunit_pos;
    const uint8_t N0 = 0x02;
    const uint8_t N1 = 0x03;
    const uint8_t N3 = 0x05;

    // Collecting partial good and chiplet unit position attribute
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, i_target_cplt,
                           l_attr_chipunit_pos));

    //Setting HANG_PULSE_0_REG register value (Setting all fields)
    //HANG_PULSE_0_REG.HANG_PULSE_REG_0 = p9SbeChipletReset::HANG_PULSE_0X10
    l_data64.insertFromRight<0, 6>(p9SbeChipletReset::HANG_PULSE_0X10);
    l_data64.clearBit<6>();  //HANG_PULSE_0_REG.SUPPRESS_HANG_0 = 0
    FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_HANG_PULSE_0_REG, l_data64));
    //Setting HANG_PULSE_5_REG register value (Setting all fields)
    //HANG_PULSE_5_REG.HANG_PULSE_REG_5 = p9SbeChipletReset::HANG_PULSE_0X06
    l_data64.insertFromRight<0, 6>(p9SbeChipletReset::HANG_PULSE_0X06);
    l_data64.clearBit<6>();  //HANG_PULSE_5_REG.SUPPRESS_HANG_5 = 0
    FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_HANG_PULSE_5_REG, l_data64));

    if ( l_attr_chipunit_pos == N0 )
    {
        //Setting HANG_PULSE_1_REG register value (Setting all fields)
        //HANG_PULSE_1_REG.HANG_PULSE_REG_1 = p9SbeChipletReset::HANG_PULSE_0X10
        l_data64.insertFromRight<0, 6>(p9SbeChipletReset::HANG_PULSE_0X10);
        l_data64.clearBit<6>();  //HANG_PULSE_1_REG.SUPPRESS_HANG_1 = 0
        FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_HANG_PULSE_1_REG, l_data64));
        //Setting HANG_PULSE_2_REG register value (Setting all fields)
        //HANG_PULSE_2_REG.HANG_PULSE_REG_2 = p9SbeChipletReset::HANG_PULSE_0X22
        l_data64.insertFromRight<0, 6>(p9SbeChipletReset::HANG_PULSE_0X22);
        l_data64.clearBit<6>();  //HANG_PULSE_2_REG.SUPPRESS_HANG_2 = 0
        FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_HANG_PULSE_2_REG, l_data64));
    }

    if ( l_attr_chipunit_pos == N1 )
    {
        //Setting HANG_PULSE_2_REG register value (Setting all fields)
        //HANG_PULSE_2_REG.HANG_PULSE_REG_2 = p9SbeChipletReset::HANG_PULSE_0X0F
        l_data64.insertFromRight<0, 6>(p9SbeChipletReset::HANG_PULSE_0X0F);
        l_data64.clearBit<6>();  //HANG_PULSE_2_REG.SUPPRESS_HANG_2 = 0
        FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_HANG_PULSE_2_REG, l_data64));
    }

    if ( l_attr_chipunit_pos == N3 )
    {
        //Setting HANG_PULSE_1_REG register value (Setting all fields)
        //HANG_PULSE_1_REG.HANG_PULSE_REG_1 = p9SbeChipletReset::HANG_PULSE_0X17
        l_data64.insertFromRight<0, 6>(p9SbeChipletReset::HANG_PULSE_0X17);
        l_data64.clearBit<6>();  //HANG_PULSE_1_REG.SUPPRESS_HANG_1 = 0
        FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_HANG_PULSE_1_REG, l_data64));
        //Setting HANG_PULSE_2_REG register value (Setting all fields)
        //HANG_PULSE_2_REG.HANG_PULSE_REG_2 = p9SbeChipletReset::HANG_PULSE_0X13
        l_data64.insertFromRight<0, 6>(p9SbeChipletReset::HANG_PULSE_0X13);
        l_data64.clearBit<6>();  //HANG_PULSE_2_REG.SUPPRESS_HANG_2 = 0
        FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_HANG_PULSE_2_REG, l_data64));
        //Setting HANG_PULSE_3_REG register value (Setting all fields)
        //HANG_PULSE_3_REG.HANG_PULSE_REG_3 = p9SbeChipletReset::HANG_PULSE_0X0F
        l_data64.insertFromRight<0, 6>(p9SbeChipletReset::HANG_PULSE_0X0F);
        l_data64.clearBit<6>();  //HANG_PULSE_3_REG.SUPPRESS_HANG_3 = 0
        FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_HANG_PULSE_3_REG, l_data64));
    }

    FAPI_DBG("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Calling nest/mc/other bus multicasting set up
///
/// @param[in]     i_target_cplt   Reference to TARGET_TYPE_PROC_CHIP target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_nest_mc_call(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_cplt)
{
    auto l_perv_functional_vector =
        i_target_cplt.getChildren<fapi2::TARGET_TYPE_PERV>
        (fapi2::TARGET_STATE_FUNCTIONAL);
    FAPI_DBG("Entering ...");

    for (auto l_target_cplt : l_perv_functional_vector)
    {
        uint8_t l_attr_chip_unit_pos = 0; //actual value is read in FAPI_ATTR_GET below
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_target_cplt,
                               l_attr_chip_unit_pos));

        if (!((l_attr_chip_unit_pos == 0x07
               || l_attr_chip_unit_pos == 0x08/* McChiplet */) ||
              (l_attr_chip_unit_pos == 0x02 || l_attr_chip_unit_pos == 0x03
               || l_attr_chip_unit_pos == 0x04
               || l_attr_chip_unit_pos == 0x05/* NestChiplet */) ||
              (l_attr_chip_unit_pos == 0x09 || l_attr_chip_unit_pos == 0x0A
               || l_attr_chip_unit_pos == 0x0B
               || l_attr_chip_unit_pos == 0x0C/* ObusChiplet */) ||
              (l_attr_chip_unit_pos == 0x0D || l_attr_chip_unit_pos == 0x0E
               || l_attr_chip_unit_pos == 0x0F/* PcieChiplet */) ||
              (l_attr_chip_unit_pos == 0x06/* XbusChiplet */)))
        {
            continue;
        }

        FAPI_TRY(p9_sbe_chiplet_reset_nest_mc_setup(l_target_cplt));
    }

    FAPI_DBG("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Configuring nest chiplet multcast registers based on partial good
///
/// @param[in]     i_target_cplt   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_nest_mc_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_cplt)
{
    FAPI_DBG("Entering ...");

    //Setting MULTICAST_GROUP_1 register value
    //MULTICAST_GROUP_1 = p9SbeChipletReset::MCGR0_CNFG_SETTINGS
    FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_MULTICAST_GROUP_1,
                            p9SbeChipletReset::MCGR0_CNFG_SETTINGS));
    //Setting MULTICAST_GROUP_2 register value
    //MULTICAST_GROUP_2 = p9SbeChipletReset::MCGR3_CNFG_SETTINGS
    FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_MULTICAST_GROUP_2,
                            p9SbeChipletReset::MCGR3_CNFG_SETTINGS));

    FAPI_DBG("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief configuring nest chiplet OPCG registers..
///
/// @param[in]     i_target_clpt   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_opcg_cnfg(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_clpt)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_DBG("Entering ...");

    // INOP_ALIGN is 8:1 ratio
    // INOP_WAIT is zero
    // OPCG_WAIT is 0x020
    // SCAN_RATIO is 4:1
    //Setting OPCG_ALIGN register value
    l_data64 =
        p9SbeChipletReset::OPCG_ALIGN_SETTING;  //OPCG_ALIGN = p9SbeChipletReset::OPCG_ALIGN_SETTING
    //OPCG_ALIGN.INOP_ALIGN = p9SbeChipletReset::INOP_ALIGN_SETTING_0X9
    l_data64.insertFromRight<0, 4>(p9SbeChipletReset::INOP_ALIGN_SETTING_0X9);
    l_data64.clearBit<12, 8>();  //OPCG_ALIGN.INOP_WAIT = 0
    //OPCG_ALIGN.OPCG_WAIT_CYCLES = p9SbeChipletReset::OPCG_WAIT_CYCLE_0X020
    l_data64.insertFromRight<52, 12>(p9SbeChipletReset::OPCG_WAIT_CYCLE_0X020);
    //OPCG_ALIGN.SCAN_RATIO = p9SbeChipletReset::SCAN_RATIO_0X3
    l_data64.insertFromRight<47, 5>(p9SbeChipletReset::SCAN_RATIO_0X3);
    FAPI_TRY(fapi2::putScom(i_target_clpt, PERV_OPCG_ALIGN, l_data64));

    // Enable listen_to_sync mode
    //Setting SYNC_CONFIG register value
    //SYNC_CONFIG = p9SbeChipletReset::SYNC_CONFIG_DEFAULT
    FAPI_TRY(fapi2::putScom(i_target_clpt, PERV_SYNC_CONFIG,
                            p9SbeChipletReset::SYNC_CONFIG_DEFAULT));

    FAPI_DBG("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief configuring MC chiplet OPCG registers
///
/// @param[in]     i_target_cplt         Reference to TARGET_TYPE_PERV target
/// @param[in]     i_attr_mc_sync_mode   Collecting MC sync mode attribute
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_opcg_cnfg(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_cplt,
    const uint8_t i_attr_mc_sync_mode)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_DBG("Entering ...");

    if ( i_attr_mc_sync_mode  )
    {
        // INOP_ALIGN is 8:1 ratio
        // INOP_WAIT is zero
        // OPCG_WAIT is 0x020
        // SCAN_RATIO is 4:1
        //Setting OPCG_ALIGN register value
        l_data64 =
            p9SbeChipletReset::OPCG_ALIGN_SETTING;  //OPCG_ALIGN = p9SbeChipletReset::OPCG_ALIGN_SETTING
        //OPCG_ALIGN.INOP_ALIGN = p9SbeChipletReset::INOP_ALIGN_SETTING_0X9
        l_data64.insertFromRight<0, 4>(p9SbeChipletReset::INOP_ALIGN_SETTING_0X9);
        l_data64.clearBit<12, 8>();  //OPCG_ALIGN.INOP_WAIT = 0
        //OPCG_ALIGN.OPCG_WAIT_CYCLES = p9SbeChipletReset::OPCG_WAIT_CYCLE_0X020
        l_data64.insertFromRight<52, 12>(p9SbeChipletReset::OPCG_WAIT_CYCLE_0X020);
        //OPCG_ALIGN.SCAN_RATIO = p9SbeChipletReset::SCAN_RATIO_0X3
        l_data64.insertFromRight<47, 5>(p9SbeChipletReset::SCAN_RATIO_0X3);
        FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_OPCG_ALIGN, l_data64));

        // Enable listen_to_sync mode
        //Setting SYNC_CONFIG register value
        //SYNC_CONFIG = p9SbeChipletReset::SYNC_CONFIG_DEFAULT
        FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_SYNC_CONFIG,
                                p9SbeChipletReset::SYNC_CONFIG_DEFAULT));
    }

    FAPI_DBG("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Dorping fence on Partial good chiplet and resetting it.
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_pg_fence_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_DBG("Entering ...");

    // Local variable and constant definition
    fapi2::buffer <uint32_t> l_attr_pg;
    const uint64_t ERROR_DEFAULT_VALUE = 0xFFFFFFFFFFFFFFFFull;

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG, i_target_chiplet, l_attr_pg));

    // EP Reset all chiplet with in multicasting group
    //Setting NET_CTRL0 register value
    l_data64.flush<0>();
    l_data64.setBit<0>();  //NET_CTRL0.CHIPLET_ENABLE = 0b1
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL0_WOR, l_data64));

    //Setting ERROR_REG register value
    //ERROR_REG = ERROR_DEFAULT_VALUE
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_ERROR_REG, ERROR_DEFAULT_VALUE));

    // EP Reset all chiplet with in multicasting group
    //Setting NET_CTRL0 register value
    l_data64.flush<1>();
    l_data64.clearBit<1>();  //NET_CTRL0.PCB_EP_RESET = 0b0
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL0_WAND, l_data64));

    // Not needed as have only nest chiplet (no dual clock controller) Bit 62 ->0
    //
    //Setting CPLT_CTRL1 register value
    FAPI_TRY(fapi2::getScom(i_target_chiplet, PERV_CPLT_CTRL1, l_data64));
    //CPLT_CTRL1.TC_REGION2_FENCE = l_attr_pg.getBit<21>()
    l_data64.writeBit<6>(l_attr_pg.getBit<21>());
    //CPLT_CTRL1.TC_REGION3_FENCE = l_attr_pg.getBit<22>()
    l_data64.writeBit<7>(l_attr_pg.getBit<22>());
    //CPLT_CTRL1.TC_REGION4_FENCE = l_attr_pg.getBit<23>()
    l_data64.writeBit<8>(l_attr_pg.getBit<23>());
    //CPLT_CTRL1.TC_REGION5_FENCE = l_attr_pg.getBit<24>()
    l_data64.writeBit<9>(l_attr_pg.getBit<24>());
    //CPLT_CTRL1.TC_REGION6_FENCE = l_attr_pg.getBit<25>()
    l_data64.writeBit<10>(l_attr_pg.getBit<25>());
    //CPLT_CTRL1.TC_REGION7_FENCE = l_attr_pg.getBit<26>()
    l_data64.writeBit<11>(l_attr_pg.getBit<26>());
    //CPLT_CTRL1.UNUSED_12B = l_attr_pg.getBit<27>()
    l_data64.writeBit<12>(l_attr_pg.getBit<27>());
    //CPLT_CTRL1.UNUSED_13B = l_attr_pg.getBit<28>()
    l_data64.writeBit<13>(l_attr_pg.getBit<28>());
    //CPLT_CTRL1.UNUSED_14B = l_attr_pg.getBit<29>()
    l_data64.writeBit<14>(l_attr_pg.getBit<29>());
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_CPLT_CTRL1, l_data64));

    FAPI_DBG("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

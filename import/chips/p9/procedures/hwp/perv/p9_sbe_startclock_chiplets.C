/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: chips/p9/procedures/hwp/perv/p9_sbe_startclock_chiplets.C $   */
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
//------------------------------------------------------------------------------
/// @file  p9_sbe_startclock_chiplets.C
///
/// @brief Start clock procedure for XBUS, OBUS, PCIe
//------------------------------------------------------------------------------
// *HWP HW Owner        : Anusha Reddy Rangareddygari <anusrang@in.ibm.com>
// *HWP HW Backup Owner : Srinivas V Naga <srinivan@in.ibm.com>
// *HWP FW Owner        : sunil kumar <skumar8j@in.ibm.com>
// *HWP Team            : Perv
// *HWP Level           : 2
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------


//## auto_generated
#include "p9_sbe_startclock_chiplets.H"
//## auto_generated
#include "p9_const_common.H"

#include <p9_perv_scom_addresses.H>
#include <p9_perv_scom_addresses_fld.H>
#include <p9_perv_sbe_cmn.H>
#include <p9_sbe_common.H>


enum P9_SBE_STARTCLOCK_CHIPLETS_Private_Constants
{
    DONT_STARTMASTER = 0x0,
    DONT_STARTSLAVE = 0x0,
    CLOCK_CMD = 0x1,
    CLOCK_TYPES = 0x7,
    REGIONS_ALL_EXCEPT_VITAL_NESTPLL = 0x7FE
};

static fapi2::ReturnCode p9_sbe_startclock_chiplets_check_checkstop_function(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet);

static fapi2::ReturnCode p9_sbe_startclock_chiplets_cplt_ctrl_action_function(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet);

static fapi2::ReturnCode p9_sbe_startclock_chiplets_get_pg_vector(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chip,
    fapi2::buffer<uint64_t>& o_pg_vector);

static fapi2::ReturnCode p9_sbe_startclock_chiplets_ob_fence_drop(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chip,
    const fapi2::buffer<uint64_t> i_pg_vector);

static fapi2::ReturnCode p9_sbe_startclock_chiplets_pci_fence_drop(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chip,
    const fapi2::buffer<uint64_t> i_pg_vector);

static fapi2::ReturnCode p9_sbe_startclock_chiplets_sync_config(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet);

static fapi2::ReturnCode p9_sbe_startclock_chiplets_xb_fence_drop(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chip,
    const fapi2::buffer<uint64_t> i_pg_vector);

fapi2::ReturnCode p9_sbe_startclock_chiplets(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chiplets)
{
    fapi2::buffer<uint64_t> l_pg_vector;
    fapi2::buffer<uint64_t> l_regions;
    FAPI_INF("Entering ...");

    for (auto l_target_cplt :
         i_target_chiplets.getChildren<fapi2::TARGET_TYPE_PERV>
         (static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_ALL_NEST |
                                           fapi2::TARGET_FILTER_TP), fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_TRY(p9_sbe_startclock_chiplets_get_pg_vector(l_target_cplt, l_pg_vector));
        FAPI_DBG("partial good targets vector: %#018lX", l_pg_vector);
    }

    for (auto l_trgt_chplt : i_target_chiplets.getChildren<fapi2::TARGET_TYPE_PERV>
         (static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_ALL_OBUS |
                                           fapi2::TARGET_FILTER_ALL_PCI | fapi2::TARGET_FILTER_XBUS),
          fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_DBG("Call p9_sbe_startclock_chiplets_cplt_ctrl_action_function for xbus, obus, pcie chiplets");
        FAPI_TRY(p9_sbe_startclock_chiplets_cplt_ctrl_action_function(l_trgt_chplt));

        FAPI_DBG("Disable listen to sync for all non-master/slave chiplets");
        FAPI_TRY(p9_sbe_startclock_chiplets_sync_config(l_trgt_chplt));

        FAPI_DBG("call module align chiplets for xbus, obus, pcie chiplets");
        FAPI_TRY(p9_sbe_common_align_chiplets(l_trgt_chplt));

        FAPI_DBG("Region setup ");
        FAPI_TRY(p9_perv_sbe_cmn_regions_setup_64(l_trgt_chplt,
                 REGIONS_ALL_EXCEPT_VITAL_NESTPLL, l_regions));
        FAPI_DBG("Regions value: %#018lX", l_regions);

        FAPI_DBG("Call module clock start stop for xbus, obus, pcie chiplets");
        FAPI_TRY(p9_sbe_common_clock_start_stop(l_trgt_chplt, CLOCK_CMD,
                                                DONT_STARTSLAVE, DONT_STARTMASTER, l_regions, CLOCK_TYPES));
    }

    for (auto l_trgt_chplt : i_target_chiplets.getChildren<fapi2::TARGET_TYPE_PERV>
         (fapi2::TARGET_FILTER_XBUS, fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_DBG("Drop chiplet fence for Xbus");
        FAPI_TRY(p9_sbe_startclock_chiplets_xb_fence_drop(l_trgt_chplt, l_pg_vector));
    }

    for (auto l_trgt_chplt : i_target_chiplets.getChildren<fapi2::TARGET_TYPE_PERV>
         (fapi2::TARGET_FILTER_ALL_OBUS, fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_DBG("Drop Chiplet fence for Obus");
        FAPI_TRY(p9_sbe_startclock_chiplets_ob_fence_drop(l_trgt_chplt, l_pg_vector));
    }

    for (auto l_trgt_chplt : i_target_chiplets.getChildren<fapi2::TARGET_TYPE_PERV>
         (fapi2::TARGET_FILTER_ALL_PCI, fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_DBG("Drop chiplet fence for PCIe");
        FAPI_TRY(p9_sbe_startclock_chiplets_pci_fence_drop(l_trgt_chplt, l_pg_vector));
    }

    for (auto l_trgt_chplt : i_target_chiplets.getChildren<fapi2::TARGET_TYPE_PERV>
         (static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_ALL_OBUS |
                                           fapi2::TARGET_FILTER_ALL_PCI | fapi2::TARGET_FILTER_XBUS),
          fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_DBG("call sbe_startclock_chiplets_check_checkstop_function for xbus, obus, pcie chiplets");
        FAPI_TRY(p9_sbe_startclock_chiplets_check_checkstop_function(l_trgt_chplt));
    }

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief --drop chiplet fence
/// --check checkstop register
/// --clear flush inhibit to go into flush mode
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_startclock_chiplets_check_checkstop_function(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet)
{
    fapi2::buffer<uint64_t> l_read_reg;
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("Entering ...");

    FAPI_DBG("Check checkstop register");
    //Getting XFIR register value
    FAPI_TRY(fapi2::getScom(i_target_chiplet, PERV_XFIR,
                            l_read_reg)); //l_read_reg = XFIR

    FAPI_ASSERT(l_read_reg == 0,
                fapi2::READ_CHECKSTOP_ERR()
                .set_READ_CHECKSTOP(l_read_reg),
                "ERROR: COMBINE ALL CHECKSTOP ERROR");

    FAPI_DBG("Clear flush_inhibit to go in to flush mode");
    //Setting CPLT_CTRL0 register value
    l_data64.flush<0>();
    //CPLT_CTRL0.CTRL_CC_FLUSHMODE_INH_DC = 0
    l_data64.setBit<PERV_1_CPLT_CTRL0_CTRL_CC_FLUSHMODE_INH_DC>();
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_CPLT_CTRL0_CLEAR, l_data64));

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief --drop vital fence
/// --reset abstclk muxsel
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_startclock_chiplets_cplt_ctrl_action_function(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet)
{
    // Local variable and constant definition
    fapi2::buffer <uint32_t> l_attr_pg;
    fapi2::buffer <uint16_t> l_cplt_ctrl_init;
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("Entering ...");

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG, i_target_chiplet, l_attr_pg));

    l_attr_pg.invert();
    l_attr_pg.extractToRight<20, 11>(l_cplt_ctrl_init);

    // Not needed as have only nest chiplet (no dual clock controller) Bit 62 ->0
    //
    FAPI_DBG("Drop partial good fences");
    //Setting CPLT_CTRL1 register value
    l_data64.flush<0>();
    l_data64.writeBit<PERV_1_CPLT_CTRL1_TC_VITL_REGION_FENCE>
    (l_attr_pg.getBit<19>());  //CPLT_CTRL1.TC_VITL_REGION_FENCE = l_attr_pg.getBit<19>()
    //CPLT_CTRL1.TC_ALL_REGIONS_FENCE = l_cplt_ctrl_init
    l_data64.insertFromRight<4, 11>(l_cplt_ctrl_init);
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_CPLT_CTRL1_CLEAR, l_data64));

    FAPI_DBG("reset abistclk_muxsel and syncclk_muxsel");
    //Setting CPLT_CTRL0 register value
    l_data64.flush<0>();
    //CPLT_CTRL0.CTRL_CC_ABSTCLK_MUXSEL_DC = 1
    l_data64.setBit<PERV_1_CPLT_CTRL0_CTRL_CC_ABSTCLK_MUXSEL_DC>();
    //CPLT_CTRL0.TC_UNIT_SYNCCLK_MUXSEL_DC = 1
    l_data64.setBit<PERV_1_CPLT_CTRL0_TC_UNIT_SYNCCLK_MUXSEL_DC>();
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_CPLT_CTRL0_CLEAR, l_data64));

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief get children for all chiplets : Perv, Nest, XB, MC, OB, PCIe
///
/// @param[in]     i_target_chip   Reference to TARGET_TYPE_PERV target
/// @param[out]    o_pg_vector     vector of targets
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_startclock_chiplets_get_pg_vector(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chip,
    fapi2::buffer<uint64_t>& o_pg_vector)
{
    fapi2::buffer<uint8_t> l_read_attrunitpos;
    FAPI_INF("Entering ...");

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, i_target_chip,
                           l_read_attrunitpos));

    if (    l_read_attrunitpos == 0x01 )
    {
        o_pg_vector.setBit<0>();
    }

    if ( l_read_attrunitpos == 0x02 )
    {
        o_pg_vector.setBit<1>();
    }

    if (    l_read_attrunitpos == 0x03 )
    {
        o_pg_vector.setBit<2>();
    }

    if (    l_read_attrunitpos == 0x04 )
    {
        o_pg_vector.setBit<3>();
    }

    if (    l_read_attrunitpos == 0x05 )
    {
        o_pg_vector.setBit<4>();
    }

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Drop chiplet fence for OB chiplet
///
/// @param[in]     i_target_chip   Reference to TARGET_TYPE_PERV target
/// @param[in]     i_pg_vector     Pg vector of targets
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_startclock_chiplets_ob_fence_drop(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chip,
    const fapi2::buffer<uint64_t> i_pg_vector)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("Entering ...");

    FAPI_DBG("Drop chiplet fence");

    //Setting NET_CTRL0 register value
    if (i_pg_vector.getBit<2>() == 1)
    {
        l_data64.flush<1>();
        //NET_CTRL0.FENCE_EN = (i_pg_vector.getBit<2>() == 1) ? 0
        l_data64.clearBit<PERV_1_NET_CTRL0_FENCE_EN>();
        FAPI_TRY(fapi2::putScom(i_target_chip, PERV_NET_CTRL0_WAND, l_data64));
    }

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Drop chiplet fence for pcie chiplet
///
/// @param[in]     i_target_chip   Reference to TARGET_TYPE_PERV target
/// @param[in]     i_pg_vector     Pg vector of targets
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_startclock_chiplets_pci_fence_drop(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chip,
    const fapi2::buffer<uint64_t> i_pg_vector)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("Entering ...");

    FAPI_DBG("Drop chiplet fence");

    //Setting NET_CTRL0 register value
    if (i_pg_vector.getBit<3>() == 1)
    {
        l_data64.flush<1>();
        //NET_CTRL0.FENCE_EN = (i_pg_vector.getBit<3>() == 1) ? 0
        l_data64.clearBit<PERV_1_NET_CTRL0_FENCE_EN>();
        FAPI_TRY(fapi2::putScom(i_target_chip, PERV_NET_CTRL0_WAND, l_data64));
    }

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Disable listen to sync for all non-master / slave chiplets
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_startclock_chiplets_sync_config(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("Entering ...");

    //Setting SYNC_CONFIG register value
    FAPI_TRY(fapi2::getScom(i_target_chiplet, PERV_SYNC_CONFIG, l_data64));
    l_data64.setBit<4>();  //SYNC_CONFIG.LISTEN_TO_SYNC_PULSE_DIS = 0b1
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_SYNC_CONFIG, l_data64));

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Drop chiplet fence for XB chiplet
///
/// @param[in]     i_target_chip   Reference to TARGET_TYPE_PERV target
/// @param[in]     i_pg_vector     vector of targets
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_startclock_chiplets_xb_fence_drop(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chip,
    const fapi2::buffer<uint64_t> i_pg_vector)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("Entering ...");

    FAPI_DBG("Drop chiplet fence");

    //Setting NET_CTRL0 register value
    if (i_pg_vector.getBit<1>() == 1)
    {
        l_data64.flush<1>();
        //NET_CTRL0.FENCE_EN = (i_pg_vector.getBit<1>() == 1) ? 0
        l_data64.clearBit<PERV_1_NET_CTRL0_FENCE_EN>();
        FAPI_TRY(fapi2::putScom(i_target_chip, PERV_NET_CTRL0_WAND, l_data64));
    }

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

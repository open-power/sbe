/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: chips/p9/procedures/hwp/perv/p9_sbe_chiplet_reset.C $         */
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

#include <p9_perv_scom_addresses.H>
#include <p9_perv_scom_addresses_fld.H>
#include <p9_perv_sbe_cmn.H>


static fapi2::ReturnCode p9_sbe_chiplet_reset_all_cplt_hang_cnt_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_cplt);

static fapi2::ReturnCode p9_sbe_chiplet_reset_all_cplt_net_cntl_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_cplt);

static fapi2::ReturnCode p9_sbe_chiplet_reset_cache_hang_cnt_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_ep);

static fapi2::ReturnCode p9_sbe_chiplet_reset_clk_mux_MC(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet,
    const fapi2::buffer<uint32_t> i_clk_mux_value);

static fapi2::ReturnCode p9_sbe_chiplet_reset_clk_mux_call(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chiplet);

static fapi2::ReturnCode p9_sbe_chiplet_reset_clk_mux_obus(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet,
    const fapi2::buffer<uint32_t> i_clk_mux_value);

static fapi2::ReturnCode p9_sbe_chiplet_reset_clk_mux_pcie(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet,
    const fapi2::buffer<uint32_t> i_clk_mux_value);

static fapi2::ReturnCode p9_sbe_chiplet_reset_clk_mux_xbus(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet,
    const fapi2::buffer<uint32_t> i_clk_mux_value);

static fapi2::ReturnCode p9_sbe_chiplet_reset_core_hang_cnt_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_ec);

static fapi2::ReturnCode p9_sbe_chiplet_reset_disable_listen_to_sync(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet);

static fapi2::ReturnCode p9_sbe_chiplet_reset_div_clk_bypass(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet);

static fapi2::ReturnCode p9_sbe_chiplet_reset_enable_listen_to_sync(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet);

static fapi2::ReturnCode p9_sbe_chiplet_reset_enable_pll(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet);

static fapi2::ReturnCode p9_sbe_chiplet_reset_mc_net_ctrl_clk_async_reset(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet);

static fapi2::ReturnCode p9_sbe_chiplet_reset_mc_setup(const
        fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet);

static fapi2::ReturnCode p9_sbe_chiplet_reset_mc_setup_MC(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet);

static fapi2::ReturnCode p9_sbe_chiplet_reset_mc_setup_cache(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet);

static fapi2::ReturnCode p9_sbe_chiplet_reset_mc_setup_core(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet);

static fapi2::ReturnCode p9_sbe_chiplet_reset_nest_hang_cnt_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_cplt);

static fapi2::ReturnCode p9_sbe_chiplet_reset_nest_mc_call(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_cplt);

static fapi2::ReturnCode
p9_sbe_chiplet_reset_net_ctrl_lvltrans_fence_pcb_ep_reset(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet);

static fapi2::ReturnCode p9_sbe_chiplet_reset_opcg_cnfg(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet);

static fapi2::ReturnCode p9_sbe_chiplet_reset_opcg_cnfg_scan_ratio(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_cplt);

static fapi2::ReturnCode p9_sbe_chiplet_reset_scan0_call(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chip);

static fapi2::ReturnCode p9_sbe_chiplet_reset_setup(const
        fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet);

fapi2::ReturnCode p9_sbe_chiplet_reset(const
                                       fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    // Local variable
    //uint8_t l_mc_sync_mode = 0;
    FAPI_INF("Entering ...");

    // Configuring chiplet multicasting registers..
    FAPI_TRY(p9_sbe_chiplet_reset_nest_mc_call(i_target_chip));

    for (auto l_target_cplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (fapi2::TARGET_FILTER_ALL_MC, fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_DBG("Configuring multicast registers for MC01,MC23");
        FAPI_TRY(p9_sbe_chiplet_reset_mc_setup_MC(l_target_cplt));
    }

    for (auto l_target_cplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (fapi2::TARGET_FILTER_ALL_CACHES, fapi2::TARGET_STATE_FUNCTIONAL))
    {
        // Configuring chiplet multicasting registers..
        FAPI_DBG("Configuring cache chiplet multicasting registers");
        FAPI_TRY(p9_sbe_chiplet_reset_mc_setup_cache(l_target_cplt));
    }

    for (auto l_target_cplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (fapi2::TARGET_FILTER_ALL_CORES, fapi2::TARGET_STATE_FUNCTIONAL))
    {
        // Configuring chiplet multicasting registers..
        FAPI_DBG("Configuring core chiplet multicasting registers");
        FAPI_TRY(p9_sbe_chiplet_reset_mc_setup_core(l_target_cplt));
    }

    for (auto l_target_cplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_ALL_CACHES |
                                           fapi2::TARGET_FILTER_ALL_CORES | fapi2::TARGET_FILTER_ALL_MC |
                                           fapi2::TARGET_FILTER_ALL_NEST | fapi2::TARGET_FILTER_ALL_OBUS |
                                           fapi2::TARGET_FILTER_ALL_PCI | fapi2::TARGET_FILTER_XBUS),
          fapi2::TARGET_STATE_FUNCTIONAL))
    {
        // Configuring NET control registers into Default required value
        FAPI_DBG("Restore NET_CTRL0&1 init value - for all chiplets except TP");
        FAPI_TRY(p9_sbe_chiplet_reset_all_cplt_net_cntl_setup(l_target_cplt));
    }

    for (auto l_target_cplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_ALL_MC |
                                           fapi2::TARGET_FILTER_ALL_OBUS | fapi2::TARGET_FILTER_ALL_PCI |
                                           fapi2::TARGET_FILTER_XBUS), fapi2::TARGET_STATE_FUNCTIONAL))
    {
        // Setting up hang pulse counter for register 0 and register 6
        FAPI_DBG("Setup hang pulse counter for Mc,Xbus,Obus,Pcie");
        FAPI_TRY(p9_sbe_chiplet_reset_all_cplt_hang_cnt_setup(l_target_cplt));
    }

    for (auto l_target_cplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (fapi2::TARGET_FILTER_ALL_NEST, fapi2::TARGET_STATE_FUNCTIONAL))
    {
        // Setting up hang pulse counter for register 5
        FAPI_DBG("Setup hang pulse counter for nest chiplet");
        FAPI_TRY(p9_sbe_chiplet_reset_nest_hang_cnt_setup(l_target_cplt));
    }

    for (auto l_target_cplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (fapi2::TARGET_FILTER_ALL_CORES, fapi2::TARGET_STATE_FUNCTIONAL))
    {
        // Setting up hang pulse counter for register 5
        FAPI_DBG("Setup hang pulse counter for core chiplet");
        FAPI_TRY(p9_sbe_chiplet_reset_core_hang_cnt_setup(l_target_cplt));
    }

    for (auto l_target_cplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (fapi2::TARGET_FILTER_ALL_CACHES, fapi2::TARGET_STATE_FUNCTIONAL))
    {
        // Setting up hang pulse counter for register 5
        FAPI_DBG("Setup hang pulse counter for cache chiplet");
        FAPI_TRY(p9_sbe_chiplet_reset_cache_hang_cnt_setup(l_target_cplt));
    }

    FAPI_DBG("Clock mux settings");
    FAPI_TRY(p9_sbe_chiplet_reset_clk_mux_call(i_target_chip));

    for (auto l_target_cplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_ALL_CACHES |
                                           fapi2::TARGET_FILTER_ALL_CORES | fapi2::TARGET_FILTER_ALL_MC |
                                           fapi2::TARGET_FILTER_ALL_NEST | fapi2::TARGET_FILTER_ALL_OBUS |
                                           fapi2::TARGET_FILTER_ALL_PCI | fapi2::TARGET_FILTER_XBUS),
          fapi2::TARGET_STATE_FUNCTIONAL))
    {
        // Setting up partial good fence drop and resetting chiplet.
        FAPI_DBG("Enable pll");
        FAPI_TRY(p9_sbe_chiplet_reset_enable_pll(l_target_cplt));
    }

    for (auto l_target_cplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_ALL_MC |
                                           fapi2::TARGET_FILTER_NEST_WEST), fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_DBG("Drop clk async reset for N3 and Mc chiplets");
        FAPI_TRY(p9_sbe_chiplet_reset_mc_net_ctrl_clk_async_reset(l_target_cplt));
    }

    for (auto l_target_cplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (fapi2::TARGET_FILTER_ALL_MC, fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_DBG("Drop clk_div_bypass for Mc chiplet");
        FAPI_TRY(p9_sbe_chiplet_reset_div_clk_bypass(l_target_cplt));
    }

    for (auto l_target_cplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_ALL_CACHES |
                                           fapi2::TARGET_FILTER_ALL_CORES | fapi2::TARGET_FILTER_ALL_MC |
                                           fapi2::TARGET_FILTER_ALL_NEST | fapi2::TARGET_FILTER_ALL_OBUS |
                                           fapi2::TARGET_FILTER_ALL_PCI | fapi2::TARGET_FILTER_XBUS),
          fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_DBG("Enable chiplet and reset error register");
        FAPI_TRY(p9_sbe_chiplet_reset_setup(l_target_cplt));
    }

    for (auto l_target_cplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_ALL_MC |
                                           fapi2::TARGET_FILTER_ALL_NEST | fapi2::TARGET_FILTER_ALL_OBUS |
                                           fapi2::TARGET_FILTER_ALL_PCI | fapi2::TARGET_FILTER_XBUS),
          fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_DBG("Drop lvltrans fence and endpoint reset");
        FAPI_TRY(p9_sbe_chiplet_reset_net_ctrl_lvltrans_fence_pcb_ep_reset(
                     l_target_cplt));
    }

    for (auto l_target_cplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (fapi2::TARGET_FILTER_ALL_PCI, fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_DBG("Setup IOP logic for PCIe chiplet");
        FAPI_TRY(p9_sbe_chiplet_reset_pcie_iop_logic_setup(l_target_cplt));
    }

    for (auto l_target_cplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_ALL_MC |
                                           fapi2::TARGET_FILTER_ALL_NEST | fapi2::TARGET_FILTER_ALL_OBUS |
                                           fapi2::TARGET_FILTER_ALL_PCI | fapi2::TARGET_FILTER_XBUS),
          fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_DBG("Initialize OPCG registers for Nest,MC,XB,OB,PCIe");
        FAPI_TRY(p9_sbe_chiplet_reset_opcg_cnfg(l_target_cplt));
    }

    for (auto l_target_cplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_ALL_NEST |
                                           fapi2::TARGET_FILTER_ALL_OBUS | fapi2::TARGET_FILTER_ALL_PCI |
                                           fapi2::TARGET_FILTER_XBUS), fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_DBG("Enable listen to sync for NEST,OB,XB,PCIe");
        FAPI_TRY(p9_sbe_chiplet_reset_enable_listen_to_sync(l_target_cplt));
    }

    for (auto l_target_cplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (fapi2::TARGET_FILTER_ALL_MC, fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_DBG("Disable listen_to_sync for Nest,MC,XB,OB,PCIe");
        FAPI_TRY(p9_sbe_chiplet_reset_disable_listen_to_sync(l_target_cplt));
    }

    for (auto l_target_cplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_ALL_MC |
                                           fapi2::TARGET_FILTER_ALL_NEST | fapi2::TARGET_FILTER_ALL_OBUS |
                                           fapi2::TARGET_FILTER_ALL_PCI | fapi2::TARGET_FILTER_XBUS),
          fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_DBG("set scan ratio to 1:1 ");
        FAPI_TRY(p9_sbe_chiplet_reset_opcg_cnfg_scan_ratio(l_target_cplt));
    }

    for (auto l_target_cplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_ALL_MC |
                                           fapi2::TARGET_FILTER_ALL_NEST | fapi2::TARGET_FILTER_ALL_OBUS |
                                           fapi2::TARGET_FILTER_ALL_PCI | fapi2::TARGET_FILTER_XBUS),
          fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_TRY(p9_sbe_chiplet_reset_scan0_call(l_target_cplt));
    }

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Setup IOP logic for PCIe chiplet
///
/// @param[in]     i_target_chip   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
fapi2::ReturnCode p9_sbe_chiplet_reset_pcie_iop_logic_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chip)
{
    fapi2::buffer<uint64_t> l_data64_cplt_conf1;
    FAPI_INF("Entering ...");

    //Setting CPLT_CONF1 register value
    l_data64_cplt_conf1.flush<0>();
    l_data64_cplt_conf1.setBit<30>();  //CPLT_CONF1.TC_IOP_HSSPORWREN = 0b1
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_CPLT_CONF1_OR,
                            l_data64_cplt_conf1));

    fapi2::delay(p9SbeChipletReset::HW_NS_DELAY,
                 p9SbeChipletReset::SIM_CYCLE_DELAY);

    //Setting CPLT_CONF1 register value
    l_data64_cplt_conf1.flush<0>();
    l_data64_cplt_conf1.setBit<28>();  //CPLT_CONF1.TC_IOP_SYS_RESET_PCS = 0b1
    l_data64_cplt_conf1.setBit<29>();  //CPLT_CONF1.TC_IOP_SYS_RESET_PMA = 0b1
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_CPLT_CONF1_OR,
                            l_data64_cplt_conf1));

    FAPI_INF("Exiting ...");

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
    FAPI_INF("Entering ...");

    uint32_t l_attr_pg;

    FAPI_DBG("Reading ATTR_PG");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG, i_target_cplt, l_attr_pg));

    if ( l_attr_pg != 0xFFFF )
    {
        //Setting HANG_PULSE_0_REG register value (Setting all fields)
        //HANG_PULSE_0_REG.HANG_PULSE_REG_0 = p9SbeChipletReset::HANG_PULSE_0X10
        l_data64.insertFromRight<0, 6>(p9SbeChipletReset::HANG_PULSE_0X10);
        l_data64.clearBit<6>();  //HANG_PULSE_0_REG.SUPPRESS_HANG_0 = 0
        FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_HANG_PULSE_0_REG, l_data64));
    }

    FAPI_INF("Exiting ...");

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
    FAPI_INF("Entering ...");

    //Setting NET_CTRL0 register value
    //NET_CTRL0 = p9SbeChipletReset::NET_CNTL0_HW_INIT_VALUE
    FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_NET_CTRL0,
                            p9SbeChipletReset::NET_CNTL0_HW_INIT_VALUE));
    //Setting NET_CTRL1 register value
    //NET_CTRL1 = p9SbeChipletReset::NET_CNTL1_HW_INIT_VALUE
    FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_NET_CTRL1,
                            p9SbeChipletReset::NET_CNTL1_HW_INIT_VALUE));

    FAPI_INF("Exiting ...");

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
    FAPI_INF("Entering ...");

    uint32_t l_attr_pg;

    FAPI_DBG("Reading ATTR_PG");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG, i_target_ep, l_attr_pg));

    if ( l_attr_pg != 0xFFFF )
    {
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
    }

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief clock mux settings for Mc chiplet
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @param[in]     i_clk_mux_value    clock mux value
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_clk_mux_MC(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet,
    const fapi2::buffer<uint32_t> i_clk_mux_value)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("Entering ...");

    uint32_t l_attr_pg;

    FAPI_DBG("Reading ATTR_PG");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG, i_target_chiplet, l_attr_pg));

    if ( l_attr_pg != 0xFFFF )
    {
        //Setting NET_CTRL1 register value
        FAPI_TRY(fapi2::getScom(i_target_chiplet, PERV_NET_CTRL1, l_data64));
        //NET_CTRL1.PLL_CLKIN_SEL = i_clk_mux_value.getBit<3>()
        l_data64.writeBit<PERV_1_NET_CTRL1_PLL_CLKIN_SEL>(i_clk_mux_value.getBit<3>());
        FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL1, l_data64));
    }

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief call all the related mux settings on chiplets
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PROC_CHIP target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_clk_mux_call(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chiplet)
{
    fapi2::buffer<uint32_t> l_read_attr;
    FAPI_INF("Entering ...");

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CLOCK_PLL_MUX, i_target_chiplet,
                           l_read_attr));

    for (auto l_target_cplt : i_target_chiplet.getChildren<fapi2::TARGET_TYPE_PERV>
         (fapi2::TARGET_FILTER_ALL_MC, fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_DBG("Mux settings for Mc chiplet");
        FAPI_TRY(p9_sbe_chiplet_reset_clk_mux_MC(l_target_cplt, l_read_attr));
    }

    for (auto l_target_cplt : i_target_chiplet.getChildren<fapi2::TARGET_TYPE_PERV>
         (fapi2::TARGET_FILTER_ALL_OBUS, fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_DBG("Mux settings for OB chiplet");
        FAPI_TRY(p9_sbe_chiplet_reset_clk_mux_obus(l_target_cplt, l_read_attr));
    }

    for (auto l_target_cplt : i_target_chiplet.getChildren<fapi2::TARGET_TYPE_PERV>
         (fapi2::TARGET_FILTER_XBUS, fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_DBG("Mux settings for XB chiplet");
        FAPI_TRY(p9_sbe_chiplet_reset_clk_mux_xbus(l_target_cplt, l_read_attr));
    }

    for (auto l_target_cplt : i_target_chiplet.getChildren<fapi2::TARGET_TYPE_PERV>
         (fapi2::TARGET_FILTER_ALL_PCI, fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_DBG("Mux settings for Pcie chiplet");
        FAPI_TRY(p9_sbe_chiplet_reset_clk_mux_pcie(l_target_cplt, l_read_attr));
    }

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief clock mux settings for OB chiplet
///
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @param[in]     i_clk_mux_value    Clock mux value
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_clk_mux_obus(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet,
    const fapi2::buffer<uint32_t> i_clk_mux_value)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("Entering ...");

    uint32_t l_attr_pg;
    uint8_t l_attr_unit_pos;

    FAPI_DBG("Reading ATTR_PG");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG, i_target_chiplet, l_attr_pg));

    if ( l_attr_pg != 0xFFFF )
    {
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, i_target_chiplet,
                               l_attr_unit_pos));

        if ( l_attr_unit_pos == 0x09 )
        {
            //Setting NET_CTRL1 register value
            FAPI_TRY(fapi2::getScom(i_target_chiplet, PERV_NET_CTRL1, l_data64));
            //NET_CTRL1.PLL_CLKIN_SEL = i_clk_mux_value.getBit<6>()
            l_data64.writeBit<PERV_1_NET_CTRL1_PLL_CLKIN_SEL>(i_clk_mux_value.getBit<6>());
            l_data64.writeBit<PERV_1_NET_CTRL1_REFCLK_CLKMUX0_SEL>
            (i_clk_mux_value.getBit<13>());  //NET_CTRL1.REFCLK_CLKMUX0_SEL = i_clk_mux_value.getBit<13>()
            l_data64.writeBit<PERV_1_NET_CTRL1_REFCLK_CLKMUX1_SEL>
            (i_clk_mux_value.getBit<15>());  //NET_CTRL1.REFCLK_CLKMUX1_SEL = i_clk_mux_value.getBit<15>()
            FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL1, l_data64));
        }

        if ( l_attr_unit_pos == 0x0A )
        {
            //Setting NET_CTRL1 register value
            FAPI_TRY(fapi2::getScom(i_target_chiplet, PERV_NET_CTRL1, l_data64));
            l_data64.writeBit<PERV_1_NET_CTRL1_PLL_CLKIN_SEL>
            (i_clk_mux_value.getBit<16>());  //NET_CTRL1.PLL_CLKIN_SEL = i_clk_mux_value.getBit<16>()
            FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL1, l_data64));
        }

        if ( l_attr_unit_pos == 0x0B )
        {
            //Setting NET_CTRL1 register value
            FAPI_TRY(fapi2::getScom(i_target_chiplet, PERV_NET_CTRL1, l_data64));
            l_data64.writeBit<PERV_1_NET_CTRL1_PLL_CLKIN_SEL>
            (i_clk_mux_value.getBit<17>());  //NET_CTRL1.PLL_CLKIN_SEL = i_clk_mux_value.getBit<17>()
            FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL1, l_data64));
        }

        if ( l_attr_unit_pos == 0x0C )
        {
            //Setting NET_CTRL1 register value
            FAPI_TRY(fapi2::getScom(i_target_chiplet, PERV_NET_CTRL1, l_data64));
            //NET_CTRL1.PLL_CLKIN_SEL = i_clk_mux_value.getBit<7>()
            l_data64.writeBit<PERV_1_NET_CTRL1_PLL_CLKIN_SEL>(i_clk_mux_value.getBit<7>());
            l_data64.writeBit<PERV_1_NET_CTRL1_REFCLK_CLKMUX0_SEL>
            (i_clk_mux_value.getBit<9>());  //NET_CTRL1.REFCLK_CLKMUX0_SEL = i_clk_mux_value.getBit<9>()
            l_data64.writeBit<PERV_1_NET_CTRL1_REFCLK_CLKMUX1_SEL>
            (i_clk_mux_value.getBit<14>());  //NET_CTRL1.REFCLK_CLKMUX1_SEL = i_clk_mux_value.getBit<14>()
            FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL1, l_data64));
        }
    }

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief clock mux settings for Pcie chiplet
///
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @param[in]     i_clk_mux_value    clock mux value
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_clk_mux_pcie(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet,
    const fapi2::buffer<uint32_t> i_clk_mux_value)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("Entering ...");

    uint32_t l_attr_pg;
    uint8_t l_attr_unit_pos;

    FAPI_DBG("Reading ATTR_PG");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG, i_target_chiplet, l_attr_pg));

    if ( l_attr_pg != 0xFFFF )
    {
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, i_target_chiplet,
                               l_attr_unit_pos));

        if ( l_attr_unit_pos == 0x0D )
        {
            //Setting NET_CTRL1 register value
            FAPI_TRY(fapi2::getScom(i_target_chiplet, PERV_NET_CTRL1, l_data64));
            //NET_CTRL1.PLL_CLKIN_SEL = i_clk_mux_value.getBit<5>()
            l_data64.writeBit<PERV_1_NET_CTRL1_PLL_CLKIN_SEL>(i_clk_mux_value.getBit<5>());
            l_data64.writeBit<PERV_1_NET_CTRL1_REFCLK_CLKMUX0_SEL>
            (i_clk_mux_value.getBit<10>());  //NET_CTRL1.REFCLK_CLKMUX0_SEL = i_clk_mux_value.getBit<10>()
            l_data64.writeBit<PERV_1_NET_CTRL1_REFCLK_CLKMUX1_SEL>
            (i_clk_mux_value.getBit<11>());  //NET_CTRL1.REFCLK_CLKMUX1_SEL = i_clk_mux_value.getBit<11>()
            FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL1, l_data64));
        }

        if ( l_attr_unit_pos == 0x0F )
        {
            //Setting NET_CTRL1 register value
            FAPI_TRY(fapi2::getScom(i_target_chiplet, PERV_NET_CTRL1, l_data64));
            //NET_CTRL1.PLL_CLKIN_SEL = i_clk_mux_value.getBit<4>()
            l_data64.writeBit<PERV_1_NET_CTRL1_PLL_CLKIN_SEL>(i_clk_mux_value.getBit<4>());
            FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL1, l_data64));
        }
    }

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief clock mux settings for XB chiplet
///
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @param[in]     i_clk_mux_value    clock mux value
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_clk_mux_xbus(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet,
    const fapi2::buffer<uint32_t> i_clk_mux_value)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("Entering ...");

    uint32_t l_attr_pg;

    FAPI_DBG("Reading ATTR_PG");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG, i_target_chiplet, l_attr_pg));

    if ( l_attr_pg != 0xFFFF )
    {
        //Setting NET_CTRL1 register value
        FAPI_TRY(fapi2::getScom(i_target_chiplet, PERV_NET_CTRL1, l_data64));
        //NET_CTRL1.PLL_CLKIN_SEL = i_clk_mux_value.getBit<8>()
        l_data64.writeBit<PERV_1_NET_CTRL1_PLL_CLKIN_SEL>(i_clk_mux_value.getBit<8>());
        FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL1, l_data64));
    }

    FAPI_INF("Exiting ...");

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
    FAPI_INF("Entering ...");

    uint32_t l_attr_pg;

    FAPI_DBG("Reading ATTR_PG");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG, i_target_ec, l_attr_pg));

    if ( l_attr_pg != 0xFFFF )
    {
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
    }

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Disable listen_to_sync mode for MC
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_disable_listen_to_sync(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet)
{
    FAPI_INF("Entering ...");

    uint32_t l_attr_pg;

    FAPI_DBG("Reading ATTR_PG");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG, i_target_chiplet, l_attr_pg));

    if ( l_attr_pg != 0xFFFF )
    {
        //Setting SYNC_CONFIG register value
        //SYNC_CONFIG = p9SbeChipletReset::SYNC_CONFIG_4TO1
        FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_SYNC_CONFIG,
                                p9SbeChipletReset::SYNC_CONFIG_4TO1));
    }

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Drop clk div bypass for Mc chiplet
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_div_clk_bypass(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("Entering ...");

    uint32_t l_attr_pg;

    FAPI_DBG("Reading ATTR_PG");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG, i_target_chiplet, l_attr_pg));

    if ( l_attr_pg != 0xFFFF )
    {
        FAPI_DBG("drop clk_div_bypass_en");
        //Setting NET_CTRL1 register value
        l_data64.flush<1>();
        //NET_CTRL1.CLK_DIV_BYPASS_EN = 0
        l_data64.clearBit<PERV_1_NET_CTRL1_CLK_DIV_BYPASS_EN>();
        FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL1_WAND, l_data64));
    }

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Enable listen_to_sync mode for all chiplets except MC
///
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_enable_listen_to_sync(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet)
{
    FAPI_INF("Entering ...");

    uint32_t l_attr_pg;

    FAPI_DBG("Reading ATTR_PG");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG, i_target_chiplet, l_attr_pg));

    if ( l_attr_pg != 0xFFFF )
    {
        //Setting SYNC_CONFIG register value
        //SYNC_CONFIG = p9SbeChipletReset::SYNC_CONFIG_DEFAULT
        FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_SYNC_CONFIG,
                                p9SbeChipletReset::SYNC_CONFIG_DEFAULT));
    }

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Enable PLL
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_enable_pll(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet)
{
    fapi2::buffer<uint64_t> l_data;
    uint32_t l_attr_pg = 0;
    FAPI_INF("Entering ...");

    FAPI_DBG("Reading ATTR_PG");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG, i_target_chiplet, l_attr_pg));

    if ( l_attr_pg != 0xFFFF )
    {
        l_data.flush<0>();
        l_data.setBit<31>();

        //Setting NET_CTRL0 register value
        //NET_CTRL0 = l_data
        FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL0_WOR, l_data));
    }

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Dropping the net_ctrl0 clock_async_reset
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_mc_net_ctrl_clk_async_reset(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("Entering ...");

    uint32_t l_attr_pg;

    FAPI_DBG("Reading ATTR_PG");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG, i_target_chiplet, l_attr_pg));

    if ( l_attr_pg != 0xFFFF )
    {
        //Setting NET_CTRL0 register value
        l_data64.flush<1>();
        //NET_CTRL0.CLK_ASYNC_RESET = 0
        l_data64.clearBit<PERV_1_NET_CTRL0_CLK_ASYNC_RESET>();
        FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL0_WAND, l_data64));

        FAPI_DBG("drop clk_div_bypass_en");
        //Setting NET_CTRL1 register value
        l_data64.flush<1>();
        //NET_CTRL1.CLK_DIV_BYPASS_EN = 0
        l_data64.clearBit<PERV_1_NET_CTRL1_CLK_DIV_BYPASS_EN>();
        FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL1_WAND, l_data64));
    }

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Configuring multicast registers for nest, cache, core
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_mc_setup(const
        fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet)
{
    FAPI_INF("Entering ...");

    uint32_t l_attr_pg;

    FAPI_DBG("Reading ATTR_PG");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG, i_target_chiplet, l_attr_pg));

    if ( l_attr_pg != 0xFFFF )
    {
        //Setting MULTICAST_GROUP_1 register value
        //MULTICAST_GROUP_1 = p9SbeChipletReset::MCGR0_CNFG_SETTINGS
        FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_MULTICAST_GROUP_1,
                                p9SbeChipletReset::MCGR0_CNFG_SETTINGS));
    }

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Multicast register setup for MC01, MC23
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_mc_setup_MC(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet)
{
    FAPI_INF("Entering ...");

    uint32_t l_attr_pg;

    FAPI_DBG("Reading ATTR_PG");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG, i_target_chiplet, l_attr_pg));

    if ( l_attr_pg != 0xFFFF )
    {
        //Setting MULTICAST_GROUP_1 register value
        //MULTICAST_GROUP_1 = p9SbeChipletReset::MCGR0_CNFG_SETTINGS
        FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_MULTICAST_GROUP_1,
                                p9SbeChipletReset::MCGR0_CNFG_SETTINGS));
        //Setting MULTICAST_GROUP_2 register value
        //MULTICAST_GROUP_2 = p9SbeChipletReset::MCGR2_CNFG_SETTINGS
        FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_MULTICAST_GROUP_2,
                                p9SbeChipletReset::MCGR2_CNFG_SETTINGS));
    }

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Multicast register setup for Cache chiplets
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_mc_setup_cache(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet)
{
    FAPI_INF("Entering ...");

    uint32_t l_attr_pg;

    FAPI_DBG("Reading ATTR_PG");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG, i_target_chiplet, l_attr_pg));

    if ( l_attr_pg != 0xFFFF )
    {
        FAPI_DBG("Setting Multicast register 1&2 for cache chiplet");
        //Setting MULTICAST_GROUP_1 register value
        //MULTICAST_GROUP_1 = p9SbeChipletReset::MCGR0_CNFG_SETTINGS
        FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_MULTICAST_GROUP_1,
                                p9SbeChipletReset::MCGR0_CNFG_SETTINGS));
        //Setting MULTICAST_GROUP_2 register value
        //MULTICAST_GROUP_2 = p9SbeChipletReset::MCGR2_CACHE_CNFG_SETTINGS
        FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_MULTICAST_GROUP_2,
                                p9SbeChipletReset::MCGR2_CACHE_CNFG_SETTINGS));

        if ( l_attr_pg == 0x0 )
        {
            FAPI_DBG("Setting up multicast register 3&4 for cache chiplet");
            //Setting MULTICAST_GROUP_3 register value
            //MULTICAST_GROUP_3 = p9SbeChipletReset::MCGR3_CACHE_CNFG_SETTINGS
            FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_MULTICAST_GROUP_3,
                                    p9SbeChipletReset::MCGR3_CACHE_CNFG_SETTINGS));
            //Setting MULTICAST_GROUP_4 register value
            //MULTICAST_GROUP_4 = p9SbeChipletReset::MCGR4_CACHE_CNFG_SETTINGS
            FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_MULTICAST_GROUP_4,
                                    p9SbeChipletReset::MCGR4_CACHE_CNFG_SETTINGS));
        }
    }

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Multicast register setup for core chiplets
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_mc_setup_core(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet)
{
    FAPI_INF("Entering ...");

    uint32_t l_attr_pg;

    FAPI_DBG("Reading ATTR_PG");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG, i_target_chiplet, l_attr_pg));

    if ( l_attr_pg != 0xFFFF )
    {
        //Setting MULTICAST_GROUP_1 register value
        //MULTICAST_GROUP_1 = p9SbeChipletReset::MCGR0_CNFG_SETTINGS
        FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_MULTICAST_GROUP_1,
                                p9SbeChipletReset::MCGR0_CNFG_SETTINGS));
        //Setting MULTICAST_GROUP_2 register value
        //MULTICAST_GROUP_2 = p9SbeChipletReset::MCGR1_CNFG_SETTINGS
        FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_MULTICAST_GROUP_2,
                                p9SbeChipletReset::MCGR1_CNFG_SETTINGS));
    }

    FAPI_INF("Exiting ...");

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
    FAPI_INF("Entering ...");

    // Local variables
    //
    uint8_t   l_attr_chipunit_pos;
    uint32_t l_attr_pg;
    const uint8_t N0 = 0x02;
    const uint8_t N1 = 0x03;
    const uint8_t N3 = 0x05;

    FAPI_DBG("Reading ATTR_PG");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG, i_target_cplt, l_attr_pg));

    if ( l_attr_pg != 0xFFFF )
    {
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
    }

    FAPI_INF("Exiting ...");

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
    FAPI_INF("Entering ...");

    for (auto l_target_cplt : i_target_cplt.getChildren<fapi2::TARGET_TYPE_PERV>
         (static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_ALL_NEST |
                                           fapi2::TARGET_FILTER_ALL_OBUS | fapi2::TARGET_FILTER_ALL_PCI |
                                           fapi2::TARGET_FILTER_XBUS), fapi2::TARGET_STATE_FUNCTIONAL))
    {
        // Configuring chiplet multicasting registers.
        FAPI_DBG("Configuring multicasting registers for Nest,Xb,Obus,pcie chiplets" );
        FAPI_TRY(p9_sbe_chiplet_reset_mc_setup(l_target_cplt));
    }

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Drop Endpoint reset
/// Drop lvltrans fence
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode
p9_sbe_chiplet_reset_net_ctrl_lvltrans_fence_pcb_ep_reset(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("Entering ...");

    uint32_t l_attr_pg;

    FAPI_DBG("Reading ATTR_PG");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG, i_target_chiplet, l_attr_pg));

    if ( l_attr_pg != 0xFFFF )
    {
        FAPI_DBG("Drop lvltrans fence");
        //Setting NET_CTRL0 register value
        l_data64.flush<1>();
        //NET_CTRL0.LVLTRANS_FENCE = 0b0
        l_data64.clearBit<PERV_1_NET_CTRL0_LVLTRANS_FENCE>();
        FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL0_WAND, l_data64));

        FAPI_DBG("Drop endpoint reset");
        //Setting NET_CTRL0 register value
        l_data64.flush<1>();
        //NET_CTRL0.PCB_EP_RESET = 0b0
        l_data64.clearBit<PERV_1_NET_CTRL0_PCB_EP_RESET>();
        FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL0_WAND, l_data64));
    }

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief configuring Nest chiplet OPCG registers
///
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_opcg_cnfg(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("Entering ...");

    uint32_t l_attr_pg;

    FAPI_DBG("Reading ATTR_PG");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG, i_target_chiplet, l_attr_pg));

    if ( l_attr_pg != 0xFFFF )
    {
        //Setting OPCG_ALIGN register value
        l_data64 =
            p9SbeChipletReset::OPCG_ALIGN_SETTING;  //OPCG_ALIGN = p9SbeChipletReset::OPCG_ALIGN_SETTING
        //OPCG_ALIGN.INOP_ALIGN = p9SbeChipletReset::INOP_ALIGN_SETTING_0X5
        l_data64.insertFromRight<0, 4>(p9SbeChipletReset::INOP_ALIGN_SETTING_0X5);
        l_data64.clearBit<PERV_1_OPCG_ALIGN_INOP_WAIT, PERV_1_OPCG_ALIGN_INOP_WAIT_LEN>();  //OPCG_ALIGN.INOP_WAIT = 0
        //OPCG_ALIGN.OPCG_WAIT_CYCLES = p9SbeChipletReset::OPCG_WAIT_CYCLE_0X020
        l_data64.insertFromRight<52, 12>(p9SbeChipletReset::OPCG_WAIT_CYCLE_0X020);
        l_data64.insertFromRight<PERV_1_OPCG_ALIGN_SCAN_RATIO, PERV_1_OPCG_ALIGN_SCAN_RATIO_LEN>
        (p9SbeChipletReset::SCAN_RATIO_0X3);  //OPCG_ALIGN.SCAN_RATIO = p9SbeChipletReset::SCAN_RATIO_0X3
        FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_OPCG_ALIGN, l_data64));
    }

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief set scan ratio to 1:1 as long as PLL is in bypass mode
///
/// @param[in]     i_target_cplt   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_opcg_cnfg_scan_ratio(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_cplt)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("Entering ...");

    uint32_t l_attr_pg;

    FAPI_DBG("Reading ATTR_PG");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG, i_target_cplt, l_attr_pg));

    if ( l_attr_pg != 0xFFFF )
    {
        FAPI_DBG("Set scan ratio to 1:1 as long as PLL is in bypass mode");
        //Setting OPCG_ALIGN register value
        FAPI_TRY(fapi2::getScom(i_target_cplt, PERV_OPCG_ALIGN, l_data64));
        l_data64.insertFromRight<PERV_1_OPCG_ALIGN_SCAN_RATIO, PERV_1_OPCG_ALIGN_SCAN_RATIO_LEN>
        (p9SbeChipletReset::SCAN_RATIO_0X0);  //OPCG_ALIGN.SCAN_RATIO = p9SbeChipletReset::SCAN_RATIO_0X0
        FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_OPCG_ALIGN, l_data64));
    }

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Scan0 module call
///
/// @param[in]     i_target_chip   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_scan0_call(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chip)
{
    fapi2::buffer<uint16_t> l_regions;
    int l_timeout = 1 ;
    FAPI_INF("Entering ...");

    FAPI_TRY(p9_perv_sbe_cmn_regions_setup_16(i_target_chip,
             p9SbeChipletReset::REGIONS_EXCEPT_VITAL, l_regions));

    while(l_timeout != 0)
    {

        FAPI_DBG("run scan0 module for region except vital and pll, scan types GPTR, TIME, REPR");
        FAPI_TRY(p9_perv_sbe_cmn_scan0_module(i_target_chip, l_regions,
                                              p9SbeChipletReset::SCAN_TYPES_TIME_GPTR_REPR));

        FAPI_DBG("run scan0 module for region except vital and pll, scan types except GPTR, TIME, REPR");
        FAPI_TRY(p9_perv_sbe_cmn_scan0_module(i_target_chip, l_regions,
                                              p9SbeChipletReset::SCAN_TYPES_EXCEPT_TIME_GPTR_REPR));
        FAPI_DBG("Loop Count :%d", l_timeout);
        --l_timeout;

    }

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Dorping fence on Partial good chiplet and resetting it.
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_setup(const
        fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("Entering ...");

    // Local variable and constant definition
    uint32_t l_attr_pg;
    const uint64_t ERROR_DEFAULT_VALUE = 0xFFFFFFFFFFFFFFFFull;

    FAPI_DBG("Reading ATTR_PG");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG, i_target_chiplet, l_attr_pg));

    if ( l_attr_pg != 0xFFFF )
    {
        // EP Reset all chiplet with in multicasting group
        //Setting NET_CTRL0 register value
        l_data64.flush<0>();
        //NET_CTRL0.CHIPLET_ENABLE = 0b1
        l_data64.setBit<PERV_1_NET_CTRL0_CHIPLET_ENABLE>();
        FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL0_WOR, l_data64));

        //Setting ERROR_REG register value
        //ERROR_REG = ERROR_DEFAULT_VALUE
        FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_ERROR_REG, ERROR_DEFAULT_VALUE));
    }

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

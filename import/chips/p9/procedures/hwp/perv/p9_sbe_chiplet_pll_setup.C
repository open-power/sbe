/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: chips/p9/procedures/hwp/perv/p9_sbe_chiplet_pll_setup.C $     */
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
/// @file  p9_sbe_chiplet_pll_setup.C
///
/// @brief Setup PLL for Obus, Xbus, PCIe, DMI
//------------------------------------------------------------------------------
// *HWP HW Owner        : Abhishek Agarwal <abagarw8@in.ibm.com>
// *HWP HW Backup Owner : Srinivas V Naga <srinivan@in.ibm.com>
// *HWP FW Owner        : sunil kumar <skumar8j@in.ibm.com>
// *HWP Team            : Perv
// *HWP Level           : 2
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------


//## auto_generated
#include "p9_sbe_chiplet_pll_setup.H"

#include "p9_perv_scom_addresses.H"


enum P9_SBE_CHIPLET_PLL_SETUP_Private_Constants
{
    NS_DELAY = 100000, // unit is nano seconds
    SIM_CYCLE_DELAY = 1000 // unit is sim cycles
};

static fapi2::ReturnCode p9_sbe_chiplet_pll_setup_function(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet);

fapi2::ReturnCode p9_sbe_chiplet_pll_setup(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    uint8_t l_read_attr = 0;
    auto l_perv_functional_vector =
        i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
        (fapi2::TARGET_STATE_FUNCTIONAL);
    FAPI_DBG("Entering ...");

    FAPI_INF("Switch MC meshs to Nest mesh");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_MC_SYNC_MODE, i_target_chip, l_read_attr));

    if ( l_read_attr )
    {
        for (auto l_chplt_trgt : l_perv_functional_vector)
        {
            uint8_t l_attr_chip_unit_pos = 0; //actual value is read in FAPI_ATTR_GET below
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_chplt_trgt,
                                   l_attr_chip_unit_pos));

            if (!((l_attr_chip_unit_pos == 0x09 || l_attr_chip_unit_pos == 0x0A
                   || l_attr_chip_unit_pos == 0x0B
                   || l_attr_chip_unit_pos == 0x0C/* ObusChiplet */) ||
                  (l_attr_chip_unit_pos == 0x0D || l_attr_chip_unit_pos == 0x0E
                   || l_attr_chip_unit_pos == 0x0F/* PcieChiplet */) ||
                  (l_attr_chip_unit_pos == 0x06/* XbusChiplet */)))
            {
                continue;
            }

            FAPI_INF("p9_sbe_chiplet_pll_setup_function");
            FAPI_TRY(p9_sbe_chiplet_pll_setup_function(l_chplt_trgt));
        }
    }
    else
    {
        for (auto l_chplt_trgt : l_perv_functional_vector)
        {
            uint8_t l_attr_chip_unit_pos = 0; //actual value is read in FAPI_ATTR_GET below
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_chplt_trgt,
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

            FAPI_INF("p9_sbe_chiplet_pll_setup_function");
            FAPI_TRY(p9_sbe_chiplet_pll_setup_function(l_chplt_trgt));
        }
    }

    FAPI_DBG("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Setup PLL for XBus, OBus, PCIe, (MC) chiplets
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_pll_setup_function(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet)
{
    fapi2::buffer<uint64_t> l_read_reg;
    fapi2::buffer<uint64_t> l_data64;
    uint8_t l_attr_chip_unit_pos = 0; //actual value is read in FAPI_ATTR_GET

    FAPI_DBG("Entering ...");

    FAPI_INF("Drop PLL Test Enable");
    //Setting NET_CTRL0 register value
    l_data64.flush<1>();
    l_data64.clearBit<3>();  //NET_CTRL0.PLL_TEST_EN = 0
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL0_WAND, l_data64));

    FAPI_INF("Drop PLL Reset");
    //Setting NET_CTRL0 register value
    l_data64.flush<1>();
    l_data64.clearBit<4>();  //NET_CTRL0.PLL_RESET = 0
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL0_WAND, l_data64));

    fapi2::delay(NS_DELAY, SIM_CYCLE_DELAY);

    FAPI_INF("Check  PLL lock");
    //Getting PLL_LOCK_REG register value
    FAPI_TRY(fapi2::getScom(i_target_chiplet, PERV_PLL_LOCK_REG,
                            l_read_reg)); //l_read_reg = PLL_LOCK_REG
    //-- Manual Update

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, i_target_chiplet,
                           l_attr_chip_unit_pos));

    //-- HW335674
    if (l_attr_chip_unit_pos == 0x06/* XbusChiplet */)
    {
        FAPI_ASSERT(l_read_reg.getBit<2>(),
                    fapi2::PLL_LOCK_ERR()
                    .set_PLL_READ(l_read_reg),
                    "ERROR:PLL LOCK NOT SET");
    }
    else
    {
        FAPI_ASSERT(l_read_reg.getBit<0>(),
                    fapi2::PLL_LOCK_ERR()
                    .set_PLL_READ(l_read_reg),
                    "ERROR:PLL LOCK NOT SET");
    }

    FAPI_INF("Drop PLL Bypass");
    //Setting NET_CTRL0 register value
    l_data64.flush<1>();
    l_data64.clearBit<5>();  //NET_CTRL0.PLL_BYPASS = 0
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL0_WAND, l_data64));

    FAPI_INF("Reset PCB Slave error register");
    //Setting ERROR_REG register value
    //ERROR_REG = 0xFFFFFFFFFFFFFFFF
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_ERROR_REG, 0xFFFFFFFFFFFFFFFF));

    FAPI_DBG("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}


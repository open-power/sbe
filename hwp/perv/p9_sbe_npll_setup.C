//------------------------------------------------------------------------------
/// @file  p9_sbe_npll_setup.C
///
/// @brief scan initialize level 0 & 1 PLLs
//------------------------------------------------------------------------------
// *HWP HW Owner        : Abhishek Agarwal <abagarw8@in.ibm.com>
// *HWP HW Backup Owner : Srinivas V Naga <srinivan@in.ibm.com>
// *HWP FW Owner        : sunil kumar <skumar8j@in.ibm.com>
// *HWP Team            : Perv
// *HWP Level           : 2
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------


//## auto_generated
#include "p9_sbe_npll_setup.H"

#include "perv_scom_addresses.H"


enum P9_SBE_NPLL_SETUP_Private_Constants
{
    NS_DELAY = 100000, // unit is nano seconds
    SIM_CYCLE_DELAY = 1000 // unit is sim cycles
};

fapi2::ReturnCode p9_sbe_npll_setup(const
                                    fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> & i_target_chip)
{
    fapi2::buffer<uint64_t> l_read_reg;
    uint8_t l_read_attr = 0;
    fapi2::buffer<uint64_t> l_data64_root_ctrl8;
    fapi2::buffer<uint64_t> l_data64;
    FAPI_DBG("Entering ...");

    FAPI_INF("Release PLL test enable for SS,  Filt & NEST PLLs");
    //Setting ROOT_CTRL8 register value
    FAPI_TRY(fapi2::getScom(i_target_chip, PERV_ROOT_CTRL8_SCOM,
                            l_data64_root_ctrl8));
    l_data64_root_ctrl8.clearBit<12>();  //PIB.ROOT_CTRL8.TP_PLL_TEST_ENABLE_DC = 0
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_ROOT_CTRL8_SCOM,
                            l_data64_root_ctrl8));

    FAPI_INF("Release SS PLL reset0");
    //Setting ROOT_CTRL8 register value
    l_data64_root_ctrl8.clearBit<0>();  //PIB.ROOT_CTRL8.TP_SSPLL_PLL_RESET0_DC = 0
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_ROOT_CTRL8_SCOM,
                            l_data64_root_ctrl8));

    fapi2::delay(NS_DELAY, SIM_CYCLE_DELAY);

    FAPI_INF("check SS PLL lock");
    //Getting PLL_LOCK_REG register value
    FAPI_TRY(fapi2::getScom(i_target_chip, PERV_TP_PLL_LOCK_REG,
                            l_read_reg)); //l_read_reg = PERV.PLL_LOCK_REG

    FAPI_ASSERT(l_read_reg.getBit<0>(),
                fapi2::SS_PLL_LOCK_ERR()
                .set_SS_PLL_READ(l_read_reg),
                "ERROR:SS PLL LOCK NOT SET");

    FAPI_INF("Release SS PLL Bypass0");
    //Setting ROOT_CTRL8 register value
    FAPI_TRY(fapi2::getScom(i_target_chip, PERV_ROOT_CTRL8_SCOM,
                            l_data64_root_ctrl8));
    //PIB.ROOT_CTRL8.TP_SSPLL_PLL_BYPASS0_DC = 0
    l_data64_root_ctrl8.clearBit<1>();
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_ROOT_CTRL8_SCOM,
                            l_data64_root_ctrl8));

    FAPI_INF("Release Filter PLL reset1");
    //Setting ROOT_CTRL8 register value
    //PIB.ROOT_CTRL8.TP_FILTPLL_PLL_RESET1_DC = 0
    l_data64_root_ctrl8.clearBit<4>();
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_ROOT_CTRL8_SCOM,
                            l_data64_root_ctrl8));

    fapi2::delay(NS_DELAY, SIM_CYCLE_DELAY);

    FAPI_INF("check  PLL lock for CP Filter PLL , Check PLL lock fir IO Filter PLL");
    //Getting PLL_LOCK_REG register value
    FAPI_TRY(fapi2::getScom(i_target_chip, PERV_TP_PLL_LOCK_REG,
                            l_read_reg)); //l_read_reg = PERV.PLL_LOCK_REG

    FAPI_ASSERT(l_read_reg.getBit<1>() && l_read_reg.getBit<2>(),
                fapi2::FILTER_PLL_LOCK_ERR()
                .set_FILTER_PLL_READ(l_read_reg),
                "ERROR:CP or IO FILTER PLL LOCK NOT SET");

    FAPI_INF("Release FILTER PLL Bypass Signals");
    //Setting ROOT_CTRL8 register value
    FAPI_TRY(fapi2::getScom(i_target_chip, PERV_ROOT_CTRL8_SCOM,
                            l_data64_root_ctrl8));
    //PIB.ROOT_CTRL8.TP_FILTPLL_PLL_BYPASS1_DC = 0
    l_data64_root_ctrl8.clearBit<5>();
    //PIB.ROOT_CTRL8.TP_FILTPLL_CP_ALT_BYPASS_DC = 0
    l_data64_root_ctrl8.clearBit<6>();
    //PIB.ROOT_CTRL8.TP_FILTPLL_IO_ALT_BYPASS_DC = 0
    l_data64_root_ctrl8.clearBit<7>();
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_ROOT_CTRL8_SCOM,
                            l_data64_root_ctrl8));

    FAPI_INF("Switch MC meshs to Nest mesh");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_MC_SYNC_MODE, i_target_chip, l_read_attr));

    if ( l_read_attr == 1 )
    {
        FAPI_INF("Set MUX to Nest Clock input");
        //Setting ROOT_CTRL8 register value
        l_data64_root_ctrl8.setBit<31>();  //PIB.ROOT_CTRL8.TP_PLL_CLKIN_SEL4_DC = 1
        FAPI_TRY(fapi2::putScom(i_target_chip, PERV_ROOT_CTRL8_SCOM,
                                l_data64_root_ctrl8));
    }

    FAPI_INF("Release test_pll_bypass2");
    //Setting ROOT_CTRL8 register value
    //PIB.ROOT_CTRL8.TP_TANKPLL_TEST_PLL_BYPASS2_DC = 0
    l_data64_root_ctrl8.clearBit<10>();
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_ROOT_CTRL8_SCOM,
                            l_data64_root_ctrl8));

    FAPI_INF("Release Tank PLL reset2");
    //Setting PERV_CTRL0 register value
    FAPI_TRY(fapi2::getScom(i_target_chip, PERV_PERV_CTRL0_SCOM, l_data64));
    l_data64.clearBit<4>();  //PIB.PERV_CTRL0.TP_PLLRST_DC = 0
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_PERV_CTRL0_SCOM, l_data64));

    fapi2::delay(NS_DELAY, SIM_CYCLE_DELAY);

    FAPI_INF("Check  NEST PLL lock");
    //Getting PLL_LOCK_REG register value
    FAPI_TRY(fapi2::getScom(i_target_chip, PERV_TP_PLL_LOCK_REG,
                            l_read_reg)); //l_read_reg = PERV.PLL_LOCK_REG

    FAPI_ASSERT(l_read_reg.getBit<3>(),
                fapi2::NEST_PLL_ERR()
                .set_NEST_PLL_READ(l_read_reg),
                "ERROR:NEST PLL LOCK NOT SET");

    FAPI_INF("Release Tank PLL bypass2");
    //Setting PERV_CTRL0 register value
    FAPI_TRY(fapi2::getScom(i_target_chip, PERV_PERV_CTRL0_SCOM, l_data64));
    l_data64.clearBit<5>();  //PIB.PERV_CTRL0.TP_PLLBYP_DC = 0
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_PERV_CTRL0_SCOM, l_data64));

    FAPI_DBG("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

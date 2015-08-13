///
/// @file  p9_hcd_cache_dpll_setup.C
/// @brief Quad DPLL Setup
///
// *HWP HWP Owner   : David Young       <davidy@us.ibm.com>
// *HWP FW Owner    : Sangeetha T S  <sangeet2@in.ibm.com>
// *HWP Team        : PM
// *HWP Consumed by : SBE:SGPE
// *HWP Level       : 2
//
// Procedure Summary:
//   Put DPLL into bypass
//   Set DPLL syncmux sel
//   *Set clock controller scan ratio to 1:1 as this is done at refclk speeds
//   *xLoad the EX DPLL scan ring
//   *Set clock controller scan ratio to 8:1 for future scans
//   Frequency is controlled by the Quad PPM
//   Actual frequency value for boot is stored into the Quad PPM by
//     p9_hcd_setup_evid.C in istep 2
//   In real cache STOP exit, the frequency value is persistent
//   Enable the DPLL in the correct mode
//   non-dynamic
//   Slew rate established per DPLL team
//   Take the cache glitchless mux out of reset
//     (TODO:  is there still a reset on P9?)
//   Remove DPLL bypass
//   Drop DPLL Tholds
//   Check for DPLL lock
//   Timeout:  200us
//   Switch cache glitchless mux to use the DPLL
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <fapi2.H>
#if 0
#include <common_scom_addresses.H>
//will be replaced with real scom address header file
#endif
#include "p9_hcd_cache_dpll_setup.H"

//-----------------------------------------------------------------------------
// Constant Definitions
//-----------------------------------------------------------------------------

#define QPPM_DPLLFREQ 0x100f0151
#define QPPM_DPLLPARM 0x100f0152
#define QPPM_DPLLPARM_CLR 0x100f0153
#define QPPM_DPLLPARM_OR 0x100f0154
#define QPPM_DPLLSTAT 0x100f0155

#define PERV_CLK_REGION 0x00030006
#define PERV_TP_CLK_REGION 0x01030006
#define PERV_N0_CLK_REGION 0x02030006

enum PERV_CLK_REGION_param {
    CLOCK_CMD_SIZE=2,
    CLOCK_CMD_BIT=0,
    CLOCK_REGION_UNIT0_BIT=2,
    CLOCK_REGION_UNIT1=3,
    CLOCK_REGION_UNIT2=4,
    CLOCK_REGION_UNIT3=5,
    CLOCK_REGION_UNIT4=6,
    CLOCK_REGION_UNIT5=7,
    CLOCK_REGION_PLL=8,
    CLOCK_REGION_OSCSW=9,
    SEL_THOLD_SL=10,
    SEL_THOLD_NSL=11,
    SEL_THOLD_ARY=12
};

enum PERV_CLK_REGION_cmd {
    CLOCK_CMD_NOP=0,
    CLOCK_CMD_START=1,
    CLOCK_CMD_STOP=2,
    CLOCK_CMD_PULSE=3
};

enum {FMAX_BIT=3, FMIN_BIT=15, FMAX_FIELD_SIZE=9, FMIN_FIELD_SIZE=9};

enum {
    PDPLL_LOCK_SEL_BIT=0,
    DYNAMIC_FILTER_ENABLE_BIT=1,
    FF_BYPASS_BIT=2,
    FF_SLEWRATE_BIT=6,
    FF_SLEWRATE_FIELD_SIZE=10
};

enum {
    FREQOUT_BIT=1,
    EXTENDED_FREQOUT_BIT=12,
    FSAFE_ACTIVE_BIT=16,
    FREQ_CHANGE_BIT=22,
    LOCK_BIT=23,
    DPLL_CHAR_TBD_BIT=24,
    FASTER_THAN_FMAX_PLUS_DELTA1_BIT=25,
    SLOWER_THAN_FMIN_MINUS_DELTA2_BIT=26,
    FREQOUT_MAX_BIT=33,
    FREQOUT_MAX_EXTENDED_BIT=44,
    FREQOUT_MIN_BIT=49,
    FREQOUT_MIN_EXTENDED_BIT=60
};

enum {FREQOUT_FIELD_SIZE=11, EXTENDED_FREQOUT_FIELD_SIZE=4,
      FREQOUT_MAX_FIELD_SIZE=11, FREQOUT_MAX_EXTENDED_FIELD_SIZE=4,
      FREQOUT_MIN_FIELD_SIZE=11, FREQOUT_MIN_EXTENDED_FIELD_SIZE=4};

#define QPPM_QACCR 0x100f0160
#define QPPM_QACCR_CLR 0x100f0161
#define QPPM_QACCR_OR 0x100f0162

enum QPPM_QACCR_fields {
    CORE_CLK_SB_STRENGTH_BIT=0, CORE_CLK_SB_STRENGTH_LENGTH=4,
    CORE_CLK_SB_SPARE_BIT=4, CORE_CLK_SB_SPARE_LENGTH=1,
    CORE_CLK_SB_PULSE_MODE_EN_BIT=5, CORE_CLK_SB_PULSE_MODE_EN_LENGTH=1,
    CORE_CLK_SB_PULSE_MODE_BIT=6, CORE_CLK_SB_PULSE_MODE_LENGTH=2,
    CORE_CLK_SW_RESCLK_BIT=8, CORE_CLK_SW_RESCLK_LENGTH=4,
    CORE_CLK_SW_SPARE_BIT=12, CORE_CLK_SW_SPARE_LENGTH=1,
    L2_EX0_CLK_SYNC_ENABLE_BIT=13, L2_EX0_CLK_SYNC_ENABLE_LENGTH=1,
    RESERVED_14_151_BIT=14, RESERVED_14_151_LENGTH=2,
    L2_EX0_CLKGLM_ASYNC_RESET_BIT=16, L2_EX0_CLKGLM_ASYNC_RESET_LENGTH=1,
    RESERVED_17_182_BIT=17, RESERVED_17_182_LENGTH=2,
    L2_EX0_CLKGLM_SEL_BIT=19, L2_EX0_CLKGLM_SEL_LENGTH=1,
    L2_EX0_CLK_SB_STRENGTH_BIT=20, L2_EX0_CLK_SB_STRENGTH_LENGTH=4,
    L2_EX0_CLK_SB_SPARE0_BIT=24, L2_EX0_CLK_SB_SPARE0_LENGTH=1,
    L2_EX0_CLK_SB_PULSE_MODE_EN_BIT=25, L2_EX0_CLK_SB_PULSE_MODE_EN_LENGTH=1,
    L2_EX0_CLK_SB_PULSE_MODE_BIT=26, L2_EX0_CLK_SB_PULSE_MODE_LENGTH=2,
    L2_EX0_CLK_SW_RESCLK_BIT=28, L2_EX0_CLK_SW_RESCLK_LENGTH=4,
    L2_EX0_CLK_SW_SPARE1_BIT=32, L2_EX0_CLK_SW_SPARE1_LENGTH=1,
    L2_EX1_CLK_SYNC_ENABLE_BIT=33, L2_EX1_CLK_SYNC_ENABLE_LENGTH=1,
    RESERVED_34_353_BIT=34, RESERVED_34_353_LENGTH=2,
    L2_EX1_CLKGLM_ASYNC_RESET_BIT=36, L2_EX1_CLKGLM_ASYNC_RESET_LENGTH=1,
    RESERVED_37_384_BIT=37, RESERVED_37_384_LENGTH=2,
    L2_EX1_CLKGLM_SEL_BIT=39, L2_EX1_CLKGLM_SEL_LENGTH=1,
    L2_EX1_CLK_SB_STRENGTH_BIT=40, L2_EX1_CLK_SB_STRENGTH_LENGTH=4,
    L2_EX1_CLK_SB_SPARE0_BIT=44, L2_EX1_CLK_SB_SPARE0_LENGTH=1,
    L2_EX1_CLK_SB_PULSE_MODE_EN_BIT=45, L2_EX1_CLK_SB_PULSE_MODE_EN_LENGTH=1,
    L2_EX1_CLK_SB_PULSE_MODE_BIT=46, L2_EX1_CLK_SB_PULSE_MODE_LENGTH=2,
    L2_EX1_CLK_SW_RESCLK_BIT=48, L2_EX1_CLK_SW_RESCLK_LENGTH=4,
    L2_EX1_CLK_SW_SPARE1_BIT=52, L2_EX1_CLK_SW_SPARE1_LENGTH=1,
    RESERVED_53_55_BIT=53, RESERVED_53_55_LENGTH=3,
    L3_CLK_SB_STRENGTH_BIT=56, L3_CLK_SB_STRENGTH_LENGTH=4,
    L3_CLK_SB_SPARE0_BIT=60, L3_CLK_SB_SPARE0_LENGTH=1,
    L3_CLK_SB_PULSE_MODE_EN_BIT=61, L3_CLK_SB_PULSE_MODE_EN_LENGTH=1,
    L3_CLK_SB_PULSE_MODE_BIT=62, L3_CLK_SB_PULSE_MODE_LENGTH=2};


#define PPM_CGCR     0x100f0165
#define PPM_CGCR_CLR 0x100f0166
#define PPM_CGCR_OR  0x100f0167

enum PPM_CGCR_fields {
    CLKGLM_ASYNC_RESET_BIT=0, CLKGLM_ASYNC_RESET_LENGTH=1,
    RESERVED_1_21_BIT=1, RESERVED_1_21_LENGTH=2,
    CLKGLM_SEL_BIT=3, CLKGLM_SEL_LENGTH=1,
    RESERVED2_BIT=4, RESERVED2_LENGTH=60};

enum PPM_CGCR_CLKGLM_SEL_field_options{
    REFCLK=0,   //0: refclk
    DPLL= 1};   //1: DPLL output
                // (full speed Div1 for Core, half speed Div2 for Cache)


enum {CYCLES_PER_MS = 500000};

// TODO: count inst per loop in dissassembly
enum {INST_PER_LOOP = 8};

//-----------------------------------------------------------------------------
// Procedure: Quad DPLL Setup
//-----------------------------------------------------------------------------

// Setup DPLL like this: (from dpll_spec_14nm_20140107)
//  (marked as IPL from IPL document)
// Initial State:  (Do we need these (1-3) in IPL?)
// 1) reset, dpll_func_clksel, and all SL_HLD inputs are asserted
// 2) If grid clock connected to dpll clkout, bypass also has to be asserted to allow refclk on grid
// 3) Scan init:
//    - All scan rings in DPLL are scanned with zeroes
//    - Scan in VPD table values into the DPLL_MODE ring (config registers)
//     lf_param_dc<0:15> = b1010001010000000# will change based on HW characterization
//     lf_sdorder_dc<0:1> = b10
//     cd_div124_dc<0:1> = b10
//     cd_dpllout124_dc<0:1> = b10
//     vc_vtune_dc<0:2> = 0b100 # may change based on HW characterization
//     ref_div_dc<0:5> = b000001
//     ref_div_vreg_dc<0:4> = b000010
//     refclk_sel_dc = b1
//     All configuration register bits not specified should be set to b0

#define FAPI_CLEANUP() fapi_try_exit:

fapi2::ReturnCode
p9_hcd_cache_dpll_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_EQ>& i_target)
{
    uint32_t l_loopsPerMs;
    fapi2::buffer<uint64_t> l_data;

    FAPI_INF("EX Chiplet -> Start DPLL setup" )
    FAPI_INF("Set up CPM PARM register for DPLL")

    FAPI_TRY(fapi2::getScom(i_target, QPPM_DPLLPARM, l_data));
    // ----- Make sure that ff_bypass is set to "1" -----
    // This is necessary to ensure that the DPLL is in Mode 1.  If not,
    // the lock times will go from ~30us to 3-5ms

    FAPI_DBG("Put DPLL into Mode 1 by asserting ff_bypass.")
    l_data.setBit<FF_BYPASS_BIT>();

    FAPI_DBG("Set slew rate to a modest value")
    l_data.insertFromRight<FF_SLEWRATE_BIT,FF_SLEWRATE_FIELD_SIZE>(0x40);
    FAPI_TRY(fapi2::putScom(i_target, QPPM_DPLLPARM, l_data));

    // ----- Clear dpllclk_muxsel (syncclk_muxsel) to "0" -----
    FAPI_INF("Reset syncclk_muxsel or dpllclk_muxsel")
    // TC_SYNCCLK_MUXSEL_DC: syncclk mux for DPLL ,Reset(0b1)
    // TODO: do we need this in this procedure?

    FAPI_TRY(fapi2::putScom(i_target, QPPM_QACCR_CLR,
                     l_data.flush<0>().
                     setBit<L2_EX0_CLK_SYNC_ENABLE_BIT>().
                     setBit<L2_EX1_CLK_SYNC_ENABLE_BIT>()
                      ));

    FAPI_TRY(fapi2::putScom(i_target, PPM_CGCR_CLR,
                     l_data.flush<0>().
                     setBit<CLKGLM_SEL_BIT>()
                      ));

    // ----- Take DPLL out of bypass -----
    FAPI_INF("EX Chiplet -> Take DPLL out of bypass" )
    FAPI_TRY(fapi2::putScom(i_target, QPPM_DPLLPARM_CLR,
            l_data.flush<0>().setBit<FF_BYPASS_BIT>()));


    // TODO: There doesn't seem to be a CLR version of this register...
    FAPI_INF("EX Chiplet -> Drop internal DPLL THOLD" )
    //DPLL_THOLD: Thold for the DPLL clock domain in the chiplet
    FAPI_TRY(fapi2::getScom(i_target, PERV_TP_CLK_REGION, l_data));

    FAPI_TRY(fapi2::putScom(i_target, PERV_TP_CLK_REGION,
                 l_data.clearBit<SEL_THOLD_SL>()));

    // ----- Delay to wait for DPLL to lock  -----
    // TODO:  Determine whether or not we should POLL instead of put delay here.
    // Wait for >150us

    // Check for lock
    FAPI_INF("Wait for DPLL to lock" )
    // - waits       DPLL_LOCK_LOOP_NUMBER*DPLL_LOCK_DELAY
    l_loopsPerMs = CYCLES_PER_MS/INST_PER_LOOP;
    do
    {
        FAPI_TRY(fapi2::getScom(i_target, QPPM_DPLLSTAT, l_data));  // poll
        break; // TODO:  Skipping the lock checking until model is ready
    } while ((l_data.getBit<LOCK_BIT>() != 1 ) &&
            (--l_loopsPerMs >= 0));

    FAPI_ASSERT((l_loopsPerMs != 0),
                 fapi2::PMPROC_DPLL_LOCK_TIMEOUT()
                        .set_ADDRESS(QPPM_DPLLSTAT),
                 "DPLL lock timeout");

    FAPI_INF("EX Chiplet -> Recycle DPLL in and out of bypass" )
    FAPI_TRY(fapi2::putScom(i_target, QPPM_DPLLPARM_OR,
            l_data.flush<0>().setBit<FF_BYPASS_BIT>()));
    FAPI_TRY(fapi2::putScom(i_target, QPPM_DPLLPARM_CLR,
            l_data));

    FAPI_INF("EX Chiplet -> EX DPLL is locked" )

    // ----- Set Glitch-Less MUXes for IPL/Winkle case ----
    // For Sleep, the core glitchless mux is change earlier as the DPLL
    // was already up and locked.
    //
    // Set various gl muxes to pass the output of the DPLLs to the clock grid.

    FAPI_INF("EX Chiplet -> Set glitchless mux select for primary chiplet clock source to 001 ")
    FAPI_INF("EX Chiplet -> Do we need: Set glitchless mux select for core and eco domain to 0?")
    l_data.flush<0>().setBit<CLKGLM_SEL_BIT>();  // setBit corresponds to 1: DPLL

    // Store the final result to the hardware
    FAPI_TRY(fapi2::putScom(i_target, PPM_CGCR_OR, l_data));

    // ----- Drop ff_bypass to enable slewing -----
    FAPI_INF("EX Chiplet -> Clear ff_bypass to switch into slew-controlled mode")
    l_data.flush<0>().setBit<FF_BYPASS_BIT>();
    FAPI_TRY(fapi2::putScom(i_target, QPPM_DPLLPARM_CLR, l_data));

    // ----- Drop other tholds -----
    FAPI_INF("EX Chiplet -> Drop DPLL thold" )
    //DPLL_THOLD: Thold for the DPLL clock domain in the chiplet
    FAPI_TRY(fapi2::getScom(i_target, PERV_TP_CLK_REGION, l_data));
    FAPI_TRY(fapi2::putScom(i_target, PERV_TP_CLK_REGION,
                 l_data.clearBit<SEL_THOLD_NSL>().
                      clearBit<SEL_THOLD_ARY>()));


    // - updatestep STEP_SBE_DPLL_SETUP_COMPLETE, D0, P1
    FAPI_INF("EX Chiplet -> DPLL setup completed" )
    // fapi2::current_err will be initialized with FAPI2_RC_SUCCESS
    FAPI_CLEANUP()
    return fapi2::current_err;
} // Procedure




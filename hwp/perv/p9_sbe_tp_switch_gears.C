//------------------------------------------------------------------------------
/// @file  p9_sbe_tp_switch_gears.C
///
/// @brief Switch from refclock to PLL AND adjust I2C
//------------------------------------------------------------------------------
// *HWP HW Owner        : abagarw8 <srinivan@in.ibm.com>
// *HWP HW Backup Owner : Srinivas V Naga <srinivan@in.ibm.com>
// *HWP FW Owner        : sunil kumar <skumarj8@in.ibm.com>
// *HWP Team            : Perv
// *HWP Level           : 2
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------


//## auto_generated
#include "p9_sbe_tp_switch_gears.H"

#include "p9_sbe_gear_switcher.H"


fapi2::ReturnCode p9_sbe_tp_switch_gears(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> & i_target_chip)
{
    FAPI_DBG("Entering ...");

    FAPI_TRY(p9_sbe_gear_switcher_apply_i2c_bit_rate_divisor_setting(
                 i_target_chip));

    FAPI_TRY(p9_sbe_gear_switcher_i2c_stop_sequence(i_target_chip));

    FAPI_DBG("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

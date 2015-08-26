//------------------------------------------------------------------------------
/// @file  p9_sbe_gear_switcher.C
///
/// @brief Modules for I2C Bit rate divisor setting
/// And stop sequence on I2C
//------------------------------------------------------------------------------
// *HWP HW Owner        : Abhishek Agarwal <abagarw8@in.ibm.com>
// *HWP HW Backup Owner : Srinivas V Naga <srinivan@in.ibm.com>
// *HWP FW Owner        : sunil kumar <skumar8j@in.ibm.com>
// *HWP Team            : Perv
// *HWP Level           : 2
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------


//## auto_generated
#include "p9_sbe_gear_switcher.H"

#include "misc_scom_addresses.H"
#include "perv_scom_addresses.H"


enum P9_SBE_GEAR_SWITCHER_Private_Constants
{
    DEFAULT_MB_BIT_RATE_DIVISOR = 0x00000000,
    BUS_STATUS_BUSY_POLL_COUNT = 64
};

/// @brief --adjust I2C bit rate divisor setting in I2CM B mode reg
///
/// @param[in]     i_target_chip   Reference to TARGET_TYPE_PROC_CHIP target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
fapi2::ReturnCode p9_sbe_gear_switcher_apply_i2c_bit_rate_divisor_setting(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> & i_target_chip)
{
    fapi2::buffer<uint64_t> l_read_scratch_reg = 0;
    uint16_t l_mb_bit_rate_divisor = 0;
    FAPI_DBG("Entering ...");

    FAPI_INF("Check Mailbox for Valid I2C bit rate divisor setting");
    //Getting SCRATCH_REGISTER_2 register value
    FAPI_TRY(fapi2::getScom(i_target_chip, PERV_SCRATCH_REGISTER_2_SCOM,
                            l_read_scratch_reg)); //l_read_scratch_reg = PIB.SCRATCH_REGISTER_2

    if ( !l_read_scratch_reg )
    {
        FAPI_INF("Set with Default value if Mailbox empty");
        //Setting MODE_REGISTER_B register value
        //PIB.MODE_REGISTER_B = DEFAULT_MB_BIT_RATE_DIVISOR
        FAPI_TRY(fapi2::putScom(i_target_chip, PU_MODE_REGISTER_B,
                                DEFAULT_MB_BIT_RATE_DIVISOR));
    }
    else
    {
        l_read_scratch_reg.extractToRight<0, 16>(l_mb_bit_rate_divisor);

        FAPI_INF("Adjust I2C bit rate divisor setting in I2CM B Mode Reg");
        //Setting MODE_REGISTER_B register value
        //PIB.MODE_REGISTER_B = l_mb_bit_rate_divisor
        FAPI_TRY(fapi2::putScom(i_target_chip, PU_MODE_REGISTER_B,
                                l_mb_bit_rate_divisor));
    }

    FAPI_DBG("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief --send a stop sequence on I2C
/// --poll for stop command completion
/// --check for magic number
///
/// @param[in]     i_target_chip   Reference to TARGET_TYPE_PROC_CHIP target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
fapi2::ReturnCode p9_sbe_gear_switcher_i2c_stop_sequence(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> & i_target_chip)
{
    uint8_t l_read_attr = 0;
    fapi2::buffer<uint64_t> l_data64;
    int l_timeout = 0;
    FAPI_DBG("Entering ...");

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_BACKUP_SEEPROM_SELECT, i_target_chip,
                           l_read_attr));

    // WRITE Control register
    // enable enhance mode
    // Point to port_0 where the Primary SEEPROM Sits
    FAPI_INF("Send a STOP sequence on I2C");
    //Setting CONTROL_REGISTER_B register value
    FAPI_TRY(fapi2::getScom(i_target_chip, PU_CONTROL_REGISTER_B, l_data64));
    l_data64.setBit<3>();  //PIB.CONTROL_REGISTER_B.PIB_CNTR_REG_BIT_WITHSTOP_0 = 1
    //PIB.CONTROL_REGISTER_B.PIB_CNTR_REG_PORT_NUMBER_0 = l_read_attr
    l_data64.insertFromRight<18, 5>(l_read_attr);
    l_data64.setBit<26>();  //PIB.CONTROL_REGISTER_B.ENH_MODE_0 = 1
    FAPI_TRY(fapi2::putScom(i_target_chip, PU_CONTROL_REGISTER_B, l_data64));

    FAPI_INF("Poll for stop command completion");
    l_timeout = BUS_STATUS_BUSY_POLL_COUNT;

    //UNTIL STATUS_REGISTER_B.BUS_STATUS_BUSY_0 == 0
    while (l_timeout != 0)
    {
        //Getting STATUS_REGISTER_B register value
        FAPI_TRY(fapi2::getScom(i_target_chip, PU_STATUS_REGISTER_B, l_data64));
        //bool l_poll_data = PIB.STATUS_REGISTER_B.BUS_STATUS_BUSY_0
        bool l_poll_data = l_data64.getBit<44>();

        if (l_poll_data == 0)
        {
            break;
        }

        --l_timeout;
    }

    FAPI_INF("Loop Count :%d", l_timeout);

    FAPI_ASSERT(l_timeout > 0,
                fapi2::BUS_STATUS_BUSY_0(),
                "ERROR:BUS_STSTUS_BUSY_0 NOT SET TO 0");

    FAPI_DBG("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

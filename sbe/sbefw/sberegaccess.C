/*
 * @file: ppe/sbe/sbefw/sberegaccess.C
 *
 * @brief This file contains interfaces to get/set FW flags either in the
 * scratch registers and/or the FW attributes.
 */

#include "sberegaccess.H"
#include "sbetrace.H"
#include "fapi2.H"
#include <ppe42_scom.h>
#include <p9_perv_scom_addresses.H>

using namespace fapi2;

/**
 * @brief Initizlize the class
 *
 * @return An RC indicating success/failure
 */
uint32_t SbeRegAccess::init()
{
    #define SBE_FUNC "SbeRegAccess::SbeRegAccess "
    static bool l_initDone = false;
    uint32_t l_rc = 0;
    uint64_t l_mbx8 = 0;
    do
    {
        if(l_initDone)
        {
            break;
        }
        Target<TARGET_TYPE_PROC_CHIP> l_chip = plat_getChipTarget();
        // Read SBE messaging register into iv_messagingReg
        l_rc = getscom_abs(PERV_SB_MSG_SCOM, &iv_messagingReg);
        if(PCB_ERROR_NONE != l_rc)
        {
            SBE_ERROR(SBE_FUNC"Failed reading sbe messaging reg., RC: 0x%08X. ",
                      l_rc);
            break;
        }
        // Read Mailbox register 8 to check if the mailbox registers 3 and 6 are
        // valid
        l_rc = getscom_abs(PERV_SCRATCH_REGISTER_8_SCOM, &l_mbx8);
        if(PCB_ERROR_NONE != l_rc)
        {
            SBE_ERROR(SBE_FUNC"Failed reading mailbox reg 7, RC: 0x%08X. ",
                      l_rc);
            break;
        }
        if(l_mbx8 & SBE_MBX8_MBX3_VALID_MASK)
        {
            // Read MBX3
            l_rc = getscom_abs(PERV_SCRATCH_REGISTER_3_SCOM, &iv_mbx3);
            if(PCB_ERROR_NONE != l_rc)
            {
                SBE_ERROR(SBE_FUNC"Failed reading mailbox reg 3, RC: 0x%08X. ",
                        l_rc);
                break;
            }
        }
        else
        {
            // Need to read the values off the attributes
            // TODO: via RTC 150291 : Needs to be changed when all FW control
            // attributes are to be combined into one
            uint8_t l_attr = 0;
            FAPI_ATTR_GET(ATTR_ISTEP_MODE, l_chip, l_attr);
            iv_istepMode = l_attr;
            FAPI_ATTR_GET(ATTR_SBE_RUNTIME_MODE, l_chip, l_attr);
            iv_sbeDestRuntime = l_attr;
            FAPI_ATTR_GET(ATTR_IS_SP_MODE, l_chip, l_attr);
            iv_fspAttached = l_attr;
            FAPI_ATTR_GET(ATTR_SBE_FFDC_ENABLE, l_chip, l_attr);
            iv_collectFFDC = l_attr;
            FAPI_ATTR_GET(ATTR_SBE_INTERNAL_FFDC_ENABLE, l_chip, l_attr);
            iv_sendFFDC = l_attr;
        }
        if(l_mbx8 & SBE_MBX8_MBX6_VALID_MASK)
        {
            // Read MBX6
            l_rc = getscom_abs(PERV_SCRATCH_REGISTER_6_SCOM, &iv_mbx6);
            if(PCB_ERROR_NONE != l_rc)
            {
                SBE_ERROR(SBE_FUNC"Failed reading mailbox reg 6, RC: 0x%08X. "
                          l_rc);
                break;
            }
        }
        // If the master/slave bit is 0 (either default or read from mbx6),
        // check the C4 board pin to determine role
        if(0 == iv_isSlave)
        {
            uint64_t l_sbeDevIdReg = 0;
            // Read device ID register
            l_rc = getscom_abs(PERV_DEVICE_ID_REG, &l_sbeDevIdReg);
            if(PCB_ERROR_NONE != l_rc)
            {
                SBE_ERROR(SBE_FUNC"Failed reading device id reg, RC: 0x%08X. "
                          l_rc);
                break;
            }
            iv_isSlave = l_sbeDevIdReg & SBE_DEV_ID_C4_PIN_MASK;
            SBE_DEBUG(SBE_FUNC"Overriding master/slave with data read from "
                      "C4 pin: HI: 0x%08X, LO: 0x%08X",
                      (uint32_t)(l_sbeDevIdReg >> 32),
                      (uint32_t)(l_sbeDevIdReg & 0xFFFFFFFF));
        }
    } while(false);

    SBE_DEBUG(SBE_FUNC"Read mailbox registers: mbx8: 0x%08X, mbx3: 0x%08X, "
              "mbx6: 0x%08X", (uint32_t)(l_mbx8 >> 32),
              (uint32_t)(iv_mbx3 >> 32), (uint32_t)(iv_mbx6 >> 32));
    l_initDone = true;
    return l_rc;
    #undef SBE_FUNC
}

/**
 * @brief Update the SBE states into the SBE messaging register. The
 * function does a read-modify-write, so any bits other than the state
 * bits are preserved. The current state of the register is set to
 * i_state, whereas the old current state is copied to previous state
 *
 * @param [in] i_state The current SBE state
 *
 * @return RC indicating success/failure.
 *
 */
uint32_t SbeRegAccess::updateSbeState(const sbeState &i_state)
{
    #define SBE_FUNC "SbeRegAccess::updateSbeState "
    uint32_t l_rc = 0;

    iv_prevState = iv_currState;
    iv_currState = i_state;
    l_rc = putscom_abs(PERV_SB_MSG_SCOM, iv_messagingReg);
    if(PCB_ERROR_NONE != l_rc)
    {
        SBE_ERROR(SBE_FUNC"Failed to update state to messaging "
                "register. RC: 0x%08X", l_rc);
    }
    return l_rc;
    #undef SBE_FUNC
}

/**
 * @brief Update the SBE IPL steps into the SBE messaging register. The
 * function does a read-modify-write, so any bits other than the IPL
 * steps are retianed
 *
 * @param [in] i_major IPL major step number
 * @param [in] i_minor IPL minor step number
 *
 * @return RC indicating success/failure.
 *
 */
uint32_t SbeRegAccess::updateSbeStep(const uint8_t i_major,
                                     const uint8_t i_minor)
{
    #define SBE_FUNC "SbeRegAccess::updateSbeStep "
    uint32_t l_rc = 0;

    iv_majorStep = i_major;
    iv_minorStep = i_minor;

    l_rc = putscom_abs(PERV_SB_MSG_SCOM, iv_messagingReg);
    if(l_rc)
    {
        SBE_ERROR(SBE_FUNC"Failed to update SBE step to messaging "
                "register. RC: 0x%08X", l_rc);
    }
    return l_rc;
    #undef SBE_FUNC
}

/**
 * @brief Set the SBE ready bit into the SBE messaging register
 * (meaning that SBE control loop is initialized) The function does a
 * read-modify-write, so any bits other than the SBE ready bit remain
 * unchanged.
 *
 * @return RC indicating success/failure.
 *
 */
uint32_t SbeRegAccess::setSbeReady()
{
    #define SBE_FUNC "SbeRegAccess::setSbeReady "
    uint32_t l_rc = 0;

    iv_sbeBooted = true;
    l_rc = putscom_abs(PERV_SB_MSG_SCOM, iv_messagingReg);
    if(l_rc)
    {
        SBE_ERROR(SBE_FUNC"Failed to update SBE ready state to "
                "messaging register. RC: 0x%08X", l_rc);
    }
    return l_rc;
    #undef SBE_FUNC
}


/**
 * @brief Set the MPIPL mode bit into the mailbox scratch reg. 3
 * The function does a read-modify-write, so any bits other than the
 * SBE ready bit remain unchanged.
 *
 * @param i_set [in] Whether to set or clear the MPIPL flag
 *
 * @return RC indicating success/failure.
 *
 */
uint32_t SbeRegAccess::setMpIplMode(const bool i_set)
{
    #define SBE_FUNC "SbeRegAccess::setMpIplMode"
    uint32_t l_rc = 0;
    uint8_t l_set = i_set;
    iv_mpiplMode = i_set;
    FAPI_ATTR_SET(ATTR_IS_MPIPL, Target<TARGET_TYPE_SYSTEM>(), l_set);
    l_rc = putscom_abs(PERV_SCRATCH_REGISTER_3_SCOM, iv_mbx3);
    if(l_rc)
    {
        SBE_ERROR(SBE_FUNC"Failed to set/clear MPIPL flag in "
                "mbx reg. 3. RC: 0x%08X", l_rc);
    }
    return l_rc;
    #undef SBE_FUNC
}


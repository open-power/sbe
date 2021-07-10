/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/core/sberegaccess.C $                               */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2018                        */
/* [+] International Business Machines Corp.                              */
/*                                                                        */
/*                                                                        */
/* Licensed under the Apache License, Version 2.0 (the "License");        */
/* you may not use this file except in compliance with the License.       */
/* You may obtain a copy of the License at                                */
/*                                                                        */
/*     http://www.apache.org/licenses/LICENSE-2.0                         */
/*                                                                        */
/* Unless required by applicable law or agreed to in writing, software    */
/* distributed under the License is distributed on an "AS IS" BASIS,      */
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or        */
/* implied. See the License for the specific language governing           */
/* permissions and limitations under the License.                         */
/*                                                                        */
/* IBM_PROLOG_END_TAG                                                     */
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
#include <p9_misc_scom_addresses.H>

using namespace fapi2;

// Struct to Map Current State - Event - Final State Transition
typedef struct stateTransitionStr
{
    uint16_t currState:4;
    uint16_t event:4;
    uint16_t finalState:4;
    uint16_t reserved:4;
} stateTransitionStr_t;

// Start and End point of Event Transition in stateTransMap Table
typedef struct stateEventRangeStr
{
    uint16_t start:8;
    uint16_t end:8;
}stateEventRangeStr_t;

// Entry Point and End point to the StateTransition Map for a State
// It is sequenced as per the sbeState enum, Don't change the sequence
// of states. Events are incremented w.r.t previous event.
static const stateEventRangeStr_t eventRangePerState[SBE_MAX_STATE] =
{
    {SBE_STATE_UNKNOWN_ENTRY_TO_MAP, SBE_STATE_UNKNOWN_MAX_EVENT},
    {SBE_STATE_IPLING_ENTRY_TO_MAP, SBE_STATE_IPLING_MAX_EVENT},
    {SBE_STATE_ISTEP_ENTRY_TO_MAP, SBE_STATE_ISTEP_MAX_EVENT},
    {SBE_STATE_MPIPL_ENTRY_TO_MAP, SBE_STATE_MPIPL_MAX_EVENT},
    {SBE_STATE_RUNTIME_ENTRY_TO_MAP, SBE_STATE_RUNTIME_MAX_EVENT},
    {SBE_STATE_DMT_ENTRY_TO_MAP, SBE_STATE_DMT_MAX_EVENT},
    {SBE_STATE_DUMP_ENTRY_TO_MAP, SBE_STATE_DUMP_MAX_EVENT},
    {SBE_STATE_FAILURE_ENTRY_TO_MAP, SBE_STATE_FAILURE_MAX_EVENT},
    {SBE_STATE_QUIESCE_ENTRY_TO_MAP, SBE_STATE_QUIESCE_MAX_EVENT},
};

// Map to connect the current State with an event along with the final state
// transition. It is sequenced according to the sbeState enums, Don't change the
// sequence of states.
static const stateTransitionStr_t stateTransMap[SBE_MAX_TRANSITIONS] = {
    {SBE_STATE_UNKNOWN, SBE_FAILURE_EVENT, SBE_STATE_FAILURE},
    {SBE_STATE_UNKNOWN, SBE_RUNTIME_EVENT, SBE_STATE_RUNTIME},
    {SBE_STATE_UNKNOWN, SBE_ISTEP_EVENT, SBE_STATE_ISTEP},
    {SBE_STATE_UNKNOWN, SBE_PLCK_EVENT, SBE_STATE_IPLING},
    {SBE_STATE_IPLING, SBE_RUNTIME_EVENT, SBE_STATE_RUNTIME},
    {SBE_STATE_IPLING, SBE_DUMP_FAILURE_EVENT, SBE_STATE_DUMP},
    {SBE_STATE_IPLING, SBE_FAILURE_EVENT, SBE_STATE_FAILURE},
    {SBE_STATE_IPLING, SBE_QUIESCE_EVENT, SBE_STATE_QUIESCE},
    {SBE_STATE_ISTEP, SBE_RUNTIME_EVENT, SBE_STATE_RUNTIME},
    {SBE_STATE_ISTEP, SBE_FAILURE_EVENT, SBE_STATE_FAILURE},
    {SBE_STATE_ISTEP, SBE_QUIESCE_EVENT, SBE_STATE_QUIESCE},
    {SBE_STATE_MPIPL, SBE_RUNTIME_EVENT, SBE_STATE_RUNTIME},
    {SBE_STATE_MPIPL, SBE_DUMP_FAILURE_EVENT, SBE_STATE_DUMP},
    {SBE_STATE_MPIPL, SBE_QUIESCE_EVENT, SBE_STATE_QUIESCE},
    {SBE_STATE_RUNTIME, SBE_DUMP_FAILURE_EVENT, SBE_STATE_DUMP},
    {SBE_STATE_RUNTIME, SBE_ENTER_MPIPL_EVENT, SBE_STATE_MPIPL},
    {SBE_STATE_RUNTIME, SBE_DMT_ENTER_EVENT, SBE_STATE_DMT},
    {SBE_STATE_RUNTIME, SBE_FAILURE_EVENT, SBE_STATE_FAILURE},
    {SBE_STATE_RUNTIME, SBE_QUIESCE_EVENT, SBE_STATE_QUIESCE},
    {SBE_STATE_DMT, SBE_DMT_COMP_EVENT, SBE_STATE_RUNTIME},
    {SBE_STATE_DMT, SBE_QUIESCE_EVENT, SBE_STATE_QUIESCE},
    {SBE_STATE_DMT, SBE_DUMP_FAILURE_EVENT, SBE_STATE_DUMP},
};

SbeRegAccess SbeRegAccess::cv_instance __attribute__((section (".sbss")));

/**
 * @brief Initizlize the class
 *
 * @return An RC indicating success/failure
 */

void SbeRegAccess::stateTransition(const sbeEvent &i_event)
{
    #define SBE_FUNC "SbeRegAccess::stateTransition "
    //Fetch Current State
    uint32_t l_state = (uint32_t)getSbeState();
    uint8_t l_startCnt = eventRangePerState[l_state].start;
    SBE_INFO(SBE_FUNC "Event Received %d CurrState 0x%08X StartCnt%d EndCnt%d",
        i_event, l_state, l_startCnt, eventRangePerState[l_state].end);
    // Fetch the final State from the Map
    while(l_startCnt <
          (eventRangePerState[l_state].end + eventRangePerState[l_state].start))
    {
        if(stateTransMap[l_startCnt].event == i_event)
        {
            SBE_INFO(SBE_FUNC "Updating State as %d",
                        (sbeState)stateTransMap[l_startCnt].finalState);
            updateSbeState((sbeState)stateTransMap[l_startCnt].finalState);
            break;
        }
        else
            ++l_startCnt;
    }
    #undef SBE_FUNC
}

uint32_t SbeRegAccess::init(bool forced)
{
    #define SBE_FUNC "SbeRegAccess::SbeRegAccess "
    static bool l_initDone = false;
    uint32_t rc = 0;

    do
    {
        if(l_initDone && !forced)
        {
            break;
        }
        // Read SBE messaging register into iv_messagingReg
        rc = getscom_abs(PERV_SB_MSG_SCOM, &iv_messagingReg);
        if(PCB_ERROR_NONE != rc)
        {
            SBE_ERROR(SBE_FUNC"Failed reading sbe messaging reg., RC: 0x%08X. ",
                      rc);
            break;
        }
        // Read Mailbox register 8 to check if mbox registers 3 and 6 are valid
        rc = getscom_abs(PERV_SCRATCH_REGISTER_8_SCOM, &iv_mbx8);
        if(PCB_ERROR_NONE != rc)
        {
            SBE_ERROR(SBE_FUNC"Failed reading mailbox reg 8, RC: 0x%08X. ", rc);
            break;
        }
        if(iv_mbx3_valid)
        {
            // Read MBX3
            rc = getscom_abs(PERV_SCRATCH_REGISTER_3_SCOM, &iv_mbx3);
            if(PCB_ERROR_NONE != rc)
            {
                SBE_ERROR(SBE_FUNC"Failed reading mailbox reg 3, RC: 0x%08X. ",
                        rc);
                break;
            }
        }
        else
        {
            // Need to read the values off the attributes
            uint32_t l_attr = 0;
            FAPI_ATTR_GET(ATTR_BOOT_FLAGS, Target<TARGET_TYPE_SYSTEM>(),
                          l_attr);
            iv_mbx3 = ((uint64_t) l_attr ) << 32;
        }
        if(iv_mbx6_valid)
        {
            // Read MBX6
            rc = getscom_abs(PERV_SCRATCH_REGISTER_6_SCOM, &iv_mbx6);
            if(PCB_ERROR_NONE != rc)
            {
                SBE_ERROR(SBE_FUNC"Failed reading mailbox reg 6, RC: 0x%08X. ",
                          rc);
                break;
            }
        }

        // If the master/slave bit is 0 (either default or read from mbx6),
        // check the C4 board pin to determine role
        // Read device ID register
        uint64_t l_sbeDevIdReg = 0;
        rc = getscom_abs(PERV_DEVICE_ID_REG, &l_sbeDevIdReg);
        if(PCB_ERROR_NONE != rc)
        {
            SBE_ERROR(SBE_FUNC"Failed reading device id reg, RC: 0x%08X.",rc);
            break;
        }

        if(0 == iv_isSlave)
        {
            iv_isSlave = l_sbeDevIdReg & SBE_DEV_ID_C4_PIN_MASK;
            SBE_INFO(SBE_FUNC"Overriding master/slave with data read from "
                      "C4 pin: HI: 0x%08X, LO: 0x%08X",
                      (uint32_t)(l_sbeDevIdReg >> 32),
                      (uint32_t)(l_sbeDevIdReg & 0xFFFFFFFF));
        }
    } while(false);

    SBE_INFO(SBE_FUNC"Read mailbox registers: mbx8: 0x%08X, mbx3: 0x%08X, "
              "mbx6: 0x%08X", (uint32_t)(iv_mbx8 >> 32),
              (uint32_t)(iv_mbx3 >> 32), (uint32_t)(iv_mbx6 >> 32));
    l_initDone = true;
    return rc;
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
    uint32_t rc = 0;

    iv_prevState = iv_currState;
    iv_currState = i_state;
    rc = putscom_abs(PERV_SB_MSG_SCOM, iv_messagingReg);
    if(PCB_ERROR_NONE != rc)
    {
        SBE_ERROR(SBE_FUNC"Failed to update state to messaging "
                "register. RC: 0x%08X", rc);
    }
    return rc;
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
    uint32_t rc = 0;

    iv_majorStep = i_major;
    iv_minorStep = i_minor;

    rc = putscom_abs(PERV_SB_MSG_SCOM, iv_messagingReg);
    if(rc)
    {
        SBE_ERROR(SBE_FUNC"Failed to update SBE step to messaging "
                "register. RC: 0x%08X", rc);
    }
    return rc;
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
    uint32_t rc = 0;

    iv_sbeBooted = true;
    rc = putscom_abs(PERV_SB_MSG_SCOM, iv_messagingReg);
    if(rc)
    {
        SBE_ERROR(SBE_FUNC"Failed to update SBE ready state to "
                "messaging register. RC: 0x%08X", rc);
    }
    return rc;
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
    uint32_t rc = 0;
    uint8_t l_set = i_set;

    //NOTE: FSP system during the reset-reload to runtime loses the CFAM 283A,
    //SCOM 5003A content. As a workaround we will use iv_mbx3 to do read modify
    //write into 5003A until HWSV fixes the defect SW485901.We will only update
    //the MPIPL bit in the iv_mbx3
    uint64_t scratch3Reg = 0;
    rc = getscom_abs(PERV_SCRATCH_REGISTER_3_SCOM, &scratch3Reg);
    if(rc)
    {
        SBE_ERROR(SBE_FUNC"Failed read PERV_SCRATCH_REGISTER_3_SCOM "
                "RC: 0x%08X", rc);
    }

    //Update the MPIPL mode bit in the SBE cached iv_mbx3 value.
    iv_mpiplMode = i_set;
    //Update the Attribute 
    PLAT_ATTR_INIT(ATTR_IS_MPIPL, Target<TARGET_TYPE_SYSTEM>(), l_set);
 
    if(i_set)
    {
        //Set the MPIPL bit
        scratch3Reg = scratch3Reg | 0x2000000000000000;
    }
    else
    {
        //Clear the MPIPL bit
        scratch3Reg = scratch3Reg & 0xDFFFFFFFFFFFFFFF;
    }

    //Write back to the register 
    rc = putscom_abs(PERV_SCRATCH_REGISTER_3_SCOM, scratch3Reg);
    if(rc)
    {
        SBE_ERROR(SBE_FUNC"Failed to set/clear MPIPL flag in "
                "mbx reg. 3. RC: 0x%08X", rc);
    }
    return rc;
    #undef SBE_FUNC
}

uint32_t SbeRegAccess::updateAsyncFFDCBit( bool i_on )
{
    #define SBE_FUNC "SbeRegAccess::updateAsyncFFDCBit "
    uint32_t rc = 0;

    iv_asyncFFDC = i_on;

    rc = putscom_abs(PERV_SB_MSG_SCOM, iv_messagingReg);
    if(rc)
    {
        SBE_ERROR(SBE_FUNC"Failed to update SBE Aync bit in message "
                "register. RC: 0x%08X", rc);
    }
    return rc;
    #undef SBE_FUNC
}

bool SbeRegAccess::isSbeRegressionBit()
{
    #define SBE_FUNC "SbeRegAccess::isSbeRegressionBit"
    bool isDisableInvalidScomAddrCheck = false;
    uint8_t readData;
    fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYSTEM;

    FAPI_ATTR_GET(fapi2::ATTR_SECURITY_MODE, FAPI_SYSTEM, readData);
    if((iv_mbx3_valid) && !readData && iv_disableInvalidScomAddrCheck)
    {
        isDisableInvalidScomAddrCheck = true;
    }
    return isDisableInvalidScomAddrCheck;
    #undef SBE_FUNC
}

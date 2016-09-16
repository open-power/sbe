/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/sberegaccess.H $                                    */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016                             */
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
 * @file: ppe/sbe/sbefw/sberegaccess.H
 *
 * @brief This file contains interfaces to get/set FW flags either in the
 * scratch registers and/or the FW attributes.
 */

#ifndef __SBEFW_SBEREGACCESS_H
#define __SBEFW_SBEREGACCESS_H

#include <stdint.h>
#include "sbestates.H"
#include "sbeevents.H"

/**
 * @brief Utility singleton that SBEFW can use to read write various scratch
 * registers/FW attributes
 * TODO: via RTC ??????: Need to change the read of scratch registers to FAPI
 * attribute accesses once we have the attributes defined in the HWP code.
 */
class SbeRegAccess
{
    public:
        // Disable copy construction and assignment operators
        SbeRegAccess(const SbeRegAccess&) = delete;
        SbeRegAccess& operator=(const SbeRegAccess&) = delete;

        /**
         * @brief Returns the instance of this class
         *
         * @return A reference to SbeRegAccess
         *
         */
        static SbeRegAccess& theSbeRegAccess()
        {
            static SbeRegAccess iv_instance;
            return iv_instance;
        }

        /**
         * @brief Initializes the class for use
         *
         * @return An RC indicating success/failure
         *
         */
        uint32_t init();

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
        uint32_t updateSbeState(const sbeState &i_state);

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
        uint32_t updateSbeStep(const uint8_t i_major, const uint8_t i_minor);

        /**
         * @brief Set the SBE ready bit into the SBE messaging register
         * (meaning that SBE control loop is initialized) The function does a
         * read-modify-write, so any bits other than the SBE ready bit remain
         * unchanged.
         *
         * @return RC indicating success/failure.
         *
         */
        uint32_t setSbeReady();

        /**
         * @brief Set the MPIPL mode bit into the mailbox scratch reg. 3
         * The function does a read-modify-write, so any bits other than the
         * SBE ready bit remain unchanged. It also updates the attribute
         * ATTR_MPIPL
         *
         * @param i_set [in] true == set, false == clear
         *
         * @return RC indicating success/failure.
         *
         */
        uint32_t setMpIplMode(const bool i_set);

        /**
         * @brief Check if we are on an FSP attached
         *
         * @return true if FSP attached system, false otherwise
         *
         */
        inline bool isFspSystem() const
        {
            return iv_fspAttached;
        }

        /**
         * @brief Check if we are in ISTEP IPL mode
         *
         * @return true if in istep mode, false otherwise
         *
         */
        inline bool isIstepMode() const
        {
            return iv_istepMode;
        }

        /**
         * @brief Check if SBE should directly go to runtime state
         *
         * @return true if SBE should go directly to runtime state,
         * false otherwise
         *
         */
        inline bool isDestBitRuntime() const
        {
            return iv_sbeDestRuntime;
        }

        /**
         * @brief Check if SBE should collect FFDC
         *
         * @return true if in istep mode, false otherwise
         *
         */
        inline bool isCollectFFDCSet() const
        {
            return iv_collectFFDC;
        }

        /**
         * @brief Check if SBE should send internal FFDC for any chip op
         * failures as a part of the response
         *
         * @return true if in istep mode, false otherwise
         *
         */
        inline bool isSendInternalFFDCSet() const
        {
            return iv_sendFFDC;
        }

        /**
         * @brief Check if SBE is slave/master
         *
         * @return true if SBE is slave, false if master
         *
         */
        inline bool isSbeSlave() const
        {
            return iv_isSlave;
        }

        /**
         * @brief Get the SBE current State
         *
         * @return SBE current State, sbeState enum
         *
         */
        uint64_t getSbeState() const
        {
            return iv_currState;
        }

        /**
         * @brief Update the SBE State as per the transition event
         *
         * @param [in] i_event Transition Event
         */
        void stateTransition(const sbeEvent &i_event);

    private:

        /**
         * @brief Constructor
         */
        SbeRegAccess() : iv_mbx3(0), iv_mbx6(0), iv_messagingReg(0)
        {}

        union
        {
            struct
            {
                uint64_t iv_istepMode : 1;
                uint64_t iv_sbeDestRuntime : 1;
                uint64_t iv_mpiplMode : 1;
                uint64_t iv_fspAttached : 1;
                uint64_t iv_collectFFDC : 1;
                uint64_t iv_sendFFDC : 1;
                uint64_t iv_mbx3DontCare : 26;
                uint64_t iv_mbx3Unused : 32;
            };
            uint64_t iv_mbx3;
        };

        union
        {
            struct
            {
                uint64_t iv_mbx6DontCare : 24;
                uint64_t iv_isSlave : 1;
                uint64_t iv_mbx6DontCare2 : 7;
                uint64_t iv_mbx6Unused : 32;
            };
            uint64_t iv_mbx6;
        };

        union
        {
            struct
            {
                uint64_t iv_sbeBooted : 1;
                uint64_t iv_reserved1 : 3;
                uint64_t iv_prevState : 4;
                uint64_t iv_currState : 4;
                uint64_t iv_majorStep : 4;
                uint64_t iv_minorStep : 8;
                uint64_t iv_reserved2 : 8;
                uint64_t iv_unused : 32;
            };
            uint64_t iv_messagingReg;
        };

        // Bit masks defining bits in the above registers that the SBE is
        // interested in
        static const uint64_t SBE_MBX8_MBX3_VALID_MASK = 0x2000000000000000ULL;
        static const uint64_t SBE_MBX8_MBX6_VALID_MASK = 0x0400000000000000ULL;
        static const uint64_t SBE_DEV_ID_C4_PIN_MASK = 0x0000000000800000ULL;
};
#endif //__SBEFW_SBEREGACCESS_H


/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_sbe_sync_quiesce_states.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2020                             */
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
/// @file  p10_sbe_sync_quiesce_states.C
///
/// @brief To Check on Quiescing of all Chips in a System by Local SBE
///     Local SBE to poll LQA Bit(Bit3) of Security Swich register(x10005) all other chips on the system
///     FW code to handle timeout scenarios if a chip isn't quiescing - LQA bit not getting set
///
//------------------------------------------------------------------------------
// *HWP HW Owner        : Santosh Balasubramanian <sbalasub@in.ibm.com>
// *HWP HW Backup Owner : Srinivas V Naga <srinivan@in.ibm.com>
// *HWP FW Owner        : Murulidhar Nataraju <murulidhar@in.ibm.com>
// *HWP Team            : Perv
// *HWP Level           : 3
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include <fapi2.H>
#include <p10_adu_setup.H>
#include <p10_adu_access.H>
#include <p10_adu_utils.H>
#include "p10_sbe_sync_quiesce_states.H"

///----------------------------------------------------------------------------
/// Constant definitions
///----------------------------------------------------------------------------
const uint64_t SECURITY_SWITCH_REG_ADDR  = 0x00010005;   // Security Switch Register Address
const uint64_t ADU_XSCOM_BASE_ADDR       = 0x000E03FC00000000;

fapi2::ReturnCode p10_sbe_sync_quiesce_states(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip,
        uint8_t& o_quiesced_status)
{
    FAPI_INF("p10_sbe_sync_quiesce_states : Entering ...");
    uint64_t l_sys_config   = 0;
    // Flags for ADU operation
    bool l_rnw = true;
    uint64_t address;
    uint32_t granulesBeforeSetup = 0;
    adu_operationFlag l_myAduFlag;
    uint8_t l_adu_lock_attempts = 5;
    bool firstGranule = true;
    bool lastGranule  = true;

    uint8_t  l_lqa_achieved = 0b00010000;
    uint8_t l_maxchips = 63;

    uint8_t validchip = 0;
    uint64_t group_id;
    uint64_t chip_id = 0;
    uint8_t data[8];
    uint8_t read_data;

    const fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYSTEM;

    // Bit Mapping of l_sys_config
    // 16-23: .........
    // 08-15: Node 1, Porc 8/9/10/11/12/13/14/15
    // 00-07: Node 0, Proc 0/1/2/3/4/5/6/7

    //Issue Reads to secure switch register(0x10005) of each of the other chips on the system and check for LQA bit(Bit 3)
    //Return back if LQA=0 for any chip - Start from beginning next time
    //If all valid chips have LQA bit set to '1', return successful RC back to upper level FW code

    // Gather system information
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_SBE_SYS_CONFIG, FAPI_SYSTEM, l_sys_config));
    FAPI_INF("Current System configuration : %#010lx - %#x", (l_sys_config >> 32), (uint32_t)l_sys_config);


    //Setup ADU flags
    l_myAduFlag.setOperationType(adu_operationFlag::CACHE_INHIBIT);   // XSCOM to PIB are Cache Inhibited
    l_myAduFlag.setTransactionSize(adu_operationFlag::TSIZE_8);       // 8 Bytes register read
    l_myAduFlag.setAutoIncrement(false);                             // No Auto Increment, single register read
    l_myAduFlag.setLockControl(true);                                // Set ADU lock
    l_myAduFlag.setOperFailCleanup(false);                           // Reset and release after failed ADU operation
    l_myAduFlag.setFastMode(false);                                  // Do Status check after every operation
    l_myAduFlag.setItagMode(false);                                  // Single 8B read operation, no need to collect itag?!
    l_myAduFlag.setEccMode(false);                                   // Don't collect ECC
    l_myAduFlag.setEccItagOverrideMode(false);                       // Don't overwrite ECC, not valid for Read operation
    l_myAduFlag.setNumLockAttempts(l_adu_lock_attempts);             // Number of lock attempts - Set to 5.

    //Walk through l_sys_config, if the bit is set(valid chip in system), read LQA bit value
    //Set quiesced_status as '1' - As soon as it encounters a chip with LQA not set, set quiesced_status to '0' and break
    o_quiesced_status = 1;

    while (validchip <= l_maxchips)
    {

        // Identify if the chip is not matching to originator of the request - Current chip's LQA should already be set
        //if ((l_sys_config & uint64_t(1)<<(63-validchip)) != l_current_position)
        //Procedure will ignore current chip position and issue a read to all chips including itself
        if ((l_sys_config & (uint64_t(1) << (63 - validchip))) != 0)
        {

            // Determine Group(Node) ID
            group_id = validchip / 8; //Divider

            // Determine Chip ID
            chip_id = validchip % 8; //Remainder

            //XSCOM_BASE_ADDR as per ADU document
            //PIB Address(0x00010005) in address location 30:60
            //Node Id in location 15:18, Chip id in location 19:21
            //address =  0x0006010000080028 | (group_id << 45) | (chip_id << 42);
            address =  ADU_XSCOM_BASE_ADDR | (group_id << 45) | (chip_id << 42) | (SECURITY_SWITCH_REG_ADDR << 3);
            FAPI_INF("Address obtained =0x%.8x%.8x", ((address & 0xFFFFFFFF00000000ull) >> 32), (address & 0xFFFFFFFF));
            // To read from each chip
            // Set up ADU
            FAPI_TRY(p10_adu_setup(i_target_chip, address, l_rnw, l_myAduFlag.setFlag(), granulesBeforeSetup));
            // Access ADU
            FAPI_TRY(p10_adu_access(i_target_chip, address, l_rnw, l_myAduFlag.setFlag(), firstGranule, lastGranule, data));

            read_data = data[0];

            //Check if Bit 3(LQA) is not set
            if (l_lqa_achieved != (read_data & l_lqa_achieved))
            {
                //Return Pending return code
                o_quiesced_status = 0;
                FAPI_ERR("Current Chip on the system not quiesced - %#018lx", (uint64_t(1) << (63 - validchip)));
                break; //From while loop with PENDING return code
            }
            else
            {
                FAPI_IMP("Chip [%#018lx] has LQA set", (uint64_t(1) << (63 - validchip)));
                FAPI_IMP("Current System configuration : %#010lx - %#x", (l_sys_config >> 32), (uint32_t)l_sys_config);
            }
        }

        validchip++;

    } //End of while (validchip <= 63)

    FAPI_INF("p10_sbe_sync_quiesce_states : Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

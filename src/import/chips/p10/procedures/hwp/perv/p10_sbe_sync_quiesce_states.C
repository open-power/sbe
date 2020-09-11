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
//Each 5-bit topology index addresses a 16 terabyte space
const uint64_t  MMIO_OFFSET_PER_TOPO_INDEX =  0x0000100000000000;

/**
 * @brief bit definitions for 5-bit mode 0/1 topology inde
 */
typedef union
{
    struct // GGG0C
    {
        uint8_t rsvd1: 3; // 0:2
        uint8_t group: 3; // 3:5
        uint8_t rsvd2: 1; // 6
        uint8_t chip : 1; // 7
    } mode0;
    struct // 0GGCC
    {
        uint8_t rsvd1: 4; // 0:3
        uint8_t group: 2; // 4:5
        uint8_t chip : 2; // 6:7
    } mode1;
    uint8_t topoIndex;
} topologyIndexBits_t;

fapi2::ReturnCode p10_sbe_sync_quiesce_states(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip,
        uint8_t& o_quiesced_status)
{
    FAPI_INF("p10_sbe_sync_quiesce_states : Entering ...");
    fapi2::buffer<uint64_t> sysConfig = 0x0;
    // Flags for ADU operation
    bool l_rnw = true;
    uint64_t address;
    uint32_t granulesBeforeSetup = 0;
    adu_operationFlag l_myAduFlag;
    uint8_t l_adu_lock_attempts = 5;
    bool firstGranule = true;
    bool lastGranule  = true;

    uint8_t  l_lqa_achieved = 0b00010000;
    uint8_t l_maxchips = 16;
    uint32_t group_id = 0;
    uint32_t chip_id = 0;
    uint8_t data[8] = {0};
    uint8_t read_data = 0;
    uint8_t topologyMode = 0;
    const fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYSTEM;

    //Issue Reads to secure switch register(0x10005) of each of the other chips on the system and check for LQA bit(Bit 3)
    //Return back if LQA=0 for any chip - Start from beginning next time
    //If all valid chips have LQA bit set to '1', return successful RC back to upper level FW code

    // Gather system information
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_SBE_SYS_CONFIG, FAPI_SYSTEM, sysConfig()));
    //Among 0-15 bits, any bit being set indicates that the processor with ATTR_PROC_FABRIC_EFF_TOPOLOGY_ID equal to
    //that bit position is functional
    FAPI_INF("Current System configuration : %.8x%.8x", (((uint64_t)sysConfig & 0xFFFFFFFF00000000ull) >> 32),
             ((uint64_t)sysConfig & 0xFFFFFFFF));
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

    //Walk through sysConfig, if the bit is set(valid chip in system), read LQA bit value
    //Set quiesced_status as '1' - As soon as it encounters a chip with LQA not set, set quiesced_status to '0' and break
    o_quiesced_status = 1;
    FAPI_ATTR_GET(fapi2::ATTR_PROC_FABRIC_TOPOLOGY_MODE, FAPI_SYSTEM, topologyMode);

    for(uint8_t TopologyId = 0; TopologyId < l_maxchips; ++TopologyId)
    {

        if(sysConfig.getBit(static_cast<uint32_t>(TopologyId)))
        {
            FAPI_IMP("PROC chip with Effective Fabric Topology ID:0x%.8x is functional", TopologyId);

            // Determine Group(Node) ID and the ChipId
            //If Mode = 0 , Topology ID: GGG_C
            //If Mode = 1 , Topology ID: GG_CC

            //5-bit mode 0/1 topology index value
            topologyIndexBits_t topoIndexBits;
            topoIndexBits.topoIndex = 0;

            if(topologyMode == fapi2::ENUM_ATTR_PROC_FABRIC_TOPOLOGY_MODE_MODE0) //GGG_C
            {
                chip_id = TopologyId & 0x1;
                group_id = (TopologyId & 0xE) >> 1;
                topoIndexBits.mode0.group = group_id;
                topoIndexBits.mode0.chip = chip_id;
            }
            else //GG_CC
            {
                chip_id = TopologyId & 0x3;
                group_id = (TopologyId & 0xC) >> 2;
                topoIndexBits.mode1.group = group_id;
                topoIndexBits.mode1.chip = chip_id;
            }

            FAPI_INF("group_id=0x%.8x, chip_id=0x%.8x topoIndexBits.topoIndex=0x%x", group_id, chip_id, topoIndexBits.topoIndex);

            //XSCOM_BASE_ADDR as per ADU document
            //PIB Address(0x00010005) in address location 30:60
            //Topology Index (5bit) MODE0:GGG0C / MODE1:0GGCC
            address =  (ADU_XSCOM_BASE_ADDR + (MMIO_OFFSET_PER_TOPO_INDEX * topoIndexBits.topoIndex)) |
                       (SECURITY_SWITCH_REG_ADDR << 3);
            FAPI_IMP("TopologyId=%d Address obtained =0x%.8x%.8x", TopologyId, ((address & 0xFFFFFFFF00000000ull) >> 32),
                     (address & 0xFFFFFFFF));
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
                FAPI_IMP("Proc Chip with Effective Topology Id :0x%.8x is not Quiesced", TopologyId);
                break; //From while loop with PENDING return code
            }
            else
            {
                FAPI_IMP("Proc Chip with Effective Topology Id :0x%.8x has LQA set", TopologyId);
            }
        }

    }

    FAPI_INF("p10_sbe_sync_quiesce_states : Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

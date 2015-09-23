/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/nest/p9_pba_coherent_utils.H $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2019                        */
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
//-----------------------------------------------------------------------------------
// *!
/// @file p9_pba_coherent_utils.H
/// @brief Common Code to support PBA get/putmem procedures (FAPI)
///
// *HWP HWP Owner: Christina Graves clgraves@us.ibm.com
// *HWP FW Owner: Thi Tran thi@us.ibm.com
// *HWP Team: Nest
// *HWP Level: 2
// *HWP Consumed by: SBE
// ---------------------------------------------------------------------------------
// *! ADDITIONAL COMMENTS :
// *!
// *!
//-----------------------------------------------------------------------------------

#ifndef _P9_PBA_COHERENT_UTILS_H_
#define _P9_PBA_COHERENT_UTILS_H_

//-----------------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------------

#include <fapi2.H>
#include <misc_scom_addresses.H>
#include <p9_pba_constants.H>

extern "C"
{

//-----------------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------------

//PBA Delay Constants
    const uint32_t PBA_SLVRST_DELAY_HW_NS = 1000;
    const uint32_t PBA_SLVRST_DELAY_SIM_CYCLES = 200;
    const uint32_t WRITE_DELAY_HW_NS = 100;
    const uint32_t WRITE_DELAY_SIM_CYCLES = 20;
    const uint32_t PBA_BAR_SCOPE_LOCAL_NODE = 0;

//PBA Slave Control register field/bit definitions
    const uint32_t PBA_SLVCTL_ENABLE_BIT = 0;
    const uint32_t PBA_SLVCTL_MASTER_ID_MATCH_START_BIT = 1;
    const uint32_t PBA_SLVCTL_MASTER_ID_MATCH_END_BIT = 3;
    const uint32_t PBA_SLVCTL_MASTER_ID_CARE_MASK_START_BIT = 5;
    const uint32_t PBA_SLVCTL_MASTER_ID_CARE_MASK_END_BIT = 7;
    const uint32_t PBA_SLVCTL_WRITE_TTYPE_START_BIT = 8;
    const uint32_t PBA_SLVCTL_WRITE_TTYPE_END_BIT = 10;
    const uint32_t PBA_SLVCTL_READ_TTYPE_BIT = 15;
    const uint32_t PBA_SLVCTL_READ_PREFETCH_CTL_START_BIT = 16;
    const uint32_t PBA_SLVCTL_READ_PREFETCH_CTL_END_BIT = 17;
    const uint32_t PBA_SLVCTL_READ_BUF_INVALIDATE_CTL_BIT = 18;
    const uint32_t PBA_SLVCTL_WRITE_BUF_PAIR_ALLOCATION_BIT = 19;
    const uint32_t PBA_SLVCTL_READ_BUF_PAIR_A_ALLOCATION_BIT = 20;
    const uint32_t PBA_SLVCTL_READ_BUF_PAIR_B_ALLOCATION_BIT = 21;
    const uint32_t PBA_SLVCTL_READ_BUF_PAIR_C_ALLOCATION_BIT = 22;
    const uint32_t PBA_SLVCTL_DISABLE_WRITE_GATHER_BIT = 24;
    const uint32_t PBA_SLVCTL_WRITE_GATHER_TIMEOUT_START_BIT = 25;
    const uint32_t PBA_SLVCTL_WRITE_GATHER_TIMEOUT_END_BIT = 27;
    const uint32_t PBA_SLVCTL_WRITE_TSIZE_START_BIT = 28;
    const uint32_t PBA_SLVCTL_WRITE_TSIZE_END_BIT = 35;
    const uint32_t PBA_SLVCTL_EXT_ADDR_START_BIT = 36;
    const uint32_t PBA_SLVCTL_EXT_ADDR_END_BIT = 49;

    const uint32_t PBA_SLVCTL_EXTADDR_SHIFT = 27;
    const uint32_t PBA_SLVCTL_EXTADDR_MASK = 0x3fff;

//PBA Slave Reset register field/bit definitions
    const uint32_t PBA_SLVRST_SET_START_BIT = 0;
    const uint32_t PBA_SLVRST_SET_END_BIT = 2;
    const uint32_t PBA_SLVRST_SLVCTL0_IN_PROG = 4;
    const uint32_t PBA_SLVRST_SLVCTL1_IN_PROG = 5;
    const uint32_t PBA_SLVRST_SLVCTL2_IN_PROG = 6;
    const uint32_t PBA_SLVRST_SLVCTL3_IN_PROG = 7;
    const uint32_t PBA_SLVRST_IN_PROG_START_BIT = 4;
    const uint32_t PBA_SLVRST_IN_PROG_END_BIT = 7;
    const uint32_t PBA_SLVRST_BUSY_START_BIT = 8;
    const uint32_t PBA_SLVRST_BUSY_END_BIT = 11;
    //mask to check if there is a PBA slave rest in progress and if the PBA Slave Control is busy
    //if it is not all these bits 4:11 should be set to 0
    const uint64_t PBA_SLVRST_BUSY_IN_PROG_MASK = 0xFF0000000000000ull;

//PBA Read Buffer Valid Status field/bit definitions
    const uint32_t PBA_RD_BUF_VALID_START_BIT = 33;
    const uint32_t PBA_RD_BUF_VALID_END_BIT = 39;
    const uint64_t PBA_RD_BUF_VALID_MASK = 0x7F000000ull;
    const uint64_t PBA_RD_BUF_EMPTY =  0x1000000ull;

//PBA Write Buffer Valid Status field/bit definitions
    const uint32_t PBA_WR_BUF_VALID_START_BIT = 35;
    const uint32_t PBA_WR_BUF_VALID_END_BIT = 39;
    const uint64_t PBA_WR_BUF_VALID_MASK = 0x1F000000ull;
    const uint64_t PBA_WR_BUF_EMPTY = 0x1000000ull;

//PBA BAR register field/bit definitions
    const uint32_t PBA_BAR_SCOPE_START_BIT = 0;
    const uint32_t PBA_BAR_SCOPE_END_BIT = 2;
    const uint32_t PBA_BAR_BASE_ADDRESS_START_BIT = 8;
    const uint32_t PBA_BAR_BASE_ADDRESS_END_BIT = 43;
    const uint32_t PBA_BAR_BASE_ADDRESS_SHIFT = 21;
    const uint64_t PBA_BAR_BASE_ADDRESS_MASK = 0xFFFFFFFFFull;

//PBA BAR Mask register field/bit definitions
    const uint32_t PBA_BAR_MASK_START_BIT = 23;
    const uint32_t PBA_BAR_MASK_END_BIT = 43;

//-----------------------------------------------------------------------------------
// Function prototypes
//-----------------------------------------------------------------------------------
    fapi2::ReturnCode p9_pba_coherent_setup_pba(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
        const uint64_t i_address,
        const bool i_rnw,
        const uint32_t i_flags);

    fapi2::ReturnCode p9_pba_coherent_pba_write(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
        const uint64_t i_address,
        const uint32_t i_flags,
        const uint64_t i_write_data[]);

    fapi2::ReturnCode p9_pba_coherent_pba_read(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
        const uint64_t i_address,
        const uint32_t i_flags,
        uint64_t o_read_data[]);

    fapi2::ReturnCode p9_pba_coherent_utils_get_num_granules(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
        const uint64_t i_address,
        uint32_t& o_numGranules);

    fapi2::ReturnCode p9_pba_coherent_utils_check_fbc_state(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target);

    fapi2::ReturnCode p9_pba_coherent_utils_check_args(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
        const uint64_t i_address);

    fapi2::ReturnCode p9_pba_coherent_status_check(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target);

    fapi2::ReturnCode p9_pba_coherent_cleanup_pba(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target);
    fapi2::ReturnCode p9_pba_coherent_setup_pba_bar(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
        const uint64_t i_baseAddress);

} //extern "C"

#endif //_P9_PBA_COHERENT_UTILS_H_

/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: chips/p9/procedures/ipl/hwp/tests/p9_pba_coherent_utils.H $   */
/*                                                                        */
/* IBM CONFIDENTIAL                                                       */
/*                                                                        */
/* EKB Project                                                            */
/*                                                                        */
/* COPYRIGHT 2015                                                         */
/* [+] International Business Machines Corp.                              */
/*                                                                        */
/*                                                                        */
/* The source code for this program is not published or otherwise         */
/* divested of its trade secrets, irrespective of what has been           */
/* deposited with the U.S. Copyright Office.                              */
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
#include <p9_pba_constants.H>

extern "C"
{

//-----------------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// Function prototypes
//-----------------------------------------------------------------------------------

/// @brief does the setup for the PBA to set up the initial registers for a read/write
/// @param[in] i_target => P9 chip target
/// @param[in] i_ex_target => Ex target for which L3 we are targeting
/// @param[in] i_address => starting address for PBA operation
/// @param[in] i_rnw => whether the operation is a read or write
/// @param[in] i_flags => flags that contain information that the PBA needs to know to set up registers
/// @return FAPI_RC_SUCCESS if setting up the pba registers is a success
    fapi2::ReturnCode p9_pba_coherent_setup_pba(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
        const fapi2::Target<fapi2::TARGET_TYPE_EX>& i_ex_target,
        const uint64_t i_address,
        const bool i_rnw,
        const uint32_t i_flags);

/// @brief does the write for the PBA
/// @param[in] i_target => P9 chip target
/// @param[in] i_address => address for this write
/// @param[in] i_flags => flags that contain information that the PBA needs to know to set up registers
/// @param[in] i_write_data => the data that is to be written to the PBA
/// @return FAPI_RC_SUCCESS if writing the PBA is a success
    fapi2::ReturnCode p9_pba_coherent_pba_write(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
        const uint64_t i_address,
        const uint32_t i_flags,
        const uint64_t i_write_data[]);

/// @brief does the read for the PBA
/// @param[in] i_target => P9 chip target
/// @param[in] i_address => address for this write
/// @param[in] i_flags => flags that contain information that the PBA needs to know to set up registers
/// @param[out] o_read_data => the data that is read from the PBA
/// @return FAPI_RC_SUCCESS if reading the PBA is a success
    fapi2::ReturnCode p9_pba_coherent_pba_read(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
        const uint64_t i_address,
        const uint32_t i_flags,
        uint64_t o_read_data[]);

/// @brief calculates the number of 128 byte granules that can be read/written before setup needs to be run again
/// @param[in] i_target => P9 chip target
/// @param[in] i_address => starting address for PBA operation
/// @return number of 128 byte granules that can be read/written before setup needs to be run again
    fapi2::ReturnCode p9_pba_coherent_utils_get_num_granules(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
        const uint64_t i_address,
        uint32_t& o_numGranules);

/// @brief ensure that fabric is initialized and stop control is not set
///           (by checkstop/mode switch), which if set would prohibit fabric
///           commands from being broadcasted
/// @param[in] i_target => P9 chip target
/// @return FAPI_RC_SUCCESS if fabric is not stopped
    fapi2::ReturnCode p9_pba_coherent_utils_check_fbc_state(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target);

/// @brief check that the address is cacheline aligned and within the fabric real address range
/// @param[in] i_target => P9 chip target
/// @param[in] i_address => starting address for PBA operation
/// @return FAPI_RC_SUCCESS if arguments are valid
    fapi2::ReturnCode p9_pba_coherent_utils_check_args(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
        const uint64_t i_address);
/// @brief this checks the PBA/OCB status registers - this is for use at the end of each write/read or at the end of each stream
/// @return FAPI_RC_SUCCESS if the status check is a success
    fapi2::ReturnCode p9_pba_coherent_status_check(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target);

/// @brief this does any cleanup for the PBA after all reads/writes have been done
/// @param[in] i_target => P9 chip target
/// @return FAPI_RC_SUCCESS if cleaning up the PBA is a success
    fapi2::ReturnCode p9_pba_coherent_cleanup_pba(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target);

///@brief sets up the PBA Bar
///@param[in] i_target => P9 chip target
///@param[in] i_address => address for this read/write
///@return FAPI_RC_SUCCESS if writing the PBA is a success
    fapi2::ReturnCode p9_pba_coherent_setup_pba_bar(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
        const uint64_t i_baseAddress);

} //extern "C"

#endif //_P9_PBA_COHERENT_UTILS_H_

/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: chips/p9/procedures/ipl/sbe/p9_adu_access.H $                 */
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
//
/// @file p9_adu_access.H
/// @brief Read coherent state of memory via the ADU (FAPI)
///
// *HWP HWP Owner: Christina Graves clgraves@us.ibm.com
// *HWP FW Owner: Thi Tran thi@us.ibm.com
// *HWP Team: Nest
// *HWP Level: 1
// *HWP Consumed by:
// ----------------------------------------------------------------------------------
//
// *! ADDITIONAL COMMENTS :
// *!
// *! The purpose of this procedure is to perform a coherent read from system
// *! memory via fabric commands issued from the ADU.
// *!
// *! Succcessful operation assumes that:
// *!   o System clocks are running
// *!   o Fabric is initalized
// *!
// *!
//-----------------------------------------------------------------------------------

#ifndef _P9_ADU_ACCESS_H_
#define _P9_ADU_ACCESS_H_

//-----------------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------------

#include <fapi2.H>
#include <p9_adu_constants.H>
#include <p9_pba_constants.H>

//-----------------------------------------------------------------------------------
// Structure definitions
//-----------------------------------------------------------------------------------

//function pointer typedef definition for HWP call support
typedef fapi2::ReturnCode
(*p9_adu_access_FP_t) (const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>&,
                       const uint64_t,
                       const bool,
                       const uint32_t,
                       const bool,
                       const bool,
                       uint8_t[] );

//-----------------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------------

extern "C" {

//-----------------------------------------------------------------------------------
// Function prototype
//-----------------------------------------------------------------------------------

/// @brief do the actual read/write from the ADU
/// @param[in] i_target       => P9 chip target
/// @param[in] i_address      => base real address for read/write operation (expected to be 8B aligned)
/// @param[in] i_rnw          => if the operation is a read not write (1 for read, 0 for write)
/// @param[in] i_flags        => other information that is needed - the flags are:
/// @param[in] i_lastGranule  => if this is the last 8B of data that we are collecting (true = last granule, false = not last granule)
/// @param[in] i_firstGranule => if this is the first 8B of data that we are collecting (true = first granule, false = not first granule)
/// @param[in, out] io_data   => The data is read/written
/// @return FAPI_RC_SUCCESS if the read/write completes successfully
    fapi2::ReturnCode p9_adu_access(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
        const uint64_t i_address,
        const bool i_rnw,
        const uint32_t i_flags,
        const bool i_firstGranule,
        const bool i_lastGranule,
        uint8_t io_data[]);
} //extern "C"

#endif //_P9_ADU_ACCESS_H_


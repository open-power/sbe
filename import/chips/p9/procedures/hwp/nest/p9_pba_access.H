/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: chips/p9/procedures/hwp/nest/p9_pba_access.H $                */
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
/// @file p9_pba_access.H
/// @brief Read coherent state of memory via the PBA (FAPI)
///
// *HWP HWP Owner: Christina Graves clgraves@us.ibm.com
// *HWP FW Owner: Thi Tran thi@us.ibm.com
// *HWP Team: Nest
// *HWP Level: 2
// *HWP Consumed by: SBE
// ----------------------------------------------------------------------------------
//
// *! ADDITIONAL COMMENTS :
// *!
// *! The purpose of this procedure is to perform a coherent read or write from system
// *! memory via fabric commands issued from the PBA.
// *!
// *! Succcessful operation assumes that:
// *!   o System clocks are running
// *!   o Fabric is initalized
// *!
// *!
//-----------------------------------------------------------------------------------

#ifndef _P9_PBA_ACCESS_H_
#define _P9_PBA_ACCESS_H_

//-----------------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------------

#include <fapi2.H>
#include <p9_pba_constants.H>
//-----------------------------------------------------------------------------------
// Structure definitions
//-----------------------------------------------------------------------------------

//function pointer typedef definition for HWP call support
typedef fapi2::ReturnCode
(*p9_pba_access_FP_t) (const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>&,
                       const uint64_t,
                       const bool,
                       const uint32_t,
                       const bool,
                       const bool,
                       uint8_t[]);

//-----------------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------------

extern "C" {

//-----------------------------------------------------------------------------------
// Function prototype
//-----------------------------------------------------------------------------------

/// @brief setup for reads/writes from the PBA
/// @param[in]  i_target       => P9 chip target
/// @param[in]  i_address      => base real address for read/write operation (expected to be 128B aligned)
/// @param[in]  i_rnw          => if the operation is read not write (1 for read, 0 for write)
/// @param[in]  i_flags        => other information that is needed - the flags are: bit 1 - fastmode
/// @param[in]  i_lastGranule  => is this the last granule that is to be read/written
/// @param[in]  i_firstGranule => first granule that is to be read/written NOT USED FOR PBA
/// @param[in, out] io_data    => The data that is read/written
/// @return FAPI_RC_SUCCESS if the setup completes successfully,

    fapi2::ReturnCode p9_pba_access(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
        const uint64_t i_address,
        const bool i_rnw,
        const uint32_t i_flags,
        const bool i_firstGranule,
        const bool i_lastGranule,
        uint8_t io_data[]);
} //extern "C"

#endif //_P9_PBA_ACCESS_H_


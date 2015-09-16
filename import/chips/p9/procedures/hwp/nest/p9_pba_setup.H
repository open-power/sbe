/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: chips/p9/procedures/ipl/hwp/tests/p9_pba_setup.H $            */
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
//------------------------------------------------------------------------------------
//
/// @file p9_pba_setup.H
/// @brief Setup the PBA to do reads/writes
///
// *HWP HWP Owner: Christina Graves clgraves@us.ibm.com
// *HWP FW Owner: Thi Tran thi@us.ibm.com
// *HWP Team: Nest
// *HWP Level: 2
// *HWP Consumed by: SBE
// ----------------------------------------------------------------------------------
// *!
// *! ADDITIONAL COMMENTS:
// *!
// *! The purpose of this procedure is to setup the PBA to do reads/writes
// *! and to return the number of granules (number of 128B reads/writes) that
// *! can be done before setup needs to be called again
// *!
// *! Successful operation assumes that:
// *!
// *! High-level procedure flow:
// *!
// *!
// *!---------------------------------------------------------------------------------

#ifndef _P9_PBA_SETUP_H_
#define _P9_PBA_SETUP_H_

//-----------------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------------

#include <fapi2.H>
#include <p9_pba_constants.H>

//-----------------------------------------------------------------------------------
// Structure definitions
//-----------------------------------------------------------------------------------
typedef fapi2::ReturnCode
(*p9_pba_setup_FP_t) (const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>&,
                      const fapi2::Target<fapi2::TARGET_TYPE_EX>&,
                      const uint64_t,
                      const bool,
                      const uint32_t,
                      uint32_t&);

// Constant definitions
//-----------------------------------------------------------------------------------

extern "C" {

//-----------------------------------------------------------------------------------
// Function prototype
//-----------------------------------------------------------------------------------

/// @brief setup for reads/writes from the PBA
/// @param[in]  i_target  => P9 chip target
/// @param[in]  i_ex_target => P9 EX Target for use with lco_m operations
/// @param[in]  i_address => base real address for read/write operation (expected to be 128B aligned)
/// @param[in]  i_rnw     => if the operation is read not write (1 for read, 0 for write)
/// @param[in]  i_flags   => other information that is needed - the flags are:
///                          -bit 1 - fastmode,
/// @param[out] o_numGranules => number of 128B granules that can be read/written before setup needs to be called again
//
/// @return FAPI_RC_SUCCESS if the setup completes successfully,
//
    fapi2::ReturnCode p9_pba_setup(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
        const fapi2::Target<fapi2::TARGET_TYPE_EX>& i_ex_target,
        const uint64_t i_address,
        const bool i_rnw,
        const uint32_t i_flags,
        uint32_t& o_numGranules);
} //extern "C"

#endif //_P9_PBA_SETUP_H_

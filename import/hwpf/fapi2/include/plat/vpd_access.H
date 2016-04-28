/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: hwpf/fapi2/include/plat/vpd_access.H $                        */
/*                                                                        */
/* IBM CONFIDENTIAL                                                       */
/*                                                                        */
/* EKB Project                                                            */
/*                                                                        */
/* COPYRIGHT 2016                                                         */
/* [+] International Business Machines Corp.                              */
/*                                                                        */
/*                                                                        */
/* The source code for this program is not published or otherwise         */
/* divested of its trade secrets, irrespective of what has been           */
/* deposited with the U.S. Copyright Office.                              */
/*                                                                        */
/* IBM_PROLOG_END_TAG                                                     */
/// @file vpd_access.H
///
/// @brief VPD access functions that needs to be specialized for
/// platform implementation.
///

#ifndef __VPDACCESS_H_
#define __VPDACCESS_H_

#include <target.H>
#include <return_code.H>
#include <plat_vpd_access.H>
#include <fapi2_vpd_access.H>
namespace fapi2
{
// -- platform specializations for getVPD go here --

// specialization for MCA target
template<> fapi2::ReturnCode getVPD(const fapi2::Target<fapi2::TARGET_TYPE_MCA>& i_target,
                                    VPDInfo<fapi2::TARGET_TYPE_MCA>& io_vpd_info,
                                    uint8_t* o_blob)
{
    return platGetVPD( i_target, io_vpd_info, o_blob );
}

};

#endif // _FAPI2_VPDACCESS_H_

/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: hwpf/fapi2/include/plat/plat_vpd_access.H $                   */
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
/// @file plat_vpd_access.H
///
/// @brief Platform vpd-access definitions
///

#ifndef _FAPI2_PLAT_VPDACCESS_H_
#define _FAPI2_PLAT_VPDACCESS_H_

#include <fapi2_vpd_access.H>

namespace fapi2
{
// -- platform specific getVPD implementation goes here --

// platform specific for MCA target -- test code only
inline fapi2::ReturnCode platGetVPD(const fapi2::Target<fapi2::TARGET_TYPE_MCA>& i_target,
                                    VPDInfo<fapi2::TARGET_TYPE_MCA>& io_vpd_info,
                                    uint8_t* o_blob)
{
    fapi2::ReturnCode l_rc = fapi2::FAPI2_RC_SUCCESS;

    if( o_blob == nullptr )
    {
        io_vpd_info.iv_size = 2048;
    }
    else
    {
        o_blob[0] = 111;

        io_vpd_info.iv_freq_mhz = 10000;
        io_vpd_info.iv_rank_count_dimm_0 = 1;
        io_vpd_info.iv_rank_count_dimm_1 = 8;

    }

    return l_rc;
}

};
#endif // _FAPI2_PLAT_VPDACCESS_H_

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

/// @brief  Class representing required VPDInfo to be used in vpd collection.
/// @tparam T, the type of target this class is used with.
template<>
class VPDInfo<TARGET_TYPE_MCS>
{
    public:
        // @brief VPDInfo constructor
        VPDInfo( const fapi2::MemVpdData& i_vpd_type)
            : iv_vpd_type(i_vpd_type),
              iv_size(0), iv_freq(0), iv_rank_count_dimm_0(0),
              iv_rank_count_dimm_1(0), iv_dimm_drop_per_port(0)
        {};

        // size of the vpd data
        size_t   iv_size;
        uint64_t iv_freq;
        uint64_t iv_rank_count_dimm_0;
        uint64_t iv_rank_count_dimm_1;
        uint64_t iv_dimm_drop_per_port;

        // type of vpd field to return
        fapi2::MemVpdData_t iv_vpd_type;
};

};
#endif // _FAPI2_PLAT_VPDACCESS_H_

/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/hwpf/fapi2/include/plat/vpd_access.H $             */
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

/// @brief Return a blob of memory VPD data associated with the input target
/// @param[in] i_target, a valid fapi2 target
/// @param[in] io_vpd_info, fapi2::VPDInfo class that specifies which piece of data to return
/// @param[out] o_blob, the blob of raw data from the vpd
/// @return FAPI2_RC_SUCCESS if there's no problem
/// @note passing nullptr for o_blob will return the size of the keyword
///
/// Example:
/// fapi2::VPDInfo<fapi2::TARGET_TYPE_MCS> vpdInfo(i_target, MR_keyword);
/// vpdInfo.iv_speed_bin = 2400_SPEED_BIN;
///
/// FAPI_TRY(getVPD( mcs, vpdInfo, nullptr ));
/// char blob[vpdInfo.iv_size];
/// FAPI_TRY(getVPD( mcs, vpdInfo, blob ));
/// blob now contains the VPD data for the MCS.
///
template<> ReturnCode getVPD(const Target<TARGET_TYPE_MCS>& i_target,
                             VPDInfo<TARGET_TYPE_MCS>& io_vpd_info,
                             uint8_t* o_blob)
{
    ReturnCode l_rc = FAPI2_RC_SUCCESS;

    if( o_blob == nullptr )
    {
        io_vpd_info.iv_size = 2048;
    }
    else
    {
        o_blob[0] = 111;
        printf("io_vpd_info.iv_vpd_type = %d\n", io_vpd_info.iv_vpd_type);
        printf("io_vpd_info.iv_freq = %d\n", io_vpd_info.iv_freq);
        printf("io_vpd_info.iv_rank_count_dimm_0 = %d\n", io_vpd_info.iv_rank_count_dimm_0);
        printf("io_vpd_info.iv_rank_count_dimm_1 = %d\n", io_vpd_info.iv_rank_count_dimm_1);

    }

    return l_rc;
}
};

#endif // _FAPI2_VPDACCESS_H_

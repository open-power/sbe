/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/hwpf/fapi2/include/plat/plat_vpd_access.H $        */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2024                        */
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

// platform specific for MCS target -- test code only
#ifndef __PPE__ // MCS target type not needed for PPE , hence commenting to avoid including its dependencies.
inline fapi2::ReturnCode platGetVPD(const fapi2::Target<fapi2::TARGET_TYPE_MCS>& i_target,
                                    VPDInfo<fapi2::TARGET_TYPE_MCS>& io_vpd_info,
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
    }

    return l_rc;
}
#endif // __PPE__
// platform specific for OCMB target -- test code only
inline fapi2::ReturnCode platGetVPD(const fapi2::Target<fapi2::TARGET_TYPE_OCMB_CHIP>& i_target,
                                    VPDInfo<fapi2::TARGET_TYPE_OCMB_CHIP>& io_vpd_info,
                                    uint8_t* o_blob)
{
    fapi2::ReturnCode l_rc = fapi2::FAPI2_RC_SUCCESS;

    if( o_blob == nullptr )
    {
        io_vpd_info.iv_size = 128;
    }
    else
    {
        //-- Implementation Example/Suggestion for 'EFD' --
        // Check byte 2 for DDR4 or DDR5
        // Check bytes 205-206 of SPD for supported speeds
        // Look at EFD metadata in 277-415
        // Walk through all present EFD blocks looking for
        //   a match for speed, rank, channel/position
        // Copy entire EFD block into o_blob
        // Update output variables with the metadata that
        //   should be used to decode the block

        if( io_vpd_info.iv_omi_freq_mhz == 25600 )
        {
            o_blob[0] = 0x40;
        }
        else if( io_vpd_info.iv_omi_freq_mhz == 21330 )
        {
            o_blob[0] = 0x10;
        }

        io_vpd_info.iv_efd_type = 0x11;
        io_vpd_info.iv_dmb_mfg_id = 0x2980;
        io_vpd_info.iv_dmb_revision = 0x00;
        io_vpd_info.iv_ddr_mode = 0x0C;

    }

    return l_rc;
}

};
#endif // _FAPI2_PLAT_VPDACCESS_H_

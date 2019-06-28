/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/utils/contained/p10_contained_load.H $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2019,2020                        */
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

#ifndef _P10_CONTAINED_LOAD_H_
#define _P10_CONTAINED_LOAD_H_

#include <fapi2.H>

typedef fapi2::ReturnCode(*p10_contained_load_FP_t) (const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>&,
        const uint64_t&,
        const uint64_t&,
        const uint8_t*,
        const uint64_t&,
        const uint8_t*);

extern "C" {
    ///
    /// @brief Load test content for cache- and chip-contained mode IPLs
    ///        Optionally, load QME image in chip-contained mode
    /// @param[in] i_target             Reference to chip target
    /// @param[in] i_cache_start_addr   Start address for where to start loading cache image
    /// @param[in] i_cache_img_bytes    Size of the cache image to load in bytes (MUST be 128B aligned)
    /// @param[in] i_cache_img          Pointer to the cache image to load
    /// @param[in] i_qme_img_bytes      Size of the QME image to load in bytes
    /// @param[in] i_qme_img            Pointer to the QME image to load (or NULL)
    ///
    /// @return FAPI2_RC_SUCCESS if success else error code
    ///
    fapi2::ReturnCode p10_contained_load(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_chip,
                                         const uint64_t& i_cache_img_start_addr,
                                         const uint64_t& i_cache_img_bytes,
                                         const uint8_t* i_cache_img,
                                         const uint64_t& i_qme_img_bytes,
                                         const uint8_t* i_qme_img);
}

#endif

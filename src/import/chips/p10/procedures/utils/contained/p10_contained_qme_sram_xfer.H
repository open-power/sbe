/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/utils/contained/p10_contained_qme_sram_xfer.H $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2020                             */
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
#ifndef _P10_CONTAINED_QME_SRAM_XFER_H_
#define _P10_CONTAINED_QME_SRAM_XFER_H_

#include <fapi2.H>
#include <p10_scom_eq.H>
#include <p10_pm_utils.H>

///
/// @brief Setup QME SRAM access (QSAR)
/// @param[in] i_target    Reference to PERV EQ target
/// @param[in] i_addr      Full 32b QME SRAM address to access
/// @param[in] i_auto_incr Enable auto-increment mode when accessing QME_QSDR
///
/// @return FAPI2_RC_SUCCESS if success else error code
///
inline fapi2::ReturnCode qme_sram_xfer_setup(const fapi2::Target < fapi2::TARGET_TYPE_PERV |
        fapi2::TARGET_TYPE_MULTICAST > &i_target,
        const uint32_t i_addr, const bool i_auto_incr = true)
{
    using namespace scomt::eq;

    if (i_addr % 8)
    {
        FAPI_ERR("QME SRAM address is not 8B aligned: %08lx", i_addr);
        return fapi2::FAPI2_RC_FALSE;
    }

    // Writing to QME_QSAR clears the ACCESS_MODE bit in QME_QSCR - which means
    // we can just control the auto-increment using QME_QSAR[63] using a single
    // SCOM. Not sure if this is intended behavior or not <shrug>...
    FAPI_TRY(PUT_QME_QSAR(i_target, ((uint64_t)(i_addr) << 32) | i_auto_incr));

fapi_try_exit:
    return fapi2::current_err;
}

///
/// @brief Write to/read from QME SRAM via SCOM (QSDR)
/// @param[in] i_target Reference to PERV EQ target
/// @param[in] io_data  Data to write to / data read from QME SRAM
/// @param[in] i_rnw    Perform a read (read-not-write)
///
/// @return FAPI2_RC_SUCCESS if success else error code
///
inline fapi2::ReturnCode qme_sram_xfer_do_xfer(const fapi2::Target < fapi2::TARGET_TYPE_PERV |
        fapi2::TARGET_TYPE_MULTICAST > &i_target,
        uint64_t& io_data, const bool i_rnw = false)
{
    using namespace scomt::eq;

    if (i_rnw)
    {
        fapi2::buffer<uint64_t> tmp;
        FAPI_TRY(GET_QME_QSDR(i_target, tmp));
        io_data = tmp;
    }
    else
    {
        FAPI_TRY(PUT_QME_QSDR(i_target, io_data));
    }

fapi_try_exit:
    return fapi2::current_err;
}

///
/// @brief Copy data in 8B chunks to the QME SRAM
/// @param[in] i_target Reference to PERV EQ target
/// @param[in] i_data   Data buffer to copy
/// @param[in] i_size   Size (in bytes) to copy, must be multiple of 8B
/// @param[in] i_addr   First QME SRAM address to copy to
/// @param[in] i_swap   Perform endian swap on LE machines for each 8B chunk
///                     before copying
///
/// @return FAPI2_RC_SUCCESS if success else error code
///
inline fapi2::ReturnCode qme_sram_xfer_copy(const fapi2::Target < fapi2::TARGET_TYPE_PERV |
        fapi2::TARGET_TYPE_MULTICAST > &i_target,
        const uint64_t* i_data, const uint32_t i_size,
        const uint64_t i_addr, const bool i_swap = true)
{
    uint64_t tmp;

    FAPI_TRY(qme_sram_xfer_setup(i_target, i_addr));

    if (i_size % 8)
    {
        FAPI_ERR("Data size is not a multiple of 8B: %d", i_size);
        return fapi2::FAPI2_RC_FALSE;
    }

    for (size_t blk = 0; blk < i_size; blk += 8)
    {
        tmp = *i_data++;
        tmp = (i_swap) ? revle64(tmp) : tmp;
        FAPI_TRY(qme_sram_xfer_do_xfer(i_target, tmp));
    }

fapi_try_exit:
    return fapi2::current_err;
}

#endif

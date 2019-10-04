/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_scan_via_scom.H $ */
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
#ifndef _P10_SCAN_VIA_SCOM_H_
#define _P10_SCAN_VIA_SCOM_H_

#include <fapi2.H>
#include <p10_scom_perv.H>
#include <hwp_data_stream.H>

namespace
{
/**
 * @brief Wait for OPCG_DONE after a long rotate
 * @param[in] The target chiplet to wait on
 * @return FAPI2_RC_SUCCESS          if all went well
 *         RC_SCAN_VIA_SCOM_TIMEOUT  if OPCG_DONE didn't turn on within 100ms
 *         another FAPI2 error       if any of the SCOM operations failed
 */
inline fapi2::ReturnCode _svs_wait_opcg_done(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target)
{
    fapi2::buffer<uint64_t> buf;
    uint32_t timeout = 10000;

    while (timeout--)
    {
        fapi2::delay(10000, 100000);
        FAPI_TRY(fapi2::getScom(i_target, scomt::perv::CPLT_STAT0, buf), "Failed to read chiplet status 0");

        if (buf.getBit<scomt::perv::CPLT_STAT0_CC_CTRL_OPCG_DONE_DC>())
        {
            return fapi2::FAPI2_RC_SUCCESS;
        }
    }

    FAPI_ASSERT(false, fapi2::SCAN_VIA_SCOM_TIMEOUT().set_TARGET(i_target));

fapi_try_exit:
    return fapi2::current_err;
}

/**
 * @brief Initiate a long rotate operation and wait for it to complete.
 * @param[in] i_target The target chiplet
 * @param[in] i_rotate Amount of bits to rotate by; min 0, max LONG_ROTATE_MAX()
 * @return FAPI2_RC_SUCCESS or error code of a sub-operation
 */
inline fapi2::ReturnCode _svs_long_rotate(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target,
    uint32_t i_rotate)
{
    fapi2::buffer<uint64_t> buf = 0;
    buf.insertFromRight<12, 20>(i_rotate);
    FAPI_TRY(fapi2::putScom(i_target, 0x039000, buf), "Failed to initiate long rotate");
    FAPI_TRY(_svs_wait_opcg_done(i_target));

fapi_try_exit:
    return fapi2::current_err;
}
} // anon. namespace

namespace svs
{
/**
 * @brief Limits of the clock controller
 * The rotate amount can go up to FFF, but we'd run into PIB timeouts at too high values if the core is running slowly
 * Looking at the rotate amounts generated on the P9DD1 core, a limit of ~1K will make almost no difference versus
 * 4K or 8K, so pick that.
 *
 * TODO Convert to `inline constexpr` variables if/when we decide to compile with c++17.
 *      The `inline constexpr` function thing is a c++11 smell to prevent a unique instance
 *      of these variables per translation-unit.
 */
inline constexpr uint32_t SCAN_ROTATE_MAX()
{
    return 0x380;
}
inline constexpr uint32_t LONG_ROTATE_MAX()
{
    return ((1 << 20) - 1);
}

/**
 * @brief Low-level scan-via-scom read: rotate and return the 64bit scan buffer
 * @param[in]  i_target The target to operate on
 * @param[in]  i_rotate Rotate by this many bits; min 0, max SCAN_ROTATE_MAX()
 * @param[out] o_data   Resulting scan buffer data
 * @return the FAPI2 return code of the SCOM operation
 */
inline fapi2::ReturnCode scan64_get(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target,
    uint32_t i_rotate, fapi2::buffer<uint64_t>& o_data)
{
    return fapi2::getScom(i_target, 0x03E000 | i_rotate, o_data);
}

/**
 * @brief Low-level scan-via-scom write: set the 64bit scan buffer and rotate
 * @param[in] i_target The target to operate on
 * @param[in] i_data   Data to write into the scan buffer. If i_rotate is < 64, only the first i_rotate bits will be written
 * @param[in] i_rotate Rotate by this many bits; min 0, max SCAN_ROTATE_MAX()
 * @return the FAPI2 return code of the SCOM operation
 */
inline fapi2::ReturnCode scan64_put(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target,
    const fapi2::buffer<uint64_t> i_data, uint32_t i_rotate)
{
    return fapi2::putScom(i_target, 0x03E000 | i_rotate, i_data);
}

/*
 * To minimize the amount of necessary SCOMs, scan_out() and rotate() work
 * in lockstep -- scan_out() doesn't do the last rotate but returns the extra
 * number of bits to rotate in the next rotate operation. Conversely, rotate()
 * returns the scan buffer after rotation, which already contains the first 64
 * bits of the data scan_out() is interested in.
 */

/**
 * @brief Scan out N bits of data
 * @param[in]  i_target       The target chiplet
 * @param[in]  i_nbits        Amount of bits to scan out
 * @param[out] o_stream       Bit stream to append scanned data to
 * @param[in]  i_data         Scan data buffer to use, pre-filled with current scan buffer contents
 * @param[out] o_extra_rotate Remainder of i_nbits not yet rotated, for deferring to rotate()
 * @return FAPI2 return code
 */
inline fapi2::ReturnCode scan_out(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target,
    uint32_t i_nbits, fapi2::hwp_bit_ostream& o_stream,
    fapi2::buffer<uint64_t>& i_data, uint32_t& o_extra_rotate)
{
    while (1)
    {
        FAPI_TRY(o_stream.put(i_data, std::min(i_nbits, (uint32_t)64), true),
                 "Failed to place scanned data into the output buffer");

        if (i_nbits <= 64)
        {
            o_extra_rotate = i_nbits;
            return fapi2::FAPI2_RC_SUCCESS;
        }

        i_nbits -= 64;
        FAPI_TRY(scan64_get(i_target, 64, i_data), "Failed to perform a scan read operation");
    }

fapi_try_exit:
    return fapi2::current_err;
}

/**
 * @brief Rotate ring by N bits
 * @param[in]  i_target The target chiplet
 * @param[in]  i_nbits  Amount of bits to rotate by
 * @param[out] o_data   Scan data buffer after rotate
 * @return FAPI2 return code
 */
inline fapi2::ReturnCode rotate(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target,
    uint32_t i_nbits, fapi2::buffer<uint64_t>& o_data)
{
    /* Get the # remaining bits down to a sensible range */
    while (i_nbits > LONG_ROTATE_MAX())
    {
        FAPI_TRY(_svs_long_rotate(i_target, LONG_ROTATE_MAX()));
        i_nbits -= LONG_ROTATE_MAX();
    }

    /* If we're outside the range of normal scan, do long rotate and just read the buffer next without rotating */
    if (i_nbits > SCAN_ROTATE_MAX())
    {
        FAPI_TRY(_svs_long_rotate(i_target, i_nbits));
        i_nbits = 0;
    }

    /* Rotate the remainder and get the buffer for the following scan's first data */
    FAPI_TRY(scan64_get(i_target, i_nbits, o_data), "Failed to read scan data");
    return fapi2::FAPI2_RC_SUCCESS;

fapi_try_exit:
    return fapi2::current_err;
}

/**
 * @brief Scan in N bits of data
 * XXX this isn't super useful as-is, but it *does* make the scom-via-scan API "complete"
 * @param[in]  i_target       The target chiplet
 * @param[in]  i_nbits        Amount of bits to scan in
 * @param[in]  i_stream       Bit stream of data to stream in sequentially
 * @return FAPI2 return code
 */
inline fapi2::ReturnCode scan_in(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target,
    uint32_t i_nbits, fapi2::hwp_bit_istream& i_stream)
{
    uint64_t tmp = 0;

    while (1)
    {
        FAPI_TRY(i_stream.get(tmp, std::min(i_nbits, (uint32_t)64), true),
                 "Failed to read scan data from input buffer");

        if (i_nbits <= 64)
        {
            FAPI_TRY(scan64_put(i_target, i_nbits, tmp), "Failed to perform scan put operation");
            return fapi2::FAPI2_RC_SUCCESS;
        }

        i_nbits -= 64;
        FAPI_TRY(scan64_put(i_target, 64, tmp), "Failed to perform scan put operation");
    }

fapi_try_exit:
    return fapi2::current_err;
}

} // namespace svs
#endif//_P10_SCAN_VIA_SCOM_H_

/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_sbe_fastarray.H $ */
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
//------------------------------------------------------------------------------
/// @file  p10_sbe_fastarray.H
///
/// @brief SBE part of fast array dump: Sets up the ABIST engines, clocks ABIST
///        cycles and dumps the interesting bits of the FARR ring back to the
///        service processor.
///        The information about which bits are interesting during which ABIST
///        cycle is supplied by the caller.
//------------------------------------------------------------------------------
// *HWP HW Owner        : Joachim Fenkes <fenkes@de.ibm.com>
// *HWP HW Backup Owner : Joe McGill <jmcgill@us.ibm.com>
// *HWP FW Owner        : Raja Das <rajadas2@in.ibm.com>
// *HWP Team            : Chip
// *HWP Level           : 3
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------

#ifndef _P10_SBE_FASTARRAY_H_
#define _P10_SBE_FASTARRAY_H_

#include <fapi2.H>
#include <fapi2_subroutine_executor.H>
#include <hwp_data_stream.H>

typedef fapi2::ReturnCode (*p10_sbe_fastarray_FP_t)(
    const fapi2::Target < fapi2::TARGET_TYPE_PERV | fapi2::TARGET_TYPE_CORE > & i_target,
    fapi2::hwp_data_istream&  i_instructions,
    fapi2::hwp_data_ostream&  o_dump_data);

/**
 * @brief Extract a raw fast array dump out of a given chiplet
 *
 * This will go through the entire process of setting up the chiplet's ABIST engines for fast array readout,
 * clocking ABIST cycles and dumping out the interesting bits of the FARR ring. It will also clean up after
 * itself, so that after successful return from this procedure the same chiplet can be dumped again and
 * should yield the exact same data.
 *
 * The data returned by this procedure is raw ring data as it was scanned out of the FARR ring, with no
 * inversion mask applied yet and array data still in the order the observation latches are arranged on
 * the ring in. It will need to be post processed to yield usable array content.
 *
 * The procedure needs some control data to perform its duty; that control data is streamed into the
 * procedure, either from static data compiled into the SBE binary, or from outside the SBE if custom
 * control is required. Likewise, the data that is dumped out of the chiplet is returned in another stream.
 *
 * @param[in]  i_target            The chiplet to be dumped
 * @param[in]  i_instructions      Control data for the procedure
 * @param[out] o_dump_data         Raw fast array dump data that will require offline post processing
 *
 * @return the usual FAPI return codes
 *
 *
 * Input stream format description:
 *
 * The first 32-bit word in the input stream is the ring address of the ring to be dumped, in the format
 * we all know and hate.
 *
 * The rest of the input stream consists of (ncycles, nwords, carebits) tuples:
 *   The first word (the "header") has ncycles in the upper halfword and nwords in the lower halfword
 *   The header is followed by a number of words equal to nwords; these words make up the carebits information
 *
 * The carebits block can be decoded by hwp_bit_istream and consists of a series of variable width integers,
 * terminated by a zero value. The values represent alternate run lengths of don't care and care bits,
 * starting with don't care.
 *
 * For each tuple, perform the step/dump process ncycles times. If nwords is zero, skip that many cycles.
 * If ncycles is zero, the input stream is finished and no further dump data is required.
 *
 *
 * Output stream format description:
 *
 * The output stream format is identical to the input stream format minus the initial ring address word, except
 * after each tuple the read data is appended in a separate byte_ostream. For each run of care bits, that many
 * bits are appended to the stream, and at the end of each row, the number of bits is zero-padded to a byte boundary.
 *
 * This way, the output stream contains all necessary information about its own composition, and the offline
 * post processor can decode it without knowledge of the SBE image that produced it.
 */
extern "C"
{
    fapi2::ReturnCode p10_sbe_fastarray(
        const fapi2::Target < fapi2::TARGET_TYPE_PERV | fapi2::TARGET_TYPE_CORE > & i_target,
        fapi2::hwp_data_istream&  i_instructions,
        fapi2::hwp_data_ostream&  o_dump_data);
}

#endif

/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/utils/p9_putRingUtils.H $                 */
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
/// @file p9_putRingUtils.H
/// @brief Headers and Constants used by rs4 decompression and
/// ring SCAN/ROTATE functionality
///
// *HWP HWP Owner: Bilicon Patil <bilpatil@in.ibm.com>
// *HWP FW Owner: Prasad Ranganath <prasadbgr@in.ibm.com>
// *HWP Team: PM
// *HWP Level: 2
// *HWP Consumed by: SBE:CME:SGPE:PGPE

#ifndef _P9_PUTRINGUTILS_H_
#define _P9_PUTRINGUTILS_H_

#include <return_code.H>
#include <fapi2.H>

namespace RS4
{

//
// Forward Declarations
//

///
/// @brief Decode an unsigned integer from a 4-bit octal stop code.
/// @param[in] i_rs4Str The RS4 scan string
/// @param[in] i_nibbleIndx Index into i_rs4Str that has the stop-code
/// @param[out] o_numRotate No.of rotates decoded from the stop-code.
/// @return The number of nibbles decoded.
///
uint64_t stop_decode(const uint8_t* i_rs4Str,
                     uint32_t i_nibbleIndx,
                     uint64_t& o_numRotate);

///
/// @brief Return a big-endian-indexed nibble from a byte string
/// @param[in] i_rs4Str The RS4 scan string
/// @param[in] i_nibbleIndx Index into i_rs4Str that need to converted
///                         into a nibble
/// @return big-endian-indexed nibble
///
uint8_t rs4_get_nibble(const uint8_t* i_rs4Str, const uint32_t i_nibbleIndx);

///
/// @brief Return verbatim data from the RS4 string
/// @param[in] i_rs4Str The RS4 scan string
/// @param[in] i_nibbleIndx Index into i_rs4Str that need to converted
///                         into a nibble
/// @param[in] i_nibbleCount The count of nibbles that need to be put
///                          in the return value.
/// @return big-endian-indexed double word
///
uint64_t rs4_get_verbatim(const uint8_t* i_rs4Str,
                          const uint32_t i_nibbleIndx,
                          const uint8_t i_nibbleCount);

}; // end of RS4 namespace

using namespace RS4;
//
// Constants and Structures
//
struct restoreOpcgRegisters
{
    fapi2::buffer<uint64_t> l_opcgReg0;
    fapi2::buffer<uint64_t> l_opcgReg1;
    fapi2::buffer<uint64_t> l_opcgReg2;
    fapi2::buffer<uint64_t> l_opcgCapt1;
    fapi2::buffer<uint64_t> l_opcgCapt2;
    fapi2::buffer<uint64_t> l_opcgCapt3;
};


#define SIXTYFOUR_BIT_HEADER 64

/// @brief Constants for operations performed by putRing function.
enum opType_t
{
    ROTATE = 0,  ///< Indicates a Rotate operation on the ring
    SCAN = 1     ///< Indicates a Scan operation on the ring
};

/// @brief Constants for the type of Scans supported by putRing
enum scanType_t
{
    STANDARD_SCAN = 1,  ///< used in SBE Plat
    QUEUED_SCAN   = 2,  ///< used in CME plat
    POLLED_SCAN   = 3   ///< used in SGPE plat
};

//
// Function Definitions
//
using namespace RING_TYPES;
void getRingProperties(const RingID i_ringId,
                       uint32_t& o_torOffset,
                       RINGTYPE& o_ringType,
                       CHIPLET_TYPE& o_type);

/// @brief Function to apply the Ring data using the standard-scan method
/// @param[in] i_target Chiplet Target of Scan
//  @param[in] i_chipletId data from RS4
/// @param[in] i_operation Type of operation to perform - ROTATE/SCAN
/// @param[in] i_opVal Number of bits for the operation
/// @param[in] i_scanData This value has to be scanned when i_operation is SCAN
/// @return FAPI2_RC_SUCCESS if success, else error code.
fapi2::ReturnCode standardScan(
    const fapi2::Target<fapi2::TARGET_TYPE_ALL>& i_target,
    const uint8_t i_chipletId,
    opType_t i_operation,
    uint64_t i_opVal,
    uint64_t i_scanData = 0);

/// @brief Function to set the Scan Region
/// @param[in] i_target Chiplet Target of Scan
/// @param[in] i_scanRegion Value to be set to select a Scan Region
//  @param[in] i_chipletId data from RS4
/// @return FAPI2_RC_SUCCESS if success, else error code.
fapi2::ReturnCode setupScanRegion(const fapi2::Target<fapi2::TARGET_TYPE_ALL>&
                                  i_target,
                                  uint64_t i_scanRegion,
                                  const uint8_t i_chipletId,
                                  const RINGTYPE i_ringType = COMMON_RING);


/// @brief Function to write the header data to the ring.
/// @param[in] i_target Chiplet Target of Scan
/// @param[in] i_header The header data that is to be written.
//  @param[in] i_chipletId data from RS4
/// @return FAPI2_RC_SUCCESS if success, else error code.
fapi2::ReturnCode writeHeader(const fapi2::Target<fapi2::TARGET_TYPE_ALL>&
                              i_target,
                              const uint64_t i_header,
                              const uint8_t i_chipletId);


/// @brief Function to reader the header data from the ring and verify it.
/// @param[in] i_target Chiplet Target of Scan
/// @param[in] i_header The header data that is expected.
//  @param[in] i_chipletId data from RS4
//  @param[in] i_ringMode differnet ring mode operations
/// @return FAPI2_RC_SUCCESS if success, else error code.
fapi2::ReturnCode verifyHeader(const fapi2::Target<fapi2::TARGET_TYPE_ALL>&
                               i_target,
                               const uint64_t i_header,
                               const uint8_t i_chipletId,
                               const fapi2::RingMode i_ringMode);


/// @brief Function to decompress the RS4 and apply the Ring data
/// @param[in] i_target Chiplet Target of Scan
/// @param[in] i_rs4 The RS4 compressed string
//  @param[in] i_applyOverride: state of the override mode
//  @param[in] i_ringType : Ring type Instance /Common
//  @param[in] i_ringMode differnet ring modes operations
/// @return FAPI2_RC_SUCCESS if success, else error code.
fapi2::ReturnCode rs4DecompressionSvc(
    const fapi2::Target<fapi2::TARGET_TYPE_ALL>& i_target,
    const uint8_t* i_rs4,
    bool i_applyOverride,
    const fapi2::RingMode i_ringMode,
    const RINGTYPE i_ringType = COMMON_RING);


/// @brief Function to clean up the scan region and type
/// @param[in] i_target Chiplet Target of Scan
//  @param[in] chipletId data from RS4
/// @return FAPI2_RC_SUCCESS if success, else error code.
fapi2::ReturnCode cleanScanRegionandTypeData(
    const fapi2::Target<fapi2::TARGET_TYPE_ALL>& i_target,
    const uint8_t i_chipletId);

/// @brief Function to restore the opcg registers
/// @param[in] i_target Chiplet Target of Scan
/// @param[in] i_OPCGData opcg register data to restore original values
//  @param[in] chipletId data from RS4
/// @return FAPI2_RC_SUCCESS if success, else error code.
fapi2::ReturnCode restoreOPCGRegData(
    const fapi2::Target<fapi2::TARGET_TYPE_ALL>& i_target,
    struct restoreOpcgRegisters i_OPCGData,
    const uint8_t i_chipletId);

/// @brief Function to restore the opcg registers
/// @param[in] i_target Chiplet Target of Scan
/// @param[out]o_OPCGData Structure that contains opcg data
//  @param[in] i_chipletId data from RS4
/// @return FAPI2_RC_SUCCESS if success, else error code.
fapi2::ReturnCode storeOPCGRegData(
    const fapi2::Target<fapi2::TARGET_TYPE_ALL>& i_target,
    struct restoreOpcgRegisters& o_OPCGData,
    const uint8_t i_chipletId);

/// @brief Function to set the Scan Region for set pulse mode
/// @param[in] i_target Chiplet Target of Scan
/// @param[in] i_scanRegion Value to be set to select a Scan Region
//  @param[in] i_ringMode differnet ring mode operations
//  @param[in] i_chipletId data from RS4
/// @return FAPI2_RC_SUCCESS if success, else error code.
fapi2::ReturnCode setupScanRegionForSetPulse(
    const fapi2::Target<fapi2::TARGET_TYPE_ALL>& i_target,
    uint64_t i_scanRegion,
    const fapi2::RingMode i_ringMode,
    const uint8_t i_chipletId);


#endif

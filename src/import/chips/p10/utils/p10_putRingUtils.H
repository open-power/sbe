/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/utils/p10_putRingUtils.H $               */
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
/// @file p10_putRingUtils.H
/// @brief Headers and Constants used by rs4 decompression and
/// ring SCAN/ROTATE functionality
///
// *HWP HW Owner:       Greg Still  <stillgs@us.ibm.com>
// *HWP FW Owner:       Prem S Jha  <premjha2@in.ibm.com>
// *HWP Team:           PM
// *HWP Level:          2
// *HWP Consumed by:    SBE

// *INDENT-OFF*

#ifndef _P10_PUTRINGUTILS_H_
#define _P10_PUTRINGUTILS_H_

#include <p10_plat_ring_traverse.H>

// Constants and Structures
//
struct restoreOpcgRegisters
{
    fapi2::buffer<uint64_t> l_opcgAlign;
    fapi2::buffer<uint64_t> l_opcgReg0;
    fapi2::buffer<uint64_t> l_opcgReg1;
    fapi2::buffer<uint64_t> l_opcgReg2;
    fapi2::buffer<uint64_t> l_opcgCapt1;
    fapi2::buffer<uint64_t> l_opcgCapt2;
    fapi2::buffer<uint64_t> l_opcgCapt3;
};
typedef union  __attribute__((__packed__)) rs4_data
{
    uint32_t iv_data;
    struct
    {
        uint8_t val[4];
    } fields;
} rs4_data_t;

namespace RS4
{
#define SIXTYFOUR_BIT_HEADER 64

/// @brief Constants for operations performed by putRing function.
enum opType_t
{
    ROTATE      =   0,  ///< Indicates a Rotate operation on the ring
    SCAN        =   1   ///< Indicates a Scan operation on the ring
};

/// @brief Constants for the type of Scans supported by putRing
enum scanType_t
{
    STANDARD_SCAN   =   1,  ///< used in SBE Plat
    QUEUED_SCAN     =   2,  ///< used in CME plat
    POLLED_SCAN     =   3   ///< used in SGPE plat
};


/// @brief scan region supported by putring through 32 bit vector.
enum ScanRegion_t
{
    REGION_RESERVE      =   0xE0000000,
    REGION_VITL         =   0x10000000,
    REGION_PERV         =   0x08000000,
    REGION_ECL0         =   0x04000000,
    REGION_ECL1         =   0x02000000,
    REGION_ECL2         =   0x01000000,
    REGION_ECL3         =   0x00800000,
    REGION_L30          =   0x00400000,
    REGION_L31          =   0x00200000,
    REGION_L32          =   0x00100000,
    REGION_L33          =   0x00080000,
    REGION_QME          =   0x00040000,
    REGION_CLKADJ       =   0x00020000,
    REGION_MMA0         =   0x00010000,
    REGION_MMA1         =   0x00008000,
    REGION_MMA2         =   0x00004000,
    REGION_MMA3         =   0x00002000,
};

/**
 * @brief  helper function to swizzle given input data
 * @note   swizles bytes to handle endianess issue.
 */
#ifdef _BIG_ENDIAN

// NOP if it is a big endian system
#define rev_16(WORD) WORD
#define rev_32(WORD) WORD
#define rev_64(WORD) WORD

#else
#define rev_16(WORD) \
    ( (((WORD) >> 8) & 0x00FF) | (((WORD) << 8) & 0xFF00) )

#define rev_32(WORD) \
    ( (((WORD) >> 24) & 0x000000FF) | (((WORD) >>  8) & 0x0000FF00) | \
      (((WORD) <<  8) & 0x00FF0000) | (((WORD) << 24) & 0xFF000000) )

#define rev_64(WORD) \
    ( (((WORD) >> 56) & 0x00000000000000FF) |  \
      (((WORD) >> 40) & 0x000000000000FF00) |  \
      (((WORD) >> 24) & 0x0000000000FF0000) |  \
      (((WORD) >>  8) & 0x00000000FF000000) |  \
      (((WORD) <<  8) & 0x000000FF00000000) |  \
      (((WORD) << 24) & 0x0000FF0000000000) |  \
      (((WORD) << 40) & 0x00FF000000000000) |  \
      (((WORD) << 56) & 0xFF00000000000000) )
#endif
} //namespace RS4
extern "C"
{

typedef fapi2::ReturnCode( *p10_putRingUtils_FP_t )
( const fapi2::Target<fapi2::TARGET_TYPE_ALL_MC>& i_target,
  const uint8_t* i_rs4,
  const MyBool_t i_applyOverride,
  const fapi2::RingMode i_ringMode,
  const RingType_t i_ringType,
  RS4::OpMode_t    i_opMode );

/// @brief Function to decompress the RS4 and apply the Ring data
/// @param[in] i_target         Chiplet Target of Scan
/// @param[in] i_rs4            The RS4 compressed string
/// @param[in] i_applyOverride  state of the override mode 0:flush, 1:override, 0xff:use iv_type
/// @param[in] i_ringMode       different ring access operation mode
/// @param[in] i_ringType       Ring type Instance /Common
/// @param[in] i_opMode         operational mode of putring
/// @return FAPI2_RC_SUCCESS if success, else error code.
fapi2::ReturnCode p10_putRingUtils(
    const fapi2::Target<fapi2::TARGET_TYPE_ALL_MC>& i_target,
    const uint8_t* i_rs4,
    bool i_applyOverride,
    const fapi2::RingMode i_ringMode,
    const RingType_t i_ringType,
    RS4::OpMode_t    i_opMode );

//
// Forward Declarations
//
///
/// @brief Decode an unsigned integer from a 4-bit octal stop code.
/// @param[in]   i_rs4Str         The RS4 scan string
/// @param[in]   i_nibbleIndx     Index into i_rs4Str that has the stop-code
/// @param[out]  o_numRotate      No.of rotates decoded from the stop-code.
/// @return      The number of nibbles decoded.
///
uint64_t stop_decode( const uint8_t* i_rs4Str,
                      uint32_t i_nibbleIndx,
                      uint64_t& o_numRotate );

///
/// @brief Return verbatim data from the RS4 string
/// @param[in] i_rs4Str         The RS4 scan string
/// @param[in] i_nibbleIndx     Index into i_rs4Str that need to be converted into a nibble
/// @param[in] i_nibbleCount    The count of nibbles that need to be put in the return value.
/// @return big-endian-indexed  double word
///
uint64_t rs4_get_verbatim( const uint8_t* i_rs4Str,
                           const uint32_t i_nibbleIndx,
                           const uint8_t i_nibbleCount );

///
/// @brief Return a big-endian-indexed nibble from a byte string
/// @param[in] i_rs4Str     The RS4 scan string
/// @param[in] i_nibbleIndx Index into i_rs4Str that need to converted into a nibble
/// @return big-endian-indexed nibble
///
inline uint8_t rs4_get_nibble( const uint8_t* i_rs4Str, const uint32_t i_nibbleIndx )
{
    uint8_t l_byte = i_rs4Str[i_nibbleIndx >> 1];

    return ((i_nibbleIndx % 2) ? (l_byte & 0x0f) : (l_byte >> 4));
}

/// @brief Function to apply the Ring data using the standard-scan method
/// @param[in] i_target         Chiplet Target of Scan
/// @param[in] i_operation      Type of operation to perform - ROTATE/SCAN
/// @param[in] i_opVal          Number of bits for the operation
/// @param[in] i_chipletMask    chiplet id bit shifted by 24 position
/// @param[in] i_scanData       This value has to be scanned when i_operation is SCAN
/// @param[in] i_ringType       COMMON or INSTANCE_RING
/// @param[in] i_ringId         Id associated with ring to be scanned
/// @param[in] i_opMode         mode of operation for scanning code.
/// @return FAPI2_RC_SUCCESS if success, else error code.
fapi2::ReturnCode standardScan(
    const fapi2::Target<fapi2::TARGET_TYPE_ALL_MC>& i_target,
    RS4::opType_t   i_operation,
    uint64_t        i_opVal,
    uint32_t        i_chipletMask,
    uint64_t        i_scanData  =   0,
    const RingType_t i_ringType =   COMMON_RING,
    const uint16_t  i_ringId    =   0,
    RS4::OpMode_t   i_opMode    =   RS4::SCANNING_MODE );

/// @brief Function to write the header data to the ring.
/// @param[in] i_target         Chiplet Target of Scan
/// @param[in] i_header         The header data that is to be written.
/// @param[in] i_ringType       COMMON or INSTANCE_RING
/// @param[in] i_chipletMask    chiplet id bit shifted by 24 position
/// @return FAPI2_RC_SUCCESS if success, else error code.
fapi2::ReturnCode writeHeader( const fapi2::Target<fapi2::TARGET_TYPE_ALL_MC>& i_target,
                               const uint64_t i_header,
                               const RingType_t i_ringType,
                               uint32_t         i_chipletMask );


/// @brief Function to reader the header data from the ring and verify it.
/// @param[in] i_target         Chiplet Target of Scan
/// @param[in] i_ringMode       different ring access operation mode
/// @param[in] i_scanRegion     64 bit scan region type value
/// @param[in] i_ringId         Id associated with ring to be scanned
/// @param[in] i_chipletMask    chiplet id bit shifted by 24 position
/// @param[in] i_bitsDecoded    number of bits rotated and scanned
/// @param[in] i_ringType       COMMON or INSTANCE_RING
/// @param[in] i_opMode         mode of operation for scanning code.
/// @return FAPI2_RC_SUCCESS if success, else error code.
fapi2::ReturnCode verifyHeader( const fapi2::Target<fapi2::TARGET_TYPE_ALL_MC>& i_target,
                                const fapi2::RingMode i_ringMode,
                                uint64_t       i_scanRegion,
                                const uint16_t i_ringId,
                                uint32_t       i_chipletMask,
                                const uint32_t i_bitsDecoded =  0,
                                const RingType_t i_ringType  =  COMMON_RING,
                                RS4::OpMode_t i_opMode       =  RS4::SCANNING_MODE );

} //extern "C"

#endif  //_P10_PUTRINGUTILS_H_

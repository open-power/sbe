/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/utils/p9_putRingUtils.C $                 */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016                             */
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
/// @file p9_putRingUtils.C
/// @brief Provide the service of decompressing the rs4 encoded string.
///
// *HWP HWP Owner: Bilicon Patil <bilpatil@in.ibm.com>
// *HWP FW Owner: Prasad Ranganath <prasadbgr@in.ibm.com>
// *HWP Team: PM
// *HWP Level: 2
// *HWP Consumed by: SBE:CME:SGPE:PGPE

#include <p9_putRingUtils.H>
#include <p9_scan_compression.H>

using namespace RING_TYPES;
namespace RS4
{
//
// Function Definitions
//

///
/// @brief Return a big-endian-indexed nibble from a byte string
/// @param[in] i_rs4Str The RS4 scan string
/// @param[in] i_nibbleIndx Index into i_rs4Str that need to converted
///                         into a nibble
/// @return big-endian-indexed nibble
///
uint8_t rs4_get_nibble(const uint8_t* i_rs4Str, const uint32_t i_nibbleIndx)
{
    uint8_t l_byte;
    uint8_t l_nibble;

    l_byte = i_rs4Str[i_nibbleIndx / 2];


    if(i_nibbleIndx % 2)
    {
        l_nibble = (l_byte & 0x0f);
    }
    else
    {
        l_nibble = (l_byte >> 4);
    }

    return l_nibble;
}

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
                          const uint8_t i_nibbleCount)
{
    uint8_t l_byte;
    uint8_t l_nibble;
    uint64_t l_doubleWord = 0;

    uint32_t l_index = i_nibbleIndx;

    for(uint8_t i = 1; i <= i_nibbleCount; i++, l_index++)
    {
        l_byte = i_rs4Str[l_index / 2];

        if(l_index % 2)
        {
            l_nibble = (l_byte & 0x0f);
        }
        else
        {
            l_nibble = (l_byte >> 4);
        }

        uint64_t l_tempDblWord = l_nibble;
        l_tempDblWord <<= (64 - (4 * i));

        l_doubleWord |= l_tempDblWord;
    }

    return l_doubleWord;
}

///
/// @brief Decode an unsigned integer from a 4-bit octal stop code.
/// @param[in] i_rs4Str The RS4 scan string
/// @param[in] i_nibbleIndx Index into i_rs4Str that has the stop-code
/// @param[out] o_numRotate No.of rotates decoded from the stop-code.
/// @return The number of nibbles decoded.
///
uint64_t stop_decode(const uint8_t* i_rs4Str,
                     uint32_t i_nibbleIndx,
                     uint64_t& o_numRotate)
{
    uint64_t l_numNibblesParsed = 0; // No.of nibbles that make up the stop-code
    uint64_t l_numNonZeroNibbles = 0;
    uint8_t l_nibble;

    do
    {
        l_nibble = rs4_get_nibble(i_rs4Str, i_nibbleIndx);

        l_numNonZeroNibbles = (l_numNonZeroNibbles * 8) + (l_nibble & 0x07);

        i_nibbleIndx++;
        l_numNibblesParsed++;
    }
    while((l_nibble & 0x08) == 0);

    o_numRotate = l_numNonZeroNibbles;

    return l_numNibblesParsed;
}

/// @brief Byte-reverse a 64-bit integer
/// @param[in] i_x 64-bit word that need to be byte reversed
/// @return Byte reversed 64-bit word
uint64_t rs4_revle64(const uint64_t i_x)
{
    uint64_t rx;

#ifndef _BIG_ENDIAN
    uint8_t* pix = (uint8_t*)(&i_x);
    uint8_t* prx = (uint8_t*)(&rx);

    prx[0] = pix[7];
    prx[1] = pix[6];
    prx[2] = pix[5];
    prx[3] = pix[4];
    prx[4] = pix[3];
    prx[5] = pix[2];
    prx[6] = pix[1];
    prx[7] = pix[0];
#else
    rx = i_x;
#endif

    return rx;
}
}; //end of namespace

void getRingProperties(const RingID i_ringId,
                       uint32_t& o_torOffset,
                       RINGTYPE& o_ringType,
                       CHIPLET_TYPE& o_chipletType)
{
    do
    {
        // Determine the TOR ID
        o_torOffset =
            (INSTANCE_RING_MASK & (RING_PROPERTIES[i_ringId].iv_torOffSet));
        o_chipletType = RING_PROPERTIES[i_ringId].iv_type;

        if(INVALID_RING == o_torOffset)
        {
            break;
        }

        // Determine Ring Type
        if(INSTANCE_RING_MARK & (RING_PROPERTIES[i_ringId].iv_torOffSet))
        {
            o_ringType = INSTANCE_RING;
        }
        else
        {
            o_ringType = COMMON_RING;
        }
    }
    while(0);
}
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
    uint64_t i_scanData)
{
    FAPI_INF(">> standardScan");

    fapi2::ReturnCode l_rc = fapi2::FAPI2_RC_SUCCESS;
    // @TODO: getChipletNumber is only supported on SBE plat.
    //        Need to have it supported in the eKB code.
    uint32_t l_chiplet = i_chipletId << 24;
#ifdef __PPE__
    uint32_t l_chipletID = i_target.getChipletNumber();

    if ( l_chipletID )
    {
        l_chiplet = (l_chipletID << 24);
    }

#endif

#ifndef __PPE__

    // Non-PPE platform - Cronus need a Chip target to be used
    // in putScom/getScom.
    fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> l_parent;

    if (i_target.getType() == fapi2::TARGET_TYPE_CORE)
    {
        l_parent = i_target.template getParent<fapi2::TARGET_TYPE_PROC_CHIP> ();
    }

#endif

    do
    {
        // **************
        // Scan or Rotate
        // **************
        if(ROTATE == i_operation)
        {
            // Setup Scom Address for rotate operation
            uint32_t l_scomAddress = 0x00039000;

            // Add the chiplet ID in the Scom Address
            l_scomAddress |= l_chiplet;

            const uint64_t l_maxRotates = 0xFFFFF;
            uint64_t l_rotateCount = i_opVal;
            uint32_t l_numRotateScoms = 1; // 1 - We need to do atleast one scom

            if(i_opVal > l_maxRotates)
            {
                l_numRotateScoms = (i_opVal / l_maxRotates);
                l_rotateCount = l_maxRotates;
            }


            // Scom Data needs to have the no.of rotates in the bits 12-31
            l_rotateCount <<= 32;

            for(uint32_t i = 0; i < (l_numRotateScoms + 1); i++)
            {
                if(i == l_numRotateScoms)
                {
                    if(i_opVal <= l_maxRotates)
                    {
                        break;
                    }

                    l_rotateCount = (i_opVal % l_maxRotates);
                    l_rotateCount <<= 32;
                }

                FAPI_INF("l_rotateCount %u", l_rotateCount);
                fapi2::buffer<uint64_t> l_scomData(l_rotateCount);

#ifndef __PPE__
                l_rc = fapi2::putScom(l_parent, l_scomAddress, l_scomData);
#else
                l_rc = fapi2::putScom(i_target, l_scomAddress, l_scomData);
#endif

                if(l_rc != fapi2::FAPI2_RC_SUCCESS)
                {
                    FAPI_ERR("ROTATE for %d, failed", i_opVal);
                    break;
                }

                // Check OPCG_DONE status
                uint32_t  l_OPCGAddress = 0x00000100;

                // Add the chiplet ID in the Scom Address
                l_OPCGAddress |= l_chiplet;

                // @TODO: Value 300 is a random number to start with.
                uint32_t l_attempts = 300;

                while(l_attempts > 0)
                {
                    l_attempts--;

                    fapi2::buffer<uint64_t> l_opcgStatus;
#ifndef __PPE__
                    l_rc = fapi2::getScom(l_parent, l_OPCGAddress, l_opcgStatus);
#else
                    l_rc = fapi2::getScom(i_target, l_OPCGAddress, l_opcgStatus);
#endif

                    if(l_rc != fapi2::FAPI2_RC_SUCCESS)
                    {
                        FAPI_ERR("Failure during OPCG Check");
                        break;
                    }

                    if(l_opcgStatus.getBit<8>())
                    {
                        FAPI_INF("OPCG_DONE set");
                        break;
                    }

                    // @TODO: 1 micro second is a number that works now.
                    //        Need to derive the real delay number.
                    fapi2::delay(1000, 1000000);

                }

                if(l_attempts == 0 )
                {
                    l_rc = fapi2::FAPI2_RC_PLAT_ERR_SEE_DATA;
                    FAPI_ERR("Max attempts exceeded checking OPCG_DONE");
                    break;
                }
            }// end of for loop
        }
        else if(SCAN == i_operation)
        {
            // Setting Scom Address for a 64-bit scan
            uint32_t l_scomAddress = 0x0003E000;

            // Add the chiplet ID in the Scom Address
            l_scomAddress |= l_chiplet;

            uint32_t l_scanCount = i_opVal;

            fapi2::buffer<uint64_t> l_scomData(i_scanData);

            // Set the scan count to the actual value
            l_scomAddress |= l_scanCount;

#ifndef __PPE__
            l_rc = fapi2::putScom(l_parent, l_scomAddress, l_scomData);
#else
            l_rc = fapi2::putScom(i_target, l_scomAddress, l_scomData);
#endif

            if(l_rc != fapi2::FAPI2_RC_SUCCESS)
            {
                FAPI_ERR("SCAN for %d, failed", i_opVal);
                break;
            }
        } // end of if(SCAN == i_operation)
    }
    while(0);

    FAPI_INF("<< standardScan");
    return l_rc;
}

/// @brief Function to set the Scan Region
/// @param[in] i_target Chiplet Target of Scan
/// @param[in] i_scanRegion Value to be set to select a Scan Region
//  @param[in] i_chipletId data from RS4
/// @return FAPI2_RC_SUCCESS if success, else error code.
fapi2::ReturnCode setupScanRegion(const fapi2::Target<fapi2::TARGET_TYPE_ALL>&
                                  i_target,
                                  uint64_t i_scanRegion,
                                  const uint8_t i_chipletId)
{
    fapi2::ReturnCode l_rc;
    uint32_t l_chiplet =  i_chipletId << 24;
    uint64_t l_scan_region = i_scanRegion;
#ifdef __PPE__
    uint32_t l_chipletID = i_target.getChipletNumber();

    if ( l_chipletID )
    {
        l_chiplet = (l_chipletID << 24);
    }

    if (fapi2::TARGET_TYPE_EX & (i_target.get().getFapiTargetType()))
    {
        // this gives position of ex (0 or 1)
        uint32_t l_ex_number = i_target.getTargetNumber();

        // We are in odd EX and it's placed in bottom half of the EQ
        if (l_ex_number % 2)
        {
            uint32_t l_type = (uint32_t)i_scanRegion;
            // need to shift one bit.
            uint32_t l_ex_region = i_scanRegion >> 32;
            l_ex_region = l_ex_region >> 1;

            l_scan_region = l_ex_region;
            l_scan_region = l_scan_region << 32;
            l_scan_region = l_scan_region | l_type;

        }

        // for even ex, the data from RS4 hold good.
    }

#endif



#ifndef __PPE__
    // Non-PPE platform - Cronus need a Chip target to be used
    // in putScom/getScom.
    fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> l_parent(
        i_target.template getParent<fapi2::TARGET_TYPE_PROC_CHIP>());
#endif

    do
    {
        // **************************
        // Setup Scan-Type and Region
        // **************************
        uint32_t l_scomAddress = 0x00030005;

        // Add the chiplet ID in the Scom Address
        l_scomAddress |= l_chiplet;

        // Do the scom
        fapi2::buffer<uint64_t> l_scanRegion(l_scan_region);
#ifndef __PPE__
        l_rc = fapi2::putScom(l_parent, l_scomAddress, l_scanRegion);
#else
        l_rc = fapi2::putScom(i_target, l_scomAddress, l_scanRegion);
#endif

        if(l_rc != fapi2::FAPI2_RC_SUCCESS)
        {
            FAPI_ERR("Setup Scan-Type and Region failed");
            break;
        }
    }
    while(0);

    return l_rc;
}

/// @brief Function to write the header data to the ring.
/// @param[in] i_target Chiplet Target of Scan
/// @param[in] i_header The header data that is to be written.
//  @param[in] i_chipletId data from RS4
/// @return FAPI2_RC_SUCCESS if success, else error code.
fapi2::ReturnCode writeHeader(const fapi2::Target<fapi2::TARGET_TYPE_ALL>&
                              i_target,
                              const uint64_t i_header,
                              const uint8_t i_chipletId)
{
    fapi2::ReturnCode l_rc = fapi2::FAPI2_RC_SUCCESS;

    do
    {
        // @TODO: getChipletNumber is only supported on SBE plat.
        //        Need to have it supported in the eKB code.
        uint32_t l_chiplet = i_chipletId << 24;
#ifdef __PPE__
        uint32_t l_chipletID = i_target.getChipletNumber();

        if ( l_chipletID )
        {
            l_chiplet = (l_chipletID << 24);
        }

#endif

        uint32_t l_scomAddress = 0x0003E040; // 64-bit scan
        // Add the chiplet ID in the Scom Address
        l_scomAddress |= l_chiplet;

// I think we won't require this
#ifndef __PPE__
        l_rc = fapi2::putScom(
                   i_target.template getParent<fapi2::TARGET_TYPE_PROC_CHIP>(),
                   l_scomAddress,
                   i_header);
#else
        l_rc = fapi2::putScom(i_target, l_scomAddress, i_header);
#endif

        if(l_rc != fapi2::FAPI2_RC_SUCCESS)
        {
            FAPI_ERR("Error during writing header %016x", i_header);
            break;
        }
    }
    while(0);

    return l_rc;

}

/// @brief Function to reader the header data from the ring and verify it.
/// @param[in] i_target Chiplet Target of Scan
/// @param[in] i_header The header data that is expected.
//  @param[in] i_chipletId data from RS4
//  @param[in] i_ringMode different ring mode operations
/// @return FAPI2_RC_SUCCESS if success, else error code.
fapi2::ReturnCode verifyHeader(const fapi2::Target<fapi2::TARGET_TYPE_ALL>&
                               i_target,
                               const uint64_t i_header,
                               const uint8_t i_chipletId,
                               const fapi2::RingMode i_ringMode)
{
    fapi2::ReturnCode l_rc = fapi2::FAPI2_RC_SUCCESS;

    do
    {
        // @TODO: getChipletNumber is only supported on SBE plat.
        //        Need to have it supported in the eKB code.
        uint32_t l_chiplet = i_chipletId << 24;
#ifdef __PPE__
        uint32_t l_chipletID = i_target.getChipletNumber();

        if ( l_chipletID )
        {
            l_chiplet = (l_chipletID << 24);
        }

#endif

        uint32_t l_scomAddress = 0x0003E000; // 64-bit scan
        // Add the chiplet ID in the Scom Address
        l_scomAddress |= l_chiplet;

        fapi2::buffer<uint64_t> l_readHeader;

#ifndef __PPE__
        l_rc = fapi2::getScom(
                   i_target.template getParent<fapi2::TARGET_TYPE_PROC_CHIP>(),
                   l_scomAddress,
                   l_readHeader);
#else
        l_rc = fapi2::getScom(i_target, l_scomAddress, l_readHeader);
#endif

        if(l_rc != fapi2::FAPI2_RC_SUCCESS)
        {
            FAPI_ERR("Error during reading header %016x", i_header);
            break;
        }

        FAPI_INF("Got header - %016x", uint64_t(l_readHeader));

        if(l_readHeader != i_header)
        {
            FAPI_ERR("Read header(%016x) data incorrect", uint64_t(l_readHeader));
            l_rc = fapi2::FAPI2_RC_PLAT_ERR_RING_HEADER_CHECK;
            break;
        }

        if ((i_ringMode &  fapi2::RING_MODE_SET_PULSE_NSL))
        {
            const uint64_t l_header = 0xa5a5a5a500000000;
            uint32_t l_address = 0x0003A000; // 64-bit scan
            // Add the chiplet ID in the Scom Address
            l_address |= l_chiplet;

#ifndef __PPE__
            l_rc = fapi2::putScom(
                       i_target.template getParent<fapi2::TARGET_TYPE_PROC_CHIP>(),
                       l_address,
                       l_header);
#else
            l_rc = fapi2::putScom(i_target, l_address, l_header);
#endif

            if(l_rc != fapi2::FAPI2_RC_SUCCESS)
            {
                FAPI_ERR("Error during writing header %016x", l_header);
                break;
            }

        }
    }
    while(0);

    return l_rc;

}

/// @brief Function to set the Scan Region for set pulse mode
/// @param[in] i_target Chiplet Target of Scan
/// @param[in] i_scanRegion Value to be set to select a Scan Region
/// @return FAPI2_RC_SUCCESS if success, else error code.
fapi2::ReturnCode setupScanRegionForSetPulse(
    const fapi2::Target<fapi2::TARGET_TYPE_ALL>& i_target,
    uint64_t i_scanRegion,
    const fapi2::RingMode i_ringMode,
    const uint8_t i_chipletId)
{
    fapi2::ReturnCode l_rc;

    uint32_t l_chiplet = (i_chipletId << 24);
#ifdef __PPE__
    uint32_t l_chipletID = i_target.getChipletNumber();

    if ( l_chipletID )
    {
        l_chiplet = (l_chipletID << 24);
    }

#endif

#ifndef __PPE__
    // Non-PPE platform - Cronus need a Chip target to be used
    // in putScom/getScom.
    fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> l_parent(
        i_target.template getParent<fapi2::TARGET_TYPE_PROC_CHIP>());
#endif

    do
    {
        // **************************
        // Setup OPCG_ALIGN – SNOP Align=5 and SNOP Wait=7
        // **************************
        uint32_t l_scomAddress = 0x00030001;

        // Add the chiplet ID in the Scom Address
        l_scomAddress |= l_chiplet;

        fapi2::buffer<uint64_t> l_opcgAlign;

#ifndef __PPE__
        l_rc = fapi2::getScom(l_parent, l_scomAddress, l_opcgAlign);
#else
        l_rc = fapi2::getScom(i_target, l_scomAddress, l_opcgAlign);
#endif
        //set SNOP Align=8:1 and SNOP Wait=7
        // bits: 4:7   SNOP_ALIGN(0:3) 5: 8:1
        // bits: 20:31 SNOP_WAIT(0:11)
        l_opcgAlign.setBit<5>();
        l_opcgAlign.setBit<7>();

        l_opcgAlign.setBit<29>();
        l_opcgAlign.setBit<30>();
        l_opcgAlign.setBit<31>();

        // Do the scom
#ifndef __PPE__
        l_rc = fapi2::putScom(l_parent, l_scomAddress, l_opcgAlign);
#else
        l_rc = fapi2::putScom(i_target, l_scomAddress, l_opcgAlign);
#endif

        if(l_rc != fapi2::FAPI2_RC_SUCCESS)
        {
            FAPI_ERR("Setup OPCG Algin failed");
            break;
        }

        // **************************
        // Setup Scan-Type and Region
        // **************************
        l_scomAddress = 0x00030005;

        // Add the chiplet ID in the Scom Address
        l_scomAddress |= l_chiplet;

        // Do the scom
        fapi2::buffer<uint64_t> l_scanRegion(i_scanRegion);
#ifndef __PPE__
        l_rc = fapi2::putScom(l_parent, l_scomAddress, l_scanRegion);
#else
        l_rc = fapi2::putScom(i_target, l_scomAddress, l_scanRegion);
#endif

        if(l_rc != fapi2::FAPI2_RC_SUCCESS)
        {
            FAPI_ERR("Setup Scan-Type and Region failed");
            break;
        }

        ////////////////////////////
        //prepare clk_region register
        ////////////////////////////
        l_scomAddress = 0x00030006;

        // Add the chiplet ID in the Scom Address
        l_scomAddress |= l_chiplet;

        //First 32 bits tells clock region
        uint64_t l_clk_region = i_scanRegion & 0xFFFFFFFF00000000;

        fapi2::buffer<uint64_t> l_clkRegion(l_clk_region);

#ifndef __PPE__
        l_rc = fapi2::putScom(l_parent, l_scomAddress, l_clkRegion);
#else
        l_rc = fapi2::putScom(i_target, l_scomAddress, l_clkRegion);
#endif

        if(l_rc != fapi2::FAPI2_RC_SUCCESS)
        {
            FAPI_ERR("Setup NSL mode failed");
            break;
        }

        //////////////////////
        //prepare opcg_reg0
        //////////////////////
        l_scomAddress = 0x00030002;

        // Add the chiplet ID in the Scom Address
        l_scomAddress |= l_chiplet;

        //bit 11 -- RUN_OPCG_ON_UPDATE_DR
        uint64_t l_opcg_reg0 = 0x0010000000000000;

        fapi2::buffer<uint64_t> l_opcgReg0(l_opcg_reg0);

#ifndef __PPE__
        l_rc = fapi2::putScom(l_parent, l_scomAddress, l_opcgReg0);
#else
        l_rc = fapi2::putScom(i_target, l_scomAddress, l_opcgReg0);
#endif

        if(l_rc != fapi2::FAPI2_RC_SUCCESS)
        {
            FAPI_ERR("Setup NSL mode failed");
            break;
        }

        uint64_t l_opcg_reg1 = 0x0;
        uint64_t l_opcg_reg2 = 0x0;
        // NSL for slow regions
        uint64_t l_opcg_capt1 = 0x0;
        // NSL for fast regions
        uint64_t l_opcg_capt2 = 0x0;
        uint64_t l_opcg_capt3 = 0x0;

        // setup NSL mode
        if (i_ringMode &  fapi2::RING_MODE_SET_PULSE_NSL)
        {
            l_opcg_reg1 = 0x0;
            l_opcg_reg2 = 0x0;
            // NSL for slow regions
            l_opcg_capt1 = 0x1400000000000000;
            // NSL for fast regions
            l_opcg_capt2 = 0x0400000000000000;
            l_opcg_capt3 = 0x0;

        }
        else if ((i_ringMode &  fapi2::RING_MODE_SET_PULSE_SL))
        {
            l_opcg_reg1 = 0x0;
            l_opcg_reg2 = 0x0;
            // NSL for slow regions
            l_opcg_capt1 = 0x1800000000000000;
            // NSL for fast regions
            l_opcg_capt2 = 0x0800000000000000;
            l_opcg_capt3 = 0x0;
        }
        else   //set pulse all
        {
            l_opcg_reg1 = 0x0;
            l_opcg_reg2 = 0x0;
            // NSL for slow regions
            l_opcg_capt1 = 0x1E00000000000000;
            // NSL for fast regions
            l_opcg_capt2 = 0x0E00000000000000;
            l_opcg_capt3 = 0x0;

        }

        //prepare opcg_reg1
        l_scomAddress = 0x00030003;

        // Add the chiplet ID in the Scom Address
        l_scomAddress |= l_chiplet;

        fapi2::buffer<uint64_t> l_opcgReg1(l_opcg_reg1);

#ifndef __PPE__
        l_rc = fapi2::putScom(l_parent, l_scomAddress, l_opcgReg1);
#else
        l_rc = fapi2::putScom(i_target, l_scomAddress, l_opcgReg1);
#endif

        if(l_rc != fapi2::FAPI2_RC_SUCCESS)
        {
            FAPI_ERR("Setup opcg reg1 failed");
            break;
        }

        //prepare opcg_reg2
        l_scomAddress = 0x00030004;

        // Add the chiplet ID in the Scom Address
        l_scomAddress |= l_chiplet;


        fapi2::buffer<uint64_t> l_opcgReg2(l_opcg_reg2);

#ifndef __PPE__
        l_rc = fapi2::putScom(l_parent, l_scomAddress, l_opcgReg2);
#else
        l_rc = fapi2::putScom(i_target, l_scomAddress, l_opcgReg2);
#endif

        if(l_rc != fapi2::FAPI2_RC_SUCCESS)
        {
            FAPI_ERR("Setup opcg reg2 failed");
            break;
        }

        //prepare opcg_capt1
        l_scomAddress = 0x00030010;

        // Add the chiplet ID in the Scom Address
        l_scomAddress |= l_chiplet;


        fapi2::buffer<uint64_t> l_opcgCapt1(l_opcg_capt1);

#ifndef __PPE__
        l_rc = fapi2::putScom(l_parent, l_scomAddress, l_opcgCapt1);
#else
        l_rc = fapi2::putScom(i_target, l_scomAddress, l_opcgCapt1);
#endif

        if(l_rc != fapi2::FAPI2_RC_SUCCESS)
        {
            FAPI_ERR("Setup opcg capture 1 failed");
            break;
        }

        //prepare opcg_capt2
        l_scomAddress = 0x00030011;

        // Add the chiplet ID in the Scom Address
        l_scomAddress |= l_chiplet;


        fapi2::buffer<uint64_t> l_opcgCapt2(l_opcg_capt2);

#ifndef __PPE__
        l_rc = fapi2::putScom(l_parent, l_scomAddress, l_opcgCapt2);
#else
        l_rc = fapi2::putScom(i_target, l_scomAddress, l_opcgCapt2);
#endif

        if(l_rc != fapi2::FAPI2_RC_SUCCESS)
        {
            FAPI_ERR("Setup opcg capture 2 failed");
            break;
        }

        //prepare opcg_capt3
        l_scomAddress = 0x00030012;

        // Add the chiplet ID in the Scom Address
        l_scomAddress |= l_chiplet;


        fapi2::buffer<uint64_t> l_opcgCapt3(l_opcg_capt3);

#ifndef __PPE__
        l_rc = fapi2::putScom(l_parent, l_scomAddress, l_opcgCapt3);
#else
        l_rc = fapi2::putScom(i_target, l_scomAddress, l_opcgCapt3);
#endif

        if(l_rc != fapi2::FAPI2_RC_SUCCESS)
        {
            FAPI_ERR("Setup opcg capture 3 failed");
            break;
        }

    }
    while(0);

    return l_rc;
}
/// @brief Function to restore the opcg registers
/// @param[in] i_target Chiplet Target of Scan
/// @param[out]o_OPCGData Structure that contains opcg data
/// @return FAPI2_RC_SUCCESS if success, else error code.
fapi2::ReturnCode storeOPCGRegData(
    const fapi2::Target<fapi2::TARGET_TYPE_ALL>& i_target,
    struct restoreOpcgRegisters& o_OPCGData,
    const uint8_t i_chipletId)
{
    fapi2::ReturnCode l_rc;
    uint32_t l_chiplet = i_chipletId << 24;
#ifdef __PPE__
    uint32_t l_chipletID = i_target.getChipletNumber();

    if ( l_chipletID )
    {
        l_chiplet = (l_chipletID << 24);
    }

#endif

#ifndef __PPE__
    // Non-PPE platform - Cronus need a Chip target to be used
    // in putScom/getScom.
    fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> l_parent(
        i_target.template getParent<fapi2::TARGET_TYPE_PROC_CHIP>());
#endif

    do
    {
        //////////////////////
        //prepare opcg_reg0
        //////////////////////
        uint32_t l_scomAddress = 0x00030002;

        // Add the chiplet ID in the Scom Address
        l_scomAddress |= l_chiplet;


#ifndef __PPE__
        l_rc = fapi2::getScom(l_parent, l_scomAddress, o_OPCGData.l_opcgReg0);
#else
        l_rc = fapi2::getScom(i_target, l_scomAddress, o_OPCGData.l_opcgReg0);
#endif

        if(l_rc != fapi2::FAPI2_RC_SUCCESS)
        {
            FAPI_ERR("OPCG_REG0 read op failed");
            break;
        }

        //prepare opcg_reg1
        l_scomAddress = 0x00030003;

        // Add the chiplet ID in the Scom Address
        l_scomAddress |= l_chiplet;

#ifndef __PPE__
        l_rc = fapi2::getScom(l_parent, l_scomAddress, o_OPCGData.l_opcgReg1);
#else
        l_rc = fapi2::getScom(i_target, l_scomAddress, o_OPCGData.l_opcgReg1);
#endif

        if(l_rc != fapi2::FAPI2_RC_SUCCESS)
        {
            FAPI_ERR("OPCG_REG1 read op failed");
            break;
        }

        //prepare opcg_reg2
        l_scomAddress = 0x00030004;

        // Add the chiplet ID in the Scom Address
        l_scomAddress |= l_chiplet;

#ifndef __PPE__
        l_rc = fapi2::getScom(l_parent, l_scomAddress, o_OPCGData.l_opcgReg2);
#else
        l_rc = fapi2::getScom(i_target, l_scomAddress, o_OPCGData.l_opcgReg2);
#endif

        if(l_rc != fapi2::FAPI2_RC_SUCCESS)
        {
            FAPI_ERR("OPCG_REG2 read op failed");
            break;
        }

        //prepare opcg_capt1
        l_scomAddress = 0x00030010;

        // Add the chiplet ID in the Scom Address
        l_scomAddress |= l_chiplet;


#ifndef __PPE__
        l_rc = fapi2::getScom(l_parent, l_scomAddress, o_OPCGData.l_opcgCapt1);
#else
        l_rc = fapi2::getScom(i_target, l_scomAddress, o_OPCGData.l_opcgCapt1);
#endif

        if(l_rc != fapi2::FAPI2_RC_SUCCESS)
        {
            FAPI_ERR("OPCG_CAPT1 read op failed");
            break;
        }

        //prepare opcg_capt2
        l_scomAddress = 0x00030011;

        // Add the chiplet ID in the Scom Address
        l_scomAddress |= l_chiplet;

#ifndef __PPE__
        l_rc = fapi2::getScom(l_parent, l_scomAddress, o_OPCGData.l_opcgCapt2);
#else
        l_rc = fapi2::getScom(i_target, l_scomAddress, o_OPCGData.l_opcgCapt2);
#endif

        if(l_rc != fapi2::FAPI2_RC_SUCCESS)
        {
            FAPI_ERR("OPCG_CAPT2 read op failed");
            break;
        }

        //prepare opcg_capt3
        l_scomAddress = 0x00030012;

        // Add the chiplet ID in the Scom Address
        l_scomAddress |= l_chiplet;

#ifndef __PPE__
        l_rc = fapi2::getScom(l_parent, l_scomAddress, o_OPCGData.l_opcgCapt3);
#else
        l_rc = fapi2::getScom(i_target, l_scomAddress, o_OPCGData.l_opcgCapt3);
#endif

        if(l_rc != fapi2::FAPI2_RC_SUCCESS)
        {
            FAPI_ERR("OPCG_CAPT3 read op failed");
            break;
        }
    }
    while(0);

    return l_rc;
}
/// @brief Function to restore the opcg registers
/// @param[in] i_target Chiplet Target of Scan
/// @param[in] i_OPCGData opcg register data to restore original values
/// @return FAPI2_RC_SUCCESS if success, else error code.
fapi2::ReturnCode restoreOPCGRegData(
    const fapi2::Target<fapi2::TARGET_TYPE_ALL>& i_target,
    struct restoreOpcgRegisters i_OPCGData,
    const uint8_t i_chipletId)
{
    fapi2::ReturnCode l_rc;
    uint32_t l_chiplet = i_chipletId << 24;
#ifdef __PPE__
    uint32_t l_chipletID = i_target.getChipletNumber();

    if ( l_chipletID )
    {
        l_chiplet = (l_chipletID << 24);
    }

#endif

#ifndef __PPE__
    // Non-PPE platform - Cronus need a Chip target to be used
    // in putScom/getScom.
    fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> l_parent(
        i_target.template getParent<fapi2::TARGET_TYPE_PROC_CHIP>());
#endif

    do
    {
        //////////////////////
        //clear clk region
        //////////////////////
        uint32_t l_scomAddress = 0x00030006;

        // Add the chiplet ID in the Scom Address
        l_scomAddress |= l_chiplet;

        fapi2::buffer<uint64_t> l_clkReg(0);

#ifndef __PPE__
        l_rc = fapi2::putScom(l_parent, l_scomAddress, l_clkReg);
#else
        l_rc = fapi2::putScom(i_target, l_scomAddress, l_clkReg);
#endif

        if(l_rc != fapi2::FAPI2_RC_SUCCESS)
        {
            FAPI_ERR("OPCG_REG0 write op failed");
            break;
        }

        //////////////////////
        //prepare opcg_reg0
        //////////////////////
        l_scomAddress = 0x00030002;

        // Add the chiplet ID in the Scom Address
        l_scomAddress |= l_chiplet;


#ifndef __PPE__
        l_rc = fapi2::putScom(l_parent, l_scomAddress, i_OPCGData.l_opcgReg0);
#else
        l_rc = fapi2::putScom(i_target, l_scomAddress, i_OPCGData.l_opcgReg0);
#endif

        if(l_rc != fapi2::FAPI2_RC_SUCCESS)
        {
            FAPI_ERR("OPCG_REG0 write op failed");
            break;
        }

        //prepare opcg_reg1
        l_scomAddress = 0x00030003;

        // Add the chiplet ID in the Scom Address
        l_scomAddress |= l_chiplet;

#ifndef __PPE__
        l_rc = fapi2::putScom(l_parent, l_scomAddress, i_OPCGData.l_opcgReg1);
#else
        l_rc = fapi2::putScom(i_target, l_scomAddress, i_OPCGData.l_opcgReg1);
#endif

        if(l_rc != fapi2::FAPI2_RC_SUCCESS)
        {
            FAPI_ERR("OPCG_REG1 write op failed");
            break;
        }

        //prepare opcg_reg2
        l_scomAddress = 0x00030004;

        // Add the chiplet ID in the Scom Address
        l_scomAddress |= l_chiplet;

#ifndef __PPE__
        l_rc = fapi2::putScom(l_parent, l_scomAddress, i_OPCGData.l_opcgReg2);
#else
        l_rc = fapi2::putScom(i_target, l_scomAddress, i_OPCGData.l_opcgReg2);
#endif

        if(l_rc != fapi2::FAPI2_RC_SUCCESS)
        {
            FAPI_ERR("OPCG_REG2 write op failed");
            break;
        }

        //prepare opcg_capt1
        l_scomAddress = 0x00030010;

        // Add the chiplet ID in the Scom Address
        l_scomAddress |= l_chiplet;


#ifndef __PPE__
        l_rc = fapi2::putScom(l_parent, l_scomAddress, i_OPCGData.l_opcgCapt1);
#else
        l_rc = fapi2::putScom(i_target, l_scomAddress, i_OPCGData.l_opcgCapt1);
#endif

        if(l_rc != fapi2::FAPI2_RC_SUCCESS)
        {
            FAPI_ERR("OPCG_CAPT1 write op failed");
            break;
        }

        //prepare opcg_capt2
        l_scomAddress = 0x00030011;

        // Add the chiplet ID in the Scom Address
        l_scomAddress |= l_chiplet;

#ifndef __PPE__
        l_rc = fapi2::putScom(l_parent, l_scomAddress, i_OPCGData.l_opcgCapt2);
#else
        l_rc = fapi2::putScom(i_target, l_scomAddress, i_OPCGData.l_opcgCapt2);
#endif

        if(l_rc != fapi2::FAPI2_RC_SUCCESS)
        {
            FAPI_ERR("OPCG_CAPT2 write op failed");
            break;
        }

        //prepare opcg_capt3
        l_scomAddress = 0x00030012;

        // Add the chiplet ID in the Scom Address
        l_scomAddress |= l_chiplet;

#ifndef __PPE__
        l_rc = fapi2::putScom(l_parent, l_scomAddress, i_OPCGData.l_opcgCapt3);
#else
        l_rc = fapi2::putScom(i_target, l_scomAddress, i_OPCGData.l_opcgCapt3);
#endif

        if(l_rc != fapi2::FAPI2_RC_SUCCESS)
        {
            FAPI_ERR("OPCG_CAPT3 write op failed");
            break;
        }
    }
    while(0);

    return l_rc;
}

/// @brief Function to decompress the RS4 and apply the Ring data
/// @param[in] i_target Chiplet Target of Scan
/// @param[in] i_rs4 The RS4 compressed string
/// @param[in] i_applyOverride: state of override mode
/// @param[in] i_ringMode: different ring modes
/// @return FAPI2_RC_SUCCESS if success, else error code.
fapi2::ReturnCode rs4DecompressionSvc(
    const fapi2::Target<fapi2::TARGET_TYPE_ALL>& i_target,
    const uint8_t* i_rs4,
    const bool i_applyOverride,
    const fapi2::RingMode i_ringMode)
{
    FAPI_INF(">> rs4DecompressionSvc");
    CompressedScanData* l_rs4Header = (CompressedScanData*) i_rs4;
    const uint8_t* l_rs4Str = (i_rs4 + sizeof(CompressedScanData));

    opType_t l_opType = ROTATE;
    uint64_t l_nibbleIndx = 0;
    uint64_t l_bitsDecoded = 0;
    bool l_decompressionDone = false;
    uint64_t l_scanRegion = rs4_revle64(l_rs4Header->iv_scanSelect);
    uint8_t l_chipletId = l_rs4Header->iv_chipletId;
    fapi2::ReturnCode l_rc;
    struct restoreOpcgRegisters l_opcgData;
    uint8_t l_mask = 0x08;
    uint64_t l_scomData = 0x0;

    do
    {
        if (l_rs4Header->iv_length == 0)
        {
            FAPI_ERR("Invalid ring length in RS4 image");
            break;
        }

        if ((i_ringMode &  fapi2::RING_MODE_SET_PULSE_NSL) ||
            (i_ringMode &  fapi2::RING_MODE_SET_PULSE_SL) ||
            (i_ringMode &  fapi2::RING_MODE_SET_PULSE_ALL))
        {
            l_rc =  storeOPCGRegData (i_target, l_opcgData, l_chipletId);

            if(l_rc != fapi2::FAPI2_RC_SUCCESS)
            {
                break;
            }

            l_rc = setupScanRegionForSetPulse(i_target, l_scanRegion, i_ringMode
                                              , l_chipletId);

            if(l_rc != fapi2::FAPI2_RC_SUCCESS)
            {
                break;
            }
        }
        else
        {
            // Set up the scan region for the ring.
            l_rc = setupScanRegion(i_target, l_scanRegion, l_chipletId);

            if(l_rc != fapi2::FAPI2_RC_SUCCESS)
            {
                break;
            }
        }

        // Write a 64 bit value for header.
        const uint64_t l_header = 0xa5a5a5a5a5a5a5a5;
        l_rc = writeHeader(i_target, l_header, l_chipletId);

        if(l_rc != fapi2::FAPI2_RC_SUCCESS)
        {
            break;
        }

        //if the ring length is not 8bit aligned, then we need to skip the
        //padding bits
        uint8_t l_padding_bits = 0;

        if (l_rs4Header->iv_length % 4)
        {
            l_padding_bits = (4 - (l_rs4Header->iv_length % 4));
        }

        bool l_skip_64bits = true;

        // Decompress the RS4 string and scan
        do
        {
            if (l_opType == ROTATE)
            {
                // Determine the no.of ROTATE operations encoded in stop-code
                uint64_t l_count = 0;
                l_nibbleIndx += stop_decode(l_rs4Str, l_nibbleIndx, l_count);

                // Determine the no.of rotates in bits
                uint64_t l_bitRotates = (4 * l_count);

                //Need to skip 64bits , because we have already written header
                //data.
                if (l_skip_64bits)
                {
                    l_bitRotates -= SIXTYFOUR_BIT_HEADER;
                    l_skip_64bits = false;
                }

                l_bitsDecoded += l_bitRotates;

                if(l_bitsDecoded > l_rs4Header->iv_length)
                {
                    FAPI_ERR("Rotate decompression done."
                             "l_bitsDecoded = %d, length = %d",
                             l_bitsDecoded, l_rs4Header->iv_length);
                    l_decompressionDone = true;
                    l_rc = fapi2::FAPI2_RC_PLAT_RING_DECODE_LENGTH_EXCEEDED;
                    break;
                }

                // Do the ROTATE operation
                if (l_bitRotates != 0)
                {
                    l_rc = standardScan(i_target,
                                        l_chipletId,
                                        ROTATE,
                                        l_bitRotates);

                    if(l_rc != fapi2::FAPI2_RC_SUCCESS)
                    {
                        break;
                    }
                }

                l_opType = SCAN;
            }
            else if(l_opType == SCAN)
            {
                uint8_t l_scanCount = rs4_get_nibble(l_rs4Str, l_nibbleIndx);
                l_nibbleIndx++;

                if (l_scanCount == 0)
                {
                    FAPI_INF("SCAN COUNT %u", l_scanCount);
                    break;
                }

                if (!i_applyOverride)
                {
                    l_bitsDecoded += (4 * l_scanCount);

                    if(l_bitsDecoded > l_rs4Header->iv_length)
                    {
                        FAPI_ERR("Scan decompression done."
                                 "l_bitsDecoded = %d, length = %d",
                                 l_bitsDecoded, l_rs4Header->iv_length);
                        l_decompressionDone = true;
                        l_rc = fapi2::FAPI2_RC_PLAT_RING_DECODE_LENGTH_EXCEEDED;
                        break;
                    }

                    // Parse the non-zero nibbles of the RS4 string and
                    // scan them into the ring
                    l_scomData = rs4_get_verbatim(l_rs4Str,
                                                  l_nibbleIndx,
                                                  l_scanCount);
                    l_nibbleIndx += l_scanCount;

                    l_rc = standardScan(i_target,
                                        l_chipletId,
                                        SCAN,
                                        (l_scanCount * 4),
                                        l_scomData);

                    if(l_rc != fapi2::FAPI2_RC_SUCCESS)
                    {
                        break;
                    }
                }
                else
                {
                    if(0xF == l_scanCount) // We are parsing RS4 for override rings
                    {
                        uint8_t l_careMask =
                            rs4_get_nibble(l_rs4Str, l_nibbleIndx);
                        l_nibbleIndx++;
                        uint8_t l_spyData =
                            rs4_get_nibble(l_rs4Str, l_nibbleIndx);
                        l_nibbleIndx++;

                        for(uint8_t i = 0; i < 4; i++)
                        {
                            l_bitsDecoded += 1;
                            l_scomData = 0x0;

                            if((l_careMask & (l_mask >> i)))
                            {
                                if((l_spyData & (l_mask >> i)))
                                {
                                    l_scomData = 0xFFFFFFFFFFFFFFFF;
                                }

                                l_opType = SCAN;
                            }
                            else
                            {
                                l_opType = ROTATE;
                            }

                            l_rc = standardScan(i_target,
                                                l_chipletId,
                                                l_opType,
                                                1, // Insert 1 bit
                                                l_scomData);

                            if(l_rc != fapi2::FAPI2_RC_SUCCESS)
                            {
                                break;
                            }
                        } // end of looper for bit-parsing a non-zero nibble
                    }
                    else // We are parsing RS4 for base rings
                    {
                        for (uint8_t x = 0; x < l_scanCount; x++)
                        {
                            // Parse the non-zero nibbles of the RS4 string and
                            // scan them into the ring
                            uint8_t l_data =
                                rs4_get_nibble(l_rs4Str, l_nibbleIndx);
                            l_nibbleIndx += 1;

                            FAPI_INF ("VERBATIm l_nibbleIndx %u l_scanCount %u "
                                      "l_bitsDecoded %u", l_nibbleIndx, l_scanCount, l_bitsDecoded);

                            for(uint8_t i = 0; i < 4; i++)
                            {
                                l_scomData = 0x0;

                                if((l_data & (l_mask >> i)))
                                {
                                    l_opType = SCAN;
                                    l_scomData = 0xFFFFFFFFFFFFFFFF;
                                }
                                else
                                {
                                    l_opType = ROTATE;
                                }

                                l_rc = standardScan(i_target,
                                                    l_chipletId,
                                                    l_opType,
                                                    1, // Insert 1 bit
                                                    l_scomData);

                                if(l_rc != fapi2::FAPI2_RC_SUCCESS)
                                {
                                    break;
                                }
                            }

                            if(l_rc != fapi2::FAPI2_RC_SUCCESS)
                            {
                                break;
                            }
                        } // end of looper for bit-parsing a non-zero nibble
                    }
                }

                l_opType = ROTATE;
            } // end of - if(l_opType == SCAN)

            if(l_rc != fapi2::FAPI2_RC_SUCCESS)
            {
                break;
            }
        }
        while(1);

        if((l_rc != fapi2::FAPI2_RC_SUCCESS) || (true == l_decompressionDone))
        {
            break;
        }

        // Handle the string termination
        uint8_t l_nibble = rs4_get_nibble(l_rs4Str, l_nibbleIndx);
        l_nibbleIndx++;

        if (l_nibble != 0)
        {
            // Parse the non-zero nibbles of the RS4 string and
            // scan them into the ring
            if((l_bitsDecoded + l_nibble) > l_rs4Header->iv_length)
            {
                FAPI_ERR("Decompression Done."
                         "l_bitsDecoded = %d, l_nibble= %d, length = %d",
                         l_bitsDecoded, l_nibble, l_rs4Header->iv_length);
                l_rc = fapi2::FAPI2_RC_PLAT_RING_DECODE_LENGTH_EXCEEDED;
                break;
            }
            else
            {
                if (!i_applyOverride)
                {
                    l_bitsDecoded += l_nibble;
                    l_scomData = rs4_get_verbatim(l_rs4Str,
                                                  l_nibbleIndx,
                                                  1); // return 1 nibble

                    l_rc = standardScan(i_target,
                                        l_chipletId,
                                        SCAN,
                                        (4 - l_padding_bits) , // scan 4 bits
                                        l_scomData);
                }
                else
                {
                    if(0x8 & l_nibble) // We are parsing RS4 for override rings
                    {
                        uint8_t l_careMask = rs4_get_nibble(l_rs4Str, l_nibbleIndx);
                        l_nibbleIndx++;
                        uint8_t l_spyData = rs4_get_nibble(l_rs4Str, l_nibbleIndx);
                        l_nibbleIndx++;

                        for(uint8_t i = 0; i < 4; i++)
                        {
                            l_bitsDecoded += 1;
                            l_scomData = 0x0;

                            if((l_careMask & (l_mask >> i)))
                            {
                                if((l_spyData & (l_mask >> i)))
                                {
                                    l_scomData = 0xFFFFFFFFFFFFFFFF;
                                }

                                l_opType = SCAN;
                            }
                            else
                            {
                                l_opType = ROTATE;
                            }

                            l_rc = standardScan(i_target,
                                                l_chipletId,
                                                l_opType,
                                                1, // Insert 1 bit
                                                l_scomData);

                            if(l_rc != fapi2::FAPI2_RC_SUCCESS)
                            {
                                break;
                            }
                        } // end of looper for bit-parsing a non-zero nibble
                    }
                    else // We are parsing RS4 for base rings
                    {
                        // scan them into the ring
                        uint8_t l_data = rs4_get_nibble(l_rs4Str, l_nibbleIndx);

                        l_nibbleIndx += 1;

                        for(uint8_t i = 0; i < l_nibble; i++)
                        {
                            l_scomData = 0x0;
                            l_bitsDecoded += 1;

                            if((l_data & (l_mask >> i)))
                            {
                                l_opType = SCAN;
                                l_scomData = 0xFFFFFFFFFFFFFFFF;

                            }
                            else
                            {
                                l_opType = ROTATE;

                            }

                            l_rc = standardScan(i_target,
                                                l_chipletId,
                                                l_opType,
                                                1, // Insert 1 bit
                                                l_scomData);

                            if(l_rc != fapi2::FAPI2_RC_SUCCESS)
                            {
                                break;
                            }
                        } //end of for
                    }
                }
            }
        } // end of if(l_nibble != 0)

        if(l_rc != fapi2::FAPI2_RC_SUCCESS)
        {
            break;
        }

        // Verify header
        l_rc = verifyHeader(i_target, l_header, l_chipletId, i_ringMode);

        if(l_rc)
        {
            break;
        }

        // Clean scan region and type data
        l_rc = cleanScanRegionandTypeData(i_target, l_chipletId);

        if(l_rc)
        {
            break;
        }

        if ((i_ringMode &  fapi2::RING_MODE_SET_PULSE_NSL) ||
            (i_ringMode &  fapi2::RING_MODE_SET_PULSE_SL) ||
            (i_ringMode &  fapi2::RING_MODE_SET_PULSE_ALL))
        {
            l_rc = restoreOPCGRegData (i_target, l_opcgData, l_chipletId);

            if (l_rc)
            {
                break;
            }
        }

    }
    while(0);

    FAPI_INF("<< rs4DecompressionSvc");
    return l_rc;
}
/// @brief Function to clean up the scan region and type
/// @param[in] i_target Chiplet Target of Scan
//  @param[in] chipletId data from RS4
//  @param[in] i_chipletId data from RS4
/// @return FAPI2_RC_SUCCESS if success, else error code.
fapi2::ReturnCode cleanScanRegionandTypeData(
    const fapi2::Target<fapi2::TARGET_TYPE_ALL>& i_target,
    const uint8_t i_chipletId)
{
    fapi2::ReturnCode l_rc;
    uint32_t l_chiplet = i_chipletId << 24;
#ifdef __PPE__
    uint32_t l_chipletID = i_target.getChipletNumber();

    if ( l_chipletID )
    {
        l_chiplet = (l_chipletID << 24);
    }

#endif

#ifndef __PPE__
    // Non-PPE platform - Cronus need a Chip target to be used
    // in putScom/getScom.
    fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> l_parent(
        i_target.template getParent<fapi2::TARGET_TYPE_PROC_CHIP>());
#endif

    do
    {
        //////////////////////
        //cleanup opcg_reg0
        //////////////////////
        uint32_t l_scomAddress = 0x00030005;

        // Add the chiplet ID in the Scom Address
        l_scomAddress |= l_chiplet;

        fapi2::buffer<uint64_t> l_data(0);

#ifndef __PPE__
        l_rc = fapi2::putScom(l_parent, l_scomAddress, l_data);
#else
        l_rc = fapi2::putScom(i_target, l_scomAddress, l_data);
#endif

        if(l_rc != fapi2::FAPI2_RC_SUCCESS)
        {
            FAPI_ERR("OPCG_REG0 write op failed");
            break;
        }
    }
    while(0);

    return l_rc;
}



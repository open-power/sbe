/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/utils/p10_putRingUtils.C $               */
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
/// @file   p10_putRingUtils.C
/// @brief  Headers and Constants used by rs4 decompression and
/// ring SCAN/ROTATE functionality
///
// *HWP HW Owner:       Greg Still  <stillgs@us.ibm.com>
// *HWP FW Owner:       Prem S Jha  <premjha2@in.ibm.com>
// *HWP Team:           PM
// *HWP Level:          2
// *HWP Consumed by:    SBE

#include <p10_putRingUtils.H>
#include <p10_scom_eq_2.H>
#include <p10_scom_perv_0.H>
#include <p10_scom_perv_1.H>
#include <p10_scom_perv_7.H>
#include <p10_scom_perv_8.H>
#include <p10_scom_perv_2.H>
#include <p10_scom_perv_d.H>
#include <p10_scom_perv_e.H>
#include <p10_scom_perv_4.H>
#include <p10_scom_perv_c.H>
#include <p10_scan_compression.H>

namespace RS4
{

enum
{
    ROTATE_ADDRESS_REG          =   0x00039000,
    CHECK_WORD_REG_ADDRESS      =   0x0003F040,
};

}// namespace RS4

using namespace RS4;
extern "C"
{

///
/// @brief Return verbatim data from the RS4 string
/// @param[in] i_rs4Str         The RS4 scan string
/// @param[in] i_nibbleIndx     Index into RS4 string pointed to by i_rs4Str
/// @param[in] i_nibbleCount    The count of nibbles that need to be put in the return value.
/// @return big-endian-indexed double word
///
    uint64_t rs4_get_verbatim( const uint8_t* i_rs4Str,
                               const uint32_t i_nibbleIndx,
                               const uint8_t i_nibbleCount )
    {
        uint8_t l_byte;
        uint8_t l_nibble;
        uint64_t l_doubleWord   =   0;
        const uint32_t* l_data  =   NULL;
        uint32_t l_cnt          =   0;
        uint32_t l_offset       =   0;
        uint32_t l_index        =   i_nibbleIndx;
        rs4_data_t l_rs4_data;

        for( uint8_t i = 1; i <= i_nibbleCount; i++ )
        {
            l_cnt = l_index >> 1;

            if ( !( l_index %  8 ) || ( i == 1 ) )
            {
                l_offset    =   l_cnt - ( l_cnt % 4 );
                l_data      =   reinterpret_cast<uint32_t*> ( const_cast<uint8_t*>( i_rs4Str ) + l_offset );
                l_rs4_data.iv_data  =   *l_data;
            }

            l_byte      =   l_rs4_data.fields.val[l_cnt % 4];
            l_nibble    =   ( l_index % 2 ) ? ( l_byte & 0x0f ) : ( l_byte >> 4 );

            uint64_t l_tempDblWord = l_nibble;
            l_tempDblWord <<=   ( 64 - ( 4 * i ) );
            l_doubleWord   |=   l_tempDblWord;
            l_index++;
        }

        return l_doubleWord;
    }

//-------------------------------------------------------------------------------------------------------

    uint64_t decodeScanRegionData( const uint32_t i_ringAddress, uint32_t i_regionSelect )
    {
        uint64_t l_value        =   0;
        uint32_t l_scan_region  =   ( ( i_ringAddress & 0x0000FFF0 ) |
                                      ( ( i_ringAddress & 0x00F00000 ) >> 20 ) ) << 13;

        //Unlike P9, cores in P10 are not physical chiplet. Actually, it is eight quads which contain
        //four cores each, are the physical chiplet. It is true for L3 and MMA too which live inside the quad.
        //It is only the quads which get represented in the ring scan address. Inorder to identify the core/L3/MMA
        //associated with a quad for scanning, scan address in RS4 container is not sufficient. Putring in P10,
        //need a region select info to determine target core/MMA/L3 associated within the quad.
        //
        //In a multicast scenario, this region select shall determine which of the four core/L3/MMA
        //needs to be touched during scanning. putring code expects that platform will pass region select
        //as 32 bit bit vector.
        //For bit vector definition, refer to p10_putRingUtils.H

        l_scan_region           |=  i_regionSelect;

        //Inserting scan type value starting from bit position 48
        uint32_t l_scan_type    =   0x00008000 >> ( i_ringAddress & 0x0000000F );

        // Exception: If scantype is 0x0f then we need special handling.

        if ( ( i_ringAddress & 0x0000000F ) == 0xF )
        {
            //Setting bit 48 and 51
            l_scan_type     =   0x00009000;
        }

        l_value             =   l_scan_region;
        l_value             =   ( l_value << 32 ) |  l_scan_type;

        return l_value;
    }

//-------------------------------------------------------------------------------------------------------

///
/// @brief Decode an unsigned integer from a 4-bit octal stop code.
/// @param[in]  i_rs4Str        The RS4 scan string
/// @param[in]  i_nibbleIndx    Index into i_rs4Str that has the stop-code
/// @param[out] o_numRotate     No.of rotates decoded from the stop-code.
/// @return The number of nibbles decoded.
///
    uint64_t stop_decode( const uint8_t* i_rs4Str,
                          uint32_t i_nibbleIndx,
                          uint64_t& o_numRotate )
    {
        uint64_t l_numNibblesParsed     =   0; // No.of nibbles that make up the stop-code
        uint64_t l_numNonZeroNibbles    =   0;
        const uint32_t* l_data          =   NULL;
        uint32_t l_cnt                  =   0;
        uint32_t l_offset               =   0;
        rs4_data_t l_rs4_data;
        uint8_t l_nibble;
        uint8_t l_byte;

        do
        {
            l_cnt = i_nibbleIndx >> 1;

            if ( !( i_nibbleIndx %  8 ) || ( l_numNibblesParsed == 0 ) )
            {
                l_offset    =   l_cnt  - ( l_cnt % 4 );
                l_data      =   reinterpret_cast< uint32_t* > ( const_cast<uint8_t*>( i_rs4Str ) + l_offset );
                l_rs4_data.iv_data = *l_data;
            }

            l_byte      =   l_rs4_data.fields.val[l_cnt % 4];
            l_nibble    =   (i_nibbleIndx % 2) ? (l_byte & 0x0f) : (l_byte >> 4);
            l_numNonZeroNibbles     =   (l_numNonZeroNibbles * 8) + (l_nibble & 0x07);
            i_nibbleIndx++;
            l_numNibblesParsed++;

        }
        while( ( l_nibble & 0x08 ) == 0 );

        o_numRotate     =   l_numNonZeroNibbles;

        return l_numNibblesParsed;
    }

//-------------------------------------------------------------------------------------------------------

/// @brief Byte-reverse a 32-bit integer
/// @param[in] i_x  32-bit word that need to be byte reversed
/// @return Byte reversed 32-bit word
    uint32_t rs4_revle32( const uint32_t i_x )
    {
        uint32_t rx;

#ifndef _BIG_ENDIAN
        uint8_t* pix = (uint8_t*)(&i_x);
        uint8_t* prx = (uint8_t*)(&rx);

        prx[0] = pix[3];
        prx[1] = pix[2];
        prx[2] = pix[1];
        prx[3] = pix[0];
#else
        rx = i_x;
#endif

        return rx;
    }

//---------------------------------------------------------------------------------------------

    void getRingProperties( const RingId_t  i_ringId,
                            RingId_t&       o_torOffset,
                            RingType_t&     o_ringType,
                            ChipletType_t&  o_chipletType )
    {
        do
        {
            o_torOffset     =   RING_PROPERTIES[i_ringId].idxRing;

            // Check for valid ring index
            if( INVALID_RING_OFFSET == o_torOffset )
            {
                break;
            }

            // Determine Ring Type
            if( INSTANCE_RING_MARK & o_torOffset )
            {
                o_ringType  =   INSTANCE_RING;
            }
            else
            {
                o_ringType  =   COMMON_RING;
            }

            // Now that we know ringType, get the effective torOffset
            o_torOffset     =   INSTANCE_RING_MASK & o_torOffset;

            o_chipletType   =   RING_PROPERTIES[i_ringId].chipletType;
        }
        while(0);
    }

//---------------------------------------------------------------------------------------------

/// @brief Function to apply the Ring data using the standard-scan method
/// @param[in] i_target         Chiplet Target of Scan
//  @param[in] i_chipletId      data from RS4
/// @param[in] i_operation      Type of operation to perform - ROTATE/SCAN
/// @param[in] i_opVal          Number of bits for the operation
/// @param[in] i_scanData       This value has to be scanned when i_operation is SCAN
/// @return FAPI2_RC_SUCCESS if success, else error code.
    fapi2::ReturnCode standardScan(
        const fapi2::Target<fapi2::TARGET_TYPE_ALL_MC>& i_target,
        const uint8_t i_chipletId,
        opType_t i_operation,
        uint64_t i_opVal,
        uint64_t i_scanData,
        const uint16_t i_ringId )
    {
        FAPI_INF(">> standardScan");
        using namespace scomt;
        using namespace eq;
        using namespace perv;

        fapi2::ReturnCode l_rc  =   fapi2::FAPI2_RC_SUCCESS;
        // @TODO: getChipletNumber is only supported on SBE plat.
        //        Need to have it supported in the eKB code.
        uint32_t l_chiplet      =   i_chipletId << 24;
#ifdef __PPE__
        uint32_t l_chipletID    =   i_target.getChipletNumber();

        if ( l_chipletID )
        {
            l_chiplet           =   (l_chipletID << 24);
        }

#endif

        // Non-PPE platform - Cronus need a Chip target to be used
        // in putScom/getScom.
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> l_parent =
            i_target.template getParent<fapi2::TARGET_TYPE_PROC_CHIP> ();

        do
        {
            // **************
            // Scan or Rotate
            // **************
            if( ROTATE == i_operation )
            {
                // Setup Scom Address for rotate operation
                uint32_t l_scomAddress          =   ROTATE_ADDRESS_REG;
                const uint64_t l_maxRotates     =   0xFFFFF;
                uint64_t l_rotateCount          =   i_opVal;
                uint32_t l_numRotateScoms       =   1; // 1 - We need to do atleast one scom

                // Add the chiplet ID in the Scom Address
                l_scomAddress                  |=   l_chiplet;

                if( i_opVal > l_maxRotates )
                {
                    l_numRotateScoms = (i_opVal / l_maxRotates);
                    l_rotateCount = l_maxRotates;
                }

                // Scom Data needs to have the no.of rotates in the bits 12-31
                l_rotateCount <<= 32;

                for( uint32_t i = 0; i < (l_numRotateScoms + 1); i++ )
                {
                    if( i == l_numRotateScoms )
                    {
                        if( i_opVal <= l_maxRotates )
                        {
                            break;
                        }

                        l_rotateCount = ( i_opVal % l_maxRotates );
                        l_rotateCount <<= 32;
                    }

                    FAPI_INF("l_rotateCount %u", l_rotateCount);
                    fapi2::buffer<uint64_t> l_scomData( l_rotateCount );

                    l_rc = fapi2::putScom( l_parent, l_scomAddress, l_scomData );

                    if( l_rc != fapi2::FAPI2_RC_SUCCESS )
                    {
                        FAPI_ERR("ROTATE for %d, failed", i_opVal);
                        break;
                    }

                    // Check OPCG_DONE status
                    uint32_t  l_OPCGAddress = 0x00000100;

                    // Add the chiplet ID in the Scom Address
                    l_OPCGAddress |= l_chiplet;

                    // @TODO: Value 1000 is a random number to start with.
                    uint32_t l_attempts = 1000;

                    while( l_attempts > 0 )
                    {
                        l_attempts--;

                        fapi2::buffer<uint64_t> l_opcgStatus;

                        l_rc = fapi2::getScom( l_parent, l_OPCGAddress, l_opcgStatus );

                        if( l_rc != fapi2::FAPI2_RC_SUCCESS )
                        {
                            FAPI_ERR("Failure during OPCG Check");
                            break;
                        }

                        if( l_opcgStatus.getBit( perv::CPLT_STAT0_CC_CTRL_OPCG_DONE_DC ) )
                        {
                            FAPI_INF("OPCG_DONE set");
                            break;
                        }

                        // @TODO: 1 micro second is a number that works now.
                        //        Need to derive the real delay number.
                        fapi2::delay( 1000, 1000000 );

                    }

                    if( 0 == l_attempts )
                    {
                        l_rc = fapi2::FAPI2_RC_PLAT_ERR_SEE_DATA;
                        FAPI_ERR("Max attempts exceeded checking OPCG_DONE");
                        FAPI_ASSERT(false,
                                    fapi2::P10_PUTRING_OPCG_DONE_TIMEOUT()
                                    .set_TARGET(l_parent)
                                    .set_CHIPLET_ID(l_chiplet)
                                    .set_SCOM_ADDRESS(l_scomAddress)
                                    .set_SCOM_DATA(l_scomData)
                                    .set_ROTATE_COUNT(l_rotateCount)
                                    .set_RING_ID(i_ringId)
                                    .set_RETURN_CODE(l_rc),
                                    "ROTATE operation failed  due to timeout");
                    }
                }// end of for loop
            }
            else if( SCAN == i_operation )
            {
                // Setting Scom Address for a 64-bit scan
                uint32_t l_scomAddress = SCAN64CONTSCAN;

                // Add the chiplet ID in the Scom Address
                l_scomAddress |= l_chiplet;

                uint32_t l_scanCount = i_opVal;

                fapi2::buffer<uint64_t> l_scomData( i_scanData );

                // Set the scan count to the actual value
                l_scomAddress |= l_scanCount;

                l_rc = fapi2::putScom( l_parent, l_scomAddress, l_scomData );

                if( l_rc != fapi2::FAPI2_RC_SUCCESS )
                {
                    FAPI_ERR( "SCAN for %d, failed", i_opVal );
                    break;
                }
            } // end of if(SCAN == i_operation)
        }
        while(0);

    fapi_try_exit:

        FAPI_INF("<< standardScan");
        return l_rc;
    }

//---------------------------------------------------------------------------------------------

/// @brief Function to set the Scan Region
/// @param[in] i_target         Chiplet Target of Scan
/// @param[in] i_scanRegion     Value to be set to select a Scan Region
//  @param[in] i_chipletId data from RS4
/// @return FAPI2_RC_SUCCESS if success, else error code.
    fapi2::ReturnCode setupScanRegion( const fapi2::Target<fapi2::TARGET_TYPE_ALL_MC>& i_target,
                                       uint64_t i_scanRegion,
                                       const uint8_t i_chipletId,
                                       const RingType_t i_ringType,
                                       OpMode_t i_opMode )
    {
        using namespace scomt;
        using namespace eq;
        fapi2::ReturnCode l_rc;
        uint32_t l_chiplet      =   i_chipletId << 24;

        if( TEST_MODE == i_opMode )
        {
            l_rc    =   fapi2::FAPI2_RC_SUCCESS;
            FAPI_INF( "Scan Region 0x%016lx", i_scanRegion );
            goto  fapi_try_exit;
        }

        {
            // Non-PPE platform - Cronus need a Chip target to be used
            // in putScom/getScom.
            fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> l_parent(
                i_target.template getParent<fapi2::TARGET_TYPE_PROC_CHIP>());

            do
            {
                // **************************
                // Setup Scan-Type and Region
                // **************************
                uint32_t l_scomAddress = 0x00030005;

                // Add the chiplet ID in the Scom Address
                l_scomAddress |= l_chiplet;

                // Do the scom
                fapi2::buffer<uint64_t> l_regionData( i_scanRegion );

                l_rc = fapi2::putScom( l_parent, l_scomAddress, l_regionData );

                if( l_rc != fapi2::FAPI2_RC_SUCCESS )
                {
                    FAPI_ERR("Setup Scan-Type and Region failed");
                    break;
                }
            }
            while(0);
        }

    fapi_try_exit:
        return l_rc;
    }

//---------------------------------------------------------------------------------------------

/// @brief Function to write the header data to the ring.
/// @param[in] i_target         Chiplet Target of Scan
/// @param[in] i_header         The header data that is to be written.
//  @param[in] i_chipletId      data from RS4
/// @return FAPI2_RC_SUCCESS if success, else error code.
    fapi2::ReturnCode writeHeader( const fapi2::Target<fapi2::TARGET_TYPE_ALL_MC>& i_target,
                                   const uint64_t i_header,
                                   const uint8_t i_chipletId )
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

            uint32_t l_scomAddress = CHECK_WORD_REG_ADDRESS; // 64-bit scan
            // Add the chiplet ID in the Scom Address
            l_scomAddress |= l_chiplet;

            l_rc = fapi2::putScom(
                       i_target.template getParent<fapi2::TARGET_TYPE_PROC_CHIP>(),
                       l_scomAddress,
                       i_header);

            if( l_rc != fapi2::FAPI2_RC_SUCCESS )
            {
                FAPI_ERR("Error during writing header %016x", i_header);
                break;
            }
        }
        while(0);

        return l_rc;

    }

//---------------------------------------------------------------------------------------------

/// @brief Function to reader the header data from the ring and verify it.
/// @param[in] i_target         Chiplet Target of Scan
//  @param[in] i_chipletId      data from RS4
//  @param[in] i_ringMode       different ring mode operations
//  @param[in] i_bitsDecoded    number of bits rotated and scanned
//  @param[in] i_ringId         ring Id that we scanned
/// @return FAPI2_RC_SUCCESS if success, else error code.
    fapi2::ReturnCode verifyHeader( const fapi2::Target<fapi2::TARGET_TYPE_ALL_MC>& i_target,
                                    const uint8_t i_chipletId,
                                    const fapi2::RingMode i_ringMode,
                                    const uint32_t i_bitsDecoded,
                                    const uint16_t i_ringId )
    {
        using namespace scomt;
        using namespace perv;
        fapi2::ReturnCode l_rc  =   fapi2::FAPI2_RC_SUCCESS;
        uint32_t l_address      =   0;
        fapi2::buffer<uint64_t> l_scomData;

        do
        {
            // @TODO: getChipletNumber is only supported on SBE plat.
            //        Need to have it supported in the eKB code.
            uint32_t l_chiplet  =   i_chipletId << 24;
#ifdef __PPE__
            uint32_t l_chipletID = i_target.getChipletNumber();

            if ( l_chipletID )
            {
                l_chiplet = (l_chipletID << 24);
            }

#endif
            uint32_t l_scomAddress  =   SCAN64CONTSCAN; // 64-bit scan
            // Add the chiplet ID in the Scom Address
            l_scomAddress          |=   l_chiplet;

            fapi2::buffer<uint64_t> l_readHeader;

            l_rc = fapi2::getScom(
                       i_target.template getParent<fapi2::TARGET_TYPE_PROC_CHIP>(),
                       l_scomAddress,
                       l_readHeader );

            if( l_rc != fapi2::FAPI2_RC_SUCCESS )
            {
                FAPI_ERR("Error during reading header");
                break;
            }

            FAPI_INF("Got header - %016x", uint64_t(l_readHeader));

            if( l_readHeader != 0xa5a5a5a5a5a5a5a5 )
            {
                FAPI_ERR("Read CHECKWORD (%016x) data incorrect and total bit decoded 0x%016x",
                         uint64_t(l_readHeader), (uint64_t)i_bitsDecoded);
                l_rc = fapi2::FAPI2_RC_PLAT_ERR_RING_HEADER_CHECK;

                FAPI_ASSERT( false,
                             fapi2::P10_PUTRING_CHECKWORD_DATA_MISMATCH()
                             .set_TARGET( i_target )
                             .set_CHIPLET_ID( l_chiplet )
                             .set_SCOM_ADDRESS( l_scomAddress )
                             .set_SCOM_DATA( l_readHeader )
                             .set_BITS_DECODED( i_bitsDecoded )
                             .set_RING_ID( i_ringId )
                             .set_RINGMODE( i_ringMode )
                             .set_RETURN_CODE( l_rc ),
                             "CHECKWORD DATA mismatch" );
            }

            l_address   =   (CPLT_STAT0 | l_chiplet);

            l_rc        =   fapi2::getScom(
                                i_target.template getParent<fapi2::TARGET_TYPE_PROC_CHIP>(),
                                l_address,
                                l_scomData );

            if( l_scomData.getBit<CPLT_STAT0_CC_CTRL_PARALLEL_SCAN_COMPARE_ERR>() )
            {
                FAPI_ERR( "Parallel Scan Error For Ring 0x%02x ", i_ringId );
                FAPI_ASSERT( false,
                             fapi2::P10_PARALLEL_SCAN_COMPARE_ERR()
                             .set_TARGET( i_target )
                             .set_CHIPLET_ID( l_chiplet )
                             .set_RING_ID( i_ringId )
                             .set_CPLT_STAT0( l_scomData ),
                             "Failed To Complete Parallel Scan" );
            }
        }
        while(0);

    fapi_try_exit:
        return l_rc;

    }

//---------------------------------------------------------------------------------------------

/// @brief Function to clean up the scan region and type
/// @param[in] i_target         Chiplet Target of Scan
//  @param[in] i_chipletId      data from RS4
//  @param[in] i_chipletId      data from RS4
/// @return FAPI2_RC_SUCCESS if success, else error code.
    fapi2::ReturnCode cleanScanRegionandTypeData(
        const fapi2::Target<fapi2::TARGET_TYPE_ALL_MC>& i_target,
        const uint8_t i_chipletId )
    {
        fapi2::ReturnCode l_rc;
        uint32_t l_chiplet      =   i_chipletId << 24;
#ifdef __PPE__
        uint32_t l_chipletID    =   i_target.getChipletNumber();

        if ( l_chipletID )
        {
            l_chiplet           =   ( l_chipletID << 24 );
        }

#endif

        // Non-PPE platform - Cronus need a Chip target to be used
        // in putScom/getScom.
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> l_parent(
            i_target.template getParent<fapi2::TARGET_TYPE_PROC_CHIP>() );

        do
        {
            uint32_t l_scomAddress = 0x00030005;

            // Add the chiplet ID in the Scom Address
            l_scomAddress |= l_chiplet;

            fapi2::buffer<uint64_t> l_data( 0 );

            l_rc = fapi2::putScom( l_parent, l_scomAddress, l_data );

            if( l_rc    !=  fapi2::FAPI2_RC_SUCCESS )
            {
                FAPI_ERR( "OPCG_REG0 write op failed" );
                break;
            }
        }
        while(0);

        return l_rc;
    }

//----------------------------------------------------------------------------------------------------

    fapi2::ReturnCode p10_putRingUtils(
        const fapi2::Target<fapi2::TARGET_TYPE_ALL_MC>& i_target,
        const uint8_t* i_rs4,
        bool  i_applyOverride,
        const fapi2::RingMode i_ringMode,
        const RingType_t i_ringType,
        uint32_t i_regionSelect,
        OpMode_t    i_opMode )
    {
        FAPI_INF(">> p10_putRingUtils");
        fapi2::ReturnCode l_rc              =   fapi2::FAPI2_RC_SUCCESS;

        CompressedScanData* l_rs4Header     =   (CompressedScanData*) i_rs4;
        const uint8_t* l_rs4Str             =   ( i_rs4 +  sizeof( CompressedScanData ) );
        opType_t l_opType                   =   ROTATE;
        uint64_t l_nibbleIndx               =   0;
        uint64_t l_bitsDecoded              =   0;
        bool l_decompressionDone            =   false;
        uint32_t l_scanAddr                 =   rs4_revle32( l_rs4Header->iv_scanAddr );
        uint64_t l_scanRegion               =   decodeScanRegionData( l_scanAddr, i_regionSelect );
        uint8_t l_chipletId                 =   ( l_scanAddr & 0xFF000000UL ) >> 24;
        uint16_t l_ringId                   =   l_rs4Header->iv_ringId;
        uint8_t l_mask                      =   0x08;
        uint64_t l_scomData                 =   0x0;
        MyBool_t l_bOverride                =   UNDEFINED_BOOLEAN;

        do
        {
            //Determine Override/flush status

            if ( i_applyOverride )
            {
                if( UNDEFINED_BOOLEAN == i_applyOverride )
                {
                    if( ( l_rs4Header->iv_type & RS4_IV_TYPE_OVRD_MASK ) == RS4_IV_TYPE_OVRD_OVRD )
                    {
                        l_bOverride     =   true;
                    }
                    else if( ( l_rs4Header->iv_type & RS4_IV_TYPE_OVRD_MASK ) == RS4_IV_TYPE_OVRD_FLUSH )
                    {
                        l_bOverride = false;
                    }
                }

                else if( true == i_applyOverride )
                {
                    l_bOverride     =   true;
                }
                else
                {
                    FAPI_ASSERT( false,
                                 fapi2::RS4_BAD_RING_TYPE()
                                 .set_RING_TYPE( l_rs4Header->iv_type )
                                 .set_RING_ID( l_ringId )
                                 .set_INPUT_OVERRIDE( i_applyOverride ),
                                 "Bad Ring Type Field In RS4 Container 0x%02x", l_rs4Header->iv_type );
                    break;
                }
            }
            else if( !i_applyOverride )
            {
                l_bOverride     =   false;
            }

            // Set up the scan region for the ring.
            l_rc = setupScanRegion( i_target, l_scanRegion, l_chipletId, i_ringType, i_opMode );

            if( l_rc != fapi2::FAPI2_RC_SUCCESS )
            {
                break;
            }

            if( TEST_MODE == i_opMode )
            {
                goto fapi_try_exit;
            }

            // Write a 64 bit value for header.
            const uint64_t l_header     =   0xa5a5a5a5a5a5a5a5;
            l_rc                        =   writeHeader( i_target, l_header, l_chipletId );

            if( l_rc != fapi2::FAPI2_RC_SUCCESS )
            {
                break;
            }

            bool l_skip_64bits = true;

            // Decompress the RS4 string and scan
            do
            {
                if ( l_opType == ROTATE )
                {
                    // Determine the no.of ROTATE operations encoded in stop-code
                    uint64_t l_count    =   0;
                    l_nibbleIndx       +=   stop_decode( l_rs4Str, l_nibbleIndx, l_count );

                    // Determine the no.of rotates in bits
                    uint64_t l_bitRotates   =   ( 4 * l_count );

                    //Need to skip 64bits , because we have already written header
                    //data.
                    if ( l_skip_64bits && ( l_bitRotates >= SIXTYFOUR_BIT_HEADER ) )
                    {
                        l_bitRotates    -=  SIXTYFOUR_BIT_HEADER;
                        l_skip_64bits    =  false;
                    }

                    l_bitsDecoded   +=  l_bitRotates;

                    // Do the ROTATE operation
                    if ( l_bitRotates != 0 )
                    {
                        l_rc = standardScan( i_target,
                                             l_chipletId,
                                             ROTATE,
                                             l_bitRotates );

                        if( l_rc  !=  fapi2::FAPI2_RC_SUCCESS )
                        {
                            break;
                        }
                    }

                    l_opType = SCAN;
                }
                else if( l_opType == SCAN )
                {
                    uint8_t l_scanCount = rs4_get_nibble( l_rs4Str, l_nibbleIndx );
                    l_nibbleIndx++;

                    if ( 0 == l_scanCount )
                    {
                        FAPI_INF("SCAN COUNT %u", l_scanCount);
                        break;
                    }

                    if (( !l_bOverride ) && ( l_scanCount != 0xF ) )  // Process flush ring (and ensure no '0'-writes)
                    {
                        l_bitsDecoded += (4 * l_scanCount);

                        // Parse the non-zero nibbles of the RS4 string and
                        // scan them into the ring
                        l_scomData = rs4_get_verbatim( l_rs4Str,
                                                       l_nibbleIndx,
                                                       l_scanCount );
                        l_nibbleIndx += l_scanCount;

                        l_rc = standardScan( i_target,
                                             l_chipletId,
                                             SCAN,
                                             ( l_scanCount * 4 ),
                                             l_scomData );

                        if( l_rc  !=  fapi2::FAPI2_RC_SUCCESS )
                        {
                            break;
                        }
                    }
                    else    // Process override ring (plus the occasional flush ring with '0'-write bits)
                    {
                        if( 0xF == l_scanCount ) // Process ring with '0'-write bits
                        {
                            i_applyOverride = true;
                            uint8_t l_careMask = rs4_get_nibble( l_rs4Str, l_nibbleIndx );
                            l_nibbleIndx++;
                            uint8_t l_spyData = rs4_get_nibble( l_rs4Str, l_nibbleIndx );
                            l_nibbleIndx++;

                            for( uint8_t i = 0; i < 4; i++ )
                            {
                                l_bitsDecoded   +=  1;
                                l_scomData       =  0x0;

                                if( ( l_careMask & ( l_mask >> i ) ) )
                                {
                                    if( ( l_spyData & ( l_mask >> i ) ) )
                                    {
                                        l_scomData = 0xFFFFFFFFFFFFFFFF;
                                    }

                                    l_opType = SCAN;
                                }
                                else
                                {
                                    l_opType = ROTATE;
                                }

                                l_rc = standardScan( i_target,
                                                     l_chipletId,
                                                     l_opType,
                                                     1, // Insert 1 bit
                                                     l_scomData );

                                if( l_rc    !=  fapi2::FAPI2_RC_SUCCESS )
                                {
                                    break;
                                }
                            }
                        }
                        else    // Process ring with only '1'-write bits
                        {
                            for ( uint8_t x = 0; x < l_scanCount; x++ )
                            {
                                // Parse the non-zero nibbles of the RS4 string and
                                // scan them into the ring
                                uint8_t l_data      =    rs4_get_nibble( l_rs4Str, l_nibbleIndx );
                                l_nibbleIndx       +=    1;

                                FAPI_INF ( "VERBATIm l_nibbleIndx %u l_scanCount %u "
                                           "l_bitsDecoded %u", l_nibbleIndx, l_scanCount, l_bitsDecoded );

                                for( uint8_t i = 0; i < 4; i++ )
                                {
                                    l_scomData      =    0x0;

                                    if( ( l_data & ( l_mask >> i ) ) )
                                    {
                                        l_opType    =   SCAN;
                                        l_scomData  =   0xFFFFFFFFFFFFFFFF;
                                    }
                                    else
                                    {
                                        l_opType    =   ROTATE;
                                    }

                                    l_rc = standardScan( i_target,
                                                         l_chipletId,
                                                         l_opType,
                                                         1, // Insert 1 bit
                                                         l_scomData );

                                    if( l_rc != fapi2::FAPI2_RC_SUCCESS )
                                    {
                                        break;
                                    }
                                }

                                if( l_rc != fapi2::FAPI2_RC_SUCCESS )
                                {
                                    break;
                                }
                            } // end of looper for bit-parsing a non-zero nibble
                        }
                    }

                    l_opType = ROTATE;
                } // end of - if(l_opType == SCAN)

                if( l_rc != fapi2::FAPI2_RC_SUCCESS )
                {
                    break;
                }
            }
            while(1);

            if(( l_rc != fapi2::FAPI2_RC_SUCCESS ) || ( true == l_decompressionDone ) )
            {
                break;
            }

            // Handle the string termination
            uint8_t l_nibble    =   rs4_get_nibble( l_rs4Str, l_nibbleIndx );
            l_nibbleIndx++;

            if ( l_nibble != 0 )
            {
                if ( ( !l_bOverride ) && !( l_nibble & 0x8 ) ) // Process flush ring (and ensure no '0'-writes))
                {
                    l_bitsDecoded += l_nibble;
                    l_scomData = rs4_get_verbatim( l_rs4Str,
                                                   l_nibbleIndx,
                                                   1 ); // return 1 nibble

                    l_rc = standardScan( i_target,
                                         l_chipletId,
                                         SCAN,
                                         l_nibble & 0x3,
                                         l_scomData );
                }
                else // Process override ring (plus the occasional flush ring with '0'-write bits)
                {
                    if( 0x8 & l_nibble ) // Process ring with '0'-write bits
                    {
                        uint8_t l_careMask  =   rs4_get_nibble(l_rs4Str, l_nibbleIndx);
                        l_nibbleIndx++;
                        uint8_t l_spyData   =   rs4_get_nibble(l_rs4Str, l_nibbleIndx);
                        l_nibbleIndx++;

                        for( uint8_t i = 0; i < ( l_nibble & 0x3 ); i++ )
                        {
                            l_bitsDecoded += 1;
                            l_scomData = 0x0;

                            if( ( l_careMask & ( l_mask >> i ) ) )
                            {
                                if( ( l_spyData & ( l_mask >> i ) ) )
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
                        }
                    }
                    else // Process ring with only '1'-write bits
                    {
                        // scan them into the ring
                        uint8_t l_data      =   rs4_get_nibble( l_rs4Str, l_nibbleIndx );
                        l_nibbleIndx       +=   1;

                        for( uint8_t i = 0; i < l_nibble; i++ )
                        {
                            l_scomData      =   0x0;
                            l_bitsDecoded   +=  1;

                            if(( l_data & ( l_mask >> i ) ) )
                            {
                                l_opType    =   SCAN;
                                l_scomData  =   0xFFFFFFFFFFFFFFFF;

                            }
                            else
                            {
                                l_opType    =   ROTATE;
                            }

                            l_rc    =   standardScan( i_target,
                                                      l_chipletId,
                                                      l_opType,
                                                      1, // Insert 1 bit
                                                      l_scomData );

                            if( l_rc  !=  fapi2::FAPI2_RC_SUCCESS )
                            {
                                break;
                            }
                        } //end of for
                    }
                }
            } // end of if(l_nibble != 0)

            if( l_rc  !=  fapi2::FAPI2_RC_SUCCESS )
            {
                break;
            }

            // Verify header
            l_rc    =   verifyHeader( i_target, l_chipletId, i_ringMode, l_bitsDecoded, l_ringId );

            if( l_rc )
            {
                break;
            }

            // Clean scan region and type data
            l_rc = cleanScanRegionandTypeData( i_target, l_chipletId );

            if( l_rc )
            {
                break;
            }
        }
        while( 0 );

    fapi_try_exit:

        FAPI_INF( "<< p10_putRingUtils" );
        return l_rc;
    }

};   //extern "C"

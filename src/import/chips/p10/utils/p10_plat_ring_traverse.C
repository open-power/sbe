/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/utils/p10_plat_ring_traverse.C $         */
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
#include <fapi2.H>
#include <p10_tor.H>
#include <common_ringId.H>
#include <p10_ipl_image.H>
#include <p10_plat_ring_traverse.H>
#include <p10_putRingUtils.H>
#include <p10_ringId.H>
#include <p10_scan_compression.H>

//-------------------------------------------------------------------------------------------------

using namespace RS4;
fapi2::ReturnCode lookUpRingSection( uint8_t* i_pImgPtr,
                                     const fapi2::Target<fapi2::TARGET_TYPE_ALL_MC>& i_target,
                                     const RingId_t i_ringId,
                                     const fapi2::RingMode i_ringMode,
                                     const uint32_t i_regionSelect,
                                     OpMode_t i_opMode )
{
    FAPI_INF( ">> lookUpRingSection" );
    fapi2::ReturnCode l_rc      =   fapi2::FAPI2_RC_SUCCESS;
    P9XipHeader* l_imgHdr       =   (P9XipHeader*)( i_pImgPtr );
    P9XipSection* l_pSection    =   &l_imgHdr->iv_section[P9_XIP_SECTION_SBE_RINGS];
    TorHeader_t* l_torHeader    =   NULL;

    FAPI_ASSERT( ( l_pSection->iv_offset > 0 ),
                 fapi2::INVALID_RING_SECTION()
                 .set_RING_OFFSET( l_pSection->iv_offset ),
                 "Invalid Offset To Ring In SBE Image 0x%08x", rev_32( l_pSection->iv_offset  ));

    l_torHeader     =   (TorHeader_t*) ( i_pImgPtr + rev_32( l_pSection->iv_offset ) );

    FAPI_ASSERT( TOR_MAGIC_SBE == rev_32( l_torHeader->magic ),
                 fapi2::INVALID_RING_CHIPLET_SECTION()
                 .set_TOR_MAGIC_WORD( l_torHeader->magic ),
                 "Invalid Offset To Chiplet Section 0x%08x Magic Word 0x%08x",
                 rev_32( l_pSection->iv_offset ), rev_32( l_torHeader->magic ) );

    //FIXME RTC  212097 Need to handle the case of override.

    FAPI_TRY( getRS4ImageFromTor( i_target,
                                  (i_pImgPtr + rev_32( l_pSection->iv_offset )),
                                  i_ringId,
                                  false,
                                  i_ringMode,
                                  i_regionSelect,
                                  i_opMode ) );

    FAPI_INF( "<< lookUpRingSection" );
fapi_try_exit:
    return fapi2::current_err;
}

//-------------------------------------------------------------------------------------------------

fapi2::ReturnCode getRS4ImageFromTor(
    const fapi2::Target<fapi2::TARGET_TYPE_ALL_MC>& i_target,
    uint8_t* i_pChipletSectn,
    const RingId_t i_ringId,
    bool i_applyOverride,
    const fapi2::RingMode i_ringMode,
    const uint32_t i_regionSelect,
    OpMode_t i_opMode )
{
    uint32_t l_torHdrSize       =   sizeof( TorHeader_t );
    uint32_t l_torVersion       =   ((TorHeader_t*)i_pChipletSectn)->version;
    SectionTOR* l_pSectnTor     =   (SectionTOR*) ( i_pChipletSectn + l_torHdrSize );
    uint32_t l_torOffset        =   0;
    uint32_t l_sectionOffset    =   0;
    uint32_t l_ringIndexOffset  =   0;
    RingType_t l_ringType       =   COMMON_RING;
    ChipletType_t l_chipletType;
    fapi2::ReturnCode l_rc      =   fapi2::FAPI2_RC_SUCCESS;
    uint8_t l_chipletID         =   i_target.getChipletNumber();
    ChipletData_t* l_pChipletData;
    uint16_t* l_pRingTor        =   NULL;
    uint8_t* l_pRs4             =   NULL;

    FAPI_ASSERT( ( l_torVersion == TOR_VERSION ),
                 fapi2::INVALID_TOR_VERSION()
                 .set_TOR_VER( l_torVersion ),
                 "Invalid TOR Version 0x%04x ", l_torVersion  );

    FAPI_ASSERT( ( i_ringId < NUM_RING_IDS ),
                 fapi2::INVALID_RING_ID()
                 .set_RING_ID( i_ringId ),
                 "Invalid Ring Id 0x%04x", i_ringId );

    l_torOffset     =   ( INSTANCE_RING_MASK    &  ( RING_PROPERTIES[(uint8_t)i_ringId].idxRing ) );
    l_ringType      =   ( INSTANCE_RING_MARK    &  ( RING_PROPERTIES[(uint8_t)i_ringId].idxRing ) ) ?
                        INSTANCE_RING : COMMON_RING;

    l_chipletType   =   RING_PROPERTIES[(uint8_t)i_ringId].chipletType;

    switch( l_chipletType )
    {
        case PERV_TYPE: // PERV
            l_pChipletData      =   (ChipletData_t*)&PERV::g_chipletData;
            l_sectionOffset     =   rev_32( l_pSectnTor->TOC_PERV_COMMON_RING );

            if( INSTANCE_RING == l_ringType )
            {
                l_sectionOffset = rev_32(l_pSectnTor->TOC_PERV_INSTANCE_RING);
            }

            break;

        case N0_TYPE: // Nest - N0

            l_pChipletData      =   (ChipletData_t*)&N0::g_chipletData;
            l_sectionOffset     =   rev_32(l_pSectnTor->TOC_N0_COMMON_RING);

            if( INSTANCE_RING == l_ringType )
            {
                l_sectionOffset =   rev_32(l_pSectnTor->TOC_N0_INSTANCE_RING);
            }

            break;

        case N1_TYPE: // Nest - N1

            l_pChipletData      =   (ChipletData_t*)&N1::g_chipletData;
            l_sectionOffset     =   rev_32(l_pSectnTor->TOC_N1_COMMON_RING);

            if( INSTANCE_RING == l_ringType )
            {
                l_sectionOffset =   rev_32(l_pSectnTor->TOC_N1_INSTANCE_RING);
            }

            break;

        case PCI_TYPE: // PCI

            l_pChipletData      =   (ChipletData_t*)&PCI::g_chipletData;
            l_sectionOffset     =   rev_32(l_pSectnTor->TOC_PCI_COMMON_RING);

            if( INSTANCE_RING == l_ringType )
            {
                l_sectionOffset =   rev_32(l_pSectnTor->TOC_PCI_INSTANCE_RING);
            }

            break;

        case MC_TYPE: // MC

            l_pChipletData      =   (ChipletData_t*)&MC::g_chipletData;
            l_sectionOffset     =   rev_32(l_pSectnTor->TOC_MC_COMMON_RING);

            if( INSTANCE_RING == l_ringType )
            {
                l_sectionOffset =   rev_32(l_pSectnTor->TOC_MC_INSTANCE_RING);
            }

            break;

        case PAU0_TYPE: // PAU0

            l_pChipletData      =   (ChipletData_t*)&PAU0::g_chipletData;
            l_sectionOffset     =   rev_32(l_pSectnTor->TOC_PAU0_COMMON_RING);

            if( INSTANCE_RING == l_ringType )
            {
                l_sectionOffset =   rev_32(l_pSectnTor->TOC_PAU0_INSTANCE_RING);
            }

            break;

        case PAU1_TYPE: // PAU1

            l_pChipletData      =   (ChipletData_t*)&PAU1::g_chipletData;
            l_sectionOffset     =   rev_32(l_pSectnTor->TOC_PAU1_COMMON_RING);

            if( INSTANCE_RING == l_ringType )
            {
                l_sectionOffset =   rev_32(l_pSectnTor->TOC_PAU1_INSTANCE_RING);
            }

            break;

        case PAU2_TYPE: // PAU2

            l_pChipletData      =   (ChipletData_t*)&PAU2::g_chipletData;
            l_sectionOffset     =   rev_32(l_pSectnTor->TOC_PAU2_COMMON_RING);

            if( INSTANCE_RING == l_ringType )
            {
                l_sectionOffset =   rev_32(l_pSectnTor->TOC_PAU2_INSTANCE_RING);
            }

            break;

        case PAU3_TYPE: // PAU3

            l_pChipletData      =   (ChipletData_t*)&PAU3::g_chipletData;
            l_sectionOffset     =   rev_32(l_pSectnTor->TOC_PAU3_COMMON_RING);

            if( INSTANCE_RING == l_ringType )
            {
                l_sectionOffset =   rev_32(l_pSectnTor->TOC_PAU3_INSTANCE_RING);
            }

            break;

        case AXON_TYPE: // AXON

            l_pChipletData      =   (ChipletData_t*)&AXON::g_chipletData;
            l_sectionOffset     =   rev_32(l_pSectnTor->TOC_AXON_COMMON_RING);

            if( INSTANCE_RING == l_ringType )
            {
                l_sectionOffset =   rev_32(l_pSectnTor->TOC_AXON_INSTANCE_RING);
            }

            break;

        case EQ_TYPE: // EQ - Quad 0 - Quad 5

            l_pChipletData      =   (ChipletData_t*)&EQ::g_chipletData;
            l_sectionOffset     =   rev_32(l_pSectnTor->TOC_EQ_COMMON_RING);

            if( INSTANCE_RING == l_ringType )
            {
                l_sectionOffset = rev_32(l_pSectnTor->TOC_EQ_INSTANCE_RING);
            }

            break;

        default:
            fapi2::current_err = fapi2::FAPI2_RC_INVALID_PARAMETER;
            goto fapi_try_exit;
            break;

    } // end of switch(l_chipletID)

    l_pRs4              =   i_pChipletSectn + l_torHdrSize + l_sectionOffset + (2 * l_torOffset) +
                            ( l_chipletType * 8 ) + (( INSTANCE_RING == l_ringType ) ? 4 : 0);
    l_pRingTor          =   (uint16_t*) l_pRs4;

    FAPI_INF( "TOR Traversal: Ring Id 0x%08x Sectn Offset 0x%08x Chiplet Offset 0x%08x Ring offset 0x%08x",
              i_ringId, (*((uint32_t*)i_pChipletSectn)), l_sectionOffset, l_torOffset );

    if( INSTANCE_RING == l_ringType )
    {
        if( l_chipletID >= l_pChipletData->chipletBaseId )
        {
            l_ringIndexOffset   =   l_chipletID - l_pChipletData->chipletBaseId;
            l_pRingTor         +=   ( l_pChipletData->numInstanceRings * l_ringIndexOffset );
            //FIXME RTC 212097 Need to review instance ring traversal, once repair ring properties
            //get updated in infrastructure code.
        }
    }

    if( *l_pRingTor )
    {
        l_pRs4  +=  rev_16(*l_pRingTor);
        CompressedScanData* l_pRs4Hdr   =  (CompressedScanData*)l_pRs4;

        FAPI_ASSERT( ( rev_16(l_pRs4Hdr->iv_ringId) == i_ringId ),
                     fapi2::TOR_TRAVERSAL_ERROR()
                     .set_ACTUAL_RING_ID( rev_16( l_pRs4Hdr->iv_ringId ) )
                     .set_EXPECTED_RING_ID( i_ringId ),
                     "Failed To Traverse To Correct Ring. Found 0x%04x Expected  0x%04x ",
                     rev_16( l_pRs4Hdr->iv_ringId ), i_ringId );

        FAPI_EXEC_HWP( l_rc,
                       p10_putRingUtils,
                       i_target,
                       l_pRs4,
                       i_applyOverride,
                       i_ringMode,
                       l_ringType,
                       i_regionSelect,
                       i_opMode );

        if( l_rc )
        {
            fapi2::current_err  =   l_rc;
        }
    }
    else
    {
        FAPI_INF( "TOR slot not populated for ring 0x%04x ", (uint16_t) i_ringId );
    }

fapi_try_exit:
    return fapi2::current_err;
}

//-------------------------------------------------------------------------------------------------
extern "C"
{
    /**
     * @brief       cronus interface for verification of ring traversal.
     * @param[in]   i_pImgPtr   points to SBE SEEPROM image.
     * @param[in]   i_target    fapi2 target
     * @param[in]   i_ringId    ring to be scanned.
     * @param[in]   i_ringMode  ring mode.
     */
    fapi2::ReturnCode p10_plat_ring_traverse(
        uint8_t* i_pImgPtr,
        const fapi2::Target<fapi2::TARGET_TYPE_ALL>& i_target,
        const RingId_t i_ringId,
        const fapi2::RingMode i_ringMode,
        const uint32_t i_regionSelect,
        OpMode_t i_opMode )
    {

        FAPI_TRY( lookUpRingSection( i_pImgPtr, i_target, i_ringId, i_ringMode, i_regionSelect, i_opMode ) );
    fapi_try_exit:
        return fapi2::current_err;
    }
}
//-------------------------------------------------------------------------------------------------

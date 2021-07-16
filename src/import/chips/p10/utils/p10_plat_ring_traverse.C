/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/utils/p10_plat_ring_traverse.C $         */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2019,2021                        */
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
OpMode_t G_opMode = SCANNING_MODE;


fapi2::ReturnCode lookUpRingSection( uint8_t* i_pImgPtr,
                                     const fapi2::Target<fapi2::TARGET_TYPE_ALL_MC>& i_target,
                                     const RingId_t i_ringId,
                                     const fapi2::RingMode i_ringMode)
{
    FAPI_INF( ">> lookUpRingSection" );
#ifndef DFT
    // For non-DFT, find offset to TOR in the XIP header
    p9_xip_section_sbe_t l_sectionName = (i_ringMode & fapi2::RING_MODE_FASTARRAY) ? P9_XIP_SECTION_SBE_FA_RING_OVRD :
                                         P9_XIP_SECTION_SBE_RINGS;

    P9XipHeader* l_imgHdr       =   (P9XipHeader*)( i_pImgPtr );
    P9XipSection* l_pSection    =   &l_imgHdr->iv_section[l_sectionName];
    TorHeader_t* l_torHeader    =   NULL;

    // Creating local variables for creating ffdc for image pointers
    uint64_t l_imgPtr;

    l_imgPtr = reinterpret_cast<uint64_t>(i_pImgPtr);

    FAPI_ASSERT( ( (l_pSection->iv_offset) > 0 ),
                 fapi2::INVALID_RING_SECTION()
                 .set_RING_MODE(i_ringMode)
                 .set_SECTION_NAME(l_sectionName)
                 .set_IMG_POINTER(l_imgPtr)
                 .set_RING_OFFSET( l_pSection->iv_offset),
                 "Invalid Offset To Ring In SBE Image 0x%08x", rev_32( l_pSection->iv_offset));

    l_torHeader     =   (TorHeader_t*) ( i_pImgPtr + rev_32( l_pSection->iv_offset) );

    FAPI_ASSERT( TOR_MAGIC == (rev_32( l_torHeader->magic ) >> 8),
                 fapi2::INVALID_RING_CHIPLET_SECTION()
                 .set_RING_MODE(i_ringMode)
                 .set_SECTION_NAME(l_sectionName)
                 .set_IMG_POINTER(l_imgPtr)
                 .set_RING_OFFSET( l_pSection->iv_offset)
                 .set_TOR_MAGIC_WORD( l_torHeader->magic ),
                 "Invalid Offset To Chiplet Section 0x%08x Magic Word 0x%08x",
                 rev_32( l_pSection->iv_offset), rev_32( l_torHeader->magic ) );

    //FIXME RTC  212097 Need to handle the case of override.

    FAPI_TRY( getRS4ImageFromTor( i_target,
                                  (i_pImgPtr + rev_32( l_pSection->iv_offset)),
                                  i_ringId,
                                  false,
                                  i_ringMode) );

    //Scan Override rings if Present
    l_pSection    =   &l_imgHdr->iv_section[P9_XIP_SECTION_SBE_OVERRIDES];

    if( !rev_32(l_pSection->iv_size) )
    {
        goto fapi_try_exit;
    }

    FAPI_TRY( getRS4ImageFromTor( i_target,
                                  (i_pImgPtr + rev_32( l_pSection->iv_offset)),
                                  i_ringId,
                                  false,
                                  i_ringMode) );
#else
    // For DFT, find offset to TOR using attributes
    fapi2::ATTR_RING_IMG_CNFG_Type l_ringImgCnfg = 0;
    fapi2::ATTR_HW_IMG_PTR_Type l_imgAddr = 0;
    uint8_t* l_pImgPtr = 0;

    fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> l_procTgt   =
        i_target.template getParent<fapi2::TARGET_TYPE_PROC_CHIP> ();

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_RING_IMG_CNFG, l_procTgt, l_ringImgCnfg));

    if (l_ringImgCnfg == 0)
    {
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_HW_IMG_PTR, l_procTgt, l_imgAddr));
        FAPI_DBG("DFT putRing reading appended BASE RS4 image beginning at 0x%.8x", l_imgAddr);
    }
    else
    {
        FAPI_ERR("Unknown image pointer %.2x in ATTR_RING_IMG_CNFG", l_ringImgCnfg);
    }

    l_pImgPtr = (uint8_t*) l_imgAddr;

    FAPI_TRY( getRS4ImageFromTor( i_target,
                                  l_pImgPtr,
                                  i_ringId,
                                  false,
                                  i_ringMode) );
#endif

    FAPI_INF( "<< lookUpRingSection" );
fapi_try_exit:
    return fapi2::current_err;
}

//-------------------------------------------------------------------------------------------------

fapi2::ReturnCode getRS4ImageFromTor(
    const fapi2::Target<fapi2::TARGET_TYPE_ALL_MC>& i_target,
    uint8_t* i_pRingSectn, // Pts to the beginning of the TOR ring section hdr
    const RingId_t i_ringId,
    bool i_applyOverride,
    const fapi2::RingMode i_ringMode)
{
    uint32_t l_torVersion       =   ((TorHeader_t*)i_pRingSectn)->version;
    SectionTOR* l_pSectnTor     =   (SectionTOR*) ( i_pRingSectn + sizeof( TorHeader_t ) );
    uint8_t  l_torIndex         =   0;
    TorOffset_t l_sectionOffset =   0;
    RingType_t l_ringType       =   COMMON_RING;
    ChipletType_t l_chipletType;
    RingId_t l_rpIndex          = UNDEFINED_RING_ID;
    fapi2::ReturnCode l_rc      =   fapi2::FAPI2_RC_SUCCESS;
    ChipletData_t* l_pChipletData;
    TorOffset_t* l_pRingTor     =   NULL;
    fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> l_procTgt   =
        i_target.template getParent<fapi2::TARGET_TYPE_PROC_CHIP> ();

    OpMode_t i_opMode = G_opMode;

    FAPI_ASSERT( ( l_torVersion == TOR_VERSION ),
                 fapi2::INVALID_TOR_VERSION()
                 .set_TOR_VER( l_torVersion ),
                 "Invalid TOR Version 0x%04x ", l_torVersion  );

    // Get the ring properties (rp) index
    l_rpIndex = ringid_convert_ringId_to_rpIndex(i_ringId);

    FAPI_ASSERT( ( l_rpIndex < NUM_RING_IDS ),
                 fapi2::INVALID_RING_ID()
                 .set_RING_ID( i_ringId ),
                 "Invalid Ring Id 0x%04x", i_ringId );

    l_torIndex      =   ( INSTANCE_RING_MASK    &  ( RING_PROPERTIES[l_rpIndex].idxRing ) );
    l_ringType      =   ( INSTANCE_RING_MARK    &  ( RING_PROPERTIES[l_rpIndex].idxRing ) ) ?
                        INSTANCE_RING : COMMON_RING;

    l_chipletType   =   RING_PROPERTIES[l_rpIndex].chipletType;

    switch( l_chipletType )
    {
        case PERV_TYPE: // PERV
            l_pChipletData      =   (ChipletData_t*)&PERV::g_chipletData;

            l_sectionOffset = ( INSTANCE_RING == l_ringType ) ?
                              rev_16(l_pSectnTor->TOC_PERV_INSTANCE_RING) :
                              rev_16(l_pSectnTor->TOC_PERV_COMMON_RING);

            break;

        case N0_TYPE: // Nest - N0

            l_pChipletData      =   (ChipletData_t*)&N0::g_chipletData;

            l_sectionOffset = ( INSTANCE_RING == l_ringType ) ?
                              rev_16(l_pSectnTor->TOC_N0_INSTANCE_RING) :
                              rev_16(l_pSectnTor->TOC_N0_COMMON_RING);

            break;

        case N1_TYPE: // Nest - N1

            l_pChipletData      =   (ChipletData_t*)&N1::g_chipletData;

            l_sectionOffset = ( INSTANCE_RING == l_ringType ) ?
                              rev_16(l_pSectnTor->TOC_N1_INSTANCE_RING) :
                              rev_16(l_pSectnTor->TOC_N1_COMMON_RING);

            break;

        case PCI_TYPE: // PCI

            l_pChipletData      =   (ChipletData_t*)&PCI::g_chipletData;

            l_sectionOffset = ( INSTANCE_RING == l_ringType ) ?
                              rev_16(l_pSectnTor->TOC_PCI_INSTANCE_RING) :
                              rev_16(l_pSectnTor->TOC_PCI_COMMON_RING);

            break;

        case MC_TYPE: // MC

            l_pChipletData      =   (ChipletData_t*)&MC::g_chipletData;

            l_sectionOffset = ( INSTANCE_RING == l_ringType ) ?
                              rev_16(l_pSectnTor->TOC_MC_INSTANCE_RING) :
                              rev_16(l_pSectnTor->TOC_MC_COMMON_RING);

            break;

        case PAU0_TYPE: // PAU0

            l_pChipletData      =   (ChipletData_t*)&PAU0::g_chipletData;

            l_sectionOffset = ( INSTANCE_RING == l_ringType ) ?
                              rev_16(l_pSectnTor->TOC_PAU0_INSTANCE_RING) :
                              rev_16(l_pSectnTor->TOC_PAU0_COMMON_RING);

            break;

        case PAU1_TYPE: // PAU1

            l_pChipletData      =   (ChipletData_t*)&PAU1::g_chipletData;

            l_sectionOffset = ( INSTANCE_RING == l_ringType ) ?
                              rev_16(l_pSectnTor->TOC_PAU1_INSTANCE_RING) :
                              rev_16(l_pSectnTor->TOC_PAU1_COMMON_RING);

            break;

        case PAU2_TYPE: // PAU2

            l_pChipletData      =   (ChipletData_t*)&PAU2::g_chipletData;

            l_sectionOffset = ( INSTANCE_RING == l_ringType ) ?
                              rev_16(l_pSectnTor->TOC_PAU2_INSTANCE_RING) :
                              rev_16(l_pSectnTor->TOC_PAU2_COMMON_RING);

            break;

        case PAU3_TYPE: // PAU3

            l_pChipletData      =   (ChipletData_t*)&PAU3::g_chipletData;

            l_sectionOffset = ( INSTANCE_RING == l_ringType ) ?
                              rev_16(l_pSectnTor->TOC_PAU3_INSTANCE_RING) :
                              rev_16(l_pSectnTor->TOC_PAU3_COMMON_RING);

            break;

        case AXON0_TYPE: // AXON0

            l_pChipletData      =   (ChipletData_t*)&AXON0::g_chipletData;

            l_sectionOffset = ( INSTANCE_RING == l_ringType ) ?
                              rev_16(l_pSectnTor->TOC_AXON0_INSTANCE_RING) :
                              rev_16(l_pSectnTor->TOC_AXON0_COMMON_RING);

            break;

        case AXON1_TYPE: // AXON1

            l_pChipletData      =   (ChipletData_t*)&AXON1::g_chipletData;

            l_sectionOffset = ( INSTANCE_RING == l_ringType ) ?
                              rev_16(l_pSectnTor->TOC_AXON1_INSTANCE_RING) :
                              rev_16(l_pSectnTor->TOC_AXON1_COMMON_RING);

            break;

        case AXON2_TYPE: // AXON2

            l_pChipletData      =   (ChipletData_t*)&AXON2::g_chipletData;

            l_sectionOffset = ( INSTANCE_RING == l_ringType ) ?
                              rev_16(l_pSectnTor->TOC_AXON2_INSTANCE_RING) :
                              rev_16(l_pSectnTor->TOC_AXON2_COMMON_RING);

            break;

        case AXON3_TYPE: // AXON3

            l_pChipletData      =   (ChipletData_t*)&AXON3::g_chipletData;

            l_sectionOffset = ( INSTANCE_RING == l_ringType ) ?
                              rev_16(l_pSectnTor->TOC_AXON3_INSTANCE_RING) :
                              rev_16(l_pSectnTor->TOC_AXON3_COMMON_RING);

            break;

        case AXON4_TYPE: // AXON4

            l_pChipletData      =   (ChipletData_t*)&AXON4::g_chipletData;

            l_sectionOffset = ( INSTANCE_RING == l_ringType ) ?
                              rev_16(l_pSectnTor->TOC_AXON4_INSTANCE_RING) :
                              rev_16(l_pSectnTor->TOC_AXON4_COMMON_RING);

            break;

        case AXON5_TYPE: // AXON5

            l_pChipletData      =   (ChipletData_t*)&AXON5::g_chipletData;

            l_sectionOffset = ( INSTANCE_RING == l_ringType ) ?
                              rev_16(l_pSectnTor->TOC_AXON5_INSTANCE_RING) :
                              rev_16(l_pSectnTor->TOC_AXON5_COMMON_RING);

            break;

        case AXON6_TYPE: // AXON6

            l_pChipletData      =   (ChipletData_t*)&AXON6::g_chipletData;

            l_sectionOffset = ( INSTANCE_RING == l_ringType ) ?
                              rev_16(l_pSectnTor->TOC_AXON6_INSTANCE_RING) :
                              rev_16(l_pSectnTor->TOC_AXON6_COMMON_RING);

            break;

        case AXON7_TYPE: // AXON7

            l_pChipletData      =   (ChipletData_t*)&AXON7::g_chipletData;

            l_sectionOffset = ( INSTANCE_RING == l_ringType ) ?
                              rev_16(l_pSectnTor->TOC_AXON7_INSTANCE_RING) :
                              rev_16(l_pSectnTor->TOC_AXON7_COMMON_RING);

            break;

        case EQ_TYPE: // EQ - Quad 0 - Quad 7

            l_pChipletData      =   (ChipletData_t*)&EQ::g_chipletData;

            l_sectionOffset = ( INSTANCE_RING == l_ringType ) ?
                              rev_16(l_pSectnTor->TOC_EQ_INSTANCE_RING) :
                              rev_16(l_pSectnTor->TOC_EQ_COMMON_RING);

            break;

        default:
            fapi2::current_err = fapi2::RC_PUTRING_INVALID_PARAMETER;
            goto fapi_try_exit;
            break;

    } // end of switch(l_chipletID)

    l_pRingTor      =   (TorOffset_t*) (i_pRingSectn + l_sectionOffset);


    FAPI_INF( "TOR Traversal: Ring Id 0x%04x Sectn TOR 0x%08x Chiplet Offset 0x%04x Ring offset 0x%02x ",
              i_ringId, rev_32(*((uint32_t*)i_pRingSectn)), l_sectionOffset, l_torIndex );

    if( INSTANCE_RING == l_ringType )
    {
        fapi2::ATTR_SCAN_CHIPLET_OVERRIDE_Type l_scan_chiplet_override;
        FAPI_TRY( FAPI_ATTR_GET( fapi2::ATTR_SCAN_CHIPLET_OVERRIDE, l_procTgt, l_scan_chiplet_override ) );

        uint8_t l_chipletPos;

        if ( fapi2::ENUM_ATTR_SCAN_CHIPLET_OVERRIDE_NONE != l_scan_chiplet_override )
        {
            l_chipletPos = l_scan_chiplet_override;
            FAPI_INF("l_scan_chiplet_override l_chipletPos %02x", l_chipletPos);
        }
        else
        {
            // Try to reduce the target to chiplet type
            fapi2::Target < fapi2::TARGET_TYPE_CHIPLETS | fapi2::TARGET_TYPE_MULTICAST > l_cplt_target;
            fapi2::ReturnCode l_reduce_rc = i_target.reduceType(l_cplt_target);

            if (l_reduce_rc == fapi2::FAPI2_RC_SUCCESS)
            {
                // It's a chiplet, so grab the chiplet number
                l_chipletPos = l_cplt_target
                               .getParent < fapi2::TARGET_TYPE_PERV | fapi2::TARGET_TYPE_MULTICAST > ()
                               .getChildren< fapi2::TARGET_TYPE_PERV >()[0].getChipletNumber();
            }
            else
            {
                // It's not a chiplet, so it's the chip and we use the first chiplet ID
                l_chipletPos = l_pChipletData->chipletBaseId;
            }
        }

        l_chipletPos    -=   l_pChipletData->chipletBaseId;

        FAPI_INF("l_chipletPos %02x\n", l_chipletPos);
        l_pRingTor      +=   ( ( l_pChipletData->numInstanceRings * l_chipletPos ) + ( l_torIndex ) );
    }
    else
    {
        l_pRingTor  +=  l_torIndex;
    }

    if( *l_pRingTor )
    {
        FAPI_INF("*l_pRingTor %04x", rev_16(*l_pRingTor));
        uint8_t* l_pRs4  = i_pRingSectn + rev_16(*l_pRingTor);
        CompressedScanData* l_pRs4Hdr   =  (CompressedScanData*)l_pRs4;

        // Creating local variables for creating ffdc for image pointers
        uint64_t l_ringSectionAddr = reinterpret_cast<uint64_t>(i_pRingSectn);

        FAPI_ASSERT( ( rev_16(l_pRs4Hdr->iv_ringId) == i_ringId ),
                     fapi2::TOR_TRAVERSAL_ERROR()
                     .set_RING_SECTION(l_ringSectionAddr)
                     .set_SECTION_OFFSET(l_sectionOffset)
                     .set_RING_TYPE(l_ringType)
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
        OpMode_t i_opMode )
    {

        G_opMode = i_opMode;
        FAPI_TRY( lookUpRingSection( i_pImgPtr, i_target, i_ringId, i_ringMode) );
    fapi_try_exit:
        return fapi2::current_err;
    }
}
//-------------------------------------------------------------------------------------------------

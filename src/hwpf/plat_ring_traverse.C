/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/hwpf/plat_ring_traverse.C $                               */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2017                        */
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

#include <plat_ring_traverse.H>

#include <p9_putRingUtils.H> // for RS4 decompression utilities
#include <sbeXipUtils.H>
#include <fapi2_attribute_service.H> // for FAPI_ATTR_GET
#include <plat_target_utils.H> // for plat_getChipTarget
#include <p9_tor.H>

// SEEPROM start address
const uint32_t g_seepromAddr = SBE_SEEPROM_BASE_ORIGIN;
const uint32_t CACHE_CONTAINED_MODE = 4;
const uint32_t RISK_LEVEL_MODE = 1;
#define CACHE_CONTAINED_MODE_OFFSET_IN_TOR 1
#define RISK_LEVEL_MODE_OFFSET_IN_TOR      2
#define OVERRIDE_VARIANT_SIZE 1


///
/// @brief This is a plat pecific (SBE Plat) function that locates the
///        Ring Container in the image and calls the functin to decompress the
///        RS4 string and apply it to the hardware.
/// @param i_target The target of Ring apply.
/// @param i_ringID The Ring ID that identifies the ring to be applied.
/// @return FAPI2_RC_SUCCESS on success, else error code.
///
fapi2::ReturnCode findRS4InImageAndApply(
    const fapi2::Target<fapi2::TARGET_TYPE_ALL>& i_target,
    const RingId_t i_ringID,
    const fapi2::RingMode i_ringMode)
{
    SBE_TRACE(">> findRS4InImageAndApply");

    fapi2::ReturnCode l_rc = fapi2::FAPI2_RC_SUCCESS;
    bool l_applyOverride = false;

    do
    {
        //Apply scanring from .ring section

        // Get the address of the Section-TOR
        P9XipHeader* l_hdr = getXipHdr();
        P9XipSection* l_section =
            &(l_hdr->iv_section[P9_XIP_SECTION_SBE_RINGS]);

        if (!(l_section->iv_offset))
        {
            SBE_TRACE("No ring data in .RING section");
            break;
        }

        uint8_t l_torHdrSz;
        TorHeader_t* torHeader = (TorHeader_t*)(g_seepromAddr +
                                  l_section->iv_offset);

        if (torHeader->magic == TOR_MAGIC_SBE)
        {
            l_torHdrSz = sizeof(TorHeader_t);
        }
        else
        {
            // Note that after the merge of this code and after the TOR
            // magic merge on the HB side has settled down, the code
            // inside these bracket will be replaced with the return of
            // an error.
            l_torHdrSz = 0;
            SBE_TRACE("TOR magic header (=0x%08x) != TOR_MAGIC_SBE (=0x%08x)",
                      torHeader->magic, TOR_MAGIC_SBE);
        }

        SectionTOR* l_sectionTOR = (SectionTOR*)(g_seepromAddr +
                                   l_section->iv_offset +
                                   l_torHdrSz);

        l_rc = getRS4ImageFromTor(i_target, i_ringID, l_sectionTOR,
                                  l_applyOverride,
                                  l_section->iv_offset + l_torHdrSz,
                                  i_ringMode);

        if(l_rc != fapi2::FAPI2_RC_SUCCESS)
        {
            break;
        }

        //Apply scanring from .ring section
        l_applyOverride = true;
        l_section = NULL;
        l_section =
            &(l_hdr->iv_section[P9_XIP_SECTION_SBE_OVERRIDES]);

        if (!(l_section->iv_offset))
        {
            SBE_TRACE("No ring data in .OVERRIDE section");
            break;
        }

        l_sectionTOR = NULL;
        l_sectionTOR = (SectionTOR*)(g_seepromAddr +
                                     l_section->iv_offset +
                                     l_torHdrSz);


        l_rc = getRS4ImageFromTor(i_target, i_ringID, l_sectionTOR,
                                  l_applyOverride,
                                  l_section->iv_offset + l_torHdrSz,
                                  i_ringMode);
    }
    while(0);

    return l_rc;
}

fapi2::ReturnCode getRS4ImageFromTor(
    const fapi2::Target<fapi2::TARGET_TYPE_ALL>& i_target,
    const RingId_t i_ringID,
    SectionTOR* i_sectionTOR,
    bool i_applyOverride,
    const uint32_t i_sectionOffset,
    const fapi2::RingMode i_ringMode)
{

    // Determine the Offset ID and Ring Type for the given Ring ID.
    uint32_t l_torOffset = 0;
    RingType_t l_ringType = COMMON_RING;
    ChipletType_t l_chipLetType = INVALID_CHIPLET_TYPE;
    fapi2::ReturnCode l_rc = fapi2::FAPI2_RC_SUCCESS;

    do
    {

        getRingProperties(i_ringID, l_torOffset, l_ringType, l_chipLetType);

        if(INVALID_RING_OFFSET == l_torOffset)
        {
            SBE_TRACE("Invalid Ring ID - %d", i_ringID);
            l_rc = fapi2::FAPI2_RC_INVALID_PARAMETER;
            break;
        }

        CHIPLET_DATA l_chipletData;
        l_chipletData.iv_base_chiplet_number = 0;
        l_chipletData.iv_num_common_rings = 0;
        l_chipletData.iv_num_instance_rings = 0;

        uint8_t l_chipletID = i_target.getChipletNumber();
        uint16_t l_cpltRingVariantSz = 0;
        uint32_t l_sectionOffset = 0;
        uint8_t l_CC_offset = 0;
        uint8_t l_RL_offset = 1;

        switch(l_chipLetType)
        {
            case PERV_TYPE: // PERV
                l_chipletData = PERV::g_pervData;
                l_cpltRingVariantSz = i_applyOverride ? OVERRIDE_VARIANT_SIZE :
                                      (sizeof(PERV::RingVariants) /
                                       sizeof(l_cpltRingVariantSz));

                l_sectionOffset = i_sectionTOR->TOC_PERV_COMMON_RING;

                if(INSTANCE_RING == l_ringType)
                {
                    l_sectionOffset = i_sectionTOR->TOC_PERV_INSTANCE_RING;
                }

                break;

            case N0_TYPE: // Nest - N0
                l_chipletData = N0::g_n0Data;
                l_cpltRingVariantSz = i_applyOverride ? OVERRIDE_VARIANT_SIZE :
                                      (sizeof(N0::RingVariants) /
                                       sizeof(l_cpltRingVariantSz));

                l_sectionOffset = i_sectionTOR->TOC_N0_COMMON_RING;

                if(INSTANCE_RING == l_ringType)
                {
                    l_sectionOffset = i_sectionTOR->TOC_N0_INSTANCE_RING;
                }

                break;

            case N1_TYPE: // Nest - N1
                l_chipletData = N1::g_n1Data;
                l_cpltRingVariantSz = i_applyOverride ? OVERRIDE_VARIANT_SIZE :
                                      (sizeof(N1::RingVariants) /
                                       sizeof(l_cpltRingVariantSz));

                l_sectionOffset = i_sectionTOR->TOC_N1_COMMON_RING;

                if(INSTANCE_RING == l_ringType)
                {
                    l_sectionOffset = i_sectionTOR->TOC_N1_INSTANCE_RING;
                }

                break;

            case N2_TYPE: // Nest - N2
                l_chipletData = N2::g_n2Data;
                l_cpltRingVariantSz = i_applyOverride ? OVERRIDE_VARIANT_SIZE :
                                      (sizeof(N2::RingVariants) /
                                       sizeof(l_cpltRingVariantSz));

                l_sectionOffset = i_sectionTOR->TOC_N2_COMMON_RING;

                if(INSTANCE_RING == l_ringType)
                {
                    l_sectionOffset = i_sectionTOR->TOC_N2_INSTANCE_RING;
                }

                break;

            case N3_TYPE: // Nest - N3
                l_chipletData = N3::g_n3Data;
                l_cpltRingVariantSz = i_applyOverride ? OVERRIDE_VARIANT_SIZE :
                                      (sizeof(N3::RingVariants) /
                                       sizeof(l_cpltRingVariantSz));

                l_sectionOffset = i_sectionTOR->TOC_N3_COMMON_RING;

                if(INSTANCE_RING == l_ringType)
                {
                    l_sectionOffset = i_sectionTOR->TOC_N3_INSTANCE_RING;
                }

                break;

            case XB_TYPE: // XB - XBus2
                l_chipletData = XB::g_xbData;
                l_cpltRingVariantSz = i_applyOverride ? OVERRIDE_VARIANT_SIZE :
                                      (sizeof(XB::RingVariants) /
                                       sizeof(l_cpltRingVariantSz));

                l_sectionOffset = i_sectionTOR->TOC_XB_COMMON_RING;

                if(INSTANCE_RING == l_ringType)
                {
                    l_sectionOffset = i_sectionTOR->TOC_XB_INSTANCE_RING;
                }

                break;

            case MC_TYPE: // MC - MC23
                l_chipletData = MC::g_mcData;
                l_cpltRingVariantSz = i_applyOverride ? OVERRIDE_VARIANT_SIZE :
                                      (sizeof(MC::RingVariants) /
                                       sizeof(l_cpltRingVariantSz));

                l_sectionOffset = i_sectionTOR->TOC_MC_COMMON_RING;

                if(INSTANCE_RING == l_ringType)
                {
                    l_sectionOffset = i_sectionTOR->TOC_MC_INSTANCE_RING;
                }

                break;

            case OB0_TYPE: // OB0
                l_chipletData = OB0::g_ob0Data;
                l_cpltRingVariantSz = i_applyOverride ? OVERRIDE_VARIANT_SIZE :
                                      (sizeof(OB0::RingVariants) /
                                       sizeof(l_cpltRingVariantSz));

                l_sectionOffset = i_sectionTOR->TOC_OB0_COMMON_RING;

                if(INSTANCE_RING == l_ringType)
                {
                    l_sectionOffset = i_sectionTOR->TOC_OB0_INSTANCE_RING;
                }

                break;

            case OB1_TYPE: // OB1
                l_chipletData = OB1::g_ob1Data;
                l_cpltRingVariantSz = i_applyOverride ? OVERRIDE_VARIANT_SIZE :
                                      (sizeof(OB1::RingVariants) /
                                       sizeof(l_cpltRingVariantSz));

                l_sectionOffset = i_sectionTOR->TOC_OB1_COMMON_RING;

                if(INSTANCE_RING == l_ringType)
                {
                    l_sectionOffset = i_sectionTOR->TOC_OB1_INSTANCE_RING;
                }

                break;

            case OB2_TYPE: // OB2
                l_chipletData = OB2::g_ob2Data;
                l_cpltRingVariantSz = i_applyOverride ? OVERRIDE_VARIANT_SIZE :
                                      (sizeof(OB2::RingVariants) /
                                       sizeof(l_cpltRingVariantSz));

                l_sectionOffset = i_sectionTOR->TOC_OB2_COMMON_RING;

                if(INSTANCE_RING == l_ringType)
                {
                    l_sectionOffset = i_sectionTOR->TOC_OB2_INSTANCE_RING;
                }

                break;

            case OB3_TYPE: // OB3
                l_chipletData = OB3::g_ob3Data;
                l_cpltRingVariantSz = i_applyOverride ? OVERRIDE_VARIANT_SIZE :
                                      (sizeof(OB3::RingVariants) /
                                       sizeof(l_cpltRingVariantSz));

                l_sectionOffset = i_sectionTOR->TOC_OB3_COMMON_RING;

                if(INSTANCE_RING == l_ringType)
                {
                    l_sectionOffset = i_sectionTOR->TOC_OB3_INSTANCE_RING;
                }

                break;


            case PCI0_TYPE: // PCI - PCI0
                l_chipletData = PCI0::g_pci0Data;
                l_cpltRingVariantSz = i_applyOverride ? OVERRIDE_VARIANT_SIZE :
                                      (sizeof(PCI0::RingVariants) /
                                       sizeof(l_cpltRingVariantSz));

                l_sectionOffset = i_sectionTOR->TOC_PCI0_COMMON_RING;

                if(INSTANCE_RING == l_ringType)
                {
                    l_sectionOffset = i_sectionTOR->TOC_PCI0_INSTANCE_RING;
                }

                break;

            case PCI1_TYPE: // PCI - PCI1
                l_chipletData = PCI1::g_pci1Data;
                l_cpltRingVariantSz = i_applyOverride ? OVERRIDE_VARIANT_SIZE :
                                      (sizeof(PCI1::RingVariants) /
                                       sizeof(l_cpltRingVariantSz));

                l_sectionOffset = i_sectionTOR->TOC_PCI1_COMMON_RING;

                if(INSTANCE_RING == l_ringType)
                {
                    l_sectionOffset = i_sectionTOR->TOC_PCI1_INSTANCE_RING;
                }

                break;

            case PCI2_TYPE: // PCI - PCI2
                l_chipletData = PCI2::g_pci2Data;
                l_cpltRingVariantSz = i_applyOverride ? OVERRIDE_VARIANT_SIZE :
                                      (sizeof(PCI2::RingVariants) /
                                       sizeof(l_cpltRingVariantSz));

                l_sectionOffset = i_sectionTOR->TOC_PCI2_COMMON_RING;

                if(INSTANCE_RING == l_ringType)
                {
                    l_sectionOffset = i_sectionTOR->TOC_PCI2_INSTANCE_RING;
                }

                break;

            case EQ_TYPE: // EQ - Quad 0 - Quad 5
                l_chipletData = EQ::g_eqData;
                l_cpltRingVariantSz = i_applyOverride ? OVERRIDE_VARIANT_SIZE :
                                      ( sizeof(EQ::RingVariants) /
                                        sizeof(l_cpltRingVariantSz));


                l_sectionOffset = i_sectionTOR->TOC_EQ_COMMON_RING;
                l_CC_offset = 1;
                l_RL_offset = 2;

                if(INSTANCE_RING == l_ringType)
                {
                    l_sectionOffset = i_sectionTOR->TOC_EQ_INSTANCE_RING;
                }

                break;

            case EC_TYPE: // EC - Core 0 - 23
                l_chipletData = EC::g_ecData;
                l_cpltRingVariantSz = i_applyOverride ? OVERRIDE_VARIANT_SIZE :
                                      (sizeof(EC::RingVariants) /
                                       sizeof(l_cpltRingVariantSz));

                l_sectionOffset = i_sectionTOR->TOC_EC_COMMON_RING;
                l_CC_offset = 1;
                l_RL_offset = 2;

                if(INSTANCE_RING == l_ringType)
                {
                    l_sectionOffset = i_sectionTOR->TOC_EC_INSTANCE_RING;
                }

                break;

            default:
                SBE_TRACE("Invalid Target/ChipletID - %d", l_chipletID);
                l_rc = fapi2::FAPI2_RC_INVALID_PARAMETER;
                break;

        } // end of switch(l_chipletID)

        if (l_rc)
        {
            break;
        }

        FAPI_INF("l_sectionOffset %08x", l_sectionOffset);
        // Determine the section TOR address for the ring
        uint32_t* l_sectionAddr = reinterpret_cast<uint32_t*>(g_seepromAddr +
                                  i_sectionOffset + l_sectionOffset);

        SBE_TRACE ("l_sectionAddr %08X", (uint32_t)l_sectionAddr);

        uint16_t* l_ringTorAddr = NULL;


        if(INSTANCE_RING == l_ringType)
        {
            if ( l_chipletID >= l_chipletData.iv_base_chiplet_number)
            {
                uint8_t l_chipletOffset =
                    (l_chipletID - l_chipletData.iv_base_chiplet_number);

                if (l_chipLetType == EQ_TYPE)
                {
                    if (fapi2::TARGET_TYPE_EX & (i_target.get().getFapiTargetType()))
                    {
                        uint32_t l_ex_number = i_target.get().getTargetInstance();
                        l_chipletOffset = l_ex_number;
                        SBE_TRACE ("l_chipletID %d l_ex_number %d", l_chipletID, l_ex_number);
                    }
                }

                SBE_TRACE ("l_chipletID %u l_chipletOffset %u", l_chipletID, l_chipletOffset);
                l_ringTorAddr =  reinterpret_cast<uint16_t*>(l_sectionAddr ) + ((l_chipletOffset *
                                 (l_chipletData.iv_num_instance_rings * l_cpltRingVariantSz)) + (l_torOffset * l_cpltRingVariantSz));

            }
            else
            {
                l_ringTorAddr = reinterpret_cast<uint16_t*>(l_sectionAddr) +
                                (l_torOffset * l_cpltRingVariantSz);
            }
        }
        else
        {
            // TOR records of Ring TOR are 2 bytes in size.
            l_ringTorAddr = reinterpret_cast<uint16_t*>(l_sectionAddr) +
                            (l_torOffset * l_cpltRingVariantSz);
        }

        // The ring variants in section TOR are expected to be in the sequence -
        // 1. Base
        // 2. Cache-Contained
        // 3. Risk Level


        SBE_TRACE ("ring tor address %08X", (uint32_t)l_ringTorAddr);

        // If there are non-base variants of the ring, we'll have to check
        // attributes to determine the sequence of ring apply.
        if( l_cpltRingVariantSz > 1)
        {
            // Check if this is cache-contained IPL
            uint8_t l_iplPhase;
            FAPI_ATTR_GET(fapi2::ATTR_SYSTEM_IPL_PHASE,
                          fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> (),
                          l_iplPhase);

            // 4 : Use Cache Contained mode _if_ present, else fall back
            // to normal ring
            if((CACHE_CONTAINED_MODE == l_iplPhase)
               && *(l_ringTorAddr + l_CC_offset))
            {
                l_ringTorAddr += l_CC_offset;
            }
            else
            {
                // Check if this is risk-level IPL
                // _if_ present, else fall back to normal ring
                uint8_t l_riskLevel;
                FAPI_ATTR_GET(fapi2::ATTR_RISK_LEVEL,
                              fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> (),
                              l_riskLevel);

                if((RISK_LEVEL_MODE == l_riskLevel) &&
                   *(l_ringTorAddr + l_RL_offset))
                {
                    l_ringTorAddr += l_RL_offset;
                }
            }
        }

        SBE_TRACE("l_ringTorAddr %u", *l_ringTorAddr);

        if(*l_ringTorAddr != 0)
        {
            uint8_t* l_addr = reinterpret_cast<uint8_t*>(l_sectionAddr);
            uint8_t* l_rs4Address = reinterpret_cast<uint8_t*>
                                    (l_addr + *l_ringTorAddr);
            SBE_TRACE("l_rs4Address %08x", (uint32_t)l_rs4Address);
            l_rc = rs4DecompressionSvc(i_target, l_rs4Address,
                                       i_applyOverride, i_ringMode, l_ringType);

            if(l_rc != fapi2::FAPI2_RC_SUCCESS)
            {
                SBE_TRACE("Error from applyRS4_SS");
                break;
            }
        }
        else
        {
            SBE_TRACE("Ring image is not found for this is ringId %u", i_ringID);
        }
    }
    while(0);

    SBE_TRACE("<< findRS4InImageAndApply Exit for ringId %d", i_ringID);
    return l_rc;
}


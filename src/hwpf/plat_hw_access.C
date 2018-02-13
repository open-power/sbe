/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/hwpf/plat_hw_access.C $                                   */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2018                        */
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
#include "hw_access.H"
#include "plat_hw_access.H"
#include "p9_perv_scom_addresses.H"
#include <p9_putRingUtils.H>

namespace fapi2
{

struct restoreOpcgRegisters g_opcgData;

uint64_t decodeScanRegionData(const uint32_t i_ringAddress)
{
    uint32_t l_scan_region = (i_ringAddress & 0x0000FFF0) << 13;

    uint32_t l_scan_type = 0x00008000 >> (i_ringAddress & 0x0000000F);

    // This is special case if encoded type is 0xF

    if ( (i_ringAddress & 0x0000000F) == 0xF)
    {
        l_scan_type = 0x00008000 | (l_scan_type << 12);
    }
    uint64_t l_value = l_scan_region;
    l_value = (l_value << 32) |  l_scan_type;

    return l_value;
}

ReturnCode getRing_setup(const uint32_t i_ringAddress,
                             const RingMode i_ringMode)
{
    fapi2::ReturnCode l_rc = FAPI2_RC_SUCCESS;
    uint64_t l_scanRegion = 0;
    uint32_t l_chipletId = i_ringAddress >> 24;

    Target<fapi2::TARGET_TYPE_PROC_CHIP> l_proc = plat_getChipTarget();

    do
    {
        l_scanRegion = decodeScanRegionData(i_ringAddress);

        if (i_ringMode &  fapi2::RING_MODE_SET_PULSE_SL)
        {
            l_rc =  storeOPCGRegData (l_proc, g_opcgData, l_chipletId);
            if(l_rc != fapi2::FAPI2_RC_SUCCESS)
            {
                break;
            }

            l_rc = setupScanRegionForSetPulse(l_proc, l_scanRegion,
                                        i_ringMode,l_chipletId);
            if(l_rc != fapi2::FAPI2_RC_SUCCESS)
            {
                break;
            }
        }
        else
        {
            // Set up the scan region for the ring.
            l_rc = setupScanRegion(l_proc, l_scanRegion, l_chipletId);
            if(l_rc != fapi2::FAPI2_RC_SUCCESS)
            {
                break;
            }
        }
        // Write a 64 bit value for header.
        const uint64_t l_header = 0xa5a5a5a5a5a5a5a5;
        uint32_t l_scomAddress = 0x0003E000 |  (i_ringAddress & 0xFF000000);
        l_rc = fapi2::putScom(l_proc, l_scomAddress, l_header);
        if(l_rc != fapi2::FAPI2_RC_SUCCESS)
        {
            break;
        }

    }while(0);

    return l_rc;
}

ReturnCode getRing_verifyAndcleanup(const uint32_t i_ringAddress,
                                    const RingMode i_ringMode)
{
    fapi2::ReturnCode l_rc = FAPI2_RC_SUCCESS;
    uint32_t l_chipletId = i_ringAddress >> 24;

    fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> l_proc;

    do
    {
        // Verify header
        l_rc = verifyHeader(l_proc,l_chipletId,i_ringMode);
        if(l_rc != fapi2::FAPI2_RC_SUCCESS)
        {
            break;
        }

        l_rc = cleanScanRegionandTypeData(l_proc, l_chipletId);
        if(l_rc != fapi2::FAPI2_RC_SUCCESS)
        {
            break;
        }

        if (i_ringMode &  fapi2::RING_MODE_SET_PULSE_SL)
        {
            l_rc =  restoreOPCGRegData(l_proc, g_opcgData, l_chipletId);
            if(l_rc != fapi2::FAPI2_RC_SUCCESS)
            {
                break;
            }
        }

    }while(0);

    return l_rc;
}

static uint32_t getEffectiveAddress(const plat_target_handle_t &i_target, const uint32_t i_addr)
{
    ScomAddr l_addr(i_addr);
    switch(i_target.getTargetType())
    {
        case PPE_TARGET_TYPE_EX:
            if((EQ_CHIPLET_OFFSET <= l_addr.iv_chiplet) &&
               ((EQ_CHIPLET_OFFSET + EQ_TARGET_COUNT) > l_addr.iv_chiplet))
            {
                l_addr.iv_chiplet = i_target.fields.chiplet_num;
                l_addr.iv_ring = (l_addr.iv_ring - (l_addr.iv_ring % 2)) +
                                  (i_target.getTargetInstance() % 2);
            }
            else if ((CORE_CHIPLET_OFFSET <= l_addr.iv_chiplet) &&
                     ((CORE_CHIPLET_OFFSET + CORE_TARGET_COUNT) > l_addr.iv_chiplet))
            {
                l_addr.iv_chiplet = CORE_CHIPLET_OFFSET + (l_addr.iv_chiplet % 2) +
                                    (i_target.getTargetInstance() * 2);
            }
            else
            {
               assert(false);
            }
            break;
        case PPE_TARGET_TYPE_MCS:
        case PPE_TARGET_TYPE_MI:
            l_addr.iv_chiplet = i_target.fields.chiplet_num;
            l_addr.iv_satId = (2 * (i_target.getTargetInstance() % 2));
            break;
        case PPE_TARGET_TYPE_PHB:
            if(l_addr.iv_chiplet == N2_CHIPLET)
            {
                if (i_target.getTargetInstance() == 0)
                {
                    l_addr.iv_ring = 0x3;
                    l_addr.iv_satId = ((l_addr.iv_satId < 4) ? (1) : (4));
                }
                else
                {
                    l_addr.iv_ring = (0x3 + (i_target.getTargetInstance() / 3) + 1) & 0xF;
                    l_addr.iv_satId = ((l_addr.iv_satId < 4) ? (1) : (4)) +
                                       ((i_target.getTargetInstance() % 2) ? (0) : (1)) +
                                       (2 * (i_target.getTargetInstance() / 5));
                }
            }
            else
            {
                l_addr.iv_chiplet = i_target.fields.chiplet_num;
                if (i_target.getTargetInstance() == 0)
                {
                    l_addr.iv_satId = ((l_addr.iv_satId < 4) ? (1) : (4));
                }
                else
                {
                    l_addr.iv_satId = (((l_addr.iv_satId < 4) ? (1) : (4)) +
                                       ((i_target.getTargetInstance() % 2) ? (0) : (1)) +
                                       (2 * (i_target.getTargetInstance() / 5)));
                }
            }
            break;
        default:
            if(0 != i_target.getAddressOverlay())
            {
                l_addr.iv_chiplet = i_target.fields.chiplet_num;
            }
            break;
    }
    return l_addr;
}

static fapi2::ReturnCode pibRcToFapiRc(const uint32_t i_pibRc)
{
    fapi2::ReturnCode l_fapiRc = FAPI2_RC_SUCCESS;
    switch(i_pibRc)
    {
        case PIB_XSCOM_ERROR:
            l_fapiRc = RC_SBE_PIB_XSCOM_ERROR;
            break;
        case PIB_OFFLINE_ERROR:
            l_fapiRc = RC_SBE_PIB_OFFLINE_ERROR;
            break;
        case PIB_PARTIAL_ERROR:
            l_fapiRc = RC_SBE_PIB_PARTIAL_ERROR;
            break;
        case PIB_ADDRESS_ERROR:
            l_fapiRc = RC_SBE_PIB_ADDRESS_ERROR;
            break;
        case PIB_CLOCK_ERROR:
            l_fapiRc = RC_SBE_PIB_CLOCK_ERROR;
            break;
        case PIB_PARITY_ERROR:
            l_fapiRc = RC_SBE_PIB_PARITY_ERROR;
            break;
        case PIB_TIMEOUT_ERROR:
            l_fapiRc = RC_SBE_PIB_TIMEOUT_ERROR;
            break;
        case PIB_NO_ERROR:
        default:
            break;
    }
    return l_fapiRc;
}

fapi2::ReturnCode getscom_abs_wrap(const void *i_target,
                                   const uint32_t i_addr, uint64_t *o_data)
{
    uint32_t l_pibRc = 0;
    uint32_t l_addr = getEffectiveAddress(*(plat_target_handle_t*)i_target, i_addr);
    FAPI_INF("getScom: address: 0x%08X", l_addr);
    l_pibRc = getscom_abs(l_addr, o_data);
    if( PIB_NO_ERROR != l_pibRc )
    {
        l_pibRc = p9_handlePibErr( l_addr, o_data, l_pibRc, true);
    }
    FAPI_INF("getScom: returned pibRc: 0x%08X, data HI: 0x%08X, "
             "data LO: 0x%08X", l_pibRc, (*o_data >> 32),
             static_cast<uint32_t>(*o_data & 0xFFFFFFFF));
    fapi2::current_err = fapi2::FAPI2_RC_SUCCESS;
    // Setting 64bit address in ffdc package as the parsers are
    // hard coded to read 64bit address
    const uint64_t ffdcAddr = l_addr;
    PLAT_FAPI_ASSERT( PIB_NO_ERROR == l_pibRc,
                      SBE_SCOM_FAILURE().
                      set_address(ffdcAddr).
                      set_pcb_pib_rc(l_pibRc),
                      "getScom:pcb pib error, pibRc[0x%08X] Scom_Addr[0x%08X]",
                      l_pibRc,l_addr);
fapi_try_exit:
    if(PIB_NO_ERROR != l_pibRc)
    {
        // Override FAPI RC based on PIB RC
        fapi2::current_err = pibRcToFapiRc(l_pibRc);
        fapi2::g_FfdcData.fapiRc = fapi2::current_err;
    }
    return fapi2::current_err;
}

fapi2::ReturnCode putscom_abs_wrap(const void *i_target,
                                    const uint32_t i_addr, uint64_t i_data)
{
    uint32_t l_pibRc = 0;
    uint32_t l_addr = getEffectiveAddress(*(plat_target_handle_t*)i_target,
                                                                    i_addr);
    FAPI_INF("putScom: address: 0x%08X, data HI: 0x%08X, data LO: 0x%08X",
             l_addr, (i_data >> 32),
             static_cast<uint32_t>(i_data & 0xFFFFFFFF));
    l_pibRc = putscom_abs(l_addr, i_data);
    if( PIB_NO_ERROR != l_pibRc )
    {
        l_pibRc = p9_handlePibErr( l_addr, &i_data, l_pibRc, false);
    }
    FAPI_INF("putScom: returned pibRc: 0x%08X", l_pibRc);
    fapi2::current_err = fapi2::FAPI2_RC_SUCCESS;
    // Setting 64bit address in ffdc package as the parsers are
    // hard coded to read 64bit address
    const uint64_t ffdcAddr = l_addr;
    PLAT_FAPI_ASSERT( PIB_NO_ERROR == l_pibRc,
                      SBE_SCOM_FAILURE().
                      set_address(ffdcAddr).
                      set_pcb_pib_rc(l_pibRc),
                      "putScom:pcb pib error, pibRc[0x%08X] Scom_Addr[0x%08X] "
                      "Scom_Data[0x%08X%08X]",
                      l_pibRc,l_addr,(i_data >> 32),
                      static_cast<uint32_t>(i_data & 0xFFFFFFFF));
fapi_try_exit:
    if(PIB_NO_ERROR != l_pibRc)
    {
        // Override FAPI RC based on PIB RC
        fapi2::current_err = pibRcToFapiRc(l_pibRc);
        fapi2::g_FfdcData.fapiRc = fapi2::current_err;
    }
    return fapi2::current_err;
}


fapi2::ReturnCode putscom_under_mask(const void *i_target,
                                     uint32_t i_addr,
                                     uint64_t i_data,
                                     uint64_t i_mask)
{
    uint64_t l_read = 0;
    fapi2::ReturnCode l_rc = FAPI2_RC_SUCCESS;

    do
    {
        l_rc = getscom_abs_wrap(i_target, i_addr, &l_read);

        if(l_rc)
        {
            break;
        }

        l_read = (l_read & ~i_mask) | (i_data & i_mask);

        l_rc = putscom_abs_wrap(i_target, i_addr, l_read);
    } while (false);

    return l_rc;
}

uint32_t p9_handlePibErr( const uint32_t i_addr, uint64_t *io_data,
                         const uint8_t i_pibErr, const bool i_isRead)
{
    FAPI_INF("Entering p9_handlePibErr");
    static const uint8_t MAX_RETRIES = 2;
    uint8_t l_retryCount = 0;
    uint32_t pibErr = i_pibErr;
    uint32_t retryOnOffline = false;
    // the format for scom is
    // 0   1  2  3   4   5  6  7  8  9  10  11  12  13  14  15
    // WR  MC [MC Type]  [MC gp]  [PIB M Addr]  [Port Number ]
    //        [slave addr(MC=0)]
    // 16 17 18 19 20 21 22 23 24 25 26  27  28  29  30   31
    // [          Local addr                               ]

    // For multicast address address bit 1 should be 1
    static const uint32_t MULTICAST_MASK = 0x40000000;
    PkMachineContext  ctx = {};
    do
    {
        // There is a HW bug where if any chiplet is offline, we can get
        // PIB_OFFLINE_ERROR on multicast address  though data is valid.
        // In this case SBE should treat it as success.
        if((  i_pibErr == PIB_OFFLINE_ERROR ) &&
           (i_addr & MULTICAST_MASK))
        {
            FAPI_INF("PIB_OFFLINE_ERROR on Multicast scom addr:0x%08X. "
                    "Ignoring error", i_addr );
            pibErr = PIB_NO_ERROR;
            retryOnOffline = true;
            // The critical section enter/exit set is done to ensure
            // PIB OFFILE mask is not propogated to any other context.
            pk_critical_section_enter(&ctx);
            // bit 2 of MSR. PLAT_SET_PIB_ERROR_MASK shifts mask by 24bits
            static const uint32_t i_mask = 0x20;
            PLAT_SET_PIB_ERROR_MASK(i_mask);
        }
        // Only retry for parity and timeout errors
        if (( i_pibErr != PIB_PARITY_ERROR )
            && ( i_pibErr !=  PIB_TIMEOUT_ERROR )
            && ( ! retryOnOffline ) )
        {
            break;
        }
        for(l_retryCount = 0; l_retryCount < MAX_RETRIES; l_retryCount++)
        {
            FAPI_DBG( "*** Retry %i ***", l_retryCount );

            if ( i_isRead )
            {
                pibErr = getscom_abs(i_addr, io_data);
            }
            else
            {
                pibErr = putscom_abs(i_addr, *io_data);
            }

            if(( PIB_NO_ERROR == pibErr ) ||
               ( ( PIB_OFFLINE_ERROR == pibErr )  && retryOnOffline ))
            {
                pibErr = PIB_NO_ERROR;
                FAPI_INF("Read/Write Retry Successful");
                break;
            }
            if ( pibErr != i_pibErr ) break;
        }
    }while(0);
    if (retryOnOffline )
    {
        // bit 2 of MSR. PLAT_SET_PIB_ERROR_MASK shifts mask by 24bits
        static const uint32_t i_mask = 0x00;
        PLAT_SET_PIB_ERROR_MASK(i_mask);
        pk_critical_section_exit(&ctx);
    }
    FAPI_INF("Exiting p9_handlePibErr");
    return pibErr;
}

};

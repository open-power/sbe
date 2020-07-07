/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/core/ipl.C $                                        */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2017,2020                        */
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
#include "fapi2.H"
#include "sberegaccess.H"
#include "sbe_sp_intf.H"
#include "sbeFFDC.H"

#include "ipl.H"
#include "sbeConsole.H"
#include "sbeglobals.H"

//#include "p9n2_perv_scom_addresses.H"
#include "p10_scom_perv_b.H"
using namespace fapi2;

sbeRole g_sbeRole = SBE_ROLE_MASTER;

uint64_t G_ring_save[8] = {0, 0, 0, 0, 0, 0, 0, 0};

// Globals
////////////////////// Declaration in sbeirq.C ////////////////////////
extern "C" void __sbe_register_saveoff();
extern uint32_t __g_isParityError;
//////////////////////////////////////////////////////////////////////
// TODO: via RTC 123602 This global needs to move to a class that will store the
// SBE FFDC.
fapi2::ReturnCode g_iplFailRc = FAPI2_RC_SUCCESS;

bool isSystemCheckstop()
{
    bool checkstop = false;
    fapi2::buffer<uint64_t> attnReg = 0;
    ReturnCode fapiRc = FAPI2_RC_SUCCESS;
    plat_target_handle_t hndl;
    fapiRc = getscom_abs_wrap(&hndl,
                              scomt::perv::ATTN_INTERRUPT_REG,
                              attnReg.pointer());
    if( fapiRc == FAPI2_RC_SUCCESS )
    {
        checkstop = attnReg.getBit<2>();
    }
    return checkstop;
}

//--------------------------------------------------------------------------
// Check for Parity Error in the respective Boot SPI
//--------------------------------------------------------------------------
#define SPI_PARITY_CHECK_MASK          0xFFE00000
#define PRIMARY_BOOT_SPI_STATUS_REG    0xC0008
#define SECONDARY_BOOT_SPI_STATUS_REG  0xC0028
bool isSpiParityError()
{
    sbe_local_LFR lfrReg;
    uint32_t spiBaseAddr = 0;
    uint64_t data = 0;
    // Directly Load the LFR here
    PPE_LVD(0xc0002040, lfrReg);
    if(lfrReg.sec_boot_seeprom)
    {
        spiBaseAddr = SECONDARY_BOOT_SPI_STATUS_REG;
    }
    else
    {
        spiBaseAddr = PRIMARY_BOOT_SPI_STATUS_REG;
    }
    // Load the SPI Status Register Here
    PPE_LVD(spiBaseAddr, data);
    if(data & SPI_PARITY_CHECK_MASK) // Check bit32 to 42 for set
    {
        return true;
    }
    return false;
}
//----------------------------------------------------------------------------
// @note This is the responsibilty of caller to verify major/minor
//       number before calling this function

ReturnCode sbeExecuteIstep (const uint8_t i_major, const uint8_t i_minor)
{
    #define SBE_FUNC "sbeExecuteIstep "
    SBE_INFO(SBE_FUNC"Major number:0x%x minor number:0x%x",
                       i_major, i_minor );

    ReturnCode rc = FAPI2_RC_SUCCESS;

    for(size_t entry = 0; entry < istepTable.len; entry++)
    {
        auto istepTableEntry = &istepTable.istepMajorArr[entry];
        if(( i_major == istepTableEntry->istepMajorNum ) &&
           ( i_minor <= istepTableEntry->len ))
        {
            auto istepMap = &istepTableEntry->istepMinorArr[i_minor-1];
            if(istepMap->istepWrapper != NULL)
            {
                rc = istepMap->istepWrapper(istepMap->istepHwp);
            }
            break;
        }
    }
    if (isSpiParityError()) // If true call saveoff and halt
    {
        __g_isParityError = 1;
        __sbe_register_saveoff();
    }

    if(rc != FAPI2_RC_SUCCESS)
    {
        SBE_ERROR( SBE_FUNC" FAPI RC:0x%08X", rc);
        (void)SbeRegAccess::theSbeRegAccess().stateTransition(
                                                    SBE_DUMP_FAILURE_EVENT);
    }
    else
    {
        (void)SbeRegAccess::theSbeRegAccess().updateSbeStep(i_major, i_minor);
    }

    return rc;
    #undef SBE_FUNC
}

//----------------------------------------------------------------------------
void sbeDoContinuousIpl()
{
    #define SBE_FUNC "sbeDoContinuousIpl "
    SBE_ENTER(SBE_FUNC);
    ReturnCode rc = FAPI2_RC_SUCCESS;
    do
    {
        // Set SBE state as IPLing
        (void)SbeRegAccess::theSbeRegAccess().stateTransition(
                                                SBE_PLCK_EVENT);
        // Run isteps
        for(size_t entry = 0; entry < istepTable.len; entry++)
        {
            auto istepTableEntry = &istepTable.istepMajorArr[entry];
            for(size_t step = 1; step <= istepTableEntry->len; step++)
            {
                auto istepMap = &istepTableEntry->istepMinorArr[step-1];
                if(istepMap->istepWrapper != NULL)
                {
                    SBE_MSG_CONSOLE("istep ", istepTableEntry->istepMajorNum, ".", step);
                    rc = istepMap->istepWrapper(istepMap->istepHwp);
                }
                // TODO - F001A is not available till istep 2.3, which is driven by the
                // nest clock, so we can enable this only after 2.3, For time being
                // commenting this out.

                //bool checkstop = isSystemCheckstop();
                //if((rc != FAPI2_RC_SUCCESS) || checkstop )
    
                if (isSpiParityError()) // If true call saveoff and halt
                {
                    __g_isParityError = 1;
                    __sbe_register_saveoff();
                }
                if(rc != FAPI2_RC_SUCCESS)
                {
                    SBE_ERROR(SBE_FUNC"Failed istep execution in plck mode: "
                            "Major: %d, Minor: %d",
                            istepTableEntry->istepMajorNum, step);

                    uint32_t secRc = SBE_PRI_GENERIC_EXECUTION_FAILURE;
                    //uint32_t secRc = checkstop ? SBE_SEC_SYSTEM_CHECKSTOP:
                    //                    SBE_SEC_GENERIC_FAILURE_IN_EXECUTION;

                    captureAsyncFFDC(SBE_PRI_GENERIC_EXECUTION_FAILURE,
                                     secRc);
                    // exit outer loop as well
                    entry = istepTable.len;
                    break;
                }
                (void)SbeRegAccess::theSbeRegAccess().updateSbeStep(
                                    istepTableEntry->istepMajorNum,
                                    step);
                // Check if we reached runtime
                if(SBE_STATE_RUNTIME ==
                                SbeRegAccess::theSbeRegAccess().getSbeState())
                {
                    // Exit outer loop as well
                    entry = istepTable.len;
                    break;
                }
            }
        }
    } while(false);
    // Store l_rc in a global variable that will be a part of the SBE FFDC
    g_iplFailRc = rc;
    SBE_EXIT(SBE_FUNC);
    #undef SBE_FUNC
}

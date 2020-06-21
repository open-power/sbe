/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/pm/p10_suspend_powman.C $ */
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
/// @file p10_suspend_powman.C
/// @brief Suspend the OCC/PM
///
// *HWP HWP Owner: Greg Still (stillgs@us.ibm.com)
// *HWP FW  Owner: Prasad Bg Ranganath(prasadbgr@in.ibm.com)
// *HWP Team: PM
// *HWP Level: 2
// *HWP Consumed by: SBE
//
//--------------------------------------------------------------------------


//PGPE PM Suspend Flow
//  enters safe mode and notifies qme
//  qme ignores future stop requests
//  qme acks -> xgpe acks -> xgpe sets suspended bit in OCCS2

// *INDENT-OFF*
//--------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------
#include <p10_suspend_powman.H>
#include <p10_pm_hcd_flags.h>
#include <p10_hcd_common.H>
#include <p10_ppe_defs.H>
#ifndef __PPE__
  #include <p10_ppe_utils.H>
#endif
#include <p10_scom_proc.H>
#include <p10_scom_eq.H>
#include <multicast_group_defs.H>
#include <vector>
#include <p10_pm.H>
#include <p10_pm_pgpe_init.H>
#include <p10_pm_xgpe_init.H>
#include <p10_pm_qme_init.H>
#include <p10_pm_occ_gpe_init.H>



// 1000000 nanosecond = 1 millisecond
// total timeout = 10 milliseconds
static const uint64_t POLLTIME_NS = 1000000;
static const uint64_t POLLTIME_MCYCLES = 4;
static const uint32_t TRIES_BEFORE_TIMEOUT = 500;
// Following constants hold an approximate value.
static const uint32_t PPE_TIMEOUT_MS       = 50000;
static const uint32_t PPE_POLLTIME_MS      = 20;
static const uint32_t PPE_POLLTIME_MCYCLES = 2;



enum
{
    QME_ACTIVE              =   p10hcd::QME_FLAGS_STOP_READY,
    QME_POLLTIME_MS         =   1,
    QME_POLLTIME_MCYCLES    =   1,
};



extern "C" 
{
fapi2::ReturnCode suspend_pm_halt(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target);

    using namespace scomt;
    using namespace proc;
    using namespace p10hcd;
    //--------------------------------------------------------------------------
    // Constant definitions
    //--------------------------------------------------------------------------



    //--------------------------------------------------------------------------
    //  HWP entry point
    //--------------------------------------------------------------------------
    fapi2::ReturnCode p10_suspend_powman(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        FAPI_DBG("Entering p10_suspend_powman...\n")
        fapi2::buffer<uint64_t> l_occs2_data(0);
        fapi2::buffer<uint64_t> l_occs3_data(0);
        fapi2::buffer<uint64_t> l_xsr(0);
        static const uint64_t  PPE_XIXCR_XCR_HALT      = 0x1000000000000000;

        bool l_pgpe_in_safe_mode = false;
        bool l_xgpe_suspended = false;
        uint8_t l_occ_mode = false;

        do
        {
            const fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYSTEM;
            FAPI_TRY( FAPI_ATTR_GET(fapi2::ATTR_SYSTEM_SUSPEND_OCC_MODE,
                      FAPI_SYSTEM,l_occ_mode));
            FAPI_TRY(GET_TP_TPCHIP_OCC_OCI_GPE2_OCB_GPEXIXSR(i_target,l_xsr));

            if(!(l_xsr >> 63))
            {
                // SBE waits for PGPE to set OCC Scratch2[PGPE_SAFE_MODE_ACTIVE]
                for(uint32_t method = 0; method < 2; method++)
                {
                    //If this is set, then OCC is not booting,hence skipping
                    //the request from occ to pgpe
                    if (l_occ_mode)
                    {
                        method = 1;
                    }
                    if(method == 0)
                    {
                        // SBE requests OCC enter safe state by setting OCC_Flag[REQUEST_OCC_SAFE_STATE]
                        // OCC polls this bit every 500us,
                        // if detected heartbeat stop PGPE is interrupted and enters suspend
                        PREP_TP_TPCHIP_OCC_OCI_OCB_OCCFLG0_WO_OR(i_target);
                        FAPI_TRY(PUT_TP_TPCHIP_OCC_OCI_OCB_OCCFLG0_WO_OR(i_target,BIT64(p10hcd::REQUEST_OCC_SAFE_STATE)));
                    }
                    else
                    {
                        FAPI_DBG("Safe Mode bit failed after requesting occ safe state\n Requesting in PGPE...");
                        //PGPE polls this bit on a reduced FIT timer period
                        //if detected enters safe mode
                        PREP_TP_TPCHIP_OCC_OCI_OCB_OCCFLG2_WO_OR(i_target);
                        FAPI_TRY(PUT_TP_TPCHIP_OCC_OCI_OCB_OCCFLG2_WO_OR(i_target,BIT64(PGPE_SAFE_MODE)));
                    }

                    for (uint32_t i = 0; i < TRIES_BEFORE_TIMEOUT; i++)
                    {
                        FAPI_TRY(GET_TP_TPCHIP_OCC_OCI_OCB_OCCFLG2_RW(i_target,l_occs2_data));

                        if(l_occs2_data.getBit<PGPE_SAFE_MODE_ACTIVE>())
                        {
                            l_pgpe_in_safe_mode = true;
                            break;
                        }

                        fapi2::delay(POLLTIME_NS , POLLTIME_MCYCLES * 1000 * 1000);
                    }

                    if(l_pgpe_in_safe_mode)
                    {
                        break;
                    }
                }
                //RTC 214388 (error handling)
                if(!l_pgpe_in_safe_mode)
                {
                    FAPI_ERR("PGPE fails to put the system in safe mode");
                    break;
                }

                //SBE issues "halt OCC complex" to stop OCC instructions
                PREP_TP_TPCHIP_OCC_OCI_OCB_PIB_OCR_WO_OR(i_target);
                FAPI_TRY(PUT_TP_TPCHIP_OCC_OCI_OCB_PIB_OCR_WO_OR(i_target,BIT64(TP_TPCHIP_OCC_OCI_OCB_PIB_OCR_OCR_DBG_HALT)));

                //XGPE polls this bit on a reduced FIT timer period
                //if detected executes XGPE pm_suspend flow
                PREP_TP_TPCHIP_OCC_OCI_OCB_OCCFLG3_WO_OR(i_target);
                FAPI_TRY(PUT_TP_TPCHIP_OCC_OCI_OCB_OCCFLG3_WO_OR(i_target,BIT64(PM_COMPLEX_SUSPEND)));

                //Poll PM_COMPLEX_SUSPENDED (bit 19) in OCCFLG3 register, that tells
                //QME has responded to XGPE and suspend all the cores from stop
                //entry/exit
                for (uint32_t i = 0; i < TRIES_BEFORE_TIMEOUT; i++)
                {
                    FAPI_TRY(GET_TP_TPCHIP_OCC_OCI_OCB_OCCFLG3_RW(i_target,l_occs3_data));

#ifdef __PPE__
                    //Adding this code because, in simics FIT interrupt is
                    //taking longer duration to interrupt the XGPE and when I
                    //tried to change the FIT frequency to different values, it
                    //was very fast and that was causing other interrupts task
                    //to hold.So for now adding this code only for simics
                    //environment
                    if( SBE::isSimicsRunning() )
                    {
                        FAPI_TRY(PUT_TP_TPCHIP_OCC_OCI_OCB_OCCFLG3_WO_OR(i_target,BIT64(PM_COMPLEX_SUSPENDED)));
                        FAPI_TRY(PUT_TP_TPCHIP_OCC_OCI_OCB_OCCFLG3_WO_CLEAR(i_target,BIT64(PM_COMPLEX_SUSPEND)));
                    }
#endif

                    if(l_occs3_data.getBit<PM_COMPLEX_SUSPENDED>())
                    {
                        l_xgpe_suspended = true;
                        break;
                    }

                    fapi2::delay(POLLTIME_NS, POLLTIME_MCYCLES * 1000 * 1000);
                }
                //TBD for L3
                //RTC 214388 (error handling)
#if 0
                //if timeout, hwp fails
                if(!l_xgpe_suspended)
                {
                    FAPI_ERR("XGPE did not signal that PM Complex Suspend Finished");
                    FAPI_TRY ( p10_collect_suspend_ffdc (
                                i_target));
                }
#endif
                if (!l_xgpe_suspended)
                {
                    FAPI_ERR("Suspend Power Management failed");
                    break;
                }
                else
                {
                    FAPI_DBG("Suspend Power Management Successful");
                }
            }
            else
            {
                FAPI_TRY(GET_TP_TPCHIP_OCC_OCI_GPE3_OCB_GPEXIXSR(i_target,l_xsr));

                //Make sure that the XGPE is not halted before requesting halt
                if(!(l_xsr >> 63))
                {
                    FAPI_INF("WARNING! PGPE Already Halted, halting XGPE via XIXSR");
                    FAPI_TRY(PUT_TP_TPCHIP_OCC_OCI_GPE3_OCB_GPEXIXSR(i_target,PPE_XIXCR_XCR_HALT));
                }
                else
                {
                    FAPI_INF("WARNING! XGPE and PGPE Already Halted, skipping procedure");
                }
            }

            //Halt all gpe's and qme
            FAPI_TRY(suspend_pm_halt(i_target));
        }while (0);


fapi_try_exit:
        FAPI_DBG("Exiting p10_suspend_powman...");
        return fapi2::current_err;

    }



    fapi2::ReturnCode suspend_pm_halt(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        fapi2::ReturnCode l_rc;

        //  ************************************************************************
        //  Issue halt to OCC GPEs ( GPE0 and GPE1) (Bring them to HALT)
        //  ************************************************************************
        FAPI_DBG("Executing p10_pm_occ_gpe_init to halt OCC GPE");
        FAPI_EXEC_HWP(l_rc, p10_pm_occ_gpe_init,
                i_target,
                pm::PM_HALT,
                occgpe::GPEALL // Apply to both OCC GPEs
                );
        FAPI_TRY(l_rc, "ERROR: Failed to halt the OCC GPEs");

        //  ************************************************************************
        //  Reset the PSTATE GPE (Bring it to HALT)
        //  ************************************************************************
        FAPI_DBG("Executing p10_pm_pstate_gpe_init to halt PGPE");
        FAPI_EXEC_HWP(l_rc, p10_pm_pgpe_init, i_target, pm::PM_HALT);
        FAPI_TRY(l_rc, "ERROR: Failed to halt the PGPE");

        //  ************************************************************************
        //  Reset the XGPE (Bring it to HALT)
        //  ************************************************************************
        FAPI_DBG("Executing p10_pm_stop_gpe_init to halt XGPE");
        FAPI_EXEC_HWP(l_rc, p10_pm_xgpe_init, i_target, pm::PM_HALT);
        FAPI_TRY(l_rc, "ERROR: Failed to halt XGPE");

        //  ************************************************************************
        //  Reset the QME (Bring it to HALT)
        //  ************************************************************************
        FAPI_DBG("Executing p10_pm_qme_init to halt QME");
        FAPI_EXEC_HWP(l_rc, p10_pm_qme_init, i_target, pm::PM_HALT);
        FAPI_TRY(l_rc, "ERROR: Failed to halt QME");



fapi_try_exit:
        FAPI_IMP("<< p10_pm_halt...");
        return fapi2::current_err;

    }


} // extern "C"

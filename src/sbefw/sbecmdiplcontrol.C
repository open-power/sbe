/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/sbecmdiplcontrol.C $                                */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2016                        */
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
/*
 * @file: ppe/sbe/sbefw/sbecmdiplcontrol.C
 *
 * @brief This file contains the SBE istep chipOps
 *
 */

#include "sbecmdiplcontrol.H"
#include "sbefifo.H"
#include "sbetrace.H"
#include "sbe_sp_intf.H"
#include "sbeFifoMsgUtils.H"
#include "assert.h"
#include "sberegaccess.H"
#include "sbestates.H"

#include "fapi2.H"
// Pervasive HWP Header Files ( istep 2)
#include <p9_sbe_attr_setup.H>
#include <p9_sbe_tp_chiplet_init1.H>
#include <p9_sbe_tp_gptr_time_initf.H>
#include <p9_sbe_npll_initf.H>
#include <p9_sbe_npll_setup.H>
#include <p9_sbe_tp_switch_gears.H>
#include <p9_sbe_clock_test2.H>
#include <p9_sbe_tp_chiplet_reset.H>
#include <p9_sbe_tp_repr_initf.H>
#include <p9_sbe_tp_chiplet_init2.H>
#include <p9_sbe_tp_arrayinit.H>
#include <p9_sbe_tp_initf.H>
#include <p9_sbe_tp_chiplet_init3.H>

// Pervasive HWP Header Files ( istep 3)
#include <p9_sbe_chiplet_reset.H>
#include <p9_sbe_gptr_time_initf.H>
#include <p9_sbe_chiplet_init.H>
#include <p9_sbe_chiplet_pll_initf.H>
#include <p9_sbe_chiplet_pll_setup.H>
#include <p9_sbe_repr_initf.H>
#include <p9_sbe_arrayinit.H>
#include <p9_sbe_tp_enable_ridi.H>
#include <p9_sbe_setup_boot_freq.H>
#include <p9_sbe_nest_initf.H>
#include <p9_sbe_nest_startclocks.H>
#include <p9_sbe_io_initf.H>
#include <p9_sbe_nest_enable_ridi.H>
#include <p9_sbe_startclock_chiplets.H>
#include <p9_sbe_scominit.H>
#include <p9_sbe_lpc_init.H>
#include <p9_sbe_fabricinit.H>
#include <p9_sbe_mcs_setup.H>
#include <p9_sbe_select_ex.H>
// Cache HWP header file
#include <p9_hcd_cache.H>
#include <p9_hcd_cache_dcc_skewadjust_setup.H>
#include <p9_hcd_cache_chiplet_l3_dcc_setup.H>
#include <p9_hcd_cache_dpll_initf.H>
// Core HWP header file
#include <p9_hcd_core.H>

// istep 5 hwp header files
#include "p9_sbe_instruct_start.H"
#include "p9_sbe_load_bootloader.H"
// Nest frequency array
#include "p9_frequency_buckets.H"

#include "sbeXipUtils.H" // For getting hbbl offset
#include "sbeutil.H" // For getting SBE_TO_NEST_FREQ_FACTOR
// Forward declaration
using namespace fapi2;
ReturnCode sbeExecuteIstep (uint8_t i_major, uint8_t i_minor);
bool validateIstep (uint8_t i_major, uint8_t i_minor);

//typedefs
typedef ReturnCode (*sbeIstepHwpProc_t)
                    (const Target<TARGET_TYPE_PROC_CHIP> & i_target);

typedef ReturnCode (*sbeIstepHwpEq_t)
                    (const Target<TARGET_TYPE_EQ> & i_target);

typedef ReturnCode (*sbeIstepHwpCore_t)
                    (const Target<TARGET_TYPE_CORE> & i_target);

typedef union
{
    sbeIstepHwpProc_t procHwp;
    sbeIstepHwpEq_t eqHwp;
    sbeIstepHwpCore_t coreHwp;
}sbeIstepHwp_t;
// Wrapper function for HWP IPl functions
typedef ReturnCode (*sbeIstep_t)( sbeIstepHwp_t );

// Wrapper function which will call HWP.
ReturnCode istepWithProc( sbeIstepHwp_t i_hwp );
ReturnCode istepAttrSetup( sbeIstepHwp_t i_hwp );
ReturnCode istepNoOp( sbeIstepHwp_t i_hwp );
ReturnCode istepWithEq( sbeIstepHwp_t i_hwp);
ReturnCode istepWithCore( sbeIstepHwp_t i_hwp);
ReturnCode istepSelectEx( sbeIstepHwp_t i_hwp);
ReturnCode istepLoadBootLoader( sbeIstepHwp_t i_hwp);
ReturnCode istepCheckSbeMaster( sbeIstepHwp_t i_hwp);
ReturnCode istepStartInstruction( sbeIstepHwp_t i_hwp);
ReturnCode istepWithCoreConditional( sbeIstepHwp_t i_hwp);
ReturnCode istepWithEqConditional( sbeIstepHwp_t i_hwp);
ReturnCode istepNestFreq( sbeIstepHwp_t i_hwp);

#ifdef SEEPROM_IMAGE
// Using function pointer to force long call.
p9_sbe_select_ex_FP_t p9_sbe_select_ex_hwp = &p9_sbe_select_ex;
#endif

//structure for mapping SBE wrapper and HWP functions
typedef struct
{
    sbeIstep_t istepWrapper;
    sbeIstepHwp_t istepHwp;
}istepMap_t;

// Major isteps which are supported
typedef enum
{
    SBE_ISTEP2 = 2,
    SBE_ISTEP_FIRST = SBE_ISTEP2,
    SBE_ISTEP3 = 3,
    SBE_ISTEP_LAST_SLAVE = SBE_ISTEP3,
    SBE_ISTEP4 = 4,
    SBE_ISTEP5 = 5,
    SBE_ISTEP_LAST_MASTER = SBE_ISTEP5,
}sbe_supported_steps_t;

// constants
const uint32_t ISTEP2_MAX_SUBSTEPS = 17;
const uint32_t ISTEP3_MAX_SUBSTEPS = 22;
const uint32_t ISTEP4_MAX_SUBSTEPS = 34;
const uint32_t ISTEP5_MAX_SUBSTEPS = 2;
static const uint8_t ISTEP_MINOR_START = 1;
static const uint8_t SLAVE_LAST_MINOR_ISTEP = 20;
static const uint8_t ISTEP2_MINOR_START = 2;
static const uint32_t SBE_ROLE_MASK = 0x00000002;

// Globals
// TODO: via RTC 123602 This global needs to move to a class that will store the
// SBE FFDC.
fapi2::ReturnCode g_iplFailRc = FAPI2_RC_SUCCESS;

sbeRole g_sbeRole = SBE_ROLE_MASTER;

// File static data

static istepMap_t g_istep2PtrTbl[ ISTEP2_MAX_SUBSTEPS ] =
         {
#ifdef SEEPROM_IMAGE
             { NULL, NULL },
             { &istepAttrSetup, { .procHwp = &p9_sbe_attr_setup }},
             { &istepWithProc, { .procHwp = &p9_sbe_tp_chiplet_init1 }},
             { &istepWithProc, { .procHwp = &p9_sbe_tp_gptr_time_initf }},
             { &istepNoOp, NULL },  // DFT only
             { &istepWithProc, { .procHwp = &p9_sbe_npll_initf }},
             { &istepNestFreq, { .procHwp = &p9_sbe_npll_setup }},
             { &istepWithProc, { .procHwp = &p9_sbe_tp_switch_gears }},
             { &istepWithProc, { .procHwp = &p9_sbe_clock_test2 }},
             { &istepWithProc, { .procHwp = &p9_sbe_tp_chiplet_reset }},
             { &istepWithProc, { .procHwp = &p9_sbe_tp_repr_initf }},
             { &istepWithProc, { .procHwp = &p9_sbe_tp_chiplet_init2 }},
             { &istepNoOp, NULL },  // DFT only
             { &istepWithProc, { .procHwp = &p9_sbe_tp_arrayinit }},
             { &istepWithProc, { .procHwp = &p9_sbe_tp_initf }},
             { &istepNoOp, NULL }, // DFT only
             { &istepWithProc, { .procHwp = &p9_sbe_tp_chiplet_init3 }},
#endif
         };

static istepMap_t g_istep3PtrTbl[ ISTEP3_MAX_SUBSTEPS ] =
         {
#ifdef SEEPROM_IMAGE
             { &istepWithProc, { .procHwp = &p9_sbe_chiplet_reset }},
             { &istepWithProc, { .procHwp = &p9_sbe_gptr_time_initf }},
             { &istepWithProc, { .procHwp = &p9_sbe_chiplet_pll_initf }},
             { &istepWithProc, { .procHwp = &p9_sbe_chiplet_pll_setup }},
             { &istepWithProc, { .procHwp = &p9_sbe_repr_initf }},
             { &istepWithProc, { .procHwp = &p9_sbe_chiplet_init }},
             { &istepNoOp, NULL }, // DFT only
             { &istepWithProc, { .procHwp = &p9_sbe_arrayinit }},
             { &istepNoOp, NULL }, // DFT only
             { &istepWithProc, { .procHwp = &p9_sbe_tp_enable_ridi }},
             { &istepWithProc, { .procHwp = &p9_sbe_setup_boot_freq }},
             { &istepWithProc, { .procHwp = &p9_sbe_nest_initf }},
             { &istepWithProc, { .procHwp = &p9_sbe_nest_startclocks }},
             { &istepWithProc, { .procHwp = &p9_sbe_nest_enable_ridi }},
             { &istepWithProc, { .procHwp = &p9_sbe_io_initf }},
             { &istepWithProc, { .procHwp = &p9_sbe_startclock_chiplets }},
             { &istepWithProc, { .procHwp = &p9_sbe_scominit }},
             { &istepWithProc, { .procHwp = &p9_sbe_lpc_init }},
             { &istepWithProc, { .procHwp = &p9_sbe_fabricinit }},
             { &istepCheckSbeMaster, NULL },
             { &istepWithProc, { .procHwp = &p9_sbe_mcs_setup }},
             { &istepSelectEx, NULL },
#endif
         };
static istepMap_t g_istep4PtrTbl[ ISTEP4_MAX_SUBSTEPS ] =
         {
#ifdef SEEPROM_IMAGE
             { &istepWithEq, { .eqHwp  = &p9_hcd_cache_poweron} },
             { &istepWithEq, { .eqHwp  = &p9_hcd_cache_chiplet_reset } },
             { &istepWithEq, { .eqHwp  = &p9_hcd_cache_chiplet_l3_dcc_setup }},
             { &istepWithEq, { .eqHwp  = &p9_hcd_cache_gptr_time_initf }},
             { &istepWithEq, { .eqHwp  = &p9_hcd_cache_dpll_initf }},
             { &istepWithEq, { .eqHwp  = &p9_hcd_cache_dpll_setup }},
             { &istepWithEq, { .eqHwp  = &p9_hcd_cache_dcc_skewadjust_setup }},
             { &istepWithEq, { .eqHwp  = &p9_hcd_cache_chiplet_init }},
             { &istepWithEq, { .eqHwp  = &p9_hcd_cache_repair_initf }},
             { &istepWithEq, { .eqHwp  = &p9_hcd_cache_arrayinit }},
             { &istepNoOp, NULL },  // DFT Only
             { &istepNoOp, NULL },  // DFT Only
             { &istepWithEq, { .eqHwp = &p9_hcd_cache_initf }},
             { &istepWithEqConditional, { .eqHwp = &p9_hcd_cache_startclocks }},
             { &istepWithEqConditional, { .eqHwp = &p9_hcd_cache_scominit }},
             { &istepWithEqConditional, { .eqHwp = &p9_hcd_cache_scomcust }},
             { &istepNoOp, NULL }, // Runtime only
             { &istepNoOp, NULL }, // Runtime only
             { &istepNoOp, NULL }, // stub for SBE
             { &istepNoOp, NULL },  // stub for SBE
             { &istepWithCore, { .coreHwp = &p9_hcd_core_poweron }},
             { &istepWithCore, { .coreHwp = &p9_hcd_core_chiplet_reset }},
             { &istepWithCore, { .coreHwp = &p9_hcd_core_gptr_time_initf }},
             { &istepWithCore, { .coreHwp = &p9_hcd_core_chiplet_init }},
             { &istepWithCore, { .coreHwp = &p9_hcd_core_repair_initf }},
             { &istepWithCore, { .coreHwp = &p9_hcd_core_arrayinit }},
             { &istepNoOp, NULL },  // DFT Only
             { &istepNoOp, NULL },  // DFT Only
             { &istepWithCore, { .coreHwp = &p9_hcd_core_initf }},
             { &istepWithCoreConditional,
                              { .coreHwp = &p9_hcd_core_startclocks }},
             { &istepWithCoreConditional, { .coreHwp = &p9_hcd_core_scominit }},
             { &istepWithCoreConditional, { .coreHwp = &p9_hcd_core_scomcust }},
             { &istepNoOp, NULL },
             { &istepNoOp, NULL },
#endif
         };

// TODO via RTC 135345
//  Add the support for istep 5 HWP
static istepMap_t g_istep5PtrTbl[ ISTEP5_MAX_SUBSTEPS ]
         {
#ifdef SEEPROM_IMAGE
             { &istepLoadBootLoader, NULL },
             { &istepStartInstruction,  { .coreHwp = &p9_sbe_instruct_start }},
#endif
         };

// Functions
//----------------------------------------------------------------------------
uint32_t sbeHandleIstep (uint8_t *i_pArg)
{
    #define SBE_FUNC "sbeHandleIstep "
    SBE_ENTER(SBE_FUNC);
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
    ReturnCode fapiRc = FAPI2_RC_SUCCESS;
    uint32_t len = 0;
    sbeIstepReqMsg_t req;
    sbeRespGenHdr_t respHdr;
    respHdr.init();
    sbeResponseFfdc_t ffdc;

    // NOTE: In this function we will have two loops
    // First loop will deque data and prepare the response
    // Second response will enque the data on DS FIFO
    //loop 1
    do
    {
        len = sizeof( req )/sizeof(uint32_t);
        rc = sbeUpFifoDeq_mult ( len, (uint32_t *)&req);
        if (rc != SBE_SEC_OPERATION_SUCCESSFUL) //FIFO access issue
        {
            SBE_ERROR(SBE_FUNC"FIFO dequeue failed, rc[0x%X]", rc);
            break;
        }

        if( false == validateIstep( req.major, req.minor ) )
        {
            SBE_ERROR(SBE_FUNC" Invalid Istep. major:0x%08x"
                      " minor:0x%08x", req.major, req.minor);
            // @TODO via RTC 132295.
            // Need to change code asper better error handling.
            respHdr.setStatus( SBE_PRI_INVALID_DATA,
                               SBE_SEC_GENERIC_FAILURE_IN_EXECUTION);
            break;
        }

        fapiRc = sbeExecuteIstep( req.major, req.minor );
        if( fapiRc != FAPI2_RC_SUCCESS )
        {
            SBE_ERROR(SBE_FUNC" sbeExecuteIstep() Failed. major:0x%08x"
                                      " minor:0x%08x", req.major, req.minor);
            respHdr.setStatus( SBE_PRI_GENERIC_EXECUTION_FAILURE,
                               SBE_SEC_GENERIC_FAILURE_IN_EXECUTION);
            ffdc.setRc(fapiRc);
            break;
        }

    }while(0);

    //loop 2
    do
    {
        // FIFO error
        if ( rc )
        {
            break;
        }

        rc = sbeDsSendRespHdr(respHdr, &ffdc);
    }while(0);

    if( rc )
    {
        SBE_ERROR( SBE_FUNC"Failed. rc[0x%X]", rc);
    }
    SBE_EXIT(SBE_FUNC);
    return rc;
    #undef SBE_FUNC
}

//----------------------------------------------------------------------------
// @note This is the responsibilty of caller to verify major/minor
//       number before calling this function

// @TODO via RTC 129077.
// This function should check for system checkstop as well.
ReturnCode sbeExecuteIstep (const uint8_t i_major, const uint8_t i_minor)
{
    #define SBE_FUNC "sbeExecuteIstep "
    SBE_INFO(SBE_FUNC"Major number:0x%x minor number:0x%x",
                       i_major, i_minor );

    ReturnCode rc = FAPI2_RC_SUCCESS;
    switch( i_major )
    {
        case SBE_ISTEP2:
            rc = (g_istep2PtrTbl[i_minor-1].istepWrapper)(
                                g_istep2PtrTbl[i_minor-1].istepHwp);
            break;

        case SBE_ISTEP3:
            rc = (g_istep3PtrTbl[i_minor-1].istepWrapper)(
                                g_istep3PtrTbl[i_minor-1].istepHwp);
            break;

        case SBE_ISTEP4:
            rc = (g_istep4PtrTbl[i_minor-1].istepWrapper)(
                              g_istep4PtrTbl[i_minor-1].istepHwp);
            break;

        case SBE_ISTEP5:
            rc = (g_istep5PtrTbl[i_minor-1].istepWrapper)(
                              g_istep5PtrTbl[i_minor-1].istepHwp);
            break;

        // We should never reach here as before calling this validation has
        // been done.
        default:
            assert(0);
            break;
        }

    (void)SbeRegAccess::theSbeRegAccess().updateSbeStep(i_major, i_minor);

    if(rc != FAPI2_RC_SUCCESS)
    {
        // If IPLing State
        uint64_t l_state = SbeRegAccess::theSbeRegAccess().getSbeState();
        if(l_state == SBE_STATE_IPLING)
        {
            (void)SbeRegAccess::theSbeRegAccess().
                  stateTransition(SBE_DUMP_FAILURE_EVENT);
        }
    }

    return rc;
    #undef SBE_FUNC
}

//----------------------------------------------------------------------------
bool validateIstep (const uint8_t i_major, const uint8_t i_minor)
{
    bool valid = true;
    do
    {
        if( 0 == i_minor )
        {
            valid = false;
            break;
        }

        if((SBE_ROLE_SLAVE == g_sbeRole) &&
           ((SBE_ISTEP_LAST_SLAVE < i_major) ||
            ((SBE_ISTEP_LAST_SLAVE == i_major) &&
             (SLAVE_LAST_MINOR_ISTEP < i_minor)
            )))
        {
            // Cannot run beyond 3.20 on a slave SBE
            valid = false;
            break;
        }

        switch( i_major )
        {
            case SBE_ISTEP2:
                // istep 2.1 loads image to PIBMEM
                // So SBE control loop can not execute istep 2.1.
                if(( i_minor > ISTEP2_MAX_SUBSTEPS ) || ( i_minor == 1) )
                {
                    valid = false;
                }
                break;

            case SBE_ISTEP3:
                if( i_minor > ISTEP3_MAX_SUBSTEPS ) { valid = false; } ;
                break;

            case SBE_ISTEP4:
                if( i_minor > ISTEP4_MAX_SUBSTEPS )
                {
                    valid = false;
                }
                break;

            case SBE_ISTEP5:
                if( i_minor > ISTEP5_MAX_SUBSTEPS )
                {
                    valid = false;
                }
                break;

            default:
                valid= false;
                break;
        }
    } while(0);

    return valid;
}

//----------------------------------------------------------------------------

ReturnCode istepAttrSetup( sbeIstepHwp_t i_hwp)
{
    SBE_ENTER("istepAttrSetup");
    Target<TARGET_TYPE_PROC_CHIP > proc = plat_getChipTarget();
    ReturnCode rc = FAPI2_RC_SUCCESS;
    do
    {
        assert( NULL != i_hwp.procHwp );
        SBE_EXEC_HWP(rc, i_hwp.procHwp, proc)
        if( rc != FAPI2_RC_SUCCESS )
        {
            break;
        }
        // Apply the gard records
        rc = plat_ApplyGards();
     }while(0);
    SBE_EXIT("istepAttrSetup");
    return rc;
}

//----------------------------------------------------------------------------

ReturnCode istepWithProc( sbeIstepHwp_t i_hwp)
{
    ReturnCode rc = FAPI2_RC_SUCCESS;
    Target<TARGET_TYPE_PROC_CHIP > proc = plat_getChipTarget();
    assert( NULL != i_hwp.procHwp );
    SBE_EXEC_HWP(rc, i_hwp.procHwp,proc)
    return rc;
}

//----------------------------------------------------------------------------

ReturnCode istepNestFreq( sbeIstepHwp_t i_hwp)
{
    #define SBE_FUNC "istepNestFreq "
    Target<TARGET_TYPE_PROC_CHIP > proc = plat_getChipTarget();
    fapi2::Target<TARGET_TYPE_SYSTEM> sys;
    ReturnCode rc = FAPI2_RC_SUCCESS;
    uint8_t nestPllBkt = 0;
    FAPI_ATTR_GET( ATTR_NEST_PLL_BUCKET, sys, nestPllBkt );
    assert( nestPllBkt && (nestPllBkt <= NEST_PLL_FREQ_BUCKETS ));
    uint32_t sbeFreqHz = ( NEST_PLL_FREQ_LIST[ nestPllBkt - 1 ] * 1000 * 1000 )/
                                                 SBE::SBE_TO_NEST_FREQ_FACTOR;

    assert( NULL != i_hwp.procHwp );
    do
    {
        SBE_EXEC_HWP(rc, i_hwp.procHwp,proc)
        if( rc != FAPI2_RC_SUCCESS )
        {
            break;
        }
        // This function signature though has return value
        // But it always return SUCCESS.
        SBE_INFO(SBE_FUNC"Setting new frequency:0x%08X", sbeFreqHz);
        pk_timebase_freq_set(sbeFreqHz);
    }while(0);
    return rc;
    #undef SBE_FUNC
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

ReturnCode istepSelectEx( sbeIstepHwp_t i_hwp)
{
    ReturnCode rc = FAPI2_RC_SUCCESS;
    Target<TARGET_TYPE_PROC_CHIP > proc = plat_getChipTarget();
    // TODO via RTC 135345
    // Once multicast targets are supported, we may need to pass
    // p9selectex::ALL as input.
    SBE_EXEC_HWP(rc, p9_sbe_select_ex_hwp, proc, p9selectex::SINGLE)
    return rc;
}

//----------------------------------------------------------------------------


ReturnCode istepWithEq( sbeIstepHwp_t i_hwp)
{
    ReturnCode rc = FAPI2_RC_SUCCESS;
    // TODO via RTC 135345
    // Curently we are passing Hard code eq target. Finally it is
    // going to be a multicast target. Once multicast support is
    // present, use the right target.
    fapi2::Target<fapi2::TARGET_TYPE_EQ > eqTgt;
    // Put this in scope so that vector can be freed up before calling hwp.
    {
        Target<TARGET_TYPE_PROC_CHIP > proc = plat_getChipTarget();
        auto eqList = proc.getChildren<fapi2::TARGET_TYPE_EQ>();
        // As it is workaround lets assume there will always be atleast one
        // functional eq. No need to validate.
        eqTgt = eqList[0];
    }

    assert( NULL != i_hwp.eqHwp );
    SBE_EXEC_HWP(rc, i_hwp.eqHwp, eqTgt )
    return rc;
}

//----------------------------------------------------------------------------

ReturnCode istepWithCore( sbeIstepHwp_t i_hwp)
{
    ReturnCode rc = FAPI2_RC_SUCCESS;
    // TODO via RTC 135345
    // Curently we are passing Hard code core target. Finally it is
    // going to be a multicast target. Once multicast support is
    // present, use the right target.
    fapi2::Target<fapi2::TARGET_TYPE_CORE > coreTgt;
    // Put this in scope so that vector can be freed up before calling hwp.
    {
        Target<TARGET_TYPE_PROC_CHIP > proc = plat_getChipTarget();
        auto coreList = proc.getChildren<fapi2::TARGET_TYPE_CORE>();
        // As it is workaround lets assume there will always be atleast one
        // functional ec. No need to validate.
        coreTgt = coreList[0];
    }
    assert( NULL != i_hwp.coreHwp );
    SBE_EXEC_HWP(rc, i_hwp.coreHwp, coreTgt )
    return rc;
}

//----------------------------------------------------------------------------

ReturnCode istepWithEqConditional( sbeIstepHwp_t i_hwp)
{
    SBE_ENTER("istepWithEqCondtional");
    fapi2::Target<fapi2::TARGET_TYPE_SYSTEM > sysTgt;
    ReturnCode rc = FAPI2_RC_SUCCESS;
    do
    {
        uint8_t iplPhase = ENUM_ATTR_SYSTEM_IPL_PHASE_HB_IPL;
        FAPI_ATTR_GET(ATTR_SYSTEM_IPL_PHASE, sysTgt, iplPhase);
        if( ENUM_ATTR_SYSTEM_IPL_PHASE_CACHE_CONTAINED == iplPhase )
        {
            break;
        }
        rc = istepWithEq(i_hwp);
     }while(0);
    SBE_EXIT("istepWithEqCondtional");
    return rc;
}

//----------------------------------------------------------------------------

ReturnCode istepWithCoreConditional( sbeIstepHwp_t i_hwp)
{
    SBE_ENTER("istepWithCoreCondtional");
    fapi2::Target<fapi2::TARGET_TYPE_SYSTEM > sysTgt;
    ReturnCode rc = FAPI2_RC_SUCCESS;
    do
    {
        uint8_t iplPhase = ENUM_ATTR_SYSTEM_IPL_PHASE_HB_IPL;
        FAPI_ATTR_GET(ATTR_SYSTEM_IPL_PHASE, sysTgt, iplPhase);
        if( ENUM_ATTR_SYSTEM_IPL_PHASE_CACHE_CONTAINED == iplPhase )
        {
            break;
        }
        rc = istepWithCore(i_hwp);
     }while(0);
    SBE_EXIT("istepWithCoreCondtional");
    return rc;
}

//----------------------------------------------------------------------------

ReturnCode istepLoadBootLoader( sbeIstepHwp_t i_hwp)
{
    ReturnCode rc = FAPI2_RC_SUCCESS;
    // Get master Ex
    uint8_t exId = 0;
    Target<TARGET_TYPE_PROC_CHIP > proc = plat_getChipTarget();
    FAPI_ATTR_GET(fapi2::ATTR_MASTER_EX,proc,exId);
    fapi2::Target<fapi2::TARGET_TYPE_EX >
        exTgt(plat_getTargetHandleByInstance<fapi2::TARGET_TYPE_EX>(exId));
    // Get hbbl section
    P9XipHeader *hdr = getXipHdr();
    P9XipSection *hbblSection =  &(hdr->iv_section[P9_XIP_SECTION_SBE_HBBL]);

    SBE_EXEC_HWP(rc, p9_sbe_load_bootloader, proc, exTgt, hbblSection->iv_size,
                                            getSectionAddr(hbblSection) )
    return rc;
}

//----------------------------------------------------------------------------

ReturnCode istepStartInstruction( sbeIstepHwp_t i_hwp)
{
    ReturnCode rc = FAPI2_RC_SUCCESS;
    rc = istepWithCore(i_hwp);
    if(rc == FAPI2_RC_SUCCESS)
    {
        (void)SbeRegAccess::theSbeRegAccess().stateTransition(
                                          SBE_RUNTIME_EVENT);
    }
    return rc;
}

//----------------------------------------------------------------------------
ReturnCode istepCheckSbeMaster( sbeIstepHwp_t i_hwp)
{
    ReturnCode rc = FAPI2_RC_SUCCESS;
    g_sbeRole = SbeRegAccess::theSbeRegAccess().isSbeSlave() ?
                SBE_ROLE_SLAVE : SBE_ROLE_MASTER;
    SBE_INFO("stepCheckSbeMaster g_sbeRole [%x]", g_sbeRole);
    if(SBE_ROLE_SLAVE == g_sbeRole)
    {
        (void)SbeRegAccess::theSbeRegAccess().stateTransition(
                                        SBE_RUNTIME_EVENT);
    }
    return rc;
}

//----------------------------------------------------------------------------
ReturnCode istepNoOp( sbeIstepHwp_t i_hwp)
{
    SBE_INFO("istepNoOp");
    return FAPI2_RC_SUCCESS ;
}

// Only allowed in PLCK Mode, since FFDC State mode is set only in PLCK
//----------------------------------------------------------------------------
uint32_t sbeContinueBoot (uint8_t *i_pArg)
{
    #define SBE_FUNC "sbeContinueBoot "
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
    uint32_t len = 0;
    sbeRespGenHdr_t respHdr;
    respHdr.init();

    do
    {
        // Dequeue the EOT entry as no more data is expected.
        rc = sbeUpFifoDeq_mult (len, NULL);
        if(rc != SBE_SEC_OPERATION_SUCCESSFUL)
        {
            // let command processor routine handle the RC
            break;
        }

        uint32_t distance = 1;
        len = sizeof(respHdr)/sizeof(uint32_t);
        rc = sbeDownFifoEnq_mult ( len, ( uint32_t *) &respHdr);
        if (rc)
        {
            break;
        }
        distance += len;

        len = sizeof(distance)/sizeof(uint32_t);
        rc = sbeDownFifoEnq_mult ( len, &distance);
        if (rc)
        {
            break;
        }
        rc = sbeDownFifoSignalEot();
        if (rc)
        {
            break;
        }

        // Expecting this to be in PLCK Mode and not in Istep mode
        if(SbeRegAccess::theSbeRegAccess().isDestBitRuntime())
        {
            (void)SbeRegAccess::theSbeRegAccess().stateTransition(
                                    SBE_CONTINUE_BOOT_RUNTIME_EVENT);
            // Nothing to do here.
        }
        else
        {
            SBE_INFO(SBE_FUNC"Continuous IPL Mode set... IPLing");
            (void)SbeRegAccess::theSbeRegAccess().stateTransition(
                                    SBE_CONTINUE_BOOT_PLCK_EVENT);
            sbeDoContinuousIpl();
        }
    }while(0);

    SBE_DEBUG(SBE_FUNC "RC = 0x%08X", rc);
    return rc;
    #undef SBE_FUNC
}

//----------------------------------------------------------------------------
void sbeDoContinuousIpl()
{
    #define SBE_FUNC "sbeDoContinuousIpl "
    SBE_ENTER(SBE_FUNC);
    ReturnCode l_rc = FAPI2_RC_SUCCESS;
    do
    {
        // An array that holds the max number of minor steps per major step
        const uint8_t l_minorSteps[] =
        {
            ISTEP2_MAX_SUBSTEPS,
            ISTEP3_MAX_SUBSTEPS,
            ISTEP4_MAX_SUBSTEPS,
            ISTEP5_MAX_SUBSTEPS
        };

        // Where does each minor istep start from?
        const uint8_t l_minorStartStep[] =
        {
            ISTEP2_MINOR_START,
            ISTEP_MINOR_START,
            ISTEP_MINOR_START,
            ISTEP_MINOR_START
        };

        // Set SBE state as IPLing
        (void)SbeRegAccess::theSbeRegAccess().stateTransition(
                                                SBE_PLCK_EVENT);
        bool l_done = false;
        // Run isteps
        for(uint8_t l_major = SBE_ISTEP_FIRST;
            (l_major <= SBE_ISTEP_LAST_MASTER) &&
            (false == l_done);
            ++l_major)
        {
            for(uint8_t l_minor = l_minorStartStep[l_major - SBE_ISTEP_FIRST];
                l_minor <= l_minorSteps[l_major - SBE_ISTEP_FIRST];
                ++l_minor)
            {
                l_rc = sbeExecuteIstep(l_major, l_minor);
                if(l_rc != FAPI2_RC_SUCCESS)
                {
                    SBE_ERROR(SBE_FUNC"Failed istep execution in plck mode: "
                            "Major: %d, Minor: %d", l_major, l_minor);
                    l_done = true;
                    break;
                }
                // Check if we are at step 3.20 on the slave SBE
                if(((SBE_ISTEP_LAST_SLAVE == l_major) &&
                        (SLAVE_LAST_MINOR_ISTEP == l_minor)) &&
                        (SBE_ROLE_SLAVE == g_sbeRole))
                {
                    l_done = true;
                    break;
                }
            }
        }
    } while(false);
    // Store l_rc in a global variable that will be a part of the SBE FFDC
    g_iplFailRc = l_rc;
    SBE_EXIT(SBE_FUNC);
    #undef SBE_FUNC
}

// TODO - RTC 133367
//----------------------------------------------------------------------------
uint32_t sbeEnterMpipl(uint8_t *i_pArg)
{
    #define SBE_FUNC " sbeEnterMpipl "
    uint32_t l_rc = SBE_SEC_OPERATION_SUCCESSFUL;
    uint32_t len = 0;
    sbeRespGenHdr_t l_respHdr;
    l_respHdr.init();

    do
    {
        // Dequeue the EOT entry as no more data is expected.
        l_rc = sbeUpFifoDeq_mult (len, NULL);
        if ( l_rc != SBE_SEC_OPERATION_SUCCESSFUL )
        {
            // Let command processor routine to handle the RC
            break;
        }

        // TODO via  RTC:123696  MPIPL Related procedure/steps
        // Can send FFDC if MPIPL procedure fails
        l_rc = sbeDsSendRespHdr( l_respHdr );

        // set state to MPIPL Wait
        (void)SbeRegAccess::theSbeRegAccess().
               stateTransition(SBE_ENTER_MPIPL_EVENT);

    }while(0);
    return l_rc;
    #undef SBE_FUNC
}

// TODO - RTC 133367
//----------------------------------------------------------------------------
uint32_t sbeContinueMpipl(uint8_t *i_pArg)
{
    #define SBE_FUNC " sbeContinueMpipl "
    uint32_t l_rc = SBE_SEC_OPERATION_SUCCESSFUL;
    uint32_t len = 0;
    sbeRespGenHdr_t l_respHdr;
    l_respHdr.init();

    do
    {
        // Dequeue the EOT entry as no more data is expected.
        l_rc = sbeUpFifoDeq_mult (len, NULL);
        if ( l_rc != SBE_SEC_OPERATION_SUCCESSFUL )
        {
            // Let command processor routine to handle the RC
            break;
        }

        l_rc = sbeDsSendRespHdr( l_respHdr);

        //TODO RTC-134278  Continue MPIPL Related procedure/steps

        // TODO - Once continue steps are over, it will trigger the
        // istep5.2 and transition to runtime will happen

    }while(0);

    return l_rc;
    #undef SBE_FUNC
}



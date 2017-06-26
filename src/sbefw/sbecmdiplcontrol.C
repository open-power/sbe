/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/sbecmdiplcontrol.C $                                */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2017                        */
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
#include "sbecmdcntrldmt.H"

#include "fapi2.H"
#include "p9_misc_scom_addresses_fld.H"
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

// istep mpipl header files
#include "p9_block_wakeup_intr.H"
#include "p9_query_core_access_state.H"
#include "p9_sbe_check_quiesce.H"
#include "p9_l2_flush.H"
#include "p9_l3_flush.H"
#include "p9_sbe_sequence_drtm.H"
#include "p9_thread_control.H"
#include "sbecmdcntlinst.H"
#include "p9_quad_power_off.H"
#include "p9_hcd_cache_stopclocks.H"
#include "p9_stopclocks.H"

#include "sbeXipUtils.H" // For getting hbbl offset
#include "sbeutil.H" // For getting SBE_TO_NEST_FREQ_FACTOR
// Forward declaration
using namespace fapi2;

bool validateIstep (uint8_t i_major, uint8_t i_minor);

//typedefs
typedef ReturnCode (*sbeIstepHwpProc_t)
                    (const Target<TARGET_TYPE_PROC_CHIP> & i_target);

typedef ReturnCode (*sbeIstepHwpEq_t)
                    (const Target<TARGET_TYPE_EQ> & i_target);

typedef ReturnCode (*sbeIstepHwpCore_t)
                    (const Target<TARGET_TYPE_CORE> & i_target);

typedef ReturnCode (*sbeIstepHwpExL2Flush_t)
                    (const Target<TARGET_TYPE_EX> & i_target,
                     const p9core::purgeData_t & i_purgeData);

typedef ReturnCode (*sbeIstepHwpExL3Flush_t)
                    (const Target<TARGET_TYPE_EX> & i_target,
                     const uint32_t i_purgeType,
                     const uint32_t i_purgeAddr);

typedef ReturnCode (*sbeIstepHwpCoreBlockIntr_t)
                    (const Target<TARGET_TYPE_CORE> & i_target,
                     const p9pmblockwkup::OP_TYPE i_oper);

typedef ReturnCode (*sbeIstepHwpCoreScomState_t)
                    (const Target<TARGET_TYPE_CORE> & i_target,
                     bool & o_isScom,
                     bool & o_isScan);

typedef ReturnCode (*sbeIstepHwpSequenceDrtm_t)
                    (const Target<TARGET_TYPE_PROC_CHIP> & i_target,
                     uint8_t & o_status);

typedef ReturnCode (*sbeIstepHwpQuadPoweroff_t)
                    (const Target<TARGET_TYPE_EQ> & i_target,
                     uint64_t * o_ring_save_data);

typedef ReturnCode (*sbeIstepHwpCacheInitf_t)
                    (const Target<TARGET_TYPE_EQ> & i_target,
                    const uint64_t * i_ring_save_data);

typedef union
{
    sbeIstepHwpProc_t procHwp;
    sbeIstepHwpEq_t eqHwp;
    sbeIstepHwpCore_t coreHwp;
    sbeIstepHwpExL2Flush_t exL2Hwp;
    sbeIstepHwpExL3Flush_t exL3Hwp;
    sbeIstepHwpCoreBlockIntr_t coreBlockIntrHwp;
    sbeIstepHwpCoreScomState_t coreScomStateHwp;
    sbeIstepHwpSequenceDrtm_t  procSequenceDrtm;
    sbeIstepHwpQuadPoweroff_t  quadPoweroffHwp;
    sbeIstepHwpCacheInitf_t    cacheInitfHwp;
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
ReturnCode istepLpcInit( sbeIstepHwp_t i_hwp);
ReturnCode istepCacheInitf( sbeIstepHwp_t i_hwp );

//MPIPL Specific
ReturnCode istepWithCoreSetBlock( sbeIstepHwp_t i_hwp );
ReturnCode istepWithCoreState( sbeIstepHwp_t i_hwp );
ReturnCode istepMpiplRstClrTpmBits( sbeIstepHwp_t i_hwp );
ReturnCode istepWithProcQuiesceLQASet( sbeIstepHwp_t i_hwp );
ReturnCode istepWithExL2Flush( sbeIstepHwp_t i_hwp );
ReturnCode istepWithExL3Flush( sbeIstepHwp_t i_hwp );
ReturnCode istepStartMpipl( sbeIstepHwp_t i_hwp );
ReturnCode istepWithProcSequenceDrtm( sbeIstepHwp_t i_hwp );
ReturnCode istepMpiplSetFunctionalState( sbeIstepHwp_t i_hwp );
ReturnCode istepMpiplQuadPoweroff( sbeIstepHwp_t i_hwp );
ReturnCode istepStopClockMpipl( sbeIstepHwp_t i_hwp );

// Utility function to do TPM reset
ReturnCode performTpmReset();

//Utility function to update PHB functional State
ReturnCode updatePhbFunctionalState( void );

#ifdef SEEPROM_IMAGE
// Using function pointer to force long call.
p9_sbe_select_ex_FP_t p9_sbe_select_ex_hwp = &p9_sbe_select_ex;
extern p9_stopclocks_FP_t p9_stopclocks_hwp;
//p9_thread_control_FP_t threadCntlhwp = &p9_thread_control;
extern p9_thread_control_FP_t threadCntlhwp;
#endif

//structure for mapping SBE wrapper and HWP functions
typedef struct
{
    sbeIstep_t istepWrapper;
    sbeIstepHwp_t istepHwp;
}istepMap_t;


// constants
static const uint32_t SBE_ROLE_MASK = 0x00000002;
static const uint32_t SBE_SYSTEM_QUIESCE_TIMEOUT_LOOP = 25;

static const uint64_t SBE_LQA_DELAY_HW_US = 1000000ULL; // 1ms
static const uint64_t SBE_LQA_DELAY_SIM_CYCLES = 0x1ULL;

static const uint32_t PEC_PHB_IOVALID_BIT_SHIFT = 59;
static const uint64_t PEC_PHB_IOVALID_BIT_MASK = 0x1ULL;

// Bit-33 used to checkstop the system, Since this is directly getting inserted
// will have to use bit (63-33) = 30th bit
static const uint64_t  N3_FIR_SYSTEM_CHECKSTOP_BIT = 30; // 63-33 = 30

// Globals
// TODO: via RTC 123602 This global needs to move to a class that will store the
// SBE FFDC.
fapi2::ReturnCode g_iplFailRc = FAPI2_RC_SUCCESS;

uint64_t G_ring_save[8] = {0, 0, 0, 0, 0, 0, 0, 0};

sbeRole g_sbeRole = SBE_ROLE_MASTER;

static istepMap_t g_istepMpiplStartPtrTbl[MPIPL_START_MAX_SUBSTEPS] =
        {
#ifdef SEEPROM_IMAGE
            // Place holder for StartMpipl, State Change, PHB State Update
            // Set MPIPL mode in Sratch Reg 3
            { &istepStartMpipl, NULL },
            // Find all the child cores within proc and call set block intr
            { &istepWithCoreSetBlock, { .coreBlockIntrHwp = &p9_block_wakeup_intr }},
            // Find all the child cores within proc and call hwp to know the
            // scom state and call instruction control
            { &istepWithCoreState, { .coreScomStateHwp = &p9_query_core_access_state }},
            //  Reset the TPM and clear the TPM deconfig bit, it's not a
            //  procedure but local SBE function
            { &istepMpiplRstClrTpmBits, NULL },
            // quiesce state for all units on the powerbus on its chip
            { &istepWithProcQuiesceLQASet, { .procHwp = &p9_sbe_check_quiesce }},
            // L2 cache flush via purge engine on each EX
            { &istepWithExL2Flush, { .exL2Hwp = &p9_l2_flush }},
            // L3 cache flush via purge engine on each EX
            { &istepWithExL3Flush, { .exL3Hwp = &p9_l3_flush }},
            // Check on Quiescing of all Chips in a System by Local SBE
            { &istepWithProcSequenceDrtm, { .procSequenceDrtm = &p9_sbe_sequence_drtm }},
#endif
        };

static istepMap_t g_istepMpiplContinuePtrTbl[MPIPL_CONTINUE_MAX_SUBSTEPS] =
        {
#ifdef SEEPROM_IMAGE
            // Setup EC/EQ guard records
            { &istepMpiplSetFunctionalState, NULL},
            // p9_quad_power_off
            { istepMpiplQuadPoweroff, { .quadPoweroffHwp = &p9_quad_power_off} },
            // No-op
            { &istepNoOp, NULL},
#endif
        };

static istepMap_t g_istepStopClockPtrTbl[ISTEP_STOPCLOCK_MAX_SUBSTEPS] =
        {
#ifdef SEEPROM_IMAGE
            // Stop Clock Mpipl
            { &istepStopClockMpipl, NULL},
#endif
        };

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
             { &istepLpcInit,  { .procHwp = &p9_sbe_lpc_init }},
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
             { &istepCacheInitf, { .cacheInitfHwp = &p9_hcd_cache_initf }},
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
                      " minor:0x%08x",
                      (uint32_t)req.major, (uint32_t)req.minor);
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
                                      " minor:0x%08x",
                                     (uint32_t)req.major,
                                     (uint32_t)req.minor);
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

        case SBE_ISTEP_MPIPL_START:
            rc = (g_istepMpiplStartPtrTbl[i_minor-1].istepWrapper)(
                        g_istepMpiplStartPtrTbl[i_minor-1].istepHwp);
            break;

        case SBE_ISTEP_MPIPL_CONTINUE:
            rc = (g_istepMpiplContinuePtrTbl[i_minor-1].istepWrapper)(
                        g_istepMpiplContinuePtrTbl[i_minor-1].istepHwp);
            break;

        case SBE_ISTEP_STOPCLOCK:
            rc = (g_istepStopClockPtrTbl[i_minor-1].istepWrapper)(
                        g_istepStopClockPtrTbl[i_minor-1].istepHwp);
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
        SBE_ERROR( SBE_FUNC" FAPI RC:0x%08X", rc);
        (void)SbeRegAccess::theSbeRegAccess().stateTransition(
                                                    SBE_DUMP_FAILURE_EVENT);
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
                if( (i_minor > ISTEP3_MAX_SUBSTEPS ) ||
                    ((SBE_ROLE_SLAVE == g_sbeRole) &&
                    (i_minor > SLAVE_LAST_MINOR_ISTEP)) )
                {
                    valid = false;
                }
                break;

            case SBE_ISTEP4:
                if( (i_minor > ISTEP4_MAX_SUBSTEPS ) ||
                    (SBE_ROLE_SLAVE == g_sbeRole) )
                {
                    valid = false;
                }
                break;

            case SBE_ISTEP5:
                if( (i_minor > ISTEP5_MAX_SUBSTEPS ) ||
                    (SBE_ROLE_SLAVE == g_sbeRole) )
                {
                    valid = false;
                }
                break;

            case SBE_ISTEP_MPIPL_START:
                if( i_minor > MPIPL_START_MAX_SUBSTEPS )
                {
                    valid = false;
                }
                break;

            case SBE_ISTEP_MPIPL_CONTINUE:
                if( i_minor > MPIPL_CONTINUE_MAX_SUBSTEPS )
                {
                    valid = false;
                }
                break;

            case SBE_ISTEP_STOPCLOCK:
                if( i_minor > ISTEP_STOPCLOCK_MAX_SUBSTEPS )
                {
                    valid = false;
                }
                break;

            default:
                valid = false;
                break;
        }
    } while(0);

    return valid;
}

//----------------------------------------------------------------------------

ReturnCode performAttrSetup( )
{
    #define SBE_FUNC "performAttrSetup "
    SBE_ENTER("performAttrSetup ");
    Target<TARGET_TYPE_PROC_CHIP > proc = plat_getChipTarget();
    ReturnCode rc = FAPI2_RC_SUCCESS;
    do
    {
        SBE_EXEC_HWP(rc, p9_sbe_attr_setup, proc)
        if( rc != FAPI2_RC_SUCCESS )
        {
            break;
        }
        // Apply the gard records
        rc = plat_ApplyGards();
     }while(0);
    SBE_EXIT(SBE_FUNC);
    return rc;
    #undef SBE_FUNC
}

//----------------------------------------------------------------------------

ReturnCode istepAttrSetup( sbeIstepHwp_t i_hwp)
{
    return performAttrSetup();
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
    ReturnCode rc = FAPI2_RC_SUCCESS;
    assert( NULL != i_hwp.procHwp );
    do
    {
        SBE_EXEC_HWP(rc, i_hwp.procHwp,proc)
        if( rc != FAPI2_RC_SUCCESS )
        {
            break;
        }
        // Update PK frequency
        SBE::updatePkFreq();
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
ReturnCode istepCacheInitf (sbeIstepHwp_t i_hwp )
{
    #define SBE_FUNC "istepCacheInitf"
    SBE_ENTER(SBE_FUNC);
    ReturnCode l_rc = FAPI2_RC_SUCCESS;

    // TODO via RTC 135345
    fapi2::Target<fapi2::TARGET_TYPE_EQ > eqTgt;
    // Put this in scope so that vector can be freed up before calling hwp.
    {
        Target<TARGET_TYPE_PROC_CHIP > proc = plat_getChipTarget();
        auto eqList = proc.getChildren<fapi2::TARGET_TYPE_EQ>();
        // As it is workaround lets assume there will always be atleast one
        // functional eq. No need to validate.
        eqTgt = eqList[0];
    }

    SBE_EXEC_HWP(l_rc, i_hwp.cacheInitfHwp, eqTgt, G_ring_save)
    SBE_EXIT(SBE_FUNC);
    return l_rc;
    #undef SBE_FUNC
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
    #define SBE_FUNC "istepCheckSbeMaster "
    ReturnCode rc = FAPI2_RC_SUCCESS;
    do
    {
        rc = performTpmReset();
        if( rc != FAPI2_RC_SUCCESS )
        {
            SBE_ERROR(SBE_FUNC" performTpmReset failed");
            break;
        }
        g_sbeRole = SbeRegAccess::theSbeRegAccess().isSbeSlave() ?
                    SBE_ROLE_SLAVE : SBE_ROLE_MASTER;
        SBE_INFO(SBE_FUNC"g_sbeRole [%x]", g_sbeRole);
        if(SBE_ROLE_SLAVE == g_sbeRole)
        {
            (void)SbeRegAccess::theSbeRegAccess().stateTransition(
                                            SBE_RUNTIME_EVENT);
        }
    }while(0);
    return rc;
    #undef SBE_FUNC
}

//----------------------------------------------------------------------------
ReturnCode istepNoOp( sbeIstepHwp_t i_hwp)
{
    SBE_INFO("istepNoOp");
    return FAPI2_RC_SUCCESS ;
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

// MPIPL Specific
//----------------------------------------------------------------------------
ReturnCode istepWithCoreSetBlock( sbeIstepHwp_t i_hwp)
{
    #define SBE_FUNC "istepWithCoreSetBlock"
    SBE_ENTER(SBE_FUNC);
    ReturnCode l_rc = FAPI2_RC_SUCCESS;
    Target<TARGET_TYPE_PROC_CHIP > l_procTgt = plat_getChipTarget();
    for (auto l_coreTgt : l_procTgt.getChildren<fapi2::TARGET_TYPE_CORE>())
    {
        SBE_EXEC_HWP(l_rc, i_hwp.coreBlockIntrHwp, l_coreTgt, p9pmblockwkup::SET)
        if(l_rc != FAPI2_RC_SUCCESS)
        {
            SBE_ERROR(SBE_FUNC " p9_block_wakeup_intr failed, RC=[0x%08X]",
                l_rc);
            break;
        }
    }
    SBE_EXIT(SBE_FUNC);
    return l_rc;
    #undef SBE_FUNC
}

//----------------------------------------------------------------------------
ReturnCode istepWithCoreState( sbeIstepHwp_t i_hwp)
{
    #define SBE_FUNC "istepWithCoreState"
    SBE_ENTER(SBE_FUNC);
    ReturnCode l_rc = FAPI2_RC_SUCCESS;
    Target<TARGET_TYPE_PROC_CHIP > l_procTgt = plat_getChipTarget();
    for (auto l_coreTgt : l_procTgt.getChildren<fapi2::TARGET_TYPE_CORE>())
    {
        bool l_isScanEnable = false;
        bool l_isCoreScomEnabled = false;
        SBE_EXEC_HWP(l_rc, i_hwp.coreScomStateHwp, l_coreTgt,
                     l_isCoreScomEnabled, l_isScanEnable)
        if(l_rc != FAPI2_RC_SUCCESS)
        {
            SBE_ERROR(SBE_FUNC " p9_query_core_access_state failed, "
               "RC=[0x%08X]", l_rc);
            break;
        }
        if(l_isCoreScomEnabled) //true
        {
            uint8_t l_thread = SMT4_THREAD0;
            fapi2::buffer<uint64_t> l_data64;
            uint64_t l_state;
            bool l_warnCheck = true;
            do
            {
                // Call instruction control stop
                // TODO RTC 164425 - Can we pass in 1111 i.e. all threads at the
                // same time instead of individual threads
                SBE_EXEC_HWP(l_rc, threadCntlhwp, l_coreTgt,
                             (SINGLE_THREAD_BIT_MASK >> l_thread),
                             PTC_CMD_STOP, l_warnCheck,l_data64, l_state)
                if(l_rc != FAPI2_RC_SUCCESS)
                {
                    SBE_ERROR(SBE_FUNC "p9_thread_control stop Failed for "
                        "Core Thread  RC[0x%08X]", l_rc);
                    break;
                }
            }while(++l_thread < SMT4_THREAD_MAX);
        }
    }
    SBE_EXIT(SBE_FUNC);
    return l_rc;
    #undef SBE_FUNC
}

//----------------------------------------------------------------------------
ReturnCode istepMpiplRstClrTpmBits( sbeIstepHwp_t i_hwp)
{
    #define SBE_FUNC "istepMpiplRstClrTpmBits"
    SBE_ENTER(SBE_FUNC);

    ReturnCode l_rc = performTpmReset();
    if( l_rc != FAPI2_RC_SUCCESS )
    {
        SBE_ERROR(SBE_FUNC" performTpmReset failed");
    }

    SBE_EXIT(SBE_FUNC);
    return l_rc;
    #undef SBE_FUNC
}

//----------------------------------------------------------------------------
ReturnCode istepWithExL2Flush( sbeIstepHwp_t i_hwp)
{
    #define SBE_FUNC "istepWithExL2Flush"
    SBE_ENTER(SBE_FUNC);
    ReturnCode l_rc = FAPI2_RC_SUCCESS;

    Target<TARGET_TYPE_PROC_CHIP > l_procTgt = plat_getChipTarget();
    for (auto l_exTgt : l_procTgt.getChildren<fapi2::TARGET_TYPE_EX>())
    {
        p9core::purgeData_t l_purgeData;
        // TODO RTC 164425 need to check if L2 is Scomable
        // This will come from the HWP team.
        SBE_EXEC_HWP(l_rc, i_hwp.exL2Hwp, l_exTgt, l_purgeData)
        if(l_rc != FAPI2_RC_SUCCESS)
        {
            SBE_ERROR(SBE_FUNC " p9_l2_flush failed, RC=[0x%08X]", l_rc);
            break;
        }
    }
    SBE_EXIT(SBE_FUNC);
    return l_rc;
    #undef SBE_FUNC
}

//----------------------------------------------------------------------------
ReturnCode istepWithExL3Flush( sbeIstepHwp_t i_hwp)
{
    #define SBE_FUNC "istepWithExL3Flush"
    SBE_ENTER(SBE_FUNC);
    ReturnCode l_rc = FAPI2_RC_SUCCESS;

    Target<TARGET_TYPE_PROC_CHIP > l_procTgt = plat_getChipTarget();
    for (auto l_exTgt : l_procTgt.getChildren<fapi2::TARGET_TYPE_EX>())
    {
        // TODO RTC 164425 need to check if L3 is Scomable
        // This will come from the HWP team.
        SBE_EXEC_HWP(l_rc, i_hwp.exL3Hwp, l_exTgt, L3_FULL_PURGE, 0x0)
        if(l_rc != FAPI2_RC_SUCCESS)
        {
            SBE_ERROR(SBE_FUNC " p9_l3_flush failed, RC=[0x%08X]", l_rc);
            break;
        }
    }
    SBE_EXIT(SBE_FUNC);
    return l_rc;
    #undef SBE_FUNC
}

//----------------------------------------------------------------------------
ReturnCode istepWithProcSequenceDrtm( sbeIstepHwp_t i_hwp)
{
    #define SBE_FUNC "istepWithProcSequenceDrtm"
    SBE_ENTER(SBE_FUNC);
    ReturnCode l_rc = FAPI2_RC_SUCCESS;
    Target<TARGET_TYPE_PROC_CHIP > l_procTgt = plat_getChipTarget();

    uint8_t l_status = 0;
    size_t l_timeOut = SBE_SYSTEM_QUIESCE_TIMEOUT_LOOP;
    while(l_timeOut)
    {
        SBE_EXEC_HWP(l_rc, i_hwp.procSequenceDrtm, l_procTgt, l_status)
        if(l_rc != FAPI2_RC_SUCCESS)
        {
            SBE_ERROR(SBE_FUNC "p9_sbe_sequence_drtm failed, RC=[0x%08X]",l_rc);
            break;
        }
        if(l_status)
        {
            SBE_INFO(SBE_FUNC "p9_sbe_sequence_drtm LQA SBE System Quiesce done");
            break;
        }
        else
        {
            l_timeOut--;
            // delay prior to repeating the above
            FAPI_TRY(fapi2::delay(SBE_LQA_DELAY_HW_US, SBE_LQA_DELAY_SIM_CYCLES),
                     "Error from delay");
        }
    }
    // Checkstop system if SBE system quiesce not set after the loop
    if(!l_status || l_rc)
    {
        SBE_ERROR(SBE_FUNC "p9_sbe_sequence_drtm LQA SBE System Quiesce failed,"
            "Either System Quiesce Achieved not true or procedure "
            "failed RC=[0x%08X]",l_rc);
        // check stop the system
        // TODO RTC 164425 this needs to be replicated on any MPIPL Hwp failure
        Target<TARGET_TYPE_PROC_CHIP > l_proc = plat_getChipTarget();
        l_rc = putscom_abs_wrap(&l_proc, PERV_N3_LOCAL_FIR_OR,
                                ((uint64_t)1 << N3_FIR_SYSTEM_CHECKSTOP_BIT));
        if(l_rc != FAPI2_RC_SUCCESS)
        {
            // Scom failed
            SBE_ERROR(SBE_FUNC "PutScom failed for REG PERV_N3_LOCAL_FIR");
            // TODO - Store the response in Async Response
            // RTC:149074
        }
    }
fapi_try_exit:
    if(fapi2::current_err)
    {
        l_rc = fapi2::current_err;
    }
    SBE_EXIT(SBE_FUNC);
    return l_rc;
    #undef SBE_FUNC
}

//----------------------------------------------------------------------------
ReturnCode istepStartMpipl( sbeIstepHwp_t i_hwp)
{
    #define SBE_FUNC "istepStartMpipl"
    SBE_ENTER(SBE_FUNC);
    ReturnCode rc = FAPI2_RC_SUCCESS;

    (void)SbeRegAccess::theSbeRegAccess().stateTransition(
            SBE_ENTER_MPIPL_EVENT);
    // Set MPIPL mode bit in Scratch Reg 3
    (void)SbeRegAccess::theSbeRegAccess().setMpIplMode(true);

    rc = updatePhbFunctionalState();
    if(rc != FAPI2_RC_SUCCESS)
    {
        SBE_ERROR(SBE_FUNC "updatePhbFunctionalState failed");
    }

    SBE_EXIT(SBE_FUNC);
    return rc;
    #undef SBE_FUNC
}

//----------------------------------------------------------------------------
ReturnCode istepMpiplQuadPoweroff( sbeIstepHwp_t i_hwp)
{
    #define SBE_FUNC "istepMpiplQuadPoweroff"
    SBE_ENTER(SBE_FUNC);
    ReturnCode l_rc = FAPI2_RC_SUCCESS;
    if(g_sbeRole == SBE_ROLE_MASTER)
    {
        Target<TARGET_TYPE_PROC_CHIP > l_proc = plat_getChipTarget();
        // Fetch the MASTER_CORE attribute
        uint8_t l_coreId = 0;
        FAPI_ATTR_GET(fapi2::ATTR_MASTER_CORE, l_proc, l_coreId);
        // Construct the Master Core Target
        fapi2::Target<fapi2::TARGET_TYPE_CORE > l_core(
            plat_getTargetHandleByChipletNumber<fapi2::TARGET_TYPE_CORE>(
            l_coreId + CORE_CHIPLET_OFFSET));
        fapi2::Target<fapi2::TARGET_TYPE_EQ> l_quad =
                                l_core.getParent<fapi2::TARGET_TYPE_EQ>();
        SBE_EXEC_HWP(l_rc, i_hwp.quadPoweroffHwp, l_quad, G_ring_save)
    }
    SBE_EXIT(SBE_FUNC);
    return l_rc;
    #undef SBE_FUNC
}

//----------------------------------------------------------------------------
ReturnCode istepWithProcQuiesceLQASet( sbeIstepHwp_t i_hwp )
{
    #define SBE_FUNC "istepWithProcQuiesceLQASet"
    SBE_ENTER(SBE_FUNC);
    ReturnCode l_rc = FAPI2_RC_SUCCESS;
    do
    {
        l_rc = istepWithProc(i_hwp);
        if(l_rc == FAPI2_RC_SUCCESS)
        {
            //set the LQA Bit
            // TODO RTC 164425 - Create another istep for Setting LQA bit after
            // L2/L3 flush istep
            Target<TARGET_TYPE_PROC_CHIP > l_proc = plat_getChipTarget();
            uint64_t l_data = 0x1000000000000000ULL; //Set bit3
            l_rc = putscom_abs_wrap(&l_proc, PU_SECURITY_SWITCH_REGISTER_SCOM, l_data);
            if(l_rc != FAPI2_RC_SUCCESS)
            {
                SBE_ERROR(SBE_FUNC "PutScom failed for PU_SECURITY_SWITCH_REGISTER_SCOM");
                break;
            }
            l_data = 0;
            l_rc = getscom_abs_wrap (&l_proc, PU_SECURITY_SWITCH_REGISTER_SCOM, &l_data);
            if(l_rc != FAPI2_RC_SUCCESS)
            {
                SBE_ERROR(SBE_FUNC "GetScom failed for PU_SECURITY_SWITCH_REGISTER_SCOM");
                break;
            }
            SBE_INFO(SBE_FUNC "PU_SECURITY_SWITCH_REGISTER_SCOM Data [0x%08X][%08X]",
                (uint32_t)((l_data >> 32) & 0xFFFFFFFF), (uint32_t)(l_data & 0xFFFFFFFF));
        }
    }while(0);
    SBE_EXIT(SBE_FUNC);
    return l_rc;
    #undef SBE_FUNC
}

//----------------------------------------------------------------------------
ReturnCode istepMpiplSetFunctionalState( sbeIstepHwp_t i_hwp )
{
    #define SBE_FUNC "istepMpiplSetFunctionalState"
    SBE_ENTER(SBE_FUNC);
    Target<TARGET_TYPE_PROC_CHIP > proc = plat_getChipTarget();
    ReturnCode rc = FAPI2_RC_SUCCESS;
    do
    {
        // Read the EQ and EC gard attributes from the chip target
        fapi2::buffer<uint64_t> l_scratchReg1 = 0;
        uint64_t l_scratchReg8 = 0;
        static const uint64_t SCRATCH8_SCRATCH1REG_VALID_BIT =
                                                    0x8000000000000000ULL;
        fapi2::buffer<uint8_t> l_eqMask = 0;
        fapi2::buffer<uint32_t> l_ecMask = 0;
        plat_target_handle_t l_hndl;

        // Set MPIPL mode bit in Scratch Reg 3
        (void)SbeRegAccess::theSbeRegAccess().setMpIplMode(true);

        rc = getscom_abs_wrap (&l_hndl,
                               PERV_SCRATCH_REGISTER_8_SCOM,
                               &l_scratchReg8);
        if( rc != FAPI2_RC_SUCCESS )
        {
            SBE_ERROR(SBE_FUNC" Failed to read Scratch RegR8");
            break;
        }
        if(l_scratchReg8 & SCRATCH8_SCRATCH1REG_VALID_BIT)
        {
            rc = getscom_abs_wrap (&l_hndl,
                                   PERV_SCRATCH_REGISTER_1_SCOM,
                                   &l_scratchReg1());
            if( rc != FAPI2_RC_SUCCESS )
            {
                SBE_ERROR(SBE_FUNC" Failed to read Scratch Reg1");
                break;
            }

            l_scratchReg1.extract<0, 6>(l_eqMask);
            l_scratchReg1.extract<8, 24>(l_ecMask);
            SBE_INFO(SBE_FUNC" Setting ATTR_EQ_GARD [0x%08X] "
                             "ATTR_EC_GARD [0x%08X]",
                             l_eqMask, l_ecMask);

            FAPI_ATTR_SET(fapi2::ATTR_EQ_GARD, proc, l_eqMask);
            FAPI_ATTR_SET(fapi2::ATTR_EC_GARD, proc, l_ecMask);

            // Apply the gard records
            rc = plat_ApplyGards();
            if( rc != FAPI2_RC_SUCCESS )
            {
                SBE_ERROR(SBE_FUNC" Failed to to apply gard records");
                break;
            }

            // TODO via RTC 135345
            // Once multicast targets are supported, we may need to pass
            // p9selectex::ALL as input.
            SBE_EXEC_HWP(rc, p9_sbe_select_ex_hwp, proc, p9selectex::SINGLE)
            if( rc != FAPI2_RC_SUCCESS )
            {
                SBE_ERROR(SBE_FUNC" Failed hwp p9_sbe_select_ex_hwp");
                break;
            }
        }
        else
        {
            SBE_ERROR(SBE_FUNC " Scratch Reg 1 is invalid,"
                    "not applying gard records");
        }
     }while(0);
    SBE_EXIT(SBE_FUNC);
    return rc;
    #undef SBE_FUNC
}

//----------------------------------------------------------------------------
ReturnCode performTpmReset()
{
    #define SBE_FUNC "performTpmReset "
    SBE_ENTER(SBE_FUNC);
    ReturnCode rc = FAPI2_RC_SUCCESS;
    do
    {
        constexpr uint64_t tpmBitMask = 0x0008000000000000ULL;
        plat_target_handle_t tgtHndl;
        uint64_t regData = 0;
        rc = getscom_abs_wrap (&tgtHndl,
                               PU_PRV_MISC_PPE,
                               &regData);
        if( rc != FAPI2_RC_SUCCESS )
        {
            SBE_ERROR(SBE_FUNC" Failed to read SBE internal reg for TPM reset");
            break;
        }

        // To do TPM reset, first we should set bit 12 of PU_PRV_MISC_PPE
        // and then clear it up.
        regData = regData | tpmBitMask;
        rc = putscom_abs_wrap(&tgtHndl, PU_PRV_MISC_PPE, regData);
        if( rc != FAPI2_RC_SUCCESS )
        {
            SBE_ERROR(SBE_FUNC" Failed to set TPM mask");
            break;
        }

        regData = regData & ( ~tpmBitMask);
        rc = putscom_abs_wrap(&tgtHndl, PU_PRV_MISC_PPE, regData);
        if( rc != FAPI2_RC_SUCCESS )
        {
            SBE_ERROR(SBE_FUNC" Failed to clear TPM mask");
            break;
        }

    }while(0);
    SBE_EXIT(SBE_FUNC);
    return rc;
    #undef SBE_FUNC
}

//----------------------------------------------------------------------------
ReturnCode istepLpcInit( sbeIstepHwp_t i_hwp)
{
    ReturnCode rc = FAPI2_RC_SUCCESS;
    Target<TARGET_TYPE_PROC_CHIP > proc = plat_getChipTarget();
    assert( NULL != i_hwp.procHwp );
    if( !(SbeRegAccess::theSbeRegAccess().isSbeSlave()) )
    {
        SBE_EXEC_HWP(rc, i_hwp.procHwp,proc);
    }
    return rc;
}

//----------------------------------------------------------------------------
ReturnCode istepStopClockMpipl( sbeIstepHwp_t i_hwp )
{
    #define SBE_FUNC "istepStopClockMpipl"
    SBE_ENTER(SBE_FUNC);
    uint32_t l_fapiRc = FAPI2_RC_SUCCESS;
    p9_stopclocks_flags l_flags; // Default Flag Values
    Target<TARGET_TYPE_PROC_CHIP > l_procTgt = plat_getChipTarget();
    p9hcd::P9_HCD_CLK_CTRL_CONSTANTS l_clk_regions =
                p9hcd::CLK_REGION_ALL_BUT_PLL_REFR;
    p9hcd::P9_HCD_EX_CTRL_CONSTANTS l_ex_select = p9hcd::BOTH_EX;

    l_flags.clearAll();
    l_flags.stop_core_clks = true;
    l_flags.stop_cache_clks = true;

    SBE_EXEC_HWP(l_fapiRc,
                 p9_stopclocks_hwp,
                 l_procTgt,
                 l_flags,
                 l_clk_regions,
                 l_ex_select);

    SBE_EXIT(SBE_FUNC);
    return l_fapiRc;
    #undef SBE_FUNC
}

//----------------------------------------------------------------------------
ReturnCode updatePhbFunctionalState( void )
{
    #define SBE_FUNC "updatePhbFunctionalState"
    SBE_ENTER(SBE_FUNC);
    ReturnCode rc = FAPI2_RC_SUCCESS;
    const uint64_t pci_cplt_conf1[3] = {PEC_0_CPLT_CONF1, PEC_1_CPLT_CONF1, PEC_2_CPLT_CONF1};

    Target<TARGET_TYPE_PROC_CHIP > procTgt = plat_getChipTarget();
    auto phbTgt = procTgt.getChildren<fapi2::TARGET_TYPE_PHB>();

    uint8_t phbCnt = 0;
    for (uint8_t pciCnt=0; pciCnt<3; pciCnt++)
    {
        uint64_t data = 0;
        uint8_t phbPerPciCnt = 0;

        rc = getscom_abs_wrap (&procTgt, pci_cplt_conf1[pciCnt], &data);
        if(rc != FAPI2_RC_SUCCESS)
        {
            SBE_ERROR(SBE_FUNC" Failed to read Pec[%d] Chiplet Config1 register",pciCnt);
            break;
        }

        do
        {
            // Fetch bit4 from D000009 for PHB0
            // Fetch bit4/5 from E000009 for PHB1/2
            // Fetch bit4/5/6 from F000009 for PHB3/4/5
            if( ((data >> (PEC_PHB_IOVALID_BIT_SHIFT - phbPerPciCnt)) &
                        PEC_PHB_IOVALID_BIT_MASK) )
            {
                static_cast<plat_target_handle_t&>(phbTgt[phbCnt++].operator ()()).setFunctional(true);
            }
            else
            {
                static_cast<plat_target_handle_t&>(phbTgt[phbCnt++].operator ()()).setFunctional(false);
            }
        }while(++phbPerPciCnt<=pciCnt);
    }

    SBE_EXIT(SBE_FUNC);
    return rc;
#undef SBE_FUNC
}


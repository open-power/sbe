/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/power/ipl_table.C $                             */
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
#include "core/ipl.H"

#include "sbecmdiplcontrol.H"
#include "sbefifo.H"
#include "sbetrace.H"
#include "sbe_sp_intf.H"
#include "sbeFifoMsgUtils.H"
#include "assert.h"
#include "sberegaccess.H"
#include "sbestates.H"
#include "sbecmdcntrldmt.H"
#include "sbeglobals.H"
// TODO Workaround
#include "plat_target_parms.H"

#include "fapi2.H"

#include "p9_misc_scom_addresses.H"
#include "p10_scom_perv_9.H"

// Pervasive HWP Header Files ( istep 2)
#include <p10_sbe_attr_setup.H>
#include <p10_sbe_tp_chiplet_reset.H>
#include <p10_sbe_tp_gptr_time_initf.H>
#include <p10_sbe_dft_probe_setup_1.H>
#include <p10_sbe_npll_initf.H>
#include <p10_sbe_rcs_setup.H>
#include <p10_sbe_tp_switch_gears.H>
#include <p10_sbe_npll_setup.H>
#include <p10_sbe_tp_repr_initf.H>
#include <p10_sbe_tp_abist_setup.H>
#include <p10_sbe_tp_arrayinit.H>
#include <p10_sbe_tp_initf.H>
#include <p10_sbe_dft_probe_setup_2.H>
#include <p10_sbe_tp_chiplet_init.H>

// Pervasive HWP Header Files ( istep 3)
#include <p10_sbe_chiplet_setup.H>
#include <p10_sbe_chiplet_clk_config.H>
#include <p10_sbe_chiplet_reset.H>
#include <p10_sbe_gptr_time_initf.H>
#include <p10_sbe_chiplet_pll_initf.H>
#include <p10_sbe_chiplet_pll_setup.H>
#include <p10_sbe_repr_initf.H>
#include <p10_sbe_arrayinit.H>
#include <p10_sbe_initf.H>
#include <p10_sbe_startclocks.H>
#include <p10_sbe_chiplet_init.H>
#include <p10_sbe_nest_enable_ridi.H>
#include <p10_sbe_scominit.H>
#include <p10_sbe_lpc_init.H>
#include <p10_sbe_fabricinit.H>
#include <p10_sbe_mcs_setup.H>
#include <p10_sbe_select_ex.H>
#include <p10_sbe_dts_init.H>
#include <p10_sbe_chiplet_fir_init.H>

// Core Cache HWP header files (istep 4)
#include <p10_hcd_cache_poweron.H>
#include <p10_hcd_cache_reset.H>
#include <p10_hcd_cache_gptr_time_initf.H>
#include <p10_hcd_cache_repair_initf.H>
#include <p10_hcd_cache_arrayinit.H>
#include <p10_hcd_cache_initf.H>
#include <p10_hcd_cache_startclocks.H>
#include <p10_hcd_cache_scominit.H>
#include <p10_hcd_cache_scom_customize.H>
#include <p10_hcd_cache_ras_runtime_scom.H>
#include <p10_hcd_cache_ras_runtime_scom.H>
//Core Procedures.
#include <p10_hcd_core_poweron.H>
#include <p10_hcd_core_reset.H>
#include <p10_hcd_core_repair_initf.H>
#include <p10_hcd_core_arrayinit.H>
#include <p10_hcd_core_initf.H>
#include <p10_hcd_core_startclocks.H>
#include <p10_hcd_core_scominit.H>
#include <p10_hcd_core_scom_customize.H>
#include <p10_hcd_core_ras_runtime_scom.H>
#include <p10_hcd_core_gptr_time_initf.H>

// istep 5 hwp header files
#include "p10_sbe_instruct_start.H"
#include "p10_sbe_core_spr_setup.H"
#include "p10_sbe_load_bootloader.H"

// istep mpipl header files
#include "p10_query_corecachemma_access_state.H"
#include "p10_sbe_check_quiesce.H"
#include "p10_l2_flush.H"
#include "p10_l3_flush.H"
//#include "p9_sbe_sequence_drtm.H" //This will be enabled later on
#include "p10_thread_control.H"
#include "sbecmdcntlinst.H"
#include "p10_hcd_mma_poweroff.H"
#include "p10_hcd_core_poweroff.H"
#include "p10_hcd_cache_poweroff.H"
#include "p10_stopclocks.H"
#include "p10_suspend_powman.H"

#include "sbeXipUtils.H" // For getting hbbl offset
#include "sbeutil.H" // For getting SBE_TO_NEST_FREQ_FACTOR

#include "p10_fbc_utils.H"
#include "sbeSecureMemRegionManager.H"

#include "sbeConsole.H"
#include "sbecmdflushnvdimm.H"

// Forward declaration
using namespace fapi2;

// constants
static const uint32_t SBE_ROLE_MASK = 0x00000002;
static const uint32_t SBE_SYSTEM_QUIESCE_TIMEOUT_LOOP = 20000;

static const uint64_t SBE_LQA_DELAY_HW_US = 1000000ULL; // 1ms
static const uint64_t SBE_LQA_DELAY_SIM_CYCLES = 0x1ULL;

// Bit-33 used to checkstop the system, Since this is directly getting inserted
// will have to use bit (63-33) = 30th bit
static const uint64_t  N3_FIR_SYSTEM_CHECKSTOP_BIT = 30; // 63-33 = 30

// Externs
extern uint64_t G_ring_save[8];
//Utility function to mask special attention
extern ReturnCode maskSpecialAttn( const Target<TARGET_TYPE_CORE>& i_target );

// Aliases
using sbeIstepHwpTpSwitchGears_t = ReturnCode (*)
                    (const Target<TARGET_TYPE_PROC_CHIP> & i_target);

using  sbeIstepHwpProc_t = ReturnCode (*)
                    (const Target<TARGET_TYPE_PROC_CHIP> & i_target);
extern p10_thread_control_FP_t threadCntlhwp;
#if 0
using  sbeIstepHwpEq_t = ReturnCode (*)
                    (const Target<TARGET_TYPE_EQ> & i_target);
#endif
using sbeIstepHwpCore_t =  ReturnCode (*)
                    (const Target<TARGET_TYPE_CORE> & i_target);

using sbeIstepHwpMCCore_t =  ReturnCode (*)
                    (const Target<fapi2::TARGET_TYPE_CORE | fapi2::TARGET_TYPE_MULTICAST, fapi2::MULTICAST_AND > & i_target);

using sbeIstepHwpMCORCore_t =  ReturnCode (*)
                    (const Target<fapi2::TARGET_TYPE_CORE | fapi2::TARGET_TYPE_MULTICAST> & i_target);

using sbeIstepHwpStartCore_t =  ReturnCode (*)
                    (const Target<TARGET_TYPE_CORE> & i_target,
                     const ThreadSpecifier i_thread_mask);

using  sbeIstepHwpCoreL2Flush_t = ReturnCode (*)
                    (const Target<TARGET_TYPE_CORE> & i_target,
                     const p10core::purgeData_t & i_purgeData);

using  sbeIstepHwpCoreL3Flush_t = ReturnCode (*)
                    (const Target<TARGET_TYPE_CORE> & i_target,
                     const uint32_t i_purgeType,
                     const uint32_t i_purgeAddr);
 
#if 0
using sbeIstepHwpCoreBlockIntr_t =  ReturnCode (*)
                    (const Target<TARGET_TYPE_CORE> & i_target,
                     const p9pmblockwkup::OP_TYPE i_oper);

using  sbeIstepHwpCoreScomState_t = ReturnCode (*)
                    (const Target<TARGET_TYPE_CORE> & i_target,
                     bool & o_isScom,
                     bool & o_isScan);

using  sbeIstepHwpSequenceDrtm_t = ReturnCode (*)
                    (const Target<TARGET_TYPE_PROC_CHIP> & i_target,
                     uint8_t & o_status);

using  sbeIstepHwpQuadPoweroff_t = ReturnCode (*)
                    (const Target<TARGET_TYPE_EQ> & i_target,
                     uint64_t * o_ring_save_data);

using  sbeIstepHwpCacheInitf_t = ReturnCode (*)
                    (const Target<TARGET_TYPE_EQ> & i_target,
                    const uint64_t * i_ring_save_data);
#endif
// Forward declarations
// Wrapper function which will call HWP.
ReturnCode istepWithProc( voidfuncptr_t i_hwp );
ReturnCode istepLpcInit( voidfuncptr_t i_hwp );
ReturnCode istepHwpTpSwitchGears( voidfuncptr_t i_hwp);
ReturnCode istepAttrSetup( voidfuncptr_t i_hwp );
ReturnCode istepNoOp( voidfuncptr_t i_hwp );
ReturnCode istepWithEq( voidfuncptr_t i_hwp);
ReturnCode istepWithCore( voidfuncptr_t i_hwp);
ReturnCode istepWithCoreStart( voidfuncptr_t i_hwp);
ReturnCode istepWithGoodEqMCCore( voidfuncptr_t i_hwp);
ReturnCode istepWithGoodEqMCORCore( voidfuncptr_t i_hwp);
ReturnCode istepSelectEx( voidfuncptr_t i_hwp);
ReturnCode istepLoadBootLoader( voidfuncptr_t i_hwp);
ReturnCode istepCheckSbeMaster( voidfuncptr_t i_hwp);
ReturnCode istepStartInstruction( voidfuncptr_t i_hwp);
ReturnCode istepWithCoreConditional( voidfuncptr_t i_hwp);
ReturnCode istepWithEqConditional( voidfuncptr_t i_hwp);
ReturnCode istepNestFreq( voidfuncptr_t i_hwp);
ReturnCode istepCacheInitf( voidfuncptr_t i_hwp );

//MPIPL Specific
ReturnCode istepWithCoreSetBlock( voidfuncptr_t i_hwp );
ReturnCode istepWithCoreState( voidfuncptr_t i_hwp );
//ReturnCode istepMpiplRstClrTpmBits( voidfuncptr_t i_hwp );//TODO::RTC:253772
ReturnCode istepWithProcQuiesceLQASet( voidfuncptr_t i_hwp );
ReturnCode istepWithCoreL2Flush( voidfuncptr_t i_hwp );
ReturnCode istepWithCoreL3Flush( voidfuncptr_t i_hwp );
ReturnCode istepStartMpipl( voidfuncptr_t i_hwp );
ReturnCode istepWithProcSequenceDrtm( voidfuncptr_t i_hwp );
ReturnCode istepMpiplSetFunctionalState( voidfuncptr_t i_hwp );
ReturnCode istepMpiplQuadPoweroff( voidfuncptr_t i_hwp );
ReturnCode istepStopClockMpipl( voidfuncptr_t i_hwp );

#ifndef __SBEFW_SEEPROM__
/*
 * --------------------------------------------- start PIBMEM CODE
 */
static istepMap_t g_istepMpiplStartPtrTbl[] =
        {
#ifdef SEEPROM_IMAGE
            // Place holder for StartMpipl, State Change, PHB State Update,
            // Clear CRESP error latch register.
            // Set MPIPL mode in Sratch Reg 3
            ISTEP_MAP( istepStartMpipl, NULL ),
            // Call suspend powerman
            ISTEP_MAP( istepWithProc, p10_suspend_powman),
            // Find all the child cores within proc and call hwp to know the
            // scom state and call instruction control. Also mask spl attention
            // from core.
            ISTEP_MAP( istepWithCoreState, NULL ),
            //  Reset the TPM is not done as separate istep. It will be done
            //  as part of measurement seeprom boot flow.
            ISTEP_MAP( istepNoOp, NULL ),
            // quiesce state for all units on the powerbus on its chip
            ISTEP_MAP( istepWithProcQuiesceLQASet, p10_sbe_check_quiesce ),
            // L2 cache flush via purge engine on each EX
            ISTEP_MAP( istepWithCoreL2Flush, p10_l2_flush ),
            // L3 cache flush via purge engine on each EX
            ISTEP_MAP( istepWithCoreL3Flush, p10_l3_flush ),
            // Check on Quiescing of all Chips in a System by Local SBE
            ISTEP_MAP( istepWithProcSequenceDrtm, NULL ),
#endif
        };
static istepMap_t g_istepMpiplContinuePtrTbl[] =
        {
#ifdef SEEPROM_IMAGE
            ISTEP_MAP( istepLpcInit, p10_sbe_lpc_init ),
            // Setup EC/EQ guard records
            ISTEP_MAP( istepMpiplSetFunctionalState, p10_sbe_select_ex),
            ISTEP_MAP( istepNoOp, NULL ),  // Witherspoon only (mpipl_dump_reg)
            ISTEP_MAP( istepNoOp, NULL ),  // Witherspoon only (mpipl_query_quad_access_state)
            ISTEP_MAP( istepNoOp, NULL ),  // Witherspoon only (mpipl_hcd_core_stopclocks)
            ISTEP_MAP( istepNoOp, NULL ),  // Witherspoon only (mpipl_hcd_cache_stopclocks)
            //istepMpiplQuadPoweroff internally calls 3 procedures
            //P10_hcd_mma_poweroff,p10_hcd_core_poweroff and p10_hcd_cache_poweroff
            ISTEP_MAP( istepMpiplQuadPoweroff, NULL),
            // No-op
            ISTEP_MAP( istepNoOp, NULL ),
#endif
        };

static istepMap_t g_istepStopClockPtrTbl[] =
        {
#ifdef SEEPROM_IMAGE
            // Stop Clock Mpipl
            ISTEP_MAP( istepStopClockMpipl, NULL),
#endif
        };

// File static data
static istepMap_t g_istep2PtrTbl[] =
         {
#ifdef SEEPROM_IMAGE
             ISTEP_MAP( NULL, NULL ),
             ISTEP_MAP( istepAttrSetup, p10_sbe_attr_setup),
             ISTEP_MAP( istepNoOp, NULL), //p10_sbe_tp_dpll_bypass
             ISTEP_MAP( istepWithProc, p10_sbe_tp_chiplet_reset),
             ISTEP_MAP( istepWithProc, p10_sbe_tp_gptr_time_initf),
             ISTEP_MAP( istepNoOp, p10_sbe_dft_probe_setup_1),// DFT only
             ISTEP_MAP( istepWithProc, p10_sbe_npll_initf),
             ISTEP_MAP( istepWithProc, p10_sbe_rcs_setup),
             ISTEP_MAP( istepHwpTpSwitchGears, p10_sbe_tp_switch_gears),
             ISTEP_MAP( istepNestFreq, p10_sbe_npll_setup),
             ISTEP_MAP( istepWithProc, p10_sbe_tp_repr_initf),
             ISTEP_MAP( istepWithProc, p10_sbe_tp_abist_setup),
             ISTEP_MAP( istepWithProc, p10_sbe_tp_arrayinit),
             ISTEP_MAP( istepWithProc, p10_sbe_tp_initf),
             ISTEP_MAP( istepNoOp, p10_sbe_dft_probe_setup_2), //DFT only
             ISTEP_MAP( istepWithProc, p10_sbe_tp_chiplet_init),
#endif
         };

static istepMap_t g_istep3PtrTbl[] =
         {
#ifdef SEEPROM_IMAGE
             ISTEP_MAP( istepWithProc, p10_sbe_chiplet_setup),
             ISTEP_MAP( istepWithProc, p10_sbe_chiplet_clk_config),
             ISTEP_MAP( istepWithProc, p10_sbe_chiplet_reset),
             ISTEP_MAP( istepWithProc, p10_sbe_gptr_time_initf),
             ISTEP_MAP( istepWithProc, p10_sbe_chiplet_pll_initf),
             ISTEP_MAP( istepWithProc, p10_sbe_chiplet_pll_setup),
             ISTEP_MAP( istepWithProc, p10_sbe_repr_initf),
             ISTEP_MAP( istepNoOp, NULL ), // DFT - Hook for DFT to run ABIST
             ISTEP_MAP( istepWithProc, p10_sbe_arrayinit), // Chiplet array init
             ISTEP_MAP( istepNoOp, NULL ), // DFT - Hook for DFT to run LBIST
             ISTEP_MAP( istepWithProc, p10_sbe_initf),
             ISTEP_MAP( istepWithProc, p10_sbe_startclocks),
             ISTEP_MAP( istepWithProc, p10_sbe_chiplet_init),
             ISTEP_MAP( istepNoOp, p10_sbe_chiplet_fir_init),
             ISTEP_MAP( istepWithProc, p10_sbe_dts_init), //p10_sbe_dts_init
             ISTEP_MAP( istepNoOp, NULL), //p10_sbe_skew_adjust_setup
             ISTEP_MAP( istepWithProc, p10_sbe_nest_enable_ridi),
             ISTEP_MAP( istepWithProc, p10_sbe_scominit),
             ISTEP_MAP( istepLpcInit,  p10_sbe_lpc_init),
             ISTEP_MAP( istepWithProc, p10_sbe_fabricinit),
             ISTEP_MAP( istepCheckSbeMaster, NULL),
             ISTEP_MAP( istepWithProc, p10_sbe_mcs_setup),
             ISTEP_MAP( istepSelectEx, p10_sbe_select_ex),
#endif
         };
static istepMap_t g_istep4PtrTbl[] =
         {
#ifdef SEEPROM_IMAGE
             ISTEP_MAP( istepWithGoodEqMCCore, p10_hcd_cache_poweron),
             ISTEP_MAP( istepWithGoodEqMCCore, p10_hcd_cache_reset),
             ISTEP_MAP( istepWithGoodEqMCORCore, p10_hcd_cache_gptr_time_initf),
             ISTEP_MAP( istepWithGoodEqMCORCore, p10_hcd_cache_repair_initf),
             ISTEP_MAP( istepWithGoodEqMCCore, p10_hcd_cache_arrayinit),
             ISTEP_MAP( istepWithGoodEqMCORCore, p10_hcd_cache_initf),
             ISTEP_MAP( istepWithGoodEqMCCore, p10_hcd_cache_startclocks),
             ISTEP_MAP( istepWithGoodEqMCORCore, p10_hcd_cache_scominit),
             ISTEP_MAP( istepWithGoodEqMCORCore, p10_hcd_cache_scom_customize),
             ISTEP_MAP( istepWithGoodEqMCORCore, p10_hcd_cache_ras_runtime_scom),
             ISTEP_MAP( istepWithGoodEqMCCore, p10_hcd_core_poweron),
             ISTEP_MAP( istepWithGoodEqMCCore, p10_hcd_core_reset),
             ISTEP_MAP( istepWithGoodEqMCORCore, p10_hcd_core_gptr_time_initf), //p10_hcd_core_gptr_time_initf 
             ISTEP_MAP( istepWithGoodEqMCORCore, p10_hcd_core_repair_initf ),
             ISTEP_MAP( istepWithGoodEqMCCore, p10_hcd_core_arrayinit),
             ISTEP_MAP( istepWithGoodEqMCORCore, p10_hcd_core_initf),
             ISTEP_MAP( istepWithGoodEqMCCore, p10_hcd_core_startclocks),
             ISTEP_MAP( istepWithGoodEqMCORCore, p10_hcd_core_scominit),
             ISTEP_MAP( istepWithGoodEqMCORCore, p10_hcd_core_scom_customize),
             ISTEP_MAP( istepWithGoodEqMCORCore, p10_hcd_core_ras_runtime_scom),
#endif
         };

// TODO via RTC 135345
//  Add the support for istep 5 HWP
static istepMap_t g_istep5PtrTbl[]
         {
             ISTEP_MAP( istepLoadBootLoader, NULL ),
             ISTEP_MAP( istepWithProc, p10_sbe_core_spr_setup ),
             ISTEP_MAP( istepStartInstruction, p10_sbe_instruct_start),
         };

istepTableEntry_t istepTableEntries[] = {
    ISTEP_ENTRY(  2, g_istep2PtrTbl),
    ISTEP_ENTRY(  3, g_istep3PtrTbl),
    ISTEP_ENTRY(  4, g_istep4PtrTbl),
    ISTEP_ENTRY(  5, g_istep5PtrTbl),
    ISTEP_ENTRY( 96, g_istepMpiplStartPtrTbl),
    ISTEP_ENTRY( 97, g_istepMpiplContinuePtrTbl),
    ISTEP_ENTRY( 98, g_istepStopClockPtrTbl),
};

REGISTER_ISTEP_TABLE(istepTableEntries)

//----------------------------------------------------------------------------

ReturnCode istepAttrSetup( voidfuncptr_t i_hwp)
{
    return performAttrSetup();
}
//----------------------------------------------------------------------------

ReturnCode istepWithProc( voidfuncptr_t i_hwp)
{
    ReturnCode rc = FAPI2_RC_SUCCESS;
    Target<TARGET_TYPE_PROC_CHIP > proc = plat_getChipTarget();
    assert( NULL != i_hwp );
    SBE_EXEC_HWP(rc, reinterpret_cast<sbeIstepHwpProc_t>( i_hwp ), proc)
    return rc;
}
//----------------------------------------------------------------------------

ReturnCode istepLpcInit( voidfuncptr_t i_hwp)
{
    ReturnCode rc = FAPI2_RC_SUCCESS;
    Target<TARGET_TYPE_PROC_CHIP > proc = plat_getChipTarget();
    assert( NULL != i_hwp );
    SBE_EXEC_HWP(rc, reinterpret_cast<sbeIstepHwpProc_t>( i_hwp ), proc)
    
    g_sbeRole = SbeRegAccess::theSbeRegAccess().isSbeMaster() ?
                    SBE_ROLE_MASTER : SBE_ROLE_SLAVE;
    if(SBE_ROLE_MASTER == g_sbeRole)
    {
        SBE_UART_INIT;
        SBE_MSG_CONSOLE( SBE_CONSOLE_WELCOME_MSG );
    }
    else
    {
        SBE_INFO(SBE_FUNC "Skipping UART_INIT on slave chip");
    }
    return rc;
}
//----------------------------------------------------------------------------

ReturnCode istepHwpTpSwitchGears( voidfuncptr_t i_hwp)
{
    ReturnCode rc = FAPI2_RC_SUCCESS;
    Target<TARGET_TYPE_PROC_CHIP > proc = plat_getChipTarget();
    assert( NULL != i_hwp );
    SBE_EXEC_HWP(rc, reinterpret_cast<sbeIstepHwpTpSwitchGears_t>( i_hwp ), proc)

    // backup i2c mode register
    uint32_t reg_address = PU_MODE_REGISTER_B;
    PPE_LVD( reg_address, SBE_GLOBAL->i2cModeRegister);
    return rc;
}

//----------------------------------------------------------------------------

ReturnCode istepNestFreq( voidfuncptr_t i_hwp)
{
    #define SBE_FUNC "istepNestFreq "
    ReturnCode rc = FAPI2_RC_SUCCESS;

    Target<TARGET_TYPE_PROC_CHIP > proc = plat_getChipTarget();
    assert( NULL != i_hwp );
    do
    {
        SBE_EXEC_HWP(rc, reinterpret_cast<sbeIstepHwpProc_t>( i_hwp ),proc)
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

ReturnCode istepSelectEx( voidfuncptr_t i_hwp)
{
    ReturnCode rc = FAPI2_RC_SUCCESS;
    Target<TARGET_TYPE_PROC_CHIP > proc = plat_getChipTarget();
    // TODO via RTC 135345
    // Once multicast targets are supported, we may need to pass
    // p9selectex::ALL as input.
    SBE_EXEC_HWP(rc, reinterpret_cast<p10_sbe_select_ex_FP_t>(i_hwp), proc, selectex::SINGLE)
    return rc;
}

//----------------------------------------------------------------------------
ReturnCode istepWithEq( voidfuncptr_t i_hwp)
{
    ReturnCode rc = FAPI2_RC_SUCCESS;
#if 0
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

    assert( NULL != i_hwp);
    //SBE_EXEC_HWP(rc, reinterpret_cast<sbeIstepHwpEq_t>(i_hwp), eqTgt )
#endif
    return rc;
}

//----------------------------------------------------------------------------
ReturnCode istepCacheInitf (voidfuncptr_t i_hwp )
{
    #define SBE_FUNC "istepCacheInitf"
    SBE_ENTER(SBE_FUNC);
    ReturnCode l_rc = FAPI2_RC_SUCCESS;

#if 0
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

    //SBE_EXEC_HWP(l_rc, reinterpret_cast<sbeIstepHwpCacheInitf_t>( i_hwp ), eqTgt, G_ring_save)
#endif
    SBE_EXIT(SBE_FUNC);
    return l_rc;
    #undef SBE_FUNC
}

//----------------------------------------------------------------------------
ReturnCode istepWithCore( voidfuncptr_t i_hwp)
{
    #define SBE_FUNC "istepWithCore"
    ReturnCode rc = FAPI2_RC_SUCCESS;
    // Get master Core
    uint8_t coreId = 0;
    uint8_t fuseMode = 0;
    Target<TARGET_TYPE_PROC_CHIP > proc = plat_getChipTarget();
    FAPI_ATTR_GET(fapi2::ATTR_MASTER_CORE,proc,coreId);
    FAPI_ATTR_GET(ATTR_FUSED_CORE_MODE, Target<TARGET_TYPE_SYSTEM>(), fuseMode);
    fapi2::Target<fapi2::TARGET_TYPE_CORE >
        coreTgt(plat_getTargetHandleByInstance<fapi2::TARGET_TYPE_CORE>(coreId));
    assert( NULL != i_hwp );

    //for (auto &coreTgt : exTgt.getChildren<fapi2::TARGET_TYPE_CORE>())
    do
    {
        // Core0 is assumed to be the master core
        SBE_EXEC_HWP(rc, reinterpret_cast<sbeIstepHwpCore_t>(i_hwp), coreTgt)
        if(rc != FAPI2_RC_SUCCESS)
        {
            SBE_ERROR(SBE_FUNC " istepWithCore failed, RC=[0x%08X]", rc);
            break;
        }
        // Only continue in case of istep4 && fuse core mode
        if(!( (fuseMode) &&
              (SbeRegAccess::theSbeRegAccess().getSbeMajorIstepNumber() ==
                                                            4) ) )
        {
            break;
        }
    }while(0);
    return rc;
    #undef SBE_FUNC
}

//----------------------------------------------------------------------------
ReturnCode istepWithCoreStart( voidfuncptr_t i_hwp)
{
    #define SBE_FUNC "istepWithCoreStart"
    ReturnCode rc = FAPI2_RC_SUCCESS;
    // Get master Core
    uint8_t coreId = 0;

    uint8_t fuseMode = 0;
    Target<TARGET_TYPE_PROC_CHIP > proc = plat_getChipTarget();
    FAPI_ATTR_GET(fapi2::ATTR_MASTER_CORE,proc,coreId);
    FAPI_ATTR_GET(ATTR_FUSED_CORE_MODE, Target<TARGET_TYPE_SYSTEM>(), fuseMode);
    fapi2::Target<fapi2::TARGET_TYPE_CORE >
        coreTgt(plat_getTargetHandleByInstance<fapi2::TARGET_TYPE_CORE>(coreId));
    assert( NULL != i_hwp );

    do
    {
        // Core0 is assumed to be the master core
       ThreadSpecifier l_thread = THREAD0;
       SBE_EXEC_HWP(rc, reinterpret_cast<sbeIstepHwpStartCore_t>(i_hwp), coreTgt, l_thread)
        if(rc != FAPI2_RC_SUCCESS)
        {
            SBE_ERROR(SBE_FUNC " istepWithCoreStart failed, RC=[0x%08X]", rc);
            break;
        }
        // Only continue in case of istep4 && fuse core mode
        if(!( (fuseMode) &&
              (SbeRegAccess::theSbeRegAccess().getSbeMajorIstepNumber() ==
                                                            4) ) )
        {
            break;
        }
    }while(0);
    return rc;
    #undef SBE_FUNC
}
//----------------------------------------------------------------------------
ReturnCode istepWithGoodEqMCORCore( voidfuncptr_t i_hwp)
{
    #define SBE_FUNC "istepWithGoodEqMCORCore"
    ReturnCode rc = FAPI2_RC_SUCCESS;
    Target<TARGET_TYPE_PROC_CHIP > proc = plat_getChipTarget();
    fapi2::Target < fapi2::TARGET_TYPE_CORE | fapi2::TARGET_TYPE_MULTICAST > mc_cores;
    mc_cores = proc.getMulticast(fapi2::MCGROUP_GOOD_EQ, fapi2::MCCORE_ALL);
    FAPI_DBG("MultiCast Code target for group MCGROUP_GOOD_EQ Created=0x%.8x",mc_cores.get());
    do
    {
        SBE_EXEC_HWP(rc, reinterpret_cast<sbeIstepHwpMCORCore_t>(i_hwp),mc_cores)
        if(rc != FAPI2_RC_SUCCESS)
        {
            SBE_ERROR(SBE_FUNC " istepWithGoodEqMCORCore failed, RC=[0x%08X]", rc);
            break;
        }

    }while(0);
    return rc;
    #undef SBE_FUNC
}

ReturnCode istepWithGoodEqMCCore( voidfuncptr_t i_hwp)
{
    #define SBE_FUNC "istepWithGoodEqMCCore"
    ReturnCode rc = FAPI2_RC_SUCCESS;
    Target<TARGET_TYPE_PROC_CHIP > proc = plat_getChipTarget();
    fapi2::Target < fapi2::TARGET_TYPE_CORE | fapi2::TARGET_TYPE_MULTICAST,
                     fapi2::MULTICAST_AND > mc_cores;
    mc_cores = proc.getMulticast<fapi2::MULTICAST_AND>(fapi2::MCGROUP_GOOD_EQ, fapi2::MCCORE_ALL);
    FAPI_DBG("MultiCast Code target for group MCGROUP_GOOD_EQ Created=0x%.8x",mc_cores.get());
    do
    {
        SBE_EXEC_HWP(rc, reinterpret_cast<sbeIstepHwpMCCore_t>(i_hwp),mc_cores)
        if(rc != FAPI2_RC_SUCCESS)
        {
            SBE_ERROR(SBE_FUNC " istepWithGoodEqMCCore failed, RC=[0x%08X]", rc);
            break;
        }

    }while(0);
    return rc;
    #undef SBE_FUNC
}

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
ReturnCode istepWithEqConditional( voidfuncptr_t i_hwp)
{
    SBE_ENTER("istepWithEqCondtional");
    ReturnCode rc = FAPI2_RC_SUCCESS;
#if 0
    fapi2::Target<fapi2::TARGET_TYPE_SYSTEM > sysTgt;
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
#endif
    SBE_EXIT("istepWithEqCondtional");
    return rc;
}
//----------------------------------------------------------------------------

ReturnCode istepWithCoreConditional( voidfuncptr_t i_hwp)
{
    SBE_ENTER("istepWithCoreCondtional");
    ReturnCode rc = FAPI2_RC_SUCCESS;
#if 0
    fapi2::Target<fapi2::TARGET_TYPE_SYSTEM > sysTgt;
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
#endif
    SBE_EXIT("istepWithCoreCondtional");
    return rc;
}

//----------------------------------------------------------------------------
ReturnCode istepNoOp( voidfuncptr_t i_hwp)
{
    SBE_INFO("istepNoOp");
    return FAPI2_RC_SUCCESS ;
}

/*
 * end PIBMEMCODE -----------------------------------------------
 */
#endif // #ifndef __SBEFW_SEEPROM__

#ifdef __SBEFW_SEEPROM__
//----------------------------------------------------------------------------
constexpr uint32_t HB_MEM_WINDOW_SIZE = 8*1024*1024; //8 MB
ReturnCode istepLoadBootLoader( voidfuncptr_t i_hwp)
{
    ReturnCode rc = FAPI2_RC_SUCCESS;

    // Get master Core
    uint8_t coreId = 0;
    uint8_t l_is_mpipl = 0;
    Target< TARGET_TYPE_SYSTEM > sysTgt;
    Target<TARGET_TYPE_PROC_CHIP > proc = plat_getChipTarget();
    FAPI_ATTR_GET(fapi2::ATTR_MASTER_CORE,proc,coreId);
    fapi2::Target<fapi2::TARGET_TYPE_CORE >
        coreTgt(plat_getTargetHandleByInstance<fapi2::TARGET_TYPE_CORE>(coreId));
    // Get hbbl section
    P9XipHeader *hdr = getXipHdr();
    P9XipSection *hbblSection =  &(hdr->iv_section[P9_XIP_SECTION_SBE_HBBL]);

    uint64_t drawer_base_address_nm0, drawer_base_address_nm1;
    uint64_t drawer_base_address_m;
    uint64_t drawer_base_address_mmio;
    uint64_t l_hostboot_hrmor_offset;
    do
    {
        // Update the ATTR_SBE_ADDR_KEY_STASH_ADDR before calling the bootloader,
        // since it is going to access these data from inside.
        uint64_t addr = SBE_GLOBAL->sbeKeyAddrPair.fetchStashAddrAttribute();
        PLAT_ATTR_INIT(fapi2::ATTR_SBE_ADDR_KEY_STASH_ADDR, sysTgt, addr);

        //Initialize attribute ATTR_LPC_CONSOLE_INITIALIZED.
        ATTR_LPC_CONSOLE_INITIALIZED_Type isLpcInitialized = SBE_GLOBAL->sbeUartActive;
        PLAT_ATTR_INIT(fapi2::ATTR_LPC_CONSOLE_INITIALIZED, sysTgt, isLpcInitialized);

        fapi2::ATTR_BACKING_CACHES_NUM_Type l_ATTR_BACKING_CACHES_NUM;

        //Backing cache is of 4MB size.
        //Num of backing cache should be equal or greater than 2 for IPL.

        FAPI_ATTR_GET(fapi2::ATTR_BACKING_CACHES_NUM,
                               proc,
                               l_ATTR_BACKING_CACHES_NUM);
        fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYSTEM;
        FAPI_ATTR_GET(fapi2::ATTR_IS_MPIPL,FAPI_SYSTEM,l_is_mpipl);
        //In the MPIPL path Memory is alive and backing cache is not mandatory.
        //In MPIPL boot loader image is loaded(DMAed) directly into the memory 
        if( (l_ATTR_BACKING_CACHES_NUM < 2) && (!l_is_mpipl) )
        {
           SBE_ERROR(" Num of backing cache is less than 2. Cannot proceed with IPL");
           pk_halt();
        }
        ATTR_NUM_KEY_ADDR_PAIR_Type l_value = MAX_ROW_COUNT;
        PLAT_ATTR_INIT(fapi2::ATTR_NUM_KEY_ADDR_PAIR, sysTgt, l_value);
        SBE_EXEC_HWP(rc, p10_sbe_load_bootloader, proc, coreTgt, hbblSection->iv_size,
                     getSectionAddr(hbblSection))
        if(rc != FAPI2_RC_SUCCESS)
        {
            SBE_ERROR(" p10_sbe_load_bootloader failed");
            break;
        }

        // Open HB Dump memory Region
        FAPI_ATTR_GET(fapi2::ATTR_HOSTBOOT_HRMOR_OFFSET,
                      FAPI_SYSTEM,
                      l_hostboot_hrmor_offset);
        rc = p10_fbc_utils_get_chip_base_address(
                                    proc,
                                    HB_BOOT_ID,
                                    drawer_base_address_nm0,
                                    drawer_base_address_nm1,
                                    drawer_base_address_m,
                                    drawer_base_address_mmio);
        if(rc != FAPI2_RC_SUCCESS)
        {
            SBE_ERROR(" p10_fbc_utils_get_chip_base_address failed");
            break;
        }
        drawer_base_address_nm0 += l_hostboot_hrmor_offset;
        SBE_INFO("istep 5.1 HB Dump mem Region [0x%08X%08X]",
                                SBE::higher32BWord(drawer_base_address_nm0),
                                SBE::lower32BWord(drawer_base_address_nm0));
        mainStoreSecMemRegionManager.add(drawer_base_address_nm0,
                                    HB_MEM_WINDOW_SIZE,
                                    static_cast<uint8_t>(memRegionMode::READ));

    } while(0);

    return rc;
}

//----------------------------------------------------------------------------

ReturnCode istepStartInstruction( voidfuncptr_t i_hwp)
{
    ReturnCode rc = FAPI2_RC_SUCCESS;

    SBE_MSG_CONSOLE("SBE starting hostboot");
    SBE_UART_DISABLE;
    rc = istepWithCoreStart(i_hwp);
    if(rc == FAPI2_RC_SUCCESS)
    {
        (void)SbeRegAccess::theSbeRegAccess().stateTransition(
                SBE_RUNTIME_EVENT);
    }
    return rc;
}

//----------------------------------------------------------------------------
ReturnCode istepCheckSbeMaster( voidfuncptr_t i_hwp)
{
    #define SBE_FUNC "istepCheckSbeMaster "
    ReturnCode rc = FAPI2_RC_SUCCESS;
    
    do
    {
        g_sbeRole = SbeRegAccess::theSbeRegAccess().isSbeMaster() ?
                    SBE_ROLE_MASTER : SBE_ROLE_SLAVE;
        SBE_INFO(SBE_FUNC"Sbe role [%x]", g_sbeRole);
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
ReturnCode istepWithCoreSetBlock( voidfuncptr_t i_hwp)
{
    #define SBE_FUNC "istepWithCoreSetBlock"
    SBE_ENTER(SBE_FUNC);
    ReturnCode l_rc = FAPI2_RC_SUCCESS;
#if 0
    Target<TARGET_TYPE_PROC_CHIP > l_procTgt = plat_getChipTarget();
    for (auto l_coreTgt : l_procTgt.getChildren<fapi2::TARGET_TYPE_CORE>())
    {
        //SBE_EXEC_HWP(l_rc, reinterpret_cast<sbeIstepHwpCoreBlockIntr_t>( i_hwp ), l_coreTgt, p9pmblockwkup::SET)
        if(l_rc != FAPI2_RC_SUCCESS)
        {
            SBE_ERROR(SBE_FUNC " p9_block_wakeup_intr failed, RC=[0x%08X]",
                l_rc);
            break;
        }
    }
#endif
    SBE_EXIT(SBE_FUNC);
    return l_rc;
    #undef SBE_FUNC
}

//----------------------------------------------------------------------------
ReturnCode istepWithCoreState( voidfuncptr_t i_hwp)
{
    #define SBE_FUNC "istepWithCoreState"
    SBE_ENTER(SBE_FUNC);
    ReturnCode l_rc = FAPI2_RC_SUCCESS;

    l_rc = stopAllCoreInstructions();
    if( l_rc == FAPI2_RC_SUCCESS )
    {
        //TODO:RTC:253772, Enable flushNVDIMM() support
        /*
        l_rc = flushNVDIMM();
        if( l_rc != FAPI2_RC_SUCCESS )
        {
            SBE_ERROR(SBE_FUNC "flushNVDIMM failed");
        }*/
    }
    else
    {
        SBE_ERROR(SBE_FUNC "Stop all core instructions is failed, "
                 "RC=[0x%08X]", l_rc);
    }

    SBE_EXIT(SBE_FUNC);
    return l_rc;
    #undef SBE_FUNC
}

//TODO:RTC:253772, Cleanup
#if 0
//----------------------------------------------------------------------------
ReturnCode istepMpiplRstClrTpmBits( voidfuncptr_t i_hwp)
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
#endif

//----------------------------------------------------------------------------
ReturnCode istepWithCoreL2Flush( voidfuncptr_t i_hwp)
{
    #define SBE_FUNC "istepWithCoreL2Flush"
    SBE_ENTER(SBE_FUNC);
    ReturnCode fapiRc = FAPI2_RC_SUCCESS;
    Target<TARGET_TYPE_PROC_CHIP > procTgt = plat_getChipTarget();
    //Fetch all EQ Targets
    for(auto eqTgt: procTgt.getChildren<fapi2::TARGET_TYPE_EQ>())
    {
        //Default Set the bits represeting the L2 SCOM state as TRUE
        scomStatus_t scomStateData;
        scanStatus_t scanStateData;
        SBE_EXEC_HWP(fapiRc,p10_query_corecachemma_access_state, eqTgt,
                     scomStateData,scanStateData);
        if(fapiRc != FAPI2_RC_SUCCESS)
        {
            SBE_ERROR(SBE_FUNC " p10_query_corecachemma_access_state failed, "
                    " RC=[0x%08X]", fapiRc);
            break;
        }

        //For all Core targets associated with the EQ target,call procedure to
        //purge L2 if scomStateData for the particular core is TRUE
        for (auto coreTgt : eqTgt.getChildren<fapi2::TARGET_TYPE_CORE>())
        {
            if(isCoreOrL2CacheScomEnabled(coreTgt,scomStateData))
            {
                p10core::purgeData_t purgeData;
                SBE_EXEC_HWP(fapiRc,
                             reinterpret_cast<sbeIstepHwpCoreL2Flush_t>(i_hwp),
                             coreTgt,
                             purgeData)
                if(fapiRc != FAPI2_RC_SUCCESS)
                {
                    SBE_ERROR(SBE_FUNC " p10_l2_flush failed, RC=[0x%08X]", fapiRc);
                    break;
                }

            }//End of SCOM state check
        }//End of Core loop
        if(fapiRc != FAPI2_RC_SUCCESS)
        {
            break;
        }
    }//End of Eq loop
    SBE_EXIT(SBE_FUNC);
    return fapiRc;
    #undef SBE_FUNC
}

//----------------------------------------------------------------------------
ReturnCode istepWithCoreL3Flush( voidfuncptr_t i_hwp)
{
    #define SBE_FUNC "istepWithCoreL3Flush"
    SBE_ENTER(SBE_FUNC);
    ReturnCode fapiRc = FAPI2_RC_SUCCESS;
    Target<TARGET_TYPE_PROC_CHIP > procTgt = plat_getChipTarget();
    //Fetch all EQ Targets
    for(auto eqTgt: procTgt.getChildren<fapi2::TARGET_TYPE_EQ>())
    {
        scomStatus_t scomStateData;
        scanStatus_t scanStateData;
        SBE_EXEC_HWP(fapiRc,p10_query_corecachemma_access_state, eqTgt,
                     scomStateData,scanStateData);
        if(fapiRc != FAPI2_RC_SUCCESS)
        {
            SBE_ERROR(SBE_FUNC " p10_query_corecachemma_access_state failed, "
                               " RC=[0x%08X]", fapiRc);
            break;
        }

        //For all Core targets associated with the EQ target,call procedure to
        //purge L3 if scomStateData for the particular core is TRUE
        for (auto coreTgt : eqTgt.getChildren<fapi2::TARGET_TYPE_CORE>())
        {
            if(isL3CacheScomEnabled(coreTgt,scomStateData))
            {
                p10core::purgeData_t l_purgeData;
                SBE_EXEC_HWP(fapiRc,
                             reinterpret_cast<sbeIstepHwpCoreL3Flush_t>(i_hwp),
                             coreTgt,L3_FULL_PURGE,0x0);
                if(fapiRc != FAPI2_RC_SUCCESS)
                {
                    SBE_ERROR(SBE_FUNC " p10_l3_flush failed, RC=[0x%08X]", fapiRc);
                    break;
                }

            }//End of SCOM state check
        }//End of Core loop
        if(fapiRc != FAPI2_RC_SUCCESS)
        {
            break;
        }
    }//End of Eq loop

    SBE_EXIT(SBE_FUNC);
    return fapiRc;
    #undef SBE_FUNC
}

//----------------------------------------------------------------------------
ReturnCode istepWithProcSequenceDrtm( voidfuncptr_t i_hwp)
{
    #define SBE_FUNC "istepWithProcSequenceDrtm"
    SBE_ENTER(SBE_FUNC);
    ReturnCode l_rc = FAPI2_RC_SUCCESS;
#if 0
        Target<TARGET_TYPE_PROC_CHIP > l_procTgt = plat_getChipTarget();

    uint8_t l_status = 0;
    size_t l_timeOut = SBE_SYSTEM_QUIESCE_TIMEOUT_LOOP;
     while(l_timeOut)
    {
        //SBE_EXEC_HWP(l_rc, reinterpret_cast<sbeIstepHwpSequenceDrtm_t>(i_hwp), l_procTgt, l_status)
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
#endif
    SBE_EXIT(SBE_FUNC);
    return l_rc;
    #undef SBE_FUNC
}

//----------------------------------------------------------------------------
ReturnCode istepStartMpipl( voidfuncptr_t i_hwp)
{
    #define SBE_FUNC "istepStartMpipl"
    SBE_ENTER(SBE_FUNC);
    ReturnCode rc = FAPI2_RC_SUCCESS;

    (void)SbeRegAccess::theSbeRegAccess().stateTransition(
            SBE_ENTER_MPIPL_EVENT);
    // Set MPIPL mode bit in Scratch Reg 3
    (void)SbeRegAccess::theSbeRegAccess().setMpIplMode(true);
    // Close all non-secure memory regions
    mainStoreSecMemRegionManager.closeAllRegions();

    do
    {
        rc = updatePhbFunctionalState();
        if(rc != FAPI2_RC_SUCCESS)
        {
            SBE_ERROR(SBE_FUNC "updatePhbFunctionalState failed");
            break;
        }
        rc = resetCrespErrLatch();
        if(rc != FAPI2_RC_SUCCESS)
        {
            SBE_ERROR(SBE_FUNC "resetCrespErrLatch failed");
            break;
        }

    }while(0);

    SBE_EXIT(SBE_FUNC);
    return rc;
    #undef SBE_FUNC
}

//----------------------------------------------------------------------------
ReturnCode istepMpiplQuadPoweroff( voidfuncptr_t i_hwp)
{
#define SBE_FUNC "istepMpiplQuadPoweroff"
    SBE_ENTER(SBE_FUNC);
    ReturnCode fapiRc = FAPI2_RC_SUCCESS;
    if(g_sbeRole == SBE_ROLE_MASTER)
    {
        Target<TARGET_TYPE_PROC_CHIP > proc = plat_getChipTarget();
        fapi2::Target < fapi2::TARGET_TYPE_CORE | fapi2::TARGET_TYPE_MULTICAST,
                      fapi2::MULTICAST_AND > mc_cores;
        mc_cores = proc.getMulticast<fapi2::MULTICAST_AND>(fapi2::MCGROUP_GOOD_EQ, fapi2::MCCORE_ALL);
        SBE_DEBUG("MultiCast Code target for group MCGROUP_GOOD_EQ Created=0x%.8x",mc_cores.get());
        do
        {
            SBE_EXEC_HWP(fapiRc, p10_hcd_mma_poweroff,mc_cores)
            if(fapiRc != FAPI2_RC_SUCCESS)
            {
                SBE_ERROR(SBE_FUNC "p10_hcd_mma_poweroff() failed, fapiRc=[0x%08X]", fapiRc);
                break;
            }
            SBE_EXEC_HWP(fapiRc, p10_hcd_core_poweroff,mc_cores)
            if(fapiRc != FAPI2_RC_SUCCESS)
            {
                SBE_ERROR(SBE_FUNC "p10_hcd_core_poweroff() failed, fapiRc=[0x%08X]", fapiRc);
                break;
            }
            SBE_EXEC_HWP(fapiRc, p10_hcd_cache_poweroff,mc_cores)
            if(fapiRc != FAPI2_RC_SUCCESS)
            {
                SBE_ERROR(SBE_FUNC "p10_hcd_cache_poweroff() failed, fapiRc=[0x%08X]", fapiRc);
                break;
            }
        }while(0);
    }
    SBE_EXIT(SBE_FUNC);
    return fapiRc;
#undef SBE_FUNC
}

//----------------------------------------------------------------------------
ReturnCode istepWithProcQuiesceLQASet( voidfuncptr_t i_hwp )
{
    #define SBE_FUNC "istepWithProcQuiesceLQASet"
    SBE_ENTER(SBE_FUNC);
    ReturnCode l_rc = FAPI2_RC_SUCCESS;

    do
    {
        l_rc = istepWithProc(i_hwp);
        if(l_rc == FAPI2_RC_SUCCESS)
        {
#if 0
            //TODO:Re-enabel this with update P10 addresses and enable
            //setting of the LQA Bit
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
#endif         
        }
    }while(0);

    SBE_EXIT(SBE_FUNC);
    return l_rc;
    #undef SBE_FUNC
}

//----------------------------------------------------------------------------
ReturnCode istepMpiplSetFunctionalState( voidfuncptr_t i_hwp )
{
    #define SBE_FUNC "istepMpiplSetFunctionalState"
    SBE_ENTER(SBE_FUNC);
    ReturnCode rc = FAPI2_RC_SUCCESS;
    Target<TARGET_TYPE_PROC_CHIP > proc = plat_getChipTarget();
    do
    {
        // Read the EQ and EC gard attributes from the chip target
        fapi2::buffer<uint64_t> l_scratchReg1 = 0;
        uint64_t l_scratchReg8 = 0;
        //TODO:Update this bit information post analysis
        static const uint64_t SCRATCH8_SCRATCH1REG_VALID_BIT =
                                                    0x8000000000000000ULL;
        fapi2::buffer<uint8_t> l_eqMask = 0;
        fapi2::buffer<uint32_t> l_ecMask = 0;
        plat_target_handle_t l_hndl;

        // Set MPIPL mode bit in Scratch Reg 3
        (void)SbeRegAccess::theSbeRegAccess().setMpIplMode(true);

        rc = getscom_abs_wrap (&l_hndl,
                               scomt::perv::FSXCOMP_FSXLOG_SCRATCH_REGISTER_8_RW ,
                               &l_scratchReg8);
        if( rc != FAPI2_RC_SUCCESS )
        {
            SBE_ERROR(SBE_FUNC" Failed to read Scratch RegR8, rc=0x%.8x",rc);
            break;
        }
        if(l_scratchReg8 & SCRATCH8_SCRATCH1REG_VALID_BIT)
        {
            //TODO:Revisit the below logic to apply EQ and CORE gard
            //records.
#if 0
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

            PLAT_ATTR_INIT(fapi2::ATTR_EQ_GARD, proc, l_eqMask);
            PLAT_ATTR_INIT(fapi2::ATTR_EC_GARD, proc, l_ecMask);

            // Apply the gard records
            rc = plat_ApplyGards();
            if( rc != FAPI2_RC_SUCCESS )
            {
                SBE_ERROR(SBE_FUNC" Failed to to apply gard records");
                break;
            }
#endif

            if (g_sbeRole == SBE_ROLE_MASTER)
            {
                SBE_EXEC_HWP(rc, reinterpret_cast<p10_sbe_select_ex_FP_t>(i_hwp), proc, selectex::SINGLE)
                if( rc != FAPI2_RC_SUCCESS )
                {
                    SBE_ERROR(SBE_FUNC" Failed hwp p10_sbe_select_ex_hwp");
                    break;
                }
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
ReturnCode istepStopClockMpipl( voidfuncptr_t i_hwp )
{
    #define SBE_FUNC "istepStopClockMpipl"
    SBE_ENTER(SBE_FUNC);
    uint32_t fapiRc = FAPI2_RC_SUCCESS;
    p10_stopclocks_flags flags;
    flags.clearAll();
    flags.stop_core_clks = true;
    flags.stop_cache_clks = true;
    Target<TARGET_TYPE_PROC_CHIP > l_procTgt = plat_getChipTarget();
    SBE_EXEC_HWP(fapiRc, reinterpret_cast<p10_stopclocks_FP_t>(i_hwp),
                 l_procTgt, flags);
    if(fapiRc != FAPI2_RC_SUCCESS)
    {
        SBE_ERROR(SBE_FUNC" Failed p10_stopclocks() procedure, RC=0x%.8x",
                  fapiRc);
    }
    SBE_EXIT(SBE_FUNC);
    return fapiRc;
    #undef SBE_FUNC
}

/*
 * end SEEPROM CODE --------------------------------------------------
 */
#endif // ifdef __SBEFW_SEEPROM__

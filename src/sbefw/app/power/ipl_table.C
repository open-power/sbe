/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/power/ipl_table.C $                             */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2017,2022                        */
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
#include "sbearchregdump.H"
#include "sbecmdmpipl.H"
// TODO Workaround
#include "plat_target_parms.H"

#include "fapi2.H"

#include "p9_misc_scom_addresses.H"
#include "p10_scom_perv_9.H"
#include "p10_scom_proc_9.H"
#include "p10_scom_proc_2.H"
#include "p10_scom_proc_d.H"
// Pervasive HWP Header Files ( istep 2)
#include <p10_sbe_attr_setup.H>
#include <p10_sbe_tp_dpll_bypass.H>
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
#include <p10_sbe_check_master.H>

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
#include "p10_sbe_sync_quiesce_states.H"
#include "p10_thread_control.H"
#include "sbecmdcntlinst.H"
#include "p10_hcd_ecl2_l3_poweroff.H"
#include "p10_stopclocks.H"
#include "p10_suspend_powman.H"
#include "p10_cl2_l3_cleanup.H"

#include "sbeXipUtils.H" // For getting hbbl offset
#include "sbeutil.H" // For getting SBE_TO_NEST_FREQ_FACTOR

#include "p10_scom_pibms.H"
#include "p10_fbc_utils.H"
#include "p10_sbe_scratch_regs.H"
#include "p10_scom_perv_d.H"
#include "sbeSecureMemRegionManager.H"

#include "sbeConsole.H"
#include "sbecmdflushnvdimm.H"

// Forward declaration
using namespace fapi2;

// constants
static const uint32_t SBE_ROLE_MASK = 0x00000002;

// Bit-33 used to checkstop the system, Since this is directly getting inserted
// will have to use bit (63-33) = 30th bit
static const uint64_t  N3_FIR_SYSTEM_CHECKSTOP_BIT = 30; // 63-33 = 30

// Externs
extern uint64_t G_ring_save[8];
//Utility function to mask special attention
extern ReturnCode maskUnmaskSpecialAttn( const Target<TARGET_TYPE_EQ>& i_target, bool isMaskReq );

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
using  sbeIstepHwpCoreCleanup_t = ReturnCode (*)
                     (const Target<TARGET_TYPE_PROC_CHIP> & i_target);
using  sbeIstepHwpSyncQuiesceState_t = ReturnCode (*)
                    (const Target<TARGET_TYPE_PROC_CHIP> & i_target,
                     uint8_t & o_status);

using  sbeIstepHwpQuadPoweroff_t = ReturnCode (*)
                   ( const fapi2::Target < fapi2::TARGET_TYPE_CORE |
                    fapi2::TARGET_TYPE_MULTICAST,fapi2::MULTICAST_AND > &);

// Forward declarations
// Wrapper function which will call HWP.
ReturnCode istepWithProc( voidfuncptr_t i_hwp );
ReturnCode istepLpcInit( voidfuncptr_t i_hwp );
ReturnCode istepHwpTpSwitchGears( voidfuncptr_t i_hwp);
ReturnCode istepHwpPauBypass( voidfuncptr_t i_hwp);
ReturnCode istepSpiScreen( voidfuncptr_t i_hwp );
ReturnCode istepAttrSetup( voidfuncptr_t i_hwp );
ReturnCode istepNoOp( voidfuncptr_t i_hwp );
ReturnCode istepCollectArcRegData(voidfuncptr_t i_hwp);
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
ReturnCode istepWithCoreCLeanup( voidfuncptr_t i_hwp );
ReturnCode istepStartMpipl( voidfuncptr_t i_hwp );
ReturnCode istepWithProcSyncQuiesceState( voidfuncptr_t i_hwp );
ReturnCode istepMpiplSetFunctionalState( voidfuncptr_t i_hwp );
ReturnCode istepMpiplQuadPoweroff( voidfuncptr_t i_hwp );
ReturnCode istepStopClockMpipl( voidfuncptr_t i_hwp );

//Function to unlock M SPI.
ReturnCode unlockMSPI();

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
            // Collect Architected Register data
            ISTEP_MAP( istepCollectArcRegData, NULL ),
            // L2 cache flush via purge engine on each EX
            ISTEP_MAP( istepWithCoreL2Flush, p10_l2_flush ),
            // L3 cache flush via purge engine on each EX
            ISTEP_MAP( istepWithCoreL3Flush, p10_l3_flush ),
            // quiesce state for all units on the powerbus on its chip
            ISTEP_MAP( istepWithProcQuiesceLQASet, p10_sbe_check_quiesce ),
            // Check on Quiescing of all Chips in a System by Local SBE
            ISTEP_MAP( istepWithProcSyncQuiesceState, p10_sbe_sync_quiesce_states ),
            // Run core,L2 nd L3 cleanup procedures
            ISTEP_MAP( istepWithCoreCLeanup, p10_cl2_l3_cleanup),
#endif
        };
static istepMap_t g_istepMpiplContinuePtrTbl[] =
        {
#ifdef SEEPROM_IMAGE
            ISTEP_MAP( istepLpcInit, p10_sbe_lpc_init ),
            ISTEP_MAP( istepMpiplQuadPoweroff, p10_hcd_ecl2_l3_poweroff),
            ISTEP_MAP( istepNoOp, NULL ),  // Witherspoon only (mpipl_dump_reg)
            ISTEP_MAP( istepNoOp, NULL ),  // Witherspoon only (mpipl_query_quad_access_state)
            ISTEP_MAP( istepNoOp, NULL ),  // Witherspoon only (mpipl_hcd_core_stopclocks)
            ISTEP_MAP( istepNoOp, NULL ),  // Witherspoon only (mpipl_hcd_cache_stopclocks)
            // Setup EC/EQ guard records
            ISTEP_MAP( istepMpiplSetFunctionalState, p10_sbe_select_ex),
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
             ISTEP_MAP( istepHwpPauBypass, p10_sbe_tp_dpll_bypass),
             ISTEP_MAP( istepWithProc, p10_sbe_tp_chiplet_reset),
             ISTEP_MAP( istepWithProc, p10_sbe_tp_gptr_time_initf),
             ISTEP_MAP( istepNoOp, p10_sbe_dft_probe_setup_1),// DFT only
             ISTEP_MAP( istepWithProc, p10_sbe_npll_initf),
             ISTEP_MAP( istepWithProc, p10_sbe_rcs_setup),
             ISTEP_MAP( istepHwpTpSwitchGears, p10_sbe_tp_switch_gears),
             ISTEP_MAP( istepNestFreq, p10_sbe_npll_setup),
             ISTEP_MAP( istepWithProc, p10_sbe_tp_repr_initf),
             ISTEP_MAP( istepSpiScreen, NULL),
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
             ISTEP_MAP( istepWithProc, p10_sbe_chiplet_fir_init),
             ISTEP_MAP( istepWithProc, p10_sbe_dts_init), //p10_sbe_dts_init
             ISTEP_MAP( istepNoOp, NULL), //p10_sbe_skew_adjust_setup
             ISTEP_MAP( istepWithProc, p10_sbe_nest_enable_ridi),
             ISTEP_MAP( istepWithProc, p10_sbe_scominit),
             ISTEP_MAP( istepLpcInit,  p10_sbe_lpc_init),
             ISTEP_MAP( istepWithProc, p10_sbe_fabricinit),
             ISTEP_MAP( istepCheckSbeMaster, p10_sbe_check_master),
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
ReturnCode unlockMSPI()
{
    ReturnCode rc = FAPI2_RC_SUCCESS;
    #define SBE_FUNC "unlockMSPI "
    do
    {
        // Unlock the Measurement SPI with which SBE has booted.
        // Get the Side of the M Seeprom with which SBE has booted.
        Target<TARGET_TYPE_PROC_CHIP > proc = plat_getChipTarget();
        uint64_t data = 0;
        rc = getscom_abs_wrap (&proc, MAILBOX_CBS_SELFBOOT_CTRL_STATUS, &data);
        if(rc != FAPI2_RC_SUCCESS)
        {
            SBE_ERROR(SBE_FUNC " GetScom failed for  MAILBOX_CBS_SELFBOOT_CTRL_STATUS(0x50008) with rc 0x%08X", rc);
            break;
        }
        SBE_DEBUG(SBE_FUNC " MAILBOX_CBS_SELFBOOT_CTRL_STATUS is 0x%08X %08X", SBE::higher32BWord(data), SBE::lower32BWord(data));
        // Get Bit 18 for measurement side.
        bool measSide = (data & 0x200000000000) >> 45;
        SBE_INFO(SBE_FUNC " Measurement side is 0x%02X", measSide);

        // Get the SPI config register.
        data = 0;
        uint64_t spimReg = measSide?SPIMST2_CONFIG:SPIMST3_CONFIG;
        SBE_INFO(SBE_FUNC "spimReg is 0x%08X %08X", SBE::higher32BWord(spimReg), SBE::lower32BWord(spimReg));
        rc = getscom_abs_wrap (&proc, spimReg, &data);
        if(rc != FAPI2_RC_SUCCESS)
        {
            SBE_ERROR( SBE_FUNC " Getscom failed for SPIM config reg with rc 0x%08X", rc);
            break;
        }
        SBE_DEBUG(SBE_FUNC " SPIM config reg is 0x%08X with value 0x%08X %08X",
                  SBE::lower32BWord(spimReg), SBE::higher32BWord(data), SBE::lower32BWord(data));
        data = data & 0x07FFFFFFFFFFFFFF;
        SBE_DEBUG(SBE_FUNC " Data to be written is 0x%08X %08X", SBE::higher32BWord(data), SBE::lower32BWord(data));

        // Set the SPI config register.
        rc = putscom_abs_wrap(&proc, spimReg, data);
        if(rc != FAPI2_RC_SUCCESS)
        {
            SBE_ERROR(SBE_FUNC " PutScom failed for SPIM config reg with rc 0x%08X", rc);
            break;
        }
    }while(0);
    return rc;
    #undef SBE_FUNC
}

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

ReturnCode istepHwpPauBypass( voidfuncptr_t i_hwp)
{
    #define SBE_FUNC "istepHwpPauBypass "
    ReturnCode rc = FAPI2_RC_SUCCESS;
    Target<TARGET_TYPE_PROC_CHIP > proc = plat_getChipTarget();
    assert( NULL != i_hwp );
    do
    {
        SBE_EXEC_HWP(rc, reinterpret_cast<sbeIstepHwpProc_t>( i_hwp ), proc)
        if( rc != FAPI2_RC_SUCCESS )
        {
            break;
        }
        // Update PK frequency
        SBE::updatePkFreqToRefClk();
    }while(0);
    return rc;
    #undef SBE_FUNC
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
        SBE::updatePkFreqToPauDpll();
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

//----------------------------------------------------------------------------
ReturnCode istepCollectArcRegData( voidfuncptr_t i_hwp)
{
    SBE_ENTER("istepCollectArcRegData");
    ReturnCode fapiRc = FAPI2_RC_SUCCESS;
    if (!SbeRegAccess::theSbeRegAccess().isSkipMpiplDump())
    {
        fapiRc = sbeDumpArchRegs();
        if( fapiRc != FAPI2_RC_SUCCESS )
        {
            SBE_ERROR(SBE_FUNC "Internal Failure detected in sbeDumpArchRegs()"
            "MPIPL will still be continued!");
            fapiRc = FAPI2_RC_SUCCESS;
        }
    }
    else
    {
        SBE_INFO("Caller disabled collection of Architected data via Bit15(0x5003A)");
    }
    SBE_EXIT("istepCollectArcRegData");
    return fapiRc;
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
    P9XipHeader *bSeepromHdr = getXipHdr();
    P9XipHeader *mSeepromHdr = getMSeepromXipHdr();
    P9XipSection *sbSettingMSection = &(mSeepromHdr->iv_section[P9_XIP_SECTION_SBE_SB_SETTINGS]);
    P9XipSection *hbblSection =  &(bSeepromHdr->iv_section[P9_XIP_SECTION_SBE_HBBL]);
    P9XipSection *sbSettingBSection =  &(bSeepromHdr->iv_section[P9_XIP_SECTION_SBE_SB_SETTINGS]);
    uint8_t *bSeepromSbSettingPtr = (uint8_t*)(sbSettingBSection->iv_offset + g_headerAddr);
    uint32_t *mSeepromSbSettingPtr = (uint32_t*)(sbSettingMSection->iv_offset + g_mseeprom_headerAddr);

    uint64_t drawer_base_address_nm0, drawer_base_address_nm1;
    uint64_t drawer_base_address_m;
    uint64_t drawer_base_address_mmio;
    uint64_t l_hostboot_hrmor_offset;
    uint32_t mSeepromVersion = *mSeepromSbSettingPtr;
    uint8_t  minimumSecureVersion = *(uint8_t*)(bSeepromSbSettingPtr + 64*sizeof(uint8_t));
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
        ATTR_NUM_KEY_ADDR_PAIR_Type keyAddrPair = MAX_ROW_COUNT;
        PLAT_ATTR_INIT(fapi2::ATTR_NUM_KEY_ADDR_PAIR, sysTgt, keyAddrPair);

        SBE_INFO(SBE_FUNC "seepromVersion = 0x%08X", mSeepromVersion);
        fapi2::ATTR_SBE_MEASUREMENT_SEEPROM_VERSION_Type seepromVersion = mSeepromVersion;
        PLAT_ATTR_INIT(fapi2::ATTR_SBE_MEASUREMENT_SEEPROM_VERSION, sysTgt, seepromVersion);

        SBE_INFO(SBE_FUNC "minimum secure version = 0x%02X", minimumSecureVersion);
        fapi2::ATTR_SBE_MINIMUM_SECURE_VERSION_Type secureVersion = minimumSecureVersion;
        PLAT_ATTR_INIT(fapi2::ATTR_SBE_MINIMUM_SECURE_VERSION, sysTgt, secureVersion);

        fapi2::ATTR_SBE_HW_KEY_HASH_ADDR_Type hashKeyAddr = reinterpret_cast<uint64_t>(getSectionAddr(sbSettingBSection));
        PLAT_ATTR_INIT(fapi2::ATTR_SBE_HW_KEY_HASH_ADDR, sysTgt, hashKeyAddr);

        SBE_EXEC_HWP(rc, p10_sbe_load_bootloader, proc, coreTgt, hbblSection->iv_size, getSectionAddr(hbblSection))
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
        rc = unlockMSPI();
        if(rc != FAPI2_RC_SUCCESS)
        {
            SBE_ERROR("Failed to unlock the SPI with rc 0x%08X", rc);
            break;
        }

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
        rc = istepWithProc(i_hwp);
        if(rc != FAPI2_RC_SUCCESS)
        {
            SBE_ERROR("Failed in p10_sbe_check_master():rc=0x%.8x",rc);
            break;
        }
        g_sbeRole = SbeRegAccess::theSbeRegAccess().isSbeMaster() ?
                    SBE_ROLE_MASTER : SBE_ROLE_SLAVE;
        SBE_INFO(SBE_FUNC"Sbe role [%x]", g_sbeRole);
        if(SBE_ROLE_SLAVE == g_sbeRole)
        {
            (void)SbeRegAccess::theSbeRegAccess().stateTransition(
                                            SBE_RUNTIME_EVENT);
            rc = unlockMSPI();
            if(rc != FAPI2_RC_SUCCESS)
            {
                SBE_ERROR("Failed to unlock the SPI with rc 0x%08X", rc);
                break;
            }
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
ReturnCode istepWithCoreCLeanup( voidfuncptr_t i_hwp)
{
#define SBE_FUNC "istepWithCoreCLeanup"
    SBE_ENTER(SBE_FUNC);
    ReturnCode fapiRc = FAPI2_RC_SUCCESS;
    Target<TARGET_TYPE_PROC_CHIP > procTgt = plat_getChipTarget();
    SBE_EXEC_HWP(fapiRc,
            reinterpret_cast<sbeIstepHwpCoreCleanup_t>(i_hwp),
            procTgt);
    if(fapiRc != FAPI2_RC_SUCCESS)
    {
        SBE_ERROR(SBE_FUNC " p10_cl2_l3_cleanup failed, RC=[0x%08X]", fapiRc);
    }


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
ReturnCode istepWithProcSyncQuiesceState( voidfuncptr_t i_hwp)
{
    #define SBE_FUNC "istepWithProcSyncQuiesceState"
    SBE_ENTER(SBE_FUNC);
    ReturnCode l_rc = FAPI2_RC_SUCCESS;

    Target<TARGET_TYPE_PROC_CHIP > l_procTgt = plat_getChipTarget();

    uint8_t l_status = 0;
    size_t l_timeOut = SBE_SYSTEM_QUIESCE_TIMEOUT_LOOP;
    while(l_timeOut)
    {
        SBE_EXEC_HWP(l_rc, reinterpret_cast<sbeIstepHwpSyncQuiesceState_t>(i_hwp), l_procTgt, l_status)
        if(l_rc != FAPI2_RC_SUCCESS)
        {
            SBE_ERROR(SBE_FUNC "p10_sbe_sync_quiesce_states failed, RC=[0x%08X]",l_rc);
            break;
        }
        if(l_status)
        {
            SBE_INFO(SBE_FUNC "p10_sbe_sync_quiesce_states LQA SBE System Quiesce done");
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
        SBE_ERROR(SBE_FUNC "p10_sbe_sync_quiesce_states LQA SBE System Quiesce failed,"
            "Either System Quiesce Achieved not true or procedure "
            "failed RC=[0x%08X]",l_rc);

        SBE_ERROR(SBE_FUNC "Forced the System Checkstop to indicate sync_quiesce_states failure!!");
        //Force System Checkstop
        Target<TARGET_TYPE_PROC_CHIP > l_proc = plat_getChipTarget();
        fapi2::buffer<uint64_t> data(0);
        data.setBit<scomt::proc::TP_TCN1_N1_LOCAL_FIR_IN57>();
        l_rc = putscom_abs_wrap(&l_proc,scomt::proc::TP_TCN1_N1_LOCAL_FIR_WO_OR,
                                data());
        if(l_rc != FAPI2_RC_SUCCESS)
        {
            SBE_ERROR("Failed to force system checkstop to indicate failure in"
            "p10_sbe_sync_quiesce_states HWP");
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
    Target<TARGET_TYPE_PROC_CHIP > proc = plat_getChipTarget();
    fapi2::Target < fapi2::TARGET_TYPE_CORE | fapi2::TARGET_TYPE_MULTICAST,
        fapi2::MULTICAST_AND > mc_cores;
    mc_cores = proc.getMulticast<fapi2::MULTICAST_AND>(fapi2::MCGROUP_GOOD_EQ, fapi2::MCCORE_ALL);
    SBE_DEBUG("MultiCast Code target for group MCGROUP_GOOD_EQ Created=0x%.8x",mc_cores.get());
    do
    {
        SBE_EXEC_HWP(fapiRc, reinterpret_cast<sbeIstepHwpQuadPoweroff_t>(i_hwp),mc_cores)
            if(fapiRc != FAPI2_RC_SUCCESS)
            {
                SBE_ERROR(SBE_FUNC "p10_hcd_ecl2_l3_poweroff() failed, fapiRc=[0x%08X]", fapiRc);
                break;
            }
    }while(0);
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
            // TODO RTC 164425 - Create another istep for Setting LQA bit after
            // L2/L3 flush istep
            Target<TARGET_TYPE_PROC_CHIP > l_proc = plat_getChipTarget();
            uint64_t l_data = 0x1000000000000000ULL; //Set bit3
            //Set bit3(SECURITY_SWITCH_LOCAL_QUIESCE_ACHIEVED) on 0x00010005
            l_rc = putscom_abs_wrap(&l_proc, OTP_SECURITY_SWITCH, l_data);
            if(l_rc != FAPI2_RC_SUCCESS)
            {
                SBE_ERROR(SBE_FUNC "PutScom failed for OTP_SECURITY_SWITCH(0x10005)");
                break;
            }
            l_data = 0;
            l_rc = getscom_abs_wrap (&l_proc, OTP_SECURITY_SWITCH, &l_data);
            if(l_rc != FAPI2_RC_SUCCESS)
            {
                SBE_ERROR(SBE_FUNC "GetScom failed for OTP_SECURITY_SWITCH(0x10005)");
                break;
            }
            SBE_INFO(SBE_FUNC "OTP_SECURITY_SWITCH Data [0x%08X][%08X]",
                (uint32_t)((l_data >> 32) & 0xFFFFFFFF), (uint32_t)(l_data & 0xFFFFFFFF));
        }
    }while(0);

    SBE_EXIT(SBE_FUNC);
    return l_rc;
    #undef SBE_FUNC
}
//----------------------------------------------------------------------------
void setFunctionalStateByCoreNum(uint32_t coreNum)
{
   SBE_INFO("Making %d'th core non-functional", coreNum);
   fapi2::Target<fapi2::TARGET_TYPE_CORE> target = G_vec_targets.at(coreNum + CORE_TARGET_OFFSET);
   static_cast<plat_target_handle_t&>(target.operator ()()).setFunctional(false);
   G_vec_targets.at(coreNum + CORE_TARGET_OFFSET) = target.get();
   SBE_DEBUG("Core target is 0x%08X", G_vec_targets.at(coreNum + CORE_TARGET_OFFSET));
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
        // In MPIPL path, stopCntlisntruction mask the spl mask attn register.
        // In continue mpipl path, unmask the bits in set in mpipl up path explicitly because
        // istep 3 won't be executed so EQ scan inits won't be run again.
	for(auto eqTgt: proc.getChildren<fapi2::TARGET_TYPE_EQ>())
	{
            rc = maskUnmaskSpecialAttn(eqTgt, false);
            if(rc != FAPI2_RC_SUCCESS)
            {
                SBE_ERROR(SBE_FUNC "Failure in unmasking SpecialAttn register with rc 0x%08X", rc);
                break;  // This should not break the continue mpipl.
	    }
	}
	// Read the EC gard attributes from the chip target
	fapi2::buffer<uint64_t> scratchReg1 = 0;
        fapi2::buffer<uint64_t> scratchReg8 = 0;
        fapi2::buffer<uint32_t> ecMask = 0;
        plat_target_handle_t hndl;

        // Set MPIPL mode bit in Scratch Reg 3
        (void)SbeRegAccess::theSbeRegAccess().setMpIplMode(true);
        rc = getscom_abs_wrap (&hndl,
                               scomt::perv::FSXCOMP_FSXLOG_SCRATCH_REGISTER_8_RW ,
                               &scratchReg8());
        if( rc != FAPI2_RC_SUCCESS )
        {
            SBE_ERROR(SBE_FUNC" Failed to read Scratch RegR8, rc=0x%.8x",rc);
            break;
        }
 
        //Read Scratch register and Update the core fuctional states
        if(scratchReg8.getBit<SCRATCH1_REG_VALID_BIT>())
        {
            //Read 0x50038 
            rc = getscom_abs_wrap (&hndl,
                                   scomt::perv::FSXCOMP_FSXLOG_SCRATCH_REGISTER_1_RW,
                                   &scratchReg1());
            if( rc != FAPI2_RC_SUCCESS )
            {
                SBE_ERROR(SBE_FUNC" Failed to read Scratch Reg1");
                break;
            }
            //0..31 bits indicate CORE GARD state for all cores with
            //respect to the PROC chip.
            scratchReg1.extract<0, 32>(ecMask);
            SBE_INFO(SBE_FUNC "Scratch reg 1 is 0x%08X", ecMask);
            fapi2::ATTR_FUSED_CORE_MODE_Type l_fused_core;
            FAPI_ATTR_GET(fapi2::ATTR_FUSED_CORE_MODE,
                             fapi2::Target<fapi2::TARGET_TYPE_SYSTEM>(),
                             l_fused_core);
            SBE_INFO(SBE_FUNC "Fused core mode is 0x%02X", l_fused_core);
            uint32_t shiftCnt = (CORE_TARGET_COUNT - 1);
            for(uint32_t idx = 0; idx < CORE_TARGET_COUNT; idx++)
            {
                FAPI_IMP("Core target count is %d", idx);
                //Pick Core specific bit , If set , then mark the core
                //NON-Functional
                if(l_fused_core)
                {
                   if( ((ecMask >> (shiftCnt-idx) ) & 0x1  ) || ((ecMask >> (shiftCnt-idx - 1))  & 0x1))
                   {
                       // EC chiplet idx is to be marked non-functional
                       for(uint32_t coreCount = 0; coreCount < 2; coreCount++)
                       {
                           setFunctionalStateByCoreNum(idx + coreCount);
                       }
                   }
                   ++idx;
                }
                else
                {
                    if((ecMask >> (shiftCnt-idx) ) & 0x1 )
                    {
                        //Pick Core specific bit , If set , then mark the core
                        //NON-Functional
                        if( (ecMask >> (shiftCnt-idx) ) & 0x1  )
                        {
                            setFunctionalStateByCoreNum(idx);
                        }
                    }
                }
            }
            //If SBE is master call the procedure p10_sbe_select_ex
            if (g_sbeRole == SBE_ROLE_MASTER)
            {
                SBE_EXEC_HWP(rc, reinterpret_cast<p10_sbe_select_ex_FP_t>(i_hwp), proc, selectex::SINGLE)
                if( rc != FAPI2_RC_SUCCESS )
                {
                    SBE_ERROR(SBE_FUNC" Failed hwp p10_sbe_select_ex_hwp");
                    break;
                }
            }
            else
            {
                // Unlock meas SPI in MPIPL path for secondary SBEs.
                rc = unlockMSPI();
                if(rc != FAPI2_RC_SUCCESS)
                {
                    SBE_ERROR("Failed to unlock the SPI with rc 0x%08X", rc);
                    break;
                }
            }
        }
        else
        {
            SBE_ERROR(SBE_FUNC " Scratch Reg 1 is invalid not applying gard records in MPIPL path");
        }
    }while(0);
    SBE_EXIT(SBE_FUNC);
    return rc;
    #undef SBE_FUNC
}

enum P10_ISTEP_2_10_Private_Constants
{
    PLL_LOCK_DELAY_NS = 100000,
    PLL_LOCK_DELAY_CYCLES = 100000,
    PLL_LOCK_DELAY_LOOPS = 10,
};

ReturnCode updateSpiClockRegWithNewPAU(uint32_t freq_pau_mhz)
{
    Target<TARGET_TYPE_PROC_CHIP> procTgt = plat_getChipTarget();
    uint32_t fapiRc = FAPI2_RC_SUCCESS;
    fapi2::buffer<uint64_t> data64;
    uint32_t sck_clock_divider = ((freq_pau_mhz / 40) - 1 ); // freq = pau_freq/4*2*(divider + 1)

    for (uint32_t spiAddr = 0x000C0003; spiAddr <= 0x000C0023; spiAddr += 0x20)
    {
        data64.flush<0>();
        FAPI_TRY(fapi2::getScom(procTgt, spiAddr, data64));
        data64.insertFromRight< 0, 12 >(sck_clock_divider);
        data64.insertFromRight< 12, 8 >(0x80 >> 1);
        FAPI_TRY(fapi2::putScom(procTgt, spiAddr, data64));
    }
fapi_try_exit:
    if(fapi2::current_err)
    {
        fapiRc = fapi2::current_err;
        fapi2::current_err = FAPI2_RC_SUCCESS;
    }
    return fapiRc;
}

ReturnCode pauDpllLockNewFreq(uint32_t freq_pau_mhz, bool & o_lockStatus)
{
    using namespace scomt;
    Target<TARGET_TYPE_PROC_CHIP> procTgt = plat_getChipTarget();
    uint32_t fapiRc = FAPI2_RC_SUCCESS;
    fapi2::buffer<uint64_t> read_reg;
    o_lockStatus = false;
    uint16_t timeout = PLL_LOCK_DELAY_LOOPS;

    uint32_t freq_calculated = (((freq_pau_mhz + 1) * 3) / 50);
    uint64_t NEST_DPLL_INITIALIZE_MODE2 = 0x8001010000000000ULL;

    //uint64_t freq = 0x0970097009700000ULL;
    //PPE_STVD(0x01060052, NEST_DPLL_INITIALIZE_MODE2);
    //PPE_STVD(0x01060051, freq);

    // enable slewing
    FAPI_TRY(fapi2::putScom(procTgt, proc::TP_TPCHIP_TPC_DPLL_CNTL_PAU_REGS_CTRL_RW, NEST_DPLL_INITIALIZE_MODE2));

    // write the new frequency
    read_reg.flush<0>();
    read_reg.insertFromRight < proc::TP_TPCHIP_TPC_DPLL_CNTL_PAU_REGS_FREQ_FMAX,
              proc::TP_TPCHIP_TPC_DPLL_CNTL_PAU_REGS_FREQ_FMAX_LEN > (freq_calculated);
    read_reg.insertFromRight < proc::TP_TPCHIP_TPC_DPLL_CNTL_PAU_REGS_FREQ_FMULT,
              proc::TP_TPCHIP_TPC_DPLL_CNTL_PAU_REGS_FREQ_FMULT_LEN > (freq_calculated);
    read_reg.insertFromRight < proc::TP_TPCHIP_TPC_DPLL_CNTL_PAU_REGS_FREQ_FMIN,
              proc::TP_TPCHIP_TPC_DPLL_CNTL_PAU_REGS_FREQ_FMIN_LEN > (freq_calculated);
    FAPI_TRY(fapi2::putScom(procTgt, proc::TP_TPCHIP_TPC_DPLL_CNTL_PAU_REGS_FREQ, read_reg));

    //poll DPLL_STAT bit 61 (FREQ_CHANGE) until it is zero 
    while (timeout)
    {
        read_reg.flush<0>();
        FAPI_TRY(fapi2::getScom(procTgt, 0x1060055, read_reg)); // DPLL Control Status

        if ((read_reg & 0x9ULL) == 0x9ULL)
        {
            o_lockStatus = true;
            break;
        }
        timeout--;
        fapi2::delay(PLL_LOCK_DELAY_NS, PLL_LOCK_DELAY_CYCLES);
    }
    if(o_lockStatus == false)
    {
        SBE_ERROR("PAU DPLL Lock failed ");
    }

fapi_try_exit:
    if(fapi2::current_err)
    {
        fapiRc = fapi2::current_err;
        fapi2::current_err = FAPI2_RC_SUCCESS;
    }
    return fapiRc;
}

ReturnCode istepSpiScreen( voidfuncptr_t i_hwp )
{
    #define SBE_FUNC "istepSpiScreen"
    SBE_ENTER(SBE_FUNC);
    uint32_t fapiRc = FAPI2_RC_SUCCESS;
    uint32_t attr_freq_pau_mhz;
    bool lockStatus = false;
    bool spiChipSelectIssueFound = false;
    fapi2::buffer<uint64_t> data64;
    mailbox2_cmdhdr_spics_screen_reg0 boot_seeprom_screen_reg;
    uint32_t NEW_PAU_FREQ = 0;
    Target<TARGET_TYPE_PROC_CHIP> procTgt = plat_getChipTarget();
    fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYSTEM;
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_FREQ_PAU_MHZ, FAPI_SYSTEM, attr_freq_pau_mhz));

    SBE_INFO(SBE_FUNC " System Default PAU DPLL is 0x%08X", attr_freq_pau_mhz);

    // Mask the PAU DPLL Error bit18 in register 0x10F001E
    data64.flush<0>();
    FAPI_TRY(fapi2::getScom(procTgt, 0x10F001E, data64)); // TP.TPCHIP.NET.PCBSLPERV.SLAVE_CONFIG_REG
    data64.setBit<18>(); // Bit(12:19) : CFG_MASK_PLL_ERRS(0:7)
    FAPI_TRY(fapi2::putScom(procTgt, 0x10F001E, data64));

    for(uint8_t freqOffset = 1; freqOffset <= 15; freqOffset++)
    {
        NEW_PAU_FREQ = attr_freq_pau_mhz + (freqOffset * 50); // In MHz
        SBE_INFO(SBE_FUNC " Starting PAU Freq Experimentation is [0x%08X][%d] ", NEW_PAU_FREQ, NEW_PAU_FREQ);
        lockStatus = false;
        fapiRc = pauDpllLockNewFreq(NEW_PAU_FREQ, lockStatus);
        if( ! ((fapiRc == FAPI2_RC_SUCCESS) && (lockStatus == true)) )
        {
            SBE_ERROR(SBE_FUNC " PAU DPLL Lock Failed with New Freq[0x%08X][%d] LockStatus[0x%02X] FapiRc[0x%08X]",
                NEW_PAU_FREQ, NEW_PAU_FREQ,lockStatus, fapiRc);
            break;
        }

        // Update SPI Registers to match up to the new PAU DPLL
        // Update clock divider and receive delay
        fapiRc = updateSpiClockRegWithNewPAU(NEW_PAU_FREQ);
        if(fapiRc == FAPI2_RC_SUCCESS)
        {
            //SBE_INFO("SPI Clock Divider Updated for New PAU, Doing Seeprom Reads");
            for(uint32_t cnt=0; cnt<1024; cnt++)
            {
                volatile uint64_t A = *(volatile uint64_t *)(0xFF800000);
                volatile uint64_t B = *(volatile uint64_t *)(0xFF800008);
                volatile uint64_t C = *(volatile uint64_t *)(0xFF800010);
                volatile uint64_t D = *(volatile uint64_t *)(0xFF800018);
                if( (A==0) || (B==0) || (C==0) || (D==0) )
                {
                    SBE_ERROR(SBE_FUNC " $$$$ SPI CS issue reproduced at PAU Freq [0x%08X][%d]", NEW_PAU_FREQ, NEW_PAU_FREQ);
                    spiChipSelectIssueFound = true;
                    //Update mailbox2 Header Commond 0 to let user know that it failed on a PAU Freq
                    PPE_LVD(0x50029, boot_seeprom_screen_reg);
                    boot_seeprom_screen_reg.boot_seeprom_pri_screen_freq = NEW_PAU_FREQ;
                    PPE_STVD(0x50029, boot_seeprom_screen_reg);
                    break;
                }
            }
            if(spiChipSelectIssueFound == true)
            {
                break;
            }
        }
        else
        {
            SBE_ERROR(SBE_FUNC " updateSpiClockRegWithNewPAU failed with fapiRc[0x%8X]", fapiRc);
            break;
        }
    }
    if(spiChipSelectIssueFound == false)
    {
        // Update 0xf, so that user know it is latest screener code.
        PPE_LVD(0x50029, boot_seeprom_screen_reg);
        boot_seeprom_screen_reg.boot_seeprom_pri_screen_freq = NEW_PAU_FREQ; // It should be 2250 + 15*50 = 3000MHz
        PPE_STVD(0x50029, boot_seeprom_screen_reg);
    }

    // Get Back Original PAU Freq Lock
    SBE_INFO(SBE_FUNC " Reverting back to original Pau Freq [0x%08X][%d]", attr_freq_pau_mhz, attr_freq_pau_mhz);
    lockStatus = false;
    pauDpllLockNewFreq(attr_freq_pau_mhz, lockStatus);
    updateSpiClockRegWithNewPAU(attr_freq_pau_mhz);

    // Clear the PAU DPLL Error bit18 Mask in register 0x10F001E
    data64.flush<0>();
    FAPI_TRY(fapi2::getScom(procTgt, 0x10F001E, data64)); // TP.TPCHIP.NET.PCBSLPERV.SLAVE_CONFIG_REG
    data64.clearBit<18>(); // Bit(12:19) : CFG_MASK_PLL_ERRS(0:7)
    FAPI_TRY(fapi2::putScom(procTgt, 0x10F001E, data64));

fapi_try_exit: 
    if(lockStatus == false || fapiRc)
    {
        // Shift back to Original PAU DPLL
        SBE_ERROR(SBE_FUNC " Can't Do much, Something failed, FapiRc[0x%08X] lockStatus[%d]", fapiRc, lockStatus);
    }
    SBE_EXIT(SBE_FUNC);
    return fapiRc;
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

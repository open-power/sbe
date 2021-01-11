/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/power_dft/ipl_table.C $                         */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2017,2021                        */
/* [+] International Business Machines Corp.                              */
/* [+] Justin.Ginn@ibm.com                                                */
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
//#include "p9_perv_scom_addresses_fld.H"
//#include "p9n2_quad_scom_addresses.H"

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
#include <p10_sbe_skew_adjust_setup.H>
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
#include <p10_hcd_core_gptr_time_initf.H>
#include <p10_hcd_core_repair_initf.H>
#include <p10_hcd_core_arrayinit.H>
#include <p10_hcd_core_initf.H>
#include <p10_hcd_core_startclocks.H>
#include <p10_hcd_core_scominit.H>
#include <p10_hcd_core_scom_customize.H>
#include <p10_hcd_core_ras_runtime_scom.H>
#include <p10_hcd_l2_purge.H>
// Core HWP header file
//#include <p9_hcd_core.H>

// Contained procedures
#include <p10_contained.H>
#include <p10_contained_load.H>
#include <p10_contained_ipl.H>
#include <p10_contained_run.H>
#include <p10_contained_dft.H>

// istep mpipl header files
//#include "p9_block_wakeup_intr.H"
//#include "p9_query_core_access_state.H"
//#include "p9_sbe_check_quiesce.H"
//#include "p9_l2_flush.H"
//#include "p9_l3_flush.H"
//#include "p9_sbe_sequence_drtm.H"
#include "p10_thread_control.H"
//#include "sbecmdcntlinst.H"
//#include "p9_quad_power_off.H"
//#include "p9_hcd_cache_stopclocks.H"
//#include "p9_stopclocks.H"
//#include "p9_suspend_powman.H"
//#include "p9_query_cache_access_state.H"

#include "sbeXipUtils.H" // For getting hbbl offset
#include "sbeutil.H" // For getting SBE_TO_NEST_FREQ_FACTOR

#include "p10_fbc_utils.H"
#include "sbeSecureMemRegionManager.H"

#include "sbeConsole.H"
#include "sbecmdflushnvdimm.H"

// p10 stopclocks 
#include "p10_stopclocks.H"
#include "p10_hcd_common.H"
#include "p10_scom_perv_2.H"
#include "p10_scom_perv_3.H"
#include "p10_scom_perv_4.H"
#include "p10_scom_perv_7.H"
#include "p10_scom_perv_a.H"
#include "p10_scom_perv_f.H"
#include <p10_cplt_stopclocks.H>
#include <p10_tp_stopclocks.H>
#include <p10_hcd_core_stopclocks.H>
#include <p10_hcd_cache_stopclocks.H>
#include <p10_hcd_eq_stopclocks.H>
#include <p10_common_stopclocks.H>
#include <p10_hcd_common.H>
#include <multicast_group_defs.H>
#include <target_filters.H>
#include <fapi2_attribute_service.H>

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

#if 0
using  sbeIstepHwpExL2Flush_t = ReturnCode (*)
                    (const Target<TARGET_TYPE_EX> & i_target,
                     const p9core::purgeData_t & i_purgeData);

using  sbeIstepHwpExL3Flush_t = ReturnCode (*)
                    (const Target<TARGET_TYPE_EX> & i_target,
                     const uint32_t i_purgeType,
                     const uint32_t i_purgeAddr);

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

//Contained
ReturnCode istepContainedLoad( voidfuncptr_t i_hwp );
ReturnCode istepContainedRun( voidfuncptr_t i_hwp );

//MPIPL Specific
ReturnCode istepWithCoreSetBlock( voidfuncptr_t i_hwp );
ReturnCode istepWithCoreState( voidfuncptr_t i_hwp );
ReturnCode istepMpiplRstClrTpmBits( voidfuncptr_t i_hwp );
ReturnCode istepWithProcQuiesceLQASet( voidfuncptr_t i_hwp );
ReturnCode istepWithExL2Flush( voidfuncptr_t i_hwp );
ReturnCode istepWithExL3Flush( voidfuncptr_t i_hwp );
ReturnCode istepStartMpipl( voidfuncptr_t i_hwp );
ReturnCode istepWithProcSequenceDrtm( voidfuncptr_t i_hwp );
ReturnCode istepMpiplSetFunctionalState( voidfuncptr_t i_hwp );
ReturnCode istepMpiplQuadPoweroff( voidfuncptr_t i_hwp );
ReturnCode istepStopClockMpipl( voidfuncptr_t i_hwp );

//DFT Specific
ReturnCode istepStopClockDFT( voidfuncptr_t i_hwp );

#ifndef __SBEFW_SEEPROM__
/*
 * --------------------------------------------- start PIBMEM CODE
 */

// File static data
static istepMap_t g_istep2PtrTbl[] =
         {
             ISTEP_MAP( NULL, NULL ), //1
             ISTEP_MAP( istepAttrSetup, p10_sbe_attr_setup), //2
             ISTEP_MAP( istepWithProc, p10_sbe_tp_chiplet_reset), //3
             ISTEP_MAP( istepWithProc    , p10_sbe_tp_gptr_time_initf), //4
             ISTEP_MAP( istepNoOp, p10_sbe_dft_probe_setup_1), //5
             ISTEP_MAP( istepWithProc, p10_sbe_npll_initf), //6
             ISTEP_MAP( istepWithProc, p10_sbe_rcs_setup), //7
             ISTEP_MAP( istepHwpTpSwitchGears, p10_sbe_tp_switch_gears), //8
             ISTEP_MAP( istepNestFreq, p10_sbe_npll_setup), //9
             ISTEP_MAP( istepNoOp, p10_sbe_tp_repr_initf), //10
             ISTEP_MAP( istepNoOp, p10_sbe_tp_abist_setup), //11
             ISTEP_MAP( istepWithProc, p10_sbe_tp_arrayinit), //12
             ISTEP_MAP( istepWithProc, p10_sbe_tp_initf), //13
             ISTEP_MAP( istepNoOp, p10_sbe_dft_probe_setup_2), //14
             ISTEP_MAP( istepWithProc, p10_sbe_tp_chiplet_init), //15
         };

static istepMap_t g_istep3PtrTbl[] =
         {
             ISTEP_MAP( istepWithProc, p10_sbe_chiplet_setup),	//1
             ISTEP_MAP( istepWithProc, p10_sbe_chiplet_clk_config), //2
             ISTEP_MAP( istepWithProc, p10_sbe_chiplet_reset), //3
             ISTEP_MAP( istepNoOp, p10_sbe_gptr_time_initf), //4
             ISTEP_MAP( istepWithProc, p10_sbe_chiplet_pll_initf), //5
             ISTEP_MAP( istepWithProc, p10_sbe_chiplet_pll_setup),  //6
             ISTEP_MAP( istepNoOp, p10_sbe_repr_initf),  //7
             ISTEP_MAP( istepNoOp, NULL), //8
             ISTEP_MAP( istepWithProc, p10_sbe_arrayinit), //9
             ISTEP_MAP( istepNoOp, NULL ), //10
             ISTEP_MAP( istepWithProc, p10_sbe_initf), //11
             ISTEP_MAP( istepWithProc, p10_sbe_startclocks), //12
             ISTEP_MAP( istepWithProc, p10_sbe_chiplet_init), //13
             ISTEP_MAP( istepWithProc, p10_sbe_chiplet_fir_init), //14
             ISTEP_MAP( istepWithProc, p10_sbe_dts_init), //15
             ISTEP_MAP( istepWithProc, p10_sbe_skew_adjust_setup), //16
             ISTEP_MAP( istepWithProc, p10_sbe_nest_enable_ridi), //17
             ISTEP_MAP( istepWithProc, p10_sbe_scominit), //18
             ISTEP_MAP( istepNoOp,  p10_sbe_lpc_init), //19
             ISTEP_MAP( istepWithProc, p10_sbe_fabricinit), //20
             ISTEP_MAP( istepNoOp, NULL), //21
             ISTEP_MAP( istepNoOp, p10_sbe_mcs_setup), //22
             ISTEP_MAP( istepSelectEx, p10_sbe_select_ex), //23
         };
static istepMap_t g_istep4PtrTbl[] =
         {
             ISTEP_MAP( istepWithGoodEqMCCore, p10_hcd_cache_poweron), //1
             ISTEP_MAP( istepWithGoodEqMCCore, p10_hcd_cache_reset), //2
             ISTEP_MAP( istepNoOp, p10_hcd_cache_gptr_time_initf), //3
             ISTEP_MAP( istepNoOp, p10_hcd_cache_repair_initf), //4
             ISTEP_MAP( istepWithGoodEqMCCore, p10_hcd_core_poweron), //5
             ISTEP_MAP( istepWithGoodEqMCCore, p10_hcd_core_reset), //6
             ISTEP_MAP( istepNoOp, p10_hcd_core_gptr_time_initf), // 7
             ISTEP_MAP( istepNoOp, p10_hcd_core_repair_initf ), //8
             ISTEP_MAP( istepWithGoodEqMCCore, p10_hcd_cache_arrayinit), //9
             ISTEP_MAP( istepWithGoodEqMCORCore, p10_hcd_cache_initf), //10
             ISTEP_MAP( istepWithGoodEqMCCore, p10_hcd_cache_startclocks), //11
             ISTEP_MAP( istepWithGoodEqMCORCore, p10_hcd_cache_scominit), //12
             ISTEP_MAP( istepWithGoodEqMCORCore, p10_hcd_cache_scom_customize), //13
             ISTEP_MAP( istepWithGoodEqMCORCore, p10_hcd_cache_ras_runtime_scom), //15
             ISTEP_MAP( istepWithGoodEqMCCore, p10_hcd_core_arrayinit), //15
             ISTEP_MAP( istepWithGoodEqMCORCore, p10_hcd_core_initf), //16
             ISTEP_MAP( istepWithGoodEqMCCore, p10_hcd_core_startclocks), //17
             ISTEP_MAP( istepWithGoodEqMCORCore, p10_hcd_core_scominit), //18
             ISTEP_MAP( istepWithGoodEqMCORCore, p10_hcd_core_scom_customize), //19
             ISTEP_MAP( istepWithGoodEqMCORCore, p10_hcd_core_ras_runtime_scom), //20
             ISTEP_MAP( istepStopClockDFT, p10_stopclocks), //21
         };
static istepMap_t g_istep8PtrTbl[] =
        {
            ISTEP_MAP( istepContainedLoad, p10_contained_load),    //1
            ISTEP_MAP( istepNoOp, p10_contained_dyn_inits), //2
			ISTEP_MAP( istepNoOp, p10_hcd_l2_purge),
            ISTEP_MAP( istepWithProc, p10_contained_save_config), //3
            ISTEP_MAP( istepWithProc, p10_contained_stopclocks), //4
			ISTEP_MAP( istepWithProc, p10_contained_setup), //5
            ISTEP_MAP( istepWithProc, p10_contained_restore), //6
			//NOTE: istep 4 procedures are called in this interim
            ISTEP_MAP( istepContainedRun, p10_contained_run),    //7
        };

istepTableEntry_t istepTableEntries[] = {
    ISTEP_ENTRY(  2, g_istep2PtrTbl),
    ISTEP_ENTRY(  3, g_istep3PtrTbl),
    ISTEP_ENTRY(  4, g_istep4PtrTbl),
    ISTEP_ENTRY(  8, g_istep8PtrTbl),
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
    SBE_UART_INIT;
    SBE_MSG_CONSOLE( SBE_CONSOLE_WELCOME_MSG );
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
    if ( proc.getChildren<fapi2::TARGET_TYPE_CORE>(fapi2::TARGET_STATE_FUNCTIONAL).size() == 0 )
    {
        FAPI_DBG("Skipping proc since no cores are configured");
        return fapi2::FAPI2_RC_SUCCESS;
    }
	SBE_EXEC_HWP(rc, reinterpret_cast<p10_sbe_select_ex_FP_t>(i_hwp), proc, selectex::SINGLE)
    return rc;
}

//----------------------------------------------------------------------------
ReturnCode istepWithEq( voidfuncptr_t i_hwp)
{
    ReturnCode rc = FAPI2_RC_SUCCESS;
//    // TODO via RTC 135345
//    // Curently we are passing Hard code eq target. Finally it is
//    // going to be a multicast target. Once multicast support is
//    // present, use the right target.
//    fapi2::Target<fapi2::TARGET_TYPE_EQ > eqTgt;
//    // Put this in scope so that vector can be freed up before calling hwp.
//    {
//        Target<TARGET_TYPE_PROC_CHIP > proc = plat_getChipTarget();
//        auto eqList = proc.getChildren<fapi2::TARGET_TYPE_EQ>();
//        // As it is workaround lets assume there will always be atleast one
//        // functional eq. No need to validate.
//        eqTgt = eqList[0];
//    }
//
//    assert( NULL != i_hwp);
//    //SBE_EXEC_HWP(rc, reinterpret_cast<sbeIstepHwpEq_t>(i_hwp), eqTgt )
    return rc;
}

//----------------------------------------------------------------------------
ReturnCode istepCacheInitf (voidfuncptr_t i_hwp )
{
    #define SBE_FUNC "istepCacheInitf"
    SBE_ENTER(SBE_FUNC);
    ReturnCode l_rc = FAPI2_RC_SUCCESS;

//    // TODO via RTC 135345
//    fapi2::Target<fapi2::TARGET_TYPE_EQ > eqTgt;
//    // Put this in scope so that vector can be freed up before calling hwp.
//    {
//        Target<TARGET_TYPE_PROC_CHIP > proc = plat_getChipTarget();
//        auto eqList = proc.getChildren<fapi2::TARGET_TYPE_EQ>();
//        // As it is workaround lets assume there will always be atleast one
//        // functional eq. No need to validate.
//        eqTgt = eqList[0];
//    }
//
//    //SBE_EXEC_HWP(l_rc, reinterpret_cast<sbeIstepHwpCacheInitf_t>( i_hwp ), eqTgt, G_ring_save)
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
    fapi2::Target < fapi2::TARGET_TYPE_CORE | fapi2::TARGET_TYPE_MULTICAST > l_mc_cores;
    l_mc_cores = proc.getMulticast(fapi2::MCGROUP_GOOD_EQ, fapi2::MCCORE_ALL);
    FAPI_DBG("MultiCast Code target for group MCGROUP_GOOD_EQ Created=0x%.8x",l_mc_cores.get());
    do
    {
        SBE_EXEC_HWP(rc, reinterpret_cast<sbeIstepHwpMCORCore_t>(i_hwp),l_mc_cores)
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
    if ( proc.getChildren<fapi2::TARGET_TYPE_CORE>(fapi2::TARGET_STATE_FUNCTIONAL).size() == 0 )
    {
        FAPI_DBG("Skipping proc since no cores are configured");
        return fapi2::FAPI2_RC_SUCCESS;
    }
	fapi2::Target < fapi2::TARGET_TYPE_CORE | fapi2::TARGET_TYPE_MULTICAST,
                     fapi2::MULTICAST_AND > l_mc_cores;
    l_mc_cores = proc.getMulticast<fapi2::MULTICAST_AND>(fapi2::MCGROUP_GOOD_EQ, fapi2::MCCORE_ALL);
    FAPI_DBG("MultiCast Code target for group MCGROUP_GOOD_EQ Created=0x%.8x",l_mc_cores.get());
    do
    {
        SBE_EXEC_HWP(rc, reinterpret_cast<sbeIstepHwpMCCore_t>(i_hwp),l_mc_cores)
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
//                    Contained Procedures
//----------------------------------------------------------------------------
ReturnCode istepContainedLoad( voidfuncptr_t i_hwp)
{
    SBE_ENTER("istepContainedLoad");
    ReturnCode rc = FAPI2_RC_SUCCESS;
    Target<TARGET_TYPE_PROC_CHIP > proc = plat_getChipTarget();
    const uint64_t l_cache_img_start_addr = 0;
    const uint64_t l_qme_img_bytes = 0;
    const uint8_t *l_qme_img = 0;

    uint32_t p_cache_img = 0;
    uint64_t l_cache_img_bytes = 0; 

    // Get AVP image pointer from attribute
    FAPI_ATTR_GET(ATTR_AVP_IMG_PTR, proc, p_cache_img);
    //SBE_INFO("Image pointer: [0x%08X]", p_cache_img);
    SBE_INFO("Image pointer: 0x%X", p_cache_img);
    // Get AVP image size from attribute
    FAPI_ATTR_GET(ATTR_AVP_IMG_SIZE, proc, l_cache_img_bytes);
    //SBE_INFO("Image size: [0x%08X]", l_cache_img_bytes);
    SBE_INFO("Image size: 0x%X", l_cache_img_bytes);

    uint8_t* l_cache_img = (uint8_t*) p_cache_img;
    
    SBE_EXEC_HWP(rc, reinterpret_cast<p10_contained_load_FP_t>(i_hwp), proc, l_cache_img_start_addr, l_cache_img_bytes, l_cache_img, l_qme_img_bytes, l_qme_img);

    SBE_EXIT("istepContainedLoad");
    return rc;
}    
//----------------------------------------------------------------------------
ReturnCode istepContainedRun( voidfuncptr_t i_hwp)
{
    SBE_ENTER("istepContainedRun");
    ReturnCode rc=FAPI2_RC_SUCCESS;
    Target<TARGET_TYPE_PROC_CHIP > proc = plat_getChipTarget();

    SBE_EXEC_HWP(rc, reinterpret_cast<p10_contained_run_FP_t>(i_hwp), proc);

    SBE_EXIT("istepContainedRun");
    return rc;
}
//----------------------------------------------------------------------------

ReturnCode istepWithEqConditional( voidfuncptr_t i_hwp)
{
    SBE_ENTER("istepWithEqCondtional");
    ReturnCode rc = FAPI2_RC_SUCCESS;
//    fapi2::Target<fapi2::TARGET_TYPE_SYSTEM > sysTgt;
//    do
//    {
//        uint8_t iplPhase = ENUM_ATTR_SYSTEM_IPL_PHASE_HB_IPL;
//        FAPI_ATTR_GET(ATTR_SYSTEM_IPL_PHASE, sysTgt, iplPhase);
//        if( ENUM_ATTR_SYSTEM_IPL_PHASE_CACHE_CONTAINED == iplPhase )
//        {
//            break;
//        }
//        rc = istepWithEq(i_hwp);
//     }while(0);
    SBE_EXIT("istepWithEqCondtional");
    return rc;
}
//----------------------------------------------------------------------------

ReturnCode istepWithCoreConditional( voidfuncptr_t i_hwp)
{
    SBE_ENTER("istepWithCoreCondtional");
    ReturnCode rc = FAPI2_RC_SUCCESS;
//    fapi2::Target<fapi2::TARGET_TYPE_SYSTEM > sysTgt;
//    do
//    {
//        uint8_t iplPhase = ENUM_ATTR_SYSTEM_IPL_PHASE_HB_IPL;
//        FAPI_ATTR_GET(ATTR_SYSTEM_IPL_PHASE, sysTgt, iplPhase);
//        if( ENUM_ATTR_SYSTEM_IPL_PHASE_CACHE_CONTAINED == iplPhase )
//        {
//            break;
//        }
//        rc = istepWithCore(i_hwp);
//     }while(0);
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
constexpr uint32_t HB_MEM_WINDOW_SIZE = 10*1024*1024; //10 MB
//ReturnCode istepLoadBootLoader( voidfuncptr_t i_hwp)
//{
//    ReturnCode rc = FAPI2_RC_SUCCESS;
//
//    // Get master Core
//    uint8_t coreId = 0;
//    Target< TARGET_TYPE_SYSTEM > sysTgt;
//    Target<TARGET_TYPE_PROC_CHIP > proc = plat_getChipTarget();
//    FAPI_ATTR_GET(fapi2::ATTR_MASTER_CORE,proc,coreId);
//    fapi2::Target<fapi2::TARGET_TYPE_CORE >
//        coreTgt(plat_getTargetHandleByInstance<fapi2::TARGET_TYPE_CORE>(coreId));
//    // Get hbbl section
//    P9XipHeader *hdr = getXipHdr();
//    P9XipSection *hbblSection =  &(hdr->iv_section[P9_XIP_SECTION_SBE_HBBL]);
//
//    uint64_t drawer_base_address_nm0, drawer_base_address_nm1;
//    uint64_t drawer_base_address_m;
//    uint64_t drawer_base_address_mmio;
//    uint64_t l_hostboot_hrmor_offset;
//    do
//    {
//        // Update the ATTR_SBE_ADDR_KEY_STASH_ADDR before calling the bootloader,
//        // since it is going to access these data from inside.
//        uint64_t addr = SBE_GLOBAL->sbeKeyAddrPair.fetchStashAddrAttribute();
//        PLAT_ATTR_INIT(fapi2::ATTR_SBE_ADDR_KEY_STASH_ADDR, sysTgt, addr);
//
//        //Initialize attribute ATTR_LPC_CONSOLE_INITIALIZED.
//        ATTR_LPC_CONSOLE_INITIALIZED_Type isLpcInitialized = SBE_GLOBAL->sbeUartActive;
//        PLAT_ATTR_INIT(fapi2::ATTR_LPC_CONSOLE_INITIALIZED, sysTgt, isLpcInitialized);
//
//        fapi2::ATTR_BACKING_CACHES_NUM_Type l_ATTR_BACKING_CACHES_NUM;
//
//        //Backing cache is of 4MB size.
//        //Num of backing cache should be equal or greater than 2 for IPL.
//
//        FAPI_ATTR_GET(fapi2::ATTR_BACKING_CACHES_NUM,
//                               proc,
//                               l_ATTR_BACKING_CACHES_NUM);
//        if(l_ATTR_BACKING_CACHES_NUM < 2)
//        {
//           SBE_ERROR(" Num of backing cache is less than 2. Cannot proceed with IPL");
//           pk_halt();
//        }
//        ATTR_NUM_KEY_ADDR_PAIR_Type l_value = MAX_ROW_COUNT;
//        PLAT_ATTR_INIT(fapi2::ATTR_NUM_KEY_ADDR_PAIR, sysTgt, l_value);
//        SBE_EXEC_HWP(rc, p10_sbe_load_bootloader, proc, coreTgt, hbblSection->iv_size,
//                     getSectionAddr(hbblSection))
//        if(rc != FAPI2_RC_SUCCESS)
//        {
//            SBE_ERROR(" p10_sbe_load_bootloader failed");
//            break;
//        }
//
//        // Open HB Dump memory Region
//        fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYSTEM;
//        FAPI_ATTR_GET(fapi2::ATTR_HOSTBOOT_HRMOR_OFFSET,
//                      FAPI_SYSTEM,
//                      l_hostboot_hrmor_offset);
//        //rc = p9_fbc_utils_get_chip_base_address_no_aliases(
//        rc = p10_fbc_utils_get_chip_base_address(
//                                    proc,
//                                    HB_BOOT_ID,
//                                    drawer_base_address_nm0,
//                                    drawer_base_address_nm1,
//                                    drawer_base_address_m,
//                                    drawer_base_address_mmio);
//        if(rc != FAPI2_RC_SUCCESS)
//        {
//            SBE_ERROR(" p9_fbc_utils_get_chip_base_address failed");
//            break;
//        }
//        drawer_base_address_nm0 += l_hostboot_hrmor_offset;
//        SBE_INFO("istep 5.1 HB Dump mem Region [0x%08X%08X]",
//                                SBE::higher32BWord(drawer_base_address_nm0),
//                                SBE::lower32BWord(drawer_base_address_nm0));
//        mainStoreSecMemRegionManager.add(drawer_base_address_nm0,
//                                    HB_MEM_WINDOW_SIZE,
//                                    static_cast<uint8_t>(memRegionMode::READ));
//
//    } while(0);
//
//    return rc;
//}

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
#if 0
    do
    {
        rc = performTpmReset();
        if( rc != FAPI2_RC_SUCCESS )
        {
            SBE_ERROR(SBE_FUNC" performTpmReset failed");
            break;
        }
        g_sbeRole = SbeRegAccess::theSbeRegAccess().isSbeMaster() ?
                    SBE_ROLE_MASTER : SBE_ROLE_SLAVE;
        SBE_INFO(SBE_FUNC"g_sbeRole [%x]", g_sbeRole);
        if(SBE_ROLE_SLAVE == g_sbeRole)
        {
            (void)SbeRegAccess::theSbeRegAccess().stateTransition(
                                            SBE_RUNTIME_EVENT);
        }
    }while(0);
#endif
    return rc;
    #undef SBE_FUNC
}

/*
 * --------------------------------------------- start SEEPROM CODE
 */
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

#if 0
    l_rc = stopAllCoreInstructions();
    if( l_rc == FAPI2_RC_SUCCESS )
    {
        l_rc = flushNVDIMM();
        if( l_rc != FAPI2_RC_SUCCESS )
        {
            SBE_ERROR(SBE_FUNC "flushNVDIMM failed");
        }
    }
    else
    {
        SBE_ERROR(SBE_FUNC "Stop all core instructions is failed, "
                 "RC=[0x%08X]", l_rc);
    }
#endif
    SBE_EXIT(SBE_FUNC);
    return l_rc;
    #undef SBE_FUNC
}

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

//----------------------------------------------------------------------------
ReturnCode istepWithExL2Flush( voidfuncptr_t i_hwp)
{
    #define SBE_FUNC "istepWithExL2Flush"
    SBE_ENTER(SBE_FUNC);
    ReturnCode rc = FAPI2_RC_SUCCESS;
#if 0
    Target<TARGET_TYPE_PROC_CHIP > procTgt = plat_getChipTarget();
    for (auto& exTgt : procTgt.getChildren<fapi2::TARGET_TYPE_EX>())
    {
        bool l2IsScomable[MAX_L2_PER_QUAD] = {false};
        bool l2IsScanable[MAX_L2_PER_QUAD] = {false};
        bool l3IsScomable[MAX_L3_PER_QUAD] = {false};
        bool l3IsScanable[MAX_L3_PER_QUAD] = {false};
        uint8_t chipUnitNum = 0;

        // Get EQ Parent to figure if this EX's L2 is scommable
        fapi2::Target<fapi2::TARGET_TYPE_EQ> eqTgt =
                                exTgt.getParent<fapi2::TARGET_TYPE_EQ>();

        //rc = p9_query_cache_access_state(eqTgt,
        //                                 l2IsScomable,
        //                                 l2IsScanable,
        //                                 l3IsScomable,
        //                                 l3IsScanable);
        if(rc != FAPI2_RC_SUCCESS)
        {
            SBE_ERROR(SBE_FUNC " p9_query_cache_access_state failed, "
                "RC=[0x%08X]", rc);
            break;
        }

        FAPI_ATTR_GET( fapi2::ATTR_CHIP_UNIT_POS, exTgt, chipUnitNum );

        // check the position of EX i.e. Ex0 or Ex1
        if( !(l2IsScomable[(chipUnitNum % 2)]) )
        {
            SBE_INFO(SBE_FUNC "Ex chipletId [%d] Ex%d is not l2 scomable, "
                "so no purge", chipUnitNum, (chipUnitNum % 2));
            continue;
        }

        p9core::purgeData_t l_purgeData;
        //SBE_EXEC_HWP(rc,
        //             reinterpret_cast<sbeIstepHwpExL2Flush_t>(i_hwp),
        //             exTgt,
        //             l_purgeData)
        if(rc != FAPI2_RC_SUCCESS)
        {
            SBE_ERROR(SBE_FUNC " p9_l2_flush failed, RC=[0x%08X]", rc);
            break;
        }
    }
#endif
    SBE_EXIT(SBE_FUNC);
    return rc;
    #undef SBE_FUNC
}

//----------------------------------------------------------------------------
ReturnCode istepWithExL3Flush( voidfuncptr_t i_hwp)
{
    #define SBE_FUNC "istepWithExL3Flush"
    SBE_ENTER(SBE_FUNC);
    ReturnCode rc = FAPI2_RC_SUCCESS;
#if 0
    Target<TARGET_TYPE_PROC_CHIP > procTgt = plat_getChipTarget();
    for (auto& exTgt : procTgt.getChildren<fapi2::TARGET_TYPE_EX>())
    {
        bool l2IsScomable[MAX_L2_PER_QUAD] = {false};
        bool l2IsScanable[MAX_L2_PER_QUAD] = {false};
        bool l3IsScomable[MAX_L3_PER_QUAD] = {false};
        bool l3IsScanable[MAX_L3_PER_QUAD] = {false};
        uint8_t chipUnitNum = 0;

        // Get EQ Parent to figure if this EX's L3 is scommable
        fapi2::Target<fapi2::TARGET_TYPE_EQ> eqTgt =
                                exTgt.getParent<fapi2::TARGET_TYPE_EQ>();

        //rc = p9_query_cache_access_state(eqTgt,
        //                                 l2IsScomable,
        //                                 l2IsScanable,
        //                                 l3IsScomable,
        //                                 l3IsScanable);
        if(rc != FAPI2_RC_SUCCESS)
        {
            SBE_ERROR(SBE_FUNC " p9_query_cache_access_state failed, "
                "RC=[0x%08X]", rc);
            break;
        }

        FAPI_ATTR_GET( fapi2::ATTR_CHIP_UNIT_POS, exTgt, chipUnitNum );

        // check the position of EX i.e. Ex0 or Ex1
        if( !(l3IsScomable[(chipUnitNum % 2)]) )
        {
            SBE_INFO(SBE_FUNC "Ex chipletId [%d] EX%d is not l3 scomable, "
                "so no purge", chipUnitNum, (chipUnitNum % 2));
            continue;
        }

        //SBE_EXEC_HWP(rc,
        //            reinterpret_cast<sbeIstepHwpExL3Flush_t>(i_hwp),
        //             exTgt,
        //             L3_FULL_PURGE,
        //             0x0)
        if(rc != FAPI2_RC_SUCCESS)
        {
            SBE_ERROR(SBE_FUNC " p9_l3_flush failed, RC=[0x%08X]", rc);
            break;
        }
    }
#endif
    SBE_EXIT(SBE_FUNC);
    return rc;
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
    ReturnCode l_rc = FAPI2_RC_SUCCESS;
#if 0
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
        //SBE_EXEC_HWP(l_rc, reinterpret_cast<sbeIstepHwpQuadPoweroff_t>(i_hwp), l_quad, G_ring_save)
    }
#endif
    SBE_EXIT(SBE_FUNC);
    return l_rc;
    #undef SBE_FUNC
}

//----------------------------------------------------------------------------
ReturnCode istepWithProcQuiesceLQASet( voidfuncptr_t i_hwp )
{
    #define SBE_FUNC "istepWithProcQuiesceLQASet"
    SBE_ENTER(SBE_FUNC);
    ReturnCode l_rc = FAPI2_RC_SUCCESS;
#if 0
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
#endif
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
#if 0
    Target<TARGET_TYPE_PROC_CHIP > proc = plat_getChipTarget();
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

            PLAT_ATTR_INIT(fapi2::ATTR_EQ_GARD, proc, l_eqMask);
            PLAT_ATTR_INIT(fapi2::ATTR_EC_GARD, proc, l_ecMask);

            // Apply the gard records
            rc = plat_ApplyGards();
            if( rc != FAPI2_RC_SUCCESS )
            {
                SBE_ERROR(SBE_FUNC" Failed to to apply gard records");
                break;
            }

            if (g_sbeRole == SBE_ROLE_MASTER)
            {
                // TODO via RTC 135345
                // Once multicast targets are supported, we may need to pass
                // p9selectex::ALL as input.
                SBE_EXEC_HWP(rc, reinterpret_cast<p9_sbe_select_ex_FP_t>(i_hwp), proc, p9selectex::SINGLE)
                if( rc != FAPI2_RC_SUCCESS )
                {
                    SBE_ERROR(SBE_FUNC" Failed hwp p9_sbe_select_ex_hwp");
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
#endif
    SBE_EXIT(SBE_FUNC);
    return rc;
    #undef SBE_FUNC
}

//----------------------------------------------------------------------------
ReturnCode istepStopClockMpipl( voidfuncptr_t i_hwp )
{
    #define SBE_FUNC "istepStopClockMpipl"
    SBE_ENTER(SBE_FUNC);
    uint32_t l_fapiRc = FAPI2_RC_SUCCESS;
#if 0
    p9_stopclocks_flags l_flags; // Default Flag Values
    Target<TARGET_TYPE_PROC_CHIP > l_procTgt = plat_getChipTarget();
    p9hcd::P9_HCD_CLK_CTRL_CONSTANTS l_clk_regions =
                p9hcd::CLK_REGION_ALL_BUT_PLL_REFR;
    p9hcd::P9_HCD_EX_CTRL_CONSTANTS l_ex_select = p9hcd::BOTH_EX;

    l_flags.clearAll();
    l_flags.stop_core_clks = true;
    l_flags.stop_cache_clks = true;

    //SBE_EXEC_HWP(l_fapiRc,
     //            reinterpret_cast<p9_stopclocks_FP_t>(i_hwp),
     //            l_procTgt,
     //            l_flags,
     //            l_clk_regions,
     //            l_ex_select);
#endif
    SBE_EXIT(SBE_FUNC);
    return l_fapiRc;
    #undef SBE_FUNC
}

ReturnCode istepStopClockDFT( voidfuncptr_t i_hwp )
{
    #define SBE_FUNC "istepStopClockDFT"
    SBE_ENTER(SBE_FUNC);
    uint32_t l_fapiRc = FAPI2_RC_SUCCESS;
    p10_stopclocks_flags l_flags; // Default Flag Values
    Target<fapi2::TARGET_TYPE_PROC_CHIP> l_procTgt = plat_getChipTarget();

    l_flags.clearAll();
    l_flags.stop_nest_clks = true;
    l_flags.stop_pcie_clks = true;
    l_flags.stop_mc_clks = true;
    l_flags.stop_pau_clks = true;
    l_flags.stop_axon_clks = true;
    l_flags.stop_tp_clks = true;
	l_flags.ignore_pib_net_dpllnest_ppll = true;
    //l_flags.stop_sbe_clks = true;
    //l_flags.stop_vitl_clks = true;
    l_flags.stop_cache_clks = true;
    l_flags.stop_core_clks = true;
    l_flags.stop_eq_clks = true;

    SBE_EXEC_HWP(l_fapiRc,
                 reinterpret_cast<p10_stopclocks_FP_t>(i_hwp),
                 l_procTgt,
                 l_flags);
    
    SBE_EXIT(SBE_FUNC);
    return l_fapiRc;
    #undef SBE_FUNC
}


/*
 * end SEEPROM CODE --------------------------------------------------
 */
#endif // ifdef __SBEFW_SEEPROM__

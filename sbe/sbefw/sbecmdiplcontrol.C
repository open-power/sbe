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
#include <p9_sbe_tp_gptr_time_repr_initf.H>
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
// Core HWP header file
#include <p9_hcd_core.H>

// istep 5 hwp header files
#include "p9_sbe_instruct_start.H"
#include "p9_sbe_load_bootloader.H"

#include "sbeXipUtils.H" // For getting hbbl offset
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
static const uint8_t SLAVE_LAST_MINOR_ISTEP = 18;
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
             { NULL, NULL },
             { &istepAttrSetup, { .procHwp = &p9_sbe_attr_setup }},
             { &istepWithProc, { .procHwp = &p9_sbe_tp_chiplet_init1 }},
             { &istepWithProc, { .procHwp = &p9_sbe_tp_gptr_time_initf }},
             { &istepNoOp, NULL },  // DFT only
             { &istepWithProc, { .procHwp = &p9_sbe_npll_initf }},
             { &istepWithProc, { .procHwp = &p9_sbe_npll_setup }},
             { &istepWithProc, { .procHwp = &p9_sbe_tp_switch_gears }},
             { &istepWithProc, { .procHwp = &p9_sbe_clock_test2 }},
             { &istepWithProc, { .procHwp = &p9_sbe_tp_chiplet_reset }},
             { &istepWithProc, { .procHwp = &p9_sbe_tp_gptr_time_repr_initf }},
             { &istepWithProc, { .procHwp = &p9_sbe_tp_chiplet_init2 }},
             { &istepNoOp, NULL },  // DFT only
             { &istepWithProc, { .procHwp = &p9_sbe_tp_arrayinit }},
             { &istepWithProc, { .procHwp = &p9_sbe_tp_initf }},
             { &istepNoOp, NULL }, // DFT only
             { &istepWithProc, { .procHwp = &p9_sbe_tp_chiplet_init3 }},
         };

static istepMap_t g_istep3PtrTbl[ ISTEP3_MAX_SUBSTEPS ] =
         {
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
         };
static istepMap_t g_istep4PtrTbl[ ISTEP4_MAX_SUBSTEPS ] =
         {
             { &istepWithEq, { .eqHwp  = &p9_hcd_cache_poweron} },
             { &istepWithEq, { .eqHwp  = &p9_hcd_cache_chiplet_reset } },
             // TODO via RTC 148465
             // L1 for hwp is still not available
             { &istepNoOp, NULL },  //  p9_hcd_cache_chiplet_l3_dcc_setup.C
             { &istepWithEq, { .eqHwp  = &p9_hcd_cache_gptr_time_initf }},
             // TODO via RTC 148465
             // L1 for hwp is still not available
             { &istepNoOp, NULL },  // p9_hcd_cache_dpll_initf.C
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
         };

// TODO via RTC 135345
//  Add the support for istep 5 HWP
static istepMap_t g_istep5PtrTbl[ ISTEP5_MAX_SUBSTEPS ]
         {
             { &istepLoadBootLoader, NULL },
             { &istepStartInstruction,  { .coreHwp = &p9_sbe_instruct_start }},
         };

// Functions
//----------------------------------------------------------------------------
uint32_t sbeHandleIstep (uint8_t *i_pArg)
{
    #define SBE_FUNC "sbeHandleIstep "
    SBE_DEBUG(SBE_FUNC);
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

        SBE_DEBUG(SBE_FUNC"Major number:0x%08x minor number:0x%08x",
                  req.major, req.minor );

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

        uint32_t distance = 1; //initialise by 1 for entry count itself.
        len = sizeof(respHdr)/sizeof(uint32_t);
        // sbeDownFifoEnq_mult.
        rc = sbeDownFifoEnq_mult ( len, ( uint32_t *) &respHdr);
        if (rc)
        {
            break;
        }
        distance += len;

        // If no ffdc , exit;
        if( ffdc.getRc() )
        {
            len = sizeof(ffdc)/sizeof(uint32_t);
            rc = sbeDownFifoEnq_mult ( len, ( uint32_t *) &ffdc);
            if (rc)
            {
                break;
            }
            distance += len;
        }
        len = sizeof(distance)/sizeof(uint32_t);
        //@TODO via RTC 129076.
        //Need to add FFDC data as well.
        rc = sbeDownFifoEnq_mult ( len, &distance);
        if (rc)
        {
            break;
        }
    }while(0);

    if( rc )
    {
        SBE_ERROR( SBE_FUNC"Failed. rc[0x%X]", rc);
    }
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
    SBE_DEBUG(SBE_FUNC"Major number:0x%x minor number:0x%x",
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

    // TODO: via RTC: 126146 - Should the state be set to DUMP even in istep
    // mode failures? Revisit this when we implement state management.
    if(rc != FAPI2_RC_SUCCESS)
    {
        (void)SbeRegAccess::theSbeRegAccess().updateSbeState(SBE_STATE_DUMP);
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
            // Cannot run beyond 3.18 on a slave SBE
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
    SBE_DEBUG("istepAttrSetup");
    Target<TARGET_TYPE_PROC_CHIP > proc = plat_getChipTarget();
    ReturnCode rc = FAPI2_RC_SUCCESS;
    do
    {
        assert( NULL != i_hwp.procHwp );
        rc = i_hwp.procHwp(proc);
        if( rc != FAPI2_RC_SUCCESS )
        {
            break;
        }
        // Apply the gard records
        rc = plat_ApplyGards();
     }while(0);
    return rc;
}

//----------------------------------------------------------------------------

ReturnCode istepWithProc( sbeIstepHwp_t i_hwp)
{
    SBE_DEBUG("istepWithProc");
    Target<TARGET_TYPE_PROC_CHIP > proc = plat_getChipTarget();
    assert( NULL != i_hwp.procHwp );
    return i_hwp.procHwp(proc);
}

//----------------------------------------------------------------------------

ReturnCode istepSelectEx( sbeIstepHwp_t i_hwp)
{
    SBE_DEBUG("istepWithProc");
    Target<TARGET_TYPE_PROC_CHIP > proc = plat_getChipTarget();
    // TODO via RTC 135345
    // Once multicast targets are supported, we may need to pass
    // p9selectex::ALL as input.
    return p9_sbe_select_ex(proc, p9selectex::SINGLE);
}

//----------------------------------------------------------------------------


ReturnCode istepWithEq( sbeIstepHwp_t i_hwp)
{
    SBE_DEBUG("istepWithEq");
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
    return i_hwp.eqHwp( eqTgt );
}

//----------------------------------------------------------------------------

ReturnCode istepWithCore( sbeIstepHwp_t i_hwp)
{
    SBE_DEBUG("istepWithCore");
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
    return i_hwp.coreHwp( coreTgt );
}

//----------------------------------------------------------------------------

ReturnCode istepWithEqConditional( sbeIstepHwp_t i_hwp)
{
    SBE_DEBUG("istepWithEqCondtional");
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
    return rc;
}

//----------------------------------------------------------------------------

ReturnCode istepWithCoreConditional( sbeIstepHwp_t i_hwp)
{
    SBE_DEBUG("istepWithCoreCondtional");
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
    return rc;
}

//----------------------------------------------------------------------------

ReturnCode istepLoadBootLoader( sbeIstepHwp_t i_hwp)
{
    // Get master Ex
    uint8_t exId = 0;
    Target<TARGET_TYPE_PROC_CHIP > proc = plat_getChipTarget();
    FAPI_ATTR_GET(fapi2::ATTR_MASTER_EX,proc,exId);
    fapi2::Target<fapi2::TARGET_TYPE_EX > exTgt(exId);
    // Get hbbl section
    P9XipHeader *hdr = getXipHdr();
    P9XipSection *hbblSection =  &(hdr->iv_section[P9_XIP_SECTION_SBE_HBBL]);

    ReturnCode rc = p9_sbe_load_bootloader( proc, exTgt, hbblSection->iv_size,
                                            getSectionAddr(hbblSection) );
    return rc;
}

//----------------------------------------------------------------------------

ReturnCode istepStartInstruction( sbeIstepHwp_t i_hwp)
{
    ReturnCode rc = FAPI2_RC_SUCCESS;
    rc = istepWithCore(i_hwp);
    if(rc == FAPI2_RC_SUCCESS)
    {
        (void)SbeRegAccess::theSbeRegAccess().updateSbeState(SBE_STATE_RUNTIME);
    }
    return rc;
}

//----------------------------------------------------------------------------
ReturnCode istepCheckSbeMaster( sbeIstepHwp_t i_hwp)
{
    ReturnCode rc = FAPI2_RC_SUCCESS;
    g_sbeRole = SbeRegAccess::theSbeRegAccess().isSbeSlave() ?
                SBE_ROLE_SLAVE : SBE_ROLE_MASTER;
    if(SBE_ROLE_SLAVE == g_sbeRole)
    {
        (void)SbeRegAccess::theSbeRegAccess().updateSbeState(SBE_STATE_RUNTIME);
    }
    return rc;
}

//----------------------------------------------------------------------------
ReturnCode istepNoOp( sbeIstepHwp_t i_hwp)
{
    SBE_DEBUG("istepNoOp");
    return FAPI2_RC_SUCCESS ;
}

//----------------------------------------------------------------------------

uint32_t sbeWaitForSbeIplDone (uint8_t *i_pArg)
{
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
    SBE_TRACE("sbeWaitForSbeIplDone");
    return rc;
}

//----------------------------------------------------------------------------
void sbeDoContinuousIpl()
{
    #define SBE_FUNC "sbeDoContinuousIpl "
    SBE_DEBUG(SBE_FUNC);
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
        (void)SbeRegAccess::theSbeRegAccess().updateSbeState(SBE_STATE_IPLING);
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
                    SBE_DEBUG(SBE_FUNC"Failed istep execution in plck mode: "
                            "Major: %d, Minor: %d", l_major, l_minor);
                    l_done = true;
                    break;
                }
                // Check if we are at step 3.18 on the slave SBE
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
    #undef SBE_FUNC
}


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

#include "fapi2.H"
// Pervasive HWP Header Files ( istep 2)
#include <p9_sbe_attr_setup.H>
#include <p9_sbe_tp_chiplet_init1.H>
#include <p9_sbe_npll_initf.H>
#include <p9_sbe_npll_setup.H>
#include <p9_sbe_tp_switch_gears.H>
#include <p9_sbe_tp_chiplet_reset.H>
#include <p9_sbe_tp_gptr_time_repr_initf.H>
#include <p9_sbe_tp_chiplet_init2.H>
#include <p9_sbe_tp_arrayinit.H>
#include <p9_sbe_tp_initf.H>
#include <p9_sbe_tp_chiplet_init3.H>

// Pervasive HWP Header Files ( istep 3)
#include <p9_sbe_chiplet_reset.H>
#include <p9_sbe_chiplet_pll_initf.H>
#include <p9_sbe_chiplet_pll_setup.H>
#include <p9_sbe_gptr_time_repr_initf.H>
#include <p9_sbe_chiplet_init.H>
#include <p9_sbe_arrayinit.H>
#include <p9_sbe_tp_enable_ridi.H>
#include <p9_sbe_setup_evid.H>
#include <p9_sbe_nest_initf.H>
#include <p9_sbe_nest_startclocks.H>
#include <p9_sbe_nest_enable_ridi.H>
#include <p9_sbe_startclock_chiplets.H>
#include <p9_sbe_scominit.H>
#include <p9_sbe_lpc_init.H>
#include <p9_sbe_fabricinit.H>
#include <p9_sbe_mcs_setup.H>
#include <p9_sbe_select_ex.H>
// Cache HWP header file
#include "p9_hcd_cache.H"
// Core HWP header file
#include "p9_hcd_core.H"

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
    SBE_ISTEP3 = 3,
    SBE_ISTEP4 = 4,
    SBE_ISTEP5 = 5,
}sbe_supported_steps_t;

// constants
// TODO via RTC 135345
// Check with Dean. In IPL flow doc ( version 0.63 ),
// after istep 2.9, next istep is 2.11. istep 2.10 is not present.
// So in IPL flow doc, total minor isteps for step 2 are 16.
const uint32_t ISTEP2_MAX_SUBSTEPS = 15;
const uint32_t ISTEP3_MAX_SUBSTEPS = 20;
const uint32_t ISTEP4_MAX_SUBSTEPS = 31;
const uint32_t ISTEP5_MAX_SUBSTEPS = 2;

// File static data

static istepMap_t g_istep2PtrTbl[ ISTEP2_MAX_SUBSTEPS ] =
         {
             { NULL, NULL },
             { &istepAttrSetup, { .procHwp = &p9_sbe_attr_setup }},
             { &istepWithProc, { .procHwp = &p9_sbe_tp_chiplet_init1 }},
             { &istepNoOp, NULL },  // DFT only
             { &istepWithProc, { .procHwp = &p9_sbe_npll_initf }},
             { &istepWithProc, { .procHwp = &p9_sbe_npll_setup }},
             { &istepWithProc, { .procHwp = &p9_sbe_tp_switch_gears }},
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
             { &istepWithProc, { .procHwp = &p9_sbe_chiplet_pll_initf }},
             { &istepWithProc, { .procHwp = &p9_sbe_chiplet_pll_setup }},
             { &istepWithProc, { .procHwp = &p9_sbe_gptr_time_repr_initf }},
             { &istepWithProc, { .procHwp = &p9_sbe_chiplet_init }},
             { &istepNoOp, NULL }, // DFT only
             { &istepWithProc, { .procHwp = &p9_sbe_arrayinit }},
             { &istepNoOp, NULL }, // DFT only
             { &istepWithProc, { .procHwp = &p9_sbe_tp_enable_ridi }},
             { &istepWithProc, { .procHwp = &p9_sbe_setup_evid }},
             { &istepWithProc, { .procHwp = &p9_sbe_nest_initf }},
             { &istepWithProc, { .procHwp = &p9_sbe_nest_startclocks }},
             { &istepWithProc, { .procHwp = &p9_sbe_nest_enable_ridi }},
             { &istepWithProc, { .procHwp = &p9_sbe_startclock_chiplets }},
             { &istepWithProc, { .procHwp = &p9_sbe_scominit }},
             { &istepWithProc, { .procHwp = &p9_sbe_lpc_init }},
             { &istepWithProc, { .procHwp = &p9_sbe_fabricinit }},
             { &istepNoOp, NULL }, // TODO via RTC 120752
                                   // FW proc_sbe_check_master
             // TODO via RTC 142710
             // mcs_setup does not compile currently as MI target support
             // is not present. So currently this istep has neem made NoOp
             // in this code
             //{ &istepWithProc, { .procHwp = &p9_sbe_mcs_setup }},
             { &istepNoOp, NULL }, // mcs_setup does not compile currently
             { &istepSelectEx, NULL },
         };
static istepMap_t g_istep4PtrTbl[ ISTEP4_MAX_SUBSTEPS ] =
         {
             { &istepWithEq, { .eqHwp  = &p9_hcd_cache_poweron} },
             { &istepWithEq, { .eqHwp  = &p9_hcd_cache_chiplet_reset } },
             { &istepWithEq, { .eqHwp  = &p9_hcd_cache_gptr_time_initf }},
             { &istepWithEq, { .eqHwp  = &p9_hcd_cache_dpll_setup }},
             { &istepWithEq, { .eqHwp  = &p9_hcd_cache_chiplet_init }},
             { &istepWithEq, { .eqHwp  = &p9_hcd_cache_repair_initf }},
             { &istepWithEq, { .eqHwp  = &p9_hcd_cache_arrayinit }},
             { &istepNoOp, NULL },  // DFT Only
             { &istepNoOp, NULL },  // DFT Only
             { &istepWithEq, { .eqHwp = &p9_hcd_cache_initf }},
             { &istepWithEq, { .eqHwp = &p9_hcd_cache_startclocks }},
             { &istepWithEq, { .eqHwp = &p9_hcd_cache_scominit }},
             { &istepWithEq, { .eqHwp = &p9_hcd_cache_scomcust }},
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
             { &istepWithCore, { .coreHwp = &p9_hcd_core_startclocks }},
             { &istepWithCore, { .coreHwp = &p9_hcd_core_scominit }},
             { &istepWithCore, { .coreHwp = &p9_hcd_core_scomcust }},
             { &istepNoOp, NULL },
             { &istepNoOp, NULL },
         };

// TODO via RTC 135345
//  Add the support for istep 5 HWP
static istepMap_t g_istep5PtrTbl[ ISTEP5_MAX_SUBSTEPS ]
         {
             { &istepLoadBootLoader, NULL },
             { &istepWithCore,  { .coreHwp = &p9_sbe_instruct_start }},
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
    sbeResponseGenericHeader_t respHdr;
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

    // bits 16-23 major istep number, 24-31 minor istep number
    uint64_t l_iplState = (uint64_t)(i_major)<<40  |  (uint64_t)(i_minor)<<32;
    SBE_UPDATE_SBE_MSG_REG (l_iplState);

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

ReturnCode istepLoadBootLoader( sbeIstepHwp_t i_hwp)
{
    // Get master Ex
    uint8_t exId = 0;
    Target<TARGET_TYPE_PROC_CHIP > proc = plat_getChipTarget();
    FAPI_ATTR_GET(fapi2::ATTR_MASTER_EX,proc,exId);
    fapi2::Target<fapi2::TARGET_TYPE_EX > exTgt(exId);
    // Get hbbl section
    P9XipHeader *hdr = getXipHdr();
    P9XipSection *hbblSection =  &(hdr->iv_section[P9_XIP_SECTION_HBBL]);

    ReturnCode rc = p9_sbe_load_bootloader( proc, exTgt, hbblSection->iv_size, 
                                            getSectionAddr(hbblSection) );
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

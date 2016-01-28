/*
 * @file: ppe/sbe/sbefw/sbecmdcntrldmt.C
 *
 * @brief This file contains the Core State Control Messages
 *
 */

#include "sbecmdcntrldmt.H"
#include "sbefifo.H"
#include "sbetrace.H"
#include "sbe_sp_intf.H"
#include "sbe_build_info.H"
#include "sbeFifoMsgUtils.H"
#include "sbeHostMsg.H"

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
uint32_t sbeControlDeadmanTimer (uint8_t *i_pArg)
{
    #define SBE_FUNC "sbeControlDeadmanTimer"
    SBE_DEBUG(SBE_FUNC);
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;

    return rc;
    #undef SBE_FUNC
}

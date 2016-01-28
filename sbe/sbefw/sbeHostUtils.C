/*
 * @file: ppe/sbe/sbefw/sbeHostUtils.C
 *
 * @brief This file contains the PSU Access Utility Functions
 *
 */

#include "sbetrace.H"
#include "sbe_sp_intf.H"
#include "sbeHostUtils.H"
#include "sbeHostMsg.H"
#include "sbe_host_intf.H"
#include "sbeerrorcodes.H"
#include "assert.h"



///////////////////////////////////////////////////////////////////
// PSU->SBE register access utilities
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
uint32_t sbeReadPsu2SbeMbxReg (uint32_t       i_addr,
                               uint8_t       &io_count,
                               uint64_t       *o_pData)
{
    #define SBE_FUNC " sbeReadPsu2SbeMbxReg "
    SBE_DEBUG(SBE_FUNC"io_count[0x%02X], i_addr=[0x%08X]", io_count, i_addr);

    uint32_t l_rc = SBE_SEC_OPERATION_SUCCESSFUL;
    uint8_t  l_count = 0;

    assert((io_count >0 ) && (NULL != o_pData))
    assert( (SBE_HOST_PSU_MBOX_REG0 <= i_addr) &&
            (SBE_HOST_PSU_MBOX_REG3 >= (i_addr + io_count - 1)) )

    while (l_count < io_count)
    {
        l_rc = getscom_abs ( i_addr,
                    reinterpret_cast<uint64_t*>(&o_pData[l_count]) );

        if (l_rc)
        {
            // Error while reading from PSU->SBE mbx register
            SBE_ERROR(SBE_FUNC"getscom_abs failed,"
                         "l_rc=[0x%08X], i_addr=[0x%08X]",
                         l_rc, i_addr);
            break;
        }

        SBE_DEBUG(SBE_FUNC"l_data=[0x%016X]", o_pData[l_count]);
        ++l_count;
        ++i_addr;
    }

    // Indicate the number of Mbx registers read off
    io_count = l_count;
    return l_rc;

    #undef SBE_FUNC
}

///////////////////////////////////////////////////////////////////
// SBE->PSU register access utilities
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
uint32_t sbeIntrHostUponRespWaiting ()
{
    uint32_t l_rc = SBE_SEC_OPERATION_SUCCESSFUL;

    // Interrupt the host by setting bit0 in SBE->PSU DB register
    // if the caller requested for it.
    if (g_sbePsu2SbeCmdReqHdr.flags & SBE_PSU_FLAGS_RESP_REQUIRED)
    {
        l_rc = sbeSetSbe2PsuDbBitX(SBE_SBE2PSU_DOORBELL_SET_BIT0);
    }
    return l_rc;
}

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
uint32_t sbeAcknowledgeHost ()
{
    uint32_t l_rc = SBE_SEC_OPERATION_SUCCESSFUL;

    // Set the Ack bit in SBE->PSU DB register
    // if the caller requested for it.
    if (g_sbePsu2SbeCmdReqHdr.flags & SBE_PSU_FLAGS_ACK_REQUIRED)
    {
        l_rc = sbeSetSbe2PsuDbBitX(SBE_SBE2PSU_DOORBELL_SET_BIT1);
    }
    return l_rc;
}

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
uint32_t sbeWriteSbe2PsuMbxReg (uint32_t        i_addr,
                                const uint64_t  *i_pData,
                                uint8_t         &io_count)
{
    #define SBE_FUNC " sbeWriteSbe2PsuMbxReg "
    SBE_DEBUG(SBE_FUNC"io_count[0x%02X], i_addr=[0x%08X]", io_count, i_addr);
    uint32_t  l_rc   = SBE_SEC_OPERATION_SUCCESSFUL;
    uint8_t   l_count = 0;

    assert( ((io_count >0 ) && (NULL != i_pData)) || (io_count = 0) )
    assert( (SBE_HOST_PSU_MBOX_REG4 <= i_addr) &&
            (SBE_HOST_PSU_MBOX_REG7 >= (i_addr + io_count - 1)) )

    while ( (g_sbePsu2SbeCmdReqHdr.flags & SBE_PSU_FLAGS_RESP_REQUIRED) &&
            (l_count < io_count) )
    {
        SBE_DEBUG(SBE_FUNC"l_data=[0x%016X]", *(i_pData+l_count));

        l_rc = putscom_abs ( i_addr, *(i_pData+l_count) );

        if (l_rc)
        {
            // Error while reading from PSU->SBE mbx register
            SBE_ERROR(SBE_FUNC"putscom_abs failed,"
                         "l_rc=[0x%08X], i_addr=[0x%08X]",
                         l_rc, i_addr);
            break;
        }

        ++l_count;
        ++i_addr;
    }

    // Indicate the number of Mbx registers written into
    io_count = l_count;
    return l_rc;

    #undef SBE_FUNC
}

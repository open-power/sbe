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
                               const uint8_t  i_count,
                               uint64_t       *o_pData)
{
    #define SBE_FUNC " sbeReadPsu2SbeMbxReg "
    SBE_DEBUG(SBE_FUNC"i_count[0x%02X], i_addr=[0x%08X]", i_count, i_addr);

    uint32_t l_rc = SBE_SEC_OPERATION_SUCCESSFUL;
    uint8_t  l_count = 0;

    assert((i_count >0 ) && (NULL != o_pData))
    assert( (SBE_HOST_PSU_MBOX_REG0 <= i_addr) &&
            (SBE_HOST_PSU_MBOX_REG3 >= (i_addr + i_count - 1)) )

    while (l_count < i_count)
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
                                const uint8_t   i_count,
                                bool            i_setBit0ToIntrHB)
{
    #define SBE_FUNC " sbeWriteSbe2PsuMbxReg "
    SBE_DEBUG(SBE_FUNC"i_count[0x%02X], i_addr=[0x%08X]", i_count, i_addr);
    uint32_t  l_rc   = SBE_SEC_OPERATION_SUCCESSFUL;
    uint8_t   l_count = 0;

    assert( (i_count >0 ) && (NULL != i_pData) )
    assert( (SBE_HOST_PSU_MBOX_REG4 <= i_addr) &&
            (SBE_HOST_PSU_MBOX_REG7 >= (i_addr + i_count - 1)) )

    if( g_sbePsu2SbeCmdReqHdr.flags & SBE_PSU_FLAGS_RESP_REQUIRED )
    {
        while (l_count < i_count)
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

        if( (i_setBit0ToIntrHB) && (SBE_SEC_OPERATION_SUCCESSFUL == l_rc) )
        {
            // indicate the Host via Bit SBE_SBE2PSU_DOORBELL_SET_BIT0
            l_rc = sbeSetSbe2PsuDbBitX(SBE_SBE2PSU_DOORBELL_SET_BIT0);
            if(l_rc)
            {
                SBE_ERROR(SBE_FUNC " Failed to Sent Ack to Host over "
                        "SBE_SBE2PSU_DOORBELL_SET_BIT0");
            }
        }
    }
    return l_rc;

    #undef SBE_FUNC
}

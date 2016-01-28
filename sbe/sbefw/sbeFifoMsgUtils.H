/*
 * @file: ppe/sbe/sbefw/sbeFifoMsgUtils.H
 *
 * @brief  This file contains the SBE FIFO Access Common Utility Functions
 *
 */

#ifndef __SBEFW_SBEFIFOMSGUTILS_H
#define __SBEFW_SBEFIFOMSGUTILS_H

#include <stdint.h>
#include "sbeexeintf.H"


/**********************************************************************/
// SBE Utilities
/**********************************************************************/

/**
  * @brief sbeUpFifoDeq_mult : Dequeue multiple entries from upstream FIFO
  *
  * @param[in/out] io_len
  *    On input: Non-zero number of entries (excluding EOT) to dequeue.
  *        Ignored when i_flush == true.
  *    On output: Number of entries dequeued (excluding EOT).
  * @param[out]    o_pData  Entries dequeued into the buffer
  * @param[in]     i_isEotExpected true / false
  *    true - Default case.
  *        Caller expects an EOT entry after io_len entries are
  *        dequeued. Accordingly, this function will attempt to dequeue
  *        the EOT entry after io_len entries are dequeued.
  *    false - Caller doesn't expect an EOT after io_len entries are
  *        dequeued. Accordingly, this function will not attempt to
  *        dequeue the EOT entry after io_len entries are dequeued.
  * @param[in]     i_flush true / false
  *    true - caller requested FIFO flush
  *        Usually caller marks this flag as true to handle error scenario.
  *        All entries written in the US fifo (until an EOT is encountered),
  *        would be dequeued and discarded (not processed). Note that io_len
  *        and i_isEotExpected inputs are ignored in this case.
  *        However, flag i_isEotExpected is always interpreted as true in
  *        case.
  *    false - Default good path.
  *        US Fifo entries will be dequeued and processed per inputs io_len
  *        and i_isEotExpected.
  *
  * @return  Return Code  SUCCESS or a secondary response code
  *                SBE_SEC_OPERATION_SUCCESSFUL
  *                SBE_SEC_FIFO_ACCESS_FAILURE
  *                SBE_SEC_UNEXPECTED_EOT_INSUFFICIENT_DATA
  *                SBE_SEC_UNEXPECTED_EOT_EXCESS_DATA
  *                SBE_FIFO_RESET_RECEIVED
  *
  */
extern uint32_t sbeUpFifoDeq_mult (uint32_t    &io_len,
                                   uint32_t    *o_pData,
                                   const bool  i_isEotExpected = true,
                                   const bool  i_flush = false);


/**
  * @brief sbeDownFifoEnq_mult : Enqueue into downstream FIFO
  *
  * @param[in/out] io_len   number of entries to enqueue as input,
  *                         number of entries enqueued as output
  * @param[in]     i_pData  buffer containting data to be enqueued
  *
  * @return        Rc  SUCCESS or a secondary response code
  *                SBE_SEC_OPERATION_SUCCESSFUL
  *                SBE_SEC_FIFO_ACCESS_FAILURE
  *                SBE_FIFO_RESET_RECEIVED
  *
  */
extern uint32_t sbeDownFifoEnq_mult (uint32_t        &io_len,
                                     const uint32_t *i_pData) ;


/**
  * @brief sbeBuildRespHeaderMagicCodeCmdClass
  *          Builds the header word containing the magic code,
  *          the command class and the opcode
  *
  * @return     Returns the header word in the response header
  *             containing  the magic code, command class and opcode
  *
  */
extern inline uint32_t sbeBuildRespHeaderMagicCodeCmdClass (void)
{
    return ( (0xC0DE0000) |
             (uint32_t)(g_sbeFifoCmdHdr.cmdClass << 8) |
             (uint32_t)(g_sbeFifoCmdHdr.command));
}

/**
  * @brief sbeBuildRespHeaderStatusWordGlobal
  *          Builds the status header word from global variables
  *
  * @return     Returns the status word in the response header
  *
  */
extern inline uint32_t sbeBuildRespHeaderStatusWordGlobal (void)
{
    return ( (((uint32_t)g_sbeCmdRespHdr.prim_status)<<16) |
                        (g_sbeCmdRespHdr.sec_status) );
}

/**
  * @brief sbeBuildRespHeaderStatusWordLocal
  *          Builds the status header word from passed in parameters
  *
  * @param[in]  const uint16_t i_primStatus Primary Response Status Code
  * @param[in]  const uint16_t i_secStatus  Secondary Response Status Code
  *
  * @return     Returns the status word in the response header
  *
  */
extern inline uint32_t sbeBuildRespHeaderStatusWordLocal (
                                const uint16_t i_primStatus,
                                const uint16_t i_secStatus)
{
    return ( (((uint32_t)i_primStatus)<<16) | (i_secStatus) );
}

/**
 * @brief sbeBuildMinRespHdr : Builds minimum response header
 *
 * @desc  This builds the buffer with the following status words
 *           - Magic Bytes, Command Class, Command opcode
 *           - Primary Status Code, Secondary Status Code
 *           - PCB / PIB Status Code [optional]
 *           - Distance to Status Header
 * @param[in/out]    uint32_t  *io_pBuf       Buffer to be filled in
 * @param[in/out]    uint32_t  &io_curIndex   Current Index into the buffer
 * @param[in]  const uint16_t  i_primStatus   Primary Response Status Code
 * @param[in]  const uint16_t  i_secStatus    Secondary Response Status Code
 * @param[in]  const uint32_t  i_pcbpibStatus PCB-PIB Response Status Code
 * @param[in]  const uint32_t  i_startIndex   Starting Index into the buffer
 */

void sbeBuildMinRespHdr ( uint32_t  *io_pBuf,
                          uint32_t  &io_curIndex,
                    const uint16_t  i_primStatus,
                    const uint16_t  i_secStatus,
                    const uint32_t  i_pcbpibStatus,
                    const uint32_t  i_startIndex = 0 );


#endif // __SBEFW_SBEFIFOMSGUTILS_H

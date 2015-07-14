/*
 * @file: ppe/sbe/sbefw/sbecmdscomaccess.C
 *
 * @brief This file contains the SBE SCOM Access chipOps
 *
 */

#include "sbecmdscomaccess.H"
#include "sbefifo.H"
#include "sbe_sp_intf.H"
#include "sbetrace.H"
#include "sbeFifoMsgUtils.H"


//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
uint32_t sbeGetScom (uint8_t *i_pArg)
{
    #define SBE_FUNC " sbeGetScom "
    SBE_ENTER(SBE_FUNC);

    uint32_t l_rc = SBE_SEC_OPERATION_SUCCESSFUL;

    do
    {
        uint16_t l_primStatus = g_sbeCmdRespHdr.prim_status;
        uint16_t l_secStatus  = g_sbeCmdRespHdr.sec_status ;

        // Will attempt to dequeue two entries for
        // the scom addresses plus the expected
        // EOT entry at the end

        uint32_t  l_len2dequeue  = 2;
        uint32_t l_scomAddr[2] = {0};
        l_rc = sbeUpFifoDeq_mult (l_len2dequeue, &l_scomAddr[0]);

        // If FIFO access failure
        if (l_rc != SBE_SEC_OPERATION_SUCCESSFUL)
        {
            // Let command processor routine to handle the RC.
            break;
        }

        uint32_t l_sbeDownFifoRespBuf[6] = {0};
        uint32_t l_pcbpibStatus = SBE_PCB_PIB_ERROR_NONE;
        uint32_t l_len2enqueue  = 0;
        uint32_t l_index = 0;

        // @TODO via RTC : 126140
        //       Support Indirect SCOM
        // Data entry 1 : Scom Register Address (0..31)
        // Data entry 2 : Register Address (32..63)
        // For Direct SCOM, will ignore entry 1

        uint64_t l_scomData = 0;
        SBE_DEBUG(SBE_FUNC"scomAddr1[0x%08X]", l_scomAddr[1]);
        l_rc = getscom (0, l_scomAddr[1], &l_scomData);

        if (l_rc) // scom failed
        {
            SBE_ERROR(SBE_FUNC"getscom failed, l_rc[0x%08X]", l_rc);
            l_primStatus = SBE_PRI_GENERIC_EXECUTION_FAILURE;
            l_secStatus  = SBE_SEC_GENERIC_FAILURE_IN_EXECUTION;
            l_pcbpibStatus = l_rc;
        }
        else // successful scom
        {
            SBE_DEBUG(SBE_FUNC"getscom succeeds, l_scomData[0x%X]",
                                   l_scomData);

            l_sbeDownFifoRespBuf[0] = (uint32_t)(l_scomData>>32);
            l_sbeDownFifoRespBuf[1] = (uint32_t)(l_scomData);

            // Push the data into downstream FIFO
            l_len2enqueue = 2;
            l_rc = sbeDownFifoEnq_mult (l_len2enqueue,
                                      &l_sbeDownFifoRespBuf[0]);
            if (l_rc)
            {
                // will let command processor routine
                // handle the failure
                break;
            }
            l_index = 2;
        } // end successful scom

        // Build the response header packet

        uint32_t l_curIndex = l_index ;
        sbeBuildMinRespHdr(&l_sbeDownFifoRespBuf[0],
                            l_curIndex,
                            l_primStatus,
                            l_secStatus,
                            l_pcbpibStatus,
                            l_index);

        // Now enqueue into the downstream FIFO
        l_len2enqueue = ++l_curIndex - l_index;
        l_rc = sbeDownFifoEnq_mult (l_len2enqueue,
                               &l_sbeDownFifoRespBuf[l_index]);
        if (l_rc)
        {
           // will let command processor routine
           // handle the failure
           break;
        }

    } while(false);

    return l_rc;
    #undef SBE_FUNC
}

/////////////////////////////////////////////////////
//////////////////////////////////////////////////////
uint32_t sbePutScom (uint8_t *i_pArg)
{
    #define SBE_FUNC " sbePutScom "
    SBE_ENTER(SBE_FUNC);

    uint32_t l_rc = SBE_SEC_OPERATION_SUCCESSFUL;

    do
    {
        uint16_t l_primStatus = g_sbeCmdRespHdr.prim_status;
        uint16_t l_secStatus  = g_sbeCmdRespHdr.sec_status ;

        // Will attempt to dequeue four entries for
        // the scom address (two entries) and the
        // corresponding data (two entries) plus
        // the expected EOT entry at the end

        uint32_t  l_len2dequeue  = 4;
        uint32_t l_scomAddr_Data[4] = {0};
        l_rc = sbeUpFifoDeq_mult (l_len2dequeue, &l_scomAddr_Data[0]);

        // If FIFO access failure
        if (l_rc != SBE_SEC_OPERATION_SUCCESSFUL)
        {
            // Let command processor routine to handle the RC.
            break;
        }

        uint64_t l_scomData = 0;
        uint32_t l_sbeDownFifoRespBuf[4] = {0};
        uint32_t l_pcbpibStatus = SBE_PCB_PIB_ERROR_NONE;
        uint32_t l_len2enqueue  = 0;
        // successfully dequeued two entries for
        // scom address followed by the EOT entry

        // @TODO via RTC : 126140
        //       Support Indirect SCOM
        // Data entry 0 : Scom Register Address (0..31)
        // Data entry 1 : Scom Register Address (32..63)
        // Data entry 2 : Scom Register Data (0..31)
        // Data entry 3 : Scom Register Data (32..63)
        // For Direct SCOM, will ignore entry 0
        l_scomData = ((uint64_t)(l_scomAddr_Data[2])<<32)
                           | (l_scomAddr_Data[3]);

        l_rc = putscom (0, l_scomAddr_Data[1], l_scomData);

        if (l_rc) // scom failed
        {
            SBE_ERROR(SBE_FUNC"putscom failed, l_rc[0x%08X]", l_rc);
            SBE_ERROR(SBE_FUNC"putscom failure data, "
                          "scomAddr0[0x%08X], "
                          "scomAddr1[0x%08X], "
                          "scomData0[0x%08X], "
                          "scomData1[0x%08X]",
                          l_scomAddr_Data[0],
                          l_scomAddr_Data[1],
                          l_scomAddr_Data[2],
                          l_scomAddr_Data[3]);
            l_primStatus = SBE_PRI_GENERIC_EXECUTION_FAILURE;
            l_secStatus  = SBE_SEC_GENERIC_FAILURE_IN_EXECUTION;
            l_pcbpibStatus = l_rc;
        }

        // Build the response header packet

        uint32_t  l_curIndex = 0;
        sbeBuildMinRespHdr(&l_sbeDownFifoRespBuf[0],
                            l_curIndex,
                            l_primStatus,
                            l_secStatus,
                            l_pcbpibStatus);

        // Now enqueue into the downstream FIFO
        l_len2enqueue = ++l_curIndex;
        l_rc = sbeDownFifoEnq_mult (l_len2enqueue, &l_sbeDownFifoRespBuf[0]);
        if (l_rc)
        {
           // will let command processor routine
           // handle the failure
           break;
        }

    } while(false);

    return l_rc;
    #undef SBE_FUNC
}

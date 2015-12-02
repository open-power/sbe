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
    sbeGetScomReqMsg_t l_getScomReqMsg;

    do
    {
        uint16_t l_primStatus = g_sbeCmdRespHdr.prim_status;
        uint16_t l_secStatus  = g_sbeCmdRespHdr.sec_status ;

        // Will attempt to dequeue two entries for
        // the scom addresses plus the expected
        // EOT entry at the end

        uint32_t l_len2dequeue  = sizeof(l_getScomReqMsg)/sizeof(uint32_t);
        l_rc = sbeUpFifoDeq_mult (l_len2dequeue, (uint32_t *)&l_getScomReqMsg);

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
        // For Direct SCOM, will ignore Bit 0-31

        uint64_t l_scomData = 0;
        SBE_DEBUG(SBE_FUNC"scomAddrLow[0x%08X]", l_getScomReqMsg.lowAddr);
        l_pcbpibStatus = getscom_abs (l_getScomReqMsg.lowAddr, &l_scomData);

        if (l_pcbpibStatus != SBE_PCB_PIB_ERROR_NONE) // scom failed
        {
            SBE_ERROR(SBE_FUNC"getscom failed, l_pcbpibStatus[0x%08X], "
                "scomAddr[0x%08X]", l_pcbpibStatus, l_getScomReqMsg.lowAddr);
            l_primStatus = SBE_PRI_GENERIC_EXECUTION_FAILURE;
            l_secStatus  = SBE_SEC_GENERIC_FAILURE_IN_EXECUTION;
        }
        else // successful scom
        {
            SBE_DEBUG(SBE_FUNC"getscom succeeds, l_scomData[0x%016X]",
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
    sbePutScomReqMsg_t l_putScomReqMsg;

    do
    {
        uint16_t l_primStatus = g_sbeCmdRespHdr.prim_status;
        uint16_t l_secStatus  = g_sbeCmdRespHdr.sec_status ;

        // Will attempt to dequeue four entries for
        // the scom address (two entries) and the
        // corresponding data (two entries) plus
        // the expected EOT entry at the end

        uint32_t  l_len2dequeue  = sizeof(l_putScomReqMsg)/sizeof(uint32_t);
        l_rc = sbeUpFifoDeq_mult (l_len2dequeue, (uint32_t *)&l_putScomReqMsg);

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

        l_scomData = l_putScomReqMsg.getScomData();

        l_pcbpibStatus = putscom_abs (l_putScomReqMsg.lowAddr, l_scomData);

        if (l_pcbpibStatus != SBE_PCB_PIB_ERROR_NONE) // scom failed
        {
            SBE_ERROR(SBE_FUNC"putscom failed, l_pcbpibStatus[0x%08X]",
                            l_pcbpibStatus);
            SBE_ERROR(SBE_FUNC"putscom failure data, "
                          "scomAddr[0x%08X%08X], "
                          "scomData[0x%016X]",
                          l_putScomReqMsg.hiAddr,
                          l_putScomReqMsg.lowAddr,
                          l_scomData);
            l_primStatus = SBE_PRI_GENERIC_EXECUTION_FAILURE;
            l_secStatus  = SBE_SEC_GENERIC_FAILURE_IN_EXECUTION;
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


//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
uint32_t sbeModifyScom (uint8_t *i_pArg)
{
    #define SBE_FUNC " sbeModifyScom "
    SBE_ENTER(SBE_FUNC);

    uint32_t l_rc = SBE_SEC_OPERATION_SUCCESSFUL;

    sbeModifyScomReqMsg_t l_modifyScomMsg;

    do
    {
        uint16_t l_primStatus = g_sbeCmdRespHdr.prim_status;
        uint16_t l_secStatus  = g_sbeCmdRespHdr.sec_status ;

        // @TODO via RTC : 128916
        //       Use structures for payload entries

        // Will attempt to dequeue the following entries:
        // Entry 1 : Operation Mode
        // Entry 2 : Scom Register Address (0..31)
        // Entry 3 : Scom Register Address (32..63)
        // Entry 4 : Modifying Data (0..31)
        // Entry 5 : Modifying Data (32..63)
        // Entry 6 : EOT entry at the end

        uint32_t  l_len2dequeue  = sizeof(l_modifyScomMsg)/sizeof(uint32_t);
        l_rc = sbeUpFifoDeq_mult (l_len2dequeue, (uint32_t *)&l_modifyScomMsg);

        // If FIFO access failure
        if (l_rc != SBE_SEC_OPERATION_SUCCESSFUL)
        {
            // Let command processor routine to handle the RC.
            break;
        }

        uint32_t l_sbeDownFifoRespBuf[4] = {0};
        uint32_t l_pcbpibStatus = SBE_PCB_PIB_ERROR_NONE;
        uint32_t  l_len2enqueue  = 0;

        // @TODO via RTC : 126140
        //       Support Indirect SCOM

        // Modifying Data
        uint64_t l_modifyingData = l_modifyScomMsg.getModifyingData();

        SBE_DEBUG(SBE_FUNC"OpMode[0x%02X], modifyingData[0x%016X]",
                    l_modifyScomMsg.opMode, l_modifyingData);

        // The following steps need to be done as part of this command :
        //    1. Read Register Data (getscom)
        //    2. 'AND' the Mask with the data read from register
        //    3. 'OR' the modifying data with the result of step 2
        //    4. Write the result of step 3 into the register (putscom)
        do
        {
                // Check for a valid OpMode
                if ( (l_modifyScomMsg.opMode != SBE_MODIFY_MODE_OR)   &&
                     (l_modifyScomMsg.opMode != SBE_MODIFY_MODE_AND)  &&
                     (l_modifyScomMsg.opMode != SBE_MODIFY_MODE_XOR) )
                {
                    // Invalid Data passed
                    SBE_ERROR(SBE_FUNC"Invalid OpMode");
                    l_primStatus = SBE_PRI_INVALID_DATA;
                    l_secStatus  = SBE_SEC_GENERIC_FAILURE_IN_EXECUTION;
                    break;
                }

                uint64_t l_scomData = 0;
                l_pcbpibStatus = getscom_abs (l_modifyScomMsg.lowAddr,
                                              &l_scomData);

                if (l_pcbpibStatus != SBE_PCB_PIB_ERROR_NONE) // scom failed
                {
                    SBE_ERROR(SBE_FUNC"getscom failed, l_pcbpibStatus[0x%08X],"
                        " ScomAddress[0x%08X%08X]", l_pcbpibStatus,
                        l_modifyScomMsg.hiAddr, l_modifyScomMsg.lowAddr);
                    l_primStatus = SBE_PRI_GENERIC_EXECUTION_FAILURE;
                    l_secStatus  = SBE_SEC_GENERIC_FAILURE_IN_EXECUTION;
                    break;
                }

                if (l_modifyScomMsg.opMode == SBE_MODIFY_MODE_OR)
                {
                    l_modifyingData |= l_scomData;
                }
                else if (l_modifyScomMsg.opMode == SBE_MODIFY_MODE_AND)
                {
                    l_modifyingData &= l_scomData;
                }
                else
                {
                    l_modifyingData ^= l_scomData;
                }

                // Write the modified data
                l_pcbpibStatus = putscom_abs (l_modifyScomMsg.lowAddr,
                                              l_modifyingData);

                if (l_pcbpibStatus != SBE_PCB_PIB_ERROR_NONE) // scom failed
                {
                    SBE_ERROR(SBE_FUNC"putscom failed, l_pcbpibStatus[0x%08X],"
                        " ScomAddress[0x%08X%08X]", l_pcbpibStatus,
                        l_modifyScomMsg.hiAddr, l_modifyScomMsg.lowAddr);
                    SBE_ERROR(SBE_FUNC"modifyingData[0x%016X]",l_modifyingData);
                    l_primStatus = SBE_PRI_GENERIC_EXECUTION_FAILURE;
                    l_secStatus  = SBE_SEC_GENERIC_FAILURE_IN_EXECUTION;
                    break;
                }
        } while (false);

        // Build the response header packet

        uint32_t  l_curIndex = 0;
        sbeBuildMinRespHdr(&l_sbeDownFifoRespBuf[0],
                            l_curIndex,
                            l_primStatus,
                            l_secStatus,
                            l_pcbpibStatus);

        // Now enqueue into the downstream FIFO
        l_len2enqueue = ++l_curIndex;
        l_rc = sbeDownFifoEnq_mult (l_len2enqueue,
                                   (uint32_t *)&l_sbeDownFifoRespBuf);
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
uint32_t sbePutScomUnderMask (uint8_t *i_pArg)
{
    #define SBE_FUNC " sbePutScomUnderMask "
    SBE_ENTER(SBE_FUNC);

    uint32_t l_rc = SBE_SEC_OPERATION_SUCCESSFUL;
    sbePutScomUnderMaskReqMsg_t l_putScomUmaskMsg;

    do
    {
        uint16_t l_primStatus = g_sbeCmdRespHdr.prim_status;
        uint16_t l_secStatus  = g_sbeCmdRespHdr.sec_status ;

        // @TODO via RTC : 128916
        //       Use structures for payload entries

        // Will attempt to dequeue the following entries:
        // Entry 1 : Scom Register Address (0..31)
        // Entry 2 : Scom Register Address (32..63)
        // Entry 3 : Modifying Data (0..31)
        // Entry 4 : Modifying Data (32..63)
        // Entry 5 : Mask Data (0..31)
        // Entry 6 : Mask Data (32..63)
        // Entry 7 : EOT entry at the end

        uint32_t  l_len2dequeue  = sizeof(l_putScomUmaskMsg)/sizeof(uint32_t);
        l_rc = sbeUpFifoDeq_mult (l_len2dequeue,
                                  (uint32_t *)&l_putScomUmaskMsg);

        // If FIFO access failure
        if (l_rc != SBE_SEC_OPERATION_SUCCESSFUL)
        {
            // Let command processor routine to handle the RC.
            break;
        }

        uint32_t l_sbeDownFifoRespBuf[4] = {0};
        uint32_t l_pcbpibStatus = SBE_PCB_PIB_ERROR_NONE;
        uint32_t  l_len2enqueue  = 0;

        // @TODO via RTC : 126140
        //       Support Indirect SCOM
        // For Direct SCOM, will ignore entry 1

        SBE_DEBUG(SBE_FUNC"scomAddr[0x%08X%08X],"
                     "modifyingData[0x%08X%08X]",
                      l_putScomUmaskMsg.hiAddr,
                      l_putScomUmaskMsg.lowAddr,
                      l_putScomUmaskMsg.hiInputData,
                      l_putScomUmaskMsg.lowInputData);
        SBE_DEBUG(SBE_FUNC"maskData[0x%08X%08X]",
                      l_putScomUmaskMsg.hiMaskData,
                      l_putScomUmaskMsg.lowMaskData);

        // PutScomUnderMask formula:
        // dest_reg = (dest_reg & ~input_mask) | (input_data & input_mask)

        do
        {
            uint64_t l_scomData = 0;

            l_pcbpibStatus = getscom_abs (l_putScomUmaskMsg.lowAddr,
                                          &l_scomData);
            if (l_pcbpibStatus == SBE_PCB_PIB_ERROR_NONE) // getscom succeeded
            {
                l_putScomUmaskMsg.getScomData(l_scomData);

                // Write the modified data
                l_pcbpibStatus = putscom_abs (l_putScomUmaskMsg.lowAddr,
                                              l_scomData);
            }

            if (l_pcbpibStatus != SBE_PCB_PIB_ERROR_NONE) // scom failed
            {
                SBE_ERROR(SBE_FUNC"scom failed, l_pcbpibStatus[0x%08X], "
                    "ScomAddress[0x%08X%08X]", l_pcbpibStatus,
                    l_putScomUmaskMsg.hiAddr,
                    l_putScomUmaskMsg.lowAddr);
                SBE_ERROR(SBE_FUNC"modifyingData[0x%08X%08X]"
                    "maskData[0x%08X%08X]",
                    l_putScomUmaskMsg.hiInputData,
                    l_putScomUmaskMsg.lowInputData,
                    l_putScomUmaskMsg.hiMaskData,
                    l_putScomUmaskMsg.lowMaskData);

                l_primStatus = SBE_PRI_GENERIC_EXECUTION_FAILURE;
                l_secStatus  = SBE_SEC_GENERIC_FAILURE_IN_EXECUTION;
                break;
            }
        } while (false);

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

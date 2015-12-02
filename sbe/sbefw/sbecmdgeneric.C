/*
 * @file: ppe/sbe/sbefw/sbecmdgeneric.C
 *
 * @brief This file contains the SBE generic Commands
 *
 */

#include "sbecmdgeneric.H"
#include "sbefifo.H"
#include "sbetrace.H"
#include "sbe_sp_intf.H"
#include "sbe_build_info.H"
#include "sbeFifoMsgUtils.H"

// Forward declaration
sbeCapabilityRespMsg::sbeCapabilityRespMsg()
{
    verMajor= SBE_FW_MAJOR_VERSION;
    verMinor = SBE_FW_MINOR_VERSION;
    fwCommitId = SBE_COMMIT_ID;
    // We can remove this for llop once all capabilities
    // are supported
    for(uint32_t idx = 0; idx < SBE_MAX_CAPABILITIES; idx++ )
    {
        capability[idx] = 0;
    }
    capability[IPL_CAPABILITY_START_IDX] =
                                EXECUTE_ISTEP_SUPPPORTED;

    capability[SCOM_CAPABILITY_START_IDX] =
                                GET_SCOM_SUPPPORTED |
                                PUT_SCOM_SUPPPORTED |
                                MODIFY_SCOM_SUPPPORTED |
                                PUT_SCOM_UNDER_MASK_SUPPPORTED ;

    capability[GENERIC_CHIPOP_CAPABILITY_START_IDX] =
                                GET_SBE_CAPABILITIES_SUPPPORTED;
}
// Functions
//----------------------------------------------------------------------------
uint32_t sbeGetCapabilities (uint8_t *i_pArg)
{
    #define SBE_FUNC "sbeGetCapabilities "
    SBE_DEBUG(SBE_FUNC);
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
    uint32_t len = 0;
    sbeResponseGenericHeader_t respHdr;
    respHdr.init();
    sbeCapabilityRespMsg_t capMsg;

    do
    {
        // Dequeue the EOT entry as no more data is expected.
        rc = sbeUpFifoDeq_mult (len, NULL);
        // @TODO via RTC : 130575
        // Optimize both the RC handling and
        // FIFO operation infrastructure.
        if ( rc != SBE_SEC_OPERATION_SUCCESSFUL )
        {
            // Let command processor routine to handle the RC
            break;
        }

        len = sizeof(capMsg)/sizeof(uint32_t);
        rc = sbeDownFifoEnq_mult ( len, ( uint32_t *) &capMsg);
        if (rc)
        {
            break;
        }

        uint32_t distance = 1; //initialise by 1 for entry count itself.
        len = sizeof(respHdr)/sizeof(uint32_t);
        rc = sbeDownFifoEnq_mult ( len, ( uint32_t *) &respHdr);
        if (rc)
        {
            break;
        }
        distance += len;

        len = sizeof(distance)/sizeof(uint32_t);
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

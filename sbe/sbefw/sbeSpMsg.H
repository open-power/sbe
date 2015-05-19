/*
 * @file: ppe/sbe/sbefw/sbeSpMsg.H
 *
 * @brief This file contains the message structures for FIFO
 *        communication.
 *
 */

#ifndef __SBEFW_SBESP_MSG_H
#define __SBEFW_SBESP_MSG_H

// @NOTE Make sure all FIFO structures are 32 bit alligned ( the largest
// member should be atleast 4 byte). It is required as in sbe fifo
// operation we are casting these structures to uint32_t pointer. It can
// cause alignment issue if largest member of structure is not atleast
// 32 bit. We can use bit fields to optimize  memory requirements.
// These are two coding guidleines we will follow for this file
// 1. All data members less than 32 bits will be bit fields
// 2. All data members more than 32 buts will be divided into small
//    members of 32 bit each. This is required as compiler pads structure
//    to largest data member and we do not want extra padding for data
//    members gretater than 32 bits ( e.g. uint64_t )
/**
  * @brief Command Request Header
  */
typedef struct
{
    uint32_t len;
    uint32_t reserved:16;
    uint32_t cmdClass:8;
    uint32_t command:8;
}sbeCmdReqBuf_t;

extern sbeCmdReqBuf_t g_sbeCmdHdr;

/**
  * @brief structure for generic header for fifo response.
  *
  */
typedef struct
{
    uint32_t magicCode:16;
    uint32_t cmdClass:8;
    uint32_t command:8;
    uint32_t primaryStatus:16;
    uint32_t secondaryStatus:16;

    /**
      * @brief set the primary and secondary status
      *
      * @param[in] i_prim  Primary status
      * @param[in] i_sec   Secondary status
      *
      * @return
      */
    void setStatus( const uint16_t i_prim, const uint16_t i_sec)
    {
        primaryStatus = i_prim;
        secondaryStatus = i_sec;
    }

    /**
      * @brief set initial values for response header
      *
      * @note  We did not set this in constructor as based on use case
      *        it is possible that g_sbeCmdHdr does not have proper
      *        values at time of object creation.
      *
      */
    void init()
    {
        magicCode = 0xC0DE;
        cmdClass  = g_sbeCmdHdr.cmdClass;
        command = g_sbeCmdHdr.command;
        primaryStatus = SBE_PRI_OPERATION_SUCCESSFUL;
        secondaryStatus = SBE_SEC_OPERATION_SUCCESSFUL;
    }

}sbeResponseGenericHeader_t;

/**
  * @brief structure for ffdc header for fifo response.
  *
  */
typedef struct sbeResponseFfdc
{
    uint32_t magicBytes:16;
    uint32_t lenInWords:16;  // length in word( 4 byte )
    uint32_t hiFapiRc;
    uint32_t lowFapiRc;

    /**
      * @brief set rc
      *
      * @param[in] i_rc  FAPI RC
      *
      * @return
      */
    void setRc(const uint64_t i_rc)
    {
        lowFapiRc = uint32_t(i_rc);
        hiFapiRc = uint32_t(i_rc>>32);
    }

    /**
      * @brief return fapiRc
      *
      * @return fapiRc
      */
    uint64_t getRc()
    {
        uint64_t temp = ( (uint64_t)hiFapiRc << 32) | lowFapiRc;
        return temp;
    }

    /**
      * @brief constructor
      *
      * @param[in] i_rc  FAPI RC
      *
      * @return
      */
    sbeResponseFfdc()
    {
        magicBytes = 0xFFDC;
        //TODO via 129076.
        //Need to change value for length once FFDC design is final.
        lenInWords  = ( sizeof(uint32_t )    // For magicBytes + lenInWords
                        + sizeof(lowFapiRc) + sizeof(hiFapiRc) )
                           / sizeof(uint32_t);
        lowFapiRc = 0;
        hiFapiRc = 0;
    }
}sbeResponseFfdc_t;

/**
  * @brief structure for execute istep chipop (0xA101) contents.
  *
  */
typedef struct
{
    uint32_t reserved1:8;
    uint32_t major:8;
    uint32_t reserved2:8;
    uint32_t minor:8;
}sbeIstepReqMsg_t;

//  TODO via RTC 128658
//  We may be able to replace this structure by sbeResponseGenericHeader_t

/**
  * @brief Command response structure to hold the primary and secondary
  *        status values. This will be utilized when a command class
  *        validation or state machine check fails.
  *
  */
typedef struct
{
    uint32_t          prim_status:16 ;    // Primary Response Status
    uint32_t          sec_status:16  ;    // Secondary Response Status
} sbeCmdRespHdr_t;

extern sbeCmdRespHdr_t g_sbeCmdRespHdr;

#endif // __SBEFW_SBESP_MSG_H

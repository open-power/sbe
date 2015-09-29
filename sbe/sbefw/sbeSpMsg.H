/*
 * @file: ppe/sbe/sbefw/sbeSpMsg.H
 *
 * @brief This file contains the message structures for FIFO
 *        communication.
 *
 */

#ifndef __SBEFW_SBESP_MSG_H
#define __SBEFW_SBESP_MSG_H

#include "sbe_sp_intf.H"

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


/**
  * @brief structure for GetScom Chipop (0xA201) contents.
  *
  */
typedef struct
{
    uint32_t hiAddr;
    uint32_t lowAddr;
}sbeGetScomReqMsg_t;

/**
  * @brief structure for PutScom Chipop (0xA202) contents.
  *
  */
typedef struct
{
    uint32_t hiAddr;
    uint32_t lowAddr;
    uint32_t hiInputData;
    uint32_t lowInputData;

    /**
      * @brief return 64-bit Scom data
      *
      * @return 64-bit Scom data
      */
    uint64_t getScomData()
    {
        uint64_t data = ((uint64_t)hiInputData << 32) | lowInputData;
        return data;
    }
}sbePutScomReqMsg_t;

/**
  * @brief structure for Modify_Scom Chipop (0xA203) contents.
  *
  */
typedef struct
{
    uint32_t reserved:24;
    uint32_t opMode:8;
    uint32_t hiAddr;
    uint32_t lowAddr;
    uint32_t hiInputData;
    uint32_t lowInputData;

    /**
      * @brief return 64-bit modifying data
      *
      * @return 64-bit modifying data
      */
    uint64_t getModifyingData()
    {
        uint64_t data = ((uint64_t)hiInputData << 32) | lowInputData;
        return data;
    }
}sbeModifyScomReqMsg_t;

/**
  * @brief structure for PutScom_UnderMask Chipop (0xA204) contents.
  *
  */
typedef struct
{
    uint32_t hiAddr;
    uint32_t lowAddr;
    uint32_t hiInputData;
    uint32_t lowInputData;
    uint32_t hiMaskData;
    uint32_t lowMaskData;

    /**
      * @brief return 64-bit input data
      *
      * @return 64-bit input data
      */
    uint64_t getInputData()
    {
        uint64_t data = ((uint64_t)hiInputData << 32) | lowInputData;
        return data;
    }

    /**
      * @brief return 64-bit input mask
      *
      * @return 64-bit input mask
      */
    uint64_t getInputMask()
    {
        uint64_t data = ((uint64_t)hiMaskData << 32) | lowMaskData;
        return data;
    }

    /**
      * @brief Determines 64-bit Scom data
      *
      * @param[in/out] io_scomData 64-bit scom data
      */
    void getScomData(uint64_t &io_scomData)
    {
        uint64_t l_inputMask = getInputMask();
        uint64_t l_inputData = getInputData();
        io_scomData = (io_scomData & (~l_inputMask))
                    | (l_inputData & l_inputMask);
    }
}sbePutScomUnderMaskReqMsg_t;

/**
 * @brief Structure for SBE Memory Access ChipOps (0xA401/A402)
 *
 */
typedef struct
{
    uint32_t flags;  // Operational Flags
    uint32_t addrHi; // Higher 32-Bit Memory Address
    uint32_t addrLo; // Lower  32-Bit Memory Address
    uint32_t len;    // Length of Data in Bytes

    /**
      * @brief Calculates 64-bit PBA Address
      *
      * @return Return 64-bit PBA address
      */
    uint64_t getPbaAddr()
    {
        uint64_t addr = ((uint64_t)addrHi << 32) | addrLo;
        return addr;
    }

    /**
      * @brief Determines if PBA flag is set
      *
      * @return Returns True if PBA Flag is set
      *                 False if PBA flag is not set
      */
    bool isPbaFlagSet()
    {
        bool tmp_flag = (flags & SBE_MEM_ACCESS_FLAGS_TARGET_PBA) ?
                           true : false ;
        return tmp_flag;
    }

    /**
      * @brief Determines if Auto Increment Mode is set
      *
      * @return Returns True if Auto Increment mode is set
      *                 False if Auto Increment is not set
      */
    bool isAutoIncrModeSet()
    {
        bool tmp_flag = (flags & SBE_MEM_ACCESS_FLAGS_AUTO_INCR_ON) ?
                           true : false ;
        return tmp_flag;
    }

    /**
      * @brief Determines if Fast Mode is set
      *
      * @return Returns True if Fast mode is set
      *                 False if Fast mode is not set
      */
    uint32_t isFastModeSet()
    {
        uint32_t tmp_flag = (flags & SBE_MEM_ACCESS_FLAGS_FAST_MODE_ON) ?
                             (1<<31) : 0;
        return tmp_flag;
    }

    /**
      * @brief Calculates Data length in alignment with PBA Cacheline (128B)
      *
      * @return Returns Data length in alignment with PBA Cacheline
      */
    uint64_t getDataLenPbaCacheAlign()
    {
        uint64_t l_len = (len / 8) / 16;
        return l_len;
    }
}sbeMemAccessReqMsgHdr_t;


// Maximum number of capabilities
static const uint32_t SBE_MAX_CAPABILITIES = 18;

/**
  * @brief structure for SBE Get Capabilities chipop (0xA802) contents.
  *
  */
typedef struct sbeCapabilityRespMsg
{
    uint32_t verMajor:16;
    uint32_t verMinor:16;
    uint32_t fwCommitId;
    uint32_t capability[SBE_MAX_CAPABILITIES];
    // ctor. constructor will initialise all values.
    sbeCapabilityRespMsg();
}sbeCapabilityRespMsg_t;

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

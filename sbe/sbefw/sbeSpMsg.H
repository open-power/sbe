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
}sbeFifoCmdReqBuf_t;

extern sbeFifoCmdReqBuf_t g_sbeFifoCmdHdr;

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
      *        it is possible that g_sbeFifoCmdHdr does not have proper
      *        values at time of object creation.
      *
      */
    void init()
    {
        magicCode = 0xC0DE;
        cmdClass  = g_sbeFifoCmdHdr.cmdClass;
        command = g_sbeFifoCmdHdr.command;
        primaryStatus = SBE_PRI_OPERATION_SUCCESSFUL;
        secondaryStatus = SBE_SEC_OPERATION_SUCCESSFUL;
    }

}sbeRespGenHdr_t;

/**
  * @brief structure for generic Resp header with Len for fifo response.
  *
  */
typedef struct
{
    uint32_t respLen;
    sbeRespGenHdr_t respHdr;

    /**
      * @brief set response length
      *
      * @param[in] i_len   Length
      *
      * @return
      */
    void setLength( const uint32_t i_len )
    {
        respLen = i_len;
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
        respHdr.init();
    }
}sbeRespGenHdrWithLen_t;


/**
  * @brief structure for ffdc header for fifo response.
  *
  */
typedef struct sbeResponseFfdc
{
    uint32_t magicBytes:16;
    uint32_t lenInWords:16;  // length in word( 4 byte )
    uint32_t hiFapiRc; // Unused since FAPI RC is 32 bit on the SBE
    uint32_t lowFapiRc;

    /**
      * @brief set rc
      *
      * @param[in] i_rc  FAPI RC
      *
      * @return
      */
    void setRc(const uint32_t i_rc)
    {
        lowFapiRc = i_rc;
    }

    /**
      * @brief return fapiRc
      *
      * @return fapiRc
      */
    uint32_t getRc() const
    {
        return lowFapiRc;
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


/**
 * @brief Structure for SBE OCC Get/Put Sram Access ChipOps (0xA403/A404)
 */
typedef struct
{
    uint32_t reserved:24;           // Not used
    sbeSramAccessMode mode:8;       // Channel select 0-3
    uint32_t addr;                  // 32-Bit Memory Address
    uint32_t len;                   // Length of Data in Bytes
}sbeOccSramAccessReqMsgHdr_t;

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
//  We may be able to replace this structure by sbeRespGenHdr_t

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

/**
  * @brief structure for Control Instruction Chipop (0xA701) contents.
  *
  */
typedef struct
{
    uint32_t           reserved:12;
    sbeErrorMode       mode:4;
    sbeCoreChipletId   coreChipletId:8;
    sbeThreadNum       threadNum:4;
    sbeThreadOps       threadOps:4;

    /**
      * @brief Validate input arguments
      *
      * @return bool, true if the validation is success, else false for
      * validation failure
      */
    bool validateInputArgs()
    {
        bool l_validatePassFlag = true;
        // Validate Thread Command / Thread Num / Error Mode
        if((threadOps > THREAD_SRESET_INS) ||
           (mode > IGNORE_HW_ERRORS) ||
           !((threadNum <= SMT4_THREAD3) || (threadNum == SMT4_THREAD_ALL)))
        {
            SBE_ERROR(SBE_FUNC "Invalid Parameter by User, ThreadOps[%d] "
                "mode[%d] ThreadNum[%d]", threadOps, mode, threadNum);
            l_validatePassFlag = false;
        }
        return l_validatePassFlag; 
    }

    /**
      * @brief Process the input to find out core/thread ids to iterate
      *        over HWP for the internal business logic
      *
      * @param[out] o_core, Core Id to start with in the iteration
      * @param[out] o_coreCntMax, Core Max count to iterate, start from o_core
      * @param[out] o_threadCnt, Thread Num to start with in the iteration
      * @param[out] o_threadCntMax, Thread Max Num to iterate, start from
      *             o_threadCnt
      *
      * @return void
      */
    void processInputDataToIterate(uint8_t & o_core, uint8_t & o_coreCntMax,
                               uint8_t & o_threadCnt, uint8_t & o_threadCntMax)
    {
        //Default Init
        o_threadCnt = SMT4_THREAD0;
        o_threadCntMax = SMT4_THREAD_MAX;

        o_core = SMT4_CORE0_ID;
        o_coreCntMax = SMT4_CORE_ID_MAX;

        if( SMT4_ALL_CORES != coreChipletId )
        {
            o_core = coreChipletId;
            o_coreCntMax = coreChipletId;
        }
        if( SMT4_THREAD_ALL != threadNum )
        {
            o_threadCnt = threadNum;
            o_threadCntMax = threadNum;
        }
    }
}sbeCntlInstRegMsgHdr_t;
/**
  * @brief Reg access message header
  */
typedef struct
{
    uint32_t reserved:8;
    uint32_t coreChiplet:8;
    uint32_t threadNr:4;
    uint32_t regType:4;
    uint32_t numRegs:8;

    /**
      * @brief checks if it is valid request.
      *
      * @return true if valid request, false otherwise
      */
    bool isValidRequest() const
    {
        return (( SBE_REG_ACCESS_FPR >= regType )
                &&( SBE_MAX_REG_ACCESS_REGS >= numRegs )
                &&( SMT4_THREAD3 >= threadNr )
                &&( SMT4_CORE0_ID <= coreChiplet )
                &&( SMT4_CORE_ID_MAX >= coreChiplet )) ? true:false;
    }
}sbeRegAccessMsgHdr_t;

/**
  * @brief reg scom package
  */
typedef struct
{
    uint32_t regNr;
    uint32_t hiData;
    uint32_t lowData;

    /**
      * @brief data for a register.
      *
      * @return data.
      */
    uint64_t getData() const
    {
        return (((uint64_t)hiData << 32 ) | lowData );
    }
}sbeRegAccessPackage_t;

#endif // __SBEFW_SBESP_MSG_H

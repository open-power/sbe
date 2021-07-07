/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/hwpf/plat_i2c_access.C $                                  */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2018,2021                        */
/* [+] International Business Machines Corp.                              */
/*                                                                        */
/*                                                                        */
/* Licensed under the Apache License, Version 2.0 (the "License");        */
/* you may not use this file except in compliance with the License.       */
/* You may obtain a copy of the License at                                */
/*                                                                        */
/*     http://www.apache.org/licenses/LICENSE-2.0                         */
/*                                                                        */
/* Unless required by applicable law or agreed to in writing, software    */
/* distributed under the License is distributed on an "AS IS" BASIS,      */
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or        */
/* implied. See the License for the specific language governing           */
/* permissions and limitations under the License.                         */
/*                                                                        */
/* IBM_PROLOG_END_TAG                                                     */
#include "plat_i2c_access.H"
#include "exp_i2c_scom.H"
#include "sbeutil.H"

////////////////////////////////I2C Driver//////////////////////////////////////
using namespace fapi2;
#define I2C_MASTER_PIB_BASE_ADDR_LEGACY_MODE 0xA0004
//TODO: This has to be updated.
#define P9_ENGINE_SCOM_OFFSET 0x1000
#define SIM_INTERVAL_DELAY  320000

/**
 * @brief I2C Register Offsets
 */
enum i2c_reg_offset_t
{
    I2C_REG_FIFO        = 0,
    I2C_REG_COMMAND     = 1,
    I2C_REG_MODE        = 2,
    I2C_REG_INTMASK     = 4,
    I2C_REG_INTERRUPT   = 6,
    I2C_REG_STATUS      = 7,
    I2C_REG_RESET       = 7,
    I2C_REG_RESET_ERRORS= 8,
    I2C_REG_SET_SCL     = 9,
    I2C_REG_RESET_SCL   = 11,
    I2C_REG_SET_SDA     = 12,
    I2C_REG_RESET_SDA   = 13,
    I2C_REG_ATOMIC_LOCK = 0x3FB,

};

/**
 * @brief Status register definition
 */
union status_reg_t
{
    uint64_t value;
    struct
    {
        uint64_t invalid_cmd : 1;
        uint64_t lbus_parity_error : 1;
        uint64_t backend_overrun_error : 1;
        uint64_t backend_access_error : 1;
        uint64_t arbitration_lost_error : 1;
        uint64_t nack_received : 1;
        uint64_t data_request : 1;
        uint64_t command_complete : 1;
        uint64_t stop_error : 1;
        uint64_t upper_threshold : 7;
        uint64_t any_i2c_interrupt : 1;
        uint64_t waiting_for_i2c_busy : 1;
        uint64_t error_in : 1;
        uint64_t i2c_port_history_busy : 1;
        uint64_t scl_input_level : 1;
        uint64_t sda_input_level : 1;
        uint64_t i2c_port_busy : 1;
        uint64_t i2c_interface_busy : 1;
        uint64_t fifo_entry_count : 8;
        uint64_t padding : 32;
    };
};

/**
 * @brief Interrupt register definition
 */
union interrupt_reg_t
{
    uint64_t value;
    struct
    {
        uint64_t reserved0 : 16;
        uint64_t invalid_cmd : 1;
        uint64_t lbus_parity_error : 1;
        uint64_t backend_overrun_error : 1;
        uint64_t backend_access_error : 1;
        uint64_t arbitration_lost_error : 1;
        uint64_t nack_received_error : 1;
        uint64_t data_request : 1;
        uint64_t command_complete : 1;
        uint64_t stop_error : 1;
        uint64_t i2c_busy : 1;
        uint64_t not_i2c_busy : 1;
        uint64_t reserved1 : 1;
        uint64_t scl_eq_1 : 1;
        uint64_t scl_eq_0 : 1;
        uint64_t sda_eq_1 : 1;
        uint64_t sda_eq_0 : 1;
        uint64_t padding: 32;
    };
};

/**
 * @brief I2C Command register definition
 */
union command_reg_t
{
    uint64_t value;
    struct
    {
        uint64_t with_start : 1;
        uint64_t with_addr : 1;
        uint64_t read_continue : 1; // Not Supported at this time
        uint64_t with_stop : 1;
        uint64_t reserved : 4;
        uint64_t device_addr : 7;
        uint64_t read_not_write : 1;
        uint64_t length_b : 16;
        uint64_t padding : 32;
    };
};

/**
 * @brief I2C Mode register definition
 */
union mode_reg_t
{
    uint64_t value;
    struct
    {
        uint64_t bit_rate_div : 16;
        uint64_t port_num : 6;
        uint64_t reserved : 6;
        uint64_t enhanced_mode : 1;
        uint64_t diag_mode : 1;
        uint64_t pacing_allow_mode : 1;
        uint64_t wrap_mode : 1;
        uint64_t padding : 32;
    };
};

union fifo_reg_t
{
    uint64_t value;
    struct
    {
        uint64_t byte_0 : 8;
        uint64_t padding : 56;
    };
};

enum fifo_condition_t
{
    FIFO_FREE,
    FIFO_DATA_AVAILABLE
};

/**
 * @brief Structure used to pass important variables between functions
 */
struct misc_args_t
{
    uint8_t  port;
    uint8_t  engine;
    uint64_t devAddr;
    bool     skip_mode_setup;
    bool     with_stop;
    bool     read_not_write;
    uint64_t bus_speed;          // in kbits/sec (ie 400KHz)
    uint16_t bit_rate_divisor;   // uint16_t to match size in mode register
    uint64_t polling_interval_ns;// in nanoseconds
    uint64_t timeout_count;
    misc_args_t():port(0x00),
                  engine(0x03),
                  devAddr(0x40),
                  skip_mode_setup(false),
                  with_stop(true),
                  read_not_write(true),
                  bus_speed(0),
                  bit_rate_divisor(0),
                  polling_interval_ns(0),
                  timeout_count(0){}
};

/**
 *  @brief Number of nanoseconds per second
 */
#define NS_PER_SEC (1000000000ull)

/**
 *  @brief Number of nanoseconds per milisecond
 */
#define NS_PER_MSEC (1000000ull)
/**
 * @brief Determine I2C Timeout Count based on I2C_MAX_WAIT_TIME_NS and
 *         I2C Polling Interval (in ns)
 */
#define I2C_MAX_WAIT_TIME_NS (20 * NS_PER_MSEC)
#define I2C_TIMEOUT_COUNT(i_interval_ns) (I2C_MAX_WAIT_TIME_NS / i_interval_ns)
/**
 * @brief FIFO capacity in bytes.  This dictates the maximum number
 *      of bytes that the FIFO can hold.
 */
#define I2C_MAX_FIFO_CAPACITY 8
/**
 * @brief Inline function used to calculate Bit Rate Divisor setting
 *        based on I2C Bus Speed and Nest Frequency
 *
 * @param [in] i_bus_speed_khz  Bus Speed in KHz
 * @param [in] i_local_bus_MHZ  Local Bus that feeds I2C Master's clock
 *
 * @return Bit Rate Divisor value
 */
inline uint16_t i2cGetBitRateDivisor(uint64_t i_bus_speed_khz,
                                     uint64_t i_local_bus_MHZ)
{
  // Use tmp variable to convert everything to KHZ safely
  uint64_t tmp = ( i_local_bus_MHZ  ) * 1000;

  return ( ( ( tmp / i_bus_speed_khz ) - 1 ) / 4 );
}

/**
 * @brief I2C Polling Interval based on bus speed; set to 1/10th of expected
 *        duration
 *
 * NOTE: I2C Bus Speed in KBits/sec, so multiple by 8
 *       since Device Driver works on a byte-level
 *
 * @param [in] i_bus_speed_khz Bus Speed in KHz
 *
 * @return Polling Interval in nanoseconds
 */
inline uint64_t i2cGetPollingInterval(uint64_t i_bus_speed_khz )
{
  // Polling Interval = 8 * (1/bus+speed) * (1/10) -> converted to ns
  return ( ( 8 * NS_PER_SEC ) / ( 10 * i_bus_speed_khz * 1000 ) );
};
/*
 * @brief Perform I2C register operation
 *
 * @param[in]       reg           I2C register offset
 * @param[in]       args          Additional argument
 * @param[in]       readNotWrite  Read/Write
 * @param[in,out]   io_data       uint64_t data buffer
 *
 * @return FAPI RC
 */
ReturnCode i2cRegisterOp(i2c_reg_offset_t reg,
                         misc_args_t &args,
                         bool readNotWrite,
                         uint64_t* io_data)
{
    #define SBE_FUNC "i2cRegisterOp "
    SBE_ENTER(SBE_FUNC "Reg[%02x] Engine[%02x] ReadNotWrite[%d]",
                        reg,
                        args.engine,
                        readNotWrite);

    Target<TARGET_TYPE_PROC_CHIP > procTgt = plat_getChipTarget();
    uint64_t addr = I2C_MASTER_PIB_BASE_ADDR_LEGACY_MODE +
                    (args.engine * P9_ENGINE_SCOM_OFFSET) +
                    reg;
    if(readNotWrite)
    {
        buffer<uint64_t> data;
        FAPI_TRY(getScom(procTgt,
                                addr,
                                data),
                                "Error reading i2c register");
        *io_data = data;
    }
    else
    {
        FAPI_TRY(putScom(procTgt,
                                addr,
                                *io_data),
                                "Error writing i2c register");
    }

    return FAPI2_RC_SUCCESS;

fapi_try_exit:
    SBE_ERROR(SBE_FUNC "Reg[%02x] Engine[%02x] ReadNotWrite[%d]",
                reg,
                args.engine,
                readNotWrite);
    return current_err;
    #undef SBE_FUNC
}

ReturnCode i2cCheckForErrors(misc_args_t &args,
                             status_reg_t &status)
{
    ReturnCode rc = FAPI2_RC_SUCCESS;
    do
    {
        if(status.invalid_cmd ||
           status.lbus_parity_error ||
           status.backend_overrun_error ||
           status.backend_access_error ||
           status.arbitration_lost_error ||
           status.nack_received ||
           status.stop_error)
        {

            rc = RC_I2C_STATUS_ERROR;
            FAPI_ASSERT(false,
                        I2C_STATUS_ERROR()
                        .set_VALUE(status.value)
                        .set_PORT(args.port)
                        .set_ENGINE(args.engine),
                        "I2C Status error");
        }

        if(status.any_i2c_interrupt)
        {
            interrupt_reg_t intreg;
            rc = i2cRegisterOp(I2C_REG_INTERRUPT,
                               args,
                               true,
                               (uint64_t*)&intreg);
            if(rc != FAPI2_RC_SUCCESS)
            {
                break;
            }
            rc = RC_I2C_STATUS_INTR_ERROR;
            FAPI_ASSERT(false,
                        I2C_STATUS_INTR_ERROR()
                        .set_STATUS(status.value)
                        .set_VALUE(intreg.value)
                        .set_PORT(args.port)
                        .set_ENGINE(args.engine),
                        "I2C Interrupt Status error");
        }
    } while(0);

fapi_try_exit:
    return rc;
}

ReturnCode i2cReadStatusReg(misc_args_t &args,
                            status_reg_t &o_status)
{
    #define SBE_FUNC "i2cReadStatusReg "
    ReturnCode rc = FAPI2_RC_SUCCESS;

    do
    {
        rc = i2cRegisterOp(I2C_REG_STATUS,
                           args,
                           true,
                           (uint64_t*)&o_status);
        if(rc != FAPI2_RC_SUCCESS)
        {
            break;
        }
    } while(0);

    return rc;
    #undef SBE_FUNC
}

/*
 * @brief Wait for previous command complete on i2c master
 *
 * @param[in]       args          i2c arguments
 *
 * @return FAPI RC
 */
ReturnCode i2cWaitForCmdComp(misc_args_t &args)
{
    #define SBE_FUNC "i2cWaitForCmdComp "
    ReturnCode rc = FAPI2_RC_SUCCESS;
    status_reg_t status = {};
    uint64_t interval_ns = args.polling_interval_ns;
    uint64_t timeoutCount = args.timeout_count;
    SBE_DEBUG(SBE_FUNC "interval [%d]ns count [%d]", interval_ns, timeoutCount);

    do
    {
        do
        {
            delay(interval_ns,
                  SIM_INTERVAL_DELAY);

            status.value = 0;
            rc = i2cReadStatusReg(args,
                                  status);
            if(rc != FAPI2_RC_SUCCESS)
            {
                break;
            }
            if(--timeoutCount == 0)
            {
                SBE_ERROR(SBE_FUNC "timedout waiting for cmd completion");
                break;
            }
        } while(!status.command_complete);
        if(rc != FAPI2_RC_SUCCESS)
        {
            break;
        }
    } while(0);

    return rc;
    #undef SBE_FUNC
}


/*
 * @brief Setup I2C mode register
 *
 * @param[in]       len    Length to be read/written
 * @param[in,out]   args   i2c arguments
 *
 * @return FAPI RC
 */
ReturnCode i2cSetup(size_t len,
                    misc_args_t &args)
{
    ReturnCode rc = FAPI2_RC_SUCCESS;
    mode_reg_t mode;
    command_reg_t cmd;

    do
    {
        rc = i2cWaitForCmdComp(args);
        if(rc != FAPI2_RC_SUCCESS)
        {
            break;
        }

        if(!args.skip_mode_setup)
        {
            mode.value = 0;
            mode.bit_rate_div = args.bit_rate_divisor;
            mode.port_num = args.port;

            rc = i2cRegisterOp(I2C_REG_MODE,
                               args,
                               false,
                               (uint64_t*)&mode.value);
            if(rc != FAPI2_RC_SUCCESS)
            {
                break;
            }
        }

        // Write Command Register:
        //      - with start
        //      - with stop
        //      - RnW
        //      - length
        cmd.value = 0x0ull;
        cmd.with_start = 1;
        cmd.with_stop = (args.with_stop ? 1 : 0);
        cmd.with_addr = 1;

        // cmd.device_addr is 7 bits
        // devAddr though is a uint64_t
        //  -- value stored in LSB byte of uint64_t
        //  -- LS-bit is unused, creating the 7 bit cmd.device_addr
        //  So will be masking for LSB, and then shifting to push off LS-bit
        cmd.device_addr = (0x000000FF & args.devAddr) >> 1;

        cmd.read_not_write = (args.read_not_write ? 1 : 0);
        cmd.length_b = len;

        rc = i2cRegisterOp(I2C_REG_COMMAND,
                           args,
                           false,
                           (uint64_t*)&cmd.value);
        if(rc != FAPI2_RC_SUCCESS)
        {
            break;
        }

    } while(0);

    return rc;
}

/*
 * @brief Check for a fifo condition on i2c status register
 *
 * @param[in] condition condition to check for
 * @param[in] status    value of i2c status register
 *
 * @return TRUE if condition holds good, else FALSE
 */
bool fifoCondition(fifo_condition_t condition,
                   status_reg_t status)
{
    if(condition == FIFO_DATA_AVAILABLE)
    {
        return ((0 == status.fifo_entry_count) &&
                (0 == status.data_request));
    }
    else if(condition == FIFO_FREE)
    {
        return ((I2C_MAX_FIFO_CAPACITY <= status.fifo_entry_count) &&
                (0 == status.data_request));
    }

    return false;
}

/*
 * @brief Wait for a fifo condition on i2c status register
 *
 * @param[in] condition condition to wait for
 * @param[in] args      i2c arguments
 *
 * @return FAPI RC
 */
ReturnCode i2cWaitForFifo(fifo_condition_t condition,
                          misc_args_t &args)
{
    #define SBE_FUNC "i2cWaitForFifo "
    ReturnCode rc = FAPI2_RC_SUCCESS;
    uint64_t interval_ns = args.polling_interval_ns;
    uint64_t timeoutCount = args.timeout_count;

    status_reg_t status = {};
    do
    {
        rc = i2cReadStatusReg(args, status);
        if(rc != FAPI2_RC_SUCCESS)
        {
            break;
        }

        while(fifoCondition(condition, status))
        {
            delay(interval_ns,
                  SIM_INTERVAL_DELAY);

            status.value = 0;
            rc = i2cReadStatusReg(args, status);
            if(rc != FAPI2_RC_SUCCESS)
            {
                break;
            }
            if(--timeoutCount == 0)
            {
                SBE_ERROR(SBE_FUNC "timedout waiting for fifo condition");
                rc = RC_I2C_FIFO_TIMEOUT_ERROR;
                break;
            }
        }
    } while(0);
    return rc;
    #undef SBE_FUNC
}
/*
 * @brief Set I2C bus variables
 *
 * @param[in]       speed     Speed in kbits/sec
 * @param[in,out]   args      i2c arguments
 *
 * @return FAPI RC
 */
ReturnCode i2cSetBusVariables(uint64_t speed,
                              misc_args_t &args)
{
    args.bus_speed = speed;
    args.polling_interval_ns = i2cGetPollingInterval(speed);
    args.timeout_count = I2C_TIMEOUT_COUNT(args.polling_interval_ns);
    // TODO: In actual read the Freq from the attribute and set it.
    // This will be done while actual testing.
    fapi2::ATTR_FREQ_PAU_MHZ_Type l_attr_freq_pau_mhz = 0;
    const Target<TARGET_TYPE_SYSTEM> FAPI_SYSTEM;
    FAPI_TRY(FAPI_ATTR_GET(ATTR_FREQ_PAU_MHZ, FAPI_SYSTEM,
                                           l_attr_freq_pau_mhz));
    FAPI_IMP("NEST Frequency is 0x%08X", l_attr_freq_pau_mhz);
    args.bit_rate_divisor = i2cGetBitRateDivisor(args.bus_speed,
                                l_attr_freq_pau_mhz/2);
    FAPI_IMP("i2cSetBusVariables: Bus speed 0x%08X %08X", (args.bus_speed >> 32) & 0xFFFFFFFF,
                                                          (args.bus_speed & 0xFFFFFFFF));
    FAPI_IMP("i2cSetBusVariables: Polling interval 0x%08X %08X", (args.polling_interval_ns >> 32) & 0xFFFFFFFF,
                                                          (args.polling_interval_ns & 0xFFFFFFFF));
    FAPI_IMP("i2cSetBusVariables: Time out count 0x%08X %08X", (args.timeout_count >> 32) & 0xFFFFFFFF,
                                                          (args.timeout_count & 0xFFFFFFFF));
    FAPI_IMP("i2cSetBusVariables: Bit rate Divisor 0x%08X ", args.bit_rate_divisor);
    return FAPI2_RC_SUCCESS;
fapi_try_exit:
    return current_err;
}

/*
 * @brief write to a I2C device
 *
 * @param[out] buffer       Buffer with data to be written
 * @param[in]  buflen       Length in bytes to be written
 * @param[in]  args         i2c arguments
 *
 * @return FAPI RC
 */
ReturnCode i2cWrite(void *const buffer,
                    size_t buflen,
                    misc_args_t &args)
{
    #define SBE_FUNC "i2cWrite "
    ReturnCode rc = FAPI2_RC_SUCCESS;

    size_t bytesWritten = buflen;

    fifo_reg_t fifo = {};

    do
    {
        args.read_not_write = false;
        rc = i2cSetup(buflen, args);
        if(rc != FAPI2_RC_SUCCESS)
        {
            break;
        }

        for(bytesWritten = 0; bytesWritten < buflen; bytesWritten++)
        {
            rc = i2cWaitForFifo(FIFO_FREE,
                                args);
            if(rc != FAPI2_RC_SUCCESS)
            {
                break;
            }

            fifo.value = 0;
            // Write data to FIFO
            fifo.byte_0 = *((uint8_t*)buffer + bytesWritten);
            rc = i2cRegisterOp(I2C_REG_FIFO,
                               args,
                               false,
                               (uint64_t*)&fifo);
            if(rc != FAPI2_RC_SUCCESS)
            {
                break;
            }
        }
        if(rc != FAPI2_RC_SUCCESS)
        {
            break;
        }
        rc = i2cWaitForCmdComp(args);
        if(rc != FAPI2_RC_SUCCESS)
        {
            break;
        }
    } while(0);

    // Unlock the I2C Engine E
    SBE::unlockI2CEngineE();

    return rc;
    #undef SBE_FUNC
}

/*
 * @brief Read from a I2C device
 *
 * @param[out] o_buffer     Buffer to hold the read data
 * @param[in]  buflen       Length in bytes to be read
 * @param[in]  args         i2c arguments
 *
 * @return FAPI RC
 */
ReturnCode i2cRead(void *o_buffer,
                   size_t buflen,
                   misc_args_t &args)
{
    #define SBE_FUNC "i2cRead "
    ReturnCode rc = FAPI2_RC_SUCCESS;

    size_t bytesRead = buflen;

    fifo_reg_t fifo = {};

    do
    {
        args.read_not_write = true;
        rc = i2cSetup(buflen, args);
        if(rc != FAPI2_RC_SUCCESS)
        {
            break;
        }

        for(bytesRead = 0; bytesRead < buflen; bytesRead++)
        {
            rc = i2cWaitForFifo(FIFO_DATA_AVAILABLE,
                                args);
            if(rc != FAPI2_RC_SUCCESS)
            {
                break;
            }

            fifo.value = 0;
            rc = i2cRegisterOp(I2C_REG_FIFO,
                               args,
                               true,
                               (uint64_t*)&fifo);
            if(rc != FAPI2_RC_SUCCESS)
            {
                break;
            }

            *((uint8_t*)o_buffer + bytesRead) = fifo.byte_0;
        }
        if(rc != FAPI2_RC_SUCCESS)
        {
            break;
        }
        rc = i2cWaitForCmdComp(args);
        if(rc != FAPI2_RC_SUCCESS)
        {
            break;
        }
    } while(0);

    // Unlock the I2C Engine E
    SBE::unlockI2CEngineE();

    return rc;
    #undef SBE_FUNC
}

/*
 * @brief Lock I2C engine
 *
 * @param[in]  i_args         i2c arguments
 *
 * @return FAPI RC
 */
ReturnCode i2cLockEngine(misc_args_t i_args)
{
    #define SBE_FUNC "i2cLockEngine "
    ReturnCode rc = FAPI2_RC_SUCCESS;
    uint64_t interval_ns = 160000; // 160 us
    uint64_t timeoutCount = 4000/160; // total 4 ms
    uint64_t lock_data = 0x8000000000000000;

    do
    {
        // Clearing the current RC, which might be set in 
        // previous loop, while accessing locked resource
        fapi2::current_err = fapi2::FAPI2_RC_SUCCESS;

        rc = i2cRegisterOp(I2C_REG_ATOMIC_LOCK,
                           i_args,
                           false,
                           &lock_data);
        if(rc != (const uint32_t) RC_SBE_PIB_XSCOM_ERROR)
        {
            break;
        }

        delay(interval_ns, SIM_INTERVAL_DELAY);
    } while(--timeoutCount);
    if(timeoutCount == 0)
    {
        SBE_ERROR(SBE_FUNC "Failed to lock engine: %d", i_args.engine);
    }

    return rc;
    #undef SBE_FUNC
}

/*
 * @brief Unlock I2C engine
 *
 * @param[in]  i_args         i2c arguments
 *
 * @return FAPI RC
 */
ReturnCode i2cUnlockEngine(misc_args_t i_args)
{
    ReturnCode rc = FAPI2_RC_SUCCESS;
    uint64_t unlock_data = 0;

    rc = i2cRegisterOp(I2C_REG_ATOMIC_LOCK,
                       i_args,
                       false,
                       &unlock_data);

    return rc;
}

////////////////////////////////End - I2C Driver//////////////////////////////////////

//////////////////////////////////Plat implementation/////////////////////////////
#define MAX_OCMB_CMD_SIZE 16
namespace fapi2
{
    ReturnCode platGetI2c( const Target<TARGET_TYPE_ALL>& target,
                           const size_t get_size,
                           const std::vector<uint8_t>& cfgData,
                           std::vector<uint8_t>& o_data )
    {
        ReturnCode rc = FAPI2_RC_SUCCESS;
        FAPI_IMP("platGetI2c: Input target passed is 0x%08X", target.get());
        misc_args_t args;
        // Derive the port, engine and deviceaddress from input target.
        args.port = (uint8_t)(target.get().fields.port);
        args.engine = (uint8_t)(target.get().fields.engine);
        args.devAddr = (uint8_t)(target.get().fields.devAddr);
        FAPI_IMP("platGetI2c Port: %02X Engine: %02X DevAddr: %02x set",
                       args.port, args.engine, args.devAddr);
        do {
            rc = i2cSetBusVariables(400, args);
            if(rc != FAPI2_RC_SUCCESS)
            {
                break;
            }

            rc = i2cLockEngine(args);
            if(rc != FAPI2_RC_SUCCESS)
            {
                break;
            }

            uint8_t data[MAX_OCMB_CMD_SIZE] = {};
            ////////////////////////////////
            // I2C read with offset ///////
            ///////////////////////////////
            if(!cfgData.empty())
            {
                std::copy(cfgData.begin(), cfgData.end(), data);
                // First write offset to device without stop
                args.with_stop = false;
                args.skip_mode_setup = false;

                rc = i2cWrite(data,
                              cfgData.size(),
                              args);
                if(rc != FAPI2_RC_SUCCESS)
                {
                    break;
                }
                // Now do the READ with a stop
                args.with_stop = true;
                // Skip mode setup, cmd already set
                args.skip_mode_setup = true;
            }
            ////////////////////////////////
            // I2C read without offset ///////
            ///////////////////////////////
            else
            {
                // Do a direct read
                args.with_stop = true;
                args.skip_mode_setup = false;
            }
            rc = i2cRead(data,
                         get_size,
                         args);
            if(rc != FAPI2_RC_SUCCESS)
            {
                break;
            }

            rc = i2cUnlockEngine(args);
            if(rc != FAPI2_RC_SUCCESS)
            {
                break;
            }

            o_data.clear();
            for(size_t i = 0; i < get_size; i++)
            {
                o_data.push_back(data[i]);
            }
        } while(0);
        return rc;
    }

    ReturnCode platPutI2c( const Target<TARGET_TYPE_ALL>& target,
                           const std::vector<uint8_t>& data )
    {
        ReturnCode rc = FAPI2_RC_SUCCESS;

        FAPI_IMP("platPutI2c Input OCMB target passed is 0x%08X", target.get());
        misc_args_t args;
        // Derive the port, engine and deviceaddress from input target.
        args.port = (uint8_t)(target.get().fields.port);
        args.engine = (uint8_t)(target.get().fields.engine);
        args.devAddr = (uint8_t)(target.get().fields.devAddr);
        FAPI_IMP("platPutI2c OCMB Port: %02X Engine: %02X DevAddr: %02x set.", 
                       args.port, args.engine, args.devAddr);
        do {
            rc = i2cSetBusVariables(400, args);
            if(rc != FAPI2_RC_SUCCESS)
            {
                break;
            }

            rc = i2cLockEngine(args);
            if(rc != FAPI2_RC_SUCCESS)
            {
                break;
            }

            uint8_t buffer[MAX_OCMB_CMD_SIZE] = {};
            std::copy(data.begin(), data.end(), buffer);

            // Do a write with stop
            args.with_stop = true;
            args.skip_mode_setup = false;
            rc = i2cWrite(buffer,
                          data.size(),
                          args);
            if(rc != FAPI2_RC_SUCCESS)
            {
                break;
            }

            rc = i2cUnlockEngine(args);
            if(rc != FAPI2_RC_SUCCESS)
            {
                break;
            }
        } while(0);
        return rc;
    }

    ReturnCode i2cGetScom( const void *target, const uint32_t addr, uint64_t *o_data)
    {
        ReturnCode rc = FAPI2_RC_SUCCESS;
        buffer<uint64_t> data64;
        buffer<uint32_t> data32;

        do
        {
            // Check if this is a IBM_SCOM address by &ing the address with IBM_SCOM_INDICATOR
            // If the indicator is not set, then we will assume this is a microChip address
            if( (addr & mss::exp::i2c::IBM_SCOM_INDICATOR) == mss::exp::i2c::IBM_SCOM_INDICATOR)
            {
                // i/o data is expected to be 8 bytes for IBM scoms
                FAPI_EXEC_HWP(rc , mss::exp::i2c::i2c_get_scom,
                              *((fapi2::Target<fapi2::TARGET_TYPE_OCMB_CHIP>*)(target)),
                              addr,
                              data64);

                if(rc != FAPI2_RC_SUCCESS)
                {
                    FAPI_ERR("i2cGetScom for IBM_SCOM address failed with rc 0x%08X", rc);
                    break;
                }
            }
            else
            {
                // i/o data is expected to be 4 bytes for Microchip scoms
                FAPI_EXEC_HWP(rc , mss::exp::i2c::i2c_get_scom,
                              *((fapi2::Target<fapi2::TARGET_TYPE_OCMB_CHIP>*)(target)),
                              addr,
                              data32);
                if(rc != FAPI2_RC_SUCCESS)
                {
                    FAPI_ERR("i2cGetScom for Micro Chip Addresss failed with rc 0x%08X", rc);
                    break;
                }
                else
                {
                    // Put the contexts of the 32 bit buffer right justified into the 64 bit buffer
                    data64.insertFromRight<32,32>(data32);
                }
            }
            // Copy contents of 64 bit buffer to o_data
            *o_data = data64;
        } while(0);

        return rc;
    }

    ReturnCode i2cPutScom( const void *target, const uint32_t addr, uint64_t data)
    {
        ReturnCode rc = FAPI2_RC_SUCCESS;
        buffer<uint64_t> data64(data);

        do
        {
            // Check if this is a IBM_SCOM address by &ing the address with IBM_SCOM_INDICATOR
            // If the indicator is not set, then we will assume this is a microChip address
            if( (addr & mss::exp::i2c::IBM_SCOM_INDICATOR) == mss::exp::i2c::IBM_SCOM_INDICATOR)
            {
                // i/o data is expected to be 8 bytes for IBM scoms
                FAPI_EXEC_HWP(rc , mss::exp::i2c::i2c_put_scom,
                              *((fapi2::Target<fapi2::TARGET_TYPE_OCMB_CHIP>*)(target)),
                              addr,
                              data64);

                if(rc != FAPI2_RC_SUCCESS)
                {
                    FAPI_ERR("i2cPutScom for IBM SCOM Address failed with rc 0x%08X", rc);
                    break;
                }
            }
            else
            {
                buffer<uint32_t> data32;
                data64.extractToRight<32,32>(data32);
                // i/o data is expected to be 4 bytes for Microchip scoms
                FAPI_EXEC_HWP(rc , mss::exp::i2c::i2c_put_scom,
                              *((fapi2::Target<fapi2::TARGET_TYPE_OCMB_CHIP>*)(target)),
                              addr,
                              data32);
                if(rc != FAPI2_RC_SUCCESS)
                {
                    FAPI_ERR("i2cPutScom for Micor Chip Address failed with rc 0x%08X", rc);
                    break;
                }
            }
        } while(0);

        return rc;
    }
};

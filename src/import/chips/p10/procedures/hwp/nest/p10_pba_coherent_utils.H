/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/nest/p10_pba_coherent_utils.H $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2019                             */
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
//-----------------------------------------------------------------------------------
///
/// @file p10_pba_coherent_utils.H
/// @brief Common Code to support PBA get/putmem procedures (FAPI)
///
// *HWP HW Maintainer : Chris Riedl (cmr@ibm.com)
// *HWP FW Maintainer : Raja Das    (rajadas2@in.ibm.com)
// *HWP Consumed by   : SBE
// ---------------------------------------------------------------------------------
// *! ADDITIONAL COMMENTS :
// *!
// *!
//-----------------------------------------------------------------------------------

#ifndef _P10_PBA_COHERENT_UTILS_H_
#define _P10_PBA_COHERENT_UTILS_H_

//-----------------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------------

#include <fapi2.H>
#include <p10_pba_constants.H>

extern "C"
{

//-----------------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// Classes
//-----------------------------------------------------------------------------------

///
/// @brief  Manage PBA operation flag that is used to program the
///         PU_PBASLVCTL3_SCOM register.
///
    class p10_PBA_oper_flag
    {
        public:

            // Type of PBA operations
            enum OperationType_t
            {
                // Note: Encodes used directly for PBASLVCTL[write_ttype]
                DMA               = 0,
                LCO               = 1,
                ATOMIC            = 2,
                INJ               = 3,
                CI                = 4,
                ILLEGAL_OPER_TYPE = CI + 1, // Must be last entry.
            };

            enum AtomicOpType_t : std::uint8_t
            {
                // Note: Encodes used directly for PBASLVCTL[write_tsize]
                ADD = 0b00000010,
                AND = 0b00100010,
                OR  = 0b01000010,
                XOR = 0b01100010,
            };

            // Constructor
            inline p10_PBA_oper_flag(): iv_operType(DMA), iv_atomicOp(ADD),
                iv_fastMode(false), iv_flagInvalid(false)
            {
            }

            ///
            /// @brief Set the PBA Atomic operation type
            ///
            /// @param[in] i_type     PBA Atomic operation type
            ///
            /// @return void.
            ///
            inline void setAtomicOpType(const AtomicOpType_t i_type)
            {
                iv_atomicOp = i_type;

                if ( (iv_atomicOp != ADD) &&
                     (iv_atomicOp != AND) &&
                     (iv_atomicOp != OR) &&
                     (iv_atomicOp != XOR) )
                {
                    FAPI_ERR("Invalid Atomic Operation type (%d)", iv_atomicOp);
                    setFlagInvalid();
                }

                return;
            }

            ///
            /// @brief Get the PBA Atomic operation type setting.
            ///
            /// @return iv_atomicOp.
            ///
            inline const AtomicOpType_t getAtomicOpType(void)
            {
                return iv_atomicOp;
            }

            ///
            /// @brief Set the PBA operation type
            ///
            /// @param[in] i_type     PBA operation type
            ///
            /// @return void.
            ///
            inline void setOperationType(const OperationType_t i_type)
            {
                iv_operType = i_type;

                if (iv_operType >= ILLEGAL_OPER_TYPE)
                {
                    FAPI_ERR("Invalid PBA Operation type (%d)", iv_operType);
                    setFlagInvalid();
                }

                return;
            }

            ///
            /// @brief Get the PBA operation type setting.
            ///
            /// @return iv_operType.
            ///
            inline const OperationType_t getOperationType(void)
            {
                return iv_operType;
            }

            ///
            /// @brief Set fast read/write mode.
            ///        For fast read/write mode, no error check until
            ///        last granule.
            ///
            /// @param[in] i_value     True: Enable fast read/write mode.
            ///                        False: Disable fast read/write mode.
            ///
            /// @return void.
            ///
            inline void setFastMode(bool i_value)
            {
                iv_fastMode = i_value;
                return;
            }

            ///
            /// @brief Get the PBA operation fast mode setting.
            ///
            /// @return iv_fastMode.
            ///
            inline const bool getFastMode(void)
            {
                return iv_fastMode;
            }

            ///
            /// @brief Assemble the 32-bit PBA flag based on current
            ///        info contained in this class.
            ///        This flag is to be used in PBA interface call
            ///        See flag bit definitions in p10_pba_constants.H
            ///
            /// @return uint32_t
            ///
            inline uint32_t setFlag();

            ///
            /// @brief Update the PBA class instant variables with info
            ///        embedded in the passed in flag value.
            ///
            /// @return void.
            ///
            inline void getFlag(uint32_t i_flag);

            /// @brief Return flag valid status
            /// @return true if flag is valid
            inline bool isFlagValid()
            {
                return (!iv_flagInvalid);
            }

        private:

            /// @brief Set the flag invalid bit
            /// @return void
            inline void setFlagInvalid()
            {
                iv_flagInvalid = true;
                return;
            }

            // Class variables
            OperationType_t iv_operType;         // Operation type
            AtomicOpType_t iv_atomicOp;
            bool iv_fastMode;                    // Fast PBA read/write mode
            bool iv_flagInvalid;                 // True if flag is invalid
    };

///
/// See doxygen in class definition
///
    uint32_t p10_PBA_oper_flag::setFlag()
    {
        uint32_t l_pbaFlag = 0;

        // Operation type
        l_pbaFlag |= (iv_operType << FLAG_PBA_TTYPE_SHIFT);

        if (iv_operType == p10_PBA_oper_flag::ATOMIC)
        {
            l_pbaFlag |= (iv_atomicOp << FLAG_PBA_ATOMIC_OPTYPE_SHIFT);
        }

        // Fast mode
        if (iv_fastMode == true)
        {
            l_pbaFlag |= FLAG_PBA_FASTMODE;
        }

        // Set flag validity
        if (iv_flagInvalid == true)
        {
            l_pbaFlag |= FLAG_PBA_INVALID_BIT;
        }

        // Debug trace
        FAPI_DBG("p10_PBA_oper_flag::setFlag()");
        FAPI_DBG("    iv_operType 0x%.8X, iv_fastMode 0x%.8X, iv_atomicOp 0x%.8X, iv_flagInvalid 0x%.8X",
                 iv_operType, iv_fastMode, iv_atomicOp, iv_flagInvalid);
        FAPI_DBG("   PBA Flag value: 0x%.8X", l_pbaFlag);

        return l_pbaFlag;
    }

///
/// See doxygen in class definition
///
    void p10_PBA_oper_flag::getFlag(const uint32_t i_flag)
    {
        // Decode Operation type
        iv_operType = static_cast<OperationType_t>
                      ( (i_flag & FLAG_PBA_TTYPE) >> FLAG_PBA_TTYPE_SHIFT);

        if (iv_operType == p10_PBA_oper_flag::ATOMIC)
        {
            iv_atomicOp = static_cast<AtomicOpType_t>
                          ( (i_flag & FLAG_PBA_ATOMIC_OPTYPE) >> FLAG_PBA_ATOMIC_OPTYPE_SHIFT);
        }

        // Fast mode
        iv_fastMode = (i_flag & FLAG_PBA_FASTMODE);

        // Flag validity
        iv_flagInvalid = (i_flag & FLAG_PBA_INVALID_BIT);

        // Debug trace
        FAPI_DBG("Input flag value: 0x%.8X", i_flag);
        FAPI_DBG("p10_PBA_oper_flag::getFlag() - Flag value 0x%.8X", i_flag);
        FAPI_DBG("    iv_operType 0x%.8X, iv_fastMode 0x%.8X, iv_atomicOp 0x%.8X, iv_flagInvalid 0x%.8X",
                 iv_operType, iv_fastMode, iv_atomicOp, iv_flagInvalid);

        return;
    }

//-----------------------------------------------------------------------------------
// Function prototypes
//-----------------------------------------------------------------------------------

/// @brief does the setup for the PBA to set up the initial registers for a read/write
/// @param[in] i_target => P9 chip target
/// @param[in] i_core_target => Ex target for which L3 we are targeting
/// @param[in] i_address => starting address for PBA operation
/// @param[in] i_rnw => whether the operation is a read or write
/// @param[in] i_flags => flags that contain information that the PBA needs to know to set up registers
/// @return FAPI_RC_SUCCESS if setting up the pba registers is a success
    fapi2::ReturnCode p10_pba_coherent_setup_pba(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
        const fapi2::Target<fapi2::TARGET_TYPE_CORE>& i_core_target,
        const uint64_t i_address,
        const bool i_rnw,
        const uint32_t i_flags);

/// @brief does the write for the PBA
/// @param[in] i_target => P9 chip target
/// @param[in] i_address => address for this write
/// @param[in] i_write_data => the data that is to be written to the PBA
/// @param[in] i_ttype => ttype to use for write access
/// @return FAPI_RC_SUCCESS if writing the PBA is a success
    fapi2::ReturnCode p10_pba_coherent_pba_write(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
        const uint64_t i_address,
        const uint8_t i_write_data[],
        const p10_PBA_oper_flag::OperationType_t i_ttype);

/// @brief does the read for the PBA
/// @param[in] i_target => P9 chip target
/// @param[in] i_address => address for this read
/// @param[in] i_ttype => ttype to use for read access
/// @param[out] o_read_data => the data that is read from the PBA
/// @return FAPI_RC_SUCCESS if reading the PBA is a success
    fapi2::ReturnCode p10_pba_coherent_pba_read(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
        const uint64_t i_address,
        const p10_PBA_oper_flag::OperationType_t i_ttype,
        uint8_t o_read_data[]);

/// @brief calculates the number of 128 byte granules that can be read/written before setup needs to be run again
/// @param[in] i_target => P9 chip target
/// @param[in] i_address => starting address for PBA operation
/// @return number of 128 byte granules that can be read/written before setup needs to be run again
    fapi2::ReturnCode p10_pba_coherent_utils_get_num_granules(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
        const uint64_t i_address,
        uint32_t& o_numGranules);

/// @brief ensure that fabric is initialized and stop control is not set
///           (by checkstop/mode switch), which if set would prohibit fabric
///           commands from being broadcasted
/// @param[in] i_target => P9 chip target
/// @return FAPI_RC_SUCCESS if fabric is not stopped
    fapi2::ReturnCode p10_pba_coherent_utils_check_fbc_state(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target);

/// @brief check that the address is cacheline aligned and within the fabric real address range
/// @param[in] i_target => P9 chip target
/// @param[in] i_address => starting address for PBA operation
///@param[in] i_flags => flags that contain information that the PBA needs to know to set up registers
/// @return FAPI_RC_SUCCESS if arguments are valid
    fapi2::ReturnCode p10_pba_coherent_utils_check_args(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
        const uint64_t i_address,
        const uint32_t i_flags);

/// @brief this does any cleanup for the PBA after all reads/writes have been done
/// @param[in] i_target => P9 chip target
/// @return FAPI_RC_SUCCESS if cleaning up the PBA is a success
    fapi2::ReturnCode p10_pba_coherent_cleanup_pba(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target);

///@brief sets up the PBA Bar
///@param[in] i_target => P9 chip target
///@param[in] i_address => address for this read/write
///@return FAPI_RC_SUCCESS if writing the PBA is a success
    fapi2::ReturnCode p10_pba_coherent_setup_pba_bar(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
        const uint64_t i_baseAddress);

///@brief does extra error handling for if we hit a problem with a read or write
///@param[in] i_target => P9 chip target
///@param[in] i_rc => The current error that we are seeing from the read/write
///@return the error that we got from the scom or a different error that points to a problem in the PBA
    fapi2::ReturnCode p10_pba_coherent_error_handling(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
            fapi2::ReturnCode i_rc);

///@brief does error checking on the OCB side
///@param[in] i_target => P9 chip target
///@return FAPI_RC_SUCCESS if no errors are detected otherwise an error that contains what is in the OCB status registers
    fapi2::ReturnCode p10_pba_coherent_check_ocb_status(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target);

///@brief does error checking on the PBA Fir
///@param[in] i_target => P9 chip target
///@return FAPI_RC_SUCCESS if no errros are detected otherwise an error that has what error is in the PBA Fir
    fapi2::ReturnCode p10_pba_coherent_check_pba_fir(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target);

///@brief calls all of the error checking procedures (ocb_status, check_pba_fir, and status_check)
///@param[in] i_target => P9 chip target
///@return FAPI_RC_SUCCESS if no errors are detected on the PBA
    fapi2::ReturnCode p10_pba_coherent_check_status(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>&
            i_target);

///@brief appends input data to the fapi2 return code if we got an error back
///@param[in] i_address => address for the read or write
///@param[in] i_rnw => whether the operation is a read or write
///@param[in] i_flags => flags that contain information that the PBA needs to know to set up registers
///@param[inout] o_rc => fapi error code to be appended with input data
    void p10_pba_coherent_append_input_data(const uint64_t i_address, const bool i_rnw, const uint32_t i_flags,
                                            fapi2::ReturnCode& o_rc);

} //extern "C"

#endif //_P10_PBA_COHERENT_UTILS_H_

/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/nest/p9_pba_coherent_utils.H $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2019                        */
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
// *!
/// @file p9_pba_coherent_utils.H
/// @brief Common Code to support PBA get/putmem procedures (FAPI)
///
// *HWP HWP Owner: Christina Graves clgraves@us.ibm.com
// *HWP FW Owner: Thi Tran thi@us.ibm.com
// *HWP Team: Nest
// *HWP Level: 2
// *HWP Consumed by: SBE
// ---------------------------------------------------------------------------------
// *! ADDITIONAL COMMENTS :
// *!
// *!
//-----------------------------------------------------------------------------------

#ifndef _P9_PBA_COHERENT_UTILS_H_
#define _P9_PBA_COHERENT_UTILS_H_

//-----------------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------------

#include <fapi2.H>
#include <p9_pba_constants.H>

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
    class p9_PBA_oper_flag
    {
        public:

            // Type of PBA operations
            enum OperationType_t
            {
                DMA            = 0,
                LCO            = 1,
                ATOMIC         = 2,
                INJ            = 3,
                CI             = 4,
            };

            // Constructor
            inline p9_PBA_oper_flag(): iv_operType(DMA), iv_fastMode(false)
            {
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
            ///        See flag bit definitions in p9_pba_constants.H
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

        private:

            // Class variables
            OperationType_t iv_operType;         // Operation type
            bool iv_fastMode;                    // Fast PBA read/write mode
    };

///
/// See doxygen in class definition
///
    uint32_t p9_PBA_oper_flag::setFlag()
    {
        uint32_t l_pbaFlag = 0;

        // Operation type
        l_pbaFlag |= (iv_operType << FLAG_PBA_TTYPE_SHIFT);

        // Fast mode
        if (iv_fastMode == true)
        {
            l_pbaFlag |= FLAG_PBA_FASTMODE;
        }

        // Debug trace
        FAPI_DBG("p9_PBA_oper_flag::setFlag()");
        FAPI_DBG("    iv_operType 0x%.8X, iv_fastMode 0x%.8X", iv_operType, iv_fastMode);
        FAPI_DBG("   PBA Flag value: 0x%.8X", l_pbaFlag);

        return l_pbaFlag;
    }

///
/// See doxygen in class definition
///
    void p9_PBA_oper_flag::getFlag(const uint32_t i_flag)
    {
        // Decode Operation type
        iv_operType = static_cast<OperationType_t>
                      ( (i_flag & FLAG_PBA_TTYPE) >> FLAG_PBA_TTYPE_SHIFT);

        // Fast mode
        iv_fastMode = (i_flag & FLAG_PBA_FASTMODE);

        // Debug trace
        FAPI_DBG("p9_PBA_oper_flag::getFlag() - Flag value 0x%.8X", i_flag);
        FAPI_DBG("    iv_operType 0x%.8X, iv_fastMode 0x%.8X",
                 iv_operType, iv_fastMode);
        return;
    }

//-----------------------------------------------------------------------------------
// Function prototypes
//-----------------------------------------------------------------------------------

/// @brief does the setup for the PBA to set up the initial registers for a read/write
/// @param[in] i_target => P9 chip target
/// @param[in] i_ex_target => Ex target for which L3 we are targeting
/// @param[in] i_address => starting address for PBA operation
/// @param[in] i_rnw => whether the operation is a read or write
/// @param[in] i_flags => flags that contain information that the PBA needs to know to set up registers
/// @return FAPI_RC_SUCCESS if setting up the pba registers is a success
    fapi2::ReturnCode p9_pba_coherent_setup_pba(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
        const fapi2::Target<fapi2::TARGET_TYPE_EX>& i_ex_target,
        const uint64_t i_address,
        const bool i_rnw,
        const uint32_t i_flags);

/// @brief does the write for the PBA
/// @param[in] i_target => P9 chip target
/// @param[in] i_address => address for this write
/// @param[in] i_write_data => the data that is to be written to the PBA
/// @return FAPI_RC_SUCCESS if writing the PBA is a success
    fapi2::ReturnCode p9_pba_coherent_pba_write(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
        const uint64_t i_address,
        const uint8_t i_write_data[]);

/// @brief does the read for the PBA
/// @param[in] i_target => P9 chip target
/// @param[in] i_address => address for this write
/// @param[out] o_read_data => the data that is read from the PBA
/// @return FAPI_RC_SUCCESS if reading the PBA is a success
    fapi2::ReturnCode p9_pba_coherent_pba_read(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
        const uint64_t i_address,
        uint8_t o_read_data[]);

/// @brief calculates the number of 128 byte granules that can be read/written before setup needs to be run again
/// @param[in] i_target => P9 chip target
/// @param[in] i_address => starting address for PBA operation
/// @return number of 128 byte granules that can be read/written before setup needs to be run again
    fapi2::ReturnCode p9_pba_coherent_utils_get_num_granules(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
        const uint64_t i_address,
        uint32_t& o_numGranules);

/// @brief ensure that fabric is initialized and stop control is not set
///           (by checkstop/mode switch), which if set would prohibit fabric
///           commands from being broadcasted
/// @param[in] i_target => P9 chip target
/// @return FAPI_RC_SUCCESS if fabric is not stopped
    fapi2::ReturnCode p9_pba_coherent_utils_check_fbc_state(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target);

/// @brief check that the address is cacheline aligned and within the fabric real address range
/// @param[in] i_target => P9 chip target
/// @param[in] i_address => starting address for PBA operation
/// @return FAPI_RC_SUCCESS if arguments are valid
    fapi2::ReturnCode p9_pba_coherent_utils_check_args(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
        const uint64_t i_address);
/// @brief this checks the PBA/OCB status registers - this is for use at the end of each write/read or at the end of each stream
/// @return FAPI_RC_SUCCESS if the status check is a success
    fapi2::ReturnCode p9_pba_coherent_status_check(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target);

/// @brief this does any cleanup for the PBA after all reads/writes have been done
/// @param[in] i_target => P9 chip target
/// @return FAPI_RC_SUCCESS if cleaning up the PBA is a success
    fapi2::ReturnCode p9_pba_coherent_cleanup_pba(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target);

///@brief sets up the PBA Bar
///@param[in] i_target => P9 chip target
///@param[in] i_address => address for this read/write
///@return FAPI_RC_SUCCESS if writing the PBA is a success
    fapi2::ReturnCode p9_pba_coherent_setup_pba_bar(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
        const uint64_t i_baseAddress);

} //extern "C"

#endif //_P9_PBA_COHERENT_UTILS_H_

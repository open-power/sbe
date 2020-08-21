/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_ram_core.H $     */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2019,2020                        */
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
/// @file p10_ram_core.H
/// @brief Class that implements the base ramming capability
///
// *HWP HW Maintainer : Doug Holtsinger <Douglas.Holtsinger@ibm.com>
// *HWP FW Maintainer : Raja Das        <rajadas2@in.ibm.com>
// *HWP Consumed by   : SBE
//

#ifndef _P10_RAM_CORE_H_
#define _P10_RAM_CORE_H_

//-----------------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------------
#include <fapi2.H>

//-----------------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------------

// pseudo identifiers for special registers -- see p10_spr_name_map.H
const uint32_t RAM_REG_NIA     = 2000;
const uint32_t RAM_REG_MSRD    = 2001;
const uint32_t RAM_REG_CR      = 2002;
const uint32_t RAM_REG_FPSCR   = 2003;
const uint32_t RAM_REG_VSCR    = 2004;
const uint32_t RAM_REG_MSR     = 2005;
const uint32_t RAM_REG_MSR_L1  = 2006;
const uint32_t RAM_REG_MSRD_L1 = 2007;

//-----------------------------------------------------------------------------------
// Structure definitions
//-----------------------------------------------------------------------------------
// register access type
enum Enum_RegType
{
    REG_GPR,
    REG_SPR,
    REG_FPR,
    REG_VSR
};

class RamCore
{
    public:
        //-----------------------------------------------------------------------------------
        // Function prototype
        //-----------------------------------------------------------------------------------
        /// @brief Constructor of the class that implements the base ramming capability
        /// @param[in] i_target => core target
        /// @param[in] i_thread => thread number
        //
        RamCore(const fapi2::Target<fapi2::TARGET_TYPE_CORE>& i_target,
                const uint8_t i_thread);

        //-----------------------------------------------------------------------------------
        /// @brief Destructor of the class that implements the base ramming capability
        //
        ~RamCore();

        //-----------------------------------------------------------------------------------
        /// @brief Enable RAM mode and backup the registers(SCR0/GPR0/GPR1) that
        ///        will be destroyed later during ramming. Avoid doing RAM setup if
        //         required to avoid HW542214.
        /// @return FAPI2_RC_SUCCESS if success, else error code.
        //
        fapi2::ReturnCode ram_setup();

        //-----------------------------------------------------------------------------------
        /// @brief Perform the ram and check ram is done
        /// @param[in] i_opcode     => opcode to ram
        /// @param[in] i_allow_mult => indicate whether to setup and cleanup
        ///                            true: only perform ram, do not call ram_setup
        ///                                  and ram_cleanup
        ///                            false: call ram_setup and ram_cleanup
        /// @return FAPI2_RC_SUCCESS if success, else error code.
        //
        fapi2::ReturnCode ram_opcode(const uint32_t i_opcode,
                                     const bool i_allow_mult = false);

        //-----------------------------------------------------------------------------------
        /// @brief Clear RAM mode and restore the backup registers.
        /// @return FAPI2_RC_SUCCESS if success, else error code.
        //
        fapi2::ReturnCode ram_cleanup();

        //-----------------------------------------------------------------------------------
        /// @brief Get a register value by ramming.  Avoid RAMing if required to avoid HW542214.
        /// @param[in] i_type       => register type (REG_SPR/REG_GPR/REG_FPR/REG_VSR)
        /// @param[in] i_reg_num    => register nubmer
        /// @param[out] o_buffer    => register value.  Return 'INACTIVE' if required to avoid HW542214.
        /// @param[in] i_allow_mult => indicate whether to setup and cleanup
        ///                            true: only perform ram, do not call ram_setup
        ///                                  and ram_cleanup
        ///                            false: call ram_setup and ram_cleanup
        /// @return FAPI2_RC_SUCCESS if success, else error code.
        //
        fapi2::ReturnCode get_reg(const Enum_RegType i_type,
                                  const uint32_t i_reg_num,
                                  fapi2::buffer<uint64_t>* o_buffer,
                                  const bool i_allow_mult = false);

        //-----------------------------------------------------------------------------------
        /// @brief Put a register value by ramming. Return an error if required to avoid HW542214.
        /// @param[in] i_type       => register type (REG_SPR/REG_GPR/REG_FPR/REG_VSR)
        /// @param[in] i_reg_num    => register nubmer
        /// @param[in] i_buffer     => register value
        /// @param[in] i_allow_mult => indicate whether to setup and cleanup
        ///                            true: only perform ram, do not call ram_setup
        ///                                  and ram_cleanup
        ///                            false: call ram_setup and ram_cleanup
        /// @return FAPI2_RC_SUCCESS if success, else error code.
        //
        fapi2::ReturnCode put_reg(const Enum_RegType i_type,
                                  const uint32_t i_reg_num,
                                  const fapi2::buffer<uint64_t>* i_buffer,
                                  const bool i_allow_mult = false);

    private:
        fapi2::Target<fapi2::TARGET_TYPE_CORE> iv_target;  // core target
        uint8_t iv_thread;     // thread number
        bool iv_ram_enable;    // ram mode is enabled
        bool iv_ram_scr0_save; // SCR0 is saved when setup
        bool iv_ram_setup;     // ram mode is enabled and register backup is done
        bool iv_ram_err;       // error happened during ram
        bool iv_write_gpr0;    // putGPR0 operation is executed
        bool iv_write_gpr1;    // putGPR1 operation is executed
        bool iv_thread_activated;   // iv_thread was activated
        bool iv_fake_ramming;   // Using fake RAMing to avoid HW542214
        fapi2::buffer<uint64_t> iv_backup_scr0;  // SCR0 register backup data
        fapi2::buffer<uint64_t> iv_backup_gpr0;  // GPR0 register backup data
        fapi2::buffer<uint64_t> iv_backup_gpr1;  // GPR1 register backup data

        //-----------------------------------------------------------------------------------
        /// @brief Generate predecode for the opcode to ramming
        /// @param[in] i_opcode => opcode to ram
        /// @return the predecode
        //
        uint8_t gen_predecode(const uint32_t i_opcode);

        //-----------------------------------------------------------------------------------
        /// @brief Check if the opcode is load/store or not
        /// @param[in] i_opcode => opcode to ram
        /// @return TRUE if it is load/store
        //
        bool is_load_store(const uint32_t i_opcode);

        //-----------------------------------------------------------------------------------
        /// @brief Check if the thread is ready for RAMing
        /// @param[out] o_ready_for_ram => return true if ready, false if not ready
        /// @param[out] o_thread_info   => contents of THREAD_INFO register
        /// @return FAPI2_RC_SUCCESS if success, else error code.
        //
        fapi2::ReturnCode ram_ready(bool& o_ready, fapi2::buffer<uint64_t>& o_thread_info);

        //-----------------------------------------------------------------------------------
        /// @brief Check if the thread is ready for RAMing and activate thread if necessary
        /// @return FAPI2_RC_SUCCESS if success, else error code.
        //
        fapi2::ReturnCode ram_ready_activate();

        //-----------------------------------------------------------------------------------
        /// @brief Initiate HW recovery as workaround for HW533775
        /// @return FAPI2_RC_SUCCESS if success, else error code.
        fapi2::ReturnCode ram_initiate_recovery();

        //-----------------------------------------------------------------------------------
        /// @brief Enable RAM mode and backup the registers(SCR0/GPR0/GPR1) that
        ///        will be destroyed later during ramming
        /// @return FAPI2_RC_SUCCESS if success, else error code.
        //
        fapi2::ReturnCode ram_setup_internal();

        //-----------------------------------------------------------------------------------
        /// @brief Get a register value by ramming.
        /// @param[in] i_type       => register type (REG_SPR/REG_GPR/REG_FPR/REG_VSR)
        /// @param[in] i_reg_num    => register nubmer
        /// @param[out] o_buffer    => register value
        /// @param[in] i_allow_mult => indicate whether to setup and cleanup
        ///                            true: only perform ram, do not call ram_setup
        ///                                  and ram_cleanup
        ///                            false: call ram_setup and ram_cleanup
        /// @return FAPI2_RC_SUCCESS if success, else error code.
        //
        fapi2::ReturnCode get_reg_internal(const Enum_RegType i_type,
                                           const uint32_t i_reg_num,
                                           fapi2::buffer<uint64_t>* o_buffer,
                                           const bool i_allow_mult = false);

        //-----------------------------------------------------------------------------------
        /// @brief Clear RAM mode and restore the backup registers.
        /// @return FAPI2_RC_SUCCESS if success, else error code.
        //
        fapi2::ReturnCode ram_cleanup_internal();

};

#endif //_P10_RAM_CORE_H_

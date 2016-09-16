/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/hwpf/include/plat/plat_target.H $                         */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2016                        */
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
/**
 * @file plat_target.H
 * @brief platform definitions for fapi2 targets
 */

#ifndef __FAPI2_PLAT_TARGET__
#define __FAPI2_PLAT_TARGET__

#include <stdint.h>
#include <target_types.H>
#include <target_states.H>
#include <assert.h>
#include <plat_target_parms.H>
#include <vector>

static const uint8_t CORES_PER_QUAD = 4;
static const uint8_t EX_PER_QUAD = 2;
static const uint8_t CORES_PER_EX = 2;
static const uint8_t N3_CHIPLET = 5;
static const uint8_t MCS_PER_MCBIST = 2;

//
// Define what a platform handle looks like. For Hostboot,
// for example, this might be a void*. For the SBE, this
// will be a uint32_t ...
//
namespace fapi2
{
    typedef enum plat_target_type
    {
        PPE_TARGET_TYPE_NONE              = 0x00,
        PPE_TARGET_TYPE_PROC_CHIP         = 0x01,
        PPE_TARGET_TYPE_MCS               = 0x02,
        PPE_TARGET_TYPE_CORE              = 0x04,
        PPE_TARGET_TYPE_EQ                = 0x08,
        PPE_TARGET_TYPE_EX                = 0x10,
        PPE_TARGET_TYPE_PERV              = 0x20,
        PPE_TARGET_TYPE_MCBIST            = 0x40,
        PPE_TARGET_TYPE_SYSTEM            = 0x80,
        PPE_TARGET_TYPE_ALL               = 0xFFF,
    } plat_target_type_t;

    typedef union plat_target_handle {
        uint32_t value;
        struct {
#ifdef _BIG_ENDIAN
            uint32_t chiplet_num       : 8;
            uint32_t type_target_num   : 8;
            uint32_t present           : 1;
            uint32_t functional        : 1;
            uint32_t is_multicast      : 1;
            uint32_t valid             : 1;
            uint32_t type              : 12;
#else
            uint32_t type              : 12;
            uint32_t valid             : 1;
            uint32_t is_multicast      : 1;
            uint32_t functional        : 1;
            uint32_t present           : 1;
            uint32_t type_target_num   : 8;
            uint32_t chiplet_num       : 8;
#endif
        } fields;
        ///
        /// @brief Plat target handle constructor
        ///
        /// @param  i_value Value to instantiate handle with
        ///
        plat_target_handle(uint32_t i_value = 0):value(i_value) {}

        ///
        /// @brief Get the fapi2::TargetType for this target
        ///
        /// @par Converts the internal PPE type for this target to fapi2 enum
        ///
        /// @return The fapi2::TargetType for this target
        ///
        TargetType getFapiTargetType() const;

        ///
        /// @brief Get the instance number for this target
        ///
        /// @return The instance number for this target
        ///
        inline uint32_t getTargetInstance() const
        {
            return fields.type_target_num;
        }

        ///
        /// @brief Get this target's children
        ///
        /// @param [in] i_parentType fapi2 type of the parent
        /// @param [in] i_childType  fapi2 type of the child
        /// @param [in] i_platType   Plat type of the parent
        /// @param [in] i_state      Required state of the children
        /// @param [out] o_children  A vector of child target handles
        ///
        void getChildren(const TargetType i_parentType,
                         const TargetType i_childType,
                         const plat_target_type_t i_platType,
                         const TargetState i_state,
                         std::vector<plat_target_handle> &o_children) const;

        ///
        /// @brief Get proc chip target's children - filtered
        ///
        /// @param [in] i_filter     Target filter
        /// @param [in] i_state      Required state of the children
        /// @param [out] o_children  A vector of child target handles
        ///
        void getChildren(const TargetFilter i_filter,
                         const TargetState i_state,
                         std::vector<plat_target_handle>& o_children) const;

        ///
        /// @brief Gets the plat target handle as a uint32
        ///
        /// @return Plat target handle as a uint32_t
        ///
        operator uint32_t() const {return value;}
    } plat_target_handle_t;

    typedef plat_target_handle_t plat_target_argument_t;

    template<TargetType K, bool MULTICAST = false>
    plat_target_handle_t createPlatTargetHandle(const uint32_t i_plat_argument)
    {
        static_assert((MULTICAST != true) || (K == TARGET_TYPE_PROC_CHIP),
                "Only PROC_CHIP types can be multicast");
        plat_target_handle_t l_handle = 0;

        if(MULTICAST == true)
        {
            // Simply set the is multicast flag
            l_handle.fields.is_multicast = 1;
        }
        else if(K & TARGET_TYPE_PROC_CHIP)
        {
            l_handle.fields.chiplet_num = 0;
            l_handle.fields.type = PPE_TARGET_TYPE_PROC_CHIP;
            l_handle.fields.type_target_num = 0;
        }
        else if(K & TARGET_TYPE_PERV)
        {
            l_handle.fields.chiplet_num = i_plat_argument + NEST_GROUP1_CHIPLET_OFFSET;
            l_handle.fields.type = PPE_TARGET_TYPE_PERV;
            l_handle.fields.type_target_num = i_plat_argument;
        }
        else if(K & TARGET_TYPE_CORE)
        {
            l_handle.fields.chiplet_num = i_plat_argument + CORE_CHIPLET_OFFSET;
            l_handle.fields.type = PPE_TARGET_TYPE_CORE | PPE_TARGET_TYPE_PERV;
            l_handle.fields.type_target_num = i_plat_argument;
        }
        else if(K & TARGET_TYPE_EQ)
        {
            l_handle.fields.chiplet_num = i_plat_argument + EQ_CHIPLET_OFFSET;
            l_handle.fields.type = PPE_TARGET_TYPE_EQ | PPE_TARGET_TYPE_PERV;
            l_handle.fields.type_target_num = i_plat_argument;
        }
        else if(K & TARGET_TYPE_EX)
        {
            l_handle.fields.chiplet_num = (i_plat_argument / 2) + EX_CHIPLET_OFFSET;
            l_handle.fields.type = PPE_TARGET_TYPE_EX;
            l_handle.fields.type_target_num = i_plat_argument;
        }
        else if(K & TARGET_TYPE_MCBIST)
        {
            l_handle.fields.chiplet_num = i_plat_argument + MCBIST_CHIPLET_OFFSET;
            l_handle.fields.type = PPE_TARGET_TYPE_MCBIST | PPE_TARGET_TYPE_PERV;
            l_handle.fields.type_target_num = i_plat_argument;
        }
        else if(K & TARGET_TYPE_MCS)
        {
            l_handle.fields.chiplet_num = N3_CHIPLET - (MCS_PER_MCBIST * (i_plat_argument / MCS_PER_MCBIST));
            l_handle.fields.type = PPE_TARGET_TYPE_MCS;
            l_handle.fields.type_target_num = i_plat_argument;
        }
        else if(K == TARGET_TYPE_ALL)
        {
            l_handle.fields.chiplet_num = i_plat_argument;
            l_handle.fields.type = PPE_TARGET_TYPE_ALL;
        }

        l_handle.fields.valid = 1;
        return l_handle;
    }

};

#endif

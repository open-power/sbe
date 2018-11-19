/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/hwpf/plat_target.H $                                      */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2019                        */
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
#include <multicast_defs.H>
#include <assert.h>
#include <plat_target_parms.H>
#include <vector>

static const uint8_t CORES_PER_QUAD = 4;
static const uint8_t EX_PER_QUAD = 2;
static const uint8_t CORES_PER_EX = 2;
static const uint8_t N2_CHIPLET = 4;
static const uint8_t N3_CHIPLET = 5;
static const uint8_t MCS_PER_MCBIST = 2;
static const uint8_t MI_PER_MC = 2;
static const uint8_t PCI0_CHIPLET = 0x0D;

//
// Define what a platform handle looks like. For Hostboot,
// for example, this might be a void*. For the SBE, this
// will be a uint32_t ...
//
namespace fapi2
{
    template<TargetType K, MulticastType M, typename V>
    static constexpr void plat_apply_target_limits(void)
    {
        static_assert(!(K & TARGET_TYPE_MULTICAST),
                "Multicast targets are not supported on this platform");
    }

    typedef enum plat_target_type
    {
        PPE_TARGET_TYPE_NONE              = 0x00,
        PPE_TARGET_TYPE_PROC_CHIP         = 0x01,
        PPE_TARGET_TYPE_MCS               = 0x02,
        PPE_TARGET_TYPE_CORE              = 0x03,
        PPE_TARGET_TYPE_EQ                = 0x04,
        PPE_TARGET_TYPE_EX                = 0x05,
        PPE_TARGET_TYPE_PERV              = 0x06,
        PPE_TARGET_TYPE_MCBIST            = 0x07,
        PPE_TARGET_TYPE_SYSTEM            = 0x08,
        PPE_TARGET_TYPE_PHB               = 0x09,
        PPE_TARGET_TYPE_MI                = 0x0A,
        PPE_TARGET_TYPE_MC                = 0x0B,
    } plat_target_type_t;

    typedef union plat_target_handle {
        uint32_t value;
        struct {
            /* chiplet number is overloaded for multicast vs. unicast targets */
            union {
                uint32_t chiplet_num     :8;
                struct {
                    uint32_t                 :1; // unused in the multicast case, must be zero for unicast targets
                    uint32_t is_multicast    :1; // used for targeting
                    uint32_t mcast_type      :3; // used for targeting
                    uint32_t mcast_group     :3; // used for targeting
                } __attribute__((packed));
            } __attribute__((packed));
            uint32_t type_target_num :8; // used internally
            /* lower 16 bits start here */
            uint32_t core_select     :4; // used for targeting
            uint32_t                 :2;
            uint32_t present         :1; // used internally
            uint32_t functional      :1; // used internally
            uint32_t                 :4;
            uint32_t type            :4; // used internally
        } fields __attribute__((packed));

		/* Return the target value with all internal bits masked away */
		uint32_t getPIBAddress() {
			return value & 0x7F00F000;
		}

        ///
        /// @brief Plat target handle constructor
        ///
        /// @param  i_value Value to instantiate handle with
        ///
        explicit plat_target_handle(uint32_t i_value = 0):value(i_value) {}

        ///
        /// @brief Get the fapi2::TargetType for this target
        ///
        /// @par Converts the internal PPE type for this target to fapi2 enum
        ///
        /// @return The fapi2::TargetType for this target
        ///
        TargetType getFapiTargetType() const;

        ///
        /// @brief Get the scom address overlay for this target
        ///
        /// @return Address overlay
        ///
        uint32_t getAddressOverlay() const
        {
            return (value & 0xFF000000);
        }

        ///
        /// @brief Get the plat target type
        ///
        /// @return The plat target type as a fapi2::TargetType
        ///
        plat_target_type getTargetType() const
        {
            return static_cast<plat_target_type>(fields.type);
        }

        ///
        /// @brief Get the instance number for this target
        ///
        /// @return The instance number for this target
        ///
        uint32_t getTargetInstance() const
        {
            return fields.type_target_num;
        }

        ///
        /// @brief Returns whether this target is functional
        ///
        /// @return true if Target is functional
        ///
        bool getFunctional() const
        {
            return fields.functional;
        }

        ///
        /// @brief Set functional state of the Target
        ///
        /// @param [in] i_state Functional state to set
        ///
        void setFunctional(const bool &i_state)
        {
            fields.functional = i_state;
        }

        ///
        /// @brief Returns whether this target is present
        ///
        /// @return true if Target is present
        ///
        bool getPresent() const
        {
            return fields.present;
        }

        ///
        /// @brief Set Target as present
        ///
        void setPresent()
        {
            fields.present = true;
        }

        ///
        /// @brief Get this target's parent
        ///
        /// @param [in] The fapi2 type of the requested parent
        /// @return Plat target handle to the parent target
        ///
        plat_target_handle getParent(const TargetType i_parentType) const;

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
        /// @brief Is this also usable as a PERV target?
        ///
        bool isPerv() const;

        ///
        /// @brief Gets the plat target handle as a uint32
        ///
        /// @return Plat target handle as a uint32_t
        ///
        operator uint32_t() const {return value;}
    } plat_target_handle_t;

    typedef plat_target_handle_t plat_target_argument_t;

    template<TargetType K>
    plat_target_handle_t createPlatTargetHandle(const uint32_t i_plat_argument)
    {
        static_assert(K != TARGET_TYPE_ALL, "Target instances cannot be of type ALL");
        plat_target_handle_t l_handle;

        if(K & TARGET_TYPE_PROC_CHIP)
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
            l_handle.fields.type = PPE_TARGET_TYPE_CORE;
            l_handle.fields.type_target_num = i_plat_argument;
        }
        else if(K & TARGET_TYPE_EQ)
        {
            l_handle.fields.chiplet_num = i_plat_argument + EQ_CHIPLET_OFFSET;
            l_handle.fields.type = PPE_TARGET_TYPE_EQ;
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
            l_handle.fields.type = PPE_TARGET_TYPE_MCBIST;
            l_handle.fields.type_target_num = i_plat_argument;
        }
        else if(K & TARGET_TYPE_MCS)
        {
            l_handle.fields.chiplet_num = N3_CHIPLET - (MCS_PER_MCBIST * (i_plat_argument / MCS_PER_MCBIST));
            l_handle.fields.type = PPE_TARGET_TYPE_MCS;
            l_handle.fields.type_target_num = i_plat_argument;
        }
        else if(K & TARGET_TYPE_PHB)
        {
            l_handle.fields.chiplet_num = (0 == i_plat_argument) ? PCI0_CHIPLET :
                                           (((i_plat_argument / 3) + 1) + PCI0_CHIPLET);
            l_handle.fields.type = PPE_TARGET_TYPE_PHB;
            l_handle.fields.type_target_num = i_plat_argument;
        }
        else if(K & TARGET_TYPE_MC)
        {
            l_handle.fields.chiplet_num = i_plat_argument + MC_CHIPLET_OFFSET;
            l_handle.fields.type = PPE_TARGET_TYPE_MC;
            l_handle.fields.type_target_num = i_plat_argument;
        }
        else if(K & TARGET_TYPE_MI)
        {
            l_handle.fields.chiplet_num = N3_CHIPLET - (MI_PER_MC * (i_plat_argument / MI_PER_MC));
            l_handle.fields.type = PPE_TARGET_TYPE_MI;
            l_handle.fields.type_target_num = i_plat_argument;
        }

        return l_handle;
    }

};

#endif

/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/hwpf/plat_target.H $                                      */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2020                        */
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

static const uint8_t CORES_PER_QUAD     = 4;
static const uint8_t PAU_PER_PAUC       = 2;
static const uint8_t MI_PER_MC          = 1;
static const uint8_t PHB_PER_PEC        = 3;
static const uint8_t PARTIAL_GOOD_CORE0 = 13;
static const uint8_t PARTIAL_GOOD_PAU0  = 13;
static const uint8_t OCMB_PER_PROC = 16;

//
// Define what a platform handle looks like. For Hostboot,
// for example, this might be a void*. For the SBE, this
// will be a uint32_t ...
//
namespace fapi2
{
    template<TargetType K, MulticastType M, typename V>
    static constexpr void plat_apply_target_limits(void)
    {}

    typedef enum plat_target_type
    {
        PPE_TARGET_TYPE_NONE              = 0x00,
        PPE_TARGET_TYPE_PROC_CHIP         = 0x01,
        PPE_TARGET_TYPE_PAU               = 0x02,
        PPE_TARGET_TYPE_CORE              = 0x03,
        PPE_TARGET_TYPE_EQ                = 0x04,
        // 0x05 is free
        PPE_TARGET_TYPE_PERV              = 0x06,
        PPE_TARGET_TYPE_PEC               = 0x07,
        PPE_TARGET_TYPE_SYSTEM            = 0x08,
        PPE_TARGET_TYPE_PHB               = 0x09,
        PPE_TARGET_TYPE_MI                = 0x0A,
        PPE_TARGET_TYPE_MC                = 0x0B,
        PPE_TARGET_TYPE_PAUC              = 0x0C,
        PPE_TARGET_TYPE_IOHS              = 0x0D,
        PPE_TARGET_TYPE_NMMU              = 0x0E,
        PPE_TARGET_TYPE_OCMB              = 0x0F,
        // 0x0F is free
    } plat_target_type_t;

    constexpr struct {
        bool is_chiplet;
        unsigned int first_chiplet;
    } _PLAT_TARGET_TYPE_TRAITS[] = {
        { false, 0                   }, // PPE_TARGET_TYPE_NONE
        { false, 0                   }, // PPE_TARGET_TYPE_PROC_CHIP
        { false, 0                   }, // PPE_TARGET_TYPE_PAU
        { false, 0                   }, // PPE_TARGET_TYPE_CORE
        { true,  EQ_CHIPLET_OFFSET   }, // PPE_TARGET_TYPE_EQ
        { false, 0                   }, // 0x05
        { true,  0                   }, // PPE_TARGET_TYPE_PERV
        { true,  PEC_CHIPLET_OFFSET  }, // PPE_TARGET_TYPE_PEC
        { false, 0                   }, // PPE_TARGET_TYPE_SYSTEM
        { false, 0                   }, // PPE_TARGET_TYPE_PHB
        { false, 0                   }, // PPE_TARGET_TYPE_MI
        { true,  MC_CHIPLET_OFFSET   }, // PPE_TARGET_TYPE_MC
        { true,  PAUC_CHIPLET_OFFSET }, // PPE_TARGET_TYPE_PAUC
        { true,  IOHS_CHIPLET_OFFSET }, // PPE_TARGET_TYPE_IOHS
        { false, 0                   }, // PPE_TARGET_TYPE_NMMU
        { false, 0                   }, // PPE_TARGET_TYPE_OCMB
        { false, 0                   }, // 0x10
    };

    template <plat_target_type T>
    constexpr bool plat_target_is_chiplet()
    {
        return _PLAT_TARGET_TYPE_TRAITS[T].is_chiplet;
    }

    template <plat_target_type T>
    constexpr unsigned int plat_target_first_chiplet()
    {
        static_assert(plat_target_is_chiplet<T>(),
                      "Destination type is not a chiplet");

        return _PLAT_TARGET_TYPE_TRAITS[T].first_chiplet;
    }

    /*
     * The plat_target_handle bit layout is designed so that when the bits
     * used for internal bookkeeping are masked off, we end up with a PIB
     * base address that can be ORed onto a register address constant to
     * yield a final PIB address.
     */
    typedef union plat_target_handle {
        uint32_t value;
        struct {
            /* chiplet number is overloaded for multicast vs. unicast targets */
            union {
                uint32_t chiplet_num      :8; // used for targeting
                struct {
                    uint32_t              :1; // unused in the multicast case, must be zero for unicast targets
                    uint32_t is_multicast :1; // used for targeting
                    uint32_t mcast_type   :3; // used for targeting
                    uint32_t mcast_group  :3; // used for targeting
                } __attribute__((packed));
            } __attribute__((packed));
            uint32_t type_target_num      :8; // used internally
            /* lower 16 bits start here */
            union {
                // core_select (for core targets) and ring_id (for non-core targets)
                // overlap, so we need another union here, ugh
                struct {
                    uint32_t core_select  :4; // used for targeting
                    uint32_t              :12;
                } __attribute__((packed));
                struct {
                    uint32_t              :2;
                    uint32_t ring_id      :4; // used for targeting
                    uint32_t sat_id       :4; // used for targeting
                    uint32_t              :1; // unused
                    uint32_t functional   :1; // used internally
                    uint32_t type         :4; // used internally
                } __attribute__((packed));
            } __attribute__((packed));
        } fields __attribute__((packed));

		/* Return the target value with all internal bits masked away */
		uint32_t getPIBAddress() const {
			return value & 0x7F00FFC0;
		}

        ///
        /// @brief Plat target handle constructor
        ///
        /// @param  i_value Value to instantiate handle with
        ///
        explicit plat_target_handle(uint32_t i_value = 0):value(i_value)
            {
                static_assert(sizeof(*this) == sizeof(uint32_t),
                              "Incorrect size of plat_target, check your unions!");
            }

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
        /// @brief Returns the multicast group from the target
        ///
        /// @return The multicast group for this target
        ///
        uint32_t getMultiCastGroup() const
        {
             return fields.mcast_group;
        }

        ///
        /// @brief Returns the status of the multicast bit
        ///
        /// @return The multicast bit of the target
        bool isMulticastSet() const
        {
            return fields.is_multicast;
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
        /// @brief Get this target's parent
        ///
        /// @param [in] The fapi2 type of the requested parent
        /// @return Plat target handle to the parent target
        ///
        plat_target_handle getParent(const TargetType i_parentType) const;

        ///
        /// @brief Break a multicast target up into unicast targets
        ///
        /// @param [in] i_include_nonfunctional Return also targets that are not functional
        /// @param [out] o_children             A vector of child target handles
        ///
        void getMulticastChildren(
            const bool i_include_nonfunctional,
            std::vector<plat_target_handle> &o_children) const;

        ///
        /// @brief Enumerate all targets with a given type that match this target's chiplet ID
        ///
        /// @param [in] i_child_type            Plat target type of requested children
        /// @param [in] i_include_nonfunctional Return also targets that are not functional
        /// @param [out] o_children             A vector of child target handles
        void getChipletChildren(
            const plat_target_type_t i_child_type,
            const bool i_include_nonfunctional,
            std::vector<plat_target_handle> &o_children) const;

        ///
        /// @brief Enumerate all targets with a given type regardless of chiplet ID
        ///
        /// @param [in] i_child_type            Plat target type of requested children
        /// @param [in] i_include_nonfunctional Return also targets that are not functional
        /// @param [out] o_children             A vector of child target handles
        void getProcChildren(
            const plat_target_type_t i_child_type,
            const bool i_include_nonfunctional,
            std::vector<plat_target_handle> &o_children) const;

        ///
        /// @brief Get proc chip target's children - filtered
        ///
        /// @param [in] i_filter     Target filter
        /// @param [in] i_state      Required state of the children
        /// @param [out] o_children  A vector of child target handles
        ///
        void getChildren(
            const TargetFilter i_filter,
            const bool i_include_nonfunctional,
            std::vector<plat_target_handle>& o_children) const;

        ///
        /// @brief Gets the plat target handle as a uint32
        ///
        /// @return Plat target handle as a uint32_t
        ///
        operator uint32_t() const {return value;}

        ///
        /// @brief Convert any target to a chiplet target of specified
        ///        type, stripping any unit specific address bits.
        ///
        /// This is used by getParent() to map a chiplet child target to a
        /// chiplet target, or any target to PERV. It returns a copy of this
        /// target that has all unit-level address bits cleared and
        /// type_target_num updated to match the new target type.
        ///
        template <plat_target_type K>
        plat_target_handle convertToChiplet() const
        {
            static_assert(plat_target_is_chiplet<K>(),
                          "Destination type is not a chiplet");

            plat_target_handle copy = *this;

            // type becomes the new type
            copy.fields.type = K;

            // type_target_num becomes the chiplet ID minus the first chiplet
            // This only works for chiplet targets of course, but there it works.
            copy.fields.type_target_num =
                copy.fields.chiplet_num - plat_target_first_chiplet<K>();

            // Clear out any unit level address bits
            copy.fields.core_select = 0;
            copy.fields.ring_id = 0;
            copy.fields.sat_id = 0;
            return copy;
        }
    } plat_target_handle_t;
};

#endif

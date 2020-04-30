/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/hwpf/target.H $                                           */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2012,2020                        */
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
 * @file target.H
 * @brief platform specializations for fapi2 targets
 */

#ifndef __FAPI2_TARGET__
#define __FAPI2_TARGET__

#include <plat_target.H>
#include <plat_target_parms.H>
#include <fapi2_target.H>
#include <plat_trace.H>
#include <utils.H>
#include <stdint.h>
#include <vector>
#include <plat_target_utils.H>
#include <multicast_group_defs.H>

extern "C"
{
    extern std::vector<fapi2::plat_target_handle_t> G_vec_targets;
}

struct ScomAddr
{
    ScomAddr(uint32_t i_addr) : iv_addr(i_addr)
    {
    }

    operator uint32_t()
    {
        return iv_addr;
    }

    union
    {
        struct
        {
            uint32_t iv_unused : 1;
            uint32_t iv_multicast : 1;
            uint32_t iv_chiplet : 6;
            uint32_t iv_pibMaster : 4;
            uint32_t iv_port : 4;
            uint32_t iv_unused2 : 2;
            uint32_t iv_ring : 4;
            uint32_t iv_satId : 4;
            uint32_t iv_satOffset : 6;
        };
        uint32_t iv_addr;
    };
};


namespace fapi2
{

    template<TargetType T>
    constexpr plat_target_type_t fapiTargetTypeToPlatTargetType()
    {
        static_assert(T == TARGET_TYPE_PROC_CHIP, "Invalid or composite target type");
        return PPE_TARGET_TYPE_NONE;
    }

    template<>
    constexpr plat_target_type_t fapiTargetTypeToPlatTargetType<TARGET_TYPE_PROC_CHIP>()
    {
        return PPE_TARGET_TYPE_PROC_CHIP;
    }

    template<>
    constexpr plat_target_type_t fapiTargetTypeToPlatTargetType<TARGET_TYPE_EQ>()
    {
        return PPE_TARGET_TYPE_EQ;
    }

    template<>
    constexpr plat_target_type_t fapiTargetTypeToPlatTargetType<TARGET_TYPE_CORE>()
    {
        return PPE_TARGET_TYPE_CORE;
    }

    template<>
    constexpr plat_target_type_t fapiTargetTypeToPlatTargetType<TARGET_TYPE_MI>()
    {
        return PPE_TARGET_TYPE_MI;
    }

    template<>
    constexpr plat_target_type_t fapiTargetTypeToPlatTargetType<TARGET_TYPE_PHB>()
    {
        return PPE_TARGET_TYPE_PHB;
    }

    template<>
    constexpr plat_target_type_t fapiTargetTypeToPlatTargetType<TARGET_TYPE_PEC>()
    {
        return PPE_TARGET_TYPE_PEC;
    }

    template<>
    constexpr plat_target_type_t fapiTargetTypeToPlatTargetType<TARGET_TYPE_MC>()
    {
        return PPE_TARGET_TYPE_MC;
    }

    template<>
    constexpr plat_target_type_t fapiTargetTypeToPlatTargetType<TARGET_TYPE_PAUC>()
    {
        return PPE_TARGET_TYPE_PAUC;
    }

    template<>
    constexpr plat_target_type_t fapiTargetTypeToPlatTargetType<TARGET_TYPE_PAU>()
    {
        return PPE_TARGET_TYPE_PAU;
    }

    template<>
    constexpr plat_target_type_t fapiTargetTypeToPlatTargetType<TARGET_TYPE_IOHS>()
    {
        return PPE_TARGET_TYPE_IOHS;
    }

    template<>
    constexpr plat_target_type_t fapiTargetTypeToPlatTargetType<TARGET_TYPE_PERV>()
    {
        return PPE_TARGET_TYPE_PERV;
    }

    template<>
    constexpr plat_target_type_t fapiTargetTypeToPlatTargetType<TARGET_TYPE_NMMU>()
    {
        return PPE_TARGET_TYPE_NMMU;
    }

    ///
    /// @brief Assignment Operator.
    /// @param[in] i_right Reference to Target to assign from.
    /// @return Reference to 'this' Target
    ///
    template<TargetType K, MulticastType M, typename V>
    Target<K, M, V>& Target<K, M, V>::operator=(const Target& i_right)
    {
        this->iv_handle.value = i_right.iv_handle.value;
        return *this;
    }

    ///
    /// @brief Equality Comparison Operator
    /// @param[in] i_right Reference to Target to compare.
    /// @return bool. True if equal.
    /// @note Platforms need to define this so that the physical
    /// targets are determined to be equivilent rather than just the handles
    ///
    template<TargetType K, MulticastType M, typename V>
    bool Target<K, M, V>::operator==(const Target& i_right) const
    {
        if (this->iv_handle.value == i_right.iv_handle.value)
            return true;
        else
            return false;
    }

    ///
    /// @brief Inquality Comparison Operator
    /// @param[in] i_right Reference to Target to compare.
    /// @return bool. True if not equal.
    /// @note Platforms need to define this so that the physical
    /// targets are determined to be equivilent rather than just the handles
    ///
    template<TargetType K, MulticastType M, typename V>
    bool Target<K, M, V>::operator!=(const Target& i_right) const
    {
        if (this->iv_handle.value != i_right.iv_handle.value)
            return true;
        else
            return false;
    }

    ///
    /// @brief Platform MC handle creation moved into function call
    /// @param[in] i_plat_type requested type of multicast target
    /// @param[in] i_group abstract group specification, will be mapped to a HW group
    ///
    plat_target_handle_t createPlatMCHandle(const plat_target_type_t i_plat_type, const MulticastGroup i_group, uint8_t i_core_select);

    template<TargetType K, MulticastType M, typename V>
    template<TargetType T>
    inline ReturnCodes Target<K, M, V>::reduceType(Target<T, M, V> &o_target) const
    {
        const TargetType l_type = iv_handle.getFapiTargetType() |
            (iv_handle.fields.is_multicast ? TARGET_TYPE_MULTICAST : TARGET_TYPE_NONE);

        if ((l_type & T) != l_type)
        {
            return FAPI2_RC_INVALID_PARAMETER;
        }

        o_target = get();
        return FAPI2_RC_SUCCESS;
    }

    constexpr TargetType TARGET_TYPE_PROC_CHILDREN = TARGET_TYPE_CHIPLETS;
    constexpr TargetType TARGET_TYPE_PERV_CHILDREN = TARGET_TYPE_CHIPLETS;
    constexpr TargetType TARGET_TYPE_EQ_CHILDREN = TARGET_TYPE_CORE;
    constexpr TargetType TARGET_TYPE_PAUC_CHILDREN = TARGET_TYPE_PAU;
    constexpr TargetType TARGET_TYPE_MC_CHILDREN = TARGET_TYPE_MI;
    constexpr TargetType TARGET_TYPE_PEC_CHILDREN = TARGET_TYPE_PHB;

    ///
    /// @brief Get this target's immediate parent
    /// @tparam T The type of the parent
    /// @return Target<T, M, V> a target representing the parent
    ///
    template<TargetType K, MulticastType M, typename V>
    template<TargetType T>
    inline Target<T, M, V> Target<K, M, V>::getParent(void) const
    {
        // This is a rather complicated function with many special cases,
        // so let's lay down a rigid structure:
        //
        // This inline function is only a dispatcher in the form of nested
        // if/else if statements. Each case must first check its prerequisites
        // before performing a single return statement.
        //
        // If the code necessary for a given case is more complicated than a
        // single return statement, it must be factored out into a separate
        // function. getParent() is not being called often so we can accept
        // the runtime overhead in favor of small code.

        constexpr TargetType T_NO_MCAST = T & ~TARGET_TYPE_MULTICAST;
        constexpr TargetType K_NO_MCAST = K & ~TARGET_TYPE_MULTICAST;

        check_getParent<T>();

        // Anything to PROC_CHIP
        if (T == TARGET_TYPE_PROC_CHIP)
        {
            return G_vec_targets[CHIP_TARGET_OFFSET];
        }
        // Anything to PERV, regardless of multicast or not
        else if (T_NO_MCAST == TARGET_TYPE_PERV)
        {
            STATIC_CHECK((K_NO_MCAST & ~TARGET_TYPE_PERV_CHILDREN) == 0,
                         "Invalid child target for PERV parent");
            return get().template convertToChiplet<PPE_TARGET_TYPE_PERV>();
        }
        // Anything to EQ, regardless of multicast or not
        else if (T_NO_MCAST == TARGET_TYPE_EQ)
        {
            STATIC_CHECK((K_NO_MCAST & ~TARGET_TYPE_EQ_CHILDREN) == 0,
                         "Invalid child target for EQ parent");
            return get().template convertToChiplet<PPE_TARGET_TYPE_EQ>();
        }
        // Anything to PAUC, regardless of multicast or not
        else if (T_NO_MCAST == TARGET_TYPE_PAUC)
        {
            STATIC_CHECK((K_NO_MCAST & ~TARGET_TYPE_PAUC_CHILDREN) == 0,
                         "Invalid child target for PAUC parent");
            return get().template convertToChiplet<PPE_TARGET_TYPE_PAUC>();
        }
        // Anything to MC, regardless of multicast or not
        else if (T_NO_MCAST == TARGET_TYPE_MC)
        {
            STATIC_CHECK((K_NO_MCAST & ~TARGET_TYPE_MC_CHILDREN) == 0,
                         "Invalid child target for MC parent");
            return get().template convertToChiplet<PPE_TARGET_TYPE_MC>();
        }
        // Anything to PEC, regardless of multicast or not
        else if (T_NO_MCAST == TARGET_TYPE_PEC)
        {
            STATIC_CHECK((K_NO_MCAST & ~TARGET_TYPE_PEC_CHILDREN) == 0,
                         "Invalid child target for PEC parent");
            return get().template convertToChiplet<PPE_TARGET_TYPE_PEC>();
        }

        // If we fall through to here, there is no defined code path to handle the
        // combination of T and K -- let's fail loudly.
        STATIC_COMPILE_ERROR("Unsupported combination of parent and child for getParent()");
    }

    /// @brief Get this target's children - handles EQ/CORE/PEC conversions
    /// @tparam K The type of parent
    /// @tparam V The plat target handle type
    /// @tparam T The type of child
    /// @param[in] i_state The desired TargetState of the children
    /// @return std::vector<Target<T, M, V> > a vector of present/functional
    /// children
    /// @warning The children are returned in order, ex child[0] is
    /// std::vector[0]
    template<TargetType K, MulticastType M, typename V>
    template<TargetType T>
    std::vector<Target<T, M, V>>
    Target<K, M, V>::getChildren(const TargetState i_state) const
    {
        constexpr TargetType K_NO_PROC  = K & ~TARGET_TYPE_PROC_CHIP;
        constexpr TargetType K_NO_MCAST = K & ~TARGET_TYPE_MULTICAST;
        constexpr plat_target_type_t P = fapiTargetTypeToPlatTargetType<T>();

        const bool l_include_nonfunctional = i_state == TARGET_STATE_PRESENT;

        check_getChildren<T>();

        static_assert(sizeof(Target<T, M, V>) == sizeof(plat_target_handle_t),
                      "Sizes of plat target and FAPI target must match");

        // Using the same kind of "dispatcher" approach as getParent()
        std::vector<Target<T, M, V> > l_children;
        auto &l_cast_children = reinterpret_cast<std::vector<plat_target_handle>&>(l_children);
        if ((K & TARGET_TYPE_MULTICAST) && (T == K_NO_MCAST))
        {
            get().getMulticastChildren(l_include_nonfunctional, l_cast_children);
            return l_children;
        }
        else if (K_NO_PROC == TARGET_TYPE_EQ)
        {
            STATIC_CHECK((T & ~TARGET_TYPE_EQ_CHILDREN) == 0,
                         "Invalid child type for EQ target");
            get().getChipletChildren(P, l_include_nonfunctional, l_cast_children);
            return l_children;
        }
        else if (K_NO_PROC == TARGET_TYPE_PAUC)
        {
            STATIC_CHECK((T & ~TARGET_TYPE_PAUC_CHILDREN) == 0,
                         "Invalid child type for EQ target");
            get().getChipletChildren(P, l_include_nonfunctional, l_cast_children);
            return l_children;
        }
        else if (K_NO_PROC == TARGET_TYPE_PEC)
        {
            STATIC_CHECK((T & ~TARGET_TYPE_PEC_CHILDREN) == 0,
                         "Invalid child type for EQ target");
            get().getChipletChildren(P, l_include_nonfunctional, l_cast_children);
            return l_children;
        }
        else if (K_NO_PROC == TARGET_TYPE_MC)
        {
            STATIC_CHECK((T & ~TARGET_TYPE_MC_CHILDREN) == 0,
                         "Invalid child type for EQ target");
            get().getChipletChildren(P, l_include_nonfunctional, l_cast_children);
            return l_children;
        }
        else if (K == TARGET_TYPE_PROC_CHIP)
        {
            STATIC_CHECK((T & ~TARGET_TYPE_PROC_CHILDREN) == 0,
                         "Invalid child type for PROC_CHIP targets");
            get().getProcChildren(P, l_include_nonfunctional, l_cast_children);
            return l_children;
        }
        else
        {
            // If we fall through to here, there is no defined code path to handle the
            // combination of T and K -- let's fail loudly.
            STATIC_COMPILE_ERROR("Unsupported combination of parent and child for getChildren()");
            return l_children;
        }
    }

    // Specialization of getChildren, filtered for the chip target
    template<TargetType K, MulticastType M, typename V>
    template<TargetType T>
    std::vector<Target<T, M, V> >
    Target<K, M, V>::getChildren(const TargetFilter i_filter,
                                          const TargetState i_state) const
    {
        const bool l_include_nonfunctional = i_state == TARGET_STATE_PRESENT;
        check_getChildren<T>();

        static_assert(sizeof(Target<T, M, V>) == sizeof(plat_target_handle_t),
                      "Sizes of plat target and FAPI target must match");

        static_assert(K == TARGET_TYPE_PROC_CHIP,
                      "Parent target must be the proc chip");

        static_assert((T & ~TARGET_TYPE_PROC_CHILDREN) == 0,
                      "Child target type must be a pervasive chiplet");

        std::vector<Target<T, M, V> > l_children;
        get().getChildren(i_filter, l_include_nonfunctional,
                          reinterpret_cast<std::vector<plat_target_handle_t>&>(l_children));
        return l_children;
    }

    template< TargetType K, MulticastType M, typename V>
    template< TargetType T, MulticastType O >
    inline Target<T | TARGET_TYPE_MULTICAST, O, V>
    Target<K, M, V>::getMulticast(const MulticastGroup i_group) const
    {
        check_getMulticast<T, O>();
        constexpr plat_target_type_t l_plat_type = fapiTargetTypeToPlatTargetType<T>();
        return createPlatMCHandle(l_plat_type, i_group, 0);
    }

    template< TargetType K, MulticastType M, typename V>
    template< MulticastType O >
    inline Target<TARGET_TYPE_CORE | TARGET_TYPE_MULTICAST, O, V>
    Target<K, M, V>::getMulticast(const MulticastGroup i_group, const MulticastCoreSelect i_cores) const
    {
        check_getMulticast<TARGET_TYPE_CORE, O>();
        return createPlatMCHandle(PPE_TARGET_TYPE_CORE, i_group, i_cores);
    }

    ///
    /// @brief Is the target functional?
    /// @return true if target is functional, false if non-functional
    ///
    template<TargetType K, MulticastType M, typename V>
    inline bool
    Target<K, M, V>::isFunctional(void) const
    {
        return static_cast<plat_target_handle_t>(get()).getFunctional();
    }

    ///
    /// @brief Returns the chiplet number for this Target
    /// @return The chiplet number
    ///
    template<TargetType K, MulticastType M, typename V>
    inline uint8_t
    Target<K, M, V>::getChipletNumber(void) const
    {
        check_getChipletNumber();
        return static_cast<plat_target_handle_t>(iv_handle).fields.chiplet_num;
    }

    ///
    /// @brief Return the string interpretation of this target
    /// @tparam T The type of the target
    /// @param[in] i_target Target<T>
    /// @param[in] i_buffer buffer to write in to
    /// @param[in] i_bsize size of the buffer
    /// @return void
    /// @post The contents of the buffer is replaced with the string
    /// representation of the target
    ///
    template< TargetType T, MulticastType M, typename V >
    inline void toString(const Target<T, M, V>& i_target, char* i_buffer, size_t i_bsize)
    {
        snprintf(i_buffer, i_bsize, "Target 0x%lx/0x%x", i_target.get(), T);
    }

    ///
    /// @brief Return the string interpretation of this target
    /// @tparam T The type of the target
    /// @tparam B The type of the buffer
    /// @param[in] A pointer to the Target<T, M, V>
    /// @param[in] i_buffer buffer to write in to
    /// @param[in] i_bsize size of the buffer
    /// @return void
    /// @post The contents of the buffer is replaced with the string
    /// representation of the target
    ///
    template< TargetType T, MulticastType M, typename V >
    inline void toString(const Target<T, M, V>* i_target, char* i_buffer, size_t i_bsize)
    {
        snprintf(i_buffer, i_bsize, "Target 0x%lx/0x%x", i_target->get(), T);
    }

    ///
    /// @brief Get an enumerated target of a specific type
    /// @tparam T The type of the target
    /// @param[in] Ordinal representing the ordinal number of
    /// the desired target
    /// @return Target<T, M, V> the target requested
    ///
    template<TargetType T, MulticastType M, typename V>
    inline Target<T, M, V> getTarget(uint64_t Ordinal)
    {
        // For testing
        return Target<T, M, V>(Ordinal);
    }

    template<TargetType T, MulticastType M, typename V>
    inline void Target<T, M, V>::mcUpdateHandle()
    {
        if (iv_handle.fields.is_multicast)
        {
            iv_handle.fields.mcast_type = static_cast<int>(M);
        }
    }

    template<TargetType K, MulticastType M, typename V>
    inline MulticastCoreSelect
    Target<K, M, V>::_getCoreSelect(void) const
    {
        // Platform can return the core select value stored in it's Target handle
        return (MulticastCoreSelect)this->iv_handle.fields.core_select;
    }

    /// @brief Set up or replace the multicast group mapping for a given chip
    ///
    /// @param[in] i_mappings A list of multicast group mappings
    ///
    /// @return FAPI2_RC_SUCCESS if the map was updated, otherwise error code.
    ReturnCode plat_setMcMap(const std::vector< MulticastGroupMapping > &i_mappings);
}

#endif

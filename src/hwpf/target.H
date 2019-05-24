/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/hwpf/target.H $                                           */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2012,2019                        */
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
    constexpr plat_target_type_t fapiTargetTypeToPlatTargetType<TARGET_TYPE_EX>()
    {
        return PPE_TARGET_TYPE_EX;
    }

    template<>
    constexpr plat_target_type_t fapiTargetTypeToPlatTargetType<TARGET_TYPE_MCS>()
    {
        return PPE_TARGET_TYPE_MCS;
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
    constexpr plat_target_type_t fapiTargetTypeToPlatTargetType<TARGET_TYPE_MCBIST>()
    {
        return PPE_TARGET_TYPE_MCBIST;
    }

    template<>
    constexpr plat_target_type_t fapiTargetTypeToPlatTargetType<TARGET_TYPE_MC>()
    {
        return PPE_TARGET_TYPE_MC;
    }

    template<>
    constexpr plat_target_type_t fapiTargetTypeToPlatTargetType<TARGET_TYPE_PERV>()
    {
        return PPE_TARGET_TYPE_PERV;
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
    /// @brief Get this target's immediate parent
    /// @tparam T The type of the parent
    /// @return Target<T, M, V> a target representing the parent
    ///
    template<TargetType K, MulticastType M, typename V>
    template<TargetType T>
    inline Target<T, M, V> Target<K, M, V>::getParent(void) const
    {
        constexpr TargetType TARGET_TYPE_PROC_CHILDREN =
            TARGET_TYPE_EQ | TARGET_TYPE_PERV | TARGET_TYPE_EX |
            TARGET_TYPE_MCBIST | TARGET_TYPE_CORE |
            TARGET_TYPE_MCS | TARGET_TYPE_PROC_CHIP | TARGET_TYPE_PHB |
            TARGET_TYPE_MC | TARGET_TYPE_MI;

        static_assert(((
                       ((K & TARGET_TYPE_PROC_CHILDREN) != TARGET_TYPE_NONE) ||
                       (K == TARGET_TYPE_ALL)) &&
                      ((T == TARGET_TYPE_EQ) ||
                       (T == TARGET_TYPE_EX) ||
                       (T == TARGET_TYPE_PROC_CHIP) ||
                       (T == TARGET_TYPE_PERV))),
                      "Invalid parent/child target type passed");

        static_assert(!((K == TARGET_TYPE_EQ) &&
                        (T != TARGET_TYPE_PERV) &&
                        (T != TARGET_TYPE_PROC_CHIP)),
                        "Invalid parent for EQ target, must be PERV or "
                        "PROC_CHIP");

        static_assert(!((T == TARGET_TYPE_PROC_CHIP) &&
                        ((K & TARGET_TYPE_PROC_CHILDREN) == TARGET_TYPE_NONE)),
                        "Parent proc chip invalid for this target type");

        static_assert(!((K == TARGET_TYPE_MCBIST) &&
                        (T != TARGET_TYPE_PERV) &&
                        (T != TARGET_TYPE_PROC_CHIP)),
                        "Invalid parent for MCBIST target, must be PERV or "
                        "PROC_CHIP");

        static_assert(!((K == TARGET_TYPE_MC) &&
                        (T != TARGET_TYPE_PERV) &&
                        (T != TARGET_TYPE_PROC_CHIP)),
                        "Invalid parent for MC target, must be PERV or "
                        "PROC_CHIP");

        static_assert(!((K == TARGET_TYPE_CORE) &&
                        (T != TARGET_TYPE_PERV) &&
                        (T != TARGET_TYPE_PROC_CHIP) &&
                        (T != TARGET_TYPE_EQ) &&
                        (T != TARGET_TYPE_EX)),
                        "Invalid parent for CORE target, must be PERV or "
                        "PROC_CHIP or EQ or EX");

        static_assert(!((K == TARGET_TYPE_PERV) &&
                        (T != TARGET_TYPE_PERV) &&
                        (T != TARGET_TYPE_PROC_CHIP)),
                        "Invalid parent for PERV target, must be PERV or "
                        "PROC_CHIP");

        static_assert(!((K == TARGET_TYPE_EX) &&
                        (T != TARGET_TYPE_PROC_CHIP) &&
                        (T != TARGET_TYPE_EQ)),
                        "Invalid parent for EX target, must be PERV or "
                        "PROC_CHIP or EQ");

        static_assert((!(K & TARGET_TYPE_MULTICAST)) ||
                        (T == TARGET_TYPE_PROC_CHIP) ||
                        (T == TARGET_TYPE_PERV),
                        "Invalid parent for multicast target, must be PERV "
                        "or PROC_CHIP");

        static_assert(!(((K ^ T) & TARGET_TYPE_MULTICAST) && ((T & TARGET_TYPE_CHIPLETS) != TARGET_TYPE_NONE)),
                      "Parent target for a multicast target must also be multicast or PROC_CHIP");

        if((TARGET_TYPE_PERV | TARGET_TYPE_MULTICAST) == T) // EQ/EC/MCBIST/MC/PERV ===> PERV
        {
            return static_cast<V>(this->iv_handle);
        }
        if(TARGET_TYPE_PERV == T)
        {
            return static_cast<V>(this->iv_handle);
        }
        if(TARGET_TYPE_PROC_CHIP == T) // EQ/EC/EX/MCBIST/MC/PERV ===> PROC
        {
            return static_cast<V>(G_vec_targets[CHIP_TARGET_OFFSET]);
        }
        if((TARGET_TYPE_EQ == T) && (TARGET_TYPE_CORE == K)) // EC ===> EQ
        {
            return static_cast<V>(G_vec_targets[(static_cast<plat_target_handle_t>(get()).getTargetInstance() / CORES_PER_QUAD) + EQ_TARGET_OFFSET]);
        }
        if((TARGET_TYPE_EQ == T) && (TARGET_TYPE_EX == K)) // EX ===> EQ
        {
            return static_cast<V>(G_vec_targets[(static_cast<plat_target_handle_t>(get()).getTargetInstance() / EX_PER_QUAD) + EQ_TARGET_OFFSET]);
        }
        if(TARGET_TYPE_EX == T) // EC ===> EX
        {
            return static_cast<V>(G_vec_targets[(static_cast<plat_target_handle_t>(get()).getTargetInstance() / CORES_PER_EX) + EX_TARGET_OFFSET]);
        }
        constexpr TargetType TARGET_TYPE_CORE_OR_EX = TARGET_TYPE_CORE | TARGET_TYPE_EX;
        if((TARGET_TYPE_EQ == T) && ((K & (TARGET_TYPE_CORE_OR_EX)) != TARGET_TYPE_NONE))
        {
            return get().getParent(T);
        }
    }

    /// @brief Get this target's children - handles EQ/EX/EC conversions
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
        constexpr TargetType L = static_cast<TargetType>(K & ~(TARGET_TYPE_PROC_CHIP));
        constexpr plat_target_type_t P = fapiTargetTypeToPlatTargetType<T>();

        static_assert(sizeof(Target<T, M, V>) == sizeof(plat_target_handle_t),
                      "Sizes of plat target and FAPI target must match");

        static_assert(((L == TARGET_TYPE_EQ) || (L == TARGET_TYPE_EX) || (K == TARGET_TYPE_PROC_CHIP) ||
                      (((K & TARGET_TYPE_MULTICAST) != 0) && ((K & TARGET_TYPE_MULTICASTABLE) != 0))),
                      "Invalid parent passed to getChildren");
        // valid children for EQ
        // EQ -> CORE
        // EQ -> EX
        static_assert(!((L == fapi2::TARGET_TYPE_EQ) &&
                        (T != fapi2::TARGET_TYPE_CORE) &&
                        (T != fapi2::TARGET_TYPE_EX)),
                      "improper child of fapi2::TARGET_TYPE_EQ");

        // valid children for EX
        // EX -> CORE
        static_assert(!((L == fapi2::TARGET_TYPE_EX) &&
                        (T != fapi2::TARGET_TYPE_CORE)),
                      "improper child of fapi2::TARGET_TYPE_EX");

        // valid children for multicast targets are the same with multicast stripped
        static_assert(!((K & TARGET_TYPE_MULTICAST) &&
                        (K != (T | TARGET_TYPE_MULTICAST))),
                      "improper child of fapi2::TARGET_TYPE_MULTICAST");

        std::vector<Target<T, M, V> > l_children;
        static_cast<plat_target_handle_t>(get()).getChildren(K, T, P, i_state, reinterpret_cast<std::vector<plat_target_handle>&>(l_children));
        return l_children;
    }

    // Specialization of getChildren, filtered for the chip target
    template<TargetType K, MulticastType M, typename V>
    template<TargetType T>
    std::vector<Target<T, M, V> >
    Target<K, M, V>::getChildren(const TargetFilter i_filter,
                                          const TargetState i_state) const
    {
        static_assert(sizeof(Target<T, M, V>) == sizeof(plat_target_handle_t),
                      "Sizes of plat target and FAPI target must match");

        static_assert((K == TARGET_TYPE_PROC_CHIP), "Parent target must be the proc chip");
        static_assert((T == TARGET_TYPE_EQ) || (T == TARGET_TYPE_CORE)
                || (T == TARGET_TYPE_PERV) || (T == TARGET_TYPE_MCBIST)
                || (T == TARGET_TYPE_MC),
                "Child target type must be a pervasive chiplet");

        std::vector<Target<T> > l_children;

        (static_cast<plat_target_handle_t>(get())).getChildren(i_filter, i_state, reinterpret_cast<std::vector<plat_target_handle_t>&>(l_children));

        return l_children;
    }

    ///
    /// @brief Platform MC handle creation moved into function call
    /// @param[in] i_plat_type requested type of multicast target
    /// @param[in] i_group abstract group specification, will be mapped to a HW group
    ///
    plat_target_handle_t createPlatMCHandle(const plat_target_type_t i_plat_type, const MulticastGroup i_group, uint8_t i_core_select);

    template< TargetType K, MulticastType M, typename V>
    template< TargetType T, MulticastType O >
    inline Target<T | TARGET_TYPE_MULTICAST, O, V>
    Target<K, M, V>::getMulticast(const MulticastGroup i_group) const
    {
        static_assert((K == TARGET_TYPE_PROC_CHIP), "Parent target must be the proc chip");
        static_assert((T & TARGET_TYPE_MULTICASTABLE) != 0,
                "Requested target type must be part of TARGET_TYPE_MULTICASTABLE");
        static_assert(bitCount<T>::count == 1,
                "Requested target type must be a single type, TARGET_TYPE_MULTICAST will be added automatically");

        constexpr plat_target_type_t l_plat_type = fapiTargetTypeToPlatTargetType<T>();
        return createPlatMCHandle(l_plat_type, i_group, 0);
    }

    template< TargetType K, MulticastType M, typename V>
    template< MulticastType O >
    inline Target<TARGET_TYPE_CORE | TARGET_TYPE_MULTICAST, O, V>
    Target<K, M, V>::getMulticast(const MulticastGroup i_group, const MulticastCoreSelect i_cores) const
    {
        static_assert((K == TARGET_TYPE_PROC_CHIP), "Parent target must be the proc chip");

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
        static_assert((K & TARGET_TYPE_MULTICAST) == 0, "getChipletNumber is not supported for multicast targets");
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

    /// @brief Set up or replace the multicast group mapping for a given chip
    ///
    /// @param[in] i_mappings A list of multicast group mappings
    ///
    /// @return FAPI2_RC_SUCCESS if the map was updated, otherwise error code.
    ReturnCode plat_setMcMap(const std::vector< MulticastGroupMapping > &i_mappings);
}

#endif

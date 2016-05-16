/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: hwpf/include/plat/target.H $                                  */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2012,2016                        */
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

extern "C"
{
    extern std::vector<fapi2::plat_target_handle_t> G_vec_targets;
}

static const uint8_t CORES_PER_QUAD = 4;
static const uint8_t EX_PER_QUAD = 2;
static const uint8_t CORES_PER_EX = 2;

namespace fapi2
{
    /// @brief Create a Target, with a value
    /// @param[in] Value the value (i.e., specific element this
    /// target represents, or pointer)
    /// @note Platforms can mangle the value and K to get a
    /// single uint64_t in value which represents all the information
    /// they might need. value( K | V ), for example
    ///
    template<TargetType K, typename V>
    Target<K, V>::Target(V Value)
    {
        uint32_t l_plat_argument = Value.value;
        // Already formed target handle?
        if(Value.fields.valid == 1)
        {
            this->iv_handle = Value;
            return;
        }

        this->iv_handle.value = 0;
        if(K & TARGET_TYPE_PROC_CHIP)
        {
            this->iv_handle.fields.chiplet_num = 0;
            this->iv_handle.fields.type = TARGET_TYPE_PROC_CHIP;
            this->iv_handle.fields.type_target_num = 0;
        }
        else if(K & TARGET_TYPE_PERV)
        {
            this->iv_handle.fields.chiplet_num = l_plat_argument + NEST_GROUP1_CHIPLET_OFFSET;
            this->iv_handle.fields.type = TARGET_TYPE_PERV;
            this->iv_handle.fields.type_target_num = l_plat_argument;
        }
        else if(K & TARGET_TYPE_CORE)
        {
            this->iv_handle.fields.chiplet_num = l_plat_argument + CORE_CHIPLET_OFFSET;
            this->iv_handle.fields.type = TARGET_TYPE_CORE | TARGET_TYPE_PERV;
            this->iv_handle.fields.type_target_num = l_plat_argument;
        }
        else if(K & TARGET_TYPE_EQ)
        {
            this->iv_handle.fields.chiplet_num = l_plat_argument + EQ_CHIPLET_OFFSET;
            this->iv_handle.fields.type = TARGET_TYPE_EQ | TARGET_TYPE_PERV;
            this->iv_handle.fields.type_target_num = l_plat_argument;
        }
        else if(K & TARGET_TYPE_EX)
        {
            this->iv_handle.fields.chiplet_num = (l_plat_argument / 2) + EX_CHIPLET_OFFSET;
            this->iv_handle.fields.type = TARGET_TYPE_EX;
            this->iv_handle.fields.type_target_num = l_plat_argument;
        }
        else if(K & TARGET_TYPE_MCS)
        {

            this->iv_handle.fields.chiplet_num = l_plat_argument + MCS_CHIPLET_OFFSET;
            this->iv_handle.fields.type = TARGET_TYPE_MCS | TARGET_TYPE_PERV;
            this->iv_handle.fields.type_target_num = l_plat_argument;
        }
        else if(K == TARGET_TYPE_ALL)
        {
            this->iv_handle.fields.chiplet_num = l_plat_argument;
            this->iv_handle.fields.type = TARGET_TYPE_ALL;
        }

        this->iv_handle.fields.valid = 1;

    }

    ///
    /// @brief Assignment Operator.
    /// @param[in] i_right Reference to Target to assign from.
    /// @return Reference to 'this' Target
    ///
    template<TargetType K, typename V>
    Target<K, V>& Target<K, V>::operator=(const Target& i_right)
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
    template<TargetType K, typename V>
    bool Target<K, V>::operator==(const Target& i_right) const
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
    template<TargetType K, typename V>
    bool Target<K, V>::operator!=(const Target& i_right) const
    {
        if (this->iv_handle.value != i_right.iv_handle.value)
            return true;
        else
            return false;
    }

    ///
    /// @brief Get this target's immediate parent
    /// @tparam T The type of the parent
    /// @return Target<T> a target representing the parent
    ///
    template<TargetType K, typename V>
    template<TargetType T>
    inline Target<T> Target<K, V>::getParent(void) const
    {
        static_assert(((K == TARGET_TYPE_EQ) ||
                       (K == TARGET_TYPE_CORE) ||
                       (K == TARGET_TYPE_MCS) ||
                       (K == TARGET_TYPE_PERV) ||
                       (K == TARGET_TYPE_EX)) &&
                      ((T == TARGET_TYPE_EQ) ||
                       (T == TARGET_TYPE_EX) ||
                       (T == TARGET_TYPE_PROC_CHIP) ||
                       (T == TARGET_TYPE_PERV)),
                      "Invalid parent/child target type passed");

        static_assert(!((K == TARGET_TYPE_EQ) &&
                        (T != TARGET_TYPE_PERV) &&
                        (T != TARGET_TYPE_PROC_CHIP)),
                        "Invalid parent for EQ target, must be PERV or "
                        "PROC_CHIP");

        static_assert(!((K == TARGET_TYPE_MCS) &&
                        (T != TARGET_TYPE_PERV) &&
                        (T != TARGET_TYPE_PROC_CHIP)),
                        "Invalid parent for MCS target, must be PERV or "
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

        if(TARGET_TYPE_PERV == T) // EQ/EC/MCS/PERV ===> PERV
        {
            return this->iv_handle;
        }
        if(TARGET_TYPE_PROC_CHIP == T) // EQ/EC/EX/MCS/PERV ===> PROC
        {
            return G_vec_targets[CHIP_TARGET_OFFSET];
        }
        if((TARGET_TYPE_EQ == T) && (TARGET_TYPE_CORE == K)) // EC ===> EQ
        {
            return G_vec_targets[(getTargetNumber() / CORES_PER_QUAD) + EQ_TARGET_OFFSET];
        }
        if((TARGET_TYPE_EQ == T) && (TARGET_TYPE_EX == K)) // EX ===> EQ
        {
            return G_vec_targets[(getTargetNumber() / EX_PER_QUAD) + EQ_TARGET_OFFSET];
        }
        if(TARGET_TYPE_EX == T) // EC ===> EX
        {
            return G_vec_targets[(getTargetNumber() / CORES_PER_EX) + EX_TARGET_OFFSET];
        }
    }

    /// @brief Get this target's children - handles EQ/EX/EC conversions
    /// @tparam K The type of parent
    /// @tparam V The plat target handle type
    /// @tparam T The type of child
    /// @param[in] i_state The desired TargetState of the children
    /// @return std::vector<Target<T> > a vector of present/functional
    /// children
    /// @warning The children are returned in order, ex child[0] is
    /// std::vector[0]
    template<TargetType K, typename V>
    template<TargetType T>
    std::vector<Target<T>>
    Target<K, V>::getChildren(const TargetState i_state) const
    {
        static_assert(((K == TARGET_TYPE_EQ) || (K == TARGET_TYPE_EX)) &&
                      ((T == TARGET_TYPE_EX) || (T == TARGET_TYPE_CORE)),
                      "Invalid parent/child passed to getChildren");
        // valid children for EQ
        // EQ -> CORE
        // EQ -> EX
        static_assert(!((K == fapi2::TARGET_TYPE_EQ) &&
                        (T != fapi2::TARGET_TYPE_CORE) &&
                        (T != fapi2::TARGET_TYPE_EX)),
                      "improper child of fapi2::TARGET_TYPE_EQ");

        // valid children for EX
        // EX -> CORE
        static_assert(!((K == fapi2::TARGET_TYPE_EX) &&
                        (T != fapi2::TARGET_TYPE_CORE)),
                      "improper child of fapi2::TARGET_TYPE_EX");

        std::vector<Target<T> > l_children;
        uint32_t l_childPerChiplet = 0;
        uint32_t l_childTargetOffset = 0;

        // EQ ==> EX
        if((K == TARGET_TYPE_EQ) && (T == TARGET_TYPE_EX))
        {
            l_childPerChiplet = EX_PER_QUAD;
            l_childTargetOffset = EX_TARGET_OFFSET;
        }

        // EQ ==> EC
        if((K == TARGET_TYPE_EQ) && (T == TARGET_TYPE_CORE))
        {
            l_childPerChiplet = CORES_PER_QUAD;
            l_childTargetOffset = CORE_TARGET_OFFSET;
        }

        // EX ==> EC
        if((K == TARGET_TYPE_EX) && (T == TARGET_TYPE_CORE))
        {
            l_childPerChiplet = CORES_PER_EX;
            l_childTargetOffset = CORE_TARGET_OFFSET;
        }

        for(uint32_t i = 0; i < l_childPerChiplet; ++i)
        {
            Target<T> *l_temp = reinterpret_cast< Target<T>* >(&G_vec_targets.at((getTargetNumber() * l_childPerChiplet) + l_childTargetOffset + i));
            switch (i_state)
            {
                case TARGET_STATE_PRESENT:
                    if ((*l_temp).getPresent())
                    {
                        l_children.push_back((*l_temp));
                    }
                    break;
                case TARGET_STATE_FUNCTIONAL:
                    if ((*l_temp).getFunctional())
                    {
                        l_children.push_back((*l_temp));
                    }
                    break;
                default:
                    assert(false);
            }
        }

        return l_children;
    }

    ///
    /// @brief Get this target's children - specialization for PROC_CHIP
    /// @tparam T The type of child
    /// @param[in] i_state The desired TargetState of the children
    /// @return std::vector<Target<T> > a vector of present/functional
    /// children
    ///
    template<>
    template< TargetType T>
    std::vector<Target<T> >
    Target<TARGET_TYPE_PROC_CHIP>::getChildren(const TargetState i_state) const
    {
        std::vector<fapi2::plat_target_handle_t>::iterator l_iter;
        std::vector<Target<T> > l_children;

        for (l_iter = G_vec_targets.begin(); l_iter != G_vec_targets.end(); ++l_iter)
        {

            Target<T> * l_temp =  reinterpret_cast< Target<T>* >(l_iter);
            if (((*l_temp).getTargetType() & T) == T)
            {
                switch (i_state)
                {
                    case TARGET_STATE_PRESENT:
                        if ((*l_temp).getPresent())
                        {
                            l_children.push_back((*l_temp));
                        }
                        break;
                    case TARGET_STATE_FUNCTIONAL:
                        if ((*l_temp).getFunctional())
                        {
                            l_children.push_back((*l_temp));
                        }
                        break;
                    default:
                        assert(false);
                }
            }
        }

        return l_children;
    }

    // Specialization of getChildren, filtered for the chip target
    template<TargetType K, typename V>
    template<TargetType T>
    std::vector<Target<T> >
    Target<K, V>::getChildren(const TargetFilter i_filter,
                                          const TargetState i_state) const
    {
        static_assert((K == TARGET_TYPE_PROC_CHIP), "Parent target must be the proc chip");
        static_assert((T == TARGET_TYPE_EQ) || (T == TARGET_TYPE_CORE)
                || (T == TARGET_TYPE_PERV) || (T == TARGET_TYPE_MCS),
                "Child target type must be a pervasive chiplet");

        std::vector<Target<T> > l_children;
        static const uint64_t mask = 1;

        // Walk the bits in the input target filter. For every bit, at
        // position x, that is set, x can be used as an index into our global
        // target vector (indexed by chiplet number)
        for (uint32_t l_idx = 0;
                l_idx < sizeof(TargetFilter) * 8;
                ++l_idx)
        {
            if (i_filter & (mask << (((sizeof(TargetFilter)*8)-1) - l_idx)))
            {
                plat_target_handle_t l_targetHandle = G_vec_targets.at(l_idx + NEST_GROUP1_CHIPLET_OFFSET);

                if(l_targetHandle.fields.type & TARGET_TYPE_PERV) // Can be an assertion?
                {
                    switch (i_state)
                    {
                        case TARGET_STATE_PRESENT:
                            if(l_targetHandle.fields.present)
                            {
                                l_children.push_back(l_targetHandle);
                            }
                            break;
                         case TARGET_STATE_FUNCTIONAL:
                            if(l_targetHandle.fields.functional)
                            {
                                l_children.push_back(l_targetHandle);
                            }
                            break;
                        default:
                            break;
                     }
                 }
             }
         }
         return l_children;
    }

    ///
    /// @brief Get the target at the other end of a bus - dimm included
    /// @tparam T The type of the parent
    /// @param[in] i_state The desired TargetState of the children
    /// @return Target<T> a target representing the thing on the other end
    /// @note Can be easily changed to a vector if needed
    ///
    template<TargetType K, typename V>
    template<TargetType T>
    inline Target<T>
    Target<K, V>::getOtherEnd(const TargetState i_state) const
    {
//        static_assert( false, "getOtherEnd() is not supported on PPE platforms");
    }

    ///
    /// @brief Is the target functional?
    /// @return true if target is functional, false if non-functional
    ///

    template<TargetType K, typename V>
    inline bool
    Target<K, V>::isFunctional(void) const
    {
        return getFunctional();
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
    template< TargetType T >
    inline void toString(const Target<T>& i_target, char* i_buffer, size_t i_bsize)
    {
        snprintf(i_buffer, i_bsize, "Target 0x%lx/0x%x", i_target.get(), T);
    }

    ///
    /// @brief Return the string interpretation of this target
    /// @tparam T The type of the target
    /// @tparam B The type of the buffer
    /// @param[in] A pointer to the Target<T>
    /// @param[in] i_buffer buffer to write in to
    /// @param[in] i_bsize size of the buffer
    /// @return void
    /// @post The contents of the buffer is replaced with the string
    /// representation of the target
    ///
    template< TargetType T >
    inline void toString(const Target<T>* i_target, char* i_buffer, size_t i_bsize)
    {
        snprintf(i_buffer, i_bsize, "Target 0x%lx/0x%x", i_target->get(), T);
    }

    ///
    /// @brief Get an enumerated target of a specific type
    /// @tparam T The type of the target
    /// @param[in] Ordinal representing the ordinal number of
    /// the desired target
    /// @return Target<T> the target requested
    ///
    template<TargetType T>
    inline Target<T> getTarget(uint64_t Ordinal)
    {
        // For testing
        return Target<T>(Ordinal);
    }
}

#endif

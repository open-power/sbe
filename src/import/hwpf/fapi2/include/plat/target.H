/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/hwpf/fapi2/include/plat/target.H $                 */
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
 * @file target.H
 * @brief platform specializations for fapi2 targets
 */

#ifndef __FAPI2_TARGET__
#define __FAPI2_TARGET__

#include <plat_target.H>
#include <fapi2_target.H>
#include <stdio.h>

namespace fapi2
{

template<TargetType K, MulticastType M, typename V>
static constexpr void plat_apply_target_limits(void)
{
    static_assert(!(K & TARGET_TYPE_MULTICAST),
                  "Multicast targets are not supported on this platform");
}

template<TargetType T, MulticastType M, typename V>
inline void Target<T, M, V>::mcUpdateHandle() {};

///
/// @brief Assignment Operator.
/// @param[in] i_right Reference to Target to assign from.
/// @return Reference to 'this' Target
///
template<TargetType K, MulticastType M, typename V>
Target<K, M, V>& Target<K, M, V>::operator=(const Target& i_right)
{
    iv_handle = i_right.iv_handle;
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
    return i_right.iv_handle == iv_handle;
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
    return i_right.iv_handle != iv_handle;
}

///
/// @brief Less Than Comparison Operator
/// @param[in] i_right Reference to Target to compare.
/// @return bool. True if less than i_right.
/// @note Platforms need to define this so that the physical
/// targets are determined to be less than rather than just the handles
///
template<TargetType K, MulticastType M, typename V>
bool Target<K, M, V>::operator<(const Target& i_right) const
{
    return i_right.iv_handle < iv_handle;
}

///
/// @brief Get this target's immediate parent
/// @tparam T The type of the parent
/// @return Target<T> a target representing the parent
///
template<TargetType K, MulticastType M, typename V>
template<TargetType T>
inline Target<T, M, V> Target<K, M, V>::getParent(void) const
{
    // It's forbidden to strip multicast from a target below PROC_CHIP level:
    // Since a multicast might end up expanding to more than one unicast target,
    // users are required to use getChildren if they want to map back to unicast.
    static_assert(!(((K ^ T) & TARGET_TYPE_MULTICAST) && ((T & TARGET_TYPE_CHIPLETS) != TARGET_TYPE_NONE)),
                  "Parent target for a multicast target must also be multicast or PROC_CHIP");

    // For testing
    return Target<T, M, V>(iv_handle);
}

///
/// @brief Get this target's children
/// @tparam T The type of the parent
/// @param[in] i_state The desired TargetState of the children
/// @return std::vector<Target<T> > a vector of present/functional
/// children
/// @warning The children of EX's (cores) are expected to be returned
/// in order. That is, core 0 is std::vector[0].
///
template<TargetType K, MulticastType M, typename V>
template<TargetType T>
inline std::vector<Target<T, M, V> >
Target<K, M, V>::getChildren(const TargetState i_state) const
{
    // valid children for multicast targets are the same with multicast stripped
    static_assert(!((K & TARGET_TYPE_MULTICAST) &&
                    (K != (T | TARGET_TYPE_MULTICAST))),
                  "improper child of fapi2::TARGET_TYPE_MULTICAST");

    // Example implementation of evaluating the members of a multicast group for P10,
    // this should go into a .C file of course.
    //
    // if (K & TARGET_TYPE_MULTICAST)
    // {
    //     if (the plat_target turns out to really be a multicast target)
    //     {
    //         buffer<uint64_t> l_enabledTargets;
    //         /*
    //          * This "magic" SCOM operation will perform a "grab bit 0" multicast read from
    //          * an address in the PCB slave that is guaranteed to be readable (since it does
    //          * not depend on a chiplet being powered/enabled/clocked) and always have bit 0
    //          * set to '1' (per definition).
    //          * The result of the operation will be a 64bit value that will contain said bit 0,
    //          * i.e. a '1' for all members of the multicast group, and a '0' for all others.
    //          */
    //         getScom(Target < TARGET_TYPE_PERV | TARGET_TYPE_MULTICAST, MULTICAST_BITX > (*this),
    //                 0xF0001, l_enabledTargets);
    //
    //         return {targets for all chiplets enabled in l_enabledTargets};
    //     }
    //     else
    //     {
    //         // If this target is a unicast target in disguise, return just this target
    //         // with the multicast qualifier stripped.
    //         return {Target<T, M, V>(*this)};
    //     }
    // }

    // To keep the compiler quiet about unused variables
    static_cast<void>(i_state);
    // For testing
    return {Target<T, M, V>(), Target<T, M, V>()};
}
// Specialization of getChildren, filtered for pervasive targets.
template<>
template<>
inline std::vector<Target<TARGET_TYPE_PERV> >
Target<TARGET_TYPE_PERV>::getChildren(const TargetFilter i_filter,
                                      const TargetState i_state ) const
{
    // To keep the compiler quiet about unused variables
    static_cast<void>(i_state);
    static_cast<void>(i_filter);

    // For testing
    return {Target<TARGET_TYPE_PERV>(i_filter), Target<TARGET_TYPE_PERV>(i_filter)};
}

template<TargetType K, MulticastType M, typename V>
template< TargetType T, MulticastType O >
inline Target < T | TARGET_TYPE_MULTICAST, O, V >
Target<K, M, V>::getMulticast(const MulticastGroup i_group) const
{
    // C++ does not allow partial specialization of class member functions (please prove me wrong!),
    // so we need to use this static assertion instead. On the upside, the error message is much clearer ;)
    static_assert((K == TARGET_TYPE_PROC_CHIP), "Parent target must be the proc chip");
    static_assert((T & TARGET_TYPE_MULTICASTABLE) != 0,
                  "Requested target type must be part of TARGET_TYPE_MULTICASTABLE");
    static_assert(bitCount<T>::count == 1,
                  "Requested target type must be a single type, TARGET_TYPE_MULTICAST will be added automatically");

    return Target < T | TARGET_TYPE_MULTICAST, O, V > ();
}

template<TargetType K, MulticastType M, typename V>
template< MulticastType O >
inline Target < TARGET_TYPE_CORE | TARGET_TYPE_MULTICAST, O, V >
Target<K, M, V>::getMulticast(const MulticastGroup i_group, const MulticastCoreSelect i_cores) const
{
    // C++ does not allow partial specialization of class member functions (please prove me wrong!),
    // so we need to use this static assertion instead. On the upside, the error message is much clearer ;)
    static_assert((K == TARGET_TYPE_PROC_CHIP), "Parent target must be the proc chip");
    return Target < TARGET_TYPE_CORE | TARGET_TYPE_MULTICAST, O, V > ();
}


///
/// @brief Get the target at the other end of a bus
/// @tparam T The type of the target on the other end
/// @param[out] o_target A target representing the thing on the other end
/// @param[in] i_state The desired TargetState of the other end
/// @return FAPI2_RC_SUCCESS if OK, platforms will return a non-success
/// ReturnCode in the event of failure
/// @note o_target is only valid if return is FAPI2_RC_SUCCESS
///

template<TargetType K, MulticastType M, typename V>
template<TargetType T>
inline fapi2::ReturnCodes
Target<K, M, V>::getOtherEnd(Target<T, M, V>& o_target,
                             const TargetState i_state) const
{
    // To keep the compiler quiet about unused variables
    static_cast<void>(i_state);

    o_target = Target<T, M, V>();

    return FAPI2_RC_SUCCESS;
}

///
/// @brief Is the target functional?
/// @return true if target is functional, false if non-functional
///

template<TargetType K, MulticastType M, typename V>
inline bool
Target<K, M, V>::isFunctional(void) const
{
    // Platform check if target is good
    // Could check ATTR_FUNCTIONAL or ATTR_PG_*
    return true;
}

///
/// @brief Returns the chiplet number associated with the Target
/// @return The chiplet number for the Target. 0 is returned if the
/// Target does not have a chiplet number (for ex, the PROC_CHIP Target)
/// @note For logical targets such as the EX, the chiplet number of
/// their immediate parent chiplet is returned
///
template<TargetType K, MulticastType M, typename V>
inline uint8_t
Target<K, M, V>::getChipletNumber(void) const
{
    static_assert((K & TARGET_TYPE_MULTICAST) == 0, "getChipletNumber is not supported for multicast targets");
    // Platform can return the chiplet number stored in it's Target handle
    return 0;
}

#ifdef FAPI2_ENABLE_PLATFORM_GET_CORE_SELECT

template<TargetType K, MulticastType M, typename V>
inline MulticastCoreSelect
Target<K, M, V>::_getCoreSelect(void) const
{
    // Platform can return the core select value stored in it's Target handle
    return MCCORE_0;
}

#endif // FAPI2_ENABLE_PLATFORM_GET_CORE_SELECT

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
template<TargetType T, MulticastType M, typename V >
inline void toString(const Target<T, M, V>& i_target,
                     char* i_buffer, size_t i_bsize)
{
    snprintf(i_buffer, i_bsize, "Target 0x%lX/0x%.16lX/0x%X", i_target.get(), T, M);
}

///
/// @brief Return the string interpretation of this target
/// @tparam T The type of the target
/// @tparam B The type of the buffer
/// @param[in] i_target A pointer to the Target<T>
/// @param[in] i_buffer buffer to write in to
/// @param[in] i_bsize  size of the buffer
/// @return void
/// @post The contents of the buffer is replaced with the string
/// representation of the target
///
template< TargetType T, MulticastType M, typename V >
inline void toString(const Target<T, M, V>* i_target,
                     char* i_buffer, size_t i_bsize)
{
    toString(*i_target, i_buffer, i_bsize);
}

///
/// @brief Get an enumerated target of a specific type
/// @tparam T The type of the target
/// @param[in] Ordinal representing the ordinal number of
/// the desired target
/// @return Target<T> the target requested
///
template<TargetType T, MulticastType M, typename V>
inline Target<T, M, V> getTarget(uint64_t Ordinal)
{
    // For testing
    return Target<T, M, V>(Ordinal);
}

}

#endif

/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/hwpf/fapi2/include/fapi2_multicast.H $             */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016                             */
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
///
/// @file fapi2_multicast.H
/// @brief Common definitions for fapi2 multicast target value wrapper class
///

#ifndef __FAPI2_COMMON_MULTICAST__
#define __FAPI2_COMMON_MULTICAST__

#include <target.H>
#include <fapi2_multicast_defs.H>

namespace fapi2
{

///
/// @brief Class representing a FAPI2 Multicast target value wrapper
/// @tparam M the type of multicast operation
/// @tparam G the type of multicast group
/// @tparam V the type of the target's Value this class is wrapping
/// @remark This wrapper class defines how a handle will behave by
///     type and group if internal to the handle mulitcast operation
///     is enabled. If the handle used is not multicast enabled most
///     function of this class will be ignored.
///
template<MulticastType M, MulticastGroup G, typename V = plat_target_handle_t >
class Multicast
{
    public:
        ///
        /// @brief Delagate default constructor to constructor
        ///        that takes in a value as a param
        ///
        Multicast() : Multicast(V()) {}

        ///
        /// @brief Create a Multicast value with a target value
        /// @param[in] Value the value (i.e., specific element this
        /// target represents, or pointer)
        /// @note Platforms can will update the handle value with
        /// information on the multicast type and group
        ///
        Multicast(const V& value) : iv_handle(value)
        {
            updateHandle<M, G>(iv_handle);
        }

        ///
        /// @brief Create a Multicast value from another Multicast value
        /// @param[in] other the value
        /// @note Platforms can will update the handle value with
        /// information on the multicast type and group
        ///
        template<MulticastType O, MulticastGroup N>
        Multicast(const Multicast<O, N> other) :
            iv_handle(static_cast<V>(other))
        {
            updateHandle<M, G>(iv_handle);
        }

        ///
        /// @brief Get the handle as a V
        /// @return V The Multicast wrapper's internal handle, or value
        ///
        inline operator V() const
        {
            return iv_handle;
        }

        ///
        /// @brief Get the handle as a V
        /// @return V The Multicast wrapper's internal handle, or value
        ///
        inline V& operator()() const
        {
            return iv_handle;
        }

        ///
        /// @brief Has the handle been enabled for multicast operation
        /// @return Return true if multicast, false otherwise
        ///
        inline bool isMulticast() const;

    private:
        ///
        /// @brief update the handle with group and type given
        /// @tparam O the type of multicast operation
        /// @tparam N the type of multicast group
        /// @param[in] Value the value/handle
        ///
        template<MulticastType O, MulticastGroup N>
        inline void updateHandle(V& value);

        V iv_handle;
};

// multicast from unicast
template<MulticastType M, MulticastGroup G, TargetType K, typename V>
inline Target<K, Multicast<M, G, V>> make_multicast(const Target<K, V>& t)
{
    return Target<K, Multicast<M, G, V>>(t.get());
}

// multicast from multicast -- changing type
template<MulticastType M, MulticastType N, MulticastGroup G, TargetType K, typename V>
inline Target<K, Multicast<M, G, V>> make_multicast(const Target<K, Multicast<N, G, V>>& t)
{
    return Target<K, Multicast<M, G, V>>(t.get());
}

// multicast from multicast -- changing type and group
template<MulticastType M, MulticastGroup O, MulticastType N, MulticastGroup G, TargetType K, typename V>
inline Target<K, Multicast<M, O, V>> make_multicast(const Target<K, Multicast<N, G, V>>& t)
{
    return Target<K, Multicast<M, O, V>>(t.get());
}

// unicast from multicast
template<MulticastType M, MulticastGroup G, TargetType K, typename V>
inline Target<K, V> make_unicast(const Target<K, Multicast<M, G, V>>& t)
{
    return Target<K, V>(t.get());
}

// test if a multicast target
template<MulticastType M, MulticastGroup G, TargetType K, typename V>
inline bool is_multicast(const Target<K, Multicast<M, G, V>>& t)
{
    const Multicast<M, G, V>& l_mref = t;
    return l_mref.isMulticast();
}

// return false if testing a non-multicast target
template<TargetType K, typename V>
inline bool is_multicast(const Target<K, V>& t)
{
    return false;
}

template<MulticastType M1, MulticastType M2>
constexpr bool is_same()
{
    return (M1 == M2);
}
}

#endif

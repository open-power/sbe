/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/ocmb/explorer/procedures/hwp/memory/lib/i2c/exp_i2c_fields.H $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2018,2019                        */
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
/// @file exp_i2c_fields.H
/// @brief explorer I2C field definition and operations
///
// *HWP HWP Owner: Andre A. Marin <aamarin@us.ibm.com>
// *HWP HWP Backup: Louis Stermole <stermole@us.ibm.com>
// *HWP Team: Memory
// *HWP Level: 2
// *HWP Consumed by: HB:FSP

#ifndef _MSS_EXP_I2C_FIELDS_H_
#define _MSS_EXP_I2C_FIELDS_H_

#include <generic/memory/lib/utils/mss_field.H>
#include <lib/shared/exp_consts.H>
#include <functional>

namespace mss
{
namespace exp
{
namespace i2c
{

///
/// @class fields
/// @brief Explorer I2C command fields
///
struct fields
{
    // First value is byte index, then buffer extract start bit, and extract data length

    // Part of EXP_FW_BOOT_CONFIG
    static constexpr mss::field_t BOOT_MODE{0, 0, 1};
    static constexpr mss::field_t LANE_MODE{0, 1, 3};
    static constexpr mss::field_t SERDES_FREQ{0, 4, 4};

    static constexpr mss::field_t FW_MODE{1, 2, 1};
    static constexpr mss::field_t LOOPBACK_TEST{1, 3, 1};
    static constexpr mss::field_t TRANSPORT_LAYER{1, 4, 2};
    static constexpr mss::field_t DL_LAYER_BOOT_MODE{1, 6, 2};

    // Part of EXP_FW_STATUS
    static constexpr mss::field_t CMD_ID{0, 0, 8};
    static constexpr mss::field_t STATUS_CODE{1, 0, 8};
    static constexpr mss::field_t BOOT_STAGE{2, 6, 2};
};

///
/// @class fieldTraits
/// @brief Traits assoiated with the Explorer I2C commands
/// @tparam F the Explorer I2C field
///
template < const field_t& F >
struct fieldTraits;

///
/// @class fieldTraits - SERDES_FREQ specialization
/// @brief Traits assoiated with the Explorer I2C commands
///
template <>
struct fieldTraits<fields::SERDES_FREQ>
{
    static constexpr size_t COMPARISON_VAL = 0x0F;
    static constexpr const char* FIELD_STR = "SerDes Frequency";

    template <typename T>
    using COMPARISON_OP = std::less_equal<T>;
};

///
/// @class fieldTraits - LANE_MODE specialization
/// @brief Traits assoiated with the Explorer I2C commands
///
template <>
struct fieldTraits<fields::LANE_MODE>
{
    static constexpr size_t COMPARISON_VAL = 0x02;
    static constexpr const char* FIELD_STR = "Lane mode";

    template <typename T>
    using COMPARISON_OP = std::less_equal<T>;
};

///
/// @class fieldTraits - BOOT_MODE specialization
/// @brief Traits assoiated with the Explorer I2C commands
///
template <>
struct fieldTraits<fields::BOOT_MODE>
{
    static constexpr size_t COMPARISON_VAL = 0x01;
    static constexpr const char* FIELD_STR = "Boot mode";

    template <typename T>
    using COMPARISON_OP = std::less_equal<T>;
};

///
/// @class fieldTraits - DL_LAYER_BOOT_MODE specialization
/// @brief Traits assoiated with the Explorer I2C commands
///
template <>
struct fieldTraits<fields::DL_LAYER_BOOT_MODE>
{
    static constexpr size_t COMPARISON_VAL = 0x01;
    static constexpr const char* FIELD_STR = "DL Layer Boot mode";

    template <typename T>
    using COMPARISON_OP = std::less_equal<T>;
};

///
/// @class fieldTraits - TRANSPORT_LAYER specialization
/// @brief Traits assoiated with the Explorer I2C commands
///
template <>
struct fieldTraits<fields::TRANSPORT_LAYER>
{
    static constexpr size_t COMPARISON_VAL = 0x02;
    static constexpr const char* FIELD_STR = "Transport Layer";

    template <typename T>
    using COMPARISON_OP = std::less_equal<T>;
};

///
/// @class fieldTraits - LOOPBACK_TEST specialization
/// @brief Traits assoiated with the Explorer I2C commands
///
template <>
struct fieldTraits<fields::LOOPBACK_TEST>
{
    static constexpr size_t COMPARISON_VAL = 0x01;
    static constexpr const char* FIELD_STR = "OpenCapi looptest test";

    template <typename T>
    using COMPARISON_OP = std::less_equal<T>;
};

///
/// @class fieldTraits - FW_MODE specialization
/// @brief Traits assoiated with the Explorer I2C commands
///
template <>
struct fieldTraits<fields::FW_MODE>
{
    static constexpr size_t COMPARISON_VAL = 0x01;
    static constexpr const char* FIELD_STR = "FW Mode";

    template <typename T>
    using COMPARISON_OP = std::less_equal<T>;
};

///
/// @class fieldTraits - CMD_ID specialization
/// @brief Traits assoiated with the Explorer I2C commands
///
template<>
struct fieldTraits<fields::CMD_ID>
{
    static constexpr size_t COMPARISON_VAL = 0xFF;
    static constexpr const char* FIELD_STR = "Command ID";

    template <typename T>
    using COMPARISON_OP = std::less_equal<T>;
};

///
/// @class fieldTraits - STATUS_CODE specialization
/// @brief Traits assoiated with the Explorer I2C commands
///
template<>
struct fieldTraits<fields::STATUS_CODE>
{
    static constexpr size_t COMPARISON_VAL = 0xFF;
    static constexpr const char* FIELD_STR = "Status Code";

    template <typename T>
    using COMPARISON_OP = std::less_equal<T>;
};

///
/// @class fieldTraits - BOOT_STAGE specialization
/// @brief Traits assoiated with the Explorer I2C commands
///
template<>
struct fieldTraits<fields::BOOT_STAGE>
{
    static constexpr size_t COMPARISON_VAL = 0x03;
    static constexpr const char* FIELD_STR = "Boot Stage";

    template <typename T>
    using COMPARISON_OP = std::less_equal<T>;
};

///
/// @brief Explorer I2C field getter
/// @tparam F the explorer I2C field
/// @tparam IT Input type
/// @tparam OT Output type
/// @tparam TT Traits associated with exp I2C - defaults to fieldTraits<F>
/// @param[in] i_target the OCMB target
/// @param[in] i_data the buffer as a reference to a vector
/// @param[out] o_value the output value received
/// @return FAPI2_RC_SUCCESS iff okay
///
template< const field_t& F,
          typename IT,
          typename OT,
          typename TT = fieldTraits<F> >
inline fapi2::ReturnCode get_field(const fapi2::Target<fapi2::TARGET_TYPE_OCMB_CHIP>& i_target,
                                   const std::vector<IT>& i_data,
                                   OT& o_value)
{
    return mss::get_field<F, TT>(i_target, i_data, EXP_I2C_GET_FIELD, o_value);
}

///
/// @brief Explorer I2C field setter
/// @tparam IT Input type
/// @tparam OT Output type
/// @tparam TT Traits associated with exp I2C - defaults to fieldTraits<F>
/// @param[in] i_target the OCMB target
/// @param[in] i_value the input value to set
/// @param[in,out] io_data the buffer as a reference to a vector
/// @return FAPI2_RC_SUCCESS iff okay
///
template< const field_t& F,
          typename IT,
          typename OT,
          typename TT = fieldTraits<F> >
inline fapi2::ReturnCode set_field(const fapi2::Target<fapi2::TARGET_TYPE_OCMB_CHIP>& i_target,
                                   std::vector<OT>& io_data,
                                   const IT i_value)

{
    return mss::set_field<F, TT>(i_target, i_value, EXP_I2C_SET_FIELD, io_data);
}

namespace boot_cfg
{

///
/// @brief SERDES_FREQ getter
/// @param[in] i_target the OCMB target
/// @param[in] i_data the buffer as a reference to a vector
/// @param[out] o_setting
/// @return FAPI2_RC_SUCCESS iff okay
///
inline fapi2::ReturnCode get_serdes_freq(const fapi2::Target<fapi2::TARGET_TYPE_OCMB_CHIP>& i_target,
        const std::vector<uint8_t>& i_data,
        size_t& o_setting)
{
    return get_field<fields::SERDES_FREQ>(i_target, i_data, o_setting);
}

///
/// @brief SERDES_FREQ setter
/// @param[in] i_target the OCMB target
/// @param[in,out] io_data the buffer as a reference to a vector
/// @param[in] i_setting the value to set
/// @return FAPI2_RC_SUCCESS iff okay
///
inline fapi2::ReturnCode set_serdes_freq(const fapi2::Target<fapi2::TARGET_TYPE_OCMB_CHIP>& i_target,
        std::vector<uint8_t>& io_data,
        const uint8_t i_setting)
{
    return set_field<fields::SERDES_FREQ>(i_target, io_data, i_setting);
}

///
/// @brief LANE_MODE getter
/// @param[in] i_target the OCMB target
/// @param[in] i_data the buffer as a reference to a vector
/// @param[out] o_setting
/// @return FAPI2_RC_SUCCESS iff okay
///
inline fapi2::ReturnCode get_lane_mode(const fapi2::Target<fapi2::TARGET_TYPE_OCMB_CHIP>& i_target,
                                       const std::vector<uint8_t>& i_data,
                                       size_t& o_setting)
{
    return get_field<fields::LANE_MODE>(i_target, i_data, o_setting);
}

///
/// @brief LANE_MODE setter
/// @param[in] i_target the OCMB target
/// @param[in,out] io_data the buffer as a reference to a vector
/// @param[in] i_setting the value to set
/// @return FAPI2_RC_SUCCESS iff okay
///
inline fapi2::ReturnCode set_lane_mode(const fapi2::Target<fapi2::TARGET_TYPE_OCMB_CHIP>& i_target,
                                       std::vector<uint8_t>& io_data,
                                       const uint8_t i_setting)
{
    return set_field<fields::LANE_MODE>(i_target, io_data, i_setting);
}

///
/// @brief BOOT_MODE getter
/// @param[in] i_target the OCMB target
/// @param[in] i_data the buffer as a reference to a vector
/// @param[out] o_setting
/// @return FAPI2_RC_SUCCESS iff okay
///
inline fapi2::ReturnCode get_boot_mode(const fapi2::Target<fapi2::TARGET_TYPE_OCMB_CHIP>& i_target,
                                       const std::vector<uint8_t>& i_data,
                                       size_t& o_setting)
{
    return get_field<fields::BOOT_MODE>(i_target, i_data, o_setting);
}

///
/// @brief BOOT_MODE setter
/// @param[in] i_target the OCMB target
/// @param[in,out] io_data the buffer as a reference to a vector
/// @param[in] i_setting the value to set
/// @return FAPI2_RC_SUCCESS iff okay
///
inline fapi2::ReturnCode set_boot_mode(const fapi2::Target<fapi2::TARGET_TYPE_OCMB_CHIP>& i_target,
                                       std::vector<uint8_t>& io_data,
                                       const uint8_t i_setting)
{
    return set_field<fields::BOOT_MODE>(i_target, io_data, i_setting);
}

/// @brief DL_LAYER_BOOT_MODE getter
/// @param[in] i_target the OCMB target
/// @param[in] i_data the buffer as a reference to a vector
/// @param[out] o_setting
/// @return FAPI2_RC_SUCCESS iff okay
///
inline fapi2::ReturnCode get_dl_layer_boot_mode(const fapi2::Target<fapi2::TARGET_TYPE_OCMB_CHIP>& i_target,
        const std::vector<uint8_t>& i_data,
        size_t& o_setting)
{
    return get_field<fields::DL_LAYER_BOOT_MODE>(i_target, i_data, o_setting);
}

///
/// @brief DL_LAYER_BOOT_MODE setter
/// @param[in] i_target the OCMB target
/// @param[in,out] io_data the buffer as a reference to a vector
/// @param[in] i_setting the value to set
/// @return FAPI2_RC_SUCCESS iff okay
///
inline fapi2::ReturnCode set_dl_layer_boot_mode(const fapi2::Target<fapi2::TARGET_TYPE_OCMB_CHIP>& i_target,
        std::vector<uint8_t>& io_data,
        const uint8_t i_setting)
{
    return set_field<fields::DL_LAYER_BOOT_MODE>(i_target, io_data, i_setting);
}

///
/// @brief DL_LAYER_BOOT_MODE getter
/// @param[in] i_target the OCMB target
/// @param[in] i_data the buffer as a reference to a vector
/// @param[out] o_setting
/// @return FAPI2_RC_SUCCESS iff okay
///
inline fapi2::ReturnCode get_transport_layer(const fapi2::Target<fapi2::TARGET_TYPE_OCMB_CHIP>& i_target,
        const std::vector<uint8_t>& i_data,
        size_t& o_setting)
{
    return get_field<fields::TRANSPORT_LAYER>(i_target, i_data, o_setting);
}


///
/// @brief TRANSPORT_LAYER setter
/// @param[in] i_target the OCMB target
/// @param[in,out] io_data the buffer as a reference to a vector
/// @param[in] i_setting the value to set
/// @return FAPI2_RC_SUCCESS iff okay
///
inline fapi2::ReturnCode set_transport_layer(const fapi2::Target<fapi2::TARGET_TYPE_OCMB_CHIP>& i_target,
        std::vector<uint8_t>& io_data,
        const uint8_t i_setting)
{
    return set_field<fields::TRANSPORT_LAYER>(i_target, io_data, i_setting);
}

///
/// @brief LOOPBACK_TEST getter
/// @param[in] i_target the OCMB target
/// @param[in] i_data the buffer as a reference to a vector
/// @param[out] o_setting
/// @return FAPI2_RC_SUCCESS iff okay
///
inline fapi2::ReturnCode get_loopback_test(const fapi2::Target<fapi2::TARGET_TYPE_OCMB_CHIP>& i_target,
        const std::vector<uint8_t>& i_data,
        size_t& o_setting)
{
    return get_field<fields::LOOPBACK_TEST>(i_target, i_data, o_setting);
}

///
/// @brief LOOPBACK_TEST setter
/// @param[in] i_target the OCMB target
/// @param[in,out] io_data the buffer as a reference to a vector
/// @param[in] i_setting the value to set
/// @return FAPI2_RC_SUCCESS iff okay
///
inline fapi2::ReturnCode set_loopback_test(const fapi2::Target<fapi2::TARGET_TYPE_OCMB_CHIP>& i_target,
        std::vector<uint8_t>& io_data,
        const uint8_t i_setting)
{
    return set_field<fields::LOOPBACK_TEST>(i_target, io_data, i_setting);
}

///
/// @brief FW_MODE getter
/// @param[in] i_target the OCMB target
/// @param[in] i_data the buffer as a reference to a vector
/// @param[out] o_setting
/// @return FAPI2_RC_SUCCESS iff okay
///
inline fapi2::ReturnCode get_fw_mode(const fapi2::Target<fapi2::TARGET_TYPE_OCMB_CHIP>& i_target,
                                     const std::vector<uint8_t>& i_data,
                                     size_t& o_setting)
{
    return get_field<fields::FW_MODE>(i_target, i_data, o_setting);
}

///
/// @brief FW_MODE setter
/// @param[in] i_target the OCMB target
/// @param[in] i_setting the value to set
/// @param[in,out] io_data the buffer as a reference to a vector
/// @return FAPI2_RC_SUCCESS iff okay
///
inline fapi2::ReturnCode set_fw_mode(const fapi2::Target<fapi2::TARGET_TYPE_OCMB_CHIP>& i_target,
                                     std::vector<uint8_t>& io_data,
                                     const uint8_t i_setting)
{
    return set_field<fields::FW_MODE>(i_target, io_data, i_setting);
}

}// boot_cfg

namespace status
{

///
/// @brief BOOT_STAGE getter
/// @param[in] i_target the OCMB target
/// @param[in] i_data the buffer as a reference to a vector
/// @param[out] o_setting
/// @return FAPI2_RC_SUCCESS iff okay
///
inline fapi2::ReturnCode get_boot_stage(const fapi2::Target<fapi2::TARGET_TYPE_OCMB_CHIP>& i_target,
                                        const std::vector<uint8_t>& i_data,
                                        size_t& o_setting)
{
    return get_field<fields::BOOT_STAGE>(i_target, i_data, o_setting);
}

///
/// @brief STATUS_CODE getter
/// @param[in] i_target the OCMB target
/// @param[in] i_data the buffer as a reference to a vector
/// @param[out] o_setting
/// @return FAPI2_RC_SUCCESS iff okay
///
inline fapi2::ReturnCode get_status_code(const fapi2::Target<fapi2::TARGET_TYPE_OCMB_CHIP>& i_target,
        const std::vector<uint8_t>& i_data,
        size_t& o_setting)
{
    return get_field<fields::STATUS_CODE>(i_target, i_data, o_setting);
}

///
/// @brief CMD_ID getter
/// @param[in] i_target the OCMB target
/// @param[in] i_data the buffer as a reference to a vector
/// @param[out] o_setting
/// @return FAPI2_RC_SUCCESS iff okay
///
inline fapi2::ReturnCode get_cmd_id(const fapi2::Target<fapi2::TARGET_TYPE_OCMB_CHIP>& i_target,
                                    const std::vector<uint8_t>& i_data,
                                    size_t& o_setting)
{
    return get_field<fields::CMD_ID>(i_target, i_data, o_setting);
}

}// status
}// i2c
}// exp
}// mss

#endif
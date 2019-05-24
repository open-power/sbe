/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/common/include/ppe_c/reg00003.H $        */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2019                             */
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
#include "fapi2.H"

#ifndef __PPE_C_REG00003_H_
#define __PPE_C_REG00003_H_

namespace scomt
{
namespace ppe_c
{


template< fapi2::TargetType K, fapi2::MulticastType M, typename V >
static inline fapi2::ReturnCode PREP_CPMS_CL2_PFETCNTL(const fapi2::Target<K, M, V>& i_target)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::init(i_target));
    scomt::last_scom = CPMS_CL2_PFETCNTL;
#endif
    return fapi2::current_err;
#ifdef SCOM_CHECKING
fapi_try_exit:
    return fapi2::current_err;
#endif
}

template< fapi2::TargetType K, fapi2::MulticastType M, typename V >
static inline fapi2::ReturnCode GET_CPMS_CL2_PFETCNTL(const fapi2::Target<K, M, V>& i_target,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::init(i_target));
    scomt::last_scom = CPMS_CL2_PFETCNTL;
#endif
    return fapi2::getScom(i_target, CPMS_CL2_PFETCNTL, o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    return fapi2::current_err;
#endif
}

template< fapi2::TargetType K, fapi2::MulticastType M, typename V >
static inline fapi2::ReturnCode PUT_CPMS_CL2_PFETCNTL(const fapi2::Target<K, M, V>& i_target, const uint64_t i_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::init(i_target));
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return fapi2::putScom(i_target, CPMS_CL2_PFETCNTL, i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    return fapi2::current_err;
#endif
}

template< fapi2::TargetType K, fapi2::MulticastType M, typename V >
static inline fapi2::ReturnCode PREP_CPMS_CL2_PFETCNTL_WO_CLEAR(const fapi2::Target<K, M, V>& i_target)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::init(i_target));
    scomt::last_scom = CPMS_CL2_PFETCNTL_WO_CLEAR;
#endif
    return fapi2::current_err;
#ifdef SCOM_CHECKING
fapi_try_exit:
    return fapi2::current_err;
#endif
}

template< fapi2::TargetType K, fapi2::MulticastType M, typename V >
static inline fapi2::ReturnCode GET_CPMS_CL2_PFETCNTL_WO_CLEAR(const fapi2::Target<K, M, V>& i_target,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::init(i_target));
    scomt::last_scom = CPMS_CL2_PFETCNTL_WO_CLEAR;
#endif
    return fapi2::getScom(i_target, CPMS_CL2_PFETCNTL_WO_CLEAR, o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    return fapi2::current_err;
#endif
}

template< fapi2::TargetType K, fapi2::MulticastType M, typename V >
static inline fapi2::ReturnCode PUT_CPMS_CL2_PFETCNTL_WO_CLEAR(const fapi2::Target<K, M, V>& i_target,
        const uint64_t i_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::init(i_target));
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return fapi2::putScom(i_target, CPMS_CL2_PFETCNTL_WO_CLEAR, i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    return fapi2::current_err;
#endif
}

template< fapi2::TargetType K, fapi2::MulticastType M, typename V >
static inline fapi2::ReturnCode PREP_CPMS_CL2_PFETCNTL_WO_OR(const fapi2::Target<K, M, V>& i_target)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::init(i_target));
    scomt::last_scom = CPMS_CL2_PFETCNTL_WO_OR;
#endif
    return fapi2::current_err;
#ifdef SCOM_CHECKING
fapi_try_exit:
    return fapi2::current_err;
#endif
}

template< fapi2::TargetType K, fapi2::MulticastType M, typename V >
static inline fapi2::ReturnCode GET_CPMS_CL2_PFETCNTL_WO_OR(const fapi2::Target<K, M, V>& i_target,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::init(i_target));
    scomt::last_scom = CPMS_CL2_PFETCNTL_WO_OR;
#endif
    return fapi2::getScom(i_target, CPMS_CL2_PFETCNTL_WO_OR, o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    return fapi2::current_err;
#endif
}

template< fapi2::TargetType K, fapi2::MulticastType M, typename V >
static inline fapi2::ReturnCode PUT_CPMS_CL2_PFETCNTL_WO_OR(const fapi2::Target<K, M, V>& i_target,
        const uint64_t i_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::init(i_target));
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return fapi2::putScom(i_target, CPMS_CL2_PFETCNTL_WO_OR, i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    return fapi2::current_err;
#endif
}


static inline fapi2::buffer<uint64_t>& SET_CPMS_CL2_PFETCNTL_VDD_PFET_SEQ_STATE(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return o_data.insertFromRight<CPMS_CL2_PFETCNTL_VDD_PFET_SEQ_STATE,
           CPMS_CL2_PFETCNTL_VDD_PFET_SEQ_STATE_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_VDD_PFET_SEQ_STATE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}

static inline fapi2::buffer<uint64_t>& SET_CPMS_CL2_PFETCNTL_VDD_PFET_SEQ_STATE(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return o_data.insertFromRight<CPMS_CL2_PFETCNTL_VDD_PFET_SEQ_STATE,
           CPMS_CL2_PFETCNTL_VDD_PFET_SEQ_STATE_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_VDD_PFET_SEQ_STATE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}

static inline fapi2::buffer<uint64_t>& GET_CPMS_CL2_PFETCNTL_VDD_PFET_SEQ_STATE(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return i_data.extractToRight<CPMS_CL2_PFETCNTL_VDD_PFET_SEQ_STATE,
           CPMS_CL2_PFETCNTL_VDD_PFET_SEQ_STATE_LEN>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_VDD_PFET_SEQ_STATE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}

static inline fapi2::buffer<uint64_t>& SET_CPMS_CL2_PFETCNTL_VCS_PFET_SEQ_STATE(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return o_data.insertFromRight<CPMS_CL2_PFETCNTL_VCS_PFET_SEQ_STATE,
           CPMS_CL2_PFETCNTL_VCS_PFET_SEQ_STATE_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_VCS_PFET_SEQ_STATE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}

static inline fapi2::buffer<uint64_t>& SET_CPMS_CL2_PFETCNTL_VCS_PFET_SEQ_STATE(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return o_data.insertFromRight<CPMS_CL2_PFETCNTL_VCS_PFET_SEQ_STATE,
           CPMS_CL2_PFETCNTL_VCS_PFET_SEQ_STATE_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_VCS_PFET_SEQ_STATE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}

static inline fapi2::buffer<uint64_t>& GET_CPMS_CL2_PFETCNTL_VCS_PFET_SEQ_STATE(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return i_data.extractToRight<CPMS_CL2_PFETCNTL_VCS_PFET_SEQ_STATE,
           CPMS_CL2_PFETCNTL_VCS_PFET_SEQ_STATE_LEN>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_VCS_PFET_SEQ_STATE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}

static inline fapi2::buffer<uint64_t>& SET_CPMS_CL2_PFETCNTL_VDD_PFET_VAL_OVERRIDE(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return o_data.insertFromRight<CPMS_CL2_PFETCNTL_VDD_PFET_VAL_OVERRIDE,
           1>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_VDD_PFET_VAL_OVERRIDE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}

static inline fapi2::buffer<uint64_t>& SET_CPMS_CL2_PFETCNTL_VDD_PFET_VAL_OVERRIDE(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return o_data.insertFromRight<CPMS_CL2_PFETCNTL_VDD_PFET_VAL_OVERRIDE,
           1>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_VDD_PFET_VAL_OVERRIDE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}

static inline fapi2::buffer<uint64_t>& SET_CPMS_CL2_PFETCNTL_VDD_PFET_VAL_OVERRIDE(fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return o_data.setBit<CPMS_CL2_PFETCNTL_VDD_PFET_VAL_OVERRIDE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_VDD_PFET_VAL_OVERRIDE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}

static inline fapi2::buffer<uint64_t>& CLEAR_CPMS_CL2_PFETCNTL_VDD_PFET_VAL_OVERRIDE(fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return o_data.clearBit<CPMS_CL2_PFETCNTL_VDD_PFET_VAL_OVERRIDE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_VDD_PFET_VAL_OVERRIDE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}

static inline fapi2::buffer<uint64_t>& GET_CPMS_CL2_PFETCNTL_VDD_PFET_VAL_OVERRIDE(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return i_data.extractToRight<CPMS_CL2_PFETCNTL_VDD_PFET_VAL_OVERRIDE,
           1>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_VDD_PFET_VAL_OVERRIDE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}

static inline bool GET_CPMS_CL2_PFETCNTL_VDD_PFET_VAL_OVERRIDE(fapi2::buffer<uint64_t>& i_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return i_data.getBit<CPMS_CL2_PFETCNTL_VDD_PFET_VAL_OVERRIDE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_VDD_PFET_VAL_OVERRIDE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}

static inline fapi2::buffer<uint64_t>& SET_CPMS_CL2_PFETCNTL_VDD_PFET_SEL_OVERRIDE(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return o_data.insertFromRight<CPMS_CL2_PFETCNTL_VDD_PFET_SEL_OVERRIDE,
           1>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_VDD_PFET_SEL_OVERRIDE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}

static inline fapi2::buffer<uint64_t>& SET_CPMS_CL2_PFETCNTL_VDD_PFET_SEL_OVERRIDE(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return o_data.insertFromRight<CPMS_CL2_PFETCNTL_VDD_PFET_SEL_OVERRIDE,
           1>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_VDD_PFET_SEL_OVERRIDE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}

static inline fapi2::buffer<uint64_t>& SET_CPMS_CL2_PFETCNTL_VDD_PFET_SEL_OVERRIDE(fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return o_data.setBit<CPMS_CL2_PFETCNTL_VDD_PFET_SEL_OVERRIDE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_VDD_PFET_SEL_OVERRIDE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}

static inline fapi2::buffer<uint64_t>& CLEAR_CPMS_CL2_PFETCNTL_VDD_PFET_SEL_OVERRIDE(fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return o_data.clearBit<CPMS_CL2_PFETCNTL_VDD_PFET_SEL_OVERRIDE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_VDD_PFET_SEL_OVERRIDE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}

static inline fapi2::buffer<uint64_t>& GET_CPMS_CL2_PFETCNTL_VDD_PFET_SEL_OVERRIDE(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return i_data.extractToRight<CPMS_CL2_PFETCNTL_VDD_PFET_SEL_OVERRIDE,
           1>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_VDD_PFET_SEL_OVERRIDE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}

static inline bool GET_CPMS_CL2_PFETCNTL_VDD_PFET_SEL_OVERRIDE(fapi2::buffer<uint64_t>& i_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return i_data.getBit<CPMS_CL2_PFETCNTL_VDD_PFET_SEL_OVERRIDE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_VDD_PFET_SEL_OVERRIDE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}

static inline fapi2::buffer<uint64_t>& SET_CPMS_CL2_PFETCNTL_VCS_PFET_VAL_OVERRIDE(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return o_data.insertFromRight<CPMS_CL2_PFETCNTL_VCS_PFET_VAL_OVERRIDE,
           1>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_VCS_PFET_VAL_OVERRIDE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}

static inline fapi2::buffer<uint64_t>& SET_CPMS_CL2_PFETCNTL_VCS_PFET_VAL_OVERRIDE(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return o_data.insertFromRight<CPMS_CL2_PFETCNTL_VCS_PFET_VAL_OVERRIDE,
           1>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_VCS_PFET_VAL_OVERRIDE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}

static inline fapi2::buffer<uint64_t>& SET_CPMS_CL2_PFETCNTL_VCS_PFET_VAL_OVERRIDE(fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return o_data.setBit<CPMS_CL2_PFETCNTL_VCS_PFET_VAL_OVERRIDE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_VCS_PFET_VAL_OVERRIDE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}

static inline fapi2::buffer<uint64_t>& CLEAR_CPMS_CL2_PFETCNTL_VCS_PFET_VAL_OVERRIDE(fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return o_data.clearBit<CPMS_CL2_PFETCNTL_VCS_PFET_VAL_OVERRIDE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_VCS_PFET_VAL_OVERRIDE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}

static inline fapi2::buffer<uint64_t>& GET_CPMS_CL2_PFETCNTL_VCS_PFET_VAL_OVERRIDE(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return i_data.extractToRight<CPMS_CL2_PFETCNTL_VCS_PFET_VAL_OVERRIDE,
           1>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_VCS_PFET_VAL_OVERRIDE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}

static inline bool GET_CPMS_CL2_PFETCNTL_VCS_PFET_VAL_OVERRIDE(fapi2::buffer<uint64_t>& i_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return i_data.getBit<CPMS_CL2_PFETCNTL_VCS_PFET_VAL_OVERRIDE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_VCS_PFET_VAL_OVERRIDE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}

static inline fapi2::buffer<uint64_t>& SET_CPMS_CL2_PFETCNTL_VCS_PFET_SEL_OVERRIDE(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return o_data.insertFromRight<CPMS_CL2_PFETCNTL_VCS_PFET_SEL_OVERRIDE,
           1>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_VCS_PFET_SEL_OVERRIDE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}

static inline fapi2::buffer<uint64_t>& SET_CPMS_CL2_PFETCNTL_VCS_PFET_SEL_OVERRIDE(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return o_data.insertFromRight<CPMS_CL2_PFETCNTL_VCS_PFET_SEL_OVERRIDE,
           1>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_VCS_PFET_SEL_OVERRIDE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}

static inline fapi2::buffer<uint64_t>& SET_CPMS_CL2_PFETCNTL_VCS_PFET_SEL_OVERRIDE(fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return o_data.setBit<CPMS_CL2_PFETCNTL_VCS_PFET_SEL_OVERRIDE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_VCS_PFET_SEL_OVERRIDE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}

static inline fapi2::buffer<uint64_t>& CLEAR_CPMS_CL2_PFETCNTL_VCS_PFET_SEL_OVERRIDE(fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return o_data.clearBit<CPMS_CL2_PFETCNTL_VCS_PFET_SEL_OVERRIDE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_VCS_PFET_SEL_OVERRIDE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}

static inline fapi2::buffer<uint64_t>& GET_CPMS_CL2_PFETCNTL_VCS_PFET_SEL_OVERRIDE(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return i_data.extractToRight<CPMS_CL2_PFETCNTL_VCS_PFET_SEL_OVERRIDE,
           1>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_VCS_PFET_SEL_OVERRIDE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}

static inline bool GET_CPMS_CL2_PFETCNTL_VCS_PFET_SEL_OVERRIDE(fapi2::buffer<uint64_t>& i_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return i_data.getBit<CPMS_CL2_PFETCNTL_VCS_PFET_SEL_OVERRIDE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_VCS_PFET_SEL_OVERRIDE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}

static inline fapi2::buffer<uint64_t>& SET_CPMS_CL2_PFETCNTL_VDD_PFET_REGULATION_FINGER_SET(
    fapi2::buffer<uint64_t>& i_data,
    fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return o_data.insertFromRight<CPMS_CL2_PFETCNTL_VDD_PFET_REGULATION_FINGER_SET,
           1>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_VDD_PFET_REGULATION_FINGER_SET chip: %x, ec: %x", scomt::chip,
             scomt::ec);
    return o_data;
#endif
}

static inline fapi2::buffer<uint64_t>& SET_CPMS_CL2_PFETCNTL_VDD_PFET_REGULATION_FINGER_SET(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return o_data.insertFromRight<CPMS_CL2_PFETCNTL_VDD_PFET_REGULATION_FINGER_SET,
           1>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_VDD_PFET_REGULATION_FINGER_SET chip: %x, ec: %x", scomt::chip,
             scomt::ec);
    return o_data;
#endif
}

static inline fapi2::buffer<uint64_t>& SET_CPMS_CL2_PFETCNTL_VDD_PFET_REGULATION_FINGER_SET(
    fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return o_data.setBit<CPMS_CL2_PFETCNTL_VDD_PFET_REGULATION_FINGER_SET>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_VDD_PFET_REGULATION_FINGER_SET chip: %x, ec: %x", scomt::chip,
             scomt::ec);
    return o_data;
#endif
}

static inline fapi2::buffer<uint64_t>& CLEAR_CPMS_CL2_PFETCNTL_VDD_PFET_REGULATION_FINGER_SET(
    fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return o_data.clearBit<CPMS_CL2_PFETCNTL_VDD_PFET_REGULATION_FINGER_SET>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_VDD_PFET_REGULATION_FINGER_SET chip: %x, ec: %x", scomt::chip,
             scomt::ec);
    return o_data;
#endif
}

static inline fapi2::buffer<uint64_t>& GET_CPMS_CL2_PFETCNTL_VDD_PFET_REGULATION_FINGER_SET(
    fapi2::buffer<uint64_t>& i_data,
    uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return i_data.extractToRight<CPMS_CL2_PFETCNTL_VDD_PFET_REGULATION_FINGER_SET,
           1>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_VDD_PFET_REGULATION_FINGER_SET chip: %x, ec: %x", scomt::chip,
             scomt::ec);
    return i_data;
#endif
}

static inline bool GET_CPMS_CL2_PFETCNTL_VDD_PFET_REGULATION_FINGER_SET(fapi2::buffer<uint64_t>& i_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return i_data.getBit<CPMS_CL2_PFETCNTL_VDD_PFET_REGULATION_FINGER_SET>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_VDD_PFET_REGULATION_FINGER_SET chip: %x, ec: %x", scomt::chip,
             scomt::ec);
    return i_data;
#endif
}

static inline fapi2::buffer<uint64_t>& SET_CPMS_CL2_PFETCNTL_VDD_PFET_ENABLE_VALUE(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return o_data.insertFromRight<CPMS_CL2_PFETCNTL_VDD_PFET_ENABLE_VALUE,
           CPMS_CL2_PFETCNTL_VDD_PFET_ENABLE_VALUE_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_VDD_PFET_ENABLE_VALUE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}

static inline fapi2::buffer<uint64_t>& SET_CPMS_CL2_PFETCNTL_VDD_PFET_ENABLE_VALUE(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return o_data.insertFromRight<CPMS_CL2_PFETCNTL_VDD_PFET_ENABLE_VALUE,
           CPMS_CL2_PFETCNTL_VDD_PFET_ENABLE_VALUE_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_VDD_PFET_ENABLE_VALUE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}

static inline fapi2::buffer<uint64_t>& GET_CPMS_CL2_PFETCNTL_VDD_PFET_ENABLE_VALUE(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return i_data.extractToRight<CPMS_CL2_PFETCNTL_VDD_PFET_ENABLE_VALUE,
           CPMS_CL2_PFETCNTL_VDD_PFET_ENABLE_VALUE_LEN>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_VDD_PFET_ENABLE_VALUE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}

static inline fapi2::buffer<uint64_t>& SET_CPMS_CL2_PFETCNTL_VDD_PFET_SEL_VALUE(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return o_data.insertFromRight<CPMS_CL2_PFETCNTL_VDD_PFET_SEL_VALUE,
           CPMS_CL2_PFETCNTL_VDD_PFET_SEL_VALUE_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_VDD_PFET_SEL_VALUE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}

static inline fapi2::buffer<uint64_t>& SET_CPMS_CL2_PFETCNTL_VDD_PFET_SEL_VALUE(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return o_data.insertFromRight<CPMS_CL2_PFETCNTL_VDD_PFET_SEL_VALUE,
           CPMS_CL2_PFETCNTL_VDD_PFET_SEL_VALUE_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_VDD_PFET_SEL_VALUE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}

static inline fapi2::buffer<uint64_t>& GET_CPMS_CL2_PFETCNTL_VDD_PFET_SEL_VALUE(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return i_data.extractToRight<CPMS_CL2_PFETCNTL_VDD_PFET_SEL_VALUE,
           CPMS_CL2_PFETCNTL_VDD_PFET_SEL_VALUE_LEN>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_VDD_PFET_SEL_VALUE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}

static inline fapi2::buffer<uint64_t>& SET_CPMS_CL2_PFETCNTL_VCS_PFET_ENABLE_VALUE(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return o_data.insertFromRight<CPMS_CL2_PFETCNTL_VCS_PFET_ENABLE_VALUE,
           CPMS_CL2_PFETCNTL_VCS_PFET_ENABLE_VALUE_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_VCS_PFET_ENABLE_VALUE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}

static inline fapi2::buffer<uint64_t>& SET_CPMS_CL2_PFETCNTL_VCS_PFET_ENABLE_VALUE(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return o_data.insertFromRight<CPMS_CL2_PFETCNTL_VCS_PFET_ENABLE_VALUE,
           CPMS_CL2_PFETCNTL_VCS_PFET_ENABLE_VALUE_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_VCS_PFET_ENABLE_VALUE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}

static inline fapi2::buffer<uint64_t>& GET_CPMS_CL2_PFETCNTL_VCS_PFET_ENABLE_VALUE(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return i_data.extractToRight<CPMS_CL2_PFETCNTL_VCS_PFET_ENABLE_VALUE,
           CPMS_CL2_PFETCNTL_VCS_PFET_ENABLE_VALUE_LEN>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_VCS_PFET_ENABLE_VALUE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}

static inline fapi2::buffer<uint64_t>& SET_CPMS_CL2_PFETCNTL_VCS_PFET_SEL_VALUE(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return o_data.insertFromRight<CPMS_CL2_PFETCNTL_VCS_PFET_SEL_VALUE,
           CPMS_CL2_PFETCNTL_VCS_PFET_SEL_VALUE_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_VCS_PFET_SEL_VALUE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}

static inline fapi2::buffer<uint64_t>& SET_CPMS_CL2_PFETCNTL_VCS_PFET_SEL_VALUE(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return o_data.insertFromRight<CPMS_CL2_PFETCNTL_VCS_PFET_SEL_VALUE,
           CPMS_CL2_PFETCNTL_VCS_PFET_SEL_VALUE_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_VCS_PFET_SEL_VALUE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}

static inline fapi2::buffer<uint64_t>& GET_CPMS_CL2_PFETCNTL_VCS_PFET_SEL_VALUE(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return i_data.extractToRight<CPMS_CL2_PFETCNTL_VCS_PFET_SEL_VALUE,
           CPMS_CL2_PFETCNTL_VCS_PFET_SEL_VALUE_LEN>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_VCS_PFET_SEL_VALUE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}

static inline fapi2::buffer<uint64_t>& SET_CPMS_CL2_PFETCNTL_VDD_PG_STATE(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return o_data.insertFromRight<CPMS_CL2_PFETCNTL_VDD_PG_STATE,
           CPMS_CL2_PFETCNTL_VDD_PG_STATE_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_VDD_PG_STATE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}

static inline fapi2::buffer<uint64_t>& SET_CPMS_CL2_PFETCNTL_VDD_PG_STATE(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return o_data.insertFromRight<CPMS_CL2_PFETCNTL_VDD_PG_STATE,
           CPMS_CL2_PFETCNTL_VDD_PG_STATE_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_VDD_PG_STATE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}

static inline fapi2::buffer<uint64_t>& GET_CPMS_CL2_PFETCNTL_VDD_PG_STATE(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return i_data.extractToRight<CPMS_CL2_PFETCNTL_VDD_PG_STATE,
           CPMS_CL2_PFETCNTL_VDD_PG_STATE_LEN>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_VDD_PG_STATE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}

static inline fapi2::buffer<uint64_t>& SET_CPMS_CL2_PFETCNTL_VDD_PG_SEL(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return o_data.insertFromRight<CPMS_CL2_PFETCNTL_VDD_PG_SEL,
           CPMS_CL2_PFETCNTL_VDD_PG_SEL_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_VDD_PG_SEL chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}

static inline fapi2::buffer<uint64_t>& SET_CPMS_CL2_PFETCNTL_VDD_PG_SEL(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return o_data.insertFromRight<CPMS_CL2_PFETCNTL_VDD_PG_SEL,
           CPMS_CL2_PFETCNTL_VDD_PG_SEL_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_VDD_PG_SEL chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}

static inline fapi2::buffer<uint64_t>& GET_CPMS_CL2_PFETCNTL_VDD_PG_SEL(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return i_data.extractToRight<CPMS_CL2_PFETCNTL_VDD_PG_SEL,
           CPMS_CL2_PFETCNTL_VDD_PG_SEL_LEN>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_VDD_PG_SEL chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}

static inline fapi2::buffer<uint64_t>& SET_CPMS_CL2_PFETCNTL_VCS_PG_STATE(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return o_data.insertFromRight<CPMS_CL2_PFETCNTL_VCS_PG_STATE,
           CPMS_CL2_PFETCNTL_VCS_PG_STATE_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_VCS_PG_STATE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}

static inline fapi2::buffer<uint64_t>& SET_CPMS_CL2_PFETCNTL_VCS_PG_STATE(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return o_data.insertFromRight<CPMS_CL2_PFETCNTL_VCS_PG_STATE,
           CPMS_CL2_PFETCNTL_VCS_PG_STATE_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_VCS_PG_STATE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}

static inline fapi2::buffer<uint64_t>& GET_CPMS_CL2_PFETCNTL_VCS_PG_STATE(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return i_data.extractToRight<CPMS_CL2_PFETCNTL_VCS_PG_STATE,
           CPMS_CL2_PFETCNTL_VCS_PG_STATE_LEN>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_VCS_PG_STATE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}

static inline fapi2::buffer<uint64_t>& SET_CPMS_CL2_PFETCNTL_VCS_PG_SEL(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return o_data.insertFromRight<CPMS_CL2_PFETCNTL_VCS_PG_SEL,
           CPMS_CL2_PFETCNTL_VCS_PG_SEL_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_VCS_PG_SEL chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}

static inline fapi2::buffer<uint64_t>& SET_CPMS_CL2_PFETCNTL_VCS_PG_SEL(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return o_data.insertFromRight<CPMS_CL2_PFETCNTL_VCS_PG_SEL,
           CPMS_CL2_PFETCNTL_VCS_PG_SEL_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_VCS_PG_SEL chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}

static inline fapi2::buffer<uint64_t>& GET_CPMS_CL2_PFETCNTL_VCS_PG_SEL(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return i_data.extractToRight<CPMS_CL2_PFETCNTL_VCS_PG_SEL,
           CPMS_CL2_PFETCNTL_VCS_PG_SEL_LEN>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_VCS_PG_SEL chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}

static inline fapi2::buffer<uint64_t>& SET_CPMS_CL2_PFETCNTL_SRAM_ENABLE(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return o_data.insertFromRight<CPMS_CL2_PFETCNTL_SRAM_ENABLE,
           1>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_SRAM_ENABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}

static inline fapi2::buffer<uint64_t>& SET_CPMS_CL2_PFETCNTL_SRAM_ENABLE(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return o_data.insertFromRight<CPMS_CL2_PFETCNTL_SRAM_ENABLE,
           1>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_SRAM_ENABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}

static inline fapi2::buffer<uint64_t>& SET_CPMS_CL2_PFETCNTL_SRAM_ENABLE(fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return o_data.setBit<CPMS_CL2_PFETCNTL_SRAM_ENABLE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_SRAM_ENABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}

static inline fapi2::buffer<uint64_t>& CLEAR_CPMS_CL2_PFETCNTL_SRAM_ENABLE(fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return o_data.clearBit<CPMS_CL2_PFETCNTL_SRAM_ENABLE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_SRAM_ENABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}

static inline fapi2::buffer<uint64_t>& GET_CPMS_CL2_PFETCNTL_SRAM_ENABLE(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return i_data.extractToRight<CPMS_CL2_PFETCNTL_SRAM_ENABLE,
           1>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_SRAM_ENABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}

static inline bool GET_CPMS_CL2_PFETCNTL_SRAM_ENABLE(fapi2::buffer<uint64_t>& i_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return i_data.getBit<CPMS_CL2_PFETCNTL_SRAM_ENABLE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access CPMS_CL2_PFETCNTL_SRAM_ENABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}

template< fapi2::TargetType K, fapi2::MulticastType M, typename V >
static inline fapi2::ReturnCode PREP_QME_CDBA(const fapi2::Target<K, M, V>& i_target)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::init(i_target));
    scomt::last_scom = QME_CDBA;
#endif
    return fapi2::current_err;
#ifdef SCOM_CHECKING
fapi_try_exit:
    return fapi2::current_err;
#endif
}

template< fapi2::TargetType K, fapi2::MulticastType M, typename V >
static inline fapi2::ReturnCode GET_QME_CDBA(const fapi2::Target<K, M, V>& i_target, fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::init(i_target));
    scomt::last_scom = QME_CDBA;
#endif
    return fapi2::getScom(i_target, QME_CDBA, o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    return fapi2::current_err;
#endif
}

template< fapi2::TargetType K, fapi2::MulticastType M, typename V >
static inline fapi2::ReturnCode PUT_QME_CDBA(const fapi2::Target<K, M, V>& i_target, const uint64_t i_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::init(i_target));
    FAPI_TRY(scomt::chkReg(QME_CDBA));
#endif
    return fapi2::putScom(i_target, QME_CDBA, i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    return fapi2::current_err;
#endif
}


static inline fapi2::buffer<uint64_t>& SET_QME_CDBA_QME_CDBA_MESSAGE_NUMBER(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_CDBA));
#endif
    return o_data.insertFromRight<QME_CDBA_QME_CDBA_MESSAGE_NUMBER,
           QME_CDBA_QME_CDBA_MESSAGE_NUMBER_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access QME_CDBA_QME_CDBA_MESSAGE_NUMBER chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}

static inline fapi2::buffer<uint64_t>& SET_QME_CDBA_QME_CDBA_MESSAGE_NUMBER(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_CDBA));
#endif
    return o_data.insertFromRight<QME_CDBA_QME_CDBA_MESSAGE_NUMBER,
           QME_CDBA_QME_CDBA_MESSAGE_NUMBER_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access QME_CDBA_QME_CDBA_MESSAGE_NUMBER chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}

static inline fapi2::buffer<uint64_t>& GET_QME_CDBA_QME_CDBA_MESSAGE_NUMBER(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_CDBA));
#endif
    return i_data.extractToRight<QME_CDBA_QME_CDBA_MESSAGE_NUMBER,
           QME_CDBA_QME_CDBA_MESSAGE_NUMBER_LEN>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access QME_CDBA_QME_CDBA_MESSAGE_NUMBER chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}

template< fapi2::TargetType K, fapi2::MulticastType M, typename V >
static inline fapi2::ReturnCode PREP_QME_CSIR(const fapi2::Target<K, M, V>& i_target)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::init(i_target));
    scomt::last_scom = QME_CSIR;
#endif
    return fapi2::current_err;
#ifdef SCOM_CHECKING
fapi_try_exit:
    return fapi2::current_err;
#endif
}

template< fapi2::TargetType K, fapi2::MulticastType M, typename V >
static inline fapi2::ReturnCode GET_QME_CSIR(const fapi2::Target<K, M, V>& i_target, fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::init(i_target));
    scomt::last_scom = QME_CSIR;
#endif
    return fapi2::getScom(i_target, QME_CSIR, o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    return fapi2::current_err;
#endif
}

template< fapi2::TargetType K, fapi2::MulticastType M, typename V >
static inline fapi2::ReturnCode PUT_QME_CSIR(const fapi2::Target<K, M, V>& i_target, const uint64_t i_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::init(i_target));
    FAPI_TRY(scomt::chkReg(QME_CSIR));
#endif
    return fapi2::putScom(i_target, QME_CSIR, i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    return fapi2::current_err;
#endif
}


static inline fapi2::buffer<uint64_t>& SET_QME_CSIR_QME_CSIR_PMSR_INSTR_DATA(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_CSIR));
#endif
    return o_data.insertFromRight<QME_CSIR_QME_CSIR_PMSR_INSTR_DATA,
           QME_CSIR_QME_CSIR_PMSR_INSTR_DATA_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access QME_CSIR_QME_CSIR_PMSR_INSTR_DATA chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}

static inline fapi2::buffer<uint64_t>& SET_QME_CSIR_QME_CSIR_PMSR_INSTR_DATA(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_CSIR));
#endif
    return o_data.insertFromRight<QME_CSIR_QME_CSIR_PMSR_INSTR_DATA,
           QME_CSIR_QME_CSIR_PMSR_INSTR_DATA_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access QME_CSIR_QME_CSIR_PMSR_INSTR_DATA chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}

static inline fapi2::buffer<uint64_t>& GET_QME_CSIR_QME_CSIR_PMSR_INSTR_DATA(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_CSIR));
#endif
    return i_data.extractToRight<QME_CSIR_QME_CSIR_PMSR_INSTR_DATA,
           QME_CSIR_QME_CSIR_PMSR_INSTR_DATA_LEN>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("Invalid dial access QME_CSIR_QME_CSIR_PMSR_INSTR_DATA chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}


}
}

#endif

/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/common/include/ppe_c/reg00003.H $        */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2019,2020                        */
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

#ifndef __PPE_HCODE__
namespace scomt
{
namespace ppe_c
{
#endif


//>> PREP_[CPMS_CL2_PFETCNTL]
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
//<< PREP_[CPMS_CL2_PFETCNTL]

//>> GET_[CPMS_CL2_PFETCNTL]
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
//<< GET_[CPMS_CL2_PFETCNTL]

//>> PUT_[CPMS_CL2_PFETCNTL]
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
//<< PUT_[CPMS_CL2_PFETCNTL]

//>> PREP_[CPMS_CL2_PFETCNTL_WO_CLEAR]
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
//<< PREP_[CPMS_CL2_PFETCNTL_WO_CLEAR]

//>> GET_[CPMS_CL2_PFETCNTL_WO_CLEAR]
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
//<< GET_[CPMS_CL2_PFETCNTL_WO_CLEAR]

//>> PUT_[CPMS_CL2_PFETCNTL_WO_CLEAR]
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
//<< PUT_[CPMS_CL2_PFETCNTL_WO_CLEAR]

//>> PREP_[CPMS_CL2_PFETCNTL_WO_OR]
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
//<< PREP_[CPMS_CL2_PFETCNTL_WO_OR]

//>> GET_[CPMS_CL2_PFETCNTL_WO_OR]
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
//<< GET_[CPMS_CL2_PFETCNTL_WO_OR]

//>> PUT_[CPMS_CL2_PFETCNTL_WO_OR]
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
//<< PUT_[CPMS_CL2_PFETCNTL_WO_OR]


//>> SET_[CPMS_CL2_PFETCNTL_VDD_PFET_SEQ_STATE]
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
    FAPI_ERR("0:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_VDD_PFET_SEQ_STATE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CL2_PFETCNTL_VDD_PFET_SEQ_STATE]

//>> SET_[CPMS_CL2_PFETCNTL_VDD_PFET_SEQ_STATE]
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
    FAPI_ERR("1:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_VDD_PFET_SEQ_STATE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CL2_PFETCNTL_VDD_PFET_SEQ_STATE]

//>> GET_[CPMS_CL2_PFETCNTL_VDD_PFET_SEQ_STATE]
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
    FAPI_ERR("4:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_VDD_PFET_SEQ_STATE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_CL2_PFETCNTL_VDD_PFET_SEQ_STATE]

//>> SET_[CPMS_CL2_PFETCNTL_VCS_PFET_SEQ_STATE]
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
    FAPI_ERR("0:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_VCS_PFET_SEQ_STATE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CL2_PFETCNTL_VCS_PFET_SEQ_STATE]

//>> SET_[CPMS_CL2_PFETCNTL_VCS_PFET_SEQ_STATE]
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
    FAPI_ERR("1:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_VCS_PFET_SEQ_STATE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CL2_PFETCNTL_VCS_PFET_SEQ_STATE]

//>> GET_[CPMS_CL2_PFETCNTL_VCS_PFET_SEQ_STATE]
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
    FAPI_ERR("4:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_VCS_PFET_SEQ_STATE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_CL2_PFETCNTL_VCS_PFET_SEQ_STATE]

//>> SET_[CPMS_CL2_PFETCNTL_VDD_PFET_VAL_OVERRIDE]
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
    FAPI_ERR("0:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_VDD_PFET_VAL_OVERRIDE chip: %x, ec: %x", scomt::chip,
             scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CL2_PFETCNTL_VDD_PFET_VAL_OVERRIDE]

//>> SET_[CPMS_CL2_PFETCNTL_VDD_PFET_VAL_OVERRIDE]
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
    FAPI_ERR("1:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_VDD_PFET_VAL_OVERRIDE chip: %x, ec: %x", scomt::chip,
             scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CL2_PFETCNTL_VDD_PFET_VAL_OVERRIDE]

//>> SET_[CPMS_CL2_PFETCNTL_VDD_PFET_VAL_OVERRIDE]
static inline fapi2::buffer<uint64_t>& SET_CPMS_CL2_PFETCNTL_VDD_PFET_VAL_OVERRIDE(fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return o_data.setBit<CPMS_CL2_PFETCNTL_VDD_PFET_VAL_OVERRIDE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("2:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_VDD_PFET_VAL_OVERRIDE chip: %x, ec: %x", scomt::chip,
             scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CL2_PFETCNTL_VDD_PFET_VAL_OVERRIDE]

//>> CLEAR_[CPMS_CL2_PFETCNTL_VDD_PFET_VAL_OVERRIDE]
static inline fapi2::buffer<uint64_t>& CLEAR_CPMS_CL2_PFETCNTL_VDD_PFET_VAL_OVERRIDE(fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return o_data.clearBit<CPMS_CL2_PFETCNTL_VDD_PFET_VAL_OVERRIDE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("3:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_VDD_PFET_VAL_OVERRIDE chip: %x, ec: %x", scomt::chip,
             scomt::ec);
    return o_data;
#endif
}
//<< CLEAR_[CPMS_CL2_PFETCNTL_VDD_PFET_VAL_OVERRIDE]

//>> GET_[CPMS_CL2_PFETCNTL_VDD_PFET_VAL_OVERRIDE]
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
    FAPI_ERR("4:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_VDD_PFET_VAL_OVERRIDE chip: %x, ec: %x", scomt::chip,
             scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_CL2_PFETCNTL_VDD_PFET_VAL_OVERRIDE]

//>> GET_[CPMS_CL2_PFETCNTL_VDD_PFET_VAL_OVERRIDE]
static inline bool GET_CPMS_CL2_PFETCNTL_VDD_PFET_VAL_OVERRIDE(fapi2::buffer<uint64_t>& i_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return i_data.getBit<CPMS_CL2_PFETCNTL_VDD_PFET_VAL_OVERRIDE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("5:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_VDD_PFET_VAL_OVERRIDE chip: %x, ec: %x", scomt::chip,
             scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_CL2_PFETCNTL_VDD_PFET_VAL_OVERRIDE]

//>> SET_[CPMS_CL2_PFETCNTL_VDD_PFET_SEL_OVERRIDE]
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
    FAPI_ERR("0:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_VDD_PFET_SEL_OVERRIDE chip: %x, ec: %x", scomt::chip,
             scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CL2_PFETCNTL_VDD_PFET_SEL_OVERRIDE]

//>> SET_[CPMS_CL2_PFETCNTL_VDD_PFET_SEL_OVERRIDE]
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
    FAPI_ERR("1:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_VDD_PFET_SEL_OVERRIDE chip: %x, ec: %x", scomt::chip,
             scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CL2_PFETCNTL_VDD_PFET_SEL_OVERRIDE]

//>> SET_[CPMS_CL2_PFETCNTL_VDD_PFET_SEL_OVERRIDE]
static inline fapi2::buffer<uint64_t>& SET_CPMS_CL2_PFETCNTL_VDD_PFET_SEL_OVERRIDE(fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return o_data.setBit<CPMS_CL2_PFETCNTL_VDD_PFET_SEL_OVERRIDE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("2:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_VDD_PFET_SEL_OVERRIDE chip: %x, ec: %x", scomt::chip,
             scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CL2_PFETCNTL_VDD_PFET_SEL_OVERRIDE]

//>> CLEAR_[CPMS_CL2_PFETCNTL_VDD_PFET_SEL_OVERRIDE]
static inline fapi2::buffer<uint64_t>& CLEAR_CPMS_CL2_PFETCNTL_VDD_PFET_SEL_OVERRIDE(fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return o_data.clearBit<CPMS_CL2_PFETCNTL_VDD_PFET_SEL_OVERRIDE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("3:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_VDD_PFET_SEL_OVERRIDE chip: %x, ec: %x", scomt::chip,
             scomt::ec);
    return o_data;
#endif
}
//<< CLEAR_[CPMS_CL2_PFETCNTL_VDD_PFET_SEL_OVERRIDE]

//>> GET_[CPMS_CL2_PFETCNTL_VDD_PFET_SEL_OVERRIDE]
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
    FAPI_ERR("4:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_VDD_PFET_SEL_OVERRIDE chip: %x, ec: %x", scomt::chip,
             scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_CL2_PFETCNTL_VDD_PFET_SEL_OVERRIDE]

//>> GET_[CPMS_CL2_PFETCNTL_VDD_PFET_SEL_OVERRIDE]
static inline bool GET_CPMS_CL2_PFETCNTL_VDD_PFET_SEL_OVERRIDE(fapi2::buffer<uint64_t>& i_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return i_data.getBit<CPMS_CL2_PFETCNTL_VDD_PFET_SEL_OVERRIDE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("5:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_VDD_PFET_SEL_OVERRIDE chip: %x, ec: %x", scomt::chip,
             scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_CL2_PFETCNTL_VDD_PFET_SEL_OVERRIDE]

//>> SET_[CPMS_CL2_PFETCNTL_VCS_PFET_VAL_OVERRIDE]
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
    FAPI_ERR("0:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_VCS_PFET_VAL_OVERRIDE chip: %x, ec: %x", scomt::chip,
             scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CL2_PFETCNTL_VCS_PFET_VAL_OVERRIDE]

//>> SET_[CPMS_CL2_PFETCNTL_VCS_PFET_VAL_OVERRIDE]
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
    FAPI_ERR("1:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_VCS_PFET_VAL_OVERRIDE chip: %x, ec: %x", scomt::chip,
             scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CL2_PFETCNTL_VCS_PFET_VAL_OVERRIDE]

//>> SET_[CPMS_CL2_PFETCNTL_VCS_PFET_VAL_OVERRIDE]
static inline fapi2::buffer<uint64_t>& SET_CPMS_CL2_PFETCNTL_VCS_PFET_VAL_OVERRIDE(fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return o_data.setBit<CPMS_CL2_PFETCNTL_VCS_PFET_VAL_OVERRIDE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("2:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_VCS_PFET_VAL_OVERRIDE chip: %x, ec: %x", scomt::chip,
             scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CL2_PFETCNTL_VCS_PFET_VAL_OVERRIDE]

//>> CLEAR_[CPMS_CL2_PFETCNTL_VCS_PFET_VAL_OVERRIDE]
static inline fapi2::buffer<uint64_t>& CLEAR_CPMS_CL2_PFETCNTL_VCS_PFET_VAL_OVERRIDE(fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return o_data.clearBit<CPMS_CL2_PFETCNTL_VCS_PFET_VAL_OVERRIDE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("3:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_VCS_PFET_VAL_OVERRIDE chip: %x, ec: %x", scomt::chip,
             scomt::ec);
    return o_data;
#endif
}
//<< CLEAR_[CPMS_CL2_PFETCNTL_VCS_PFET_VAL_OVERRIDE]

//>> GET_[CPMS_CL2_PFETCNTL_VCS_PFET_VAL_OVERRIDE]
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
    FAPI_ERR("4:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_VCS_PFET_VAL_OVERRIDE chip: %x, ec: %x", scomt::chip,
             scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_CL2_PFETCNTL_VCS_PFET_VAL_OVERRIDE]

//>> GET_[CPMS_CL2_PFETCNTL_VCS_PFET_VAL_OVERRIDE]
static inline bool GET_CPMS_CL2_PFETCNTL_VCS_PFET_VAL_OVERRIDE(fapi2::buffer<uint64_t>& i_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return i_data.getBit<CPMS_CL2_PFETCNTL_VCS_PFET_VAL_OVERRIDE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("5:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_VCS_PFET_VAL_OVERRIDE chip: %x, ec: %x", scomt::chip,
             scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_CL2_PFETCNTL_VCS_PFET_VAL_OVERRIDE]

//>> SET_[CPMS_CL2_PFETCNTL_VCS_PFET_SEL_OVERRIDE]
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
    FAPI_ERR("0:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_VCS_PFET_SEL_OVERRIDE chip: %x, ec: %x", scomt::chip,
             scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CL2_PFETCNTL_VCS_PFET_SEL_OVERRIDE]

//>> SET_[CPMS_CL2_PFETCNTL_VCS_PFET_SEL_OVERRIDE]
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
    FAPI_ERR("1:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_VCS_PFET_SEL_OVERRIDE chip: %x, ec: %x", scomt::chip,
             scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CL2_PFETCNTL_VCS_PFET_SEL_OVERRIDE]

//>> SET_[CPMS_CL2_PFETCNTL_VCS_PFET_SEL_OVERRIDE]
static inline fapi2::buffer<uint64_t>& SET_CPMS_CL2_PFETCNTL_VCS_PFET_SEL_OVERRIDE(fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return o_data.setBit<CPMS_CL2_PFETCNTL_VCS_PFET_SEL_OVERRIDE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("2:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_VCS_PFET_SEL_OVERRIDE chip: %x, ec: %x", scomt::chip,
             scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CL2_PFETCNTL_VCS_PFET_SEL_OVERRIDE]

//>> CLEAR_[CPMS_CL2_PFETCNTL_VCS_PFET_SEL_OVERRIDE]
static inline fapi2::buffer<uint64_t>& CLEAR_CPMS_CL2_PFETCNTL_VCS_PFET_SEL_OVERRIDE(fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return o_data.clearBit<CPMS_CL2_PFETCNTL_VCS_PFET_SEL_OVERRIDE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("3:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_VCS_PFET_SEL_OVERRIDE chip: %x, ec: %x", scomt::chip,
             scomt::ec);
    return o_data;
#endif
}
//<< CLEAR_[CPMS_CL2_PFETCNTL_VCS_PFET_SEL_OVERRIDE]

//>> GET_[CPMS_CL2_PFETCNTL_VCS_PFET_SEL_OVERRIDE]
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
    FAPI_ERR("4:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_VCS_PFET_SEL_OVERRIDE chip: %x, ec: %x", scomt::chip,
             scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_CL2_PFETCNTL_VCS_PFET_SEL_OVERRIDE]

//>> GET_[CPMS_CL2_PFETCNTL_VCS_PFET_SEL_OVERRIDE]
static inline bool GET_CPMS_CL2_PFETCNTL_VCS_PFET_SEL_OVERRIDE(fapi2::buffer<uint64_t>& i_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return i_data.getBit<CPMS_CL2_PFETCNTL_VCS_PFET_SEL_OVERRIDE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("5:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_VCS_PFET_SEL_OVERRIDE chip: %x, ec: %x", scomt::chip,
             scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_CL2_PFETCNTL_VCS_PFET_SEL_OVERRIDE]

//>> SET_[CPMS_CL2_PFETCNTL_VDD_PFET_REGULATION_FINGER_SET]
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
    FAPI_ERR("0:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_VDD_PFET_REGULATION_FINGER_SET chip: %x, ec: %x", scomt::chip,
             scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CL2_PFETCNTL_VDD_PFET_REGULATION_FINGER_SET]

//>> SET_[CPMS_CL2_PFETCNTL_VDD_PFET_REGULATION_FINGER_SET]
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
    FAPI_ERR("1:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_VDD_PFET_REGULATION_FINGER_SET chip: %x, ec: %x", scomt::chip,
             scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CL2_PFETCNTL_VDD_PFET_REGULATION_FINGER_SET]

//>> SET_[CPMS_CL2_PFETCNTL_VDD_PFET_REGULATION_FINGER_SET]
static inline fapi2::buffer<uint64_t>& SET_CPMS_CL2_PFETCNTL_VDD_PFET_REGULATION_FINGER_SET(
    fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return o_data.setBit<CPMS_CL2_PFETCNTL_VDD_PFET_REGULATION_FINGER_SET>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("2:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_VDD_PFET_REGULATION_FINGER_SET chip: %x, ec: %x", scomt::chip,
             scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CL2_PFETCNTL_VDD_PFET_REGULATION_FINGER_SET]

//>> CLEAR_[CPMS_CL2_PFETCNTL_VDD_PFET_REGULATION_FINGER_SET]
static inline fapi2::buffer<uint64_t>& CLEAR_CPMS_CL2_PFETCNTL_VDD_PFET_REGULATION_FINGER_SET(
    fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return o_data.clearBit<CPMS_CL2_PFETCNTL_VDD_PFET_REGULATION_FINGER_SET>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("3:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_VDD_PFET_REGULATION_FINGER_SET chip: %x, ec: %x", scomt::chip,
             scomt::ec);
    return o_data;
#endif
}
//<< CLEAR_[CPMS_CL2_PFETCNTL_VDD_PFET_REGULATION_FINGER_SET]

//>> GET_[CPMS_CL2_PFETCNTL_VDD_PFET_REGULATION_FINGER_SET]
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
    FAPI_ERR("4:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_VDD_PFET_REGULATION_FINGER_SET chip: %x, ec: %x", scomt::chip,
             scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_CL2_PFETCNTL_VDD_PFET_REGULATION_FINGER_SET]

//>> GET_[CPMS_CL2_PFETCNTL_VDD_PFET_REGULATION_FINGER_SET]
static inline bool GET_CPMS_CL2_PFETCNTL_VDD_PFET_REGULATION_FINGER_SET(fapi2::buffer<uint64_t>& i_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return i_data.getBit<CPMS_CL2_PFETCNTL_VDD_PFET_REGULATION_FINGER_SET>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("5:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_VDD_PFET_REGULATION_FINGER_SET chip: %x, ec: %x", scomt::chip,
             scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_CL2_PFETCNTL_VDD_PFET_REGULATION_FINGER_SET]

//>> SET_[CPMS_CL2_PFETCNTL_VDD_PFET_ENABLE_VALUE]
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
    FAPI_ERR("0:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_VDD_PFET_ENABLE_VALUE chip: %x, ec: %x", scomt::chip,
             scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CL2_PFETCNTL_VDD_PFET_ENABLE_VALUE]

//>> SET_[CPMS_CL2_PFETCNTL_VDD_PFET_ENABLE_VALUE]
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
    FAPI_ERR("1:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_VDD_PFET_ENABLE_VALUE chip: %x, ec: %x", scomt::chip,
             scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CL2_PFETCNTL_VDD_PFET_ENABLE_VALUE]

//>> GET_[CPMS_CL2_PFETCNTL_VDD_PFET_ENABLE_VALUE]
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
    FAPI_ERR("4:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_VDD_PFET_ENABLE_VALUE chip: %x, ec: %x", scomt::chip,
             scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_CL2_PFETCNTL_VDD_PFET_ENABLE_VALUE]

//>> SET_[CPMS_CL2_PFETCNTL_VDD_PFET_SEL_VALUE]
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
    FAPI_ERR("0:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_VDD_PFET_SEL_VALUE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CL2_PFETCNTL_VDD_PFET_SEL_VALUE]

//>> SET_[CPMS_CL2_PFETCNTL_VDD_PFET_SEL_VALUE]
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
    FAPI_ERR("1:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_VDD_PFET_SEL_VALUE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CL2_PFETCNTL_VDD_PFET_SEL_VALUE]

//>> GET_[CPMS_CL2_PFETCNTL_VDD_PFET_SEL_VALUE]
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
    FAPI_ERR("4:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_VDD_PFET_SEL_VALUE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_CL2_PFETCNTL_VDD_PFET_SEL_VALUE]

//>> SET_[CPMS_CL2_PFETCNTL_VCS_PFET_ENABLE_VALUE]
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
    FAPI_ERR("0:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_VCS_PFET_ENABLE_VALUE chip: %x, ec: %x", scomt::chip,
             scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CL2_PFETCNTL_VCS_PFET_ENABLE_VALUE]

//>> SET_[CPMS_CL2_PFETCNTL_VCS_PFET_ENABLE_VALUE]
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
    FAPI_ERR("1:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_VCS_PFET_ENABLE_VALUE chip: %x, ec: %x", scomt::chip,
             scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CL2_PFETCNTL_VCS_PFET_ENABLE_VALUE]

//>> GET_[CPMS_CL2_PFETCNTL_VCS_PFET_ENABLE_VALUE]
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
    FAPI_ERR("4:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_VCS_PFET_ENABLE_VALUE chip: %x, ec: %x", scomt::chip,
             scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_CL2_PFETCNTL_VCS_PFET_ENABLE_VALUE]

//>> SET_[CPMS_CL2_PFETCNTL_VCS_PFET_SEL_VALUE]
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
    FAPI_ERR("0:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_VCS_PFET_SEL_VALUE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CL2_PFETCNTL_VCS_PFET_SEL_VALUE]

//>> SET_[CPMS_CL2_PFETCNTL_VCS_PFET_SEL_VALUE]
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
    FAPI_ERR("1:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_VCS_PFET_SEL_VALUE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CL2_PFETCNTL_VCS_PFET_SEL_VALUE]

//>> GET_[CPMS_CL2_PFETCNTL_VCS_PFET_SEL_VALUE]
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
    FAPI_ERR("4:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_VCS_PFET_SEL_VALUE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_CL2_PFETCNTL_VCS_PFET_SEL_VALUE]

//>> SET_[CPMS_CL2_PFETCNTL_VDD_PG_STATE]
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
    FAPI_ERR("0:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_VDD_PG_STATE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CL2_PFETCNTL_VDD_PG_STATE]

//>> SET_[CPMS_CL2_PFETCNTL_VDD_PG_STATE]
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
    FAPI_ERR("1:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_VDD_PG_STATE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CL2_PFETCNTL_VDD_PG_STATE]

//>> GET_[CPMS_CL2_PFETCNTL_VDD_PG_STATE]
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
    FAPI_ERR("4:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_VDD_PG_STATE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_CL2_PFETCNTL_VDD_PG_STATE]

//>> SET_[CPMS_CL2_PFETCNTL_VDD_PG_SEL]
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
    FAPI_ERR("0:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_VDD_PG_SEL chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CL2_PFETCNTL_VDD_PG_SEL]

//>> SET_[CPMS_CL2_PFETCNTL_VDD_PG_SEL]
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
    FAPI_ERR("1:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_VDD_PG_SEL chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CL2_PFETCNTL_VDD_PG_SEL]

//>> GET_[CPMS_CL2_PFETCNTL_VDD_PG_SEL]
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
    FAPI_ERR("4:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_VDD_PG_SEL chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_CL2_PFETCNTL_VDD_PG_SEL]

//>> SET_[CPMS_CL2_PFETCNTL_VCS_PG_STATE]
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
    FAPI_ERR("0:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_VCS_PG_STATE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CL2_PFETCNTL_VCS_PG_STATE]

//>> SET_[CPMS_CL2_PFETCNTL_VCS_PG_STATE]
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
    FAPI_ERR("1:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_VCS_PG_STATE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CL2_PFETCNTL_VCS_PG_STATE]

//>> GET_[CPMS_CL2_PFETCNTL_VCS_PG_STATE]
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
    FAPI_ERR("4:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_VCS_PG_STATE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_CL2_PFETCNTL_VCS_PG_STATE]

//>> SET_[CPMS_CL2_PFETCNTL_VCS_PG_SEL]
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
    FAPI_ERR("0:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_VCS_PG_SEL chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CL2_PFETCNTL_VCS_PG_SEL]

//>> SET_[CPMS_CL2_PFETCNTL_VCS_PG_SEL]
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
    FAPI_ERR("1:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_VCS_PG_SEL chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CL2_PFETCNTL_VCS_PG_SEL]

//>> GET_[CPMS_CL2_PFETCNTL_VCS_PG_SEL]
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
    FAPI_ERR("4:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_VCS_PG_SEL chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_CL2_PFETCNTL_VCS_PG_SEL]

//>> SET_[CPMS_CL2_PFETCNTL_ACS_MINMAX_SEL]
static inline fapi2::buffer<uint64_t>& SET_CPMS_CL2_PFETCNTL_ACS_MINMAX_SEL(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return o_data.insertFromRight<CPMS_CL2_PFETCNTL_ACS_MINMAX_SEL,
           CPMS_CL2_PFETCNTL_ACS_MINMAX_SEL_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("0:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_ACS_MINMAX_SEL chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CL2_PFETCNTL_ACS_MINMAX_SEL]

//>> SET_[CPMS_CL2_PFETCNTL_ACS_MINMAX_SEL]
static inline fapi2::buffer<uint64_t>& SET_CPMS_CL2_PFETCNTL_ACS_MINMAX_SEL(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return o_data.insertFromRight<CPMS_CL2_PFETCNTL_ACS_MINMAX_SEL,
           CPMS_CL2_PFETCNTL_ACS_MINMAX_SEL_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("1:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_ACS_MINMAX_SEL chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CL2_PFETCNTL_ACS_MINMAX_SEL]

//>> GET_[CPMS_CL2_PFETCNTL_ACS_MINMAX_SEL]
static inline fapi2::buffer<uint64_t>& GET_CPMS_CL2_PFETCNTL_ACS_MINMAX_SEL(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return i_data.extractToRight<CPMS_CL2_PFETCNTL_ACS_MINMAX_SEL,
           CPMS_CL2_PFETCNTL_ACS_MINMAX_SEL_LEN>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("4:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_ACS_MINMAX_SEL chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_CL2_PFETCNTL_ACS_MINMAX_SEL]

//>> SET_[CPMS_CL2_PFETCNTL_ACS_ENABLE]
static inline fapi2::buffer<uint64_t>& SET_CPMS_CL2_PFETCNTL_ACS_ENABLE(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return o_data.insertFromRight<CPMS_CL2_PFETCNTL_ACS_ENABLE,
           1>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("0:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_ACS_ENABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CL2_PFETCNTL_ACS_ENABLE]

//>> SET_[CPMS_CL2_PFETCNTL_ACS_ENABLE]
static inline fapi2::buffer<uint64_t>& SET_CPMS_CL2_PFETCNTL_ACS_ENABLE(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return o_data.insertFromRight<CPMS_CL2_PFETCNTL_ACS_ENABLE,
           1>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("1:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_ACS_ENABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CL2_PFETCNTL_ACS_ENABLE]

//>> SET_[CPMS_CL2_PFETCNTL_ACS_ENABLE]
static inline fapi2::buffer<uint64_t>& SET_CPMS_CL2_PFETCNTL_ACS_ENABLE(fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return o_data.setBit<CPMS_CL2_PFETCNTL_ACS_ENABLE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("2:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_ACS_ENABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CL2_PFETCNTL_ACS_ENABLE]

//>> CLEAR_[CPMS_CL2_PFETCNTL_ACS_ENABLE]
static inline fapi2::buffer<uint64_t>& CLEAR_CPMS_CL2_PFETCNTL_ACS_ENABLE(fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return o_data.clearBit<CPMS_CL2_PFETCNTL_ACS_ENABLE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("3:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_ACS_ENABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< CLEAR_[CPMS_CL2_PFETCNTL_ACS_ENABLE]

//>> GET_[CPMS_CL2_PFETCNTL_ACS_ENABLE]
static inline fapi2::buffer<uint64_t>& GET_CPMS_CL2_PFETCNTL_ACS_ENABLE(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return i_data.extractToRight<CPMS_CL2_PFETCNTL_ACS_ENABLE,
           1>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("4:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_ACS_ENABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_CL2_PFETCNTL_ACS_ENABLE]

//>> GET_[CPMS_CL2_PFETCNTL_ACS_ENABLE]
static inline bool GET_CPMS_CL2_PFETCNTL_ACS_ENABLE(fapi2::buffer<uint64_t>& i_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return i_data.getBit<CPMS_CL2_PFETCNTL_ACS_ENABLE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("5:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_ACS_ENABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_CL2_PFETCNTL_ACS_ENABLE]

//>> SET_[CPMS_CL2_PFETCNTL_SRAM_ENABLE]
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
    FAPI_ERR("0:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_SRAM_ENABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CL2_PFETCNTL_SRAM_ENABLE]

//>> SET_[CPMS_CL2_PFETCNTL_SRAM_ENABLE]
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
    FAPI_ERR("1:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_SRAM_ENABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CL2_PFETCNTL_SRAM_ENABLE]

//>> SET_[CPMS_CL2_PFETCNTL_SRAM_ENABLE]
static inline fapi2::buffer<uint64_t>& SET_CPMS_CL2_PFETCNTL_SRAM_ENABLE(fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return o_data.setBit<CPMS_CL2_PFETCNTL_SRAM_ENABLE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("2:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_SRAM_ENABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CL2_PFETCNTL_SRAM_ENABLE]

//>> CLEAR_[CPMS_CL2_PFETCNTL_SRAM_ENABLE]
static inline fapi2::buffer<uint64_t>& CLEAR_CPMS_CL2_PFETCNTL_SRAM_ENABLE(fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return o_data.clearBit<CPMS_CL2_PFETCNTL_SRAM_ENABLE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("3:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_SRAM_ENABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< CLEAR_[CPMS_CL2_PFETCNTL_SRAM_ENABLE]

//>> GET_[CPMS_CL2_PFETCNTL_SRAM_ENABLE]
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
    FAPI_ERR("4:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_SRAM_ENABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_CL2_PFETCNTL_SRAM_ENABLE]

//>> GET_[CPMS_CL2_PFETCNTL_SRAM_ENABLE]
static inline bool GET_CPMS_CL2_PFETCNTL_SRAM_ENABLE(fapi2::buffer<uint64_t>& i_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CL2_PFETCNTL, CPMS_CL2_PFETCNTL_WO_CLEAR, CPMS_CL2_PFETCNTL_WO_OR));
#endif
    return i_data.getBit<CPMS_CL2_PFETCNTL_SRAM_ENABLE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("5:ppe_c Invalid dial access CPMS_CL2_PFETCNTL_SRAM_ENABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_CL2_PFETCNTL_SRAM_ENABLE]

//>> PREP_[CPMS_FDCR]
template< fapi2::TargetType K, fapi2::MulticastType M, typename V >
static inline fapi2::ReturnCode PREP_CPMS_FDCR(const fapi2::Target<K, M, V>& i_target)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::init(i_target));
    scomt::last_scom = CPMS_FDCR;
#endif
    return fapi2::current_err;
#ifdef SCOM_CHECKING
fapi_try_exit:
    return fapi2::current_err;
#endif
}
//<< PREP_[CPMS_FDCR]

//>> GET_[CPMS_FDCR]
template< fapi2::TargetType K, fapi2::MulticastType M, typename V >
static inline fapi2::ReturnCode GET_CPMS_FDCR(const fapi2::Target<K, M, V>& i_target, fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::init(i_target));
    scomt::last_scom = CPMS_FDCR;
#endif
    return fapi2::getScom(i_target, CPMS_FDCR, o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    return fapi2::current_err;
#endif
}
//<< GET_[CPMS_FDCR]

//>> PUT_[CPMS_FDCR]
template< fapi2::TargetType K, fapi2::MulticastType M, typename V >
static inline fapi2::ReturnCode PUT_CPMS_FDCR(const fapi2::Target<K, M, V>& i_target, const uint64_t i_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::init(i_target));
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return fapi2::putScom(i_target, CPMS_FDCR, i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    return fapi2::current_err;
#endif
}
//<< PUT_[CPMS_FDCR]

//>> PREP_[CPMS_FDCR_WO_CLEAR]
template< fapi2::TargetType K, fapi2::MulticastType M, typename V >
static inline fapi2::ReturnCode PREP_CPMS_FDCR_WO_CLEAR(const fapi2::Target<K, M, V>& i_target)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::init(i_target));
    scomt::last_scom = CPMS_FDCR_WO_CLEAR;
#endif
    return fapi2::current_err;
#ifdef SCOM_CHECKING
fapi_try_exit:
    return fapi2::current_err;
#endif
}
//<< PREP_[CPMS_FDCR_WO_CLEAR]

//>> GET_[CPMS_FDCR_WO_CLEAR]
template< fapi2::TargetType K, fapi2::MulticastType M, typename V >
static inline fapi2::ReturnCode GET_CPMS_FDCR_WO_CLEAR(const fapi2::Target<K, M, V>& i_target,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::init(i_target));
    scomt::last_scom = CPMS_FDCR_WO_CLEAR;
#endif
    return fapi2::getScom(i_target, CPMS_FDCR_WO_CLEAR, o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    return fapi2::current_err;
#endif
}
//<< GET_[CPMS_FDCR_WO_CLEAR]

//>> PUT_[CPMS_FDCR_WO_CLEAR]
template< fapi2::TargetType K, fapi2::MulticastType M, typename V >
static inline fapi2::ReturnCode PUT_CPMS_FDCR_WO_CLEAR(const fapi2::Target<K, M, V>& i_target, const uint64_t i_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::init(i_target));
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return fapi2::putScom(i_target, CPMS_FDCR_WO_CLEAR, i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    return fapi2::current_err;
#endif
}
//<< PUT_[CPMS_FDCR_WO_CLEAR]


//>> SET_[CPMS_FDCR_DISABLE]
static inline fapi2::buffer<uint64_t>& SET_CPMS_FDCR_DISABLE(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return o_data.insertFromRight<CPMS_FDCR_DISABLE,
           1>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("0:ppe_c Invalid dial access CPMS_FDCR_DISABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_FDCR_DISABLE]

//>> SET_[CPMS_FDCR_DISABLE]
static inline fapi2::buffer<uint64_t>& SET_CPMS_FDCR_DISABLE(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return o_data.insertFromRight<CPMS_FDCR_DISABLE,
           1>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("1:ppe_c Invalid dial access CPMS_FDCR_DISABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_FDCR_DISABLE]

//>> SET_[CPMS_FDCR_DISABLE]
static inline fapi2::buffer<uint64_t>& SET_CPMS_FDCR_DISABLE(fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return o_data.setBit<CPMS_FDCR_DISABLE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("2:ppe_c Invalid dial access CPMS_FDCR_DISABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_FDCR_DISABLE]

//>> CLEAR_[CPMS_FDCR_DISABLE]
static inline fapi2::buffer<uint64_t>& CLEAR_CPMS_FDCR_DISABLE(fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return o_data.clearBit<CPMS_FDCR_DISABLE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("3:ppe_c Invalid dial access CPMS_FDCR_DISABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< CLEAR_[CPMS_FDCR_DISABLE]

//>> GET_[CPMS_FDCR_DISABLE]
static inline fapi2::buffer<uint64_t>& GET_CPMS_FDCR_DISABLE(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return i_data.extractToRight<CPMS_FDCR_DISABLE,
           1>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("4:ppe_c Invalid dial access CPMS_FDCR_DISABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_FDCR_DISABLE]

//>> GET_[CPMS_FDCR_DISABLE]
static inline bool GET_CPMS_FDCR_DISABLE(fapi2::buffer<uint64_t>& i_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return i_data.getBit<CPMS_FDCR_DISABLE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("5:ppe_c Invalid dial access CPMS_FDCR_DISABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_FDCR_DISABLE]

//>> SET_[CPMS_FDCR_FORCE_SAMPLE]
static inline fapi2::buffer<uint64_t>& SET_CPMS_FDCR_FORCE_SAMPLE(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return o_data.insertFromRight<CPMS_FDCR_FORCE_SAMPLE,
           1>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("0:ppe_c Invalid dial access CPMS_FDCR_FORCE_SAMPLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_FDCR_FORCE_SAMPLE]

//>> SET_[CPMS_FDCR_FORCE_SAMPLE]
static inline fapi2::buffer<uint64_t>& SET_CPMS_FDCR_FORCE_SAMPLE(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return o_data.insertFromRight<CPMS_FDCR_FORCE_SAMPLE,
           1>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("1:ppe_c Invalid dial access CPMS_FDCR_FORCE_SAMPLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_FDCR_FORCE_SAMPLE]

//>> SET_[CPMS_FDCR_FORCE_SAMPLE]
static inline fapi2::buffer<uint64_t>& SET_CPMS_FDCR_FORCE_SAMPLE(fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return o_data.setBit<CPMS_FDCR_FORCE_SAMPLE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("2:ppe_c Invalid dial access CPMS_FDCR_FORCE_SAMPLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_FDCR_FORCE_SAMPLE]

//>> CLEAR_[CPMS_FDCR_FORCE_SAMPLE]
static inline fapi2::buffer<uint64_t>& CLEAR_CPMS_FDCR_FORCE_SAMPLE(fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return o_data.clearBit<CPMS_FDCR_FORCE_SAMPLE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("3:ppe_c Invalid dial access CPMS_FDCR_FORCE_SAMPLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< CLEAR_[CPMS_FDCR_FORCE_SAMPLE]

//>> GET_[CPMS_FDCR_FORCE_SAMPLE]
static inline fapi2::buffer<uint64_t>& GET_CPMS_FDCR_FORCE_SAMPLE(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return i_data.extractToRight<CPMS_FDCR_FORCE_SAMPLE,
           1>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("4:ppe_c Invalid dial access CPMS_FDCR_FORCE_SAMPLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_FDCR_FORCE_SAMPLE]

//>> GET_[CPMS_FDCR_FORCE_SAMPLE]
static inline bool GET_CPMS_FDCR_FORCE_SAMPLE(fapi2::buffer<uint64_t>& i_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return i_data.getBit<CPMS_FDCR_FORCE_SAMPLE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("5:ppe_c Invalid dial access CPMS_FDCR_FORCE_SAMPLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_FDCR_FORCE_SAMPLE]

//>> SET_[CPMS_FDCR_STICKY_CONTROL]
static inline fapi2::buffer<uint64_t>& SET_CPMS_FDCR_STICKY_CONTROL(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return o_data.insertFromRight<CPMS_FDCR_STICKY_CONTROL,
           CPMS_FDCR_STICKY_CONTROL_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("0:ppe_c Invalid dial access CPMS_FDCR_STICKY_CONTROL chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_FDCR_STICKY_CONTROL]

//>> SET_[CPMS_FDCR_STICKY_CONTROL]
static inline fapi2::buffer<uint64_t>& SET_CPMS_FDCR_STICKY_CONTROL(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return o_data.insertFromRight<CPMS_FDCR_STICKY_CONTROL,
           CPMS_FDCR_STICKY_CONTROL_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("1:ppe_c Invalid dial access CPMS_FDCR_STICKY_CONTROL chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_FDCR_STICKY_CONTROL]

//>> GET_[CPMS_FDCR_STICKY_CONTROL]
static inline fapi2::buffer<uint64_t>& GET_CPMS_FDCR_STICKY_CONTROL(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return i_data.extractToRight<CPMS_FDCR_STICKY_CONTROL,
           CPMS_FDCR_STICKY_CONTROL_LEN>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("4:ppe_c Invalid dial access CPMS_FDCR_STICKY_CONTROL chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_FDCR_STICKY_CONTROL]

//>> SET_[CPMS_FDCR_CONTROL_UPDATE_DISABLE]
static inline fapi2::buffer<uint64_t>& SET_CPMS_FDCR_CONTROL_UPDATE_DISABLE(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return o_data.insertFromRight<CPMS_FDCR_CONTROL_UPDATE_DISABLE,
           1>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("0:ppe_c Invalid dial access CPMS_FDCR_CONTROL_UPDATE_DISABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_FDCR_CONTROL_UPDATE_DISABLE]

//>> SET_[CPMS_FDCR_CONTROL_UPDATE_DISABLE]
static inline fapi2::buffer<uint64_t>& SET_CPMS_FDCR_CONTROL_UPDATE_DISABLE(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return o_data.insertFromRight<CPMS_FDCR_CONTROL_UPDATE_DISABLE,
           1>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("1:ppe_c Invalid dial access CPMS_FDCR_CONTROL_UPDATE_DISABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_FDCR_CONTROL_UPDATE_DISABLE]

//>> SET_[CPMS_FDCR_CONTROL_UPDATE_DISABLE]
static inline fapi2::buffer<uint64_t>& SET_CPMS_FDCR_CONTROL_UPDATE_DISABLE(fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return o_data.setBit<CPMS_FDCR_CONTROL_UPDATE_DISABLE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("2:ppe_c Invalid dial access CPMS_FDCR_CONTROL_UPDATE_DISABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_FDCR_CONTROL_UPDATE_DISABLE]

//>> CLEAR_[CPMS_FDCR_CONTROL_UPDATE_DISABLE]
static inline fapi2::buffer<uint64_t>& CLEAR_CPMS_FDCR_CONTROL_UPDATE_DISABLE(fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return o_data.clearBit<CPMS_FDCR_CONTROL_UPDATE_DISABLE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("3:ppe_c Invalid dial access CPMS_FDCR_CONTROL_UPDATE_DISABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< CLEAR_[CPMS_FDCR_CONTROL_UPDATE_DISABLE]

//>> GET_[CPMS_FDCR_CONTROL_UPDATE_DISABLE]
static inline fapi2::buffer<uint64_t>& GET_CPMS_FDCR_CONTROL_UPDATE_DISABLE(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return i_data.extractToRight<CPMS_FDCR_CONTROL_UPDATE_DISABLE,
           1>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("4:ppe_c Invalid dial access CPMS_FDCR_CONTROL_UPDATE_DISABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_FDCR_CONTROL_UPDATE_DISABLE]

//>> GET_[CPMS_FDCR_CONTROL_UPDATE_DISABLE]
static inline bool GET_CPMS_FDCR_CONTROL_UPDATE_DISABLE(fapi2::buffer<uint64_t>& i_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return i_data.getBit<CPMS_FDCR_CONTROL_UPDATE_DISABLE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("5:ppe_c Invalid dial access CPMS_FDCR_CONTROL_UPDATE_DISABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_FDCR_CONTROL_UPDATE_DISABLE]

//>> SET_[CPMS_FDCR_SUPPRESS_FTC_UPDATE]
static inline fapi2::buffer<uint64_t>& SET_CPMS_FDCR_SUPPRESS_FTC_UPDATE(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return o_data.insertFromRight<CPMS_FDCR_SUPPRESS_FTC_UPDATE,
           1>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("0:ppe_c Invalid dial access CPMS_FDCR_SUPPRESS_FTC_UPDATE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_FDCR_SUPPRESS_FTC_UPDATE]

//>> SET_[CPMS_FDCR_SUPPRESS_FTC_UPDATE]
static inline fapi2::buffer<uint64_t>& SET_CPMS_FDCR_SUPPRESS_FTC_UPDATE(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return o_data.insertFromRight<CPMS_FDCR_SUPPRESS_FTC_UPDATE,
           1>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("1:ppe_c Invalid dial access CPMS_FDCR_SUPPRESS_FTC_UPDATE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_FDCR_SUPPRESS_FTC_UPDATE]

//>> SET_[CPMS_FDCR_SUPPRESS_FTC_UPDATE]
static inline fapi2::buffer<uint64_t>& SET_CPMS_FDCR_SUPPRESS_FTC_UPDATE(fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return o_data.setBit<CPMS_FDCR_SUPPRESS_FTC_UPDATE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("2:ppe_c Invalid dial access CPMS_FDCR_SUPPRESS_FTC_UPDATE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_FDCR_SUPPRESS_FTC_UPDATE]

//>> CLEAR_[CPMS_FDCR_SUPPRESS_FTC_UPDATE]
static inline fapi2::buffer<uint64_t>& CLEAR_CPMS_FDCR_SUPPRESS_FTC_UPDATE(fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return o_data.clearBit<CPMS_FDCR_SUPPRESS_FTC_UPDATE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("3:ppe_c Invalid dial access CPMS_FDCR_SUPPRESS_FTC_UPDATE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< CLEAR_[CPMS_FDCR_SUPPRESS_FTC_UPDATE]

//>> GET_[CPMS_FDCR_SUPPRESS_FTC_UPDATE]
static inline fapi2::buffer<uint64_t>& GET_CPMS_FDCR_SUPPRESS_FTC_UPDATE(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return i_data.extractToRight<CPMS_FDCR_SUPPRESS_FTC_UPDATE,
           1>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("4:ppe_c Invalid dial access CPMS_FDCR_SUPPRESS_FTC_UPDATE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_FDCR_SUPPRESS_FTC_UPDATE]

//>> GET_[CPMS_FDCR_SUPPRESS_FTC_UPDATE]
static inline bool GET_CPMS_FDCR_SUPPRESS_FTC_UPDATE(fapi2::buffer<uint64_t>& i_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return i_data.getBit<CPMS_FDCR_SUPPRESS_FTC_UPDATE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("5:ppe_c Invalid dial access CPMS_FDCR_SUPPRESS_FTC_UPDATE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_FDCR_SUPPRESS_FTC_UPDATE]

//>> SET_[CPMS_FDCR_CAL_ADJUST]
static inline fapi2::buffer<uint64_t>& SET_CPMS_FDCR_CAL_ADJUST(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return o_data.insertFromRight<CPMS_FDCR_CAL_ADJUST,
           CPMS_FDCR_CAL_ADJUST_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("0:ppe_c Invalid dial access CPMS_FDCR_CAL_ADJUST chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_FDCR_CAL_ADJUST]

//>> SET_[CPMS_FDCR_CAL_ADJUST]
static inline fapi2::buffer<uint64_t>& SET_CPMS_FDCR_CAL_ADJUST(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return o_data.insertFromRight<CPMS_FDCR_CAL_ADJUST,
           CPMS_FDCR_CAL_ADJUST_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("1:ppe_c Invalid dial access CPMS_FDCR_CAL_ADJUST chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_FDCR_CAL_ADJUST]

//>> GET_[CPMS_FDCR_CAL_ADJUST]
static inline fapi2::buffer<uint64_t>& GET_CPMS_FDCR_CAL_ADJUST(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return i_data.extractToRight<CPMS_FDCR_CAL_ADJUST,
           CPMS_FDCR_CAL_ADJUST_LEN>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("4:ppe_c Invalid dial access CPMS_FDCR_CAL_ADJUST chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_FDCR_CAL_ADJUST]

//>> SET_[CPMS_FDCR_DELAY]
static inline fapi2::buffer<uint64_t>& SET_CPMS_FDCR_DELAY(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return o_data.insertFromRight<CPMS_FDCR_DELAY,
           CPMS_FDCR_DELAY_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("0:ppe_c Invalid dial access CPMS_FDCR_DELAY chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_FDCR_DELAY]

//>> SET_[CPMS_FDCR_DELAY]
static inline fapi2::buffer<uint64_t>& SET_CPMS_FDCR_DELAY(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return o_data.insertFromRight<CPMS_FDCR_DELAY,
           CPMS_FDCR_DELAY_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("1:ppe_c Invalid dial access CPMS_FDCR_DELAY chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_FDCR_DELAY]

//>> GET_[CPMS_FDCR_DELAY]
static inline fapi2::buffer<uint64_t>& GET_CPMS_FDCR_DELAY(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return i_data.extractToRight<CPMS_FDCR_DELAY,
           CPMS_FDCR_DELAY_LEN>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("4:ppe_c Invalid dial access CPMS_FDCR_DELAY chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_FDCR_DELAY]

//>> SET_[CPMS_FDCR_THRESHOLD_UPDATE_DISABLE]
static inline fapi2::buffer<uint64_t>& SET_CPMS_FDCR_THRESHOLD_UPDATE_DISABLE(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return o_data.insertFromRight<CPMS_FDCR_THRESHOLD_UPDATE_DISABLE,
           1>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("0:ppe_c Invalid dial access CPMS_FDCR_THRESHOLD_UPDATE_DISABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_FDCR_THRESHOLD_UPDATE_DISABLE]

//>> SET_[CPMS_FDCR_THRESHOLD_UPDATE_DISABLE]
static inline fapi2::buffer<uint64_t>& SET_CPMS_FDCR_THRESHOLD_UPDATE_DISABLE(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return o_data.insertFromRight<CPMS_FDCR_THRESHOLD_UPDATE_DISABLE,
           1>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("1:ppe_c Invalid dial access CPMS_FDCR_THRESHOLD_UPDATE_DISABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_FDCR_THRESHOLD_UPDATE_DISABLE]

//>> SET_[CPMS_FDCR_THRESHOLD_UPDATE_DISABLE]
static inline fapi2::buffer<uint64_t>& SET_CPMS_FDCR_THRESHOLD_UPDATE_DISABLE(fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return o_data.setBit<CPMS_FDCR_THRESHOLD_UPDATE_DISABLE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("2:ppe_c Invalid dial access CPMS_FDCR_THRESHOLD_UPDATE_DISABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_FDCR_THRESHOLD_UPDATE_DISABLE]

//>> CLEAR_[CPMS_FDCR_THRESHOLD_UPDATE_DISABLE]
static inline fapi2::buffer<uint64_t>& CLEAR_CPMS_FDCR_THRESHOLD_UPDATE_DISABLE(fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return o_data.clearBit<CPMS_FDCR_THRESHOLD_UPDATE_DISABLE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("3:ppe_c Invalid dial access CPMS_FDCR_THRESHOLD_UPDATE_DISABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< CLEAR_[CPMS_FDCR_THRESHOLD_UPDATE_DISABLE]

//>> GET_[CPMS_FDCR_THRESHOLD_UPDATE_DISABLE]
static inline fapi2::buffer<uint64_t>& GET_CPMS_FDCR_THRESHOLD_UPDATE_DISABLE(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return i_data.extractToRight<CPMS_FDCR_THRESHOLD_UPDATE_DISABLE,
           1>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("4:ppe_c Invalid dial access CPMS_FDCR_THRESHOLD_UPDATE_DISABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_FDCR_THRESHOLD_UPDATE_DISABLE]

//>> GET_[CPMS_FDCR_THRESHOLD_UPDATE_DISABLE]
static inline bool GET_CPMS_FDCR_THRESHOLD_UPDATE_DISABLE(fapi2::buffer<uint64_t>& i_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return i_data.getBit<CPMS_FDCR_THRESHOLD_UPDATE_DISABLE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("5:ppe_c Invalid dial access CPMS_FDCR_THRESHOLD_UPDATE_DISABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_FDCR_THRESHOLD_UPDATE_DISABLE]

//>> SET_[CPMS_FDCR_TRIP_OFFSET]
static inline fapi2::buffer<uint64_t>& SET_CPMS_FDCR_TRIP_OFFSET(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return o_data.insertFromRight<CPMS_FDCR_TRIP_OFFSET,
           CPMS_FDCR_TRIP_OFFSET_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("0:ppe_c Invalid dial access CPMS_FDCR_TRIP_OFFSET chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_FDCR_TRIP_OFFSET]

//>> SET_[CPMS_FDCR_TRIP_OFFSET]
static inline fapi2::buffer<uint64_t>& SET_CPMS_FDCR_TRIP_OFFSET(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return o_data.insertFromRight<CPMS_FDCR_TRIP_OFFSET,
           CPMS_FDCR_TRIP_OFFSET_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("1:ppe_c Invalid dial access CPMS_FDCR_TRIP_OFFSET chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_FDCR_TRIP_OFFSET]

//>> GET_[CPMS_FDCR_TRIP_OFFSET]
static inline fapi2::buffer<uint64_t>& GET_CPMS_FDCR_TRIP_OFFSET(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return i_data.extractToRight<CPMS_FDCR_TRIP_OFFSET,
           CPMS_FDCR_TRIP_OFFSET_LEN>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("4:ppe_c Invalid dial access CPMS_FDCR_TRIP_OFFSET chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_FDCR_TRIP_OFFSET]

//>> SET_[CPMS_FDCR_DATA0_DETECT]
static inline fapi2::buffer<uint64_t>& SET_CPMS_FDCR_DATA0_DETECT(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return o_data.insertFromRight<CPMS_FDCR_DATA0_DETECT,
           CPMS_FDCR_DATA0_DETECT_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("0:ppe_c Invalid dial access CPMS_FDCR_DATA0_DETECT chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_FDCR_DATA0_DETECT]

//>> SET_[CPMS_FDCR_DATA0_DETECT]
static inline fapi2::buffer<uint64_t>& SET_CPMS_FDCR_DATA0_DETECT(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return o_data.insertFromRight<CPMS_FDCR_DATA0_DETECT,
           CPMS_FDCR_DATA0_DETECT_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("1:ppe_c Invalid dial access CPMS_FDCR_DATA0_DETECT chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_FDCR_DATA0_DETECT]

//>> GET_[CPMS_FDCR_DATA0_DETECT]
static inline fapi2::buffer<uint64_t>& GET_CPMS_FDCR_DATA0_DETECT(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return i_data.extractToRight<CPMS_FDCR_DATA0_DETECT,
           CPMS_FDCR_DATA0_DETECT_LEN>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("4:ppe_c Invalid dial access CPMS_FDCR_DATA0_DETECT chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_FDCR_DATA0_DETECT]

//>> SET_[CPMS_FDCR_DATA1_DETECT]
static inline fapi2::buffer<uint64_t>& SET_CPMS_FDCR_DATA1_DETECT(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return o_data.insertFromRight<CPMS_FDCR_DATA1_DETECT,
           CPMS_FDCR_DATA1_DETECT_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("0:ppe_c Invalid dial access CPMS_FDCR_DATA1_DETECT chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_FDCR_DATA1_DETECT]

//>> SET_[CPMS_FDCR_DATA1_DETECT]
static inline fapi2::buffer<uint64_t>& SET_CPMS_FDCR_DATA1_DETECT(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return o_data.insertFromRight<CPMS_FDCR_DATA1_DETECT,
           CPMS_FDCR_DATA1_DETECT_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("1:ppe_c Invalid dial access CPMS_FDCR_DATA1_DETECT chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_FDCR_DATA1_DETECT]

//>> GET_[CPMS_FDCR_DATA1_DETECT]
static inline fapi2::buffer<uint64_t>& GET_CPMS_FDCR_DATA1_DETECT(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return i_data.extractToRight<CPMS_FDCR_DATA1_DETECT,
           CPMS_FDCR_DATA1_DETECT_LEN>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("4:ppe_c Invalid dial access CPMS_FDCR_DATA1_DETECT chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_FDCR_DATA1_DETECT]

//>> SET_[CPMS_FDCR_DATA2_DETECT]
static inline fapi2::buffer<uint64_t>& SET_CPMS_FDCR_DATA2_DETECT(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return o_data.insertFromRight<CPMS_FDCR_DATA2_DETECT,
           CPMS_FDCR_DATA2_DETECT_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("0:ppe_c Invalid dial access CPMS_FDCR_DATA2_DETECT chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_FDCR_DATA2_DETECT]

//>> SET_[CPMS_FDCR_DATA2_DETECT]
static inline fapi2::buffer<uint64_t>& SET_CPMS_FDCR_DATA2_DETECT(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return o_data.insertFromRight<CPMS_FDCR_DATA2_DETECT,
           CPMS_FDCR_DATA2_DETECT_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("1:ppe_c Invalid dial access CPMS_FDCR_DATA2_DETECT chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_FDCR_DATA2_DETECT]

//>> GET_[CPMS_FDCR_DATA2_DETECT]
static inline fapi2::buffer<uint64_t>& GET_CPMS_FDCR_DATA2_DETECT(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return i_data.extractToRight<CPMS_FDCR_DATA2_DETECT,
           CPMS_FDCR_DATA2_DETECT_LEN>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("4:ppe_c Invalid dial access CPMS_FDCR_DATA2_DETECT chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_FDCR_DATA2_DETECT]

//>> SET_[CPMS_FDCR_LARGE_DROOP_DETECT]
static inline fapi2::buffer<uint64_t>& SET_CPMS_FDCR_LARGE_DROOP_DETECT(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return o_data.insertFromRight<CPMS_FDCR_LARGE_DROOP_DETECT,
           CPMS_FDCR_LARGE_DROOP_DETECT_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("0:ppe_c Invalid dial access CPMS_FDCR_LARGE_DROOP_DETECT chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_FDCR_LARGE_DROOP_DETECT]

//>> SET_[CPMS_FDCR_LARGE_DROOP_DETECT]
static inline fapi2::buffer<uint64_t>& SET_CPMS_FDCR_LARGE_DROOP_DETECT(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return o_data.insertFromRight<CPMS_FDCR_LARGE_DROOP_DETECT,
           CPMS_FDCR_LARGE_DROOP_DETECT_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("1:ppe_c Invalid dial access CPMS_FDCR_LARGE_DROOP_DETECT chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_FDCR_LARGE_DROOP_DETECT]

//>> GET_[CPMS_FDCR_LARGE_DROOP_DETECT]
static inline fapi2::buffer<uint64_t>& GET_CPMS_FDCR_LARGE_DROOP_DETECT(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return i_data.extractToRight<CPMS_FDCR_LARGE_DROOP_DETECT,
           CPMS_FDCR_LARGE_DROOP_DETECT_LEN>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("4:ppe_c Invalid dial access CPMS_FDCR_LARGE_DROOP_DETECT chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_FDCR_LARGE_DROOP_DETECT]

//>> SET_[CPMS_FDCR_SMALL_DROOP_DETECT]
static inline fapi2::buffer<uint64_t>& SET_CPMS_FDCR_SMALL_DROOP_DETECT(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return o_data.insertFromRight<CPMS_FDCR_SMALL_DROOP_DETECT,
           CPMS_FDCR_SMALL_DROOP_DETECT_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("0:ppe_c Invalid dial access CPMS_FDCR_SMALL_DROOP_DETECT chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_FDCR_SMALL_DROOP_DETECT]

//>> SET_[CPMS_FDCR_SMALL_DROOP_DETECT]
static inline fapi2::buffer<uint64_t>& SET_CPMS_FDCR_SMALL_DROOP_DETECT(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return o_data.insertFromRight<CPMS_FDCR_SMALL_DROOP_DETECT,
           CPMS_FDCR_SMALL_DROOP_DETECT_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("1:ppe_c Invalid dial access CPMS_FDCR_SMALL_DROOP_DETECT chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_FDCR_SMALL_DROOP_DETECT]

//>> GET_[CPMS_FDCR_SMALL_DROOP_DETECT]
static inline fapi2::buffer<uint64_t>& GET_CPMS_FDCR_SMALL_DROOP_DETECT(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return i_data.extractToRight<CPMS_FDCR_SMALL_DROOP_DETECT,
           CPMS_FDCR_SMALL_DROOP_DETECT_LEN>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("4:ppe_c Invalid dial access CPMS_FDCR_SMALL_DROOP_DETECT chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_FDCR_SMALL_DROOP_DETECT]

//>> SET_[CPMS_FDCR_SLOPEA_START_DETECT]
static inline fapi2::buffer<uint64_t>& SET_CPMS_FDCR_SLOPEA_START_DETECT(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return o_data.insertFromRight<CPMS_FDCR_SLOPEA_START_DETECT,
           CPMS_FDCR_SLOPEA_START_DETECT_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("0:ppe_c Invalid dial access CPMS_FDCR_SLOPEA_START_DETECT chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_FDCR_SLOPEA_START_DETECT]

//>> SET_[CPMS_FDCR_SLOPEA_START_DETECT]
static inline fapi2::buffer<uint64_t>& SET_CPMS_FDCR_SLOPEA_START_DETECT(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return o_data.insertFromRight<CPMS_FDCR_SLOPEA_START_DETECT,
           CPMS_FDCR_SLOPEA_START_DETECT_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("1:ppe_c Invalid dial access CPMS_FDCR_SLOPEA_START_DETECT chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_FDCR_SLOPEA_START_DETECT]

//>> GET_[CPMS_FDCR_SLOPEA_START_DETECT]
static inline fapi2::buffer<uint64_t>& GET_CPMS_FDCR_SLOPEA_START_DETECT(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return i_data.extractToRight<CPMS_FDCR_SLOPEA_START_DETECT,
           CPMS_FDCR_SLOPEA_START_DETECT_LEN>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("4:ppe_c Invalid dial access CPMS_FDCR_SLOPEA_START_DETECT chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_FDCR_SLOPEA_START_DETECT]

//>> SET_[CPMS_FDCR_SLOPEA_END_DETECT]
static inline fapi2::buffer<uint64_t>& SET_CPMS_FDCR_SLOPEA_END_DETECT(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return o_data.insertFromRight<CPMS_FDCR_SLOPEA_END_DETECT,
           CPMS_FDCR_SLOPEA_END_DETECT_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("0:ppe_c Invalid dial access CPMS_FDCR_SLOPEA_END_DETECT chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_FDCR_SLOPEA_END_DETECT]

//>> SET_[CPMS_FDCR_SLOPEA_END_DETECT]
static inline fapi2::buffer<uint64_t>& SET_CPMS_FDCR_SLOPEA_END_DETECT(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return o_data.insertFromRight<CPMS_FDCR_SLOPEA_END_DETECT,
           CPMS_FDCR_SLOPEA_END_DETECT_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("1:ppe_c Invalid dial access CPMS_FDCR_SLOPEA_END_DETECT chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_FDCR_SLOPEA_END_DETECT]

//>> GET_[CPMS_FDCR_SLOPEA_END_DETECT]
static inline fapi2::buffer<uint64_t>& GET_CPMS_FDCR_SLOPEA_END_DETECT(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return i_data.extractToRight<CPMS_FDCR_SLOPEA_END_DETECT,
           CPMS_FDCR_SLOPEA_END_DETECT_LEN>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("4:ppe_c Invalid dial access CPMS_FDCR_SLOPEA_END_DETECT chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_FDCR_SLOPEA_END_DETECT]

//>> SET_[CPMS_FDCR_SLOPEB_START_DETECT]
static inline fapi2::buffer<uint64_t>& SET_CPMS_FDCR_SLOPEB_START_DETECT(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return o_data.insertFromRight<CPMS_FDCR_SLOPEB_START_DETECT,
           CPMS_FDCR_SLOPEB_START_DETECT_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("0:ppe_c Invalid dial access CPMS_FDCR_SLOPEB_START_DETECT chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_FDCR_SLOPEB_START_DETECT]

//>> SET_[CPMS_FDCR_SLOPEB_START_DETECT]
static inline fapi2::buffer<uint64_t>& SET_CPMS_FDCR_SLOPEB_START_DETECT(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return o_data.insertFromRight<CPMS_FDCR_SLOPEB_START_DETECT,
           CPMS_FDCR_SLOPEB_START_DETECT_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("1:ppe_c Invalid dial access CPMS_FDCR_SLOPEB_START_DETECT chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_FDCR_SLOPEB_START_DETECT]

//>> GET_[CPMS_FDCR_SLOPEB_START_DETECT]
static inline fapi2::buffer<uint64_t>& GET_CPMS_FDCR_SLOPEB_START_DETECT(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return i_data.extractToRight<CPMS_FDCR_SLOPEB_START_DETECT,
           CPMS_FDCR_SLOPEB_START_DETECT_LEN>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("4:ppe_c Invalid dial access CPMS_FDCR_SLOPEB_START_DETECT chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_FDCR_SLOPEB_START_DETECT]

//>> SET_[CPMS_FDCR_SLOPEB_END_DETECT]
static inline fapi2::buffer<uint64_t>& SET_CPMS_FDCR_SLOPEB_END_DETECT(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return o_data.insertFromRight<CPMS_FDCR_SLOPEB_END_DETECT,
           CPMS_FDCR_SLOPEB_END_DETECT_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("0:ppe_c Invalid dial access CPMS_FDCR_SLOPEB_END_DETECT chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_FDCR_SLOPEB_END_DETECT]

//>> SET_[CPMS_FDCR_SLOPEB_END_DETECT]
static inline fapi2::buffer<uint64_t>& SET_CPMS_FDCR_SLOPEB_END_DETECT(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return o_data.insertFromRight<CPMS_FDCR_SLOPEB_END_DETECT,
           CPMS_FDCR_SLOPEB_END_DETECT_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("1:ppe_c Invalid dial access CPMS_FDCR_SLOPEB_END_DETECT chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_FDCR_SLOPEB_END_DETECT]

//>> GET_[CPMS_FDCR_SLOPEB_END_DETECT]
static inline fapi2::buffer<uint64_t>& GET_CPMS_FDCR_SLOPEB_END_DETECT(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return i_data.extractToRight<CPMS_FDCR_SLOPEB_END_DETECT,
           CPMS_FDCR_SLOPEB_END_DETECT_LEN>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("4:ppe_c Invalid dial access CPMS_FDCR_SLOPEB_END_DETECT chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_FDCR_SLOPEB_END_DETECT]

//>> SET_[CPMS_FDCR_SLOPEA_CYCLES]
static inline fapi2::buffer<uint64_t>& SET_CPMS_FDCR_SLOPEA_CYCLES(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return o_data.insertFromRight<CPMS_FDCR_SLOPEA_CYCLES,
           CPMS_FDCR_SLOPEA_CYCLES_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("0:ppe_c Invalid dial access CPMS_FDCR_SLOPEA_CYCLES chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_FDCR_SLOPEA_CYCLES]

//>> SET_[CPMS_FDCR_SLOPEA_CYCLES]
static inline fapi2::buffer<uint64_t>& SET_CPMS_FDCR_SLOPEA_CYCLES(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return o_data.insertFromRight<CPMS_FDCR_SLOPEA_CYCLES,
           CPMS_FDCR_SLOPEA_CYCLES_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("1:ppe_c Invalid dial access CPMS_FDCR_SLOPEA_CYCLES chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_FDCR_SLOPEA_CYCLES]

//>> GET_[CPMS_FDCR_SLOPEA_CYCLES]
static inline fapi2::buffer<uint64_t>& GET_CPMS_FDCR_SLOPEA_CYCLES(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return i_data.extractToRight<CPMS_FDCR_SLOPEA_CYCLES,
           CPMS_FDCR_SLOPEA_CYCLES_LEN>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("4:ppe_c Invalid dial access CPMS_FDCR_SLOPEA_CYCLES chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_FDCR_SLOPEA_CYCLES]

//>> SET_[CPMS_FDCR_SLOPEB_CYCLES]
static inline fapi2::buffer<uint64_t>& SET_CPMS_FDCR_SLOPEB_CYCLES(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return o_data.insertFromRight<CPMS_FDCR_SLOPEB_CYCLES,
           CPMS_FDCR_SLOPEB_CYCLES_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("0:ppe_c Invalid dial access CPMS_FDCR_SLOPEB_CYCLES chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_FDCR_SLOPEB_CYCLES]

//>> SET_[CPMS_FDCR_SLOPEB_CYCLES]
static inline fapi2::buffer<uint64_t>& SET_CPMS_FDCR_SLOPEB_CYCLES(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return o_data.insertFromRight<CPMS_FDCR_SLOPEB_CYCLES,
           CPMS_FDCR_SLOPEB_CYCLES_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("1:ppe_c Invalid dial access CPMS_FDCR_SLOPEB_CYCLES chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_FDCR_SLOPEB_CYCLES]

//>> GET_[CPMS_FDCR_SLOPEB_CYCLES]
static inline fapi2::buffer<uint64_t>& GET_CPMS_FDCR_SLOPEB_CYCLES(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return i_data.extractToRight<CPMS_FDCR_SLOPEB_CYCLES,
           CPMS_FDCR_SLOPEB_CYCLES_LEN>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("4:ppe_c Invalid dial access CPMS_FDCR_SLOPEB_CYCLES chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_FDCR_SLOPEB_CYCLES]

//>> PREP_[CPMS_FDCR_PPE2]
template< fapi2::TargetType K, fapi2::MulticastType M, typename V >
static inline fapi2::ReturnCode PREP_CPMS_FDCR_PPE2(const fapi2::Target<K, M, V>& i_target)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::init(i_target));
    scomt::last_scom = CPMS_FDCR_PPE2;
#endif
    return fapi2::current_err;
#ifdef SCOM_CHECKING
fapi_try_exit:
    return fapi2::current_err;
#endif
}
//<< PREP_[CPMS_FDCR_PPE2]

//>> GET_[CPMS_FDCR_PPE2]
template< fapi2::TargetType K, fapi2::MulticastType M, typename V >
static inline fapi2::ReturnCode GET_CPMS_FDCR_PPE2(const fapi2::Target<K, M, V>& i_target,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::init(i_target));
    scomt::last_scom = CPMS_FDCR_PPE2;
#endif
    return fapi2::getScom(i_target, CPMS_FDCR_PPE2, o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    return fapi2::current_err;
#endif
}
//<< GET_[CPMS_FDCR_PPE2]

//>> PUT_[CPMS_FDCR_PPE2]
template< fapi2::TargetType K, fapi2::MulticastType M, typename V >
static inline fapi2::ReturnCode PUT_CPMS_FDCR_PPE2(const fapi2::Target<K, M, V>& i_target, const uint64_t i_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::init(i_target));
    FAPI_TRY(scomt::chkReg(CPMS_FDCR, CPMS_FDCR_PPE2, CPMS_FDCR_WO_CLEAR));
#endif
    return fapi2::putScom(i_target, CPMS_FDCR_PPE2, i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    return fapi2::current_err;
#endif
}
//<< PUT_[CPMS_FDCR_PPE2]

//>> PREP_[CPMS_CCDR]
template< fapi2::TargetType K, fapi2::MulticastType M, typename V >
static inline fapi2::ReturnCode PREP_CPMS_CCDR(const fapi2::Target<K, M, V>& i_target)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::init(i_target));
    scomt::last_scom = CPMS_CCDR;
#endif
    return fapi2::current_err;
#ifdef SCOM_CHECKING
fapi_try_exit:
    return fapi2::current_err;
#endif
}
//<< PREP_[CPMS_CCDR]

//>> GET_[CPMS_CCDR]
template< fapi2::TargetType K, fapi2::MulticastType M, typename V >
static inline fapi2::ReturnCode GET_CPMS_CCDR(const fapi2::Target<K, M, V>& i_target, fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::init(i_target));
    scomt::last_scom = CPMS_CCDR;
#endif
    return fapi2::getScom(i_target, CPMS_CCDR, o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    return fapi2::current_err;
#endif
}
//<< GET_[CPMS_CCDR]

//>> PUT_[CPMS_CCDR]
template< fapi2::TargetType K, fapi2::MulticastType M, typename V >
static inline fapi2::ReturnCode PUT_CPMS_CCDR(const fapi2::Target<K, M, V>& i_target, const uint64_t i_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::init(i_target));
    FAPI_TRY(scomt::chkReg(CPMS_CCDR));
#endif
    return fapi2::putScom(i_target, CPMS_CCDR, i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    return fapi2::current_err;
#endif
}
//<< PUT_[CPMS_CCDR]


//>> SET_[CPMS_CCDR_CHAR_COUNTA_SCALE]
static inline fapi2::buffer<uint64_t>& SET_CPMS_CCDR_CHAR_COUNTA_SCALE(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CCDR));
#endif
    return o_data.insertFromRight<CPMS_CCDR_CHAR_COUNTA_SCALE,
           CPMS_CCDR_CHAR_COUNTA_SCALE_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("0:ppe_c Invalid dial access CPMS_CCDR_CHAR_COUNTA_SCALE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CCDR_CHAR_COUNTA_SCALE]

//>> SET_[CPMS_CCDR_CHAR_COUNTA_SCALE]
static inline fapi2::buffer<uint64_t>& SET_CPMS_CCDR_CHAR_COUNTA_SCALE(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CCDR));
#endif
    return o_data.insertFromRight<CPMS_CCDR_CHAR_COUNTA_SCALE,
           CPMS_CCDR_CHAR_COUNTA_SCALE_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("1:ppe_c Invalid dial access CPMS_CCDR_CHAR_COUNTA_SCALE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CCDR_CHAR_COUNTA_SCALE]

//>> GET_[CPMS_CCDR_CHAR_COUNTA_SCALE]
static inline fapi2::buffer<uint64_t>& GET_CPMS_CCDR_CHAR_COUNTA_SCALE(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CCDR));
#endif
    return i_data.extractToRight<CPMS_CCDR_CHAR_COUNTA_SCALE,
           CPMS_CCDR_CHAR_COUNTA_SCALE_LEN>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("4:ppe_c Invalid dial access CPMS_CCDR_CHAR_COUNTA_SCALE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_CCDR_CHAR_COUNTA_SCALE]

//>> SET_[CPMS_CCDR_CHAR_COUNTA_VALUE]
static inline fapi2::buffer<uint64_t>& SET_CPMS_CCDR_CHAR_COUNTA_VALUE(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CCDR));
#endif
    return o_data.insertFromRight<CPMS_CCDR_CHAR_COUNTA_VALUE,
           CPMS_CCDR_CHAR_COUNTA_VALUE_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("0:ppe_c Invalid dial access CPMS_CCDR_CHAR_COUNTA_VALUE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CCDR_CHAR_COUNTA_VALUE]

//>> SET_[CPMS_CCDR_CHAR_COUNTA_VALUE]
static inline fapi2::buffer<uint64_t>& SET_CPMS_CCDR_CHAR_COUNTA_VALUE(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CCDR));
#endif
    return o_data.insertFromRight<CPMS_CCDR_CHAR_COUNTA_VALUE,
           CPMS_CCDR_CHAR_COUNTA_VALUE_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("1:ppe_c Invalid dial access CPMS_CCDR_CHAR_COUNTA_VALUE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CCDR_CHAR_COUNTA_VALUE]

//>> GET_[CPMS_CCDR_CHAR_COUNTA_VALUE]
static inline fapi2::buffer<uint64_t>& GET_CPMS_CCDR_CHAR_COUNTA_VALUE(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CCDR));
#endif
    return i_data.extractToRight<CPMS_CCDR_CHAR_COUNTA_VALUE,
           CPMS_CCDR_CHAR_COUNTA_VALUE_LEN>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("4:ppe_c Invalid dial access CPMS_CCDR_CHAR_COUNTA_VALUE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_CCDR_CHAR_COUNTA_VALUE]

//>> SET_[CPMS_CCDR_CHAR_COUNTB_SCALE]
static inline fapi2::buffer<uint64_t>& SET_CPMS_CCDR_CHAR_COUNTB_SCALE(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CCDR));
#endif
    return o_data.insertFromRight<CPMS_CCDR_CHAR_COUNTB_SCALE,
           CPMS_CCDR_CHAR_COUNTB_SCALE_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("0:ppe_c Invalid dial access CPMS_CCDR_CHAR_COUNTB_SCALE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CCDR_CHAR_COUNTB_SCALE]

//>> SET_[CPMS_CCDR_CHAR_COUNTB_SCALE]
static inline fapi2::buffer<uint64_t>& SET_CPMS_CCDR_CHAR_COUNTB_SCALE(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CCDR));
#endif
    return o_data.insertFromRight<CPMS_CCDR_CHAR_COUNTB_SCALE,
           CPMS_CCDR_CHAR_COUNTB_SCALE_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("1:ppe_c Invalid dial access CPMS_CCDR_CHAR_COUNTB_SCALE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CCDR_CHAR_COUNTB_SCALE]

//>> GET_[CPMS_CCDR_CHAR_COUNTB_SCALE]
static inline fapi2::buffer<uint64_t>& GET_CPMS_CCDR_CHAR_COUNTB_SCALE(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CCDR));
#endif
    return i_data.extractToRight<CPMS_CCDR_CHAR_COUNTB_SCALE,
           CPMS_CCDR_CHAR_COUNTB_SCALE_LEN>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("4:ppe_c Invalid dial access CPMS_CCDR_CHAR_COUNTB_SCALE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_CCDR_CHAR_COUNTB_SCALE]

//>> SET_[CPMS_CCDR_CHAR_COUNTB_VALUE]
static inline fapi2::buffer<uint64_t>& SET_CPMS_CCDR_CHAR_COUNTB_VALUE(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CCDR));
#endif
    return o_data.insertFromRight<CPMS_CCDR_CHAR_COUNTB_VALUE,
           CPMS_CCDR_CHAR_COUNTB_VALUE_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("0:ppe_c Invalid dial access CPMS_CCDR_CHAR_COUNTB_VALUE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CCDR_CHAR_COUNTB_VALUE]

//>> SET_[CPMS_CCDR_CHAR_COUNTB_VALUE]
static inline fapi2::buffer<uint64_t>& SET_CPMS_CCDR_CHAR_COUNTB_VALUE(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CCDR));
#endif
    return o_data.insertFromRight<CPMS_CCDR_CHAR_COUNTB_VALUE,
           CPMS_CCDR_CHAR_COUNTB_VALUE_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("1:ppe_c Invalid dial access CPMS_CCDR_CHAR_COUNTB_VALUE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CCDR_CHAR_COUNTB_VALUE]

//>> GET_[CPMS_CCDR_CHAR_COUNTB_VALUE]
static inline fapi2::buffer<uint64_t>& GET_CPMS_CCDR_CHAR_COUNTB_VALUE(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CCDR));
#endif
    return i_data.extractToRight<CPMS_CCDR_CHAR_COUNTB_VALUE,
           CPMS_CCDR_CHAR_COUNTB_VALUE_LEN>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("4:ppe_c Invalid dial access CPMS_CCDR_CHAR_COUNTB_VALUE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_CCDR_CHAR_COUNTB_VALUE]

//>> SET_[CPMS_CCDR_CHAR_COUNTC_SCALE]
static inline fapi2::buffer<uint64_t>& SET_CPMS_CCDR_CHAR_COUNTC_SCALE(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CCDR));
#endif
    return o_data.insertFromRight<CPMS_CCDR_CHAR_COUNTC_SCALE,
           CPMS_CCDR_CHAR_COUNTC_SCALE_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("0:ppe_c Invalid dial access CPMS_CCDR_CHAR_COUNTC_SCALE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CCDR_CHAR_COUNTC_SCALE]

//>> SET_[CPMS_CCDR_CHAR_COUNTC_SCALE]
static inline fapi2::buffer<uint64_t>& SET_CPMS_CCDR_CHAR_COUNTC_SCALE(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CCDR));
#endif
    return o_data.insertFromRight<CPMS_CCDR_CHAR_COUNTC_SCALE,
           CPMS_CCDR_CHAR_COUNTC_SCALE_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("1:ppe_c Invalid dial access CPMS_CCDR_CHAR_COUNTC_SCALE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CCDR_CHAR_COUNTC_SCALE]

//>> GET_[CPMS_CCDR_CHAR_COUNTC_SCALE]
static inline fapi2::buffer<uint64_t>& GET_CPMS_CCDR_CHAR_COUNTC_SCALE(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CCDR));
#endif
    return i_data.extractToRight<CPMS_CCDR_CHAR_COUNTC_SCALE,
           CPMS_CCDR_CHAR_COUNTC_SCALE_LEN>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("4:ppe_c Invalid dial access CPMS_CCDR_CHAR_COUNTC_SCALE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_CCDR_CHAR_COUNTC_SCALE]

//>> SET_[CPMS_CCDR_CHAR_COUNTC_VALUE]
static inline fapi2::buffer<uint64_t>& SET_CPMS_CCDR_CHAR_COUNTC_VALUE(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CCDR));
#endif
    return o_data.insertFromRight<CPMS_CCDR_CHAR_COUNTC_VALUE,
           CPMS_CCDR_CHAR_COUNTC_VALUE_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("0:ppe_c Invalid dial access CPMS_CCDR_CHAR_COUNTC_VALUE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CCDR_CHAR_COUNTC_VALUE]

//>> SET_[CPMS_CCDR_CHAR_COUNTC_VALUE]
static inline fapi2::buffer<uint64_t>& SET_CPMS_CCDR_CHAR_COUNTC_VALUE(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CCDR));
#endif
    return o_data.insertFromRight<CPMS_CCDR_CHAR_COUNTC_VALUE,
           CPMS_CCDR_CHAR_COUNTC_VALUE_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("1:ppe_c Invalid dial access CPMS_CCDR_CHAR_COUNTC_VALUE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CCDR_CHAR_COUNTC_VALUE]

//>> GET_[CPMS_CCDR_CHAR_COUNTC_VALUE]
static inline fapi2::buffer<uint64_t>& GET_CPMS_CCDR_CHAR_COUNTC_VALUE(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CCDR));
#endif
    return i_data.extractToRight<CPMS_CCDR_CHAR_COUNTC_VALUE,
           CPMS_CCDR_CHAR_COUNTC_VALUE_LEN>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("4:ppe_c Invalid dial access CPMS_CCDR_CHAR_COUNTC_VALUE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_CCDR_CHAR_COUNTC_VALUE]

//>> SET_[CPMS_CCDR_CHAR_COUNTA_MOD]
static inline fapi2::buffer<uint64_t>& SET_CPMS_CCDR_CHAR_COUNTA_MOD(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CCDR));
#endif
    return o_data.insertFromRight<CPMS_CCDR_CHAR_COUNTA_MOD,
           1>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("0:ppe_c Invalid dial access CPMS_CCDR_CHAR_COUNTA_MOD chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CCDR_CHAR_COUNTA_MOD]

//>> SET_[CPMS_CCDR_CHAR_COUNTA_MOD]
static inline fapi2::buffer<uint64_t>& SET_CPMS_CCDR_CHAR_COUNTA_MOD(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CCDR));
#endif
    return o_data.insertFromRight<CPMS_CCDR_CHAR_COUNTA_MOD,
           1>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("1:ppe_c Invalid dial access CPMS_CCDR_CHAR_COUNTA_MOD chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CCDR_CHAR_COUNTA_MOD]

//>> SET_[CPMS_CCDR_CHAR_COUNTA_MOD]
static inline fapi2::buffer<uint64_t>& SET_CPMS_CCDR_CHAR_COUNTA_MOD(fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CCDR));
#endif
    return o_data.setBit<CPMS_CCDR_CHAR_COUNTA_MOD>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("2:ppe_c Invalid dial access CPMS_CCDR_CHAR_COUNTA_MOD chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CCDR_CHAR_COUNTA_MOD]

//>> CLEAR_[CPMS_CCDR_CHAR_COUNTA_MOD]
static inline fapi2::buffer<uint64_t>& CLEAR_CPMS_CCDR_CHAR_COUNTA_MOD(fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CCDR));
#endif
    return o_data.clearBit<CPMS_CCDR_CHAR_COUNTA_MOD>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("3:ppe_c Invalid dial access CPMS_CCDR_CHAR_COUNTA_MOD chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< CLEAR_[CPMS_CCDR_CHAR_COUNTA_MOD]

//>> GET_[CPMS_CCDR_CHAR_COUNTA_MOD]
static inline fapi2::buffer<uint64_t>& GET_CPMS_CCDR_CHAR_COUNTA_MOD(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CCDR));
#endif
    return i_data.extractToRight<CPMS_CCDR_CHAR_COUNTA_MOD,
           1>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("4:ppe_c Invalid dial access CPMS_CCDR_CHAR_COUNTA_MOD chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_CCDR_CHAR_COUNTA_MOD]

//>> GET_[CPMS_CCDR_CHAR_COUNTA_MOD]
static inline bool GET_CPMS_CCDR_CHAR_COUNTA_MOD(fapi2::buffer<uint64_t>& i_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CCDR));
#endif
    return i_data.getBit<CPMS_CCDR_CHAR_COUNTA_MOD>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("5:ppe_c Invalid dial access CPMS_CCDR_CHAR_COUNTA_MOD chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_CCDR_CHAR_COUNTA_MOD]

//>> SET_[CPMS_CCDR_CHAR_COUNTB_MOD]
static inline fapi2::buffer<uint64_t>& SET_CPMS_CCDR_CHAR_COUNTB_MOD(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CCDR));
#endif
    return o_data.insertFromRight<CPMS_CCDR_CHAR_COUNTB_MOD,
           1>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("0:ppe_c Invalid dial access CPMS_CCDR_CHAR_COUNTB_MOD chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CCDR_CHAR_COUNTB_MOD]

//>> SET_[CPMS_CCDR_CHAR_COUNTB_MOD]
static inline fapi2::buffer<uint64_t>& SET_CPMS_CCDR_CHAR_COUNTB_MOD(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CCDR));
#endif
    return o_data.insertFromRight<CPMS_CCDR_CHAR_COUNTB_MOD,
           1>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("1:ppe_c Invalid dial access CPMS_CCDR_CHAR_COUNTB_MOD chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CCDR_CHAR_COUNTB_MOD]

//>> SET_[CPMS_CCDR_CHAR_COUNTB_MOD]
static inline fapi2::buffer<uint64_t>& SET_CPMS_CCDR_CHAR_COUNTB_MOD(fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CCDR));
#endif
    return o_data.setBit<CPMS_CCDR_CHAR_COUNTB_MOD>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("2:ppe_c Invalid dial access CPMS_CCDR_CHAR_COUNTB_MOD chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CCDR_CHAR_COUNTB_MOD]

//>> CLEAR_[CPMS_CCDR_CHAR_COUNTB_MOD]
static inline fapi2::buffer<uint64_t>& CLEAR_CPMS_CCDR_CHAR_COUNTB_MOD(fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CCDR));
#endif
    return o_data.clearBit<CPMS_CCDR_CHAR_COUNTB_MOD>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("3:ppe_c Invalid dial access CPMS_CCDR_CHAR_COUNTB_MOD chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< CLEAR_[CPMS_CCDR_CHAR_COUNTB_MOD]

//>> GET_[CPMS_CCDR_CHAR_COUNTB_MOD]
static inline fapi2::buffer<uint64_t>& GET_CPMS_CCDR_CHAR_COUNTB_MOD(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CCDR));
#endif
    return i_data.extractToRight<CPMS_CCDR_CHAR_COUNTB_MOD,
           1>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("4:ppe_c Invalid dial access CPMS_CCDR_CHAR_COUNTB_MOD chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_CCDR_CHAR_COUNTB_MOD]

//>> GET_[CPMS_CCDR_CHAR_COUNTB_MOD]
static inline bool GET_CPMS_CCDR_CHAR_COUNTB_MOD(fapi2::buffer<uint64_t>& i_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CCDR));
#endif
    return i_data.getBit<CPMS_CCDR_CHAR_COUNTB_MOD>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("5:ppe_c Invalid dial access CPMS_CCDR_CHAR_COUNTB_MOD chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_CCDR_CHAR_COUNTB_MOD]

//>> SET_[CPMS_CCDR_CHAR_COUNTC_MOD]
static inline fapi2::buffer<uint64_t>& SET_CPMS_CCDR_CHAR_COUNTC_MOD(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CCDR));
#endif
    return o_data.insertFromRight<CPMS_CCDR_CHAR_COUNTC_MOD,
           1>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("0:ppe_c Invalid dial access CPMS_CCDR_CHAR_COUNTC_MOD chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CCDR_CHAR_COUNTC_MOD]

//>> SET_[CPMS_CCDR_CHAR_COUNTC_MOD]
static inline fapi2::buffer<uint64_t>& SET_CPMS_CCDR_CHAR_COUNTC_MOD(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CCDR));
#endif
    return o_data.insertFromRight<CPMS_CCDR_CHAR_COUNTC_MOD,
           1>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("1:ppe_c Invalid dial access CPMS_CCDR_CHAR_COUNTC_MOD chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CCDR_CHAR_COUNTC_MOD]

//>> SET_[CPMS_CCDR_CHAR_COUNTC_MOD]
static inline fapi2::buffer<uint64_t>& SET_CPMS_CCDR_CHAR_COUNTC_MOD(fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CCDR));
#endif
    return o_data.setBit<CPMS_CCDR_CHAR_COUNTC_MOD>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("2:ppe_c Invalid dial access CPMS_CCDR_CHAR_COUNTC_MOD chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CCDR_CHAR_COUNTC_MOD]

//>> CLEAR_[CPMS_CCDR_CHAR_COUNTC_MOD]
static inline fapi2::buffer<uint64_t>& CLEAR_CPMS_CCDR_CHAR_COUNTC_MOD(fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CCDR));
#endif
    return o_data.clearBit<CPMS_CCDR_CHAR_COUNTC_MOD>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("3:ppe_c Invalid dial access CPMS_CCDR_CHAR_COUNTC_MOD chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< CLEAR_[CPMS_CCDR_CHAR_COUNTC_MOD]

//>> GET_[CPMS_CCDR_CHAR_COUNTC_MOD]
static inline fapi2::buffer<uint64_t>& GET_CPMS_CCDR_CHAR_COUNTC_MOD(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CCDR));
#endif
    return i_data.extractToRight<CPMS_CCDR_CHAR_COUNTC_MOD,
           1>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("4:ppe_c Invalid dial access CPMS_CCDR_CHAR_COUNTC_MOD chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_CCDR_CHAR_COUNTC_MOD]

//>> GET_[CPMS_CCDR_CHAR_COUNTC_MOD]
static inline bool GET_CPMS_CCDR_CHAR_COUNTC_MOD(fapi2::buffer<uint64_t>& i_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CCDR));
#endif
    return i_data.getBit<CPMS_CCDR_CHAR_COUNTC_MOD>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("5:ppe_c Invalid dial access CPMS_CCDR_CHAR_COUNTC_MOD chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_CCDR_CHAR_COUNTC_MOD]

//>> SET_[CPMS_CCDR_SAMPLE_DISABLE_INDICATOR]
static inline fapi2::buffer<uint64_t>& SET_CPMS_CCDR_SAMPLE_DISABLE_INDICATOR(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CCDR));
#endif
    return o_data.insertFromRight<CPMS_CCDR_SAMPLE_DISABLE_INDICATOR,
           1>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("0:ppe_c Invalid dial access CPMS_CCDR_SAMPLE_DISABLE_INDICATOR chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CCDR_SAMPLE_DISABLE_INDICATOR]

//>> SET_[CPMS_CCDR_SAMPLE_DISABLE_INDICATOR]
static inline fapi2::buffer<uint64_t>& SET_CPMS_CCDR_SAMPLE_DISABLE_INDICATOR(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CCDR));
#endif
    return o_data.insertFromRight<CPMS_CCDR_SAMPLE_DISABLE_INDICATOR,
           1>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("1:ppe_c Invalid dial access CPMS_CCDR_SAMPLE_DISABLE_INDICATOR chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CCDR_SAMPLE_DISABLE_INDICATOR]

//>> SET_[CPMS_CCDR_SAMPLE_DISABLE_INDICATOR]
static inline fapi2::buffer<uint64_t>& SET_CPMS_CCDR_SAMPLE_DISABLE_INDICATOR(fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CCDR));
#endif
    return o_data.setBit<CPMS_CCDR_SAMPLE_DISABLE_INDICATOR>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("2:ppe_c Invalid dial access CPMS_CCDR_SAMPLE_DISABLE_INDICATOR chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_CCDR_SAMPLE_DISABLE_INDICATOR]

//>> CLEAR_[CPMS_CCDR_SAMPLE_DISABLE_INDICATOR]
static inline fapi2::buffer<uint64_t>& CLEAR_CPMS_CCDR_SAMPLE_DISABLE_INDICATOR(fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CCDR));
#endif
    return o_data.clearBit<CPMS_CCDR_SAMPLE_DISABLE_INDICATOR>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("3:ppe_c Invalid dial access CPMS_CCDR_SAMPLE_DISABLE_INDICATOR chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< CLEAR_[CPMS_CCDR_SAMPLE_DISABLE_INDICATOR]

//>> GET_[CPMS_CCDR_SAMPLE_DISABLE_INDICATOR]
static inline fapi2::buffer<uint64_t>& GET_CPMS_CCDR_SAMPLE_DISABLE_INDICATOR(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CCDR));
#endif
    return i_data.extractToRight<CPMS_CCDR_SAMPLE_DISABLE_INDICATOR,
           1>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("4:ppe_c Invalid dial access CPMS_CCDR_SAMPLE_DISABLE_INDICATOR chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_CCDR_SAMPLE_DISABLE_INDICATOR]

//>> GET_[CPMS_CCDR_SAMPLE_DISABLE_INDICATOR]
static inline bool GET_CPMS_CCDR_SAMPLE_DISABLE_INDICATOR(fapi2::buffer<uint64_t>& i_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_CCDR));
#endif
    return i_data.getBit<CPMS_CCDR_SAMPLE_DISABLE_INDICATOR>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("5:ppe_c Invalid dial access CPMS_CCDR_SAMPLE_DISABLE_INDICATOR chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_CCDR_SAMPLE_DISABLE_INDICATOR]

//>> PREP_[CPMS_PRSR]
template< fapi2::TargetType K, fapi2::MulticastType M, typename V >
static inline fapi2::ReturnCode PREP_CPMS_PRSR(const fapi2::Target<K, M, V>& i_target)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::init(i_target));
    scomt::last_scom = CPMS_PRSR;
#endif
    return fapi2::current_err;
#ifdef SCOM_CHECKING
fapi_try_exit:
    return fapi2::current_err;
#endif
}
//<< PREP_[CPMS_PRSR]

//>> GET_[CPMS_PRSR]
template< fapi2::TargetType K, fapi2::MulticastType M, typename V >
static inline fapi2::ReturnCode GET_CPMS_PRSR(const fapi2::Target<K, M, V>& i_target, fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::init(i_target));
    scomt::last_scom = CPMS_PRSR;
#endif
    return fapi2::getScom(i_target, CPMS_PRSR, o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    return fapi2::current_err;
#endif
}
//<< GET_[CPMS_PRSR]

//>> PUT_[CPMS_PRSR]
template< fapi2::TargetType K, fapi2::MulticastType M, typename V >
static inline fapi2::ReturnCode PUT_CPMS_PRSR(const fapi2::Target<K, M, V>& i_target, const uint64_t i_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::init(i_target));
    FAPI_TRY(scomt::chkReg(CPMS_PRSR));
#endif
    return fapi2::putScom(i_target, CPMS_PRSR, i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    return fapi2::current_err;
#endif
}
//<< PUT_[CPMS_PRSR]


//>> SET_[CPMS_PRSR_CPMS_PPSR_PERIOD_PROXY_REMAIN]
static inline fapi2::buffer<uint64_t>& SET_CPMS_PRSR_CPMS_PPSR_PERIOD_PROXY_REMAIN(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_PRSR));
#endif
    return o_data.insertFromRight<CPMS_PRSR_CPMS_PPSR_PERIOD_PROXY_REMAIN,
           CPMS_PRSR_CPMS_PPSR_PERIOD_PROXY_REMAIN_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("0:ppe_c Invalid dial access CPMS_PRSR_CPMS_PPSR_PERIOD_PROXY_REMAIN chip: %x, ec: %x", scomt::chip,
             scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_PRSR_CPMS_PPSR_PERIOD_PROXY_REMAIN]

//>> SET_[CPMS_PRSR_CPMS_PPSR_PERIOD_PROXY_REMAIN]
static inline fapi2::buffer<uint64_t>& SET_CPMS_PRSR_CPMS_PPSR_PERIOD_PROXY_REMAIN(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_PRSR));
#endif
    return o_data.insertFromRight<CPMS_PRSR_CPMS_PPSR_PERIOD_PROXY_REMAIN,
           CPMS_PRSR_CPMS_PPSR_PERIOD_PROXY_REMAIN_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("1:ppe_c Invalid dial access CPMS_PRSR_CPMS_PPSR_PERIOD_PROXY_REMAIN chip: %x, ec: %x", scomt::chip,
             scomt::ec);
    return o_data;
#endif
}
//<< SET_[CPMS_PRSR_CPMS_PPSR_PERIOD_PROXY_REMAIN]

//>> GET_[CPMS_PRSR_CPMS_PPSR_PERIOD_PROXY_REMAIN]
static inline fapi2::buffer<uint64_t>& GET_CPMS_PRSR_CPMS_PPSR_PERIOD_PROXY_REMAIN(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(CPMS_PRSR));
#endif
    return i_data.extractToRight<CPMS_PRSR_CPMS_PPSR_PERIOD_PROXY_REMAIN,
           CPMS_PRSR_CPMS_PPSR_PERIOD_PROXY_REMAIN_LEN>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("4:ppe_c Invalid dial access CPMS_PRSR_CPMS_PPSR_PERIOD_PROXY_REMAIN chip: %x, ec: %x", scomt::chip,
             scomt::ec);
    return i_data;
#endif
}
//<< GET_[CPMS_PRSR_CPMS_PPSR_PERIOD_PROXY_REMAIN]

//>> PREP_[QME_CDBA]
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
//<< PREP_[QME_CDBA]

//>> GET_[QME_CDBA]
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
//<< GET_[QME_CDBA]

//>> PUT_[QME_CDBA]
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
//<< PUT_[QME_CDBA]


//>> SET_[QME_CDBA_MESSAGE_NUMBER]
static inline fapi2::buffer<uint64_t>& SET_QME_CDBA_MESSAGE_NUMBER(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_CDBA));
#endif
    return o_data.insertFromRight<QME_CDBA_MESSAGE_NUMBER,
           QME_CDBA_MESSAGE_NUMBER_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("0:ppe_c Invalid dial access QME_CDBA_MESSAGE_NUMBER chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[QME_CDBA_MESSAGE_NUMBER]

//>> SET_[QME_CDBA_MESSAGE_NUMBER]
static inline fapi2::buffer<uint64_t>& SET_QME_CDBA_MESSAGE_NUMBER(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_CDBA));
#endif
    return o_data.insertFromRight<QME_CDBA_MESSAGE_NUMBER,
           QME_CDBA_MESSAGE_NUMBER_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("1:ppe_c Invalid dial access QME_CDBA_MESSAGE_NUMBER chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[QME_CDBA_MESSAGE_NUMBER]

//>> GET_[QME_CDBA_MESSAGE_NUMBER]
static inline fapi2::buffer<uint64_t>& GET_QME_CDBA_MESSAGE_NUMBER(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_CDBA));
#endif
    return i_data.extractToRight<QME_CDBA_MESSAGE_NUMBER,
           QME_CDBA_MESSAGE_NUMBER_LEN>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("4:ppe_c Invalid dial access QME_CDBA_MESSAGE_NUMBER chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[QME_CDBA_MESSAGE_NUMBER]

//>> PREP_[QME_CSIR]
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
//<< PREP_[QME_CSIR]

//>> GET_[QME_CSIR]
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
//<< GET_[QME_CSIR]

//>> PUT_[QME_CSIR]
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
//<< PUT_[QME_CSIR]


//>> SET_[QME_CSIR_PMSR_INSTR_DATA]
static inline fapi2::buffer<uint64_t>& SET_QME_CSIR_PMSR_INSTR_DATA(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_CSIR));
#endif
    return o_data.insertFromRight<QME_CSIR_PMSR_INSTR_DATA,
           QME_CSIR_PMSR_INSTR_DATA_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("0:ppe_c Invalid dial access QME_CSIR_PMSR_INSTR_DATA chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[QME_CSIR_PMSR_INSTR_DATA]

//>> SET_[QME_CSIR_PMSR_INSTR_DATA]
static inline fapi2::buffer<uint64_t>& SET_QME_CSIR_PMSR_INSTR_DATA(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_CSIR));
#endif
    return o_data.insertFromRight<QME_CSIR_PMSR_INSTR_DATA,
           QME_CSIR_PMSR_INSTR_DATA_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("1:ppe_c Invalid dial access QME_CSIR_PMSR_INSTR_DATA chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[QME_CSIR_PMSR_INSTR_DATA]

//>> GET_[QME_CSIR_PMSR_INSTR_DATA]
static inline fapi2::buffer<uint64_t>& GET_QME_CSIR_PMSR_INSTR_DATA(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_CSIR));
#endif
    return i_data.extractToRight<QME_CSIR_PMSR_INSTR_DATA,
           QME_CSIR_PMSR_INSTR_DATA_LEN>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("4:ppe_c Invalid dial access QME_CSIR_PMSR_INSTR_DATA chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[QME_CSIR_PMSR_INSTR_DATA]


//>>THE END<<

#ifndef __PPE_HCODE__
}
}

#endif
#endif

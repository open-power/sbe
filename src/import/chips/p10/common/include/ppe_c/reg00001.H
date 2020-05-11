/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/common/include/ppe_c/reg00001.H $        */
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

#ifndef __PPE_C_REG00001_H_
#define __PPE_C_REG00001_H_

#ifndef __PPE_HCODE__
namespace scomt
{
namespace ppe_c
{
#endif


//>> PREP_[QME_PSCRS2]
template< fapi2::TargetType K, fapi2::MulticastType M, typename V >
static inline fapi2::ReturnCode PREP_QME_PSCRS2(const fapi2::Target<K, M, V>& i_target)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::init(i_target));
    scomt::last_scom = QME_PSCRS2;
#endif
    return fapi2::current_err;
#ifdef SCOM_CHECKING
fapi_try_exit:
    return fapi2::current_err;
#endif
}
//<< PREP_[QME_PSCRS2]

//>> GET_[QME_PSCRS2]
template< fapi2::TargetType K, fapi2::MulticastType M, typename V >
static inline fapi2::ReturnCode GET_QME_PSCRS2(const fapi2::Target<K, M, V>& i_target, fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::init(i_target));
    scomt::last_scom = QME_PSCRS2;
#endif
    return fapi2::getScom(i_target, QME_PSCRS2, o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    return fapi2::current_err;
#endif
}
//<< GET_[QME_PSCRS2]

//>> PUT_[QME_PSCRS2]
template< fapi2::TargetType K, fapi2::MulticastType M, typename V >
static inline fapi2::ReturnCode PUT_QME_PSCRS2(const fapi2::Target<K, M, V>& i_target, const uint64_t i_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::init(i_target));
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return fapi2::putScom(i_target, QME_PSCRS2, i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    return fapi2::current_err;
#endif
}
//<< PUT_[QME_PSCRS2]


//>> SET_[QME_PSCRS2_DPDES_EXIT_ENABLE]
static inline fapi2::buffer<uint64_t>& SET_QME_PSCRS2_DPDES_EXIT_ENABLE(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return o_data.insertFromRight<QME_PSCRS2_DPDES_EXIT_ENABLE,
           1>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("0:ppe_c Invalid dial access QME_PSCRS2_DPDES_EXIT_ENABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[QME_PSCRS2_DPDES_EXIT_ENABLE]

//>> SET_[QME_PSCRS2_DPDES_EXIT_ENABLE]
static inline fapi2::buffer<uint64_t>& SET_QME_PSCRS2_DPDES_EXIT_ENABLE(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return o_data.insertFromRight<QME_PSCRS2_DPDES_EXIT_ENABLE,
           1>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("1:ppe_c Invalid dial access QME_PSCRS2_DPDES_EXIT_ENABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[QME_PSCRS2_DPDES_EXIT_ENABLE]

//>> SET_[QME_PSCRS2_DPDES_EXIT_ENABLE]
static inline fapi2::buffer<uint64_t>& SET_QME_PSCRS2_DPDES_EXIT_ENABLE(fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return o_data.setBit<QME_PSCRS2_DPDES_EXIT_ENABLE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("2:ppe_c Invalid dial access QME_PSCRS2_DPDES_EXIT_ENABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[QME_PSCRS2_DPDES_EXIT_ENABLE]

//>> CLEAR_[QME_PSCRS2_DPDES_EXIT_ENABLE]
static inline fapi2::buffer<uint64_t>& CLEAR_QME_PSCRS2_DPDES_EXIT_ENABLE(fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return o_data.clearBit<QME_PSCRS2_DPDES_EXIT_ENABLE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("3:ppe_c Invalid dial access QME_PSCRS2_DPDES_EXIT_ENABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< CLEAR_[QME_PSCRS2_DPDES_EXIT_ENABLE]

//>> GET_[QME_PSCRS2_DPDES_EXIT_ENABLE]
static inline fapi2::buffer<uint64_t>& GET_QME_PSCRS2_DPDES_EXIT_ENABLE(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return i_data.extractToRight<QME_PSCRS2_DPDES_EXIT_ENABLE,
           1>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("4:ppe_c Invalid dial access QME_PSCRS2_DPDES_EXIT_ENABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[QME_PSCRS2_DPDES_EXIT_ENABLE]

//>> GET_[QME_PSCRS2_DPDES_EXIT_ENABLE]
static inline bool GET_QME_PSCRS2_DPDES_EXIT_ENABLE(fapi2::buffer<uint64_t>& i_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return i_data.getBit<QME_PSCRS2_DPDES_EXIT_ENABLE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("5:ppe_c Invalid dial access QME_PSCRS2_DPDES_EXIT_ENABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[QME_PSCRS2_DPDES_EXIT_ENABLE]

//>> SET_[QME_PSCRS2_SD]
static inline fapi2::buffer<uint64_t>& SET_QME_PSCRS2_SD(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return o_data.insertFromRight<QME_PSCRS2_SD,
           1>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("0:ppe_c Invalid dial access QME_PSCRS2_SD chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[QME_PSCRS2_SD]

//>> SET_[QME_PSCRS2_SD]
static inline fapi2::buffer<uint64_t>& SET_QME_PSCRS2_SD(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return o_data.insertFromRight<QME_PSCRS2_SD,
           1>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("1:ppe_c Invalid dial access QME_PSCRS2_SD chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[QME_PSCRS2_SD]

//>> SET_[QME_PSCRS2_SD]
static inline fapi2::buffer<uint64_t>& SET_QME_PSCRS2_SD(fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return o_data.setBit<QME_PSCRS2_SD>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("2:ppe_c Invalid dial access QME_PSCRS2_SD chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[QME_PSCRS2_SD]

//>> CLEAR_[QME_PSCRS2_SD]
static inline fapi2::buffer<uint64_t>& CLEAR_QME_PSCRS2_SD(fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return o_data.clearBit<QME_PSCRS2_SD>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("3:ppe_c Invalid dial access QME_PSCRS2_SD chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< CLEAR_[QME_PSCRS2_SD]

//>> GET_[QME_PSCRS2_SD]
static inline fapi2::buffer<uint64_t>& GET_QME_PSCRS2_SD(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return i_data.extractToRight<QME_PSCRS2_SD,
           1>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("4:ppe_c Invalid dial access QME_PSCRS2_SD chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[QME_PSCRS2_SD]

//>> GET_[QME_PSCRS2_SD]
static inline bool GET_QME_PSCRS2_SD(fapi2::buffer<uint64_t>& i_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return i_data.getBit<QME_PSCRS2_SD>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("5:ppe_c Invalid dial access QME_PSCRS2_SD chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[QME_PSCRS2_SD]

//>> SET_[QME_PSCRS2_ESL]
static inline fapi2::buffer<uint64_t>& SET_QME_PSCRS2_ESL(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return o_data.insertFromRight<QME_PSCRS2_ESL,
           1>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("0:ppe_c Invalid dial access QME_PSCRS2_ESL chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[QME_PSCRS2_ESL]

//>> SET_[QME_PSCRS2_ESL]
static inline fapi2::buffer<uint64_t>& SET_QME_PSCRS2_ESL(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return o_data.insertFromRight<QME_PSCRS2_ESL,
           1>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("1:ppe_c Invalid dial access QME_PSCRS2_ESL chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[QME_PSCRS2_ESL]

//>> SET_[QME_PSCRS2_ESL]
static inline fapi2::buffer<uint64_t>& SET_QME_PSCRS2_ESL(fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return o_data.setBit<QME_PSCRS2_ESL>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("2:ppe_c Invalid dial access QME_PSCRS2_ESL chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[QME_PSCRS2_ESL]

//>> CLEAR_[QME_PSCRS2_ESL]
static inline fapi2::buffer<uint64_t>& CLEAR_QME_PSCRS2_ESL(fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return o_data.clearBit<QME_PSCRS2_ESL>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("3:ppe_c Invalid dial access QME_PSCRS2_ESL chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< CLEAR_[QME_PSCRS2_ESL]

//>> GET_[QME_PSCRS2_ESL]
static inline fapi2::buffer<uint64_t>& GET_QME_PSCRS2_ESL(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return i_data.extractToRight<QME_PSCRS2_ESL,
           1>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("4:ppe_c Invalid dial access QME_PSCRS2_ESL chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[QME_PSCRS2_ESL]

//>> GET_[QME_PSCRS2_ESL]
static inline bool GET_QME_PSCRS2_ESL(fapi2::buffer<uint64_t>& i_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return i_data.getBit<QME_PSCRS2_ESL>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("5:ppe_c Invalid dial access QME_PSCRS2_ESL chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[QME_PSCRS2_ESL]

//>> SET_[QME_PSCRS2_EC]
static inline fapi2::buffer<uint64_t>& SET_QME_PSCRS2_EC(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return o_data.insertFromRight<QME_PSCRS2_EC,
           1>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("0:ppe_c Invalid dial access QME_PSCRS2_EC chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[QME_PSCRS2_EC]

//>> SET_[QME_PSCRS2_EC]
static inline fapi2::buffer<uint64_t>& SET_QME_PSCRS2_EC(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return o_data.insertFromRight<QME_PSCRS2_EC,
           1>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("1:ppe_c Invalid dial access QME_PSCRS2_EC chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[QME_PSCRS2_EC]

//>> SET_[QME_PSCRS2_EC]
static inline fapi2::buffer<uint64_t>& SET_QME_PSCRS2_EC(fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return o_data.setBit<QME_PSCRS2_EC>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("2:ppe_c Invalid dial access QME_PSCRS2_EC chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[QME_PSCRS2_EC]

//>> CLEAR_[QME_PSCRS2_EC]
static inline fapi2::buffer<uint64_t>& CLEAR_QME_PSCRS2_EC(fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return o_data.clearBit<QME_PSCRS2_EC>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("3:ppe_c Invalid dial access QME_PSCRS2_EC chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< CLEAR_[QME_PSCRS2_EC]

//>> GET_[QME_PSCRS2_EC]
static inline fapi2::buffer<uint64_t>& GET_QME_PSCRS2_EC(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return i_data.extractToRight<QME_PSCRS2_EC,
           1>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("4:ppe_c Invalid dial access QME_PSCRS2_EC chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[QME_PSCRS2_EC]

//>> GET_[QME_PSCRS2_EC]
static inline bool GET_QME_PSCRS2_EC(fapi2::buffer<uint64_t>& i_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return i_data.getBit<QME_PSCRS2_EC>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("5:ppe_c Invalid dial access QME_PSCRS2_EC chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[QME_PSCRS2_EC]

//>> SET_[QME_PSCRS2_PSLL]
static inline fapi2::buffer<uint64_t>& SET_QME_PSCRS2_PSLL(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return o_data.insertFromRight<QME_PSCRS2_PSLL,
           QME_PSCRS2_PSLL_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("0:ppe_c Invalid dial access QME_PSCRS2_PSLL chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[QME_PSCRS2_PSLL]

//>> SET_[QME_PSCRS2_PSLL]
static inline fapi2::buffer<uint64_t>& SET_QME_PSCRS2_PSLL(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return o_data.insertFromRight<QME_PSCRS2_PSLL,
           QME_PSCRS2_PSLL_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("1:ppe_c Invalid dial access QME_PSCRS2_PSLL chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[QME_PSCRS2_PSLL]

//>> GET_[QME_PSCRS2_PSLL]
static inline fapi2::buffer<uint64_t>& GET_QME_PSCRS2_PSLL(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return i_data.extractToRight<QME_PSCRS2_PSLL,
           QME_PSCRS2_PSLL_LEN>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("4:ppe_c Invalid dial access QME_PSCRS2_PSLL chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[QME_PSCRS2_PSLL]

//>> SET_[QME_PSCRS2_HYP_VIRT_EXIT_ENABLE]
static inline fapi2::buffer<uint64_t>& SET_QME_PSCRS2_HYP_VIRT_EXIT_ENABLE(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return o_data.insertFromRight<QME_PSCRS2_HYP_VIRT_EXIT_ENABLE,
           1>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("0:ppe_c Invalid dial access QME_PSCRS2_HYP_VIRT_EXIT_ENABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[QME_PSCRS2_HYP_VIRT_EXIT_ENABLE]

//>> SET_[QME_PSCRS2_HYP_VIRT_EXIT_ENABLE]
static inline fapi2::buffer<uint64_t>& SET_QME_PSCRS2_HYP_VIRT_EXIT_ENABLE(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return o_data.insertFromRight<QME_PSCRS2_HYP_VIRT_EXIT_ENABLE,
           1>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("1:ppe_c Invalid dial access QME_PSCRS2_HYP_VIRT_EXIT_ENABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[QME_PSCRS2_HYP_VIRT_EXIT_ENABLE]

//>> SET_[QME_PSCRS2_HYP_VIRT_EXIT_ENABLE]
static inline fapi2::buffer<uint64_t>& SET_QME_PSCRS2_HYP_VIRT_EXIT_ENABLE(fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return o_data.setBit<QME_PSCRS2_HYP_VIRT_EXIT_ENABLE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("2:ppe_c Invalid dial access QME_PSCRS2_HYP_VIRT_EXIT_ENABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[QME_PSCRS2_HYP_VIRT_EXIT_ENABLE]

//>> CLEAR_[QME_PSCRS2_HYP_VIRT_EXIT_ENABLE]
static inline fapi2::buffer<uint64_t>& CLEAR_QME_PSCRS2_HYP_VIRT_EXIT_ENABLE(fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return o_data.clearBit<QME_PSCRS2_HYP_VIRT_EXIT_ENABLE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("3:ppe_c Invalid dial access QME_PSCRS2_HYP_VIRT_EXIT_ENABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< CLEAR_[QME_PSCRS2_HYP_VIRT_EXIT_ENABLE]

//>> GET_[QME_PSCRS2_HYP_VIRT_EXIT_ENABLE]
static inline fapi2::buffer<uint64_t>& GET_QME_PSCRS2_HYP_VIRT_EXIT_ENABLE(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return i_data.extractToRight<QME_PSCRS2_HYP_VIRT_EXIT_ENABLE,
           1>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("4:ppe_c Invalid dial access QME_PSCRS2_HYP_VIRT_EXIT_ENABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[QME_PSCRS2_HYP_VIRT_EXIT_ENABLE]

//>> GET_[QME_PSCRS2_HYP_VIRT_EXIT_ENABLE]
static inline bool GET_QME_PSCRS2_HYP_VIRT_EXIT_ENABLE(fapi2::buffer<uint64_t>& i_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return i_data.getBit<QME_PSCRS2_HYP_VIRT_EXIT_ENABLE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("5:ppe_c Invalid dial access QME_PSCRS2_HYP_VIRT_EXIT_ENABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[QME_PSCRS2_HYP_VIRT_EXIT_ENABLE]

//>> SET_[QME_PSCRS2_UV_DB_EXIT_ENABLE]
static inline fapi2::buffer<uint64_t>& SET_QME_PSCRS2_UV_DB_EXIT_ENABLE(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return o_data.insertFromRight<QME_PSCRS2_UV_DB_EXIT_ENABLE,
           1>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("0:ppe_c Invalid dial access QME_PSCRS2_UV_DB_EXIT_ENABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[QME_PSCRS2_UV_DB_EXIT_ENABLE]

//>> SET_[QME_PSCRS2_UV_DB_EXIT_ENABLE]
static inline fapi2::buffer<uint64_t>& SET_QME_PSCRS2_UV_DB_EXIT_ENABLE(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return o_data.insertFromRight<QME_PSCRS2_UV_DB_EXIT_ENABLE,
           1>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("1:ppe_c Invalid dial access QME_PSCRS2_UV_DB_EXIT_ENABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[QME_PSCRS2_UV_DB_EXIT_ENABLE]

//>> SET_[QME_PSCRS2_UV_DB_EXIT_ENABLE]
static inline fapi2::buffer<uint64_t>& SET_QME_PSCRS2_UV_DB_EXIT_ENABLE(fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return o_data.setBit<QME_PSCRS2_UV_DB_EXIT_ENABLE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("2:ppe_c Invalid dial access QME_PSCRS2_UV_DB_EXIT_ENABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[QME_PSCRS2_UV_DB_EXIT_ENABLE]

//>> CLEAR_[QME_PSCRS2_UV_DB_EXIT_ENABLE]
static inline fapi2::buffer<uint64_t>& CLEAR_QME_PSCRS2_UV_DB_EXIT_ENABLE(fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return o_data.clearBit<QME_PSCRS2_UV_DB_EXIT_ENABLE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("3:ppe_c Invalid dial access QME_PSCRS2_UV_DB_EXIT_ENABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< CLEAR_[QME_PSCRS2_UV_DB_EXIT_ENABLE]

//>> GET_[QME_PSCRS2_UV_DB_EXIT_ENABLE]
static inline fapi2::buffer<uint64_t>& GET_QME_PSCRS2_UV_DB_EXIT_ENABLE(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return i_data.extractToRight<QME_PSCRS2_UV_DB_EXIT_ENABLE,
           1>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("4:ppe_c Invalid dial access QME_PSCRS2_UV_DB_EXIT_ENABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[QME_PSCRS2_UV_DB_EXIT_ENABLE]

//>> GET_[QME_PSCRS2_UV_DB_EXIT_ENABLE]
static inline bool GET_QME_PSCRS2_UV_DB_EXIT_ENABLE(fapi2::buffer<uint64_t>& i_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return i_data.getBit<QME_PSCRS2_UV_DB_EXIT_ENABLE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("5:ppe_c Invalid dial access QME_PSCRS2_UV_DB_EXIT_ENABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[QME_PSCRS2_UV_DB_EXIT_ENABLE]

//>> SET_[QME_PSCRS2_HYP_DB_EXIT_ENABLE]
static inline fapi2::buffer<uint64_t>& SET_QME_PSCRS2_HYP_DB_EXIT_ENABLE(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return o_data.insertFromRight<QME_PSCRS2_HYP_DB_EXIT_ENABLE,
           1>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("0:ppe_c Invalid dial access QME_PSCRS2_HYP_DB_EXIT_ENABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[QME_PSCRS2_HYP_DB_EXIT_ENABLE]

//>> SET_[QME_PSCRS2_HYP_DB_EXIT_ENABLE]
static inline fapi2::buffer<uint64_t>& SET_QME_PSCRS2_HYP_DB_EXIT_ENABLE(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return o_data.insertFromRight<QME_PSCRS2_HYP_DB_EXIT_ENABLE,
           1>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("1:ppe_c Invalid dial access QME_PSCRS2_HYP_DB_EXIT_ENABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[QME_PSCRS2_HYP_DB_EXIT_ENABLE]

//>> SET_[QME_PSCRS2_HYP_DB_EXIT_ENABLE]
static inline fapi2::buffer<uint64_t>& SET_QME_PSCRS2_HYP_DB_EXIT_ENABLE(fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return o_data.setBit<QME_PSCRS2_HYP_DB_EXIT_ENABLE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("2:ppe_c Invalid dial access QME_PSCRS2_HYP_DB_EXIT_ENABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[QME_PSCRS2_HYP_DB_EXIT_ENABLE]

//>> CLEAR_[QME_PSCRS2_HYP_DB_EXIT_ENABLE]
static inline fapi2::buffer<uint64_t>& CLEAR_QME_PSCRS2_HYP_DB_EXIT_ENABLE(fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return o_data.clearBit<QME_PSCRS2_HYP_DB_EXIT_ENABLE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("3:ppe_c Invalid dial access QME_PSCRS2_HYP_DB_EXIT_ENABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< CLEAR_[QME_PSCRS2_HYP_DB_EXIT_ENABLE]

//>> GET_[QME_PSCRS2_HYP_DB_EXIT_ENABLE]
static inline fapi2::buffer<uint64_t>& GET_QME_PSCRS2_HYP_DB_EXIT_ENABLE(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return i_data.extractToRight<QME_PSCRS2_HYP_DB_EXIT_ENABLE,
           1>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("4:ppe_c Invalid dial access QME_PSCRS2_HYP_DB_EXIT_ENABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[QME_PSCRS2_HYP_DB_EXIT_ENABLE]

//>> GET_[QME_PSCRS2_HYP_DB_EXIT_ENABLE]
static inline bool GET_QME_PSCRS2_HYP_DB_EXIT_ENABLE(fapi2::buffer<uint64_t>& i_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return i_data.getBit<QME_PSCRS2_HYP_DB_EXIT_ENABLE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("5:ppe_c Invalid dial access QME_PSCRS2_HYP_DB_EXIT_ENABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[QME_PSCRS2_HYP_DB_EXIT_ENABLE]

//>> SET_[QME_PSCRS2_EXT_EXIT_ENABLE]
static inline fapi2::buffer<uint64_t>& SET_QME_PSCRS2_EXT_EXIT_ENABLE(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return o_data.insertFromRight<QME_PSCRS2_EXT_EXIT_ENABLE,
           1>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("0:ppe_c Invalid dial access QME_PSCRS2_EXT_EXIT_ENABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[QME_PSCRS2_EXT_EXIT_ENABLE]

//>> SET_[QME_PSCRS2_EXT_EXIT_ENABLE]
static inline fapi2::buffer<uint64_t>& SET_QME_PSCRS2_EXT_EXIT_ENABLE(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return o_data.insertFromRight<QME_PSCRS2_EXT_EXIT_ENABLE,
           1>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("1:ppe_c Invalid dial access QME_PSCRS2_EXT_EXIT_ENABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[QME_PSCRS2_EXT_EXIT_ENABLE]

//>> SET_[QME_PSCRS2_EXT_EXIT_ENABLE]
static inline fapi2::buffer<uint64_t>& SET_QME_PSCRS2_EXT_EXIT_ENABLE(fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return o_data.setBit<QME_PSCRS2_EXT_EXIT_ENABLE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("2:ppe_c Invalid dial access QME_PSCRS2_EXT_EXIT_ENABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[QME_PSCRS2_EXT_EXIT_ENABLE]

//>> CLEAR_[QME_PSCRS2_EXT_EXIT_ENABLE]
static inline fapi2::buffer<uint64_t>& CLEAR_QME_PSCRS2_EXT_EXIT_ENABLE(fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return o_data.clearBit<QME_PSCRS2_EXT_EXIT_ENABLE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("3:ppe_c Invalid dial access QME_PSCRS2_EXT_EXIT_ENABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< CLEAR_[QME_PSCRS2_EXT_EXIT_ENABLE]

//>> GET_[QME_PSCRS2_EXT_EXIT_ENABLE]
static inline fapi2::buffer<uint64_t>& GET_QME_PSCRS2_EXT_EXIT_ENABLE(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return i_data.extractToRight<QME_PSCRS2_EXT_EXIT_ENABLE,
           1>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("4:ppe_c Invalid dial access QME_PSCRS2_EXT_EXIT_ENABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[QME_PSCRS2_EXT_EXIT_ENABLE]

//>> GET_[QME_PSCRS2_EXT_EXIT_ENABLE]
static inline bool GET_QME_PSCRS2_EXT_EXIT_ENABLE(fapi2::buffer<uint64_t>& i_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return i_data.getBit<QME_PSCRS2_EXT_EXIT_ENABLE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("5:ppe_c Invalid dial access QME_PSCRS2_EXT_EXIT_ENABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[QME_PSCRS2_EXT_EXIT_ENABLE]

//>> SET_[QME_PSCRS2_DEC_EXIT_ENABLE]
static inline fapi2::buffer<uint64_t>& SET_QME_PSCRS2_DEC_EXIT_ENABLE(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return o_data.insertFromRight<QME_PSCRS2_DEC_EXIT_ENABLE,
           1>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("0:ppe_c Invalid dial access QME_PSCRS2_DEC_EXIT_ENABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[QME_PSCRS2_DEC_EXIT_ENABLE]

//>> SET_[QME_PSCRS2_DEC_EXIT_ENABLE]
static inline fapi2::buffer<uint64_t>& SET_QME_PSCRS2_DEC_EXIT_ENABLE(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return o_data.insertFromRight<QME_PSCRS2_DEC_EXIT_ENABLE,
           1>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("1:ppe_c Invalid dial access QME_PSCRS2_DEC_EXIT_ENABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[QME_PSCRS2_DEC_EXIT_ENABLE]

//>> SET_[QME_PSCRS2_DEC_EXIT_ENABLE]
static inline fapi2::buffer<uint64_t>& SET_QME_PSCRS2_DEC_EXIT_ENABLE(fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return o_data.setBit<QME_PSCRS2_DEC_EXIT_ENABLE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("2:ppe_c Invalid dial access QME_PSCRS2_DEC_EXIT_ENABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[QME_PSCRS2_DEC_EXIT_ENABLE]

//>> CLEAR_[QME_PSCRS2_DEC_EXIT_ENABLE]
static inline fapi2::buffer<uint64_t>& CLEAR_QME_PSCRS2_DEC_EXIT_ENABLE(fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return o_data.clearBit<QME_PSCRS2_DEC_EXIT_ENABLE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("3:ppe_c Invalid dial access QME_PSCRS2_DEC_EXIT_ENABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< CLEAR_[QME_PSCRS2_DEC_EXIT_ENABLE]

//>> GET_[QME_PSCRS2_DEC_EXIT_ENABLE]
static inline fapi2::buffer<uint64_t>& GET_QME_PSCRS2_DEC_EXIT_ENABLE(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return i_data.extractToRight<QME_PSCRS2_DEC_EXIT_ENABLE,
           1>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("4:ppe_c Invalid dial access QME_PSCRS2_DEC_EXIT_ENABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[QME_PSCRS2_DEC_EXIT_ENABLE]

//>> GET_[QME_PSCRS2_DEC_EXIT_ENABLE]
static inline bool GET_QME_PSCRS2_DEC_EXIT_ENABLE(fapi2::buffer<uint64_t>& i_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return i_data.getBit<QME_PSCRS2_DEC_EXIT_ENABLE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("5:ppe_c Invalid dial access QME_PSCRS2_DEC_EXIT_ENABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[QME_PSCRS2_DEC_EXIT_ENABLE]

//>> SET_[QME_PSCRS2_HMI_EXIT_ENABLE]
static inline fapi2::buffer<uint64_t>& SET_QME_PSCRS2_HMI_EXIT_ENABLE(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return o_data.insertFromRight<QME_PSCRS2_HMI_EXIT_ENABLE,
           1>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("0:ppe_c Invalid dial access QME_PSCRS2_HMI_EXIT_ENABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[QME_PSCRS2_HMI_EXIT_ENABLE]

//>> SET_[QME_PSCRS2_HMI_EXIT_ENABLE]
static inline fapi2::buffer<uint64_t>& SET_QME_PSCRS2_HMI_EXIT_ENABLE(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return o_data.insertFromRight<QME_PSCRS2_HMI_EXIT_ENABLE,
           1>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("1:ppe_c Invalid dial access QME_PSCRS2_HMI_EXIT_ENABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[QME_PSCRS2_HMI_EXIT_ENABLE]

//>> SET_[QME_PSCRS2_HMI_EXIT_ENABLE]
static inline fapi2::buffer<uint64_t>& SET_QME_PSCRS2_HMI_EXIT_ENABLE(fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return o_data.setBit<QME_PSCRS2_HMI_EXIT_ENABLE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("2:ppe_c Invalid dial access QME_PSCRS2_HMI_EXIT_ENABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[QME_PSCRS2_HMI_EXIT_ENABLE]

//>> CLEAR_[QME_PSCRS2_HMI_EXIT_ENABLE]
static inline fapi2::buffer<uint64_t>& CLEAR_QME_PSCRS2_HMI_EXIT_ENABLE(fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return o_data.clearBit<QME_PSCRS2_HMI_EXIT_ENABLE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("3:ppe_c Invalid dial access QME_PSCRS2_HMI_EXIT_ENABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< CLEAR_[QME_PSCRS2_HMI_EXIT_ENABLE]

//>> GET_[QME_PSCRS2_HMI_EXIT_ENABLE]
static inline fapi2::buffer<uint64_t>& GET_QME_PSCRS2_HMI_EXIT_ENABLE(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return i_data.extractToRight<QME_PSCRS2_HMI_EXIT_ENABLE,
           1>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("4:ppe_c Invalid dial access QME_PSCRS2_HMI_EXIT_ENABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[QME_PSCRS2_HMI_EXIT_ENABLE]

//>> GET_[QME_PSCRS2_HMI_EXIT_ENABLE]
static inline bool GET_QME_PSCRS2_HMI_EXIT_ENABLE(fapi2::buffer<uint64_t>& i_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return i_data.getBit<QME_PSCRS2_HMI_EXIT_ENABLE>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("5:ppe_c Invalid dial access QME_PSCRS2_HMI_EXIT_ENABLE chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[QME_PSCRS2_HMI_EXIT_ENABLE]

//>> SET_[QME_PSCRS2_TR]
static inline fapi2::buffer<uint64_t>& SET_QME_PSCRS2_TR(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return o_data.insertFromRight<QME_PSCRS2_TR,
           QME_PSCRS2_TR_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("0:ppe_c Invalid dial access QME_PSCRS2_TR chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[QME_PSCRS2_TR]

//>> SET_[QME_PSCRS2_TR]
static inline fapi2::buffer<uint64_t>& SET_QME_PSCRS2_TR(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return o_data.insertFromRight<QME_PSCRS2_TR,
           QME_PSCRS2_TR_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("1:ppe_c Invalid dial access QME_PSCRS2_TR chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[QME_PSCRS2_TR]

//>> GET_[QME_PSCRS2_TR]
static inline fapi2::buffer<uint64_t>& GET_QME_PSCRS2_TR(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return i_data.extractToRight<QME_PSCRS2_TR,
           QME_PSCRS2_TR_LEN>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("4:ppe_c Invalid dial access QME_PSCRS2_TR chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[QME_PSCRS2_TR]

//>> SET_[QME_PSCRS2_MTL]
static inline fapi2::buffer<uint64_t>& SET_QME_PSCRS2_MTL(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return o_data.insertFromRight<QME_PSCRS2_MTL,
           QME_PSCRS2_MTL_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("0:ppe_c Invalid dial access QME_PSCRS2_MTL chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[QME_PSCRS2_MTL]

//>> SET_[QME_PSCRS2_MTL]
static inline fapi2::buffer<uint64_t>& SET_QME_PSCRS2_MTL(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return o_data.insertFromRight<QME_PSCRS2_MTL,
           QME_PSCRS2_MTL_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("1:ppe_c Invalid dial access QME_PSCRS2_MTL chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[QME_PSCRS2_MTL]

//>> GET_[QME_PSCRS2_MTL]
static inline fapi2::buffer<uint64_t>& GET_QME_PSCRS2_MTL(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return i_data.extractToRight<QME_PSCRS2_MTL,
           QME_PSCRS2_MTL_LEN>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("4:ppe_c Invalid dial access QME_PSCRS2_MTL chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[QME_PSCRS2_MTL]

//>> SET_[QME_PSCRS2_RL]
static inline fapi2::buffer<uint64_t>& SET_QME_PSCRS2_RL(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return o_data.insertFromRight<QME_PSCRS2_RL,
           QME_PSCRS2_RL_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("0:ppe_c Invalid dial access QME_PSCRS2_RL chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[QME_PSCRS2_RL]

//>> SET_[QME_PSCRS2_RL]
static inline fapi2::buffer<uint64_t>& SET_QME_PSCRS2_RL(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return o_data.insertFromRight<QME_PSCRS2_RL,
           QME_PSCRS2_RL_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("1:ppe_c Invalid dial access QME_PSCRS2_RL chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[QME_PSCRS2_RL]

//>> GET_[QME_PSCRS2_RL]
static inline fapi2::buffer<uint64_t>& GET_QME_PSCRS2_RL(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return i_data.extractToRight<QME_PSCRS2_RL,
           QME_PSCRS2_RL_LEN>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("4:ppe_c Invalid dial access QME_PSCRS2_RL chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[QME_PSCRS2_RL]

//>> SET_[QME_PSCRS2_PLS]
static inline fapi2::buffer<uint64_t>& SET_QME_PSCRS2_PLS(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return o_data.insertFromRight<QME_PSCRS2_PLS,
           QME_PSCRS2_PLS_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("0:ppe_c Invalid dial access QME_PSCRS2_PLS chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[QME_PSCRS2_PLS]

//>> SET_[QME_PSCRS2_PLS]
static inline fapi2::buffer<uint64_t>& SET_QME_PSCRS2_PLS(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return o_data.insertFromRight<QME_PSCRS2_PLS,
           QME_PSCRS2_PLS_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("1:ppe_c Invalid dial access QME_PSCRS2_PLS chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[QME_PSCRS2_PLS]

//>> GET_[QME_PSCRS2_PLS]
static inline fapi2::buffer<uint64_t>& GET_QME_PSCRS2_PLS(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PSCRS2));
#endif
    return i_data.extractToRight<QME_PSCRS2_PLS,
           QME_PSCRS2_PLS_LEN>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("4:ppe_c Invalid dial access QME_PSCRS2_PLS chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[QME_PSCRS2_PLS]


//>> PREP_[QME_PMSRS]
template< fapi2::TargetType K, fapi2::MulticastType M, typename V >
static inline fapi2::ReturnCode PREP_QME_PMSRS(const fapi2::Target<K, M, V>& i_target)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::init(i_target));
    scomt::last_scom = QME_PMSRS;
#endif
    return fapi2::current_err;
#ifdef SCOM_CHECKING
fapi_try_exit:
    return fapi2::current_err;
#endif
}
//<< PREP_[QME_PMSRS]

//>> GET_[QME_PMSRS]
template< fapi2::TargetType K, fapi2::MulticastType M, typename V >
static inline fapi2::ReturnCode GET_QME_PMSRS(const fapi2::Target<K, M, V>& i_target, fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::init(i_target));
    scomt::last_scom = QME_PMSRS;
#endif
    return fapi2::getScom(i_target, QME_PMSRS, o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    return fapi2::current_err;
#endif
}
//<< GET_[QME_PMSRS]

//>> PUT_[QME_PMSRS]
template< fapi2::TargetType K, fapi2::MulticastType M, typename V >
static inline fapi2::ReturnCode PUT_QME_PMSRS(const fapi2::Target<K, M, V>& i_target, const uint64_t i_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::init(i_target));
    FAPI_TRY(scomt::chkReg(QME_PMSRS));
#endif
    return fapi2::putScom(i_target, QME_PMSRS, i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    return fapi2::current_err;
#endif
}
//<< PUT_[QME_PMSRS]


//>> SET_[QME_PMSRS_PSTATE_DATA]
static inline fapi2::buffer<uint64_t>& SET_QME_PMSRS_PSTATE_DATA(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PMSRS));
#endif
    return o_data.insertFromRight<QME_PMSRS_PSTATE_DATA,
           QME_PMSRS_PSTATE_DATA_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("0:ppe_c Invalid dial access QME_PMSRS_PSTATE_DATA chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[QME_PMSRS_PSTATE_DATA]

//>> SET_[QME_PMSRS_PSTATE_DATA]
static inline fapi2::buffer<uint64_t>& SET_QME_PMSRS_PSTATE_DATA(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PMSRS));
#endif
    return o_data.insertFromRight<QME_PMSRS_PSTATE_DATA,
           QME_PMSRS_PSTATE_DATA_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("1:ppe_c Invalid dial access QME_PMSRS_PSTATE_DATA chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[QME_PMSRS_PSTATE_DATA]

//>> GET_[QME_PMSRS_PSTATE_DATA]
static inline fapi2::buffer<uint64_t>& GET_QME_PMSRS_PSTATE_DATA(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PMSRS));
#endif
    return i_data.extractToRight<QME_PMSRS_PSTATE_DATA,
           QME_PMSRS_PSTATE_DATA_LEN>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("4:ppe_c Invalid dial access QME_PMSRS_PSTATE_DATA chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[QME_PMSRS_PSTATE_DATA]

//>> SET_[QME_PMSRS_INVALID_VERSION]
static inline fapi2::buffer<uint64_t>& SET_QME_PMSRS_INVALID_VERSION(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PMSRS));
#endif
    return o_data.insertFromRight<QME_PMSRS_INVALID_VERSION,
           1>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("0:ppe_c Invalid dial access QME_PMSRS_INVALID_VERSION chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[QME_PMSRS_INVALID_VERSION]

//>> SET_[QME_PMSRS_INVALID_VERSION]
static inline fapi2::buffer<uint64_t>& SET_QME_PMSRS_INVALID_VERSION(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PMSRS));
#endif
    return o_data.insertFromRight<QME_PMSRS_INVALID_VERSION,
           1>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("1:ppe_c Invalid dial access QME_PMSRS_INVALID_VERSION chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[QME_PMSRS_INVALID_VERSION]

//>> SET_[QME_PMSRS_INVALID_VERSION]
static inline fapi2::buffer<uint64_t>& SET_QME_PMSRS_INVALID_VERSION(fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PMSRS));
#endif
    return o_data.setBit<QME_PMSRS_INVALID_VERSION>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("2:ppe_c Invalid dial access QME_PMSRS_INVALID_VERSION chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[QME_PMSRS_INVALID_VERSION]

//>> CLEAR_[QME_PMSRS_INVALID_VERSION]
static inline fapi2::buffer<uint64_t>& CLEAR_QME_PMSRS_INVALID_VERSION(fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PMSRS));
#endif
    return o_data.clearBit<QME_PMSRS_INVALID_VERSION>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("3:ppe_c Invalid dial access QME_PMSRS_INVALID_VERSION chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< CLEAR_[QME_PMSRS_INVALID_VERSION]

//>> GET_[QME_PMSRS_INVALID_VERSION]
static inline fapi2::buffer<uint64_t>& GET_QME_PMSRS_INVALID_VERSION(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PMSRS));
#endif
    return i_data.extractToRight<QME_PMSRS_INVALID_VERSION,
           1>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("4:ppe_c Invalid dial access QME_PMSRS_INVALID_VERSION chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[QME_PMSRS_INVALID_VERSION]

//>> GET_[QME_PMSRS_INVALID_VERSION]
static inline bool GET_QME_PMSRS_INVALID_VERSION(fapi2::buffer<uint64_t>& i_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PMSRS));
#endif
    return i_data.getBit<QME_PMSRS_INVALID_VERSION>();
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("5:ppe_c Invalid dial access QME_PMSRS_INVALID_VERSION chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[QME_PMSRS_INVALID_VERSION]

//>> SET_[QME_PMSRS_VERSION]
static inline fapi2::buffer<uint64_t>& SET_QME_PMSRS_VERSION(fapi2::buffer<uint64_t>& i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PMSRS));
#endif
    return o_data.insertFromRight<QME_PMSRS_VERSION,
           QME_PMSRS_VERSION_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("0:ppe_c Invalid dial access QME_PMSRS_VERSION chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[QME_PMSRS_VERSION]

//>> SET_[QME_PMSRS_VERSION]
static inline fapi2::buffer<uint64_t>& SET_QME_PMSRS_VERSION(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PMSRS));
#endif
    return o_data.insertFromRight<QME_PMSRS_VERSION,
           QME_PMSRS_VERSION_LEN>(i_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("1:ppe_c Invalid dial access QME_PMSRS_VERSION chip: %x, ec: %x", scomt::chip, scomt::ec);
    return o_data;
#endif
}
//<< SET_[QME_PMSRS_VERSION]

//>> GET_[QME_PMSRS_VERSION]
static inline fapi2::buffer<uint64_t>& GET_QME_PMSRS_VERSION(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data)
{
#ifdef SCOM_CHECKING
    FAPI_TRY(scomt::chkReg(QME_PMSRS));
#endif
    return i_data.extractToRight<QME_PMSRS_VERSION,
           QME_PMSRS_VERSION_LEN>(o_data);
#ifdef SCOM_CHECKING
fapi_try_exit:
    FAPI_ERR("4:ppe_c Invalid dial access QME_PMSRS_VERSION chip: %x, ec: %x", scomt::chip, scomt::ec);
    return i_data;
#endif
}
//<< GET_[QME_PMSRS_VERSION]

//>>THE END<<

#ifndef __PPE_HCODE__
}
}

#endif
#endif

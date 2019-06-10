/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/hwpf/plat_attribute_service.H $                           */
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
 *  @file fapiPlatAttributeService.H
 *
 *  @brief Defines the PLAT attribute access macros and defines which macro
 *         handles each attribute.
 *
 */


#ifndef FAPI2PLATATTRIBUTESERVICE_H_
#define FAPI2PLATATTRIBUTESERVICE_H_

#include <stdint.h>
#include <stddef.h>
#include <type_traits>
#include <attribute_ids.H>
#include <fapi2_chip_ec_feature.H>
#include <plat_includes.H>
#include "proc_sbe_fixed.H"
#include "plat_target_parms.H"

#define PLAT_GET_CHIP_EC_FEATURE_OVERRIDE(ID, PTARGET, VAL) \
      _getEcFeatureOverride<ID##_Type>(ID, PTARGET, VAL)

#define PLAT_ATTR_INIT(ID, TARGET, VAL) \
    (fapi2::checkIdType<ID##_Type>(ID, VAL), \
     ID##_PLAT_INIT(ID, TARGET, VAL))

/* INSERT NEW ATTRIBUTES HERE */


/******************************************************************************/
/*                          * Global macros *                                 */
/* These macros are called by the macros above to invoke the appropriate API. */
/* These macros do not need to change when a new attribute is introduced.     */
/******************************************************************************/

/* global get uint8_t 1D array macro */
#define PLAT_ATTR_GET_UINT8_1D_ARRAY(ID, PTARGET, VAL) \
     _getAttributeArrayShort<static_cast<fapi2::TargetType>(ID##_TargetType), ID> \
            (ID, PTARGET, VAL)

/* global set uint8_t 1D array macro */
#define PLAT_ATTR_SET_UINT8_1D_ARRAY(ID, PTARGET, VAL) \
     _setAttributeArrayShort<static_cast<fapi2::TargetType>(ID##_TargetType), ID> \
            (ID, PTARGET, VAL)

/* global get uint8_t 2D array macro */
#define PLAT_ATTR_GET_UINT8_2D_ARRAY(ID, PTARGET, VAL) \
     _getAttributeArrayShort(ID, PTARGET, VAL[0])
/* global set uint8_t 2D array macro */
#define PLAT_ATTR_SET_UINT8_2D_ARRAY(ID, PTARGET, VAL) \
     _setAttributeArrayShort(ID, PTARGET, VAL[0])

/* global get uint8_t 3D array macro */
#define PLAT_ATTR_GET_UINT8_3D_ARRAY(ID, PTARGET, VAL) \
     _getAttributeArrayShort(ID, PTARGET, VAL[0][0])
/* global set uint8_t 3D array macro */
#define PLAT_ATTR_SET_UINT8_3D_ARRAY(ID, PTARGET, VAL) \
     _setAttributeArrayShort(ID, PTARGET, VAL[0][0])

/* global get uint8_t 4D array macro */
#define PLAT_ATTR_GET_UINT8_4D_ARRAY(ID, PTARGET, VAL) \
     _getAttributeArrayShort(ID, PTARGET, VAL[0][0][0])
/* global set uint8_t 4D array macro */
#define PLAT_ATTR_SET_UINT8_4D_ARRAY(ID, PTARGET, VAL) \
     _setAttributeArrayShort(ID, PTARGET, VAL[0][0][0])

/* global get uint32_t 1D array macro */
#define PLAT_ATTR_GET_UINT32_1D_ARRAY(ID, PTARGET, VAL) \
     _getAttributeArrayWord(ID, PTARGET, VAL)
/* global set uint32_t 1D array macro */
#define PLAT_ATTR_SET_UINT32_1D_ARRAY(ID, PTARGET, VAL) \
     _setAttributeArrayWord(ID, PTARGET, VAL)

/* global get uint32_t 2D array macro */
#define PLAT_ATTR_GET_UINT32_2D_ARRAY(ID, PTARGET, VAL) \
     _getAttributeArrayWord(ID, PTARGET, VAL[0])
/* global set uint32_t 2D array macro */
#define PLAT_ATTR_SET_UINT32_2D_ARRAY(ID, PTARGET, VAL) \
     _setAttributeArrayWord(ID, PTARGET, VAL[0])

/* global get uint32_t 3D array macro */
#define PLAT_ATTR_GET_UINT32_3D_ARRAY(ID, PTARGET, VAL) \
     _getAttributeArrayWord(ID, PTARGET, VAL[0][0])
/* global set uint32_t 3D array macro */
#define PLAT_ATTR_SET_UINT32_3D_ARRAY(ID, PTARGET, VAL) \
     _setAttributeArrayWord(ID, PTARGET, VAL[0][0])

/* global get uint32_t 4D array macro */
#define PLAT_ATTR_GET_UINT32_4D_ARRAY(ID, PTARGET, VAL) \
     _getAttributeArrayWord(ID, PTARGET, VAL[0][0][0])
/* global set uint32_t 4D array macro */
#define PLAT_ATTR_SET_UINT32_4D_ARRAY(ID, PTARGET, VAL) \
     _setAttributeArrayWord(ID, PTARGET, VAL[0][0][0])


/* global get uint64_t 1D array macro */
#define PLAT_ATTR_GET_UINT64_1D_ARRAY(ID, PTARGET, VAL) \
     _getAttributeArrayDoubleWord<static_cast<fapi2::TargetType>(ID##_TargetType), ID> \
             (ID, PTARGET, VAL)
/* global set uint64_t 1D array macro */
#define PLAT_ATTR_SET_UINT64_1D_ARRAY(ID, PTARGET, VAL) \
     _setAttributeArrayDoubleWord<static_cast<fapi2::TargetType>(ID##_TargetType), ID> \
             (ID, PTARGET, VAL)

/* global get uint64_t 2D array macro */
#define PLAT_ATTR_GET_UINT64_2D_ARRAY(ID, PTARGET, VAL) \
     _getAttributeArrayDoubleWord(ID, PTARGET, VAL[0])
/* global set uint64_t 2D array macro */
#define PLAT_ATTR_SET_UINT64_2D_ARRAY(ID, PTARGET, VAL) \
     _setAttributeArrayDoubleWord(ID, PTARGET, VAL[0])

/* global get uint64_t 3D array macro */
#define PLAT_ATTR_GET_UINT64_3D_ARRAY(ID, PTARGET, VAL) \
     _getAttributeArrayDoubleWord(ID, PTARGET, VAL[0][0])
/* global set uint64_t 3D array macro */
#define PLAT_ATTR_SET_UINT64_3D_ARRAY(ID, PTARGET, VAL) \
     _setAttributeArrayDoubleWord(ID, PTARGET, VAL[0][0])

/* global get uint64_t 4D array macro */
#define PLAT_ATTR_GET_UINT64_4D_ARRAY(ID, PTARGET, VAL) \
     _getAttributeArrayDoubleWord(ID, PTARGET, VAL[0][0][0])
/* global set uint64_t 4D array macro */
#define PLAT_ATTR_SET_UINT64_4D_ARRAY(ID, PTARGET, VAL) \
     _setAttributeArrayDoubleWord(ID, PTARGET, VAL[0][0][0])

/* global get int macro (uint8_t, 16, 32 and 64) */
#define PLAT_ATTR_GET_GLOBAL_INT(ID, PTARGET, VAL) \
     _get<ID##_Type, static_cast<fapi2::TargetType>(ID##_TargetType), ID> \
                (ID, PTARGET, VAL)

/* global set int macro (uint8_t, 16, 32 and 64) */
#define PLAT_ATTR_SET_GLOBAL_INT(ID, PTARGET, VAL) \
     _set<ID##_Type, static_cast<fapi2::TargetType>(ID##_TargetType), ID> \
                (ID, PTARGET, VAL)


//here

/******************************************************************************/
// Get Override Macros
/******************************************************************************/
/* global get override uint8_t 1D array macro */
#define PLAT_ATTR_GET_OVERRIDE_UINT8_1D_ARRAY(ID, PTARGET, VAL) \
     _getAttributeOverrideArrayShort(ID, PTARGET, VAL)
/* global get override uint8_t 2D array macro */
#define PLAT_ATTR_GET_OVERRIDE_UINT8_2D_ARRAY(ID, PTARGET, VAL) \
     _getAttributeOverrideArrayShort(ID, PTARGET, VAL[0])
/* global get override uint8_t 3D array macro */
#define PLAT_ATTR_GET_OVERRIDE_UINT8_3D_ARRAY(ID, PTARGET, VAL) \
     _getAttributeOverrideArrayShort(ID, PTARGET, VAL[0][0])
/* global get override uint8_t 4D array macro */
#define PLAT_ATTR_GET_OVERRIDE_UINT8_4D_ARRAY(ID, PTARGET, VAL) \
     _getAttributeOverrideArrayShort(ID, PTARGET, VAL[0][0][0])


/* global get override uint32_t 1D array macro */
#define PLAT_ATTR_GET_OVERRIDE_UINT32_1D_ARRAY(ID, PTARGET, VAL) \
     _getAttributeOverrideArrayWord(ID, PTARGET, VAL)
/* global get override uint32_t 2D array macro */
#define PLAT_ATTR_GET_OVERRIDE_UINT32_2D_ARRAY(ID, PTARGET, VAL) \
     _getAttributeOverrideArrayWord(ID, PTARGET, VAL[0])
/* global get override uint32_t 3D array macro */
#define PLAT_ATTR_GET_OVERRIDE_UINT32_3D_ARRAY(ID, PTARGET, VAL) \
     _getAttributeOverrideArrayWord(ID, PTARGET, VAL[0][0])
/* global get override uint32_t 4D array macro */
#define PLAT_ATTR_GET_OVERRIDE_UINT32_4D_ARRAY(ID, PTARGET, VAL) \
     _getAttributeOverrideArrayWord(ID, PTARGET, VAL[0][0][0])


/* global get override uint64_t 1D array macro */
#define PLAT_ATTR_GET_OVERRIDE_UINT64_1D_ARRAY(ID, PTARGET, VAL) \
     _getAttributeOverrideArrayDoubleWord(ID, PTARGET, VAL)
/* global get override uint64_t 2D array macro */
#define PLAT_ATTR_GET_OVERRIDE_UINT64_2D_ARRAY(ID, PTARGET, VAL) \
     _getAttributeOverrideArrayDoubleWord(ID, PTARGET, VAL[0])
/* global get override uint64_t 3D array macro */
#define PLAT_ATTR_GET_OVERRIDE_UINT64_3D_ARRAY(ID, PTARGET, VAL) \
     _getAttributeOverrideArrayDoubleWord(ID, PTARGET, VAL[0][0])
/* global get override uint64_t 4D array macro */
#define PLAT_ATTR_GET_OVERRIDE_UINT64_4D_ARRAY(ID, PTARGET, VAL) \
     _getAttributeOverrideArrayDoubleWord(ID, PTARGET, VAL[0][0][0])

/* global get override int macro (uint8_t, 32 and 64) */
#define PLAT_ATTR_GET_OVERRIDE_GLOBAL_INT(ID, PTARGET, VAL) \
     _getOverride<ID##_Type>(ID, PTARGET, VAL)

/******************************************************************************/
// Get string
/******************************************************************************/

extern "C"
{
    extern fapi2attr::SystemAttributes_t    G_system_attributes    asm("G_system_attributes")     __attribute__ ((section (".fixed")));
    extern fapi2attr::ProcChipAttributes_t  G_proc_chip_attributes asm("G_proc_chip_attributes")  __attribute__ ((section (".fixed")));
    extern fapi2attr::PervAttributes_t      G_perv_attributes      asm("G_perv_attributes")       __attribute__ ((section (".fixed")));
    extern fapi2attr::CoreAttributes_t      G_core_attributes      asm("G_core_attributes")       __attribute__ ((section (".fixed")));
    extern fapi2attr::EQAttributes_t        G_eq_attributes        asm("G_eq_attributes")         __attribute__ ((section (".fixed")));
    extern fapi2attr::EXAttributes_t        G_ex_attributes        asm("G_ex_attributes")         __attribute__ ((section (".fixed")));

    extern fapi2attr::SystemAttributes_t*   G_system_attributes_ptr;
    extern fapi2attr::ProcChipAttributes_t* G_proc_chip_attributes_ptr;
    extern fapi2attr::PervAttributes_t*     G_perv_attributes_ptr;
    extern fapi2attr::CoreAttributes_t*     G_core_attributes_ptr;
    extern fapi2attr::EQAttributes_t*       G_eq_attributes_ptr;
    extern fapi2attr::EXAttributes_t*       G_ex_attributes_ptr;

}

namespace fapi2
{

// Parameters are done as pointers (vs references) to allow the attribute
// storage to be relocated
template<TargetType K, typename TAttrStruct, typename TValue, AttributeId AId>
void __set( const Target<K>& i_ptarget, TAttrStruct* object, const AttributeId attrid, const TValue& value );

template<TargetType K, typename TAttrStruct, typename TValue, AttributeId AId>
void __get( const Target<K>& i_ptarget, const TAttrStruct* object, const AttributeId attrid, TValue* value );

#define ATTR_CHIP_UNIT_POS_GETMACRO PLAT_ATTR_GET_CHIP_UNIT_POS

#define PLAT_ATTR_GET_CHIP_UNIT_POS(ID, PTARGET, VAL) \
    _getAttrChipUnitPos(PTARGET, VAL)

template<TargetType K>
inline
AttributeRC _getAttrChipUnitPos(const Target<K>& i_target,
        ATTR_CHIP_UNIT_POS_Type& o_val)
{
    o_val = i_target.get().getTargetInstance();
    return FAPI2_RC_SUCCESS;
}

template<>
inline
AttributeRC _getAttrChipUnitPos(const Target<TARGET_TYPE_PERV>& i_target,
        ATTR_CHIP_UNIT_POS_Type& o_val)
{
    o_val = i_target.getChipletNumber();
    return FAPI2_RC_SUCCESS;
}

/* INSERT NEW GETTER AND SETTER FUNCTIONS HERE */



//******************************************************************************
// Get base template
//******************************************************************************
template<typename T, TargetType K, AttributeId A>
AttributeRC _get(const AttributeId i_id,
                const Target<K> & i_pTarget,
                T& o_value)
{
    return FAPI2_RC_SUCCESS;
}

//******************************************************************************
// Get uint8_t
//******************************************************************************
template<typename T, TargetType K, AttributeId A>
AttributeRC _get(const AttributeId i_id,
                const Target<K> & i_pTarget,
                uint8_t& o_value)
{
    static_assert(std::is_same<T, uint8_t>::value, "Attribute type mismatch");

    if(K & TARGET_TYPE_SYSTEM)
    {
        __get<K, fapi2attr::SystemAttributes_t, T, A>( i_pTarget, G_system_attributes_ptr, i_id, &o_value );
    }

    if(K & TARGET_TYPE_PROC_CHIP)
    {
        __get<K, fapi2attr::ProcChipAttributes_t, T, A>( i_pTarget, G_proc_chip_attributes_ptr, i_id, &o_value );
    }

    if(K & TARGET_TYPE_PERV)
    {
         __get<K, fapi2attr::PervAttributes_t, T, A>( i_pTarget, G_perv_attributes_ptr, i_id, &o_value );
    }

    if(K & TARGET_TYPE_CORE)
    {
        __get<K, fapi2attr::CoreAttributes_t, T, A>( i_pTarget, G_core_attributes_ptr, i_id, &o_value );
    }

    if(K & TARGET_TYPE_EQ)
    {
        __get<K, fapi2attr::EQAttributes_t, T, A>( i_pTarget, G_eq_attributes_ptr, i_id, &o_value );
    }

    if(K & TARGET_TYPE_EX)
    {
        __get<K, fapi2attr::EXAttributes_t, T, A>( i_pTarget, G_ex_attributes_ptr, i_id, &o_value );
    }

    return FAPI2_RC_SUCCESS;
}

//******************************************************************************
// Get uint16_t
//******************************************************************************
template<typename T, TargetType K, AttributeId A>
AttributeRC _get(const AttributeId i_id,
                const Target<K> & i_pTarget,
                uint16_t& o_value)
{
    static_assert(std::is_same<T, uint16_t>::value, "Attribute type mismatch");

    if(K & TARGET_TYPE_SYSTEM)
    {
        __get<K, fapi2attr::SystemAttributes_t, T, A>( i_pTarget, G_system_attributes_ptr, i_id, &o_value );
    }

    if(K & TARGET_TYPE_PROC_CHIP)
    {
        __get<K, fapi2attr::ProcChipAttributes_t, T, A>( i_pTarget, G_proc_chip_attributes_ptr, i_id, &o_value );
    }

    if(K & TARGET_TYPE_PERV)
    {
         __get<K, fapi2attr::PervAttributes_t, T, A>( i_pTarget, G_perv_attributes_ptr, i_id, &o_value );
    }

    if(K & TARGET_TYPE_CORE)
    {
        __get<K, fapi2attr::CoreAttributes_t, T, A>( i_pTarget, G_core_attributes_ptr, i_id, &o_value );
    }

    if(K & TARGET_TYPE_EQ)
    {
        __get<K, fapi2attr::EQAttributes_t, T, A>( i_pTarget, G_eq_attributes_ptr, i_id, &o_value );
    }

    if(K & TARGET_TYPE_EX)
    {
        __get<K, fapi2attr::EXAttributes_t, T, A>( i_pTarget, G_ex_attributes_ptr, i_id, &o_value );
    }

    return FAPI2_RC_SUCCESS;
}


//******************************************************************************
// Get uint32_t
//******************************************************************************
template<typename T, TargetType K, AttributeId A>
AttributeRC _get(const AttributeId i_id,
                const Target<K> & i_pTarget,
                uint32_t& o_value)
{
    static_assert(std::is_same<T, uint32_t>::value, "Attribute type mismatch");

    if(K & TARGET_TYPE_SYSTEM)
    {
        __get<K, fapi2attr::SystemAttributes_t, T, A>( i_pTarget, G_system_attributes_ptr, i_id, &o_value );
    }

    if(K & TARGET_TYPE_PROC_CHIP)
    {
        __get<K, fapi2attr::ProcChipAttributes_t, T, A>( i_pTarget, G_proc_chip_attributes_ptr, i_id, &o_value );
    }

    if(K & TARGET_TYPE_PERV)
    {
         __get<K, fapi2attr::PervAttributes_t, T, A>( i_pTarget, G_perv_attributes_ptr, i_id, &o_value );
    }

    if(K & TARGET_TYPE_CORE)
    {
        __get<K, fapi2attr::CoreAttributes_t, T, A>( i_pTarget, G_core_attributes_ptr, i_id, &o_value );
    }

    if(K & TARGET_TYPE_EQ)
    {
        __get<K, fapi2attr::EQAttributes_t, T, A>( i_pTarget, G_eq_attributes_ptr, i_id, &o_value );
    }

    if(K & TARGET_TYPE_EX)
    {
        __get<K, fapi2attr::EXAttributes_t, T, A>( i_pTarget, G_ex_attributes_ptr, i_id, &o_value );
    }

    return FAPI2_RC_SUCCESS;
}


//******************************************************************************
// Get uint64_t
//******************************************************************************
template<typename T, TargetType K, AttributeId A>
AttributeRC _get(const AttributeId i_id,
                const Target<K> & i_pTarget,
                uint64_t& o_value)
{
    static_assert(std::is_same<T, uint64_t>::value, "Attribute type mismatch");

    if(K & TARGET_TYPE_SYSTEM)
    {
        __get<K, fapi2attr::SystemAttributes_t, T, A>( i_pTarget, G_system_attributes_ptr, i_id, &o_value );
    }

    if(K & TARGET_TYPE_PROC_CHIP)
    {
        __get<K, fapi2attr::ProcChipAttributes_t, T, A>( i_pTarget, G_proc_chip_attributes_ptr, i_id, &o_value );
    }

    if(K & TARGET_TYPE_PERV)
    {
         __get<K, fapi2attr::PervAttributes_t, T, A>( i_pTarget, G_perv_attributes_ptr, i_id, &o_value );
    }

    if(K & TARGET_TYPE_CORE)
    {
        __get<K, fapi2attr::CoreAttributes_t, T, A>( i_pTarget, G_core_attributes_ptr, i_id, &o_value );
    }

    if(K & TARGET_TYPE_EQ)
    {
        __get<K, fapi2attr::EQAttributes_t, T, A>( i_pTarget, G_eq_attributes_ptr, i_id, &o_value );
    }

    if(K & TARGET_TYPE_EX)
    {
        __get<K, fapi2attr::EXAttributes_t, T, A>( i_pTarget, G_ex_attributes_ptr, i_id, &o_value );
    }

    return FAPI2_RC_SUCCESS;
}


//******************************************************************************
// Get Override uint8_t
//******************************************************************************
template<typename T, TargetType K, AttributeId A>
AttributeRC _getOverride(const AttributeId i_id,
                        const Target<K> & i_pTarget,
                        uint8_t& o_value)
{
    static_assert(std::is_same<T, uint8_t>::value, "Attribute type mismatch");

    return FAPI2_RC_SUCCESS;
}


//******************************************************************************
// Get Override uint16_t
//******************************************************************************
template<typename T, TargetType K, AttributeId A>
AttributeRC _getOverride(const AttributeId i_id,
                        const Target<K> & i_pTarget,
                        uint16_t& o_value)
{
    static_assert(std::is_same<T, uint16_t>::value, "Attribute type mismatch");

    return FAPI2_RC_SUCCESS;
}


//******************************************************************************
// Get Override uint32_t
//******************************************************************************
template<typename T, TargetType K, AttributeId A>
AttributeRC _getOverride(const AttributeId i_id,
                        const Target<K> & i_pTarget,
                        uint32_t& o_value)
{
    static_assert(std::is_same<T, uint32_t>::value, "Attribute type mismatch");

    return FAPI2_RC_SUCCESS;
}


//******************************************************************************
// Get Override uint64_t
//******************************************************************************
template<typename T, TargetType K, AttributeId A>
AttributeRC _getOverride(const AttributeId i_id,
                        const Target<K> & i_pTarget,
                        uint64_t& o_value)
{
    static_assert(std::is_same<T, uint64_t>::value, "Attribute type mismatch");

    return FAPI2_RC_SUCCESS;
}


//******************************************************************************
// Get override EC Feature (uint8_t)
//******************************************************************************
template<typename T, TargetType K>
AttributeRC _getEcFeatureOverride(const AttributeId i_id,
                                 const Target<K> & i_pTarget,
                                 uint8_t& o_value)
{
    static_assert(std::is_same<T, uint8_t>::value, "Attribute type mismatch");

    // The way this is implemented, we want to return a non-zero return code if we found an override.
    // Return 0 if there was an error.
    // This is how it's implemented:
    // PLAT_GET_CHIP_EC_FEATURE_OVERRIDE(ID, PTARGET, VAL) ? fapi::FAPI_RC_SUCCESS : fapi::fapiQueryChipEcFeature(fapi::ID, PTARGET, VAL)

    return FAPI2_RC_SUCCESS;
}


//******************************************************************************
// Get uint8_t array
//******************************************************************************
template<TargetType K, AttributeId A>
AttributeRC _getAttributeArrayShort(const AttributeId i_id,
                                   const Target<K> & i_pTarget,
                                   uint8_t * o_pValues)
{
    if(K & TARGET_TYPE_SYSTEM)
    {
        __get<K, fapi2attr::SystemAttributes_t, uint8_t, A>( i_pTarget, G_system_attributes_ptr, i_id, o_pValues );
    }

    if(K & TARGET_TYPE_PROC_CHIP)
    {
        __get<K, fapi2attr::ProcChipAttributes_t, uint8_t, A>( i_pTarget, G_proc_chip_attributes_ptr, i_id, o_pValues );
    }

    if(K & TARGET_TYPE_PERV)
    {
        __get<K, fapi2attr::PervAttributes_t, uint8_t, A>( i_pTarget, G_perv_attributes_ptr, i_id, o_pValues );
    }

    if(K & TARGET_TYPE_CORE)
    {
        __get<K, fapi2attr::CoreAttributes_t, uint8_t, A>( i_pTarget, G_core_attributes_ptr, i_id, o_pValues );
    }

    if(K & TARGET_TYPE_EQ)
    {
        __get<K, fapi2attr::EQAttributes_t, uint8_t, A>( i_pTarget, G_eq_attributes_ptr, i_id, o_pValues );
    }

    if(K & TARGET_TYPE_EX)
    {
        __get<K, fapi2attr::EXAttributes_t, uint8_t, A>( i_pTarget, G_ex_attributes_ptr, i_id, o_pValues );
    }

    return FAPI2_RC_SUCCESS;
}

//******************************************************************************
// Set uint8_t array
//******************************************************************************
template<TargetType K, AttributeId A>
AttributeRC _setAttributeArrayShort(const AttributeId i_id,
                                   const Target<K> & i_pTarget,
                                   uint8_t * i_pValues)
{
    if(K & TARGET_TYPE_SYSTEM)
    {
        __set<K, fapi2attr::SystemAttributes_t, uint8_t, A>( i_pTarget, G_system_attributes_ptr, i_id, *i_pValues );
    }

    if(K & TARGET_TYPE_PROC_CHIP)
    {
        __set<K, fapi2attr::ProcChipAttributes_t, uint8_t, A>( i_pTarget, G_proc_chip_attributes_ptr, i_id, *i_pValues );
    }

    if(K & TARGET_TYPE_PERV)
    {
        __set<K, fapi2attr::PervAttributes_t, uint8_t, A>( i_pTarget, G_perv_attributes_ptr, i_id, *i_pValues );
    }

    if(K & TARGET_TYPE_CORE)
    {
        __set<K, fapi2attr::CoreAttributes_t, uint8_t, A>( i_pTarget, G_core_attributes_ptr, i_id, *i_pValues );
    }

    if(K & TARGET_TYPE_EQ)
    {
        __set<K, fapi2attr::EQAttributes_t, uint8_t, A>( i_pTarget, G_eq_attributes_ptr, i_id, *i_pValues );
    }

    if(K & TARGET_TYPE_EX)
    {
        __set<K, fapi2attr::EXAttributes_t, uint8_t, A>( i_pTarget, G_ex_attributes_ptr, i_id, *i_pValues );
    }

    return FAPI2_RC_SUCCESS;
}

//******************************************************************************
// Get uint16_t array
//******************************************************************************
template<typename T, TargetType K, AttributeId A>
AttributeRC _getAttributeArrayWord(const AttributeId i_id,
                                  const Target<K> & i_pTarget,
                                  uint16_t * o_pValues)
{
    return FAPI2_RC_SUCCESS;
}

//******************************************************************************
// Set uint16_t array
//******************************************************************************
template<typename T, TargetType K, AttributeId A>
AttributeRC _setAttributeArrayWord(const AttributeId i_id,
                                  const Target<K> & i_pTarget,
                                  uint16_t * i_pValues)
{
    return FAPI2_RC_SUCCESS;
}

//******************************************************************************
// Get uint32_t array
//******************************************************************************
template<typename T, TargetType K, AttributeId A>
AttributeRC _getAttributeArrayWord(const AttributeId i_id,
                                  const Target<K> & i_pTarget,
                                  uint32_t * o_pValues)
{
    return FAPI2_RC_SUCCESS;
}

//******************************************************************************
// Set uint32_t array
//******************************************************************************
template<typename T, TargetType K, AttributeId A>
AttributeRC _setAttributeArrayWord(const AttributeId i_id,
                                  const Target<K> & i_pTarget,
                                  uint32_t * i_pValues)
{
    return FAPI2_RC_SUCCESS;
}


//******************************************************************************
// Get uint64_t array
//******************************************************************************
template<TargetType K, AttributeId A>
AttributeRC _getAttributeArrayDoubleWord(const AttributeId i_id,
                                        const Target<K> & i_pTarget,
                                        uint64_t * o_pValues)
{
    if(K & TARGET_TYPE_SYSTEM)
    {
        __get<K, fapi2attr::SystemAttributes_t, uint64_t, A>( i_pTarget, G_system_attributes_ptr, i_id, o_pValues );
    }

    if(K & TARGET_TYPE_PROC_CHIP)
    {
        __get<K, fapi2attr::ProcChipAttributes_t, uint64_t, A>( i_pTarget, G_proc_chip_attributes_ptr, i_id, o_pValues );
    }

    if(K & TARGET_TYPE_PERV)
    {
        __get<K, fapi2attr::PervAttributes_t, uint64_t, A>( i_pTarget, G_perv_attributes_ptr, i_id, o_pValues );
    }

    if(K & TARGET_TYPE_CORE)
    {
        __get<K, fapi2attr::CoreAttributes_t, uint64_t, A>( i_pTarget, G_core_attributes_ptr, i_id, o_pValues );
    }

    if(K & TARGET_TYPE_EQ)
    {
        __get<K, fapi2attr::EQAttributes_t, uint64_t, A>( i_pTarget, G_eq_attributes_ptr, i_id, o_pValues );
    }

    if(K & TARGET_TYPE_EX)
    {
        __get<K, fapi2attr::EXAttributes_t, uint64_t, A>( i_pTarget, G_ex_attributes_ptr, i_id, o_pValues );
    }

    return FAPI2_RC_SUCCESS;
}

//******************************************************************************
// Set uint64_t array
//******************************************************************************
template<TargetType K, AttributeId A>
AttributeRC _setAttributeArrayDoubleWord(const AttributeId i_id,
                                        const Target<K> & i_pTarget,
                                        uint64_t * i_pValues)
{
    if(K & TARGET_TYPE_SYSTEM)
    {
        __set<K, fapi2attr::SystemAttributes_t, uint64_t, A>( i_pTarget, G_system_attributes_ptr, i_id, *i_pValues );
    }

    if(K & TARGET_TYPE_PROC_CHIP)
    {
        __set<K, fapi2attr::ProcChipAttributes_t, uint64_t, A>( i_pTarget, G_proc_chip_attributes_ptr, i_id, *i_pValues );
    }

    if(K & TARGET_TYPE_PERV)
    {
        __set<K, fapi2attr::PervAttributes_t, uint64_t, A>( i_pTarget, G_perv_attributes_ptr, i_id, *i_pValues );
    }

    if(K & TARGET_TYPE_CORE)
    {
        __set<K, fapi2attr::CoreAttributes_t, uint64_t, A>( i_pTarget, G_core_attributes_ptr, i_id, *i_pValues );
    }

    if(K & TARGET_TYPE_EQ)
    {
        __set<K, fapi2attr::EQAttributes_t, uint64_t, A>( i_pTarget, G_eq_attributes_ptr, i_id, *i_pValues );
    }

    if(K & TARGET_TYPE_EX)
    {
        __set<K, fapi2attr::EXAttributes_t, uint64_t, A>( i_pTarget, G_ex_attributes_ptr, i_id, *i_pValues );
    }

    return FAPI2_RC_SUCCESS;
}

//******************************************************************************
// Get Override uint8_t array
//******************************************************************************
template<typename T, TargetType K, AttributeId A>
AttributeRC _getAttributeOverrideArrayShort(const AttributeId i_id,
                                           const Target<K> & i_pTarget,
                                           uint8_t * o_pValues)
{
    return FAPI2_RC_SUCCESS;
}

//******************************************************************************
// Get Override uint16_t array
//******************************************************************************
template<typename T, TargetType K, AttributeId A>
AttributeRC _getAttributeOverrideArrayWord(const AttributeId i_id,
                                          const Target<K> & i_pTarget,
                                          uint16_t * o_pValues)
{
    return FAPI2_RC_SUCCESS;
}

//******************************************************************************
// Get Override uint32_t array
//******************************************************************************
template<typename T, TargetType K, AttributeId A>
AttributeRC _getAttributeOverrideArrayWord(const AttributeId i_id,
                                          const Target<K> & i_pTarget,
                                          uint32_t * o_pValues)
{
    return FAPI2_RC_SUCCESS;
}

//******************************************************************************
// Get Override uint64_t array
//******************************************************************************
template<typename T, TargetType K, AttributeId A>
AttributeRC _getAttributeOverrideArrayDoubleWord(const AttributeId i_id,
                                                const Target<K> & i_pTarget,
                                                uint64_t * o_pValues)
{
    return FAPI2_RC_SUCCESS;
}


//******************************************************************************
// Set base template
//******************************************************************************
template<typename T, TargetType K, AttributeId A>
AttributeRC _set(const AttributeId i_id,
                const Target<K> & i_pTarget,
                const T& i_value)
{
    return FAPI2_RC_SUCCESS;
}



//******************************************************************************
// Set uint8_t
//******************************************************************************
template<typename T, TargetType K, AttributeId A>
AttributeRC _set(const AttributeId i_id,
                const Target<K> & i_pTarget,
                const uint8_t& i_value)
{
    static_assert(std::is_same<T, uint8_t>::value, "Attribute type mismatch"); // May need to remove

    if(K & TARGET_TYPE_SYSTEM)
    {
        __set<K, fapi2attr::SystemAttributes_t, T, A>( i_pTarget, G_system_attributes_ptr, i_id, i_value );
    }

    if(K & TARGET_TYPE_PROC_CHIP)
    {
        __set<K, fapi2attr::ProcChipAttributes_t, T, A>( i_pTarget, G_proc_chip_attributes_ptr, i_id, i_value );
    }

    if(K & TARGET_TYPE_PERV)
    {
        __set<K, fapi2attr::PervAttributes_t, T, A>( i_pTarget, G_perv_attributes_ptr, i_id, i_value );
    }

    if(K & TARGET_TYPE_CORE)
    {
        __set<K, fapi2attr::CoreAttributes_t, T, A>( i_pTarget, G_core_attributes_ptr, i_id, i_value );
    }

    if(K & TARGET_TYPE_EQ)
    {
        __set<K, fapi2attr::EQAttributes_t, T, A>( i_pTarget, G_eq_attributes_ptr, i_id, i_value );
    }

    if(K & TARGET_TYPE_EX)
    {
        __set<K, fapi2attr::EXAttributes_t, T, A>( i_pTarget, G_ex_attributes_ptr, i_id, i_value );
    }

    return FAPI2_RC_SUCCESS;
}


//******************************************************************************
// Set uint16_t
//******************************************************************************
template<typename T, TargetType K, AttributeId A>
AttributeRC _set(const AttributeId i_id,
                const Target<K> & i_pTarget,
                const uint16_t& i_value)
{
    static_assert(std::is_same<T, uint16_t>::value, "Attribute type mismatch"); // May need to remove

    if(K & TARGET_TYPE_SYSTEM)
    {
        __set<K, fapi2attr::SystemAttributes_t, T, A>( i_pTarget, G_system_attributes_ptr, i_id, i_value );
    }

    if(K & TARGET_TYPE_PROC_CHIP)
    {
        __set<K, fapi2attr::ProcChipAttributes_t, T, A>( i_pTarget, G_proc_chip_attributes_ptr, i_id, i_value );
    }

    if(K & TARGET_TYPE_PERV)
    {
        __set<K, fapi2attr::PervAttributes_t, T, A>( i_pTarget, G_perv_attributes_ptr, i_id, i_value );
    }

    if(K & TARGET_TYPE_CORE)
    {
        __set<K, fapi2attr::CoreAttributes_t, T, A>( i_pTarget, G_core_attributes_ptr, i_id, i_value );
    }

    if(K & TARGET_TYPE_EQ)
    {
        __set<K, fapi2attr::EQAttributes_t, T, A>( i_pTarget, G_eq_attributes_ptr, i_id, i_value );
    }

    if(K & TARGET_TYPE_EX)
    {
        __set<K, fapi2attr::EXAttributes_t, T, A>( i_pTarget, G_ex_attributes_ptr, i_id, i_value );
    }

    return FAPI2_RC_SUCCESS;
}

//******************************************************************************
// Set uint32_t
//******************************************************************************
template<typename T, TargetType K, AttributeId A>
AttributeRC _set(const AttributeId i_id,
                const Target<K> & i_pTarget,
                const uint32_t& i_value)
{
    static_assert(std::is_same<T, uint32_t>::value, "Attribute type mismatch"); // May need to remove

    if(K & TARGET_TYPE_SYSTEM)
    {
        __set<K, fapi2attr::SystemAttributes_t, T, A>( i_pTarget, G_system_attributes_ptr, i_id, i_value );
    }

    if(K & TARGET_TYPE_PROC_CHIP)
    {
        __set<K, fapi2attr::ProcChipAttributes_t, T, A>( i_pTarget, G_proc_chip_attributes_ptr, i_id, i_value );
    }

    if(K & TARGET_TYPE_PERV)
    {
        __set<K, fapi2attr::PervAttributes_t, T, A>( i_pTarget, G_perv_attributes_ptr, i_id, i_value );
    }

    if(K & TARGET_TYPE_CORE)
    {
        __set<K, fapi2attr::CoreAttributes_t, T, A>( i_pTarget, G_core_attributes_ptr, i_id, i_value );
    }

    if(K & TARGET_TYPE_EQ)
    {
        __set<K, fapi2attr::EQAttributes_t, T, A>( i_pTarget, G_eq_attributes_ptr, i_id,i_value );
    }

    if(K & TARGET_TYPE_EX)
    {
        __set<K, fapi2attr::EXAttributes_t, T, A>( i_pTarget, G_ex_attributes_ptr, i_id, i_value );
    }

    return FAPI2_RC_SUCCESS;
}


//******************************************************************************
// Set uint64_t
//******************************************************************************

template<typename T, TargetType K, AttributeId A>
AttributeRC _set(const AttributeId i_id,
                const Target<K> & i_pTarget,
                const uint64_t & i_value)
{
    static_assert(std::is_same<T, uint64_t>::value, "Attribute type mismatch"); // May need to remove

    if(K & TARGET_TYPE_SYSTEM)
    {
        __set<K, fapi2attr::SystemAttributes_t, T, A>( i_pTarget, G_system_attributes_ptr, i_id, i_value );
    }

    if(K & TARGET_TYPE_PROC_CHIP)
    {
        __set<K, fapi2attr::ProcChipAttributes_t, T, A>( i_pTarget, G_proc_chip_attributes_ptr, i_id, i_value );
    }

    if(K & TARGET_TYPE_PERV)
    {
        __set<K, fapi2attr::PervAttributes_t, T, A>( i_pTarget, G_perv_attributes_ptr, i_id, i_value );
    }

    if(K & TARGET_TYPE_CORE)
    {
        __set<K, fapi2attr::CoreAttributes_t, T, A>( i_pTarget, G_core_attributes_ptr, i_id, i_value );
    }

    if(K & TARGET_TYPE_EQ)
    {
        __set<K, fapi2attr::EQAttributes_t, T, A>( i_pTarget, G_eq_attributes_ptr, i_id, i_value );
    }

    if(K & TARGET_TYPE_EX)
    {
        __set<K, fapi2attr::EXAttributes_t, T, A>( i_pTarget, G_ex_attributes_ptr, i_id, i_value );
    }

    return FAPI2_RC_SUCCESS;
}


//******************************************************************************
// Get int8_t
//******************************************************************************
template<typename T, TargetType K, AttributeId A>
AttributeRC _get(const AttributeId i_id,
                const Target<K> & i_pTarget,
                int8_t& o_value)
{
    static_assert(std::is_same<T, int8_t>::value, "Attribute type mismatch");

    if(K & TARGET_TYPE_SYSTEM)
    {
        __get<K, fapi2attr::SystemAttributes_t, T, A>( i_pTarget, G_system_attributes_ptr, i_id, &o_value );
    }

    if(K & TARGET_TYPE_PROC_CHIP)
    {
        __get<K, fapi2attr::ProcChipAttributes_t, T, A>( i_pTarget, G_proc_chip_attributes_ptr, i_id, &o_value );
    }

    if(K & TARGET_TYPE_PERV)
    {
         __get<K, fapi2attr::PervAttributes_t, T, A>( i_pTarget, G_perv_attributes_ptr, i_id, &o_value );
    }

    if(K & TARGET_TYPE_CORE)
    {
        __get<K, fapi2attr::CoreAttributes_t, T, A>( i_pTarget, G_core_attributes_ptr, i_id, &o_value );
    }

    if(K & TARGET_TYPE_EQ)
    {
        __get<K, fapi2attr::EQAttributes_t, T, A>( i_pTarget, G_eq_attributes_ptr, i_id, &o_value );
    }

    if(K & TARGET_TYPE_EX)
    {
        __get<K, fapi2attr::EXAttributes_t, T, A>( i_pTarget, G_ex_attributes_ptr, i_id, &o_value );
    }

    return FAPI2_RC_SUCCESS;
}

//******************************************************************************
// Get int16_t
//******************************************************************************
template<typename T, TargetType K, AttributeId A>
AttributeRC _get(const AttributeId i_id,
                const Target<K> & i_pTarget,
                int16_t& o_value)
{
    static_assert(std::is_same<T, int16_t>::value, "Attribute type mismatch");

    if(K & TARGET_TYPE_SYSTEM)
    {
        __get<K, fapi2attr::SystemAttributes_t, T, A>( i_pTarget, G_system_attributes_ptr, i_id, &o_value );
    }

    if(K & TARGET_TYPE_PROC_CHIP)
    {
        __get<K, fapi2attr::ProcChipAttributes_t, T, A>( i_pTarget, G_proc_chip_attributes_ptr, i_id, &o_value );
    }

    if(K & TARGET_TYPE_PERV)
    {
         __get<K, fapi2attr::PervAttributes_t, T, A>( i_pTarget, G_perv_attributes_ptr, i_id, &o_value );
    }

    if(K & TARGET_TYPE_CORE)
    {
        __get<K, fapi2attr::CoreAttributes_t, T, A>( i_pTarget, G_core_attributes_ptr, i_id, &o_value );
    }

    if(K & TARGET_TYPE_EQ)
    {
        __get<K, fapi2attr::EQAttributes_t, T, A>( i_pTarget, G_eq_attributes_ptr, i_id, &o_value );
    }

    if(K & TARGET_TYPE_EX)
    {
        __get<K, fapi2attr::EXAttributes_t, T, A>( i_pTarget, G_ex_attributes_ptr, i_id, &o_value );
    }

    return FAPI2_RC_SUCCESS;
}


//******************************************************************************
// Get int32_t
//******************************************************************************
template<typename T, TargetType K, AttributeId A>
AttributeRC _get(const AttributeId i_id,
                const Target<K> & i_pTarget,
                int32_t& o_value)
{
    static_assert(std::is_same<T, int32_t>::value, "Attribute type mismatch");

    if(K & TARGET_TYPE_SYSTEM)
    {
        __get<K, fapi2attr::SystemAttributes_t, T, A>( i_pTarget, G_system_attributes_ptr, i_id, &o_value );
    }

    if(K & TARGET_TYPE_PROC_CHIP)
    {
        __get<K, fapi2attr::ProcChipAttributes_t, T, A>( i_pTarget, G_proc_chip_attributes_ptr, i_id, &o_value );
    }

    if(K & TARGET_TYPE_PERV)
    {
         __get<K, fapi2attr::PervAttributes_t, T, A>( i_pTarget, G_perv_attributes_ptr, i_id, &o_value );
    }

    if(K & TARGET_TYPE_CORE)
    {
        __get<K, fapi2attr::CoreAttributes_t, T, A>( i_pTarget, G_core_attributes_ptr, i_id, &o_value );
    }

    if(K & TARGET_TYPE_EQ)
    {
        __get<K, fapi2attr::EQAttributes_t, T, A>( i_pTarget, G_eq_attributes_ptr, i_id, &o_value );
    }

    if(K & TARGET_TYPE_EX)
    {
        __get<K, fapi2attr::EXAttributes_t, T, A>( i_pTarget, G_ex_attributes_ptr, i_id, &o_value );
    }

    return FAPI2_RC_SUCCESS;
}


//******************************************************************************
// Get int64_t
//******************************************************************************
template<typename T, TargetType K, AttributeId A>
AttributeRC _get(const AttributeId i_id,
                const Target<K> & i_pTarget,
                int64_t& o_value)
{
    static_assert(std::is_same<T, int64_t>::value, "Attribute type mismatch");

    if(K & TARGET_TYPE_SYSTEM)
    {
        __set<K, fapi2attr::SystemAttributes_t, T, A>( i_pTarget, G_system_attributes_ptr, i_id, &o_value );
    }

    if(K & TARGET_TYPE_PROC_CHIP)
    {
        __get<K, fapi2attr::ProcChipAttributes_t, T, A>( i_pTarget, G_proc_chip_attributes_ptr, i_id, &o_value );
    }

    if(K & TARGET_TYPE_PERV)
    {
         __get<K, fapi2attr::PervAttributes_t, T, A>( i_pTarget, G_perv_attributes_ptr, i_id, &o_value );
    }

    if(K & TARGET_TYPE_CORE)
    {
        __get<K, fapi2attr::CoreAttributes_t, T, A>( i_pTarget, G_core_attributes_ptr, i_id, &o_value );
    }

    if(K & TARGET_TYPE_EQ)
    {
        __get<K, fapi2attr::EQAttributes_t, T, A>( i_pTarget, G_eq_attributes_ptr, i_id, &o_value );
    }

    if(K & TARGET_TYPE_EX)
    {
        __get<K, fapi2attr::EXAttributes_t, T, A>( i_pTarget, G_ex_attributes_ptr, i_id, &o_value );
    }

    return FAPI2_RC_SUCCESS;
}


//******************************************************************************
// Get Override int8_t
//******************************************************************************
template<typename T, TargetType K, AttributeId A>
AttributeRC _getOverride(const AttributeId i_id,
                        const Target<K> & i_pTarget,
                        int8_t& o_value)
{
    static_assert(std::is_same<T, int8_t>::value, "Attribute type mismatch");

    return FAPI2_RC_SUCCESS;
}

//******************************************************************************
// Get Override int16_t
//******************************************************************************
template<typename T, TargetType K, AttributeId A>
AttributeRC _getOverride(const AttributeId i_id,
                        const Target<K> & i_pTarget,
                        int16_t& o_value)
{
    static_assert(std::is_same<T, int16_t>::value, "Attribute type mismatch");

    return FAPI2_RC_SUCCESS;
}

//******************************************************************************
// Get Override int32_t
//******************************************************************************
template<typename T, TargetType K, AttributeId A>
AttributeRC _getOverride(const AttributeId i_id,
                        const Target<K> & i_pTarget,
                        int32_t& o_value)
{
    static_assert(std::is_same<T, int32_t>::value, "Attribute type mismatch");

    return FAPI2_RC_SUCCESS;
}

//******************************************************************************
// Get Override int64_t
//******************************************************************************
template<typename T, TargetType K, AttributeId A>
AttributeRC _getOverride(const AttributeId i_id,
                        const Target<K> & i_pTarget,
                        int64_t& o_value)
{
    static_assert(std::is_same<T, int64_t>::value, "Attribute type mismatch");

    return FAPI2_RC_SUCCESS;
}


//******************************************************************************
// Get int8_t array
//******************************************************************************
template<TargetType K>
AttributeRC _getAttributeArraySignedShort(const AttributeId i_id,
                                         const Target<K> & i_pTarget,
                                         int8_t * o_pValues)
{
    return FAPI2_RC_SUCCESS;
}


//******************************************************************************
// Set int8_t array
//******************************************************************************
template<TargetType K>
AttributeRC _setAttributeArraySignedShort(const AttributeId i_id,
                                         const Target<K> & i_pTarget,
                                         int8_t * i_pValues)
{
    return FAPI2_RC_SUCCESS;
}

//******************************************************************************
// Get int16_t array
//******************************************************************************
template<TargetType K>
AttributeRC _getAttributeArraySignedWord(const AttributeId i_id,
                                        const Target<K> & i_pTarget,
                                        int16_t * o_pValues)
{
    return FAPI2_RC_SUCCESS;
}


//******************************************************************************
// Set int16_t array
//******************************************************************************
template<TargetType K>
AttributeRC _setAttributeArraySignedWord(const AttributeId i_id,
                                        const Target<K> & i_pTarget,
                                        int16_t * i_pValues)
{
    return FAPI2_RC_SUCCESS;
}

//******************************************************************************
// Get int32_t array
//******************************************************************************
template<TargetType K>
AttributeRC _getAttributeArraySignedWord(const AttributeId i_id,
                                        const Target<K> & i_pTarget,
                                        int32_t * o_pValues)
{
    return FAPI2_RC_SUCCESS;
}


//******************************************************************************
// Set int32_t array
//******************************************************************************
template<TargetType K>
AttributeRC _setAttributeArraySignedWord(const AttributeId i_id,
                                        const Target<K> & i_pTarget,
                                        int32_t * i_pValues)
{
    return FAPI2_RC_SUCCESS;
}


//******************************************************************************
// Get int64_t array
//******************************************************************************
template<TargetType K>
AttributeRC _getAttributeArraySignedDoubleWord(const AttributeId i_id,
                                              const Target<K> & i_pTarget,
                                              int64_t * o_pValues)
{
    return FAPI2_RC_SUCCESS;
}

//******************************************************************************
// Set int64_t array
//******************************************************************************
template<TargetType K>
AttributeRC _setAttributeArraySignedDoubleWord(const AttributeId i_id,
                                              const Target<K> & i_pTarget,
                                              int64_t * i_pValues)
{
    return FAPI2_RC_SUCCESS;
}

//******************************************************************************
// Get Override int8_t array
//******************************************************************************
template<TargetType K>
AttributeRC _getAttributeOverrideArraySignedShort(const AttributeId i_id,
                                                 const Target<K> & i_pTarget,
                                                 int8_t * o_pValues)
{
    return FAPI2_RC_SUCCESS;
}

//******************************************************************************
// Get Override int16_t array
//******************************************************************************
template<TargetType K>
AttributeRC _getAttributeOverrideArraySignedWord(const AttributeId i_id,
                                                const Target<K> & i_pTarget,
                                                int16_t * o_pValues)
{
    return FAPI2_RC_SUCCESS;
}

//******************************************************************************
// Get Override int32_t array
//******************************************************************************
template<TargetType K>
AttributeRC _getAttributeOverrideArraySignedWord(const AttributeId i_id,
                                                const Target<K> & i_pTarget,
                                                int32_t * o_pValues)
{
    return FAPI2_RC_SUCCESS;
}


//******************************************************************************
// Get Override int64_t array
//******************************************************************************
template<TargetType K>
AttributeRC _getAttributeOverrideArraySignedDoubleWord(const AttributeId i_id,
                                                      const Target<K> & i_pTarget,
                                                      int64_t * o_pValues)
{
    return FAPI2_RC_SUCCESS;
}


//******************************************************************************
// Set int8_t
//******************************************************************************
template<typename T, TargetType K, AttributeId A>
AttributeRC _set(const AttributeId i_id,
                const Target<K> & i_pTarget,
                const int8_t& i_value)
{
    static_assert(std::is_same<T, int8_t>::value, "Attribute type mismatch"); // May need to remove

    if(K & TARGET_TYPE_SYSTEM)
    {
        __set<K, fapi2attr::SystemAttributes_t, T, A>( i_pTarget, G_system_attributes_ptr, i_id, i_value );
    }

    if(K & TARGET_TYPE_PROC_CHIP)
    {
        __set<K, fapi2attr::ProcChipAttributes_t, T, A>( i_pTarget, G_proc_chip_attributes_ptr, i_id, i_value );
    }

    if(K & TARGET_TYPE_PERV)
    {
        __set<K, fapi2attr::PervAttributes_t, T, A>( i_pTarget, G_perv_attributes_ptr, i_id, i_value );
    }

    if(K & TARGET_TYPE_CORE)
    {
        __set<K, fapi2attr::CoreAttributes_t, T, A>( i_pTarget, G_core_attributes_ptr, i_id, i_value );
    }

    if(K & TARGET_TYPE_EQ)
    {
        __set<K, fapi2attr::EQAttributes_t, T, A>( i_pTarget, G_eq_attributes_ptr, i_id, i_value );
    }

    if(K & TARGET_TYPE_EX)
    {
        __set<K, fapi2attr::EXAttributes_t, T, A>( i_pTarget, G_ex_attributes_ptr, i_id, i_value );
    }

    return FAPI2_RC_SUCCESS;
}

//******************************************************************************
// Set int16_t
//******************************************************************************
template<typename T, TargetType K, AttributeId A>
AttributeRC _set(const AttributeId i_id,
                const Target<K> & i_pTarget,
                const int16_t& i_value)
{
    static_assert(std::is_same<T, int16_t>::value, "Attribute type mismatch"); // May need to remove

    if(K & TARGET_TYPE_SYSTEM)
    {
        __set<K, fapi2attr::SystemAttributes_t, T, A>( i_pTarget, G_system_attributes_ptr, i_id, i_value );
    }

    if(K & TARGET_TYPE_PROC_CHIP)
    {
        __set<K, fapi2attr::CoreAttributes_t, T, A>( i_pTarget, G_proc_chip_attributes_ptr, i_id, i_value );
    }

    if(K & TARGET_TYPE_PERV)
    {
        __set<K, fapi2attr::ProcChipAttributes_t, T, A>( i_pTarget, G_perv_attributes_ptr, i_id, i_value );
    }

    if(K & TARGET_TYPE_CORE)
    {
        __set<K, fapi2attr::CoreAttributes_t, T, A>( i_pTarget, G_core_attributes_ptr, i_id, i_value );
    }

    if(K & TARGET_TYPE_EQ)
    {
        __set<K, fapi2attr::EQAttributes_t, T, A>( i_pTarget, G_eq_attributes_ptr, i_id, i_value );
    }

    if(K & TARGET_TYPE_EX)
    {
        __set<K, fapi2attr::EXAttributes_t, T, A>( i_pTarget, G_ex_attributes_ptr, i_id, i_value );
    }

    return FAPI2_RC_SUCCESS;
}

//******************************************************************************
// Set int32_t
//******************************************************************************
template<typename T, TargetType K, AttributeId A>
AttributeRC _set(const AttributeId i_id,
                const Target<K> & i_pTarget,
                const int32_t& i_value)
{
    static_assert(std::is_same<T, int32_t>::value, "Attribute type mismatch"); // May need to remove

    if(K & TARGET_TYPE_SYSTEM)
    {
        __set<K, fapi2attr::SystemAttributes_t, T, A>( i_pTarget, G_system_attributes_ptr, i_id, i_value );
    }

    if(K & TARGET_TYPE_PROC_CHIP)
    {
        __set<K, fapi2attr::ProcChipAttributes_t, T, A>( i_pTarget, G_proc_chip_attributes_ptr, i_id, i_value );
    }

    if(K & TARGET_TYPE_PERV)
    {
        __set<K, fapi2attr::PervAttributes_t, T, A>( i_pTarget, G_perv_attributes_ptr, i_id, i_value );
    }

    if(K & TARGET_TYPE_CORE)
    {
        __set<K, fapi2attr::CoreAttributes_t, T, A>( i_pTarget, G_core_attributes_ptr, i_id, i_value );
    }

    if(K & TARGET_TYPE_EQ)
    {
        __set<K, fapi2attr::EQAttributes_t, T, A>( i_pTarget, G_eq_attributes_ptr, i_id, i_value );
    }

    if(K & TARGET_TYPE_EX)
    {
        __set<K, fapi2attr::EXAttributes_t, T, A>( i_pTarget, G_ex_attributes_ptr, i_id, i_value );
    }

    return FAPI2_RC_SUCCESS;
}


//******************************************************************************
// Set int64_t
//******************************************************************************
template<typename T, TargetType K, AttributeId A>
AttributeRC _set(const AttributeId i_id,
                const Target<K> & i_pTarget,
                const int64_t & i_value)
{
    static_assert(std::is_same<T, int64_t>::value, "Attribute type mismatch"); // May need to remove

    if(K & TARGET_TYPE_SYSTEM)
    {
        __set<K, fapi2attr::SystemAttributes_t, T, A>( i_pTarget, G_system_attributes_ptr, i_id, i_value );
    }

    if(K & TARGET_TYPE_PROC_CHIP)
    {
        __set<K, fapi2attr::ProcChipAttributes_t, T, A>( i_pTarget, G_proc_chip_attributes_ptr, i_id, i_value );
    }

    if(K & TARGET_TYPE_PERV)
    {
        __set<K, fapi2attr::PervAttributes_t, T, A>( i_pTarget, G_perv_attributes_ptr, i_id, i_value );
    }

    if(K & TARGET_TYPE_CORE)
    {
        __set<K, fapi2attr::CoreAttributes_t, T, A>( i_pTarget, G_core_attributes_ptr, i_id, i_value );
    }

    if(K & TARGET_TYPE_EQ)
    {
        __set<K, fapi2attr::EQAttributes_t, T, A>( i_pTarget, G_eq_attributes_ptr, i_id, i_value );
    }

    if(K & TARGET_TYPE_EX)
    {
        __set<K, fapi2attr::EXAttributes_t, T, A>( i_pTarget, G_ex_attributes_ptr, i_id, i_value );
    }

    return FAPI2_RC_SUCCESS;
}


} // namespace fapi2
#endif // FAPIPLATATTRIBUTESERVICE_H_

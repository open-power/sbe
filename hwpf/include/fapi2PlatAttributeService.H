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
#include <fapi2AttributeIds.H>
#include <plat_includes.H>
#include "proc_sbe_fixed.H"
#include "plat_target_parms.H"

#define PLAT_GET_CHIP_EC_FEATURE_OVERRIDE(ID, PTARGET, VAL) \
      _getEcFeatureOverride<ID##_Type>(ID, PTARGET, VAL)

/* INSERT NEW ATTRIBUTES HERE */

#define ATTR_BACKUP_SEEPROM_SELECT_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_CHIP_UNIT_POS_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_CORE_GPTR_RING_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_CORE_GPTR_RING_SETMACRO PLAT_ATTR_SET_GLOBAL_INT
#define ATTR_CORE_REPR_RING_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_CORE_REPR_RING_SETMACRO PLAT_ATTR_SET_GLOBAL_INT
#define ATTR_CORE_TIME_RING_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_CORE_TIME_RING_SETMACRO PLAT_ATTR_SET_GLOBAL_INT
#define ATTR_DPLL_RING_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_FABRIC_SYSTEM_ID_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_GPTR_RING_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_L2_GPTR_RING_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_L2_REPR_RING_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_L2_TIME_RING_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_L3_GPTR_RING_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_L3_REPR_RING_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_L3_TIME_RING_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_MC_SYNC_MODE_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_PG_EC00_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_PG_EC01_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_PG_EC02_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_PG_EC03_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_PG_EC04_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_PG_EC05_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_PG_EC06_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_PG_EC07_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_PG_EC08_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_PG_EC09_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_PG_EC10_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_PG_EC11_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_PG_EC12_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_PG_EC13_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_PG_EC14_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_PG_EC15_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_PG_EC16_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_PG_EC17_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_PG_EC18_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_PG_EC19_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_PG_EC20_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_PG_EC21_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_PG_EC22_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_PG_EC23_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_PG_EQ0_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_PG_EQ1_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_PG_EQ2_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_PG_EQ3_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_PG_EQ4_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_PG_EQ5_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_PG_FSI_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_PG_MC01_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_PG_MC23_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_PG_N0_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_PG_N1_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_PG_N2_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_PG_N3_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_PG_OB0_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_PG_OB1_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_PG_OB2_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_PG_OB3_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_PG_PCI0_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_PG_PCI1_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_PG_PCI2_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_PG_PRV_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_PG_XB_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_PLL_RING_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_REPR_RING_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_SCRATCH_INT32_1_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_SCRATCH_INT32_1_SETMACRO PLAT_ATTR_SET_GLOBAL_INT
#define ATTR_SCRATCH_INT32_2_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_SCRATCH_INT32_2_SETMACRO PLAT_ATTR_SET_GLOBAL_INT
#define ATTR_SCRATCH_INT64_1_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_SCRATCH_INT64_2_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_SCRATCH_INT8_1_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_SCRATCH_INT8_2_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_SCRATCH_UINT32_1_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_SCRATCH_UINT32_1_SETMACRO PLAT_ATTR_SET_GLOBAL_INT
#define ATTR_SCRATCH_UINT32_2_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_SCRATCH_UINT32_2_SETMACRO PLAT_ATTR_SET_GLOBAL_INT
#define ATTR_SCRATCH_UINT32_ARRAY_GETMACRO PLAT_ATTR_GET_UINT32_1D_ARRAY
#define ATTR_SCRATCH_UINT32_ARRAY_SETMACRO PLAT_ATTR_SET_UINT32_1D_ARRAY
#define ATTR_SCRATCH_UINT32_PERV_ARRAY_GETMACRO PLAT_ATTR_GET_UINT32_1D_ARRAY
#define ATTR_SCRATCH_UINT32_PERV_ARRAY_SETMACRO PLAT_ATTR_SET_UINT32_1D_ARRAY
#define ATTR_SCRATCH_UINT64_1_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_SCRATCH_UINT64_1_SETMACRO PLAT_ATTR_SET_GLOBAL_INT
#define ATTR_SCRATCH_UINT64_2_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_SCRATCH_UINT8_1_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_SCRATCH_UINT8_1_SETMACRO PLAT_ATTR_SET_GLOBAL_INT
#define ATTR_SCRATCH_UINT8_2_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_SCRATCH_UINT8_2_SETMACRO PLAT_ATTR_SET_GLOBAL_INT
#define ATTR_SYSTEM_IPL_PHASE_GETMACRO PLAT_ATTR_GET_GLOBAL_INT
#define ATTR_TIME_RING_GETMACRO PLAT_ATTR_GET_GLOBAL_INT


/******************************************************************************/
/*                          * Global macros *                                 */
/* These macros are called by the macros above to invoke the appropriate API. */
/* These macros do not need to change when a new attribute is introduced.     */
/******************************************************************************/

/* global get uint8_t 1D array macro */
#define PLAT_ATTR_GET_UINT8_1D_ARRAY(ID, PTARGET, VAL) \
     _getAttributeArrayShort<ID##_Type, static_cast<fapi2::TargetType>(ID##_TargetTypes), ID> \
            (ID, PTARGET, VAL)

/* global set uint8_t 1D array macro */
#define PLAT_ATTR_SET_UINT8_1D_ARRAY(ID, PTARGET, VAL) \
     _setAttributeArrayShort<ID##_Type, static_cast<fapi2::TargetType>(ID##_TargetTypes), ID> \
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
     _getAttributeArrayDoubleWord(ID, PTARGET, VAL)
/* global set uint64_t 1D array macro */
#define PLAT_ATTR_SET_UINT64_1D_ARRAY(ID, PTARGET, VAL) \
     _setAttributeArrayDoubleWord(ID, PTARGET, VAL)

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
     _get<ID##_Type, static_cast<fapi2::TargetType>(ID##_TargetTypes), ID> \
                (ID, PTARGET, VAL)

/* global set int macro (uint8_t, 16, 32 and 64) */
#define PLAT_ATTR_SET_GLOBAL_INT(ID, PTARGET, VAL) \
     _set<ID##_Type, static_cast<fapi2::TargetType>(ID##_TargetTypes), ID> \
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


/* INSERT NEW GETTER AND SETTER FUNCTIONS HERE */

template<> void __get<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, uint8_t, fapi2::ATTR_BACKUP_SEEPROM_SELECT>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, const fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid, uint8_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PERV, fapi2attr::PervAttributes_t, uint8_t, fapi2::ATTR_CHIP_UNIT_POS>   ( const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_ptarget, const fapi2attr::PervAttributes_t* object, const fapi2::AttributeId attrid, uint8_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_CORE, fapi2attr::CoreAttributes_t, uint32_t, fapi2::ATTR_CORE_GPTR_RING>   ( const fapi2::Target<fapi2::TARGET_TYPE_CORE>& i_ptarget, const fapi2attr::CoreAttributes_t* object, const fapi2::AttributeId attrid, uint32_t* o_pvalue );
template<> void __set<fapi2::TARGET_TYPE_CORE, fapi2attr::CoreAttributes_t, uint32_t, fapi2::ATTR_CORE_GPTR_RING>   ( const fapi2::Target<fapi2::TARGET_TYPE_CORE>& i_ptarget, fapi2attr::CoreAttributes_t* object, const fapi2::AttributeId attrid,  const uint32_t& i_pvalue );
template<> void __get<fapi2::TARGET_TYPE_CORE, fapi2attr::CoreAttributes_t, uint32_t, fapi2::ATTR_CORE_REPR_RING>   ( const fapi2::Target<fapi2::TARGET_TYPE_CORE>& i_ptarget, const fapi2attr::CoreAttributes_t* object, const fapi2::AttributeId attrid, uint32_t* o_pvalue );
template<> void __set<fapi2::TARGET_TYPE_CORE, fapi2attr::CoreAttributes_t, uint32_t, fapi2::ATTR_CORE_REPR_RING>   ( const fapi2::Target<fapi2::TARGET_TYPE_CORE>& i_ptarget, fapi2attr::CoreAttributes_t* object, const fapi2::AttributeId attrid,  const uint32_t& i_pvalue );
template<> void __get<fapi2::TARGET_TYPE_CORE, fapi2attr::CoreAttributes_t, uint32_t, fapi2::ATTR_CORE_TIME_RING>   ( const fapi2::Target<fapi2::TARGET_TYPE_CORE>& i_ptarget, const fapi2attr::CoreAttributes_t* object, const fapi2::AttributeId attrid, uint32_t* o_pvalue );
template<> void __set<fapi2::TARGET_TYPE_CORE, fapi2attr::CoreAttributes_t, uint32_t, fapi2::ATTR_CORE_TIME_RING>   ( const fapi2::Target<fapi2::TARGET_TYPE_CORE>& i_ptarget, fapi2attr::CoreAttributes_t* object, const fapi2::AttributeId attrid,  const uint32_t& i_pvalue );
template<> void __get<fapi2::TARGET_TYPE_EQ, fapi2attr::EQAttributes_t, uint32_t, fapi2::ATTR_DPLL_RING>   ( const fapi2::Target<fapi2::TARGET_TYPE_EQ>& i_ptarget, const fapi2attr::EQAttributes_t* object, const fapi2::AttributeId attrid, uint32_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, uint8_t, fapi2::ATTR_FABRIC_SYSTEM_ID>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, const fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid, uint8_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PERV, fapi2attr::PervAttributes_t, uint32_t, fapi2::ATTR_GPTR_RING>   ( const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_ptarget, const fapi2attr::PervAttributes_t* object, const fapi2::AttributeId attrid, uint32_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_EX, fapi2attr::EXAttributes_t, uint32_t, fapi2::ATTR_L2_GPTR_RING>   ( const fapi2::Target<fapi2::TARGET_TYPE_EX>& i_ptarget, const fapi2attr::EXAttributes_t* object, const fapi2::AttributeId attrid, uint32_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_EX, fapi2attr::EXAttributes_t, uint32_t, fapi2::ATTR_L2_REPR_RING>   ( const fapi2::Target<fapi2::TARGET_TYPE_EX>& i_ptarget, const fapi2attr::EXAttributes_t* object, const fapi2::AttributeId attrid, uint32_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_EX, fapi2attr::EXAttributes_t, uint32_t, fapi2::ATTR_L2_TIME_RING>   ( const fapi2::Target<fapi2::TARGET_TYPE_EX>& i_ptarget, const fapi2attr::EXAttributes_t* object, const fapi2::AttributeId attrid, uint32_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_EX, fapi2attr::EXAttributes_t, uint32_t, fapi2::ATTR_L3_GPTR_RING>   ( const fapi2::Target<fapi2::TARGET_TYPE_EX>& i_ptarget, const fapi2attr::EXAttributes_t* object, const fapi2::AttributeId attrid, uint32_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_EX, fapi2attr::EXAttributes_t, uint32_t, fapi2::ATTR_L3_REPR_RING>   ( const fapi2::Target<fapi2::TARGET_TYPE_EX>& i_ptarget, const fapi2attr::EXAttributes_t* object, const fapi2::AttributeId attrid, uint32_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_EX, fapi2attr::EXAttributes_t, uint32_t, fapi2::ATTR_L3_TIME_RING>   ( const fapi2::Target<fapi2::TARGET_TYPE_EX>& i_ptarget, const fapi2attr::EXAttributes_t* object, const fapi2::AttributeId attrid, uint32_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, uint8_t, fapi2::ATTR_MC_SYNC_MODE>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, const fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid, uint8_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, uint16_t, fapi2::ATTR_PG_EC00>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, const fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid, uint16_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, uint16_t, fapi2::ATTR_PG_EC01>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, const fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid, uint16_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, uint16_t, fapi2::ATTR_PG_EC02>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, const fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid, uint16_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, uint16_t, fapi2::ATTR_PG_EC03>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, const fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid, uint16_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, uint16_t, fapi2::ATTR_PG_EC04>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, const fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid, uint16_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, uint16_t, fapi2::ATTR_PG_EC05>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, const fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid, uint16_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, uint16_t, fapi2::ATTR_PG_EC06>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, const fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid, uint16_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, uint16_t, fapi2::ATTR_PG_EC07>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, const fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid, uint16_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, uint16_t, fapi2::ATTR_PG_EC08>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, const fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid, uint16_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, uint16_t, fapi2::ATTR_PG_EC09>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, const fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid, uint16_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, uint16_t, fapi2::ATTR_PG_EC10>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, const fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid, uint16_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, uint16_t, fapi2::ATTR_PG_EC11>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, const fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid, uint16_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, uint16_t, fapi2::ATTR_PG_EC12>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, const fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid, uint16_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, uint16_t, fapi2::ATTR_PG_EC13>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, const fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid, uint16_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, uint16_t, fapi2::ATTR_PG_EC14>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, const fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid, uint16_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, uint16_t, fapi2::ATTR_PG_EC15>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, const fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid, uint16_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, uint16_t, fapi2::ATTR_PG_EC16>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, const fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid, uint16_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, uint16_t, fapi2::ATTR_PG_EC17>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, const fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid, uint16_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, uint16_t, fapi2::ATTR_PG_EC18>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, const fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid, uint16_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, uint16_t, fapi2::ATTR_PG_EC19>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, const fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid, uint16_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, uint16_t, fapi2::ATTR_PG_EC20>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, const fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid, uint16_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, uint16_t, fapi2::ATTR_PG_EC21>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, const fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid, uint16_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, uint16_t, fapi2::ATTR_PG_EC22>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, const fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid, uint16_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, uint16_t, fapi2::ATTR_PG_EC23>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, const fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid, uint16_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, uint16_t, fapi2::ATTR_PG_EQ0>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, const fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid, uint16_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, uint16_t, fapi2::ATTR_PG_EQ1>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, const fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid, uint16_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, uint16_t, fapi2::ATTR_PG_EQ2>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, const fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid, uint16_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, uint16_t, fapi2::ATTR_PG_EQ3>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, const fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid, uint16_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, uint16_t, fapi2::ATTR_PG_EQ4>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, const fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid, uint16_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, uint16_t, fapi2::ATTR_PG_EQ5>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, const fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid, uint16_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, uint16_t, fapi2::ATTR_PG_FSI>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, const fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid, uint16_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, uint16_t, fapi2::ATTR_PG_MC01>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, const fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid, uint16_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, uint16_t, fapi2::ATTR_PG_MC23>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, const fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid, uint16_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, uint16_t, fapi2::ATTR_PG_N0>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, const fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid, uint16_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, uint16_t, fapi2::ATTR_PG_N1>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, const fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid, uint16_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, uint16_t, fapi2::ATTR_PG_N2>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, const fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid, uint16_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, uint16_t, fapi2::ATTR_PG_N3>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, const fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid, uint16_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, uint16_t, fapi2::ATTR_PG_OB0>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, const fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid, uint16_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, uint16_t, fapi2::ATTR_PG_OB1>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, const fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid, uint16_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, uint16_t, fapi2::ATTR_PG_OB2>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, const fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid, uint16_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, uint16_t, fapi2::ATTR_PG_OB3>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, const fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid, uint16_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, uint16_t, fapi2::ATTR_PG_PCI0>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, const fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid, uint16_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, uint16_t, fapi2::ATTR_PG_PCI1>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, const fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid, uint16_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, uint16_t, fapi2::ATTR_PG_PCI2>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, const fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid, uint16_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, uint16_t, fapi2::ATTR_PG_PRV>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, const fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid, uint16_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, uint16_t, fapi2::ATTR_PG_XB>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, const fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid, uint16_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PERV, fapi2attr::PervAttributes_t, uint32_t, fapi2::ATTR_PLL_RING>   ( const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_ptarget, const fapi2attr::PervAttributes_t* object, const fapi2::AttributeId attrid, uint32_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PERV, fapi2attr::PervAttributes_t, uint32_t, fapi2::ATTR_REPR_RING>   ( const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_ptarget, const fapi2attr::PervAttributes_t* object, const fapi2::AttributeId attrid, uint32_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, int32_t, fapi2::ATTR_SCRATCH_INT32_1>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, const fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid, int32_t* o_pvalue );
template<> void __set<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, int32_t, fapi2::ATTR_SCRATCH_INT32_1>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid,  const int32_t& i_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, int32_t, fapi2::ATTR_SCRATCH_INT32_2>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, const fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid, int32_t* o_pvalue );
template<> void __set<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, int32_t, fapi2::ATTR_SCRATCH_INT32_2>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid,  const int32_t& i_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, int64_t, fapi2::ATTR_SCRATCH_INT64_1>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, const fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid, int64_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, int64_t, fapi2::ATTR_SCRATCH_INT64_2>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, const fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid, int64_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, int8_t, fapi2::ATTR_SCRATCH_INT8_1>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, const fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid, int8_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, int8_t, fapi2::ATTR_SCRATCH_INT8_2>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, const fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid, int8_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, uint32_t, fapi2::ATTR_SCRATCH_UINT32_1>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, const fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid, uint32_t* o_pvalue );
template<> void __set<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, uint32_t, fapi2::ATTR_SCRATCH_UINT32_1>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid,  const uint32_t& i_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, uint32_t, fapi2::ATTR_SCRATCH_UINT32_2>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, const fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid, uint32_t* o_pvalue );
template<> void __set<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, uint32_t, fapi2::ATTR_SCRATCH_UINT32_2>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid,  const uint32_t& i_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, uint32_t, fapi2::ATTR_SCRATCH_UINT32_ARRAY>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, const fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid, uint32_t* o_pvalue );
template<> void __set<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, uint32_t, fapi2::ATTR_SCRATCH_UINT32_ARRAY>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid,  const uint32_t& i_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PERV, fapi2attr::PervAttributes_t, uint32_t, fapi2::ATTR_SCRATCH_UINT32_PERV_ARRAY>   ( const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_ptarget, const fapi2attr::PervAttributes_t* object, const fapi2::AttributeId attrid, uint32_t* o_pvalue );
template<> void __set<fapi2::TARGET_TYPE_PERV, fapi2attr::PervAttributes_t, uint32_t, fapi2::ATTR_SCRATCH_UINT32_PERV_ARRAY>   ( const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_ptarget, fapi2attr::PervAttributes_t* object, const fapi2::AttributeId attrid,  const uint32_t& i_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, uint64_t, fapi2::ATTR_SCRATCH_UINT64_1>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, const fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid, uint64_t* o_pvalue );
template<> void __set<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, uint64_t, fapi2::ATTR_SCRATCH_UINT64_1>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid,  const uint64_t& i_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, uint64_t, fapi2::ATTR_SCRATCH_UINT64_2>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, const fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid, uint64_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, uint8_t, fapi2::ATTR_SCRATCH_UINT8_1>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, const fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid, uint8_t* o_pvalue );
template<> void __set<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, uint8_t, fapi2::ATTR_SCRATCH_UINT8_1>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid,  const uint8_t& i_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, uint8_t, fapi2::ATTR_SCRATCH_UINT8_2>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, const fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid, uint8_t* o_pvalue );
template<> void __set<fapi2::TARGET_TYPE_PROC_CHIP, fapi2attr::ProcChipAttributes_t, uint8_t, fapi2::ATTR_SCRATCH_UINT8_2>   ( const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_ptarget, fapi2attr::ProcChipAttributes_t* object, const fapi2::AttributeId attrid,  const uint8_t& i_pvalue );
template<> void __get<fapi2::TARGET_TYPE_SYSTEM, fapi2attr::SystemAttributes_t, uint8_t, fapi2::ATTR_SYSTEM_IPL_PHASE>   ( const fapi2::Target<fapi2::TARGET_TYPE_SYSTEM>& i_ptarget, const fapi2attr::SystemAttributes_t* object, const fapi2::AttributeId attrid, uint8_t* o_pvalue );
template<> void __get<fapi2::TARGET_TYPE_PERV, fapi2attr::PervAttributes_t, uint32_t, fapi2::ATTR_TIME_RING>   ( const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_ptarget, const fapi2attr::PervAttributes_t* object, const fapi2::AttributeId attrid, uint32_t* o_pvalue );



//******************************************************************************
// Get base template
//******************************************************************************
template<typename T, TargetType K, AttributeId A>
ReturnCode _get(const AttributeId i_id,
                const Target<K> & i_pTarget,
                T& o_value)
{
    return FAPI2_RC_SUCCESS;
}

//******************************************************************************
// Get uint8_t
//******************************************************************************
template<typename T, TargetType K, AttributeId A>
ReturnCode _get(const AttributeId i_id,
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
ReturnCode _get(const AttributeId i_id,
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
ReturnCode _get(const AttributeId i_id,
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
ReturnCode _get(const AttributeId i_id,
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
ReturnCode _getOverride(const AttributeId i_id,
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
ReturnCode _getOverride(const AttributeId i_id,
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
ReturnCode _getOverride(const AttributeId i_id,
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
ReturnCode _getOverride(const AttributeId i_id,
                        const Target<K> & i_pTarget,
                        uint64_t& o_value)
{
    static_assert(std::is_same<T, uint64_t>::value, "Attribute type mismatch");

    return FAPI2_RC_SUCCESS;
}


//******************************************************************************
// Get override EC Feature (uint8_t)
//******************************************************************************
template<typename T, TargetType K, AttributeId A>
ReturnCode _getEcFeatureOverride(const AttributeId i_id,
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
template<typename T, TargetType K, AttributeId A>
ReturnCode _getAttributeArrayShort(const AttributeId i_id,
                                   const Target<K> & i_pTarget,
                                   uint8_t * o_pValues)
{
//       fapi2::Attributeta o_data;
//     fapi2::ReturnCode l_fapi_rc(FAPI2_RC_SUCCESS);
//     uint32_t l_ecmd_rc = ECMD_SUCCESS;
//
//     ecmdChipTarget l_ecmd_target;
//     fapiTargetPointerToEcmdTarget(i_pTarget, l_ecmd_target);
//
//     o_data.faValidMask = FAPI_ATTRIBUTE_TYPE_UINT8ARY;
//     o_data.faUint8ary = o_pValues;
//
//     l_ecmd_rc = fapi2GetAttribute(l_ecmd_target, i_id, o_data);
//     if (l_ecmd_rc)
//     {
//         l_fapi_rc = (ReturnCodes) l_ecmd_rc;
//     }
//     return l_fapi_rc;
    return FAPI2_RC_SUCCESS;
}

//******************************************************************************
// Set uint8_t array
//******************************************************************************
template<typename T, TargetType K, AttributeId A>
ReturnCode _setAttributeArrayShort(const AttributeId i_id,
                                   const Target<K> & i_pTarget,
                                   uint8_t * i_pValues)
{
    if(K & TARGET_TYPE_SYSTEM)
    {
        __set<K, fapi2attr::SystemAttributes_t, T, A>( i_pTarget, G_system_attributes_ptr, i_id, *i_pValues );
    }

    if(K & TARGET_TYPE_PROC_CHIP)
    {
        __set<K, fapi2attr::ProcChipAttributes_t, T, A>( i_pTarget, G_proc_chip_attributes_ptr, *i_pValues );
    }

    if(K & TARGET_TYPE_PERV)
    {
        __set<K, fapi2attr::PervAttributes_t, T, A>( i_pTarget, G_perv_attributes_ptr, *i_pValues );
    }

    if(K & TARGET_TYPE_CORE)
    {
        __set<K, fapi2attr::CoreAttributes_t, T, A>( i_pTarget, G_core_attributes_ptr, *i_pValues );
    }

    if(K & TARGET_TYPE_EQ)
    {
        __set<K, fapi2attr::EQAttributes_t, T, A>( i_pTarget, G_eq_attributes_ptr, *i_pValues );
    }

    if(K & TARGET_TYPE_EX)
    {
        __set<K, fapi2attr::EXAttributes_t, T, A>( i_pTarget, G_ex_attributes_ptr, *i_pValues );
    }

    return FAPI2_RC_SUCCESS;
}

//******************************************************************************
// Get uint16_t array
//******************************************************************************
template<typename T, TargetType K, AttributeId A>
ReturnCode _getAttributeArrayWord(const AttributeId i_id,
                                  const Target<K> & i_pTarget,
                                  uint16_t * o_pValues)
{
    return FAPI2_RC_SUCCESS;
}

//******************************************************************************
// Set uint16_t array
//******************************************************************************
template<typename T, TargetType K, AttributeId A>
ReturnCode _setAttributeArrayWord(const AttributeId i_id,
                                  const Target<K> & i_pTarget,
                                  uint16_t * i_pValues)
{
    return FAPI2_RC_SUCCESS;
}

//******************************************************************************
// Get uint32_t array
//******************************************************************************
template<typename T, TargetType K, AttributeId A>
ReturnCode _getAttributeArrayWord(const AttributeId i_id,
                                  const Target<K> & i_pTarget,
                                  uint32_t * o_pValues)
{
    return FAPI2_RC_SUCCESS;
}

//******************************************************************************
// Set uint32_t array
//******************************************************************************
template<typename T, TargetType K, AttributeId A>
ReturnCode _setAttributeArrayWord(const AttributeId i_id,
                                  const Target<K> & i_pTarget,
                                  uint32_t * i_pValues)
{
    return FAPI2_RC_SUCCESS;
}


//******************************************************************************
// Get uint64_t array
//******************************************************************************
template<typename T, TargetType K, AttributeId A>
ReturnCode _getAttributeArrayDoubleWord(const AttributeId i_id,
                                        const Target<K> & i_pTarget,
                                        uint64_t * o_pValues)
{
    return FAPI2_RC_SUCCESS;
}

//******************************************************************************
// Set uint64_t array
//******************************************************************************
template<typename T, TargetType K, AttributeId A>
ReturnCode _setAttributeArrayDoubleWord(const AttributeId i_id,
                                        const Target<K> & i_pTarget,
                                        uint64_t * i_pValues)
{
    return FAPI2_RC_SUCCESS;
}

//******************************************************************************
// Get Override uint8_t array
//******************************************************************************
template<typename T, TargetType K, AttributeId A>
ReturnCode _getAttributeOverrideArrayShort(const AttributeId i_id,
                                           const Target<K> & i_pTarget,
                                           uint8_t * o_pValues)
{
    return FAPI2_RC_SUCCESS;
}

//******************************************************************************
// Get Override uint16_t array
//******************************************************************************
template<typename T, TargetType K, AttributeId A>
ReturnCode _getAttributeOverrideArrayWord(const AttributeId i_id,
                                          const Target<K> & i_pTarget,
                                          uint16_t * o_pValues)
{
    return FAPI2_RC_SUCCESS;
}

//******************************************************************************
// Get Override uint32_t array
//******************************************************************************
template<typename T, TargetType K, AttributeId A>
ReturnCode _getAttributeOverrideArrayWord(const AttributeId i_id,
                                          const Target<K> & i_pTarget,
                                          uint32_t * o_pValues)
{
    return FAPI2_RC_SUCCESS;
}

//******************************************************************************
// Get Override uint64_t array
//******************************************************************************
template<typename T, TargetType K, AttributeId A>
ReturnCode _getAttributeOverrideArrayDoubleWord(const AttributeId i_id,
                                                const Target<K> & i_pTarget,
                                                uint64_t * o_pValues)
{
    return FAPI2_RC_SUCCESS;
}


//******************************************************************************
// Set base template
//******************************************************************************
template<typename T, TargetType K, AttributeId A>
ReturnCode _set(const AttributeId i_id,
                const Target<K> & i_pTarget,
                const T& i_value)
{
    return FAPI2_RC_SUCCESS;
}



//******************************************************************************
// Set uint8_t
//******************************************************************************
template<typename T, TargetType K, AttributeId A>
ReturnCode _set(const AttributeId i_id,
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
ReturnCode _set(const AttributeId i_id,
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
ReturnCode _set(const AttributeId i_id,
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
ReturnCode _set(const AttributeId i_id,
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
ReturnCode _get(const AttributeId i_id,
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
ReturnCode _get(const AttributeId i_id,
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
ReturnCode _get(const AttributeId i_id,
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
ReturnCode _get(const AttributeId i_id,
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
ReturnCode _getOverride(const AttributeId i_id,
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
ReturnCode _getOverride(const AttributeId i_id,
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
ReturnCode _getOverride(const AttributeId i_id,
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
ReturnCode _getOverride(const AttributeId i_id,
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
ReturnCode _getAttributeArraySignedShort(const AttributeId i_id,
                                         const Target<K> & i_pTarget,
                                         int8_t * o_pValues)
{
    return FAPI2_RC_SUCCESS;
}


//******************************************************************************
// Set int8_t array
//******************************************************************************
template<TargetType K>
ReturnCode _setAttributeArraySignedShort(const AttributeId i_id,
                                         const Target<K> & i_pTarget,
                                         int8_t * i_pValues)
{
    return FAPI2_RC_SUCCESS;
}

//******************************************************************************
// Get int16_t array
//******************************************************************************
template<TargetType K>
ReturnCode _getAttributeArraySignedWord(const AttributeId i_id,
                                        const Target<K> & i_pTarget,
                                        int16_t * o_pValues)
{
    return FAPI2_RC_SUCCESS;
}


//******************************************************************************
// Set int16_t array
//******************************************************************************
template<TargetType K>
ReturnCode _setAttributeArraySignedWord(const AttributeId i_id,
                                        const Target<K> & i_pTarget,
                                        int16_t * i_pValues)
{
    return FAPI2_RC_SUCCESS;
}

//******************************************************************************
// Get int32_t array
//******************************************************************************
template<TargetType K>
ReturnCode _getAttributeArraySignedWord(const AttributeId i_id,
                                        const Target<K> & i_pTarget,
                                        int32_t * o_pValues)
{
    return FAPI2_RC_SUCCESS;
}


//******************************************************************************
// Set int32_t array
//******************************************************************************
template<TargetType K>
ReturnCode _setAttributeArraySignedWord(const AttributeId i_id,
                                        const Target<K> & i_pTarget,
                                        int32_t * i_pValues)
{
    return FAPI2_RC_SUCCESS;
}


//******************************************************************************
// Get int64_t array
//******************************************************************************
template<TargetType K>
ReturnCode _getAttributeArraySignedDoubleWord(const AttributeId i_id,
                                              const Target<K> & i_pTarget,
                                              int64_t * o_pValues)
{
    return FAPI2_RC_SUCCESS;
}

//******************************************************************************
// Set int64_t array
//******************************************************************************
template<TargetType K>
ReturnCode _setAttributeArraySignedDoubleWord(const AttributeId i_id,
                                              const Target<K> & i_pTarget,
                                              int64_t * i_pValues)
{
    return FAPI2_RC_SUCCESS;
}

//******************************************************************************
// Get Override int8_t array
//******************************************************************************
template<TargetType K>
ReturnCode _getAttributeOverrideArraySignedShort(const AttributeId i_id,
                                                 const Target<K> & i_pTarget,
                                                 int8_t * o_pValues)
{
    return FAPI2_RC_SUCCESS;
}

//******************************************************************************
// Get Override int16_t array
//******************************************************************************
template<TargetType K>
ReturnCode _getAttributeOverrideArraySignedWord(const AttributeId i_id,
                                                const Target<K> & i_pTarget,
                                                int16_t * o_pValues)
{
    return FAPI2_RC_SUCCESS;
}

//******************************************************************************
// Get Override int32_t array
//******************************************************************************
template<TargetType K>
ReturnCode _getAttributeOverrideArraySignedWord(const AttributeId i_id,
                                                const Target<K> & i_pTarget,
                                                int32_t * o_pValues)
{
    return FAPI2_RC_SUCCESS;
}


//******************************************************************************
// Get Override int64_t array
//******************************************************************************
template<TargetType K>
ReturnCode _getAttributeOverrideArraySignedDoubleWord(const AttributeId i_id,
                                                      const Target<K> & i_pTarget,
                                                      int64_t * o_pValues)
{
    return FAPI2_RC_SUCCESS;
}


//******************************************************************************
// Set int8_t
//******************************************************************************
template<typename T, TargetType K, AttributeId A>
ReturnCode _set(const AttributeId i_id,
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
ReturnCode _set(const AttributeId i_id,
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
ReturnCode _set(const AttributeId i_id,
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
ReturnCode _set(const AttributeId i_id,
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

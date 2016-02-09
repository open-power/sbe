/*
 * @file: ppe/sbe/sbefw/sbecmdsram.H
 *
 * @brief This file contains the Interfaces for the Sram Access chip-ops
 *
 */

#ifndef __SBEFW_SBECMDSRAM_H
#define __SBEFW_SBECMDSRAM_H

#include <stdint.h>

static const uint8_t SBE_32BIT_ALIGN_FACTOR = 4;
static const uint8_t SBE_64BIT_ALIGN_FACTOR = 8;

static const uint64_t SBE_FWCTRLFLG3_FSP_ATTACHED = 0X1000000000000000;

/*
 * @brief Granule size in Bytes for 'Get from Hwp'/'Put to Hwp'
 */
static const uint32_t SBE_OCC_SRAM_GRANULE = 128;

/**
  * @brief sbeGetOccSram : Implements SBE Get Occ Sram ChipOp
  *
  * @param[in] i_pArg Buffer to be passed to the function (not used as of now)
  *
  * @return    Rc from the FIFO access utility
  */
uint32_t sbeGetOccSram (uint8_t *i_pArg);

/**
  * @brief sbePutOccSram : Implements SBE Put Occ Sram ChipOp
  *
  * @param[in] i_pArg Buffer to be passed to the function (not used as of now)
  *
  * @return    Rc from the FIFO access utility
  */
uint32_t sbePutOccSram  (uint8_t *i_pArg);

#endif /* __SBEFW_SBECMDSRAM_H */

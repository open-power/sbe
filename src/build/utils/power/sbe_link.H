/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/build/utils/power/sbe_link.H $                            */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2021                        */
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
#ifndef __SBE_LINK_H
#define __SBE_LINK_H

// $Id:  Exp $

/// \file sbe_link.H
/// \brief Constants required for linking SBE code images
///
/// This header contains those cpp manifest constants required for processing
/// the linker scripts used to generate SBE code images.  They may also be
/// useful for PPE assembler programming so they are included in sbe.H as
/// well.
///
/// The link address of OTPROM code as a byte-address
///
/// The OTPROM can be addressed both as an I2C slave and as a direct PIB
/// slave.  The I2C slave is at PIB local address 0x0 and the PIB memory
/// interface is at local address 0x8000.  SBE/IPL uses the PIB addressing
/// mode, so the OTPROM image is linked at 0x40000 (the byte-address
/// equivalent of 0x8000).
#define OTPROM_ORIGIN 0xC0000

/// The amount of memory contained in each OTPROM macro
#define OTPROM_BLOCK_SIZE 1024

/// The amount of memory reserved for OTPROM code
///
/// This amount is exclusive of physical OTPROM memory reserved for compressed
/// scan ring images.
#define OTPROM_CODE_SIZE (1 * OTPROM_BLOCK_SIZE)

/// The link address of the P10 SBE Base image
#define SBE_BASE_ORIGIN 0xFFF80000
/// SBE base image length ( 512 KB - 512B for loader - 128B for Pibmem Cntl)
#define SBE_BASE_LENGTH ((512 * 1024) - 512 - 128)

///This Index depends upon XIP HEADER File
// Start offset of sectionTable in XIP header. There are 8 entries
// of 8 byte each.
#define SBE_XIP_TOC_OFFSET 8*8

/// The link address of the P10 SBE Boot Seeprom image
#define SBE_SEEPROM_BASE_ORIGIN 0xFF800000
#define OTPROM_SEEPROM_HIGHER_NIBBLE 0xFF80
#define OTPROM_MEASUREMENT_HIGHER_NIBBLE    0xFF88

/// The link address of the P10 SBE Measurement Seeprom image
#define SBE_MEASUREMENT_BASE_ORIGIN 0xFF880000

//   Base section is the 9th (P9_XIP_SECTION_SBE_BASE) section
#define SBE_MEASUREMENT_BASE_SECTION  SBE_MEASUREMENT_BASE_ORIGIN \
                                    + SBE_XIP_TOC_OFFSET  + 108
//Maximum Length of pibmem. This is used to zero out the memory
//after loading the base section of the measurement seeprom
#define SBE_MEASUREMENT_PIBMEM_LENGTH (0x80000 - 0x80)

///  ALL XIP related offsets are valid for measurement seeprom as well
///  This Index depends upon XIP HEADER File
///  Each section table entry is 12 bytes(SIZE_OF_SBE_XIP_SECTION) size,
//   Base Loader is 10 th (P9_XIP_SECTION_BASELOADER) section
#define SBE_LOADER_BASE_SECTION  SBE_SEEPROM_BASE_ORIGIN + SBE_XIP_TOC_OFFSET \
                                  + 120
//   Base section is the 9th (P9_XIP_SECTION_SBE_BASE) section
#define SBE_MEASUREMENT_BASE_SECTION  SBE_MEASUREMENT_BASE_ORIGIN + SBE_XIP_TOC_OFFSET \
                                  + 108
//   sb_verification section is the 22th section
#define SBE_SHA_LOADER_BASE_SECTION  SBE_SEEPROM_BASE_ORIGIN + SBE_XIP_TOC_OFFSET \
                                  + 264 // This needs to be updated.
// Base Loader start address
#define SBE_LOADER_BASE_ORIGIN 0xFFFFD000
// Base Loader length - Pibmem control area for P10
#define SBE_LOADER_BASE_LENGTH (0x3000 - 0x80)

// Base Loader start address
#define SBE_VERIFICATION_PIBMEM_ORIGIN 0xFFFA0000
#define SBE_VERIFICATION_PIBMEM_LENGTH (SBE_BASE_LENGTH - 0x20000)

// Base Loader entry function offset in header
#define SBE_LOADER_ENTRY_HEADER_OFFSET 20
// PK boot offset in header
#define SBE_KERNEL_ENTRY_HEADER_OFFSET 28

// Pibmem Repair Section XIP HDR
// Each section table entry is 12Bytes size, Pibmem repair is 6th section
#define SBE_PIBMEM_REPAIR_SECTION SBE_MEASUREMENT_BASE_ORIGIN + SBE_XIP_TOC_OFFSET \
                                    + 72

/// The physical address offset where SBE-SEEPROM code is loaded
///
/// This address *must* be a constant known to the OTPROM code.
#define SBE_SEEPROM_LOAD_ADDRESS 0x2000

/// The offset (in bytes) of the .fixed section in P10 SBE-XIP images
#define SBE_XIP_FIXED_OFFSET 512

/// The link address of the PIBMEM image
#define PIBMEM_ORIGIN 0

/// The amount of space available in the PIBMEM, in bytes (512KB)
// The last 128 Bytes are used by Pibmem control block as control register
#define PIBMEM_SIZE (512 * 1024 - 128)

/// The fixed section of Otprom
#define OTPROM_FIXED_SIZE (OTPROM_ORIGIN + 0x340)

/// Sbe fixed sction for the pibmem repair
#define SBE_FIXED_SECTION (SBE_SEEPROM_BASE_ORIGIN + SBE_XIP_TOC_OFFSET + 12)

// Macros to use in assembly code in OTPROM/SEEPROM
// This will be updated in last two bits of messaging register.
// This will help in debugging sbe boot failures
//
// Code reached to OTPROM
#define SBE_CODE_OTPROM_START_MSG 0x01
// Code reached to MEASUREMENT SEEPROM loader
#define SBE_CODE_MEASUREMENT_SEEPROM_START_MSG 0x02
// Code reached to MEASUREMENT SEEPROM Pibmem Repair
#define SBE_CODE_MEASUREMENT_SEEPROM_PIBMEM_REPAIR_MSG 0x03
// Code reached to MEASUREMENT SEEPROM Pib Init
#define SBE_CODE_MEASUREMENT_SEEPROM_PIB_INIT_MSG 0x04
// Code reached to MEASUREMENT TPM RESET
#define SBE_CODE_MEASUREMENT_SEEPROM_TPM_RESET_MSG 0x05
// Code reached to MEASUREMENT PIBMEM
#define SBE_CODE_MEASURMENT_PIBMEM_START_MSG 0x06
// Code reached to MEASUREMENT PAU DPLL LOCK
#define SBE_CODE_MEASURMENT_PAU_DPLL_LOCK_MSG 0x07
// Code reached to MEASUREMENT TPM INIT SEQUENCE
#define SBE_CODE_MEASURMENT_TPM_INIT_SEQUENCE_MSG 0x08
//Code reached to MEASURMENT SHA512 SEQUENCE COMPLETE
#define SBE_CODE_MEASURMENT_SHA512_COMPLETE 0x09
// Code reached to Verification Image Main Flow
#define SBE_CODE_VERIFICATION_PIBMEM_MAIN_MSG 0x0A
// Code reached to Verification Image SBE_FW Secure header complete
#define SBE_CODE_VERIFICATION_SBEFW_SECURE_HDR_DONE 0x0B
// Code reached to Verification Image HBBL Secure header complete
#define SBE_CODE_VERIFICATION_HBBL_SECURE_HDR_DONE 0x0C
// Code reached to BOOT SEEPROM loader L1
#define SBE_CODE_BOOT_SEEPROM_L1_LOADER_MSG 0x0D
// Code reached to BOOT PIBMEM Loader L2
#define SBE_CODE_BOOT_PIBMEM_L2_LOADER_MSG 0x0E
// Code reached to BOOT PIBMEM Main Flow
#define SBE_CODE_BOOT_PIBMEM_MAIN_MSG 0x0F

// Macros to use in assembly code in Seeprom for Traps
#define PIB_OPCG_LOCK_FAIL                    0x01
#define PIB_CLOCK_RUNNING_STATUS_FAIL         0x02
#define PIBMEM_REPAIR_COMPARE_HDR_FAIL        0x03

#define PAU_DPLL_SCAN_HDR_COMPARE_FAIL        0x04
#define PAU_DPLL_START_CLOCK_OPCG_FAIL        0x05
#define PAU_DPLL_CLOCK_RUNNING_STATUS_FAIL    0x06
#define PAU_DPLL_LOCK_FAIL                    0x07


#endif  // __SBE_LINK_H


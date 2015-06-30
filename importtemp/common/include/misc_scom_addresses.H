/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: chips/p9/common/include/misc_scom_addresses.H $               */
/*                                                                        */
/* IBM CONFIDENTIAL                                                       */
/*                                                                        */
/* EKB Project                                                            */
/*                                                                        */
/* COPYRIGHT 2015                                                         */
/* [+] International Business Machines Corp.                              */
/*                                                                        */
/*                                                                        */
/* The source code for this program is not published or otherwise         */
/* divested of its trade secrets, irrespective of what has been           */
/* deposited with the U.S. Copyright Office.                              */
/*                                                                        */
/* IBM_PROLOG_END_TAG                                                     */
///
/// @file misc_scom_addresses.H
/// @brief  Defines constants for scom addresses
///
// *HWP HWP Owner: Ben Gass <bgass@us.ibm.com>
// *HWP FW Owner: ?
// *HWP Team: Infrastructure
// *HWP Level: 1
// *HWP Consumed by: FSP:HB:HS:OCC:SBE:CME:SGPE:PGPE:FPPE:IPPE

/*---------------------------------------------------------------
 *
/*---------------------------------------------------------------
 *
 *  Issues:
 *    MC regs with same address.        HW323435 (matteo)
 *         Duplicate IOM registers.          HW320456 (designers)
 *    OSC/perv regs same address.       HW323437
 *    TOD reg same address.             HW323439
 *    PHB registers need fixed.         HW320416  ( all regs commented out now )
 *    IO0 registers need fixed.         HW320437
 *
 * Closed
 *    PB.PB_PPE registers need fixed.   HW320435
 *    EX05 registers need fixed.        HW320427  (9020) - L2 ring id's are incorrect
 *    IOFPPE registers need fixed.      HW320424  (9020) - Investigate NULL scope
 *    PEC addresses are wrong.          HW322598  (9020)
 *    MC registers need fixed.          HW320433
 *    VA.VA_NORTH registers need fixed. HW320436
 *
 * Format:
 *
 *  <UNIT>_<SUBUNIT>_<INSTANCE>_<REGISTER>_<ACCESS/TYPE>
 *
 *  Notes: Subunits are only added to make names unique when
 *  there are name collisions.
 *  Only units with more than one instance has instance numbers.
 *  If there is only one, the instance number is omitted.
 *
 *  Instance numbers are chiplet id's for the PERV unit.  The
 *  chiplet id's are mapped to their name and used instead of
 *  instance numbers.  See bellow.
 *
 *  For registers with a single access type the type and access
 *  methods are omitted.
 *
 *  For access types where all bits have the same access methods, the
 *  access method is appended to the name.  If the access methods
 *  are different for some bits, the access type is appended to the
 *  name _SCOM instead of _RO.  The _RW(X) access method is omitted
 *  and assumed to be default.
 *
 *  Valid units / subunits
 *    PU                    : No unit chip level
 *        MCD0[0..1]        : mcd subunit
 *        PIB2OPB[0..1]     : PIB2OPB subunit
 *        OTPROM[0..1]      : otprom subunit
 *        NPU               : common npu subunit
 *        NPU[0..2]         : Npu stacks 0 to 2
 *          CTL             : Npu CTL subunit
 *          DAT             : Npu DAT subunit
 *          SM[0..3]        : Npu SM subunits
 *          NTL[0..1]       : Npu NTL subunit
 *    PERV                  : Pervasive
 *        FSI2PIB           : subunit
 *        FSISHIFT          : subunit
 *        FSII2C            : subunit
 *        FSB               : subunit
 *    EX                    : Ex unit (1/2 quad, 2 cores)
 *        L2                : L2 subunit
 *        L3                : L3 subunit
 *    PEC                   : PCI Pec unit
 *        STACK0            : subunit
 *        STACK1            : subunit
 *        STACK2            : subunit
 *    C                     : core
 *    EQ                    : quad
 *    OBUS                  : obus
 *    CAPP                  : capp
 *    MCBIST                : mcbist
 *    MCA                   : mca
 *    NVBUS                 : (not implemented yet)
 *    PHB                   : (not implemented yet)
 *    MI                    : (not implemented yet)
 *    DMI                   : (not implemented yet)
 *    MCS                   : (not implemented yet)
 *    OCC                   : (not implemented yet)
 *    PPE                   : (not implemented yet)
 *    SBE                   : (not implemented yet)
 *    XBUS                  : (not implemented yet)
 *
 * Pervasive instance names follow chiplet id.
 *
 * Instance/  | Chiplet
 * Chiplet    | name
 * -----------+-----------
 * 0x00       | PIB
 * 0x01       | TP
 * 0x02       | N0
 * 0x03       | N1
 * 0x04       | N2
 * 0x05       | N3
 * 0x06       | XB
 * 0x07       | MC01
 * 0x08       | MC23
 * 0x09       | OB0
 * 0x0A       | OB1
 * 0x0B       | OB2
 * 0x0C       | OB3
 * 0x0D       | PCI0
 * 0x0E       | PCI1
 * 0x0F       | PCI2
 * 0x10       | EP00
 * 0x11       | EP01
 * 0x12       | EP02
 * 0x13       | EP03
 * 0x14       | EP04
 * 0x15       | EP05
 * 0x20       | EC00
 * 0x21       | EC01
 * 0x22       | EC02
 * 0x23       | EC03
 * 0x24       | EC04
 * 0x25       | EC05
 * 0x26       | EC06
 * 0x27       | EC07
 * 0x28       | EC08
 * 0x29       | EC09
 * 0x2A       | EC10
 * 0x2B       | EC11
 * 0x2C       | EC12
 * 0x2D       | EC13
 * 0x2E       | EC14
 * 0x2F       | EC15
 * 0x30       | EC16
 * 0x31       | EC17
 * 0x32       | EC18
 * 0x33       | EC19
 * 0x34       | EC20
 * 0x35       | EC21
 * 0x36       | EC22
 * 0x37       | EC23
 *
 *
 *---------------------------------------------------------------
 *
 * NOTES:
 *
 * there is a SPR ring that goes around the chip with an
 * address(0:9)/tid(0:1) (thread id)/mfspr_data(0:63) and return mfspr_data_v/mfspr_data(0:63)
 *
 *    Add PU_<SUBUNITS>  (only if there are conflicts on these registers)
 *       0x0001XXXX OTPROM
 *       0x0002XXXX FSIM0
 *       0x0003XXXX FSIM1
 *       0x0004XXXX TOD
 *       0x0005XXXX FSI_MBOX
 *       0x0006XXXX OCI_BRIDGE
 *       0x0007XXXX SPI_ADC
 *       0x0008XXXX PIBMEM
 *       0x0009XXXX ADU
 *       0x000AXXXX I2CM
 *       0x000BXXXX SBE_FIFO
 *       0x000DXXXX PSU
 *       0x000EXXXX SBE
 *
 * 0x0000100A for FSI2PIB  => PERV_FSI2PIB
 * 0x00000Cxx for FSISHIFT =>  PERV_FSISHIFT
 * 0x000018xx for FSI I2C =>  PERV_FSII2C
 * 0x000024xx for FSI SBEFIFO => PERV_FSB
 *
 * 0x00000400 PEEK_TABLE
 * 0x00000800 FSI_SLAVE
 * 0x00000C00 FSI_SHIFT
 * 0x00001000 FSI2PIB
 * 0x00001400 FSI_SCRATCHPAD
 * 0x00001800 FSI_I2CM
 * 0x00002400 FSI_SBE_FIFO
 *
 * address fields
 * 0xCCRPxxxx
 *
 * CC=chiplet
 * R=always 0?
 * P=port
 *   0=gpregs
 *   1=normal unit scom ring (exclude)
 *   3=clock controller
 *   4=firs
 *   5=cpm
 *
 * =============================================================================
 * Compiling
 *
 * Precompile the header to save time on subsquent compiles:
 * g++ -I. -c scom_addresses.H
 *
 * Use these options to help reduce the binary size
 * g++ -I. -Os -fdata-sections -ffunction-sections <file>.C -o <output> -Wl,--gc-sections
 *
 *
 *---------------------------------------------------------------
 */

#include <const_common.H>

#ifndef __MISC_SCOM_ADDRESSES_H
#define __MISC_SCOM_ADDRESSES_H

#include <scom_template_consts.H>
#include <misc_scom_addresses_fixes.H>


CONST_UINT64_T( PU_ADDR_0_HASH_FUNCTION_REG                             ,
                ULL(0x02011141), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_ADDR_10_HASH_FUNCTION_REG                            ,
                ULL(0x0201114B), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_ADDR_1_HASH_FUNCTION_REG                             ,
                ULL(0x02011142), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_ADDR_2_HASH_FUNCTION_REG                             ,
                ULL(0x02011143), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_ADDR_3_HASH_FUNCTION_REG                             ,
                ULL(0x02011144), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_ADDR_4_HASH_FUNCTION_REG                             ,
                ULL(0x02011145), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_ADDR_5_HASH_FUNCTION_REG                             ,
                ULL(0x02011146), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_ADDR_6_HASH_FUNCTION_REG                             ,
                ULL(0x02011147), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_ADDR_7_HASH_FUNCTION_REG                             ,
                ULL(0x02011148), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_ADDR_8_HASH_FUNCTION_REG                             ,
                ULL(0x02011149), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_ADDR_9_HASH_FUNCTION_REG                             ,
                ULL(0x0201114A), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_ADS_XSCOM_CMD_REG                                    ,
                ULL(0x0502281C), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_ADU_HANG_DIV_REG                                     ,
                ULL(0x05022850), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_ALTD_ADDR_REG                                        ,
                ULL(0x05022800), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_ALTD_CMD_REG                                         ,
                ULL(0x05022801), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_ALTD_DATA_REG                                        ,
                ULL(0x05022804), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_ALTD_OPTION_REG                                      ,
                ULL(0x05022802), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_ALTD_STATUS_REG                                      ,
                ULL(0x05022803), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU_SM0_ATS_CKSW                                     ,
                ULL(0x05011305), SH_UNT_PU_NPU_SM0, SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU_SM1_ATS_CTRL                                     ,
                ULL(0x05011325), SH_UNT_PU_NPU_SM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_BANK0_MCD_BOT                                        ,
                ULL(0x0301140C), SH_UNT          , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_MCD1_BANK0_MCD_BOT                                   ,
                ULL(0x0301100C), SH_UNT_PU_MCD1  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_BANK0_MCD_CHA                                        ,
                ULL(0x0301140D), SH_UNT          , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_MCD1_BANK0_MCD_CHA                                   ,
                ULL(0x0301100D), SH_UNT_PU_MCD1  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_BANK0_MCD_CMD                                        ,
                ULL(0x0301140E), SH_UNT          , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_MCD1_BANK0_MCD_CMD                                   ,
                ULL(0x0301100E), SH_UNT_PU_MCD1  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_BANK0_MCD_REC                                        ,
                ULL(0x03011410), SH_UNT          , SH_ACS_SCOM      );
CONST_UINT64_T( PU_MCD1_BANK0_MCD_REC                                   ,
                ULL(0x03011010), SH_UNT_PU_MCD1  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_BANK0_MCD_RW                                         ,
                ULL(0x0301140F), SH_UNT          , SH_ACS_SCOM      );
CONST_UINT64_T( PU_MCD1_BANK0_MCD_RW                                    ,
                ULL(0x0301100F), SH_UNT_PU_MCD1  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_BANK0_MCD_STR                                        ,
                ULL(0x0301140B), SH_UNT          , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_MCD1_BANK0_MCD_STR                                   ,
                ULL(0x0301100B), SH_UNT_PU_MCD1  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_BANK0_MCD_TOP                                        ,
                ULL(0x0301140A), SH_UNT          , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_MCD1_BANK0_MCD_TOP                                   ,
                ULL(0x0301100A), SH_UNT_PU_MCD1  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_BANK0_MCD_VGC                                        ,
                ULL(0x03011411), SH_UNT          , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_MCD1_BANK0_MCD_VGC                                   ,
                ULL(0x03011011), SH_UNT_PU_MCD1  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PEC_0_STACK0_BARE_REG                                   ,
                ULL(0x04010C55), SH_UNT_PEC_0_STACK0, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_0_STACK1_BARE_REG                                   ,
                ULL(0x04010C95), SH_UNT_PEC_0_STACK1, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_0_STACK2_BARE_REG                                   ,
                ULL(0x04010CD5), SH_UNT_PEC_0_STACK2, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_1_STACK0_BARE_REG                                   ,
                ULL(0x04011055), SH_UNT_PEC_1_STACK0, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_1_STACK1_BARE_REG                                   ,
                ULL(0x04011095), SH_UNT_PEC_1_STACK1, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_1_STACK2_BARE_REG                                   ,
                ULL(0x040110D5), SH_UNT_PEC_1_STACK2, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_2_STACK0_BARE_REG                                   ,
                ULL(0x04011455), SH_UNT_PEC_2_STACK0, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_2_STACK1_BARE_REG                                   ,
                ULL(0x04011495), SH_UNT_PEC_2_STACK1, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_2_STACK2_BARE_REG                                   ,
                ULL(0x040114D5), SH_UNT_PEC_2_STACK2, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_STACK0_BARE_REG                                     ,
                ULL(0x04010C55), SH_UNT_PEC_STACK0, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_STACK1_BARE_REG                                     ,
                ULL(0x04010C95), SH_UNT_PEC_STACK1, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_STACK2_BARE_REG                                     ,
                ULL(0x04010CD5), SH_UNT_PEC_STACK2, SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_BCDE_CTL_OCI                                         ,
                ULL(0x40020080), SH_UNT          , SH_ACS_OCI       );
CONST_UINT64_T( PU_BCDE_CTL_SCOM                                        ,
                ULL(0x05016850), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_BCDE_OCIBAR_OCI                                      ,
                ULL(0x400200A0), SH_UNT          , SH_ACS_OCI       );
CONST_UINT64_T( PU_BCDE_OCIBAR_SCOM                                     ,
                ULL(0x05016854), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_BCDE_PBADR_OCI                                       ,
                ULL(0x40020098), SH_UNT          , SH_ACS_OCI       );
CONST_UINT64_T( PU_BCDE_PBADR_SCOM                                      ,
                ULL(0x05016853), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_BCDE_SET_OCI                                         ,
                ULL(0x40020088), SH_UNT          , SH_ACS_OCI       );
CONST_UINT64_T( PU_BCDE_SET_SCOM                                        ,
                ULL(0x05016851), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_BCDE_STAT_OCI                                        ,
                ULL(0x40020090), SH_UNT          , SH_ACS_OCI       );
CONST_UINT64_T( PU_BCDE_STAT_SCOM                                       ,
                ULL(0x05016852), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_BCUE_CTL_OCI                                         ,
                ULL(0x400200A8), SH_UNT          , SH_ACS_OCI       );
CONST_UINT64_T( PU_BCUE_CTL_SCOM                                        ,
                ULL(0x05016855), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_BCUE_OCIBAR_OCI                                      ,
                ULL(0x400200C8), SH_UNT          , SH_ACS_OCI       );
CONST_UINT64_T( PU_BCUE_OCIBAR_SCOM                                     ,
                ULL(0x05016859), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_BCUE_PBADR_OCI                                       ,
                ULL(0x400200C0), SH_UNT          , SH_ACS_OCI       );
CONST_UINT64_T( PU_BCUE_PBADR_SCOM                                      ,
                ULL(0x05016858), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_BCUE_SET_OCI                                         ,
                ULL(0x400200B0), SH_UNT          , SH_ACS_OCI       );
CONST_UINT64_T( PU_BCUE_SET_SCOM                                        ,
                ULL(0x05016856), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_BCUE_STAT_OCI                                        ,
                ULL(0x400200B8), SH_UNT          , SH_ACS_OCI       );
CONST_UINT64_T( PU_BCUE_STAT_SCOM                                       ,
                ULL(0x05016857), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_NPU0_CTL_BDF2PE_00_CONFIG                            ,
                ULL(0x0501108A), SH_UNT_PU_NPU0_CTL, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_CTL_BDF2PE_00_CONFIG                            ,
                ULL(0x0501118A), SH_UNT_PU_NPU1_CTL, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_CTL_BDF2PE_00_CONFIG                            ,
                ULL(0x0501128A), SH_UNT_PU_NPU2_CTL, SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU0_CTL_BDF2PE_01_CONFIG                            ,
                ULL(0x0501108B), SH_UNT_PU_NPU0_CTL, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_CTL_BDF2PE_01_CONFIG                            ,
                ULL(0x0501118B), SH_UNT_PU_NPU1_CTL, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_CTL_BDF2PE_01_CONFIG                            ,
                ULL(0x0501128B), SH_UNT_PU_NPU2_CTL, SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU0_CTL_BDF2PE_02_CONFIG                            ,
                ULL(0x0501108C), SH_UNT_PU_NPU0_CTL, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_CTL_BDF2PE_02_CONFIG                            ,
                ULL(0x0501118C), SH_UNT_PU_NPU1_CTL, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_CTL_BDF2PE_02_CONFIG                            ,
                ULL(0x0501128C), SH_UNT_PU_NPU2_CTL, SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU0_CTL_BDF2PE_10_CONFIG                            ,
                ULL(0x0501108D), SH_UNT_PU_NPU0_CTL, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_CTL_BDF2PE_10_CONFIG                            ,
                ULL(0x0501118D), SH_UNT_PU_NPU1_CTL, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_CTL_BDF2PE_10_CONFIG                            ,
                ULL(0x0501128D), SH_UNT_PU_NPU2_CTL, SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU0_CTL_BDF2PE_11_CONFIG                            ,
                ULL(0x0501108E), SH_UNT_PU_NPU0_CTL, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_CTL_BDF2PE_11_CONFIG                            ,
                ULL(0x0501118E), SH_UNT_PU_NPU1_CTL, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_CTL_BDF2PE_11_CONFIG                            ,
                ULL(0x0501128E), SH_UNT_PU_NPU2_CTL, SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU0_CTL_BDF2PE_12_CONFIG                            ,
                ULL(0x0501108F), SH_UNT_PU_NPU0_CTL, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_CTL_BDF2PE_12_CONFIG                            ,
                ULL(0x0501118F), SH_UNT_PU_NPU1_CTL, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_CTL_BDF2PE_12_CONFIG                            ,
                ULL(0x0501128F), SH_UNT_PU_NPU2_CTL, SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU0_CERR_ECC_HOLD                                   ,
                ULL(0x050110A4), SH_UNT_PU_NPU0  , SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_CERR_ECC_HOLD                                   ,
                ULL(0x050111A4), SH_UNT_PU_NPU1  , SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_CERR_ECC_HOLD                                   ,
                ULL(0x050112A4), SH_UNT_PU_NPU2  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU0_CERR_ECC_MASK                                   ,
                ULL(0x050110A5), SH_UNT_PU_NPU0  , SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_CERR_ECC_MASK                                   ,
                ULL(0x050111A5), SH_UNT_PU_NPU1  , SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_CERR_ECC_MASK                                   ,
                ULL(0x050112A5), SH_UNT_PU_NPU2  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU0_CERR_LOG_HOLD                                   ,
                ULL(0x050110A8), SH_UNT_PU_NPU0  , SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_CERR_LOG_HOLD                                   ,
                ULL(0x050111A8), SH_UNT_PU_NPU1  , SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_CERR_LOG_HOLD                                   ,
                ULL(0x050112A8), SH_UNT_PU_NPU2  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU0_CERR_LOG_MASK                                   ,
                ULL(0x050110A9), SH_UNT_PU_NPU0  , SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_CERR_LOG_MASK                                   ,
                ULL(0x050111A9), SH_UNT_PU_NPU1  , SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_CERR_LOG_MASK                                   ,
                ULL(0x050112A9), SH_UNT_PU_NPU2  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU0_CERR_PTY_HOLD                                   ,
                ULL(0x050110A6), SH_UNT_PU_NPU0  , SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_CERR_PTY_HOLD                                   ,
                ULL(0x050111A6), SH_UNT_PU_NPU1  , SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_CERR_PTY_HOLD                                   ,
                ULL(0x050112A6), SH_UNT_PU_NPU2  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU0_CERR_PTY_MASK                                   ,
                ULL(0x050110A7), SH_UNT_PU_NPU0  , SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_CERR_PTY_MASK                                   ,
                ULL(0x050111A7), SH_UNT_PU_NPU1  , SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_CERR_PTY_MASK                                   ,
                ULL(0x050112A7), SH_UNT_PU_NPU2  , SH_ACS_SCOM      );

CONST_UINT64_T( PEC_0_STACK0_CERR_RPT0_REG                              ,
                ULL(0x04010C4A), SH_UNT_PEC_0_STACK0, SH_ACS_SCOM_RO   );
CONST_UINT64_T( PEC_0_STACK1_CERR_RPT0_REG                              ,
                ULL(0x04010C8A), SH_UNT_PEC_0_STACK1, SH_ACS_SCOM_RO   );
CONST_UINT64_T( PEC_0_STACK2_CERR_RPT0_REG                              ,
                ULL(0x04010CCA), SH_UNT_PEC_0_STACK2, SH_ACS_SCOM_RO   );
CONST_UINT64_T( PEC_1_STACK0_CERR_RPT0_REG                              ,
                ULL(0x0401104A), SH_UNT_PEC_1_STACK0, SH_ACS_SCOM_RO   );
CONST_UINT64_T( PEC_1_STACK1_CERR_RPT0_REG                              ,
                ULL(0x0401108A), SH_UNT_PEC_1_STACK1, SH_ACS_SCOM_RO   );
CONST_UINT64_T( PEC_1_STACK2_CERR_RPT0_REG                              ,
                ULL(0x040110CA), SH_UNT_PEC_1_STACK2, SH_ACS_SCOM_RO   );
CONST_UINT64_T( PEC_2_STACK0_CERR_RPT0_REG                              ,
                ULL(0x0401144A), SH_UNT_PEC_2_STACK0, SH_ACS_SCOM_RO   );
CONST_UINT64_T( PEC_2_STACK1_CERR_RPT0_REG                              ,
                ULL(0x0401148A), SH_UNT_PEC_2_STACK1, SH_ACS_SCOM_RO   );
CONST_UINT64_T( PEC_2_STACK2_CERR_RPT0_REG                              ,
                ULL(0x040114CA), SH_UNT_PEC_2_STACK2, SH_ACS_SCOM_RO   );
CONST_UINT64_T( PEC_STACK0_CERR_RPT0_REG                                ,
                ULL(0x04010C4A), SH_UNT_PEC_STACK0, SH_ACS_SCOM_RO   );
CONST_UINT64_T( PEC_STACK1_CERR_RPT0_REG                                ,
                ULL(0x04010C8A), SH_UNT_PEC_STACK1, SH_ACS_SCOM_RO   );
CONST_UINT64_T( PEC_STACK2_CERR_RPT0_REG                                ,
                ULL(0x04010CCA), SH_UNT_PEC_STACK2, SH_ACS_SCOM_RO   );

CONST_UINT64_T( PEC_0_STACK0_CERR_RPT1_REG                              ,
                ULL(0x04010C4B), SH_UNT_PEC_0_STACK0, SH_ACS_SCOM_RO   );
CONST_UINT64_T( PEC_0_STACK1_CERR_RPT1_REG                              ,
                ULL(0x04010C8B), SH_UNT_PEC_0_STACK1, SH_ACS_SCOM_RO   );
CONST_UINT64_T( PEC_0_STACK2_CERR_RPT1_REG                              ,
                ULL(0x04010CCB), SH_UNT_PEC_0_STACK2, SH_ACS_SCOM_RO   );
CONST_UINT64_T( PEC_1_STACK0_CERR_RPT1_REG                              ,
                ULL(0x0401104B), SH_UNT_PEC_1_STACK0, SH_ACS_SCOM_RO   );
CONST_UINT64_T( PEC_1_STACK1_CERR_RPT1_REG                              ,
                ULL(0x0401108B), SH_UNT_PEC_1_STACK1, SH_ACS_SCOM_RO   );
CONST_UINT64_T( PEC_1_STACK2_CERR_RPT1_REG                              ,
                ULL(0x040110CB), SH_UNT_PEC_1_STACK2, SH_ACS_SCOM_RO   );
CONST_UINT64_T( PEC_2_STACK0_CERR_RPT1_REG                              ,
                ULL(0x0401144B), SH_UNT_PEC_2_STACK0, SH_ACS_SCOM_RO   );
CONST_UINT64_T( PEC_2_STACK1_CERR_RPT1_REG                              ,
                ULL(0x0401148B), SH_UNT_PEC_2_STACK1, SH_ACS_SCOM_RO   );
CONST_UINT64_T( PEC_2_STACK2_CERR_RPT1_REG                              ,
                ULL(0x040114CB), SH_UNT_PEC_2_STACK2, SH_ACS_SCOM_RO   );
CONST_UINT64_T( PEC_STACK0_CERR_RPT1_REG                                ,
                ULL(0x04010C4B), SH_UNT_PEC_STACK0, SH_ACS_SCOM_RO   );
CONST_UINT64_T( PEC_STACK1_CERR_RPT1_REG                                ,
                ULL(0x04010C8B), SH_UNT_PEC_STACK1, SH_ACS_SCOM_RO   );
CONST_UINT64_T( PEC_STACK2_CERR_RPT1_REG                                ,
                ULL(0x04010CCB), SH_UNT_PEC_STACK2, SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_PIB2OPB0_CMD_WRDAT                                   ,
                ULL(0x00020000), SH_UNT_PU_PIB2OPB0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_PIB2OPB1_CMD_WRDAT                                   ,
                ULL(0x00020010), SH_UNT_PU_PIB2OPB1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_CME0_CME_LCL_DBG_PPE                                 ,
                ULL(0x109010120), SH_UNT_PU_CME0  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME0_CME_LCL_DBG_PPE1                                ,
                ULL(0x109010138), SH_UNT_PU_CME0  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME0_CME_LCL_DBG_PPE2                                ,
                ULL(0x109010130), SH_UNT_PU_CME0  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME1_CME_LCL_DBG_PPE                                 ,
                ULL(0x109020120), SH_UNT_PU_CME1  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME1_CME_LCL_DBG_PPE1                                ,
                ULL(0x109020138), SH_UNT_PU_CME1  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME1_CME_LCL_DBG_PPE2                                ,
                ULL(0x109020130), SH_UNT_PU_CME1  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME2_CME_LCL_DBG_PPE                                 ,
                ULL(0x10A010120), SH_UNT_PU_CME2  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME2_CME_LCL_DBG_PPE1                                ,
                ULL(0x10A010138), SH_UNT_PU_CME2  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME2_CME_LCL_DBG_PPE2                                ,
                ULL(0x10A010130), SH_UNT_PU_CME2  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME3_CME_LCL_DBG_PPE                                 ,
                ULL(0x10A020120), SH_UNT_PU_CME3  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME3_CME_LCL_DBG_PPE1                                ,
                ULL(0x10A020138), SH_UNT_PU_CME3  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME3_CME_LCL_DBG_PPE2                                ,
                ULL(0x10A020130), SH_UNT_PU_CME3  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME4_CME_LCL_DBG_PPE                                 ,
                ULL(0x10B010120), SH_UNT_PU_CME4  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME4_CME_LCL_DBG_PPE1                                ,
                ULL(0x10B010138), SH_UNT_PU_CME4  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME4_CME_LCL_DBG_PPE2                                ,
                ULL(0x10B010130), SH_UNT_PU_CME4  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME5_CME_LCL_DBG_PPE                                 ,
                ULL(0x10B020120), SH_UNT_PU_CME5  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME5_CME_LCL_DBG_PPE1                                ,
                ULL(0x10B020138), SH_UNT_PU_CME5  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME5_CME_LCL_DBG_PPE2                                ,
                ULL(0x10B020130), SH_UNT_PU_CME5  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME6_CME_LCL_DBG_PPE                                 ,
                ULL(0x10C010120), SH_UNT_PU_CME6  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME6_CME_LCL_DBG_PPE1                                ,
                ULL(0x10C010138), SH_UNT_PU_CME6  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME6_CME_LCL_DBG_PPE2                                ,
                ULL(0x10C010130), SH_UNT_PU_CME6  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME7_CME_LCL_DBG_PPE                                 ,
                ULL(0x10C020120), SH_UNT_PU_CME7  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME7_CME_LCL_DBG_PPE1                                ,
                ULL(0x10C020138), SH_UNT_PU_CME7  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME7_CME_LCL_DBG_PPE2                                ,
                ULL(0x10C020130), SH_UNT_PU_CME7  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME8_CME_LCL_DBG_PPE                                 ,
                ULL(0x10D010120), SH_UNT_PU_CME8  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME8_CME_LCL_DBG_PPE1                                ,
                ULL(0x10D010138), SH_UNT_PU_CME8  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME8_CME_LCL_DBG_PPE2                                ,
                ULL(0x10D010130), SH_UNT_PU_CME8  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME9_CME_LCL_DBG_PPE                                 ,
                ULL(0x10D020120), SH_UNT_PU_CME9  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME9_CME_LCL_DBG_PPE1                                ,
                ULL(0x10D020138), SH_UNT_PU_CME9  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME9_CME_LCL_DBG_PPE2                                ,
                ULL(0x10D020130), SH_UNT_PU_CME9  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME10_CME_LCL_DBG_PPE                                ,
                ULL(0x10E010120), SH_UNT_PU_CME10 , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME10_CME_LCL_DBG_PPE1                               ,
                ULL(0x10E010138), SH_UNT_PU_CME10 , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME10_CME_LCL_DBG_PPE2                               ,
                ULL(0x10E010130), SH_UNT_PU_CME10 , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME11_CME_LCL_DBG_PPE                                ,
                ULL(0x10E020120), SH_UNT_PU_CME11 , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME11_CME_LCL_DBG_PPE1                               ,
                ULL(0x10E020138), SH_UNT_PU_CME11 , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME11_CME_LCL_DBG_PPE2                               ,
                ULL(0x10E020130), SH_UNT_PU_CME11 , SH_ACS_PPE2      );

CONST_UINT64_T( PU_CME0_CME_LCL_EIMR_PPE                                ,
                ULL(0x109010020), SH_UNT_PU_CME0  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME0_CME_LCL_EIMR_PPE1                               ,
                ULL(0x109010038), SH_UNT_PU_CME0  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME0_CME_LCL_EIMR_PPE2                               ,
                ULL(0x109010030), SH_UNT_PU_CME0  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME1_CME_LCL_EIMR_PPE                                ,
                ULL(0x109020020), SH_UNT_PU_CME1  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME1_CME_LCL_EIMR_PPE1                               ,
                ULL(0x109020038), SH_UNT_PU_CME1  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME1_CME_LCL_EIMR_PPE2                               ,
                ULL(0x109020030), SH_UNT_PU_CME1  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME2_CME_LCL_EIMR_PPE                                ,
                ULL(0x10A010020), SH_UNT_PU_CME2  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME2_CME_LCL_EIMR_PPE1                               ,
                ULL(0x10A010038), SH_UNT_PU_CME2  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME2_CME_LCL_EIMR_PPE2                               ,
                ULL(0x10A010030), SH_UNT_PU_CME2  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME3_CME_LCL_EIMR_PPE                                ,
                ULL(0x10A020020), SH_UNT_PU_CME3  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME3_CME_LCL_EIMR_PPE1                               ,
                ULL(0x10A020038), SH_UNT_PU_CME3  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME3_CME_LCL_EIMR_PPE2                               ,
                ULL(0x10A020030), SH_UNT_PU_CME3  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME4_CME_LCL_EIMR_PPE                                ,
                ULL(0x10B010020), SH_UNT_PU_CME4  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME4_CME_LCL_EIMR_PPE1                               ,
                ULL(0x10B010038), SH_UNT_PU_CME4  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME4_CME_LCL_EIMR_PPE2                               ,
                ULL(0x10B010030), SH_UNT_PU_CME4  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME5_CME_LCL_EIMR_PPE                                ,
                ULL(0x10B020020), SH_UNT_PU_CME5  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME5_CME_LCL_EIMR_PPE1                               ,
                ULL(0x10B020038), SH_UNT_PU_CME5  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME5_CME_LCL_EIMR_PPE2                               ,
                ULL(0x10B020030), SH_UNT_PU_CME5  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME6_CME_LCL_EIMR_PPE                                ,
                ULL(0x10C010020), SH_UNT_PU_CME6  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME6_CME_LCL_EIMR_PPE1                               ,
                ULL(0x10C010038), SH_UNT_PU_CME6  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME6_CME_LCL_EIMR_PPE2                               ,
                ULL(0x10C010030), SH_UNT_PU_CME6  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME7_CME_LCL_EIMR_PPE                                ,
                ULL(0x10C020020), SH_UNT_PU_CME7  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME7_CME_LCL_EIMR_PPE1                               ,
                ULL(0x10C020038), SH_UNT_PU_CME7  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME7_CME_LCL_EIMR_PPE2                               ,
                ULL(0x10C020030), SH_UNT_PU_CME7  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME8_CME_LCL_EIMR_PPE                                ,
                ULL(0x10D010020), SH_UNT_PU_CME8  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME8_CME_LCL_EIMR_PPE1                               ,
                ULL(0x10D010038), SH_UNT_PU_CME8  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME8_CME_LCL_EIMR_PPE2                               ,
                ULL(0x10D010030), SH_UNT_PU_CME8  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME9_CME_LCL_EIMR_PPE                                ,
                ULL(0x10D020020), SH_UNT_PU_CME9  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME9_CME_LCL_EIMR_PPE1                               ,
                ULL(0x10D020038), SH_UNT_PU_CME9  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME9_CME_LCL_EIMR_PPE2                               ,
                ULL(0x10D020030), SH_UNT_PU_CME9  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME10_CME_LCL_EIMR_PPE                               ,
                ULL(0x10E010020), SH_UNT_PU_CME10 , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME10_CME_LCL_EIMR_PPE1                              ,
                ULL(0x10E010038), SH_UNT_PU_CME10 , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME10_CME_LCL_EIMR_PPE2                              ,
                ULL(0x10E010030), SH_UNT_PU_CME10 , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME11_CME_LCL_EIMR_PPE                               ,
                ULL(0x10E020020), SH_UNT_PU_CME11 , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME11_CME_LCL_EIMR_PPE1                              ,
                ULL(0x10E020038), SH_UNT_PU_CME11 , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME11_CME_LCL_EIMR_PPE2                              ,
                ULL(0x10E020030), SH_UNT_PU_CME11 , SH_ACS_PPE2      );

CONST_UINT64_T( PU_CME0_CME_LCL_EINR_PPE                                ,
                ULL(0x1090100A0), SH_UNT_PU_CME0  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME1_CME_LCL_EINR_PPE                                ,
                ULL(0x1090200A0), SH_UNT_PU_CME1  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME2_CME_LCL_EINR_PPE                                ,
                ULL(0x10A0100A0), SH_UNT_PU_CME2  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME3_CME_LCL_EINR_PPE                                ,
                ULL(0x10A0200A0), SH_UNT_PU_CME3  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME4_CME_LCL_EINR_PPE                                ,
                ULL(0x10B0100A0), SH_UNT_PU_CME4  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME5_CME_LCL_EINR_PPE                                ,
                ULL(0x10B0200A0), SH_UNT_PU_CME5  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME6_CME_LCL_EINR_PPE                                ,
                ULL(0x10C0100A0), SH_UNT_PU_CME6  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME7_CME_LCL_EINR_PPE                                ,
                ULL(0x10C0200A0), SH_UNT_PU_CME7  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME8_CME_LCL_EINR_PPE                                ,
                ULL(0x10D0100A0), SH_UNT_PU_CME8  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME9_CME_LCL_EINR_PPE                                ,
                ULL(0x10D0200A0), SH_UNT_PU_CME9  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME10_CME_LCL_EINR_PPE                               ,
                ULL(0x10E0100A0), SH_UNT_PU_CME10 , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME11_CME_LCL_EINR_PPE                               ,
                ULL(0x10E0200A0), SH_UNT_PU_CME11 , SH_ACS_PPE       );

CONST_UINT64_T( PU_CME0_CME_LCL_EIPR_PPE                                ,
                ULL(0x109010040), SH_UNT_PU_CME0  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME0_CME_LCL_EIPR_PPE1                               ,
                ULL(0x109010058), SH_UNT_PU_CME0  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME0_CME_LCL_EIPR_PPE2                               ,
                ULL(0x109010050), SH_UNT_PU_CME0  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME1_CME_LCL_EIPR_PPE                                ,
                ULL(0x109020040), SH_UNT_PU_CME1  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME1_CME_LCL_EIPR_PPE1                               ,
                ULL(0x109020058), SH_UNT_PU_CME1  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME1_CME_LCL_EIPR_PPE2                               ,
                ULL(0x109020050), SH_UNT_PU_CME1  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME2_CME_LCL_EIPR_PPE                                ,
                ULL(0x10A010040), SH_UNT_PU_CME2  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME2_CME_LCL_EIPR_PPE1                               ,
                ULL(0x10A010058), SH_UNT_PU_CME2  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME2_CME_LCL_EIPR_PPE2                               ,
                ULL(0x10A010050), SH_UNT_PU_CME2  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME3_CME_LCL_EIPR_PPE                                ,
                ULL(0x10A020040), SH_UNT_PU_CME3  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME3_CME_LCL_EIPR_PPE1                               ,
                ULL(0x10A020058), SH_UNT_PU_CME3  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME3_CME_LCL_EIPR_PPE2                               ,
                ULL(0x10A020050), SH_UNT_PU_CME3  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME4_CME_LCL_EIPR_PPE                                ,
                ULL(0x10B010040), SH_UNT_PU_CME4  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME4_CME_LCL_EIPR_PPE1                               ,
                ULL(0x10B010058), SH_UNT_PU_CME4  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME4_CME_LCL_EIPR_PPE2                               ,
                ULL(0x10B010050), SH_UNT_PU_CME4  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME5_CME_LCL_EIPR_PPE                                ,
                ULL(0x10B020040), SH_UNT_PU_CME5  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME5_CME_LCL_EIPR_PPE1                               ,
                ULL(0x10B020058), SH_UNT_PU_CME5  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME5_CME_LCL_EIPR_PPE2                               ,
                ULL(0x10B020050), SH_UNT_PU_CME5  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME6_CME_LCL_EIPR_PPE                                ,
                ULL(0x10C010040), SH_UNT_PU_CME6  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME6_CME_LCL_EIPR_PPE1                               ,
                ULL(0x10C010058), SH_UNT_PU_CME6  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME6_CME_LCL_EIPR_PPE2                               ,
                ULL(0x10C010050), SH_UNT_PU_CME6  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME7_CME_LCL_EIPR_PPE                                ,
                ULL(0x10C020040), SH_UNT_PU_CME7  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME7_CME_LCL_EIPR_PPE1                               ,
                ULL(0x10C020058), SH_UNT_PU_CME7  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME7_CME_LCL_EIPR_PPE2                               ,
                ULL(0x10C020050), SH_UNT_PU_CME7  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME8_CME_LCL_EIPR_PPE                                ,
                ULL(0x10D010040), SH_UNT_PU_CME8  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME8_CME_LCL_EIPR_PPE1                               ,
                ULL(0x10D010058), SH_UNT_PU_CME8  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME8_CME_LCL_EIPR_PPE2                               ,
                ULL(0x10D010050), SH_UNT_PU_CME8  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME9_CME_LCL_EIPR_PPE                                ,
                ULL(0x10D020040), SH_UNT_PU_CME9  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME9_CME_LCL_EIPR_PPE1                               ,
                ULL(0x10D020058), SH_UNT_PU_CME9  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME9_CME_LCL_EIPR_PPE2                               ,
                ULL(0x10D020050), SH_UNT_PU_CME9  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME10_CME_LCL_EIPR_PPE                               ,
                ULL(0x10E010040), SH_UNT_PU_CME10 , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME10_CME_LCL_EIPR_PPE1                              ,
                ULL(0x10E010058), SH_UNT_PU_CME10 , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME10_CME_LCL_EIPR_PPE2                              ,
                ULL(0x10E010050), SH_UNT_PU_CME10 , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME11_CME_LCL_EIPR_PPE                               ,
                ULL(0x10E020040), SH_UNT_PU_CME11 , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME11_CME_LCL_EIPR_PPE1                              ,
                ULL(0x10E020058), SH_UNT_PU_CME11 , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME11_CME_LCL_EIPR_PPE2                              ,
                ULL(0x10E020050), SH_UNT_PU_CME11 , SH_ACS_PPE2      );

CONST_UINT64_T( PU_CME0_CME_LCL_EISR_PPE                                ,
                ULL(0x109010000), SH_UNT_PU_CME0  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME0_CME_LCL_EISR_PPE1                               ,
                ULL(0x109010018), SH_UNT_PU_CME0  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME0_CME_LCL_EISR_PPE2                               ,
                ULL(0x109010010), SH_UNT_PU_CME0  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME1_CME_LCL_EISR_PPE                                ,
                ULL(0x109020000), SH_UNT_PU_CME1  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME1_CME_LCL_EISR_PPE1                               ,
                ULL(0x109020018), SH_UNT_PU_CME1  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME1_CME_LCL_EISR_PPE2                               ,
                ULL(0x109020010), SH_UNT_PU_CME1  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME2_CME_LCL_EISR_PPE                                ,
                ULL(0x10A010000), SH_UNT_PU_CME2  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME2_CME_LCL_EISR_PPE1                               ,
                ULL(0x10A010018), SH_UNT_PU_CME2  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME2_CME_LCL_EISR_PPE2                               ,
                ULL(0x10A010010), SH_UNT_PU_CME2  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME3_CME_LCL_EISR_PPE                                ,
                ULL(0x10A020000), SH_UNT_PU_CME3  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME3_CME_LCL_EISR_PPE1                               ,
                ULL(0x10A020018), SH_UNT_PU_CME3  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME3_CME_LCL_EISR_PPE2                               ,
                ULL(0x10A020010), SH_UNT_PU_CME3  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME4_CME_LCL_EISR_PPE                                ,
                ULL(0x10B010000), SH_UNT_PU_CME4  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME4_CME_LCL_EISR_PPE1                               ,
                ULL(0x10B010018), SH_UNT_PU_CME4  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME4_CME_LCL_EISR_PPE2                               ,
                ULL(0x10B010010), SH_UNT_PU_CME4  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME5_CME_LCL_EISR_PPE                                ,
                ULL(0x10B020000), SH_UNT_PU_CME5  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME5_CME_LCL_EISR_PPE1                               ,
                ULL(0x10B020018), SH_UNT_PU_CME5  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME5_CME_LCL_EISR_PPE2                               ,
                ULL(0x10B020010), SH_UNT_PU_CME5  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME6_CME_LCL_EISR_PPE                                ,
                ULL(0x10C010000), SH_UNT_PU_CME6  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME6_CME_LCL_EISR_PPE1                               ,
                ULL(0x10C010018), SH_UNT_PU_CME6  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME6_CME_LCL_EISR_PPE2                               ,
                ULL(0x10C010010), SH_UNT_PU_CME6  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME7_CME_LCL_EISR_PPE                                ,
                ULL(0x10C020000), SH_UNT_PU_CME7  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME7_CME_LCL_EISR_PPE1                               ,
                ULL(0x10C020018), SH_UNT_PU_CME7  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME7_CME_LCL_EISR_PPE2                               ,
                ULL(0x10C020010), SH_UNT_PU_CME7  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME8_CME_LCL_EISR_PPE                                ,
                ULL(0x10D010000), SH_UNT_PU_CME8  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME8_CME_LCL_EISR_PPE1                               ,
                ULL(0x10D010018), SH_UNT_PU_CME8  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME8_CME_LCL_EISR_PPE2                               ,
                ULL(0x10D010010), SH_UNT_PU_CME8  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME9_CME_LCL_EISR_PPE                                ,
                ULL(0x10D020000), SH_UNT_PU_CME9  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME9_CME_LCL_EISR_PPE1                               ,
                ULL(0x10D020018), SH_UNT_PU_CME9  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME9_CME_LCL_EISR_PPE2                               ,
                ULL(0x10D020010), SH_UNT_PU_CME9  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME10_CME_LCL_EISR_PPE                               ,
                ULL(0x10E010000), SH_UNT_PU_CME10 , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME10_CME_LCL_EISR_PPE1                              ,
                ULL(0x10E010018), SH_UNT_PU_CME10 , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME10_CME_LCL_EISR_PPE2                              ,
                ULL(0x10E010010), SH_UNT_PU_CME10 , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME11_CME_LCL_EISR_PPE                               ,
                ULL(0x10E020000), SH_UNT_PU_CME11 , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME11_CME_LCL_EISR_PPE1                              ,
                ULL(0x10E020018), SH_UNT_PU_CME11 , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME11_CME_LCL_EISR_PPE2                              ,
                ULL(0x10E020010), SH_UNT_PU_CME11 , SH_ACS_PPE2      );

CONST_UINT64_T( PU_CME0_CME_LCL_EISTR_PPE                               ,
                ULL(0x109010080), SH_UNT_PU_CME0  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME1_CME_LCL_EISTR_PPE                               ,
                ULL(0x109020080), SH_UNT_PU_CME1  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME2_CME_LCL_EISTR_PPE                               ,
                ULL(0x10A010080), SH_UNT_PU_CME2  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME3_CME_LCL_EISTR_PPE                               ,
                ULL(0x10A020080), SH_UNT_PU_CME3  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME4_CME_LCL_EISTR_PPE                               ,
                ULL(0x10B010080), SH_UNT_PU_CME4  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME5_CME_LCL_EISTR_PPE                               ,
                ULL(0x10B020080), SH_UNT_PU_CME5  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME6_CME_LCL_EISTR_PPE                               ,
                ULL(0x10C010080), SH_UNT_PU_CME6  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME7_CME_LCL_EISTR_PPE                               ,
                ULL(0x10C020080), SH_UNT_PU_CME7  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME8_CME_LCL_EISTR_PPE                               ,
                ULL(0x10D010080), SH_UNT_PU_CME8  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME9_CME_LCL_EISTR_PPE                               ,
                ULL(0x10D020080), SH_UNT_PU_CME9  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME10_CME_LCL_EISTR_PPE                              ,
                ULL(0x10E010080), SH_UNT_PU_CME10 , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME11_CME_LCL_EISTR_PPE                              ,
                ULL(0x10E020080), SH_UNT_PU_CME11 , SH_ACS_PPE       );

CONST_UINT64_T( PU_CME0_CME_LCL_EITR_PPE                                ,
                ULL(0x109010060), SH_UNT_PU_CME0  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME0_CME_LCL_EITR_PPE1                               ,
                ULL(0x109010078), SH_UNT_PU_CME0  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME0_CME_LCL_EITR_PPE2                               ,
                ULL(0x109010070), SH_UNT_PU_CME0  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME1_CME_LCL_EITR_PPE                                ,
                ULL(0x109020060), SH_UNT_PU_CME1  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME1_CME_LCL_EITR_PPE1                               ,
                ULL(0x109020078), SH_UNT_PU_CME1  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME1_CME_LCL_EITR_PPE2                               ,
                ULL(0x109020070), SH_UNT_PU_CME1  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME2_CME_LCL_EITR_PPE                                ,
                ULL(0x10A010060), SH_UNT_PU_CME2  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME2_CME_LCL_EITR_PPE1                               ,
                ULL(0x10A010078), SH_UNT_PU_CME2  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME2_CME_LCL_EITR_PPE2                               ,
                ULL(0x10A010070), SH_UNT_PU_CME2  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME3_CME_LCL_EITR_PPE                                ,
                ULL(0x10A020060), SH_UNT_PU_CME3  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME3_CME_LCL_EITR_PPE1                               ,
                ULL(0x10A020078), SH_UNT_PU_CME3  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME3_CME_LCL_EITR_PPE2                               ,
                ULL(0x10A020070), SH_UNT_PU_CME3  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME4_CME_LCL_EITR_PPE                                ,
                ULL(0x10B010060), SH_UNT_PU_CME4  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME4_CME_LCL_EITR_PPE1                               ,
                ULL(0x10B010078), SH_UNT_PU_CME4  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME4_CME_LCL_EITR_PPE2                               ,
                ULL(0x10B010070), SH_UNT_PU_CME4  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME5_CME_LCL_EITR_PPE                                ,
                ULL(0x10B020060), SH_UNT_PU_CME5  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME5_CME_LCL_EITR_PPE1                               ,
                ULL(0x10B020078), SH_UNT_PU_CME5  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME5_CME_LCL_EITR_PPE2                               ,
                ULL(0x10B020070), SH_UNT_PU_CME5  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME6_CME_LCL_EITR_PPE                                ,
                ULL(0x10C010060), SH_UNT_PU_CME6  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME6_CME_LCL_EITR_PPE1                               ,
                ULL(0x10C010078), SH_UNT_PU_CME6  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME6_CME_LCL_EITR_PPE2                               ,
                ULL(0x10C010070), SH_UNT_PU_CME6  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME7_CME_LCL_EITR_PPE                                ,
                ULL(0x10C020060), SH_UNT_PU_CME7  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME7_CME_LCL_EITR_PPE1                               ,
                ULL(0x10C020078), SH_UNT_PU_CME7  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME7_CME_LCL_EITR_PPE2                               ,
                ULL(0x10C020070), SH_UNT_PU_CME7  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME8_CME_LCL_EITR_PPE                                ,
                ULL(0x10D010060), SH_UNT_PU_CME8  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME8_CME_LCL_EITR_PPE1                               ,
                ULL(0x10D010078), SH_UNT_PU_CME8  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME8_CME_LCL_EITR_PPE2                               ,
                ULL(0x10D010070), SH_UNT_PU_CME8  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME9_CME_LCL_EITR_PPE                                ,
                ULL(0x10D020060), SH_UNT_PU_CME9  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME9_CME_LCL_EITR_PPE1                               ,
                ULL(0x10D020078), SH_UNT_PU_CME9  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME9_CME_LCL_EITR_PPE2                               ,
                ULL(0x10D020070), SH_UNT_PU_CME9  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME10_CME_LCL_EITR_PPE                               ,
                ULL(0x10E010060), SH_UNT_PU_CME10 , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME10_CME_LCL_EITR_PPE1                              ,
                ULL(0x10E010078), SH_UNT_PU_CME10 , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME10_CME_LCL_EITR_PPE2                              ,
                ULL(0x10E010070), SH_UNT_PU_CME10 , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME11_CME_LCL_EITR_PPE                               ,
                ULL(0x10E020060), SH_UNT_PU_CME11 , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME11_CME_LCL_EITR_PPE1                              ,
                ULL(0x10E020078), SH_UNT_PU_CME11 , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME11_CME_LCL_EITR_PPE2                              ,
                ULL(0x10E020070), SH_UNT_PU_CME11 , SH_ACS_PPE2      );

CONST_UINT64_T( PU_CME0_CME_LCL_ICCR_PPE                                ,
                ULL(0x109010700), SH_UNT_PU_CME0  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME0_CME_LCL_ICCR_PPE1                               ,
                ULL(0x109010718), SH_UNT_PU_CME0  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME0_CME_LCL_ICCR_PPE2                               ,
                ULL(0x109010710), SH_UNT_PU_CME0  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME1_CME_LCL_ICCR_PPE                                ,
                ULL(0x109020700), SH_UNT_PU_CME1  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME1_CME_LCL_ICCR_PPE1                               ,
                ULL(0x109020718), SH_UNT_PU_CME1  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME1_CME_LCL_ICCR_PPE2                               ,
                ULL(0x109020710), SH_UNT_PU_CME1  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME2_CME_LCL_ICCR_PPE                                ,
                ULL(0x10A010700), SH_UNT_PU_CME2  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME2_CME_LCL_ICCR_PPE1                               ,
                ULL(0x10A010718), SH_UNT_PU_CME2  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME2_CME_LCL_ICCR_PPE2                               ,
                ULL(0x10A010710), SH_UNT_PU_CME2  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME3_CME_LCL_ICCR_PPE                                ,
                ULL(0x10A020700), SH_UNT_PU_CME3  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME3_CME_LCL_ICCR_PPE1                               ,
                ULL(0x10A020718), SH_UNT_PU_CME3  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME3_CME_LCL_ICCR_PPE2                               ,
                ULL(0x10A020710), SH_UNT_PU_CME3  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME4_CME_LCL_ICCR_PPE                                ,
                ULL(0x10B010700), SH_UNT_PU_CME4  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME4_CME_LCL_ICCR_PPE1                               ,
                ULL(0x10B010718), SH_UNT_PU_CME4  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME4_CME_LCL_ICCR_PPE2                               ,
                ULL(0x10B010710), SH_UNT_PU_CME4  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME5_CME_LCL_ICCR_PPE                                ,
                ULL(0x10B020700), SH_UNT_PU_CME5  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME5_CME_LCL_ICCR_PPE1                               ,
                ULL(0x10B020718), SH_UNT_PU_CME5  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME5_CME_LCL_ICCR_PPE2                               ,
                ULL(0x10B020710), SH_UNT_PU_CME5  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME6_CME_LCL_ICCR_PPE                                ,
                ULL(0x10C010700), SH_UNT_PU_CME6  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME6_CME_LCL_ICCR_PPE1                               ,
                ULL(0x10C010718), SH_UNT_PU_CME6  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME6_CME_LCL_ICCR_PPE2                               ,
                ULL(0x10C010710), SH_UNT_PU_CME6  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME7_CME_LCL_ICCR_PPE                                ,
                ULL(0x10C020700), SH_UNT_PU_CME7  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME7_CME_LCL_ICCR_PPE1                               ,
                ULL(0x10C020718), SH_UNT_PU_CME7  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME7_CME_LCL_ICCR_PPE2                               ,
                ULL(0x10C020710), SH_UNT_PU_CME7  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME8_CME_LCL_ICCR_PPE                                ,
                ULL(0x10D010700), SH_UNT_PU_CME8  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME8_CME_LCL_ICCR_PPE1                               ,
                ULL(0x10D010718), SH_UNT_PU_CME8  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME8_CME_LCL_ICCR_PPE2                               ,
                ULL(0x10D010710), SH_UNT_PU_CME8  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME9_CME_LCL_ICCR_PPE                                ,
                ULL(0x10D020700), SH_UNT_PU_CME9  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME9_CME_LCL_ICCR_PPE1                               ,
                ULL(0x10D020718), SH_UNT_PU_CME9  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME9_CME_LCL_ICCR_PPE2                               ,
                ULL(0x10D020710), SH_UNT_PU_CME9  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME10_CME_LCL_ICCR_PPE                               ,
                ULL(0x10E010700), SH_UNT_PU_CME10 , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME10_CME_LCL_ICCR_PPE1                              ,
                ULL(0x10E010718), SH_UNT_PU_CME10 , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME10_CME_LCL_ICCR_PPE2                              ,
                ULL(0x10E010710), SH_UNT_PU_CME10 , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME11_CME_LCL_ICCR_PPE                               ,
                ULL(0x10E020700), SH_UNT_PU_CME11 , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME11_CME_LCL_ICCR_PPE1                              ,
                ULL(0x10E020718), SH_UNT_PU_CME11 , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME11_CME_LCL_ICCR_PPE2                              ,
                ULL(0x10E020710), SH_UNT_PU_CME11 , SH_ACS_PPE2      );

CONST_UINT64_T( PU_CME0_CME_LCL_ICRR_PPE                                ,
                ULL(0x109010740), SH_UNT_PU_CME0  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME1_CME_LCL_ICRR_PPE                                ,
                ULL(0x109020740), SH_UNT_PU_CME1  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME2_CME_LCL_ICRR_PPE                                ,
                ULL(0x10A010740), SH_UNT_PU_CME2  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME3_CME_LCL_ICRR_PPE                                ,
                ULL(0x10A020740), SH_UNT_PU_CME3  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME4_CME_LCL_ICRR_PPE                                ,
                ULL(0x10B010740), SH_UNT_PU_CME4  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME5_CME_LCL_ICRR_PPE                                ,
                ULL(0x10B020740), SH_UNT_PU_CME5  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME6_CME_LCL_ICRR_PPE                                ,
                ULL(0x10C010740), SH_UNT_PU_CME6  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME7_CME_LCL_ICRR_PPE                                ,
                ULL(0x10C020740), SH_UNT_PU_CME7  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME8_CME_LCL_ICRR_PPE                                ,
                ULL(0x10D010740), SH_UNT_PU_CME8  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME9_CME_LCL_ICRR_PPE                                ,
                ULL(0x10D020740), SH_UNT_PU_CME9  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME10_CME_LCL_ICRR_PPE                               ,
                ULL(0x10E010740), SH_UNT_PU_CME10 , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME11_CME_LCL_ICRR_PPE                               ,
                ULL(0x10E020740), SH_UNT_PU_CME11 , SH_ACS_PPE       );

CONST_UINT64_T( PU_CME0_CME_LCL_ICSR_PPE                                ,
                ULL(0x109010720), SH_UNT_PU_CME0  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME1_CME_LCL_ICSR_PPE                                ,
                ULL(0x109020720), SH_UNT_PU_CME1  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME2_CME_LCL_ICSR_PPE                                ,
                ULL(0x10A010720), SH_UNT_PU_CME2  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME3_CME_LCL_ICSR_PPE                                ,
                ULL(0x10A020720), SH_UNT_PU_CME3  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME4_CME_LCL_ICSR_PPE                                ,
                ULL(0x10B010720), SH_UNT_PU_CME4  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME5_CME_LCL_ICSR_PPE                                ,
                ULL(0x10B020720), SH_UNT_PU_CME5  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME6_CME_LCL_ICSR_PPE                                ,
                ULL(0x10C010720), SH_UNT_PU_CME6  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME7_CME_LCL_ICSR_PPE                                ,
                ULL(0x10C020720), SH_UNT_PU_CME7  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME8_CME_LCL_ICSR_PPE                                ,
                ULL(0x10D010720), SH_UNT_PU_CME8  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME9_CME_LCL_ICSR_PPE                                ,
                ULL(0x10D020720), SH_UNT_PU_CME9  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME10_CME_LCL_ICSR_PPE                               ,
                ULL(0x10E010720), SH_UNT_PU_CME10 , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME11_CME_LCL_ICSR_PPE                               ,
                ULL(0x10E020720), SH_UNT_PU_CME11 , SH_ACS_PPE       );

CONST_UINT64_T( PU_CME0_CME_LCL_LMCR_PPE                                ,
                ULL(0x1090101A0), SH_UNT_PU_CME0  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME1_CME_LCL_LMCR_PPE                                ,
                ULL(0x1090201A0), SH_UNT_PU_CME1  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME2_CME_LCL_LMCR_PPE                                ,
                ULL(0x10A0101A0), SH_UNT_PU_CME2  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME3_CME_LCL_LMCR_PPE                                ,
                ULL(0x10A0201A0), SH_UNT_PU_CME3  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME4_CME_LCL_LMCR_PPE                                ,
                ULL(0x10B0101A0), SH_UNT_PU_CME4  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME5_CME_LCL_LMCR_PPE                                ,
                ULL(0x10B0201A0), SH_UNT_PU_CME5  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME6_CME_LCL_LMCR_PPE                                ,
                ULL(0x10C0101A0), SH_UNT_PU_CME6  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME7_CME_LCL_LMCR_PPE                                ,
                ULL(0x10C0201A0), SH_UNT_PU_CME7  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME8_CME_LCL_LMCR_PPE                                ,
                ULL(0x10D0101A0), SH_UNT_PU_CME8  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME9_CME_LCL_LMCR_PPE                                ,
                ULL(0x10D0201A0), SH_UNT_PU_CME9  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME10_CME_LCL_LMCR_PPE                               ,
                ULL(0x10E0101A0), SH_UNT_PU_CME10 , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME11_CME_LCL_LMCR_PPE                               ,
                ULL(0x10E0201A0), SH_UNT_PU_CME11 , SH_ACS_PPE       );

CONST_UINT64_T( PU_CME0_CME_LCL_PECESR0_PPE                             ,
                ULL(0x109010280), SH_UNT_PU_CME0  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME1_CME_LCL_PECESR0_PPE                             ,
                ULL(0x109020280), SH_UNT_PU_CME1  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME2_CME_LCL_PECESR0_PPE                             ,
                ULL(0x10A010280), SH_UNT_PU_CME2  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME3_CME_LCL_PECESR0_PPE                             ,
                ULL(0x10A020280), SH_UNT_PU_CME3  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME4_CME_LCL_PECESR0_PPE                             ,
                ULL(0x10B010280), SH_UNT_PU_CME4  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME5_CME_LCL_PECESR0_PPE                             ,
                ULL(0x10B020280), SH_UNT_PU_CME5  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME6_CME_LCL_PECESR0_PPE                             ,
                ULL(0x10C010280), SH_UNT_PU_CME6  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME7_CME_LCL_PECESR0_PPE                             ,
                ULL(0x10C020280), SH_UNT_PU_CME7  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME8_CME_LCL_PECESR0_PPE                             ,
                ULL(0x10D010280), SH_UNT_PU_CME8  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME9_CME_LCL_PECESR0_PPE                             ,
                ULL(0x10D020280), SH_UNT_PU_CME9  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME10_CME_LCL_PECESR0_PPE                            ,
                ULL(0x10E010280), SH_UNT_PU_CME10 , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME11_CME_LCL_PECESR0_PPE                            ,
                ULL(0x10E020280), SH_UNT_PU_CME11 , SH_ACS_PPE       );

CONST_UINT64_T( PU_CME0_CME_LCL_PECESR1_PPE                             ,
                ULL(0x1090102A0), SH_UNT_PU_CME0  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME1_CME_LCL_PECESR1_PPE                             ,
                ULL(0x1090202A0), SH_UNT_PU_CME1  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME2_CME_LCL_PECESR1_PPE                             ,
                ULL(0x10A0102A0), SH_UNT_PU_CME2  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME3_CME_LCL_PECESR1_PPE                             ,
                ULL(0x10A0202A0), SH_UNT_PU_CME3  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME4_CME_LCL_PECESR1_PPE                             ,
                ULL(0x10B0102A0), SH_UNT_PU_CME4  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME5_CME_LCL_PECESR1_PPE                             ,
                ULL(0x10B0202A0), SH_UNT_PU_CME5  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME6_CME_LCL_PECESR1_PPE                             ,
                ULL(0x10C0102A0), SH_UNT_PU_CME6  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME7_CME_LCL_PECESR1_PPE                             ,
                ULL(0x10C0202A0), SH_UNT_PU_CME7  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME8_CME_LCL_PECESR1_PPE                             ,
                ULL(0x10D0102A0), SH_UNT_PU_CME8  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME9_CME_LCL_PECESR1_PPE                             ,
                ULL(0x10D0202A0), SH_UNT_PU_CME9  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME10_CME_LCL_PECESR1_PPE                            ,
                ULL(0x10E0102A0), SH_UNT_PU_CME10 , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME11_CME_LCL_PECESR1_PPE                            ,
                ULL(0x10E0202A0), SH_UNT_PU_CME11 , SH_ACS_PPE       );

CONST_UINT64_T( PU_CME0_CME_LCL_SISR_PPE                                ,
                ULL(0x109010520), SH_UNT_PU_CME0  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME1_CME_LCL_SISR_PPE                                ,
                ULL(0x109020520), SH_UNT_PU_CME1  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME2_CME_LCL_SISR_PPE                                ,
                ULL(0x10A010520), SH_UNT_PU_CME2  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME3_CME_LCL_SISR_PPE                                ,
                ULL(0x10A020520), SH_UNT_PU_CME3  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME4_CME_LCL_SISR_PPE                                ,
                ULL(0x10B010520), SH_UNT_PU_CME4  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME5_CME_LCL_SISR_PPE                                ,
                ULL(0x10B020520), SH_UNT_PU_CME5  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME6_CME_LCL_SISR_PPE                                ,
                ULL(0x10C010520), SH_UNT_PU_CME6  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME7_CME_LCL_SISR_PPE                                ,
                ULL(0x10C020520), SH_UNT_PU_CME7  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME8_CME_LCL_SISR_PPE                                ,
                ULL(0x10D010520), SH_UNT_PU_CME8  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME9_CME_LCL_SISR_PPE                                ,
                ULL(0x10D020520), SH_UNT_PU_CME9  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME10_CME_LCL_SISR_PPE                               ,
                ULL(0x10E010520), SH_UNT_PU_CME10 , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME11_CME_LCL_SISR_PPE                               ,
                ULL(0x10E020520), SH_UNT_PU_CME11 , SH_ACS_PPE       );

CONST_UINT64_T( PU_CME0_CME_LCL_TSEL_PPE                                ,
                ULL(0x109010100), SH_UNT_PU_CME0  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME1_CME_LCL_TSEL_PPE                                ,
                ULL(0x109020100), SH_UNT_PU_CME1  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME2_CME_LCL_TSEL_PPE                                ,
                ULL(0x10A010100), SH_UNT_PU_CME2  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME3_CME_LCL_TSEL_PPE                                ,
                ULL(0x10A020100), SH_UNT_PU_CME3  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME4_CME_LCL_TSEL_PPE                                ,
                ULL(0x10B010100), SH_UNT_PU_CME4  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME5_CME_LCL_TSEL_PPE                                ,
                ULL(0x10B020100), SH_UNT_PU_CME5  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME6_CME_LCL_TSEL_PPE                                ,
                ULL(0x10C010100), SH_UNT_PU_CME6  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME7_CME_LCL_TSEL_PPE                                ,
                ULL(0x10C020100), SH_UNT_PU_CME7  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME8_CME_LCL_TSEL_PPE                                ,
                ULL(0x10D010100), SH_UNT_PU_CME8  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME9_CME_LCL_TSEL_PPE                                ,
                ULL(0x10D020100), SH_UNT_PU_CME9  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME10_CME_LCL_TSEL_PPE                               ,
                ULL(0x10E010100), SH_UNT_PU_CME10 , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME11_CME_LCL_TSEL_PPE                               ,
                ULL(0x10E020100), SH_UNT_PU_CME11 , SH_ACS_PPE       );

CONST_UINT64_T( PU_CME0_CME_SCOM_AFSR_PPE                               ,
                ULL(0x109010160), SH_UNT_PU_CME0  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME1_CME_SCOM_AFSR_PPE                               ,
                ULL(0x109020160), SH_UNT_PU_CME1  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME2_CME_SCOM_AFSR_PPE                               ,
                ULL(0x10A010160), SH_UNT_PU_CME2  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME3_CME_SCOM_AFSR_PPE                               ,
                ULL(0x10A020160), SH_UNT_PU_CME3  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME4_CME_SCOM_AFSR_PPE                               ,
                ULL(0x10B010160), SH_UNT_PU_CME4  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME5_CME_SCOM_AFSR_PPE                               ,
                ULL(0x10B020160), SH_UNT_PU_CME5  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME6_CME_SCOM_AFSR_PPE                               ,
                ULL(0x10C010160), SH_UNT_PU_CME6  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME7_CME_SCOM_AFSR_PPE                               ,
                ULL(0x10C020160), SH_UNT_PU_CME7  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME8_CME_SCOM_AFSR_PPE                               ,
                ULL(0x10D010160), SH_UNT_PU_CME8  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME9_CME_SCOM_AFSR_PPE                               ,
                ULL(0x10D020160), SH_UNT_PU_CME9  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME10_CME_SCOM_AFSR_PPE                              ,
                ULL(0x10E010160), SH_UNT_PU_CME10 , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME11_CME_SCOM_AFSR_PPE                              ,
                ULL(0x10E020160), SH_UNT_PU_CME11 , SH_ACS_PPE       );

CONST_UINT64_T( PU_CME0_CME_SCOM_AFTR_PPE                               ,
                ULL(0x109010180), SH_UNT_PU_CME0  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME1_CME_SCOM_AFTR_PPE                               ,
                ULL(0x109020180), SH_UNT_PU_CME1  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME2_CME_SCOM_AFTR_PPE                               ,
                ULL(0x10A010180), SH_UNT_PU_CME2  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME3_CME_SCOM_AFTR_PPE                               ,
                ULL(0x10A020180), SH_UNT_PU_CME3  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME4_CME_SCOM_AFTR_PPE                               ,
                ULL(0x10B010180), SH_UNT_PU_CME4  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME5_CME_SCOM_AFTR_PPE                               ,
                ULL(0x10B020180), SH_UNT_PU_CME5  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME6_CME_SCOM_AFTR_PPE                               ,
                ULL(0x10C010180), SH_UNT_PU_CME6  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME7_CME_SCOM_AFTR_PPE                               ,
                ULL(0x10C020180), SH_UNT_PU_CME7  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME8_CME_SCOM_AFTR_PPE                               ,
                ULL(0x10D010180), SH_UNT_PU_CME8  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME9_CME_SCOM_AFTR_PPE                               ,
                ULL(0x10D020180), SH_UNT_PU_CME9  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME10_CME_SCOM_AFTR_PPE                              ,
                ULL(0x10E010180), SH_UNT_PU_CME10 , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME11_CME_SCOM_AFTR_PPE                              ,
                ULL(0x10E020180), SH_UNT_PU_CME11 , SH_ACS_PPE       );

CONST_UINT64_T( PU_CME0_CME_SCOM_BCECSR_PPE                             ,
                ULL(0x1090101F0), SH_UNT_PU_CME0  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME1_CME_SCOM_BCECSR_PPE                             ,
                ULL(0x1090201F0), SH_UNT_PU_CME1  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME2_CME_SCOM_BCECSR_PPE                             ,
                ULL(0x10A0101F0), SH_UNT_PU_CME2  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME3_CME_SCOM_BCECSR_PPE                             ,
                ULL(0x10A0201F0), SH_UNT_PU_CME3  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME4_CME_SCOM_BCECSR_PPE                             ,
                ULL(0x10B0101F0), SH_UNT_PU_CME4  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME5_CME_SCOM_BCECSR_PPE                             ,
                ULL(0x10B0201F0), SH_UNT_PU_CME5  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME6_CME_SCOM_BCECSR_PPE                             ,
                ULL(0x10C0101F0), SH_UNT_PU_CME6  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME7_CME_SCOM_BCECSR_PPE                             ,
                ULL(0x10C0201F0), SH_UNT_PU_CME7  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME8_CME_SCOM_BCECSR_PPE                             ,
                ULL(0x10D0101F0), SH_UNT_PU_CME8  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME9_CME_SCOM_BCECSR_PPE                             ,
                ULL(0x10D0201F0), SH_UNT_PU_CME9  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME10_CME_SCOM_BCECSR_PPE                            ,
                ULL(0x10E0101F0), SH_UNT_PU_CME10 , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME11_CME_SCOM_BCECSR_PPE                            ,
                ULL(0x10E0201F0), SH_UNT_PU_CME11 , SH_ACS_PPE       );

CONST_UINT64_T( PU_CME0_CME_SCOM_FLAGS_PPE                              ,
                ULL(0x109010400), SH_UNT_PU_CME0  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME0_CME_SCOM_FLAGS_PPE1                             ,
                ULL(0x109010418), SH_UNT_PU_CME0  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME0_CME_SCOM_FLAGS_PPE2                             ,
                ULL(0x109010410), SH_UNT_PU_CME0  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME1_CME_SCOM_FLAGS_PPE                              ,
                ULL(0x109020400), SH_UNT_PU_CME1  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME1_CME_SCOM_FLAGS_PPE1                             ,
                ULL(0x109020418), SH_UNT_PU_CME1  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME1_CME_SCOM_FLAGS_PPE2                             ,
                ULL(0x109020410), SH_UNT_PU_CME1  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME2_CME_SCOM_FLAGS_PPE                              ,
                ULL(0x10A010400), SH_UNT_PU_CME2  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME2_CME_SCOM_FLAGS_PPE1                             ,
                ULL(0x10A010418), SH_UNT_PU_CME2  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME2_CME_SCOM_FLAGS_PPE2                             ,
                ULL(0x10A010410), SH_UNT_PU_CME2  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME3_CME_SCOM_FLAGS_PPE                              ,
                ULL(0x10A020400), SH_UNT_PU_CME3  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME3_CME_SCOM_FLAGS_PPE1                             ,
                ULL(0x10A020418), SH_UNT_PU_CME3  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME3_CME_SCOM_FLAGS_PPE2                             ,
                ULL(0x10A020410), SH_UNT_PU_CME3  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME4_CME_SCOM_FLAGS_PPE                              ,
                ULL(0x10B010400), SH_UNT_PU_CME4  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME4_CME_SCOM_FLAGS_PPE1                             ,
                ULL(0x10B010418), SH_UNT_PU_CME4  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME4_CME_SCOM_FLAGS_PPE2                             ,
                ULL(0x10B010410), SH_UNT_PU_CME4  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME5_CME_SCOM_FLAGS_PPE                              ,
                ULL(0x10B020400), SH_UNT_PU_CME5  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME5_CME_SCOM_FLAGS_PPE1                             ,
                ULL(0x10B020418), SH_UNT_PU_CME5  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME5_CME_SCOM_FLAGS_PPE2                             ,
                ULL(0x10B020410), SH_UNT_PU_CME5  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME6_CME_SCOM_FLAGS_PPE                              ,
                ULL(0x10C010400), SH_UNT_PU_CME6  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME6_CME_SCOM_FLAGS_PPE1                             ,
                ULL(0x10C010418), SH_UNT_PU_CME6  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME6_CME_SCOM_FLAGS_PPE2                             ,
                ULL(0x10C010410), SH_UNT_PU_CME6  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME7_CME_SCOM_FLAGS_PPE                              ,
                ULL(0x10C020400), SH_UNT_PU_CME7  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME7_CME_SCOM_FLAGS_PPE1                             ,
                ULL(0x10C020418), SH_UNT_PU_CME7  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME7_CME_SCOM_FLAGS_PPE2                             ,
                ULL(0x10C020410), SH_UNT_PU_CME7  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME8_CME_SCOM_FLAGS_PPE                              ,
                ULL(0x10D010400), SH_UNT_PU_CME8  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME8_CME_SCOM_FLAGS_PPE1                             ,
                ULL(0x10D010418), SH_UNT_PU_CME8  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME8_CME_SCOM_FLAGS_PPE2                             ,
                ULL(0x10D010410), SH_UNT_PU_CME8  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME9_CME_SCOM_FLAGS_PPE                              ,
                ULL(0x10D020400), SH_UNT_PU_CME9  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME9_CME_SCOM_FLAGS_PPE1                             ,
                ULL(0x10D020418), SH_UNT_PU_CME9  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME9_CME_SCOM_FLAGS_PPE2                             ,
                ULL(0x10D020410), SH_UNT_PU_CME9  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME10_CME_SCOM_FLAGS_PPE                             ,
                ULL(0x10E010400), SH_UNT_PU_CME10 , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME10_CME_SCOM_FLAGS_PPE1                            ,
                ULL(0x10E010418), SH_UNT_PU_CME10 , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME10_CME_SCOM_FLAGS_PPE2                            ,
                ULL(0x10E010410), SH_UNT_PU_CME10 , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME11_CME_SCOM_FLAGS_PPE                             ,
                ULL(0x10E020400), SH_UNT_PU_CME11 , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME11_CME_SCOM_FLAGS_PPE1                            ,
                ULL(0x10E020418), SH_UNT_PU_CME11 , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME11_CME_SCOM_FLAGS_PPE2                            ,
                ULL(0x10E020410), SH_UNT_PU_CME11 , SH_ACS_PPE2      );

CONST_UINT64_T( PU_CME0_CME_SCOM_PMCRS0_PPE                             ,
                ULL(0x109010240), SH_UNT_PU_CME0  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME1_CME_SCOM_PMCRS0_PPE                             ,
                ULL(0x109020240), SH_UNT_PU_CME1  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME2_CME_SCOM_PMCRS0_PPE                             ,
                ULL(0x10A010240), SH_UNT_PU_CME2  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME3_CME_SCOM_PMCRS0_PPE                             ,
                ULL(0x10A020240), SH_UNT_PU_CME3  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME4_CME_SCOM_PMCRS0_PPE                             ,
                ULL(0x10B010240), SH_UNT_PU_CME4  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME5_CME_SCOM_PMCRS0_PPE                             ,
                ULL(0x10B020240), SH_UNT_PU_CME5  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME6_CME_SCOM_PMCRS0_PPE                             ,
                ULL(0x10C010240), SH_UNT_PU_CME6  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME7_CME_SCOM_PMCRS0_PPE                             ,
                ULL(0x10C020240), SH_UNT_PU_CME7  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME8_CME_SCOM_PMCRS0_PPE                             ,
                ULL(0x10D010240), SH_UNT_PU_CME8  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME9_CME_SCOM_PMCRS0_PPE                             ,
                ULL(0x10D020240), SH_UNT_PU_CME9  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME10_CME_SCOM_PMCRS0_PPE                            ,
                ULL(0x10E010240), SH_UNT_PU_CME10 , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME11_CME_SCOM_PMCRS0_PPE                            ,
                ULL(0x10E020240), SH_UNT_PU_CME11 , SH_ACS_PPE       );

CONST_UINT64_T( PU_CME0_CME_SCOM_PMCRS1_PPE                             ,
                ULL(0x109010260), SH_UNT_PU_CME0  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME1_CME_SCOM_PMCRS1_PPE                             ,
                ULL(0x109020260), SH_UNT_PU_CME1  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME2_CME_SCOM_PMCRS1_PPE                             ,
                ULL(0x10A010260), SH_UNT_PU_CME2  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME3_CME_SCOM_PMCRS1_PPE                             ,
                ULL(0x10A020260), SH_UNT_PU_CME3  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME4_CME_SCOM_PMCRS1_PPE                             ,
                ULL(0x10B010260), SH_UNT_PU_CME4  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME5_CME_SCOM_PMCRS1_PPE                             ,
                ULL(0x10B020260), SH_UNT_PU_CME5  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME6_CME_SCOM_PMCRS1_PPE                             ,
                ULL(0x10C010260), SH_UNT_PU_CME6  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME7_CME_SCOM_PMCRS1_PPE                             ,
                ULL(0x10C020260), SH_UNT_PU_CME7  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME8_CME_SCOM_PMCRS1_PPE                             ,
                ULL(0x10D010260), SH_UNT_PU_CME8  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME9_CME_SCOM_PMCRS1_PPE                             ,
                ULL(0x10D020260), SH_UNT_PU_CME9  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME10_CME_SCOM_PMCRS1_PPE                            ,
                ULL(0x10E010260), SH_UNT_PU_CME10 , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME11_CME_SCOM_PMCRS1_PPE                            ,
                ULL(0x10E020260), SH_UNT_PU_CME11 , SH_ACS_PPE       );

CONST_UINT64_T( PU_CME0_CME_SCOM_PMSRS0_PPE                             ,
                ULL(0x109010200), SH_UNT_PU_CME0  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME1_CME_SCOM_PMSRS0_PPE                             ,
                ULL(0x109020200), SH_UNT_PU_CME1  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME2_CME_SCOM_PMSRS0_PPE                             ,
                ULL(0x10A010200), SH_UNT_PU_CME2  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME3_CME_SCOM_PMSRS0_PPE                             ,
                ULL(0x10A020200), SH_UNT_PU_CME3  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME4_CME_SCOM_PMSRS0_PPE                             ,
                ULL(0x10B010200), SH_UNT_PU_CME4  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME5_CME_SCOM_PMSRS0_PPE                             ,
                ULL(0x10B020200), SH_UNT_PU_CME5  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME6_CME_SCOM_PMSRS0_PPE                             ,
                ULL(0x10C010200), SH_UNT_PU_CME6  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME7_CME_SCOM_PMSRS0_PPE                             ,
                ULL(0x10C020200), SH_UNT_PU_CME7  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME8_CME_SCOM_PMSRS0_PPE                             ,
                ULL(0x10D010200), SH_UNT_PU_CME8  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME9_CME_SCOM_PMSRS0_PPE                             ,
                ULL(0x10D020200), SH_UNT_PU_CME9  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME10_CME_SCOM_PMSRS0_PPE                            ,
                ULL(0x10E010200), SH_UNT_PU_CME10 , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME11_CME_SCOM_PMSRS0_PPE                            ,
                ULL(0x10E020200), SH_UNT_PU_CME11 , SH_ACS_PPE       );

CONST_UINT64_T( PU_CME0_CME_SCOM_PMSRS1_PPE                             ,
                ULL(0x109010220), SH_UNT_PU_CME0  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME1_CME_SCOM_PMSRS1_PPE                             ,
                ULL(0x109020220), SH_UNT_PU_CME1  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME2_CME_SCOM_PMSRS1_PPE                             ,
                ULL(0x10A010220), SH_UNT_PU_CME2  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME3_CME_SCOM_PMSRS1_PPE                             ,
                ULL(0x10A020220), SH_UNT_PU_CME3  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME4_CME_SCOM_PMSRS1_PPE                             ,
                ULL(0x10B010220), SH_UNT_PU_CME4  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME5_CME_SCOM_PMSRS1_PPE                             ,
                ULL(0x10B020220), SH_UNT_PU_CME5  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME6_CME_SCOM_PMSRS1_PPE                             ,
                ULL(0x10C010220), SH_UNT_PU_CME6  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME7_CME_SCOM_PMSRS1_PPE                             ,
                ULL(0x10C020220), SH_UNT_PU_CME7  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME8_CME_SCOM_PMSRS1_PPE                             ,
                ULL(0x10D010220), SH_UNT_PU_CME8  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME9_CME_SCOM_PMSRS1_PPE                             ,
                ULL(0x10D020220), SH_UNT_PU_CME9  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME10_CME_SCOM_PMSRS1_PPE                            ,
                ULL(0x10E010220), SH_UNT_PU_CME10 , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME11_CME_SCOM_PMSRS1_PPE                            ,
                ULL(0x10E020220), SH_UNT_PU_CME11 , SH_ACS_PPE       );

CONST_UINT64_T( PU_CME0_CME_SCOM_PSCRS00_PPE                            ,
                ULL(0x109010300), SH_UNT_PU_CME0  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME1_CME_SCOM_PSCRS00_PPE                            ,
                ULL(0x109020300), SH_UNT_PU_CME1  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME2_CME_SCOM_PSCRS00_PPE                            ,
                ULL(0x10A010300), SH_UNT_PU_CME2  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME3_CME_SCOM_PSCRS00_PPE                            ,
                ULL(0x10A020300), SH_UNT_PU_CME3  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME4_CME_SCOM_PSCRS00_PPE                            ,
                ULL(0x10B010300), SH_UNT_PU_CME4  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME5_CME_SCOM_PSCRS00_PPE                            ,
                ULL(0x10B020300), SH_UNT_PU_CME5  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME6_CME_SCOM_PSCRS00_PPE                            ,
                ULL(0x10C010300), SH_UNT_PU_CME6  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME7_CME_SCOM_PSCRS00_PPE                            ,
                ULL(0x10C020300), SH_UNT_PU_CME7  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME8_CME_SCOM_PSCRS00_PPE                            ,
                ULL(0x10D010300), SH_UNT_PU_CME8  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME9_CME_SCOM_PSCRS00_PPE                            ,
                ULL(0x10D020300), SH_UNT_PU_CME9  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME10_CME_SCOM_PSCRS00_PPE                           ,
                ULL(0x10E010300), SH_UNT_PU_CME10 , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME11_CME_SCOM_PSCRS00_PPE                           ,
                ULL(0x10E020300), SH_UNT_PU_CME11 , SH_ACS_PPE       );

CONST_UINT64_T( PU_CME0_CME_SCOM_PSCRS01_PPE                            ,
                ULL(0x109010320), SH_UNT_PU_CME0  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME1_CME_SCOM_PSCRS01_PPE                            ,
                ULL(0x109020320), SH_UNT_PU_CME1  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME2_CME_SCOM_PSCRS01_PPE                            ,
                ULL(0x10A010320), SH_UNT_PU_CME2  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME3_CME_SCOM_PSCRS01_PPE                            ,
                ULL(0x10A020320), SH_UNT_PU_CME3  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME4_CME_SCOM_PSCRS01_PPE                            ,
                ULL(0x10B010320), SH_UNT_PU_CME4  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME5_CME_SCOM_PSCRS01_PPE                            ,
                ULL(0x10B020320), SH_UNT_PU_CME5  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME6_CME_SCOM_PSCRS01_PPE                            ,
                ULL(0x10C010320), SH_UNT_PU_CME6  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME7_CME_SCOM_PSCRS01_PPE                            ,
                ULL(0x10C020320), SH_UNT_PU_CME7  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME8_CME_SCOM_PSCRS01_PPE                            ,
                ULL(0x10D010320), SH_UNT_PU_CME8  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME9_CME_SCOM_PSCRS01_PPE                            ,
                ULL(0x10D020320), SH_UNT_PU_CME9  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME10_CME_SCOM_PSCRS01_PPE                           ,
                ULL(0x10E010320), SH_UNT_PU_CME10 , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME11_CME_SCOM_PSCRS01_PPE                           ,
                ULL(0x10E020320), SH_UNT_PU_CME11 , SH_ACS_PPE       );

CONST_UINT64_T( PU_CME0_CME_SCOM_PSCRS02_PPE                            ,
                ULL(0x109010340), SH_UNT_PU_CME0  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME1_CME_SCOM_PSCRS02_PPE                            ,
                ULL(0x109020340), SH_UNT_PU_CME1  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME2_CME_SCOM_PSCRS02_PPE                            ,
                ULL(0x10A010340), SH_UNT_PU_CME2  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME3_CME_SCOM_PSCRS02_PPE                            ,
                ULL(0x10A020340), SH_UNT_PU_CME3  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME4_CME_SCOM_PSCRS02_PPE                            ,
                ULL(0x10B010340), SH_UNT_PU_CME4  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME5_CME_SCOM_PSCRS02_PPE                            ,
                ULL(0x10B020340), SH_UNT_PU_CME5  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME6_CME_SCOM_PSCRS02_PPE                            ,
                ULL(0x10C010340), SH_UNT_PU_CME6  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME7_CME_SCOM_PSCRS02_PPE                            ,
                ULL(0x10C020340), SH_UNT_PU_CME7  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME8_CME_SCOM_PSCRS02_PPE                            ,
                ULL(0x10D010340), SH_UNT_PU_CME8  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME9_CME_SCOM_PSCRS02_PPE                            ,
                ULL(0x10D020340), SH_UNT_PU_CME9  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME10_CME_SCOM_PSCRS02_PPE                           ,
                ULL(0x10E010340), SH_UNT_PU_CME10 , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME11_CME_SCOM_PSCRS02_PPE                           ,
                ULL(0x10E020340), SH_UNT_PU_CME11 , SH_ACS_PPE       );

CONST_UINT64_T( PU_CME0_CME_SCOM_PSCRS03_PPE                            ,
                ULL(0x109010360), SH_UNT_PU_CME0  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME1_CME_SCOM_PSCRS03_PPE                            ,
                ULL(0x109020360), SH_UNT_PU_CME1  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME2_CME_SCOM_PSCRS03_PPE                            ,
                ULL(0x10A010360), SH_UNT_PU_CME2  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME3_CME_SCOM_PSCRS03_PPE                            ,
                ULL(0x10A020360), SH_UNT_PU_CME3  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME4_CME_SCOM_PSCRS03_PPE                            ,
                ULL(0x10B010360), SH_UNT_PU_CME4  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME5_CME_SCOM_PSCRS03_PPE                            ,
                ULL(0x10B020360), SH_UNT_PU_CME5  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME6_CME_SCOM_PSCRS03_PPE                            ,
                ULL(0x10C010360), SH_UNT_PU_CME6  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME7_CME_SCOM_PSCRS03_PPE                            ,
                ULL(0x10C020360), SH_UNT_PU_CME7  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME8_CME_SCOM_PSCRS03_PPE                            ,
                ULL(0x10D010360), SH_UNT_PU_CME8  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME9_CME_SCOM_PSCRS03_PPE                            ,
                ULL(0x10D020360), SH_UNT_PU_CME9  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME10_CME_SCOM_PSCRS03_PPE                           ,
                ULL(0x10E010360), SH_UNT_PU_CME10 , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME11_CME_SCOM_PSCRS03_PPE                           ,
                ULL(0x10E020360), SH_UNT_PU_CME11 , SH_ACS_PPE       );

CONST_UINT64_T( PU_CME0_CME_SCOM_PSCRS10_PPE                            ,
                ULL(0x109010380), SH_UNT_PU_CME0  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME1_CME_SCOM_PSCRS10_PPE                            ,
                ULL(0x109020380), SH_UNT_PU_CME1  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME2_CME_SCOM_PSCRS10_PPE                            ,
                ULL(0x10A010380), SH_UNT_PU_CME2  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME3_CME_SCOM_PSCRS10_PPE                            ,
                ULL(0x10A020380), SH_UNT_PU_CME3  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME4_CME_SCOM_PSCRS10_PPE                            ,
                ULL(0x10B010380), SH_UNT_PU_CME4  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME5_CME_SCOM_PSCRS10_PPE                            ,
                ULL(0x10B020380), SH_UNT_PU_CME5  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME6_CME_SCOM_PSCRS10_PPE                            ,
                ULL(0x10C010380), SH_UNT_PU_CME6  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME7_CME_SCOM_PSCRS10_PPE                            ,
                ULL(0x10C020380), SH_UNT_PU_CME7  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME8_CME_SCOM_PSCRS10_PPE                            ,
                ULL(0x10D010380), SH_UNT_PU_CME8  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME9_CME_SCOM_PSCRS10_PPE                            ,
                ULL(0x10D020380), SH_UNT_PU_CME9  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME10_CME_SCOM_PSCRS10_PPE                           ,
                ULL(0x10E010380), SH_UNT_PU_CME10 , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME11_CME_SCOM_PSCRS10_PPE                           ,
                ULL(0x10E020380), SH_UNT_PU_CME11 , SH_ACS_PPE       );

CONST_UINT64_T( PU_CME0_CME_SCOM_PSCRS11_PPE                            ,
                ULL(0x1090103A0), SH_UNT_PU_CME0  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME1_CME_SCOM_PSCRS11_PPE                            ,
                ULL(0x1090203A0), SH_UNT_PU_CME1  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME2_CME_SCOM_PSCRS11_PPE                            ,
                ULL(0x10A0103A0), SH_UNT_PU_CME2  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME3_CME_SCOM_PSCRS11_PPE                            ,
                ULL(0x10A0203A0), SH_UNT_PU_CME3  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME4_CME_SCOM_PSCRS11_PPE                            ,
                ULL(0x10B0103A0), SH_UNT_PU_CME4  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME5_CME_SCOM_PSCRS11_PPE                            ,
                ULL(0x10B0203A0), SH_UNT_PU_CME5  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME6_CME_SCOM_PSCRS11_PPE                            ,
                ULL(0x10C0103A0), SH_UNT_PU_CME6  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME7_CME_SCOM_PSCRS11_PPE                            ,
                ULL(0x10C0203A0), SH_UNT_PU_CME7  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME8_CME_SCOM_PSCRS11_PPE                            ,
                ULL(0x10D0103A0), SH_UNT_PU_CME8  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME9_CME_SCOM_PSCRS11_PPE                            ,
                ULL(0x10D0203A0), SH_UNT_PU_CME9  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME10_CME_SCOM_PSCRS11_PPE                           ,
                ULL(0x10E0103A0), SH_UNT_PU_CME10 , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME11_CME_SCOM_PSCRS11_PPE                           ,
                ULL(0x10E0203A0), SH_UNT_PU_CME11 , SH_ACS_PPE       );

CONST_UINT64_T( PU_CME0_CME_SCOM_PSCRS12_PPE                            ,
                ULL(0x1090103C0), SH_UNT_PU_CME0  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME1_CME_SCOM_PSCRS12_PPE                            ,
                ULL(0x1090203C0), SH_UNT_PU_CME1  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME2_CME_SCOM_PSCRS12_PPE                            ,
                ULL(0x10A0103C0), SH_UNT_PU_CME2  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME3_CME_SCOM_PSCRS12_PPE                            ,
                ULL(0x10A0203C0), SH_UNT_PU_CME3  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME4_CME_SCOM_PSCRS12_PPE                            ,
                ULL(0x10B0103C0), SH_UNT_PU_CME4  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME5_CME_SCOM_PSCRS12_PPE                            ,
                ULL(0x10B0203C0), SH_UNT_PU_CME5  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME6_CME_SCOM_PSCRS12_PPE                            ,
                ULL(0x10C0103C0), SH_UNT_PU_CME6  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME7_CME_SCOM_PSCRS12_PPE                            ,
                ULL(0x10C0203C0), SH_UNT_PU_CME7  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME8_CME_SCOM_PSCRS12_PPE                            ,
                ULL(0x10D0103C0), SH_UNT_PU_CME8  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME9_CME_SCOM_PSCRS12_PPE                            ,
                ULL(0x10D0203C0), SH_UNT_PU_CME9  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME10_CME_SCOM_PSCRS12_PPE                           ,
                ULL(0x10E0103C0), SH_UNT_PU_CME10 , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME11_CME_SCOM_PSCRS12_PPE                           ,
                ULL(0x10E0203C0), SH_UNT_PU_CME11 , SH_ACS_PPE       );

CONST_UINT64_T( PU_CME0_CME_SCOM_PSCRS13_PPE                            ,
                ULL(0x1090103E0), SH_UNT_PU_CME0  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME1_CME_SCOM_PSCRS13_PPE                            ,
                ULL(0x1090203E0), SH_UNT_PU_CME1  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME2_CME_SCOM_PSCRS13_PPE                            ,
                ULL(0x10A0103E0), SH_UNT_PU_CME2  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME3_CME_SCOM_PSCRS13_PPE                            ,
                ULL(0x10A0203E0), SH_UNT_PU_CME3  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME4_CME_SCOM_PSCRS13_PPE                            ,
                ULL(0x10B0103E0), SH_UNT_PU_CME4  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME5_CME_SCOM_PSCRS13_PPE                            ,
                ULL(0x10B0203E0), SH_UNT_PU_CME5  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME6_CME_SCOM_PSCRS13_PPE                            ,
                ULL(0x10C0103E0), SH_UNT_PU_CME6  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME7_CME_SCOM_PSCRS13_PPE                            ,
                ULL(0x10C0203E0), SH_UNT_PU_CME7  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME8_CME_SCOM_PSCRS13_PPE                            ,
                ULL(0x10D0103E0), SH_UNT_PU_CME8  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME9_CME_SCOM_PSCRS13_PPE                            ,
                ULL(0x10D0203E0), SH_UNT_PU_CME9  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME10_CME_SCOM_PSCRS13_PPE                           ,
                ULL(0x10E0103E0), SH_UNT_PU_CME10 , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME11_CME_SCOM_PSCRS13_PPE                           ,
                ULL(0x10E0203E0), SH_UNT_PU_CME11 , SH_ACS_PPE       );

CONST_UINT64_T( PU_CME0_CME_SCOM_QFMR_PPE                               ,
                ULL(0x109010140), SH_UNT_PU_CME0  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME1_CME_SCOM_QFMR_PPE                               ,
                ULL(0x109020140), SH_UNT_PU_CME1  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME2_CME_SCOM_QFMR_PPE                               ,
                ULL(0x10A010140), SH_UNT_PU_CME2  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME3_CME_SCOM_QFMR_PPE                               ,
                ULL(0x10A020140), SH_UNT_PU_CME3  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME4_CME_SCOM_QFMR_PPE                               ,
                ULL(0x10B010140), SH_UNT_PU_CME4  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME5_CME_SCOM_QFMR_PPE                               ,
                ULL(0x10B020140), SH_UNT_PU_CME5  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME6_CME_SCOM_QFMR_PPE                               ,
                ULL(0x10C010140), SH_UNT_PU_CME6  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME7_CME_SCOM_QFMR_PPE                               ,
                ULL(0x10C020140), SH_UNT_PU_CME7  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME8_CME_SCOM_QFMR_PPE                               ,
                ULL(0x10D010140), SH_UNT_PU_CME8  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME9_CME_SCOM_QFMR_PPE                               ,
                ULL(0x10D020140), SH_UNT_PU_CME9  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME10_CME_SCOM_QFMR_PPE                              ,
                ULL(0x10E010140), SH_UNT_PU_CME10 , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME11_CME_SCOM_QFMR_PPE                              ,
                ULL(0x10E020140), SH_UNT_PU_CME11 , SH_ACS_PPE       );

CONST_UINT64_T( PU_CME0_CME_SCOM_SICR_PPE                               ,
                ULL(0x109010500), SH_UNT_PU_CME0  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME0_CME_SCOM_SICR_PPE1                              ,
                ULL(0x109010518), SH_UNT_PU_CME0  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME0_CME_SCOM_SICR_PPE2                              ,
                ULL(0x109010510), SH_UNT_PU_CME0  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME1_CME_SCOM_SICR_PPE                               ,
                ULL(0x109020500), SH_UNT_PU_CME1  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME1_CME_SCOM_SICR_PPE1                              ,
                ULL(0x109020518), SH_UNT_PU_CME1  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME1_CME_SCOM_SICR_PPE2                              ,
                ULL(0x109020510), SH_UNT_PU_CME1  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME2_CME_SCOM_SICR_PPE                               ,
                ULL(0x10A010500), SH_UNT_PU_CME2  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME2_CME_SCOM_SICR_PPE1                              ,
                ULL(0x10A010518), SH_UNT_PU_CME2  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME2_CME_SCOM_SICR_PPE2                              ,
                ULL(0x10A010510), SH_UNT_PU_CME2  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME3_CME_SCOM_SICR_PPE                               ,
                ULL(0x10A020500), SH_UNT_PU_CME3  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME3_CME_SCOM_SICR_PPE1                              ,
                ULL(0x10A020518), SH_UNT_PU_CME3  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME3_CME_SCOM_SICR_PPE2                              ,
                ULL(0x10A020510), SH_UNT_PU_CME3  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME4_CME_SCOM_SICR_PPE                               ,
                ULL(0x10B010500), SH_UNT_PU_CME4  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME4_CME_SCOM_SICR_PPE1                              ,
                ULL(0x10B010518), SH_UNT_PU_CME4  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME4_CME_SCOM_SICR_PPE2                              ,
                ULL(0x10B010510), SH_UNT_PU_CME4  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME5_CME_SCOM_SICR_PPE                               ,
                ULL(0x10B020500), SH_UNT_PU_CME5  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME5_CME_SCOM_SICR_PPE1                              ,
                ULL(0x10B020518), SH_UNT_PU_CME5  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME5_CME_SCOM_SICR_PPE2                              ,
                ULL(0x10B020510), SH_UNT_PU_CME5  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME6_CME_SCOM_SICR_PPE                               ,
                ULL(0x10C010500), SH_UNT_PU_CME6  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME6_CME_SCOM_SICR_PPE1                              ,
                ULL(0x10C010518), SH_UNT_PU_CME6  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME6_CME_SCOM_SICR_PPE2                              ,
                ULL(0x10C010510), SH_UNT_PU_CME6  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME7_CME_SCOM_SICR_PPE                               ,
                ULL(0x10C020500), SH_UNT_PU_CME7  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME7_CME_SCOM_SICR_PPE1                              ,
                ULL(0x10C020518), SH_UNT_PU_CME7  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME7_CME_SCOM_SICR_PPE2                              ,
                ULL(0x10C020510), SH_UNT_PU_CME7  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME8_CME_SCOM_SICR_PPE                               ,
                ULL(0x10D010500), SH_UNT_PU_CME8  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME8_CME_SCOM_SICR_PPE1                              ,
                ULL(0x10D010518), SH_UNT_PU_CME8  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME8_CME_SCOM_SICR_PPE2                              ,
                ULL(0x10D010510), SH_UNT_PU_CME8  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME9_CME_SCOM_SICR_PPE                               ,
                ULL(0x10D020500), SH_UNT_PU_CME9  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME9_CME_SCOM_SICR_PPE1                              ,
                ULL(0x10D020518), SH_UNT_PU_CME9  , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME9_CME_SCOM_SICR_PPE2                              ,
                ULL(0x10D020510), SH_UNT_PU_CME9  , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME10_CME_SCOM_SICR_PPE                              ,
                ULL(0x10E010500), SH_UNT_PU_CME10 , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME10_CME_SCOM_SICR_PPE1                             ,
                ULL(0x10E010518), SH_UNT_PU_CME10 , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME10_CME_SCOM_SICR_PPE2                             ,
                ULL(0x10E010510), SH_UNT_PU_CME10 , SH_ACS_PPE2      );
CONST_UINT64_T( PU_CME11_CME_SCOM_SICR_PPE                              ,
                ULL(0x10E020500), SH_UNT_PU_CME11 , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME11_CME_SCOM_SICR_PPE1                             ,
                ULL(0x10E020518), SH_UNT_PU_CME11 , SH_ACS_PPE1      );
CONST_UINT64_T( PU_CME11_CME_SCOM_SICR_PPE2                             ,
                ULL(0x10E020510), SH_UNT_PU_CME11 , SH_ACS_PPE2      );

CONST_UINT64_T( PU_CME0_CME_SCOM_SRTCH0_PPE                             ,
                ULL(0x109010420), SH_UNT_PU_CME0  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME1_CME_SCOM_SRTCH0_PPE                             ,
                ULL(0x109020420), SH_UNT_PU_CME1  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME2_CME_SCOM_SRTCH0_PPE                             ,
                ULL(0x10A010420), SH_UNT_PU_CME2  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME3_CME_SCOM_SRTCH0_PPE                             ,
                ULL(0x10A020420), SH_UNT_PU_CME3  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME4_CME_SCOM_SRTCH0_PPE                             ,
                ULL(0x10B010420), SH_UNT_PU_CME4  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME5_CME_SCOM_SRTCH0_PPE                             ,
                ULL(0x10B020420), SH_UNT_PU_CME5  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME6_CME_SCOM_SRTCH0_PPE                             ,
                ULL(0x10C010420), SH_UNT_PU_CME6  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME7_CME_SCOM_SRTCH0_PPE                             ,
                ULL(0x10C020420), SH_UNT_PU_CME7  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME8_CME_SCOM_SRTCH0_PPE                             ,
                ULL(0x10D010420), SH_UNT_PU_CME8  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME9_CME_SCOM_SRTCH0_PPE                             ,
                ULL(0x10D020420), SH_UNT_PU_CME9  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME10_CME_SCOM_SRTCH0_PPE                            ,
                ULL(0x10E010420), SH_UNT_PU_CME10 , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME11_CME_SCOM_SRTCH0_PPE                            ,
                ULL(0x10E020420), SH_UNT_PU_CME11 , SH_ACS_PPE       );

CONST_UINT64_T( PU_CME0_CME_SCOM_SRTCH1_PPE                             ,
                ULL(0x109010440), SH_UNT_PU_CME0  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME1_CME_SCOM_SRTCH1_PPE                             ,
                ULL(0x109020440), SH_UNT_PU_CME1  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME2_CME_SCOM_SRTCH1_PPE                             ,
                ULL(0x10A010440), SH_UNT_PU_CME2  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME3_CME_SCOM_SRTCH1_PPE                             ,
                ULL(0x10A020440), SH_UNT_PU_CME3  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME4_CME_SCOM_SRTCH1_PPE                             ,
                ULL(0x10B010440), SH_UNT_PU_CME4  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME5_CME_SCOM_SRTCH1_PPE                             ,
                ULL(0x10B020440), SH_UNT_PU_CME5  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME6_CME_SCOM_SRTCH1_PPE                             ,
                ULL(0x10C010440), SH_UNT_PU_CME6  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME7_CME_SCOM_SRTCH1_PPE                             ,
                ULL(0x10C020440), SH_UNT_PU_CME7  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME8_CME_SCOM_SRTCH1_PPE                             ,
                ULL(0x10D010440), SH_UNT_PU_CME8  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME9_CME_SCOM_SRTCH1_PPE                             ,
                ULL(0x10D020440), SH_UNT_PU_CME9  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME10_CME_SCOM_SRTCH1_PPE                            ,
                ULL(0x10E010440), SH_UNT_PU_CME10 , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME11_CME_SCOM_SRTCH1_PPE                            ,
                ULL(0x10E020440), SH_UNT_PU_CME11 , SH_ACS_PPE       );

CONST_UINT64_T( PU_CME0_CME_SCOM_VDSR_PPE                               ,
                ULL(0x109010640), SH_UNT_PU_CME0  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME1_CME_SCOM_VDSR_PPE                               ,
                ULL(0x109020640), SH_UNT_PU_CME1  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME2_CME_SCOM_VDSR_PPE                               ,
                ULL(0x10A010640), SH_UNT_PU_CME2  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME3_CME_SCOM_VDSR_PPE                               ,
                ULL(0x10A020640), SH_UNT_PU_CME3  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME4_CME_SCOM_VDSR_PPE                               ,
                ULL(0x10B010640), SH_UNT_PU_CME4  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME5_CME_SCOM_VDSR_PPE                               ,
                ULL(0x10B020640), SH_UNT_PU_CME5  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME6_CME_SCOM_VDSR_PPE                               ,
                ULL(0x10C010640), SH_UNT_PU_CME6  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME7_CME_SCOM_VDSR_PPE                               ,
                ULL(0x10C020640), SH_UNT_PU_CME7  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME8_CME_SCOM_VDSR_PPE                               ,
                ULL(0x10D010640), SH_UNT_PU_CME8  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME9_CME_SCOM_VDSR_PPE                               ,
                ULL(0x10D020640), SH_UNT_PU_CME9  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME10_CME_SCOM_VDSR_PPE                              ,
                ULL(0x10E010640), SH_UNT_PU_CME10 , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME11_CME_SCOM_VDSR_PPE                              ,
                ULL(0x10E020640), SH_UNT_PU_CME11 , SH_ACS_PPE       );

CONST_UINT64_T( PU_CME0_CME_SCOM_VTSR0_PPE                              ,
                ULL(0x109010600), SH_UNT_PU_CME0  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME1_CME_SCOM_VTSR0_PPE                              ,
                ULL(0x109020600), SH_UNT_PU_CME1  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME2_CME_SCOM_VTSR0_PPE                              ,
                ULL(0x10A010600), SH_UNT_PU_CME2  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME3_CME_SCOM_VTSR0_PPE                              ,
                ULL(0x10A020600), SH_UNT_PU_CME3  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME4_CME_SCOM_VTSR0_PPE                              ,
                ULL(0x10B010600), SH_UNT_PU_CME4  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME5_CME_SCOM_VTSR0_PPE                              ,
                ULL(0x10B020600), SH_UNT_PU_CME5  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME6_CME_SCOM_VTSR0_PPE                              ,
                ULL(0x10C010600), SH_UNT_PU_CME6  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME7_CME_SCOM_VTSR0_PPE                              ,
                ULL(0x10C020600), SH_UNT_PU_CME7  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME8_CME_SCOM_VTSR0_PPE                              ,
                ULL(0x10D010600), SH_UNT_PU_CME8  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME9_CME_SCOM_VTSR0_PPE                              ,
                ULL(0x10D020600), SH_UNT_PU_CME9  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME10_CME_SCOM_VTSR0_PPE                             ,
                ULL(0x10E010600), SH_UNT_PU_CME10 , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME11_CME_SCOM_VTSR0_PPE                             ,
                ULL(0x10E020600), SH_UNT_PU_CME11 , SH_ACS_PPE       );

CONST_UINT64_T( PU_CME0_CME_SCOM_VTSR1_PPE                              ,
                ULL(0x109010620), SH_UNT_PU_CME0  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME1_CME_SCOM_VTSR1_PPE                              ,
                ULL(0x109020620), SH_UNT_PU_CME1  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME2_CME_SCOM_VTSR1_PPE                              ,
                ULL(0x10A010620), SH_UNT_PU_CME2  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME3_CME_SCOM_VTSR1_PPE                              ,
                ULL(0x10A020620), SH_UNT_PU_CME3  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME4_CME_SCOM_VTSR1_PPE                              ,
                ULL(0x10B010620), SH_UNT_PU_CME4  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME5_CME_SCOM_VTSR1_PPE                              ,
                ULL(0x10B020620), SH_UNT_PU_CME5  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME6_CME_SCOM_VTSR1_PPE                              ,
                ULL(0x10C010620), SH_UNT_PU_CME6  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME7_CME_SCOM_VTSR1_PPE                              ,
                ULL(0x10C020620), SH_UNT_PU_CME7  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME8_CME_SCOM_VTSR1_PPE                              ,
                ULL(0x10D010620), SH_UNT_PU_CME8  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME9_CME_SCOM_VTSR1_PPE                              ,
                ULL(0x10D020620), SH_UNT_PU_CME9  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME10_CME_SCOM_VTSR1_PPE                             ,
                ULL(0x10E010620), SH_UNT_PU_CME10 , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME11_CME_SCOM_VTSR1_PPE                             ,
                ULL(0x10E020620), SH_UNT_PU_CME11 , SH_ACS_PPE       );

CONST_UINT64_T( PU_CME0_CME_SCOM_XIPCBMD0_PPE                           ,
                ULL(0x109010580), SH_UNT_PU_CME0  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME1_CME_SCOM_XIPCBMD0_PPE                           ,
                ULL(0x109020580), SH_UNT_PU_CME1  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME2_CME_SCOM_XIPCBMD0_PPE                           ,
                ULL(0x10A010580), SH_UNT_PU_CME2  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME3_CME_SCOM_XIPCBMD0_PPE                           ,
                ULL(0x10A020580), SH_UNT_PU_CME3  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME4_CME_SCOM_XIPCBMD0_PPE                           ,
                ULL(0x10B010580), SH_UNT_PU_CME4  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME5_CME_SCOM_XIPCBMD0_PPE                           ,
                ULL(0x10B020580), SH_UNT_PU_CME5  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME6_CME_SCOM_XIPCBMD0_PPE                           ,
                ULL(0x10C010580), SH_UNT_PU_CME6  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME7_CME_SCOM_XIPCBMD0_PPE                           ,
                ULL(0x10C020580), SH_UNT_PU_CME7  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME8_CME_SCOM_XIPCBMD0_PPE                           ,
                ULL(0x10D010580), SH_UNT_PU_CME8  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME9_CME_SCOM_XIPCBMD0_PPE                           ,
                ULL(0x10D020580), SH_UNT_PU_CME9  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME10_CME_SCOM_XIPCBMD0_PPE                          ,
                ULL(0x10E010580), SH_UNT_PU_CME10 , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME11_CME_SCOM_XIPCBMD0_PPE                          ,
                ULL(0x10E020580), SH_UNT_PU_CME11 , SH_ACS_PPE       );

CONST_UINT64_T( PU_CME0_CME_SCOM_XIPCBMD1_PPE                           ,
                ULL(0x1090105A0), SH_UNT_PU_CME0  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME1_CME_SCOM_XIPCBMD1_PPE                           ,
                ULL(0x1090205A0), SH_UNT_PU_CME1  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME2_CME_SCOM_XIPCBMD1_PPE                           ,
                ULL(0x10A0105A0), SH_UNT_PU_CME2  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME3_CME_SCOM_XIPCBMD1_PPE                           ,
                ULL(0x10A0205A0), SH_UNT_PU_CME3  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME4_CME_SCOM_XIPCBMD1_PPE                           ,
                ULL(0x10B0105A0), SH_UNT_PU_CME4  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME5_CME_SCOM_XIPCBMD1_PPE                           ,
                ULL(0x10B0205A0), SH_UNT_PU_CME5  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME6_CME_SCOM_XIPCBMD1_PPE                           ,
                ULL(0x10C0105A0), SH_UNT_PU_CME6  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME7_CME_SCOM_XIPCBMD1_PPE                           ,
                ULL(0x10C0205A0), SH_UNT_PU_CME7  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME8_CME_SCOM_XIPCBMD1_PPE                           ,
                ULL(0x10D0105A0), SH_UNT_PU_CME8  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME9_CME_SCOM_XIPCBMD1_PPE                           ,
                ULL(0x10D0205A0), SH_UNT_PU_CME9  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME10_CME_SCOM_XIPCBMD1_PPE                          ,
                ULL(0x10E0105A0), SH_UNT_PU_CME10 , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME11_CME_SCOM_XIPCBMD1_PPE                          ,
                ULL(0x10E0205A0), SH_UNT_PU_CME11 , SH_ACS_PPE       );

CONST_UINT64_T( PU_CME0_CME_SCOM_XIPCBMI0_PPE                           ,
                ULL(0x1090105C0), SH_UNT_PU_CME0  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME1_CME_SCOM_XIPCBMI0_PPE                           ,
                ULL(0x1090205C0), SH_UNT_PU_CME1  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME2_CME_SCOM_XIPCBMI0_PPE                           ,
                ULL(0x10A0105C0), SH_UNT_PU_CME2  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME3_CME_SCOM_XIPCBMI0_PPE                           ,
                ULL(0x10A0205C0), SH_UNT_PU_CME3  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME4_CME_SCOM_XIPCBMI0_PPE                           ,
                ULL(0x10B0105C0), SH_UNT_PU_CME4  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME5_CME_SCOM_XIPCBMI0_PPE                           ,
                ULL(0x10B0205C0), SH_UNT_PU_CME5  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME6_CME_SCOM_XIPCBMI0_PPE                           ,
                ULL(0x10C0105C0), SH_UNT_PU_CME6  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME7_CME_SCOM_XIPCBMI0_PPE                           ,
                ULL(0x10C0205C0), SH_UNT_PU_CME7  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME8_CME_SCOM_XIPCBMI0_PPE                           ,
                ULL(0x10D0105C0), SH_UNT_PU_CME8  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME9_CME_SCOM_XIPCBMI0_PPE                           ,
                ULL(0x10D0205C0), SH_UNT_PU_CME9  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME10_CME_SCOM_XIPCBMI0_PPE                          ,
                ULL(0x10E0105C0), SH_UNT_PU_CME10 , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME11_CME_SCOM_XIPCBMI0_PPE                          ,
                ULL(0x10E0205C0), SH_UNT_PU_CME11 , SH_ACS_PPE       );

CONST_UINT64_T( PU_CME0_CME_SCOM_XIPCBMI1_PPE                           ,
                ULL(0x1090105E0), SH_UNT_PU_CME0  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME1_CME_SCOM_XIPCBMI1_PPE                           ,
                ULL(0x1090205E0), SH_UNT_PU_CME1  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME2_CME_SCOM_XIPCBMI1_PPE                           ,
                ULL(0x10A0105E0), SH_UNT_PU_CME2  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME3_CME_SCOM_XIPCBMI1_PPE                           ,
                ULL(0x10A0205E0), SH_UNT_PU_CME3  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME4_CME_SCOM_XIPCBMI1_PPE                           ,
                ULL(0x10B0105E0), SH_UNT_PU_CME4  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME5_CME_SCOM_XIPCBMI1_PPE                           ,
                ULL(0x10B0205E0), SH_UNT_PU_CME5  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME6_CME_SCOM_XIPCBMI1_PPE                           ,
                ULL(0x10C0105E0), SH_UNT_PU_CME6  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME7_CME_SCOM_XIPCBMI1_PPE                           ,
                ULL(0x10C0205E0), SH_UNT_PU_CME7  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME8_CME_SCOM_XIPCBMI1_PPE                           ,
                ULL(0x10D0105E0), SH_UNT_PU_CME8  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME9_CME_SCOM_XIPCBMI1_PPE                           ,
                ULL(0x10D0205E0), SH_UNT_PU_CME9  , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME10_CME_SCOM_XIPCBMI1_PPE                          ,
                ULL(0x10E0105E0), SH_UNT_PU_CME10 , SH_ACS_PPE       );
CONST_UINT64_T( PU_CME11_CME_SCOM_XIPCBMI1_PPE                          ,
                ULL(0x10E0205E0), SH_UNT_PU_CME11 , SH_ACS_PPE       );

CONST_UINT64_T( PU_COMMAND_REGISTER                                     ,
                ULL(0x00010000), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_COMMAND_REGISTER_B                                   ,
                ULL(0x000A0005), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_COMMAND_REGISTER_C                                   ,
                ULL(0x000A1005), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_COMMAND_REGISTER_D                                   ,
                ULL(0x000A2005), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_COMMAND_REGISTER_E                                   ,
                ULL(0x000A3005), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU0_CTL_CONFIG0                                     ,
                ULL(0x05011080), SH_UNT_PU_NPU0_CTL, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_CTL_CONFIG0                                     ,
                ULL(0x05011180), SH_UNT_PU_NPU1_CTL, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_CTL_CONFIG0                                     ,
                ULL(0x05011280), SH_UNT_PU_NPU2_CTL, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM0_CONFIG0                                     ,
                ULL(0x05011000), SH_UNT_PU_NPU0_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM1_CONFIG0                                     ,
                ULL(0x05011020), SH_UNT_PU_NPU0_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM2_CONFIG0                                     ,
                ULL(0x05011040), SH_UNT_PU_NPU0_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM3_CONFIG0                                     ,
                ULL(0x05011060), SH_UNT_PU_NPU0_SM3, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM0_CONFIG0                                     ,
                ULL(0x05011100), SH_UNT_PU_NPU1_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM1_CONFIG0                                     ,
                ULL(0x05011120), SH_UNT_PU_NPU1_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM2_CONFIG0                                     ,
                ULL(0x05011140), SH_UNT_PU_NPU1_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM3_CONFIG0                                     ,
                ULL(0x05011160), SH_UNT_PU_NPU1_SM3, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM0_CONFIG0                                     ,
                ULL(0x05011200), SH_UNT_PU_NPU2_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM1_CONFIG0                                     ,
                ULL(0x05011220), SH_UNT_PU_NPU2_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM2_CONFIG0                                     ,
                ULL(0x05011240), SH_UNT_PU_NPU2_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM3_CONFIG0                                     ,
                ULL(0x05011260), SH_UNT_PU_NPU2_SM3, SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU0_CTL_CONFIG1                                     ,
                ULL(0x05011081), SH_UNT_PU_NPU0_CTL, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_DAT_CONFIG1                                     ,
                ULL(0x050110B0), SH_UNT_PU_NPU0_DAT, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_CTL_CONFIG1                                     ,
                ULL(0x05011181), SH_UNT_PU_NPU1_CTL, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_DAT_CONFIG1                                     ,
                ULL(0x050111B0), SH_UNT_PU_NPU1_DAT, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_DAT_CONFIG1                                     ,
                ULL(0x050112B0), SH_UNT_PU_NPU2_DAT, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_CTL_CONFIG1                                     ,
                ULL(0x05011281), SH_UNT_PU_NPU2_CTL, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_NTL0_CONFIG1                                    ,
                ULL(0x050110D0), SH_UNT_PU_NPU0_NTL0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_NTL1_CONFIG1                                    ,
                ULL(0x050110F0), SH_UNT_PU_NPU0_NTL1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM0_CONFIG1                                     ,
                ULL(0x05011001), SH_UNT_PU_NPU0_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM1_CONFIG1                                     ,
                ULL(0x05011021), SH_UNT_PU_NPU0_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM2_CONFIG1                                     ,
                ULL(0x05011041), SH_UNT_PU_NPU0_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM3_CONFIG1                                     ,
                ULL(0x05011061), SH_UNT_PU_NPU0_SM3, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_NTL0_CONFIG1                                    ,
                ULL(0x050111D0), SH_UNT_PU_NPU1_NTL0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_NTL1_CONFIG1                                    ,
                ULL(0x050111F0), SH_UNT_PU_NPU1_NTL1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM0_CONFIG1                                     ,
                ULL(0x05011101), SH_UNT_PU_NPU1_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM1_CONFIG1                                     ,
                ULL(0x05011121), SH_UNT_PU_NPU1_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM2_CONFIG1                                     ,
                ULL(0x05011141), SH_UNT_PU_NPU1_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM3_CONFIG1                                     ,
                ULL(0x05011161), SH_UNT_PU_NPU1_SM3, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_NTL0_CONFIG1                                    ,
                ULL(0x050112D0), SH_UNT_PU_NPU2_NTL0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_NTL1_CONFIG1                                    ,
                ULL(0x050112F0), SH_UNT_PU_NPU2_NTL1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM0_CONFIG1                                     ,
                ULL(0x05011201), SH_UNT_PU_NPU2_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM1_CONFIG1                                     ,
                ULL(0x05011221), SH_UNT_PU_NPU2_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM2_CONFIG1                                     ,
                ULL(0x05011241), SH_UNT_PU_NPU2_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM3_CONFIG1                                     ,
                ULL(0x05011261), SH_UNT_PU_NPU2_SM3, SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU0_CTL_CONFIG2                                     ,
                ULL(0x05011082), SH_UNT_PU_NPU0_CTL, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_CTL_CONFIG2                                     ,
                ULL(0x05011182), SH_UNT_PU_NPU1_CTL, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_CTL_CONFIG2                                     ,
                ULL(0x05011282), SH_UNT_PU_NPU2_CTL, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_NTL0_CONFIG2                                    ,
                ULL(0x050110C0), SH_UNT_PU_NPU0_NTL0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_NTL1_CONFIG2                                    ,
                ULL(0x050110E0), SH_UNT_PU_NPU0_NTL1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM0_CONFIG2                                     ,
                ULL(0x05011002), SH_UNT_PU_NPU0_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM1_CONFIG2                                     ,
                ULL(0x05011022), SH_UNT_PU_NPU0_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM2_CONFIG2                                     ,
                ULL(0x05011042), SH_UNT_PU_NPU0_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM3_CONFIG2                                     ,
                ULL(0x05011062), SH_UNT_PU_NPU0_SM3, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_NTL0_CONFIG2                                    ,
                ULL(0x050111C0), SH_UNT_PU_NPU1_NTL0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_NTL1_CONFIG2                                    ,
                ULL(0x050111E0), SH_UNT_PU_NPU1_NTL1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM0_CONFIG2                                     ,
                ULL(0x05011102), SH_UNT_PU_NPU1_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM1_CONFIG2                                     ,
                ULL(0x05011122), SH_UNT_PU_NPU1_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM2_CONFIG2                                     ,
                ULL(0x05011142), SH_UNT_PU_NPU1_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM3_CONFIG2                                     ,
                ULL(0x05011162), SH_UNT_PU_NPU1_SM3, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_NTL0_CONFIG2                                    ,
                ULL(0x050112C0), SH_UNT_PU_NPU2_NTL0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_NTL1_CONFIG2                                    ,
                ULL(0x050112E0), SH_UNT_PU_NPU2_NTL1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM0_CONFIG2                                     ,
                ULL(0x05011202), SH_UNT_PU_NPU2_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM1_CONFIG2                                     ,
                ULL(0x05011222), SH_UNT_PU_NPU2_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM2_CONFIG2                                     ,
                ULL(0x05011242), SH_UNT_PU_NPU2_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM3_CONFIG2                                     ,
                ULL(0x05011262), SH_UNT_PU_NPU2_SM3, SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU0_CTL_CONFIG3                                     ,
                ULL(0x05011083), SH_UNT_PU_NPU0_CTL, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_CTL_CONFIG3                                     ,
                ULL(0x05011183), SH_UNT_PU_NPU1_CTL, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_CTL_CONFIG3                                     ,
                ULL(0x05011283), SH_UNT_PU_NPU2_CTL, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_NTL0_CONFIG3                                    ,
                ULL(0x050110C1), SH_UNT_PU_NPU0_NTL0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_NTL1_CONFIG3                                    ,
                ULL(0x050110E1), SH_UNT_PU_NPU0_NTL1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM0_CONFIG3                                     ,
                ULL(0x05011003), SH_UNT_PU_NPU0_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM1_CONFIG3                                     ,
                ULL(0x05011023), SH_UNT_PU_NPU0_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM2_CONFIG3                                     ,
                ULL(0x05011043), SH_UNT_PU_NPU0_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM3_CONFIG3                                     ,
                ULL(0x05011063), SH_UNT_PU_NPU0_SM3, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_NTL0_CONFIG3                                    ,
                ULL(0x050111C1), SH_UNT_PU_NPU1_NTL0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_NTL1_CONFIG3                                    ,
                ULL(0x050111E1), SH_UNT_PU_NPU1_NTL1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM0_CONFIG3                                     ,
                ULL(0x05011103), SH_UNT_PU_NPU1_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM1_CONFIG3                                     ,
                ULL(0x05011123), SH_UNT_PU_NPU1_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM2_CONFIG3                                     ,
                ULL(0x05011143), SH_UNT_PU_NPU1_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM3_CONFIG3                                     ,
                ULL(0x05011163), SH_UNT_PU_NPU1_SM3, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_NTL0_CONFIG3                                    ,
                ULL(0x050112C1), SH_UNT_PU_NPU2_NTL0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_NTL1_CONFIG3                                    ,
                ULL(0x050112E1), SH_UNT_PU_NPU2_NTL1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM0_CONFIG3                                     ,
                ULL(0x05011203), SH_UNT_PU_NPU2_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM1_CONFIG3                                     ,
                ULL(0x05011223), SH_UNT_PU_NPU2_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM2_CONFIG3                                     ,
                ULL(0x05011243), SH_UNT_PU_NPU2_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM3_CONFIG3                                     ,
                ULL(0x05011263), SH_UNT_PU_NPU2_SM3, SH_ACS_SCOM      );

CONST_UINT64_T( PU_CONTROL_REGISTER_B                                   ,
                ULL(0x000A0000), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_CONTROL_REGISTER_C                                   ,
                ULL(0x000A1000), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_CONTROL_REGISTER_D                                   ,
                ULL(0x000A2000), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_CONTROL_REGISTER_E                                   ,
                ULL(0x000A3000), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PEC_0_STACK0_CQSTAT_REG                                 ,
                ULL(0x04010C4C), SH_UNT_PEC_0_STACK0, SH_ACS_SCOM_RO   );
CONST_UINT64_T( PEC_0_STACK1_CQSTAT_REG                                 ,
                ULL(0x04010C8C), SH_UNT_PEC_0_STACK1, SH_ACS_SCOM_RO   );
CONST_UINT64_T( PEC_0_STACK2_CQSTAT_REG                                 ,
                ULL(0x04010CCC), SH_UNT_PEC_0_STACK2, SH_ACS_SCOM_RO   );
CONST_UINT64_T( PEC_1_STACK0_CQSTAT_REG                                 ,
                ULL(0x0401104C), SH_UNT_PEC_1_STACK0, SH_ACS_SCOM_RO   );
CONST_UINT64_T( PEC_1_STACK1_CQSTAT_REG                                 ,
                ULL(0x0401108C), SH_UNT_PEC_1_STACK1, SH_ACS_SCOM_RO   );
CONST_UINT64_T( PEC_1_STACK2_CQSTAT_REG                                 ,
                ULL(0x040110CC), SH_UNT_PEC_1_STACK2, SH_ACS_SCOM_RO   );
CONST_UINT64_T( PEC_2_STACK0_CQSTAT_REG                                 ,
                ULL(0x0401144C), SH_UNT_PEC_2_STACK0, SH_ACS_SCOM_RO   );
CONST_UINT64_T( PEC_2_STACK1_CQSTAT_REG                                 ,
                ULL(0x0401148C), SH_UNT_PEC_2_STACK1, SH_ACS_SCOM_RO   );
CONST_UINT64_T( PEC_2_STACK2_CQSTAT_REG                                 ,
                ULL(0x040114CC), SH_UNT_PEC_2_STACK2, SH_ACS_SCOM_RO   );
CONST_UINT64_T( PEC_STACK0_CQSTAT_REG                                   ,
                ULL(0x04010C4C), SH_UNT_PEC_STACK0, SH_ACS_SCOM_RO   );
CONST_UINT64_T( PEC_STACK1_CQSTAT_REG                                   ,
                ULL(0x04010C8C), SH_UNT_PEC_STACK1, SH_ACS_SCOM_RO   );
CONST_UINT64_T( PEC_STACK2_CQSTAT_REG                                   ,
                ULL(0x04010CCC), SH_UNT_PEC_STACK2, SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_PIB2OPB0_CRSIC                                       ,
                ULL(0x00020005), SH_UNT_PU_PIB2OPB0, SH_ACS_SCOM_WCLEAR );
CONST_UINT64_T( PU_PIB2OPB1_CRSIC                                       ,
                ULL(0x00020015), SH_UNT_PU_PIB2OPB1, SH_ACS_SCOM_WCLEAR );

CONST_UINT64_T( PU_PIB2OPB0_CRSIM                                       ,
                ULL(0x00020006), SH_UNT_PU_PIB2OPB0, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_PIB2OPB1_CRSIM                                       ,
                ULL(0x00020016), SH_UNT_PU_PIB2OPB1, SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_PIB2OPB0_CRSIS                                       ,
                ULL(0x00020007), SH_UNT_PU_PIB2OPB0, SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_PIB2OPB1_CRSIS                                       ,
                ULL(0x00020017), SH_UNT_PU_PIB2OPB1, SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_NPU0_CTL_CTLSM_HOLD0                                 ,
                ULL(0x05011097), SH_UNT_PU_NPU0_CTL, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_CTL_CTLSM_HOLD0                                 ,
                ULL(0x05011197), SH_UNT_PU_NPU1_CTL, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_CTL_CTLSM_HOLD0                                 ,
                ULL(0x05011297), SH_UNT_PU_NPU2_CTL, SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU0_CTL_CTLSM_HOLD1                                 ,
                ULL(0x05011098), SH_UNT_PU_NPU0_CTL, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_CTL_CTLSM_HOLD1                                 ,
                ULL(0x05011198), SH_UNT_PU_NPU1_CTL, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_CTL_CTLSM_HOLD1                                 ,
                ULL(0x05011298), SH_UNT_PU_NPU2_CTL, SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU0_CTL_CTLSM_MASK0                                 ,
                ULL(0x05011099), SH_UNT_PU_NPU0_CTL, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_CTL_CTLSM_MASK0                                 ,
                ULL(0x05011199), SH_UNT_PU_NPU1_CTL, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_CTL_CTLSM_MASK0                                 ,
                ULL(0x05011299), SH_UNT_PU_NPU2_CTL, SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU0_CTL_CTLSM_MASK1                                 ,
                ULL(0x0501109A), SH_UNT_PU_NPU0_CTL, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_CTL_CTLSM_MASK1                                 ,
                ULL(0x0501119A), SH_UNT_PU_NPU1_CTL, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_CTL_CTLSM_MASK1                                 ,
                ULL(0x0501129A), SH_UNT_PU_NPU2_CTL, SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU0_CTL_CTL_HOLD                                    ,
                ULL(0x0501109B), SH_UNT_PU_NPU0_CTL, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_CTL_CTL_HOLD                                    ,
                ULL(0x0501119B), SH_UNT_PU_NPU1_CTL, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_CTL_CTL_HOLD                                    ,
                ULL(0x0501129B), SH_UNT_PU_NPU2_CTL, SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU0_CTL_CTL_MASK                                    ,
                ULL(0x0501109C), SH_UNT_PU_NPU0_CTL, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_CTL_CTL_MASK                                    ,
                ULL(0x0501119C), SH_UNT_PU_NPU1_CTL, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_CTL_CTL_MASK                                    ,
                ULL(0x0501129C), SH_UNT_PU_NPU2_CTL, SH_ACS_SCOM      );

CONST_UINT64_T( PU_DATA0TO7_REGISTER_B                                  ,
                ULL(0x000A0003), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_DATA0TO7_REGISTER_C                                  ,
                ULL(0x000A1003), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_DATA0TO7_REGISTER_D                                  ,
                ULL(0x000A2003), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_DATA0TO7_REGISTER_E                                  ,
                ULL(0x000A3003), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_DATA8TO15_REGISTER_B                                 ,
                ULL(0x000A0001), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_DATA8TO15_REGISTER_C                                 ,
                ULL(0x000A1001), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_DATA8TO15_REGISTER_D                                 ,
                ULL(0x000A2001), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_DATA8TO15_REGISTER_E                                 ,
                ULL(0x000A3001), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_DATATAG_0_HASH_FUNCTION_REG                          ,
                ULL(0x0201114C), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_DATATAG_1_HASH_FUNCTION_REG                          ,
                ULL(0x0201114D), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_DATATAG_2_HASH_FUNCTION_REG                          ,
                ULL(0x0201114E), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_DATATAG_3_HASH_FUNCTION_REG                          ,
                ULL(0x0201114F), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_DATATAG_4_HASH_FUNCTION_REG                          ,
                ULL(0x02011150), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_DATATAG_5_HASH_FUNCTION_REG                          ,
                ULL(0x02011151), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_DATA_REGISTER                                        ,
                ULL(0x00010003), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU_DAT_DA_ADDR                                      ,
                ULL(0x050113BE), SH_UNT_PU_NPU_DAT, SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU_DAT_DA_DATA                                      ,
                ULL(0x050113BF), SH_UNT_PU_NPU_DAT, SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_NPU0_CTL_DEBUG0_CONFIG                               ,
                ULL(0x05011088), SH_UNT_PU_NPU0_CTL, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_DAT_DEBUG0_CONFIG                               ,
                ULL(0x050110B4), SH_UNT_PU_NPU0_DAT, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_CTL_DEBUG0_CONFIG                               ,
                ULL(0x05011188), SH_UNT_PU_NPU1_CTL, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_DAT_DEBUG0_CONFIG                               ,
                ULL(0x050111B4), SH_UNT_PU_NPU1_DAT, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_DAT_DEBUG0_CONFIG                               ,
                ULL(0x050112B4), SH_UNT_PU_NPU2_DAT, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_CTL_DEBUG0_CONFIG                               ,
                ULL(0x05011288), SH_UNT_PU_NPU2_CTL, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM0_DEBUG0_CONFIG                               ,
                ULL(0x05011008), SH_UNT_PU_NPU0_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM1_DEBUG0_CONFIG                               ,
                ULL(0x05011028), SH_UNT_PU_NPU0_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM2_DEBUG0_CONFIG                               ,
                ULL(0x05011048), SH_UNT_PU_NPU0_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM3_DEBUG0_CONFIG                               ,
                ULL(0x05011068), SH_UNT_PU_NPU0_SM3, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM0_DEBUG0_CONFIG                               ,
                ULL(0x05011108), SH_UNT_PU_NPU1_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM1_DEBUG0_CONFIG                               ,
                ULL(0x05011128), SH_UNT_PU_NPU1_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM2_DEBUG0_CONFIG                               ,
                ULL(0x05011148), SH_UNT_PU_NPU1_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM3_DEBUG0_CONFIG                               ,
                ULL(0x05011168), SH_UNT_PU_NPU1_SM3, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM0_DEBUG0_CONFIG                               ,
                ULL(0x05011208), SH_UNT_PU_NPU2_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM1_DEBUG0_CONFIG                               ,
                ULL(0x05011228), SH_UNT_PU_NPU2_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM2_DEBUG0_CONFIG                               ,
                ULL(0x05011248), SH_UNT_PU_NPU2_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM3_DEBUG0_CONFIG                               ,
                ULL(0x05011268), SH_UNT_PU_NPU2_SM3, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU_SM2_DEBUG0_CONFIG                                ,
                ULL(0x05011346), SH_UNT_PU_NPU_SM2, SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU0_CTL_DEBUG1_CONFIG                               ,
                ULL(0x05011089), SH_UNT_PU_NPU0_CTL, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_DAT_DEBUG1_CONFIG                               ,
                ULL(0x050110B5), SH_UNT_PU_NPU0_DAT, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_CTL_DEBUG1_CONFIG                               ,
                ULL(0x05011189), SH_UNT_PU_NPU1_CTL, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_DAT_DEBUG1_CONFIG                               ,
                ULL(0x050111B5), SH_UNT_PU_NPU1_DAT, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_DAT_DEBUG1_CONFIG                               ,
                ULL(0x050112B5), SH_UNT_PU_NPU2_DAT, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_CTL_DEBUG1_CONFIG                               ,
                ULL(0x05011289), SH_UNT_PU_NPU2_CTL, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM0_DEBUG1_CONFIG                               ,
                ULL(0x05011009), SH_UNT_PU_NPU0_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM1_DEBUG1_CONFIG                               ,
                ULL(0x05011029), SH_UNT_PU_NPU0_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM2_DEBUG1_CONFIG                               ,
                ULL(0x05011049), SH_UNT_PU_NPU0_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM3_DEBUG1_CONFIG                               ,
                ULL(0x05011069), SH_UNT_PU_NPU0_SM3, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM0_DEBUG1_CONFIG                               ,
                ULL(0x05011109), SH_UNT_PU_NPU1_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM1_DEBUG1_CONFIG                               ,
                ULL(0x05011129), SH_UNT_PU_NPU1_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM2_DEBUG1_CONFIG                               ,
                ULL(0x05011149), SH_UNT_PU_NPU1_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM3_DEBUG1_CONFIG                               ,
                ULL(0x05011169), SH_UNT_PU_NPU1_SM3, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM0_DEBUG1_CONFIG                               ,
                ULL(0x05011209), SH_UNT_PU_NPU2_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM1_DEBUG1_CONFIG                               ,
                ULL(0x05011229), SH_UNT_PU_NPU2_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM2_DEBUG1_CONFIG                               ,
                ULL(0x05011249), SH_UNT_PU_NPU2_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM3_DEBUG1_CONFIG                               ,
                ULL(0x05011269), SH_UNT_PU_NPU2_SM3, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU_SM2_DEBUG1_CONFIG                                ,
                ULL(0x05011347), SH_UNT_PU_NPU_SM2, SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU_CTL_DEBUG_CONFIG                                 ,
                ULL(0x05011380), SH_UNT_PU_NPU_CTL, SH_ACS_SCOM      );

CONST_UINT64_T( PU_DISABLE_FORCE_PFET_OFF                               ,
                ULL(0x0001000D), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU0_DISPALT_CTRL                                    ,
                ULL(0x050110AC), SH_UNT_PU_NPU0  , SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_DISPALT_CTRL                                    ,
                ULL(0x050111AC), SH_UNT_PU_NPU1  , SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_DISPALT_CTRL                                    ,
                ULL(0x050112AC), SH_UNT_PU_NPU2  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU0_DISPALT_DAT0                                    ,
                ULL(0x050110AD), SH_UNT_PU_NPU0  , SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_DISPALT_DAT0                                    ,
                ULL(0x050111AD), SH_UNT_PU_NPU1  , SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_DISPALT_DAT0                                    ,
                ULL(0x050112AD), SH_UNT_PU_NPU2  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU0_DISPALT_DAT1                                    ,
                ULL(0x050110AE), SH_UNT_PU_NPU0  , SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_DISPALT_DAT1                                    ,
                ULL(0x050111AE), SH_UNT_PU_NPU1  , SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_DISPALT_DAT1                                    ,
                ULL(0x050112AE), SH_UNT_PU_NPU2  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU0_DISPALT_STAT_ECC                                ,
                ULL(0x050110AF), SH_UNT_PU_NPU0  , SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_DISPALT_STAT_ECC                                ,
                ULL(0x050111AF), SH_UNT_PU_NPU1  , SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_DISPALT_STAT_ECC                                ,
                ULL(0x050112AF), SH_UNT_PU_NPU2  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_DMA_UP_ADDR                                          ,
                ULL(0x05012914), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_DMA_VAS_MMIO_BAR                                     ,
                ULL(0x0201105E), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PEC_DRPPRICTL_REG                                       ,
                ULL(0x04010C01), SH_UNT_PEC      , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_0_DRPPRICTL_REG                                     ,
                ULL(0x04010C01), SH_UNT_PEC_0    , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_1_DRPPRICTL_REG                                     ,
                ULL(0x04011001), SH_UNT_PEC_1    , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_2_DRPPRICTL_REG                                     ,
                ULL(0x04011401), SH_UNT_PEC_2    , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_NPU0_ECC_CONFIG                                      ,
                ULL(0x050110A2), SH_UNT_PU_NPU0  , SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_ECC_CONFIG                                      ,
                ULL(0x050111A2), SH_UNT_PU_NPU1  , SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_ECC_CONFIG                                      ,
                ULL(0x050112A2), SH_UNT_PU_NPU2  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU0_ECC_COUNT                                       ,
                ULL(0x050110A3), SH_UNT_PU_NPU0  , SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_ECC_COUNT                                       ,
                ULL(0x050111A3), SH_UNT_PU_NPU1  , SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_ECC_COUNT                                       ,
                ULL(0x050112A3), SH_UNT_PU_NPU2  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU0_ECC_ERRINJ                                      ,
                ULL(0x050110A1), SH_UNT_PU_NPU0  , SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_ECC_ERRINJ                                      ,
                ULL(0x050111A1), SH_UNT_PU_NPU1  , SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_ECC_ERRINJ                                      ,
                ULL(0x050112A1), SH_UNT_PU_NPU2  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART0_REGISTER                          ,
                ULL(0x00018000), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART0_REGISTER                          ,
                ULL(0x00018040), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART10_REGISTER                         ,
                ULL(0x0001800A), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART10_REGISTER                         ,
                ULL(0x0001804A), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART11_REGISTER                         ,
                ULL(0x0001800B), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART11_REGISTER                         ,
                ULL(0x0001804B), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART12_REGISTER                         ,
                ULL(0x0001800C), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART12_REGISTER                         ,
                ULL(0x0001804C), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART13_REGISTER                         ,
                ULL(0x0001800D), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART13_REGISTER                         ,
                ULL(0x0001804D), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART14_REGISTER                         ,
                ULL(0x0001800E), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART14_REGISTER                         ,
                ULL(0x0001804E), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART15_REGISTER                         ,
                ULL(0x0001800F), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART15_REGISTER                         ,
                ULL(0x0001804F), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART16_REGISTER                         ,
                ULL(0x00018010), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART16_REGISTER                         ,
                ULL(0x00018050), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART17_REGISTER                         ,
                ULL(0x00018011), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART17_REGISTER                         ,
                ULL(0x00018051), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART18_REGISTER                         ,
                ULL(0x00018012), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART18_REGISTER                         ,
                ULL(0x00018052), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART19_REGISTER                         ,
                ULL(0x00018013), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART19_REGISTER                         ,
                ULL(0x00018053), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART1_REGISTER                          ,
                ULL(0x00018001), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART1_REGISTER                          ,
                ULL(0x00018041), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART20_REGISTER                         ,
                ULL(0x00018014), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART20_REGISTER                         ,
                ULL(0x00018054), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART21_REGISTER                         ,
                ULL(0x00018015), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART21_REGISTER                         ,
                ULL(0x00018055), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART22_REGISTER                         ,
                ULL(0x00018016), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART22_REGISTER                         ,
                ULL(0x00018056), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART23_REGISTER                         ,
                ULL(0x00018017), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART23_REGISTER                         ,
                ULL(0x00018057), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART24_REGISTER                         ,
                ULL(0x00018018), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART24_REGISTER                         ,
                ULL(0x00018058), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART25_REGISTER                         ,
                ULL(0x00018019), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART25_REGISTER                         ,
                ULL(0x00018059), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART26_REGISTER                         ,
                ULL(0x0001801A), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART26_REGISTER                         ,
                ULL(0x0001805A), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART27_REGISTER                         ,
                ULL(0x0001801B), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART27_REGISTER                         ,
                ULL(0x0001805B), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART28_REGISTER                         ,
                ULL(0x0001801C), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART28_REGISTER                         ,
                ULL(0x0001805C), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART29_REGISTER                         ,
                ULL(0x0001801D), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART29_REGISTER                         ,
                ULL(0x0001805D), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART2_REGISTER                          ,
                ULL(0x00018002), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART2_REGISTER                          ,
                ULL(0x00018042), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART30_REGISTER                         ,
                ULL(0x0001801E), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART30_REGISTER                         ,
                ULL(0x0001805E), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART31_REGISTER                         ,
                ULL(0x0001801F), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART31_REGISTER                         ,
                ULL(0x0001805F), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART32_REGISTER                         ,
                ULL(0x00018020), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART32_REGISTER                         ,
                ULL(0x00018060), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART33_REGISTER                         ,
                ULL(0x00018021), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART33_REGISTER                         ,
                ULL(0x00018061), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART34_REGISTER                         ,
                ULL(0x00018022), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART34_REGISTER                         ,
                ULL(0x00018062), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART35_REGISTER                         ,
                ULL(0x00018023), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART35_REGISTER                         ,
                ULL(0x00018063), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART36_REGISTER                         ,
                ULL(0x00018024), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART36_REGISTER                         ,
                ULL(0x00018064), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART37_REGISTER                         ,
                ULL(0x00018025), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART37_REGISTER                         ,
                ULL(0x00018065), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART38_REGISTER                         ,
                ULL(0x00018026), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART38_REGISTER                         ,
                ULL(0x00018066), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART39_REGISTER                         ,
                ULL(0x00018027), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART39_REGISTER                         ,
                ULL(0x00018067), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART3_REGISTER                          ,
                ULL(0x00018003), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART3_REGISTER                          ,
                ULL(0x00018043), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART40_REGISTER                         ,
                ULL(0x00018028), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART40_REGISTER                         ,
                ULL(0x00018068), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART41_REGISTER                         ,
                ULL(0x00018029), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART41_REGISTER                         ,
                ULL(0x00018069), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART42_REGISTER                         ,
                ULL(0x0001802A), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART42_REGISTER                         ,
                ULL(0x0001806A), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART43_REGISTER                         ,
                ULL(0x0001802B), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART43_REGISTER                         ,
                ULL(0x0001806B), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART44_REGISTER                         ,
                ULL(0x0001802C), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART44_REGISTER                         ,
                ULL(0x0001806C), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART45_REGISTER                         ,
                ULL(0x0001802D), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART45_REGISTER                         ,
                ULL(0x0001806D), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART46_REGISTER                         ,
                ULL(0x0001802E), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART46_REGISTER                         ,
                ULL(0x0001806E), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART47_REGISTER                         ,
                ULL(0x0001802F), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART47_REGISTER                         ,
                ULL(0x0001806F), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART48_REGISTER                         ,
                ULL(0x00018030), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART48_REGISTER                         ,
                ULL(0x00018070), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART49_REGISTER                         ,
                ULL(0x00018031), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART49_REGISTER                         ,
                ULL(0x00018071), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART4_REGISTER                          ,
                ULL(0x00018004), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART4_REGISTER                          ,
                ULL(0x00018044), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART50_REGISTER                         ,
                ULL(0x00018032), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART50_REGISTER                         ,
                ULL(0x00018072), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART51_REGISTER                         ,
                ULL(0x00018033), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART51_REGISTER                         ,
                ULL(0x00018073), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART52_REGISTER                         ,
                ULL(0x00018034), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART52_REGISTER                         ,
                ULL(0x00018074), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART53_REGISTER                         ,
                ULL(0x00018035), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART53_REGISTER                         ,
                ULL(0x00018075), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART54_REGISTER                         ,
                ULL(0x00018036), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART54_REGISTER                         ,
                ULL(0x00018076), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART55_REGISTER                         ,
                ULL(0x00018037), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART55_REGISTER                         ,
                ULL(0x00018077), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART56_REGISTER                         ,
                ULL(0x00018038), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART56_REGISTER                         ,
                ULL(0x00018078), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART57_REGISTER                         ,
                ULL(0x00018039), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART57_REGISTER                         ,
                ULL(0x00018079), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART58_REGISTER                         ,
                ULL(0x0001803A), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART58_REGISTER                         ,
                ULL(0x0001807A), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART59_REGISTER                         ,
                ULL(0x0001803B), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART59_REGISTER                         ,
                ULL(0x0001807B), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART5_REGISTER                          ,
                ULL(0x00018005), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART5_REGISTER                          ,
                ULL(0x00018045), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART60_REGISTER                         ,
                ULL(0x0001803C), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART60_REGISTER                         ,
                ULL(0x0001807C), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART61_REGISTER                         ,
                ULL(0x0001803D), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART61_REGISTER                         ,
                ULL(0x0001807D), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART62_REGISTER                         ,
                ULL(0x0001803E), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART62_REGISTER                         ,
                ULL(0x0001807E), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART63_REGISTER                         ,
                ULL(0x0001803F), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART63_REGISTER                         ,
                ULL(0x0001807F), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART6_REGISTER                          ,
                ULL(0x00018006), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART6_REGISTER                          ,
                ULL(0x00018046), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART7_REGISTER                          ,
                ULL(0x00018007), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART7_REGISTER                          ,
                ULL(0x00018047), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART8_REGISTER                          ,
                ULL(0x00018008), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART8_REGISTER                          ,
                ULL(0x00018048), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_OTPROM0_ECID_PART9_REGISTER                          ,
                ULL(0x00018009), SH_UNT_PU_OTPROM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_OTPROM1_ECID_PART9_REGISTER                          ,
                ULL(0x00018049), SH_UNT_PU_OTPROM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_EECNT_REG                                            ,
                ULL(0x05012809), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_EFT_HI_PRIOR_RCV_FIFO_ASB                            ,
                ULL(0x020110C6), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_EFT_HI_PRIOR_RCV_FIFO_BAR                            ,
                ULL(0x020110C0), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_EFT_HI_PRIOR_RCV_FIFO_CNTL                           ,
                ULL(0x020110C3), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_EFT_LO_PRIOR_RCV_FIFO_ASB                            ,
                ULL(0x020110CF), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_EFT_LO_PRIOR_RCV_FIFO_BAR                            ,
                ULL(0x020110C9), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_EFT_LO_PRIOR_RCV_FIFO_CNTL                           ,
                ULL(0x020110CC), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_EFT_MAX_BYTE_CNT                                     ,
                ULL(0x02011059), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_EHHCA_FIR_ACTION0_REG                                ,
                ULL(0x05012986), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_EHHCA_FIR_ACTION1_REG                                ,
                ULL(0x05012987), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_EHHCA_FIR_MASK_REG                                   ,
                ULL(0x05012983), SH_UNT          , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_EHHCA_FIR_MASK_REG_AND                               ,
                ULL(0x05012984), SH_UNT          , SH_ACS_SCOM1_AND );
CONST_UINT64_T( PU_EHHCA_FIR_MASK_REG_OR                                ,
                ULL(0x05012985), SH_UNT          , SH_ACS_SCOM2_OR  );

CONST_UINT64_T( PU_EHHCA_FIR_REG                                        ,
                ULL(0x05012980), SH_UNT          , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_EHHCA_FIR_REG_AND                                    ,
                ULL(0x05012981), SH_UNT          , SH_ACS_SCOM1_AND );
CONST_UINT64_T( PU_EHHCA_FIR_REG_OR                                     ,
                ULL(0x05012982), SH_UNT          , SH_ACS_SCOM2_OR  );

CONST_UINT64_T( PU_NPU0_SM0_EPSILON_CONFIG                              ,
                ULL(0x05011004), SH_UNT_PU_NPU0_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM1_EPSILON_CONFIG                              ,
                ULL(0x05011024), SH_UNT_PU_NPU0_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM2_EPSILON_CONFIG                              ,
                ULL(0x05011044), SH_UNT_PU_NPU0_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM3_EPSILON_CONFIG                              ,
                ULL(0x05011064), SH_UNT_PU_NPU0_SM3, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM0_EPSILON_CONFIG                              ,
                ULL(0x05011104), SH_UNT_PU_NPU1_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM1_EPSILON_CONFIG                              ,
                ULL(0x05011124), SH_UNT_PU_NPU1_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM2_EPSILON_CONFIG                              ,
                ULL(0x05011144), SH_UNT_PU_NPU1_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM3_EPSILON_CONFIG                              ,
                ULL(0x05011164), SH_UNT_PU_NPU1_SM3, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM0_EPSILON_CONFIG                              ,
                ULL(0x05011204), SH_UNT_PU_NPU2_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM1_EPSILON_CONFIG                              ,
                ULL(0x05011224), SH_UNT_PU_NPU2_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM2_EPSILON_CONFIG                              ,
                ULL(0x05011244), SH_UNT_PU_NPU2_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM3_EPSILON_CONFIG                              ,
                ULL(0x05011264), SH_UNT_PU_NPU2_SM3, SH_ACS_SCOM      );

CONST_UINT64_T( PU_ERAT_STATUS_CONTROL                                  ,
                ULL(0x020110D6), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU0_NTL0_ERROR1                                     ,
                ULL(0x050110C4), SH_UNT_PU_NPU0_NTL0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_NTL1_ERROR1                                     ,
                ULL(0x050110E4), SH_UNT_PU_NPU0_NTL1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM0_ERROR1                                      ,
                ULL(0x05011014), SH_UNT_PU_NPU0_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM1_ERROR1                                      ,
                ULL(0x05011034), SH_UNT_PU_NPU0_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM2_ERROR1                                      ,
                ULL(0x05011054), SH_UNT_PU_NPU0_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM3_ERROR1                                      ,
                ULL(0x05011074), SH_UNT_PU_NPU0_SM3, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_NTL0_ERROR1                                     ,
                ULL(0x050111C4), SH_UNT_PU_NPU1_NTL0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_NTL1_ERROR1                                     ,
                ULL(0x050111E4), SH_UNT_PU_NPU1_NTL1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM0_ERROR1                                      ,
                ULL(0x05011114), SH_UNT_PU_NPU1_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM1_ERROR1                                      ,
                ULL(0x05011134), SH_UNT_PU_NPU1_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM2_ERROR1                                      ,
                ULL(0x05011154), SH_UNT_PU_NPU1_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM3_ERROR1                                      ,
                ULL(0x05011174), SH_UNT_PU_NPU1_SM3, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_NTL0_ERROR1                                     ,
                ULL(0x050112C4), SH_UNT_PU_NPU2_NTL0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_NTL1_ERROR1                                     ,
                ULL(0x050112E4), SH_UNT_PU_NPU2_NTL1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM0_ERROR1                                      ,
                ULL(0x05011214), SH_UNT_PU_NPU2_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM1_ERROR1                                      ,
                ULL(0x05011234), SH_UNT_PU_NPU2_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM2_ERROR1                                      ,
                ULL(0x05011254), SH_UNT_PU_NPU2_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM3_ERROR1                                      ,
                ULL(0x05011274), SH_UNT_PU_NPU2_SM3, SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU0_CTL_ERROR2                                      ,
                ULL(0x05011096), SH_UNT_PU_NPU0_CTL, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_CTL_ERROR2                                      ,
                ULL(0x05011196), SH_UNT_PU_NPU1_CTL, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_CTL_ERROR2                                      ,
                ULL(0x05011296), SH_UNT_PU_NPU2_CTL, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_NTL0_ERROR2                                     ,
                ULL(0x050110C6), SH_UNT_PU_NPU0_NTL0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_NTL1_ERROR2                                     ,
                ULL(0x050110E6), SH_UNT_PU_NPU0_NTL1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM0_ERROR2                                      ,
                ULL(0x05011016), SH_UNT_PU_NPU0_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM1_ERROR2                                      ,
                ULL(0x05011036), SH_UNT_PU_NPU0_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM2_ERROR2                                      ,
                ULL(0x05011056), SH_UNT_PU_NPU0_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM3_ERROR2                                      ,
                ULL(0x05011076), SH_UNT_PU_NPU0_SM3, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_NTL0_ERROR2                                     ,
                ULL(0x050111C6), SH_UNT_PU_NPU1_NTL0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_NTL1_ERROR2                                     ,
                ULL(0x050111E6), SH_UNT_PU_NPU1_NTL1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM0_ERROR2                                      ,
                ULL(0x05011116), SH_UNT_PU_NPU1_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM1_ERROR2                                      ,
                ULL(0x05011136), SH_UNT_PU_NPU1_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM2_ERROR2                                      ,
                ULL(0x05011156), SH_UNT_PU_NPU1_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM3_ERROR2                                      ,
                ULL(0x05011176), SH_UNT_PU_NPU1_SM3, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_NTL0_ERROR2                                     ,
                ULL(0x050112C6), SH_UNT_PU_NPU2_NTL0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_NTL1_ERROR2                                     ,
                ULL(0x050112E6), SH_UNT_PU_NPU2_NTL1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM0_ERROR2                                      ,
                ULL(0x05011216), SH_UNT_PU_NPU2_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM1_ERROR2                                      ,
                ULL(0x05011236), SH_UNT_PU_NPU2_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM2_ERROR2                                      ,
                ULL(0x05011256), SH_UNT_PU_NPU2_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM3_ERROR2                                      ,
                ULL(0x05011276), SH_UNT_PU_NPU2_SM3, SH_ACS_SCOM      );

CONST_UINT64_T( PU_ESB_CI_BASE                                          ,
                ULL(0x05012916), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_ESB_NOTIFY                                           ,
                ULL(0x05012917), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_EXPORT_REGL_CTRL                                     ,
                ULL(0x0001000E), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_EXTENDED_STATUS_B                                    ,
                ULL(0x000A000C), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_EXTENDED_STATUS_C                                    ,
                ULL(0x000A100C), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_EXTENDED_STATUS_D                                    ,
                ULL(0x000A200C), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_EXTENDED_STATUS_E                                    ,
                ULL(0x000A300C), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_FIFO1_REGISTER_READ_B                                ,
                ULL(0x000A0004), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_FIFO1_REGISTER_READ_C                                ,
                ULL(0x000A1004), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_FIFO1_REGISTER_READ_D                                ,
                ULL(0x000A2004), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_FIFO1_REGISTER_READ_E                                ,
                ULL(0x000A3004), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_FIFO4_REGISTER_READ_B                                ,
                ULL(0x000A0012), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_FIFO4_REGISTER_READ_C                                ,
                ULL(0x000A1012), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_FIFO4_REGISTER_READ_D                                ,
                ULL(0x000A2012), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_FIFO4_REGISTER_READ_E                                ,
                ULL(0x000A3012), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_FIR_ACTION0_REG                                      ,
                ULL(0x06010806), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_IOP0_FIR_ACTION0_REG                                 ,
                ULL(0x0D010C06), SH_UNT_PU_IOP0  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_IOP1_FIR_ACTION0_REG                                 ,
                ULL(0x0E010C06), SH_UNT_PU_IOP1  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_IOP2_FIR_ACTION0_REG                                 ,
                ULL(0x0F010C06), SH_UNT_PU_IOP2  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_NPU_CTL_FIR_ACTION0_REG                              ,
                ULL(0x05011786), SH_UNT_PU_NPU_CTL, SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_FIR_ACTION1_REG                                      ,
                ULL(0x06010807), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_IOP0_FIR_ACTION1_REG                                 ,
                ULL(0x0D010C07), SH_UNT_PU_IOP0  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_IOP1_FIR_ACTION1_REG                                 ,
                ULL(0x0E010C07), SH_UNT_PU_IOP1  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_IOP2_FIR_ACTION1_REG                                 ,
                ULL(0x0F010C07), SH_UNT_PU_IOP2  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_NPU_CTL_FIR_ACTION1_REG                              ,
                ULL(0x05011787), SH_UNT_PU_NPU_CTL, SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_FIR_MASK_REG                                         ,
                ULL(0x06010803), SH_UNT          , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_FIR_MASK_REG_AND                                     ,
                ULL(0x06010804), SH_UNT          , SH_ACS_SCOM1_AND );
CONST_UINT64_T( PU_FIR_MASK_REG_OR                                      ,
                ULL(0x06010805), SH_UNT          , SH_ACS_SCOM2_OR  );

CONST_UINT64_T( PU_IOP0_FIR_MASK_REG                                    ,
                ULL(0x0D010C03), SH_UNT_PU_IOP0  , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_IOP0_FIR_MASK_REG_AND                                ,
                ULL(0x0D010C04), SH_UNT_PU_IOP0  , SH_ACS_SCOM1_AND );
CONST_UINT64_T( PU_IOP0_FIR_MASK_REG_OR                                 ,
                ULL(0x0D010C05), SH_UNT_PU_IOP0  , SH_ACS_SCOM2_OR  );
CONST_UINT64_T( PU_IOP1_FIR_MASK_REG                                    ,
                ULL(0x0E010C03), SH_UNT_PU_IOP1  , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_IOP1_FIR_MASK_REG_AND                                ,
                ULL(0x0E010C04), SH_UNT_PU_IOP1  , SH_ACS_SCOM1_AND );
CONST_UINT64_T( PU_IOP1_FIR_MASK_REG_OR                                 ,
                ULL(0x0E010C05), SH_UNT_PU_IOP1  , SH_ACS_SCOM2_OR  );
CONST_UINT64_T( PU_IOP2_FIR_MASK_REG                                    ,
                ULL(0x0F010C03), SH_UNT_PU_IOP2  , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_IOP2_FIR_MASK_REG_AND                                ,
                ULL(0x0F010C04), SH_UNT_PU_IOP2  , SH_ACS_SCOM1_AND );
CONST_UINT64_T( PU_IOP2_FIR_MASK_REG_OR                                 ,
                ULL(0x0F010C05), SH_UNT_PU_IOP2  , SH_ACS_SCOM2_OR  );
CONST_UINT64_T( PU_NPU_CTL_FIR_MASK_REG                                 ,
                ULL(0x05011783), SH_UNT_PU_NPU_CTL, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_NPU_CTL_FIR_MASK_REG_AND                             ,
                ULL(0x05011784), SH_UNT_PU_NPU_CTL, SH_ACS_SCOM1_AND );
CONST_UINT64_T( PU_NPU_CTL_FIR_MASK_REG_OR                              ,
                ULL(0x05011785), SH_UNT_PU_NPU_CTL, SH_ACS_SCOM2_OR  );

CONST_UINT64_T( PU_FIR_MASK_REGISTER                                    ,
                ULL(0x00088008), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_FIR_REG                                              ,
                ULL(0x06010800), SH_UNT          , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_FIR_REG_AND                                          ,
                ULL(0x06010801), SH_UNT          , SH_ACS_SCOM1_AND );
CONST_UINT64_T( PU_FIR_REG_OR                                           ,
                ULL(0x06010802), SH_UNT          , SH_ACS_SCOM2_OR  );

CONST_UINT64_T( PU_NPU_CTL_FIR_REG                                      ,
                ULL(0x05011780), SH_UNT_PU_NPU_CTL, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_NPU_CTL_FIR_REG_AND                                  ,
                ULL(0x05011781), SH_UNT_PU_NPU_CTL, SH_ACS_SCOM1_AND );
CONST_UINT64_T( PU_NPU_CTL_FIR_REG_OR                                   ,
                ULL(0x05011782), SH_UNT_PU_NPU_CTL, SH_ACS_SCOM2_OR  );

CONST_UINT64_T( PU_IOP0_FIR_STATUS_REG                                  ,
                ULL(0x0D010C00), SH_UNT_PU_IOP0  , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_IOP0_FIR_STATUS_REG_AND                              ,
                ULL(0x0D010C01), SH_UNT_PU_IOP0  , SH_ACS_SCOM1_AND );
CONST_UINT64_T( PU_IOP0_FIR_STATUS_REG_OR                               ,
                ULL(0x0D010C02), SH_UNT_PU_IOP0  , SH_ACS_SCOM2_OR  );
CONST_UINT64_T( PU_IOP1_FIR_STATUS_REG                                  ,
                ULL(0x0E010C00), SH_UNT_PU_IOP1  , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_IOP1_FIR_STATUS_REG_AND                              ,
                ULL(0x0E010C01), SH_UNT_PU_IOP1  , SH_ACS_SCOM1_AND );
CONST_UINT64_T( PU_IOP1_FIR_STATUS_REG_OR                               ,
                ULL(0x0E010C02), SH_UNT_PU_IOP1  , SH_ACS_SCOM2_OR  );
CONST_UINT64_T( PU_IOP2_FIR_STATUS_REG                                  ,
                ULL(0x0F010C00), SH_UNT_PU_IOP2  , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_IOP2_FIR_STATUS_REG_AND                              ,
                ULL(0x0F010C01), SH_UNT_PU_IOP2  , SH_ACS_SCOM1_AND );
CONST_UINT64_T( PU_IOP2_FIR_STATUS_REG_OR                               ,
                ULL(0x0F010C02), SH_UNT_PU_IOP2  , SH_ACS_SCOM2_OR  );

CONST_UINT64_T( PU_FIR_WOF_REG                                          ,
                ULL(0x06010808), SH_UNT          , SH_ACS_SCOM_WCLRREG );
CONST_UINT64_T( PU_IOP0_FIR_WOF_REG                                     ,
                ULL(0x0D010C08), SH_UNT_PU_IOP0  , SH_ACS_SCOM_WCLRREG );
CONST_UINT64_T( PU_IOP1_FIR_WOF_REG                                     ,
                ULL(0x0E010C08), SH_UNT_PU_IOP1  , SH_ACS_SCOM_WCLRREG );
CONST_UINT64_T( PU_IOP2_FIR_WOF_REG                                     ,
                ULL(0x0F010C08), SH_UNT_PU_IOP2  , SH_ACS_SCOM_WCLRREG );
CONST_UINT64_T( PU_NPU_CTL_FIR_WOF_REG                                  ,
                ULL(0x05011788), SH_UNT_PU_NPU_CTL, SH_ACS_SCOM_WCLRREG );

CONST_UINT64_T( PU_FMU_FORCE_OP_REG                                     ,
                ULL(0x0102002B), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_FMU_MODE_REG                                         ,
                ULL(0x01020028), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_FMU_OSC_CNTR1_REG                                    ,
                ULL(0x01020029), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_FMU_OSC_CNTR2_REG                                    ,
                ULL(0x0102002A), SH_UNT          , SH_ACS_SCOM      );

// ERROR: ALREADY DEFINED !  01020029, _SCOM ::   CONST_UINT64_T( PU_FMU_PULSE_GEN_REG                                    , ULL(0x01020029), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_FORCE_ECC_REG                                        ,
                ULL(0x0502280D), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_FSB_DOWNFIFO_DATA_IN                                 ,
                ULL(0x000B0010), SH_UNT          , SH_ACS_SCOM_WO   );

CONST_UINT64_T( PU_FSB_DOWNFIFO_REQ_RESET                               ,
                ULL(0x000B0013), SH_UNT          , SH_ACS_SCOM_WO   );

CONST_UINT64_T( PU_FSB_DOWNFIFO_SIG_EOT                                 ,
                ULL(0x000B0012), SH_UNT          , SH_ACS_SCOM_WO   );

CONST_UINT64_T( PU_FSB_DOWNFIFO_STATUS                                  ,
                ULL(0x000B0011), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_FSB_UPFIFO_ACK_EOT                                   ,
                ULL(0x000B0005), SH_UNT          , SH_ACS_SCOM_WO   );

CONST_UINT64_T( PU_FSB_UPFIFO_DATA_OUT                                  ,
                ULL(0x000B0000), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_FSB_UPFIFO_RESET                                     ,
                ULL(0x000B0004), SH_UNT          , SH_ACS_SCOM_WO   );

CONST_UINT64_T( PU_FSB_UPFIFO_STATUS                                    ,
                ULL(0x000B0001), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU0_SM0_GENID_BAR                                   ,
                ULL(0x05011019), SH_UNT_PU_NPU0_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM1_GENID_BAR                                   ,
                ULL(0x05011039), SH_UNT_PU_NPU0_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM2_GENID_BAR                                   ,
                ULL(0x05011059), SH_UNT_PU_NPU0_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM3_GENID_BAR                                   ,
                ULL(0x05011079), SH_UNT_PU_NPU0_SM3, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM0_GENID_BAR                                   ,
                ULL(0x05011119), SH_UNT_PU_NPU1_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM1_GENID_BAR                                   ,
                ULL(0x05011139), SH_UNT_PU_NPU1_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM2_GENID_BAR                                   ,
                ULL(0x05011159), SH_UNT_PU_NPU1_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM3_GENID_BAR                                   ,
                ULL(0x05011179), SH_UNT_PU_NPU1_SM3, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM0_GENID_BAR                                   ,
                ULL(0x05011219), SH_UNT_PU_NPU2_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM1_GENID_BAR                                   ,
                ULL(0x05011239), SH_UNT_PU_NPU2_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM2_GENID_BAR                                   ,
                ULL(0x05011259), SH_UNT_PU_NPU2_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM3_GENID_BAR                                   ,
                ULL(0x05011279), SH_UNT_PU_NPU2_SM3, SH_ACS_SCOM      );

CONST_UINT64_T( PU_GPE0_GPEDBG_OCI                                      ,
                ULL(0xC0000010), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE0_GPEDBG_SCOM                                     ,
                ULL(0x00060002), SH_UNT_PU_GPE0  , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_GPE1_GPEDBG_OCI                                      ,
                ULL(0xC0010010), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_GPEDBG_SCOM                                     ,
                ULL(0x00062002), SH_UNT_PU_GPE1  , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_GPE2_GPEDBG_OCI                                      ,
                ULL(0xC0020010), SH_UNT_PU_GPE2  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE2_GPEDBG_SCOM                                     ,
                ULL(0x00064002), SH_UNT_PU_GPE2  , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_GPE3_GPEDBG_OCI                                      ,
                ULL(0xC0030010), SH_UNT_PU_GPE3  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE3_GPEDBG_SCOM                                     ,
                ULL(0x00066002), SH_UNT_PU_GPE3  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE0_GPEIVPR_OCI                                     ,
                ULL(0xC0000008), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE0_GPEIVPR_SCOM                                    ,
                ULL(0x00060001), SH_UNT_PU_GPE0  , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_GPE1_GPEIVPR_OCI                                     ,
                ULL(0xC0010008), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_GPEIVPR_SCOM                                    ,
                ULL(0x00062001), SH_UNT_PU_GPE1  , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_GPE2_GPEIVPR_OCI                                     ,
                ULL(0xC0020008), SH_UNT_PU_GPE2  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE2_GPEIVPR_SCOM                                    ,
                ULL(0x00064001), SH_UNT_PU_GPE2  , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_GPE3_GPEIVPR_OCI                                     ,
                ULL(0xC0030008), SH_UNT_PU_GPE3  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE3_GPEIVPR_SCOM                                    ,
                ULL(0x00066001), SH_UNT_PU_GPE3  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE0_GPEMACR_OCI                                     ,
                ULL(0xC0000020), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE0_GPEMACR_SCOM                                    ,
                ULL(0x00060004), SH_UNT_PU_GPE0  , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_GPE1_GPEMACR_OCI                                     ,
                ULL(0xC0010020), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_GPEMACR_SCOM                                    ,
                ULL(0x00062004), SH_UNT_PU_GPE1  , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_GPE2_GPEMACR_OCI                                     ,
                ULL(0xC0020020), SH_UNT_PU_GPE2  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE2_GPEMACR_SCOM                                    ,
                ULL(0x00064004), SH_UNT_PU_GPE2  , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_GPE3_GPEMACR_OCI                                     ,
                ULL(0xC0030020), SH_UNT_PU_GPE3  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE3_GPEMACR_SCOM                                    ,
                ULL(0x00066004), SH_UNT_PU_GPE3  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE0_GPESTR_OCI                                      ,
                ULL(0xC0000018), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE0_GPESTR_SCOM                                     ,
                ULL(0x00060003), SH_UNT_PU_GPE0  , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_GPE1_GPESTR_OCI                                      ,
                ULL(0xC0010018), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_GPESTR_SCOM                                     ,
                ULL(0x00062003), SH_UNT_PU_GPE1  , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_GPE2_GPESTR_OCI                                      ,
                ULL(0xC0020018), SH_UNT_PU_GPE2  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE2_GPESTR_SCOM                                     ,
                ULL(0x00064003), SH_UNT_PU_GPE2  , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_GPE3_GPESTR_OCI                                      ,
                ULL(0xC0030018), SH_UNT_PU_GPE3  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE3_GPESTR_SCOM                                     ,
                ULL(0x00066003), SH_UNT_PU_GPE3  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE0_GPETSEL_OCI                                     ,
                ULL(0xC0000000), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE0_GPETSEL_SCOM                                    ,
                ULL(0x00060000), SH_UNT_PU_GPE0  , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_GPE1_GPETSEL_OCI                                     ,
                ULL(0xC0010000), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_GPETSEL_SCOM                                    ,
                ULL(0x00062000), SH_UNT_PU_GPE1  , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_GPE2_GPETSEL_OCI                                     ,
                ULL(0xC0020000), SH_UNT_PU_GPE2  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE2_GPETSEL_SCOM                                    ,
                ULL(0x00064000), SH_UNT_PU_GPE2  , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_GPE3_GPETSEL_OCI                                     ,
                ULL(0xC0030000), SH_UNT_PU_GPE3  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE3_GPETSEL_SCOM                                    ,
                ULL(0x00066000), SH_UNT_PU_GPE3  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE0_GPEXIEDR_OCI                                    ,
                ULL(0xC0000118), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE0_GPEXIEDR_SCOM                                   ,
                ULL(0x00060023), SH_UNT_PU_GPE0  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE1_GPEXIEDR_OCI                                    ,
                ULL(0xC0010118), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_GPEXIEDR_SCOM                                   ,
                ULL(0x00062023), SH_UNT_PU_GPE1  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE2_GPEXIEDR_OCI                                    ,
                ULL(0xC0020118), SH_UNT_PU_GPE2  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE2_GPEXIEDR_SCOM                                   ,
                ULL(0x00064023), SH_UNT_PU_GPE2  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE3_GPEXIEDR_OCI                                    ,
                ULL(0xC0030118), SH_UNT_PU_GPE3  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE3_GPEXIEDR_SCOM                                   ,
                ULL(0x00066023), SH_UNT_PU_GPE3  , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_GPE0_GPEXIIAR_OCI                                    ,
                ULL(0xC0000128), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE0_GPEXIIAR_SCOM                                   ,
                ULL(0x00060025), SH_UNT_PU_GPE0  , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_GPE1_GPEXIIAR_OCI                                    ,
                ULL(0xC0010128), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_GPEXIIAR_SCOM                                   ,
                ULL(0x00062025), SH_UNT_PU_GPE1  , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_GPE2_GPEXIIAR_OCI                                    ,
                ULL(0xC0020128), SH_UNT_PU_GPE2  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE2_GPEXIIAR_SCOM                                   ,
                ULL(0x00064025), SH_UNT_PU_GPE2  , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_GPE3_GPEXIIAR_OCI                                    ,
                ULL(0xC0030128), SH_UNT_PU_GPE3  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE3_GPEXIIAR_SCOM                                   ,
                ULL(0x00066025), SH_UNT_PU_GPE3  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE0_GPEXIIR_OCI                                     ,
                ULL(0xC0000120), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE0_GPEXIIR_SCOM                                    ,
                ULL(0x00060024), SH_UNT_PU_GPE0  , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_GPE1_GPEXIIR_OCI                                     ,
                ULL(0xC0010120), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_GPEXIIR_SCOM                                    ,
                ULL(0x00062024), SH_UNT_PU_GPE1  , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_GPE2_GPEXIIR_OCI                                     ,
                ULL(0xC0020120), SH_UNT_PU_GPE2  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE2_GPEXIIR_SCOM                                    ,
                ULL(0x00064024), SH_UNT_PU_GPE2  , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_GPE3_GPEXIIR_OCI                                     ,
                ULL(0xC0030120), SH_UNT_PU_GPE3  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE3_GPEXIIR_SCOM                                    ,
                ULL(0x00066024), SH_UNT_PU_GPE3  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE0_GPEXISPRG0_OCI                                  ,
                ULL(0xC0000110), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE0_GPEXISPRG0_SCOM                                 ,
                ULL(0x00060022), SH_UNT_PU_GPE0  , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_GPE1_GPEXISPRG0_OCI                                  ,
                ULL(0xC0010110), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_GPEXISPRG0_SCOM                                 ,
                ULL(0x00062022), SH_UNT_PU_GPE1  , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_GPE2_GPEXISPRG0_OCI                                  ,
                ULL(0xC0020110), SH_UNT_PU_GPE2  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE2_GPEXISPRG0_SCOM                                 ,
                ULL(0x00064022), SH_UNT_PU_GPE2  , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_GPE3_GPEXISPRG0_OCI                                  ,
                ULL(0xC0030110), SH_UNT_PU_GPE3  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE3_GPEXISPRG0_SCOM                                 ,
                ULL(0x00066022), SH_UNT_PU_GPE3  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE0_GPEXIXCR_OCI                                    ,
                ULL(0xC0000100), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE0_GPEXIXCR_SCOM                                   ,
                ULL(0x00060020), SH_UNT_PU_GPE0  , SH_ACS_SCOM_WO   );
CONST_UINT64_T( PU_GPE1_GPEXIXCR_OCI                                    ,
                ULL(0xC0010100), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_GPEXIXCR_SCOM                                   ,
                ULL(0x00062020), SH_UNT_PU_GPE1  , SH_ACS_SCOM_WO   );
CONST_UINT64_T( PU_GPE2_GPEXIXCR_OCI                                    ,
                ULL(0xC0020100), SH_UNT_PU_GPE2  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE2_GPEXIXCR_SCOM                                   ,
                ULL(0x00064020), SH_UNT_PU_GPE2  , SH_ACS_SCOM_WO   );
CONST_UINT64_T( PU_GPE3_GPEXIXCR_OCI                                    ,
                ULL(0xC0030100), SH_UNT_PU_GPE3  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE3_GPEXIXCR_SCOM                                   ,
                ULL(0x00066020), SH_UNT_PU_GPE3  , SH_ACS_SCOM_WO   );

CONST_UINT64_T( PU_GPE0_GPEXIXSR_OCI                                    ,
                ULL(0xC0000108), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE0_GPEXIXSR_SCOM                                   ,
                ULL(0x00060021), SH_UNT_PU_GPE0  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE1_GPEXIXSR_OCI                                    ,
                ULL(0xC0010108), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_GPEXIXSR_SCOM                                   ,
                ULL(0x00062021), SH_UNT_PU_GPE1  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE2_GPEXIXSR_OCI                                    ,
                ULL(0xC0020108), SH_UNT_PU_GPE2  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE2_GPEXIXSR_SCOM                                   ,
                ULL(0x00064021), SH_UNT_PU_GPE2  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE3_GPEXIXSR_OCI                                    ,
                ULL(0xC0030108), SH_UNT_PU_GPE3  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE3_GPEXIXSR_SCOM                                   ,
                ULL(0x00066021), SH_UNT_PU_GPE3  , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_NPU0_SM0_GPU0_BAR                                    ,
                ULL(0x0501100B), SH_UNT_PU_NPU0_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM1_GPU0_BAR                                    ,
                ULL(0x0501102B), SH_UNT_PU_NPU0_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM2_GPU0_BAR                                    ,
                ULL(0x0501104B), SH_UNT_PU_NPU0_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM3_GPU0_BAR                                    ,
                ULL(0x0501106B), SH_UNT_PU_NPU0_SM3, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM0_GPU0_BAR                                    ,
                ULL(0x0501110B), SH_UNT_PU_NPU1_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM1_GPU0_BAR                                    ,
                ULL(0x0501112B), SH_UNT_PU_NPU1_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM2_GPU0_BAR                                    ,
                ULL(0x0501114B), SH_UNT_PU_NPU1_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM3_GPU0_BAR                                    ,
                ULL(0x0501116B), SH_UNT_PU_NPU1_SM3, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM0_GPU0_BAR                                    ,
                ULL(0x0501120B), SH_UNT_PU_NPU2_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM1_GPU0_BAR                                    ,
                ULL(0x0501122B), SH_UNT_PU_NPU2_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM2_GPU0_BAR                                    ,
                ULL(0x0501124B), SH_UNT_PU_NPU2_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM3_GPU0_BAR                                    ,
                ULL(0x0501126B), SH_UNT_PU_NPU2_SM3, SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU0_SM0_GPU1_BAR                                    ,
                ULL(0x0501100C), SH_UNT_PU_NPU0_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM1_GPU1_BAR                                    ,
                ULL(0x0501102C), SH_UNT_PU_NPU0_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM2_GPU1_BAR                                    ,
                ULL(0x0501104C), SH_UNT_PU_NPU0_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM3_GPU1_BAR                                    ,
                ULL(0x0501106C), SH_UNT_PU_NPU0_SM3, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM0_GPU1_BAR                                    ,
                ULL(0x0501110C), SH_UNT_PU_NPU1_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM1_GPU1_BAR                                    ,
                ULL(0x0501112C), SH_UNT_PU_NPU1_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM2_GPU1_BAR                                    ,
                ULL(0x0501114C), SH_UNT_PU_NPU1_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM3_GPU1_BAR                                    ,
                ULL(0x0501116C), SH_UNT_PU_NPU1_SM3, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM0_GPU1_BAR                                    ,
                ULL(0x0501120C), SH_UNT_PU_NPU2_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM1_GPU1_BAR                                    ,
                ULL(0x0501122C), SH_UNT_PU_NPU2_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM2_GPU1_BAR                                    ,
                ULL(0x0501124C), SH_UNT_PU_NPU2_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM3_GPU1_BAR                                    ,
                ULL(0x0501126C), SH_UNT_PU_NPU2_SM3, SH_ACS_SCOM      );

CONST_UINT64_T( PU_GZIP_CONTROL_REG                                     ,
                ULL(0x02011140), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_GZIP_ERRRPT_HOLD_REG                                 ,
                ULL(0x02011152), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_GZIP_HI_PRIOR_RCV_FIFO_ASB                           ,
                ULL(0x020110C8), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_GZIP_HI_PRIOR_RCV_FIFO_BAR                           ,
                ULL(0x020110C2), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_GZIP_HI_PRIOR_RCV_FIFO_CNTL                          ,
                ULL(0x020110C5), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_GZIP_LO_PRIOR_RCV_FIFO_ASB                           ,
                ULL(0x020110D1), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_GZIP_LO_PRIOR_RCV_FIFO_BAR                           ,
                ULL(0x020110CB), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_GZIP_LO_PRIOR_RCV_FIFO_CNTL                          ,
                ULL(0x020110CE), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_GZIP_MAX_BYTE_CNT                                    ,
                ULL(0x0201105B), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_HCA_BAR                                              ,
                ULL(0x0501298A), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_HCA_COUNT_BAR                                        ,
                ULL(0x0501298B), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_HCA_DROP                                             ,
                ULL(0x05012991), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_HCA_FLUSH                                            ,
                ULL(0x05012990), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_HCA_MIRROR_BAR                                       ,
                ULL(0x05012993), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_HCA_MODES                                            ,
                ULL(0x0501298F), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_HCA_REF_BAR                                          ,
                ULL(0x0501298E), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_HCA_RESET                                            ,
                ULL(0x05012992), SH_UNT          , SH_ACS_SCOM_W    );

CONST_UINT64_T( PU_NPU0_SM0_HIGH_WATER                                  ,
                ULL(0x05011012), SH_UNT_PU_NPU0_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM1_HIGH_WATER                                  ,
                ULL(0x05011032), SH_UNT_PU_NPU0_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM2_HIGH_WATER                                  ,
                ULL(0x05011052), SH_UNT_PU_NPU0_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM3_HIGH_WATER                                  ,
                ULL(0x05011072), SH_UNT_PU_NPU0_SM3, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM0_HIGH_WATER                                  ,
                ULL(0x05011112), SH_UNT_PU_NPU1_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM1_HIGH_WATER                                  ,
                ULL(0x05011132), SH_UNT_PU_NPU1_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM2_HIGH_WATER                                  ,
                ULL(0x05011152), SH_UNT_PU_NPU1_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM3_HIGH_WATER                                  ,
                ULL(0x05011172), SH_UNT_PU_NPU1_SM3, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM0_HIGH_WATER                                  ,
                ULL(0x05011212), SH_UNT_PU_NPU2_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM1_HIGH_WATER                                  ,
                ULL(0x05011232), SH_UNT_PU_NPU2_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM2_HIGH_WATER                                  ,
                ULL(0x05011252), SH_UNT_PU_NPU2_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM3_HIGH_WATER                                  ,
                ULL(0x05011272), SH_UNT_PU_NPU2_SM3, SH_ACS_SCOM      );

CONST_UINT64_T( PU_HTM0_HTM_CFG                                         ,
                ULL(0x05012888), SH_UNT_PU_HTM0  , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_HTM1_HTM_CFG                                         ,
                ULL(0x050128C8), SH_UNT_PU_HTM1  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_HTM0_HTM_CTRL                                        ,
                ULL(0x05012885), SH_UNT_PU_HTM0  , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_HTM1_HTM_CTRL                                        ,
                ULL(0x050128C5), SH_UNT_PU_HTM1  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_HTM0_HTM_FILT                                        ,
                ULL(0x05012886), SH_UNT_PU_HTM0  , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_HTM1_HTM_FILT                                        ,
                ULL(0x050128C6), SH_UNT_PU_HTM1  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_HTM0_HTM_FLEX                                        ,
                ULL(0x05012889), SH_UNT_PU_HTM0  , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_HTM1_HTM_FLEX                                        ,
                ULL(0x050128C9), SH_UNT_PU_HTM1  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_HTM0_HTM_LAST                                        ,
                ULL(0x05012883), SH_UNT_PU_HTM0  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_HTM1_HTM_LAST                                        ,
                ULL(0x050128C3), SH_UNT_PU_HTM1  , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_HTM0_HTM_MEM                                         ,
                ULL(0x05012881), SH_UNT_PU_HTM0  , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_HTM1_HTM_MEM                                         ,
                ULL(0x050128C1), SH_UNT_PU_HTM1  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_HTM0_HTM_MODE                                        ,
                ULL(0x05012880), SH_UNT_PU_HTM0  , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_HTM1_HTM_MODE                                        ,
                ULL(0x050128C0), SH_UNT_PU_HTM1  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_HTM0_HTM_STAT                                        ,
                ULL(0x05012882), SH_UNT_PU_HTM0  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_HTM1_HTM_STAT                                        ,
                ULL(0x050128C2), SH_UNT_PU_HTM1  , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_HTM0_HTM_TRIG                                        ,
                ULL(0x05012884), SH_UNT_PU_HTM0  , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_HTM1_HTM_TRIG                                        ,
                ULL(0x050128C4), SH_UNT_PU_HTM1  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_HTM0_HTM_TTYPEFILT                                   ,
                ULL(0x05012887), SH_UNT_PU_HTM0  , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_HTM1_HTM_TTYPEFILT                                   ,
                ULL(0x050128C7), SH_UNT_PU_HTM1  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_I2C_BUSY_REGISTER_B                                  ,
                ULL(0x000A000E), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_I2C_BUSY_REGISTER_C                                  ,
                ULL(0x000A100E), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_I2C_BUSY_REGISTER_D                                  ,
                ULL(0x000A200E), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_I2C_BUSY_REGISTER_E                                  ,
                ULL(0x000A300E), SH_UNT          , SH_ACS_SCOM      );

// ERROR: ALREADY DEFINED !  000A000C, _SCOM ::   CONST_UINT64_T( PU_IMM_RESET_ERR_B                                      , ULL(0x000A000C), SH_UNT          , SH_ACS_SCOM      );

// ERROR: ALREADY DEFINED !  000A100C, _SCOM ::   CONST_UINT64_T( PU_IMM_RESET_ERR_C                                      , ULL(0x000A100C), SH_UNT          , SH_ACS_SCOM      );

// ERROR: ALREADY DEFINED !  000A200C, _SCOM ::   CONST_UINT64_T( PU_IMM_RESET_ERR_D                                      , ULL(0x000A200C), SH_UNT          , SH_ACS_SCOM      );

// ERROR: ALREADY DEFINED !  000A300C, _SCOM ::   CONST_UINT64_T( PU_IMM_RESET_ERR_E                                      , ULL(0x000A300C), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_IMM_RESET_I2C_B                                      ,
                ULL(0x000A000B), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_IMM_RESET_I2C_C                                      ,
                ULL(0x000A100B), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_IMM_RESET_I2C_D                                      ,
                ULL(0x000A200B), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_IMM_RESET_I2C_E                                      ,
                ULL(0x000A300B), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_IMM_RESET_S_SCL_B                                    ,
                ULL(0x000A000F), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_IMM_RESET_S_SCL_C                                    ,
                ULL(0x000A100F), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_IMM_RESET_S_SCL_D                                    ,
                ULL(0x000A200F), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_IMM_RESET_S_SCL_E                                    ,
                ULL(0x000A300F), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_IMM_RESET_S_SDA_B                                    ,
                ULL(0x000A0011), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_IMM_RESET_S_SDA_C                                    ,
                ULL(0x000A1011), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_IMM_RESET_S_SDA_D                                    ,
                ULL(0x000A2011), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_IMM_RESET_S_SDA_E                                    ,
                ULL(0x000A3011), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_IMM_SET_S_SCL_B                                      ,
                ULL(0x000A000D), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_IMM_SET_S_SCL_C                                      ,
                ULL(0x000A100D), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_IMM_SET_S_SCL_D                                      ,
                ULL(0x000A200D), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_IMM_SET_S_SCL_E                                      ,
                ULL(0x000A300D), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_IMM_SET_S_SDA_B                                      ,
                ULL(0x000A0010), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_IMM_SET_S_SDA_C                                      ,
                ULL(0x000A1010), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_IMM_SET_S_SDA_D                                      ,
                ULL(0x000A2010), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_IMM_SET_S_SDA_E                                      ,
                ULL(0x000A3010), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_INTERRUPTS_B                                         ,
                ULL(0x000A000A), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_INTERRUPTS_C                                         ,
                ULL(0x000A100A), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_INTERRUPTS_D                                         ,
                ULL(0x000A200A), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_INTERRUPTS_E                                         ,
                ULL(0x000A300A), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_INTERRUPT_COND_B                                     ,
                ULL(0x000A0009), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_INTERRUPT_COND_C                                     ,
                ULL(0x000A1009), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_INTERRUPT_COND_D                                     ,
                ULL(0x000A2009), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_INTERRUPT_COND_E                                     ,
                ULL(0x000A3009), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_INTERRUPT_MASK_REGISTER_B_WO                         ,
                ULL(0x000A0008), SH_UNT          , SH_ACS_SCOM_WO   );
CONST_UINT64_T( PU_INTERRUPT_MASK_REGISTER_B_OR                         ,
                ULL(0x000A0009), SH_UNT          , SH_ACS_SCOM1_OR  );
CONST_UINT64_T( PU_INTERRUPT_MASK_REGISTER_B_AND                        ,
                ULL(0x000A000A), SH_UNT          , SH_ACS_SCOM2_AND );

CONST_UINT64_T( PU_INTERRUPT_MASK_REGISTER_C_WO                         ,
                ULL(0x000A1008), SH_UNT          , SH_ACS_SCOM_WO   );
CONST_UINT64_T( PU_INTERRUPT_MASK_REGISTER_C_OR                         ,
                ULL(0x000A1009), SH_UNT          , SH_ACS_SCOM1_OR  );
CONST_UINT64_T( PU_INTERRUPT_MASK_REGISTER_C_AND                        ,
                ULL(0x000A100A), SH_UNT          , SH_ACS_SCOM2_AND );

CONST_UINT64_T( PU_INTERRUPT_MASK_REGISTER_D_WO                         ,
                ULL(0x000A2008), SH_UNT          , SH_ACS_SCOM_WO   );
CONST_UINT64_T( PU_INTERRUPT_MASK_REGISTER_D_OR                         ,
                ULL(0x000A2009), SH_UNT          , SH_ACS_SCOM1_OR  );
CONST_UINT64_T( PU_INTERRUPT_MASK_REGISTER_D_AND                        ,
                ULL(0x000A200A), SH_UNT          , SH_ACS_SCOM2_AND );

CONST_UINT64_T( PU_INTERRUPT_MASK_REGISTER_E_WO                         ,
                ULL(0x000A3008), SH_UNT          , SH_ACS_SCOM_WO   );
CONST_UINT64_T( PU_INTERRUPT_MASK_REGISTER_E_OR                         ,
                ULL(0x000A3009), SH_UNT          , SH_ACS_SCOM1_OR  );
CONST_UINT64_T( PU_INTERRUPT_MASK_REGISTER_E_AND                        ,
                ULL(0x000A300A), SH_UNT          , SH_ACS_SCOM2_AND );

CONST_UINT64_T( PU_INTERRUPT_MASK_REGISTER_READ_B                       ,
                ULL(0x000A0008), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_INTERRUPT_MASK_REGISTER_READ_C                       ,
                ULL(0x000A1008), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_INTERRUPT_MASK_REGISTER_READ_D                       ,
                ULL(0x000A2008), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_INTERRUPT_MASK_REGISTER_READ_E                       ,
                ULL(0x000A3008), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_INT_CQ_AIB_CTL                                       ,
                ULL(0x05013422), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_INT_CQ_CFG_LDQ                                       ,
                ULL(0x05013426), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_CQ_CFG_PB_GEN                                    ,
                ULL(0x0501340A), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_INT_CQ_CFG_STQ1                                      ,
                ULL(0x05013424), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_CQ_CFG_STQ2                                      ,
                ULL(0x05013425), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_CQ_CNPM_SEL                                      ,
                ULL(0x0501340F), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_CQ_ERR_INFO0                                     ,
                ULL(0x0501343A), SH_UNT          , SH_ACS_SCOM_WCLRPART );

CONST_UINT64_T( PU_INT_CQ_ERR_INFO1                                     ,
                ULL(0x0501343B), SH_UNT          , SH_ACS_SCOM_WCLRPART );

CONST_UINT64_T( PU_INT_CQ_ERR_INFO2                                     ,
                ULL(0x0501343C), SH_UNT          , SH_ACS_SCOM_WCLRPART );

CONST_UINT64_T( PU_INT_CQ_ERR_INFO3                                     ,
                ULL(0x0501343D), SH_UNT          , SH_ACS_SCOM_WCLRPART );

CONST_UINT64_T( PU_INT_CQ_ERR_RPT_HOLD                                  ,
                ULL(0x05013439), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_INT_CQ_IC_BAR                                        ,
                ULL(0x05013410), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_CQ_MSGSND                                        ,
                ULL(0x0501340B), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_CQ_PBI_CTL                                       ,
                ULL(0x05013420), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_INT_CQ_PBO_CTL                                       ,
                ULL(0x05013421), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_INT_CQ_PC_BAR                                        ,
                ULL(0x05013416), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_CQ_PC_BARM                                       ,
                ULL(0x05013417), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_CQ_PMC_0                                         ,
                ULL(0x05013428), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_INT_CQ_PMC_1                                         ,
                ULL(0x05013429), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_INT_CQ_PMC_2                                         ,
                ULL(0x0501342A), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_INT_CQ_PMC_3                                         ,
                ULL(0x0501342B), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_INT_CQ_PMC_4                                         ,
                ULL(0x0501342C), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_INT_CQ_PMC_5                                         ,
                ULL(0x0501342D), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_INT_CQ_PMC_6                                         ,
                ULL(0x0501342E), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_INT_CQ_PMC_7                                         ,
                ULL(0x0501342F), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_INT_CQ_PM_CTL                                        ,
                ULL(0x05013427), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_INT_CQ_RST_CTL                                       ,
                ULL(0x05013423), SH_UNT          , SH_ACS_SCOM_WO   );

CONST_UINT64_T( PU_INT_CQ_SWI_RSP                                       ,
                ULL(0x05013409), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_INT_CQ_TM1_BAR                                       ,
                ULL(0x05013412), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_CQ_TM2_BAR                                       ,
                ULL(0x05013414), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_CQ_VC_BAR                                        ,
                ULL(0x05013418), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_CQ_VC_BARM                                       ,
                ULL(0x05013419), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_PC_AIB_MAX_CMD_CRD_REG                           ,
                ULL(0x05013529), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_PC_AIB_MAX_DAT_CRD_REG                           ,
                ULL(0x0501352A), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_PC_CRD_INIT_TIMER                                ,
                ULL(0x05013528), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_PC_GEN_CFG                                       ,
                ULL(0x0501352B), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_PC_INDIR0_REG                                    ,
                ULL(0x05013503), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_PC_INDIR1_REG                                    ,
                ULL(0x05013504), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_PC_INDIR2_REG                                    ,
                ULL(0x05013505), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_PC_INDIR3_REG                                    ,
                ULL(0x05013506), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_PC_MMIO_ARB_CFG                                  ,
                ULL(0x05013525), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_PC_PCMD_ARB_CFG                                  ,
                ULL(0x05013520), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_PC_REGS_ERR_CFG_REG0                             ,
                ULL(0x05013530), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_PC_REGS_ERR_CFG_REG1                             ,
                ULL(0x05013531), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_PC_REGS_FATAL_ERR                                ,
                ULL(0x05013533), SH_UNT          , SH_ACS_SCOM_CLRPART );

CONST_UINT64_T( PU_INT_PC_REGS_INFO_ERR                                 ,
                ULL(0x05013534), SH_UNT          , SH_ACS_SCOM_CLRPART );

CONST_UINT64_T( PU_INT_PC_REGS_RECOV_ERR                                ,
                ULL(0x05013535), SH_UNT          , SH_ACS_SCOM_CLRPART );

CONST_UINT64_T( PU_INT_PC_REGS_WOF_ERR                                  ,
                ULL(0x05013532), SH_UNT          , SH_ACS_SCOM_CLRPART );

CONST_UINT64_T( PU_INT_PC_REGS_WOF_ERR_DETAIL                           ,
                ULL(0x05013536), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_INT_PC_TCTXT_CFG_REG                                 ,
                ULL(0x05013500), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_PC_THRD_EN_REG0                                  ,
                ULL(0x05013501), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_PC_THRD_EN_REG1                                  ,
                ULL(0x05013502), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_PC_VRQ_ARB_CFG                                   ,
                ULL(0x05013523), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_PC_VRQ_CRD_CFG                                   ,
                ULL(0x05013524), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_PC_VRQ_PEND_CFG                                  ,
                ULL(0x05013522), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_PC_VRQ_QUE_CFG                                   ,
                ULL(0x05013521), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_PC_VSD_TABLE_ADDR                                ,
                ULL(0x0501352C), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_AIB_TIMEOUT                                   ,
                ULL(0x0501362B), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_AIB_TX_CMD_PRIORITY                           ,
                ULL(0x0501363D), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_AIB_TX_ORDERING_TAG_1                         ,
                ULL(0x0501362C), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_AIB_TX_ORDERING_TAG_2                         ,
                ULL(0x0501362D), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_ATX_INIT_CREDIT_COUNT                         ,
                ULL(0x0501363C), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_ATX_PERF_EVENT_SEL_1                          ,
                ULL(0x05013640), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_ATX_PERF_EVENT_SEL_2                          ,
                ULL(0x05013641), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_ATX_PERF_EVENT_SEL_3                          ,
                ULL(0x05013642), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_AT_MACRO_KILL                                 ,
                ULL(0x0501363E), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_AT_MACRO_KILL_MASK                            ,
                ULL(0x0501363F), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_EQC_ADDITIONAL_PERF_1                         ,
                ULL(0x05013653), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_INT_VC_EQC_ADDITIONAL_PERF_2                         ,
                ULL(0x05013654), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_INT_VC_EQC_CACHE_EN                                  ,
                ULL(0x05013611), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_EQC_CACHE_WATCH_DATA0                         ,
                ULL(0x05013616), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_EQC_CACHE_WATCH_DATA1                         ,
                ULL(0x05013617), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_EQC_CACHE_WATCH_DATA2                         ,
                ULL(0x05013618), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_EQC_CACHE_WATCH_DATA3                         ,
                ULL(0x05013619), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_EQC_CACHE_WATCH_SPEC                          ,
                ULL(0x05013615), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_INT_VC_EQC_CONFIG                                    ,
                ULL(0x05013614), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_EQC_DEBUG                                     ,
                ULL(0x0501361A), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_EQC_PERF_EVENT_SEL_1                          ,
                ULL(0x05013650), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_EQC_PERF_EVENT_SEL_2                          ,
                ULL(0x05013651), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_EQC_PERF_EVENT_SEL_3                          ,
                ULL(0x05013652), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_EQC_SCRUB_MASK                                ,
                ULL(0x05013613), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_EQC_SCRUB_TRIG                                ,
                ULL(0x05013612), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_EQD_BLOCK_MODE                                ,
                ULL(0x05013604), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_ERR_CFG_G0R0                                  ,
                ULL(0x05013670), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_ERR_CFG_G0R1                                  ,
                ULL(0x05013671), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_ERR_CFG_G1R0                                  ,
                ULL(0x05013672), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_ERR_CFG_G1R1                                  ,
                ULL(0x05013673), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_FATAL_ERR_G0                                  ,
                ULL(0x05013678), SH_UNT          , SH_ACS_SCOM_CLRPART );

CONST_UINT64_T( PU_INT_VC_FATAL_ERR_G1                                  ,
                ULL(0x0501367B), SH_UNT          , SH_ACS_SCOM_CLRPART );

CONST_UINT64_T( PU_INT_VC_GLOBAL_CONFIG                                 ,
                ULL(0x05013600), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_INFO_ERR_G0                                   ,
                ULL(0x0501367A), SH_UNT          , SH_ACS_SCOM_CLRPART );

CONST_UINT64_T( PU_INT_VC_INFO_ERR_G1                                   ,
                ULL(0x0501367D), SH_UNT          , SH_ACS_SCOM_CLRPART );

CONST_UINT64_T( PU_INT_VC_IRQ_CONFIG_0                                  ,
                ULL(0x05013608), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_INT_VC_IRQ_CONFIG_1                                  ,
                ULL(0x05013609), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_INT_VC_IRQ_CONFIG_2                                  ,
                ULL(0x0501360A), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_INT_VC_IRQ_CONFIG_3                                  ,
                ULL(0x0501360B), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_INT_VC_IRQ_CONFIG_4                                  ,
                ULL(0x0501360C), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_INT_VC_IRQ_CONFIG_5                                  ,
                ULL(0x0501360D), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_INT_VC_IRQ_PERF_EVENT_SEL_0                          ,
                ULL(0x05013668), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_IRQ_PERF_EVENT_SEL_1                          ,
                ULL(0x05013669), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_IRQ_PERF_EVENT_SEL_2                          ,
                ULL(0x0501366A), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_IRQ_PERF_EVENT_SEL_3                          ,
                ULL(0x0501366B), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_IRQ_PERF_EVENT_SEL_4                          ,
                ULL(0x0501366C), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_IRQ_PERF_EVENT_SEL_5                          ,
                ULL(0x0501366D), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_IRQ_TO_EQC_CREDITS                            ,
                ULL(0x0501360E), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_IVC_ADDITIONAL_PERF                           ,
                ULL(0x0501365B), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_INT_VC_IVC_CACHE_EN                                  ,
                ULL(0x05013621), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_IVC_CACHE_WATCH_ADDR                          ,
                ULL(0x05013625), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_IVC_CACHE_WATCH_DATA                          ,
                ULL(0x05013626), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_INT_VC_IVC_DEBUG                                     ,
                ULL(0x0501362A), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_IVC_HASH_1                                    ,
                ULL(0x05013627), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_IVC_HASH_2                                    ,
                ULL(0x05013628), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_IVC_HASH_3                                    ,
                ULL(0x05013629), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_IVC_PERF_EVENT_SEL_1                          ,
                ULL(0x05013658), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_IVC_PERF_EVENT_SEL_2                          ,
                ULL(0x05013659), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_IVC_PERF_EVENT_SEL_3                          ,
                ULL(0x0501365A), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_IVC_SCRUB_MASK                                ,
                ULL(0x05013623), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_IVC_SCRUB_TRIG                                ,
                ULL(0x05013622), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_IVE_ISB_BLOCK_MODE                            ,
                ULL(0x05013603), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_LBS6_DEBUG                                    ,
                ULL(0x05013606), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_MAX_OUTSTANDING_EQC_OUTB_CMD                  ,
                ULL(0x05013610), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_MAX_OUTSTANDING_IRQ_DMA                       ,
                ULL(0x0501363B), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_MAX_OUTSTANDING_IVC_CMD                       ,
                ULL(0x05013620), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_MAX_OUTSTANDING_SBC_CMD                       ,
                ULL(0x05013630), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_RECOV_ERR_G0                                  ,
                ULL(0x05013679), SH_UNT          , SH_ACS_SCOM_CLRPART );

CONST_UINT64_T( PU_INT_VC_RECOV_ERR_G1                                  ,
                ULL(0x0501367C), SH_UNT          , SH_ACS_SCOM_CLRPART );

CONST_UINT64_T( PU_INT_VC_SBC_ADDITIONAL_PERF                           ,
                ULL(0x05013663), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_INT_VC_SBC_CACHE_EN                                  ,
                ULL(0x05013631), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_SBC_CACHE_WATCH_ADDR                          ,
                ULL(0x05013635), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_SBC_CACHE_WATCH_DATA                          ,
                ULL(0x05013636), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_INT_VC_SBC_CONFIG                                    ,
                ULL(0x05013634), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_SBC_DEBUG                                     ,
                ULL(0x0501363A), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_SBC_PERF_EVENT_SEL_1                          ,
                ULL(0x05013660), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_SBC_PERF_EVENT_SEL_2                          ,
                ULL(0x05013661), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_SBC_PERF_EVENT_SEL_3                          ,
                ULL(0x05013662), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_SBC_SCRUB_MASK                                ,
                ULL(0x05013633), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_SBC_SCRUB_TRIG                                ,
                ULL(0x05013632), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_VPS_BLOCK_MODE                                ,
                ULL(0x05013605), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_VSD_TABLE_ADDR                                ,
                ULL(0x05013601), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_INT_VC_WOF_ERR_G0                                    ,
                ULL(0x05013674), SH_UNT          , SH_ACS_SCOM_CLRPART );

CONST_UINT64_T( PU_INT_VC_WOF_ERR_G0_DETAIL                             ,
                ULL(0x05013675), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_INT_VC_WOF_ERR_G1                                    ,
                ULL(0x05013676), SH_UNT          , SH_ACS_SCOM_CLRPART );

CONST_UINT64_T( PU_INT_VC_WOF_ERR_G1_DETAIL                             ,
                ULL(0x05013677), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_NPU_SM1_IODA_ADDR                                    ,
                ULL(0x05011321), SH_UNT_PU_NPU_SM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU_SM1_IODA_DAT0                                    ,
                ULL(0x05011322), SH_UNT_PU_NPU_SM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_IO_DATA_REG                                          ,
                ULL(0x05022830), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_IVT_OFFSET                                           ,
                ULL(0x05012918), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_GPE6_JTG_PIB_OJCFG                                   ,
                ULL(0x0006D004), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_GPE6_JTG_PIB_OJCFG_CLEAR                             ,
                ULL(0x0006D005), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_CLEAR );
CONST_UINT64_T( PU_GPE6_JTG_PIB_OJCFG_OR                                ,
                ULL(0x0006D006), SH_UNT_PU_GPE6  , SH_ACS_SCOM2_OR  );

CONST_UINT64_T( PU_GPE6_JTG_PIB_OJFRST                                  ,
                ULL(0x0006D007), SH_UNT_PU_GPE6  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_GPE6_JTG_PIB_OJIC_SCOM                               ,
                ULL(0x0006D008), SH_UNT_PU_GPE6  , SH_ACS_SCOM      );
CONST_UINT64_T( PU_GPE6_JTG_PIB_OJIC_SCOM1                              ,
                ULL(0x0006D009), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_CLEAR );
CONST_UINT64_T( PU_GPE6_JTG_PIB_OJIC_SCOM2                              ,
                ULL(0x0006D00A), SH_UNT_PU_GPE6  , SH_ACS_SCOM2_OR  );

CONST_UINT64_T( PU_GPE6_JTG_PIB_OJSTAT                                  ,
                ULL(0x0006D00B), SH_UNT_PU_GPE6  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_GPE6_JTG_PIB_OJTDI                                   ,
                ULL(0x0006D00C), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE6_JTG_PIB_OJTDO                                   ,
                ULL(0x0006D00D), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_NPU_CTL_LCO_CONFIG                                   ,
                ULL(0x05011382), SH_UNT_PU_NPU_CTL, SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU0_NTL0_LOW_PWR                                    ,
                ULL(0x050110C2), SH_UNT_PU_NPU0_NTL0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_NTL1_LOW_PWR                                    ,
                ULL(0x050110E2), SH_UNT_PU_NPU0_NTL1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_NTL0_LOW_PWR                                    ,
                ULL(0x050111C2), SH_UNT_PU_NPU1_NTL0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_NTL1_LOW_PWR                                    ,
                ULL(0x050111E2), SH_UNT_PU_NPU1_NTL1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_NTL0_LOW_PWR                                    ,
                ULL(0x050112C2), SH_UNT_PU_NPU2_NTL0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_NTL1_LOW_PWR                                    ,
                ULL(0x050112E2), SH_UNT_PU_NPU2_NTL1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU0_SM0_LOW_WATER                                   ,
                ULL(0x05011011), SH_UNT_PU_NPU0_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM1_LOW_WATER                                   ,
                ULL(0x05011031), SH_UNT_PU_NPU0_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM2_LOW_WATER                                   ,
                ULL(0x05011051), SH_UNT_PU_NPU0_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM3_LOW_WATER                                   ,
                ULL(0x05011071), SH_UNT_PU_NPU0_SM3, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM0_LOW_WATER                                   ,
                ULL(0x05011111), SH_UNT_PU_NPU1_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM1_LOW_WATER                                   ,
                ULL(0x05011131), SH_UNT_PU_NPU1_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM2_LOW_WATER                                   ,
                ULL(0x05011151), SH_UNT_PU_NPU1_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM3_LOW_WATER                                   ,
                ULL(0x05011171), SH_UNT_PU_NPU1_SM3, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM0_LOW_WATER                                   ,
                ULL(0x05011211), SH_UNT_PU_NPU2_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM1_LOW_WATER                                   ,
                ULL(0x05011231), SH_UNT_PU_NPU2_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM2_LOW_WATER                                   ,
                ULL(0x05011251), SH_UNT_PU_NPU2_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM3_LOW_WATER                                   ,
                ULL(0x05011271), SH_UNT_PU_NPU2_SM3, SH_ACS_SCOM      );

CONST_UINT64_T( PU_LPC_BASE_REG                                         ,
                ULL(0x05022840), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_LPC_CMD_REG                                          ,
                ULL(0x05022841), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_LPC_DATA_REG                                         ,
                ULL(0x05022842), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PEC_0_STACK0_LSIBAR_REG                                 ,
                ULL(0x04010C54), SH_UNT_PEC_0_STACK0, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_0_STACK1_LSIBAR_REG                                 ,
                ULL(0x04010C94), SH_UNT_PEC_0_STACK1, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_0_STACK2_LSIBAR_REG                                 ,
                ULL(0x04010CD4), SH_UNT_PEC_0_STACK2, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_1_STACK0_LSIBAR_REG                                 ,
                ULL(0x04011054), SH_UNT_PEC_1_STACK0, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_1_STACK1_LSIBAR_REG                                 ,
                ULL(0x04011094), SH_UNT_PEC_1_STACK1, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_1_STACK2_LSIBAR_REG                                 ,
                ULL(0x040110D4), SH_UNT_PEC_1_STACK2, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_2_STACK0_LSIBAR_REG                                 ,
                ULL(0x04011454), SH_UNT_PEC_2_STACK0, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_2_STACK1_LSIBAR_REG                                 ,
                ULL(0x04011494), SH_UNT_PEC_2_STACK1, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_2_STACK2_LSIBAR_REG                                 ,
                ULL(0x040114D4), SH_UNT_PEC_2_STACK2, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_STACK0_LSIBAR_REG                                   ,
                ULL(0x04010C54), SH_UNT_PEC_STACK0, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_STACK1_LSIBAR_REG                                   ,
                ULL(0x04010C94), SH_UNT_PEC_STACK1, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_STACK2_LSIBAR_REG                                   ,
                ULL(0x04010CD4), SH_UNT_PEC_STACK2, SH_ACS_SCOM      );

CONST_UINT64_T( PU_PIB2OPB0_LSTAT                                       ,
                ULL(0x00020002), SH_UNT_PU_PIB2OPB0, SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_PIB2OPB1_LSTAT                                       ,
                ULL(0x00020012), SH_UNT_PU_PIB2OPB1, SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_NPU0_NTL0_MASK1                                      ,
                ULL(0x050110C5), SH_UNT_PU_NPU0_NTL0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_NTL1_MASK1                                      ,
                ULL(0x050110E5), SH_UNT_PU_NPU0_NTL1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM0_MASK1                                       ,
                ULL(0x05011013), SH_UNT_PU_NPU0_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM1_MASK1                                       ,
                ULL(0x05011033), SH_UNT_PU_NPU0_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM2_MASK1                                       ,
                ULL(0x05011053), SH_UNT_PU_NPU0_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM3_MASK1                                       ,
                ULL(0x05011073), SH_UNT_PU_NPU0_SM3, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_NTL0_MASK1                                      ,
                ULL(0x050111C5), SH_UNT_PU_NPU1_NTL0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_NTL1_MASK1                                      ,
                ULL(0x050111E5), SH_UNT_PU_NPU1_NTL1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM0_MASK1                                       ,
                ULL(0x05011113), SH_UNT_PU_NPU1_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM1_MASK1                                       ,
                ULL(0x05011133), SH_UNT_PU_NPU1_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM2_MASK1                                       ,
                ULL(0x05011153), SH_UNT_PU_NPU1_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM3_MASK1                                       ,
                ULL(0x05011173), SH_UNT_PU_NPU1_SM3, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_NTL0_MASK1                                      ,
                ULL(0x050112C5), SH_UNT_PU_NPU2_NTL0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_NTL1_MASK1                                      ,
                ULL(0x050112E5), SH_UNT_PU_NPU2_NTL1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM0_MASK1                                       ,
                ULL(0x05011213), SH_UNT_PU_NPU2_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM1_MASK1                                       ,
                ULL(0x05011233), SH_UNT_PU_NPU2_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM2_MASK1                                       ,
                ULL(0x05011253), SH_UNT_PU_NPU2_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM3_MASK1                                       ,
                ULL(0x05011273), SH_UNT_PU_NPU2_SM3, SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU0_CTL_MASK2                                       ,
                ULL(0x05011095), SH_UNT_PU_NPU0_CTL, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_CTL_MASK2                                       ,
                ULL(0x05011195), SH_UNT_PU_NPU1_CTL, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_CTL_MASK2                                       ,
                ULL(0x05011295), SH_UNT_PU_NPU2_CTL, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_NTL0_MASK2                                      ,
                ULL(0x050110C7), SH_UNT_PU_NPU0_NTL0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_NTL1_MASK2                                      ,
                ULL(0x050110E7), SH_UNT_PU_NPU0_NTL1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM0_MASK2                                       ,
                ULL(0x05011015), SH_UNT_PU_NPU0_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM1_MASK2                                       ,
                ULL(0x05011035), SH_UNT_PU_NPU0_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM2_MASK2                                       ,
                ULL(0x05011055), SH_UNT_PU_NPU0_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM3_MASK2                                       ,
                ULL(0x05011075), SH_UNT_PU_NPU0_SM3, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_NTL0_MASK2                                      ,
                ULL(0x050111C7), SH_UNT_PU_NPU1_NTL0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_NTL1_MASK2                                      ,
                ULL(0x050111E7), SH_UNT_PU_NPU1_NTL1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM0_MASK2                                       ,
                ULL(0x05011115), SH_UNT_PU_NPU1_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM1_MASK2                                       ,
                ULL(0x05011135), SH_UNT_PU_NPU1_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM2_MASK2                                       ,
                ULL(0x05011155), SH_UNT_PU_NPU1_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM3_MASK2                                       ,
                ULL(0x05011175), SH_UNT_PU_NPU1_SM3, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_NTL0_MASK2                                      ,
                ULL(0x050112C7), SH_UNT_PU_NPU2_NTL0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_NTL1_MASK2                                      ,
                ULL(0x050112E7), SH_UNT_PU_NPU2_NTL1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM0_MASK2                                       ,
                ULL(0x05011215), SH_UNT_PU_NPU2_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM1_MASK2                                       ,
                ULL(0x05011235), SH_UNT_PU_NPU2_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM2_MASK2                                       ,
                ULL(0x05011255), SH_UNT_PU_NPU2_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM3_MASK2                                       ,
                ULL(0x05011275), SH_UNT_PU_NPU2_SM3, SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU0_SM0_MAX_BAR                                     ,
                ULL(0x0501100F), SH_UNT_PU_NPU0_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM1_MAX_BAR                                     ,
                ULL(0x0501102F), SH_UNT_PU_NPU0_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM2_MAX_BAR                                     ,
                ULL(0x0501104F), SH_UNT_PU_NPU0_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM3_MAX_BAR                                     ,
                ULL(0x0501106F), SH_UNT_PU_NPU0_SM3, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM0_MAX_BAR                                     ,
                ULL(0x0501110F), SH_UNT_PU_NPU1_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM1_MAX_BAR                                     ,
                ULL(0x0501112F), SH_UNT_PU_NPU1_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM2_MAX_BAR                                     ,
                ULL(0x0501114F), SH_UNT_PU_NPU1_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM3_MAX_BAR                                     ,
                ULL(0x0501116F), SH_UNT_PU_NPU1_SM3, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM0_MAX_BAR                                     ,
                ULL(0x0501120F), SH_UNT_PU_NPU2_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM1_MAX_BAR                                     ,
                ULL(0x0501122F), SH_UNT_PU_NPU2_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM2_MAX_BAR                                     ,
                ULL(0x0501124F), SH_UNT_PU_NPU2_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM3_MAX_BAR                                     ,
                ULL(0x0501126F), SH_UNT_PU_NPU2_SM3, SH_ACS_SCOM      );

CONST_UINT64_T( PU_MCD_DBG                                              ,
                ULL(0x03011413), SH_UNT          , SH_ACS_SCOM      );
CONST_UINT64_T( PU_MCD1_MCD_DBG                                         ,
                ULL(0x03011013), SH_UNT_PU_MCD1  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_MCD_ECAP                                             ,
                ULL(0x03011412), SH_UNT          , SH_ACS_SCOM      );
CONST_UINT64_T( PU_MCD1_MCD_ECAP                                        ,
                ULL(0x03011012), SH_UNT_PU_MCD1  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_GPE0_MIB_XIDCAC_OCI                                  ,
                ULL(0xC00000D0), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE0_MIB_XIDCAC_SCOM                                 ,
                ULL(0x0006001A), SH_UNT_PU_GPE0  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE1_MIB_XIDCAC_OCI                                  ,
                ULL(0xC00100D0), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_MIB_XIDCAC_SCOM                                 ,
                ULL(0x0006201A), SH_UNT_PU_GPE1  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE2_MIB_XIDCAC_OCI                                  ,
                ULL(0xC00200D0), SH_UNT_PU_GPE2  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE2_MIB_XIDCAC_SCOM                                 ,
                ULL(0x0006401A), SH_UNT_PU_GPE2  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE3_MIB_XIDCAC_OCI                                  ,
                ULL(0xC00300D0), SH_UNT_PU_GPE3  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE3_MIB_XIDCAC_SCOM                                 ,
                ULL(0x0006601A), SH_UNT_PU_GPE3  , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_MIB_XIICAC                                           ,
                ULL(0x000E0009), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_GPE0_MIB_XIICAC                                      ,
                ULL(0x00060019), SH_UNT_PU_GPE0  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE1_MIB_XIICAC                                      ,
                ULL(0x00062019), SH_UNT_PU_GPE1  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE2_MIB_XIICAC                                      ,
                ULL(0x00064019), SH_UNT_PU_GPE2  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE3_MIB_XIICAC                                      ,
                ULL(0x00066019), SH_UNT_PU_GPE3  , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_MIB_XIMEM                                            ,
                ULL(0x000E0007), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_GPE0_MIB_XIMEM                                       ,
                ULL(0x00060017), SH_UNT_PU_GPE0  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE1_MIB_XIMEM                                       ,
                ULL(0x00062017), SH_UNT_PU_GPE1  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE2_MIB_XIMEM                                       ,
                ULL(0x00064017), SH_UNT_PU_GPE2  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE3_MIB_XIMEM                                       ,
                ULL(0x00066017), SH_UNT_PU_GPE3  , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_MIB_XISGB                                            ,
                ULL(0x000E0008), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_GPE0_MIB_XISGB                                       ,
                ULL(0x00060018), SH_UNT_PU_GPE0  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE1_MIB_XISGB                                       ,
                ULL(0x00062018), SH_UNT_PU_GPE1  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE2_MIB_XISGB                                       ,
                ULL(0x00064018), SH_UNT_PU_GPE2  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE3_MIB_XISGB                                       ,
                ULL(0x00066018), SH_UNT_PU_GPE3  , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_MIB_XISIB                                            ,
                ULL(0x000E0006), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_GPE0_MIB_XISIB                                       ,
                ULL(0x00060016), SH_UNT_PU_GPE0  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE1_MIB_XISIB                                       ,
                ULL(0x00062016), SH_UNT_PU_GPE1  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE2_MIB_XISIB                                       ,
                ULL(0x00064016), SH_UNT_PU_GPE2  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE3_MIB_XISIB                                       ,
                ULL(0x00066016), SH_UNT_PU_GPE3  , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PEC_0_STACK0_MMIOBAR0_MASK_REG                          ,
                ULL(0x04010C4F), SH_UNT_PEC_0_STACK0, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_0_STACK1_MMIOBAR0_MASK_REG                          ,
                ULL(0x04010C8F), SH_UNT_PEC_0_STACK1, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_0_STACK2_MMIOBAR0_MASK_REG                          ,
                ULL(0x04010CCF), SH_UNT_PEC_0_STACK2, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_1_STACK0_MMIOBAR0_MASK_REG                          ,
                ULL(0x0401104F), SH_UNT_PEC_1_STACK0, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_1_STACK1_MMIOBAR0_MASK_REG                          ,
                ULL(0x0401108F), SH_UNT_PEC_1_STACK1, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_1_STACK2_MMIOBAR0_MASK_REG                          ,
                ULL(0x040110CF), SH_UNT_PEC_1_STACK2, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_2_STACK0_MMIOBAR0_MASK_REG                          ,
                ULL(0x0401144F), SH_UNT_PEC_2_STACK0, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_2_STACK1_MMIOBAR0_MASK_REG                          ,
                ULL(0x0401148F), SH_UNT_PEC_2_STACK1, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_2_STACK2_MMIOBAR0_MASK_REG                          ,
                ULL(0x040114CF), SH_UNT_PEC_2_STACK2, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_STACK0_MMIOBAR0_MASK_REG                            ,
                ULL(0x04010C4F), SH_UNT_PEC_STACK0, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_STACK1_MMIOBAR0_MASK_REG                            ,
                ULL(0x04010C8F), SH_UNT_PEC_STACK1, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_STACK2_MMIOBAR0_MASK_REG                            ,
                ULL(0x04010CCF), SH_UNT_PEC_STACK2, SH_ACS_SCOM      );

CONST_UINT64_T( PEC_0_STACK0_MMIOBAR0_REG                               ,
                ULL(0x04010C4E), SH_UNT_PEC_0_STACK0, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_0_STACK1_MMIOBAR0_REG                               ,
                ULL(0x04010C8E), SH_UNT_PEC_0_STACK1, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_0_STACK2_MMIOBAR0_REG                               ,
                ULL(0x04010CCE), SH_UNT_PEC_0_STACK2, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_1_STACK0_MMIOBAR0_REG                               ,
                ULL(0x0401104E), SH_UNT_PEC_1_STACK0, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_1_STACK1_MMIOBAR0_REG                               ,
                ULL(0x0401108E), SH_UNT_PEC_1_STACK1, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_1_STACK2_MMIOBAR0_REG                               ,
                ULL(0x040110CE), SH_UNT_PEC_1_STACK2, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_2_STACK0_MMIOBAR0_REG                               ,
                ULL(0x0401144E), SH_UNT_PEC_2_STACK0, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_2_STACK1_MMIOBAR0_REG                               ,
                ULL(0x0401148E), SH_UNT_PEC_2_STACK1, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_2_STACK2_MMIOBAR0_REG                               ,
                ULL(0x040114CE), SH_UNT_PEC_2_STACK2, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_STACK0_MMIOBAR0_REG                                 ,
                ULL(0x04010C4E), SH_UNT_PEC_STACK0, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_STACK1_MMIOBAR0_REG                                 ,
                ULL(0x04010C8E), SH_UNT_PEC_STACK1, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_STACK2_MMIOBAR0_REG                                 ,
                ULL(0x04010CCE), SH_UNT_PEC_STACK2, SH_ACS_SCOM      );

CONST_UINT64_T( PEC_0_STACK0_MMIOBAR1_MASK_REG                          ,
                ULL(0x04010C51), SH_UNT_PEC_0_STACK0, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_0_STACK1_MMIOBAR1_MASK_REG                          ,
                ULL(0x04010C91), SH_UNT_PEC_0_STACK1, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_0_STACK2_MMIOBAR1_MASK_REG                          ,
                ULL(0x04010CD1), SH_UNT_PEC_0_STACK2, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_1_STACK0_MMIOBAR1_MASK_REG                          ,
                ULL(0x04011051), SH_UNT_PEC_1_STACK0, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_1_STACK1_MMIOBAR1_MASK_REG                          ,
                ULL(0x04011091), SH_UNT_PEC_1_STACK1, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_1_STACK2_MMIOBAR1_MASK_REG                          ,
                ULL(0x040110D1), SH_UNT_PEC_1_STACK2, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_2_STACK0_MMIOBAR1_MASK_REG                          ,
                ULL(0x04011451), SH_UNT_PEC_2_STACK0, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_2_STACK1_MMIOBAR1_MASK_REG                          ,
                ULL(0x04011491), SH_UNT_PEC_2_STACK1, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_2_STACK2_MMIOBAR1_MASK_REG                          ,
                ULL(0x040114D1), SH_UNT_PEC_2_STACK2, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_STACK0_MMIOBAR1_MASK_REG                            ,
                ULL(0x04010C51), SH_UNT_PEC_STACK0, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_STACK1_MMIOBAR1_MASK_REG                            ,
                ULL(0x04010C91), SH_UNT_PEC_STACK1, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_STACK2_MMIOBAR1_MASK_REG                            ,
                ULL(0x04010CD1), SH_UNT_PEC_STACK2, SH_ACS_SCOM      );

CONST_UINT64_T( PEC_0_STACK0_MMIOBAR1_REG                               ,
                ULL(0x04010C50), SH_UNT_PEC_0_STACK0, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_0_STACK1_MMIOBAR1_REG                               ,
                ULL(0x04010C90), SH_UNT_PEC_0_STACK1, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_0_STACK2_MMIOBAR1_REG                               ,
                ULL(0x04010CD0), SH_UNT_PEC_0_STACK2, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_1_STACK0_MMIOBAR1_REG                               ,
                ULL(0x04011050), SH_UNT_PEC_1_STACK0, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_1_STACK1_MMIOBAR1_REG                               ,
                ULL(0x04011090), SH_UNT_PEC_1_STACK1, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_1_STACK2_MMIOBAR1_REG                               ,
                ULL(0x040110D0), SH_UNT_PEC_1_STACK2, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_2_STACK0_MMIOBAR1_REG                               ,
                ULL(0x04011450), SH_UNT_PEC_2_STACK0, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_2_STACK1_MMIOBAR1_REG                               ,
                ULL(0x04011490), SH_UNT_PEC_2_STACK1, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_2_STACK2_MMIOBAR1_REG                               ,
                ULL(0x040114D0), SH_UNT_PEC_2_STACK2, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_STACK0_MMIOBAR1_REG                                 ,
                ULL(0x04010C50), SH_UNT_PEC_STACK0, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_STACK1_MMIOBAR1_REG                                 ,
                ULL(0x04010C90), SH_UNT_PEC_STACK1, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_STACK2_MMIOBAR1_REG                                 ,
                ULL(0x04010CD0), SH_UNT_PEC_STACK2, SH_ACS_SCOM      );

CONST_UINT64_T( PU_NMMU_MM_CFG_NMMU_CTL_MISC                            ,
                ULL(0x05013049), SH_UNT_PU_NMMU  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NMMU_MM_CFG_NMMU_CTL_SLB                             ,
                ULL(0x0501304A), SH_UNT_PU_NMMU  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NMMU_MM_CFG_NMMU_CTL_SM                              ,
                ULL(0x05013048), SH_UNT_PU_NMMU  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NMMU_MM_CFG_NMMU_CTL_TLB                             ,
                ULL(0x0501304B), SH_UNT_PU_NMMU  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NMMU_MM_CFG_NMMU_XLAT_CTL_REG0                       ,
                ULL(0x05013040), SH_UNT_PU_NMMU  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NMMU_MM_CFG_NMMU_XLAT_CTL_REG1                       ,
                ULL(0x05013041), SH_UNT_PU_NMMU  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NMMU_MM_CFG_NMMU_XLAT_CTL_REG2                       ,
                ULL(0x05013042), SH_UNT_PU_NMMU  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NMMU_MM_NMMU_DBG_MODE                                ,
                ULL(0x0501304F), SH_UNT_PU_NMMU  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NMMU_MM_NMMU_ERR_INJ                                 ,
                ULL(0x0501304E), SH_UNT_PU_NMMU  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NMMU_MM_NMMU_ERR_LOG                                 ,
                ULL(0x0501304D), SH_UNT_PU_NMMU  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NMMU_MM_NMMU_FIR                                     ,
                ULL(0x0501304C), SH_UNT_PU_NMMU  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_MODE_REGISTER                                        ,
                ULL(0x00010008), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_MODE_REGISTER_B                                      ,
                ULL(0x000A0006), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_MODE_REGISTER_C                                      ,
                ULL(0x000A1006), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_MODE_REGISTER_D                                      ,
                ULL(0x000A2006), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_MODE_REGISTER_E                                      ,
                ULL(0x000A3006), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PEC_0_STACK0_MSIBAR_REG                                 ,
                ULL(0x04010C53), SH_UNT_PEC_0_STACK0, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_0_STACK1_MSIBAR_REG                                 ,
                ULL(0x04010C93), SH_UNT_PEC_0_STACK1, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_0_STACK2_MSIBAR_REG                                 ,
                ULL(0x04010CD3), SH_UNT_PEC_0_STACK2, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_1_STACK0_MSIBAR_REG                                 ,
                ULL(0x04011053), SH_UNT_PEC_1_STACK0, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_1_STACK1_MSIBAR_REG                                 ,
                ULL(0x04011093), SH_UNT_PEC_1_STACK1, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_1_STACK2_MSIBAR_REG                                 ,
                ULL(0x040110D3), SH_UNT_PEC_1_STACK2, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_2_STACK0_MSIBAR_REG                                 ,
                ULL(0x04011453), SH_UNT_PEC_2_STACK0, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_2_STACK1_MSIBAR_REG                                 ,
                ULL(0x04011493), SH_UNT_PEC_2_STACK1, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_2_STACK2_MSIBAR_REG                                 ,
                ULL(0x040114D3), SH_UNT_PEC_2_STACK2, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_STACK0_MSIBAR_REG                                   ,
                ULL(0x04010C53), SH_UNT_PEC_STACK0, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_STACK1_MSIBAR_REG                                   ,
                ULL(0x04010C93), SH_UNT_PEC_STACK1, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_STACK2_MSIBAR_REG                                   ,
                ULL(0x04010CD3), SH_UNT_PEC_STACK2, SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU_CTL_NDLMUX_CONFIG                                ,
                ULL(0x05011383), SH_UNT_PU_NPU_CTL, SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU0_SM0_NDT0_BAR                                    ,
                ULL(0x0501100D), SH_UNT_PU_NPU0_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM1_NDT0_BAR                                    ,
                ULL(0x0501102D), SH_UNT_PU_NPU0_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM2_NDT0_BAR                                    ,
                ULL(0x0501104D), SH_UNT_PU_NPU0_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM3_NDT0_BAR                                    ,
                ULL(0x0501106D), SH_UNT_PU_NPU0_SM3, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM0_NDT0_BAR                                    ,
                ULL(0x0501110D), SH_UNT_PU_NPU1_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM1_NDT0_BAR                                    ,
                ULL(0x0501112D), SH_UNT_PU_NPU1_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM2_NDT0_BAR                                    ,
                ULL(0x0501114D), SH_UNT_PU_NPU1_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM3_NDT0_BAR                                    ,
                ULL(0x0501116D), SH_UNT_PU_NPU1_SM3, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM0_NDT0_BAR                                    ,
                ULL(0x0501120D), SH_UNT_PU_NPU2_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM1_NDT0_BAR                                    ,
                ULL(0x0501122D), SH_UNT_PU_NPU2_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM2_NDT0_BAR                                    ,
                ULL(0x0501124D), SH_UNT_PU_NPU2_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM3_NDT0_BAR                                    ,
                ULL(0x0501126D), SH_UNT_PU_NPU2_SM3, SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU0_SM0_NDT1_BAR                                    ,
                ULL(0x0501100E), SH_UNT_PU_NPU0_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM1_NDT1_BAR                                    ,
                ULL(0x0501102E), SH_UNT_PU_NPU0_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM2_NDT1_BAR                                    ,
                ULL(0x0501104E), SH_UNT_PU_NPU0_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM3_NDT1_BAR                                    ,
                ULL(0x0501106E), SH_UNT_PU_NPU0_SM3, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM0_NDT1_BAR                                    ,
                ULL(0x0501110E), SH_UNT_PU_NPU1_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM1_NDT1_BAR                                    ,
                ULL(0x0501112E), SH_UNT_PU_NPU1_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM2_NDT1_BAR                                    ,
                ULL(0x0501114E), SH_UNT_PU_NPU1_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM3_NDT1_BAR                                    ,
                ULL(0x0501116E), SH_UNT_PU_NPU1_SM3, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM0_NDT1_BAR                                    ,
                ULL(0x0501120E), SH_UNT_PU_NPU2_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM1_NDT1_BAR                                    ,
                ULL(0x0501122E), SH_UNT_PU_NPU2_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM2_NDT1_BAR                                    ,
                ULL(0x0501124E), SH_UNT_PU_NPU2_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM3_NDT1_BAR                                    ,
                ULL(0x0501126E), SH_UNT_PU_NPU2_SM3, SH_ACS_SCOM      );

CONST_UINT64_T( PEC_NESTTRC_REG                                         ,
                ULL(0x04010C03), SH_UNT_PEC      , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_0_NESTTRC_REG                                       ,
                ULL(0x04010C03), SH_UNT_PEC_0    , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_1_NESTTRC_REG                                       ,
                ULL(0x04011003), SH_UNT_PEC_1    , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_2_NESTTRC_REG                                       ,
                ULL(0x04011403), SH_UNT_PEC_2    , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PEC_0_STACK0_NFIRACTION0_REG                            ,
                ULL(0x04010C46), SH_UNT_PEC_0_STACK0, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_0_STACK1_NFIRACTION0_REG                            ,
                ULL(0x04010C86), SH_UNT_PEC_0_STACK1, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_0_STACK2_NFIRACTION0_REG                            ,
                ULL(0x04010CC6), SH_UNT_PEC_0_STACK2, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_1_STACK0_NFIRACTION0_REG                            ,
                ULL(0x04011046), SH_UNT_PEC_1_STACK0, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_1_STACK1_NFIRACTION0_REG                            ,
                ULL(0x04011086), SH_UNT_PEC_1_STACK1, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_1_STACK2_NFIRACTION0_REG                            ,
                ULL(0x040110C6), SH_UNT_PEC_1_STACK2, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_2_STACK0_NFIRACTION0_REG                            ,
                ULL(0x04011446), SH_UNT_PEC_2_STACK0, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_2_STACK1_NFIRACTION0_REG                            ,
                ULL(0x04011486), SH_UNT_PEC_2_STACK1, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_2_STACK2_NFIRACTION0_REG                            ,
                ULL(0x040114C6), SH_UNT_PEC_2_STACK2, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_STACK0_NFIRACTION0_REG                              ,
                ULL(0x04010C46), SH_UNT_PEC_STACK0, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_STACK1_NFIRACTION0_REG                              ,
                ULL(0x04010C86), SH_UNT_PEC_STACK1, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_STACK2_NFIRACTION0_REG                              ,
                ULL(0x04010CC6), SH_UNT_PEC_STACK2, SH_ACS_SCOM_RW   );

CONST_UINT64_T( PEC_0_STACK0_NFIRACTION1_REG                            ,
                ULL(0x04010C47), SH_UNT_PEC_0_STACK0, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_0_STACK1_NFIRACTION1_REG                            ,
                ULL(0x04010C87), SH_UNT_PEC_0_STACK1, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_0_STACK2_NFIRACTION1_REG                            ,
                ULL(0x04010CC7), SH_UNT_PEC_0_STACK2, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_1_STACK0_NFIRACTION1_REG                            ,
                ULL(0x04011047), SH_UNT_PEC_1_STACK0, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_1_STACK1_NFIRACTION1_REG                            ,
                ULL(0x04011087), SH_UNT_PEC_1_STACK1, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_1_STACK2_NFIRACTION1_REG                            ,
                ULL(0x040110C7), SH_UNT_PEC_1_STACK2, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_2_STACK0_NFIRACTION1_REG                            ,
                ULL(0x04011447), SH_UNT_PEC_2_STACK0, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_2_STACK1_NFIRACTION1_REG                            ,
                ULL(0x04011487), SH_UNT_PEC_2_STACK1, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_2_STACK2_NFIRACTION1_REG                            ,
                ULL(0x040114C7), SH_UNT_PEC_2_STACK2, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_STACK0_NFIRACTION1_REG                              ,
                ULL(0x04010C47), SH_UNT_PEC_STACK0, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_STACK1_NFIRACTION1_REG                              ,
                ULL(0x04010C87), SH_UNT_PEC_STACK1, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_STACK2_NFIRACTION1_REG                              ,
                ULL(0x04010CC7), SH_UNT_PEC_STACK2, SH_ACS_SCOM_RW   );

CONST_UINT64_T( PEC_0_STACK0_NFIRMASK_REG                               ,
                ULL(0x04010C43), SH_UNT_PEC_0_STACK0, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_0_STACK0_NFIRMASK_REG_AND                           ,
                ULL(0x04010C44), SH_UNT_PEC_0_STACK0, SH_ACS_SCOM1_AND );
CONST_UINT64_T( PEC_0_STACK0_NFIRMASK_REG_OR                            ,
                ULL(0x04010C45), SH_UNT_PEC_0_STACK0, SH_ACS_SCOM2_OR  );
CONST_UINT64_T( PEC_0_STACK1_NFIRMASK_REG                               ,
                ULL(0x04010C83), SH_UNT_PEC_0_STACK1, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_0_STACK1_NFIRMASK_REG_AND                           ,
                ULL(0x04010C84), SH_UNT_PEC_0_STACK1, SH_ACS_SCOM1_AND );
CONST_UINT64_T( PEC_0_STACK1_NFIRMASK_REG_OR                            ,
                ULL(0x04010C85), SH_UNT_PEC_0_STACK1, SH_ACS_SCOM2_OR  );
CONST_UINT64_T( PEC_0_STACK2_NFIRMASK_REG                               ,
                ULL(0x04010CC3), SH_UNT_PEC_0_STACK2, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_0_STACK2_NFIRMASK_REG_AND                           ,
                ULL(0x04010CC4), SH_UNT_PEC_0_STACK2, SH_ACS_SCOM1_AND );
CONST_UINT64_T( PEC_0_STACK2_NFIRMASK_REG_OR                            ,
                ULL(0x04010CC5), SH_UNT_PEC_0_STACK2, SH_ACS_SCOM2_OR  );
CONST_UINT64_T( PEC_1_STACK0_NFIRMASK_REG                               ,
                ULL(0x04011043), SH_UNT_PEC_1_STACK0, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_1_STACK0_NFIRMASK_REG_AND                           ,
                ULL(0x04011044), SH_UNT_PEC_1_STACK0, SH_ACS_SCOM1_AND );
CONST_UINT64_T( PEC_1_STACK0_NFIRMASK_REG_OR                            ,
                ULL(0x04011045), SH_UNT_PEC_1_STACK0, SH_ACS_SCOM2_OR  );
CONST_UINT64_T( PEC_1_STACK1_NFIRMASK_REG                               ,
                ULL(0x04011083), SH_UNT_PEC_1_STACK1, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_1_STACK1_NFIRMASK_REG_AND                           ,
                ULL(0x04011084), SH_UNT_PEC_1_STACK1, SH_ACS_SCOM1_AND );
CONST_UINT64_T( PEC_1_STACK1_NFIRMASK_REG_OR                            ,
                ULL(0x04011085), SH_UNT_PEC_1_STACK1, SH_ACS_SCOM2_OR  );
CONST_UINT64_T( PEC_1_STACK2_NFIRMASK_REG                               ,
                ULL(0x040110C3), SH_UNT_PEC_1_STACK2, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_1_STACK2_NFIRMASK_REG_AND                           ,
                ULL(0x040110C4), SH_UNT_PEC_1_STACK2, SH_ACS_SCOM1_AND );
CONST_UINT64_T( PEC_1_STACK2_NFIRMASK_REG_OR                            ,
                ULL(0x040110C5), SH_UNT_PEC_1_STACK2, SH_ACS_SCOM2_OR  );
CONST_UINT64_T( PEC_2_STACK0_NFIRMASK_REG                               ,
                ULL(0x04011443), SH_UNT_PEC_2_STACK0, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_2_STACK0_NFIRMASK_REG_AND                           ,
                ULL(0x04011444), SH_UNT_PEC_2_STACK0, SH_ACS_SCOM1_AND );
CONST_UINT64_T( PEC_2_STACK0_NFIRMASK_REG_OR                            ,
                ULL(0x04011445), SH_UNT_PEC_2_STACK0, SH_ACS_SCOM2_OR  );
CONST_UINT64_T( PEC_2_STACK1_NFIRMASK_REG                               ,
                ULL(0x04011483), SH_UNT_PEC_2_STACK1, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_2_STACK1_NFIRMASK_REG_AND                           ,
                ULL(0x04011484), SH_UNT_PEC_2_STACK1, SH_ACS_SCOM1_AND );
CONST_UINT64_T( PEC_2_STACK1_NFIRMASK_REG_OR                            ,
                ULL(0x04011485), SH_UNT_PEC_2_STACK1, SH_ACS_SCOM2_OR  );
CONST_UINT64_T( PEC_2_STACK2_NFIRMASK_REG                               ,
                ULL(0x040114C3), SH_UNT_PEC_2_STACK2, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_2_STACK2_NFIRMASK_REG_AND                           ,
                ULL(0x040114C4), SH_UNT_PEC_2_STACK2, SH_ACS_SCOM1_AND );
CONST_UINT64_T( PEC_2_STACK2_NFIRMASK_REG_OR                            ,
                ULL(0x040114C5), SH_UNT_PEC_2_STACK2, SH_ACS_SCOM2_OR  );
CONST_UINT64_T( PEC_STACK0_NFIRMASK_REG                                 ,
                ULL(0x04010C43), SH_UNT_PEC_STACK0, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_STACK0_NFIRMASK_REG_AND                             ,
                ULL(0x04010C44), SH_UNT_PEC_STACK0, SH_ACS_SCOM1_AND );
CONST_UINT64_T( PEC_STACK0_NFIRMASK_REG_OR                              ,
                ULL(0x04010C45), SH_UNT_PEC_STACK0, SH_ACS_SCOM2_OR  );
CONST_UINT64_T( PEC_STACK1_NFIRMASK_REG                                 ,
                ULL(0x04010C83), SH_UNT_PEC_STACK1, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_STACK1_NFIRMASK_REG_AND                             ,
                ULL(0x04010C84), SH_UNT_PEC_STACK1, SH_ACS_SCOM1_AND );
CONST_UINT64_T( PEC_STACK1_NFIRMASK_REG_OR                              ,
                ULL(0x04010C85), SH_UNT_PEC_STACK1, SH_ACS_SCOM2_OR  );
CONST_UINT64_T( PEC_STACK2_NFIRMASK_REG                                 ,
                ULL(0x04010CC3), SH_UNT_PEC_STACK2, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_STACK2_NFIRMASK_REG_AND                             ,
                ULL(0x04010CC4), SH_UNT_PEC_STACK2, SH_ACS_SCOM1_AND );
CONST_UINT64_T( PEC_STACK2_NFIRMASK_REG_OR                              ,
                ULL(0x04010CC5), SH_UNT_PEC_STACK2, SH_ACS_SCOM2_OR  );

CONST_UINT64_T( PEC_0_STACK0_NFIRWOF_REG                                ,
                ULL(0x04010C48), SH_UNT_PEC_0_STACK0, SH_ACS_SCOM_WCLRREG );
CONST_UINT64_T( PEC_0_STACK1_NFIRWOF_REG                                ,
                ULL(0x04010C88), SH_UNT_PEC_0_STACK1, SH_ACS_SCOM_WCLRREG );
CONST_UINT64_T( PEC_0_STACK2_NFIRWOF_REG                                ,
                ULL(0x04010CC8), SH_UNT_PEC_0_STACK2, SH_ACS_SCOM_WCLRREG );
CONST_UINT64_T( PEC_1_STACK0_NFIRWOF_REG                                ,
                ULL(0x04011048), SH_UNT_PEC_1_STACK0, SH_ACS_SCOM_WCLRREG );
CONST_UINT64_T( PEC_1_STACK1_NFIRWOF_REG                                ,
                ULL(0x04011088), SH_UNT_PEC_1_STACK1, SH_ACS_SCOM_WCLRREG );
CONST_UINT64_T( PEC_1_STACK2_NFIRWOF_REG                                ,
                ULL(0x040110C8), SH_UNT_PEC_1_STACK2, SH_ACS_SCOM_WCLRREG );
CONST_UINT64_T( PEC_2_STACK0_NFIRWOF_REG                                ,
                ULL(0x04011448), SH_UNT_PEC_2_STACK0, SH_ACS_SCOM_WCLRREG );
CONST_UINT64_T( PEC_2_STACK1_NFIRWOF_REG                                ,
                ULL(0x04011488), SH_UNT_PEC_2_STACK1, SH_ACS_SCOM_WCLRREG );
CONST_UINT64_T( PEC_2_STACK2_NFIRWOF_REG                                ,
                ULL(0x040114C8), SH_UNT_PEC_2_STACK2, SH_ACS_SCOM_WCLRREG );
CONST_UINT64_T( PEC_STACK0_NFIRWOF_REG                                  ,
                ULL(0x04010C48), SH_UNT_PEC_STACK0, SH_ACS_SCOM_WCLRREG );
CONST_UINT64_T( PEC_STACK1_NFIRWOF_REG                                  ,
                ULL(0x04010C88), SH_UNT_PEC_STACK1, SH_ACS_SCOM_WCLRREG );
CONST_UINT64_T( PEC_STACK2_NFIRWOF_REG                                  ,
                ULL(0x04010CC8), SH_UNT_PEC_STACK2, SH_ACS_SCOM_WCLRREG );

CONST_UINT64_T( PEC_0_STACK0_NFIR_REG                                   ,
                ULL(0x04010C40), SH_UNT_PEC_0_STACK0, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_0_STACK0_NFIR_REG_AND                               ,
                ULL(0x04010C41), SH_UNT_PEC_0_STACK0, SH_ACS_SCOM1_AND );
CONST_UINT64_T( PEC_0_STACK0_NFIR_REG_OR                                ,
                ULL(0x04010C42), SH_UNT_PEC_0_STACK0, SH_ACS_SCOM2_OR  );
CONST_UINT64_T( PEC_0_STACK1_NFIR_REG                                   ,
                ULL(0x04010C80), SH_UNT_PEC_0_STACK1, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_0_STACK1_NFIR_REG_AND                               ,
                ULL(0x04010C81), SH_UNT_PEC_0_STACK1, SH_ACS_SCOM1_AND );
CONST_UINT64_T( PEC_0_STACK1_NFIR_REG_OR                                ,
                ULL(0x04010C82), SH_UNT_PEC_0_STACK1, SH_ACS_SCOM2_OR  );
CONST_UINT64_T( PEC_0_STACK2_NFIR_REG                                   ,
                ULL(0x04010CC0), SH_UNT_PEC_0_STACK2, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_0_STACK2_NFIR_REG_AND                               ,
                ULL(0x04010CC1), SH_UNT_PEC_0_STACK2, SH_ACS_SCOM1_AND );
CONST_UINT64_T( PEC_0_STACK2_NFIR_REG_OR                                ,
                ULL(0x04010CC2), SH_UNT_PEC_0_STACK2, SH_ACS_SCOM2_OR  );
CONST_UINT64_T( PEC_1_STACK0_NFIR_REG                                   ,
                ULL(0x04011040), SH_UNT_PEC_1_STACK0, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_1_STACK0_NFIR_REG_AND                               ,
                ULL(0x04011041), SH_UNT_PEC_1_STACK0, SH_ACS_SCOM1_AND );
CONST_UINT64_T( PEC_1_STACK0_NFIR_REG_OR                                ,
                ULL(0x04011042), SH_UNT_PEC_1_STACK0, SH_ACS_SCOM2_OR  );
CONST_UINT64_T( PEC_1_STACK1_NFIR_REG                                   ,
                ULL(0x04011080), SH_UNT_PEC_1_STACK1, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_1_STACK1_NFIR_REG_AND                               ,
                ULL(0x04011081), SH_UNT_PEC_1_STACK1, SH_ACS_SCOM1_AND );
CONST_UINT64_T( PEC_1_STACK1_NFIR_REG_OR                                ,
                ULL(0x04011082), SH_UNT_PEC_1_STACK1, SH_ACS_SCOM2_OR  );
CONST_UINT64_T( PEC_1_STACK2_NFIR_REG                                   ,
                ULL(0x040110C0), SH_UNT_PEC_1_STACK2, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_1_STACK2_NFIR_REG_AND                               ,
                ULL(0x040110C1), SH_UNT_PEC_1_STACK2, SH_ACS_SCOM1_AND );
CONST_UINT64_T( PEC_1_STACK2_NFIR_REG_OR                                ,
                ULL(0x040110C2), SH_UNT_PEC_1_STACK2, SH_ACS_SCOM2_OR  );
CONST_UINT64_T( PEC_2_STACK0_NFIR_REG                                   ,
                ULL(0x04011440), SH_UNT_PEC_2_STACK0, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_2_STACK0_NFIR_REG_AND                               ,
                ULL(0x04011441), SH_UNT_PEC_2_STACK0, SH_ACS_SCOM1_AND );
CONST_UINT64_T( PEC_2_STACK0_NFIR_REG_OR                                ,
                ULL(0x04011442), SH_UNT_PEC_2_STACK0, SH_ACS_SCOM2_OR  );
CONST_UINT64_T( PEC_2_STACK1_NFIR_REG                                   ,
                ULL(0x04011480), SH_UNT_PEC_2_STACK1, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_2_STACK1_NFIR_REG_AND                               ,
                ULL(0x04011481), SH_UNT_PEC_2_STACK1, SH_ACS_SCOM1_AND );
CONST_UINT64_T( PEC_2_STACK1_NFIR_REG_OR                                ,
                ULL(0x04011482), SH_UNT_PEC_2_STACK1, SH_ACS_SCOM2_OR  );
CONST_UINT64_T( PEC_2_STACK2_NFIR_REG                                   ,
                ULL(0x040114C0), SH_UNT_PEC_2_STACK2, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_2_STACK2_NFIR_REG_AND                               ,
                ULL(0x040114C1), SH_UNT_PEC_2_STACK2, SH_ACS_SCOM1_AND );
CONST_UINT64_T( PEC_2_STACK2_NFIR_REG_OR                                ,
                ULL(0x040114C2), SH_UNT_PEC_2_STACK2, SH_ACS_SCOM2_OR  );
CONST_UINT64_T( PEC_STACK0_NFIR_REG                                     ,
                ULL(0x04010C40), SH_UNT_PEC_STACK0, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_STACK0_NFIR_REG_AND                                 ,
                ULL(0x04010C41), SH_UNT_PEC_STACK0, SH_ACS_SCOM1_AND );
CONST_UINT64_T( PEC_STACK0_NFIR_REG_OR                                  ,
                ULL(0x04010C42), SH_UNT_PEC_STACK0, SH_ACS_SCOM2_OR  );
CONST_UINT64_T( PEC_STACK1_NFIR_REG                                     ,
                ULL(0x04010C80), SH_UNT_PEC_STACK1, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_STACK1_NFIR_REG_AND                                 ,
                ULL(0x04010C81), SH_UNT_PEC_STACK1, SH_ACS_SCOM1_AND );
CONST_UINT64_T( PEC_STACK1_NFIR_REG_OR                                  ,
                ULL(0x04010C82), SH_UNT_PEC_STACK1, SH_ACS_SCOM2_OR  );
CONST_UINT64_T( PEC_STACK2_NFIR_REG                                     ,
                ULL(0x04010CC0), SH_UNT_PEC_STACK2, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_STACK2_NFIR_REG_AND                                 ,
                ULL(0x04010CC1), SH_UNT_PEC_STACK2, SH_ACS_SCOM1_AND );
CONST_UINT64_T( PEC_STACK2_NFIR_REG_OR                                  ,
                ULL(0x04010CC2), SH_UNT_PEC_STACK2, SH_ACS_SCOM2_OR  );

CONST_UINT64_T( PU_NOTRUST_BAR0                                         ,
                ULL(0x05015F40), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NOTRUST_BAR0MASK                                     ,
                ULL(0x05015F42), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NOTRUST_BAR1                                         ,
                ULL(0x05015F41), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NOTRUST_BAR1MASK                                     ,
                ULL(0x05015F43), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU_SM0_NPU_AT_ECC                                   ,
                ULL(0x05011300), SH_UNT_PU_NPU_SM0, SH_ACS_SCOM      );

CONST_UINT64_T( PU_NXCQ_PB_MODE_REG                                     ,
                ULL(0x02011095), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NMMU_NX_AS_CMD_CFG                                   ,
                ULL(0x05013021), SH_UNT_PU_NMMU  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NMMU_NX_COP_CFG                                      ,
                ULL(0x0501300F), SH_UNT_PU_NMMU  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NX_CQ_FIR_ACTION0_REG                                ,
                ULL(0x02011086), SH_UNT          , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_NMMU_NX_CQ_FIR_ACTION0_REG                           ,
                ULL(0x05013006), SH_UNT_PU_NMMU  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_NX_CQ_FIR_ACTION1_REG                                ,
                ULL(0x02011087), SH_UNT          , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_NMMU_NX_CQ_FIR_ACTION1_REG                           ,
                ULL(0x05013007), SH_UNT_PU_NMMU  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_NX_CQ_FIR_MASK_REG                                   ,
                ULL(0x02011083), SH_UNT          , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_NX_CQ_FIR_MASK_REG_AND                               ,
                ULL(0x02011084), SH_UNT          , SH_ACS_SCOM1_AND );
CONST_UINT64_T( PU_NX_CQ_FIR_MASK_REG_OR                                ,
                ULL(0x02011085), SH_UNT          , SH_ACS_SCOM2_OR  );
CONST_UINT64_T( PU_NMMU_NX_CQ_FIR_MASK_REG                              ,
                ULL(0x05013003), SH_UNT_PU_NMMU  , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_NMMU_NX_CQ_FIR_MASK_REG_AND                          ,
                ULL(0x05013004), SH_UNT_PU_NMMU  , SH_ACS_SCOM1_AND );
CONST_UINT64_T( PU_NMMU_NX_CQ_FIR_MASK_REG_OR                           ,
                ULL(0x05013005), SH_UNT_PU_NMMU  , SH_ACS_SCOM2_OR  );

CONST_UINT64_T( PU_NX_CQ_FIR_REG                                        ,
                ULL(0x02011080), SH_UNT          , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_NX_CQ_FIR_REG_AND                                    ,
                ULL(0x02011081), SH_UNT          , SH_ACS_SCOM1_AND );
CONST_UINT64_T( PU_NX_CQ_FIR_REG_OR                                     ,
                ULL(0x02011082), SH_UNT          , SH_ACS_SCOM2_OR  );
CONST_UINT64_T( PU_NMMU_NX_CQ_FIR_REG                                   ,
                ULL(0x05013000), SH_UNT_PU_NMMU  , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_NMMU_NX_CQ_FIR_REG_AND                               ,
                ULL(0x05013001), SH_UNT_PU_NMMU  , SH_ACS_SCOM1_AND );
CONST_UINT64_T( PU_NMMU_NX_CQ_FIR_REG_OR                                ,
                ULL(0x05013002), SH_UNT_PU_NMMU  , SH_ACS_SCOM2_OR  );

CONST_UINT64_T( PU_NX_CQ_FIR_WOF_REG                                    ,
                ULL(0x02011088), SH_UNT          , SH_ACS_SCOM_WCLRREG );
CONST_UINT64_T( PU_NMMU_NX_CQ_FIR_WOF_REG                               ,
                ULL(0x05013008), SH_UNT_PU_NMMU  , SH_ACS_SCOM_WCLRREG );

CONST_UINT64_T( PU_NX_DEBUGMUX_CTRL                                     ,
                ULL(0x0201110A), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_NX_DEBUG_SNAPSHOT_0                                  ,
                ULL(0x020110A4), SH_UNT          , SH_ACS_SCOM      );
CONST_UINT64_T( PU_NMMU_NX_DEBUG_SNAPSHOT_0                             ,
                ULL(0x05013024), SH_UNT_PU_NMMU  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NX_DEBUG_SNAPSHOT_1                                  ,
                ULL(0x020110A5), SH_UNT          , SH_ACS_SCOM      );
CONST_UINT64_T( PU_NMMU_NX_DEBUG_SNAPSHOT_1                             ,
                ULL(0x05013025), SH_UNT_PU_NMMU  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NX_DMA_ENG_FIR                                       ,
                ULL(0x02011100), SH_UNT          , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_NX_DMA_ENG_FIR_AND                                   ,
                ULL(0x02011101), SH_UNT          , SH_ACS_SCOM1_AND );
CONST_UINT64_T( PU_NX_DMA_ENG_FIR_OR                                    ,
                ULL(0x02011102), SH_UNT          , SH_ACS_SCOM2_OR  );

CONST_UINT64_T( PU_NX_DMA_ENG_FIR_ACTION0                               ,
                ULL(0x02011106), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_NX_DMA_ENG_FIR_ACTION1                               ,
                ULL(0x02011107), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_NX_DMA_ENG_FIR_MASK                                  ,
                ULL(0x02011103), SH_UNT          , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_NX_DMA_ENG_FIR_MASK_AND                              ,
                ULL(0x02011104), SH_UNT          , SH_ACS_SCOM1_AND );
CONST_UINT64_T( PU_NX_DMA_ENG_FIR_MASK_OR                               ,
                ULL(0x02011105), SH_UNT          , SH_ACS_SCOM2_OR  );

CONST_UINT64_T( PU_NX_DMA_ENG_FIR_WOF                                   ,
                ULL(0x02011108), SH_UNT          , SH_ACS_SCOM_WCLRREG );

CONST_UINT64_T( PU_NX_EPSILON_COUNTER_VALUE                             ,
                ULL(0x0201109D), SH_UNT          , SH_ACS_SCOM      );
CONST_UINT64_T( PU_NMMU_NX_EPSILON_COUNTER_VALUE                        ,
                ULL(0x0501301D), SH_UNT_PU_NMMU  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NX_ERRORINJ_CTRL                                     ,
                ULL(0x0201110C), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_NMMU_NX_FOREIGN_F_BAR0                               ,
                ULL(0x0501301B), SH_UNT_PU_NMMU  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_NMMU_NX_FOREIGN_F_BAR1                               ,
                ULL(0x0501301C), SH_UNT_PU_NMMU  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_NMMU_NX_FOREIGN_L_BAR0                               ,
                ULL(0x05013019), SH_UNT_PU_NMMU  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_NMMU_NX_FOREIGN_L_BAR1                               ,
                ULL(0x0501301A), SH_UNT_PU_NMMU  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_NMMU_NX_GROUP_BAR0                                   ,
                ULL(0x05013017), SH_UNT_PU_NMMU  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_NMMU_NX_GROUP_BAR1                                   ,
                ULL(0x05013018), SH_UNT_PU_NMMU  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_NMMU_NX_ICS_CFG_REG                                  ,
                ULL(0x05013013), SH_UNT_PU_NMMU  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NMMU_NX_ICS_CFG_REG2                                 ,
                ULL(0x05013014), SH_UNT_PU_NMMU  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NX_MISC_CONTROL_REG                                  ,
                ULL(0x020110A8), SH_UNT          , SH_ACS_SCOM      );
CONST_UINT64_T( PU_NMMU_NX_MISC_CONTROL_REG                             ,
                ULL(0x05013028), SH_UNT_PU_NMMU  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NX_MMIO_BAR                                          ,
                ULL(0x0201108D), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NMMU_NX_NODAL_BAR0                                   ,
                ULL(0x05013015), SH_UNT_PU_NMMU  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_NMMU_NX_NODAL_BAR1                                   ,
                ULL(0x05013016), SH_UNT_PU_NMMU  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_NX_PB_DEBUG_REG                                      ,
                ULL(0x02011090), SH_UNT          , SH_ACS_SCOM      );
CONST_UINT64_T( PU_NMMU_NX_PB_DEBUG_REG                                 ,
                ULL(0x05013010), SH_UNT_PU_NMMU  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NX_PB_ECC_REG                                        ,
                ULL(0x02011091), SH_UNT          , SH_ACS_SCOM      );
CONST_UINT64_T( PU_NMMU_NX_PB_ECC_REG                                   ,
                ULL(0x05013011), SH_UNT_PU_NMMU  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NX_PB_ERR_RPT_0                                      ,
                ULL(0x020110A2), SH_UNT          , SH_ACS_SCOM      );
CONST_UINT64_T( PU_NMMU_NX_PB_ERR_RPT_0                                 ,
                ULL(0x05013022), SH_UNT_PU_NMMU  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NX_PB_ERR_RPT_1                                      ,
                ULL(0x020110A1), SH_UNT          , SH_ACS_SCOM      );
CONST_UINT64_T( PU_NMMU_NX_PB_ERR_RPT_1                                 ,
                ULL(0x05013023), SH_UNT_PU_NMMU  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NX_PMU0_CONTROL_REG                                  ,
                ULL(0x020110A6), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NX_PMU0_COUNTER_REG                                  ,
                ULL(0x020110A7), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NX_PMU1_CONTROL_REG                                  ,
                ULL(0x020110A9), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NX_PMU1_COUNTER_REG                                  ,
                ULL(0x020110AA), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NMMU_NX_PMU_CONTROL_REG                              ,
                ULL(0x05013026), SH_UNT_PU_NMMU  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NMMU_NX_PMU_COUNTER_REG                              ,
                ULL(0x05013027), SH_UNT_PU_NMMU  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NX_RNG_BYPASS                                        ,
                ULL(0x020110E4), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NX_RNG_CFG                                           ,
                ULL(0x020110E0), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NX_RNG_RDELAY                                        ,
                ULL(0x020110E5), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NX_RNG_RESET                                         ,
                ULL(0x020110E6), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NX_RNG_ST0                                           ,
                ULL(0x020110E1), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NX_RNG_ST1                                           ,
                ULL(0x020110E2), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NX_RNG_ST2                                           ,
                ULL(0x020110E3), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NX_TRIGGER_CTRL                                      ,
                ULL(0x0201110B), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE0_OCB_OCI_CCSR_OCI                                ,
                ULL(0xC0060480), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE0_OCB_OCI_CCSR_OCI1                               ,
                ULL(0xC0060488), SH_UNT_PU_GPE0  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE0_OCB_OCI_CCSR_OCI2                               ,
                ULL(0xC0060490), SH_UNT_PU_GPE0  , SH_ACS_OCI2      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_CCSR                                    ,
                ULL(0x0006C090), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_CCSR_CLEAR                              ,
                ULL(0x0006C091), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_CLEAR );
CONST_UINT64_T( PU_GPE6_OCB_OCI_CCSR_OR                                 ,
                ULL(0x0006C092), SH_UNT_PU_GPE6  , SH_ACS_SCOM2_OR  );

CONST_UINT64_T( PU_GPE0_OCB_OCI_G0ISR0_OCI                              ,
                ULL(0xC0060320), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_G0ISR0                                  ,
                ULL(0x0006C064), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_GPE0_OCB_OCI_G0ISR1_OCI                              ,
                ULL(0xC00603A0), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_G0ISR1                                  ,
                ULL(0x0006C074), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_GPE0_OCB_OCI_G1ISR0_OCI                              ,
                ULL(0xC0060328), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_G1ISR0                                  ,
                ULL(0x0006C065), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_GPE0_OCB_OCI_G1ISR1_OCI                              ,
                ULL(0xC00603A8), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_G1ISR1                                  ,
                ULL(0x0006C075), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_GPE0_OCB_OCI_G2ISR0_OCI                              ,
                ULL(0xC0060330), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_G2ISR0                                  ,
                ULL(0x0006C066), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_GPE0_OCB_OCI_G2ISR1_OCI                              ,
                ULL(0xC00603B0), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_G2ISR1                                  ,
                ULL(0x0006C076), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_GPE0_OCB_OCI_G3ISR0_OCI                              ,
                ULL(0xC0060338), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_G3ISR0                                  ,
                ULL(0x0006C067), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_GPE0_OCB_OCI_G3ISR1_OCI                              ,
                ULL(0xC00603B8), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_G3ISR1                                  ,
                ULL(0x0006C077), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_GPE1_OCB_OCI_O2SCMD0A_OCI                            ,
                ULL(0xC0063838), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_O2SCMD0A                                ,
                ULL(0x0006C707), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE1_OCB_OCI_O2SCMD0B_OCI                            ,
                ULL(0xC00638B8), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_O2SCMD0B                                ,
                ULL(0x0006C717), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE1_OCB_OCI_O2SCMD1A_OCI                            ,
                ULL(0xC0063938), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_O2SCMD1A                                ,
                ULL(0x0006C727), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE1_OCB_OCI_O2SCMD1B_OCI                            ,
                ULL(0xC00639B8), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_O2SCMD1B                                ,
                ULL(0x0006C737), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE1_OCB_OCI_O2SCTRL10A_OCI                          ,
                ULL(0xC0063810), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_O2SCTRL10A                              ,
                ULL(0x0006C702), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE1_OCB_OCI_O2SCTRL10B_OCI                          ,
                ULL(0xC0063890), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_O2SCTRL10B                              ,
                ULL(0x0006C712), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE1_OCB_OCI_O2SCTRL11A_OCI                          ,
                ULL(0xC0063910), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_O2SCTRL11A                              ,
                ULL(0x0006C722), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE1_OCB_OCI_O2SCTRL11B_OCI                          ,
                ULL(0xC0063990), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_O2SCTRL11B                              ,
                ULL(0x0006C732), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE1_OCB_OCI_O2SCTRL20A_OCI                          ,
                ULL(0xC0063818), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_O2SCTRL20A                              ,
                ULL(0x0006C703), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE1_OCB_OCI_O2SCTRL20B_OCI                          ,
                ULL(0xC0063898), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_O2SCTRL20B                              ,
                ULL(0x0006C713), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE1_OCB_OCI_O2SCTRL21A_OCI                          ,
                ULL(0xC0063918), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_O2SCTRL21A                              ,
                ULL(0x0006C723), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE1_OCB_OCI_O2SCTRL21B_OCI                          ,
                ULL(0xC0063998), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_O2SCTRL21B                              ,
                ULL(0x0006C733), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE1_OCB_OCI_O2SCTRLF0A_OCI                          ,
                ULL(0xC0063800), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_O2SCTRLF0A                              ,
                ULL(0x0006C700), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE1_OCB_OCI_O2SCTRLF0B_OCI                          ,
                ULL(0xC0063880), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_O2SCTRLF0B                              ,
                ULL(0x0006C710), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE1_OCB_OCI_O2SCTRLF1A_OCI                          ,
                ULL(0xC0063900), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_O2SCTRLF1A                              ,
                ULL(0x0006C720), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE1_OCB_OCI_O2SCTRLF1B_OCI                          ,
                ULL(0xC0063980), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_O2SCTRLF1B                              ,
                ULL(0x0006C730), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE1_OCB_OCI_O2SCTRLS0A_OCI                          ,
                ULL(0xC0063808), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_O2SCTRLS0A                              ,
                ULL(0x0006C701), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE1_OCB_OCI_O2SCTRLS0B_OCI                          ,
                ULL(0xC0063888), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_O2SCTRLS0B                              ,
                ULL(0x0006C711), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE1_OCB_OCI_O2SCTRLS1A_OCI                          ,
                ULL(0xC0063908), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_O2SCTRLS1A                              ,
                ULL(0x0006C721), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE1_OCB_OCI_O2SCTRLS1B_OCI                          ,
                ULL(0xC0063988), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_O2SCTRLS1B                              ,
                ULL(0x0006C731), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE1_OCB_OCI_O2SRD0A_OCI                             ,
                ULL(0xC0063848), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_O2SRD0A                                 ,
                ULL(0x0006C709), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_GPE1_OCB_OCI_O2SRD0B_OCI                             ,
                ULL(0xC00638C8), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_O2SRD0B                                 ,
                ULL(0x0006C719), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_GPE1_OCB_OCI_O2SRD1A_OCI                             ,
                ULL(0xC0063948), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_O2SRD1A                                 ,
                ULL(0x0006C729), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_GPE1_OCB_OCI_O2SRD1B_OCI                             ,
                ULL(0xC00639C8), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_O2SRD1B                                 ,
                ULL(0x0006C739), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_GPE1_OCB_OCI_O2SST0A_OCI                             ,
                ULL(0xC0063830), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_O2SST0A                                 ,
                ULL(0x0006C706), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_GPE1_OCB_OCI_O2SST0B_OCI                             ,
                ULL(0xC00638B0), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_O2SST0B                                 ,
                ULL(0x0006C716), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_GPE1_OCB_OCI_O2SST1A_OCI                             ,
                ULL(0xC0063930), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_O2SST1A                                 ,
                ULL(0x0006C726), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_GPE1_OCB_OCI_O2SST1B_OCI                             ,
                ULL(0xC00639B0), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_O2SST1B                                 ,
                ULL(0x0006C736), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_GPE1_OCB_OCI_O2SWD0A_OCI                             ,
                ULL(0xC0063840), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_O2SWD0A                                 ,
                ULL(0x0006C708), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE1_OCB_OCI_O2SWD0B_OCI                             ,
                ULL(0xC00638C0), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_O2SWD0B                                 ,
                ULL(0x0006C718), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE1_OCB_OCI_O2SWD1A_OCI                             ,
                ULL(0xC0063940), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_O2SWD1A                                 ,
                ULL(0x0006C728), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE1_OCB_OCI_O2SWD1B_OCI                             ,
                ULL(0xC00639C0), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_O2SWD1B                                 ,
                ULL(0x0006C738), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OCBLWCR0_OCI                            ,
                ULL(0xC0061040), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OCBLWCR0                                ,
                ULL(0x0006C208), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OCBLWCR1_OCI                            ,
                ULL(0xC00610C0), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OCBLWCR1                                ,
                ULL(0x0006C218), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OCBLWCR2_OCI                            ,
                ULL(0xC0061140), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OCBLWCR2                                ,
                ULL(0x0006C228), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OCBLWCR3_OCI                            ,
                ULL(0xC00611C0), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OCBLWCR3                                ,
                ULL(0x0006C238), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OCBLWSBR0_OCI                           ,
                ULL(0xC0061060), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OCBLWSBR0                               ,
                ULL(0x0006C20C), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OCBLWSBR1_OCI                           ,
                ULL(0xC00610E0), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OCBLWSBR1                               ,
                ULL(0x0006C21C), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OCBLWSBR2_OCI                           ,
                ULL(0xC0061160), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OCBLWSBR2                               ,
                ULL(0x0006C22C), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OCBLWSBR3_OCI                           ,
                ULL(0xC00611E0), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OCBLWSBR3                               ,
                ULL(0x0006C23C), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OCBLWSR0_OCI                            ,
                ULL(0xC0061050), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OCBLWSR0                                ,
                ULL(0x0006C20A), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OCBLWSR1_OCI                            ,
                ULL(0xC00610D0), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OCBLWSR1                                ,
                ULL(0x0006C21A), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OCBLWSR2_OCI                            ,
                ULL(0xC0061150), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OCBLWSR2                                ,
                ULL(0x0006C22A), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OCBLWSR3_OCI                            ,
                ULL(0xC00611D0), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OCBLWSR3                                ,
                ULL(0x0006C23A), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OCBSES0_OCI                             ,
                ULL(0xC0061030), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OCBSES0                                 ,
                ULL(0x0006C206), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OCBSES1_OCI                             ,
                ULL(0xC00610B0), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OCBSES1                                 ,
                ULL(0x0006C216), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OCBSES2_OCI                             ,
                ULL(0xC0061130), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OCBSES2                                 ,
                ULL(0x0006C226), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OCBSES3_OCI                             ,
                ULL(0xC00611B0), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OCBSES3                                 ,
                ULL(0x0006C236), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OCBSHBR0_OCI                            ,
                ULL(0xC0061018), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OCBSHBR0                                ,
                ULL(0x0006C203), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OCBSHBR1_OCI                            ,
                ULL(0xC0061098), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OCBSHBR1                                ,
                ULL(0x0006C213), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OCBSHBR2_OCI                            ,
                ULL(0xC0061118), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OCBSHBR2                                ,
                ULL(0x0006C223), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OCBSHBR3_OCI                            ,
                ULL(0xC0061198), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OCBSHBR3                                ,
                ULL(0x0006C233), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OCBSHCS0_OCI                            ,
                ULL(0xC0061020), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OCBSHCS0                                ,
                ULL(0x0006C204), SH_UNT_PU_GPE6  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OCBSHCS1_OCI                            ,
                ULL(0xC00610A0), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OCBSHCS1                                ,
                ULL(0x0006C214), SH_UNT_PU_GPE6  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OCBSHCS2_OCI                            ,
                ULL(0xC0061120), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OCBSHCS2                                ,
                ULL(0x0006C224), SH_UNT_PU_GPE6  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OCBSHCS3_OCI                            ,
                ULL(0xC00611A0), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OCBSHCS3                                ,
                ULL(0x0006C234), SH_UNT_PU_GPE6  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OCBSHI0_OCI                             ,
                ULL(0xC0061028), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OCBSHI0                                 ,
                ULL(0x0006C205), SH_UNT_PU_GPE6  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OCBSHI1_OCI                             ,
                ULL(0xC00610A8), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OCBSHI1                                 ,
                ULL(0x0006C215), SH_UNT_PU_GPE6  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OCBSHI2_OCI                             ,
                ULL(0xC0061128), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OCBSHI2                                 ,
                ULL(0x0006C225), SH_UNT_PU_GPE6  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OCBSHI3_OCI                             ,
                ULL(0xC00611A8), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OCBSHI3                                 ,
                ULL(0x0006C235), SH_UNT_PU_GPE6  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OCBSLBR0_OCI                            ,
                ULL(0xC0061000), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OCBSLBR0                                ,
                ULL(0x0006C200), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OCBSLBR1_OCI                            ,
                ULL(0xC0061080), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OCBSLBR1                                ,
                ULL(0x0006C210), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OCBSLBR2_OCI                            ,
                ULL(0xC0061100), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OCBSLBR2                                ,
                ULL(0x0006C220), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OCBSLBR3_OCI                            ,
                ULL(0xC0061180), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OCBSLBR3                                ,
                ULL(0x0006C230), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OCBSLCS0_OCI                            ,
                ULL(0xC0061008), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OCBSLCS0                                ,
                ULL(0x0006C201), SH_UNT_PU_GPE6  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OCBSLCS1_OCI                            ,
                ULL(0xC0061088), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OCBSLCS1                                ,
                ULL(0x0006C211), SH_UNT_PU_GPE6  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OCBSLCS2_OCI                            ,
                ULL(0xC0061108), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OCBSLCS2                                ,
                ULL(0x0006C221), SH_UNT_PU_GPE6  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OCBSLCS3_OCI                            ,
                ULL(0xC0061188), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OCBSLCS3                                ,
                ULL(0x0006C231), SH_UNT_PU_GPE6  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OCBSLI0_OCI                             ,
                ULL(0xC0061010), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OCBSLI0                                 ,
                ULL(0x0006C202), SH_UNT_PU_GPE6  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OCBSLI1_OCI                             ,
                ULL(0xC0061090), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OCBSLI1                                 ,
                ULL(0x0006C212), SH_UNT_PU_GPE6  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OCBSLI2_OCI                             ,
                ULL(0xC0061110), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OCBSLI2                                 ,
                ULL(0x0006C222), SH_UNT_PU_GPE6  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OCBSLI3_OCI                             ,
                ULL(0xC0061190), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OCBSLI3                                 ,
                ULL(0x0006C232), SH_UNT_PU_GPE6  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OCCFLG_OCI                              ,
                ULL(0xC0060450), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE0_OCB_OCI_OCCFLG_OCI1                             ,
                ULL(0xC0060458), SH_UNT_PU_GPE0  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE0_OCB_OCI_OCCFLG_OCI2                             ,
                ULL(0xC0060460), SH_UNT_PU_GPE0  , SH_ACS_OCI2      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OCCFLG                                  ,
                ULL(0x0006C08A), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OCCFLG_CLEAR                            ,
                ULL(0x0006C08B), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_CLEAR );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OCCFLG_OR                               ,
                ULL(0x0006C08C), SH_UNT_PU_GPE6  , SH_ACS_SCOM2_OR  );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OCCHBR_OCI                              ,
                ULL(0xC0060478), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OCCHBR                                  ,
                ULL(0x0006C08F), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OCCMISC_OCI                             ,
                ULL(0xC0060400), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE0_OCB_OCI_OCCMISC_OCI1                            ,
                ULL(0xC0060408), SH_UNT_PU_GPE0  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE0_OCB_OCI_OCCMISC_OCI2                            ,
                ULL(0xC0060410), SH_UNT_PU_GPE0  , SH_ACS_OCI2      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OCCMISC_SCOM                            ,
                ULL(0x0006C080), SH_UNT_PU_GPE6  , SH_ACS_SCOM      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OCCMISC_SCOM1                           ,
                ULL(0x0006C081), SH_UNT_PU_GPE6  , SH_ACS_SCOM1     );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OCCMISC_SCOM2                           ,
                ULL(0x0006C082), SH_UNT_PU_GPE6  , SH_ACS_SCOM2     );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OCCS0_OCI                               ,
                ULL(0xC0060430), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OCCS0                                   ,
                ULL(0x0006C086), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OCCS1_OCI                               ,
                ULL(0xC0060438), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OCCS1                                   ,
                ULL(0x0006C087), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OCCS2_OCI                               ,
                ULL(0xC0060440), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OCCS2                                   ,
                ULL(0x0006C088), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OCICFG_OCI                              ,
                ULL(0xC0060428), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OCICFG                                  ,
                ULL(0x0006C085), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OCISR0_OCI                              ,
                ULL(0xC0060308), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OCISR0                                  ,
                ULL(0x0006C061), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OCISR1_OCI                              ,
                ULL(0xC0060388), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OCISR1                                  ,
                ULL(0x0006C071), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_GPE0_OCB_OCI_ODISR0_OCI                              ,
                ULL(0xC0060318), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_ODISR0                                  ,
                ULL(0x0006C063), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_GPE0_OCB_OCI_ODISR1_OCI                              ,
                ULL(0xC0060398), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_ODISR1                                  ,
                ULL(0x0006C073), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OEHDR_OCI                               ,
                ULL(0xC0060420), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OEHDR                                   ,
                ULL(0x0006C084), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OHTMCR_OCI                              ,
                ULL(0xC0060418), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OHTMCR                                  ,
                ULL(0x0006C083), SH_UNT_PU_GPE6  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OIEPR0_OCI                              ,
                ULL(0xC0060060), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE0_OCB_OCI_OIEPR0_OCI1                             ,
                ULL(0xC0060068), SH_UNT_PU_GPE0  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE0_OCB_OCI_OIEPR0_OCI2                             ,
                ULL(0xC0060070), SH_UNT_PU_GPE0  , SH_ACS_OCI2      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OIEPR0                                  ,
                ULL(0x0006C00C), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OIEPR0_CLEAR                            ,
                ULL(0x0006C00D), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_CLEAR );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OIEPR0_OR                               ,
                ULL(0x0006C00E), SH_UNT_PU_GPE6  , SH_ACS_SCOM2_OR  );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OIEPR1_OCI                              ,
                ULL(0xC0060160), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE0_OCB_OCI_OIEPR1_OCI1                             ,
                ULL(0xC0060168), SH_UNT_PU_GPE0  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE0_OCB_OCI_OIEPR1_OCI2                             ,
                ULL(0xC0060170), SH_UNT_PU_GPE0  , SH_ACS_OCI2      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OIEPR1                                  ,
                ULL(0x0006C02C), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OIEPR1_CLEAR                            ,
                ULL(0x0006C02D), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_CLEAR );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OIEPR1_OR                               ,
                ULL(0x0006C02E), SH_UNT_PU_GPE6  , SH_ACS_SCOM2_OR  );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OIMR0_OCI                               ,
                ULL(0xC0060020), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE0_OCB_OCI_OIMR0_OCI1                              ,
                ULL(0xC0060028), SH_UNT_PU_GPE0  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE0_OCB_OCI_OIMR0_OCI2                              ,
                ULL(0xC0060030), SH_UNT_PU_GPE0  , SH_ACS_OCI2      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OIMR0                                   ,
                ULL(0x0006C004), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OIMR0_CLEAR                             ,
                ULL(0x0006C005), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_CLEAR );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OIMR0_OR                                ,
                ULL(0x0006C006), SH_UNT_PU_GPE6  , SH_ACS_SCOM2_OR  );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OIMR1_OCI                               ,
                ULL(0xC0060120), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE0_OCB_OCI_OIMR1_OCI1                              ,
                ULL(0xC0060128), SH_UNT_PU_GPE0  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE0_OCB_OCI_OIMR1_OCI2                              ,
                ULL(0xC0060130), SH_UNT_PU_GPE0  , SH_ACS_OCI2      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OIMR1                                   ,
                ULL(0x0006C024), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OIMR1_CLEAR                             ,
                ULL(0x0006C025), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_CLEAR );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OIMR1_OR                                ,
                ULL(0x0006C026), SH_UNT_PU_GPE6  , SH_ACS_SCOM2_OR  );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OIRR0A_OCI                              ,
                ULL(0xC0060200), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE0_OCB_OCI_OIRR0A_OCI1                             ,
                ULL(0xC0060208), SH_UNT_PU_GPE0  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE0_OCB_OCI_OIRR0A_OCI2                             ,
                ULL(0xC0060210), SH_UNT_PU_GPE0  , SH_ACS_OCI2      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OIRR0A                                  ,
                ULL(0x0006C040), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OIRR0A_CLEAR                            ,
                ULL(0x0006C041), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_CLEAR );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OIRR0A_OR                               ,
                ULL(0x0006C042), SH_UNT_PU_GPE6  , SH_ACS_SCOM2_OR  );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OIRR0B_OCI                              ,
                ULL(0xC0060220), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE0_OCB_OCI_OIRR0B_OCI1                             ,
                ULL(0xC0060228), SH_UNT_PU_GPE0  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE0_OCB_OCI_OIRR0B_OCI2                             ,
                ULL(0xC0060230), SH_UNT_PU_GPE0  , SH_ACS_OCI2      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OIRR0B                                  ,
                ULL(0x0006C044), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OIRR0B_CLEAR                            ,
                ULL(0x0006C045), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_CLEAR );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OIRR0B_OR                               ,
                ULL(0x0006C046), SH_UNT_PU_GPE6  , SH_ACS_SCOM2_OR  );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OIRR0C_OCI                              ,
                ULL(0xC0060240), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE0_OCB_OCI_OIRR0C_OCI1                             ,
                ULL(0xC0060248), SH_UNT_PU_GPE0  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE0_OCB_OCI_OIRR0C_OCI2                             ,
                ULL(0xC0060250), SH_UNT_PU_GPE0  , SH_ACS_OCI2      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OIRR0C                                  ,
                ULL(0x0006C048), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OIRR0C_CLEAR                            ,
                ULL(0x0006C049), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_CLEAR );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OIRR0C_OR                               ,
                ULL(0x0006C04A), SH_UNT_PU_GPE6  , SH_ACS_SCOM2_OR  );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OIRR1A_OCI                              ,
                ULL(0xC0060280), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE0_OCB_OCI_OIRR1A_OCI1                             ,
                ULL(0xC0060288), SH_UNT_PU_GPE0  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE0_OCB_OCI_OIRR1A_OCI2                             ,
                ULL(0xC0060290), SH_UNT_PU_GPE0  , SH_ACS_OCI2      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OIRR1A                                  ,
                ULL(0x0006C050), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OIRR1A_CLEAR                            ,
                ULL(0x0006C051), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_CLEAR );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OIRR1A_OR                               ,
                ULL(0x0006C052), SH_UNT_PU_GPE6  , SH_ACS_SCOM2_OR  );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OIRR1B_OCI                              ,
                ULL(0xC00602A0), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE0_OCB_OCI_OIRR1B_OCI1                             ,
                ULL(0xC00602A8), SH_UNT_PU_GPE0  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE0_OCB_OCI_OIRR1B_OCI2                             ,
                ULL(0xC00602B0), SH_UNT_PU_GPE0  , SH_ACS_OCI2      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OIRR1B                                  ,
                ULL(0x0006C054), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OIRR1B_CLEAR                            ,
                ULL(0x0006C055), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_CLEAR );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OIRR1B_OR                               ,
                ULL(0x0006C056), SH_UNT_PU_GPE6  , SH_ACS_SCOM2_OR  );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OIRR1C_OCI                              ,
                ULL(0xC00602C0), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE0_OCB_OCI_OIRR1C_OCI1                             ,
                ULL(0xC00602C8), SH_UNT_PU_GPE0  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE0_OCB_OCI_OIRR1C_OCI2                             ,
                ULL(0xC00602D0), SH_UNT_PU_GPE0  , SH_ACS_OCI2      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OIRR1C                                  ,
                ULL(0x0006C058), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OIRR1C_CLEAR                            ,
                ULL(0x0006C059), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_CLEAR );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OIRR1C_OR                               ,
                ULL(0x0006C05A), SH_UNT_PU_GPE6  , SH_ACS_SCOM2_OR  );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OISR0_OCI                               ,
                ULL(0xC0060000), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE0_OCB_OCI_OISR0_OCI1                              ,
                ULL(0xC0060008), SH_UNT_PU_GPE0  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE0_OCB_OCI_OISR0_OCI2                              ,
                ULL(0xC0060010), SH_UNT_PU_GPE0  , SH_ACS_OCI2      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OISR0_RO                                ,
                ULL(0x0006C000), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OISR0_CLEAR                             ,
                ULL(0x0006C001), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_CLEAR );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OISR0_OR                                ,
                ULL(0x0006C002), SH_UNT_PU_GPE6  , SH_ACS_SCOM2_OR  );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OISR1_OCI                               ,
                ULL(0xC0060100), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE0_OCB_OCI_OISR1_OCI1                              ,
                ULL(0xC0060108), SH_UNT_PU_GPE0  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE0_OCB_OCI_OISR1_OCI2                              ,
                ULL(0xC0060110), SH_UNT_PU_GPE0  , SH_ACS_OCI2      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OISR1_RO                                ,
                ULL(0x0006C020), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OISR1_CLEAR                             ,
                ULL(0x0006C021), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_CLEAR );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OISR1_OR                                ,
                ULL(0x0006C022), SH_UNT_PU_GPE6  , SH_ACS_SCOM2_OR  );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OITR0_OCI                               ,
                ULL(0xC0060040), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE0_OCB_OCI_OITR0_OCI1                              ,
                ULL(0xC0060048), SH_UNT_PU_GPE0  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE0_OCB_OCI_OITR0_OCI2                              ,
                ULL(0xC0060050), SH_UNT_PU_GPE0  , SH_ACS_OCI2      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OITR0                                   ,
                ULL(0x0006C008), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OITR0_CLEAR                             ,
                ULL(0x0006C009), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_CLEAR );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OITR0_OR                                ,
                ULL(0x0006C00A), SH_UNT_PU_GPE6  , SH_ACS_SCOM2_OR  );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OITR1_OCI                               ,
                ULL(0xC0060140), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE0_OCB_OCI_OITR1_OCI1                              ,
                ULL(0xC0060148), SH_UNT_PU_GPE0  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE0_OCB_OCI_OITR1_OCI2                              ,
                ULL(0xC0060150), SH_UNT_PU_GPE0  , SH_ACS_OCI2      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OITR1                                   ,
                ULL(0x0006C028), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OITR1_CLEAR                             ,
                ULL(0x0006C029), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_CLEAR );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OITR1_OR                                ,
                ULL(0x0006C02A), SH_UNT_PU_GPE6  , SH_ACS_SCOM2_OR  );

CONST_UINT64_T( PU_GPE0_OCB_OCI_ONISR0_OCI                              ,
                ULL(0xC0060300), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_ONISR0                                  ,
                ULL(0x0006C060), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_GPE0_OCB_OCI_ONISR1_OCI                              ,
                ULL(0xC0060380), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_ONISR1                                  ,
                ULL(0x0006C070), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT0C0_OCI                             ,
                ULL(0xC0062000), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT0C0_OCI1                            ,
                ULL(0xC0062800), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT0C0_SCOM                            ,
                ULL(0x0006C400), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT0C0_SCOM1                           ,
                ULL(0x0006C500), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT0C1_OCI                             ,
                ULL(0xC0062008), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT0C1_OCI1                            ,
                ULL(0xC0062808), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT0C1_SCOM                            ,
                ULL(0x0006C401), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT0C1_SCOM1                           ,
                ULL(0x0006C501), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT0C10_OCI                            ,
                ULL(0xC0062050), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT0C10_OCI1                           ,
                ULL(0xC0062850), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT0C10_SCOM                           ,
                ULL(0x0006C40A), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT0C10_SCOM1                          ,
                ULL(0x0006C50A), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT0C11_OCI                            ,
                ULL(0xC0062058), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT0C11_OCI1                           ,
                ULL(0xC0062858), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT0C11_SCOM                           ,
                ULL(0x0006C40B), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT0C11_SCOM1                          ,
                ULL(0x0006C50B), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT0C12_OCI                            ,
                ULL(0xC0062060), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT0C12_OCI1                           ,
                ULL(0xC0062860), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT0C12_SCOM                           ,
                ULL(0x0006C40C), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT0C12_SCOM1                          ,
                ULL(0x0006C50C), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT0C13_OCI                            ,
                ULL(0xC0062068), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT0C13_OCI1                           ,
                ULL(0xC0062868), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT0C13_SCOM                           ,
                ULL(0x0006C40D), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT0C13_SCOM1                          ,
                ULL(0x0006C50D), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT0C14_OCI                            ,
                ULL(0xC0062070), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT0C14_OCI1                           ,
                ULL(0xC0062870), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT0C14_SCOM                           ,
                ULL(0x0006C40E), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT0C14_SCOM1                          ,
                ULL(0x0006C50E), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT0C15_OCI                            ,
                ULL(0xC0062078), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT0C15_OCI1                           ,
                ULL(0xC0062878), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT0C15_SCOM                           ,
                ULL(0x0006C40F), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT0C15_SCOM1                          ,
                ULL(0x0006C50F), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT0C16_OCI                            ,
                ULL(0xC0062080), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT0C16_OCI1                           ,
                ULL(0xC0062880), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT0C16_SCOM                           ,
                ULL(0x0006C410), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT0C16_SCOM1                          ,
                ULL(0x0006C510), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT0C17_OCI                            ,
                ULL(0xC0062088), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT0C17_OCI1                           ,
                ULL(0xC0062888), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT0C17_SCOM                           ,
                ULL(0x0006C411), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT0C17_SCOM1                          ,
                ULL(0x0006C511), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT0C18_OCI                            ,
                ULL(0xC0062090), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT0C18_OCI1                           ,
                ULL(0xC0062890), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT0C18_SCOM                           ,
                ULL(0x0006C412), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT0C18_SCOM1                          ,
                ULL(0x0006C512), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT0C19_OCI                            ,
                ULL(0xC0062098), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT0C19_OCI1                           ,
                ULL(0xC0062898), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT0C19_SCOM                           ,
                ULL(0x0006C413), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT0C19_SCOM1                          ,
                ULL(0x0006C513), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT0C2_OCI                             ,
                ULL(0xC0062010), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT0C2_OCI1                            ,
                ULL(0xC0062810), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT0C2_SCOM                            ,
                ULL(0x0006C402), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT0C2_SCOM1                           ,
                ULL(0x0006C502), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT0C20_OCI                            ,
                ULL(0xC00620A0), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT0C20_OCI1                           ,
                ULL(0xC00628A0), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT0C20_SCOM                           ,
                ULL(0x0006C414), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT0C20_SCOM1                          ,
                ULL(0x0006C514), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT0C21_OCI                            ,
                ULL(0xC00620A8), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT0C21_OCI1                           ,
                ULL(0xC00628A8), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT0C21_SCOM                           ,
                ULL(0x0006C415), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT0C21_SCOM1                          ,
                ULL(0x0006C515), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT0C22_OCI                            ,
                ULL(0xC00620B0), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT0C22_OCI1                           ,
                ULL(0xC00628B0), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT0C22_SCOM                           ,
                ULL(0x0006C416), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT0C22_SCOM1                          ,
                ULL(0x0006C516), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT0C23_OCI                            ,
                ULL(0xC00620B8), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT0C23_OCI1                           ,
                ULL(0xC00628B8), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT0C23_SCOM                           ,
                ULL(0x0006C417), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT0C23_SCOM1                          ,
                ULL(0x0006C517), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT0C3_OCI                             ,
                ULL(0xC0062018), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT0C3_OCI1                            ,
                ULL(0xC0062818), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT0C3_SCOM                            ,
                ULL(0x0006C403), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT0C3_SCOM1                           ,
                ULL(0x0006C503), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT0C4_OCI                             ,
                ULL(0xC0062020), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT0C4_OCI1                            ,
                ULL(0xC0062820), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT0C4_SCOM                            ,
                ULL(0x0006C404), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT0C4_SCOM1                           ,
                ULL(0x0006C504), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT0C5_OCI                             ,
                ULL(0xC0062028), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT0C5_OCI1                            ,
                ULL(0xC0062828), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT0C5_SCOM                            ,
                ULL(0x0006C405), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT0C5_SCOM1                           ,
                ULL(0x0006C505), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT0C6_OCI                             ,
                ULL(0xC0062030), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT0C6_OCI1                            ,
                ULL(0xC0062830), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT0C6_SCOM                            ,
                ULL(0x0006C406), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT0C6_SCOM1                           ,
                ULL(0x0006C506), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT0C7_OCI                             ,
                ULL(0xC0062038), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT0C7_OCI1                            ,
                ULL(0xC0062838), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT0C7_SCOM                            ,
                ULL(0x0006C407), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT0C7_SCOM1                           ,
                ULL(0x0006C507), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT0C8_OCI                             ,
                ULL(0xC0062040), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT0C8_OCI1                            ,
                ULL(0xC0062840), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT0C8_SCOM                            ,
                ULL(0x0006C408), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT0C8_SCOM1                           ,
                ULL(0x0006C508), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT0C9_OCI                             ,
                ULL(0xC0062048), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT0C9_OCI1                            ,
                ULL(0xC0062848), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT0C9_SCOM                            ,
                ULL(0x0006C409), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT0C9_SCOM1                           ,
                ULL(0x0006C509), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT0PRA_OCI                            ,
                ULL(0xC0063000), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT0PRA_OCI1                           ,
                ULL(0xC0063008), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT0PRA_RO                             ,
                ULL(0x0006C600), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT0PRA_CLEAR                          ,
                ULL(0x0006C601), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_CLEAR );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT1C0_OCI                             ,
                ULL(0xC0062100), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT1C0_OCI1                            ,
                ULL(0xC0062900), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT1C0_SCOM                            ,
                ULL(0x0006C420), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT1C0_SCOM1                           ,
                ULL(0x0006C520), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT1C1_OCI                             ,
                ULL(0xC0062108), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT1C1_OCI1                            ,
                ULL(0xC0062908), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT1C1_SCOM                            ,
                ULL(0x0006C421), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT1C1_SCOM1                           ,
                ULL(0x0006C521), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT1C10_OCI                            ,
                ULL(0xC0062150), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT1C10_OCI1                           ,
                ULL(0xC0062950), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT1C10_SCOM                           ,
                ULL(0x0006C42A), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT1C10_SCOM1                          ,
                ULL(0x0006C52A), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT1C11_OCI                            ,
                ULL(0xC0062158), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT1C11_OCI1                           ,
                ULL(0xC0062958), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT1C11_SCOM                           ,
                ULL(0x0006C42B), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT1C11_SCOM1                          ,
                ULL(0x0006C52B), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT1C12_OCI                            ,
                ULL(0xC0062160), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT1C12_OCI1                           ,
                ULL(0xC0062960), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT1C12_SCOM                           ,
                ULL(0x0006C42C), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT1C12_SCOM1                          ,
                ULL(0x0006C52C), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT1C13_OCI                            ,
                ULL(0xC0062168), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT1C13_OCI1                           ,
                ULL(0xC0062968), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT1C13_SCOM                           ,
                ULL(0x0006C42D), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT1C13_SCOM1                          ,
                ULL(0x0006C52D), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT1C14_OCI                            ,
                ULL(0xC0062170), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT1C14_OCI1                           ,
                ULL(0xC0062970), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT1C14_SCOM                           ,
                ULL(0x0006C42E), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT1C14_SCOM1                          ,
                ULL(0x0006C52E), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT1C15_OCI                            ,
                ULL(0xC0062178), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT1C15_OCI1                           ,
                ULL(0xC0062978), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT1C15_SCOM                           ,
                ULL(0x0006C42F), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT1C15_SCOM1                          ,
                ULL(0x0006C52F), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT1C16_OCI                            ,
                ULL(0xC0062180), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT1C16_OCI1                           ,
                ULL(0xC0062980), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT1C16_SCOM                           ,
                ULL(0x0006C430), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT1C16_SCOM1                          ,
                ULL(0x0006C530), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT1C17_OCI                            ,
                ULL(0xC0062188), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT1C17_OCI1                           ,
                ULL(0xC0062988), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT1C17_SCOM                           ,
                ULL(0x0006C431), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT1C17_SCOM1                          ,
                ULL(0x0006C531), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT1C18_OCI                            ,
                ULL(0xC0062190), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT1C18_OCI1                           ,
                ULL(0xC0062990), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT1C18_SCOM                           ,
                ULL(0x0006C432), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT1C18_SCOM1                          ,
                ULL(0x0006C532), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT1C19_OCI                            ,
                ULL(0xC0062198), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT1C19_OCI1                           ,
                ULL(0xC0062998), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT1C19_SCOM                           ,
                ULL(0x0006C433), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT1C19_SCOM1                          ,
                ULL(0x0006C533), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT1C2_OCI                             ,
                ULL(0xC0062110), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT1C2_OCI1                            ,
                ULL(0xC0062910), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT1C2_SCOM                            ,
                ULL(0x0006C422), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT1C2_SCOM1                           ,
                ULL(0x0006C522), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT1C20_OCI                            ,
                ULL(0xC00621A0), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT1C20_OCI1                           ,
                ULL(0xC00629A0), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT1C20_SCOM                           ,
                ULL(0x0006C434), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT1C20_SCOM1                          ,
                ULL(0x0006C534), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT1C21_OCI                            ,
                ULL(0xC00621A8), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT1C21_OCI1                           ,
                ULL(0xC00629A8), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT1C21_SCOM                           ,
                ULL(0x0006C435), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT1C21_SCOM1                          ,
                ULL(0x0006C535), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT1C22_OCI                            ,
                ULL(0xC00621B0), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT1C22_OCI1                           ,
                ULL(0xC00629B0), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT1C22_SCOM                           ,
                ULL(0x0006C436), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT1C22_SCOM1                          ,
                ULL(0x0006C536), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT1C23_OCI                            ,
                ULL(0xC00621B8), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT1C23_OCI1                           ,
                ULL(0xC00629B8), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT1C23_SCOM                           ,
                ULL(0x0006C437), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT1C23_SCOM1                          ,
                ULL(0x0006C537), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT1C3_OCI                             ,
                ULL(0xC0062118), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT1C3_OCI1                            ,
                ULL(0xC0062918), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT1C3_SCOM                            ,
                ULL(0x0006C423), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT1C3_SCOM1                           ,
                ULL(0x0006C523), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT1C4_OCI                             ,
                ULL(0xC0062120), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT1C4_OCI1                            ,
                ULL(0xC0062920), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT1C4_SCOM                            ,
                ULL(0x0006C424), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT1C4_SCOM1                           ,
                ULL(0x0006C524), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT1C5_OCI                             ,
                ULL(0xC0062128), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT1C5_OCI1                            ,
                ULL(0xC0062928), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT1C5_SCOM                            ,
                ULL(0x0006C425), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT1C5_SCOM1                           ,
                ULL(0x0006C525), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT1C6_OCI                             ,
                ULL(0xC0062130), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT1C6_OCI1                            ,
                ULL(0xC0062930), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT1C6_SCOM                            ,
                ULL(0x0006C426), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT1C6_SCOM1                           ,
                ULL(0x0006C526), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT1C7_OCI                             ,
                ULL(0xC0062138), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT1C7_OCI1                            ,
                ULL(0xC0062938), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT1C7_SCOM                            ,
                ULL(0x0006C427), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT1C7_SCOM1                           ,
                ULL(0x0006C527), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT1C8_OCI                             ,
                ULL(0xC0062140), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT1C8_OCI1                            ,
                ULL(0xC0062940), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT1C8_SCOM                            ,
                ULL(0x0006C428), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT1C8_SCOM1                           ,
                ULL(0x0006C528), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT1C9_OCI                             ,
                ULL(0xC0062148), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT1C9_OCI1                            ,
                ULL(0xC0062948), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT1C9_SCOM                            ,
                ULL(0x0006C429), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT1C9_SCOM1                           ,
                ULL(0x0006C529), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT1PRA_OCI                            ,
                ULL(0xC0063100), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT1PRA_OCI1                           ,
                ULL(0xC0063108), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT1PRA_RO                             ,
                ULL(0x0006C620), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT1PRA_CLEAR                          ,
                ULL(0x0006C621), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_CLEAR );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT2C0_OCI                             ,
                ULL(0xC0062200), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT2C0_OCI1                            ,
                ULL(0xC0062A00), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT2C0_SCOM                            ,
                ULL(0x0006C440), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT2C0_SCOM1                           ,
                ULL(0x0006C540), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT2C1_OCI                             ,
                ULL(0xC0062208), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT2C1_OCI1                            ,
                ULL(0xC0062A08), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT2C1_SCOM                            ,
                ULL(0x0006C441), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT2C1_SCOM1                           ,
                ULL(0x0006C541), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT2C10_OCI                            ,
                ULL(0xC0062250), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT2C10_OCI1                           ,
                ULL(0xC0062A50), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT2C10_SCOM                           ,
                ULL(0x0006C44A), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT2C10_SCOM1                          ,
                ULL(0x0006C54A), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT2C11_OCI                            ,
                ULL(0xC0062258), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT2C11_OCI1                           ,
                ULL(0xC0062A58), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT2C11_SCOM                           ,
                ULL(0x0006C44B), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT2C11_SCOM1                          ,
                ULL(0x0006C54B), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT2C12_OCI                            ,
                ULL(0xC0062260), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT2C12_OCI1                           ,
                ULL(0xC0062A60), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT2C12_SCOM                           ,
                ULL(0x0006C44C), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT2C12_SCOM1                          ,
                ULL(0x0006C54C), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT2C13_OCI                            ,
                ULL(0xC0062268), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT2C13_OCI1                           ,
                ULL(0xC0062A68), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT2C13_SCOM                           ,
                ULL(0x0006C44D), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT2C13_SCOM1                          ,
                ULL(0x0006C54D), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT2C14_OCI                            ,
                ULL(0xC0062270), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT2C14_OCI1                           ,
                ULL(0xC0062A70), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT2C14_SCOM                           ,
                ULL(0x0006C44E), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT2C14_SCOM1                          ,
                ULL(0x0006C54E), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT2C15_OCI                            ,
                ULL(0xC0062278), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT2C15_OCI1                           ,
                ULL(0xC0062A78), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT2C15_SCOM                           ,
                ULL(0x0006C44F), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT2C15_SCOM1                          ,
                ULL(0x0006C54F), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT2C16_OCI                            ,
                ULL(0xC0062280), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT2C16_OCI1                           ,
                ULL(0xC0062A80), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT2C16_SCOM                           ,
                ULL(0x0006C450), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT2C16_SCOM1                          ,
                ULL(0x0006C550), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT2C17_OCI                            ,
                ULL(0xC0062288), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT2C17_OCI1                           ,
                ULL(0xC0062A88), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT2C17_SCOM                           ,
                ULL(0x0006C451), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT2C17_SCOM1                          ,
                ULL(0x0006C551), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT2C18_OCI                            ,
                ULL(0xC0062290), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT2C18_OCI1                           ,
                ULL(0xC0062A90), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT2C18_SCOM                           ,
                ULL(0x0006C452), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT2C18_SCOM1                          ,
                ULL(0x0006C552), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT2C19_OCI                            ,
                ULL(0xC0062298), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT2C19_OCI1                           ,
                ULL(0xC0062A98), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT2C19_SCOM                           ,
                ULL(0x0006C453), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT2C19_SCOM1                          ,
                ULL(0x0006C553), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT2C2_OCI                             ,
                ULL(0xC0062210), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT2C2_OCI1                            ,
                ULL(0xC0062A10), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT2C2_SCOM                            ,
                ULL(0x0006C442), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT2C2_SCOM1                           ,
                ULL(0x0006C542), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT2C20_OCI                            ,
                ULL(0xC00622A0), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT2C20_OCI1                           ,
                ULL(0xC0062AA0), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT2C20_SCOM                           ,
                ULL(0x0006C454), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT2C20_SCOM1                          ,
                ULL(0x0006C554), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT2C21_OCI                            ,
                ULL(0xC00622A8), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT2C21_OCI1                           ,
                ULL(0xC0062AA8), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT2C21_SCOM                           ,
                ULL(0x0006C455), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT2C21_SCOM1                          ,
                ULL(0x0006C555), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT2C22_OCI                            ,
                ULL(0xC00622B0), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT2C22_OCI1                           ,
                ULL(0xC0062AB0), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT2C22_SCOM                           ,
                ULL(0x0006C456), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT2C22_SCOM1                          ,
                ULL(0x0006C556), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT2C23_OCI                            ,
                ULL(0xC00622B8), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT2C23_OCI1                           ,
                ULL(0xC0062AB8), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT2C23_SCOM                           ,
                ULL(0x0006C457), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT2C23_SCOM1                          ,
                ULL(0x0006C557), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT2C3_OCI                             ,
                ULL(0xC0062218), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT2C3_OCI1                            ,
                ULL(0xC0062A18), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT2C3_SCOM                            ,
                ULL(0x0006C443), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT2C3_SCOM1                           ,
                ULL(0x0006C543), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT2C4_OCI                             ,
                ULL(0xC0062220), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT2C4_OCI1                            ,
                ULL(0xC0062A20), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT2C4_SCOM                            ,
                ULL(0x0006C444), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT2C4_SCOM1                           ,
                ULL(0x0006C544), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT2C5_OCI                             ,
                ULL(0xC0062228), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT2C5_OCI1                            ,
                ULL(0xC0062A28), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT2C5_SCOM                            ,
                ULL(0x0006C445), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT2C5_SCOM1                           ,
                ULL(0x0006C545), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT2C6_OCI                             ,
                ULL(0xC0062230), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT2C6_OCI1                            ,
                ULL(0xC0062A30), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT2C6_SCOM                            ,
                ULL(0x0006C446), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT2C6_SCOM1                           ,
                ULL(0x0006C546), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT2C7_OCI                             ,
                ULL(0xC0062238), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT2C7_OCI1                            ,
                ULL(0xC0062A38), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT2C7_SCOM                            ,
                ULL(0x0006C447), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT2C7_SCOM1                           ,
                ULL(0x0006C547), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT2C8_OCI                             ,
                ULL(0xC0062240), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT2C8_OCI1                            ,
                ULL(0xC0062A40), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT2C8_SCOM                            ,
                ULL(0x0006C448), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT2C8_SCOM1                           ,
                ULL(0x0006C548), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT2C9_OCI                             ,
                ULL(0xC0062248), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT2C9_OCI1                            ,
                ULL(0xC0062A48), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT2C9_SCOM                            ,
                ULL(0x0006C449), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT2C9_SCOM1                           ,
                ULL(0x0006C549), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT2PRA_OCI                            ,
                ULL(0xC0063200), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT2PRA_OCI1                           ,
                ULL(0xC0063208), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT2PRA_RO                             ,
                ULL(0x0006C640), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT2PRA_CLEAR                          ,
                ULL(0x0006C641), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_CLEAR );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT3C0_OCI                             ,
                ULL(0xC0062300), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT3C0_OCI1                            ,
                ULL(0xC0062B00), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT3C0_SCOM                            ,
                ULL(0x0006C460), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT3C0_SCOM1                           ,
                ULL(0x0006C560), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT3C1_OCI                             ,
                ULL(0xC0062308), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT3C1_OCI1                            ,
                ULL(0xC0062B08), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT3C1_SCOM                            ,
                ULL(0x0006C461), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT3C1_SCOM1                           ,
                ULL(0x0006C561), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT3C10_OCI                            ,
                ULL(0xC0062350), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT3C10_OCI1                           ,
                ULL(0xC0062B50), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT3C10_SCOM                           ,
                ULL(0x0006C46A), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT3C10_SCOM1                          ,
                ULL(0x0006C56A), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT3C11_OCI                            ,
                ULL(0xC0062358), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT3C11_OCI1                           ,
                ULL(0xC0062B58), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT3C11_SCOM                           ,
                ULL(0x0006C46B), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT3C11_SCOM1                          ,
                ULL(0x0006C56B), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT3C12_OCI                            ,
                ULL(0xC0062360), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT3C12_OCI1                           ,
                ULL(0xC0062B60), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT3C12_SCOM                           ,
                ULL(0x0006C46C), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT3C12_SCOM1                          ,
                ULL(0x0006C56C), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT3C13_OCI                            ,
                ULL(0xC0062368), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT3C13_OCI1                           ,
                ULL(0xC0062B68), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT3C13_SCOM                           ,
                ULL(0x0006C46D), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT3C13_SCOM1                          ,
                ULL(0x0006C56D), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT3C14_OCI                            ,
                ULL(0xC0062370), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT3C14_OCI1                           ,
                ULL(0xC0062B70), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT3C14_SCOM                           ,
                ULL(0x0006C46E), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT3C14_SCOM1                          ,
                ULL(0x0006C56E), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT3C15_OCI                            ,
                ULL(0xC0062378), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT3C15_OCI1                           ,
                ULL(0xC0062B78), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT3C15_SCOM                           ,
                ULL(0x0006C46F), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT3C15_SCOM1                          ,
                ULL(0x0006C56F), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT3C16_OCI                            ,
                ULL(0xC0062380), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT3C16_OCI1                           ,
                ULL(0xC0062B80), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT3C16_SCOM                           ,
                ULL(0x0006C470), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT3C16_SCOM1                          ,
                ULL(0x0006C570), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT3C17_OCI                            ,
                ULL(0xC0062388), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT3C17_OCI1                           ,
                ULL(0xC0062B88), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT3C17_SCOM                           ,
                ULL(0x0006C471), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT3C17_SCOM1                          ,
                ULL(0x0006C571), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT3C18_OCI                            ,
                ULL(0xC0062390), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT3C18_OCI1                           ,
                ULL(0xC0062B90), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT3C18_SCOM                           ,
                ULL(0x0006C472), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT3C18_SCOM1                          ,
                ULL(0x0006C572), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT3C19_OCI                            ,
                ULL(0xC0062398), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT3C19_OCI1                           ,
                ULL(0xC0062B98), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT3C19_SCOM                           ,
                ULL(0x0006C473), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT3C19_SCOM1                          ,
                ULL(0x0006C573), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT3C2_OCI                             ,
                ULL(0xC0062310), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT3C2_OCI1                            ,
                ULL(0xC0062B10), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT3C2_SCOM                            ,
                ULL(0x0006C462), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT3C2_SCOM1                           ,
                ULL(0x0006C562), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT3C20_OCI                            ,
                ULL(0xC00623A0), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT3C20_OCI1                           ,
                ULL(0xC0062BA0), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT3C20_SCOM                           ,
                ULL(0x0006C474), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT3C20_SCOM1                          ,
                ULL(0x0006C574), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT3C21_OCI                            ,
                ULL(0xC00623A8), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT3C21_OCI1                           ,
                ULL(0xC0062BA8), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT3C21_SCOM                           ,
                ULL(0x0006C475), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT3C21_SCOM1                          ,
                ULL(0x0006C575), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT3C22_OCI                            ,
                ULL(0xC00623B0), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT3C22_OCI1                           ,
                ULL(0xC0062BB0), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT3C22_SCOM                           ,
                ULL(0x0006C476), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT3C22_SCOM1                          ,
                ULL(0x0006C576), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT3C23_OCI                            ,
                ULL(0xC00623B8), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT3C23_OCI1                           ,
                ULL(0xC0062BB8), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT3C23_SCOM                           ,
                ULL(0x0006C477), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT3C23_SCOM1                          ,
                ULL(0x0006C577), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT3C3_OCI                             ,
                ULL(0xC0062318), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT3C3_OCI1                            ,
                ULL(0xC0062B18), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT3C3_SCOM                            ,
                ULL(0x0006C463), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT3C3_SCOM1                           ,
                ULL(0x0006C563), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT3C4_OCI                             ,
                ULL(0xC0062320), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT3C4_OCI1                            ,
                ULL(0xC0062B20), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT3C4_SCOM                            ,
                ULL(0x0006C464), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT3C4_SCOM1                           ,
                ULL(0x0006C564), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT3C5_OCI                             ,
                ULL(0xC0062328), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT3C5_OCI1                            ,
                ULL(0xC0062B28), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT3C5_SCOM                            ,
                ULL(0x0006C465), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT3C5_SCOM1                           ,
                ULL(0x0006C565), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT3C6_OCI                             ,
                ULL(0xC0062330), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT3C6_OCI1                            ,
                ULL(0xC0062B30), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT3C6_SCOM                            ,
                ULL(0x0006C466), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT3C6_SCOM1                           ,
                ULL(0x0006C566), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT3C7_OCI                             ,
                ULL(0xC0062338), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT3C7_OCI1                            ,
                ULL(0xC0062B38), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT3C7_SCOM                            ,
                ULL(0x0006C467), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT3C7_SCOM1                           ,
                ULL(0x0006C567), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT3C8_OCI                             ,
                ULL(0xC0062340), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT3C8_OCI1                            ,
                ULL(0xC0062B40), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT3C8_SCOM                            ,
                ULL(0x0006C468), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT3C8_SCOM1                           ,
                ULL(0x0006C568), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT3C9_OCI                             ,
                ULL(0xC0062348), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT3C9_OCI1                            ,
                ULL(0xC0062B48), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT3C9_SCOM                            ,
                ULL(0x0006C469), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT3C9_SCOM1                           ,
                ULL(0x0006C569), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT3PRA_OCI                            ,
                ULL(0xC0063300), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT3PRA_OCI1                           ,
                ULL(0xC0063308), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT3PRA_RO                             ,
                ULL(0x0006C660), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT3PRA_CLEAR                          ,
                ULL(0x0006C661), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_CLEAR );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT4C0_OCI                             ,
                ULL(0xC0062400), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT4C0_OCI1                            ,
                ULL(0xC0062C00), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT4C0_SCOM                            ,
                ULL(0x0006C480), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT4C0_SCOM1                           ,
                ULL(0x0006C580), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT4C1_OCI                             ,
                ULL(0xC0062408), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT4C1_OCI1                            ,
                ULL(0xC0062C08), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT4C1_SCOM                            ,
                ULL(0x0006C481), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT4C1_SCOM1                           ,
                ULL(0x0006C581), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT4C10_OCI                            ,
                ULL(0xC0062450), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT4C10_OCI1                           ,
                ULL(0xC0062C50), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT4C10_SCOM                           ,
                ULL(0x0006C48A), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT4C10_SCOM1                          ,
                ULL(0x0006C58A), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT4C11_OCI                            ,
                ULL(0xC0062458), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT4C11_OCI1                           ,
                ULL(0xC0062C58), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT4C11_SCOM                           ,
                ULL(0x0006C48B), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT4C11_SCOM1                          ,
                ULL(0x0006C58B), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT4C12_OCI                            ,
                ULL(0xC0062460), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT4C12_OCI1                           ,
                ULL(0xC0062C60), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT4C12_SCOM                           ,
                ULL(0x0006C48C), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT4C12_SCOM1                          ,
                ULL(0x0006C58C), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT4C13_OCI                            ,
                ULL(0xC0062468), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT4C13_OCI1                           ,
                ULL(0xC0062C68), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT4C13_SCOM                           ,
                ULL(0x0006C48D), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT4C13_SCOM1                          ,
                ULL(0x0006C58D), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT4C14_OCI                            ,
                ULL(0xC0062470), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT4C14_OCI1                           ,
                ULL(0xC0062C70), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT4C14_SCOM                           ,
                ULL(0x0006C48E), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT4C14_SCOM1                          ,
                ULL(0x0006C58E), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT4C15_OCI                            ,
                ULL(0xC0062478), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT4C15_OCI1                           ,
                ULL(0xC0062C78), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT4C15_SCOM                           ,
                ULL(0x0006C48F), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT4C15_SCOM1                          ,
                ULL(0x0006C58F), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT4C16_OCI                            ,
                ULL(0xC0062480), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT4C16_OCI1                           ,
                ULL(0xC0062C80), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT4C16_SCOM                           ,
                ULL(0x0006C490), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT4C16_SCOM1                          ,
                ULL(0x0006C590), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT4C17_OCI                            ,
                ULL(0xC0062488), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT4C17_OCI1                           ,
                ULL(0xC0062C88), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT4C17_SCOM                           ,
                ULL(0x0006C491), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT4C17_SCOM1                          ,
                ULL(0x0006C591), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT4C18_OCI                            ,
                ULL(0xC0062490), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT4C18_OCI1                           ,
                ULL(0xC0062C90), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT4C18_SCOM                           ,
                ULL(0x0006C492), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT4C18_SCOM1                          ,
                ULL(0x0006C592), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT4C19_OCI                            ,
                ULL(0xC0062498), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT4C19_OCI1                           ,
                ULL(0xC0062C98), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT4C19_SCOM                           ,
                ULL(0x0006C493), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT4C19_SCOM1                          ,
                ULL(0x0006C593), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT4C2_OCI                             ,
                ULL(0xC0062410), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT4C2_OCI1                            ,
                ULL(0xC0062C10), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT4C2_SCOM                            ,
                ULL(0x0006C482), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT4C2_SCOM1                           ,
                ULL(0x0006C582), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT4C20_OCI                            ,
                ULL(0xC00624A0), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT4C20_OCI1                           ,
                ULL(0xC0062CA0), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT4C20_SCOM                           ,
                ULL(0x0006C494), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT4C20_SCOM1                          ,
                ULL(0x0006C594), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT4C21_OCI                            ,
                ULL(0xC00624A8), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT4C21_OCI1                           ,
                ULL(0xC0062CA8), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT4C21_SCOM                           ,
                ULL(0x0006C495), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT4C21_SCOM1                          ,
                ULL(0x0006C595), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT4C22_OCI                            ,
                ULL(0xC00624B0), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT4C22_OCI1                           ,
                ULL(0xC0062CB0), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT4C22_SCOM                           ,
                ULL(0x0006C496), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT4C22_SCOM1                          ,
                ULL(0x0006C596), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT4C23_OCI                            ,
                ULL(0xC00624B8), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT4C23_OCI1                           ,
                ULL(0xC0062CB8), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT4C23_SCOM                           ,
                ULL(0x0006C497), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT4C23_SCOM1                          ,
                ULL(0x0006C597), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT4C3_OCI                             ,
                ULL(0xC0062418), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT4C3_OCI1                            ,
                ULL(0xC0062C18), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT4C3_SCOM                            ,
                ULL(0x0006C483), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT4C3_SCOM1                           ,
                ULL(0x0006C583), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT4C4_OCI                             ,
                ULL(0xC0062420), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT4C4_OCI1                            ,
                ULL(0xC0062C20), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT4C4_SCOM                            ,
                ULL(0x0006C484), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT4C4_SCOM1                           ,
                ULL(0x0006C584), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT4C5_OCI                             ,
                ULL(0xC0062428), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT4C5_OCI1                            ,
                ULL(0xC0062C28), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT4C5_SCOM                            ,
                ULL(0x0006C485), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT4C5_SCOM1                           ,
                ULL(0x0006C585), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT4C6_OCI                             ,
                ULL(0xC0062430), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT4C6_OCI1                            ,
                ULL(0xC0062C30), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT4C6_SCOM                            ,
                ULL(0x0006C486), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT4C6_SCOM1                           ,
                ULL(0x0006C586), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT4C7_OCI                             ,
                ULL(0xC0062438), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT4C7_OCI1                            ,
                ULL(0xC0062C38), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT4C7_SCOM                            ,
                ULL(0x0006C487), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT4C7_SCOM1                           ,
                ULL(0x0006C587), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT4C8_OCI                             ,
                ULL(0xC0062440), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT4C8_OCI1                            ,
                ULL(0xC0062C40), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT4C8_SCOM                            ,
                ULL(0x0006C488), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT4C8_SCOM1                           ,
                ULL(0x0006C588), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT4C9_OCI                             ,
                ULL(0xC0062448), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT4C9_OCI1                            ,
                ULL(0xC0062C48), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT4C9_SCOM                            ,
                ULL(0x0006C489), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT4C9_SCOM1                           ,
                ULL(0x0006C589), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT4PRA_OCI                            ,
                ULL(0xC0063400), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT4PRA_OCI1                           ,
                ULL(0xC0063408), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT4PRA_RO                             ,
                ULL(0x0006C680), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT4PRA_CLEAR                          ,
                ULL(0x0006C681), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_CLEAR );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT5C0_OCI                             ,
                ULL(0xC0062500), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT5C0_OCI1                            ,
                ULL(0xC0062D00), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT5C0_SCOM                            ,
                ULL(0x0006C4A0), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT5C0_SCOM1                           ,
                ULL(0x0006C5A0), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT5C1_OCI                             ,
                ULL(0xC0062508), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT5C1_OCI1                            ,
                ULL(0xC0062D08), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT5C1_SCOM                            ,
                ULL(0x0006C4A1), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT5C1_SCOM1                           ,
                ULL(0x0006C5A1), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT5C10_OCI                            ,
                ULL(0xC0062550), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT5C10_OCI1                           ,
                ULL(0xC0062D50), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT5C10_SCOM                           ,
                ULL(0x0006C4AA), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT5C10_SCOM1                          ,
                ULL(0x0006C5AA), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT5C11_OCI                            ,
                ULL(0xC0062558), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT5C11_OCI1                           ,
                ULL(0xC0062D58), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT5C11_SCOM                           ,
                ULL(0x0006C4AB), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT5C11_SCOM1                          ,
                ULL(0x0006C5AB), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT5C12_OCI                            ,
                ULL(0xC0062560), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT5C12_OCI1                           ,
                ULL(0xC0062D60), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT5C12_SCOM                           ,
                ULL(0x0006C4AC), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT5C12_SCOM1                          ,
                ULL(0x0006C5AC), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT5C13_OCI                            ,
                ULL(0xC0062568), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT5C13_OCI1                           ,
                ULL(0xC0062D68), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT5C13_SCOM                           ,
                ULL(0x0006C4AD), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT5C13_SCOM1                          ,
                ULL(0x0006C5AD), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT5C14_OCI                            ,
                ULL(0xC0062570), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT5C14_OCI1                           ,
                ULL(0xC0062D70), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT5C14_SCOM                           ,
                ULL(0x0006C4AE), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT5C14_SCOM1                          ,
                ULL(0x0006C5AE), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT5C15_OCI                            ,
                ULL(0xC0062578), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT5C15_OCI1                           ,
                ULL(0xC0062D78), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT5C15_SCOM                           ,
                ULL(0x0006C4AF), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT5C15_SCOM1                          ,
                ULL(0x0006C5AF), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT5C16_OCI                            ,
                ULL(0xC0062580), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT5C16_OCI1                           ,
                ULL(0xC0062D80), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT5C16_SCOM                           ,
                ULL(0x0006C4B0), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT5C16_SCOM1                          ,
                ULL(0x0006C5B0), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT5C17_OCI                            ,
                ULL(0xC0062588), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT5C17_OCI1                           ,
                ULL(0xC0062D88), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT5C17_SCOM                           ,
                ULL(0x0006C4B1), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT5C17_SCOM1                          ,
                ULL(0x0006C5B1), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT5C18_OCI                            ,
                ULL(0xC0062590), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT5C18_OCI1                           ,
                ULL(0xC0062D90), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT5C18_SCOM                           ,
                ULL(0x0006C4B2), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT5C18_SCOM1                          ,
                ULL(0x0006C5B2), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT5C19_OCI                            ,
                ULL(0xC0062598), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT5C19_OCI1                           ,
                ULL(0xC0062D98), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT5C19_SCOM                           ,
                ULL(0x0006C4B3), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT5C19_SCOM1                          ,
                ULL(0x0006C5B3), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT5C2_OCI                             ,
                ULL(0xC0062510), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT5C2_OCI1                            ,
                ULL(0xC0062D10), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT5C2_SCOM                            ,
                ULL(0x0006C4A2), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT5C2_SCOM1                           ,
                ULL(0x0006C5A2), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT5C20_OCI                            ,
                ULL(0xC00625A0), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT5C20_OCI1                           ,
                ULL(0xC0062DA0), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT5C20_SCOM                           ,
                ULL(0x0006C4B4), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT5C20_SCOM1                          ,
                ULL(0x0006C5B4), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT5C21_OCI                            ,
                ULL(0xC00625A8), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT5C21_OCI1                           ,
                ULL(0xC0062DA8), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT5C21_SCOM                           ,
                ULL(0x0006C4B5), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT5C21_SCOM1                          ,
                ULL(0x0006C5B5), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT5C22_OCI                            ,
                ULL(0xC00625B0), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT5C22_OCI1                           ,
                ULL(0xC0062DB0), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT5C22_SCOM                           ,
                ULL(0x0006C4B6), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT5C22_SCOM1                          ,
                ULL(0x0006C5B6), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT5C23_OCI                            ,
                ULL(0xC00625B8), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT5C23_OCI1                           ,
                ULL(0xC0062DB8), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT5C23_SCOM                           ,
                ULL(0x0006C4B7), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT5C23_SCOM1                          ,
                ULL(0x0006C5B7), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT5C3_OCI                             ,
                ULL(0xC0062518), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT5C3_OCI1                            ,
                ULL(0xC0062D18), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT5C3_SCOM                            ,
                ULL(0x0006C4A3), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT5C3_SCOM1                           ,
                ULL(0x0006C5A3), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT5C4_OCI                             ,
                ULL(0xC0062520), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT5C4_OCI1                            ,
                ULL(0xC0062D20), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT5C4_SCOM                            ,
                ULL(0x0006C4A4), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT5C4_SCOM1                           ,
                ULL(0x0006C5A4), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT5C5_OCI                             ,
                ULL(0xC0062528), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT5C5_OCI1                            ,
                ULL(0xC0062D28), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT5C5_SCOM                            ,
                ULL(0x0006C4A5), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT5C5_SCOM1                           ,
                ULL(0x0006C5A5), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT5C6_OCI                             ,
                ULL(0xC0062530), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT5C6_OCI1                            ,
                ULL(0xC0062D30), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT5C6_SCOM                            ,
                ULL(0x0006C4A6), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT5C6_SCOM1                           ,
                ULL(0x0006C5A6), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT5C7_OCI                             ,
                ULL(0xC0062538), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT5C7_OCI1                            ,
                ULL(0xC0062D38), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT5C7_SCOM                            ,
                ULL(0x0006C4A7), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT5C7_SCOM1                           ,
                ULL(0x0006C5A7), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT5C8_OCI                             ,
                ULL(0xC0062540), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT5C8_OCI1                            ,
                ULL(0xC0062D40), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT5C8_SCOM                            ,
                ULL(0x0006C4A8), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT5C8_SCOM1                           ,
                ULL(0x0006C5A8), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT5C9_OCI                             ,
                ULL(0xC0062548), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT5C9_OCI1                            ,
                ULL(0xC0062D48), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT5C9_SCOM                            ,
                ULL(0x0006C4A9), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT5C9_SCOM1                           ,
                ULL(0x0006C5A9), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT5PRA_OCI                            ,
                ULL(0xC0063500), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT5PRA_OCI1                           ,
                ULL(0xC0063508), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT5PRA_RO                             ,
                ULL(0x0006C6A0), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT5PRA_CLEAR                          ,
                ULL(0x0006C6A1), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_CLEAR );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT6PRB_OCI                            ,
                ULL(0xC0063600), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT6PRB_OCI1                           ,
                ULL(0xC0063608), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT6PRB_RO                             ,
                ULL(0x0006C6C0), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT6PRB_CLEAR                          ,
                ULL(0x0006C6C1), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_CLEAR );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT6Q0_OCI                             ,
                ULL(0xC0062600), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT6Q0_OCI1                            ,
                ULL(0xC0062E00), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT6Q0_SCOM                            ,
                ULL(0x0006C4C0), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT6Q0_SCOM1                           ,
                ULL(0x0006C5C0), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT6Q1_OCI                             ,
                ULL(0xC0062608), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT6Q1_OCI1                            ,
                ULL(0xC0062E08), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT6Q1_SCOM                            ,
                ULL(0x0006C4C1), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT6Q1_SCOM1                           ,
                ULL(0x0006C5C1), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT6Q2_OCI                             ,
                ULL(0xC0062610), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT6Q2_OCI1                            ,
                ULL(0xC0062E10), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT6Q2_SCOM                            ,
                ULL(0x0006C4C2), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT6Q2_SCOM1                           ,
                ULL(0x0006C5C2), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT6Q3_OCI                             ,
                ULL(0xC0062618), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT6Q3_OCI1                            ,
                ULL(0xC0062E18), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT6Q3_SCOM                            ,
                ULL(0x0006C4C3), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT6Q3_SCOM1                           ,
                ULL(0x0006C5C3), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT6Q4_OCI                             ,
                ULL(0xC0062620), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT6Q4_OCI1                            ,
                ULL(0xC0062E20), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT6Q4_SCOM                            ,
                ULL(0x0006C4C4), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT6Q4_SCOM1                           ,
                ULL(0x0006C5C4), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT6Q5_OCI                             ,
                ULL(0xC0062628), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT6Q5_OCI1                            ,
                ULL(0xC0062E28), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT6Q5_SCOM                            ,
                ULL(0x0006C4C5), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT6Q5_SCOM1                           ,
                ULL(0x0006C5C5), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT7PRB_OCI                            ,
                ULL(0xC0063700), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT7PRB_OCI1                           ,
                ULL(0xC0063708), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT7PRB_RO                             ,
                ULL(0x0006C6E0), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT7PRB_CLEAR                          ,
                ULL(0x0006C6E1), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_CLEAR );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT7Q0_OCI                             ,
                ULL(0xC0062700), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT7Q0_OCI1                            ,
                ULL(0xC0062F00), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT7Q0_SCOM                            ,
                ULL(0x0006C4E0), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT7Q0_SCOM1                           ,
                ULL(0x0006C5E0), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT7Q1_OCI                             ,
                ULL(0xC0062708), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT7Q1_OCI1                            ,
                ULL(0xC0062F08), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT7Q1_SCOM                            ,
                ULL(0x0006C4E1), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT7Q1_SCOM1                           ,
                ULL(0x0006C5E1), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT7Q2_OCI                             ,
                ULL(0xC0062710), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT7Q2_OCI1                            ,
                ULL(0xC0062F10), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT7Q2_SCOM                            ,
                ULL(0x0006C4E2), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT7Q2_SCOM1                           ,
                ULL(0x0006C5E2), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT7Q3_OCI                             ,
                ULL(0xC0062718), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT7Q3_OCI1                            ,
                ULL(0xC0062F18), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT7Q3_SCOM                            ,
                ULL(0x0006C4E3), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT7Q3_SCOM1                           ,
                ULL(0x0006C5E3), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT7Q4_OCI                             ,
                ULL(0xC0062720), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT7Q4_OCI1                            ,
                ULL(0xC0062F20), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT7Q4_SCOM                            ,
                ULL(0x0006C4E4), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT7Q4_SCOM1                           ,
                ULL(0x0006C5E4), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT7Q5_OCI                             ,
                ULL(0xC0062728), SH_UNT_PU_GPE1  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE1_OCB_OCI_OPIT7Q5_OCI1                            ,
                ULL(0xC0062F28), SH_UNT_PU_GPE1  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT7Q5_SCOM                            ,
                ULL(0x0006C4E5), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OPIT7Q5_SCOM1                           ,
                ULL(0x0006C5E5), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_RO  );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OTBR_OCI                                ,
                ULL(0xC00604F8), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OTBR                                    ,
                ULL(0x0006C09F), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OTR0_OCI                                ,
                ULL(0xC0060800), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OTR0                                    ,
                ULL(0x0006C100), SH_UNT_PU_GPE6  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OTR1_OCI                                ,
                ULL(0xC0060808), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OTR1                                    ,
                ULL(0x0006C101), SH_UNT_PU_GPE6  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OUISR0_OCI                              ,
                ULL(0xC0060310), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OUISR0                                  ,
                ULL(0x0006C062), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_GPE0_OCB_OCI_OUISR1_OCI                              ,
                ULL(0xC0060390), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE6_OCB_OCI_OUISR1                                  ,
                ULL(0x0006C072), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_GPE0_OCB_OCI_QCSR_OCI                                ,
                ULL(0xC00604A0), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE0_OCB_OCI_QCSR_OCI1                               ,
                ULL(0xC00604A8), SH_UNT_PU_GPE0  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE0_OCB_OCI_QCSR_OCI2                               ,
                ULL(0xC00604B0), SH_UNT_PU_GPE0  , SH_ACS_OCI2      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_QCSR                                    ,
                ULL(0x0006C094), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_QCSR_CLEAR                              ,
                ULL(0x0006C095), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_CLEAR );
CONST_UINT64_T( PU_GPE6_OCB_OCI_QCSR_OR                                 ,
                ULL(0x0006C096), SH_UNT_PU_GPE6  , SH_ACS_SCOM2_OR  );

CONST_UINT64_T( PU_GPE0_OCB_OCI_QSSR_OCI                                ,
                ULL(0xC00604C0), SH_UNT_PU_GPE0  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE0_OCB_OCI_QSSR_OCI1                               ,
                ULL(0xC00604C8), SH_UNT_PU_GPE0  , SH_ACS_OCI1      );
CONST_UINT64_T( PU_GPE0_OCB_OCI_QSSR_OCI2                               ,
                ULL(0xC00604D0), SH_UNT_PU_GPE0  , SH_ACS_OCI2      );
CONST_UINT64_T( PU_GPE6_OCB_OCI_QSSR                                    ,
                ULL(0x0006C098), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_GPE6_OCB_OCI_QSSR_CLEAR                              ,
                ULL(0x0006C099), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_CLEAR );
CONST_UINT64_T( PU_GPE6_OCB_OCI_QSSR_OR                                 ,
                ULL(0x0006C09A), SH_UNT_PU_GPE6  , SH_ACS_SCOM2_OR  );

CONST_UINT64_T( PU_GPE6_OCB_PIB_OACR                                    ,
                ULL(0x0006D207), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE6_OCB_PIB_OCBAR0                                  ,
                ULL(0x0006D010), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE6_OCB_PIB_OCBAR1                                  ,
                ULL(0x0006D030), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE6_OCB_PIB_OCBAR2                                  ,
                ULL(0x0006D050), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE6_OCB_PIB_OCBAR3                                  ,
                ULL(0x0006D070), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE6_OCB_PIB_OCBCSR0_RO                              ,
                ULL(0x0006D011), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_PIB_OCBCSR0_CLEAR                           ,
                ULL(0x0006D012), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_CLEAR );
CONST_UINT64_T( PU_GPE6_OCB_PIB_OCBCSR0_OR                              ,
                ULL(0x0006D013), SH_UNT_PU_GPE6  , SH_ACS_SCOM2_OR  );

CONST_UINT64_T( PU_GPE6_OCB_PIB_OCBCSR1_RO                              ,
                ULL(0x0006D031), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_PIB_OCBCSR1_CLEAR                           ,
                ULL(0x0006D032), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_CLEAR );
CONST_UINT64_T( PU_GPE6_OCB_PIB_OCBCSR1_OR                              ,
                ULL(0x0006D033), SH_UNT_PU_GPE6  , SH_ACS_SCOM2_OR  );

CONST_UINT64_T( PU_GPE6_OCB_PIB_OCBCSR2_RO                              ,
                ULL(0x0006D051), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_PIB_OCBCSR2_CLEAR                           ,
                ULL(0x0006D052), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_CLEAR );
CONST_UINT64_T( PU_GPE6_OCB_PIB_OCBCSR2_OR                              ,
                ULL(0x0006D053), SH_UNT_PU_GPE6  , SH_ACS_SCOM2_OR  );

CONST_UINT64_T( PU_GPE6_OCB_PIB_OCBCSR3_RO                              ,
                ULL(0x0006D071), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_PIB_OCBCSR3_CLEAR                           ,
                ULL(0x0006D072), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_CLEAR );
CONST_UINT64_T( PU_GPE6_OCB_PIB_OCBCSR3_OR                              ,
                ULL(0x0006D073), SH_UNT_PU_GPE6  , SH_ACS_SCOM2_OR  );

CONST_UINT64_T( PU_GPE6_OCB_PIB_OCBDR0                                  ,
                ULL(0x0006D015), SH_UNT_PU_GPE6  , SH_ACS_SCOM_WO   );

CONST_UINT64_T( PU_GPE6_OCB_PIB_OCBDR1                                  ,
                ULL(0x0006D035), SH_UNT_PU_GPE6  , SH_ACS_SCOM_WO   );

CONST_UINT64_T( PU_GPE6_OCB_PIB_OCBDR2                                  ,
                ULL(0x0006D055), SH_UNT_PU_GPE6  , SH_ACS_SCOM_WO   );

CONST_UINT64_T( PU_GPE6_OCB_PIB_OCBDR3                                  ,
                ULL(0x0006D075), SH_UNT_PU_GPE6  , SH_ACS_SCOM_WO   );

CONST_UINT64_T( PU_GPE6_OCB_PIB_OCBEAR                                  ,
                ULL(0x0006D210), SH_UNT_PU_GPE6  , SH_ACS_SCOM_WCLRPART );

CONST_UINT64_T( PU_GPE6_OCB_PIB_OCBESR0                                 ,
                ULL(0x0006D014), SH_UNT_PU_GPE6  , SH_ACS_SCOM_WCLRREG );

CONST_UINT64_T( PU_GPE6_OCB_PIB_OCBESR1                                 ,
                ULL(0x0006D034), SH_UNT_PU_GPE6  , SH_ACS_SCOM_WCLRREG );

CONST_UINT64_T( PU_GPE6_OCB_PIB_OCBESR2                                 ,
                ULL(0x0006D054), SH_UNT_PU_GPE6  , SH_ACS_SCOM_WCLRREG );

CONST_UINT64_T( PU_GPE6_OCB_PIB_OCBESR3                                 ,
                ULL(0x0006D074), SH_UNT_PU_GPE6  , SH_ACS_SCOM_WCLRREG );

CONST_UINT64_T( PU_GPE6_OCB_PIB_OCDBG                                   ,
                ULL(0x0006D003), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE6_OCB_PIB_OCR_RO                                  ,
                ULL(0x0006D000), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_GPE6_OCB_PIB_OCR_CLEAR                               ,
                ULL(0x0006D001), SH_UNT_PU_GPE6  , SH_ACS_SCOM1_CLEAR );
CONST_UINT64_T( PU_GPE6_OCB_PIB_OCR_OR                                  ,
                ULL(0x0006D002), SH_UNT_PU_GPE6  , SH_ACS_SCOM2_OR  );

CONST_UINT64_T( PU_GPE6_OCB_PIB_OEAR                                    ,
                ULL(0x0006D206), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_GPE6_OCB_PIB_OESR                                    ,
                ULL(0x0006D204), SH_UNT_PU_GPE6  , SH_ACS_SCOM_WCLEAR );

CONST_UINT64_T( PU_GPE6_OCB_PIB_OPPCINJ                                 ,
                ULL(0x0006D111), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE6_OCB_PIB_OREV                                    ,
                ULL(0x0006D202), SH_UNT_PU_GPE6  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_GPE6_OCB_PIB_OSTOEAR                                 ,
                ULL(0x0006D200), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_GPE6_OCB_PIB_OSTOESR                                 ,
                ULL(0x0006D201), SH_UNT_PU_GPE6  , SH_ACS_SCOM_WCLEAR );

CONST_UINT64_T( PU_GPE6_OCB_PIB_OTDCR                                   ,
                ULL(0x0006D110), SH_UNT_PU_GPE6  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_OSCERR_HOLD                                          ,
                ULL(0x01020019), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_OSCERR_MASK                                          ,
                ULL(0x0102001A), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_OSCERR_MCODE                                         ,
                ULL(0x0102001B), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PBABAR0                                              ,
                ULL(0x05012B00), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_PBABAR1                                              ,
                ULL(0x05012B01), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_PBABAR2                                              ,
                ULL(0x05012B02), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_PBABAR3                                              ,
                ULL(0x05012B03), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_PBABARMSK0                                           ,
                ULL(0x05012B04), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_PBABARMSK1                                           ,
                ULL(0x05012B05), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_PBABARMSK2                                           ,
                ULL(0x05012B06), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_PBABARMSK3                                           ,
                ULL(0x05012B07), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_PBACFG                                               ,
                ULL(0x0501284B), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_PBAERRRPT0                                           ,
                ULL(0x0501284C), SH_UNT          , SH_ACS_SCOM_WCLRPART );

CONST_UINT64_T( PU_PBAERRRPT1                                           ,
                ULL(0x0501284D), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_PBAERRRPT2                                           ,
                ULL(0x0501284E), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_PBAFIR                                               ,
                ULL(0x05012840), SH_UNT          , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_PBAFIR_AND                                           ,
                ULL(0x05012841), SH_UNT          , SH_ACS_SCOM1_AND );
CONST_UINT64_T( PU_PBAFIR_OR                                            ,
                ULL(0x05012842), SH_UNT          , SH_ACS_SCOM2_OR  );

CONST_UINT64_T( PU_PBAFIRACT0                                           ,
                ULL(0x05012846), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_PBAFIRACT1                                           ,
                ULL(0x05012847), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_PBAFIRMASK                                           ,
                ULL(0x05012843), SH_UNT          , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_PBAFIRMASK_AND                                       ,
                ULL(0x05012844), SH_UNT          , SH_ACS_SCOM1_AND );
CONST_UINT64_T( PU_PBAFIRMASK_OR                                        ,
                ULL(0x05012845), SH_UNT          , SH_ACS_SCOM2_OR  );

CONST_UINT64_T( PU_PBAIB0_PBAIBHWCFG_REG                                ,
                ULL(0x0D010802), SH_UNT_PU_PBAIB0, SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_PBAIB_STACK0_PBAIB_CERR_RPT_REG                      ,
                ULL(0x0D010841), SH_UNT_PU_PBAIB_STACK0, SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_PBAIB_STACK1_PBAIB_CERR_RPT_REG                      ,
                ULL(0x0D010881), SH_UNT_PU_PBAIB_STACK1, SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_PBAIB_STACK2_PBAIB_CERR_RPT_REG                      ,
                ULL(0x0D0108C1), SH_UNT_PU_PBAIB_STACK2, SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_PBAMODE_OCI                                          ,
                ULL(0x40020000), SH_UNT          , SH_ACS_OCI       );
CONST_UINT64_T( PU_PBAMODE_SCOM                                         ,
                ULL(0x05016840), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PBAOCCACT                                            ,
                ULL(0x0501284A), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_PBAPBOCR0_OCI                                        ,
                ULL(0x400200D0), SH_UNT          , SH_ACS_OCI       );
CONST_UINT64_T( PU_PBAPBOCR0_SCOM                                       ,
                ULL(0x0501685A), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_PBAPBOCR1_OCI                                        ,
                ULL(0x400200D8), SH_UNT          , SH_ACS_OCI       );
CONST_UINT64_T( PU_PBAPBOCR1_SCOM                                       ,
                ULL(0x0501685B), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_PBAPBOCR2_OCI                                        ,
                ULL(0x400200E0), SH_UNT          , SH_ACS_OCI       );
CONST_UINT64_T( PU_PBAPBOCR2_SCOM                                       ,
                ULL(0x0501685C), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_PBAPBOCR3_OCI                                        ,
                ULL(0x400200E8), SH_UNT          , SH_ACS_OCI       );
CONST_UINT64_T( PU_PBAPBOCR3_SCOM                                       ,
                ULL(0x0501685D), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_PBAPBOCR4_OCI                                        ,
                ULL(0x400200F0), SH_UNT          , SH_ACS_OCI       );
CONST_UINT64_T( PU_PBAPBOCR4_SCOM                                       ,
                ULL(0x0501685E), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_PBAPBOCR5_OCI                                        ,
                ULL(0x400200F8), SH_UNT          , SH_ACS_OCI       );
CONST_UINT64_T( PU_PBAPBOCR5_SCOM                                       ,
                ULL(0x0501685F), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_PBARBUFVAL0                                          ,
                ULL(0x05012850), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_PBARBUFVAL1                                          ,
                ULL(0x05012851), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_PBARBUFVAL2                                          ,
                ULL(0x05012852), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_PBARBUFVAL3                                          ,
                ULL(0x05012853), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_PBARBUFVAL4                                          ,
                ULL(0x05012854), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_PBARBUFVAL5                                          ,
                ULL(0x05012855), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_PBASLVCTL0_OCI                                       ,
                ULL(0x40020020), SH_UNT          , SH_ACS_OCI       );
CONST_UINT64_T( PU_PBASLVCTL0_SCOM                                      ,
                ULL(0x05016844), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_PBASLVCTL1_OCI                                       ,
                ULL(0x40020028), SH_UNT          , SH_ACS_OCI       );
CONST_UINT64_T( PU_PBASLVCTL1_SCOM                                      ,
                ULL(0x05016845), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_PBASLVCTL2_OCI                                       ,
                ULL(0x40020030), SH_UNT          , SH_ACS_OCI       );
CONST_UINT64_T( PU_PBASLVCTL2_SCOM                                      ,
                ULL(0x05016846), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_PBASLVCTL3_OCI                                       ,
                ULL(0x40020038), SH_UNT          , SH_ACS_OCI       );
CONST_UINT64_T( PU_PBASLVCTL3_SCOM                                      ,
                ULL(0x05016847), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_PBASLVRST_OCI                                        ,
                ULL(0x40020008), SH_UNT          , SH_ACS_OCI       );
CONST_UINT64_T( PU_PBASLVRST_SCOM                                       ,
                ULL(0x05016841), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PBAWBUFVAL0                                          ,
                ULL(0x05012858), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_PBAWBUFVAL1                                          ,
                ULL(0x05012859), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_PBAXCFG_OCI                                          ,
                ULL(0x40020108), SH_UNT          , SH_ACS_OCI       );
CONST_UINT64_T( PU_PBAXCFG_SCOM                                         ,
                ULL(0x05016861), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PBAXRCVSTAT_OCI                                      ,
                ULL(0x40020120), SH_UNT          , SH_ACS_OCI       );
CONST_UINT64_T( PU_PBAXRCVSTAT_SCOM                                     ,
                ULL(0x05016864), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_PBAXSHBR0_OCI                                        ,
                ULL(0x40020130), SH_UNT          , SH_ACS_OCI       );
CONST_UINT64_T( PU_PBAXSHBR0_SCOM                                       ,
                ULL(0x05016866), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_PBAXSHBR1_OCI                                        ,
                ULL(0x40020150), SH_UNT          , SH_ACS_OCI       );
CONST_UINT64_T( PU_PBAXSHBR1_SCOM                                       ,
                ULL(0x0501686A), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_PBAXSHCS0_OCI                                        ,
                ULL(0x40020138), SH_UNT          , SH_ACS_OCI       );
CONST_UINT64_T( PU_PBAXSHCS0_SCOM                                       ,
                ULL(0x05016867), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PBAXSHCS1_OCI                                        ,
                ULL(0x40020158), SH_UNT          , SH_ACS_OCI       );
CONST_UINT64_T( PU_PBAXSHCS1_SCOM                                       ,
                ULL(0x0501686B), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PBAXSNDSTAT_OCI                                      ,
                ULL(0x40020110), SH_UNT          , SH_ACS_OCI       );
CONST_UINT64_T( PU_PBAXSNDSTAT_SCOM                                     ,
                ULL(0x05016862), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_PBAXSNDTX_OCI                                        ,
                ULL(0x40020100), SH_UNT          , SH_ACS_OCI       );
CONST_UINT64_T( PU_PBAXSNDTX_SCOM                                       ,
                ULL(0x05016860), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PEC_PBCQEINJ_REG                                        ,
                ULL(0x04010C02), SH_UNT_PEC      , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_0_PBCQEINJ_REG                                      ,
                ULL(0x04010C02), SH_UNT_PEC_0    , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_1_PBCQEINJ_REG                                      ,
                ULL(0x04011002), SH_UNT_PEC_1    , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_2_PBCQEINJ_REG                                      ,
                ULL(0x04011402), SH_UNT_PEC_2    , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PEC_PBCQHWCFG_REG                                       ,
                ULL(0x04010C00), SH_UNT_PEC      , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_0_PBCQHWCFG_REG                                     ,
                ULL(0x04010C00), SH_UNT_PEC_0    , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_1_PBCQHWCFG_REG                                     ,
                ULL(0x04011000), SH_UNT_PEC_1    , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_2_PBCQHWCFG_REG                                     ,
                ULL(0x04011400), SH_UNT_PEC_2    , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PEC_0_STACK0_PBCQMODE_REG                               ,
                ULL(0x04010C4D), SH_UNT_PEC_0_STACK0, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_0_STACK1_PBCQMODE_REG                               ,
                ULL(0x04010C8D), SH_UNT_PEC_0_STACK1, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_0_STACK2_PBCQMODE_REG                               ,
                ULL(0x04010CCD), SH_UNT_PEC_0_STACK2, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_1_STACK0_PBCQMODE_REG                               ,
                ULL(0x0401104D), SH_UNT_PEC_1_STACK0, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_1_STACK1_PBCQMODE_REG                               ,
                ULL(0x0401108D), SH_UNT_PEC_1_STACK1, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_1_STACK2_PBCQMODE_REG                               ,
                ULL(0x040110CD), SH_UNT_PEC_1_STACK2, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_2_STACK0_PBCQMODE_REG                               ,
                ULL(0x0401144D), SH_UNT_PEC_2_STACK0, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_2_STACK1_PBCQMODE_REG                               ,
                ULL(0x0401148D), SH_UNT_PEC_2_STACK1, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_2_STACK2_PBCQMODE_REG                               ,
                ULL(0x040114CD), SH_UNT_PEC_2_STACK2, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_STACK0_PBCQMODE_REG                                 ,
                ULL(0x04010C4D), SH_UNT_PEC_STACK0, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_STACK1_PBCQMODE_REG                                 ,
                ULL(0x04010C8D), SH_UNT_PEC_STACK1, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_STACK2_PBCQMODE_REG                                 ,
                ULL(0x04010CCD), SH_UNT_PEC_STACK2, SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_IOP0_PCS_M1_CONTROL_REG                              ,
                ULL(0x80000C010D010D3F), SH_UNT_PU_IOP0  , SH_ACS_SCOM      );
CONST_UINT64_T( PU_IOP1_PCS_M1_CONTROL_REG                              ,
                ULL(0x80000C010E010D3F), SH_UNT_PU_IOP1  , SH_ACS_SCOM      );
CONST_UINT64_T( PU_IOP2_PCS_M1_CONTROL_REG                              ,
                ULL(0x80000C010F010D3F), SH_UNT_PU_IOP2  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_IOP0_PCS_M2_CONTROL_REG                              ,
                ULL(0x80000C020D010E3F), SH_UNT_PU_IOP0  , SH_ACS_SCOM      );
CONST_UINT64_T( PU_IOP1_PCS_M2_CONTROL_REG                              ,
                ULL(0x80000C020E010E3F), SH_UNT_PU_IOP1  , SH_ACS_SCOM      );
CONST_UINT64_T( PU_IOP2_PCS_M2_CONTROL_REG                              ,
                ULL(0x80000C020F010E3F), SH_UNT_PU_IOP2  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_IOP0_PCS_M3_CONTROL_REG                              ,
                ULL(0x80000C030D010F3F), SH_UNT_PU_IOP0  , SH_ACS_SCOM      );
CONST_UINT64_T( PU_IOP1_PCS_M3_CONTROL_REG                              ,
                ULL(0x80000C030E010F3F), SH_UNT_PU_IOP1  , SH_ACS_SCOM      );
CONST_UINT64_T( PU_IOP2_PCS_M3_CONTROL_REG                              ,
                ULL(0x80000C030F010F3F), SH_UNT_PU_IOP2  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_IOP0_PCS_M4_CONTROL_REG                              ,
                ULL(0x80000C040D01103F), SH_UNT_PU_IOP0  , SH_ACS_SCOM      );
CONST_UINT64_T( PU_IOP1_PCS_M4_CONTROL_REG                              ,
                ULL(0x80000C040E01103F), SH_UNT_PU_IOP1  , SH_ACS_SCOM      );
CONST_UINT64_T( PU_IOP2_PCS_M4_CONTROL_REG                              ,
                ULL(0x80000C040F01103F), SH_UNT_PU_IOP2  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_IOP0_PCS_SYS_CONTROL_REG                             ,
                ULL(0x80000C000D010C3F), SH_UNT_PU_IOP0  , SH_ACS_SCOM      );
CONST_UINT64_T( PU_IOP1_PCS_SYS_CONTROL_REG                             ,
                ULL(0x80000C000E010C3F), SH_UNT_PU_IOP1  , SH_ACS_SCOM      );
CONST_UINT64_T( PU_IOP2_PCS_SYS_CONTROL_REG                             ,
                ULL(0x80000C000F010C3F), SH_UNT_PU_IOP2  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PBAIB0_PECAPP_CNTL_REG                               ,
                ULL(0x0D010800), SH_UNT_PU_PBAIB0, SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_PBAIB0_PECAPP_SEC_BAR                                ,
                ULL(0x0D010801), SH_UNT_PU_PBAIB0, SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU0_CTL_PERF_CONFIG                                 ,
                ULL(0x05011087), SH_UNT_PU_NPU0_CTL, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_CTL_PERF_CONFIG                                 ,
                ULL(0x05011187), SH_UNT_PU_NPU1_CTL, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_CTL_PERF_CONFIG                                 ,
                ULL(0x05011287), SH_UNT_PU_NPU2_CTL, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM0_PERF_CONFIG                                 ,
                ULL(0x05011007), SH_UNT_PU_NPU0_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM1_PERF_CONFIG                                 ,
                ULL(0x05011027), SH_UNT_PU_NPU0_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM2_PERF_CONFIG                                 ,
                ULL(0x05011047), SH_UNT_PU_NPU0_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM3_PERF_CONFIG                                 ,
                ULL(0x05011067), SH_UNT_PU_NPU0_SM3, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM0_PERF_CONFIG                                 ,
                ULL(0x05011107), SH_UNT_PU_NPU1_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM1_PERF_CONFIG                                 ,
                ULL(0x05011127), SH_UNT_PU_NPU1_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM2_PERF_CONFIG                                 ,
                ULL(0x05011147), SH_UNT_PU_NPU1_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM3_PERF_CONFIG                                 ,
                ULL(0x05011167), SH_UNT_PU_NPU1_SM3, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM0_PERF_CONFIG                                 ,
                ULL(0x05011207), SH_UNT_PU_NPU2_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM1_PERF_CONFIG                                 ,
                ULL(0x05011227), SH_UNT_PU_NPU2_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM2_PERF_CONFIG                                 ,
                ULL(0x05011247), SH_UNT_PU_NPU2_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM3_PERF_CONFIG                                 ,
                ULL(0x05011267), SH_UNT_PU_NPU2_SM3, SH_ACS_SCOM      );

CONST_UINT64_T( PEC_0_STACK0_PHBBAR_REG                                 ,
                ULL(0x04010C52), SH_UNT_PEC_0_STACK0, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_0_STACK1_PHBBAR_REG                                 ,
                ULL(0x04010C92), SH_UNT_PEC_0_STACK1, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_0_STACK2_PHBBAR_REG                                 ,
                ULL(0x04010CD2), SH_UNT_PEC_0_STACK2, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_1_STACK0_PHBBAR_REG                                 ,
                ULL(0x04011052), SH_UNT_PEC_1_STACK0, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_1_STACK1_PHBBAR_REG                                 ,
                ULL(0x04011092), SH_UNT_PEC_1_STACK1, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_1_STACK2_PHBBAR_REG                                 ,
                ULL(0x040110D2), SH_UNT_PEC_1_STACK2, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_2_STACK0_PHBBAR_REG                                 ,
                ULL(0x04011452), SH_UNT_PEC_2_STACK0, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_2_STACK1_PHBBAR_REG                                 ,
                ULL(0x04011492), SH_UNT_PEC_2_STACK1, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_2_STACK2_PHBBAR_REG                                 ,
                ULL(0x040114D2), SH_UNT_PEC_2_STACK2, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_STACK0_PHBBAR_REG                                   ,
                ULL(0x04010C52), SH_UNT_PEC_STACK0, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_STACK1_PHBBAR_REG                                   ,
                ULL(0x04010C92), SH_UNT_PEC_STACK1, SH_ACS_SCOM      );
CONST_UINT64_T( PEC_STACK2_PHBBAR_REG                                   ,
                ULL(0x04010CD2), SH_UNT_PEC_STACK2, SH_ACS_SCOM      );

CONST_UINT64_T( PU_PBAIB_STACK0_PHBRESET_REG                            ,
                ULL(0x0D010840), SH_UNT_PU_PBAIB_STACK0, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_PBAIB_STACK1_PHBRESET_REG                            ,
                ULL(0x0D010880), SH_UNT_PU_PBAIB_STACK1, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_PBAIB_STACK2_PHBRESET_REG                            ,
                ULL(0x0D0108C0), SH_UNT_PU_PBAIB_STACK2, SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_NPU0_SM0_PHY_BAR                                     ,
                ULL(0x05011010), SH_UNT_PU_NPU0_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM1_PHY_BAR                                     ,
                ULL(0x05011030), SH_UNT_PU_NPU0_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM2_PHY_BAR                                     ,
                ULL(0x05011050), SH_UNT_PU_NPU0_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM3_PHY_BAR                                     ,
                ULL(0x05011070), SH_UNT_PU_NPU0_SM3, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM0_PHY_BAR                                     ,
                ULL(0x05011110), SH_UNT_PU_NPU1_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM1_PHY_BAR                                     ,
                ULL(0x05011130), SH_UNT_PU_NPU1_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM2_PHY_BAR                                     ,
                ULL(0x05011150), SH_UNT_PU_NPU1_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM3_PHY_BAR                                     ,
                ULL(0x05011170), SH_UNT_PU_NPU1_SM3, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM0_PHY_BAR                                     ,
                ULL(0x05011210), SH_UNT_PU_NPU2_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM1_PHY_BAR                                     ,
                ULL(0x05011230), SH_UNT_PU_NPU2_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM2_PHY_BAR                                     ,
                ULL(0x05011250), SH_UNT_PU_NPU2_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM3_PHY_BAR                                     ,
                ULL(0x05011270), SH_UNT_PU_NPU2_SM3, SH_ACS_SCOM      );

CONST_UINT64_T( PU_PIBMEM_ADDRESS_REGISTER                              ,
                ULL(0x00088001), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PIBMEM_ADDRESS_REGISTER_FA                           ,
                ULL(0x00088007), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PIBMEM_CONTROL_REGISTER                              ,
                ULL(0x00088000), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PIBMEM_REPAIR_REGISTER_0                             ,
                ULL(0x0008800B), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PIBMEM_REPAIR_REGISTER_1                             ,
                ULL(0x0008800C), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PIBMEM_REPAIR_REGISTER_2                             ,
                ULL(0x0008800D), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PIBMEM_RESET_REGISTER                                ,
                ULL(0x00088006), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PIBMEM_STATUS_REG                                    ,
                ULL(0x00088005), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PIB_CMD_REG                                          ,
                ULL(0x05022831), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PIB_DATA_REG                                         ,
                ULL(0x05022832), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_PIB_RESET_REG                                        ,
                ULL(0x05022833), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PEC_PMONCTL_REG                                         ,
                ULL(0x04010C04), SH_UNT_PEC      , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_0_PMONCTL_REG                                       ,
                ULL(0x04010C04), SH_UNT_PEC_0    , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_1_PMONCTL_REG                                       ,
                ULL(0x04011004), SH_UNT_PEC_1    , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_2_PMONCTL_REG                                       ,
                ULL(0x04011404), SH_UNT_PEC_2    , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_PMU_CONTROL_2                                        ,
                ULL(0x020110D8), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PPE_XIDBGPRO                                         ,
                ULL(0x000E0005), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_GPE0_PPE_XIDBGPRO                                    ,
                ULL(0x00060015), SH_UNT_PU_GPE0  , SH_ACS_SCOM      );
CONST_UINT64_T( PU_GPE1_PPE_XIDBGPRO                                    ,
                ULL(0x00062015), SH_UNT_PU_GPE1  , SH_ACS_SCOM      );
CONST_UINT64_T( PU_GPE2_PPE_XIDBGPRO                                    ,
                ULL(0x00064015), SH_UNT_PU_GPE2  , SH_ACS_SCOM      );
CONST_UINT64_T( PU_GPE3_PPE_XIDBGPRO                                    ,
                ULL(0x00066015), SH_UNT_PU_GPE3  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PPE_XIRAMDBG                                         ,
                ULL(0x000E0003), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_GPE0_PPE_XIRAMDBG                                    ,
                ULL(0x00060013), SH_UNT_PU_GPE0  , SH_ACS_SCOM      );
CONST_UINT64_T( PU_GPE1_PPE_XIRAMDBG                                    ,
                ULL(0x00062013), SH_UNT_PU_GPE1  , SH_ACS_SCOM      );
CONST_UINT64_T( PU_GPE2_PPE_XIRAMDBG                                    ,
                ULL(0x00064013), SH_UNT_PU_GPE2  , SH_ACS_SCOM      );
CONST_UINT64_T( PU_GPE3_PPE_XIRAMDBG                                    ,
                ULL(0x00066013), SH_UNT_PU_GPE3  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PPE_XIRAMEDR                                         ,
                ULL(0x000E0004), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_GPE0_PPE_XIRAMEDR                                    ,
                ULL(0x00060014), SH_UNT_PU_GPE0  , SH_ACS_SCOM      );
CONST_UINT64_T( PU_GPE1_PPE_XIRAMEDR                                    ,
                ULL(0x00062014), SH_UNT_PU_GPE1  , SH_ACS_SCOM      );
CONST_UINT64_T( PU_GPE2_PPE_XIRAMEDR                                    ,
                ULL(0x00064014), SH_UNT_PU_GPE2  , SH_ACS_SCOM      );
CONST_UINT64_T( PU_GPE3_PPE_XIRAMEDR                                    ,
                ULL(0x00066014), SH_UNT_PU_GPE3  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PPE_XIRAMGA                                          ,
                ULL(0x000E0002), SH_UNT          , SH_ACS_SCOM_WO   );

CONST_UINT64_T( PU_GPE0_PPE_XIRAMGA                                     ,
                ULL(0x00060012), SH_UNT_PU_GPE0  , SH_ACS_SCOM_WO   );
CONST_UINT64_T( PU_GPE1_PPE_XIRAMGA                                     ,
                ULL(0x00062012), SH_UNT_PU_GPE1  , SH_ACS_SCOM_WO   );
CONST_UINT64_T( PU_GPE2_PPE_XIRAMGA                                     ,
                ULL(0x00064012), SH_UNT_PU_GPE2  , SH_ACS_SCOM_WO   );
CONST_UINT64_T( PU_GPE3_PPE_XIRAMGA                                     ,
                ULL(0x00066012), SH_UNT_PU_GPE3  , SH_ACS_SCOM_WO   );

CONST_UINT64_T( PU_PPE_XIRAMRA                                          ,
                ULL(0x000E0001), SH_UNT          , SH_ACS_SCOM_WO   );

CONST_UINT64_T( PU_GPE0_PPE_XIRAMRA                                     ,
                ULL(0x00060011), SH_UNT_PU_GPE0  , SH_ACS_SCOM_WO   );
CONST_UINT64_T( PU_GPE1_PPE_XIRAMRA                                     ,
                ULL(0x00062011), SH_UNT_PU_GPE1  , SH_ACS_SCOM_WO   );
CONST_UINT64_T( PU_GPE2_PPE_XIRAMRA                                     ,
                ULL(0x00064011), SH_UNT_PU_GPE2  , SH_ACS_SCOM_WO   );
CONST_UINT64_T( PU_GPE3_PPE_XIRAMRA                                     ,
                ULL(0x00066011), SH_UNT_PU_GPE3  , SH_ACS_SCOM_WO   );

CONST_UINT64_T( PU_PPE_XIXCR                                            ,
                ULL(0x000E0000), SH_UNT          , SH_ACS_SCOM_WO   );

CONST_UINT64_T( PU_GPE0_PPE_XIXCR                                       ,
                ULL(0x00060010), SH_UNT_PU_GPE0  , SH_ACS_SCOM_WO   );
CONST_UINT64_T( PU_GPE1_PPE_XIXCR                                       ,
                ULL(0x00062010), SH_UNT_PU_GPE1  , SH_ACS_SCOM_WO   );
CONST_UINT64_T( PU_GPE2_PPE_XIXCR                                       ,
                ULL(0x00064010), SH_UNT_PU_GPE2  , SH_ACS_SCOM_WO   );
CONST_UINT64_T( PU_GPE3_PPE_XIXCR                                       ,
                ULL(0x00066010), SH_UNT_PU_GPE3  , SH_ACS_SCOM_WO   );

CONST_UINT64_T( PEC_PREDV_REG                                           ,
                ULL(0x04010C06), SH_UNT_PEC      , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_0_PREDV_REG                                         ,
                ULL(0x04010C06), SH_UNT_PEC_0    , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_1_PREDV_REG                                         ,
                ULL(0x04011006), SH_UNT_PEC_1    , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_2_PREDV_REG                                         ,
                ULL(0x04011406), SH_UNT_PEC_2    , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_PRGM_REGISTER                                        ,
                ULL(0x00010009), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PROBE_PROTECT_STATUS                                 ,
                ULL(0x0001000A), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PSIHB_DEBUG_REG                                      ,
                ULL(0x05012911), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PSIHB_ERROR_MASK_REG                                 ,
                ULL(0x0501290F), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PSIHB_FIR_ACTION0_REG                                ,
                ULL(0x05012906), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_PSIHB_FIR_ACTION1_REG                                ,
                ULL(0x05012907), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_PSIHB_FIR_MASK_REG                                   ,
                ULL(0x05012903), SH_UNT          , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_PSIHB_FIR_MASK_REG_AND                               ,
                ULL(0x05012904), SH_UNT          , SH_ACS_SCOM1_AND );
CONST_UINT64_T( PU_PSIHB_FIR_MASK_REG_OR                                ,
                ULL(0x05012905), SH_UNT          , SH_ACS_SCOM2_OR  );

CONST_UINT64_T( PU_PSIHB_FIR_REG                                        ,
                ULL(0x05012900), SH_UNT          , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_PSIHB_FIR_REG_AND                                    ,
                ULL(0x05012901), SH_UNT          , SH_ACS_SCOM1_AND );
CONST_UINT64_T( PU_PSIHB_FIR_REG_OR                                     ,
                ULL(0x05012902), SH_UNT          , SH_ACS_SCOM2_OR  );

CONST_UINT64_T( PU_PSIHB_INTERRUPT_CONTROL                              ,
                ULL(0x05012915), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PSIHB_INTERRUPT_STATUS                               ,
                ULL(0x05012919), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PSIHB_STATUS_CTL_REG_SCOM                            ,
                ULL(0x0501290E), SH_UNT          , SH_ACS_SCOM      );
CONST_UINT64_T( PU_PSIHB_STATUS_CTL_REG_SCOM1                           ,
                ULL(0x05012912), SH_UNT          , SH_ACS_SCOM1     );
CONST_UINT64_T( PU_PSIHB_STATUS_CTL_REG_SCOM2                           ,
                ULL(0x05012913), SH_UNT          , SH_ACS_SCOM2     );

CONST_UINT64_T( PU_PSI_BRIDGE_BAR_REG                                   ,
                ULL(0x0501290A), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PSI_BRIDGE_FSP_BAR_REG                               ,
                ULL(0x0501290B), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PSI_FSP_MMR_REG                                      ,
                ULL(0x0501290C), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PSI_TCE_ADDR_REG                                     ,
                ULL(0x05015F44), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PSU_HOST_SBE_MBOX0_REG                               ,
                ULL(0x000D0050), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PSU_HOST_SBE_MBOX1_REG                               ,
                ULL(0x000D0051), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PSU_HOST_SBE_MBOX2_REG                               ,
                ULL(0x000D0052), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PSU_HOST_SBE_MBOX3_REG                               ,
                ULL(0x000D0053), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PSU_HOST_SBE_MBOX4_REG                               ,
                ULL(0x000D0054), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PSU_HOST_SBE_MBOX5_REG                               ,
                ULL(0x000D0055), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PSU_HOST_SBE_MBOX6_REG                               ,
                ULL(0x000D0056), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PSU_HOST_SBE_MBOX7_REG                               ,
                ULL(0x000D0057), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PSU_INSTR0_ACTCYCLECNT_REG                           ,
                ULL(0x000D0023), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PSU_INSTR0_CYCLECNT_REG                              ,
                ULL(0x000D0022), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PSU_INSTR0_EVENTCNT_REG                              ,
                ULL(0x000D0024), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PSU_INSTR0_FILTER_REG                                ,
                ULL(0x000D0021), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PSU_INSTR0_MAXCYCLECNT_REG                           ,
                ULL(0x000D0025), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PSU_INSTR0_MINCYCLECNT_REG                           ,
                ULL(0x000D0026), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PSU_INSTR0_STOP_TIMER_REG                            ,
                ULL(0x000D0020), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PSU_INSTR1_ACTCYCLECNT_REG                           ,
                ULL(0x000D0033), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PSU_INSTR1_CYCLECNT_REG                              ,
                ULL(0x000D0032), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PSU_INSTR1_EVENTCNT_REG                              ,
                ULL(0x000D0034), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PSU_INSTR1_FILTER_REG                                ,
                ULL(0x000D0031), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PSU_INSTR1_MAXCYCLECNT_REG                           ,
                ULL(0x000D0035), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PSU_INSTR1_MINCYCLECNT_REG                           ,
                ULL(0x000D0036), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PSU_INSTR1_STOP_TIMER_REG                            ,
                ULL(0x000D0030), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PSU_INSTR2_ACTCYCLECNT_REG                           ,
                ULL(0x000D0043), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PSU_INSTR2_CYCLECNT_REG                              ,
                ULL(0x000D0042), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PSU_INSTR2_EVENTCNT_REG                              ,
                ULL(0x000D0044), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PSU_INSTR2_FILTER_REG                                ,
                ULL(0x000D0041), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PSU_INSTR2_MAXCYCLECNT_REG                           ,
                ULL(0x000D0045), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PSU_INSTR2_MINCYCLECNT_REG                           ,
                ULL(0x000D0046), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PSU_INSTR2_STOP_TIMER_REG                            ,
                ULL(0x000D0040), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PSU_INSTR_CTRL_STATUS_REG                            ,
                ULL(0x000D0010), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PSU_PIBHIST_2NDLAST_ADDR_TRACE_REG                   ,
                ULL(0x000D0005), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PSU_PIBHIST_2NDLAST_REQDATA_TRACE_REG                ,
                ULL(0x000D0006), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PSU_PIBHIST_2NDLAST_RSPDATA_TRACE_REG                ,
                ULL(0x000D0007), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PSU_PIBHIST_CTRL_STATUS_REG                          ,
                ULL(0x000D0000), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PSU_PIBHIST_FILTER_REG                               ,
                ULL(0x000D0001), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PSU_PIBHIST_LAST_ADDR_TRACE_REG                      ,
                ULL(0x000D0002), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PSU_PIBHIST_LAST_REQDATA_TRACE_REG                   ,
                ULL(0x000D0003), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PSU_PIBHIST_LAST_RSPDATA_TRACE_REG                   ,
                ULL(0x000D0004), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PSU_SBE_DOORBELL0_REG                                ,
                ULL(0x000D0060), SH_UNT          , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_PSU_SBE_DOORBELL0_REG_AND                            ,
                ULL(0x000D0061), SH_UNT          , SH_ACS_SCOM1_AND );
CONST_UINT64_T( PU_PSU_SBE_DOORBELL0_REG_OR                             ,
                ULL(0x000D0062), SH_UNT          , SH_ACS_SCOM2_OR  );

CONST_UINT64_T( PU_PSU_SBE_DOORBELL1_REG                                ,
                ULL(0x000D0063), SH_UNT          , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_PSU_SBE_DOORBELL1_REG_AND                            ,
                ULL(0x000D0064), SH_UNT          , SH_ACS_SCOM1_AND );
CONST_UINT64_T( PU_PSU_SBE_DOORBELL1_REG_OR                             ,
                ULL(0x000D0065), SH_UNT          , SH_ACS_SCOM2_OR  );

CONST_UINT64_T( PU_RCV_ERRLOG0_REG                                      ,
                ULL(0x05022822), SH_UNT          , SH_ACS_SCOM_WAND );

CONST_UINT64_T( PU_RCV_ERRLOG1_REG                                      ,
                ULL(0x05022823), SH_UNT          , SH_ACS_SCOM_WAND );

CONST_UINT64_T( PU_NPU0_SM0_RELAXED_CMD                                 ,
                ULL(0x05011006), SH_UNT_PU_NPU0_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM1_RELAXED_CMD                                 ,
                ULL(0x05011026), SH_UNT_PU_NPU0_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM2_RELAXED_CMD                                 ,
                ULL(0x05011046), SH_UNT_PU_NPU0_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM3_RELAXED_CMD                                 ,
                ULL(0x05011066), SH_UNT_PU_NPU0_SM3, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM0_RELAXED_CMD                                 ,
                ULL(0x05011106), SH_UNT_PU_NPU1_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM1_RELAXED_CMD                                 ,
                ULL(0x05011126), SH_UNT_PU_NPU1_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM2_RELAXED_CMD                                 ,
                ULL(0x05011146), SH_UNT_PU_NPU1_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM3_RELAXED_CMD                                 ,
                ULL(0x05011166), SH_UNT_PU_NPU1_SM3, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM0_RELAXED_CMD                                 ,
                ULL(0x05011206), SH_UNT_PU_NPU2_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM1_RELAXED_CMD                                 ,
                ULL(0x05011226), SH_UNT_PU_NPU2_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM2_RELAXED_CMD                                 ,
                ULL(0x05011246), SH_UNT_PU_NPU2_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM3_RELAXED_CMD                                 ,
                ULL(0x05011266), SH_UNT_PU_NPU2_SM3, SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU0_SM0_RELAXED_SOURCE                              ,
                ULL(0x05011005), SH_UNT_PU_NPU0_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM1_RELAXED_SOURCE                              ,
                ULL(0x05011025), SH_UNT_PU_NPU0_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM2_RELAXED_SOURCE                              ,
                ULL(0x05011045), SH_UNT_PU_NPU0_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM3_RELAXED_SOURCE                              ,
                ULL(0x05011065), SH_UNT_PU_NPU0_SM3, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM0_RELAXED_SOURCE                              ,
                ULL(0x05011105), SH_UNT_PU_NPU1_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM1_RELAXED_SOURCE                              ,
                ULL(0x05011125), SH_UNT_PU_NPU1_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM2_RELAXED_SOURCE                              ,
                ULL(0x05011145), SH_UNT_PU_NPU1_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM3_RELAXED_SOURCE                              ,
                ULL(0x05011165), SH_UNT_PU_NPU1_SM3, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM0_RELAXED_SOURCE                              ,
                ULL(0x05011205), SH_UNT_PU_NPU2_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM1_RELAXED_SOURCE                              ,
                ULL(0x05011225), SH_UNT_PU_NPU2_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM2_RELAXED_SOURCE                              ,
                ULL(0x05011245), SH_UNT_PU_NPU2_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM3_RELAXED_SOURCE                              ,
                ULL(0x05011265), SH_UNT_PU_NPU2_SM3, SH_ACS_SCOM      );

CONST_UINT64_T( PU_PIB2OPB0_RESET                                       ,
                ULL(0x00020004), SH_UNT_PU_PIB2OPB0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_PIB2OPB1_RESET                                       ,
                ULL(0x00020014), SH_UNT_PU_PIB2OPB1, SH_ACS_SCOM      );

// ERROR: ALREADY DEFINED !  000A0001, _SCOM ::   CONST_UINT64_T( PU_RESET_REGISTER_B                                     , ULL(0x000A0001), SH_UNT          , SH_ACS_SCOM      );

// ERROR: ALREADY DEFINED !  000A1001, _SCOM ::   CONST_UINT64_T( PU_RESET_REGISTER_C                                     , ULL(0x000A1001), SH_UNT          , SH_ACS_SCOM      );

// ERROR: ALREADY DEFINED !  000A2001, _SCOM ::   CONST_UINT64_T( PU_RESET_REGISTER_D                                     , ULL(0x000A2001), SH_UNT          , SH_ACS_SCOM      );

// ERROR: ALREADY DEFINED !  000A3001, _SCOM ::   CONST_UINT64_T( PU_RESET_REGISTER_E                                     , ULL(0x000A3001), SH_UNT          , SH_ACS_SCOM      );

// ERROR: ALREADY DEFINED !  000A000D, _SCOM ::   CONST_UINT64_T( PU_RESIDUAL_FRONT_END_BACK_END_LENGTH_B                 , ULL(0x000A000D), SH_UNT          , SH_ACS_SCOM      );

// ERROR: ALREADY DEFINED !  000A100D, _SCOM ::   CONST_UINT64_T( PU_RESIDUAL_FRONT_END_BACK_END_LENGTH_C                 , ULL(0x000A100D), SH_UNT          , SH_ACS_SCOM      );

// ERROR: ALREADY DEFINED !  000A200D, _SCOM ::   CONST_UINT64_T( PU_RESIDUAL_FRONT_END_BACK_END_LENGTH_D                 , ULL(0x000A200D), SH_UNT          , SH_ACS_SCOM      );

// ERROR: ALREADY DEFINED !  000A300D, _SCOM ::   CONST_UINT64_T( PU_RESIDUAL_FRONT_END_BACK_END_LENGTH_E                 , ULL(0x000A300D), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU_CTL_RLX_CONFIG                                   ,
                ULL(0x05011381), SH_UNT_PU_NPU_CTL, SH_ACS_SCOM      );

CONST_UINT64_T( PU_RNG_FAILED_INT                                       ,
                ULL(0x020110E7), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PIB2OPB0_RSIC                                        ,
                ULL(0x00020008), SH_UNT_PU_PIB2OPB0, SH_ACS_SCOM_WCLEAR );
CONST_UINT64_T( PU_PIB2OPB1_RSIC                                        ,
                ULL(0x00020018), SH_UNT_PU_PIB2OPB1, SH_ACS_SCOM_WCLEAR );

CONST_UINT64_T( PU_PIB2OPB0_RSIM                                        ,
                ULL(0x00020009), SH_UNT_PU_PIB2OPB0, SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_PIB2OPB1_RSIM                                        ,
                ULL(0x00020019), SH_UNT_PU_PIB2OPB1, SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_PIB2OPB0_RSIS                                        ,
                ULL(0x0002000A), SH_UNT_PU_PIB2OPB0, SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_PIB2OPB1_RSIS                                        ,
                ULL(0x0002001A), SH_UNT_PU_PIB2OPB1, SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_RX_CH_FSM_REG                                        ,
                ULL(0x0501280D), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_RX_CH_INTADDR_REG                                    ,
                ULL(0x05012818), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_RX_CH_MISC_REG                                       ,
                ULL(0x0501281B), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_RX_CTRL_STAT_REG                                     ,
                ULL(0x05012808), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_RX_DBFF_REG0                                         ,
                ULL(0x05012819), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_RX_DBFF_REG1                                         ,
                ULL(0x0501281A), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_RX_DF_FSM_REG                                        ,
                ULL(0x0501280E), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_RX_ERROR_REG_WCLEAR                                  ,
                ULL(0x0501280A), SH_UNT          , SH_ACS_SCOM_WCLEAR );
CONST_UINT64_T( PU_RX_ERROR_REG_OR                                      ,
                ULL(0x0501280C), SH_UNT          , SH_ACS_SCOM1_OR  );

CONST_UINT64_T( PU_RX_ERR_MODE                                          ,
                ULL(0x0501280F), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_RX_MASK_REG                                          ,
                ULL(0x0501280B), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_RX_PSI_CNTL                                          ,
                ULL(0x06010820), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_RX_PSI_MODE                                          ,
                ULL(0x06010821), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_RX_PSI_STATUS                                        ,
                ULL(0x06010822), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU0_SCRATCH0                                        ,
                ULL(0x050110AB), SH_UNT_PU_NPU0  , SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SCRATCH0                                        ,
                ULL(0x050111AB), SH_UNT_PU_NPU1  , SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SCRATCH0                                        ,
                ULL(0x050112AB), SH_UNT_PU_NPU2  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU0_DAT_SCRATCH1                                    ,
                ULL(0x050110BC), SH_UNT_PU_NPU0_DAT, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_DAT_SCRATCH1                                    ,
                ULL(0x050111BC), SH_UNT_PU_NPU1_DAT, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_DAT_SCRATCH1                                    ,
                ULL(0x050112BC), SH_UNT_PU_NPU2_DAT, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_NTL0_SCRATCH1                                   ,
                ULL(0x050110D4), SH_UNT_PU_NPU0_NTL0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_NTL1_SCRATCH1                                   ,
                ULL(0x050110F4), SH_UNT_PU_NPU0_NTL1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_NTL0_SCRATCH1                                   ,
                ULL(0x050111D4), SH_UNT_PU_NPU1_NTL0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_NTL1_SCRATCH1                                   ,
                ULL(0x050111F4), SH_UNT_PU_NPU1_NTL1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_NTL0_SCRATCH1                                   ,
                ULL(0x050112D4), SH_UNT_PU_NPU2_NTL0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_NTL1_SCRATCH1                                   ,
                ULL(0x050112F4), SH_UNT_PU_NPU2_NTL1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU0_NTL0_SCRATCH2                                   ,
                ULL(0x050110CC), SH_UNT_PU_NPU0_NTL0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_NTL1_SCRATCH2                                   ,
                ULL(0x050110EC), SH_UNT_PU_NPU0_NTL1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_NTL0_SCRATCH2                                   ,
                ULL(0x050111CC), SH_UNT_PU_NPU1_NTL0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_NTL1_SCRATCH2                                   ,
                ULL(0x050111EC), SH_UNT_PU_NPU1_NTL1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_NTL0_SCRATCH2                                   ,
                ULL(0x050112CC), SH_UNT_PU_NPU2_NTL0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_NTL1_SCRATCH2                                   ,
                ULL(0x050112EC), SH_UNT_PU_NPU2_NTL1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU0_NTL0_SCRATCH3                                   ,
                ULL(0x050110CD), SH_UNT_PU_NPU0_NTL0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_NTL1_SCRATCH3                                   ,
                ULL(0x050110ED), SH_UNT_PU_NPU0_NTL1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_NTL0_SCRATCH3                                   ,
                ULL(0x050111CD), SH_UNT_PU_NPU1_NTL0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_NTL1_SCRATCH3                                   ,
                ULL(0x050111ED), SH_UNT_PU_NPU1_NTL1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_NTL0_SCRATCH3                                   ,
                ULL(0x050112CD), SH_UNT_PU_NPU2_NTL0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_NTL1_SCRATCH3                                   ,
                ULL(0x050112ED), SH_UNT_PU_NPU2_NTL1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_SECURITY_SWITCH_REGISTER_WOR                         ,
                ULL(0x00010005), SH_UNT          , SH_ACS_SCOM_WOR  );
CONST_UINT64_T( PU_SECURITY_SWITCH_REGISTER_CLEAR                       ,
                ULL(0x00010006), SH_UNT          , SH_ACS_SCOM1_CLEAR );

CONST_UINT64_T( PU_SEND_WC_BASE_ADDR                                    ,
                ULL(0x020110D2), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU0_SM0_SM_HOLD                                     ,
                ULL(0x05011017), SH_UNT_PU_NPU0_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM1_SM_HOLD                                     ,
                ULL(0x05011037), SH_UNT_PU_NPU0_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM2_SM_HOLD                                     ,
                ULL(0x05011057), SH_UNT_PU_NPU0_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM3_SM_HOLD                                     ,
                ULL(0x05011077), SH_UNT_PU_NPU0_SM3, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM0_SM_HOLD                                     ,
                ULL(0x05011117), SH_UNT_PU_NPU1_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM1_SM_HOLD                                     ,
                ULL(0x05011137), SH_UNT_PU_NPU1_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM2_SM_HOLD                                     ,
                ULL(0x05011157), SH_UNT_PU_NPU1_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM3_SM_HOLD                                     ,
                ULL(0x05011177), SH_UNT_PU_NPU1_SM3, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM0_SM_HOLD                                     ,
                ULL(0x05011217), SH_UNT_PU_NPU2_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM1_SM_HOLD                                     ,
                ULL(0x05011237), SH_UNT_PU_NPU2_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM2_SM_HOLD                                     ,
                ULL(0x05011257), SH_UNT_PU_NPU2_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM3_SM_HOLD                                     ,
                ULL(0x05011277), SH_UNT_PU_NPU2_SM3, SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU0_SM0_SM_MASK                                     ,
                ULL(0x05011018), SH_UNT_PU_NPU0_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM1_SM_MASK                                     ,
                ULL(0x05011038), SH_UNT_PU_NPU0_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM2_SM_MASK                                     ,
                ULL(0x05011058), SH_UNT_PU_NPU0_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM3_SM_MASK                                     ,
                ULL(0x05011078), SH_UNT_PU_NPU0_SM3, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM0_SM_MASK                                     ,
                ULL(0x05011118), SH_UNT_PU_NPU1_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM1_SM_MASK                                     ,
                ULL(0x05011138), SH_UNT_PU_NPU1_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM2_SM_MASK                                     ,
                ULL(0x05011158), SH_UNT_PU_NPU1_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM3_SM_MASK                                     ,
                ULL(0x05011178), SH_UNT_PU_NPU1_SM3, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM0_SM_MASK                                     ,
                ULL(0x05011218), SH_UNT_PU_NPU2_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM1_SM_MASK                                     ,
                ULL(0x05011238), SH_UNT_PU_NPU2_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM2_SM_MASK                                     ,
                ULL(0x05011258), SH_UNT_PU_NPU2_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM3_SM_MASK                                     ,
                ULL(0x05011278), SH_UNT_PU_NPU2_SM3, SH_ACS_SCOM      );

CONST_UINT64_T( PU_SND_MODE_REG                                         ,
                ULL(0x05022821), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_SND_STAT_REG                                         ,
                ULL(0x05022820), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_SPIMPSS_ADC_CTRL_REG0                                ,
                ULL(0x00070000), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_SPIPSS_100NS_REG                                     ,
                ULL(0x00070028), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_SPIPSS_ADC_CMD_REG                                   ,
                ULL(0x00070004), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_SPIPSS_ADC_CTRL_REG1                                 ,
                ULL(0x00070001), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_SPIPSS_ADC_CTRL_REG2                                 ,
                ULL(0x00070002), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_SPIPSS_ADC_RDATA_REG0                                ,
                ULL(0x00070020), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_SPIPSS_ADC_RDATA_REG1                                ,
                ULL(0x00070021), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_SPIPSS_ADC_RDATA_REG2                                ,
                ULL(0x00070022), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_SPIPSS_ADC_RDATA_REG3                                ,
                ULL(0x00070023), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_SPIPSS_ADC_RESET_REGISTER                            ,
                ULL(0x00070005), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_SPIPSS_ADC_STATUS_REG                                ,
                ULL(0x00070003), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_SPIPSS_ADC_WDATA_REG                                 ,
                ULL(0x00070010), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_SPIPSS_P2S_COMMAND_REG                               ,
                ULL(0x00070044), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_SPIPSS_P2S_CTRL_REG0                                 ,
                ULL(0x00070040), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_SPIPSS_P2S_CTRL_REG1                                 ,
                ULL(0x00070041), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_SPIPSS_P2S_CTRL_REG2                                 ,
                ULL(0x00070042), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_SPIPSS_P2S_RDATA_REG                                 ,
                ULL(0x00070060), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_SPIPSS_P2S_RESET_REGISTER                            ,
                ULL(0x00070045), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_SPIPSS_P2S_STATUS_REG                                ,
                ULL(0x00070043), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_SPIPSS_P2S_WDATA_REG                                 ,
                ULL(0x00070050), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_GPE5_SRAM_SRBV0_OCI                                  ,
                ULL(0xC0050020), SH_UNT_PU_GPE5  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE5_SRAM_SRBV0_SCOM                                 ,
                ULL(0x0006A004), SH_UNT_PU_GPE5  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE5_SRAM_SRBV1_OCI                                  ,
                ULL(0xC0050028), SH_UNT_PU_GPE5  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE5_SRAM_SRBV1_SCOM                                 ,
                ULL(0x0006A005), SH_UNT_PU_GPE5  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE5_SRAM_SRBV2_OCI                                  ,
                ULL(0xC0050030), SH_UNT_PU_GPE5  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE5_SRAM_SRBV2_SCOM                                 ,
                ULL(0x0006A006), SH_UNT_PU_GPE5  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE5_SRAM_SRBV3_OCI                                  ,
                ULL(0xC0050038), SH_UNT_PU_GPE5  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE5_SRAM_SRBV3_SCOM                                 ,
                ULL(0x0006A007), SH_UNT_PU_GPE5  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE5_SRAM_SRCHSW_OCI                                 ,
                ULL(0xC0050040), SH_UNT_PU_GPE5  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE5_SRAM_SRCHSW_SCOM                                ,
                ULL(0x0006A008), SH_UNT_PU_GPE5  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE5_SRAM_SREAR_OCI                                  ,
                ULL(0xC0050018), SH_UNT_PU_GPE5  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE5_SRAM_SREAR_SCOM                                 ,
                ULL(0x0006A003), SH_UNT_PU_GPE5  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE5_SRAM_SRMAP_OCI                                  ,
                ULL(0xC0050010), SH_UNT_PU_GPE5  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE5_SRAM_SRMAP_SCOM                                 ,
                ULL(0x0006A002), SH_UNT_PU_GPE5  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_GPE5_SRAM_SRMR_OCI                                   ,
                ULL(0xC0050008), SH_UNT_PU_GPE5  , SH_ACS_OCI       );
CONST_UINT64_T( PU_GPE5_SRAM_SRMR_SCOM                                  ,
                ULL(0x0006A001), SH_UNT_PU_GPE5  , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_STATUS_REGISTER                                      ,
                ULL(0x00010002), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_STATUS_REGISTER_B                                    ,
                ULL(0x000A0002), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_STATUS_REGISTER_C                                    ,
                ULL(0x000A1002), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_STATUS_REGISTER_D                                    ,
                ULL(0x000A2002), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_STATUS_REGISTER_E                                    ,
                ULL(0x000A3002), SH_UNT          , SH_ACS_SCOM      );

// ERROR: ALREADY DEFINED !  000A000B, _SCOM ::   CONST_UINT64_T( PU_STATUS_REGISTER_ENGINE_B                             , ULL(0x000A000B), SH_UNT          , SH_ACS_SCOM      );

// ERROR: ALREADY DEFINED !  000A100B, _SCOM ::   CONST_UINT64_T( PU_STATUS_REGISTER_ENGINE_C                             , ULL(0x000A100B), SH_UNT          , SH_ACS_SCOM      );

// ERROR: ALREADY DEFINED !  000A200B, _SCOM ::   CONST_UINT64_T( PU_STATUS_REGISTER_ENGINE_D                             , ULL(0x000A200B), SH_UNT          , SH_ACS_SCOM      );

// ERROR: ALREADY DEFINED !  000A300B, _SCOM ::   CONST_UINT64_T( PU_STATUS_REGISTER_ENGINE_E                             , ULL(0x000A300B), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_PIB2OPB0_STAT_RDDAT_ERRES                            ,
                ULL(0x00020001), SH_UNT_PU_PIB2OPB0, SH_ACS_SCOM_RO   );
CONST_UINT64_T( PU_PIB2OPB1_STAT_RDDAT_ERRES                            ,
                ULL(0x00020011), SH_UNT_PU_PIB2OPB1, SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_SU_CH0_ABORT_CSB                                     ,
                ULL(0x02011043), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_SU_CH1_ABORT_CSB                                     ,
                ULL(0x02011045), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_SU_CH2_ABORT_CSB                                     ,
                ULL(0x02011047), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_SU_CH3_ABORT_CSB                                     ,
                ULL(0x02011049), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_SU_CH4_ABORT_CSB                                     ,
                ULL(0x0201104B), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_SU_CRB_KILL_REQ                                      ,
                ULL(0x02011053), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_SU_DMA_ERROR_REPORT_0                                ,
                ULL(0x02011057), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_SU_DMA_ERROR_REPORT_1                                ,
                ULL(0x02011058), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_SU_ENGINE_ENABLE                                     ,
                ULL(0x02011041), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_SU_ERAT_ERROR_RPT                                    ,
                ULL(0x020110D7), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_SU_INBOUND_WRITE_CONTROL                             ,
                ULL(0x02011042), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_SU_MARKER_TRACE_CONTROL                              ,
                ULL(0x02011056), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_SU_PERFMON_CONTROL_0                                 ,
                ULL(0x02011054), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_SU_PERFMON_CONTROL_1                                 ,
                ULL(0x02011055), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_SU_STATUS                                            ,
                ULL(0x02011040), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_SU_UMAC_ERROR_RPT                                    ,
                ULL(0x020110D3), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_SYM_HI_PRIOR_RCV_FIFO_ASB                            ,
                ULL(0x020110C7), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_SYM_HI_PRIOR_RCV_FIFO_BAR                            ,
                ULL(0x020110C1), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_SYM_HI_PRIOR_RCV_FIFO_CNTL                           ,
                ULL(0x020110C4), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_SYM_LO_PRIOR_RCV_FIFO_ASB                            ,
                ULL(0x020110D0), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_SYM_LO_PRIOR_RCV_FIFO_BAR                            ,
                ULL(0x020110CA), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_SYM_LO_PRIOR_RCV_FIFO_CNTL                           ,
                ULL(0x020110CD), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_SYM_MAX_BYTE_CNT                                     ,
                ULL(0x0201105A), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_SYNC_FIR_ACTION0_REG                                 ,
                ULL(0x050129C6), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_SYNC_FIR_ACTION1_REG                                 ,
                ULL(0x050129C7), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_SYNC_FIR_MASK_REG                                    ,
                ULL(0x050129C3), SH_UNT          , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_SYNC_FIR_MASK_REG_AND                                ,
                ULL(0x050129C4), SH_UNT          , SH_ACS_SCOM1_AND );
CONST_UINT64_T( PU_SYNC_FIR_MASK_REG_OR                                 ,
                ULL(0x050129C5), SH_UNT          , SH_ACS_SCOM2_OR  );

CONST_UINT64_T( PU_SYNC_FIR_REG                                         ,
                ULL(0x050129C0), SH_UNT          , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_SYNC_FIR_REG_AND                                     ,
                ULL(0x050129C1), SH_UNT          , SH_ACS_SCOM1_AND );
CONST_UINT64_T( PU_SYNC_FIR_REG_OR                                      ,
                ULL(0x050129C2), SH_UNT          , SH_ACS_SCOM2_OR  );

CONST_UINT64_T( PU_NPU_SM1_TCE_KILL                                     ,
                ULL(0x05011320), SH_UNT_PU_NPU_SM1, SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCMC01_FAST_TRA0_TRACE_HI_DATA_REG                   ,
                ULL(0x07010C00), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCMC01_FAST_TRA0_TRACE_LO_DATA_REG                   ,
                ULL(0x07010C01), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCMC01_FAST_TRA0_TRACE_TRCTRL_CONFIG                 ,
                ULL(0x07010C02), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCMC01_FAST_TRA0_TRACE_TRDATA_CONFIG_0               ,
                ULL(0x07010C03), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCMC01_FAST_TRA0_TRACE_TRDATA_CONFIG_1               ,
                ULL(0x07010C04), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCMC01_FAST_TRA0_TRACE_TRDATA_CONFIG_2               ,
                ULL(0x07010C05), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCMC01_FAST_TRA0_TRACE_TRDATA_CONFIG_3               ,
                ULL(0x07010C06), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCMC01_FAST_TRA0_TRACE_TRDATA_CONFIG_4               ,
                ULL(0x07010C07), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCMC01_FAST_TRA0_TRACE_TRDATA_CONFIG_5               ,
                ULL(0x07010C08), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCMC01_FAST_TRA0_TRACE_TRDATA_CONFIG_9               ,
                ULL(0x07010C09), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCMC01_FAST_TRA1_TRACE_HI_DATA_REG                   ,
                ULL(0x07010C40), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCMC01_FAST_TRA1_TRACE_LO_DATA_REG                   ,
                ULL(0x07010C41), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCMC01_FAST_TRA1_TRACE_TRCTRL_CONFIG                 ,
                ULL(0x07010C42), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCMC01_FAST_TRA1_TRACE_TRDATA_CONFIG_0               ,
                ULL(0x07010C43), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCMC01_FAST_TRA1_TRACE_TRDATA_CONFIG_1               ,
                ULL(0x07010C44), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCMC01_FAST_TRA1_TRACE_TRDATA_CONFIG_2               ,
                ULL(0x07010C45), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCMC01_FAST_TRA1_TRACE_TRDATA_CONFIG_3               ,
                ULL(0x07010C46), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCMC01_FAST_TRA1_TRACE_TRDATA_CONFIG_4               ,
                ULL(0x07010C47), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCMC01_FAST_TRA1_TRACE_TRDATA_CONFIG_5               ,
                ULL(0x07010C48), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCMC01_FAST_TRA1_TRACE_TRDATA_CONFIG_9               ,
                ULL(0x07010C49), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCMC01_SLOW_TRA0_TRACE_HI_DATA_REG                   ,
                ULL(0x07010400), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCMC01_SLOW_TRA0_TRACE_LO_DATA_REG                   ,
                ULL(0x07010401), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCMC01_SLOW_TRA0_TRACE_TRCTRL_CONFIG                 ,
                ULL(0x07010402), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCMC01_SLOW_TRA0_TRACE_TRDATA_CONFIG_0               ,
                ULL(0x07010403), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCMC01_SLOW_TRA0_TRACE_TRDATA_CONFIG_1               ,
                ULL(0x07010404), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCMC01_SLOW_TRA0_TRACE_TRDATA_CONFIG_2               ,
                ULL(0x07010405), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCMC01_SLOW_TRA0_TRACE_TRDATA_CONFIG_3               ,
                ULL(0x07010406), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCMC01_SLOW_TRA0_TRACE_TRDATA_CONFIG_4               ,
                ULL(0x07010407), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCMC01_SLOW_TRA0_TRACE_TRDATA_CONFIG_5               ,
                ULL(0x07010408), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCMC01_SLOW_TRA0_TRACE_TRDATA_CONFIG_9               ,
                ULL(0x07010409), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCMC23_FAST_TRA0_TRACE_HI_DATA_REG                   ,
                ULL(0x08010C00), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCMC23_FAST_TRA0_TRACE_LO_DATA_REG                   ,
                ULL(0x08010C01), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCMC23_FAST_TRA0_TRACE_TRCTRL_CONFIG                 ,
                ULL(0x08010C02), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCMC23_FAST_TRA0_TRACE_TRDATA_CONFIG_0               ,
                ULL(0x08010C03), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCMC23_FAST_TRA0_TRACE_TRDATA_CONFIG_1               ,
                ULL(0x08010C04), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCMC23_FAST_TRA0_TRACE_TRDATA_CONFIG_2               ,
                ULL(0x08010C05), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCMC23_FAST_TRA0_TRACE_TRDATA_CONFIG_3               ,
                ULL(0x08010C06), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCMC23_FAST_TRA0_TRACE_TRDATA_CONFIG_4               ,
                ULL(0x08010C07), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCMC23_FAST_TRA0_TRACE_TRDATA_CONFIG_5               ,
                ULL(0x08010C08), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCMC23_FAST_TRA0_TRACE_TRDATA_CONFIG_9               ,
                ULL(0x08010C09), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCMC23_FAST_TRA1_TRACE_HI_DATA_REG                   ,
                ULL(0x08010C40), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCMC23_FAST_TRA1_TRACE_LO_DATA_REG                   ,
                ULL(0x08010C41), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCMC23_FAST_TRA1_TRACE_TRCTRL_CONFIG                 ,
                ULL(0x08010C42), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCMC23_FAST_TRA1_TRACE_TRDATA_CONFIG_0               ,
                ULL(0x08010C43), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCMC23_FAST_TRA1_TRACE_TRDATA_CONFIG_1               ,
                ULL(0x08010C44), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCMC23_FAST_TRA1_TRACE_TRDATA_CONFIG_2               ,
                ULL(0x08010C45), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCMC23_FAST_TRA1_TRACE_TRDATA_CONFIG_3               ,
                ULL(0x08010C46), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCMC23_FAST_TRA1_TRACE_TRDATA_CONFIG_4               ,
                ULL(0x08010C47), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCMC23_FAST_TRA1_TRACE_TRDATA_CONFIG_5               ,
                ULL(0x08010C48), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCMC23_FAST_TRA1_TRACE_TRDATA_CONFIG_9               ,
                ULL(0x08010C49), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCMC23_SLOW_TRA0_TRACE_HI_DATA_REG                   ,
                ULL(0x08010400), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCMC23_SLOW_TRA0_TRACE_LO_DATA_REG                   ,
                ULL(0x08010401), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCMC23_SLOW_TRA0_TRACE_TRCTRL_CONFIG                 ,
                ULL(0x08010402), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCMC23_SLOW_TRA0_TRACE_TRDATA_CONFIG_0               ,
                ULL(0x08010403), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCMC23_SLOW_TRA0_TRACE_TRDATA_CONFIG_1               ,
                ULL(0x08010404), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCMC23_SLOW_TRA0_TRACE_TRDATA_CONFIG_2               ,
                ULL(0x08010405), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCMC23_SLOW_TRA0_TRACE_TRDATA_CONFIG_3               ,
                ULL(0x08010406), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCMC23_SLOW_TRA0_TRACE_TRDATA_CONFIG_4               ,
                ULL(0x08010407), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCMC23_SLOW_TRA0_TRACE_TRDATA_CONFIG_5               ,
                ULL(0x08010408), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCMC23_SLOW_TRA0_TRACE_TRDATA_CONFIG_9               ,
                ULL(0x08010409), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN0_TRA0_TR0_TRACE_HI_DATA_REG                      ,
                ULL(0x02010400), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCN0_TRA0_TR0_TRACE_LO_DATA_REG                      ,
                ULL(0x02010401), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCN0_TRA0_TR0_TRACE_TRCTRL_CONFIG                    ,
                ULL(0x02010402), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN0_TRA0_TR0_TRACE_TRDATA_CONFIG_0                  ,
                ULL(0x02010403), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN0_TRA0_TR0_TRACE_TRDATA_CONFIG_1                  ,
                ULL(0x02010404), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN0_TRA0_TR0_TRACE_TRDATA_CONFIG_2                  ,
                ULL(0x02010405), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN0_TRA0_TR0_TRACE_TRDATA_CONFIG_3                  ,
                ULL(0x02010406), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN0_TRA0_TR0_TRACE_TRDATA_CONFIG_4                  ,
                ULL(0x02010407), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN0_TRA0_TR0_TRACE_TRDATA_CONFIG_5                  ,
                ULL(0x02010408), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN0_TRA0_TR0_TRACE_TRDATA_CONFIG_9                  ,
                ULL(0x02010409), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN0_TRA0_TR1_TRACE_HI_DATA_REG                      ,
                ULL(0x02010440), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCN0_TRA0_TR1_TRACE_LO_DATA_REG                      ,
                ULL(0x02010441), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCN0_TRA0_TR1_TRACE_TRCTRL_CONFIG                    ,
                ULL(0x02010442), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN0_TRA0_TR1_TRACE_TRDATA_CONFIG_0                  ,
                ULL(0x02010443), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN0_TRA0_TR1_TRACE_TRDATA_CONFIG_1                  ,
                ULL(0x02010444), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN0_TRA0_TR1_TRACE_TRDATA_CONFIG_2                  ,
                ULL(0x02010445), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN0_TRA0_TR1_TRACE_TRDATA_CONFIG_3                  ,
                ULL(0x02010446), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN0_TRA0_TR1_TRACE_TRDATA_CONFIG_4                  ,
                ULL(0x02010447), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN0_TRA0_TR1_TRACE_TRDATA_CONFIG_5                  ,
                ULL(0x02010448), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN0_TRA0_TR1_TRACE_TRDATA_CONFIG_9                  ,
                ULL(0x02010449), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN0_TRA1_TRACE_HI_DATA_REG                          ,
                ULL(0x02010480), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCN0_TRA1_TRACE_LO_DATA_REG                          ,
                ULL(0x02010481), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCN0_TRA1_TRACE_TRCTRL_CONFIG                        ,
                ULL(0x02010482), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN0_TRA1_TRACE_TRDATA_CONFIG_0                      ,
                ULL(0x02010483), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN0_TRA1_TRACE_TRDATA_CONFIG_1                      ,
                ULL(0x02010484), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN0_TRA1_TRACE_TRDATA_CONFIG_2                      ,
                ULL(0x02010485), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN0_TRA1_TRACE_TRDATA_CONFIG_3                      ,
                ULL(0x02010486), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN0_TRA1_TRACE_TRDATA_CONFIG_4                      ,
                ULL(0x02010487), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN0_TRA1_TRACE_TRDATA_CONFIG_5                      ,
                ULL(0x02010488), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN0_TRA1_TRACE_TRDATA_CONFIG_9                      ,
                ULL(0x02010489), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA0_TR0_TRACE_HI_DATA_REG                      ,
                ULL(0x03010400), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCN1_TRA0_TR0_TRACE_LO_DATA_REG                      ,
                ULL(0x03010401), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCN1_TRA0_TR0_TRACE_TRCTRL_CONFIG                    ,
                ULL(0x03010402), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA0_TR0_TRACE_TRDATA_CONFIG_0                  ,
                ULL(0x03010403), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA0_TR0_TRACE_TRDATA_CONFIG_1                  ,
                ULL(0x03010404), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA0_TR0_TRACE_TRDATA_CONFIG_2                  ,
                ULL(0x03010405), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA0_TR0_TRACE_TRDATA_CONFIG_3                  ,
                ULL(0x03010406), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA0_TR0_TRACE_TRDATA_CONFIG_4                  ,
                ULL(0x03010407), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA0_TR0_TRACE_TRDATA_CONFIG_5                  ,
                ULL(0x03010408), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA0_TR0_TRACE_TRDATA_CONFIG_9                  ,
                ULL(0x03010409), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA0_TR1_TRACE_HI_DATA_REG                      ,
                ULL(0x03010440), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCN1_TRA0_TR1_TRACE_LO_DATA_REG                      ,
                ULL(0x03010441), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCN1_TRA0_TR1_TRACE_TRCTRL_CONFIG                    ,
                ULL(0x03010442), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA0_TR1_TRACE_TRDATA_CONFIG_0                  ,
                ULL(0x03010443), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA0_TR1_TRACE_TRDATA_CONFIG_1                  ,
                ULL(0x03010444), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA0_TR1_TRACE_TRDATA_CONFIG_2                  ,
                ULL(0x03010445), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA0_TR1_TRACE_TRDATA_CONFIG_3                  ,
                ULL(0x03010446), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA0_TR1_TRACE_TRDATA_CONFIG_4                  ,
                ULL(0x03010447), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA0_TR1_TRACE_TRDATA_CONFIG_5                  ,
                ULL(0x03010448), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA0_TR1_TRACE_TRDATA_CONFIG_9                  ,
                ULL(0x03010449), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA1_TR0_TRACE_HI_DATA_REG                      ,
                ULL(0x03010480), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCN1_TRA1_TR0_TRACE_LO_DATA_REG                      ,
                ULL(0x03010481), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCN1_TRA1_TR0_TRACE_TRCTRL_CONFIG                    ,
                ULL(0x03010482), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA1_TR0_TRACE_TRDATA_CONFIG_0                  ,
                ULL(0x03010483), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA1_TR0_TRACE_TRDATA_CONFIG_1                  ,
                ULL(0x03010484), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA1_TR0_TRACE_TRDATA_CONFIG_2                  ,
                ULL(0x03010485), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA1_TR0_TRACE_TRDATA_CONFIG_3                  ,
                ULL(0x03010486), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA1_TR0_TRACE_TRDATA_CONFIG_4                  ,
                ULL(0x03010487), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA1_TR0_TRACE_TRDATA_CONFIG_5                  ,
                ULL(0x03010488), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA1_TR0_TRACE_TRDATA_CONFIG_9                  ,
                ULL(0x03010489), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA1_TR1_TRACE_HI_DATA_REG                      ,
                ULL(0x030104C0), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCN1_TRA1_TR1_TRACE_LO_DATA_REG                      ,
                ULL(0x030104C1), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCN1_TRA1_TR1_TRACE_TRCTRL_CONFIG                    ,
                ULL(0x030104C2), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA1_TR1_TRACE_TRDATA_CONFIG_0                  ,
                ULL(0x030104C3), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA1_TR1_TRACE_TRDATA_CONFIG_1                  ,
                ULL(0x030104C4), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA1_TR1_TRACE_TRDATA_CONFIG_2                  ,
                ULL(0x030104C5), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA1_TR1_TRACE_TRDATA_CONFIG_3                  ,
                ULL(0x030104C6), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA1_TR1_TRACE_TRDATA_CONFIG_4                  ,
                ULL(0x030104C7), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA1_TR1_TRACE_TRDATA_CONFIG_5                  ,
                ULL(0x030104C8), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA1_TR1_TRACE_TRDATA_CONFIG_9                  ,
                ULL(0x030104C9), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA2_TR0_TRACE_HI_DATA_REG                      ,
                ULL(0x03010500), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCN1_TRA2_TR0_TRACE_LO_DATA_REG                      ,
                ULL(0x03010501), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCN1_TRA2_TR0_TRACE_TRCTRL_CONFIG                    ,
                ULL(0x03010502), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA2_TR0_TRACE_TRDATA_CONFIG_0                  ,
                ULL(0x03010503), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA2_TR0_TRACE_TRDATA_CONFIG_1                  ,
                ULL(0x03010504), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA2_TR0_TRACE_TRDATA_CONFIG_2                  ,
                ULL(0x03010505), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA2_TR0_TRACE_TRDATA_CONFIG_3                  ,
                ULL(0x03010506), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA2_TR0_TRACE_TRDATA_CONFIG_4                  ,
                ULL(0x03010507), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA2_TR0_TRACE_TRDATA_CONFIG_5                  ,
                ULL(0x03010508), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA2_TR0_TRACE_TRDATA_CONFIG_9                  ,
                ULL(0x03010509), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA2_TR1_TRACE_HI_DATA_REG                      ,
                ULL(0x03010540), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCN1_TRA2_TR1_TRACE_LO_DATA_REG                      ,
                ULL(0x03010541), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCN1_TRA2_TR1_TRACE_TRCTRL_CONFIG                    ,
                ULL(0x03010542), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA2_TR1_TRACE_TRDATA_CONFIG_0                  ,
                ULL(0x03010543), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA2_TR1_TRACE_TRDATA_CONFIG_1                  ,
                ULL(0x03010544), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA2_TR1_TRACE_TRDATA_CONFIG_2                  ,
                ULL(0x03010545), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA2_TR1_TRACE_TRDATA_CONFIG_3                  ,
                ULL(0x03010546), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA2_TR1_TRACE_TRDATA_CONFIG_4                  ,
                ULL(0x03010547), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA2_TR1_TRACE_TRDATA_CONFIG_5                  ,
                ULL(0x03010548), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA2_TR1_TRACE_TRDATA_CONFIG_9                  ,
                ULL(0x03010549), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA3_TR0_TRACE_HI_DATA_REG                      ,
                ULL(0x03010580), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCN1_TRA3_TR0_TRACE_LO_DATA_REG                      ,
                ULL(0x03010581), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCN1_TRA3_TR0_TRACE_TRCTRL_CONFIG                    ,
                ULL(0x03010582), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA3_TR0_TRACE_TRDATA_CONFIG_0                  ,
                ULL(0x03010583), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA3_TR0_TRACE_TRDATA_CONFIG_1                  ,
                ULL(0x03010584), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA3_TR0_TRACE_TRDATA_CONFIG_2                  ,
                ULL(0x03010585), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA3_TR0_TRACE_TRDATA_CONFIG_3                  ,
                ULL(0x03010586), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA3_TR0_TRACE_TRDATA_CONFIG_4                  ,
                ULL(0x03010587), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA3_TR0_TRACE_TRDATA_CONFIG_5                  ,
                ULL(0x03010588), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA3_TR0_TRACE_TRDATA_CONFIG_9                  ,
                ULL(0x03010589), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA3_TR1_TRACE_HI_DATA_REG                      ,
                ULL(0x030105C0), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCN1_TRA3_TR1_TRACE_LO_DATA_REG                      ,
                ULL(0x030105C1), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCN1_TRA3_TR1_TRACE_TRCTRL_CONFIG                    ,
                ULL(0x030105C2), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA3_TR1_TRACE_TRDATA_CONFIG_0                  ,
                ULL(0x030105C3), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA3_TR1_TRACE_TRDATA_CONFIG_1                  ,
                ULL(0x030105C4), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA3_TR1_TRACE_TRDATA_CONFIG_2                  ,
                ULL(0x030105C5), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA3_TR1_TRACE_TRDATA_CONFIG_3                  ,
                ULL(0x030105C6), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA3_TR1_TRACE_TRDATA_CONFIG_4                  ,
                ULL(0x030105C7), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA3_TR1_TRACE_TRDATA_CONFIG_5                  ,
                ULL(0x030105C8), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA3_TR1_TRACE_TRDATA_CONFIG_9                  ,
                ULL(0x030105C9), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA4_TR0_TRACE_HI_DATA_REG                      ,
                ULL(0x03010600), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCN1_TRA4_TR0_TRACE_LO_DATA_REG                      ,
                ULL(0x03010601), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCN1_TRA4_TR0_TRACE_TRCTRL_CONFIG                    ,
                ULL(0x03010602), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA4_TR0_TRACE_TRDATA_CONFIG_0                  ,
                ULL(0x03010603), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA4_TR0_TRACE_TRDATA_CONFIG_1                  ,
                ULL(0x03010604), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA4_TR0_TRACE_TRDATA_CONFIG_2                  ,
                ULL(0x03010605), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA4_TR0_TRACE_TRDATA_CONFIG_3                  ,
                ULL(0x03010606), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA4_TR0_TRACE_TRDATA_CONFIG_4                  ,
                ULL(0x03010607), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA4_TR0_TRACE_TRDATA_CONFIG_5                  ,
                ULL(0x03010608), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA4_TR0_TRACE_TRDATA_CONFIG_9                  ,
                ULL(0x03010609), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA4_TR1_TRACE_HI_DATA_REG                      ,
                ULL(0x03010640), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCN1_TRA4_TR1_TRACE_LO_DATA_REG                      ,
                ULL(0x03010641), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCN1_TRA4_TR1_TRACE_TRCTRL_CONFIG                    ,
                ULL(0x03010642), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA4_TR1_TRACE_TRDATA_CONFIG_0                  ,
                ULL(0x03010643), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA4_TR1_TRACE_TRDATA_CONFIG_1                  ,
                ULL(0x03010644), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA4_TR1_TRACE_TRDATA_CONFIG_2                  ,
                ULL(0x03010645), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA4_TR1_TRACE_TRDATA_CONFIG_3                  ,
                ULL(0x03010646), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA4_TR1_TRACE_TRDATA_CONFIG_4                  ,
                ULL(0x03010647), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA4_TR1_TRACE_TRDATA_CONFIG_5                  ,
                ULL(0x03010648), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN1_TRA4_TR1_TRACE_TRDATA_CONFIG_9                  ,
                ULL(0x03010649), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN2_TRA0_TR0_TRACE_HI_DATA_REG                      ,
                ULL(0x04010400), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCN2_TRA0_TR0_TRACE_LO_DATA_REG                      ,
                ULL(0x04010401), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCN2_TRA0_TR0_TRACE_TRCTRL_CONFIG                    ,
                ULL(0x04010402), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN2_TRA0_TR0_TRACE_TRDATA_CONFIG_0                  ,
                ULL(0x04010403), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN2_TRA0_TR0_TRACE_TRDATA_CONFIG_1                  ,
                ULL(0x04010404), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN2_TRA0_TR0_TRACE_TRDATA_CONFIG_2                  ,
                ULL(0x04010405), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN2_TRA0_TR0_TRACE_TRDATA_CONFIG_3                  ,
                ULL(0x04010406), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN2_TRA0_TR0_TRACE_TRDATA_CONFIG_4                  ,
                ULL(0x04010407), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN2_TRA0_TR0_TRACE_TRDATA_CONFIG_5                  ,
                ULL(0x04010408), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN2_TRA0_TR0_TRACE_TRDATA_CONFIG_9                  ,
                ULL(0x04010409), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN2_TRA0_TR1_TRACE_HI_DATA_REG                      ,
                ULL(0x04010440), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCN2_TRA0_TR1_TRACE_LO_DATA_REG                      ,
                ULL(0x04010441), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCN2_TRA0_TR1_TRACE_TRCTRL_CONFIG                    ,
                ULL(0x04010442), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN2_TRA0_TR1_TRACE_TRDATA_CONFIG_0                  ,
                ULL(0x04010443), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN2_TRA0_TR1_TRACE_TRDATA_CONFIG_1                  ,
                ULL(0x04010444), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN2_TRA0_TR1_TRACE_TRDATA_CONFIG_2                  ,
                ULL(0x04010445), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN2_TRA0_TR1_TRACE_TRDATA_CONFIG_3                  ,
                ULL(0x04010446), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN2_TRA0_TR1_TRACE_TRDATA_CONFIG_4                  ,
                ULL(0x04010447), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN2_TRA0_TR1_TRACE_TRDATA_CONFIG_5                  ,
                ULL(0x04010448), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN2_TRA0_TR1_TRACE_TRDATA_CONFIG_9                  ,
                ULL(0x04010449), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA0_TR0_TRACE_HI_DATA_REG                      ,
                ULL(0x05010400), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCN3_TRA0_TR0_TRACE_LO_DATA_REG                      ,
                ULL(0x05010401), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCN3_TRA0_TR0_TRACE_TRCTRL_CONFIG                    ,
                ULL(0x05010402), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA0_TR0_TRACE_TRDATA_CONFIG_0                  ,
                ULL(0x05010403), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA0_TR0_TRACE_TRDATA_CONFIG_1                  ,
                ULL(0x05010404), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA0_TR0_TRACE_TRDATA_CONFIG_2                  ,
                ULL(0x05010405), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA0_TR0_TRACE_TRDATA_CONFIG_3                  ,
                ULL(0x05010406), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA0_TR0_TRACE_TRDATA_CONFIG_4                  ,
                ULL(0x05010407), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA0_TR0_TRACE_TRDATA_CONFIG_5                  ,
                ULL(0x05010408), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA0_TR0_TRACE_TRDATA_CONFIG_9                  ,
                ULL(0x05010409), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA0_TR1_TRACE_HI_DATA_REG                      ,
                ULL(0x05010440), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCN3_TRA0_TR1_TRACE_LO_DATA_REG                      ,
                ULL(0x05010441), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCN3_TRA0_TR1_TRACE_TRCTRL_CONFIG                    ,
                ULL(0x05010442), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA0_TR1_TRACE_TRDATA_CONFIG_0                  ,
                ULL(0x05010443), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA0_TR1_TRACE_TRDATA_CONFIG_1                  ,
                ULL(0x05010444), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA0_TR1_TRACE_TRDATA_CONFIG_2                  ,
                ULL(0x05010445), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA0_TR1_TRACE_TRDATA_CONFIG_3                  ,
                ULL(0x05010446), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA0_TR1_TRACE_TRDATA_CONFIG_4                  ,
                ULL(0x05010447), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA0_TR1_TRACE_TRDATA_CONFIG_5                  ,
                ULL(0x05010448), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA0_TR1_TRACE_TRDATA_CONFIG_9                  ,
                ULL(0x05010449), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA1_TR0_TRACE_HI_DATA_REG                      ,
                ULL(0x05010480), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCN3_TRA1_TR0_TRACE_LO_DATA_REG                      ,
                ULL(0x05010481), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCN3_TRA1_TR0_TRACE_TRCTRL_CONFIG                    ,
                ULL(0x05010482), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA1_TR0_TRACE_TRDATA_CONFIG_0                  ,
                ULL(0x05010483), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA1_TR0_TRACE_TRDATA_CONFIG_1                  ,
                ULL(0x05010484), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA1_TR0_TRACE_TRDATA_CONFIG_2                  ,
                ULL(0x05010485), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA1_TR0_TRACE_TRDATA_CONFIG_3                  ,
                ULL(0x05010486), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA1_TR0_TRACE_TRDATA_CONFIG_4                  ,
                ULL(0x05010487), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA1_TR0_TRACE_TRDATA_CONFIG_5                  ,
                ULL(0x05010488), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA1_TR0_TRACE_TRDATA_CONFIG_9                  ,
                ULL(0x05010489), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA1_TR1_TRACE_HI_DATA_REG                      ,
                ULL(0x050104C0), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCN3_TRA1_TR1_TRACE_LO_DATA_REG                      ,
                ULL(0x050104C1), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCN3_TRA1_TR1_TRACE_TRCTRL_CONFIG                    ,
                ULL(0x050104C2), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA1_TR1_TRACE_TRDATA_CONFIG_0                  ,
                ULL(0x050104C3), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA1_TR1_TRACE_TRDATA_CONFIG_1                  ,
                ULL(0x050104C4), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA1_TR1_TRACE_TRDATA_CONFIG_2                  ,
                ULL(0x050104C5), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA1_TR1_TRACE_TRDATA_CONFIG_3                  ,
                ULL(0x050104C6), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA1_TR1_TRACE_TRDATA_CONFIG_4                  ,
                ULL(0x050104C7), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA1_TR1_TRACE_TRDATA_CONFIG_5                  ,
                ULL(0x050104C8), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA1_TR1_TRACE_TRDATA_CONFIG_9                  ,
                ULL(0x050104C9), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA2_TR0_TRACE_HI_DATA_REG                      ,
                ULL(0x05010500), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCN3_TRA2_TR0_TRACE_LO_DATA_REG                      ,
                ULL(0x05010501), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCN3_TRA2_TR0_TRACE_TRCTRL_CONFIG                    ,
                ULL(0x05010502), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA2_TR0_TRACE_TRDATA_CONFIG_0                  ,
                ULL(0x05010503), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA2_TR0_TRACE_TRDATA_CONFIG_1                  ,
                ULL(0x05010504), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA2_TR0_TRACE_TRDATA_CONFIG_2                  ,
                ULL(0x05010505), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA2_TR0_TRACE_TRDATA_CONFIG_3                  ,
                ULL(0x05010506), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA2_TR0_TRACE_TRDATA_CONFIG_4                  ,
                ULL(0x05010507), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA2_TR0_TRACE_TRDATA_CONFIG_5                  ,
                ULL(0x05010508), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA2_TR0_TRACE_TRDATA_CONFIG_9                  ,
                ULL(0x05010509), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA2_TR1_TRACE_HI_DATA_REG                      ,
                ULL(0x05010540), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCN3_TRA2_TR1_TRACE_LO_DATA_REG                      ,
                ULL(0x05010541), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCN3_TRA2_TR1_TRACE_TRCTRL_CONFIG                    ,
                ULL(0x05010542), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA2_TR1_TRACE_TRDATA_CONFIG_0                  ,
                ULL(0x05010543), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA2_TR1_TRACE_TRDATA_CONFIG_1                  ,
                ULL(0x05010544), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA2_TR1_TRACE_TRDATA_CONFIG_2                  ,
                ULL(0x05010545), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA2_TR1_TRACE_TRDATA_CONFIG_3                  ,
                ULL(0x05010546), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA2_TR1_TRACE_TRDATA_CONFIG_4                  ,
                ULL(0x05010547), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA2_TR1_TRACE_TRDATA_CONFIG_5                  ,
                ULL(0x05010548), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA2_TR1_TRACE_TRDATA_CONFIG_9                  ,
                ULL(0x05010549), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA3_TR0_TRACE_HI_DATA_REG                      ,
                ULL(0x05010580), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCN3_TRA3_TR0_TRACE_LO_DATA_REG                      ,
                ULL(0x05010581), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCN3_TRA3_TR0_TRACE_TRCTRL_CONFIG                    ,
                ULL(0x05010582), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA3_TR0_TRACE_TRDATA_CONFIG_0                  ,
                ULL(0x05010583), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA3_TR0_TRACE_TRDATA_CONFIG_1                  ,
                ULL(0x05010584), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA3_TR0_TRACE_TRDATA_CONFIG_2                  ,
                ULL(0x05010585), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA3_TR0_TRACE_TRDATA_CONFIG_3                  ,
                ULL(0x05010586), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA3_TR0_TRACE_TRDATA_CONFIG_4                  ,
                ULL(0x05010587), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA3_TR0_TRACE_TRDATA_CONFIG_5                  ,
                ULL(0x05010588), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA3_TR0_TRACE_TRDATA_CONFIG_9                  ,
                ULL(0x05010589), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA3_TR1_TRACE_HI_DATA_REG                      ,
                ULL(0x050105C0), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCN3_TRA3_TR1_TRACE_LO_DATA_REG                      ,
                ULL(0x050105C1), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCN3_TRA3_TR1_TRACE_TRCTRL_CONFIG                    ,
                ULL(0x050105C2), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA3_TR1_TRACE_TRDATA_CONFIG_0                  ,
                ULL(0x050105C3), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA3_TR1_TRACE_TRDATA_CONFIG_1                  ,
                ULL(0x050105C4), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA3_TR1_TRACE_TRDATA_CONFIG_2                  ,
                ULL(0x050105C5), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA3_TR1_TRACE_TRDATA_CONFIG_3                  ,
                ULL(0x050105C6), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA3_TR1_TRACE_TRDATA_CONFIG_4                  ,
                ULL(0x050105C7), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA3_TR1_TRACE_TRDATA_CONFIG_5                  ,
                ULL(0x050105C8), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA3_TR1_TRACE_TRDATA_CONFIG_9                  ,
                ULL(0x050105C9), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA4_TRACE_HI_DATA_REG                          ,
                ULL(0x05010600), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCN3_TRA4_TRACE_LO_DATA_REG                          ,
                ULL(0x05010601), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCN3_TRA4_TRACE_TRCTRL_CONFIG                        ,
                ULL(0x05010602), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA4_TRACE_TRDATA_CONFIG_0                      ,
                ULL(0x05010603), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA4_TRACE_TRDATA_CONFIG_1                      ,
                ULL(0x05010604), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA4_TRACE_TRDATA_CONFIG_2                      ,
                ULL(0x05010605), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA4_TRACE_TRDATA_CONFIG_3                      ,
                ULL(0x05010606), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA4_TRACE_TRDATA_CONFIG_4                      ,
                ULL(0x05010607), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA4_TRACE_TRDATA_CONFIG_5                      ,
                ULL(0x05010608), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA4_TRACE_TRDATA_CONFIG_9                      ,
                ULL(0x05010609), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA5_TR0_TRACE_HI_DATA_REG                      ,
                ULL(0x05010680), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCN3_TRA5_TR0_TRACE_LO_DATA_REG                      ,
                ULL(0x05010681), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCN3_TRA5_TR0_TRACE_TRCTRL_CONFIG                    ,
                ULL(0x05010682), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA5_TR0_TRACE_TRDATA_CONFIG_0                  ,
                ULL(0x05010683), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA5_TR0_TRACE_TRDATA_CONFIG_1                  ,
                ULL(0x05010684), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA5_TR0_TRACE_TRDATA_CONFIG_2                  ,
                ULL(0x05010685), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA5_TR0_TRACE_TRDATA_CONFIG_3                  ,
                ULL(0x05010686), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA5_TR0_TRACE_TRDATA_CONFIG_4                  ,
                ULL(0x05010687), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA5_TR0_TRACE_TRDATA_CONFIG_5                  ,
                ULL(0x05010688), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA5_TR0_TRACE_TRDATA_CONFIG_9                  ,
                ULL(0x05010689), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA5_TR1_TRACE_HI_DATA_REG                      ,
                ULL(0x050106C0), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCN3_TRA5_TR1_TRACE_LO_DATA_REG                      ,
                ULL(0x050106C1), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCN3_TRA5_TR1_TRACE_TRCTRL_CONFIG                    ,
                ULL(0x050106C2), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA5_TR1_TRACE_TRDATA_CONFIG_0                  ,
                ULL(0x050106C3), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA5_TR1_TRACE_TRDATA_CONFIG_1                  ,
                ULL(0x050106C4), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA5_TR1_TRACE_TRDATA_CONFIG_2                  ,
                ULL(0x050106C5), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA5_TR1_TRACE_TRDATA_CONFIG_3                  ,
                ULL(0x050106C6), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA5_TR1_TRACE_TRDATA_CONFIG_4                  ,
                ULL(0x050106C7), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA5_TR1_TRACE_TRDATA_CONFIG_5                  ,
                ULL(0x050106C8), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCN3_TRA5_TR1_TRACE_TRDATA_CONFIG_9                  ,
                ULL(0x050106C9), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCPCI0_TRA0_TRACE_HI_DATA_REG                        ,
                ULL(0x0D010400), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCPCI0_TRA0_TRACE_LO_DATA_REG                        ,
                ULL(0x0D010401), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCPCI0_TRA0_TRACE_TRCTRL_CONFIG                      ,
                ULL(0x0D010402), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCPCI0_TRA0_TRACE_TRDATA_CONFIG_0                    ,
                ULL(0x0D010403), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCPCI0_TRA0_TRACE_TRDATA_CONFIG_1                    ,
                ULL(0x0D010404), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCPCI0_TRA0_TRACE_TRDATA_CONFIG_2                    ,
                ULL(0x0D010405), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCPCI0_TRA0_TRACE_TRDATA_CONFIG_3                    ,
                ULL(0x0D010406), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCPCI0_TRA0_TRACE_TRDATA_CONFIG_4                    ,
                ULL(0x0D010407), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCPCI0_TRA0_TRACE_TRDATA_CONFIG_5                    ,
                ULL(0x0D010408), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCPCI0_TRA0_TRACE_TRDATA_CONFIG_9                    ,
                ULL(0x0D010409), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCPCI1_TRA0_TRACE_HI_DATA_REG                        ,
                ULL(0x0E010400), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCPCI1_TRA0_TRACE_LO_DATA_REG                        ,
                ULL(0x0E010401), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCPCI1_TRA0_TRACE_TRCTRL_CONFIG                      ,
                ULL(0x0E010402), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCPCI1_TRA0_TRACE_TRDATA_CONFIG_0                    ,
                ULL(0x0E010403), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCPCI1_TRA0_TRACE_TRDATA_CONFIG_1                    ,
                ULL(0x0E010404), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCPCI1_TRA0_TRACE_TRDATA_CONFIG_2                    ,
                ULL(0x0E010405), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCPCI1_TRA0_TRACE_TRDATA_CONFIG_3                    ,
                ULL(0x0E010406), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCPCI1_TRA0_TRACE_TRDATA_CONFIG_4                    ,
                ULL(0x0E010407), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCPCI1_TRA0_TRACE_TRDATA_CONFIG_5                    ,
                ULL(0x0E010408), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCPCI1_TRA0_TRACE_TRDATA_CONFIG_9                    ,
                ULL(0x0E010409), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCPCI2_TRA0_TRACE_HI_DATA_REG                        ,
                ULL(0x0F010400), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCPCI2_TRA0_TRACE_LO_DATA_REG                        ,
                ULL(0x0F010401), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCPCI2_TRA0_TRACE_TRCTRL_CONFIG                      ,
                ULL(0x0F010402), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCPCI2_TRA0_TRACE_TRDATA_CONFIG_0                    ,
                ULL(0x0F010403), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCPCI2_TRA0_TRACE_TRDATA_CONFIG_1                    ,
                ULL(0x0F010404), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCPCI2_TRA0_TRACE_TRDATA_CONFIG_2                    ,
                ULL(0x0F010405), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCPCI2_TRA0_TRACE_TRDATA_CONFIG_3                    ,
                ULL(0x0F010406), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCPCI2_TRA0_TRACE_TRDATA_CONFIG_4                    ,
                ULL(0x0F010407), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCPCI2_TRA0_TRACE_TRDATA_CONFIG_5                    ,
                ULL(0x0F010408), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCPCI2_TRA0_TRACE_TRDATA_CONFIG_9                    ,
                ULL(0x0F010409), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCXB_TRA0_TR0_TRACE_HI_DATA_REG                      ,
                ULL(0x06010400), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCXB_TRA0_TR0_TRACE_LO_DATA_REG                      ,
                ULL(0x06010401), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCXB_TRA0_TR0_TRACE_TRCTRL_CONFIG                    ,
                ULL(0x06010402), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCXB_TRA0_TR0_TRACE_TRDATA_CONFIG_0                  ,
                ULL(0x06010403), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCXB_TRA0_TR0_TRACE_TRDATA_CONFIG_1                  ,
                ULL(0x06010404), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCXB_TRA0_TR0_TRACE_TRDATA_CONFIG_2                  ,
                ULL(0x06010405), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCXB_TRA0_TR0_TRACE_TRDATA_CONFIG_3                  ,
                ULL(0x06010406), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCXB_TRA0_TR0_TRACE_TRDATA_CONFIG_4                  ,
                ULL(0x06010407), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCXB_TRA0_TR0_TRACE_TRDATA_CONFIG_5                  ,
                ULL(0x06010408), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCXB_TRA0_TR0_TRACE_TRDATA_CONFIG_9                  ,
                ULL(0x06010409), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCXB_TRA0_TR1_TRACE_HI_DATA_REG                      ,
                ULL(0x06010440), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCXB_TRA0_TR1_TRACE_LO_DATA_REG                      ,
                ULL(0x06010441), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCXB_TRA0_TR1_TRACE_TRCTRL_CONFIG                    ,
                ULL(0x06010442), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCXB_TRA0_TR1_TRACE_TRDATA_CONFIG_0                  ,
                ULL(0x06010443), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCXB_TRA0_TR1_TRACE_TRDATA_CONFIG_1                  ,
                ULL(0x06010444), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCXB_TRA0_TR1_TRACE_TRDATA_CONFIG_2                  ,
                ULL(0x06010445), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCXB_TRA0_TR1_TRACE_TRDATA_CONFIG_3                  ,
                ULL(0x06010446), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCXB_TRA0_TR1_TRACE_TRDATA_CONFIG_4                  ,
                ULL(0x06010447), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCXB_TRA0_TR1_TRACE_TRDATA_CONFIG_5                  ,
                ULL(0x06010448), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCXB_TRA0_TR1_TRACE_TRDATA_CONFIG_9                  ,
                ULL(0x06010449), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCXB_TRA1_TRACE_HI_DATA_REG                          ,
                ULL(0x06010480), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCXB_TRA1_TRACE_LO_DATA_REG                          ,
                ULL(0x06010481), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TCXB_TRA1_TRACE_TRCTRL_CONFIG                        ,
                ULL(0x06010482), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCXB_TRA1_TRACE_TRDATA_CONFIG_0                      ,
                ULL(0x06010483), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCXB_TRA1_TRACE_TRDATA_CONFIG_1                      ,
                ULL(0x06010484), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCXB_TRA1_TRACE_TRDATA_CONFIG_2                      ,
                ULL(0x06010485), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCXB_TRA1_TRACE_TRDATA_CONFIG_3                      ,
                ULL(0x06010486), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCXB_TRA1_TRACE_TRDATA_CONFIG_4                      ,
                ULL(0x06010487), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCXB_TRA1_TRACE_TRDATA_CONFIG_5                      ,
                ULL(0x06010488), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TCXB_TRA1_TRACE_TRDATA_CONFIG_9                      ,
                ULL(0x06010489), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU0_TEST_CERR                                       ,
                ULL(0x050110AA), SH_UNT_PU_NPU0  , SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_TEST_CERR                                       ,
                ULL(0x050111AA), SH_UNT_PU_NPU1  , SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_TEST_CERR                                       ,
                ULL(0x050112AA), SH_UNT_PU_NPU2  , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TOD_CMD_REG                                          ,
                ULL(0x0502282A), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TOD_DATA_RCV_REG                                     ,
                ULL(0x05022829), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TOD_DATA_SND_REG                                     ,
                ULL(0x05022828), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_TPCHIP_TRA0_TR0_TRACE_HI_DATA_REG                    ,
                ULL(0x01010400), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TPCHIP_TRA0_TR0_TRACE_LO_DATA_REG                    ,
                ULL(0x01010401), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TPCHIP_TRA0_TR0_TRACE_TRCTRL_CONFIG                  ,
                ULL(0x01010402), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TPCHIP_TRA0_TR0_TRACE_TRDATA_CONFIG_0                ,
                ULL(0x01010403), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TPCHIP_TRA0_TR0_TRACE_TRDATA_CONFIG_1                ,
                ULL(0x01010404), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TPCHIP_TRA0_TR0_TRACE_TRDATA_CONFIG_2                ,
                ULL(0x01010405), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TPCHIP_TRA0_TR0_TRACE_TRDATA_CONFIG_3                ,
                ULL(0x01010406), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TPCHIP_TRA0_TR0_TRACE_TRDATA_CONFIG_4                ,
                ULL(0x01010407), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TPCHIP_TRA0_TR0_TRACE_TRDATA_CONFIG_5                ,
                ULL(0x01010408), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TPCHIP_TRA0_TR0_TRACE_TRDATA_CONFIG_9                ,
                ULL(0x01010409), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TPCHIP_TRA0_TR1_TRACE_HI_DATA_REG                    ,
                ULL(0x01010440), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TPCHIP_TRA0_TR1_TRACE_LO_DATA_REG                    ,
                ULL(0x01010441), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_TPCHIP_TRA0_TR1_TRACE_TRCTRL_CONFIG                  ,
                ULL(0x01010442), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TPCHIP_TRA0_TR1_TRACE_TRDATA_CONFIG_0                ,
                ULL(0x01010443), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TPCHIP_TRA0_TR1_TRACE_TRDATA_CONFIG_1                ,
                ULL(0x01010444), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TPCHIP_TRA0_TR1_TRACE_TRDATA_CONFIG_2                ,
                ULL(0x01010445), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TPCHIP_TRA0_TR1_TRACE_TRDATA_CONFIG_3                ,
                ULL(0x01010446), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TPCHIP_TRA0_TR1_TRACE_TRDATA_CONFIG_4                ,
                ULL(0x01010447), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TPCHIP_TRA0_TR1_TRACE_TRDATA_CONFIG_5                ,
                ULL(0x01010448), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TPCHIP_TRA0_TR1_TRACE_TRDATA_CONFIG_9                ,
                ULL(0x01010449), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TRUST_CONTROL                                        ,
                ULL(0x05015F45), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PEC_TUNNEL_BAR_REG                                      ,
                ULL(0x04010C05), SH_UNT_PEC      , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_0_TUNNEL_BAR_REG                                    ,
                ULL(0x04010C05), SH_UNT_PEC_0    , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_1_TUNNEL_BAR_REG                                    ,
                ULL(0x04011005), SH_UNT_PEC_1    , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PEC_2_TUNNEL_BAR_REG                                    ,
                ULL(0x04011405), SH_UNT_PEC_2    , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_TX_CH_FSM_REG                                        ,
                ULL(0x05012805), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TX_CH_INTADDR_REG                                    ,
                ULL(0x05012810), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TX_CH_MISC_REG                                       ,
                ULL(0x05012813), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TX_CTRL_STAT_REG                                     ,
                ULL(0x05012800), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TX_DBFF_REG0                                         ,
                ULL(0x05012811), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TX_DBFF_REG1                                         ,
                ULL(0x05012812), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TX_DF_FSM_REG                                        ,
                ULL(0x05012806), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TX_ERROR_REG_WCLEAR                                  ,
                ULL(0x05012802), SH_UNT          , SH_ACS_SCOM_WCLEAR );
CONST_UINT64_T( PU_TX_ERROR_REG_OR                                      ,
                ULL(0x05012804), SH_UNT          , SH_ACS_SCOM1_OR  );

CONST_UINT64_T( PU_TX_ERR_MODE                                          ,
                ULL(0x05012807), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TX_MASK_REG                                          ,
                ULL(0x05012803), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TX_PSI_CNTL                                          ,
                ULL(0x06010830), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TX_PSI_MODE                                          ,
                ULL(0x06010831), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TX_PSI_STATUS                                        ,
                ULL(0x06010832), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_TX_TO_RT_REG                                         ,
                ULL(0x05012801), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_UMAC_STATUS_CONTROL                                  ,
                ULL(0x020110D5), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_VAS_BUFCTL                                           ,
                ULL(0x0301180C), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_VAS_CQERRRPT                                         ,
                ULL(0x03011848), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_VAS_DBGCONT                                          ,
                ULL(0x0301182E), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_VAS_DBGNORTH                                         ,
                ULL(0x0301182D), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_VAS_DBGSOUTH                                         ,
                ULL(0x0301184C), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_VAS_DBGTRIG                                          ,
                ULL(0x0301182F), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_VAS_EGERRRPT                                         ,
                ULL(0x0301184A), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_VAS_ERRINJ                                           ,
                ULL(0x0301184B), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_VAS_FIR_ACTION0_REG                                  ,
                ULL(0x03011806), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_VAS_FIR_ACTION1_REG                                  ,
                ULL(0x03011807), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_VAS_FIR_MASK_REG                                     ,
                ULL(0x03011803), SH_UNT          , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_VAS_FIR_MASK_REG_AND                                 ,
                ULL(0x03011804), SH_UNT          , SH_ACS_SCOM1_AND );
CONST_UINT64_T( PU_VAS_FIR_MASK_REG_OR                                  ,
                ULL(0x03011805), SH_UNT          , SH_ACS_SCOM2_OR  );

CONST_UINT64_T( PU_VAS_FIR_REG                                          ,
                ULL(0x03011800), SH_UNT          , SH_ACS_SCOM_RW   );
CONST_UINT64_T( PU_VAS_FIR_REG_AND                                      ,
                ULL(0x03011801), SH_UNT          , SH_ACS_SCOM1_AND );
CONST_UINT64_T( PU_VAS_FIR_REG_OR                                       ,
                ULL(0x03011802), SH_UNT          , SH_ACS_SCOM2_OR  );

CONST_UINT64_T( PU_VAS_FIR_WOF_REG                                      ,
                ULL(0x03011808), SH_UNT          , SH_ACS_SCOM_WCLRREG );

CONST_UINT64_T( PU_VAS_INERRRPT                                         ,
                ULL(0x0301182B), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_VAS_MISCCTL                                          ,
                ULL(0x0301180D), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_VAS_MMIOCTL                                          ,
                ULL(0x03011829), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_VAS_MMIODATA                                         ,
                ULL(0x0301182A), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_VAS_MMIOECC                                          ,
                ULL(0x03011831), SH_UNT          , SH_ACS_SCOM_RO   );

CONST_UINT64_T( PU_VAS_MMIO_BASE_ADDR                                   ,
                ULL(0x020110D4), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_VAS_PBCFG0                                           ,
                ULL(0x0301184D), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_VAS_PBCFG1                                           ,
                ULL(0x0301184E), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_VAS_PGMIG1                                           ,
                ULL(0x03011841), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_VAS_PGMIG2                                           ,
                ULL(0x03011842), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_VAS_PGMIG3                                           ,
                ULL(0x03011843), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_VAS_PGMIG4                                           ,
                ULL(0x03011844), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_VAS_PGMIG5                                           ,
                ULL(0x03011845), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_VAS_PGMIG6                                           ,
                ULL(0x03011846), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_VAS_PGMIG7                                           ,
                ULL(0x03011847), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_VAS_PMCNTL                                           ,
                ULL(0x03011830), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_VAS_RGERRRPT                                         ,
                ULL(0x0301182C), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_VAS_RMABAR                                           ,
                ULL(0x0301180E), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_VAS_RMABARM                                          ,
                ULL(0x0301180F), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_VAS_SOUTHCTL                                         ,
                ULL(0x0301184F), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_VAS_UWMBAR                                           ,
                ULL(0x0301180B), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_VAS_WCBSBAR                                          ,
                ULL(0x03011840), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_VAS_WCERRRPT                                         ,
                ULL(0x03011849), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_VAS_WCMBAR                                           ,
                ULL(0x0301180A), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_VAS_WCPOIS                                           ,
                ULL(0x03011828), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_VAS_WRMON0BAR                                        ,
                ULL(0x03011810), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_VAS_WRMON0CMP                                        ,
                ULL(0x03011820), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_VAS_WRMON0WID                                        ,
                ULL(0x03011818), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_VAS_WRMON1BAR                                        ,
                ULL(0x03011811), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_VAS_WRMON1CMP                                        ,
                ULL(0x03011821), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_VAS_WRMON1WID                                        ,
                ULL(0x03011819), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_VAS_WRMON2BAR                                        ,
                ULL(0x03011812), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_VAS_WRMON2CMP                                        ,
                ULL(0x03011822), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_VAS_WRMON2WID                                        ,
                ULL(0x0301181A), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_VAS_WRMON3BAR                                        ,
                ULL(0x03011813), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_VAS_WRMON3CMP                                        ,
                ULL(0x03011823), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_VAS_WRMON3WID                                        ,
                ULL(0x0301181B), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_VAS_WRMON4BAR                                        ,
                ULL(0x03011814), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_VAS_WRMON4CMP                                        ,
                ULL(0x03011824), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_VAS_WRMON4WID                                        ,
                ULL(0x0301181C), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_VAS_WRMON5BAR                                        ,
                ULL(0x03011815), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_VAS_WRMON5CMP                                        ,
                ULL(0x03011825), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_VAS_WRMON5WID                                        ,
                ULL(0x0301181D), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_VAS_WRMON6BAR                                        ,
                ULL(0x03011816), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_VAS_WRMON6CMP                                        ,
                ULL(0x03011826), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_VAS_WRMON6WID                                        ,
                ULL(0x0301181E), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_VAS_WRMON7BAR                                        ,
                ULL(0x03011817), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_VAS_WRMON7CMP                                        ,
                ULL(0x03011827), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_VAS_WRMON7WID                                        ,
                ULL(0x0301181F), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_WATCHDOG_HANG_TIMERS_CNTL                            ,
                ULL(0x0201105C), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_WATER_MARK_REGISTER_B                                ,
                ULL(0x000A0007), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_WATER_MARK_REGISTER_C                                ,
                ULL(0x000A1007), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_WATER_MARK_REGISTER_D                                ,
                ULL(0x000A2007), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_WATER_MARK_REGISTER_E                                ,
                ULL(0x000A3007), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XSCOM_BASE_REG                                       ,
                ULL(0x05022810), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XSCOM_DAT0_REG                                       ,
                ULL(0x0502281E), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_XSCOM_DAT1_REG                                       ,
                ULL(0x0502281F), SH_UNT          , SH_ACS_SCOM_RW   );

CONST_UINT64_T( PU_XSCOM_ERR_REG                                        ,
                ULL(0x05022813), SH_UNT          , SH_ACS_SCOM_WCLRREG );

CONST_UINT64_T( PU_XSCOM_LOG_REG                                        ,
                ULL(0x05022812), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XSCOM_MODE_REG                                       ,
                ULL(0x05022811), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XSCOM_RCVED_STAT_REG                                 ,
                ULL(0x05022818), SH_UNT          , SH_ACS_SCOM_WCLRREG );

CONST_UINT64_T( PU_NPU0_SM0_XTIMER_CONFIG                               ,
                ULL(0x0501100A), SH_UNT_PU_NPU0_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM1_XTIMER_CONFIG                               ,
                ULL(0x0501102A), SH_UNT_PU_NPU0_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM2_XTIMER_CONFIG                               ,
                ULL(0x0501104A), SH_UNT_PU_NPU0_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU0_SM3_XTIMER_CONFIG                               ,
                ULL(0x0501106A), SH_UNT_PU_NPU0_SM3, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM0_XTIMER_CONFIG                               ,
                ULL(0x0501110A), SH_UNT_PU_NPU1_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM1_XTIMER_CONFIG                               ,
                ULL(0x0501112A), SH_UNT_PU_NPU1_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM2_XTIMER_CONFIG                               ,
                ULL(0x0501114A), SH_UNT_PU_NPU1_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU1_SM3_XTIMER_CONFIG                               ,
                ULL(0x0501116A), SH_UNT_PU_NPU1_SM3, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM0_XTIMER_CONFIG                               ,
                ULL(0x0501120A), SH_UNT_PU_NPU2_SM0, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM1_XTIMER_CONFIG                               ,
                ULL(0x0501122A), SH_UNT_PU_NPU2_SM1, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM2_XTIMER_CONFIG                               ,
                ULL(0x0501124A), SH_UNT_PU_NPU2_SM2, SH_ACS_SCOM      );
CONST_UINT64_T( PU_NPU2_SM3_XTIMER_CONFIG                               ,
                ULL(0x0501126A), SH_UNT_PU_NPU2_SM3, SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_ATSD_AVA0                                        ,
                ULL(0x050AF341), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_ATSD_AVA1                                        ,
                ULL(0x050B1341), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_ATSD_AVA2                                        ,
                ULL(0x050B3341), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_ATSD_AVA3                                        ,
                ULL(0x050B5341), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_ATSD_AVA4                                        ,
                ULL(0x050B7341), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_ATSD_AVA5                                        ,
                ULL(0x050B9341), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_ATSD_AVA6                                        ,
                ULL(0x050BB341), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_ATSD_AVA7                                        ,
                ULL(0x050BD341), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU_SM3_XTS_ATSD_HYP0                                ,
                ULL(0x05011360), SH_UNT_PU_NPU_SM3, SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU_SM3_XTS_ATSD_HYP1                                ,
                ULL(0x05011361), SH_UNT_PU_NPU_SM3, SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU_SM3_XTS_ATSD_HYP2                                ,
                ULL(0x05011362), SH_UNT_PU_NPU_SM3, SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU_SM3_XTS_ATSD_HYP3                                ,
                ULL(0x05011363), SH_UNT_PU_NPU_SM3, SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU_SM3_XTS_ATSD_HYP4                                ,
                ULL(0x05011364), SH_UNT_PU_NPU_SM3, SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU_SM3_XTS_ATSD_HYP5                                ,
                ULL(0x05011365), SH_UNT_PU_NPU_SM3, SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU_SM3_XTS_ATSD_HYP6                                ,
                ULL(0x05011366), SH_UNT_PU_NPU_SM3, SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU_SM3_XTS_ATSD_HYP7                                ,
                ULL(0x05011367), SH_UNT_PU_NPU_SM3, SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_ATSD_LNCH0                                       ,
                ULL(0x050AF340), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_ATSD_LNCH1                                       ,
                ULL(0x050B1340), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_ATSD_LNCH2                                       ,
                ULL(0x050B3340), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_ATSD_LNCH3                                       ,
                ULL(0x050B5340), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_ATSD_LNCH4                                       ,
                ULL(0x050B7340), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_ATSD_LNCH5                                       ,
                ULL(0x050B9340), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_ATSD_LNCH6                                       ,
                ULL(0x050BB340), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_ATSD_LNCH7                                       ,
                ULL(0x050BD340), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_ATSD_STAT0                                       ,
                ULL(0x050AF342), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_ATSD_STAT1                                       ,
                ULL(0x050B1342), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_ATSD_STAT2                                       ,
                ULL(0x050B3342), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_ATSD_STAT3                                       ,
                ULL(0x050B5342), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_ATSD_STAT4                                       ,
                ULL(0x050B7342), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_ATSD_STAT5                                       ,
                ULL(0x050B9342), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_ATSD_STAT6                                       ,
                ULL(0x050BB342), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_ATSD_STAT7                                       ,
                ULL(0x050BD342), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( _SM2_XTS_BDF0                                           ,
                ULL(0x05011B40), SH_UNT__SM2     , SH_ACS_SCOM      );

CONST_UINT64_T( _SM2_XTS_BDF1                                           ,
                ULL(0x05011B41), SH_UNT__SM2     , SH_ACS_SCOM      );

CONST_UINT64_T( _SM2_XTS_BDF10                                          ,
                ULL(0x05011B4A), SH_UNT__SM2     , SH_ACS_SCOM      );

CONST_UINT64_T( _SM2_XTS_BDF11                                          ,
                ULL(0x05011B4B), SH_UNT__SM2     , SH_ACS_SCOM      );

CONST_UINT64_T( _SM2_XTS_BDF12                                          ,
                ULL(0x05011B4C), SH_UNT__SM2     , SH_ACS_SCOM      );

CONST_UINT64_T( _SM2_XTS_BDF13                                          ,
                ULL(0x05011B4D), SH_UNT__SM2     , SH_ACS_SCOM      );

CONST_UINT64_T( _SM2_XTS_BDF14                                          ,
                ULL(0x05011B4E), SH_UNT__SM2     , SH_ACS_SCOM      );

CONST_UINT64_T( _SM2_XTS_BDF15                                          ,
                ULL(0x05011B4F), SH_UNT__SM2     , SH_ACS_SCOM      );

CONST_UINT64_T( _SM2_XTS_BDF2                                           ,
                ULL(0x05011B42), SH_UNT__SM2     , SH_ACS_SCOM      );

CONST_UINT64_T( _SM2_XTS_BDF3                                           ,
                ULL(0x05011B43), SH_UNT__SM2     , SH_ACS_SCOM      );

CONST_UINT64_T( _SM2_XTS_BDF4                                           ,
                ULL(0x05011B44), SH_UNT__SM2     , SH_ACS_SCOM      );

CONST_UINT64_T( _SM2_XTS_BDF5                                           ,
                ULL(0x05011B45), SH_UNT__SM2     , SH_ACS_SCOM      );

CONST_UINT64_T( _SM2_XTS_BDF6                                           ,
                ULL(0x05011B46), SH_UNT__SM2     , SH_ACS_SCOM      );

CONST_UINT64_T( _SM2_XTS_BDF7                                           ,
                ULL(0x05011B47), SH_UNT__SM2     , SH_ACS_SCOM      );

CONST_UINT64_T( _SM2_XTS_BDF8                                           ,
                ULL(0x05011B48), SH_UNT__SM2     , SH_ACS_SCOM      );

CONST_UINT64_T( _SM2_XTS_BDF9                                           ,
                ULL(0x05011B49), SH_UNT__SM2     , SH_ACS_SCOM      );

CONST_UINT64_T( PU_NPU_SM2_XTS_CONFIG                                   ,
                ULL(0x05011344), SH_UNT_PU_NPU_SM2, SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID0                                             ,
                ULL(0x05012340), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID1                                             ,
                ULL(0x05012344), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID10                                            ,
                ULL(0x05012368), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID100                                           ,
                ULL(0x050124D0), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID101                                           ,
                ULL(0x050124D4), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID102                                           ,
                ULL(0x050124D8), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID103                                           ,
                ULL(0x050124DC), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID104                                           ,
                ULL(0x050124E0), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID105                                           ,
                ULL(0x050124E4), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID106                                           ,
                ULL(0x050124E8), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID107                                           ,
                ULL(0x050124EC), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID108                                           ,
                ULL(0x050124F0), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID109                                           ,
                ULL(0x050124F4), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID11                                            ,
                ULL(0x0501236C), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID110                                           ,
                ULL(0x050124F8), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID111                                           ,
                ULL(0x050124FC), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID112                                           ,
                ULL(0x05012500), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID113                                           ,
                ULL(0x05012504), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID114                                           ,
                ULL(0x05012508), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID115                                           ,
                ULL(0x0501250C), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID116                                           ,
                ULL(0x05012510), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID117                                           ,
                ULL(0x05012514), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID118                                           ,
                ULL(0x05012518), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID119                                           ,
                ULL(0x0501251C), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID12                                            ,
                ULL(0x05012370), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID120                                           ,
                ULL(0x05012520), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID121                                           ,
                ULL(0x05012524), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID122                                           ,
                ULL(0x05012528), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID123                                           ,
                ULL(0x0501252C), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID124                                           ,
                ULL(0x05012530), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID125                                           ,
                ULL(0x05012534), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID126                                           ,
                ULL(0x05012538), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID127                                           ,
                ULL(0x0501253C), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID128                                           ,
                ULL(0x05012540), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID129                                           ,
                ULL(0x05012544), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID13                                            ,
                ULL(0x05012374), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID130                                           ,
                ULL(0x05012548), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID131                                           ,
                ULL(0x0501254C), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID132                                           ,
                ULL(0x05012550), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID133                                           ,
                ULL(0x05012554), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID134                                           ,
                ULL(0x05012558), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID135                                           ,
                ULL(0x0501255C), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID136                                           ,
                ULL(0x05012560), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID137                                           ,
                ULL(0x05012564), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID138                                           ,
                ULL(0x05012568), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID139                                           ,
                ULL(0x0501256C), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID14                                            ,
                ULL(0x05012378), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID140                                           ,
                ULL(0x05012570), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID141                                           ,
                ULL(0x05012574), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID142                                           ,
                ULL(0x05012578), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID143                                           ,
                ULL(0x0501257C), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID144                                           ,
                ULL(0x05012580), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID145                                           ,
                ULL(0x05012584), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID146                                           ,
                ULL(0x05012588), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID147                                           ,
                ULL(0x0501258C), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID148                                           ,
                ULL(0x05012590), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID149                                           ,
                ULL(0x05012594), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID15                                            ,
                ULL(0x0501237C), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID150                                           ,
                ULL(0x05012598), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID151                                           ,
                ULL(0x0501259C), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID152                                           ,
                ULL(0x050125A0), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID153                                           ,
                ULL(0x050125A4), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID154                                           ,
                ULL(0x050125A8), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID155                                           ,
                ULL(0x050125AC), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID156                                           ,
                ULL(0x050125B0), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID157                                           ,
                ULL(0x050125B4), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID158                                           ,
                ULL(0x050125B8), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID159                                           ,
                ULL(0x050125BC), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID16                                            ,
                ULL(0x05012380), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID160                                           ,
                ULL(0x050125C0), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID161                                           ,
                ULL(0x050125C4), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID162                                           ,
                ULL(0x050125C8), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID163                                           ,
                ULL(0x050125CC), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID164                                           ,
                ULL(0x050125D0), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID165                                           ,
                ULL(0x050125D4), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID166                                           ,
                ULL(0x050125D8), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID167                                           ,
                ULL(0x050125DC), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID168                                           ,
                ULL(0x050125E0), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID169                                           ,
                ULL(0x050125E4), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID17                                            ,
                ULL(0x05012384), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID170                                           ,
                ULL(0x050125E8), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID171                                           ,
                ULL(0x050125EC), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID172                                           ,
                ULL(0x050125F0), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID173                                           ,
                ULL(0x050125F4), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID174                                           ,
                ULL(0x050125F8), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID175                                           ,
                ULL(0x050125FC), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID176                                           ,
                ULL(0x05012600), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID177                                           ,
                ULL(0x05012604), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID178                                           ,
                ULL(0x05012608), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID179                                           ,
                ULL(0x0501260C), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID18                                            ,
                ULL(0x05012388), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID180                                           ,
                ULL(0x05012610), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID181                                           ,
                ULL(0x05012614), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID182                                           ,
                ULL(0x05012618), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID183                                           ,
                ULL(0x0501261C), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID184                                           ,
                ULL(0x05012620), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID185                                           ,
                ULL(0x05012624), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID186                                           ,
                ULL(0x05012628), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID187                                           ,
                ULL(0x0501262C), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID188                                           ,
                ULL(0x05012630), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID189                                           ,
                ULL(0x05012634), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID19                                            ,
                ULL(0x0501238C), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID190                                           ,
                ULL(0x05012638), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID191                                           ,
                ULL(0x0501263C), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID192                                           ,
                ULL(0x05012640), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID193                                           ,
                ULL(0x05012644), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID194                                           ,
                ULL(0x05012648), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID195                                           ,
                ULL(0x0501264C), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID196                                           ,
                ULL(0x05012650), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID197                                           ,
                ULL(0x05012654), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID198                                           ,
                ULL(0x05012658), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID199                                           ,
                ULL(0x0501265C), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID2                                             ,
                ULL(0x05012348), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID20                                            ,
                ULL(0x05012390), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID200                                           ,
                ULL(0x05012660), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID201                                           ,
                ULL(0x05012664), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID202                                           ,
                ULL(0x05012668), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID203                                           ,
                ULL(0x0501266C), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID204                                           ,
                ULL(0x05012670), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID205                                           ,
                ULL(0x05012674), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID206                                           ,
                ULL(0x05012678), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID207                                           ,
                ULL(0x0501267C), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID208                                           ,
                ULL(0x05012680), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID209                                           ,
                ULL(0x05012684), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID21                                            ,
                ULL(0x05012394), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID210                                           ,
                ULL(0x05012688), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID211                                           ,
                ULL(0x0501268C), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID212                                           ,
                ULL(0x05012690), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID213                                           ,
                ULL(0x05012694), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID214                                           ,
                ULL(0x05012698), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID215                                           ,
                ULL(0x0501269C), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID216                                           ,
                ULL(0x050126A0), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID217                                           ,
                ULL(0x050126A4), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID218                                           ,
                ULL(0x050126A8), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID219                                           ,
                ULL(0x050126AC), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID22                                            ,
                ULL(0x05012398), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID220                                           ,
                ULL(0x050126B0), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID221                                           ,
                ULL(0x050126B4), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID222                                           ,
                ULL(0x050126B8), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID223                                           ,
                ULL(0x050126BC), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID224                                           ,
                ULL(0x050126C0), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID225                                           ,
                ULL(0x050126C4), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID226                                           ,
                ULL(0x050126C8), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID227                                           ,
                ULL(0x050126CC), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID228                                           ,
                ULL(0x050126D0), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID229                                           ,
                ULL(0x050126D4), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID23                                            ,
                ULL(0x0501239C), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID230                                           ,
                ULL(0x050126D8), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID231                                           ,
                ULL(0x050126DC), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID232                                           ,
                ULL(0x050126E0), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID233                                           ,
                ULL(0x050126E4), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID234                                           ,
                ULL(0x050126E8), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID235                                           ,
                ULL(0x050126EC), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID236                                           ,
                ULL(0x050126F0), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID237                                           ,
                ULL(0x050126F4), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID238                                           ,
                ULL(0x050126F8), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID239                                           ,
                ULL(0x050126FC), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID24                                            ,
                ULL(0x050123A0), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID240                                           ,
                ULL(0x05012700), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID241                                           ,
                ULL(0x05012704), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID242                                           ,
                ULL(0x05012708), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID243                                           ,
                ULL(0x0501270C), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID244                                           ,
                ULL(0x05012710), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID245                                           ,
                ULL(0x05012714), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID246                                           ,
                ULL(0x05012718), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID247                                           ,
                ULL(0x0501271C), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID248                                           ,
                ULL(0x05012720), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID249                                           ,
                ULL(0x05012724), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID25                                            ,
                ULL(0x050123A4), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID250                                           ,
                ULL(0x05012728), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID251                                           ,
                ULL(0x0501272C), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID252                                           ,
                ULL(0x05012730), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID253                                           ,
                ULL(0x05012734), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID254                                           ,
                ULL(0x05012738), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID255                                           ,
                ULL(0x0501273C), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID26                                            ,
                ULL(0x050123A8), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID27                                            ,
                ULL(0x050123AC), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID28                                            ,
                ULL(0x050123B0), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID29                                            ,
                ULL(0x050123B4), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID3                                             ,
                ULL(0x0501234C), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID30                                            ,
                ULL(0x050123B8), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID31                                            ,
                ULL(0x050123BC), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID32                                            ,
                ULL(0x050123C0), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID33                                            ,
                ULL(0x050123C4), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID34                                            ,
                ULL(0x050123C8), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID35                                            ,
                ULL(0x050123CC), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID36                                            ,
                ULL(0x050123D0), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID37                                            ,
                ULL(0x050123D4), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID38                                            ,
                ULL(0x050123D8), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID39                                            ,
                ULL(0x050123DC), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID4                                             ,
                ULL(0x05012350), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID40                                            ,
                ULL(0x050123E0), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID41                                            ,
                ULL(0x050123E4), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID42                                            ,
                ULL(0x050123E8), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID43                                            ,
                ULL(0x050123EC), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID44                                            ,
                ULL(0x050123F0), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID45                                            ,
                ULL(0x050123F4), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID46                                            ,
                ULL(0x050123F8), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID47                                            ,
                ULL(0x050123FC), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID48                                            ,
                ULL(0x05012400), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID49                                            ,
                ULL(0x05012404), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID5                                             ,
                ULL(0x05012354), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID50                                            ,
                ULL(0x05012408), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID51                                            ,
                ULL(0x0501240C), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID52                                            ,
                ULL(0x05012410), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID53                                            ,
                ULL(0x05012414), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID54                                            ,
                ULL(0x05012418), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID55                                            ,
                ULL(0x0501241C), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID56                                            ,
                ULL(0x05012420), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID57                                            ,
                ULL(0x05012424), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID58                                            ,
                ULL(0x05012428), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID59                                            ,
                ULL(0x0501242C), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID6                                             ,
                ULL(0x05012358), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID60                                            ,
                ULL(0x05012430), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID61                                            ,
                ULL(0x05012434), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID62                                            ,
                ULL(0x05012438), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID63                                            ,
                ULL(0x0501243C), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID64                                            ,
                ULL(0x05012440), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID65                                            ,
                ULL(0x05012444), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID66                                            ,
                ULL(0x05012448), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID67                                            ,
                ULL(0x0501244C), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID68                                            ,
                ULL(0x05012450), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID69                                            ,
                ULL(0x05012454), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID7                                             ,
                ULL(0x0501235C), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID70                                            ,
                ULL(0x05012458), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID71                                            ,
                ULL(0x0501245C), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID72                                            ,
                ULL(0x05012460), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID73                                            ,
                ULL(0x05012464), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID74                                            ,
                ULL(0x05012468), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID75                                            ,
                ULL(0x0501246C), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID76                                            ,
                ULL(0x05012470), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID77                                            ,
                ULL(0x05012474), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID78                                            ,
                ULL(0x05012478), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID79                                            ,
                ULL(0x0501247C), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID8                                             ,
                ULL(0x05012360), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID80                                            ,
                ULL(0x05012480), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID81                                            ,
                ULL(0x05012484), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID82                                            ,
                ULL(0x05012488), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID83                                            ,
                ULL(0x0501248C), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID84                                            ,
                ULL(0x05012490), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID85                                            ,
                ULL(0x05012494), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID86                                            ,
                ULL(0x05012498), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID87                                            ,
                ULL(0x0501249C), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID88                                            ,
                ULL(0x050124A0), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID89                                            ,
                ULL(0x050124A4), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID9                                             ,
                ULL(0x05012364), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID90                                            ,
                ULL(0x050124A8), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID91                                            ,
                ULL(0x050124AC), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID92                                            ,
                ULL(0x050124B0), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID93                                            ,
                ULL(0x050124B4), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID94                                            ,
                ULL(0x050124B8), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID95                                            ,
                ULL(0x050124BC), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID96                                            ,
                ULL(0x050124C0), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID97                                            ,
                ULL(0x050124C4), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID98                                            ,
                ULL(0x050124C8), SH_UNT          , SH_ACS_SCOM      );

CONST_UINT64_T( PU_XTS_PID99                                            ,
                ULL(0x050124CC), SH_UNT          , SH_ACS_SCOM      );
#endif
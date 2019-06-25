/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/boot/otprom_init.S $                                      */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2019                        */
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
# # include "pk.h"
#include "sbe_link.H"
.set r0 , 0
.set r1 , 1
.set r2 , 2
.set r3 , 3
.set r4 , 4
.set r5 , 5
.set r6 , 6
.set r7 , 7
.set r8 , 8
.set d0 , 0
.set d2 , 2
.set d7 , 7
.global __reserved
__reserved:
#############################################################
################## OTPROM location data contents ############
# Magic number stored in OTP                            1806C
# 0x0040 : otprom loader
#############################################################
_start:
#lis r4 , 0x50                                   #dummy instruction
.org __reserved + 0x0040
.macro     .pm_otprom_fixed_system
         .section   .fixed, "a", @progbits
__otprom_magic_num:
    .quad   0x584950205345504D
 .endm
oos_start:
#######################################################################################
#   UPDATE bits 30:31 of sbe message register : 0x50009 with 0b01 to indicate OTPROM Control
#######################################################################################
        lis r5 , 0x5                  # load r5 with 0x00050000
        lvd d2 , 0x9(r5)              # loads data from 0x50009 to d2
        rlwinm r2, r2, 0x0, 0x0, 0x1d #Clear off the bits 30:31
        ori r2 , r2 , 0x1             # set bit 31 to 1
        stvd d2 , 0x9(r5)             # store data back into 0x50009

#######################################################################################
        # R5 --> 0x50000
        # Write into the SPI Clock divider
        # Check valid bit rate divisor is there in scratch_8 (5003F)
        #     - If yes load that value from 50039 to SPI config register
        #       The clock divider value is in the MSB 4 Nibbles
        #     - If no then let the default value stay on
        # SCK_clock_divider is 12 bit. Mask the value read and write to all the
        # SPI engines
#######################################################################################

oos_load_spi_clk_cnfg:
        lvd d2 , 0x3F(r5)                  # loads scratch_8 .
        bb0wi r2 , 1 , oos_skip_spi_clk_cnfg  # checks If valid bit (bit1) is 1
                                              # If yes cont Else branch
        lvd d2 , 0x39(r5)
        andis. r2 , r2 , 0xfff              # Pick the last 12 bits. This needs
                                           # to be written to SCK_clock_divider
        slwi r2, r2, 0x4                   # Shift left to MSB 12 bits position for easy
                                           # ORing with clock register
        #Save the clock divider in the local regsiter 0xc0002040
        #Layout of this local register is struct sbe_local_regX
        #TODO Question: Will this be same for all the SPIs?
        lis r3, 0xC000
        ori r3, r3, 0x2040
        srwi r4, r2, 0x14                   #Right align the divider value for writing
        sth r4, 0(r3)                       #Write into the local register



#######################################################################################
        # R2: Contains the clock divider register from the above block
        # R3: Pointer to the base SPI address space: 0xC00000
        # D7: Will be used a scratch register for updating the clock register

        lis r3, 0xC         # SPI BASE ADDR
        ori r3, r3, 0x40    # SPI address space for measurement seeprom.
.set spi_clock_config_reg, 0x3  # Offset of the config reg
# Set the clock divider for SPI Engine 0
        lvd d7, spi_clock_config_reg(r3)
        rlwinm r7, r7, 0x0, 0xC, 0x1F       # Zero the first 12bits
        or r7, r7, r2                       # Put the clock divider bits
        stvd d7, spi_clock_config_reg(r3)   # Write to config reg

oos_write_spi_clk_cnfg:

oos_skip_spi_clk_cnfg:

oos_write_mode_done:


####################### READ Magic number from SEEPROM ######################
                # R5: Contains 0x50000
# Read out Selfboot control status register(50008) for port number
# Write that value into SPI Config Clk register and start the magic number read
    
oos_chk_port_num:
#                lvd d2 , 0x8(r5)                   #Read the port number from Selfboot control / status register ::
#                bb0wi r2 , 17 , oos_sel_prim_sprm  #Check If backup seeprom select is '1' bit_17 according to Srinivas
#                ori r0 , r0 , 0x0200               #enable backup_sprm port
#                lvd d7, -24(r6)                    #load SBE_CONFIG local reg
#                oris r8 , r8 , 0x0200              #make bit 38 of sbe_config_reg bit '1'. (C0000800)
#                stvd d7, -24(r6)                   #Store SBE_CONFIG local reg


oos_sel_prim_sprm:
#Read from 0th location of SEEPROM for magic number
                lis r4 , OTPROM_SEEPROM_HIGHER_NIBBLE
                ori r4 , r4 , 0x0000
                lvd d0 , 0(r4)

        # compared the seeprom data with 0x584950205345504D
        # If doesn't match write the error msg into 0x50009

oss_load_constant_magic_num:                       #Magic number of seeprom = 0x584950205345504D
                lis r2 , 0x5849                    #load constant otprom magic number
                ori r2 , r2 , 0x5020
                lis r3 , 0x5345
                ori r3 , r3, 0x504d
                cmplwbc 0, 2, r0, r2, oos_cmp_magic_fail
                cmplwbc 0, 2, r1, r3, oos_cmp_magic_fail

           ##### Branch to SEEPROM ###################
                lis r4 , OTPROM_SEEPROM_HIGHER_NIBBLE #Go and fetch the branch address from 0x8000_0001
                ori r4 , r4 , 0x0008
                lvd d0 , 0(r4)
                mtctr r1
                bctr                                 #Branch to fetched address
           ##### Branch to SEEPROM ###################

oos_cmp_magic_fail:
                trap                               #FIXME hve to give branch to SEEPROM

.pm_otprom_fixed_system

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
.set r1 , 0
.set r2 , 0
.set r3 , 0
.set r4 , 0
.set r5 , 0
.set r6 , 0
.set r7 , 0
.set r8 , 0
.set d0 , 0
.set d2 , 0
.set d7 , 0
.global __reserved
__reserved:
#############################################################
################## OTPROM location data contents ############
# Magic number stored in OTP                            1806C
# 0x0040 : otprom loader
#############################################################
_start:
#lis r4 , 0x50	                               #dummy instruction
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
        lis r5 , 0x5               # load r5 with 0x00050000
        lvd d2 , 0x9(r5)           # loads data from 0x50009 to d2
        ori r2 , r2 , 0x0001	   # set bit 31 to 1

		lis r6 , 0xFFFF			   #  clear bit 30
		ori r6 , r6 ,0xFFFD
		and r2 , r2 , r6
		stvd d2 , 0x9(r5)		   # store data back into 0x50009

#######################################################################################
        #R4 --> A0000   R5 --> 5003F   R6 --> C000_0818
        # Write into I2CM reset register
        # Check valid bit rate divisor is there in scratch_8 (5003F)
        #     - If yes load that value from 50039 to SPI config register
        #     - If no then let the default value stay on

#######################################################################################	

oos_load_spi_clk_cnfg:
                lis r5 , 0x5                       #Check the validity of scratch reg
                #ori r5 , r5 , 0x0000
                lvd d2 , 0x3F(r5)                  #loads scratch_8 and updates R5 to scratch_1
                bb0wi r2 , 1 , oos_skip_spi_clk_cnfg  #checks if valid bit (bit1) is 1 if yes continue else branch
                lvd d2 , 0x39(r5)
                andis. r2 , r2 , 0xffff            #delete last 2 bytes Confirmed first 2 bytes has Bit rate divisor

oos_write_spi_clk_cnfg:

oos_skip_spi_clk_cnfg:

oos_write_mode_done:


####################### READ Magic number from SEEPROM ######################
# Read out Selfboot control status register(50008) for port number
# Write that value into SPI Config Clk register and start the magic number read
	
oos_chk_port_num:
#                lvd d2 , 0x8(r5)                   #Read the port number from Selfboot control / status register ::
#                bb0wi r2 , 17 , oos_sel_prim_sprm  #Check if backup seeprom select is '1' bit_17 according to Srinivas
#                ori r0 , r0 , 0x0200               #enable backup_sprm port
#                lvd d7, -24(r6)                    #load SBE_CONFIG local reg
#                oris r8 , r8 , 0x0200              #make bit 38 of sbe_config_reg bit '1'. (C0000800)
#                stvd d7, -24(r6)                   #Store SBE_CONFIG local reg


oos_sel_prim_sprm:
#Read from 0th location of SEEPROM for magic number using FI2C
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

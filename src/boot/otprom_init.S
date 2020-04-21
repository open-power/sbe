/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/boot/otprom_init.S $                                      */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2020                        */
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
.set r9 , 9
.set r10, 10
.set r29, 29
.set d0 , 0
.set d2 , 2
.set d3 , 3
.set d6 , 6
.set d7 , 7
.set d8 , 8
.set d9 , 9
.set d29, 29
.global __reserved
__reserved:
###############################################################################
################## OTPROM location data contents ##############################
# Magic number stored in OTP        1806C
# We have directly loaded the register with liw instead of a global variable
# 0x0040 : otprom loader
###############################################################################
_start:
.org __reserved + 0x0040
.macro     .pm_otprom_fixed_system
           .section  .fixed, "a", @progbits
oos_cmp_magic_fail:
    trap             #Fixed location for TRAP instruction
.endm
oos_start:

################################################################################
## Set DBCR bit7, this will enable trap instruction to halt the processor and
## not cause a program exception. Ultimately program exception will also halt
## the processor but there will be jump, with DBCR bit7 set, there is no jump
## to the program exception vector.
################################################################################
    lis    r5, 0x100
    mtdbcr r5
################################################################################
#   UPDATE bits 28:29:30:31 of sbe message register : 0x50009 with 0b01
#   to indicate OTPROM Control
#   Through out the code R5 is 0x00050000, R4 is 0xC0000000, Don't alter this
################################################################################
    lis    r5, 0x5                 # load r5 with 0x00050000
    lis    r4, 0xC000              # load r4 with 0xC0000000
    lvd    d2, 0x9(r5)             # loads data from 0x50009 to d2
    rlwinm r2, r2, 0x0, 0x0, 0x1b  # Clear off the bits 28:29:30:31
    ori    r2, r2, 0x1             # set bit 31 to 1
    clrlwi r2, r2, 1               # clear bit 0 of 0x50009 (SBE Booted flag)
    stvd   d2, 0x9(r5)             # store data back into 0x50009

.set g_hreset_path, 31             # r31 will be used as global variable
    li g_hreset_path, 1            # Init the global variable to 1

# Check for HRESET trigger from 50008:bit13 (HARDWARE Trigger)
## use r5 wherever you need reference for 0x00050000
    lvd   d2, 0x8(r5)
    bb1wi r2, 13, hreset_path   # Jump to hreset function
    li    g_hreset_path, 0      # Not HRESET path. Unset the global variable

# Check for IPL Reset Trigger from 50008:bit12 (HARDWARE Trigger)
    bb0wi r2, 12, skip_bit12_pull_down   # Jump to hreset function
    # Bit12 is level trigger interrupt, pull down bit to take PPE out of reset
    xoris r2, r2, 0x0008        # Bit12 is pulled down here, XOR will flip bit12
    stvd  d2, 0x8(r5)           # write to 50008 to pull down bit 12

# Scan0 init sequence of c0002040 is expected to be all zeros. Not trying to
# reset other bits here since it is fresh start
# Set bit15 in LFR, to indicate Vector0(50008:12) reset triggered in FW
    lis   r8, 0x1
    li    r9, 0x0             # d8 is 0001_0000_0000_0000
    stvd  d8, 0x2040(r4)      # Write the mask to set the 15th bit
                              # this is an exception to use 2040 instead 2050

skip_bit12_pull_down:
# No bit:12 and No bit:13, check if FW triggerred for MPIPL
# Check for Mpipl trigger from C0002040:bit14 (FIRMWARE Trigger)
    lvd   d2, 0x2040(r4)
    bb1wi r2, 14, mpipl_path  #check if mpipl set in sbe_local_regX (LFR)

################################################################################
    # R5 --> 0x50000
    # Check valid bit is set in scratch_8 (5003F) for clock divider & round trip
    #   - If yes load that value from 5003B to LFR/SPIm Clock Divider
    #       The clock divider value is in the MSB 12 bits of 5003B
    #       The round trip delay is the bit 12-15 of 5003B
    #   - If no then let the default value stay
    # SCK_clock_divider is 12bits (0-11 bits). Round trip delay is the next
    # 4bits (12-15) in the register. Round trip delay is an enum from user
    #       b0000 -> 0x80 - No Delay
    #       b0001 -> 0x40 - 1 clock cycle delay
    #       b0010 -> 0x20 - 2 clock cycle delay
    #       b0011 -> 0x10 - 3 clock cycle delay
    #       b0100 -> 0x08 - 4 clock cycle delay
    #       b0101 -> 0x04 - 5 clock cycle delay
    #       b0110 -> 0x02 - 6 clock cycle delay
    #       b0111 -> 0x01 - 7 clock cycle delay
    #       b1XXX -> 0x00 - receiving clocking disabled
    # Mask the value read and write to all the
    # SPI engines
################################################################################

#Note: Scratch register is not used on the hreset/mpipl path. THIS is IPL Path.
#      Move the values from scratch register to local registers

oos_load_spi_clk_cnfg:
    lvd    d2, 0x3F(r5)                    # loads scratch_8 .
    bb0wi  r2, 3, spi_clk_cnfg_not_valid   # checks If valid Bit3 is Set
    lvd    d2, 0x3B(r5)                    # Valid. Load the clock divider
    rlwinm r2, r2, 0x0, 0x0, 0xF           # Pick the MSB 16 bits (12 + 4 bits).
    b save_spi_cnfg

spi_clk_cnfg_not_valid:
    # Config not valid, load default value of 0x004 in the top 12 bits and let
    # round trip delay be 1 clock cycle in next 4bits (Enum for 1 clock delay)
    lis r2, 0x0041        # 12bits clock divider + 4bits round trip delay
                          # round trip delay enum is 1 which is 0x40

    #Save the clock divider in the local regsiter 0xc0002040
    #Use the clear reg 0xC0002058 and OR reg at 0xC0002050 to set 0xC0002040
    #Layout of this local register is struct sbe_local_regX
save_spi_cnfg:
    lis    r8, 0xfff0     # Set the mask to reset the top 12bits (clock divider)
    ori    r8, r8, 0x0f00 # Set mask to reset bit20-23 (round trip delay)
    li     r9, 0x0        # d8 is fff0_0f00_0000_0000
    stvd   d8, 0x2058(r4) # Write the mask to clear the top 12bits + bit20-23
    andis. r6, r2, 0xfff0 # fetch the first 12bits from r2 and write to LFR
    li     r7, 0
    stvd   d6, 0x2050(r4) # Write the divider value into the OR reg c0002050
    andis. r6, r2, 0x000f # fetch the bits 12-15 from r2
    srawi  r6, r6, 0x8    # shift the content by 8bits towards right
    stvd   d6, 0x2050(r4) # Write the round trip delay into the OR reg c0002050

## For the new flow: This needs to be broken into two parts
# part 1: Check the external config reg and if valid, latch to local register.
# part 2: Check the local reg and set the ports if secondary
# part 1 will happen only in ipl path. part 2 will happen in all paths.

########################### This is IPL PATH #############################
## Note: Layout out for 0xC0002040 is sbe_local_LFR
# Use the clear reg 0xC0002058 and OR reg at 0xC0002050 to set 0xC0002040
# First reset the bits sec_boot_seeprom and sec_meas_seeprom bit in 0xC0002040
# r4 is coming from up with value as c0000000
# r5 is coming from up with value as 50000
    lis   r6, 0xC        # Set the clear bit for 12 and 13 bit,r7 is 0 already
    stvd  d6, 0x2058(r4) # Clears bit 12 and 13 in c0002050
    lvd   d2, 0x8(r5)    # Read the port number from Selfboot CS reg
    li    r6, 0          # Set this to zero, to be used as primary default
    bb0wi r2, 17, sel_prim_boot_seeprom  # Check If sec boot seeprom is selected
    oris  r6, r6, 0x8    # Using sec boot seeprom. Set sec_boot_seeprom in LFR

sel_prim_boot_seeprom:
    bb0wi r2, 18, sel_prim_meas_seeprom  # Check if backup measurement is selected
    oris  r6, r6, 0x4    # Using sec meas seeprom.Set sec_meas_seeprom in LFR

sel_prim_meas_seeprom:   # At this point, boot and measurement seeprom selection is done in r6
    stvd  r6, 0x2050(r4) # Set bits in OR regsiter for sec_boot_seeprom and
                         # sec_meas_seeprom. r7 is already 0
############################################################################

mpipl_path:
# MPIPL path will reset both Boot/Measurement SPIs
# Load the clock divider value from the local register
    lvd   d8, 0x2040(r4)
    bb0wi r8, 14, skip_lfr_bit_flush
## Unset bit15, bit16, bit17, bit18 to indicate MPIPL path only
## Bit15 Ipl Reset, Bit16 Hreset, Bit17 HReset Done, Bit18 Mpipl Done
    lis   r8, 0x1            # Set the mask to reset the bit 15
    ori   r8, r8, 0xe000     # Set the mask to reset the bit 16/17/18
    li    r9, 0x0            # d8 is 0x0001_E000_0000_0000
    stvd  d8, 0x2058(r4)     # write to WO_CLR Register
skip_lfr_bit_flush:
## Fetch Clock Divider and seeprom selection from LFR
    lvd   d9, 0x2040(r4)     # Load the latched value from the local register
    srwi  r3, r9, 0x14       # Move the clock divider value to LSB
    srwi  r2, r9, 0x8        # Move bit20-23 to LSB, round trip delay bits
    andi. r2, r2, 0x000f     # Mask all other bits except the last nibble
    li    r10,0x80           # this is the value to be updated for zero round
                             # trip delay
    sraw  r2, r10, r2        # shift right 0x80 by number defined in r2
    li    r1, 0              # R1: Load with primary meas seeprom
    bb0wi r9, 13, enable_prim_meas_spi  # Check if prim meas seeprom is selected
                                        # Check sec_meas_seeprom bit
    li    r1, 1              # Backup meas seeprom should be enabled
enable_prim_meas_spi:
    bl init_measurement_spi # Note: r1,r2,r3 are the parameters to this
                            # R1 -> Primary/Secondary Measurement Seeprom
                            # R2 -> Round trip delay enum
                            # R3 -> Clock Divider

hreset_path:
## Bit13 is a level trigger interrupt, pull down bit to take PPE out of reset
## use r5 wherever you need reference for 0x00050000
    lvd   d2, 0x8(r5)
    bb0wi r2, 13, skip_bit13_pull_down
    xoris r2, r2, 0x0004   # Bit13 is pulled down here
    stvd  d2, 0x8(r5)

## HRESET path will reset only Boot SPIs
# Set bit 16 in LFR, to indicate Vector1 reset triggered in FW
    li    r8, 0x0
    ori   r8, r8, 0x8000
    li    r9, 0x0          # d8 is 0000_8000_0000_0000
    stvd  d8, 0x2050(r4)   # Write the mask to set the 16th bit with W_OR
# Unset Bit15 (IPL Reset) Bit14 (MPIPL) Bit17 (HReset Done) Bit18 (MPIPL Done)
# to clear off previous triggers, in order to keep only the current trigger
    lis   r8, 0x3          # Set the mask to reset the bit 14/15
    ori   r8, r8, 0x6000   # Set the mask to reset the bit 17/18
    li    r9, 0x0          # d8 is 0x0003_4000_0000_0000
    stvd  d8, 0x2058(r4)   # write to WO_CLR Register

skip_bit13_pull_down:
    lvd   d9, 0x2040(r4)   # Load the latched value value from local regsiter
    srwi  r3, r9, 0x14     # Move the clock divider value to LSB
    srwi  r2, r9, 0x8      # Move bit20-23 to LSB, round trip delay bits
    andi. r2, r2, 0x000f   # Mask all other bits except the last nibble
    li    r10,0x80         # this is the value to be updated for zero round
                           # trip delay
    sraw  r2, r10, r2      # shift right 0x80 by number defined in r2
    li    r1, 0            # R1: Load with primary seeprom
    bb0wi r9, 12, enable_prim_boot_spi  # Check if prim boot is selected.
                                        # Check sec_seeprom bit
    li    r1, 1            # Secondary boot seeprom should be enabled
enable_prim_boot_spi:
    bl init_boot_spi       # Note : r1,r2,r3 are the parameters to this
                           # R1 -> Primary/Secondary Boot Seeprom
                           # R2 -> Round trip delay
                           # R3 -> Clock Divider

#Check if we are in hreset path
    bb1wi  g_hreset_path, 31, jump_to_boot
    #If not, we are in IPL or MPIPL path. Need to jump to measurement
    b      jump_to_measurement
jump_to_boot:
    ## Let's set the PIB SPIm Selection in Root Control Register 50018:0:1
    ## Boot seeprom should be handled in this path (HReset Path)
    lvd    d8, 0x18(r5)            # load/fetch 50018 onto r8/r9
    rlwinm r8, r8, 0x0, 0x2, 0x1f  # Set Bit0 for SPIm[0] and Bit1 for SPIm[1]
    stvd   d8, 0x18(r5)            # store 50018 with bit0 and bit1 as SET
    ## Don't change r3 here on
    lis    r3, OTPROM_SEEPROM_HIGHER_NIBBLE
    bl     check_magic_number      # Verify the magic number
    lis    r6, OTPROM_SEEPROM_HIGHER_NIBBLE
    b      jump_to_next_seeprom

jump_to_measurement:
    ## Let's set the PIB SPIm Selection in Root Control Register 50018:0:1:2:3
    ## Both Boot and Measurement seeprom should be handled in this path (IPL
    ## Reset & MPIPL Path)
    lvd    d8, 0x18(r5)            # load/fetch 50018 onto r8/r9
    rlwinm r8, r8, 0x0, 0x4, 0x1f  # Set Bit0 for SPIm[0] and Bit1 for SPIm[1]
                                   # Set Bit2 for SPIm[2] and Bit3 for SPIm[3]
    stvd   d8, 0x18(r5)            # store 50018 with bit0/bit1/bit2/bit3 as SET
    ## Here we verify magic number for both boot and measurement
    ## Don't change r3 here on
    lis    r3, OTPROM_SEEPROM_HIGHER_NIBBLE
    bl     check_magic_number   #Verify the magic number
    lis    r3, OTPROM_MEASUREMENT_HIGHER_NIBBLE
    bl     check_magic_number
    lis    r6, OTPROM_MEASUREMENT_HIGHER_NIBBLE

jump_to_next_seeprom:
    ########### Branch to NEXT SEEPROM ###################
    #Note: r6 still contains NIBBLE for the target seeprom
    lvd    d0 , 0x8(r6)
    mtctr  r1
    bctr                 #Branch to fetched address
    ########## Branch to SEEPROM ###################

#Pass the memory address of the magic number to be verified in r3
#If the match fail, this API will directly trap and will not return
check_magic_number:
    lvd d3, 0x0(r3) #Read the magic number
    lis r1, 0x5849
    ori r1, r1, 0x5020
    lis r2, 0x5345
    ori r2, r2, 0x504d
    cmplwbc 0, 2, r3, r1, oos_cmp_magic_fail
    cmplwbc 0, 2, r4, r2, oos_cmp_magic_fail
    blr


#This api will be used to reset the boot SPI (both primary/backup).
#0xC0000 and 0xC0020 are the two boot SPI (TODO: Verify these)
# R1: 0 to enable primary. 1 to enable secondary.
# R2: Round trip delay bits to be set
# R3: The clock divider value to be set.
# As per the SPI Spec, mmSPIsm_enable is enabled by default at reset.
# Code not written to use this fact. Simics doesn't seem to set it by default.
# If behaviour is confirmed then code can be optimized to reduce 1-2
# instructions per SPI.
.set spi_config1_reg, 0x2       # Offset of config1 reg
.set spi_clock_config_reg, 0x3  # Offset of clock config reg

init_boot_spi:
## Reset the boot SPIs. They are eng 0 and 1 (TODO: Verify this)
# To reset, use the OR reg 0xC0002010 to set the bits
    lis    r6, 0xC000
    ori    r6, r6, 0x2000
    lis    r7, 0xC0       # Bit 8 and 9
    li     r8, 0
    stvd   d7, 0x10(r6)   # Set pin for eng0 and eng1 to high using c0002010
    stvd   d7, 0x18(r6)   # Reset pin for eng0 and eng1 to low using c0002018

# Primary BOOT SPI
    lis    r6, 0xC        # SPI BASE ADDR

    # Load the clock_config_reg & set the clock divider, round trip delay and
    # mmSPI_enable bit for SPI Engine to be set
    lvd    d7, spi_clock_config_reg(r6)
    rlwinm r7, r7, 0x0, 0x14, 0x1F   # Zero the first 20bits (12 + 8)
    slwi   r3, r3, 0x14              # We need clock divider in 0..11th bits
    slwi   r2, r2, 0xC               # We need round trip delay in 12..19th bits
    or     r7, r7, r3                # Put the clock divider bits
    or     r7, r7, r2                # Put the round trip delay bits
    rlwinm r7, r7, 0x0, 0, 0x1E      # Zero out the mmSPIsm_enable bit31
    bb1wi  r1, 31, save_prim_boot_config # sec seeprom is selected.Save config
    ori    r7, r7, 0x1                   # Enable mmSPIsm_enable bit
save_prim_boot_config:
    stvd   d7, spi_clock_config_reg(r6)  # Write to config reg

    #Backup BOOT SPI
    lis    r6, 0xC
    ori    r6, r6, 0x20                  # Backup SPI BASE ADDR

    # Load the clock_config_reg & set the clock divider, round trip delay and
    # mmSPI_enable bit for SPI Engine to be set
    lvd    d7, spi_clock_config_reg(r6)
    rlwinm r7, r7, 0x0, 0x14, 0x1F       # Zero the first 20bits (12 + 8)
    or     r7, r7, r3     # Put clock divider. Note r3 is already left shifted
    or     r7, r7, r2     # Put round trip delay.Note r2 is already left shifted
    rlwinm r7, r7, 0x0, 0, 0x1E          # Zero out the mmSPIsm_enable bit31
    bb0wi  r1, 31, save_sec_boot_config  # Prim seeprom is selected. Save config
    ori    r7, r7, 0x1                   # Enable mmSPIsm_enable bit
save_sec_boot_config:
    stvd   d7, spi_clock_config_reg(r6)   # Write to config reg
    blr

#This api will be used to reset the measurement SPI (both primary/backup).
#Parameters to this function are:
# R1: 0 to enable primary. 1 to enable secondary.
# R2: Round trip delay enum to be set
# R3: The clock divider value to be set.
# As per the SPI Spec, mmSPIsm_enable is enabled by default at reset.
# Code not written to use this fact. Simics doesn't seem to set it by default.
# If behaviour is confirmed then code can be optimized to reduce 1-2
# instructions per SPI
## 0xC0060(Pri) and 0xC0040(Sec) are the two measurement SPI
init_measurement_spi:
## Reset the boot SPIs. They are eng 2 and 3 (TODO: Verify this)
# To reset, use the OR reg 0xC0002010 to set the bits
    lis    r6, 0xC000
    ori    r6, r6, 0x2000
    lis    r7, 0x30        # Bit 10 and 11
    li     r8, 0
    stvd   d7, 0x10(r6)   # Set pin for eng2 and eng3 to high using c0002010
    stvd   d7, 0x18(r6)   # Reset pin for eng2 and eng3 to low using c0002018

    #Primary Measurement SPI
    lis    r6, 0xC
    ori    r6, r6, 0x60              # Primary SPI BASE ADDR (0xC0060)
    # Load the clock_config_reg & set the clock divider, round trip delay and
    # mmSPI_enable bit for SPI Engine to be set
    lvd    d7, spi_clock_config_reg(r6)
    rlwinm r7, r7, 0x0, 0x14, 0x1F   # Zero the first 20bits (12 + 8)
    slwi   r3, r3, 0x14              # We need clock divider in 0..11th bits
    slwi   r2, r2, 0xC               # We need round trip delay in 12..19th bits
    or     r7, r7, r3                # Put the clock divider bits
    or     r7, r7, r2                # Put the round trip delay bits
    rlwinm r7, r7, 0x0, 0, 0x1E      # Zero out the mmSPIsm_enable bit31

    #Set the lock bit for Primary Seeprom
    lis    r29, 0x8000
    ori    r29, r29, 0x0

    bb1wi  r1, 31, save_prim_meas_config # sec seeprom is selected.Save config
    ori    r7, r7, 0x1                   # Enable primary
    stvd   d29, spi_config1_reg(r6)      # Set lock bit for Prim Meas Seeprom

save_prim_meas_config:
    stvd d7, spi_clock_config_reg(r6)    # Write to config reg

    lis    r6, 0xC
    ori    r6, r6, 0x40                 # Secondary SPI BASE ADDR (0xC0040)
    # Load the clock_config_reg & set the clock divider, round trip delay and
    # mmSPI_enable bit for SPI Engine to be set
    lvd    d7, spi_clock_config_reg(r6)
    rlwinm r7, r7, 0x0, 0x14, 0x1F      # Zero the first 20bits (12 + 8)
    or     r7, r7, r3    # Put clock divider. Note r3 is already left shifted
    or     r7, r7, r2    # Put round trip delay. Note r2 is already left shifted
    rlwinm r7, r7, 0x0, 0, 0x1E         # Zero out the mmSPIsm_enable bit31
    bb0wi  r1, 31, save_sec_meas_config # Prim seeprom is selected.Save config
    ori    r7, r7, 0x1                  # Enable Backup Measurement
    stvd   d29, spi_config1_reg(r6)     # Set lock bit for Prim Meas Seeprom

save_sec_meas_config:
    stvd   d7, spi_clock_config_reg(r6) # Write to config reg
    blr

.pm_otprom_fixed_system

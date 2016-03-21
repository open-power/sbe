# # include "pk.h"
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
# SBE Seeprom Config_0                                  18068
# SBE Seeprom Config_1                                  18069
# SBE Seeprom Config_2                                  1806A
# SBE Seeprom Config_3                                  1806B
# Magic number stored in OTP                            1806C
# Reserved                                              1806D
# Reserved                                              1806E
# Reserved                                              1806F
# 0x0040 : otprom loader
#############################################################
_start:
lis r4 , 0x50	                               #dummy instruction
.org __reserved + 0x0040
.macro     .pm_otprom_fixed_system
         .section   .fixed, "a", @progbits
__seeprom0_config_reg:
    .quad   0x80A91C7100000000
__seeprom1_config_reg:
    .quad   0x80AB1C7100001C71
__seeprom2_config_reg:
    .quad   0x80AD1C71000038E2
__seeprom3_config_reg:
    .quad   0x80AF1C7100005553
__otprom_magic_num:
    .quad   0x584950205345504D
__sbe_config_reg:
    .quad   0x000A800060000000
 .endm
oos_start:                                         #R4 --> A0000   R5 --> 5003F   R6 --> C000_0818
                lis r4 , 0xA
                ori r4 , r4 , 0x0
                stvd d0 , 1(r4)                    #Write reset reg . A0001
                lis r5 , 0x5                       #Check the validity of scratch reg and then program the bit rate div
                ori r5 , r5 , 0x0000
                lvd d2 , 0x3F(r5)                  #loads scratch_8 and updates R5 to scratch_1
                bb0wi r2 , 1 , oos_load_const_brd  #checks if valid bit (bit1) is 1 if yes continue else branch
                lvd d2 , 0x39(r5)
                andis. r2 , r2 , 0xffff            #delete last 2 bytes Confirmed first 2 bytes has Bit rate divisor

oos_write_mode_reg:
                lvd d0 , 6(r4)                     #D0 --> old mode reg and D2 --> has new brd
                andi. r0 , r0 , 0xffff
                or r0 , r0 , r2
                stvd d0 , 6(r4)                    #Store mode register
                lis r6 , 0xc000
                ori r6 , r6, 0x0818                #Load clear address of local register FI2C_CFG
                li r0 , 0xfff                      #Create the Clear mask
                lis r1 , 0xf000
                stvd d0 , 0(r6)                    #write to clear register of local register
                rlwinm r3, r2, 12 , 0 , 3          #First move last nibble to R3 0:3
                rlwinm r2, r2, 12 , 20, 31         #Store bit rate div in 20:35 location of local register 20:31
                stvd d2 , -8(r6)                   #Set register = C0000810
                b oos_write_mode_done

oos_load_const_brd:
                lis r2 , 0x6                       #load constant BRD = 6 to first 2 bytes of R2
                b oos_write_mode_reg

oos_write_mode_done:

oos_force_stop_to_both_ports:
                lis r0 , 0x8000                    #load 0x80000000 for port busy register write
                stvd d0 , 0xE(r4)                  #Write port busy register to clear
                andi. r0 , r0 ,0                   #stop_command = 0x1000_0020_0000_0000
                andi. r1 , r1 , 0
                oris r0 , r0 , 0x1000
                ori r0 , r0 , 0x0020
                stvd d0 , 0(r4)                    #Write control register with Stop command

oos_poll_status_bit_0:
                lvd d2 , 2(r4)                     #Read Status register
                bb1wi  r3 , 0xc , oos_poll_status_bit_0
                ori r0 , r0 , 0x0200               #stop_command = 0x1000_0220_0000_0000
                stvd d0 , 0(r4)                    #write control register with stop command to port 1

oos_poll_status_bit_1:
                lvd d2 , 2(r4)
                bb1wi r3 , 0xc , oos_poll_status_bit_1
                lis r0 , 0xd8a9
                ori r0 , r0 , 0x0090               #change the port number later after checking from the scratch register. Add the seeprom address pointer location by updating the R1
                andi. r1 , r1 , 0x0                #Chose address 0x0 of SEEPROM : 0xA8

oos_chk_port_num:
                lvd d2 , 0x8(r5)                   #Read the port number from Selfboot control / status register ::
                bb0wi r2 , 17 , oos_sel_prim_sprm  #Check if backup seeprom select is '1' bit_17 according to Srinivas
                ori r0 , r0 , 0x0200               #enable backup_sprm port
                lvd d7, -24(r6)                    #load SBE_CONFIG local reg
                oris r8 , r8 , 0x0200              #make bit 38 of sbe_config_reg bit '1'. (C0000800)
                stvd d7, -24(r6)                   #Store SBE_CONFIG local reg

oos_sel_prim_sprm:
                stvd d0 , 0(r4)                    #write control register

oos_poll_status_bit_2:
                lvd d2 , 2(r4)                     #poll status reg
                bb1wi r3 , 0xc , oos_poll_status_bit_2
                lvd d0 , 3(r4)                     #Read data reg to get magic number

oss_load_constant_magic_num:                       #Magic number of seeprom = 0x584950205345504D
                lis r2 , 0x5849                    #load constant otprom magic number
                ori r2 , r2 , 0x5020
                lis r3 , 0x5345
                ori r3 , r3, 0x504d
                cmplwbc 0, 2, r0, r2, oos_cmp_magic_fail
                cmplwbc 0, 2, r1, r3, oos_cmp_magic_fail
                lis r4 , 0x0001                    #OTPROM address = 0xZZZZ_YYYY    PIB_addr (R4 = 18070)
                ori r4 , r4 , 0x8068
                lis r5 , 0xc000                    #local_reg_addr (R5 = C0000860)
                ori r5 , r5 , 0x0860
                lvd d0 , 0(r4)
                stvd d0 , 0(r5)                    #Seeprom_0
                lvd d0 , 1(r4)
                stvd d0 , 32(r5)                   #Seeprom_1
                lvd d0 , 2(r4)
                stvd d0 , 64(r5)                   #Seeprom_2
                lvd d0 , 3(r4)
                stvd d0 , 96(r5)                   #Seeprom_3
                lvd d0 , 5(r4)                     #SBE Config
                stvd d0 , -96(r5)
           ##### Branch to SEEPROM ###################
                lis r4 , 0x8000                    #Go and fetch the branch address from 0x8000_0001
                ori r4 , r4 , 0x0008
                lvd d0 , 0(r4)
                mtctr r1
                bctr                               #Branch to fetched address
           ##### Branch to SEEPROM ###################
                trap                               #FIXME hve to give branch to SEEPROM

oos_cmp_magic_fail:
                lis r5 , 0x5                       #PIB_addr (R5 = 0x00050008)
                ori r5 , r5 , 0x0000
                lvd d0 , 8(r5)
                andi. r1 , r1 , 0xfff0
                ori r1 , r1 , 0x000e               #59:63 : Error message : E = Magic number mismatch
                stvd d0 ,8(r5)
                trap

.pm_otprom_fixed_system

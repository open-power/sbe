/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/core/sbes1handler.C $                               */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2022                             */
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

#include "sbes1handler.H"
#include "sbetrace.H"

void __wait_for_s1()
{
    SBE_INFO("Going into virtual halt (wait) state...");

    asm(
        // Disable External Intrrupts
        " wrteei 0\n "

        // load r4 with 0xC0000000
        " lis    %r4, 0xC000\n "
        // CLear r8
        " clrlwi %r8, %r8, 31\n"
        // Clear bit 3 and set all other bits in SBE_LCL_EIMR
        // i.e Disable all interrupts except S1 interrupt
        " not    %r8, %r8\n "
        " rlwinm %r1, %r8, 0x0, 4, 2\n "
        // Write data to 0xC0000020 (SBE_LCL_EIMR) from d1
        " stvd   %d1, 0x0020(%r4)\n "

        // Load r5 with 0x00050000
        " lis    %r5, 0x5\n "
        // loads data from 0x50009 to d2
        " lvd    %d2, 0x9(%r5)\n "
        // Copy the contents of r2 into r9
        " mr     %r9, %r2\n "
        // Clear current state and previous state
        " rlwinm %r2, %r2, 0x0, 0xB, 0x4\n"
        // Clear everyting except current state and move it to previous state
        " rlwinm %r9, %r9, 0x4, 0x4, 0x7\n "
        // Update current state to SBE_STATE_WAIT_FOR_S1(0xB). Refer sbestates.H
        " oris   %r9, %r9, 0xB0\n "
        // update state
        " or     %r2, %r2, %r9\n "
        // Create a mask to set bit 2
        " rlwinm %r1, %r8, 0x0, 2, 2\n "
        // Set bit 2 to indicate PPE supports s1 interrupt. This is cleared above
        " or     %r2, %r2, %r1\n "
        // Create a mask to set bit 0
        " rlwinm %r1, %r8, 0x0, 0, 0\n "
        // Set bit 0 to indicate PPE is booted up. This is cleared above
        " or     %r2, %r2, %r1\n "
        // Store data back into 0x50009
        " stvd   %d2, 0x9(%r5)\n "

        " wait_for_s1_loop:\n "
        // Load data from 0xC0000000 (SBE_LCL_EISR)
        " lvd    %d2, 0x0(%r4)\n "
        // Check if S1 is triggered
        " bb1wi  %r2, 0x3, __s1_interupt_handler\n"
        " b wait_for_s1_loop\n "
    );
}

void __s1_interupt_handler()
{

    SBE_INFO("S1 interrupt triggered. HRESET PPE...");

    asm(
        // load r4 with 0xC0000000
        " lis    %r4, 0xC000\n "
        // Load data from 0xC0000000 (SBE_LCL_EISR)
        " lvd    %d2, 0x0(%r4)\n "
        // Clear bit3 in SBE_LCL_EISR (S1 status)
        " rlwinm %r2, %r2, 0x0, 4, 2\n "
        // Write data to 0xC0000000 (SBE_LCL_EISR) from d1
        " stvd   %d2, 0x0(%r4)\n "

        // Load r5 with 0x00050000
        " lis    %r5, 0x5\n "
        // loads data from 0x50008 to d2
        " lvd    %d2, 0x8(%r5)\n "
        // Clear bit 15 (s1 intr)
        " rlwinm %r2, %r2, 0x0, 16, 14\n "
        // Store data back into 0x50008
        " stvd   %d2, 0x8(%r5)\n "

        // loads data from 0x50009 to d2
        " lvd    %d2, 0x9(%r5)\n "
        // clear bit 0 of 0x50009 (SBE Booted flag)
        " clrlwi %r2, %r2, 1\n "
        // store data back into 0x50009
        " stvd   %d2, 0x9(%r5)\n "

        // Set bit 16 in LFR, to indicate S1 reset(HRESET) triggered in FW
        " li    %r8, 0x0\n "
        " ori   %r8, %r8, 0x8000\n "
        " li    %r9, 0x0\n "           // d8 is 0000_8000_0000_0000
        " stvd  %d8, 0x2050(%r4)\n "   // Write the mask to set the 16th bit with W_OR

        // Unset Bit15 (IPL Reset) Bit14 (MPIPL) Bit17 (HReset Done) Bit18 (MPIPL Done)
        // to clear off previous triggers, in order to keep only the current trigger
        " lis   %r8, 0x3\n "            // Set the mask to reset the bit 14/15
        " ori   %r8, %r8, 0x6000\n "    // Set the mask to reset the bit 17/18
        " li    %r9, 0x0\n "            // d8 is 0x0003_6000_0000_0000
        " stvd  %d8, 0x2058(%r4)\n "    // write to WO_CLR Register

        // Jump to __system_reset
        " b __system_reset\n "
    );
}

/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/core/sbes1handler.H $                               */
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
#ifndef _SBES1HANDLER_H_
#define _SBES1HANDLER_H_

extern "C"
{
    /**
     * @brief Function to make SBE to go into wait/idle state.
     *        1) Disable External interupts
     *        2) Mask all interupts except S1
     *        3) Check for s1 interrupt in EISR until triggered
     *        4) Jump to __system_reset if s1 triggered set in EISR
     *
     */
    void __wait_for_s1();

    /**
     * @brief Function to handle s1 interrupt.
     *        On s1 interrupt:
     *        1) Clear bit3(PPE wait state) in messaging reg
     *        2) Jump to system reset
     *
     */
    void __s1_interupt_handler();
}

#endif /* _SBES1HANDLER_H_ */

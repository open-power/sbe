/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: sbe/sbefw/sbescom.H $                                         */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016                             */
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

/**
  * @brief check if scom address is Indirect scom and perform scom
  *
  * @param[in] i_isRead True if read operation, false otherwise
  * @param[in] i_addr scom addess
  * @param[in] io_data data for scom operation. For read, it is output operand.
  * @param[in] o_pcbPibStatus pcbpib status
  *
  * @return    seconday error code.
  */
uint32_t checkIndirectAndDoScom( const bool i_isRead,
                                 const uint64_t i_addr,
                                 uint64_t & io_data,
                                 uint32_t & o_pcbPibStatus );

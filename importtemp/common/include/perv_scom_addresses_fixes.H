/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: chips/p9/common/include/perv_scom_addresses_fixes.H $         */
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
/// @file perv_scom_addresses_fixes.H
/// @brief The scom_addresses.H is generated form figtree, but the figree can be
///        wrong.  This file is included at the end of scom_addresses.H and allows
///        incorrect constants to be fixed manually.
///
// *HWP HWP Owner: Ben Gass <bgass@us.ibm.com>
// *HWP FW Owner: ? <?>
// *HWP Team: SAO
// *HWP Level: 1
// *HWP Consumed by: FSP:HB:HS:OCC:SBE:CME:SGPE:PGPE:FPPE:IPPE

#ifndef __PERV_SCOM_ADDRESSES_FIXES_H
#define __PERV_SCOM_ADDRESSES_FIXES_H

CONST_UINT32_T( PERV_FSI2PIB_STATUS_FSI                                 ,
                ULL(0x00001007), SH_UNT_PERV_FSI2PIB, SH_ACS_FSI       );

#endif

/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: hwpf/fapi2/include/fapi2_multicast_defs.H $                   */
/*                                                                        */
/* IBM CONFIDENTIAL                                                       */
/*                                                                        */
/* EKB Project                                                            */
/*                                                                        */
/* COPYRIGHT 2016                                                         */
/* [+] International Business Machines Corp.                              */
/*                                                                        */
/*                                                                        */
/* The source code for this program is not published or otherwise         */
/* divested of its trade secrets, irrespective of what has been           */
/* deposited with the U.S. Copyright Office.                              */
/*                                                                        */
/* IBM_PROLOG_END_TAG                                                     */
#ifndef __FAPI2_COMMON_MULTICAST_DEFS__
#define __FAPI2_COMMON_MULTICAST_DEFS__

namespace fapi2
{
enum MulticastType
{
    MULTICAST_WRITE,
    MULTICAST_READAND,
    MULTICAST_READOR,
    MULTICAST_READBITX,
    MULTICAST_READCOMPARE,
};

typedef uint32_t MulticastGroup;
}

#endif

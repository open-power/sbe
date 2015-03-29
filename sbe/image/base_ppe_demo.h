//-----------------------------------------------------------------------------
// *! (C) Copyright International Business Machines Corp. 2014
// *! All Rights Reserved -- Property of IBM
// *! *** IBM Confidential ***
//-----------------------------------------------------------------------------

/// \file base_ppe_demo.h
/// \brief routine to be placed into the fixed section. 
///
///  this routing is a demonstration for functions to be placed into the fixed
///  fixed section
#ifndef __BASE_PPE_DEMO_H__
#define __BASE_PPE_DEMO_H__

/// this is a demo function whose pointer is placed into the fixed section
/// @param str demo string
void base_ppe_demo_func(const char*);

#endif  // __BASE_PPE_DEMO_H__

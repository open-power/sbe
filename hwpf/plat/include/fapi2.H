/**
 *  @file fapi2.H
 *
 *  @brief Includes all the header files necessary for the FAPI2 interface.
 */


#ifndef FAPI2_H_
#define FAPI2_H_

// Define which platforms will not have FAPI Return Codes
#undef __noRC__
#if defined (__CME__) || defined (__SGPE__) || defined (__PGPE__)
#define __noRC__
#endif

// Determine if running on a PPE platform
#ifndef __PPE__
#if defined (__SBE__) || defined (__CME__) || defined (__SGPE__) || defined (__PGPE__)
#define __PPE__
#endif
#endif

#include <buffer.H>
#include <buffer_base.H>
#include <buffer_parameters.H>
#include <buffer_traits.H>
#include <error_scope.H>
#include <hw_access.H>
#include <return_code.H>
#include <target.H>
#include <target_types.H>
#include <utils.H>
//#include <variable_buffer.H>
#include <fapi2AttributeService.H>
#include <fapi2AttributeIds.H>      // Generated file


#endif // FAPI2_H_






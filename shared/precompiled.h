/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/
#pragma once

#pragma warning (disable:4800) // forcing value to bool 'true' or 'false' (mainly in asserts)

/*

Precompiled header file to make compilation faster.
It is useful for compilers which support this feature.

How does it works?
1. You must enable Use precompiled header for all your project files in project settings and specify this precompiled.h file as precompiled header.
2. Inside your project / module you must create precompiled.cpp. Only for this file you must enable Crate precompiled header with precompiled.h.
2. You must include this file as your FIRST include file in every .cpp file of project.

*/


// Here add all huge include files which rarely changes but are included in almost every cpp. 
// This headers will be precompiled and compiled version will be included instead of compiling these headers for every cpp

// Note: It is recommended to use separated precompiled.cpp for each project, because then you can have specific precompiled headers for each project.
//       It means you can set some preprocesor definitions in project settings and here use #ifdef. Additionaly you can make separate precompiled.h
//       for your project to use your own precompiled #includes

#if defined(WIN32)
  #include <windows.h>
 #undef GetObject
#endif

// internal headers
#include "symbols.h" // must be first
#include "rassert.h" // must be second
#include "types.h" // must be third
#include "interface.h" // must be second

// filesystem
#include <stdio.h>

// renderer
#include <SDL/SDL.h>
#include <Cg/cg.h>
#if defined(WIN32)
 #include <DX/d3d9.h> 
#endif

// other
#include <string>
#include <vector>
#include "sqlite/sqlite3.h"

// character converters
#include "unicode/utypes.h"   /* Basic ICU data types */
#include "unicode/ucnv.h"     /* C   Converter API    */
#include "unicode/ustring.h"  /* some more string fcns*/
#include "unicode/uchar.h"    /* char names           */
#include "unicode/uloc.h"
#include "unicode/unistr.h"
#include "unicode/translit.h" /* transliteration */
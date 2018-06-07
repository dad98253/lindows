// StdAfx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_X)
#define AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_X


#ifndef WINDOZE
#define _MSC_VER	0
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

// C RunTime Header Files
#include <stdlib.h>
#ifndef BSD
#include <malloc.h>
#endif
#include <memory.h>

#ifdef WINDOZE
// Windows Header Files:
#include <windows.h>
// Windows Html Help Header Files:
#include <htmlhelp.h>
#include <tchar.h>
#define UNUSED(x) x
#define CLEARSCR system ( "cls" )
#else	// WINDOZE
// Linux Header Files:
#include "lindows.h"
#define CLEARSCR system ( "reset -m unknown:dumb" )
#endif	// WINDOZE

#define Stringize( L ) #L
#define MakeString( M, L ) M(L)
#ifndef TURK
#define $Line MakeString( Stringize, __LINE__ )
#else	// TURK  ... the above will not work using the older ARM compiler
#define Line MakeString(Stringize,__LINE__)
#endif	// TURK
#define Reminder __FILE__ "(" $Line ") : Reminder: "
#ifdef DEBUG
#ifndef __MYFILE__
#ifdef WINDOZE
#define __MYFILE__  (strrchr ( __FILE__ , '\\' )+1)
#else	// WINDOZE
#define __MYFILE__  (strrchr ( __FILE__ , '/' )+1)
#endif	// WINDOZE
#endif	// __MYFILE__
#else	// DEBUG
#define __MYFILE__	""
#endif // DEBUG
#ifdef WINDOZE
#define JKNULL	NULL
#else
#ifndef JKNULL
#ifdef __cplusplus
#define JKNULL    0
#else
#define JKNULL    ((void *)0)
#endif
#endif
#endif


// Local Header Files

// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_X)


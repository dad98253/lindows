/*
 * CompareStringResource.cpp
 *
 *  Created on: Jan 17, 2017
 *      Author: dad
 */

#ifdef WINDOZE
#pragma message( "Compiling " __FILE__ " on " __DATE__ " at " __TIME__ )
#pragma message( "File last modified on " __TIMESTAMP__ )
#pragma message( "  ")
#pragma title( "My Secret Box version 2.0" )
#pragma subtitle( "Copyright (c) 2003 - 2015, Nehemiah Ministries, Inc." )
#pragma comment( compiler )
#pragma comment( user, "File: " __FILE__ ". Compiled on " __DATE__ " at " __TIME__ ".  Last modified on " __TIMESTAMP__ )
#endif

//#include "StdAfx.h"

#include "extwindata.h"

int CompareStringResource(const void *m1, const void *m2)
{
	WindowsStringDataTYPE *mi1;
    mi1 = (WindowsStringDataTYPE *)m1;
    WindowsStringDataTYPE *mi2;
    mi2 = (WindowsStringDataTYPE *)m2;
//    return (mi2->id - mi1->id);
    return ( mi1->id - mi2->id);
}

/*
static int CompareStringResourcex(const void *m1, const void *m2)
{
    struct WindowsStringData *mi1;
    mi1 = &StringResourceData[*(int *)m1];
    struct WindowsStringData *mi2;
    mi2 = &StringResourceData[*(int *)m2];
    return (mi2->id - mi1->id);
//    return ( mi1->id - mi2->id);
}

void* bsearchx (const void* key, const void* base,
               size_t num, size_t size,
               int (*compar)(const void*,const void*))
{
	int i;
	for (i=0;i< (int)num ; i++) {
		if (compar(key,&i) == 0) {
			unsigned int temxx;
			temxx = size*i;
			return((void*)((char *)base+temxx));
		}
		int jck = compar(key,&i);

	}
	return (NULL);
}
*/


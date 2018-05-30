/*
 * windows.cpp
 *
 *  Created on: Dec 13, 2015
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

#include "lindows_StdAfx.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>
#ifndef WINDOZE
#include <unistd.h>
#endif
#include <ctype.h>
#include <limits.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#ifndef BSD
#include <sys/sendfile.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#endif
#include <fcntl.h>
#include <syslog.h>
#include <pwd.h>

//#include "passwin.h"
//#include "options.h"
//#include "globals.h"
//#include "debugflags.h"
//#include "resource.h"
//#include "extwindata.h"
//#include "Resources.h"
#include "debug.h"
#include "msgbox.h"
#include "extwindata.h"
#include "stringresources.h"
#ifdef NOECBM2X
#include "WMessages.h"
#include "ecbm2.h"
#endif

#define TRACESTATUS		1
#define TRACEGETFULLPATHNAME	TRACESTATUS
#define DEBUGSETTINGSINPUT		TRACESTATUS


#define MIN(x, y) (((x) > (y)) ? (y) : (x))
#define SZNULL(x) !x ? "NULL" : (char *)x
#define FLAGSET(x) x ? "Set" : "Not Set"    /* return a const char string of either "Set" or "Not Set" based on the bool argument x    */

extern bool FileExists(char *szFileName);
extern int CompareStringResource(const void *m1, const void *m2);

int WINAPI LoadString(
  _In_opt_ HINSTANCE hInstance,
  _In_     UINT      uID,
  _Out_    LPTSTR    lpBuffer,
  _In_     int       nBufferMax
)
{
	WindowsStringDataTYPE *res;
	if (nBufferMax == 0){
		printf("LoadString called with nBufferMax == 0, this feature is not available in this version of LoadString\n");
		return (0);
	}
      res = (WindowsStringDataTYPE *)bsearch (&uID, StringResourceData, numStringItems, sizeof(WindowsStringDataTYPE), CompareStringResource );
//    res = (WindowsStringDataTYPE *)bsearchx(&uID, StringResourceData, numStringItems, sizeof(WindowsStringData), CompareStringResourcex);
    if (res == NULL) {
        printf("LoadString called with uID == %i, this element could not be found in the Windows string resource data\n", uID);
        return(0);
    } else {
    	int numChar2Copy = MIN(nBufferMax,(int)strlen(res->string));
    	if (numChar2Copy == 0) return(0);
    	strncpy(lpBuffer,res->string,numChar2Copy);
        int termloc = MIN(numChar2Copy,nBufferMax-1);
        lpBuffer[termloc] = '\000';
#ifdef DEBUGLOADSTRING
    	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"LoadString returned #%i as \"%s\"\n", res->id, lpBuffer);
#endif
    }
	return (strlen(lpBuffer));
}

HMODULE WINAPI GetModuleHandle(
   LPCTSTR lpModuleName
)
{
#ifdef DEBUGGETMODULEHANDLE
	if ( lpModuleName == NULL ) dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"GetModuleHandle called with NULL argument, returning 1\n");
	if ( lpModuleName != NULL ) dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"GetModuleHandle called with (%s) argument, returning NULL\n",lpModuleName);
#endif
	if ( lpModuleName == NULL ) return (1);
	return (JKNULL);
}
DWORD WINAPI GetFullPathName(
    LPCTSTR lpFileName,
    DWORD   nBufferLength,
   LPTSTR  lpBuffer,
   LPTSTR  *lpFilePart
)
{

	char *ptr;

	ptr = realpath(lpFileName, NULL);
	if ( ptr == NULL ) {
#ifdef DEBUG
		dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"GetFullPathName \33[1;32mFAILED\33[0m for \"%s\", errno = %u\n",lpFileName,errno);
#endif
		return(0);
	}
	if ( (strlen(ptr)+1) > nBufferLength ) {
		*lpBuffer = '\000';
//		*lpFilePart = NULL;
		int iLenNeeded = strlen(ptr);
		free(ptr);
		return (iLenNeeded);
	}
	*lpBuffer = '\000';
	strcpy(lpBuffer,(const char *)ptr);
	char *szTemp;
	szTemp = strrchr(lpBuffer, '/') + 1;
//	if ( **lpFilePart == JKNULL ) {
//#ifdef DEBUG
//	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"**lpFilePart == NULL in GetFullPathName\n");
//#endif
//	}
//	*lpFilePart = szTemp;   //  for some reasone, this fails... possibly something wrong with the argument in the calling routine???

#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"GetFullPathName for \"%s\' is \"%s\"\n",szTemp,lpBuffer);
#endif
	free(ptr);
	return (strlen(lpBuffer));
}

// global variables used in the following Resource related routines :
char ** tempIntResource = NULL;
char ** ResourceName = NULL;
char ** ResourceValue = NULL;
char ** ResourceType = NULL;
int NumTempIntResources = 0;
int NumResources = 0;

LPTSTR MAKEINTRESOURCE(
   WORD wInteger
)
{
	NumTempIntResources++;

	tempIntResource = (char**)realloc(tempIntResource,sizeof(char*)*(NumTempIntResources+2));
#ifdef DEBUGMAKEINTRESOURCE
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"MAKEINTRESOURCE reallocated %i bytes for tempIntResource starting at (0x%llx)\n",(int)_msize(tempIntResource),(long long unsigned int)tempIntResource);
#endif
	if (tempIntResource == NULL) {
#ifdef DEBUGMAKEINTRESOURCE
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"MAKEINTRESOURCE = NULL, unable to allocate space for scratch resource, requested value = %i\n",wInteger);
#endif
		return (NULL);
	}
	tempIntResource[NumTempIntResources] = NULL;
	tempIntResource[NumTempIntResources] = (char*)malloc(16);
#ifdef DEBUGMAKEINTRESOURCE
	if (tempIntResource[NumTempIntResources] == NULL ) dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"MAKEINTRESOURCE = NULL, requested value = %i\n",wInteger);
	if (tempIntResource[NumTempIntResources] != NULL ) dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"MAKEINTRESOURCE created scratch resource %i for requested value = %i of length %i at (0x%llx)\n",NumTempIntResources,wInteger,(int)_msize(tempIntResource[NumTempIntResources]),(long long unsigned int)tempIntResource[NumTempIntResources]);
#endif
	if (tempIntResource[NumTempIntResources] != NULL ) sprintf(tempIntResource[NumTempIntResources],"%i",wInteger);
	return (tempIntResource[NumTempIntResources]);
}

HRSRC WINAPI FindResource(
   HMODULE hModule,
       LPCTSTR lpName,
       LPCTSTR lpType
)
{
	if (strncmp(lpName,"112",3) == 0 && strncmp(lpType,"10",2) == 0) {
#ifdef DEBUGFINDRESOURCE
		dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"FindResource found WM_MESS resource\n");
#endif
		return (112);
	}
	if (strncmp(lpName,"110",3) == 0 && strncmp(lpType,"10",2) == 0) {
#ifdef DEBUG
		dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"FindResource found ECB_F2 resource\n");
#endif
		return (110);
	}
	if (strncmp(lpName,"18727",3) == 0 && strncmp(lpType,"TEXTFILE",8) == 0) {
#ifdef DEBUG
		dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"FindResource found IDR_TEXTFILE2 resource\n");
#endif
		return (18727);
	}
	if (hModule != 1 || NumResources == 0) {
#ifdef DEBUG
		dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"FindResource = \33[1;32mNULL\33[0m\n");
#endif
		return (JKNULL);
	}
	int i;
	for (i=0;i<NumResources;i++){
		if (ResourceName[i] == lpName && ResourceType[i] == lpType)
		{
			return (JKNULL);
		}
	}
	return (JKNULL);
}

HGLOBAL WINAPI LoadResource(
   HMODULE hModule,
       HRSRC   hResInfo
)
{
	if (hModule != JKNULL) {
#ifdef DEBUG
		dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"LoadResource called with non-NULL module handle... this version can only process null as first argument\n");
#endif
		return (0);
	}
	if (hResInfo == 110) return (110);
	if (hResInfo == 112) return (112);
	if (hResInfo == 18727) return (18727);
	return(0);
}

LPVOID WINAPI LockResource(
   HGLOBAL hResData
)
{
#ifdef NOECBM2X
	if (hResData == 110) return (&ecbm2);
	if (hResData == 112) return (&WMessages);
#endif
	if (hResData == 18727) {
#ifdef DEBUG
		dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"LockResource failed for hResData = %i.. the TEXTFILE is not implemented, yet\n" ,hResData);
#endif
		return (NULL);
	}
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"LockResource failed for hResData = %i\n" ,hResData);
#endif
	return (NULL);
}

DWORD WINAPI SizeofResource(
  _In_opt_ HMODULE hModule,
  _In_     HRSRC   hResInfo
)
{
	if (hModule != JKNULL && hModule != 1 ) {
#ifdef DEBUG
		dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"SizeofResource called with module handle of %u, this version can only process null or 1 as first argument\n",hModule);
#endif
		return (0);
	}
#ifdef NOECBM2X
	if (hResInfo == 110) return ((DWORD)sizeof(ecbm2));
	if (hResInfo == 112) return ((DWORD)sizeof(WMessages));
#endif
	if (hResInfo == 18727) {
#ifdef DEBUG
		dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"SizeofResource called for TEXTFILE resource... this version cannot process TEXTFILE, yet...\n");
#endif
		return (0);
	}

#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"SizeofResource = 0\n");
#endif
	return (0);
}

int WINAPI MessageBox(
   HWND    hWnd,
   LPCTSTR lpText,
   LPCTSTR lpCaption,
       UINT    uType
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"MessageBox = %s\n",lpText);
#endif
	return (0);
}


extern char *szProgFilename;

DWORD WINAPI GetModuleFileName(
   HMODULE hModule,
      LPTSTR  lpFilename,
       DWORD   nSize
)
{
	strcpy(lpFilename,szProgFilename);
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"GetModuleFileName = %s\n",lpFilename);
#endif
	return (strlen(lpFilename));
}

DWORD WINAPI GetLastError(void)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"GetLastError = \33[1;32mNULL\33[0m\n");
#endif
	return (0);
}

BOOL WINAPI GetFileVersionInfo(
         LPCTSTR lptstrFilename,
		 HANDLE*   dwHandle,
         DWORD   dwLen,
        LPVOID  lpData
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"GetFileVersionInfo = \33[1;32mNULL\33[0m\n");
#endif
	return (FALSE);
}

DWORD WINAPI GetFileVersionInfoSize(
        LPCTSTR lptstrFilename,
   LPDWORD lpdwHandle
)
{
//  no file version available on linux... return zero
	return (0);
}

BOOL WINAPI VerQueryValue(
    LPCVOID pBlock,
    LPCTSTR lpSubBlock,
   LPVOID  *lplpBuffer,
   PUINT   puLen
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"VerQueryValue = \33[1;32mNULL\33[0m\n");
#endif
	return (FALSE);
}

void WINAPI GetSystemInfo(
   LPSYSTEM_INFO lpSystemInfo
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"GetSystemInfo = \33[1;32mNULL\33[0m\n");
#endif
	return;
}

DWORD WINAPI GetVersion(void)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"GetVersion = \33[1;32mNULL\33[0m\n");
#endif
	return (0);
}

int WINAPI GetSystemMetrics(
   int nIndex
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"GetSystemMetrics = \33[1;32mNULL\33[0m\n");
#endif
	return (0);
}

BOOL WINAPI WriteFile(
          HANDLE       hFile,
          LPCVOID      lpBuffer,
          DWORD        nNumberOfBytesToWrite,
     LPDWORD      lpNumberOfBytesWritten,
   LPOVERLAPPED lpOverlapped
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"WriteFile = \33[1;32mNULL\33[0m\n");
#endif
	return (FALSE);
}

void WINAPI OutputDebugString(
   LPCTSTR lpOutputString
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"OutputDebugString = \33[1;32mNULL\33[0m\n");
#endif
	return;
}

HANDLE WINAPI GetStdHandle(
   DWORD nStdHandle
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"GetStdHandle = \33[1;32mNULL\33[0m\n");
#endif
	return (0);
}

DWORD WINAPI GetProfileString(
    LPCTSTR lpAppName,
    LPCTSTR lpKeyName,
    LPCTSTR lpDefault,
   LPTSTR  lpReturnedString,
    DWORD   nSize
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"GetProfileString = \33[1;32mNULL\33[0m\n");
#endif
	return (0);
}

// the following global variables ar used by the windows file routines
#define NUMCFHANDLES    20
int numCFHandles = 0;
char *CFileHandles[NUMCFHANDLES] = { NULL };

HANDLE WINAPI CreateFile(
       LPCTSTR               lpFileName,
       DWORD                 dwDesiredAccess,
       DWORD                 dwShareMode,
   LPSECURITY_ATTRIBUTES lpSecurityAttributes,
       DWORD                 dwCreationDisposition,
       DWORD                 dwFlagsAndAttributes,
	   JKPHANDLE                hTemplateFile
)
{
	int i;
	int newindex = 0;
	if ( numCFHandles == NUMCFHANDLES ) {
		printf ("CreateFile failed due to too many files open...\n");
		return (INVALID_HANDLE_VALUE);
	}
	numCFHandles++;
	for (i=0;i<NUMCFHANDLES;i++) {
		if(CFileHandles[i] == NULL) {
			newindex = i;
			break;
		}
	}
	CFileHandles[newindex] = (char *)malloc(strlen(lpFileName)+1);
	strcpy(CFileHandles[newindex],lpFileName);
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"CreateFile handle for %s = %i\n",lpFileName,newindex+1);
#endif

	if ( dwDesiredAccess == GENERIC_READ && dwCreationDisposition != OPEN_EXISTING ) return (INVALID_HANDLE_VALUE);
	if ( dwDesiredAccess == GENERIC_WRITE && dwCreationDisposition != CREATE_NEW && dwCreationDisposition != CREATE_ALWAYS ) return (INVALID_HANDLE_VALUE);
	if ( dwDesiredAccess == (GENERIC_READ|GENERIC_WRITE) && dwCreationDisposition != OPEN_EXISTING ) {
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"CreateFile COM port option is not available at this time... need more code to finish this...\n");
#endif
		return (INVALID_HANDLE_VALUE);
	}
	if ( dwDesiredAccess == GENERIC_READ ) {
		int fpin = open(lpFileName,O_RDONLY,S_IREAD);
		if ( fpin == -1 ) {
			printf("CreateFile unable to open %s for GENERIC_READ, errno = %i\n",lpFileName,errno);
			return (INVALID_HANDLE_VALUE);
		}
		close(fpin);
	}
	if ( dwDesiredAccess == GENERIC_WRITE ) {
		int fpout = open(lpFileName,O_RDWR | O_CREAT,S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
		if ( fpout == -1 ) {
			printf("CreateFile unable to open %s for GENERIC_WRITE, errno = %i\n",lpFileName,errno);
			return (INVALID_HANDLE_VALUE);
		}
		close(fpout);
	}

	return (newindex+1);
}

BOOL WINAPI CloseHandle(
  _In_ HANDLE hObject
)
{
	if(CFileHandles[hObject-1] != NULL) {
		free(CFileHandles[hObject-1]);
		CFileHandles[hObject-1] = NULL;
#ifdef DEBUG
		dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"Closed Handle %li\n",hObject);
#endif
		return (TRUE);
	}
	return (FALSE);
}


long long int Int32x32To64 ( LONG Multiplier, LONG Multiplicand )
{
	return ((long long int)Multiplier*(long long int)Multiplicand);
}

void TimetToFileTime( time_t t, LPFILETIME pft )
{
	/*
	FILETIME Contains a 64-bit value representing the number of 100-nanosecond intervals since January 1, 1601 (UTC).

	typedef struct _FILETIME {
	  DWORD dwLowDateTime;
	  DWORD dwHighDateTime;
	} FILETIME, *PFILETIME;

	time_t:

	typedef time_t;

	Arithmetic (until C11) Real (since C11) type capable of representing times.
	Although not defined by the C standard, this is almost always an integral value holding the number of seconds
	(not counting leap seconds) since 00:00, Jan 1 1970 UTC, corresponding to POSIX time.
	 */
#ifdef DEBUG
	struct tm UNUSED(*ptm);
	char s[41];
	ptm = gmtime ( &t );
	strftime(s, sizeof(s), "%D %T",ptm);
	dfprintf(__LINE__,__FILE__,DEBUGSETTINGSINPUT,"in TimetToFileTime, time_t = %s\n",s);
#endif
    LONGLONG ll = Int32x32To64(t, 10000000) + 116444736000000000LL;
    pft->dwLowDateTime = (unsigned int) ll;
    pft->dwHighDateTime = ll >>32;
#ifdef DEBUG
    LONGLONG ll2 = ( ( ((long long int)pft->dwHighDateTime) << 32 ) + ((long long int)pft->dwLowDateTime) - 116444736000000000LL ) / 10000000LL;
    time_t t2 = (time_t)ll2;
	dfprintf(__LINE__,__FILE__,DEBUGSETTINGSINPUT,"in TimetToFileTime, t = %lu, t2 = %lu, FileTime = %lu,%lu\n",t,t2,pft->dwHighDateTime,pft->dwLowDateTime);
#endif
    return;
}

void FileTimeToTimet( LPFILETIME pft , time_t *t )
{
    LONGLONG ll = ( ( ((long long int)pft->dwHighDateTime) << 32 ) + ((long long int)pft->dwLowDateTime) - 116444736000000000LL ) / 10000000LL;
    *t = (time_t)ll;
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,DEBUGSETTINGSINPUT,"in FileTimeToTimet, t = %lu, FileTime = %lu,%lu\n",t,pft->dwHighDateTime,pft->dwLowDateTime);
#endif
	return;
}

BOOL WINAPI GetFileTime(
  _In_      HANDLE     hFile,
  _Out_opt_ LPFILETIME lpCreationTime,
  _Out_opt_ LPFILETIME lpLastAccessTime,
  _Out_opt_ LPFILETIME lpLastWriteTime
)
{
   struct stat sb;

#ifdef HAVE_ST_BIRTHTIME
#define birthtime(x) x.st_birthtime
#else
#define birthtime(x) x.st_ctime
#endif
   if (hFile < 1 || hFile > NUMCFHANDLES){
	   printf("bad handle passed to GetFileTime, hFile = %li\n",hFile);
       return(TRUE);
   }
   if (CFileHandles[hFile-1] == NULL){
	   printf("invalid handle passed to GetFileTime, hFile = %li is not an open file\n",hFile);
       return(TRUE);
   }
   if (stat(CFileHandles[hFile-1], &sb) == -1) {
	    printf("stat call in GetFileTime failed for %s\n",CFileHandles[hFile-1]);
        perror("stat");
        return(TRUE);
    }

#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,DEBUGSETTINGSINPUT,"\33[1;35mCreation time\33[0m:\n");
#endif
   TimetToFileTime (birthtime(sb),lpCreationTime);
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,DEBUGSETTINGSINPUT,"\33[1;35mLast Access time\33[0m:\n");
#endif
   TimetToFileTime (sb.st_atime,lpLastAccessTime);
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,DEBUGSETTINGSINPUT,"\33[1;35mLast Modification time\33[0m:\n");
#endif
   TimetToFileTime (sb.st_mtime,lpLastWriteTime);

#ifdef DEBUG
		dfprintf(__LINE__,__FILE__,DEBUGSETTINGSINPUT,"\33[1;35mGetFileTime\33[0m stat of \'%s\':\nFile type:                ",CFileHandles[hFile-1]);

		switch (sb.st_mode & S_IFMT) {
			case S_IFBLK:  dfprintf(__LINE__,__FILE__,DEBUGSETTINGSINPUT,"block device\n");            break;
			case S_IFCHR:  dfprintf(__LINE__,__FILE__,DEBUGSETTINGSINPUT,"character device\n");        break;
			case S_IFDIR:  dfprintf(__LINE__,__FILE__,DEBUGSETTINGSINPUT,"directory\n");               break;
			case S_IFIFO:  dfprintf(__LINE__,__FILE__,DEBUGSETTINGSINPUT,"FIFO/pipe\n");               break;
			case S_IFLNK:  dfprintf(__LINE__,__FILE__,DEBUGSETTINGSINPUT,"symlink\n");                 break;
			case S_IFREG:  dfprintf(__LINE__,__FILE__,DEBUGSETTINGSINPUT,"regular file\n");            break;
			case S_IFSOCK: dfprintf(__LINE__,__FILE__,DEBUGSETTINGSINPUT,"socket\n");                  break;
			default:       dfprintf(__LINE__,__FILE__,DEBUGSETTINGSINPUT,"unknown\n");                 break;
		}
		dfprintf(__LINE__,__FILE__,DEBUGSETTINGSINPUT,"I-node number:            %ld\n", (long) sb.st_ino);
		dfprintf(__LINE__,__FILE__,DEBUGSETTINGSINPUT,"Mode:                     %lo (octal)\n",(unsigned long) sb.st_mode);
		dfprintf(__LINE__,__FILE__,DEBUGSETTINGSINPUT,"Link count:               %ld\n", (long) sb.st_nlink);
		dfprintf(__LINE__,__FILE__,DEBUGSETTINGSINPUT,"Ownership:                UID=%ld   GID=%ld\n",(long) sb.st_uid, (long) sb.st_gid);
		dfprintf(__LINE__,__FILE__,DEBUGSETTINGSINPUT,"Preferred I/O block size: %ld bytes\n",(long) sb.st_blksize);
		dfprintf(__LINE__,__FILE__,DEBUGSETTINGSINPUT,"File size:                %lld bytes\n",(long long) sb.st_size);
		dfprintf(__LINE__,__FILE__,DEBUGSETTINGSINPUT,"Blocks allocated:         %lld\n",(long long) sb.st_blocks);
		dfprintf(__LINE__,__FILE__,DEBUGSETTINGSINPUT,"Last status change:       %s", ctime(&sb.st_ctime));
		dfprintf(__LINE__,__FILE__,DEBUGSETTINGSINPUT,"Last file access:         %s", ctime(&sb.st_atime));
		dfprintf(__LINE__,__FILE__,DEBUGSETTINGSINPUT,"Last file modification:   %s", ctime(&sb.st_mtime));
#endif
	return (TRUE);
}


BOOL WINAPI SetupComm(
   HANDLE hFile,
   DWORD  dwInQueue,
   DWORD  dwOutQueue
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"SetupComm = \33[1;32mNULL\33[0m\n");
#endif
	return (FALSE);
}

BOOL WINAPI GetCommState(
      HANDLE hFile,
   LPDCB  lpDCB
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"GetCommState = \33[1;32mNULL\33[0m\n");
#endif
	return (FALSE);
}

BOOL WINAPI SetCommState(
   HANDLE hFile,
   LPDCB  lpDCB
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"SetCommState = \33[1;32mNULL\33[0m\n");
#endif
	return (FALSE);
}

BOOL WINAPI FlushFileBuffers(
   HANDLE hFile
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"FlushFileBuffers = \33[1;32mNULL\33[0m\n");
#endif
	return (FALSE);
}

BOOL WINAPI FreeConsole(void)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"FreeConsole = \33[1;32mNULL\33[0m\n");
#endif
	return (FALSE);
}

BOOL WINAPI GetComputerName(
     LPTSTR  lpBuffer,
   LPDWORD lpnSize
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"GetComputerName = \33[1;32mNULL\33[0m\n");
#endif
	return (FALSE);
}


DWORD WINAPI FormatMessage(
  _In_     DWORD   dwFlags,
  _In_opt_ LPCVOID lpSource,
  _In_     DWORD   dwMessageId,
  _In_     DWORD   dwLanguageId,
  _Out_    LPTSTR  lpBuffer,
  _In_     DWORD   nSize,
  _In_opt_ va_list *Arguments
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"FormatMessage = \33[1;32mNULL\33[0m\n");
#endif
	return (0);
}
#ifndef BSD
#include <malloc.h>
#else
#include <stdlib.h>
#include <malloc_np.h>
#endif
size_t _msize(
void *memblock
)
{
#ifdef DEBUGMSIZE
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"msize = %lu\n", malloc_usable_size (memblock));
#endif
	return (malloc_usable_size (memblock));
}

DWORD WINAPI CommDlgExtendedError(void)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"CommDlgExtendedError = \33[1;32mNULL\33[0m\n");
#endif
	return (0);
}

BOOL WINAPI MessageBeep(
  _In_ UINT uType
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"MessageBeep = \33[1;32mNULL\33[0m\n");
#endif
	return (FALSE);
}

HWND WINAPI GetFocus(void)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"GetFocus = \33[1;32mNULL\33[0m\n");
#endif
	return (JKNULL);
}

void WINAPI FatalExit(
  _In_ int ExitCode
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"FatalExit = \33[1;32mNULL\33[0m\n");
#endif
	return;
}

BOOL WINAPI DestroyWindow(
  _In_ HWND hWnd
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"DestroyWindow = \33[1;32mNULL\33[0m\n");
#endif
	return (FALSE);
}
int PASCAL FAR WSAStartup(WORD wVersionRequired, LPWSADATA lpWSAData)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"WSAStartup = \33[1;32mNULL\33[0m\n");
#endif
	return (0);
}

int PASCAL FAR WSAGetLastError(void)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"WSAGetLastError = \33[1;32mNULL\33[0m\n");
#endif
	return (0);
}

int PASCAL FAR WSACleanup(void)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"WSACleanup = \33[1;32mNULL\33[0m\n");
#endif
	return (0);
}

/*struct hostent* FAR gethostbyname(
  _In_ const char *name
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"gethostbyname = \33[1;32mNULL\33[0m\n");
#endif
	return (NULL);
}

unsigned long inet_addr(
  _In_ const char *cp
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"inet_addr = \33[1;32mNULL\33[0m\n");
#endif
	return (0);
}
 */
int closesocket(
  _In_ SOCKET s
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"closesocket = \33[1;32mNULL\33[0m\n");
#endif
	return (0);
}

HANDLE WINAPI CreateConsoleScreenBuffer(
  _In_             DWORD               dwDesiredAccess,
  _In_             DWORD               dwShareMode,
  _In_opt_   const SECURITY_ATTRIBUTES *lpSecurityAttributes,
  _In_             DWORD               dwFlags,
  _Reserved_       LPVOID              lpScreenBufferData
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"CreateConsoleScreenBuffer = \33[1;32mNULL\33[0m\n");
#endif
	return (0);
}

BOOL WINAPI ScrollConsoleScreenBuffer(
  _In_           HANDLE     hConsoleOutput,
  _In_     const SMALL_RECT *lpScrollRectangle,
  _In_opt_ const SMALL_RECT *lpClipRectangle,
  _In_           COORD      dwDestinationOrigin,
  _In_     const CHAR_INFO  *lpFill
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"ScrollConsoleScreenBuffer = \33[1;32mNULL\33[0m\n");
#endif
	return (FALSE);
}

BOOL WINAPI AllocConsole(void)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"AllocConsole = \33[1;32mNULL\33[0m\n");
#endif
	return (FALSE);
}

BOOL WINAPI GetConsoleScreenBufferInfo(
  _In_  HANDLE                      hConsoleOutput,
  _Out_ PCONSOLE_SCREEN_BUFFER_INFO lpConsoleScreenBufferInfo
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"GetConsoleScreenBufferInfo = \33[1;32mNULL\33[0m\n");
#endif
	return (FALSE);
}

BOOL WINAPI SetConsoleTextAttribute(
  _In_ HANDLE hConsoleOutput,
  _In_ WORD   wAttributes
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"SetConsoleTextAttribute = \33[1;32mNULL\33[0m\n");
#endif
	return (FALSE);
}

BOOL WINAPI SetConsoleScreenBufferSize(
  _In_ HANDLE hConsoleOutput,
  _In_ COORD  dwSize
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"SetConsoleScreenBufferSize = \33[1;32mNULL\33[0m\n");
#endif
	return (FALSE);
}

BOOL WINAPI GetConsoleMode(
  _In_  HANDLE  hConsoleHandle,
  _Out_ LPDWORD lpMode
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"GetConsoleMode = \33[1;32mNULL\33[0m\n");
#endif
	return (FALSE);
}

BOOL WINAPI SetConsoleMode(
  _In_ HANDLE hConsoleHandle,
  _In_ DWORD  dwMode
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"SetConsoleMode = \33[1;32mNULL\33[0m\n");
#endif
	return (FALSE);
}

BOOL WINAPI SetConsoleCursorPosition(
  _In_ HANDLE hConsoleOutput,
  _In_ COORD  dwCursorPosition
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"SetConsoleCursorPosition = \33[1;32mNULL\33[0m\n");
#endif
	return (FALSE);
}

HLOCAL WINAPI LocalFree(
  _In_ HLOCAL hMem
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"LocalFree = \33[1;32mNULL\33[0m\n");
#endif
	return (0);
}


LPWSTR* CommandLineToArgvW(
  _In_  LPCWSTR lpCmdLine,
  _Out_ int     *pNumArgs
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"CommandLineToArgvW = \33[1;32mNULL\33[0m\n");
#endif
	return (NULL);
}

LPWSTR GetCommandLineW(void)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"GetCommandLineW = \33[1;32mNULL\33[0m\n");
#endif
	return (NULL);
}

BOOL WINAPI DeleteFile(
  _In_ LPCTSTR lpFileName
)
{
	int iret = unlink(lpFileName);
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"in DeleteFile unlink(%s) returns %i\n",lpFileName,iret);
#endif
	if ( iret == 0 ) {
		return(TRUE);
	} else {
		return (FALSE);
	}
}

#define INVALID_FILE_ATTRIBUTES ((DWORD)-1)
// ... (from GetPASSWDfileName)

bool FileExistsJK2(char *szFileName)
{
  DWORD ftyp = GetFileAttributes(szFileName);
  if (ftyp == INVALID_FILE_ATTRIBUTES)
    return false;  //something is wrong with your path!

  if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
    return false;   // this is a directory!

  return true;    // this is not a directory!

//  DWORD dwAttrib = GetFileAttributes(szFileName);
//  return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
//         !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}


BOOL WINAPI CopyFile(
  _In_ LPCTSTR lpExistingFileName,
  _In_ LPCTSTR lpNewFileName,
  _In_ BOOL    bFailIfExists
)
{
	if ( bFailIfExists ) {
		if ( FileExistsJK2( (char*)lpNewFileName ) ) return (FALSE);
	}
	int fpin = open(lpExistingFileName,O_RDONLY,S_IREAD);
	if ( fpin == -1 ) {
		printf("CopyFile unable to open %s for read, errno = %i\n",lpExistingFileName,errno);
		return (FALSE);
	}
	int fpout = open(lpNewFileName,O_WRONLY | O_CREAT,S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if ( fpout == -1 ) {
		printf("CopyFile unable to open %s for write, errno = %i\n",lpNewFileName,errno);
		return (FALSE);
	}
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"copying %s to %s :",lpExistingFileName,lpNewFileName);
#endif
#ifndef BSD
	while ( sendfile(fpout, fpin, 0, 32768) == 32768 ) {
#else	// BSD
// oops... wait a minute! in BSD, the output handle for sendfile is still
// required to be a socket (linux extened this to include file)
// so, for freebsd based systems, we drop back to read/write to play it safe...
    char *buf = (char *)malloc(32768);
	assert(buf != NULL);
	if ( buf != NULL ) {
		printf("malloc unable to open buffer in CopyFile when copying %s\n",lpExistingFileName);
		return (FALSE);
	}
	ssize_t w = 0, r = 0, t, n, m;
//	t = filesize(in);
    int pos;
    FILE * shin = NULL;
    shin = fdopen(fpin,	"r");
    if ( shin == NULL ) {
    	printf("fdopen failed in CopyFile when copying %s, errno = %i\n",lpExistingFileName,errno);
    	return (FALSE);
    }
    pos = ftell (shin);
    fseek (shin, 0, SEEK_END);
    t   = ftell (shin);
    fseek (shin, pos, SEEK_SET);

	while(r < t && (n = read(fpin, buf, 32768))) {
		if(n == -1) {
			assert(errno == EINTR);
			if ( errno == EINTR ) {
				printf("read failed in in CopyFile when copying %s, errno = %i\n",lpExistingFileName,errno);
				return (FALSE);
			}
			continue;
		}
		r = n;
		w = 0;
		while(w < r && (m = write(fpout, buf + w, (r - w)))) {
            if(m == -1) {
            	assert(errno == EINTR);
            	if (errno == EINTR) {
            		printf("write failed in in CopyFile when copying %s, errno = %i\n",lpExistingFileName,errno);
            		return (FALSE);
            	}
            	continue;
            }
            w += m;
        }
#endif	// BSD
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"*");
#endif
	}
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"\n");
#endif
	close(fpin);
	close(fpout);
#ifdef BSD
    free(buf);
#endif	// BSD
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"CopyFile done\n");
#endif
	return (TRUE);
}

DWORD WINAPI GetFileAttributes(
  _In_ LPCTSTR lpFileName
)
{
	 int rval,r,w,e,d,returnVal;
//#ifdef DEBUGGETFILEATTRIBUTES
	 struct stat sb;
//#endif
	 returnVal = 0;
	 d=e=0;

	 /* Check file existence. */
	 rval = access(lpFileName, F_OK);
	 if (rval == 0) {
#ifdef DEBUGGETFILEATTRIBUTES
	  dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"%s exists\n", lpFileName);
#endif
	  returnVal = 1;}
	 else {
	  if (errno == ENOENT) {
#ifdef DEBUG
		  dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"%s does not exist\n", lpFileName);
#endif
		  returnVal = -1;}
	  if (errno == EACCES) {
#ifdef DEBUG
		  dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"%s is not accessible\n", lpFileName);
#endif
		  returnVal = -1;}
	 }

	 /* Check read access. */
	 rval = access(lpFileName, R_OK);
	 if (rval == 0) {
#ifdef DEBUGGETFILEATTRIBUTES
		 dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"%s is readable\n", lpFileName);
#endif
		 r=1;
	 } else {
#ifdef DEBUGGETFILEATTRIBUTES
		 dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"%s is not readable (access denied)\n", lpFileName);
#endif
		 r=0;
	 }
	 /* Check write access. */
	 w=0;
	 rval = access(lpFileName, W_OK);
	 if (rval == 0) {
#ifdef DEBUGGETFILEATTRIBUTES
		 dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"%s is writable\n", lpFileName);
#endif
		 w=1;
	 }
#ifdef DEBUG
	 else if (errno == EACCES) {
		 dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"%s is not writable (access denied)\n", lpFileName);
	 }
	 else if (errno == EROFS) {
		 dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"%s is not writable (read-only filesystem)\n", lpFileName);
	 }
#endif
//#ifdef DEBUGGETFILEATTRIBUTES
// check for directory:
	 if (stat(lpFileName, &sb) == -1) {
#ifdef DEBUG
		 dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"unable to stat file %s in GetFileAttributes\n",lpFileName);
#endif
		 d = 0;
	 }
	 if ( (sb.st_mode & S_IFMT) == S_IFDIR ) d = 1;
#ifdef DEBUGGETFILEATTRIBUTES
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"File type:                ");

	   switch (sb.st_mode & S_IFMT) {
	    case S_IFBLK:  dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"block device\n");            break;
	    case S_IFCHR:  dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"character device\n");        break;
	    case S_IFDIR:  dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"directory\n");				  break;
	    case S_IFIFO:  dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"FIFO/pipe\n");               break;
	    case S_IFLNK:  dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"symlink\n");                 break;
	    case S_IFREG:  dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"regular file\n");            break;
	    case S_IFSOCK: dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"socket\n");                  break;
	    default:       dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"unknown?\n");                break;
	    }
#endif
	 if (returnVal != -1) returnVal=1000*d+100*r+10*w+e;
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"GetFileAttributes set to %i for %s\n",returnVal,lpFileName);
#endif
	return (returnVal);
}

WINSHELLAPI HRESULT WINAPI SHGetSpecialFolderLocation(HWND hwndOwner, int nFolder, LPITEMIDLIST * ppidl)
{
	char *homedir;
	unsigned int iSizeHomedir;
	struct passwd *pw;
	uid_t uid;

	if ( nFolder != CSIDL_APPDATA ) {
#ifdef DEBUG
		dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"SHGetSpecialFolderLocation = \33[1;32mNULL\33[0m, CSIDL\n",nFolder);
#endif
		printf("bad call to SHGetSpecialFolderLocation... CSIDL reqested is not CSIDL_APPDATA\n");
		return 1;
	}

	uid = getuid();
	if ( (pw = getpwuid(uid)) == NULL ) {
#ifdef DEBUG
		dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"getpwuid failed in SHGetSpecialFolderLocation for uid = %u... errno = %i\n",uid,errno);
#endif
		printf("getpwuid failed in SHGetSpecialFolderLocation...errno = %i\n",errno);
		return 2;
	}

	iSizeHomedir = strlen(pw->pw_dir);
	if ( iSizeHomedir < 1 ) {
#ifdef DEBUG
		dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"bad size of pw->pw_dir in SHGetSpecialFolderLocation, size = %u\n",iSizeHomedir );
#endif
		printf("bad size of pw->pw_dir in SHGetSpecialFolderLocation...\n");
		return 3;
	}
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"size of pw->pw_dir for uid %u in SHGetSpecialFolderLocation is %u, pw->pw_dir = \"%s\"\n",uid,iSizeHomedir,pw->pw_dir);
#endif
	if ( ( homedir = (char *)malloc( iSizeHomedir + 1 ) ) == NULL ) {
#ifdef DEBUG
		dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"malloc failed in SHGetSpecialFolderLocation, size = %u\n",iSizeHomedir+1 );
#endif
		printf("malloc failed in SHGetSpecialFolderLocation...\n");
		return 4;
	}
	strcpy(homedir,(const char *)pw->pw_dir);
	*ppidl = (LPITEMIDLIST)homedir;

	return (NOERROR);
}

WINSHELLAPI BOOL WINAPI SHGetPathFromIDList(LPCITEMIDLIST pidl, LPSTR pszPath)
{
	char *homedir;


	if ((homedir = getenv("HOME")) == NULL) {
	    homedir = (CHAR *)pidl;
	}
#ifdef DEBUG
	else {
		dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"SHGetPathFromIDList says $HOME environment variable = \"%s\"\n",getenv("HOME"));
	}
#endif
	if ( (strlen(homedir)+1) > _MAX_PATH ) {
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"SHGetPathFromIDList = \33[1;32mNULL\33[0m, length of homedir > _MAX_PATH(%i), homedir = \"%s\"\n",_MAX_PATH,homedir);
#endif
		return (FALSE);
	}
	strcpy(pszPath,(const char *)homedir);
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"SHGetPathFromIDList says homedir = \"%s\"\n",homedir);
#endif

	return (TRUE);
}

BOOL WINAPI CreateDirectory(
  _In_     LPCTSTR               lpPathName,
  _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes
)
{
	if ( lpSecurityAttributes != NULL ) {
#ifdef DEBUG
		dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"CreateDirectory = \33[1;32mNULL\33[0m ... called with non-NULL security attributes\n");
#endif
		printf("CreateDirectory failed due to call with non-NULL Security Attributes (not supported by this version)\n This is a programming error...\n");
		return (FALSE);
	}

//	struct stat st = {0};
	int result;

//	if (stat("/some/directory", &st) == -1) {
		result = mkdir(lpPathName, 0700);
//	}
	if ( result != 0 ) {
#ifdef DEBUG
		dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"CreateDirectory for \"%s\" failed with errno = %u\n",lpPathName,errno);
#endif
		return (FALSE);
	}

	return (TRUE);
}

int DialogBoxParam(
    HINSTANCE hInstance,
    LPCSTR lpTemplateName,
    HWND hWndParent ,
    DLGPROC lpDialogFunc,
    LPARAM dwInitParam)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"DialogBoxParam = \33[1;32mNULL\33[0m\n");
#endif
	return (0);
}

HWND WINAPI CreateWindow(
  _In_opt_ LPCTSTR   lpClassName,
  _In_opt_ LPCTSTR   lpWindowName,
  _In_     DWORD     dwStyle,
  _In_     int       x,
  _In_     int       y,
  _In_     int       nWidth,
  _In_     int       nHeight,
  _In_opt_ HWND      hWndParent,
  _In_opt_ HMENU     hMenu,
  _In_opt_ HINSTANCE hInstance,
  _In_opt_ LPVOID    lpParam
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"CreateWindow = \33[1;32mNULL\33[0m\n");
#endif
	return (JKNULL);
}

ATOM WINAPI RegisterClassEx(
  _In_ const WNDCLASSEX *lpwcx
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"RegisterClassEx = \33[1;32mNULL\33[0m\n");
#endif
	return (0);
}

LRESULT WINAPI DefWindowProc(
  _In_ HWND   hWnd,
  _In_ UINT   Msg,
  _In_ WPARAM wParam,
  _In_ LPARAM lParam
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"DefWindowProc = \33[1;32mNULL\33[0m\n");
#endif
	return (0);
}

HWND
WINAPI
HtmlHelp(
    HWND hwndCaller,
    LPCWSTR pszFile,
    UINT uCommand,
    DWORD_PTR dwData
    )
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"HtmlHelp = \33[1;32mNULL\33[0m\n");
#endif
	return (JKNULL);
}

//int wcstombs( char * strout, char * strin, int nchar ) {
//	return ( strlen( strncpy(strout, strin, nchar) ) );
//}
size_t wcstombs(
   char *mbstr,
   const char *wcstr,
   size_t count
)
{
#ifdef DEBUGWCSTOMBS
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"wcstombs = %s\n",wcstr);
#endif
	return ( strlen( strncpy(mbstr, wcstr, count) ) );
}


BOOL WINAPI FileTimeToSystemTime(
  _In_  const FILETIME     *lpFileTime,
  _Out_       LPSYSTEMTIME lpSystemTime
)
{
	time_t t;
	struct tm *tminfo;

	FileTimeToTimet( (FILETIME*)lpFileTime , &t );
	tminfo = gmtime( &t );
	tminfo->tm_sec = tminfo->tm_sec % 60;
	/*
	tm_sec	int	seconds after the minute	0-61*
	tm_min	int	minutes after the hour	0-59
	tm_hour	int	hours since midnight	0-23
	tm_mday	int	day of the month	1-31
	tm_mon	int	months since January	0-11
	tm_year	int	years since 1900
	tm_wday	int	days since Sunday	0-6
	tm_yday	int	days since January 1	0-365
	tm_isdst	int	Daylight Saving Time flag

	typedef struct _SYSTEMTIME {
	  WORD wYear;
	  WORD wMonth;
	  WORD wDayOfWeek;
	  WORD wDay;
	  WORD wHour;
	  WORD wMinute;
	  WORD wSecond;
	  WORD wMilliseconds;
	} SYSTEMTIME, *PSYSTEMTIME; */
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,DEBUGSETTINGSINPUT,"FileTimeToSystemTime = %s",asctime(tminfo));
#endif
	lpSystemTime->wYear = tminfo->tm_year + 1900;
	lpSystemTime->wMonth = tminfo->tm_mon + 1;
	lpSystemTime->wDay = tminfo->tm_mday;
	lpSystemTime->wDayOfWeek = tminfo->tm_wday;
	lpSystemTime->wHour = tminfo->tm_hour;
	lpSystemTime->wMinute = tminfo->tm_min;
	lpSystemTime->wSecond = tminfo->tm_sec;
	lpSystemTime->wMilliseconds = 0;

	return (TRUE);
}


BOOL WINAPI SystemTimeToTzSpecificLocalTime(
  _In_opt_ LPTIME_ZONE_INFORMATION lpTimeZone,
  _In_     LPSYSTEMTIME            lpUniversalTime,
  _Out_    LPSYSTEMTIME            lpLocalTime
)
{
	struct tm tminfo;
	struct tm *tmlocal;
	time_t	t;
	if ( lpTimeZone != NULL ) {
		printf("bad call to SystemTimeToTzSpecificLocalTime, lpTimeZone must be NULL\n");
		return (FALSE);
	}
	tminfo.tm_year = lpUniversalTime->wYear - 1900;
	tminfo.tm_mon =  lpUniversalTime->wMonth - 1;
	tminfo.tm_mday = lpUniversalTime->wDay;
	tminfo.tm_wday = lpUniversalTime->wDayOfWeek;
	tminfo.tm_hour = lpUniversalTime->wHour;
	tminfo.tm_min =  lpUniversalTime->wMinute;
	tminfo.tm_sec =  lpUniversalTime->wSecond;

	t = mktime(&tminfo);
	tmlocal = localtime(&t);

	tmlocal->tm_sec =         tmlocal->tm_sec % 60;
	lpLocalTime->wYear =      tmlocal->tm_year + 1900;
	lpLocalTime->wMonth =     tmlocal->tm_mon + 1;
	lpLocalTime->wDay =       tmlocal->tm_mday;
	lpLocalTime->wDayOfWeek = tmlocal->tm_wday;
	lpLocalTime->wHour =      tmlocal->tm_hour;
	lpLocalTime->wMinute =    tmlocal->tm_min;
	lpLocalTime->wSecond =    tmlocal->tm_sec;
	lpLocalTime->wMilliseconds = 0;


#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,DEBUGSETTINGSINPUT,"SystemTimeToTzSpecificLocalTime = %s",asctime(tmlocal));
#endif
	return (FALSE);
}


BOOL WINAPI MoveFile(
  _In_ LPCTSTR lpExistingFileName,
  _In_ LPCTSTR lpNewFileName
)
{
	if ( rename((const char *)lpExistingFileName, (const char *)lpNewFileName) == -1 ) {
		MsgBox("error moving \"%s\" to \"%s\", errno = , %i", lpExistingFileName,lpNewFileName,errno);
		return(FALSE);
	}
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"MoveFile \33[1;32mcalled\33[0m\n");
#endif
	return (TRUE);
}


BOOL WINAPI QueryPerformanceCounter(
  _Out_ LARGE_INTEGER *lpPerformanceCount
)
{
// note: the only place that QueryPerformanceCounter is used in My Secret Box is as an entropy source for
//       key generation. The call is in file gmpVarDef.cpp and it only uses the LowPart of the LARGE_INTEGER
//       that QueryPerformanceCounter returns. This is done to minimize the possibility of exhausting the entropy
//       in the random number source on linux. Take care not to expect the entire LARGE_INTEGER to be random.
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"calling QueryPerformanceCounter\n");
#endif
	BOOL bUsingDevRandon = TRUE;
	int randomData = open("/dev/random", O_RDONLY | O_NONBLOCK);
	if ( randomData == -1 ) {
		MsgBox("error, unable to open /dev/random\n");
		return (FALSE);
	}
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"open return = %i\n",randomData);
#endif
	size_t randomDataLen = 0;
	while (randomDataLen < sizeof (lpPerformanceCount->LowPart) )
	{
#ifdef DEBUG
		if (bUsingDevRandon) dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"reading /dev/random\n");
		if (!bUsingDevRandon) dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"reading /dev/urandom\n");
#endif
	    ssize_t result = read(randomData, &(lpPerformanceCount->LowPart), sizeof (lpPerformanceCount->LowPart) );
	    if (result < (ssize_t)sizeof (lpPerformanceCount->LowPart))
	    {
	    	if (!bUsingDevRandon) {
	    		MsgBox("error, unable to read /dev/urandom\n");
	    		return(FALSE);
	    	} else {
		// write message to log file
				openlog("secretbox", LOG_PID|LOG_CONS|LOG_NDELAY|LOG_NOWAIT, LOG_USER);
				syslog(LOG_INFO, "Warning: low entropy detected on /dev/random");
				closelog();
#ifdef DEBUG
				dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"QueryPerformanceCounter = %llx, size of result is %i, switching to /dev/urandom\n", lpPerformanceCount->LowPart,result );
#endif
				close(randomData);
				result = 0;
				bUsingDevRandon = FALSE;
				randomData = open("/dev/urandom", O_RDONLY);
				if ( randomData == -1 ) {
					MsgBox("error, unable to open /dev/urandom\n");
					return (FALSE);
				}
#ifdef DEBUG
				dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"open return = %i\n",randomData);
#endif
	    	}
	    }
	    randomDataLen += result;
	}
	close(randomData);

#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"QueryPerformanceCounter = %llx, size of result is %i\n", lpPerformanceCount->LowPart,randomDataLen );
#endif
	return (TRUE);
}

BOOL WINAPI EndDialog(
  _In_ HWND    hDlg,
  _In_ INT_PTR nResult
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"EndDialog = \33[1;32mNULL\33[0m\n");
#endif
	return (FALSE);
}

HWND WINAPI GetWindow(
  _In_ HWND hWnd,
  _In_ UINT uCmd
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"GetWindow = \33[1;32mNULL\33[0m\n");
#endif
	return (JKNULL);
}
BOOL WINAPI ShowWindow(
  _In_ HWND hWnd,
  _In_ int  nCmdShow
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"ShowWindow = \33[1;32mNULL\33[0m\n");
#endif
	return (FALSE);
}

HWND WINAPI GetParent(
  _In_ HWND hWnd
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"GetParent = \33[1;32mNULL\33[0m\n");
#endif
	return (JKNULL);
}

HWND WINAPI GetDesktopWindow(void)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"GetDesktopWindow = \33[1;32mNULL\33[0m\n");
#endif
	return (JKNULL);
}

BOOL WINAPI GetWindowRect(
  _In_  HWND   hWnd,
  _Out_ LPRECT lpRect
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"GetWindowRect = \33[1;32mNULL\33[0m\n");
#endif
	return (FALSE);
}

BOOL WINAPI SetWindowPos(
  _In_     HWND hWnd,
  _In_opt_ HWND hWndInsertAfter,
  _In_     int  X,
  _In_     int  Y,
  _In_     int  cx,
  _In_     int  cy,
  _In_     UINT uFlags
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"SetWindowPos = \33[1;32mNULL\33[0m\n");
#endif
	return (FALSE);
}

BOOL WINAPI SetWindowText(
  _In_     HWND    hWnd,
  _In_opt_ LPCTSTR lpString
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"SetWindowText = \33[1;32mNULL\33[0m\n");
#endif
	return (FALSE);
}

HWND WINAPI GetDlgItem(
  _In_opt_ HWND hDlg,
  _In_     int  nIDDlgItem
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"GetDlgItem = \33[1;32mNULL\33[0m\n");
#endif
	return (JKNULL);
}

int WINAPI GetWindowTextLength(
  _In_ HWND hWnd
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"GetWindowTextLength = \33[1;32mNULL\33[0m\n");
#endif
	return (0);
}

int WINAPI GetWindowText(
  _In_  HWND   hWnd,
  _Out_ LPTSTR lpString,
  _In_  int    nMaxCount
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"GetWindowText = \33[1;32mNULL\33[0m\n");
#endif
	return (0);
}

//BOOL WINAPI GetOpenFileNameTX( LPOPENFILENAME lpofn);
BOOL WINAPI GetOpenFileName(
  _Inout_ LPOPENFILENAME lpofn
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"GetOpenFileName = \33[1;32mNULL\33[0m\n");
#endif

#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME," in GetOpenFileName lStructSize = %u\n lpstrFilter = %s\n lpstrFile = %s\n nMaxFile = %u\n lpstrInitialDir = %s\n"
			     " lpstrFileTitle = %s\n nMaxFileTitle = %u\n lpstrTitle = %s\n lpstrDefExt = %s\n Flags = %x\n OFN_CREATEPROMPT is %s\n"
		         " OFN_HIDEREADONLY is %s\n OFN_NOCHANGEDIR is %s\n OFN_NOREADONLYRETURN is %s\n OFN_OVERWRITEPROMPT is %s\n OFN_FILEMUSTEXIST is %s\n",
			     SZNULL(lpofn->lStructSize), SZNULL(lpofn->lpstrFilter), SZNULL(lpofn->lpstrFile), lpofn->nMaxFile, SZNULL(lpofn->lpstrInitialDir), SZNULL(lpofn->lpstrFileTitle),
			     lpofn->nMaxFileTitle, SZNULL(lpofn->lpstrTitle), SZNULL(lpofn->lpstrDefExt), lpofn->Flags , FLAGSET(lpofn->Flags & OFN_CREATEPROMPT), FLAGSET(lpofn->Flags & OFN_HIDEREADONLY),
			     FLAGSET(lpofn->Flags & OFN_NOCHANGEDIR), FLAGSET(lpofn->Flags & OFN_NOREADONLYRETURN), FLAGSET(lpofn->Flags & OFN_OVERWRITEPROMPT), FLAGSET(lpofn->Flags & OFN_FILEMUSTEXIST));
#endif
//if (bTextMode) return (GetOpenFileNameTX(lpofn));
	return (FALSE);
}
/*
BOOL WINAPI GetOpenFileNameTX( LPOPENFILENAME lpofn){
#define MYDIRDIMSIZE	512
	char szMyDir[MYDIRDIMSIZE];
	char A[MYDIRDIMSIZE];

#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACE,"in GetOpenFileNameTX at line %i\n",__LINE__);
#endif

	*A = '\000';
	*szMyDir = '\000';
	GenRepString( strcpy(A,"  "), '*', 58);
	tutPrintString(A);
	tutPrintString(strcat(strcpy(A,"  "),lpofn->lpstrTitle));
	tutPrintString(" ");

	while(1) {
		int iAnswer;
		if ( getcwd(szMyDir,sizeof(szMyDir)) == NULL ) {
			MsgBox("getcwd failed in GetOpenFileNameTX with errno = %i",errno);
			return (FALSE);
		}
		tutDisplayPrompt ( "OPEN FILENAME MENU" , 2 , "DEFFOLDER" , szMyDir , "FILENAME" , lpofn->lpstrFile );
// fetch the input
		iAnswer = (int)tutFetchData(1);
#ifdef DEBUG
		dfprintf(__LINE__,__FILE__,TRACE,"tutFetchData at line %i in GetOpenFileNameTX returned iAnswer = %i\n",__LINE__,iAnswer);
#endif
		//   SAVE FILENAME MENU
   	    // 1) Change the folder that the file is located in
   	    // 2) List the files currently in the folder
   	    // 3) Specify a different file name
   	    // 4) Open the file and return to the previous menu
   	    // 5) Return to the previous menu without opening the file
		 //
		switch (iAnswer)
		{
			case 1:  // change directory
			{
				tutDisplayTextPrompt ( "ENTER DIRECTOY NAME" , 0 );
			// fetch the input
				*A = '\0';
				tutConvertString( A, DEFSTRINGSIZE );
				if (strlen(A) == 0) continue;
				if ( chdir(A) == -1 ) {
#ifdef DEBUG
					dfprintf(__LINE__,__FILE__,TRACE,"The directory change failed at line %i in GetOpenFileNameTX and returned errno = %i\n",__LINE__,errno);
#endif
					MsgBox("  The directory change failed.\n");
				}
				continue;
				break;
			}

			case 2:  // list directory
			{
				system ("ls -l");
				continue;
				break;
			}

			case 3:  // specify file name
			{
				tutDisplayTextPrompt ( "ENTER FILE NAME" , 0 );
			// fetch the input
				*A = '\0';
				tutConvertString( A, DEFSTRINGSIZE );
				if(strlen(A) == 0) continue;
				if (strlen(A) > lpofn->nMaxFile - 1) {
					MsgBox("  The file name entered is too long. Your name is %i characters.\n  The maximum permitted is %i\n  Input discarded.",strlen(A),lpofn->nMaxFile-1);
					continue;
				}
				strcpy(lpofn->lpstrFile,A);
				continue;
				break;
			}

			case 4:  // save file (must exist??)
			{
				if ( ( strlen(szMyDir) + strlen(lpofn->lpstrFile) + 2 ) > lpofn->nMaxFile ) {
					MsgBox("  The combination of file name and directory is too long.\n  The maximum permitted is %i\n  Try again with a shorter fully qualified name.",lpofn->nMaxFile-2);
					continue;
				}
				strcpy(A,szMyDir);
				if( ( A[strlen(A)] != '/' ) && *(lpofn->lpstrFile) != '/' ) strcat(A,"/");
				if( ( A[strlen(A)] == '/' ) && *(lpofn->lpstrFile) == '/' ) A[strlen(A)] = '\000';
				strcat(A,lpofn->lpstrFile);
				if ( lpofn->Flags & OFN_FILEMUSTEXIST ) {
// check that the file exsists
					// Check file existence.
					int rval = access(A, F_OK);
					if (rval == 0) {
#ifdef DEBUGGETFILEATTRIBUTES
						dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME, "%s exists\n", A);
#endif
					} else {
						if (errno == ENOENT) {
#ifdef DEBUG
							dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME, "%s does not exist\n", A);
#endif
						}
						if (errno == EACCES) {
#ifdef DEBUG
							dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME, "%s is not accessible\n", A);
#endif
						}
						MsgBox(
								"  The file does not exist or is not accessible\n  Try again with a different file.");
						continue;
					}
				}
				GenRepString( strcpy(A,"  "), '*', 58);
				tutPrintString(A);
				return(TRUE);
				break;
			}

			case 5:  // cancel
			{
				return(FALSE);
				break;
			}

			default:
			{
				continue;
				break;
			}

		}  // end switch
		break;

	}  // end while

	return(FALSE);
}

BOOL WINAPI GetSaveFileNameTX( LPOPENFILENAME lpofn);  */
BOOL WINAPI GetSaveFileName(
  _Inout_ LPOPENFILENAME lpofn
)
{
/*
 * typedef struct tagOFN {
  DWORD         lStructSize;  	// The length, in bytes, of the structure
  HWND          hwndOwner;		// A handle to the window that owns the dialog box.
  HINSTANCE     hInstance;		// If the OFN_ENABLETEMPLATEHANDLE flag is set in the Flags member,
  	  	  	  	  	  	  	  	 * hInstance is a handle to a memory object containing a dialog box template.
  	  	  	  	  	  	  	  	 * If the OFN_ENABLETEMPLATE flag is set, hInstance is a handle to a module that
  	  	  	  	  	  	  	  	 * contains a dialog box template named by the lpTemplateName member. If neither
  	  	  	  	  	  	  	  	 * flag is set, this member is ignored. If the OFN_EXPLORER flag is set, the system
  	  	  	  	  	  	  	  	 * uses the specified template to create a dialog box that is a child of the default
  	  	  	  	  	  	  	  	 * Explorer-style dialog box. If the OFN_EXPLORER flag is not set, the system uses
  	  	  	  	  	  	  	  	 * the template to create an old-style dialog box that replaces the default dialog box.
  LPCTSTR       lpstrFilter;	// A buffer containing pairs of null-terminated filter strings. The last string in the
  	  	  	  	  	  	  	  	 * buffer must be terminated by two NULL characters. The first string in each pair is
  	  	  	  	  	  	  	  	 * a display string that describes the filter (for example, "Text Files"), and the
  	  	  	  	  	  	  	  	 * second string specifies the filter pattern (for example, "*.TXT"). To specify multiple
  	  	  	  	  	  	  	  	 * filter patterns for a single display string, use a semicolon to separate the patterns
  LPTSTR        lpstrCustomFilter;	// A static buffer that contains a pair of null-terminated filter strings for preserving
  	  	  	  	  	  	  	  	  	 * the filter pattern chosen by the user. The first string is your display string that
  	  	  	  	  	  	  	  	  	 * describes the custom filter, and the second string is the filter pattern selected by
  	  	  	  	  	  	  	  	  	 * the user. The first time your application creates the dialog box, you specify the first
  	  	  	  	  	  	  	  	  	 * string, which can be any nonempty string. When the user selects a file, the dialog box
  	  	  	  	  	  	  	  	  	 * copies the current filter pattern to the second string. The system uses the strings to
  	  	  	  	  	  	  	  	  	 * initialize the user-defined file filter the next time the dialog box is created. If the
  	  	  	  	  	  	  	  	  	 * nFilterIndex member is zero, the dialog box uses the custom filter. If this member is NULL,
  	  	  	  	  	  	  	  	  	 * the dialog box does not preserve user-defined filter patterns.
  DWORD         nMaxCustFilter;		// The size, in characters, of the buffer identified by lpstrCustomFilter. This buffer should
  	  	  	  	  	  	  	  	  	 * be at least 40 characters long.
  DWORD         nFilterIndex;		// The index of the currently selected filter in the File Types control. The buffer pointed
  	  	  	  	  	  	  	  	  	 * to by lpstrFilter contains pairs of strings that define the filters. The first pair of
  	  	  	  	  	  	  	  	  	 * strings has an index value of 1, the second pair 2, and so on. An index of zero indicates
  	  	  	  	  	  	  	  	  	 * the custom filter specified by lpstrCustomFilter. You can specify an index on input to
  	  	  	  	  	  	  	  	  	 * indicate the initial filter description and filter pattern for the dialog box. When the user
  	  	  	  	  	  	  	  	  	 * selects a file, nFilterIndex returns the index of the currently displayed filter.
  LPTSTR        lpstrFile;			// If the OFN_ALLOWMULTISELECT flag is set and the user selects multiple files, the buffer
  	  	  	  	  	  	  	  	  	 * contains the current directory followed by the file names of the selected files. For
  	  	  	  	  	  	  	  	  	 * Explorer-style dialog boxes, the directory and file name strings are NULL separated, with an
  	  	  	  	  	  	  	  	  	 * extra NULL character after the last file name. If the buffer is too small, the function returns
  	  	  	  	  	  	  	  	  	 * FALSE and the CommDlgExtendedError function returns FNERR_BUFFERTOOSMALL. In this case, the first
  	  	  	  	  	  	  	  	  	 * two bytes of the lpstrFile buffer contain the required size, in bytes or characters.
  DWORD         nMaxFile;			// The size, in characters, of the buffer pointed to by lpstrFile. The GetOpenFileName and
  	  	  	  	  	  	  	  	  	 * GetSaveFileName functions return FALSE if the buffer is too small to contain the file information.
  	  	  	  	  	  	  	  	  	 * The buffer should be at least 256 characters long.
  LPTSTR        lpstrFileTitle;		// The file name and extension (without path information) of the selected file. This member can be NULL.
  DWORD         nMaxFileTitle;		// The size, in characters, of the buffer pointed to by lpstrFileTitle.
  LPCTSTR       lpstrInitialDir;	// The initial directory. The algorithm for selecting the initial directory varies on different platforms.
									 * Windows 7:
									 * If lpstrInitialDir has the same value as was passed the first time the application used an Open or Save
									 * As dialog box, the path most recently selected by the user is used as the initial directory.
									 * Otherwise, if lpstrFile contains a path, that path is the initial directory.
									 * Otherwise, if lpstrInitialDir is not NULL, it specifies the initial directory.
									 * If lpstrInitialDir is NULL and the current directory contains any files of the specified filter types,
									 * the initial directory is the current directory.
									 * Otherwise, the initial directory is the personal files directory of the current user.
									 * Otherwise, the initial directory is the Desktop folder.
  LPCTSTR       lpstrTitle;			// A string to be placed in the title bar of the dialog box. If this member is NULL, the system uses the
  	  	  	  	  	  	  	  	  	 * default title (that is, Save As or Open).
  DWORD         Flags;				// A set of bit flags you can use to initialize the dialog box. When the dialog box returns, it sets these
  	  	  	  	  	  	  	  	  	 * flags to indicate the user's input. This member can be a combination of the following flags:
  	  	  	  	  	  	  	  	  	 * OFN_ALLOWMULTISELECT (0x00000200) : The File Name list box allows multiple selections.
  	  	  	  	  	  	  	  	  	 * OFN_CREATEPROMPT (0x00002000) : If the user specifies a file that does not exist, this flag causes the
  	  	  	  	  	  	  	  	  	 *                                 dialog box to prompt the user for permission to create the file.
  	  	  	  	  	  	  	  	  	 * OFN_DONTADDTORECENT (0x02000000) : Prevents the system from adding a link to the selected file in the
  	  	  	  	  	  	  	  	  	 *                                    file system directory that contains the user's most recently used documents.
  	  	  	  	  	  	  	  	  	 * OFN_ENABLEHOOK (0x00000020) : Enables the hook function specified in the lpfnHook member.
  	  	  	  	  	  	  	  	  	 * OFN_ENABLEINCLUDENOTIFY (0x00400000) : Causes the dialog box to send CDN_INCLUDEITEM notification messages
  	  	  	  	  	  	  	  	  	 *                                        to your OFNHookProc hook procedure when the user opens a folder.
  	  	  	  	  	  	  	  	  	 * OFN_ENABLESIZING (0x00800000) : Enables the Explorer-style dialog box to be resized.
  	  	  	  	  	  	  	  	  	 * OFN_ENABLETEMPLATE (0x00000040) : The lpTemplateName member is a pointer to the name of a dialog template
  	  	  	  	  	  	  	  	  	 *                                   resource in the module identified by the hInstance member.
  	  	  	  	  	  	  	  	  	 * OFN_ENABLETEMPLATEHANDLE (0x00000080) : The hInstance member identifies a data block that contains a preloaded
  	  	  	  	  	  	  	  	  	 *                                         dialog box template. The system ignores lpTemplateName if this flag is specified.
  	  	  	  	  	  	  	  	  	 * OFN_EXPLORER (0x00080000) : Indicates that any customizations made to the Open or Save As dialog box use the
  	  	  	  	  	  	  	  	  	 *                             Explorer-style customization methods.
  	  	  	  	  	  	  	  	  	 * OFN_EXTENSIONDIFFERENT (0x00000400) : The user typed a file name extension that differs from the extension specified by
  	  	  	  	  	  	  	  	  	 *                                       lpstrDefExt.
  	  	  	  	  	  	  	  	  	 * OFN_FILEMUSTEXIST (0x00001000) : The user can type only names of existing files in the File Name entry field. If this
  	  	  	  	  	  	  	  	  	 *                                  flag is specified and the user enters an invalid name, the dialog box procedure displays
  	  	  	  	  	  	  	  	  	 *                                  a warning in a message box.
  	  	  	  	  	  	  	  	  	 * OFN_FORCESHOWHIDDEN (0x10000000) : Forces the showing of system and hidden files.
  	  	  	  	  	  	  	  	  	 * OFN_HIDEREADONLY (0x00000004) : Hides the Read Only check box.
  	  	  	  	  	  	  	  	  	 * OFN_LONGNAMES (0x00200000) : For old-style dialog boxes, this flag causes the dialog box to use long file names.
  	  	  	  	  	  	  	  	  	 * OFN_NOCHANGEDIR (0x00000008) : Restores the current directory to its original value if the user changed the directory
  	  	  	  	  	  	  	  	  	 *                                while searching for files.
  	  	  	  	  	  	  	  	  	 * OFN_NODEREFERENCELINKS (0x00100000) : Directs the dialog box to return the path and file name of the selected shortcut
  	  	  	  	  	  	  	  	  	 *                                       (.LNK) file. If this value is not specified, the dialog box returns the path and
  	  	  	  	  	  	  	  	  	 *                                       file name of the file referenced by the shortcut.
  	  	  	  	  	  	  	  	  	 * OFN_NOLONGNAMES (0x00040000) : For old-style dialog boxes, this flag causes the dialog box to use short file names (8.3 format).
  	  	  	  	  	  	  	  	  	 * OFN_NONETWORKBUTTON (0x00020000) : Hides and disables the Network button.
  	  	  	  	  	  	  	  	  	 * OFN_NOREADONLYRETURN (0x00008000) : The returned file does not have the Read Only check box selected and is not in a
  	  	  	  	  	  	  	  	  	 *                                     write-protected directory.
  	  	  	  	  	  	  	  	  	 * OFN_NOTESTFILECREATE (0x00010000) : The file is not created before the dialog box is closed.
  	  	  	  	  	  	  	  	  	 * OFN_NOVALIDATE (0x00000100) : The common dialog boxes allow invalid characters in the returned file name.
  	  	  	  	  	  	  	  	  	 * OFN_OVERWRITEPROMPT (0x00000002) : Causes the Save As dialog box to generate a message box if the selected file already
  	  	  	  	  	  	  	  	  	 *                                    exists. The user must confirm whether to overwrite the file.
  	  	  	  	  	  	  	  	  	 * OFN_PATHMUSTEXIST (0x00000800) : The user can type only valid paths and file names.
  	  	  	  	  	  	  	  	  	 * OFN_READONLY (0x00000001) : Causes the Read Only check box to be selected initially when the dialog box is created.
  	  	  	  	  	  	  	  	  	 *                             This flag indicates the state of the Read Only check box when the dialog box is closed.
  	  	  	  	  	  	  	  	  	 * OFN_SHAREAWARE (0x00004000) : Specifies that if a call to the OpenFile function fails because of a network sharing violation,
  	  	  	  	  	  	  	  	  	 *                               the error is ignored and the dialog box returns the selected file name.
  	  	  	  	  	  	  	  	  	 * OFN_SHOWHELP (0x00000010) : Causes the dialog box to display the Help button. The hwndOwner member must specify the window
  	  	  	  	  	  	  	  	  	 *                             to receive the HELPMSGSTRING registered messages that the dialog box sends when the user clicks
  	  	  	  	  	  	  	  	  	 *                             the Help button.
  WORD          nFileOffset;		// The zero-based offset, in characters, from the beginning of the path to the file name in the string pointed to by lpstrFile.
  WORD          nFileExtension;		// The zero-based offset, in characters, from the beginning of the path to the file name extension in the string pointed to by
                                     * lpstrFile.
  LPCTSTR       lpstrDefExt;		// The default extension. GetOpenFileName and GetSaveFileName append this extension to the file name if the user fails to type
  	  	  	  	  	  	  	         * an extension.
  LPARAM        lCustData;			// Application-defined data that the system passes to the hook procedure identified by the lpfnHook member.
  LPOFNHOOKPROC lpfnHook;			// A pointer to a hook procedure.
  LPCTSTR       lpTemplateName;		// The name of the dialog template resource in the module identified by the hInstance member. For numbered dialog box resources,
                                     * this can be a value returned by the MAKEINTRESOURCE macro.
#if (_WIN32_WINNT >= 0x0500)
  void          *pvReserved;		// This member is reserved.
  DWORD         dwReserved;			// This member is reserved.
  DWORD         FlagsEx;			// A set of bit flags you can use to initialize the dialog box. Currently, this member can be zero or the following flag:
  	  	  	  	  	  	  	  	  	 * OFN_EX_NOPLACESBAR (0x00000001) : If this flag is set, the places bar is not displayed. If this flag is not set,
  	  	  	  	  	  	  	  	  	 *                                   Explorer-style dialog boxes include a places bar containing icons for commonly-used
  	  	  	  	  	  	  	  	  	 *                                   folders, such as Favorites and Desktop.
#endif
} OPENFILENAME, *LPOPENFILENAME;
 *
 */
  	   DWORD        lStructSize;
//	   HWND         hwndOwner;
//	   HINSTANCE    hInstance;
	   LPCSTR       lpstrFilter;
//	   LPSTR        lpstrCustomFilter;
//	   DWORD        nMaxCustFilter;
//	   DWORD        nFilterIndex;
	   LPSTR        lpstrFile;
	   DWORD        nMaxFile;
	   LPSTR        lpstrFileTitle;
	   DWORD        nMaxFileTitle;
	   LPCSTR       lpstrInitialDir;
	   LPCSTR       lpstrTitle;
	   DWORD        Flags;
//	   WORD         nFileOffset;
//	   WORD         nFileExtension;
	   LPCSTR       lpstrDefExt;
//	   LPARAM       lCustData;
//	   LPOFNHOOKPROC lpfnHook;
//	   LPCSTR       lpTemplateName;

/* (from SaveFileDialg)
 *  ofn.lStructSize       = sizeof(OPENFILENAME);
    ofn.hwndOwner	  	  = hDlg;
    ofn.lpstrFilter	  	  = szFilterSpec;
    ofn.lpstrCustomFilter = NULL;
    ofn.nMaxCustFilter	  = 0;
    ofn.nFilterIndex	  = 0;
    ofn.lpstrFile         = szFileName;
    ofn.nMaxFile		  = nMaxFName;
    ofn.lpstrInitialDir   = NULL;
    ofn.lpstrFileTitle    = szFileTitle;
    ofn.nMaxFileTitle     = nMaxFileTitle;
    ofn.lpstrTitle        = "Save Password Database File";
    ofn.lpstrDefExt       = NULL;
//	if (clear text) ofn.lpstrDefExt       = "txt";
    ofn.Flags             = OFN_CREATEPROMPT | OFN_HIDEREADONLY | OFN_NOCHANGEDIR |
		                    OFN_NOREADONLYRETURN | OFN_OVERWRITEPROMPT;
 *
 */
/* (from OpenFileDialg)
 *    ofn.lStructSize       = sizeof(OPENFILENAME);
    ofn.hwndOwner	  = hDlg;
    ofn.lpstrFilter	  = szFilterSpec;
    ofn.lpstrCustomFilter = NULL;
    ofn.nMaxCustFilter	  = 0;
    ofn.nFilterIndex	  = 0;
    ofn.lpstrFile         = szFileName;
    ofn.nMaxFile		  = nMaxFName;
    ofn.lpstrInitialDir   = NULL;
    ofn.lpstrFileTitle    = szFileTitle;
    ofn.nMaxFileTitle     = nMaxFileTitle;
    ofn.lpstrTitle        = "Open PASSWD File";
    ofn.lpstrDefExt       = "";
    ofn.Flags             = OFN_FILEMUSTEXIST;
 *
 */
/* * from SaveTextHelpFile)
 * 		ofn.lStructSize       = sizeof(OPENFILENAME);
		ofn.hwndOwner	  = hDlg;
		ofn.lpstrFilter	  = szFilterSpec;
		ofn.lpstrCustomFilter = NULL;
		ofn.nMaxCustFilter	  = 0;
		ofn.nFilterIndex	  = 0;
		ofn.lpstrFile         = szFileName;
		ofn.nMaxFile		  = nMaxFName;
		ofn.lpstrInitialDir   = NULL;
		ofn.lpstrFileTitle    = szFileTitle;
		ofn.nMaxFileTitle     = nMaxFileTitle;
		ofn.lpstrTitle        = "Save Reset Instructions File";
		ofn.lpstrDefExt       = NULL;
	//	if (clear text) ofn.lpstrDefExt       = "txt";
		ofn.lpstrDefExt       = "txt";
		ofn.Flags             = OFN_CREATEPROMPT | OFN_HIDEREADONLY | OFN_NOCHANGEDIR |
								OFN_NOREADONLYRETURN | OFN_OVERWRITEPROMPT;
 *
 */


#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"GetSaveFileName = \33[1;32mNULL\33[0m\n");
#endif
	lStructSize = lpofn->lStructSize;
//    lpofn.hwndOwner	  = hDlg;
	lpstrFilter = lpofn->lpstrFilter;
//    lpofn.lpstrCustomFilter = NULL;
//    lpofn.nMaxCustFilter	  = 0;
//    lpofn.nFilterIndex	  = 0;
	lpstrFile = lpofn->lpstrFile;
	nMaxFile = lpofn->nMaxFile;
	lpstrInitialDir = lpofn->lpstrInitialDir;
	lpstrFileTitle = lpofn->lpstrFileTitle;
	nMaxFileTitle = lpofn->nMaxFileTitle;
	lpstrTitle = lpofn->lpstrTitle;
	lpstrDefExt = lpofn->lpstrDefExt;
//	if (clear text) lpofn.lpstrDefExt       = "txt";
	Flags = lpofn->Flags;
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME," in GetSaveFileName lStructSize = %u\n lpstrFilter = %s\n lpstrFile = %s\n nMaxFile = %u\n lpstrInitialDir = %s\n"
			     " lpstrFileTitle = %s\n nMaxFileTitle = %u\n lpstrTitle = %s\n lpstrDefExt = %s\n Flags = %x\n OFN_CREATEPROMPT is %s\n"
		         " OFN_HIDEREADONLY is %s\n OFN_NOCHANGEDIR is %s\n OFN_NOREADONLYRETURN is %s\n OFN_OVERWRITEPROMPT is %s\n OFN_FILEMUSTEXIST is %s\n",
			     SZNULL(lStructSize), SZNULL(lpstrFilter), SZNULL(lpstrFile), nMaxFile, SZNULL(lpstrInitialDir), SZNULL(lpstrFileTitle),
			     nMaxFileTitle, SZNULL(lpstrTitle), SZNULL(lpstrDefExt), Flags , FLAGSET(Flags & OFN_CREATEPROMPT), FLAGSET(Flags & OFN_HIDEREADONLY),
			     FLAGSET(Flags & OFN_NOCHANGEDIR), FLAGSET(Flags & OFN_NOREADONLYRETURN), FLAGSET(Flags & OFN_OVERWRITEPROMPT), FLAGSET(Flags & OFN_FILEMUSTEXIST));
#endif
//if (bTextMode) return (GetSaveFileNameTX(lpofn));
	return (FALSE);
}
/*
BOOL WINAPI GetSaveFileNameTX( LPOPENFILENAME lpofn){
#define MYDIRDIMSIZE	512
	char szMyDir[MYDIRDIMSIZE];
	char A[MYDIRDIMSIZE];

#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACE,"in GetSaveFileNameTX at line %i\n",__LINE__);
#endif

	*A = '\000';
	*szMyDir = '\000';
	GenRepString( strcpy(A,"  "), '*', 58);
	tutPrintString(A);
	tutPrintString(strcat(strcpy(A,"  "),lpofn->lpstrTitle));
	tutPrintString(" ");

	while(1) {
		int iAnswer;
		if ( getcwd(szMyDir,sizeof(szMyDir)) == NULL ) {
			MsgBox("getcwd failed in GetSaveFileNameTX with errno = %i",errno);
			return (FALSE);
		}
		tutDisplayPrompt ( "SAVE FILENAME MENU" , 2 , "DEFFOLDER" , szMyDir , "FILENAME" , lpofn->lpstrFile );
// fetch the input
		iAnswer = (int)tutFetchData(1);
#ifdef DEBUG
		dfprintf(__LINE__,__FILE__,TRACE,"tutFetchData at line %i in GetSaveFileNameTX returned iAnswer = %i\n",__LINE__,iAnswer);
#endif
		//   SAVE FILENAME MENU
   	    // 1) Change the folder that the file will be saved in
   	    // 2) List the files the folder
   	    // 3) Specify a different save file name
   	    // 4) Save the file and return to the previous menu
   	    // 5) Return to the previous menu without saving the file
		 //
		switch (iAnswer)
		{
			case 1:  // change directory
			{
				tutDisplayTextPrompt ( "ENTER DIRECTOY NAME" , 0 );
			// fetch the input
				*A = '\0';
				tutConvertString( A, DEFSTRINGSIZE );
				if (strlen(A) == 0) continue;
				if ( chdir(A) == -1 ) {
#ifdef DEBUG
					dfprintf(__LINE__,__FILE__,TRACE,"The directory change failed at line %i in GetSaveFileNameTX and returned errno = %i\n",__LINE__,errno);
#endif
					MsgBox("  The directory change failed.\n");
				}
				continue;
				break;
			}

			case 2:  // list directory
			{
				system ("ls -l");
				continue;
				break;
			}

			case 3:  // specify file name
			{
				tutDisplayTextPrompt ( "ENTER FILE NAME" , 0 );
			// fetch the input
				*A = '\0';
				tutConvertString( A, DEFSTRINGSIZE );
				if(strlen(A) == 0) continue;
				if (strlen(A) > lpofn->nMaxFile - 1) {
					MsgBox("  The file name entered is too long. Your name is %i characters.\n  The maximum permitted is %i\n  Input discarded.",strlen(A),lpofn->nMaxFile-1);
					continue;
				}
				strcpy(lpofn->lpstrFile,A);
				continue;
				break;
			}

			case 4:  // save file
			{
				if ( ( strlen(szMyDir) + strlen(lpofn->lpstrFile) + 2 ) > lpofn->nMaxFile ) {
					MsgBox("  The combination of file name and directory is too long.\n  The maximum permitted is %i\n  Try again with a shorter fully qualified name.",lpofn->nMaxFile-2);
					continue;
				}
				strcpy(A,szMyDir);
				if( ( A[strlen(A)] != '/' ) && *(lpofn->lpstrFile) != '/' ) strcat(A,"/");
				if( ( A[strlen(A)] == '/' ) && *(lpofn->lpstrFile) == '/' ) A[strlen(A)] = '\000';
				strcat(A,lpofn->lpstrFile);
				GenRepString( strcpy(A,"  "), '*', 58);
				tutPrintString(A);
				return(TRUE);
				break;
			}

			case 5:  // cancel
			{
				return(FALSE);
				break;
			}

			default:
			{
				continue;
				break;
			}

		}  // end switch
		break;

	}  // end while

	return(FALSE);
}
*/

UINT WINAPI GetDlgItemText(
  _In_  HWND   hDlg,
  _In_  int    nIDDlgItem,
  _Out_ LPTSTR lpString,
  _In_  int    nMaxCount
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"GetDlgItemText = \33[1;32mNULL\33[0m\n");
#endif
	return (0);
}

HBRUSH CreateSolidBrush(
  _In_ COLORREF crColor
)
{
	unsigned int r,g,b;
	r = crColor & 0xff;
	g = (crColor>>8) & 0xff;
	b = (crColor>>16) & 0xff;
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"CreateSolidBrush = NULL (r=%u,g=%u,b=%u)\n",r,g,b);
#endif
	return (JKNULL);
}

BOOL WinHelp(
   HWND      hWndMain,
   LPCTSTR   lpszHelp,
   UINT      uCommand,
   ULONG_PTR dwData
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"WinHelp = \33[1;32mNULL\33[0m\n");
#endif
	return (FALSE);
}

LRESULT WINAPI SendMessage(
  _In_ HWND   hWnd,
  _In_ UINT   Msg,
  _In_ WPARAM wParam,
  _In_ LPARAM lParam
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"SendMessage = \33[1;32mNULL\33[0m\n");
#endif
	return (0);
}

UINT_PTR WINAPI SetTimer(
  _In_opt_ HWND      hWnd,
  _In_     UINT_PTR  nIDEvent,
  _In_     UINT      uElapse,
  _In_opt_ TIMERPROC lpTimerFunc
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"SetTimer = \33[1;32mNULL\33[0m\n");
#endif
	return (0);
}

BOOL WINAPI KillTimer(
  _In_opt_ HWND     hWnd,
  _In_     UINT_PTR uIDEvent
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"KillTimer = \33[1;32mNULL\33[0m\n");
#endif
	return (FALSE);
}


BOOL WINAPI GetClientRect(
  _In_  HWND   hWnd,
  _Out_ LPRECT lpRect
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"GetClientRect = \33[1;32mNULL\33[0m\n");
#endif
	return (FALSE);
}

int MapWindowPoints(
  _In_    HWND    hWndFrom,
  _In_    HWND    hWndTo,
  _Inout_ LPPOINT lpPoints,
  _In_    UINT    cPoints
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"MapWindowPoints = \33[1;32mNULL\33[0m\n");
#endif
	return (0);
}

HWND WINAPI CreateWindowEx(
  _In_     DWORD     dwExStyle,
  _In_opt_ LPCTSTR   lpClassName,
  _In_opt_ LPCTSTR   lpWindowName,
  _In_     DWORD     dwStyle,
  _In_     int       x,
  _In_     int       y,
  _In_     int       nWidth,
  _In_     int       nHeight,
  _In_opt_ HWND      hWndParent,
  _In_opt_ HMENU     hMenu,
  _In_opt_ HINSTANCE hInstance,
  _In_opt_ LPVOID    lpParam
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"CreateWindowEx = \33[1;32mNULL\33[0m\n");
#endif
	return (JKNULL);
}

HGDIOBJ GetStockObject(
  _In_ int fnObject
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"GetStockObject = \33[1;32mNULL\33[0m\n");
#endif
	return (JKNULL);
}

UINT IsDlgButtonChecked(
  _In_ HWND hDlg,
  _In_ int  nIDButton
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"IsDlgButtonChecked = \33[1;32mNULL\33[0m\n");
#endif
	return (0);
}

BOOL RedrawWindow(
  _In_       HWND hWnd,
  _In_ const RECT *lprcUpdate,
  _In_       HRGN hrgnUpdate,
  _In_       UINT flags
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"RedrawWindow = \33[1;32mNULL\33[0m\n");
#endif
	return (FALSE);
}

HGLOBAL GlobalAlloc(
    UINT uFlags,
    DWORD dwBytes
    )
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"GlobalAlloc = \33[1;32mNULL\33[0m\n");
#endif
	return (JKNULL);
}

LPVOID GlobalLock(
    HGLOBAL hMem
    )
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"GlobalLock = \33[1;32mNULL\33[0m\n");
#endif
	return (NULL);
}

BOOL GlobalUnlock(
    HGLOBAL hMem
    )
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"GlobalUnlock = \33[1;32mNULL\33[0m\n");
#endif
	return (FALSE);
}

BOOL OpenClipboard(
    HWND hWndNewOwner
	)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"OpenClipboard = \33[1;32mNULL\33[0m\n");
#endif
	return (FALSE);
}

BOOL EmptyClipboard(
	)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"EmptyClipboard = \33[1;32mNULL\33[0m\n");
#endif
	return (FALSE);
}

HANDLE SetClipboardData(
    UINT uFormat,
    HANDLE hMem
	)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"SetClipboardData = \33[1;32mNULL\33[0m\n");
#endif
	return (JKNULL);
}

BOOL CloseClipboard(
	)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"CloseClipboard = \33[1;32mNULL\33[0m\n");
#endif
	return (FALSE);
}

BOOL CheckDlgButton(
  _In_ HWND hDlg,
  _In_ int  nIDButton,
  _In_ UINT uCheck
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"CheckDlgButton = \33[1;32mNULL\33[0m\n");
#endif
	return (FALSE);
}

BOOL WINAPI SetDlgItemText(
  _In_ HWND    hDlg,
  _In_ int     nIDDlgItem,
  _In_ LPCTSTR lpString
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"SetDlgItemText = \33[1;32mNULL\33[0m\n");
#endif
	return (FALSE);
}

BOOL ScreenToClient(
  _In_ HWND    hWnd,
       LPPOINT lpPoint
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"ScreenToClient = \33[1;32mNULL\33[0m\n");
#endif
	return (FALSE);
}

BOOL ClientToScreen(
  _In_    HWND    hWnd,
  _Inout_ LPPOINT lpPoint
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"ClientToScreen = \33[1;32mNULL\33[0m\n");
#endif
	return (FALSE);
}

BOOL WINAPI MoveWindow(
  _In_ HWND hWnd,
  _In_ int  X,
  _In_ int  Y,
  _In_ int  nWidth,
  _In_ int  nHeight,
  _In_ BOOL bRepaint
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"MoveWindow = \33[1;32mNULL\33[0m\n");
#endif
	return (FALSE);
}

UINT WINAPI GetDlgItemInt(
  _In_      HWND hDlg,
  _In_      int  nIDDlgItem,
  _Out_opt_ BOOL *lpTranslated,
  _In_      BOOL bSigned
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"GetDlgItemInt = \33[1;32mNULL\33[0m\n");
#endif
	return (0);
}

BOOL WINAPI SetDlgItemInt(
  _In_ HWND hDlg,
  _In_ int  nIDDlgItem,
  _In_ UINT uValue,
  _In_ BOOL bSigned
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"SetDlgItemInt = \33[1;32mNULL\33[0m\n");
#endif
	return (0);
}

HWND WINAPI SetFocus(
  _In_opt_ HWND hWnd
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"SetFocus = \33[1;32mNULL\33[0m\n");
#endif
	return (JKNULL);
}


BOOL WINAPI EnableWindow(
  _In_ HWND hWnd,
  _In_ BOOL bEnable
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"EnableWindow = \33[1;32mNULL\33[0m\n");
#endif
	return (FALSE);
}

char *  itoa ( int value, char * str, int base )
{
	if ( base == 10 ) {
		snprintf(str, 29, "%i" , value);
	} else if ( base == 16 ) {
		snprintf(str, 29, "%x" , value);
	} else if ( base == 8 ) {
		snprintf(str, 29, "%o" , value);
	} else if ( base < 10 ) {
		int tempchar[66];
		int i=0;
		int tempval;
		tempval=value;
		while (i<64 && abs(tempval) != 0) 
		{
			tempchar[i]=tempval%base;
			tempval/=base;
			i++;
		}
		if (tempval != 0) {
			*str = '\000';
			printf("itoa = NULL\n");
			return (NULL);
		}
		if (value<0) 
		{
			str[tempval] = '-';
			tempval++;
		}
		while ( i > 0 )
		{
			snprintf(&str[tempval], 1, "%i" , tempchar[i]);
			i--;
			tempval++;
		}
	} else {
		*str = '\000';
		printf("itoa = NULL\n");
		return (NULL);
	}
	return (str);
}

int WINAPI SetBkMode(HDC,int)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"SetBkMode = \33[1;32mNULL\33[0m\n");
#endif
	return (0);
}


BOOL ComboBox_ShowDropdown(
   HWND hwndCtl,
   BOOL fShow
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"ComboBox_ShowDropdown = \33[1;32mNULL\33[0m\n");
#endif
	return (TRUE);
}

LRESULT WINAPI SendDlgItemMessage(
  _In_ HWND   hDlg,
  _In_ int    nIDDlgItem,
  _In_ UINT   Msg,
  _In_ WPARAM wParam,
  _In_ LPARAM lParam
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"SendDlgItemMessage = \33[1;32mNULL\33[0m\n");
#endif
	return (TRUE);
}

BOOL InvalidateRect(
  _In_       HWND hWnd,
  _In_ const RECT *lpRect,
  _In_       BOOL bErase
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"InvalidateRect = \33[1;32mNULL\33[0m\n");
#endif
	return (TRUE);
}


BOOL UpdateWindow(
  _In_ HWND hWnd
)
{
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACEGETFULLPATHNAME,"UpdateWindow = \33[1;32mNULL\33[0m\n");
#endif
	return (TRUE);
}







/*
 * lindows.h
 *
 *  Created on  Dec 9, 2015
 *      Author  dad
 */

#ifndef LINDOWS_H_
#define LINDOWS_H_

#include "windef.h"
#include <stdarg.h>
#include <sys/types.h>

#ifndef bool
    #define bool int
    #define false ((bool)0)
    #define true  ((bool)1)
#endif

#ifdef WINDOZE
#define JKNULL	NULL
#else	// WINDOZE
#ifndef JKNULL
#ifdef __cplusplus
#define JKNULL    0
#else	//  __cplusplus
#define JKNULL    ((void *)0)
#endif	//  __cplusplus
#endif	// JKNULL
#endif	// WINDOZE


typedef LONG HRESULT;
typedef void	VOID, *PVOID;
typedef long long	__int64;
typedef long long	LONGLONG;
#if defined(_WIN64)
 typedef __int64 INT_PTR;
#else
 typedef int INT_PTR;
#endif
typedef union _LARGE_INTEGER {
  struct {
    DWORD LowPart;
    LONG  HighPart;
  };
  struct {
    DWORD LowPart;
    LONG  HighPart;
  } u;
  LONGLONG QuadPart;
} LARGE_INTEGER, *PLARGE_INTEGER;
//typedef void *PVOID;
//typedef PVOID HANDLE;
typedef HANDLE HWND;
//typedef HANDLE*	HWND;
typedef HANDLE*	JKPHANDLE;
typedef HANDLE	HINSTANCE;
typedef HINSTANCE HMODULE;
#define WINAPI
typedef HANDLE	HRSRC;
//typedef HANDLE HGLOBAL;
typedef HANDLE HCURSOR;
typedef HANDLE HBRUSH;
typedef HANDLE HICON;
typedef HANDLE HMENU;
typedef HANDLE HFONT;
typedef HANDLE HRGN;
typedef HANDLE HDC;
typedef char*	LPSTR;
typedef char*	LPTSTR;
typedef const char* LPCSTR;
typedef char	CHAR;
typedef char	TCHAR;
typedef char*	LPWSTR;
typedef const char* LPCWSTR;
typedef const char *	LPCTSTR;
typedef CONST CHAR *PCSTR;
typedef unsigned long DWORD, *PDWORD, *LPDWORD;
typedef unsigned int ULONG_PTR, UINT_PTR;
typedef ULONG_PTR DWORD_PTR;
#ifndef _TCHAR_DEFINED
typedef char TCHAR, *PTCHAR;
typedef char	WCHAR;
typedef unsigned char TBYTE , *PTBYTE ;
#define _In_
#define _In_opt_
#define _Out_
#define _In_Out_
#define _Inout_
#define _Out_opt_
#define _Reserved_
#define _TCHAR_DEFINED
#define UNALIGNED
#define WINSHELLAPI
#endif /* !_TCHAR_DEFINED */

typedef struct _SYSTEM_INFO {
  union {
    DWORD  dwOemId;
    struct {
      WORD wProcessorArchitecture;
      WORD wReserved;
    };
  };
  DWORD     dwPageSize;
  LPVOID    lpMinimumApplicationAddress;
  LPVOID    lpMaximumApplicationAddress;
  DWORD_PTR dwActiveProcessorMask;
  DWORD     dwNumberOfProcessors;
  DWORD     dwProcessorType;
  DWORD     dwAllocationGranularity;
  WORD      wProcessorLevel;
  WORD      wProcessorRevision;
} SYSTEM_INFO;
typedef _SYSTEM_INFO*	LPSYSTEM_INFO;
typedef struct _OVERLAPPED {
  ULONG_PTR Internal;
  ULONG_PTR InternalHigh;
  union {
    struct {
      DWORD Offset;
      DWORD OffsetHigh;
    };
    PVOID  Pointer;
  };
  HANDLE    hEvent;
} OVERLAPPED, *LPOVERLAPPED;
typedef void * PVOID;
typedef struct _DCB {
  DWORD DCBlength;
  DWORD BaudRate;
  DWORD fBinary  :1;
  DWORD fParity  :1;
  DWORD fOutxCtsFlow  :1;
  DWORD fOutxDsrFlow  :1;
  DWORD fDtrControl  :2;
  DWORD fDsrSensitivity  :1;
  DWORD fTXContinueOnXoff  :1;
  DWORD fOutX  :1;
  DWORD fInX  :1;
  DWORD fErrorChar  :1;
  DWORD fNull  :1;
  DWORD fRtsControl  :2;
  DWORD fAbortOnError  :1;
  DWORD fDummy2  :17;
  WORD  wReserved;
  WORD  XonLim;
  WORD  XoffLim;
  BYTE  ByteSize;
  BYTE  Parity;
  BYTE  StopBits;
  char  XonChar;
  char  XoffChar;
  char  ErrorChar;
  char  EofChar;
  char  EvtChar;
  WORD  wReserved1;
} DCB, *LPDCB;
typedef struct _SECURITY_ATTRIBUTES {
  DWORD  nLength;
  LPVOID lpSecurityDescriptor;
  BOOL   bInheritHandle;
} SECURITY_ATTRIBUTES, *PSECURITY_ATTRIBUTES, *LPSECURITY_ATTRIBUTES;
typedef u_int           SOCKET;
/*typedef struct in_addr {
  union {
    struct {
      u_char s_b1,s_b2,s_b3,s_b4;
    } S_un_b;
    struct {
      u_short s_w1,s_w2;
    } S_un_w;
    u_long S_addr;
  } S_un;
} IN_ADDR, *PIN_ADDR, FAR *LPIN_ADDR;*/
/*struct sockaddr_in{
short sin_family;
unsigned short sin_port;
struct in_addr sin_addr;
char sin_zero[8];
};*/
#define WSADESCRIPTION_LEN      256
#define WSASYS_STATUS_LEN       128
typedef struct WSAData {
  WORD           wVersion;
  WORD           wHighVersion;
  char           szDescription[WSADESCRIPTION_LEN+1];
  char           szSystemStatus[WSASYS_STATUS_LEN+1];
  unsigned short iMaxSockets;
  unsigned short iMaxUdpDg;
  char FAR       *lpVendorInfo;
} WSADATA, *LPWSADATA;
/*typedef struct hostent {
  char FAR      *h_name;
  char FAR  FAR **h_aliases;
  short         h_addrtype;
  short         h_length;
  char FAR  FAR **h_addr_list;
} HOSTENT, *PHOSTENT, FAR *LPHOSTENT; */
typedef struct _COORD {
  SHORT X;
  SHORT Y;
} COORD, *PCOORD;
typedef struct _SMALL_RECT {
  SHORT Left;
  SHORT Top;
  SHORT Right;
  SHORT Bottom;
} SMALL_RECT;
typedef struct _CONSOLE_SCREEN_BUFFER_INFO {
    COORD dwSize;
    COORD dwCursorPosition;
    WORD  wAttributes;
    SMALL_RECT srWindow;
    COORD dwMaximumWindowSize;
} CONSOLE_SCREEN_BUFFER_INFO, *PCONSOLE_SCREEN_BUFFER_INFO;
typedef struct _CHAR_INFO {
  union {
    WCHAR UnicodeChar;
    CHAR  AsciiChar;
  } Char;
  WORD  Attributes;
} CHAR_INFO, *PCHAR_INFO;
typedef struct _SHITEMID        // mkid
{
    USHORT      cb;             // Size of the ID (including cb itself)
    BYTE        abID[1];        // The item ID (variable length)
} SHITEMID;
typedef UNALIGNED SHITEMID *LPSHITEMID;
typedef const UNALIGNED SHITEMID *LPCSHITEMID;
typedef struct _ITEMIDLIST      // idl
{
    SHITEMID    mkid;
} ITEMIDLIST;
typedef UNALIGNED ITEMIDLIST * LPITEMIDLIST;
typedef const UNALIGNED ITEMIDLIST * LPCITEMIDLIST;
//#define IMalloc()		malloc()
//typedef void *(*LPMALLOC)(size_t);
typedef BOOL (CALLBACK* DLGPROC)(HWND, UINT, WPARAM, LPARAM);
typedef LRESULT (CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
typedef VOID (CALLBACK* TIMERPROC)(HWND, UINT, UINT, DWORD);
typedef struct tagWNDCLASSEX {
    UINT        cbSize;
    /* Win 3.x */
    UINT        style;
    WNDPROC     lpfnWndProc;  // WNDPROC
    int         cbClsExtra;
    int         cbWndExtra;
    HINSTANCE   hInstance;
    HICON       hIcon;
    HCURSOR     hCursor;
    HBRUSH      hbrBackground;
    LPCSTR      lpszMenuName;
    LPCSTR      lpszClassName;
    /* Win 4.0 */
    HICON       hIconSm;
} WNDCLASSEX, *PWNDCLASSEX, NEAR *NPWNDCLASSEX, FAR *LPWNDCLASSEX;
typedef struct tagCREATESTRUCT {
  LPVOID    lpCreateParams;
  HINSTANCE hInstance;
  HMENU     hMenu;
  HWND      hwndParent;
  int       cy;
  int       cx;
  int       y;
  int       x;
  LONG      style;
  LPCTSTR   lpszName;
  LPCTSTR   lpszClass;
  DWORD     dwExStyle;
} CREATESTRUCT, *LPCREATESTRUCT;
typedef struct tagNMHDR {
  HWND     hwndFrom;
  UINT_PTR idFrom;
  UINT     code;
} NMHDR;
typedef struct tagHHN_NOTIFY
{
    NMHDR   hdr;
    PCSTR   pszUrl; // Multi-byte, null-terminated string
} HHN_NOTIFY;
typedef DWORD HH_INFOTYPE;
#define HH_MAX_TABS 19  // maximum number of tabs
typedef struct tagHH_WINTYPE {
    int     cbStruct;        // IN: size of this structure including all Information Types
    BOOL    fUniCodeStrings; // IN/OUT: TRUE if all strings are in UNICODE
    LPCTSTR pszType;         // IN/OUT: Name of a type of window
    DWORD   fsValidMembers;  // IN: Bit flag of valid members (HHWIN_PARAM_)
    DWORD   fsWinProperties; // IN/OUT: Properties/attributes of the window (HHWIN_)

    LPCTSTR pszCaption;      // IN/OUT: Window title
    DWORD   dwStyles;        // IN/OUT: Window styles
    DWORD   dwExStyles;      // IN/OUT: Extended Window styles
    RECT    rcWindowPos;     // IN: Starting position, OUT: current position
    int     nShowState;      // IN: show state (e.g., SW_SHOW)
    HWND  hwndHelp;          // OUT: window handle
    HWND  hwndCaller;        // OUT: who called this window
    HH_INFOTYPE* paInfoTypes;  // IN: Pointer to an array of Information Types
    // The following members are only valid if HHWIN_PROP_TRI_PANE is set
    HWND  hwndToolBar;      // OUT: toolbar window in tri-pane window
    HWND  hwndNavigation;   // OUT: navigation window in tri-pane window
    HWND  hwndHTML;         // OUT: window displaying HTML in tri-pane window
    int   iNavWidth;        // IN/OUT: width of navigation window
    RECT  rcHTML;           // OUT: HTML window coordinates
    LPCTSTR pszToc;         // IN: Location of the table of contents file
    LPCTSTR pszIndex;       // IN: Location of the index file
    LPCTSTR pszFile;        // IN: Default location of the html file
    LPCTSTR pszHome;        // IN/OUT: html file to display when Home button is clicked
    DWORD   fsToolBarFlags; // IN: flags controling the appearance of the toolbar
    BOOL    fNotExpanded;   // IN: TRUE/FALSE to contract or expand, OUT: current state
    int     curNavType;     // IN/OUT: UI to display in the navigational pane
    int     tabpos;         // IN/OUT: HHWIN_NAVTAB_TOP, HHWIN_NAVTAB_LEFT, or HHWIN_NAVTAB_BOTTOM
    int     idNotify;       // IN: ID to use for WM_NOTIFY messages
    BYTE    tabOrder[HH_MAX_TABS + 1];    // IN/OUT: tab order: Contents, Index, Search, History, Favorites, Reserved 1-5, Custom tabs
    int     cHistory;       // IN/OUT: number of history items to keep (default is 30)
    LPCTSTR pszJump1;       // Text for HHWIN_BUTTON_JUMP1
    LPCTSTR pszJump2;       // Text for HHWIN_BUTTON_JUMP2
    LPCTSTR pszUrlJump1;    // URL for HHWIN_BUTTON_JUMP1
    LPCTSTR pszUrlJump2;    // URL for HHWIN_BUTTON_JUMP2
    RECT    rcMinSize;      // Minimum size for window (ignored in version 1)
    int     cbInfoTypes;    // size of paInfoTypes;
    LPCTSTR pszCustomTabs;  // multiple zero-terminated strings
} HH_WINTYPE, *PHH_WINTYPE;
typedef struct tagHHNTRACK
{
    NMHDR   hdr;
    PCSTR   pszCurUrl;      // Multi-byte, null-terminated string
    int     idAction;       // HHACT_ value
    HH_WINTYPE* phhWinType; // Current window type structure
} HHNTRACK;
enum {
    HHACT_TAB_CONTENTS,
    HHACT_TAB_INDEX,
    HHACT_TAB_SEARCH,
    HHACT_TAB_HISTORY,
    HHACT_TAB_FAVORITES,

    HHACT_EXPAND,
    HHACT_CONTRACT,
    HHACT_BACK,
    HHACT_FORWARD,
    HHACT_STOP,
    HHACT_REFRESH,
    HHACT_HOME,
    HHACT_SYNC,
    HHACT_OPTIONS,
    HHACT_PRINT,
    HHACT_HIGHLIGHT,
    HHACT_CUSTOMIZE,
    HHACT_JUMP1,
    HHACT_JUMP2,
    HHACT_ZOOM,
    HHACT_TOC_NEXT,
    HHACT_TOC_PREV,
    HHACT_NOTES,

    HHACT_LAST_ENUM,
};
typedef struct _FILETIME {
    DWORD dwLowDateTime;
    DWORD dwHighDateTime;
} FILETIME, *PFILETIME, *LPFILETIME;
typedef struct _SYSTEMTIME {
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
} SYSTEMTIME, *PSYSTEMTIME, *LPSYSTEMTIME;
typedef struct _TIME_ZONE_INFORMATION {
    LONG Bias;
    WCHAR StandardName[ 32 ];
    SYSTEMTIME StandardDate;
    LONG StandardBias;
    WCHAR DaylightName[ 32 ];
    SYSTEMTIME DaylightDate;
    LONG DaylightBias;
} TIME_ZONE_INFORMATION, *PTIME_ZONE_INFORMATION, *LPTIME_ZONE_INFORMATION;
typedef struct tagWINDOWPOS {
    HWND    hwnd;
    HWND    hwndInsertAfter;
    int     x;
    int     y;
    int     cx;
    int     cy;
    UINT    flags;
} WINDOWPOS, *LPWINDOWPOS, *PWINDOWPOS;
typedef UINT (APIENTRY *LPOFNHOOKPROC) (HWND, UINT, WPARAM, LPARAM);
typedef struct tagOFNA {
   DWORD        lStructSize;
   HWND         hwndOwner;
   HINSTANCE    hInstance;
   LPCSTR       lpstrFilter;
   LPSTR        lpstrCustomFilter;
   DWORD        nMaxCustFilter;
   DWORD        nFilterIndex;
   LPSTR        lpstrFile;
   DWORD        nMaxFile;
   LPSTR        lpstrFileTitle;
   DWORD        nMaxFileTitle;
   LPCSTR       lpstrInitialDir;
   LPCSTR       lpstrTitle;
   DWORD        Flags;
   WORD         nFileOffset;
   WORD         nFileExtension;
   LPCSTR       lpstrDefExt;
   LPARAM       lCustData;
   LPOFNHOOKPROC lpfnHook;
   LPCSTR       lpTemplateName;
} OPENFILENAME, *LPOPENFILENAME;
typedef struct tagHELPINFO      /* Structure pointed to by lParam of WM_HELP */
{
    UINT    cbSize;             /* Size in bytes of this struct  */
    int     iContextType;       /* Either HELPINFO_WINDOW or HELPINFO_MENUITEM */
    int     iCtrlId;            /* Control Id or a Menu item Id. */
    HANDLE  hItemHandle;        /* hWnd of control or hMenu.     */
    DWORD   dwContextId;        /* Context Id associated with this item */
    POINT   MousePos;           /* Mouse Position in screen co-ordinates */
}  HELPINFO, FAR *LPHELPINFO;


#define _MAX_PATH			256
#define NOERROR				0
#define WM_USER                         0x0400
#define HWND_DESKTOP        ((HWND)0)
#define SW_HIDE 			1
#define SW_MINIMIZE 		2
#define SW_RESTORE 			3
#define SW_SHOW 			4
#define SW_SHOWMAXIMIZED	5
#define SW_SHOWMINIMIZED 	6
#define SW_SHOWMINNOACTIVE 	7
#define SW_SHOWNA 			8
#define SW_SHOWNOACTIVATE 	9
#define SW_SHOWNORMAL 		10
#define SW_FORCEMINIMIZE 	11
#define SW_SHOWDEFAULT 		12
#define SWP_NOMOVE          0x0002
#define SWP_NOZORDER        0x0004
#define MB_YESNO			4
#define MB_ICONHAND			0x10
#define MB_ICONQUESTION		0x20
#define MB_ICONEXCLAMATION	0x30
#define MB_ICONERROR		0x10
#define MB_SYSTEMMODAL		0x1000
#define MB_HELP				0x4000
#define MB_YESNOCANCEL		0x3
#define MB_ICONINFORMATION	0x40L
#define MB_ICONASTERISK		0x40L
#define MB_OK				0
#define MB_APPLMODAL		0
#define CB_ADDSTRING                0x0143
#define CB_RESETCONTENT             0x014B
#define CB_GETCURSEL                0x0147
#define CB_SETCURSEL                0x014E
#define CB_OKAY				0
#define CB_ERR              (-1)
#define CB_ERRSPACE         (-2)
#define CB_DELETESTRING             0x0144
#define CBN_SELCHANGE       1
#define CF_TEXT             1
#define BST_UNCHECKED      0x0000
#define BST_CHECKED        0x0001
#define BM_GETCHECK        0x00F0
#define BN_CLICKED          0
#define BS_PUSHBUTTON       0x00000000L
#define BS_AUTOCHECKBOX     0x00000003L
#define EM_SETPASSWORDCHAR      0x00CC
#define EM_GETPASSWORDCHAR      0x00D2
#define ES_PASSWORD         0x0020L
#define ES_AUTOHSCROLL      0x0080L
#define ES_READONLY         0x0800L
#define EN_SETFOCUS         0x0100
#define EN_CHANGE           0x0300
#define IDOK				1
#define IDCANCEL			2
#define IDYES				6
#define IDNO				7
#define SM_DBCSENABLED 		42
#define SM_CYKANJIWINDOW 	18
#define SM_CXSCREEN 		0
#define SM_CYSCREEN 		1
#define SM_CXDLGFRAME 		7
#define SM_CYDLGFRAME 		8
#define SM_CYCAPTION 		4
#define SM_PENWINDOWS 		41
#define SM_DEBUG 			22
#define WS_CHILD            0x40000000L
#define WS_VISIBLE          0x10000000L
#define WS_EX_CLIENTEDGE    0x00000200L
#define WS_TABSTOP          0x00010000L
#define RDW_INVALIDATE          0x0001
#define RDW_ALLCHILDREN         0x0080
#define RDW_UPDATENOW           0x0100
#define UDM_SETRANGE            (WM_USER+101)
#define UDM_SETPOS              (WM_USER+103)
#define DEFAULT_GUI_FONT    17
#define PROCESSOR_ARCHITECTURE_INTEL	0
#define PROCESSOR_ARCHITECTURE_MIPS		1
#define PROCESSOR_ARCHITECTURE_ALPHA	2
#define PROCESSOR_ARCHITECTURE_PPC		3
#define PROCESSOR_ARCHITECTURE_SHX		4
#define PROCESSOR_ARCHITECTURE_ARM		5
#define PROCESSOR_ARCHITECTURE_IA64		6
#define PROCESSOR_ARCHITECTURE_ALPHA64	7
#define PROCESSOR_ARCHITECTURE_AMD64	9
#define PROCESSOR_INTEL_386				386
#define PROCESSOR_INTEL_486				486
#define PROCESSOR_INTEL_PENTIUM			586
#define PROCESSOR_MIPS_R4000    4000    // incl R4101 & R3910 for Windows CE
#define PROCESSOR_ALPHA_21064   21064
#define PROCESSOR_PPC_601       601
#define PROCESSOR_PPC_603       603
#define PROCESSOR_PPC_604       604
#define PROCESSOR_PPC_620       620
#define PROCESSOR_HITACHI_SH3   10003   // Windows CE
#define PROCESSOR_HITACHI_SH3E  10004   // Windows CE
#define PROCESSOR_HITACHI_SH4   10005   // Windows CE
#define PROCESSOR_MOTOROLA_821  821     // Windows CE
#define PROCESSOR_SHx_SH3       103     // Windows CE
#define PROCESSOR_SHx_SH4       104     // Windows CE
#define PROCESSOR_STRONGARM     2577    // Windows CE - 0xA11
#define PROCESSOR_ARM720        1824    // Windows CE - 0x720
#define PROCESSOR_ARM820        2080    // Windows CE - 0x820
#define PROCESSOR_ARM920        2336    // Windows CE - 0x920
#define PROCESSOR_ARM_7TDMI     70001   // Windows CE
#define STD_INPUT_HANDLE		(DWORD)-10
#define STD_OUTPUT_HANDLE		(DWORD)-11
#define STD_ERROR_HANDLE		(DWORD)-12
#define INVALID_HANDLE_VALUE	-1
#define GENERIC_READ                     (0x80000000L)
#define GENERIC_WRITE                    (0x40000000L)
#define GENERIC_EXECUTE                  (0x20000000L)
#define GENERIC_ALL                      (0x10000000L)
#define FILE_SHARE_READ                 	0x00000001
#define FILE_ATTRIBUTE_NORMAL               0x00000080
#define CREATE_NEW				1
#define CREATE_ALWAYS       	2
#define OPEN_EXISTING			3
#define NOPARITY            0
#define ODDPARITY           1
#define EVENPARITY          2
#define MARKPARITY          3
#define SPACEPARITY         4
#define ONESTOPBIT          0
#define ONE5STOPBITS        1
#define TWOSTOPBITS         2
#define FORMAT_MESSAGE_FROM_SYSTEM		0x00001000
#define FORMAT_MESSAGE_IGNORE_INSERTS	0x00000200
#define MAKELANGID(p, s)       ((((WORD  )(s)) << 10) | (WORD  )(p))
#define PRIMARYLANGID(lgid)    ((WORD  )(lgid) & 0x3ff)
#define SUBLANGID(lgid)        ((WORD  )(lgid) >> 10)
#define SUBLANG_NEUTRAL                  0x00    // language neutral
#define SUBLANG_DEFAULT                  0x01    // user default
#define SUBLANG_SYS_DEFAULT              0x02    // system default
#define LANG_NEUTRAL                     0x00
#define LANG_ENGLISH                     0x09
#define LANG_MALAY                       0x3e
#define __TEXT(quote) quote         // r_winnt
#define TEXT(quote) __TEXT(quote)   // r_winnt
#define _tprintf			printf
#define UNUSED(x) x __attribute__((unused))
#define CDERR_DIALOGFAILURE   0xFFFF
#define CDERR_STRUCTSIZE       0x0001
#define CDERR_INITIALIZATION   0x0002
#define CDERR_NOTEMPLATE       0x0003
#define CDERR_NOHINSTANCE      0x0004
#define CDERR_LOADSTRFAILURE   0x0005
#define CDERR_FINDRESFAILURE   0x0006
#define CDERR_LOADRESFAILURE   0x0007
#define CDERR_LOCKRESFAILURE   0x0008
#define CDERR_MEMALLOCFAILURE  0x0009
#define CDERR_MEMLOCKFAILURE   0x000A
#define CDERR_NOHOOK           0x000B
#define CDERR_REGISTERMSGFAIL  0x000C
#define PDERR_SETUPFAILURE     0x1001
#define PDERR_PARSEFAILURE     0x1002
#define PDERR_RETDEFFAILURE    0x1003
#define PDERR_LOADDRVFAILURE   0x1004
#define PDERR_GETDEVMODEFAIL   0x1005
#define PDERR_INITFAILURE      0x1006
#define PDERR_NODEVICES        0x1007
#define PDERR_NODEFAULTPRN     0x1008
#define PDERR_DNDMMISMATCH     0x1009
#define PDERR_CREATEICFAILURE  0x100A
#define PDERR_PRINTERNOTFOUND  0x100B
#define PDERR_DEFAULTDIFFERENT 0x100C
#define CFERR_NOFONTS          0x2001
#define CFERR_MAXLESSTHANMIN   0x2002
#define FNERR_SUBCLASSFAILURE  0x3001
#define FNERR_INVALIDFILENAME  0x3002
#define FNERR_BUFFERTOOSMALL   0x3003
#define FRERR_BUFFERLENGTHZERO 0x4001
#define INVALID_SOCKET  (SOCKET)(~0)
#define SOCKET_ERROR            -1
//#define SOCK_DGRAM      2               /* datagram socket */
#define FOREGROUND_RED       0x0004 // text color contains red.
#define FOREGROUND_INTENSITY 0x0008 // text color is intensified.
#define CONSOLE_TEXTMODE_BUFFER  1
#define lstrlen  strlen
#define lstrlenA  strlen
#define wcscmp	strcmp
#define wcschr	strchr
#define wcslen	strlen
#define ENABLE_LINE_INPUT      0x0002
#define ENABLE_ECHO_INPUT      0x0004
#define FORMAT_MESSAGE_ALLOCATE_BUFFER 0x00000100
#define FORMAT_MESSAGE_ARGUMENT_ARRAY  0x00002000
#define FILE_ATTRIBUTE_DIRECTORY	16
#define CSIDL_APPDATA                   0x001a
#define CW_USEDEFAULT       ((int)0x80000000)
#define GW_OWNER            4
#define GMEM_MOVEABLE       0x0002
#define GMEM_DDESHARE       0x2000
#define WM_NCCREATE                     0x0081
#define WM_NULL                         0x0000
#define WM_CREATE                       0x0001
#define WM_DESTROY                      0x0002
#define WM_MOVE                         0x0003
#define WM_SIZE                         0x0005
#define WM_ACTIVATE                     0x0006
#define WM_SETFONT                      0x0030
#define WM_COMMAND                      0x0111
#define WM_HELP                         0x0053
#define WM_NOTIFY                       0x004E
#define WM_TCARD                        0x0052
#define WM_CLOSE                        0x0010
#define WM_WINDOWPOSCHANGING            0x0046
#define WM_INITDIALOG                   0x0110
#define WM_SYSCOMMAND                   0x0112
#define WM_CONTEXTMENU                  0x007B
#define WM_CTLCOLORDLG                  0x0136
#define WM_TIMER                        0x0113
#define WM_CTLCOLORSTATIC				0x0138
#define WM_SETCURSOR					0x0020
#define HELP_FINDER       0x000b
#define HELP_QUIT         0x0002L  /* Terminate help */
#define HH_DISPLAY_TOPIC        0x0000
#define HH_HELP_CONTEXT         0x000F  // display mapped numeric value in dwData
#define HH_GET_WIN_TYPE         0x0005
#define HH_SET_WIN_TYPE         0x0004
#define HH_TP_HELP_CONTEXTMENU  0x0010  // text popup help, same as WinHelp HELP_CONTEXTMENU
#define HH_TP_HELP_WM_HELP      0x0011  // text popup help, same as WinHelp HELP_WM_HELP
#define HHN_FIRST       		(0U-860U)
#define HHN_LAST        		(0U-879U)
#define HHN_NAVCOMPLETE   		(HHN_FIRST-0)
#define HHN_TRACK         		(HHN_FIRST-1)
#define HHN_WINDOW_CREATE 		(HHN_FIRST-2)
#define SWP_SHOWWINDOW      	0x0040
#define	WA_ACTIVE				1
#define SC_CONTEXTHELP			0xF180
#define HH_CLOSE_ALL            0x0012  // close all windows opened directly or indirectly by the caller
#define HWND_TOP        ((HWND)0)
#define Edit_GetTextLength(hwndCtl)             GetWindowTextLength(hwndCtl)
#define Edit_GetText(hwndCtl, lpch, cchMax)     GetWindowText((hwndCtl), (lpch), (cchMax))
#define OFN_OVERWRITEPROMPT          0x00000002
#define OFN_HIDEREADONLY             0x00000004
#define OFN_NOCHANGEDIR              0x00000008
#define OFN_CREATEPROMPT             0x00002000
#define OFN_NOREADONLYRETURN         0x00008000
#define OFN_FILEMUSTEXIST            0x00001000
#define RGB(r,g,b)          ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))
#define Edit_SetText(hwndCtl, lpsz)             SetWindowText((hwndCtl), (lpsz))
#define Static_SetText(hwndCtl, lpsz)           SetWindowText((hwndCtl), (lpsz))
#define MAKELONG(a, b)      ((LONG)(((WORD)(a)) | ((DWORD)((WORD)(b))) << 16))
#define MAKELPARAM(l, h)      (LPARAM)MAKELONG(l, h)
#define TRANSPARENT 1




//int wcstombs( char * strout, char * strin, int nchar );
HMODULE WINAPI GetModuleHandle(
   LPCTSTR lpModuleName
);
DWORD WINAPI GetFullPathName(
    LPCTSTR lpFileName,
    DWORD   nBufferLength,
   LPTSTR  lpBuffer,
   LPTSTR  *lpFilePart
);
LPTSTR MAKEINTRESOURCE(
   WORD wInteger
);
#define RT_RCDATA	MAKEINTRESOURCE(10)
HRSRC WINAPI FindResource(
   HMODULE hModule,
       LPCTSTR lpName,
       LPCTSTR lpType
);
HGLOBAL WINAPI LoadResource(
   HMODULE hModule,
       HRSRC   hResInfo
);
LPVOID WINAPI LockResource(
   HGLOBAL hResData
);
int WINAPI MessageBox(
   HWND    hWnd,
   LPCTSTR lpText,
   LPCTSTR lpCaption,
       UINT    uType
);
DWORD WINAPI GetModuleFileName(
   HMODULE hModule,
      LPTSTR  lpFilename,
       DWORD   nSize
);
DWORD WINAPI GetLastError(void);
BOOL WINAPI GetFileVersionInfo(
         LPCTSTR lptstrFilename,
		 HANDLE*   dwHandle,
         DWORD   dwLen,
        LPVOID  lpData
);
DWORD WINAPI GetFileVersionInfoSize(
        LPCTSTR lptstrFilename,
   LPDWORD lpdwHandle
);
BOOL WINAPI VerQueryValue(
    LPCVOID pBlock,
    LPCTSTR lpSubBlock,
   LPVOID  *lplpBuffer,
   PUINT   puLen
);
void WINAPI GetSystemInfo(
   LPSYSTEM_INFO lpSystemInfo
);
DWORD WINAPI GetVersion(void);
int WINAPI GetSystemMetrics(
   int nIndex
);
BOOL WINAPI WriteFile(
          HANDLE       hFile,
          LPCVOID      lpBuffer,
          DWORD        nNumberOfBytesToWrite,
     LPDWORD      lpNumberOfBytesWritten,
   LPOVERLAPPED lpOverlapped
);
void WINAPI OutputDebugString(
   LPCTSTR lpOutputString
);
HANDLE WINAPI GetStdHandle(
   DWORD nStdHandle
);
DWORD WINAPI GetProfileString(
    LPCTSTR lpAppName,
    LPCTSTR lpKeyName,
    LPCTSTR lpDefault,
   LPTSTR  lpReturnedString,
    DWORD   nSize
);
HANDLE WINAPI CreateFile(
       LPCTSTR               lpFileName,
       DWORD                 dwDesiredAccess,
       DWORD                 dwShareMode,
   LPSECURITY_ATTRIBUTES lpSecurityAttributes,
       DWORD                 dwCreationDisposition,
       DWORD                 dwFlagsAndAttributes,
	   JKPHANDLE                hTemplateFile
);
BOOL WINAPI SetupComm(
   HANDLE hFile,
   DWORD  dwInQueue,
   DWORD  dwOutQueue
);
BOOL WINAPI GetCommState(
      HANDLE hFile,
   LPDCB  lpDCB
);
BOOL WINAPI SetCommState(
   HANDLE hFile,
   LPDCB  lpDCB
);
BOOL WINAPI FlushFileBuffers(
   HANDLE hFile
);
BOOL WINAPI FreeConsole(void);
BOOL WINAPI GetComputerName(
     LPTSTR  lpBuffer,
   LPDWORD lpnSize
);
DWORD WINAPI FormatMessage(
  _In_     DWORD   dwFlags,
  _In_opt_ LPCVOID lpSource,
  _In_     DWORD   dwMessageId,
  _In_     DWORD   dwLanguageId,
  _Out_    LPTSTR  lpBuffer,
  _In_     DWORD   nSize,
  _In_opt_ va_list *Arguments
);
size_t _msize(
void *memblock
);
DWORD WINAPI CommDlgExtendedError(void);
BOOL WINAPI MessageBeep(
  _In_ UINT uType
);
int WINAPI LoadString(
  _In_opt_ HINSTANCE hInstance,
  _In_     UINT      uID,
  _Out_    LPTSTR    lpBuffer,
  _In_     int       nBufferMax
);
HWND WINAPI GetFocus(void);
void WINAPI FatalExit(
  _In_ int ExitCode
);
BOOL WINAPI DestroyWindow(
  _In_ HWND hWnd
);
int PASCAL FAR WSAStartup(WORD wVersionRequired, LPWSADATA lpWSAData);
int PASCAL FAR WSAGetLastError(void);
int PASCAL FAR WSACleanup(void);
/*struct hostent* FAR gethostbyname(
  _In_ const char *name
);
unsigned long inet_addr(
  _In_ const char *cp
); */
int closesocket(
  _In_ SOCKET s
);
HANDLE WINAPI CreateConsoleScreenBuffer(
  _In_             DWORD               dwDesiredAccess,
  _In_             DWORD               dwShareMode,
  _In_opt_   const SECURITY_ATTRIBUTES *lpSecurityAttributes,
  _In_             DWORD               dwFlags,
  _Reserved_       LPVOID              lpScreenBufferData
);
BOOL WINAPI ScrollConsoleScreenBuffer(
  _In_           HANDLE     hConsoleOutput,
  _In_     const SMALL_RECT *lpScrollRectangle,
  _In_opt_ const SMALL_RECT *lpClipRectangle,
  _In_           COORD      dwDestinationOrigin,
  _In_     const CHAR_INFO  *lpFill
);
BOOL WINAPI AllocConsole(void);
BOOL WINAPI GetConsoleScreenBufferInfo(
  _In_  HANDLE                      hConsoleOutput,
  _Out_ PCONSOLE_SCREEN_BUFFER_INFO lpConsoleScreenBufferInfo
);
BOOL WINAPI SetConsoleTextAttribute(
  _In_ HANDLE hConsoleOutput,
  _In_ WORD   wAttributes
);
BOOL WINAPI SetConsoleScreenBufferSize(
  _In_ HANDLE hConsoleOutput,
  _In_ COORD  dwSize
);
BOOL WINAPI GetConsoleMode(
  _In_  HANDLE  hConsoleHandle,
  _Out_ LPDWORD lpMode
);
BOOL WINAPI SetConsoleMode(
  _In_ HANDLE hConsoleHandle,
  _In_ DWORD  dwMode
);
BOOL WINAPI SetConsoleCursorPosition(
  _In_ HANDLE hConsoleOutput,
  _In_ COORD  dwCursorPosition
);
HLOCAL WINAPI LocalFree(
  _In_ HLOCAL hMem
);
BOOL WINAPI CloseHandle(
  _In_ HANDLE hObject
);
LPWSTR* CommandLineToArgvW(
  _In_  LPCWSTR lpCmdLine,
  _Out_ int     *pNumArgs
);
LPWSTR GetCommandLineW(void);
BOOL WINAPI DeleteFile(
  _In_ LPCTSTR lpFileName
);
BOOL WINAPI CopyFile(
  _In_ LPCTSTR lpExistingFileName,
  _In_ LPCTSTR lpNewFileName,
  _In_ BOOL    bFailIfExists
);
DWORD WINAPI GetFileAttributes(
  _In_ LPCTSTR lpFileName
);
WINSHELLAPI HRESULT WINAPI SHGetSpecialFolderLocation(HWND hwndOwner, int nFolder, LPITEMIDLIST * ppidl);
WINSHELLAPI BOOL WINAPI SHGetPathFromIDList(LPCITEMIDLIST pidl, LPSTR pszPath);
//WINSHELLAPI HRESULT WINAPI SHGetMalloc(LPMALLOC * ppMalloc);
BOOL WINAPI CreateDirectory(
  _In_     LPCTSTR               lpPathName,
  _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes
);
int DialogBoxParam(
    HINSTANCE hInstance,
    LPCSTR lpTemplateName,
    HWND hWndParent ,
    DLGPROC lpDialogFunc,
    LPARAM dwInitParam);
#define DialogBox(hInstance, lpTemplate, hWndParent, lpDialogFunc) \
DialogBoxParam(hInstance, lpTemplate, hWndParent, lpDialogFunc, 0L)
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
);
ATOM WINAPI RegisterClassEx(
  _In_ const WNDCLASSEX *lpwcx
);
LRESULT WINAPI DefWindowProc(
  _In_ HWND   hWnd,
  _In_ UINT   Msg,
  _In_ WPARAM wParam,
  _In_ LPARAM lParam
);
HWND
WINAPI
HtmlHelp(
    HWND hwndCaller,
    LPCWSTR pszFile,
    UINT uCommand,
    DWORD_PTR dwData
    );
size_t wcstombs(
   char *mbstr,
   const char *wcstr,
   size_t count
);
DWORD WINAPI SizeofResource(
  _In_opt_ HMODULE hModule,
  _In_     HRSRC   hResInfo
);
BOOL WINAPI GetFileTime(
  _In_      HANDLE     hFile,
  _Out_opt_ LPFILETIME lpCreationTime,
  _Out_opt_ LPFILETIME lpLastAccessTime,
  _Out_opt_ LPFILETIME lpLastWriteTime
);
BOOL WINAPI FileTimeToSystemTime(
  _In_  const FILETIME     *lpFileTime,
  _Out_       LPSYSTEMTIME lpSystemTime
);
BOOL WINAPI SystemTimeToTzSpecificLocalTime(
  _In_opt_ LPTIME_ZONE_INFORMATION lpTimeZone,
  _In_     LPSYSTEMTIME            lpUniversalTime,
  _Out_    LPSYSTEMTIME            lpLocalTime
);
BOOL WINAPI MoveFile(
  _In_ LPCTSTR lpExistingFileName,
  _In_ LPCTSTR lpNewFileName
);
BOOL WINAPI QueryPerformanceCounter(
  _Out_ LARGE_INTEGER *lpPerformanceCount
);
BOOL WINAPI EndDialog(
  _In_ HWND    hDlg,
  _In_ INT_PTR nResult
);
HWND WINAPI GetWindow(
  _In_ HWND hWnd,
  _In_ UINT uCmd
);
BOOL WINAPI ShowWindow(
  _In_ HWND hWnd,
  _In_ int  nCmdShow
);
HWND WINAPI GetParent(
  _In_ HWND hWnd
);
HWND WINAPI GetDesktopWindow(void);
BOOL WINAPI GetWindowRect(
  _In_  HWND   hWnd,
  _Out_ LPRECT lpRect
);
BOOL WINAPI SetWindowPos(
  _In_     HWND hWnd,
  _In_opt_ HWND hWndInsertAfter,
  _In_     int  X,
  _In_     int  Y,
  _In_     int  cx,
  _In_     int  cy,
  _In_     UINT uFlags
);
BOOL WINAPI SetWindowText(
  _In_     HWND    hWnd,
  _In_opt_ LPCTSTR lpString
);
HWND WINAPI GetDlgItem(
  _In_opt_ HWND hDlg,
  _In_     int  nIDDlgItem
);
int WINAPI GetWindowTextLength(
  _In_ HWND hWnd
);
int WINAPI GetWindowText(
  _In_  HWND   hWnd,
  _Out_ LPTSTR lpString,
  _In_  int    nMaxCount
);
BOOL WINAPI GetSaveFileName(
  _Inout_ LPOPENFILENAME lpofn
);
UINT WINAPI GetDlgItemText(
  _In_  HWND   hDlg,
  _In_  int    nIDDlgItem,
  _Out_ LPTSTR lpString,
  _In_  int    nMaxCount
);
HBRUSH CreateSolidBrush(
  _In_ COLORREF crColor
);
BOOL WinHelp(
   HWND      hWndMain,
   LPCTSTR   lpszHelp,
   UINT      uCommand,
   ULONG_PTR dwData
);
LRESULT WINAPI SendMessage(
  _In_ HWND   hWnd,
  _In_ UINT   Msg,
  _In_ WPARAM wParam,
  _In_ LPARAM lParam
);
UINT_PTR WINAPI SetTimer(
  _In_opt_ HWND      hWnd,
  _In_     UINT_PTR  nIDEvent,
  _In_     UINT      uElapse,
  _In_opt_ TIMERPROC lpTimerFunc
);
BOOL WINAPI KillTimer(
  _In_opt_ HWND     hWnd,
  _In_     UINT_PTR uIDEvent
);
BOOL WINAPI GetClientRect(
  _In_  HWND   hWnd,
  _Out_ LPRECT lpRect
);
int MapWindowPoints(
  _In_    HWND    hWndFrom,
  _In_    HWND    hWndTo,
  _Inout_ LPPOINT lpPoints,
  _In_    UINT    cPoints
);
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
);
HGDIOBJ GetStockObject(
  _In_ int fnObject
);
UINT IsDlgButtonChecked(
  _In_ HWND hDlg,
  _In_ int  nIDButton
);
BOOL RedrawWindow(
  _In_       HWND hWnd,
  _In_ const RECT *lprcUpdate,
  _In_       HRGN hrgnUpdate,
  _In_       UINT flags
);
HGLOBAL GlobalAlloc(
    UINT uFlags,
    DWORD dwBytes
    );
LPVOID GlobalLock(
    HGLOBAL hMem
    );
BOOL GlobalUnlock(
    HGLOBAL hMem
    );
BOOL OpenClipboard(
    HWND hWndNewOwner
	);
BOOL EmptyClipboard(
	);
HANDLE SetClipboardData(
    UINT uFormat,
    HANDLE hMem
	);
BOOL CloseClipboard(
	);
BOOL CheckDlgButton(
  _In_ HWND hDlg,
  _In_ int  nIDButton,
  _In_ UINT uCheck
);
BOOL WINAPI SetDlgItemText(
  _In_ HWND    hDlg,
  _In_ int     nIDDlgItem,
  _In_ LPCTSTR lpString
);
BOOL ScreenToClient(
  _In_ HWND    hWnd,
       LPPOINT lpPoint
);
BOOL ClientToScreen(
  _In_    HWND    hWnd,
  _Inout_ LPPOINT lpPoint
);
BOOL WINAPI MoveWindow(
  _In_ HWND hWnd,
  _In_ int  X,
  _In_ int  Y,
  _In_ int  nWidth,
  _In_ int  nHeight,
  _In_ BOOL bRepaint
);
UINT WINAPI GetDlgItemInt(
  _In_      HWND hDlg,
  _In_      int  nIDDlgItem,
  _Out_opt_ BOOL *lpTranslated,
  _In_      BOOL bSigned
);
BOOL WINAPI SetDlgItemInt(
  _In_ HWND hDlg,
  _In_ int  nIDDlgItem,
  _In_ UINT uValue,
  _In_ BOOL bSigned
);
BOOL WINAPI GetOpenFileName(
  _Inout_ LPOPENFILENAME lpofn
);
HWND WINAPI SetFocus(
  _In_opt_ HWND hWnd
);
BOOL WINAPI EnableWindow(
  _In_ HWND hWnd,
  _In_ BOOL bEnable
);
char * itoa(
		int value,
		char * str,
		int base
);
int WINAPI SetBkMode(HDC,int);
BOOL ComboBox_ShowDropdown(
   HWND hwndCtl,
   BOOL fShow
);
LRESULT WINAPI SendDlgItemMessage(
  _In_ HWND   hDlg,
  _In_ int    nIDDlgItem,
  _In_ UINT   Msg,
  _In_ WPARAM wParam,
  _In_ LPARAM lParam
);
BOOL InvalidateRect(
  _In_       HWND hWnd,
  _In_ const RECT *lpRect,
  _In_       BOOL bErase
);
BOOL UpdateWindow(
  _In_ HWND hWnd
);

#endif /* LINDOWS_H_ */

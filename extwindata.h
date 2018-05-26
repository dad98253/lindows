#ifndef _extwindata_h_
#define _extwindata_h_


#ifndef EXTWINDATAMAIN
#define EXTERN		extern
#define INITIZERO
#define INITSZERO
#define INITBOOLFALSE
#define INITBOOLTRUE
#define INITNULL
#define INITNEGDONE
#else
#define EXTERN
#define INITIZERO	=0
#define INITSZERO	={0}
#define INITBOOLFALSE	=false
#define INITBOOLTRUE	=true
#define INITNULL	=NULL
#define INITNEGDONE	=-1
#endif


EXTERN int numStringItems;

typedef struct {
  int id;
  const char *string;
} WindowsStringDataTYPE;

EXTERN WindowsStringDataTYPE StringResourceData[];

EXTERN char Comments[];
EXTERN char CompanyName[];
EXTERN char FileDescription[];
EXTERN char FileVersion[];
EXTERN char InternalName[];
EXTERN char LegalCopyright[];
EXTERN char LegalTrademarks[];
EXTERN char OriginalFilename[];
EXTERN char PrivateBuild[];
EXTERN char ProductName[];
EXTERN char ProductVersion[];
EXTERN char SpecialBuild[];
EXTERN char Translation[];

#endif

#ifndef __AC_STD__
#define __AC_STD__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>

#pragma warning(disable : 4996)

#ifndef NULLP
#define NULLP ((PTR) 0)
#endif

#ifndef NULLC
#define NULLC '\0'
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef PTR
#define PTR void*
#endif

#ifndef __PFI__
#define __PFI__
typedef int (*PFI)();
#endif

#ifndef __AC_MEM__
#define __AC_MEM__
#define mem_get(size)           calloc(size, 1)
#define mem_free(ptr)           free(ptr); 
#define mem_reset(ptr, size)        memset(ptr, '\0', size);
#define mem_copy(src, desc, size)   memcpy(desc, src, size);
#endif

#ifndef iseqstr
#define iseqstr(a,b)            (!strcmp(a,b))
#endif

#ifndef st_dup
#define st_dup(a)           strdup(a)
#endif

#endif

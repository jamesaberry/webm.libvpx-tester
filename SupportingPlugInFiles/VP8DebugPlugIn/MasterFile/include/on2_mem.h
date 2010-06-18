/*
//==========================================================================
//
//  Copyright (c) On2 Technologies Inc. All Rights Reserved.
//
//--------------------------------------------------------------------------
//
//  File:        $Workfile$
//               $Revision$
//
//  Last Update: $DateUTC$
//
//--------------------------------------------------------------------------
*/
#ifndef __ON2_MEM_H__
#define __ON2_MEM_H__

#if defined(__uClinux__)
# include <lddk.h>
#endif

/* on2_mem version info */
#define on2_mem_version "2.2.1.5"

#define ON2_MEM_VERSION_CHIEF 2
#define ON2_MEM_VERSION_MAJOR 2
#define ON2_MEM_VERSION_MINOR 1
#define ON2_MEM_VERSION_PATCH 5
/* end - on2_mem version info */

#ifndef ON2_TRACK_MEM_USAGE
# define ON2_TRACK_MEM_USAGE       0  //enable memory tracking/integrity checks
#endif
#ifndef ON2_CHECK_MEM_FUNCTIONS
# define ON2_CHECK_MEM_FUNCTIONS   0  //enable basic safety checks in _memcpy,
//_memset, and _memmove
#endif
#ifndef REPLACE_BUILTIN_FUNCTIONS
# define REPLACE_BUILTIN_FUNCTIONS 0  //replace builtin functions with their
//on2_ equivalents
#endif

#include <stdlib.h>
#include <stddef.h>

#if defined(__cplusplus)
extern "C" {
#endif

    /*
        on2_mem_get_version()
        provided for runtime version checking. Returns an unsigned int of the form
        CHIEF | MAJOR | MINOR | PATCH, where the chief version number is the high
        order byte.
    */
    unsigned int on2_mem_get_version(void);

    /*
        on2_mem_set_heap_size(size_t size)
          size - size in bytes for the memory manager to allocate for its heap
        Sets the memory manager's initial heap size
        Return:
          0: on success
          -1: if memory manager calls have not been included in the on2_mem lib
          -2: if the memory manager has been compiled to use static memory
          -3: if the memory manager has already allocated its heap
    */
    int on2_mem_set_heap_size(size_t size);

    void *on2_memalign(size_t align, size_t size);
    void *on2_malloc(size_t size);
    void *on2_calloc(size_t num, size_t size);
    void *on2_realloc(void *memblk, size_t size);
    void on2_free(void *memblk);

    void *on2_memcpy(void *dest, const void *src, size_t length);
    void *on2_memset(void *dest, int val, size_t length);
    void *on2_memmove(void *dest, const void *src, size_t count);

// special memory functions
    void *on2_mem_alloc(int id, size_t size, size_t align);
    void on2_mem_free(int id, void *mem, size_t size);

    /* Wrappers to standard library functions. */
    typedef void*(* g_malloc_func)(size_t);
    typedef void*(* g_calloc_func)(size_t, size_t);
    typedef void*(* g_realloc_func)(void *, size_t);
    typedef void (* g_free_func)(void *);
    typedef void*(* g_memcpy_func)(void *, const void *, size_t);
    typedef void*(* g_memset_func)(void *, int, size_t);
    typedef void*(* g_memmove_func)(void *, const void *, size_t);

    int on2_mem_set_functions(g_malloc_func g_malloc_l
                              , g_calloc_func g_calloc_l
                              , g_realloc_func g_realloc_l
                              , g_free_func g_free_l
                              , g_memcpy_func g_memcpy_l
                              , g_memset_func g_memset_l
                              , g_memmove_func g_memmove_l);
    int on2_mem_unset_functions(void);


    /* some defines for backward compatibility */
#define DMEM_GENERAL 0

#define duck_memalign(X,Y,Z) on2_memalign(X,Y)
#define duck_malloc(X,Y) on2_malloc(X)
#define duck_calloc(X,Y,Z) on2_calloc(X,Y)
#define duck_realloc  on2_realloc
#define duck_free     on2_free
#define duck_memcpy   on2_memcpy
#define duck_memmove  on2_memmove
#define duck_memset   on2_memset

#if REPLACE_BUILTIN_FUNCTIONS
# ifndef __ON2_MEM_C__
#  define memalign on2_memalign
#  define malloc   on2_malloc
#  define calloc   on2_calloc
#  define realloc  on2_realloc
#  define free     on2_free
#  define memcpy   on2_memcpy
#  define memmove  on2_memmove
#  define memset   on2_memset
# endif
#endif

#if ON2_TRACK_MEM_USAGE
#include <stdarg.h>
    /*from on2_mem/on2_mem_tracker.c*/
    extern void on2_MemoryTrackerDump();
    extern void on2_MemoryTrackerCheckIntegrity(char *file, unsigned int line);
    extern int on2_MemoryTrackerSetLogType(int type, char *option);
    extern int on2_MemoryTrackerSetLogFunc(void *userdata,
                                           void(*logfunc)(void *userdata,
                                                   const char *fmt, va_list args));
# ifndef __ON2_MEM_C__
#  define on2_memalign(align, size) xon2_memalign((align), (size), __FILE__, __LINE__)
#  define on2_malloc(size)          xon2_malloc((size), __FILE__, __LINE__)
#  define on2_calloc(num, size)     xon2_calloc(num, size, __FILE__, __LINE__)
#  define on2_realloc(addr, size)   xon2_realloc(addr, size, __FILE__, __LINE__)
#  define on2_free(addr)            xon2_free(addr, __FILE__, __LINE__)
#  define on2_MemoryTrackerCheckIntegrity() on2_MemoryTrackerCheckIntegrity(__FILE__, __LINE__)
#  define on2_mem_alloc(id,size,align) xon2_mem_alloc(id, size, align, __FILE__, __LINE__)
#  define on2_mem_free(id,mem,size) xon2_mem_free(id, mem, size, __FILE__, __LINE__)
# endif

    void *xon2_memalign(size_t align, size_t size, char *file, int line);
    void *xon2_malloc(size_t size, char *file, int line);
    void *xon2_calloc(size_t num, size_t size, char *file, int line);
    void *xon2_realloc(void *memblk, size_t size, char *file, int line);
    void xon2_free(void *memblk, char *file, int line);
    void *xon2_mem_alloc(int id, size_t size, size_t align, char *file, int line);
    void xon2_mem_free(int id, void *mem, size_t size, char *file, int line);

#else
# ifndef __ON2_MEM_C__
#  define on2_MemoryTrackerDump()
#  define on2_MemoryTrackerCheckIntegrity()
#  define on2_MemoryTrackerSetLogType(t,o) 0
#  define on2_MemoryTrackerSetLogFunc(u,f) 0
# endif
#endif

#if !ON2_CHECK_MEM_FUNCTIONS
# ifndef __ON2_MEM_C__
#  include <string.h>
#  define on2_memcpy  memcpy
#  define on2_memset  memset
#  define on2_memmove memmove
# endif
#endif

#ifdef ON2_MEM_PLTFRM
# include ON2_MEM_PLTFRM
#endif

#if defined(__cplusplus)
}
#endif

#endif /* __ON2_MEM_H__ */

/*
 * stdio_ext.h
 *
 * Definitions for I/O internal operations, originally from Solaris.
 */

#ifndef _STDIO_EXT_H_
#define _STDIO_EXT_H_

#ifdef __rtems__
#error "<stdio_ext.h> not supported"
#endif

#include <stdio.h>

#define	FSETLOCKING_QUERY	0
#define	FSETLOCKING_INTERNAL	1
#define	FSETLOCKING_BYCALLER	2

void	 __fpurge (FILE *);
int	 __fsetlocking (FILE *, int);

/* TODO:

   void _flushlbf (void);
*/

static __inline__ size_t
__fbufsize (FILE *__fp) { return (size_t) __fp->_bf._size; }

static __inline__ int
__freading (FILE *__fp) { return (__fp->_flags & __SRD) != 0; }

static __inline__ int
__fwriting (FILE *__fp) { return (__fp->_flags & __SWR) != 0; }

static __inline__ int
__freadable (FILE *__fp) { return (__fp->_flags & (__SRD | __SRW)) != 0; }

static __inline__ int
__fwritable (FILE *__fp) { return (__fp->_flags & (__SWR | __SRW)) != 0; }

static __inline__ int
__flbf (FILE *__fp) { return (__fp->_flags & __SLBF) != 0; }

static __inline__ size_t
__fpending (FILE *__fp) { return __fp->_p - __fp->_bf._base; }

#endif /* _STDIO_EXT_H_ */

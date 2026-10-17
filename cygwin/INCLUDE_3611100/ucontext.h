/* ucontext.h

This file is part of Cygwin.

This software is a copyrighted work licensed under the terms of the
Cygwin license.  Please consult the file "CYGWIN_LICENSE" for
details. */

#ifndef _UCONTEXT_H
#define _UCONTEXT_H

#include <sys/cdefs.h>
#include <sys/ucontext.h>

extern int getcontext (ucontext_t *);
extern int setcontext (const ucontext_t *);
extern int swapcontext (ucontext_t *, const ucontext_t *);
extern void makecontext (ucontext_t *, void (*) (void), int, ...);

#endif /* _UCONTEXT_H */

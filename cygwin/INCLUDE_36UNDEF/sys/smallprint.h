/* sys/smallprint.h

This file is part of Cygwin.

This software is a copyrighted work licensed under the terms of the
Cygwin license.  Please consult the file "CYGWIN_LICENSE" for
details. */

#ifndef _SYS_SMALLPRINT_H
#define _SYS_SMALLPRINT_H

#include <stdarg.h>

int __small_sprintf (char *__dst, const char *__fmt, ...);
int __small_vsprintf (char *__dst, const char *__fmt, va_list __ap);

#endif /* _SYS_SMALLPRINT_H */

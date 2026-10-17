/* sys/utime.h

   This software is a copyrighted work licensed under the terms of the
   Cygwin license.  Please consult the file "CYGWIN_LICENSE" for
   details. */

#ifndef _SYS_UTIME_H
#define _SYS_UTIME_H

#include <sys/types.h>

struct utimbuf
{
  time_t actime;
  time_t modtime;
};

int utime (const char *__path, const struct utimbuf *__buf);

#endif /* _SYS_UTIME_H */

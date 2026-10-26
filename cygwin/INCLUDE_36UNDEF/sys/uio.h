/* sys/uio.h

This file is part of Cygwin.

This software is a copyrighted work licensed under the terms of the
Cygwin license.  Please consult the file "CYGWIN_LICENSE" for
details. */

#ifndef _UIO_H_
#define _UIO_H_

/* For size_t */
#include <stddef.h>
/* For ssize_t */
#include <sys/types.h>

#include <sys/cdefs.h>

/*
 * Define the uio buffers used for writev, readv.
 */

struct iovec
{
  void *iov_base;
  size_t iov_len;
};

extern ssize_t readv (int filedes, const struct iovec *vector, int count);
extern ssize_t writev (int filedes, const struct iovec *vector, int count);

#endif /* _UIO_H_ */

#ifndef __PTY_H__
#define __PTY_H__

#include <sys/termios.h>

int openpty (int *, int *, char *, const struct termios *,
		     const struct winsize *);
int forkpty (int *, char *, const struct termios *,
		     const struct winsize *);

#endif /* __PTY_H__ */

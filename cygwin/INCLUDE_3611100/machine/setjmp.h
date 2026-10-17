#ifdef __x86_64__
# ifdef __CYGWIN__
#  define _JBTYPE long
#  define _JBLEN  32
# else
#  define _JBTYPE long long
#  define _JBLEN  8
# endif
#endif

#ifdef _JBLEN
#ifdef _JBTYPE
typedef	_JBTYPE jmp_buf[_JBLEN];
#else
typedef	int jmp_buf[_JBLEN];
#endif
#endif

#if (defined(__CYGWIN__) || defined(__rtems__)) && __POSIX_VISIBLE
#include <signal.h>

/* POSIX sigsetjmp/siglongjmp macros */
#ifdef _JBTYPE
typedef _JBTYPE sigjmp_buf[_JBLEN+1+((sizeof (_JBTYPE) + sizeof (sigset_t) - 1)
				     /sizeof (_JBTYPE))];
#else
typedef int sigjmp_buf[_JBLEN+1+(sizeof (sigset_t)/sizeof (int))];
#endif

#define _SAVEMASK	_JBLEN
#define _SIGMASK	(_JBLEN+1)

#ifdef __CYGWIN__
# define _CYGWIN_WORKING_SIGSETJMP
#endif

#ifdef _POSIX_THREADS
#define __SIGMASK_FUNC pthread_sigmask
#else
#define __SIGMASK_FUNC sigprocmask
#endif

#ifdef __CYGWIN__
/* Per POSIX, siglongjmp has to be implemented as function.  Cygwin
   provides functions for both, siglongjmp and sigsetjmp since 2.2.0. */
extern void siglongjmp (sigjmp_buf, int) __attribute__((__noreturn__));
extern int sigsetjmp (sigjmp_buf, int);
#endif

#define sigsetjmp(env, savemask) ((env)[_SAVEMASK] = savemask,\
               __SIGMASK_FUNC (SIG_SETMASK, 0, (sigset_t *) ((env) + _SIGMASK)),\
               setjmp (env))

#define siglongjmp(env, val) ((((env)[_SAVEMASK])?\
               __SIGMASK_FUNC (SIG_SETMASK, (sigset_t *) ((env) + _SIGMASK), 0):0),\
               longjmp (env, val))

/* POSIX _setjmp/_longjmp, maintained for XSI compatibility.  These
   are equivalent to sigsetjmp/siglongjmp when not saving the signal mask.
   New applications should use sigsetjmp/siglongjmp instead. */
#ifdef __CYGWIN__
extern void _longjmp (jmp_buf, int) __attribute__((__noreturn__));
extern int _setjmp (jmp_buf);
#else
#define _setjmp(env)		sigsetjmp ((env), 0)
#define _longjmp(env, val)	siglongjmp ((env), (val))
#endif

#endif /* (__CYGWIN__ or __rtems__) and __POSIX_VISIBLE */

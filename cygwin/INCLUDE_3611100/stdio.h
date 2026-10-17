/*
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * and/or other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 *	@(#)stdio.h	5.3 (Berkeley) 3/15/86
 */

/*
 * NB: to fit things in six character monocase externals, the
 * stdio code uses the prefix `__s' for stdio objects, typically
 * followed by a three-character attempt at a mnemonic.
 */

#ifndef _STDIO_H_
#define	_STDIO_H_

#define	_FSTDIO			/* ``function stdio'' */

#define __need_size_t
#define __need_NULL
#include <sys/cdefs.h>
#include <stddef.h>

/* typedef only __gnuc_va_list, used throughout the header */
#define __need___va_list
#include <stdarg.h>

/*
 * <sys/reent.h> defines __FILE, _fpos_t.
 * They must be defined there because struct _reent needs them (and we don't
 * want reent.h to include this file.
 */

#include <sys/reent.h>
#include <sys/_types.h>

#if !defined(__FILE_defined)
typedef __FILE FILE;
# define __FILE_defined
#endif

typedef _fpos_t fpos_t;
#ifdef __LARGE64_FILES
typedef _fpos64_t fpos64_t;
#endif

#ifndef _OFF_T_DECLARED
typedef __off_t off_t;
#define	_OFF_T_DECLARED
#endif

#ifndef _SSIZE_T_DECLARED
typedef _ssize_t ssize_t;
#define	_SSIZE_T_DECLARED
#endif

#include <sys/stdio.h>

#define	__SLBF	0x0001		/* line buffered */
#define	__SNBF	0x0002		/* unbuffered */
#define	__SRD	0x0004		/* OK to read */
#define	__SWR	0x0008		/* OK to write */
	/* RD and WR are never simultaneously asserted */
#define	__SRW	0x0010		/* open for reading & writing */
#define	__SEOF	0x0020		/* found EOF */
#define	__SERR	0x0040		/* found error */
#define	__SMBF	0x0080		/* _buf is from malloc */
#define	__SAPP	0x0100		/* fdopen()ed in append mode - so must  write to end */
#define	__SSTR	0x0200		/* this is an sprintf/snprintf string */
#define	__SOPT	0x0400		/* do fseek() optimisation */
#define	__SNPT	0x0800		/* do not do fseek() optimisation */
#define	__SOFF	0x1000		/* set iff _offset is in fact correct */
#define	__SORD	0x2000		/* true => stream orientation (byte/wide) decided */
#define	__SL64	0x8000		/* is 64-bit offset large file */

/* _flags2 flags */
#define	__SNLK  0x0001		/* stdio functions do not lock streams themselves */
#define	__SWID	0x2000		/* true => stream orientation wide, false => byte, only valid if __SORD in _flags is true */

/*
 * The following three definitions are for ANSI C, which took them
 * from System V, which stupidly took internal interface macros and
 * made them official arguments to setvbuf(), without renaming them.
 * Hence, these ugly _IOxxx names are *supposed* to appear in user code.
 *
 * Although these happen to match their counterparts above, the
 * implementation does not rely on that (so these could be renumbered).
 */
#define	_IOFBF	0		/* setvbuf should set fully buffered */
#define	_IOLBF	1		/* setvbuf should set line buffered */
#define	_IONBF	2		/* setvbuf should set unbuffered */

#define	EOF	(-1)

#ifdef __BUFSIZ__
#define	BUFSIZ		__BUFSIZ__
#else
#define	BUFSIZ		1024
#endif

#ifdef __FOPEN_MAX__
#define FOPEN_MAX	__FOPEN_MAX__
#else
#define	FOPEN_MAX	20
#endif

#ifdef __FILENAME_MAX__
#define FILENAME_MAX    __FILENAME_MAX__
#else
#define	FILENAME_MAX	1024
#endif

#ifdef __L_tmpnam__
#define L_tmpnam	__L_tmpnam__
#else
#define	L_tmpnam	FILENAME_MAX
#endif

#if __BSD_VISIBLE || __XSI_VISIBLE
#define P_tmpdir        "/tmp"
#endif

#ifndef SEEK_SET
#define	SEEK_SET	0	/* set file offset to offset */
#endif
#ifndef SEEK_CUR
#define	SEEK_CUR	1	/* set file offset to current plus offset */
#endif
#ifndef SEEK_END
#define	SEEK_END	2	/* set file offset to EOF plus offset */
#endif

#define	TMP_MAX		26

#define	stdin	_REENT_STDIN(_REENT)
#define	stdout	_REENT_STDOUT(_REENT)
#define	stderr	_REENT_STDERR(_REENT)

#define _stdin_r(x)	_REENT_STDIN(x)
#define _stdout_r(x)	_REENT_STDOUT(x)
#define _stderr_r(x)	_REENT_STDERR(x)

/*
 * Functions defined in ANSI C standard.
 */

#define __VALIST __gnuc_va_list

#if __POSIX_VISIBLE
char *	ctermid (char *);
#endif
#if __GNU_VISIBLE || (__XSI_VISIBLE && __XSI_VISIBLE < 600)
char *	cuserid (char *);
#endif
FILE *	tmpfile (void);
char *	tmpnam (char *);
#if __BSD_VISIBLE || __XSI_VISIBLE || __POSIX_VISIBLE >= 200112
char *	tempnam (const char *, const char *);
#endif
int	fclose (FILE *);
int	fflush (FILE *);
FILE *	freopen (const char *restrict, const char *restrict, FILE *restrict);
void	setbuf (FILE *restrict, char *restrict);
int	setvbuf (FILE *restrict, char *restrict, int, size_t);
int	fprintf (FILE *restrict, const char *restrict, ...);
int	fscanf (FILE *restrict, const char *restrict, ...);
int	printf (const char *restrict, ...);
int	scanf (const char *restrict, ...);
int	sscanf (const char *restrict, const char *restrict, ...);
int	vfprintf (FILE *restrict, const char *restrict, __VALIST);
int	vprintf (const char *, __VALIST);
int	vsprintf (char *restrict, const char *restrict, __VALIST);
int	fgetc (FILE *);
char *  fgets (char *restrict, int, FILE *restrict);
int	fputc (int, FILE *);
int	fputs (const char *restrict, FILE *restrict);
int	getc (FILE *);
int	getchar (void);
char *  gets (char *);
int	putc (int, FILE *);
int	putchar (int);
int	puts (const char *);
int	ungetc (int, FILE *);
size_t	fread (void *restrict, size_t _size, size_t _n, FILE *restrict);
size_t	fwrite (const void *restrict , size_t _size, size_t _n, FILE *);
#ifdef _LIBC
int	fgetpos (FILE *, _fpos_t *);
#else
int	fgetpos (FILE *restrict, fpos_t *restrict);
#endif
int	fseek (FILE *, long, int);
#ifdef _LIBC
int	fsetpos (FILE *, const _fpos_t *);
#else
int	fsetpos (FILE *, const fpos_t *);
#endif
long	ftell ( FILE *);
void	rewind (FILE *);
void	clearerr (FILE *);
int	feof (FILE *);
int	ferror (FILE *);
void    perror (const char *);
#ifndef _REENT_ONLY
FILE *	fopen (const char *restrict _name, const char *restrict _type);
int	sprintf (char *restrict, const char *restrict, ...);
int	remove (const char *);
int	rename (const char *, const char *);
#ifdef _LIBC
int	_rename (const char *, const char *);
#endif
#endif
#if __LARGEFILE_VISIBLE || __POSIX_VISIBLE >= 200112
#ifdef _LIBC
int	fseeko (FILE *, _off_t, int);
_off_t	ftello (FILE *);
#else
int	fseeko (FILE *, off_t, int);
off_t	ftello (FILE *);
#endif
#endif
#if __GNU_VISIBLE
int	fcloseall (void);
#endif
#ifndef _REENT_ONLY
#if __ISO_C_VISIBLE >= 1999
int	snprintf (char *restrict, size_t, const char *restrict, ...);
int	vsnprintf (char *restrict, size_t, const char *restrict, __VALIST);
int	vfscanf (FILE *restrict, const char *restrict, __VALIST);
int	vscanf (const char *, __VALIST);
int	vsscanf (const char *restrict, const char *restrict, __VALIST);
#endif
#if __GNU_VISIBLE
int	asprintf (char **restrict, const char *restrict, ...);
int	vasprintf (char **, const char *, __VALIST);
#endif
#if __MISC_VISIBLE /* Newlib-specific */
int	asiprintf (char **, const char *, ...);
char *	asniprintf (char *, size_t *, const char *, ...);
char *	asnprintf (char *restrict, size_t *restrict, const char *restrict, ...);
#ifndef diprintf
int	diprintf (int, const char *, ...);
#endif
int	fiprintf (FILE *, const char *, ...);
int	fiscanf (FILE *, const char *, ...);
int	iprintf (const char *, ...);
int	iscanf (const char *, ...);
int	siprintf (char *, const char *, ...);
int	siscanf (const char *, const char *, ...);
int	sniprintf (char *, size_t, const char *, ...);
int	vasiprintf (char **, const char *, __VALIST);
char *	vasniprintf (char *, size_t *, const char *, __VALIST);
char *	vasnprintf (char *, size_t *, const char *, __VALIST);
int	vdiprintf (int, const char *, __VALIST);
int	vfiprintf (FILE *, const char *, __VALIST);
int	vfiscanf (FILE *, const char *, __VALIST);
int	viprintf (const char *, __VALIST);
int	viscanf (const char *, __VALIST);
int	vsiprintf (char *, const char *, __VALIST);
int	vsiscanf (const char *, const char *, __VALIST);
int	vsniprintf (char *, size_t, const char *, __VALIST);
#endif /* __MISC_VISIBLE */
#endif /* !_REENT_ONLY */

/*
 * Routines in POSIX 1003.1:2001.
 */

#if __POSIX_VISIBLE
#ifndef _REENT_ONLY
FILE *	fdopen (int, const char *);
#endif
int	fileno (FILE *);
#endif
#if __MISC_VISIBLE || __POSIX_VISIBLE >= 199209
int	pclose (FILE *);
FILE *  popen (const char *, const char *);
#endif

#if __BSD_VISIBLE
void    setbuffer (FILE *, char *, int);
int	setlinebuf (FILE *);
#endif

#if __MISC_VISIBLE || (__XSI_VISIBLE && __POSIX_VISIBLE < 200112)
int	getw (FILE *);
int	putw (int, FILE *);
#endif
#if __MISC_VISIBLE || __POSIX_VISIBLE
int	getc_unlocked (FILE *);
int	getchar_unlocked (void);
void	flockfile (FILE *);
int	ftrylockfile (FILE *);
void	funlockfile (FILE *);
int	putc_unlocked (int, FILE *);
int	putchar_unlocked (int);
#endif

/*
 * Routines in POSIX 1003.1:200x.
 */

#if __POSIX_VISIBLE >= 200809
# ifndef _REENT_ONLY
#  ifndef dprintf
int	dprintf (int, const char *restrict, ...);
#  endif
FILE *	fmemopen (void *restrict, size_t, const char *restrict);
/* getdelim - see __getdelim for now */
/* getline - see __getline for now */
FILE *	open_memstream (char **, size_t *);
int	vdprintf (int, const char *restrict, __VALIST);
# endif
#endif
#if __ATFILE_VISIBLE
int	renameat (int, const char *, int, const char *);
# ifdef __CYGWIN__
int	renameat2 (int, const char *, int, const char *, unsigned int);
# endif
#endif

/*
 * Recursive versions of the above.
 */

int	_asiprintf_r (struct _reent *, char **, const char *, ...);
char *	_asniprintf_r (struct _reent *, char *, size_t *, const char *, ...);
char *	_asnprintf_r (struct _reent *, char *restrict, size_t *restrict, const char *restrict, ...);
int	_asprintf_r (struct _reent *, char **restrict, const char *restrict, ...);
int	_diprintf_r (struct _reent *, int, const char *, ...);
int	_dprintf_r (struct _reent *, int, const char *restrict, ...);
int	_fclose_r (struct _reent *, FILE *);
int	_fcloseall_r (struct _reent *);
FILE *	_fdopen_r (struct _reent *, int, const char *);
int	_fflush_r (struct _reent *, FILE *);
int	_fgetc_r (struct _reent *, FILE *);
int	_fgetc_unlocked_r (struct _reent *, FILE *);
char *  _fgets_r (struct _reent *, char *restrict, int, FILE *restrict);
char *  _fgets_unlocked_r (struct _reent *, char *restrict, int, FILE *restrict);
#ifdef _LIBC
int	_fgetpos_r (struct _reent *, FILE *restrict, _fpos_t *restrict);
int	_fsetpos_r (struct _reent *, FILE *, const _fpos_t *);
#else
int	_fgetpos_r (struct _reent *, FILE *, fpos_t *);
int	_fsetpos_r (struct _reent *, FILE *, const fpos_t *);
#endif
int	_fiprintf_r (struct _reent *, FILE *, const char *, ...);
int	_fiscanf_r (struct _reent *, FILE *, const char *, ...);
FILE *	_fmemopen_r (struct _reent *, void *restrict, size_t, const char *restrict);
FILE *	_fopen_r (struct _reent *, const char *restrict, const char *restrict);
FILE *	_freopen_r (struct _reent *, const char *restrict, const char *restrict, FILE *restrict);
int	_fprintf_r (struct _reent *, FILE *restrict, const char *restrict, ...);
int	_fpurge_r (struct _reent *, FILE *);
int	_fputc_r (struct _reent *, int, FILE *);
int	_fputc_unlocked_r (struct _reent *, int, FILE *);
int	_fputs_r (struct _reent *, const char *restrict, FILE *restrict);
int	_fputs_unlocked_r (struct _reent *, const char *restrict, FILE *restrict);
size_t	_fread_r (struct _reent *, void *restrict, size_t _size, size_t _n, FILE *restrict);
size_t	_fread_unlocked_r (struct _reent *, void *restrict, size_t _size, size_t _n, FILE *restrict);
int	_fscanf_r (struct _reent *, FILE *restrict, const char *restrict, ...);
int	_fseek_r (struct _reent *, FILE *, long, int);
int	_fseeko_r (struct _reent *, FILE *, _off_t, int);
long	_ftell_r (struct _reent *, FILE *);
_off_t	_ftello_r (struct _reent *, FILE *);
void	_rewind_r (struct _reent *, FILE *);
size_t	_fwrite_r (struct _reent *, const void *restrict, size_t _size, size_t _n, FILE *restrict);
size_t	_fwrite_unlocked_r (struct _reent *, const void *restrict, size_t _size, size_t _n, FILE *restrict);
int	_getc_r (struct _reent *, FILE *);
int	_getc_unlocked_r (struct _reent *, FILE *);
int	_getchar_r (struct _reent *);
int	_getchar_unlocked_r (struct _reent *);
char *	_gets_r (struct _reent *, char *);
int	_iprintf_r (struct _reent *, const char *, ...);
int	_iscanf_r (struct _reent *, const char *, ...);
FILE *	_open_memstream_r (struct _reent *, char **, size_t *);
void	_perror_r (struct _reent *, const char *);
int	_printf_r (struct _reent *, const char *restrict, ...);
int	_putc_r (struct _reent *, int, FILE *);
int	_putc_unlocked_r (struct _reent *, int, FILE *);
int	_putchar_unlocked_r (struct _reent *, int);
int	_putchar_r (struct _reent *, int);
int	_puts_r (struct _reent *, const char *);
int	_remove_r (struct _reent *, const char *);
int	_rename_r (struct _reent *,
			   const char *_old, const char *_new);
int	_scanf_r (struct _reent *, const char *restrict, ...);
int	_siprintf_r (struct _reent *, char *, const char *, ...);
int	_siscanf_r (struct _reent *, const char *, const char *, ...);
int	_sniprintf_r (struct _reent *, char *, size_t, const char *, ...);
int	_snprintf_r (struct _reent *, char *restrict, size_t, const char *restrict, ...);
int	_sprintf_r (struct _reent *, char *restrict, const char *restrict, ...);
int	_sscanf_r (struct _reent *, const char *restrict, const char *restrict, ...);
char *	_tempnam_r (struct _reent *, const char *, const char *);
FILE *	_tmpfile_r (struct _reent *);
char *	_tmpnam_r (struct _reent *, char *);
int	_ungetc_r (struct _reent *, int, FILE *);
int	_vasiprintf_r (struct _reent *, char **, const char *, __VALIST);
char *	_vasniprintf_r (struct _reent*, char *, size_t *, const char *, __VALIST);
char *	_vasnprintf_r (struct _reent*, char *, size_t *, const char *, __VALIST);
int	_vasprintf_r (struct _reent *, char **, const char *, __VALIST);
int	_vdiprintf_r (struct _reent *, int, const char *, __VALIST);
int	_vdprintf_r (struct _reent *, int, const char *restrict, __VALIST);
int	_vfiprintf_r (struct _reent *, FILE *, const char *, __VALIST);
int	_vfiscanf_r (struct _reent *, FILE *, const char *, __VALIST);
int	_vfprintf_r (struct _reent *, FILE *restrict, const char *restrict, __VALIST);
int	_vfscanf_r (struct _reent *, FILE *restrict, const char *restrict, __VALIST);
int	_viprintf_r (struct _reent *, const char *, __VALIST);
int	_viscanf_r (struct _reent *, const char *, __VALIST);
int	_vprintf_r (struct _reent *, const char *restrict, __VALIST);
int	_vscanf_r (struct _reent *, const char *restrict, __VALIST);
int	_vsiprintf_r (struct _reent *, char *, const char *, __VALIST);
int	_vsiscanf_r (struct _reent *, const char *, const char *, __VALIST);
int	_vsniprintf_r (struct _reent *, char *, size_t, const char *, __VALIST);
int	_vsnprintf_r (struct _reent *, char *restrict, size_t, const char *restrict, __VALIST);
int	_vsprintf_r (struct _reent *, char *restrict, const char *restrict, __VALIST);
int	_vsscanf_r (struct _reent *, const char *restrict, const char *restrict, __VALIST);

/* Other extensions.  */

int	fpurge (FILE *);
ssize_t __getdelim (char **, size_t *, int, FILE *);
ssize_t __getline (char **, size_t *, FILE *);

#if __MISC_VISIBLE
void	clearerr_unlocked (FILE *);
int	feof_unlocked (FILE *);
int	ferror_unlocked (FILE *);
int	fileno_unlocked (FILE *);
int	fflush_unlocked (FILE *);
int	fgetc_unlocked (FILE *);
int	fputc_unlocked (int, FILE *);
size_t	fread_unlocked (void *restrict, size_t _size, size_t _n, FILE *restrict);
size_t	fwrite_unlocked (const void *restrict , size_t _size, size_t _n, FILE *);
#endif

#if __GNU_VISIBLE
char *  fgets_unlocked (char *restrict, int, FILE *restrict);
int	fputs_unlocked (const char *restrict, FILE *restrict);
#endif

#ifdef __LARGE64_FILES
#if !defined(__CYGWIN__) || defined(_LIBC)
FILE *	fdopen64 (int, const char *);
FILE *  fopen64 (const char *, const char *);
FILE *  freopen64 (const char *, const char *, FILE *);
_off64_t ftello64 (FILE *);
_off64_t fseeko64 (FILE *, _off64_t, int);
int     fgetpos64 (FILE *, _fpos64_t *);
int     fsetpos64 (FILE *, const _fpos64_t *);
FILE *  tmpfile64 (void);

FILE *	_fdopen64_r (struct _reent *, int, const char *);
FILE *  _fopen64_r (struct _reent *,const char *, const char *);
FILE *  _freopen64_r (struct _reent *, const char *, const char *, FILE *);
_off64_t _ftello64_r (struct _reent *, FILE *);
_off64_t _fseeko64_r (struct _reent *, FILE *, _off64_t, int);
int     _fgetpos64_r (struct _reent *, FILE *, _fpos64_t *);
int     _fsetpos64_r (struct _reent *, FILE *, const _fpos64_t *);
FILE *  _tmpfile64_r (struct _reent *);
#endif /* !__CYGWIN__ */
#endif /* __LARGE64_FILES */

/*
 * Routines internal to the implementation.
 */

int	__srget_r (struct _reent *, FILE *);
int	__swbuf_r (struct _reent *, int, FILE *);

/*
 * Stdio function-access interface.
 */

#if __BSD_VISIBLE
# ifdef __LARGE64_FILES
FILE	*funopen (const void *__cookie,
		int (*__readfn)(void *__c, char *__buf,
				_READ_WRITE_BUFSIZE_TYPE __n),
		int (*__writefn)(void *__c, const char *__buf,
				 _READ_WRITE_BUFSIZE_TYPE __n),
		_fpos64_t (*__seekfn)(void *__c, _fpos64_t __off, int __whence),
		int (*__closefn)(void *__c));
FILE	*_funopen_r (struct _reent *, const void *__cookie,
		int (*__readfn)(void *__c, char *__buf,
				_READ_WRITE_BUFSIZE_TYPE __n),
		int (*__writefn)(void *__c, const char *__buf,
				 _READ_WRITE_BUFSIZE_TYPE __n),
		_fpos64_t (*__seekfn)(void *__c, _fpos64_t __off, int __whence),
		int (*__closefn)(void *__c));
# else
FILE	*funopen (const void *__cookie,
		int (*__readfn)(void *__cookie, char *__buf,
				_READ_WRITE_BUFSIZE_TYPE __n),
		int (*__writefn)(void *__cookie, const char *__buf,
				 _READ_WRITE_BUFSIZE_TYPE __n),
		fpos_t (*__seekfn)(void *__cookie, fpos_t __off, int __whence),
		int (*__closefn)(void *__cookie));
FILE	*_funopen_r (struct _reent *, const void *__cookie,
		int (*__readfn)(void *__cookie, char *__buf,
				_READ_WRITE_BUFSIZE_TYPE __n),
		int (*__writefn)(void *__cookie, const char *__buf,
				 _READ_WRITE_BUFSIZE_TYPE __n),
		fpos_t (*__seekfn)(void *__cookie, fpos_t __off, int __whence),
		int (*__closefn)(void *__cookie));
# endif /* !__LARGE64_FILES */

# define	fropen(__cookie, __fn) funopen(__cookie, __fn, NULL, NULL, NULL)
# define	fwopen(__cookie, __fn) funopen(__cookie, NULL, __fn, NULL, NULL)
#endif /* __BSD_VISIBLE */

#if __GNU_VISIBLE
typedef ssize_t cookie_read_function_t(void *__cookie, char *__buf, size_t __n);
typedef ssize_t cookie_write_function_t(void *__cookie, const char *__buf,
					size_t __n);
# ifdef __LARGE64_FILES
typedef int cookie_seek_function_t(void *__cookie, _off64_t *__off,
				   int __whence);
# else
typedef int cookie_seek_function_t(void *__cookie, off_t *__off, int __whence);
# endif /* !__LARGE64_FILES */
typedef int cookie_close_function_t(void *__cookie);
typedef struct
{
  /* These four struct member names are dictated by Linux; hopefully,
     they don't conflict with any macros.  */
  cookie_read_function_t  *read;
  cookie_write_function_t *write;
  cookie_seek_function_t  *seek;
  cookie_close_function_t *close;
} cookie_io_functions_t;
FILE *fopencookie (void *__cookie,
		const char *__mode, cookie_io_functions_t __functions);
FILE *_fopencookie_r (struct _reent *, void *__cookie,
		const char *__mode, cookie_io_functions_t __functions);
#endif /* __GNU_VISIBLE */

#define	getchar()	getc(stdin)
#define	putchar(x)	putc(x, stdout)

#if __MISC_VISIBLE || __POSIX_VISIBLE
#define	getchar_unlocked()	getc_unlocked(stdin)
#define	putchar_unlocked(x)	putc_unlocked(x, stdout)
#endif

#if __SSP_FORTIFY_LEVEL > 0
#error "TCC does not support SSP (Stack Smashing Protector)."
#endif

#endif /* _STDIO_H_ */

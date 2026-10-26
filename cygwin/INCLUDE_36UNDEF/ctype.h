#ifndef _CTYPE_H_
#define _CTYPE_H_

#include <sys/cdefs.h>

#if __POSIX_VISIBLE >= 200809 || __MISC_VISIBLE || defined (_LIBC)
#include <sys/_locale.h>
#endif

int isalnum (int __c);
int isalpha (int __c);
int iscntrl (int __c);
int isdigit (int __c);
int isgraph (int __c);
int islower (int __c);
int isprint (int __c);
int ispunct (int __c);
int isspace (int __c);
int isupper (int __c);
int isxdigit (int __c);
int tolower (int __c);
int toupper (int __c);

#if __ISO_C_VISIBLE >= 1999
int isblank (int __c);
#endif

#if __MISC_VISIBLE || __XSI_VISIBLE
int isascii (int __c);
int toascii (int __c);
#define _tolower(__c) ((unsigned char)(__c) - 'A' + 'a')
#define _toupper(__c) ((unsigned char)(__c) - 'a' + 'A')
#endif

#if __POSIX_VISIBLE >= 200809
extern int isalnum_l (int __c, locale_t __l);
extern int isalpha_l (int __c, locale_t __l);
extern int isblank_l (int __c, locale_t __l);
extern int iscntrl_l (int __c, locale_t __l);
extern int isdigit_l (int __c, locale_t __l);
extern int isgraph_l (int __c, locale_t __l);
extern int islower_l (int __c, locale_t __l);
extern int isprint_l (int __c, locale_t __l);
extern int ispunct_l (int __c, locale_t __l);
extern int isspace_l (int __c, locale_t __l);
extern int isupper_l (int __c, locale_t __l);
extern int isxdigit_l(int __c, locale_t __l);
extern int tolower_l (int __c, locale_t __l);
extern int toupper_l (int __c, locale_t __l);
#endif

#if __MISC_VISIBLE
extern int isascii_l (int __c, locale_t __l);
extern int toascii_l (int __c, locale_t __l);
#endif

#endif /* _CTYPE_H_ */

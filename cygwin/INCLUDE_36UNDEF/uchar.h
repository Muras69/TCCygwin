#ifndef _UCHAR_H
#define	_UCHAR_H

#include <sys/cdefs.h>
#include <wchar.h>

/* Either C23 or if C++ doesn't already define char8_t */
#if __ISO_C_VISIBLE >= 2023 && !defined (__cpp_char8_t)
typedef unsigned char		char8_t;
#endif

typedef	__char16_t	char16_t;
typedef	__char32_t	char32_t;

/* Either C23 or if C++ defines char8_t */
#if __ISO_C_VISIBLE >= 2023 || defined (__cpp_char8_t)
size_t  c8rtomb(char * restrict, char8_t, mbstate_t * restrict);
size_t	mbrtoc8(char8_t * restrict, const char * restrict, size_t,
		mbstate_t * restrict);
#endif

size_t	c16rtomb(char * restrict, char16_t, mbstate_t * restrict);
size_t	mbrtoc16(char16_t * restrict, const char * restrict, size_t,
		 mbstate_t * restrict);

size_t	c32rtomb(char * restrict, char32_t, mbstate_t * restrict);
size_t	mbrtoc32(char32_t * restrict, const char * restrict, size_t,
		 mbstate_t * restrict);

#endif /* _UCHAR_H */

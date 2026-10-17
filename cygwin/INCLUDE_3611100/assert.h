/*
	assert.h
*/

#undef assert

#ifdef NDEBUG           /* required by ANSI standard */
# define assert(__e) ((void)0)
#else
# define assert(__e) ((__e) ? (void)0 : __assert_func (__FILE__, __LINE__, \
						       __func__, #__e))
#endif

void __assert_func (const char *, int, const char *, const char *)
	    __attribute__((__noreturn__));

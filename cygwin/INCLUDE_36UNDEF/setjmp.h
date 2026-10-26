/*
	setjmp.h
	stubs for future use.
*/

#ifndef _SETJMP_H_
#define _SETJMP_H_

#include <machine/setjmp.h>

void	longjmp (jmp_buf __jmpb, int __retval)
			__attribute__((__noreturn__));

int	setjmp (jmp_buf __jmpb);

#endif /* _SETJMP_H_ */

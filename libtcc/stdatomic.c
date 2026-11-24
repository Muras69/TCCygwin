typedef unsigned long long size_t;

unsigned char __atomic_exchange_1(volatile void *atom, unsigned char value, int memorder)
{
 unsigned char xchg, cmp;
 __atomic_load((unsigned char *)atom, (unsigned char *)&cmp, 0);
do
{
	xchg = (value);
} 
while (!__atomic_compare_exchange((unsigned char *)atom, &cmp, &xchg, 1, 5, 5));
	return cmp;
} 

unsigned char __atomic_add_fetch_1(volatile void *atom, unsigned char value, int memorder)
{
 unsigned char xchg, cmp;
 __atomic_load((unsigned char *)atom, (unsigned char *)&cmp, 0);
do
{
	xchg = ((cmp + value));
} 
while (!__atomic_compare_exchange((unsigned char *)atom, &cmp, &xchg, 1, 5, 5));
	return xchg;
} 

unsigned char __atomic_sub_fetch_1(volatile void *atom, unsigned char value, int memorder)
{
 unsigned char xchg, cmp;
 __atomic_load((unsigned char *)atom, (unsigned char *)&cmp, 0);
do
{
	xchg = ((cmp - value));
} 
while (!__atomic_compare_exchange((unsigned char *)atom, &cmp, &xchg, 1, 5, 5));
	return xchg;
} 

unsigned char __atomic_and_fetch_1(volatile void *atom, unsigned char value, int memorder)
{
 unsigned char xchg, cmp;
 __atomic_load((unsigned char *)atom, (unsigned char *)&cmp, 0);
do
{
	xchg = ((cmp & value));
} 
while (!__atomic_compare_exchange((unsigned char *)atom, &cmp, &xchg, 1, 5, 5));
	return xchg;
} 

unsigned char __atomic_or_fetch_1(volatile void *atom, unsigned char value, int memorder)
{
 unsigned char xchg, cmp;
 __atomic_load((unsigned char *)atom, (unsigned char *)&cmp, 0);
do
{
	xchg = ((cmp | value));
} 
while (!__atomic_compare_exchange((unsigned char *)atom, &cmp, &xchg, 1, 5, 5));
	return xchg;
} 

unsigned char __atomic_xor_fetch_1(volatile void *atom, unsigned char value, int memorder)
{
 unsigned char xchg, cmp;
 __atomic_load((unsigned char *)atom, (unsigned char *)&cmp, 0);
do
{
	xchg = ((cmp ^ value));
} 
while (!__atomic_compare_exchange((unsigned char *)atom, &cmp, &xchg, 1, 5, 5));
	return xchg;
} 

unsigned char __atomic_nand_fetch_1(volatile void *atom, unsigned char value, int memorder)
{
 unsigned char xchg, cmp;
 __atomic_load((unsigned char *)atom, (unsigned char *)&cmp, 0);
do
{
	xchg = (~(cmp & value));
} 
while (!__atomic_compare_exchange((unsigned char *)atom, &cmp, &xchg, 1, 5, 5));
	return xchg;
} 

unsigned char __atomic_fetch_add_1(volatile void *atom, unsigned char value, int memorder)
{
 unsigned char xchg, cmp;
 __atomic_load((unsigned char *)atom, (unsigned char *)&cmp, 0);
do
{
	xchg = ((cmp + value));
} 
while (!__atomic_compare_exchange((unsigned char *)atom, &cmp, &xchg, 1, 5, 5));
	return cmp;
} 

unsigned char __atomic_fetch_sub_1(volatile void *atom, unsigned char value, int memorder)
{
 unsigned char xchg, cmp;
 __atomic_load((unsigned char *)atom, (unsigned char *)&cmp, 0);
do
{
	xchg = ((cmp - value));
} 
while (!__atomic_compare_exchange((unsigned char *)atom, &cmp, &xchg, 1, 5, 5));
	return cmp;
} 

unsigned char __atomic_fetch_and_1(volatile void *atom, unsigned char value, int memorder)
{
 unsigned char xchg, cmp;
 __atomic_load((unsigned char *)atom, (unsigned char *)&cmp, 0);
do
{
	xchg = ((cmp & value));
} 
while (!__atomic_compare_exchange((unsigned char *)atom, &cmp, &xchg, 1, 5, 5));
	return cmp;
} 

unsigned char __atomic_fetch_or_1(volatile void *atom, unsigned char value, int memorder)
{
 unsigned char xchg, cmp;
 __atomic_load((unsigned char *)atom, (unsigned char *)&cmp, 0);
do
{
	xchg = ((cmp | value));
} 
while (!__atomic_compare_exchange((unsigned char *)atom, &cmp, &xchg, 1, 5, 5));
	return cmp;
} 

unsigned char __atomic_fetch_xor_1(volatile void *atom, unsigned char value, int memorder)
{
 unsigned char xchg, cmp;
 __atomic_load((unsigned char *)atom, (unsigned char *)&cmp, 0);
do
{
	xchg = ((cmp ^ value));
} 
while (!__atomic_compare_exchange((unsigned char *)atom, &cmp, &xchg, 1, 5, 5));
	return cmp;
} 

unsigned char __atomic_fetch_nand_1(volatile void *atom, unsigned char value, int memorder)
{
 unsigned char xchg, cmp;
 __atomic_load((unsigned char *)atom, (unsigned char *)&cmp, 0);
do
{
	xchg = (~(cmp & value));
} 
while (!__atomic_compare_exchange((unsigned char *)atom, &cmp, &xchg, 1, 5, 5));
	return cmp;
}
unsigned short __atomic_exchange_2(volatile void *atom, unsigned short value, int memorder)
{
 unsigned short xchg, cmp;
 __atomic_load((unsigned short *)atom, (unsigned short *)&cmp, 0);
do
{
	xchg = (value);
} 
while (!__atomic_compare_exchange((unsigned short *)atom, &cmp, &xchg, 1, 5, 5));
	return cmp;
} 

unsigned short __atomic_add_fetch_2(volatile void *atom, unsigned short value, int memorder)
{
 unsigned short xchg, cmp;
 __atomic_load((unsigned short *)atom, (unsigned short *)&cmp, 0);
do
{
	xchg = ((cmp + value));
} 
while (!__atomic_compare_exchange((unsigned short *)atom, &cmp, &xchg, 1, 5, 5));
	return xchg;
} 

unsigned short __atomic_sub_fetch_2(volatile void *atom, unsigned short value, int memorder)
{
 unsigned short xchg, cmp;
 __atomic_load((unsigned short *)atom, (unsigned short *)&cmp, 0);
do
{
	xchg = ((cmp - value));
} 
while (!__atomic_compare_exchange((unsigned short *)atom, &cmp, &xchg, 1, 5, 5));
	return xchg;
} 

unsigned short __atomic_and_fetch_2(volatile void *atom, unsigned short value, int memorder)
{
 unsigned short xchg, cmp;
 __atomic_load((unsigned short *)atom, (unsigned short *)&cmp, 0);
do
{
	xchg = ((cmp & value));
} 
while (!__atomic_compare_exchange((unsigned short *)atom, &cmp, &xchg, 1, 5, 5));
	return xchg;
} 

unsigned short __atomic_or_fetch_2(volatile void *atom, unsigned short value, int memorder)
{
 unsigned short xchg, cmp;
 __atomic_load((unsigned short *)atom, (unsigned short *)&cmp, 0);
do
{
	xchg = ((cmp | value));
} 
while (!__atomic_compare_exchange((unsigned short *)atom, &cmp, &xchg, 1, 5, 5));
	return xchg;
} 

unsigned short __atomic_xor_fetch_2(volatile void *atom, unsigned short value, int memorder)
{
 unsigned short xchg, cmp;
 __atomic_load((unsigned short *)atom, (unsigned short *)&cmp, 0);
do
{
	xchg = ((cmp ^ value));
} 
while (!__atomic_compare_exchange((unsigned short *)atom, &cmp, &xchg, 1, 5, 5));
	return xchg;
} 

unsigned short __atomic_nand_fetch_2(volatile void *atom, unsigned short value, int memorder)
{
 unsigned short xchg, cmp;
 __atomic_load((unsigned short *)atom, (unsigned short *)&cmp, 0);
do
{
	xchg = (~(cmp & value));
} 
while (!__atomic_compare_exchange((unsigned short *)atom, &cmp, &xchg, 1, 5, 5));
	return xchg;
} 

unsigned short __atomic_fetch_add_2(volatile void *atom, unsigned short value, int memorder)
{
 unsigned short xchg, cmp;
 __atomic_load((unsigned short *)atom, (unsigned short *)&cmp, 0);
do
{
	xchg = ((cmp + value));
} 
while (!__atomic_compare_exchange((unsigned short *)atom, &cmp, &xchg, 1, 5, 5));
	return cmp;
} 

unsigned short __atomic_fetch_sub_2(volatile void *atom, unsigned short value, int memorder)
{
 unsigned short xchg, cmp;
 __atomic_load((unsigned short *)atom, (unsigned short *)&cmp, 0);
do
{
	xchg = ((cmp - value));
} 
while (!__atomic_compare_exchange((unsigned short *)atom, &cmp, &xchg, 1, 5, 5));
	return cmp;
} 

unsigned short __atomic_fetch_and_2(volatile void *atom, unsigned short value, int memorder)
{
 unsigned short xchg, cmp;
 __atomic_load((unsigned short *)atom, (unsigned short *)&cmp, 0);
do
{
	xchg = ((cmp & value));
} 
while (!__atomic_compare_exchange((unsigned short *)atom, &cmp, &xchg, 1, 5, 5));
	return cmp;
} 

unsigned short __atomic_fetch_or_2(volatile void *atom, unsigned short value, int memorder)
{
 unsigned short xchg, cmp;
 __atomic_load((unsigned short *)atom, (unsigned short *)&cmp, 0);
do
{
	xchg = ((cmp | value));
} 
while (!__atomic_compare_exchange((unsigned short *)atom, &cmp, &xchg, 1, 5, 5));
	return cmp;
} 

unsigned short __atomic_fetch_xor_2(volatile void *atom, unsigned short value, int memorder)
{
 unsigned short xchg, cmp;
 __atomic_load((unsigned short *)atom, (unsigned short *)&cmp, 0);
do
{
	xchg = ((cmp ^ value));
} 
while (!__atomic_compare_exchange((unsigned short *)atom, &cmp, &xchg, 1, 5, 5));
	return cmp;
} 

unsigned short __atomic_fetch_nand_2(volatile void *atom, unsigned short value, int memorder)
{
 unsigned short xchg, cmp;
 __atomic_load((unsigned short *)atom, (unsigned short *)&cmp, 0);
do
{
	xchg = (~(cmp & value));
} 
while (!__atomic_compare_exchange((unsigned short *)atom, &cmp, &xchg, 1, 5, 5));
	return cmp;
}
unsigned int __atomic_exchange_4(volatile void *atom, unsigned int value, int memorder)
{
 unsigned int xchg, cmp;
 __atomic_load((unsigned int *)atom, (unsigned int *)&cmp, 0);
do
{
	xchg = (value);
} 
while (!__atomic_compare_exchange((unsigned int *)atom, &cmp, &xchg, 1, 5, 5));
	return cmp;
} 

unsigned int __atomic_add_fetch_4(volatile void *atom, unsigned int value, int memorder)
{
 unsigned int xchg, cmp;
 __atomic_load((unsigned int *)atom, (unsigned int *)&cmp, 0);
do
{
	xchg = ((cmp + value));
} 
while (!__atomic_compare_exchange((unsigned int *)atom, &cmp, &xchg, 1, 5, 5));
	return xchg;
} 

unsigned int __atomic_sub_fetch_4(volatile void *atom, unsigned int value, int memorder)
{
 unsigned int xchg, cmp;
 __atomic_load((unsigned int *)atom, (unsigned int *)&cmp, 0);
do
{
	xchg = ((cmp - value));
} 
while (!__atomic_compare_exchange((unsigned int *)atom, &cmp, &xchg, 1, 5, 5));
	return xchg;
} 

unsigned int __atomic_and_fetch_4(volatile void *atom, unsigned int value, int memorder)
{
 unsigned int xchg, cmp;
 __atomic_load((unsigned int *)atom, (unsigned int *)&cmp, 0);
do
{
	xchg = ((cmp & value));
} 
while (!__atomic_compare_exchange((unsigned int *)atom, &cmp, &xchg, 1, 5, 5));
	return xchg;
} 

unsigned int __atomic_or_fetch_4(volatile void *atom, unsigned int value, int memorder)
{
 unsigned int xchg, cmp;
 __atomic_load((unsigned int *)atom, (unsigned int *)&cmp, 0);
do
{
	xchg = ((cmp | value));
} 
while (!__atomic_compare_exchange((unsigned int *)atom, &cmp, &xchg, 1, 5, 5));
	return xchg;
} 

unsigned int __atomic_xor_fetch_4(volatile void *atom, unsigned int value, int memorder)
{
 unsigned int xchg, cmp;
 __atomic_load((unsigned int *)atom, (unsigned int *)&cmp, 0);
do
{
	xchg = ((cmp ^ value));
} 
while (!__atomic_compare_exchange((unsigned int *)atom, &cmp, &xchg, 1, 5, 5));
	return xchg;
} 

unsigned int __atomic_nand_fetch_4(volatile void *atom, unsigned int value, int memorder)
{
 unsigned int xchg, cmp;
 __atomic_load((unsigned int *)atom, (unsigned int *)&cmp, 0);
do
{
	xchg = (~(cmp & value));
} 
while (!__atomic_compare_exchange((unsigned int *)atom, &cmp, &xchg, 1, 5, 5));
	return xchg;
} 

unsigned int __atomic_fetch_add_4(volatile void *atom, unsigned int value, int memorder)
{
 unsigned int xchg, cmp;
 __atomic_load((unsigned int *)atom, (unsigned int *)&cmp, 0);
do
{
	xchg = ((cmp + value));
} 
while (!__atomic_compare_exchange((unsigned int *)atom, &cmp, &xchg, 1, 5, 5));
	return cmp;
} 

unsigned int __atomic_fetch_sub_4(volatile void *atom, unsigned int value, int memorder)
{
 unsigned int xchg, cmp;
 __atomic_load((unsigned int *)atom, (unsigned int *)&cmp, 0);
do
{
	xchg = ((cmp - value));
} 
while (!__atomic_compare_exchange((unsigned int *)atom, &cmp, &xchg, 1, 5, 5));
	return cmp;
} 

unsigned int __atomic_fetch_and_4(volatile void *atom, unsigned int value, int memorder)
{
 unsigned int xchg, cmp;
 __atomic_load((unsigned int *)atom, (unsigned int *)&cmp, 0);
do
{
	xchg = ((cmp & value));
} 
while (!__atomic_compare_exchange((unsigned int *)atom, &cmp, &xchg, 1, 5, 5));
	return cmp;
} 

unsigned int __atomic_fetch_or_4(volatile void *atom, unsigned int value, int memorder)
{
 unsigned int xchg, cmp;
 __atomic_load((unsigned int *)atom, (unsigned int *)&cmp, 0);
do
{
	xchg = ((cmp | value));
} 
while (!__atomic_compare_exchange((unsigned int *)atom, &cmp, &xchg, 1, 5, 5));
	return cmp;
} 

unsigned int __atomic_fetch_xor_4(volatile void *atom, unsigned int value, int memorder)
{
 unsigned int xchg, cmp;
 __atomic_load((unsigned int *)atom, (unsigned int *)&cmp, 0);
do
{
	xchg = ((cmp ^ value));
} 
while (!__atomic_compare_exchange((unsigned int *)atom, &cmp, &xchg, 1, 5, 5));
	return cmp;
} 

unsigned int __atomic_fetch_nand_4(volatile void *atom, unsigned int value, int memorder)
{
 unsigned int xchg, cmp;
 __atomic_load((unsigned int *)atom, (unsigned int *)&cmp, 0);
do
{
	xchg = (~(cmp & value));
} 
while (!__atomic_compare_exchange((unsigned int *)atom, &cmp, &xchg, 1, 5, 5));
	return cmp;
}
unsigned long long __atomic_exchange_8(volatile void *atom, unsigned long long value, int memorder)
{
 unsigned long long xchg, cmp;
 __atomic_load((unsigned long long *)atom, (unsigned long long *)&cmp, 0);
do
{
	xchg = (value);
} 
while (!__atomic_compare_exchange((unsigned long long *)atom, &cmp, &xchg, 1, 5, 5));
	return cmp;
} 

unsigned long long __atomic_add_fetch_8(volatile void *atom, unsigned long long value, int memorder)
{
 unsigned long long xchg, cmp;
 __atomic_load((unsigned long long *)atom, (unsigned long long *)&cmp, 0);
do
{
	xchg = ((cmp + value));
} 
while (!__atomic_compare_exchange((unsigned long long *)atom, &cmp, &xchg, 1, 5, 5));
	return xchg;
} 

unsigned long long __atomic_sub_fetch_8(volatile void *atom, unsigned long long value, int memorder)
{
 unsigned long long xchg, cmp;
 __atomic_load((unsigned long long *)atom, (unsigned long long *)&cmp, 0);
do
{
	xchg = ((cmp - value));
} 
while (!__atomic_compare_exchange((unsigned long long *)atom, &cmp, &xchg, 1, 5, 5));
	return xchg;
} 

unsigned long long __atomic_and_fetch_8(volatile void *atom, unsigned long long value, int memorder)
{
 unsigned long long xchg, cmp;
 __atomic_load((unsigned long long *)atom, (unsigned long long *)&cmp, 0);
do
{
	xchg = ((cmp & value));
} 
while (!__atomic_compare_exchange((unsigned long long *)atom, &cmp, &xchg, 1, 5, 5));
	return xchg;
} 

unsigned long long __atomic_or_fetch_8(volatile void *atom, unsigned long long value, int memorder)
{
 unsigned long long xchg, cmp;
 __atomic_load((unsigned long long *)atom, (unsigned long long *)&cmp, 0);
do
{
	xchg = ((cmp | value));
} 
while (!__atomic_compare_exchange((unsigned long long *)atom, &cmp, &xchg, 1, 5, 5));
	return xchg;
} 

unsigned long long __atomic_xor_fetch_8(volatile void *atom, unsigned long long value, int memorder)
{
 unsigned long long xchg, cmp;
 __atomic_load((unsigned long long *)atom, (unsigned long long *)&cmp, 0);
do
{
	xchg = ((cmp ^ value));
} 
while (!__atomic_compare_exchange((unsigned long long *)atom, &cmp, &xchg, 1, 5, 5));
	return xchg;
} 

unsigned long long __atomic_nand_fetch_8(volatile void *atom, unsigned long long value, int memorder)
{
 unsigned long long xchg, cmp;
 __atomic_load((unsigned long long *)atom, (unsigned long long *)&cmp, 0);
do
{
	xchg = (~(cmp & value));
} 
while (!__atomic_compare_exchange((unsigned long long *)atom, &cmp, &xchg, 1, 5, 5));
	return xchg;
} 

unsigned long long __atomic_fetch_add_8(volatile void *atom, unsigned long long value, int memorder)
{
 unsigned long long xchg, cmp;
 __atomic_load((unsigned long long *)atom, (unsigned long long *)&cmp, 0);
do
{
	xchg = ((cmp + value));
} 
while (!__atomic_compare_exchange((unsigned long long *)atom, &cmp, &xchg, 1, 5, 5));
	return cmp;
} 

unsigned long long __atomic_fetch_sub_8(volatile void *atom, unsigned long long value, int memorder)
{
 unsigned long long xchg, cmp;
 __atomic_load((unsigned long long *)atom, (unsigned long long *)&cmp, 0);
do
{
	xchg = ((cmp - value));
} 
while (!__atomic_compare_exchange((unsigned long long *)atom, &cmp, &xchg, 1, 5, 5));
	return cmp;
} 

unsigned long long __atomic_fetch_and_8(volatile void *atom, unsigned long long value, int memorder)
{
 unsigned long long xchg, cmp;
 __atomic_load((unsigned long long *)atom, (unsigned long long *)&cmp, 0);
do
{
	xchg = ((cmp & value));
} 
while (!__atomic_compare_exchange((unsigned long long *)atom, &cmp, &xchg, 1, 5, 5));
	return cmp;
} 

unsigned long long __atomic_fetch_or_8(volatile void *atom, unsigned long long value, int memorder)
{
 unsigned long long xchg, cmp;
 __atomic_load((unsigned long long *)atom, (unsigned long long *)&cmp, 0);
do
{
	xchg = ((cmp | value));
} 
while (!__atomic_compare_exchange((unsigned long long *)atom, &cmp, &xchg, 1, 5, 5));
	return cmp;
} 

unsigned long long __atomic_fetch_xor_8(volatile void *atom, unsigned long long value, int memorder)
{
 unsigned long long xchg, cmp;
 __atomic_load((unsigned long long *)atom, (unsigned long long *)&cmp, 0);
do
{
	xchg = ((cmp ^ value));
} 
while (!__atomic_compare_exchange((unsigned long long *)atom, &cmp, &xchg, 1, 5, 5));
	return cmp;
} 

unsigned long long __atomic_fetch_nand_8(volatile void *atom, unsigned long long value, int memorder)
{
 unsigned long long xchg, cmp;
 __atomic_load((unsigned long long *)atom, (unsigned long long *)&cmp, 0);
do
{
	xchg = (~(cmp & value));
} 
while (!__atomic_compare_exchange((unsigned long long *)atom, &cmp, &xchg, 1, 5, 5));
	return cmp;}

_Bool __atomic_is_lock_free (unsigned long size, const volatile void *ptr)
{
	_Bool ret;
	switch (size)
	{
	case 1: ret = 1; break;
	case 2: ret = 1; break;
	case 4: ret = 1; break;
	case 8: ret = 1; break;
	default: ret = 0; break;
	}
	return ret;
}

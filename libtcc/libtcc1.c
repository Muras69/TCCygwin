typedef int Wtype;
typedef unsigned int UWtype;
typedef unsigned int USItype;
typedef long long DWtype;
typedef unsigned long long UDWtype;
struct DWstruct {
    Wtype low, high;
};
typedef union
{
  struct DWstruct s;
  DWtype ll;
} DWunion;
typedef long double XFtype;
union ldouble_long {
    long double ld;
    struct {
        unsigned long long lower;
        unsigned short upper;
    } l;
};
union double_long {
    double d;
    struct {
        unsigned int lower;
        int upper;
    } l;
    long long ll;
};
union float_long {
    float f;
    unsigned int l;
};
float __floatundisf(unsigned long long a)
{
    DWunion uu;
    XFtype r;
    uu.ll = a;
    if (uu.s.high >= 0) {
        return (float)uu.ll;
    } else {
        r = (XFtype)uu.ll;
        r += 18446744073709551616.0;
        return (float)r;
    }
}
double __floatundidf(unsigned long long a)
{
    DWunion uu;
    XFtype r;
    uu.ll = a;
    if (uu.s.high >= 0) {
        return (double)uu.ll;
    } else {
        r = (XFtype)uu.ll;
        r += 18446744073709551616.0;
        return (double)r;
    }
}
long double __floatundixf(unsigned long long a)
{
    DWunion uu;
    XFtype r;
    uu.ll = a;
    if (uu.s.high >= 0) {
        return (long double)uu.ll;
    } else {
        r = (XFtype)uu.ll;
        r += 18446744073709551616.0;
        return (long double)r;
    }
}
unsigned long long __fixunssfdi (float a1)
{
    register union float_long fl1;
    register int exp;
    register unsigned long long l;
    fl1.f = a1;
    if (fl1.l == 0)
	return (0);
    exp = (((fl1.l) >> 23) & 0xFF) - 126 - 24;
    l = (((fl1.l) & 0x7FFFFF) | (1 << 23));
    if (exp >= 41)
        return 1ULL << 63;
    else if (exp >= 0)
        l <<= exp;
    else if (exp >= -23)
        l >>= -exp;
    else
	return 0;
    if (((fl1.l) & 0x80000000))
        l = (unsigned long long)-l;
    return l;
}
long long __fixsfdi (float a1)
{
    long long ret; int s;
    ret = __fixunssfdi((s = a1 >= 0) ? a1 : -a1);
    return s ? ret : -ret;
}
unsigned long long __fixunsdfdi (double a1)
{
    register union double_long dl1;
    register int exp;
    register unsigned long long l;
    dl1.d = a1;
    if (dl1.ll == 0)
	return (0);
    exp = (((dl1.l.upper) >> 20) & 0x7FF) - 1022 - 53;
    l = ((dl1.ll & (((long long)1 << 52)-1)) | ((long long)1 << 52));
    if (exp >= 12)
        return 1ULL << 63;
    else if (exp >= 0)
        l <<= exp;
    else if (exp >= -52)
        l >>= -exp;
    else
        return 0;
    if (((dl1.l.upper) & 0x80000000))
        l = (unsigned long long)-l;
    return l;
}
long long __fixdfdi (double a1)
{
    long long ret; int s;
    ret = __fixunsdfdi((s = a1 >= 0) ? a1 : -a1);
    return s ? ret : -ret;
}
unsigned long long __fixunsxfdi (long double a1)
{
    register union ldouble_long dl1;
    register int exp;
    register unsigned long long l;
    dl1.ld = a1;
    if (dl1.l.lower == 0 && dl1.l.upper == 0)
	return (0);
    exp = (dl1.l.upper & 0x7fff) - 16382 - 64;
    l = dl1.l.lower;
    if (exp > 0)
	return 1ULL << 63;
    if (exp < -63)
        return 0;
    l >>= -exp;
    if (((dl1.l.upper) & 0x8000))
        l = (unsigned long long)-l;
    return l;
}
long long __fixxfdi (long double a1)
{
    long long ret; int s;
    ret = __fixunsxfdi((s = a1 >= 0) ? a1 : -a1);
    return s ? ret : -ret;
}

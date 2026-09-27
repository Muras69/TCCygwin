@echo on
setlocal

cd /d "%~dp0"

if exist x86_64-win32-tcc_ rmdir /Q /S x86_64-win32-tcc_

x86_64-win32-tcc\x86_64-win32-tcc -Iwin32/include -DC2STR conftest.c -o c2str.exe

c2str.exe win32\include\tccdefs.h tccdefs_.h

x86_64-win32-tcc\x86_64-win32-tcc -Iwin32/include -Iwin32/include/winapi -c tcc.c -o tcc.o
x86_64-win32-tcc\x86_64-win32-tcc -Iwin32/include -Iwin32/include/winapi -c tcctools.c -o tcctools.o
x86_64-win32-tcc\x86_64-win32-tcc -Iwin32/include -Iwin32/include/winapi -c libtcc.c -o libtcc.o
x86_64-win32-tcc\x86_64-win32-tcc -Iwin32/include -Iwin32/include/winapi -c tccpp.c -o tccpp.o
x86_64-win32-tcc\x86_64-win32-tcc -Iwin32/include -Iwin32/include/winapi -c tccgen.c -o tccgen.o
x86_64-win32-tcc\x86_64-win32-tcc -Iwin32/include -Iwin32/include/winapi -c tccdbg.c -o tccdbg.o
x86_64-win32-tcc\x86_64-win32-tcc -Iwin32/include -Iwin32/include/winapi -c tccelf.c -o tccelf.o
x86_64-win32-tcc\x86_64-win32-tcc -Iwin32/include -Iwin32/include/winapi -c tccasm.c -o tccasm.o
x86_64-win32-tcc\x86_64-win32-tcc -Iwin32/include -Iwin32/include/winapi -c x86_64-gen.c -o x86_64-gen.o
x86_64-win32-tcc\x86_64-win32-tcc -Iwin32/include -Iwin32/include/winapi -c x86_64-link.c -o x86_64-link.o
x86_64-win32-tcc\x86_64-win32-tcc -Iwin32/include -Iwin32/include/winapi -c i386-asm.c -o i386-asm.o
x86_64-win32-tcc\x86_64-win32-tcc -Iwin32/include -Iwin32/include/winapi -c tccpe.c -o tccpe.o
x86_64-win32-tcc\x86_64-win32-tcc -Iwin32/include -Iwin32/include/winapi -c tccrun.c -o tccrun.o

x86_64-win32-tcc\x86_64-win32-tcc -ar rcs tcc.a tcctools.o libtcc.o tccpp.o tccgen.o tccdbg.o tccelf.o tccasm.o x86_64-gen.o x86_64-link.o i386-asm.o tccpe.o tccrun.o

x86_64-win32-tcc\x86_64-win32-tcc -Lwin32/libwin tcc.o tcc.a -o x86_64-win32-tcc.exe

x86_64-win32-tcc\x86_64-win32-tcc -Iwin32/include -c win32\libtcc\libtcc1.c -o libtcc1.o
x86_64-win32-tcc\x86_64-win32-tcc -Iwin32/include -Iwin32/include/winapi -c win32\libwin\crt1.c -o crt1.o
x86_64-win32-tcc\x86_64-win32-tcc -Iwin32/include -Iwin32/include/winapi -c win32\libwin\crt1w.c -o crt1w.o
x86_64-win32-tcc\x86_64-win32-tcc -Iwin32/include -Iwin32/include/winapi -c win32\libwin\wincrt1.c -o wincrt1.o
x86_64-win32-tcc\x86_64-win32-tcc -Iwin32/include -Iwin32/include/winapi -c win32\libwin\wincrt1w.c -o wincrt1w.o
x86_64-win32-tcc\x86_64-win32-tcc -Iwin32/include -Iwin32/include/winapi -c win32\libwin\dllcrt1.c -o dllcrt1.o
x86_64-win32-tcc\x86_64-win32-tcc -Iwin32/include -Iwin32/include/winapi -c win32\libwin\dllmain.c -o dllmain.o
x86_64-win32-tcc\x86_64-win32-tcc -Iwin32/include -c win32\libwin\winex.c -o winex.o
x86_64-win32-tcc\x86_64-win32-tcc -c win32\libwin\chkstk.S -o chkstk.o
x86_64-win32-tcc\x86_64-win32-tcc -c win32\libtcc\alloca.S -o alloca.o
x86_64-win32-tcc\x86_64-win32-tcc -c win32\libtcc\alloca-bt.S -o alloca-bt.o
x86_64-win32-tcc\x86_64-win32-tcc -Iwin32/include -c win32\libtcc\stdatomic.c -o stdatomic.o
x86_64-win32-tcc\x86_64-win32-tcc -c win32\libtcc\atomic.S -o atomic.o
x86_64-win32-tcc\x86_64-win32-tcc -Iwin32/include -c win32\libtcc\builtin.c -o builtin.o

x86_64-win32-tcc\x86_64-win32-tcc -ar rcs x86_64-win32-libtcc1.a libtcc1.o crt1.o crt1w.o wincrt1.o wincrt1w.o dllcrt1.o dllmain.o winex.o chkstk.o alloca.o alloca-bt.o stdatomic.o atomic.o builtin.o

mkdir x86_64-win32-tcc_\lib

move x86_64-win32-tcc.exe x86_64-win32-tcc_\
move x86_64-win32-libtcc1.a x86_64-win32-tcc_\lib\

del c2str.exe
del tccdefs_.h
del *.o
del *.a

endlocal

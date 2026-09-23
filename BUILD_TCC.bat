@echo on
setlocal

cd /d "%~dp0"

if exist x86_64-win32-tcc_ rmdir /Q /S x86_64-win32-tcc_

x86_64-win32-tcc\x86_64-win32-tcc -DC2STR conftest.c -o c2str.exe

c2str.exe win32\include\tccdefs.h tccdefs_.h

set /p TCC_GITHASH= < TCC_GITHASH

echo> config.h #define TCC_VERSION "%TCC_GITHASH%"
echo>> config.h #define CONFIG_TCC_CROSSPREFIX "x86_64-win32-"

x86_64-win32-tcc\x86_64-win32-tcc -c tcc.c -o tcc.o
x86_64-win32-tcc\x86_64-win32-tcc -c tcctools.c -o tcctools.o
x86_64-win32-tcc\x86_64-win32-tcc -c libtcc.c -o libtcc.o
x86_64-win32-tcc\x86_64-win32-tcc -c tccpp.c -o tccpp.o
x86_64-win32-tcc\x86_64-win32-tcc -c tccgen.c -o tccgen.o
x86_64-win32-tcc\x86_64-win32-tcc -c tccdbg.c -o tccdbg.o
x86_64-win32-tcc\x86_64-win32-tcc -c tccelf.c -o tccelf.o
x86_64-win32-tcc\x86_64-win32-tcc -c tccasm.c -o tccasm.o
x86_64-win32-tcc\x86_64-win32-tcc -c x86_64-gen.c -o x86_64-gen.o
x86_64-win32-tcc\x86_64-win32-tcc -c x86_64-link.c -o x86_64-link.o
x86_64-win32-tcc\x86_64-win32-tcc -c i386-asm.c -o i386-asm.o
x86_64-win32-tcc\x86_64-win32-tcc -c tccpe.c -o tccpe.o
x86_64-win32-tcc\x86_64-win32-tcc -c tccrun.c -o tccrun.o

x86_64-win32-tcc\x86_64-win32-tcc -ar rcs tcc.a tcctools.o libtcc.o tccpp.o tccgen.o tccdbg.o tccelf.o tccasm.o x86_64-gen.o x86_64-link.o i386-asm.o tccpe.o tccrun.o

x86_64-win32-tcc\x86_64-win32-tcc tcc.o tcc.a -o x86_64-win32-tcc.exe

x86_64-win32-tcc\x86_64-win32-tcc -c win32\libtcc\libtcc1.c -o libtcc1.o
x86_64-win32-tcc\x86_64-win32-tcc -c win32\libwin\crt1.c -o crt1.o
x86_64-win32-tcc\x86_64-win32-tcc -c win32\libwin\crt1w.c -o crt1w.o
x86_64-win32-tcc\x86_64-win32-tcc -c win32\libwin\wincrt1.c -o wincrt1.o
x86_64-win32-tcc\x86_64-win32-tcc -c win32\libwin\wincrt1w.c -o wincrt1w.o
x86_64-win32-tcc\x86_64-win32-tcc -c win32\libwin\dllcrt1.c -o dllcrt1.o
x86_64-win32-tcc\x86_64-win32-tcc -c win32\libwin\dllmain.c -o dllmain.o
x86_64-win32-tcc\x86_64-win32-tcc -c win32\libwin\winex.c -o winex.o
x86_64-win32-tcc\x86_64-win32-tcc -c win32\libwin\chkstk.S -o chkstk.o
x86_64-win32-tcc\x86_64-win32-tcc -c win32\libtcc\alloca.S -o alloca.o
x86_64-win32-tcc\x86_64-win32-tcc -c win32\libtcc\alloca-bt.S -o alloca-bt.o
x86_64-win32-tcc\x86_64-win32-tcc -c win32\libtcc\stdatomic.c -o stdatomic.o
x86_64-win32-tcc\x86_64-win32-tcc -c win32\libtcc\atomic.S -o atomic.o
x86_64-win32-tcc\x86_64-win32-tcc -c win32\libtcc\builtin.c -o builtin.o

x86_64-win32-tcc\x86_64-win32-tcc -ar rcs x86_64-win32-libtcc1.a libtcc1.o crt1.o crt1w.o wincrt1.o wincrt1w.o dllcrt1.o dllmain.o winex.o chkstk.o alloca.o alloca-bt.o stdatomic.o atomic.o builtin.o

x86_64-win32-tcc\x86_64-win32-tcc -bt -c win32\libtcc\bcheck.c -o x86_64-win32-bcheck.o
x86_64-win32-tcc\x86_64-win32-tcc -bt -DBCHECK_RUN -c win32\libtcc\bcheck.c -o x86_64-win32-bcheck_run.o
x86_64-win32-tcc\x86_64-win32-tcc -c win32\libtcc\bt-exe.c -o x86_64-win32-bt-exe.o
x86_64-win32-tcc\x86_64-win32-tcc -c win32\libtcc\bt-log.c -o x86_64-win32-bt-log.o
x86_64-win32-tcc\x86_64-win32-tcc -c win32\libtcc\bt-dll.c -o x86_64-win32-bt-dll.o
x86_64-win32-tcc\x86_64-win32-tcc -c win32\libtcc\runmain.c -o x86_64-win32-runmain.o

mkdir x86_64-win32-tcc_\lib x86_64-win32-tcc_\include x86_64-win32-tcc_\doc
xcopy /E /Q /Y win32\include\ x86_64-win32-tcc_\include\
xcopy /E /Q /Y doc\ x86_64-win32-tcc_\doc\

move x86_64-win32-tcc.exe x86_64-win32-tcc_\
move x86_64-win32-libtcc1.a x86_64-win32-tcc_\lib\
move x86_64-win32-runmain.o x86_64-win32-tcc_\lib\
move x86_64-win32-bt-exe.o x86_64-win32-tcc_\lib\
move x86_64-win32-bt-dll.o x86_64-win32-tcc_\lib\
move x86_64-win32-bt-log.o x86_64-win32-tcc_\lib\
move x86_64-win32-bcheck.o x86_64-win32-tcc_\lib\
move x86_64-win32-bcheck_run.o x86_64-win32-tcc_\lib\

copy win32\libwin\gdi32.def x86_64-win32-tcc_\lib\
copy win32\libwin\kernel32.def x86_64-win32-tcc_\lib\
copy win32\libwin\msvcrt.def x86_64-win32-tcc_\lib\
copy win32\libwin\user32.def x86_64-win32-tcc_\lib\
copy win32\libwin\ws2_32.def x86_64-win32-tcc_\lib\

del c2str.exe
del tccdefs_.h
del *.o
del *.a
del config.h

endlocal

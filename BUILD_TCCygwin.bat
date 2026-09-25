@echo on
setlocal

cd /d "%~dp0"

call BUILD_TCC_bootstrapping.bat

if exist TCCygwin-3.6.10-1 rmdir /Q /S TCCygwin-3.6.10-1
if exist TCCygwin-3.7.0-0.605 rmdir /Q /S TCCygwin-3.7.0-0.605

x86_64-win32-tcc\x86_64-win32-tcc -DC2STR conftest.c -o c2str.exe

c2str.exe win32\include\tccdefs.h tccdefs_.h

set /p TCC_GITHASH= < TCC_GITHASH

echo> config.h #define TCC_VERSION "%TCC_GITHASH%"
echo>> config.h #define TCC_TARGET_X86_64 1
echo>> config.h #define TCC_TARGET_PE 1
echo>> config.h #define CONFIG_TCC_BACKTRACE 0
echo>> config.h #define CONFIG_TCC_BCHECK 0
echo>> config.h #define CONFIG_TCC_PIC 0
echo>> config.h #define CONFIG_TCC_SEMLOCK 0
echo>> config.h #define CONFIG_SYSROOT ""
echo>> config.h #define CONFIG_TCC_CRTPREFIX "{B}/lib"
echo>> config.h #define CONFIG_TCC_SYSINCLUDEPATHS "{B}/include"
echo>> config.h #define CONFIG_TCC_LIBPATHS "{B}/lib"
echo>> config.h #define TCC_LIBTCC1 "libtcc1.a"
echo>> config.h #define CONFIG_TCC_CROSSPREFIX "x86_64-win32-"
echo>> config.h #define CONFIG_TCC_PREDEFS 0
echo>> config.h #define __CYGWIN__

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

x86_64-win32-tcc\x86_64-win32-tcc -c win32\libwin\chkstk.S -o cygwin\x86_64-win32-chkstk.o

x86_64-win32-tcc\x86_64-win32-tcc -DCYGWIN_VERSION_DLL_MAJOR=3006 -DCYGWIN_VERSION_DLL_MINOR=10 -DCYGWIN_VERSION_API_MAJOR=0 -DCYGWIN_VERSION_API_MINOR=357 -c cygwin\crt0.c -o cygwin\CRT0_CYGWIN3610100.o

x86_64-win32-tcc\x86_64-win32-tcc -DCYGWIN_VERSION_DLL_MAJOR=3007 -DCYGWIN_VERSION_DLL_MINOR=0 -DCYGWIN_VERSION_API_MAJOR=0 -DCYGWIN_VERSION_API_MINOR=362 -c cygwin\crt0.c -o cygwin\CRT0_CYGWIN3700605.o

mkdir TCCygwin-3.6.10-1\lib TCCygwin-3.6.10-1\include
mkdir TCCygwin-3.7.0-0.605\lib TCCygwin-3.7.0-0.605\include

move cygwin\CRT0_CYGWIN3610100.o TCCygwin-3.6.10-1\lib\crt0.o
x86_64-win32-tcc\x86_64-win32-tcc -ar rcs TCCygwin-3.6.10-1\lib\x86_64-win32-libtcc1.a TCCygwin-3.6.10-1\lib\crt0.o
del TCCygwin-3.6.10-1\lib\crt0.o

move cygwin\CRT0_CYGWIN3700605.o TCCygwin-3.7.0-0.605\lib\crt0.o
x86_64-win32-tcc\x86_64-win32-tcc -ar rcs TCCygwin-3.7.0-0.605\lib\x86_64-win32-libtcc1.a TCCygwin-3.7.0-0.605\lib\crt0.o
del TCCygwin-3.7.0-0.605\lib\crt0.o

copy cygwin\x86_64-win32-chkstk.o TCCygwin-3.6.10-1\lib
copy cygwin\x86_64-win32-chkstk.o TCCygwin-3.7.0-0.605\lib
del cygwin\x86_64-win32-chkstk.o

xcopy /E /Q /Y cygwin\INCLUDE_3610100 TCCygwin-3.6.10-1\include
xcopy /E /Q /Y cygwin\INCLUDE_3610100 TCCygwin-3.7.0-0.605\include
xcopy /E /Q /Y cygwin\INCLUDE_3700605 TCCygwin-3.7.0-0.605\include

copy cygwin\kernel32.def TCCygwin-3.6.10-1\lib
copy cygwin\kernel32.def TCCygwin-3.7.0-0.605\lib

copy cygwin\CYGWIN1_3610100.def TCCygwin-3.6.10-1\lib\cygwin1.def
copy cygwin\CYGWIN1_3700605.def TCCygwin-3.7.0-0.605\lib\cygwin1.def

copy x86_64-win32-tcc.exe TCCygwin-3.6.10-1\
copy x86_64-win32-tcc.exe TCCygwin-3.7.0-0.605\

mkdir TCCygwin-3.6.10-1\examples
mkdir TCCygwin-3.7.0-0.605\examples

xcopy /E /Q /Y cygwin\examples TCCygwin-3.6.10-1\examples
xcopy /E /Q /Y cygwin\examples TCCygwin-3.7.0-0.605\examples

xcopy /E /Q /Y doc\ TCCygwin-3.6.10-1\doc\
xcopy /E /Q /Y doc\ TCCygwin-3.7.0-0.605\doc\

del c2str.exe
del x86_64-win32-tcc.exe
del tccdefs_.h
del *.o
del *.a
del config.h

endlocal

pause

@echo on
setlocal

cd /d "%~dp0"

call BUILD_TCC_bootstrapping.bat

if exist TCCygwin-3.6.11-1 rmdir /Q /S TCCygwin-3.6.11-1
if exist TCCygwin-3.7.0-0.622 rmdir /Q /S TCCygwin-3.7.0-0.622

x86_64-win32-tcc\x86_64-win32-tcc -nostdinc -D__CYGWIN__ tcc.c -o x86_64-win32-tcc.exe

x86_64-win32-tcc\x86_64-win32-tcc -nostdinc -nostdlib -c libtcc\chkstk.S -o cygwin\x86_64-win32-chkstk.o

x86_64-win32-tcc\x86_64-win32-tcc -nostdinc -nostdlib -Icygwin/INCLUDE_3611100 -DCYGWIN_VERSION_DLL_MAJOR=3006 -DCYGWIN_VERSION_DLL_MINOR=11 -DCYGWIN_VERSION_API_MAJOR=0 -DCYGWIN_VERSION_API_MINOR=357 -c cygwin\crt0.c -o cygwin\CRT0_CYGWIN3611100.o

x86_64-win32-tcc\x86_64-win32-tcc -nostdinc -nostdlib -Icygwin/INCLUDE_3611100 -DCYGWIN_VERSION_DLL_MAJOR=3007 -DCYGWIN_VERSION_DLL_MINOR=0 -DCYGWIN_VERSION_API_MAJOR=0 -DCYGWIN_VERSION_API_MINOR=362 -c cygwin\crt0.c -o cygwin\CRT0_CYGWIN3700622.o

mkdir TCCygwin-3.6.11-1\lib TCCygwin-3.6.11-1\include
mkdir TCCygwin-3.7.0-0.622\lib TCCygwin-3.7.0-0.622\include

move cygwin\CRT0_CYGWIN3611100.o TCCygwin-3.6.11-1\lib\crt0.o
x86_64-win32-tcc\x86_64-win32-tcc -ar rcs TCCygwin-3.6.11-1\lib\x86_64-win32-libtcc1.a TCCygwin-3.6.11-1\lib\crt0.o
del TCCygwin-3.6.11-1\lib\crt0.o

move cygwin\CRT0_CYGWIN3700622.o TCCygwin-3.7.0-0.622\lib\crt0.o
x86_64-win32-tcc\x86_64-win32-tcc -ar rcs TCCygwin-3.7.0-0.622\lib\x86_64-win32-libtcc1.a TCCygwin-3.7.0-0.622\lib\crt0.o
del TCCygwin-3.7.0-0.622\lib\crt0.o

copy cygwin\x86_64-win32-chkstk.o TCCygwin-3.6.11-1\lib
copy cygwin\x86_64-win32-chkstk.o TCCygwin-3.7.0-0.622\lib
del cygwin\x86_64-win32-chkstk.o

xcopy /E /Q /Y cygwin\INCLUDE_3611100 TCCygwin-3.6.11-1\include
xcopy /E /Q /Y cygwin\INCLUDE_3611100 TCCygwin-3.7.0-0.622\include
xcopy /E /Q /Y cygwin\INCLUDE_3700622 TCCygwin-3.7.0-0.622\include

copy cygwin\kernel32.def TCCygwin-3.6.11-1\lib
copy cygwin\kernel32.def TCCygwin-3.7.0-0.622\lib

copy cygwin\CYGWIN1_3611100.def TCCygwin-3.6.11-1\lib\cygwin1.def
copy cygwin\CYGWIN1_3700622.def TCCygwin-3.7.0-0.622\lib\cygwin1.def

copy x86_64-win32-tcc.exe TCCygwin-3.6.11-1\
copy x86_64-win32-tcc.exe TCCygwin-3.7.0-0.622\
del x86_64-win32-tcc.exe

rmdir /Q /S x86_64-win32-tcc\lib

mkdir TCCygwin-3.6.11-1\examples
mkdir TCCygwin-3.7.0-0.622\examples

xcopy /E /Q /Y cygwin\examples TCCygwin-3.6.11-1\examples
xcopy /E /Q /Y cygwin\examples TCCygwin-3.7.0-0.622\examples

xcopy /E /Q /Y doc\ TCCygwin-3.6.11-1\doc\
xcopy /E /Q /Y doc\ TCCygwin-3.7.0-0.622\doc\

endlocal

pause

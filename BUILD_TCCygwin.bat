@echo on
setlocal

cd /d "%~dp0"

call BUILD_TCC_bootstrapping.bat

if exist TCCygwin-3.6.12-1 rmdir /Q /S TCCygwin-3.6.12-1
if exist TCCygwin-3.7.0-0.655 rmdir /Q /S TCCygwin-3.7.0-0.655

mkdir TCCygwin-3.6.12-1\lib TCCygwin-3.6.12-1\include
mkdir TCCygwin-3.7.0-0.655\lib TCCygwin-3.7.0-0.655\include

x86_64-win32-tcc\x86_64-win32-tcc -nostdinc -D__CYGWIN__ tcc.c -o x86_64-win32-tcc.exe

cd libtcc\

..\x86_64-win32-tcc\x86_64-win32-tcc -nostdinc -nostdlib -c chkstk.S -o chkstk.o
..\x86_64-win32-tcc\x86_64-win32-tcc -nostdinc -nostdlib -c libtcc1.c -o libtcc1.o
..\x86_64-win32-tcc\x86_64-win32-tcc -nostdinc -nostdlib -c alloca.S -o alloca.o
..\x86_64-win32-tcc\x86_64-win32-tcc -nostdinc -nostdlib -c builtin.c -o builtin.o
..\x86_64-win32-tcc\x86_64-win32-tcc -nostdinc -nostdlib -c atomic.S -o atomic.o
..\x86_64-win32-tcc\x86_64-win32-tcc -nostdinc -nostdlib -c stdatomic.c -o stdatomic.o
..\x86_64-win32-tcc\x86_64-win32-tcc -nostdinc -nostdlib -c winex.c -o winex.o
..\x86_64-win32-tcc\x86_64-win32-tcc -nostdinc -nostdlib -c dllcrt1.c -o dllcrt1.o
..\x86_64-win32-tcc\x86_64-win32-tcc -nostdinc -nostdlib -c dllmain.c -o dllmain.o

..\x86_64-win32-tcc\x86_64-win32-tcc -nostdinc -nostdlib -DCYGWIN_VERSION_DLL_MAJOR=3006 -DCYGWIN_VERSION_DLL_MINOR=12 -DCYGWIN_VERSION_API_MAJOR=0 -DCYGWIN_VERSION_API_MINOR=357 -c ..\cygwin\crt0.c -o crt0.o
..\x86_64-win32-tcc\x86_64-win32-tcc -ar rcs x86_64-win32-libtcc1.a chkstk.o crt0.o libtcc1.o alloca.o builtin.o atomic.o stdatomic.o winex.o dllcrt1.o dllmain.o
move x86_64-win32-libtcc1.a ..\TCCygwin-3.6.12-1\lib\
..\x86_64-win32-tcc\x86_64-win32-tcc -nostdinc -nostdlib -DCYGWIN_VERSION_DLL_MAJOR=3007 -DCYGWIN_VERSION_DLL_MINOR=0 -DCYGWIN_VERSION_API_MAJOR=0 -DCYGWIN_VERSION_API_MINOR=362 -c ..\cygwin\crt0.c -o crt0.o
..\x86_64-win32-tcc\x86_64-win32-tcc -ar rcs x86_64-win32-libtcc1.a chkstk.o crt0.o libtcc1.o alloca.o builtin.o atomic.o stdatomic.o winex.o dllcrt1.o dllmain.o
move x86_64-win32-libtcc1.a ..\TCCygwin-3.7.0-0.655\lib\

del *.o

cd ..

xcopy /E /Q /Y cygwin\INCLUDE_36UNDEF TCCygwin-3.6.12-1\include
xcopy /E /Q /Y cygwin\INCLUDE_36UNDEF TCCygwin-3.7.0-0.655\include
xcopy /E /Q /Y cygwin\INCLUDE_37UNDEF TCCygwin-3.7.0-0.655\include

copy cygwin\kernel32.def TCCygwin-3.6.12-1\lib
copy cygwin\kernel32.def TCCygwin-3.7.0-0.655\lib

copy cygwin\CYGWIN1_36UNDEF.def TCCygwin-3.6.12-1\lib\cygwin1.def
copy cygwin\CYGWIN1_37UNDEF.def TCCygwin-3.7.0-0.655\lib\cygwin1.def

copy x86_64-win32-tcc.exe TCCygwin-3.6.12-1\
copy x86_64-win32-tcc.exe TCCygwin-3.7.0-0.655\
del x86_64-win32-tcc.exe

xcopy /E /Q /Y doc\ TCCygwin-3.6.12-1\doc\
xcopy /E /Q /Y doc\ TCCygwin-3.7.0-0.655\doc\

mkdir TCCygwin-3.6.12-1\examples
mkdir TCCygwin-3.7.0-0.655\examples

xcopy /E /Q /Y cygwin\examples TCCygwin-3.6.12-1\examples
xcopy /E /Q /Y cygwin\examples TCCygwin-3.7.0-0.655\examples

rmdir /Q /S x86_64-win32-tcc\lib

endlocal

pause

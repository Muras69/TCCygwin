@echo on
setlocal

cd /d "%~dp0"

if exist x86_64-win32-tcc_ rmdir /Q /S x86_64-win32-tcc_

cd libtcc\

..\x86_64-win32-tcc\x86_64-win32-tcc -nostdinc -nostdlib -c chkstk.S -o chkstk.o
..\x86_64-win32-tcc\x86_64-win32-tcc -nostdinc -nostdlib -c alloca.S -o alloca.o
..\x86_64-win32-tcc\x86_64-win32-tcc -nostdinc -nostdlib -c crt1.c -o crt1.o
..\x86_64-win32-tcc\x86_64-win32-tcc -nostdinc -nostdlib -c libtcc1.c -o libtcc1.o

..\x86_64-win32-tcc\x86_64-win32-tcc -ar rcs x86_64-win32-libtcc1.a chkstk.o alloca.o crt1.o libtcc1.o

mkdir ..\x86_64-win32-tcc_\lib

move x86_64-win32-libtcc1.a ..\x86_64-win32-tcc_\lib\
copy kernel32.def ..\x86_64-win32-tcc_\lib\
copy msvcrt.def ..\x86_64-win32-tcc_\lib\

del *.o

cd ..

x86_64-win32-tcc\x86_64-win32-tcc -nostdinc -Lx86_64-win32-tcc_\lib\ tcc.c -o x86_64-win32-tcc.exe

move x86_64-win32-tcc.exe x86_64-win32-tcc_\

endlocal

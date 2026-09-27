@echo on
setlocal

cd /d "%~dp0"

set /p TCC_GITHASH= < TCC_GITHASH

echo> config.h #define TCC_VERSION "0.9.28rc"
echo>> config.h #define TCC_GITHASH "%TCC_GITHASH%"
echo>> config.h #define CONFIG_TCC_CROSSPREFIX "x86_64-win32-"

call BUILD_TCC.bat
rmdir /Q /S x86_64-win32-tcc
rename x86_64-win32-tcc_ x86_64-win32-tcc
call BUILD_TCC.bat
rmdir /Q /S x86_64-win32-tcc
rename x86_64-win32-tcc_ x86_64-win32-tcc
call BUILD_TCC.bat
rmdir /Q /S x86_64-win32-tcc
rename x86_64-win32-tcc_ x86_64-win32-tcc
call BUILD_TCC.bat
rmdir /Q /S x86_64-win32-tcc
rename x86_64-win32-tcc_ x86_64-win32-tcc

x86_64-win32-tcc\x86_64-win32-tcc -Iwin32/include -Iwin32/include/winapi -bt -c win32\libtcc\bcheck.c -o x86_64-win32-bcheck.o
x86_64-win32-tcc\x86_64-win32-tcc -Iwin32/include -Iwin32/include/winapi -bt -DBCHECK_RUN -c win32\libtcc\bcheck.c -o x86_64-win32-bcheck_run.o
x86_64-win32-tcc\x86_64-win32-tcc -Iwin32/include -Iwin32/include/winapi -c win32\libtcc\bt-exe.c -o x86_64-win32-bt-exe.o
x86_64-win32-tcc\x86_64-win32-tcc -Iwin32/include -c win32/libtcc/bt-log.c -o x86_64-win32-bt-log.o
x86_64-win32-tcc\x86_64-win32-tcc -Iwin32/include -Iwin32/include/winapi -c win32\libtcc\bt-dll.c -o x86_64-win32-bt-dll.o
x86_64-win32-tcc\x86_64-win32-tcc -Iwin32/include -c win32/libtcc/runmain.c -o x86_64-win32-runmain.o

move x86_64-win32-runmain.o x86_64-win32-tcc\lib\
move x86_64-win32-bt-exe.o x86_64-win32-tcc\lib\
move x86_64-win32-bt-dll.o x86_64-win32-tcc\lib\
move x86_64-win32-bt-log.o x86_64-win32-tcc\lib\
move x86_64-win32-bcheck.o x86_64-win32-tcc\lib\
move x86_64-win32-bcheck_run.o x86_64-win32-tcc\lib\

copy win32\libwin\gdi32.def x86_64-win32-tcc\lib\
copy win32\libwin\kernel32.def x86_64-win32-tcc\lib\
copy win32\libwin\msvcrt.def x86_64-win32-tcc\lib\
copy win32\libwin\user32.def x86_64-win32-tcc\lib\
copy win32\libwin\ws2_32.def x86_64-win32-tcc\lib\

mkdir x86_64-win32-tcc\include x86_64-win32-tcc\doc

xcopy /E /Q /Y win32\include\ x86_64-win32-tcc\include\
xcopy /E /Q /Y doc\ x86_64-win32-tcc\doc\

del config.h

endlocal

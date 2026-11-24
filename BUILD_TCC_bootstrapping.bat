@echo on
setlocal

cd /d "%~dp0"

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

endlocal

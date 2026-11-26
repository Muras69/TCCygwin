### socket and dlopen test ###

compilation:

x86_64-win32-tcc -Wall -Werror=unsupported -vv -shared server.c -o libserver.dll
x86_64-win32-tcc -Wall -Werror=unsupported -vv server_run.c -o server_run.exe

start-up:

server_run.exe

### end of test ###

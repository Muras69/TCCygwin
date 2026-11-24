extern void (*__init_array_start[])(int argc, char **argv, char **envp);
extern void (*__init_array_end[])(int argc, char **argv, char **envp);
extern void (*__fini_array_start[])(void);
extern void (*__fini_array_end[])(void);

typedef struct { int unused; } *HINSTANCE;

int DllMain(HINSTANCE hDll, unsigned long dwReason, void *lpReserved);

static void run_ctors(int argc, char **argv, char **env)
{
    int i = 0;
    while (&__init_array_start[i] != __init_array_end)
        (*__init_array_start[i++])(argc, argv, env);
}

static void run_dtors(void)
{
    int i = 0;
    while (&__fini_array_end[i] != __fini_array_start)
        (*__fini_array_end[--i])();
}

int _dllstart(HINSTANCE hDll, unsigned long dwReason, void *lpReserved)
{
    int bRet;
    if (dwReason == 1)
        run_ctors(0, 0, 0);
    bRet = DllMain(hDll, dwReason, lpReserved);
    if (dwReason == 0)
        run_dtors();
    return bRet;
}

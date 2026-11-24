extern int main(int argc, char * argv[], char * env[]);
extern void (*__init_array_start[]) (int argc, char **argv, char **envp);
extern void (*__init_array_end[]) (int argc, char **argv, char **envp);
extern void (*__fini_array_start[]) (void);
extern void (*__fini_array_end[]) (void);
extern int *_imp____argc;
extern char ***_imp____argv;
extern char ***_imp___environ;

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef unsigned long long DWORD64;
typedef long long LONGLONG;
typedef unsigned long long ULONGLONG;
typedef unsigned long long ULONG_PTR;
typedef void *PVOID;

#define EXCEPTION_MAXIMUM_PARAMETERS 15

typedef struct __attribute__((aligned(16))) _M128A {
  ULONGLONG Low;
  LONGLONG High;
} M128A;

typedef struct _XMM_SAVE_AREA32 {
  WORD ControlWord;
  WORD StatusWord;
  BYTE TagWord;
  BYTE Reserved1;
  WORD ErrorOpcode;
  DWORD ErrorOffset;
  WORD ErrorSelector;
  WORD Reserved2;
  DWORD DataOffset;
  WORD DataSelector;
  WORD Reserved3;
  DWORD MxCsr;
  DWORD MxCsr_Mask;
  M128A FloatRegisters[8];
  M128A XmmRegisters[16];
  BYTE Reserved4[96];
} XMM_SAVE_AREA32;

typedef struct __attribute__((aligned(16))) _CONTEXT {
  DWORD64 P1Home;
  DWORD64 P2Home;
  DWORD64 P3Home;
  DWORD64 P4Home;
  DWORD64 P5Home;
  DWORD64 P6Home;
  DWORD ContextFlags;
  DWORD MxCsr;
  WORD SegCs;
  WORD SegDs;
  WORD SegEs;
  WORD SegFs;
  WORD SegGs;
  WORD SegSs;
  DWORD EFlags;
  DWORD64 Dr0;
  DWORD64 Dr1;
  DWORD64 Dr2;
  DWORD64 Dr3;
  DWORD64 Dr6;
  DWORD64 Dr7;
  DWORD64 Rax;
  DWORD64 Rcx;
  DWORD64 Rdx;
  DWORD64 Rbx;
  DWORD64 Rsp;
  DWORD64 Rbp;
  DWORD64 Rsi;
  DWORD64 Rdi;
  DWORD64 R8;
  DWORD64 R9;
  DWORD64 R10;
  DWORD64 R11;
  DWORD64 R12;
  DWORD64 R13;
  DWORD64 R14;
  DWORD64 R15;
  DWORD64 Rip;
  union {
    XMM_SAVE_AREA32 FltSave;
    XMM_SAVE_AREA32 FloatSave;
    struct {
M128A Header[2];
M128A Legacy[8];
M128A Xmm0;
M128A Xmm1;
M128A Xmm2;
M128A Xmm3;
M128A Xmm4;
M128A Xmm5;
M128A Xmm6;
M128A Xmm7;
M128A Xmm8;
M128A Xmm9;
M128A Xmm10;
M128A Xmm11;
M128A Xmm12;
M128A Xmm13;
M128A Xmm14;
M128A Xmm15;
    };
  };
  M128A VectorRegister[26];
  DWORD64 VectorControl;
  DWORD64 DebugControl;
  DWORD64 LastBranchToRip;
  DWORD64 LastBranchFromRip;
  DWORD64 LastExceptionToRip;
  DWORD64 LastExceptionFromRip;
} CONTEXT;

typedef struct _EXCEPTION_RECORD {
  DWORD ExceptionCode;
  DWORD ExceptionFlags;
  struct _EXCEPTION_RECORD *ExceptionRecord;
  PVOID ExceptionAddress;
  DWORD NumberParameters;
  ULONG_PTR ExceptionInformation[15];
} EXCEPTION_RECORD;

typedef struct _EXCEPTION_POINTERS {
  EXCEPTION_RECORD *ExceptionRecord;
  CONTEXT *ContextRecord;
} EXCEPTION_POINTERS;

int _XcptFilter(unsigned long _ExceptionNum, struct _EXCEPTION_POINTERS * _ExceptionPtr);
typedef long (*PTOP_LEVEL_EXCEPTION_FILTER)(struct _EXCEPTION_POINTERS *ExceptionInfo);
typedef PTOP_LEVEL_EXCEPTION_FILTER LPTOP_LEVEL_EXCEPTION_FILTER;
LPTOP_LEVEL_EXCEPTION_FILTER SetUnhandledExceptionFilter(LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter);
typedef struct { int newmode; } _startupinfo;
int __getmainargs(int *pargc, char ***pargv, char ***penv, int globb, _startupinfo*);
void __set_app_type(int apptype);
unsigned int _controlfp(unsigned int new_value, unsigned int mask);
void exit(int _Code) __attribute__((noreturn));

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
int _dowildcard;
static long catch_sig(EXCEPTION_POINTERS *ex)
{
  return _XcptFilter(ex->ExceptionRecord->ExceptionCode, ex);
}
void _start(void)
{
    int ret;
    _startupinfo start_info = {0};
    SetUnhandledExceptionFilter(catch_sig);
    __set_app_type(1);
    _controlfp(0x00010000, 0x00030000);
    __getmainargs( &(*_imp____argc), &(*_imp____argv), &(*_imp___environ), _dowildcard, &start_info);
    run_ctors((*_imp____argc), (*_imp____argv), (*_imp___environ));
    ret = main((*_imp____argc), (*_imp____argv), (*_imp___environ));
    run_dtors();
    exit(ret);
}

/* core_dump.h

   Written by Egor Duda <deo@logos-m.ru>

This file is part of Cygwin.

This software is a copyrighted work licensed under the terms of the
Cygwin license.  Please consult the file "CYGWIN_LICENSE" for
details. */

#ifndef _CYGWIN_CORE_DUMP_H
#define _CYGWIN_CORE_DUMP_H

/*
  Note that elfcore_grok_win32pstatus() in libbfd relies on the precise layout
  of these structures.
*/

#include <stdint.h>
#include <stdbool.h>

#define BOOL _Bool
#define BYTE uint8_t
#define WORD uint16_t
#define DWORD uint32_t
#define DWORD64 uint64_t
#define LONGLONG int64_t
#define ULONGLONG uint64_t

#define	NOTE_INFO_PROCESS	1
#define	NOTE_INFO_THREAD	2
#define	NOTE_INFO_MODULE	3
#define	NOTE_INFO_MODULE64	4

typedef struct _M128A {
  ULONGLONG Low;
  LONGLONG High;
} M128A __attribute__((aligned(16)));

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

typedef struct _CONTEXT {
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
  union __C89u_NAMELESS {
    XMM_SAVE_AREA32 FltSave;
    XMM_SAVE_AREA32 FloatSave;
    struct __C89s_NAMELESS {
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
} CONTEXT __attribute__((aligned(16)));

struct win32_core_process_info
{
  DWORD pid;
  DWORD signal;
  DWORD command_line_size;
  char command_line[1];
}__attribute__((__packed__));

struct win32_core_thread_info
{
  DWORD tid;
  BOOL is_active_thread;
  CONTEXT thread_context;
}__attribute__((__packed__));

/* Used with data_type NOTE_INFO_MODULE or NOTE_INFO_MODULE64, depending on
   arch */
struct win32_core_module_info
{
  void* base_address;
  DWORD module_name_size;
  char module_name[1];
}__attribute__((__packed__));

struct win32_pstatus
{
  DWORD data_type;
  union
    {
      struct win32_core_process_info process_info;
      struct win32_core_thread_info thread_info;
      struct win32_core_module_info module_info;
    } data ;
}__attribute__((__packed__));

typedef struct win32_pstatus win32_pstatus_t ;

#endif /* _CYGWIN_CORE_DUMP_H */

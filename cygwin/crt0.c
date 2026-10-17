/* created and modified 2026-09-15 by Przemysław Korcik (p.korcik@gmail.com)

      Everything you need to launch the application from cygwin1.dll

    Some parts have been combined and modified from the original files
    and header files.

   This software is a copyrighted work licensed under the terms of the
   Cygwin license.  Please consult the file "CYGWIN_LICENSE" for
   details. */

#include <stdint.h>

extern int main(int argc, char **argv);
extern void _dll_crt0() __attribute__((dllimport)) __attribute__((noreturn));
extern struct _reent *_impure_ptr __attribute__((dllimport));
extern uintptr_t cygwin_internal(uint32_t, ...) __attribute__((dllimport));
extern void *malloc(size_t) __attribute__((dllimport));
extern void free(void *) __attribute__((dllimport));
extern void *realloc(void *, size_t) __attribute__((dllimport));
extern void *calloc(size_t, size_t) __attribute__((dllimport));
extern int posix_memalign (void **, size_t, size_t) __attribute__((dllimport));
extern void* GetModuleHandleA(const char *) __attribute__((dllimport));
extern int __cxa_atexit(void (*)(void*), void*, void*) __attribute__((dllimport));

#define CW_USER_DATA 8
#define SIZEOF_PER_PROCESS (41 * 8)

typedef int (*MainFunc)(int argc, char *argv[], char **env);

int __dynamically_loaded = 0;
int _fmode;

struct per_process
{

  char *initial_sp;

  uint32_t magic_biscuit;
  uint32_t dll_major;
  uint32_t dll_minor;

  struct _reent **impure_ptr_ptr;

  void *(*malloc)(size_t);
  void (*free)(void *);
  void *(*realloc)(void *, size_t);

  int *fmode_ptr;

  int (*main)(int, char **, char **);
  void (**ctors)(void);
  void (**dtors)(void);

  void *data_start;
  void *data_end;
  void *bss_start;
  void *bss_end;

  void *(*calloc)(size_t, size_t);

  void (*premain[4])(int, char **, struct per_process *);

  int32_t run_ctors_p;

  uintptr_t unused[7];

  void *hmodule;

  uint32_t api_major;
  uint32_t api_minor;
 
  uintptr_t unused2[4];

  int (*posix_memalign)(void **, size_t, size_t);

  void *pseudo_reloc_start;
  void *pseudo_reloc_end;
  void *image_base;

  void *threadinterface;

  struct _reent *impure_ptr;

};

int atexit(void (*fn)(void)){

  return __cxa_atexit((void (*)(void*))fn, NULL, NULL);

}

void _pei386_runtime_relocator(){
  
  return;

}

void cygwin_premain0(int argc, char **argv, struct per_process *u);
void cygwin_premain1(int argc, char **argv, struct per_process *u);
void cygwin_premain2(int argc, char **argv, struct per_process *u);
void cygwin_premain3(int argc, char **argv, struct per_process *u);

int _cygwin_crt0_common(MainFunc f, struct per_process *u){

  struct per_process *newu = (struct per_process *)cygwin_internal(CW_USER_DATA);

  u = newu;   

  u->magic_biscuit = SIZEOF_PER_PROCESS;

  u->dll_major = CYGWIN_VERSION_DLL_MAJOR;

  u->dll_minor = CYGWIN_VERSION_DLL_MINOR;

  u->api_major = CYGWIN_VERSION_API_MAJOR;

  u->api_minor = CYGWIN_VERSION_API_MINOR;

  _impure_ptr = u->impure_ptr;

  u->main = f;

  u->premain[0] = cygwin_premain0;
  u->premain[1] = cygwin_premain1;
  u->premain[2] = cygwin_premain2;
  u->premain[3] = cygwin_premain3;
  u->fmode_ptr = &_fmode;

  u->initial_sp = NULL;

  u->malloc = &malloc;
  u->free = &free;
  u->realloc = &realloc;
  u->calloc = &calloc;
  u->posix_memalign = &posix_memalign;

  u->hmodule = GetModuleHandleA(NULL);

  _pei386_runtime_relocator(NULL);

  return 1;

}

void cygwin_crt0(MainFunc f){

  _cygwin_crt0_common(f, NULL);
  _dll_crt0();

}

void cygwin_premain0(int argc, char **argv, struct per_process *u){

}

void cygwin_premain1(int argc, char **argv, struct per_process *u){

}

void cygwin_premain2(int argc, char **argv, struct per_process *u){

}

void cygwin_premain3(int argc, char **argv, struct per_process *u){

}

void mainCRTStartup(void){

  cygwin_crt0(main);

  cygwin_premain0(0, NULL, NULL);
  cygwin_premain1(0, NULL, NULL);
  cygwin_premain2(0, NULL, NULL);
  cygwin_premain3(0, NULL, NULL);

}

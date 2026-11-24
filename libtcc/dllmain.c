typedef struct { int unused; } *HINSTANCE;

int DllMain(HINSTANCE hDll, unsigned long dwReason, void *lpReserved)
{
  return 1;
}

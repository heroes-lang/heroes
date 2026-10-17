/* A header whose own code, kept by the link, reaches a function no library of
   the probe holds: a constructor calling it before main. */
int unlinked_somewhere(void);
__attribute__((constructor)) static void unlinked_starts(void) { (void)unlinked_somewhere(); }
#define U_VALUE (1 << 3)

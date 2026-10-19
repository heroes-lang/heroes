/* Defect 2146's header: a value that asks whether `main` is a macro where
   the header is read; the program never defines one. */
#ifdef main
#define K_M ((-2147483647 - 1) / -1)
#else
#define K_M 7
#endif

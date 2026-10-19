/* Defect 2146's header: a value C leaves undefined where `main` is not a
   macro, and the program never defines one. */
#ifndef main
#define K_N ((-2147483647 - 1) / -1)
#else
#define K_N 7
#endif

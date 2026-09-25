/* Critic, panel 178: a typedef'd union whose first arm is smaller than the union. */
#include <unistd.h>
typedef union { int a; char big[64]; } U;
static inline long write_u(U *u) { return (long)write(1, u, sizeof *u); }

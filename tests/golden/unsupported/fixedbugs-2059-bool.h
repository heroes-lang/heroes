/* Defect 2059, 2026-10-14: a header making `bool`, which the C the compiler
 * writes reads as C's own, a macro, and a constant of it. */
#define bool int
#define BOOL_SIZE ((long)sizeof(bool))

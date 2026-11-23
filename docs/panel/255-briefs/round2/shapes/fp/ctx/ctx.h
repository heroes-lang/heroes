/* Panel 255, the ffi-pragmatist: shape 3 on Darwin (the coordinator's header
 * compiles it out there): a heap context filled by `getcontext`, freed, then
 * a save point; and a coroutine whose context and 64 KiB stack are freed
 * while it is suspended, then a save point and a jump. */
#define _XOPEN_SOURCE 700
#include <setjmp.h>
#include <stdint.h>
#include <stdlib.h>
#include <ucontext.h>
static inline int64_t cx_heap(void) {
    ucontext_t *c = calloc(1, 1024);
    jmp_buf local;
    if (c == NULL || getcontext(c) != 0) return -1;
    free(c);
    return setjmp(local);
}
static ucontext_t *cx_main, *cx_co;
static void cx_body(void) { swapcontext(cx_co, cx_main); }
static inline int64_t cx_co_freed(void) {
    cx_main = calloc(1, 1024);
    cx_co = calloc(1, 1024);
    void *stack = malloc((size_t)1 << 16);
    if (cx_main == NULL || cx_co == NULL || stack == NULL || getcontext(cx_co) != 0) return -1;
    cx_co->uc_stack.ss_sp = stack;
    cx_co->uc_stack.ss_size = (size_t)1 << 16;
    cx_co->uc_link = cx_main;
    makecontext(cx_co, cx_body, 0);
    swapcontext(cx_main, cx_co);
    free(stack);
    free(cx_co);
    jmp_buf a;
    if (setjmp(a) == 0) longjmp(a, 1);
    free(cx_main);
    return 0;
}

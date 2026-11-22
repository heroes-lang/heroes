/* Panel 255's reproducers for defect 2679, written by the coordinator on
 * 2026-11-22 from the issue's ten lines and the shapes `2679-question.md`
 * names (lane-core's own files were not on disk that day). Each shape saves
 * into a buffer, gives the buffer's memory back, and then reaches the
 * runtime's next save point or jump. A correct C program, each one. */
#ifndef SHAPES_2679_H
#define SHAPES_2679_H
#include <setjmp.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#if defined(__linux__)
#include <ucontext.h>
#endif

/* 1. A heap buffer saved into and freed, then a save (the issue's own). */
static inline int shape_heap(void) {
    jmp_buf *heap = malloc(sizeof *heap);
    jmp_buf local;
    if (heap == NULL) return -1;
    int first = setjmp(*heap);
    free(heap);
    return first + setjmp(local);
}

/* 2. A page mapped, saved into, unmapped, then a save. */
static inline int shape_mmap(void) {
    size_t page = (size_t)sysconf(_SC_PAGESIZE);
    void *p = mmap(NULL, page, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    jmp_buf local;
    if (p == MAP_FAILED) return -1;
    int first = setjmp(*(jmp_buf *)p);
    munmap(p, page);
    return first + setjmp(local);
}

/* 3. A heap context filled by `getcontext` and freed, then a save (Linux). */
static inline int shape_context(void) {
#if defined(__linux__)
    ucontext_t *c = malloc(sizeof *c);
    jmp_buf local;
    if (c == NULL) return -1;
    if (getcontext(c) != 0) return -1;
    free(c);
    return setjmp(local);
#else
    return -2;
#endif
}

/* 4. A stack buffer whose frame returned, then a save in another frame. */
static inline __attribute__((noinline)) int shape_stack_inner(void) {
    jmp_buf inner;
    return setjmp(inner);
}
static inline int shape_stack(void) {
    int a = shape_stack_inner();
    jmp_buf local;
    return a + setjmp(local);
}

/* 5 and 6. A handler the program's C installed, raised synchronously; before
 * the raise a buffer saved into and given back (freed, or unmapped); the
 * handler jumps out through a static buffer saved first. The jump is the
 * runtime's first save point or jump after the buffer was given back. */
static jmp_buf shape_out;
static void shape_leave(int sig) {
    (void)sig;
    longjmp(shape_out, 1);
}
static inline int shape_install(void) {
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = shape_leave;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_NODEFER;
    return sigaction(SIGUSR1, &sa, NULL);
}
static inline int shape_handler_heap(void) {
    jmp_buf *heap = malloc(sizeof *heap);
    if (heap == NULL || shape_install() != 0) return -1;
    if (setjmp(shape_out) == 0) {
        (void)setjmp(*heap);
        free(heap);
        raise(SIGUSR1);
        return -3;
    }
    return 1;
}
static inline int shape_handler_mmap(void) {
    size_t page = (size_t)sysconf(_SC_PAGESIZE);
    void *p = mmap(NULL, page, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    if (p == MAP_FAILED || shape_install() != 0) return -1;
    if (setjmp(shape_out) == 0) {
        (void)setjmp(*(jmp_buf *)p);
        munmap(p, page);
        raise(SIGUSR1);
        return -3;
    }
    return 1;
}

/* 7 and 8 (added after the critic's first reading, its finding 2). A static
 * buffer saved first; then a buffer saved into and given back (freed, or
 * unmapped); then a jump to the static buffer with NO handler anywhere, as
 * libpng's own error path jumps. */
static jmp_buf shape_home;
static inline int shape_plain_heap(void) {
    jmp_buf *heap = malloc(sizeof *heap);
    if (heap == NULL) return -1;
    if (setjmp(shape_home) == 0) {
        (void)setjmp(*heap);
        free(heap);
        longjmp(shape_home, 1);
    }
    return 1;
}
static inline int shape_plain_mmap(void) {
    size_t page = (size_t)sysconf(_SC_PAGESIZE);
    void *p = mmap(NULL, page, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    if (p == MAP_FAILED) return -1;
    if (setjmp(shape_home) == 0) {
        (void)setjmp(*(jmp_buf *)p);
        munmap(p, page);
        longjmp(shape_home, 1);
    }
    return 1;
}
#endif

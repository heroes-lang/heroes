/* Lane p261-beside, 2026-11-26 (the draft d-p261-long-long): callbacks whose
 * integers the header writes with C's words of the same width and sign as
 * the program's `i64` and `u64`, `int64_t` and `uint64_t`, which are `long`
 * and `unsigned long` on Linux and `long long` and `unsigned long long` on
 * Darwin, and C functions calling each callback, as sqlite3_update_hook's
 * library does. */
#ifndef HERO_P261_INTEGER_WORDS_H
#define HERO_P261_INTEGER_WORDS_H
#include <stddef.h>
static inline long long call_ll(long long (*f)(long long), long long x) { return f(x); }
static inline unsigned long long call_ull(unsigned long long (*f)(unsigned long long), unsigned long long x) { return f(x); }
static inline size_t call_size(size_t (*f)(size_t), size_t x) { return f(x); }
static inline void call_hook(void (*f)(void *, int, const char *, const char *, long long), void *data) { f(data, 18, "main", "t", 9007199254740993LL); }
#endif

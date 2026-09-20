/* Defect 072's C side: TWO allocator families that both hand back `void *`,
 * which is SQLite's own shape — `sqlite3_malloc`/`sqlite3_free` beside
 * `malloc`/`free`. The header declares no type for either, so neither family
 * has a tag to differ by, and before panel 170's narrowing both had to be
 * `record X tag void` — one Heroes type, and the crossed free an ordinary
 * call.
 *
 * `arena_free` deliberately does not free, so the crossed program's damage is
 * a destroyed arena and a leaked block rather than a trap: the case asserts
 * the REFUSAL, and the refusal is the point. */
#include <stdio.h>
#include <stdlib.h>

static void *arena_base = 0;

static inline void *arena_new(void) {
    if (!arena_base) arena_base = malloc(4096);
    return arena_base;
}

static inline void arena_free(void *p) { printf("arena_free %s\n", p == arena_base ? "its own" : "SOMEBODY ELSE'S"); }

static inline void *heap_new(void) { return malloc(8); }

static inline void heap_free(void *p) { free(p); }

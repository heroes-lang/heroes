/* Issue 1336 (panel 178, M-buildable-structs), 2026-10-14: glibc's
   `pthread_mutex_t` is a typedef of an anonymous union, and `tag
   pthread_mutex_t` names a struct no header declares. `ffi_unknown_tag`
   names the spelling that binds it, the typedef's own name with no `tag`
   (defect 145, `unsupported/fixedbugs-145-a-typedef-of-an-anonymous-union-
   is-not-a-tag`), where panel 178 found it sending the program to a handle
   that aborts. These are that spelling's two shapes, run: a `partial` record
   whose size stays C's, handed to C, which writes every byte of it. */
#include <stdint.h>
#include <stddef.h>
#include <string.h>

/* glibc's mutex: a typedef of an anonymous union whose widest member is a
   byte array no program names. */
typedef union { char __size[40]; long __align; } lock_t;
static inline int32_t lock_init(lock_t *m) { memset(m->__size, 0x5a, sizeof m->__size); return 0; }
static inline int64_t lock_marked(const lock_t *m) { int64_t k = 0; for (size_t i = 0; i < sizeof m->__size; i++) k += m->__size[i] == 0x5a; return k; }

/* the shape beside it, glibc's `regex_t`: a typedef of an anonymous struct
   with bytes past the field the program names. */
typedef struct { size_t re_nsub; char __opaque[56]; } pattern_t;
static inline int32_t pattern_init(pattern_t *p) { p->re_nsub = 3; memset(p->__opaque, 0x5a, sizeof p->__opaque); return 0; }
static inline int64_t pattern_marked(const pattern_t *p) { int64_t k = 0; for (size_t i = 0; i < sizeof p->__opaque; i++) k += p->__opaque[i] == 0x5a; return k; }

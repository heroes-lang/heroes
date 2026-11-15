/* The package `pk`'s own header, asking after its own data file with
   `__has_embed`, which clang 19 and later list whole in `-M` (defect 2555);
   a clang before 19 knows no `__has_embed`, lists nothing, and answers as
   one that finds the file, so the case reads the same at the floor. */
#include <stdint.h>
#if defined(__has_embed)
#if __has_embed("data.bin")
#define PK_FOUND 2
#else
#define PK_FOUND 1
#endif
#if __has_embed("missing.bin")
#define PK_MISSING 2
#else
#define PK_MISSING 1
#endif
#else
#define PK_FOUND 2
#define PK_MISSING 1
#endif

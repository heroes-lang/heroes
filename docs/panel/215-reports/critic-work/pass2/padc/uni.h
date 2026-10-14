/* Compiler-engineer, panel 215: an anonymous union whose arm the program names
   (fd) is narrower than the arm C writes (h). C sets h, then reads it back
   through a second lend. */
#include <stdint.h>
struct s { int32_t kind; union { int32_t fd; void *h; }; };
static inline void set_h(struct s *p) { p->h = (void *)(uintptr_t)0x1122334455667788ull; }
static inline int64_t get_h(const struct s *p) { return (int64_t)(uintptr_t)p->h; }

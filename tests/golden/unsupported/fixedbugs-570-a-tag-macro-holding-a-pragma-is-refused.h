/* Defect 570, 2026-10-10: a tag macro that holds a pragma. */
#include <stdlib.h>
struct real_s { int x; };
static inline int use_s(struct real_s *p) { return p == 0; }
#define real_s _Pragma("clang diagnostic ignored \"-Wsign-conversion\"") real_s

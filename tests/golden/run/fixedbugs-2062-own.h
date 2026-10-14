/* Defect 2062, 2026-10-14: the header of the program's own the note drafts,
 * which includes the group's and reaches the union's member; the group names
 * it in that header's place, since one with no guard is refused read twice. */
#include "fixedbugs-2062-tagged.h"
static inline int tagged_fd(const struct tagged *t) { return t->via.fd; }
static inline void tagged_set_fd(struct tagged *t, int fd) { t->via.fd = fd; }

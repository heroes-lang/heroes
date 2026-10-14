# ffi-pragmatist, panel 215: shared by the route patches. Inserted after heroes_standard.h:
# the record's gaps, computed by clang from the header's own struct (offsetof, sizeof), never by hand.
our $GAPS = <<'C';
#include <stddef.h>
#include <string.h>
/* route's helper: zero the bytes of struct cd no member covers, in place */
#define HERO_ZERO_GAPS_CD(p) memset((char *)(p) + offsetof(struct cd, c) + sizeof((p)->c), 0, \
    offsetof(struct cd, d) - (offsetof(struct cd, c) + sizeof((p)->c)))
C
1;

/* C's ANSWER points into the bytes it was handed: `sqlite3_prepare_v2`'s
 * `*pzTail` shape (sqlite3.h:4472), which panel 124's ffi seat filed as the
 * second half of defect 024. Here it is the byte after the first dash. */
#include <string.h>
static void after_dash(const char *s, const char **tail) { *tail = strchr(s, '-') + 1; }

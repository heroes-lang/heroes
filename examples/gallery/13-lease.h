/* A C side that KEEPS the text it is handed and reads it later. Libraries do
 * this — SQLite's SQLITE_STATIC, libcurl's CURLOPT_POSTFIELDS — and the header
 * cannot say so: both a copying and a keeping parameter are `const char *`. */
#include <string.h>
static const char *label_kept_by_c;
static void keep_label(const char *s) { label_kept_by_c = s; }
static size_t kept_label_length(void) { return strlen(label_kept_by_c); }

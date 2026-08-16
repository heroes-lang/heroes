/* Beside the case, so no leg skips it for a missing package. */
#include <stdint.h>
struct ops { int32_t version; };
static const struct ops OPS = { 7 };
static const unsigned char BLOB[] = { 'A', 0, 'B', 'C' };

typedef struct { const struct ops *pMethods; int32_t id; } Handle;
static inline Handle get_handle(void) { Handle h = { &OPS, 3 }; return h; }
static inline const void *get_blob(void) { return (const void *)BLOB; }
static inline const char *get_name(void) { return "ziggy"; }
static inline const unsigned char *get_utext(void) { return (const unsigned char *)"stardust"; }

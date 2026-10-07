/* Beside defect 426's cases: functions a header declares with no prototype,
   the old C way, each defined here so that a binding nothing refused would
   build and run. `kr_void` is the shape beside them: a prototype that names
   no parameter, `(void)`. */
#include <stdint.h>

static int32_t kr_write();
static int32_t kr_write(p) unsigned char *p; { p[0] = 'Z'; return 1; }
static int32_t kr_double();
static int32_t kr_double(d) double d; { return (int32_t)d; }
static int32_t kr_none();
static int32_t kr_none() { return 3; }
static int32_t (*kr_pointer)() = kr_write;
static int32_t kr_void(void) { return 7; }

#include <stdbool.h>
struct pt { int x; int y; };
struct opaque;
struct kinds { char tag; double ratio; bool on; void *p; const char *name; struct pt at; struct pt pts[3]; float f[2]; struct opaque *h; long last; };

/* Panel 175: a refcounted handle, the shape of CFRetain/CFRelease,
 * g_object_ref/unref, X509_up_ref/X509_free, cairo_reference/destroy. */
#include <stdlib.h>
#include <stdio.h>
typedef struct obj { int refs; } obj;
static __attribute__((noinline)) obj *obj_new(void) { obj *o = malloc(sizeof *o); o->refs = 1; return o; }
static __attribute__((noinline)) obj *obj_ref(obj *o) { o->refs++; return o; }
static __attribute__((noinline)) void obj_unref(obj *o) { if (--o->refs == 0) { free(o); fprintf(stderr, "[C] freed at refcount 0\n"); } }

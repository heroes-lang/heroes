/* Panel 178 ffi-pragmatist: is all-zero bytes a defined VALUE of every scalar a
   group record can hold (spec § 13: a number, bool, ptr, cstr, a record, a
   fixed array), on this platform? */
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
int main(void) {
    void *p; memset(&p, 0, sizeof p); const char *c; memset(&c, 0, sizeof c); void (*f)(void); memset(&f, 0, sizeof f);
    double d; memset(&d, 0, sizeof d); float g; memset(&g, 0, sizeof g); bool b; memset(&b, 0, sizeof b);
    printf("zero bytes: void* == NULL %s, char* == NULL %s, fnptr == NULL %s, double == +0.0 %s (signbit %d), float == 0 %s, bool == false %s\n",
           p == NULL ? "yes" : "NO", c == NULL ? "yes" : "NO", f == NULL ? "yes" : "NO", d == 0.0 ? "yes" : "NO", __builtin_signbit(d), g == 0.0f ? "yes" : "NO", b == false ? "yes" : "NO");
    return 0;
}

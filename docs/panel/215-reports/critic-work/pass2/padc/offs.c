#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
struct s { int32_t kind; union { int32_t fd; void *h; }; };
struct cd { char c; double d; };
struct ocd { struct cd inner; };
struct arr { struct cd a[2]; int32_t n; };
int main(void) {
    printf("anon-union member h: %zu+%zu\n", offsetof(struct s, h), sizeof(((struct s *)0)->h));
    printf("nested inner.d: %zu\n", offsetof(struct ocd, inner.d));
    printf("array a[1].c: %zu\n", offsetof(struct arr, a[1].c));
    return 0;
}

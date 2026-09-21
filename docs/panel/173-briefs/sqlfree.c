/* Panel 173 q3: SQLite's own free on a lease-SHAPED pointer, in plain C, because
 * the Heroes checker admits no spelling that hands a `cstr` to `sqlite3_free`
 * (measured: `type_mismatch` for `ptr`, `ffi_writable_parameter` for `cstr`).
 * A lease is a 16-byte header followed by the bytes, and C receives the bytes'
 * address; so this frees block+16 through sqlite3_free, and block+16 through
 * libc's free beside it, and says which signal and which line each produces.
 * sqlite3_initialize() first: without it sqlite3_free calls a null xSize and
 * the 139 measured on the first attempt was that, not the allocator. */
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char **argv) {
    int which = argc > 1 ? atoi(argv[1]) : 0;
    sqlite3_initialize();
    char *block = malloc(16 + 8);
    memset(block, 0, 24);
    strcpy(block + 16, "payload");
    fprintf(stderr, "sqlite %s which %d\n", sqlite3_libversion(), which);
    if (which == 0) sqlite3_free(block + 16);   /* the lease's bytes through SQLite */
    if (which == 1) free(block + 16);           /* the same bytes through libc */
    if (which == 2) { char *own = sqlite3_malloc(8); sqlite3_free(own); sqlite3_free(own); } /* SQLite double free */
    fprintf(stderr, "after\n");
    return 0;
}

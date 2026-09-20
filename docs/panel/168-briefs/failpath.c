/* Panel 168, ffi-pragmatist, experiment 3: the destructor fires when the bind
 * FAILS. sqlite3.h says so in the SDK header read today, lines 4891-4894:
 *
 *   "^ (1) A destructor to dispose of the BLOB or string after SQLite has
 *    finished with it may be passed. ^It is called to dispose of the BLOB or
 *    string even if the call to the bind API fails, except the destructor is
 *    not called if the third parameter is a NULL pointer or the fourth
 *    parameter is negative."
 *
 * A give-away spelling must therefore waive the release on the FAILURE path
 * too, and a Heroes program that inspects the result code and "recovers" is
 * holding a freed pointer. Bind at parameter index 2 on a statement with one
 * parameter: SQLITE_RANGE.
 *
 * usage: ./failpath <endlease>
 */
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int main(int argc, char **argv) {
    setvbuf(stdout, NULL, _IONBF, 0);
    int endlease = argc > 1 ? atoi(argv[1]) : 1;
    printf("failpath endlease=%d sqlite=%s\n", endlease, sqlite3_libversion());

    sqlite3 *db = NULL;
    sqlite3_open(":memory:", &db);
    sqlite3_exec(db, "create table t(b blob)", NULL, NULL, NULL);
    sqlite3_stmt *ins = NULL;
    sqlite3_prepare_v2(db, "insert into t values (?1)", -1, &ins, NULL);

    /* the block is the library's own, so the allocator is not the variable */
    char *p = sqlite3_malloc(9);
    memcpy(p, "\x0A\x14\x1E\x28\x32\x3C\x46\x50", 8);
    p[8] = '\0';
    printf("  bound bytes at %p\n", (void *)p);

    /* parameter index 2 on a one-parameter statement */
    int rc = sqlite3_bind_blob(ins, 2, p, 8, (void (*)(void *))sqlite3_free);
    printf("  bind rc=%d (SQLITE_RANGE=%d, SQLITE_OK=%d)\n", rc, SQLITE_RANGE, SQLITE_OK);
    printf("  the program now thinks it still owns the buffer\n");

    if (endlease) {
        printf("  reading the buffer back after the failed bind...\n");
        printf("  first byte = %02X\n", (unsigned char)p[0]);
        printf("  freeing it, as a program that saw an error code would...\n");
        sqlite3_free(p);
        printf("  freed\n");
    }
    sqlite3_finalize(ins);
    sqlite3_close(db);
    printf("  survived\n");
    return 0;
}

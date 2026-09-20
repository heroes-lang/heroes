/* Panel 168, ffi-pragmatist, experiment 4: route A's OWN program, in C.
 *
 * A field lease copies a fixed-array field's bytes out of an `extern` record
 * into a block the program owns. The real header is <dirent.h>, whose
 * `struct dirent` carries `char d_name[1024]` on Darwin; the real consumer is
 * libsqlite3, which keeps the pointer under SQLITE_STATIC.
 *
 * The question the sitting asks: does this need the allocation base?
 *
 * usage: ./fieldlease <layout>   0 = leading (runtime today), 1 = trailing
 */
#include <dirent.h>
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct {
    uint64_t magic;
    int64_t len;
} HeroHeldHeader;
#define HERO_HELD_MAGIC UINT64_C(0x4845524f48454c44)

static const char *lead_hold(const char *bytes, int64_t n) {
    HeroHeldHeader *h = malloc(sizeof(HeroHeldHeader) + (size_t)n + 1);
    if (h == NULL) abort();
    h->magic = HERO_HELD_MAGIC;
    h->len = n;
    char *b = (char *)(void *)(h + 1);
    memcpy(b, bytes, (size_t)n);
    b[n] = '\0';
    return b;
}
static void lead_release(const char **slot) {
    const char *p = *slot;
    HeroHeldHeader *h =
        (HeroHeldHeader *)(void *)((char *)(void *)(uintptr_t)p - sizeof(HeroHeldHeader));
    if (h->magic != HERO_HELD_MAGIC) { fprintf(stderr, "panic\n"); abort(); }
    h->magic = 0;
    *slot = NULL;
    free(h);
}
static size_t pad(int64_t n) {
    size_t raw = (size_t)n + 1, a = _Alignof(HeroHeldHeader);
    return (raw + a - 1) / a * a;
}
static const char *trail_hold(const char *bytes, int64_t n) {
    char *b = malloc(pad(n) + sizeof(HeroHeldHeader));
    if (b == NULL) abort();
    memcpy(b, bytes, (size_t)n);
    b[n] = '\0';
    HeroHeldHeader *h = (HeroHeldHeader *)(void *)(b + pad(n));
    h->magic = HERO_HELD_MAGIC;
    h->len = n;
    return b;
}
static void trail_release_n(const char **slot, int64_t n) {
    const char *p = *slot;
    HeroHeldHeader *h = (HeroHeldHeader *)(void *)((char *)(void *)(uintptr_t)p + pad(n));
    if (h->magic != HERO_HELD_MAGIC) { fprintf(stderr, "panic\n"); abort(); }
    h->magic = 0;
    *slot = NULL;
    free((void *)(uintptr_t)p);
}

int main(int argc, char **argv) {
    setvbuf(stdout, NULL, _IONBF, 0);
    int layout = argc > 1 ? atoi(argv[1]) : 0;
    printf("fieldlease layout=%d  sizeof(struct dirent)=%zu  d_name is char[%zu]\n",
           layout, sizeof(struct dirent), sizeof(((struct dirent *)0)->d_name));

    sqlite3 *db = NULL;
    sqlite3_open(":memory:", &db);
    sqlite3_exec(db, "create table f(name text)", NULL, NULL, NULL);
    sqlite3_stmt *ins = NULL;
    sqlite3_prepare_v2(db, "insert into f values (?1)", -1, &ins, NULL);

    DIR *d = opendir("/usr/lib");
    if (d == NULL) { printf("  cannot open /usr/lib\n"); return 2; }
    int rows = 0;
    for (struct dirent *e = readdir(d); e != NULL && rows < 3; e = readdir(d)) {
        if (e->d_name[0] == '.') continue;
        /* THE FIELD LEASE: the fixed-array field's bytes, copied out */
        int64_t n = (int64_t)strlen(e->d_name);
        const char *p = layout == 0 ? lead_hold(e->d_name, n) : trail_hold(e->d_name, n);

        /* SQLITE_STATIC: the library KEEPS the pointer until finalize */
        int rc = sqlite3_bind_text(ins, 1, p, (int)n, SQLITE_STATIC);
        if (rc != SQLITE_OK) { printf("  bind rc=%d\n", rc); return 2; }
        rc = sqlite3_step(ins);
        sqlite3_reset(ins);
        sqlite3_clear_bindings(ins); /* the library lets the pointer go here */
        printf("  bound %s (step rc=%d)\n", p, rc);

        /* the end_lease the language requires */
        if (layout == 0) lead_release(&p);
        else trail_release_n(&p, n);
        rows++;
    }
    closedir(d);
    sqlite3_finalize(ins);

    sqlite3_stmt *sel = NULL;
    sqlite3_prepare_v2(db, "select count(*), max(length(name)) from f", -1, &sel, NULL);
    if (sqlite3_step(sel) == SQLITE_ROW)
        printf("  stored %d rows, longest name %d\n", sqlite3_column_int(sel, 0),
               sqlite3_column_int(sel, 1));
    sqlite3_finalize(sel);
    sqlite3_close(db);
    printf("  survived\n");
    return 0;
}

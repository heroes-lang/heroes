/* Panel 168, ffi-pragmatist: the FULL sequence a Heroes program runs, against
 * the real sqlite3.h and the real libsqlite3, under both header layouts and
 * all three retention modes.
 *
 * usage: ./bind_blob <layout> <mode> <endlease>
 *   layout 0 = leading   (runtime today, hero_str_held)
 *   layout 1 = trailing  (panel 167 clause 1)
 *   layout 2 = sqlite3_malloc, no Heroes header at all (the control)
 *   mode   0 = SQLITE_TRANSIENT
 *   mode   1 = SQLITE_STATIC
 *   mode   2 = sqlite3_free as the destructor
 *   endlease 0 = skip end_lease (a hypothetical waiver), 1 = pay it
 */
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* --- the runtime's header, copied from runtime/heroes_runtime.h:155-160 --- */
typedef struct {
    uint64_t magic;
    int64_t len;
} HeroHeldHeader;
#define HERO_HELD_MAGIC UINT64_C(0x4845524f48454c44)

static void hero_panic(const char *m) {
    fprintf(stderr, "panic: %s\n", m);
    abort();
}

/* --- leading: hero_str_held / hero_held_release, runtime/parts/str.c:419-470 */
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
    if (slot == NULL) hero_panic("release with no cell — this is a compiler bug, please report it");
    const char *p = *slot;
    if (p == NULL) hero_panic("end_lease of a lease that is already over");
    HeroHeldHeader *h =
        (HeroHeldHeader *)(void *)((char *)(void *)(uintptr_t)p - sizeof(HeroHeldHeader));
    if (h->magic != HERO_HELD_MAGIC)
        hero_panic("end_lease of a pointer no `.lease()` made — the checker admits "
                   "`end_lease` only on a lease cell, so this is a compiler bug, "
                   "please report it");
    h->magic = 0;
    *slot = NULL;
    free(h);
}

/* --- trailing: panel 167 clause 1, released with the length in hand -------- */
static size_t pad(int64_t n) {
    size_t raw = (size_t)n + 1;
    size_t a = _Alignof(HeroHeldHeader);
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
    if (slot == NULL) hero_panic("release with no cell");
    const char *p = *slot;
    if (p == NULL) hero_panic("end_lease of a lease that is already over");
    HeroHeldHeader *h = (HeroHeldHeader *)(void *)((char *)(void *)(uintptr_t)p + pad(n));
    if (h->magic != HERO_HELD_MAGIC)
        hero_panic("end_lease of a pointer no `.lease()` made — the checker admits "
                   "`end_lease` only on a lease cell, so this is a compiler bug, "
                   "please report it");
    h->magic = 0;
    *slot = NULL;
    free((void *)(uintptr_t)p);
}

/* --- the control: the library's own allocator, no Heroes header ----------- */
static const char *sqlite_hold(const char *bytes, int64_t n) {
    char *b = sqlite3_malloc((int)n + 1);
    if (b == NULL) abort();
    memcpy(b, bytes, (size_t)n);
    b[n] = '\0';
    return b;
}

static void die(sqlite3 *db, const char *what) {
    fprintf(stderr, "%s: %s\n", what, sqlite3_errmsg(db));
    exit(2);
}

int main(int argc, char **argv) {
    const char *src = "\x0A\x14\x1E\x28\x32\x3C\x46\x50";
    const int64_t n = 8;
    int layout = argc > 1 ? atoi(argv[1]) : 0;
    int mode = argc > 2 ? atoi(argv[2]) : 0;
    int endlease = argc > 3 ? atoi(argv[3]) : 1;

    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    printf("layout=%d mode=%d endlease=%d sqlite=%s\n", layout, mode, endlease,
           sqlite3_libversion());

    sqlite3 *db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) die(db, "open");
    if (sqlite3_exec(db, "create table t(b blob)", NULL, NULL, NULL) != SQLITE_OK)
        die(db, "create");

    sqlite3_stmt *ins = NULL;
    if (sqlite3_prepare_v2(db, "insert into t values (?1)", -1, &ins, NULL) != SQLITE_OK)
        die(db, "prepare");

    /* 1. the lease */
    const char *p = layout == 0   ? lead_hold(src, n)
                    : layout == 1 ? trail_hold(src, n)
                                  : sqlite_hold(src, n);

    /* 2. the bind, the retention mode being argument five */
    void (*dtor)(void *) = mode == 0   ? SQLITE_TRANSIENT
                           : mode == 1 ? SQLITE_STATIC
                                       : (void (*)(void *))sqlite3_free;
    int rc = sqlite3_bind_blob(ins, 1, p, (int)n, dtor);
    printf("  bind rc=%d\n", rc);
    if (rc != SQLITE_OK) die(db, "bind");

    /* 3. the step */
    rc = sqlite3_step(ins);
    printf("  step rc=%d (done=%d)\n", rc, SQLITE_DONE);

    /* 4. the finalize — this is where SQLite calls the destructor */
    sqlite3_finalize(ins);
    printf("  finalized\n");

    /* 5. read it back, so a wrong answer is visible and not only a crash */
    sqlite3_stmt *sel = NULL;
    if (sqlite3_prepare_v2(db, "select b from t", -1, &sel, NULL) != SQLITE_OK)
        die(db, "prepare select");
    if (sqlite3_step(sel) == SQLITE_ROW) {
        const unsigned char *b = sqlite3_column_blob(sel, 0);
        int len = sqlite3_column_bytes(sel, 0);
        printf("  stored %d bytes: ", len);
        for (int i = 0; i < len; i++) printf("%02X", b[i]);
        printf("  (honest 0A141E28323C4650)\n");
    } else {
        printf("  no row\n");
    }
    sqlite3_finalize(sel);
    sqlite3_close(db);

    /* 6. the end_lease the language requires */
    if (endlease) {
        printf("  end_lease...\n");
        fflush(stdout);
        if (layout == 0) lead_release(&p);
        else if (layout == 1) trail_release_n(&p, n);
        else sqlite3_free((void *)(uintptr_t)p);
        printf("  released\n");
    } else {
        printf("  end_lease SKIPPED\n");
    }
    printf("  survived\n");
    return 0;
}

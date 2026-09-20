/* Panel 168, ffi-pragmatist, experiment 2: is `sqlite3_free` `free`?
 *
 * The trailing header makes the give-away case work ONLY IF the library's
 * freer is the same allocator the Heroes runtime allocated from. SQLite says
 * in its own header (sqlite3.h:1879-1891) that the allocator is replaceable
 * through sqlite3_config(SQLITE_CONFIG_MALLOC, ...). If a replaced allocator
 * breaks the trailing layout, then "the pointer C receives is the allocation
 * base" is not sufficient: it must be the base of THE RIGHT HEAP.
 *
 * usage: ./allocator <custom> <layout>
 *   custom 0 = the default allocator, 1 = a private one with its own header
 *   layout 1 = trailing (panel 167), 2 = sqlite3_malloc (the control)
 */
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

/* A private allocator of exactly the shape SQLITE_CONFIG_MALLOC takes. It puts
 * its OWN eight-byte size word in front, which is what memsys3/memsys5 and
 * every arena allocator in the wild do. */
#define MY_OFF 16
static void *my_malloc(int n) {
    char *raw = malloc((size_t)n + MY_OFF);
    if (raw == NULL) return NULL;
    *(int64_t *)(void *)raw = n;
    return raw + MY_OFF;
}
static void my_free(void *p) {
    if (p == NULL) return;
    char *raw = (char *)p - MY_OFF;
    int64_t n = *(int64_t *)(void *)raw;
    if (n < 0 || n > (1 << 24)) {
        fprintf(stderr, "  my_free: size word reads %lld — this is not my block\n",
                (long long)n);
        abort();
    }
    free(raw);
}
static void *my_realloc(void *p, int n) {
    if (p == NULL) return my_malloc(n);
    char *raw = (char *)p - MY_OFF;
    char *out = realloc(raw, (size_t)n + MY_OFF);
    if (out == NULL) return NULL;
    *(int64_t *)(void *)out = n;
    return out + MY_OFF;
}
static int my_size(void *p) { return (int)*(int64_t *)(void *)((char *)p - MY_OFF); }
static int my_roundup(int n) { return (n + 7) & ~7; }
static int my_init(void *x) { (void)x; return SQLITE_OK; }
static void my_shutdown(void *x) { (void)x; }

int main(int argc, char **argv) {
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    int custom = argc > 1 ? atoi(argv[1]) : 0;
    int layout = argc > 2 ? atoi(argv[2]) : 1;
    printf("custom=%d layout=%d sqlite=%s\n", custom, layout, sqlite3_libversion());

    if (custom) {
        sqlite3_mem_methods m = {my_malloc, my_free,  my_realloc, my_size,
                                 my_roundup, my_init, my_shutdown, NULL};
        int rc = sqlite3_config(SQLITE_CONFIG_MALLOC, &m);
        printf("  sqlite3_config(SQLITE_CONFIG_MALLOC) rc=%d\n", rc);
        if (rc != SQLITE_OK) {
            printf("  the build refuses a replaced allocator; nothing further is proved\n");
            return 3;
        }
    }

    sqlite3 *db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) { printf("  open failed\n"); return 2; }
    if (sqlite3_exec(db, "create table t(b blob)", NULL, NULL, NULL) != SQLITE_OK) {
        printf("  create failed: %s\n", sqlite3_errmsg(db)); return 2;
    }
    sqlite3_stmt *ins = NULL;
    if (sqlite3_prepare_v2(db, "insert into t values (?1)", -1, &ins, NULL) != SQLITE_OK) {
        printf("  prepare failed\n"); return 2;
    }

    const char *src = "\x0A\x14\x1E\x28\x32\x3C\x46\x50";
    const int64_t n = 8;
    const char *p = layout == 1 ? trail_hold(src, n) : NULL;
    if (layout != 1) {
        char *b = sqlite3_malloc((int)n + 1);
        memcpy(b, src, (size_t)n); b[n] = '\0'; p = b;
    }

    /* the give-away: the library frees it, the program does not */
    int rc = sqlite3_bind_blob(ins, 1, p, (int)n, (void (*)(void *))sqlite3_free);
    printf("  bind rc=%d\n", rc);
    rc = sqlite3_step(ins);
    printf("  step rc=%d\n", rc);
    printf("  finalizing (the destructor fires here)...\n");
    sqlite3_finalize(ins);
    printf("  finalized\n");
    sqlite3_close(db);
    printf("  survived\n");
    return 0;
}

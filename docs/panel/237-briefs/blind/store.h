/* store.h: a small store of notes, the whole library in this header. */
#ifndef STORE_H
#define STORE_H
#include <stdio.h>
#include <stdlib.h>

#define STORE_OK 0
#define STORE_BUSY 5

typedef struct store { int cursors; } store;
typedef struct cursor { store *owner; } cursor;

static int store_open_count;

/* Opens a store; *out receives it. Returns STORE_OK. */
static inline int store_open(const char *name, store **out) {
    (void)name;
    *out = calloc(1, sizeof(store));
    store_open_count++;
    return STORE_OK;
}

/* Opens a cursor over the store; *out receives it. Returns STORE_OK. */
static inline int cursor_open(store *s, cursor **out) {
    *out = calloc(1, sizeof(cursor));
    (*out)->owner = s;
    s->cursors++;
    return STORE_OK;
}

/* Closes a cursor. Returns STORE_OK. */
static inline int cursor_close(cursor *c) {
    c->owner->cursors--;
    free(c);
    return STORE_OK;
}

/* Closes the store and returns STORE_OK; while a cursor is open it closes
   nothing and returns STORE_BUSY, and the store stays open. */
static inline int store_close(store *s) {
    if (s->cursors > 0) return STORE_BUSY;
    free(s);
    store_open_count--;
    return STORE_OK;
}

__attribute__((destructor)) static void store_report(void) {
    FILE *o = fopen("stores.txt", "w");
    if (o) { fprintf(o, "%d\n", store_open_count); fclose(o); }
}
#endif

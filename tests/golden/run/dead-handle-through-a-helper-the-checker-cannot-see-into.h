/* Panel 177: a three-function cJSON in miniature over a static pool, the
 * shape of defect 088 (the critic of panel 176). `obj_add` links `item`
 * under `object`; `obj_delete` frees a node and its children. Nothing is
 * `malloc`ed, so the case fires the same way on all three platforms and a
 * write into a deleted node lands in the pool, where `obj_count` can see it. */
#include <stdint.h>

typedef struct Obj Obj;

struct Obj {
    Obj *child;
    Obj *next;
    int64_t live;
};

static inline __attribute__((noinline)) Obj *obj_new(void) {
    static Obj pool[8];
    static int64_t next = 0;
    Obj *o = &pool[next++];
    o->child = 0;
    o->next = 0;
    o->live = 1;
    return o;
}

static inline __attribute__((noinline)) int64_t obj_add(Obj *object, const char *string, Obj *item) {
    (void)string;
    item->next = object->child;
    object->child = item;
    return 1;
}

static inline __attribute__((noinline)) void obj_delete(Obj *item) {
    while (item != 0) {
        Obj *next = item->next;
        if (item->child) obj_delete(item->child);
        item->live = 0;
        item->child = 0;
        item = next;
    }
}

static inline __attribute__((noinline)) int64_t one(void) { return 1; }

/* Defect 2518, 2026-11-13: functions whose pointer parameter points at no
 * number, a struct as glibc's `sigset_t` is, a union and an array, each of a
 * size no Heroes number has. */
typedef struct {
    unsigned long v[2];
} st2518_t;

typedef union {
    long long a;
    double b[2];
} un2518_t;

typedef int quad2518_t[4];

static inline int st2518_fill(st2518_t *p) {
    p->v[0] = 1;
    return 0;
}

static inline int un2518_fill(un2518_t *p) {
    p->a = 1;
    return 0;
}

static inline int quad2518_fill(quad2518_t *p) {
    (*p)[0] = 1;
    return 0;
}

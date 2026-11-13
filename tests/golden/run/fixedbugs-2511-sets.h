/* Defect 2511, 2026-11-13: functions, and beside each a macro of its name that
 * assigns through its argument, as Darwin's signal.h defines `sigemptyset`.
 * Each function answers -1 for a bit outside the set, where its macro does
 * not check; the answer says which of the two a call reached. */
typedef unsigned int set2511_t;

static inline int empty2511(set2511_t *s) {
    *s = 0u;
    return 0;
}

static inline int add2511(set2511_t *s, int n) {
    if (n < 1 || n > 32) {
        return -1;
    }
    *s |= 1u << (n - 1);
    return 0;
}

static inline int has2511(const set2511_t *s, int n) {
    if (n < 1 || n > 32) {
        return -1;
    }
    return (int)((*s >> (n - 1)) & 1u);
}

#define empty2511(s) (*(s) = 0, 0)
#define add2511(s, n) (*(s) |= 1u << ((n) - 1), 0)
#define has2511(s, n) ((int)((*(s) >> ((n) - 1)) & 1u))

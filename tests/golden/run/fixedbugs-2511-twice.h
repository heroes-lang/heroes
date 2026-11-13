/* Defect 2511, 2026-11-13: a function, and a macro of its name that reads its
 * argument twice and means another value. */
static inline int twice2511(int x) {
    return x;
}

#define twice2511(x) ((x) + (x))

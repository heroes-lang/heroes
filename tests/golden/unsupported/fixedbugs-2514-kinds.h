/* Defect 2514, 2026-11-13: typedefs of types that are no struct, one per kind
 * a group record named after it was told the header kept opaque, and a struct
 * the header keeps to itself, whose record that note stays true of. The same
 * on every platform, as Darwin's `sigset_t` is not. */
typedef unsigned int mask2514_t;
typedef struct hidden2514 *handle2514_t;
typedef int quad2514_t[4];
typedef struct kept2514 kept2514_t;

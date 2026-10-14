/* ffi-pragmatist, panel 215: a C callback that receives a padded record BY VALUE, and a C
   function that takes and returns one by value. */
struct cd { char c; double d; };
typedef int (*cd_cb)(struct cd);
static inline int call_cd(cd_cb f, struct cd x) { return f(x); }
static inline struct cd bump_cd(struct cd x) { x.c = (char)(x.c + 1); return x; }

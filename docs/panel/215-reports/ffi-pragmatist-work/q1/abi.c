/* ffi-pragmatist, panel 215: how each target lowers a padded struct passed and returned by value. */
struct cd { char c; double d; };   /* 7 bytes of padding after c */
struct ci { char c; int i; };      /* 3 bytes */
struct fc { float f; char c; };    /* 3 trailing bytes */
struct big { char c; double d[3]; }; /* 32 bytes: memory class on x86-64 */
struct cd pass_cd(struct cd x) { return x; }
struct ci pass_ci(struct ci x) { return x; }
struct fc pass_fc(struct fc x) { return x; }
struct big pass_big(struct big x) { return x; }

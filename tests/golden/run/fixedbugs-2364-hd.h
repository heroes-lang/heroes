typedef struct Hd Hd;
static inline Hd *hd_none(void) { return (Hd *)0; }
static inline int hd_is_none(Hd *h) { return h == (Hd *)0; }

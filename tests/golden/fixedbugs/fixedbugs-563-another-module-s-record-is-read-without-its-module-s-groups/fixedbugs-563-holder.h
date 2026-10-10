typedef struct Holder { int v; } Holder;
static inline int holder_need(Need *n) { return n->v; }

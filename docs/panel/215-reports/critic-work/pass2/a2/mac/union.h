struct tagged {
    int kind;
    union {
        int fd;
        void *handle;
    } via;
};
#define fd via.fd
#define handle via.handle
static inline int tg_kind(const struct tagged *t) { return t->kind; }

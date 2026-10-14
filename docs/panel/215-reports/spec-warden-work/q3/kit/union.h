struct tagged {
    int kind;
    union {
        int fd;
        void *handle;
    } via;
};
static inline int tg_kind(const struct tagged *t) { return t->kind; }

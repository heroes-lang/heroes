- [ ] **M-separate-compilation step 7 (the cache that could not see a header)** | Before this step, this program printed `1` while `conf.h` on disk said `#define CONF_LIMIT 2`, at exit 0. The compiler was not confused about the value — it never asked. Given that `constant CONF_LIMIT: i64` in an `extern` group emits as `int64_t h_bind_CONF_LIMIT(void) { return CONF_LIMIT; }`, say why the compiler's cache key — which contains the whole emitted C, character for character — cannot see the difference between a header saying 1 and the same header saying 2

    **Where to look:** selfhost/cli/deps.hero (the module header) · selfhost/cli/units.hero:75 (the key)
    **Why it matters:** the cache key is only as good as what it can see, and this is the one thing it structurally could not

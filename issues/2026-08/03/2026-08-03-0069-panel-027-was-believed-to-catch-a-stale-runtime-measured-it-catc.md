- [ ] **panel 027** | `_Static_assert(HERO_RUNTIME_ABI == N)` was believed to catch a stale runtime. Measured: it catches a **function** change (undefined symbol at link) and **not** a struct-field change — new header, old `runtime.o`, link exit 0, a read four bytes past a 40-byte object, silence. Question: what actually protects the build today, and what would have to be true for the `_Static_assert` to earn the sentence the header's comment gives it?

    **Where to look:** runtime/heroes_runtime.h:10-12 · archive/bootstrap-rs/heroes-cli/src/commands/toolchain.rs (the cache key)
    **Why it matters:** a guard credited with a job it cannot do is worse than no guard

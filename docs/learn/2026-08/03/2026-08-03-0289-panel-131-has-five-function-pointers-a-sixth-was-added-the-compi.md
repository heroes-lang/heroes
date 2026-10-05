- [ ] **panel 131** | `HeroDesc` has five function pointers. A sixth was added, the compiler was rebuilt with the project's own flags, and it compiled at exit 0 with zero warnings. Why is that the dangerous outcome rather than the good one?

    **Where to look:** `runtime/heroes_runtime.h`, `seed/heroes.c`'s
    `static const HeroDesc` initialisers, and C11 6.7.9p21.

    **Why it matters:** the same experiment was run at panel 117 and again at
    panel 131, and both times **125 descriptors silently filled the new member
    with NULL** and the first program to read it died at address zero with no
    type and no line. The guard that should have caught it,
    `_Static_assert(HERO_RUNTIME_ABI == N)`, is blind to a struct field. This is
    the shape of a whole class: a change that compiles clean because C fills the
    gap for you.

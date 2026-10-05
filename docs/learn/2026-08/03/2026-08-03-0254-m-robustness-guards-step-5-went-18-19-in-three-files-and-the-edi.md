- [ ] **M-robustness-guards step 5** | `HERO_RUNTIME_ABI` went 18 → 19 in THREE files and the edits had to land in an order. Name the three, say which one is the seed binary's and why bumping the header first would have left the repository with no compiler that can compile anything — then say what `seed/heroes.c`'s eighth line does the day someone forgets

    **Where to look:** runtime/heroes_runtime.h:37 · selfhost/emit/decls.hero (the `_Static_assert` line) · seed/heroes.c:8 · seed/README.md § When this file must be regenerated
    **Why it matters:** the stamp is the one guard that fires at compile time rather than at run time, and it fires against the compiler you are holding

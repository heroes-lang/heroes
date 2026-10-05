- [ ] **M-isolated-threads step 4** | The check written at step 3 went red on FOUR objects that step 4's own repair had just introduced, and the author had not noticed writing them. Open `runtime/parts/alloc.c`, find the key, the slot and the two once-guards, and say for each one whether you would have called it shared mutable state — then read the allow-list entry that says why each is safe anyway

    **Where to look:** tests/harness/suite_runtime.hero § Rule 3 · runtime/parts/alloc.c
    **Why it matters:** it is the difference between a list somebody wrote and a list taken from the world, and it is the thing CLAUDE.md §1's newest rule is about

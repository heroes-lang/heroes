- [ ] **M-optional-map** | The map's seed is **fixed**, and the reason is not security or speed. Task: say what would break, and at which milestone, if the seed were taken from the clock — and why the answer is about `diff` rather than about the map

    **Where to look:** runtime/runtime.c (HERO_MAP_SEED) · docs/panel/006 · ROADMAP M-selfhost-fixpoint
    **Why it matters:** it is the one place where a hash table's usual defaults are wrong for this project specifically

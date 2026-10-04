- [ ] **M-isolated-threads.0** | **How many shapes of allocation does the runtime have, and which one would lie?** Open `runtime/parts/alloc.c` and count the entry points below `hero_malloc_raw`. Three of them hand memory out. Then answer: if `hero_eq_queue` in `parts/array.c` had been given `hero_alloc` instead of `hero_grow_kept`, what exactly would every program that compares two deep arrays print at exit — nothing, a panic naming a heap block, or a panic naming a scratch buffer?

    **Where to look:** runtime/parts/alloc.c · runtime/parts/array.c:186-215 · design.md:2210-2222
    **Why it matters:** the leak gate is the only leak instrument this platform has, and an instrument that cries wolf is one people switch off

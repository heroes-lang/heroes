- [ ] **M-isolated-threads step 3** | The leak gate's counter could have been made per-thread — it is faster and the race goes away. Step 2 measured why that is worse. A worker thread allocates a `str` and never gives it back: say what the program does under each of the two counters, and which of the two answers a person would rather get

    **Where to look:** runtime/parts/alloc.c, the comment above the two counters · DESIGN-LOG 2026-09-05 (step 2)
    **Why it matters:** the cheap repair and the right one differ by what the instrument can still SEE, which is a shape that recurs

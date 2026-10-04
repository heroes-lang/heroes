---
kind: feature
area: runtime
milestone: M-core-packages
filed: 2026-09-06
commit: none
github: none
---

- [ ] **M-core-packages** | copy-on-write's `refcount == 1` is a test and then a mutate, and nobody could race it | `runtime/parts/cow.c:44`, `:78`, `:83` · `runtime/parts/map-write.c:127` · `docs/panel/113`

    **Origin:** `docs/panel/113`, 2026-09-06; the finding is both compiling
    seats', independently, in separate checkouts, and the ffi seat's positive
    control is what makes their silence admissible. **Re-homed a second time
    2026-09-06, at M-thread-stacks' close**: that milestone ran threads and did
    not meet the return condition either, so the item moves on rather than
    expiring with the milestone that failed to close it. Its home is now the
    next milestone whose own work runs many threads for a real reason — a web
    server is one connection per thread — because this is a WATCH item and a
    watch needs traffic, not a schedule.

    **The sitting was convened on this and re-aimed itself.**
    `runtime/parts/cow.c:44`, `:78`, `:83` and `runtime/parts/map-write.c:127`
    read `refcount == 1` and then mutate; step 3 made the read whole and did not
    make the pair single. **What no seat could do is race it.** With
    `parts/thread.c`'s guard patched down in their own copies: 8 threads and
    240,000 mutations of nested `[[str]]` and `{str: i64}` under ASan and
    ThreadSanitizer, **exit 0, zero warnings**; and 4 foreign threads with
    100,000 concurrent mutating touches of one shared header, same result, for a
    `[i64]`, a `{str: [i64]}` and a `str`. **The instrument was proved live in
    the same session**: a hand-made race on `hero_array_push_owned` is `data
    race … cow.c:80`, exit 134, 3 races, length 246821 instead of 400000.

    **The reason is the grammar, not luck**: a callback parameter arrives
    BORROWED and the emitted body increfs before it can store (`t1 = h0_a;
    hero_array_incref(t1); h1_ys = t1;`), and the two ways round that are closed
    — `@` inside a function type is `error[expected_type]`, and writing a
    callback parameter is `error[not_mutable]`. **Option A, a
    compare-and-exchange from 1 to a busy sentinel, is deferred and NOT on
    cost**: measured cheap (0.37 s against 0.36 s over 40M stores) and unsound
    as scoped, because the sentinel must be held across the CALLER's mutation —
    `hero_array_set` does `drop(place); memcpy(...)` after `unshare` returns —
    so a panic inside `drop` would strand a block busy forever. **Option B is
    vetoed by both compiling seats**: 400,000 stores at 24.50 s against
    40,000,000 at 0.36 s, O(n²) and not a percentage.

    **THE RETURN CONDITION, and it is the whole item now**: a program that
    corrupts memory through those four sites **with every reference counted** —
    that is, without C releasing a reference it still lends. Produce it and A
    lands with the critical section widened to cover the caller. The searches
    that failed are named in the sitting rather than hidden, and what was NOT
    tried is named too: `sort` through a function pointer, the `eq` and `hash`
    descriptor walks, and the drop-list drain under contention.

    **Where to look also:** `docs/panel/113` § What did NOT reproduce.
    **Why it matters:** a window nobody can reach is not a repair anybody should
    ship, and the sitting's own measurements are what say so.

    **Re-verified 2026-09-10: STILL OPEN, and every citation is exact.**
    `grep -rn 'refcount == 1' runtime/parts/*.c` returns exactly the four sites the
    item names, `cow.c:44`, `:78`, `:83` and `map-write.c:127`, and `cow.c:38-43`
    still carries the sitting's note that until this lands, `parts/thread.c`'s guard
    is what keeps any other thread out. The return condition — a corrupting program
    with every reference counted — is unmet, so the watch stands. Its 240,000-mutation
    figures need threads under TSan and were not re-run.

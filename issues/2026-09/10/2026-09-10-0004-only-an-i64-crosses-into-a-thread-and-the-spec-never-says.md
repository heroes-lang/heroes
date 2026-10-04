---
kind: feature
area: runtime
milestone: M-core-packages
filed: 2026-09-10
commit: none
github: none
---

- [ ] **M-core-packages** step 6 | only an `i64` crosses into a thread, and the spec never says the word | `runtime/hero_os.h:251-256` · `selfhost/check/ffi.hero:76-92` · `design.md` Part 7.13 · `examples/threads/main.hero`

    **Origin:** author decision 2026-09-10, § What production-ready means row 6.

    **Two measurements, and the second is the one nobody had written down.**
    Concurrency arrives as `extern "hero_os.h"` with `hero_thread_spawn`,
    `hero_thread_join` and `hero_thread_limit`, where **only an `i64` crosses in
    each direction** and the checker enforces it — a `[T]` or `{K: V}` inside the
    callback's signature is `error[ffi_type]`
    (`selfhost/check/ffi.hero:76-92`). That is Part 7.13's data-parallelism rung
    exactly as designed, and `examples/threads/main.hero` says so in its own first
    paragraph: *"it is the model"*. **And `spec/heroes-spec.md` never says thread,
    concurrency, spawn or parallel** — zero occurrences — so a program written from
    the spec alone, which is the one reader this language exists for, cannot use a
    thread at all.

    **Why it is the server step's question.** `docs/ROADMAP.md` § M-web-framework
    describes *"a `Request` record … exactly the message the separate-heap model
    wants, small and copied once"*, and **no program can write that today**. One
    connection per thread needs only the descriptor, which is an `i64`, so the
    server step is where it is found out whether the rung is enough — and if it is,
    that is the answer and the spec is what changes, not the runtime.

    **What it may not do**: widen the model on an appetite. Panel 111 measured what
    an unguarded `str` across 32 threads costs — `heap-use-after-free` in nine ASan
    runs of ten — and `runtime/parts/cow.c`'s test-then-mutate is still this
    milestone's own open watch item.

---
kind: task
area: harness
milestone: M-core-packages
filed: 2026-09-10
commit: none
github: none
---

- [ ] **M-core-packages** step 6 | the long run: every program in the net exits, and a service is the one that stays up | `tests/harness/suite_corpus.hero` § configurations · `runtime/parts/alloc.c` § `hero_runtime_check_leaks` · `docs/ROADMAP.md` § M-core-packages

    **Origin:** author decision 2026-09-10, § What production-ready means. The
    finding is structural rather than a defect: `main.expected` is the shape of the
    whole net, so **every instrument here watches a program that starts, prints and
    stops**.

    **What that makes invisible** is exactly what production meets first: a
    refcount that drifts by one per request, a descriptor never closed, memory that
    grows one connection at a time. None of it is reachable by a corpus of
    one-shot programs, and this milestone's own acceptance says why — a listening
    server does not fit `main.expected`, so its loopback case is server and client
    in one process with deterministic output.

    **Two cheap additions on top of that case, not a new programme**: drive the
    loopback server for N requests rather than two, and read the allocation count
    at the end; and assert `hero_runtime_check_leaks()` after the long run rather
    than after the short one. If the count is flat across N, the class is shut for
    the shape the corpus can see. **What it is not**: an endurance suite, a soak
    farm, or anything that goes red at random — CLAUDE.md's own warning about an
    instrument nobody trusts applies here first.

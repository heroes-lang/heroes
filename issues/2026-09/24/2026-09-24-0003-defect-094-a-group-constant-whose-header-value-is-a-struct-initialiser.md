---
kind: defect
area: emit
milestone: none
filed: 2026-09-24
commit: none
github: none
---

- [ ] **094 — a group `constant` whose header value is a struct initialiser passes `check` and stops the build with `internal error`** | the emitter writes `return PT_INIT;` and a file-scope `__typeof__(PT_INIT)` probe, and `{1, 2}` is neither an expression nor a type, so clang fails and the compiler reports itself broken | `selfhost/emit/` constant emission · `.claude/rules/c-boundary.md` · `spec § 13` · **class: blocking**

    **Origin:** panel 178's compiler-engineer, 2026-09-24 (its *Found while
    measuring* 1), looking for a route by which C itself says which value is
    valid; reproduced by the coordinator the same day on all three legs before
    filing.

    **Class: blocking**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery), classed the day it reached the trunk: an exit 2 on a binding `check` accepted, where the author can be told.

    **The reproducer** (`docs/panel/178-reports/compiler-engineer-work/w/cinit/`):
    `cinit.h` is `struct pt { int x; int y; };` and `#define PT_INIT {1, 2}`;

        extern "cinit.h"
            record Pt tag pt
                x: i32
                y: i32
            constant PT_INIT: Pt

        function main()
            p = PT_INIT
            print(p.y)

    `check` **0**, `run` **2**, `internal error: compiling the generated C
    failed`, on Darwin arm64, Linux arm64 and Linux x86-64.

    **Why it is a defect.** CLAUDE.md § 7's exception: the compiler blames
    itself for a binding it accepted. And it is the one route to *which value of
    a struct is valid* where C says it rather than the binding's author: the
    same program over Darwin's `PTHREAD_MUTEX_INITIALIZER` fails the same way
    (the seat's `w/mutex_init.hero`). A compound literal, `(struct pt)PT_INIT`,
    is valid C.

    Re-run 2026-10-06 on `bef739dd`: this worktree's compiler, built from the round's seed, over the reproducer as `docs/panel/178-reports/compiler-engineer-work/w/cinit/` holds it, in a scratch directory from 11:38 by the clock: `check` 0; `run` 2 three of three, *internal error: compiling the generated C failed*, clang refusing both places the item names, `_Static_assert(HERO_RET_RECORD(PT_INIT, struct pt), ...)` with *statement expression not allowed at file scope* and `return PT_INIT;` with *expected expression*. Still broken. Linux arm64 and x86-64 unrun that day.

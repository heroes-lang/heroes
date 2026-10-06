---
kind: defect
area: emit
milestone: none
filed: 2026-09-24
commit: 41c5d1b49c6dbde2177bdd1ca40f1f6b163af1ef
github: none
---

- [x] **094 — a group `constant` whose header value is a struct initialiser passes `check` and stops the build with `internal error`** | the emitter writes `return PT_INIT;` and a file-scope `__typeof__(PT_INIT)` probe, and `{1, 2}` is neither an expression nor a type, so clang fails and the compiler reports itself broken | `selfhost/emit/` constant emission · `.claude/rules/c-boundary.md` · `spec § 13` · **class: blocking**

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

    Repaired at `41c5d1b4`, 2026-10-06, lane ffi13 of batch 12, gated by its cases and the compiler's own tests; the net is owed at the batch's close, and the platform legs before the item closes (its cases ran that day on Linux arm64, run 8 and unsupported 6, 0 failed, and on the Windows box, run 7 and unsupported 6, 0 failed, the mutex case skipped there by name for `pthread.h`).

## The repair

Repaired at `41c5d1b4`. A group `constant` of a record whose header value is a brace list (`#define PT_INIT {1, 2}`, Darwin's `PTHREAD_MUTEX_INITIALIZER`) passed `check` and stopped the build with *internal error* at exit 2, the accessor saying `return PT_INIT;` and the return assertion asking `__typeof__(PT_INIT)` at file scope. The accessor now declares the header's struct with the header's value and returns it (C11 6.7.9p13), at the constant's own line (`emit/record_constant.hero`), and that declaration is the type question; a value C will not build as written is told `ffi_constant_type` at exit 1 on the constant, in clang's words with the header's line (`emit/ffi_value.hero`), no new diagnostic code. Its cases are eight `run/fixedbugs-094-*` and six `unsupported/fixedbugs-094-*`, twelve red on the base at exit 2. Its platforms, run at batch 12's seed commit `43e6be50`: Linux arm64 in Docker (Debian clang 22.1.8), the compiler's own tests 1,299 all passed and 26 suites green, `run` 303 and `emission` 825 passed; and the Windows box (clang 23.1.1), the compiler's own tests 1,299 and the net's own 286 all passed, `run` 298, `emission` 797, `warnings` 355, `determinism` 332, `emit` 9 and `cache` 7, all 0 failed, `unsupported` 142 passed with seven cases skipped by name for headers the box lacks; on both, the one other red was `unsupported`'s floor, raised from 118 to 150 at `51f1a18c`. Lane cli12's boundary work had passed 27 of the box's 29 suites at `b3738dc9`, `records` and `unseen` reading no `.git` in an archive.

**Closed 2026-10-06** with batch 12 (lanes b12-str192, b12-parse12, b12-hook, b12-cli12, b12-fit12, b12-ir12 and b12-ffi13, merged into one round tree made from the trunk at `0f48f9f9`), its closing gate run on the round's head: the seed regenerated at `43e6be50` over two generations, the runtime's ABI moving from 26 to 27, 37,358,146 bytes, SHA-256 beginning `fc9751a29a1ecb8a`, its fixpoint by `cmp`, and the compiler's own tests 1,299, all passed; then the net's own tests 286, all passed, and the full net, 28 suites and `cache`, 5,862 passed and 0 failed (`probe` red in the parallel pass and 27 passed alone; `unsupported` asking its floor raised from 118 to 150, done at `51f1a18c`, then 151 passed). The census, the trunk's compiler at `0f48f9f9` against the round's over the tree's tracked files, each with its own runtime: `check --brief` over 2,655, 66 moved, every one attributed to a repair of the batch, 10 of them the trunk unable to read a `\u{...}` escape the round's source writes; `build --emit-c` over the 1,146 holding an `extern`, the ABI stamp moving every unit and 186 moved besides, every one attributed, the trunk's 27 runs at exit 2 all gone, and one wrong binding newly accepted filed as defect 392. Panel 187's R2, its instrument and plan rebuilt from the transcripts after the machine's restart that morning (the same plan on every recorded fact), the trunk's compiler against the round's over 13,594 single mutants and 16,041 pairs: one mutant worse in both arms, filed as defect 391, and one pair's second newly told. The site's build: 38 claims and 2 verb lists checked over 20 pages. The cost, instructions retired over the trunk's compiler source (its two raw SOH bytes removed in the measuring copy, which the round's compiler refuses): `check` 82,245,375,769 against the round's 58,318,643,669, and `build --emit-c` 1,032,122,020,001 against 988,241,573,073; seconds unrun, the machine not still. Linux arm64 at `43e6be50`: the compiler's own tests 1,299, all passed, and 26 suites green, `unsupported` asking the same floor and `records` and `unseen` reading no `.git` in an archive.

Closed after its platform legs, the C boundary's rule (`.claude/rules/verification.md` § The batch), on 2026-10-06 at batch 12's push.

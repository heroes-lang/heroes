---
kind: defect
area: ir
milestone: none
filed: 2026-10-06
commit: a2d349b67181b0828dafd0495d2687990e1d1d47
github: none
---

- [x] **387 — a value read before a branch and a write through `@` in the same expression reads freed memory and prints a wrong value** | `print(total(out, half(4).must() + bump(@out)))`, where `bump` pushes onto `out` through `@` and `total` reads the length of the `out` it was given: prints 3004 where the program means 1004, and `--sanitize` reports `heap-use-after-free` (the trunk's compiler at `0f48f9f9` and the round's at `bead6e45` alike, run by the coordinator between 11:32 and 11:36 on 2026-10-06, the clock read before and after); the same line without `.must()` prints 1004 and is clean, and `out @ out.push(half(4).must() + bump(@out))` fails the same way (lane ir12's report) | `selfhost/ir/survival.hero`, `survivors` asking only within one block whether a loaded value is read after a write to its root; a `.must()` puts the load and the write in two blocks · defect 381's `ir/between.hero` · **class: blocking**

    **Origin:** lane b12-ir12, 2026-10-06, found beside defect 381 (its final reply's *Found beside*, item 1); reproduced on both compilers and filed by the coordinator at 11:36 under the author's instruction of 2026-10-05, any defect found that is not an improvement goes into the batch.

    **Class: blocking**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): a memory fault and a wrong value at exit 0, in a correct program; the language's first promise is that a program does not corrupt memory (design.md §1.12, CLAUDE.md § Precedence), so this class is never deferred.

    The reproducer, whole:

        function half(n: i64) -> i64?
            if n % 2 == 0
                return ok(n / 2)
            return fail(code: "odd", msg: "odd")

        function bump(@xs: [i64]) -> i64
            xs @ xs.push(100)
            return xs.len()

        function total(xs: [i64], n: i64) -> i64
            return xs.len() * 1000 + n

        function main()
            out: [i64] @ []
            out @ out.push(5)
            print(total(out, half(4).must() + bump(@out)))

    Repaired at `a2d349b6`, 2026-10-06 (lane b12-ir12), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The shapes beside it held the same cause and stay in the item: a field, an element or a `str` field of a load read after a write to its root, which `total(b.xs, bump(@b.xs))` shows in ONE block (the base printed 4002 where 1002 is meant, `heap-use-after-free` under `--sanitize`); a write in the load's own block read in a later one; a callee writing by a store (5002 printed, 1002 meant); a loop, where the borrowed array grows in place on some turns and moves on others (7020 printed, 10020 meant). Each is a line of `tests/golden/run/fixedbugs-387-*`, the base's output kept in the lane's scratch.

## The repair

Repaired at `a2d349b6`. A load borrows what its root holds, and the ownership pass gave it a reference of its own only where `survival.survivors` saw its value read after a write to its root in one block; a `.must()`, `.default()`, `?`, `&&` or `||` between the load and an `@` write puts the read in a later block, and a field, element, payload or cast of the load is read through the same header, so `print(total(out, half(4).must() + bump(@out)))` printed 3004 for 1004 and read freed memory, `total(b.xs, bump(@b.xs))` printed 4002 for 1002, and a loop printed 7020 for 10020 with no crash at all. `selfhost/ir/borrowed.hero` now asks it of the whole function, across blocks through defect 381's `between.written`, and the ownership pass asks it alone. Its cases are four `run/fixedbugs-387-*` programs, each red on the base and clean under `--sanitize`, and three unit tests. The compiler's own `--emit-c` from 1,052,671,630,903 to 1,053,558,521,507 instructions, +0.08%.

**Closed 2026-10-06** with batch 12 (lanes b12-str192, b12-parse12, b12-hook, b12-cli12, b12-fit12, b12-ir12 and b12-ffi13, merged into one round tree made from the trunk at `0f48f9f9`), its closing gate run on the round's head: the seed regenerated at `43e6be50` over two generations, the runtime's ABI moving from 26 to 27, 37,358,146 bytes, SHA-256 beginning `fc9751a29a1ecb8a`, its fixpoint by `cmp`, and the compiler's own tests 1,299, all passed; then the net's own tests 286, all passed, and the full net, 28 suites and `cache`, 5,862 passed and 0 failed (`probe` red in the parallel pass and 27 passed alone; `unsupported` asking its floor raised from 118 to 150, done at `51f1a18c`, then 151 passed). The census, the trunk's compiler at `0f48f9f9` against the round's over the tree's tracked files, each with its own runtime: `check --brief` over 2,655, 66 moved, every one attributed to a repair of the batch, 10 of them the trunk unable to read a `\u{...}` escape the round's source writes; `build --emit-c` over the 1,146 holding an `extern`, the ABI stamp moving every unit and 186 moved besides, every one attributed, the trunk's 27 runs at exit 2 all gone, and one wrong binding newly accepted filed as defect 392. Panel 187's R2, its instrument and plan rebuilt from the transcripts after the machine's restart that morning (the same plan on every recorded fact), the trunk's compiler against the round's over 13,594 single mutants and 16,041 pairs: one mutant worse in both arms, filed as defect 391, and one pair's second newly told. The site's build: 38 claims and 2 verb lists checked over 20 pages. The cost, instructions retired over the trunk's compiler source (its two raw SOH bytes removed in the measuring copy, which the round's compiler refuses): `check` 82,245,375,769 against the round's 58,318,643,669, and `build --emit-c` 1,032,122,020,001 against 988,241,573,073; seconds unrun, the machine not still. Linux arm64 at `43e6be50`: the compiler's own tests 1,299, all passed, and 26 suites green, `unsupported` asking the same floor and `records` and `unseen` reading no `.git` in an archive.

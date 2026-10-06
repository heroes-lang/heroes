---
kind: defect
area: ir
milestone: none
filed: 2026-10-06
commit: none
github: none
---

- [ ] **387 — a value read before a branch and a write through `@` in the same expression reads freed memory and prints a wrong value** | `print(total(out, half(4).must() + bump(@out)))`, where `bump` pushes onto `out` through `@` and `total` reads the length of the `out` it was given: prints 3004 where the program means 1004, and `--sanitize` reports `heap-use-after-free` (the trunk's compiler at `0f48f9f9` and the round's at `bead6e45` alike, run by the coordinator between 11:32 and 11:36 on 2026-10-06, the clock read before and after); the same line without `.must()` prints 1004 and is clean, and `out @ out.push(half(4).must() + bump(@out))` fails the same way (lane ir12's report) | `selfhost/ir/survival.hero`, `survivors` asking only within one block whether a loaded value is read after a write to its root; a `.must()` puts the load and the write in two blocks · defect 381's `ir/between.hero` · **class: blocking**

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

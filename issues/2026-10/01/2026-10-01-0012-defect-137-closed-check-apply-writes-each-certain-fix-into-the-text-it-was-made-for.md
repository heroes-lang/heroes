---
kind: defect
area: cli
milestone: none
filed: 2026-09-30
commit: 04e90077ab8f0ebcc06e7141a1b35f1fa8243b29
github: none
---

# Defect 137 closed: check --apply writes each certain fix into the text it was made for, and asks the stage again until it has none left

- [x] **137 — `check --apply` applies overlapping certain fixes against the original text, so an enclosing fix overwrites, misaligns or overruns an inner one** | `print(total(xs.must()).must())` as a file's last line: `heroes check --apply` aborts, `panic: string slice out of range`, exit 134; with a line below it, exit 0 and the written text is `print(total(xs.must())rint(0)`, the line break, the margin and the next line's `p` eaten, which `--in-place` writes into the author's file | `selfhost/cli/check.hero:242-280` (`apply`) · the two sites whose span can enclose another fix, `check/builtins.hero:413` and `parse/type.hero:255` · **closed 2026-10-01**

    **Origin:** the coordinator's certain-fix audit, 2026-09-30, at
    `6d781be1` (`scratchpad/certain-audit/findings/C9-unwrapping_nothing/`,
    `c9c` and `c9j`, and `P7-bare_function_type/`, `p7b` and `p7c`);
    reproduced by the coordinator at 10:54 on the trunk's compiler, both
    shapes, from those files.

    **The cause, the audit's reading of `apply`, not yet proved by a
    repair.** The fixes of one file are applied back to front, each with the
    span and the replacement taken from the ORIGINAL text, so a fix whose
    span encloses another is applied over text an inner fix has already
    changed: it overwrites the inner edit, it is misaligned by the inner
    edit's change of length, and where it ends inside that change it slices
    past the end. `.must()` and `.default()`'s fix spans the whole call and
    the bare function type's the whole type, the two the audit found able to
    enclose another fix; its `p7c` shows the overwrite (both inner `int` swaps
    undone, their `reserved_word` standing after `--apply`).

    **Why it is a defect.** `check --apply` is the one command that writes
    the compiler's certain fixes into the author's file without a reader, so
    what it writes must be the fixes and nothing else: here it writes text no
    fix proposed, joined across a line, or aborts outside the exit-code
    contract (`.claude/rules/cli-surface.md`). The in-place write of the
    corrupted text is inferred from `check.hero`'s write path, not run
    (defect 136 is that write's other half).

    **2026-09-30, lane 136, `check --apply` writes each certain fix into the
    text it was made for, and asks the stage again until it has none left**
    (widened by the lane: a chain the checker reports at its first link lost
    one link per `--apply`): repaired at `5bec1b03`, gated by its cases and
    the compiler's own tests; the net ran at the lane's close, its counts in
    the closing commit's body.

    **The repair**, lane 136, `5bec1b03`, closed at `2ca85531` beside defect
    136. **The class**: the certain fixes of one stage were applied back to
    front with spans and replacements from the original text, so one
    enclosing another overwrote the inner edit, landed shifted, or sliced past
    the text's end; and beside it, found by the lane, a chain the checker
    reports at its first link lost one link per `--apply`, leaving a certain
    fix for the next run. Now `selfhost/cli/certain.hero` writes the fixes
    that touch no other, the shortest first where they do, and withholds the
    rest; `selfhost/cli/check.hero`'s rounds ask the same stage again of the
    text a round left until it gives no certain fix, a round that writes
    nothing or changes nothing being the last, 64 rounds and a text seen
    before said out loud; a span that ends before it starts is never written.

    **The entry's shapes, before and after**: on the certain-fix audit's six
    reproducers (`scratchpad/certain-audit/findings/C9-unwrapping_nothing/`
    `c9c`, `c9j`, `c9g`, `c9h`, `P7-bare_function_type/` `p7b`, `p7c`),
    before, one aborted at exit 134 writing nothing and five wrote text no fix
    proposed; after, all six exit 0 and each result checks clean, as do
    `one().must().must().must()` and `one().must().default(2)`, which one
    `--apply` left half done before.

    **Cases**: four in `cli/certain.hero`, eight texts in `cli/check.hero`,
    three fixtures under `tests/golden/surface-fixtures/certain137-*`, each
    with its `.fixed`, 3 failed on the base compiler.

    **Found beside it at the close and not of it**, on the trunk's compiler as
    on the lane's: `check --apply` writes the newline `source.from_files` adds
    to close a root file's open last line, so a file of 22 bytes ending
    without one comes out 29 where its one certain fix makes 28: bytes no fix
    proposed, filed apart as defect 141 (the rule this entry's repair holds,
    *what `--apply` writes is exactly the fixes it applied*, is the same; the
    cause is the loader's, not the overlap's).

    **The gates**: defect 136's, the same lane closing at `2ca85531` (its
    record, closed the same day, carries the counts): the seed regenerated
    once and its fixpoint; this Mac's 25 suites and `cache` alone at 0 failed,
    the compiler's own tests 947 and the net's own 184; the census, 0 moved
    over 1,394 files; Linux x86-64, 947 tests and 19 suites at 0 failed;
    Windows, 947 tests, `surface` 327, `fixes` 541, `probe` 24 at 0 failed.
    **The integration gate** (defect 136's record carries it whole): this
    entry's case and fixture were the one interaction with defect 135's rule,
    redone at `638c73f7` as `copy_text("a", to: "b")`, which still witnesses
    the defect on the trunk's old compiler; closed at `3cc3b553`, the full net
    4,023 and 0, Linux x86-64 3,899 and 0, the trunk fast-forwarded to it at
    00:03 on 2026-10-01. Owed before the push: Linux arm64 and the Windows
    box.

    **Linux arm64, 2026-10-01 11:14**, on the trunk at `9d1c209d`, whose
    `selfhost/`, `runtime/`, `seed/`, `tests/`, `examples/` and `spec/` are
    those of `3cc3b553` (`git diff --stat` over those paths prints nothing),
    in the `heroes-linux-arm64` image with clang 22.1.8, the seed built there
    (its SHA-256 begins `e4723edfe15d71f7`, the integration's): the compiler's
    own tests 960, all passed, and the 19 suites each 0 failed, 3,899 passed.
    **The Windows box is owed still**: its leg refused to start at 10:51, the
    `ssh` connection to the box timing out, and nothing was sent.

---
kind: defect
area: emit
milestone: none
filed: 2026-09-24
commit: self
github: none
---

- [x] **093 — a `str` holding a zero byte reaches C through `.cstr()` cut at that byte, at exit 0** | `read_file` makes a `str` from any bytes, a zero among them, and `.cstr()` hands C the same bytes zero-copy, so C reads a shorter string than the program holds and nothing says so | `selfhost/emit/` `.cstr()` · `runtime/` `hero_str_cstr` · `spec § 13` · `docs/records/log/2026-08-04-0001-escape-sequences-five-split-by-context-go-s-rule.md` · **class: blocking**

    **Origin:** panel 178's ffi-pragmatist, 2026-09-24 (its § 8 F3), found
    while pricing route T's refusal of an interior zero; reproduced by the
    coordinator the same day on all three legs before filing.

    **Class: blocking**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery), classed the day it reached the trunk: a wrong value at exit 0. Defect 245, the same defect filed again on the trunk, is `systemic`, its remedy having needed panel 192's ruling.

    **The reproducer** (`docs/panel/178-reports/ffi-pragmatist-work/nul.hero`),
    with `p/nul.bin` holding the five bytes `a b \0 c d`:

        extern "string.h"
            function strlen(s: cstr lent) -> u64

        function main()
            s = read_file("p/nul.bin").must()
            print(s.len())
            print(strlen(s.cstr()))

    `check` **0**, run **0**, prints `5` then `2`: three of three on Darwin
    arm64, and on Linux arm64 and x86-64.

    **Why it is a defect.** A wrong answer at exit 0. The language already
    ruled that an interior zero must not reach C: panel 008 froze the escape
    set with `\0` out because *interior NUL voids §4.20's free `.cstr()`* (the
    log entry above). The escape was closed and `read_file` is a second door to
    the same byte, which nobody closed. Whether the repair refuses at `.cstr()`
    (a `T?`, or an abort) or at `read_file` is the repair's question.

## Closed into defect 245

Closed 2026-10-06, when `lane-panel-178` was merged, as the same defect as 245, which the trunk filed on 2026-10-04 with this item's shape among its doors, `read_file` over a NUL lent to `strlen` through `.cstr()`, because this item stood only on that branch (`issues/2026-10/04/2026-10-04-0001-defect-245-a-str-holding-a-nul-is-lent-to-c-by-cstr-as-it-is-so-c-reads.md`). It is not closed as repaired: 245 is a defect at the C boundary, its repair `6b33db23` of 2026-10-05 closes only after the push's platform legs have run its cases (`.claude/rules/verification.md` § The batch), and those legs are 245's.

Re-run 2026-10-06 on `bef739dd`: this worktree's compiler, built from the round's seed, over the reproducer as `docs/panel/178-reports/ffi-pragmatist-work/nul.hero` holds it, with `p/nul.bin` the five bytes `a b \0 c d`, in a scratch directory from 11:37 by the clock: `check` 0; `run` 134 three of three, printing `5` and then *panic: `.cstr()` lends to C a str holding a NUL byte, at index 2 of its 5 bytes: C reads a string only to its first NUL, so it would read a shorter string than the program holds*. The lend stops before C reads, so the `2` is gone.

**245's repair is what changed it, proven on this Mac**: the same files on a compiler built from the seed of `ca5fa51e`, `6b33db23`'s parent, with that commit's runtime: `check` 0, `run` 0, printing `5` and `2`, the defect as filed; on a compiler built from `6b33db23`'s own `selfhost/` by that one, with `6b33db23`'s runtime: `check` 0, `run` 134, the panic above. Linux and Windows unrun here.

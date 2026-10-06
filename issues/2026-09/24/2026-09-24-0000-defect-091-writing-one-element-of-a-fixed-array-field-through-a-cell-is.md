---
kind: defect
area: emit
milestone: none
filed: 2026-09-24
commit: d64da8ff33379d7252eaea24ef7db2a9511f9358
github: none
---

- [x] **091 — writing one element of a fixed-array field through a cell is `check` 0 and dies at run time saying it is a compiler bug** | a store whose place ends in an index is always sent to the copy-on-write element writer, which serves `[T]` and fails for `T[N]`, and the emitter writes `hero_unreachable()` where the store should be | `selfhost/emit/inst.hero:133` · `selfhost/emit/container.hero:234` · `spec § 5` · **class: blocking**

    **Origin:** the coordinator of panel 178, 2026-09-24, measuring today's
    route for `sockaddr_un.sun_path` for that sitting's shared brief;
    number agreed with the session holding this list that night.

    **Class: blocking**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery), classed the day it reached the trunk: a correct program accepted at `check` and stopped at run time by the compiler's own admission of a bug.

    **The reproducer** (`docs/panel/178-briefs/elem_min.hero` + `elem_min.h`):

        extern "elem_min.h"
            record Slot tag slot partial
                name: i8[4]

        function main()
            s: Slot @ Slot(name: [0, 0, 0, 0])
            s.name[1] @ 72
            print(s.name[1])

    `check` **0**, run **134**, `panic: entered unreachable code — this is a
    compiler bug, please report it`: three of three on Darwin arm64 and on
    Linux x86-64, and on Linux arm64 by `heroes run` and by the built binary.
    The emitted C (`--emit-c`) carries
    `hero_unreachable(); /* not an element write */` where the store belongs.

    **Why it is a defect, and why the repair is a lowering.** It is defect
    052's sibling: that was `s[0] @ 65` on a `str`, closed 2026-09-16 by a
    REFUSAL, because spec § 3 calls `str` immutable. Here the document has
    ruled the other way: spec § 5 says *`@` declares a mutable cell and
    re-binds it, or a field or element inside one*, and § 13 gives a group
    record a fixed array field. So the spec admits the write, the compiler has
    the bug (CLAUDE.md § 12), and the repair is to emit the store, index
    checked as the read already is.

    **What it cost before it was found.** It is the only route today for the
    bytes a program writes into a fixed field, `sun_path` being the case that
    found it: `sunpath_*_ascii.hero` dies the same way on all three legs, so
    such a field has no working route except a literal that spells every byte
    as a number. Panel 178 weighs a form for it; the repair is owed whatever
    that sitting decides.

## The repair

Repaired at `d64da8ff`, by defect 097's repair. 097 is this defect found again: one element of a fixed array inside a group record written through a cell, found on the trunk on 2026-09-25 by the skeptic seat over panel 177's landing and filed there under its own number, because this item stood only on `lane-panel-178`, which the trunk did not hold. That repair is M-agreed-retention step 15, lane commit `d64da8ff`, merged at `dadba73b` and closed by `ce018b1c` (`issues/2026-09/25/2026-09-25-1905-defect-097-closed-a-fixed-array-element-is-written-in-place.md`). Closed 2026-10-06, when `lane-panel-178` was merged.

Re-run 2026-10-06 on `bef739dd`: this worktree's compiler, built from the round's seed, over the reproducer as `docs/panel/178-briefs/elem_min.hero` and `elem_min.h` hold it, in a scratch directory from 11:37 by the clock: `check` 0; `run` 0 three of three, printing `72`; `run --sanitize` 0, printing `72`.

**The repair proven rather than inferred**, the same two files on a compiler built from the seed of each commit: at `9e17d471`, `d64da8ff`'s parent, `check` 0 and `run` 134, *panic: entered unreachable code — this is a compiler bug, please report it*, after clang's *variable 't7' set but not used*; at `d64da8ff`, `check` 0 and `run` 0, printing `72`.

**The shapes beside it** that panel 178's seats wrote, run on `bef739dd`: `docs/panel/178-reports/ffi-pragmatist-work/shapes.hero` and the critic's ten programs in `docs/panel/178-reports/completeness-critic-work/w/x091/`. No output holds *unreachable*. The writes print what was written: a nested record's fixed field, a field of a fixed array's element, a whole element, a write through an `@` parameter, a group record inside a shared `[T]` and inside a Heroes record, each copy untouched; the element lend `bump(@o.pts[i])` prints `13`. Every index past the length, `-1` into an `i8[4]` and `3` into a `u8[3]` and into a `Pt[3]`, aborts 134 with the read's own *index out of range for a fixed array*. Four are refused at `check` by rules of their own: `type_mismatch` for a `u8` index, `unused_binding` and `not_mutable` for a loop variable, `no_such_field` through a map's `Slot?`, `ffi_field_type` for `i8[4][3]`.

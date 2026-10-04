---
kind: defect
area: parse
milestone: none
filed: 2026-10-03
commit: 1eb854c3c079c5ab7a90e7067848efa21ce839a4
github: none
---

- [x] **225 — `machine_locked` reads a header's first bytes, so a `..` climb to the root names a header by where this machine keeps it, and a `package` naming a `.pc` file is read from the working directory** | `extern "../../(24 times)/<the absolute path without its leading />"` over `function seven() -> i32`: `check` 0, `build` 0, prints `7`, while the same path written absolute is refused `machine_locked_path` (the trunk's compiler at `826ddc2f`, 2026-10-03, `<scratchpad>/repro188/climb/`); `package "./seven.pc"`, `"seven.pc"`, `"seven.PC"`, `".pc"` build from the `.pc`'s directory and are *not installed* from another (the ffi-pragmatist and the critic, pkg-config 3.0.7 and pkgconf 1.8.1) | `selfhost/parse/group_head.hero` (`machine_locked`; the leaf of panel 188 R9 once landed) · panel 055 · panel 188 R6, which refuses the `.pc` half · **class: adjacent**

    **Origin:** panel 188's ffi-pragmatist, 2026-10-03, on Q5, the `.pc` spellings widened by the critic's third pass; the climb reproduced by the coordinator the same day, the `.pc` path not.

    **Class: adjacent**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): real, beside the work, a thesis rule's reach rather than a wrong program; the `.pc` half lands with panel 188's R6, and for the climb the value alone cannot tell a climb to the root from a climb to a sibling directory.

    **2026-10-03, batch 8's FFI lane**: the `.pc` half is refused at `6779162c` (panel 188's R6), and a climb out of a named directory at `e8397297` (defect 234). For a leading climb no value-only rule was found among those searched (any `..`; a depth threshold; a root directory's name after the climb; the climb against the program's own depth; the climb against the parts after it): a climb to the root and one to a sibling directory are one string, and which it is depends on where the program sits. The closing is proposed as a pinned known cost, which needs a ruling (`<scratchpad>/batch8/ffi/225-pin/`, 2026-10-03).

## The repair

Repaired at `1eb854c3` (the pin; the `.pc` half refused at 6779162c, a climb out of a named directory at e8397297). Closed as a known cost pinned in a golden case, by the author's *1a* of 2026-10-04 (`docs/records/log/2026-10-04-0150-the-author-answers-1a-2a-3a-4a-5a.md`): `tests/golden/check/fixedbugs-225-a-climb-to-the-root-is-a-known-cost` refuses the absolute name (`machine_locked_path`, annotated) and passes the two climbs, so a rule that ever refuses a leading climb moves the golden at a gate. Batch 8's FFI lane tried five rules and none tells, from the string alone, a climb to the root from a climb to a sibling.

**Closed 2026-10-04** with batch 8 (lanes b8-ffi, b8-source, b8-recovery, b8-emit and b8-defects, merged into one round tree), its closing gate run on `921dc61e` with the seed regenerated: 40,628,892 bytes, SHA-256 beginning `2d55c5ff8309b812`, its fixpoint by `cmp`; the compiler's own tests 1,158, all passed; the net's own tests 210, all passed; the full net, 26 suites, 5,177 passed and 0 failed. The census at the batch's first gate, the trunk's compiler at `7d9f2e8f` against the round's over the tree's tracked files: `check --brief` over 1,954, 30 moved, and `build --emit-c` over the 1,228 holding an `extern`, 44 moved, every one the batch's own (its new refusals, its words, its `#line` before a fixed array field's assertion). Panel 187's R2, the trunk's compiler against the round's over one frozen plan: 13,594 single mutants, 12 fewer messages and none more; 15,842 pairs, one told second now carried in the first message's fixes (defect 271).

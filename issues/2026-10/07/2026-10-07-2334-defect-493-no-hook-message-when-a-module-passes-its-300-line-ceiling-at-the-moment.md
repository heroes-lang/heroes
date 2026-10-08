---
kind: defect
area: process
milestone: none
filed: 2026-10-07
commit: e608659ee0d945e3e5632bafddb64d649cec1aac
github: none
---

- [ ] **493 — no hook message when a module passes its 300-line ceiling at the moment of writing** | `verify.hero`, `typeorder.hero`, `container.hero` and `ops.hero` passed 300 in lane b14-emit's work with no word from the write hook; `layout` whole found them later (lane b14-emit) | `.claude/hooks/fmt_check.py`, `.claude/hooks/ceiling.py` · **class: improvement**

    **Origin:** filed by the coordinator at 23:34 on 2026-10-07 from lane b14-emit's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): what a hook can see on the touched file waits for a suite (`.claude/rules/verification.md` § A suite is the last judge).

    Repaired at `e608659e`, 2026-10-08 (lane b15-hooks), gated by its cases and the hooks' own tests; the net is owed at the batch's close. Reproduced on the base: a module of 303 lines of code written not canonical was told only *not canonical*, and the `heroes fmt --in-place` that answers it runs no hook; a failed check or a compiler older than its tree hid the ceiling the same way. The four modules named crossed it in batch 14 under the trunk's hook of before defect 254, silent on a lane's module (`affbd872`'s, exit 0). `fmt_check.py` now counts the ceiling on every write, on the canonical form where the file parses, asks the check, growth and marks of a file that parses, canonical or not, within the hook's budget, and says every answer; `ceiling.might_grow` is linear, same answer on 2,096 modules. 10 tests in `test_hooks.py`'s `EveryAnswer`, 9 red on the base; a write not canonical now costs the whole compiler's check a canonical one already did, 67.8G to 68.0G instructions at 50, 150 and 300 lines, 0.33G to 0.51G before; the hooks' own tests 86, OK.

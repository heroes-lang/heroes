---
kind: defect
area: runtime
milestone: none
filed: 2026-10-07
commit: 0690164366894f72edbcf0f5f7551df41dee34d4
github: none
---

- [x] **464 — `runtime/runtime.c` counts its parts as seventeen, and there are twenty-four** | the comment says *SEVENTEEN FILES*; `runtime/parts/` holds 24 (lane b14-runtime) | `runtime/runtime.c` · **class: improvement**

    **Origin:** filed by the coordinator at 18:37 on 2026-10-07 from lane b14-runtime's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a false comment.

    Repaired at `06901643`, 2026-10-08 (lane b15-runtime), gated by its cases and the compiler's own tests; the net is owed at the batch's close. `runtime.c` and `cli/runtime_key.hero` say no count of the runtime's parts now (the second said sixteen files, two headers and fourteen parts, beside four headers and 24 parts), and the claim a count stood for is a compiler test, *every file under the runtime's parts is included by runtime.c, and every part it includes is there*: 1431 tests all passed, and 1431 with 1 failed, that one, with a stray `runtime/parts/zz_stray.c` in a scratch copy.

## The repair

Repaired at `06901643`, 2026-10-08 (lane b15-runtime), gated by its cases and the compiler's own tests; the net is owed at the batch's close. `runtime.c` and `cli/runtime_key.hero` say no count of the runtime's parts now (the second said sixteen files, two headers and fourteen parts, beside four headers and 24 parts), and the claim a count stood for is a compiler test, *every file under the runtime's parts is included by runtime.c, and every part it includes is there*: 1431 tests all passed, and 1431 with 1 failed, that one, with a stray `runtime/parts/zz_stray.c` in a scratch copy.

**Closed 2026-10-09**, after the push's platform legs, this defect being at the C boundary or changing what clang gets on every platform (`.claude/rules/verification.md` § The batch): batch 15 closed on this Mac alone (`811f8398`) and the CI's legs ran its cases afterwards, run 37844345400, created at 23:06 on 2026-10-08 and its Windows leg finished at 01:31 on 2026-10-09. Darwin arm64 read the net 6,960 passed and 0 failed, Linux arm64 and Linux x86-64 6,941 each, and on those three legs the compiler's own tests 1,478, the module's 269 and the net's own tests 318, all passed; the Windows leg read the compiler's own tests 1,478 and the module's 269, all passed, and the net 6,811 passed and 1 failed, defect 322's sanitiser case alone (defect 509), its net's own tests not reached. This defect's cases are not among any leg's SKIP lines, read from the four logs, and passed on every leg; defect 447's vcpkg step ran on the Windows leg and its SDL3 case ran there, where batch 14's leg had skipped it. Defect 463's case is the hand-written one its lane ran on a Linux with SELinux enforcing, which no leg is.

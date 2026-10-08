---
kind: defect
area: runtime
milestone: none
filed: 2026-10-07
commit: 0690164366894f72edbcf0f5f7551df41dee34d4
github: none
---

- [ ] **464 — `runtime/runtime.c` counts its parts as seventeen, and there are twenty-four** | the comment says *SEVENTEEN FILES*; `runtime/parts/` holds 24 (lane b14-runtime) | `runtime/runtime.c` · **class: improvement**

    **Origin:** filed by the coordinator at 18:37 on 2026-10-07 from lane b14-runtime's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a false comment.

    Repaired at `06901643`, 2026-10-08 (lane b15-runtime), gated by its cases and the compiler's own tests; the net is owed at the batch's close. `runtime.c` and `cli/runtime_key.hero` say no count of the runtime's parts now (the second said sixteen files, two headers and fourteen parts, beside four headers and 24 parts), and the claim a count stood for is a compiler test, *every file under the runtime's parts is included by runtime.c, and every part it includes is there*: 1431 tests all passed, and 1431 with 1 failed, that one, with a stray `runtime/parts/zz_stray.c` in a scratch copy.

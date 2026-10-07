---
kind: defect
area: cli
milestone: none
filed: 2026-10-07
commit: 040d583f704513b7ef863d8b9ab02f745201ebd3
github: none
---

- [ ] **435 — every warm build compiles the runtime again** | a warm `heroes build` runs `clang -c runtime/runtime.c` every time, read in the clang-call log by lane b13-run400; its cost unmeasured | `selfhost/cli/`, the runtime object's cache · **class: improvement**

    **Origin:** filed by the coordinator at 00:11 on 2026-10-07, from lane b13-run400's report of the evening before (*found beside*); the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a cost, no program judged wrong.

    Repaired at `040d583f`, 2026-10-07 (lane b14-cli), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Not reproduced: logged, a warm build makes three clang calls, `clang_floor.identity`'s `-###` dry run, which names `runtime/runtime.c` and compiles nothing (about 255 million instructions a build), the headers' `-M` and the link; the runtime object was already cached by `runtime_object`, and a compiler test pins that a warm ask compiles nothing and reads no runtime.

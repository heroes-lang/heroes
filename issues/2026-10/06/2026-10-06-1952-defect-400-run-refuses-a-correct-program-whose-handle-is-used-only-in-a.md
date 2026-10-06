---
kind: defect
area: cli
milestone: none
filed: 2026-10-06
commit: 9be5f8b3c8d23575dc4b87d85208ca3d730d76d0
github: none
---

- [ ] **400 — `run` refuses a correct program whose handle is used only in a test** | `fwd.h` holding `struct opaque;`, `extern "fwd.h"` / `record Opaque tag opaque`, `main` printing 1, and `test "t"` binding `h: Opaque = nullptr`: `heroes run` exits 1 with *`fwd.h` declares no `opaque` — clang read the header and could not find it*, while `heroes test` passes (1 test, all passed); without the test block the program runs (the trunk's compiler at `6a03c488`, built from its seed at 19:52 and run by the coordinator at 19:52 on 2026-10-06) | the header's question asked of a record a `run` build does not reach, `selfhost/cli/` (the pointee and layout checks) · **class: blocking**

    **Origin:** lane b13-bs, 2026-10-06 (its report, *found beside* 1), met while landing panel 194; reproduced by the coordinator on the trunk's compiler.

    **Class: blocking**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): a correct program refused, with a false message: the header does declare `struct opaque`. Into batch 13.

    Repaired at `9be5f8b3`, 2026-10-06 (lane b13-run400), gated by its cases and the compiler's own tests; the net is owed at the batch's close.

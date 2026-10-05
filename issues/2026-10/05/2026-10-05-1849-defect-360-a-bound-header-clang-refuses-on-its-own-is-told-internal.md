---
kind: defect
area: cli
milestone: none
filed: 2026-10-05
commit: none
github: none
---

- [ ] **360 — a bound header clang refuses on its own is told `internal error` at exit 2, the author's header blamed on the compiler** | `extern "bad.h"` over a header holding `int broken = ;` answers *internal error: compiling the generated C failed:* with clang's *./bad.h:3:14: error: expected expression*, then *clang refused the generated C*, at exit 2 (this Mac, the round's compiler at `2dd5611c`, 2026-10-05, `<scratchpad>/hdr-bad/`); the Windows box said the same for defect 337's fixture header, which clang 23.1.1 refuses there | `selfhost/cli/produce.hero:220-242` (a clang refusal `blamed` does not read as the author's becomes `internal error`) · `selfhost/cli/units.hero:175` (the unit's failure, the generated C named) · **class: blocking**

    **Origin:** batch 11's coordinator, 2026-10-05, reading the Windows leg's `unsupported` failure on defect 337's case, then reproduced on this Mac with a header of one line that does not compile.

    **Class: blocking**, 2026-10-05 (`.claude/rules/verification.md` § Bounded discovery): an exit 2 where the author can be told, and a false message, *internal error* for an error clang locates in the author's own header. **Unrun**: a header that fails only under the compile's flags (a `-Werror=` of the flag list) against one that fails under any flags; a header reached through another header.

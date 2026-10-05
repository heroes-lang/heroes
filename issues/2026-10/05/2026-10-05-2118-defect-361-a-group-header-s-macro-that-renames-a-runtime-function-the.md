---
kind: defect
area: cli
milestone: none
filed: 2026-10-05
commit: none
github: none
---

- [ ] **361 — a group header's macro that renames a runtime function the generated C calls is told `internal error` at exit 2** | `extern "mac2.h"` over a header holding `#define hero_print_int(x) nothing`, with `print(twice(x: 21))`, answers *internal error: compiling the generated C failed:* with clang's *c4.hero:5:5: error: use of undeclared identifier 'nothing'* and its note *./mac2.h:3:27: note: expanded from macro 'hero_print_int'*, at exit 2 (this Mac, lane b12-cli12's compiler, which holds defect 360's repair, 2026-10-05, `<scratchpad>/r361/`) | `selfhost/cli/whose.hero` and `cli/header_refused.hero` (defect 360's reader takes clang's first error, which lies in the generated C, so its note into the header is not read) · `.claude/rules/c-boundary.md` (the generated C's names against a header's macros) · **class: blocking**

    **Origin:** lane b12-cli12, 2026-10-05, beside defect 360 (its report's *found beside*, a different cause from 360); reproduced by the coordinator at 21:18.

    **Class: blocking**, 2026-10-05 (`.claude/rules/verification.md` § Bounded discovery): an exit 2 where the author can be told, and a false message, *internal error* for a macro of the author's header that the error's own note locates.

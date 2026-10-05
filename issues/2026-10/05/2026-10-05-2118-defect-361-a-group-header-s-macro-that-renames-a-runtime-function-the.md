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

    **2026-10-05, lane b12-cli12, the shapes beside, measured** on that lane's compiler (`<scratchpad>/r361/`): two build at exit 0 and print wrong values, `#define INT64_C(c) 0` printing `0 0` for `42 1000000000000`, every integer literal being `INT64_C(n)`, and `#define hero_print_int(x) ((void)(x))` printing nothing; the item's own shape, the same as a plain macro, and the same from a header the group's header includes are `internal error` at exit 2; `#define HERO_RUNTIME_ABI 99` is exit 2 with the false *heroes_runtime.h is from another compiler*; `#define main other_main` fails the link at exit 2; `bool`, `int64_t` and `HeroStr` redefined build and run right on this Mac by luck; a runtime function the program never calls is harmless. So the item is a wrong value as well as a false message. The robust repair the lane measured on hand-built units under Apple clang 21, Debian clang 18.1.8 and 22.1.8: after the group's includes, the emitted unit restores every name its own code uses with `#pragma push_macro` and `pop_macro`, the names the program binds left unguarded (glibc's `#define st_mtime st_mtim.tv_sec` refuses a guard); in a helper beside `emit/c_text.hero`'s `includes`, used by every writer of a unit that includes the group's headers, `emit/unit`, `layout_text`, `layout_check`, `layout_screen`, `ffi_asked`, and `cli/header_types.hero`'s pointee units. Unrun: Windows, and the text's cost. A reader in `cli/` alone would tell the exit 2 rows and never see the two at exit 0.

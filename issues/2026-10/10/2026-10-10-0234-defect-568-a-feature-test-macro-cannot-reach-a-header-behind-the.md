---
kind: defect
area: emit
milestone: none
filed: 2026-10-10
commit: none
github: none
---

- [ ] **568 — a feature-test macro cannot reach a header behind the compiler's own prefix, so a GNU function is bound only through an unchecked prototype** | panel 204's ffi-pragmatist on Linux arm64: glibc's `sched_getcpu` (declared only under `_GNU_SOURCE`) is refused `ffi_unknown_name` bound plainly, through a header of the program's own that defines `_GNU_SOURCE`, and with that group written first, because every unit opens with `heroes_runtime.h`, `<math.h>` and `<hero_os.h>` before any group's header and a feature-test macro must precede every header (POSIX 2.2.1.1, glibc's feature_test_macros(7)); the one route that builds is a hand-written prototype, and a wrong one, `long sched_getcpu(int)`, builds and runs; at C level `-D_GNU_SOURCE` restores the check, which panel 076's ffi-pragmatist refused as a build flag on a measurement (`strerror_r` re-typed); on this Mac `_POSIX_C_SOURCE` in a group's header written first does nothing (the critic) | the unit's opening, `selfhost/emit/externs.hero` (`SEEDS`, `:79`) and `runtime/heroes_runtime.h:54-56`; panel 204 R5, for a sitting of its own · **class: blocking**

    **Origin:** filed by the coordinator at 02:34 on 2026-10-10 from panel 204 (`docs/panel/204-c-reads-a-module-s-headers-in-the-order-its-groups-are-written-and-the-spec-says-so.md`, R5), the seats' measurements, not re-run by the coordinator.

    **Class: blocking**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): a correct program refused, and a binding's verification lost.

    **Ruled 2026-10-10** by panel 205 (`docs/panel/205-a-library-s-own-header-code-is-judged-as-clang-judges-a-system-header-the-checks-raised-again-after-it-and-a-switch-goes-in-the-first-group.md`, ratified at 03:38): R3: the compiler's prefix reads no libc header before the groups (its integer types from clang's builtins, `<stdint.h>` and `<math.h>` after the close, defect 361's guard kept), so a module's first group can set a switch; a name a header declares only under a feature-test macro is told so, never *declares no*; spec § 13 gains H2f. This item's *only a hand-written prototype builds* is false: a package's `-D` reaches it today (the critic's first pass), program-wide, which defect 572 repairs.

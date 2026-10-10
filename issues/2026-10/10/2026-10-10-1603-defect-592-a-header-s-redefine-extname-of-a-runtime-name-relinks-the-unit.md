---
kind: defect
area: emit
milestone: none
filed: 2026-10-10
commit: 6fe4c37f9d7b3c225be54105e64af78edcf63996
github: none
---

- [ ] **592 — a header's `#pragma redefine_extname` of a name the unit's own C calls renames the compiler's call, a link failure at exit 2** | a header of the program's own writing `#pragma redefine_extname hero_print_int hdr_print_int`, binding `labs`, builds at exit 2 with *internal error: linking failed*, *Undefined symbols: "hdr_print_int"*: the pragma attaches its label to the runtime's declaration read before the groups, so every `print` of an `i64` the compiler writes calls a name nobody defines; the guard of defect 361 gives back a macro of such a name, and a label attached by this pragma it cannot, since a second label does not replace the first; measured on this Mac (Apple clang 21) by lane b19-pack at 16:00 | `selfhost/emit/macro_guard.hero`'s guard, or the round's reading of an undefined runtime name after a header renamed it · **class: blocking**

    **Origin:** found by lane b19-pack attacking the shapes beside defect 582 (`.claude/worktrees/scratch-b15/b19-pack/shapes/`, ignored by git), filed by the lane at 16:03 on 2026-10-10.

    **Class: blocking**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): an exit 2 where the author can be told.

    Repaired at `6fe4c37f9d7b3c225be54105e64af78edcf63996`, 2026-10-11 (lane b20-pragma), with defect 591, gated by its cases and the compiler's own tests; the net is owed at the batch's close. An undefined symbol that is a header's label of a name the C this compiler writes uses, or `main` renamed away, is told `ffi_header_refused` on the group's header, the label's file and line named (`cli/renamed.hero`, `cli/label_dump.hero`). Measured on this Mac (Apple clang 21), each exit 2 before and exit 1 after at `-O0` and `-O2`: the reproducer (`hero_print_int`), `main`, `fmod` (which the unit writes for `%` on an `f64`, declared after the groups), `hero_print_int` declared again by the header with an `__asm__` label, the pragma in a header the group's header includes, the pragma in the second of two groups' headers, and `heroes test` of such a program. In the Linux arm64 image (clang 22.1.8), at `6fe4c37f9d7b3c225be54105e64af78edcf63996`: its cases pass, and `fmod` and `main` are told. Measured and not taken: an explicit label on the runtime's first declaration resists a later pragma (a declaration labelled `_rt_fn`, then the pragma, calls `_rt_fn`), a route that labels each of the runtime's prototypes, about 150 by a grep of the two headers' lines ending in `);`, and still leaves the functions of `<math.h>` and `main`, declared after the groups, to the header; after the header C has no way back. Cases `unsupported/fixedbugs-592-*` (3).

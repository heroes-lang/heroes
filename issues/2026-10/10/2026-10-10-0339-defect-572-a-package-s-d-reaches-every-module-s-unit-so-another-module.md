---
kind: defect
area: cli
milestone: none
filed: 2026-10-10
commit: b87d2bc2bca69b13038b0ec0f243d37d0a5e987b
github: none
---

- [ ] **572 — a package's `-D` reaches every module's unit, so another module's package decides a module's verdict** | panel 205's ffi-pragmatist and critic on Linux arm64: module `w` binds `wcwidth(c: i32)` from `wchar.h`; with no module naming `package "ncursesw"` it is refused *`wchar.h` declares no `wcwidth`*, false, and when `main` names `ncursesw` (whose `.pc` answers `-D_DEFAULT_SOURCE -D_XOPEN_SOURCE=600`) the declaration is found and `w`'s `i32` refused `ffi_parameter_type` against `unsigned int`; a package's answer reaches every unit of the program (`selfhost/cli/produce.hero:142`) | `selfhost/cli/produce.hero` and the units' compile words; panel 205 R3 and R5, landing with R3 · **class: blocking**

    **Origin:** filed by the coordinator at 03:39 on 2026-10-10 from panel 205 (`docs/panel/205-a-library-s-own-header-code-is-judged-as-clang-judges-a-system-header-the-checks-raised-again-after-it-and-a-switch-goes-in-the-first-group.md`, R5), the seats' measurements, not re-run by the coordinator.

    **Class: blocking**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): a false message, and a module's verdict decided by another module's group.

    Repaired at `b87d2bc2bca69b13038b0ec0f243d37d0a5e987b`, 2026-10-10, gated by its cases and the compiler's own tests; the net is owed at the batch's close. Each package is asked alone (`cli/libraries.hero`'s `resolve_each`), each group's header is mapped once per build to the packages its groups name (`emit/header_packages.hero`), and a module's unit compiles under the words of the packages of the headers it reads; each probe list carries its own words, used by the probes, a refused round's questions and a refused header's alone check. The words follow the header rather than the module: asked by the module's own groups, the library module's unit, which reads `uv.h` for the record it spells, lost libuv's `-I` and `run/fixedbugs-413-libuv-keeps-every-handles-address` was refused; so where two modules bind one header and one group names its package, the package reaches both (a question for the coordinator against R3's words). Measured on Linux arm64: the sitting's `nopkg`, `nopkg2`, `withpkg` and `wrong` are all told *`wchar.h` declares `wcwidth` only under `_XOPEN_SOURCE`*, with or without `main` naming `ncursesw`; on this Mac a fixture package answering only a `-D` reaches the module whose header it names and not a module reading another (by hand, no golden: the harness hands a case no `PKG_CONFIG_PATH`). The compiler's own tests 1,560, run 445 and 0, unsupported 229 and 0, warnings 509 and 0, emission 1,124 and 0 with no byte moved, cache 7 and 0, layout 6 and 0; a cold self-build 427.54G to 427.74G instructions.

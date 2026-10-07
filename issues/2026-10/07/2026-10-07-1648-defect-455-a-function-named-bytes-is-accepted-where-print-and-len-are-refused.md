---
kind: defect
area: resolve
milestone: none
filed: 2026-10-07
commit: none
github: none
---

- [ ] **455 — a function named `bytes` is accepted where `print` and `len` are refused, and its own `s.bytes()` calls itself for ever** | `function bytes(s: str) -> [u8]` returning `s.bytes()`, called once from `main`: `check` exit 0; `build` exit 0 printing clang's *warning: all paths through this function will call itself [-Winfinite-recursion]* over the generated C (`HeroArrayHeader * h_shadow_bytes(HeroStr h0_s) {`); the program aborts *panic: stack exhausted in shadow.bytes*, exit 134; the same file with a function named `print` or `len` is refused `builtin_name_taken` (run by the coordinator between 16:44 and 16:48 on the trunk's compiler, `<scratchpad>/batch14/shadow/`; first seen by lane b14-runtime) | the resolver's built-in name check (`builtin_name_taken`, `selfhost/resolve/`) against the table of built-ins (`selfhost/inventory.hero`) · design.md §4.4, *Shadowing is forbidden* · **class: blocking**

    **Origin:** filed by the coordinator at 16:48 on 2026-10-07, from lane b14-runtime's final report (*found beside*, *a clang warning in a build, to check whether it is a defect*), run by the coordinator before filing.

    **Class: blocking**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a wrong program accepted, a rule the compiler already enforces (`builtin_name_taken`) missing a name, and a clang warning in C words reaching the author.

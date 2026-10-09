---
kind: defect
area: resolve
milestone: none
filed: 2026-10-07
commit: 9c72d0b5ff64a7d2ce26aceeeb161b9a48c08a91
github: none
---

- [ ] **501 — a program's own `validated` replaces the library's in silence** | panel 162's text says `validated` moves into the table of built-ins; it is still on `resolve/builtin_names.hero`'s exception list, so a program's own `validated` silently replaces the library's in its module; reserving it needs `emit/bytes_text.hero`'s own `validated` renamed (lane b14-resolve) | `selfhost/resolve/builtin_names.hero`, `selfhost/emit/bytes_text.hero` · defect 455 · panel 162 · **class: improvement**

    **Origin:** filed by the coordinator at 23:34 on 2026-10-07 from lane b14-resolve's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a rule the compiler enforces missing one name a sitting named.

    Repaired at `9c72d0b5`, 2026-10-09 (lane b15-emit), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The compiler's own function of that name is `bytes_text.emit`, the exception and the test of its premise are gone, and a program's `validated` is refused `builtin_name_taken` at a top-level function, a constant, a parameter, a type parameter, a local, a loop variable and a match binding (`tests/golden/check/fixedbugs-501-a-program-s-own-validated-is-refused-everywhere`), where on the base the function was told nothing and its call printed the program's own text at exit 0. A field and a variant case of the name stay legal. `check` over the 3,047 tracked programs moved this case alone.

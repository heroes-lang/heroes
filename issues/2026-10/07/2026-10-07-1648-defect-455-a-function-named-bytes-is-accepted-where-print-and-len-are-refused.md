---
kind: defect
area: resolve
milestone: none
filed: 2026-10-07
commit: b8edf1b748c4123871ad6520a5b718bb343d5928
github: none
---

- [ ] **455 — a function named `bytes` is accepted where `print` and `len` are refused, and its own `s.bytes()` calls itself for ever** | `function bytes(s: str) -> [u8]` returning `s.bytes()`, called once from `main`: `check` exit 0; `build` exit 0 printing clang's *warning: all paths through this function will call itself [-Winfinite-recursion]* over the generated C (`HeroArrayHeader * h_shadow_bytes(HeroStr h0_s) {`); the program aborts *panic: stack exhausted in shadow.bytes*, exit 134; the same file with a function named `print` or `len` is refused `builtin_name_taken` (run by the coordinator between 16:44 and 16:48 on the trunk's compiler, `<scratchpad>/batch14/shadow/`; first seen by lane b14-runtime) | the resolver's built-in name check (`builtin_name_taken`, `selfhost/resolve/`) against the table of built-ins (`selfhost/inventory.hero`) · design.md §4.4, *Shadowing is forbidden* · **class: blocking**

    **Origin:** filed by the coordinator at 16:48 on 2026-10-07, from lane b14-runtime's final report (*found beside*, *a clang warning in a build, to check whether it is a defect*), run by the coordinator before filing.

    **Class: blocking**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a wrong program accepted, a rule the compiler already enforces (`builtin_name_taken`) missing a name, and a clang warning in C words reaching the author.

    Repaired at `b8edf1b7`, 2026-10-07 (lane b14-resolve), gated by its cases and the compiler's own tests; the net is owed at the batch's close. **`bytes` is no built-in**: spec § 11, `inventory.hero`, the library's declarations and the names the checker and the emitter dispatch on were enumerated, and none holds it, so its `s.bytes()` is UFCS for the program's own `bytes(s)`, calling itself as written; the clang warning and the stack's end are any unconditional recursion's (`function forever(n: i64) -> i64` returning `forever(n)` reads the same), reported to the coordinator apart. The enumeration found the escape the entry's class names: the library, whose every declaration each module reads unqualified, declares `args_checked` (§ 11, beside `args`), which the inventory lacks, so a program's own was accepted at the top level and took every call, and a parameter, type parameter or local of that name took its scope. The check now reads the library's own declarations beside the inventory (`resolve/builtin_names.hero`), externs aside and `validated`, left free by the decision `inventory.hero` records; `check selfhost/main.hero` 66.092 against 66.109 billion instructions.

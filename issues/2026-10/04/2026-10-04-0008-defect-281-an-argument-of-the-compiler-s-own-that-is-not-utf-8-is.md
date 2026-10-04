---
kind: defect
area: compiler
milestone: none
filed: 2026-10-04
commit: none
github: none
---

- [ ] **281 — an argument of the compiler's own that is not UTF-8 is refused at exit 2 with advice meant for a program's author, so a file so named cannot be checked** | `heroes check` with the argument `caf<e9>.hero`: exit 2, *error: argument 2 is not UTF-8, and a Heroes `str` cannot hold it (spec § 3 Types)* and *note: build the program with `heroes build` and read it with `args_checked()`*, advice to the author of a Heroes program handed to the user of the compiler (batch 8's round compiler, this Mac, 2026-10-04, `<scratchpad>/filings-b8/probe/c281/`); on Linux arm64 a correct program in a file so named cannot be checked, built or formatted at all (panel 189's ffi-pragmatist, its Linux table) | the compiler's reading of its own `argv` (`selfhost/main.hero` and `selfhost/cli/argv.hero`), which takes `args()` · defect 238, Windows' narrow API, the other platform's half · **class: adjacent**

    **Origin:** panel 189's critic, first pass and second (*the compiler's own argv not UTF-8 on Linux and this Mac*, `docs/panel/189-reports/completeness-critic.md` § Q6), met by the compiler-engineer and the ffi-pragmatist; filed by the sitting's R12 and run by the coordinator.

    **Class: adjacent**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): exit 2 is the tool unable to take the name, which is true; its note routes the compiler's user to a program author's repair. Not 227's cause: no file's contents are read.

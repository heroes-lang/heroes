---
kind: defect
area: compiler
milestone: none
filed: 2026-10-04
commit: 89d31024bf82f2cedcdb57a8a10fb87033d87df5
github: none
---

- [x] **281 — an argument of the compiler's own that is not UTF-8 is refused at exit 2 with advice meant for a program's author, so a file so named cannot be checked** | `heroes check` with the argument `caf<e9>.hero`: exit 2, *error: argument 2 is not UTF-8, and a Heroes `str` cannot hold it (spec § 3 Types)* and *note: build the program with `heroes build` and read it with `args_checked()`*, advice to the author of a Heroes program handed to the user of the compiler (batch 8's round compiler, this Mac, 2026-10-04, `<scratchpad>/filings-b8/probe/c281/`); on Linux arm64 a correct program in a file so named cannot be checked, built or formatted at all (panel 189's ffi-pragmatist, its Linux table) | the compiler's reading of its own `argv` (`selfhost/main.hero` and `selfhost/cli/argv.hero`), which takes `args()` · defect 238, Windows' narrow API, the other platform's half · **class: adjacent**

    **Origin:** panel 189's critic, first pass and second (*the compiler's own argv not UTF-8 on Linux and this Mac*, `docs/panel/189-reports/completeness-critic.md` § Q6), met by the compiler-engineer and the ffi-pragmatist; filed by the sitting's R12 and run by the coordinator.

    **Class: adjacent**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): exit 2 is the tool unable to take the name, which is true; its note routes the compiler's user to a program author's repair. Not 227's cause: no file's contents are read.

    **2026-10-04:** Repaired at `89d31024`, gated by its cases, the compiler's own tests (1,215, all passed) and the net's own (262, all passed) on this Mac; the net is owed at the batch's close, and its runtime half the platform legs.

## The repair

Repaired at `89d31024`. An argument of the compiler's own that is not UTF-8 was refused at exit 2 with advice meant for a Heroes program's author; a runtime read, `hero_args_shown`, lets the compiler name such an argument by its bytes, `caf<0xE9>.hero`, a file told in defect 239's words, a flag's value by its flag, and only an argument after `run`'s `--` keeps the advice to build the program and read `args_checked()`. Its cases are compiler tests and the net's own tests over three roles; making such a file checkable is left, the message being the repair.

**Closed 2026-10-05**, after batch 11's platform legs, each on `2dd5611c`, its tree as pushed at `61e085ae` but for records: Linux arm64 in its container under Debian clang 22.1.8 and again under 18.1.8, the compiler's own tests 1,233, all passed, and 22 suites, 5,244 passed and 0 failed, each time; the Windows box under clang 23.1.1, lane b11-windows' three trees, each with its compiler built from the seed and then from `selfhost/`: in `land`, the round's own runtime, the net's own tests 280, all passed, the compiler's own tests 1,233 with one failed, defect 337's, and 21 of 22 suites 0 failed, `unsupported` 130 passed and 1 failed, 337's other case; in `base`, the runtime as it stood at `7c615049`, the compiler's own tests failing 337's, 345's, 346's and 239's, and the net's own failing 12, 238's eleven and 239's walk; `heroes doctor` exit 0 in `land` with its MSVC row, where batch 10's compiler on the same box says *FAIL cc not found* with Linux's advice at exit 2. The CI after the push, at `61e085ae`: Darwin arm64, Linux arm64 and Linux x86-64 green; Windows x86-64 one of the net's own tests failed, defect 238's W16 and W21, its fixture's `clang -shared` exiting 1120 under clang 20.1.8, a row of 238, which stays open.

---
kind: defect
area: harness
milestone: none
filed: 2026-10-04
commit: 73ff6c965e560960369ff7963bf2be5fa56ab732
github: none
---

- [x] **331 — a bare compiler name, `HEROES_COMPILER=heroes` or `-- heroes`, is checked in the working directory and started through the PATH, so the net can judge another compiler** | a compiler named without a `/` is checked for a file in the working directory and then started through `execvp`, which searches the PATH; where the PATH holds another `heroes`, the net judges that one; on the lane's machine none was on the PATH, so it failed loudly (unrun in the shape that misleads) | `tests/harness/shell.hero` (`some_compiler` and the start of a named program) · defect 316 · **class: adjacent**

    **Origin:** lane b10-harness, 2026-10-04, measured on its worktree at `bd1f168c` (its final reply's *Found beside*), beside 316.

    **Class: adjacent**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a question about which program is judged, answered by the machine's PATH where the author named a file; no wrong verdict measured.

    **2026-10-04:** Repaired in part at `69d9f47b`: a `HEROES_COMPILER` named bare is started as the file checked, gated by its case and the net's own tests on this Mac (261, all passed). The `-- <compiler>` entrance is `tests/harness/main.hero:125`, a file of lane misc's set, and owes `compiler @ shell.started_as(given[0])`, reported to the coordinator; the net is owed at the batch's close.

    **2026-10-05, batch 11's round**: the `-- <compiler>` entrance, Repaired at `73ff6c96`, starts the compiler `shell.started_as` names, the file the harness checked, as `HEROES_COMPILER` already was at `69d9f47b`; the item is repaired at both entrances, gated by `check tests/harness/main.hero` and the round's gate.

## The repair

Repaired at `69d9f47b` for `HEROES_COMPILER` and at `73ff6c96` for `-- <compiler>`. A compiler named bare was checked in the working directory and started through the PATH, so the net could judge another compiler; `shell.started_as` turns a bare name into `./name`, one string for the check and the start, `\` and `:` parting a path only on Windows. Its case is in the net's own tests, two stubs and a probe.

**Closed 2026-10-05**, after batch 11's platform legs, each on `2dd5611c`, its tree as pushed at `61e085ae` but for records: Linux arm64 in its container under Debian clang 22.1.8 and again under 18.1.8, the compiler's own tests 1,233, all passed, and 22 suites, 5,244 passed and 0 failed, each time; the Windows box under clang 23.1.1, lane b11-windows' three trees, each with its compiler built from the seed and then from `selfhost/`: in `land`, the round's own runtime, the net's own tests 280, all passed, the compiler's own tests 1,233 with one failed, defect 337's, and 21 of 22 suites 0 failed, `unsupported` 130 passed and 1 failed, 337's other case; in `base`, the runtime as it stood at `7c615049`, the compiler's own tests failing 337's, 345's, 346's and 239's, and the net's own failing 12, 238's eleven and 239's walk; `heroes doctor` exit 0 in `land` with its MSVC row, where batch 10's compiler on the same box says *FAIL cc not found* with Linux's advice at exit 2. The CI after the push, at `61e085ae`: Darwin arm64, Linux arm64 and Linux x86-64 green; Windows x86-64 one of the net's own tests failed, defect 238's W16 and W21, its fixture's `clang -shared` exiting 1120 under clang 20.1.8, a row of 238, which stays open.

# Panel 154 — brief for the compiler-engineer

Read `00-shared.md` first. You judge the ceiling (design.md §1.1, §1.7, Part 5):
implementation cost and core-versus-sugar, for A to E and any sixth route you
name. You have a veto.

**Where each piece lives in the live compiler** (`selfhost/`, never `archive/`):
the `cstr` guard this is symmetric with, `guard_cstr_arguments` — find it and
report its exact site and line count, because the whole sitting turns on whether
a handle guard is that same edit; `selfhost/emit/ops.hero`'s call emission;
`selfhost/emit/extern_probe.hero`; `selfhost/handles.hero` for what a handle is;
`selfhost/check/acquiring.hero` and `check/consuming.hero` for the marks route D
would lean on; `selfhost/parse/members.hero` for how a new parameter word is
parsed, and `selfhost/ast.hero`'s `Param`; every tool that re-prints a program
(`.claude/rules/diagnostics-and-goldens.md` § A new surface form).

**What your report must carry.** The line count of each route against those
files, and whether any file crosses `tests/harness/suite_layout.hero`'s `DECIDED`
ceiling. Whether the guard costs anything at run time: build a program that calls
a C function through a handle in a tight loop, a million iterations, and time it
with and without a hand-written null check, `/usr/bin/time -p`, machine still.
Whether route D's premise survives the compiler's own corpus: enumerate every
handle parameter in `examples/` and `tests/golden/` and say which are marked and
which are not. One prediction with the command that scores it.

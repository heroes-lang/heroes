# Panel 245, the brief every seat reads first

Convened 2026-11-10 by the coordinator of batch 30, in M-core-packages, on the author's instruction
of that day, meant as: *close all the systemic defects too; hold one sitting with all of them in it*.
Without asking again for the gate (CLAUDE.md § 4: asked once per milestone, at panel 242). Seats as
panels 242 to 244 sat by the author's answer *four seats, no blind seat*: compiler-engineer,
ffi-pragmatist, spec-warden, historian, and the completeness critic before and after. The
llm-ergonomist's blind reading is a paid run and is not named here; if a route needs a specification
sentence, the synthesis queues it for the author's yes.

## The base

Batch 30's round at `a4b89740bc36fdaeb553a159fa16c0e4a5918a2f` (`git rev-parse HEAD` in
`.claude/worktrees/b30-round`, 2026-11-10 22:3x), which holds panel 243's R1 (defect 2421, every
argument passed by value past a header's `...` refused, a door required) and the batch's other
repairs. Not `main`: the questions below arise from R1's landing. Each seat with Bash works in its
own detached worktree `.claude/worktrees/245-<seat>` at that commit and builds its own compiler
there in two stages, `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, then `./heroes
build selfhost/main.hero -o heroes-next`; never another tree's binary. Probes in
`.claude/worktrees/p245-scratch/<seat>/`. No file is written outside the repository's root.

## Why these three

`heroes run tests/harness/issues.hero -- open --kind defect --class systemic`, run 2026-11-10,
lists two open `systemic` defects, 2464 and 2465. The coordinator adds 2448, `blocking`: panel 243
(§ Left open, R4) filed it as a mismatch no route of that sitting touches, so no lane can repair it
without a ruling, the case a `systemic` class exists for (`.claude/rules/verification.md` § Bounded
discovery). All three stand at the C boundary.

## The three questions

**Q1, defect 2464 (systemic)** (`gh issue view 2464 --repo heroes-lang/heroes`). A door that calls a
deprecated C function, defect 584's `sprintf` with `_FORTIFY_SOURCE 0`, builds at exit 0 and prints
clang's raw `-Wdeprecated-declarations`. Since panel 243's R1 a door is the only route to a variadic
such as `sprintf`. The unit's opening (the text `selfhost/cli/order_trial.hero:161`'s test pins)
raises `-Wformat` and `-Wformat-nonliteral` to errors and ignores `-Wformat-security` inside the
header region (`selfhost/emit/header_region.hero`), and ignores `-Wdeprecated-declarations`,
`-Wdeprecated-pragma` and `-Wattribute-warning` only after the region's close
(`selfhost/emit/deprecation.hero`), which is panel 208's R1 (`docs/panel/208-*.md:93-97`: *"ignored
over the whole unit after the groups' close, nothing else widened"*). A door's body stands inside the
region. Lane b30-variadic's recommendation: quiet the three words in the region too; its census,
*0 of 2,400 Homebrew headers in the region's shape and 1 tracked header warn today*, is the lane's,
unrun by the coordinator: run it. The lane's probe: `.claude/worktrees/b30-scratch/variadic/dep584/`
(read only). Question: what a door's call of a deprecated or attribute-warned function prints, and
where the rule lives.

**Q2, defect 2465 (systemic).** A binding with nothing past a header's `...`, such as
`printf(format: cstr lent) -> i32`, called with a format holding `%d` given at run time, builds at exit
0 and prints what the callee reads; the lane reported `6099856048` and `6138276528`, one value per
run; no file in its scratch holds them (`grep -rl` found none): reproduce it. A door has the same gap
when its format comes from outside it. Panel 243 refused the route that would read a literal
format's conversions in the compiler (its (R), the spec-warden's veto on Principle 0), kept
`-Wformat-nonliteral` an error in the region, and kept `-Wformat-security` ignored there (defect
2036; its four `run` cases, `ls tests/golden/run/ | grep fixedbugs-2036`, call `printf` or
`snprintf` in a header's own code, one with a format held in a variable). Question:
whether a format function may be bound directly at all, or only through a door holding its format
literal where `-Wformat` judges it; and what a program that prints a run-time string writes then.

**Q3, defect 2448 (blocking, no route).** A program binding `extern "curl/curl.h" link "curl"` built
with `CPATH=/opt/homebrew/opt/curl/include` (Homebrew's keg-only libcurl 8.23.0) prints `header 530176`
beside `library libcurl/8.7.1`, linked to `/usr/lib/libcurl.4.dylib`, at exit 0; panel 243's critic
measured readline 8.3's header against libedit under `CPATH`, its ffi-pragmatist `--include`. The
coordinator's reproducer: `.claude/worktrees/b30-scratch/repro/mismatch/` (read only). design.md
§4.19 (`docs/design.md:3186` on, *"The environment is a named folder"*, defect 2344, panel 055)
reads `CPATH`, `C_INCLUDE_PATH` and `CCC_OVERRIDE_OPTIONS` from clang's own answer and treats such a
folder as named; panel 243's R2 made a folder a `pkg-config` answer names its library's. Question:
when a header found through the environment or `--include` describes another release than the
library `link` names, what the build does, and on what it rests (a header and a library carry no
shared version in general).

## How the sitting runs

- Every number, path and count you write is produced by a command you run while writing it, and you
  say which; a negative sentence is run or written as a question.
- One heavy command at a time in your worktree; never two in parallel. A run that may not end is a
  built binary under `timeout`, its output bounded.
- No paid run, no credential printed (never `gh auth token`), no GitHub write, no push, no edit of
  the tree outside your own worktree and your report.
- Write your report to `docs/panel/245-reports/<seat>.md` in `.claude/worktrees/p245` as you go; if
  the tool refuses that write, your final message is the report, whole.
- A route you recommend is one you built and ran, in your worktree, on the cases you name.

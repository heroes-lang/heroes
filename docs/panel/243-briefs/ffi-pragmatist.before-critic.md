# Panel 243, ffi-pragmatist

Read `00-shared.md` first, whole. Your worktree: `.claude/worktrees/243-ffi-pragmatist`
(detached at `89db84347`; build your compiler there from the seed in two stages).

You judge the C every route implies, by compiling it. For each question:

- **Q1**: write the C a binding past `...` compiles to today (the reproducer's `--emit-c`), then
  the wrapper header route (R) names: a header of the program's own declaring `slot_read` at a
  fixed prototype, `long slot_read_one(int count, long value)` or the spelling you find right,
  and show it builds and prints `-1`. Measure in C, on this Mac and in the Linux arm64 container
  if Docker answers (image `heroes-linux-arm64:latest`; one try, then say if it cannot), what a
  narrower integer past `...` reads under `va_arg` of `int`, `long`, `long long`, `size_t`,
  `mode_t`, `double`; and say for each whether the C standard's default promotions make it
  defined. Name the real C variadics a Heroes program binds today (grep `extern` groups of
  `examples/`, `tests/golden/` and `packages/`) and which of them route (R) refuses. Windows is
  unrun: say what your C says for a 32-bit `long`, as reasoning.
- **Q2**: run defect 2428's reproducer; then show, with the include listing clang gives (`-H`,
  `-v`), which `curl/curl.h` each build reads with and without `PKG_CONFIG_PATH`, and what a
  build that wants the keg-only library must and must not read. Is the SDK's `curl/curl.h` ever
  read in the keg's place, and could it be?
- **Q3**: compile the shapes `d` and the folder-above form with `-H` and say which file C reads
  and from which folder; and whether any `#include <sys/timeb.h>` of the platform or runtime
  would read the program's file in those shapes.

Report: per question, the C you compiled, the commands, what each printed, your verdict.

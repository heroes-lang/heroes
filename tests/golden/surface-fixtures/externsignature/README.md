# externsignature — acceptance row 4, as programs

M-separate-compilation acceptance row 4 (panel 030 R2, re-read by panel
093 R5): a two-module program whose `extern` signature the header
refuses. The row was tabled as *"exit 1 in both TUs"* and the sitting
measured that unsatisfiable — the caller is **correct**, and the mistake
is one module over, so there is no second failing unit to demand. What
R5 put in its place is three claims, and these files carry the first two:

1. every dirty translation unit compiles before any verdict is rendered;
2. the declaring TU's clang failure maps to **exit 1 on the `.hero`
   line** — the author's own line, in the module that declared the
   `extern`, not the unit that happened to be compiling;
3. a cache hit never converts a failure into a success.

Four files, two verdicts:

- `bind.hero` — the declaring module. `math.h` says `double
  sqrt(double)`; this says `i64` both ways. `#~ ffi_return_type`, because
  the annotation belongs where the span lands.
- `wrong.hero` — `use bind`, and nothing wrong of its own. Exit 1 at
  `build`, with the caret on `bind.hero:13`.
- `bindright.hero` / `right.hero` — the same program with the signature
  the header actually declares. Exit 0, prints `4.0`. Without this pair
  the case would pass by refusing every program forever.

**The third claim is not here, and cannot be**: it is about the SECOND
invocation, which no `.hero`/`.expected` row can express. It lives in
`tests/harness/suite_cache.hero` as *a warm cache never turns a failure
into a pass* — three builds in a row, then a repair that must reach exit
0.

It lives under `surface-fixtures/` for `externroute/`'s reason, next
door: `cases.collect` requires a `.hero`/`.expected` pair, which a
neighbour module breaks, and `suite_emission` requires every collected
program to emit, which a refused case breaks. What runs these files is
the pair of `suite_surface` rows naming them.

## `link "m"`, and why it is here

Added 2026-08-26, hours after the fixture landed, because **the CI's Linux leg
went red and macOS never would have**. `sqrt` lives in `libm` on Linux and must
be linked explicitly; on Darwin the maths functions are in libSystem, so
`extern "math.h"` with no `link` builds and runs there and nowhere else.

Three checks failed on one cause — this fixture's `surface` row, the
`annotations` sweep (an unannotated `ffi_missing_link` it had never seen), and
`suite_cache`'s row-4 case, which writes the same binding into its own scratch
program. All three are one edit.

It is the platform lesson this repository keeps re-learning from the other
direction: `shell.hero` carries the note about `/tmp`, true of both platforms
the CI covered and false of the third. A binding that works on the machine it
was written on is not a binding that works.

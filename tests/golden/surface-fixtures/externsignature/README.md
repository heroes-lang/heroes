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

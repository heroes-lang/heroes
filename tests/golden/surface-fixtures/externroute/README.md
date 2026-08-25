# externroute — acceptance row 1's two directions, as programs

M-separate-compilation acceptance row 1 (panel 030 R2, answered by panel
033 R5): an `extern` declaration is never callable across a module
boundary; a qualified mention of one — a call, or the function taken as a
value — is `error[extern_across_modules]`, and the route is a Heroes
function in the declaring module.

Three files, two verdicts, both pinned by `suite_surface` rows:

- `bind.hero` — the declaring module: one `extern` group, its wrapper
  `shell`, and a `main` of its own so the module is also a whole program.
- `wrong.hero` — `use bind`, then every refused spelling: the qualified
  call and the qualified value. Exit 1 at `check`, `#~` annotated.
- `right.hero` — the same work through `bind.shell`. Exit 0, runs.

It lives under `surface-fixtures/` because that is the home for
row-driven multi-file cases (`cross/` is the precedent): `cases.collect`
requires a `.hero`/`.expected` pair (a neighbour module breaks it) and
`suite_emission` requires every collected program to emit (a
check-refused case breaks it), so neither collector sweeps here. What
runs these files is the pair of `suite_surface` rows naming them.

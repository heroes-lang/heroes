# releaser085 — a mark's releaser is read in the mark's own module

`bio.hero` declares `h_open() -> H acquires h_close2` and only `main.hero`
declares `h_close2`. Defect 085 (panel 176's ffi-pragmatist, 2026-09-23):
`check bio.hero` was exit 1 with `unread_releaser`, `check main.hero` was exit
0, one program with two verdicts, because `ends_a_life` walked every
declaration of the program while its message said *no `extern` of this module
declares it*. Panel 176's item 5 refused the non-local reading, so the lookup
is now the module's own: both files are refused, and the repair is to declare
`h_close2` beside the mark that names it.

`tests/harness/suite_surface.hero` runs `check` on both files and expects exit
1 with `error[unread_releaser]` from each.

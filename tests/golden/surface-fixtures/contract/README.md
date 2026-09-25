# contract — one C function, one contract

Two modules declare `keep` against one header: `main.hero` with `s: cstr lent`,
`alt.hero` with `s: cstr`. Each is checked against C alone and passes; the two
disagree about what C does with the bytes, and until `contract_differs` landed
(M-agreed-retention, 2026-09-24) nothing compared them: `check` 0, and the
shape is the one that broke upstream Clang's `noescape` on its first day.

The rule is program-wide, like `one_tag_one_type`: a C symbol is one identity
across every module that names it, so the positions two declarations share
must say the same thing, at one type and one spelling of the marks. The
`twoarity` fixture beside this one is the legal case — two arities of `printf`
that share one position and agree on it.

`tests/harness/suite_surface.hero` runs `check main.hero` and expects exit 1
with `error[contract_differs]`.

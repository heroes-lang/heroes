# M-harness-port — the net, in Heroes *(closed 2026-08-18)*


The 4,279 lines of Rust harness (`golden` 1,095 · `surface` 2,105 · `corpus` 544
· `milestones` 310 · `layout` 116 · `expectation` 109) became a Heroes program
run by the one command. **No compiler lines and no new surface**: a 79-line probe
ran 65 of 65 `check/` cases green, and exit codes, separate streams, `getenv` and
file comparison were each measured reachable. Predicted **2,500–3,760 lines**,
centre ~3,200, by three independent ratios; landed at 3,630.

Two things a straight port would have missed. `layout.rs` was a **rewrite** — it
walked `crates/**/*.rs`, the compiler became `selfhost/*.hero`, and 29 of 143
selfhost files already exceeded §11's 300 lines, so the check had to carry the
knot rule or go red on day one. And a directory walk had no sound cheap route:
`ls > file` plus `read_file` **splits a filename containing a newline into two
entries at exit 0**, measured, which §1.12 forbids — so it is
`popen`/`fgets`/`pclose`, measured working on both compilers, with the wait
status from `pclose`.

*******************************************************************************
**OPEN: 0**

*******************************************************************************

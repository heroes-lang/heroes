- [x] M-selfhost-port, 2026-08-17 | **THE MEASUREMENT THAT ROUTED A MILESTONE, kept whole.** It is CLAUDE.md §13's named case — performance is not a goal *and not a licence* — and the numbers below are what made the fixpoint's cost the author's to route rather than an assistant's optimisation instinct. The measurement verbatim:

  The port compiles small programs instantly and its own 34,512 lines not at
  all within 20 minutes. Measured with the -O0 selfhost binary, `check` only
  (frontend, no emit), wall clock:

  | input | modules pulled in | time |
  |---|---|---|
  | `token.hero` (193 lines) | 2 | 0s |
  | `parse.hero` | the lexer + parser subtree | 24s |
  | `checker.hero` | the frontend subtree | 152s |
  | `emit.hero` | + the whole backend | >600s (killed) |
  | `main.hero` (everything) | 143 | >1200s (killed) |

  Superlinear, and the shape says where to look before any guess: every
  `for … in xs` over a growing `[T]` in this port copies (COW value
  semantics, §4.20), and several walks are O(n) scans inside O(n) loops —
  `out_edges` in check_sized rescans every edge per node, `resolve_state`'s
  `top_visible` scans the table, `emit_ctype`'s tables are maps keyed by
  declaration. None of that is a language defect; it is the port written the
  way the Rust reads rather than the way this language costs.

  **This is CLAUDE.md §13's named case, and it is the author's to route.**
  Performance is not a goal *and not a licence*: where a cost stops a program
  the closure list needs from running at all, that is §1.0 compiler-need and
  it goes to the panel rather than to an assistant's optimisation instinct.
  The fixpoint (M-selfhost-fixpoint) is the v1 finish line and it cannot be
  reached at these numbers.

  **Measurement (a) is now done, and it moves the numbers without moving the
  question.** The same inputs, the selfhost binary built at -O2 instead of
  -O0 (4.1 MB against 6.3 MB):

  | input | -O0 | -O2 | ratio |
  |---|---|---|---|
  | `token.hero` | 0s | 1s | — (both noise) |
  | `parse.hero` | 24s | 8s | 3.0x |
  | `checker.hero` | 152s | 54s | 2.8x |

  A constant factor of ~2.8x against a superlinear curve: it buys one step
  along the curve, not the shape of it. Whether it is enough for all 143
  modules is being measured as this entry is written and the answer belongs
  under it.

  **Measurement (b) is still owed, and it is the one a sitting needs**: a
  profile naming the top three call sites, so the panel is briefed with where
  the time goes rather than with the shape argument above — which is a
  reading of the code, not a measurement of it (§1's rule about an inference
  presented as a measurement). It needs an instrument this project does not
  have on the surface, and adding one is itself a §10 stopping-rule question:
  `--profile` would be a flag nothing in the fixpoint invocation types.
  The cheap alternative that needs no surface at all is `sample` (macOS) or
  `perf` (Linux) against the running binary, which is the machine's
  instrument rather than this compiler's.

  | CLAUDE.md §13 · docs/journal/021-selfhost-port.md | a number nobody wrote down is a number the next sitting invents

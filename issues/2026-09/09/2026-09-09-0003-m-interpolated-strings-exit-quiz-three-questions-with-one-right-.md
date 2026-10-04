- [ ] **M-interpolated-strings, exit quiz** | Three questions with one right answer each: why does `f"{m["k"]}"` not compile and what does the note say to write; why is the desugar in the lowering and not in the parser; and why did four decided ceilings move rather than four helper modules taking the code | `tests/golden/check/hole-not-renderable.hero` · `tests/harness/suite_layout.hero` § DECIDED | a helper that calls back into a knot is a `use` cycle, and the knot clause is what the ceiling is for

    **Origin:** M-interpolated-strings close, 2026-09-09.

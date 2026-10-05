---
kind: learn
area: none
milestone: M-stated-grammar
filed: 2026-09-12
commit: none
github: none
---

- [ ] **M-stated-grammar, the two spellings** | `grammar/powers` reads `binary_op`'s arms out of the source as text and compares them to what `heroes grammar` prints. Panel 067's defect was `.percent => .add`. Say whether that check catches it, and why — then say which check does.

    **Origin:** M-stated-grammar close, 2026-09-12. Falsified both ways that day; the answer is in `tests/harness/suite_grammar.hero`'s comment on `powers`.

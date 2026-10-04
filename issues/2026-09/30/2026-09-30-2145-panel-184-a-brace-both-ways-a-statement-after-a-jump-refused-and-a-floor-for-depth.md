---
kind: decision
area: none
milestone: none
filed: 2026-09-30
commit: 93375d63594c5e734addefafdd2c6668cabbfa07
github: none
---

# Panel 184: a brace written both ways, a statement after a jump refused, and a floor for depth

2026-09-30, evening: the three language questions the author's decision of the
morning sent to one sitting (the forgotten `f`, a statement after a jump, a
nesting limit told as a message).

## The decision

| | |
|---|---|
| date | 2026-09-30 |
| decision | **provisional, author ratification pending**: in an `f` literal `{{` and `}}` each write one brace and a lone `}` is an error, landing in two stages; `unused_binding` on a name a plain literal holds in braces points at the literal, with the `f` as a guess; no refusal of a forgotten `f` in this sitting, the question whether design.md §1.3's locality test reaches a rule of legality put to the author; a statement after `return`, `break` or `continue` in its block is refused, `exit(code:)`, `assert false` and a `while true` with no `break` satisfy `missing_return`, and the spec states the return rule; the `match` defect repaired first under its own number; the compiler's passes on a thread whose stack it chooses; the spec states a floor N for depth, measured on three platforms, and refuses no source for its depth; (3e) queued until built |
| reason | the seats' measurements (`docs/panel/184-a-brace-is-written-both-ways-a-statement-after-a-jump-is-refused-and-depth-is-the-compilers-to-hold.md`): the stray brace found by four seats apart and measured; 34 false alarms and 0 true ones for the shape-only refusal; 0 false alarms and 17 of 17 for the scope-reading one, held under the blind seat's veto on the wording it judged; the jump refusal moving 0 tracked files and silent over-indent 68 to 49 of 2,000; the compiler unable to check itself at `ulimit -s 512` without the thread; a stack alone removing no abort |
| design.md § | §4.17, §1.3, §1.12 |
| panel | 184 |

## What is still to happen

The ratification item is `panel 184` in `docs/work/DECIDE.md`. The `match`
defect, the depth of types and the FFI seat's three side findings are filed as
defects; the landing of R1, R4, R5 and R6 is a milestone's work with the
conditions the synthesis names.

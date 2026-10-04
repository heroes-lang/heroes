- [x] **205 — a closer of another kind where a map entry's `:` goes is told without the `{` or its closer** | `m = {1: 2` over `print(x) )`: `expected_map_entry_colon` at the `)`, *expected `:` between a map's key and its value, found `)`*, naming no `{` and no `}`; `{1: 2` over `y]` the same | `selfhost/grammar_expr.hero:567` (`map_literal`'s `line_end.expect_after` for the `:`) · defect 203's message, the separator's, which names them (`selfhost/parse/list_line.hero`, `another_kind`) · **class: adjacent**

    **Origin:** lane rec187's first pass beside defect 203, 2026-10-03, on the head's compiler and on the lane's (`scratchpad/lane-rec187/pass1/r6/r04_map_paren.hero` and `r12_map_bracket.hero`, 2026-10-03). The same reading as 203's at another site: the line is read as the map's next key, and the closer stands where its `:` goes.

    **Why it is a defect.** As 203: one reading served, the other's repair a run more (design.md §4.17's measure).

    **Class: adjacent**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): a true message less exact than it could be.

    Repaired at `9642191b` (2026-10-04, lane b9-recovery), gated by its cases and the compiler's own tests; the net is owed at the batch's close.

## The repair

Repaired at `9642191b`. A closer of another kind where a map key's `:` goes is told with the `{` still open and where, and the two edits: the line as the map's last entry, or a statement the map took in. Its cases are `fixedbugs-205-a-closer-where-a-keys-colon-goes-names-the-brace`, seven shapes. The advice inside a call is filed as defect 310.

**Closed 2026-10-04** with batch 9 (lanes b9-notext, b9-emit, b9-harness, b9-recovery and b9-annot, merged into one round tree with the trunk at `f6a3122e`), its closing gate run on the round's head from `2c58b28e` to `662870e6`, no line of `selfhost/`, `runtime/` or the seed moving between, with the seed regenerated: 41,364,146 bytes, SHA-256 beginning `26ccaa9d96478a20`, its fixpoint by `cmp`; the compiler's own tests 1,190, all passed; the net's own tests 246, all passed; the full net, 27 suites, 5,268 passed and 0 failed, `fixes` read alone after `662870e6`, which stopped that suite copying the byte fixtures of defects 227 and 241 as text. The census, the trunk's compiler at `703af779` against the round's over the tree's tracked files, each with its own runtime: `check --brief` over 1,993, 34 moved, and `build --emit-c` over the 621 holding an `extern`, 3 files of C and 22 of messages moved, every one the batch's own. Panel 187's R2, the trunk's compiler against the round's over one frozen plan: 13,594 single mutants, 68 fewer messages in the normal arm and 71 in the control arm and none more; 15,842 pairs, no told second hidden. The site's build: 188 pages, 36 claims and 2 verb lists checked.

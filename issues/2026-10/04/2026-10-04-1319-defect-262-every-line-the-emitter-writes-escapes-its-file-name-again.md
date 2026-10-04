---
kind: defect
area: emit
milestone: none
filed: 2026-10-03
commit: 4084f886a54a2e4ff475a6bdaafc016282a22806
github: none
---

- [x] **262 — every `#line` the emitter writes escapes its file name again** | 25,545,276 calls for 1,155,601 directives in the compiler's own emission (batch 8's emit lane, counted in an instrumented copy of its compiler's C (`build --emit-c`, the lane's tip `96473596`), 2026-10-03, its report's *Found beside*) | `selfhost/emit/c_text.hero` (the `#line` name's escaping, which defect 240 also touches) · **class: improvement**

    **Origin:** batch 8's emit lane, counted in an instrumented copy of its compiler's C (`build --emit-c`, the lane's tip `96473596`), 2026-10-03, its report's *Found beside*.

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a cost per directive that one escape per file would pay once; no program refused or wrong.

    Repaired at `4084f886`, 2026-10-04, gated by its cases and the compiler's own tests; the net is owed at the batch's close.

## The repair

Repaired at `4084f886`. The writer spells a file's `#line` name once and writes the spelling it holds at each directive: building the compiler's own C, `line_name` is called 1,224 times over 27,034 bytes where it was called 1,174,304 times over 25,589,526, for 1,174,498 directives, the output byte for byte the same.

**Closed 2026-10-04** with batch 9 (lanes b9-notext, b9-emit, b9-harness, b9-recovery and b9-annot, merged into one round tree with the trunk at `f6a3122e`), its closing gate run on the round's head from `2c58b28e` to `662870e6`, no line of `selfhost/`, `runtime/` or the seed moving between, with the seed regenerated: 41,364,146 bytes, SHA-256 beginning `26ccaa9d96478a20`, its fixpoint by `cmp`; the compiler's own tests 1,190, all passed; the net's own tests 246, all passed; the full net, 27 suites, 5,268 passed and 0 failed, `fixes` read alone after `662870e6`, which stopped that suite copying the byte fixtures of defects 227 and 241 as text. The census, the trunk's compiler at `703af779` against the round's over the tree's tracked files, each with its own runtime: `check --brief` over 1,993, 34 moved, and `build --emit-c` over the 621 holding an `extern`, 3 files of C and 22 of messages moved, every one the batch's own. Panel 187's R2, the trunk's compiler against the round's over one frozen plan: 13,594 single mutants, 68 fewer messages in the normal arm and 71 in the control arm and none more; 15,842 pairs, no told second hidden. The site's build: 188 pages, 36 claims and 2 verb lists checked.

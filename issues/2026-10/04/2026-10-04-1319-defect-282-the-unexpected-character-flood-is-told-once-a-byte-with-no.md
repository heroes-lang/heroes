---
kind: defect
area: compiler
milestone: none
filed: 2026-10-04
commit: f34d8eba10c556b6b0982ce5938dc0aea1236646
github: none
---

- [x] **282 — the `unexpected_character` flood is told once a byte with no bound, and each telling rebuilds a table, so 32 KB of control bytes cost 31,600 messages** | a file of 32 KB of control bytes: `check` exit 1 with 31,600 diagnostics and 8.7 billion instructions retired, `shown_char.named` rebuilding its `UNSEEN` table at every call (panel 189's compiler-engineer, `<scratchpad>/189-compiler-engineer/q6/unseen/`, 2026-10-04, counted on the trunk's compiler) | the lexer's `unexpected_character` (`selfhost/lexer.hero`) and `selfhost/shown_char.hero` (`named`, and the `UNSEEN` table it rebuilds) · defect 247, a two-line file's thirty, the same telling small · **class: adjacent**

    **Origin:** panel 189's compiler-engineer, its Q6 (`docs/panel/189-reports/compiler-engineer.md`), which called it *filed apart*; found unfiled by the critic's second pass (`git grep` of `UNSEEN`, `shown_char`, `31,600` over `docs/work/defects/`) and filed by the sitting's R12.

    **Class: adjacent**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): one mistake told thirty thousand times, at a cost growing with the table at each telling; not 227's cause, the file being UTF-8.

    Repaired at `f34d8eba`, 2026-10-04, gated by its cases and the compiler's own tests; the net is owed at the batch's close.

## The repair

Repaired at `f34d8eba`. A run of one refused character that cannot be seen is told once, at its first, with a note counting it, and past eight such messages in a text the rest are counted on the last one told: 8 messages and 2.2 billion instructions for 32 KB of control bytes, where there were 31,600 and 8.8 billion.

**Closed 2026-10-04** with batch 9 (lanes b9-notext, b9-emit, b9-harness, b9-recovery and b9-annot, merged into one round tree with the trunk at `f6a3122e`), its closing gate run on the round's head from `2c58b28e` to `662870e6`, no line of `selfhost/`, `runtime/` or the seed moving between, with the seed regenerated: 41,364,146 bytes, SHA-256 beginning `26ccaa9d96478a20`, its fixpoint by `cmp`; the compiler's own tests 1,190, all passed; the net's own tests 246, all passed; the full net, 27 suites, 5,268 passed and 0 failed, `fixes` read alone after `662870e6`, which stopped that suite copying the byte fixtures of defects 227 and 241 as text. The census, the trunk's compiler at `703af779` against the round's over the tree's tracked files, each with its own runtime: `check --brief` over 1,993, 34 moved, and `build --emit-c` over the 621 holding an `extern`, 3 files of C and 22 of messages moved, every one the batch's own. Panel 187's R2, the trunk's compiler against the round's over one frozen plan: 13,594 single mutants, 68 fewer messages in the normal arm and 71 in the control arm and none more; 15,842 pairs, no told second hidden. The site's build: 188 pages, 36 claims and 2 verb lists checked.

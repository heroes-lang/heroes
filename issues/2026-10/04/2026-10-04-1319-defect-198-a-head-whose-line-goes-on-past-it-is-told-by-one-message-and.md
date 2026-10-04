---
kind: defect
area: parse
milestone: none
filed: 2026-10-02
commit: a6eeb4e669596d0c5b5a74900a5a7d5f667be212
github: none
---

- [x] **198 — a head whose line goes on past it is told by one message and the rest of its line dropped, so a stray closer on it is told only once the first is repaired** | `variant T )` and `record Point )` with nothing below: `empty_variant` and `empty_record`, the `)` untold; `function f(): i64 )` over its body: `expected_end_of_line` at the `:`, the `)` untold until `->` replaces the `:`; with no body, `missing_body` (found `:`) and the `)` untold | `selfhost/parse/members_below.hero:87` (`skip_line` after a record's or a variant's head) · `selfhost/parse/opening.hero:192` (`drop_rest_of_line` after a function's) · pinned by `tests/golden/check/panel-187-a-heads-line-that-goes-on-is-told-once.hero` · **class: adjacent**

    **Origin:** panel 187's compiler engineer, its § 1's cause B1 (`docs/panel/187-reports/compiler-engineer.md`, 2026-10-02), on the audit's rows 131-16a, 131-16b, 131-53a and 131-53b; filed by lane rec187 under the sitting's R1 with its pin, which reads byte for byte the same on the head's compiler and on `29425af6` (2026-10-03).

    **Class: adjacent**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery, panel 187's R1): a mistake told only once another is fixed, class (b).

    Repaired at `a6eeb4e6` (2026-10-04, lane b9-recovery), gated by its cases and the compiler's own tests; the net is owed at the batch's close.

## The repair

Repaired at `a6eeb4e6`. A head's junk is told over members and over none, the empty block beside it, and a closer one too many on a head's line is told in the same run, as a failed statement's is; an `extern` group's `expected_extern_block` stands at its head. Its cases are `fixedbugs-198-a-heads-junk-and-a-closer-past-it-are-told`, twelve shapes. Its pin, `panel-187-a-heads-line-that-goes-on-is-told-once`, moved from 4 messages to 8, each added one a stray `)` on a head's line told in the same run where it waited for another fix, and `fixedbugs-131-a-record-or-variant-head-that-goes-on-over-its-members` from 8 to 11 the same way; `fixedbugs-131-a-colon-in-mid-line-left-to-its-head` moved from 5 to 6, the member `variant T: a` writes on its head's line now told beside the empty block, a second message for one mistake filed as defect 315. All read at the gate.

**Closed 2026-10-04** with batch 9 (lanes b9-notext, b9-emit, b9-harness, b9-recovery and b9-annot, merged into one round tree with the trunk at `f6a3122e`), its closing gate run on the round's head from `2c58b28e` to `662870e6`, no line of `selfhost/`, `runtime/` or the seed moving between, with the seed regenerated: 41,364,146 bytes, SHA-256 beginning `26ccaa9d96478a20`, its fixpoint by `cmp`; the compiler's own tests 1,190, all passed; the net's own tests 246, all passed; the full net, 27 suites, 5,268 passed and 0 failed, `fixes` read alone after `662870e6`, which stopped that suite copying the byte fixtures of defects 227 and 241 as text. The census, the trunk's compiler at `703af779` against the round's over the tree's tracked files, each with its own runtime: `check --brief` over 1,993, 34 moved, and `build --emit-c` over the 621 holding an `extern`, 3 files of C and 22 of messages moved, every one the batch's own. Panel 187's R2, the trunk's compiler against the round's over one frozen plan: 13,594 single mutants, 68 fewer messages in the normal arm and 71 in the control arm and none more; 15,842 pairs, no told second hidden. The site's build: 188 pages, 36 claims and 2 verb lists checked.

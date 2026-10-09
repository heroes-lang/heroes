---
kind: defect
area: cli
milestone: none
filed: 2026-10-04
commit: bcb68219a3daa3356aa6d7da690a63f73d91e131
github: none
---

- [x] **322 — `-gline-tables-only` in place of `-g` would cut clang's memory at `-O2` on a function of many returns by more than two fifths, a route nobody listed** | panel 190's completeness critic, 2026-10-04, by clang's own peak footprint at `-O2` with `flags()`'s sixteen words: on the 400-return shape the trunk's C needs 1,695,369,184 bytes under `-g` and 970,589,672 under `-gline-tables-only` (−42.7%), and route A-star's 1,985,595,312 and 913,409,056, below the trunk's; at 800 unrun; what it costs, read and not run: C-level variable inspection in lldb, which design.md Part 2 describes and does not promise, while line stepping, `-g`'s one reason in `flags.hero`, is kept | `selfhost/cli/flags.hero` (`flags()`, its `-g`) · `.claude/rules/generated-c.md` § Flags · design.md Part 2 (`:630` to `:632`) · panel 190's R11 · **class: improvement**

    **Origin:** panel 190's completeness critic, second pass, 2026-10-04 (`docs/panel/190-reports/completeness-critic.md` § 6, `<scratchpad>/190-critic/pass2/fpx.sh`).

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a cost on extreme shapes, nobody's program wrong for it; the flag list touches every program's debug information, a question for a sitting of its own (CLAUDE.md § 4, architecture).

    Repaired at `bcb68219`, 2026-10-08 (lane b14-land197), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Panel 197's R2 (ratified 2026-10-07), not this item's own route (B): `-g` left `flags()` for the level, `-g` at `-O0` and `-gline-tables-only` at `-O2`, in each unit's words, the runtime object's key, the identity probe and the link, so the constant factor is what closes: the N-return shape's unit at `-O2`, replayed alone with the compiler's own words, peaks at 77, 257, 887 and 3,404 MB at 100, 200, 400 and 800 returns where it peaked at 175, 579, 2,023 and 8,017 MB (two reads each, -56% to -58%), its instructions -11% to -6%; the growth, about fourfold per doubling under both words, is filed apart as defect 470.

    **2026-10-09**: the CI's Windows leg on `811f8398` read this defect's case red at `--sanitize`, ASan naming no frame under line tables at `-O2` on clang 20.1.8 (defect 509); the item stays open until 509 is repaired and that leg reads the case green.

## The repair

Repaired at `bcb68219`, 2026-10-08 (lane b14-land197), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Panel 197's R2 (ratified 2026-10-07), not this item's own route (B): `-g` left `flags()` for the level, `-g` at `-O0` and `-gline-tables-only` at `-O2`, in each unit's words, the runtime object's key, the identity probe and the link, so the constant factor is what closes: the N-return shape's unit at `-O2`, replayed alone with the compiler's own words, peaks at 77, 257, 887 and 3,404 MB at 100, 200, 400 and 800 returns where it peaked at 175, 579, 2,023 and 8,017 MB (two reads each, -56% to -58%), its instructions -11% to -6%; the growth, about fourfold per doubling under both words, is filed apart as defect 470.

**Closed 2026-10-09** with batch 16 (lanes b16-emit, b16-runtime, b16-land199, b15-parse, b16-tools, b16-compiler, b16-land198 and b16-misc, merged into one round tree made from the trunk), its closing gate run on the round at `164e699a`: the seed regenerated over two generations, the runtime's ABI staying at 29, 46,097,621 bytes, SHA-256 beginning `d3bf3451eae3f3e8`, its fixpoint by `cmp`; the compiler's own tests 1,531, all passed; the net's own tests 322, all passed; the full net 7,237 passed over 29 suites, 0 failed, `run` taken one harness process per case and its one red the skip ratio of a one-case run (defect 437's case, which binds `sys/prctl.h`, skipped on this Mac as in every whole run). It is the first batch under the optimistic chain (author instruction 2026-10-09, `.claude/rules/verification.md` § The optimistic chain): the census and panel 187's R2 run after the push beside the CI, no platform leg ran before it, and a defect at the C boundary closes here, a CI leg red on its case filing a new `blocking` defect naming it. A trial of the same gate on the round at `2dbbf7e6`, before lane b16-misc's four repairs, read the full net 7,226 and 0, the census's every move attributed and R2's one finding read as defect 457's rule speaking.

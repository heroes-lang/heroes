---
kind: defect
area: parse
milestone: none
filed: 2026-10-04
commit: 038a90fcd3b78be585a6c4b265eeca558b054e80
github: none
---

- [x] **315 — a member written on its head's line over nothing, `variant T: a`, is told twice: the head's junk and the empty block, where moving the member below cures both** | `variant T: a` over nothing: *expected the end of the line after `variant T`, found `:` — its cases go on the lines below it*, at the `:`, and `empty_variant` at the head; `record R: x` the same with `empty_record`; the trunk's compiler at `703af779` told the empty block alone, and with a member below either compiler tells the junk alone (batch 9's round compiler, built from the seed beginning `26ccaa9d`, 2026-10-04, `<scratchpad>/p315/`); the census at batch 9's gate moved `check/fixedbugs-131-a-colon-in-mid-line-left-to-its-head` from 5 messages to 6 by it | `selfhost/parse/members_below.hero` (`no_members_below`, the junk told over no members with the empty block beside it) · defect 198's repair, `a6eeb4e6` · **class: adjacent**

    **Origin:** the coordinator, 2026-10-04, reading at batch 9's gate the goldens its census moved (panel 187's R2, a worse move filed by its class); each shape run on both compilers. Where the junk is a stray closer, `variant T )` over nothing, the block is empty indeed and the two messages name two mistakes, as `check/panel-187-a-heads-line-that-goes-on-is-told-once` pins.

    **Class: adjacent**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a second message for one mistake, both true, no fix certain; the trunk's one message named the empty block and not the member on the head's line, so it cost a second exchange, which the round's two messages do not.

    Repaired at `2ee246ca`, 2026-10-05 (lane b11-parse), gated by its cases and the compiler's own tests; the net is owed at the batch's close.

    **2026-10-05, the shapes with this cause the repair leaves at two messages** (lane b11-parse's report): a generic head with a field after its type parameters, `record Pair<T>: x: T`, and `record R = x: i64` are still told the member on the head's line and the empty record both. They are this item's cause, so they stay its rows and are owed before it closes. `variant T a` keeps two on purpose: `record R extends Base` reads the same, and there the record is empty.

    Repaired at `038a90fc`, 2026-10-05 (lane b11-parse), its two rows: the members read past a head's type parameters and after a `=`; gated by its cases and the compiler's own tests; the net is owed at the batch's close.

## The repair

Repaired at `2ee246ca` and, for its two rows of the same cause, at `038a90fc`. A member written on its head's line over nothing, `variant T: a`, was told the member and the empty record both; where the text after the head is the members themselves, the empty record's message is no longer said, past type parameters and after `=` too (`record Pair<T>: x: T`, `record R = x: i64`). `variant T a` keeps two messages on purpose: `record R extends Base` reads the same, and there the record is empty. Its case is `fixedbugs-315-*`; `fixedbugs-131-a-colon-in-mid-line-left-to-its-head` goes from six messages to five.

**Closed 2026-10-05** with batch 11 (lanes b11-misc, b11-windows and b11-parse, merged into one round tree made from the trunk at `f5194276`, the trunk merged again at `1bbf2dd7`), its closing gate run on the round's head: the seed regenerated at `97ff7ce8`, 35,479,762 bytes, SHA-256 beginning `f6c1d06c25596494`, its fixpoint by `cmp`, and the compiler's own tests 1,233, all passed; the merge at `1bbf2dd7` moved no line of `selfhost/`, `runtime/` or the seed; then the net's own tests 280, all passed, and the full net, 27 suites and `cache`, 5,365 passed and 0 failed. The census, the trunk's compiler at `f5194276` against the round's over the tree's tracked files, each with its own runtime: `check --brief` over 2,033, 13 moved, the batch's own seven cases and six older goldens its repairs move, each as its repair says; `build --emit-c` over the 623 holding an `extern`, 3 exits moved (the compiler's own source, which the trunk's runtime cannot build, and defect 337's two cases), 2 files of C moved, and 16 of messages, 15 by defect 327's one warning a build and the build cache's key alone, one by defect 311's words. Panel 187's R2, its instrument rebuilt (defect 358), batch 10's compiler, whose source differs from the trunk's at `f5194276` in comments alone, against the round's over one frozen plan: 13,594 single mutants, no class, flag or message count moved in either arm; 16,041 pairs, no second's told status moved. The site's build: 36 claims and 2 verb lists checked. The clock, on a still machine over the trunk's compiler source: `check` 5.70 s against the trunk's 5.68 s, the means of three, and `build --emit-c` 69.33, 69.30 and 69.11 s against the trunk's 70.03 and 70.11 s warm, every run's `real` within 2% of `user` plus `sys`, the C the two emit byte-identical.

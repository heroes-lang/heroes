---
kind: defect
area: parse
milestone: none
filed: 2026-10-04
commit: 2ee246cadc5b3095d8ae7b8db9fd6f85dbdb8b16
github: none
---

- [ ] **315 — a member written on its head's line over nothing, `variant T: a`, is told twice: the head's junk and the empty block, where moving the member below cures both** | `variant T: a` over nothing: *expected the end of the line after `variant T`, found `:` — its cases go on the lines below it*, at the `:`, and `empty_variant` at the head; `record R: x` the same with `empty_record`; the trunk's compiler at `703af779` told the empty block alone, and with a member below either compiler tells the junk alone (batch 9's round compiler, built from the seed beginning `26ccaa9d`, 2026-10-04, `<scratchpad>/p315/`); the census at batch 9's gate moved `check/fixedbugs-131-a-colon-in-mid-line-left-to-its-head` from 5 messages to 6 by it | `selfhost/parse/members_below.hero` (`no_members_below`, the junk told over no members with the empty block beside it) · defect 198's repair, `a6eeb4e6` · **class: adjacent**

    **Origin:** the coordinator, 2026-10-04, reading at batch 9's gate the goldens its census moved (panel 187's R2, a worse move filed by its class); each shape run on both compilers. Where the junk is a stray closer, `variant T )` over nothing, the block is empty indeed and the two messages name two mistakes, as `check/panel-187-a-heads-line-that-goes-on-is-told-once` pins.

    **Class: adjacent**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a second message for one mistake, both true, no fix certain; the trunk's one message named the empty block and not the member on the head's line, so it cost a second exchange, which the round's two messages do not.

    Repaired at `2ee246ca`, 2026-10-05 (lane b11-parse), gated by its cases and the compiler's own tests; the net is owed at the batch's close.

    **2026-10-05, the shapes with this cause the repair leaves at two messages** (lane b11-parse's report): a generic head with a field after its type parameters, `record Pair<T>: x: T`, and `record R = x: i64` are still told the member on the head's line and the empty record both. They are this item's cause, so they stay its rows and are owed before it closes. `variant T a` keeps two on purpose: `record R extends Base` reads the same, and there the record is empty.

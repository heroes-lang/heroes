---
kind: defect
area: resolve
milestone: none
filed: 2026-10-06
commit: none
github: none
---

- [ ] **401 — a binding named `zero` hides what the words `rest: zero` mean** | with `zero = 0` bound, `add(a: 1, rest: zero)` is told only `unused_binding` for `zero`, and `rest_not_a_construction` appears only once the binding is removed; `.circle(r: 2, rest: zero)` on a variant case is told only `unknown_name: zero`, never naming the words (lane b13-bs's measurement on its branch, which lands panel 194's R1; not re-run by the coordinator) | the resolver's reading of the stripped `zero`, `selfhost/resolve/` · panel 194 R1 · **class: adjacent**

    **Origin:** lane b13-bs, 2026-10-06 (its report, *found beside* 2); filed by the coordinator at 19:52.

    **Class: adjacent**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): a mistake told only after another is fixed, and a message that does not name the words; R1's own surface, landing in this batch. The lane recommends that `rest: zero` be refused at the words whenever a binding named `zero` is visible, since a reader cannot tell the two meanings apart.

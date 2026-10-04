---
kind: decision
area: none
milestone: none
filed: 2026-09-27
commit: 70db9f6c1da6de73a0c5f4de045b64e5884d7800
github: none
---

# A split subtraction is a thesis rule, so the control arm reads it as a language without the rule

2026-09-27 | `spaced_minus_element` joins `diag.is_thesis_rule`, so `check
--permissive` drops it and reads `[a` / `- b]` as the two elements a language
without the rule reads, while `line_end_before_continuation` stays out of the
list | the list holds the rules the thesis adds, *as opposed to a rule without
which the program has no meaning* (`selfhost/diag.hero`), and without panel
180's refusal the program still has one, the silent reading the rule exists to
refuse; without the other the program is refused all the same, so it is not a
thesis rule; measured on the trunk with the change: `check` exit 1 with
`spaced_minus_element`, `check --permissive` exit 0 | design.md Part 11, §1.4
| the coordinator's ruling on a question panel 180 did not ask, raised by the
lane that landed it; queued with that sitting's ratification

---
kind: decision
area: none
milestone: none
filed: 2026-09-25
commit: 7dc739251188afc0670eb51771b8350e376b1cfd
github: none
---

# A transfer that was made ends the program's own name for the value

2026-09-25 | a value handed over by `transfers` is dead in the program once the
transfer is made, as a value given back is: route M (must) refuses a read of
the name on every path after it, the poison empties the binding, and § 13's
transfer sentence ends *once made, the program's own name for it has ended as
if given back* | the other reading, the name as a borrowed reference while the
receiver lives, needs the runtime to see the receiver free the child, and it
cannot; panel 176's historian named the question (its Risk 2) and neither
sitting ruled it | design.md §1.12, spec § 13 | decided at panel 177's
landing, step 14, with no sitting's ruling; queued for the author's
ratification in panel 177's `docs/work/DECIDE.md` item

## What it costs a program

The json-c README's order, adding a child and then filling it, is written the
other way round: fill the child, then hand it to its parent. +18 vendored and
+19 real tokens in § 13, in the ledger row of panel 177's landing.

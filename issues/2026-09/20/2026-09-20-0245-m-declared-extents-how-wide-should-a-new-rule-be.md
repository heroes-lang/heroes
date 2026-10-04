- [ ] **M-declared-extents walkthrough** | Defect 067: `Box(p: x)` put a lease pointer somewhere `end_lease` could not reach, and the rule that should have caught it says a lend may stand *"only as an argument of a call"*. Before reading: a record's construction IS a call. **Four other construction shapes were run before the clause was written — a variant case, an array literal, a map literal, and `ok(x)`. How many of those four were already refused, and what does the answer decide about how wide the new rule should be?** | `selfhost/check/lending.hero`'s `constructs_a_record`

    **Where to look after answering:** all four. The record constructor was the
    **one** door, which is why the clause names that shape rather than widening
    to every construction — the tidy rule would have been the wrong one, and the
    only way to know was to run the four.

    **The question to carry away.** The module's own doc claims it closes
    `Rec(field: s.cstr())`, and the sentence is true — outside a group. Ask what
    made the sentence true and the rule holed at the same time, and what that
    says about a comment as a guard.

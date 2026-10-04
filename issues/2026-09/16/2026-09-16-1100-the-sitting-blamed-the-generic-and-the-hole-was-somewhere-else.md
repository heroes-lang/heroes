# The sitting blamed the generic, and the hole was somewhere else

2026-09-16. Panel 155, at M-check-completeness. Five seats and a completeness
critic, convened to decide whether a generic call should be refused when the
types it binds make the generic's body illegal. The resolution is provisional and
queued; this entry is what the sitting decided and why.

## The question was aimed at the wrong thing, and the seats measured it

Both remaining faces of this milestone have `check` 0 and `build` 0. They agree.
The milestone is named *what `heroes check` accepts, `heroes build` compiles*, and
the one face that ever violated that promise was `sort`, closed on the morning of
this sitting by a **body** refusal. The divergence in the other two is at **run**.

Then the ffi-pragmatist attacked the shapes beside the one it was asked about and
found the hole has nothing to do with generics: a record chain 16 deep ending in
an `f64` map key, or in a `partial` record compared with `==`, is `check` 0 where
15 deep is `check` 1. No generic anywhere. Both walks abandon at the same
`depth > 16` give-up, and both comments call that absence *the safe direction*
because a runtime guard stands behind it. It is defect **046**.

## What was refused, and on what ground

The call-site obligation pass, panel 082 R3's shape, ruled in August and never
built. Refused on **soundness and not on price**: `instantiations` does not record
who was called, so the pass needs a second span-keyed table against
`check/state.hero:78-85`'s standing rule; the compiler has no node-to-declaration
map, which is 150 of the prototype's 284 measured lines against an estimate of
90-110; it duplicates two rules into a third file; and its own walk needs a
`depth > 16` give-up **with no runtime guard behind it**, reproducing in the
repair the shape of the defect it was meant to cure.

The precedent runs the same way and was read first-hand. Stroustrup on his own
templates: *"The lack of well-specified interfaces led to the spectacularly bad
error messages we saw over the years"*, and *"The requirements of `sort` on its
argument type are implicit ('hidden') in its function body."* Go's proposal
rejects the mechanism by name: *"We don't want to derive the constraints from
whatever `Stringify` happens to do."* Zig closed the request `not_planned` after
seven years. And the precedent for what Heroes ships today comes from Go, which
in 1.20 loosened a compile-time refusal into a run-time panic for generic map
keys — so this project's claim to be the only language making it a run-time event
does not survive as written, and only its float-specific half does.

## What was adopted, and it was already written down

`design.md:1721` does not stop at *No constraints. No `where`, no bounds.* It
continues: **"If an operation on `T` is needed, pass it as a parameter."** No seat
cited it; the completeness critic found it.

So `==` on two values of a bare type parameter is refused **in the body**, and a
generic that needs equality takes it as a parameter — which is what the shipped
`sort` diagnostic already recommends in words, and what closed this milestone's
first face. Measured at the synthesis: of the **50** generic functions in this
tree, two use `==` in a body and **neither compares two bare type parameters**, so
the rule deletes nothing here. `spec § 13` then becomes true as written, at **+0**
spec tokens.

The float half waits under Principle 0: no compiler need, no spec sentence behind
it, no measured Part 11 effect, and the body rule there would delete two goldens,
one of which had already been rewritten once to route through the generic this
would close.

And `spec § 10` owes a sentence saying which types may key a map. The
llm-ergonomist, reading only the specification, could not derive the rule the
compiler enforces, and the spec-warden confirmed from the tree that no such
sentence exists. That is owed whether or not anything else lands.

## Two things recorded against the sitting rather than smoothed

**Three of five seats were killed by the watchdog at 600 seconds.** Panel 087
recorded four of five for the same cause; this is the second time. The seats that
measure run long silent commands.

**The coordinator damaged the instrument.** Resuming the two stalled seats, it
handed them the findings of the two that had finished, so their verdicts are not
independent. The split is two independent readings plus two informed
concurrences. The process rule this paid for: **a resumed seat is given its own
brief again, never another seat's answers.** And four numbers in the
coordinator's briefs were carried from documents rather than measured — the
ceiling, the generic count, the seam, and a *provably* that traced to the archived
tree the briefs themselves forbid reading. One of the four cut in the proposal's
favour, and the seat that would have benefited refused to stand on it.

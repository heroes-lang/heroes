# Panel 158 — spec-warden brief

Read `docs/panel/158-briefs/00-shared.md` first. You judge design.md §1.6's
budget and Principle 0's burden, and you have a veto on budget breach.

Measured 2026-09-16 with `./heroes measure spec/heroes-spec.md`: `claude-legacy`
5863, `cl100k_base` **5989**, `real` **7974** (claude-opus-5), ceiling **10240**,
headroom 2266 and 2206 net of the FFI floor.

## What you are asked

**Price all four repairs in spec tokens, both directions.** Three of them change
the document: option 1 (refuse `{K: V?}`) needs a sentence saying so; option 3
(make `T??` legal) may need none at all, or may need one removing the
restriction a reader currently infers; option 4 IS a sentence. Draft each in
your scratchpad and measure it with `./heroes measure <draft>`.

**And the question that is yours alone.** `spec § 10` promises `m[k]` returns a
`V?`. With `V = i64?` that is `i64??` — so the document already promises a type
it never shows and the compiler refuses to parse. **Is the specification already
wrong, or already right?** If right, the compiler has the bug and the repair
costs zero. If wrong, which sentence is it and what does correcting it cost.

**Principle 0.** `selfhost/` has no generics and the closure list does not need
this. So anything that ENTERS must provably serve the thesis. Say what that
argument would have to be and whether the shared brief supplies it. Note that
the measured footing is *consistency, not safety* — no leak, stable
representation, one library function of six — which is the same ground panel 155
R3 used to make the twin WAIT.

Your verdict owes R1-R4, the deltas, a prediction with its instrument, your
condition, what you left UNRUN, and whether you cast your veto. Write to
`docs/panel/158-reports/spec-warden.md` first.

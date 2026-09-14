# Panel 149 — report of the llm-ergonomist

Input read: `docs/panel/149-briefs/llm-ergonomist.md` and
`spec/heroes-spec.md`. Nothing else. Method: the binding was written twice, once
under each candidate text, and every point where the seat guessed was recorded.

## Verdicts

1. **Does the specification as written already tell a reader that the
   `pair_make` program needs a mark? — REFUSE.** It does not. It tells the
   reader the opposite: the only sentence that could require the mark restricts
   itself to a *handle result*, and `Pair` is not a handle by the document's own
   definition.
2. **Candidate B — ADOPT WITH CONDITION**, four conditions below. Candidate A
   was identified as the current text, verbatim.
3. **The two-handle record — REFUSE WITH VETO.** Leaving it open creates a
   construct whose obligation cannot be determined from the line plus the
   signature it names. Refuse it at the declaration.

## Task 1 — what a reader of the document believes

**The program compiles and `pair_make` needs no mark.** The sentence used,
quoted:

> `acquires sqlite3_finalize` after a handle result or `@` out-parameter says
> the call begins that handle's life and names the one that ends it, which the
> program owes it.

and the definition that decides it:

> One with a `tag` and no fields is a **handle**, C's pointer to what the header
> leaves opaque.

`Pair` has fields, so `Pair` is not a handle, so `pair_make` has no handle
result, so the `acquires` sentence does not reach it. The sentence that pulls
the other way, *"where a group consumes a handle type every call handing one
back says which it is"*, was read the same way as its neighbour: the thing
handed back **is** the handle. And the singular *which it is* presumes exactly
one handle per call, which reinforces the narrow reading.

**On a compiler that rejected the program, the seat would have read the
diagnostic as a compiler bug against the document.** That is the ergonomic tell:
under the current text, a compiler doing the right thing reads to a
spec-reader as a compiler that is wrong.

**On being misled**, the seat relied on two sentences:

> a lease nobody ends, like a handle nobody consumes, aborts when `main`
> returns, saying how many.

> The owing is counted, so a handle consumed twice hides one never consumed.

Both say the compiler keeps a ledger of handles, from which the seat concluded
that any route by which a live handle reaches the program is one the compiler
knows about. A run-time failure means the ledger has an entry the reader was
never shown how to create.

## Task 2 — the blind A/B, and the finding inside it

**A is the current text; the seat identified it correctly after judging both.**

**The current text does not merely fail to require the mark — it argues a
careful reader out of writing it.** The seat considered writing
`-> Pair acquires slot_close` and **deleted it**, because no sentence licenses
`acquires` after a result that is not a handle, and § 13 is a section full of
narrow refusals (*"A parameter declared with it takes no other handle"*, *"two
records may not name one tag"*, *"a map key is an error"*). In a section that
refuses that often, an unlicensed mark reads as a refusal waiting to happen.

**Keep B.** Cost measured in words: **A is 28, B is 37 — nine words.** Nine
words buys the move from a silent leak at run time to a compile error at the
declaration, at the one moment the reader has the C header open.

## Task 3 — the two-handle record, and the veto

**The document cannot answer.** The production permits `-> Two acquires
slot_close`; the prose reaches it with *"says which it is"*, which is singular,
and this group consumes two handle types. It is not that the answer is hard, it
is that **the sentence has no slot for the second handle**.

Three readings the document eliminates none of:

- **(a) one close on the slot, nothing on the conn** — the connection leaks
  silently, forever;
- **(b) both are owed and the mark only managed to say one** — the program
  consumes a handle nobody acquired;
- **(c) `acquires` names the releaser of the RESULT** — nothing says the named
  function's `consumes` parameter must have the type of the handle reached, and
  the document's own `sqlite3_finalize` example cannot distinguish the rules
  because there the two coincide.

**The escape already ships, and this is the finding that makes the refusal
cheap.** § 13 already says:

> `record Font partial` names only some, and then comparing it and using it as a
> map key are compile errors — for it and for any value holding it.

A reader binding a real header that returns such a struct writes `record Two
partial`, listing only the fields they touch. That is an honest declaration —
*I am not handling those pointers* — and it is already legal. The program is not
lost; it is stated more precisely.

**The veto, stated against the seat's own standard.** At `t = two_make(n: 7)`
the obligation the line creates is two closes and the line says one. To discover
the second a reader must find `record Two`, notice `c: Conn`, find that `Conn`
has a tag and no fields, and then find `conn_close` elsewhere in the group:
**four declarations to know what one line owes.** The one-handle case is
different and acceptable, because the mark plus the signature it names
determines the obligation uniquely. So the rule to adopt is one rule and not
two: **the mark is required when the result reaches a handle, and the
declaration is refused when the mark cannot name it uniquely.**

## Hesitation points, each with what a wrong guess produces

| # | the guess | under the current text | under the proposal |
|---|---|---|---|
| 1 | `acquires` after a non-handle result | unlicensed, so omitted → **silently different program** | required → **compile error** |
| 2 | `slot_close(p.s)` while `p` still holds `s` | **silently wrong** under both; § 7 warns the address is recycled | unchanged, but the life becomes visible |
| 3 | `consumes` without `@` | three sentences in three sections to reach one call site; a wrong guess is a compile error | unchanged |
| 4 | must the named releaser's parameter type match the handle? | unstated → **silently wrong** on the two-handle shape | still unstated unless condition 3 lands |
| 5 | a fixed array field `slots: Slot[4]` | **silently wrong** either way; is the count one or four? | unstated unless condition 2 lands |
| 6 | depth: a record whose field is a record whose field is a handle | not covered | *"a record's fields"* reads ONE level unless condition 1 lands |

## Conditions on the adoption, and any one unmet drops the verdict to object

1. **Depth.** The text must say the rule holds at any depth, or a nested record
   silently escapes it.
2. **Fixed arrays.** Say whether `slots: Slot[4]` reaches handles, and how many
   the mark acquires.
3. **Type match.** Say that the releaser a mark names consumes a handle of the
   type reached. Without it, reading (c) stays alive and compiles.
4. **The refusal of the two-handle shape**, which is the veto. One mark per
   reached handle is also acceptable and costs more words. What is not
   acceptable is leaving it open.

## Prediction, falsifiable

Given the spec and a C header whose function returns a struct containing exactly
one opaque handle pointer, where the group also declares that handle's releaser
with `consumes`: **a model writing the binding omits the mark on the
record-returning function in at least 8 of 10 samples under the current text,
and in at most 1 of 10 under the proposal.**

Subsidiary, same run:

- **silent-leak rate on this shape** (compiles, no mark, handle never counted):
  predicted **> 0.7** under the current text, **< 0.1** under the proposal;
- **first-try COMPILE rate goes slightly down** under the proposal while
  first-try CORRECT goes up. A harness measuring only *did it compile* will show
  the proposal marginally worse; that would not falsify this, it would measure
  the wrong thing;
- **what a model gets wrong under the proposal**: it names a releaser whose
  `consumes` parameter is not the type of the handle reached. Predicted **> 0.2**
  of two-handle bindings, and **0.0** once condition 3 lands.

**How to test.** Three headers — one handle result as control, one
record-with-one-handle, one record-with-two-handles — times two spec variants, N
samples each. Score three booleans and not one: does it compile; is the mark
present where the handle is acquired; does every acquired handle have exactly one
discharging call in `main`. The control leg is what says the delta is about the
new sentence rather than about the prompt getting longer.

## What would change this verdict

If the harness runs that experiment and the current text already produces the
mark in a majority of samples, the Task 1 answer was idiosyncratic and the nine
words are not worth it: the seat withdraws to *object on cost* and asks only for
conditions 3 and 4. If the proposal's leg shows first-try-correct no better
while first-try-compile is worse, it is buying nothing and the seat withdraws it
entirely. **Nothing about the compiler's actual behaviour changes this verdict**,
because the verdict is about what the document makes a reader believe.

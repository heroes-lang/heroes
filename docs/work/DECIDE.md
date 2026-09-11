# DECIDE — the decisions the compiler is waiting on

Read by **`/decide`**. Every item asks **what should be true**, and until it is
answered the compiler goes on behaving some way by default — so the item names
that default, because it is the cost of leaving the item open.

**Only open items live here.** The moment one is answered it is ticked with the
verdict written into it and moved to `docs/work/DONE.md`, the record. Rank by
what an item blocks, never by age, and verify it against the repository before
putting it to the author: asking a settled question is the one cost this list
cannot pay.

**The shape.** One line per item, and an optional body indented four spaces
under it. The first field is the item's **origin**, and where that origin is a
sitting it is spelled `panel NNN`, padded — because
`tests/harness/suite_records.hero`'s `queued` check reads the `- [ ] ` lines
alone and scans them for exactly that, so a citation that slides into the body
makes every pending sitting report as unqueued, and it fails silently. Nothing
lives outside the two banners: `records/lists` is the executor of that.

Format: `- [ ] **<origin>** | <the question, in one line> | <where to look>`

*******************************************************************************
**OPEN: 2**

- [ ] **panel 132** | Two seats invented the same repair independently and two others invented a different one. The cheap shape is adopted and the complete one is scheduled with what it owes | `docs/panel/132-two-seats-invented-the-same-thing-and-two-others-invented-the-other.md`

    **Origin:** panel 132, 2026-09-11, the corrected sitting panel 131 called for,
    full five seats.

    **The finding first, because it is what the sitting is.** The brief gave the
    walls and four candidate shapes with their objections already written, and
    said that inventing a fifth was the most valuable thing a seat could do.
    **Two seats invented the same fifth, independently**: the one that had only
    the specification and no repository wrote `Room::width`, the one that had the
    whole compiler wrote `Point.width`, and they mean the same thing — a field's
    name becomes a name the checker resolves rather than a string the program
    types. **Two other seats reached a different fifth the same way**: the seat at
    the C boundary found that a record of handlers cannot work because a record
    cannot be generic, moved the handlers into parameters and compiled it; the
    historian, reading Ada's thirty-year-old answer, wrote that the precedent
    transfers *"only if the per-scalar writers are passed as ordinary function
    arguments"*. Neither pair was a compromise and neither was in the brief.

    **What your yes covers.** Three shapes refused, including the one the brief
    thought most likely, on **four vetoes with four different compiled grounds**.
    **Shape E adopted** — built, run, and the only thing in this sitting that is
    measured green: **+59 code lines**, **zero** in the emitter, the runtime, the
    lexer, the parser, ownership and mono; 631 compiler tests and 109 goldens
    pass; `heroes check selfhost/main.hero` goes 14.41 s to 13.90 s, so no
    slowdown; and it is **sugar rather than core** by design.md §1.7's own test,
    because it is erased to an interned literal on the way into the IR.
    **Shape B-prime scheduled and NOT adopted**, because what was compiled is a
    hand-written simulation of what the compiler would generate rather than the
    generation — **the distinction that cost panel 117 four days ago**. And a
    **mutation operator lands with E**, converged on by two seats separately:
    `heroes mutate` has 14 operators and none typos a string literal, so the net
    is blind to the exact mistake this milestone exists to kill.

    **Recommended: ratify.** The reason in one line: E is the only shape measured
    here that **cannot be wrong about the record it names**, and it takes nothing
    away from the program, which was the clause the corrected question said was
    not optional.

    **The honest half, because it decides whether you agree.** E kills the typo
    and **does not kill the rot**: add a field and the marshaller still compiles
    and silently never mentions it. Only a walk that enumerates fields closes
    that, which is B-prime, which is scheduled.

    **What the conservative resolution would have been**, so you can take it: land
    nothing. The document's guardian voted for exactly that and its case is the
    strongest thing in the file — of 56 hand-written renderers **1** is a
    foreign-notation field walk and it saves **0 lines**, and the repair already
    exists at **zero spec tokens** in an idiom documented at the top of the very
    file the experiment imitated, `constant K_WIDTH: str` turning `K_WITDH` into
    `error[unknown_name] ... did you mean K_WIDTH?`. It was not adopted for one
    measured reason: **a constant is not bound to the field**, so a constant whose
    body reads `"widht"` compiles and ships, which moves the mistake one line
    sideways rather than removing it.

- [ ] **panel 132** | Panel 117's clause *until this language renders one* now names a condition nothing is working toward. Remove it, which reverses a wording you chose against a seat's recommendation on 2026-09-07 | `spec/heroes-spec.md` § 12 · `docs/panel/117-both-sides-of-an-assert-when-a-side-is-an-aggregate.md`

    **Origin:** panel 132, 2026-09-11. It is a separate item because it reverses
    your own choice, the way the panel 117 withdrawal was, and a reversal must not
    ride inside a longer yes.

    **What changed.** On 2026-09-07 you took the fuller wording over the
    document guardian's cheaper one, on *better robust than cheap*, and the fuller
    one carries a restoration condition: the aggregate case shows the expression
    alone **until this language renders one**. That was honest while something was
    working toward it. Panel 131 was asked to declare a branch and said *still
    being worked toward*; that is no longer true. A derived render of Heroes' own
    construction call was refused at panel 131 on a 0-of-56 measurement, nothing
    in this sitting revives it, and **neither shape adopted nor scheduled here is
    a renderer `assert` could use** — both require a program to say what to do
    with each field, and `assert` has no program to ask.

    **Recommended: remove the clause**, leaving the narrowing as a plain
    statement. Measured **−7 vendored, ≈ −9 real**, so it is a removal that helps
    pay for what lands. The reason is this project's own rule rather than a
    preference: a promise with no date is the shape the records exist to end, and
    a condition nothing is working toward is that shape wearing a date's clothes.

    **What keeping it costs, so the choice is real**: nothing today, and one
    reader at a time from here on who takes *until* as a plan.


*******************************************************************************

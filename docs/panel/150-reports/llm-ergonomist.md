# Panel 150 — report of the llm-ergonomist

Input read: `docs/panel/150-briefs/llm-ergonomist.md` and `spec/heroes-spec.md`.
Nothing else.

| | verdict |
|---|---|
| **Q1 — a rule keyed on `ptr`** | **approve**, on three conditions |
| **Q2 — `acquires` on a record containing an array of handles** | **VETO** |

## Q1 — the untyped pointer

**The program compiles, the seat hesitated, and the hesitation is the finding.**
`ptr` is not a handle, because a handle is *"One with a `tag` and no fields"*, so
the group does not consume a handle type and `opaque_open` owes no mark.

**The trap, quoted.** § 3 calls `ptr` *"an opaque pointer"*. § 13 calls a handle
*"C's pointer to what the header leaves opaque"*. **Those are the same phrase
with the words reordered.** The seat spent real effort establishing that one is a
defined term and the other a table gloss, and says a reader who does not do that
work concludes the opposite.

**The fact the seat wants on the record.** *"`ptr` is the only pointer-shaped
thing in the language with no diagnostic at either end."* A handle nobody
consumes aborts at `main`'s return saying how many; a lease nobody ends does the
same; **a `ptr` nobody frees does nothing at all.** And `ptr` is the type a model
reaches for first, because it is one type for every C pointer and needs no
`record` declaration.

**The most plausible wrong program in the task, and it compiles.** `_ = p` is
sanctioned by § 5 as discarding *"on purpose"*. A reader who has half-absorbed
the ownership paragraph reads it as *and here I release it*. **It releases
nothing.**

**Would the rule bury a reader? No — it SHRINKS the model.** The seat wrote both
bindings of a four-function pool library and chose the marked one without
hesitation:

> Under the current spec those four lines are four silent questions and I must
> answer each of them out of the C header, at every call site, forever, with
> nothing in the Heroes file recording my answer. Under the rule I answer each
> once, at the declaration, in one word.

> Today I must hold two regimes for pointer-shaped things … Under the rule there
> is one sentence: a pointer whose life the group manages says so. That is less
> to remember, not more.

**The cost it refuses to wave away.** A demanded mark forces a guess, and the two
wrong guesses are **not symmetric**: `borrows` where `acquires` is true
reproduces today's leak, while `acquires` where `borrows` is true makes the
language REQUIRE a release of a pointer the library kept, and the program then
double-frees. Worse, **the demand itself biases toward `acquires`**, because the
mark is demanded precisely when a consuming function is visible in the group. It
approves anyway, because that double-free is not a class the rule creates.

**Two holes it names rather than fixes.** A free function in a DIFFERENT `extern`
group fires no demand, so the leak is silent again. And a trigger keyed on a
universal type means one line's legality depends on another line's existence. It
considered vetoing on the second and declined, by its own standard: **the
non-locality is in legality, not in meaning**, and a later consuming member turns
unmarked siblings into a loud compile error naming the group.

## Q2 — the array of handles, and the veto

**The grammar derives a form the prose never defines.** The Member production
puts no constraint on `Type`, and the only sentence defining what `acquires`
means says *"after a handle result or `@` out-parameter"*. `Four` is not a
handle.

**One release or four? The document cannot decide it**, and the seat says so
rather than pretending. It states it would have guessed **four**, with low
confidence and nothing to quote.

**The sentence it found is the sharpest thing in the report**:

> "The owing is counted, so a handle consumed twice hides one never consumed."

**That is the document admitting its count is a number and not an identity.**

*Coordinator's measurement, 2026-09-15, run on receipt.* The seat built a
scenario on that admission: releasing `f.a[0]` four times would balance four
against four, triple-freeing one slot and leaking three, with nothing firing.
**The specific scenario is REFUTED and the uncertainty under it is CONFIRMED.**
Run against the compiler at the current tip, that program prints and then
**aborts at `-3`**, because the mark produces ONE increment and not four. So the
counter does fire, and its message — *"the same handle given back TWICE, which is
a double release"* — is correct for this program. **But the seat could not know
that from the document, and said it would have guessed four**, which is the
veto's actual ground and is untouched.

**Two further undecidables in the same shape**, both hit while trying to settle
the first: whether consuming a COPY reached through a Place discharges anything,
given § 3's *"Every value behaves as an independent copy"*; and what `g = f` does
to the count.

**And the worst reading, which the counting rule pushes toward.** If the owing
were four and C filled only two slots, satisfying the counter means calling
`slot_close(nullptr)`, which type-checks. *"The document would have me close a
null handle to balance a counter."*

**The veto, stated precisely.** Not on `Slot[4]` as a field, which § 13 permits
and the seat does not object to as data. On **`acquires` after a result that is
not a handle, while the distribution rule is unwritten**, because the meaning of
`slot_close(f.a[0])` cannot be determined from that line plus its signature:
three hops, two of them unwritten.

**Refusal beats an open question here**, and the seat priced both sides: a
refusal may leave a header unbindable, which it takes anyway, because *"a refusal
is a program I cannot write, which I can see and report. An open question is a
program I did write, which compiles, and which is wrong in a way no amount of
careful reading of this document can detect."* And a refusal is reversible where
a shipped ambiguity is not.

## Conditions

**Q1 approve turns to object unless the diff answers three things:**

1. **`_` must not drop an owed `ptr`**, on the model of *"A `_` never drops a
   `T?`"*. The cheapest and most important clause, because `_ = p` is exactly
   what a model writes to satisfy the unused-binding error.
2. **Say what happens when the consuming function is in another `extern`
   group** — either the demand reaches across, or the text says it does not.
3. **Say whether a `ptr` from a `borrows` call may be passed to a `consumes`
   parameter.** If it is refused as it is for handles, the
   `acquires`-where-`borrows`-is-true double-free is caught at the call site and
   the seat's last objection dissolves. If not, it downgrades to object.

**Q2 veto lifts on either**: refuse the form with a diagnostic naming the
alternative, or write the distribution rule — how many a mark over `Slot[4]`
owes, what `g = f` does to the count, whether consuming through a Place
discharges, whether a null handle owes a release. **Four sentences, or one
refusal. Not a derivation left to the reader.**

## Prediction

Given the spec and Q2's `extern` group, asked *how many times must `main` call
`slot_close`, and quote the sentence that decides it*: **no single answer reaches
80%**, **at least 30% answer one**, and **no sample quotes a sentence that
actually decides it**. Convergence without a decisive quote is shared guessing,
not falsification, and should be recorded as such.

Companion, testing Q1: given a four-function pool group under the current spec,
**8 of 10 or more produce zero `acquires`/`borrows` tokens**. One grep settles
it; falsified if more than 2 in 10 carry a mark today.

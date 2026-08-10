# 009 — `T?` and `{K: V}`: the last two types

M5d. Tag `m5d`. Built on the author's instruction, against a deferral this project
had already recorded.

## Goal

Finish the type table. After M5c the backend emitted every type except two, and
the two were entangled: `m[k]` returns `V?`, so the map could not land before
`T?` had a representation.

**Delivered:** `T?` as a tagged union by value with `HeroFailure` shipped in the
runtime; one generated option struct per distinct `T?`; the map as open addressing
with a fixed seed, order-independent equality, and read-only access. `HERO_RUNTIME_ABI`
went 3 → 4 → 5 in one session, which the author licensed explicitly ("everything is
under construction").

**The disagreement, recorded because it is not resolved.** M5c closed with these
two deferred on Principle 0: `{K: V}` may be **deleted from the language** at M6's
closure audit, at −57 spec tokens against +29 to fund `set` plus `for k in m`, and
building a container that may not survive is what `cow_check` was struck for at
M5b. The author overruled that and asked for all of M5. The work is done and the
risk is unchanged: if the audit deletes `{K: V}`, this milestone's map is deleted
with it. What is *not* at risk is `T?`, which no audit questions.

## What surprised

**A question asked twice is a bug waiting.** The one repair worth generalising in
this milestone was not a fix but a merge: retaining a `T?`'s payload and retaining
a record's field are the same question — *what does this type need to keep a
reference alive* — and they had two implementations. The second one reached for
the descriptor's `copy`, which takes `(dst, src)`, and produced first a wrong
argument count and then a discarded `const`. Neither was the real defect. The real
defect was that the question had two answers, and `ir/layout.rs` had already
written down what that costs.

**A leak can be invisible for the most ordinary reason.** `Op::MapGet` was declared
non-allocating while its emission copied the found value in. The program that
exposed it needed a **heap-allocated** value: with a `str` literal the same missing
decref changes nothing, because a literal's refcount is negative and decrefing one
is a no-op. So the test that finds this class has to allocate on purpose —
`"one" + "!"` rather than `"one"` — and every golden in this milestone that touches
counting does.

**`hash` waited two milestones for its first caller.** Panel 022 required a non-null
`hash` for every type at M5c, justified by a map that did not exist yet, and the
rule's first act was to reject the spike that had frozen the descriptor ABI. The
map is where it finally gets used. A rule adopted before its use is usually a
smell; this one was right, and the reason is that the *cost of being wrong* was
`SEGV at pc 0x0` with no type name — a failure whose debugging cost does not shrink
by waiting.

**The gate ran out of types.** It refuses no type at all now. The arms stayed —
`Op::Construct` and the three access operations hold empty bodies rather than being
deleted — because the reason for enumerating was never "these are refused", it was
"adding a shape to the IR must be a compile error here". An empty arm keeps that
and a deleted one does not.

## What broke and why

| symptom | cause | fix |
|---|---|---|
| `error: static assertion failed due to requirement '4 == 3'` on the first run after the header bump | the ABI stamp is written by the emitter and asserted by the generated C — exactly the mismatch a decoy `runtime/` was measured causing without it | align the emitter; the check is the feature |
| `too few arguments to function call, expected 2, have 1` | a `T?`'s retain went through the descriptor's `copy(dst, src)`, which has no retain-in-place form | one shared `reference_line` for a field and a payload alike |
| `-Wincompatible-pointer-types-discards-qualifiers` on the same line once fixed | a retain's parameter is `const`, and a self-copy writes through it | the same merge; no cast |
| `{1: "one" + "!"}` then `.default(…)` leaked exactly one block, and only with a computed value | `Op::MapGet` was declared non-allocating while its emission copies the found value into the option, which increfs | `allocates(MapGet) = true`, the decision the arm's own comment asked for |

## What is left in the language

Not a type — a built-in, and an operator. `sort`, `join`, `chars` and `range` have
no runtime entry point, and `.must()` is an `Op::Abort` rather than a shape. All
five are M6's, and they are what four gallery programs are waiting for.

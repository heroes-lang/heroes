# Panel 162 — spec-warden

Read `00-shared.md` first, then `spec/heroes-spec.md` § 3, § 10, § 11 and § 13,
and `.claude/rules/spec-shape.md`. This file is your input only. You have a veto
on budget breach and you carry Principle 0's burden of proof.

## The budget, measured this session

`./heroes measure spec/heroes-spec.md`, run 2026-09-18:

```
  cl100k_base        5997
  real               7984   claude-opus-5 — the binding number
```

Ceiling **10240**. Headroom **2256**, of which the FFI floor mortgages 60.

**A vendored delta is not a price** (author instruction 2026-09-16: *always
measure with the real*). `--refresh` refuses every path but
`spec/heroes-spec.md` and `CLAUDE.md`, so a draft is priced one of two ways and
there is no third: apply it to the real path in a **scratchpad copy**,
`--refresh`, read the number; or write it down as a **lower bound, in those
words**. `ANTHROPIC_API_KEY` is in the repository's `.env` — `. ./.env` first;
without it `--refresh` exits 2 and the number is *unrun*, never a licence to
promote the vendored figure.

**The working tree is frozen.** Price drafts in a copy, never in the repository.

## What you are asked

**Principle 0's compiler-need branch is already closed, measured**: zero programs
in `examples/` declare a fixed byte array, and the compiler self-hosts without
this. So **every route here enters on the thesis branch or not at all**, and your
seat is where that burden is discharged or refused.

Three questions decide your verdict.

1. **Where does the rule live?** `.claude/rules/spec-shape.md` says every rule has
   exactly one home, the section of the operation it governs. A conversion from
   bytes to text: is that § 11 (built-ins and conversions), § 10 (strings,
   arrays, maps), or § 13 (the boundary the bytes arrive through)? The answer
   changes the price, because § 11's `Built-ins:` sentence is read by an
   instrument that strips parentheses and ends at `range`.

2. **What does the `Built-ins:` sentence cost per name?** Measure it: add a name
   to a copy, `--refresh`, read the delta. That number prices route 1 exactly and
   prices nothing else, which is why it is worth having before you argue.

3. **Is there a named removal?** An addition owes one, or a registered
   falsifiable prediction naming an instrument that exists. Grep § 11 and § 13
   for text any route makes redundant — in particular, if a route makes
   `c.validated()`'s sentence or the *no record holds a `cstr`* sentence
   narrower or wider, that is text moving rather than text added, and merging
   beats appending (panel 122).

## The thing to resist, and the thing to insist on

**Resist**: the cheapest route in tokens is refusing, which costs a Part 6 row
and no § 13 text at all. CLAUDE.md § Precedence puts robustness above token
cost. Say what refusing costs in tokens **and** say plainly whether the budget
is what should decide it.

**Insist**: this is a live refusal candidate, unlike most sittings. Zero corpus
programs want it, the compiler does not need it, and design.md Part 6 exists for
exactly this shape. If your reading of Principle 0 is that the proposal has not
earned its way in, **say so as your verdict** rather than pricing it politely.
A refusal here is held to Part 6's standard: it must name the program or
compiler fact that would make it wrong.

## Deliver

Verdict · the section it rests on · the real token count of each draft you
priced, or the words *lower bound* where you could not · a falsifiable
prediction with its milestone · any budget veto.

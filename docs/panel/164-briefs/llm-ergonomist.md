# Panel 164 — llm-ergonomist

**Your input is `spec/heroes-spec.md` and this file. Nothing else.** Do not read
design.md, the repository, the compiler, the other briefs or `00-shared.md`.

## The situation, without the machinery

Read **§ 3**, **§ 9**, **§ 10** and **§ 13**.

A header record has a field `name: i8[16]`, bound from C's `char name[16]`. The
group also declares `function slot_len(s: cstr) -> u64`, bound from C's
`strlen`. In C, `strlen(s.name)` is the idiom every program writes: the array
becomes a pointer at the call.

In Heroes, `slot_len(s.name)` is a compile error — `expected cstr, found
i8[16]` — and so is `slot_len(s.name.cstr())`, because § 13 says `s.cstr()`
lends a `str`, and this is not a `str`.

## Three tasks, in order, answers written before any opinion

**Task A — write it.** Given the group above, write `main` so it calls
`slot_len` on the field. Work from the specification only. Record every route
you tried and the sentence that refused it, including the ones you abandoned in
a second.

**Task B — the sentence you looked for.** § 13 has a paragraph about `cstr`:
lending, leasing, `validated`, and since recently `validated_bytes`. Where in
it did you expect to find how a FIELD reaches C, and what did you expect it to
say? A form a reader reaches for and does not find is worth more than one
somebody designs.

**Task C — blind A/B.** Three spellings for handing the field to `slot_len`. You
are not told whether any is proposed:

```
    (A)   slot_len(s.name)
    (B)   slot_len(s.name.cstr())
    (C)   slot_len(s.name.ptr())
```

For each: what does the reader believe C receives, and what does the
specification let them believe about where those bytes END? A `str` always has
a terminating zero; a `char[16]` field holds whatever C put there. Which spelling
leaves the reader least likely to write a program that reads past the field on
a machine they did not test on? Answer all three before comparing.

## What is being decided

Four routes, judged as a READER: (1) the field is accepted wherever a `cstr` or
`ptr` parameter is declared, silently, as C does; (2) an explicit lend
`f.cstr()`; (3) an explicit lend to `ptr` only, refusing `cstr` because a
`cstr` promises a terminator the field does not; (4) refusing altogether, so a
field is read with `validated_bytes` and a C function that wants its bytes is
given them by the header.

For each: does it make a program more or less likely to be read correctly by
someone who did not write it? **§ 13's sentence about `s.cstr()` says what a
lend IS; say what a reader carries from it into a field lend, and whether it is
still true there.**

## You have a veto on non-local constructs

Use it if a route makes a line's meaning depend on something not visible near
it. Name the line and what a reader would have to go and find.

## Deliver

Verdict · the section it rests on · A, B and C in full including the wrong
answers · a falsifiable prediction · any veto.

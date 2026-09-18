# Panel 163 — llm-ergonomist

**Your input is `spec/heroes-spec.md` and this file. Nothing else.** Do not read
design.md, the repository, the other briefs or `00-shared.md`.

## The situation, without the machinery

Read **§ 5**, **§ 9**, **§ 10** and **§ 13**.

A C library fills a struct the caller owns. The header is:

```c
struct utsname { char sysname[256]; char nodename[256]; char release[256];
                 char version[256]; char machine[256]; };
int uname(struct utsname *name);
```

In Heroes that binds as a `record` in an `extern` group with five `i8[256]`
fields and `function uname(@name: Utsname) -> i32`.

To call it you need a `Utsname` first, because § 5 says all bindings are
initialised. § 13 says a fixed array field is built with `[a, b, c, d]`, and a
fixed array's length is part of its type — so the literal must hold 256
elements, five times over.

## Three tasks, in order, answers written before any opinion

**Task A — write the line.** Write `main` so that it calls `uname` and prints
the machine name. Work from the specification only. **Write down every route you
attempted and the sentence that refused it**, including the ones you abandoned
in a second. If you end up writing 256 zeros, say so and say what you thought
while doing it.

**Task B — the sentence you looked for.** Where in the document did you first go
to find out how to make a value you are about to hand to C for filling, and what
did you expect to find? A name or form a reader reaches for and does not find is
worth more here than one somebody designs.

**Task C — blind A/B.** Three candidate spellings, and you are not told which if
any is proposed:

```
    (A)   u: Utsname @ Utsname(sysname: [0, 0, … 256 times], …)
    (B)   u: Utsname @ Utsname(sysname: repeat(0, 256), …)
    (C)   u: Utsname @ Utsname()
```

For each: what do you believe the fields hold afterwards, and what in the
specification tells you? Then: which leaves a reader least likely to be wrong
about what C receives? Answer all three before comparing.

## What is being decided

Four routes: a call whose type comes from context, as in (B); a zero default, as
in (C); an out-parameter that needs no initial value at all, so the `u:` line
disappears; or refusing, so a struct like this is only ever obtained from a
function the library declares and never built.

For each, judge as a READER: does it make a program more or less likely to be
read correctly by someone who did not write it? **§ 5's *all bindings are
initialised* is the rule three of the four bend — say what that rule buys a
reader today, because that is what is being spent.**

## You have a veto on non-local constructs

Use it if a route makes a line's meaning depend on something not visible near
it. Name the line and what a reader must go and find.

## Deliver

Verdict · the section your reasoning rests on · A, B and C in full including the
wrong answers · a falsifiable prediction · any veto.

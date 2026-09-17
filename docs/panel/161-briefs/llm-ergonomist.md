# Panel 161 — llm-ergonomist

**Your input is `spec/heroes-spec.md` and this file. Nothing else.** Do not read
`design.md`, the repository, the other briefs or `00-shared.md`. Your verdict is
an experiment about what a reader of the specification does, and it is worth
having only because you have not seen the machinery.

## The situation, stated without the machinery

Heroes binds C libraries. A C library's header describes a struct, and a Heroes
program restates that struct inside an `extern` group so the compiler can check
it against the real header. Read **§ 13** of the specification: it says a field
and a parameter are declared *at the header's own width and sign*.

C has a type spelled `char`. **Its sign is chosen by the machine**: on some
machines `char` holds −128 … 127, on others 0 … 255. A header that says
`char name[65]` therefore describes a signed array on one machine and an
unsigned array on another, with no change to the header.

Heroes offers `i8` (signed, 8 bits) and `u8` (unsigned, 8 bits) and no third
8-bit type. So today a program writing `name: i8[65]` compiles on one machine
and is refused on the other, and `u8[65]` does the reverse. **One source file
cannot serve both.**

## Three tasks. Do them before you form an opinion

**Task A — read the specification and answer from it alone.** A header declares:

```c
struct utsname {
    char sysname[65];
    char nodename[65];
};
```

Write the `extern` group that binds it, as § 13 tells you to. Then answer: from
the specification's text alone, **did you know which of `i8` and `u8` to write?**
Quote the sentence you used. If you had to guess, say what you guessed and why.

**Task B — blind A/B.** Two candidate spellings for the same field. You are not
told which is the status quo:

```
    (A)    sysname: i8[65]

    (B)    sysname: cchar[65]
```

where in (B) `cchar` is a type usable **only inside an `extern` group**, meaning
"this header's plain `char`, whatever sign this machine gives it".

For each: what do you expect `s.sysname[0]` to be — its type, and its value if
the byte holds 200? Which of the two leaves you less likely to write a program
that is wrong on a machine you did not test on? Answer both parts for both
variants before you compare them.

**Task C — the read.** Under (B), a program has to get a number out of that
array. Propose what `s.sysname[0]` should yield, choosing from what § 3's type
table already offers, and say what a reader would assume it yields if the
specification said nothing. If your answer and your assumption differ, that
difference is the finding.

## What is being decided

Four routes are on the table and you are asked to judge them **as a reader of
the specification**, not as an implementer:

1. a ninth integer type in the language, whose sign is the machine's;
2. a spelling legal only inside an `extern` group, as in variant (B);
3. accept **either** `i8` or `u8` against a plain `char`, everywhere — the
   program then compiles on every machine, and a byte holding 200 reads back as
   200 under one spelling and as −56 under the other;
4. refuse plain `char` altogether, so those fields cannot be bound at all.

For each: does it make a program **more** or **less** likely to be read
correctly by someone who did not write it? Route 3 in particular — say plainly
whether a reader would notice the trap, and how.

## You have a veto on non-local constructs

Use it if a route makes the meaning of a line depend on something not visible
near that line. Say which line and what a reader would have to go and find.

## Deliver

Verdict · the specification section your reasoning rests on · your answers to
A, B and C in full, including the wrong ones · a falsifiable prediction about
what a reader or a model would do · any veto.

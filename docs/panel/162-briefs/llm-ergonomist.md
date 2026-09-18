# Panel 162 — llm-ergonomist

**Your input is `spec/heroes-spec.md` and this file. Nothing else.** Do not read
`design.md`, the repository, the compiler, the other briefs or `00-shared.md`.
Your verdict is an experiment about what a reader of the specification does, and
it is worth having only because you have not seen the machinery.

## The situation, stated without the machinery

Read **§ 3**, **§ 10**, **§ 11** and **§ 13**.

A C library hands a program a run of bytes: a struct field declared
`char sysname[256]`, holding the machine's name. In Heroes that field is bound as
`sysname: i8[256]`. The program wants to print the name.

**From the specification's text, there is no way to do it.** `§ 10` gives
`s[i] -> u8` and `s.chars()`, both outbound; `§ 11`'s conversions are `to_str`,
`to_f32`, `to_f64` and the eight `to_<int>`, and `to_str` takes a number, a `str`
or a `bool`; `§ 13` gives `c.validated()`, which answers a `cstr`, and says
outside a group nothing answers `cstr` and no record holds one.

## Three tasks. Do them in order, and write your answers before forming an opinion

**Task A — try to write it, and record where you stopped.** Given

```
extern "sys/utsname.h"
    record Utsname tag utsname
        sysname: i8[256]
    function uname(@name: Utsname) -> i32
```

write `main` so it prints the machine's name. Work only from the specification.
**Write down every route you attempted and the sentence that refused it**,
including the ones you abandoned quickly — those are the finding, not the
failure.

**Task B — the sentence you expected to find.** Before reading further: where in
the document did you first go looking, and what did you expect it to say? If you
expected a name that is not there, write the name you expected. A name a reader
reaches for and does not find is worth more to this sitting than a name somebody
designs.

**Task C — blind A/B on what it should answer.** Two candidate spellings, and you
are not told whether either is proposed:

```
    (A)    name = u.sysname.to_str()          # a str, always
    (B)    name = u.sysname.to_str()?         # a str?, the error propagates
```

The bytes may not be valid UTF-8, and `§ 3` says `str` is immutable UTF-8. For
each: what do you expect when the bytes are not valid text, and which reading
does `§ 11`'s own sentence about conversions push you toward? Quote that
sentence. Then say which of the two a reader is more likely to get RIGHT without
testing it.

**Task D — the second wall.** `§ 13` says a fixed array field is built with
`[a, b, c, d]`, and `§ 5` says all bindings are initialised. To call
`uname(@u)` you must first have a `u`. Write that line. If you cannot, say what
you tried and what you would expect the language to offer.

## What is being decided

Four routes, judged **as a reader**, not as an implementer: a built-in that
answers a byte run; a `cstr` view of the field so the existing `validated()` is
the only door; something slice-shaped so the caller says where the text ends; or
refusing outright, so a byte field is read one element at a time forever.

For each: does it make a program **more** or **less** likely to be read correctly
by someone who did not write it?

## You have a veto on non-local constructs

Use it if a route makes the meaning of a line depend on something not visible
near that line. Name the line and say what a reader would have to go and find.

## Deliver

Verdict · the specification section your reasoning rests on · your answers to A,
B, C and D in full, the wrong ones included · a falsifiable prediction about what
a reader or a model would do · any veto.

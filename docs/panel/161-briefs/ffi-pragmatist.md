# Panel 161 — ffi-pragmatist

Read `00-shared.md` in this directory first. This file is your input only. You
have a veto on ABI breakage.

## What you are asked

**Write and compile the C that each route implies**, and report what the
compiler says rather than what the route claims. You are the seat that finds out
whether a proposal survives contact with a real header.

## The founding constraint this sits under

design.md §1.11: there is no standard library and everything comes from C. A
header Heroes cannot bind is a library the author must leave C code around for.
So the question *how many real headers does this actually block* is yours, and
the answer in `00-shared.md` (23 plain-`char` array fields across nine Debian
headers, 16 across eight Darwin ones, 0 scalars, 1 parameter) is a floor you are
invited to widen — raylib, SDL3, zlib and the POSIX set beyond those nine are
unwalked.

## The four routes, as C questions

1. **A ninth integer type whose sign is the target's.** What C type does it emit
   as? If it emits `char`, then every `_Static_assert` the compiler writes about
   it is trivially true and the assertion stops catching anything — check that,
   because it decides whether route 1 is sound or decorative.

2. **A spelling legal only inside `extern`.** Same question, plus: what happens
   when such a field is passed BY VALUE to a C function, and when a struct
   holding one crosses the boundary? Compile it.

3. **Accept `i8` or `u8` against plain `char`.** Write a C function that fills a
   `char[4]` with bytes above 127, bind it both ways, and **run it**. Report the
   values a Heroes program reads under each spelling on a signed-`char` and an
   unsigned-`char` machine. That is the soundness cost in numbers rather than in
   argument.

4. **Refuse.** Name three real headers that become unbindable, by walking them
   rather than by recalling them, and say what the author's workaround would be.

## The two machines

`heroes-linux-arm64` and `heroes-linux` are the Docker images; the run line is
in `docs/ref/environment/linux/LINUX-MACHINE.md`. **A sign fact needs both** —
one is not a measurement of this question. The Mac is a third and is
signed-`char`.

## The specific thing to attack, and the shapes beside it

`tests/golden/run/ffi-a-char-array-member.hero` binds `char name[4]` as
`i8[4]`. Run it on both Linux images and confirm the inversion for yourself
rather than taking `00-shared.md`'s word.

Then attack the shapes beside it (CL-061), because a class is not a class until
its exceptions have been looked for: a scalar `char` field, a `char` field
inside a nested record, a `char` field in a `partial` record, a `char` as a
function parameter, a `char` as a result, a `char **` out-parameter, and
`const char` in each position. Say which of those invert and which do not. If
one of them does **not** invert, that is a finding and it may be a route.

## One thing to weigh before you recommend portability

`struct utsname`'s arrays are `char[65]` on Debian and `char[256]` on Darwin —
measured, both walks in `00-shared.md`. That struct is **already** non-portable
in Heroes because the LENGTH differs, sign or no sign. So a route argued as
"this makes headers portable" owes a list of which headers it actually makes
portable. Walk a few and say.

## How to work

`cp -r` the tree to your scratchpad, `rm -rf build`, work there. Seed:
`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, a few seconds.
**Do not rebuild from `selfhost/`** (~20 minutes, kills seats on the watchdog).
**Never `archive/bootstrap-rs/`.**

## Deliver

Verdict · the section it rests on · the C you wrote and what the compiler said,
verbatim · a falsifiable prediction with its milestone · any ABI veto, stated as
a refusal and not a price.

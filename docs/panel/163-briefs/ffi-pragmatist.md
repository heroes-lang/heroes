# Panel 163 — ffi-pragmatist

Read `00-shared.md` first. This file is your input only. You have a veto on ABI
breakage.

## What you are asked

**Write the C, bind it, run it.** The question is how a program obtains a struct
that C is about to fill, and you are the seat that can find out whether a route
that compiles also behaves.

## The four questions only you can answer

1. **Is there already a way, and has nobody tried it?** A group may declare a
   function that RETURNS a header record. Write a header with
   `static inline struct utsname hero_blank(void)`, bind it, and see whether
   `u: Utsname @ hero_blank()` then `uname(@u)` compiles and runs. **If it does,
   this sitting may cost nothing**, and the resolution is a sentence in § 13
   rather than a language change. Report what the compiler said, verbatim.

2. **What do real out-parameter APIs actually require?** Walk a set of headers
   and classify the structs a C function FILLS: does the caller have to
   zero it first, does the function fill every byte, or does it read a field the
   caller set (`ifreq.ifr_name`, `addrinfo` hints, `sigaction`)? **That last
   class is the one that decides route 3**: a struct the callee READS before
   writing cannot be left uninitialised, and if that class is large then
   "uninitialised out-parameter" is unsound for real APIs rather than only for
   the language's rule.

3. **Does an uninitialised or zeroed struct behave?** For each route that
   produces a `Utsname`, build it and RUN `uname` under `--sanitize`, and report
   what comes back. A route that compiles and reads uninitialised memory is the
   most important finding you can return and it outranks everything else.

4. **The 803-character literal**: build it, run it, and say whether the emitted C
   is what you would expect — panel 162 measured that it works, but nobody has
   looked at what 256 designated initialisers per field do to the generated file
   or to clang's time.

## The shapes beside it (CL-061)

A short fixed array (which works today), a struct with one long array and one
scalar, a struct of only long arrays, a nested record holding one, and the same
through `@` versus by value. Say which any proposed route covers.

## How to work

`cp -r` to your scratchpad, `rm -rf build`, seed with
`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`. **Never rebuild
from `selfhost/`.** **Never `archive/bootstrap-rs/`.** Read the real header
before binding it — panel 162's run of this seat reported the compiler catching
its own misreading of `dirent`.

## Deliver

Verdict · the section it rests on · the C you wrote and what the compiler and the
program said, verbatim · a falsifiable prediction · any ABI veto as a refusal.

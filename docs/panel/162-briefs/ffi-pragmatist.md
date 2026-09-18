# Panel 162 — ffi-pragmatist

Read `00-shared.md` first. This file is your input only. You have a veto on ABI
breakage.

## What you are asked

**Write the C, bind it, run it.** You are the seat that finds out whether a route
survives a real header, and this question is entirely about what real headers
do with byte arrays.

## The founding constraint

design.md §1.11: there is no standard library and everything comes from C.
`.claude/rules/c-boundary.md`: the FFI must be **complete** — *a library Heroes
cannot bind is a library the author must leave C code around for*. A field the
language binds and cannot read is the sharpest form of that failure, because it
looks bound.

## The four questions only you can answer

1. **How do real headers actually use `char[N]`?** The shared brief counts 24 of
   321 fields across six headers. Walk a wider set and **classify them**, because
   the classes decide the route: NUL-terminated text (`utsname.sysname`),
   fixed-width text that may fill the field with no terminator, a byte blob that
   is not text at all, and a union-ish scratch area. Report counts per class with
   the command. **If most are not NUL-terminated text, a route that assumes
   termination is wrong and you are the only seat that can say so.**

2. **Is there an existing door nobody has tried?** `spec § 13` gives
   `s.cstr()`, `c.validated()` and `x: cstr @ s.lease()`. Try to reach a
   `char[N]` field's bytes through each, on a real header, and report what the
   compiler says verbatim. One of them may already work and nobody checked.

3. **The build half, and it is yours because it is an FFI shape.** `uname(@u)`
   needs a `u`. Write, compile and run every way you can think of to obtain one:
   a full literal, a `partial` record, a handle, a helper the group declares that
   returns one, an `@` out-parameter. **Report which compile and which run**, and
   if a route compiles and corrupts, that is the most important finding in this
   sitting.

4. **What would each route emit?** A `str` built from a byte field is a copy into
   the runtime's own representation, or it is a view into C's memory. If it is a
   view, say what happens when C frees the struct, and whether `spec § 13`'s
   lease machinery already answers it.

## The shapes beside it (CL-061)

`i8[N]` and `u8[N]`; a `char[N]` inside a nested record; a `char[N]` that is the
last field; `const char[N]` (panel 161 measured that this is refused at every
spelling and that it is the `const`, not the `char`); a zero-length or flexible
array; and a `char[1]` used as a tag byte. Say which of these any proposed route
covers and which it does not.

## How to work

`cp -r` the tree to your scratchpad, `rm -rf build`, work there. Seed:
`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, seconds. **Do not
rebuild from `selfhost/`.** **Never `archive/bootstrap-rs/`.** The Docker images
`heroes-linux` and `heroes-linux-arm64` are there if a platform fact is needed;
the run line is in `docs/ref/environment/linux/LINUX-MACHINE.md`.

## Deliver

Verdict · the section it rests on · the C you wrote and what the compiler and the
program said, verbatim · a falsifiable prediction with its milestone · any ABI
veto, stated as a refusal and not a price.

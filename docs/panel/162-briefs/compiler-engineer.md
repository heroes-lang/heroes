# Panel 162 — compiler-engineer

Read `00-shared.md` first. This file is your input only. You have a veto on
soundness.

## What you are asked

**Find where the answer already is.** The shared brief names it: `read_file`
returns a `str?` from arbitrary file bytes, so this compiler already decides what
happens when bytes are not valid UTF-8, somewhere. Find that code, name it, and
say whether the inbound door this sitting wants is the same door with a different
caller or a genuinely new one. **That single finding is worth more than pricing
all four routes.**

## Where to look

`selfhost/inventory.hero` is the built-in table. `selfhost/check/builtins.hero`
is what refuses `to_str` of a `[u8]` today. `runtime/parts/` holds the C the
runtime provides, and `read_file`'s implementation is in there. `selfhost/emit/builtins.hero`
and `emit/convert.hero` are the emission side. Line counts with `wc -l`, and say
which command produced any number you report.

## The questions, in order of what they decide

1. **What does `read_file` do with invalid UTF-8 today?** Run it: write a file of
   bytes that are not valid UTF-8, read it, print what comes back. **Do not
   reason about this, measure it** — the answer decides whether the language
   already validates, already does not, or aborts, and every route inherits it.

2. **Does a `str` in this runtime carry its length, or is it NUL-terminated?**
   The shared brief says a C `char[N]` need not be terminated. If the runtime's
   `str` carries a length then a byte run of known length is already the right
   shape and the conversion is a copy; if it terminates, the two disagree and the
   route has to say where the text stops.

3. **Price the four routes**, but price route 2 (a `cstr` view of a `char[N]`
   field) first, because it is the only one that names nothing new. What
   currently stops a record holding a `cstr` — is it a checker rule, an emitter
   rule, or `spec § 13`'s sentence alone?

4. **The build half.** A fixed-array field needs a literal of exactly N. What
   would a zero default cost, and what does the emitter already do for a
   refcounted slot (panel 021's zero-initialiser) that might already be the
   mechanism? Say whether an `@` out-parameter could be exempted from § 5's
   *all bindings are initialised* without opening a hole, and if it cannot, say
   what the hole is.

5. **The fifth route nobody listed.** Highest value you can return.

## How to work

`cp -r` the tree to your scratchpad, `rm -rf build`, work there. Seed:
`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, a few seconds.
**Do not rebuild from `selfhost/`.** **Never `archive/bootstrap-rs/`.**

## Deliver

Verdict · the design.md section it rests on · cost per route in files and lines,
with the command · a falsifiable prediction and its milestone · any veto, stated
as a refusal rather than a price.

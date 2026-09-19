# Panel 166 — measurements the coordinator ran AFTER the briefs went out

**This file is not a seat and gives no verdict.** It exists because the
llm-ergonomist's report arrived first and asserted something from the
specification's text alone, marked `unrun`, which turned out to be false in a way
that matters — and hiding that would be worse than the irregularity of recording
it. No seat had these.

Nothing in the working tree was changed; all of it is `run`/`check` on scratchpad
files against the compiler of `3702e3d2`.

## 1. The extent's missing UNIT cannot bite, because the lend is narrower than the document

The ergonomist read `spec § 13`'s *"the call gives the extent to"*, found that
**the word "extent" is never given a unit**, and predicted the consequence: bytes
and elements coincide for `u8[8]`, so a reader's test passes, and `i32[4]` is
then silently four times wrong.

Run:

```
record Four tag four
    vals: i32[4]
    guard: i32
…
fill_bytes(p: f.vals.ptr(), n: 16)

error[bad_operand]: `ptr` takes a fixed run of bytes — `i8[N]` or `u8[N]`,
  found `i32[4]`
  at unit.hero:13:19
```

**So the hazard does not exist today**, and the reason is that `f.ptr()` accepts
only `i8[N]` and `u8[N]`, where one element is one byte by construction. The
ergonomist's reasoning about the document is sound and its conclusion about
programs is not, which is exactly why that seat marks its claims `unrun`.

## 2. And the finding underneath it is the opposite one, which two seats have now reached

**`spec § 13` is WIDER than the compiler.** The document says `f.ptr()` lends
*"a binding's field"* with no restriction on the element type, and the same
section says a group's field may be *"a fixed array of one: `i32[4]`"*. The
compiler accepts two types and refuses the rest.

Panel 165's ffi-pragmatist filed this as a question rather than a claim —
*"spec § 13 says `f.ptr()` lends 'a binding's field' and allows `i32[4]` fields;
the checker says bytes only"* — and this is the second route to it.

`selfhost/check/lend_types.hero` states the refusal deliberately for the dynamic
`[u8]` case, in its own words: *"a route nobody priced stays refused, which is
Principle 0 read the way § 2 writes it."* **It says nothing about `i32[4]`**,
which is a fixed run with a C spelling and the shape the document names.

So a seat weighing route F, or any wording, should know that the gap between the
document and the compiler here is **a live disagreement and not a rounding**, and
CLAUDE.md § 12 has a default for it: spec beats compiler, the compiler has the
bug. Whether that default is right here is the sitting's to say — the opposite
resolution, narrowing the document to bytes, is cheaper and is what the compiler
already does.

## 3. What this does not settle

Whether `i32[4]` SHOULD cross is unmeasured: nobody has priced what the emitter
would do with a non-byte fixed run, and the coordinator did not try. It is a
question for the compiler-engineer and the ffi-pragmatist, who are both running.

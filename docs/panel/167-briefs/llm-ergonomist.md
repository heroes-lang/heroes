# Panel 167 — brief for the llm-ergonomist

**Your input is `spec/heroes-spec.md` and this file. Nothing else.** Do not read
the repository, the design document, or any other brief: your verdict is an
experiment, and it is worth having only because your input is controlled.

## The situation, stated without the compiler

`spec § 13` gives a program two ways to hand a run of bytes to C:

- **a lend**, `f.ptr()`, which gives C the address of a record's field;
- **a lease**, `x: cstr @ s.lease()`, which gives C a COPY of a string's bytes
  that C may read *for as long as the program says*, ended by `end_lease(@x)`.

The document says how long a lease is good for. It does not say how long a lent
address is good for.

## Three tasks. Do them in order and report what you wrote before reading on

**Task 1.** Reading § 13 alone, write a Heroes program that binds a C function
`void k_register(const void *p, long n)` which **stores the pointer in a global
and reads it later**, and fills an 8-byte field of an `extern` record for it to
keep. Then answer, from the document only: after your function returns, what
does C's stored pointer point at? Quote the sentence you based the answer on, or
say the document does not answer it.

**Task 2.** Here are three candidate sentences for § 13, label-stripped. For
each, say what a program may and may not do under it, and whether you could
write Task 1's binding correctly on the first try:

- **(a)** *"`f.pin()` is a COPY of a field's bytes that C may read for as long
  as the program says, and `end_pin(@p)` frees it; a pin nobody ends aborts
  when `main` returns, saying how many."*
- **(b)** *"A `ptr` parameter C keeps past the call is declared `keeps`, and a
  lend may not reach one."*
- **(c)** *"A lent address dies when the record's binding does; a C function
  that keeps it is reading memory the program has reused."*

**Task 3.** One of the three above costs a reader a new word and a new release
to remember, one costs a word on the binding, and one costs nothing to learn and
everything to get wrong. Rank them by **how many ordinary programs — programs
with no FFI in them at all — pay for the rule**, and say why.

## What your veto is for

Non-locality: a fact about what a line of Heroes does that is not visible at
that line. Say plainly if any candidate has it.

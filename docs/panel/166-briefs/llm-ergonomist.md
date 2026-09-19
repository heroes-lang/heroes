# Panel 166 — brief for the llm-ergonomist

**Your only input is `spec/heroes-spec.md`.** Read it in full. Do not read any
other file in this repository, and do not look for design documents or other
panel material. Your verdict is an experiment about what a reader of the
specification does, and material that reader would not have would spoil it.

## Why you are being asked again

At the previous sitting you held a veto on one form: *a fixed-array parameter C
writes through, unmarked*. You predicted that a reader would write the call with
no `@` at either site and then read C's bytes back, and that both resolutions
would be silent.

**That prediction has since been confirmed as a fact about the shipped
language**, not about a proposal. This sitting is about what the specification
should say now.

## What the specification says today, and what a program does

`spec § 13` carries this sentence:

> `f.ptr()` lends a binding's field to a `ptr` parameter the call gives the
> extent to, and C may write back through it.

Three questions, and please answer each from the document alone before you form a
view.

**Task 1 — the immutable binding.** Read `spec § 5`'s sentences about `=` and
`@`, and `spec § 3`'s sentences about copies and aliasing, alongside the `f.ptr()`
sentence above. Then answer: **if a program binds a record with `=` and lends one
of its fields with `f.ptr()` to a C function that writes, what does the
specification say happens?** Quote every sentence you used. Say whether the
document answers the question, contradicts itself, or is silent — and say which
of the three you think a reader would conclude.

**Task 2 — the extent.** The sentence says *"the call gives the extent to"*.
Write a program that lends an 8-byte field to
`function sum_n(p: ptr, n: i64) -> i64`. Then answer, from the document alone:
**what stops a reader from writing `n: 64`?** If nothing does, say what the
document would have to say for something to.

**Task 3 — naming the number.** A reader wants to avoid writing the extent as a
literal, because the header may spell it differently on another machine. Using
only the specification, find every way to name that number. Try at least: asking
the field its length; declaring the field with a named extent; and a group
`constant`. Report what the document licenses for each, and what you would
expect to compile.

## What your verdict must carry

A verdict (approve / object / veto) on each of these, stated as what the
specification should say:

- **A** — a `ptr` lend from a `=` binding is refused;
- **B** — a `ptr` parameter C writes through is `@`, at the declaration and the
  call site, as `spec § 13`'s own *"A C out-parameter is an `@` parameter"*
  already says for other types;
- **G** — the document simply narrows § 3 and § 5 to admit the hole, and nothing
  is checked.

Plus **one falsifiable prediction** stated so a later sitting can score it, and
what would make you wrong. You hold a veto on a construct whose meaning cannot be
worked out from the place it is written.

**Mark every "would it compile" claim as derived from the text and unrun**, as
you did last time. That was the right call and it is why your report could be
trusted.

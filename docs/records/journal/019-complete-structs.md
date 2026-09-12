# 019 — Complete structs: every field form a C header can write

The milestone nobody planned. `M-struct-passing` closed at midday with a struct
crossing the boundary by value and a headline of **191 of raylib's 349** by-value
crossings. By evening the number was **349 of 349**, and none of the four steps
between was a feature: each was a refusal that had no escape.

It gets its own id because §14 says so — *a milestone that changes shape gets a
new one* — and because its deliverable is a different sentence. The one before it
says *a struct can cross*. This one says **every struct a C header writes can be
named**.

## The goal

There was none, at first. The author's instruction was *"ok rendili legabili"*,
then *"fai la soluzione più robusta"*, then *"è fondamentale chiamare il C,
altrimenti non andiamo da nessuna parte"* — and that last sentence is the goal
this milestone turned out to have. §1.11 says everything comes from C. A boundary
that binds most of a library is a boundary that leaves the author writing C by
hand for the rest, which is §1.11 failing at the one thing it exists to do.

Three panels sat: **060** on how a struct crosses, **061** on the fields nobody
names, **062** on a C array member. Two of the three produced a veto, and both
vetoes were about the *shape* rather than the feature.

## What surprised

**Four numbers, and each of the first three was wrong for a different reason.**
191 was the reach of a mechanism nobody had built yet. 200 was that mechanism
shipped with a `ptr` rule that asked whether a header **spells** a member `void *`
— a fact about the header's prose rather than about the program. 328 added the
rule that asks whether it *is* a pointer. 349 added the array member. Panel 060's
own headline, 337, turned out to be the middle row of a table taken before the
thing it measured existed.

**Two refusals shipped with no escape, and that is the shape this project had
cited against an option four hours earlier.** Panel 061's historian argued
against Nim's polarity partly on Nim issue #19040 — five years of a diagnostic
naming a repair that does not work. Then `error[ffi_incomplete_record]` told an
author that `Camera2D` *"does not name `rotation`"* on a record naming all four of
its fields, because C's brace elision spent a flat zero list into the inner
struct; and `error[ffi_field_type]` told them to correct `Font.recs`, for which no
correct spelling existed. Both are the same failure, and both were written by the
seat that had just quoted the precedent against it.

**A guarantee can be switched off by a type nobody thought about.**
`extern_probe.rs`'s parameter table had no arm for a group record, and
`parameter_list` collects into an `Option<Vec<_>>` — so **one** struct parameter
deleted the probe for the **entire function**. `ffi_parameter_type` and
`ffi_writable_parameter`, a whole milestone's deliverable, were off for every
signature touching a struct, proven live by a golden shipped that morning. The
tell was that the sibling table had been widened and this one had not: two tables
answering one question, one grown.

**The loud fallback is not belt-and-braces.** `types/partial.rs::map_keys` walked
written type *nodes* while its own doc said the hazard is in the *type*, so a map
**literal** slipped past. What caught it was the `hero_panic` body a judge had
made a condition — `heroes check` passed at exit 0 and the program aborted **by
name**. Without that condition, two C structs differing only in unnamed bytes
would have collided as one map key, silently.

**And the toolchain cannot be the net at the C boundary.** AddressSanitizer is
documented blind to an intra-object overflow in a **C** struct;
`-Warray-bounds` does not fire on a temporary subscript, which is all this emitter
writes. So the bound on a fixed array is the compiler's, and the subscript is kept
on an array-typed lvalue for the one arm — UBSan's — that remains.

## What broke and why

**Six catch-all arms in one day**, none of them a slip: each was written when the
case it now swallows did not exist. A formatter that hoisted a record out of its
group and emitted **a different program that still parses**. A conversion that
returned `ok(0)` for every input because the destination's zero-initialiser spells
success. A `(void *)0` where a struct was wanted, refused correctly by clang at
**exit 2** — the compiler blaming itself. Two walks over `program.functions` that
stopped being every group member the day `record` joined them. And `c_spelling`
answering `None` for an array field, so **no assertion was emitted at all**.

That last one is the one to keep. Its arm carries a comment, written the day
before, saying the next type to arrive should be a `None` somebody chose. It
arrived as a `None` nobody chose, through the arm written to prevent exactly that.

**Five call sites survived a rename that existed to catch them.**
`M-struct-passing` split `Names::of()` from `Names::satellite()` because a group's
`record` is the only declaration where the C type and the generated-function
prefix differ. The split landed at the definitions; five uses kept reading the
type table, so `==` on **any** group record was `call to undeclared function` at
exit 2, and `[Color]` emitted a global unmangled `Color_desc` beside the library
that declared `Color`. The consequence outranked the bug: the two operations panel
061 spent spec tokens forbidding could not be performed on any group record at
all, so the refusal's control arm was unwritable and its golden would have passed
**vacuously**.

**And a comparison that was reflexive rather than structural.** `a->arr == b->arr`
on a C array field is legal C with zero diagnostics under twelve flags plus
`-Wextra`, and compares **pointers** — two byte-identical structs answer `false`
while `eq(a, a)` answers `true`. It is what the array field would have done had it
reused the scalar row.

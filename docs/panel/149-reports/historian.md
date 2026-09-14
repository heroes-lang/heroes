# Panel 149 — report of the historian

Advisory, no veto. Written out by the coordinator because this seat has no write
tool. Every claim below is either a page the seat fetched on 2026-09-14, with the
link, or is labelled `unverified` and carries what was searched for.

**Verdict: approve (advisory) — approve R1 deep, approve R2 path, object in part
to R3 as framed.**

## The finding that reframes the sitting

The brief offered two shapes, precedent for stopping at the result or precedent
for going deeper. **The evidence fits neither, and the third shape is the
finding: the split is not how deep the rule looks, it is WHERE THE WORD IS
WRITTEN.**

- **Every system that writes the mark on the FUNCTION stops at the function's own
  signature.** Clang's `cf_returns_retained` family, Swift's
  `SWIFT_RETURNS_RETAINED`, GObject-Introspection's schema, SAL's allocation
  family, Core Foundation's Create Rule. Five families, decades of shipping, not
  one lets a mark on a function describe a pointer inside a returned aggregate.
- **Every system that reaches a field writes the mark on the FIELD or the TYPE.**
  ARC qualifies the field's type with `__strong` and fixes a calling convention
  so no producer ever speaks. Splint annotates the field. Vala defaults it.

So R3 is not a gap in Heroes' surface. It is the predicted symptom of asking a
producer-level word to describe a field-level fact.

## The precedents, each with what was fetched

**A. Clang `cf_returns_retained` / `ns_returns_retained` stops at the result.**
The documented subjects are functions and methods: *"annotates an Objective-C
method or C function as returning a retained Core Foundation object"*. No mention
of a struct field, a member of a returned aggregate, or any nested pointer. Same
for `ownership_returns` / `ownership_takes` / `ownership_holds`, described as
marking *functions* and the *index of a parameter*. Core Foundation's Create Rule
and Get Rule are both about the returned object, never about objects reached
through it. **This is the direct answer the brief asked for: the closest living
precedent to `acquires` / `borrows` stops at the result, and Heroes' current
narrow rule is the industry's shape.**

**B. Clang ARC and struct fields — forbidden for five major releases, then made
deep by CONVENTION and never by a function annotation.** This is the most
load-bearing finding. Six versions of the ARC specification were fetched.
Forbidden in 3.5, 7.0.0, 8.0.0, 9.0.0 and 10.0.0, each with the identical
sentence:

> A program is ill-formed if it declares a member of a C struct or union to have
> a nontrivially ownership-qualified type.

with the rationale: *"C does not give us very good language tools for managing
the lifetime of aggregates, so it is more convenient to simply forbid them."*

Supported in 11.0.1 and 13.0.1:

> a function returning a non-trivial struct may be written in ARC and called from
> non-ARC or vice-versa. The convention for this always transfers ownership of
> objects stored in `__strong` fields from the callee to the caller

**Clang faced Heroes' exact question, refused the shape for five major releases,
and when it answered it did not add a function-level annotation.** It qualified
the field's type and fixed a universal calling convention, so that no producer
has to say anything. The releaser is the object's, not the function's.

`unverified`: the implementation is reported by search snippets to predate the
doc change (llvm-svn r326307, February 2018); that commit was not fetched. What
is verified is the specification text, version by version.

**C. GObject-Introspection — three words, and `field` cannot carry any of them.**
`(transfer none)`, `(transfer container)`, `(transfer full)`, applying to
*"identifier (only properties), parameters, return value"*. The RELAX NG schema
on `main` was fetched: `TransferOwnership` appears on `Property`,
`Callable.return` and `Callable.params` only. The `field` element admits `name`,
`writable`, `readable`, `private`, `bits` and a type, and **no
`TransferOwnership`**. Faced with *a thing handed over whose contents are not*,
GNOME did not recurse and did not repeat the mark: **they added a third word.**

**D. Microsoft SAL went deep into aggregates for SHAPE and not for ownership.**
The complete struct annotation list is `_Field_range_`, `_Field_size_` and its
variants, `_Field_z_`, `_Struct_size_bytes_`. Every one is a shape fact: a range,
a buffer extent, a null terminator. Not one expresses allocation, ownership or
need-to-release; the allocation family is a `_Post_` condition on a function's
result or output parameter. **Stated as a question rather than a claim, per
CLAUDE.md § RUN IT**: *does any `_Field_` annotation in `sal.h` express
ownership?* The seat searched `_Field_` against ownership, allocates,
needs-release and free and found nothing, and did not fetch `sal.h`.

**E. Swift's C++ interop — Heroes' rule almost exactly, and the mandatory warning
was switched off for NOISE.** `SWIFT_RETURNS_RETAINED` and
`SWIFT_RETURNS_UNRETAINED` on functions and methods, on the returned value,
nothing about a reference type inside a returned struct. Then the part the
sitting should hear: PR #82488, merged **2025-07-15**, re-enables the warnings
for unannotated APIs only behind an experimental flag, off by default. Per its
description they had been **disabled in Swift 6.2 because of noise**. Panel 148
R5 is the same rule, and **Swift shipped it, found it too noisy, and retracted it
to a flag inside one release cycle.** Applicability limit, stated so nobody
over-reads it: the noise was duplicate warnings across template instantiations
and Heroes has no templates. What transfers is not the mechanism, it is that a
mandatory-annotation rule at an FFI boundary has a noise budget and somebody blew
it.

**F. Go's cgo — the deep recursive walk exists, is called expensive, and was
demoted off the default path.** *"When passing a pointer to a field in a struct,
the Go memory in question is the memory occupied by the field, not the entire
struct."* Complete checking is `GOEXPERIMENT=cgocheck2`, selected at build time
since Go 1.21 (released 2023-08-08), and `internal/goexperiment` calls it *"an
expensive cgo rule checker"*. **Precedent for going deeper in the strongest form,
and it was priced and demoted.** The disanalogy, stated before anyone quotes it
against R1: Go's walk is at runtime, over values, on every call; Heroes' is at
compile time over a finite declaration graph. The cost argument does not
transfer. The shape does.

**G. Splint (LCLint) — went all the way deep, and chose the OPPOSITE default from
Heroes.** *"Unannotated return values, structure fields and global variables are
assumed to be `only`."* `only` carries *"an obligation to release the storage
associated with the reference before the reference is lost"*, and struct fields
are annotated directly. A thirty-year-old precedent for the deep rule, and the
one place where Heroes' instinct and the precedent point opposite ways:
**Splint's default for an unannotated struct field is OWNING; Heroes' current
default is exempt. Splint's default is the one that fails loudly.** Last release
3.1.2, 12 July 2007.

**H. Cyclone's own experience report names the struct field as exactly where
uniqueness broke, and names the false positives.** ISMM'04, Hicks, Morrisett,
Grossman, Jim. On placing owned pointers inside aggregates: *"placing unique
pointers in non-unique objects can lead to subtle 'leaks.'"* The direct ancestor
of `borrows`: *"A borrowed pointer is a second-class copy of a unique pointer
that cannot be deallocated and cannot 'escape.'"* And the cost sentence, on
BetaFTPD: *"this required 21% of the code to be changed … we were forced to spend
some time tracking down memory leaks that arose from failing to decrement a
count. **The warnings issued by the compiler were of little help, since there
were too many false positives.**"*

**I. Vala — the closest architectural twin, and it goes deep by DEFAULTING.**
*"All parameters are, by default, unowned, unless marked with the `owned`
keyword. All return values and `ref` and `out` parameters are, by default, owned,
unless marked with the `unowned` keyword."* Note what Vala does that Heroes
deliberately does not: it defaults. Heroes demands the word, and given the thesis
that departure is correct and should be recorded as deliberate, because a default
silently picks a side and a silently picked side is not a compile error.
`unverified` and **struck until sourced**: whether a Vala FIELD can be `unowned`
and what a field's default is. Two pages were fetched and neither states a rule.

**J. Rust — no ownership marks at the boundary at all.** The `-sys` crate
*"should provide declarations for types and functions in `libfoo`, but not
higher-level abstractions"*, and the discipline lives in a hand-written wrapper
above it. `unverified`, stated as a question: *does anything in the Rust
ecosystem mechanically detect an owned pointer inside a struct returned by an
`extern "C"` function?* bindgen's user guide and issue tracker were searched and
nothing was found; that negative is the seat's vocabulary, not the world's.

## Advisory verdicts

**R1 — approve the deep walk, with one shaping note.** Go walks these fields at
runtime on every call and ships it; ARC walks them in its triviality
computation; Splint walks the declared fields. *One level* has no precedent in
any of the nine systems above. The shaping note: `crosses_the_boundary` recurses
safely because it walks the **interned type table, acyclic by construction**,
while `acquiring` runs on a user-writable cyclic declaration graph. ARC computes
triviality over the TYPE, not the declaration. **Walk the type table for the same
reason, and the visited set becomes a belt over braces rather than the only thing
standing between the pass and non-termination.**

**R2 — approve the path, `Outer.p.s`.** Splint and Vala report at the field, and
design.md §4.17 asks that a diagnostic be fixable without opening another file,
which *the producer's name alone* fails the moment the handle is two records
down. Cyclone's authors wrote that their warnings *"were of little help, since
there were too many false positives"*: a deep rule's diagnostics decide whether
it survives contact with users, and the path is most of what makes one
actionable.

**R3 — object in part.** Precedent does not favour *allow the mark to repeat*:
GNOME hit the identical wall and grew the vocabulary by one word rather than
repeating, and ARC hit it and made the releaser a property of the object so the
mark disappeared. The shared brief's measurement says the same thing from below:
`runtime/parts/alloc.c:103` is one global `_Atomic int64_t`, so no surface syntax
can express two handle types until the counter is per-type. **The robust
resolution precedent supports is to move the releaser onto the handle TYPE** — a
`Slot` is closed by `slot_close` whoever produced it — which makes both the
multi-handle record and the fixed array fall out with no new surface. Per CL-040
the conservative alternative is recorded for the author: **refuse** a returned
type reaching more than one handle type, naming both fields; a refusal owes the
fact that would make it wrong, and here it is checkable, namely the day an extern
in the closure list must return a record carrying two distinct handle types.

## The prediction, and it was RUN and CONFIRMED on the number it named

> The deep rule's first casualty will not be a handle two records deep. It will
> be the fixed array, and it will break the `acquires` word's arity before it
> breaks anything else. If it exits 0, I am wrong. If it ends at +3 or aborts
> 134, the mark's arity is wrong and the failure is about COUNT and not only
> about KIND.

**Run by the coordinator on 2026-09-14, in the scratchpad, immediately on receipt
of this report.** `record Four` holding `a: Slot[4]`, producer marked
`four_make(n: i64) -> Four acquires slot_close`, all four elements released:

```
0
panic: 3 more handle(s) given back than were taken — …
   -> run exit 134
```

**Exactly +3, abort 134.** One mark produced one increment and four `consumes`
ran. The prediction is confirmed on its own number, and it settles more than R3:
the fixed array reaches **four handles of ONE type**, so a resolution that only
refuses multiple TYPES does not close it.

## Conditions that would change this reading

1. **A shipped function-level annotation naming a field of a returned
   aggregate** — a SAL `_Field_` ownership annotation from `sal.h`, a GIR
   `<field transfer-ownership=…>` accepted by `g-ir-scanner`, or a clang
   attribute taking a member path. Then *the word goes on the function* is not
   the industry's line and R3 could repeat the mark with precedent behind it.
2. **A Vala VAPI page stating field ownership rules**, which would move Vala to a
   second live example of field-level marks in a compile-to-C language.
3. **A tool that implemented the deep aggregate walk statically and then removed
   it.** The seat found demotion (Go, on runtime cost), deferral (ARC, nine
   releases) and decay (Splint, unmaintained since 2007), but **no static checker
   that shipped the deep walk and then deleted it.** If one exists with a stated
   reason it is the single most important document for this sitting, and it would
   flip the R1 approval to an objection.

## Sources, each fetched 2026-09-14

- https://clang.llvm.org/docs/analyzer/user-docs/Annotations.html
- https://developer.apple.com/library/archive/documentation/CoreFoundation/Conceptual/CFMemoryMgmt/Concepts/Ownership.html
- ARC specification, versions 3.5, 7.0.0, 8.0.0, 9.0.0, 10.0.0, 11.0.1, 13.0.1 at
  `https://releases.llvm.org/<version>/tools/clang/docs/AutomaticReferenceCounting.html`
- https://gi.readthedocs.io/en/latest/annotations/giannotations.html
- https://raw.githubusercontent.com/GNOME/gobject-introspection/main/docs/gir-1.2.rnc
- https://learn.microsoft.com/en-us/cpp/code-quality/annotating-structs-and-classes?view=msvc-170
- https://www.swift.org/documentation/cxx-interop/
- https://github.com/swiftlang/swift/pull/82488
- https://pkg.go.dev/cmd/cgo · https://go.dev/doc/go1.21 ·
  https://pkg.go.dev/internal/goexperiment · https://go.dev/doc/devel/release
- https://splint.org/manual/html/sec5.html · https://splint.org/
- https://personal.utdallas.edu/~kxh060100/Papers/cyclone_ismm04.pdf
- https://docs.vala.dev/guides/bindings/writing-a-vapi-manually/05-00-fundamentals-of-binding-a-c-function/05-02-ownership.html
- https://doc.rust-lang.org/cargo/reference/build-scripts.html

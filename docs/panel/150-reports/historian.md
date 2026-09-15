# Panel 150 — report of the historian

Advisory, no veto. Written out by the coordinator, which this seat has no tool
for. Every external claim carries a URL fetched 2026-09-15; no Heroes number here
is the seat's own measurement and each is attributed.

**Route disclosure, offered before it was asked for.** *"At panel 149 I argued
for moving the releaser onto the handle type. That is panel 147's Route A,
refused on its axis and ratified 2026-09-14. Nothing in this report puts an
ownership word on a type."* Every precedent below was checked for where the word
is written, and all of them write it on a function or a call.

**Verdict: approve, advisory, both questions** — three conditions on Q1, a named
shape on Q2.

## Q1.1 — Clang's `ownership_returns` family answers *every `void *` is one type*

Fetched clang's analyzer annotations page. Verbatim:

> Use this attribute to mark functions that return dynamically allocated memory.
> **The first argument is the type of the allocation** (e.g. `malloc`, `new`, or
> any other identifier).

```c
void __attribute((ownership_returns(malloc))) *my_malloc(size_t sz);
void __attribute((ownership_takes(malloc, 1))) my_free(void *);
```

A free-form class string, **written on the producing function's declaration**,
discriminating two unrelated allocators that share one C type.

**And it is off by default.** `unix.DynamicMemoryModeling:Optimistic` defaults to
`false`, and the documentation says the flag makes the analyzer assume *"that all
memory allocations and deallocations … are marked"*. **The clang docs PR (#121759,
2025-01-06) says instead that the flag is what makes the analyzer acknowledge the
attributes at all.** The seat marks the discrepancy **unverified**, having not run
clang, and notes both readings give Q1 the same lesson: *"every producer is
marked" is precisely the clang `Optimistic` assumption, and clang ships it off.*

**The string went uncross-checked for years.** PR #98941 (merged 2024-07-24) is
what made mismatched *custom* classes an error; its author wrote of the prior
state, *"It does not detect. As well as for other attributes."*

## Q1.2 — SAL is REFUTED, and the brief was wrong

**The coordinator's shared brief guessed that SAL's `__drv_allocatesMem` takes a
kind argument for this reason. It does not.** Fetched the shipped Windows SDK
header, whose own comment reads:

> **Kind is unused**, but should be 'mem' for malloc/free and 'object' for
> new/delete.

Documentation, not a discriminator. **SAL offers this sitting nothing on the
`ptr`-collision problem and any seat citing it should stop.**

## Q1.3 — who made it mandatory, and what it cost them

**Swift retracted it and then kept it.** Warnings for unannotated C++ APIs
returning shared-reference types shipped in 6.1; adopters reported false
positives — one third-party report on the PR counts **~32,000 diagnostic lines on
one project** — and they were **disabled in 6.2** (PR #81411, merged 2025-05-10),
**re-enabled behind an experimental flag** with per-source-location caching (PR
#82488, 2025-07-15), and finally **put in a diagnostic group, on by default** (PR
#85021, merged 2025-10-24). **Somebody kept it**, at the price of five and a half
months, one retraction and an off switch.

**What made it survivable is the one thing Heroes' `ptr` lacks: a type-level
predicate.** Swift's warning fires only on functions returning a type the C++
author marked `SWIFT_SHARED_REFERENCE`. No mark on the type, no demand on the
call.

**Google's C/C++ Thread Safety Analysis is the strongest *kept it* in the
record**, and its vocabulary is this language's — ACQUIRE, RELEASE, *produces a
unique capability*. SCAM 2014, read in full:

> The analysis is turned on by default, across the company, for every C++ build.
> Over 20,000 C++ files are currently annotated, with more than 140,000
> annotations…

> about 50% of the warnings produced by the analysis are caused not by incorrect
> code but rather by incorrect or missing annotations…

> Excluding cases in which the annotations were clearly wrong, the false positive
> rate is otherwise quite low: less than 5%.

It ships `NO_THREAD_SAFETY_ANALYSIS` as an opt-out.

**A note that corroborates panel 147 rather than challenging it.** That paper's
first stated limitation is *"No attributes on types … Attaching attributes to
types would result in a better and more accurate analysis. However, it was deemed
infeasible for C++."* They took the declaration axis for feasibility; Heroes took
it for correctness, since a type comes back owned from one function and borrowed
from another. **Same axis, better reason.**

## Q1 — the mechanism nobody in this sitting had named

**Clang's audited nullability regions.** r240156, Douglas Gregor, 2015-06-19:
inside `#pragma clang assume_nonnull begin` / `end`, single-level pointers are
inferred `_Nonnull`. This is **a mandatory, default-on completeness demand on an
untyped pointer**, made survivable by scoping the demand to a region the author
explicitly opened, with a default inside it.

**And the `extern` group that declares `ptr consumes` IS an audited region.** The
coordinator measured the blast radius at one file; **that is not a coincidence,
it is what an audited region buys.**

**It needed a third word.** r240596, 2015-06-24, added `_Null_unspecified`
alongside `_Nonnull` and `_Nullable`, to say *I looked and I am not saying*
without lying. **Heroes has two.**

## Q2 — the fixed array, and three systems that all refuse the caller's loop

**GObject-Introspection already carries panel 149's critic's WHOLE-versus-SHELL
distinction, and has for two decades, on the FUNCTION's return value.**

> **transfer full**: … For a container type, this means the recipient owns both
> container and elements.
> **transfer container**: the recipient owns the container, but not the elements.

**And the element route exists as one call, not N.** `g_list_free_full(GList*,
GDestroyNotify free_func)` — *"frees all the memory used by a `GList`, and calls
`free_func` on every element's data"*, since GLib 2.28. **The caller supplies the
element destructor; the walk belongs to the callee.**

**Clang's thread safety analysis refuses exactly Heroes' Q2 program.** From the
paper, verbatim:

```c
void lockAll() {
  // Warning: capability sets do not match
  // at start and end of loop.
  for (unsigned i=0; i<n; ++i)
    mutexArray[i].lock();
}
```

**The largest deployed acquire/release annotation system in existence refuses the
per-element aggregate walk and calls it a limitation**, justifying shipping
anyway on the ground that conditionally held locks are rare in practice.

**ARC forbade ownership inside C aggregates outright for years** — *"A program is
ill-formed if it declares a member of a C struct or union to have a nontrivially
ownership-qualified type"* — and when it lifted the ban (D41228, committed
2018-02-27) the repair was **IRGen synthesising per-field copy/destroy
functions**: one obligation on the whole value, discharged by a
compiler-generated walk. **That is panel 149's R3, arrived at independently.**

**Rust removed the ability to hold a partially-owned aggregate**: `mem::uninitialized`
deprecated since 1.39.0 because *"the function basically cannot be used
correctly"*, replaced by a form where each slot carries its own liveness.

## Advisory verdicts

**Q1 — approve the GROUP-SCOPED demand, with three conditions, each from a
shipped system.**

1. **Do not add a module or class argument to the mark.** Clang's exists because
   clang had nothing better; `acquires <releaser>` already discriminates two
   unrelated `ptr` allocators in one group **more precisely** than
   `ownership_returns(sqlite)` would, and clang carried its string uncross-checked
   until 2024. **Import nothing; the discriminator exists.**
2. **Key the demand on the opened region, never on `ptr`.** Clang's nullability
   pragma is the shipped form, and the blast-radius-of-one-file is what it looks
   like when it works.
3. **Expect to need a third word and price it now rather than in a defect.**
   `_Null_unspecified` and `NO_THREAD_SAFETY_ANALYSIS` both exist for the
   producer that is neither owning nor borrowing.

**Q2 — approve *refuse the caller's element-by-element release*.** The shape the
precedent offers, if a route is ever wanted, is `g_list_free_full`: one call
taking an element destructor, **the walk on the callee's side**, which stays on
panel 147's axis because the word is on a call and names a function.

## What could not be sourced, said plainly

Whether clang's `Optimistic=false` means *attributes ignored* or *completeness
not assumed*; whether `-Wnullability-completeness` fires only in files already
carrying an annotation; when clang's `ownership_*` attributes were introduced;
which Clang release shipped the ARC struct-field lift; that GI's documentation
*forbids* per-element ownership variation, as opposed to simply not offering an
annotation for it. **And a second clean retraction of an ownership-completeness
diagnostic**: the seat searched Swift, Clang and GCC, found one, and declines to
claim two.

## Prediction

**If a group-scoped demand on `ptr` producers lands, Heroes will add a third
producer word or a suppression mechanism within two milestones** — driven by a
correct-but-noisy rule, not an unsound one. Every system that made this demand
mandatory shipped three values or an off switch.

**What settles it**: at the close of the second milestone after the rule lands,
`git diff spec/heroes-spec.md` between the landing tag and that close. **Held** if
it adds a producer-side word beyond `acquires`/`borrows`, or any suppression for
this diagnostic. **Missed** if two milestones pass with exactly those two words
and no suppression of any kind.

## Conditions

- **Q1 flips to object** if someone produces a shipped system demanding an
  ownership mark on a pointer type carrying **no author-supplied predicate** — no
  audited region, no type-level mark, no allocation class — with a measured
  false-positive rate. Five systems were searched and all five narrow first.
- **Q2's refusal softens** if a shipped vocabulary is found expressing WHICH
  elements of a fixed-size aggregate carry an obligation where the count is a
  runtime choice.
- **The whole Q1 reading is overturned** by a primary clang statement that
  `Optimistic=true` is the default in any shipping configuration. That single
  fact is load-bearing for *nobody makes this mandatory*.

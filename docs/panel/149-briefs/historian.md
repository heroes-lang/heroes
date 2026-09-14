# Panel 149 — brief for the historian

Read `docs/panel/149-briefs/00-shared.md` first. You judge precedent, advisory
only, no veto. **You must verify every date, line count and claim by web search.**
This seat is the most hallucination-prone in the panel and unsourced precedent is
inadmissible: a claim with no link you actually fetched is struck, and it is
better to return three sourced findings than nine remembered ones.

## The question in one line

A foreign-function binding says whether a call hands ownership of a resource
over. Heroes asks this of a result and an out-parameter and not of a resource
riding home **inside a returned struct field**. Who else has had this exact
problem, what did they do, and what did it cost them?

## Where to look, and what to bring back

1. **Rust's `bindgen` and the `#[must_use]` / ownership gap.** Rust's FFI story
   has no ownership marks at all at the `extern "C"` boundary: everything is
   `unsafe` and the wrapper carries the discipline. Find what the community
   built on top — `bindgen`'s generated code, the `*-sys` crate convention — and
   whether anything mechanically detects an owned pointer inside a returned
   struct. Sourced.

2. **Swift's `CF_RETURNS_RETAINED` and Objective-C ARC's attribute family.**
   This is the closest living precedent to `acquires` / `borrows`: Core
   Foundation annotates functions with `CF_RETURNS_RETAINED` and
   `CF_RETURNS_NOT_RETAINED`, which is exactly the two-word answer Heroes landed.
   The question for you: **does that family reach a pointer inside a returned
   struct, or does it stop at the result too?** If it stops, that is precedent
   for the narrow rule and the sitting must hear it. Find the clang
   documentation for the attribute and quote what it applies to.

3. **GObject-Introspection's transfer annotations.** `(transfer full)`,
   `(transfer none)`, `(transfer container)` — three values where Heroes has two.
   `(transfer container)` is the case of a container handed over whose ELEMENTS
   are not, which is the multi-level question R3 asks in another vocabulary.
   Bring back what the three mean and how they are written, and whether they can
   be applied to a struct field.

4. **SAL annotations in the Windows SDK** (`_Out_`, `_Outptr_result_maybenull_`,
   and the `__drv_` allocation family) and **clang's `ownership_returns` /
   `ownership_takes` / `ownership_holds`** attributes used by the static
   analyzer. Same question: do they reach inside an aggregate?

5. **Anything that tried the deep version and abandoned it.** That is the most
   valuable thing you can find. A language or tool that walked struct fields
   looking for owned pointers and then stopped — and why. Cyclone, Vault,
   Deputy, SPARK, Frama-C's ACSL, or a static analyzer's release notes.

## What would change the sitting's mind

Two shapes of finding, and say which yours is:

- **precedent for stopping at the result** — the attribute families above all
  stop there, which would say the narrow rule is the industry's answer and the
  sitting is proposing something nobody ships;
- **precedent for going deeper** — a tool that does walk aggregates, what it
  cost, and whether it kept the feature.

## What to return

Findings with links you fetched, each with its date. Say plainly which claims
you could not source. **One falsifiable prediction** about what the deep rule
will run into, with what would settle it.

# Panel 170 — brief for the historian

Read `docs/panel/170-briefs/00-shared.md` first. Advisory, no veto. **Verify
every date, count and claim by web search**; where a search returns nothing, say
so and name the vocabulary you searched.

## What you have established across two sittings

At 167: no language enforces FOREIGN retention statically, across ten
ecosystems, and the static enforcement that exists is always caller-side. At
169: 068 is exclusivity, which ships caller-side in five languages and was never
withdrawn; every one of them **exempts raw pointers by name**; and Java's FFM
enforces the caller-side half at run time and refuses the foreign half in its own
javadoc. **A correction the coordinator owes you**: your FFM mapping was carried
too far in that synthesis and is corrected under it — FFM's exception fires on
access after an arena closes, which is not defect 068's shape.

## The question that is yours

The proposal is a **declaration-site mark** saying a C parameter retains what it
is handed, with the compiler refusing a lend there and admitting a copy or a
C-owned buffer instead.

1. **Who shipped a mark like this?** Candidates to verify rather than assume:
   Swift's `@escaping` (which is exactly *this callee keeps it*, and is
   compile-time), C#'s `[UnscopedRef]` and `scoped`, Rust's `'static` bound on a
   spawned closure, Objective-C ARC's `__attribute__((ns_consumed))` and
   `NS_RETURNS_RETAINED`, Java FFM's `Linker.Option` set, MPI's `ASYNCHRONOUS`,
   and C's own `[[clang::lifetimebound]]` and `noescape`. **Which of these is on
   the CALLEE's declaration and enforced at the CALLER?** That is the shape here.
2. **`@escaping` is the sharpest and is yours to check.** Swift made non-escaping
   the DEFAULT and escaping the marked case, reversing it in SE-0103 (2016). Why?
   What did the reversal buy, what did it cost, and is the default the lesson
   rather than the mark?
3. **The unmarked case.** Every one of these marks is the author's word and none
   audits it. Find a language that says so out loud in its own documentation, and
   quote it — that sentence is what this sitting needs to decide whether the
   asymmetry is acceptable or fatal.
4. **The negative:** has any language shipped a retention mark on a foreign
   parameter and WITHDRAWN it?

Report to `docs/panel/170-reports/historian.md` if you can write files; otherwise
return the full text and the coordinator writes it out.

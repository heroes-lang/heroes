# Panel 147 — historian

Read `00-shared.md` first. This brief adds what only you are asked for.

You judge **precedent**, and you are advisory: **no veto**. Your seat is the
most hallucination-prone in this panel and the project says so in writing.
**Unsourced precedent is inadmissible.** Every date, version, line count and
quotation must come from a source you actually fetched, with the URL in your
report. A claim you could not source is written as *unsourced* and dropped from
your conclusion.

## The question, historically

A language with **no destructors by design**, **value semantics**, **no
aliasing among its own values**, and **no standard library** — everything comes
from C — needs some answer to: *a C resource was acquired in this scope and must
be released on every path out of it, including an early return and an error
propagation.*

Three routes are on the table: the resource's TYPE names its releaser (Route
A); a scope-bound statement registers a call (Route B); refuse and say so
(Route C).

## What to find out, and the framing matters more than the list

1. **Route B's ancestry is the easy half.** Go's `defer` (2009), Swift's
   `defer`, Zig's `defer`/`errdefer`, D's `scope(exit)`. For each: **what did
   its own designers say it cost**, and — the part that is actually useful —
   **what is documented to have gone wrong with it**? Go's `defer` in a loop is
   a known footgun with a published history; find the primary sources. Zig has
   `errdefer` as a *separate* construct, which is evidence that one `defer` was
   found insufficient — find out why they split it.
2. **Route A's ancestry is the interesting half and it is easy to get wrong.**
   A type naming its own releaser, in a language WITHOUT destructors, is a
   narrower thing than RAII. Candidates worth checking: Cyclone's regions and
   its `@` qualifiers; Vale; Austral's linear types; ATS; and — the closest —
   **attribute-based cleanup in C itself**: GCC's `__attribute__((cleanup))`
   and C23's proposals. Also Objective-C ARC's `NS_RETURNS_RETAINED` family,
   which annotates C functions with ownership. **Which of these annotate a
   FOREIGN function rather than defining a native destructor?** That is the
   exact shape Route A proposes, and the honest answer may be "very few", which
   would be a finding.
3. **Route C's ancestry.** Name languages that deliberately refused a
   scope-bound release and say what they told users to do instead. C itself for
   decades. Any others?
4. **The one that would change the sitting.** Find a language that has value
   semantics, no destructors, AND a C FFI that must release handles — and say
   what it actually did. If no such language exists, say so explicitly and name
   what you searched; a measured silence is a real contribution here, and this
   project treats a silence in the literature as a ruling worth recording.

## A caution

This project has burned this seat before. Panels 143 and 144 both recorded that
the historian could not be audited because its report had no file behind it.
Your report goes to a completeness critic verbatim, so write it to be checked:
claim, source, URL, date fetched. Where a secondary source is all you have, say
*secondary*.

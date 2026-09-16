# Panel 158 — historian brief

Read `docs/panel/158-briefs/00-shared.md` first. **Verify every claim by web
search, with a source URL and the date checked.** Unsourced precedent is
inadmissible — write it down as unsourced rather than dropping it. No veto.

**Work in small steps, at most six searches, and report early.** Your seat was
killed by the watchdog at panel 155 for batching; a short sourced report beats a
long one that never arrives.

## The question, in language-design terms

A language has a fallible type constructor — `T?` — meaning *a T or an error*.
Nothing stops it nesting: a container of fallibles, read fallibly, gives `T??`.
The language's CHECKER tracks the nested type and its PARSER refuses to write
it. Three designs exist: refuse the container that produces it; **flatten**, so
two levels collapse into one; or make the nesting writable.

## What to find, in priority order

1. **Flattening is the interesting one, because it is the vetoed option here.**
   Which languages flatten a doubly-wrapped error or option, which keep it, and
   what did they say? Rust's `Option<Option<T>>` and `Result<Result<T,E>,E>` are
   kept and `flatten` is explicit. Swift's `try?` on an optional-returning
   throwing call famously produced `T??` and **SE-0230 changed it** — find that
   proposal, it is the closest match to this sitting and it has a written
   rationale. Scala's `Option.flatten`, and Haskell's `join`.
2. **The information flattening destroys.** This sitting's veto is that
   flattening collapses *absent* and *present but failed*. Has any language
   shipped that collapse and then reversed it, or shipped it and defended it?
   Swift's `Dictionary` subscript returning `Value?` and the double-optional
   problem is the same shape.
3. **A type the compiler computes and the syntax cannot write.** Is there
   precedent for that being deliberate — an internal type that is not surface —
   and what did those languages do about DIAGNOSTICS that print it? That is
   R3's question and it is the one with the least obvious answer.
4. Only if you have room: a language whose `?`-like operator peels exactly one
   level and how it explains that to a reader.

Do not recommend a design. If precedent is thin, say so.

**Return your report as your FINAL MESSAGE, in full** — you have no write tool
and the coordinator writes it to disk verbatim. Include every source, what you
could not verify, a prediction, and your condition.

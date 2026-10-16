# Panel 217, the historian's brief

Read `docs/panel/217-briefs/00-shared.md` first, whole.

## Your seat's question

**Who had a read-only lend before, and what it cost them.** Find, with web search, and mark
`verified` only what you re-read at its source: Ada's `in` parameters and the freedom the language
leaves an implementation to pass them by reference; C++'s `const T&`; Swift's `borrowing` parameter
modifier (SE-0377) and how it relates to `inout`; Rust's shared reference `&T` beside `&mut T`; Hylo's
(formerly Val's) `let` parameters, which are borrowed and immutable; and any language that gave a
second meaning to one word as F1 gives `lent`. For each: the spelling at the call site (does the
caller write a mark?), whether a callee may hand the borrowed value on, and whether the language
later regretted or amended it. Say which precedents bear on a reader of a short specification
choosing between a value and a place.

## What to return

Each precedent with its link, the date you re-read it, and `verified` or `unverified`; a verdict per
form (advisory, no veto); and the one prediction you would register.

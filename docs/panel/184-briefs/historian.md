# Panel 184, brief for the historian

Read `/Users/joseph/Temp/heroes/heroes-lang/docs/panel/184-briefs/00-shared.md`
first: the sitting, its three questions, and every measurement they rest on
(repaired on the completeness critic's first pass, 16:51 onward; question 2 is
now one predicate shared by two rules, the statement after a jump refused and
the `return` no longer demanded after a statement that always leaves, and
question 3 names three kinds of depth). You judge precedent, and
**every precedent you cite is verified by a web search in this sitting**, with
the source's address, its date or version, and the words it says; a precedent
from memory is inadmissible, and a search that found nothing is reported as
the words searched and where.

## Your questions

1. **A forgotten interpolation prefix.** Which languages or their standard
   tools tell the author, at compile or lint time, that a plain string holds
   what looks like an interpolation it will never perform, and how they decide
   (by the string's shape alone, or by the names in scope)? The coordinator
   has heard of a Ruff rule for Python, an ESLint rule for `${}` in an
   ordinary JavaScript string and a Clippy lint for Rust, and verified none of
   them. For each you find: an error or a warning, on by default or not, when
   it arrived, its false alarms as its own documentation states them, and how
   it lets a string that means its braces say so. And the other side: panel
   121's historian found that **no brace-delimited-hole language makes a bare
   `{` active in an unprefixed literal** (`docs/panel/121-the-brace-was-already-taken.md:46-50`);
   whether any language REFUSES a hole-shaped plain literal outright, as a
   compile error, rather than warning.
2. **A statement after a jump.** Which languages make a statement no path
   reaches a compile ERROR rather than a warning, since when, and how far
   their analysis reaches (after `return` alone; after an `if`/`else` whose
   branches all leave; after a call that cannot return; after `while true`).
   The coordinator believes Java does (its unreachable-statement rule) and
   that Zig refuses such code, and verified neither; C#, Swift, Go's `vet`,
   Rust, Kotlin and TypeScript each have something here to check. What did the
   languages that made it an error pay for it (a debugging habit refused, a
   rule users route around), in their own words? And the other half of the
   same predicate: how the languages that refuse an unreachable statement
   stop DEMANDING one, a `return` after a call that never returns (Java's
   `System.exit` and its definite-return rule; a bottom type, Kotlin's
   `Nothing`, Swift's `Never`, Rust's `!`; C's `_Noreturn`), with dates.
3. **How deep a source may nest.** Which language specifications or compilers
   state a limit on nesting and tell it as a diagnostic, and which abort, crash
   or grow the stack instead: C's minimum translation limits (C89 and C11,
   §5.2.4.1 in C11), clang's `-fbracket-depth` and GCC's behaviour, CPython's
   parser limit and its message, Go's parser (a nesting limit was added for a
   security report, if the coordinator remembers right), `rustc` (which grows
   its stack), `javac`. For each: the number, whether it is the same on every
   platform, the message, and the date. And panel 107's finding, that *no
   language specification states a number* for a program's run-time
   recursion (`docs/panel/107-the-number-cannot-be-uniform-the-abort-can.md:10-11`):
   whether the same holds for a SOURCE's nesting, or whether C's translation
   limits are the counter-example.

Write your report to `docs/panel/184-reports/historian.md` in the repository AS
YOU GO (the one file you write there), with, per question, `verdict`
(approve, object per route; you have no veto), `precedent` (each with its
source), `prediction` (what a precedent predicts for Heroes, falsifiable) and
`condition`. No paid run beyond your web searches. English, no em dashes.

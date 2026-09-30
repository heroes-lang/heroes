# Panel 183, brief for the historian

Read `docs/panel/183-briefs/00-shared.md` first: the two questions, both about
how far a compiler reads past an opening bracket that is never closed, and
where it reports it. Your seat is advisory (no veto), judges precedent, and
**every claim you make is verified by a web search you name**: a precedent
without a source is inadmissible.

## Your tasks

1. **How do production compilers bound an unclosed opener's reach, and where
   do they report it?** At least: CPython since 3.10 (*'(' was never
   closed*), rustc (its unclosed-delimiter recovery and the indentation it
   reads), Go (`gc` and `gofmt`), Swift, GHC's layout rule, Elm, and one C
   compiler, clang: where it reports an unclosed `(` and where it resumes
   (the critic ran Apple clang 21 and saw it report at the next token that
   cannot go on, with a note citing the `(`, and go on to a later function's
   mistake; find the source that says what clang's parser does rather than
   assume a recovery at `}`). For each: where the diagnostic lands
   (at the opener, at the point the compiler gave up, at end of file), and
   whether a later declaration or statement keyword ends the reach. Cite the
   source for each, with its date or version.
2. **Keywords as synchronisation points**: which of those compilers
   resynchronise at a keyword that begins a declaration or a statement (the
   *panic mode* recovery of the compiler texts, and its *synchronising
   tokens*), and what they measured or wrote about the cost, a false report
   from resynchronising too early.
3. **Indentation-sensitive languages**, where this language's rule lives
   (a line inside brackets goes on at any column; outside them indentation is
   structure): Python, Haskell, F#, Nim. Which of them close an open bracket
   at a line that dedents to a declaration?

Write your report to `docs/panel/183-reports/historian.md` in the repository
AS YOU GO (the one file you write there), every claim with its URL. English,
no em dashes.

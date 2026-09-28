# Panel 181, brief for the historian

Read `00-shared.md` first, in this directory
(`/Users/joseph/Temp/heroes/heroes-lang/docs/panel/181-briefs/00-shared.md`).
Your seat is advisory, on precedent; no veto, and an unsourced precedent is
inadmissible: verify every date, quotation and claim by web search and cite
the page.

## The question

Heroes has significant indentation and inserts a statement terminator by a
line's last token (Go's rule, design.md §4.15). Its compiler lets a statement
that ends with an operator run on into the next line **at the same column**,
and refuses the next line one level deeper, because a deeper line opens a
block. Its design says a long expression outside brackets is broken inside
parentheses or not at all, and deferred Nim's trailing-operator rule. What do
the languages that faced this say, and what happened to them?

Candidates to verify, not to trust:

- **Python**: explicit (backslash) and implicit (brackets) line joining in the
  Language Reference, *Lexical analysis*; what a line ending in `+` outside
  brackets does; PEP 8's advice on breaking before or after a binary operator
  and its date.
- **Nim**: the manual's rule for a line ending in an operator (and whether the
  continuation must be indented more), with the version.
- **Haskell, F#, Scala 3, Elm, CoffeeScript, Koka, Boo, Occam**: the offside
  rule and its treatment of a continuation line at the same column; Scala 3's
  *leading infix operators* rule and its date.
- **Go**: whether `a +` newline `b` at any column is accepted, from the spec's
  *Semicolons* section.
- **JavaScript** (ASI), **Kotlin**, **Swift**, **Ruby**: continuation after a
  trailing operator, and the documented hazards (`return` followed by a newline
  in JavaScript is the known one).
- Any language that accepted a same-column continuation and later refused it,
  or the reverse, and why; and any report of a real program that ran as one
  statement where a reader saw two.

## What to report

For each precedent: the language, the exact wording with its URL, what it does
with a line ending in a binary operator at the same column and at a deeper
one, and the version or date. Then what the precedents say about the sitting's
routes (`compiler-engineer.md` names them: refuse in the lexer, refuse in the
parser, admit Nim's rule one level deeper), and the shortest specification
wording any of them uses for the rule the sitting adopts.

Register a prediction the coordinator can score, with the URL that would
falsify it.

Your report is your final message; the coordinator writes it to
`docs/panel/181-reports/historian.md`. English, no em dashes.

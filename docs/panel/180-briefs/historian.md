# Panel 180, brief for the historian

Read `00-shared.md` first. Your seat is advisory, on precedent; no veto, and
an unsourced precedent is inadmissible: verify every date, quotation and
claim by web search and cite the page.

## The question

Heroes inserts a statement terminator by the line's last token, Go's rule
(design.md §4.15, panel 007), and does it inside brackets too. Its spec
currently says the opposite, Python's rule: inside brackets a line break may
fall between any two tokens. How do the languages that own each rule state
it in their specifications, and what happened where the two met?

Candidates to verify, not to trust:
- **Go**: the specification's *Semicolons* section, its list of the tokens
  after which a semicolon is inserted, and its sentence that a semicolon may
  be omitted before a closing `)` or `}`; whether Go allows a newline before
  a closing bracket without a trailing comma (`gofmt` and the compiler's
  "missing ',' before newline" error are candidates); the date and version
  the wording has had.
- **Python**: the Language Reference's *Implicit line joining* (inside
  parentheses, square brackets or curly braces an expression may be split
  over more than one physical line), and when it entered.
- **JavaScript** (ASI's restricted productions), **Kotlin** and **Swift**
  (newline handling inside and outside brackets), **Scala 3**, **Odin**, **V**,
  **Nim**: which follow Go's rule, which Python's, which a third, and how
  their specs word it.
- Any language whose spec stated one rule while its compiler did the other,
  and how that was found and repaired.

## What to report

For each precedent: the language, the exact wording with its URL, what it
permits before a closing bracket, before an operator and before a comma
inside brackets, and the version or date. Then what the precedents say about
the sitting's three routes (`compiler-engineer.md` names them), and the
shortest spec wording any of them uses for a Go-style rule inside brackets.

Register a prediction the coordinator can score (for example, that no
language using Go's rule accepts a line break before a binary operator inside
parentheses, with the URL that would falsify it).

Your report is your final message; the coordinator writes it to
`docs/panel/180-reports/historian.md`. English, no em dashes.

# Panel 185, the historian's brief

Read `00-shared.md` in this directory first; it holds the five questions and
their routes. You build nothing. Write your report as you go to
`docs/panel/185-reports/historian.md`. **Every precedent you cite is verified
by web search, with its URL and the date of the source**; an unsourced
precedent is inadmissible.

What the sitting needs, per question:
- Q1: how other FFIs reach a function-like C macro, and what each checks:
  Zig's `@cImport` and `translate-c` (macros it translates and the ones it
  gives up on), Rust's `bindgen` (function-like macros), Nim's `importc` on a
  macro, Swift's Clang importer (macros it imports), Go's cgo (`C.WEXITSTATUS`
  is refused, by common report: verify). Which of them type a macro's
  parameters, and how.
- Q2: languages whose `match` or `switch` arm takes a one-line statement or a
  block: Rust (`=>` with an expression, a block, or a statement-like
  expression), Swift's `case`, Kotlin's `when`, Scala; which take a loop on
  the arm's line.
- Q3: diverging blocks as values: Rust's `!` and a block whose last
  expression is a `return`; Kotlin's `Nothing`; Swift's `Never`; where each
  accepts a value position whose every path returns.
- Q4: any language or linter that treats `- 1` in a pattern or a case label;
  and the bulleted-list habit in code models write (markdown bullets inside
  code), if any study or tracker records it.
- Q5: the forgotten `f`: Ruff's `RUF027` (*missing-f-string-syntax*; panel 184's
  historian recorded its false-positive history: re-verify what it checks
  today), Pylint, Clippy's `format!` lints, Swift, Kotlin's string templates
  (no prefix at all), C#'s `$`; which judge by the literal's own text and which
  by the names in scope.

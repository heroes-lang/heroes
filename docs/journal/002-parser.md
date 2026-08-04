# 002 — M2: the parser, the tree, and the canonical form

Milestone M2 · 2026-08-04 · steps 1–4 · panel 013.

## 1. Goal

Tokens → syntax tree → text again. The whole specced surface: the four
entities plus `extern` and `test` (design.md §4.2, §4.18, §4.19), signatures
with generics and `@` parameters, the type grammar including the function
type panel 013 settled, the §4.14 precedence table, the three line shapes of
§4.4, `for` in both forms, `if` and `match` as expressions, patterns, `???`
as a node from the start.

Surface: `heroes parse <file> [--dump-ast]` and `heroes fmt <file>
[--write]`.

Modules born: `syntax/` (nine files: `ast/` as a directory, `cursor`,
`recover`, `decl`, `data`, `members`, `stmt`, `expr`, `primary`, `control`,
`types`, `describe`) and `printer/` (`types`, `bodies` for the dump, `fmt`,
`fmt_stmt`, `fmt_expr`). 104 crate tests, 8 golden `check/` cases through the
real binary, 5 of them adversarial.

The measurable outcome: **the 317-line acceptance program in design.md's
appendix parses with zero diagnostics, formats idempotently, and its tree is
unchanged by formatting.**

## 2. What surprised — shapes and rules

- **A registry of forbidden words is a reservation, and reservations
  collide.** Panel 013 was convened because `fn` was simultaneously a
  compile error with a prescribed fix and the syntax of the function type:
  `(fn(int) -> int)` did not lex, so *every function-typed parameter in the
  language was unwritable*, including the built-ins the spec names. Then the
  same class appeared a second time, from the other side: the acceptance
  program declared a variant case `.var`, and `var` is in the registry too.
  The rule that came out of both is one sentence — a foreign word is an
  error *everywhere*, including as a name — and its price is now measurable:
  `var case union use include class try const` cannot be identifiers.

- **A recommendation put to five judges came back overturned, on evidence.**
  The proposal recommended making `fn` a keyword legal in exactly one
  position. Four judges refused, and the decisive citation was in the
  project's own document: §4.17 lists `fn` among the autopilot mistakes the
  registry exists to catch, while §1.9 refuses familiar spelling as a
  tie-breaker and the vendored tokenisers measure `fn` and `function`
  identically. Seven ancestor languages reuse the declaration word in type
  position; none uses a second word. The panel is not a formality when its
  input is differentiated.

- **Two representations of one tree are worth their cost.** `--dump-ast`
  prints *every* parenthesis; `fmt` prints the fewest that preserve meaning.
  Comparing the two dumps is what proves the formatter did not change the
  program — a check the formatter cannot pass by agreeing with itself.

- **A `Dedent`'s span sits on the following line.** Twice this made a
  statement that ends with a block look as if it ended one line later than
  it does: once it moved a statement past a blank line, once it swallowed
  two. The parser has the same problem in the other direction — a line
  ending in `record` gets no terminator (panel 007), so a line boundary can
  be invisible in the token stream. Both were fixed by asking the *spans*
  where a line starts, not the tokens.

- **Recovery is a diagnostic feature, not politeness.** Four separate
  cascades turned one mistake into three or four messages: a bad type inside
  a signature, a depth-zero continuation, a missing body, a trailing comma.
  The general fix that ended them was not a special case but a signal — a
  statement compares the diagnostic *count* before and after itself, and if
  it produced one, it drops the rest of its line and any block hanging off
  it. Every landmark used for recovery is a bracket or a line, because those
  are the two things the language cannot lie about.

- **The formatter is where "canonical" stops being a slogan.** Three
  policies had to be chosen and written down: minimal parentheses (which
  needs associativity, the half of the precedence table nobody writes),
  breaking at 88 columns *inside brackets only* (panel 007 leaves nowhere
  else), and blank lines as content — one survives where the author put it,
  because deleting them erases the only grouping a body has. The
  load-bearing one is subtler: the blank line between a comment and the
  declaration below it decides whether that comment is documentation (§4.1),
  so a formatter that normalises it changes the program's meaning.

- **The acceptance program is a better test than any case written to pass.**
  It found both design gaps of this milestone, and it is the only test that
  covers the language rather than the parser's idea of it.

## 3. What broke and why

- `(fn(int) -> int)` did not lex → panel 013 → the function type is
  `(function(A, B) -> C)`. Cause: two spec artifacts written at different
  times, each internally consistent. Fix: 0 lines in the lexer, 5 in
  design.md, 2 in the v1 spec package (measured: +0 for the spelling, +14
  for the arity sentence the ergonomist's condition required).

- `.var` as a variant case did not lex → the appendix is amended
  (`.var` → `.variable`). Cause: the registry reserves a plausible name.

- `=> assert false` did not parse, in 8 places of the acceptance program.
  §4.7 allows an arm body to be an expression or a block, and `assert` is a
  statement. Conservative default taken (rejected, appendix rewritten with
  block arms); the question is queued with a recommendation — an arm body
  *is* a block, inline or indented, which deletes a special case instead of
  adding a form.

- The first test snapshots failed for a reason that had nothing to do with
  the parser: a Rust `"\` string continuation swallows the next line's
  leading whitespace, so every expectation whose first line was indented
  silently lost it. Recorded in `syntax/tests/mod.rs`, because it will
  happen again.

- `v: int 0` reported twice (the missing binding symbol, then the line end).
  `f(a @ n)` reported twice and produced the wrong tree. Both are now one
  diagnostic, and the second one is *recovered into the tree it meant* —
  `f(a: @n)` — with a certain fix.

- Two files crossed the 300-line rule mid-milestone and were split where
  their concerns split: `ast.rs` became a directory (types, declarations,
  bodies), and `cursor.rs` handed its recovery moves to `recover.rs`.

## 4. Left on the record

- The cascade after a *layout* error is real and unfixed: two indentation
  mistakes produce five diagnostics, because a wrong margin makes the block
  structure the parser reads untrustworthy. Whether the parser should run at
  all after a layout error is queued as a panel question, with that number
  as its evidence.
- `match` arm alignment (`=>` in a column) is not canonical: the formatter
  prints one space. The appendix's aligned arms are therefore not canonical
  form. Cheap to add later; deliberately not now.
- `.fixed` goldens — CI applying a `certain` fix and asserting the result
  compiles — remain an M3d deliverable. Three certain fixes are waiting for
  it: the reserved-word swap, the trailing comma, the misplaced `@`.

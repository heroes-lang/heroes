---
paths:
  - "spec/**"
  - "tests/harness/suite_spec.hero"
  - "tests/harness/spec_text.hero"
  - "selfhost/cli/measure.hero"
---

# The shape of the specification

Home of the rule the author gave on 2026-09-10, at M-anchored-spec: *the
specification has a written form, and the form is checked*. What each rule here
cost to learn is in `docs/contract/case-law.md`, cited as `CL-NNN`; what the
document IS and how big it may be are design.md §1.6's, reached by grep and
never restated here.

## What the document is, in one line

`spec/heroes-spec.md` is the prompt a model receives to write Heroes, the whole
language and nothing else (design.md §1.6). It is a **Report** in the tradition
of the Algol 60 and Oberon reports: thirteen numbered sections in reference
order, each rule stated once, in the section of the operation it governs.

## The thirteen sections, by number and name

| § | title | holds |
|---|---|---|
| 1 | Files and layout | modules and `use`, comments, indentation, the ASCII rule |
| 2 | Literals | integer forms, the context rule, character literals, escapes, `f"…"`; the vocabulary before the types, as every Report from Algol 60 to Oberon orders it (panel 126, the historian) |
| 3 | Types | the type table and what every value does: copies, no aliasing, by-value fields, the function type |
| 4 | Top-level declarations | the four kinds, order, constants |
| 5 | Bindings | `=` and `@`, explicit signatures, use and non-use, `_`, shadowing |
| 6 | Failure: `T?` | `ok`/`fail`, codes, the one definition of *abort*, the five operations, one example |
| 7 | Operators | the table, precedence, equality and ordering, arithmetic aborts |
| 8 | Control flow | `match`, `if`, loops |
| 9 | Functions and calls | construction, named arguments, UFCS, `@` parameters, function values, generics, recursion |
| 10 | Strings, arrays, maps | indexing, container literals, cost of `+` and `push`, maps, the index and slice aborts |
| 11 | Built-ins | the one `Built-ins:` sentence, rendering, conversions, files and the process |
| 12 | Tests and holes | `test`, `assert`, `???` |
| 13 | FFI | the group, widths, callbacks, records, constants, strings across the boundary, packages |

**A number, once assigned, never changes** (CL-069: the contract's `§1`–`§15`
were kept through a rewrite because 3280 citations named them and no instrument
verifies a section number). A new section goes at the end with the next number;
one that must stand in the middle is a panel question, because every citation
after it would move. A section is never renamed; a title that must change is a
panel question for the same reason.

## Where a rule lives

- **Every rule has exactly one home, and the home is the section of the
  operation it governs**, not the section where a reader first meets it. `/`
  truncating toward zero is an operator fact and lives in § 7, though a reader
  meets division in an example earlier; `range` excluding `to` is a built-in
  fact and lives in § 11, though loops are § 8. Where a reader would look first,
  a pointer in ASCII, *(section 11)*, is allowed and is priced like any word.
- **The definition of *abort* is in § 6 and nowhere else.** Each site that
  aborts says so beside its operation, and **no section lists every abort**:
  panel 087's warden vetoed a closed list and falsified it with a running
  program, and the runtime has more abort sites than the document names.
- **A name the compiler offers appears once in the `Built-ins:` sentence** of
  § 11, which is one sentence ending at its own full stop, whose last backticked
  fragment is `range`, and whose prose about a name stands inside parentheses,
  because `tests/harness/spec_text.hero` reads the names out of that sentence
  and strips the parentheses first. A second fact about a built-in goes in the
  prose after the sentence, not in a second list.
- **Merging beats appending** (panel 122: three drafts measured, the merged one
  cheapest). A rule that joins a sentence already there costs less than a new
  sentence, and a document whose rules each have one home stays mergeable.

## What must stay exactly so, and the instrument that says so

The checks named below are the ones `tests/harness/suite_spec.hero` runs, each
reported as `spec/<name>`; they are written here without that prefix because
`records/citations` reads `spec/<anything>` as a file under `spec/`.

| the fact | the instrument |
|---|---|
| the thirteen headings, `## N. Title`, in order, numbered from 1 with no gap | the `shape` check |
| every `spec § N` and `§ <Title>` cited in `selfhost/` and `tests/` names a section that exists | the `anchors` check |
| the first fenced block after `## 13. FFI` is the sqlite example, and it runs | the spec's-example check in `tests/harness/suite_special.hero`, which finds the heading by title through `spec_text.section_named`, number or no number |
| the `Built-ins:` sentence, one sentence, ending at `range`, at least ten names | the `offered` check |
| every name the compiler reserves appears in a code span; no word the lexer refuses appears in one | the `named` and `rejected` checks |
| the characters above ASCII are `·` `—` `…` `→` and no other | the `inventory` check |
| the `test "3-4-5 triangle"` block, byte for byte | `tests/golden/run/spec-the-documents-own-example.hero`, a hand copy |
| the heading `## 2. Types` and its table, first column backticked, at least ten rows | `site/src/lib/tables.ts`, which colours the site's type words from it |
| the token count and its digest | the `budget`, `recorded` and `ledger` checks, and design.md §1.6's payment rule |

## How to cite the specification

`spec § 7` or `spec § 7 Operators`, never `spec:NNN`. **A line number is the one
citation shape no instrument can see** (CL-037), and the re-shaping of
2026-09-10 moved every line: the 678 line-number citations that records carried
that day stay true at their date and read against the layout at commit
`834d804f`, which is why they were not rewritten (CLAUDE.md §14).

## What was refused, and by whom, so it is not proposed again as new

An EBNF or grammar appendix (author decision 2026-08-12, `docs/work/DONE.md`;
design.md Part 7 item 16: *an appendix is spec tokens spent on a grammar a
reader would then have to reconcile with the prose*). Type signatures for the
built-ins (panel 120: *the spec buys prose and not signatures*, because a
signature cannot say which half `filter` keeps). A preamble about the budget
(removed in v1 as a 96-token named removal). A closed list of aborts (panel
087). A grammar or pseudo-code in a fence, because the `rejected` check reads
every fence as Heroes.

## How a change to the document is made

design.md §1.6 is the payment rule and this file does not restate it: an
addition owes a named removal or a registered prediction naming an instrument
that exists. What this file adds is the order of work, learned at
M-anchored-spec: **instruments first** (widen a check before the text moves, so
the text is the only thing that changes); **then the move, measured alone**;
**then each merge and each addition, measured alone** (`heroes measure <draft>`
counts any file offline); **then the real count** (`heroes measure
spec/heroes-spec.md --refresh`, whose output is pasted into
`tests/harness/suite_spec.hero` and `selfhost/cli/measure.hero` in one commit,
with the ledger's row). A change to a fence is compiled before it is written:
an example in the one document a reader is told to trust is a claim, and an
unexecuted claim expires in silence.

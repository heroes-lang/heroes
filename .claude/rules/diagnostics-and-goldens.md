---
paths:
  - "tests/golden/**"
  - "tests/harness/**"
  - "selfhost/print/**"
  - "selfhost/check/**"
  - "selfhost/resolve/**"
  - "selfhost/mutate/**"
---

# Diagnostics and goldens

Home of CLAUDE.md § 8 and § 9 since 2026-09-07. What each rule cost to learn is
in `docs/contract/case-law.md`, cited as `CL-NNN`.

## Errors are a deliverable

Every `Diagnostic` carries `Fix`es tagged `certain` or `guess`, and only
`certain` is machine-applicable. An error carries everything needed to fix the
program without opening another file (design.md §4.17).

**A `certain` fix repairs the defect the diagnostic names. A fix that leaves the
defect standing is a `guess`, however well it compiles** (`/decide` 2026-09-08,
on the compiler seat's finding at panel 118). design.md §4.17 answers the
inverse case only — a fix that *changes* meaning, "change save's signature to
accept a str id" — and panel 071 answers the same inverse. This is the case
where meaning is preserved **and so is the bug**: `.is_err()` on a discarded
fallible is behaviour-identical and keeps the swallow, so certifying it would
make `heroes check --apply` automate defeating the rule M-discard-refusal
landed. **Compiling is not the bar**: `_ = xs.push(4)` compiles at exit 0 and
loses the element, which is why panel 071 made that fix a `guess` whenever a
receiver is in hand.

The discipline is a writer's and a reviewer's, and that is stated rather than
dressed as an instrument: no check can decide "repairs the defect" in general,
and panel 118 already recorded a prediction whose instrument could not exist.
What IS mechanical is the weaker half below — CI applies the fix and the result
must compile. Read at all **23** sites that can ship a `certain` fix (measured
2026-09-08: 53 `Fix(` sites in `selfhost/`, 22 unconditional, one conditional in
`grammar_expr.hero`, one forwarder in `lexer.hero`); none violates the rule, and
the two nearest the line obey it by construction — `value_errors.hero`'s
`discarded_value` de-certifies on a fallible and downgrades on a receiver, and
`resolve/errors.hero`'s rename-to-`_` fires for a loop variable and a match
payload and never for a binding.

Golden convention: `x.hero` plus `x.expected`, plus `x.fixed` where a certain
fix exists, and CI asserts the applied fix compiles.

## The goldens

`UPDATE_GOLDEN=1` **does not exist and is not implemented**, and it is
forbidden outright in `tests/golden/check/` and `tests/golden/ir/`. The
contract's hard stops carry this one.

The assistant writes all cases. Each milestone's five adversarial cases stay
marked `# UNVERIFIED — pending debrief` until the author reads them in
`/learn`; bulk regression cases are labelled as such. That marker keeps its
exact wording, because it is in 39 files (measured 2026-09-07) and a record is
not rewritten.

**Every diagnostic is annotated in the source that provokes it**, `#~ <code>`
for this line and `#~v <code>` for the next, *in addition to* the `.expected`
snapshot. That is rustc's rule and rustc's reason: the redundancy exists
because snapshots are auto-generated and absorb mistakes, and because the
annotation shows where the span points without opening a second file. It is
what turns the convention into an invariant: a regenerator can rewrite
`x.expected`, but it cannot invent an annotation in `x.hero`.

A **fixed defect gets a case named after it**, carrying symptom, cause and date
(Go's `test/fixedbugs`). Every **verifier check has a test that makes it fire**
(LLVM's `test/Verifier`). An invariant that must hold on every accepted program
is asserted over `heroes mutate`'s corpus rather than over cases somebody
thought of (`llvm-opt-fuzzer`'s `verifyModule`).

## A new surface form lands in every tool that reads the language

A form is not landed when the parser accepts it (CL-036). The tools that
re-write or re-print a program each hold their own copy of what the language
is, and one that has not learned the new form does not error, it **drops it**.
design.md §4.15 is what makes the formatter the worst of them: the canonical
form exists so that any textual difference between two versions is semantic, so
a formatter that quietly normalises a form away is the one instrument here whose
failure makes every diff untrustworthy. And the guard that watches the
formatter compares two **renderings**, so it can only see what the renderer
carries: it agreed with itself while the formatter deleted a word.

The list to walk, and each one is owed a test rather than a reading:

- **the formatter**, `selfhost/print/fmt.hero`. Its test is a round trip,
  `format(parse(text)) == text`, for the new form and for the shape beside it.
- **every `--dump-<stage>` printer**, `selfhost/print/dump.hero` and the IR and
  scope printers. One line of expected output each.
- **the formatter's own self-check**, which is the item the list's enforcement
  rests on: `selfhost/cli/syntax_cmds.hero` hands it the exact pair the 2026-09-02
  defect produced and asserts it refuses.
- **`heroes mutate`**: a form it cannot re-print is a form it silently declines
  to mutate, so the rate flatters itself.
- **the diagnostics that quote a program back**, the `Fix` replacements above
  all, because a `certain` fix built from the wrong half of a new form is
  machine-applied into a program that does not parse.
- **`heroes measure`**, where the form has spec text.

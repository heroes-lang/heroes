# Panel 180, brief for the spec-warden

Read `00-shared.md` first. Your seat judges design.md §1.2's cost formula,
§1.6's spec budget and Principle 0's burden of proof, with a veto on a budget
breach. Your directory is `/Users/joseph/Temp/heroes-recovery-2026-09-26/panel-180/spec-warden/`.

## What you decide

1. **The price of each wording.** The candidates are the three sentences in
   `llm-ergonomist.md` (P is today's text at `spec/heroes-spec.md:11-12`; Q
   states the compiler as it is, exceptions included; R states a uniform rule
   the compiler would be moved to). In your own copy, put each in place of
   lines 11-12 and run `./heroes measure spec/heroes-spec.md`: record the
   vendored rows for each, and the delta from today's 6693. The `real` row
   needs `--refresh` and the author's key; the coordinator will run it on the
   adopted wording and on today's text, so price on the vendored instrument
   and say that you did.
2. **Whether the grammar owes a change too.** The productions that write
   NEWLINE inside brackets are `Sep = "," | NEWLINE` (`grep -n 'Sep'
   spec/heroes-spec.md`) and none else inside brackets; `Args` and the group
   write no NEWLINE though the compiler accepts one before their closer
   (`00-shared.md`'s table). Say whether the sentence alone can carry the
   rule, or whether a production must say it, and price that too.
3. **Principle 0's burden.** A false sentence is a defect regardless of
   price, so the question is not whether to repair but which repair: say
   whether route (b) or (c) in `compiler-engineer.md` would have to show a
   measured design.md Part 11 effect or a compiler need to enter, or whether
   making the compiler agree with its own spec is a repair and owes neither.
4. **Where the rule's instrument lives.** The spec's sentences about the
   lexer are checked today by which suite, if any (`grep -n` the harness for
   the spec's § 0 text, for example in `tests/harness/suite_spec.hero` or
   `suite_grammar.hero`)? Say what instrument would have caught this
   sentence on the day it entered (`e497646a`, 2026-09-12), and whether the
   sitting should add it.

## Prediction

Register one falsifiable prediction with an instrument that exists today,
scored at this milestone's close.

Write your report to `<your directory>/report.md`. English, no em dashes.

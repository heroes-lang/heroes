# Panel 179, brief for the spec-warden

Read `00-shared.md` first. Your seat judges design.md §1.2's cost formula, §1.6's
spec budget and Principle 0's burden of proof, and you have veto power on a
budget breach.

## What you decide

1. **The spec token count.** The proposal adds no sentence to
   `spec/heroes-spec.md`: measure it rather than accept it. In your own copy,
   run `./heroes measure spec/heroes-spec.md` and record the vendored and real
   rows; then state whether any spelling of the probe (flag or subcommand)
   would owe the spec a sentence (§ 12, the tool surface, is where `fmt` is
   named: `grep -n 'fmt' spec/heroes-spec.md`), and if so what it would cost,
   priced on the vendored instrument in your copy. A `--refresh` needs the
   author's key and is not asked of you.
2. **Principle 0's burden.** The stopping rule (`.claude/rules/cli-surface.md`,
   panel 016) admits a capability only if the fixpoint invocation, the golden
   harness or the Part 11 harness must type it. Say which of the three types
   this one, and how: a row of `tests/harness/suite_surface.hero` over
   `tests/golden/surface-fixtures/comments101/` (42 `fmt` rows at `83ac68c1`,
   `grep -c 'argv: "fmt'`), the net's own `heroes test tests/harness/main.hero`,
   or CI. If none must type it, say what would have to be true for it to enter.
3. **The cost the reader pays.** `heroes --help` is the surface's index. Count
   its lines and its tokens today (`./heroes --help | wc -l`, and
   `./heroes measure` over the saved help text) and what one more flag or one
   more verb adds; judge it under §1.2, which counts what a reader must hold.
4. **The home of the probe's own results.** A probe that runs over the whole
   tree costs hours; a probe over the 38 fixtures costs seconds. Say which
   belongs in the net gate and which in a nightly or a `--all` form, and what
   the record must state for the gate to stay honest (a floor on the variant
   count, so a generator that silently produces fewer variants goes red).

## Prediction

Register one falsifiable prediction with an instrument that exists today, for
example the spec's real token count after the landing equals today's, or the
help text grows by exactly N lines; say when it is scored.

## Your directory

`<scratchpad>/179-spec-warden/`, copied from
`git -C /Users/joseph/Temp/heroes-lane-gm archive 83ac68c1`. Build your own
compiler from the seed there. Write your report to
`<scratchpad>/179-spec-warden/report.md` and a copy to
`/Users/joseph/Temp/heroes-recovery-2026-09-26/panel-179/spec-warden.md`.
English, no em dashes.

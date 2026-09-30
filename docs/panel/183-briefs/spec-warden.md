# Panel 183, brief for the spec-warden

Read `docs/panel/183-briefs/00-shared.md` first. **Your directory** is
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/183-spec-warden/`:
copy `171e8c45` into it as 00-shared.md says and build its compiler there;
never read or run in the repository's working tree.

## The measured count, at `171e8c45`

`./heroes measure spec/heroes-spec.md`, run by the coordinator while this
brief was written, on the trunk's compiler (built from the same seed):
`claude-legacy` 6716, `cl100k_base` 6838, `maximum` 6838, spread 122, and
`real` **9060** (`claude-opus-5, 2026-09-28 — the binding number`); headroom
1180 against the 10240 ceiling, the FFI floor mortgaging 60 of it, so 9120
is what is measured against the ceiling. The spec last changed at
`4c453f5a`, 2026-09-28 08:22 (`git log -1 -- spec/heroes-spec.md`), so the
`real` row's date is the spec's own. `grep -n -i "unclosed\|never closed"
spec/heroes-spec.md` returns nothing: the spec says nothing about an opener
that is never closed, today or before `41807577`.

## Your tasks

1. **Is a spec sentence owed?** Neither question changes what compiles (the
   compiler-engineer settles whether that is true). The spec is the language
   as a reader gets it; where a compiler reports a mistake is a diagnostic's
   placement. Say whether a reader writing this language needs a sentence
   about where the reach of an unclosed opener ends, with the reader's
   mistake that the sentence would prevent, or none. If one is owed, write it,
   price it on the reader's tokeniser (`heroes measure --refresh` needs the
   repository's `.env`; `. /Users/joseph/Temp/heroes/heroes-lang/.env` in
   your shell, and print `${#ANTHROPIC_API_KEY}`, never the value), and name
   the removal or the falsifiable prediction that pays for it (panel 012).
2. **Principle 0's burden** (CLAUDE.md § 2): the two rules change messages,
   not the language; say whether that burden applies to them at all, and if it
   does, whether a measured Part 11 effect meets it. The coordinator's
   recovery instrument measured, over 13,594 mistakes planted in 641 programs
   that a parse-stage second mistake goes unreported behind a `)` or `]` left
   out in 28 of 164 pairs at `c85bccb8` and in 8 of 164 at `e5cc73eb` (after
   batch 2), behind a string left open in 41 of 170 and then 1 of 170, and
   behind a wrong closer in 68 of 159 at both; the files are
   `scratchpad/instrument/baseline-c85bccb8/report.md` and
   `scratchpad/instrument/run-e5cc73eb/report.md`, § Pairs, the column *as the
   first, parse-stage seconds: HIDDEN / pairs* (read by the coordinator at
   05:02 and 05:30). Read them there, and say whether they bear on (b).
3. **design.md's sentence**: the words that replace §4.15's *reported at end
   of file*, as short as the rule allows and true of every program.

Verdict, condition and prediction, as your seat gives them. Write your report
to `docs/panel/183-reports/spec-warden.md` in the repository AS YOU GO (the
one file you write there). English, no em dashes.

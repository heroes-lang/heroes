# Panel 217, the spec-warden's brief

Read `docs/panel/217-briefs/00-shared.md` first, whole.

## Your seat's question

**What the form costs the specification, and draft it.** Start from `m-g-at`
(`docs/panel/215-reports/llm-ergonomist-work-2/heroes-spec-m-g-at.md`, 10,108 real on `claude-opus-5`
by the coordinator's `--refresh` of 2026-10-16 21:30, the count panel 215's spec-warden predicted).
Copy `m-g-at` over `spec/heroes-spec.md` in your own copy first: the copy at `f57af53da` differs
from it in § 9 and twice in § 13. Draft F1 as a spec diff, the fewest sentences that let a reader write task C's helper without
copying the counter and without claiming a change: `Param`'s production (line 124), § 9's `@`
sentence (lines 271-273), and § 13's `lent` (line 415) and `CParam` (line 462), merged into the
sentences already there rather than appended (`.claude/rules/spec-shape.md` § Where a rule lives).
**Price every draft on the reader's tokeniser**: apply it to `spec/heroes-spec.md` in your own copy
and run `heroes measure spec/heroes-spec.md --refresh` there (it needs `ANTHROPIC_API_KEY`: `. 
/Users/joseph/Temp/heroes/heroes-lang/.env` in your shell and print `${#ANTHROPIC_API_KEY}`, never
the value; a token count is the one API call this brief names, and it is not a model run). The
ceiling is 16,384 (`tests/harness/suite_spec.hero`); design.md §1.6's payment rule binds the
addition. **Put your chosen draft, whole, at `<scratchpad>/217-spec-warden/heroes-spec-f1.md` as
soon as it exists, and say so in your report**: the blind readers' F1 arm reads it.

## What to return

`verdict` per form, `section` (design.md §1.6, §1.2, Principle 0), the real count before and after
each draft, `prediction` (the readers' numbers you expect on the F1 arm), `condition`.

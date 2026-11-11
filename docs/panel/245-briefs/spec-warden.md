# Panel 245, spec-warden

Read `00-shared.md` first. Your worktree `.claude/worktrees/245-spec-warden` (detached at `a4b89740b`);
build its compiler from the seed in two stages; measure with `heroes measure`, vendored counts only (no
`--refresh`, a paid call), every count a lower bound said so. For each question and route: whether the
specification (§ 13 on, `grep -n '^## 13' spec/heroes-spec.md`) must state it for a reader holding it
alone to write a correct program or predict a refusal, the sentence and its vendored price in place,
the plausible mistake it prevents and the error the compiler gives that mistake today (run), and the
named removal or registered prediction that would pay for it (design.md §1.6). Also: whether each
route keeps or breaks a sentence the specification states today, and which design.md sentences
(§4.19, Part 7 row 10, panel 208's R1 as design.md carries it) each route makes false, quoted with
their lines.

## After the critic's first pass

Your base is `7fb6868b2` (00-shared.md § The base). Quote design.md's Part 7 row 10 as corrected there (`grep -n 'Corrected the same day' docs/design.md`), panel 208's R1 outcome against its mechanism (`docs/panel/208-*.md:93-97`), §4.19's reach paragraph as R2 and R3 amended it, and design.md:3125-3133 on `LIBRARY_PATH`; for each route of each question, which of them it makes false.

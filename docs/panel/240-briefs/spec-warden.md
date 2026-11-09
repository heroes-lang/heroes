# Panel 240: spec-warden

Read `00-shared.md` beside this file first. Your worktree:
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/240-spec-warden/`,
detached at the base; build its compiler from the seed for `heroes measure`.

**Your question is the indicator** (design.md §1.2, §1.6). With the spec at
**8325** vendored and **10,900** real (2026-11-08):

1. Write the § 13 diff for each candidate (`pkgconfig`, `pkgconf`, `system`,
   `library`, the mark kept as `package` with only the prose changed, the mark
   folded into `link` with a qualifier, and any other you find better) into a copy of the spec in your
   worktree, and measure each with `./heroes measure` there: the vendored
   maximum, before and after. The `real` row needs `--refresh`, a paid call the
   author excluded tonight; say so, and mark your verdict provisional on it.
2. Count the *concepts* a reader of the spec meets for the word *package*
   before and after (panel 235's spec-warden counted a sixteenth concept for the
   collision, its § 8 in `docs/panel/235-reports/spec-warden.md`); read that
   report and say whether the rename removes it.
3. Read the spec whole for every other place a word names two things among
   *module*, *package*, *library*, *catalogue*, *index*; list them with their
   lines, or say none was found and what you searched for.
4. The ledger: `docs/measurements/010-spec-budget-ledger.md`. Say what row this
   change would write and whether it needs a named removal or a prediction
   (panel 012).

Do not commit, do not push. No paid run.

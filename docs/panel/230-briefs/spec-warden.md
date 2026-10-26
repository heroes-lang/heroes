# Panel 230, the spec-warden's brief

Read `00-shared.md` beside this file first; its last section says what the critic's first pass
changed. You judge the indicator (design.md §1.2's cost, §1.6's budget and payment rule) and
Principle 0's burden of proof (CLAUDE.md § 2). You hold a veto on a budget breach.

**Where you work**: `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/230-spec-warden`,
detached at `595d92d88`; its compiler built there. Scratch:
`.claude/worktrees/230-spec-warden-scratch/`. Your report is
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/panic-location/docs/panel/230-reports/spec-warden.md`,
**written as you go, from your first finding**. No file outside the repository's root, no credential
printed. The token-counting endpoint `heroes measure --refresh` calls is named for you; nothing else
that costs.

**The instrument** (`.claude/rules/spec-shape.md` § How a change to the document is made): the real
count binds, and `--refresh` refuses every path but `spec/heroes-spec.md` and `CLAUDE.md`, so a draft
is priced by **applying it to the real path in your worktree, `heroes-next measure
spec/heroes-spec.md --refresh`, and reverting**, or written as a lower bound in those words.
`--refresh` needs `ANTHROPIC_API_KEY`: `set -a; . /Users/joseph/Temp/heroes/heroes-lang/.env; set +a`
in the shell that runs it, its length printed if you must know it loaded (`${#ANTHROPIC_API_KEY}`),
never its value. Without it the number is *unrun*, never the vendored one promoted.

**What you are asked:**

1. **Each sentence priced alone and together**, on the reader's tokeniser, against 10,297 (re-measure
   the unchanged file first, so the baseline is yours and today's): § 1, § 6, § 12 as drafted, and
   **the sentences the proposal also owes** (finding 36 and defect 2224): what § 13's *aborts when
   `main` returns* becomes under M1, and what § 11's `exit(code: i64)` says of a code outside 0 to
   255. And the cheapest wording of each route's sentence: § 6 under L1 alone is false for the third
   kind of abort (`00-shared.md`), so its wording narrows, names the walk, or is not written at all;
   **zero tokens is a route**, the place being behaviour the reader of a run sees rather than a rule
   a program is written against.
2. **What pays for each**, design.md §1.6: a named removal, or a registered prediction naming **an
   instrument that exists**. The critic found that none of the instruments the first draft of this
   brief named exists as named (findings 26, 32): no arm of Part 11's instrument varies what an abort
   says, the held-out run is not the visible suite's 90 cases, its tasks state every error status in
   prose (so it never measures a default), and its grader ignores stderr. So say what instrument a
   prediction can name: the blind seat's own measurement (84 sessions, `blind/README.md`, scored
   before its key is read), as panel 217's payment was its sitting's measurement; a new arm or a
   replay of measurement 052's generations, which do not exist and would have to be built first; or
   a named removal.
3. **Principle 0 for M1, M5, M6 and M7**: each is a new accepted shape or a new name. The compiler,
   the closure list's program, writes its errors on stderr through `hero_write_err` from 88 code
   lines and ends by `exit(code:)` with 0, 2 and other codes (`selfhost/main.hero:99-225`): it would
   use M5, not M1. So M1's ticket is the thesis alone; **panel 159** refused the shape at its R3 (*the
   sentence is the diagnostic's, `main` PRODUCES NOTHING*, ratified by delegation, not by reading),
   and this sitting reopens it: say what a reopening owes, and whether what changed since 2026-09-16
   (`00-shared.md` § The history) is enough.
4. **The consolidation block**: how much of its 1,500 stands spent (the ledger,
   `docs/measurements/010-spec-budget-ledger.md:228-232`, read forward; row 116 is a -2 *with no
   payment owed*, so whether its 2 return to the block is an inference), and **whether the block
   still binds** now that M-thesis-harness's held-out run has happened (design.md binds it *before
   the thesis harness has run*), or the next raise's rule has taken over.
5. **Every sentence's other readings**: what a reader of the draft could take it to mean that the
   compiler would not do (`written as this document writes a value` for a `str` holding a quote, a
   line feed (defect 2225), a map, a `ptr`; *the file, line and function* where the place line also
   writes a column), and the cheapest wording that closes each.

**Your output**: `verdict` per sentence, the real counts with their commands, the payment for each,
`prediction` with its instrument and the milestone at which it is scored, `condition`.

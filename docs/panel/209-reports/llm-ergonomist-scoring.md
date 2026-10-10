# Panel 209, the blind seat scored: 24 sessions, 4 readings per cell, all concordant

Written by the coordinator on 2026-10-10 at 17:02 (`date`), from the 24
`report.md` files under `.claude/worktrees/scratch-b15/209-blind/` (each
copied beside this file as `llm-ergonomist-<folder>.md` with a header saying
how it ran) and from the compiler runs named below. The method is
`docs/panel/209-briefs/llm-ergonomist.md`; the briefs are
`docs/panel/209-briefs/blind/`.

## How the 24 were run, and what reached their context

Four waves of six, each wave the three tasks in variants A (the trunk's spec
at `87794631`) and B (draft B2), one fresh `claude -p` session per folder
(`run.sh`: `--restricted --safe-mode --strict-mcp-config --model
claude-opus-5-5`, Read and Write only, `--max-budget-usd 0.8`, the
environment stripped of `ANTHROPIC_API_KEY`, the CLI logged in as
`authMethod: claude.ai`, `subscriptionType: team`). Every session exit 0, 4 to
6 turns, 56 to 70 s a wave; the CLI's own estimates sum to **5.69 USD at list
price** over the 24 (`python3` over the `run.json` files), against the
author's ten.

**Waves 1 and 2** (folders `t?-?` and `t?-?2`) ran from folders inside the
repository's tree, and all twelve `context` sections say the harness placed
into their context the project's git status: the branch, recent commit
subjects naming panels and defects, the untracked `docs/panel/209-briefs/`
and `209-reports/`, the user's email and the working directory. All twelve
say none of it informed the reading, and none of that material names the
language's rules or this sitting's question; by panel 183's rule a yes still
voids a reading until it is re-run clean, so **waves 3 and 4** (folders
`t?-?-c1` and `t?-?-c2`) ran from folders made each its own git repository
(`git init`, one commit `inputs` holding only the inputs, byte-identical to
wave 1's by `cmp`). Their twelve `context` sections say the git status they
saw listed only their own `run.err` and `run.json` and the commit `inputs`,
plus the working directory path and the user's git name and email: the
project no longer reaches the session; what remains is the folder's path.
The 24 readings agree cell for cell, so the leak of waves 1 and 2 moved
nothing; the clean twelve are the readings the sitting scores, and the first
twelve are kept as a second measurement of the same thing.

## The cells

| task | variant | readings | the declaration every reading wrote or read | verdict and repair | compiled |
|---|---|---|---|---|---|
| t1 write `longest` | A | 4 of 4 | `best: str @ words[0]`, then `best @ w` | | exit 0, prints `abcd` (the trunk's `./heroes`, `209-coordinator/blind-check/t1-A.hero`) |
| t1 write `longest` | B | 4 of 4 | `best @= words[0]`, no annotation, then `best @ w` | | B2's form exactly; the compiler-engineer's prototype is its judge |
| t2 write `tally` | A | 4 of 4 | `counts: {str: i64} @ {}`, `counts[w] @ counts[w].default(0) + 1`, `t = tally(...)` | | exit 0, prints `2` (`blind-check/t2-A.hero`) |
| t2 write `tally` | B | 4 of 4 | `counts: {str: i64} @= {}` (the annotation kept, the empty literal's rule found), the same mutation, `t = tally(...)` with `=` because nothing re-binds `t` | | B2's form exactly |
| t3 read | A | 4 of 4 | lines 2, 8, 10 *new name* by `: Type @`; 9 by `=`; 4 and 12 *existing name* by the bare `@` | refused on line 4 only; `limit` never re-bound read as allowed (*the specification does not call that an error*); repair `total`; prints 34 | the trunk's compiler agrees: `unknown_name` at 4:9 with the certain fix, nothing else; repaired, 34 |
| t3 read | B | 4 of 4 | lines 2, 8, 10 *new name* by `@=`; 9 by `=`; 4 and 12 *existing name* by the bare `@` | refused on lines 4, 8 **and 2**; repair `total` and `limit = 10`; prints 34 | the expected reading of B2, plus the cascade below |

Every t3 reader, under both variants, said the two kinds are told apart on
the line alone by the token after the leading name, and that whether the
name of a bare-`@` line exists is a question for the rest of the scope,
which is the question line 4 fails.

## What the readings found that the briefs did not ask

- **The cascade under R1b** (all four t3-B readers): with `total @= 0` on
  line 2 and the typo `totl @ total + x` on line 4, nothing re-binds
  `total`, so B2's sentence refuses line 2 as well as line 4: two messages
  for one mistake, the shape design.md §4.17 and the `adjacent` class refuse.
  Three of the four called line 2 a consequence of line 4 and said the one
  repair clears both; one listed line 2 first. Sent to the compiler-engineer
  at 16:56 for its R1b prototype to price.
- **`@=` was read as required, not merely allowed** (t1-B, t2-B readers):
  the never-re-bound sentence made the writers choose `=` for `t` and `@=`
  for the cells on purpose, and one t2-B reader asked whether an element
  write (`counts[w] @ ...`) counts as re-binding the cell, answering yes from
  *or a field or element inside one*.
- **No B writer annotated a cell the value typed** (`best @= words[0]`
  every time), and each named `best: str @= words[0]` as the other choice
  the grammar allows: the optional annotation was found and declined where
  the value said the type.
- **Under A, no reader needed §4.4's reason**: every t3-A reader derived
  *declaration* from `: Type @` and *mutation* from the bare `@` without
  being told why the type is there.
- **Each reader's least-sure line was the same under both variants**: the
  `if` with no `else` whose body is a mutation (t1), the self-referencing
  map update on one line (t2), and whether `sum` is a reserved name (t3);
  none concerned the binding forms.

## What this scores

The claim the blind seat measures is each other seat's about readers: that
`@=` is found and applied on the first try, that the one-character distance
from `@` is or is not read wrong, and that the optional annotation is kept
where the value cannot say the type. Read on 24 sessions: found and applied
4 of 4 in every B cell; read wrong 0 of 12 B readings; the annotation kept 4
of 4 where the literal was empty and declined 4 of 4 where the value typed
the cell. What it does not measure: a program longer than nine lines, a cell
born narrower than a later write, and R10 (the symbol with the type kept),
which was not a variant.

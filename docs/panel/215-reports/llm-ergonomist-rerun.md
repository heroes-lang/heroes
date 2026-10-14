# Panel 215, the paid reader re-run on the repaired specification

Run by the coordinator on 2026-10-14 from 12:41 to 12:43 (`llm-ergonomist-work-2/run.log`), on the
author's budget of that morning, after the critic's pass 2 (`critic-reports.md` § 1) found that the
first run's union task missed panel 194's registered first-try threshold for a cause in the
specification's text, and the spec-warden drafted the repair (`spec-warden-drafts.md`). Everything
below is read from the ten runs' outputs in `llm-ergonomist-work-2/` (each reader's three programs,
notes and run record, the scores, the launcher), every program built and run on this Mac with the
compiler of `main` at `3f77bd3a3` (whose compiler differs from the sitting's `7e5c02048` by comments
alone: `a0dd1aa55`'s ORDER marks).

## How it was run

- **The specification**: the spec-warden's draft `m-g-at` applied to the sitting's
  `spec/heroes-spec.md` by its own script (`spec-warden-work/q3/drafts_q3.py`), sha256
  `ac4ff3e263629118...`, kept as `llm-ergonomist-work-2/heroes-spec-m-g-at.md`. Its price on the
  reader's tokeniser, taken by the coordinator with `heroes measure spec/heroes-spec.md --refresh`
  in a scratch copy at `7e5c02048`: **10,108 real** against today's **10,048** (+60), legacy 7568,
  cl100k 7703; the draft without the § 9 rewrite (`m-g`) reads 10,120 (+72).
- **The tasks and the key**: the spec-warden's kit (`spec-warden-work/q3/kit/`): A2 as in the first
  run, word for word; U, panel 178's Darwin `utsname` task as panel 194 registered it; C, a new task
  whose words never require a copy of the lock. Judged by the kit's `judge.sh` (build, run under a
  20 s timeout, whole output compared), and every `c.hero` read.
- **The launcher**: the first run's (`run_one.sh`), unchanged, so the two runs compare: ten
  `claude -p` sessions, `--bare --restricted --strict-mcp-config --tools "Read,Write"`, `--max-budget-usd
  0.7`, each in its own folder `.claude/worktrees/readers2/r<NN>/` after `git init`, five at a time.
  Models: `claude-opus-5-5` for the answer and the CLI's own `claude-haiku-4-5-20251001` call.
  As in the first run the Write tool did not reach the sessions; each reader gave its four files in
  its answer, extracted unchanged (no answer held a second block of one name or a correction).
- **Cost**: **2.2106 USD** (each run's `total_cost_usd`, 0.20 to 0.24). With the first run, **4.1950
  USD of the author's 10**. The `--refresh` counts appear in no cost the CLI reports: *unmeasured*.
- **Context**: `context: yes` from all ten, the environment's preamble alone (agent identity, the
  folder's path, the date, the tools); nine say in words that no instruction, rule, memory or
  repository status reached them, r09 names the preamble alone. The reading stands.

## The results

| task | first try correct | what it reads |
|---|---|---|
| **A2**, the union (first run: **1 of 10**) | **10 of 10** print `2` | all ten bound the record `partial`, built with `rest: zero` (`form=partial` by the judge) |
| **U**, `utsname` | **10 of 10** print `Darwin` | `u8` written by **0 of 10** (`i8[256]` by all) |
| **C**, the lock | **10 of 10** print `6`, `4`, `5` | **10 of 10 copy an initialised lock**: every reader's read-only helper is `function doubled(c: Counter) -> i64` taking the counter **by value** and copying it again into a local cell (`view @= c`, `local @= c`, `copy @= c`) to lend it to `counter_value` (read in all ten `c.hero`; the judge's `byval=h_c_doubled` in all ten) |

## What it scores

- **The spec-warden's registered prediction on `m-g-at`** (`spec-warden-drafts.md`): A2 at least 8
  of 10: **10, held**; U at least 8 of 10 with `u8` at most 2: **10 and 0, held**; C at least 8 of 10
  correct: **10, held**, and at least 3 of 10 copying an initialised lock: **10, held**.
- **Panel 194's registered thresholds** (at least 8 of 10 first try, `u8` at most 2 of 10, R1 with
  K, one task holding a union, its `utsname` task): **met on the repaired text**, on one arm. The
  first run's miss is read as the specification's: the same tasks, the same launcher, the same
  compiler, and the union task went from 1 to 10 when the two sentences changed. The arms without R1
  and without K stay unrun.
- **Q2's missing measurement** (`critic-reports.md` § 6, question 3): where copying the lock is
  natural code, **ten of ten readers copied it**, and every program printed the right answer at exit
  0, so nothing told them. Here the copy is harmless (`counter_value` reads `n` and never the lock),
  which no reader could know from the task; with a `counter_value` that locks before reading, the same
  ten programs lock a copied mutex, which POSIX makes undefined. The spec-warden's condition for a
  refusal (*at least 3 of 10 copying*) is met.

## Why they copied, in their own notes

Every reader explains the copy the same way: the task says *without changing the counter*, a
parameter written `@c` says the function changes it, and C's `const struct counter *` still needs
`@`; so a by-value parameter promises no change, and a local copy gives C its place (r01, r03, r04,
r05, r06, r07, r09 say so in nearly these words; `grep -i doubled r*/notes.md`). Several knew the
risk and avoided it elsewhere: r04 and r05 refused to *return* a counter because *POSIX says not to
copy one*, then copied it in `doubled`; r08 notes *the copy of the mutex is never locked*. **So the
copy is what the language's surface leaves a careful reader**: a Heroes function's parameter is
`Param = [ "@" ] ident ":" Type` (spec line 124, read), a place it may change or a value, and `lent`
belongs to a C function's parameter alone (line 460, *C does not keep it*, not *nothing changes
it*); so there is no way to lend a parameter's place to C without declaring that the function
changes it. A refusal of the copy (route C or B) would turn
these ten programs into an error whose fix is `@c`, the signature every reader rejected as false.

## The verdict

**Approve the repaired sentences** (`m-g-at`, +60 real) for Q3: the instrument panel 194 registered
reads met on them. **Approve a refusal of copying a record that holds a lock** for Q2 on its thesis
effect, now measured (10 of 10 wrote the copy; nothing told them). **Object to landing that refusal
alone**: the measurement shows why readers copy, and a refusal whose only fix is a signature the
reader believes false is a rewrite turn §1.2 charges; the sitting should ask whether a read-only
lend (a place C reads and the function promises not to change) is the form the readers were missing.
*That question is not priced or built by anyone here.*

## Predictions

- With a refusal of the copy and a message naming `@c` as the fix, at least 8 of 10 readers given
  the compiler reach a correct C on their second try. Scored by a re-run that shows the message.
- With a read-only lend in the specification (if the sitting admits one), at most 2 of 10 copy the
  lock on the first try. Scored by the same task C on that text.

## What was not run

The arms without R1 and without K; any reader given a compiler; a task C whose `counter_value` locks;
Linux and Windows builds of the readers' programs (this Mac only); the cost of a read-only lend.

# Panel 251, spec-warden

Read `00-shared.md` first, whole: the frozen commit (`3a5ee269117a31780ef1f96d0305452630ac4d83`), the three proposals as spec diffs
with their conservative alternatives, each question's record, Rebel's witnesses and the rules. You judge
the indicator (design.md §1.6, reached by `grep -n '^### 1.6' docs/design.md`: line 253) and Principle
0's burden (CLAUDE.md § 2): whether the specification must say anything for each question, where, at
what price, and what pays for it. Veto on a budget breach.

**Your folder** is `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/rebel-scratch/251-spec-warden/`,
made by you as `00-shared.md` § Where each seat works shows; binaries `sw-...`. **Your report** is
`.claude/worktrees/rebel-round/docs/panel/251-reports/spec-warden.md`, written as you go, in
`.claude/agents/spec-warden.md:62-70`'s structure (`verdict`, with `provisional` where a count is
vendored, `section`, `spec_token_delta`, `removal`, `needed_for_self_hosting`, `argument` of at most 120
words, `prediction`, `condition`), **one block per question**. Read `.claude/rules/spec-shape.md` in
your copy. No paid run but the refreshes named below, no credential printed, no `heroes test`.

## The count before and after, as measured

`spec/heroes-spec.md` did not move since 2026-11-15 (`git diff --quiet cc83b4d91 HEAD --
spec/heroes-spec.md`, exit 0): 492 lines, 29,407 bytes, sha256
`1d532d514831aa70234ef0eb8ad77850ebd4c2e390234f22e973a363ed757001`. By the writer's stage 2 at the frozen
commit (`sources 23c7d2a626b29fd5`), in its copy `bw/tree/` (`bw/` is
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/rebel-scratch/251-briefwriter/`),
`./heroes-next measure spec/heroes-spec.md` (`bw/measure-before.txt`):

```
  claude-legacy      8551   (64995 ranks)
  cl100k_base        8702   (100256 ranks)
  maximum            8702   a lower bound, not the reader's tokeniser
  spread              151   between the two vendored tables (1%) — the gap between two REAL tokeniser generations was 967
  real              11384   claude-opus-5, 2026-11-15 — the binding number
Headroom: 5000 against the 16384 ceiling — but the FFI floor mortgages 60 of it (panel 030 R3), so what is measured against the ceiling is 11444 and the check goes red at 16384.
```

**The vendored rows are measured; the `real` row is the pin** in `selfhost/measure/pinned.hero`
(`SPEC_REAL_TOKENS` at its line 54, `SPEC_REAL_MODEL` 57, `SPEC_REAL_TAKEN` 60, `SPEC_DIGEST` 63, `grep
-n`), printed by the command, **carried**: no `--refresh` ran.

**After**, repaired after the critic's first reading: the writer wrote each variant from the frozen spec
by `bw/tools/make_arms2.py` (every edit asserted to match once) into `bw/arms2/`, its diff beside it, and
counted each by `./heroes-next measure bw/arms2/spec-<arm>.md` (`bw/measure-arms2.txt`); the whole
proposal B also **in place**: `bw/arms2/spec-proposal-b.md` copied over the copy's
`spec/heroes-spec.md`, `./heroes-next measure spec/heroes-spec.md` (`bw/measure-after-inplace2.txt`:
`claude-legacy 8712`, `cl100k_base 8870`, `maximum 8870`, then *"STALE: the recorded count is for
583fa94c01eddbf8 and this file is 5c3ccb7723dd5a94"* and no verdict), the copy's spec restored and
`cmp`-equal to the frozen one after:

| arm | what it is | sha256, first 16 | lines | legacy | cl100k | maximum | delta |
|---|---|---|---|---|---|---|---|
| `spec-now.md` | the frozen spec | `1d532d514831aa70` | 492 | 8551 | 8702 | 8702 | 0 |
| `spec-q2-r.md` | Q2's R, merged into the lease sentence | `49cf54954dcb84e5` | 492 | 8556 | 8707 | 8707 | **+5** |
| `spec-q2-r-after-t1.md` | Q2's R as its own sentence after T1 (sitting 252's arm `t`, the same bytes) | `c011a88408c14129` | 492 | 8574 | 8725 | 8725 | +23 |
| `spec-q2-e.md` | Q2's conservative E, *"When `main` returns, the program ends, and a thread still running ends with it; the program says how many it ended."*, after T1 (sitting 252's arm `g`, the same bytes) | `92c44fc0cb999390` | 492 | 8578 | 8729 | 8729 | **+27** |
| `spec-q1-k.md` | Q1's conservative, route K's told residue | `f7484374bdec1123` | 492 | 8577 | 8728 | 8728 | +26 |
| `spec-q3-b.md` | Q3 alone, spelling B (seven built-ins) | `562b5a7980d0d36b` | 496 | 8706 | 8864 | 8864 | +162 |
| `spec-q3-m.md` | Q3 alone, spelling M (three built-ins, four operations after a dot, § 9 amended) | `0fc6600ea429ba9f` | 498 | 8727 | 8887 | 8887 | +185 |
| `spec-proposal-b.md` | Q2's R and Q3 B, Q2's words *"a thread nobody waits for"* | `9d30660e27e983a4` | 496 | 8712 | 8870 | 8870 | **+168** |
| `spec-proposal-m.md` | Q2's R and Q3 M | `5b2d48d35e0607d2` | 498 | 8733 | 8893 | 8893 | **+191** |

The arms of the first writing (`bw/arms/`, +95 and +101 for Q3) are superseded: they held a mailbox that
contradicted § 3 and a `join` that § 11 already names (`00-shared.md` § Repairs, findings 7 and 2).

**The real counts are yours to take** (the coordinator's decision of 2026-11-19 on the critic's finding
6, following panel 249's ratification, `docs/panel/249-*.md:557-558`: *"the spec-warden's `--refresh`
calls asked with it"*): **at most eight refreshes**, one each of `now` (the pin re-measured rather than
carried), `q2-r`, `q2-e`, `q1-k`, `q3-b`, `q3-m`, `proposal-b`, `proposal-m`; `q2-r-after-t1` stays
vendored. `--refresh` refuses every path but `spec/heroes-spec.md` and `CLAUDE.md`, so each arm is
copied over your copy's `spec/heroes-spec.md`, refreshed, and reverted (`cmp` against
`bw/arms2/spec-now.md` after each). Each refresh asks
`https://api.anthropic.com/v1/messages/count_tokens` (`selfhost/cli/refresh.hero:37`), which panel 252's
brief read as free on Anthropic's page (*"Token counting is free to use but subject to requests per
minute rate limits"*, carried, `p252/docs/panel/252-briefs/00-shared.md:333-339`), so each counts at 0
USD inside the sitting's 10. The key: `set -a; . /Users/joseph/Temp/heroes/heroes-lang/.env; set +a` in
the one shell that refreshes, then `echo ${#ANTHROPIC_API_KEY}`, **its length and never its value**.
Log each refresh in your report: the time, the arm, the four constants printed. **Nothing you print is
pasted into the tree.** Whether `--refresh` runs in a copy with no `.git` is unrun; if it does not, say
so and the counts stay vendored, lower bounds (`.claude/rules/spec-shape.md` § How a change to the
document is made: the two instruments disagree by about a third).

## What you are asked

1. **Q1.** The robust proposal adds no token: is spec § 13's *"A callback is a **parameter**, never a
   result or a signal handler, nor called by one"* (line 403) and *"No function of the program is a
   signal handler"* (404) the whole rule a reader needs, or does the residue route J would leave owe
   words? Price the conservative's sentence (`bw/arms2/q1-k.diff`, +26, refreshed) and say whether it is
   the right home (spec § 13 already writes *"past a field or an element it is the binding's word"* at
   402). Question 3 of 249's R3 is yours with the blind seat (`docs/panel/249-*.md:267-272`; the blind
   seat's task 3 reads the frozen spec against the K arm).
2. **Q2. Does the spec owe a sentence at all?** Principle 0: the compiler spawns and joins
   (`selfhost/main.hero:101`); does it need the rule, or does a reader (the blind seat's task 1, which
   reads R, E and the frozen spec), or does the runtime's message carry it? The spec's silence today: a
   ruling or an omission, with what you searched (`00-shared.md` § Question 2's record). **Home**: R's
   merge (+5) against its own sentence after T1 (+23), *"Merging beats appending"*
   (`.claude/rules/spec-shape.md` § Where a rule lives) against *"the home is the section of the operation
   it governs"*: both are § 13. **E's words** (+27, sitting 252's `pE`): *"the program says how many it
   ended"* where the spec names no error stream and gives a status only to `exit(code:)` (362): does E
   need words the spec lacks? Does § 11's `exit(code:)` owe a clause under R?
3. **Q3**, as repaired (`00-shared.md` § The three proposals; `bw/arms2/proposal-b.diff`,
   `proposal-m.diff`). Judge:
   - **the two contradictions the critic found and the repair's answer** (its finding 7): § 3's *"No
     aliasing exists among the values this language owns"* (98) against a mailbox whose copies reach
     one queue, answered by amending § 3's address sentence (99-101) to class `(mailbox T)` with `ptr`
     and `cstr`; and § 9's `@` rule against a `send` that changes the queue through a plain argument,
     answered by the same sentence (*"a function taking one without `@` may still change, or free, what
     it reaches"*). Is that amendment the right one, or does a mailbox owe `@m`, a move, or design.md
     7.13's queue per thread instead?
   - **the names** (finding 2): a built-in's name is taken everywhere (`error[builtin_name_taken]`, for a
     declaration, an `extern` member and a local; today's method-form built-ins too, `00-shared.md`), and
     **the spec states that rule nowhere** (`grep -n -i -E 'reserved|taken|shadow'` over it, the
     critic's: only line 167's `use` rule): B takes 124 declarations in 98 files and 30 `extern` members
     named `close`, M takes 3 and amends § 9's *"There are no methods"*. Which spelling, and does either
     owe the unstated rule a sentence?
   - **the compiler's own sized thread** (finding 8): `selfhost/main.hero:87`, `:96-97`, `:101`, `:112`
     bind `hero_thread_spawn_sized` at 268,435,456 bytes; the proposal adds `spawn_sized`. Is it owed
     under CLAUDE.md § 2's closure-list clause, and is a program that binds the old doors refused (a
     diagnostic class) or left unnamed?
   - **the home** (§ 11's prose after the `Built-ins:` sentence, against § 13); the `Built-ins:` sentence
     still one sentence ending at `range` (the `offered` check, `tests/harness/spec_text.hero`); the type
     words `thread` and `mailbox` against the `named` and `rejected` checks
     (`tests/harness/suite_spec.hero`); the new `Prefix` alternative against the `grammar` suite
     (`tests/harness/suite_grammar.hero`); the sqlite example still the first fence of § 13
     (`tests/harness/suite_special.hero`).
   - **the removal**: T1 leaves (64 words and 416 characters, split by whitespace from *"A thread runs"*
     to *"waits for its answer."*, `python3 -I`), and the deltas above are net of it; is that a named
     removal design.md §1.6 accepts, or is the rest owed a prediction? And the conservative, no change:
     price nothing, and say what Rebel's 38 refused and 9 pending rows (`00-shared.md` § Question 3) cost
     the thesis instead.
   - **defect 2582 under Q3** (finding 12): once a worker that prints is the common shape, does § 11's
     *"`print` writes its values with no separator and exactly one trailing newline"* (348-349) already
     rule that a line is written whole across threads, or does Q3 owe a sentence?
4. **The payment** (design.md §1.6, the payment rule from line 324): an addition owes a named removal or a
   registered prediction naming an instrument that exists the day it is registered. **Register yours in
   your report before the coordinator starts `run.sh`**, in terms the blind seat's counted rules score
   (`llm-ergonomist.md` § The decision rules); and say whether the consolidation block's 1,500 real tokens
   (`docs/design.md:288-296`) still binds.
5. **The door's type before `1.0.0`**: panel 232's promise covers *"the functions `hero_os.h` declares at
   1.0.0"* (`docs/panel/232-*.md:272-274`); `runtime/hero_os.h:481-482` says *"whose type does not move
   (panel 232's promise)"*; CLAUDE.md:136, *"Before `1.0.0`, no form enters to keep an older program
   compiling"*. Which binds a route that changes or removes the thread doors now?
6. **A diagnostic class each route touches** (`.claude/rules/diagnostics-and-goldens.md` in your copy):
   Q1's widened `ffi_defined_elsewhere` (the escape); Q2's runtime message moving from *"this is a
   compiler bug"* to the program's; Q3's new refusals (a value holding a `ptr` or `cstr` crossing, a type
   form in the wrong place, a binding of the old doors if refused, `builtin_name_taken` reaching seven
   new names under B). What each owes there.

## What you are asked to say

Your definition's structure per question; the measured deltas, real where refreshed and vendored where
not, each with its command; the sentence you would adopt word for word with its home; the prediction you
register and the counted rule that scores it; and every premise of these briefs you found false.

**Added 2026-11-21.** The folders under `.claude/worktrees/` this file names were never part of
the repository, git ignoring that directory, and were removed that day by the author's
instruction, meant as: *delete every worktree that is not active; where one is cited, find a
solution*. What a seat or the coordinator kept under `rebel-scratch/` for this sitting is read
from `docs/panel/251-reports/evidence.tar.xz`, its paths relative to `rebel-scratch/`; `evidence.md`
beside it says what was kept and what was left out.

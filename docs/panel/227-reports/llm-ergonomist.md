# Panel 227, llm-ergonomist: the blind experiment's result

Written by the coordinator, 2026-10-21, from the six sessions `docs/panel/227-briefs/blind/README.md`
describes (their answers whole in `blind/w1.md` to `w6.md` beside this file) and from the lane's
compiler run on each program. The seat's verdict is this result; the coordinator holds no veto on
its behalf.

## How it ran

Six `claude -p` sessions on `claude-haiku-5-5`, one per folder `build/w/1` to `6`, each given a copy
of the specification at `63c78c54b`, two tasks and a brief asking for two programs, the choice
points and the context; **2, 3 and 5 held route A's sentence, 1, 4 and 6 today's**, the variant in
no folder. The default system prompt replaced by one sentence naming the folder, `--bare`, `Read`
alone. **Every session's `context` names the system prompt and the request and nothing else**: no
project rule, memory, file list or commit (the answers' last section, quoted in each `w<N>.md`).
A first run of six under the panel skill's own command answered `context: yes`, Claude Code's
default prompt carrying the repository's git status and five commit subjects that name the trial,
and is void (the README). Cost, from each `run.json`'s `total_cost_usd`: 0.0566 USD for the six
that count, 0.1649 USD with the void run and the two probes.

## What each reader wrote, and what the compiler said

Each answer's two programs taken from its first fenced block under `program-01` and `program-02`,
written to `.claude/worktrees/thesis-drafts/227/blind-out/w<N>-0<k>.hero`, and checked with the
lane's `heroes-next check --brief` (`sources 79fe404274caacd0`); `check` without `--brief` gives the
same codes.

| session | sentence | `slice` calls | `repeat` calls | `check --brief` |
|---|---|---|---|---|
| 1 | today's | `text.slice(...)`, `line.slice(...)`, `content.slice(...)` | `repeat("=", n)`, `repeat(" ", n)` twice, `n` an `i64` | 01: 1 `type_mismatch`; 02: 2 `type_mismatch` |
| 4 | today's | `text.slice(...)` twice, `line.slice(...)` | `repeat("=", k)`, `repeat(" ", lineno)`, `i64` counts | 01: 1 `type_mismatch`; 02: 1 `type_mismatch` |
| 6 | today's | `text.slice(...)`, `body.slice(...)` | `repeat("=", k)`, `repeat(" ", num)`, `i64` counts | 01: 1 `type_mismatch`; 02: 1 `type_mismatch` |
| 2 | route A | `text.slice(...)` | `repeat("=", to_u64(n).must())`, `repeat(" ", to_u64(line).must())` | both exit 0 |
| 3 | route A | `text.slice(...)`, `line.slice(...)` | `repeat(..., to_u64(...).must())` twice | 01 and 02: 1 `builtin_name_taken` each, a binding named `all` (`all = args_checked()`) |
| 5 | route A | `text.slice(...)` three times, `line.slice(...)` | `repeat(..., to_u64(...).must())` twice | both exit 0 |

Every `type_mismatch` is `expected `u64`, found `i64`` at a `repeat` count (the `.brief.txt` files
beside the programs). The two `builtin_name_taken` are `all`, a name the sentence lists, bound by the reader as a local
(`all = args_checked()`, line 2 of `w3-01.hero`); with it renamed (`perl -pe 's/\ball\b/argv_all/g'`
piped to `check --brief /dev/stdin`) both programs check at exit 0. The first draft of this report
said *a function named `all`*, the shared brief's framing; the critic's second pass found it was a
binding, and the coordinator corrected it before the sitting was committed.

## The result

- **`repeat`'s count**: under today's sentence **6 of 6 programs** were refused for an `i64` count;
  under route A's **0 of 6** were, all six writing `to_u64(...).must()`. By program, Fisher's exact
  test two-sided p = 0.0022; by session, the honest unit since a session's two programs share a
  reading, 3 of 3 against 0 of 3, p = 0.1 (`python3`, `math.comb`, the command in the
  coordinator's session).
- **`slice` with nothing to slice**: **none of the 12 programs**, under either sentence, wrote one;
  every `slice` has its receiver. At three Haiku sessions a variant the experiment cannot see the
  `slice` half of route A: the trial's 9 of 30 came at turn 1 over ten tasks, here twelve programs
  over two tasks.
- **No program under route A was refused for anything route A's text caused**; its two refusals are
  the binding `all`, a name today's sentence lists exactly as route A's does, and with it renamed all
  six route-A programs check at exit 0.

## Verdict

**Approve route A's `repeat` half on this experiment's evidence**, which is the strongest a blind
reading has given this sitting's question: the one spelling change made every reader write the
conversion the compiler asks for. **No reading on the `slice` half**, neither for nor against: the
experiment did not provoke the mistake. No veto: nothing in it shows route A's sentence making a
reader write a program the compiler refuses that today's sentence does not. A falsifiable
prediction for the sitting: on the next run of the trial's turn 1 under route A's sentence, Sonnet
5.5's and Opus 5.5's refusals for `repeat`'s count fall from 6 and 5 to at most 1 each, scored in
sample (the assistant's ten tasks) and said so.

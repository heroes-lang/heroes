# Panel 228, the llm-ergonomist: the blind experiment's result

**Run 2026-10-25 by the coordinator** on the second design (`docs/panel/228-briefs/blind/README.md`),
which the critic's first pass made necessary; the seat's verdict is the experiment's result, every
number below with its command. The first design was never run.

## How it ran

80 `claude -p` sessions on `claude-haiku-5-5`, one per folder of the trunk's `build/b/1` to `80`, each
holding the specification at `9a38f1b26` (its SHA-256 the trunk's own, `5b69e6a8c6b327d4…`), its
family's brief and one variant, nothing else; the folders numbered by `random.Random(22825)`. The
command is the README's, run by `blind/run_one.sh`, six at a time (`xargs -P 6`), the probe of folder
1 first. **What it cost**: 0.244 USD for the 80, by the sessions' own `total_cost_usd`, against the
author's 3 USD; the probe, folder 1, read 0.0038. **Every session exited 0**, wrote nothing to its
stderr, and **left no transcript**: `ls ~/.claude/projects | grep -c build-b` read 0 before the probe
and 0 after the run (the critic's finding 8, repaired by `--no-session-persistence`).

**Blindness**: every tool call of every session, read from its `run.jsonl`, is a `Read` of a path
inside its own folder (`blind/score.py`, 0 voids). Every `context` answer was read: each names at most
the system prompt (the folder sentence, and in eight answers an identity line, *a Claude agent on the
Claude Agent SDK*, which the CLI's prompt carries under `--bare`) and nothing of the project; a search
of the 80 answers for `milestone`, `panel`, `lane`, `CLAUDE.md`, `rechecked`, `route [A-E]`,
`worktree`, a milestone's name, `proposal` and `schema 3` matched one answer, whose *proposal* is its
own program. **The context rule was amended before the run**, after the probe: it asked for a
`context` of `no`, and the probe's `yes` named only the system prompt (README, § Scored).

## The scores, by the rules the README registered before the run

Scored by `blind/score.py` over the 80 folders before `key.txt` was opened; `scores.json` is its
output, joined to `key.txt` after. Out of 10 per family and shape (two tasks, five sessions each):

| family | shape | `finished` right | the second score right |
|---|---|---|---|
| F, *is it finished*, no program shown | T, today's | 10 | 10 (every hole's `line:column` and type) |
| | A | 10 | 10 |
| | B | 10 | 10 |
| | C | 10 | 10 |
| W, *finish it*, `lib.hero` unseen | T, today's | 9 | 10 (checks strict, prints `27` or `12.00 EUR`) |
| | A | 10 | 10 |
| | B | 10 | 10 |
| | C | 7 | 10 |

**The reading the README registered**: no shape stands 3 or more of 10 from T on either score, so
**no shape helps or hurts that this sample can show**, and **no veto is recorded on the seat's
behalf**. At five sessions a cell this is a reading, not a rate.

**What the four wrong `finished` answers are** (folders 19, 43 and 58, shape C; 25, shape T; each read
after the key): every one names the hole in the same sentence and answers *yes* about the program it
then returns filled, *the `???` is the only thing blocking a build, and it is filled below* (58),
*filling it ... removes the last obstacle* (25). So all 80 sessions found every hole; the brief's
*as it stands* was read as *as I return it* four times. The registered score stands as counted, and
the reading is recorded beside it: the gap between C's 7 and T's 9 is that ambiguity, under the
threshold, and not a shape hiding a hole.

## What the experiment can and cannot say

- **It can say**: with the hole's facts reachable only through the answer, a model of the smallest
  pinned size read the expected type, the fields, the makers and their documentation lines out of
  the text, out of route A's member, out of route B's second document and out of route C's notes,
  and wrote a program that checks and prints the right value, 40 of 40 in each family; and none
  took a program with holes for finished in family F, A's `"diagnostics": []` at exit 0 included.
- **It cannot say**: anything about a program reading the document, which is what route A against
  D and route B's stream question are about (the ffi-pragmatist's seat); anything about Sonnet,
  Opus or Fable; anything about the cap's `… and N more` (no task held more than five names in
  scope) or a variant's `cases`; and nothing about E, which was not run.
- **The prediction this seat registers**, checkable at the landing's own gate: the same 80 tasks
  re-run on the document the landing emits, its keys whatever the sitting adopts, score 10 of 10 on
  the second score in each family and shape, the same reading, at 5 sessions a cell and at most
  0.035 USD a session. Wrong if any cell falls below 8.

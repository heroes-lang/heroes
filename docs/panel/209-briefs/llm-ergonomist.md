# Panel 209, the blind seat: how it is run and what it is asked

The llm-ergonomist reads only a copy of the specification and the task; it
never sees design.md, the repository or this sitting's other briefs. Since
panel 183 it runs as fresh `claude -p` sessions, not as a subagent, so that no
project rule reaches its context; its `context` answer is recorded in the
synthesis and a yes voids the reading.

## The folders, inside the repository's root

`.claude/worktrees/scratch-b15/209-blind/<task>-<variant>/`, twelve of them:
`t1`, `t2`, `t3`, each in **A** (the trunk's `spec/heroes-spec.md` at
`87794631`, byte for byte, `cmp`) and **B** (draft B2,
`209-coordinator/measure/spec-B2.md`), each read twice (`t1-A` and `t1-A2`
are byte-identical folders, `cmp`), so every cell of the A/B has two
readings as panel 206's had. Each holds `spec.md`, `brief.md` (identical
across A and B, the texts below) and for `t3` a `main.hero`. The sessions
write `report.md`, `run.json`, `run.err` there. The launcher is
`209-blind/run.sh`, the panel skill's command of 2026-10-01 with the model
panel 201 used, `claude-opus-5-5`, `--restricted --safe-mode
--strict-mcp-config`, `Read` and `Write` only, **0.80 USD a session at most,
twelve sessions, 9.60 USD at most**. **The author said yes on 2026-10-10 to
ten dollars at most** (CLAUDE.md § 3; `.claude/rules/verification.md` § The
batch). Read the same afternoon on code.claude.com (its costs, headless,
authentication and CLI reference pages): the dollar figure the CLI prints is
a client-side estimate at list price, `--max-budget-usd` stops a session when
that estimate reaches the cap, and in `-p` mode an `ANTHROPIC_API_KEY` in the
environment is always used when present; `claude auth status` on this
machine reads `authMethod: claude.ai`, `subscriptionType: team`, and no key
is exported in the login shell, so these sessions run on the subscription's
usage and the launcher loads no `.env`. The `claude` CLI is 2.1.286 (`claude
--version`). **The critic's R10** (the symbol with the type kept) is not a
third variant: B2 is the bundle that would land, and the seats' reasoning
separates the symbol's effect from the inference's where two readings cannot.

## The tasks

**t1, write** (`brief-t1.md`): a program with `longest(words: [str]) -> str`,
the longest string of a non-empty array, the first on a tie, and `main`
printing `longest(["ab", "abcd", "xyz"])`. The natural program holds a cell
for the best so far. Scored by compiling the program: A under the trunk's
compiler, B under the compiler-engineer's R1 prototype where it builds, else
by hand against B2; the report's `sentences` says which lines of the
specification the writer found for a changing value.

**t2, write** (`brief-t2.md`): `tally(words: [str]) -> {str: i64}`, how many
times each word appears, and `main` binding `t = tally(["a", "b", "a"])` and
printing `t["a"].default(0)`. The empty map needs its annotation under both
variants (`m: {str: i64} @ {}` today, `m: {str: i64} @= {}` under B2); the
question is whether the reader finds the rule in each document.

**t3, read** (`brief-t3.md`): the program below, in A's form and in B's; the
reader says whether the checker accepts it, which lines it refuses, the
smallest repair, and for every line holding `=` or `@` whether it brings a
new name or changes one, with the words of the specification that tell the
two apart on that line alone.

A (`t3-A/main.hero`), the trunk's compiler on it: exit 1, `unknown_name` at
`4:9` with the certain fix `total`, and nothing else; with that fix applied,
exit 0 and the program prints 34 (`209-coordinator/probes/t3_repaired.hero`),
so `limit`, never re-bound, is accepted today:

```
function total_of(xs: [i64]) -> i64
    total: i64 @ 0
    for x in xs
        totl @ total + x
    return total

function main()
    limit: i64 @ 10
    scores = [3, 4, 5]
    sum: i64 @ 0
    for s in scores
        sum @ sum + s
    print(total_of(scores) + sum + limit)
```

B (`t3-B/main.hero`), the same with `total @= 0`, `limit @= 10`, `sum @= 0`.
The expected reading under B2: line 4 refused (nothing named `totl`), line 8
refused (a cell nothing re-binds), the repair `total` and `limit = 10`;
declarations on lines 2, 8, 9, 10, a re-binding on 12 and an attempted one
on 4.

## What the synthesis reads from the six reports

Per task and variant: the program compiles or not and on which line; the
sentences quoted for the changing value's declaration; the `choice_points`
(a choice the specification left open under one variant and not the other is
the finding); the `lines` table of t3 against the expected reading;
`context`. The prediction the blind seat scores is each other seat's claim
about readers: that `@=` is or is not found and applied on the first try,
and that the one-character distance from `@` is or is not read wrong.

The three briefs' texts are kept in `docs/panel/209-briefs/blind/`, and the
reports are copied into `docs/panel/209-reports/llm-ergonomist-<task>-<variant>.md`
with a header saying how each was run.

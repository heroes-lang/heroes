# Panel 232's blind seat (draft, the coordinator's, before any session)

What it measures: whether each drafted sentence changes what a reader of the
specification alone predicts a program does, against what the compiler does.
A reader who predicts the compiler's answer from today's specification needs
no sentence; one who predicts it only from the amended one is the sentence's
measured effect (design.md Part 11, the cost of a token weighed against what
it buys).

## Three families, each tied to one sentence

- **O, the order of evaluation** (item 1). A program whose output depends on
  it: `noisy(label: "left", value: 1) + noisy(label: "right", value: 2)`,
  each `noisy` printing its label; a call of two such arguments; a record
  built from two. The case panel 010 found ambiguous, labels in another order
  than the signature's, is no program: the compiler refuses it
  (`wrong_label`, measured 2026-10-29). Truth: what the frozen compiler
  prints, run.
- **S, `<<` past the sign bit** (item 2). `print(3 << 62)` and
  `print(1 << 63)`: a value, or an abort? Truth: the compiler's.
- **X, `exit` with a literal outside 0 to 255** (item 5). `exit(code: 256)`:
  refused, an abort, or a status? Truth: the compiler's, refused at compile
  time since task 2250.

## Cells

3 families × 2 variants (T, today's specification at the frozen head; P, the
same with the drafted sentences of items 1, 2 and 5 applied) × 2 models
(`claude-haiku-5-5`, `claude-sonnet-5-5`) × 7 sessions = 84 sessions, each
bounded at 0.14 USD: 11.76 USD at most, inside the author's 12, as panel 230
ran. The first session of each model runs alone and its cost is read before
the others; one the bound stops is scored as no answer and never re-run.

## Where they run, and what they see

Each session in its own folder under the lane's `build/b232/<n>` (git
ignores `/build`), inside the repository's root (CLAUDE.md § Hard stops),
with `GIT_CEILING_DIRECTORIES` set to `build/b232` so no git tree is found
above it, run as panel 230's `run_one.sh` ran it: `--bare
--no-session-persistence --restricted --safe-mode --strict-mcp-config`, the
`Read` tool alone, `--max-budget-usd 0.14`. Its folder holds the
specification's copy, `brief.md` and `task.md`, and nothing else. Its
`context` answer is read before the key is joined; one naming a project
file, rule, memory or commit is void.

## Scored before the key is read

Per session: the answer's predicted output, line by line, against the truth;
an answer that hedges between two is wrong. Per family and variant: the
share right. The spec-warden registers before the first session how many of
the 14 P sessions of each family it predicts right, and the T share it
expects; a prediction missed is written down as missed.

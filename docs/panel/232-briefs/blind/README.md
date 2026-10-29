# Panel 232's blind seat: what a reader of the specification alone predicts

The coordinator's experiment, run as panel 230's was
(`docs/panel/230-briefs/blind/README.md`; its `run_one.sh` and `score.py`
are in `docs/panel/230-reports/evidence.tar.xz`, F21), committed with the
briefs before its first session. **The brief asks no `choice_points`**:
panel 230 saw that section refused ten times by the API's safeguards as a
request for a model's reasoning, and removed it (F21); whether a session on
a prediction task refuses anything is unrun until the first session of each
model, which runs alone. It asks whether three drafted sentences change what a
reader of the specification alone predicts three programs do, against what
the compiler does (`00-shared.md` items 1, 2 and 5; `design.md` beside this file).

## The two specifications

- **T**: `spec/heroes-spec.md` at `afe983265`, byte for byte.
- **P**: the same with three sentences, the coordinator's provisional drafts,
  each in the section of the operation it governs:
  - § 7, after *`/` and `%` truncate toward zero ...*: *An expression's parts are evaluated left to right as written; a write evaluates its place's indices, then the value, and checks the place as it writes.* (repaired after F4: the first draft's *a place before the value* led a reader to put an index's abort before the value's effect);
  - § 7, *Overflow aborts at every width.* becomes *Overflow aborts at every width, but `<<` shifts the bits and wraps.*;
  - § 11, `exit`'s parenthesis becomes *(ends the program; a code outside 0 to 255 aborts, a literal one is refused).*

## The tasks and their truths

`tasks/O.md`, `tasks/S.md`, `tasks/X.md`, `tasks/W.md`, each one program and the question
*what does this program print, and how does it end?* The truths, run by the
lane's compiler at `afe983265` (`sources 3e3c95dc246b73b3`):

- **O**: `left`, `right`, `a`, `b`, `10`, `ends: 0`;
- **S**: `-4611686018427387904`, `-9223372036854775808`, `done`, `ends: 0`;
- **X**: `refused`;
- **W**: `index`, `value`, `ends: abort` (array index out of range).

## The cells and the run

**Task W added and every cell cut from 7 to 5 before any session ran**, on
the spec-warden's registered note that task O is a ceiling task: every
reader assumes left to right for operands and fields, so it cannot show
item 1's sentence serving the thesis, while a write's two phases (F4) can.
The budget holds: 80 sessions instead of 84.

4 tasks × 2 specifications × 2 models (`claude-haiku-5-5`,
`claude-sonnet-5-5`) × 5 = 80 sessions, each in its own folder under the
lane's `build/b232/<n>` (git ignores `/build`, and the folders are inside the
repository's root, CLAUDE.md § Hard stops), holding `heroes-spec.md` (T or
P), `brief.md` and `task.md` and nothing else. The order is drawn by
`random.Random(232).shuffle`; the key, folder to task, specification and
model, is written to `.claude/worktrees/232-coordinator/sealed/key.tsv`,
which the run reads for the model and the scorer never opens. Each session
runs from inside its folder, its transcript beside the folders and never in
one:

```
cd build/b232/<n> && GIT_CEILING_DIRECTORIES=<lane>/build/b232 claude -p "Read brief.md in this directory and follow it exactly. Your inputs are the files in this directory only. Answer in English." \
  --bare --no-session-persistence --system-prompt "You work in the directory <lane>/build/b232/<n>. You read its files with the Read tool, by absolute path." \
  --model <model> --restricted --safe-mode --strict-mcp-config \
  --tools "Read" --allowedTools "Read" \
  --disallowedTools "Bash,Write,WebFetch,WebSearch,Glob,Grep,Edit,Task,Agent,ListAgents,SendMessage" \
  --max-budget-usd 0.14 --output-format stream-json --verbose < /dev/null > <lane>/build/b232-out/<n>.jsonl 2> <lane>/build/b232-out/<n>.err
```

**The budget**: 80 × 0.14 = 11.20 USD at most, inside the author's 12. The
first session of each model runs alone and its cost is read before the
others; one the bound stops is scored as no answer and never re-run; if the
first of a model is stopped by the bound, the run stops and the author is
asked. The key is the trunk's `ANTHROPIC_API_KEY` from `.env`, never printed.

## Scored before the key is read

- **The context**: a session whose tool calls are anything but `Read` of a
  path inside its own folder is void and counted apart; its `context`
  section is read before the key is joined, and void where it names a
  project file, rule, memory or commit.
- **Right**: the `answer` section, lines trimmed, equals the truth exactly;
  an answer that hedges, prints two alternatives, or says `refused` and
  lines, is wrong.
- **Per task and specification**: the share right, by model and together.
- **Predictions registered before the first session** (the spec-warden's,
  copied here from its report, `docs/panel/232-reports/spec-warden.md`,
  out of the 10 sessions a cell holds, each a point with the band whose
  crossing is a miss):

  | task | P right of 10 | T right of 10 |
  |---|---|---|
  | O | 10 (a miss below 9) | 9 (a miss below 8) |
  | S | 9 (a miss below 7) | 1 (a miss above 3) |
  | X | 9 (a miss below 8) | 0 (a miss above 1) |
  | W | 9 (a miss below 8) | 4 (a miss above 6) |

  The effect each predicts, P minus T: O at most 1, S at least 5, X at
  least 7, W at least 3.

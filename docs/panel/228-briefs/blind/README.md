# The blind seat, second design: the compiler's answer as the only channel

**Why a second design.** The first (`first-design/`, kept as it was handed to the critic) showed the
program whole, `???` in plain sight and every record and function the answer named beside it, so
every score could be read off the program and none off the shape; it left route C out, wrote a
transcript per session outside the repository's root, and bounded each session and not the sitting
(`docs/panel/228-reports/critic-briefs.md`, findings 4 to 10). No session of it was run.

**What the seat measures**: whether the shape in which `heroes check --json` hands a model a hole
changes what the model can do with it, when the answer is the only place the hole's facts are.

- **Family F, *is it finished*** (`brief-f.md`): the model sees no program, only the answer, and says
  whether the program is finished and, if not, each place as `line:column` with the type that
  belongs there. Task f1 is a program of two holes, one module (`blind228/src/t1.hero` of the first
  design); f2 is w2's program, one hole whose type is another module's.
- **Family W, *finish it*** (`brief-w.md`): the model sees `main.hero`, which uses `lib.hero`, never
  shown, and writes the finished `main.hero`. The record, its fields and the functions that build it
  live in `lib.hero` and reach the model through the answer alone. Task w1 (a `Box`, two makers each
  with its documentation line): print `27`. Task w2 (a `Money`, seven makers of which the report shows
  five, the cap read): print `12.00 EUR`.
- **Four shapes**, the routes of the shared brief: **T**, today's, exit 0, the text report on stdout
  and `"diagnostics": []` on stderr; **A**, exit 0, nothing on stdout, `"holes"` beside the empty
  `"diagnostics"`; **B**, exit 0, a `"kind": "holes"` document on stdout and stderr as today; **C**,
  exit 1, each hole an entry of `"diagnostics"` with the code `hole` and its report in `"notes"`.
  Route D is A with `"schema": 3`, one digit a model reading the answer has no reason to act on, and
  route E (a count beside today's text) is T's information plus a number; neither is run, and the
  synthesis says so.

**The information is the same in the four shapes**, rendered from one hand-transcribed table,
`variants.py` beside this file, from the lane's stage-2 compiler's real answers (`sources
451802aa66692eb3`, `check --json main.hero` in each task's folder, kept with the programs in the
sitting's evidence). Two defects are removed from every shape alike, since the seat measures the shape
and not them: defect 2203's own binding in scope (f1: `q: Point` in its first hole, `d: i64` in its
second) and defect 2205's bare type (`Box` and `Money`, written `lib.Box` and `lib.Money` as the file
must write them). The JSON keys of A and B are the proposal's as the shared brief now names them; the
compiler-engineer builds to them or says why not. No hole of the four is in statement position, so
the text's *any type, or none* (defect 2204) appears in no shape. C's notes are strings, as
`selfhost/cli/check_json.hero` writes a diagnostic's notes today; the documentation line of each
function stands after it in parentheses.

**The run**: 4 tasks × 4 shapes × 5 sessions = **80 sessions**, each in its own folder holding
`heroes-spec.md` (the specification at `9a38f1b26`, `git show 9a38f1b26:spec/heroes-spec.md`), its
family's brief as `brief.md`, and one variant as `task.md`, nothing else. The folders are the
trunk's `build/b/1` to `build/b/80` (inside the repository's root, under `/build`, which git
ignores), numbered in an order drawn by `python3 -I -c 'import random; ...'` with a seed written in
the key. **The key** (folder to task and shape) is written in
`.claude/worktrees/rechecked-issues-scratch/blind228b/key.txt` and copied into the sitting only after
every answer is scored.

**The model is Haiku 5.5**, the smallest of the four the trial pins
(`tests/thesis/held-out/prediction.md`): the likeliest to miss a hole a shape hides, an inference.
**Each session is bounded at `--max-budget-usd 0.035`, so the 80 cannot pass 2.80 USD**, inside the
author's 3 USD of 2026-10-25; panel 227 measured 0.0094 USD a session on the same model with the same
specification (`docs/panel/227-reports/llm-ergonomist.md`). A session the bound stops is scored as
no answer and counted, never re-run. The key is the trunk's `ANTHROPIC_API_KEY` from `.env`, as panel
227's blind seat used it within the author's budget, though `.env.example` gives that key to
`count_tokens` alone: `--bare` takes no other credential.

**The command**, panel 227's with the critic's two repairs (no transcript written, every tool call
kept):

```
GIT_CEILING_DIRECTORIES=<build/b> ANTHROPIC_API_KEY=<from .env> claude -p "Read brief.md in this directory and follow it exactly. Your inputs are the files in this directory only. Answer in English." \
  --bare --no-session-persistence --system-prompt "You work in the directory <build/b/N>. You read its files with the Read tool, by absolute path." \
  --model claude-haiku-5-5 --restricted --safe-mode --strict-mcp-config \
  --tools "Read" --allowedTools "Read" \
  --disallowedTools "Bash,Write,WebFetch,WebSearch,Glob,Grep,Edit,Task,Agent,ListAgents,SendMessage" \
  --max-budget-usd 0.035 --output-format stream-json --verbose > run.jsonl 2> run.err
```

**Scored before the key is read, by one script over the 80 folders**, pre-registered here:

- **the context**: every `Read` a session made names a path inside its own folder (the tool calls in
  `run.jsonl`), and its `context` answer names nothing beyond the folder's files, the one-sentence
  system prompt and the request (no project rule, memory, file list or commit); a session failing
  either is void and counted apart. **Amended before the run, after the probe of folder 1**: this
  line asked for a `context` of `no`, and the probe answered `yes`, naming only the system prompt
  `--bare --system-prompt` hands it and *no project rule, memory, file list or commit history*,
  which is the one thing that command lets through and what panel 227's probe read the same way;
  under the first wording every session would have been void. The coordinator read folder 1's answer
  to see this, and it is scored with the others;
- **F**: `finished` correct when the section's first word is `no`; `places` correct when, for every
  hole of the task, its `line:column` (f1: `12:16` and `13:14`; f2: `5:21`) appears and its type
  (f1: `Point` and `i64`; f2: `Money`) appears on that line;
- **W**: `finished` as in F; the program is the **last** fenced block of the answer that holds
  `function main` (as `tests/harness/thesis_grade.hero` reads a model's program), written as
  `main.hero` beside the task's real `lib.hero`, and correct when the lane's stage 2 checks it at exit
  0 with no flag and `heroes run` prints exactly `27` (w1) or `12.00 EUR` (w2).

**What counts, written before the run**: per family, each shape's correct answers pooled over its two
tasks, out of 10, for each score. **A shape helps or hurts where it stands 3 or more of 10 from T on
a score; a smaller difference is *no difference this sample can show*.** A veto on the seat's behalf
is recorded only where a route's shape stands 3 or more of 10 below T on `finished` or on the
program. At five sessions a cell this is a reading, not a rate.

**The seat's report** is the experiment's result, written by the coordinator in
`docs/panel/228-reports/llm-ergonomist.md` from the scores, every number with its command.

# The blind seat's folders, as they were handed to the llm-ergonomist

Six `claude -p` sessions, one per folder, each holding a copy of the specification at `63c78c54b`
in one variant, the two tasks and `brief.md`, nothing else. **The key, never in any folder**: the
folders are `build/w/1` to `build/w/6` (inside the repository's root, under `/build`, which git
ignores); **2, 3 and 5 hold route A's sentence** (`xs.slice(from:, to:)`, `repeat(s, n)` (`n` a
`u64`)), **1, 4 and 6 today's** (`slice(from:, to:)`, `repeat(s, n)`); the two copies differ in
lines 316 and 317 alone (`diff`), sha256 `4d8e5251e78d…` route A and `212264059c98…` today's, the
latter the specification's own.

**The model is Haiku 5.5**, the one model whose trial answers held a `slice` with nothing to slice
(9 of its 30 turn-1 answers, none of Sonnet's or Opus's; the critic's first pass, G1). A Sonnet
session per variant, the first draft of this page, could not have told the two variants apart on
`slice`. Three sessions per variant rather than one, each bounded at 1 USD, so the seat costs at
most the 2 × 3 USD the author allowed it on 2026-10-21; at three samples a variant this is a
reading, not a rate.

**The brief asks for the programs, the choice points and the context, and nothing about the
built-ins' sentence**: the first draft asked the reader to quote the sentence each call's shape came
from, which points it at the line under test where the trial's prompt (`docs/metrics/first-try.md`)
points nowhere (the critic's G11).

**A rule gave way, and which**: `.claude/skills/panel/SKILL.md` puts the blind seat's copy
*outside the repository*, in the scratchpad, outside any git tree; CLAUDE.md § Hard stops forbids
writing a file outside the repository's root (the author's rule of 2026-10-08). A hard stop
outranks a process rule (CLAUDE.md § Precedence, ranks 1 and 5), so the folders are inside the root,
and what the skill's location bought, no project and no git reaching the session, is bought by the
command below and checked by each session's `context` answer.

## The command, and the two runs it took to be blind

```
GIT_CEILING_DIRECTORIES=<build/w> ANTHROPIC_API_KEY=<from .env> claude -p "Read brief.md in this directory and follow it exactly. Your inputs are the files in this directory only. Answer in English." \
  --bare --system-prompt "You work in the directory <build/w/N>. You read its files with the Read tool, by absolute path." \
  --model claude-haiku-5-5 --restricted --safe-mode --strict-mcp-config \
  --tools "Read" --allowedTools "Read" \
  --disallowedTools "Bash,Write,WebFetch,WebSearch,Glob,Grep,Edit,Task,Agent,ListAgents,SendMessage" \
  --max-budget-usd 1 --output-format json > run.json 2> run.err
```

The report is the session's final answer, `run.json`'s `result`.

- **The first run is void**: the panel skill's command, `--model claude-haiku-5-5`, `Read,Write`, in
  `.claude/worktrees/thesis-drafts/227-blind/p1` to `q3`, 12:38, 0.0823 USD for the six. Every
  session answered `context: yes`: Claude Code's default system prompt carried the working
  directory, the environment, the account and **a git status of the repository above the folder,
  its branch and its five latest commit subjects**, which name the trial and measurement 050.
  `GIT_CEILING_DIRECTORIES` stops `git` and did not stop that summary. The skill's rule, *a yes
  voids the reading until it is re-run from a copy*, voids all six.
- **Two probes on folder 1**: `--system-prompt` replacing the default with one sentence naming the
  folder, with `--bare` (0.0103 USD; context: that sentence and the request, nothing else; `Write`
  absent under `--bare`, so the report came as the answer) and without it (0.0157 USD; context:
  the working directory, the platform, the date, the model and the account came back, no git
  status and no commit). Kept in `.claude/worktrees/thesis-drafts/227-blind/probes/`.
- **The run that counts**, 12:44, the command above: `--bare` and one sentence of system prompt,
  `Read` alone, the report as the answer, the folders named by a digit and nothing else.

**The seat's verdict is the experiment's result**, written by the coordinator in
`docs/panel/227-reports/llm-ergonomist.md` from the six answers and the checks, every number with
its command: each program put through the lane's `heroes-next check` in strict and `--brief`,
which calls each variant's readers wrote, and which compile. The coordinator holds no veto on the
seat's behalf: one is recorded only where the experiment shows route A's sentence making a reader
write a program the compiler refuses that today's sentence does not.

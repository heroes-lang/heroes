# Panel 189, the llm-ergonomist's seat: a blind experiment of three arms, run by the coordinator

The seat runs as fresh sessions outside the repository, never as a subagent
(`/panel` § 2), within the author's cap for this sitting, **5 USD in all**
(the author's answer *"5 dollari"*, 2026-10-03): **18 sessions, six per arm,
each `--max-budget-usd 0.25`**, so at most 4.50 USD.

## The folders

`<scratchpad>/189-llm-ergonomist/<arm><n>/` for arms `a`, `b`, `c` and n
from 1 to 6, outside any git tree, with no `CLAUDE.md`, `.git` or `.claude`
in them or above them (checked by walking every parent): each holds
`spec.md`, the specification at `7d9f2e8f` byte for byte (`cmp` against the
trunk's), and `brief.md`, kept here as `blind/a-brief.md`, `blind/b-brief.md`
and `blind/c-brief.md`. The three briefs are identical but for what the
compiler printed (`diff`: that block alone):

- **A**, what the trunk's compiler prints today for the program
  (`<scratchpad>/189-blind-src/p.hero`, its bytes in the brief): `check` and
  `build` exit 2 with *error: cannot read `p.hero`* (captured from
  `7d9f2e8f`'s compiler, `check.txt` and `build.txt` beside it).
- **B** and **C**, the coordinator's draft of a refusal at `check`, in the
  compiler's own full form (its gutter copied from a real
  `unexpected_character` diagnostic of the trunk, F1's `valid-ident`), the
  same words in both, *"the byte 0xE9 at 2:15 is not UTF-8, and a `.hero` file
  is UTF-8 text"*, the source line shown with U+FFFD where the byte stands;
  **B** with the code `unexpected_character`, **C** with `not_text`. A draft
  of no route in particular (the sitting's wording is Q2), so B against C
  isolates the code's name, and A against B and C the message.

The program is `function main()` over `print("caf` + the byte 0xE9 + `")`;
what it should do is print `café`; its repair is that line with `é` written
in UTF-8 (0xC3 0xA9), which the trunk's compiler builds and runs.

## The command, per folder

```
claude -p "Read brief.md in this directory and follow it exactly. Your inputs are the files in this directory only. Write report.md here. Answer in English." \
  --model claude-opus-5-5 \
  --restricted --safe-mode --strict-mcp-config \
  --tools "Read,Write" --allowedTools "Read,Write" \
  --disallowedTools "Bash,WebFetch,WebSearch,Glob,Grep,Edit,Task,Agent,ListAgents,SendMessage" \
  --max-budget-usd 0.25 --output-format json > run.json 2> run.err
```

## What is scored

Each `c.hero` built with the trunk's compiler: **a one-turn repair is a
`c.hero` that builds and prints `café` exactly** (one that prints `cafe`, or
writes an escape the spec does not define, is not). And each report's `cause`
read against the truth (the file's encoding): named, or not. The coordinator
copies each arm's reports into `docs/panel/189-reports/llm-ergonomist-<arm>.md`
with how it was run, the model the CLI reports and the cost from `run.json`.

**What this does not measure**: whether a program in the wild is ever saved
in Latin-1; a byte other than 0xE9; a comment rather than a string (any
rewrite repairs a comment, so it would not separate the arms).

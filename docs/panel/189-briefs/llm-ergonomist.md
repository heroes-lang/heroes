# Panel 189, the llm-ergonomist's seat: a blind experiment of four arms, run by the coordinator

Repaired after the completeness critic's first pass (the text it read is
`llm-ergonomist-before-the-critic.md`; its arms' briefs are kept as
`blind/<arm>-brief-before-the-critic.md`). **The first design gave the cause
away**: every brief explained, in its own prose, that the page could not hold
the file's byte, so the arm without a message was told what the messages say.
The program now travels as itself.

The seat runs as fresh sessions outside the repository, never as a subagent
(`/panel` § 2), within the author's cap for this sitting, **5 USD in all**
(*"5 dollari"*, `docs/records/log/2026-10-03-2343-panel-189-convened-for-not-text-the-blind-seat-capped-at-5-usd.md`):
**16 sessions, four per arm, each `--max-budget-usd 0.25`**, so at most 4.00
USD if no session passes its cap (whether one can, on its last call, is
unrun; panel 188's 15 sessions cost 0.17 to 0.18 each at a 0.25 cap).

## The folders

`<scratchpad>/189-llm-ergonomist/<arm><n>/` for arms `a`, `b`, `c`, `d` and n
from 1 to 4, outside any git tree, with no `CLAUDE.md`, `.git` or `.claude`
in them or above them (checked by walking every parent): each holds
`spec.md`, the specification at `7d9f2e8f` byte for byte (`cmp` against the
trunk's), **`p.hero`, the program's own bytes** (`<scratchpad>/189-blind-src/p.hero`,
`cmp`), and `brief.md`, kept here as `blind/<arm>-brief.md`. The four briefs
say nothing of bytes, encodings or UTF-8 in their own prose (`grep -c -i -E
'utf|byte|encod|hex' blind/a-brief.md`: 0) and differ only in what the
compiler printed and its exit status (`diff`):

- **A**, what the trunk's compiler prints today for `p.hero`: `check` and
  `build` exit 2 with *error: cannot read `p.hero`* (captured from
  `7d9f2e8f`'s compiler, `<scratchpad>/189-blind-src/check.txt` and
  `build.txt`).
- **B**, **C** and **D**, the coordinator's draft of a refusal at `check` at
  exit 1, in the compiler's own full form (its gutter copied from a real
  `unexpected_character` diagnostic of the trunk, F1's `valid-ident`, and
  checked by the critic), the same words in all three, *"the byte 0xE9 at 2:15
  is not UTF-8, and a `.hero` file is UTF-8 text"*, the source line shown with
  U+FFFD where the byte stands (as the session's own Read tool shows the
  file); **B** with the code `unexpected_character`, **C** with `not_text`,
  **D** with `invalid_utf8` (a name by the bytes, as Go's front end words
  it). A draft of no route in particular (the sitting's wording is Q2): B, C
  and D isolate the code's name, and A against them the message.

The program is `function main()` over `print("caf` + the byte 0xE9 + `")`;
what it should do is print `café`. Its repair, that line with `é` written in
UTF-8, builds on the trunk's compiler and prints `63 61 66 c3 a9 0a` (the
coordinator and the critic, each on its own build).

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

Each `c.hero` built with the trunk's compiler and run. **A one-turn repair is
a `c.hero` that builds and prints `café`**: the precomposed `é` (`63 61 66 c3
a9 0a`) and the decomposed one (`63 61 66 65 cc 81 0a`) both count and are
reported apart; one that prints `cafe`, or writes an escape the spec does not
define, or keeps the bad byte, does not. A session stopped by its cap is
unscored and reported. And each report's `cause` read against the truth (the
file's encoding): named, or not. The coordinator copies each arm's reports
into `docs/panel/189-reports/llm-ergonomist-<arm>.md` with how it was run, the
model the CLI reports and the cost from `run.json`.

**What this does not measure**: whether a program in the wild is ever saved in
Latin-1 or UTF-16; a byte other than 0xE9; a comment rather than a string (any
rewrite repairs a comment, so it would not separate the arms); several bad
bytes (Q3); an encoding hint in the message (Q2). Four sessions an arm tell a
ceiling from a floor and little between.

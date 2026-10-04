# Panel 192, the llm-ergonomist's seat: a blind experiment of three arms, run by the coordinator

The seat runs as fresh sessions outside the repository, never as a subagent
(`/panel` § 2), within the author's cap for this sitting, **5 USD in all**
(*3a*, `docs/records/log/2026-10-04-1633-the-author-ratifies-panel-190-funds-sitting-192-and-asks-for-the-push.md`):
**12 sessions, four per arm, each `--max-budget-usd 0.25`**, so at most 3.00
USD if no session passes its cap. Panel 189's sessions cost 0.17 to 0.18 each
at that cap (its reports' `run.json`); whether one can pass its cap on its
last call is unrun.

## What it measures

Q3: if a refused character stays writable, which spelling a model writes
right in one turn from the specification alone. The program needs ESC
(U+001B), which no escape writes today (F3), so it isolates the route:

- **A**, the specification at the base, byte for byte (`spec.md` `cmp`
  against `4c3524fb`'s). A raw ESC is legal there and nothing writes it
  visibly.
- **B**, `:35-36` refusing raw controls and `:47-48` adding `\u{...}`, a code
  point in hex, never 0 or a surrogate, with the example `\u{e9}`
  (`blind/spec-b.diff`).
- **C**, the same `:35-36`, and `:47-48` adding `\xNN`, a byte in hex from
  `01` to `7f`, with the example `\x07` (`blind/spec-c.diff`).

The arms' briefs are one file, `blind/brief.md`, the same bytes in every
folder: they differ in `spec.md` alone. The brief says nothing of escapes,
bytes or the specification's rules in its own prose. The examples were chosen
so that neither is the answer. B and C are the coordinator's drafts of no
route in particular, worded as the specification words its rules; the
sitting's wording is Q6's.

## The folders and the command

`<scratchpad>/192-llm-ergonomist/<arm><n>/` for arms `a`, `b`, `c` and n from
1 to 4, outside any git tree, with no `CLAUDE.md`, `.git` or `.claude` in
them or above them (checked by walking every parent). Each holds `spec.md`
(`<scratchpad>/192-blind-src/spec-<arm>.md`, `cmp`) and `brief.md`. Run from
the folder:

```
claude -p "Read brief.md in this directory and follow it exactly. Your inputs are the files in this directory only. Write report.md here. Answer in English." \
  --model claude-opus-5-5 \
  --restricted --safe-mode --strict-mcp-config \
  --tools "Read,Write" --allowedTools "Read,Write" \
  --disallowedTools "Bash,WebFetch,WebSearch,Glob,Grep,Edit,Task,Agent,ListAgents,SendMessage" \
  --max-budget-usd 0.25 --output-format json > run.json 2> run.err
```

## What is scored

**A one-turn program is a `c.hero` that builds and prints the right
bytes**, scored by the coordinator with the base's compiler (sha1
`8084f018f5387536`):
- **A** as written;
- **B** with each `\u{HEX}` replaced by its character;
- **C** with each `\xNN` replaced by its byte.

A program counts under B or C only if it holds no raw control character,
which its arm refuses, and no escape its arm does not define. The output must
hold, in order:
- `1b 5b`, then `31` or `91` (or a list holding one, `1;31`), then `6d`;
- `ERROR`;
- `1b 5b`, then nothing or `30` or `39`, then `6d`;
- the line's end.

A session stopped by its cap is unscored and reported.

The coordinator copies each arm's reports into
`docs/panel/192-reports/llm-ergonomist-<arm>.md`, with how they were run, the
model the CLI reports, and the cost from `run.json`. Each report's `context`
answer is recorded: a yes voids that reading.

**What this does not measure**:
- a comment;
- a character other than ESC;
- a model reading a line a bidirectional control reorders (Q2's reader);
- a NUL;
- the refusal's message, which no arm shows.

Four sessions an arm tell a ceiling from a floor, and little between.

# Panel 192, the llm-ergonomist's seat: a blind experiment of four arms, run by the coordinator

Repaired after the completeness critic's first pass (the text it read is
`llm-ergonomist-before-the-critic.md`, its arms' diffs
`blind/spec-b-before-the-critic.diff` and `spec-c-before-the-critic.diff`).
The first design had four faults, each repaired here:
- every arm held a route the brief called absent;
- A against B and C changed two things at once;
- the two examples stood at different distances from the answer;
- the scoring rule mixed two notations.

The seat runs as fresh sessions outside the repository, never as a subagent
(`/panel` § 2), within the author's cap for this sitting, **5 USD in all**
(*3a*, `docs/records/log/2026-10-04-1633-the-author-ratifies-panel-190-funds-sitting-192-and-asks-for-the-push.md`):
**20 sessions, five per arm, each `--max-budget-usd 0.24`**, so at most
4.80 USD if no session passes its cap. Panel 189's sessions cost 0.1623 to
0.1794 USD each at a 0.25 cap (its reports, the critic's reading); whether
one can pass its cap on its last call is unrun.

## What it measures

Q3: which spelling of a refused character a model writes right in one turn
from the specification alone. The program needs ESC (U+001B). **Each arm
differs from A in one sentence**:

- **A**: the specification at the base, byte for byte (`spec.md` `cmp`
  against `4c3524fb`'s). ESC is writable today two ways the spec does not
  name: a raw ESC in the literal, and `[27].validated_bytes()` (F10), which
  `:384` gives to *a field of bytes* only.
- **R**, route (a′): `:384` says a `[u8]` answers `validated_bytes()` too,
  with the example `[104, 105]` *gives `hi`* (`blind/spec-r.diff`).
- **B**, route (b): `:47-48` adds `\u{...}`, a code point in hex, never 0 or
  a surrogate, with the example `\u{e9}` *is `é`* (`blind/spec-b.diff`).
- **C**, route (c): `:47-48` adds `\xNN`, a byte in hex from `01` to `7f`,
  with the example `\x41` *is `A`* (`blind/spec-c.diff`).

All three examples are printable characters, so none is the answer's own
class. The vendored-table maxima, as lower bounds: A 7,117, R 7,137, B 7,152,
C 7,147 (`heroes measure <file>` run from the base's root). The arms' briefs are one file,
`blind/brief.md`, the same bytes in every folder; it says nothing of escapes,
bytes or the specification's rules in its own prose. B, C and R are the
coordinator's drafts of no route in particular; the sitting's wording is
Q6's.

## The folders and the command

`<scratchpad>/192-llm-ergonomist-2/<arm><n>/` for arms `a`, `r`, `b`, `c` and
n from 1 to 5, outside any git tree, with no `CLAUDE.md`,
`CLAUDE.local.md`, `.git`, `.claude` or `.mcp.json` in them or above them
(checked by walking every parent). The first design's twelve folders,
`192-llm-ergonomist/`, were never run and stay unused. Each folder holds
`spec.md` (`<scratchpad>/192-blind-src-2/spec-<arm>.md`, `cmp`) and
`brief.md`. Run from the folder, the sessions staggered, at most three at
once:

```
claude -p "Read brief.md in this directory and follow it exactly. Your inputs are the files in this directory only. Write report.md here. Answer in English." \
  --model claude-opus-5-5 \
  --restricted --safe-mode --strict-mcp-config \
  --tools "Read,Write" --allowedTools "Read,Write" \
  --disallowedTools "Bash,WebFetch,WebSearch,Glob,Grep,Edit,Task,Agent,ListAgents,SendMessage" \
  --max-budget-usd 0.24 --output-format json > run.json 2> run.err
```

## What is scored, fixed before the first session

The scorer is `<scratchpad>/192-blind-score.py`, written and tested on
known outputs before any session ran:
- twelve cases, among them `ESC[31m` and `ESC[1;31m` passing, and black,
  palette entry 31, bold with strikethrough and a clear screen failing;
- F10's program passing.

For each `c.hero`:
- **A and R** are built as written; **B** with each `\u{HEX}` its arm
  defines replaced by its character; **C** with each `\xNN` it defines
  replaced by its byte. Upper and lower case hex both count, and leading
  zeros in `\u{...}`; `\u001b` without braces, `\033` and `\e` are not the
  arm's escape and stay, so the base refuses them.
- The program is built with the base's compiler (sha1 `8084f018f5387536`)
  and run.
- **A pass**:
  - every escape sequence in the output is SGR (`CSI ... m`);
  - with those removed the text is exactly `ERROR` and a line end;
  - before `ERROR` the foreground is red, 31 or 91 as a parameter of its own
    and never the index of a 38 or 48 colour;
  - after `ERROR` it is reset by an empty list, 0 or 39, before or after the
    line end, which a terminal shows the same.
- **The spelling of ESC is recorded for every program**, passing or not: a
  raw ESC byte, `\u{1b}`, `\x1b`, `[u8]` with `validated_bytes()`, or other.
  A pass is credited to an arm's route only when it used that route.
- A session stopped by its cap is unscored and reported. A failing `c.hero`
  is read with `xxd`, to tell *wrote `\x1b` as text* from *meant a raw byte
  and the channel lost it*.

## Expected, registered before the first session

- **A**: at most 1 of 5, by inference to `[u8]` or a raw byte.
- **R**: at least 3 of 5, by `[u8]`.
- **B**: at least 4 of 5, by `\u{1b}`.
- **C**: at least 4 of 5, by `\x1b`.

**The rule from scores to a reading**, two-sided Fisher at five an arm
separating 5 against 1 and 4 against 0 (p 0.048, the critic's computation):
- B and C both at 4 or 5: the choice between (b) and (c) rests on Q3's other
  grounds (one spelling, the NUL, half a character).
- One of them at 5 and the other at 1 or fewer: the experiment favours the
  first.
- R at 4 or 5: route (a′) serves a writer from the spec alone, so a new
  escape needs another reason than writability.
- A at 2 or more: today's spec serves a writer more than predicted.

## The `context` answer

**The harness's own system prompt is not a yes**: the platform, the working
directory's path, the date, and an account e-mail. **A yes** is any project
rule, contract, memory, or a file outside the folder reaching the context. A
yes voids that session's reading, and it is reported.

The coordinator copies each arm's reports into
`docs/panel/192-reports/llm-ergonomist-<arm>.md`, with how they were run, the
model the CLI reports, the cost from `run.json`, and the scorer's line for
each folder.

**What this does not measure**:
- a comment;
- a character other than ESC;
- a model reading a line a bidirectional control reorders (Q2's reader);
- a NUL;
- today's `unknown_escape` message (F10), which a writer meets after the
  spec and which no arm shows.

Five sessions an arm tell a ceiling from a floor, and little between.

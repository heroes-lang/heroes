# Panel 188, the llm-ergonomist's seat: a blind A/B, run by the coordinator

The seat runs as two fresh sessions outside the repository, never as a
subagent (`/panel` § 2, the author's instruction of 2026-09-30), within the
author's cap for this sitting, 3 USD (the author's answer *1a*): **two
sessions of at most 1.5 USD each**, `--max-budget-usd 1.5`.

## The two folders

`<scratchpad>/188-llm-ergonomist/a/` and `/b/`, outside any git tree, with no
`CLAUDE.md` in them or above them (checked by walking every parent, 15:5x):
each holds `spec.md`, the specification at `826ddc2f` byte for byte (`cmp`
against the trunk's), and `brief.md`, kept here as `blind/a-brief.md` and
`blind/b-brief.md`. The two briefs are identical but for what the compiler
printed (`diff` of the two: that block alone):

- **A**, what the trunk's compiler prints today for `blind/p.hero.txt`:
  `check` silent at exit 0, then `build` at exit 2 with *internal error*,
  clang's text (which happens to ask *did you mean 'math.h'?*) and the
  generated C's path; captured from `heroes build` at `826ddc2f`'s compiler
  (`<scratchpad>/188-blind-src/build.txt`).
- **B**, the coordinator's draft of route (1b)'s refusal at `check`, in the
  compiler's own full form: `error[header_name]` naming the brackets and the
  name, at the string, with `fix (certain): replace "<math.h>" with
  "math.h"`. A draft for the experiment, not the sitting's wording (Q2).

The program is `extern "<math.h>"` over `function sqrt(x: f64) -> f64`,
`main` printing `sqrt(2.0)`; its repair `extern "math.h"` builds on this
Mac and prints `1.4142135623730951` (checked, 15:5x). The spec is the
status quo in both, so the message is the one variable.

## The command, per folder

```
claude -p "Read brief.md in this directory and follow it exactly. Your inputs are the files in this directory only. Write report.md here. Answer in English." \
  --restricted --safe-mode --strict-mcp-config \
  --tools "Read,Write" --allowedTools "Read,Write" \
  --disallowedTools "Bash,WebFetch,WebSearch,Glob,Grep,Edit,Task,Agent,ListAgents,SendMessage" \
  --max-budget-usd 1.5 --output-format json > run.json 2> run.err
```

The coordinator copies each `report.md` and `c.hero` into
`docs/panel/188-reports/` with a header saying how it was run, then builds
each `c.hero` with the trunk's compiler: a one-turn repair is a `c.hero` that
builds and prints `1.4142135623730951`.

# Panel 188, the llm-ergonomist's seat: a blind A/B, run by the coordinator

Repaired after the completeness critic's first pass (the text it read is
`llm-ergonomist-before-the-critic.md`). The seat runs as two fresh sessions
outside the repository, never as a subagent (`/panel` § 2, the author's
instruction of 2026-09-30), within the author's cap for this sitting, 3 USD in
all (the author's answer *1a*): **two sessions of at most 1.5 USD each**,
`--max-budget-usd 1.5`.

## The two folders

`<scratchpad>/188-llm-ergonomist/a/` and `/b/`, outside any git tree, with no
`CLAUDE.md` in them or above them (checked by walking every parent): each
holds `spec.md`, the specification at `826ddc2f` byte for byte (`cmp` against
the trunk's), and `brief.md`, kept here as `blind/a-brief.md` and
`blind/b-brief.md`. The two briefs are identical but for what the compiler
printed (`diff` of the two: that block alone):

- **A**, what the trunk's compiler prints today for `blind/p.hero.txt`:
  `check` silent at exit 0, then `build` at exit 2 with *internal error*,
  clang's text (which happens to ask *did you mean 'math.h'?*) and the
  generated C's path; captured from `heroes build` with `826ddc2f`'s compiler
  (`<scratchpad>/188-blind-src/build.txt`).
- **B**, the coordinator's draft of a refusal at `check`, in the compiler's
  own full form (its gutter measured on a `machine_locked_path` error at line
  1, `    |` and `  1 | `): `error[header_name]` naming C's spelling carried
  into the string and the name it means, at the string, with `fix (certain):
  replace "<math.h>" with "math.h"`. A draft for the experiment, of no route
  in particular, not the sitting's wording (Q2).

The program is `extern "<math.h>"` over `function sqrt(x: f64) -> f64`,
`main` printing `sqrt(2.0)`; its repair `extern "math.h"` builds on this Mac
and prints `1.4142135623730951` (checked). The spec is the status quo in
both, so the message is the one variable.

**What this does not measure** (the critic): no arm stands for route (1d), a
true message at `build` rather than a refusal at `check`; and the experiment
measures the repair after a message, not whether models write `extern
"<math.h>"` at all, which F6 leaves a question.

## The command, per folder

```
claude -p "Read brief.md in this directory and follow it exactly. Your inputs are the files in this directory only. Write report.md here. Answer in English." \
  --model claude-opus-5-5 \
  --restricted --safe-mode --strict-mcp-config \
  --tools "Read,Write" --allowedTools "Read,Write" \
  --disallowedTools "Bash,WebFetch,WebSearch,Glob,Grep,Edit,Task,Agent,ListAgents,SendMessage" \
  --max-budget-usd 1.5 --output-format json > run.json 2> run.err
```

The coordinator copies each `report.md` and `c.hero` into
`docs/panel/188-reports/` with a header saying how it was run (the model the
CLI reports in `run.json`), then builds each `c.hero` with the trunk's
compiler: a one-turn repair is a `c.hero` that builds and prints
`1.4142135623730951`.

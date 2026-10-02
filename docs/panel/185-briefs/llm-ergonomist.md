# Panel 185, the llm-ergonomist (the blind seat)

The seat runs as one fresh session outside the repository, by the panel
skill's command of 2026-10-01 (`.claude/skills/panel/SKILL.md` § 2), from
`<scratchpad>/185-llm-ergonomist/`: no `CLAUDE.md` in or above it, no git
tree. **This is the sitting's one paid run**, named here as the author's
answer of 2026-10-01 named it (*a full panel, five seats and the critic, and
one blind reading*): one session, `--model claude-opus-5-5`, `claude` 2.1.285,
capped at 3 USD by `--max-budget-usd`. The coordinator copies its `report.md`
into `docs/panel/185-reports/llm-ergonomist.md` with a header saying how it
was run.

**What is in the folder**, and kept here as `blind/`:
- `spec.md`: `spec/heroes-spec.md` at `03e70520`, with three markers and one
  ratified change applied, panel 184's R1 (`{{` and `}}` each write one brace,
  a lone `}` an error), so the reading judges the language as it will be;
  `blind/spec-markers.diff` is the whole difference, by `diff` against `git
  show 03e70520:spec/heroes-spec.md`.
- `brief.md`: three markers, their candidates label-stripped and in no
  particular order: A for Q2 (A1 is route (2a), A2 is (2b), A3 is (2c)), B
  for Q3 (B1 is (3a), B2 is (3b)), C for Q5 (C1 is (5a), C2 is (5d), C3 is
  (5c)); the method of panel 184's second readings, with `choice_points` in
  place of a question about the seat's own reasoning.
- `task-a.hero`, `task-b.hero`, `task-c.hero`: the programs each step 2 reads,
  written by the coordinator for this sitting (a binding and a loop after
  `=>`; Q3's shape; a forgotten `f` beside a literal holding JSON's braces).

**What the reading adds, said before it runs** (the critic's D7): C1 and C3
are panel 184's M and L word for word, read blind once already, M approved
and L objected to; only C2, route (5d), is new. Route (5b) is not shown,
since its own blind reading objected and the author's condition sends it
here. And task A asks for an arm that does nothing; spec § 8 names `_ = 0` as
that arm and the compiler refuses `.blue => _ = 0` on the arm's line
(`probes/q2/s06-discard.hero`), a choice point the reading may meet (the
critic's D8), left in on purpose.

Q1 (a C macro) and Q4 (a fix's certainty) are not read by this seat: the
first has no reader-facing text a blind session can weigh beyond a spec
sentence the spec-warden prices, and the second concerns what `--apply`
writes, which the spec does not state.

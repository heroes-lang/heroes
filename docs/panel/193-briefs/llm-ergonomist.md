# Panel 193, the llm-ergonomist's seat: a blind experiment run by the coordinator

The seat runs as fresh `claude -p` sessions outside the repository, in
`<scratchpad>/193-llm-ergonomist/<arm><n>/`, with the command of
`.claude/skills/panel/SKILL.md` (`--restricted --safe-mode
--strict-mcp-config`, tools `Read` and `Write` only) and `--model
claude-opus-5-5`, the model panels 192's arms ran on, never as a subagent.
**It is a paid run and starts only on the author's yes to its budget**: two
arms of five sessions, one after another, each capped at 0.20 USD by
`--max-budget-usd`, and **no session started once the sum of the runs'
`total_cost_usd` reaches 2.2 USD**, since the cap is not shown to be a hard
ceiling (the critic: panel 189's 0.25-capped sessions cost 0.1729 to 0.1794).
At most 2.5 USD by the CLI's report.

**The experiment** (repaired after the critic's first pass): each session
receives a copy of the specification at `00217c39`, the file `case.hero`, and
one `check --json` answer for it, and is asked to write the file with every
`certain` fix applied, in one turn. `case.hero` is 178's case with **every
comment removed**, because the committed case's header describes the fix and
its `#~` marks name each diagnostic. The files are those the
compiler-engineer prints into `<scratchpad>/193-compiler-engineer/blind-json/`.
- **Arm A**: `a.json`, today's answer, each fix `title`, `replacement`,
  `certainty`.
- **Arm B**: `b.json`, the compiler-engineer's route's answer, the same with
  the fix's place.
A session passes where its file equals, byte for byte, `applied.hero`, what
`check --apply` writes for `case.hero`.

**Registered before any session runs** (the coordinator's prediction):
arm A passes at most 2 of 5, its ten fixes *delete the `,`* naming no place;
arm B passes at least 4 of 5. Read as: B's lead of two or more is the
measured effect Principle 0 asks for; a tie says the place does not help a
one-turn reader, and the field rests on tools alone.

The brief each session reads is written beside this file as `blind/brief.md`,
and its scorer as `blind/score.py`, both before the first session runs.

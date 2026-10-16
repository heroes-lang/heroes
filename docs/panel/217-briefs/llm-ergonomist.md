# Panel 217, the llm-ergonomist's seat: two blind arms, run by the coordinator

This seat is an experiment, run as fresh `claude -p` sessions, each in a folder of its own with
`git init` under the coordinator's scratchpad, outside the repository (`/panel` § 2's rule; panel
215's readers ran inside it, under `.claude/worktrees/readers2/`), by panel 215's launcher with one
flag added, `--model claude-opus-5-5`, the model 215's readers ran on, pinned
(`docs/panel/215-reports/llm-ergonomist-work-2/run_one.sh`: `--bare --restricted
--strict-mcp-config --tools "Read,Write"`, `--max-budget-usd 0.7`), five at a time. **Its size and
bound, named here as the paid-run rule asks**: two arms of ten sessions, each session bounded at
0.70 USD, so at most 14.00 USD by the bound and about 4.4 USD expected by panel 215's re-run, whose
ten sessions cost 2.2106 USD; the author's budget for the evening's sittings is 10 USD, so the
coordinator stops before a third arm if the two have spent more than 7.

- **Arm R2, the second try**: each reader gets the specification `m-g-at`, `counter.h`, task C's
  words, **its own first program from panel 215's re-run** (`r<NN>/c.hero.txt`) and **the lane's
  compiler's messages on it** (`readers-r2/r<NN>/build-r2.txt` in the coordinator's scratchpad), and
  is asked for the program again. Judged by building it with the compiler at `f57af53da`'s source
  (two stages, as the shared brief says) and the kit's `judge.sh`. **Scores** panel 215's
  llm-ergonomist's *at least 8 of 10 correct on task C's second try*.
- **Arm F1, the first try under the form**: the spec-warden's draft over `m-g-at`, `counter.h` and
  task C's words, nothing else; judged with the compiler-engineer's F1 prototype. **Scores** the
  same seat's *with a read-only lend, at most 2 of 10 copy the lock first try*, and the
  spec-warden's registered prediction.

**Scored on a person's table, not on `judge.sh` alone**: for every program, whether it builds and
prints `6`, `4`, `5`, its helper's signature (a value, `@c`, the form), and whether it copies a
lock; `judge.sh` scores F0's `doubled(@c)` correct and cannot tell the forms apart.

Every reader writes `notes.md` with every place the specification left a choice, the choice made
and what the other would produce, and says `context: yes` or `no`. The coordinator copies each arm's
outputs into `docs/panel/217-reports/llm-ergonomist-work/` and its report into
`docs/panel/217-reports/llm-ergonomist.md`, with how it was run.

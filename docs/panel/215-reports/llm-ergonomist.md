# Panel 215, the llm-ergonomist's experiment: ten blind readers, three tasks

Run by the coordinator on 2026-10-14 between 08:25 and 08:40, on the author's budget of that morning
(meant as: *you can run paid tests, I give you ten dollars*). The seat is the experiment, as
`215-briefs/llm-ergonomist.md` (corrected after the critic's pass 1) describes it. Everything below is
read from the ten runs' outputs, kept in `llm-ergonomist-work/` (inputs, each reader's programs,
notes, build transcripts and the JSON of its run with its cost), and every program was built and run
with the sitting's compiler at `7e5c02048`.

## How it was run, and what reached the readers

- **Ten `claude -p` sessions**, one per reader, each in its own folder `.claude/worktrees/readers/r<NN>/`
  (inside the repository's root, ignored) after `git init`, with `--bare --restricted
  --strict-mcp-config --tools "Read,Write" --allowedTools "Read,Write" --max-budget-usd 0.7`,
  authenticated by `ANTHROPIC_API_KEY` from `.env` (`llm-ergonomist-work/run_one.sh`). Models, from
  each run's `modelUsage`: `claude-opus-5-5`, which wrote the answer (4,988 to 7,446 output tokens a
  run), and `claude-haiku-4-5-20251001`, the CLI's own call of 923 input and 11 or 12 output tokens
  at 0.001 USD a run, too small to have held the brief or the specification.
- **Cost**: 1.9844 USD for the ten (from each run's `total_cost_usd`, 0.1685 to 0.2176 each).
- **The Write tool did not reach the sessions** under `--bare`: every reader said so and gave its
  three programs and its notes in its answer, from which the coordinator extracted them unchanged
  (reader r03 wrote a placeholder for A1 and then *Correction: a1.hero should contain exactly this*;
  its corrected program is the one scored).
- **`context: yes` from all ten, and what reached them is the environment's preamble alone**: that
  they are a Claude agent, the working directory's path (`.../heroes-lang/.claude/worktrees/readers/r<NN>`,
  read as a git worktree), the date and, for five of them, the rules for calling a tool. **All ten**
  say in words that no memory and no repository status reached them, and none names a project rule
  or contract (`grep -i -A4 context r*/notes.md`, read whole). The reading stands. The path names the
  project, whose name the specification's first line gives anyway.

## The results

| task | first try correct | what it scores |
|---|---|---|
| **A1**, a path into `char path[108]` | **10 of 10** build and print 16 | the field bound `path: i8[108]` by **10 of 10**, `u8` by **0 of 10**; the path written byte by byte, `sp.path[i] @ text[i].to_i8().must()` by seven and `to_i8(text[i]).must()` by three (`grep -n path r*/a1.hero`) |
| **A2**, a record whose member `via` is a union with no tag | **1 of 10** (r08, the record bound `partial`) | the other **9** declared the union's members `fd` and `handle` as the record's fields and were refused `ffi_unknown_field`; **all ten** constructed it with `rest: zero` and **none** wrote `u8` (`grep -c "rest: zero" r*/a2.hero`, `grep -l u8 r*/a2.hero`) |
| **B**, a counter holding a `pthread_mutex_t` | **10 of 10** build and print 3 | **0 of 10 copy the mutex**: every reader passes the counter to its function as `@c: Counter`, by address; no by-value parameter, no `=` of the record, no return of one (`grep` over the ten `b.hero`, read) |

## What it scores

- **This is not panel 194's registered test, and it scores that test's prediction only in part**
  (the spec-warden of this sitting found it; read in `194-reports/spec-warden.md` line 240 and the
  synthesis's table). Registered: at least 10 fresh readers per arm, three arms (L, *R1 honest*, *R1
  honest with K*), panel 178's Darwin `utsname` task and one task holding a union, every program
  compiled; predicted under R1 with K *at least 8 of 10 correct first try and at most 2 of 10 writing
  `u8`*; to object *if `u8` reads 3 or more of 10 under R1 with K*, or on a MemorySanitizer finding.
  Run here: **one arm**, R1 with K (the landed specification), on this sitting's tasks, not the
  `utsname` one. What it reads on that arm: **`u8` 0 of 10** on A1, the task with a C `char`, so
  the objection's `u8` half is not met; **correct first try 10 of 10 on A1 and 1 of 10 on A2**, so
  on the task holding a union the prediction's first half fails. Every A2 miss is the union's
  members declared as the record's fields, told by defect 2062's false message; every A2 reader
  wrote R1's `rest: zero`, and A2's record holds no `char`: the miss is the union binding and its
  message, not R1 or K. The registered test itself stays unscored, the L and *R1 honest* arms unrun.
- **Issue 1333's route T**, *returns if writing a path fails first try at least half the time*: **0
  of 10 failed; T does not return.**
- **Q2**: given a counter holding a mutex and the specification as it stands, **no reader copied
  it**. The risk issue 1335 measured is real (a copied locked mutex locks twice at exit 0), and these
  readers did not write it.
- **A finding outside the three questions, filed as defect 2062**
  (https://github.com/heroes-lang/heroes/issues/2062, blocking): the nine A2 readers were told
  *`union.h` declares no `fd` in `struct tagged`*, with a note proposing a spelling or another header,
  where the header declares `fd` inside the union `via` and what builds is `partial`. The specification
  says a group record may be `partial`; nothing in the message pointed there, and only one reader in
  ten found it unaided.

## The verdict

**Approve R1 as landed** (Q3) on what this arm reads: no reader wrote `u8`, every reader wrote
`rest: zero`, and the one task most readers failed failed on the union's message. That is not panel
194's instrument, which stays owed: run as registered (three arms, the `utsname` task and a union
task) once defect 2062 is repaired, so the union task measures R1 and K and not a false message;
about thirty readers, near 6 USD at this run's 0.17 to 0.22 a reader, a paid run for the author.
**Object to a Q2 mark entering the
language on this evidence alone**: no reader wrote the copy, so a mark's thesis effect is unmeasured
by this experiment; a refusal of the copy would need the copy to be written to be measured. **No
reading of Q1**: padding is invisible to a reader by construction, and no task here could measure it.
**Object to today's message for a union member** (defect 2062): it is the one place in the three tasks
where readers failed, and they failed because the message sent them away from the fix.

## Predictions

- After defect 2062's repair, the same A2 task with the repaired message shown after a first failed
  build reaches at least 8 of 10 correct on the second try. Scored by a rerun of A2 with the message,
  at the repair.
- A1 and B read the same on the next ten readers (at least 9 of 10 and 0 copies). Scored by the next
  paid reader run that includes them.

## What was not run

The arms L and *R1 honest*; a second try on A2 with the compiler's message shown; any reader on
Windows' or Linux's own clang (the programs were built on this Mac only); a task where a copy of the
mutex is the natural code.

## Corrected 2026-10-14 at about 11:10, after the critic's pass 2

Three sentences above are false, found by the completeness critic (`critic-reports.md` § 1) and
checked by the coordinator in `run_one.sh` (`--tools "Read,Write"`): **the readers had no
compiler**, wrote each program once, and the coordinator built them afterwards. So no reader *met*
`ffi_unknown_field` or *was told* anything by it: *"were refused `ffi_unknown_field`"* in the table,
*"told by defect 2062's false message ... the miss is the union binding and its message, not R1 or
K"* under *What it scores*, and *"the nine A2 readers were told"* and *"the one task most readers
failed failed on the union's message"* in the finding and the verdict are each the coordinator's
build, not the readers' first try. What decided their first try, by their own notes, is the
specification: three quote § 13's union sentence (*named or anonymous*), four rejected the `partial`
binding because R1's sentence does not say it zeroes what the record does not declare. And the
message is true of C (`clang`: *no member named 'fd' in 'struct tagged'*), so defect 2062's
*false message* premise is corrected on its issue the same day. What stands: the scores (A1 10 of
10, `u8` 0 of 10; A2 1 of 10; B 10 of 10, 0 copies), and A2's 1 of 10 is a miss of panel 194's
registered first-try threshold on its *one task holding a union*, under R1 with K, whose cause the
readers' notes put in the specification's text. The verdict above to approve R1 *as landed* is
withdrawn; what this seat's evidence supports is the critic's open question, for the synthesis.

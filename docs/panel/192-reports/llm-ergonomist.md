# Panel 192, the llm-ergonomist's seat: the blind experiment, run and scored

Run by the coordinator, 2026-10-04, from 18:16 to 21:10 by the clock, as fresh
`claude -p` sessions outside the repository (the seat's brief,
`docs/panel/192-briefs/llm-ergonomist.md`, and the arms' diffs beside it). The
sessions' reports are copied whole, with each session's cost, models and
scorer line, in `llm-ergonomist-a.md`, `-r.md`, `-b.md` and `-c.md` beside this
file.

## The result

| arm | one sentence differing from A | passes | spelling of ESC in every passing program |
|---|---|---|---|
| A | none: the specification at the base | **5 of 5** | C's `putchar` through `extern "stdio.h"`, every byte |
| R | `:384`: a `[u8]` answers `validated_bytes()` | **5 of 5** | `[27]` with `validated_bytes()` |
| B | `:47-48`: `\u{...}` | **5 of 5** | `\u{1b}` |
| C | `:47-48`: `\xNN`, `01` to `7f` | **5 of 5** | `\x1b` |

Every program passes, and every one used its arm's route: no B program used
`[u8]`, no C program used `\u{1b}`, and no program held a raw ESC. The scorer
built each with the base's compiler (sha1 `8084f018f5387536`) and read the
bytes it printed: `1b 5b 33 31 6d 45 52 52 4f 52 1b 5b 30 6d 0a` or a variant
the rule admits.

## Against what was registered before the first session

- **A**: *at most 1 of 5* registered, **5 of 5** read. **The prediction is
  false.** Today's specification already lets a model write ESC in one
  turn, through C.
  - The five A sessions' `choice_points` name the raw byte as an option and
    turn it down: *invisible in source and fragile to editors* (a2), *would
    leave an invisible byte in the source* (a3).
  - They also read from the specification that `\x1b`, `\e` and `\033` are
    compile errors.
- **R**: *at least 3* registered, 5 read.
- **B** and **C**: *at least 4* each registered, 5 each read.

**The rule from scores to a reading**, as registered:
- **B and C both at 4 or 5**: the choice between `\u{...}` and `\xNN` rests
  on Q3's other grounds (one spelling, the NUL, half a character), not on
  this experiment.
- **R at 4 or 5**: route (a′) serves a writer from the specification alone,
  so **a new escape needs another reason than writability**.
- **A at 2 or more**: today's specification serves a writer more than
  predicted. It does so through the FFI, which design.md §1.11 calls the
  language's founding constraint.

## The `context` answers

All twenty name `brief.md`, `spec.md`, and the harness's own system prompt
(environment details and an account e-mail). Under the threshold registered
before the first session, the harness's prompt is not a yes, so **no reading
is void**.

## How it ran, and where it departed from the brief

- **The trial.** `a1` ran first, alone (18:16:43 to 18:17:45), to check the
  command. It cost 0.2348 USD against its 0.24 cap. So the other sessions ran
  at **`--max-budget-usd 0.25`**, so that no session would be cut by a cap it
  nearly reached.
- **One at a time, not three.** The sessions ran one at a time, in the order
  a, r, b, c, then the next number. Each was launched only if the cost
  already spent plus 0.32 USD stayed within the author's 5 USD
  (`<scratchpad>/192-blind-run-all.sh`).
- **The scorer's description.** After `a1`, the scorer's description of a
  spelling gained the label *C through extern* (`a1`'s route, until then
  *none found*). The pass rule was not touched, and it was tested on twelve
  known outputs and on F10's program before any session ran.
- **The account's session limit.** It stopped `r5`, `b5` and `c5` at 18:30,
  before any wrote a file (`r5` cost 0.0201 USD, the other two nothing). They
  were run again from 21:08 under new names, `r5x`, `b5x` and `c5x`, in
  fresh folders with the same two files (`cmp`). The scorer takes a re-run
  only where its original wrote no `c.hero`.
- **The cost.** 3.9035 USD in all, the three stopped sessions included,
  within the author's 5 USD. Per session, from `run.json`:
  - A 0.2215 to 0.2387;
  - R 0.1936 to 0.2114;
  - B 0.1738 to 0.1789;
  - C 0.1663 to 0.1753.

  An arm with a documented route cost less to write.
- **The models.** The CLI (2.1.285) reports `claude-opus-5-5` for every
  session, and `claude-haiku-4-5-20251001` beside it in every session's
  `modelUsage`.

## What this does not measure

- a comment;
- a character other than ESC;
- a model reading a line a bidirectional control reorders (Q2's reader);
- a NUL;
- today's `unknown_escape` message, which a writer meets after the
  specification;
- a person's editor.

Five sessions an arm cannot tell B from C when both reach the ceiling, and
they did.

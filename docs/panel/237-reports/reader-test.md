# Panel 237, the reader test's results

Written by the coordinator from `blind237.py grade` (lane `p237`'s compiler, `sources 0d7a6741345899d8`, SQLite 3.54.0, the critic's shim, the store's and the journal's headers), after every session ran: R1 and R2 from 12:02:17 to 12:34:59, R3 from 12:35 (its registration committed at 15a9b28d7, 12:33) to 12:54:33. **The CLI's reported cost: 13.1583 USD** for the blind seat and the 216 sessions (`blind237.py sum`), within the author's cap of 25 USD; the spec-warden's twelve `count_tokens` requests are in its report. No session failed. The graded table, one row per session with its first error or what it printed, is `build/b237/graded/table.tsv` in lane `p237`, kept in the sitting's evidence.

## The rule's lines, as `grade` printed them

```
R1 A correct 6 silent 0 loud 0 refused 12 failed 0 unknown 0 | when 5 v2 13 bare 0 none 0
R1 B correct 6 silent 0 loud 0 refused 12 failed 0 unknown 0 | when 9 v2 9 bare 0 none 0 | p(silent lower) 1.0000 p(correct lower) 0.6377 not preferred
R1 C correct 8 silent 0 loud 0 refused 10 failed 0 unknown 0 | when 3 v2 15 bare 0 none 0 | p(silent lower) 1.0000 p(correct lower) 0.8474 not preferred
R1 D correct 6 silent 0 loud 0 refused 12 failed 0 unknown 0 | when 4 v2 14 bare 0 none 0 | p(silent lower) 1.0000 p(correct lower) 0.6377 not preferred
R2 A correct 10 silent 0 loud 0 refused 8 failed 0 unknown 0 | when 18 v2 0 bare 0 none 0
R2 B correct 10 silent 0 loud 0 refused 8 failed 0 unknown 0 | when 18 v2 0 bare 0 none 0 | p(silent lower) 1.0000 p(correct lower) 0.6310 not preferred
R2 C correct 9 silent 0 loud 0 refused 9 failed 0 unknown 0 | when 18 v2 0 bare 0 none 0 | p(silent lower) 1.0000 p(correct lower) 0.5000 not preferred
R2 D correct 9 silent 0 loud 0 refused 9 failed 0 unknown 0 | when 18 v2 0 bare 0 none 0 | p(silent lower) 1.0000 p(correct lower) 0.5000 not preferred
R3 A correct 9 silent 0 loud 0 refused 9 failed 0 unknown 0 | when 0 v2 0 bare 18 none 0
R3 B correct 8 silent 0 loud 0 refused 10 failed 0 unknown 0 | when 0 v2 0 bare 18 none 0 | p(silent lower) 1.0000 p(correct lower) 0.5000 not charged
R3 C correct 8 silent 0 loud 0 refused 10 failed 0 unknown 0 | when 0 v2 0 bare 18 none 0 | p(silent lower) 1.0000 p(correct lower) 0.5000 not charged
R3 D correct 9 silent 0 loud 0 refused 9 failed 0 unknown 0 | when 0 v2 0 bare 18 none 0 | p(silent lower) 1.0000 p(correct lower) 0.6302 not charged
```

The runner's last three lines (`arm B preferred on 1 of 3 tasks` and the same for C and D) count R3's *not charged* as a preference, a fault of the summary line and not of the rule: **no arm is preferred over A on R1 or on R2, and no arm is charged on R3.**

## What it measured

- **Silent: 0 of 216**, in every arm and task. Every program that built and exited 0 left nothing open. On R2 all 72 bound the store's close `when 0`, in every arm, A and C included; on R1, all 72 mark `sqlite3_close` `when 0`, 51 of them binding `sqlite3_close_v2` beside it (the runner's label reads `v2` first, which the critic's second pass found: `critic-reports.md`, finding 1); on R3, all 72 bound the journal's close with no `when` (no over-application, in any arm).
- **The registered rule decides nothing**: with each task documenting its library's close, the fence's reading moved no first-try outcome measurably. By the README's rule the sitting rules on item 3 on the other seats.
- **The first error of the refused programs**: `lent_shape` on a handle parameter in 111 of the 118 refusals (R1 44, R2 31, R3 36), the rest `expected_args_close` (5), `unread_releaser` (1) and `never_rebound` (1). Of R1's and R2's 75 `lent_shape` refusals, deleting `lent` after each handle type and building again ran 66 right (`build/b237/refix/result.tsv`): defect 2347, filed.
- **Predictions scored**: the blind seat's (silent rates of about 40 % under A, about 45 % under C on the store task): falsified, 0 of 18 in each. The spec-warden's P2 (B preferred over A on R2, C not): falsified, no arm preferred. The historian's (C not preferred over A on R2): held.

## Corrected on the critic's second pass, 2026-11-08

**The test could not separate the arms, and this report's first reading said more than it shows**
(`critic-reports.md`, finding 1). Step 5 of R1 and R2 tells the reader that the close can refuse and
asks for a second close; a bare binding cannot compile that second close, a `when 0` binding that
skips it aborts at `main`'s end, and `sqlite3_close_v2` always closes. So a reader who followed its
task could not come out silent, every arm's p was 1.0000 before any session ran, and the bindings
followed each task's documentation, not the fence. **The result is silent on item 3**; it is not
evidence that the fence does not matter. What would measure defect 2322's shape is a task that does
not name the refusal: R4, registered in `blind/README.md` before it runs.

**And the `lent` refusals are a contradiction, not only a missing `Fix`** (finding 2): spec section 13
says a group's record declared `lent` without `@` is a pointer to const, and a handle is a group's
record with a `tag` and no fields; the compiler refuses `lent` on a handle on panel 171's ruling,
which the specification never took in. Sonnet 5.5 built 0 of its 72 first tries, so each arm's
`correct` count rests on Opus's and Haiku's twelve sessions per task.

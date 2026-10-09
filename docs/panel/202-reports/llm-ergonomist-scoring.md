# Panel 202, llm-ergonomist (blind seat): the coordinator's scoring

Written by the coordinator at 23:48 on 2026-10-09 (`date`). Eight fresh
sessions, `claude-opus-5-5`. A first run at 23:31 stopped on the account's
session limit with no report and 0.3734 USD spent (its `run.json` files kept
under `.claude/worktrees/scratch-b15/blind-202-203/failed-2331/`); the second,
on the new account, from 23:46:42 to 23:48:04, 2.0552 USD (0.2589, 0.2428,
0.2945, 0.3054, 0.2473, 0.2292, 0.2651, 0.2120). With panel 203's still to
run, of the 6 USD the author approved 2.4286 are spent. Every `context` names
the folder's files and the harness's environment (the account's email among
it), no project rule; every reading stands.

| folder | task | the reader's answer |
|---|---|---|
| `d1-a`, `d1-b` | predict `fp` (two modules, `static inline twice` of two types) | both: `check` 0, `build` 0 printing 6 and 8, **`test` 0**; today `test` exits 1 |
| `d2-a` | predict `clash` (one C symbol, two prototypes) | `build` **refused**, *conflicting types for 'twice'*, as one unit would tell it; today it builds |
| `d2-b` | the same | `build` 0, run prints **`6`, `4.0`, `10`**: the wrong value today's compiler prints, foreseen as C's behaviour |
| `d3-a`, `d3-b` | repair `fp` given `test`'s message (defect 538's) | both rebind `right.hero` to `a.h`, the note's first remedy; `b.h` untouched; output kept |
| `d4-a`, `d4-b` | repair `onedef` given `build`'s exit 2 | both make `c.h`'s definition `static inline`; neither reads the exit 2 as the compiler's fault |

**Against the prediction** (`llm-ergonomist-prediction.md`): clause 1 held
(both d1 readers predict `test` passes, more strongly than written); clause 2
was **falsified** by `d2-b`, which foresaw the wrong value; clause 3 held in
its first half and its second (one reader editing `b.h`) was falsified; clause
4 held in its first half and its second (one reader blaming the compiler) was
falsified; clause 5 held.

**What it says.** A reader of the spec expects each module's C to stand alone
(2 of 2 predict `test` passes where it refuses), so a tool that fuses units
tells a program the reader believes correct that it is wrong, and the readers
then edit a correct program to satisfy it (d3, both). The one-C-symbol-two-
prototypes program is read either as a refusal or as C's silent wrong value,
never as a Heroes diagnostic: nothing the reader has tells them the value is
wrong. An exit 2 on a duplicate symbol is repaired in the header, correctly,
without the compiler's blame being believed. Two readings per cell; no arm
compared two texts of the spec.

**Corrected at 00:10 on 2026-10-10** (`date`), on the critic's second pass (its
finding 3): the table's d3 row is wrong for `d3-a`. `d3-a` did not rebind
`right.hero` to `a.h`; it wrote `use left` and called `left.twice(...)`,
which `check` refuses at exit 1, `extern_across_modules`. `d3-b`'s repair
(rebinding to `a.h`) passes every verb. So today's message led 2 of 2
readers to edit a correct program, and one of the two edits does not compile.

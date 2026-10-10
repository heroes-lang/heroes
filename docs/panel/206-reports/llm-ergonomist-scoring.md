# Panel 206, llm-ergonomist (blind seat): the coordinator's scoring

Written by the coordinator at 09:25 on 2026-10-10 (`date`). Ten fresh
sessions, `claude-opus-5-5`, from 09:23:20 to 09:24:39, 2.3717 USD (0.3698,
0.2523, 0.2734, 0.3579, 0.1841, 0.1887, 0.1870, 0.1876, 0.1783, 0.1926) of
the 3.69 approved within the night's 30. Every `context` names the folder's
files and the harness's environment, no project rule; every reading stands.

| program | the round | n1 (base), 2 readers | n2 (base and F7e), 2 readers |
|---|---|---|---|
| `p1` `y: u8 = 200 + 100` | builds, aborts 134 | builds, aborts, 2 of 2 | refused, 2 of 2 |
| `p2` `repeat("-", 0 - 1)` | builds, aborts 134 | builds, aborts, 2 of 2 (the count's type unstated: `n1-a` takes it as `i64`, `n1-b` reads `0 - 1` as an `i64` -1) | builds, aborts, 2 of 2 (F7e read as not reaching it) |
| `p3` constant `BIG: u8` of `200 + 100` | builds, aborts 134 when read | **refused at compile time, 2 of 2** (*a written body computes over literals and other constants* read as computed by the compiler) | refused, 2 of 2 |
| `p4` `x: u8 = 255 + 1 - 1` | builds, aborts 134 | builds, aborts at `255 + 1`, 2 of 2 | refused, 2 of 2 |
| `p5` `x: u8 = 2 - 3 + 5` | builds, aborts 134 | builds, aborts at `2 - 3`, 2 of 2 | refused, 2 of 2 |
| `p6` `y: u8 = 300 - 100` | refused `int_out_of_range` | refused, 2 of 2 | refused, 2 of 2 |

| folder | task | the reader's program |
|---|---|---|
| `w-a` to `w-f` | a constant `ALL_ONES: u64` with every bit set and `LOW_BYTE: u8` with its 8 bits set | 6 of 6: `0xffff_ffff_ffff_ffff` and `0xff`, no arithmetic |

**Against the prediction** (`llm-ergonomist-prediction.md`): n1's first half
held (both predict `p1` aborting) and its second was falsified (neither
predicts `p5` printing 4: both read each step at the width, as the round
computes it); n2 held for `p1`, `p3`, `p4`, `p5` and was falsified on `p2`
(neither reads F7e as reaching `repeat`'s count); w was falsified: 0 of 6
write literal arithmetic that cannot fit, the spec-warden's P3 (*at least 1
of 4*) unmet.

**What it says.** Today's spec, at 0 tokens, predicts the round in 5 of 6
programs, 2 of 2 readers, per step at the width: the run-time abort is what
§ 7 says. The one disagreement is the constant: 2 of 2 readers expect a
constant whose written body cannot fit to be refused when the program is
compiled, and the round aborts each time it is read. With F7e the readers
predict the refusal it describes on the four programs it names. In writing,
the mistake a refusal would catch was made by 0 of 6 readers asked for every
bit set, at this size.

# Panel 232, the blind seat: what a reader of the specification alone predicts

The coordinator's experiment, run as `blind/README.md` registered it at
5b90ee119 before its first session, and read here by the coordinator: the
seat has no agent of its own, its sessions are its evidence. Scored by
`score.py` before the key was opened; the key joined after.

## The run

80 sessions, 4 tasks × 2 specifications × 2 models × 5, each in its own
folder under the lane's `build/b232/<n>`, run by `run_one.sh` from inside it
(`claude` 2.1.308, `--bare --restricted --safe-mode --strict-mcp-config`,
the `Read` tool alone, `--max-budget-usd 0.14`). The first session of each
model ran alone and its cost was read before the others: Haiku 0.0025 USD,
Sonnet 0.0368 USD, both in shape. All 80 exited 0. **Spent: 1.5590 USD** of
the 11.20 registered and the author's 12.

**The context**: no session called a tool but `Read`, and none outside its
own folder (0 of 80); 71 `context` sections read `none` and 9 read `none`
followed by a remark on the specification in their folder (*the spec does
not state the order in which operands and record fields are evaluated*, and
the like). None names a project file, rule, memory or commit: **no session
is void**.

## Registered score: the answer section equals the truth exactly

| task | T right of 10 | P right of 10 | P minus T |
|---|---|---|---|
| O, an expression's order | 6 | 10 | +4 |
| S, `<<` past the sign bit | 3 | 5 | +2 |
| X, `exit` with a literal 256 | 0 | 5 | +5 |
| W, a write's two phases | 5 | 8 | +3 |

By model, T then P: O Haiku 3 and 5, Sonnet 3 and 5; S Haiku 3 and 2,
Sonnet 0 and 3; X Haiku 0 and 3, Sonnet 0 and 2; W Haiku 5 and 5, Sonnet 0
and 3.

**The spec-warden's predictions, scored on the registered rule**: O P 10,
hit; O T 9, **missed** (6, below the band's 8); S P 9, **missed** (5, below
7); S T 1, hit (3, not above 3); X P 9, **missed** (5, below 8); X T 0, hit;
W P 9, hit (8, not below 8); W T 4, hit (5, not above 6). The effects: O at
most 1, **missed** (+4); S at least 5, **missed** (+2); X at least 7,
**missed** (+5); W at least 3, hit (+3).

## What the misses are, read one by one

Of the 38 answers the registered rule counts wrong, **25 hold the truth's
lines exactly, followed by prose inside the `answer` section** (*the spec
does not say ... I assumed left to right*, *`3 << 62` sets bits 62 and 63*):
the brief asked for the lines and nothing else, and the rule counts the
prose a miss. **13 hold wrong lines**: ten `before`, `ends: abort` on X
under T (a run-time abort, which T's § 11 says); one `ends: abort` on S
under T; one O under T printing the values in place of the labels; one O
under T answering that the order is not determined (a hedge).

**Read post hoc, a reading the registration did not name**: an answer is
right where its first lines are the truth and what follows is prose; a hedge
or another line stays wrong. Computed by the same script over the same
answers (`answers/<n>.txt`), the key joined after:

| task | T right of 10 | P right of 10 | P minus T |
|---|---|---|---|
| O | 8 | 10 | +2 |
| S | 9 | 10 | +1 |
| X | 0 | 10 | **+10** |
| W | 10 | 10 | 0 |

## What it says of each sentence

- **§ 11's `a literal one is refused`** (item 5) is the one sentence that
  changes what a reader predicts: under T no session of 10 predicted the
  refusal on either reading, every one a run-time abort, which T's sentence
  says; under P every one did, read post hoc. The compiler refuses since
  step 1, so today's specification misleads every reader on this program.
- **The order of evaluation** (item 1): under T, 8 of 10 already predict
  left to right on O and 10 of 10 the write's two phases on W, read post
  hoc; several say so in their own words (*the spec does not state ... I
  assumed left to right*). The sentence settles what readers already
  assume and say they assume; its measured effect is one misreading and one
  hedge on O.
- **`<<` wraps** (item 2): under T, 9 of 10 predict the wrap, read post hoc,
  several noting the specification does not say whether a shift can
  overflow; the sentence turns an assumption into a statement, +1.

What this seat cannot say: whether a model reading the whole specification
in a longer task, writing rather than predicting, behaves the same; and the
post hoc reading is a reading chosen after the answers were seen, reported
beside the registered one and never in its place.

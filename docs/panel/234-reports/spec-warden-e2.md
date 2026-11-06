# Panel 234, the spec-warden: predictions for E2, the card's form

Registered 2026-11-06 at 01:11 CET, before any session of E2
(`docs/panel/234-briefs/blind/README.md`, its last section). At that minute
`build/b234-out/` held 454 files, the `.jsonl` and `.err` of the first run's
227 sessions, the newest written at 00:19; E2's 60 first turns stood prepared
in `build/b234/228` to `287` with their key appended (`sealed/key.tsv`, 288
lines), and none had run. Read only, this file apart; no API call, no compiler
run. design.md §1.6, reached by grep today: **16384** tokens on `claude-opus-5`.
E2 moves no token of the specification: its 60 folders hold the first run's
`heroes-spec.md`, md5 `dd2417d8...`, the same bytes as folder 014's.

## What the predictions rest on

**E2's inputs, checked by hash without reading which folder holds which arm**:
the 60 briefs are one file, the first run's card-H brief (md5 `37adaee6...`,
folder 014's); the 60 cards are 30 of `S/card-H.md` (`f548bba2...`) and 30 of
`S/card-H-call.md` (`ef18bc8b...`); the key's 60 new rows are 5 per arm, task
and model, experiment `E`. **So decl-H is the first run's card-H arm drawn
again, byte for byte, and its prior is that arm.**

**The first run's card arms, first turns, split by arm** (G8 pools G and H).
Counted here, 2026-11-06, by the critic's rule
(`.claude/worktrees/234-critic-scratch/r2-blind/labels.py`), with each first
turn's `error[...]` codes from `sealed/grades.jsonl` and its second turn found
by `parent` in `sealed/key.tsv`, rows 001 to 227 only:

| arm | model | right@1 | package calls labelled | answers labelling all or none | right@2 |
|---|---|---|---|---|---|
| card-H | Sonnet | 2/15 (S1 2, S2 0, S3 0) | 5 of 28 | 15/15 | 14/15 |
| card-G | Sonnet | 4/15 (S1 1, S2 2, S3 1) | 6 of 25 | 15/15 | 15/15 |
| card-H | Haiku | 15/15 | 27 of 27 | 15/15 | 15/15 |
| card-G | Haiku | 14/15 | 28 of 28 | 15/15 | 14/15 |

Three facts carry the model below:

- **Sonnet writes a mode, not a call.** Each of its 30 card answers labelled
  every two-parameter package call or none. The 6 that labelled were right at
  the first turn, 6 of 6. Of the 24 that did not, 23 were refused for
  `needs_label` alone and were right at the second turn; the 24th, folder 114
  (card-H, S2), was refused for `builtin_name_taken` alone, a counter named
  `keys`, and its two positional calls were told only by its second turn's
  build (folder 206, `needs_label`), so it is not right@2. With a card,
  Sonnet's one first-turn slip at these tasks was the label, and one `keys`.
- **Haiku labelled all 55** of its card calls; its one card miss is a logic
  slip, silent (folder 178, card-G, S2).
- **No card answer called `position`** (0 of 60): every S1 and S2 answer of
  both card arms cut with `split_once` or `cut` (20 of 20 an arm). 10 of the 60
  called the built-in `slice(from:, to:)`, all in S2 to trim by hand, every
  call labelled and none refused for one.

And the critic's count, not re-run here (G8): with no card, Sonnet labelled 18
of its 22 calls to a function with two parameters of one type.

## The rule that scores them

- **The scored unit is a share of a cell's answers per model, out of 5**: 12
  cells, each on `right@1` and `right@2`, 24 scores. The first run pooled the
  two models in a cell and so averaged Haiku's gain with Sonnet's loss (G8);
  E2's question is Sonnet's. The pooled cells, out of 10, are registered beside
  them for the reader of the first run's tables and **are not scored**, each
  being the sum of two scored cells.
- **The grade is the README's**: `right@1`, `right@2` (which includes
  `right@1`) and `silent` as it defines them, a case right only at exit 0; a
  void session leaves the denominator; a session the budget stops stays in it,
  not right.
- **A cell or a share is a point and a band**: a hit if the measured share lies
  in the band, its ends included, shares compared as exact fractions (3/15 is
  0.2).
- **An effect is call-H's share minus decl-H's**: *per model* over the three
  tasks, 15 a side; *pooled* over tasks and models, 30 a side; a hit if the
  difference lies in its band, ends included.
- **The label share is the critic's rule, unchanged**: the package's functions
  with two or more parameters of one type, read from
  `S/core-H/core/strings.hero` (twelve, `replace_all` and `matches_at` among
  them); every `strings.<name>(` in a first turn's program; labelled where its
  first argument opens with a name and a colon; first turns only, void
  sessions out. `labels.py` reads the key's rows marked `S` and E2's are marked
  `E`: its filter widens, its rule does not.
- **What I expect of myself**: under the calibration at the end, about 22 of
  the 24 cell bands and 6 of the 7 effects hit. **Fewer than 18 cell hits says
  the failure model is wrong**, not unlucky. Five answers at a ceiling say
  little, so the Haiku bands are wide; the scores a null result misses
  together are six: the three call-H Sonnet `right@1` cells, the Sonnet and
  pooled `right@1` effects, and call-H Sonnet's label share.

## The failure model, in one paragraph

Sonnet sets a mode per answer, every package call labelled or none, and a card
written as declarations sets it to *none* about four times in five (6 of 30
labelled). A card that writes each call as the language requires puts the
labelled call on the line Sonnet reads: I expect the mode set to *labelled*
about nine times in ten, with one chance in seven of a partial fix, the
declaration being still on that line. A Sonnet answer that labels is right
about 0.95 at S1 and S3 and 0.88 at S2, where `keys` is the natural name of the
counter the task prints. Haiku labels everything under either card and stands
at a ceiling in both arms; the call card's `position(s: line, substr: ": ")`
is S1's own separator and may send a few S1 answers through `position` and
`slice`, a route no first-run card answer took, priced at 0.02 to 0.03 of S1's
`right@1`.

## The cells

Per model, out of 5, **scored**:

| arm | task | model | `right@1` | `right@2` |
|---|---|---|---|---|
| decl-H | S1 | Haiku | 0.95 [0.8, 1.0] | 0.95 [0.8, 1.0] |
| decl-H | S2 | Haiku | 0.85 [0.6, 1.0] | 0.9 [0.8, 1.0] |
| decl-H | S3 | Haiku | 0.95 [0.8, 1.0] | 0.95 [0.8, 1.0] |
| decl-H | S1 | Sonnet | 0.2 [0.0, 0.4] | 0.95 [0.8, 1.0] |
| decl-H | S2 | Sonnet | 0.2 [0.0, 0.4] | 0.85 [0.6, 1.0] |
| decl-H | S3 | Sonnet | 0.2 [0.0, 0.4] | 0.95 [0.8, 1.0] |
| call-H | S1 | Haiku | 0.9 [0.8, 1.0] | 0.95 [0.8, 1.0] |
| call-H | S2 | Haiku | 0.85 [0.6, 1.0] | 0.9 [0.8, 1.0] |
| call-H | S3 | Haiku | 0.95 [0.8, 1.0] | 0.95 [0.8, 1.0] |
| call-H | S1 | Sonnet | 0.8 [0.6, 1.0] | 0.95 [0.8, 1.0] |
| call-H | S2 | Sonnet | 0.7 [0.4, 1.0] | 0.9 [0.8, 1.0] |
| call-H | S3 | Sonnet | 0.8 [0.6, 1.0] | 0.95 [0.8, 1.0] |

Pooled over the two models, out of 10, **registered and not scored**:

| arm | task | `right@1` | `right@2` |
|---|---|---|---|
| decl-H | S1 | 0.55 [0.4, 0.7] | 0.95 [0.8, 1.0] |
| decl-H | S2 | 0.5 [0.4, 0.7] | 0.9 [0.7, 1.0] |
| decl-H | S3 | 0.55 [0.4, 0.7] | 0.95 [0.8, 1.0] |
| call-H | S1 | 0.85 [0.7, 1.0] | 0.95 [0.8, 1.0] |
| call-H | S2 | 0.8 [0.6, 1.0] | 0.9 [0.8, 1.0] |
| call-H | S3 | 0.85 [0.7, 1.0] | 0.95 [0.8, 1.0] |

**`silent`**, per arm over its 30 answers, **scored**: 0.03 [0.0, 0.1] in each
(the first run's card arms: 1 of 60).

## The effects, call-H minus decl-H, scored

| metric | over | point | hit if the difference lies in |
|---|---|---|---|
| `right@1` | Sonnet, 15 a side | +0.55 | [+0.3, +1.0], +5 to +15 of 15 |
| `right@1` | Haiku, 15 a side | 0.0 | [-0.2, +0.2], -3 to +3 of 15 |
| `right@1` | pooled, 30 a side | +0.27 | [+0.1, +0.5], +3 to +15 of 30 |
| `right@2` | Sonnet | 0.0 | [-0.15, +0.15], -2 to +2 of 15 |
| `right@2` | Haiku | 0.0 | [-0.15, +0.15], -2 to +2 of 15 |
| `right@2` | pooled | 0.0 | [-0.1, +0.1], -3 to +3 of 30 |
| `silent` | pooled | 0.0 | [-0.1, +0.1], -3 to +3 of 30 |

## The labels, scored

The share of two-parameter package calls written with labels, first turns, by
the rule above:

| arm | Haiku | Sonnet |
|---|---|---|
| decl-H | 1.0 [0.9, 1.0] | 0.2 [0.0, 0.45] |
| call-H | 1.0 [0.9, 1.0] | 0.9 [0.7, 1.0] |

**And the mechanism**: of Sonnet's first turns in both arms together whose
two-parameter package calls are all labelled (an answer holding at least one),
`right@1` is 0.92 [0.8, 1.0] (the first run: 6 of 6). Where it holds, E2's
gain on Sonnet runs through the labels.

## Ceilings and floors

- **Ceilings**, both arms at 4 or 5 of 5, so the comparison there can show
  nothing: all six Haiku cells at `right@1` and at `right@2`; all six Sonnet
  cells at `right@2`, S2's at 4 to 5 rather than 5; and call-H Sonnet's S3
  `right@1` next to them. So the Haiku effects and every `right@2` effect are
  read between two ceilings, and their predicted zero is the ceilings', not
  evidence that the form is neutral. `right@2` can part the arms only through
  S2's `keys` hiding a label slip, folder 114's shape, which decl-H alone can
  suffer.
- **Floors**, 0 or 1 of 5: decl-H Sonnet's `right@1` on S2 and S3 (the first
  run's card-H: 0 and 0), with S1 at 0 to 2. So Sonnet's `right@1`, the one
  comparison that can move, is measured upward from a floor: it can show a
  gain on every task, and on S2 and S3 it cannot show a loss.

## What would say the card's form is not the cause

**If Sonnet's call-H first turns still label fewer than half of their
two-parameter package calls, the card's form is not the cause of Sonnet's
first-run loss, and what is left is the card's presence (with no card Sonnet
labelled 18 of 22 such calls, G8); and if decl-H does not lose again (Sonnet at
least 9 of 15 right at the first turn, or its label share above one half), the
first run's loss was that run's draw and no form caused it.**

Between those results: a call-H Sonnet label share from one half to 0.7, or an
effect on Sonnet's `right@1` of +3 or +4 of 15, misses my bands and says the
form is part of the cause, not the whole of it; and call-H answers that label
(share at least 0.7) yet stay wrong at the first turn (`right@1` within 2 of 15
of decl-H's) say the form cured the label and brought another slip, which the
first codes will name, not that the form is not the cause.

## What the comparison can attribute

- **call-H keeps the declaration and adds the call.** A null therefore says the
  call shown does not override the declaration on the same line, and nothing
  about a card without the declaration.
- **The call card leans by its examples as well as its form.** Five of its
  fourteen calls carry a task's own literal: `"#"` and `"="` (S2's comment mark
  and separator), `": "` and `"ERROR "` (S1's separator and prefix), `"the"`
  (S3's example needle). decl-H shows none of them. E2 measures the labelled
  call and those hints together, and at this size cannot part them. I expect
  the hints to be worth nothing measurable here, since the first run's
  labelled Sonnet answers were right without them, 6 of 6; the mechanism line
  above is where that expectation can fail.

## The calibration, as run

The bands are judgment on the first run's counts, checked against 200,000
simulated E2 runs, seed 20261106. The simulation's assumptions are inferences,
not measurements:

- Sonnet's labelled mode is drawn from Beta(7, 25) under decl-H (6 of 30). Under
  call-H it is drawn from Beta(17, 2) at weight 0.85 and Beta(4, 6) at 0.15, the
  partial fix, which is my judgment.
- A labelled Sonnet answer is right at 0.95, 0.88 and 0.96 on S1, S2 and S3,
  S1 0.03 lower under call-H. A refused one is right at its second turn at
  0.96, 0.85 and 0.97 (23 of 24, the miss being S2's).
- Haiku is right at Beta(11, 1), Beta(10, 2) and Beta(11, 1) (10, 9 and 10 of 10
  in the first run's card arms), S1 0.02 lower under call-H, and a Haiku miss is
  repaired at 0.4.

What it gave, for the bands it models: the 24 cells 0.84 to 1.00; the six
`right@1` and `right@2` effects 0.84 to 0.95; Sonnet's label bands 0.943
(decl-H) and 0.821 (call-H), read by two prints appended after the last
draw, so that every number above them is unchanged. The `silent` bands,
Haiku's label bands and the mechanism line are not in it, and rest on the
first run's counts alone. It was run from standard input, `python3 -I -B -`
from the repository's root, and it writes nothing:

```python
import random, collections
R = random.Random(20261106)
N = 200000
TASKS = ("S1", "S2", "S3")
# right@1 given every package call labelled, per task (first run: 6/6 Sonnet labelled answers right;
# Haiku 29/30 card answers right, the one miss a logic slip on S2; S2 carries the `keys` risk, 1 of 20)
P_LAB_RIGHT = {"S1": 0.95, "S2": 0.88, "S3": 0.96}
# right at the second turn for an answer refused at the first (first run: 23 of 24 Sonnet positional answers;
# the miss was S2's `keys` masking `needs_label`)
P_REPAIR = {"S1": 0.96, "S2": 0.85, "S3": 0.97}
HAIKU = {"S1": (11, 1), "S2": (10, 2), "S3": (11, 1)}   # Beta posteriors from 10/10, 9/10, 10/10
def beta(a, b): return R.betavariate(a, b)
def q_decl(): return beta(7, 25)                     # Sonnet answers fully labelled, first run 6 of 30
def q_call():                                        # the call form: mostly fixed, 15% weight on a partial fix
    return beta(17, 2) if R.random() < 0.85 else beta(4, 6)
def sonnet_cell(q, task, call):
    r1 = r2 = 0; lab_calls = pos_calls = 0
    calls = {"S1": 3, "S2": 2, "S3": 1}[task]
    for _ in range(5):
        labelled = R.random() < q
        if labelled:
            lab_calls += calls
            pr = P_LAB_RIGHT[task] - (0.03 if (call and task == "S1") else 0.0)
            ok = R.random() < pr
            r1 += ok
            # half the first-turn misses of a labelled answer are compile errors, repaired at turn 2
            r2 += ok or (R.random() < 0.5 and R.random() < 0.9)
        else:
            pos_calls += calls
            r2 += R.random() < P_REPAIR[task]
    return r1, r2, lab_calls, pos_calls
def haiku_cell(task, call):
    a, b = HAIKU[task]; p = beta(a, b) - (0.02 if (call and task == "S1") else 0.0)
    r1 = r2 = 0
    for _ in range(5):
        ok = R.random() < p
        r1 += ok; r2 += ok or (R.random() < 0.4)
    return r1, r2
dist = collections.defaultdict(collections.Counter)
eff = collections.defaultdict(collections.Counter)
labshare = collections.defaultdict(list)
for _ in range(N):
    tot = collections.Counter()
    for arm in ("decl", "call"):
        q = q_decl() if arm == "decl" else q_call()
        lab = pos = 0
        for t in TASKS:
            h1, h2 = haiku_cell(t, arm == "call")
            s1, s2, l, p = sonnet_cell(q, t, arm == "call")
            lab += l; pos += p
            dist[(arm, t, "haiku", "r1")][h1] += 1; dist[(arm, t, "haiku", "r2")][h2] += 1
            dist[(arm, t, "sonnet", "r1")][s1] += 1; dist[(arm, t, "sonnet", "r2")][s2] += 1
            dist[(arm, t, "pooled", "r1")][h1 + s1] += 1; dist[(arm, t, "pooled", "r2")][h2 + s2] += 1
            tot[(arm, "haiku", "r1")] += h1; tot[(arm, "haiku", "r2")] += h2
            tot[(arm, "sonnet", "r1")] += s1; tot[(arm, "sonnet", "r2")] += s2
        labshare[arm].append(lab / (lab + pos) if lab + pos else 1.0)
    for m in ("haiku", "sonnet"):
        for r in ("r1", "r2"):
            eff[(m, r)][tot[("call", m, r)] - tot[("decl", m, r)]] += 1
    for r in ("r1", "r2"):
        eff[("pooled", r)][tot[("call", "haiku", r)] + tot[("call", "sonnet", r)] - tot[("decl", "haiku", r)] - tot[("decl", "sonnet", r)]] += 1
def show(c, n):
    tot = sum(c.values()); mean = sum(k * v for k, v in c.items()) / tot / n
    return f"mean {mean:.2f} | " + " ".join(f"{k}:{c[k]/tot:.2f}" for k in sorted(c))
for k in sorted(dist):
    n = 10 if k[2] == "pooled" else 5
    print(*k, show(dist[k], n))
for k in sorted(eff):
    n = 30 if k[0] == "pooled" else 15
    print("EFFECT", *k, show(eff[k], n))
for arm in ("decl", "call"):
    xs = sorted(labshare[arm]); L = len(xs)
    print("SONNET LABEL SHARE", arm, "mean %.2f" % (sum(xs) / L), "5%% %.2f 10%% %.2f 50%% %.2f 90%% %.2f 95%% %.2f" % tuple(xs[int(L * f)] for f in (0.05, 0.1, 0.5, 0.9, 0.95)))
```

## Amended 2026-11-06 01:19 CET, for the corrected call card (e95ad4e4a)

**What was checked, read only.** The card's diff at `e95ad4e4a` changes six
example literals and nothing else: `contains`, `position` and `count` now
show `"ab"`, `has_prefix` and `trim_prefix` `"v1"`, `split_once` `";"`,
and the labelled call stands on all fourteen lines as before. By hash, the 30
call-H folders hold the corrected card (md5 `611ccf43...`), the 30 decl-H
folders the unchanged `S/card-H.md` (`f548bba2...`), and all 60 the same
brief and specification as before. No E2 session had run (`build/b234-out/`
held 454 files at 01:18 CET). The new literals are no task's literal and no
case's argument: S3's needles are `the`, `aa` and `x` (its `case*.arg`),
and `"ab"` occurs in the cases only inside `tab`, on a comment line of S2's
case 3.

**What moves.** In my numbers, the old literals entered at one place only: S1
under call-H lowered 0.02 for Haiku and 0.03 for Sonnet, because `position`'s
example was S1's own separator. That reason is gone, so the adjustment is
withdrawn. Re-run with both set to zero, the same seed and nothing else
changed:

| what | registered at 01:11 | amended |
|---|---|---|
| call-H, S1, Haiku, `right@1` | 0.9 [0.8, 1.0] | **0.95** [0.8, 1.0] |
| effect on `right@1`, Sonnet | +0.55 [+0.3, +1.0] | **+0.56** [+0.3, +1.0] |
| effect on `right@1`, pooled | +0.27 [+0.1, +0.5] | **+0.28** [+0.1, +0.5] |

**No band moves.** Every other point stands, and so do the label shares, the
mechanism line, the ceilings and floors, the sentence on what would say the
form is not the cause, the scoring rule and what I expect of myself. Under the
calibration the bands now cover 0.86 to 1.00 (cells), 0.87 to 0.95 (the six
`right@1` and `right@2` effects), 0.942 and 0.822 (Sonnet's label bands).

**Superseded, not deleted:** the failure model's sentence on
`position(s: line, substr: ": ")`, and the second bullet of *What the
comparison can attribute*. E2 now compares the labelled call, written with
neutral arguments, against the declaration. The first bullet stands: call-H
keeps the declaration and adds the call.

**The calibration as run for this amendment**: the script above, piped through
`sed` with its two S1 adjustments, `(0.03 if (call and task == "S1") else
0.0)` and `(0.02 if (call and task == "S1") else 0.0)`, each replaced by
`0.0`, the file unchanged; the two adjustments cost no random draw, so every
cell but call-H's S1 reads as before.

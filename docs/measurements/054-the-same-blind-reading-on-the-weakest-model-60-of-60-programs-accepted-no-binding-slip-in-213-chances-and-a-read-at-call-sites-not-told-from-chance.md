# 054: the same blind reading on the weakest model: 60 of 60 programs accepted, no binding slip in 213 chances, and a read at call sites not told from chance

Measured 2026-10-25 by the session heroes-lang-13, on the author's request
(meant as: *do the weaker model too*), given on measurement 053's closing
note that a habit is likelier to show on a weaker model. The same reading as
053, on `claude-haiku-5-5`, from the main checkout at `f5ce7d786`. The
evidence is the archive beside this file,
`054-the-same-blind-reading-on-the-weakest-model-60-of-60-programs-accepted-no-binding-slip-in-213-chances-and-a-read-at-call-sites-not-told-from-chance.tar.xz`,
self-contained (the specifications, briefs and reference programs again, the
tools as they stood at the end, both keys, every valid and every voided
session's report, `run.json` and `run.err`, and every scoring); the working
folder `.claude/worktrees/scratch-1025/` (2026-10-25) is scratch.

## 1. What was the same, and what was not

The same: the three specifications A, B and C, the three briefs, the read
program, the translator and the judge compiler (`sources 451802aa66692eb3`),
byte for byte (`cmp` over every one of the 90 folders' inputs), and the
command, `tools/run.sh` with two parameters added, the model and the folder,
its Opus run reproduced byte for byte by the parameterised scorer.

Not the same, each decided by the session and said to the author as it was
taken:

- **the model**, `claude-haiku-5-5`, the weakest of the current family;
  every `run.json` names it alone in `modelUsage`, `is_error` false;
- **the budget**: the author named none; the session kept 053's, 10 USD,
  under the same rule (a further wave only while the spent total plus its
  size times the dearest session stays under it);
- **ten readings a cell, not four**: a Haiku session cost 0.010 to 0.023 USD
  by the CLI's estimate, about an eighteenth of an Opus one (the first wave
  of nine 0.145 against 053's 2.736), and a habit that shows once in twenty
  or thirty chances needs more than four readings a cell to show at all; so
  90 sessions, the waves after the first run eighteen at a time.

## 2. The leak, and what was done about it

**45 sessions saw the project's git history and are void.** The 54 folders
of readings 5 to 10 were made after the first 36, and the loop that made each
its own git repository skipped any folder where `git -C <folder> rev-parse
-q --verify HEAD` succeeded, meant to skip a folder already made one. Inside
the project's tree that command succeeds for every folder, since it finds the
project's repository above it, so none of the 54 was made a repository, and
the check that followed (*repos with a commit: 90*) asked the same question
and could not tell. Found by session h53 (reading 6), whose `context` named
*a git status snapshot with recent commit messages from the parent
repository*: `git -C h53 rev-parse --show-toplevel` named the project's root,
and its log the project's commits, the newest *Measurement 053: the blind
reading of two birth spellings, 27 of 27 accepted and no binding slip in any
variant*. Readings 5 to 9 had run there, 45 sessions, 0.675 USD; 29 of their
45 `context` sections name the project's commits, the measurement or a parent
repository.

By panel 183's rule a reading whose context held the project is void. The
45 were moved aside whole (`void-haiku/`, scored apart in
`void-scoring-haiku/`: 45 programs accepted with the right output, 15 read
programs classified as expected), the 54 folders made again from the same
inputs with `git init` run unconditionally, and **every folder of both runs,
126, checked by the question that can tell**: `git rev-parse --show-toplevel`
is the folder itself and `git rev-list --count HEAD` is 1, where the
project's repository would answer thousands. Readings 5 to 10 then ran clean.
Measurement 053's 27 sessions and Haiku's readings 1 to 4 were made by the
first loop, which ran `git init` on every folder; the same check passes on
all of them, and none of their `context` sections names anything of the
project (one wording, h32's, is in section 3).

## 3. How it ran

| wave | readings | start | end | sessions |
|---|---|---|---|---|
| 1 | 1 | 15:35:57 | 15:37:50 | 9, valid |
| 2 | 2, 3 | 15:39:00 | 15:41:37 | 18, valid |
| 3 | 4, 5 | 15:42:18 | 15:44:30 | 9 valid (reading 4), 9 void (reading 5) |
| 4 | 6, 7 | 15:45:19 | 15:47:52 | 18, void |
| 5 | 8, 9 | 15:48:43 | 15:50:57 | 18, void |
| 6 | 5, 6 | 15:52:17 | 15:54:33 | 18, valid, the remade folders |
| 7 | 7, 8 | 15:55:39 | 15:58:03 | 18, valid |
| 8 | 9, 10 | 15:59:07 | 16:01:21 | 18, valid |

Every session exit 0. **1.387 USD for the 90 valid, 0.675 for the 45 void,
2.062 in all**, the CLI's estimate at list price; a valid session 4 to 16
turns and 45 to 107 s, more turns than Opus's 4 or 5, in part because 27 of
the 90 valid sessions say they also opened `run.json` and `run.err`, which sit
in the folder and are empty while a session runs (the shell's redirection
makes them at launch, the session's output fills them as it ends). Four valid
sessions (h10, h34, h52, h69) wrote a stray `report.md.tmp`, holding `x` or
nothing, and each said so in its report; three void ones did too (h53, which
said so, and h81 and h82, which did not). All seven are in the archive. One
valid context, h32's, calls its
git status *a snapshot for the parent repository*; h32 is reading 1, its
folder passes the check above, and its context names nothing of the project,
so it is kept, and written here.

## 4. Results

### The written programs

| task | A | B | C |
|---|---|---|---|
| t1, accepted at the first try with the right output | 10 of 10 | 10 of 10 | 10 of 10 |
| t2, accepted at the first try with the right output | 10 of 10 | 10 of 10 | 10 of 10 |

No diagnostic on any of the 60 (every `check.txt` empty), no `FOREIGN` line;
t1's programs 22 to 34 non-blank lines, t2's 28 to 46.

**The chances to slip** (`tools/count.py`, measurement 053's patterns, which
reproduces 053's table exactly on its programs):

| variant | immutable births | cell births | writes `place @ e` | `@` arguments |
|---|---|---|---|---|
| A | 40 | 96 | 136 | 70 |
| B | 42 | 85 | 123 | 70 |
| C | 35 | 67 | 104 | 70 |

Under A, **136 writes, none spelled `=`**; under B and C, **77 immutable
births, none spelled `=`**, and 152 cell births, none either: **213 chances
for the habit, in its two directions, and no slip**.

### The read program

| variant | readings | refused at 20 and 22 | repair accepted, prints `5 5 a` | all 13 lines as expected | lines as expected |
|---|---|---|---|---|---|
| A | 10 | 10 | 10 | 10 | 130 of 130 |
| B | 10 | 10 | 10 | 8 | 126 of 130 |
| C | 10 | 10 | 10 | 9 | 128 of 130 |

The six lines off the expected reading, each read in full (`tools/t3.py`,
its last version reading the class without the column that quotes the
specification, which reproduces 053's nine Opus readings at 13 of 13):

- **h01 (B) and h75 (C), lines 19 and 21**, `bump(@c, ...)`: *neither*,
  h01 because *`@` at the call site only marks `c`; the write happens in line
  6*, h75 on the same sentences. Both read the change as made inside the
  callee rather than at the call.
- **h03 (B), line 5**, the parameter list: *neither*, a parameter not being
  a name the line brings into existence in its reading; and **line 20**, the
  refused write: *neither, as written*, a refused line changing nothing.

Each is a defensible reading of the brief's words, and none confuses a birth
with a change. **The call-site reading occurred 0 times in 10 under A and 2
in 20 under B and C.** That is the direction the split write mark predicts
(under A the write and the `@` argument carry the same sign, under B and C
they do not), and it is not told from chance: were the two readings
distributed at random among the 30, the chance of both falling outside A is
**0.437**; for every off-reading reading, 0 in 10 against 3 in 20, **0.281**
(`math.comb`, one-sided).

## 5. What it says, and what it does not

**It says** that the weakest model of the family, given the specification,
wrote 60 programs of 22 to 46 lines under three spellings without one binding
slip in 213 chances, the habit absent in both directions as it was on Opus,
and read births and changes on the line alone under all three. Together with
053: **78 written programs, 183 writes under today's syntax and 87 immutable
births under the variant, no slip.** The variant's one unmeasured argument
stays measured at zero on both models.

**It does not say** anything about `claude-sonnet-5-5`, between the two, not
run; about a long session or a program far longer than 46 lines; about token
cost; or, at 10 readings a cell, whether the call-site reading depends on the
spelling. That last is the one place the two spellings came apart at all, and
a reading built for it (more readings, more call sites, a brief asking it
directly) is what would settle it.

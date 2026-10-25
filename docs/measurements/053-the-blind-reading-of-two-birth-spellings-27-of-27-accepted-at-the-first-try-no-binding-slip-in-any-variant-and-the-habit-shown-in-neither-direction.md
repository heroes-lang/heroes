# 053: the blind reading of two birth spellings: 27 of 27 accepted at the first try, no binding slip in any variant, and the habit shown in neither direction

Measured 2026-10-25 by the session heroes-lang-13, on the author's request,
from the main checkout at `76e98124e`, every time below read from `date`. The
evidence is the archive beside this file,
`053-the-blind-reading-of-two-birth-spellings-27-of-27-accepted-at-the-first-try-no-binding-slip-in-any-variant-and-the-habit-shown-in-neither-direction.tar.xz`,
whose contents section 8 lists; the working folder it was packed from,
`.claude/worktrees/scratch-1025/` (2026-10-25), is scratch and will not last.

## 1. The question, and whose it is

The variant is the author's. They raised it at panel 209's ratification on
2026-10-10 (*`:=` for the once-bound name, `@=` for the cell and bare `=` for
the mutation*, weighed and set aside unmeasured, `docs/panel/209-a-cell-is-declared-by-its-own-symbol-and-typed-by-its-value.md`
§ Author's verdict), and again in conversation today, as a pure question of
syntax: if `=` never initialises, every `=` a reader sees is a change. They
then asked whether a sign other than the colon could carry the immutable
birth, since `:=` had been refused for reading the type's colon twice. The
session found `$` the one free sign among those it tried (`$=`, `~=`, `::`
asked of the compiler: `$` is `unexpected_character`, `~` is the bitwise not,
`::` names a field) and recommended keeping today's syntax on three counted
grounds: one mark, `@`, on every write today; the write mark split between `=`
and `@` under the variant; the plain glyph given to the line a reader must
find. **The one argument left for the variant was unmeasured**: a model's
habit of writing `v = v + 1`, which today is an error (`shadowed_binding`
with a `guess` fix writing `@`, run on the trunk's compiler the same day). The
author asked for the blind reading that would measure it, with a budget of
10 USD.

The habit cuts both ways, and the reading was built to see either direction:
a writer used to Python writes `x = 5` both for a new name and for a change,
so under today's syntax the slip is `=` for a write, and under the variant it
is `=` for a birth.

## 2. The three specifications

- **A**: `spec/heroes-spec.md` at `76e98124e`, byte for byte (`cmp`).
- **B**: A with every birth `=` written `$=` and every re-binding `@` written
  `=`; `@=`, every `@` parameter and every `@` argument unchanged; the discard
  `_ $= e`, the variant's grammar giving `_` a `Binding` as A's does.
- **C**: B with every `$=` written `:=` (14 sites).

B is made by `tools/make_specs.py` from 17 exact anchors, each asserted to
occur exactly once in A before anything is written: the 14 lines of A that
spell a birth or a write (sections 2, 3, 4, 5, 8 and 10) and the prose and
productions that state the rule (`Binding`, `Simple`, *`=` binds once,
forever*, *a `=` name*, *a declaration or an `=`*). 19 lines differ from A;
nothing else does. Read in full before the derivation: every line of A that
spells a birth or a write is among the 14.

## 3. The tasks

Three tasks, the same text under every variant (`tasks/brief-t*.md`), each in
the shape of panel 209's briefs with one deliberate difference: panel 209's
`sentences` heading asked *in particular the sentences that say how a value
that changes is declared and how it is changed*, which points the reader at
the rule being measured; these briefs ask for every sentence the program rests
on and name no rule, so that a habit has the room to show.

- **t1, write**: `sorted_desc(xs: [i64]) -> [i64]` by adjacent swaps without
  `sort`, `runs(xs: [i64]) -> i64`, and `main` printing both. The swap is the
  trap in both directions: a temporary born, two elements written.
- **t2, write**: a record `Item`, `restock` and `take` over an `@` parameter
  `[Item]`, a field written, an array grown, `main` printing two `bool`s and a
  sum.
- **t3, read**: a 25-line program in the variant's spelling (made from A's by
  `tools/translate.py --to`), refused today at line 20 (`not_mutable`, a write
  to a name bound once) and line 22 (`never_rebound`); the reader gives the
  verdict, the smallest repair and its output, and classifies every line
  holding `=` or `@` as a new name, a change, or neither.

The reference programs (`ref/`) pass the trunk's checker and print the
expected output (`9 6 5 4 3 2 1 1` then `4`; `true`, `false`, `4`; the
repaired t3 `5`, `5`, `a`), and t3 is refused at exactly lines 20 and 22 with
those two codes.

## 4. How it ran

27 fresh `claude -p` sessions, each in a folder of its own named `s01` to `s36`
at random (seed 20261025), the key (`key.tsv`) outside every session folder,
each folder its own git repository with one commit `inputs` holding only its
inputs, as panel 209's clean waves were. The command is the panel skill's of
2026-10-01 (`tools/run.sh`): `--model claude-opus-5-5`, `--restricted
--safe-mode --strict-mcp-config`, `Read` and `Write` only, `--max-budget-usd
0.8`, `ANTHROPIC_API_KEY` removed from the environment (it read length 0 in the
shell anyway), CLI 2.1.308, logged in as `authMethod: claude.ai`, under
`caffeinate -i` and `timeout 900`.

**The budget rule**, set before the first wave and followed: waves of nine,
one reading of every task and variant each, and a further wave only while the
spent total plus nine times the dearest session so far stays under the
author's 10 USD.

| wave | start | end | sessions | spent | projection for the next |
|---|---|---|---|---|---|
| 1 | 14:42:46 | 14:46:14 | 9, exit 0 | 2.736 | 6.256, run |
| 2 | 14:47:16 | 14:48:34 | 9, exit 0 | 2.370 | 8.625, run |
| 3 | 14:48:56 | 14:50:26 | 9, exit 0 | 2.536 | 11.162, **not run** |

**7.642 USD in all**, the CLI's estimate at list price (`total_cost_usd` of
the 27 `run.json`), a session 0.245 to 0.391, 4 or 5 turns, 52 to 200 s. The
fourth reading of every cell, nine folders, was prepared and not launched. A
first launch of wave 2 at 14:46:47 started nothing (zsh passed the nine names
as one word, `cd` failed in each subshell); its folders were checked to hold
only their inputs before the relaunch, and nothing was spent.

**What reached the sessions' context**: every one of the 27 `context`
sections names the harness's environment note, the folder's own git status
(`run.err`, `run.json`, the commit `inputs`), the git user name, the account
email and the working directory's path, and says none of it informed the
work; none names anything of the project or of the question (each read in
full for wave 1, and all 27 screened for the project's words).

## 5. How it was scored

The judge is a compiler built for the purpose from the seed, then stage 2 from
`selfhost/` (`heroes 0.4.0`, `sources 451802aa66692eb3`, the same key the
checkout's `./heroes` printed). `tools/score.py` takes the first fenced block
under each report's `program` heading, translates a B or C program to A with
`tools/translate.py --from`, runs `check`, and runs the program under a 30 s
timeout where the check passes, comparing its output with the expected one.

**The translator preserves the errors it would hide**, which is what makes
today's compiler a fair judge of the variants. It reads only the operator after
a statement's leading place, at the start of a line or after an inline arm's
`=>`, and never adds, moves or removes a line. Under B or C a bare `=` becomes
A's `@`, so a birth written `x = 5` reaches the compiler as `x @ 5`,
`unknown_name`, as it would be under the variant; an operator the variant does
not have, A's `@ ` write or the other variant's birth sign, is reported as
`FOREIGN` and counted as an error, since A's compiler would otherwise accept a
B program writing `v @ v + 1`. Probed before any session ran (`probe/`): a
birth written with `=`, a write to a once-bound name, A's write in B, Go's
`:=` in B and the discard written `_ = e` each end refused or `FOREIGN`; a
correct B program and `_ $= e` pass. The round trips A to B to A and A to C to
A are byte-identical on all three reference programs (`cmp`).

## 6. Results

### The written programs

| task | variant | readings | accepted at the first try | output right | binding errors | `FOREIGN` |
|---|---|---|---|---|---|---|
| t1 | A | 3 | 3 | 3 | 0 | 0 |
| t1 | B | 3 | 3 | 3 | 0 | 0 |
| t1 | C | 3 | 3 | 3 | 0 | 0 |
| t2 | A | 3 | 3 | 3 | 0 | 0 |
| t2 | B | 3 | 3 | 3 | 0 | 0 |
| t2 | C | 3 | 3 | 3 | 0 | 0 |

No diagnostic of any kind on any of the 18 programs, which are 23 to 32
non-blank lines for t1 and 28 to 43 for t2.

**The chances to slip**, counted on each program's A form with the same
patterns as section 7's:

| variant | immutable births | cell births | writes `place @ e` | `@` arguments |
|---|---|---|---|---|
| A | 6 | 31 | 47 | 21 |
| B | 3 | 32 | 51 | 21 |
| C | 7 | 25 | 40 | 21 |

Under A, **47 writes, none spelled `=`**: the habit the variant was meant to
spare did not show. Under B and C, **10 immutable births, none spelled `=`**,
and 57 cell births, none either: the opposite habit did not show. Every
written swap took its variant's form: `t = ys[i]` then `ys[i] @ ys[i - 1]`
under A, `t $= a[j]` then `a[j] = a[j + 1]` under B, `t := ys[i - 1]` then
`ys[i - 1] = ys[i]` under C.

### The read program

| variant | readings | refused at 20 and 22 | the repair (`step @= 1`, `last` bound once) | output `5 5 a` | lines classified as expected |
|---|---|---|---|---|---|
| A | 3 | 3 | 3 | 3 | 39 of 39 |
| B | 3 | 3 | 3 | 3 | 39 of 39 |
| C | 3 | 3 | 3 | 3 | 39 of 39 |

The expected classes, thirteen lines each: 5, 9, 10, 11, 12, 18 and 22 a new
name (line 5 the parameters); 6, 15, 16, 19, 20 and 21 a change, 19 and 21
through an `@` argument. Every repaired program, translated where it was B or
C, passes the checker and prints `5`, `5`, `a`.

### Where the readers were least sure

A write was the least-sure line for 1 of 6 writers under A, 2 of 6 under B and
3 of 6 under C, each read in full in its `confidence` section. Each gave a
reason the three specifications share word for word, and none named the
operator: the type of an array's index, which the specification never states
(one writer under each variant, s24, s06, s19); a write into an array's
element, which it shows only on a map, *or a field or element inside one* and
`m["a"] = 1` or `m["a"] @ 1` (s20, under C); and a write of a whole `@`
parameter, which it never shows (s33 under B, s11 under C). Three t3 readers,
one under A and two under B, named a field write through an `@` parameter,
which no example shows either. At three readings a cell, 1, 2
and 3 of 6 is not a difference this reading can tell from chance, and it is
written here as what was counted, not as a finding.

## 7. What it says, and what it does not

**It says** that on `claude-opus-5-5`, with the specification in front of it,
programs of 23 to 43 lines and a reading of 25, neither habit showed: no
writer under today's syntax spelled a write `=`, no writer under the variant
spelled a birth `=`, and every reader told births from changes on the line
alone under all three spellings. The variant's one unmeasured argument is
therefore measured at this size and on this model, and it is **zero in both
directions**. Under Principle 0 (CLAUDE.md § 2) a form enters with a measured
effect; this reading found none, so it gives the variant no ground and takes
none from today's syntax either: both read at the ceiling.

**It does not say** anything about:

- another model: one was asked, the strongest; a weaker one is where a habit
  would be likelier to show, and none was run;
- a long session, where the specification is far behind in the context, or a
  program much longer than 43 lines;
- the token cost of either spelling on the reader's tokeniser, which was not
  measured;
- a fourth reading, which the budget rule left unrun.

**Beside panel 209**: its 16 writing sessions, 8 under the syntax of that day
and 8 under today's, all wrote `@` for every change
(`docs/panel/209-reports/llm-ergonomist-scoring.md`), on programs of nine
lines at most and under a brief that pointed at the rule. This reading
removes both conditions and finds the same.

## 8. The counts quoted in the conversation, and the archive

The recommendation that preceded the reading rested on counts of `selfhost/`
at `76e98124e`, 680 files, by these patterns over every line including test
blocks, so approximate: **12,826 immutable births**
(`^\s+[a-z_][a-z0-9_]*(: [^=]+)? = `, the discards taken out), **6,852 cell
births** (`... @= `), **9,023 writes** (`^\s+[a-z_][a-z0-9_.]*(\[[^]]*\])?[a-z0-9_.]* @ `),
**7,084 `@` arguments** (`[(,] ?@[a-z_]`), 395 discards and 1,108 annotated
immutable births. Panel 209's ratification called the write *the rare act*;
these counts put writes at about 70% of immutable births. That sentence of
panel 209 is the coordinator's reading of that day and stands at its date.

**The archive** holds, under `053/`: `specs/` (A, B, C), `tasks/` (the three
briefs), `tools/` (`make_specs.py`, `translate.py`, `run.sh`, `score.py`),
`ref/` (the reference programs, their B and C translations and round trips),
`probe/` (the translator's edge cases), `key.tsv` (folder to task, variant and
reading), every session's `report.md`, `run.json` and `run.err`, and under
`scoring/` each program as submitted, its A form, the checker's output and the
program's; 238 files, no build output and no compiler binary, which `76e98124e`
rebuilds.

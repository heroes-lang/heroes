# 014 — `heroes mutate` over the 35-program corpus, and the corpus leg timed alone

Date: 2026-09-03 · M-corpus-depth step 0, landed with the milestone's scheduling ·
**the first metric-3 score since 2026-08-13** (measurement 007, 45 files), and the
first over the corpus M-corpus-coverage left (78 files, 35 program directories).

## Provenance

| what | value |
|---|---|
| compiler | `b447e523` (M-robustness-guards close), rebuilt from `seed/heroes.c` with the one clang line at the start of the session |
| spec | sha256 `20bd78b75a0bdeca…` — `heroes measure` **3718** tokens, headroom 378, unchanged by this measurement |
| corpus | `examples/` — **78** `.hero` files, **36** directories, **35** with a `main.hero` (`gallery/` has none) |
| instruments | `heroes mutate --survivors`; `heroes mutate <dir>` once per directory; `heroes run tests/harness/main.hero -- ./heroes corpus` under `/usr/bin/time -p` |
| machine | the author's Mac, arm64, Apple clang 21.0.0, nothing else heavy running during a timed command |

## What was owed, and what the measurement found instead

Step 0 owed two numbers: the mutation score over the corpus as it stands, so that
every program M-corpus-depth adds moves a **known** denominator, and the corpus
leg's wall time alone, so that the milestone's projected **+1:15** is checked
against a number rather than a memory.

**The first number could not be taken the way it has always been taken.**
`heroes mutate` over `examples/` is **refused at exit 2 in 2.10 s**:

```
error: this corpus does not compile, so its kill rate would be meaningless
  2 of 78 programs are already refused by `heroes check`:
    examples/shapes/geom/area.hero
    examples/shapes/render/ascii.hero
```

`mutate` walks every `.hero` under the directory (`selfhost/cli/mutate.hero:80-102`)
and checks each one from its own directory. Those two are modules of a nested
program: `geom/area.hero` says `use geom/point`, `render/ascii.hero` says
`use geom/point` and `use scale`, and from their own directories those paths read
`examples/shapes/geom/geom/point.hero` — `error[unknown_module]`, exactly as the
diagnostic says — while `heroes check examples/shapes/main.hero` is exit 0. That is
panel 099 R5's leaf hazard, the rule M-robustness-guards step 6 pinned for the
compiler with a fixture; `mutate` is the caller that walks leaves by construction,
and nobody had run it over `examples/` since `examples/shapes/` landed on
2026-09-02 (`c12de74`, M-package-layout step 2). **The metric the thesis rests on
has been unable to read its own corpus for a day**, and the reason it was not seen
is that `heroes mutate` over `examples/` runs in no CI leg — `suite_surface.hero:281`
runs it over `examples/gallery` alone. The fix is M-corpus-depth step 1 in
`docs/work/SCHEDULED.md`; the CI question is in `docs/work/DECIDE.md`.

## The provisional score: one run per directory, summed

`mutate` takes a directory, so the corpus was scored as **36 runs, one per
directory under `examples/`**, and the tables summed per operator. **That the sum
is what one run would print was verified before it was used**, not assumed: the
gallery's twelve files were copied into two scratch directories of six, each run,
and the per-operator sums compared with `heroes mutate examples/gallery` run whole —
identical in every column, **538 | 6 | 514 (97%) | 416 (78%)**, rates included (the
summing script reproduces `score.hero::rate`'s rounding, half-to-even). So the table
below is the corpus's number over the 35 directories `mutate` accepts, and
`shapes/` — six files — is the one it cannot score today.

| operator | mutants | excluded | killed (check) | killed (--permissive) |
|---|---|---|---|---|
| swap-args | 1106 | 29 | 864 (80%) | 608 (56%) |
| drop-case | 89 | 0 | 89 (100%) | 89 (100%) |
| forget-at-decl | 386 | 0 | 380 (98%) | 380 (98%) |
| mutate-undeclared | 436 | 73 | 363 (100%) | 363 (100%) |
| typo-ident | 6161 | 36 | 6125 (100%) | 6125 (100%) |
| typo-code | 34 | 0 | 0 (0%) | 0 (0%) |
| wildcard-variant | 238 | 0 | 238 (100%) | 146 (61%) |
| positional-named | 657 | 1 | 492 (75%) | 0 (0%) |
| mix-int-float | 830 | 0 | 830 (100%) | 830 (100%) |
| shadow | 495 | 0 | 477 (96%) | 0 (0%) |
| drop-question | 84 | 0 | 75 (89%) | 75 (89%) |
| typo-digit | 29 | 0 | 0 (0%) | 0 (0%) |
| boolean-twin | 166 | 0 | 166 (100%) | 166 (100%) |
| **total** | 10711 | 139 | 10099 (96%) | 8782 (83%) |

Never pool these into one headline (panel 011): a per-operator rate is the
measurement, and `forget-at-decl` is excluded from any summary because the
mistakes it plants are caught by construction — the tool's own footer, kept.
`--survivors` was not passed to the per-directory runs, so the survivors are the
differences above and not a listing; step 1 lists them when one run reads the
whole corpus again.

**Read against measurement 006** (44 files, 2026-08-13: 5591 mutants, 5254
killed, 95% / 84%): **1.9× the mutants at the same rates**, 96% strict and 83%
permissive — twenty programs written to use every form did not move the
per-operator picture, which is what a corpus is supposed to do to a rate. Three
things in the table are worth a sentence each, none of them new:

- **`typo-code` 0 of 34 and `typo-digit` 0 of 29** — the two operators at zero.
  `typo-code` has been 0% since measurement 004 (an error code is a string, so a
  typo in one is a different string, not a compile error); `typo-digit` had its
  sites driven **5 → 0** by measurement 005's magic-constant sweep and has **29**
  again over the widened corpus. No cause is asserted; it is the number to open at
  step 1, with `--survivors`.
- **`swap-args` 864 of 1106 (80%) strict** — the most survivors of any live
  operator, 242 in the strict arm; **56% permissive**, which is the named-argument
  rule doing 24 points of work.
- **`positional-named` 492 of 657 (75%) strict and 0 permissive**, **`shadow` 477
  of 495 and 0**, **`wildcard-variant` 238 of 238 and 146** — the three operators
  where the permissive arm collapses are the three thesis rules, as designed.

**The price of the per-directory stand-in, measured**: the 36 runs took **680 s**
wall (11:20) on the Mac, sequentially, and the cost is where the mutants are.
Per directory, read off the output files' modification times (loop order, so each
is the gap to the previous file):

| directory | seconds |
|---|---|
| `json/` | 90 |
| `spreadsheet/` | 69 |
| `assembler/` | 57 |
| `calculator/` | 47 |
| `markdown/` | 45 |
| `logs/` | 35 |
| `todo/` | 29 |
| `diff/` | 28 |
| `maze/` · `dates/` | 27 each |
| `ini/` | 26 |
| `board/` | 25 |

The six largest programs are **343 s of the 680** — half the loop — and
`shapes/` refused in under a second. One run over the whole directory, when step 1
lets it read the corpus, should come in under this, since the loop paid the
compiler's startup 36 times; that is a prediction and is scored at step 1.

The gallery alone is **514 of 538 killed (97%)** strict against **512 (96%)** on
2026-08-19 (`DESIGN-LOG:396`): two more mutants die on the same twelve files, which
is the compiler getting stricter between the two dates, not the corpus changing.

## The corpus leg, timed alone

`tests/harness/main.hero` takes a suite name (`main.hero:15-20`), so the corpus was
timed without the rest of the net, twice, nothing else running:

| run | wall (`real`) | user | sys | result |
|---|---|---|---|---|
| 1 | **203.91 s** (3:24) | 99.10 | 27.03 | `corpus: 35 passed, 0 failed` |
| 2 | **188.02 s** (3:08) | 84.08 | 27.54 | `corpus: 35 passed, 0 failed` |

Both include the harness's own build, which the command performs first; journal
029's **2:51.19** for the same 35 programs on 2026-09-02 was taken inside a full net
run and is not the same command, so the two are not compared. **The "before" for
M-corpus-depth is this pair**: the milestone's projection is +1:15 on it, and the
close re-runs the same command twice.

## Appended the same evening: step 1 landed, and one run reads the whole corpus

The author moved M-corpus-depth ahead of M-isolated-threads and opened it
(*"anticipare quei due passi subito"*); step 1 teaches `mutate` to compile a
module below a nested program's root under a stand-in name beside that root's
`main.hero` (`selfhost/cli/mutate.hero::checked_name`), so its `use` paths are
read from the root exactly as the compiler reads them. With that compiler,
`heroes mutate examples --survivors` over all **78** files, alone on the Mac:

| operator | mutants | excluded | killed (check) | killed (--permissive) |
|---|---|---|---|---|
| swap-args | 1135 | 29 | 893 (81%) | 608 (55%) |
| drop-case | 89 | 0 | 89 (100%) | 89 (100%) |
| forget-at-decl | 388 | 0 | 382 (98%) | 382 (98%) |
| mutate-undeclared | 438 | 73 | 365 (100%) | 365 (100%) |
| typo-ident | 6232 | 36 | 6196 (100%) | 6196 (100%) |
| typo-code | 34 | 0 | 0 (0%) | 0 (0%) |
| wildcard-variant | 238 | 0 | 238 (100%) | 146 (61%) |
| positional-named | 692 | 1 | 521 (75%) | 0 (0%) |
| mix-int-float | 845 | 0 | 845 (100%) | 845 (100%) |
| shadow | 508 | 0 | 490 (96%) | 0 (0%) |
| drop-question | 84 | 0 | 75 (89%) | 75 (89%) |
| typo-digit | 29 | 0 | 0 (0%) | 0 (0%) |
| boolean-twin | 166 | 0 | 166 (100%) | 166 (100%) |
| **total** | 10878 | 139 | 10260 (96%) | 8872 (83%) |

**This is the corpus's number**, and the per-directory sum above was what it
said it was: the six `shapes/` files add **167 mutants, 161 killed strict, 90
permissive**, and every other operator row is the earlier row plus `shapes/`'s
share (`swap-args` 1106 → 1135, `typo-ident` 6161 → 6232, `positional-named`
657 → 692, `shadow` 495 → 508, `mix-int-float` 830 → 845, `forget-at-decl` 386 →
388, `mutate-undeclared` 436 → 438; the six others unchanged).

**Survivors, listed for the first time over this corpus: 479** — `swap-args`
213, `positional-named` 170, `typo-code` 34, `typo-digit` 29, `shadow` 18,
`drop-question` 9, `forget-at-decl` 6. By file, the most: `template/main.hero`
38, `wrap/main.hero` 36, `assembler/program.hero` 34, `json/parse.hero` 27,
`ini/main.hero` 24 — programs whose functions take two `str` or two `i64`
parameters, which is where `swap-args` and `positional-named` live. The 29
`typo-digit` survivors are all of the shape `10000` → `10001` and `0` → `1` in
`assembler/` — constants a program defines and no test pins to a second source —
which is the same thing measurement 005 said of them in a different corpus.

**The 27 survivors outside the four expected operators were read, and they are
two shapes.** The 18 `shadow` survivors are all a `_ = <call>` line duplicated —
`_ = sqlite3_close(db)` twice, `_ = node.add_node(@u, …)` twice — which
`spec:128-129` makes legal by design (`_` binds nothing and may repeat): a
repeated discarded call is a program the type system cannot tell from the
intended one, and the double `sqlite3_close` says what it costs at run time. The
9 `drop-question` survivors are **one shape and a finding**: `_ = expect(line,
2)?` → `_ = expect(line, 2)` compiles, because a discard accepts a `T?` like any
value, so the one place a `?` can be forgotten without a type error is the
discard. That is filed in `docs/work/DECIDE.md` with three shapes and a
recommendation; the nine lines are its witnesses.

**The prediction above is scored, and the honest verdict is "held by 0.74 s and
for the wrong reason."** One run took **679.26 s** wall against the loop's 680: it
did come in under, and the reason given — 36 compiler start-ups paid by the loop —
is falsified, because that saving would have been visible and it is not (a
start-up is milliseconds; the run also scored six files the loop could not). The
cost is the mutants, not the invocations, and the 11-minute price on the tag run
is what the DECIDE answer accepted.

- `docs/work/SCHEDULED.md` gains M-corpus-depth **step 1**, the `mutate` root fix,
  ahead of every program — because every later score depends on it.
- `docs/work/DECIDE.md` gains the CI question with this file's numbers as the price.
- `examples/README.md` says, dated, that `mutate` over the directory is refused today.
- The fourteenth operator (`DONE.md:924`, re-opened in `SCHEDULED.md`) is scored
  against **this** table when it lands, not against measurement 007's.

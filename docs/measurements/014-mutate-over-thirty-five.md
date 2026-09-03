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

## What this measurement changes

- `docs/work/SCHEDULED.md` gains M-corpus-depth **step 1**, the `mutate` root fix,
  ahead of every program — because every later score depends on it.
- `docs/work/DECIDE.md` gains the CI question with this file's numbers as the price.
- `examples/README.md` says, dated, that `mutate` over the directory is refused today.
- The fourteenth operator (`DONE.md:924`, re-opened in `SCHEDULED.md`) is scored
  against **this** table when it lands, not against measurement 007's.

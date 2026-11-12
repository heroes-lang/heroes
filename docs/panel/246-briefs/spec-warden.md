# Panel 246, spec-warden

Read `00-shared.md` first, whole. You judge **Part B only**; Part A is the soundness lane and changes
no spec token. Your folder is `.claude/worktrees/p246-scratch/246-spec-warden/`, your tree `tree/` in
it, a `git archive` of **`ef50a7478`** (tree `084b36b7d5db6b342c281468fff120b2fe98349b`), your
compiler built there from the seed in two stages (`sources 796d1133542b1909`). Your report is
`docs/panel/246-reports/spec-warden.md` in `.claude/worktrees/p246`, in your definition's output
structure, written as you go. Reach design.md §1.6 by grep before you cite it (`grep -n '^### 1.6'
docs/design.md`: 253; the ceiling's sentence at 255; the payment rule at 282, 301, 324, 328 and 330,
`grep -n 'payment' docs/design.md`).

## The count before, measured at the base

`./heroes-next measure spec/heroes-spec.md` from the base's root (it reads the vendored tables from
the tree's root and refuses from another folder, *"cannot read `vendor/tokenizers/claude-legacy.json`"*):

| row | count |
|---|---|
| claude-legacy | 8267 |
| cl100k_base | 8417 |
| maximum (vendored) | 8417, *"a lower bound, not the reader's tokeniser"* |
| real | **11014**, *"claude-opus-5, 2026-11-12"*, *"the binding number"*: the instrument's pin for these bytes, no `STALE` printed |
| headroom | 5370 against 16384; the FFI floor mortgages 60, so 11074 is what the ceiling judges |

The ceiling is 16384 (design.md:255; `tests/harness/suite_spec.hero:162-163`). The real 11014 is
row 8417 of `docs/measurements/010-spec-budget-ledger.md` (line 244): feature 2295's F3 sentence,
**+31 vendored and +41 real**, on a `--refresh` of 2026-11-12, 10,973 to 11,014. Run it again in your
own tree before you use it; a different figure is a finding.

## The candidates, priced on the vendored tables at the base

Each is the base's spec with the sentence in place: T1, T2, S1, S2 and R2's appended, each with one
leading space, after the F3 sentence's last words, `linked where a group names one.`
(`spec/heroes-spec.md:377`); S3 put after `one written as a number is refused) · ` in § 11, the
built-ins table's own separator (line 362). The coordinator's drafts are in
`.claude/worktrees/p246-scratch/price2/`, made by `p246-scratch/tools/drafts2.py`; rebuild them in
your own folder from 00-shared.md's text rather than reading mine.

| draft | claude-legacy | cl100k_base | maximum | delta (max) |
|---|---|---|---|---|
| base | 8267 | 8417 | 8417 | |
| T1 | 8340 | 8490 | 8490 | +73 |
| T2 | 8282 | 8432 | 8432 | +15 |
| S1 | 8335 | 8487 | 8487 | +70 |
| S2 | 8330 | 8482 | 8482 | +65 |
| S3 | 8325 | 8475 | 8475 | +58 |
| T1 + S1 | 8408 | 8560 | 8560 | +143 |
| T1 + S3 | 8398 | 8548 | 8548 | +131 |
| R2 (panel 114's sentence) | 8298 | 8448 | 8448 | +31 |

**The real delta of a draft is unmeasured**: it needs `heroes measure --refresh`, a paid call this
brief does not name, so every verdict of yours on a draft is `provisional` and every vendored delta a
lower bound, said in those words. For scale, neither a measurement of these drafts: the whole spec's
real-to-vendored ratio at the base is 11014 / 8417 = 1.3085 (`python3 -I -c`), and F3 landed at +41
real against +31 vendored (row 8417). If a draft's real count decides your verdict, name the
`--refresh` run, its size and why; it comes out of the sitting's 10 USD and is the author's to allow.

## The questions

1. **B1.** Does § 13 owe the rung? A reader of the base's spec is told threads come from `hero_os.h`
   and nothing of their names, signatures or what crosses. Run what such a reader plausibly writes and
   what the compiler does with it: a guessed name; a `pthread_create` bound from `pthread.h`, whose
   Heroes body on a thread C made is stopped at run time by `runtime/parts/thread.c`'s guard (panel
   114's R4: *"panic: main.worker ran on a thread this program did not start"*, exit 134, `grep -n
   'main.worker' docs/panel/114-*.md`: run it today); a `str` body (00-shared.md: refused at build by
   clang). T1 against T2 against nothing: the mistake each prevents and the error it meets without it.
2. **B1 and F3 share one sentence.** F3 has landed in line 377 (00-shared.md § Part B). Price T1
   appended after it against T1 merged into it (*"threads, processes and files from ... `hero_os.h`"*
   carrying the rung), and say which a reader reads as one rule.
3. **Which rung the spec would promise.** Panel 113's R5 ratified *"the scalars and `str`"*; journal
   034:79 records `i64` alone with no reason (00-shared.md, both quoted). Panel 232's paragraph
   (`docs/panel/232-*.md:272-274`) promises *"the functions `hero_os.h` declares at 1.0.0, their names,
   their types and what each result hands its caller"*: T1 writes `hero_thread_spawn`'s signature into
   the spec, and S1 or S2 would add doors to that header. For each candidate, which promise the author
   takes on and what a 1.x could no longer change.
4. **B2.** Principle 0 (CLAUDE.md § 2; design.md §1.0): the compiler does not need a stop to compile
   itself; what measured thesis effect, or what pre-registered prediction whose instrument exists
   today, pays for S1, S2 or S3, if any; whether routes (a), (b) and (c) need a spec sentence at all
   (a stop the layer owns may be the card's, not the spec's); whether a stop belongs in the spec, in a
   package's card, or in `hero_os.h`'s comments alone. S3 is a built-in: what the `spec` suite's
   built-ins table and `selfhost/inventory.hero` then owe.
5. **Panel 114's R2, ratified 2026-09-06 and never in the spec** (`docs/panel/114-*.md:176-190`; `grep
   -n -i ships spec/heroes-spec.md` prints nothing): *"The header may be one the program ships beside
   its own file, so a difference between machines is written in C, where `#ifdef` already exists."*,
   +31 vendored at the base, with R2's own named removal (§ Strings' quadratic sentence, R2's *"−31"*;
   `grep -n 'quadratic' spec/heroes-spec.md` says whether it still stands). Is it owed on its own,
   apart from B2? And does it now invite the two-module trap 00-shared.md measured (a program's own
   `stopflag.h`, one flag per C unit, Ctrl-C swallowed at exit 0)?
6. **B3.** A connection is a handle (panel 234's R3; `runtime/hero_net.h:45-46`), so *"one connection
   per thread"* is unwritable under T1 (00-shared.md measured a handle body refused). Whether the spec
   owes a sentence on that, and whether anything of copy-on-write's window owes a spec token; design.md
   Part 7 item 13 calls § 2's *"There are no mutable globals"* *"the spec-side twin"* that *"stays as
   written"* (`grep -n 'stays as written' docs/design.md`: 4079; the spec's sentence at line 135).
7. **The payment**: for each candidate you would admit, the named removal or the pre-registered
   falsifiable prediction, its instrument named.

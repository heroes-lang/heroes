# 032 — M-corpus-depth: the rung between a program and the compiler

Closed 2026-09-04 · tag `m-corpus-depth`

## Goal

Nine programs for `examples/`, chosen for **shape** rather than for a language
form — because form coverage reached zero unexercised at M-corpus-coverage
(journal 029) and what the corpus was measured thin in was size, depth, an
external oracle, and an FFI that is inside a program rather than beside one.

The measurement that opened it, over the 78 files the corpus held on
2026-09-03: the six directories that declared an `extern` group were 65 to 192
lines, every one single-module, and **not one** had a `variant`, a nested
container, a generic or an `@`-parameter structure. Generics were declared in
three programs and never crossed a `use`. Direct self-recursion lived in three,
at depths bounded by tiny inputs. One program checked an answer somebody else
had published. One wrote a file. And between `json/` at 671 lines and the
compiler at 50,452 there was **nothing** — where Nim keeps `tests/manyloc/`,
Zig `test/standalone/` and Rust `rustc-perf`'s pinned crates.

Delivered: **nine programs**, plus the two instrument items the milestone
carried and one architecture sitting it did not plan for.

| | before | after |
|---|---|---|
| program directories | 35 | **44** |
| `.hero` files in `examples/` | 78 | **108** |
| lines | 10,151 | **17,489** |
| `test` blocks | 349 | **505** |

## What surprised

**THE SITTING CONVENED TO SHARE SLOTS VETOED ITSELF TWICE, AND FOUND SOMETHING
BETTER.** `docs/panel/106` was called because `examples/interpreter/`'s
recursion ceiling was 104 under `--sanitize`, which is why the milestone's own
plan asked for a 500-deep expression and settled for 60. Both seats refused the
proposal — the ffi-pragmatist because a shared slot frees a string SQLite is
still reading (`length: 0` at exit 0, and **`--sanitize` hides it**) and turns a
`setvbuf` buffer into SIGSEGV; the compiler-engineer because after those
exclusions it is worth **0.05%** of declared frame bytes.

Then the same seat measured where the bytes actually are: the exit sweep mints
one load temporary per swept slot per returning block, every one address-taken
so clang can never promote it, and together they are **58.5% of every declared
frame**. Removing them:

| | before | after |
|---|---|---|
| interpreter frames, `-O0` | 639,792 bytes | **294,128** (−54.0%) |
| the compiler's own frames | 6,563,376 | **4,227,600** (−35.6%) |
| nesting ceiling, `-O0` | 191 | **314** |
| nesting ceiling, `--sanitize` | 104 | **166** |
| the seed | 846,804 lines | **721,238** |

**A defect on another platform repaired itself as a side effect.** Defect 008
— on the Windows box, the compiler built by the one clang line this contract
documents could not check `examples/query/main.hero`, exit 127, while CI stayed
green because CI passes a `/STACK` flag no document mentions — was measured on
2026-09-04 and gone by the evening, because the frames were 35.6% smaller. The
same commands that failed now answer exit 0 and the compiler compiles its own
178 modules there. **The defect closed and its question did not**: CI still does
not run the documented instruction, and that is filed on its own.

**Every program that reached something new found something.** `readings/` did it
at the last milestone and the pattern held: `interpreter/` found two compiler
defects, `ledger/` found three things in one run — an `extern` is not callable
across a module boundary, a `ptr` may not be the element of a `T?`, and the
record you wrap a handle in emitted C that clang warned about (defect 009).
`query/` found three defects in **itself**, one of them the classic: the low
bits of a linear congruential generator are not random, so whole teams never
appeared, and a test written to check that all four regions show up was what
said so.

**The language corrected the author of these programs at least six times**, and
each correction is written in the file that met it: UFCS does not cross a module
boundary; `<` does not compare two `str` even though `sort` orders them;
`print` already ends with a newline; there are no exponent literals; two
parameters of one type must be named at the call site; and `ok(())` is `ok()`.

## What broke and why

**My first repair for defect 009 was wrong, and the instrument caught it in one
blessing.** It decided whether to declare a variable by asking a predicate about
what the emitter's loop *would* produce, instead of looking at what it *did*. A
fixed-array field answers no to that predicate while the loop still writes
`&v->projection[0]`, so a record whose only field is a `Matrix[2]` lost its
declaration and its golden became `error: use of undeclared identifier 'v'`.
That is CLAUDE.md §11's own shape — a narrowing resting on a premise about the
world rather than on a fact about the value in hand — committed inside the
repair for a defect. The second version collects the lines it is about to write
and declares the variable only if one of them mentions it.

**A record item was ticked because the work under it was done, and the work was
never what it asked.** The panel 106 entry in `DECIDE.md` was the *question
standing in for the author*, not a task; moving it to the record left the panel
file saying "the open item exists" when it did not, and `records/verdicts` went
red within the minute. "DECIDE.md holds zero open items again" was written in a
report as an achievement and was the symptom. A peer session caught it.

**Two pinned counts and three file ceilings went red exactly as designed.** The
mutate operator table said thirteen in two places — one of them in the file
whose own comment says the count is read and never written. Three files passed
their line ceilings when panel 106's reasoning was written into them, and one of
those was cut back to 293 rather than raised, because the story was already told
in the record and the code owed only the invariant and a citation.

**A prediction of my own is scored FALSE**, on `docs/panel/106`: I said the
interpreter's nesting case would at least triple. It went 60 to 83. The
ceilings roughly doubled, but the case is governed by the tightest
configuration and I had taken the `-O0` number. The measurement was never in
doubt; the arithmetic between the measurement and the claim was.

## Predictions, scored

| origin | prediction | scored |
|---|---|---|
| panel 106, compiler-engineer | with the repair landed and slot sharing not landed, the interpreter's `-O0` ceiling is **≥ 300** and `--emit-c` emits **≤ 60,000 lines** | **HOLDS**, on the better side of both: **314** and **54,610** |
| panel 106, coordinator | the interpreter's nesting case moves to **at least three times** 60 | **FALSE.** It is 83 — see above |
| measurement 015 | one `mutate` run over the whole corpus comes in under the 680 s the 36 per-directory runs took | **HELD BY 0.74 s AND FOR THE WRONG REASON**: 679.26 s, and the saving it credited (36 compiler start-ups) is not where the time goes — the cost is the mutants |
| the milestone's plan | a 500-deep expression | **NOT MET, with a measured reason.** `--sanitize` tops out at 166 and the corpus runs all three configurations, so the case is 83 and the evaluator's guard 38, both re-derived rather than chosen |

## What landed, and what carried forward

**Nine programs, in three families.** Five with a published oracle —
`fannkuch/`, `binarytrees/`, `checksum/`, `nbody/`, `spectral/` — where the
answer is somebody else's and the program's job is to agree with it. Two large
— `interpreter/` at eleven modules and `query/`, which hands the language 5,000
generated rows and is the first program here to make copy-on-write cost
something. One FFI at program scale, `ledger/`, which binds eighteen SQLite
functions and checks every total twice, against SQLite's own aggregate, because
nobody has published the answer for a book somebody invented. And `tally/`,
which the stdin work needed and which is checked against `wc`.

**Both instrument items closed.** `heroes mutate` reads its own corpus again —
it had been refusing `examples/` for a day and nothing said so, because that
command runs in no CI leg — and it now runs on every leg as a gate plus a full
score on the tag run. The fourteenth operator gives panel 102's refusal a
number: **1,700 mutants, 1,700 killed strict, 64 that only that rule kills**.
Over the whole corpus, 19,411 mutants at 95% and 83%, and **the rates did not
move** from measurement 014 over a corpus nine programs smaller.

**The corpus can be fed.** `HERO_RUNTIME_ABI` 19 → 20, `main.stdin` beside
`main.args`, and the price was measured before it was paid — one file handle
per platform, which was the gate its scheduling item set.

**What carried forward**, each with its number and its home: the store's own
`old` temporary, 9.1% more of declared frame bytes, which genuinely needs a
temporary; CI's Windows leg still not running the documented clang line, with
three shapes and a recommendation in `SCHEDULED.md`; and `swap-args` at 80%
strict, 452 survivors, the weakest live operator and the one to attack next.

**And panel 106 is still provisional.** The repair is built, measured and
shipped; the resolution has not been ratified, and its item stands open in
`DECIDE.md` naming the sitting — which is the one thing in this milestone that
is not the assistant's to close.

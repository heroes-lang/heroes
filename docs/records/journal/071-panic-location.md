# 071 — M-panic-location: a stop says where, an assert shows both sides, and no text forges a report

## Goal

Row 72 of the chain, opened on 2026-10-26 (`595d92d88`) in the lane
`panic-location`: a panic names its `.hero` file, line, column and function; a
failed `assert` on any value shows both sides; a program's own text can never
forge a line of a report; and `exit` takes a byte. One sitting, panel 230, on
the three questions and on issue 1001 (`main -> ()?`), its resolution adopted
provisionally and ratified by the author's reading the same evening
(https://github.com/heroes-lang/heroes/issues/2227). **The acceptance**: R7's
sentence, every abort a program's own operation raises names its place, a
fault the runtime catches names the function where the platform can tell, an
`assert` on any value shows both sides, and no program's text can forge a
report; the batch's gate green on the tree that became the trunk.

## What surprised

**The library had no place of its own to give.** An abort inside `args()` or
`range` named a line of `<heroes library>`, which no reader can open, and the
place a reader needs is the program's call into it. The library's functions
now take their caller's place as a last C parameter, Rust's
`#[track_caller]` for the library alone and with no surface; a function of the
library standing as a value is reached through a twin with the parameters the
program wrote (step 7).

**A name the walk finds is not a name the program wrote.** The stack guard
reads C symbols, whose module component is the module made an identifier, so
`thread_deep` read `threaddeep`. The program's `main` hands the runtime the
spellings that differ before anything runs, read by the handler by bytes alone
(step 8). Windows walks no frames and names no function, as R1 said.

**R7 wrote down a limit the author then lifted.** The sitting said an unended
lease names no acquisition; asked what the wide promise would still need, the
author answered *go on as far as you can*, and a lease never ended and a C
handle never given back now name where each was taken (step 9). The lease's
place lives in a table of the runtime's own, never in the block's header,
which a C function writing before its pointer reaches (defect 076).

**Two equal texts cannot show their order, and that is the whole map rule.**
R2 orders a map's entries by their text so a side is a function of the value;
the first draft reached for a tie-break, and none is needed: two entries whose
texts are equal are interchangeable in the output. The lldb formatter orders
its summary the same way, and one corpus, `lldb/notation`, holds the two
notations to one by a check that compiles each value again as a test (step
10). § 12's new sentence was priced at -21 real by the sitting and measured
-21.

**The names the compiler invents shared the author's namespace.** Choosing a
name for a side's renderer found `function Point_eq` beside a compared
`record Point` refused by clang at exit 2, a correct program; defect 2234,
filed `blocking` and repaired the same evening by a digit after `h_<module>_`
in every name the compiler invents there.

**A gate filtered to a step's forms misses what another step moved.**
`lldb/lookup-block` broke on `hero_map_find` from step 6a, when a map lookup
began calling its place twin; it was found at step 10, when the `lldb` suite
ran for the formatter. The batch's full net would have found it too, which is
the optimistic chain's bet, and it was found before the gate.

## What broke and why

- **A module named `place` was shadowed** by every `place` binding of the
  emitter; renamed `stop_place`.
- **Ceilings, six times**: `main.hero`, `ir/flatten.hero`, `library_source`,
  `ir.hero`, `emit/inst.hero` and `emit/ctype.hero` each met its `DECIDED`
  line; each was answered by moving code to where it belongs (`runtime_hint`
  to `toolchain`, the abort dispatch to `abort.stop`) or by saying it once,
  never by raising a ceiling.
- **A stage A built against a runtime one step ahead** after an unreleased
  `_at` signature changed: built against the previous commit's runtime, the
  route written into the step's body.
- **`emission` takes no case filter**, so blessing the two cases this Mac skips
  in the Linux container ran the whole suite (1,397 and 0) and kept two files.
- **The first census was silently partial**: macOS's `xargs -I` builds at most
  255 bytes, and the long `fixedbugs-` paths stopped it; rerun by a parallel
  driver, 4,198 files, one newly refused, the case R4 wrote to be.

## What landed, and what carried forward

Steps 0 to 12 and defect 2234's repair, pushed to `main` at `e4ad8e14c` on
2026-10-26 with the author's yes, 19 commits:

- R3 (step 3): a value in a report quoted, a message by its codes; defects
  2225 and 2226 closed.
- R4 (step 4): `exit(code: u8)`; defect 2224 closed.
- R1 (steps 5, 6a to 6c, 7, 8): every abort names its place, kind 1 as four
  literals, kind 2 as a record per site, the library's caller place, the walk's
  spellings; step 9 past R7, what was never given back names where it was
  taken.
- R2 (step 10): both sides of every comparison, the formatter's map order, the
  notation test, § 12 at 10,276 real.
- R5: `main -> ()?` not taken; `main_returns` has its golden (step 11).
- Defect 2234: invented names in a digit-led namespace.
- Step 12: the seed refreshed by `abi-put-back`, rebuilt by the seed before it,
  its fixpoint over two generations.

**Re-measured at the gate, 2026-10-26**: the full net 32 suites, 9,419 passed
and 0 failed; the compiler's own 1,762, the net's own 428; the site's build
green; the two emissions this Mac skips blessed on Linux arm64; the census
after the push, one newly refused file and that one meant.

**Carried forward**: the CI's run 40401936020 to read, panel 187's R2 after
the push and the author's W4 and W5, on the next row
(https://github.com/heroes-lang/heroes/issues/2236); the old entry points
without a place, removed after the release whose seed knows the new ones, on
M-compatibility-promise (https://github.com/heroes-lang/heroes/issues/2237);
and the decisions 2210 (panel 229) and 2228 (R6's door, `hero_os.h`), put to
the author with their recommendations at the close.

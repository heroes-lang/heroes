---
kind: defect
area: cli
milestone: none
filed: 2026-09-27
commit: f39836a6df00ceceb5ecd36314b60a2da5fb9401
github: none
---

# Defect 105 closed: every artifact a verb prints is linear in its size, and `fmt` is faster than before its guard

2026-09-27, M-agreed-retention step 20, in lane `95dc08fe`, merged `09acdabc`. Found by the
coordinator timing lane g's merge for defect 101's record; measured on a
ladder and profiled before filing.

- [x] **105 — `heroes lex --dump-tokens` and `heroes fmt` build what they print by appending to one string, so both are quadratic in the size of their output** | every token line and every printed line is appended as `out @ out + …` or `f.out @ f.out + …`, and a string concatenation copies the whole string under value semantics (design.md Part 8 wart 8, which panel 144 left a wart because the cost is in the spelling, naming accumulation into an array joined once as the cheap one) | `selfhost/cli/lex.hero:36-71` (`json`, `text_dump`) · `selfhost/print/page.hero:70-82,294-299` (`put_line`, `blank_line`) · `selfhost/print/margins.hero:32-54` (`deepened`) · **closed 2026-09-27**

    **Origin:** the coordinator, 2026-09-27, timing lane g's merge: `fmt` on
    1 to 32 copies of `walk.hero` read 0.28, 0.58, 1.25, 2.80, 6.86 and
    21.15 s user, and `lex --dump-tokens` 50.04 s at 8 copies and 194.31 at
    16. The filing named the class, not the two witnesses: every artifact a
    verb prints, measured on a ladder, and a check where a check can read it.

## The enumeration, and where it came from

The verbs and flags are `selfhost/cli/table.hero`'s, the one argv table:
`lex` (`--dump-tokens`, `--json`), `parse` (`--dump-ast`), `check`
(`--dump-scopes`, `--json`, `--brief`, `--permissive`, `--apply`), `build`
(`--dump-ir`, `--emit-c`, the binary), `run`, `test`, `fmt`, `doctor`,
`mutate` (`--survivors`), `measure`, `grammar`, `this`, and `--help` and
`--version` outside it; what each prints was traced from its
`io.print_artifact` or `io.eprint_raw` call to the function that builds the
text. Each was timed on a ladder, `user` seconds, the trunk's compiler at
`29ed5601` built from its seed with the plain line of CLAUDE.md § Commands:
copies of `selfhost/check/walk.hero` (108 KB) for the verbs that need no
checked program, and generated inputs for those that do (their generators
are in the lane's scratch: N units of a record, a variant and a function
with an `if`, a `while`, a `for` and a `match`; N functions each with one
diagnostic, `fn` with a certain fix or an unused binding; N functions each
with one hole).

| verb, input | before | after |
|---|---|---|
| `lex`, walk.hero ×1 ×2 ×4 ×8 (×16 ×32 after) | 0.07 0.11 0.22 0.44 | 0.06 0.11 0.22 0.45 0.93 1.86 |
| `lex --dump-tokens`, ×1 ×2 ×4 ×8 (×16 ×32 after) | 0.58 2.60 12.81 49.94 | 0.12 0.24 0.50 0.99 1.99 4.02 |
| `lex --dump-tokens --json`, ×1 ×2 ×4 (×8 ×16 ×32 after) | 3.38 13.50 57.11 | 0.13 0.27 0.54 1.09 2.18 4.35 |
| `parse --dump-ast`, ×1 ×2 ×4 ×8 (×16 ×32 after) | 0.08 0.17 0.34 0.68 | 0.08 0.16 0.32 0.63 1.26 2.52 |
| `fmt`, ×1 ×2 ×4 ×8 ×16 ×32 | 0.28 0.59 1.26 2.84 7.07 20.70 | 0.18 0.36 0.73 1.47 2.95 5.93 |
| `fmt`, 250 500 1000 2000 units | 0.84 1.84 4.30 11.24 | 0.51 1.04 2.07 4.19 |
| `check`, 250 500 1000 2000 units | 0.53 1.39 4.05 13.13 | 0.53 1.35 3.92 12.73 |
| `check --dump-scopes`, the same | 0.61 1.66 5.00 16.83 | 0.54 1.37 3.95 12.76 |
| `build --dump-ir`, the same | 1.23 3.06 8.59 27.57 | 1.14 2.85 7.99 25.31 |
| `build --emit-c`, the same | 3.57 8.14 21.55 64.42 | 3.35 7.62 20.35 61.62 |
| `build`, 250 500 1000 units | 2.76 6.06 15.89 | 2.63 5.95 15.47 |
| `run`, the same | 3.78 8.74 23.66 | 3.61 8.27 21.76 |
| `test`, the same | 3.31 7.57 19.43 | 2.95 6.63 16.92 |
| `check`, 500 1000 2000 4000 diagnostics with a certain fix | 0.95 3.26 12.63 49.17 | 0.03 0.07 0.14 0.28 |
| `check --brief`, the same | 0.03 0.07 0.13 0.27 | 0.03 0.06 0.13 0.26 |
| `check --json`, the same | 0.05 0.14 0.55 1.86 | 0.04 0.07 0.15 0.30 |
| `check --permissive`, the same | 0.95 3.30 12.61 51.96 | 0.03 0.07 0.14 0.29 |
| `check --apply`, the same | 0.04 0.11 0.31 1.05 | 0.03 0.06 0.13 0.26 |
| `check`, 500 1000 2000 4000 unused bindings | 1.09 3.74 14.13 51.30 | 0.08 0.17 0.45 1.32 |
| `check --brief`, the same | 0.09 0.22 0.61 1.76 | 0.07 0.17 0.45 1.31 |
| `check --json`, the same | 0.11 0.27 0.87 2.87 | 0.08 0.18 0.46 1.35 |
| `check --permissive`, the same | 0.30 0.87 2.84 9.33 | 0.25 0.74 2.40 8.55 |
| `check`, 100 200 400 800 holes | 0.05 0.12 0.32 0.98 | 0.05 0.12 0.31 0.91 |
| `measure`, the spec ×1 ×2 ×4 ×8 | 0.21 0.33 0.55 0.98 | 0.21 0.32 0.53 0.96 |

`grammar`, `this`, `doctor`, `--help` and `--version` print a text whose
size does not depend on any input, 0.00 to 0.02 s; there is no ladder to
take. `mutate` checks each mutant, so its time is the mutants' and not the
report's; its reports are among the texts repaired.

What grew faster than linear because of what it printed: `lex
--dump-tokens` both forms, `fmt`, the rich diagnostics and `--permissive`
(the same renderer), `check --json`, `check --apply` and `check
--dump-scopes`; and a part of `build --dump-ir` and of the holes report,
whose growth is mostly their pipeline's. What grew faster than linear for
another reason, found on the same ladders and not repaired here, is at the
end.

## The repair

- **`fmt`'s page is its lines** (`selfhost/print/sheet.hero`, new). A
  `Sheet` holds the page as an array of whole lines and its size in bytes;
  a line is pushed through an `@` parameter, so the push grows in place, and
  the page is joined once. Every read the printer made of the old string is
  answered exactly: the offset a head keeps to come back to is `size`; the
  last bytes `blank_line` and a trailing comment ask for are read from the
  last lines; and `margins.deepened`, which rewrote the page from an offset
  and copied the whole of it to change a head's last lines, walks back from
  the end to the line holding the offset and rewrites only the lines after
  it, in place. The invariant that makes that rewrite the old one, every
  element one or more whole lines ending in its `\n`, is a fact about the
  value, since every writer is in that module, and its tests hold it.
- **The token dumps** push one row per token onto a local and join once.
- **The class, at every site a check can read**: 127 lines in 39 modules of
  the trunk grew a text the way `layout/concat` below reads, and each pushes
  its pieces onto a `[str]` and joins once, or pads with one `repeat`. Among them the other
  printers the ladder found: `check --json`, `--dump-scopes`, `--dump-ir`,
  the AST dump, the holes report, the rich diagnostic, the mutation report,
  `grammar`, `--help`; and the compilation's own text, `source.from_files`,
  which copied everything read so far at every module of the program.
- **What the ladder found that is not a concatenation**, each on a printed
  artifact's path: the rich renderer split the whole compilation into
  characters to find one line, per diagnostic (4000 diagnostics 49 s), and
  now slices the line from the line table; `--dump-scopes` read every local
  of the file once per declaration, and now takes them grouped by owner;
  the diagnostics, `fmt`'s items and `--apply`'s fixes were ordered by
  inserting each into a copy of the list so far, and are ordered by
  `selfhost/stable.hero`, a merge sort that keeps equal keys in their order,
  tested against the insertion it replaces on every list of five keys from 0
  to 3; `--apply` rebuilt the whole file per fix, and applies fixes that do
  not overlap in one pass, the overlapping ones as before;
  `io.print_artifact` split the artifact into characters to read its last
  byte, a string per character of the generated C; `source.line_of`
  counted the characters of the column only to throw them away, 181 of
  2,154 samples of `fmt` on eight copies; and `source.file_of` copied the
  file table into an array at every call.
- **The guard's recomputation**, from the profile the author asked for:
  `fmt` parsed the file it formats twice, once to format it and once, alone,
  for the guard, and read it alone both times, so the guard is handed the
  first parse where the source holds the file alone (a fact about the
  source); the owner rule read each of the two files twice, once for the
  printer and once for the comment guard, and `owners.Lines` now records how
  it was read, so the guard takes the printer's lines where they answer its
  own question and reads them again where they do not; and the rule searched
  for each token's line where the tokens come in order, and walks the line
  starts forward. Every check stays: the output is parsed, formatted again,
  dumped and compared, and every comment's place asked of both files.

## The check

`suite_layout`'s `layout/concat` reads every module of the compiler, as
`layout/appends` does, for three shapes (`tests/harness/growth.hero`): a
`str` local grown by `+` inside a loop that runs with it declared, a match
arm's body included; a field grown by a line, the formatter's page, whose
loop is its caller's; and a lent `str` parameter grown at all. Its comment
says what it cannot see: growth through a temporary, a text prepended to
(the number renderers, whose loops end with the number's digits), a field
grown without a newline, a helper returning `text + x` to a loop, and a
whole-text copy per item that is not a concatenation; those were found by a
ladder, which is their instrument. One allowance, checked both ways:
`selfhost/emit/container.hero`'s `lvalue`, a C lvalue that every index step
wraps whole in a call, so pieces would be joined at every step; an
allowance no growth matches fails as stale. Run over the trunk's own
`selfhost/` at `29ed5601`, it names 127 lines. Its tests are in the net's
own: the three shapes, their neighbours that are not growths, literals and
comments emptied, and the allowance both ways.

## What it does not change

Design.md Part 8 wart 8 stays a wart, on panel 144's ruling; this was the
compiler's own code using the spelling the ruling calls slow. Two crashes the
rewrite met are kept exactly, since this is a refactor and a crash is a
different defect: `heroes check` and `heroes lex` on an empty file panic
`string index out of range` at exit 134 (`source.from_files` reads the last
byte of the first file, which has none), and `heroes build` or `run` of
`print(f"é {x}")` panics `string slice splits a character` at exit 134
(`lex_interp.piece_text` slices one byte of a longer character).

**Found on the ladders and not repaired here**, because the cost is not in
what is printed:

- **`check` is quadratic in the calls and declarations of a program, and it
  is 89% of `heroes check selfhost/main.hero`.** `check/freer.marked_as_freer`
  reads every declaration and every parameter of the program at every call
  to a user function, asking whether one names it as a freer: 13,715 of
  15,411 samples of that check, and 3,506 of 6,172 on the 2000-unit ladder,
  whose `check` reads 0.53, 1.35, 3.92, 12.73 s at 250 to 2000 units. And
  `resolved.declare_top` copies a module's whole map of names at every
  declaration (`inner @ found.must()`, then a store the shared map unshares
  on), 1,039 of those 6,172. Both are one pass read once where they are
  read per item; neither prints anything.
- **Emission is quadratic in the size of a function.**
  `emit/unread.assigned` scans every instruction of a function for each
  temporary it declares: 2,196 of 6,633 samples of `build --emit-c` on the
  1000-unit program, whose `main` makes 1000 calls. `--emit-c` less `check`
  reads 2.82, 6.27, 16.43, 48.89 s at 250 to 2000 units, and `--dump-ir`
  less `check` 0.61, 1.50, 4.07, 12.58, the lowering's share
  (`ir/flatten.call` and `ir/owned_release.library_validated` lead it,
  unprofiled further).
- **The holes report reads every local and every top-level name of the
  program once per hole**, `in_scope` and `nearby`: 0.05, 0.12, 0.31, 0.91 s
  at 100 to 800 holes. What it prints per hole is capped (§4.16); what it
  reads is not.
- **The lexer's keyword lookups are a fifth of `fmt`.** `keywords.keyword`
  and `keywords.foreign_word` are `match` over a string with twenty and more
  arms, and the emitted C zero-initialises every arm's `T?` temporary at the
  function's entry: `memset` is 240 of 1,113 samples of `fmt` on eight
  copies after this repair, the lexer 297, and every verb lexes. The cost is
  the emitter's, one `= {0}` per temporary.
- **The emitter's `Writer`** pushes each line onto its chunk through a field
  (`w.chunk @ w.chunk.push(…)`), which copies the chunk, up to 128 lines, at
  every line: linear, with that constant; `Writer.line` is 520 of those
  6,633 samples.
- **Insertion into a copy of the list** stays in four places this lane did
  not need, each over a list the ladder did not grow: `modules.hero:181`,
  `resolve/cycles.hero:88`, `ir/verify.hero:159`, `emit/gate.hero:138`.
  Unmeasured.

## The measurements

The table above, `user` seconds, one process at a time: before, the trunk's
compiler at `29ed5601`; after, this lane's, both built from their own seed
with the plain line of CLAUDE.md § Commands. `real` was within 5% of
`user` plus `sys` in every row of 0.3 s or more but the three that run clang
and the program (`build`, `run`, `test`), where it counts the children's
waits; the after ladder's first run, `lex` on one copy at 0.48 real for
0.09 user, was taken again, 0.06.

`fmt` is linear now, 0.185 s per copy of walk.hero, 1.7 s per MB, where it
was 0.28 at one copy and 20.70 at thirty-two; the token dumps 0.13 and 0.14
s per copy. `heroes fmt selfhost/check/walk.hero`, alternated with the
trunk's: 0.28 and 0.28 s user before, 0.18 and 0.18 after, below the 0.21 of
the trunk before lane g's guard. `heroes check selfhost/main.hero`: 26.59
and 26.59 before, 25.76 and 25.74 after. `real` within 0.05 s of `user` plus
`sys` in all eight runs; no other session's `heroes` process was running
before or after them.

**The profile of `fmt` on eight copies**, `sample` over the whole run,
samples of about a millisecond, before and after: the run 2,154 and 1,113;
the guard 1,435 and 715; the two formatting passes 1,001 and 367, the page's
`put_line` 403 and 11; `source.line_of` 181 and 32; the owner rule's
`shape` 156 and 41, read twice where it was four times; the comment guard
270 and 154; the guard's parses 544 and 273, one where there were two. The
largest single cost left is the lexer, 297, and `memset` is 240, most of it
under the lexer's keyword and punctuation lookups (below).

## Identity

Both compilers over the same inputs, stdout, stderr and the exit code byte
for byte: `fmt` over every `.hero` file under `selfhost/`, `tests/` and
`examples/`, 949 of 949 identical, and over lane g's 826 generator
reproducers, 826 of 826; `lex --dump-tokens` and `--json` over the 263
modules of `selfhost/`; `parse --dump-ast` over the 949; `check`, `--brief`,
`--json`, `--apply` and `--dump-scopes` over the 149 programs of
`tests/golden/check/`; `--dump-scopes`, `--dump-ir` and `--emit-c` over the
54 examples and `--dump-ir` and `--emit-c` over the 206 programs of
`tests/golden/run/`; every verb above over the ladder's own inputs up to
walk.hero four times over, 2000 diagnostics and 800 holes; `mutate
--survivors` over three examples; `grammar`, `this`, `doctor`, `measure`
three ways, `--help` and `--version`. Every one identical.

## The gate

In the lane, `95dc08fe`, with the compiler built from the regenerated seed:
the compiler's 761 tests, five of them new (the sheet's two, `stable`'s, the
owner rule's line walk and the guard's reuse of the printer's lines); the
suites one at a time, canonical 2, layout 4 (`layout/concat` the fourth),
order 3, records 24, surface 167, annotations 191, fixes 25, check 150,
lines 207, corpus 55, determinism 236, emission 628, run 206 and warnings
267, then ir 24, emit 8, unsupported 15, grammar 9, spec 20, special 10,
descriptors 298, cache 6, units 3 and runtime 8: all 24 suites, 2566 passed
and 0 failed, one more than the trunk's 2565 in defect 101's record, the
new `layout/concat`; the net's own 171, three of them new (`growth`'s two and
the allowance's). The seed regenerated by the lane's compiler, sha256
`bd98fac2f6aa0d35…`, the compiler rebuilt from it with clang, the seed
emitted again and `cmp` equal: the fixpoint held. Unrun in the lane: the net as
one run rather than suite by suite, and the other three platforms.

On the trunk after the merge, `09acdabc`: the index equal to the lane's tree
under `selfhost/`, `seed/`, `tests/`, `runtime/`, `examples/` and `spec/`; the
compiler rebuilt from the seed and the fixpoint held (`bd98fac2f6aa0d35`); the
compiler's 761 tests, the net's own 171, `records` 24 and `layout` 4. Linux arm64 and x86-64, the merged tree copied into each: the compiler's
761 tests, surface 167, canonical 2, annotations 191, check 150, fixes 25,
layout 4, lines 203 and run 202, every one 0 failed. The Windows box: the
compiler's 761, surface 160, canonical 2, annotations 191, check 150, fixes 25,
layout 4 and lines 201, 0 failed, and run 199 passed and 1 failed, a runtime
message without its offset that the trunk before this merge prints the same
way on the box: defect 115, older than the lane.

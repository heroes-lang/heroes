# 064 — M-typed-inspection: a stopped program shows Heroes values, under the names C gave them

## Goal

The milestone's page, opened at row 65 by the author's answers of 2026-10-12
(https://github.com/heroes-lang/heroes/issues/2029): a program built at `-O0`
and stopped in a debugger, lldb directly or the editor's lldb-dap, should show
what the author wrote, the name they typed and a record, a `[T]`, a `{K: V}`, a
`T?` and a variant as themselves, in a frame without the emitted C's
temporaries. The `lldb` suite came first, since the claim had no live test
(step 2, issue 944), then the census of what a frame holds (step 3, measurement
042, issue 942); panel 213 ruled the mechanism (issue 939, decision issue
2037, provisional); the emitter keeps what it invented out of the debug
information (steps 4 and 4b), one fixed formatter shows the values and the
editor loads it (step 5), design.md's debugger non-goal says what landed
(R10), and measurement 043 is the after-table. The author's names are not
delivered: R4 refuses them in the emitted C and leaves the debug-information
route to the author's answer on 2037.

## What surprised

- **A frame can lose nine tenths of its locals and show nothing false.**
  `nodebug` on every temporary and every slot the lowering invented leaves
  a hidden slot with no debug entry at all, so it cannot read stale bytes
  under a name; the three census frames went from 138, 121 and 111 locals to
  10, 7 and 11, every one the author's (measurement 043), and the cold build
  of the compiler used less CPU, 64.16 s against 67.01.
- **The author's name in the emitted C breaks correct programs.** Under the
  build's own `-std=gnu11`, `linux = 1` builds on this Mac and is refused on
  Linux, `errno` is refused everywhere and `stdin` builds renamed `__stdinp`;
  and two slots of one spelling renamed to one name showed the other arm's
  value at exit 0. The `h<k>_` prefix is what keeps a Heroes name out of
  every included header's macros (panel 213's compiler-engineer and
  ffi-pragmatist).
- **Which lldb runs decides which route exists.** A formatter embedded in the
  binary loads from LLVM 20, and the CI's Linux legs install lldb 18; LLVM 23,
  released in August, loads synthetic children from the binary but cannot make
  an element at an address, which only LLVM's development line can. The
  completeness critic read the source of `release/22.x` and `main` and not
  `release/23.x`, and the coordinator found the third branch before the
  synthesis was committed.
- **clang on x86-64 drops a jump that arm64 keeps.** The entry `goto` to the
  block that follows was the one instruction on a function's own line on
  arm64; on x86-64 clang emitted none there, so `step` entered a function one
  line later, and design.md §3.1's promise, panel 200's, held on one
  architecture (defect 2038, a `nop` at `-O0` only).
- **A check that reads GitHub today judges every older tree.** Opening panel
  213's decision before its sitting reached the trunk turned the trunk's CI
  red on all four legs (defect 2039): the tree under test is placed by the
  highest sitting it holds since.
- **One repair at the C boundary found the next.** Defect 2034's `-Wformat`
  in a header's region showed 2035 (a program under `build/` told as the
  compiler's fault) and 2036 (correct formats refused); 2036's repair showed
  2040 (`NULL`'s definition blamed on the runtime); 2040's showed 2044 (a
  header's `int64_t` blamed on the runtime's identical re-declaration), and
  2044's showed 2045 (a runtime name declared again), 2045's showed four
  more (2049), and 2049's two (2051, 2052). Each moved who is told, never what
  is refused (defects 060's and 360's precedent), and the author asked at the
  close for a sitting on the rule itself, since six repairs went one shape at
  a time.
- **The Windows box found what two platforms could not.** Started by the
  author the next evening and run on the round's tree before its push, it
  read defect 2045's case in other words, and behind them a header that
  defines a runtime function again as `static`, a Microsoft extension clang
  accepts for that target: built where unused, a duplicate symbol at the link
  where used (defect 2053). No `static` shape reads alike on all three
  platforms, so its cases are judged where their words are.

## What broke and why

- **The CI on the night's first push went red**, twice over: `records/prior-art`
  on every leg, the coordinator having opened a decision whose file had not
  reached the trunk (defect 2039), and `lldb/stepping` on Linux x86-64, the
  suite's first run there (defect 2038). Both repaired before the second push.
- **A seat's evidence carried bytes no reader sees**: two Linux transcripts
  kept as lldb-dap and lldb wrote them, control characters included; `unseen`
  found them and its `known` list excuses each place, the record unchanged.
- **The commit guard holds every staged `.hero` file to `fmt`**, and the
  sitting's sample program was not canonical on purpose, its line numbers
  being what five real frames cite; it was renamed `program.hero.txt`, its
  bytes unchanged, a dated line in its README saying why.
- **A comment on GitHub carried a mistyped hash**, written from a short form;
  corrected by a second comment, a published text not being rewritten.
- **The session stood still for two hours** between two of its turns on the
  morning of 2026-10-13, and the Mac slept through most of the day after, its
  lid closed on the move; the full net started at 07:18 had read 8 of its 30
  suites by 11:20 and was stopped, its tree being outdated by then.

## What landed, and what carried forward

**The milestone's items, closed on GitHub**: the `lldb` suite (issue 944,
`a5ca08f3`), the census (942, `09213724`), the sitting on the mechanism
(939), panel 213 provisional with its decision open (2037, `8a15d5be`), the
emitter's hidden slots and scratch locals (steps 4 and 4b, `c6f4d015`,
`5ff2bb04`), the formatter and the editor's loading (step 5, `9f025f01`),
design.md's R10 (`e2dd21e7`) and measurement 043 (`b372f272`). The defects
repaired in the round: 2034 (`2190ff8a`), 2035 (`afd34b4c`), 2036
(`4dcc1e19`), 2038 (`1df70eda`), 2039 (`8020e18f`), 2040 (`1e888df1`), 2044
(`0c7eae19`), 2045 (`31a112ae`), 2049 (`801e5106`), 2051 (`d7fd9c11`), 2052
(`602071cc`) and 2053 (`de6f0f3e`); the author's format ruling, a known cost,
is decision 2048 (`5a92815d`). Filed and left by their class: 2041, 2042,
2047 and 2054 (`adjacent`), 2043, 2046 and 2050 (`improvement`).

**Beside the milestone, by the author's instruction of 2026-10-13**: the
claim became *A language for the age of LLMs*, *Un linguaggio per l'era degli
LLM*, on the site, the README, the banners, the cards, the example and the
specification's opening line (`7bb804f4`, ledger row 111, `4e287781`).

**The gate, 2026-10-13 22:15 to 22:58, on `866d1b0e`, the compiler built
from the seed regenerated over the round's merges, its fixpoint verified by
`cmp`** (`seed/heroes.c` 1,348,532 lines, 57,395,081 bytes): the compiler's
own tests 1637 passed and the net's own 396; the whole net 8,436 passed over
30 suites, 0 failed: emission 1251, annotations 1097, fixes 996, check 699,
wholes 606, descriptors 606, warnings 589, determinism 550, lines 512, run
511, surface 424, unsupported 279, corpus 70, full 38, spec 31, ir 29, probe
27, records 26, permissive 21, emit 12, lldb 11, special 10, grammar 9,
runtime 8, cache 7, layout 6, unseen 3, units 3, order 3, canonical 2. The
site's build checked its 38 claims over 188 pages. **The Windows box**, on
the same tree, read unsupported 270, annotations 1097 and emit 12, 0 failed;
**Linux arm64**, in its container, blessed the two emissions this Mac skips
at each merge that moved them, every other file equal to the Mac's. The CI's
four legs were green at `0c7eae19`, the push before this one.

**Status at the close**, for `docs/ROADMAP.md` § Where we are: the compiler
624 modules and 163,392 lines of Heroes, the seed 1,348,532 lines of C; the
spec 7655 vendored and 10,048 real against 10240; the contract 8167 real,
unchanged; sittings 211, journals 65 numbered files, milestone pages 54,
measurements 42, beats 132, questions 472 in `docs/learn/` of which 447 open.

**Carried**: the author's ratification of panel 213 and its two questions
(the names, CLAUDE.md § 10's reading), issue 2037; the sitting the author
asked for on who is told when clang refuses a header of the program's own;
the release v0.4.0, the author's act; the repository's description on
GitHub, the author's to change to the new claim; the formatter's probe
M-inferred-cell's changes still owe.

**The chain entry**: row 65, `m-typed-inspection`, closed 2026-10-13, this
journal.

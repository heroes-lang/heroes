# Panel 219, the completeness critic

Written 2026-10-17 from 09:51 as the work ran, in a detached worktree at `cbf724983`,
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/219-critic` (removed at the end), its
compiler built from its own seed (`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`,
`real 8.21 user 7.98`), then the compiler-engineer's patch applied
(`git apply .../219-ce/s4-writer-and-lines.diff`, `git diff --numstat` 112 and 11 in
`selfhost/emit/writer.hero`, 17 and 2 in `tests/harness/suite_lines.hero`, as the seat wrote).
Scratch files and scripts in
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/5f7a29c1-133b-4b38-877a-3b0496db6842/scratchpad/219-critic/`
(`parts.py`, the dead-function counts, the reproducers under `ie2/`, `r3/`, `asan/`, `cov/`,
`hist/`). Compilers: `heroes-0`, the seed's (the old writer); `heroes-w1`, built by `heroes-0`
from the patched tree; `heroes-w2`, clang on `heroes-w1`'s emission (s4). Every number names its
command; what was not run says *unrun*.

## Verdict in one paragraph

**s4 holds on every number I could re-run, and on every author-facing instrument I could
reach, with one cost it shares with s0 and one defect it does not cause.** The fixpoint, the
objects, the line tables, the six forms, the ASan frames and coverage's regions are as the seat
says. What s4 changes for an author is the C excerpt and column clang prints on the exit-2 path
(longer lines, and none at all past 4,096 bytes), and C-line coverage by hand. The cliff that
matters, a reader of clang's excerpt losing it past 4,096 bytes, is **already in s0** and
reproduces at the same size under both: a defect of the trunk, to be filed, not a reason against
s4. **"Nothing reaches 10 MB" holds even as a floor**: with every shape the seat named removed in
full, the two shapes nobody measured removed too, and the build-only root, the seed stays above
15 MB; what is left is identifier text, about 70% of s4's bytes, which no seat measured. A
compressed seed in the tree is refused by § 10 and by `seed/README.md`, and measured worse per
regeneration than raw s4. On the rhythm, `HERO_RUNTIME_ABI` moved 8 times in the last 30 days,
so *refreshed only when red* is OCaml's routine, not Zig's rare event, unless the runtime is
frozen beside the seed, the route the historian sourced and nobody priced: priced below.

## 1. The numbers, re-run

| claim | the seat's | mine | command |
|---|---|---|---|
| the seed as written | 51,514,006 bytes, 1,391,162 lines | **same** | `wc -lc seed/heroes.c` |
| its `#line`s | 621,943, 24,710,575 bytes | **same** | `grep -c '^#line '`, `grep '^#line ' \| wc -c` |
| s1, no `#line` | 26,803,431, `xz -9` 2,504,280 | **same** | `grep -v '^#line '`, `xz -9 -c \| wc -c` |
| s2, `#line N` no name | 32,868,681, `xz -9` 3,512,328 | **same** | the brief's `sed` |
| s0 `xz -9` | 3,983,808 | **same** | |
| s3 and s4 by `shapes.py` | 33,207,910 and 25,638,593; `xz -9` 3,541,656 and 2,655,336 | **same** | `python3 -I 219-ce/shapes.py seed/heroes.c <dir>` |
| s4 by the writer | 25,649,440 bytes, 257,497 lines | **25,649,882 bytes, 257,503 lines (+442, +6)** | `heroes-w1 build selfhost/main.hero --emit-c`, `wc -lc` |
| its `#line`s, bare ones, longest line | 55,071, 37,770, 13,638 | **same** | `grep -c '^#line '`, `grep -cE '^#line [0-9]+$'`, `awk` max length |
| s4 by the writer, `xz -9` | (not given) | 2,652,264 | `xz -9 -c \| wc -c` |
| the fixpoint | `cmp` silent | **holds**: clang on s4 exit 0 (`real 10.27 user 9.89`, load about 5, not a duration), its compiler re-emits s4, `cmp` exit 0 | `clang -I runtime s4.c runtime/runtime.c -o heroes-w2`; `heroes-w2 build selfhost/main.hero --emit-c`; `cmp` |
| object without `-g` | identical | **identical** | `clang -I runtime -c` of `heroes-0`'s and `heroes-w2`'s emission of the patched tree, `cmp` |
| line table, columns ignored | 0 of 1,336,288 addresses | **0 of 1,336,329**, 654 files named in each, 151,073 rows on the generated file | `clang -c -gline-tables-only`, `dwarfdump --debug-line`, `python3 -I 219-ce/linecmp2.py` |
| s2 is false | `s2.c:76` for `lexer.hero:76` | **holds**: s2's line table names one file, `s2.c` | `clang -c -gline-tables-only shapes/s2.c`, `dwarfdump --debug-line \| grep name:` |
| `content.py` shares of s1 | `T = L;` 10.5%, `L = T;` 6.2%, `G(&T)` 4.3%, `G(hero_slot_escape(&L))` 4.2%, branch 3.1% | **same** | `python3 -I 219-ce/content.py s1.c` |
| history, three seeds | s0 14,914 KiB, s4 10,235 KiB | **same** | the seat's method: throwaway repositories, `git gc --aggressive --prune=now`, `size-pack` |
| R1 and R2 in s4 | 22,142,500 and 20,631,868 (by `shapes.py` on the old writer's emission) | **22,154,727 and 20,644,628** by the s4 writer itself, the seat's `boot-r1.hero` and `boot-r2.hero` placed beside `selfhost/main.hero` in my worktree and removed | `heroes-w2 build selfhost/boot.hero --emit-c`, `wc -c` |
| no location in a panic | `grep -c '__LINE__\|__FILE__' seed/heroes.c` 0 | **0**, and none under `runtime/` | `grep -c`, `grep -rc` |

**The one number not reproduced**: s4 by the writer, 442 bytes and 6 lines more than the seat
wrote. Both runs emit the patched tree, whose own `writer.hero` is part of the emission, so a
different revision of the patch between the seat's emission and the diff it kept is the likely
cause (an inference, unrun). The `#line` counts and the longest line agree exactly.

**The emitter's speed**, re-timed on a still machine (load 3.1 to 3.3, `real` within 1.4 s of
`user` plus `sys` on every run), alternating, `--emit-c` of the patched `selfhost/main.hero`:
old `user` 39.39 and 39.34, new 39.83 and 39.81, **+1.1% to +1.2% in `user`** (the seat: +1.5%
to +2.0%); but `sys` halves, 1.67 and 1.55 against 0.82 and 0.74, so **`user` plus `sys` is about
1% lower under s4** (41.06 and 40.89 against 40.65 and 40.55), and `real` too (41.66 and 41.12
against 41.27 and 40.78). The writer writes half the bytes. A report of the cost should carry both
columns.

## 2. What s4 changes for an author, run

**The forms, with `heroes-w2` as the compiler, each its own harness process**:
`check` 699 and 0, `fixes` 999 and 0, `unsupported` 329 and 0, `full` 39 and 0, `lldb` 14 and 0,
`lines` 532 and 0 (the patched `suite_lines`; 2 cases skipped by name, `sys/prctl.h`). The
clang-message readers' cases sit mostly in `unsupported`: by `#~` marks, 56 `ffi_parameter_type`,
34 `ffi_writable_parameter`, 18 `ffi_constant_type`, 4 `field_lend_written`, 2
`ffi_callback_type`, and `check` holds 3 and 2 more (`grep -rlE '#~ .*\b<code>\b' tests/golden`).

**The readers that read a column**: `emit/ffi_narrowed.hero` and `emit/ffi_mutable.hero`, both
on the **probe line** (`emit/probe_reply.argument_columns`, recomputed from
`extern_probe.probe_line`). s4 cannot join a probe line: `extern_probes` writes each probe after
`writer.at_file` at the extern's own line, and the line before it is a `#pragma` (not plain, never
joined) or another extern's probe (another line). Read in the patch, and held by `unsupported`
above. No other module of `selfhost/emit/` or `selfhost/cli/` compares a clang column
(`grep -rn -E '\.col\b|col:|column'`, read file by file).

**The readers that read clang's excerpt**: R3's `met` (`cli/own_names.hero`), `declared_after`,
`builtin_name` and `runtime_name` find the name with `runtime_name.caret_name`, the identifier
under clang's caret in the line clang quoted. **Apple clang 21 prints no excerpt for a line longer
than 4,096 bytes**, measured: a C file whose one line is 4,000 or 4,096 bytes gives 4 lines of
stderr, 4,097 or 4,200 gives 2 (`clang -fsyntax-only`, the excerpt and caret gone). So a reader
of the excerpt is blind on such a line, and the program falls to `internal error` at exit 2.

- **That cliff is already in s0.** The prologue, every local's and temporary's declaration, is one
  physical line under s0 as well (`h_cliargv_parse`: 6,093 bytes under s0, 6,185 under s4, the
  signature joined in front). Reproduced with a header `#define t3 0` (R3's own golden's shape) and
  a `main` of n chained bindings: **both compilers tell `ffi_header_refused` at exit 1 up to
  n = 88 and print `internal error: compiling the generated C failed` at exit 2 from n = 89**
  (bisected, `r3/`). s4 moves the cliff by the signature's length, which this shape could not
  separate. **This is a defect of the trunk, `blocking` by its class (an exit 2 where the author
  can be told), not s4's**, and it is the coordinator's to file; its repair is to stop resting a
  blame rule on clang's excerpt, which panel 214's R1 already prefers (*decided by a clang run and
  by a name's form, never by clang's words*).
- **Where s4 would be the cause**: a first error in a body line that s4 joins past 4,096 bytes and
  s0 kept short. The guard (`runtime/heroes_guard_close.h`) gives every runtime name back after the
  groups' headers, so a group's macro meets only the emitter's own `t<N>`, `f_<field>` and `h_`
  names, whose first use is at file scope or in the prologue: tried with `#define
  hero_print_end() (1 +)` and a 60-term `print`, whose s4 line is 4,749 bytes, the macro never
  fired and both compilers read `ffi_missing_link` at exit 1. *Not found, and searched for only in
  that shape.* Code lines past 4,096 bytes, static tables left out: **50 under s0, 57 under s4**
  (`awk`, the patched tree's emission by each writer).

**What the author reads on the exit 2 path** (`ie2/`: a group's header `#define bb1 0`, a label
of the emitter's no reader names): s0 prints `main.hero:6:4: error: expected ';'` over the excerpt
`6 | bb1:`; s4 prints `main.hero:6:83` over `6 | t2 = h0_a; t3 = INT64_C(0); t4 = t2 > t3; if
(t4) goto bb2; else goto bb3; bb1: return; bb2:`. **The line is the same; the column, a C column
under both and never a Heroes one, moves; the excerpt is the whole Heroes line's C.** In the
compiler's own C, code lines over 500 bytes are 2,540 under s0 and **10,330** under s4, over 1,000
bytes 1,115 and 2,069 (`awk` on `seed/heroes.c` and s4). So a defect report quoting this message
grows, and past 4,096 bytes it loses the excerpt entirely. The seat's turning condition (*a clang
message an author is shown whose excerpt or column the joined line makes wrong or unreadable*):
**not wrong, measured; longer, and past 4,096 bytes absent**, on a path that is itself the
compiler's fault. I do not hold it as turning the seat.

**A sanitizer report** (`asan/`: a header's `static inline` function reading past its own
`malloc`, called from a Heroes function, `heroes run --sanitize`): exit 134 under both, frames
`oops m.h:2`, `h_main_twice main.hero:5`, `h_main_main main.hero:10`, **no column printed, the
`.hero` frames identical**; the generated wrapper's own line differs, `main.c:153` against
`main.c:125`, true in each file. A UBSan report on a Heroes line: *unrun* (the language aborts on
its own overflow first, and I built no other).

**A crash's stack trace**: the seat's `lldb` table (lines equal, columns C columns that differ)
and the `lldb` form's 14 and 0 stand; I did not re-run the table.

**Coverage**: `heroes` has no coverage flag (its help names none). By hand, `clang
-fprofile-instr-generate -fcoverage-mapping` on each compiler's emission of a 12-line program,
`llvm-cov export`: **`llvm-cov` ignores `#line` under both shapes** and reports the C file, never
the `.hero`; s4 coarsens C-line coverage (159 lines, 55.3%, against 87 lines, 28.7%), and
**regions 119 and 40, functions 17 and 3, branches 28 and 6 are identical** (`cov/`).

**Profilers**: a sampling profiler attributes by the line table, which is identical by (file,
line) at every address (above). One that shows columns would show C columns that differ, as
`lldb` does: *unrun*, no such profiler was run.

## 3. Whether *nothing reaches 10 MB* holds for shapes nobody built

`parts.py` over s4 (statements, not physical lines, so a joined line is read as s1's are):

| part | bytes | share |
|---|---|---|
| the copies themselves, `t<N> = h<N>_x;` and `h<N>_x = t<N>;` | 2,083,360 + 1,222,028 | 12.9% |
| the inline bounds checks' conditional, before the read | 931,832 | 3.6% |
| `#line` directives left | 899,158 | 3.5% |
| leading whitespace | 616,464 | 2.4% |
| `HERO_STR_STATIC` lines, the compiler's own strings | 546,704 | 2.1% |
| **functions no line names but their own head and prototype** | **1,065,721 (954 of 9,332)** | **4.2%** |
| **identifier text**: runtime `hero_`/`Hero`/`HERO_` 17.9%, `h_<module>_<name>` 13.6%, `t<N>` 12.6%, `h<N>_<name>` 11.3%, `bb<N>` 2.4%, other 12.0% | 17,879,044 | **69.7%** |

**A floor, arithmetic on those measurements and so an inference**: s4 less every copy, every
bounds conditional, every directive, all indentation and every dead function is 18,831,319 bytes;
the build-only root keeps 80.5% of s4 by the seat's numbers (20,631,868 of 25,638,593), which
gives **about 15.2 MB**. Counting a dead function's copies twice only lowers the floor. So no
combination of copy propagation (which keeps the use and renames it, so it saves less than the
copy's bytes), an out-of-line bounds check, a root, dead-function removal and no `#line` reaches
10 MB. **What could is identifier text**, which the seat's `content.py` normalises away and so
never counted: a shorter mangling, which moves every C name a stopped frame or an ASan frame shows
(`h_main_twice`), for every program, or a second mode for the seed alone, which panel 016's
objection reaches. *Unrun*; named here so that the sitting knows where the mass is.

**A compressed seed in the tree** (gzip or xz with a decompress step): **refused by CLAUDE.md
§ 10**, since the step is a script or a second binary on the one path that must need only clang,
and by `seed/README.md` § Why raw C (panel 085's R1, *a `.gz` would need a decompressor on the one
path that must not need one*). And measured worse on the history it is meant to save, by the
seat's method over the same three seeds (`hist/`): s0 raw 14,914 KiB, `xz -9` 11,826, `gzip -9`
19,267; s4 raw 10,235 KiB, `xz -9` 7,976, a first copy of 2,817,588 bytes then about 2,612 KiB a
regeneration against raw s4's 2,440, since git cannot delta a compressed stream. Raw s4 is ahead
after about 16 regenerations (arithmetic). Under 10 MB in the tree is reached that way only, and
§ 10 closes it.

**The 10 MB target is a question rather than a premise**: the milestone was opened for history,
and history is regenerations times bytes per regeneration. s4 makes the second 60% of s0's
(the seat's § 7); part 2's rhythm moves the first by a larger factor (62 regenerations in the two
weeks to 2026-10-14, by the milestone page, against at least 8 a month below). A target in pack
bytes per month would judge what the milestone measured; one in tree bytes judges what a profile
can move.

## 4. The rhythm: how often the runtime's interface moved

`git log -G'define HERO_RUNTIME_ABI' -- runtime/heroes_runtime.h` (**`-S`, as the brief named
it, finds 1 commit**, the introduction: it counts the string's occurrences and a value change
keeps the count). The value went 1 to 30 in 30 commits, one step each:

| month | moves | commits touching the header at all |
|---|---|---|
| 2026-08 (from 08-04) | 18 | 31 |
| 2026-09 | 8 | 16 |
| 2026-10 (to 10-17) | 4, the last 10-09 | 9 |

**8 moves in the 30 days since 2026-09-17**, 4 in October. The seed carries
`_Static_assert(HERO_RUNTIME_ABI == 30, ...)` at its line 63, so clang refuses an older seed at its
first lines against a moved header, as measurement 045 found for 2,279 commits. **So a seed built
against the live `runtime/` and refreshed *only when red* is refreshed at every move: about 8 a
month at September's pace**, OCaml's routine rather than Zig's rare event, plus every refresh a
new form needs. That still divides the milestone's 62 in two weeks (about 133 a month) by about
16; it is not Zig's 9 to 25 a year nor OCaml's 15, the historian's counts.

**The route that decouples them, the historian's Zig precedent, priced for Heroes**: a frozen
copy of the runtime beside the seed, as `stage1/zig.h` sits beside `zig1.wasm`. The runtime clang
reads is 33 files, 687,335 bytes, `xz -9` 166,652 (`find runtime -type f`, `clang -M`). The
bootstrap becomes three stages: clang on the seed with the frozen runtime; that compiler builds
`selfhost/main.hero` against the frozen runtime (its C speaks the frozen ABI), by
`HEROES_RUNTIME=<frozen>`, an existing input; that compiler, now the tree's logic, builds the
compiler against the live runtime. A refresh is then owed when a new form enters `selfhost/`, never
when the runtime moves. Costs: one more stage of about 70 s on every bootstrap and CI leg, a
second copy of the runtime that is history and not truth (a frozen record, as the seed is), and
the one-line bootstrap naming the frozen directory. *Unrun as a build*; the sitting should weigh it
against 8 refreshes a month.

## 5. Routes nobody listed

1. **The frozen runtime beside the seed**, § 4.
2. **Emitting only what `main` reaches**, for every program: 954 functions, 1,065,721 bytes of s4
   (4.2%), are named on no line but their own head and prototype, functions only a test or
   nothing calls (`h_clilex_has_text`, `h_clicheck_has_text`, `h_cliargv_flag_list` among them).
   `heroes test` would still emit them. Small, and every program's, so no profile; *unrun as a
   build*.
3. **Identifier text**, § 3: the only mass large enough for 10 MB.
4. **A repair before any profile**: the excerpt cliff of § 2 is s0's as much as s4's, so whichever
   shape lands, R3's readers want a reading that does not rest on clang's excerpt.

## 6. Conditions I would add to adopting s4

- The trunk's defect of § 2 filed before s4 lands (the coordinator's to file; reproducer
  `r3/`, `#define t3 0` and 89 chained bindings, exit 2 on `cbf724983`'s own compiler), since s4
  lengthens every prologue line by its signature and moves that cliff toward smaller functions.
- The cost reported in both columns, `user` and `user` plus `sys` (§ 1).
- The 442-byte difference between the seat's emission and the diff it kept explained, by emitting
  the committed patch once more at landing (the lane will do it anyway at the fixpoint).
- design.md §3.1 and `.claude/rules/generated-c.md` say what an author sees change: the C column
  and excerpt of clang's own messages, and C-line coverage by hand.

## 7. What would turn me

A reader of clang's words that s0 serves and s4 does not, found in a shape other than the two I
tried; or a profiler or debugger an author uses that reads columns as Heroes columns.

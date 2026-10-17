# Panel 220, the compiler-engineer's report

Written 2026-10-17 from 11:54 (`date` at the session's first command printed `11:54:33`). The tree
judged is the lane's working tree, `.claude/worktrees/lane-land215/`, committed head `bc978b09d`,
C2 uncommitted; read only, its gate running. Its working files were copied at 11:55:22 into
`<scratchpad>/220-ce/snap/` and every count below is of that copy (`snap.sha`, `shasum -a 256` of
each file). Written as I go: a later command that contradicts an earlier sentence is written
beneath it with its time.

## The unit, first: the brief's `awk` is not `layout`'s `code_lines`, and my 450 was `layout`'s

`tests/harness/suite_layout.hero:966`, `code_lines`, counts every line outside a `test` block whose
trimmed text is not empty: **a comment line counts** (the function's own body; its test at :999
asserts blank lines do not, and nothing says comments do not). The brief's `awk` drops comment
lines too, so its column is narrower than the instrument it names.

My panel 218 table used `layout`'s unit: re-counted now from `cbf724983` (`git show`), the two
neighbours it gave read `emit/value_check.hero` 99 and `emit/constant_accessor.hero` 78 by
`layout`'s rule, and 46 and 37 by the brief's. **So the 450 of my prediction (*its new modules
measure at most 450 code lines by `layout`'s `code_lines`*) is in `layout`'s unit**, and the build
is measured against it in that unit below, with the brief's beside it.

`<scratchpad>/220-ce/count.sh`: `layout` is an `awk` transcription of `code_lines` (a `test "`
line opens a block, a non-empty line at column 0 that is not `#` closes it, every other non-blank
line outside counts); `brief` is the brief's `awk` verbatim.

## What C2 costs, in both units (`count.sh` over the snapshot; baselines by `git show` at `bc978b09d`)

`git diff --stat 760a9035b bc978b09d -- selfhost/` names `emit/padding.hero` and
`emit/padding_tables.hero` alone (defect 2119), so the four modified modules read the same at
both commits and every line they gained since is C2's.

| module | `layout` (comments counted) | brief's `awk` | of which C2's |
|---|---|---|---|
| `cli/clang_tree.hero` (new) | 217 | 170 | all |
| `cli/value_init.hero` (new) | 281 | 216 | all |
| `cli/value_text.hero` (new) | 144 | 122 | all |
| `cli/value_c2.hero` (new) | 163 | 131 | all |
| `cli/value_told.hero` | 267, from 155 | 219, from 124 | +112 / +95 |
| `emit/value_probe.hero` | 215, from 192 | 136, from 119 | +23 / +17 |
| `cli/value_run.hero` (new) + `cli/value_ask.hero` | 232 + 117, from 289 | 177 + 78, from 217 | +60 / +38 |
| `cli/assemble.hero` | 278, from 277 | 179, from 178 | +1 / +1 |
| **C2 in all** | | | **1,001 / 790** |

`value_told.hero` reads 219 by the brief's own `awk` where the brief says 214: the file was written
at 11:49:42 (`ls -T`), after the coordinator's count. **Against my prediction's unit, C2 is 1,001
lines where I said at most 450: 2.2 times, not the brief's 1.75.**

## Where each line lands, by panel 218's conditions (`items.awk`, per top-level item, both units)

- **The reader, `clang_tree.hero`, 217 / 170**: condition 3 (one dump per unit, `id`s matched within
  it) and the critic's stub rule. My panel 218 estimate for it was 150 to 220; it is inside that.
  Not paid twice: the tree's one general JSON reader, `mutate/json_read.hero` (278 / 231), is
  recursive with `DEPTH` 64, and a clang dump nests 2N + 9 levels for an enumerator of N `|` terms
  (measured here: 8 terms 25, 16 terms 41, 20 terms 49, 32 terms 73; `depth.py` over
  `clang -fsyntax-only -Xclang -ast-dump=json` of `chain<N>.c`); SDL3's whole unit reads 46. So
  reusing it would refuse a header holding a flag mask of about 28 terms; iterative is
  robustness, not duplication.
- **The walk, `value_init.hero`, 281 / 216**: condition 1 (references followed: `questions`,
  `asked`, `reached`, `value_of`, `initialiser`, `constant_object`, about 113 / 100), the predecessor
  rule (`predecessor`, 12 / 11), records leaf by leaf (`leaves`, 21 / 18), the leaf's and the
  splice's type (`arithmetic` and its table `ARITHMETIC`, 40 / 37), the splice (`spliced`,
  `references`, 29 / 25), the guard's probe text (`function_of`, 10 / 6), test fixtures counted as
  code (`TEXT`, `DUMP`, 8 / 4).
- **The pre-filter, `value_text.reaches` and its helpers, 103 / 93**: condition 3's *none for an
  accessor whose expanded value holds no identifier and no brace list*, my own condition. It finds
  the accessor's value in the preprocessed text by byte search and bracket depth, then lexes it:
  string and character literals, the pp-number rule, identifiers, and a table of fifteen C keywords
  (`KEYWORDS`, 18 lines) my condition did not ask for (my panel 218 classifier counted a cast and
  `__builtin_huge_valf()` as identifiers).
- **The header's line, `value_text.presumed`, 30 / 27**, and **the telling, `value_told.hero`, +112 /
  +95**: condition 5. `presumed`'s `marker` is a second reader of clang's line markers:
  `cli/bound_said.hero` exports `marker`, `marker_row` and `marker_file`, and `value_told.hero`
  already uses that module.
- **The stage, `value_c2.hero`, 163 / 131**: the key, the canary (condition 4, `CANARY` and
  `lines_of`'s checks), the dump, the `-O0` probe (condition 2).
- **The guard and the shared run, `value_probe.hero` +23 / +17, `value_run.hero` +60 / +38 net**:
  the critic's value-agreement guard (`disagree`) and condition 6. Not paid twice: panel 216's
  compile, link, bounded run and canary are moved into `value_run.hero` and parametrised by
  `level` and `total`, and C2 calls them; the dispatcher is generalised, not copied.

**A deviation from panel 218's R1, in the direction of fewer lines**: R1 says *under a `-O0` build
it rides R1's own probe, under `-O2` a second*. `value_c2.questioned` always compiles, links and
runs a probe of its own at `-O0`, whatever the build's level. That saves the merging of two
probes' index spaces and costs one more compile, link and run per cold unit the pre-filter
passes; I read it as a fair trade and name it only so the record says the code and the
resolution differ.

## Built: my own copy, and the compiler's own cold build (this Mac, 12:05 to 12:15)

Worktree `.claude/worktrees/220-compiler-engineer`, detached at `bc978b09d`, the snapshot copied in,
`build/` removed. Load `9.48 7.33 5.21` (another sitting's seats were running), so no time below is
a number to keep, only the ratio's warning.

- `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`: `real 11.71 user 11.12`.
- `./heroes build selfhost/main.hero -o heroes-c2`: wrote it, `real 105.79 user 79.63 sys 18.70`.
- `rm -rf build; ./heroes-c2 build selfhost/main.hero -o heroes-c2b`: `real 128.34 user 90.72`;
  `build/value-*` 4, `build/init-*` 4, `build/init-*/base.i` **0**; each `init-` verdict 31 bytes,
  `od -c`: `kept`'s seal over an empty text; `cmp heroes-c2 heroes-c2b`: identical. **My prediction
  of 0 dumps on the compiler's own cold build holds.**

## FOUND: the pre-filter, the one part of C2 that reads C text, misses in the unsafe direction

`value_text.value_text` reads the value to the first `;` at bracket depth 0 and skips no literal;
`reaches` then lexes the cut text. A `;` inside a string or character literal at depth 0 cuts the
value before the name it reads, and C2 never asks. `<scratchpad>/220-ce/hole/hole.h`, one program
per constant (`extern "hole.h"`, `constant K: i32`, `print(K)`), built with `heroes-c2`:

```
K_QCH     ';' + HS_EA            exit 0, prints -2147483589, 0 dumps   (HS_EA = 1 << 31)
K_STR     ";"[0] + HS_EA         exit 0, prints -2147483589, 0 dumps
K_PAREN   (";"[0] + HS_EA)       exit 1, ffi_constant_type naming HS_EA, 1 dump
K_PLAIN   (HS_EA)                exit 1, the same, 1 dump
K_CHAR    0 + 1 + (int)sizeof(char) * 0 + 59 + HS_EA   exit 1, the same, 1 dump
K_QCH_OK  ';' + HS_OK            exit 0, prints 1073741883, 0 dumps    (HS_OK = 1 << 30, defined)
```

Defect 2109's own symptom, a stable value C leaves undefined at exit 0, comes back through the
pre-filter: panel 216's probe cannot see it (`59 + INT_MIN` does not overflow at run time) and C2
skipped the unit. **This is the condition I set at panel 218 for a veto, *the reader grown into
parsing C text rather than slicing clang's offsets*, met in the part my own condition 3 asked
for**: the reader proper slices offsets and is sound on every shape here; the pre-filter lexes C
and is not.

## Reshape A, built: C2 reads no C text (my worktree, `heroes-r`)

The cut: `value_text.reaches`, `value_text`, `keyword`, `KEYWORDS`, `name_byte`, `exponent`,
`quoted_end` and their test removed; `value_c2.asked` asks every asked accessor; `questioned` dumps
the unit, walks it, and runs the `-O0` probe only where an asked accessor (the canaries aside)
reaches a question; otherwise the canary is read from the walk (`canary_read`: the undefined
canary's initialiser reached through `hero_c2_ub` and sliced as `1 << 31`, or `broken`). And
`value_text.presumed` reads its markers through `bound_said.marker`, `marker_row` and
`marker_file`. Files: `<scratchpad>/220-ce/reshape/value_c2.hero`, `value_text.hero`, and their
`.diff` against the snapshot.

| | `layout` | brief |
|---|---|---|
| `cli/value_text.hero` | 144 to 27 | 122 to 19 |
| `cli/value_c2.hero` | 163 to 177 | 131 to 139 |
| **C2 in all** | **1,001 to 898** | **790 to 695** |

What it keeps and what it changes, each run with `heroes-r` (built by `heroes-c2`, `real 42.73`):

- the six `hole/` programs: `K_QCH` and `K_STR` now **refused** (`ffi_constant_type` naming `HS_EA`
  and its line), `K_PAREN`, `K_PLAIN`, `K_CHAR` refused as before, `K_QCH_OK` builds and prints
  `1073741883`; one dump each;
- `heroes-r run tests/harness/main.hero -- ./heroes-r run fixedbugs-2109`: `1 passed, 0 failed`;
- the `unsupported` case built directly by `heroes-c2` and by `heroes-r`: their outputs identical
  past line 1 (`diff` of the rest printed nothing), line 1 being clang's `In file included from
  build/tu-<hash>/...` (below, *found beside*);
- `heroes-r test selfhost/main.hero`: `1705 tests, all passed`, exit 0;
- the compiler's own cold build, `rm -rf build; heroes-r build selfhost/main.hero -o heroes-r2`:
  **4 dumps** (`build/init-*/base.i` of 101,243 to 192,287 bytes), each verdict empty, no probe run,
  `cmp heroes-r heroes-r2` identical;
- **what the 4 dumps cost**, timed alone: `clang -x cpp-output -std=gnu11 -fsigned-char
  -fsyntax-only -w -Xclang -ast-dump=json` of each `base.i` wrote 7,721,597 to 13,137,557 bytes in
  `user` 0.03 to 0.05 s; `clang_tree.read` of all four (a measurement root built by `heroes-r`, as the
  compiler is built: 15,452, 14,695, 14,413 and 9,181 nodes) `user` 1.75, 1.80, 1.77 s on three runs,
  60 MB resident, at load 10 to 17 (`real` about 1.15 times `user`). **About 2 s of `user` on a cold
  build of the compiler, against about 85 s of `user` that build measured; nothing warm.** The whole
  build's own times (`real 120.19 user 84.86` against the lane's `real 128.34 user 90.72`) were
  taken at load 8 to 9.6 and are discarded as a comparison.

**My panel 218 prediction of 0 dumps falls under this cut, and I withdraw it**: it priced the
pre-filter's time and not its soundness. Condition 4 changes with it, as the seat that set it:
where no probe runs nothing can be refused, so the canary's run-time half has nothing to guard,
and its static half, the walk reaching `hero_c2_ua` through `hero_c2_ub`, is what is read.

## Reshape B, built: the conservative one, the scan kept and a literal sent to the dump (`heroes-b`)

`reaches` returns `true` at a `"` or a `'` instead of stepping over the literal, `quoted_end` gone,
its test given `';' + EA` (`<scratchpad>/220-ce/conservative/value_text.hero`). `value_text.hero` 144
to 136 and 122 to 113: **C2 993 / 781**. `K_QCH` and `K_STR` refused, the controls as before; the
compiler's cold self-build **0 dumps**, `cmp` identical (load 32, so no time kept). Its completeness
is an argument, not a run: a cut at a `;` before the value's end needs a literal before the cut (a
`;` outside literals at bracket depth 0 ends the statement; a bracket inside a literal that moves
the depth is itself a literal), and any literal now sends the value to the dump. That argument is
a premise about C's lexical grammar, `.claude/rules/module-shape.md` § A narrowing asks the value,
never the world, which is the reason I recommend A and record B for the author to choose.

## FOUND: the walk's own type list skips a leaf, in the unsafe direction (both compilers)

`value_init.arithmetic` returns `""` for any type spelling holding `(`, so a brace-list leaf
converted to an anonymous enum's type (`enum (unnamed enum at ./anon2.h:3:13)`) is never asked.
`<scratchpad>/220-ce/anon/anon2.h`, `record P tag ae` with `k: u32`, `y: i32`:

```
R_AE2   { (unsigned)(INT_MIN / -1), 2 }  into struct ae { enum { AE0, AE1 } k; int y; }
        heroes-c2 exit 0, prints 2147483648, 1 dump;  heroes-r the same
R_NE2   the same list into struct ne { unsigned k; int y; }
        heroes-c2 exit 1, ffi_constant_type;  heroes-r the same
```

The first spelling I tried, `{ 1 << 31, 2 }`, is refused by the sign-change check before C2
(`implicit conversion changes ...`), so the hole needs a cast that keeps the sign. Not a cut: a
defect of the walk's narrowing, by `.claude/rules/verification.md` § Bounded discovery `blocking`
(a wrong program accepted), the lane's before it lands. The direction its repair owes, by
`.claude/rules/module-shape.md`: a leaf whose type the list does not name is asked, never skipped.
Unbuilt here.

## Found beside the sitting: the `unsupported` golden pins a hash that moves with the checkout

`tests/golden/unsupported/fixedbugs-2109-a-value-clang-folds-in-an-initialiser-is-refused.expected`
line 1 is clang's `In file included from build/tu-38b0a31177f9651b/...c:22:` (the header's
`-Winteger-overflow` on `TB`). The same sources print `tu-5b3a418a48b0eab5` in my worktree with
`heroes-c2` and with `heroes-r`, and `tu-3df5dd120b4aabc1` from a copy of the case, the runtime and
`heroes-r` under `<scratchpad>/220-ce/relo/`; `heroes-r run tests/harness/main.hero -- ./heroes-r
unsupported fixedbugs-2109` read `0 passed, 1 failed` on that line alone. `grep -rl build/tu-
tests/golden`: no other `unsupported` case pins one. So the case can pass only in the checkout that
blessed it; whether it passes in the lane's own tree is unrun (read only). The trunk and the CI's
legs are other checkouts.

## Paid twice, and the smaller cuts (unbuilt, estimated by reading)

- the line-marker reader: `value_text.marker` beside `bound_said.marker*`, cut in A (measured: on
  `# 7 "C:\\h\\f.h" 1` the copy returned `C:\\h\\f.h`, `bound_said.marker_file` `C:\h\f.h`; a
  measurement root built by `heroes-c2`; clang writing the doubled backslash on Windows is
  `bound_said.hero`'s own comment, unrun here);
- `value_told.initialiser` and `value_told.repeats` each parse one C2 line (`tabbed`, the space, the
  number): one reader of it, about 8 lines;
- `value_init.reached` is `references` pushing ids: about 4 lines;
- `value_told.message_of` is `value_ask.failure_of` on another type: about 4 lines.

About 16 lines in all, an inference from reading. **Not paid twice**: the run machinery (moved and
parametrised, not copied), the dispatcher (generalised), the canaries (panel 216's ask whether the
checks fire, C2's whether the dump was read), the reader (`json_read`'s guard, above).

## Where the 450 went (scoring my own prediction)

My panel 218 prediction, *its new modules measure at most 450 code lines by `layout`'s
`code_lines`*, **failed**: 1,001 as built, 898 with reshape A. The estimate was written before the
synthesis and priced the reader (150 to 220; built 217), a worklist and its runs (80 to 120), the
appended text (30 to 40), the message (30 to 50) and a canary (15 to 25). Built against it, by
`items.awk`: the reader inside its range; the walk 281 against about 110 to 160, the difference
being the synthesis's conditions my estimate did not hold (the predecessor rule, records leaf by
leaf at their type, the splice at the dump's type, the guard: about 135 lines with
`value_probe.hero`'s 23); the telling 112 against 30 to 50 (a verdict kept as text must be written
and read back, and one constant told once across two probes); the pre-filter, which I asked for and
did not price, 103; the factored run, 60. The lesson I take is the shape of the miss: a seat's
estimate made before a synthesis adds conditions is an estimate of a different design.

## The ceiling

design.md §1.1: *Implementation simplicity is a constraint, not a goal. It sets the ceiling. If we
exceed it the compiler never gets finished*. §1.7 and Part 5: what costs most is what the checker,
the lowering and the backend must all handle. C2 adds no core construct: nothing in the lexer,
parser, checker, lowering, IR or runtime; four modules of `cli/` and one of `emit/` that writes
probe text. The compiler is finished and compiles itself; 898 lines are about 0.8% of the 116,707
my panel 218 report counted, and every C2 module stays under `layout`'s 300 (`value_init.hero`
281, `value_told.hero` 267 the nearest). §1.12 outranks the triangle's lower two, not its ceiling,
so I do not argue robustness against §1.1; I say the ceiling, in §1.1's own terms, is not
breached. **No veto on the ceiling.** The veto I set at panel 218 for *the reader grown into
parsing C text* is met by the pre-filter, and reshape A answers it by removing that text reading
whole, so I ask for the reshape rather than refusing the design.

## The seat's answer

- **verdict**: **reshape.** Cut the text pre-filter whole (reshape A, built): C2 reads no C text,
  every asked unit is dumped cold, the `-O0` probe runs only where an asked accessor reaches a
  question, the canary read from the walk otherwise, `presumed` on `bound_said`'s marker reader.
  Record reshape B (the scan kept, a literal sent to the dump) as the conservative choice. Repair
  `R_AE2` before landing. No veto: no core construct, §1.1's ceiling not breached in its own terms.
- **section**: design.md §1.1, §1.7, Part 5 for the cost; §1.12 for the two holes;
  `.claude/rules/module-shape.md` § A narrowing asks the value, never the world, for choosing A over
  B. design.md does not speak to a probe's pre-filter (searched: `pre-filter`, `prefilter` in
  `docs/design.md`, no line).
- **implementation_cost**: as built **1,001 `layout` lines / 790 by the brief's `awk`**, all in
  `selfhost/cli/` (`clang_tree` 217, `value_init` 281, `value_text` 144, `value_c2` 163, `value_told`
  +112, `value_run` with `value_ask` +60, `assemble` +1) and `selfhost/emit/value_probe.hero` +23;
  no lexer, parser, checker, lowering, IR or runtime line. **Reshape A: 898 / 695** (measured,
  −103 / −95); reshape B 993 / 781. About 16 more by the smaller cuts, unbuilt. Time under A: 4
  dumps on the compiler's own cold build, about 2 s `user` (clang 0.03 to 0.05 s each, the reader
  1.75 to 1.80 s for the four), measured at load 10 to 17; warm nothing.
- **needed_for_self_hosting**: **no**; the compiler's own 22 header constants are literal macros.
  It closes a robustness class (CLAUDE.md § Precedence, rank 3).
- **argument**: C2's reader slices clang's offsets and is sound on every shape I ran; its
  pre-filter lexes C and is not. A `;` inside a literal cuts its scan, and `';' + HS_EA` builds at
  exit 0 printing `-2147483589`, defect 2109's own symptom, through the part my panel 218 condition
  asked for. Cutting that part removes 103 lines and the hole together, and costs about 2 s of a
  cold self-build. The remaining 898 lines pay for conditions the synthesis added, each once;
  nothing reaches the core. A second hole, `R_AE2`, sits in the walk's type list and is the lane's
  to repair.
- **prediction**: at R1 and C2's landing commit, with reshape A, the C2 lines by `layout`'s
  `code_lines` read **880 to 920** (898 on my build, plus the `R_AE2` repair); a cold `heroes build
  selfhost/main.hero` writes **exactly 4** `build/init-*/base.i` and runs no C2 probe; the 4 dumps
  and their read cost **at most 3 s `user`** on a still machine; and `K_QCH`, `K_STR` and `R_AE2`
  are refused on all four CI legs. Checked at that landing's gate; any one failing falsifies it.
- **condition**: I turn to **stand on B** if the dumps' cost, timed on a still machine, exceeds 5% of
  the compiler's cold self-build `user` time, or if a census over the corpus's cold builds shows the
  added dumps costing a program more than its own compile. I turn to **veto** if the repair of
  `R_AE2`, or of any later hole, grows the walk into reading C's type or expression syntax beyond
  clang's own spelling of a type, or if C2 passes 1,000 `layout` lines after reshape A without a
  sitting.

## Housekeeping

- Worktree `.claude/worktrees/220-compiler-engineer` at `bc978b09d` with the lane's snapshot copied
  in; removed at the end with `git worktree remove --force`. Every measurement stays in
  `<scratchpad>/220-ce/` (`snap/`, `snap.sha`, `count.sh`, `items.awk`, `depth/`, `hole/`, `anon/`,
  `relo/`, `dumps/`, `mk/`, `reshape/`, `conservative/`).
- The lane's tree was read and never written, built or run. No paid run, no credential printed, no
  container. Nothing written in the trunk but this report.

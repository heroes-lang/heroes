# Panel 214, the completeness critic, pass 1: the briefs

Written 2026-10-14 from 00:47 in the critic's copy `.claude/worktrees/scratch-panel-214/critic`
(detached at `8c8783ac7`, compiler built from the seed with `clang -I runtime seed/heroes.c
runtime/runtime.c -o heroes`). Every finding names the command that settled it and what it printed.
Written as I go; the ordering by consequence is done at the end (§ Ordered).

## Working notes (in the order found)

### N1. The defect table: hashes right, dates and coverage not

`git merge-base --is-ancestor <h> 8c8783ac7` for all eight hashes: every one prints `ancestor-yes`.
Each hash carries the defect number the table gives it (`git log --oneline 5d3b96606 -n 60 | grep -E
"Defect (2035|2036|2040|2044|2045|2049|2051|2052|2053)"`).

What does not hold:

- **"The night of 2026-10-12 to 13"**: `git log -1 --format="%h author %ad commit %cd"` per hash.
  2035 02:15, 2040 02:49, 2044 04:19, 2045 05:07 on 10-13 are the night; **2049 is 10-13 12:06,
  2052 is 10-13 21:12, 2051 21:18, 2053 22:12**. Three of the eight rows landed the next evening.
  Harmless to the ruling, but a sentence about the pace ("eight times in one night") is wrong.
- **The table omits three commits of the same kind it includes.** `git log --since='2026-10-12
  12:00' 8c8783ac7 -- <the twelve readers, whose.hero, order_trial.hero>` lists eleven commits, not
  eight: also `2190ff8a3` (Defect 2034, 00:58, `-Wformat` kept an error in a header's region),
  `4dcc1e197` (Defect 2036, 01:30, the parts of `-Wformat` C defines passed over in its region,
  `emit/header_region.hero` +62/-2) and `5a92815d3` (03:15, a `-Wformat` cost "by the author's
  ruling", `header_region.hero` +5). Rows 2052 and 2053 are exactly this kind (a warning flag kept an
  error in a header's region), so the table counts two region-flag repairs and leaves out three.
  The case-count line even names *2036 nine*, a defect the table does not list. A seat counting
  "shapes repaired one at a time" from the table undercounts; the region's flag list is a second
  axis of the same problem that no route A to G addresses (see N-routes).
- **"Module it added or changed" understates rows 2052, 2040 and 2035.** `git show --numstat
  --format= <h> | awk '$3 ~ /^selfhost\//'`:
  - 2052 (`602071cc4`): `clang_first.hero` +48/-5, `runtime_name.hero` +48/-9,
    `header_region.hero` +16/-1, `package_words.hero`, `order_trial.hero`. The table says only
    `emit/header_region.hero` and "(kept `-Wimplicit-function-declaration` an error)": it also added
    a reader (the "did you mean 'hero_str_len'" note of sample 4 is 2052's work). A seat pricing
    route B or E from the table thinks 2052 was a flag and nothing more.
  - 2035 (`afd34b4c2`): `header_refused.hero` +60/-21, `clang_first.hero` +18/-9, `order_trial`
    +18, `whose.hero`, `macro_twice.hero`, `one_file.hero`; `runtime_key.hero` +14/-0 was
    **changed, not added** (`git show --diff-filter=A --name-only` lists no selfhost file).
  - 2040 (`1e888df16`): also `group_file.hero` +14/-3 and `header_refused.hero` +31.
  - Every row from 2044 to 2051 also changed `header_refused.hero` (+68, +89, +72, +14), and
    2044, 2045 and 2049 `clang_first.hero` (+28, +26, +66; 2051 did not); the table names only
    the new module. The added modules are right:
    `standard_name` (2044), `runtime_name` (2045), `declared_after` (2049), `builtin_name` (2051),
    each listed by `git show --diff-filter=A`.
- **The command printed under the table is elided** (`grep -E "Defect (2035|...|2053)"`), so it does
  not run as written. Minor; give the full alternation.

### N2. "125 cases marked `#~ ffi_header_refused`" is a count of FILES: the cases are 55

`grep -rl '#~ ffi_header_refused' tests/golden | wc -l` does print **125**, but `... | sed -E
's/.*\.(hero\.frozen|[a-z]+)$/\1/' | sort | uniq -c` prints **55 `hero`, 52 `expected`, 18
`frozen`**. The 52 `.expected` files match because their excerpt reprints the source line with its
annotation; the 18 `.hero.frozen` files are copies frozen into `tests/golden/recovery/corpus/` (the
mutation corpus, read by `selfhost/cli/mutate_recovery.hero`, not by a golden form). The cases are
`grep -rl '#~ ffi_header_refused' tests/golden --include='*.hero'`: **55, 54 under `unsupported/`
and 1 under `fixedbugs/`**; `grep -rl 'error\[ffi_header_refused\]' tests/golden
--include='*.expected' | wc -l` prints 54, which agrees. By defect (same grep, `grep -oE
'fixedbugs-[0-9]+' | uniq -c`): 360 6, 538 2, 550 2, 557 1, 563 6, 569 3, 592 3, 2034 2, 2036 5,
2044 5, 2045 5, 2049 8, 2051 2, 2052 3, 2053 2.

Consequence: the compiler-engineer's brief (item 3) asks a prototype to run "the 125 cases"; a seat
that reports "125 of 125" has counted files, and one that finds 55 will think 70 are missing. And
the other direction of the witness is not in the count at all: five `run` cases name
`ffi_header_refused` in prose without the annotation (`grep -rl 'ffi_header_refused' tests/golden
--include='*.hero' | xargs grep -L '#~ ffi_header_refused'`: 569's sign conversion and four of
2036's), programs that must BUILD; a rule that moves blame can turn them red too, and no brief names
them.

The per-defect counts of 00-shared.md (`ls tests/golden/unsupported tests/golden/run | grep -c
"fixedbugs-<n>-.*\.hero$"`) all reproduce exactly: 360 6, 2036 9, 2044 6, 2045 5, 2049 8, 2051 2,
2052 3, 2053 2, 2035 0, 2040 0 (`find tests/golden -name "fixedbugs-<n>-*.hero"` agrees). The
witnesses of 2035 and 2040 are where the brief says: `grep -n '2035\|2040'
selfhost/cli/header_refused.hero` prints the tests at lines 401 and 456, and `order_trial.hero:169`
holds 2035's premise test.

### N3. The line counts reproduce

`wc -l` over the eleven `cli/` modules and `emit/header_region.hero` prints 332, 650, 181, 418, 292,
213, 133, 334, 61, 125, 197, 211 (total 3,147), exactly the brief's. `clang_first.hero`'s module
doc reads as quoted (`head -8`), with one clause the brief drops: it also says the module was *split
out of `cli/header_refused.hero` on 2026-10-10, past §11's 300 lines once a header refused in a unit
was also asked whether it compiles alone*.

### N4. Route E's line is wrong about `cli/header_alone.hero`: its "alone" holds the runtime

00-shared.md, route E: *compile the header alone (with the C library) and with the runtime ...
(`cli/header_alone.hero` already compiles a header alone for defect 557's shape)*. Read
(`cat -n selfhost/cli/header_alone.hero`): `compiles` (line 46) writes a unit of
`emit_externs.SEEDS` then the headers; `grep -n SEEDS -A2 selfhost/emit/externs.hero` prints
`["heroes_unit.h", "hero_os.h"]`, and `grep -n '#include' runtime/heroes_unit.h runtime/hero_os.h`
prints `#include "heroes_runtime.h"` in both. So header_alone's "alone" means **without the other
groups' headers, WITH the runtime's declarations**: the question 557 asked (header against header),
not the one E asks (header against runtime). It also compiles under `compiling.probe_words`, which
is `without_diagnostics(unit_words(c))` (`selfhost/cli/compiling.hero:100,147`: every `-W` word
dropped), and never with the region's pragmas, so it does not see the flags rows 2034, 2036, 2052
and 2053 made errors in a header's region.

Run, in the critic's copy, `clang -std=c11 -fsyntax-only -I runtime -I build/rep` on three units
per reproducer: the header alone, the header followed by the C headers a unit reads after it
(`<math.h> <stdlib.h> <string.h> <stdio.h>`), and header_alone's own unit (SEEDS then header):

| header | bare | then the C library | header_alone's unit |
|---|---|---|---|
| `hs.h` | 0 | 0 | **1** |
| `i64.h` | 0 | **1** | 1 |
| `e_fabs.h` | 1 | 1 | 1 |
| `imp.h` | 1 | 1 | 1 |
| `e_std.h` | 0 | **1** | **0** |
| `j_typo.h` | 1 | 1 | 1 |

So: (a) on `hs.h`, the collision E exists to name, header_alone answers *does not compile alone*,
the opposite of E's verdict; reusing it as E's probe would tell the collision as the header's alone
and never name the runtime's side. (b) E's "alone (with the C library)" is undefined: `i64.h` and
`e_std.h` compile bare and are refused only once a C header is read (`<stdint.h>`, `<math.h>`), so
whether they read as "refused alone" or "a collision" depends on which C headers the probe reads,
and in which order (the unit reads `<math.h>` AFTER the header, `heroes_standard.h`). (c) Under
header_alone's words, `e_std.h` compiles (0) where the unit refuses it. A seat told the probe
"already exists" prices E low and inherits these three. Route E needs a probe that does not exist
yet, and its "alone" has to be defined (which C headers, which order, which `-W` words and pragmas).

### N5. The samples: faithful, but they compare A against nothing and have no negative control

**Faithful**, run in the critic's copy:
- the "new" side: `./heroes build build/rep/<n>.hero` for the six (sources copied from
  `.claude/worktrees/scratch-typed-inspection/rep/`, `rep/after2049/`, `rep/2045-shapes/`; each
  identical to its sample's header and program, `cat`): all six exit 1, and each output, with
  `build/rep/` stripped, is **byte-identical** to the message the key calls new (`diff -q`: sample
  1 Y, 2 X, 3 Y, 4 X, 5 Y, 6 X);
- the "old" side: `git show 08230cbfa:seed/heroes.c` is `cmp`-identical to the sitting's
  `build/old/seed.c`; built with this tree's runtime, which is fair because `git diff --stat
  08230cbfa 8c8783ac7 -- runtime/` prints nothing. All six exit 2, and each output equals the
  message the key calls old (1 X, 2 Y, 3 X, 4 Y, 5 X, 6 Y) once the unit's hash directory, the
  unit's file name, a `./` before the header and the runtime's path (`<toolchain>/runtime/` in the
  sample, relative in my run) are normalised. Nothing else differs. The key is right.
- labels: neither message carries `old`/`new`, a date or a hash (read).

**Not fair as an experiment for THIS sitting**:
- **Both messages of every pair come from route A or from before it.** X and Y are "the raw clang
  dump at exit 2" against "today's shape-by-shape message at exit 1". No message any of routes B to
  G would print is in the folder, so the seat can at most confirm that a crafted message beats an
  internal error, which nobody disputes, and cannot discriminate among the routes the sitting must
  rule on. What would: for each sample, the message B, C (or F) and E would print (for `hs.h`,
  E's *compiles alone; refused only after the runtime's `hero_str_len`, heroes_runtime.h:278*; C's
  *declares `hero_str_len`, a name reserved to the compiler, § 13*), written by the coordinator or
  a seat and labelled as neutrally.
- **No negative control.** All six are cases whose ruling is "the header's". There is no pair where
  the truth is a compiler bug whose clang error lands at, or whose note names, the program's header
  (the shape 00-shared.md asks the compiler-engineer to find, and that 2049's emitter-name row
  shows exists). A reader answering "my header's" every time scores perfectly, so the experiment
  cannot measure the false blame in the direction 00-shared.md weighs most.
- **Two file names give the answer away**: `j_typo.h` says the mistake is a typo before either
  message is read, and `imp.h` hints at an implicit declaration. Rename them neutrally (`s6.h`).
- **The "old" side is not "before the night"** (00-shared.md § The evidence): `08230cbfa` is
  10-13 03:32 (`git log -1 --format=%ad`), after 2034, 2035, 2036, 2040 and `5a92815d3` had
  landed. "Before defects 2044 to 2053" is exact; "before the night" is not.
- **Sample 4's old message is a LINK failure** (`internal error: linking failed`, undefined
  `_hero_str_le`), not a clang refusal of the unit: the old compiler compiled `imp.h` (the
  implicit-declaration error was not yet kept an error in the region) and the linker refused. It
  belongs to the sitting's question only through 2052's flag; the seat should be told what kind of
  failure it is comparing, or it will read "the compiler said linking failed" as a blame verdict.

### N6. The 1,576: reproduced, at defect 2052's base, not at this tree

00-shared.md gives no command. Reproduced: `git grep -l -E '^[[:space:]]*extern ' 801e51069 --
'*.hero' | wc -l` prints **1576** at `801e51069` (defect 2049, the parent of 2052's `602071cc4`).
The same command at `8c8783ac7` prints **1583**; the seven more are the cases 2051 (2), 2052 (3)
and 2053 (2) added (`comm -13` of the two lists). Other plausible commands give other numbers at
this tree: `grep -l 'extern "'` 1602, `grep -lw extern` 1798, the tracked `.hero` files 3655; and
`git grep -l 'extern "' 08230cbfa` happens to print 1576 too, so the number alone does not identify
its command. **What the population is**, by first directory at `8c8783ac7`: `docs` 705 (sittings'
programs under `docs/panel/`, 16 sittings), `tests/golden/run` 313, `unsupported` 281, `check` 142,
`surface-fixtures` 49, `fixedbugs` 40, `examples` 21, `selfhost` 12, others 15. 45% of "the
tracked programs" are records under `docs/panel/`, many planted mistakes; a census over them is a
witness of messages, not of programs that must build. The compiler-engineer's brief should carry
the command and the 1,583, and say whether `docs/panel/` belongs in the census.

### N7. The include order: holds in every emission that has a group

`grep -n "#include" tests/emission/examples-sqlite-main.c`: lines 2 to 5 `heroes_unit.h` (twice,
`"..."` and `<...>`), `hero_os.h`, `heroes_runtime.h`; 22 to 24 guard open, `<sqlite3.h>`, guard
close; 48 `heroes_standard.h`, as written. Over the tree, for every `tests/emission/*.c`, the line
of `<heroes_runtime.h>` < guard open and the last guard close < `heroes_standard.h`: **605
emissions, 308 with a group, 308 in that order, 0 not; 297 have no group** (a loop of `grep -n`).
`heroes_standard.h` includes `<stdint.h>` and `<math.h>` (lines 59, 60) and nothing else, and
`heroes_runtime.h` includes `<stdint.h>` only when `HERO_STANDARD_AFTER_GROUPS` is undefined
(lines 72 to 74), otherwise declaring the integer types from clang's macros, which is why sample 2's
note lands in the runtime at line 78. Two things the sentence leaves out:
- **the program's unit is not the only unit that holds a group's header.** `grep -rn
  'include_lines(' selfhost` lists `cli/two_ways_ask.hero:65`, `cli/label_dump.hero:47`,
  `cli/defined_twice.hero:196`, `cli/compiling.hero:167` (`reads`) and `cli/header_alone.hero:47`:
  probe units that open with SEEDS (the runtime) and the headers, under `probe_words` (no `-W`),
  with no region pragmas and no `heroes_standard.h` after. A rule stated on the program unit's
  order (B's "the first error's location") must hold in each of them, or say they never report.
  Unrun: which unit reports first for each of the six.
- **a group may name the compiler's own header.** `git grep -h -E '^[[:space:]]*extern' --
  'examples/*.hero' | sort | uniq -c` prints `10 extern "hero_os.h"`; `emit/externs.hero:93`
  `is_own` treats `heroes_runtime.h` and the SEEDS as the compiler's. Route B's "located in the
  compiler's own files" and route D's "units that never hold the runtime's names" both meet a group
  whose header IS the runtime's; no brief mentions it.

### N8. The census the brief cites cannot see this question; the one it means compares too little

00-shared.md: *a census of that population is the witness a rule change owes,
`.claude/rules/verification.md` § the census*. That file's census (§ Bounded discovery, step 3) is
*the census of `check` over the tracked `.hero` files*. Run: `./heroes check build/rep/<n>.hero`
on the six reproducers: **all six exit 0**. `check` never compiles a header, so the census the rule
names is blind to every shape of this sitting; defect 2052's agent built the programs, which is a
different instrument. And the compiler-engineer's item 4 asks for **exit codes compared**: a route
that keeps exit 1 and moves the blame inside it (names the runtime where today names C's library,
tells the wrong group, drops the collision's other side) moves no exit code. A witness that *nothing
told today changes for the worse* has to compare the messages (code, group, the party named), not
the exit codes alone.

### N9. Route C's (and F's) name list is not the set of names a unit declares before the header

Route C names *the runtime's `hero_*`, `Hero*`, `HERO_*`, the emitter's `h_*`*. Run: a unit
`#define HERO_STANDARD_AFTER_GROUPS 1` + `heroes_unit.h` + `hero_os.h` + `heroes_runtime.h`
through `clang -std=c11 -fsyntax-only -Xclang -ast-dump=json` and `clang -E -dD`: **197 named
declarations** come from `runtime/` (140 from `heroes_runtime.h`, 57 from `hero_os.h`, each node
attributed by its spelling file) **and 40 macros**; outside those prefixes stand **ten typedefs**
(`int8_t` to `uint64_t`, `intptr_t`, `uintptr_t`: defect 2044's shape) and the guard macro
`HEROES_RUNTIME_H` (`HEROES_`, not `HERO_`). The emitter's non-`h_` name is `main`
(`grep` of top-level functions in `tests/emission/examples-sqlite-main.c`): 2049's main shape. And
the C library's and clang's builtins (2049's `<math.h>`, 2051's `fabs`) are no prefix at all. So C,
or F, decides 2045, 2049's typo and emitter names and 2052's suggestion; it leaves 2044, 2051,
2049's `<math.h>` and `main` shapes to another rule. A seat pricing C or F as "general" should be
handed the residue; the route lines read as if the four prefixes were the whole set.

### N10. Route G is in no seat's brief, and its premise is weaker than its line says

`grep -n -o -E '(route|Route) [A-G]...'` over the five seat briefs: the compiler-engineer prices
"B, C, E, F or one you find", the ffi-pragmatist D and F, the spec-warden B, C, E, F; **no brief
names D to the compiler-engineer or G to anyone** (`grep -l -i 'sarif\|serialize'` matches only
`00-shared.md`). G is the route closest to replacing `clang_first.hero`'s text reading, the
compiler-engineer's ground. Measured on this Mac (`clang --version`: Apple clang 21.0.0), a unit of
SEEDS + `hs.h` with `-fdiagnostics-format=sarif`: clang warns *diagnostic formatting in SARIF mode
is currently unstable [-Wsarif-format-unstable]*, appends `2 errors generated.` after the JSON on
the same stream, writes each **note as a separate result with `relatedLocations` empty** (not tied
to its error), and carries **no include stack**: the very facts `clang_first.hero` reads (the
includes above the error, the notes under it). `--serialize-diagnostics` writes a 920-byte LLVM
bitstream file (exit 1), which no module of the compiler reads. The Windows box runs clang 23.1.1
(`ssh win 'clang --version'`: `Target: x86_64-pc-windows-msvc`); unrun there.

### N11. The rule file already states a general narrowing, the opposite of route B; no brief says so

`.claude/rules/c-boundary.md:133`: ***The narrowing is `declaration()`, not whose text it is.**
Every class recovers a name and asks whether this program declared it `extern`, so a symbol nobody
declared stays exit 2 and the compiler's fault.* Route B ("the first error's own location decides")
is "whose text it is". A sitting choosing B overturns a stated principle of the exit-1 class, and
one choosing C/E/F is closer to it; the briefs quote the same file's line 49 and twelve members
(both verified: `sed -n 45,140p`, twelve bullets) and leave this paragraph out. Also on B: the
generated C carries `#line` directives, so clang locates an error in the emitter's code **in the
program's own `.hero` file**. Run with the old compiler on
`tests/golden/unsupported/fixedbugs-2049-a-header-that-declares-a-function-s-c-name-is-refused`:
`...c:95:9: error: conflicting types`, then the same error at `...refused.hero:14:9`, both with
the note at the header's line 3. B's "neither the compiler's nor the C library's" must say that a
`.hero` location is the compiler's. Route B's own parenthetical (the error in the generated C, its
note in the header) is confirmed by that run.

### N12. The ergonomist: the folder is clean, the brief drops the non-blind statement

`.claude/worktrees/scratch-panel-214/llm-ergonomist/` holds `brief.md` (`cmp`-identical to
`214-briefs/llm-ergonomist.md`), `heroes-spec.md` (`cmp`-identical to `8c8783ac7`'s spec) and
the six samples (`cmp`-identical); `grep -i '214|panel|old|new|08230|8c878'` over the samples
finds nothing but the messages' own "before"/"after". No key leaks. But:
- the agent definition (`.claude/agents/llm-ergonomist.md`) asks for *a copy of the specification
  at a path OUTSIDE the repository that your brief names* and *the proposal, stated as a spec diff*,
  and says a `context: yes` **voids the reading**. The brief names no path, gives no spec diff (not
  even route C's sentence), and, unlike panel 213's (`docs/panel/213-briefs/llm-ergonomist.md:3`:
  *You are run as a subagent ... so you are NOT blind ... the author chose ... non-blind*), says
  nothing of the run being non-blind. Run as a subagent it will see `CLAUDE.md` (it reached this
  critic), answer `context: yes`, and its reading is void by its own definition unless the brief
  records whose choice the non-blind run is for THIS sitting.
- the brief's first sentence, *a C header written by a program's author*, plus questions phrased
  *your header's mistake*, hand the seat the premise that every sample is the author's; with no
  negative control (N5) the "whose mistake" answer is given, not measured.

### N13. The convening sentence is not the record's, and "six" is not "eight"

00-shared.md quotes, in italics, *repair the two last shapes, then tag, and convene a sitting on the
general rule, since six repairs went one shape at a time*. `gh issue view 2055 --json body` (read
only): item 4 reads *the two boundary defects found last, 2051 and 2052, repaired before the tag
rather than an exception to zero blocking, and a sitting convened afterwards on the rule ..., since
six repairs went one shape at a time*, under *What was answered, through the question widget and in
conversation, each with the coordinator's recommendation where one was put*. The italic sentence is
not in the issue, and the issue does not say whether the sitting was the author's idea or the
coordinator's recommendation accepted (`.claude/rules/records.md` § whose idea it was). The record
says **six repairs**; the table counts **eight** and the brief says "carved eight times"; with 2034,
2036 and `5a92815d3` (N1) it is eleven commits. Say which six the author's answer meant.
Also: 00-shared.md cites *CLAUDE.md § 7* for *a clang failure is normally exit 2*; § 7 is the
generated-C rules' pointer and says nothing of exit codes (read); the sentence is
`.claude/rules/c-boundary.md:49` alone.

### N14. Verified as written (each by the command named)

- Defect 360's line: `gh issue view 1687 --json body` (read only), its repair paragraph: *a stack
  or note reaching the runtime's or the build's files stays the compiler's*; title and state
  `[CLOSED] Defect 360: a bound header clang refuses on its own is told internal error at exit 2`.
- Issues 2046 (`[OPEN]`, `class:improvement`, `area:spec`, *Nothing tells an author which C names
  a header ... may not declare*), 2047 (`[OPEN]`, `class:adjacent`, absolute paths), 2054
  (`[OPEN]`, `class:adjacent`, the `-Werror` note's list): `gh issue view <n> --json
  number,title,state,labels`.
- The spec: `grep -n -i "internal error\|blame" spec/heroes-spec.md` prints nothing, nor do
  `hero_`, `reserved` or `does not compile`; § 13 is at line 348. Its budget: `./heroes measure
  spec/heroes-spec.md` (offline, `ANTHROPIC_API_KEY` unset): `cl100k_base 7655`, `real 10048`
  (pinned, claude-opus-5, 2026-10-13), *Headroom: 192 against the 10240 ceiling ... the FFI floor
  mortgages 60*. So a sentence has **132 real tokens** before a removal is owed, and its real cost
  is unmeasurable without a paid `--refresh`: the spec-warden can give the vendored count only, and
  the sitting's "at what cost in tokens" stays a lower bound until the author pays a refresh.
- `c-boundary.md:49-50`: *normally exit 2 ... twelve members*; twelve bullets counted.
- Windows: `ssh win 'clang --version'` prints `clang version 23.1.1`, `x86_64-pc-windows-msvc`;
  `grep -n 2053 tests/harness/platform_skips.hero` names both 2053 cases (lines 63, 64) under
  `STATIC_ELSEWHERE`. Docker: `docker images` lists `heroes-linux-arm64:latest`.
- `design.md:686`: *Every user name becomes `h_<module>_<name>[_<typehash>]`... runtime names are
  `hero_*`; `extern` FFI names pass through unmangled*; `selfhost/emit/mangle.hero:84-105`.
- `HERO_RUNTIME_ABI` exists (`_Static_assert(HERO_RUNTIME_ABI == 30, ...)` in every emission).

### N15. Routes missing from A to G (two run, three named)

**H, read the runtime after the groups' headers.** The tree already did this for the C library:
`runtime/heroes_runtime.h:60-71` explains why a unit reads `<stdint.h>`/`<math.h>` after its groups
(`HERO_STANDARD_AFTER_GROUPS`, `_GNU_SOURCE`). Moving `heroes_runtime.h` there too is not in the
list. Run: units `#define HERO_STANDARD_AFTER_GROUPS 1`, the header, then `heroes_unit.h`,
`hero_os.h`, `heroes_standard.h`, `clang -std=c11 -fsyntax-only`:

| header | first error | first note |
|---|---|---|
| `hs.h` | `heroes_runtime.h:278` conflicting types for `hero_str_len` | `hs.h:2` previous definition |
| `i64.h` | `heroes_runtime.h:78` typedef redefinition | `i64.h:1` |
| `e_std.h` | `math.h:467` conflicting types for `sqrt` | `e_std.h:1` |
| `e_fabs.h` | `e_fabs.h:1` redefinition of `fabs` | (include site) |
| `imp.h` | `imp.h:1` call to undeclared `hero_str_le` | **none** (no runtime suggestion) |
| `j_typo.h` | `j_typo.h:1` undeclared `HeroSt` | **none** (no runtime suggestion) |

Every collision, runtime's, C library's and (already today) the emitter's, takes ONE shape: an
error in a file read after the header with its note at the header's line; and the header's own
mistakes stop reaching runtime names at all (2049's typo and 2052's suggestion shapes vanish).
Costs unrun: what in a header region needs the runtime first (the ten examples with an `extern "hero_os.h"`
group, `git grep -l` over `examples/*.hero`, N7; the guard files), and the emission and seed churn. It combines with B, inverted (a
compiler file after the header that errs with a note at the header is a collision), or with E.

**I, ablation: the program's unit without the group's header.** E compiles the header without the
program; nothing in the list compiles the program without the header, which is the one probe that
answers the direction 00-shared.md weighs most (an emitter defect whose error lands at, or whose
note names, the header). Run: the unit this tree wrote for `hs` (`build/tu-6714f30660d0f6ad/hs.c`),
`e_std` and 2049's emitter-name case, the `#include <header>` line replaced by the bound
function's prototype (`int eight(void);`, written by hand here), `clang -std=c11 -fsyntax-only`:
**exit 0 for all three**, against exit 1 as written. Refused with the header and clean without it,
the refusal involves the header; refused without it too, it is the compiler's whatever clang's
location says. Cost unrun: the emitter has to write stand-ins for what the unit takes from the
header (records by value, constants, macros used as functions), from the group's own declarations.

**J, a scan of the header's declared names before any unit.** `clang -Xclang -ast-dump=json` and
`-E -dD` enumerate a unit's declarations by file (N9: 197 declarations and 40 macros from `runtime/`); the header's own,
dumped alone, intersected with the runtime's, the emitter's mangled names and `heroes_standard.h`'s,
names a collision with both sides **before** clang's prose is read, the same on three platforms
(2053's words differ by platform; a name set does not). It is C's or F's enforcement without a
spec sentence, or E's verdict without reading a refusal.

**K, a message that states what was established and not whose it is.** The brief frames a binary
(exit 1 the header's, exit 2 the compiler's). A third outcome is unlisted: the refused line, both
sides of a collision, whether the header compiles alone, and the address to report to if it does,
so neither party is falsely blamed. CLAUDE.md § 8 / design.md §4.17 may refuse it; the sitting
should say so rather than not see it.

**Precedents the historian's list leaves out** (names to check, not claims): toolchains that emit C
and include the program's own C headers beside it, V (`#include` with generated `main__`-style
names), Vala (VAPI bindings to C headers), GHC's `foreign import capi` (stubs compiled with the
user's header), Chicken and Gambit Scheme, Haxe/hxcpp, Crystal's `lib`. And clang's own reserved
space: `-Wreserved-identifier`, which the ffi-pragmatist is asked to check, is half of F's cost.

### N16. Premises handed to seats that steer them

1. **Ergonomist**: the samples compare raw clang against route A, with no B to G message and no case
   whose truth is the compiler's (N5); the brief states every header is *written by a program's
   author* (N12). The experiment can only approve A's messages over an internal error.
2. **Compiler-engineer**: route E's premise that the probe exists (N4); "the 125 cases" (N2); "the
   1,576" without a command and stale by seven (N6); the routes to price exclude D and G (N10); the
   census compares exit codes only (N8). Each lowers the price of E or of keeping A.
3. **The table** (N1) presents 2052 and 2053 as flag rows with no reader work, which understates
   what any rule must replace (2052 added 48 lines to `clang_first.hero` and 48 to
   `runtime_name.hero`), and omits three flag commits of the same kind, so the "how much of today's
   readers it replaces" question starts from a short list.
4. **Spec-warden**: *no language form enters by these routes* is asserted, then asked (item 2).
   Route C reserves identifiers to the compiler, which may well be a language rule. Asked and
   pre-answered in one brief; let the seat answer it. Its budget question can only be priced on the
   vendored counts (N14).
5. **Historian**: *how do they keep their generated names out of the header's way* presumes they do,
   steering toward C, D and F; ask whether they do, and what they report when they do not.
6. **Every seat**: the brief's frame sentence, *the compiler must say whose mistake it is*, presumes
   the binary that route K (N15) questions.

## Ordered, most consequential first

1. **The ergonomist's experiment cannot rule between the routes** (N5, N12, N16.1). Every pair is
   raw clang against route A's message; no B to G message is in the folder; all six truths are
   "the header's", with no compiler-bug control; the brief says each header is the author's;
   `j_typo.h` names its mistake. And the brief drops panel 213's non-blind statement, so a
   `context: yes` voids the reading by the seat's own definition. The samples themselves are
   faithful: both sides reproduce, the key is right, nothing in them leaks which is which.
2. **Route E's premise is false** (N4). `cli/header_alone.hero` compiles with `heroes_unit.h` and
   `hero_os.h`, both of which include `heroes_runtime.h`, and under no `-W` word: on `hs.h` it says
   *does not compile alone*, the opposite of E's verdict. E's "alone (with the C library)" is
   undefined: `i64.h` and `e_std.h` compile bare and are refused only once `<stdint.h>` or
   `<math.h>` is read.
3. **"125 cases" counts files; the cases are 55** (54 `unsupported`, 1 `fixedbugs`; N2). Five
   `run` cases that must keep building are named by nobody.
4. **The option set lacks two routes that were run here** (N15). H, the runtime read after the
   groups' headers, turns every collision into one shape and makes two of the night's shapes
   disappear. I, the unit compiled without the group's header, is the only listed or unlisted probe
   that separates an emitter defect from a header's mistake. J (a name scan before the unit) and K
   (a message that does not assign blame) are named but unrun.
5. **The witness is mis-specified** (N6, N8). The rule file's census is of `check`, which exits 0
   on all six. The 1,576 is `git grep -l -E '^[[:space:]]*extern ' 801e51069 -- '*.hero'` and
   reads 1,583 at this tree, 705 of them under `docs/panel/`. And exit codes alone cannot witness
   *nothing told changes for the worse*.
6. **`c-boundary.md:133` already rules "the narrowing is `declaration()`, not whose text it is"**,
   which is the opposite of route B, and no brief cites it. Through `#line`, clang also places an
   emitter error in the program's own `.hero` file (N11).
7. **Route G is in no seat's brief, and D is not in the compiler-engineer's** (N10). On this Mac,
   SARIF is flagged unstable, leaves every note unlinked from its error, and carries no include
   stack.
8. **The name lists of C and F miss** the ten integer typedefs, `HEROES_RUNTIME_H`, `main` and the
   C library's and builtin names (N9), so neither is general by itself.
9. **The record and the table** (N1, N13). Three of the eight rows landed on the evening of 10-13,
   not "the night". Three commits of 2052's and 2053's kind are omitted (2034, 2036, `5a92815d3`).
   2052's reader work is not shown. Issue 2055 says *six* repairs and does not contain the brief's
   italic sentence. CLAUDE.md § 7 is cited for a rule that lives in `c-boundary.md:49`.
10. **Other units hold a group's header** (`two_ways_ask`, `label_dump`, `defined_twice`,
    `reads`, `header_alone`), with another order and no `-W` words. Ten examples name the runtime's
    own `hero_os.h` as a group's header (N7). A rule stated on the program's unit has to hold in
    those too.
11. **The spec's real cost is unmeasurable without a paid refresh** (N14): 132 real tokens of
    headroom once the FFI floor's 60 is held back. Any sentence's binding count stays a question.

Verified as written: the eight hashes and their defects, the twelve line counts, the per-defect case
counts, the include order (308 of 308 emissions with a group), the six reproducers' exits (2 before,
1 after), defect 360's line, issues 2046, 2047 and 2054, the spec's silence and budget, the twelve
members, Windows' clang 23.1.1, the 2053 skips and the Docker image (N14).

The critic's copy `.claude/worktrees/scratch-panel-214/critic` was removed at the end of pass 1. Its
`build/` paths cited above (`build/rep/`, `build/alone/`, `build/tu-*`) are reproduced by the
commands given, from that worktree's recipe.

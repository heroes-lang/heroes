# Panel 234, the completeness critic's first pass: the briefs

Written as it goes, 2026-11-05. Every command was run in this session, at
f0c4c99d2, in `.claude/worktrees/234-critic/` (greps and line citations) or
from the trunk's root with the critic's compiler (the probes), which says
`heroes 0.6.0` / `sources 1a1e340778db789a`, built with
`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` in the worktree.
The briefs read are the uncommitted files of `docs/panel/234-briefs/` in the
trunk's working tree as they stood when this pass started (mtimes 20:50 to
21:00). No verdict is given.

## The findings in order of consequence

Numbered as written (F1 to F37); this is the order the critic would repair
them in, most consequential first.

1. F1, experiment Q measures text parsing as much as thickness.
2. F15, P3 fails for a package below the root; items B and F are coupled.
3. F2, panel 028 R3 refused B4's shape; B5 (packages compiled in) unlisted.
4. F26, no seat holds the ergonomist's locality veto for B4, H and J.
5. F11, item H counts the wrong list; a keyword as a field has no route.
6. F18, the reader test's second turn is fed `check`, which passes FFI errors.
7. F7, panel 114 R3 (R2's instruments, a veto on two-arm shapes) omitted.
8. F9, item I's witness carries two reasons; the seed reason runs.
9. F16, `timespec_get` is no monotonic clock on either machine.
10. F3, B4 handed without panel 056 and the input-class rule.
11. F20 and F37, the bare arms: labels decide turn one; the sentence is true
    of G only.
12. F21, task statements leave readings open; F23, the rule's `(T, error)`
    boundary; F22, S built by the coordinator.
13. F19, "cell" defined two ways; F17, 567 headers, not seven.
14. F31, the root driver as drafted is refused; F27, two ratified rulings;
    F14, issues 1067 and 2081 unnamed; F8, `compile` vetoed or deferred.
15. The rest: F10, F4, F36, F25, F12, F5, F13, F32, F33, F28, F29, F24,
    F30, F34, F6, F35.

## The probes, re-run

Every command of `probe/README.md` was re-run from the trunk's root with the
critic's compiler (`bash -c` loop, `H=.claude/worktrees/234-critic/heroes`).
**All seventeen reproduce**: the same diagnostics, codes, positions, notes,
fixes and exit codes as the README prints (it leaves out the excerpt bars, as
it says). What the probes do not cover is below.

## Findings

### F1. Experiment Q's tasks need two things the language does not have and Q's arms do not supply: splitting text into lines, and turning text into an integer (blind/README.md:64-73)

Q1 reads *one integer per line*, Q2 *lines `name,age`* and the integer
`args()[1]`, Q3 *one name per line*. Spec § 11's built-ins
(`spec/heroes-spec.md:320-326`) hold no `split`, no line splitter and no
conversion from `str`, and Q's arms carry only `core/database/sql` (the README
gives each answer only "its arm's `core/` tree"). Run:

```
$ cat a.hero      # n = "42".to_i64()
$ heroes check a.hero
error[bad_operand]: `to_i64` takes an integer or a float, found `str`
$ cat b.hero      # n = to_i64("42")
error[bad_operand]: `to_i64` takes an integer or a float, found `str`
$ cat c.hero      # xs = "a,b".split(",")
error[unknown_function]: no function named `split` — `x.split(…)` means `split(x, …)`, so `split` has to be a top-level function or a built-in
```

(scratch `.claude/worktrees/234-critic-scratch/parse/`). The corpus shows the
cost: of the hits of `grep -rn -E '^function (number_of|int_of|digits_of|digits_in)\('
examples`, six parse a `str` into a number (`gallery/03-fallible.hero:14`,
`rpn/main.hero:52`, `todo/main.hero:94`, `assembler/program.hero:190`,
`query/data/table.hero:81`, `markdown/blocks.hero:109`; the others count
digits or take a `Value` or an `i64`), and
the milestone page itself says *`to_i64` does not take a `str`, so six corpus
programs build numbers digit by digit* (`M-core-packages.md:23`). So every Q
answer must hand-write a line splitter and, for Q1 and Q2, a digit loop (or
bind text and lean on SQLite's type affinity), in both arms alike. **A Q answer
can be right or wrong for a reason that is not the arm**, and the likeliest
first-turn failures in both arms are in code neither shape touches, which
pushes both arms toward a floor where comparison D reads nothing. The README
says the test "compares two faces of one capability, not two capabilities";
as written it compares two faces plus a text parser the reader writes blind.
Routes the coordinator can weigh: give both Q arms the same small text
package and its card (identical across arms), or shape Q's inputs so no
parsing in Heroes is needed (e.g. hand the integers to SQL as text and let a
`CAST` in the task's own SQL convert), or record per answer whether its
failure lies inside or outside the calls into `sql`.

### F2. Panel 028 R3 refused the shape of route B4, and the shared brief's negative sentence about it rests on two search words (00-shared.md:51-56)

The brief: *"panel 028's text holds neither `search` nor `path`"*
(`grep -n -i -E "search|path" docs/panel/028-the-prelude.md`: no line, exit 1,
re-run). True of the words, not of the ruling. Panel 028's ballot
(`docs/panel/028-the-prelude.md:12-14`) defines **P-source** as *"a
`prelude.hero` beside `runtime/`, discovered and parsed per compilation"*,
and R3 (`:151-190`) refuses it: *"P-source buys no capability while opening
a trust boundary the runtime does not have ... a decoy `prelude.hero` has
**no analog** — clang cannot type-check Heroes source"*, adopting **P-embed**,
the source compiled into the compiler. That is issue 803's *"panel 028 R3
keeping anything from being searched at run time"* (issue 803 body, line 30,
`gh issue view 803`), a fair paraphrase. B4's *"a root the compiler knows for
packages"* is P-source's shape applied to packages, so a seat weighing B4 owes
an answer to 028 R3's trust-boundary argument, and the brief tells it the
opposite (that the citation is empty). Two consequences:

- the sentence *"Whether any sitting refused a module search path is a
  question, not a premise"* should name panel 028 R3 as the nearest ruling,
  quoted, rather than its absence of two words;
- **a route nobody listed: B5, the canonical tree compiled into the compiler**
  (028 R3's P-embed, applied to modules reached by `use core/...` rather than
  to the prelude's scope). No file on disk, no search path, no copy, the
  packages versioned with the compiler and verified by its own net; its costs
  (the compiler's size, a release to change a package, panel 097 condition 5
  read against it, `heroes fetch` with nothing to fetch for `core/`) are for
  the compiler-engineer to price.

### F3. Route B4 is handed without panel 056, its three return conditions, or the input class a search path would arrive through (00-shared.md:141-143)

Panel 056 refused a per-project file and **left module paths explicitly
homeless**: its table row 7, *"`[workspace]`, module paths | beside the file:
no path, no search list, no hierarchy"*, and *"Rows 1 and 7 ... have no home
at all, and both belong to ... `M-package-manager` and where this sitting's
three return conditions now live"*
(`docs/panel/056-what-a-project-file-would-carry.md:322-330`; resolution
`:210-243`; `M-package-manager.md:34-40` carries them). And
`.claude/rules/cli-surface.md:47-56`: *"There is no fourth slot, because there
is no fourth input class ... a `.hero` file, argv, and the machine's
environment"* — so a search path set by a flag or by the environment is
admissible under § 10 (panel 055 already took `CPATH`, `LIBRARY_PATH` and
`PKG_CONFIG_PATH` from the environment for C, panel 056 row 4), while one set
by a project file is refused until the three conditions are met. The brief
frames B4 only against panel 099's premise; the seats need these two rulings
to price B4's variants (flag, environment, compiler-known directory, project
file), which differ in admissibility.

### F4. Item A hands the seats the coordinator's answer to question (i) as a fact (00-shared.md:113-121)

The brief: the M-package-manager sentence *"puts a package in Heroes alone on
the second side"*. The page it cites says the opposite of settled:
*"the sentence just above, verified by clang, or by whoever wrote it, is the
first question that milestone's opening sitting has to answer for a package
written in Heroes alone"* (`docs/roadmap/milestones/M-package-manager.md:31-33`,
read). The leaning is carried from issue 803's own text (*"puts a pure-Heroes
`strings` on the second side"*, `gh issue view 803`), so it is an inherited
leaning, still not a measurement. And the dichotomy omits a third verifier
nobody listed: a package written in Heroes alone is checked by the Heroes
checker itself (every type, every `T?`, every label; the probes show it) and
by its own `test` blocks run in the net, which is neither "clang against the
header" nor "whoever wrote it". Item A's ruling should be asked as the
question the page asks.

### F5. The collision probe shows less than the shared brief says of it (00-shared.md:131-132)

The brief: *"a program's own `strings.hero` beside it is
`module_binding_taken`, its note naming `as`"*. The probe's file
(`probe/core-prefix/collision.hero:1-2`) binds `use core/strings` **and**
`use strings` in **one file**; the refusal is a file binding one name twice,
and its own note says *"the two modules may both exist and both be used"*.
Run in scratch (`.claude/worktrees/234-critic-scratch/twofiles/`): `main.hero`
`use core/strings`, `mine.hero` `use strings` (the program's own
`strings.hero`):

```
$ heroes run main.hero
true
false
(exit 0)
$ heroes build main.hero --emit-c | grep -o -E '[A-Za-z0-9_]*contains[A-Za-z0-9_]*' | sort -u
h_corestrings_contains
h_strings_contains
```

So under B2 a program may keep its own `strings` beside `core/strings`, and
they mangle apart. The shape beside it, a program module `corestrings.hero`
beside `core/strings`, is refused, not silent:
`error[module_names_collide]: modules core/strings and corestrings are one
name to the compiler ... both become h_corestrings_…` (exit 1). Under B1, by
contrast, a program's own `strings.hero` at its root **is** the package's
path, so every package name is taken from every program's root; the brief
does not say so, and it is the measurable difference between B1 and B2.

### F6. The citation for eight refused names covers one (conventions-draft.md:53-55; 00-shared.md:251-253)

conventions-draft §2.3 cites probe 3 for *"`find`, `join`, `repeat`, `sort`,
`map`, `filter`, `keys`, `len` are refused everywhere"*, and item M says *"a
function named `sort` inside a package is refused (probe 3)"*; probe 3's
package (`probe/names/strings.hero:2`) declares `find` alone, and
`probe/names/sort.hero:2` declares `by`. Run for all eight (a module
`pkg.hero` declaring `function <name>(s: str) -> str`, `heroes check
pkg.hero`, scratch `builtin-names/`): **each is refused**
`error[builtin_name_taken]: <name> is a built-in of the language, so the name
is taken everywhere`, exit 1. The facts hold; the citation should be this run.

### F7. Item F's two-arm routes are handed panel 114 R2 without R3, which says R2 "does not land without" three instruments, one of them the ffi seat's veto on every two-arm shape (00-shared.md:64-71, 186-192)

P2 (a platform layer "with the `#ifdef`s inside") and P3 ("a header of
`static inline` functions ... panel 114 R2's shape") are both C with two
arms. Panel 114, ratified in full 2026-09-06
(`docs/panel/114-the-question-was-not-which-platform.md:281`), says at
`:192-212`: *"R3 — Three instruments are owed, and R2 does not land without
them"*: (1) a header the program ships and a header the machine lacks told
apart (both were `ffi_missing_header`, a silent skip); (2) *"The compiler
reports which `extern` declarations it did NOT verify on this machine ... a
wrong return type and a wrong parameter type in the dead arm is clang exit 0
and a running binary ... This is the ffi seat's veto and it binds every
two-arm shape equally"*; (3) a fifth `declaration()` class for a syntax
error in a shipped header. The shared brief cites R1, R2 and R6 and not R3.
Whether R3's instruments stand today is **a question**: the harness now
holds a witness mechanism for `ffi_missing_header`
(`tests/harness/absence.hero:1123-1204`), which may be (1); for (2) and (3)
`grep -rn -i -E "did not verify|not verified on this|unverified on this|dead
arm|ships beside|shipped header|header the program ships" selfhost
tests/harness spec docs/design.md` printed nothing, and `gh issue list
--search` for `"panel 114" instrument`, `dead arm`, `did not verify` and
`fifth declaration` found no issue for them. R2's sentence is also absent
from the spec, as the brief says (its grep exit 1, re-run; a widened
`grep -n -i -E "beside|its own file|#if|ifdef|platform|machine|static
inline|shipped" spec/heroes-spec.md` finds only an unrelated line 289). The
ffi-pragmatist's brief asks the §1.12 question ("which of them can let a wrong
width reach C in silence") but not R3's measured answer, and the
compiler-engineer's brief asks P3's cost in the compiler without R3's three
owed pieces.

### F8. The record disagrees on whether `compile "shim.c"` was vetoed, and the shared brief hands the seats one side (00-shared.md:65-71, 191-192)

The brief: *"`compile "shim.c"` was deferred, not vetoed (R6, `:234-238`)"*.
Panel 036 (`docs/panel/036-the-ffi-ladder.md:39`): the spec-warden's row reads
*"veto P2 and P3"*; `:153` *"P2 and P3 wait — two vetoes and an objection
each"*; `:275` *"not refused on their merits but deferred"*. Issue 927 (open,
the issue P4 cites, `gh issue view 927`): *"a group clause (`extern "gfx.h"
compile "gfx.c"`) was panel 036 P2's own veto, so the package-level shape is
the one that has never been judged"*. And panel 114 R6 itself ends *"That
deferral can be closed"* (`:238`), since a `static inline` header needs no
`compile`. So P4 is two routes with two standings, the group clause (vetoed
in its sitting, resolved as deferred) and a package-level clause (never
judged), and issue 927 adds *"If it does not appear in a milestone about
packages that compose, that is the answer and the deferral closes for good"*.
The seats pricing P4 should be told which shape they price.

### F9. Item I's witness carries two reasons at once, and the brief names one; the seed reason, marked unrun, runs (00-shared.md:219-225)

`grep -n '^extern ' selfhost/cli/process.hero`: **two groups**,
`40:extern "hero_os.h"` and `66:extern "stdlib.h"`. So `:66-69` compiles both
because a unit includes every group of its module (the milestone page's own
reason, `M-core-packages.md:110-113`, *"a translation unit includes every
group's header (`selfhost/emit/unit.hero:32`) and clang sees the union"*) and
because `hero_os.h` is a seed of every unit (the brief's candidate;
`selfhost/emit/externs.hero:86-87`, `["heroes_unit.h", "hero_os.h"]`, and an
emitted unit includes both before a group's header, `--emit-c` lines 2-4
then 22). The witness cannot tell them apart, and the brief does not say
the module has a `hero_os.h` group. Run in scratch (`pergroup/`): a module
whose only group is `extern "stdlib.h"` declaring `function
hero_dir_release()`, with no `hero_os.h` group:

```
$ heroes build main.hero -o prog && ./prog
wrote prog
released            (exit 0)
```

and a name no header declares, added to the same group, is still refused
(`error[ffi_unknown_name]: stdlib.h declares no hero_no_such_function`). So
**the seeds alone explain it** (now run), and item I has three positions, not
two: per group, per module (the union of the module's groups), and per module
**plus the seeds** (every group sees `hero_os.h` and `heroes_unit.h` whatever
it names), which is what the compiler does. A per-group check must also say
whether the seeds count, which is item F's question (a platform header beside
the runtime) arriving here.

### F10. Part 7 item 10's falsifier counts "three CI legs", the CI has four, and the correction beneath it is not handed on (00-shared.md:74-77; ffi-pragmatist.md:76-79)

The falsifier dates from panel 052, 2026-08-14 (`docs/design.md:3634-3644`).
`.github/workflows/ci.yml:118-127` (the `only` choices, `arm` the fourth) and its matrix (`:328`, the `all` entry):
four legs, Linux x86-64, Linux arm64, Darwin arm64, Windows x86-64, *"four
legs since 2026-09-17"* (`:105`). The ffi-pragmatist brief says *"the same on
all three CI legs (Darwin and Linux arm64 run; Windows and Linux x86-64
marked unrun)"*, naming four machines under "three". And beneath the
falsifier, *"Corrected 2026-10-25 ... a narrower one builds, C converting it
exactly (spec § 13): `labs(x: i32)` against `long labs(long)` builds and
prints 5 ... So `i32` is a spelling of a C `long` parameter that builds here,
unrun on Linux and Windows"* (`:3647-3652`). That correction changes item J's
question (a narrower spelling exists for a parameter, losing range; it does
not exist for a struct field, whose layout must match), and neither brief
mentions it.

### F11. Item H counts the wrong list: contextual words block no C name tested, and the foreign-word registry, which does, is not in the count (ffi-pragmatist.md:52-63; 00-shared.md:205-217)

The ffi-pragmatist is asked to count `keyword()`'s 21 and `contextual()`'s 16
(both counts verified: `sed -n 157,200p selfhost/keywords.hero | grep -c`
21; `:136-154` 16 words). Run in scratch (`ctxwords/`):

- `extern "unistd.h"` / `function link(from: cstr lent, to: cstr lent) -> i32`,
  POSIX `link`, a contextual word: builds and runs, prints `-1`, exit 0;
- a record with fields `tag`, `owned`, `when`, `rest`, `zero`, `lent`: runs,
  prints `21`;
- extern and Heroes parameters named `lent`, `owned`, `tag`, `when`: run,
  print `7`.

`selfhost/foreign_words.hero` (`declared_word`, `:116`) holds 17 more words,
`case class const def elif enum fn func import int let null struct switch
try union var`, and they **do** refuse (scratch `foreign/`):

```
record R / class: i64, union: i64, func: i64
error[reserved_word]: `class` is not a word in this language ...   (and `union`, `func`)
extern "stdlib.h" / function abs(int: i32) -> i32
error[reserved_word]: `int` is not a word in this language ...
extern "x.h" / function func(n: i32) -> i32
error[reserved_word]: `func` is a word Heroes refuses, so no `extern` group binds a C function by that name — call it in a header of the program's own that includes "x.h", `static inline int c_func(int x) { return func(x); }` ...
```

Panel 013's own line (`docs/panel/013-function-type-marker.md:179`) names
*"The foreign-word registry"* as what collides, and `func` (29 of issue 1's
counts) is a foreign word, not a keyword. So the count that measures issue 1
is keywords plus foreign words (those not also C keywords: `class def elif fn
func import let null try var`), in field position above all, where a tag
record must spell the C field. Two further facts the seats need: **the
compiler already prescribes route 2** for a keyword (`extern "x.h"` /
`function match(n: i32)`: `error[expected_name]: match is a keyword of Heroes,
so no extern group binds a C function by that name — call it in a header of
the program's own ... static inline int kw_match(int x) ...`), so route 2 is
the status quo's own advice, not a proposal; and **a keyword as a field name
has no route at all** (`record R / match: i64`: `expected_field` followed by
three cascading errors, `expected_expression`, `missing_match_arms`,
`expected_field_name`, and no hint), which is the case the brief asks about
("can a record of a group name it at all"): measured, no.

### F12. Item B's corpus sentence cites the wrong lines and states an inference; nine suites read `examples/`, not one (00-shared.md:133-137)

`tests/harness/suite_corpus.hero:122-130` is `run_all`; `program_directories`
is `:500-507`, and it takes a directory of `examples/` only where
`read_file(dir + "/main.hero")` succeeds. So a package tree `examples/core/`
with no `main.hero` is skipped as `examples/gallery/` is (the count, 55
directories and `gallery/` the one without `main.hero`, verified:
`ls -d examples/*/ | wc -l` 55, 54 with `main.hero`). The brief's *"so a tree
of packages placed in `examples/` is not neutral to it"* is an inference the
code read contradicts for that placement; what is not neutral is a driver
`main.hero` inside the tree (conventions §1.6), which makes it a corpus
program owing a `test` or an `extern` (`:133-138`), and a copy of the tree
inside each program (B3), whose `test` blocks then count toward that program
(`module_texts`, `:522-526`, walks `list_tree`). And the corpus is one of
**nine** suites whose code names `examples` (the verification map's own
instrument, its `code_of` filter, applied to `"examples(/...)?"`):
`canonical compat corpus emission probe records special surface warnings`,
plus `cards.hero`, `gallery.hero` and `platform_skips.hero`. The question
"what a tree under `examples/` does to the net" is the nine's, and the
compiler-engineer's brief asks it of `suite_corpus.hero` alone.

### F13. Item A's demand is four names chosen on 2026-09-03, and the tree holds others as often (00-shared.md:117-120)

Issue 805's command re-run: `split_lines` 9, `is_space` 9, `trimmed` 7,
`index_of` 4, `tests/harness/strings.hero` 15 functions: all verified.
Enumerated from the tree instead (`grep -rhoE '^function [a-z_0-9]+'
examples --include='*.hero' | sort | uniq -c | sort -rn`, then each name's
signatures read): `is_digit(c: u8) -> bool` in 4 files (5 with one
`(b: i64)`), `is_letter(c: u8) -> bool` in 3, and a `str -> i64?` number
parser, `number_of(word|text: str) -> i64?`, in 4
(`examples/gallery/03-fallible.hero:14`, `rpn/main.hero:52`,
`todo/main.hero:94`, `assembler/program.hero:190` with a second parameter),
beside `query/data/table.hero:81` `int_of` and the harness's own
`to_number`. The number parser is issue 1067's `strconv` (F14) and F1's Q
tasks. Goose's test asks the demand of the world; the brief's list is an old
enumeration of one package.

### F14. Two of the milestone's sixteen open issues are named nowhere in the briefs (00-shared.md:267-273)

`gh issue list --milestone M-core-packages --state open`: `1 803 805 893 925
927 970 1067 1069 1072 1073 1103 1154 2081 2082 2209`. The shared brief rules
or names fourteen; **1067** (*"`strconv`, because six programs write the same
digit loop by hand"*, kind feature) and **2081** (*"four questions added to
the opening sitting ... interface cards under a budget, Go's names, thin or
thick decided by a reader test, the effect measured"*, kind task, the issue
whose four questions items C, D, E and N rule) appear in no brief (a grep of
every brief file for both numbers: none). The decision issue `Panel 234:`
would not know to answer 2081, and 1067 bears on F1 and F13 and on whether
`strconv` is item A's second package.

### F15. P3, "a header shipped with the package that needs it ... no compile step", does not work for a package below the program's root without a flag or a placement-bound spelling (00-shared.md:190-191; compiler-engineer.md:64-66; conventions-draft.md:28-33)

Run in scratch (`nestedheader/`): `core/net/net.h` (one `static inline`
function) beside `core/net/net.hero` (`extern "net.h"`, a wrapper, a test),
and `main.hero` at the root `use`-ing `core/net/net`:

```
$ heroes run main.hero                         # group names "net.h"
error[ffi_missing_header]: `net.h` is not on this machine's include path — clang looked and did not find it
  note: ... install the library's development files, or pass `--include <dir>` ...   (exit 1)
$ heroes test core/net/net.hero                # same spelling, the package alone
ok   "answer"   1 test, all passed
$ heroes run main.hero --include core/net      # the root, with a flag
42
# group changed to "core/net/net.h":
$ heroes run main.hero                         # 42, exit 0
$ heroes test core/net/net.hero
error[ffi_missing_header]: `core/net/net.h` is not on this machine's include path
```

So a group's header is looked up from **the compiled file's directory**, not
from the declaring module's: a package shipping its own header can be
reached from a root **or** tested alone with one spelling, never both without
`--include`; and the spelling that works from the root (`core/net/net.h`)
names the package's **place**, so the same package under B1 (`net/net.h`),
B2 (`core/net/net.h`) or a B3 copy at another prefix needs another spelling.
Items B and F are coupled through this, and the briefs treat them apart. The
diagnostic also misdirects (*"install the library's development files"*),
which is panel 114 R3's first instrument still unmet for a nested module
(F7). The evidence the compiler-engineer is told to confirm (F17) is golden
cases whose header sits beside the compiled file, which is exactly the case
that works. A route nobody listed: the compiler searches the declaring
module's directory for its groups' headers first (C's own rule for a quoted
`#include`), a compiler change for the compiler-engineer to price.

### F16. `timespec_get` offers no monotonic clock on either machine run here (ffi-pragmatist.md:23-26; carried from M-core-packages.md:66-68)

The brief asks for *"a monotonic clock in nanoseconds (`clock_gettime` with a
`clockid_t`, against `timespec_get`, ...)"*, and the page's question (v) calls
`timespec_get` *"the portable clock"*. Run (scratch `clock/c.c`, `clang
-std=c11`): on this Mac and in `heroes-linux-arm64:latest`,
`timespec_get(&t, TIME_UTC)` returns 1 and **`TIME_MONOTONIC` is not
defined** (`TIME_MONOTONIC not defined` on both). The SDK's own line,
`$(xcrun --show-sdk-path)/usr/include/_time.h:200`: `#define TIME_UTC 1 /*
time elapsed since epoch */`; glibc's `/usr/include/time.h:64-65` defines
`TIME_UTC` alone. So `timespec_get` is portable and a **wall** clock, which a
duration or a timeout must not use; the monotonic clock has no portable C
spelling on these two machines, and `clock_gettime`'s `clockid_t` question
cannot be dodged by `timespec_get`. The brief should not offer it as the
alternative for the monotonic capability.

### F17. "`find tests/golden -name '*.h'` lists seven under `tests/golden/fixedbugs/`" is false (00-shared.md:69-70; compiler-engineer.md:65)

```
$ find tests/golden -name '*.h' | awk -F/ '{print $3}' | sort | uniq -c | sort -rn
 262 run
 236 unsupported
  33 check
  15 surface-fixtures
  12 fixedbugs
   9 full
$ find tests/golden/fixedbugs -name '*.h' | wc -l
12
```

567 headers in all, 12 under `tests/golden/fixedbugs/` (two of them in a
subfolder, `fixedbugs-563-another-module-s-record-is-read-without-its-module-s-groups/`,
a multi-module case worth item I's and conventions §1.4's attention). The
evidence that a shipped header needs no compiler change is far larger than
seven, and 262 of the cases run; but every case read compiles its header
from the compiled file's own directory, the one placement F15 shows works.

### F18. The reader test's second turn is fed `heroes check`, which passes a wrong C binding that `heroes build` refuses (blind/README.md:97-101, 135-137)

`compile@1` is judged by `heroes build`, and a first turn that does not
build gets one second turn with *"the compiler's diagnostics (`heroes check`,
the full form)"*. Run (scratch `checkvsbuild/`), `extern "stdlib.h"` /
`function atoi(s: i64) -> f64` called from `main`:

```
$ heroes check wrong.hero      # exit 0, nothing printed
$ heroes build wrong.hero -o prog2
error[ffi_return_type]: `atoi` returns `int` in `stdlib.h`, not `f64` — clang read the header
  fix (guess): declare the result `i32`        (exit 1)
```

So a first turn that fails the build on an FFI mistake gets an **empty**
`diagnostics.txt` (issue 974, open, is this gap's own title: *"a bindings
module is invisible to `heroes check`"*). It is not hypothetical here: F1's
Q answers must turn text into integers, and binding `atoi` or `strtol` is a
route a reader will take. The second turn should be fed the diagnostics of
the command that judged the first, `heroes build`.

### F19. A "cell" is defined two ways, and the registered bands break under the README's own sample cut (spec-warden.md:29-36; blind/README.md:95-97, 125-131)

The README: *"18 cells, each run on two models ... five samples each"*, a
cell being an arm on a task across both models (10 answers). The
spec-warden's brief: *"`right@1` out of 10 per cell (a cell is an arm on a
task on one model, five samples, so out of 5 per model and 10 together; say
which you use)"*, which defines the cell as one model's five and then scores
it out of ten. And the README cuts *"every cell ... from five samples to
four, then to three"* if the first sessions project past 25 USD, after the
predictions are registered in counts. Register them as proportions, or fix
the denominator before registering. For the projection itself: panel 232's
80 sessions of the same command and models cost **1.5590 USD** in all
(`docs/panel/232-what-1-0-0-promises-...md:14`), about 0.02 USD each,
against this run's cap of 0.20 a session; the README's caps are an order of
magnitude above the last measured cost, which it could say.

### F20. In the bare arms the first turn turns on parameter labels, the same in G and H, and the warden is asked for ceilings but not floors (blind/README.md:35-40; spec-warden.md:34-36)

A bare reader knows only *"it is Go's `strings` package written in
Heroes"*, and every call with two `str` parameters names each argument (S1
needs `has_prefix` and `cut`/`split_once`, S2 `cut`/`split_once`). Run with
the probe's own package (scratch `labels/`):

```
strings.index_of(s: "abc", substr: "b")   error[wrong_label]: `index_of` has nothing called `substr` at this position — it is `part`
strings.index_of("abc", "b")              error[needs_label]: two of `index_of`'s parameters are `str`, so every one of them is named ...
```

So whether a bare reader can be right at the first turn depends on the S
package's parameter names (Go's `substr`, or the probes' `part`, which
conventions-draft §2.4 leaves open for a function whose contract differs),
a choice not stated in the README and identical across variants. It can put
both bare arms at a **floor**, where `bare-H` against `bare-G` reads nothing
on `right@1`, exactly as a ceiling does; the warden's brief asks only which
cell is a ceiling.

### F21. Several task statements leave readings open that a byte-for-byte grade will decide (blind/README.md:42-52, 64-73, 133-144)

The tasks' `task.md` files and their hidden cases do not exist yet
(`find docs/panel/234-briefs/blind -type f`: `README.md` alone), so this is
read from the README's statements, and each item is a way a program right by
the words could fail a truth made by the reference's reading:

- **S2**: *"skip a blank line"* (a line of spaces or tabs?); *"first character
  that is not a space"* (a tab?); *"the spaces around them removed"* (tabs?
  `trim_space` with Go's contract removes all white space); a line with two
  `=` (split at the first, as `cut` does, or refused?); *"keys: K"* (lines
  printed, or distinct keys?).
- **S3**: no output format is given (S1, S2, Q1, Q2 and Q3 give theirs); and
  *"separated by spaces, tabs and line ends"* differs from Go's `Fields`
  (also `\r`, `\v`, `\f` and Unicode spaces) where a hidden case holds one.
- **Q1**: the empty file must print `sum: 0`, while SQL's `sum()` over no rows
  is NULL; a thin `column_int64` reads NULL as 0 and is right by accident, a
  thick API that surfaces NULL as a failure is not, so the hidden empty case
  measures NULL's surface, a real thickness difference that the README does
  not name as the thing measured. An empty file also makes a hand-written
  line splitter (F1) yield one empty line.
  Run in C against this Mac's SQLite (scratch `sqlnull/n.c`): `SELECT
  count(*), sum(x)` over an empty table prints `count=0 sum_type_is_null=1
  sum_as_int64=0`.
- **Q2**: *"alphabetical order"* (SQL's default collation is bytes,
  upper case first: the same program's `ORDER BY name` over `b`, `B`, `a`
  prints `B a b`); a repeated name over the threshold (once or twice?);
  and *"the age given to the query as a bound parameter"*, which no output
  can check, so a program that formats the age into the SQL text passes.

The hidden cases should exercise none of these, or each `task.md` should
close them.

### F22. The rule the coordinator applied to Q is not applied to S (ffi-pragmatist-sql.md:7-11; blind/README.md:20-22)

The SQL brief: *"You write both shapes, not the coordinator, because the
coordinator's own recommendation was thick, and the instrument that judges a
recommendation is not built by the one who made it."* Experiment S is
*"written once by the coordinator"*, both variants and both cards, and the
coordinator's own `conventions-draft.md` §2.2 adopts H (*"where the contract
differs, the name differs too"*). The variants are mechanical, but the cards'
one-line descriptions, the doc comments, the choice of the four functions and
of the tasks that exercise them are where a leaning enters. The same rule
asks that someone else write or check S's cards, and a check stated with
the materials (for example `diff <(sed -E 's/\b(index|last_index|cut|lines)\b/X/g'
S/core-G/core/strings.hero) <(sed -E 's/\b(position|last_position|split_once|split_lines)\b/X/g'
S/core-H/core/strings.hero)`, empty, and the same for the two cards) that the
two variants differ in the four names and nothing else.

### F23. The author's rule has no stated boundary for Go's commonest contract, `(T, error)` (conventions-draft.md:45-52; ffi-pragmatist-sql.md:39-43; blind/README.md:24-29)

The rule: a Go name *"only where the contract is Go's"*. Experiment S
counts `cut`, Go's `(before, after string, found bool)`, against a
`Halves?` as a changed contract (renamed `split_once` in H), and `-1`
against `i64?` likewise. Neither brief says whether Go's `(T, error)` against
a Heroes `T?` is the same contract or a different one, and almost every
function of Go's `database/sql` (`Open`, `Exec`, `Query`, `Scan`, `Close`)
returns an `error`. The thick SQL shape is told to *"Name its functions by
the author's rule"*, so the ffi-pragmatist-sql decides that boundary alone,
and its decision is part of what experiment Q then measures. Item C should
state it before the thick shape is written.

### F24. The reader test's materials do not exist yet, so the critic's checks of them are owed (blind/README.md:18-22, 54-59)

`find docs/panel/234-briefs/blind -type f` prints `README.md` alone: no
`S/core-G`, `S/core-H`, `S/card-*.md`, `Q/thin`, `Q/thick`, no `task.md`, no
cases. The four questions this brief asks of the design (a variant
differing in more than the four names, a card saying more or less than its
module, a task decided by something other than the arm, a grading rule a
correct program fails) can be asked of the README alone in this pass
(F1, F18 to F23). The materials' checks, F22's `diff` among them, should run
when they stand and before the warden registers.

### F25. The spec-warden is told it needs no worktree, while the route to a real count of a draft is one (spec-warden.md:17-25, 59-64)

The brief: apply the draft *"to `spec/heroes-spec.md` in a scratch copy you
name, `--refresh`, then restoring it, if `--refresh` accepts that path; if it
does not, the vendored lower bound"*, and *"You need no worktree"*. Read:
`selfhost/measure/pinned.hero:106-111` refuses every `path` that is not
literally `spec` or `contract`, and `selfhost/cli/measure.hero:73-79` passes
`budgets.SPEC`, which is `"spec/heroes-spec.md"`
(`selfhost/measure/judged.hero:27-28`). So a scratch copy at another path is
refused, and the file at the literal relative path `spec/heroes-spec.md` in a
worktree of the warden's own is accepted, whatever it holds (read, unrun:
`--refresh` is the warden's API call, not the critic's). The brief's
"restoring it" reads as an edit of a `spec/heroes-spec.md`, and the only one
it names is the trunk's, which no seat may edit. It should name the worktree.

### F26. No seat holds the llm-ergonomist's veto on non-local constructs, and three items are locality questions (00-shared.md:10-13)

The panel's five judges are the compiler-engineer, the ffi-pragmatist, the
llm-ergonomist, the spec-warden and the historian
(`ls .claude/agents/`: five definitions, one per seat; `.claude/skills/panel/SKILL.md:8`, *"Five judges"*;
`.claude/agents/llm-ergonomist.md:40`, *"You hold a **veto** on non-local constructs"*). Here the fifth seat is the blind
reader test, *"the coordinator's experiment"* with *"no agent of its own"*,
and its sessions read names, cards and thickness only (C, D, E). Panel 232
read its blind seat as the ergonomist's (*"the blind seat; the coordinator's
reading"*, `docs/panel/232-...md:130`). Three items here are questions of
locality and are given to no reader: **B4** (a `use` resolved somewhere
outside the program's tree), **H** (`tag` on an `extern function`, a
contextual word in a new position), and **J** (`alias`, whose own design.md
paragraph says errors must *"expand the alias, which restores the locality
the indirection costs"*, `docs/design.md:3438-3440`). The brief should say who
holds that veto in this sitting, or give B4, H and J a blind reading.

### F27. Item M reopens two ratified rulings and the brief names neither as ratified (00-shared.md:78-80, 251-256)

Panel 097 was *"Ratified 2026-08-30"*
(`docs/panel/097-the-platform-arm-belongs-to-the-runtime.md:155`), its
condition 5 *"Nothing reaches `selfhost/library_source.hero`"* (`:129`).
Panel 137 adopted `sort_by` into that file and was *"Ratified 2026-09-13 as
adopted, nothing changed"* (`docs/panel/137-...md:214`), and `grep -n -i -E
"condition 5|097"` over panel 137 finds no line: it never met condition 5.
So item M weighs two ratified rulings that contradict each other, which the
brief states as *"adopted by panel 137 ... after panel 097's condition 5
closed that file"* without the word ratified. And the shared brief's opening
says issue 1154 rides this sitting *"by [its] own text"*; 1154's text says
*"the library grows at the milestone that builds packages over it"*, an adopted item to build here, not a question put to the
sitting (`gh issue view 1154`, grepped for `opening|sitting|M-core`: its
sittings are panel 137's). Condition 5 has held in practice: every function
now in the library (`range map filter fold find any all validated read_file
write_file args args_checked exit`) entered before 2026-08-30 (F28).

### F28. "`exit` is a function of the Tier-2 library since task 2250" has the wrong date (00-shared.md:241-243)

```
$ git log --format='%h %ad %s' --date=short -S'"function exit(' -- selfhost/library_source.hero
7327bd9bf 2026-08-18 M-selfhost-fixpoint step 2: the library stops being read from disk, and getenv was one extern away
$ git show 7327bd9bf:selfhost/library_source.hero | grep -n exit
174:        "function exit(code: i64)"
175:        "    hero_exit(code)"
```

`exit` has been a Tier-2 function since 2026-08-18; task 2250 (fafece48e,
2026-10-29, *"exit takes an i64, a literal status outside 0 to 255 is
refused"*) changed its contract, not its tier. The repair to design.md:534-535
stands (the sentence is false), and its citation is the older commit.
`args_checked` and `validated` entered on 2026-08-24 (1998b6f54, ff7f9d57a).

### F29. Item L's fallbacks are four, and a § 11 promise would be false on each (00-shared.md:245-249)

`runtime/parts/write.c:229-260` re-read: `hero_file_write` returns through
`hero_file_write_in_place` at `:233` (the route says in place: a second
link, a file not regular, a landing that cannot be resolved, a file this
process could not open for writing, per the route's comment above `:141`),
`:241` (no private name), `:251` (the rename refused) and `:259` (any other
stage shape). The brief's summary is accurate; a sentence in § 11 that says
`write_file` *replaces whole* must carry these cases or be false on them,
which is what the spec-warden's draft for L has to price.

### F30. "The page's table names nineteen packages" is twenty (spec-warden.md:46-48)

`sed -n 20,35p docs/roadmap/milestones/M-core-packages.md`, the first column's
code spans: `bufio bytes crypto/hmac crypto/sha256 database/sql encoding/json
html/template io log math net net/http net/url os path rand sort strconv
strings time`, **20** distinct (21 rows with `net/http`'s client and server
apart; the stray `str` the pipeline also caught is the `strconv` row's prose).
The warden multiplies a per-card budget by this number.

### F31. The root driver as conventions §1.6 describes it is refused, and it need not `use` every package (conventions-draft.md:36-39; compiler-engineer.md:25-27)

§1.6: *"one file at the root that holds `core/`, `use`-ing every package, so
`heroes test` on it runs every `test` block the tree holds"*. Run (scratch
`driver/`, the probe's `core/` tree):

```
$ cat driver.hero   # use core/strings / use core/net/http / main prints "driver"
$ heroes test driver.hero
error[unused_binding]: `use core/strings` is never used — nothing in this file names anything from `core/strings`
error[unused_binding]: `use core/net/http` is never used — nothing in this file names anything from `core/net/http`
$ cat transitive.hero   # use core/net/http alone, main calls http.is_get
$ heroes test transitive.hero
ok   "is_get"
ok   "contains"
2 tests, all passed
```

So a driver must **name** something from each package it `use`s (a call
made only to satisfy the rule), and `heroes test` runs the tests of every
module reached **transitively**, which probe 2 did not show (its `main.hero`
names both packages directly). A driver needs to reach only the packages no
other package uses. Both facts change what the compiler-engineer prices.

### F32. Item E's "made by the compiler" route starts further from a card than the brief suggests, and a third route is unlisted (00-shared.md:167-172; compiler-engineer.md:48-52)

The brief: *"`check --dump-scopes` prints the symbols and every binding"*.
Run on the probe's package:

```
$ heroes check docs/panel/234-briefs/probe/near-names/core/strings.hero --dump-scopes
module strings
symbols
  function index_of (line 2)
scopes
  function index_of
    param s (reads 3, writes 0)
    param part (reads 3, writes 0)
    ...
```

No parameter type, no result type, no failure code, no doc comment: a card's
every line except the name. A route nobody listed sits between the two the
brief names: a **program of the harness** that writes or checks each card
from the module (the shape of `tests/harness/issues.hero` and
`tests/harness/latency.hero`, which `.claude/rules/verification.md` § The
latency ceiling calls *"a program of the harness (a verb of `heroes` would be
one two invocations compose to, § 10's stopping rule)"*), no verb, no flag,
and drift caught by the net.

### F33. Routes nobody listed, beside F2's B5 and F15's module-relative header (00-shared.md:137-145, 214-217)

- **B, one canonical tree reached through a symlink in each program's root.**
  Run (scratch `symlink/`): `apps/server/core -> ../../canon/core`, then
  `heroes run apps/server/main.hero` prints `true` and `heroes test` runs the
  canonical tree's test, on this Mac. No copy, no compiler change. Its cost is
  Windows (git checks a symlink out as a file unless `core.symlinks` is set),
  unrun here, and the repository tracks **no** symlink today
  (`git ls-files -s | awk '$1==120000' | wc -l`: 0). It answers the brief's
  two questions (where the canonical tree stands, where its programs stand)
  differently from B1 to B3.
- **H, a form that reaches a C field.** F11 measured that a keyword as a
  field name has no route at all, and the two routes the brief lists reach
  functions only: a shipped `static inline` header renames a function, and
  `tag` on an `extern function` names a function. An identifier escape
  (the shape Rust's `r#match`, Zig's `@"match"` and Swift's backticks take)
  or a `tag` on a record **field** are the routes that would reach it, each a
  form owing Principle 0's ticket; neither is listed, so the sitting cannot
  rule that issue 1's field case is closed or knowingly left open.

### F34. The historian's brief hands it two premises to check rather than to carry (historian.md:28-32, 36-41)

- *"Swift's `firstIndex(of:)`"* as Swift's answer to *the position of a
  substring*. **Unverified by the critic** (no web read in this pass): to
  the critic's knowledge `firstIndex(of:)` finds an element (a `Character` in
  a `String`), and a substring's range is `firstRange(of:)` or Foundation's
  `range(of:)`. The brief should name the operation and let the historian
  find the name.
- *"panel 099's historian searched for this in 2026-09 and found none"*,
  where *this* is a root-relative language later *"forced to add one"* (a
  manifest or a search path). Panel 099 records the narrower search:
  *"a documented manifest-less root-relative language whose users were forced
  to add a manifest — it searched and found none"*
  (`docs/panel/099-where-a-use-path-starts.md:189-191`). The search-path half
  is new and unsearched.

### F35. Smaller citation slips (no change of meaning)

- Panel 099's premise is at `:95-99`, not `:95-98` (the sentence ends on
  `:99`, *"the root-relative path."*). (00-shared.md:51)
- conventions-draft §3.2 lists the built-ins' failure codes as seven and
  omits `null_cstr`, spec § 13's (`spec/heroes-spec.md:414`), the one a
  package over C meets when C hands back a null string; the spec names eight
  (`not_found` 324, `missing_key` 314, `does_not_fit` 334, `not_text`,
  `file_not_found`, `read_failed` 338, `write_failed` 339, `null_cstr` 414).
- The probe README says each output is *"its first lines"*; every probe's
  output re-run here is whole (each prints one diagnostic or a short run),
  so nothing was cut. (probe/README.md:4-5)
- Item I's witness lines: the milestone page cites `process.hero:49-53`
  (2026-09-10), the brief `:66-69`, which is today's (verified, F9).

### F36. Item F's routes join two axes the author asked about apart, and item G omits a route already in use (00-shared.md:186-192, 199-203)

The author's fourth instruction asks for the platform layer *"linked only
where a group names it, and behind one header or a family of them"*
(`M-core-packages.md:331-335`): two axes, the **object** (one runtime, or a
second object linked on demand) and the **header** (`hero_os.h`, or a family).
P1 fixes both at "one", P2 both at "two"; the routes between them are
unlisted: **the runtime's one object behind a family of headers** (a group
names `hero_net.h`, the object is the runtime's, nothing new to key or link),
and a second object behind `hero_os.h`. F9 bears on the header axis: today
every unit sees `hero_os.h` as a seed whatever its groups name, so "a family
of headers" changes what a group may bind without naming it. For item G, the
milestone page's own table carries a fourth route for text that the brief's
three plus § 13 leave out: `hero_str_from_bytes(p: ptr, n: i64)`, exported
for bindings (`runtime/heroes_runtime.h:358`, panel 035 by the page's table,
`M-core-packages.md:27`) and already used by `examples/checksum/base64.hero`
(`grep -rln hero_str_from_bytes examples`).

### F37. The bare arms' one sentence is identical across variants and true of only one of them (blind/README.md:35-40)

Both bare arms read *"it is Go's `strings` package written in Heroes"*. Under
H, four names are not Go's by the author's rule, and the reader is not told
the rule; a real reader of the packages would be, since the conventions page
(item C's deliverable) states it. So `bare-H` measures the rule
**unannounced**, against a sentence that says the opposite for those four
names, and `bare-H` against `bare-G` answers *"what does an undisclosed
rename cost"* rather than *"what does the author's rule cost a reader who
knows the packages' conventions"*. If the second is the question, `bare-H`
needs the rule's one sentence (and `bare-G` the matching one, that Go's
names are kept even where the contract differs), which F22's `diff` would
then show as the arms' only other difference, on purpose.

## Negative sentences checked that held, and what was searched

- *Panel 099 "holds no resolution refusing a search path"*:
  `grep -n -i "search" docs/panel/099-where-a-use-path-starts.md` prints
  `:95` and `:97`, both in the historian's premise paragraph. Holds (and see
  F2 for panel 028).
- *"R2's sentence never reached the specification"*: the brief's grep exit 1;
  widened (F7), only an unrelated line 289. Holds.
- *"§4.19 states no rule for it"* (the contextual words): no line of design.md
  `2532-2950` (§4.19 to §4.20) matches `contextual`; its twenty lines matching
  `[Ww]ord` state no family rule. Holds on those two words.
- *"thirteen verbs and no `doc`"*: `grep -c 'Command(' selfhost/cli/table.hero`
  13, and `heroes help` lists no `doc`. Holds.
- *"60 functions, every one distinct"*: 60 distinct `hero_*(` names outside
  comments in `runtime/hero_os.h` (61 with `hero_runtime_check_leaks`, which
  appears only in a comment). Holds.
- *"one translation unit ... the 29 files of `runtime/parts/`"*: 29 files, 29
  `#include "parts/` lines plus one in a comment (`runtime.c:19`). Holds.
- *"the language has no visibility"* (conventions §1.3): no line of the spec
  matches `visib|private|pub|export`; design.md:3255 refuses `private`. Holds.
- *"nothing re-exports"* (conventions §1.2): scratch `reexport/`, `a.twice` and
  `a.b.twice` through a module `a` that `use`s `b` are both
  `unknown_in_module`. Holds.
- *"`selfhost/`, `runtime/` and `seed/` unchanged since `v0.6.0`"*:
  `git diff --quiet 265e11e7f f0c4c99d2 -- selfhost runtime seed` exit 0, and
  `v0.6.0^{commit}` is 265e11e7f. Holds.
- The machines: `sw_vers` 27.0.1, `clang` Apple 21.0.0 (clang-2100.4.1.1),
  `docker version` 29.9.2, `heroes-linux-arm64:latest` present (clang
  22.1.8, `sqlite3.h`, `curl/curl.h` under the multiarch directory,
  libcrypto). Holds.
- Every other count the briefs give was re-run and holds: issue 805's four
  counts and the 15 functions (A); 55 directories and `gallery/` (B); 132
  lines in 16 files with the five per-file figures (F); 21 and 16 words (H);
  `SEEDS` at `externs.hero:86-87` (F, I); every design.md line of item K;
  `write.c:229-260` (L); the spec-warden's 8019, 7876, 10514, 5870 and 60
  (`heroes measure spec/heroes-spec.md`); `thesis.hero`'s seven words; panel
  137's prediction; issues 803, 893, 1072, 1073 and 1103 as the brief
  describes them.

## What this pass could not run, and why

- The reader test's materials, cards, tasks and cases (F24): they do not
  exist yet.
- Any `--refresh`, any `claude -p`, any web read: a paid run or an API call,
  none of them the critic's (F25 and F34 are read, unrun).
- Windows and Linux x86-64: not machines of this sitting. The Linux arm64
  container was used for F16 and to read the image's headers (the machines
  line above).
- One slip of the critic's own, recorded: a command of this pass wrote and
  removed, in the same line, a temporary file under `/tmp` (a stripped copy
  of `runtime/hero_os.h`) before the scratch folder was used for it; nothing
  else was written outside the repository's root.

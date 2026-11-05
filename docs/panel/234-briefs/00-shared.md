# Panel 234, shared brief: the core packages' opening sitting

**Convened 2026-11-05** in M-core-packages (row 76), as its opening sitting
(issue 803: *full five seats, and no step lands before it*), by the author's
yes of that day, asked once for the milestone (CLAUDE.md § 4), on the
milestone page's thirteen questions (i) to (xiii) (issue 2081 carries (x) to
(xiii)), the four read from Goose on 2026-10-25, issues 970, 1, 1154 and 2209,
which ride this sitting by their own text or by the conflict they carry, and
the author's three answers and four instructions of 2026-11-05. Read
`docs/roadmap/milestones/M-core-packages.md` whole, its last section, *Opened
2026-11-05*, first. Five seats and a completeness critic; **the critic read
these briefs before any seat started** (`docs/panel/234-reports/critic-briefs.md`,
37 findings, cited here as F1 to F37), and the briefs were repaired on them.

`proposal.md` is the proposal in ten lines; `conventions-draft.md` is item C's
draft; `probe/README.md` holds the compiler facts this brief states about
`use`, names and calls, each with its command and the compiler's whole output,
run on 2026-11-05 at f0c4c99d2 with `heroes 0.6.0`, `sources
1a1e340778db789a`, and re-run by the critic with its own compiler, all
seventeen reproducing. The critic's scratch,
`.claude/worktrees/234-critic-scratch/`, holds one folder per further run it
cites.

## Frames no seat argues past

1. **The packages add no language form and no built-in** (the milestone
   page's first paragraph). An item that would add one (H, J, M) owes
   Principle 0's ticket, CLAUDE.md § 2: the compiler needs it, or a measured
   design.md Part 11 effect, or a measured argument the panel accepts.
2. **Before `1.0.0` no form enters to keep an older program compiling**
   (CLAUDE.md § 2, the author, 2026-10-29).
3. **design.md §1.11 stands**: *Heroes ships a minimal runtime and nothing
   else ... Writing a broad standard library is out of scope, permanently*
   (design.md:490-492). Item A rules where a package falls against it, not
   whether it stands.
4. **The author's answers and instructions of 2026-11-05** (the page's last
   section) are the author's positions: the rule for names (*a Go name only
   where the contract is Go's*), which is measured and not argued away; both
   paid runs, the reader test and the thesis harness before and after; the
   rhythm of testing; coherence first; the platform layer beside the runtime.
   A seat that finds one of them unsound says so and why.
5. **Robustness outranks everything below the hard stops** (CLAUDE.md
   § Precedence).
6. **The llm-ergonomist's veto on non-local constructs is held by no seat in
   this sitting** (F26): its seat here is the reader test, which reads names,
   cards and thickness (C, D, E) and nothing else. Three items are questions of
   locality, B4 (a `use` resolved outside the program's tree), H (a contextual
   word in a new position) and J (`alias`). **Any of the three the sitting
   would admit owes the ergonomist's blind reading before it lands**, and the
   synthesis says so; a seat weighing one of them names what a reader of one
   file would no longer see.

## Rulings on the record, each re-read for this brief

- **A `use` path starts at the directory of the file you compile, the same
  from every file of the program; there is no climb, and a module above the
  root is a module of a different program** (panel 099 R1 and R4,
  `docs/panel/099-where-a-use-path-starts.md:117-146`). Its premise is *no
  manifest, no search path* (`:95-99`).
- **Panel 028 R3 refused a prelude the compiler finds on disk** (its
  *P-source*, *"a `prelude.hero` beside `runtime/`, discovered and parsed per
  compilation"*, `docs/panel/028-the-prelude.md:12-14`) **on a trust
  boundary**: *"a decoy `prelude.hero` has no analog — clang cannot type-check
  Heroes source"* (`:151-190`), and adopted *P-embed*, the source compiled into
  the compiler. Its text holds neither `search` nor `path`, but a root the
  compiler knows for packages is P-source's shape, and route B4 owes an answer
  to that argument (F2).
- **Panel 056 refused a per-project file and left module paths homeless**,
  *"`[workspace]`, module paths | beside the file: no path, no search list, no
  hierarchy"*, homed at M-package-manager with three return conditions
  (`docs/panel/056-what-a-project-file-would-carry.md:210-243`, `:322-330`;
  `docs/roadmap/milestones/M-package-manager.md:34-40`). And CLAUDE.md § 10
  admits three input classes, *a `.hero` file, argv, and the machine's
  environment* (`.claude/rules/cli-surface.md:47-56`): a search path set by a
  flag or by the environment is admissible there, as panel 055 took `CPATH`,
  `LIBRARY_PATH` and `PKG_CONFIG_PATH` for C, and one set by a project file is
  refused until the three conditions hold (F3).
- **An `extern` is never callable across a module boundary** (panel 033 R5);
  the compiler says so, `extern_across_modules` (probe 3).
- **No conditional-compilation form enters the language** (panel 114 R1); the
  platform axis was refused with a veto (panel 049); the platform arm lives in
  the runtime behind `hero_os.h` (panel 097, ratified 2026-08-30, whose
  condition 5 closes `selfhost/library_source.hero`, `:129`).
- **Panel 114 R2: a program may name a header it ships beside itself, so a
  difference between machines is written in C** (`:176-190`), **and R3: R2
  "does not land without" three instruments** (`:192-212`): (1) a header the
  program ships told apart from one the machine lacks; (2) **the compiler
  reporting which `extern` declarations it did NOT verify on this machine, the
  ffi seat's veto, binding every two-arm shape**, because a wrong type in the
  arm a machine does not compile is clang exit 0 and a running binary; (3) a
  fifth `declaration()` class for a syntax error in a shipped header. R2's
  sentence never reached the specification (`grep -n -i -E "ships|beside its
  own|conditional compilation" spec/heroes-spec.md`, no line). Whether R3's
  instruments stand today is a question: the critic found a witness mechanism
  for `ffi_missing_header` in `tests/harness/absence.hero:1123-1204`, which may
  be (1), and no trace of (2) or (3) (F7).
- **`compile "shim.c"` is two shapes with two standings** (F8): the group
  clause, `extern "gfx.h" compile "gfx.c"`, which panel 036's spec-warden
  vetoed (`docs/panel/036-the-ffi-ladder.md:39`, `:153`) and the sitting
  recorded as deferred (`:275`); and a package-level clause, which issue 927
  says *has never been judged*. Panel 114 R6 says a header of `static inline`
  functions needs no `compile` and *"that deferral can be closed"*
  (`:234-238`); issue 927 says that if no package of this milestone needs C
  too large for such a header, *the deferral closes for good*.
- **`alias` is deferred with a return condition** (design.md Part 7 item 5,
  panel 135), condition (b) met by the letter twice on 2026-10-25 (issue 2209).
  **Part 7 item 10's falsifier** asks for *a program the closure list or
  §4.19's ladder needs, binding a C `long`, `size_t` or `unsigned long`
  parameter, whose correct spelling is the same on all three CI legs*, written
  on 2026-08-14 when there were three; **the CI has four since 2026-09-17**
  (`.github/workflows/ci.yml:105`), and the item's correction of 2026-10-25
  says `labs(x: i32)` against `long labs(long)` builds here, a narrower
  parameter that C converts, unrun on Linux and Windows (design.md:3647-3652):
  a narrower spelling exists for a parameter, losing range, and none for a
  struct field, whose layout must match (F10).
- **Two ratified rulings meet in item M** (F27): panel 097 (ratified
  2026-08-30) closed `selfhost/library_source.hero` to new functions, and
  panel 137 (ratified 2026-09-13, *"as adopted, nothing changed"*) adopted
  `sort_by` into it (issue 1154) without citing panel 097's condition 5
  (`grep -n -i -E "condition 5|097"` over panel 137: no line). Every function
  now in the library entered before 2026-08-30 (F28).

## The tree and the rules every seat keeps

The seats work at **the commit that adds these briefs**, its parent f0c4c99d2
(M-core-packages step 0), `selfhost/`, `runtime/` and `seed/` unchanged since
`v0.6.0` (265e11e7f; `git diff --quiet 265e11e7f f0c4c99d2 -- selfhost
runtime seed`, exit 0). **Each seat that builds works in its own detached
worktree, `.claude/worktrees/234-<seat>/`, made by the coordinator**, and
builds its own compiler there from the seed, `clang -I runtime seed/heroes.c
runtime/runtime.c -o heroes` (a few seconds), checking that `./heroes
--version` says `sources 1a1e340778db789a`; a seat that must prototype in
`selfhost/` rebuilds with `./heroes build selfhost/main.hero -o heroes-next`
in its own worktree. **A seat reads nothing of another seat's worktree, writes
nothing outside the repository's root** (its scratch is its worktree or
`.claude/worktrees/234-<seat>-scratch/`), **and writes its report as it goes**,
`docs/panel/234-reports/<seat>.md` in the trunk at
`/Users/joseph/Temp/heroes/heroes-lang/`. **No seat prints a credential** (`gh
auth status`, never `gh auth token`), **no seat starts a paid run** its brief
does not name, **no seat starts a clock** (a cost is lines, files and bytes),
and **no seat edits the trunk's tracked files**.

The machines: this Mac (Darwin arm64, macOS 27.0.1, Apple clang 21.0.0); a
Linux arm64 container, `heroes-linux-arm64:latest` (clang 22.1.8, with
`sqlite3.h`, `curl/curl.h` and libcrypto, the critic read); Linux x86-64 is
the CI's leg after a push and Windows x86-64 is the author's box, off: **a fact
about Windows or Linux x86-64 is unrun in this sitting and is written so**.

## The items, each with what was measured for this brief

**A. What a package may hold, under §1.11** (question (i); Goose's admission
test). **Asked as the page asks it**, not answered (F4): *"verified by clang,
or by whoever wrote it, is the first question that milestone's opening sitting
has to answer for a package written in Heroes alone"*
(`docs/roadmap/milestones/M-package-manager.md:31-33`). A third verifier
nobody listed until the critic did: the Heroes checker itself, every type,
`T?` and label of a package, and its own `test` blocks run in the net. The
demand, by issue 805's command: `split_lines` 9 files, `is_space` 9,
`trimmed` 7, `index_of` 4, `tests/harness/strings.hero` 15 functions; and
enumerated from the tree rather than from the 2026-09-03 list (F13):
`is_digit(c: u8) -> bool` in 4 files, `is_letter` in 3, and a `str -> i64?`
number parser in 4 (`gallery/03-fallible.hero:14`, `rpn/main.hero:52`,
`todo/main.hero:94`, `assembler/program.hero:190`), beside
`query/data/table.hero:81`'s `int_of` and the harness's `to_number`: issue
1067's `strconv`. The ruling owes a falsifier (CLAUDE.md § 12).

**B. Where the packages live and how a program reaches them** (question
(ii)). Run (probe 1): from a program's root, `use net/http` reaches
`net/http.hero`, whose own `use strings` reads from the root; a leaf package's
tests run alone; a package that `use`s another, compiled directly, is
`unknown_module`; a program in a directory below the root cannot reach the
root's packages. Probe 2: a `core/` directory under the program's root,
reached by `use core/strings` and `use core/net/http`, binds `strings` and
`http`, and `heroes test` on a root file runs every module it reaches,
transitively (F31). **Under B2 a program keeps its own `strings.hero` beside
`core/strings`** in another file, the two mangling apart
(`h_corestrings_contains` and `h_strings_contains`), and a program module
`corestrings.hero` beside `core/strings` is refused `module_names_collide`;
**under B1 a program's own `strings.hero` is the package's path**, so every
package name is taken from every program's root (F5). **The corpus** takes a
directory of `examples/` only where `read_file(dir + "/main.hero")` succeeds
(`tests/harness/suite_corpus.hero:500-507`, `program_directories`), so a tree
with no `main.hero` is skipped as `examples/gallery/` is; a driver
`main.hero` inside the tree would make it a corpus program, and a copy of the
tree inside each program would add its `test` blocks to that program's; and
**nine suites name `examples`** (`canonical compat corpus emission probe
records special surface warnings`, with `cards.hero`, `gallery.hero` and
`platform_skips.hero`), so what a tree there does to the net is the nine's
question (F12). **A group's header is looked up from the compiled file's
directory, not the declaring module's** (F15, run): a package shipping
`core/net/net.h` beside `core/net/net.hero` builds from the root only as
`extern "core/net/net.h"` or with `--include core/net`, and is tested alone
only as `extern "net.h"`; the spelling that works from the root names the
package's place, and the refusal tells the user to install development files.
**The routes**, a list a seat extends: B1, the tree at the program's root,
`use strings`; B2, the tree under `core/` at the program's root, `use
core/strings`; B3, a copy of the tree under each program's root, by hand, by a
check or by `heroes fetch`; B4, a root the compiler knows for packages, set by
a flag, by the environment, or by a project file (each with its standing under
the rulings above); **B5, the canonical tree compiled into the compiler**,
panel 028's P-embed applied to `core/` (F2); **B6, one canonical tree reached
through a symbolic link in each program's root**, which runs on this Mac (F33)
and which a Windows checkout gives as a file unless `core.symlinks` is set,
unrun, the repository tracking no symbolic link today. And two questions every
route answers: where the canonical tree stands in this repository, and where
the programs that use it stand (the loopback server; the hooks of issue
2082).

**C. The conventions every package keeps** (the author's instruction 3, the
naming rule, question (xi), Goose's third). `conventions-draft.md` is the
draft, now with a **boundary for the naming rule** the coordinator proposes
for judgement (F23): Go's `(T, error)` and a Heroes `T?` are one contract; a
sentinel or a `found`/`ok` flag is another. Measured: a function may not take
a built-in's name, all eight tried (F6), while a module may (`use sort` beside
the built-in `sort`); a call into a package is always qualified, UFCS not
reaching across a module (`needs_qualifying`), and labelled at every call
with two parameters of one type (`needs_label`), so a package's parameter
names are part of its surface (F20); the near-name suggestion repairs a typo
or a case, `indexOf` to `index_of`, and says nothing for Go's `index` against
`index_of`; Go's contract on a `T?`, `strings.index_of(...) < 0`, is
`bad_operand`, its fix `.must()`, an abort; two module names differing in case
are one file on this Mac (probe 4's history).

**D. Thin or thick over C** (questions (ix) and (xii)). Panel 033 R5 makes
the C module and one Heroes function beside each member mandatory, so every
package over C has a thin layer; the question is whether the package's face
is that layer or an API over it. The precedent the page names is PHP's, to be
verified rather than recalled. The reader test settles it (`blind/README.md`,
experiment Q): SQLite in both shapes, the ffi-pragmatist writing both, its
tasks taking their data from `args()` so no answer must parse text (F1).

**E. The interface card** (question (x)). `heroes` has thirteen verbs and no
`doc` (`selfhost/cli/table.hero:155-167`); `check --dump-scopes` prints a
module's names and its parameters' names, and **no parameter type, no result
type, no failure code and no doc comment** (F32, run); M-doc-generator (row
79) promises `heroes doc`; CLAUDE.md § 10's stopping rule binds a new verb or
flag. **Three routes**: a card the compiler makes (a flag, or `heroes doc`);
a card written by hand and held true by a check; **a program of the harness**
that writes or checks each card from its module, the shape of
`tests/harness/issues.hero` and `tests/harness/latency.hero` (F32). And the
budget, measured with `heroes measure`.

**F. Where the platform's C lives** (questions (iv), (v), (vi); the author's
instruction 4; issues 927, 893, 1073). Today: `runtime/hero_os.h` declares 60
distinct functions; `runtime/runtime.c` includes the 29 files of
`runtime/parts/` as one translation unit, compiled once and cached
(`selfhost/cli/runtime_key.hero`, `selfhost/cli/toolchain.hero`); 132 lines in
16 files of `runtime/` name a platform macro, the most in `parts/replace.c` 36,
`parts/run.c` 19, `parts/fs.c` 14, `parts/stack.c` 11 and `parts/spawn.c` 10;
**every translation unit includes `heroes_unit.h` and `hero_os.h` before any
group's header** (`selfhost/emit/externs.hero:86-87`, `SEEDS`), so a group may
bind a `hero_os.h` function whatever header it names (F9, run); spec § 13
says *threads, processes and files from the language's `hero_os.h`*. Question
(vi) is answered by panel 114 R1. **Two axes, which the author's instruction
names apart** (F36): the **object** (the runtime's one, or a second linked
where a group names its header) and the **header** (`hero_os.h`, or a
family: `hero_net.h`, `hero_time.h`). **The routes**: P1, one object and one
header, as today; P1', the runtime's one object behind a family of headers;
P2, a second object behind its own header or headers, project C tested on
every leg; P2', a second object behind `hero_os.h`; P3, a header of `static
inline` functions shipped with the package that needs it (panel 114 R2's
shape), which meets item B's header lookup (F15); P4, `compile` in either of
its two shapes (F8). **Every route with two arms in C meets panel 114 R3**
(F7). The compiler accepts a header shipped beside the compiled file:
`find tests/golden -name '*.h'` lists 567, 12 of them under
`tests/golden/fixedbugs/` (F17, correcting this brief's *seven*), every case
read reaching its header from the compiled file's own directory, the
placement F15 shows works. The platform facts the routes must meet,
**carried from issues 803 and 893 (2026-09-03, 2026-09-05) and unrun since**:
`sockaddr` differs between Darwin and glibc, `clockid_t` is `u32` on Darwin
and `i32` on glibc, `pthread_t` is an opaque pointer on Darwin and an 8-byte
`unsigned long` on glibc, and Windows is winsock; never measured, `tv_nsec`
is a C `long`, 32 bits on Windows. **`timespec_get` is no monotonic clock**:
`TIME_MONOTONIC` is defined on neither machine, only `TIME_UTC`, a wall clock
(F16, run), so the monotonic clock has no portable C spelling here.

**G. The byte buffer** (question (iii)). Spec § 13 today: *`@md: [u8]
counted_by N lent` hands C `N` elements*, and *`f.validated_bytes()` ... for a
`[u8]` every byte, and either fails `not_text`*; and a fourth route already in
use: `hero_str_from_bytes(p: ptr, n: i64)` (`runtime/heroes_runtime.h:358`),
which `examples/checksum/base64.hero` calls (F36). Whether these answer
question (iii)'s three routes for `read` and `recv` is unrun.

**H. A C name spelled as a word Heroes reserves** (question (vii), issue 1),
**re-aimed on the critic's runs** (F11). A **contextual** word blocks nothing
tried: POSIX `link` binds as `extern function link(...)` and runs, and fields
and parameters named `tag`, `owned`, `when`, `rest`, `zero`, `lent` run. What
blocks is the 21 **keywords** of `selfhost/keywords.hero`'s `keyword()` and
the 17 **foreign words** of `selfhost/foreign_words.hero`'s `declared_word`
(`case class const def elif enum fn func import int let null struct switch try
union var`; panel 013's own line names *"the foreign-word registry"*). **In
function position the compiler already prescribes the shipped header's route**,
`static inline int kw_match(int x) ...`, in its own refusal; **in field
position there is no route at all**: `record R` with a field `match` is
`expected_field` and three cascading errors, no hint. Issue 1's counts
(`function` 571, `func` 29, `assert` 11, `test` 5, `match` 3) are panel 013's of
2026-08-03 with no method written, to be re-measured. The routes for a field,
neither listed before the critic (F33): an identifier escape (the shape of
Rust's `r#match`, Zig's `@"match"`, Swift's backticks) or a `tag` on a record
field, each a form owing Principle 0's ticket and frame 6's reading. design.md
names the contextual words' family once, inside a Part 6 row (line 3246), and
§4.19 states no rule for it.

**I. A group's header: per group, per module, or per module and the seeds**
(question (viii)). Three positions, not two (F9): per group; per module, the
union of its groups; and per module plus the seeds, which is what the compiler
does. The witness, `selfhost/cli/process.hero:66-69`, declares `hero_dir_scan`,
`hero_dir_at` and `hero_dir_release` under `extern "stdlib.h"`, in a module
that also has an `extern "hero_os.h"` group at `:40`; **the seeds alone explain
it**: a module whose only group is `extern "stdlib.h"` declaring
`hero_dir_release` builds and runs, and a name no header declares is still
refused `ffi_unknown_name` (the critic's run, `pergroup/`). A per-group check
must say whether the seeds count, which is item F's header axis.

**J. `alias`, and the C-width vocabulary** (issue 2209; question (v)).
Whether condition (b) measures what it was meant to, both commits repaired
with a record and a qualified name; whether moving a type into a package
(`examples/json`'s value type into `encoding/json`) is Go's own warrant
arriving; and whether any core package's binding meets item 10's falsifier,
read on four legs and with the narrower-parameter correction above, which
depends on F's route.

**K. design.md's repairs** (issue 970), verified 2026-11-05 with each
repair's own `grep`: **one is done**, the `#include` order, written by panel
204 (design.md:1019-1023, :2583); **four are owed**: Part 7 item 4 still says
*no aliases, no package hierarchy* (:3426); §4.1 still says *there are no
aliases and no wildcard* (:896); §3.5 still names no condition that returns a
project file (:870-886); §1.11's Tier-2 list omits `args_checked` and
`validated` (:521-524), which entered on 2026-08-24; **and a sixth**: §1.11
says *the one edge that cannot be Tier 2 is `exit(code)`* (:534-535), and
`exit` has been a Tier-2 function since 2026-08-18 (7327bd9bf), task 2250
changing its contract on 2026-10-29 and not its tier (F28).

**L. `write_file` and the replace** (Goose's fourth). `write_file` stages a
copy and renames it over the path, and falls back to a write in place on
**four** routes (`runtime/parts/write.c:233`, `:241`, `:251`, `:259`: a second
link, a file not regular, a landing unresolved or not writable; no private
name; the rename refused; any other stage shape), so a § 11 sentence saying it
*replaces whole* must carry those or be false on them (F29).

**M. `sort_by`: a built-in, the `sort` package, or neither** (issue 1154; the
two ratified rulings above). Measured: `use sort` beside the built-in `sort`
compiles and runs, and a function named `sort` inside a package is refused
(probe 3; F6); the compiler is a program whose root is `selfhost/`, so by
probe 1's rule it cannot `use` a package not under that root. Issue 1154
names four hand-written sorts in `selfhost/` by line, from 2026-09-13,
unverified since.

**N. How the packages' effect on a model is measured** (question (xiii);
Goose's first; issue 2081). The instrument is `tests/harness/thesis.hero`
(words `tasks`, `dry`, `grade`, `plan`, `run --live --max-dollars`,
`preregister`, `report`); a context is `spec`, `none` or a variant
specification's path (`tests/harness/thesis_prompt.hero:21-88`); whether a task
can carry modules beside its answer, which a package task needs, is unread.
The held-out run is the author's act in their own terminal (journal 069, *The
seal held*). The author's words were *before and after*.

## Issues on the milestone this sitting does not rule

893 (the loopback server, one connection per thread), 925 (copy-on-write's
test-then-mutate), 1069 (a service that stops cleanly), 1072 (the long run),
1073 (only an `i64` crosses into a thread) and 1103 (shape B-prime) are homed
on the steps that write the server and JSON; 1067 (`strconv`) is item A's
evidence and the first batch's work; a seat that finds one of them decides an
item here says so.

## What the sitting produces

One resolution per item, provisional, the most robust and complete and never
the cheapest (CLAUDE.md § 4); `conventions-draft.md` adopted or amended as the
packages' conventions page; the design.md amendments (K, and any other item's)
in their own commit after the sitting's; the decision issue `Panel 234:`,
which also answers issue 2081's four questions.

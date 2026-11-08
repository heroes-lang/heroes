# Panel 237, the completeness critic's second pass: the reports

Written 2026-11-08 in worktree `.claude/worktrees/237-critic/` (detached at `163912031`, compiler
`sources 0d7a6741345899d8`), after reading in lane `p237` the six reports of
`docs/panel/237-reports/` (compiler-engineer, ffi-pragmatist, spec-warden, historian,
llm-ergonomist, reader-test), the briefs as repaired (`57d0909d1`), R3's registration (`15a9b28d7`),
the runner `p237-scratch/blind237/blind237.py` as it ran, `build/b237/graded/table.tsv` and
`build/b237/refix/result.tsv`, and defects 2343, 2344 and 2347 (`gh issue view`). No paid run, no
credential, no write outside this worktree; another seat's worktree was not opened. Scratch programs
are under this worktree's `build/critic/`.

## What reproduced, so the synthesis can lean on it

- The reader test's ledger, recounted from the 216 `run.json` files (`python3 -I`, reading
  `total_cost_usd`, `is_error`, `modelUsage`, `num_turns`): reader 12.7948 USD plus the seat
  0.3635, **13.1583**; `is_error` 0, no `run.failed-*` file; each session's `modelUsage` names the
  model its folder says (72 each of Haiku, Sonnet, Opus); every session 3 turns.
- The refusal counts (`awk` over `table.tsv`): 118 refused, 98 correct; `lent_shape` R1 44, R2 31,
  R3 36; `expected_args_close` 3 + 2, `unread_releaser` 1, `never_rebound` 1. The refix: 66 correct,
  9 still refused (`cut -f2 result.tsv`).
- The grader's counters: `connections.txt` stands in all 26 R1 programs graded correct, `stores.txt`
  in 38, `journals.txt` in 34 (each `0 0`); no R1 program names `sqlite3_open_v2`, which the shim
  does not interpose (`grep -l`).
- The blind seat's three programs, extracted from its report and built with my compiler: variant 1
  prints `5 0 0`, variant 4 `0 0`, the store program `5 0 0`, each exit 0, nothing open at exit
  (shim or `stores.txt`). Its outputs were asserted by a session with no shell, and they hold.
- Measurement 052 § 4 scores both variant contexts, and its line 108 says no task of the suite binds
  C (`grep -n`), as the spec-warden cites.
- The compiler-engineer's reading that `special`'s two programs no longer witness the class agrees
  with my first pass's finding 7: the add-overflow `stdint.h`, reached through `CPATH` and so not
  refused, still aborted at 134, the reserved pair giving the builtin back.

## Findings

### 1. The reader test could not have produced a `silent` program from a reader who followed the task

This is the reading I would not stand behind, and my first pass missed its cause: I checked that each
task was *solvable* under every arm, not that the outcome the rule counts was *reachable*.

R1's and R2's step 5 tells the reader the close can refuse and asks for a second close. A bare
`consumes` binding cannot compile a second close (`error[handle_used_after_end]`, the README's dry
run and the ffi-pragmatist's table); a `when 0` binding that skips the retry aborts at `main`'s end
(defect 2291's third golden, `panic: 1 taken at ...`); `sqlite3_close_v2` closes. So a `silent`
program had to ignore step 5. Counted over the graded programs (`awk '/^function main/ {m=1} m'` then
`grep -cE` for the close's name):

```
R1 A/B/C/D: 18 of 18 programs call the close twice or more in main, in every arm
R2 A/B/C/D: 18 of 18, in every arm
R3 A/B/C/D: 0 of 18 (once each), in every arm
```

And the binding followed the task's documentation, not the fence. Every R1 program, **arm C's 18
included** (its fence shows neither `sqlite3_close` nor `when`), marks `sqlite3_close ... when 0`:

```
R1 A: 18 programs; sqlite3_close marked when 0: 18; sqlite3_close_v2 declared: 15; called in main: 13
R1 B: 18;                                         18;                           11;                9
R1 C: 18;                                         18;                           18;               15
R1 D: 18;                                         18;                           18;               14
```

R2 is `when 18` in all four arms and R3 `bare 18` in all four (`table.tsv`'s column). So
`reader-test.md`'s *the fence's reading moved no first-try outcome measurably* is true of these
tasks and says nothing of defect 2322's shape, which is a reader copying the fence with no sentence
telling it the close can refuse. With A at 0 silent, the rule's `p(silent lower)` was 1.0000 for
every arm before a session ran: the rule could not prefer anything. And `reader-test.md`'s *on R1,
51 of 72 bound `sqlite3_close_v2` and 21 a close marked `when 0`* reads the runner's label column,
which says `v2` when `_v2` is declared and called; all 72 carry `when 0` on `sqlite3_close`, 62 of
them beside a `_v2` line.

What would measure the fence: a task that does not name the refusal (open, prepare, close, finalize,
print the codes, no step 5, no documentation sentence), where copying the fence is the reader's only
guide. Its size at this sitting's measured means (Haiku 0.0064, Sonnet 0.0481, Opus 0.1232 USD a
session, from the `run.json` files): 4 arms × 3 models × 6 = 72 sessions, about **4.26 USD**. A paid
run, the author's to decide.

### 2. Half the sessions died on one mistake the specification's own sentence invites

109 of the 111 `lent_shape` refusals are `lent` on a handle parameter (`awk` over `table.tsv`, the
text after *word nothing reads*: 42 + 36 + 31 *is a handle, the pointer itself*, 2 others), 109 of
216 first tries, and **Sonnet built 0 of its 72** (24 of 24 refused on each task; 71 `lent_shape`,
1 `never_rebound`). So each arm's `correct` count rests on 12 sessions, Opus's and Haiku's.

`grep -n lent spec/heroes-spec.md` and `grep -n -i handle` find no sentence saying a handle takes no
`lent`. Lines 441-443 say *a group's record declared `lent` without `@` is a pointer to const*, and
line 422 says a record *with a `tag` and no fields is a handle*. Read together they admit `db: Db
lent`; the blind seat read them that way (its choice point 7: *`lent` would make it a pointer to
const*). The compiler refuses it (`build/critic/lenth/`: `error[lent_shape]: ... db is a handle, the
pointer itself, which C receives by value`, exit 1), on panel 171's ruling
(`docs/panel/171-lent-and-the-pointer-c-hands-back.md:132`, *`lent_shape` ... on a handle*), which
the specification never absorbed. Defect 2347 files the missing `Fix` as `adjacent`. Whether the
specification states a rule its compiler enforces is not an `adjacent` question: by CLAUDE.md § 12 a
spec the compiler disagrees with is the compiler's bug, or the spec's sentence is wrong. It sits in
§ 13, the section item 3 amends, and it moved more first tries than any arm did. The sitting should
ask it.

### 3. A route nobody built for the environment: read the `-###` answer the build already has

Both compiler-side answers re-derive clang's reading of the environment: the compiler-engineer's
`P237_ENV=count` splits `CPATH` and `C_INCLUDE_PATH` itself (+25 lines), and both seats propose a
blanket rule for `CCC_OVERRIDE_OPTIONS`. The compiler already asks clang `clang -###` once a build
(`selfhost/cli/toolchain.hero:169`, `clang_floor.identity`, for the toolchain's key), and that answer
holds every environment edit as a plain word of the cc1 line. Parsed with `shlex` over `clang -###
-std=gnu11 -D_USE_MATH_DEFINES -D_CRT_SECURE_NO_WARNINGS -fno-strict-aliasing -O0 -g -c
runtime/runtime.c` (`build/critic/cfg/`):

```
plain:                                  -D _USE_MATH_DEFINES, -D _CRT_SECURE_NO_WARNINGS, -I/usr/local/include
CPATH=envdir C_INCLUDE_PATH=envdir CCC_OVERRIDE_OPTIONS='#+-DP237X=5', added:
                                        -D P237X=5, -I<w>/envdir, -c-isystem <w>/envdir
CPATH='' (empty), added:                -I.
```

So one reader of a text the build already holds names each folder and each switch the environment
added, with clang's own reading of empty elements and separators, and the variable that added it, at
no added process. It is a direction, **unbuilt**, so by `/panel` 3c not adoptable; what a lane would
run is the compiler-engineer's matrix and the four rows below. The plain line's
`-I/usr/local/include` is Apple clang's own: on a Mac where that folder exists (Homebrew's prefix on
Intel) it is searched before the platform's as an `-I`; how the refusal classes a file found there
is **unrun**, the folder being absent here and outside the repository.

### 4. The compiler-engineer's count rule misses `C_INCLUDE_PATH`'s empty elements

The prototype counts *an empty `CPATH` element as `.`, an empty `C_INCLUDE_PATH` one as nothing*
(its report, the variant compiler's fourth edit), on the measurement *`C_INCLUDE_PATH=` (set, empty):
clang's own alone*. That is the wholly empty value. Its elements are another reading
(`C_INCLUDE_PATH=<v> clang -### -c t.c`, the `-c-isystem` words):

```
''                      (none)
':'                     "." "."
'/nonexistent:'         "/nonexistent" "."
':/nonexistent'         "." "/nonexistent"
'/nonexistent::/other'  "/nonexistent" "." "/other"
```

Through the landed compiler, from a folder holding the `true`-as-0 `stdbool.h`, `C_INCLUDE_PATH=':'
heroes-next build sub/main.hero` builds at exit 0 and the program prints `false`
(`build/critic/cipempty/`). The prototype as described would let it through. A rule that re-derives
clang's reading has to be right about each variable's empty forms on each platform (Windows' `;` is
unrun in every report); finding 3's route reads clang's answer instead.

### 5. The `CCC_OVERRIDE_OPTIONS` disagreement, and what the commands support

The compiler-engineer would set it to `#` for every clang (measured: `'#'` alone applies nothing and
the overflow program aborts at 134); the ffi-pragmatist leans to refusing a build where it is set (not
built; one `getenv`). Both close the door; the question is what each does to a person who set it on
purpose. **This repository has done so**: `git grep -l CCC_OVERRIDE_OPTIONS` names one file,
`docs/measurements/055-...md:80`, whose first pass gave clang the linker through
`CCC_OVERRIDE_OPTIONS='#+-fuse-ld=lld'`. Under the `#` route that edit is dropped with nothing said;
under the refusal the build stops and names the variable. The commands support the ffi-pragmatist on
*tell, never silently discard*, since §4.17 asks a message to carry its own repair and a dropped edit
carries none, and they support neither blanket rule over finding 3's, under which `-fuse-ld=lld` adds
no word the class reads and passes, while a `-D` of a kept name or an `-I` of a folder holding an
opening header is refused, naming the variable. `CCC_OVERRIDE_OPTIONS`, `CPATH` and `C_INCLUDE_PATH`
are not set in this shell (`${VAR+yes}`).

**Their other reported disagreement is not one.** On `CPATH` and `C_INCLUDE_PATH` both reports count
the folders as named: the compiler-engineer's condition (a), *counts the folders of `CPATH` and
`C_INCLUDE_PATH` as named*, and the ffi-pragmatist's argument, *Count the environment's folders as
named, an empty element as the working directory*. Clearing is out on both: *not available* and
against design.md §4.19's *the search path is the machine's own `CPATH`* (compiler-engineer), and
vetoed on a measurement (ffi-pragmatist: GNU readline's `RL_READLINE_VERSION` 2051 read as 1026, and
`HIST_ENTRY` laid out in 16 bytes where the library writes 24, at exit 0). Where they differ is only
the empty element of `C_INCLUDE_PATH`, finding 4, which neither measured.

Other environment inputs Apple clang names (`strings -a $(xcrun -f clang)`): `CLANG_CONFIG_PATH`,
`CLANG_NO_DEFAULT_CONFIG`, `/etc/clang.cfg`, `COMPILER_PATH`, `SDKROOT`, `MACOSX_DEPLOYMENT_TARGET`,
`_CL_`. One tried here: a `-DP237X=5` config file named by `CLANG_CONFIG_PATH` (as the file and as a
directory holding `clang.cfg`) added nothing (`int seen = P237X;`); `--config <file>` did. Two forms
tried, so a question rather than *no door*; `/etc/clang.cfg` does not exist here.

### 6. Defect 2343's repair has no route built, and the one sketched misses a shape I ran

The compiler-engineer sketched, unbuilt, a `-H` listing of the unit's opening with its groups,
refusing a named folder's file *only where a platform header or the runtime's included it*. A program
whose own group header includes the library's header is outside that rule by construction, and it is
the same wrong value (`build/critic/viaown/`): `extern "lib.h"` binding `constant BUFSIZ: i64` and
`lib_buffer()`, `lib.h` = `#include <stdio.h>` and an inline returning `BUFSIZ * 2`, beside a
program-folder `stdio.h` that redefines `BUFSIZ` as 7:

```
clean:  build 0, prints 1024 2048
evil:   build 0, prints 7 14
```

And a name-only rule (refuse a named folder's file a platform header shares a name with) would refuse
the compiler-engineer's `utilown`, a correct program printing 7, `util.h` being an SDK name. So the
repair needs a rule that tells *the group's own header* from *a library header any header asks for*,
and its cases are at least four: the SDK-reached `_stdio.h` (refuse), `viaown` (refuse), `utilown`
(build), and `stdiogroup`, the group naming `stdio.h` itself, which the compiler-engineer read as
*the program named `stdio.h`* and filed nothing (a ruling the sitting owes, since `viaown` shows the
same file read for another header's include is the class). Nothing is adoptable for 2343 until one
route builds and is run on those four.

### 7. A false note on item 2's refusal, reproduced and not filed

The ffi-pragmatist wrote that item 2's refusal carries the allow-list's note. Reproduced with a
stand-in `.pc` (`build/critic/pcmain/`, `PKG_CONFIG_PATH`, `Cflags: -I<inc> -Dmain=SDL_main`):

```
error[ffi_package]: the package `p237fake` answered with `-Dmain=SDL_main`, which this compiler does not pass on: `main` is a name the C this compiler writes, ...
  note: only `-D`, `-U`, `-I`, `-L`, `-l`, `-F` and `-isystem` ... are accepted ... everything else is refused, because a word the list does not name could be one that runs code during the build (Go's CVE-2018-6574)
  note: name the library directly with `link` if you need it
```

The first note says `-D` is accepted, under a refusal of a `-D`, and gives a reason that is not this
refusal's; the second offers `link`, which drops the package's `-I` the group's header is found by.
A false message is `blocking` by `.claude/rules/verification.md` § Bounded discovery. Defects 2343,
2344 and 2347 do not cover it.

### 8. Claims asserted and not settled by the command that would settle them

- **The refusal's latency** (compiler-engineer): *at most 0.24 % of a warm build of the compiler* is
  244 M over `check selfhost/main.hero`'s 102.3 G on the premise that a build does at least the front
  end's work, an inference; the instrument the latency rule admits is `heroes run
  tests/harness/latency.hero -- compare` (`.claude/rules/verification.md` § The latency ceiling),
  unrun. The 244 M itself is measured, and the seat flags its own +85 M as unprofiled.
- **The spec-warden's payment for B**: P2 (B preferred over A on R2, C not) is scored falsified, and
  `reader-test.md` does not score P3 (*B's silent at most 2 of 36*), which held, 0 of 36. Both rest
  on finding 1's ceiling: A was 0 of 36 too, so P3 says nothing about B. By the spec-warden's own
  text a failed P2 sends B to the removal branch, whose removal (*and a call returning nothing takes
  no clause*, -10 vendored) has no real count; its `--refresh` is a paid request, unrun.
- **The blind seat's context answer**: the coordinator's header says it *names its directory's files
  and nothing of the project*; the seat says *the only other input was the agent setup: the
  working-directory path and the date*, and that path is `.../heroes-lang/.claude/worktrees/p237/build/b237/seat`.
  The synthesis should record what the seat said.
- **The blind seat's registered condition fired**: *If variant 4's task-3 rate matches variant 3's,
  I would approve variant 4*, and *variant 2 or variant 4 at ≤10% on task 3 ... I would change those
  verdicts to approve*: 0 of 18 under every arm. By its own condition every variant is now approve;
  that the condition was met under finding 1's ceiling belongs beside it.
- **The seat said its only tool was Read**, launched with `--tools Read,Write` and `--allowedTools
  Read,Write`; its `run.json` has `permission_denials: []`, so it never tried `Write`. Why the tool
  was absent is unrun (a session would settle it); it cost nothing here, the coordinator having
  copied the text.
- **Platform counts**: the compiler-engineer's 3,410 platform header names and the ffi-pragmatist's
  3,408 are two denominators (the first adds the toolchain's `usr/include`), not a disagreement.
- **The historian's open question** on clang's `-Wshadow-header`: Apple clang 21.0.0 here answers
  `unknown warning option '-Wshadow-header'` (`clang -Wshadow-header -fsyntax-only -I dir angle.c`),
  so it is no instrument on this Mac; on Debian clang 22.1.8 it is unrun.

## What the synthesis must not adopt without a further run

1. **Any reading of the reader test as "the fence does not matter"** (finding 1). It measured that,
   told the close can refuse and asked for a retry, every model in every arm binds a close whose
   retry compiles. The fence's effect on defect 2322's shape needs a task that does not name the
   refusal (about 4.26 USD at this sitting's means, the author's call), or the synthesis rules on
   item 3 from the other seats and says the reader test is silent on it.
2. **The environment half of 2344 in either prototype's shape** (findings 3 to 5): not before the
   chosen route is built and run on `CPATH=dir`, `C_INCLUDE_PATH=dir`, `CPATH=''`,
   `C_INCLUDE_PATH=':'` and `'/x:'`, `CCC_OVERRIDE_OPTIONS='#+-D<kept name>...'` and
   `'#+-fuse-ld=lld'` (measurement 055's use), the compiler-engineer's `envown` and the
   ffi-pragmatist's readline binding (2051).
3. **A blanket `CCC_OVERRIDE_OPTIONS='#'`** without recording that it silently drops an edit this
   repository has used (finding 5).
4. **Any repair of 2343** before a route builds and is run on `_stdio.h`, `viaown`, `utilown` and
   `stdiogroup` (finding 6).
5. **B as paid** on P3 alone (finding 8): its payment is the removal branch, real count unrun.
6. **Item 2's message as landed** (finding 7): the note is false.
7. **Defect 2347 as the whole of the `lent` finding** (finding 2): the specification's lines 441-443
   and panel 171's ruling disagree in a reader's hands, and the sitting should rule which text is
   right before a `Fix` is written for the compiler's side.

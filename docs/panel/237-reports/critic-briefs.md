# Panel 237, the completeness critic's first pass: the briefs

Written 2026-11-08 in worktree `.claude/worktrees/237-critic/`, detached at `163912031`, as I went.
The compiler: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` (real 13.22 s), then
`./heroes build selfhost/main.hero -o heroes-next`; `./heroes-next --version` prints `heroes 0.6.0`
and `sources 0d7a6741345899d8`, the same as lane `p237`'s `heroes-next`. No paid run, no credential,
no write outside this worktree. Every scratch program lives under this worktree's `build/critic/`,
which `.gitignore:7` ignores.

## What reproduced as written

Listed so the coordinator knows what needs no repair; each is the command and what it printed.

- `git -C p237 rev-parse --short HEAD` prints **`e6e692868`** (the briefs' own commit), not
  `163912031`; `git diff --stat 163912031 e6e692868` touches only the 16 files of
  `docs/panel/237-briefs/`, so the tree every seat measures is `163912031` as the brief says. The
  brief's parenthesis (*`git rev-parse --short HEAD` in lane `p237`*) no longer prints that commit:
  see finding 1.
- `96d42c250`, `ebdc8a28c`, `37fb22678`: each `git merge-base --is-ancestor <c> 163912031` yes;
  subjects as the proposal says.
- `grep -rln shadowed_header tests/golden | wc -l` prints `0`; `grep -c 'error\[' spec/heroes-spec.md`
  prints `0`.
- `suite_special.hero`: `grep -n` puts the comment at 638 and the function at 647.
- `selfhost/cli/shadowed.hero:237-246` is `function diagnostic`, which pushes one note and no `Fix`.
- `wc -l`: `shadowed.hero` 345, `package_macros.hero` 101, `guarded_names.hero` **273**, `c_words.hero`
  274, `heroes_reserved_open.h` and `heroes_reserved_close.h` 79 each.
- `ls tests/golden/run/fixedbugs-2333-*.hero` 4, `ls tests/golden/unsupported/fixedbugs-2333-*.hero` 3.
- `docs/design.md:2577-2582` is the fence binding `sqlite3_close(db: Db consumes) -> i64`.
- The two thesis contexts: `grep -c 'function sqlite3_close(db: Db consumes) -> i64$'` prints 1 in
  each; `shasum -a 256` gives `b36c80dd...` and `6e4979e4...`, the README's two pins.
- `sqlite3 --version` prints `3.54.0 2026-04-09`; `pkg-config --modversion sqlite3` prints `3.54.0`.
- `docker images` lists `heroes-linux-arm64:latest` (7 weeks old) and `heroes-linux-arm64:sdl3-b14`.
- The blind draw: `python3 -I -c "import random; x=['A','B','C']; random.Random(2373).shuffle(x);
  print(x)"` prints `['C', 'A', 'B']`, `variants.tsv`'s order (`random.Random(2373).sample` would
  print `['B', 'A', 'C']`, so the README's *drawn with* means `shuffle`).
- The prepared folders under `p237/build/b237/`: `cmp` of the seat's `heroes-spec.md` with
  `spec/heroes-spec.md` at `163912031` prints nothing; the six blind files `cmp` equal to the
  committed ones; `diff` of each reader spec prints nothing for A, line 393 for B, 392-393 for C, and
  the six specs of each arm share one sha256 (`fea65798`, `a2cdf90b`, `3cabe593`).

## Findings

### A. Framing facts that do not reproduce as written

**1. The overflow program aborts only when it is given an argument.** `00-shared.md` says *the same
program with no `stdint.h` aborts, `panic: integer overflow`, exit 134*. Its `main` adds
`args().len()` to `i64`'s top, and run with no word that length is 0:

```
$ ./heroes-next build main.hero -o prog && ./prog            -> exit 0, prints 9223372036854775807
$ ./heroes-next run main.hero                                -> exit 0, prints 9223372036854775807
$ ./prog one ; ./heroes-next run main.hero -- one            -> exit 134, panic: integer overflow at main.hero:4:11
```

(folder `build/critic/one-line/`, `stdint.h` moved aside). A seat re-running the sentence as written
reads exit 0 and a value, the very symptom the class names. The brief should name the argument.

**2. design.md §4.19's copy is not "the same" two lines: it is refused by today's compiler on
another line.** `docs/design.md:2580` binds `sqlite3_open(path: cstr, @out: ...)` with no `lent`
and the fence has no `constant SQLITE_OK`. The fence with `special`'s own `main`
(`suite_special.hero:440`) built in `build/critic/fence/design/`:

```
$ ./heroes-next build main.hero -o prog
error[lend_kept]: `.cstr()` lends bytes to C for this call and no longer, and `path` of `sqlite3_open` is not declared to let them go ...   (exit 1)
```

The spec's fence under arm A or B builds and prints `0`. So question 8's *design.md §4.19's copy*
owes `lent` (and arguably `SQLITE_OK`) whichever arm wins; the proposal's *the same* hides it.

**3. Arm C is priced in tokens alone, and it moves two instruments.** `suite_special.hero:440`
builds and runs spec § 13's fence with a `main` that calls `sqlite3_close(db)`; under C the fence
binds no such function (`build/critic/fence/armC/`): `error[unknown_name]: nothing named
`sqlite3_close` is in scope`, exit 1. `suite_spec.hero`'s `fragments` pins the fence byte for byte
(`git show ebdc8a28c -- tests/harness/suite_spec.hero`), and `examples/sqlite/main.hero:68` and
`examples/ledger/db/sqlite.hero:97` bind `sqlite3_close ... when 0` (`grep -n`), so C makes the
specification disagree with the project's two SQLite programs while B agrees with them. Neither
cost is in the spec-warden's or the compiler-engineer's brief.

**4. The thesis contexts are less frozen than the brief says.** `00-shared.md`: *the README pins
their sha256 as the frozen inputs of a registered measurement*. The registered block,
`tests/thesis/held-out/prediction.md:131`, names one context, `spec`, at sha256 `4d8e5251...`
(`git show a4e09e061:spec/heroes-spec.md | shasum -a 256` reproduces it); lines 113-119 list the
two variant contexts under *What else the run reports, exploratory*. Their sha256s are pinned by
`tests/thesis/contexts/README.md` alone (`git grep -l b36c80dd616737bd` prints only the README).
And `git diff --shortstat a4e09e061 163912031 -- spec/heroes-spec.md` prints 41 insertions and 20
deletions: the contexts already differ from today's specification far beyond this one line. The
spec-warden's item 4 should be asked of those facts.

**5. Smaller ones.**
- *`git rev-parse --short HEAD` in lane `p237`* prints `e6e692868` (the briefs' commit), not
  `163912031`; harmless today (`git diff --stat` touches only `docs/panel/237-briefs/`), but the
  sentence is the instrument a seat is told to trust.
- *`guarded_names.hero` and `c_words.hero` (274)*: `wc -l` prints 273 and 274.
- 2333's cost: the commit body (`git log -1 96d42c250`, lines 31-33) says *0.17 s to 0.20 s, best
  and median ... base against base 0.16 best, 0.17 median*; `shadowed.hero:105-109` says *0.16 s:
  0.18 s so, 0.22 s with the listing's files digested*. The brief reads the body as *0.17 before,
  0.20 after*, which it does not say. The re-measure in instructions the brief asks settles it.
- `blind/README.md` puts the seat's folder *under the seat's worktree's `build/`*; the runner puts
  it under lane `p237`'s (`WORK = LANE + "/build/b237"`, and `ls` shows `p237/build/b237/seat`).
- The proposal calls item 2 *a name the guard keeps*, decision 2342's title (`gh issue view 2342`)
  *a name C reserves*; `package_macros.kept_name` refuses the guard's names (`main`, `true`, the
  runtime's words) as well as the reserved ones. Question 3 should name which one it rules on.

### B. Negative sentences, run

**6. Each negative in the briefs, run.**
- *no golden case annotated in its source*: `grep -rln shadowed_header tests/golden | wc -l` prints
  0. Stands.
- *the specification names no diagnostic code*: `grep -c 'error\['` is one spelling. Over the 266
  codes the goldens print (`grep -rhoE '(error|warning)\[[a-z_]+\]' tests/golden | sort -u`), one
  appears in the spec in backticks, `not_text` (lines 356 and 434), and there it is a runtime
  failure's name, not the diagnostic. Stands, with that sentence.
- *`heroes check` exits 0*: yes, and `check --json` prints `"diagnostics": []`. Not in the brief:
  **`heroes test main.hero` exits 1** with the refusal, as `build`, `run` and `--emit-c` do.
  Question 5 should list all five verbs.
- *the compiler-engineer: the `-M` listing "does not see" a header reached only under a group's
  switch* (`shadowed.hero:32-35`): left to that seat, as briefed. One neighbour I ran: a group
  header `lib2.h` that includes `<string.h>` beside a program-folder `string.h` that `#undef`s and
  redefines `memcpy`, `memmove`, `memcmp` and `strlen` built at exit 0 and printed `3 4 abcdef 6`,
  the clean build's output (`build/critic/grouphdr/`); without the `#undef`s it is refused,
  `ffi_header_refused`, `-Wmacro-redefined`. No wrong value found there.
- *`--refresh` refuses any other path*: read, not run (a refresh is paid):
  `selfhost/measure/pinned.hero:106-112` and its test at 138-147 refuse every path but the spec
  and the contract. Stands.
- *"where a `.pc` here sets one"* (ffi-pragmatist item 3): over the 507 packages `pkg-config
  --list-all` names (510 lines, 507 distinct), 241 carry a `-D` or `-U`; the names are 23
  (`-DNOMINMAX` 343 times, the Qt switches, `_THREAD_SAFE` twice, `_DARWIN_C_SOURCE` once); no
  `_REENTRANT` and no `_FILE_OFFSET_BITS` here, and none of the 23 is a word of
  `emit/guarded_names.hero` (`grep -qw` each). So item 2 refuses no package on this Mac, an
  inference from a word match the ffi-pragmatist should settle with the compiler.

### C. Routes nobody listed

**7. The class is reached through the environment, and no route in the briefs closes it.** Item 1
refuses a file of *a folder the build names*: the program's own, `--include`'s, a package's.
clang also searches the folders `CPATH` and `C_INCLUDE_PATH` name (`OBJC_INCLUDE_PATH` it does not, for C: an `#error` `stdint.h` there builds at exit 0), and panel
056's table (`docs/panel/056-what-a-project-file-would-carry.md:319`) lists `CPATH` beside
`--include` as the way a Heroes program names a header folder. In `build/critic/cpath2/booldef/`, a
program `b = true; print(b); if b print("yes")` beside nothing, and an environment folder holding
`stdbool.h` = `#include_next <stdbool.h>`, `#undef true`, `#define true 0`:

```
$ CPATH=<env> ./heroes-next build main.hero -o bin && ./bin           -> build 0, exit 0, prints "false"
$ C_INCLUDE_PATH=<env> ./heroes-next build main.hero -o bin2 && ./bin2 -> build 0, exit 0, prints "false"
$ CPATH=<env> ./heroes-next run main.hero                             -> exit 0, "false"
$ (no variable) build and run                                         -> "true", "yes"
$ the same stdbool.h in the program's own folder                      -> error[shadowed_header], exit 1
```

A `#error` file in the `CPATH` folder (`build/critic/cpath/`) shows clang reads it: `internal
error: the runtime did not compile`, exit 2. The add-overflow macro through `CPATH` is given back by
the reserved pair (the sum still aborts, 134), so the refusal's own reason, *a file read in the
library's place can redefine anything the unit takes from the library* (`shadowed.hero:15-17`), is
what reaches. `-iquote` and `-idirafter` on the named folders do not touch the environment either.
Routes that would: count the folders clang reports from the environment as named (`CPATH=dir clang
-E -v -x c /dev/null` lists `dir` in the `<...>` search, and so does `C_INCLUDE_PATH`; that is the
list `listed` already reads), or clear those variables for clang (the compiler already
keys its cache on them: `toolchain.hero:270`, `opening.hero:15`, `clang_floor.hero:91`, defects
569, 2289). Which is right is the sitting's; that the class is open through this door is measured.

**8. Item 2 has the same door: `CCC_OVERRIDE_OPTIONS`.** clang's driver appends what that variable
names to every command. In `build/critic/ccc/`:

```
$ CCC_OVERRIDE_OPTIONS='+-D__builtin_add_overflow(a,b,r)=((*(r)=(a)+(b)),0)' ./heroes-next build main.hero -o bin
  -> exit 0 (stderr: "### Adding argument -D__builtin_add_overflow... at end", three times)
$ ./bin one   -> exit 0, prints -9223372036854775808      (a clean build: exit 134, panic: integer overflow)
```

This is a debugging knob a person sets on purpose, much less likely than `CPATH`; it is the shape
beside a package's `-D`, and the sitting should say whether the class's line is *what the build
names* or *what clang is handed*.

**9. `-isystem` is not a route; the brief lists it as one.** Question 1 offers *clang's `-iquote`,
`-idirafter`, `-isystem` orders*. With Apple clang 21.0.0 (`clang --version`), a folder holding a
`stdint.h` that `#warning`s, `clang <order> dir -fsyntax-only` over `#include <stdint.h>` and over
`#include "stdint.h"` (`build/critic/orders/`):

```
-I dir          <stdint.h> read    "stdint.h" read
-iquote dir     <stdint.h> NOT     "stdint.h" read
-isystem dir    <stdint.h> read    "stdint.h" read
-idirafter dir  <stdint.h> NOT     "stdint.h" NOT
```

`clang -isystem dir -E -v` lists `dir` first, before clang's resource directory and the SDK. And
`package_words.hero:44-52` already records why a package's word is not read as `-isystem` (panel
205, defect 569: it would quiet what the header region keeps).

**10. The `-iquote` route needs the emission changed first.** `./heroes-next build main.hero
--emit-c` of task 2's program (repaired) writes the group's header as `#include <money.h>`, an
angle include (line 22, between `heroes_guard_open.h` and `heroes_guard_close.h`). Under `-iquote`
for the program's folder that include finds nothing. The compiler-engineer's prototype has to
change the group's include to the quote form too, and say what a group header named like a library
header then does.

**11. What a reorder route changes that the refusal does not, on this Mac.** Question 2 counts only
names the opening reads. The opening's listing under the unit's words (`clang
@build/shadowed/words-*.rsp -M build/shadowed/opening-*.c`) names 36 library headers; over the 85
`-I` folders of the 507 packages, **0** files bear one of those names, so the refusal stops no
package here. But 3 headers in 2 package folders share a name with a platform header (SDK
`usr/include` or clang's resource directory): poppler's `Form.h` (poppler, poppler-cpp,
poppler-glib) and GNU readline's `readline/readline.h` and `readline/history.h`, named by
`fluidsynth`'s `.pc`, where the SDK's `readline/readline.h` is a link to `../editline/readline.h`.
An `-idirafter` route for package folders would hand a binding libedit's header where the package
links GNU readline's library; the refusal leaves it alone. Over the tracked tree, 0 files outside
`runtime/` bear an opening header's exact name (`git ls-files` against the 34 basenames).

**12. Item 3 has a fourth arm nobody priced.** The spec's own prose (`spec/heroes-spec.md:459`)
teaches `acquires sqlite3_close | sqlite3_close_v2`. Arm D, that mark with `sqlite3_close ... when
0` and a `sqlite3_close_v2` line beside it, measured like A to C (`build/critic/fences/`, `diff`
against the spec three lines): claude-legacy 8210, cl100k_base 8357, so +29 and +30. A, B and C
reproduce exactly: 8181/8327, 8183/8330, 8187/8333.

### D. Questions the sitting should ask, and seats handed what their input cannot answer

**13. Does an arm's lesson travel to a library with no `_v2`?** The reader test and the seat's
task are SQLite, the one library where a close that cannot refuse exists beside the one that can,
and both task texts give every arm `sqlite3_close_v2`'s declaration and its documentation. C
teaches *pick the close that cannot fail*; B teaches *mark the result that means done*. Which one a
model carries to `pthread_mutex_destroy` (EBUSY) or any release with no twin is the thesis question
item 3 is about, and nothing in the materials can measure it. One more task on a release with no
twin, under the same three arms, would.

**14. One model, six samples.** The reader test runs `claude-opus-5-5` alone; the thesis
instrument's registered models are four (`prediction.md:131`). A verdict on *a model* rests on one.

**15. Question 5 and question 1 together**: the sitting asks whether `check` should answer the
refusal; with finding 7 it should also ask whether a refusal that `check` cannot see and the
environment can bypass is the line, or whether the environment belongs to it.

### E. The blind materials

**16. The seat can tell which variant is today's.** `brief.md`: *One of them is what the block says
today; you are not told which*. The seat's `heroes-spec.md` (arm A, `cmp`-equal to the
specification) holds `variant-2.md`'s two lines verbatim at lines 392-393, and its prose at 459
names `sqlite3_close | sqlite3_close_v2`. Label-stripping (the skill's step 2) is defeated by one
read. Repair: the seat's spec with those two lines replaced by a marker naming the three variants,
or three specs.

**17. The sessions run inside a git tree with two `CLAUDE.md` above them, and the runner does not
load the key `--bare` requires.** The skill's route is a folder *outside the repository and
outside any git tree, with no `CLAUDE.md` in or above it*. `git -C p237/build/b237/seat rev-parse
--show-toplevel` prints the lane `p237`, whose `git log` subjects name the sitting (*Panel 237:
the briefs ...*); `CLAUDE.md` stands at `p237/` and at the trunk. `--bare` skips `CLAUDE.md`
discovery and auto-memory (`claude --help`, CLI 2.1.311), and `--safe-mode` disables `CLAUDE.md`;
whether the session's git status (panel 183's leak) is still sent is **unrun**, a question no
unpaid command here settles. Measured route around it: `GIT_CEILING_DIRECTORIES=<.../build/b237>
git -C <seat> rev-parse --show-toplevel` prints `fatal: not a git repository`, so the runner can
set it in each session's environment. And `claude --help` says of `--bare`: *Anthropic auth is
strictly ANTHROPIC_API_KEY or apiKeyHelper via --settings (OAuth and keychain are never read)*;
`blind237.py` passes no environment and loads no `.env`, and `${#ANTHROPIC_API_KEY}` in this shell
prints 0. If the coordinator's shell is the same, every session fails, the README counts each
failure as `refused`, and the reader test *decides nothing* at a cost of zero, with nothing saying
why. The runner should refuse to start without the key and the grader should read `run.json`'s
error before a program.

**18. The grader misclassifies three program shapes; a behavioural grader separates them.** With
the runner's own `binding()` (imported from `blind237.py`, no word of it run) and my compiler
(`build/critic/grade/`):

| program | runs | `binding()` | truly |
|---|---|---|---|
| `both`: the spec's `acquires sqlite3_close \| sqlite3_close_v2`, both closes bound bare, `sqlite3_close` called once | `close 5`, exit 0 | `v2` (not silent) | silent: connection left open |
| `spaced`: `function sqlite3_close (db: ...)`, which builds | `close 5`, exit 0 | `none` (not silent) | silent |
| `finfirst`: bare binding, finalize before close | `close 0`, exit 0 | `bare` + quiet = **silent** | closed |

A 15-line C shim interposing `sqlite3_open`, `sqlite3_close` and `sqlite3_close_v2` and writing the
connections still open at exit (`build/critic/grade/shim.c`, `clang -dynamiclib -lsqlite3`), run
as `timeout 20 env DYLD_INSERT_LIBRARIES=shim.dylib ./prog` (the variable must be set after
`timeout`, which strips it otherwise: the first try read 0 everywhere), prints 1 for `both`,
`spaced` and the README's `Abare`, and 0 for `finfirst`, `B` and `C`. *Silent* = exit 0 and a
connection open at exit is a definition no spelling games.

**19. The decision rule's threshold sits inside the noise.** *Preferred when its silent programs
number at least two fewer than A's, of six*: the one-sided Fisher exact p of a two-program gap at
six per arm is 0.227 to 0.284 (2/6 against 0/6: 0.227; 4/6 against 2/6: 0.284); a three-program gap
0.091 to 0.121; four, 0.030 (`python3 -I` with `math.comb`). The rule also ignores `refused`: an arm
whose readers fail to build has fewer silent programs and wins. And the B-C tie goes to *the
cheaper on the real tokeniser*, which needs the paid refresh; on the vendored rows B is cheaper.

**20. The grader bounds a run's time and not its bytes.** `grade()` runs `timeout 20 ./prog` with
`capture_output=True` and writes all of it to `run.out`; `.claude/rules/verification.md` § A run
that may not end asks for both bounds (`head -c` into a file). `heroes build` there has no timeout.

**21. Task 2 has no registered criterion.** Question 4 for the blind seat is *whether the message
and note lead to the right repair*; the README registers no rule for reading the answer. Task 2 is
solvable as written: its message is byte-identical to what the compiler prints for those three
files (`diff`, `build/critic/task2/`), and both repairs build and print `125000` (the `stdint.h`
deleted; renamed `money_types.h`).

**22. What reproduced in the blind materials**: the draw (`random.Random(2373).shuffle` gives C, A,
B), the prepared folders (`cmp`, `diff`, one sha256 per arm), the four references' outputs
(`refs/*/run.out`, `build.out`), and the reader task is solvable under every arm: under A by
binding `when 0` (spec line 459's paragraph defines it) or `sqlite3_close_v2` (the task gives it);
the README's `Aretry` shows the bare retry refused, `handle_used_after_end`.
  `refs/Aretry/build.out` (7 lines) names `when` once, in its second note's list (*an end ... under
  `when`, through a copy*), and never `when 0` as the repair, as the README says.

## The repairs I would make to the briefs, most important first

1. **Grade the reader test on behaviour, not on the binding's spelling** (finding 18): *silent* =
   exit 0 with a connection open at exit, read through an interposed `sqlite3_open`/`close`/`close_v2`
   shim run as `timeout 20 env DYLD_INSERT_LIBRARIES=... ./prog`, with `head -c` on its output
   (finding 20). Keep `binding()` as a reported column only.
2. **Make the paid runs able to succeed and unable to leak** (finding 17): the runner refuses to
   start without `ANTHROPIC_API_KEY` (which `--bare` alone reads) and loads it from the trunk's
   `.env` without printing it; each session gets `GIT_CEILING_DIRECTORIES` above its folder; the
   grader reads `run.json`'s error before anything and names a CLI failure as such, never as
   `refused`.
3. **Register a decision rule with power, before any session** (finding 19): at six per arm a gap of
   two is p ≈ 0.23; either more sessions per arm, or a gap of four (p 0.030), and a second count
   for `refused` so an arm cannot win by confusing its readers. Say what breaks a B-C tie if the
   paid refresh is not granted.
4. **Add the environment to question 1 and to the compiler-engineer's and ffi-pragmatist's items**
   (findings 7 and 8): `CPATH` and `C_INCLUDE_PATH` reach the class at exit 0 today (`print(true)`
   prints `false`), and `CCC_OVERRIDE_OPTIONS` reaches item 2's; each route must be measured on
   them, and panel 056 row 4 already counts `CPATH` as a way to name a header folder.
5. **Strike `-isystem` from the routes, or say it was measured and stands in** (finding 9); tell the
   compiler-engineer that `-iquote` needs the group include emitted as a quote include (finding
   10), and ask what each reorder route changes beyond the opening's names (finding 11: GNU
   readline under `fluidsynth`, poppler's `Form.h`).
6. **Hide the status quo from the blind seat** (finding 16): the seat's spec with the two fence lines
   replaced by a marker, since `variant-2.md` stands verbatim at its lines 392-393.
7. **Price arm C in what it moves, not only in tokens, and add arm D** (findings 3 and 12):
   `special`'s program and `fragments` under C, the two `examples/` that bind `when 0`; D, the
   spec's own `sqlite3_close | sqlite3_close_v2`, at +29/+30 vendored.
8. **Correct design.md §4.19's line in question 8** (finding 2): its copy is refused today
   (`lend_kept`, no `lent` on `path`), whichever arm wins.
9. **Correct the shared brief's overflow sentence** (finding 1): `./prog one` or `heroes run main.hero
   -- one`, not the bare program, which exits 0 printing `9223372036854775807`.
10. **Restate the thesis contexts' status** (finding 4): pinned by their README; the registered
    block names only the `spec` context, at a specification that has already moved by 41 and 20
    lines.
11. **Ask question 13's transfer question** (a library with no `_v2`) and register a criterion for
    task 2 (finding 21); name all five verbs in question 5 (finding 6, `test` exits 1); fix the
    smaller facts of finding 5 (lane HEAD, 273 lines, the timing reading, the seat folder's place,
    *guard keeps* against *C reserves*).

No paid run was made; nothing was written outside this worktree (the scratch programs are under its
`build/critic/`); lane `p237`, `p237-scratch` and `p237/build/b237/` were only read.

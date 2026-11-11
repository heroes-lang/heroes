# Panel 245, the completeness critic's first pass, over the briefs

Written 2026-11-10, 22:34 to about 22:55. The compiler: the base `a4b89740b` archived into
`.claude/worktrees/p245-scratch/critic/tree`, then `clang -I runtime seed/heroes.c runtime/runtime.c
-o heroes` (`real 12.23`) and `./heroes build selfhost/main.hero -o heroes-next` (`real 124.32`,
`user 92.72`; the load was 17 when it started, so neither is a duration). `heroes-next --version`
prints `heroes 0.6.0` and `sources 62c952067d86d0f5`. Every probe is under
`.claude/worktrees/p245-scratch/critic/` (`q1`, `q1b`, `q1c`, `q2`, `q3`, `q3/locked`). Read-only
everywhere else; no paid run, no GitHub write (`gh issue view` and `gh issue list --search` only).

Each item gives the fact, the command, what it printed, and the repair I propose to the brief.

## A. The base, the classes, the records

1. **The base holds panel 243's R1 and not its R2 or R3, while design.md at the base describes all
   three.** Ran: `git merge-base --is-ancestor 2a079fb89 a4b89740b` (b30-shadow's head, defect 2428).
   Printed: *not in the base*. `git log 89db84347..a4b89740b` shows b30-variadic merged (`0a40576b4`)
   and `a785d86e2` *design.md amended by panel 243's R1, R2 and R3*. `git -C b30-shadow status`
   shows uncommitted edits to `selfhost/cli/shadowed_tree.hero`, `shadowed_words.hero` and
   `stand_in.hero`, and issues 2428 and 2447 read `OPEN`. **Repair**: 00-shared § The base should say
   "R1 landed, R2 and R3 not (lane b30-shadow is open on `cli/shadowed*` and `cli/stand_in.hero`)".
   Q3's "panel 243's R2 made a folder a `pkg-config` answer names its library's" is true of
   design.md and false of the base's compiler. Any Q3 route the seats build edits the same files as
   that live lane.

2. **Panel 243 is still pending, and the brief does not say so.** Ran: `gh issue view 2449`, then
   `gh issue view 2421`. Printed: both `OPEN`; 2449 is *"Panel 243: ... R1 to R5, ratification
   pending"*. **Repair**: say that Q1 and Q2 rest on a provisional R1. Add one sentence the sitting
   can lean on: under 243's conservative alternative, the numbers-only scope, `sprintf`'s `n: i64`
   is still a number past `...`, so 2464's door shape stays the only route whichever way the author
   rules.

3. **Verified as written.** These need no repair:
   - `issues.hero -- open --kind defect --class systemic` lists exactly 2464 and 2465. I ran it with
     my base compiler.
   - The base is `a4b89740bc36...` in `b30-round`, `p245` and the three seat worktrees.
   - `order_trial.hero:161` is the `assert` holding the opening (the test starts at `:159`).
   - The opening's text holds the region's `error "-Wformat"`, `error "-Wformat-nonliteral"` and
     `ignored "-Wformat-security"`. The three deprecation words come only after the pop
     (`emit/deprecation.hero:27-38`).
   - `docs/panel/208-*.md:93-97` is R1, and the quote spans `:96-97`.
   - The four `fixedbugs-2036` `run` cases are as described. One of them has the format in a
     variable: `const char *m = "hi\n"; printf(m)`.
   - design.md `:3186` is *"The environment is a named folder"*.
   - The door's body stands inside the region. I emitted `q1/main.hero --emit-c`: `push` at line 5,
     `#include <own.h>` at 23, `pop` at 32, the three quiet lines at 45 to 47.

4. **Issue 2464's "Where to look" names a file that does not exist.** Ran: `ls
   selfhost/cli/header_region.hero selfhost/emit/header_region.hero`. Printed: *No such file* for
   the first; the module is `selfhost/emit/header_region.hero`, which the brief cites correctly.
   **Repair**: the coordinator corrects the open issue with `edit` (asked once).

5. **The brief calls 2448 "the case a systemic class exists for", but it stands `blocking`.** Ran:
   `gh issue view 2448`. Printed: label `class:blocking`. **Repair**: the brief should say whether
   the sitting reclassifies it. The tag counts it either way, but the class line is a record.

## B. Q1, defect 2464

6. **Reproduced.** Ran: `heroes-next build main.hero` on a copy of `b30-scratch/variadic/dep584/`
   (`q1/`). Printed: exit 0 and 20 lines of stderr, with *"./own.h:5:12: warning: 'sprintf' is
   deprecated ... [-Wdeprecated-declarations]"*; the binary prints `2`.

7. **Panel 208's R1 already rules the outcome, and its own case no longer witnesses it for this
   shape.** Ran: `sed -n 93,103p docs/panel/208-*.md` and `grep -n 'deprecated binds' docs/design.md
   .claude/rules/c-boundary.md`. Printed: R1's heading is *"a name its header marks deprecated binds
   and builds silent"*, and one of its own `run` cases is *"`sprintf` with fortification off, each
   building silent at exit 0"*. design.md `:2676-2677` and c-boundary.md say the same. Today that
   case (`run/fixedbugs-584-sprintf-with-fortification-off-builds-silent.h`) quiets its own call
   with a `push`/`ignored`/`pop` of its own. So the `warnings` suite can no longer see 2464 through
   it. **Question the sitting should ask**: is 2464 *"a ruling no rule reaches"*, as its class line
   says, or a ratified rule (208's R1) broken by a provisional one (243's R1)? R1 promises an
   outcome (*builds silent*) and a mechanism (*after the groups' close, nothing else widened*), and
   the two now disagree. Which half binds?

8. **The class is wider than the three deprecation words, and two of its members are undefined
   behaviour at exit 0.** Ran: `q1b/own.h`, one door per shape, built and run with `heroes-next`
   at the default level and at `-O2`. Printed, every build at exit 0 with clang's raw text:
   - `-Wdeprecated-declarations` (a function, and an enumerator) and `-Wdeprecated-pragma`;
   - `-Wattribute-warning` at the default level, and absent at `-O2`;
   - `-Wshift-count-overflow`: `1 << 40` on an `int`, run printing `2` at `-O0` and `10` at `-O2`;
   - `-Warray-bounds`: `a[5]` of an `int[2]`, printing `2` at `-O0` and `0` at `-O2`;
   - `-Wconstant-conversion`, `-Wbuiltin-memcpy-chk-size` (a `memcpy` of 6 into 2 bytes) and
     `-Wpointer-sign`;
   - in `q1c/`, a header's `#warning` (`-W#warnings`) and `#pragma message` (`-W#pragma-messages`).

   Plain C, `t.c` with `#pragma clang diagnostic ignored "-Wall"` around such a body, prints the
   same three warnings I tried (shift, constant conversion, array bounds). With `-Weverything` in
   place of `-Wall` it prints none. So each is on by default and outside Apple clang 21's `-Wall`,
   which the region's ignore cannot reach. Panel 205's routes S1, S3, H and P2 measured `-Wall`'s
   families, and none of these. Panel 205's own title promises *"judged as clang judges a system
   header"*, where clang says none of them. I searched the issues for `Warray-bounds`,
   `shift-count-overflow`, `constant-conversion`, `memcpy-chk`, `#warning`, `W#warnings` and
   `Wvisibility` and found no filing; that absence is a question, not a premise.

   **Repair**: Q1 asks what a door's body, and a header's own code, prints for a warning that is on
   by default and in none of the region's lists. Rank the warnings that name undefined behaviour by
   § Precedence's rank 3: 205's R2 already makes the UB ones it knew errors (`KEPT`). Three routes
   nobody listed:
   - (a) `ignored "-Weverything"` in the region, then the errors raised again;
   - (b) the UB words added to the region's errors and the advice words quieted, with a test that
     fires when clang's on-by-default list moves;
   - (c) the region read in clang's system-header mode, as 205's title says, with a measurement of
     whether `#pragma clang diagnostic error` still holds there.

9. **The census reproduces from the lane's files, over a narrower set than "2,400", and it is a
   different census.** Ran: `cat census/run.sh` (its arms are `base` and `lit`, so it is the
   `-Wformat-nonliteral` census), then `grep -l -E 'Wdeprecated-declarations|Wdeprecated-pragma|Wattribute-warning'
   w/*.err | wc -l` and the same over `wt/`. Printed: `0` of 9,600 files in `w/`, and `2` in `wt/`
   (`b459`/`l459`, one header in both arms: `tests/golden/unsupported/fixedbugs-158-noted-outer.h`).
   The base arm of `results2.tsv` has 1,337 of the 2,400 Homebrew headers at exit 1 in that shape,
   so the "0" is over the 1,063 that compile. The raw warnings that do appear are
   `-Winclude-next-absolute-path` (1,432 lines, an artefact of including by absolute path) and
   `-Wvisibility` (33, in headers refused anyway). I counted the lane's outputs and did not re-run
   clang. **Repair**: say "0 of the 1,063 that compile in the region's shape, read from the lane's
   `-Wformat-nonliteral` census", and ask a seat to re-run it counting every warning word of item 8.

10. **The one tracked header is not a bystander: five carriers of defect 158's cases rely on the
    region speaking a warning.** Ran: `git grep -l` for deprecation and warning attributes in tracked
    `*.h`, then `grep` of the `fixedbugs-158-*.h` carriers. Printed: `noted-mid.h` uses a deprecated
    name as its carrier (*"since 2026-10-10 a use of the first header's deprecated name"*), and
    `warned.h`, `warned-mid.h`, `warned-outer.h` and `sibling-a.h` use `#warning`. The cases are
    `a-note-in-another-header-between`, `a-warning-above-the-missing-include`,
    `a-warning-in-each-header-of-the-stack` and `a-warning-in-a-sibling-header`. They test that
    `ffi_missing_header` reads the right include among other warnings. A route that quiets
    deprecation in the region removes one carrier, and a route like item 8's (a) removes all five,
    and the cases go on passing while testing nothing. **Repair**: name these cases in Q1, and add
    to the compiler-engineer's brief "what each route does to `unsupported/fixedbugs-158-*`".

## C. Q2, defect 2465

11. **Q2's lead fact is false at the base: a direct `printf` binding with nothing past `...` is
    refused, and has been since 2026-09-03.** Ran (`q2/`), with `heroes-next build`:
    - `printf(format: cstr lent) -> i32`, called with `a[0].cstr()`, with a Heroes literal, with a
      `cstr @= s.lease()` copy and with `getenv(...)`;
    - the same binding never called;
    - `dprintf`, `fprintf`, `snprintf`, `syslog` and `sscanf`, each with nothing past `...`;
    - `curl_mprintf`;
    - a door of my own marked `__attribute__((format(printf, 1, 2)))`.

    Printed: every one exits 1 with *"error[ffi_call_shape]: `printf`'s format string is not a
    literal at this shape — `stdio.h` declares it variadic, and a format bound alone with nothing
    after it is what clang refuses (`-Wformat-security`)"*, at the binding's line. `git log -S` of
    that message names `ebed97b04` (2026-09-03, `emit/ffi_call.hero:111-155`). The brief's sentence
    and issue 2465's "What happens" do not reproduce on this Mac for `printf`. **Repair**: rewrite
    Q2's first sentence on what reproduces (items 12 and 13). Correct the compiler-engineer's brief
    too: "refusing a direct binding of a function clang marks `format`" already exists, at the
    binding's probe. "Raising `-Wformat-security` over the program's call" is the base's state as
    well, because the C after the close raises `-Wformat` whole. The open half is inside the region.

12. **Where 2465 does reproduce, three times worse than a wrong number.** Ran:
    - `q2/door.h`, `static inline int say(const char *f) { return printf(f); }`, bound
      `say(f: cstr lent)`;
    - `sqlite3_mprintf(format: cstr lent) -> cstr` over the SDK's `sqlite3.h:3273`
      (`SQLITE_API char *sqlite3_mprintf(const char*,...);`, which has no `format` attribute);
    - `q2/sdoor.h`, `sscanf(s, f)` in a door;
    - each with a format given in `args()`, every run under `timeout 10` with its output cut.

    Printed:
    - the door builds at exit 0, and `n=%ld` prints `n=4305175264` then `n=4366533344` on two runs
      (the shape of the lane's `6099856048`);
    - `n=%s %s %s %s %s %s %s %s` dies at exit 134, *"panic: a null pointer was read through"*;
    - `n=%n` dies of SIGABRT;
    - `sqlite3_mprintf` builds at exit 0 and prints `n=361761723` for `n=%d`;
    - the `sscanf` door with `%d %d` exits 0 and prints `1`, `after`: an `int` written through an
      address nobody passed. Built with `--sanitize`, it prints the same at exit 0 and ASan says
      nothing.

    So the class at the base is (i) a variadic clang does not mark `format`, bound with nothing past
    `...`, and (ii) a door that forwards a format with no data arguments, which passes because the
    region ignores `-Wformat-security` (defect 2036). **Repair**: state those two shapes and their
    outputs as the question. A memory fault and an unseen write are § Precedence's rank 3, and the
    ffi-pragmatist's brief should add the `scanf` family through a door.

13. **The class is not "format functions".** Ran: `q2/openc.hero`, `open(path: cstr lent, flags:
    i32) -> i32` over `fcntl.h:619` (`int open(const char *, int, ...)`), with `O_CREAT + O_WRONLY`
    and no mode. Printed: build exit 0, `3`, and `ls -l made.txt` shows `-r--r-x---@` on two runs:
    a mode read from the `va_list`. **Repair**: Q2 asks whether the class is *every variadic whose
    callee reads past `...` by a run-time value* (`open` with `O_CREAT`, `fcntl`, `ioctl`,
    `execl`'s sentinel, `sqlite3_config`). If it is, a rule keyed to clang's `format` attribute
    misses `open`. Add `open` and `fcntl` to the ffi-pragmatist's list.

14. **design.md states the door half closed, and it is not.** Ran: `sed -n 3928,3945p
    docs/design.md`. Printed: *"`-Wformat-nonliteral` kept an error in every group header's region,
    as `-Wformat` is, so a door forwarding its format is refused"* (`:3944-3945`). Item 12's door
    forwards its format and builds. `-Wformat-nonliteral` fires only when there are data arguments,
    and without them the diagnostic is `-Wformat-security`, which the region ignores
    (`header_region.hero:91-94` says so in its own comment). **Repair**: give the spec-warden
    `:3944-3945` to quote.

15. **The trade with defect 2036 is between two `blocking` classes, and § Precedence decides it; the
    brief cites only Principle 0.** Ran: `gh issue view 2036`. Printed: its class is *"a correct
    program refused"*, and its reason for `printf(m)` is *"nothing makes the second undefined"*. That
    is a premise about what the variable holds, and item 12 falsifies it for a door's parameter.
    Raising `-Wformat-security` in the region refuses `fixedbugs-2036-...-format-held-in-a-variable`
    (a defined program), while leaving it ignored keeps a memory fault at exit 0. CLAUDE.md §
    Precedence (*"robustness decides THAT a measured class of wrong programs closes, Principle 0
    HOW"*, panel 215) ranks the fault above the refusal. Panel 243's spec-warden veto fell on a
    format exception that admits a form, not on a refusal. **Repair**: Q2 says both, so the veto is
    not read as closing the refusal routes.

16. **The lane's numbers are in one file of its scratch: the issue's draft.** Ran: `grep -rl
    '6099856048\|6138276528' .claude/worktrees/b30-scratch`. Printed:
    `drafts/f12-format-at-runtime.draft.log`, the rendering of the issue's own text, which is not a
    measurement. **Repair**: "no file under `b30-scratch/variadic/` holds them".

## D. Q3, defect 2448

17. **Reproduced, and in the reverse direction too, which the brief does not list.** Ran (`q3/`), on
    `b30-scratch/repro/mismatch/main.hero` (whose `.out` files hold only `wrote ...` lines), with
    `heroes-next` and `K=/opt/homebrew/opt/curl` (`-> ../Cellar/curl/8.23.0`). Printed:

    | how the build is run | header | library | linked |
    |---|---|---|---|
    | plain | 526081 | 8.7.1 | `/usr/lib/libcurl.4.dylib` |
    | `CPATH=$K/include` | 530176 | 8.7.1 | `/usr/lib/libcurl.4.dylib` |
    | `C_INCLUDE_PATH=$K/include` | 530176 | 8.7.1 | `/usr/lib/libcurl.4.dylib` |
    | `--include $K/include` | 530176 | 8.7.1 | `/usr/lib/libcurl.4.dylib` |
    | `--include` with `--library $K/lib` | 530176 | 8.23.0 | the keg's |
    | `CPATH` with `LIBRARY_PATH=$K/lib` | 530176 | 8.23.0 | the keg's |
    | `--library $K/lib` alone | 526081 | 8.23.0 | the keg's |
    | `LIBRARY_PATH=$K/lib` alone | 526081 | 8.23.0 | the keg's |

    Every build exits 0, and the last two pair the SDK's header with the keg's library. design.md
    `:3125` (panel 055) names `CPATH`, `LIBRARY_PATH` and `PKG_CONFIG_PATH` as the machine's search
    route, and `--library` is a flag (`heroes build --help`). `grep LIBRARY_PATH selfhost/` finds no
    reader of it. **Repair**: Q3 asks about a pair with one half named, in both directions, and
    names `--library` and `LIBRARY_PATH`. The compiler-engineer's Q3 list should gain both.

18. **The compiler's own advice leads into 2448.** Ran: `q3/locked/l.hero`, `extern
    "/opt/homebrew/opt/readline/include/readline/readline.h" link "readline"`. Printed:
    `machine_locked_path`, telling the author to *"set `CPATH` to the directory holding it, or name
    its library with `pkgconfig`"* (`head_names.hero:222`), and nothing of the library's half.
    Following it (`rl.hero`, `CPATH=/opt/homebrew/opt/readline/include`) builds at exit 0 and prints
    `header 2051`, linked to `/usr/lib/libedit.3.dylib`: GNU readline 8.3's header against libedit,
    243's critic item 4 again. **Repair**: name that note in Q3. Whatever the ruling, its text is in
    scope, since a remedy that produces a `blocking` defect is a false message.

19. **The same keg header is refused in one shape and accepted in the other.** Ran: `q3/vi.h`, a door
    of my own including `<curl/curl.h>`, under `CPATH=$K/include`. Printed: exit 1,
    `shadowed_header`, *"stands in for the C library's `<curl/curl.h>`, which `./vi.h` includes"*.
    The group that names `curl/curl.h` itself builds at exit 0 (item 17). **Repair**: Q3 should say
    the stand-in rule already refuses the door shape, so "refused" is the base's verdict on half of
    the class. Any route makes the two shapes agree or says why they differ.

20. **Found beside, a false message.** Ran: `q3/wf.hero`, binding `curl_multi_waitfds` (in 8.23.0's
    header and absent from 8.7.1's) under `CPATH`. Printed: exit 1, *"error[ffi_missing_link]: the
    linker cannot find `curl_multi_waitfds`, and no group in this program says which library has
    it"*, with a note to write `link "<library>"`. The group says `link "curl"`. I searched the
    issues for that phrase and for `curl_multi_waitfds` and found no filing. **Repair**: name it in
    Q3. It is the mismatch's link-time half, told with a false remedy.

21. **What a build can promise stops at the link.** Ran: `DYLD_LIBRARY_PATH=/usr/lib ./inclib` (the
    build paired by `--include` and `--library`). Printed: `header 530176`, `library
    libcurl/8.7.1`, exit 0. **Question the sitting should ask**: does the rule claim the pair at
    build time only? The loader's variables (`DYLD_LIBRARY_PATH`, and on Linux `LD_LIBRARY_PATH` and
    a soname resolved at run time, unrun here) undo any link-time pairing.

22. **The harm 2448's class rests on is an inference.** Ran: `gh issue view 2448`. Printed: *"so a
    constant, a record's layout or a macro the two releases spell apart is read against code that
    does not hold it"*. That *so* is CLAUDE.md § RUN IT's tell. What I measured is milder:
    - a function only the newer header declares is refused at the link (item 20);
    - `CURLVERSION_NOW` reads `11` under `CPATH` against a library whose newest stamp is the
      header's 10 (`ELEVENTH`), and curl's own `age` field is the library's guard;
    - 8.23.0's `curl_version_info_data` adds `rtmp_version` (`diff` of the two structs), and the
      door that would read it is refused (item 19).

    **Repair**: ask a seat for one measured wrong value or memory fault from a mismatch, a record
    whose layout moved or an enumerator renumbered. Otherwise the record should say the `blocking`
    class rests on robustness by inference.

23. **Routes nobody listed for Q3.** Each can be checked without knowing the library:
    - (P1) **pair by provenance**: a group whose header clang found in a folder the build names,
      and whose `link` library the linker found in no folder the build names, or the reverse, is
      told. The build already has both answers: the include listing (`clang -v -M`, design.md
      §4.19) and the link's trace of which file supplied each name (panel 239's R1,
      `cli/supplied.hero`).
    - (P2) **pair by prefix**: `<p>/include` with `<p>/lib`, after realpath. It holds for Homebrew's
      kegs and the FHS. Nix splits `-dev` from its output, which would refute it (unrun here).
    - (P3) **ask `pkg-config` by the folder**: a `.pc` whose `includedir` is the named folder gives
      the `libdir` the link must take.
    - (P4) **make `machine_locked_path`'s note name both halves**, independent of the ruling.

    What would have to be true for (P1) to hold: no correct program takes its header from a named
    folder and its library from the system's. Windows refutes a `LIBRARY_PATH` half, because
    lld-link ignores it (design.md `:3128-3133`, defect 233). **Repair**: add (P1) to (P4) to the
    compiler-engineer's and the ffi-pragmatist's lists.

## E. Across the sitting

24. **Linux and the box are unrun by every seat, Docker being off.** The briefs say so for the
    ffi-pragmatist only. glibc's door shapes differ: fortify's `__warnattr`, link warnings (defect
    589, closed, panel 208's R3), and `LD_LIBRARY_PATH`. **Repair**: 00-shared says every platform
    sentence is this Mac's, and the Linux and Windows halves are questions.

25. **Found beside, unexamined**: `curl_easy_setopt(handle: ptr, option: u32) -> u32`, and the same
    with `-> i32`, is refused `ffi_return_type`, whose message says the function *"returns `CURLcode
    (CURL *, CURLoption, ...)`"*, a function type rather than a result type. The probe is
    `q2/setopt.hero`. I did not trace it (curl's type-check macros are the first suspect) and filed
    nothing. It is the coordinator's to read.

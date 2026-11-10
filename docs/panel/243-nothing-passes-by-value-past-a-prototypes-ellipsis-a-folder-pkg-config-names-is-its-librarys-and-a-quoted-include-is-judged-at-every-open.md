# 243: nothing passes by value past a prototype's `...`, a folder `pkg-config` names is its library's, and a quoted include is judged at every open

**Status** a full sitting at the C boundary, convened 2026-11-10 by the coordinator of batch 30
without asking again (CLAUDE.md § 4: the author's panel gate for M-core-packages was asked at
panel 242, 07:35 that day). The resolution is `provisional — author ratification pending`.
**Seats** compiler-engineer, ffi-pragmatist, spec-warden, historian, as panel 242 sat by the
author's answer *four seats, no blind seat*; the completeness critic on the briefs before any seat
(22 items, the briefs rewritten on them, the first versions kept as `243-briefs/*.before-critic.md`)
and on the reports after (20 items). No llm-ergonomist: no route adopted moves a token of the
specification (the spec-warden: base 8167 legacy, 8314 cl100k, 10,898 real; every adopted route
+0), so the blind seat's input would not differ. No paid run.
**Base** `89db843472cf1c39450f4a2fc22ff99ff3d716f7` (`main`, batch 29 pushed), its stage 2
compiler printing `sources 14e9e5ce168a2336`; each seat built its own in
`.claude/worktrees/243-<seat>`.
**Briefs** `docs/panel/243-briefs/`, **reports** `docs/panel/243-reports/`, **evidence**
`docs/panel/243-reports/evidence.tar.xz` (its note beside it).

## The proposal

Three questions at the C boundary (`243-briefs/00-shared.md` § The three questions, and § After
the critic's first pass, which added routes (R0) and (E), widened Q1's scope to every by-value
argument, reframed Q2 and named Q3's danger shape):

1. **Q1, defect 2421 (blocking).** A binding's by-value argument that falls past its header
   prototype's `...` is held to no width: `slot_read(count: i32, value: i32) -> i64` over
   `static inline long slot_read(int count, ...)` reading `va_arg(ap, long)` builds at exit 0 and
   prints `4294967295` for `value: -1`. Routes: (R) refuse unless a prototype of the program's own
   fixes the shape, a literal format's conversions excepted; (R0) the same with no format
   exception; (E) the literal format put into the C call for clang's `-Wformat`; (C) the hole
   pinned as a known cost.
2. **Q2, defect 2428 (blocking).** A `path` package whose own `curl_own.h` includes
   `<curl/curl.h>`, built with Homebrew's keg-only libcurl through `PKG_CONFIG_PATH`, is refused
   `shadowed_header`: the folder `pkg-config` names for the library is read as standing in for the
   SDK's header. Routes: (a) group-scoped, a file a program's header includes is that library's
   when it lies in a folder the same group's answer names; (b) a file a group of the build names
   directly is the program's choice wherever included; (c) R2's second arm narrowed for folders
   named for a library; (d) `pkg-config` tying each header to its own `-L`.
3. **Q3, defect 2420's two rows.** A header's quoted include found from the includer's own folder
   that is not a sibling (`"../sys/timeb.h"` from `lib/clock.h`, `"sys/timeb.h"` from the folder
   above) is refused `shadowed_header`, while design.md §4.19 excuses *a header's own sibling
   included in quotes*. The lane's reading: *a file a header includes in quotes, found from that
   header's own folder, is its own*. The danger the rule exists for: a program's file named like
   a system header, read in its place by every `#include <...>` of it.

## The verdicts

| question | compiler-engineer | ffi-pragmatist | spec-warden | historian (advisory) |
|---|---|---|---|---|
| Q1 | **approve (R0)** over every by-value argument, built (48 lines added, 2 changed, four files); object (R), estimated 350 to 500 lines; (C) record only | **approve (R0)** over numbers, no format exception; object to extending it to pointers, to (R)'s exception and to (E); **veto (C)** | **veto** the format exception of (R) and (E) on Principle 0; **approve (R0)** with no § 13 sentence (+0); object (C) | approve the refusal; (R0) has the stronger precedent; object (C) |
| Q2 | **approve (a)**, built (~120 lines); (b) built, refuses a correct program | **approve (c) in its step-3 form**, built; **veto (c) without its C11 and POSIX exception**; object (b); (a) acceptable, unbuilt by it | approve at 0 spec tokens; of the design.md wordings, prefers (a)'s | approve (a), group-scoped |
| Q3 | **approve (L+S)**, built; object (L) alone, which also builds two danger shapes | approve the widening only with `-fshow-skipped-includes` and every open judged; object the widening alone | object to the lane's sentence unless shape `e` keeps building; drafts the sentence covering `d`, `g` and `e` | approve the lane's widening for `d` and `g`; `e` is not that rule; `-iquote` a question |

No veto falls on a route this resolution adopts.

## What each seat measured, in short

- **compiler-engineer** (`243-reports/compiler-engineer.md`): every route built as its own compiler
  from the base and run on the named cases. (R0): 2421 refused at exit 1, the wrapper route builds
  and prints `-1`; the base prints `4294967295` for `%ld` with an `i32` and `42` for `%d` with an
  `i64` of 4294967338, both refused under (R0); of the tree's 7 by-value bindings past `...`, 2 wrong
  programs, 2 right by width, 3 correct pointers. (a) builds 2428 against the keg and the critic's
  sqlite wrapper; `CPATH` and `--include` stay refused. (L+S) refuses the danger shapes `live`,
  `live2`, `dd` and `gg` in both group orders and keeps `d`, `e`, `g` and panel 237's 20 cases
  green; the whole `run` form under it read **750 passed, 0 failed** (two Linux-only SKIPs).
  `selfhost/` binds no variadic (24 extern groups, none with `pkgconfig`).
- **ffi-pragmatist** (`243-reports/ffi-pragmatist.md`): the same C reads `4294967295` at `-O0` and
  `-1` at `-O2` past `...`; against Homebrew's libcurl 8.23.0, `CURLOPT_TIMEOUT_MS` returns 43 for
  `(long)-1` and 0 for `(int32_t)-1`; the door `static inline long slot_read_long(int, long)` prints
  `-1` at both levels and is header-checked. Q2 step 3 builds 2428's reproducer, `curlwrap`,
  `sqlitewrap` and its libcurl doors over the keg, while `pcstdio` (`BUFSIZ`) and `pcsocket`
  (`AF_INET`) stay refused; (c) without the exception printed `BUFSIZ` 7 and `AF_INET` 99 at exit 0.
  Q3: `d` and `g` build under its prototype, `danger-live` and `danger-live2` go from exit 0 to 1,
  and the hole is the first-open rule, not only the missing flag.
- **spec-warden** (`243-reports/spec-warden.md`): every adopted route +0 spec tokens; a § 13
  sentence for Q1 would cost V0 +19 to V2 +47, (R) V3 +52 and (E) V4 +52, both past `DELTA_GATE`,
  and no § 13 sentence becomes redundant under any route. The `shadowed_header` message for shapes
  `d` and `g` names the angle form, *"the C library's `<sys/timeb.h>`"*, for an include written in
  quotes.
- **historian** (`243-reports/historian.md`): Go, Swift and GHC's `ccall` refuse a variadic call
  and send the programmer to a function of fixed shape; literal-format readers in GCC, clang, D
  and Vala are each keyed to a marked function; GCC's quote form searches the current file's
  directory first, clang's the `#includer`'s directory.
- **critic, second pass** (`243-reports/critic-reports.md`): item 3, (a) as built prints `7` and
  `99` at exit 0 on the two shapes the ffi-pragmatist vetoed; item 16, (a) refuses a correct program
  step 3 builds (a wrapper over curl and readline, each with its own `pkg-config` group, printing
  `530176 2051` linked to both kegs); item 5, under the numbers-only scope `printf("%d", <a lease>)`
  prints `n = 42259872` at exit 0; item 2, a door that forwards its format builds and prints
  `n = 42` under (R0), and clang's `-Wformat-nonliteral` flags it; items 4 and 7, three header and
  library mismatches at exit 0 that no route touches (curl's 8.23.0 header against 8.7.1 under
  `CPATH`; readline 8.3's header against libedit under `CPATH`; `--include`); item 1, `twoarity-lent`
  is already refused at the base, so (R0) moves three tracked programs, not four, and turns
  `suite_surface.hero` rows 687 and 688.

## The resolution adopted, provisionally

**R1, Q1 (defect 2421): (R0) over every by-value argument past `...`.** A binding's argument that
falls past its header prototype's `...` and is passed by value (an integer, a `bool`, a float, a
record by value, a `cstr` or a pointer) is refused, with the note naming the door: a `static inline`
function of a new name in a header of the program's own, with a fixed prototype, calling the
variadic through its real prototype, **the literal format inside the door** where the callee is a
format function. An `@` past `...` stays refused as defect 2107 refused it. It lands with:
- `-Wformat-nonliteral` kept an error in the program's own header code, so a door that forwards
  its format is refused by clang's words (critic item 2: measured in C, unbuilt in Heroes; its tree
  cost two tracked C files, both kept building: 2036's `printf(m)` under `-Wformat-security`, 2034's
  `say` exempt by its attribute);
- R10 corrected in the spec-warden's wording (`243-reports/spec-warden.md` § 3.4,
  `docs/design.md:3900-3910`), design.md §4.19's ladder rungs 1 and 4 and design.md:2839 reworded,
  in the landing's own commit;
- `examples/curl`, `tests/golden/run/ffi-variadic-at-a-fixed-arity` and `run/fixedbugs-584`
  rewritten as doors, and `suite_surface.hero` rows 687 and 688 rewritten;
- the compiler's notes that draft a binding past `...` rewritten under it
  (`emit/ffi_call.hero:139,148`, `cli/pragma_ask.hero:298`, `emit/ffi_past_prototype.hero:205,233`);
- panel 217's R3(c), which leaves a lease open past `...`, overturned in that part;
- no § 13 sentence: § 13's sentence that a parameter is declared at the header's own width stays
  the whole rule, since past `...` the header declares none.
`examples/curl` as `v0.6.0` carries it is refused by the compiler that lands this, and the
`compat` suite tells it: a change that refuses a `0.x` program is told, never undone for it
(CLAUDE.md § 2).

**R2, Q2 (defect 2428): the ffi-pragmatist's step 3.** For the second arm of panel 237's R2, a file
in a folder a `pkg-config` answer of the build names is that library's; the second arm still
refuses a file of the name of a C11 or POSIX.1 header there, and the first arm stays whole. It
lands with:
- the exception list sourced from the documents, C11's standard headers and POSIX.1-2024's, in a
  module of its own (the prototype's 56 POSIX names were written from memory and are not landed as
  they stand);
- design.md §4.19's paragraph (`docs/design.md:3156-3170`) amended in its own commit, the
  spec-warden's (c) wording with that exception;
- `shadowed_header`'s false words rewritten: a keg's own header called *"the C library's"*, the
  angle form named for an include written in quotes, and the note that tells the author to move a
  library's file out of Homebrew's Cellar, a remedy nobody can take;
- the harness change that lets a case carry its own `.pc` (`tests/harness/case_setting.hero`
  setting `PKG_CONFIG_PATH` and `PKG_CONFIG_LIBDIR`, the `.pc` naming its folder by
  `${pcfiledir}`), so Q2's cases are tracked, not scratch probes.

**R3, Q3 (defect 2420's two rows): (L+S).** A file under the program's own folder that one of its
headers includes in quotes, found from that header's own folder, is its own, unless a platform or
runtime header includes it as well; the listing is read with `-fshow-skipped-includes`, so an
include a guard skips is seen, and every includer of a file is judged, not only its first open.
design.md §4.19's sentence becomes the spec-warden's draft covering `d`, `g` and `e` with the danger
refused in both orders (`243-reports/spec-warden.md` § 4, Q3), in the landing's own commit.

**R4, what is filed beside it.** Defect 2447 for defect 2420's two rows and the order hole (the
danger shape accepted in one group order, unguarded files included), with the false angle-form
message, `blocking`; defect 2448 for the header and library mismatch at exit 0 under `CPATH` and
`--include`, which no route touches, `blocking`. Both reproduced by the coordinator on 2026-11-10
with `main`'s compiler before filing. The false messages and the false remedy of R2 are 2428's
repair.

**R5, the landing.** R1, R2 and R3 land in batch 30, whose round is open, in two lanes whose files
are disjoint: Q1's (`selfhost/emit/ffi*`, `cli/pragma_ask.hero`, the doors' cases), and Q2's with
Q3's (`selfhost/cli/shadowed*`, `tests/harness/case_setting.hero`), the second closing 2428 and
2447 together. R1 and R2 reach the trunk in
the same push, the spec-warden's condition on (R0). Each lane builds the route from its seat's
saved patch (`p243-scratch/compiler-engineer/`, `p243-scratch/ffi-pragmatist/proto/`) and owes the
compiler's own tests and `layout` on it, unrun by every seat.

## Disagreements, not smoothed

- **Q1's scope.** The ffi-pragmatist approves (R0) over numbers only and objects to pointers,
  which would refuse `examples/curl`: *pointers have one representation on every leg*. The
  compiler-engineer and the critic take the whole scope, and the critic measured why: a `cstr`
  passed past `...` to `printf("%d")` prints `n = 42259872` at exit 0, the callee reading another
  type than the one passed, the class 2421 names. R1 takes the whole scope; **the conservative
  alternative is the numbers-only scope**, which keeps `examples/curl` building as written and
  leaves that wrong value open.
- **Q2's route.** The compiler-engineer and the historian approve (a); the ffi-pragmatist step 3.
  The compiler-engineer wrote that (a) admits no class the direct form does not; the critic ran the
  ffi-pragmatist's vetoed shapes under (a) and read `7` and `99` at exit 0, and found a correct
  program (a) refuses and step 3 builds, which meets the compiler-engineer's own condition for
  moving to (c). The spec-warden prefers (a)'s design.md wording because (c) *also excuses a header
  that a different group's answer names*; that breadth is what builds the critic's item 16, each
  header paired with its own library, every answer's library linked. R2 takes step 3; **the
  conservative alternative is (a) with the same C11 and POSIX exception**, which refuses item 16.
- **Q3's sentence.** The spec-warden objects to the lane's words because, read as a replacement,
  they stop excusing `e`, which builds today through `-I .`; R3's sentence is the spec-warden's own
  draft, which keeps `e`. The historian's MSVC reading of `e` does not fit the unit's layout (critic
  item 13), and how clang in MSVC mode searches includers on the box is unrun.

## Prior art, per resolution

R1 rests on Go's `cgo`, GHC's FFI and ISO C's `va_arg`, reached first; R3 on GCC's quote-form
search and clang's `#includer` search, reached first. R2 rests on none: the historian's Homebrew
precedent was written for route (a), which is not adopted. Swift's page, read by the historian, did
not render to the coordinator's fetch on 2026-11-10 and is not cited. Every other page was re-read
on 2026-11-10, the clang comment by `curl` of the raw file and N1570 at its printed pages 269 and
270. The lines are the decision issue's `## Prior art`.

## Left open, as questions

- `-iquote` for the program's folders (critic item 8): under it the SDK's `<sys/socket.h>` reads
  its own `sys/types.h` in both orders and `d`, `e`, `g` read the program's file; it would stop two
  tracked cases compiling (`run/fixedbugs-563`, `unsupported/fixedbugs-158`) and reverse the search
  order `cli/compiling.hero:95-96` promises. An architecture change, unbuilt; a sitting of its own if
  anyone takes it up.
- Whether a build pairs every header with the library it describes (critic item 17): defect 2448
  is the measured half.
- `-fshow-skipped-includes` on the CI's Linux clangs and the box's clang 23: documented at clang
  18.1.0's reference (critic item 10), unrun there.
- Windows: a door taking `long` bound `i64`, where `long` is 32 bits, and the box's includer search.

## Predictions to score

- **compiler-engineer**: (R0) lands in under 150 lines including its tests; (R) would add over 350
  lines across at least four modules (unbuildable as a check, since (R) is not landed). Checkable at
  batch 30's closing commit.
- **ffi-pragmatist**: (1) under (R0), rung 4 (`examples/curl`) builds unchanged (written for the
  numbers-only scope; under R1's whole scope it is rewritten as a door, so the prediction scores
  the conservative route); (2) rung 1's `printf` regression case becomes a one-line `long long` and
  `%lld` door, building on Linux arm64; (3) `twoarity` given 4294967338 never again prints `n = 42`
  at exit 0; (4) with step 3 landed, a libcurl package builds over the keg with one 16-line header,
  and `pcstdio` and `pcsocket` stay exit 1. Checkable at the landing's gate and the CI's legs after
  the push.
- **spec-warden**: P2, under (R0) gate B turns the tracked programs that pass a number past `...`
  from building to refused (written as four; the critic counted three, `twoarity-lent` being
  refused at the base); P1 and P3 belong to sentences no route adopts and are not scored.

## Process notes

- Four seats by the author's answer at panel 242, which is neither of `/panel`'s two lanes; what it
  gave up is the blind seat's reading of a specification no adopted route moves.
- The critic's first pass changed the briefs in 22 places before any seat started; its second pass
  is where R2's route and R1's `-Wformat-nonliteral` come from.
- Every agent of the session stopped on the account's rate limit at about 20:05 and was resumed on
  another account at 20:14; the compiler-engineer's whole `run` form, started at 19:32, kept running
  and ended at about 20:45 (`real 4396.56`, spent waiting on a machine whose load read 164 at 20:23
  under another session's build; not a duration). The coordinator let it finish as Q3's evidence and
  asked for no other whole form.
- The critic's second pass wrote 471 bytes to `/tmp/claude-fwd.txt` through a `tee`, outside the
  repository's root, and removed it about a minute later; said here as it said it.
- The seats' worktrees hold their prototypes uncommitted: `243-compiler-engineer` with (L+S)
  applied and six compiler binaries, `243-ffi-pragmatist` with step 3's edits to
  `cli/shadowed.hero` and `cli/shadowed_tree.hero`; every patch is in `p243-scratch/`.

## Author's verdict

Pending. The sitting's decision issue, `Panel 243:`, is filed open. **What a yes settles**: R1 to
R5 as adopted, design.md's R10, §4.19 ladder and §4.19 paragraph amended in their own commits, the
two defects of R4 filed, the landing in batch 30. **What it does not**: the predictions above, on
their own clocks; the conservative readings, which stay the author's to choose.

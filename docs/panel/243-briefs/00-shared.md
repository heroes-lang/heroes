# Panel 243, the brief every seat reads first

Convened 2026-11-10 by the coordinator of batch 30, in M-core-packages, without asking again
(CLAUDE.md § 4: asked once per milestone, panels 241 and 242 this milestone). Seats chosen by
input, as panel 242 sat by the author's answer *four seats, no blind seat*: compiler-engineer,
ffi-pragmatist, spec-warden, historian, and the completeness critic before and after. No
llm-ergonomist: its blind session is a paid run, and none is named here (below).

## The base

`main` at `89db84347` (batch 29 pushed, `git log -1 --format=%H origin/main` read
89db843472cf1c39450f4a2fc22ff99ff3d716f7 on 2026-11-10 at 18:4x), its stage 2 compiler printing
`sources 14e9e5ce168a2336` (`heroes-next --version`, built in two stages from the seed at the
batch's gate). Each seat with Bash works in its own detached worktree
`.claude/worktrees/243-<seat>` at that commit and builds its own compiler there:
`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, then
`./heroes build selfhost/main.hero -o heroes-next`. Never the trunk's binary, never another
seat's. Its probes go in `.claude/worktrees/p243-scratch/<seat>/`. No file is written outside
the repository's root (`/Users/joseph/Temp/heroes/heroes-lang`).

## The three questions, all at the C boundary

**Q1, defect 2421 (blocking)** (`gh issue view 2421 --repo heroes-lang/heroes`). A binding's
by-value integer parameter that falls past its header prototype's `...` is held to no width: a
header declaring `static inline long slot_read(int count, ...)` that reads `va_arg(ap, long)`,
bound `function slot_read(count: i32, value: i32) -> i64` and called with `value: -1`, builds
at exit 0 and prints `4294967295` (reproducers `.claude/worktrees/p242-scratch/coord-vararg/` and
`critic/vararg/`, read only). The compiler cannot see the callee's `va_arg`. Lane b29-boundary
measured and stopped on it; its analysis is
`.claude/worktrees/b29-scratch/boundary/report-final-agent.md` § 2421 (read it): two routes, its
recommendation first:
- **(R) robust**: past `...`, an integer argument is held to what C reads only by a prototype (a
  header of the program's own declaring the variadic at a fixed shape); otherwise refused, with
  the note defect 2107's refusal of an `@` past `...` gives naming that route; the exception a
  format function called with a literal format, whose conversions the compiler reads;
- **(C) conservative**: design.md's sentence that `i32` spells a C `long` (panel 234's R10,
  `docs/design.md:3903` by `grep -n 'i32. for a C .long' docs/design.md`, in the paragraph 3901-3906) said to hold
  through a prototype only, the hole pinned as a known cost.
The lane reports rejecting a refusal of every narrow integer past `...` (it refuses `open`'s
mode, `mode_t` two bytes on this Mac, and `printf("%d")`) and a widening of every argument to 64
bits (undefined behaviour for `%d`). Where to look: `selfhost/emit/ffi_past_prototype.hero`,
`selfhost/emit/ffi_call.hero` (its format-string rule, line 144's message), `ffi_build.hero`;
defects 2106 and 2107 (closed); panels 052 and 094 (`docs/panel/052-*.md`, `094-*.md`) on how a
variadic is bound today; spec § 13 (`spec/heroes-spec.md:373` on), whose line 379-382 says a
parameter is declared at the header's own width and sign, except one C converts exactly.

**Q2, defect 2428 (blocking)**. A `path` package whose own `curl_own.h` holds `#include
<curl/curl.h>`, built with `PKG_CONFIG_PATH=/opt/homebrew/opt/curl/lib/pkgconfig` (Homebrew's
keg-only libcurl 8.23.0), is refused `shadowed_header`: the folder `pkg-config` names for the
library the build asks is read as standing in for the SDK's `<curl/curl.h>`. Without the
variable, against the system's libcurl 8.7.1, it builds and runs. The coordinator's reproducer
is `.claude/worktrees/b29-scratch/coord-curlwrap/undef/` (read only). The rule is panel 237's
(`docs/panel/237-a-file-standing-in-*.md`, its R1 and R2) and design.md §4.19's paragraph
`docs/design.md:3156-3170`. The question: when is a folder `pkg-config` names, for the library a
group names, that library's own, and what does a build that wants Homebrew's curl over the SDK's
write, if anything.

**Q3, defect 2420's two rows left**. A header's quoted include found from the includer's own
folder that is not a sibling, `"../sys/timeb.h"` from `lib/clock.h`, and `"sys/timeb.h"` from the
folder above, is refused `shadowed_header` (lane b29-boundary's shapes,
`.claude/worktrees/b29-scratch/boundary/2420/shapes/d/` and its `*.after.txt`, read only). design.md
§4.19 excuses *a header's own sibling included in quotes* (`docs/design.md:3162-3163`). The
lane's recommendation: *a file a header includes in quotes, found from that header's own folder,
is its own*. The danger the rule exists for is real: a program's file named like a system
header, in a folder the build searches, is read in its place by every `#include <...>` of it.

## How the sitting runs

- Every number, path and count you write is produced by a command you run while writing it, and
  you say which; a negative sentence (*X does not exist*) is run or written as a question.
- One heavy command at a time in your worktree (a stage 2 build, a suite); never two in parallel.
  The author's Mac ran out of memory today with five lanes at once.
- A run that may not end is a built binary under `timeout`, its output bounded.
- No paid run: no `claude -p`, no API call, no `measure --refresh`. No credential printed. No
  GitHub write. No push. No edit of the tree outside your own worktree and your report.
- Write your report to `docs/panel/243-reports/<seat>.md` in the worktree
  `.claude/worktrees/p243` as you go. If the tool refuses that write, your final message is the
  report, whole.
- A route you recommend is one you built and ran, in your worktree, on the cases you name.

## After the critic's first pass (2026-11-10, `243-reports/critic-briefs.md`, 22 items)

Read the critic's report whole; it overrides what is written above where they differ. In short:

- **This milestone's sittings** are panels 234, 241 and 242; the gate was asked at panel 242
  (item 2).
- **R10's paragraph** is `docs/design.md:3900-3910`, Part 7 row 10, *Sized integers*; issue
  2421 cites 3890-3894, the older falsifier. R10 rests on *a parameter has a spelling every leg
  shares*, and past `...` a C `long` has none: `i32` is wrong on this Mac and Linux, `i64`
  undefined on Windows, where `sizeof(long)` is 4 (panel 242's ffi-pragmatist § 1.4) (item 3).
- **Route (R)'s wrapper** is a `static inline` function of a new name in a header of the
  program's own, with a fixed prototype, calling the variadic through its real prototype (defect
  2107's note, `ffi_past_prototype.hero:205,233`); **never a re-declaration of the variadic**,
  which panel 052 refused by the ffi-pragmatist's standing veto (wrong on arm64) (item 4).
- **No conversion reader exists.** The compiler reads no `printf` format; its one format rule
  reads clang's `-Wformat-security` words (`ffi_call.hero:62,111`), and the format reaches C as a
  run-time pointer, never a literal. Route (R)'s exception is new code (item 5).
- **The tree binds no `open`** (`git grep` of `function open(`): write it as a case (item 6).
- **Defect 2359's cases** are the compiler's own tests in `selfhost/cli/shadowed*.hero`, no golden
  (item 7). **Panel 237's 20 `.case` directories** holding `shadowed_header` reach no `.pc`; a case's
  `environment` may set only `PATH`, `GIT_CEILING_DIRECTORIES`, `CPATH`, `C_INCLUDE_PATH`,
  `CCC_OVERRIDE_OPTIONS` (`tests/harness/case_setting.hero:55-61`) (item 8).
- **Q3's shapes**, by their files (item 9): `d` is `lib/clock.h` including `"../sys/timeb.h"`;
  `g` is `clock.h` at the root including `"sys/timeb.h"` from its own subfolder; `e`
  (`sys/clock.h` including `"sys/timeb.h"`, found through `-I .`, not from the includer's folder)
  **builds today**. Each route says how it reads every shape. Defect 2420 is closed and no open
  issue holds these rows; the synthesis files them (item 10).
- **More to read for Q1** (item 11): panel 217's R3(c), on which `ffi_past_prototype.hero` rests
  (its doc still says *provisional*; ratified 2026-10-17, issue 2105), and the asks it drops in
  `cli/header_types.hero`'s `answered` and `emit/ffi_read_only.hero`'s `lend_answers`. **Docker is
  off**: do not start it; panel 242's ffi-pragmatist § 1.4 holds the Linux arm64 and Windows box
  measurements.

**Routes added** (items 12-16):
- **Q1 (R0)**: no format exception at all, symmetric with 2107, which refused `sscanf("%d", @n)`
  even with a literal format. The tracked by-value integers past `...` are four `i64` bindings:
  `run/ffi-variadic-at-a-fixed-arity`, `run/fixedbugs-584` (`sprintf`),
  `surface-fixtures/twoarity/as_int` and `twoarity-lent/as_int`; the tree's own variadics pass only
  `cstr`, `ptr`, a record or `@`; `packages/` binds no `extern`.
- **Q1 (E)**: put the literal format into the C call, so the `-Wformat` the build already keeps an
  error in header code (`header_region.hero:100`, defect 2034's case) judges it; its cost, measured by
  the lane: it compares types, not widths.
- **Q1's scope is every by-value argument past `...`**, not only a narrow integer: `twoarity`
  built with `value: 4294967338` prints `n = 42` at exit 0; an `f64` or a pointer read as `long` is
  the same class.
- **Q2 reframed**: *a library header in a named folder, included by a header of the program's
  own*. `CPATH`, `--include` and `pkg-config` all refuse 2428's wrapper; a sqlite wrapper over its
  keg is refused too; a group naming `<curl/curl.h>` directly with the keg's `.pc` builds and prints
  `libcurl/8.23.0`; seven keg-only formulae collide with the SDK here (curl, ncurses, readline,
  sqlite, icu4c@78, icu4c@79, llvm@22). Every wrapper the compiler's own notes draft is refused for
  such a library. Routes: (a) group-scoped, a file the program's header includes is that library's
  when it lies in a folder the same group's answer names; (b) consistency, a file a group of the
  same build names directly is the program's choice wherever included; (c) narrow R2's second arm
  for folders named for a library; (d) per arm, `pkg-config` tying the header to its own `-L`.
- **Q3's danger side**: a quoted sibling `sys/types.h` (guarded, `#include_next`) with a second
  group `<sys/socket.h>` builds in one order and is refused in the other; the rule judges a file by
  its first open in `-H`; `clang -H -fshow-skipped-includes` lists an include skipped under a guard.
  The lane's shapes use `sys/timeb.h`, which the platform never reads, so they cannot test the
  danger. Judge every Q3 route on this shape.

**Questions added** (items 17-22):
- Q1 and Q2 are one program: for each Q1 route, what does a libcurl package write
  (`curl_easy_setopt` with a `long` option, `curl_easy_getinfo(@out)`), and does it build over the
  keg?
- Is route (C) admissible at all, a wrong value at exit 0 pinned, when a `blocking` defect is never
  deferred and panel 242's R5 already refused it by the ffi-pragmatist's veto? Or is it only the
  record of what the conservative route would have been?
- Which yardstick does a reader apply: the C standard's types (C11 §7.21.6.1p9, §7.16.1.1p2) or
  the widths the machine reads?
- Windows: a reader is per platform, and its note says so; R10 needs its exception written.
- If a route needs a § 13 sentence, the synthesis queues the blind seat's paid run for the
  author's yes rather than dropping it.
- The compiler's own notes that draft a binding past `...` (`ffi_call.hero:139,148`,
  `pragma_ask.hero:298`, `ffi_past_prototype.hero:205,233`) are rewritten under the chosen route.

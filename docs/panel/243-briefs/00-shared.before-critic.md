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

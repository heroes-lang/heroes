# Panel 097 — the platform arm belongs to the runtime, not to `selfhost/`

**Lane: SOUNDNESS** (`compiler-engineer` + `ffi-pragmatist`). No surface, no
diagnostic, no spec token is at stake: the question is where the compiler's
filesystem calls live. What the lane gives up is the llm-ergonomist's blind read
and the historian's precedent search; neither has a reader-facing half to be
differentiated about here, and the two judges who compile are the two the
question needs.

Convened 2026-08-30, after a `workflow_dispatch` on `main` measured all three CI
legs for the first time since the `m-separate-compilation` tag.

## The proposal, verbatim as briefed

> The compiler's boundary with the operating system is POSIX-only, and Windows
> dies at the first command. Replace the FILESYSTEM operations with FFI calls
> instead of shell commands — `remove()` and `rename()` are C standard in
> stdio.h, directory creation is `mkdir` under sys/stat.h on POSIX and `_mkdir`
> under direct.h on Windows — leaving the shell only for invoking clang.

Five operations, twelve of twenty `cli_shell.shell()` sites:
`mkdir -p` (`cli_toolchain.hero:113`, `cli_deps.hero:274/304/319`), `mv -f`
(`cli_toolchain.hero:206/257/282`, `cli_units.hero:155/193`), `rm -f`
(`cli_doctor.hero:109`), `test -e` (`cli_toolchain.hero:107`), `test -d`
(`cli_toolchain.hero:110`).

## What provoked it, measured

| leg | result | cause |
|---|---|---|
| Linux x86-64 | **green** | the `link "m"` failure of the tag was repaired by `3acac60` on 2026-08-26 |
| Darwin arm64 | 1 of 90 | `date +%s%N` in `suite_determinism.hero:174` — unrelated to this sitting, repaired separately |
| Windows x86-64 | dies at `heroes doctor` | `mkdir -p build` reaches `cmd.exe`, which creates a directory named `-p` |

## Verdicts

| judge | verdict | section | measured cost | prediction | condition |
|---|---|---|---|---|---|
| compiler-engineer | **VETO** of the proposal as written; approves the redirected form | design.md §1.11, §1.12; spec:204-206, 222-229 | 12 lines deleted; `runtime/parts/os.c` 245 → past §11's ~300, forcing a split | ship it as written and the Windows leg still fails, naming `cc`/`clang` not found rather than a directory called `-p` | four, below |
| ffi-pragmatist | **accept-with-condition** (no ABI break, so no veto) | spec:204-207, 222; DESIGN-LOG:282; CLAUDE.md §12 | ABI 16 → 17; 47 lines in `os.c` shape, compiled on three targets | any `record … tag stat` naming `st_mode` at any width is `ffi_field_type` on **exactly one** of {Darwin, Linux} — never zero, never both | three, below |

## The veto ground: the proposal does not compile

Both judges reached this independently, from different directions, and both
compiled rather than reasoned.

**`direct.h` cannot be named from `selfhost/`.** The engineer wrote the group and
ran it: `error[ffi_missing_header]: 'direct.h' is not on this machine's include
path`. Heroes has no `#if`, by design — `selfhost/cli_io.hero:22-27` states it in
the module's own words: *"The route out is not a platform arm — this language has
no `#if`, by design."* The mkdir half of the proposal has no expressible form.

**`struct stat` is unwritable portably.** The pragmatist cross-compiled against
real glibc and real mingw-w64 headers with `zig cc`: `mode_t` is `unsigned short`
(2 bytes) on Darwin and `unsigned int` (4 bytes) on glibc; `st_mode` follows. A
`record FileStat tag stat` naming `st_mode` compiles on Darwin and fails
cross-compiled to Linux with the FFI's own static assert, and vice versa. A
field-less record is `error[empty_record]`, so `stat` cannot be bound at all
without naming one. **spec:222's exactness is what refuses it**, and neither judge
proposed weakening that.

**The remaining route reopens a closed defect.** `access()` lives in `unistd.h` —
the header removed on 2026-08-24 precisely because MSVC does not ship it, which is
why `runtime/hero_os.h` exists at all. `grep -rn '^extern ' selfhost/*.hero`
returns **three groups: `hero_os.h`, `stdlib.h`, `stdlib.h`. Zero POSIX.** Putting
`sys/stat.h` back into `seed/heroes.c` reverses that decision for the reason it
was taken.

**And it would not have fixed Windows.** Verified by the coordinator against the
source: `heroes doctor` also runs `capture(command: "uname -s")`
(`cli_doctor.hero:76`) and redirects `2>/dev/null` (`:29`), neither of which is
among the twelve. The engineer measured the deeper half: `cli_shell.sq()` emits
POSIX single quotes, is used **47 times outside its own module across 7 modules**,
and every clang line carries it — executed as `cmd.exe` would, clang answers
`no such file or directory: ''/tmp/p097/t.c''`. The shell the proposal keeps is
the one that breaks first.

## What the shell was silently providing

Measured by both judges, agreeing:

| shell | C behaviour, measured | what the replacement owes |
|---|---|---|
| `mkdir -p` on an existing dir | `mkdir` = -1, errno 17 EEXIST | treat EEXIST as success — but see the trap |
| `mkdir -p` on an existing **file** | -1, errno **17 EEXIST**, the same code | **errno cannot tell a file from a directory**; the retry must ask `is_directory` |
| `mkdir -p a/b/c` | -1, errno 2 ENOENT | a path-walking loop |
| `rm -f` on an absent file | `remove` = -1, errno 2 | treat ENOENT as success |
| `mv -f` over a target | `rename` = 0, atomic on POSIX | Windows `rename` fails with the target present (C11 7.21.4.2, implementation-defined) |

That last row is §1.12's. `cli_toolchain.hero:194-207` publishes a cache artifact
by atomic `mv -f` *after* recording its dependencies — *"an object on disk is
never one this cache cannot justify serving."* `remove()`+`rename()` opens a
window where the object is absent.

## The disagreement, recorded rather than smoothed

The two judges differ on `remove`/`rename`. The pragmatist compiled them as
`extern "stdio.h"` on all three targets — C89, clean, and would leave them as
externs. The engineer requires the Windows publish to be atomic
(`MoveFileEx(..., MOVEFILE_REPLACE_EXISTING)`), which no `stdio.h` extern can
express. **Both are right about what they measured**, and the resolution is that
`remove` may stay an extern while `rename` cannot, because only `rename` carries
the atomicity the cache depends on.

They also differ on the file: the engineer says `runtime/parts/fs.c`, because
`os.c` is 245 lines and five implementations cross §11's ~300; the pragmatist
prototyped in `os.c` shape. The engineer's split is the one that survives §11.

## Resolution — provisional, author ratification pending

**The most conservative resolution, adopted:** the proposal as briefed does not
land. The platform arm goes where `runtime/parts/os.c:21-34` already puts one for
`_setmode` — behind `hero_os.h`, in the runtime, in C, where `#if defined(_WIN32)`
is a sentence the language never has to say. `selfhost/` sees one wrapper module
of `cli_shell.hero`'s shape (56 lines) and no platform word. This is DESIGN-LOG:282
applied unchanged: *"049 vetoed a platform axis in the language; the same problem
is answered here in the driver."*

Conditions, merged from both seats. All seven must hold:

1. **`mkdir`, `test -e`, `test -d` go through `hero_os.h`**, never as
   `extern "sys/stat.h"` or `"direct.h"` in `selfhost/`. ABI 16 → 17.
2. **`remove` may stay `extern "stdio.h"`; `rename` may not** — the publish is
   atomic or the cache's guarantee is void.
3. **The Windows non-atomic replace, if it is ever taken, is written down as a
   loss in the code and marked NOT VERIFIED** until a tag run reads it.
4. **`mkdir_one` asks `is_directory` after a failure**, never trusts EEXIST: a
   file sitting at `build/` must fail.
5. **Nothing reaches `selfhost/library_source.hero`** — the prelude is the
   standard library design.md §1.11:443-445 forbids permanently.
6. **The platform arm lands in `runtime/parts/fs.c`**, not in `os.c`, which is
   245 lines today.
7. **`sq()` and `cli_toolchain.hero:129`'s `cat … 2>/dev/null` are scheduled in
   the same milestone, or that milestone does not claim Windows.** A green
   `doctor` with a red `build` is worse than a red `doctor`: it moves the failure
   out of the step that exists to detect it and into the cache key CLAUDE.md §7
   says protects the build against a decoy runtime.

**What a veto would compel if the author overrules:** the engineer withdraws only
on all four of its conditions, and its falsifier is stated — *"a run on a Windows
machine showing `heroes build` green with `sq()` unchanged. I would take that
measurement over this whole analysis."*

## Predictions to score

| judge | prediction | checkable at |
|---|---|---|
| compiler-engineer | the proposal as written leaves the Windows leg red, failing at `cc`/`clang` not found rather than at a directory named `-p` | the first tag run after any such change |
| compiler-engineer | `runtime/parts/os.c` crosses 300 lines if the five operations land there (245 today) | the milestone that lands them |
| ffi-pragmatist | `extern "sys/stat.h"` in `selfhost/` puts `#include <sys/stat.h>` into `seed/heroes.c` and kills the Windows leg at the 3.5 s seed build | the first tag run after any such change |
| ffi-pragmatist | any `record … tag stat` naming `st_mode` at any width is `ffi_field_type` on exactly one of {Darwin, glibc} — never zero, never both | today, no Windows needed |

## Author's verdict

**Ratified 2026-08-30** (author instruction, *"ratifica tutto, tutti i panel, fai
adesso il milestone di Windows"*). The sitting's resolution stands as written, all
seven conditions binding, and the wider question it flagged is answered too: the
work is taken **now**, and at the size panel 098 measured rather than at the size
this sitting was briefed on. The milestone is **M-argv-execution**.

**What the yes settles**: the briefed proposal does not land in any form —
`extern "sys/stat.h"` and `extern "direct.h"` never appear in `selfhost/`, and
`seed/heroes.c` gains no POSIX header. The five filesystem operations, if they
are taken at all, go behind `hero_os.h` in `runtime/parts/fs.c` under the seven
conditions above, with `HERO_RUNTIME_ABI` 16 → 17.

**What a yes does NOT settle, and it is the sitting's own open question**:
whether the work is taken *at all*, and at which milestone. Condition 7 makes
that unavoidable — the twelve filesystem calls are not what keeps Windows red on
their own, and a milestone that repairs only them ships a green `doctor` over a
red `build`. So the real question the author is being asked is wider than the one
briefed: does the compiler stop speaking to the operating system through a shell
altogether, `sq()` and the wait-status decode and the cache-key glob included?
This sitting did not rule on that and its judges were not briefed to.

**What a no compels**: the engineer withdraws its veto only on all four of its
conditions, and it named the measurement that would move it — *"a run on a
Windows machine showing `heroes build` green with `sq()` unchanged. I would take
that measurement over this whole analysis."*

## Two findings that outlive the decision

**The thesis worked, on a machine nobody has.** Cross-compiling the all-extern
version to mingw produced exactly one error class, on the `.hero` line:
`too many arguments to function call, expected 1, have 2 — 'mkdir' declared here
io.h:297`. The FFI's header check caught a platform mismatch that no Heroes
program could have expressed, and said so where the author writes.

**Three of the eight kept shell calls fail silently on Windows**, and one of them
is load-bearing: `cli_toolchain.hero:129-135` builds the runtime cache key with
`cat 'dir'/*.c … > f 2>/dev/null`, discards the status with `_ =`, and returns
`""` on failure. The key becomes `digest("")`. CLAUDE.md §7 names that key as the
one thing that catches a decoy runtime, since the `HERO_RUNTIME_ABI` stamp cannot.
A Windows port that stops at the twelve ships that protection void.

## Predictions, scored at M-argv-execution close (2026-08-31)

Measured in the session that writes this, per CLAUDE.md §1.

| judge | prediction | outcome |
|---|---|---|
| compiler-engineer | `runtime/parts/os.c` crosses 300 lines if the five operations land there (245 today) | **confirmed by avoidance.** `os.c` is still **245** lines, unchanged, because this sitting's file rule sent the arm elsewhere: `runtime/parts/fs.c` 160, `runtime/parts/run.c` 367, `runtime/parts/dir.c` 227. The prediction is what made the file rule a condition, so it earns the seat its point — the number it named was never allowed to happen. |
| ffi-pragmatist | `extern "sys/stat.h"` in `selfhost/` puts `#include <sys/stat.h>` into `seed/heroes.c` and kills the Windows leg at the seed build | **honoured by construction.** Condition 9 held: `grep` over `selfhost/` finds `sys/stat.h` only inside test fixtures written as string literals in `emit_ffi_tag.hero` and `emit_ffi_declared.hero` — test *inputs*, not declarations the compiler makes. `seed/heroes.c` gains no POSIX header, and the Windows seed build passes in every run of this milestone. |
| ffi-pragmatist | any `record … tag stat` naming `st_mode` at any width is `ffi_field_type` on exactly one of {Darwin, glibc} — never zero, never both | **confirmed at the sitting**, and it needed no milestone: `st_mode` is 2 bytes on Darwin against 4 on glibc, so one platform refuses whatever width is written. |
| compiler-engineer | the proposal as written leaves the Windows leg red, failing at `cc`/`clang` not found rather than at a directory named `-p` | **lapsed, not renewed** (panel 046 R2). It was made about *the briefed proposal* — the narrow FFI-only route — and that route was refused by this sitting's own verdict, so there is no run to score it against. What it was pointing at was answered anyway: `CreateProcess` finds `clang.exe` only when `lpApplicationName` is `NULL` and the documented PATH search runs, which is M-argv-execution step 3 and exists because of this line. |

## The one thing this sitting could not have known

Condition 7 — *"the harness ships in the same milestone or the milestone does not
claim Windows"* — was written about `sq()` and the wait-status decode. It bought
something else: `tests/harness/` came into scope, and with it
`heroes test tests/harness/main.hero`, a suite that had never been run in this
project's CI or in any session of this milestone. It was **83 of 90** while the
other two suites were green, and it held the leak this milestone's own leak gate
existed to catch (step 9, DESIGN-LOG 2026-08-31). A condition written to make a
port complete found a defect instead.

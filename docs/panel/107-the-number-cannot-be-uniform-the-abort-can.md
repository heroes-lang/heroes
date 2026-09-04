# 107 — The number cannot be uniform; the abort can

Date: 2026-09-04 · **full panel, five judges** · convened by author instruction
(*"option (a), with a panel"*) on the item `/decide` had put the same day.

Status: **provisional — author ratification pending.**

Lane: **full**, not the soundness lane, and the choice paid for itself twice.
The two seats that do not compile produced the sitting's two decisive
non-mechanical findings: the historian's *no language specification states a
number*, and the ergonomist's *depth is the only one of four guessing points
that fails silently*. A soundness lane would have lost both, and the resolution
below rests on them.

## The proposal, as put

> `selfhost/cli/flags.hero::link_flags` passes the same stack size on every
> platform — `-Wl,/STACK:67108864` on Windows as today, and the POSIX
> equivalent (`-Wl,-stack_size,0x4000000` on Darwin,
> `-Wl,-z,stacksize=0x4000000` on Linux) where it passes nothing.

## Why it was asked

`link_flags` gives Windows 64 MB and gives macOS and Linux whatever the
operating system hands them, which on the author's Mac is 8 MB (`ulimit -s`
8176). The same emitted C of `examples/interpreter/`, linked twice and
binary-searched by the coordinator, reaches **314** nesting levels at `-O0` and
**166** under `--sanitize` with the default, and **2,537** and **1,347** with 64
MB. Peak RSS for a program that does not exhaust is unchanged (1,671,168 bytes
large against 1,687,552 small) and the binaries are the same size.

Two facts about the record were established before the briefs went out, and one
of them corrected the question itself. The 64 MB is **not** a panel ruling:
`docs/work/DONE.md:961` is a `/decide` ratification of 2026-09-03, from
M-argv-execution steps 13 and 17, whose stated reason was that Windows' 1 MB
default **kills the compiler on its own modules**. So it is a repair for one
platform, not an answer to what a program should get, and **nobody had ever
decided this question on any platform**. And design.md §1.12 (`design.md:539`)
makes not crashing a goal of the language, so a promise that differs eightfold
by platform is a §1.12 question rather than a tuning preference.

M-corpus-depth's plan asked for a 500-deep expression and closed at 83 with the
measured reason (`docs/journal/032-corpus-depth.md`); panel 106 had already
halved the frames (`docs/measurements/015`). This is what was left.

## The verdicts

| seat | verdict | section | cost / delta | condition to flip |
|---|---|---|---|---|
| compiler-engineer | **veto** | design.md §1.12 `:539-575`, its own Principle 0 clause `:560-563`; and *no section of design.md states what stack a program gets* | +6 lines in `link_flags`, zero in lexer/checker/emitter; a **second** platform axis in `hero_os.h` where there is exactly one today; `heroes doctor` owes a row | a Linux measurement showing the flag moves a running program's depth; **or** narrow to Darwin+Windows as a driver default with the number argued from frames (16 MB, not 64); **or** the pthread mechanism it built |
| ffi-pragmatist | **object**, with a **veto scoped to the Linux spelling** | design.md §1.11 `:435`, §4.19 `:1983`, §1.12 `:539` | `HERO_RUNTIME_ABI` **does not move** — the stamp covers declaration shape and a link flag is not a declaration; emitted C byte-identical; `sqlite`/`ledger`/`curl`/`raylib` all exit 0 under the Darwin flag, `--sanitize` included | drop the Linux spelling entirely or replace it with `setrlimit` + re-exec; **and** install the guard's bounds per thread; **and** state in one line that a C library's own thread is unaffected |
| spec-warden | **approve** (say nothing) · **object** (say something) · **veto** (state a number) | design.md §1.6 `:272-275` hard 4096, `:284-295` payment standard, §1.2 `:190-204`, §1.4 `:223-235` | spec **3750** today (re-measured by the coordinator: `claude-legacy` 3669, `cl100k_base` 3750, spread 81, headroom **346**); the vague sentence **+23**, the number **+18**; no option breaches the ceiling | the vague sentence flips to approve if reworded to what measurement supports **and** paid with a named removal ≥ +23 or an instrument showing fewer first-try failures; the number flips to *object* only if a compiler-enforced minimum exists with a 1,000-frame golden green in all three configurations on all three platforms |
| llm-ergonomist | **approve** the vague sentence · object to the number · the status quo is still wrong | spec only, as the role requires | — | — |
| historian | **approve (advisory)**, with a factual correction to the proposal's own premise | — | — | — |

### What the historian found, and it reframed the sitting

*"The same 64 MB on all three platforms"* is **three flags with three fates**.
Windows' works. Darwin's `-stack_size` works, and **64 MB is exactly Apple's
documented ceiling** — there is no margin above it. **Linux has no link-time
equivalent for the main thread at all.** The seat flagged that UNVERIFIED and
asked the panel to measure it rather than asserting it, which is the shape
CLAUDE.md §1 requires of a negative claim.

On precedent: Go picks 1 GB, GHC 80% of RAM, rustc **17 MB on a spawned thread**
(so rustc does not compile on the main thread at all, and its own comment says
the reason is control over the stack size), Java 1–2 MB. **No language
specification states a number.** The JVMS explicitly refuses a size, Scheme
states a *property*, and Python — the one that does state a number — found in
3.12 that its number described the wrong stack and broke working code.

### What the ergonomist found, reading only the spec

Three programs written against `spec/heroes-spec.md` alone. Of **four guessing
points**, three fail loudly and **depth is the only silent one**. Under a
spec that states a number, a reader sizes a guard from *"1,000 calls"* while
their own parser burns ≥2 frames per level, **so the guard never fires**. It
declined its veto explicitly: *"my veto covers non-local meanings, not non-local
resources."*

### What the warden compiled

A **legal Heroes program** — two by-value record locals, frame **263,488
bytes** — runs **248** deep at 64 MB and aborts at **300**: `panic: stack
exhausted in deep.down`, exit 134. So a spec sentence promising 1,000 calls
would be **false on the day it landed**, and since CLAUDE.md §12 makes the spec
beat the compiler, every such abort would become an unfixable compiler bug. The
compiler's own worst frame is **65,936 bytes** (`h_cliproduce_produce`,
`selfhost/cli/produce.hero:52`) — 1,016 frames at 64 MB, **1.6% from breaching
its own spec**. It searched for a removal to pay with and found none endorsable:
both candidates it measured (−11 and −43) are §1.4 redundancy at an error site.

### What the compiler seat measured

**The Linux arm does not exist.** GNU ld: `warning: -z stacksize=0x4000000
ignored`. lld: `unknown -z value`. `readelf -l` shows `PT_GNU_STACK` `p_memsz`
**0x0 in both** binaries; both segfault identically at 100,000 frames. And
`selfhost/cli/toolchain.hero:106-109` echoes a **successful** clang's stderr, and
`selfhost/cli/units.hero:167-169` re-links on every build — so that warning would print on
every Linux build of every program, forever, for zero bytes. That is
`flags.hero:99-111`'s `/INCREMENTAL:NO` defect rebuilt deliberately.

**The number is Windows' repair, not a measurement.** `./heroes check
selfhost/main.hero` (178 modules) is exit 134 at `ulimit -s` 512, 640 and 768
KB and **exit 0 at 896 KB**; the whole emission is exit 0 at 1024 KB. The
deepest Heroes program that exists needs **under 1 MB**. So
`needed_for_self_hosting` is **no**, measured, and Principle 0 has no ticket to
sell here.

**Where the depth actually goes.** The interpreter's emitted C disassembled:
`h_runexpr_joined` **21,888 bytes** at `-O0` (11,600 at `-O2`), declaring **344
locals** in its prologue; `h_runexpr_combined` 17,264; `h_synparse_statement`
10,448. That is ~26 KB per nesting level, and 314 × 26 KB is the whole 8 MB.
**The eightfold asymmetry is the emitter's temporaries, not the operating
system's stinginess** — panel 106 halved frames once and bought the same factor
of two that 8 MB → 16 MB buys.

**And the reserve is not free where it matters.** For a program that does not
exhaust, the coordinator's RSS claim is **verified** (2,768,896 against
2,752,512; binary 525,312 both). For a program that runs away, max RSS goes
**9,814,016 → 68,534,272**. So `flags.hero:74-78`'s comment — *"the reserve is
address space … so a large one costs nothing real"* — is **false exactly in the
case the flag exists for**, and it is CLAUDE.md §11's expiring premise: the
argument still reads as valid while the premise has died.

Three things nobody had tried: the flag **overrides the operator**
(`pthread_get_stacksize_np` reports 67,108,864 under `ulimit -s 1024`); Darwin
grants it **above the hard rlimit** (67,108,864 > 67,092,480); and `ulimit -s
unlimited` on Linux is **less, not more** — exit 139 at 100,000 frames where
`ulimit -s 65536` is exit 0.

**One clean win, and it is real:** the guard survives the flag. `panic: stack
exhausted in deep.down` is correct at the default, 8, 64 and 512 MB on Darwin
and at every `ulimit` on Linux, because `hero_stack_bounds` reads the size at
runtime. No regression there.

### What the FFI seat compiled

`probe.c`, `sdl_thread.c`, `raylib_thread.c`, `overflow_thread.c` — the last one
linking the real `runtime/runtime.c` and calling `hero_args_set` as the
generated `main` does.

| | plain | with `-Wl,-stack_size,0x4000000` |
|---|---|---|
| Darwin main thread | 8,372,224 | **67,108,864** |
| default `pthread` attr | 524,288 | **524,288** |
| SDL2 audio thread | 536,576 | **536,576** |
| raylib miniaudio thread | 536,576 | **536,576** |
| `RLIMIT_STACK.rlim_cur` | 8,372,224 | 8,372,224 |

**The boundary is the hole, and the flag widens it.** Overflow on the main
thread is `panic: stack exhausted in deep.down`, **exit 134**, identical in both
builds. Overflow inside SDL's audio callback is **nothing on stderr, exit 132**,
identical in both builds — `runtime/parts/stack.c:55-59`'s recorded hole,
unmoved. So the gap a program can trip over goes **16:1 → 128:1**, and *the loud
side is the side that was already safe*: a program tuned to 2,500 levels blows a
library callback thread at about 20.

**And it vetoed the compiler seat's own best alternative.** `mainthread.c`: SDL2
video on a 64 MB created thread is `SDL_Init(VIDEO) failed: No available video
device`; on the real main thread the window is created, exit 0. Running `main`
on a created thread is therefore **refused by the C boundary** — measured, and it
breaks a program that is already in `examples/`.

Reported and not charged: `vmmap` shows the kernel's PROT_NONE band below the
main stack collapsing from **56.0 MB to 16 KB** the instant `-stack_size` is
given at all, at every size from 16 to 128 MB. No harm followed — thread stacks
are placed above the main stack in both layouts, and 1 MB and 4 MB `alloca`
jumps are caught identically.

### What the coordinator measured, because two seats' conditions turned on it

The FFI seat's Linux alternative — `setrlimit(RLIMIT_STACK)` + re-exec — was
measured on Linux only. On this Mac, this session:

```
plain:               main = 8,372,224    default attr = 524,288
setrlimit + re-exec: main = 67,092,480   default attr = 524,288
```

So it raises **main** on both POSIX platforms, capped at Darwin's hard rlimit.
It does **not** reach Darwin's library threads: glibc derives the default thread
size from `RLIMIT_STACK` and Darwin's libpthread has 512 KB written into it. With
the link flag already applied, `setrlimit` to the hard limit fails `Invalid
argument`, because main is then 67,108,864 > 67,092,480 — the two mechanisms
collide.

Its price, 200 launches each: **0.76 s plain against 0.82 s**, i.e. **0.3 ms per
launch** — affordable even for the compiler, which would pay it on every
invocation (CLAUDE.md §15). Its soundness is the problem: `execv(argv[0])` fails
in the normal installed case —

```
launched by full path:                       exit 0
found on PATH, launched from elsewhere:      execv: No such file or directory, exit 1
```

— so it needs a per-platform *where am I* primitive (`_NSGetExecutablePath`,
`/proc/self/exe`), which is a second platform axis in `hero_os.h` on top of the
one the compiler seat already counted.

## Where the seats disagreed

**Head-on, with a measurement on each side: the compiler seat's best alternative
is the FFI seat's outright veto.** Running the program on a created thread with a
chosen stack size is what rustc does, works on all three platforms, needs no link
flag, does not override the operator, and puts the number where a thread creator
can read it. It also **fails `examples/sdl/`**, because macOS gives video to the
process's real main thread and nothing else. Both measurements stand; the veto
wins, because a resolution that breaks a program in the corpus is not a
resolution.

**On the spec, the ergonomist approves what the warden objects to.** The
ergonomist reads from the side of a person who guesses and is not corrected —
depth is the only silent guessing point of four. The warden reads from the side
of §1.6's payment standard — the sentence is inert, misattributes the cause to
*"the machine"* when the emitter decides, and arrives with nothing to pay with.
Both are right about different things, and the resolution below takes the
ergonomist's *finding* and the warden's *terms*.

**On whether 64 is the number**, nobody defended it. The compiler seat calls it
Windows' repair number typed once; the historian calls Darwin's ceiling exactly
64 MB with no margin; the warden shows a legal program at 248 levels there. The
defensible number argued from frames is **16 MB** (500 levels × 26 KB, +23%
margin), which is the compiler seat's own condition 2.

## The resolution — provisional, and the robust one rather than the conservative one

**The conservative resolution was *do nothing*.** Two seats vetoed, so changing
nothing satisfied every veto, and CLAUDE.md §4 requires this file to say what
that would have left standing: a Heroes program that runs out of stack on any
thread the main one **dies silently at exit 132 or 138**, with `--sanitize` no
louder. That is the §1.12 defect in this area, and it is not the one the
proposal was about. Robust means leaving the fewest ways to be wrong, so the
sitting adopts a larger repair than it was convened for and refuses all three
mechanisms it was convened to choose between.

**Refused, each by a measurement rather than a preference:**

1. **`-Wl,-z,stacksize=` on Linux — never shipped.** Two linkers ignore it, the
   ELF header proves it, the depth is identical, and it would print a warning on
   every Linux build forever through a stderr path this toolchain forwards by
   design.
2. **`main` on a created thread — refused.** It breaks `examples/sdl/`.
3. **A depth number in the spec — refused.** It would be false on the day it
   landed, for a legal program and nearly for this compiler itself.

**Adopted — the one thing every seat that compiles named independently, and no
seat vetoed: the guard's bounds and alt-stack become per thread.** The
compiler seat's §3 and the FFI seat's condition (b) are the same finding:
`hero_stack_lo/hi` (`runtime/parts/stack.c:107-108`) are the **main** thread's,
set once, so `in_guard` is false everywhere else and the fault is passed on.
Making them per-thread turns exit 132 into `panic: stack exhausted in <function>`
at exit 134 on every thread, on every platform, at every stack size.

**That, and not a number, is the uniformity the question asked for — and it is
the only uniformity that measurement admits.** The *number* cannot be made
uniform: the flag is inert on Linux, Darwin pins library threads at 512 KB
whatever is done, `ulimit -s unlimited` on Linux gives less than a finite value,
and the same 64 MB is 248 levels for one legal program and 2,537 for another.
The *behaviour at the boundary* can be made uniform, is what design.md §1.12
actually promises, and is the shape the historian found every real specification
uses — Scheme states a property; the JVMS refuses a size.

**The capacity asymmetry stays, and is recorded as deliberate.** Windows keeps
`/STACK:67108864` because it repairs a measured breakage (1 MB kills the
compiler on its own modules); POSIX takes the machine's answer, which is 9x what
the deepest Heroes program in existence needs (`< 896 KB`, measured). Principle 0
has no ticket for raising it: the compiler does not need it, and no Part 11
metric moves. **The direction with a Principle 0 ticket is the emitter** — 344
hoisted locals and 26 KB per level, where panel 106 already bought a factor of
two and journal 032 carries the next 9.1% with its number.

**One correction owed now, not queued:** `flags.hero:74-78` says a large reserve
*"costs nothing real"*. Measured false for the runaway case (9,814,016 →
68,534,272 max RSS), and the comment is corrected in the same commit as this
file, with the number and the case.

**Queued, with its payment named and not pretended:** a spec sentence stating
the *property* — that recursion is bounded and exhaustion is a named error
rather than undefined behaviour — becomes true only once the per-thread guard
lands, and is worth **+16 to +23** against a headroom of 346. The warden holds
it **unpaid** and names what pays for it: a removal of equal size, or an
instrument showing fewer first-try failures on recursive programs. Deferring it
until a payment exists is §1.6 working, not a compromise; the ergonomist's
finding — that depth is the only silent guessing point — is what makes it worth
paying for, and the guard is what makes it true.

**What a veto compelled:** had the sitting adopted the proposal, the Linux arm
would have shipped a permanent build warning for zero bytes and the Darwin arm
would have widened the silent-thread gap to 128:1. Both were caught by seats
that compiled rather than by argument.

## Predictions to score

| origin | prediction | instrument | scored at |
|---|---|---|---|
| compiler-engineer | with the flag shipped to Linux, the interpreter's depth is **identical** with and without it (delta exactly 0) while macOS exceeds 2,000; and `heroes build` emits ≥1 linker warning per link, turning ≥1 byte-for-byte stderr case red | the Linux CI leg; `readelf -l` | M-corpus-depth close — **already falsifiable, and the coordinator measured the first half: 388 both ways** |
| compiler-engineer | at least one `selfhost/` function still exceeds 65,536 bytes of frame, so a 1,000-deep path through it exhausts 64 MB (today: 1 of 2,913 over 65,536; 14 over 21,845) | `-fstack-usage` + the deep-recursion golden | M-qbe-backend close |
| ffi-pragmatist | on the Linux leg every `heroes build examples/sqlite/main.hero` prints the ignored-flag warning and `sqlite3_open` still runs on an 8,388,608-byte stack; on Darwin `examples/sdl/`'s audio callback and `examples/raylib/`'s stream callback still get 536,576 bytes and still exit 132 with empty stderr | the two commands | next milestone that touches `runtime/parts/stack.c` |
| spec-warden | if the vague sentence lands anyway, it produces **zero** new `.expected` diffs and **zero** corpus program changes while the spec sits at 3773 with headroom ≤ 323 | `heroes measure` + the golden harness | M-corpus-depth close |
| coordinator | with the guard's bounds per thread, overflow inside SDL's audio callback becomes exit **134** with a named function, and the interpreter's ceilings in all three configurations move by **zero** levels | `overflow_thread.c` re-run; the corpus's three configurations | the milestone that lands the per-thread guard |

## Author's verdict

Pending.

## What the lane gave up

Nothing — this was the full panel. What it cost: five seats, four of which
compiled, about 25 minutes of wall clock. What the two non-compiling seats
bought is stated at the top of this file, because the choice to run full rather
than soundness was the coordinator's and it should be scoreable.

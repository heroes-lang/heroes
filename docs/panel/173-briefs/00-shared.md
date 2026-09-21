# Panel 173 — shared brief: the runtime names the lease at the crash

**Soundness lane**: `compiler-engineer` and `ffi-pragmatist`, the two seats
that compile, plus a completeness critic. The proposal changes **no surface, no
diagnostic class the checker emits, and no spec token**: it is a runtime
mechanism, which is what the lane exists for (`/panel` § Two lanes). What the
lane gives up is the ergonomist's reading of the message and the historian's
precedent for signal-time reporting; both are named as unrun in the synthesis.

**Every number below was produced by a command run on 2026-09-21 and the
command is beside it**, or is cited to the report that holds it. Re-run
anything you rest a verdict on.

## What panel 172 resolved, and why this sitting exists

`docs/panel/172-the-report-comes-from-the-crash-and-not-from-the-declaration.md`,
resolution item 1: **the repair of defect 070 is a run-time report and not a
word.** The critic of that sitting wrote fifty lines — `docs/panel/173-briefs/prototype.diff`,
48 in `runtime/parts/os.c` and 2 in `runtime/parts/alloc.c` — that install a
SIGTRAP/SIGABRT handler on the same mechanism `runtime/parts/stack.c:582-596`
already installs for stack exhaustion, read `hero_live_held`
(`runtime/parts/alloc.c:98`, the live lease balance the exit sweep already
checks), and write with `write(2)` only, as `hero_stack_say` does. The
coordinator rebuilt that runtime and re-ran it:

    cd <critic's copy> && rm -rf build && clang -I runtime seed/heroes.c runtime/runtime.c -o heroes
    CPATH=docs/panel/172-briefs ./heroes build docs/panel/172-briefs/lease070.hero -o /tmp/p_lease070
    for i in $(seq 10); do /tmp/p_lease070 >/dev/null 2>/tmp/e; printf "%s/%sB " $? "$(wc -c < /tmp/e)"; done
    133/250B 133/250B 134/250B 133/250B 133/250B 133/250B 133/250B 133/250B 133/250B 133/250B

    panic: a C function freed bytes this program still leases — 1 lease(s) were live when the allocator refused the free.
      the bytes of a `.lease()` are the program's, freed by `end_lease`,
      and the callee that was handed one frees what it is handed

    CPATH=docs/panel/172-briefs ./heroes build docs/panel/172-briefs/control.hero -o /tmp/p_control
    /tmp/p_control    exit 134, stderr 111B: panic: 1 lease(s) never ended — …

Item 1 says the mechanism **lands after this sitting judges it**, on four
questions panel 172's critic left unrun. This sitting judges the mechanism and
nothing else: not the direction (172's, ratification pending), not the word
(refused, Part 6 stands), not the spec sentence (priced at landing).

## The object under judgment, verbatim

`docs/panel/173-briefs/prototype.diff` is the whole of it. In prose: a
`volatile sig_atomic_t hero_exit_sweep_running` set at the top of
`hero_runtime_check_leaks`; a handler `hero_lease_crash` that, when that flag is
clear and `hero_live_held > 0`, writes the three lines above and then restores
`SIG_DFL` and re-raises; an installer that registers it for `SIGTRAP` and
`SIGABRT` with `SA_SIGINFO | SA_ONSTACK`, saving the previous dispositions and
never chaining to them; a `#else` stub for `_WIN32`; and one call to the
installer beside `hero_stack_guard_install()` in `os.c`'s init.

## The four questions, and what is already measured about each

**1. The panic path.** Every runtime-initiated death is `abort()`, which is
SIGABRT, which is this handler. Counted today:

    grep -c 'abort()' runtime/parts/*.c
    alloc.c 5 · panic.c 1 · stack.c 6 · failure.c 3 · os.c 7      (22 sites)

The prototype's flag covers ONE of the 22, the exit sweep. The critic measured
the consequence on `docs/panel/173-briefs/panic_lease.hero`, a Heroes array
index out of range while a lease is live: exit 134, **282 bytes, two `panic:`
lines**, the second false (`docs/panel/172-reports/completeness-critic.md`
§ 2). A stack overflow with a live lease would do the same through `stack.c`'s
handler if that handler ends in `abort()`, and any of alloc.c's five. **The
question**: what is the sound shape — a flag set at every runtime-initiated
abort (22 sites, or one function they all go through), a handler that reports
only when the signal's origin is not the runtime, or something the seats find
— and what does each cost?

**2. The handler `stack.c` already ships.** `hero_stack_guard_install`
registers `hero_stack_handler` for SIGSEGV and SIGBUS with the same flags and
saves the previous dispositions (`stack.c:582-596`); the prototype registers a
second handler for two other signals and saves, but does not chain to, the
dispositions it found. **The question**: do the two compose — order of
installation, the alternate stack both use (`SA_ONSTACK`), a SIGSEGV inside a C
`free` while a lease is live, `abort()` raised from inside `hero_stack_handler`
— and what does the prototype owe the previous disposition it saved and never
calls?

**3. Linux.** Every number so far is Darwin 25.6.0 arm64
(`.claude/rules/platforms.md`: a platform fact is run on a platform or it is an
inference). The Linux containers exist and their images are built:

    docker images | grep heroes-linux
    heroes-linux-arm64:latest    heroes-linux:latest

`docs/ref/environment/linux/LINUX-MACHINE.md` says how to enter one (Docker
Desktop rests stopped; start it first). glibc 2.41 there. **The questions**:
which signal does glibc's `free` of an interior pointer raise (glibc prints
`free(): invalid pointer` and calls `abort()`, which is SIGABRT — measure it);
does the prototype print on Linux, once or twice (glibc's own line plus ours);
does `--sanitize` on the Linux leg, where LeakSanitizer exists (CL-055), still
compose. **Windows** is the third platform and the box is off; the prototype's
`#else` stub means no report there, and `stack.c` has a vectored-exception
handler for Windows (`stack.c`, the `#else` arm after line 596). Whether a
Windows analogue exists is a question the synthesis records as UNRUN unless a
seat can answer it from the code.

**4. Naming the callee.** `hero_stack_blame(pc, fp, lr)` at `stack.c:331`
walks frames and blames a Heroes function by name from inside a signal handler.
**The question**: can the lease report say which Heroes function made the call,
or which C function freed, from the same context, and what would it cost
(`siginfo_t`, `ucontext_t`, frame walk); or is the lease's own name (the cell,
from the runtime's records if it keeps any) reachable?

## What the landing owes, so the seats judge the whole and not the diff

- **A `tests/golden/run/` case per shape** that reports: `lease070` (the
  filed one), `b_callback`, `b_later` (all three in this directory, `beside.h`
  beside them). The run form's `.expected` carries stdout lines and then
  stderr lines prefixed `!`, as `tests/golden/run/abort-lease-never-ended.expected`
  shows; the seats say how the exit code is judged there, because ours is 133
  on one path and 134 on another.
- **A case that stays silent**: `b_out` from the critic's report (an `@out:
  cstr` C fills and frees; no lease is live, the pointer is C's) — the class
  boundary, written down as a golden so the report never claims more than it
  knows.
- **The panic path fixed** per question 1, with `panic_lease.hero` as its
  golden: one `panic:` line, the true one.
- **The Linux leg**: a program that declares an `extern` runs there under
  `--sanitize` (CL-055), so the new goldens are judged on Linux by CI; the seats
  measure locally first.
- **No seed regeneration**: `seed/heroes.c` is the compiler's C, and the
  runtime is `runtime/runtime.c` plus `runtime/parts/`, so this change reaches
  the seed not at all. Say so if you find otherwise.
- The spec sentence (172's item 3) and defect 070's closing are the landing
  commit's and not this sitting's.

## Procedure, and it binds every seat

- **Build in a fresh copy** (the copies from panel 172 carry prototypes of
  other things):
  `rm -rf <scratch>/<seat>-173 && cp -r /Users/joseph/Temp/heroes/heroes-lang <scratch>/<seat>-173 && cd there && rm -rf build && clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`.
  Apply `docs/panel/173-briefs/prototype.diff` with `git apply` in the copy,
  rebuild the same way (the runtime is compiled into the `heroes` binary and
  into every program it builds), and measure. Never edit the real tree; your
  one file there is your report.
- **Run everything in the foreground with a generous timeout. No background
  jobs, no polling loops**: three seats of panel 172 died on a 600-second
  watchdog doing exactly that.
- `CPATH=docs/panel/173-briefs ./heroes build docs/panel/173-briefs/<x>.hero -o <out>`
  builds the programs here; `--sanitize` adds the sanitizers.
- **Your report goes to `docs/panel/173-reports/<seat>.md`.** Every number
  from a command you ran, with the command beside it (CL-077). A negative
  sentence is run or written as a question saying what you searched. No timing
  claims: the machine is shared. Verdict shape: verdict · design.md section ·
  cost · one falsifiable prediction naming an instrument that exists · condition;
  a veto is named as a veto.

# Panel 173 — the runtime may say what it saw, and not why

2026-09-21, M-declared-extents, after step 26 (`52a8faf3`). **Soundness lane**:
`compiler-engineer` and `ffi-pragmatist`, the two seats that compile, on the
mechanism panel 172 adopted and sent here before it lands. No completeness
critic: the lane is two seats by construction, and what it gives up — the
ergonomist on the message a reader gets, the historian on precedent for
signal-time reporting — is named as unrun below rather than assumed away.

Briefs and every reproducer: `docs/panel/173-briefs/`, including
`prototype.diff` (what panel 172's critic wrote) and `as-judged.diff` (what the
engineer built and this sitting judged). Reports: `docs/panel/173-reports/`.

**Procedure, recorded because it happened.** Both seats died on a model rate
limit mid-work and were relaunched; the engineer's second run inherited its
predecessor's build, verified it command by command and says in its report
which parts are its own. The coordinator ran one measurement of its own, at the
end, because the two seats measured two different builds and nobody had run the
one that matters.

## The question

Panel 172, resolution item 1: *the runtime names the live lease when a C
function frees it*, on the signal handler `runtime/parts/stack.c` already
installs, reading the live-lease balance `runtime/parts/alloc.c` already keeps.
Fifty lines, measured by that sitting's critic to turn defect 070's filed
reproducer from **zero bytes of stderr into 250 naming the lease**. It lands
after this sitting judges it, on four questions 172 left unrun: the panic path,
composition with `stack.c`'s handler, Linux, and whether the report can name
the callee.

## The resolution, in one line

**Both seats veto, on the same clause, and it is the sentence and not the
mechanism**: the handler asserts *a C function freed bytes this program still
leases*, and that is **false on six of nine measured paths** — a C library's
own `abort()`, its failed `assert`, its double free of its own pointer, a
trap — with `siginfo_t` measured unable to tell any of them from the true case
on either platform. The repair costs zero net lines: say what the runtime
observed. Everything else the engineer built is endorsed by both seats and by
the coordinator's own run: the `hero_abort()` funnel, the compile-time yield to
the sanitizer, the chaining to the disposition it found, and the Heroes
function named from the frame walk.

## The verdict table

| | compiler-engineer | ffi-pragmatist |
|---|---|---|
| **verdict** | **VETO** on soundness, cheap to answer | **VETO** on `prototype.diff` as written, three conditions lift it, all three run |
| **section** | design.md §1.12 and its own falsifier clause at `:605`; §4.17's `certain \| guess` doctrine at `:2036` | design.md §1.11, §4.19; `.claude/rules/platforms.md` CL-055 as the instrument |
| **cost** | zero in the compiler (`selfhost/` untouched, `cmp seed/heroes.c` silent); 136 runtime lines touched, +121 net, **79 code**; `os.c` 347 → 449, `panic.c` 58 → 77; two `sigaction` calls at start | the ABI, until chained: a C library's SIGABRT disposition destroyed, 10/10 on two platforms. Repair ten lines. Plus one `#include <signal.h>` for the Linux `--sanitize` build |
| **prediction** | if the landing does not touch `tests/harness/suite_runtime.hero`, that suite reports **6 passed, 2 failed** at the closing commit, naming `os.c:172`, `:173`, `panic.c:32` and *49 against a floor of 39*; and the sound repair costs within 5 lines of today's 102 in `os.c` | `suite_run.hero:146-148` refuses all three reporting goldens with *tripped a sanitiser*, on Darwin and both Linux legs, whatever the `.expected` says |
| **condition** | the sentence stops asserting the cause (recommended), **or** a discriminator measured on both platforms, **or** a measurement that its two programs are not a class | chaining; `<signal.h>`; and no path printing a sentence measured false |

## What the seats measured

### The veto, and both seats found it from opposite ends

The engineer wrote two programs, nine and ten lines, whose C side frees nothing:

| program | what C did | exit | stderr | the report |
|---|---|---|---|---|
| `cabort_lease` | `abort()` for its own reason | 134, 5/5 | 279 B | *a C function freed bytes this program still leases … called from cabortlease.main* |
| `cassert_lease` | a failed C `assert` | 134, 3/3 | 350 B | C's true line, then the same false one |

The ffi seat reached the same place by enumeration, running every neighbouring
shape: **six of nine measured paths print a sentence that is false**, including
a library's `assert` — *the most ordinary thing a C library does on a bad
argument* — which with any lease live anywhere in the program now points the
reader at the FFI when the bug is elsewhere.

**And `siginfo_t` cannot rescue it, measured twice independently.** The
engineer compiled a probe into the handler: on SIGABRT the true case and both
false cases are byte-identical in `signum`, `si_code` and `si_pid`. The ffi
seat's table adds Linux: every self-raised SIGABRT is `si_code = 0` on Darwin
and `-6` on Linux, the same for the allocator, the library and the runtime. So
the shared brief's option *report only when the origin is not the runtime* is
not implementable from the signal; it needs a flag, which is what the funnel
is, and the flag can only speak for the **Heroes** half.

A SIGTRAP-only report would be sound on Darwin and **drops 24% of the real
cases**: 200 runs of `lease070` gave 152 at 133 and 48 at 134. On Linux there
is no SIGTRAP at all from the allocator, so it would report nothing.

### The ABI clause, and the measurement neither seat could make

The ffi seat wrote `libabrt.h`: a C library that installs a SIGABRT handler and
`_exit(77)`, so whose handler ran is read off the exit code. Installed from a
constructor — *how every real crash reporter installs* — against
`prototype.diff`:

| | stock | prototype |
|---|---|---|
| Darwin, ctor-installed | **77** ×10 | 133/134, the library's handler **never runs** |
| Linux arm64, ctor-installed | **77** ×10 | 134 ×10, **never runs** |

An ABI break, and **the repository had already settled it**:
`runtime/parts/stack.c:363-375` saves the previous disposition and calls it,
with its own comment at `:35-36` stating the rule; the prototype saved and never
read. The ffi seat built the chained variant, ten lines copied from `stack.c`,
and measured 77 ×10 on both platforms with the report still printing.

**Independently, the engineer's build already chained** (`os.c:196-213`,
chain-then-restore-then-reraise) — and marked that branch UNRUN, having written
no C library that installs a handler. So the two seats measured two different
builds and the one that matters was nobody's. **The coordinator ran it**, on the
engineer's build, with the ffi seat's program:

    cd <engineer's copy>
    CPATH=docs/panel/173-briefs ./heroes build docs/panel/173-briefs/q4_ctor.hero -o /tmp/e_q4ctor
    for i in $(seq 10); do LIB_CTOR=1 /tmp/e_q4ctor >/dev/null 2>/tmp/e_q4; printf "%s " $?; done
    77 77 77 77 77 77 77 77 77 77
    stderr: our three lines, then `library: my SIGABRT handler ran`
    (stock runtime, same program: 77 ×10, the library's line alone)

**The ABI clause is answered in the build this sitting judges.** The ffi seat's
veto clause 1 is satisfied, measured.

### Linux, run on Linux

`heroes-linux-arm64`, Debian 13, glibc 2.41, clang 22.1.8. Four facts, none
inferred: the signal from the allocator is **always SIGABRT (134)**, never
SIGTRAP — the 133/134 split is Darwin's alone; glibc's line for the lease shape
is **`munmap_chunk(): invalid pointer`**, not the `free(): invalid pointer` the
brief predicted, and not one string across block sizes; the report prints
**beside** glibc's line and never instead of it, 32 + 250 = 282 bytes exactly,
10/10; and `b_out` stays silent there too, so the class boundary holds on both
platforms.

**And Linux is where the prototype breaks the build.** `heroes build
--sanitize` exits 2 for every program:
`runtime/parts/alloc.c:203:10: error: unknown type name 'sig_atomic_t'`. The
trigger is the sanitizer alone, isolated against `-std=c11` and `-std=gnu11`;
Darwin hides it completely. CL-055 makes `--sanitize` mandatory on the Linux leg
for every program that declares an `extern`, so this is every FFI program in the
repository, red on both Linux legs of the CI matrix. **One `#include
<signal.h>`, run, exit 0.** The engineer's build moves that flag to
`panic.c:32`, so whether the include is owed there instead is **UNRUN** and the
landing measures it in the container before the commit.

**`--sanitize` does not compose, and the divergence is platform-shaped.** With
the include fixed: on Linux ASan calls `_exit(1)`, no SIGABRT is raised, and our
report **never prints**, while ASan names the freeing C function by file and
line; on Darwin ASan aborts, so it does print. The engineer's compile-time yield
is therefore right in both directions and both seats endorse it: ASan's report
is strictly better, and under it `stack.c` compiles only empty doors, so the
frame walk does not exist to call.

### What the funnel bought, and what both seats would keep

Every runtime-initiated death now passes one `hero_abort()` that sets a flag
before its SIGABRT: fifteen sites in five files, zero bare `abort()` left
outside it. Measured, one true line each: a Heroes index panic 32 B, a failed
`assert` 50 B, a stack exhaustion with a live lease **39 B**, a null write
through C 161 B. That last one nests two frame walks on one 256 KiB alternate
stack with no truncation and no second fault.

Composition with `stack.c` is clean and not by luck: four distinct signals
(SIGSEGV and SIGBUS there, SIGTRAP and SIGABRT here), and the install order at
`os.c:239-242` is right because the alternate stack is mapped inside
`hero_stack_guard_enter()`.

The report **names the Heroes function** — `lease070.main`, `bcallback.main`,
`blater.main` — for six lines, reusing the walk the stack guard already has.
It **cannot name the C callee** (the walk filters for the mangled prefix; ASan
can and does) and **cannot name which lease** (`hero_live_held` is a counter;
a name per lease is a pointer per `.lease()` and an emitter change, priced and
refused on §1.1's ceiling).

### Two suites go red, and one of them is the landing's real work

**`runtime`: 8 passed, 0 failed → 6 passed, 2 failed**, attributed by reverting
the five files and re-running. Three unnamed shared objects (`os.c:172`, `:173`,
`panic.c:32`) and *49 against a floor of 39*. The landing owes three
`SHARED_BY_DECISION` entries in `tests/harness/suite_runtime.hero` and
`LEAST_SHARED_STATE` at `:217` raised — prose the suite grades in both
directions.

**`run`: 132/0 → 133 passed, 4 failed**, all *tripped a sanitiser*.
`suite_run.hero:135-148` runs every case three times, at `-O0`, `-O2` and under
`--sanitize`, and fails any case whose sanitised stderr contains
`AddressSanitizer`. Three of the new goldens are bad frees by construction and
one is a double free. Both seats grepped for an opt-out and there is none: the
only skip is a missing pkg-config library. **So the cases panel 172's
resolution owes cannot live in `tests/golden/run/` as the harness stands**, and
that is a `tests/harness/**` change judged by the net's own tests. With the new
goldens moved aside the same patched runtime reads 132 passed, 0 failed: the
mechanism breaks nothing that exists.

**And two defects in the goldens themselves**, found by the engineer against its
own predecessor's work: `abort-panic-with-a-lease-live.expected` is containment
only, so a stderr carrying the true line **and** the false second one satisfies
it — the case is green on the very prototype it exists to refuse; and
`ffi-out-pointer-is-c-to-free.expected`'s `!exit: 133` is exact and 133 is a
Darwin allocator fact, 200/200 here and a question on glibc.

### The good news, measured, and it belongs in the record

**The direct spelling of this defect cannot be written.** The ffi seat tried
every way to hand a lease to a real freeing function and the compiler refused
each: a `cstr` lease into a `ptr` parameter is `type_mismatch`; a `void *` or
`char *` parameter declared `cstr` is `ffi_writable_parameter`. Five programs,
five refusals, `free` and `sqlite3_free` alike. The defect reaches a real
library only by the indirect route — C stashes the pointer and frees it later —
which is the shape panel 172 measured that no word can reach. And the report
does print through a real `-lsqlite3` free.

## The disagreements

**None on the mechanism.** Both seats veto the same clause and endorse the same
four parts. The only divergence is which build each measured — the ffi seat
`prototype.diff`, the engineer `as-judged.diff` — which produced two apparently
conflicting ABI verdicts, resolved by the coordinator's run above: the
prototype breaks the boundary, the judged build does not.

**One disagreement with panel 172's own resolution.** That sitting's item 1
promised *a `tests/golden/run/` case per shape*; this sitting measured that the
harness refuses them. 172's resolution is amended here rather than quietly
narrowed.

## The resolution — `provisional — author ratification pending`

1. **The sentence states what the runtime observed, never the cause.** The
   handler knows two things: the process is dying by a signal that is not the
   runtime's own, and N leases are live. It does not know that a free happened,
   and `siginfo_t` cannot tell it. The line goes in §4.17's `guess` register —
   the fact first, the likely reason named as likely — and the wording is the
   landing's, held to this: **no path prints a sentence this sitting measured
   false.** `cabort_lease` and `cassert_lease` become goldens beside `b_out`, so
   the boundary is written down from both sides.
2. **Kept unchanged, all four endorsed twice**: the `hero_abort()` funnel and
   its fifteen sites; the compile-time yield to the sanitizer; the
   chain-then-restore-then-reraise, which the coordinator measured returns a C
   library's handler to 77 ×10; and the frame-walk name.
3. **`<signal.h>` where the flag now lives**, measured in the Linux container
   before the commit, because CL-055 puts every FFI program's `--sanitize`
   build on that leg and Darwin hides the error entirely.
4. **`tests/harness/suite_runtime.hero` gains the three `SHARED_BY_DECISION`
   entries and the raised `LEAST_SHARED_STATE`**, with the reason written where
   the suite grades it.
5. **The goldens need a home the harness admits.** Panel 172's *one case per
   shape* stands as the goal; the mechanism is the landing's to choose and to
   measure, and the two candidates both seats named are a per-case sanitizer
   opt-out in `tests/harness/` — a case that is *supposed* to provoke ASan,
   asserted rather than tolerated — or a golden form of its own. The net's own
   tests judge it. **Until one exists, no case lands**, because a suite made
   green by deleting its evidence is worse than a defect.
6. **Two golden defects are repaired in the same commit**: containment alone
   cannot guard the two-line regression, and `!exit: 133` is a Darwin fact that
   must not be pinned exactly.
7. **Defect 070 closes with this landing**, as panel 172's item 4 says and with
   this sitting's amendment to what the report may claim.

**What conservative would have been** (CL-040): land the mechanism silent
under any doubt — report only on SIGTRAP, where the engineer measured the
assertion sound on Darwin. Refused: it drops 24% of the true cases on this
platform and 100% on Linux, which trades a false sentence for a missing one,
and the missing one is the defect.

## Predictions to score, at the M-declared-extents close

| seat | prediction | how it is scored |
|---|---|---|
| compiler-engineer | without a `suite_runtime.hero` change, `runtime` reads 6 passed, 2 failed at the closing commit, naming the three objects and *49 against a floor of 39* | run that suite at the landing commit |
| compiler-engineer | the sound repair costs within 5 lines of today's 102 added in `os.c`; above 130 means a discriminator was built, and then the Linux measurement is owed | `git diff --numstat runtime/parts/os.c` at the landing |
| ffi-pragmatist | `suite_run.hero:146-148` refuses all three reporting goldens with *tripped a sanitiser*, whatever the `.expected` says | the engineer already measured 4 such failures; scored CORRECT at this sitting and re-checked at the landing |

## Author's verdict

**RATIFIED 2026-09-21**, in one act with panels 171 and 172, in the author's
words: *then ratify all the DECIDE items.* Recorded as a ratification given in
conversation on the coordinator's summaries in that session, not as a reading
of this file, and **not `by delegation`** (CL-058).

**The veto was answered before the ratification reached it**, at step 28: the
sentence states the two facts the runtime holds and names the C free as a
condition — measured true on all nine paths, including the two programs whose
C side frees nothing; `runtime/parts/panic.c` carries the `<signal.h>` the
Linux `--sanitize` build needs, run in the container; `tests/harness/suite_runtime.hero`
gained the three `SHARED_BY_DECISION` entries and the raised floor; and the
harness gained `!sanitizer:`, so the three cases that close defect 070 could
enter `tests/golden/run/` as cases rather than as a relaxed rule. The
conservative option this sitting recorded — report only on SIGTRAP — was **not**
taken.

What this section said while the item was open, kept because a record is never
rewritten: *Pending: `docs/work/DECIDE.md` carried this sitting as `panel 173`.
Work proceeded on the resolution: the landing commit, then defect 070 closes,
then the milestone's close checklist.*

## Unrun, and named

Windows: the box is off; the mechanism's `#else` stub means no report there,
and whether `stack.c`'s vectored-exception handler admits an analogue is unrun.
Linux x86-64: everything measured is arm64. An externally delivered SIGABRT:
`raise` is a reserved word and the seat's program would not build. The
chaining branch under `--sanitize`: compiled out, so the question does not
arise. Timing: forbidden here, the machine is shared. And the two seats the
lane does not seat — the reader's view of the message, and precedent for a
runtime that speaks from a signal handler — are unrun by construction.

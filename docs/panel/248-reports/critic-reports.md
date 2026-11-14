# Panel 248, the completeness critic's second pass, over the reports

Started 2026-11-14 11:47 CET (`date`), base `df7612ad3` (`git rev-parse HEAD`). Read whole: the four seat
reports (`compiler-engineer.md` 710 lines, `ffi-pragmatist.md` 863, `spec-warden.md` 432, `historian.md` 737,
`wc -l`) and the briefs as they stand. Every check below was run by this critic in this pass; its files
are under `.claude/worktrees/p248-scratch/critic/r2/`. Written as it goes; the numbered list is at the end.

The machine stood at load average 224 to 333 through this pass (`uptime`), so no duration below is a
measurement; latencies are indicative and said so. Linux arm64 ran in one named container
(`critic-248-r2`, `critic/r2/linux/run.sh`, log `r2.log`, `docker exit 0`, `docker ps` empty after);
Windows on the box under `/c/w/p248-critic/` (clang 23.1.1, its default target, the MSVC one by
`docs/platforms/windows/WINDOWS-MACHINE.md:357`), short commands over `ssh`, nothing left running.

## Q1: route I-onstack against route I-mask

### The veto's measurement, re-run (Linux arm64)

The ffi-pragmatist's files copied into `critic/r2/linux/` (`gcreal.c`, `inst.c`, `inst_mask.c`,
`gc-8.2.8.tar.gz`, `shasum -a 256` `7649020621cb2632...` as its report says), built exactly as its
`linux/phases.sh:146-153` builds them (`extra/gc.c` with `-DGC_THREADS -DGC_BUILTIN_ATOMIC`, `gc.o exit
0`), five runs each under `timeout -s KILL 60`:

| build | printed | 5 runs |
|---|---|---|
| base | *"the collector's suspend handler (SIGPWR) installed with SA_ONSTACK: 0 / the list intact after 301 collections"* | **5 of 5 exit 0** |
| I-onstack (`inst.c`) | *"... SA_ONSTACK: 1"*, then *"Segmentation fault"* | **5 of 5 exit 139** |
| I-mask (`inst_mask.c`) | *"... SA_ONSTACK: 0 / the list intact after 301 collections"* | **5 of 5 exit 0** |

**The measurement holds as reported.** And it meets the compiler-engineer's own withdrawal condition,
written before he saw it: *"a library whose handler is measured to need the interrupted stack"*
(`compiler-engineer.md:586-589`). His report does not mention bdwgc at all (`grep -c -i bdwgc` and
`grep -c -i collector` over his report: 0 and 0). **The veto's program is Linux's**: in the same
tarball `pthread_stop_world.c` compiles only `!defined(GC_DARWIN_THREADS)` (its lines 21-22, `tar -xzOf`),
and `darwin_stop_world.c` suspends a thread by `thread_suspend` (536, 640) and reads its registers by
`thread_get_state` (192, 208), no signal handler; so on Darwin I-onstack does not reach bdwgc, by that
reading, unrun. Other signal-suspending collectors on Darwin are a question.

### I-mask's residues, run (Linux arm64, the ffi-pragmatist's `inst_mask.c`, `critic/r2/linux/`)

- **A program's own use of SIGURG is taken from it.** `swait.c` blocks SIGURG with
  `pthread_sigmask(SIG_BLOCK)`, sends itself SIGURG and waits 2 s in `sigtimedwait`. Plain: *"SIGURG
  blocked after the program's own pthread_sigmask(SIG_BLOCK): 1"*, *"sigtimedwait: SIGURG taken"*.
  Linked with I-mask's definitions: **"... : 0"** and **"sigtimedwait: nothing in 2 s"**. The cause is in
  the route as built (`inst_mask.c:76-84`, `siginstall_mask.c:76`): a request that would take the marker
  from unblocked to blocked has the marker struck out, so the program's block is dropped, SIGURG
  arrives at its default (ignored) and is lost. This is a correct C program broken by the route, and it
  is the ffi-pragmatist's own withdrawal condition in its words: *"a C library a binding plausibly links
  ... to need the marker signal itself (SIGURG: blocked by it outside a handler through a raw system
  call, or waited for)"* (`ffi-pragmatist.md:726-729`). Waited for, through the C library's own call,
  not a raw one. A library that takes out-of-band notices with `sigwait` or `signalfd` is that shape.
- **Go uses SIGURG.** Go's runtime, `src/runtime/signal_unix.go` at `master` (`curl` of
  `raw.githubusercontent.com/golang/go/master/src/runtime/signal_unix.go`, read 2026-11-14, 1,483 lines):
  line 74, **`const sigPreempt = _SIGURG`**, its goroutine preemption signal; and line 1133, *"non-Go code
  set up signal handler without SA_ONSTACK flag"*, thrown when a signal arrives on a thread that has a
  signal stack but a handler not on it (1126-1134). A Go `c-archive` bound by a Heroes program is
  therefore a library that uses the marker; what I-mask's dropped blocks do to Go's own SIGURG
  handling is unrun here, a question. And the historian quotes Go's `os/signal`: *"If the non-Go code
  installs any signal handlers, it must use the SA_ONSTACK flag"* (`historian.md:117-121`), and ART's
  `libsigchain`, *"SA_ONSTACK: always silently enable this"* (`:98-104`): **two production runtimes
  want what I-onstack does, and one collector dies of it.** Neither soundness seat cites either
  precedent (`grep -c -E 'libsigchain|ART|os/signal'` over both reports: 0).
- **A handler that unblocks before calling back passes I-mask.** `hunblock.c`: a handler that keeps its
  mask, then the callback, *"the marker blocked 1"* (refused); one that `sigprocmask(SIG_SETMASK,
  &empty)` first, **0**; one that `pthread_sigmask(SIG_UNBLOCK, &all)` first, **0**: not refused, so the
  walk and D are what stand under it. A miss, never a false refusal; a residue neither seat lists.
- **A mask inherited with SIGURG blocked**: in C alone (`inh.c` with `inst_mask.c`), started by a parent
  that blocked SIGURG before `exec` (`python3 -I -c '... signal.pthread_sigmask(signal.SIG_BLOCK,
  {signal.SIGURG}); os.execv(...)'`), the route's question answers *"the marker blocked 1"* in `main` and
  in a correct `qsort` comparator, which would be a false refusal of every callback. **In the runtime as
  the ffi-pragmatist built it, it is not**: the base compiler built in the container (`sources
  4c9a6a9800095eef`), panel 247's `qsortcost` at n = 1,000 (`critic/r2/linux/qsmall.hero`) printed `1`,
  exit 0, under the base and under its I-mask runtime (`siginstall_mask.c`, `runtime_mask.c`,
  `sigguard_mask.c` copied in as its `phases.sh:189` does), started plainly and under the parent that
  blocked SIGURG. Why, by reading `sigguard_mask.c:213-222`: `hero_signal_learn` unblocks SIGURG for
  its probes and puts the old mask back with `pthread_sigmask(SIG_SETMASK, &was, NULL)`, which goes
  through the route's own `pthread_sigmask` and has SIGURG struck out. So **the first function handed to
  C clears an inherited SIGURG block for good**, by a side effect nobody wrote down, which the route's
  correctness now leans on.

### What the two disagree on about Darwin

- **The compiler-engineer's I-onstack defines the names in the executable**, needs no dylib, and leaves
  a two-level dylib's handler unrefused: *"R3 with the library a dylib ... 5 of 5 `answer ran for 30`,
  exit 0, not refused"* (`compiler-engineer.md:150-151`), which he calls *"the escape's to tell"*
  (`:503`) and lists as I-onstack's residue (`:580-581`). **R3 says the class is `blocking` and *"never
  a known cost"*** (`247-*.md:191-195`), and a frameless handler in a dylib calling a Heroes function is
  that class. So the route he approves keeps, on Darwin, a measured residue of the class R3 forbids
  keeping. He calls I-dylib *"unbuilt"* (`:501-503`); the ffi-pragmatist built and ran it.
- **The ffi-pragmatist's I-mask on Darwin is a dylib interposing three names of libsystem_kernel**
  (`ffi-pragmatist.md:359-363`), reaching every image's installer because libSystem's own cross-image
  binds are interposed (`:77-84`: interposing `__platform_sigaction` and calling `sigaction` from it
  recursed, exit 139), so the two-level dylib rows are refused (`:365-367`). Its price, measured by
  him and priced by nobody: `heroes build` gives a program no `LC_RPATH`, an `@rpath` install name
  failed *"no LC_RPATH's found"*, exit 134 (`:155-158`); under the hardened runtime dyld refused the
  hooks' dylib, *"different Team IDs"*, exit 134 (`:59`); and dyld's `__interpose` is undocumented in
  `man dyld` (`:70-75`). **So every program that hands C a function would need a second file beside
  it, and dies before `main` where that file is missing or unsigned.** Neither seat asked what a
  program that today is one file becomes, nor what a missing dylib means for robustness.
- **They agree** that Windows needs nothing (`compiler-engineer.md:526-530`, `ffi-pragmatist.md:285-290`),
  that the two D cases of 2494 move (`compiler-engineer.md:114-123`, `ffi-pragmatist.md:409-422`), that
  `ffi_defined_elsewhere` widens on Linux (both, in the escape's words), and that B and T fail (the
  ffi-pragmatist ran both and vetoes; the compiler-engineer rejects them by price and reading).

## Q2: route (g) against one poller

**What each rests on.** Route (g) rests on a shutdown waking a `poll` already waiting on a connected
socket, measured on Darwin (0.1 ms) and Linux arm64 (0.1 ms), not on Windows (it did not wake) and not
on Linux x86-64 (`compiler-engineer.md:214-223`, `:539-542`, its condition at `:619-621`). **The
ffi-pragmatist's poller rests on the same thing in its measured form**: `wake.c`'s `poller` shape wakes
its waiter by `shutdown(w[i].sock, SHUT_RDWR)` (`wake.c:103`, `:106`), so its 5 to 27 µs close is the
same platform behaviour, and a real poller would need its own close path that his probe did not build.
The two differ where a shutdown does not wake: a Darwin listener ((g) a pipe per listener, the poller its
one wake) and Windows ((g) keeps slices, the poller a `WSAPoll` thread with a loopback pair, run on the
box at 1,732,250 cycles against the slices' 395,168,250, `ffi-pragmatist.md:521-526`). **So defect
2502 stays open on Windows under (g)**, which the compiler-engineer says (`:392-394`).

**A contradiction about the base, re-run.** The ffi-pragmatist's table puts *"slices of 50 ms (the
base's shape)"* at a close latency of **24,401 µs median** on Darwin and 51,360 µs on Linux
(`ffi-pragmatist.md:466`, `:499`), and his verdict compares every wake's 24 to 27 µs against it
(`:756`). **His slice shape never shuts the socket down**: `wake.c:99` sets a flag and the shapes other
than `tpipe`, `tefd`, `tkq`, `poller` and `sig` write nothing, so the waiter waits out its slice. The
base's `hero_net_close` shuts a busy socket down (`hero_net.c:1149`). Run here against my copy of the
base's own layer (the compiler-engineer's `closer.c`, copied, `critic/r2/closer/`, three runs): **a
waiting `recv` ends 133 to 139 µs after the close (worst 254 µs)**, a waiting `accept` **27,464 to
29,021 µs**; his `wake.c` copied, `slice 20 1`: *"median 50928 us"*. So at the base a connected socket's
close is already fast; what the slices cost is idle CPU and a Darwin listener's close, which the
compiler-engineer says (`:257-258`). The ffi-pragmatist's latency gain over *"the base"* is over a
shape the base does not have. (The base's own comment, `hero_net.c:35-38`, says *"11 to 13 ms"* for a
shutdown ending a `recv`; that figure stands beside both readings, measured 2026-11-12.)

**Agreed and measured by both**: under `ulimit -n 256` the base holds 250 server connections, (g) 248,
one poller 249, a pipe per waiting thread 83 (`compiler-engineer.md:262-268`, `ffi-pragmatist.md:485-491`).

## Q3: the door, and the two 2344 folder cases on Windows

Both approve the door in the `fchdir` shape; both measured `CreateProcessW` on the box (a relative
folder read in the parent's, a missing one 267, a bare program name found in the parent's folder).
They split on the two `fixedbugs-2344` folder cases: the ffi-pragmatist, *"cannot pass on Windows as
written, with the door or without"* (`ffi-pragmatist.md:604-608`, `:791-792`); the compiler-engineer,
*"if either fails it fails on clang's reading of `:`"* (`:641-642`).

**What the harness gives clang on Windows is not `:`.** `case_setting.hero:290-293` (`given_value`)
splits a folder list at `:` and joins it with `;` on Windows, and its own test asserts it, `given_value(name:
"C_INCLUDE_PATH", value: ":", root: "C:/w", windows: true) == ";"` (`:438`). The ffi-pragmatist ran
the raw `:`. **Run on the box** (`/c/w/p248-critic/q3/`, a `here/stdbool.h` defining `true` as 0, a file
including `<stdbool.h>`, clang started in `here`):

| `C_INCLUDE_PATH` | clang's job (`clang -###`) | `<stdbool.h>` read from (`clang -E`) |
|---|---|---|
| `;` (what the harness gives for `:`) | `-c-isystem "."` twice | **`.\stdbool.h`**, `int v = 0;` |
| `/nonexistent-2344;` (for `/nonexistent-2344:`) | `-c-isystem "/nonexistent-2344"`, `-c-isystem "."` | **`.\stdbool.h`**, `int v = 0;` |
| `:` (the raw value) | `-c-isystem ":"` | clang's own, `C:\Program Files\LLVM\lib\clang\23\include\stdbool.h`, `int v = 1;` |

So with the value the harness gives, clang on Windows names `.` and reads the case's header first, as on
POSIX, and with the `-c-isystem` words `selfhost/cli/clang_added.hero:281` reads; the compiler splits the
variable at `;` on Windows (`:296-310`, its comment *"unrun on Windows"*). **The ffi-pragmatist's ground
is falsified; the compiler-engineer's reason is too** (clang does not misread the harness's value).
What stays unrun is the compiler's own judgement on Windows and the two cases run there with the
door: owed to the CI's Windows leg, now with clang's half measured.

## Q4: a stop the program asks itself

- **Shared or apart.** The compiler-engineer built the door so the asked stop *"counts as the first
  request"* and a later Ctrl-C ends the process at once, *"no new code"* (`compiler-engineer.md:410-414`,
  measured exit 130). The ffi-pragmatist ran both designs on three systems and **objects to the shared
  one**, since a user's first Ctrl-C then kills a server draining its own stop (`ffi-pragmatist.md:640-653`,
  `:803-813`). The spec-warden ties the sentence to the choice: q4m (+18 real) if apart, q4s (+33) if
  shared (`spec-warden.md:256-258`, `:274-275`). **The built door is the shared one, so the
  spec-warden's own rule picks q4s for it, and the ffi-pragmatist objects to that door.** The synthesis
  has to choose the design before the sentence.
- **The spelling.** The blind sessions ran with `ask_stop() -> ()?` (every `t4-q` `task.md`, 10 of 10,
  `grep -h -o`), and the compiler-engineer built `net.ask_stop()` with no result in `core` 0.4.0
  (`compiler-engineer.md:441-444`), saying the sessions would be re-prepared to it; **they were not**:
  the forty ran on the card. Grading arm q *"with the compiler and runtime of the seat whose door the
  card spells"* (`llm-ergonomist.md`) now has no built door of that spelling: an answer writing
  `net.ask_stop()?` or `.must()` would be refused for a reason the card gave it. A face `-> ()?` must be
  built for the grading, or the arm graded against the card's own words.
- **Windows, the spec-warden's owed row, run.** The spec-warden wrote *"Windows, owed (the box is off)"*
  (`spec-warden.md:155`, `:422-423`); the box was on from 10:31 (both soundness seats ran there). Run
  now (`/c/w/p248-critic/q4/critic-raise.c` against the ffi-pragmatist's copy of the base's layer on the
  box, `/c/w/p248-fp/hero_net_door.c`, read only): unarmed, `raise(SIGTERM)` **exit 3**, the line after it
  never printed; armed, *"catch_stop answered 0"*, then `raise(SIGTERM)` **exit 3**, `stop_asked` never
  read 1. **So on Windows a program has no way to ask its own stop through any door of the base**, run,
  which is the measured need behind the door there; on POSIX the spec-warden's `kill`/`getpid` route
  exists (`sw-quit`, exit 0, both legs).

## Q5: the deadline's meaning

Three meanings stand in the sitting's files, and the sessions ran on the one no seat built:

- **The card the forty sessions read** (`t5-d` `task.md`, 10 of 10, hash `01cd81a9a476ec53`, line 34):
  *"From this call on, a `read` or `write` on `c` that waits more than `ms` milliseconds fails
  `timed_out`"*: a limit on each wait (Go's name dropped since my second pass).
- **The compiler-engineer's door** (`248-compiler-engineer/tree/runtime/platform/hero_net.c:1413-1425`):
  `at = now + ms` once, at the call; every wait that begins after it ends at that absolute time; *"a call
  already waiting keeps the deadline it began with"* (`compiler-engineer.md:463-466`). He argues for the
  absolute form against a per-call limit, which *"is defeated by a peer sending one byte just before each
  limit"* (`:483-486`). Under it, a connection whose thread sets 2 s once before `http.serve_conn` is cut
  2 s after the call however busy it is; the t5 task asks the server to close *"any connection on which
  it has waited 2 seconds without receiving a byte"*, the card's meaning.
- **The ffi-pragmatist's** (`ffi-pragmatist.md:824-826`): *"an absolute time on the system's monotonic
  clock kept in the socket's table entry"*.

The rubric's quiet connections (closed 1.5 to 4 s after opening) pass under either, so the grading may
not see the difference; a reader of the card writing a keep-alive server would. **The sitting has to
choose the meaning, and the card and the door have to say the same one**, before the t5-d arm is graded.

## The specification, and the paid calls

- **The spec-warden's real counts** (`spec-warden.md:122-137`): 10 `--refresh` invocations, 30
  requests, made *"on the coordinator's direction of 10:30"*, the seat saying *"I did not see the
  author's approval myself"* (`:111-113`). 00-shared.md now names those calls among the approved paid
  runs (`00-shared.md:17-27`); **but its own rules still say *"No paid run but the blind seat's ... no
  `heroes measure --refresh`"* (`:188-190`), and `spec-warden.md:38-44` still says the refresh is *"not
  run, by you or by the coordinator"***. The brief contradicts itself; the record should carry the
  approval in the brief the seat ran under, with the calls' size.
- **Q3's sentence**: the ffi-pragmatist says the door *"needs a spec sentence"* (`ffi-pragmatist.md:797-799`);
  the spec-warden vetoes q3a's sentence on Principle 0, the door at 0 tokens by panel 242's R6
  (`spec-warden.md:378-393`); the compiler-engineer defers to the spec-warden (`:645-647`). A
  disagreement the synthesis rules, with the spec-warden's veto standing until a measured need.
- **The marker's sentence**: the ffi-pragmatist names *"the reserved marker (SIGURG, the probe's already)
  is a sentence of `hero_os.h`, the spec-warden's to price"* (`ffi-pragmatist.md:739-740`); the
  spec-warden's report holds no row for it (`grep -c -i 'SIGURG\|marker' spec-warden.md`: 0). Unpriced.
- **The blind sessions**: 40 folders, 40 `run.json`, every one readable, sum **12.9748 USD**, the largest
  **0.5323** (`python3 -I` over `s/*/run.json`), past both the 0.40 cap and the 0.496 my second pass's
  worst case assumed; inside the author's 25. All forty hold the base's spec (`163219f2fa69fe82`, ten
  each of four `task.md` hashes): **the spec-warden's registered q4m spec arm was not among them**, so
  its prediction is unscored (`spec-warden.md:246-255`).

## The question the sitting should have asked, and did not

**What does each route change in what a program's own C observes, and which C libraries a Heroes
program plausibly binds depend on the thing changed?** Both Q1 routes are a change to C's behaviour at
the boundary, not only a guard: I-onstack changes the stack a library's handler runs on, I-mask
changes the thread's mask (a marker added during every handler, and a program's own block of the
marker dropped). The sitting measured each route on R3's cases and on the C library's own differential
test (the action and the mask *"with the marker aside"*, `ffi-pragmatist.md:387-389`), and found the
first library each route breaks by accident: bdwgc for I-onstack, found by the ffi-pragmatist after his
§§ 1.3 to 1.8; a program waiting for SIGURG for I-mask, found here. It never surveyed the libraries
that depend on either: collectors that read their own frame in a handler (bdwgc on Linux; on Darwin it
suspends by Mach calls), runtimes that *require* `SA_ONSTACK` of every other handler (Go, which throws
without it, `signal_unix.go:1126-1134`; ART, which forces it), and users of SIGURG (Go's preemption,
`signal_unix.go:74`; out-of-band TCP notices). A route is chosen against that list, and the list was
never made. Its corollary for I-mask, also unasked: **who owns SIGURG**, the runtime's probe signal
today and the marker under I-mask, which a Heroes program then cannot use through any C library, with
no sentence anywhere telling it so.

## The numbered list

1. **The veto's measurement holds.** Command: the ffi-pragmatist's `gcreal.c`, `inst.c`, `inst_mask.c` and
   bdwgc 8.2.8, built as his `phases.sh:146-153`, 5 runs each in Linux arm64 (`critic/r2/linux/r2.log`).
   Printed: base 5 of 5 *"intact after 301 collections"*; I-onstack 5 of 5 *"Segmentation fault"*, exit
   139; I-mask 5 of 5 intact. Repair: the synthesis records that this meets the compiler-engineer's own
   withdrawal condition (`compiler-engineer.md:586-589`), which his report never weighs (`grep -c -i
   bdwgc`: 0).
2. **bdwgc's failure is Linux's.** Command: `tar -xzOf gc-8.2.8.tar.gz` of `pthread_stop_world.c` and
   `darwin_stop_world.c`. Printed: the first compiled only `!defined(GC_DARWIN_THREADS)` (21-22); the second
   `thread_suspend` (536, 640), `thread_get_state` (192, 208). Repair: the veto names its platform; Darwin's
   I-onstack is not shown to break a collector, a question.
3. **I-mask breaks a program that waits for SIGURG.** Command: `swait.c` plain and with `inst_mask.c`.
   Printed: plain *"blocked ... 1"*, *"SIGURG taken"*; I-mask *"blocked ... 0"*, *"nothing in 2 s"*. Repair:
   the ffi-pragmatist's withdrawal condition (`:726-729`, *"or waited for"*) is met; the synthesis either
   stops dropping a program's own block of the marker outside a handler or says, in the escape's words,
   which programs the route refuses or breaks.
4. **Go uses SIGURG and requires `SA_ONSTACK`.** Command: `curl` of Go's `src/runtime/signal_unix.go` at
   `master`. Printed: line 74 `const sigPreempt = _SIGURG`; line 1133 *"non-Go code set up signal handler
   without SA_ONSTACK flag"*. With the historian's ART (*"always silently enable this"*), two runtimes want
   I-onstack's flag; neither soundness seat cites them (`grep -c`: 0 and 0). Repair: weigh a Go
   `c-archive` under both routes before choosing; what I-mask's dropped blocks do to Go is unrun.
5. **A handler that unblocks before calling back passes I-mask.** Command: `hunblock.c` with `inst_mask.c`.
   Printed: kept mask *"the marker blocked 1"*; `SIG_SETMASK` empty first **0**; `SIG_UNBLOCK` all first
   **0**. Repair: list it as I-mask's residue (a miss, the walk and D beneath it).
6. **An inherited SIGURG block is cleared by a side effect.** Command: `inh.c` with `inst_mask.c` under a
   parent blocking SIGURG; panel 247's `qsortcost` (n = 1,000) under the base and the ffi-pragmatist's
   I-mask runtime, plain and under that parent. Printed: the C model *"the marker blocked 1"* in a correct
   comparator; the real program `1`, exit 0, all four. By reading `sigguard_mask.c:213-222`, the probe's
   restore of the old mask goes through the route's own `pthread_sigmask`, which strikes SIGURG. Repair:
   write the clearing down as a deliberate step of the route (a constructor unblocking the marker), not
   a side effect of the probe.
7. **I-onstack leaves a measured Darwin residue of R3's class.** Command: read `compiler-engineer.md:150-151`,
   `:503`, `:580-581` and `247-*.md:191-195`. Printed: a two-level dylib *"5 of 5 `answer ran for 30`, exit
   0, not refused"*; R3 *"never a known cost"*. Repair: the synthesis says whether R3 admits I-onstack
   with that residue; the I-dylib the compiler-engineer calls unbuilt was built and run by the
   ffi-pragmatist (`ffi-pragmatist.md:57-58`, `:133-136`).
8. **I-mask on Darwin makes a program two files, and a missing one fatal.** Command: read
   `ffi-pragmatist.md:155-158`, `:59`, `:70-75`. Printed: *"no LC_RPATH's found"*, exit 134; *"different Team
   IDs"*, exit 134; `man dyld` 0 lines on interposing. Repair: price the dylib beside every program that
   hands C a function (install name, signing, its absence at launch), and record that it rests on an
   undocumented dyld mechanism.
9. **The ffi-pragmatist's "base" close latency is not the base's.** Command: his `wake.c` read (`:99-106`)
   and run, `slice 20 1`; the compiler-engineer's `closer.c` against my copy of the base layer, 3 runs.
   Printed: `wake.c` *"median 50928 us"*, its slice shape never shutting the socket down; the base layer
   `recv` 133 to 139 µs (worst 254), `accept` 27,464 to 29,021 µs. Repair: correct his table and verdict
   (`:466`, `:499`, `:756`): at the base a connected socket's close is already fast; the gain is idle CPU and
   a Darwin listener.
10. **Both Q2 routes rest on a shutdown waking a waiting connected socket.** Command: `wake.c:103`, `:106`
    (`shutdown(w[i].sock, SHUT_RDWR)` in the poller shape); `compiler-engineer.md:214-223`. Printed: the
    poller's measured wake is the shutdown. Repair: say both rest on it, unmeasured on Linux x86-64, and
    that a real poller's close path is unbuilt; (g) leaves 2502 open on Windows, the poller closes it there
    (run on the box).
11. **The two 2344 folder cases: the ffi-pragmatist's ground is falsified.** Commands: `case_setting.hero:290-293`
    and `:438`; on the box `clang -###` and `clang -E` with `C_INCLUDE_PATH` `;`, `/nonexistent-2344;` and `:`.
    Printed: the harness gives `;` for `:`; with `;` and `/nonexistent-2344;` clang adds `-c-isystem "."` and
    reads `.\stdbool.h` (`int v = 0`); only raw `:` reads clang's own. Repair: strike *"cannot pass on
    Windows ... with the door or without"* and the compiler-engineer's *"fails on clang's reading of `:`"*;
    the compiler's own split at `;` (`clang_added.hero:296-310`, *"unrun on Windows"*) and the cases' run with
    the door stay owed to the CI's leg.
12. **The built stop door shares the count; the ffi-pragmatist objects to that design.** Command: read
    `compiler-engineer.md:410-414`, `ffi-pragmatist.md:640-653`, `:803-813`, `spec-warden.md:256-258`. Printed:
    the asked stop *"counts as the first request"*; a first Ctrl-C then ends the process (exit 130, three
    systems); the spec-warden's q4s for that design. Repair: the synthesis chooses shared or apart first,
    then the sentence (q4s or q4m).
13. **The blind sessions ran a stop face nobody built.** Command: `grep -h -o` over `s/*/task.md`. Printed:
    `ask_stop() -> ()?` 10 times; the compiler-engineer's `net.ask_stop()` returns nothing
    (`compiler-engineer.md:441-444`), and the sessions were not re-prepared. Repair: build the card's `-> ()?`
    face for grading arm q, or grade it against the card's words and say so.
14. **The deadline has three meanings and the sessions read the one no seat built.** Command: line 34 of the
    `t5-d` `task.md` (`01cd81a9a476ec53`); `248-compiler-engineer/tree/runtime/platform/hero_net.c:1413-1425`;
    `ffi-pragmatist.md:824-826`. Printed: the card a limit on each wait; the compiler-engineer `at = now + ms`
    once, pending calls keeping theirs; the ffi-pragmatist an absolute time. Repair: choose one meaning, make
    card and door say it, before grading `t5-d`.
15. **On Windows a program cannot ask its own stop today.** Command: `/c/w/p248-critic/q4/critic-raise.c`
    against the base's Windows layer on the box, unarmed and armed. Printed: exit 3 both, *"catch_stop
    answered 0"*, `stop_asked` never 1. Repair: replace the spec-warden's *"Windows, owed (the box is off)"*
    (`spec-warden.md:155`, `:422-423`) with this run; the box was on from 10:31.
16. **00-shared.md contradicts itself on the refresh calls.** Command: `sed -n` of `00-shared.md:15-27` and
    `:186-190`, `spec-warden.md:36-44`. Printed: the calls approved inside the 25 USD; and *"No paid run but the
    blind seat's ... no `heroes measure --refresh`"*; and *"not run, by you or by the coordinator"*. Repair:
    amend 188-190 and the spec-warden's brief to the approval, with the calls' count (10 invocations, 30
    requests, `spec-warden.md:122-123`).
17. **Q3's sentence: the ffi-pragmatist and the spec-warden disagree.** Command: read `ffi-pragmatist.md:797-799`
    and `spec-warden.md:378-393`. Printed: *"Needs a spec sentence"*; *"veto on the sentence"*, the door at 0
    tokens by panel 242's R6. Repair: the synthesis rules it; the veto stands until a measured need.
18. **The marker's sentence is unpriced.** Command: `grep -c -i -E 'SIGURG|marker' spec-warden.md`. Printed: 0,
    where `ffi-pragmatist.md:739-740` hands it to the spec-warden. Repair: price it, or say why none is owed,
    with item 3's finding beside it.
19. **The spec-warden's registered arm did not run, and the largest session passed the assumed worst case.**
    Command: `shasum` of every `s/*/heroes-spec.md` and `task.md`; `python3 -I` over `s/*/run.json`. Printed:
    40 sessions, all the base's spec, ten each of four tasks; sum 12.9748 USD, largest 0.5323. Repair: record
    the q4m prediction (`spec-warden.md:246-255`) as unscored, and a per-session worst case of at least 0.5323.
20. **The question the sitting should have asked**: what each route changes in what a program's own C
    observes, and which libraries a Heroes program plausibly binds depend on it (collectors reading their
    own frame; Go and ART requiring `SA_ONSTACK`; users of SIGURG), and who owns SIGURG. Command: items 1 to
    6. Repair: make that list before choosing between I-onstack and I-mask, and run each route against its
    first entries.

Finished 2026-11-14 (`date` below). Nothing of mine left running: `pgrep -f p248-scratch/critic` empty,
`docker ps --filter name=critic-248` empty, the box's runs ended (`/c/w/p248-critic/` holds `q3/` and `q4/`).

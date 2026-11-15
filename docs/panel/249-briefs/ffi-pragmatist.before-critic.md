# Panel 249, ffi-pragmatist

Read `00-shared.md` first, whole: the base (`722ae76bf`, `sources 2ab7ca0a644692a1`), the rules (**your
binaries are named `fp-...`, your containers `fp-249-<n>`**, every program that may not end has its
bounds there, **no `brew install`, no `docker pull`, `apt-get` only inside a `docker run --rm`, Go only
with its writes moved into your folder**), the platforms (**the Windows box is off**), the lane and **the
escape**, and the three questions with every fact this brief leans on. You judge from the C side, with a
veto on ABI breakage and on soundness at the boundary. Your folder is
`.claude/worktrees/p249-scratch/249-ffi-pragmatist/`, your tree `tree/` in it, your compiler built there
from the seed in two stages. Your report is `docs/panel/249-reports/ffi-pragmatist.md` in
`.claude/worktrees/p249`, in your definition's output structure (`.claude/agents/ffi-pragmatist.md`, its
line 33), one section per question, written as you go. **Compile, don't opine**: every mechanism below
is C you write, compile and run on this Mac and in Linux arm64 by the Docker line in 00-shared.md; a
Windows arm is named owed, never inferred.

**Your share of the building**: **you run the census (question 1) and build candidate A** (the owned
marker) **and D-tramp** (Darwin's trampoline); the compiler-engineer builds C and B' into the runtime.
Each of you prices the others' from your own side, and reads the other's report before your verdict if
it is there.

**Read first**: panel 248's R1 (`docs/panel/248-*.md:177-226`); your seat's 248 report, §§ 1.2, 1.3,
1.9, 1.10 and 1.12 (`docs/panel/248-reports/ffi-pragmatist.md:50-151`, `:319-408`, `:424-456`) and its
verdict (`:693-745`); 248's critic over the reports, its Q1 section and items 1 to 8 and 20
(`docs/panel/248-reports/critic-reports.md:14-103`, `:250-289`, `:341-345`); the probes copied under
`briefs/inputs/` (00-shared.md lists them with their hashes). **Already measured, so re-run only where
you lean on it**: R3's two cases on this Mac at this base (00-shared.md); the facts read of Go 1.27.3,
bdwgc 8.2.8, glibc 2.41's real-time range and Darwin's signals; the installed and installable libraries;
the Mac libraries' imports.

## Question 1: the census

For each library below, on each POSIX leg where it builds or installs (00-shared.md's table says
where), **first its own signal use at the base, then the same under each candidate**:

1. **Its own signal use, at the base, measured**: a probe that reads every signal's action
   (`sigaction(sig, NULL, &old)` for 1 to `NSIG - 1`: handler, flags, mask) and the thread's mask before
   and after the library starts, on the thread that started it and on another; which signals it then
   blocks, waits for (`sigwait`, `sigtimedwait`, `signalfd`) or sends; whether its handler runs on an
   alternate stack or asks for one; whether its handler calls back into code the program handed it; and
   whether it walks or reads its own stack in a handler. Its imports (00-shared.md's census seed, and
   `nm -D --undefined-only` of the Linux libraries, which the coordinator ran, 00-shared.md)
   say where to look and nothing more.
2. **The first entries, and what each asks**:
   - **bdwgc**: Linux arm64 by 248's `gcreal.c` and `gc-8.2.8.tar.gz` (built as `phases.sh` builds it),
     **and the Debian `libgc1 1:8.2.8-1` the image carries** (with `libgc-dev` from `apt-get`): whether
     Debian builds it `GC_USESIGRT_SIGNALS`, which moves its signals from SIGPWR and SIGXCPU to
     `SIGRTMIN + 6` and `+ 5` (00-shared.md, read from `gc_priv.h` and `pthread_stop_world.c`; its
     imports name no `__libc_current_sigrtmin`, which reads as *no*, an inference to run); and on
     this Mac, built from the tarball in your folder, whose stop-the-world is Mach's by the reading of
     `darwin_stop_world.c`, unrun: run it, and ask whether its incremental mode or another path takes a
     signal there.
   - **A Go `c-archive`**: on this Mac (`go1.27.3`, the environment of 00-shared.md, `-buildmode=c-archive`
     linked into a Heroes program through a `--library` folder) and in Linux arm64 (`golang-go` 1.24 by
     `apt-get` in the container). Go 1.27.3's archive installs a handler for SIGURG, its preemption
     signal (`runtime/signal_unix.go:74`, `:178-182`), and turns `SA_ONSTACK` on in a handler it finds
     (`os/signal/doc.go:195-200`): drive a goroutine-heavy loop that is preempted, an `os/signal.Notify`
     of an asynchronous signal, and a callback of the program's from Go, at the base and under each
     candidate; and what the runtime's SIGURG probe at the first callback does beside Go's SIGURG handler.
   - **libuv**: `uv_signal_t` delivered to a loop (its imports on this Mac name `sigaction`, `signal`,
     `pthread_sigmask`, `sigprocmask`, `kill`); the tree's `fixedbugs-413-libuv-keeps-every-handles-address`
     binds it (`pkgconfig "libuv"`).
   - **OpenSSL** (libcrypto 3.5.9 and 4.1.0 on this Mac, 3.5.7 in the image): what its start-up installs
     and why (its imports name `sigaction` and `signal`), on each platform; whether it leaves a handler by
     `siglongjmp` (a shape every candidate must not refuse).
   - **SDL** (SDL3 3.4.20 on this Mac; `libsdl3-dev` 3.2.10 by `apt-get` in the image): what its start-up
     installs for SIGINT and SIGTERM with no display (an `SDL_Init` of events alone), and whether a hint
     turns it off; and **which real-time signal it takes on Linux and why**, since both Linux SDLs import
     `__libc_current_sigrtmin`, `__libc_current_sigrtmax` and `sigtimedwait` (00-shared.md), which bears
     on candidate A's marker.
   - **sqlite**: the control. Its Homebrew dylib imports none of the names (00-shared.md); say whether
     the Linux library does, and that a candidate leaves a program with callbacks no signal reaches
     untouched.
   - **A program that owns a signal for itself**: 248's critic's `swait.c` (SIGURG blocked and waited for)
     with the marker of each candidate in its place, and a real-time signal taken by `SIGRTMIN + n`.
   - **ART** cannot be built here: the historian reads it.
3. **Under each candidate**, the same probes, on the four points of 00-shared.md: A (your build), B' and
   C (the compiler-engineer's builds, or your C model of each if theirs is not yet there, said so), and,
   for comparison, I-onstack and I-mask as 248 built them. A row that breaks names the library, the
   platform, the probe and its output.
4. **Who owns SIGURG** at this base, and under each candidate, measured with a Go archive in the program.

## Question 2, candidate A: a marker the runtime owns

1. **Linux, a real-time signal**: rebuild 248's I-mask (`briefs/inputs/p248-fp/inst_mask.c`,
   `siginstall_mask.c`, `sigguard_mask.c`) with the marker a real-time signal **chosen at run time** (say
   how: a fixed offset from `SIGRTMIN`, the highest free one, or one asked of the system), the `longjmp`
   family defined as your 248 route had it (`248-reports/ffi-pragmatist.md:399-407`); then R3's two cases
   20 of 20 each, armed runs counted, the shared-library and `dlopen` rows, `--sanitize` at `-O0` and
   `-O2`, 247's critic's shapes (`direct`, `dlocale`, `hold`; their Linux variants as your 248 report
   made them, `:212-219`), and **248's critic's three residues re-run with the new marker** (`swait.c`,
   `hunblock.c`, `inh.c`, `briefs/inputs/p248-critic/`), and the census's first entries (bdwgc, Debian's
   included, and the Go archive).
2. **Darwin: what then.** No real-time signal exists (00-shared.md). Say, measured against the census:
   which classic signal (if any) no entry blocks, waits for, sends or installs on this Mac, asked of each
   entry's imports, its source at a named version and a run, never from memory; or a mark that is not a
   signal (D-tramp below); and **the price of any Darwin arm that must reach a two-level dylib**: your 248
   route needed a dylib beside every program that hands C a function, `heroes build` gives a program no
   `LC_RPATH` (*"no LC_RPATH's found"*, exit 134) and the hardened runtime refused one not signed by
   the program's Team ID (*"different Team IDs"*, exit 134), resting on an `__interpose` that `man dyld` does not document
   (`248-reports/ffi-pragmatist.md:59`, `:70-75`, `:155-158`; 248's critic item 8,
   `critic-reports.md:89-98`, carried). Price it now: the install name a landing would choose, what a
   program that is one file today becomes, what a missing dylib, or one signed by another Team ID, does at launch.
3. **Cost**: one mask question a callback (your 248 count, 1,346 instructions on Darwin, `:381-385`,
   carried; re-run it with the new marker).

## Question 2, D-tramp: Darwin's trampoline of the runtime's

Posed and unbuilt (00-shared.md § Question 2, D): the kernel's `struct __sigaction` carries `sa_tramp`
(`sys/signal.h:277-282` in the SDK). **Build it far enough to judge**: an interposer of libsystem_kernel's
`__sigaction` (your 248 one-name I-dylib, `briefs/inputs/p248-fp/darwin/ipose_kernel.c`, 23 lines, is
its frame) that hands the kernel a trampoline of the runtime's in place of the one the C library passes,
the trampoline keeping a per-thread depth around the program's own handler and returning as
`_sigtramp` does. Run: whether the kernel accepts it and every image's installer reaches it (your 248
rows, static archive and two-level dylib, `248-reports/ffi-pragmatist.md:130-136`); R3's two cases 20 of
20; `sigaction(sig, NULL, &old)` answering the program's handler; `siglongjmp` and `_longjmp` out of a
handler, then a correct callback (the depth question of B'); what the exact test learns (its probes
install through the C library's own `sigaction`, `sigguard.c:175-186`: do they then return into
`_sigtramp` while the program's handlers return into the runtime's trampoline?); and the census's point
(4), what a handler that reads its stack finds where `_sigtramp`'s frame stood. Its price is the I-dylib's
(question 2's Darwin arm, above).

## Question 2, candidates B' and C, from the C side

- **B'**: what a wrapper does to each census library's handler (its frames, its `siginfo_t` and
  `ucontext_t`, its flags' meaning) and to a library that installs by a raw system call (Linux
  `syscall(SYS_rt_sigaction, ...)`, which no definition sees: run it under B' and under A); and whether
  `prctl(PR_SET_SYSCALL_USER_DISPATCH)` (in the image's headers, `/usr/include/linux/prctl.h:253`) could see
  that call, a question you may answer by reading its manual page and say so.
- **C**: what a library built apart keeps or does not keep: R3's library's handler disassembled (your
  248 report showed `stp x29, x30` with no `mov x29, sp` on arm64 with `-fomit-frame-pointer`, `:35-36`,
  carried), and whether any census library as shipped (Homebrew's, Debian's) keeps frame records in its
  handlers, by `objdump -d` of the handler you find.

## Question 3, from the C side

1. **D-imports from the library's side**: how many of the census libraries, and of the libraries the
   tree binds (00-shared.md's enumeration), import an installer, on each platform (`nm -u` on this Mac,
   `nm -D --undefined-only` in the image, the commands named); which of those install a handler that
   could call back into the program, read from their source or run; what a reading of imports misses (a
   library `dlopen`ed by another, a raw system call, a static archive's object pulled in only by a
   symbol).
2. **What a binding's author must learn** under each answer (a reserved signal, a file beside the
   program, a library refused): say so in the escape's words.
3. **Whether any answer short of a route closes 2522 under R3**, or needs the author's change to R3,
   said in those words.

## What your verdict owes

For each candidate: the C it needs on each platform, compiled, and run where the platform is here; what
it does to programs and libraries that run today (the census rows, each with its probe and output); what
it closes on Darwin (a refusal with R1's words before the callback's first C call) and on Linux (no
Heroes code inside the signal), and its residue; every crash report its storms left (armed runs alone
counted); the Windows row named owed; whether it needs a spec sentence, a new or widened diagnostic, or
a change a binding's author must learn (the escape); and a veto, if you cast one, names the program
that corrupts memory, dies in silence or breaks the boundary under the route vetoed, compiled and run.

**Linux arm64**: `linux-249.sh` (and more, numbered) in your folder, run by the Docker line in
00-shared.md, every probe inside under its own `timeout -s KILL` and its output bounded by `head -c`;
`apt-get update` and `apt-get install` inside the container only; the logs are part of your report's
evidence and stay in your folder.

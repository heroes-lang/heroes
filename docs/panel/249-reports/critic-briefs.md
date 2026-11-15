# Panel 249, the completeness critic's first pass, over the briefs

Started 2026-11-14 (`date` at the head of the work); written as it goes. Worktree
`.claude/worktrees/p249` at `722ae76bf`; my folder `.claude/worktrees/p249-scratch/critic/`.
Brief: `docs/panel/249-briefs/critic-briefs.md`, read whole; then `00-shared.md`,
`compiler-engineer.md`, `ffi-pragmatist.md`, `historian.md`, read whole; `.claude/skills/panel/SKILL.md`
§ 2 and § 3c read whole. No verdict on the routes is given here.

Sections A to E, then the numbered list; § B is a breach of mine, reported for the author.

## A. What I verified and found to hold (each with its command)

- **The base**: `git rev-parse HEAD 'HEAD^{tree}'` in the p249 worktree printed `722ae76bf693bebdb7a5cca37052ed92646753c0`
  and `ce81c0b18b1fbeb616f95aeef619d5b8cd23bfbe`; `git log -1` the subject and `2026-11-14 21:55:01 +0100`;
  `git log --oneline df7612ad3..722ae76bf | wc -l` 37; `git diff --numstat df7612ad3 722ae76bf -- runtime/`
  7 files, `hero_net.c` 1104/137, `run.c` 406/64, `alloc.c` 12/4; `git status --short` at 22:46 printed two
  untracked folders, `docs/panel/249-briefs/` and `docs/panel/249-reports/` (the briefs themselves and this
  report), so "0 lines at 22:08" was true before the briefs were written into the tree.
- **The guard did not move**: `git diff --stat df7612ad3 722ae76bf -- runtime/parts/sigguard.c ... supplied.hero`
  (the nine paths 00-shared.md names) printed **nothing**; `alloc.c`'s 12/4 is the leak check's
  `hero_run_folders_held_at_exit` (defect 2490), read with `git diff`. See item 1 of the list for the wording.
- **Line counts**: `wc -l` printed every count the briefs give (sigguard.c 364, sigdoor.c 137, stack.c 1213,
  alloc.c 1548, thread.c 144, run.c 2446, hero_net.c 2202, runtime.c 315, callback_thunk.hero 343,
  callback_guard.hero 207, flags.hero 584, link_flags.hero 215, supplied.hero 357, suite_run.hero 740,
  c-boundary.md 398; spec 492 lines and 29,216 bytes); `ls selfhost/cli | grep -c supplied` 12;
  `ls tests/golden/run | grep -c -E '\.hero$'` 795; the `fixedbugs-2494-` count 14.
- **The guard's lines**: `grep -n` over sigguard.c printed the remedy at 87, `HERO_SIGNAL_WALK` 156, the
  probe's `sigaction(SIGURG` at 183 and 185, `hero_signal_learn` 196, the mask let through and put back at
  207 and 216, `hero_callback_entered` 316, the exact test 320, the thread refusal 324, `SS_ONSTACK` 332,
  the walk 338; `sed -n 994,1010p stack.c` the 256 KiB at 1001 and `sigaltstack` at 1009;
  `callback_thunk.hero:146` the thunk's first line and `:216` `hero_signal_learn();`; `flags()` 19 words,
  none a frame word; `grep -rn -E 'frame-pointer|omit-frame|unwind-tables' selfhost/ runtime/ tests/harness/`
  the two comments only.
- **The CI's reds** (`briefs/ci/job-132377462166.log`, 475,097 bytes, 4,385 lines): line 4112
  `run: 803 passed, 3 failed`; `grep -c 'at -O2'` 3, at lines 4315, 4326, 4337; `grep -c -E 'FAIL run/.* at -O0'`
  0; line 1329 `Ubuntu clang version 21.1.8 (6ubuntu1)`; `gh api .../jobs/132377462166` printed
  `Linux x86-64`, `failure`, `df7612ad3dc6...`, `ubuntu-26.04`. The fork case's actual output also holds
  `the parent goes on`, `raised: 0`, `after the signal` (lines 4344-4349), which 00-shared.md shortens.
- **Item 4, the other two handlers, disassembled the coordinator's way** (`critic/fp/run.sh`, the three
  case headers copied from the tree, drivers that call each `_install` so the static handler is emitted;
  `docker run --rm --name critic-249-1` / `critic-249-2`, both exit 0, `docker ps` empty after each; logs
  `critic/fp/arm64.log`, `critic/fp/x86_64.log`):
  - x86-64, Debian clang 22.1.8 (emulated): `leaving_handler` (exit case) at `-O2` is `push %rax`,
    `xor %edi,%edi`, `call exit`: **no record**; `forking_handler` at `-O2` `push %rax`, `call fork`, ...,
    `pop %rax`, `ret`: **no record**; with `-O2 -fno-omit-frame-pointer` both open `push %rbp` /
    `mov %rsp,%rbp`; at `-O0` both keep it. `called_handler` reproduced the coordinator's reading.
  - aarch64, Debian clang 22.1.8: all three handlers keep `stp x29, x30` / `mov x29, sp` (or `add x29, sp,
    #0x10`) at `-O0`, `-O2` and `-O2 -fno-omit-frame-pointer`.
  - **So the same shape holds for all three cases**, by disassembly of clang 22.1.8; the CI's clang 21.1.8
    is still not the one read (item 4 of the list).

## B. A file written outside the repository's root, by me, for the author

At about 22:54 I ran `go version` (to name the toolchain whose sources I was reading) **without** the
environment block 00-shared.md gives, `HOME` unmoved. Go's telemetry then wrote under the home directory:
`stat -f 'born %SB modified %Sm'` prints **born 2026-11-14 22:54:53** for
`~/Library/Application Support/go/telemetry/local/go@go1.27.3-go1.27.3-darwin-arm64-2026-11-14.v1.count`
(16,384 bytes), `local.2026-11-13.json` (2,332 bytes) and `upload.token` (0 bytes), and the folder's
modification time moved to 22:54. **This breaks CLAUDE.md § Hard stops (no file outside the root); it is
mine, not the coordinator's**: those three files were born at my call, and before it the coordinator's
check (`find ~/Library/Application\ Support/go -newer briefs/go/src/go.mod`, nothing) held. I deleted
nothing (a removal outside the root is the author's to ask for). It is also a finding about the brief's
rule (item 9 of the list): `go version` alone writes there, so the environment block has to come before
**every** `go` command, `go version` and `go env` included, not only `go build`.

## C. Found by running: the walk through the C library's own frames (bears on candidate C and on the emulated image)

`critic/glibc/walk.c` models R1's walk exactly as `sigguard.c:333-345` does it (the thread's stack read with
`pthread_getattr_np`, a SIGURG probe learning the delivery's return address through the C library's own
`sigaction`, then at most 8 frame records climbed from the callee's `__builtin_frame_address(0)` with the
same three bounds), for three shapes: a handler that calls the function directly (`c`, the shape of
`fixedbugs-2494-...-calls-is-refused`), a handler that calls `exit` with the function in `atexit`'s list
(`e`), and one that calls `fork` with it in `pthread_atfork`'s (`f`). Built with the `run` form's words
(`-std=gnu11 ... -fno-optimize-sibling-calls`) at `-O0`, `-O2` and `-O2 -fno-omit-frame-pointer`;
`docker run --rm --name critic-249-8` (emulated x86-64) and `critic-249-9` (Linux arm64), both exit 0,
`docker ps` empty after each; logs `critic/glibc/walk-x86_64.log` and `walk-aarch64.log`:

| image (libc6 2.41-12+deb13u3, Debian clang 22.1.8) | `c` -O0 | `c` -O2 | `c` -O2 + word | `e` any level | `f` any level |
|---|---|---|---|---|---|
| x86-64, emulated | found, step 0 | **ended at step 0** | found, step 0 | **ended at step 0** at all three | **ended at step 0** at all three |
| aarch64 | found, step 0 | found, step 0 | found, step 0 | found, step 2 at all three | found, step 2 at all three |

The `c` column reproduces the CI's pattern for the `calls` case (passes at `-O0`, red at `-O2`) and the
word's predicted effect, which is what makes the model a fair stand-in. **The `e` and `f` columns say
that through Debian's x86-64 glibc the walk ends at its first step whatever the program's own flags**:
the frame the callee finds above it is `__run_exit_handlers`' or the fork handlers', and its saved `%rbp`
is no frame record. So, by this model and unrun with the Heroes cases:

- the emulated image will probably read the `atexit` and `fork` cases **red at `-O0` too**, where the
  CI's log reads them green at `-O0` (`grep -c -E 'FAIL run/.* at -O0'` 0). The compiler-engineer's brief
  already says what follows if the image does not reproduce the CI ("not this question's instrument");
  the brief should say beforehand that this is the expected outcome for two of the three, and why;
- **candidate C reaches the program's C and the runtime, never the C library's own frames between a
  handler and the callback** (`exit`, `fork`, and by the same reasoning `qsort` and any libc function
  that calls back). On a distribution whose x86-64 libc keeps no frame records, C closes the `calls`
  case and leaves the other two; the CI's Ubuntu passes them at `-O0`, so its libc keeps the chain, an
  inference (whose policy page is the historian's to read) and unrun here: no Ubuntu image is on this
  Mac and none may be pulled;
- **the tree's own `atexit` and `fork` cases would then run the Heroes function inside the signal on a
  Debian x86-64 machine at every level today**, which is 2522's class reached through the C library
  itself, not through a library the program's author built. No CI leg runs Debian x86-64. This is a
  question for the compiler-engineer's step 1 (run the two cases at `-O0` in the emulated image and say
  which level fails), and for question 3's class.

## D. My brief's ten items, answered

### Item 1, the lane

What the texts say, read today: `SKILL.md:18-23`, the soundness lane is for a proposal that changes *"no
surface, no diagnostic and no spec token"*; `SKILL.md:33-34`, *"A question that looks internal but has a
sentence in the spec behind it is a full panel"*; CLAUDE.md § 4 (lines 184-186) names *"a diagnostic
**class**"* and *"architecture"* as triggers. Measured against the three things my brief names:

- **Question 3** asks, in 00-shared.md's own words (lines 600-603), *"which class of programs the runtime
  or the compiler refuses or tells, on which platform, with which words"*. Every answer but "nothing
  changes" is a refusal class or its words, and spec § 13's sentence stands behind it (`sed -n 403,404p`:
  *"A callback is a **parameter**, never a result or a signal handler, nor called by one ... No function of
  the program is a signal handler"*). That is the shape `SKILL.md:33-34` names, and the ground on which
  248's critic escalated Q4 before any seat ran (`248-reports/critic-briefs.md:601-605`, read today: *"spec
  line 404 names both stop doors ... escalate Q4 now rather than by a seat's report"*).
- **D-imports** is *"a **new diagnostic** if it lands"* in 00-shared.md's own words (line 579): CLAUDE.md
  § 4's *"a diagnostic class"*.
- **`ffi_defined_elsewhere`'s widening** under A and B' on Linux is not a contingency a seat might find:
  both of 248's building seats measured it for any route that makes the runtime define a C library name
  (carried by the brief), and A and B' define the installers by name on Linux by construction. The
  escape's condition is therefore met in advance for two of the four routes the seats are told to build.
- **For the lane**: panel 247's R4 asks *"a soundness sitting's ruling"* on the runtime defining C library
  names (`247-*.md:207-211`), so the architecture half matches the lane; and `SKILL.md` has no step that
  convenes seats at the synthesis: the escape is this brief's mechanism, not the skill's, and seats
  convened then would get briefs no critic's first pass has read (`SKILL.md:269-273`).
- **What escalating costs, and who must say yes**: the blind seat runs as `claude -p` sessions with a
  `--max-budget-usd` (`SKILL.md:160-166`), a paid run, which stays the author's every time
  (`.claude/rules/verification.md` § The batch, the four kinds); and a spec-warden's real count of a new
  sentence is `heroes measure --refresh`, a call to `count_tokens` (`selfhost/cli/measure.hero:72-79`),
  which 00-shared.md lists under *no paid run*. So either way the coordinator must name those runs and ask
  the author; the brief should say so rather than leave it to the synthesis.
- **What the two seats would be given**: the spec-warden, spec § 13 lines 403-404 before and after each
  draft question 3 could produce (a told residue; a refusal widened to a named class; R1's words with
  defect 2533's correction, below), the vendored count (`heroes measure`, offline) and the real one by a
  named `--refresh`; the blind seat, a copy of the spec outside the repository, those drafts as spec
  diffs, and one to three tasks of the 2522 shape (a program binding a library whose handler calls back),
  scored against a registered prediction, with a budget the author approves.

I give no verdict on whether to escalate; by the text of `SKILL.md:33-34` and CLAUDE.md § 4, question 3
and D-imports meet the full panel's trigger as posed, and the brief's escape does not say why they wait.

### Item 2, the census's frame

The four points are the stack, the mask, the signal numbers and the frames. What a library's own C
observes that none of them names, each with what made it concrete today:

- **(5) The action a query answers, and the order handlers are installed and chained in.** Go saves every
  handler it finds at start-up and forwards to it (`runtime/signal_unix.go:135`, `fwdSig[i] = getsig(i)`,
  Go 1.27.3), installs its own from a constructor before `main` (a c-archive's), and turns on `SA_ONSTACK`
  in a handler already there (`os/signal/doc.go:195-198`); the runtime's own stack guard saves the
  handler it finds and chains a fault not its own to it (`runtime/parts/stack.c:670-685`, `:1054-1055`).
  B' answers `sigaction(sig, NULL, &old)` from a table, D-tramp from the kernel, A unchanged: three
  different answers. The B' row's cell (4) of the table holds this today (*"`sigaction(sig, NULL, &old)`
  through a raw system call answers the wrapper"*), which is not a frame: it needs its own column.
  **And every definitions route (A, B') is reached from a constructor before the runtime has started**
  (a Go archive's, a C++ static initialiser's): whether its table, key and marker work then is a question
  no brief asks.
- **(6) The context a handler reads and writes.** Go preempts by editing the interrupted context
  (`doSigPreempt`, `signal_unix.go:342-348`, `ctxt.pushCall(asyncPreempt, newpc)`) and turns a fault into a
  panic the same way (`preparePanic`, `signal_arm64.go:62`, `signal_amd64.go:49`). A wrapper (B') or a
  trampoline (D-tramp) must hand the handler the kernel's own `ucontext_t`, or Go's preemption and panics
  silently stop. The ffi-pragmatist's brief names `siginfo_t` and `ucontext_t` under B' (its line 125);
  the shared frame does not.
- **(7) `errno` and `SA_RESTART` as the interrupted code sees them.** A wrapper or trampoline that makes a
  call before or after the program's handler must keep `errno`; the flags the kernel holds decide whether
  an interrupted `read` restarts or returns `EINTR`, so a wrapper installed with other flags than the
  program asked changes the program outside any handler. Debian's bdwgc installs both its handlers with
  `SA_RESTART` (below), as `pthread_stop_world.c:1408-1410` reads.
- **(8) Which thread a process-directed signal reaches.** A route that changes masks (A) changes which
  thread the kernel picks; the runtime's own poller thread is born with every signal blocked
  (`hero_net.c:1115-1122`, read today); a library that blocks signals in threads of its own so that
  another thread takes them depends on that; which census library does so is the census's to run, and
  is a question here.

The table's cells I read against their citations hold (`248-reports/ffi-pragmatist.md:432-436` the 5 of
5; `:349-363` I-mask keeping the handler's stack; `critic-reports.md:40-50` and `:66-78`). Two cells
assert more than they cite: **C's cell (4)**, *"a record in every frame of the program's own C and of the
runtime"*, is x86-64's only (arm64 keeps records at `-O2` already, the coordinator's `objdump` and mine),
and leaves out the C library's own frames between a handler and the callback, which § C above measured
as no record on Debian's x86-64 glibc; **A's cells** are written for one marker on every platform, where
Darwin has no real-time signal (the brief's own § A): A's row needs a Darwin cell of its own.

### Item 3, the libraries' list

Asked of the world, not memory, three ways:

- **The tree's own bindings the brief enumerated and then left out of its first entries**: `curl` (2) and
  `ncurses` (1). `nm -u` of Homebrew's `libncursesw.6.dylib` (ncurses 6.6) prints `_kill _sigaction
  _sigprocmask`; Debian's `libncursesw.so.6` (6.5) `dlopen dlsym kill sigaction sigprocmask`; Debian's
  `libcurl.so.4` (8.14.1) imports `sigaction`, where Homebrew's `libcurl.4.dylib` (8.23.0) imports none of
  the names: a platform difference in a library the tree binds. `raylib` (15 as `pkgconfig`, the most
  bound) imports `_dlopen` and no installer on this Mac (6.2); `json-c` 0.19 none; `zlib` and `libiodbc`
  are not installed under `/opt/homebrew/opt` (`ls`), and `libiodbc2` is a Debian candidate (3.52.9).
- **Every Homebrew dylib on this Mac**: `readlink -f /opt/homebrew/lib/*.dylib | sort -u` 357 files, 25
  of which `nm -u` shows importing `_sigaction`, `_signal` or a compat installer
  (`critic/world/mac-installers.txt`): Abseil's `libabsl_failure_signal_handler` (a crash reporter),
  Boost's test monitor, GLib's `libglib-2.0` and `libgio-2.0`, Node's `libnode.147`, NSPR's `libnspr4`,
  `libgpgme`, `libassuan`, `libnpth`, `libdbus-1`, `libssh`, `libslirp`, Ghostscript's four, `libmpg123`,
  gettext's two, `libexpect`, `libuv`, `libcrypto.4` and `libSDL3`.
- **Every shared library installed in the Linux arm64 image**: 155 files under `/usr/lib/aarch64-linux-gnu`
  (the script scanned `/lib` too, which is the same folder there, so its log prints each twice: 310 and 58),
  29 importing an installer (`critic/linux/linux.log`): `libcurl`, `libncurses`/`libncursesw`,
  `libevent-2.1`, `libpython3.13`, `libperl`, `libsystemd`, `libreadline`, `libedit`, `libkrb5`,
  `libgpm`, `libmagic`, `libbsd`, `libassuan`, `libnpth`, `libpam_misc`, `libpipeline`, `libsemanage`,
  `libz3`, `libapt-pkg`, the sanitizer runtimes `libhwasan` and `libubsan`, `libLLVM`, `liblldb`,
  `libgprofng`, `libcrypto`, `libgc`, `libuv`.

So a reader would add, from those lists: **ncurses and curl** (bound by the tree), **a crash reporter**
(Abseil's is on this Mac; the historian's brief names Breakpad and Crashpad from memory), **GLib and
libevent** (event loops that install handlers, beside libuv), **an embedded interpreter** (libpython,
libperl, Node), and **the sanitizer runtimes other than ASan** (UBSan, HWASan). None of these was run.

### Item 6, facts read from sources

- **Go 1.24 at Debian's version, read** (`critic-249-3`: `apt-get install golang-1.24-src`, `1.24.4-1`,
  the candidate; `/usr/share/go-1.24/src`): `const sigPreempt = _SIGURG` at **73**, the c-archive clause
  at **177-179**, the throw at **1116**, `doc.go`'s *"must use the SA_ONSTACK flag"* at **128-129** and
  *"will turn on the SA_ONSTACK"* at **196**, `sigtab_linux_generic.go` 42-44 the same, and
  `cgo_sigaction.go:34` the same `_cgo_sigaction == nil || inForkedChild` test. **The clause stands at
  1.24.4; the line numbers the briefs cite are 1.27.3's** (74, 178-182, 1124, 129-133, 195-200) and are
  off by one to eight lines at 1.24.4. Note also that Debian's `golang-go 2:1.24~2` is a metapackage;
  the source is `golang-1.24-src 1.24.4-1`.
- **Debian's bdwgc, run** (`critic/linux/gcsig.c`, `GC_INIT()` with `GC_THREADS`, built against
  `libgc-dev 1:8.2.8-1` in `critic-249-3`, exit 0): `GC_get_suspend_signal 30 GC_get_thr_restart_signal
  24`, SIGPWR and SIGXCPU; `sig 24 ... flags 0x10000000 restart 1 onstack 0 siginfo 0` and `sig 30 ...
  flags 0x10000004 restart 1 onstack 0 siginfo 1`. **Debian builds `libgc1` without
  `GC_USESIGRT_SIGNALS`**: the brief's inference from its imports holds, now by a run.
- **Go's installers**: on Linux a c-archive installs through libc's `sigaction`
  (`runtime/cgo/gcc_sigaction.c:63`) unless in a forked child or without cgo (`cgo_sigaction.go:36-37`,
  `sysSigaction`, a raw system call); on Darwin through libSystem's (`sys_darwin.go:326-327`, `:607`,
  `cgo_import_dynamic libc_sigaction sigaction "/usr/lib/libSystem.B.dylib"`). `nm -u` of the
  coordinator's `briefs/go/src/libgoanswer.a` prints `_sigaction`, `_sigaltstack`, `_pthread_sigmask` (2),
  `_pthread_kill`, `_raise`, `_kill`: **the Go archive was built and never put through the census seed**.
- **Darwin's `sa_tramp`, read further**: `sys/signal.h:277-282` holds the five-parameter prototype; the
  SDK's stubs export `___sigaction` and `___sigreturn` from `libsystem_kernel.tbd` and `__sigtramp` from
  `libsystem_platform.tbd`, and no header declares `__sigaction` or `__sigreturn` (`grep` over
  `usr/include`, nothing). **`_sigtramp` itself, disassembled** (`lldb --batch` on `/bin/ls`, `HOME` and
  `TMPDIR` in my folder, nothing written outside the root by a `-newer` find): it keeps a **sixth**
  argument (`mov x19, x5`) and hands it, with the context and 30, to the call after the handler; and it
  stores the signal number into `libsystem_platform.dylib\`__in_sigtramp` before calling the handler and
  stores zero after (`str w2, [x21, #0x460]` / `str wzr, [x21, #0x460]`; `image lookup -a`:
  `__DATA_DIRTY.__common`). So a trampoline of the runtime's must reproduce a return whose sixth argument
  the header does not declare, and replaces a process-wide mark that libSystem's own trampoline keeps.

### Item 7, the census seed

Re-run on this Mac with the brief's names and the jump family and compat installers added
(`nm -u`): libuv 1.53.0 `_dlsym _kill _posix_spawnattr_setsigmask _pthread_sigmask _sigaction _signal
_sigprocmask`; libcrypto 3.5.9 and 4.1.0 **`__longjmp __setjmp`** `_dlsym _sigaction _signal`; libSDL3
3.4.20 `_dlsym _kill _pthread_sigmask _sigaction`; libssl none; sqlite `_dlsym`; **sdl2-compat
`_dlsym` alone**. Re-run in Linux arm64 with the extended names: `libuv.so.1` `dlopen dlsym epoll_pwait
kill pthread_sigmask sigaction signal sigprocmask syscall`. Three things follow:

- **The two filters differ**: the Linux filter asks `_setjmp`, `__sigsetjmp`, `longjmp`, `__longjmp_chk`,
  the Mac filter asks none of the jump family, so the brief's libcrypto rows (*"sigaction signal"* on the
  Mac, *"_setjmp sigaction signal"* on Linux) read as a platform difference that is the filter's: the
  Mac's libcrypto imports `_setjmp` and `_longjmp` too.
- **A call through `dlopen`, shown**: sdl2-compat 2.32.74 imports `_dlopen _dlsym _dlclose`, names
  `libSDL3.dylib` and `SDL3.framework` paths (`strings`), and has no link-time SDL3 (`otool -L`). A program
  binding SDL2 on this Mac gets SDL3's installers through `dlopen`, which the brief's row for
  `libSDL2-2.0.0.dylib` (*"none of those names"*) does not show.
- **A raw system call, shown possible**: Linux libuv imports `syscall`, so what it calls raw is unknowable
  from its imports; Go's runtime installs raw (`sysSigaction`) in a forked child.

### Item 8, the routes found unlisted

- **B' is panel 248's route I** (`248-briefs/00-shared.md:448-452`, *"I, R4's corrected form ... keeps a
  per-thread depth around the program's handler"*) under a new letter; the prime keeps it from B, but a
  seat reading 248's reports looks for *route I*. **D-tramp** is 248's one-name I-dylib
  (`ipose_kernel.c`; `248-reports/ffi-pragmatist.md:83`, *"an I-dylib interposing the one name
  `__sigaction` sees every installer"*) carrying route I's depth in a trampoline instead of a wrapper
  handler: what is new is that the kernel then answers the query and keeps the flags and mask, so I's
  table goes; what is not new is the I-dylib's price (248 objected to I-dylib, `248-*.md:318-323`).
- **D-imports and 247's veto**: 247 vetoed A for *"their selection is a name list, and `wraplocale` dies 5
  of 5 in silence through its hole"* (`247-*.md:293-294`). D-imports' list is a library's imports, so
  its holes are other ones, measured today: a library loaded by `dlopen` (sdl2-compat and SDL3, above),
  an installer reached through `dlsym` (every census library on this Mac imports `_dlsym`), a raw call
  (Linux libuv imports `syscall`), and the program's own units, which are not a bound library. Whether
  those holes are 247's veto's shape is the seats' to say; the sdl2-compat program is the one to build.
- **Syscall user dispatch**: the brief poses it as a question, rightly; the more general Linux facility,
  **seccomp's filter returning a trap on `rt_sigaction`**, is searched (`seccomp` 0 files) and not posed.
  Whether a filter survives `execve` into a child and what a child that installs a handler then gets is
  `seccomp(2)`'s to say, unread here.
- **Routes still missing**, posed and unrun:
  - **A sweep instead of an interposer (Darwin)**: the runtime, in the executable, re-installs every
    signal whose handler is a function through libsystem_kernel's `__sigaction` with its own `sa_tramp`, at
    chosen points (the first crossing, each `hero_callback_of`), with no dylib and no definition; the
    kernel hands the trampoline the handler, so no table is needed. Its holes: a handler installed after the
    last sweep, and a library that re-installs through libc (which passes `_sigtramp` again). The Linux
    analogue re-installs a wrapper and needs B''s table.
  - **The C library's own mark (Darwin)**: `__in_sigtramp` exists (above), but is process-wide, private
    (absent from the `.tbd`), cleared by a nested handler's return and left set by a `siglongjmp` out.
  - **The kernel's shadow stack (Linux)**: x86-64's CET shadow stack and arm64's GCS hold every return
    address whatever the frame records; whether any target kernel and CPU enables it for a process whose
    every object is not marked, and whether the delivery's return is on it, is a question for the
    compiler-engineer (Linux's `/proc/self/status` names a thread's shadow-stack state where the kernel
    supports it; unread here).
  - **A refusal at a library's first install** (under A or B''s definitions): refuse when a function is
    installed while the program has handed C a function. Its cost is every program binding any of the
    25 Mac or 29 Linux libraries above and handing C a function (OpenSSL's, libuv's, SDL's, ncurses' start-up).
  - **Panel 247's refused routes** (A by name, E, S, F, H, I, A's declaration form, `247-*.md:293-297`)
    are not listed beside 248's, so a seat may re-derive E or S as *unlisted*.
- **Linux's `sa_restorer`**: 00-shared.md says Linux's kernel action *"carries no such field on the way
  in"*, true, and lists `sa_restorer` itself; it is a field on the way out (where the handler returns),
  which glibc fills with its own. Whether a restorer of the runtime's serves B' (it cannot raise a depth
  before the handler; it can see the return) is the seats'; *"so on Linux the shape is B'"* is a claim
  about the option set and should say what was weighed.

### Item 9, the bounds

- **No paid run is named and none is asked**: `grep -n -E 'claude -p|--max-budget|measure --refresh|API
  call'` over the five briefs finds each only inside a prohibition.
- **Every program that may not end is bounded**: the table covers storms, census probes (Go's included),
  waiters, stage 2 builds and Docker runs; `storm.py` bounds itself (its lines 33-51). Two gaps: its reader
  threads join only on end of file (`:52`), so **a storm target that leaves a child in a process group of
  its own** (which the runtime gives every child, `run.c:1648`, `:1659`) and holding the pipes would hang
  the driver unbounded; and **the emulated stage 2**: the coordinator bounded it at 3000 s (`base.sh`) and
  its log's *"1102 s"* is counted from before the seed (`s` set once, `base.sh:4`), so stage 2 alone took
  about 1,075 s at load 282; the shared table bounds a stage 2 at 1800 s, and the compiler-engineer's
  container at 5400 s must hold two emulated stage 2s (base and word), a harness build and the cases.
- **Go's writes**: the environment block is right, but it must precede **every** `go` command: my
  `go version` without it wrote three files under the home directory (§ B). The brief quotes
  `go version go1.27.3 darwin/arm64` as a platform fact, which invites exactly that.
- **`apt-get` only inside `--rm`**: yes, in every brief. **The Windows box** is written as off and owed to
  the CI's leg: yes (my `ssh -o BatchMode=yes -o ConnectTimeout=15 win 'echo up'` at 23:07 printed
  *"Operation timed out"* again). **Each seat's copy and compiler its own**: yes; the binary prefixes and
  container names are per seat.
- **The `run` form under emulation runs `--sanitize` at `-O2` and `-O0`** (`suite_run.hero:231-240`, the loop at 238):
  whether ASan runs in the emulated image is unread, and a case read red there may be red for that.

### Item 10, the remedy and spec line 404

The brief states the difference correctly (`sed -n 404p spec/heroes-spec.md`; `sigguard.c:87-91`), and
defect **2533**, `blocking`, filed 2026-11-14T21:45:55Z by this sitting's briefs agent (`gh issue view
2533`), already tracks it; the brief does not name it. What it leaves out and what bears on question 3:
the macro has **three users**, `sigguard.c:306`, `alloc.c:555` and `lend.c:194` (`grep -rn
HERO_SIGNAL_REMEDY runtime/`); **ten** `.expected` files pin its words (`grep -rl 'hero_net_stop_asked()
-> i64\` answer 1' tests/golden | wc -l`); and panel 247's R9 binds the remedy to name each door *"as R5's
sentence does"* (`247-*.md:280-284`). So *"a refusal with R1's words"*, which the brief says R3 asks on
Darwin, carries words 2533 calls false until 2533 lands, and any new words question 3 proposes move the
same macro, the same ten files and the same spec sentence: the synthesis has to order the two changes.

### Item 4, the CI's three reds and candidate C's premise

Settled in § A (the other two handlers: the same shape, on both images) and § C (the walk through the C
library's own frames). **Run 43640139573 at this base had not ended**: `gh run view 43640139573` at 23:10
printed `in_progress`; job 132585763737 (Linux x86-64), started 2026-11-14T20:56:59Z, stood at step 20,
*"The net, in Heroes"*, `in_progress` (`gh api .../jobs/132585763737 -q '.steps[]'`). In the previous job's
log the net's lines run from 07:41 (line 1329) to 10:30 (the FAIL blocks), so this one probably ends
after the seats have started, an inference from those timestamps. The CI's clang 21.1.8 remains unread
(apt.llvm.org's `clang-21` inside a `--rm` container would read it; unrun, a question).

### Item 5, R3's two cases at this base

The counts are what the logs print. **Darwin**: `briefs/r2522/storms.log` three storms, each `armed`,
`exit -9`, 12,289, 12,903 and 9,550 signals, pids 67979, 67990, 68003; `ips.py ... coord-fliblocale`
printed `('coord-fliblocale', 'SIGKILL', 'lock') 3`, and each report's JSON names one of those three pids
with `SIGKILL`; `flib-1..3.out` hold `0|answer ran for 30|0|60|`, `.err` 0 bytes. The three flib runs'
**exit codes are in no file**: I re-ran a copy (`critic/r3/critic-flib-nofp`, `timeout -s KILL 20`):
3 of 3 exit 0, the same stdout, stderr 0 bytes. **Linux arm64**: `census.log` prints every count the
brief gives (flib 3 of 3 exit 0, `0|answer ran for 10|0|20|`; three storms armed, exit 0, 1,449,252,
1,407,350 and 1,381,368 signals, *"built 20000000 words"*); the log holds no `date`, so its times
(22:36:38 to 22:40:21) and `docker exit 0` are the coordinator's shell's, not the file's.
**At 20?** The base at three shows the defect stands (one death is enough to show it); R4's *"refused 20
of 20"* binds a route, and both building briefs ask 20 of 20, armed runs counted, for every route.
Nothing is owed at the base.

## E. Questions the sitting should ask and does not

1. **Do the tree's own `atexit` and `fork` cases run the Heroes function inside the signal on an x86-64
   whose C library keeps no frame records** (Debian's, by § C's model), at every level, and so what does
   candidate C leave besides libraries built apart: the C library's own frames.
2. **Does each definitions route work when it is reached before the runtime starts**: a Go archive's
   constructor installs its handlers before `main`, through libc's `sigaction` on Linux
   (`gcc_sigaction.c:63`) and libSystem's on Darwin; under A or B' that is the runtime's definition, called
   before its table, key or marker exist.
3. **Do Go's preemption and its panics survive each route and the runtime's own stack guard**: Go edits
   the interrupted context (`signal_unix.go:342-348`); the runtime's guard saves the handler it finds and
   chains a fault not its own to it (`stack.c:670-685`), so a Go nil dereference in a Heroes program that
   links an archive goes through the runtime's guard first, at the base already.
4. **How 2522's repair and defect 2533's are ordered**: one macro, three users, ten golden files, one spec
   sentence.
5. **What rule picks the census's first entries**: the brief's list drops two of the tree's own
   bindings (ncurses, curl) that import installers on at least one platform.
6. **What a program binding SDL2 on this Mac gets under each route and under D-imports**: SDL3, through
   `dlopen`.
7. **What D-tramp must keep that `_sigtramp` keeps**: the sixth argument it passes on at the return, and
   `__in_sigtramp`.
8. **Whether ASan runs in the emulated image**, before a red there is read as the frame records'.

## The numbered list

1. **The guard-unchanged sentence names a file outside its own pathspecs.** 00-shared.md:70-74 says the
   diff over nine named paths *"lists one file, `alloc.c`"*. Command: `git diff --stat df7612ad3 722ae76bf
   -- runtime/parts/sigguard.c ... selfhost/cli/supplied.hero`. Printed: nothing. `alloc.c` (not among the
   nine) moved +12 -4, the leak check's `hero_run_folders_held_at_exit`. Repair: *"the nine paths print
   nothing; `alloc.c`, outside them, moved +12 -4 for defect 2490's leak check"*.
2. **The emulated stage 2's 1,102 s includes the seed.** Command: `cat briefs/linux/base.sh` (`s` set once,
   line 4) and `base-amd64.log`. Printed: *"seed exit 0 after 27 s"*, *"stage2 exit 0 after 1102 s"*. So
   stage 2 alone took about 1,075 s, bounded there at 3000 s, where the shared table bounds a stage 2 at
   1800 s. Repair: say *"1,102 s from the start, the seed's 27 included"*; give the emulated stage 2 the
   coordinator's 3000 s bound; and check that the compiler-engineer's 5400 s container fits two emulated
   stage 2s, the harness and the cases, or split it in two.
3. **The other two 2494 handlers have the same shape** (my brief's item 4). Command: `critic/fp/run.sh`
   in `critic-249-1` (arm64) and `critic-249-2` (emulated x86-64). Printed: x86-64 `-O2` `leaving_handler`
   `push %rax; xor %edi,%edi; call`, `forking_handler` `push %rax; call ...; pop %rax; ret`, both `push %rbp;
   mov %rsp,%rbp` with the word and at `-O0`; arm64 `stp x29, x30` / `mov x29, sp` at every level. Repair:
   write *"all three handlers, by `objdump` on both images"*, and keep clang 21.1.8 named as unread.
4. **The walk cannot climb through Debian's x86-64 glibc from an `atexit` or `fork` callback, at any
   program flag.** Command: `critic/glibc/walk.sh` (R1's walk in C) in `critic-249-8` and `critic-249-9`.
   Printed: x86-64 `c` found at `-O0`, ended at step 0 at `-O2`, found with the word; `e` and `f` *"walk
   ended at step 0"* at all three levels; arm64 all found. Repair: in 00-shared.md § C and C's table cell,
   name the C library's own frames as C's residue on such a libc; in the compiler-engineer's brief, say
   the emulated image is expected to read the `atexit` and `fork` cases red at `-O0` and ask that run first,
   naming the level that fails; add question E.1.
5. **The CI run at this base had not ended.** Command: `gh run view 43640139573` and `gh api
   .../jobs/132585763737`. Printed at 23:10: `in_progress`, step 20 *"The net, in Heroes"*. Repair: hand the
   read to the compiler-engineer with those two commands and the log command 00-shared.md gives.
6. **R3's counts hold; the flib exit codes were in no file.** Command: `cat briefs/r2522/storms.log`,
   `python3 -I ips.py ... coord-fliblocale`, each `.ips`'s pid, `census.log`, and three runs of a copy of
   `coord-flib-nofp`. Printed: the brief's every count; `('coord-fliblocale', 'SIGKILL', 'lock') 3` with
   the three pids; my runs 3 of 3 exit 0. Repair: none to the counts; say the exit codes were re-run, and
   that `census.log` holds no `date` (its times are the shell's). Nothing is owed at 20 for the base.
7. **Go 1.24's line numbers differ; the clause stands.** Command: `apt-get install golang-1.24-src` in
   `critic-249-3`, then `grep -n` over `/usr/share/go-1.24/src`. Printed: 1.24.4-1; `sigPreempt` 73, the
   clause 177-179, the throw 1116, `doc.go` 128-129 and 196. Repair: cite 1.24.4's lines beside 1.27.3's
   for the Linux seat, and name `golang-1.24-src 1.24.4-1` as the source under `golang-go 2:1.24~2`.
8. **Debian's bdwgc takes SIGPWR and SIGXCPU**, both `SA_RESTART`, no `SA_ONSTACK`. Command:
   `critic/linux/gcsig.c` against `libgc-dev 1:8.2.8-1`. Printed: `GC_get_suspend_signal 30
   GC_get_thr_restart_signal 24`, `sig 24 ... flags 0x10000000`, `sig 30 ... flags 0x10000004`. Repair:
   replace *"an inference, the ffi-pragmatist's to run"* (00-shared.md:386-387) with this run, and drop the
   `GC_USESIGRT_SIGNALS` question for Debian's build from the ffi-pragmatist's bdwgc task.
9. **The Go archive was built and never put through the census seed.** Command: `nm -u
   briefs/go/src/libgoanswer.a`. Printed: `_sigaction _sigaltstack _pthread_sigmask (2) _pthread_kill
   _raise _kill`. Repair: add its row, and add that a c-archive installs through libc's `sigaction` on
   Linux (`gcc_sigaction.c:63`, raw `sysSigaction` in a forked child, `cgo_sigaction.go:36-37`) and
   libSystem's on Darwin (`sys_darwin.go:607`), so A's and B''s definitions see Go's installs.
10. **The Mac and Linux import filters differ.** Command: `nm -u` with the jump family added. Printed:
    Mac libcrypto 3.5.9 and 4.1.0 `__longjmp __setjmp _dlsym _sigaction _signal`. Repair: one name list on
    both platforms; correct the Mac libcrypto row; the Mac's `_longjmp` keeps the mask blocked, which is
    the B' and D-tramp depth question's shape.
11. **sdl2-compat reaches SDL3 through `dlopen`.** Command: `nm -u`, `strings`, `otool -L` of
    `libSDL2-2.0.0.dylib`. Printed: `_dlclose _dlopen _dlsym`, SDL3 paths, no link-time SDL3. Repair:
    00-shared.md's *"`libSDL2-2.0.0.dylib` (sdl2-compat) ... none of those names"* gains *"and loads SDL3
    by `dlopen`"*; name it as the `dlopen` residue of the import reading and of D-imports.
12. **Linux libuv imports `syscall`, `dlopen`, `dlsym`, `epoll_pwait`.** Command: `nm -D --undefined-only`
    with the names extended, `critic-249-3`. Printed: `dlopen dlsym epoll_pwait kill pthread_sigmask
    sigaction signal sigprocmask syscall`. Repair: the seed's libuv row and D-imports' residue say a raw
    call is possible and unknowable from imports.
13. **The first entries drop two of the tree's own bindings, and the world holds more.** Command: the
    brief's own `grep` (curl 2, ncurses 1); `nm -u` over Homebrew's ncurses, curl, raylib, json-c; `nm -u`
    over the 357 Homebrew dylibs; `nm -D` over the image's 155. Printed: ncurses `_kill _sigaction
    _sigprocmask` (Mac) and `sigaction sigprocmask kill` (Linux); Debian's libcurl `sigaction`, Homebrew's
    none; 25 of 357 and 29 of 155 importing an installer (`critic/world/`, `critic/linux/linux.log`).
    Repair: add ncurses and curl to the first entries; name a crash reporter (Abseil's is on this Mac),
    GLib and libevent, an embedded interpreter, UBSan and HWASan as candidates; state the rule that picks
    *first*.
14. **The four points miss four observables.** Command: `grep -n` in Go 1.27.3 and `sed` of
    `stack.c:670-685`. Printed: `fwdSig[i] = getsig(i)` (135), `ctxt.pushCall(asyncPreempt, newpc)`
    (348), `preparePanic` (arm64 62, amd64 49), the guard's saved-handler chain. Repair: add (5) the action
    a query answers and the order handlers install and chain (constructors before `main` included), (6) the
    context a handler writes, (7) `errno` and `SA_RESTART` as the interrupted code sees them, (8) which
    thread a process-directed signal reaches; move B''s *"`sigaction(sig, NULL, &old)` ... answers the
    wrapper"* from cell (4) to (5).
15. **Two table cells assert more than they measured.** Command: the coordinator's `objdump` and mine
    (items 3-4). Printed: arm64 keeps records at `-O2`; Debian's x86-64 glibc's frames give none. Repair:
    C's cell (4) reads *"x86-64 only; the program's C and the runtime, never the C library's frames or a
    library built apart"*; A's row gains a Darwin cell (no real-time signal there).
16. **B' is 248's route I under a new letter; D-tramp is the one-name I-dylib carrying I's depth.**
    Command: `sed -n 448-452p 248-briefs/00-shared.md`; `sed -n 83p 248-reports/ffi-pragmatist.md`.
    Printed: *"I, R4's corrected form ... keeps a per-thread depth"*; *"an I-dylib interposing the one name
    `__sigaction` sees every installer"*. Repair: say *"B' (248's route I)"* and *"D-tramp (248's one-name
    I-dylib with I's depth in a trampoline, no table)"* in § Question 2.
17. **D-tramp's trampoline has two facts the brief does not hold.** Command: `grep` of the SDK's `.tbd`
    stubs and `usr/include`; `lldb --batch ... disassemble -n _sigtramp` with `HOME` in my folder.
    Printed: `___sigaction` and `___sigreturn` exported by libsystem_kernel and declared by no header;
    `_sigtramp` keeps `x5` (a sixth argument) for its return and writes the signal number to
    `__in_sigtramp` around the handler. Repair: add both to 00-shared.md's D-tramp and to the
    ffi-pragmatist's D-tramp task (*"return as `_sigtramp` does"* must name the sixth argument and the
    mark).
18. **Routes nobody listed.** Command: the searches above and the runs of items 11, 12, 17. Printed: as
    § D, item 8. Repair: pose, unrun, in 00-shared.md § D: a Darwin sweep that re-installs each handler
    through `__sigaction` with the runtime's trampoline (no dylib, no definition); Darwin's own
    `__in_sigtramp` (process-wide, private); seccomp's trap on `rt_sigaction` beside syscall user
    dispatch; the shadow stack (CET, GCS); a refusal at a library's first install; and list panel 247's
    refused routes (`247-*.md:293-297`) beside 248's.
19. **"So on Linux the shape is B'" is a claim about the option set.** Command: `sed -n 74,81p
    /usr/include/asm-generic/signal.h` in the arm64 image. Printed: `sa_restorer` under `SA_RESTORER`.
    Repair: say what was weighed: `sa_restorer` is a field on the way out, which glibc fills, and whether
    the runtime's own serves B' is the seats'.
20. **The lane.** Command: `sed -n 18,34p .claude/skills/panel/SKILL.md`, `sed -n 184,186p CLAUDE.md`,
    `sed -n 600,603p` and `:579` of 00-shared.md, `sed -n 601,605p 248-reports/critic-briefs.md`, `sed -n
    72,79p selfhost/cli/measure.hero`. Printed: *"no surface, no diagnostic and no spec token"*; *"has a
    sentence in the spec behind it is a full panel"*; *"a diagnostic class"*; question 3's *"refuses or
    tells ... with which words"*; D-imports *"a new diagnostic"*; 248's Q4 escalated before any seat;
    `--refresh` is the real count's call. Repair: either convene the spec-warden and the blind seat now,
    with the inputs § D item 1 names, after asking the author for the blind seat's paid run and naming
    the spec-warden's `--refresh`; or write in 00-shared.md why question 3 and D-imports wait for the
    synthesis despite `SKILL.md:33-34`, citing 247's R4 (*"a soundness sitting's ruling"*,
    `247-*.md:210`) for the architecture half only, and say that `ffi_defined_elsewhere`'s widening is
    known in advance for A and B' on Linux.
21. **Defect 2533 is not named, and the remedy's reach is larger than the brief says.** Command: `gh issue
    view 2533`, `grep -rn HERO_SIGNAL_REMEDY runtime/`, `grep -rl ... tests/golden | wc -l`, `sed -n
    280,284p 247-*.md`. Printed: 2533 `blocking`, filed 21:45:55Z; users `sigguard.c:306`, `alloc.c:555`,
    `lend.c:194`; 10 files; R9's *"as R5's sentence does"*. Repair: cite 2533 in § Found while writing and
    § Question 3; say a Darwin refusal *"with R1's words"* carries words 2533 calls false until it lands;
    ask the synthesis to order the two changes (question E.4).
22. **The storm driver can hang on a target's child.** Command: `cat -n tools/storm.py`; `grep -n
    'setpgid(0, 0)' runtime/parts/run.c`. Printed: the reader threads join on end of file (line 52); the
    runtime gives a child its own group (1648, 1659). Repair: the bounds table says a storm target that
    spawns has its children's output sent to `/dev/null`, or is not stormed through `storm.py`, and that
    `pgrep -f` by the child's name follows every storm.
23. **Go's environment must precede every `go` command.** Command: `stat -f 'born %SB'` of
    `~/Library/Application Support/go/telemetry/local/*2026-11-1*` and `upload.token`. Printed: born
    22:54:53, at my bare `go version` (§ B, mine, reported for the author). Repair: the rule says *every*
    `go` invocation, `go version` and `go env` included, runs after the exports, and the platform line's
    `go version go1.27.3 darwin/arm64` says it was read so.
24. **The `run` form runs `--sanitize` under emulation.** Command: `sed -n 205,250p
    tests/harness/suite_run.hero`. Printed: `-O0`, guard malloc, `-O2`, then `--sanitize` and `--sanitize
    -O0` (the loop at 238). Repair: the compiler-engineer's brief asks which level a red is at, and whether
    ASan runs in the emulated image at all, before reading a red there as the frame records'.
25. **Verified and holding**, for the record: the base's hash, tree, subject and diff counts; every line
    count; every `sigguard.c`, `stack.c`, `callback_thunk.hero`, `flags.hero` line cited; the CI log's
    lines 1329 and 4112 and its three FAILs, none at `-O0`; the job's labels; decision 2527 closed
    20:56:59Z; 2522's labels, milestone and comment times; Go 1.27.3's quoted lines; bdwgc 8.2.8's quoted
    lines; Darwin's `__DARWIN_NSIG 32` and `sa_tramp` at 277-282; `PR_SET_SYSCALL_USER_DISPATCH 59` at
    `prctl.h:253`; the images, their IDs, architectures and dates; macOS 27.2, SDK 27.2, `ld-27106.1`;
    Amphetamine's assertion (with `powerd`'s beside it at 23:07); the Windows box off at 23:07; the
    image's missing `/usr/bin/time`; the inputs' storm.py hash in two copies; no paid run named.

Finished 2026-11-14 at about 23:12 CET (`date`). Nothing of mine left running: `docker ps` printed no
`critic-249-*` after each of my eleven containers; `pgrep -fl critic-flib` printed nothing after my runs.

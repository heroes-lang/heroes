# Panel 249, the ffi-pragmatist's report

Written as the work goes, 2026-11-14 and 15. Folder: `.claude/worktrees/p249-scratch/249-ffi-pragmatist/`
(`F/` below): `F/tree/` the base, `F/c/` every probe, `F/logs/` every log, `F/linux/` the Linux arm64
scripts and their logs, `F/gohome/` every write Go made. Binaries `fp-...`, containers `fp-249-<n>`.

## 0. The base, and where the briefs are wrong

- **On the base.** `F/tree` made by the brief's `git archive`, a `.git` FILE reading `gitdir:
  /nonexistent-ffi-pragmatist` beside it; seed exit 0 (23:35:59 to 23:36:28), stage 2 exit 0
  (to 23:40:23), `heroes 0.6.0`, **`sources 2ab7ca0a644692a1`**, `ok runtime runtime`, Apple clang
  21.0.0 (`F/logs/build.log`). In Linux arm64 the same two stages, the same `sources` line, Debian
  clang 22.1.8 (`F/linux/log-census.txt`, container `fp-249-1`).
- **Load.** `uptime` read *294.60 337.98 210.01* at 23:35 and *76.25 250.45 194.14* at 23:37: no
  duration below is a measurement; counts are instructions retired or event counts.
- **Go wrote nothing outside the root**: every `go` command ran after the brief's exports; the home
  directory's Go telemetry folder kept `1727731417` (`stat -f %m`), `find ~/Library/Application\
  Support/go -newer F/c/go/go.mod` printed nothing, no `~/Library/Caches/go-build`, no `~/go`.
- **Where the briefs are wrong or incomplete, by what was run** (each row below):
  1. **The runtime's own fault guard breaks a Go archive at the base, before any candidate**: a Go nil
     dereference that Go turns into a recovered panic in plain C (`fp-go-c`, 3 of 3 *"recovered 1"*)
     **kills the Heroes program**, 3 of 3 exit 134, with *"panic: a null pointer was read through, at
     offset 0x0, called from main.GoNilPanic ... This runtime raised neither"* (`F/logs/go-hero-darwin.txt`).
     The brief says the guard *"chains a fault not its own to the handler it found"* (00-shared.md:528-530);
     it does, but only **after** its null arm (`stack.c:961-972`), which claims every fault under 64 KiB
     on every thread as the program's own. Not this sitting's question; a defect of the boundary, for the
     coordinator to file (I hold no GitHub write).
  2. **glibc 2.41's query writes garbage into `sa_mask` past the kernel's 64 bits**: my first Linux
     snapshot compared masks with `memcmp` and read every action as moved; compared by `sigismember` 1
     to 64 they do not. Any landing's differential test compares so (248's critic met the same in
     `diff2`).
  3. **bdwgc 8.2.8 on Apple silicon aborts at start unless built non-executable** (*"Cannot allocate
     executable pages"*, exit 134, its default `GC_pages_executable = TRUE`, `os_dep.c:100`, 2269); built
     `-DNO_EXECUTE_PERMISSION` it runs (§ 1.2).
  4. **Go refuses cgo in the Linux image unless `CC=clang`** (*"build constraints exclude all Go files"*,
     the image has no `gcc`): a seat building a Go archive there sets it.
  5. **248's Darwin I-mask dylib refuses correct programs in two rows the brief carries as closed**: after
     `_longjmp` out of a handler (Darwin's `_longjmp` gives no mask back, *"still blocked 1"*; 248 measured
     plain `longjmp`, which does) and under a parent that blocked the marker (that dylib has no deliberate
     unblock) (§ 2.2, run with SIGEMT as its marker).
  6. **The critic's `swait.c` hangs on Darwin at the base** (exit 137 by my `timeout -s KILL 10`): the kernel
     discards a blocked SIGURG whose action is the default, which ignores it; its Linux run stands.
  7. **glibc's `__libc_allocate_rtsig(1)` takes from the bottom** (34, `SIGRTMIN` then 35); `(0)` from the top
     (64, `SIGRTMAX` then 63), run.
  8. **A route nobody listed reaches Darwin's two-level dylibs with no second file** (§ 3.7, D-rebind, built
     and run): the brief's Darwin arms all carried the I-dylib's price or the sweep's holes.

## 1. Question 1, the census

### 1.1 The instruments (`F/c/census/`, `F/linux/c/`)

- **`snap.h`**: every action read by `sigaction(sig, NULL, &old)` for 1 to `NSIG - 1` (handler named by
  `dladdr`, flags, mask), the calling thread's mask and alternate stack, a new thread's mask; `fp_diff`
  prints what moved between two snapshots (masks by `sigismember`, § 0 item 2).
- **`drv.c`**: one driver, compiled `-DLIB_<name>` per library, starting it as a binding would and
  diffing the snapshots around each step.
- **Darwin's tracer, `libfptrace.dylib`** (`trace_darwin.c`): an `__interpose` section over
  libsystem_kernel's `__sigaction`, `__pthread_sigmask`, `sigprocmask`, `kill`, `__pthread_kill`,
  `sigaltstack`, `__sigwait`, `sigsuspend`: every installer, mask call and sender of every image, from
  the first constructor, one line each naming the first image on the stack that is not libSystem's
  (248 § 1.2: libSystem's cross-image binds are interposed). Inserted by `DYLD_INSERT_LIBRARIES`,
  filtered to the program by `FPTRACE_ONLY` (Homebrew's `timeout` is not hardened and took it too).
- **Linux's tracer, `libfptrace.so`** (`trace_linux.c`) by `LD_PRELOAD`: glibc's installers, mask calls,
  waits (`sigwait`, `sigwaitinfo`, `sigtimedwait`, `signalfd`), senders, `sigaltstack`, **and `syscall`
  itself**, so a raw `rt_sigaction`, `rt_sigprocmask`, `tgkill` or `rt_sigtimedwait` through libc's
  `syscall` shows; an inline `svc` does not.

### 1.2 Darwin arm64, at the base (`F/logs/census-darwin.txt`, `go-darwin.txt`, `go-hero-darwin.txt`, `gc-darwin.txt`)

Every run under `timeout -s KILL 60`, output through `head -c`, plain then traced.

| library (version) | what its start installs, measured | mask calls, senders, waits | its handler's mask; `SA_ONSTACK` | calls back into the program from a handler |
|---|---|---|---|---|
| **libuv** 1.53.0 | nothing at `uv_default_loop`; `uv_signal_start(SIGUSR1)`: SIGUSR1 → `uv__signal_handler`, `SA_RESTART` | `pthread_sigmask(SETMASK, all)` around each install and stop (`uv__signal_block_and_lock`), then back | **every signal** (`mask=0xffffffff`); no | no: the handler writes a pipe, the loop calls the program's `uv_signal_cb` outside the signal (*"the loop took signal 30"*) |
| **OpenSSL** 3.5.9 and 4.1.0 | **nothing** at `OPENSSL_init_ssl`, RAND, SHA-256, an `SSL` object (snapshot and trace) | none | — | — |
| **SDL3** 3.4.20 | `SDL_Init(SDL_INIT_EVENTS)`, no display: SIGINT and SIGTERM → `SDL_HandleSIG`, `SA_RESTART`, from `SDL_EventSignal_Init`; `SDL_Quit` puts both back to default | none | empty; no | no (the handler posts a quit event) |
| SDL3 with `SDL_HINT_NO_SIGNAL_HANDLERS=1` | **nothing** | none | — | — |
| **SDL2** (sdl2-compat 2.32.74) | **SDL3's installers**: SIGINT and SIGTERM → `libSDL3.0.dylib:SDL_HandleSIG`, by `libSDL3.0.dylib+0x3cd0c(SDL_EventSignal_Init)`, a library the program never linked (§ 5) | none | empty; no | no |
| SDL2 with the hint | **nothing** | none | — | — |
| **sqlite** 3.54.1 | **nothing** at `sqlite3_open`, nor through `sqlite3_exec` with a callback (*"sqlite: callback 42"*) | none | — | — |
| **ncurses** 6.6 | `newterm("xterm")`: SIGTSTP → `handle_SIGTSTP` (`_nc_signal_handler`), SIGINT and SIGTERM → `handle_SIGINT`, SIGWINCH → `handle_SIGWINCH` (`CatchIfDefault`), each only where the action was default; `endwin` leaves them | none at start; `handle_SIGINT` and `handle_SIGTSTP` call `sigaction` from inside the handler (`otool -tV`) | empty; no | no |
| **curl** 8.23.0 | **nothing** at `curl_global_init` nor through two `curl_easy_perform` (a `file:` URL and a refused `http:` connection) | none | — | — |
| **json-c** 0.19 | nothing | none | — | — |
| **GLib** 2.90.1 (beyond the first entries, Homebrew's) | `g_unix_signal_add(SIGUSR1)`: → `g_unix_signal_handler`, **`SA_ONSTACK`** `SA_RESTART` `SA_NOCLDSTOP` | `pthread_sigmask(SETMASK, all)` around its worker thread's start | empty; **yes** | no (the loop runs the source) |
| **bdwgc** 8.2.8, tarball, `-DGC_THREADS -DNO_EXECUTE_PERMISSION` | **nothing**, plain or `GC_enable_incremental` (*"incremental mode on: 1"*); 248's list check beside a collecting thread **intact after 15,208** and **about 34,000** collections, 3 of 3 each | **none** (trace: only my own `sigaltstack`) | — | — |
| **Go 1.27.3 `c-archive`** | **before `main`, from its constructor**: SIGFPE, SIGBUS, SIGSEGV, SIGPIPE, **SIGURG (16)** → `runtime.cgoSigtramp`, through libSystem's `sigaction` (`runtime.sigaction_trampoline`); `os/signal.Notify(SIGUSR2)` installs SIGUSR2 and leaves it `SIG_DFL` with its flags after `Stop` | **SIGURG sent by `pthread_kill` constantly under load** (`GoLoad`); `pthread_sigmask(SETMASK, all)` around every thread it starts; on the calling C thread at its first call `SETMASK all`, a **32 KiB `sigaltstack`** where none stood, then `SETMASK` back | **every signal**; **yes** (`0x43`) | no Go code runs a program's function inside a signal; `GoCallBack` and `GoCallBackFromGoroutine` call back outside one |

**What a Go archive does inside a Heroes program at the base, Darwin** (`F/c/go/gohero.hero`, built with
`--library golib`, 3 runs each):

- **preemption works before and after the runtime's SIGURG probe** (*"preempt true"* both times); the
  probe borrows SIGURG from Go's handler and gives it back (`go-hero-darwin-trace.txt`: `install sig=16
  handler=hero_signal_probe_plain` then `install sig=16 handler=runtime.cgoSigtramp ... mask=0xfffefeff`,
  the kernel's own answer, SIGKILL and SIGSTOP struck);
- `Notify` works (*"notify 1"*), the callback on the calling thread works (*"callback 42"*);
- **a callback from a goroutine is refused by 2492's guard**, *"gohero2.answer ran on a thread this
  program did not start"*, exit 134, 3 of 3: correct, a Go thread is one nothing claimed;
- **the recovered nil panic is not recovered**: exit 134, 3 of 3, *"panic: a null pointer was read through,
  at offset 0x0, called from main.GoNilPanic"* (§ 0 item 1). The install order is Go's constructor first,
  then the runtime's guard over SIGSEGV and SIGBUS (`hero_stack_guard_install`), saving Go's as the one it
  chains to, then the runtime's lease handlers (SIGTRAP, SIGABRT, SIGILL).

### 1.3 Linux arm64, at the base (`F/linux/log-census2.txt`, container `fp-249-2`; `log-census.txt`, `fp-249-1`)

Debian 13's packages by `apt-get` inside the container (`dpkg-query -W`: libuv1t64 1.50.0-2, libssl3t64
3.5.7, libsdl3-0 3.2.10, libsdl2-2.0-0 2.32.4, libsqlite3-0 3.46.1, libncursesw6 6.5, libcurl4t64 8.14.1,
libgc1 8.2.8-1, libglib2.0-0t64 2.84.4, golang-go 1.24.4); their sources at those versions by `apt-get
source --download-only` (deb-src added inside the container). Every probe `timeout -s KILL 60`.

| library | what its start installs, measured | mask calls, senders, waits | handler's mask; `SA_ONSTACK` | calls back from a handler | read in its source |
|---|---|---|---|---|---|
| **libuv** 1.50 | `uv_signal_start(SIGUSR1)`: SIGUSR1, `SA_RESTART` | `SETMASK all` around install and stop | every signal; no | no (*"the loop took signal 10"*) | installers `src/unix/signal.c:238`, `:256`; **its raw `syscall`s are `gettid`, `close`, `copy_file_range`, `statx`, `getrandom`, `io_uring_*`, `_sysctl`, none a signal call** (`grep` over `src`) |
| **OpenSSL** 3.5.7 | **nothing** at start (snapshot and trace) | none | — | — | its `sigaction` calls sit by `UI_null`, the nearest exported label (`objdump`; on Darwin they are `read_string_inner`'s, the password prompt, `otool -tV`), and in the CPU probes, each leaving its handler by `siglongjmp` (`armcap.c:196`, `:369`; `ppccap.c`, `s390xcap.c`, `sparcv9cap.c`), none run at start here (the trace): the shape every candidate must not refuse, run as rows below |
| **SDL3** 3.2.10 and **SDL2** 2.32.4 | `SDL_Init(EVENTS)`: SIGINT and SIGTERM, flags 0 (`SA_RESTART` not asked) | none at start | empty; no | no | **the real-time signal is the console's VT switch**: `find_free_signal` takes the first from **`SIGRTMIN + 2` up to `SIGRTMAX` whose action is `SIG_DFL`** (`SDL_evdev_kbd.c:345-357`, `setup_vt_signal` restores and refuses one already in use), then `SIGUSR1`, `SIGUSR2`; `sigtimedwait` drains a pending SIGPIPE after a pipe write (`SDL_waylanddatamanager.c:79`); the hint turns the start-up handlers off (*"nothing moved"*) |
| **sqlite** 3.46.1 | **nothing**, a callback through `sqlite3_exec` included | none | — | — | — |
| **ncurses** 6.5 | `newterm`: SIGINT, SIGTERM, **SIGTSTP (20)**, SIGWINCH (28), each only over a default action | none | empty; no | no | `lib_tstp.c:215-409`; `handle_SIGTSTP` re-installs from inside the handler |
| **curl** 8.14.1 | nothing at `curl_global_init`; **SIGPIPE set to `SIG_IGN` and back around each transfer** (trace: `curl_multi_perform`, `curl_easy_cleanup`) | none | — | no function installed | `sigpipe.h:60-77`; its `alarm`/`siglongjmp` resolver timeout (`hostip.c:1040-1146`) is not in Debian's build (no `alarm`, no `siglongjmp` among its imports). **Homebrew's 8.23.0 imports none and toggled nothing (§ 1.2)**: the difference is in what each build compiles in, by these runs; which option, unread |
| **GLib** 2.84.4 | `g_unix_signal_add(SIGUSR1)`: `SA_ONSTACK` `SA_RESTART` | `SETMASK all` around its worker | empty; yes | no | — |
| **bdwgc**, Debian 1:8.2.8-1 and the tarball | `GC_INIT`: **SIGPWR (30) `SA_RESTART\|SA_SIGINFO`, SIGXCPU (24) `SA_RESTART`**, masks of most signals; **incremental mode adds SIGSEGV and SIGBUS** (`SA_SIGINFO`, mask {30}) | `pthread_kill` 30 and 24 to every thread at each collection; `UNBLOCK {24,30}`; `sigsuspend` among its imports (the seed; my tracer does not interpose it) | no `SA_ONSTACK` | no | (248's reading, carried: its handler stores `GC_approx_sp()`) |
| **Go 1.24.4 `c-archive`** | before `main`: SIGBUS, SIGFPE, SIGSEGV, SIGPIPE, **SIGURG (23)**, through libc's `sigaction` (cgo), mask every signal, `SA_ONSTACK\|SA_RESTART\|SA_SIGINFO`; `Notify` installs SIGUSR2 | **imports only `sigaction` and `pthread_sigmask` by `nm -u`**: its masks, alternate stacks and `tgkill` are raw `svc` of its own, which my `syscall` interposer does not see (0 lines) | every signal; yes | no | — |
| **UBSan** (clang 22, `-fsanitize=undefined`), beyond the first entries | **before the program's own constructors**: SIGSEGV, SIGBUS, SIGFPE, `SA_ONSTACK\|SA_SIGINFO\|SA_NODEFER`, an 80 KiB alternate stack | none | empty; yes | no | — |
| **HWASan** (`-fsanitize=hwaddress`) | the same four and SIGTRAP, then **exit 132** (*Illegal instruction*): it does not run in this image | — | — | — | — |

**Inside a Heroes program at the base, Linux** (`fp-gohero`, `fp-gohero2`, 3 runs each): the same as
Darwin: preemption before and after the probe, `Notify`, the callback; the goroutine's callback refused by
2492; **the recovered nil panic killed by the runtime's null arm**, exit 134, *"a null pointer was read
through ... called from main.GoNilPanic"* (§ 0 item 1, on both legs). The trace shows the probe borrowing
SIGURG from Go's handler and putting it back twice.

### 1.3b The four points the critic added, read off the rows above

- **(5) The order and the query**: a Go archive installs from its constructor before `main` on both legs,
  through the C library's own `sigaction` (cgo on Linux, libSystem's on Darwin), and the runtime's guard
  installs over SIGSEGV and SIGBUS after it, chaining to Go's (traces); UBSan's runtime installs before any
  constructor of the program's, my tracer's included (its `TRACE start` printed after them); every query
  answers the kernel's action at the base, with SIGKILL and SIGSTOP struck from masks (the probe's restore
  on Darwin, `0xfffefeff`).
- **(6) The context**: Go edits it to preempt and to turn a fault into a panic (both measured working at the
  base, from C); bdwgc's suspend handler reads its own frame (248, carried) and its incremental handler loads a
  field of the `siginfo_t` it is handed (`ldr x19, [x1, #16]`, `objdump` above); no other first entry edits or reads the context in a handler, by the runs.
- **(7) `errno` and `SA_RESTART`**: libuv's handler on Darwin calls `__error` near its entry and again before
  it returns, GLib's on Linux calls `__errno_location` at entry (`otool`, `objdump` above): each reads `errno`;
  whether each restores it is unread; ncurses (but SIGWINCH), libuv, bdwgc and Go install with
  `SA_RESTART`; SDL3 on Darwin with it and on Linux without (flags 0); GLib with it; curl's `SIG_IGN` with flags 0.
- **(8) Which thread a process-directed signal reaches**: libuv, GLib and Go start their own threads with
  every signal blocked (`SETMASK all` around `pthread_create`, traced) and Debian's bdwgc blocks every signal
  but its two (traced), so a process-directed signal
  reaches one of the program's own threads; Go's goroutine threads unblock what Go needs (`minit`'s
  `SETMASK 0` on each new thread, traced on Darwin).

### 1.4 Who owns SIGURG

At the base Go's archive owns it from its constructor on both legs (16 on Darwin, 23 on Linux), and the
runtime borrows it for two probes at the first crossing and gives Go's handler back (traces above); Go
preempts by it before and after, so nothing measured broke. **Under A and D-tramp the runtime takes no
SIGURG beyond that borrowing**; under I-mask it was the marker (refused, 248). SIGURG's other users found:
none in the census besides Go and the runtime's probe (traces, both legs).

## 2. Question 2, candidate A (Linux: `F/linux/log-a.txt` `fp-249-3`, `log-b.txt` `fp-249-4`, `log-c.txt` `fp-249-5`; Darwin: `F/logs/a-darwin.txt`)

### 2.1 Linux arm64: the marker a real-time signal chosen at run time

**The C** (`F/linux/c/inst_A.c` for the C rows, `F/linux/c/route/siginstall.c` in the runtime): 248's I-mask
(its hashes in the inputs manifest) with three changes: **the marker chosen once, at the first use, as
`SIGRTMAX`** (glibc's `__libc_current_sigrtmax()`, 64 here, *"A chose the marker 64 (SIGRTMIN 34, SIGRTMAX
64)"*), the end of the range farthest from the libraries that scan upward (SDL3 from `SIGRTMIN + 2`, bdwgc's
`SIGRTMIN + 5` and `+ 6` where built so); **the marker unblocked on purpose by a constructor** (248's critic,
item 6); and the `longjmp` family defined as 248 had it. Two other choices measured: glibc 2.41 exports
`__libc_allocate_rtsig` (`nm -D`), whose `(1)` handed out **34 and moved `SIGRTMIN` to 35** and whose `(0)`
handed out **64 and moved `SIGRTMAX` to 63** for every later caller (`log-a.txt`, `log-c.txt`). Compiled
inside the runtime under its own flags by its own compiler (every build exit 0).

**R3's cases, A built into the runtime** (`log-a.txt`, the base's compiler, the runtime's three files
replaced): **`flib` static 20 of 20 exit 134** with R1's words, **shared 20 of 20**, `-O2` 5 of 5,
`--sanitize` 3 of 3, **`--sanitize -O0` 3 of 3**; `direct-l` refused by the exact test (*"ran as a signal
handler"*), `hold-l` *"pending after the hold: 1"*; **storms: static 20, 20 armed, 20 `exit -6`, 20 refusal
lines**; shared 5 of 5; sanitized 3 of 3; `dlocale-l` 3 of 3 by the exact test (no crash reports on
Linux: exits counted). `nm` of the program: the runtime defines (T) `sigaction`, `signal`, `bsd_signal`, `ssignal`,
`sysv_signal`, `__sysv_signal`, `__sigaction`, `sigset`, `sigvec`, `siginterrupt`, `pthread_sigmask`,
`sigprocmask`, `longjmp`, `_longjmp`, `__longjmp_chk`.
**The golden `run` form narrowed**: `callback` **14 passed, 0 failed**; `2494` **13 and 2**, the two D cases
248 named (`...-allocates-inside-the-allocator-...`, `...-lends-inside-the-lend-tracking-...`, at `-O0`), now
refused earlier with R1's words. **The Go archive inside a Heroes program under A** (`log-b.txt`): 3 of 3 as
at the base (preemption before and after the probe, `Notify`, the callback; the goroutine's refused by 2492).

**The C rows** (`rows.c`, a frameless library whose handler stores its callback's result, `log-b.txt`; my
first run's library tail-called its callback, so the exact test caught it at the base too, `log-a.txt`, and
that run is not read): at the base **every library row MISSED** (the handler saves `x30` alone); under A
**all 8 static rows and both shared rows refused** (*"the marker is blocked"*), the program's own three by
the exact test, after `siglongjmp` and after plain `longjmp` a correct callback **not refused**, a claimed
worker refused, every signal blocked by the program then a correct callback **not refused**, the query the
library's handler.

**248's critic's residues, the census and the program that owns a signal, under A** (`log-a.txt`):

| program | base | I-mask (SIGURG) | **A (`SIGRTMAX`)** | A, marker by `__libc_allocate_rtsig(0)` | B' (C model) |
|---|---|---|---|---|---|
| `swait.c`, SIGURG blocked and waited for | taken | **nothing in 2 s** | **taken** | — | taken |
| the same with **`SIGRTMAX`** | taken | taken | **its block dropped, killed by the marker's default, exit 192, the program saying nothing** | **taken** (`SIGRTMAX` now 63) | taken |
| the same naming **64 by number** (`hard64.c`) | — | — | **killed, exit 192** | **killed, exit 192** | — |
| `SIGRTMAX - 1`; `SIGRTMIN + 3` | taken; taken | taken; taken | taken; taken | — | taken; taken |
| `rtprog.c`: `SIGRTMIN + 3` from a POSIX timer, then queued to an `SA_SIGINFO` handler | taken (42); 7 | the same | the same | — | the same |
| `hunblock.c`: a handler that unblocks before calling back | — | 1, 0, 0 | **1, 0, 0** (a miss) | — | **1, 1, 1** |
| `inh.c` under a parent blocking SIGURG and SIGRTMAX | — | **1, 1 (a false refusal in C)** | **0, 0** (the deliberate unblock) | — | 0, 0 |
| bdwgc 8.2.8, tarball and Debian's, 248's list check, 5 runs each | intact | — | **intact 5 of 5, both** | — | intact 5 of 5, both |
| the Go 1.24 archive from C, 3 runs | as at the base | — | **the same** | — | **the same** |
| libuv, SDL3, ncurses, GLib (Debian) | | | **the same behaviour; 64 in each handler's mask** | — | the same; masks answered from the table |
| a library installing by raw `rt_sigaction` (`rawrow.c`) | 0 | 0 (MISSED) | **0 (MISSED)**, through `sigaction` 1 | through `sigaction` 1; raw not run | **0 (MISSED)**, through `sigaction` 1 |
| an installer reached by `dlsym(RTLD_DEFAULT)`; by `dlsym(RTLD_NEXT)` | 0; 0 | — | **1; 0 (MISSED)** | — | **1; 0 (MISSED)** |

Beside them, **I-onstack** (`inst.c`) on the same collector: the tarball's **5 of 5 exit 139** and **Debian's
`libgc1` 5 of 5 exit 139** (`log-a.txt`): 248's veto program, now on the distribution's own build.

**What A's marker meets in the census**: no entry installs, blocks, waits for or sends 64 (traces, both
legs); SDL3 would take it only after `SIGRTMIN + 2` to 63 were all in use (`find_free_signal`, the action
`SIG_DFL` the test; A leaves the marker's action at default, *"a library scanning for a default action sees
it free"*); Go's runtime treats 35 to 64 as `_SigNotify` and installs none unless asked; musl's range is
unrun (no image with a compiler, 00-shared.md). **A library can see the number taken only if the runtime
installs on it** (`SIG_IGN` would turn SDL's scan away; unbuilt), **or if glibc's allocator hands it out**,
after which glibc's own `SIGRTMAX` excludes it (run).

**Its cost**: the query a callback is 1,346 instructions on Darwin whatever the marker (§ 3.6; 248's
figure, re-run).

### 2.2 Darwin: what then (`F/logs/a-darwin.txt`)

- **Which classic signal**: by my traces of every census entry on this Mac (§ 1.2), the numbers installed are
  2, 15 (SDL3), 2, 15, 18, 28 (ncurses), 8, 10, 11, 13, 16 (Go), and what a program asks (libuv, GLib, Go's
  `Notify`); blocked, only in blanket `SETMASK` calls (libuv, GLib, Go), which A's rule strikes; sent, 16
  (Go). **SIGEMT (7), SIGSYS (12) and SIGXFSZ (25) were never installed, waited for or sent** by any entry,
  by those runs (sources of the Darwin builds not read). SIGEMT taken as the marker, 248's three-name dylib
  (`ipose_mask_emt.c`):
  - all frameless rows, static and two-level, refused; a correct callback not; after `siglongjmp` not;
  - **after `_longjmp` out of a handler, a correct callback refused: a false refusal** (Darwin's `_longjmp`
    gives no mask back, *"still blocked 1"*, and 248's Darwin dylib interposes no jump);
  - **under a parent that blocked SIGEMT, every callback refused: a false refusal** (that dylib has no
    deliberate unblock);
  - **a program blocking SIGEMT and waiting for it is killed**, *"EMT trap: 7"*, exit 135 (base: *"taken"*);
  - the unblocking handler passes (1, 0, 0).
  The two false refusals are repairable as on Linux (the jumps interposed, the unblock at start), unbuilt
  here; the killed program is A's own residue.
- **A mark that is not a signal** is § 3 (D-tramp), § 3.7 (D-rebind) and § 3.6 (the sweep); **libSystem's
  `__in_sigtramp` cannot serve**: it is one process-wide int, not a thread's, and `dlsym` does not find it.
- **The price of A's Darwin arm is the dylib's** (§ 3.6): it interposes three names of libsystem_kernel by
  `__interpose`, which only a dependent dylib carries (248).

### 2.3 What A needs, in the escape's words

**A reserved signal**, a change a binding's author must learn (a C library or program that uses the marker
dies by it, run on both legs); **a widened diagnostic** on Linux (panel 239's member refuses a library
defining `bsd_signal`, `sigset`, `ssignal`, `sysv_signal`, `__sysv_signal`, `__sigaction`, `siginterrupt`,
`sigvec`, `longjmp`, `_longjmp`, `__longjmp_chk`, names the base runtime neither calls nor defines; 248 and
the compiler-engineer measured the mechanism); on Darwin **a file beside every program**. No spec sentence
that I can see; the refusal's words are R1's (2533's).

## 3. Question 2, D-tramp (Darwin arm64; `F/c/tramp/`, logs `F/logs/tramp-*`, `sigtramp-disasm.txt`)

### 3.1 What `_sigtramp` keeps, read again in my own disassembly

`lldb --batch` on a program of mine, `HOME` and `TMPDIR` in my folder (`F/logs/sigtramp-disasm.txt`):
`_sigtramp(x0 catcher, x1 style, x2 sig, x3 info, x4 uctx, x5 token)` keeps the sixth argument (`mov x19,
x5`), **calls the catcher with three arguments whatever the style** (`mov x1, x3; mov x0, x2; mov x2, x4;
blraaz x8`), stores the signal number into `__in_sigtramp` before the call and zero after (`str w2, [x21,
#0x460]`, `str wzr`), and returns by `__sigreturn(uctx, 30, x19)` (`mov w1, #0x1e`), a `brk` after it.
`image lookup` puts `__in_sigtramp` in `libsystem_platform.dylib.__DATA_DIRTY.__common`: **one int for
the whole process, not a thread's**. Two facts beyond the critic's, run:

- **libSystem installs every handler with flag `0x400`**, which no SDK header names (`grep -c 0x0400
  sys/signal.h`: 0; my tracer reads `flags=0x402` for a `SA_RESTART` install), **and with it the kernel
  checks the token**: a trampoline handing `__sigreturn` a token of 0 dies, exit 133, `__sigreturn` having
  returned into its trap; without `0x400` it passes (`token.c`, `F/logs/tramp-token-darwin.txt`, four
  rows). So the sixth argument is not optional: a runtime trampoline must hand it on, as mine does.
- **`__in_sigtramp` is not reachable**: `dlsym(RTLD_DEFAULT, "_in_sigtramp")` returns NULL (`trows.c`
  prints *"found by dlsym: 0"*), and no `.tbd` stub names it (the coordinator's grep). D-tramp cannot keep
  it. Who reads it I did not find (a question: no reader was searched beyond `lldb`'s symbol lookup).

### 3.2 The build (`ipose_tramp.c`, 248's one-name I-dylib as its frame)

A dylib whose `__interpose` section names libsystem_kernel's `__sigaction` and, where a function is
installed **and the C library passed `_sigtramp`** (by `dlsym`, so a library passing its own trampoline is
left alone), hands the kernel `hero_tramp` instead; the handler, flags, mask and stack stay the program's.
`hero_tramp` raises a per-thread depth in a **pthread key's slot** (never a thread-local: Darwin makes
one at first access with `malloc`, `sigguard.c`'s own comment), calls the catcher with three arguments,
lowers the depth, and returns by `__sigreturn(uctx, 30, token)`. The C library's jumps (`siglongjmp`,
`longjmp`, `_longjmp`) and, after the row below, `setcontext` and `swapcontext` are interposed too, each
dropping the depth to zero first (the safe direction). The thunk's question is `hero_tramp_inside()`, one
slot read. **Compiled** with Apple clang 21.0.0, `-Wall`, exit 0.

### 3.3 The rows, base against D-tramp (`trows.c`, `F/logs/tramp-rows-darwin.txt`)

| row | base | D-tramp |
|---|---|---|
| the kernel accepts the trampoline | — | **yes: 14 deliveries through it** |
| static archive, frameless (`-O2 -fomit-frame-pointer`), `sigaction` and `signal` | not inside | **inside** |
| **two-level dylib, frameless, `sigaction` and `signal`** | not inside | **inside** |
| the program's own handler | not inside | inside (and the exact test, below) |
| a correct callback, no signal | not inside | **not inside** |
| `siglongjmp` out of a handler, then a correct callback; then a delivery | not inside; not inside | **not inside; inside** |
| `_longjmp` out (the mask not given back: *"still blocked 1"* on both) , then a correct callback; then a delivery | not inside; not inside | **not inside; inside** |
| **a handler left by `setcontext`** (Darwin's gives the mask back), then a correct callback | not inside | **INSIDE with the first dylib: a false refusal**; not inside once `setcontext`/`swapcontext` are interposed (`setctx.c`) |
| a claimed worker, the library's handler | not inside | inside |
| `sigaction(sig, NULL, &old)` after the dylib's install | its handler, `0x2` | **its handler, `0x2`**: the kernel's own answer, nothing kept aside |
| `errno` across a handler that leaves it (1234 set before) | 1234 | **1234** |
| a fault resumed past by a context edit (`__ss.__pc += 4`) | 1 | **1** |
| `backtrace()` in a handler | 6 frames, `_sigtramp + 56` second | **6 frames, `hero_tramp + 156` second** |
| `_Unwind_Backtrace` in a handler | 4 frames | **4 frames** |
| `__builtin_longjmp` out of a handler | *"not supported for the current target"*: no such exit on arm64 Darwin | — |

**The Go archive under D-tramp** (`F/logs/go-tramp-darwin.txt`): from C, preemption, the recovered nil
panic (a context edit), `Notify` and both callbacks read as at the base, 3 of 3 inserted and 1 linked,
**147 deliveries through the trampoline**; inside a Heroes program (`fp-tgohero2`, 3 of 3) as at the base:
preemption before and after the probe, `Notify`, the callback, the goroutine's callback refused by 2492.

### 3.4 R3's two cases under D-tramp, Darwin (`F/logs/tramp-r3-darwin.txt`, `tramp-storms-darwin.txt`)

The thunk given the depth question for the build (`sigguard.c`, four lines after `if (plain == 0 || own ==
NULL) return;`), restored after each build and `cmp`-equal to `722ae76bf`'s (said in each log); the
programs link the dylib by a group `extern "herot.h" link "herot"`. **Panel 239's `ffi_defined_elsewhere`
refused the first build, correctly**: *"library `herot` ... defines `hero_tramp_inside`, of the C library,
which the program's own runtime calls"*; the dylib a landing ships is the runtime's own, so the builds
follow the diagnostic's remedy and bind the name in the group.

- **`flib`, static archive: 20 of 20 exit 134** with R1's words (*"panic: tflib.answer was called from
  inside a signal handler ..."*); **through a two-level dylib: 20 of 20**; `--sanitize` at `-O2` and `-O0`:
  3 of 3 each; `direct` refused by the exact test (*"ran as a signal handler"*), the probe's learned
  address now `hero_tramp`'s; `hold` *"pending after the hold: 1"*, exit 0.
- **`fliblocale` storms, armed runs alone counted**: static **20 storms, 20 armed, 20 `exit -6`, 20
  refusal lines**; two-level dylib 5 of 5; `--sanitize` 3 of 3; `dlocale` 3 of 3 by the exact test.
  **Every crash report's pid is a storm's** (`F/logs/tramp-storms-ips-join.txt`: 20, 5, 3, 3 of 20, 5, 3,
  3), every one SIGABRT at the thunk's first line (`hero_signal_refuse < hero_callback_entered <
  h_0cb_tfliblocale_answer < sigcb_handler < ...`), **none SIGKILL**; the base's were SIGKILL in libc's
  lock (00-shared.md). The system's crash reporter shows `hero_tramp` where `_sigtramp` stood and walks on
  into the interrupted function (`... h_0cb_tdlocale_on_signal < hero_tramp < hero_int_to_str`).
- **The `dlopen` hole** (§ 5.2's program): refused 3 of 3 under D-tramp, run at the base 3 of 3.

### 3.5 What the exact test learns, and point (4)

The runtime's probes install through the C library's own `sigaction`, which the dylib interposes, so
**the probes return into `hero_tramp` too and learn its address**; every handler installed through the C
library returns there, so the exact test stays exact (`direct` refused). A handler installed past the
interposer (a raw `__sigaction` naming its own trampoline, left alone by design) returns into its own and
the exact test does not see it; neither did it at the base. Point (4): a handler reading its stack finds
`hero_tramp`'s frame (a frame record, `stp x29, x30` / `mov x29, sp` at `-O2`) where `_sigtramp`'s stood;
`backtrace()` and `_Unwind_Backtrace` count the same frames as at the base.

### 3.6 Its cost and its price

- **Cost**: the thunk's question about **8 instructions** a callback (`qcost.c`, `/usr/bin/time -l`:
  25,452,255 and 39,021,137 instructions for 1,000,000 and 2,000,000 questions, against 17,193,813 and
  22,966,531 for the empty loop); A's mask question **1,346** (1,355,065,215 and 2,701,383,272), as 248's
  with SIGURG: the marker's number changes nothing. Per delivery, two slot writes and a read.
- **The price is the I-dylib's**, and it is the same for A's Darwin arm (three names) and D-tramp (one name
  and the jumps): measured now (`F/logs/price-darwin.txt`), **a program linked to the dylib by
  `@executable_path/libherot.dylib` runs beside it and, copied alone, dies before `main`**: *"dyld[...]:
  Library not loaded: @executable_path/libherot.dylib ... Reason: tried: '.../elsewhere/libherot.dylib' (no
  such file)"*, exit 134; `heroes build` writes no `LC_RPATH` (`otool -l | grep -c LC_RPATH`: 0), so
  `@rpath` needs a new link word (248: *"no LC_RPATH's found"*, exit 134); an absolute install name ties
  every program to the toolchain's path on the machine that runs it; and under the hardened runtime dyld
  refuses a dylib not signed by the program's Team ID (248's run, *"different Team IDs"*, exit 134,
  carried). It rests on `__interpose`, which `man dyld` does not document (248, carried). **A program
  that is one file today becomes two**, for every program that hands C a function (the dylib must be
  linked before any library's constructor installs: Go's archive installs before `main`).
- **The sweep instead** (`sweep.c`, `F/logs/sweep-darwin.txt`): no dylib, the executable re-installs every
  function handler through `__sigaction` with its trampoline after each C call handed a function. Rows:
  the library that installs in the call taking the function, swept after: **inside**; the library that
  takes the function and installs in a later call: **MISSED**; swept, then re-installed through the C
  library: **MISSED**; cost **41,107 instructions a sweep** (422,756,181 and 833,827,251 for 10,000 and
  20,000). Its holes are of 2522's class and silent: a refusal R3 forbids keeping.

### 3.7 D-rebind: a route nobody listed, built (`F/c/rebind/`, `F/logs/rebind-*.txt`)

Asked what would have to be true for a Darwin arm with no second file: **the executable rewrites, in every
loaded image outside the shared cache, the pointer that image's stubs load for `sigaction`, `signal`,
`siglongjmp`, `longjmp`, `_longjmp`, `setcontext` and `swapcontext`** (the indirect symbol table of its
`__got` and `__la_symbol_ptr` sections, Mach-O structures the SDK's `<mach-o/loader.h>` declares; Facebook's
fishhook's technique), and does it again for every image loaded later through dyld's
`_dyld_register_func_for_add_image` (`<mach-o/dyld.h>`). A rewritten `sigaction` calls the C library's own,
then hands the same action to libsystem_kernel's `__sigaction` with D-tramp's trampoline; a rewritten jump
drops the depth first. One sweep at registration catches what constructors installed before it.
`rebind.c`, about 120 lines, compiled with `-Wall`, exit 0, linked into the program as a static archive.

- **Rows** (`trows.c`, the same as D-tramp's): **every row reads as under D-tramp**: static archive and
  **two-level dylib** frameless rows inside, a correct callback not, both jumps and `setcontext` out of a
  handler then a correct callback not inside, a worker inside, the query the library's handler, `errno`
  1234, the context edit resumed, `backtrace()` six frames with the trampoline second, 14 deliveries through
  it. **The first build, rewriting no jump, read false refusals after both jumps**: the jumps are owed.
- **Reach** (`reach.c`, `sdlcount.c`): a library loaded by `dlopen` and its installer: **inside**; **SDL2
  reaching SDL3 by `dlopen`**: SDL3's SIGINT handler goes through the trampoline (*"before raise(SIGINT) 0,
  after 1"*); the Go archive: 3 of 3 as at the base, **139 deliveries** through the trampoline; **an installer
  reached by `dlsym(RTLD_DEFAULT, "sigaction")`: MISSED** (its hole).
- **R3's two cases** (`F/logs/rebind-r3-darwin.txt`, the thunk given `hero_tramp_inside()` for the build and
  restored, `cmp`-equal): **`flib` 20 of 20 refused through the static archive, 20 of 20 through the
  two-level dylib**; storms **20 of 20 armed `exit -6`, 20 refusal lines**, and 5 of 5 through the dylib;
  **all 25 crash reports' pids are the storms'**, SIGABRT at the thunk's first line, none SIGKILL
  (`rebind-storms-ips-join.txt`). **`otool -L` of the program names nothing outside `/usr/lib` and
  `/System`: one file.**
- **Under the hardened runtime** (`codesign -s - -o runtime`): the static-archive program refused 3 of 3 with
  R1's words: rewriting the executable's own slots passes it. (A program loading any non-platform dylib
  signed by another Team ID is refused at launch by dyld whatever the route, my test library included.)
- **Its holes, run or read**: an installer reached by `dlsym` (run, MISSED); images in the shared cache, whose
  binds are not rewritable (read: `MH_DYLIB_IN_CACHE`, skipped by design; Apple's own frameworks); a raw
  `__sigaction` naming its own trampoline (left alone, as D-tramp); a handler installed by an image's
  constructor that runs before the executable's registration (read: dyld runs a dependent dylib's
  initializers before the executable's; the sweep at registration re-installs what they installed; the Go
  archive's constructor's handlers reached the trampoline, 139 deliveries, by the rewrite or the sweep, which of
  the two not separated). It defines no C library name (every rewrite is by slot),
  so panel 239's member does not widen, by that reading; unrun through the member.

## 4. Question 2, B' and C from the C side

### 4.1 B' (248's route I): what a wrapper does to each census library's handler

My C model (`F/linux/c/inst_B.c`: a wrapper with the program's flags and `SA_SIGINFO`, the kernel's own
`siginfo_t` and `ucontext_t` handed on, a per-thread depth, no `errno` touched, the query answering the
table while the kernel's handler is the wrapper, the C library's jumps dropping the depth), Linux arm64; the
compiler-engineer's runtime build is his (`249-reports/compiler-engineer.md` § 2), read before this verdict.

- **Frames** (point 4): one more frame between the delivery and the handler; **bdwgc's suspend handler,
  which records its own frame, intact 5 of 5** (tarball and Debian's), the wrapper's frame being on the same
  stack. **Context** (point 6): the Go archive's preemption and its recovered nil panic, both context edits,
  **3 of 3 as at the base** (so the wrapper's hand-on of the kernel's pointer is enough); the
  compiler-engineer measured preemption through his wrapper on Darwin too. **`errno`**: the model touches
  none. **The query** (point 5): it answers the program's request, not the kernel's action: the masks read
  back from the table hold SIGKILL (9) and SIGSTOP (19), which the kernel strips (`log-a.txt`, `fp-go-B` and
  `uv, B`: *"mask 1,...,9,...,19,..."* against the base's without them): a visible difference a program's
  own query sees.
- **A raw `rt_sigaction`** (`rawrow.c`, run): **missed under B' and under A** (the route's question 0 inside
  its handler, 1 for the same library through `sigaction`), and the C library's query then answers the raw
  handler under both (the kernel's action, the wrapper not being installed). **What could see it**: a seccomp
  filter sees the call by number and by the calling instruction's address (`seccomp_data`'s
  `instruction_pointer`, seccomp(2), manpages-dev 6.9.1-1, read), and `SECCOMP_RET_TRAP` sends the thread a
  SIGSYS; **but "if execve(2) is allowed, the existing filters will be preserved across a call to
  execve(2)"** (the same page), and installing one needs `no_new_privs` or `CAP_SYS_ADMIN`: every child the
  program starts would carry the filter, its own C library's `sigaction` trapped by it, and a set-user-ID
  child would lose its privileges (read, unrun). `PR_SET_SYSCALL_USER_DISPATCH`: its page is not in that
  package (`man2const` absent), **unread**; a question.
- **The depth after a jump**: my model drops the depth at the C library's jumps and so reads no false refusal
  (the rows, `log-b.txt`); **the compiler-engineer's build keeps stack-pointer entries and refuses a correct
  program** that calls back from deeper than an abandoned wrapper after any of the three jumps
  (`compiler-engineer.md` § 2.3, his run). Dropping the depth at the jumps trades that false refusal for a
  miss when a jump stays inside a handler; neither is free.
- **A handler left by `setcontext`, Linux** (`setctx_l.c`, container `fp-249-7`, `F/linux/log-e.txt`): base *"not
  inside"*; A *"not inside"* (glibc's `setcontext` gives the saved mask back); **B' dropping its depth only at
  the jumps: *"INSIDE (would be refused)"*, a false refusal**; with `setcontext` and `swapcontext` defined to drop
  it too (`inst_B2.c`): *"not inside"*, and every C row as before (frameless static and shared refused, both
  jumps then a correct callback not refused, every signal blocked then a correct callback not refused). The
  same row cost D-tramp the same repair on Darwin (§ 3.3).
- **On Darwin** B' as built in the executable misses a two-level dylib's installer (248, `ffi-pragmatist.md
  :130-136`, carried; the compiler-engineer's Darwin arm defines three names in the executable); reaching it
  needs the dylib (§ 3.6) or the rewrite (§ 3.7).

### 4.2 C: whether a library as shipped keeps frame records in its handler

- **R3's library**, built `-O2 -fomit-frame-pointer`: Darwin `stp x29, x30` and no `mov x29, sp` (248,
  carried, and my `objdump` in `F/c/tramp/h`); **Linux arm64 `str x30, [sp, #-16]!` and no frame record at
  all** (`log-b.txt`), which the walk misses at the base (every frameless row MISSED). C reaches neither: the
  library is built apart.
- **Homebrew's handlers on this Mac** (`F/logs/frames-darwin.txt`, `otool -tV`): libuv's `uv__signal_handler`,
  ncurses' `handle_SIGINT` and `handle_SIGTSTP`, GLib's `g_unix_signal_handler` each `stp x29, x30` then `add
  x29, sp, ...`: **frame records**; SDL3's `SDL_HandleSIG` and ncurses' `handle_SIGWINCH` are leaves that call
  nothing.
- **Debian's on arm64** (`log-b.txt`, each handler's object and offset by `dladdr` after the library's
  start, then `objdump -d`): libuv, SDL3, ncurses (SIGINT, SIGTERM, SIGTSTP), GLib, bdwgc (SIGSEGV, SIGBUS,
  SIGPWR) each `paciasp; stp x29, x30; mov/add x29`: **frame records**; ncurses' SIGWINCH and bdwgc's SIGXCPU
  handlers are leaves.
- So **on both arm64 legs the census libraries as shipped keep frame records wherever their handler calls
  out**, and C changes nothing there (the compiler-engineer measured Darwin's objects byte-identical with
  the word); C's reach is x86-64's own units, and it leaves the library built apart and, on Debian's x86-64
  glibc, the C library's own `atexit` and `fork` frames (the compiler-engineer's emulated run, § 3.1 of his).

## 5. Question 3, from the C side

### 5.1 D-imports from the library's side

- **Which import an installer** (00-shared.md's seed, one list on both legs, carried and re-read in my traces):
  Darwin: libuv, libcrypto (3 and 4), SDL3, ncurses and the Go archive; **not** sqlite, libssl, curl 8.23.0,
  json-c, raylib, **SDL2 (sdl2-compat)**. Linux arm64: bdwgc, libuv, libcrypto, SDL3, SDL2, ncurses, curl
  8.14.1; not libssl, sqlite. **Which install a handler that could call back into the program**: none of
  them, by the runs (§§ 1.2, 1.3): libuv, GLib and SDL defer to their loop or post an event, ncurses handles
  the terminal itself, OpenSSL's prompt sets a flag, curl installs `SIG_IGN`, bdwgc's and Go's are their
  runtimes'. **So every program D-imports would refuse among the census's would be a false refusal**; the
  compiler-engineer counted one such program in the tree, correct (`compiler-engineer.md` § 6.1).
- **What a reading of imports misses, built and run**:
  - **a library loaded by `dlopen`** (`F/c/dimp/`): a bound library whose imports on the list are `dlopen dlsym
    raise` loads one importing `sigaction`, whose frameless handler calls the program back: **at the base the
    Heroes function runs inside the signal, 3 of 3, exit 0, in silence**; D-imports reads the bound library and
    passes it; **D-tramp refuses it 3 of 3** and D-rebind's rows reach the loaded library; on this Mac
    **sdl2-compat installs SDL3's handlers** that way (§ 1.2), harmless ones;
  - **an installer reached by `dlsym`** (`libdlsym.so`, whose list imports are `dlsym` alone): A's and B''s
    definitions see `RTLD_DEFAULT` and **miss `RTLD_NEXT`**; D-rebind misses `RTLD_DEFAULT`;
  - **a raw system call** (`rawrow.c`): missed by A and B' (§ 4.1); libuv 1.50's raw calls name no signal
    call (its source), Go's archive's masks, alternate stacks and `tgkill` are raw `svc` of its own;
  - **a static archive's member pulled in only by a symbol** (`libmix.a`): `nm -u` of the archive counts
    `sigaction` 1, the linked program 0: a reading of the archive refuses a program that does not contain the
    installer, unless it reads the link.
- **Panel 247's veto reaches D-imports**: its selection is a name list (a library's imports instead of a
  callee's name), and the `dlopen` program above runs Heroes code inside a signal in silence through its hole,
  compiled and run: the shape 247 vetoed (`docs/panel/247-*.md:293-294`). **I veto D-imports as a route for
  2522** on that program.
- **A refusal at a library's first install** (a function installed while the program has handed C one):
  measured installs at start-up are the Go archive (its constructor, before `main`, and SIGUSR2 at `Notify`),
  SDL (at `SDL_Init` with events), ncurses (`newterm`), bdwgc on Linux (`GC_INIT`, and incremental mode),
  libuv and GLib (when the program asks for a signal, and each takes a function). Each of those programs that
  hands C a function first (an SDL event watch, a libuv callback, a collector's finalizer) would be refused,
  and none is 2522's shape: **correct programs refused**, the `blocking` class. Object.

### 5.2 What a binding's author must learn, in the escape's words

- **A (Linux)**: a reserved signal: the marker (`SIGRTMAX`, or the number glibc's allocator hands out, which
  then moves `SIGRTMAX`); a program or library using it dies by it (run). And the widened diagnostic.
- **A and D-tramp (Darwin)**: a file beside every program that hands C a function, which a copy of the
  program alone dies without, before `main`, in dyld's words (run).
- **D-rebind (Darwin)**: nothing a binding's author writes; what is not reached is a library that finds its
  installer by `dlsym`, or Apple's own frameworks.
- **B'**: the widened diagnostic (the compiler-engineer's run); the query answers the program's own request.
- **D-imports**: a library refused by its imports, a new diagnostic.

### 5.3 Whether any answer short of a route closes 2522 under R3

**No.** Every route built leaves a residue of libraries that install past the C library's own names, and in
it the Heroes function runs inside the signal in silence: on Linux a raw `rt_sigaction` and a `dlsym(RTLD_NEXT)`
installer under A and B', and a handler that unblocks first under A; on Darwin a `dlsym` installer under
D-rebind, a raw `__sigaction` with its own trampoline under D-tramp and D-rebind, and Apple's frameworks under
D-rebind. **No first entry of the census falls in that residue** (every one installs through the C library's
names, traced on both legs), **but a library I built does, compiled and run.** So closing 2522 under R3 as
written is not reached by any route measured; **it needs the author's change to R3**, in those words: a
residue told (the class of a library that installs a handler past the C library's installers), and the
synthesis says whether it is told, refused or kept.

## 6. Verdict

**Windows: owed.** The box was off (00-shared.md); nothing here was run there. 248's rows, carried: an
executable cannot define `signal` beside UCRT, a console's SIGINT runs on a thread Windows makes, and 247's
R10 leaves no probe and no walk there; no candidate of this sitting builds on Windows, and none is needed by
those rows. The CI's Windows leg judges after a push.

### Question 1, the census

- `verdict`: **approve** the census as the first entries' measurement; **the I-onstack veto stands and
  widens**: Debian's own `libgc1` faults under it too (5 of 5 exit 139, beside the tarball's 5 of 5); **no
  other candidate is refused by any first entry** on either POSIX leg.
- `section`: design.md §1.11 (everything a program needs comes from C libraries, so the runtime must not
  change what a library's own handler does) and §1.12 (*"must not segfault and must not corrupt memory"*).
- `experiment`: two tracers (Darwin by one `__interpose` dylib over libsystem_kernel's entry points, Linux by
  `LD_PRELOAD` over glibc's installers, waits, senders and `syscall`), a snapshot of every action and mask
  before and after each start, one driver per library, compiled with Apple clang 21.0.0 and Debian clang
  22.1.8 against the real headers, **every one accepted**; the libraries' sources at Debian's versions read
  inside the container; a Go archive from C and inside a Heroes program on both legs.
- `argument`: Seven first entries install handlers; none calls the program back from one; all install through
  the C library's names; none touches 64 or SIGEMT. Go owns SIGURG before `main` and survives the probe's
  borrowing. Two things the briefs did not hold: **the runtime's own null arm already breaks a Go archive's
  recovered nil panic at the base** (exit 134, a false message, both legs, a defect for the coordinator), and
  **bdwgc on Darwin takes no signal at all**, incremental mode included, so I-onstack's veto program is
  Linux's (now Debian's build too).
- `prediction`: **Debian's SDL3 3.2.10 on a Linux console with `SDL_VIDEODRIVER=kmsdrm` takes signal 36
  (`SIGRTMIN + 2`) for its VT switch, never 64**, by `find_free_signal`'s upward scan; and a Heroes program
  binding Go 1.27.3's archive keeps dying at its first recovered nil dereference until `stack.c`'s null arm
  stops claiming faults on threads whose handler it found installed before it.
- `condition`: a library a binding plausibly links measured to block, wait for or send 64 on Linux (or the
  allocator's number), or to install a calling-back handler past the C library's names; or a collector on
  Darwin measured to need the interrupted stack.

### Question 2, the candidates

- `verdict`: **Darwin: approve D-rebind** (§ 3.7, a route nobody listed, built and run), its residue told;
  **object to D-tramp** (sound as run, priced at a second file beside every program, dominated by D-rebind),
  **object to A on Darwin** (the same file, two false refusals in 248's dylib as built, SIGEMT's users
  killed), **object to the sweep** (silent holes of 2522's class). **Linux: approve B' only with the C
  library's exits (the jumps, `setcontext`, `swapcontext`) dropping the depth** (my model; the
  compiler-engineer's build as it stands refuses a correct program, his § 2.3), **or A with its marker taken by
  glibc's own allocator** (`__libc_allocate_rtsig(0)`); every approval here stands only with its residue told,
  which is the author's change to R3 (Question 3); **veto A with the marker
  left visible as `SIGRTMAX`**; **object to C as a route** (approve it only as x86-64's complement for the
  program's own units). **The I-onstack veto stands.**
- `section`: design.md §1.11 and §4.19 (the FFI: a library's handler, its stack, its mask and the numbers it
  may use stay its own) and §1.12 for the vetoes.
- `experiment`: §§ 2 to 4: A in C and built into the Linux runtime (R3's cases 20 of 20, shared, `-O2`,
  `--sanitize` at `-O0` and `-O2`, storms 20 of 20 armed, the golden `run` form narrowed); D-tramp and
  D-rebind as dylib, archive and rows on Darwin (R3 20 of 20 static **and two-level**, storms 20 of 20, 31 and
  25 crash reports every one joined to its storm, none SIGKILL); B' modelled in C; every census first entry
  under each; every piece of C **accepted** by clang on its platform, the runtime's under its own flags.
- `argument`: On Darwin only an interposer reaches a two-level dylib; D-rebind does it from the executable,
  one file, no signal taken, no name defined, hardened runtime passing. On Linux A's marker is a signal a
  C program may use: blocking `SIGRTMAX` and waiting for it **dies in silence**, exit 192 (`swait_rt.c`), which
  glibc's allocator narrows to programs naming 64 by number. B' takes no signal, keeps every census
  library's behaviour, and sees an unblocking handler A misses, but only once the exits drop its depth.
- `prediction`: **R3's `fliblocale` against a Homebrew-shaped two-level dylib is refused 20 of 20 under
  D-rebind on each macOS the CI runs**; and B' with the exits defined **adds at most 5 instructions a callback to panel
  247's `qsortcost` ladder on Linux arm64** where no handler runs (the compiler-engineer measured 4.0 on
  Darwin).
- `condition`: for D-rebind, a library a binding plausibly links measured to reach `sigaction` by `dlsym`, or
  an Apple framework's handler measured to call a program's function; for B', a correct program refused
  under it (an exit the C library's names do not cover: a C++ exception through the wrapper, unrun); for A,
  a library measured to cache `SIGRTMAX` before the runtime's constructor runs.
- **Vetoes, each with its program**: **I-onstack**: `gcreal.c` with Debian's `libgc1` and with the tarball,
  5 of 5 `Segmentation fault` each (`log-a.txt`); **A with `SIGRTMAX` visible**: `swait_rt.c x 0`, a correct C
  program blocking `SIGRTMAX` and waiting for it, killed by signal 64, exit 192, saying nothing (`log-a.txt`,
  `log-c.txt`).

### Question 3

- `verdict`: **veto D-imports as a route for 2522**; **object to a refusal at a library's first install**;
  **closing 2522 needs the author's change to R3**, in those words.
- `section`: design.md §4.19 and §1.12; the document does not cover a told residue of the C boundary: that
  is R3's, the author's.
- `experiment`: § 5: a bound library importing `dlopen dlsym raise` loads one that installs a frameless
  calling-back handler: the Heroes function runs inside the signal 3 of 3 at the base, D-imports' reading
  passes it, D-tramp refuses it; a `dlsym` installer, a raw `rt_sigaction`, an archive member never linked,
  each built and run; seccomp(2) read.
- `argument`: A reading of imports is a name list with a hole a program runs through in silence, which panel
  247 vetoed for a callee's name. A refusal at first install refuses correct SDL, libuv, ncurses and bdwgc
  programs. Every route built leaves libraries that install past the C library's names, Heroes code then
  inside the signal in silence; none of the census's first entries does, a library I built does. No route
  closes 2522 under R3 as written.
- `prediction`: **no library of the census installs a handler by a raw `rt_sigaction` or by a `dlsym`ed
  `sigaction`** (traced on both legs today); the first that does will be found by its own trace, not by its
  imports.
- `condition`: a route measured to see an installer reached by `dlsym` and by a raw system call without
  leaving every child of the program a seccomp filter; then a closing under R3 would exist.

**What the escape binds** (00-shared.md § The lane): A needs **a reserved signal** and **a widened
diagnostic**; B' **a widened diagnostic**; D-tramp and A's Darwin arm **a file beside every program**; D-imports
**a new diagnostic**; D-rebind, by my reading, none of the four (no spec sentence, no diagnostic, no file, no
reserved signal: it rewrites slots and defines no C name), its residue the author's under R3. Every refusal's
words are R1's, which defect 2533 calls false until it lands.

Finished 2026-11-15 00:33 CET (`date`). Every container of mine ended (`docker ps --filter name=fp-249`: none); `pgrep -f 249-ffi-pragmatist`: none but my own shell; the trunk's tree, every other seat's folder and every lane untouched; `F/tree/runtime/parts/sigguard.c` `cmp`-equal to `722ae76bf`'s after each prototype build.

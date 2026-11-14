# Panel 248, the completeness critic's first pass, over the briefs

Started 2026-11-14 09:14 CET (`date`). Base `df7612ad3dc62dcc6d90bccc650adf6b9736078f`, tree
`f72f48da250e919a68ebc6a98be0facccc29a052` (`git rev-parse HEAD` and `git rev-parse 'df7612ad3^{tree}'`
in `.claude/worktrees/p248`, both printed those). Read whole: `00-shared.md` (566 lines by
`wc -l`), `compiler-engineer.md` (146), `ffi-pragmatist.md` (149), `critic-briefs.md` (64). Every check below
was run by this critic in this session; its working files are under
`.claude/worktrees/p248-scratch/critic/`. Written as it goes; the numbered list is at the end.

## A. The mandate and the base (00-shared.md, its first 95 lines)

- **Base**: `git rev-parse HEAD` printed `df7612ad3dc62dcc6d90bccc650adf6b9736078f`, `git rev-parse
  'df7612ad3^{tree}'` printed `f72f48da250e919a68ebc6a98be0facccc29a052`, `git log -1` printed
  *"Batch 35 round closed: the compiler counted again, every gate read green"*, Sat Nov 14 00:53:52
  2026 +0100. As the brief says. `git status --short` now prints `?? docs/panel/248-briefs/` (the
  briefs themselves), so "printed nothing when these briefs were begun" is unfalsifiable now and
  harmless.
- **CLAUDE.md § 4, lines 200-201**: `grep -n 'Ask once' CLAUDE.md` printed `200:`, and `grep -n
  'convene without asking again'` printed `201:`. Right.
- **Sittings 234 to 247**: the brief's `ls | grep | sed | sort -u` printed `234 ... 247`, every
  number; `grep -l 'M-core-packages' docs/panel/2*.md` printed 234, 235, 240, 241, 242, 243, 246,
  247. Right.
- **"236 to 239 name M-package-manager in their first twelve lines": false for 236.** The brief's own
  command, `head -12 <f> | grep -o -E 'M-[a-z]+(-[a-z]+)+'`, printed nothing for
  `236-a-large-group-record-...md` and `M-package-manager` for 237, 238 and 239; `grep -n -o` over 236
  finds the name first at its line 75. Harmless to the conclusion, false as written.
- **"Panel 245's status says the gate asked once for the milestone, at panel 242" (`245-*.md:6`)**:
  the words are there (lines 6-7), but `grep -o -E 'M-[a-z]+(-[a-z]+)+' docs/panel/245-*.md` prints
  nothing: 245 never names which milestone. The citation that does is **panel 242's own status,
  lines 3-4**: *"convened without asking again (the author's panel gate for M-core-packages was asked
  at 07:35 on 2026-11-10)"*, and panel 247's lines 3-4, *"convened ... by the coordinator of
  M-core-packages under CLAUDE.md § 4 (a sitting asked for once per milestone; this milestone's run
  from 234)"* (`sed -n '1,8p'` of each). Repair: cite 242:3-4, not 245:6.
- **Defect 2522** (`gh issue view 2522 --repo heroes-lang/heroes --json ...`, into
  `critic/issue-2522.txt`): `OPEN`, `M-core-packages`, `kind:defect,class:blocking,area:runtime`, 0
  comments, filed 2026-11-13; its *What happens* carries *"20 of 20 storms"* and *"5 of 5 ... 1.35 to
  2.17 million signals"*, and its *Where to look* the sentence the brief quotes. Right.
- **Panel 247's lines**: `grep -n -E '^\*\*R[0-9]+\.'` printed R1 153, R2 183, R3 191, R4 197, R5 213,
  R6 248, R7 255, R8 273, R9 280, R10 286, and *"What it does not: R4's interposition"* stands at 449.
  Every citation the briefs make into 247 lands.

## B. Item 1: Q1's storms, and whose SIGKILL it is

**The driver** (`.claude/worktrees/p248-scratch/briefs/tools/storm.py`, read whole, `shasum -a 256`
`8e6b762aaff7...` for both it and `p247-scratch/tools/storm.py`, and `ips.py` `92071c5e2328...` for
both: the brief's hashes hold). It sends SIGKILL to the target's group in exactly two places: line 45,
only after `p.wait(timeout=30)` timed out, which would print *"NOT ended 30 s after the storm"*; and
line 49, only after `p.wait` has already returned the exit code. So an `exit -9` the driver prints was
not the driver's SIGKILL: the code was fixed before the driver sent one.

**The logs**: `r2522/storms.log` printed `pid 52205; sent 17777 SIGUSR1; exit -9`, `pid 52208; sent
17070 ...; exit -9`, `pid 52211; sent 17325 ...; exit -9`, each `out| armed` and no `err|` line,
between `08:59:40` and `08:59:41`, so each run ended long before its 4 s storm. **The counts are what
the log prints** (17,070 to 17,777). `r2522/linux/linux.log` printed seed exit 0, stage 2 exit 0,
`sources 4c9a6a9800095eef`, `build exit 0`, then `pid 1044; sent 2240205 ...; exit 0`, `pid 1049;
sent 2102440`, `pid 1054; sent 2109982`, each `built 20000000 words`. **2,102,440 to 2,240,205: what
the log prints.** `cmp tools/storm.py r2522/linux/storm.py` printed nothing (the same driver).

**The reports**: `SHOW=1 python3 -I ips.py ~/Library/Logs/DiagnosticReports fliblocale` (a copy in
`critic/`) printed `('fliblocale', 'SIGKILL', 'lock') 3` and the stack the brief quotes, word for
word. Their pids, read from the three `fliblocale-2026-11-14-085941*.ips` files, are **52205, 52208,
52211, the storm log's three**; `parentPid` 52204, `captureTime 08:59:40.8943`, `procLaunch
08:59:40.5157`. Each report's `exception` reads `{'type': 'EXC_BREAKPOINT', 'signal': 'SIGKILL'}`,
`termination` `{'namespace': 'FOUNDATION', 'flags': 2, 'code': 1}`, `asi` *"BUG IN CLIENT OF
LIBPLATFORM: Trying to recursively lock an os_unfair_lock", "Abort Cause 259"*.

**Whose SIGKILL, run**: a plain C program with no Heroes runtime that locks one `os_unfair_lock`
twice (`critic/sigkill/relock.c`, `clang -O0`, run under `timeout -s KILL 20`) printed `locked once`
and `exit 137` (128 + 9), and its report `relock-critic-2026-11-14-091644.ips` reads the same
`EXC_BREAKPOINT` / `SIGKILL` / namespace `FOUNDATION` code 1 / the same `asi`, stack
`_os_unfair_lock_recursive_abort < _os_unfair_lock_lock_slow < main < start`. So the SIGKILL is
**Darwin's**: libplatform traps (`brk`, `EXC_BREAKPOINT`) on the recursive lock and the system ends
the process with SIGKILL; neither the driver nor the Heroes runtime sends it. The brief's
*"a SIGKILL from Darwin's libplatform"* is right in substance; precisely, libplatform traps and the
report names SIGKILL. `pgrep -f p248-scratch/critic` printed nothing after.

## C. Item 2: the installers' lists, re-run

**Darwin, the brief's own command** (`tr -s "[],' \n" '\n' < $SDK/usr/lib/system/<lib>.tbd | grep -x
-E '_{1,3}(sigaction|...|sigaction_self)'`, SDK `MacOSX.sdk` of Xcode): `libsystem_kernel`
`___sigaction`; `libsystem_c` `_bsd_signal _sigaction _sighold _sigignore _siginterrupt _signal
_sigpause _sigrelse _sigset _sigvec`; `libsystem_platform` and `libsystem_pthread` none. As the brief
says. The header lines hold: `grep -n` printed `signal.h:82` `bsd_signal`, `87` `sigaction`, `102`
`sigset`, `111` `sigvec`, `sys/signal.h:392` `signal`.

**But the filter is a name list, and it misses four exports that install a handler.** Every
`sig`-prefixed export of the same three stubs (`grep -E '^_+(sig|bsd_sig|platform_sig|psig)'`)
printed, beside the brief's: `libsystem_c` **`__sigaction_nobind __signal_nobind __sigvec_nobind`**
(C names `_sigaction_nobind`, `_signal_nobind`, `_sigvec_nobind`) and `libsystem_platform`
**`___platform_sigaction`** (C `__platform_sigaction`), with `__sigtramp`, `_siglongjmp`,
`_sigsetjmp`. None is declared in the SDK's headers (`grep -rn -E
'_sigaction_nobind|_signal_nobind|_sigvec_nobind|__platform_sigaction' $SDK/usr/include` printed
nothing). **Run** (`critic/darwin-installers/nobind.c` and `platform.c`, `clang -O0`, each under
`timeout -s KILL 20`): `_sigaction_nobind ret 0 handler ran 1`, `_signal_nobind ret 0x0 handler ran
1`, `_sigvec_nobind ret 0 handler ran 1`, `__platform_sigaction ret 0 handler ran 1`, and
*"calls that reached the executable's own sigaction: 0"* with the executable defining `sigaction`.
So **Darwin exports at least ten names that install a handler, not six**, and a library calling any
of the four undeclared ones passes a runtime's definition of the six. Route I's *"every installer
each C library exports defined with its whole meaning"* (R4) owes them, or says why not.

**glibc 2.41, Linux arm64** (`critic/linux/run.sh`, `docker run ... heroes-linux-arm64:latest`, exit
0; `nm -D --defined-only /lib/aarch64-linux-gnu/libc.so.6`, versions stripped, the brief's filter):
`__sigaction __sigpause __sysv_signal bsd_signal sigaction sighold sigignore siginterrupt signal
sigpause sigrelse sigset sigvec ssignal sysv_signal`, the brief's fifteen. The prototypes the brief
calls unread (`grep -n` of `/usr/include/signal.h`): `bsd_signal` 104, `__sysv_signal` 77,
`sysv_signal` 80, `ssignal` 127, `sigset` 367 (`__sighandler_t __disp`), `signal` 88, `sigaction`
243, `siginterrupt` 324, `sigignore` 363, each `extern __sighandler_t <name> (int __sig,
__sighandler_t ...)` for the handler takers. Two things the brief's *"nine of them take a handler"*
does not say:

- **`sigvec` is a compat symbol only**: the unfiltered `nm -D` prints `sigvec@GLIBC_2.17` with one
  `@` (every other name `@@`, the default version), and a fresh link calling it failed: *"undefined
  reference to `sigvec'"* (the first `reach.c`). So a library built today cannot call glibc's
  `sigvec`; only a binary linked against an older glibc does. Eight are linkable, one is not.
- **`__libc_sigaction@@GLIBC_PRIVATE`** is exported too (the unfiltered list), private.

**Which of them reach an executable's own `sigaction`, run at the base's image** (`critic/linux/
reach.c` and a shared library `libinst.so`, `critic/linux/linux2.log`): from the program, `signal`,
`bsd_signal`, `sysv_signal`, `__sysv_signal`, `ssignal`, `sigset`, `__sigaction` each *"handler ran 1,
reached the executable's sigaction 0"*; `sigaction` *"reached 1"*. From a shared library, `signal`,
`bsd_signal`, `sysv_signal`, `ssignal`, `sigset` reached 0 and `sigaction` 1. Panel 247's critic's
reading (none of glibc's own reaches it) holds at the base, re-run. **And two names that take no
handler still change what a definition's table would answer**: `siginterrupt` reached 0, and
`sigignore` reached 0 while *"disposition now SIG_IGN 1"*. A route I that keeps the program's handler
to answer `sigaction(sig, NULL, &old)` is made stale by a library's `sigignore` or `siginterrupt`;
the brief's list of names to define stops at the handler takers.

**The runtime's own 27** (the brief's `grep -n -E '(^|[^_a-z])(sigaction|signal)\('`, comment lines
dropped, into `critic/installs.txt`): 27 lines, at exactly the brief's files and line numbers.
Windows arms among them, by reading: `os.c:260`, `272`, `320`, `321` (`hero_lease_abrt_win`, UCRT
`signal`). Two of the POSIX ones are `signal(SIGABRT, SIG_DFL)` inside the refusal and panic paths
(`sigguard.c:139`, `panic.c:54`), which a runtime definition of `signal` would then serve from
inside a handler. **What the grep cannot see**: the runtime's Windows installers that are not
`signal`, `AddVectoredExceptionHandler` (`os.c:318`, `stack.c:618`, `stack.c:1210`),
`SetUnhandledExceptionFilter` (`os.c:319`) and `SetConsoleCtrlHandler` (`hero_net.c:539`)
(`grep -n -E '...(SetConsoleCtrlHandler|SetUnhandledExceptionFilter|AddVectoredExceptionHandler)\('`).
The same three are what a program's DLL would call; the brief's *"Windows' UCRT: unread; the seat
says which names install a handler there"* asks about UCRT alone. No other C beside the runtime
calls an installer (`grep` over `runtime/*.c`, `runtime/*.h`, `packages/**/*.c|h` printed nothing).

**The address sanitizer, re-read.** Darwin (`nm` of
`.../usr/lib/clang/21/lib/darwin/libclang_rt.asan_osx_dynamic.dylib`): `t _wrap_signal`, `t
_wrap_sigaction`, `s __ZL19substitution_signal`, `s __ZL22substitution_sigaction`: **all four are
local** (`t`, `s`), not only the two `substitution_` ones as the brief has it, and the dylib carries a
`__DATA,__interpose` section of `0x1300` bytes (`otool -l`), 304 pairs: the sanitizer is itself a
dyld interposer, loaded as a dependent dylib, with no `DYLD_INSERT_LIBRARIES`. Linux (`nm` of
`/usr/lib/llvm-22/lib/clang/22/lib/linux/libclang_rt.asan-aarch64.a`): `W signal`, `W sigaction`, `W
__interceptor_signal`, `W __interceptor_sigaction`, as the brief says, and also `T
___interceptor_signal`, `T ___interceptor_sigaction` (strong) and `U sigaction` (the sanitizer
references it). Neither sanitizer takes `bsd_signal`, `sigset` or `sigvec` by these names.

## D. Item 3: Q1's routes, route or restatement, and the routes not listed

**Does `--wrap` exist, run where present:**

| linker | command | printed |
|---|---|---|
| Apple's `ld`, `ld-27106.1` (`ld -v`) | `ld -help` | *"ld: unknown options: -help"* |
| the same | `man ld \| col -b` (1,575 lines) | **0** lines holding `wrap`; it has `-alias`, `-alias_list`, `-interposable`, `-interposable_list`, `-flat_namespace`, `-force_flat_namespace` (lines 608, 613, 1027, 1248, 618, 1079) |
| GNU `ld.bfd`, Linux image | `ld --help` | `--wrap SYMBOL  Use wrapper functions for SYMBOL` |
| `ld.lld` 22, Linux image | `ld.lld --help` | `--wrap=<symbol>  Redirect symbol references to __wrap_symbol and __real_symbol references to symbol` |
| lld's MinGW driver | `ld.lld -m i386pep --help` | `--wrap=<symbol>  Use wrapper functions for symbol` |
| `ld64.lld` 22 (Mach-O lld) | `--help` (307 lines), `--help-hidden` | 0 lines holding `wrap` |
| `lld-link` 22 | `lld-link /?` | *"warning: /machine is not specified ... error: subsystem must be defined"* (it reads `/?` as nothing); `-help` (207 lines) and `-help-hidden`: 0 `wrap`; it has `/alternatename:<value>  Define weak alias` |

So **W exists on GNU ld, `ld.lld` and a MinGW link, and on none of Apple's linker, `ld64.lld` or
`lld-link`**, by their own help. MSVC's `link.exe` cannot be run here. **And the project's Windows
target is MSVC's, not MinGW's**: `docs/platforms/windows/WINDOWS-MACHINE.md:346` and `:357` read
*"target `x86_64-pc-windows-msvc`"* for both boxes, and `ci.yml:540` *"Windows carries clang with
Visual Studio"*; `selfhost/cli/link_flags.hero:136-156` asks `msvc(target)` for `-windows-msvc`. The
briefs tell every seat to compile its Windows arm with `zig cc -target x86_64-windows-gnu`, a MinGW
target whose link **has** `--wrap`; a W arm compiled that way would build where the project's target
cannot. The brief should say which target a Windows arm stands for.

**W's premise, run: `--wrap` does not leave the runtime's calls alone.** `critic/linux/run5.sh`
(three objects: a "runtime" object calling `sigaction`, a static archive calling it, `main.o`
defining `__wrap_sigaction`; one link with `-Wl,--wrap=sigaction`) printed, for `-fuse-ld=bfd` and
`-fuse-ld=lld` alike, *"runtime object's call wrapped: 1"* and *"static archive's call wrapped: 1"*;
with the archive made a shared library, *"static archive's call wrapped: 0"*. `--wrap` redirects
every undefined reference in **every object on the link line**, and the runtime is one
(`r2522/build/runtime-3068a62d1f01ed59-407f74943346dd0f.o` beside the program's archive). The brief's
*"it leaves the runtime's 27 calls and the C library alone"* is false for the link as `heroes build`
makes it; W either calls the real one as `__real_sigaction` from the runtime (which then exists only
on a wrapped link) or splits the link. And a shared library escapes it entirely.

**Darwin's flat namespace, run** (`critic/darwin-installers/ff*.c`: an executable defining
`sigaction` that counts and forwards by `dlsym(RTLD_NEXT)`, a dylib calling `sigaction` and
`signal`): plain, `-Wl,-flat_namespace` and `-Wl,-force_flat_namespace` on the executable each printed
*"dylib sigaction: handler ran 1, reached the executable's 0"* and the same for `signal`; and the
linker printed *"ld: warning: -force_flat_namespace is no longer supported, using -flat_namespace
instead"*. So the manual's `-force_flat_namespace` (*"force flat namespace binding on all dylibs"*) is
refused by the linker it documents, and `-flat_namespace` on the executable does not reach a
two-level dylib. What the brief leaves unrun stays the seats': `__interpose` in the executable, a
dylib linked flat. **What the sanitizer shows**: its dylib carries a `__DATA,__interpose` section and
is loaded as a dependent dylib (section C), so a dyld interposer that reaches every image's imports
exists without `DYLD_INSERT_LIBRARIES`; *route I shipped as a dylib with an `__interpose` section*,
which the runtime is not today (it is linked in as an object), is a route the brief does not list.

**M, read**: Darwin's `man 2 sigaction` (lines 71-75) says the mask during a handler is *"the union of
the current signal mask set, the signal to be delivered, and the signal mask associated with the
handler"*, and `SA_NODEFER` (115-117) *"further occurrences of the delivered signal are not masked"*.
So M misses every `SA_NODEFER` handler, and a handler that calls `sigprocmask` (the same paragraph:
*"or until a sigprocmask(2) system call is made"*); it needs to know the thread's own mask outside any
handler, which every `pthread_sigmask` or `sigprocmask` of the program's C changes without telling
the runtime (the runtime itself calls `pthread_sigmask` or `sigprocmask` on 16 lines: `grep -h -E '(^|[^_a-z])(pthread_sigmask|sigprocmask)\('` over `runtime/parts` and `runtime/platform`, comments dropped, `wc -l`), so M is
either an interposition of those two (route I's problem again) or a comparison against a mask the
runtime cannot know. Glibc's `sysv_signal` installs with `SA_NODEFER` by glibc's documentation, which
this critic did not run: a seat runs `sysv_signal` and reads `sa_flags` back. **M is a route only with
an interposer under it; as stated it is a question, not a route.**

**U**: a route (another question, not another installer list). `libunwind.tbd` exports
`_unw_is_signal_frame` on Darwin (the broader `tbd` search, section C), so the symbol exists there;
whether it or glibc's `dl_iterate_phdr` takes a lock stays unread here, the seats'.

**K**: a restatement of the base, forbidden by R3 in its own words; not a route this sitting can adopt.

**Routes nobody listed, each a question for the seats:**

1. **I with `SA_ONSTACK` forced**: the runtime's installer definitions add `SA_ONSTACK` to every
   handler they install and give every claimed thread an alternate stack; then R1's second question
   (`SS_ONSTACK`, `sigguard.c:330-334`) sees every handler that went through a definition, with or
   without frame records, and no per-thread depth is kept. It shares I's coverage residue (the
   undeclared names above, a Darwin dylib, a raw system call) and changes what a C handler's stack
   is.
2. **Signals delivered only on a thread nothing claimed**: every claimed thread blocks the
   asynchronous signals and one runtime thread leaves them open, so a C handler runs where 2492's
   thread guard (`sigguard.c:321-325`) already refuses a Heroes function, with no walk. Panels 246 and
   247 weighed a `sigwait` thread to *count* a signal (`246-reports/critic-reports.md:383`,
   `247-reports/ffi-pragmatist.md:294`), not as this guard (`grep -n -i -E 'sigwait|dedicated.{0,20}thread|...'`
   over 246, 247 and their reports). Its residue: a fault's synchronous signal reaches the faulting
   thread whatever its mask; a thread-directed `raise` or `pthread_kill` to a claimed thread stays
   pending (the 2494 case *"the runtime's door counts each signal the program sends itself"*); a C
   library that unblocks on a claimed thread.
3. **A bounded scan of the stack for the delivery's trampoline** (`_sigtramp`'s or the sigreturn
   trampoline's return address, which the exact test already learns), instead of a walk by frame
   records: it sees a handler with no frame records but may read a stale word as a delivery, which
   would refuse a correct program. Unbuilt anywhere this critic searched.
4. **A thunk section refused by the installers** (the instructions' example): an installer
   definition refusing a handler whose address lies in the thunks' section closes only the direct
   case, which R1's exact test already refuses; it does not reach R3's case, a C handler that
   *calls* the thunk. A restatement, not a route.
5. **Renaming the references inside the program's static archives at build time** (an
   `objcopy --redefine-sym` step): W's effect without W's link flag, and without touching the
   runtime's object. `xcrun --find llvm-objcopy` printed *"unable to find utility"* and the image has
   `/usr/bin/llvm-objcopy` and `objcopy`, so on Darwin it needs a tool Xcode does not ship. Same
   residue as W for shared libraries.

**A question the brief should name and does not: what R3's "two cases" are.** R3 (`247-*.md:191-195`)
says *"Its cases are those two"* after naming `flib-nofp` and `c-fliblocale` in one sentence; the
compiler-engineer's item 6 and the ffi-pragmatist's 2(e) ask both seats to refuse *"R3's two cases"*
20 of 20, and `r2522/` holds `fliblocale` alone. Whether the two are `fliblocale` on Darwin and on
Linux arm64, or `fliblocale` and `flib` built `-fomit-frame-pointer` (`ev247/247-ffi-pragmatist/
linux-247.sh:115`, `flib.hero --library lib-nofp`), the brief should say, so both seats run the same
two.

## E. Item 4: Q2's measurement and the descriptor limits

**Re-run at the base, once each** (`git archive df7612ad3 runtime` into `critic/tree`, `diff` of its
`hero_net.c` against `briefs/base/tree`'s printed nothing; `clang -O2 -I ../tree/runtime waiters.c
../tree/runtime/platform/hero_net.c -o waiters-critic -lpthread`; `uptime` 1.75 2.29 10.10; each
under `timeout -s KILL 60 /usr/bin/time -l`): **100 waiting 5 s: 271,605,305 instructions retired,
10,467 involuntary switches, `100 waited`, exit 0; 100 sleeping 5 s: 21,847,841 and 226, `100
slept`.** The switches and the sleepers fall inside the brief's ranges; the waiters' instructions
fall 0.5% under the brief's lowest (273,025,580). `briefs/r2502/runs.log` prints the brief's six
figures exactly. The order of magnitude, about 12 times the sleepers, holds.

**The limits, re-read**: `launchctl limit maxfiles` printed `maxfiles 256 unlimited`; `ulimit -n`
1048576 and `ulimit -Hn` unlimited in this session; `sysctl kern.maxfilesperproc` 92160,
`kern.maxfiles` 184320. As the brief says. Which limit a program started outside this session meets
(Terminal, launchd) stays a question this critic could not run without starting a process through
launchd.

**The probe has a trap a seat will fall into under `ulimit -n 256`.** Run at the base:
`(ulimit -n 256; timeout -s KILL 30 ./waiters-critic N wait 1)` printed `100 waited`, `125 waited`,
`126 waited`, and at **127 and 130, nothing, exit 137**: killed by the timeout. `sample` of the
hung process: 1449 of 1489 samples in `poll` under `hero_net_accept`, CPU 0.01 s, `lsof` 253 IPv4.
The cause is `waiters.c:54-56` itself: it calls `hero_net_accept(l)` **before** it checks
`peer[i]`, so a `hero_net_connect` refused at the limit leaves `accept` waiting for a connection that
never comes. A copy that checks the connect first (`critic/r2502/waiters-checked.c`, one line added)
printed *"no connect 126, kind 9"* (`HERO_NET_LIMIT`, `hero_net.h:65`) and exit 1 at once. **So a
seat that runs the brief's probe as given past the limit reads "a wait that never ends" that is the
probe's, not the layer's.** The brief's Q2 asks exactly that question (*"what the layer answers there
(HERO_NET_LIMIT, a fall-back to slices, or a wait that never ends)"*); it should hand the seats the
corrected probe or warn them.

**And the probe holds both ends of every connection in one process** (`peer[i]` and `mine[i]`,
`waiters.c:42-56`), so its counts are two descriptors per connection: the lane's *"201 IPv4"* for 100
connections and *"409 descriptors instead of 207"* are the probe's, about twice what a server
holding 100 clients holds (100 accepted sockets and its listener). Whether a wake per waiting socket
fits under 256 for a **server** is arithmetic until a seat runs a server-shaped probe: at the base a
server holds about N + 6 (N sockets, listener, three streams, the stop's two ends), a pipe per socket
or per waiting thread about 3N + 6 (fits N up to 83), one `eventfd` or `kqueue` per socket about 2N +
6 (N up to 125), one shared wake N + 8. **An inference, unrun**; and `HERO_SPAWN_SLOTS` 256 caps a
thread per connection anyway (section G).

## F. Item 5: Q3, the four cases, POSIX's list and `CreateProcessW`

**The cases, re-run with the brief's commands**: `find tests/golden -name directory` printed the three
`unsupported/fixedbugs-2344-*` folders, each `directory` reading `here`; their `environment` files
read `CPATH=`, `C_INCLUDE_PATH=:` and `C_INCLUDE_PATH=/nonexistent-2344:`. `grep -l '^cd '
tests/golden/fetch/*/command` printed the one 2479 case, whose `command` holds 11 lines, 9 opening
`cd`. `platform_skips.hero:153` holds `Skip(name: "fixedbugs-2344-an-empty-cpath", why:
EMPTY_CPATH_IS_APPLES)` inside `darwin_alone()`. `case_setting.hero:155` is `function skip_here`,
`:258` the `[ENV, "-C", ...]` line; `suite_fetch.hero:148-153` its skip, `:217` `constant ENV`,
`:342-343` the `env -C` words. The 2479 case holds no link (`ls`: `app command expected idx`), so
`ln` does not also skip it. All as the brief says. The Windows net runs every suite in its parts
(`ci.yml:1259-1270`, `windows-net` from `net-plan`), so the fetch case would be judged there.

**POSIX's list, read 2026-11-14** (`curl -s -L -o critic/web/posix/v2chap02.html
https://pubs.opengroup.org/onlinepubs/9799919799/functions/V2_chap02.html`, 330,207 bytes; the page
names *"IEEE Std 1003.1-2024"*; WebFetch of the same URL agreed): section 2.4.3, its line 3017,
*"The following table defines a set of functions and function-like macros that shall be
async-signal-safe. Therefore, applications can call them, without restriction, from signal-catching
functions."* The table's two parts (lines 3021-3291) name 231 functions, extracted by `grep -o -E
'<i>[A-Za-z_0-9]+</i>\(\)'`. **`chdir` and `fchdir` stand on it** (`grep -c -x`: 1 each): the lane's
carried sentence holds. And the fork page (`.../functions/fork.html`, same day): *"If a
multi-threaded process calls fork(), ... the application shall ensure that the child process only
executes async-signal-safe operations until such time as one of the exec functions is successful."*

**What the brief asks the seats and this critic read on the same table**, the calls the child makes
already (`run.c:1333-1477`, the ffi-pragmatist's list): on it, `setpgid`, `sigaction`,
`sigprocmask`, `pthread_sigmask`, `open`, `dup2`, `close`, `write`, `_exit`, `getppid`, `raise`;
**not on it: `getrlimit`, `setrlimit`, `execvp`** (the table holds `execl`, `execle`, `execv`,
`execve`, `fexecve` and no `execvp`), and the non-POSIX `execvpe` (glibc), `execvP` (Darwin) and
`prctl` (Linux) are outside the standard. So **the child already makes calls POSIX does not promise
safe after a threaded `fork`**: the write ceiling's `getrlimit`/`setrlimit` (`run.c:1382-1400`) and
the path search of `execvp`. Q3's framing reads as if `chdir` were the first such question; it is
the safest call the child would make. The sitting should ask whether the existing ones stand, which
is a question about `hero_run_child` as it is, not about the door.

**`CreateProcessW`, read 2026-11-14** (`curl ... -o critic/web/ms/createprocessw.html
https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-createprocessw`,
76,648 bytes, `updated_at` 2025-07-01): *"[in, optional] lpCurrentDirectory: The full path to the
current directory for the process. The string can also specify a UNC path. If this parameter is NULL,
the new process will have the same current drive and directory as the calling process."* **The page
says nothing of a relative path or a missing folder**: split into sentences, `grep -i -E
'relative|does not exist|ERROR_DIRECTORY|PATH_NOT_FOUND'` finds none that concerns
`lpCurrentDirectory`. Both stay owed to a run on Windows. Two sentences of the same page the brief
does not carry, each bearing on the door:

- *"If the file name does not contain a directory path, the system searches for the executable file
  in the following sequence: The directory from which the application loaded. The current directory
  for the parent process. ..."*: with `lpApplicationName` NULL, as `run.c:1794` calls it, the
  program is searched in the **parent's** folder whatever `lpCurrentDirectory` says, where POSIX's
  `execvp` after a `chdir` resolves a relative path in the child's. The two platforms' answers to
  *"the program's own path when it is relative"* can differ; the brief asks for one order.
- *"If an application provides an environment block, the current directory information of the
  system drives is not automatically propagated to the new process ... An application must manually
  pass the current directory information"* (the `=C:` variables): `hero_run_go` passes a block
  (`run.c:1636`, built from `GetEnvironmentStringsW` at `:390`). Whether the staged block keeps the
  `=X:` entries, so a drive-relative path in the child resolves, is a question nobody listed.

## G. Item 6: Q4 and Q5's facts

- **The thirteen doors**: the brief's command printed 13, `accept catch_stop close connect last_error
  last_os_error listen number of port recv send stop_asked`. Right. (`hero_net.h` holds 15 declaration
  lines, `grep -n -E '^[a-z_ 0-9*]+ \*?hero_net_[a-z_]+\('`; two of them, 112-113, are the comment's
  Heroes spellings of `number` and `of`.) `grep -n -i -E 'ask|request'` printed 68, 73, 130, 136, 164,
  165, 166, 168: right.
- **`<core>/net` binds no `hero_net_stop_asked`**: `grep -rn 'stop_asked' packages/ examples/
  tests/golden --include='*.hero'` printed lines of the two `run` cases
  (`the-platform-layer-stops-a-server-on-an-interrupt.hero:17,62`, `...-on-a-request-to-end.hero:15,60,64`)
  and nothing under `packages/` or `examples/`; `net/hero_net.hero:44` binds `hero_net_catch_stop`,
  `net.hero:203` is `function catch_stop() -> ()?`. Right. `packages/core/versions` lists `0.3.0
  sha256-54d8...`, published (`git log`: `abc1e3f47 M-core-packages step 16: core 0.3.0 published`),
  and `examples/packages/README.md:52-53` holds *"A published version is never edited: a change to a
  package is a new version"*. Right.
- **No timeout door**: the brief's grep re-run printed exactly the brief's lines (`hero_net.h:62`, `131`;
  `net.hero:41-42`; `net/hero_net.hero:29`; `net/internal/failure.hero:52-53`; `net.md:38`;
  `net/hero_net.md:8`) and none in `net/http.md`, `net/http.hero` or `net/http/internal/*.hero`.
  `hero_net.c:264-265` and `291-292` map `WSAETIMEDOUT` and `ETIMEDOUT` to `HERO_NET_TIMED_OUT`. Right.
  *"No sitting ruled one"*: the brief's `grep -l ... docs/panel/*.md docs/panel/*/*.md` now prints the
  three 248 briefs themselves (they hold *SetReadDeadline*), nothing else; harmless. 246's
  critic-reports lines 371-375 and 512-514 and 246's 424-426 say what the brief says.
- **`HERO_SPAWN_SLOTS` 256 at `spawn.c:105`**, `hero_thread_limit` at `:634` returning it: right.
- **`served`** (`examples/server/main.hero`, read whole at 55-170): `most: 1` and `room:
  hero_thread_limit() - 1` at 63 (the loopback shape keeps one slot for its client), `room:
  hero_thread_limit()` at 86, `listening` 94-96, `served` 110-136 with the join at 115-117, `STOPPED`
  128-131, `connection` 151-158, `answer` 162-165. Right. **But the brief's inference is narrower than
  the code.** `served` joins `threads[0]` whenever `threads.len() >= room` (115-117), and a thread
  leaves `threads` only there: a finished thread keeps its place until it is the oldest. So the shape
  that stops the accept is not *"256 connections whose oldest sends nothing"* held at once: by reading,
  **one quiet connection accepted first, followed by 255 that finish**, stops it as surely. A reading,
  unrun; the seats' probe should run that shape beside the brief's, bounded (`timeout -s KILL` on the
  server *and* on the clients, `curl --max-time`), and `hero_os.h` offers no join that does not wait
  (`grep -n -E '^[a-z_0-9 *]+hero_thread_[a-z_]+\('`: `spawn` 441, `spawn_sized` 447, `join` 449,
  `limit` 453), so the example cannot join whichever finished. A route nobody listed for Q5: a join
  that does not wait. It would move spec § 13's sentence naming `hero_thread_join` (line 377), so it is
  the full panel's.
- **The batch 35 notes search**: the brief's own `grep -rl ... .claude/worktrees/b35-scratch` re-run
  printed 49 files: 24 under `b35-ffi/emitcmp` (build outputs), 10 `b35-signals/runs2`, 3
  `t2298/heroes.packages`, `close/*`, `b35-signals/swp6`, `msg-spec-247.txt`, `fix/msg-skips.txt` and
  three `b35-publish/driver-added*.txt`. The brief names some of those kinds and not the emitter
  outputs or the driver's emissions; the matches read (`driver-added.txt:134` a string *"the program
  was asked to stop"*, `fix/msg-skips.txt:8`, `said.txt:1023` the declaration of `hero_net_stop_asked`)
  hold no such note. The conclusion stands; the list is incomplete.

## H. Item 7: the lane for Q4's and Q5's faces

`.claude/skills/panel/SKILL.md:18-23` and `:29` say what the brief quotes. **The brief does not quote
`:33-34`, which decides Q4**: *"A question that *looks* internal but has a sentence in the spec behind
it is a full panel."* Spec § 13's line 404 (`sed -n '403,404p' spec/heroes-spec.md`) names
`hero_net_catch_stop() -> i64` and `hero_net_stop_asked() -> i64` and says what makes the second
answer 1. A door that makes it answer 1 another way has that sentence behind it whether or not
*"only"* becomes false, so by `:33-34` Q4 is the full panel's, not a question the soundness lane
may keep. And the precedents for each door Q3 to Q5 sit beside were full sittings: panel 242,
whose R6 made `hero_run_env` (`242-*.md:176-188`, ratified at `:276`), sat four seats (its status,
lines 3-7); panel 246's stop (R4) was the full part of that sitting (*"3a, a full sitting on the
server step's two questions"*, its lines 3-8); panel 247, which put hg3 in the spec, sat in full.
**Q5's face is a new function of `<core>/net` and a card line**; SKILL.md names no package card and
CLAUDE.md § 4's triggers name none, so whether a card is reader-facing in SKILL.md's sense is a
question the brief rightly poses; panel 246's resolution (`246-*.md:351-355`) is the one record that
weighed a card, and it kept the stop's card at 0 spec tokens **inside a full sitting**.

**And Q1 is not cleanly in the lane either.** 00-shared.md:25-28 says no question is expected to
change *"what the checker refuses"*; Q1's own question 1 asks whether the runtime's definition of
`sigaction` makes a program's library defining it *"a refused program"* under panel 239's member, and
R4 asks that member *"amended in the same commit"*. A route that defines `bsd_signal`, `sigset`,
`sigvec`, `ssignal`, `sysv_signal` or the four undeclared Darwin names widens which libraries
`ffi_defined_elsewhere` refuses (the runtime does not call them today, `critic/installs.txt`), a
change to what a diagnostic class covers. The escape (00-shared.md:34-37) names *"a new diagnostic"*;
it should name *"a widened one"* too.

## I. Item 8: the briefs' bounds

- **No paid run**: `grep -n -i -E 'claude -p|--refresh|paid|api call|cloud run|USD|cost'` over the three
  seat files found only the prohibitions (00-shared.md:9, 132-133), the Windows box's *"Paid by the
  hour"* (174) and *cost* in the sense of instructions. No brief names one. Right.
- **The Windows box**: written as off (00-shared.md:171-183) and owed to the CI's leg; re-run, `ssh -o
  BatchMode=yes -o ConnectTimeout=15 win 'echo up'` printed *"Could not resolve hostname apponfly-vps"*,
  exit 255. Right.
- **Each seat's copy and compiler its own**: 00-shared.md:119-122, both seat briefs name their own
  folder and tree. Right. **But one thing they share and the briefs do not split: the crash reports.**
  Both seats are pointed at 00-shared.md:199's `-o fliblocale` and its `ips.py ... fliblocale`; `ips.py`
  takes every `<name>-2026-*.ips`, and `ls ~/Library/Logs/DiagnosticReports | grep -c '^fliblocale-2026-'`
  already prints 3 (the coordinator's). Two seats storming a binary of the same name each read the
  other's reports and the coordinator's. Repair: a binary name per seat (`ce-fliblocale`,
  `fp-fliblocale`), or reports filtered by the pids the storm prints.
- **The storm arms by the clock, not by the program**: `storm.py` sleeps `<arm-delay>` and starts
  sending whether or not the program has installed its handler (lines 33-38). This critic's C twin
  (section J) read, in its first of three runs, *"sent 66 SIGUSR1; exit -30"*, no `armed` line: the
  storm's own SIGUSR1 (30 on Darwin) killed it before it armed, on a machine where other sessions
  build. A seat counting *"N of N"* must count only runs whose output holds `armed`.
- **The Linux leg is not bounded by its `timeout`**: `timeout -s KILL 8 docker run -i --rm --name
  critic-248-bound heroes-linux-arm64:latest sleep 40 </dev/null` printed exit 137, and `docker ps
  --filter name=critic-248-bound` then printed *"critic-248-bound Up 6 seconds"*: killing the client
  leaves the container running (stopped by this critic with `docker stop`, then gone by `--rm`). And
  `pgrep -f <your folder>` does not see a container's processes. 00-shared.md:164's line therefore
  bounds the wait, not the run: a hung probe inside goes on. Repair: `docker run --name <seat>-<n>`,
  `docker ps` read and the container stopped after a timeout, and every probe inside the script under
  its own `timeout -s KILL` (the image has GNU `timeout`; this critic's scripts used it there).
- **Programs that may not end, by brief**: the server of Q4 is bounded in the compiler-engineer's brief
  (120-121); the waiters, the quiet-peer probes of Q5, the ThreadSanitizer probe of the ffi-pragmatist's
  Q4 and its threaded fork-and-`chdir` stress of Q3 are bounded only by 00-shared.md's general rule
  (136-142), and the Q5 *clients* (256 held connections, a `GET /hello`) by nothing named. `timeout`
  here is GNU coreutils 9.12 (`/opt/homebrew/bin/timeout`), which kills the command's process group
  (`timeout -s KILL 2 sh -c 'sleep 31 & sleep 32'` left no `sleep`), so a forked C child dies with it;
  a child the Heroes runtime spawns has a group of its own (`run.c:1372`) and does not, which the
  `pgrep -f <folder>` rule catches only where its command line holds the folder.
- **Section E's probe trap is a bound too**: `waiters.c` past `ulimit -n 256` waits for ever (exit 137
  under `timeout`), so a seat that runs it unbounded hangs.

## J. Questions the sitting should ask, and routes not listed for Q2 and Q3

**R3's residue on Darwin is C's own, reachable with no Heroes code.** `critic/ctwin/critic-ctwin.c`:
a handler of C's that calls `localeconv` itself while `main` formats integers with `snprintf`, built
`clang -O2 -fomit-frame-pointer`, three storms by `storm.py ... SIGUSR1 4 0.3`: run 1 *"sent 66; exit
-30"* (killed by the storm before it armed, item I), runs 2 and 3 *"exit -9"* after 19,897 and 16,503
signals, and `ips.py ... critic-ctwin` printed `('critic-ctwin', 'SIGKILL', 'lock') 2`, stack
`_os_unfair_lock_recursive_abort < _os_unfair_lock_lock_slow < localeconv_l < handler < localeconv_l
< __vfprintf < _vsnprintf < snprintf < main`. So the SIGKILL in 2522 is libc's reentry, which a
Heroes function in the chain does not cause and a C program dies of alike. What R3 asks of a route on
Darwin is a refusal **before** the Heroes function's own C call, at the thunk's first line, so the
death has words; on Linux arm64 it is R4 of 246 alone (the program exits 0). The brief should say
which of the two a route closes on each platform, since a route can close one and not the other.

**Q2, routes nobody listed** (each a question, unbuilt by this critic):
- **One poller for the process**: a runtime thread polls every waiting socket on one `kqueue` or
  `epoll` (and the stop's pipe) and wakes each waiter by a condition variable, so a waiter holds no
  descriptor of its own, a close is a mark and a signal of the variable, and a deadline (Q5) is a timer
  in that one poller. Go's network poller is this shape. Its cost is a thread and a hand-off per wait.
- **A thread-directed signal as the wake**: the closer `pthread_kill`s the waiting thread with a
  signal of the runtime's, `poll` returns `EINTR`, the waiter asks its socket again: no descriptor at
  all. It meets 2494's world (the runtime's probes already use SIGURG, `sigguard.c:175-186`) and a
  program's C that takes the same signal; on Windows it has no arm.

**Q3, shapes not listed**: `fchdir` on a folder the **parent** opened (`open(dir, O_DIRECTORY |
O_CLOEXEC)` at staging or before `fork`), so a missing or unreadable folder is the parent's error,
told before any child exists, and the relative folder is resolved once, in the parent's folder; the
brief lists `fchdir` only as a name on POSIX's list. And for the four cases alone, not the defect's
general gap: a `heroes` flag that starts the compiler in a folder (git's `-C`), which would serve the
net and no other program (`.claude/rules/cli-surface.md` governs a flag).

**Unrun by this critic, posed**: whether `./heroes-next test tests/harness/main.hero` (the
compiler-engineer's Q3 asks it) passes in a `git archive` copy with no `.git`;
`.claude/rules/platforms.md` § Measuring in a copy says only that `records` cannot run there.

## The numbered list

1. **Panel 236 does not name M-package-manager in its first twelve lines.** `head -12
   docs/panel/236-*.md | grep -o -E 'M-[a-z]+(-[a-z]+)+'` printed nothing (237, 238, 239 print
   `M-package-manager`; 236 names it first at line 75). Repair: *"237 to 239 ... and 236 at its line
   75"*.
2. **`245-*.md:6` does not name the milestone it says was asked once.** `grep -o -E
   'M-[a-z]+(-[a-z]+)+' docs/panel/245-*.md` printed nothing. Panel 242's lines 3-4 do: *"the author's
   panel gate for M-core-packages was asked at 07:35 on 2026-11-10"*. Repair: cite `242-*.md:3-4` (and
   `247-*.md:3-4`).
3. **Darwin exports four more installers than the brief lists.** Every `sig`-prefixed export of
   `libsystem_c.tbd` and `libsystem_platform.tbd` printed `__sigaction_nobind __signal_nobind
   __sigvec_nobind` and `___platform_sigaction`; undeclared in the SDK (`grep -rn` printed nothing);
   run (`critic/darwin-installers/`), each printed *"handler ran 1"* and *"calls that reached the
   executable's own sigaction: 0"*. Repair: Darwin's list is ten names, not six; route I owes all ten
   or says why not.
4. **glibc's prototypes, which the brief calls unread, and two facts its "nine" hides.**
   `/usr/include/signal.h`: `bsd_signal` 104, `__sysv_signal` 77, `sysv_signal` 80, `ssignal` 127,
   `sigset` 367, `signal` 88, `sigaction` 243. `nm -D` (unfiltered) prints `sigvec@GLIBC_2.17` (one
   `@`, a compat symbol) and a fresh link calling it printed *"undefined reference to `sigvec'"*; it
   also prints `__libc_sigaction@@GLIBC_PRIVATE`. Repair: eight linkable handler takers, `sigvec`
   reachable only from an old binary, and the private one named.
5. **`sigignore` and `siginterrupt` change what a definition's table answers without reaching it.**
   `critic/linux/reach.c`: `siginterrupt` reached the executable's `sigaction` 0 times; `sigignore`
   0 times with *"disposition now SIG_IGN 1"*. The other glibc installers, from the program and from a
   shared library, reached it 0 times; `sigaction` 1. Repair: name them in route I's list of what must
   be defined or mirrored.
6. **Windows installers are not only UCRT's.** The runtime itself installs with
   `AddVectoredExceptionHandler` (`os.c:318`, `stack.c:618`, `:1210`), `SetUnhandledExceptionFilter`
   (`os.c:319`) and `SetConsoleCtrlHandler` (`hero_net.c:539`), which the 27-line grep cannot see.
   Repair: Q1's Windows question names these beside UCRT's `signal`.
7. **The sanitizers' symbols, corrected.** Darwin: `nm` printed `t _wrap_signal`, `t _wrap_sigaction`,
   `s __ZL19substitution_signal`, `s __ZL22substitution_sigaction`, all local, and `otool -l` a
   `__DATA,__interpose` section of `0x1300` bytes. Linux: also `T ___interceptor_signal`, `T
   ___interceptor_sigaction`, `U sigaction`. Repair: say all four Darwin symbols are local and the
   sanitizer is a dyld interposer loaded as a dependent dylib.
8. **`--wrap` on each linker, run.** Apple `ld` (`ld-27106.1`): `ld -help` printed *"unknown options:
   -help"*; `man ld` 0 lines with `wrap` in 1,575. `ld64.lld`: 0 in `--help` and `--help-hidden`.
   `lld-link`: `/?` printed *"error: subsystem must be defined"*, 0 `wrap` in `-help` and
   `-help-hidden`. GNU `ld`, `ld.lld` and `ld.lld -m i386pep` print a `--wrap` line. Repair: W exists
   on GNU ld, lld's ELF and MinGW drivers, and not on Darwin's or an MSVC link.
9. **The project's Windows target is MSVC's; the briefs' `zig cc` target is MinGW's.**
   `WINDOWS-MACHINE.md:346,357` *"target `x86_64-pc-windows-msvc"`*, `ci.yml:540` *"Windows carries
   clang with Visual Studio"*. Repair: say a `zig cc -target x86_64-windows-gnu` arm stands for MinGW,
   where W's `--wrap` exists and MSVC's link has none.
10. **W's premise is false: `--wrap` redirects the runtime's own calls.** `critic/linux/run5.sh`:
    *"runtime object's call wrapped: 1"* under `ld.bfd` and `ld.lld`; with the library shared,
    *"static archive's call wrapped: 0"*. Repair: drop *"it leaves the runtime's 27 calls ... alone"*;
    W needs the runtime calling `__real_<name>` or a split link, and never reaches a shared library.
11. **Darwin's flat namespace, run.** `-Wl,-flat_namespace` and `-Wl,-force_flat_namespace` on the
    executable: *"dylib sigaction: handler ran 1, reached the executable's 0"*; the linker printed
    *"-force_flat_namespace is no longer supported, using -flat_namespace instead"*. Repair: record
    both; the seats' question narrows to `__interpose` and a dylib linked flat.
12. **M needs an oracle it does not have.** `man 2 sigaction` lines 71-75 (the mask is *"the union of
    the current signal mask set, the signal to be delivered, and the signal mask associated with the
    handler"*, *"or until a sigprocmask(2) system call is made"*) and 115-117 (`SA_NODEFER`). Repair:
    quote it (it was "unread tonight"), and pose M as a question that needs the thread's own mask,
    which the program's C changes unseen (the runtime alone calls `pthread_sigmask`/`sigprocmask` on 16
    lines).
13. **Routes not listed for Q1** (section D): I with `SA_ONSTACK` forced by the definitions; asynchronous
    signals blocked on every claimed thread so a C handler runs where 2492's guard refuses; a bounded
    scan of the stack for the trampoline's return address; route I shipped as a dylib with an
    `__interpose` section; renaming the archives' references at build (`xcrun --find llvm-objcopy`:
    *"unable to find utility"*). The thunk-section refusal is a restatement of the exact test. Repair:
    add them as questions.
14. **R3's "two cases" are named nowhere in the briefs.** `247-*.md:191-195`: *"Its cases are those
    two"* after `flib-nofp` and `c-fliblocale`; `r2522/` holds `fliblocale` alone. Repair: name the two
    (and their build lines) so both seats run the same.
15. **2522's Darwin death needs no Heroes code.** `critic/ctwin`: 2 of 2 armed runs `exit -9`, `ips.py`
    `('critic-ctwin', 'SIGKILL', 'lock') 2`, stack `localeconv_l < handler < localeconv_l < __vfprintf`.
    Repair: say what a route must close on each platform (a refusal before the Heroes function's C call
    on Darwin, R4 of 246 on Linux).
16. **Q2's base figures, re-run once**: 271,605,305 instructions and 10,467 switches waiting, 21,847,841
    and 226 sleeping (`timeout -s KILL 60 /usr/bin/time -l ./waiters-critic 100 [sleep] 5`). The
    waiters read 0.5% under the brief's range. Repair: none needed beyond writing the range as about
    272 to 275 M.
17. **`waiters.c` hangs past the limit by its own ordering.** Under `ulimit -n 256`: 126 *"waited"*, 127
    and 130 exit 137 under `timeout`; `sample`: 1449 of 1489 in `poll` under `hero_net_accept`. With the
    connect checked first, *"no connect 126, kind 9"* (`HERO_NET_LIMIT`). Repair: hand the seats the
    corrected probe, or warn that `waiters.c:54-56` calls `accept` before checking `peer[i]`.
18. **The probe's descriptor counts are twice a server's.** `waiters.c:42-56` holds `peer[i]` and
    `mine[i]` in one process; the lane's *"409 instead of 207"* counts both ends. Repair: ask for a
    server-shaped count; the arithmetic in section E is an inference.
19. **POSIX's list read**: `chdir` and `fchdir` are on it (231 names extracted from
    `V2_chap02.html`, IEEE Std 1003.1-2024, read 2026-11-14); `getrlimit`, `setrlimit` and `execvp` are
    not, and the child calls all three today (`run.c:1382-1400`, `1460`). The fork page: *"the
    application shall ensure that the child process only executes async-signal-safe operations until
    ... one of the exec functions is successful"*. Repair: quote both, and ask whether the existing
    calls stand, which the brief frames as if `chdir` were the first.
20. **`CreateProcessW` read** (learn.microsoft.com, read 2026-11-14): *"The full path to the current
    directory for the process ... If this parameter is NULL, the new process will have the same
    current drive and directory as the calling process"*; nothing on a relative or missing folder.
    Two more sentences bear on the door: the program is searched in *"The current directory for the
    parent process"*, and an environment block drops the `=C:` drive folders unless passed. Repair:
    quote them; ask the program-path order per platform and whether `hero_run_go`'s block keeps `=X:`.
21. **`served` stops on less than the brief says.** `main.hero:115-117` joins `threads[0]` whatever
    its state, a finished thread leaving the list only there; by reading, one quiet first connection
    and 255 finished ones stop the accept. `hero_os.h` has no join that does not wait (441, 447, 449,
    453). Repair: pose the narrower shape for the seats' bounded probe, and list a non-waiting join as
    a Q5 route (spec § 13 line 377 names `hero_thread_join`: full panel).
22. **The b35 notes search printed more than the brief says**: 49 files, 24 under `b35-ffi/emitcmp`,
    three `b35-publish/driver-added*.txt`, `fix/msg-skips.txt`, none a note of Q4 or Q5. Repair: list
    the kinds whole; the conclusion stands.
23. **The lane: SKILL.md:33-34 sends Q4 to the full panel.** *"A question that looks internal but has
    a sentence in the spec behind it is a full panel"*; spec line 404 names both stop doors. The doors
    beside Q3 to Q5 were ruled in full sittings (242's R6, 246's R4, 247's R5). Q1's routes widen what
    `ffi_defined_elsewhere` refuses. Repair: quote `:33-34`, escalate Q4 now rather than by a seat's
    report, and add *"a widened diagnostic"* to the escape.
24. **The seats' crash reports collide.** Both use `-o fliblocale`; `ips.py` takes every
    `fliblocale-2026-*.ips`; 3 exist already. Repair: a binary name per seat, or filter by pid.
25. **The storm arms by the clock.** The C twin's first run, *"sent 66 SIGUSR1; exit -30"*, no `armed`.
    Repair: count only runs whose output holds `armed`.
26. **`timeout -s KILL 900 docker run` does not stop the container.** After exit 137, `docker ps`
    printed *"critic-248-bound Up 6 seconds"*. Repair: `--name`, `docker ps` and `docker stop` after a
    timeout, and every probe inside under its own `timeout -s KILL`.
27. **Bounds left to the general rule.** The Q5 clients are bounded by nothing named; the TSan probe and
    the fork-and-`chdir` stress only by 00-shared.md:136-142. Repair: name the time and bytes for each,
    as the Q4 server's are.
28. **Routes not listed for Q2 and Q3** (section J): one poller thread for the process; a
    thread-directed signal as the wake; `fchdir` on a folder the parent opened; a `heroes -C` flag for
    the four cases alone. Repair: add them as questions.
29. **Unrun, posed**: whether the net's own tests pass in a `git archive` copy with no `.git`.

Everything else the briefs state that this critic checked held: the base and its tree, the sittings'
lines, 2522's labels and text, the storm counts and pids, the tools' hashes, Darwin's header lines,
glibc's fifteen names, the runtime's 27 lines, every line number cited in `sigguard.c`, `sigdoor.c`,
`alloc.c`, `callback_thunk.hero`, `run.c`, `hero_os.h`, `hero_net.h` and `hero_net.c`, the 2494 and
layer case names, the line counts, the 22 search terms, the issue comments posted twice with the
same words, the 2344 and 2479 cases and their skips, `c-boundary.md`'s 186-220, 345-347 and 358-363,
panel 242's R6 lines, panel 246's R4 lines, the descriptor limits, clang 21.0.0, zig 0.15.2, the
ignore rule, the Linux image's five facts.

## Second pass, the three added briefs

Started 2026-11-14 10:03 CET (`date`), over `spec-warden.md` (103 lines), `historian.md` (94) and
`llm-ergonomist.md` (254), read whole, and the repaired `00-shared.md` (862). Same rules; my files are
under `.claude/worktrees/p248-scratch/critic/` (`base/`, `runner-sim/`, `web/go/`, `web/posix/`,
`linux/run6.sh`). The base compiler for `measure` was built there in two stages at load average 236
(`uptime`), so nothing in this pass is a duration.

### The spec-warden's figures

- **The counts, re-run.** `git archive df7612ad3` into `critic/base`, seed and stage 2 exit 0,
  `heroes-next --version` *"sources 4c9a6a9800095eef"*, `shasum -a 256 spec/heroes-spec.md`
  `163219f2fa69fe82`, `wc -l -c` 492 and 29,187. `./heroes-next measure spec/heroes-spec.md` (no
  `--refresh`), exit 0: `claude-legacy 8470`, `cl100k_base 8621`, `maximum 8621`, `spread 151`, `real
  11285 claude-opus-5, 2026-11-13 — the binding number`, *"Headroom: 5099 against the 16384 ceiling
  ... what is measured against the ceiling is 11345"*: byte for byte `briefs/base/measure-spec.txt`.
  **How each is measured**: the vendored rows are counted now by the compiler's own tables; **`real`
  is not counted now**. It is a pin, `SPEC_REAL_TOKENS 11285`, `SPEC_REAL_MODEL "claude-opus-5"`,
  `SPEC_REAL_TAKEN "2026-11-13"` (`selfhost/measure/pinned.hero:54-61`), mirrored at
  `tests/harness/suite_spec.hero:176-177`, shown because the spec's digest still matches (no
  `STALE`). The brief says *"the instrument's pin"*: right. `CEILING 16384` at `suite_spec.hero:162-163`,
  `DELTA_GATE 50` at `:217-218`: right. `11285 - 11108 = 177`, 11108 being
  `247-briefs/spec-warden.md:25`'s pin: right.
- **The anchors**, each counted by occurrence (`grep -o -F '<anchor>' spec/heroes-spec.md | wc -l`):
  *"Ctrl-C and SIGTERM only make `hero_net_stop_asked() -> i64` answer 1"* 1 (line 404); *"gives the count
  since its last call"* 1 (404); *"and `hero_thread_join(handle: i64) -> i64` waits for its answer."* 1
  (377); line 404 ends *"gives the count since its last call"* with no full stop (`tail -c`), line 405
  blank. Right.
- **One grep the brief reports is false as written.** *"`grep -n -i -E 'library the program
  names|another library|two libraries|supplied' spec/heroes-spec.md` printed nothing of the kind (its one
  hit, line 5, is the grammar's `=` defines)"*: `grep -c` of that pattern printed **0**, exit 1. Line 5
  holds *"defines"*, which the pattern does not contain. The conclusion holds (no spec sentence names
  the member); the parenthesis is a hit of another pattern.
- **Three of the five drafts are spelled with doors no seat was handed.** The brief: *"each spelled
  with the door the seats were handed (`compiler-engineer.md`, `llm-ergonomist.md`)"*. `grep -c` over
  the five other briefs: `hero_net_ask_stop` in `compiler-engineer.md` (149) and 00-shared.md, so q4m
  and q4a hold; but **`hero_net_set_deadline` (q5d), `hero_thread_ready` (q5j) and `hero_run_dir`
  (q3a) occur in no brief but the spec-warden's** (compiler-engineer 0, ffi-pragmatist 0, 00-shared 0,
  llm-ergonomist 0); the compiler-engineer's Q3 asks *"the door ... beside `hero_run_env`"* with no
  name, and its Q5 hands the card's `net.set_deadline`. **And q5j is not the join 00-shared.md
  describes**: 00-shared.md:811-812 has *"`hero_thread_join` answering "not yet" for a thread still
  running"*, which would make line 377's *"waits for its answer"* false; q5j adds a door
  `hero_thread_ready` beside an unchanged join. The spec-warden prices one shape and the other briefs
  describe the other.
- **The other citations hold**: `grep -n` of design.md printed `### 1.0` 112, `### 1.2` 190, `### 1.6`
  253, `payment` at 282, 301, 324, 328, 330, 4053; `selfhost/cli/refresh.hero:37` is
  `"https://api.anthropic.com/v1/messages/count_tokens"`; `SKILL.md:291-293` names *"a `claude -p`
  session, an API call, a cloud run"* among paid runs; `247-*.md:231-232` holds `163219f2fa69fe82`.
- **The real delta stays unmeasured**, as the brief says; `.claude/agents/spec-warden.md:3` says
  *"Input MUST include the real token count before/after, never an estimate"* and its output allows
  `provisional`, which the brief asks. The real counts of q4m, q4a, q5d, q5j and q3a are one
  `--refresh` each, an API call, the author's to allow.

### The blind seat's folders, read and never run

- **Twelve folders, three files each, nothing else**: `ls s` printed `30c5af 4a8095 5154bf 8c0283 8ca14c
  a323c3 bd049d c532f0 ca0425 d619ea dbaad4 f32865`; each `ls -A` printed `brief.md heroes-spec.md
  task.md`; `find s -type f | wc -l` 36; no `run.json`.
- **Every spec copy is the base's, byte for byte**: `cmp` of each `heroes-spec.md` with `git show
  df7612ad3:spec/heroes-spec.md` printed *equal* twelve times, `163219f2fa69fe82` each. There is no spec
  variant to look for: the brief's arm is the card, and all twelve hold one spec. Every `brief.md` is
  `1d0b6a53cddcb0a0`, and `cmp` against `p247-scratch/s/027658/brief.md` printed equal. Every
  `task.md` hash is the brief's table's, folder by folder, and `s-map.txt` and `s-order.txt` hold the
  table's rows and order (rounds forward, reversed, forward).
- **The tasks are what the brief says they are**: each arm's `task.md`, rebuilt from `src/` by
  `sessions.py`'s join (task, `\n## The package's documentation\n\n`, the arm's card, `\n`, `http.md`),
  `cmp`-equal to `s/d619ea`, `s/dbaad4`, `s/8ca14c`, `s/bd049d`; `src/net-x.md` and `src/http.md`
  `cmp`-equal to core 0.3.0's `net.md` and `net/http.md`, whose hashes `packages/core/0.3.0/list` gives
  (`f5aa1e67...`, `37f68b5d...`); `diff src/net-x.md src/net-q.md` and `... net-d.md` print exactly the
  brief's lines. Tool hashes: `cards.py` `0419d34aba0a5e2b`, `sessions.py` `cae1b8bcdcdb9510`,
  `blind-one.sh` `1d53cd5829f0f969`, `blind-run.sh` `19dc1235a03fb6ad`, `task-t4.md`
  `2dea9e954ed2892b`, `task-t5.md` `640cfcc85ca0b2a2`; both scripts pass `bash -n`. Right.
- **No word of the project's names in `brief.md` or `task.md`**: `grep -i -o -w` over all 24 files for
  *panel, sitting, seat, arm, coordinator, ergonomist, llm, blind, candidate, face, grade, rubric,
  critic, warden, historian, defect, decision, milestone, batch, lane, worktree, scratch, p248, p247,
  248, 247, 246, t4, t5, q4m, q4a, q5d, `M-...`, claude, anthropic, opus*, the author's and the
  organisation's names, `CLAUDE` printed nothing. What they do hold is the language's own names
  (`Heroes`, `heroes`, `.hero`, `hero_net` in the card) and `127.0.0.1`. **But the session sees its
  folder's path**, `.../heroes-lang/.claude/worktrees/p248-scratch/s/<hex>`, which names the sitting:
  of panel 247's 40 context answers (`p247-scratch/context.tsv`), 40 read `no`, and `597f7e` *"names
  the folder's path (a `heroes-lang` worktree, `.claude/worktrees/p247-scratch`)"*. It names no arm.
- **The arm d card attributes to Go what Go's page denies.** Its line ends *"Go's `Conn.SetDeadline`,
  counted from each call that waits"*. Go's page (`curl -o critic/web/go/net.html https://pkg.go.dev/net`,
  read 2026-11-14): *"A deadline is an absolute time after which I/O operations fail instead of
  blocking. The deadline applies to all future and pending I/O, not just the immediately following call
  to Read or Write."* 00-shared.md:809 describes Go's the same way (*"an absolute time the socket's waits
  end at"*). A reader who knows Go is told one mechanism by its name and another by its words, inside
  the session's input.
- **Above the folders**: walking up from `s/d619ea` to `/`, the one `CLAUDE.md` is the repository's
  (`/Users/joseph/Temp/heroes/heroes-lang/CLAUDE.md`); no `CLAUDE.local.md`; no `~/.claude/CLAUDE.md`
  (`ls`: no such file). The repository's `.claude/settings.json` and `.claude/hooks` also stand above
  them; whether `--bare --safe-mode` keeps settings and hooks from a session is unrun (it would be a
  session), and panel 247's 40 answers read `no`. `git check-ignore -v .claude/worktrees/p248-scratch/s`:
  `.gitignore:34:.claude/worktrees/`. `claude --version`: `2.1.311 (Claude Code)`. Right.

### The runner, and its ceiling

- **The arithmetic**: `python3 -I -c` printed `4.800000000000001` (12 × 0.40), `4.984` (3.00 + 4 ×
  0.496), `2.6268` (12 × 0.2189). Panel 247's 96 `run.json` (`python3 -I`): n 96, unreadable 0, sum
  21.0131, min 0.1465, max 0.4960, mean 0.2189, one above 0.40; `247-*.md:423-424` says the CLI passed
  its cap by one request (t4-m2-3, 0.4960). Right. The 4.984 is a bound only while no session passes
  0.496, the largest seen, which the brief says.
- **The runner stops itself after its first session ends, because of a line 247's runner did not
  have.** `blind-one.sh:12-21` sums every `run.json` under `s/` and **stops on any it cannot read**
  (line 21, *"STOP before $f: $bad unreadable run.json"*); line 32's `> run.json` creates the file
  empty the moment a session starts, so every session in flight is an unreadable `run.json`. Run dry
  (`critic/runner-sim/`: the script with `S` pointed at an empty copy of the twelve folders and
  `claude` replaced by a local stub that sleeps and prints a cost, `grep` showing no other `claude` on
  any executed line, the key empty), `xargs -n 1 -P 4` as `blind-run.sh:12` does: with equal 3 s
  stubs, twelve `END`s (each batch of four started together); **with stubs of 2 to 8 s, four
  `START`s, then at the first `END` eight `STOP before <hex>: 3 unreadable run.json`, four `run.json`
  written** (`sim2.log`). Panel 247's runner (`p247-scratch/tools/blind-one.sh:19-21`) has no such
  line and printed *"unreadable run.json: 3"* beside each `START` and went on
  (`p247-scratch/blind-runner.log`). So *"runs the rest four at a time"* is false: one invocation runs
  the first four free slots and drops the rest, and the order's *"a stop leaves every cell
  represented"* holds only across reruns. The cost bound is not broken (it errs low).

### The historian's brief

- **The earlier historians' lines hold**: `246-reports/historian.md:72` and `472-476` (*"first closing
  all open listeners, then closing all idle connections"*), `485-486` (`serve_forever`), `340` (*"B2,
  the stop: pages read 2026-11-12"*); `247-reports/historian.md:15` (the correction), `53-55`
  (ThreadSanitizer *"interposing the registration calls themselves"*), Go issue 35814 at 273,
  `unw_is_signal_frame` at 189-190, and `## One thing written outside the repository's root, reported`
  at 510. `SKILL.md:281` gives the historian `Write` since 2026-09-29 (its line 252 says the opposite,
  as the history 281 supersedes); `.claude/agents/historian.md:4` `tools: Read, Write, WebSearch,
  WebFetch`, no Bash, and the brief asks it to build nothing. The 90 and 33 are
  `.claude/rules/records.md`'s.
- **The list is declared unverified, and one line in it is wrong on its face**: *"POSIX's
  `pthread_tryjoin_np` on glibc"*. `curl -w '%{http_code}'
  https://pubs.opengroup.org/onlinepubs/9799919799/functions/pthread_tryjoin_np.html` printed **404**
  (`pthread_join.html` 200), and glibc declares it inside `#ifdef __USE_GNU` (`/usr/include/pthread.h:221-224`
  in the image, `critic/linux/linux6.log`): a GNU extension, not POSIX's.
- **A fact the historian may cite rather than search**: POSIX.1-2024's
  `posix_spawn_file_actions_addchdir` exists (`.../functions/posix_spawn_file_actions_addchdir.html`
  HTTP 200, *"Issue 8"*); Darwin's SDK declares it without `_np` (`spawn.h:72`, `addfchdir` 75) and
  marks the `_np` form `__API_DEPRECATED("posix_spawn_file_actions_addchdir(3) has replaced
  posix_spawn_file_actions_addchdir_np(3)"` (184-185); glibc 2.41 declares only the `_np` forms (the
  image's `spawn.h`, *"no non-_np addchdir"*).
- The pointers into 00-shared.md hold: route (e) is *"one poller for the process"* (617); the
  `CreateProcessW` page is quoted (693); the sanitizer's `__interpose` section (430); and
  `-force_flat_namespace` refused by `ld-27106.1` is this critic's run.

### Paid and forbidden runs

No brief starts a paid run: the spec-warden's forbids `--refresh` in words; the historian's says *"run
no paid call"*; the llm-ergonomist's is a registration for the coordinator, *"unrun until the
author's yes"*, and no seat is launched on it. No brief asks a seat for a run its rules forbid: the
spec-warden's servers send SIGTERM only to their own process and are bounded by 00-shared.md's table
(196-216), every binary `sw-`; the historian builds and runs nothing.

### Items 3, 10, 15, 17 and 23 in the repaired 00-shared.md

Read whole at 369-385 (item 3), 466-481 (10), 308-318 (15), 559-567 (17) and 70-100 (23): each says
what this critic measured, with the coordinator's own re-runs added (15: *"3 of 3 armed, 3 of 3 exit
-9"*; 17: *"09:45:08 to 09:45:38"*). One word to correct in item 3's last sentence: *"a library calling
any of the four passes a runtime definition of the declared five"* should read *"of the declared five
and `__sigaction`"*. Item 23's *"panel 247 ... all five"*: `ls docs/panel/247-reports/` holds
`compiler-engineer.md`, `ffi-pragmatist.md`, `historian.md`, `llm-ergonomist.md`, `spec-warden.md`:
right.

### The second pass's numbered list

1. **The blind seat's runner stops after its first session ends.** Command: `critic/runner-sim/`,
   `blind-one.sh` with `claude` replaced by a local stub, `xargs -n 1 -P 4`, stubs of 2 to 8 s.
   Printed: four `START`, then eight *"STOP before <hex>: 3 unreadable run.json"*, four `run.json`.
   Panel 247's runner, without line 21, logged *"unreadable run.json: 3"* and went on. Repair: count
   only ended sessions (write `run.json.part` and rename, or pass over an empty file), or drop line 21
   and keep 247's; then say *"four at a time"* again.
2. **Arm d's card names Go for a mechanism Go's page denies.** Command: `curl https://pkg.go.dev/net`.
   Printed: *"A deadline is an absolute time ... applies to all future and pending I/O"*. The card:
   *"Go's `Conn.SetDeadline`, counted from each call that waits"*. Repair: drop Go's name from the line,
   or describe Go's shape; `cards.py` re-run and `s/` re-made before any session, as the brief provides.
3. **Three drafts' doors were handed to no seat.** Command: `grep -c` of `hero_net_set_deadline`,
   `hero_thread_ready`, `hero_run_dir` over the five other briefs. Printed: 0 everywhere. Repair: say
   q5d, q5j and q3a are spelled by the spec-warden's brief alone; only q4m and q4a carry a handed door.
4. **q5j is another join than 00-shared.md's.** Command: `sed -n '808,815p' 00-shared.md`. Printed:
   *"`hero_thread_join` answering "not yet" for a thread still running"*. q5j adds `hero_thread_ready`
   beside an unchanged join. Repair: price both shapes, the one that falsifies line 377's *"waits for
   its answer"* and the one that adds to it.
5. **A spec grep the spec-warden's brief reports did not print a hit.** Command: `grep -c -i -E
   'library the program names|another library|two libraries|supplied' spec/heroes-spec.md`. Printed:
   0, exit 1. Repair: *"printed nothing"*, dropping *"its one hit, line 5"*.
6. **The counts hold, and `real` is a pin.** Command: `./heroes-next measure spec/heroes-spec.md` in
   `critic/base` (stage 2, `sources 4c9a6a9800095eef`). Printed: 8470, 8621, 8621, 151, real 11285,
   headroom 5099, 11345 against 16384. `real` is `SPEC_REAL_TOKENS 11285`, taken 2026-11-13
   (`selfhost/measure/pinned.hero:54-61`), not a count of today. Repair: none needed; the brief already
   calls it the pin.
7. **The anchors occur once each.** Command: `grep -o -F '<anchor>' spec/heroes-spec.md | wc -l`. Printed:
   1, 1, 1 (lines 404, 404, 377); line 404 has no full stop. Repair: none.
8. **The twelve folders are as the brief says.** Commands: `ls -A`, `find s -type f | wc -l`, `cmp`
   against `git show df7612ad3:spec/heroes-spec.md`, `shasum -a 256`, the tasks rebuilt from `src/`.
   Printed: three files each, 36 in all, no `run.json`, twelve spec copies equal, every hash the table's,
   four rebuilt tasks equal. There is no spec variant: the arm is the card. Repair: none.
9. **No project word in `brief.md` or `task.md`; the folder's path still names the sitting.** Command:
   `grep -i -o -w` over the 24 files, and `p247-scratch/context.tsv`. Printed: nothing; and `597f7e`,
   *"names the folder's path (a `heroes-lang` worktree, `.claude/worktrees/p247-scratch`)"*. Repair:
   say that the path is seen and names `p248-scratch`, never the arm.
10. **The ceiling arithmetic holds as an assumption, not a bound.** Command: `python3 -I -c`. Printed:
    4.800000000000001, 4.984, 2.6268; 247's 96: sum 21.0131, max 0.4960, one above 0.40. Repair: none
    beyond the brief's own *"if no session costs more than 0.496"*; with item 1 repaired, the four in
    flight it assumes come back.
11. **The historian's list attributes `pthread_tryjoin_np` to POSIX.** Command: `curl -w
    '%{http_code}' .../functions/pthread_tryjoin_np.html`; the image's `pthread.h`. Printed: 404;
    `#ifdef __USE_GNU` at 221, the declaration at 224. Repair: *"glibc's `pthread_tryjoin_np`, a GNU
    extension"*.
12. **`posix_spawn_file_actions_addchdir` is POSIX.1-2024's, Darwin's without `_np`, glibc's with it.**
    Commands: `curl` of its Open Group page; `grep` of both `spawn.h`. Printed: HTTP 200, *"Issue 8"*;
    Darwin `spawn.h:72` and the `_np` form deprecated at 184-185; glibc *"no non-_np addchdir"*. Repair:
    give the historian and the ffi-pragmatist these as read.
13. **No brief starts a paid run or asks a forbidden one.** Command: the three briefs read whole;
    `grep -n -i -E 'claude -p|--refresh|paid|api call'`. Printed: prohibitions and the registration's own
    *"unrun until the author's yes"*. Repair: none. The spec-warden's definition wants real
    before/after counts; the brief's `provisional` is what the definition allows, and a `--refresh` of
    the five drafts is the author's call.
14. **Items 3, 10, 15, 17 and 23 read correctly in 00-shared.md.** Command: `sed -n` of 369-385,
    466-481, 308-318, 559-567, 70-100. Printed: each as measured, with the coordinator's re-runs. Repair:
    item 3's *"a runtime definition of the declared five"* should say *"the declared five and
    `__sigaction`"*.
15. **The settings and hooks above the session folders.** Command: a walk from `s/d619ea` to `/` for
    `CLAUDE.md`; `ls -d .claude/settings.json .claude/hooks`. Printed: the repository's `CLAUDE.md` only;
    both settings and hooks present at the root. Whether `--bare --safe-mode` keeps them from a session
    is unrun (a session); panel 247's 40 context answers read `no`. Repair: name the settings and hooks
    beside `CLAUDE.md` in what each `context` answer is read for.

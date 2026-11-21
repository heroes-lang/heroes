# Panel 251, ffi-pragmatist

Read `00-shared.md` first, whole: the frozen commit (`3a5ee269117a31780ef1f96d0305452630ac4d83`, stage 2 `sources
23c7d2a626b29fd5`), what moved since the 2026-11-15 briefs, the three proposals and their conservative
alternatives, each question's record, Rebel's witnesses and the rules. You judge from the C boundary,
with a veto on ABI breakage and on soundness at the boundary: a veto names the program that dies in
silence, is refused while correct, or breaks the boundary under the route vetoed, **compiled and run**.
**Compile, don't opine.**

**Your folder** is `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/rebel-scratch/251-ffi-pragmatist/`,
made by you as `00-shared.md` § Where each seat works shows. Binaries `fp-...`, containers `fp-251-<n>`,
one at a time. Read `.claude/rules/c-boundary.md` and `.claude/rules/platforms.md` in your copy before
your first build. **Your report** is `.claude/worktrees/rebel-round/docs/panel/251-reports/ffi-pragmatist.md`,
written as you go, in `.claude/agents/ffi-pragmatist.md:33-40`'s structure (`verdict`, `section`,
`experiment` (the C you wrote, and whether clang accepted it), `argument` of at most 120 words,
`prediction` about a named binding, `condition`), **one block per question**. No paid run, no
credential, no `heroes test`; Go only with every write moved into your folder (`00-shared.md`); `lldb`
with `HOME` and `TMPDIR` in your folder.

## Question 1: a C handler reaching Heroes, route J on Darwin

By 249's R3 (`docs/panel/249-*.md:253-282`): **you build J into D-rebind on Darwin**, D-rebind's
`--sanitize` loop repaired first; the compiler-engineer builds it into B' on Linux; **one rule on both**,
or each says where and why it differs.

1. **The `--sanitize` loop** (carried, `p251/docs/panel/251-briefs/00-shared.md:357-366`, uncommitted, in
   the worktree `.claude/worktrees/p251`): D-rebind's constructor takes the real `sigaction` by
   `dlsym(RTLD_DEFAULT, "sigaction")` (`ev249/249-ffi-pragmatist/c/rebind/rebind.c:76`), which under ASan
   answers ASan's interceptor, and then rewrites ASan's own image's slot, so every `--sanitize` program
   died before `main`. Build a repair (libSystem's own by a handle, or ASan's image left unrewritten), run
   `ev249/critic/reports/asanprobe/q.c` with and without `-fsanitize=address`, then `rflib` built
   `--sanitize` and `--sanitize -O0`, 3 of 3 each, refused with R1's words and nothing before `main`.
2. **Build J into it, starting from the overlay** `bw/routes/drebind/` (ported to the frozen
   `runtime.c`; `hero_tramp_inside()` asked at `parts/sigguard.c:336`). Its `runtime/parts/rebind.c` is
   85 lines, **your 249 source (`ev249/249-ffi-pragmatist/c/rebind/rebind.c`, 86 lines,
   `0281ee1c166636cb`) less its line 10, `#define _DARWIN_C_SOURCE`** (`diff` prints `10d9`, the critic's
   finding 10, re-run), so every line stands one lower in the overlay: the key and the counter 22-23,
   `drop()` 39, the shared cache's skip 48, the slots from 67, the real `sigaction` by `dlsym` 75 (`sed
   -n`; in the 249 source 23-28, 40-45, 49, 68-70, 76). Darwin's save points are `_setjmp`, `__setjmp`, `_sigsetjmp`, and
   `_getcontext` and `_swapcontext` (the SDK's `libsystem_platform.tbd`, carried); rewrite their slots,
   each recording its buffer and caller's frame and tail-calling libSystem's own; turn the depth from one
   counter into the trampoline frames a thread is inside (in the pthread key's slot); make each rewritten
   jump (`_siglongjmp`, `_longjmp`, `__longjmp`, `_setcontext`, `_swapcontext`) drop only the deeper
   entries. **A jump to a buffer with no record keeps every entry** (the proposal): run it and the other
   way. Say what a `dlsym(RTLD_DEFAULT, "setjmp")` reaches, and how you asked whether libSystem makes a
   `setjmp` inside itself (the shared cache's images are skipped by design, `rebind.c:49`).
3. **The C a binding writes and compiles**: write the C handler that reaches Heroes, as a library author
   would, in three shapes, each compiled and linked against the frozen runtime and bound from a Heroes
   program: a handler that calls the program's function directly (the tree's own
   `tests/golden/run/fixedbugs-2494-a-heroes-function-a-c-handler-calls-is-refused.h`, its handler
   keeping a frame record), the same built `-O2 -fomit-frame-pointer` into a static archive and a
   two-level dylib (247's `flib` and `fliblocale` in `ev249/249-compiler-engineer/amd64/bpship/r3/`), and a
   handler that `sigsetjmp`s and jumps within itself before calling back (`injump`,
   `ev249/249-compiler-engineer/injump/`). Base, D-rebind and D-rebind+J, `-O0` and `-O2`: `injump` and
   its three shapes 20 of 20, `kmiss` 20 of 20, `flib` 20 of 20 static and through the dylib,
   `fliblocale` storms (20 armed static, 5 through the dylib, every crash report's pid joined to its
   storm by `ips.py`, none SIGKILL; `storm.py` and `ips.py` are in
   `ev249/249-compiler-engineer/inputs/ev247/247-compiler-engineer/tools/`), the twelve depth programs exit 0, the goldens (`run fixedbugs-2558`,
   `run recovers-leaves-no-line`, `run fixedbugs-2494`), the `run` form's Guard Malloc at `-O0`
   (`tests/harness/freed_bytes.hero:90`). The writer's run of the tree's case at the frozen commit:
   `installed: 0` then R1's refusal, exit 134, 5 of 5 at each level (`bw/t3/`).
4. **The runtime's own installs under the rewrite**: three new installs (`platform/hero_term.c:258`,
   `:278`; `platform/hero_proc.c:1630`) and three reads (`hero_term.c:247`, `:272`; `hero_proc.c:1628`),
   the stack guard's install (`stack.c:1220`) and the lease handler's prior call (`os.c:392`): what each
   does under D-rebind+J. **And the two forked children**: `run.c:1805`'s `sigaction` between `fork` and
   `exec`, and `hero_proc_child` (`hero_proc.c:1603`), a loop over every signal reading at 1628 and
   resetting at 1630 between `fork()` (1671) and `execve` (1633), taken only where
   `hero_proc_spawn_enters()` (1438) answers false and a folder is named (1777-1779), Darwin before 10.15
   among them by the critic's reading, so not this Mac's 27.2 by itself: **reach it in code** (a build
   with `hero_proc_spawn_enters` forced false, a second thread inside the rewritten `_sigaction` or a
   lock it takes at the fork) and say whether the rewritten slot stays async-signal-safe in the child.
5. **The census libraries under D-rebind+J**, as 249 owed (libuv, ncurses, GLib, OpenSSL's prompt,
   sqlite as the control, the Go archive): read each version with `readlink -f /opt/homebrew/opt/<name>`
   today and run your 249 snapshot and tracer (`ev249/249-ffi-pragmatist/c/census/`). Beside other
   interposers: ASan repaired, Guard Malloc, TSan in a C probe, a `DYLD_INSERT_LIBRARIES` tool, a second
   rebinder.
6. **The forward compatibility of the rewrite**, now that the historian sits: say what this Mac's SDK
   settles (`mach-o/loader.h`, `mach-o/dyld.h`, `man dyld`) and hand the rest to the historian's reading,
   by name.

## Question 2: what C does at the end of `main` with a thread running

The doors (`runtime/hero_os.h`, unchanged): `hero_thread_spawn` `:467`, `hero_thread_spawn_sized`
`:473`, `hero_thread_join` `:475`, `hero_thread_spawn_str` `:490`, `hero_thread_join_str` `:491`; the
handle an index into `spawn.c`'s table, *"never a `pthread_t`"* (`:450-452`); `pthread_create` and
`pthread_join` at `spawn.c:566` and `:645`, `CreateThread` and `WaitForSingleObject` with `CloseHandle`
at `:526-528` and `:642-643`. **The runtime already runs detached threads of its own** (sitting 252's
critic, widened at the frozen commit, `00-shared.md` § What moved): `hero_net.c:1267-1269`,
`run.c:1469-1470`, `hero_proc.c:684-694`, and dispatch queues in `hero_watch.c:1736` and
`hero_tls.c:3812`, `:5082`.

1. **In C alone** (`fp-*.c`, clang on this Mac and in Linux arm64): a `pthread` worker running when
   `main` returns and when it calls `exit`; a worker inside `malloc`/`free`, inside `printf`, holding a
   mutex, and inside a dispatch queue's block on Darwin, each 200 runs under `timeout -s KILL 60`, output
   through `head -c 65536`: every status other than the expected one, every partial line.
2. **Through the runtime**, under base, R and E as the compiler-engineer builds them (or your own build
   of each in your folder): the writer's `bw/repro/` programs and a worker printing in a loop, one
   allocating in a loop, one inside a platform-layer call (`hero_net_*` or a `hero_tls` read), and one
   with an `atexit` callback bound as spec § 13 shows (`atexit(f: (function() -> ())) -> i32`), each
   while `main` returns, 200 runs: any status but 0 and 134 is a finding under design.md §1.12.
3. **Linux arm64 under `--sanitize`**: what LeakSanitizer and ASan say of the reproducer, whether a
   running thread's blocks are read as reachable; and `leaks --atExit -- <binary>` on this Mac (it needs
   `DevToolsSecurity`; say if it waited).
4. **Q2's C**: the door R needs (a count or a list of unjoined slots read by the gate under the table's
   lock) and E's, **compiled against the frozen `hero_os.h`** on this Mac and in Linux arm64; whether any
   binding a program already writes moves (`examples/threads/main.hero:34-44` declares three doors;
   Rebel's `packages/rebel/0.1.0/files/threads/hero_os.hero`).
5. **A worker that never ends**, under R and E: blocked in a `read` nobody writes, in an `accept`, in
   `receive` of the proposal's mailbox (written as the C the runtime would run): what R's abort and E's
   ending do to it, bounded by `timeout -s KILL 60`.

## Question 3: any value across, at the boundary

The proposal (`00-shared.md` § The three proposals, repaired; `bw/arms2/proposal-b.diff`,
`proposal-m.diff`) makes `spawn`, `spawn_sized` and the mailbox the language's, every value copied whole,
`ptr` and `cstr` never crossing, a handle crossing only by moving its life, and **a mailbox itself a
handle with a life** (`mailbox()` begins it, a close ends it), classed by an amended § 3 with `ptr` and
`cstr` as a copied address whose copies reach one queue. Whether a program that binds the old
`hero_thread_*` doors is then refused or left unnamed is a question (yours for the binding's side). Your
questions are the boundary's:

1. **A handle that moves into a thread**: write the C of a handle (a SQLite connection, a `tls` stream of
   Rebel's, a `FILE *`) opened on one thread and used and closed on another, for three C libraries a
   binding plausibly links (SQLite in its default threading mode, libcurl, OpenSSL), compiled and run 20
   times each on this Mac and in Linux arm64: which library's documentation or run forbids a handle
   crossing threads, and so whether *"a handle crosses only in `spawn` or `send`"* needs a mark the
   binding's author writes (a change a binding's author must learn: say so in those words).
2. **A mailbox in C, as the handle the proposal makes it**: the runtime's queue (a mutex, a condition
   variable, a bounded wait, woken by the platform layer's stop, `hero_net_ask_stop`, spec § 13 line 404),
   its life begun and ended once while copies of its address sit on several threads: what a send through
   a copy after the close does, and what a close while another thread waits in a receive does. Compiled
   on this Mac, in Linux arm64, and for Windows by `zig cc` or clang's `--target` if this Mac has it (owed
   otherwise); 32 senders and one receiver, 20 runs, plain and `--sanitize`.
3. **`ptr` and `cstr`**: a program that hands a `ptr` into a thread today through the `i64` door
   (`ptr` to `i64` is refused by the checker? run it): what the proposal's refusal of them costs a
   binding that does so now, by a search of the tree's `.hero` files.
4. **C's own `send` and `close`**: under the built-ins spelling B, `send`, `close`, `wait` and `receive`
   are taken everywhere, so a binding of C's `send(2)`, `close(2)` or `wait(2)` under its own name is
   refused (`error[builtin_name_taken]` is what a built-in's name gets, `extern` members included, the
   critic's run, carried); the tree holds 30 `extern` members named `close` (`grep -rn -E '^ +function
   close\('`, `00-shared.md`). Say what each spelling costs a binding's author, in those words.

## What your verdict owes

Per question: the C on each platform, compiled and run; each case's outcome per level with its run
count; every crash report its storms left, armed runs alone counted; what each route closes and its
residue, in words the author can read beside route K (249's R4, `docs/panel/249-*.md:284-319`); whether
it needs a spec sentence, a new or widened diagnostic, new refusal words or a change a binding's author
must learn, in those words; the Windows row named owed; every premise of these briefs you found false;
and a veto, if you cast one, with its program.

**Added 2026-11-21.** The folders under `.claude/worktrees/` this file names were never part of
the repository, git ignoring that directory, and were removed that day by the author's
instruction, meant as: *delete every worktree that is not active; where one is cited, find a
solution*. What a seat or the coordinator kept under `rebel-scratch/` for this sitting is read
from `docs/panel/251-reports/evidence.tar.xz`, its paths relative to `rebel-scratch/`; `evidence.md`
beside it says what was kept and what was left out.

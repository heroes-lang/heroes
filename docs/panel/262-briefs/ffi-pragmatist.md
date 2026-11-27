# Panel 262, the ffi-pragmatist's brief

Read `00-shared.md` first, whole (its head: the repair and the move; § 2 the questions, § 3.3 the property the runtime
tests, § 3.4 what runs and what is refused, § 3.5 the guard's defects and the lane, § 4 the candidates, § 5 the shapes
beside), then the critic's first reading (`docs/panel/262-reports/critic-briefs.md`, C1 to C28), on which these briefs
were repaired before any seat starts. **Your agent file is the base's** (`.claude/agents/ffi-pragmatist.md` of your
copy); design.md §1.11, §1.12 and §4.19 are reached by `grep -n`. Your veto on ABI breakage stands; so does design.md
§1.12's *"any C library must be bindable"*, which panel 259 read as your veto's ground.

**Your copy**: `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/262-ffi-pragmatist`, a detached worktree at the
base **`87003cb42`**; build its compiler from the seed and stage 2 there (`00-shared.md` § 6), **`sources
6ce3f9e116d22e3a`**; **`main`'s runtime as a second column** where it moved after the base (§ 6). **No paid run, no
credential printed, `gh` reads only, no download (use what is on this disk and in the image; no `apt-get` inside a
container, which is a download), nothing outside the repository's root (`TMPDIR` inside your copy), no `pkill`, no
commit; one container at a time, `--rm`, named `p262-ffi-pragmatist`, no `docker pull`.** Report as you go to
`docs/panel/262-reports/ffi-pragmatist.md` in your copy, programs under `docs/panel/262-reports/programs/ffi-pragmatist/`.
**You have no web**: the precedents you build from (the signal stack trick, libcoro, libcurl's resolver timeout) are
recollections the historian sources at the same time; build every variant the recollection leaves open, and name which
one a sourced page describes once the historian's report exists (the critic's C20).

## The question you answer

**What a binding author's C does, and what the program is told, under each candidate of `00-shared.md` § 4**: for
each real library below, the callbacks it calls and the switches it makes, built and run at the base (this Mac and the
Linux arm64 image, plain and `--sanitize`, three runs each); then, **for each text the warden writes**
(`docs/panel/262-reports/spec-w*.txt`) and for the sentence as it stands, **whether a binding author reading only that
text would predict what ran**: promised to run, may abort, or silent; and where the text is silent or wrong, the
program that shows it. A text a real library's correct program falsifies is your veto's ground.

## The C (what is on this disk and in the image, read by the clerk, the critic and the repair; none downloaded)

**What exists where** (this Mac by `ls` and `nm -u`, the repair, 16:10; the Linux arm64 image `heroes-linux-arm64:latest`,
`338a1044ac90`, by `find /usr/include /usr/local/include`, `find / -xdev -name '<lib>*.so*'` and `dpkg -l`, the
critic's § 4, **re-read by the repair in its own container**, `ground/linux-arm64/L-libs.txt`):

| library | this Mac | Linux arm64 image |
|---|---|---|
| Boost.Context, Coroutine, Fiber | 1.92 (`/opt/homebrew/Cellar/boost/1.92.0_1`, `BOOST_LIB_VERSION "1_92"`; `libboost_context`, `_coroutine`, `_fiber` `.a` and `.dylib` under `/opt/homebrew/lib`) | **absent**: no `boost/version.hpp`, no `libboost_*` |
| OpenSSL `ASYNC` | 3.5.9 (`openssl@3/3.5.9`) and 4.1.0 (`openssl@4/4.1.0`); **`/opt/homebrew/lib/libcrypto.dylib` links to `openssl@4/4.1.0`** (`ls -l`); both libcryptos import `__longjmp __setjmp _getcontext _makecontext _setcontext` (`nm -u`) | `libssl-dev` **3.5.7** (`openssl/async.h`, `libcrypto.so.3`) |
| libuv | 1.53.0 (`/opt/homebrew/Cellar/libuv/1.53.0`) | `libuv1t64` 1.50.0's runtime alone, **no `uv.h`** |
| libcurl | **none** under `/opt/homebrew/Cellar` (`ls -d /opt/homebrew/Cellar/curl*`: no match) | `libcurl4-openssl-dev` 8.14.1 (`curl/curl.h`, `libcurl.so.4`) |
| GNU Pth, libcoro | none found (the critic's) | none: no `pth.h` (`libnpth0t64` is GnuPG's nPth, another library), no libcoro |

**So Boost's and libuv's Linux cells cannot be built under your rules, and OpenSSL's is 3.5.7, not this Mac's 3.5.9 or
4.1.0**: those cells are **unrun**, written so, never inferred from Darwin's (the critic's C21). **libcurl's cells are
Linux's alone.** The hand-written shapes (items 3 to 5, 7 and 8) need no library and run on both.

1. **Boost.Context 1.92** (this Mac only): `fcontext`'s switch is assembly the runtime never sees, its `ucontext`
   backend `swapcontext`. Panel 259 bound it (`bfc_host`, `bfc_heap`, `docs/panel/259-reports/evidence.tar.xz`,
   `programs/ffi-pragmatist/`): run them again at the base with no handler, then with a handler of C's installed in the
   same program that never fires, then with one that fires on another thread.
2. **OpenSSL's `ASYNC`** (this Mac's 3.5.9 and 4.1.0, **say which you link**; the image's 3.5.7): fibers switched by
   `_longjmp` over `makecontext` stacks on this Mac, by `swapcontext` on Debian x86-64 (panel 259's finding, carried
   from its synthesis, `docs/panel/259-*.md:41-44`). `ASYNC_start_job` with a Heroes callback inside the job, and
   `ASYNC_set_mem_functions` (panel 259's `aj_carved`).
3. **A hand-written `swapcontext` scheduler** and **a `longjmp` fiber** (two stacks the program made, jumped between by
   `setjmp` and `longjmp`), with no handler; and beside them panel 257's `cs2`, **a handler's** jump between two
   stacks the program made, refused on Linux by mapping order then (`docs/panel/257-*.md:67-69`) and run since defect
   2707 (`fixedbugs-2707-*`; the blind key's `h10`, `00-shared.md` § 3.4).
4. **The signal stack trick** (`00-shared.md` § 2 question 8: a coroutine library that creates each stack by raising a
   signal on an alternate stack, saving a buffer inside the handler and leaving it, then jumping back into that frame
   to start the coroutine; GNU Pth's design as recalled, **unverified**: the historian sources it; libcoro's
   `CORO_SJLJ`, Marc Lehmann's, used by Perl's Coro, **as the critic recalls it, unverified**, a second library on the
   trick). Write it by hand **in four variants** (the critic's C20), because the runtime's answer turns on the two
   details the recollection leaves open: **the handler returns after its save** (seen to end, an entry popped) **or
   leaves by a jump**; **the library restores the previous alternate stack** after the creation **or leaves its own**.
   Rule 4 reads *"the alternate stack registered now"*, rule 6 re-enters on *"a jump that lands on the alternate stack
   from outside it"*, and **every plain-built thread already has the runtime's own alternate stack**
   (`runtime/parts/stack.c:1269-1282` and `:1235`, the critic's C12): report what each variant does to it, and what §
   9's *"running out of stack aborts"* then does (unrun). **Every coroutine of such a library begins inside a handler's
   frames**, so the sentence's fifth exit (*"a jump that lands within it"*) is the library's normal path. What is its
   callback told, at the base and under each text?
5. **A green-thread scheduler preempting by `SIGALRM`** (panel 257's `sched`, carried: *"`sched` is refused on both
   landed builds"*, `docs/panel/257-*.md:134`, and *"a switch of context never switched back is not the class"*,
   `:65-66`): a correct C design whose handler switches threads; under each text, is a reader told it may abort, and
   in which words? **And its jumping twin, the watchdog** (the critic's C25; `00-shared.md` § 5): a handler that
   `siglongjmp`s out of whatever it interrupted meets **panel 258's jump refusal** (`sigguard.c:175-212`) where the
   signal lands inside the runtime's allocator; **libcurl's synchronous resolver timeout** (`alarm` and `siglongjmp`
   around `getaddrinfo`, as the critic recalls it, **unverified**; curl 8.14.1 in the image) is a real library on that
   shape: bind it in the image (`CURLOPT_TIMEOUT` with `CURLOPT_NOSIGNAL` 0, a resolver that cannot answer), a rate over
   many runs, and say what each text tells its author.
6. **libuv** (this Mac's 1.53.0; **no Linux cell**, no `uv.h` there): `uv_queue_work`'s work callback runs on libuv's
   own thread pool, **a thread C made**, which the runtime refuses in the thread's words (`runtime/parts/thread.c:132-143`
   at the base; the blind key's `n4`, `00-shared.md` § 3.4), a sentence no text of the spec holds today; and
   `uv_signal_start`, whose callback runs **from the loop, not from the handler**: does a text tell a binding author that
   one is promised and the other refused?
7. **Copying designs** (greenlet's slices on the thread's own stack, Tencent libco's copied stacks; panel 259's `cpo_*`,
   in its evidence) with no handler: promised to run under each text?
8. **POSIX's own callbacks on threads the C library makes** (the critic's C25): `timer_create` with `SIGEV_THREAD`,
   `mq_notify`, glibc's `getaddrinfo_a` (the image's glibc 2.41): a binding author who never wrote `pthread_create`
   meets the thread's refusal. And **the thread reading** (`00-shared.md` question 1, T): a handler of C's left on one
   thread, the callback on another (the blind key's `h9`, runs): would a binding author reading `now` predict it?

## What you run, and write

- **The falsifiers every run-promise meets** (`00-shared.md` question 3, the critic's C2, C4, C7, C8; the repair's first
  item): `fixedbugs-2705` and the own-stack coroutine (`2702-a-handler-jumps-into-a-coroutine-on-the-own-stack-...`),
  refused in ONSTACK's words on route C; **panel 258's jump refusal** (a handler's `siglongjmp` out of code inside the
  runtime's allocator stops the program at the jump, `sigguard.c:175-212`); **route A**, which is glibc's architectures
  but aarch64 and x86-64 **and Darwin x86-64 by construction** (`runtime/parts/rebind_decode.c:314-315`), where 2706's
  two programs are refused (step 199a's body) and the drafted `d-lane-sig-family-1` refuses `b_0` and `cs_0`, correct
  programs, in UNFOLLOWED's words; **so the `setcontext` split** (`cs_0` runs with the handler on the coroutine's own
  stack, is refused with it on the alternate stack) **is route C's alone**. A binding author on an Intel Mac or on
  glibc's riscv64 stands on route A: no machine of this sitting is one (the simulation under `HERO_JMP_NO_DECODE` in the
  Linux arm64 image is the compiler-engineer's), so every such cell of yours is *unrun*, and a text that promises a
  library's callback a run there is judged by those falsifiers, not by your route-C cells.
- For each library and shape: the program, its header, its output at the base (exit, words, three runs), on both
  platforms where both exist, plain and `--sanitize`; **name every cell where the two platforms, the two builds or the
  base's and `main`'s runtimes differ**, and every cell **unrun** with its reason.
- **A table**: shape · handler of C's (none, installed, fires on this thread, fires on another, the runtime's own) ·
  base's verdict · for the sentence as it stands and for each warden text: promised to run, may abort, silent, or false
  (a promise a run breaks).
- **What a binding author owes under each text**: the C a correct binding must write to keep its callbacks
  running (a stack of `malloc`'s, a handler that returns, `siglongjmp` and not assembly, no `siglongjmp` out of code
  that may be inside the runtime's allocator, no callback on a thread C made), and whether the text says it or only the
  runtime's words do. **The thread**: should a text name a callback on a thread C made, and what does it cost a binding
  of libuv's pool or of `SIGEV_THREAD` to be told only at its abort?
- **The defects of `00-shared.md` § 3.5** as a binding author meets them: which of your shapes reaches 2852, 2853 (repaired after the base by
  step 199e, `00-shared.md`'s head: your second column), or the drafted `d-lane-sig-family-2` (a yield saving into another context than the one switched to; 2850, 2851 and
  2855 are repaired at the base, by steps 199b, 199c and 199d); **ask the lane's state** (`git log --oneline 87003cb42..main -- runtime`) before
  writing a cell that rests on one, and build `main`'s runtime in your copy as the second column, never in its
  worktree.

## What you judge

`verdict` (approve, object, veto) per text and per candidate of `00-shared.md` § 4, `section` (design.md §1.12, §4.19),
`cost` (none at the ABI unless a candidate adds a `hero_os.h` entry: a promise for ever, decision 2228, carried from
panel 259's verdict table), `prediction` (one falsifiable sentence, with the landing that checks it) and `condition`
(what would change your mind). **A text a correct program of a real library falsifies on a platform it claims is a
veto**; name the program.

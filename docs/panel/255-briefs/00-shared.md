# Panel 255, shared brief: which route closes defect 2679

Written 2026-11-22 by the sitting's coordinator. Every number, path and count below was produced by a
command run while this brief was written, and the command is named beside it; a fact this brief carries
without running it says **carried** and from where. Read this file whole, then your own brief.

**Repaired after the completeness critic's first reading** (`docs/panel/255-reports/critic-briefs.md`, 13
findings, all answered before any seat started): each repair names its finding as *(critic N)*. Read that
report's findings 1 to 10 too; they are measurements a seat may build on, each with its command.

## 1. The tree and the lane

- The sitting's tree: branch `lane-p255` at `a75179641` (`git log -1 --format=%h`), `git status --short`
  printing 0 lines; every seat's copy is a detached worktree at that commit.
- **The lane is the soundness lane** (compiler-engineer and ffi-pragmatist, `.claude/skills/panel/SKILL.md`
  § Two lanes). The test was the convener's: the full panel if the spec states the mechanism
  panel 251's R1 rules on. `grep -n -i -E 'setjmp|longjmp|jmp_buf|getcontext|swapcontext|signal handler|handler|fingerprint|save point' spec/heroes-spec.md`
  prints two lines, 427 and 428, both the RULE the mechanism enforces (*"A callback is a **parameter**,
  never a result or a signal handler, nor called by one"*, and *"No function of the program is a signal
  handler"*), and no word of the mechanism: no save point, record, fingerprint or jump. R1 itself says
  *"No spec token"* (`docs/panel/251-*.md`, § R1). What the lane gives up is said in the synthesis.
- The critic reads these briefs before any seat starts, and the reports after.

## 2. The question

`2679-question.md` beside this file is lane-core's analysis, copied verbatim on 2026-11-22 from
`.claude/worktrees/rebel-scratch/coord/2679-question.md` (git ignores that folder, so the copy is what a
later reader can open). Read it whole. Its options A, B and C, its reasons and its recommendation (**C on
Linux, A on Darwin until a seat builds Darwin's decode**) are lane-core's, and are put to you as a
proposal, never as premises. Defect 2679 is `gh issue view 2679 --repo heroes-lang/heroes` (read only;
open, `kind:defect`, `class:blocking`, `area:runtime`, filed 2026-11-22, 0 comments when read).

**Carried, its sources gone, its output kept** (critic 1): the question names its files under
`.claude/worktrees/lane-core-scratch/2679/`, and `ls` of that folder answers *No such file or directory*;
but lane-core's transcript is `.claude/worktrees/rebel-scratch/logs/lane-core-2679.log` (7,519 bytes),
holding the probe's printed lines: on arm64 *"slot 13 key agrees across two frames ... 1 slot(s) of 22
agree"*, then *"slot 10 ... slot 13 ... 2 slot(s) of 22 agree"* for two stack pointers **0x20 apart**; on
x86-64 *"slot 1 ... slot 6 ... 2 slot(s) of 8"*, then *"slot 6 ... 1 slot(s) of 8"*. It does **not** hold
the transformation: *"by `xor` on arm64"* and *"`ror 17` then `xor` on x86-64"* are the question's words
with no output behind them. So the slot, the transformation and the calibration's distance between frames
are a seat's to re-run (a wrong slot agreed at frames 0x20 apart: calibrate far apart, and say how far).
`ijstale.c`, the refill shape R1's fingerprint exists for, is in the tracked
`docs/panel/251-reports/evidence.tar.xz` (`251-compiler-engineer/q1/ship/ij/`), beside `ijv.c` (three ways
a handler jumps WITHIN itself, one of them a SIGUSR2 handler that `siglongjmp`s back into the still-running
SIGUSR1 handler, a shape with **no golden twin**: `grep -l SIGUSR2` over the 2522, 2558 and 2571 files
finds none), `ijo.c` (`SA_ONSTACK`) and `ijn.c` (the control); extract them with `tar -xJf` into your own
copy. So are: Darwin's `setjmp` mangling and signing its saved stack pointer (panel 251's
ffi-pragmatist, `dyld_info -disassemble`, carried); Docker's default seccomp profile refusing
`process_vm_readv` (carried, unrun); Arm MTE faulting a freed granule's read (carried, unrun); and
*"how common is unmeasured"* for thread-local buffers (the question's own word).

## 3. The defect, reproduced at this tree while the brief was written

The reproducers are `repro/` beside this file: `shapes.h` (six shapes, each a correct C program), one
`<shape>.hero` per shape, and `run.sh <compiler> <out>`, which builds each plain and with `--sanitize`
and prints the exit and the first sanitizer line. Run by the coordinator from its own copy
(`.claude/worktrees/p255-scratch/coord/`, a detached worktree at `a75179641`, its compiler from the seed):
this Mac in `repro/mac.log`; Linux arm64 (`heroes-linux-arm64:latest`, Debian clang 22.1.8, glibc 2.41,
`aarch64`, the image's own `clang --version`, `ldd --version`, `uname -m`) in `repro/linux-arm64.log`.

| shape (`shapes.h`) | Mac plain | Mac `--sanitize` | Linux arm64 plain | Linux arm64 `--sanitize` |
|---|---|---|---|---|
| 1 `heap`: a heap buffer saved into, freed, then a save (the issue's) | exit 0 | 134, heap-use-after-free, `#0 hero_rebind_fingerprint rebind.c:334`, `#1 hero_rebind_settle rebind.c:371` | exit 0 | 1, heap-use-after-free, `#0 hero_jmp_fingerprint sigjump.c:138`, `#1 hero_jmp_read_pending sigjump.c:148` |
| 2 `mmap`: a page mapped, saved into, unmapped, then a save | **139, SIGSEGV** | 134, SEGV, the same two frames | **139, SIGSEGV** | 1, SEGV, the same two frames |
| 3 `context`: a heap `ucontext_t` filled by `getcontext`, freed, then a save | exit 0, prints `context: -2` (the shape is compiled out by the header's `#if defined(__linux__)`) | the same | exit 0 | 1, heap-use-after-free, the same two frames |
| 4 `stack`: a stack buffer whose frame returned, then a save in another frame | exit 0 | exit 0, clean | exit 0 | exit 0, clean |
| 5 `handler_heap`: a buffer saved and freed, then a handler raised synchronously jumps out | exit 0 | 134, heap-use-after-free, the same two frames | exit 0 | 1, heap-use-after-free, the same two frames |
| 6 `handler_mmap`: the same with an unmapped page | **139, SIGSEGV** | 134, SEGV, the same two frames | **139, SIGSEGV** | 1, SEGV, the same two frames |
| 7 `plain_heap`: a static buffer saved, a heap buffer saved and freed, then `longjmp` to the static one, **no handler anywhere** (critic 2) | exit 0 | 134, heap-use-after-free, `#2 hero_rebind_jump rebind.c:425`, `#3 hero_rebind_longjmp rebind.c:478` | exit 0 | exit 0, clean |
| 8 `plain_mmap`: the same with an unmapped page (critic 2) | **139, SIGSEGV** | 134, SEGV, the same four frames | exit 0 | exit 0, clean |

Shapes 5 and 6 are the coordinator's, beside the question's four: the read happens at a JUMP inside a
handler too (`hero_jmp_leave` calls `hero_jmp_read_pending` at `sigjump.c:189`). Shapes 7 and 8 were added
on the critic's finding 2 and run by the coordinator (`repro/mac7.log`, `repro/linux7.log`, `SHAPES="plain_heap
plain_mmap" ./run.sh`): Darwin's `hero_rebind_jump` calls `hero_rebind_settle` at `rebind.c:425` BEFORE it
asks whether a handler is live (`now == NULL`, line 427), so on Darwin every `longjmp` reads given-back
memory, libpng's own error path included; Linux's `hero_jmp_leave` returns at `depth == 0` (line 187)
before it reads. **A Darwin route must remove the read at a jump outside every handler too.** `run.sh`
runs all eight shapes by default. `sizeof(jmp_buf)` is 312 on Linux arm64 (`linux-arm64.log`'s last line).

## 4. Where the mechanism lives (line numbers by `grep -n` and `sed -n` on this tree)

- `runtime/parts/sigjump.c`, 283 lines (`wc -l`), Linux and every POSIX but Darwin (`#if !defined(_WIN32)
  && !defined(__APPLE__)`, line 69): `hero_jmp_fingerprint` 135 (FNV-1a over the whole buffer),
  `hero_jmp_read_pending` 144 (every recorded pair not yet read, the defect's read), `hero_jmp_note` 156,
  `hero_jmp_leave` 185 (returns at once where `own->depth == 0`, line 187), the save points 212 to 238
  (each a `musttail` call to the C library's own, the reason the runtime cannot read the filled buffer at
  the save), the jumps 243 to 281. Its header comment, lines 10 to 67, states the design and the residue.
- `runtime/parts/sigguard.c` 317: `HERO_SIG_JUMPS 32` pairs per thread; 320 to 340: `HeroSignalStack`,
  `lo`, `hi`, `entries`, `depth`, `unsure`, `jump_buf`, `jump_frame`, `jump_print`, `jump_size`.
- `runtime/parts/siginstall.c` 150: `hero_sig_prune(own, at)`, which drops the entries `at` proves left
  (deeper on the same stack, or on an alternate stack while `at` is on the thread's own).
- `runtime/parts/rebind.c`, 727 lines, Darwin: route J by rewritten slots. Its record is a global table of
  4096 (`HERO_REBIND_RECORDS`, line 301) keyed by buffer and thread, each holding the ENTRY live at the save
  (*"The entry, never the frame, because Darwin's `setjmp` mangles the saved stack pointer"*, lines 52 and
  53), one pending buffer per thread in a `pthread` key (316 to 319); `hero_rebind_fingerprint` 330 (192
  bytes, `HERO_REBIND_PRINT_BYTES`, line 304), `hero_rebind_settle` 363 (the defect's read), `hero_rebind_note` 377
  (which calls it first, line 378),
  `hero_rebind_jump` 424, the slots 444 to 489, the add-image callback registered at 721.
- The cases: `tests/golden/run/` holds 25 `fixedbugs-2522-*.hero`, 5 `fixedbugs-2558-*.hero` and 4
  `fixedbugs-2571-*.hero` (`ls ... | wc -l`). The twelve `a-callback-after-*` cases share one header,
  `fixedbugs-2522-a-jump-out-of-a-handler-then-a-correct-callback.h`, which holds `static sigjmp_buf
  depth_sig_env` (`grep -l depth_sig_env`). Across the 2522, 2558 and 2571 headers the buffers declared are
  seven `static` ones, three locals and two function parameters (`sigjmp_buf env` and `jmp_buf env`,
`fixedbugs-2522-a-handler-of-cs-jumps-and-its-deep-callback.h:34, 36`; critic 11), by
  `grep -h -o -E '(static )?(__thread |_Thread_local )?(sig)?jmp_buf [a-z_]+'`; one header names `malloc`,
  `_Thread_local`, `__thread`, `dlopen` or `mmap` (`grep -l`):
  `fixedbugs-2522-a-handler-installed-past-the-runtimes-names.h`. No case keeps a buffer on the heap, in
  thread-local storage or in a late-loaded library by that grep, which is a reading of declarations and
  not of every use.
- The refill case R1's fingerprint exists for: `fixedbugs-2522-a-buffer-a-handler-refills-by-memcpy-and-jumps-to-is-refused`;
  the known cost ruled by the author's ruling B of 2026-11-20 (`sigjump.c:43` to 46):
  `fixedbugs-2522-a-correct-jump-out-of-a-handler-through-a-copied-buffer-is-refused-in-words-that-say-so`.
- **Beyond the three sets, what every route's jump also reaches** (critic 7):
  `fixedbugs-a-c-handler-that-recovers-leaves-no-line` (a handler that `longjmp`s and `siglongjmp`s out of
  `abort` and a trap through a static `recover_jump`, the canonical correct jump out); the other headers
  that install a handler, by the critic's grep (its finding 7 counts eighteen and names them by seven
  prefixes), run by the filters `fixedbugs-2494`, `fixedbugs-2257`, `fixedbugs-2583`,
  `fixedbugs-2496`, `fixedbugs-2503`, `an-ignored-signal-stays-ignored` and `the-platform-layer-stop`
  (14, 3, 6, 1, 1, 1 and 3 `.hero` cases, and 1 for the recovers case, `ls tests/golden/run | grep -- <filter>
  | grep -c '\.hero$'`; a filter is a substring of a case's name); and
  the `runtime` suite, whose census of the runtime's statics names `rebind.c`'s `hero_rebind_records`
  (`tests/harness/suite_runtime.hero:383`), which a route adding or removing a static moves.
  `runtime/parts/siglookup.c:134` to 145 is the table of the ten names' `dlsym` answers.
- **Two readings of today's code a builder needs** (critic 10, by reading, unrun): `hero_sig_on_own`
  (`siginstall.c:144`) knows only *own stack* or *not*, so `hero_sig_prune` compares an entry on the
  alternate stack with a target on a third stack (a `makecontext` stack) by address, as one stack; and
  `swapcontext` notes `from` and then reads every pending pair but `to`'s BEFORE the C library fills
  `from` (`sigjump.c:278` and 279, inside a handler; `rebind.c:465` and 466, at every swap), so a later
  jump back to that context reads a mismatch and is refused. A context case refused under today's runtime
  may be refused for this cause and not the one it tests: tell the two apart.
- **Today's verdicts, the baseline a moved verdict is read against** (critic 12, measured by the critic in
  its copy with stage 2, this Mac): `fixedbugs-2571` 4 passed 0 failed, `fixedbugs-2558` 5 and 0,
  `fixedbugs-2522` 25 and 0, each exit 0 (the 2522 set took `5:51.92` of wall, indicative). **Linux arm64's
  baseline is the compiler-engineer's** run of today's runtime in the container, unrun before the seats.

## 5. The routes, and the question about the set

The routes lane-core listed are A, B and C (`2679-question.md` § The options). **Asked by the coordinator,
as CLAUDE.md § RUN IT's list rule asks (what would have to be true for a route nobody listed to exist),
and put to you as QUESTIONS, not premises; none was built or run by the coordinator:**

- **D, the releases told**: the runtime learns of a release (`free`, `munmap`, `dlclose`, `realloc`) and
  drops the pending pairs inside it. What would have to be true: every release a program makes passes
  through a name the runtime can see. Questions: glibc's own unmapping of a thread's stack, a library's
  custom allocator, `madvise`, an arena; and a definition of `free` displacing AddressSanitizer's
  interceptor as `sigjump.c:56` says a jump's definition displaces it.
- **E, a read that cannot fault**: the bytes read through the kernel (`process_vm_readv`, or a raw
  `write` into a pipe), a fault answered `EFAULT`. What would have to be true: reading memory the program
  gave back, by a path that cannot fault, is robust by § Precedence's rank 3. Questions: what such a read
  means for a freed block, its cost per save point (a system call for each pending pair), and whether
  AddressSanitizer's `write` interceptor sees it.
- **F, the save read at its return**: the save point not a tail call. What would have to be true: the
  C library's own saves the CALLER's frame from a call that is not a tail call. Lane-core's paragraph says
  it is not available; that is carried.
- **The Darwin decode**: whether Darwin's saved stack pointer can be decoded at a jump on this Mac (a
  cookie, a signature, both; `xpacd`), which decides whether C reaches Darwin. Carried from panel 251
  (*"`eor` with a cookie, then `autdb`"*, `251-reports/ffi-pragmatist.md:75`); the ffi-pragmatist's to measure.
- **T, a token in a span no save writes** (critic 4, a route nobody listed; its first premise measured by
  the critic, `critic-briefs.md` finding 4): the save point writes an identifier into bytes of the buffer
  the C library's save never writes, before its tail call; a jump reads that span of its TARGET only
  (it lives, the C library reads it next) and takes the record the identifier names. A whole-buffer
  `memcpy` carries its source's identifier, so the refill case names the handler's own save and stays
  refused, and the copied-buffer case names its source and would run. The critic's probe (fill with a
  pattern, save, read what changed; plain clang, the C library's own saves) found, never written by
  `setjmp`, `_setjmp`, `sigsetjmp(,0)` or `sigsetjmp(,1)`: **Darwin arm64** `[104,112)` and `[180,184)`
  (`jmp_buf` 192, `sigjmp_buf` 196 bytes); **glibc 2.41 arm64** `[96,104)`, `[180,184)`, `[192,312)` (312
  and 312). Unmeasured, and what a builder settles: (a) that no jump of the C library reads or checks the
  span; (b) a partial copy of a buffer, which leaves an old identifier over new registers (the dangerous
  side); (c) an identifier colliding with a stale one; (d) contexts (`ucontext_t`'s spare fields); (e)
  **whether the runtime may write into memory the program handed the C library**, a soundness question
  for both vetoes; (f) a buffer the C library fills from a save the runtime never sees, which keeps an
  old identifier (today's fingerprint and C's decode both see that; T alone does not).
- **The composition by calibration** (critic 5): C where its calibration verifies, and where it does not
  (glibc's four other architectures `siglookup.c:68` to 79 names, a future glibc) A or T rather than the
  told refusal of every jump out of a handler the question names as C's fallback, which is below A.
- **F in assembly** (critic 5): a save point that is not a tail call yet leaves the caller's frame as the
  C library must see it. On arm64 a `bl` writes only `x30` and leaves `sp`; on x86-64 a `call` moves `rsp`
  by 8. The difficulty is where the caller's return address and the stub's registers live across a later
  jump, which the C library restores from the buffer alone. Judged as that shape, not as a C `musttail`.
- **Any route you find**, named in your report with what would have to be true for it.

## 6. How the sitting decides

CLAUDE.md § Precedence (rank 3, robustness: a Heroes program must not segfault and must not corrupt
memory) and § 4: the sitting adopts **the most robust and complete route, never the cheapest and never a
compromise**, records what conservative would have been, and marks it *provisional, author ratification
pending*. **A route that does not build is not adopted** (`.claude/skills/panel/SKILL.md` § 3c): each seat
builds the routes it argues for in its own copy and runs the reproducers and the cases. A seat's veto is a
refusal on soundness (a read of memory the program gave back, a fault, a corruption, Heroes code running
inside a signal), never a price; a cost is reported. Whether a route changes what ruling B accepted (the
copied buffer refused) or adds a new known cost (a correct program refused) is said, with the case that
shows it, for the author: ruling B was the author's, and a change to it is theirs to read.

**Amended by the author today** (`CLAUDE.md` § Precedence, commit `691531908`, *"the most robust solution
now always wins, a new form included and at the cost of the spec's size"*; critic 13). So two questions
every seat answers per route, with the program that shows it:
- **May one program's verdict depend on the platform?** (critic 9) Under *C on Linux, A on Darwin* the
  copied-buffer case would run on Linux and be refused on Darwin, and a heap or thread-local buffer jumped
  out of a handler likewise. The run harness keeps one `.expected` per case (920 `.hero`, 920 `.expected`)
  and its skip lists name Windows, `elsewhere` and `darwin_alone` only
  (`grep -n '^function ' tests/harness/platform_skips.hero`). `grep -n -i -E 'every platform|all (three
  )?platforms|portab' docs/design.md` found no rule either way: a question, not a premise.
- **Does the route owe the spec a sentence?** Spec lines 427 and 428 say a callback is never called by a
  signal handler; a reader predicts that a program which has LEFT the handler, by any jump, runs its
  callback. Name every correct program the route refuses that such a reader expects to run. If a route
  the sitting may adopt owes the spec words, the soundness lane has no seat to write or price them, and
  the synthesis says so.

## 7. How to build and run in your copy (measured here)

- Stage 1 from the seed: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, **real 23.78 s** in
  the coordinator's copy this afternoon (`/usr/bin/time -p`, other lanes' work running; the skill's 3.3 s
  was a still machine). `./heroes --version` printed `heroes 0.6.0` and `sources 05186f2b38e77a97`.
- **The net refuses that compiler on this tree**: `.claude/hooks/guard_bash.py` refused
  `./heroes run tests/harness/main.hero -- ./heroes run fixedbugs-2571` in the coordinator's copy, the tree
  keying `61416c0c33ecc5a9` by the hook's mirror against the compiler's `05186f2b38e77a97`. So build stage
  2, `./heroes build selfhost/main.hero -o heroes-next`, then `mv heroes-next heroes` in a command of its
  own, and again **after every edit under `runtime/`** (the tree's key includes the runtime,
  `.claude/hooks/provenance.py` lines 32 to 34). Its time in the coordinator's copy is in § 9.
- A program built from a subfolder of your copy uses your copy's runtime: `heroes doctor` run from the
  coordinator's `repro/` printed `ok runtime <copy>/runtime`. Check the row in yours.
- The cases: `./heroes run tests/harness/main.hero -- ./heroes run fixedbugs-2522` (and `fixedbugs-2558`,
  `fixedbugs-2571`); the harness's third word filters by name, and zero matches exit 2. Never pipe the
  net into `head` or `tail` (the hook refuses it): send it to a file and read the file.
- **Linux arm64**: Docker answered (`docker info`); the image `heroes-linux-arm64:latest` is present
  (`docker images`), with `heroes-linux-arm64:gtk4-ui`, `gtk4-web` and `gtk-ui` beside it (GTK, so libpng,
  the issue's origin) and `heroes-linux:latest` (Linux x86-64, emulated on this Mac). The coordinator's run,
  81.34 s real for the copy, the seed build and the twelve builds and runs:
  ```
  docker run --rm -v "$PWD":/src:ro heroes-linux-arm64:latest bash -c '
    mkdir -p /w && cd /w && tar -C /src --exclude=.git --exclude=build --exclude=heroes -cf - . | tar -xf -
    clang -I runtime seed/heroes.c runtime/runtime.c -o heroes
    cd <your folder> && ./run.sh ../heroes out'
  ```
  Inside the container the hook does not run; build stage 2 there before any harness run all the same, so
  the compiler that judges is the tree's. Never one wait over 3 minutes without a probe: run long
  container work in the background (`docker run -d`, then `docker logs`), and check it every few minutes.
- **The CI's legs** (`.github/workflows/ci.yml:373`; critic 3): `ubuntu-26.04` (Linux x86-64),
  `ubuntu-26.04-arm` (Linux arm64), `macos-26` (Darwin arm64), `windows-latest`. Both local Linux images
  are Debian's glibc 2.41, and this Mac is macOS 27.2.1 (`sw_vers`, the critic's), so a premise measured
  here first meets Ubuntu 26.04's glibc and `macos-26`'s libsystem_platform on the CI after the push,
  unless a seat runs its probe in `ubuntu:26.04` (`docker pull ubuntu:26.04`, native arm64; a probe binary
  built dynamically in the Debian image runs there against Ubuntu's glibc). `/usr/bin/time` is absent in
  `heroes-linux-arm64:latest`: use the shell's `time` there.
- **x86-64**: only the emulated image exists here, slow, fit for a probe and the shapes; the cases under
  a route on x86-64 are run in the background where a seat can afford it, or said unrun.
- **musl is no platform any route reaches** (critic 3, measured in `postgres:16.15-alpine`): the runtime
  does not compile against it (`siglookup.c:87` `dlvsym`, `:173` `dladdr1` and `RTLD_DL_LINKMAP`
  undeclared), and no CI leg names it. The non-Darwin POSIX arm is glibc's in fact, on six architectures
  by `siglookup.c:68` to 79 (x86-64, arm64 and powerpc64, riscv, arm, s390x), two of them measurable here.
  What a route gives the other four is judged by what it does when its premise fails there, by reading.
- **Darwin x86-64**: `rebind.c` has an x86-64 arm (`grep -n '__x86_64__' rebind.c`: 517) with no CI leg
  and no Rosetta here (`arch -x86_64` answered *Bad CPU type*, the critic's); judged by reading only.
- **Windows**: none of this exists there (`sigguard.c:172` to 180, *"none on Windows"*), so the defect and
  every route stop at that leg.

## 8. What binds every seat

- **Your own copy only**, a detached worktree named in your brief; never the trunk's `./heroes`, never
  another seat's folder, never `lane-p255` but for your report file. `export TMPDIR=<your copy>/p255/tmp`.
  No file outside the repository's root (`/tmp` included): your work goes under `<your copy>/p255/`.
- **Your report** is `docs/panel/255-reports/<seat>.md` in `lane-p255`, written **as you go**, so a seat
  stopped early leaves what it had.
- **No paid run** (no `claude -p`, no API call, no cloud run); one you find worth running goes in your
  report with its size. **No credential printed**: `gh auth status` if you must know, never `gh auth
  token`. **No GitHub write, no push, no commit, no merge.**
- **A program that may not end** runs as a built binary under `timeout 20`, its output to a file through
  `head -c 1000000` or to `/dev/null`, and after a timeout `pgrep -f p255/` is read and what it names is
  stopped.
- **Timing is not owed.** Robustness outranks speed (§ Precedence). A seat that times anything says it
  ran beside other seats' work and reads `real` against `user` plus `sys`; such a number is indicative and
  no resolution rests on it alone.
- **Every claim in your report carries the command that settled it**, or says *unrun*.

## 9. Measured after the briefs were first written, before any seat

In the coordinator's copy, this Mac at load 9.5 to 13 (`uptime`, other lanes' work running):
- stage 2, `./heroes build selfhost/main.hero -o heroes-next`: **real 249.16**, user 171.97, sys 43.05,
  exit 0 (`stage2.log`); then `mv heroes-next heroes`, and the hook let the harness run;
- `./heroes run tests/harness/main.hero -- ./heroes run fixedbugs-2571`: `run (only fixedbugs-2571): 4
  passed, 0 failed`, **real 160.56**, user 112.66, sys 22.97, exit 0 (`harness-2571.log`), the harness's
  own build included. So the 25 cases of `fixedbugs-2522` are a run of many minutes: start it in the
  background (`run_in_background`, or `nohup ... &` into a file) and read the file when it ends.

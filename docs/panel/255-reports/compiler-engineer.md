# Panel 255, compiler-engineer: the ceiling and the soundness of each route on Linux

Written 2026-11-22 as I went, by the compiler-engineer seat (soundness lane, defect 2679). Every
measurement below was run in my copy, `.claude/worktrees/p255-scratch/compiler-engineer/` (written `CE/`
below), a detached worktree at `a75179641`, `TMPDIR=CE/p255/tmp`, unless a line says otherwise; the
command that settled each claim is beside it, and what I could not reach says *unrun*. Read whole before
any run: `00-shared.md`, `critic-briefs.md` (findings 1 to 13), `2679-question.md`, panel 251's § R1, my
251 report lines 560 to 600.

## Q2. The premise C rests on, re-run (answered first: it needs no stage 2)

**The probe**: `CE/p255/probe/mangle.c` (165 lines). Each save goes through an assembly stub that
records the EXACT stack pointer the C library's save will see and branches to it with no frame of its
own (arm64 `mov x9, sp; str x9, [x1]; br x2`; x86-64 `lea 8(%rsp), %rax`, since glibc's x86-64 save
stores the stack pointer as it will be after the return). For every word of the buffer and every
rotation 0 to 63 the candidate key is `ror(word, r) ^ sp`, and a (word, rotation) whose key agrees at
two frames is printed with its key. The known slot is then verified at both frames, on a second
thread, in a forked child, and end to end: a jump by `longjmp`, `siglongjmp` and `__longjmp_chk` to a
`sigsetjmp(,1)` buffer lands (read by a second stub) at the stack pointer the decode named. Built
with `clang -O0` and `-O2 -pthread` in each image; logs `CE/p255/probe/{arm64,x86,ubuntu2604-arm64,ubuntu2604-x86}.log`.

**Frames far apart**: `0x101130` apart (1 MiB plus `0x1130`, an `alloca` of 256 bytes against one of
`1<<20 | 0x1230`, so high and low bits both move), and `0x1c5200` to `0x242200` apart for the same
frame shape 4,000 calls deeper (`SAME=4000`, where a frame-pointer word keeps its offset to the
stack pointer). And the critic's `0x20` (an `alloca` of 256 against 288).

| | arm64 (Debian glibc 2.41, native) | x86-64 (Debian glibc 2.41, emulated) |
|---|---|---|
| buffer | `jmp_buf` = `sigjmp_buf` = 312 bytes, 39 words | 200 bytes, 25 words |
| the slot and its transformation, measured | **word 13, `ror 0`, i.e. `sp ^ key`** (`xor` alone) | **word 6, `ror 17` then `^ key`** (stored = `rol(sp ^ key, 17)`) |
| the same for `setjmp`, `_setjmp`, `sigsetjmp(,0)`, `sigsetjmp(,1)`, `-O0` and `-O2` | yes, all eight | yes, all eight |
| one key per process: a second thread, a forked child | THE SAME, THE SAME | THE SAME, THE SAME |
| a jump by `longjmp`, `siglongjmp`, `__longjmp_chk` lands at the decoded stack pointer | EQUAL, all three, `-O0` and `-O2` | EQUAL, all three |
| other (word, rotation) pairs whose key agreed at two far frames, `-O0` | **none** of 2,496 | **none** of 1,600 |
| the same, `-O2` | **word 3** (far) or **word 0** (same shape), each with **key 0** | **word 5** (far) or **word 4** (same shape), each with **key 0** |
| at `0x20` apart | `-O0` none, `-O2` word 3 (key 0) | `-O0` none, `-O2` word 5 (key 0) |

So a second word CAN pass a calibration that only asks *"does the key agree at two far frames"*: at
`-O2` a callee-saved register holding exactly the stack pointer (the `alloca`'s base) agrees with key
0 at any distance. Distance alone does not rule it out; what rules it out is what the stub puts in
the registers. **The calibration I built (Q3) therefore does not search and does not trust distance**:
its stub is a whole function that fills every callee-saved register with a pattern before it calls the
C library's save, so no word but the stack pointer's can relate to the stack pointer, and it verifies
three facts with one key: the known stack-pointer word decodes to the exact stack pointer at two
frames `0x101130` apart, a SECOND mangled word decodes to its known value with the same key (arm64 the
link register, word 11, to the stub's return address; x86-64 `rbp`, word 1, to its pattern, and the
program counter, word 7, to the stub's return address), and the unmangled words hold their patterns
(arm64 words 0 to 10, `x19` to `x29`; x86-64 words 0, 2 to 5). A glibc that moved any of these fails
the calibration.

**The CI's glibc**: `docker pull ubuntu:26.04` answered (native arm64, and `--platform linux/amd64`
emulated). The Debian-built binaries run there against **Ubuntu GLIBC 2.43-2ubuntu2.4** (`ldd
--version`): every line the same as Debian's 2.41 on both architectures (word 13 `xor` / word 6
`ror 17`, eight saves agreeing, one key on a second thread and after `fork`, the three jumps landing
EQUAL; the `-O2` key-0 word again word 3 / word 5). So the premise holds on the CI legs' glibc, by a
run, before the push.

## Q1. Linux arm64's baseline, today's runtime

**How**: `CE/p255/linux.sh` (my container driver), in a container of `heroes-linux-arm64:latest` named
`ce255-today`, my copy mounted read-only and `CE/p255/out/today/` for its files. Inside: the tree
copied, stage 1 from the seed (29 s), **stage 2** `./heroes build selfhost/main.hero -o heroes-next`
then `mv` (stage 2 ready at 198 s; `heroes 0.6.0 sources 61416c0c33ecc5a9`, the tree's key; `heroes
doctor`: `ok runtime runtime`, the copy's own). `aarch64`, `Debian GLIBC 2.41-12+deb13u3`, `Debian
clang version 22.1.8`. Whole leg 584 s (the sets four at a time by `xargs -P 4`). Files: `leg.log`,
`shapes.log`, `ij.log`, `h-<filter>.log`, `h-runtime-suite.log`.

| what | today's runtime, Linux arm64 |
|---|---|
| shape 1 `heap` | plain exit 0 `heap: 0`; `--sanitize` exit 1, heap-use-after-free |
| shape 2 `mmap` | plain **exit 139, SIGSEGV**; `--sanitize` exit 1, SEGV |
| shape 3 `context` | plain exit 0; `--sanitize` exit 1, heap-use-after-free |
| shape 4 `stack` | exit 0 both, clean |
| shape 5 `handler_heap` | plain exit 0; `--sanitize` exit 1, heap-use-after-free |
| shape 6 `handler_mmap` | plain **exit 139**; `--sanitize` exit 1, SEGV |
| shapes 7, 8 `plain_heap`, `plain_mmap` | exit 0 all four, clean |
| `ijn`, `ijv` 1 (and fortified), 2, 3, `ijo`, at `-O0` and `-O2`, 5 runs each | refused 5 of 5 each, exit 134, *"... a signal handler of C's that this runtime saw begin had not been seen to end ..."* (STANDING) |
| `ijstale` `-O0`, `-O2` | refused 5 of 5 each, exit 134, *"... jumped to a buffer this runtime holds no record of as it now stands ..."* (UNFOLLOWED) |
| `fixedbugs-2522` | `run (only fixedbugs-2522): 25 passed, 0 failed`, exit 0 |
| `fixedbugs-2558` | `5 passed, 0 failed`, exit 0 |
| `fixedbugs-2571` | `4 passed, 0 failed`, exit 0 |
| `fixedbugs-a-c-handler-that-recovers-leaves-no-line` | `1 passed, 0 failed`, exit 0 |
| `fixedbugs-2494` | **`15 passed, 0 failed`** (the filter matches 15 `.hero` today, not the brief's 14), exit 0 |
| `fixedbugs-2257`, `-2583`, `-2496`, `-2503` | 3, 6, 1, 1 passed, 0 failed each, exit 0 |
| `an-ignored-signal-stays-ignored`, `the-platform-layer-stop` | 1 and 3 passed, 0 failed, exit 0 |
| the `runtime` suite (`-- ./heroes runtime`) | `runtime: 10 passed, 0 failed, 2 floors outgrown` (`runtime/threads`: *"objects that survive between calls: 348 against a floor of 275"*), exit 0 |

The eight shapes match the coordinator's table cell for cell. Every count line read whole from its file.

## Q3. Route C on Linux, built: `CE/p255/routes/C/sigjump.c`

**What it is** (561 lines against today's 283; `diff` +350 / -72; 108 of the lines comment). One file
replaces `runtime/parts/sigjump.c`; no other file of the runtime and **no line of the compiler** moves.

- **The calibration** (a constructor beside `hero_jmp_find`'s, never a handler, no lock): an assembly
  probe per architecture (`hero_jmp_probe`, `.hidden`) that saves every callee-saved register, loads a
  pattern into each, records the exact stack pointer and its return address, and calls the C library's
  own save. `hero_jmp_try` then verifies, under ONE key, every word the probe knows: arm64 words 0 to 9
  the patterns of `x19` to `x28`, word 10 the probe's `x29`, word 11 `^ key` its return address,
  word 13 `^ key` the stack pointer; x86-64 words 0, 2 to 5 the patterns, word 1 `ror 17 ^ key` the
  `rbp` pattern, word 7 the return address, word 6 the stack pointer. It does so for `setjmp`,
  `_setjmp`, `__sigsetjmp` and (the stack pointer only) `getcontext`, each at two frames `0x9230`
  bytes apart (an `alloca` between). A key is taken only where all of it holds; it never searches.
  Q2's trap (a register holding the stack pointer at `-O2`, key 0) cannot pass: every callee-saved
  register holds a pattern when the save runs.
- **The jump** reads ONE word of its target, `ror(word, ROT) ^ key`, or a context's
  `uc_mcontext.sp` / `gregs[REG_RSP]`, and calls `hero_sig_prune(own, sp)`. At `depth == 0` it reads
  nothing, as today. No pending buffer is read, ever.
- **The save points** stay defined and, once the key is verified, do nothing but branch to the C
  library's own (`hero_jmp_recording()` false).
- **The fallback**, where the key is not verified: **A** (below), the record kept by the save points
  and its fingerprint read only of a buffer on the thread's stack, its current alternate stack, or a
  writable segment of an image loaded before any constructor (read at `.preinit_array` by
  `dl_iterate_phdr`, so none can be unloaded); anything else loses its record unread. A premise
  this adds: a `.preinit_array` entry in EVERY build, where today's runtime has one under ASan alone
  (`siglookup.c:154` to 165), so the runtime must never be linked into a shared object (`grep -rn
  '"-shared"' selfhost/` finds one, `cli/produce.hero:357`, a test library of its own, not the
  runtime). Every plain and sanitized build of every leg linked and ran with it.
- **Statics**: `hero_jmp_decoding`, `hero_jmp_key`, `hero_jmp_image_lo`/`_hi` (64 each),
  `hero_jmp_images`, all `_Atomic`, so the `runtime` suite's census needs no entry (its first draft had
  plain ones and I made them atomic before any run: the census would have refused them,
  `tests/harness/suite_runtime.hero:528` onward). The census reads 353 under C and A against 348 today
  (`objects that survive between calls`), `runtime: 10 passed, 0 failed` in every leg.

**Ran** (Linux arm64, `ce255-C`, stage 2 built in the container with C's runtime, `sources
0d61aefe7224cd4b`; leg 1,159 s; files `CE/p255/out/C/`):

| case set or shape | today | **C** |
|---|---|---|
| the eight shapes, plain and `--sanitize` | 2 SEGV plain, 5 ASan reports | **16 of 16 exit 0, clean**; `mmap` and `handler_mmap` no longer 139 |
| `ijn`, `ijv` 1/1f/2/3, `ijo`, `-O0` and `-O2`, 5 runs each | refused, STANDING words | refused, STANDING words, 5 of 5 each |
| `ijstale` (the refill R1 exists for) | refused, UNFOLLOWED words | **refused**, STANDING words (the decode read the handler's own stack pointer: the proof the key verified) |
| `fixedbugs-2522` | 25 passed, 0 failed | **23 passed, 2 failed**, exit 1 |
| `fixedbugs-2558`, `-2571`, `recovers-leaves-no-line`, `-2494`, `-2257`, `-2583`, `-2496`, `-2503`, `ignored-signal`, `platform-layer-stop` | all passed | all passed, the same counts (5, 4, 1, 15, 3, 6, 1, 1, 1, 3), exit 0 |
| `runtime` suite | 10 passed, 0 failed | 10 passed, 0 failed |

**The two 2522 failures, read whole** (`h-fixedbugs-2522.log`): (1)
`a-buffer-a-handler-refills-by-memcpy-and-jumps-to-is-refused` is **still refused**, exit 134, and fails
only on its words: it pins UNFOLLOWED (*"jumped to a buffer this runtime holds no record of"*) and C
says STANDING (*"... a signal handler of C's that this runtime saw begin had not been seen to end"*),
which is the truer sentence: under C the runtime knows where the jump landed. (2)
`a-correct-jump-out-of-a-handler-through-a-copied-buffer-is-refused-in-words-that-say-so` **runs**,
`called 8`, exit 0: **ruling B's known cost is lifted** (a change to the author's ruling, theirs to read).
A landing rewrites the first case's expected words and turns the second into a case that must run.

**What the calibration's failure gives, by a case run** (`CE/p255/routes/Cfail/`, C with
`HERO_JMP_FORCE_UNCALIBRATED`; `ce255-Cfail`): every row reads exactly as A's (below): the eight shapes
16 of 16 clean, `ijstale` refused in UNFOLLOWED words (the record path ran, so the decode was off),
`fixedbugs-2522` **25 passed, 0 failed**, every other set as today, `runtime` 10 and 0. So the
fallback is A, measured, and it is the most robust of the three the brief names: *keep every entry*
(by reading, unbuilt) refuses every correct jump out of a handler that today follows (`x_static`, the
twelve depth cases); T widens the dangerous residue (Q4). A refuses less than *keep every entry* and reads no
given-back memory. Where it runs: glibc's four other architectures by `siglookup.c:68` to 79
(powerpc64, riscv, arm, s390x; unrun, no machine), Darwin never (its own file), and a glibc whose
layout moved (forced here).

**Whether the save points stay defined**: under C as built, **yes**, for A. Removing them (and A with
them, the fallback becoming *keep every entry*) would move: `siglookup.c:134` to 145's table by four
rows (`setjmp`, `_setjmp`, `__sigsetjmp`, `getcontext`); `ffi_defined_elsewhere` by **no compiler
line** (`grep -rn setjmp selfhost/cli/supplied*.hero` finds nothing: the member asks the link who
supplies each name the runtime's object defines, so the four names simply stop being the runtime's);
the `jump_*` fields and `HERO_SIG_JUMPS` (`sigguard.c:317` to 340, 900 bytes per claimed thread)
would go; and every correct jump out of a handler on the other four architectures would be refused. I
keep them: robustness on a platform nobody measures is worth 900 bytes a thread.

**The header comment's residue under C, line by line** (`sigjump.c:59` to 67 today):
- *"a buffer refilled between its save and its jump by a save this file never saw ... with no other
  save or jump between"*: **closed under C**, measured: `x_unseen0` (a static buffer refilled inside
  the handler by glibc's own `__sigsetjmp` reached by `dlvsym`, a jump within the handler, the callback
  twelve frames down) runs the Heroes function inside the signal under today, A and Cfail (3 of 3,
  plain and `--sanitize`, *"answer ran for 5"*) and is **refused under C** (STANDING, 3 of 3 each). It
  stays a residue of the fallback.
- *"`makecontext`'s new stack ..."*: **stays**, measured below (the third stack).
- *"a C++ exception or `_Unwind_*` out of a handler"*: stays (no jump is called); unrun.
- *"A program library defining one of these ten names is told `ffi_defined_elsewhere`"*: stays while
  the save points are defined.

**Two stacks, and a third** (`CE/p255/extra/extra.h` `x_ctx`; a handler, on the alternate stack
(`s1`) or the thread's own (`s0`), `makecontext`s a context on a third `mmap`ed stack and swaps into
it; `o0` the third stack ABOVE the alternate one, `o1` below; mode 1 the context calls the Heroes
function, inside the signal, MUST be refused; mode 2 the context returns, the handler returns, the
function runs deep after, MUST run; mode 3 the context swaps back explicitly and the handler calls the
function, MUST be refused). 3 runs each, plain and `--sanitize`:

| | today | A | C | T |
|---|---|---|---|---|
| m1 o0 s0, m1 o1 s0 (handler on its own stack) | **runs inside the signal** | **runs** | **runs** | **runs** |
| m1 o0 s1 (third stack above the alternate) | **runs inside the signal** | **runs** | **runs** | **runs** |
| m1 o1 s1 (third stack below) | refused (STANDING) | refused | refused | refused |
| m2, all four | runs | runs | runs | runs |
| m3, all four | refused, **UNFOLLOWED** words | refused, UNFOLLOWED | refused, STANDING (o0 s1: the ONSTACK test's words) | refused, STANDING |

Mode 3 tells the two causes apart: today's UNFOLLOWED words are the `swapcontext` pre-fill cause
(critic 10: `sigjump.c:278` notes `oucp`, `:279` fingerprints it before the C library fills it, so the
jump back reads a mismatch), and C, which reads no fingerprint, refuses the same program for the cause
tested, in STANDING words. Mode 1 is the finding: **in three of four orientations a Heroes function
runs inside a signal handler's dynamic extent under today's runtime and under every route**, because
`hero_sig_inside` (`siginstall.c:211` to 220) counts only entries on the same side of
`hero_sig_on_own` as the thunk's frame: an entry on the thread's own stack is never compared with a
frame on a third stack, and an entry on the alternate stack is compared by address with one. It is the
residue today's header names (*"a context it starts runs on a stack neither the thread's own nor an
alternate one"*), now measured, and it is not 2679's: no route closes or opens it. What would close
it: a frame on a stack that is neither the thread's own nor its current alternate one
(`sigaltstack(NULL)`'s bounds, a system call, async-signal-safe) refused wherever any entry stands, and
`hero_sig_prune` dropping nothing for a target on such a stack. **Unbuilt** by me; it is the
ffi-pragmatist's 251 point about two stacks, which a decoded stack pointer alone does not answer.
*(Added the same afternoon: built and run after this paragraph was written, as C3, below; it closes
mode 1 in all four orientations.)*

## Q4. Route T on Linux, built: `CE/p255/routes/T/sigjump.c`

**What it is** (226 lines against 283; `diff` +55 / -112). Each save point takes a process-wide
64-bit token (`_Atomic`, never 0), writes it into the buffer **before** its tail call, and keeps
(token, frame) in the thread's 32 pairs (a pair taken back where the same buffer address was saved
before, as today); a jump reads that one word of its TARGET and prunes by the record it names; a token
no record names keeps every entry. The word: offset 96 of a `jmp_buf` on both architectures, of a
`ucontext_t` 96 on arm64 and 304 on x86-64. No pending buffer is ever read.

**The spans, measured** (`CE/p255/probe/untouched.c`, the critic's method, `-O0` and `-O2`,
`untouched.log`): glibc 2.41 arm64 never writes `[96,104) [180,184) [192,312)` of a `sigjmp_buf` over
the four saves (the critic's), and `getcontext` never writes `[0,40) [48,184) [192,328) ...` of a
4,560-byte `ucontext_t`; x86-64 (emulated) never writes **`[68,72) [80,200)`** of its 200 bytes, and of
its 968-byte `ucontext_t` `[0,40) [56,72) ... [304,424) [452,968)`.

**(a) Whether a jump of the C library reads or checks the span: the measured spans are NOT the C
library's spare bytes**, by disassembly (`llvm-objdump -d` of each image's own `libc.so.6`):
- Debian glibc 2.41 arm64: `__sigsetjmp` (0x36600) runs `chkfeat`, and where the Guarded Control
  Stack is on, `mrs x2, GCSPR_EL0; str x2, [x0, #0xd0]`: **offset 208, inside the critic's
  `[192,312)`**; and the jump (0x36740 onward) `ldr x3, [x0, #0xd0]` and unwinds the GCS to it.
- **Ubuntu 26.04's glibc 2.43, the CI's** (copied out of `ubuntu:26.04`, both architectures): arm64
  the same `str x2, [x0, #0xd0]`; **x86-64 `__sigsetjmp`: `testl $0x2, %fs:0x48; rdsspq %rax; movq
  %rax, 0x58(%rdi)`**, the shadow-stack pointer at **offset 88, inside the span `[80,200)` this
  container's glibc (built without it) never writes**.
So a span measured *never written* on a machine without shadow stacks is exactly where glibc keeps the
shadow stack on a machine with them. My T avoids both (96 to 104 is neither 88 to 96 nor 208 to 216),
and its token is written BEFORE the C library's save, so a glibc that took those bytes would overwrite
the token, never the reverse: the jump would then find no record and refuse. Word 12 on arm64 and
`__saved_mask.__val[3]` on x86-64 are read by no jump of 2.41 or 2.43: the offsets each internal
`__longjmp` reads, listed by `llvm-objdump` and `grep -oE` of its operands, are Debian arm64 `[x0]`
to `#0x50`, `#0x68` to `#0xa0`, `#0xd0`; Ubuntu arm64 the same; Debian x86-64 `0x0` to `0x38(%rdi)`;
**Ubuntu x86-64 `0x0` to `0x38` and `0x58(%rdi)`**, the shadow stack read back (the `siglongjmp`
wrappers add `0xb0`/`0x40` and the mask). None reads 96. A premise about glibc's private layout all
the same, which glibc has already moved twice (the shadow stack and the GCS).

**(b) A partial copy**: `x_partial` (a static buffer, a seen save between, inside the handler a seen
save of which ONLY `__jmpbuf` is copied into the static buffer, a jump within the handler, the callback
twelve frames down; MUST be refused). Linux arm64, 3 runs each, plain and `--sanitize`: today refused
(UNFOLLOWED), **C refused**, **T refused** (STANDING): `__jmpbuf` is 176 bytes there and carries word
12, the token. On x86-64 `__jmpbuf` is 64 bytes and the token lies at 96, so the copy leaves the static
buffer's OLD token over the handler's registers: by reading, T drops the handler's live entry there.
The emulated x86-64 run of it is below (Q6) or said unrun.

**(c) A stale token**: a 64-bit count is not reused in a process's life; a token whose record a full
table overwrote finds none, the told refusal. No collision case could be built.

**(d) Contexts**: the token at 96 (arm64) and 304 (x86-64) lies in `uc_sigmask`'s tail, which
`getcontext` never writes and a `sigfillset` by the program would (the token then names nothing: a
refusal). `makecontext` is not wrapped, so a context it makes keeps `getcontext`'s token and is judged
by `getcontext`'s frame, not by its new stack: today's flaw, kept. The third-stack table (Q3) reads
the same under T as under C.

**(e) May the runtime write into memory the program handed the C library?** My answer as a soundness
seat: **yes, within the object and only before the C library's own save**: `setjmp`, `sigsetjmp` and
`getcontext` are given the whole object to fill, the program may not rely on any byte of it but
through a jump, and from the program's side the runtime's definition IS the C library's save (it
already is: the program's call reaches this file first). Two conditions make it sound and T as built
meets both: the bytes lie inside `sizeof` of the type the program declared (96 + 8 <= 200 and 312;
304 + 8 <= 968 and 4,560), and the write precedes the C library's, which therefore wins any byte it
also writes. It is not where T fails.

**(f) A buffer the C library fills from a save the runtime never sees: here T fails, measured.**
`x_unseen`: a static buffer saved (seen) at a shallow frame, a seen save between, then INSIDE the
handler refilled by glibc's own `__sigsetjmp` reached by `dlvsym(RTLD_NEXT, ..., "GLIBC_2.17")` (as a
library bound to the C library directly would), a jump to it within the handler, and the Heroes
function called twelve frames down from the still-running handler:

| `x_unseen` (MUST be refused), Linux arm64, 3 runs | plain | `--sanitize` |
|---|---|---|
| today | refused (UNFOLLOWED: the fingerprint saw the bytes change) | refused |
| A, Cfail | refused | refused |
| C | refused (STANDING: the decoded stack pointer is the handler's) | refused |
| **T** | **runs: `answer ran for 5`, exit 0, inside the signal** | **runs, exit 0** |

The unseen save leaves T's token (glibc writes no byte of it), so T takes the shallow record and drops
the handler's live entry. **T turns a program today refuses into Heroes code running inside a signal
handler.** That is the dangerous side, the one R1's fingerprint was adopted to close, and the reason I
veto T (below). (`x_unseen0`, the same with no save between, runs under today, A and T alike: today's
documented residue; only C refuses it.)

**Ran** (Linux arm64, `ce255-T`, stage 2 with T's runtime, `sources` its own; leg 1,093 s): the eight
shapes 16 of 16 exit 0, clean; the `ij` cases refused as under C (`ijstale` in STANDING words);
`fixedbugs-2522` **23 passed, 2 failed** (the same two as C: the refill case still refused but in
STANDING words, the copied-buffer case running, `called 8`); every other set as today; `runtime` 10
and 0, census 349.

## Q5. Route A on Linux, the conservative arm: `CE/p255/routes/A/sigjump.c`

A is C's file with `HERO_JMP_NO_DECODE` (562 lines): the record, the fingerprint read only of a buffer
inside the thread's stack, its current alternate stack (`sigaltstack(NULL)`, read at each such check),
or a writable `PT_LOAD` segment of an image loaded before any constructor (`dl_iterate_phdr` at
`.preinit_array`, 64 segments at most; under ASan `__asan_init` first, as `siglookup.c:122` does).
Anything else loses its record unread.

**Ran** (`ce255-A`, leg 1,154 s): the eight shapes **16 of 16 exit 0, clean** (2679 closed on Linux);
the `ij` cases refused, `ijstale` in UNFOLLOWED words; **`fixedbugs-2522` 25 passed, 0 failed**, every
other set as today, `runtime` 10 and 0 (census 353). **No case of the tree moves under A.** What A
refuses that today runs (the shapes beside, 3 runs each, plain and `--sanitize`, every one a correct
program, the callback 64 KiB below the landing frame so DEEPER than the dead handler's entry, the
depth programs' shape):

| correct program, a jump out of a handler then the callback deep | today | **A** | C | T |
|---|---|---|---|---|
| `x_static` (control) | runs | runs | runs | runs |
| `x_heap0`, a heap buffer, no save between | runs | runs | runs | runs |
| `x_heap`, a heap buffer, a save between | runs | **refused** | runs | runs |
| `x_tls`, `_Thread_local`, main thread | runs | **refused** | runs | runs |
| `x_dlopen`, a static of a library `dlopen`ed after `main` | runs | **refused** | runs | runs |
| `x_fake`, held by a frame that does not save (critic 8) | runs | runs plain, **refused under `--sanitize`** | runs | runs |
| `x_prot`, a page made `PROT_NONE` after the save, a save, readable again | **SEGV, 139** (2679) | **refused** | runs | runs |
| `x_realloc`, the holder moved by `realloc` | refused plain, **heap-use-after-free** under `--sanitize` (2679) | **refused** | runs | runs |
| `x_refill_out`, refilled by `memcpy` outside a handler from a deeper live frame | refused | **refused** | runs | runs |
| `x_dlclose`, a library's static saved into, the library `dlclose`d, a save | **SEGV, 139** (2679) | runs | runs | runs |

Critic 8's sanitizer shape is confirmed as the critic measured it: `x_fake`'s buffer is on ASan's fake
stack under `--sanitize` with the default options, so A refuses the sanitized build of a program whose
plain build it runs. A program's verdict would then depend on `--sanitize`, which the run harness's
four builds of every case would see as a red. **A is sound** (it reads nothing it cannot prove lives;
the two SEGVs and the heap-use-after-free of today are gone) and keeps every case of the tree; its
price is seven correct shapes refused beside ruling B's copy, and the residue `x_unseen0` (Heroes code
inside a signal) that today already carries.

## Q6. The same inputs under every route (today, C, Cfail = C's fallback, A, T), Linux arm64

Every row is 3 runs (the `ij` rows 5) at the program's default level unless it says otherwise, plain
and `--sanitize`, each container's own stage 2. Files `CE/p255/out/<route>/` (`shapes.log`, `ij.log`,
`extra.log`, `h-*.log`); the shapes beside are `CE/p255/extra/extra.h` (md5
`d628472b31c6d88e359a74f1d1b8d582` for every leg's `extra.log`; `x_partial` added after, run apart).
REF-S: refused in STANDING words; REF-U: in UNFOLLOWED words.

| input | today | A | Cfail | **C** | T |
|---|---|---|---|---|---|
| the eight shapes (16 runs) | 2 SEGV, 5 ASan | 16 clean | 16 clean | **16 clean** | 16 clean |
| `fixedbugs-2522` | 25 / 0 | 25 / 0 | 25 / 0 | **23 / 2** (refill: words; copy: runs) | 23 / 2 (the same two) |
| `-2558`, `-2571`, recovers, `-2494`, `-2257`, `-2583`, `-2496`, `-2503`, ignored, layer-stop | all pass | all pass | all pass | **all pass** | all pass |
| `runtime` suite | 10 / 0 (348) | 10 / 0 (353) | 10 / 0 (353) | **10 / 0 (353)** | 10 / 0 (349) |
| `ijn`, `ijv` 1/1f/2/3 (its nested SIGUSR2 shape included), `ijo`, `-O0`/`-O2` | REF-S | REF-S | REF-S | **REF-S** | REF-S |
| `ijstale` (MUST be refused) | REF-U | REF-U | REF-U | **REF-S** | REF-S |
| `x_static`, `x_heap0`, `x_dlclose` (MUST run) | static, heap0 run; **dlclose SEGV 139** | run | run | **run** | run |
| `x_heap`, `x_tls`, `x_dlopen`, `x_prot`, `x_realloc`, `x_refill_out` (MUST run) | heap, tls, dlopen run; **prot SEGV**; realloc, refill_out refused (**realloc heap-use-after-free** sanitized) | **all six refused** | all six refused | **all run** | all run |
| `x_fake` (MUST run) | runs | runs plain, **refused sanitized** | the same | **runs** | runs |
| `x_unseen` (MUST be refused) | REF-U | REF-U | REF-U | **REF-S** | **RUNS inside the signal** |
| `x_unseen0` (MUST be refused) | **runs inside the signal** | runs | runs | **REF-S** | runs |
| `x_partial` (MUST be refused; today, C, T only) | REF-U | | | **REF-S** | REF-S (arm64 copies the token with `__jmpbuf`) |
| third stack, m1 (MUST be refused) | runs 3 of 4 orientations | the same | the same | **the same** | the same |
| third stack, m2 / m3 | run / REF-U | run / REF-U | run / REF-U | **run / REF-S** | run / REF-S |

**The refill case stays refused under every route** (C and T in STANDING words, so its golden's words
move); **the copied-buffer case moves to running under C and T** and stays refused under A.

**libpng 1.6.48**, the issue's origin (`heroes-linux-arm64:gtk4-ui`, `CE/p255/png/`, `run-png.sh`;
each route's own stage 2 compiler copied out of its leg; `png.hero` writes a 64 x 48 PNG through
libpng (136 bytes), a copy cut to 68 bytes, then decodes the good, the cut (libpng's error path: its
`longjmp` to the `png_struct`'s buffer, the struct then freed) and the good again; prints `0 68 3072
-1 3072` when right), `-O0` and `-O2`, plain and `--sanitize`, 3 runs each, `CE/p255/out/png/png2.log`:

| libpng | today | C | A | T | Cfail |
|---|---|---|---|---|---|
| plain, `-O0` and `-O2` | `0 68 3072 -1 3072`, exit 0 | the same | the same | the same | the same |
| `--sanitize`, `-O0` and `-O2` | **exit 1, heap-use-after-free, `#0 hero_jmp_fingerprint sigjump.c:138`**, after `0 68`: the first decode's save read `png_make`'s freed write struct | **exit 0, clean, the whole output** | exit 0, clean | exit 0, clean | exit 0, clean |

So today a GTK program with no handler at all reports a use-after-free in the runtime under
`--sanitize` at its second PNG; under every route the program is clean.

## Q7. Routes B, D, E, F in assembly, and what else exists

**B, a record trusted where no save was seen since the outermost entry the jump would drop began.**
Unbuilt; judged by the shape I ran under T. Lane-core's case against it holds and I add the run that
shows it: `x_unseen` is exactly *"a refill inside the handler from a save the runtime never sees, where
a seen save stood between the buffer's save and the handler"*. Under B no save is SEEN after the
handler's entry began (the refill is glibc's own, by `dlvsym`), so B trusts the shallow record, drops
the live entry, and the Heroes function runs inside the signal, as it does under T (measured 3 of 3,
plain and `--sanitize`), where today, A and C refuse it. And B refuses every correct handler that saves
anything before jumping out. What would have to be true for B to beat C: that every save reaching a
buffer passes through this file. `x_unseen` shows it is false. **Veto, by reading of B and by the run
of its mechanism under T.**

**D, the releases told** (`free`, `munmap`, `dlclose`, `realloc` drop pending pairs inside them).
Unbuilt. It is incomplete by construction, and the shapes say where: `x_prot` gives nothing back and
still faults today's read (`mprotect` to `PROT_NONE`, SEGV 139 measured); a raw `munmap` system call, a
`madvise(MADV_DONTNEED)`, `shmdt`, an arena handing a block to another owner, a release on another
thread (which would have to write this thread's pairs, from outside, inside a lock-free table read by
handlers) all pass beside the names. Defining `free` and `realloc` in the runtime displaces
AddressSanitizer's interceptors as `sigjump.c:56` says a jump's definition displaces it, and makes every
program that links jemalloc, tcmalloc or mimalloc a `ffi_defined_elsewhere` refusal (panel 239's member,
`.claude/rules/c-boundary.md`). What would have to be true for D to beat C: every release in a process
passes through one of four names the runtime owns. **Object** (no soundness veto: a missed release is
2679 again, which is the reason to refuse it, but C closes 2679 without the premise).

**E, a read that cannot fault** (`process_vm_readv` on itself, a `write` into a pipe). Unbuilt. It
reads memory the program gave back, by a path that hides the fault, which is the sitting's own named
ground for a veto (`00-shared.md` § 6, *"a read of memory the program gave back"*); under
`--sanitize` either the sanitizer's `write` interceptor still sees the freed range (my recollection of
sanitizer_common, **unrun here**) or, by a raw system call, the use-after-free ASan exists to show is
hidden from it; Docker's default
seccomp profile refuses `process_vm_readv` (carried, unrun); a system call per pending pair per save.
What would have to be true for E to beat C: that reading another allocation's bytes is harmless once it
cannot fault. It does not need to be: C reads none. **Veto.**

**F in assembly** (a save point that is not a tail call yet leaves the caller's frame as the C library
must see it). Unbuilt; judged from the code of glibc's saves disassembled above. On arm64 a `bl` leaves
`sp`, but the stub must keep its caller's `x30` across the call somewhere the C library does not
restore from the buffer, and must make the SECOND return (a `longjmp`, which restores `x30` = the
stub's continuation from word 11) find the caller again: kept per thread it is overwritten by any later
save, kept per buffer address it is lost by a copied buffer (ruling B's case would then not be refused
but return to the wrong place: a crash), so the stub has to REWRITE word 11 with the caller's address
mangled by the key, which is C's calibration plus a write into the buffer. On x86-64 the stub must jump,
not call, so it must replace the caller's return address on the stack with its continuation for glibc's
`movq (%rsp)` to save it; under a CET shadow stack (Ubuntu 26.04's glibc is built for one:
`rdsspq`/`0x58` above) glibc's `ret` then faults with a control-protection exception. What F would buy
over C: a fingerprint taken at the save itself, i.e. today's R1 without 2679, which C already beats (the
copy followed, the unseen refill refused). What would have to be true for F to beat C: a C library whose
saved stack pointer cannot be decoded but whose return word can be rewritten, which glibc is not.
**Object**: more machinery than C on every save, and strictly no case where it is sounder.

**Routes I find**:
- **C3, C with the third stack told apart** (built, `CE/p255/routes/C3/`, below): C, plus
  `hero_sig_on_own`'s two answers made three (the thread's own stack, its CURRENT alternate stack by
  `sigaltstack(NULL)`, another), each entry keeping the class of the stack its wrapper ran on
  (`entry_stack`, `sigguard.c` +3 lines), `hero_sig_prune` dropping nothing for a target on another
  stack, and `hero_sig_inside` refusing a frame on another stack wherever an entry stands
  (`siginstall.c` +28 / -8). What would have to be true for it to be needed: a Heroes function reached
  from a `makecontext` context while a handler stands, which Q3 measured running in three orientations
  of four under every route.
- **Reading the pointer guard itself** (x86-64 `%fs:0x30`, arm64 glibc's `__pointer_chk_guard`,
  `GLIBC_PRIVATE`) instead of calibrating: less robust than C's calibration, since it trusts the slot
  and the transformation without verifying either. Not built.
- **C everywhere with no fallback** (*keep every entry* where the key does not verify): smaller (the
  save points, A, `jump_*` and four rows of `siglookup.c` gone) and less robust on the four
  architectures nobody measures. Recorded as what conservative-in-size would be.

## `00-shared.md` § 6's two questions

**May one program's verdict depend on the platform?** It already does, measured: today `plain_mmap`
(shape 8) is SEGV 139 on Darwin and exit 0 on Linux (§ 3's table). Under my composition it would
depend on the architecture inside Linux too: `x_heap`, `x_tls`, `x_dlopen`, `x_prot`, `x_realloc`,
`x_refill_out` and the copied-buffer case run where C's key verifies (arm64, x86-64, measured on glibc
2.41 and, for the probe, 2.43) and are refused where it does not (Cfail, i.e. powerpc64, riscv, arm,
s390x by reading). My answer: **a verdict may differ by platform only between *runs* and *a told
refusal*, never toward a crash or Heroes code inside a signal**, because §1.12 forbids the second and
the first is honest; the harness then owes a per-platform expectation or a skip naming the platform,
which it does not have (`platform_skips.hero`: Windows, `elsewhere`, `darwin_alone`). design.md says
nothing (the critic's grep, re-run by nobody since): **a question for the author**, with my
recommendation that the split be confined to platforms with no CI leg, i.e. that the Darwin seat's
decode, if it holds, make Darwin C as well.

**Does the route owe the spec a sentence?** Spec 427 and 428 lead a reader to expect that a program
which has LEFT the handler runs its callback. **C** refuses, of everything I ran on the CI's two Linux
architectures, no correct program a reader expects to run beyond what today already refuses for a jump
the runtime cannot see (a C++ exception, an inline `__builtin_longjmp`; unrun by me): it owes no new
sentence on those legs. **A** refuses six correct shapes today runs or should run (`x_heap`, `x_tls`,
`x_dlopen`, `x_prot`, `x_realloc`, `x_refill_out`) and one more under `--sanitize` alone (`x_fake`): it
owes the reader a sentence (*"a jump out of a handler is followed through a buffer on the thread's
stack or in the static storage of the program and the libraries it starts with"*), and so does C's
fallback on the four other architectures. The soundness lane has no seat to write or price it; the
synthesis should say so.

## C3: C with the third stack told apart, built and run (Linux arm64)

`CE/p255/routes/C3/`: C's `sigjump.c` unchanged, `siginstall.c` 362 -> 382 (+28 / -8:
`hero_sig_stack_of`, the three answers; `hero_sig_prune` and `hero_sig_inside` on them; the wrapper
storing each entry's stack), `sigguard.c` 523 -> 526 (+3: `entry_stack[HERO_SIG_DEPTH]`, 8 bytes a
thread). One `sigaltstack(NULL)` system call where an address is off the thread's own stack, in a
wrapper, a jump or a callback's check; async-signal-safe. Leg `ce255-C3`, stage 2 with C3's runtime
(`sources 452aaf564d7833c1`):

| | C | **C3** |
|---|---|---|
| the eight shapes | 16 clean | **16 clean** |
| every `x_` shape beside (`x_unseen`, `x_unseen0`, `x_partial` refused; the rest run) | as Q6 | **identical** (the rows' first 60 characters `diff`ed) |
| third stack m1 (MUST be refused), four orientations, plain and `--sanitize`, 3 runs | **runs in three of four** | **refused in all four**, STANDING words |
| third stack m2 (MUST run) | runs | **runs, all four** |
| third stack m3 (MUST be refused) | refused | refused, all four |

So C3 closes the residue today's header names for `makecontext`'s stack, for every orientation I
could build, with no measured cost on the other shapes. Its known cost, by reading and unrun: a signal
delivered while a coroutine runs on a third stack WITHOUT `SA_ONSTACK`, whose handler then jumps out to
a buffer on that same coroutine stack, leaves its entry standing (a third-stack target proves nothing
left), so a later callback from that coroutine is refused in STANDING words: a correct green-thread
program refused, where today compares the two addresses and follows it. Robustness ranks it: a refusal
told against a Heroes function running inside a signal. The case sets under C3 are below.

**C3's case sets** (leg 922 s): `fixedbugs-2522` **23 passed, 2 failed**, the same two as C (the refill
case refused in STANDING words, the copied-buffer case running); `-2558` 5, `-2571` 4, recovers 1,
`-2494` 15, `-2257` 3, `-2583` 6, `-2496` 1, `-2503` 1, ignored 1, layer-stop 3, every one 0 failed;
`runtime` 10 passed, 0 failed. No case of the tree moves beyond C's two.

## Verdicts (Linux; Darwin is the ffi-pragmatist's and I give it none)

### Route C, with A beneath it, and C3 with it: **approve** (Linux arm64 measured; Linux x86-64 below)

- `verdict`: **approve C3** (C, its fallback A, the third stack told apart); approve C alone if the
  sitting keeps the third stack apart from 2679, the third stack then owed as a `blocking` defect of
  its own (Heroes code inside a signal, measured in three orientations of four under today's runtime).
- `section`: design.md §1.12 (*"A Heroes program must not segfault and must not corrupt memory"*,
  line 648), and §1.1 / §1.7 for the ceiling (the compiler is untouched).
- `implementation_cost`: **runtime only, compiler 0 lines, spec 0 tokens.**
  `runtime/parts/sigjump.c` 283 -> 561 (+350 / -72: the calibration and its two assembly probes about
  190 with their comments, A's image table and liveness about 60); `runtime/parts/siginstall.c` 362 ->
  382 (+28 / -8) and `runtime/parts/sigguard.c` 523 -> 526 (+3) for C3. A landing splits `sigjump.c`
  past §11's ~300 (`sigjump.c` the names, a `sigdecode.c` the calibration, a `sigrecord.c` A). Tests:
  two expectations of `fixedbugs-2522` move (the refill case's words, the copied-buffer case to running,
  renamed), and the shapes owe `run` goldens: the eight of 2679, `x_*` and the third stack's twelve,
  libpng where `pkg-config` answers. The fallback owes a test that forces it (`Cfail`'s define), which
  no CI leg otherwise reaches: the runtime premise's test that fires when it dies (module-shape.md).
- `needed_for_self_hosting`: **no**.
- `argument`: C reads one word of the buffer the C library is about to read, verified before `main`
  against a probe that fills every register with a pattern, so no route reads less and none reads
  truer. Measured: 2679's eight shapes, `x_prot`, `x_dlclose`, `x_realloc` and libpng clean where today
  faults or reports; every correct shape I built runs, the copied buffer included; every MUST-refuse
  shape refused, including `x_unseen0`, which today runs inside the signal. Forced to fail it becomes A,
  every case as today. C3 adds the third stack, closed in four orientations.
- `prediction`: below.
- `condition`: a run where the calibration verifies and a jump lands elsewhere than decoded (the probe's
  end-to-end rows), or a correct program C refuses that today runs; or the Darwin seat's decode failing,
  which would leave the platform split of § 6 for the author.

### Route A alone: **object** (sound; approve it only as C's fallback)

- `section`: §1.12 (it reads nothing it cannot prove lives: no veto); spec 427 and 428 for what it owes.
- `implementation_cost`: `sigjump.c` 283 -> 562 as built (A is C's file with the decode off; alone,
  about 340), no compiler line.
- `argument`: A closes 2679 (16 of 16 shapes clean, the cases 25 of 25) but refuses correct programs
  today runs (`x_heap`, `x_tls`, `x_dlopen`) or ought to (`x_prot`, `x_realloc`, `x_refill_out`), and
  `x_fake` under `--sanitize` only (critic 8, confirmed): a verdict that depends on a build flag. It
  keeps `x_unseen0`, Heroes code inside the signal. It owes the spec a sentence.
- `condition`: C failing on a CI leg (then A is the floor, said in the spec).

### Route T: **veto**

- `section`: §1.12, and the sitting's veto ground, Heroes code running inside a signal (`00-shared.md`
  § 6).
- `implementation_cost`: `sigjump.c` 283 -> 226 (+55 / -112), no compiler line.
- `argument`: T reads no given-back memory and its write into the buffer is within the C library's
  license (Q4 e). But a save the runtime never sees leaves T's token in place: `x_unseen`, which today,
  A and C refuse, runs the Heroes function inside the signal under T, 3 of 3, plain and `--sanitize`,
  on arm64 and on x86-64; and on x86-64 a copy of `__jmpbuf` alone (`x_partial`) runs it inside the
  signal too, where C refuses. And the spans it rests on are glibc's own reserve: Ubuntu 26.04's glibc writes the shadow-stack
  pointer at 88 and the GCS pointer at 208, both inside spans measured *never written* here.
- `condition`: `x_unseen` refused under a T, which needs a fingerprint or a decode, i.e. C.

### B: **veto**, by reading, its mechanism run under T (`x_unseen`). D: **object** (incomplete:
`mprotect`, a raw `munmap`; collides with every allocator a program links). E: **veto** (reads memory
the program gave back, by a path that hides the fault). F in assembly: **object** (needs C's key and a
write into the buffer, would fault under a CET shadow stack on x86-64 by reading of Ubuntu's
`rdsspq`, sounder than C nowhere). B, D, E and F are unbuilt; each verdict says what it rests on.

## Prediction

**At the lane that lands C (or C3) on Linux, the CI's `ubuntu-26.04-arm` and `ubuntu-26.04` legs read
`run (only fixedbugs-2522): 25 passed, 0 failed` with exactly two of its expectations moved (the
refill case still refused, in STANDING words; the copied-buffer case running, `called 8`), every other
set of § 4 unchanged, and the eight 2679 shapes kept as `run` goldens exit 0 at `-O0`, `-O2` and both
sanitized on both legs.** Falsified, on either leg, by the refill case reading UNFOLLOWED words (the
calibration did not verify against Ubuntu's glibc 2.43 on the runner's CPU, and A ran instead), or by
any 2679 shape giving a sanitizer report or an exit other than 0. Checkable after the push that carries
the landing, on those two legs' `run` lines.

## Everything unrun, said

- **Darwin**, every route: the ffi-pragmatist's; I neither read nor ran it.
- **glibc's four other architectures** (powerpc64, riscv, arm, s390x): no machine; their verdict is
  Cfail's (A), by forcing the calibration to fail, not by a run there. Whether Docker's emulation runs
  `linux/riscv64` was not tried.
- **A CPU with a shadow stack or GCS enabled**: neither image runs one; what T or F would meet there is
  read from the disassembly, not run.
- **The `fixedbugs` sets on emulated x86-64** under any route: below where the leg reached them, else
  unrun.
- **The cost in time** of today's per-save fingerprint against C's branch: not timed (§ 8 of the shared
  brief: not owed); the machine ran four to six containers at once all afternoon.
- **A C++ exception or `__builtin_longjmp` out of a handler** under C: not built; by reading, as today.
- **musl**: no route reaches it (critic 3).
- **The green-thread cost of C3** (a handler run on a coroutine stack that jumps out within it):
  by reading only.
- **Lua**: absent from the images (critic 6); not run.

## Linux x86-64 (emulated, `heroes-linux:latest`, Debian glibc 2.41): C and T

Legs `ce255-x86-C`, `ce255-x86-T`: stage 2 built in each with its route's runtime, **1,540 s** each
under emulation beside the arm64 legs (C `sources 0d61aefe7224cd4b`, the arm64 C's key, so the same
tree; T `68ec3ffe375e58f0`). Files `CE/p255/out/x86-{C,T}/`.

| x86-64, 3 runs each, plain and `--sanitize` | C | T |
|---|---|---|
| the eight shapes | **16 of 16 exit 0, clean** | 16 of 16 exit 0, clean |
| `x_static`, `x_heap` (MUST run) | run | run |
| `x_unseen` (MUST be refused) | **refused, STANDING** | **RUNS: `answer ran for 5`, exit 0, inside the signal** |
| `x_partial` (MUST be refused; only `__jmpbuf`, 64 bytes, copied) | **refused, STANDING** | **RUNS: `answer ran for 6`, exit 0, inside the signal** |
| `ijn`, `ijv` 1/1f/2/3, `ijo`, `-O0` and `-O2`, 5 runs each | refused, STANDING, 5 of 5 each | `ijn`, `ijv` 1 refused (the leg was stopped there) |
| `ijstale` `-O0` and `-O2` | **refused, STANDING**, 5 of 5 each | unrun |

**The x86-64 calibration verifies** (the decode ran: `x_partial` and `x_unseen` are refused in
STANDING words, which only the decode gives for a buffer whose bytes changed; under A they would read
UNFOLLOWED). And **T fails twice on x86-64**, the partial copy predicted in Q4 (b) included: the token
at 96 lies past the 64 bytes of `__jmpbuf`, so a copy of the registers alone keeps the old token and T
drops the handler's live entry. `extra.h` md5 `f4f7ece3a365181fad6c920e700942cc` (the arm64 one plus
`x_partial`).

**C's case sets on emulated x86-64** (`ce255-x86-C`, its stage 2, `SETS` four of them, two at a time,
started 17:59 beside the other legs at a load of 20 to 47): `run (only fixedbugs-2571): 4 passed, 0
failed`, `run (only fixedbugs-2558): 5 passed, 0 failed`, `run (only
fixedbugs-a-c-handler-that-recovers-leaves-no-line): 1 passed, 0 failed`, each exit 0
(`CE/p255/out/x86-C/h-*.log`). `fixedbugs-2522` was still running at 18:50, about one case in three
minutes under emulation: its line follows if it ends, else it is unrun on x86-64.

*(18:53)* `fixedbugs-2522` on emulated x86-64 was at its ninth case of 25 after 54 minutes, about two
minutes a case on a Mac at load 20 to 47 that other seats share; I stopped it there, so **its count
line is unrun on x86-64** and the first judge of it is the CI's `ubuntu-26.04` leg after the push.
Its decisive shapes ran there all the same: `ijstale` and `x_partial` and `x_unseen` refused under C
in STANDING words, `x_static` and `x_heap` running. Every container I started is removed
(`docker ps` names no `ce255-`).


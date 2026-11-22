# Defect 2679: a panel question on panel 251's R1 (lane-core, 2026-11-22)

**The question.** Panel 251's R1 validates each save point's record by a fingerprint of the buffer's
bytes "once the C library has filled it". The save reaches the C library by a tail call, so the first
moment the runtime can read the filled buffer is this thread's NEXT save point or jump, and by then the
program may have given the buffer's memory back. No repair inside R1's mechanism both reads only memory
it can prove lives and keeps today's set of followed jumps; each robust route changes what R1 ratified
or what the author's ruling B of 2026-11-20 accepted as a known cost. Which route lands?

## The reproducer and what it does, measured

The issue's ten lines (`heapjmp.h`: `setjmp(*heap)`, `free(heap)`, `setjmp(local)`), and three shapes
beside it, built with the lane's compiler (`sources 41f5aa6448503b40`; the runtime is the branch's,
unchanged by this lane) on Linux arm64 (Docker `heroes-linux-arm64`, glibc 2.41, clang 22.1.8) and on
this Mac. Files: `.claude/worktrees/lane-core-scratch/2679/` (`repro.sh`, `shapes.h`, `mac/run.sh`; logs
`linux-arm64.log`, `mac/mac.log`).

| shape | Linux arm64 plain | Linux arm64 `--sanitize` | Mac plain | Mac `--sanitize` |
|---|---|---|---|---|
| heap buffer freed, then a save (the issue's) | `saved: 0`, exit 0 | heap-use-after-free, `sigjump.c:138` | `saved: 0`, exit 0 | heap-use-after-free, `rebind.c:334` (`hero_rebind_settle`) |
| a page `mmap`ed, saved into, `munmap`ed, then a save | **exit 139, SIGSEGV** | SEGV in `hero_jmp_fingerprint` | **exit 139, SIGSEGV** | SEGV in `hero_rebind_fingerprint` |
| a heap `ucontext_t` filled by `getcontext`, freed, then a save | exit 0 | heap-use-after-free, 4,560-byte region | not built (Darwin's `ucontext` is deprecated) | |
| a stack buffer whose frame returned, then a save in another frame | exit 0 | clean, also with `detect_stack_use_after_return=1` | not built | |

So the defect is wider than a sanitizer's report: a correct C program that unmaps memory it once saved
into **segfaults inside the runtime on both platforms with no sanitizer at all** (design.md §1.12), and
both arms carry it, `parts/sigjump.c` (Linux and every POSIX but Darwin) and `parts/rebind.c` (Darwin,
one pending buffer instead of a table, the same read). The shapes in the world: libpng and libjpeg save
into the struct they later free (GTK's icons, the issue's origin), coroutine and green-thread libraries
`getcontext`/`swapcontext` into contexts they free or unmap with their stacks, a pool, `dlclose` of a
library with a static `jmp_buf`.

## Why R1's mechanism cannot be repaired in place

The fingerprint exists to catch a REFILL: a buffer saved at a shallow frame, then overwritten inside a
handler by `memcpy` from a buffer the handler saved, and jumped to (`ijstale.c`, the case
`fixedbugs-2522-a-buffer-a-handler-refills-by-memcpy-and-jumps-to-is-refused`). It must therefore be
taken after the fill and before any refill; the runtime sees no moment in between but later save points
and jumps, where a heap buffer's life is unknowable (no allocator answers "is this freed", and reading it
anyway is the defect: freed pages may be unmapped, and under Arm MTE heap tagging a freed granule's read
faults even when mapped, by MTE's definition, unrun here). Reading through a channel that cannot fault (`process_vm_readv` on itself, a
pipe) hides the read rather than removing it, costs a system call per save, and Docker's default seccomp
profile refuses `process_vm_readv` (by its published profile, unrun here): not a candidate.

The issue's suggested shape, a fingerprint only of a buffer on the thread's own stack or its alternate
stack, would also take the record from every STATIC buffer, and the canonical correct program keeps its
`sigjmp_buf` static: all twelve depth cases of defect 2522 do (`static sigjmp_buf depth_sig_env`,
`fixedbugs-2522-a-jump-out-of-a-handler-then-a-correct-callback.h`), so each would be refused.

## The options, each with its cost

**A. A fingerprint read only where the memory provably lives, no record elsewhere** (inside R1's letter).
Proven: the thread's stack (`HeroSignalStack.lo`/`hi`, already measured at the claim), its alternate
stack, and the writable segments of the images loaded before `main` (Linux: `dl_iterate_phdr` in the
constructor, never in a handler since it locks; Darwin: the add-image callback `rebind.c` already has).
Anything else has no record, so its jump keeps every entry: R1's told refusal. Costs: a CORRECT jump out
of a handler through a buffer on the heap, in thread-local storage (`static __thread sigjmp_buf`, how a
library probing faults on several threads would keep one; how common is unmeasured), or in the statics of a library `dlopen`ed later, is refused,
a new known cost beyond ruling B's (copies); new premises about mappings, each to be pinned by a test
(stack bounds, image segments, MTE stack tagging absent); stale stack bytes read under ASan's redzones
need the fingerprint exempted from the sanitizer, as a machine read of a mapped stack. No existing case
moves, by reading: every 2522, 2558 and 2571 header keeps its buffers static or on the stack.

**B. No read at all: a record trusted only where no save point was seen since the outermost entry the
jump would drop began** (a generation count per thread). A refill that would drop a live entry needs a
source saved inside the handler, so a save seen there distrusts the record. Costs: a correct handler that
saves anything and then jumps out is refused; and it WIDENS the residue on the dangerous side: a refill
inside the handler from a save the runtime never sees (an inline `__builtin_setjmp` on x86-64, the C
library's own) is followed into Heroes code inside the signal even where a seen save stood between the
buffer's save and the handler, which today's fingerprint catches. Not recommended.

**C. Record-free on Linux: the jump reads its target's stack pointer out of the buffer it jumps to**
(ThreadSanitizer's `ExtractLongJmpSp`, prior art R1 itself cites; the compiler-engineer's stated
preference in panel 251, `251-reports/compiler-engineer.md`, its condition: "if a seat builds it and it
refuses the stale and memcpy-within shapes without refusing the memcpy-out one"). At a jump the buffer
lives (the C library reads it next), so the runtime reads one word of it and nothing else, demangles it
by a key calibrated before `main` from a save at a known stack pointer, and drops the entries deeper than
it by `hero_sig_prune`'s rule. A context's stack pointer is a public field of `ucontext_t`, never
mangled. Measured today: glibc 2.41 keeps it in one word under one key across two frames, at `-O0` and
`-O2`: slot 13 (`JB_SP`) by `xor` on arm64, slot 6 (`JB_RSP`) by `ror 17` then `xor` on x86-64
(`probe_mangle.c`, `linux-arm64.log`, `linux-x86-probe.log`); a second slot coincided once on each, so a
calibration verifies a known slot per C library and architecture and never searches. A calibration that
does not verify leaves every jump inside a handler keeping every entry, today's miss rule, loud. What it
gives: 2679 closes on Linux with no read of any buffer but the target; defect 2522's row 1 closes (a
correct jump out through a copied buffer is followed, the copied-buffer case flips from refused to
running, a change to ruling B's known cost); a refill by a save never seen is read as its true target;
the save points would need no wrapper on Linux, removing the per-save fingerprinting of up to 32 pending
buffers of 312 bytes each on arm64 (unmeasured in time). Its premise: the C library's `jmp_buf` layout
and mangling, verified at calibration; musl (unmangled, the same slots by its source) is unrun here.
Darwin cannot take it as measured: its `setjmp` mangles the stack pointer with a cookie and signs it
(panel 251's ffi-pragmatist, `dyld_info -disassemble`), so Darwin's arm needs A (images from the
add-image callback) or a decode that strips the signature first (`xpacd`), unrun.

## Recommendation, with its reason

**C on Linux, with A on Darwin until a seat builds Darwin's decode**: C is the one route that reads no
memory the program may have given back AND refuses no correct program today's record follows; its one
premise is checked before `main` and fails toward the told refusal. A is the robust floor where C cannot
run. What conservative would be: A everywhere, its new known costs (heap, thread-local and late-image
buffers jumped out of a handler) put to the author as ruling B's were. B is recorded and not recommended.

Unrun here: musl; the x86-64 shapes of the reproducer (only the probe ran there); the existing 2522
cases under A or C (their verdicts above are by reading); the cost in time of today's per-save
fingerprinting.

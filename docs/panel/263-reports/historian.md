# Panel 263, the historian's report

Written as it went, 2026-11-27 from about 11:20, by the historian seat, from the briefs as repaired at 10:58-11:15
(`263-briefs/historian.md`, `263-briefs/00-shared.md`) and the critic's first reading (`263-reports/critic-briefs.md`).
**Every page below was read on 2026-11-27** by WebFetch or WebSearch from this seat. Nothing was run, no build, no
container, no paid run, no commit, no GitHub write, no file written but this report.

**One thing went past the brief's "download nothing"**, and it is said here first: fetching the arXiv PDF of the OCaml 5
paper (https://arxiv.org/pdf/2104.00250), the fetch tool itself saved the binary to
`/Users/joseph/.claude/projects/-Users-joseph-Temp-heroes-heroes-lang/9d1670cb-db5d-4975-9d5b-1db104217e45/tool-results/webfetch-1795775034686-wvcqej.pdf`,
outside the repository's root. I did not ask for it, did not open it, and read the paper from its HTML rendering
instead (§ 6.7). This seat has no tool to delete it; it is the coordinator's to remove.

**The discipline of each line.** A precedent ends **verified** (its page read this day, at most one quotation under 15
words, the rest paraphrased, the URL given) or **unverified**. Each line says what its page states and nothing of what
Heroes should do; that reading is § 11's alone. Where a page was reached only through a search engine's summary and
could not be opened, the line says so and stays unverified. Code identifiers and numbers in backticks are the page's
names, not quotations of its prose.

## 0. Where the brief's recall was wrong or silent (first, as `00-shared.md` § 8 asks)

1. **GNU Pth does not schedule from `SIGALRM`** (the brief's item 8, *"as recalled"*): its manual says it is
   non-preemptive and that its design avoids `SIGVTALRM` (§ 8.4). The library that does preempt by a signal, and
   refuses to switch inside the handler, is Go since 1.14 (§ 8.6).
2. **cgo is both (b) and (c)**, as the critic read it, **and its thunk's own frames run on the C stack under a bound of
   their own**: an assumed 32 KiB below `SP`, checked at each runtime function's entry (§ 1.1, § 1.6). Panel 259's
   carried sentence on issue 62440 is right about the mend and silent on its cost (§ 1.3 to § 1.6).
3. **Go 1.3's notes say "hot spot"**, not "hot split"; Go 1.4's notes and the design document say "hot split" (§ 2).
4. **Rust's main-thread change** is verified with its reason (issue 43052, the kernel's guard gap on 64 KB-page Linux
   after Stack Clash), **but not its release** (§ 3.2, § 3.3).
5. **OpenSSL 4.1.0's default fibre**, the critic's item 11: the source at the `openssl-4.1.0` tag allocates 32,768
   bytes with `OPENSSL_malloc` and no guard, as at `openssl-3.5.0` and on `master` (§ 7.1). This is the source, not
   this Mac's binary: **where the `_mmap` and `_mprotect` imports of the dylib come from is unverified**.
6. **Lua's counter has a history the brief did not carry**: 5.4.0 and 5.4.1 budgeted the C stack in units of their own
   (`LUAI_MAXCSTACK`, `lua_setcstacklimit`), and 5.4.2 removed it and went back to a count of 200 (§ 6.3).
7. **Not in the brief, and nearest to the defect's own shape**: CPython 3.14 checks the stack pointer against the
   thread's bounds and, by design, **does nothing when the pointer lies outside them**, because user-space threads move
   the C stack (§ 6.4); Wasmtime's limit **does not ensure the space exists on the calling thread's stack** (§ 6.11);
   HotSpot **places its guard zones inside a native thread's stack when the thread attaches** (§ 6.5).
8. **The brief's item 4** asked what the compilers say of `-fsplit-stack` on arm64: GCC's page says x86 GNU/Linux
   only; LLVM's page names no target but by its x86-64 example; **no page I read says arm64 lacks it** (§ 4).
9. **The brief's item 6 on ASan's fibre annotations**: the header tells the sanitizer a stack's bounds; it says nothing
   of detecting an overflow on that stack (§ 6.10).

## 1. cgo: Go called from C

**1.1 The callback's path.** `runtime/cgocall.go`'s head comment (https://go.dev/src/runtime/cgocall.go): a Go call into
C runs the C function on the M's `g0` system stack, switched to by `asmcgocall`. A call from C back into Go enters
`runtime.cgocallback`, which saves the current system stack pointer and *"switches from m.g0's stack to the original g
(m.curg)'s stack"* (lines 51-52); `cgocallbackg` then runs on that goroutine stack, not on a `g0` stack, calls
`exitsyscall`, and may block until the `GOMAXPROCS` limit lets the goroutine run. **The callback's Go code runs on a
goroutine stack the Go runtime allocated.** **verified**, read 2026-11-27.

**1.2 A thread Go did not create.** `runtime/proc.go` (https://go.dev/src/runtime/proc.go, the first 100,000 characters):
*"needm is called when a cgo callback happens on a thread without an m"*. It takes an M from a list of extra Ms whose
`g0` and `curg` serve as the callback's scheduling stack and goroutine, and may call only `nosplit` functions. The
comment does not say in words on which stack `needm`'s own frames lie. **verified** for those statements, read
2026-11-27.

**1.3 golang/go issue 62440** (https://github.com/golang/go/issues/62440): *"runtime: "fatal: morestack on g0" on amd64
after upgrade to Go 1.21"*, opened 2023-09-04. A Go shared object loaded as a Fluent Bit plugin worked on Go 1.20 and
crashed on 1.21; the reporter bisected it to `ef0dedc` (CL 495855). Fluent Bit uses C coroutines, and a maintainer wrote
that the coroutine library appears to do nothing to update pthread's view of the stack. The closing commit is titled
"runtime: always update stack bounds on cgocallback" (`1ffc296`, milestone Go1.23, closed 2024-05-16), backported to the
go1.21 branch (`ac14d4d`) and the go1.22 branch (`3560cf0`) on 2024-06-24. **verified**, read 2026-11-27.

**1.4 The 1.21 cherry-pick** (Michael Pratt; https://fuchsia.googlesource.com/third_party/go/+/491c1e7e951bd6fc6ba37ef91be45ca14654eb05):
it combines CLs 527715, 527775, 527797 and 529216, for issues 62440, 62130 and 63209. Since CL 495855, Ms were cached
for C threads calling into Go together with the system stack's bounds; *"Some C libraries (e.g., coroutine libraries) do
manual stack management"* and may change stacks between calls into Go on one thread, so the change lets those bounds be
updated, at initial setup in `needm` and in `cgocallbackg`. Its stated price: systems that do not move the stack may not
notice unintentional stack corruption as quickly. It says *system stack* and does not name `g0`. **verified**, read
2026-11-27.

**1.5 The `dropm` follow-up** (https://fuchsia.googlesource.com/third_party/go/+/7e34c4308f1f48f3d817779797729c75b695f492),
titled *"runtime: clear g0 stack bounds in dropm"*, for 62440, fixing 63209: the bounds update did nothing when `SP`
already lay inside the old bounds, so bounds left by an earlier thread's use of an extra M survived; a new thread's
C-allocated stack partly overlapping the old one, with `SP` near the stale lower bound, overflowed and died with
`morestack on g0`. `dropm` now clears `g0`'s bounds when it returns an M. **verified**, read 2026-11-27.

**1.6 What the refresh cost, and how the thunk's frames are bounded today.** golang/go issue 68587
(https://github.com/golang/go/issues/68587), opened 2024-07-25 against go1.22.5: a test went from about 2 s to over
5.5 s, about 96.56% of the time in `callbackUpdateSystemStack`, traced to `3560cf0` / CL 585935, every callback
refreshing the bounds; the reporter found that *"In glibc, `pthread_getattr_np` ends up directly parsing
`/proc/self/maps`"*, seen for calls from the main thread. Fixed by CL 600296, "runtime: update and restore g0 stack
bounds at cgocallback" (`76a8409`, closed 2024-10-30, milestone Go1.24, backported to go1.23 by CL 635775); its benchmark
`CgoCallbackMainThread` went from 96.4 µs to 0.1 µs per operation. The code today (`cgocall.go`, lines 276-300): where
the bounds are not known, `g0.stack.hi = sp + 1024` and `g0.stack.lo = sp - 32*1024`, the comment assuming at least
32 kB; a pthread bound replaces them only if it contains `SP`, and is not asked from a signal handler. **verified**, read
2026-11-27 (both pages).

**1.7 Release dates** (https://go.dev/doc/devel/release): go1.21.0 2023-08-08, go1.22.0 2024-02-06, go1.23.0
2024-08-13, go1.24.0 2025-02-11. **verified**, read 2026-11-27.

**1.8 The `cmd/cgo` documentation** (https://pkg.go.dev/cmd/cgo) describes `//export` and says nothing of which stack
the exported function runs on, of threads Go did not create, or of C code that switches stacks. **verified** as a
silence of that page, read 2026-11-27.

**So, step by step, from the pages**: the C thread calls the exported function; `cgocallback` starts on `g0`, which for a
C thread is described by bounds the runtime sets *for a callback on sp* (the C stack in use, estimated as 32 KiB below
`SP` unless pthread's bounds contain `SP`); `needm` borrows an M on such a thread; `cgocallback` switches to the
goroutine stack (`m.curg`), and the Go code runs there under the entry check of § 2.1. **That `g0` on a C thread is the C
caller's own stack** is how 1.4 and 1.5 read ("C-allocated stack", "coroutine libraries"), not one sentence of
`cgocall.go`: **unverified as a single sentence**.

## 2. Go's move from segmented to contiguous stacks

**2.1 The entry check, today.** `runtime/stack.go`'s head comment (https://go.dev/src/runtime/stack.go): the guard is
set StackGuard bytes above the stack's bottom and *"Each function compares its stack pointer against g->stackguard to
check for overflow"*; a failing check calls `morestack`, whose `newstack` allocates a larger stack and moves the frames;
a frame under `StackSmall` may protrude below the guard, and the comment's sequence for a frame of `StackBig` or more
calls `morestack` directly; room is kept below the guard for a chain of `NOSPLIT` functions that never check
(`stackNosplit`); and storing `stackPreempt` in `stackguard0` forces the next check to fail, as a preemption request.
**verified**, read 2026-11-27.

**2.2 The design document** (https://docs.google.com/document/d/1wAaf1rYoM4S4gtnPh0zOlGzWtrZFQ5suE8qr2sD8uWQ/pub, linked
from Go 1.3's notes as `/s/contigstacks`): the hot split is that near a full segment *"a call will force a new stack
chunk to be allocated"*, freed on return, so a call in a loop pays both every time. The remedy: allocate a larger stack
(doubling), copy the old one, and adjust every word pointing into it, which needs a precise pointer map. Its numbers:
segmented 1.26 s without the split against 5.37 s with it; contiguous 1.26 s and 1.26 s; doubling about 10% faster on a
Peano benchmark, growth by 25% about 20% slower. **verified**, read 2026-11-27; **the page names no author or date, so
Keith Randall as its author is unverified**.

**2.3 Go 1.3** (https://go.dev/doc/go1.3), released 2014-06-18: goroutine stacks left the segmented model; a stack needing
more is *"transferred to a larger single block of memory"*; the notes name the "hot spot" problem. **verified**, read
2026-11-27.

**2.4 Go 1.4** (https://go.dev/doc/go1.4), released 2014-12-10: the starting goroutine stack went from 8,192 to 2,048
bytes; at the limit a larger stack is allocated and *"all active frames for the goroutine are copied there"*, pointers
updated; the notes name the "hot split" problem. **verified**, read 2026-11-27.

## 3. Rust: guard pages, stack probes, the main thread on Linux, segmented stacks

**3.1 The overflow handler** (https://doc.rust-lang.org/src/std/sys/pal/unix/stack_overflow.rs.html): on `SIGSEGV` or
`SIGBUS` with a nonzero `si_addr`, if the address lies in the current thread's recorded guard range it prints
*"thread '{name}' ({tid}) has overflowed its stack"* and aborts. Any other fault prints nothing: the handler resets the
signal to `SIG_DFL` and returns so that it is delivered again. For a non-main thread the guard range is recorded from
`pthread_getattr_np`, `pthread_attr_getguardsize` and `pthread_attr_getstack`; an alternate signal stack is made only
where none exists. **verified**, read 2026-11-27. **What a thread Rust did not spawn gets** is in files that page does
not hold: **unverified**.

**3.2 The main thread on Linux**, the same file's `install_main_guard_linux`: Linux grows the stack lazily and *"the
kernel has its own stack-guard mechanism"*; a guard Rust maps itself makes the kernel enforce a large gap above it,
wasting much of the stack (it cites #43052), so Rust only records where `rlimit` should fault, to say "stack overflow",
and trusts the kernel. On other platforms `install_main_guard_default` still remaps the last page. **verified**, read
2026-11-27.

**3.3 Issue 43052** (https://github.com/rust-lang/rust/issues/43052), opened 2017-07-04: Rust's main-thread guard
*"breaks nearly all Rust programs on 64KB-pagesize post-stack-clash Linux"*; the kernel's gap became 256 pages, 16 MB
on 64 KB pages, against an 8 MB `rlimit`, so the stack could not grow and the handler failed to recognise the fault.
Fixed by PR 43072, "Skip the main thread's manual stack guard on Linux", merged 2017-07-08; the same patch ships in
AlmaLinux's rust 1.19.0 package, Josh Stone, 2017-07-05
(https://git.almalinux.org/rpms/rust/src/commit/6d4fd15349b94383eb3376e6087d31f171c1c9da/rust-1.19.0-43072-stack-guard.patch).
**verified**, read 2026-11-27. **The first upstream release carrying it: unverified.**

**3.4 Stack probes.** PR 77885 (https://github.com/rust-lang/rust/pull/77885), "Use probe-stack=inline-asm in LLVM
11+", merged 2021-01-16, milestone 1.51.0, then reverted by PR 84708 for codegen breakage (issues 83139 and 84667), the
stable 1.52.0 carrying the beta revert (#84710). PR 102503 (https://github.com/rust-lang/rust/pull/102503), cuviper,
merged 2022-10-03, milestone 1.66.0: *"The known problems with x86 inline-asm stack probes have been solved"* on LLVM
16. PR 118491 (https://github.com/rust-lang/rust/pull/118491), "Enable stack probes on aarch64 for LLVM 18", merged
2023-12-14, milestone 1.76.0, its visible diff setting inline probes for Apple aarch64 targets; Windows aarch64 probes
through the external call its platform mandates. **verified**, read 2026-11-27. **Which aarch64 Linux target sets them,
from a page: unverified.**

**3.5 Segmented stacks abandoned.** Brian Anderson to rust-dev, 2013-11-04
(https://mail-archive.com/rust-dev@mozilla.org/msg06296.html): *"The new runtime does not implement segmented stacks and
it never will."* Reasons, the message's: the old runtime's experience that the trade-offs did not fit a high-performance
systems language; switching stacks costs; thrashing at a segment boundary, Go's hot split; foreign code expects large
stacks, so the FFI often needs a stack switch; libc and LLVM intrinsics force a large minimum segment. In their place:
the OS and MMU mapping pages lazily. **verified**, read 2026-11-27.

## 4. `-fsplit-stack`

**4.1 GCC's documentation** (https://gcc.gnu.org/onlinedocs/gcc/Instrumentation-Options.html): the option makes code that
splits the stack before it overflows, giving a discontiguous stack; the page says it is implemented only for x86
GNU/Linux targets; calls from split code into unsplit code may find little stack, which the gold linker (binutils 2.21
and later) can mend. It does not name `__morestack`. **verified**, read 2026-11-27.

**4.2 LLVM's documentation** (https://llvm.org/docs/SegmentedStacks.html): functions with the `"split-stack"` attribute;
the prologue keeps the limit in a TCB slot (*"use a slot in the TCB to store the current stack limit"*), compares it
with `SP` (`cmpq %fs:112, %r10` on x86-64) and calls `__morestack`; variable-sized allocas fall back to a libgcc heap
allocation freed with the stacklet. It names no supported target beyond its x86-64 example, and no AArch64.
**verified**, read 2026-11-27. **A sentence by LLVM or clang that arm64 lacks split stacks: not found, unverified**; the
clerk's measurement (both arm64 clangs accept the flag and emit no `__morestack`) stands against GCC's x86-only line and
LLVM's silence.

**4.3 Its users and its cost.** Ian Lance Taylor to the gcc list, 2016-04-10
(https://gcc.gnu.org/legacy-ml/gcc/2016-04/msg00066.html), on split stacks for aarch64: gccgo is, as far as he knows,
the only serious user, and *"The Go gc compiler now uses stack copying rather than stack splitting."* **verified**. Anders
Oleson, 2015-09-12, quoted in Keith Randall's reply (https://gcc.gnu.org/pipermail/gcc/2015-September/218889.html): *"It
is taking >4000 clocks per __morestack call"*, two `sigprocmask` calls each, about 60 clocks with the signal blocking
removed. **verified**. Taylor's answer, 2015-09-13 (https://gcc.gnu.org/legacy-ml/gcc/2015-09/msg00181.html): the
blocking is about *"a signal occurring while the stack is being split"*; gccgo avoids it by marking handlers
`SA_ONSTACK`, calling `sigaltstack` and `__splitstack_block_signals`. **verified**, all read 2026-11-27.

**4.4 Taylor's design page**, the GCC wiki's `SplitStacks`: it refused this client (an access-denied page), so its content
reached me only through a search summary (the limit in a TCB field reached through `%fs` or `%gs`; `__fnptr_morestack`
for calls through pointers). **unverified**.

## 5. `-fstack-check`, `-fstack-clash-protection`, `-fstack-limit-*`, and the Stack Clash

**5.1 GCC's documentation**, the page of § 4.1: `-fstack-check` makes code that lets the system see the stack being
extended, designed for Ada's need to detect infinite recursion and overflow, and is *"not generally sufficient to protect
against stack-clash attacks"*; `-fstack-clash-protection` allocates the stack one page at a time, touching each page, so
that no allocation jumps the OS's guard page, and the page says most targets do not fully support it. **verified**, read
2026-11-27.

**5.2 `-fstack-limit-register=` and `-fstack-limit-symbol=`**, the same page: code that keeps the stack from growing past
a limit held in a register or at a symbol's address; past it *"a signal is raised at run time"*, on most targets before
the boundary is overrun; a function opts out with the `no_stack_limit` attribute; the page says the setup may work only
with the GNU linker. **verified**, read 2026-11-27.

**5.3 The Stack Clash advisory**, Qualys, 2017-06-19 (https://www.qualys.com/2017/06/19/stack-clash/stack-clash.txt, its
first 100,000 characters; the date and the Qualys Research Team from
https://blog.qualys.com/vulnerabilities-threat-research/2017/06/19/the-stack-clash): if another region is mapped
directly below the stack, the stack pointer can move into it without a fault; *"a stack guard-page of a few kilobytes is
insufficient"*; the vanilla kernel's guard page was a hard-coded 4 KB; it recommends a guard gap of at least 1 MB as a
short-term fix a very large stack buffer defeats, and recompiling userland with `-fstack-check` as the long-term one.
**verified**, read 2026-11-27. **The kernel's own change (`stack_guard_gap`) from a kernel page: not read, unverified**;
§ 3.3's issue gives 256 pages.

**5.4 clang, 2020.** The LLVM blog (Serge Guelton, Sylvestre Ledru, Josh Stone;
https://blog.llvm.org/posts/2021-01-05-stack-clash-protection/): support *"landed in Clang in 2020, only for X86, SystemZ
and PowerPC"*; it gives no cost figure and reports no regression found in Firefox; it says inline probes would replace
`__rust_probestack` on i686 and x86_64. **verified**, read 2026-11-27.

**5.5 AArch64, LLVM 18.** Momchil Velikov, Arm's "What is new in LLVM 18", 2024-04-12
(https://developer.arm.com/community/arm-community-blogs/b/tools-software-ides-blog/posts/p2-whats-new-in-llvm-18): LLVM
18 *"now supports stack clash protection for AArch64 targets"*, probing in decreasing address order in 4 KiB chunks,
`-mstack-probe-size=N`; its example targets `aarch64-linux`, and it names no Darwin. **verified**, read 2026-11-27.

**5.6 Darwin.** D92245, Ed Maste, 2020-12-01 (https://lists.llvm.org/pipermail/llvm-commits/Week-of-Mon-20201130/857778.html),
*"-fstack-clash-protection: Return an actual error when used on unsupported OS"*; a reviewer notes the implementation is
inline code needing no runtime. **verified**. D40863, Amara Emerson, 2023-04-19
(https://lists.llvm.org/pipermail/llvm-commits/Week-of-Mon-20230417/1149056.html): *"For Darwin, we have this implemented
downstream in the xcode compiler on by default"*, kept downstream because it was tied to Darwin routines for fast stack
bounds checking, with the stated intention to upstream it. **verified**, read 2026-11-27; an intention, not a landing.
**Whether Apple's clang on arm64 probes by default today, and whether upstream clang accepts the flag for arm64 Darwin:
unverified**, a question a seat can run.

## 6. The others the brief recalled, and the ones found

**6.1 Rust's `stacker` crate** (docs, 0.1.25: https://docs.rs/stacker/latest/stacker/; source:
https://raw.githubusercontent.com/rust-lang/stacker/master/src/lib.rs). `maybe_grow` *"Grows the call stack if
necessary"*: it compares the remaining stack with a red zone and, below it, runs the closure on a newly allocated stack
(`grow`), otherwise on the current one; the docs' example switches to a 1 MB stack inside a 32 K red zone. The remaining
stack is `SP` minus a limit kept in a thread-local `STACK_LIMIT`, first guessed from the OS (`guess_os_stack_limit`,
whose body the file does not show) and set to the new stack's base when `stacker` switches to its own; where no guess
exists it grows at once; the new stack's allocation, per its comment, sets up guards where the target supports them.
**verified**, read 2026-11-27. **How the guess is made, and what it says on a stack C switched to: unverified.**

**6.2 Boost.Context's stack allocators** (https://www.boost.org/doc/libs/latest/libs/context/doc/html/context/stack.html
and its class pages). The `StackAllocator` concept: `allocate()` makes a stack, `deallocate()` frees one, and an
implementation may protect against exceeding the stack. `protected_fixedsize_stack` *"appends a guard page at the end of
each stack"*, reserved address space with no physical memory, and the page warns that starting a coroutine with a new
such stack is expensive; `fixedsize_stack` takes its memory from `std::malloc` and `std::free` and has no guard page,
with no warning given; `segmented_stack` needs gcc 4.7 or clang 3.4 and later, a build with `segmented-stacks=on`, and
works only with `callcc()` over `ucontext_t`. **verified**, read 2026-11-27. **Which allocator is the default: not on the
pages read, unverified.**

**6.3 Lua's C stack limit** (https://www.lua.org/source/5.4/llimits.h.html, Lua 5.4.9): `LUAI_MAXCCALLS`, 200, the
maximum depth of nested C calls, nested syntactic non-terminals and other recursion in C; the comment adds *"It must also
be compatible with the size of the C stack."* `ldo.c` (https://lua.org/source/5.4/ldo.c.html) raises `C stack overflow`
when the count reaches it. The 5.4.1 to 5.4.2 diff (https://www.lua.org/work/diffu-lua-5.4.1-lua-5.4.2.html) removes
`LUAI_MAXCSTACK` (2000), `lua_setcstacklimit` and `debug.setcstacklimit` (the manual had said the limit must be under
40000) and the `CSTACKERR` accounting, and adds `LUAI_MAXCCALLS` 200 and `luaE_checkcstack`; `lua_setcstacklimit`
becomes a stub returning `LUAI_MAXCCALLS`. Lua 5.4 was released 2020-06-29 (https://www.lua.org/versions.html).
**verified**, read 2026-11-27. **Roberto Ierusalimschy's reason** (a search summary: *reversing back to a stackless
implementation*) could not be opened (lua-l's archive looped on its redirect): **unverified**. **The release dates of
5.4.1 and 5.4.2: unverified.**

**6.4 CPython's recursion limits.**
- *3.12* (https://docs.python.org/3/whatsnew/3.12.html): *"The recursion limit now applies only to Python code."*;
  built-ins are protected by a different mechanism against crashing the virtual machine. **verified**, read
  2026-11-27.
- *The mechanism today*, CPython's `InternalDocs/stack_protection.md` (read at the Chromium mirror,
  https://chromium.googlesource.com/external/github.com/python/cpython/+/082c4bbfc2bfdac920a254fcb1b07ca12515f5e9/InternalDocs/stack_protection.md,
  GitHub answering 503): C-stack protection came in 3.12 as a counter and in 3.14 became the actual stack depth; on
  Windows, Mac and most Linuxes the OS is asked for the stack's bounds, elsewhere estimated; `_Py_EnterRecursiveCall`
  compares `SP` with a soft limit; between soft and hard limits it raises `RecursionError` (`Stack overflow (used … kB)`),
  past the hard limit a fatal error. **Because libraries with user-space threads change the C stack at run time, it
  raises only when `SP` lies between the expected stack base and the soft limit; below the known bottom the pseudocode
  reads *"Our stack limits could be wrong so it is safest to do nothing."*** **verified**, read 2026-11-27.
- *gh-130396* (https://github.com/python/cpython/issues/130396), Mark Shannon, 2025-02-21, "Implement stack overflow
  protection for linux based on actual stack depth": *"Linux doesn't offer an API for determining the current stack
  bounds"*, first a probe by `SIGSEGV` and `setjmp` was considered, then `pthread_get_stackaddr_np`; it broke two
  buildbots, crashed under ThreadSanitizer (mended in #130966), needed an Emscripten workaround (#131088); closed
  2025-12-24. **verified**, read 2026-11-27. **3.14 as the release that landed it is the internal document's word, not
  the issue's.** 3.14's notes (https://docs.python.org/3/whatsnew/3.14.html) remove `Py_C_RECURSION_LIMIT` and
  `PyThreadState.c_recursion_remaining`, added in 3.13, without deprecation (gh-133079). **verified**.

**6.5 HotSpot.**
- *Zones and banging*, `stackOverflow.hpp` (https://raw.githubusercontent.com/openjdk/jdk/master/src/hotspot/share/runtime/stackOverflow.hpp):
  at the stack's low end a red zone (access: unrecoverable, the VM crashes with a report), a yellow zone (access: a
  `StackOverflowError` is thrown and the zone's protection lifted while handlers run) and a reserved zone (lifted for a
  method annotated `@ReservedStackAccess` on the stack, else treated as yellow); overflow is found by touching the stack
  ahead of `SP` within a shadow zone, banged *"page by page at every Java/native method entry"* in the simplest form,
  with techniques to skip it. **verified**, read 2026-11-27.
- *A native thread attaching*, `jni.cpp`'s `attach_current_thread` (https://raw.githubusercontent.com/openjdk/jdk/master/src/hotspot/share/prims/jni.cpp,
  from character 100,000): `record_stack_base_and_size()`, then, under the comment *"// Enable stack overflow checks"*,
  `create_stack_guard_pages()` on that thread's stack; `jni_DetachCurrentThread`'s comment says `JavaThread::exit()`
  removes the guards. **verified**, read 2026-11-27.
- *What placing them does*, `stackOverflow.cpp` (https://raw.githubusercontent.com/openjdk/jdk/master/src/hotspot/share/runtime/stackOverflow.cpp):
  skipped for the primordial thread under `DisablePrimordialThreadGuardPages`; otherwise `os::guard_memory` over the
  zones; on failure it logs *"Attempt to protect stack guard pages failed"* and exits the VM out of memory; removal that
  fails only warns. **verified**, read 2026-11-27.
- *JEP 270* and the OpenJDK bug pages (JDK-6675312, on HotSpot's guard overlapping glibc's on attached threads;
  JDK-8170655) answered 403 to this client and reached me only through search summaries: **unverified**.

**6.6 JNI's `AttachCurrentThread`** (https://docs.oracle.com/en/java/javase/21/docs/specs/jni/invocation.html): a native
thread must attach before using the VM and then works like a Java thread inside a native method; it *"should have enough
stack space to perform a reasonable amount of work"*, the size being the OS's (`pthread_attr_t` with pthreads); it
detaches with `DetachCurrentThread`, which cannot happen with Java methods on its stack. The page says nothing of guard
pages or of the VM learning the stack's bounds. **verified**, read 2026-11-27.

**6.7 OCaml 5's fibres.** Sivaramakrishnan, Dolan, White, Kelly, Jaffer and Madhavapeddy, "Retrofitting Effect Handlers
onto OCaml", PLDI 2021 (read at https://ar5iv.labs.arxiv.org/html/2104.00250, its first 100,000 characters): fibres are
`malloc`'d on the C heap, their frame area initially 16 words; each OCaml function's prologue compares `SP` with a
threshold in `Caml_state`, and on overflow the whole fibre is copied to an area of double the size; leaf functions under a
16-word red zone skip it. C functions have no such checks, so *"we have to execute the external calls in the system
stack"*, switching `SP` at each call and back. On the first entry from C a fibre is allocated for the main OCaml stack;
later callbacks run on the current fibre, the stacks forming alternating OCaml and C segments. Its micro-benchmarks
against stock OCaml (Table 1, 10^8 calls of an identity function for the external call): external calls +17% time,
callbacks +65% time and +72% instructions. **verified**, read 2026-11-27.

**6.8 GHC's `foreign export`.** The FFI chapter (https://downloads.haskell.org/ghc/latest/docs/users_guide/exts/ffi.html):
calling a `foreign export` has much overhead, as *"it creates a complete new Haskell thread"*; a thread calling in takes
a little thread-local memory freed at `hs_exit()` or `hs_thread_done()`; the chapter says nothing of which stack the
Haskell code uses. The RTS options (https://downloads.haskell.org/ghc/latest/docs/users_guide/runtime_control.html):
thread stacks, the main thread's included, live on the heap, `-ki` 1k initial, grown in `-kc` 32k chunks with a `-kb` 1k
buffer copied across a boundary to avoid repeated overflow and underflow there, up to `-K` (default 80% of physical
memory), past which the thread receives the `StackOverflow` exception. **verified**, read 2026-11-27.

**6.9 Windows.** `CreateFiber` (https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-createfiber)
allocates a fibre object and *"assigns it a stack"*, `dwStackSize` being the initial commit. `SetThreadStackGuarantee`
(https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-setthreadstackguarantee)
sets the stack kept available while an overflow exception is handled; set for a fibre, it follows the fibre to any
thread. `_resetstkoflw` (https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/resetstkoflw): the stack grows
by a `PAGE_GUARD` page moved down a page at a time; at the maximum the guard is lifted and the exception raised, and if
`_resetstkoflw` does not restore it, the next overflow raises no exception and the process ends without warning.
**verified**, read 2026-11-27.

**6.10 AddressSanitizer.** The fibre annotations (https://raw.githubusercontent.com/llvm/llvm-project/main/compiler-rt/include/sanitizer/common_interface_defs.h):
before a switch the program calls `__sanitizer_start_switch_fiber` *"with a pointer to the bottom of the destination
stack and with its size"*, and `__sanitizer_finish_switch_fiber` once running there; the comments concern the fake stack
of use-after-return detection and say nothing of overflow detection. **verified**. The interceptors
(https://raw.githubusercontent.com/llvm/llvm-project/main/compiler-rt/lib/asan/asan_interceptors.cpp): `makecontext` is
intercepted to sign the context's stack; `swapcontext` reads the target context's stack and clears its shadow before and
after the switch, since the new context may share the current one's stack, and warns once that *"ASan doesn't fully
support makecontext/swapcontext functions and may produce false positives"*. **verified**, read 2026-11-27.

**6.11 Wasmtime** (found, not recalled). `Config::max_wasm_stack` (https://docs.rs/wasmtime/latest/wasmtime/struct.Config.html,
51.0.0, and its source, https://docs.wasmtime.dev/api/src/wasmtime/config.rs.html): the maximum stack for WebAssembly
code, 512 KiB by default; a wasm call chain past it raises a stack-overflow trap; host functions' stack counts toward it
but they are not stopped past it, and a host function past it that calls back into wasm traps at once; the limit *"does
not ensure that this much stack space is available"* on the calling thread's stack, and exhausting that stack typically
aborts the process; `async_stack_size` (2 MiB) is a separate stack per future, the host keeping `async_stack_size -
max_wasm_stack`. **verified**, read 2026-11-27. Issue 8135, cfallin, 2024-03-14, read through the project's Zulip
archive (https://bytecodealliance.github.io/zulip-archive/stream/225357-git-wasmtime/topic/wasmtime.20.2F.20issue.20.238135.20Investigate.20use.20of.20stack.20probes.20an.2E.2E.2E.html):
*"these checks are generated in the prologue of every Wasm function"*, at a cost on every call, the limit several
chained loads deep; when wasm uses almost all the stack and the host then overflows, it looks like a host crash; it
proposes a guard page with probes for frames larger than it, a `sigaltstack` handler, and an explicit check when wasm
returns to the host. **verified** for the archive's relay, read 2026-11-27; the issue's own page was not opened.

**6.12 V8** (found). `include/v8-isolate.h` (https://chromium.googlesource.com/v8/v8.git/+/main/include/v8-isolate.h):
`Isolate::SetStackLimit`'s parameter is *"An address beyond which the Vm's stack may not grow."*; with threads, the
embedder holds `v8::Locker` and sets a non-default limit separately for each thread. **verified**, read 2026-11-27.

**6.13 RethinkDB** (found). "Handling stack overflow on custom stacks", the RethinkDB Team, 2010-12-31
(https://rethinkdb.com/blog/handling-stack-overflow-on-custom-stacks/): coroutine stacks from `valloc`;
*"Memory-protect the page on the bottom of the stack"* with `mprotect(PROT_NONE)`; reset the protection before `free`
(the post gives no reason); a `SIGSEGV` handler with `SA_SIGINFO` on a `sigaltstack` maps the faulting address to a
coroutine's protected page and crashes with a message naming it; the alternate stack must be set per pthread; no cost
when nothing overflows. **verified**, read 2026-11-27.

## 7. The routes the repair added

**7.1 OpenSSL's door (route (o)).** The default fibre, `crypto/async/arch/async_posix.c`, read at the tags
`openssl-4.1.0` (https://raw.githubusercontent.com/openssl/openssl/openssl-4.1.0/crypto/async/arch/async_posix.c),
`openssl-3.5.0` and `master`: `STACKSIZE` 32768, the default allocator *`return OPENSSL_malloc(*num);`*, freed with
`OPENSSL_free`, no `mmap`, `mprotect` or guard in the file; `ASYNC_set_mem_functions` refuses once the first stack is
allocated. **verified**, read 2026-11-27. The POD (https://raw.githubusercontent.com/openssl/openssl/master/doc/man3/ASYNC_start_job.pod):
custom allocators are supported on POSIX, meant for example for `mmap` or the current thread's stack; the size defaults
to 32k and the callback may hand back another in `*num`; *"ASYNC_set_mem_functions(), ASYNC_get_mem_functions() were
added in OpenSSL 3.2."* **verified**, read 2026-11-27. **Homebrew's 4.1.0 build matching the tag's source, and the
origin of its `mmap` and `mprotect` imports: unverified.**

**7.2 A static bound (route (n)).**
- GCC (https://gcc.gnu.org/onlinedocs/gcc/Developer-Options.html): `-fstack-usage` writes each function's stack use with
  the qualifiers `static`, `dynamic` and `bounded`, the last meaning the number is *"an upper bound of the total amount of
  stack used by the function"*; `-fcallgraph-info=su,da` writes a per-object-file call graph in VCG decorated with it,
  none under `-flto`. **verified**, read 2026-11-27.
- clang: "Support GCC's -fstack-usage flag", Pengxuan Zheng, 2021-05-15
  (https://lists.llvm.org/pipermail/cfe-commits/Week-of-Mon-20210510/370151.html): *"-fstack-usage generates an extra .su
  file per input source file."* **verified**, read 2026-11-27. The GNAT guide's page
  (https://gcc.gnu.org/onlinedocs/gnat_ugn/Static-Stack-Usage-Analysis.html) says its `-fstack-usage` is not supported by
  the LLVM back end. **verified**.
- GNATstack, its guide 28.0w (https://docs.adacore.com/live/wave/gnatstack/html/gnatstack_ug/The_GNATstack_Tool.html):
  it computes the maximum stack of every entry point, tasks included; cycles are potential recursion and so potentially
  unbounded; it counts indirect and dispatching calls and lists reachable subprograms it knows nothing of; when it says a
  result is accurate *"the computed bound can never overflow"*, which needs no indirect calls, no variable-sized locals,
  no recursion and the whole environment. **verified**, read 2026-11-27.

**7.3 A hardware watchpoint (route (p)).** `perf_event_open(2)` (https://man7.org/linux/man-pages/man2/perf_event_open.2.html):
`PERF_TYPE_BREAKPOINT` with `HW_BREAKPOINT_W` counts writes to `bp_addr`, of length 1, 2, 4 or 8 bytes; since Linux 5.13
the `sigtrap` bit *"enables synchronous signal delivery of SIGTRAP on event overflow"*, with `sig_data` passed in
`si_perf`. The page does not say how many registers the hardware has. **verified**, read 2026-11-27. TI's Code Composer
page (https://software-dl.ti.com/ccs/esd/documents/ccs_checking_for_stack_overview.html) names the symbol on which to set
a watchpoint for the stack and points F28x users to a note on configuring watchpoints at run time without an emulator.
**verified** for the page; **the note itself (SPRA820, a PDF) was not opened: unverified**. **A user-space write
watchpoint on Darwin**: searched, nothing found that a process may set on its own threads; **unverified either way**.

**7.4 Copy-stack coroutines (shape C).** greenlet (https://raw.githubusercontent.com/python-greenlet/greenlet/master/src/greenlet/greenlet.cpp):
a greenlet is a range of C stack addresses whose data is partly in place and *"partly saved away in the heap"*, the
greenlets chained so that each points to the one owning the data above its `stack_stop`. **verified**, read 2026-11-27.
libco (https://raw.githubusercontent.com/Tencent/libco/master/co_routine.cpp): a coroutine's stack is
*`stack_mem->stack_buffer = (char*)malloc(stack_size);`*, no guard, 128 KB by default and at most 8 MB; a shared stack
is an array of such buffers; on a switch the occupying coroutine's live stack is `memcpy`'d to a `malloc`'d buffer and
the incoming one's copied back. **verified**, read 2026-11-27.

## 8. The secondary question: a handler switching contexts (step 199j's stop)

**8.1 POSIX, 2004** (Issue 6; https://pubs.opengroup.org/onlinepubs/009695399/functions/getcontext.html and
`makecontext.html`): a context may come from a signal handler's argument; a handler leaving by `longjmp` leaves unspecified
whether the `setjmp` context is restored, and *"Signal handlers should use siglongjmp() or setcontext() instead."*; Issue
6 marks `getcontext`, `setcontext`, `makecontext` and `swapcontext` obsolescent, because ISO C made function declarators
with empty parentheses obsolescent, and points to threads instead. Neither page says whether the functions are
async-signal-safe. **verified**, read 2026-11-27.

**8.2 Removed in 2008.** Linux's `getcontext(3)` (https://man7.org/linux/man-pages/man3/getcontext.3.html): *"POSIX.1-2008
removes these functions, citing portability issues"*; the sentence that a handler's context resumes after the
interrupted instruction was removed in SUSv2, the result now unspecified. `signal-safety(7)`
(https://man7.org/linux/man-pages/man7/signal-safety.7.html): none of the four is on the async-signal-safe list, and a
handler that interrupted an unsafe function and then calls an unsafe one is undefined. **verified**, read 2026-11-27;
**the 2008 edition's own page: not read, unverified**.

**8.3 Split stacks and a signal mid-switch** (§ 4.3): GCC's `__morestack` blocks signals so that a signal arriving while
the stack is being split is handled; gccgo instead runs every handler on `sigaltstack` and tells `__morestack` it need
not block. **verified**.

**8.4 GNU Pth** (its manual, Debian's copy, https://manpages.debian.org/jessie/libpth-dev/pth.3.en.html; gnu.org answered
429): Pth *"provides non-preemptive priority-based scheduling for multiple threads of execution"*, a design that avoids
signals such as `SIGVTALRM`; its scheduler does not block signals itself; `PTH_ATTR_STACK_SIZE` defaults to 64 KB, the
manual warning against less. **verified**, read 2026-11-27.

**8.5 Engelschall, "Portable Multithreading: The Signal Stack Trick For User-Space Thread Creation", USENIX 2000**
(https://www.usenix.org/legacy/event/usenix2000/general/full_papers/engelschall/engelschall_html/index.html): Pth makes a
new thread's machine context by declaring its stack the signal stack with `sigaltstack`, raising a signal, saving the
context with `setjmp` in the handler and returning at once, because leaving a handler's scope otherwise is restricted on
Unix; stacks are fixed in size, 16 to 32 KB advised, and overflow is detected by a red zone *"marked with a magic
value"*, which the scheduler checks between dispatches, no guard page. **verified**, read 2026-11-27.

**8.6 Go's asynchronous preemption, 1.14** (https://go.dev/doc/go1.14: goroutines are asynchronously preemptible on all
platforms but windows/arm, darwin/arm, js/wasm and plan9, programs receiving more signals and `EINTR`s;
https://go.dev/src/runtime/preempt.go): the signal handler does not switch; it injects a call to `asyncPreempt`, since
*"the scheduler itself is not designed to run from a signal handler"*; `isAsyncSafePoint` declines a non-user goroutine,
an M without a P, holding locks or allocating, too little stack, an unsafe point the compiler marked (atomic sequences,
`nosplit` functions), assembly, non-Go code, and the runtime itself. **verified**, read 2026-11-27.

## 9. The table

| precedent | where foreign or callback code runs | what bounds it | cost the page gives | what went wrong |
|---|---|---|---|---|
| cgo (§ 1) | Go code on a goroutine stack the runtime owns; thunk (`cgocallback`, `needm`) on `g0`, the C stack | entry check against `stackguard0`, grow by copying; `g0` checked against bounds refreshed per callback, 32 KiB below `SP` when unknown | refresh up to 96.4 µs a callback on glibc's main thread, 0.1 µs after CL 600296 | 1.21 cached C-thread bounds, broke on a C coroutine library (62440); stale bounds via extra Ms (63209); refresh cost (68587) |
| Go contiguous stacks (§ 2) | own goroutine stacks | prologue `SP` vs `stackguard0`, copy to double | hot split 5.37 s vs 1.26 s (segmented) | segmented stacks' hot split, abandoned in 1.3 |
| Rust std (§ 3.1-3.3) | own threads; main thread's OS stack | guard page (pthread's or remapped); on Linux main, the kernel's gap | none given | own main guard broke 64 KB-page Linux after Stack Clash (43052) |
| Rust probes (§ 3.4) | own frames | inline probes, a page at a time | none given | 1.51 inline probes reverted for codegen breakage, relanded 1.66 (x86), 1.76 (aarch64) |
| Rust segmented, 2013 (§ 3.5) | (removed) | (removed) | a cost of switching stacks, not measured there | FFI needs stack switches; thrashing |
| `-fsplit-stack` (§ 4) | segments from `__morestack` | prologue `SP` vs TCB limit | >4,000 clocks a call with `sigprocmask`, ~60 without | x86 GNU/Linux only (GCC); gccgo its only serious user |
| `-fstack-check`, `-fstack-clash-protection` (§ 5.1, 5.4-5.6) | own frames | probes, a page at a time | none given | `-fstack-check` not sufficient against Stack Clash; clang x86/SystemZ/PowerPC 2020, AArch64 2024 |
| `-fstack-limit-*` (§ 5.2) | own frames | `SP` vs a register or symbol, signal | none given | none recorded |
| Stack Clash (§ 5.3) | a region mapped directly below the stack | a 4 KB guard, insufficient | (an advisory) | `SP` jumps into the region with no fault |
| stacker (§ 6.1) | current stack, or a new one it allocates | `SP` minus a per-thread limit vs a red zone | none given | limit is a guess where the OS gives none |
| Boost.Context (§ 6.2) | stacks from an allocator the caller picks | guard page (`protected_fixedsize`) or none (`fixedsize`, `malloc`) | a guarded stack costly to start, no number | none recorded |
| Lua (§ 6.3) | the C stack | a count of nested C calls, 200 | none given | 5.4.0-5.4.1's C-stack budget removed in 5.4.2 |
| CPython 3.14 (§ 6.4) | the thread's C stack | `SP` vs soft and hard limits from the OS's bounds; nothing off the known stack | none given | buildbots, TSan, Emscripten (130396) |
| HotSpot, JNI attach (§ 6.5, 6.6) | the native thread's own stack | yellow, red, reserved zones placed in it at attach, removed at detach; banging at entry | none given | guard placement failure exits the VM; glibc overlap (unverified) |
| OCaml 5 (§ 6.7) | callbacks on an OCaml fibre (`malloc`'d); C on the system stack | prologue `SP` vs `Caml_state` limit, copy to double | callbacks +65% time, external calls +17% | none recorded |
| GHC (§ 6.8) | a new Haskell thread, stack on the heap in chunks | `-K` limit, `StackOverflow` exception | much overhead, no number | none recorded |
| Windows (§ 6.9) | fibre stacks the system assigns | `PAGE_GUARD` moving down; a guarantee for the handler | none given | after one overflow, no guard until `_resetstkoflw` |
| ASan (§ 6.10) | any stack, bounds told by annotation or read at `swapcontext` | (no overflow bound stated) | none given | false positives on `makecontext`/`swapcontext` |
| Wasmtime (§ 6.11) | the host's stack, or a per-future stack (async) | prologue check vs a limit; host counted | per call, the limit several loads deep | does not ensure the calling stack has the space; host overflow looks like a host crash |
| V8 (§ 6.12) | the embedder's thread | an address the embedder sets per thread | none given | none recorded |
| RethinkDB (§ 6.13) | its own `valloc`'d coroutine stacks | a `PROT_NONE` bottom page, reset before `free` | none when nothing overflows | none recorded |
| OpenSSL ASYNC (§ 7.1) | 32,768 bytes from `OPENSSL_malloc` unless a door (3.2+) replaces it | none in the default | none given | none recorded |
| GNATstack, `-fstack-usage` (§ 7.2) | (static) | a computed bound | none given | inaccurate with recursion, indirect calls, VLAs |
| `perf_event_open` (§ 7.3) | (a mechanism) | a write watchpoint of 1 to 8 bytes, `SIGTRAP` since 5.13 | none given | none recorded |
| greenlet, libco (§ 7.4) | one shared stack, copied in and out | none in libco (`malloc`) | none given | none recorded |
| Pth (§ 8.4, 8.5) | fixed stacks, 64 KB default | a magic red zone checked between dispatches | none given | none recorded |
| Go preemption (§ 8.6) | (a handler) | declines inside the runtime, assembly, unsafe points | more signals, `EINTR` | none recorded |

## 10. Which route each precedent is (`00-shared.md` § 4), and which none took

- **(a), a guard the runtime places inside a stack it did not allocate**: **HotSpot at `AttachCurrentThread`** (§ 6.5),
  on a native thread's own stack, removed at detach; Rust's `install_main_guard_default` on a main thread's OS stack
  (§ 3.2), **given up on Linux** after the kernel's gap made it harmful (§ 3.3). On stacks the program itself allocated:
  RethinkDB (§ 6.13), Boost's `protected_fixedsize_stack` (§ 6.2). **(a3)**, the size of what a frame can jump:
  `-fstack-clash-protection`, LLVM 18 on AArch64, Rust's inline probes, HotSpot's banging (§ 3.4, 5, 6.5).
- **(a4), `free` wrapped to lift the guard**: RethinkDB lifts it before its own `free` (§ 6.13); **no precedent found
  wrapping a foreign library's `free`** (searched: "intercept makecontext replace coroutine stack with guarded stack
  runtime interpose free munmap guard page mprotect user-allocated stack").
- **(b), callbacks on a stack the runtime owns**: **cgo** (§ 1), **OCaml 5** (§ 6.7), **GHC** (§ 6.8). In cgo the thunk's
  own frames stay on the C stack under a bound of their own (§ 1.6), the critic's item 6 in another language.
- **(c), a check at entry against a limit**: Go (§ 2.1), OCaml 5 (§ 6.7), CPython 3.14 (§ 6.4), Wasmtime (§ 6.11), V8
  (§ 6.12), GCC's `-fstack-limit-*` (§ 5.2), `-fsplit-stack` (§ 4), and cgo's `g0` (§ 1.6). **cgo is (b) and (c)
  together.**
- **(d), `sigaltstack`**: Rust, RethinkDB, gccgo use it for the handler (§ 3.1, 6.13, 4.3); **no page claims it stops a
  write**.
- **(e) with (g)**: `stacker` (§ 6.1). **(e) alone**: none found as a precedent of its own.
- **(f), refusing a callback on a stack the runtime cannot bound**: **none found**; CPython does the opposite, checking
  nothing off its known stack (§ 6.4), and Wasmtime does not ensure the calling stack's space (§ 6.11).
- **(h), the stack swapped at `makecontext`**: **none found**; ASan intercepts `makecontext` and `swapcontext` but only
  signs and reads the stack (§ 6.10).
- **(i), a guard below the block where the page is free**: **none found; not searched by a query of its own**.
- **(j), split stacks**: refused by Go in 1.3 (§ 2) and Rust in 2013 (§ 3.5); GCC's x86 GNU/Linux only (§ 4.1).
- **(k), a counter**: Lua (§ 6.3), CPython 3.12 (§ 6.4); **CPython left it for the stack pointer in 3.14**, and Lua's
  own comment ties its count to the C stack's size.
- **(l), (q), the runtime's words out of reach**: **none found; not searched by a query of its own**.
- **(m), a canary read later**: **Pth's red zone, checked between dispatches** (§ 8.5).
- **(n), a static bound**: GCC's and clang's `-fstack-usage`, GCC's `-fcallgraph-info`, GNATstack (§ 7.2).
- **(o), the library's door**: OpenSSL's `ASYNC_set_mem_functions` since 3.2 (§ 7.1), Boost's `StackAllocator` (§ 6.2).
- **(p), a watchpoint**: the Linux mechanism (§ 7.3) and TI's F28x use (unverified); **none found on Darwin**.
- **Step 199j's stop**: Go's preemption declines to act when interrupted inside the runtime rather than aborting
  (§ 8.6); gccgo and libgcc handle a signal arriving mid-split (§ 4.3); POSIX leaves the context functions off the
  async-signal-safe list and removed them in 2008 (§ 8.2). **No precedent found that aborts on a handler's switch naming
  a context the interrupted switch was itself using**.

## 11. The seat's report, in the agent file's structure

- **`verdict`**: **approve (advisory)** of **(b) with (c) together** as the route history has taken where a runtime is
  entered from C, and **object (advisory)** to **(k)** and **(j)** as the whole of any resolution.
- **`precedents`**: §§ 1 to 8 above, each marked; the table is § 9, the mapping § 10.
- **`argument`** (115 words): Runtimes entered from C mostly leave the caller's stack: cgo, OCaml 5 and GHC run the
  callback on a stack they own, growing under a limit they keep. Those staying on a foreign stack take the embedder's
  word (V8, JNI), put their own guard inside it at attach (HotSpot), or decline: CPython 3.14 checks nothing off its
  known stack; Wasmtime does not ensure the space exists. Go is (b)'s warning: its thunk's frames stay on the C stack,
  and caching their bounds broke on a C coroutine library in 1.21, then cost up to 96 µs a callback until a 32 KiB
  estimate. Counters (Lua, CPython 3.12) gave way to sizes, split stacks to copying.
- **`condition`**: a runtime that runs a callback's own code on a C library's `malloc`'d coroutine stack and stops a
  recursion there before it writes below the block, with no stack of its own and no protection inside C's memory, would
  move my reading toward (c) or (f) alone; so would a page showing HotSpot's attach guard, or Rust's remapped main guard,
  breaking on memory a C library later freed or reused, against (a); and so would a precedent of (h) or (a4) that
  shipped.
- **`search`**: § 12.

## 12. Search

Tools: WebSearch (standard mode) and WebFetch, from this seat only. **Queries made**: `"runtime: always update stack
bounds on cgocallback" golang commit`; `Rust std main thread guard page Linux removed kernel stack guard gap pull request`;
`"Abandoning segmented stacks in Rust" Brian Anderson rust-dev 2013`; `Rust inline stack probes replace __rust_probestack
LLVM probe-stack inline-asm x86 release`; `rust-lang pull "stack probes" AArch64 "inline-asm" LLVM 18 enable aarch64
linux`; `clang -fstack-clash-protection Darwin macOS arm64 unsupported "fstack-clash-protection"`; `Ian Lance Taylor split
stacks gccgo __morestack TCB stack limit prologue airs.com`; `LLVM segmented stacks supported targets x86 ARM
"split-stack" AArch64 not supported __morestack clang`; `Lua 5.4 lua_setcstacklimit deprecated 5.4.3 LUAI_MAXCSTACK "C
stack overflow" change`; `"stack overflow protection on supported platforms" CPython whatsnew gh-130396 PyOS_CheckStack
stack pointer`; `HotSpot stack banging yellow red reserved zone guard pages StackShadowPages JEP 270
ReservedStackAccess`; `HotSpot AttachCurrentThread native thread stack guard pages created "attached" thread yellow zone
pthread_getattr_np JDK bug`; `"Retrofitting effect handlers onto OCaml" PLDI 2021 C stack external calls callbacks fiber
stack overflow check function prologue`; `greenlet implementation copies stack slices to heap on switch "stack_copy"
stack_start shared C stack how it works`; `Tencent libco shared stack co_alloc_sharestack copy stack save_stack_buffer
stCoRoutineAttr_t share_stack`; `macOS arm64 user space set hardware watchpoint own process thread_set_state
ARM_DEBUG_STATE64 without debugger`; `clang -fstack-usage support added release notes LLVM 13 ".su" file`; `GNATstack
worst-case stack usage call graph recursion indirect calls cycles AdaCore documentation`; `Engelschall "Portable
Multithreading" "signal stack trick" user-space thread creation sigaltstack USENIX 2000`; `GNU Pth "non-preemptive"
"priority-based scheduling" manual pth(3) stack guard`; `V8 Isolate::SetStackLimit embedder threads "stack limit"
ResourceConstraints set_stack_limit documentation v8.h comment`; `wasmtime stack_limit set on entry from host
"max_wasm_stack" stack pointer minus prologue check cranelift "stack limit" VMStoreContext stack overflow detection`;
`Julia jl_adopt_thread foreign thread stack bounds stack overflow detection "foreign thread" @ccallable Julia 1.9 stack`
(nothing citable found, so Julia is not a precedent here); `hardware watchpoint used as stack overflow guard instead of
guard page debug register stack limit detection embedded RTOS`; `intercept makecontext replace coroutine stack with
guarded stack runtime interpose free munmap guard page mprotect user-allocated stack`.

**Pages read** (each with its URL above): Go's `cgocall.go`, `proc.go`, `stack.go`, `preempt.go`, release notes 1.3,
1.4, 1.14, the release history, issues 62440 and 68587, two Fuchsia mirrors of release-branch commits, the contiguous
stacks design document, `cmd/cgo`; Rust's `stack_overflow.rs`, issue 43052, PRs 77885, 102503, 118491, AlmaLinux's patch,
Anderson's rust-dev message; GCC's Instrumentation and Developer options, GNAT's static stack usage page, three gcc-list
messages; LLVM's SegmentedStacks, the LLVM stack-clash blog, Arm's LLVM 18 post, D92245, D40863, the clang
`-fstack-usage` commit; Qualys's advisory and post; stacker's docs and source; four Boost.Context pages; Lua's
`llimits.h`, `ldo.c`, the 5.4.1-5.4.2 diff, the versions page; CPython's 3.12 and 3.14 notes, `stack_protection.md`,
gh-130396; HotSpot's `stackOverflow.hpp`, `stackOverflow.cpp`, `jni.cpp`; the JNI invocation spec; the OCaml 5 paper
(HTML); GHC's FFI and RTS chapters; four Microsoft pages; compiler-rt's sanitizer header and ASan interceptors; Wasmtime's
`Config` docs and source, issue 8135's archive; V8's `v8-isolate.h`; RethinkDB's post; the `context` crate's
`sys/unix/mod.rs` (its `protect_stack` guards a stack its own `allocate_stack` made, so a search summary's *"a stack it
was given"* was wrong and the crate is not cited above); OpenSSL's `async_posix.c` at three refs and its POD;
GNATstack's guide; `perf_event_open(2)`, `getcontext(3)`, `signal-safety(7)`; TI's CCS page; greenlet's `greenlet.cpp`;
libco's `co_routine.cpp`; POSIX Issue 6's `getcontext` and `makecontext`; Pth's manual (Debian); Engelschall's paper
(HTML).

**Refused or unreadable, so their claims stay unverified**: the GCC wiki's `SplitStacks` (access denied); openjdk.org's
JEP 270, the hotspot-runtime list, `bugs.openjdk.org` (403); github.com's CPython and HotSpot pages (503, read through
mirrors or raw files instead); lua-l's archive (a redirect loop); gnu.org's Pth manual (429, read through Debian);
`docs.openssl.org` (empty or navigation only, read through the POD); the Linux kernel thread on per-task watchpoints
(403); the arXiv PDF (binary; see the head of this report).

**What was looked for and not found, with where**: a sentence by LLVM or clang that arm64 has no split stacks (LLVM's
SegmentedStacks page, the search above); a Darwin user-space write watchpoint (Apple's forums and XNU sources through the
search above); a runtime that swaps a foreign coroutine's stack at `makecontext`, or wraps a foreign `free` to lift a
guard (the interposition search above; ASan's interceptors read); a runtime that refuses a callback on an unboundable
stack (CPython's and Wasmtime's pages read; no query of its own); routes (i), (l) and (q) (no query of their own, so
their absence here says only that the pages read do not hold them). The author's book, *Heroes of code*, was not used as
a route for this sitting: every precedent here came from the queries above.

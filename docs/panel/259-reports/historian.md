# Panel 259, the historian's report

Written 2026-11-25 by the historian seat, as it went (stopped once by the usage limit, resumed at the coordinator's
word of 09:18). Tools: Read, Write, WebSearch, WebFetch; no shell, so **nothing here was run**. Every web page below
was read on **2026-11-25** through WebFetch, whose page reader returns quotations and summaries: a quotation in
double quotes and italics is the reader's verbatim excerpt; a line number from a web page is the reader's count and
marked *approx.*; a fact taken from a search engine's summary and not re-read on its page is marked **unverified**.
*Heroes of code* was not used for any finding here: none of this sitting's rooms (sanitizers, coroutine libraries,
runtimes under C) is one the book walks, so it served as no index this time.

## What I read and what I ran, apart

- **Ran**: nothing.
- **Read on this Mac**: `/opt/homebrew/opt/boost/include/boost/context/fiber_ucontext.hpp:140-179` (Boost 1.92.0_1,
  the version per the shared brief § 4.7). Under `#if defined(BOOST_USE_ASAN)` it calls
  `__sanitizer_start_switch_fiber( & from->fake_stack, stack_bottom, stack_size)` before `::swapcontext( & from->uctx,
  & uctx)` and `__sanitizer_finish_switch_fiber(...)` after (lines 154-170), and **under `#if defined (BOOST_USE_TSAN)`
  it calls `__tsan_switch_to_fiber(tsan_fiber, 0)` (lines 161-163)**, which the shared brief does not name: the
  `ucontext` backend tells ThreadSanitizer too.
- **Read on the web**: the pages under each question below, each with its URL.
- **Two facts of the briefs this report corrects**: (1) the historian's brief recalls that *"cgo's documentation
  forbids C from jumping over Go frames"*: the cgo document (https://pkg.go.dev/cmd/cgo) holds no such sentence (Q1);
  (2) the shared brief's Boost reading names ASan alone; the same header tells TSan (above).

## Q1. Go's runtime and cgo

- **verified**, golang/go issue 62440, https://github.com/golang/go/issues/62440: *"runtime: "fatal: morestack on g0"
  on amd64 after upgrade to Go 1.21"*, opened by VirrageS on Sep 4, 2023; the program worked with Go 1.20. The
  maintainer prattmic traced it to Fluent Bit's coroutine library: *"It looks like there is stack manipulation going
  on."*; *"After CL 495855, the M is initialized on the first callback with the stack at that time."*; the coroutine
  library *"doesn't appear to do anything to update pthread's view."* Closed *"as completed in 1ffc296"* (*"runtime:
  always update stack bounds on cgocallback"*) on May 16, 2024, milestone Go1.23, backported to 1.21 and 1.22 (CLs
  585337, 585935). A search summary said CL 525455 was reverted for breaking darwin, alpine and android:
  **unverified**, the issue page shows no revert.
- **verified**, golang/go issue 68587, https://github.com/golang/go/issues/68587: *"runtime: severe performance drop
  for cgo calls in go1.22.5"*, opened by zeroshade on Jul 25, 2024: the backport (commit 3560cf0afb, CL 585935) made
  every cgo call update the system stack bounds, and the runtime's `x_cgo_getstackbound` calls glibc's
  `pthread_getattr_np`, which on the main thread parses `/proc/self/maps`. Closed Oct 30, 2024 by commit 76a8409,
  *"runtime: update and restore g0 stack bounds at cgocallback"*, milestone Go1.24, backported to 1.23 (59b7d40, Dec
  13, 2024).
- **verified**, `runtime.callbackUpdateSystemStack`, https://go.dev/src/runtime/cgocall.go (tip on the day read):
  *"Set or reset the system stack bounds for a callback on sp."*; it updates where *"the current SP is not within the
  previous bounds (the stack may have"* [changed]; *"N.B. we need to update the stack bounds even if SP appears to
  already be in bounds, if our bounds are estimated dummy bounds"*; with no known size it assumes *"there's at least 32
  kB"*; of the C library's answer, *"Don't use these bounds if they don't contain SP. Perhaps we were called by
  something not using the standard thread"* [stack]; and *"Don't adjust if called from the signal handler."* The page
  reader found no `throw` in the function.
- **verified**, Go's own test of the shape, `src/runtime/testdata/testprogcgo/stackswitch.c` (*"Copyright 2023"*),
  https://go.googlesource.com/go/+/121b442c85b9cc8bfb8f37485937108d37ec93c1/src/runtime/testdata/testprogcgo/stackswitch.c:
  *"Next, verify that switching stacks doesn't break callbacks."*; *"since we used a custom stack with makecontext,
  callbackUpdateSystemStack had to guess the bounds. Its guess assumes a 32KiB stack."*
- **verified (an absence, with what was searched)**: https://pkg.go.dev/cmd/cgo: the page reader searched `longjmp`,
  `setjmp`, `swapcontext`, `makecontext`, `coroutine`, `stack switching`, `unwind`, `jump`, and found none in the
  document's text; the nearest rule is `#cgo nocallback` (*"If the nocallback directive is used and the C function
  does call back into Go code, the program will panic."*). A golang-nuts thread, *"cgo & long jump"*
  (https://groups.google.com/g/golang-nuts/c/ypdrRZbQdo8/m/4xSEz-QUCQAJ), is summarised by the search engine as Ian
  Lance Taylor saying C must not longjmp across Go frames: **unverified**, the thread was not read.
- **Answer: none of (a) to (e) exactly; a re-measure at each entry, then a guess.** Go is not told of a switch; at a
  callback it asks the C library for the thread's bounds, and where they do not hold SP it **guesses 32 KiB below
  SP**. The function holds no `throw` (the page reader's search), so a switch is not refused there; issue 62440's
  crash was a `fatal: morestack on g0`, not a told refusal. **What happened**: the bounds kept from a first callback
  broke a coroutine library in 1.21 (62440); refreshing them on every call (1.23 and backports) brought a reported
  slowdown (68587); the third repair (1.24) updates and restores them at each callback. **The rule it wrote down is in
  code and a test, not in its documentation.**

## Q2. Boost.Context

- **verified, the Windows switch saves and restores the thread block's four stack fields, unconditionally**:
  `src/asm/jump_x86_64_ms_pe_masm.asm`,
  https://raw.githubusercontent.com/boostorg/context/develop/src/asm/jump_x86_64_ms_pe_masm.asm (*"Copyright Oliver
  Kowalke 2009."*), branch `develop` on the day read. Save path, approx. lines 110-123: *"; load NT_TIB"* `mov r10,
  gs:[030h]`; *"; save fiber local storage"* `mov rax, [r10+020h]`; *"; save current deallocation stack"* `mov rax,
  [r10+01478h]`; *"; save current stack limit"* `mov rax, [r10+010h]`; *"; save current stack base"* `mov rax,
  [r10+08h]`. Restore path, approx. lines 162-174: the four written back, ending *"; restore current stack base"* `mov
  [r10+08h], rax`. The file's one conditional, `IFNDEF BOOST_USE_TSX`, guards the XMM and x87 saves, **not** these
  lines. The GNU-assembler twin, https://raw.githubusercontent.com/boostorg/context/develop/src/asm/jump_x86_64_ms_pe_gas.asm
  (*"Copyright Thomas Sailer 2013."* beside Kowalke's), does the same: `movq %gs:(0x30), %r10` *"/* load NT_TIB */"*,
  `movq 0x08(%r10), %rax` *"/* save current stack base */"*, the restores outside its `#if !defined(BOOST_USE_TSX)`.
  **Caveats**: read at `develop`, not at the 1.92.0 tag this Mac holds (**that the tag's file is the same is
  unverified**); and what values a NEW context restores depends on what `make_fcontext` writes into those slots, which
  I did not read for Windows (**unverified**). So Boost's Windows switch **rewrites the four fields the thread block
  names a stack by**, the fields `stack_lane.c:89-100` reads; that they then name the coroutine's own stack is an
  inference, unrun on Windows.
- **verified**, Boost.Context's sanitizer page,
  https://www.boost.org/doc/libs/latest/libs/context/doc/html/context/stack/sanitizers.html: *"Sanitizers (GCC/Clang)
  are confused by the stack switches."*; *"The library is required to be compiled with property (b2 command-line)
  `context-impl=ucontext`"*; *"Users must define `BOOST_USE_ASAN` before including any Boost.Context headers when
  linking against Boost binaries."* So **a sanitizer is told only by the `ucontext` backend, and only on request**.
- **verified**, the Valgrind page, https://www.boost.org/doc/libs/latest/libs/context/doc/html/context/stack/valgrind.html:
  *"Running programs that switch stacks under valgrind causes problems."*; `valgrind=on` lets *"valgrind treat the memory
  regions as stack space which suppresses the errors."*; `BOOST_USE_VALGRIND` defined *"before including any
  Boost.Context headers"*.
- **verified**, where a stack comes from: https://www.boost.org/doc/libs/latest/libs/context/doc/html/context/stack.html
  and https://www.boost.org/doc/libs/latest/libs/context/doc/html/context/ff.html: *"allocated/deallocated via a
  StackAllocator"*; `a.allocate()` *"creates a stack"*; *"On construction of fiber a stack is allocated."*; `fiber`
  takes `preallocated(sp,size,sctx)`, in code only. **Neither page says whether a stack may be memory the caller carves
  from its own stack**, neither allowing nor forbidding it (the page reader's search of both).
- **verified, unexplained**: the x86-64 System V `make_fcontext`,
  https://raw.githubusercontent.com/boostorg/context/develop/src/asm/make_x86_64_sysv_elf_gas.S, holds `.cfi_startproc`,
  `.cfi_undefined rip` and `.cfi_endproc` around its trampoline and a `push %rbp` in it, with no comment saying what a
  backtrace does across the switch. What frame-pointer value that first frame holds is **unverified** (bears on Q4).
- **Answer: (c), opt-in, `ucontext` backend only**, for ASan, TSan and Valgrind; the default assembly backend tells no
  tool. On Windows the switch itself rewrites the thread block's four stack fields.

## Q3. libco, the two libraries

- **verified**, byuu's libco (now `higan-emu/libco`), https://raw.githubusercontent.com/higan-emu/libco/master/libco.h:
  *"libco v20 (2019-10-16) author: byuu license: ISC"*; `cothread_t co_derive(void*, unsigned int, void (*)(void));`
  beside `co_create`. Usage, https://raw.githubusercontent.com/higan-emu/libco/master/doc/usage.md: `co_derive`
  *"attempts to use the provided memory instead of allocating new memory on the heap."*; *"certain implementations
  (currently only Windows Fibers) cannot be created using existing memory, and as such, this function will fail."*;
  `co_create`: *"Library is responsible for allocating cothread stack memory"*. **The caller may supply the memory; the
  document says nothing of where it may come from** (so a carved array is neither allowed nor forbidden in words), nor
  of signals, exceptions, debuggers or sanitizers (the page reader's search). The README,
  https://github.com/higan-emu/libco, lists *"Windows platforms (fibers)"* and *"POSIX platforms (setjmp)"* among its
  backends. **It does not copy stacks.**
- **verified**, Tencent's libco, https://raw.githubusercontent.com/Tencent/libco/master/co_routine.cpp (*"Copyright (C)
  2014 THL A29 Limited, a Tencent company."*): **it copies stacks**. `co_alloc_stackmem`: `stack_mem->stack_buffer =
  (char*)malloc(stack_size);`; `co_alloc_sharestack` makes `count` such stacks, `co_get_stackmem` hands them out by
  `alloc_idx % count`; switching to a coroutine whose shared stack another occupies, `save_stack_buffer(occupy_co)`
  mallocs `occupy_co->save_buffer` and `memcpy`s the live part out; after `coctx_swap`,
  `memcpy(update_pending_co->stack_sp, update_pending_co->save_buffer, update_pending_co->save_size);` copies the
  resumed one back, beside *"//stack buffer may be overwrite, so get again;"*. README, https://github.com/Tencent/libco:
  *"By libco copy-stack mode, you can easily build a back-end service support tens of millions of tcp connection."*;
  *"running on tens of thousands of machines since 2013"*. Its stacks are `malloc`'s; neither page names a sanitizer,
  Valgrind or a debugger.
- **Answer**: neither is a runtime with an address-ordered table, so neither chose (a) to (e); both tell no tool.
  **Tencent's copy-stack mode is § 6 G's shape in a shipped library**: coroutines' frames occupy the same addresses
  in turn (on `malloc`'s memory, not the thread's stack).

## Q4. Rust: `stacker` and `corosensei`

- **verified, `corosensei` links a coroutine's first frame to the resumer's ON PURPOSE**. README,
  https://raw.githubusercontent.com/Amanieu/corosensei/master/README.md: *"Backtraces taken from within a coroutine
  will continue into the parent stack"*; a panic will *"unwind through the coroutine stack and then continue to unwind
  out of the caller"*; *"Linked backtraces are not supported on x86 Windows."*; the `sanitizer` feature supports
  sanitizers *"like ASAN"*; `DefaultStack` is *"allocated with a guard page using OS APIs"*, else *"you must implement
  your own stack type which implements the `Stack` trait."* Source, x86-64,
  https://raw.githubusercontent.com/Amanieu/corosensei/master/src/arch/x86_64.rs: *"Save the RBP of the parent context
  to the parent stack. When combined with the return address this forms a valid frame record (RBP & RIP) in the frame
  pointer chain."*; *"Set up the frame pointer to point at the parent link."*; the parent link is *"filled in every
  time switch_and_link() is called."* AArch64, https://raw.githubusercontent.com/Amanieu/corosensei/master/src/arch/aarch64.rs:
  *"Set up the frame pointer to point at the parent link."*; *"the parent link is dynamically updated on each stack
  switch so that the unwinder always finds the current parent frame."*; DWARF via `.cfi_escape` (*"0x0f:
  DW_CFA_def_cfa_expression"*). The x86-64 file says nothing of the Windows thread block (the page reader's search for
  `gs:`, `DeallocationStack`, TIB).
  **So route C's frame-record walk (§ 6 C) is fooled by a real library by design**: from a corosensei coroutine the
  frame-pointer chain reaches the resumer's frames and so the thread's outermost frame (an inference from the quoted
  design, unrun).
- **verified**, `stacker`, README https://raw.githubusercontent.com/rust-lang/stacker/master/README.md: *"Spills over
  to the heap if the stack has hit its limit."*; *"This library currently uses psm for its cross platform
  capabilities"*; on Windows *"an implementation based on Fibers"*; *"On all unsupported platforms this library is a
  noop."* Source, https://raw.githubusercontent.com/rust-lang/stacker/master/src/lib.rs: `_grow` calls
  `set_stack_limit(Some(stack_base as usize));` then `psm::on_stack(stack_base, allocated_stack_size, move || {`; and
  https://raw.githubusercontent.com/rust-lang/stacker/master/src/mmap_stack_restore_guard.rs: the segment is a
  `libc::mmap` with *"two guard pages"* (*"one guard page below the stack and another above it"*), and the guard's
  `drop` ends `set_stack_limit(self.old_stack_limit);`. **It keeps its own thread-local bound true across its own
  switch; it tells nobody else** (no sanitizer or pthread call in the pages read).
- **Answer**: `stacker` (c) **to itself** (it owns the switch and its own bound); `corosensei` (c) to ASan on request,
  and it **links frame chains across the switch for every unwinder and frame walker**, which is the opposite of what
  route C's walk needs.

## Q5. AddressSanitizer, ThreadSanitizer, Valgrind

- **verified**, LLVM review D20913, https://reviews.llvm.org/D20913: *"[asan] add primitives that allow coroutine
  implementations"*, authored by blastrock Jun 2 2016, *"committed in r273260"* (Jun 21 2016): *"This patch adds the
  sanitizer_start_switch_fiber and sanitizer_finish_switch_fiber methods inspired from"* Facebook's folly; *"These
  methods are needed when the compiled software needs to implement coroutines, fibers or the like."*; the motive:
  *"when the program jumps to a stack that is not the thread stack, __asan_handle_no_return shows a warning about that,
  and the fake stack mechanism may free fake frames that are still in use."*
- **verified, what ASan does when NOT told**: `libsanitizer/asan/asan_rtl.cpp` as GCC vendors it,
  https://gnu.googlesource.com/gcc/+/refs/heads/trunk/libsanitizer/asan/asan_rtl.cpp, `UnpoisonStack`: `static const
  uptr kMaxExpectedCleanupSize = 64 << 20; // 64M`, `if (top - bottom > kMaxExpectedCleanupSize)`, then *"WARNING: ASan
  is ignoring requested __asan_handle_no_return: "* *"stack type: %s top: %p; bottom %p; size: %p (%zd)\n"* *"False
  positive error reports may follow\n"* *"For details see "* *"https://github.com/google/sanitizers/issues/189\n"*,
  once (`static bool reported_warning`), and it returns without unpoisoning.
- **verified**, google/sanitizers issue 189, https://github.com/google/sanitizers/issues/189: *"support swapcontext"*,
  first reported 2013-05-22 by konstantin.s.serebryany: *"AddressSanitizer does not fully support swapcontext."*; *"I
  am not at all sure if asan can fully support swapcontext."*; kcc, 2014-01-09: *"swapcontext is a really tricky
  beast."* Shown Closed; no date or commit on the page.
- **verified**, ThreadSanitizer, llvm-commits, https://lists.llvm.org/pipermail/llvm-commits/Week-of-Mon-20190211/627339.html:
  *"[compiler-rt] r353947 - tsan: add fiber support"*, Feb 13 2019 (dvyukov committing, the change Yuri Per's), *"This
  patch adds functions for managing fibers"*: `__tsan_get_current_fiber`, `__tsan_create_fiber`,
  `__tsan_destroy_fiber`, `__tsan_switch_to_fiber`, `__tsan_set_fiber_name`; a resubmit after an earlier commit
  *"was reverted"*. The message gives no reason; review D54889 (https://reviews.llvm.org/D54889) was not read:
  its content is **unverified**.
- **verified**, Valgrind. Manual, https://valgrind.org/docs/manual/manual-core.html: `--max-stackframe=<number>
  [default: 2000000]`; past it *"Valgrind assumes your program is switching to a different stack"*; *"this heuristic
  will be fooled, and Memcheck will subsequently report large numbers of invalid stack accesses."* Source,
  `coregrind/m_stacks.c` of Debian's valgrind 3.24.0, https://sources.debian.org/src/valgrind/1%3A3.24.0-3/coregrind/m_stacks.c:
  *"Warning: client switching stacks?  "*, *"to suppress, use: --max-stackframe=%ld "*; *"We take this to mean that
  the application is switching to a new stack, for whatever reason."*; registered stacks are consulted first. Advanced
  manual, https://valgrind.org/docs/manual/manual-core-adv.html: `VALGRIND_STACK_REGISTER`, `_DEREGISTER`, `_CHANGE`,
  each: *"Unfortunately, this client request is unreliable and best avoided."*
- **Answer: (b) heuristic and warn** when not told, **(c)** when told. Both heuristics are **a distance threshold**
  (ASan 64 MiB, Valgrind 2,000,000 bytes), not a proof: by the conditions quoted, a switch between two stacks nearer
  than the threshold is not taken for a switch (an inference from the conditions, unrun), and both document false
  reports in their own words. **Neither refuses.** Their told interfaces are opt-in, and Valgrind calls its own
  unreliable.

## Q6. Beside them

- **Python's greenlet, verified: stack copying on the thread's own stack.** Old `greenlet.c` (0.3.1, Debian),
  https://sources.debian.org/src/python-greenlet/0.3.1-2.5/greenlet.c: *"A PyGreenlet is a range of C stack addresses
  that must be saved and restored in such a way that the full range of the stack contains valid data when we switch
  to it."*; *"two blocks, the more recent one in the heap and the older one still in the stack"*. Current
  `TStackState.cpp`, https://raw.githubusercontent.com/python-greenlet/greenlet/master/src/greenlet/TStackState.cpp:
  `memcpy(c + sz1, this->_stack_start + sz1, sz2 - sz1);` out to a `PyMem_Realloc` buffer, and *"/* Restore the heap
  copy back into the C stack */"* `memcpy(this->_stack_start, this->stack_copy, this->_stack_saved);`. **This is § 6
  G's case on the thread's own stack**: a C frame of one greenlet and of another hold the same addresses in turn,
  inside the thread's measured range.
- **Windows fibers.** Microsoft, https://learn.microsoft.com/en-us/windows/win32/procthread/fibers (ms.date
  2025-07-14), **verified**: *"The only state information maintained for a fiber is its stack, a subset of its
  registers, and the fiber data provided during fiber creation."*; the page does **not** say that `SwitchToFiber`
  rewrites the thread block's stack fields. Wine's reimplementation,
  https://raw.githubusercontent.com/wine-mirror/wine/master/dlls/kernelbase/thread.c, **verified as Wine's code, not
  Microsoft's**: `NtCurrentTeb()->Tib.StackBase = new_fiber->stack_base;`, `NtCurrentTeb()->Tib.StackLimit =
  new_fiber->stack_limit;`, `NtCurrentTeb()->DeallocationStack = new_fiber->stack_allocation;`,
  `NtCurrentTeb()->Tib.FiberData = new_fiber;`. That Windows itself does the same is **unverified** by a Microsoft page.
- **The .NET CLR, verified: a runtime told of each fiber switch, and the told mode cut.** `ICLRTask::SwitchOut`,
  https://learn.microsoft.com/en-us/dotnet/framework/unmanaged-api/hosting/iclrtask-switchout-method: *"A host calls
  `SwitchOut` to inform the CLR that it has temporarily stopped executing the task"*; *"Available since 2.0"*. Dino
  Viehland, *"Fiber mode is gone..."* (archived MSDN blog, dated 2005-09-15),
  https://learn.microsoft.com/en-us/archive/blogs/dinoviehland/fiber-mode-is-gone: *"In the end we had one final exit
  criteria we'd have to pass in order to make sure fiber mode was solid: Stress."*; they chose *"fixing the bugs that we
  knew ALL of our customers would hit in thread mode"*; *"one API will now return E_NOTIMPL. That one API is
  ICLRTask::SwitchOut."* Joe Duffy, *"Fibers and the CLR"*, Nov 9, 2006, https://joeduffyblog.com/2006/11/09/fibers-and-the-clr/:
  it needs *"close coordination between the host which is doing user-mode scheduling and the CLR"*, and per the page
  reader, without it *"things will break badly"*: the GC *"will fail to find roots for stacks that aren't live on
  threads"*.
- **The Boehm collector, verified: told.** `include/gc/gc.h`, https://raw.githubusercontent.com/ivmai/bdwgc/master/include/gc/gc.h:
  `GC_set_stackbottom`, *"Set the cool end of the user (coroutine) stack of the specified thread."* (sic, the reader
  notes *cool* for *cold*); the client calls it when it switches, under the allocator lock; the comments say nothing of
  the collector detecting a switch itself. Crystal's account, Brian J. Cardiff, 16 February 2022,
  https://crystal-lang.org/2022/02/16/bdw-gc-coroutines-support/: support *"required some patches and eventual
  contributions to bdw-gc"*, as *"there was no built-in support for coroutines in the library"*; Crystal calls
  `GC_set_stackbottom` only before a collection, through `GC_set_push_other_roots`, because it takes a lock.
- **OCaml 5, verified: a runtime that owns the switch and links the stacks for unwinders.** Sivaramakrishnan, Dolan,
  White, Kelly, Jaffer, Madhavapeddy, *"Retrofitting Effect Handlers onto OCaml"*, PLDI 2021, arXiv 2104.00250
  (https://arxiv.org/abs/2104.00250; the PDF read, § 5.2, 5.3, 5.5): *"Fibers are allocated on the C heap using
  malloc"*; *"Since C functions do not have stack overflow checks, we have to execute the external calls in the
  system stack."*; *"Since callbacks may be frequent in OCaml programs that use finalisers, we run the callbacks on the
  same fiber as the current one."*; § 5.5, to keep backtraces whole, *"we insert DWARF bytecode to follow the
  parent_fiber pointer"*; *"During callbacks into OCaml, we save the current system stack pointer in the context
  block"*. The paper does not discuss a C library switching stacks behind the runtime (my reading of § 5.3 and 5.5).
- **The JVM's virtual threads, verified: a runtime that will not move a stack holding native frames.** Oracle, Java
  SE 21 *Virtual Threads*, https://docs.oracle.com/en/java/javase/21/core/virtual-threads.html: *"A virtual thread
  cannot be unmounted during blocking operations when it is pinned to its carrier."*, pinned when *"The virtual thread
  runs a `native` method or a foreign function"*; *"Pinning does not make an application incorrect, but it might hinder
  its scalability."* (JEP 444, https://openjdk.org/jeps/444, answered 403: not read.)
- **Lua, verified: a runtime that refuses a yield across C frames.** Lua 5.4 manual § 4.5,
  https://www.lua.org/manual/5.4/manual.html: Lua *"raises an error whenever it tries to yield across an API call,
  except for three functions"* (`lua_yieldk`, `lua_callk`, `lua_pcallk`).

## The verdict structure

- `verdict`: **approve (advisory)** of a resolution whose correctness never rests on an address order an unseen C switch
  can falsify (route E's direction), with route D as a told accelerator and route A only as a refusal; **object
  (advisory)** to route C's frame-record walk as the proof of a stack, and to any route that detects heuristically and
  continues.
- `precedents`: above, Q1 to Q6, each marked; the load-bearing ones: Go 62440 and 68587 and `cgocall.go` (verified);
  ASan D20913, `asan_rtl.cpp`, sanitizers 189 (verified); TSan r353947 (verified); Valgrind manual and `m_stacks.c`
  (verified); Boost's Windows `jump_fcontext` and its sanitizer and Valgrind pages (verified, at `develop`);
  corosensei's README and `x86_64.rs`/`aarch64.rs` (verified); Tencent libco and greenlet (verified); the CLR's
  `SwitchOut` and its cut (verified); Boehm `gc.h` (verified); OCaml 5's PLDI paper (verified); the JVM's pinning and
  Lua's yield error (verified).
- `argument` (116 words): Every system that keeps stack-address state without owning the switch either must be told
  (ASan, TSan, Valgrind, Boehm, the CLR) or guesses: ASan and Valgrind by distance thresholds, each documenting false
  reports; Go by re-reading bounds and assuming 32 KiB. None found proves which stack it is on by an unforgeable fact;
  corosensei links its frame-pointer chain across switches on purpose, defeating route C's walk. Telling is opt-in and
  fragile: Boost tells only from its `ucontext` backend, Valgrind calls its requests unreliable, the CLR cut
  `SwitchOut` when fiber mode missed its stress criterion. Runtimes owning the switch keep C frames off moved stacks:
  OCaml runs C on the system stack, the JVM pins, Lua refuses.
- `condition`: a production runtime that judges which stack it is on, under C switches it is not told of, by a fact
  that survived corosensei-style linked frames and copy-stack libraries (Tencent libco, greenlet), would move me
  toward route C; a Boost 1.92.0 Windows `jump_fcontext` that does not restore the thread block would put Windows into
  this defect for Boost; a told-switch interface a widely used library calls **by default** (not under a build flag)
  would make route D carry more than an accelerator.
- `search`: below.

## One line per question

1. **Go**: re-reads the thread's bounds at each callback and guesses 32 KiB below SP, with no `throw` there (closest to
   (b) without the warning); https://github.com/golang/go/issues/62440 and 68587, `cgocall.go`. Changed by a Go release
   whose callback refuses an SP outside known bounds.
2. **Boost.Context**: (c), opt-in, `ucontext` only (sanitizers, Valgrind pages); its Windows assembly saves and
   restores the TIB's stack base, limit, deallocation stack and fiber data unconditionally (`jump_x86_64_ms_pe_*.asm`
   at `develop`). Changed by the 1.92.0 tag's file differing.
3. **libco**: byuu's takes caller memory (`co_derive`), copies nothing, tells no tool; Tencent's copies stacks through
   `malloc`'d save buffers, tells no tool; neither is a runtime choosing (a) to (e). Changed by a libco page naming a
   sanitizer call.
4. **stacker / corosensei**: stacker keeps its own bound (c, to itself); corosensei links its first frame record to the
   resumer's on purpose, so route C's walk is fooled by design (README, `x86_64.rs`). Changed by corosensei dropping
   linked backtraces.
5. **ASan / TSan / Valgrind**: (b) by a distance threshold with *"False positive error reports may follow"* / *"client
   switching stacks?"*, and (c) when told (D20913, 2016; r353947, 2019). Changed by a sanitizer that refuses instead
   of warning.
6. **greenlet** copies stack slices on the thread's own stack (§ 6 G's shape, verified); **Windows fibers** rewrite the
   TIB in Wine's code (Microsoft's page silent); **Boehm** (c), told by `GC_set_stackbottom`; **the CLR** (c), then cut
   (`SwitchOut` E_NOTIMPL, 2005); **OCaml 5** owns the switch and links stacks for DWARF; **the JVM** pins, **Lua**
   refuses a yield across C frames (a). Changed by a Microsoft page contradicting Wine.

## Search

Every query was a WebSearch of 2026-11-25 (mode standard); every page a WebFetch the same day.

- **Queries made**: `Go cgo callback x_cgo_getstackbound stack bounds swapcontext issue`; `__sanitizer_start_switch_fiber
  "False positive error reports may follow" __asan_handle_no_return stack switch`; `"ASan is ignoring requested
  __asan_handle_no_return" "False positive error reports may follow"`; `cmd/cgo documentation C code longjmp over Go
  frames "callback" rule`; `boost context preallocated stack "preallocated" fiber constructor user memory stack
  allocator documentation`; `tsan __tsan_create_fiber __tsan_switch_to_fiber added fiber support ThreadSanitizer review
  2019`; `valgrind "client switching stacks?" max-stackframe VALGRIND_STACK_REGISTER manual`; `greenlet slp_switch stack
  copying "stack_start" "stack_stop" saves stack slices to heap how greenlet works`; `Boehm GC GC_get_my_stackbottom
  GC_set_stackbottom coroutine stack switch documentation`; `OCaml 5 effects fibers C stack callback "external" C calls
  run on system stack runtime switch fiber documentation`; `SwitchToFiber updates TIB StackBase StackLimit NT_TIB fiber
  switch Raymond Chen GetCurrentThreadStackLimits fiber` (returned no relevant page but a SwitchToFiber reference
  mirror); `OCaml 5 C library switching stacks swapcontext callback into OCaml crash issue "Stack_overflow" or "fiber"
  ocaml/ocaml issue`; `CLR fiber mode hosting API SQL Server lightweight pooling removed .NET Core "fiber mode" not
  supported`; `Lua 5.4 lua_setcstacklimit deprecated 5.4.3 C stack overflow detection changed counting C calls instead
  of stack address` (the premise did not hold; nothing of it is used above).
- **Pages read** (all cited above): golang/go issues 62440 and 68587; go.dev `cgocall.go`; googlesource
  `stackswitch.c`; pkg.go.dev `cmd/cgo`; reviews.llvm.org D20913; gcc's `asan_rtl.cpp`; google/sanitizers 189;
  llvm-commits r353947; valgrind.org manual-core and manual-core-adv; Debian's valgrind `m_stacks.c`; boostorg/context
  `jump_x86_64_ms_pe_masm.asm`, `jump_x86_64_ms_pe_gas.asm`, `make_x86_64_sysv_elf_gas.S`; boost.org Context pages
  `stack`, `stack/sanitizers`, `stack/valgrind`, `ff`; higan-emu/libco `libco.h`, `doc/usage.md`, README; Tencent/libco
  README and `co_routine.cpp`; corosensei README, `x86_64.rs`, `aarch64.rs`; rust-lang/stacker README, `lib.rs`,
  `mmap_stack_restore_guard.rs`; greenlet 0.3.1 `greenlet.c` (Debian) and current `TStackState.cpp`; Microsoft's
  *Fibers* page and `ICLRTask::SwitchOut`; the archived *Fiber mode is gone...*; Joe Duffy's *Fibers and the CLR*;
  bdwgc `gc.h` (both halves); Crystal's bdw-gc post; arXiv 2104.00250 (abstract page and the PDF's pages 7 to 10);
  Oracle's Java 21 *Virtual Threads*; the Lua 5.4 manual (first 100,000 characters).
- **Tried and not read**: openjdk.org JEP 444 (HTTP 403); gitlab.winehq.org and sourceware.org (bot-protection pages,
  so glibc's `__start_context.S` was not read and what glibc's `makecontext` writes as the first frame record is
  **unread here**); reviews.llvm.org D54889; the golang-nuts *cgo & long jump* thread; golang/go issue 59294; OCaml's
  `runtime/callback.c` (a search summary's quotation of it is not used).
- **Swept and not found, said as what was looked for**: a sentence of cgo's documentation on C jumps or stack switches
  (searched on its page under eight words, Q1); a sentence of Boost.Context's documentation on a stack carved by the
  caller (its `stack` and `ff` pages); a sentence of byuu's libco on where `co_derive`'s memory may come from; a
  Microsoft page stating that `SwitchToFiber` rewrites the TIB's stack fields (one search, the *Fibers* page). **Not
  searched at all**: a precedent for HELD's rule (a handler not seen to end held to be running), so this report says
  nothing for or against that arm.
- **A side effect to report**: WebFetch of the arXiv PDF saved the binary under this session's tool-results folder in
  `/Users/joseph/.claude/projects/.../tool-results/` (the tool's own behaviour, not a write of mine); it is outside the
  repository, and its removal is the coordinator's to decide.

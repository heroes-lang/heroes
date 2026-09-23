# Panel 175: historian report (advisory seat, no veto)

Written out verbatim by the coordinator on 2026-09-23 from the seat's final
message; the seat has no file-write tool.

**`verdict`: approve D (route A with route B), with A keyed on a set of releasers rather than one name, and approve E with F. Object to C and to G.**

I did not use *Heroes of code* as evidence. Every claim below was checked on the web on 2026-09-23 and carries its link. I read two repository files to place the precedents against what exists: panel 148's record and `runtime/parts/os.c`.

---

## Precedents

### 1. The releaser recorded per allocation, and the wrong one refused at run time (route A's shape)

**Android fdsan: verified.** Its documentation says it "detects mishandling of file descriptor ownership, which tend to manifest as use-after-close and double-close" ([fdsan.md](https://android.googlesource.com/platform/bionic/+/master/docs/fdsan.md)).
- **What it records:** a 64-bit tag per file descriptor, made of an 8-bit owner type and a 56-bit value.
- **Owner types include:** `FILE*`, `DIR*`, `unique_fd` and `sqlite` ([fdsan.h](https://android.googlesource.com/platform/bionic/+/master/libc/include/android/fdsan.h)).
- **Abort or report:** closing with the wrong tag "Logs and aborts if the tag is incorrect". The default was warn-once at API 29 and became fatal at API 30.
- **Its message names both sides:** *"attempted to close file descriptor 3, expected to be unowned, actually owned by unique_fd 0x7bf15dc448"*.
- **What happened:** it caught a real wrong-releaser in emacs on Android 11 on 2021-04-04: *"fdsan: attempted to close file descriptor 2, expected to be unowned, actually owned by FILE\* 0xb6c8800c"* ([termux #6592](https://github.com/termux/termux-packages/issues/6592)). I did not verify the root cause of that bug.
- **Cost:** the documentation gives none. **Undocumented.**
- This is the closest precedent to route A: a registry of live resources that remembers who may end each one, and refuses at the release call.

**AddressSanitizer `alloc-dealloc-mismatch`: verified, with gaps.**
- **What it records:** a 2-bit `alloc_type` in each chunk header (`FROM_MALLOC`, `FROM_NEW`, `FROM_NEW_BR`). `Deallocate` compares it with the kind of release and calls `ReportAllocTypeMismatch` ([asan_allocator.cpp](https://raw.githubusercontent.com/llvm/llvm-project/main/compiler-rt/lib/asan/asan_allocator.cpp)).
- **Defaults:** on for Linux, "false on Darwin and Windows". It reports, and then crashes by default unless the program was built with `-fsanitize-recover` ([ASan flags](https://github.com/google/sanitizers/wiki/addresssanitizerflags)).
- **MSVC:** "In Windows, `alloc-dealloc-mismatch` error detection is off by default" ([Microsoft Learn](https://learn.microsoft.com/en-us/cpp/sanitizers/error-alloc-dealloc-mismatch?view=msvc-170)).
- **Documented false positives:** Mozilla turned the check off (milestone mozilla25; the page dates it about 13 years ago, exact date **UNVERIFIED**). Two reasons were given. Firefox builds `new`/`delete` on top of `malloc`/`free`, so the two releasers really are one. And *"The function produces false positives, e.g. when a call to 'new' is inlined, but the call to 'delete' is not"* ([bug 898230](https://bugzilla.mozilla.org/show_bug.cgi?id=898230)).
- **UNVERIFIED:** when the check first shipped, and why it defaults to off on Darwin.

**Valgrind Memcheck, "Mismatched free() / delete / delete []": verified, with gaps.**
- `--show-mismatched-frees` defaults to `yes`.
- The documented false positive is when "the user provides implementations of `new`/`new[]` that call `malloc` and of `delete`/`delete[]` that call `free`, and these functions are asymmetrically inlined" ([Memcheck manual](https://valgrind.org/docs/manual/mc-manual.html)). That is again the case where the two releasers are really one.
- **UNVERIFIED:** that it reports and then continues (the manual as fetched describes reporting and says nothing about stopping), and which release added the option.

**Application Verifier: verified.**
- Stop `0x6 SWITCHED_HEAP_HANDLE`, *"Corrupted heap pointer or using wrong heap"*.
- Its probable cause reads: *"The most common example is a msvcrt allocation using `malloc` paired with a kernel32 deallocation using `HeapFree`."*
- It records the heap each block came from ("Parameter 4 - Heap where block was originally allocated"), and a stop breaks into the debugger ([stop codes](https://learn.microsoft.com/en-us/windows-hardware/drivers/devtest/application-verifier-stop-codes-basics)).
- **UNVERIFIED:** when it shipped.

**MSVC debug heap: partly verified.**
- It records a block type per block: `_NORMAL_BLOCK`, `_CRT_BLOCK`, `_CLIENT_BLOCK`, `_FREE_BLOCK`, `_IGNORE_BLOCK` ([CRT debug heap](https://learn.microsoft.com/en-us/cpp/c-runtime-library/crt-debug-heap-details?view=msvc-170)).
- `_free_dbg` takes a `blockType` argument ([_free_dbg](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/free-dbg?view=msvc-170)).
- **UNVERIFIED:** whether a mismatched type is refused. The documentation says only that it checks for overwrites.

**What all five share, and it matters for route A.** None of them keys on the identity of one function. fdsan keys on an owner, ASan and Valgrind on a family, AppVerifier on a heap. **Every documented false positive is the case where two releasers are interchangeable.**

### 2. The wrong releaser refused at compile time (route B's shape)

**GCC 11, `malloc (deallocator)` and `-Wmismatched-dealloc`: verified.**
- **The mark:** GCC 11 extended the `malloc` attribute "to identify allocator/deallocator API pairs". `-Wmismatched-dealloc` is "enabled by default" ([GCC 11 changes](https://gcc.gnu.org/gcc-11/changes.html)); the commit is r11-5732, archived in 2020q4 ([commit](https://gcc.gnu.org/pipermail/libstdc++-cvs/2020q4/035182.html)).
- **GCC's own example is our reproducer:** `malloc (pclose, 1)` on `popen` and `malloc (fclose, 1)` on `fopen` ([GCC 11.4 docs](https://gcc.gnu.org/onlinedocs/gcc-11.4.0/gcc/Common-Function-Attributes.html)).
- **What happened:** glibc master ships it today: `popen (...) __attribute_malloc__ __attr_dealloc (pclose, 1)` ([stdio.h](https://raw.githubusercontent.com/bminor/glibc/master/libio/stdio.h)). **UNVERIFIED:** the first glibc release that carried it.
- **Across a function boundary:** "detection in the absence of optimization is limited to the scope of function bodies". It has false positives and "frequent" false negatives (Martin Sebor, 2021-05-05, [Red Hat](https://developers.redhat.com/blog/2021/05/05/detecting-memory-management-bugs-with-gcc-11-part-2-deallocation-functions)).
- **GCC's own analyzer learned the set lesson:** *"There can be more than one valid deallocator for a given allocator, for example: `malloc (fclose)`, `malloc (freopen, 3)` … We track the expected deallocator_set for a value, but not the allocation function"* ([sm-malloc.cc](https://github.com/gcc-mirror/gcc/blob/master/gcc/analyzer/sm-malloc.cc)).
- **The glibc review said the same:** Florian Weimer, 2021-01-11: *"realloc is a deallocator for malloc, xfclose is one for fopen"* ([libc-alpha](https://sourceware.org/pipermail/libc-alpha/2021-January/121487.html)).

**Clang Static Analyzer: verified.**
- `unix.MismatchedDeallocator` knows `malloc`/`free`, `new`/`delete`, `new[]`/`delete[]`, plus custom pairs through the ownership attributes ([checkers](https://clang.llvm.org/docs/analyzer/checkers.html)).
- The attribute's first argument is "the type of the allocation (e.g. `malloc`, `new`, or any other identifier)" ([Annotations](https://clang.llvm.org/docs/analyzer/user-docs/Annotations.html)). So it keys on a family name, not on a function.
- The analyzer only reads those attributes when `unix.DynamicMemoryModeling:Optimistic=true` is set (Kristóf Umann, merged 2025-01-07, [PR #121759](https://github.com/llvm/llvm-project/pull/121759)).
- "Normally, static analysis works in the boundary of one translation unit" ([CTU](https://clang.llvm.org/docs/analyzer/user-docs/CrossTranslationUnit.html)).

### 3. The releaser put in the TYPE (route C's shape)

**C++ `std::unique_ptr&lt;T, Deleter&gt;`: verified.** The deleter is a template parameter. cppreference's `FILE` example is `std::unique_ptr&lt;std::FILE, decltype(&amp;close_file)&gt;` with `&amp;close_file` passed at construction ([cppreference](https://en.cppreference.com/cpp/memory/unique_ptr)).
- *My inference:* `decltype(&amp;close_file)` is a function-pointer type, so an `fclose` wrapper and a `pclose` wrapper give the same type. With that deleter, the releaser is stored per object, which is route A's shape. Only a stateless functor separates them by type.

**`shared_ptr` refused route C on purpose: verified.** N1450 (Dimov, Dawes, Colvin, 2003-03-27): *"Has the same object type regardless of features used, greatly facilitating interoperability between libraries"*, and *"Since the deleter is not part of the type, changing the allocation strategy does not break source or binary compatibility"* ([N1450](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2003/n1450.html)).

**Rust `Box&lt;T, A = Global&gt;`: verified.**
- `pub struct Box&lt;T, A = Global&gt; where A: Allocator` ([Box](https://doc.rust-lang.org/std/boxed/struct.Box.html)). The parameter was merged on 2020-10-26 ([PR #77187](https://github.com/rust-lang/rust/pull/77187)).
- `allocator_api` is still unstable ([#32838](https://doc.rust-lang.org/nightly/unstable-book/library-features/allocator-api.html)).
- `from_raw`: "must point to a block of memory allocated by the global allocator".

**.NET `SafeHandle`: verified.** *"some use the CloseHandle function, while others use … FindClose. For this reason, you must create a derived class of SafeHandle for each operating system handle type"* ([SafeHandle](https://learn.microsoft.com/en-us/dotnet/fundamentals/runtime-libraries/system-runtime-interopservices-safehandle)).
- *My inference:* functions that take any handle are spared the cost because C# has subclassing.

**WIL (Windows Implementation Libraries): verified.** `unique_handle` closes with `CloseHandle`, `unique_hfind` with `FindClose`, `unique_hfind_change` with `FindCloseChangeNotification`. WIL says it is "meant to be used through a dedicated typedef for each of the many unique handle and token types". The raw `HANDLE` comes back out through `.get()` ([wiki](https://github.com/Microsoft/wil/wiki/RAII-resource-wrappers)).

**win32metadata, CsWin32 and windows-rs: the closest analogue, a binding generator over C headers. Verified, with one inference.**
- **The friction:** CsWin32's `FindFirstFile` returned `FindCloseSafeHandle`, "not compatible with FindClose, which receives a handle of type HANDLE" (2021-04-04, [#399](https://github.com/microsoft/win32metadata/issues/399)). A windows-rs user asked to "Improve FindFileHandle" and said the current way was "some casting" (2022-07-06, [#989](https://github.com/microsoft/win32metadata/issues/989)).
- **The plan now:** opened 2026-08-27, merged 2026-09-15, it says *"Do not create `PRINTER_HANDLE`, `HEAP_HANDLE`, or similar metadata-only pseudo types"* and *"APIs that natively use `HANDLE` remain `HANDLE`, and ownership is attached only to a producer return or output parameter"* ([plan](https://github.com/microsoft/win32metadata/blob/main/docs/copilot/plans/shift-left-annotation-spec.md), [PR #2295](https://github.com/microsoft/win32metadata/pull/2295)).
- **Today:** windows-rs has `FindFirstFileA(...) -&gt; Result&lt;HANDLE&gt;` and `FindClose(hfindfile: HANDLE)` ([FindFirstFileA](https://microsoft.github.io/windows-docs-rs/doc/windows/Win32/Storage/FileSystem/fn.FindFirstFileA.html), [FindClose](https://microsoft.github.io/windows-docs-rs/doc/windows/Win32/Storage/FileSystem/fn.FindClose.html)).
- *My inference* is that they "walked it back". **UNVERIFIED:** the date of that change, and whether the plan is implemented beyond documentation.

**Cyclone: verified.** "every pointer type carries a region annotation". "Porting legacy C to Cyclone has required altering about 8% of the code; of the changes, only 6% (of the 8%) were region annotations", held down by "default annotations, local type inference, and a novel treatment of region effects" (Grossman et al., PLDI 2002, [paper](https://www.cs.umd.edu/projects/cyclone/papers/cyclone-regions.pdf)).

### 4. A crash handler that speaks when the C allocator kills the process (route E), and what each platform prints

**What each C allocator does on a double free by default:**
- **glibc: verified.** It prints `free(): double free detected in tcache 2` (DJ Delorie, committed 2018-11-20, backported to the 2.27 branch, [libc-stable](https://sourceware.org/legacy-ml/libc-stable/2018-12/msg00008.html)). glibc 2.29 came out 2019-01-31 ([LWN](https://lwn.net/Articles/778286/)); that 2.29 is the first release carrying the check is my inference from those two dates.
- **macOS libmalloc: partly verified.** It printed `pointer being freed was not allocated` and died on SIGABRT on macOS 26.2 ([tmux #4777](https://github.com/tmux/tmux/issues/4777)). Our brief measured a different outcome here: SIGTRAP with 0 bytes on Darwin arm64. **UNVERIFIED:** which path through libmalloc prints and which traps silently.
- **Windows: verified.** Termination on heap corruption is on "for all 64-bit processes" and is "a one-way door" (Raymond Chen, 2013-12-27, [Old New Thing](https://devblogs.microsoft.com/oldnewthing/20131227-00/?p=2243)). An unhandled-exception filter is not called for `0xC0000374`; a vectored handler is (2017-07-30, [blog](https://peteronprogramming.wordpress.com/2017/07/30/crashes-you-cant-handle-easily-3-status_heap_corruption-on-windows/)). `os.c:229` already installs a vectored handler, so this precedent agrees with what was built.

**Language runtimes that add their own line:**
- **Go: verified, and it speaks with no bookkeeping.** `fatalsignal` prints `signal arrived during cgo execution` when `mp.incgo`, then prints the Go stack that led to the C call ([signal_unix.go](https://go.dev/src/runtime/signal_unix.go?m=text)). Real output from 2018: glibc's `double free or corruption (out)`, then `SIGABRT: abort … signal arrived during cgo execution`, then Go's frame `_Cfunc_free` ([moby #37916](https://github.com/moby/moby/issues/37916)).
- **Python `faulthandler`: verified.** Added in 3.3; not on by default. It handles SIGSEGV, SIGFPE, SIGABRT, SIGBUS and SIGILL, prints "Fatal Python error: …" and the traceback ([docs](https://docs.python.org/3/library/faulthandler.html)). Then it restores the previous handler and calls `raise(signum)` ([faulthandler.c](https://raw.githubusercontent.com/python/cpython/main/Modules/faulthandler.c)).
- **HotSpot: verified.** Its fatal-error log names a "Problematic frame: C [libNativeSEGV.so+0x9d7]" in a Java 6 example ([Oracle TSG](https://docs.oracle.com/javase/7/docs/webnotes/tsg/TSG-VM/html/felog.html)). It also says "The crash happened outside the Java Virtual Machine in native code" (2020-12-03, [inside.java](https://inside.java/2020/12/03/crash-outside-the-jvm/)).
- **Rust: verified, and it marks the limit.** It speaks only when the faulting address is inside the guard page. Otherwise "the handler un-registers itself and then returns" ([stack_overflow.rs](https://doc.rust-lang.org/nightly/src/std/sys/pal/unix/stack_overflow.rs.html)). This dates from 2014-10-24 ([PR #16388](https://github.com/rust-lang/rust/pull/16388/files)).

### 5. A spec sentence saying the escape hatch is not tracked (route F)

- **Nim: verified.** "Nim distinguishes between traced and untraced references … Thus, untraced references are *unsafe*" ([manual](https://nim-lang.org/docs/manual.html)).
- **Rust `CString::from_raw`: verified.** "trying to take ownership of a string that was allocated by foreign code … is likely to lead to undefined behavior or allocator corruption" ([CString](https://doc.rust-lang.org/std/ffi/struct.CString.html)).

### The case of two releasers that are both valid

- **SQLite: verified.** "The sqlite3_close() and sqlite3_close_v2() routines are destructors for the sqlite3 object" ([close](https://www.sqlite.org/c3ref/close.html)).
- **GLib, for plain pointers: verified.** Since 2.46, "memory allocated with `malloc()` can be used interchangeably with memory allocated using g_malloc()" ([mem_is_system_malloc](https://docs.gtk.org/glib/func.mem_is_system_malloc.html)).

---

## What the precedents predict for Heroes, route by route

**A: the shape is proven.**
- Its cost is one tag per live handle: 2 bits in ASan, 64 in fdsan. Aborting and naming both sides is where fdsan ended up.
- The memory tools' false positives came from inlining, which hides which function actually freed. Heroes calls a releaser by the name written in the source, so that class should not carry over (*my inference*).
- The class of two valid releasers does carry over: GCC's own comment says so, and so do SQLite and Weimer.
- **Keyed on one name, A refuses a correct program that closes with `sqlite3_close_v2`. Keyed on a set, it does not.**

**B: partial, as GCC is without optimisation.**
- It is limited to one function body.
- Both GCC's author and Clang's documentation describe false negatives. No precedent I found treats the compile-time check as enough on its own.
- It is worth having only paired with A, which is route D.

**C: every precedent needed a way out for the functions taking the C type.**
- The ways out were subclassing (.NET), unwrapping (WIL's `.get()`), polymorphism with defaults (Cyclone), or a default type parameter that is still unstable (Rust).
- `shared_ptr` refused C for interoperability.
- The closest analogue is now moving away from C, towards the producer mark that Heroes already has.
- Heroes has no way out today, because `declared_twice` refuses the duplicate declaration. So C would mean one `FILE*` declaration per Heroes type, or a new language form (a Principle 0 cost).
- C does handle SQLite's two destructors naturally, because both take the same type. It fails on stdio's shared readers. A keyed on a set handles both.

**E: this is Go's default shape.**
- Say the signal first, then the Heroes frame, and claim no cause. Rust's rule sets the limit: never state a cause that was not verified.
- On Linux, glibc's line will come first, in the same order as moby's output. On Darwin and Windows, where the brief measured 0 bytes, E's line will be the only one.

**F:** the Nim and Rust documentation sentences are cheap and conventional. They catch nothing by themselves, so F goes with E.

**G:** Go and HotSpot speak by default, and Python offers it. G is only right where no true sentence is available, and E's sentence is a fact.

**Is a sentence owed in § 13?** GCC documents the pairing and, separately, the diagnostics that read it; fdsan documents that it aborts. Defect 075 is what a pairing becomes when the name is written and no consequence is stated. **The sentence is owed.**

**Two routes nobody listed.**
- **A-set:** the mark names one or more releasers. GCC's multiple `malloc (dealloc)` attributes and its analyzer's `deallocator_set` are the model. It changes the grammar, which is the warden's price to set.
- **H:** the handle carries its releaser, and the program ends its life with a single form, as `shared_ptr`, Haskell's `newForeignPtr` ([Haskell 2010](https://www.haskell.org/onlinereport/haskell2010/haskellch29.html)) and `SafeHandle.ReleaseHandle` do. Panel 148's file quotes panel 147's rule, "the compiler never picks the release call". Whether calling the releaser the program itself named counts as the compiler picking is **a question, not a premise**. I did not read panel 147.

---

## `argument` (117 words)

Every shipped checker of this class records who may release each live object and checks it at the release call; fdsan went from warn-once to fatal in one API level, and ASan has done it since at least 2013. None keys on one function: each keys on a family, owner or set, and GCC's analyzer says why, a second valid releaser exists; SQLite documents two destructors for one connection. So A, keyed on a set. C was tried by the closest analogue, Microsoft's Win32 metadata, whose plan, merged 2026-09-15, now forbids such pseudo-types and marks the producer's return, Heroes's present design. E is what Go prints by default: the signal, then the frame, and no claimed cause.

## `condition`: what would change my reading

1. **A keyed on one name is enough** if a measured search of the libraries this tree binds (SQLite, libcurl, stdio, the Windows CRT) finds no acquisition documented with two valid releasers. SQLite's page already says otherwise, so the ffi seat has to show that `sqlite3_close_v2` is out of scope.
2. **C becomes viable** if someone finds a shipped binding generator that gave one C type several wrapper types by releaser, with no subclassing and no unwrap, and whose shared functions did not multiply.
3. **E becomes suspect** if Go's `signal arrived during cgo execution` line, or HotSpot's problematic-frame line, is ever documented naming a frame that was not executing.

## Falsifiable prediction

**If A lands keyed on one releaser per acquisition, the first program in `examples/` to declare two `consumes` functions for one handle type will have both of them valid** (a pair of destructors like SQLite's `sqlite3_close` and `sqlite3_close_v2`), not mutually exclusive like `pclose` and `fclose`. Its correct programs will then abort under that version of A unless the mark names both.

- **Checkable at:** the first milestone after M-agreed-retention whose `examples/` gains a second `consumes` function for an existing handle type. The brief counts zero today. Golden tests written to exercise A itself are excluded.
- **Falsified if:** that first pair is mutually exclusive, popen-style.

## Named as unverified

- When ASan's mismatch check first shipped, and why it is off on Darwin.
- That Memcheck continues after reporting, and which release added `--show-mismatched-frees`.
- When Application Verifier shipped.
- Whether the MSVC debug heap refuses a mismatched block type.
- Which glibc release first carried `__attr_dealloc`.
- Which libmalloc path prints and which traps silently.
- The date of win32metadata's change back to `HANDLE`, and whether its plan is implemented.
- The root cause of the emacs bug fdsan caught.
- What fdsan costs at run time.

Sources:
- [fdsan.md](https://android.googlesource.com/platform/bionic/+/master/docs/fdsan.md) · [fdsan.h](https://android.googlesource.com/platform/bionic/+/master/libc/include/android/fdsan.h) · [termux #6592](https://github.com/termux/termux-packages/issues/6592)
- [ASan flags](https://github.com/google/sanitizers/wiki/addresssanitizerflags) · [asan_allocator.cpp](https://raw.githubusercontent.com/llvm/llvm-project/main/compiler-rt/lib/asan/asan_allocator.cpp) · [MSVC alloc-dealloc-mismatch](https://learn.microsoft.com/en-us/cpp/sanitizers/error-alloc-dealloc-mismatch?view=msvc-170) · [Mozilla 898230](https://bugzilla.mozilla.org/show_bug.cgi?id=898230)
- [Memcheck manual](https://valgrind.org/docs/manual/mc-manual.html) · [AppVerifier stop codes](https://learn.microsoft.com/en-us/windows-hardware/drivers/devtest/application-verifier-stop-codes-basics) · [CRT debug heap](https://learn.microsoft.com/en-us/cpp/c-runtime-library/crt-debug-heap-details?view=msvc-170) · [_free_dbg](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/free-dbg?view=msvc-170)
- [GCC 11 changes](https://gcc.gnu.org/gcc-11/changes.html) · [GCC 11.4 attributes](https://gcc.gnu.org/onlinedocs/gcc-11.4.0/gcc/Common-Function-Attributes.html) · [r11-5732](https://gcc.gnu.org/pipermail/libstdc++-cvs/2020q4/035182.html) · [Sebor, Red Hat](https://developers.redhat.com/blog/2021/05/05/detecting-memory-management-bugs-with-gcc-11-part-2-deallocation-functions) · [sm-malloc.cc](https://github.com/gcc-mirror/gcc/blob/master/gcc/analyzer/sm-malloc.cc) · [glibc stdio.h](https://raw.githubusercontent.com/bminor/glibc/master/libio/stdio.h) · [Weimer, libc-alpha](https://sourceware.org/pipermail/libc-alpha/2021-January/121487.html)
- [Clang checkers](https://clang.llvm.org/docs/analyzer/checkers.html) · [Clang annotations](https://clang.llvm.org/docs/analyzer/user-docs/Annotations.html) · [PR #121759](https://github.com/llvm/llvm-project/pull/121759) · [Clang CTU](https://clang.llvm.org/docs/analyzer/user-docs/CrossTranslationUnit.html)
- [unique_ptr](https://en.cppreference.com/cpp/memory/unique_ptr) · [N1450](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2003/n1450.html) · [Rust Box](https://doc.rust-lang.org/std/boxed/struct.Box.html) · [PR #77187](https://github.com/rust-lang/rust/pull/77187) · [allocator_api](https://doc.rust-lang.org/nightly/unstable-book/library-features/allocator-api.html)
- [SafeHandle](https://learn.microsoft.com/en-us/dotnet/fundamentals/runtime-libraries/system-runtime-interopservices-safehandle) · [WIL wrappers](https://github.com/Microsoft/wil/wiki/RAII-resource-wrappers)
- [win32metadata #399](https://github.com/microsoft/win32metadata/issues/399) · [#989](https://github.com/microsoft/win32metadata/issues/989) · [shift-left plan](https://github.com/microsoft/win32metadata/blob/main/docs/copilot/plans/shift-left-annotation-spec.md) · [PR #2295](https://github.com/microsoft/win32metadata/pull/2295) · [windows-rs FindFirstFileA](https://microsoft.github.io/windows-docs-rs/doc/windows/Win32/Storage/FileSystem/fn.FindFirstFileA.html) · [FindClose](https://microsoft.github.io/windows-docs-rs/doc/windows/Win32/Storage/FileSystem/fn.FindClose.html)
- [Cyclone regions paper](https://www.cs.umd.edu/projects/cyclone/papers/cyclone-regions.pdf)
- [glibc tcache commit](https://sourceware.org/legacy-ml/libc-stable/2018-12/msg00008.html) · [LWN glibc 2.29](https://lwn.net/Articles/778286/) · [tmux #4777](https://github.com/tmux/tmux/issues/4777) · [Raymond Chen](https://devblogs.microsoft.com/oldnewthing/20131227-00/?p=2243) · [STATUS_HEAP_CORRUPTION blog](https://peteronprogramming.wordpress.com/2017/07/30/crashes-you-cant-handle-easily-3-status_heap_corruption-on-windows/)
- [Go signal_unix.go](https://go.dev/src/runtime/signal_unix.go?m=text) · [moby #37916](https://github.com/moby/moby/issues/37916) · [faulthandler docs](https://docs.python.org/3/library/faulthandler.html) · [faulthandler.c](https://raw.githubusercontent.com/python/cpython/main/Modules/faulthandler.c) · [HotSpot TSG](https://docs.oracle.com/javase/7/docs/webnotes/tsg/TSG-VM/html/felog.html) · [inside.java](https://inside.java/2020/12/03/crash-outside-the-jvm/) · [Rust stack_overflow.rs](https://doc.rust-lang.org/nightly/src/std/sys/pal/unix/stack_overflow.rs.html) · [PR #16388](https://github.com/rust-lang/rust/pull/16388/files)
- [Nim manual](https://nim-lang.org/docs/manual.html) · [Rust CString](https://doc.rust-lang.org/std/ffi/struct.CString.html) · [SQLite close](https://www.sqlite.org/c3ref/close.html) · [GLib mem_is_system_malloc](https://docs.gtk.org/glib/func.mem_is_system_malloc.html) · [Haskell 2010 ForeignPtr](https://www.haskell.org/onlinereport/haskell2010/haskellch29.html)

Repository files read: `/Users/joseph/Temp/heroes/heroes-lang/docs/panel/175-briefs/historian.md`, `/Users/joseph/Temp/heroes/heroes-lang/docs/panel/175-briefs/00-shared.md`, `/Users/joseph/Temp/heroes/heroes-lang/docs/panel/148-the-mark-is-written-and-never-inferred-and-it-names-what-ends-the-life.md`, `/Users/joseph/Temp/heroes/heroes-lang/runtime/parts/os.c` (lines 150-292).

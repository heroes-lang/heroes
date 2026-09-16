# Panel 156 — historian report

Written to disk by the coordinator, verbatim from the seat's final message,
because this seat has no write tool. **All URLs checked 2026-09-16.** Every claim
carries a source or the word `unverified`. *Heroes of code* was not a route to
any item here: these are runtime/standards questions, each settled against a
standard or a primary source, so the book is named only to say it played no part.

## verdict

**object (advisory), narrowly.** Not to any repair, and not to a design — I
recommend none. I object to one *factual premise* that R1 may rest on: that
naming only the foreign function is a defensible chosen answer. I searched for a
language that deliberately names only the foreign frame and found none; the two
comparable systems I could source both do the opposite, one with a code comment
stating the intent. That is a negative finding limited by my search vocabulary
(see `condition`), not a proof.

On R3 the standards settle something outright, and it is not in the direction of
"just flush": see items 3 and 4.

## Item 3 — Does C require `abort()` to flush? **No. Implementation-defined.** VERIFIED

- **Clause: C11/C17 §7.22.4.1 "The abort function"** (C99 §7.20.4.1, C89
  §4.10.4.1). Verified via cppreference's standard-reference block: *"Whether
  open resources such as files are closed is implementation defined"*;
  *"Functions passed to `atexit()` are not called."*
  https://en.cppreference.com/w/c/program/abort
  **PARTIALLY VERIFIED**: the clause NUMBER and a faithful summary, but not the
  verbatim ISO sentence — two N1570 mirrors truncated before §7.22. The panel
  should cite the clause number and the implementation-defined status, and not
  quote ISO wording from me.

- **POSIX corroborates, and weakened its own text because of it.** IEEE Std
  1003.1-2024 (Issue 8), `abort()`. DESCRIPTION: *"The abnormal termination
  processing shall include the default actions defined for SIGABRT and **may**
  include an attempt to effect `fclose()` on all open streams."* RATIONALE: *"The
  ISO/IEC 9899:1999 standard required (and the current standard still requires)
  the `abort()` function to be async-signal-safe. Since POSIX.1-2024 defers to
  the ISO C standard, this required a change to the DESCRIPTION from 'shall
  include the effect of `fclose()`' to 'may include an attempt to effect
  `fclose()`.'"*
  https://pubs.opengroup.org/onlinepubs/9799919799/functions/abort.html
  **VERIFIED.** The single most load-bearing find: the standards body **already
  had this exact argument** and resolved it by *removing* the flush guarantee, on
  async-signal-safety grounds.

## Item 3b — What the two libcs do, and both documented the tension. VERIFIED

- **glibc changed sides at 2.27.** `abort(3)` HISTORY: *"Up to glibc 2.26: if the
  `abort()` function caused process termination, all open streams were closed and
  flushed (as with `fclose(3)`)."* … *"From glibc 2.27 onward: `abort()`
  terminates the process without flushing streams."* Motivation stated: flushing
  *"could result in deadlocks and data corruption."* And: *"POSIX.1 permits either
  possible behavior."*
  https://man7.org/linux/man-pages/man3/abort.3.html
  **VERIFIED.** A precedent that ran the experiment: a major libc shipped
  flush-on-abort for decades, then removed it, naming deadlock and corruption.
  Under CLAUDE.md § Precedence rank 3 that motivation is directly on point and
  cuts against an unguarded flush.

- **FreeBSD keeps flushing and says in the source that it knows it is wrong.**
  `lib/libc/stdlib/abort.c`, verbatim:

  ```c
  /*
   * POSIX requires we flush stdio buffers on abort.
   * XXX ISO C requires that abort() be async-signal-safe.
   */
  if (__cleanup)
  	(*__cleanup)();
  ```

  https://raw.githubusercontent.com/freebsd/freebsd-src/main/lib/libc/stdlib/abort.c
  **VERIFIED.** The `XXX` is the whole panel question in one comment line.

- **Darwin specifically: UNVERIFIED.** The brief's measurement (stdout `7`
  survives on macOS) is consistent with Apple's Libc carrying the FreeBSD file,
  but the opensource.apple.com URL returned **HTTP 404** on 2026-09-16. Write
  "macOS's libc flushes on abort" as measured, not as sourced.

## Item 4 — Is `fflush` async-signal-safe? **No. `write` is.** VERIFIED

**POSIX.1-2024 (Issue 8), System Interfaces §2.4.3 "Signal Actions."** The
async-signal-safe list **includes `write()`, `abort()` and `_exit()`** and **does
not include `fflush()`**. The governing sentence: *"…the behavior is undefined if
… the signal handler calls any function or function-like macro defined in this
standard other than one of the functions and macros specified below as being
async-signal-safe."*
https://pubs.opengroup.org/onlinepubs/9799919799/functions/V2_chap02.html
**VERIFIED** (section number and membership read off that page on 2026-09-16).

Consequence for R3, as a constraint and not a recommendation: calling `fflush`
from the SIGSEGV handler is **undefined behaviour by the letter of POSIX**, not
folklore, and glibc's HISTORY names the two failure modes it produces. What is
safe is `write`, which `hero_stack_say` already uses — the same conclusion the
two libcs reached from opposite directions.

## Item 1 — What does Go NAME when the fault is inside C? **Its own frame, deliberately.** VERIFIED

`src/runtime/signal_unix.go`, verbatim:

```go
if mp.incgo && gp == mp.g0 && mp.curg != nil {
    print("signal arrived during cgo execution\n")
    // Switch to curg so that we get a traceback of the Go code
    // leading up to the cgocall, which switched from curg to g0.
    gp = mp.curg
}
```

and for the machine facts:

```go
print("PC=", hex(c.sigpc()), " m=", mp.id, " sigcode=", c.sigcode())
if sig == _SIGSEGV || sig == _SIGBUS {
    print(" addr=", hex(c.fault()))
}
```

https://go.dev/src/runtime/signal_unix.go?m=text
**VERIFIED.** An explicit design statement: when the fault is in C, Go *switches
goroutines specifically so the traceback names the Go code that made the cgo
call*. Go's answer to question A is **its own calling frame, plus raw PC and
fault address for the foreign side** — names for its own code, numbers for the
foreign code. It does not symbolise the C function.

**C frames require an opt-in.** `runtime.SetCgoTraceback`: *"SetCgoTraceback
records three C functions to use to gather traceback information from C code and
to convert that traceback information into symbolic information."*
https://pkg.go.dev/runtime — **VERIFIED**. Naming the foreign function is an
explicitly registered extra mechanism, never the default.

**Corroborating field report, weaker source.** golang/go#63277 shows real Go
1.21.1 output: `SIGSEGV: segmentation violation PC=0x46bd10 m=0 sigcode=1` /
`signal arrived during cgo execution`, with the Go-side chain through `cgocall`
and **no C function name**. https://github.com/golang/go/issues/63277
**PARTIALLY VERIFIED**: the issue exists and contains that text; a user report,
not a spec. Illustration only.

## Item 1b — Rust, on a segfault inside `extern "C"`: **names nothing, re-raises.** VERIFIED

`library/std/src/sys/pal/unix/stack_overflow.rs`. The handler is installed for
`SIGSEGV` and `SIGBUS` only. When the faulting address is **not** in the guard
page it un-registers itself and returns so the original signal is delivered
again (`action.sa_sigaction = SIG_DFL; sigaction(signum, &action, …)`). When it
**is** a guard-page hit: `rtprintpanic!("\nthread '{name}' ({tid}) has overflowed
its stack\n")` then `rtabort!("stack overflow")`.
https://raw.githubusercontent.com/rust-lang/rust/master/library/std/src/sys/pal/unix/stack_overflow.rs
**VERIFIED.** Two findings. (i) A null read inside an `extern "C"` callee is not a
guard-page hit, so Rust deliberately says **nothing at all** and dies by signal —
the exact state the brief's Windows row shows, shipped on purpose by a
memory-safety-first language. That does not make Heroes' Windows row acceptable
(`spec § 6` and design.md §1.12 are Heroes' own commitments, not Rust's), but the
silent-death route is a *chosen* design somewhere mainstream, not only an
accident. (ii) Even in the case it does handle, Rust names a **thread**, not a
function — no frame, no symbol, no backtrace on that path.

## Item 2 — Is naming only the foreign function ever chosen? **No precedent found; the negative is search-limited.** UNVERIFIED (negative)

Of the two systems sourced: Go names its own frame by explicit design and gives
the foreign side as hex; Rust names nothing. I found **no** language that
deliberately names only the foreign frame. Per CL-018 this is a claim about my
vocabulary, not about the world: I searched Go and Rust primary sources only, and
did not examine Python (`faulthandler`), CPython C extensions, Java's
`hs_err_pid` crash log, .NET, Erlang, Swift or Zig. **Java's `hs_err_pid` is the
most likely counter-example** — it prints "Problematic frame: C [libfoo.so+0x…]",
the native frame first — and I did **not** verify it. Highest-value follow-up;
see `condition`.

## Item 5 — Windows `ExceptionInformation[1]`. **Documented; reading `[1]` is correct.** VERIFIED

`EXCEPTION_RECORD` (winnt.h), Microsoft Learn, on `EXCEPTION_ACCESS_VIOLATION`:
*"The first element of the array contains a read-write flag that indicates the
type of operation that caused the access violation. If this value is zero, the
thread attempted to read the inaccessible data. If this value is 1, the thread
attempted to write to an inaccessible address. If this value is 8, the thread
caused a user-mode data execution prevention (DEP) violation. **The second array
element specifies the virtual address of the inaccessible data.**"* Also, of the
array in general: *"For most exception codes, the array elements are undefined"*,
and `NumberParameters` is *"the number of defined elements in the
ExceptionInformation array"* — so a handler should check `NumberParameters >= 2`
before reading `[1]`.
https://learn.microsoft.com/en-us/windows/win32/api/winnt/ns-winnt-exception_record
**VERIFIED.** `ExceptionAddress` is separately documented as *"The address where
the exception occurred"* — the PC — which confirms that the existing
`ExceptionAddress == 0` arm catches a null *call*, not a null *read*. The same
table also documents `[0] == 0` as a **read** and `1` as a **write**, information
the POSIX side does not get from `si_addr` at all.

## Item 6 — a handler green on two platforms, silent on a third

**NOT ATTEMPTED.** Stopped at the search budget. No claim.

## argument

Two standards settle more than expected. C §7.22.4.1 makes flush-on-abort
implementation-defined; POSIX Issue 8 *downgraded* its own "shall flush" to
"may", in writing, because `abort()` must be async-signal-safe; §2.4.3 excludes
`fflush` from the safe list and includes `write`. glibc shipped flushing for
decades, removed it at 2.27 citing deadlock and data corruption; FreeBSD keeps it
under an `XXX` admitting the conflict. So the macOS/Linux divergence is two legal
answers, not one bug. On naming, Go switches goroutines *specifically* to blame
the Go caller of the cgo call and gives the foreign side as hex; C symbols need
`SetCgoTraceback`. Rust says nothing at all. Foreign-frame-only naming: no
precedent found.

## falsifiable predictions

1. **Flush.** If the repair calls `fflush(NULL)` — or any stdio function — from
   the SIGSEGV handler, a fixture that faults *while inside a stdio call* (a C
   callback faulting mid-`printf`, so the stream lock is held) will **deadlock or
   double-fault** on glibc rather than print. Observable: the test hangs to
   timeout, or exits 139/134 with truncated or duplicated output. Falsified if
   such a fixture prints cleanly on glibc across repeated runs. This is the
   concrete shape behind glibc's "deadlocks and data corruption".
2. **Naming.** The arm64 walk returning `node_value` is predicted to be a
   *symbolisation/frame-pointer* failure, not a platform law: `dladdr` resolves
   only symbols the dynamic linker can see, and a leaf C callee on arm64 need not
   establish a frame pointer. Observable: building the fixture's C with
   `-fno-omit-frame-pointer`, and ensuring `h_main_main` is dynamically visible,
   makes the macOS walk return `main.main`, matching Linux. Falsified if the walk
   still returns `node_value` under both conditions — which would make R2's
   "legitimate platform answer" reading much stronger. **UNRUN by me**; the
   command belongs to the ffi or compiler seat.
3. **Windows.** A third arm testing `ExceptionCode == EXCEPTION_ACCESS_VIOLATION
   && NumberParameters >= 2 && ExceptionInformation[1] < HERO_NULL_WINDOW` is
   predicted to fire on this fixture, because the docs say `[1]` is the touched
   address and the fixture touches 0. Falsified if the box shows
   `NumberParameters < 2` or a non-zero `[1]`.

## what I could NOT verify

- The **verbatim ISO C** sentence of §7.22.4.1 (clause number verified, exact
  wording not). Two mirrors truncated.
- **Apple's Libc `abort.c`** (404). macOS flush behaviour rests on the
  coordinator's measurement plus the FreeBSD lineage, not on an Apple source.
- **Item 2's negative** beyond Go and Rust. Java's `hs_err_pid` "Problematic
  frame: C [lib…]" is the likely counter-example and is unverified.
- **Item 6** entirely.
- Nothing here was run on a machine by me; every number is quoted from a
  document, and the platform rows remain the coordinator's measurements.

## condition

I would change my reading of item 2 — and withdraw the narrow objection — if
someone produces a primary source showing a language that, **by design and not by
fallback**, names only the foreign frame in a crash inside foreign code. First
place to look: HotSpot's `hs_err_pid` crash log, whose "Problematic frame" line
appears to name the native frame; if its documentation says that is the *intended*
blame line rather than the top of a stack that happens to be native, the precedent
for R1's `node_value` answer exists and my objection falls. Python's
`faulthandler` and .NET's crash dumps are next.

I would also revise item 3 if the exact ISO §7.22.4.1 text turns out to *require*
something stronger than implementation-defined — POSIX's own rationale makes that
unlikely, but I did not read the ISO sentence myself.

## sources

- https://pubs.opengroup.org/onlinepubs/9799919799/functions/abort.html
- https://pubs.opengroup.org/onlinepubs/9799919799/functions/V2_chap02.html
- https://en.cppreference.com/w/c/program/abort
- https://man7.org/linux/man-pages/man3/abort.3.html
- https://raw.githubusercontent.com/freebsd/freebsd-src/main/lib/libc/stdlib/abort.c
- https://go.dev/src/runtime/signal_unix.go?m=text
- https://pkg.go.dev/runtime
- https://github.com/golang/go/issues/63277
- https://raw.githubusercontent.com/rust-lang/rust/master/library/std/src/sys/pal/unix/stack_overflow.rs
- https://learn.microsoft.com/en-us/windows/win32/api/winnt/ns-winnt-exception_record

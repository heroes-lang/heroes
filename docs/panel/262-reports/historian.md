# Panel 262, the historian's report

Written as it goes, 2026-11-26 from about 22:00, stopped by the usage limit and resumed at the coordinator's word after
22:15 from this file as it stood (sections 1 and 2 written before the stop, kept; one sentence of § 1 corrected after
its page was opened). Seat: historian (advisory, no veto). Briefs read whole: `docs/panel/262-briefs/00-shared.md` and
`docs/panel/262-briefs/historian.md`. **Every precedent below is a page read on the web on 2026-11-26**, quoted, with
its URL; anything not confirmed by a page read is marked `unverified`. Each line says what the page states, never what
Heroes would do about it; where this report draws a reading of its own it says *reading*. The fetch tool returns
quotations in pieces of under 125 characters; pieces given one after another in italics are verbatim and consecutive
as the tool returned them, and `...` marks words the tool left out. Two PDFs (ISO/IEC Directives Part 2, the Oberon
report) were read as page images the fetch tool cached outside the repository; nothing was saved by this seat but this
report. No claim below rests on *Heroes of code*; it was not consulted in this sitting.

## 1. POSIX (IEEE Std 1003.1-2024, Issue 8, and its earlier issues)

**Edition.** The page header reads *"The Open Group Base Specifications Issue 8"* and *"IEEE Std 1003.1-2024"*
(https://pubs.opengroup.org/onlinepubs/9799919799/functions/V2_chap02.html, read 2026-11-26): the edition the Open
Group serves; whether a later one exists, `unverified`.

**(rule) What a handler may call, XSH 2.4.3 Signal Actions.** For a multi-threaded process, or a single-threaded one
whose handler runs other than as the result of *"The process calling abort(), raise(), kill(), pthread_kill(), or
sigqueue() to generate a signal that is not blocked"* or *"A pending signal being unblocked and being delivered before
the call that unblocked it returns"*, *"the behavior is undefined if:"* *"The signal handler refers to any object other
than errno with static or thread storage duration"* *"that is not a lock-free atomic object, and not a non-modifiable
object"* *"other than by assigning a value to an object declared as volatile sig_atomic_t"*, or *"The signal handler
calls any function or function-like macro defined in this standard"* *"other than one of the functions and macros
specified below as being async-signal-safe."* Then: *"All other functions (including generic functions) and
function-like macros may be unsafe with respect to signals."* And: *"In the presence of signals, all functions defined
by this volume of POSIX.1-2024 shall behave as defined"* *"when a signal interrupts an unsafe function or function-like
macro, or equivalent"* *"and the signal-catching function calls an unsafe function or function-like macro, the behavior
is undefined."* (same URL, read 2026-11-26). **(scope)** a **list of safe calls** (the table), and a **property of the
interrupted code** (*"interrupts an unsafe function"*). **(word)** *undefined*; and *"may be unsafe"* for the unlisted
functions, a *may* of possibility. Where XBD defines *undefined*: **unread**. `verified`.

**`longjmp` and `siglongjmp` from a handler, today.** Both are in the async-signal-safe table; `_longjmp` is not (the
tool's reading of the table, same URL). The table's note: *"Note that, although there is no restriction on the calls
themselves, for certain functions there are restrictions on subsequent behavior after the function is called from a
signal-catching function (see longjmp())."* The `longjmp()` page, DESCRIPTION: *"Although longjmp() is an
async-signal-safe function, if it is invoked from a signal handler which interrupted a "* *"non-async-signal-safe
function or equivalent (such as the processing equivalent to exit() performed after a return "* *"from the initial call
to main()), the behavior of any subsequent call "* *"to a non-async-signal-safe function or equivalent is undefined."*
APPLICATION USAGE: *"It is recommended that applications do not call longjmp() or siglongjmp() from signal handlers."*
*"To avoid undefined behavior when calling these functions from a signal handler, "* *"the application needs to ensure
one of the following two things:"* *"1. After the call to longjmp() or siglongjmp() the process only calls
async-signal-safe functions "* *"and does not return from the initial call to main()."* *"2. Any signal whose handler
calls longjmp() or siglongjmp() is blocked during every call "* *"to a non-async-signal-safe function, and no such calls
are made after returning from the initial call to main()."* RATIONALE and FUTURE DIRECTIONS: *"None."*
(https://pubs.opengroup.org/onlinepubs/9799919799/functions/longjmp.html, read 2026-11-26). **(scope)** a property
(*what the handler interrupted*); the consequence falls on **every later call**, not on the jump. **(word)** *undefined*,
for the later calls; the jump itself is permitted. `verified`.

**(history) The nested-handler rule, and its removal.** The Issue 6 (2004) `longjmp` page: *"As it bypasses the usual
function call and return mechanisms,"* *"longjmp() shall execute correctly in contexts of interrupts, signals, and any of
their associated functions."* *"However, if longjmp() is invoked from a nested signal handler"* *"(that is, from a
function invoked as a result of a signal raised during the handling of another signal), the behavior is undefined."*
(https://pubs.opengroup.org/onlinepubs/009695399/functions/longjmp.html, read 2026-11-26). **Austin Group defect
0000516, *"Calling longjmp() from a signal handler"***, submitted 2011-11-24, *"Accepted As Marked"*, interpretation
approved 2012-08-30; its accepted text says *"At page 489 line 16722-16755 section 2.4.3 add longjmp() and siglongjmp()
to the table of async-signal-safe functions"* and brings the sentence *"Although longjmp() is an async-signal-safe
function"*; that it removes the nested-handler restriction is the tool's summary of the page, not a quotation
(https://www.austingroupbugs.net/view.php?id=516, read 2026-11-26). The Issue 7 2018 page's CHANGE HISTORY: *"POSIX.1-2008,
Technical Corrigendum 2, XSH/TC2-2008/0202 [516] is applied."*
(https://pubs.opengroup.org/onlinepubs/9699919799/functions/longjmp.html, read 2026-11-26). **Reading**: the rule moved
from a bound on **the jump** (one level of handler) to a bound on **what follows it** (every later unsafe call), by a
property of the interrupted code; and the 2024 text still lets a program that keeps to its two conditions continue
after the jump. `verified`.

**`makecontext`, `swapcontext`, `getcontext`, `setcontext`: obsolescent in 2004, removed in 2008.** Issue 6 (IEEE Std
1003.1, 2004 Edition) marks the SYNOPSIS *"[OB XSI]"*; its RATIONALE: *"the ISO C standard (Subclause 6.11.6) specifies
that the use of function declarators with empty parentheses"* *"is an obsolescent feature."* *"a strictly conforming
POSIX application cannot use this form."* *"There are very few applications today that use the"* context routines,
and *"those that do use them are almost always using them to implement co-routines."* APPLICATION USAGE: *"The
obsolescent functions getcontext(), makecontext(), and swapcontext() can be replaced using POSIX threads functions."*
CHANGE HISTORY: *"IEEE Std 1003.1-2001/Cor 2-2004, item XSH/TC2/D6/57 is applied, obsoleting the functions"* *"and
giving advice on the alternatives."* The page says **nothing** about calling them from a handler
(https://pubs.opengroup.org/onlinepubs/009695399/functions/makecontext.html, read 2026-11-26). The Issue 6
`getcontext` page: *"The effects of passing a ucp argument obtained from any other source are unspecified."*;
APPLICATION USAGE: *"When a signal handler is executed, the current user context is saved and a new context is
created."* *"If the thread leaves the signal handler via longjmp(), then it is unspecified whether the context at the
time"* *"of the corresponding setjmp() call is restored and thus whether future calls to getcontext() provide"* *"an
accurate representation of the current context, since the context restored by longjmp() does not"* *"necessarily
contain all the information that setcontext() requires."* *"Signal handlers should use siglongjmp() or setcontext()
instead."* ... *"Use of contexts to create alternate stacks is not defined by this volume of IEEE Std 1003.1-2001."*
CHANGE HISTORY, Issue 5: *"The following sentence was removed from the DESCRIPTION: "If the ucp argument was passed to
a signal"* *"handler, program execution continues with the program"* *"instruction following the instruction
interrupted by the signal.""* (https://pubs.opengroup.org/onlinepubs/009695399/functions/getcontext.html, read
2026-11-26). **Removal in 2008**: Linux's `makecontext(3)`, HISTORY: *"Removed in POSIX.1-2008, citing portability
issues,"* *"recommending that applications be rewritten to use POSIX threads instead."*
(https://man7.org/linux/man-pages/man3/makecontext.3.html, read 2026-11-26); NetBSD's `swapcontext(3)`, STANDARDS:
*"The IEEE Std 1003.1-2004 (``POSIX.1'') revision marked the functions makecontext() and swapcontext() as obsolete,"*
*"citing portability issues and recommending the use of POSIX threads instead."* *"The IEEE Std 1003.1-2008
(``POSIX.1'') revision removed the functions from the specification."*; HISTORY: *"The makecontext() and swapcontext()
functions first appeared in AT&T System V.4 UNIX."* (https://man.netbsd.org/swapcontext.3, read 2026-11-26).
`verified` for 2004 (the standard's own page) and for 2008 (two systems' manuals); **the standard's own words for the
2008 removal `unverified`** (searched: `POSIX.1-2008 removed makecontext swapcontext getcontext obsolescent "Issue 7"
removed functions`; no Open Group page of Issue 7 naming the removal was opened).

**What the C libraries' manuals say of leaving a handler by a context** (not the standard). Linux `getcontext(3)`,
DESCRIPTION: *"If the context was obtained by a call to a signal handler, then old standard text says that "*
*""program execution continues with the program instruction following the instruction interrupted by the signal"."*
*"However, this sentence was removed in SUSv2, and the present verdict is "the result is unspecified"."* NOTES: *"When
a signal occurs, the current user context is saved and a new context is created by the kernel for the"* *"signal
handler."* *"Do not leave the handler using longjmp(3): it is undefined what would happen with contexts."* *"Use
siglongjmp(3) or setcontext() instead."* (https://man7.org/linux/man-pages/man3/getcontext.3.html, read 2026-11-26).
The glibc manual, *System V contexts*: *"If the context was created by a call to a signal handler or from any other
source"* *"then the behaviour of `setcontext` is unspecified."*; and, of its coroutine example: *"It is not recommended
to do the context switching from the signal handler directly since leaving the signal handler"* *"via `setcontext` if
the signal was delivered during code that was not asynchronous signal safe could lead to problems."* *"Setting a
variable in the signal handler and checking it in the body of the functions"* *"which are executed is a safer
approach."* (https://sourceware.org/glibc/manual/latest/html_node/System-V-contexts.html, read 2026-11-26; no version
shown on the page). `verified`.

**`sigaltstack`.** POSIX 2024: *"Use of this function by library threads that are not bound to kernel-scheduled
entities results in undefined behavior."* APPLICATION USAGE: *"On some implementations, stack space is automatically
extended as needed."* *"On those implementations, automatic extension is typically not available for an alternate
stack."* *"If the stack overflows, the behavior is undefined."*; nothing on leaving a handler on the alternate stack by
a jump (https://pubs.opengroup.org/onlinepubs/9799919799/functions/sigaltstack.html, read 2026-11-26). Linux's manual:
*"Functions called from a signal handler executing on an alternate signal stack will also use the alternate signal
stack."* *"Exceeding the allocated size of the alternate signal stack will lead to unpredictable results."*; and
*"SS_AUTODISARM (since Linux 4.7)"* *"Clear the alternate signal stack settings on entry to the signal handler."* *"When
the signal handler returns, the previous alternate signal stack settings are restored."* *"This flag was added in
order to make it safe to switch away from the signal handler with swapcontext(3)."* *"Without this flag, a subsequently
handled signal will corrupt the state of the switched-away signal handler."*
(https://man7.org/linux/man-pages/man2/sigaltstack.2.html, read 2026-11-26). `verified`.

**`timer_create` with `SIGEV_THREAD`** (question 8's shape), XSH 2.4.2: *"the function sigev_notify_function is called
with parameter sigev_value."* *"The function shall be executed in a newly created thread as if it were the start_routine
for a call to pthread_create"* *"with the thread attributes specified by sigev_notify_attributes."* *"Supplying an
attributes structure with a detachstate attribute of PTHREAD_CREATE_JOINABLE results in undefined behavior."* *"It is
implementation-defined whether the signal mask of this thread has all signals except SIGKILL and SIGSTOP blocked,"*
(https://pubs.opengroup.org/onlinepubs/9799919799/functions/V2_chap02.html, read 2026-11-26). `verified`.

## 2. C (ISO/IEC 9899), and its history through WG14

**Source used**: N1570, the committee draft of C11 (https://port70.net/~nsz/c/c11/n1570.html), and N1256, C99 with
TC3 (https://port70.net/~nsz/c/c99/n1256.html), both read 2026-11-26. The published standards are paid documents and
were **not read**; that N1570's text is C11's is `unverified` here. **C17 and C23: unread, `unverified`.**

**(rule) 7.14.1.1 The signal function**, N1570 p5: *"If the signal occurs other than as the result of calling the abort
or raise function, the behavior is undefined"* *" if the signal handler refers to any object with static or thread
storage duration that is not a lock-free atomic object"* *" other than by assigning a value to an object declared as
volatile sig_atomic_t, or the signal handler calls any"* *" function in the standard library other than the abort
function, the _Exit function, the quick_exit function,"* *" or the signal function with the first argument equal to the
signal number"* *" corresponding to the signal that caused the invocation of the handler."* Footnote 252: *"If any
signal is generated by an asynchronous signal handler, the behavior is undefined."* p3: *"If and when the function
returns, if the value of sig is SIGFPE, SIGILL, SIGSEGV, or any other implementation-defined"* *" value corresponding
to a computational exception, the behavior is undefined;"* *" otherwise the program will resume execution at the point
it was interrupted."* p7: *"Use of this function in a multi-threaded program results in undefined behavior."* N1256's
p5 names *abort*, *_Exit* and *signal* and says *"static storage duration"*; N1570 adds *quick_exit*, *thread storage
duration* and the lock-free atomic exception (both read 2026-11-26). **(scope)** a list of safe calls, and the
asynchronous/synchronous split (*abort* or *raise*). **(word)** *undefined*. `verified` (for the drafts).

**(rule) 7.13.2.1 The longjmp function.** In N1570 and N1256 it has five paragraphs, the last an EXAMPLE, and **neither
says anything of interrupts, signals or a nested signal handler** (the tool's reading, keyword-checked: *"interrupts"*
and *"signals"* absent; *"nested"* only in a footnote's *"the set of nested calls"*). p2 ends *"... modified type and
execution has left that scope in the interim, the behavior is undefined."* `verified` (for the drafts).

**(history) What C promised, how WG14 read it, and how it left.** WG14 N1318, *"longjmp() from signal handler"*, Fred
Tydeman (USA), 2008-07-13: C89 4.6.2.1's third paragraph read *"As it bypasses the usual function call and return
mechanisms, the longjmp function shall execute correctly in contexts of interrupts, signals and any of their associated
functions."*; *"it is missing in C99 7.13.2.1, which is an undocumented quiet change"*; *"To keep this job manageable,
the C89 Committee agreed to restrict longjmp to only one level of signal handling."*; *"N789 is the C99 proposal that
removed the paragraph from longjmp about one level of signal handling."*; and *"longjmp() and siglongjmp() are not in
the async-signal-safe functions list in XSH 2.4.3"* (true in 2008; § 1's defect 516 changed it)
(https://open-std.org/jtc1/sc22/wg14/www/docs/n1318.htm, read 2026-11-26). **C90 issue 0152** (Defect Report 152),
*"Can longjmp be used to return from a signal handler invoked other than through abort or raise?"*, Jutta Degener, DIN,
1995-06-11; WG14's response, 1997-09-23: *"The C Standard is clear enough as is."* *"The longjmp function shall execute
correctly when called from a non-nested signal handler"* *"invoked through calls to the raise or abort functions;"*
*"if longjmp is called from a signal handler invoked by other means, or from a nested signal handler,"* *"the behavior
is undefined."* (https://open-std.org/jtc1/sc22/wg14/issues/c90/issue0152.html, read 2026-11-26). `verified`.

**C's words for a permitted outcome**, N1570 3.4: **3.4.1 implementation-defined**, *"unspecified behavior where each
implementation documents how the choice is made"*; **3.4.3 undefined**, *"behavior, upon use of a nonportable or
erroneous program construct or of erroneous data,"* *"for which this International Standard imposes no requirements"*,
NOTE *"Possible undefined behavior ranges from ignoring the situation completely with unpredictable results,"* ... *"to
terminating a translation or execution"*; **3.4.4 unspecified**, *"... two or more possibilities and imposes no further
requirements on which is chosen in any instance"*. Clause 4 p2: *"Undefined behavior is otherwise indicated in this
International Standard by the words"* *"undefined behavior" or by the omission of any explicit definition of
behavior."* (N1570, read 2026-11-26). **Reading** (the brief's question): by C's definitions a sentence naming exactly
two outcomes, *runs* or *aborts saying X*, with no requirement on which, has the shape of **unspecified** behaviour, not
of undefined (no requirements at all); a text that also says where the choice is documented has the shape of
**implementation-defined**. C's definitions say nothing of any other document. `verified` (the definitions).

## 3. Rust

**Panic out of `extern "C"`.** RFC 2945 (*"C-unwind" ABI*), start date *"2019-04-03"*: a plain old frame (POF) is
*"a frame that can be trivially deallocated"*; platform features relying on forced unwinding *"will always be considered
undefined behavior if they cross non-POFs"*; *"Crossing only POFs is necessary but not sufficient, however, to make
forced unwinding safe."*; for POFs *"this is not specified, and must be considered undefined behavior"* for now; a
non-forced foreign unwind entering Rust through an `extern "C"` declaration *"is always undefined behavior, and is not
guaranteed to cause the program to abort"* under `panic=abort`; the RFC's table defines *"“UB” stands for undefined
behavior."* (https://rust-lang.github.io/rfcs/2945-c-unwind-abi.html, read 2026-11-26). Rust 1.81.0, *"Sept. 5,
2024"*, heading *"Abort on uncaught panics in `extern "C"` functions"*: *"This completes the transition started in
1.71"* *"which added dedicated "C-unwind" (amongst other -unwind variants) ABIs"* *"As of 1.81, the non-unwind ABIs (e.g.,
"C") will now abort on uncaught unwinds,"* *"closing the longstanding soundness problem."*; the announcement does not
say *undefined behavior* of what came before (https://blog.rust-lang.org/2024/09/05/Rust-1.81.0/, read 2026-11-26).
`verified`.

**`longjmp` over Rust frames.** The Reference, *Behavior considered undefined*: *"The runtime assumes that a Rust stack
frame is not deallocated without executing destructors for local variables owned by the stack frame."* *"This
assumption can be violated by C functions like `longjmp`."*; and of the list itself: *"The following list is not
exhaustive; it may grow or shrink."* *"There is no formal model of Rust's semantics for what is and is not allowed in
unsafe code, so there may be more behavior considered unsafe."*; the page does not mention signals, signal handlers or
stack switching (https://doc.rust-lang.org/reference/behavior-considered-undefined.html, read 2026-11-26). **What the
standard library or the Reference says a signal handler may do: not found** (that page only; `std` docs and the
`signal-hook` crate not opened), `unverified`. **(word)** *undefined behavior*, by a list declared not exhaustive; and,
since 1.81, **a definite *abort*** for an unwind leaving a non-unwind ABI. `verified`.

## 4. Zig

**Not found.** Searched `ziglang github issue setjmp longjmp support "setjmp" zig language`; the results held a forum
thread (https://ziggit.dev/t/alternative-or-proper-way-to-use-cs-setjmp-and-longjmp/3055, read 2026-11-26) whose
replies are by users the page does not identify as Zig's team (one: *"Setjmp/longjmp are included in musl, which is
included in Zig, so you should be able to access them."*). Zig's language reference and `std.posix.sigaction`'s
documentation were **not opened**. Everything the brief asks of Zig is `unverified`.

## 5. Go: cgo, `os/signal`, and golang/go issue 62440

**`cmd/cgo`**: the page says *"Go functions can be exported for use by C code in the following way:"*, and nothing of
threads Go did not create, of signal handlers, of stack switching or of `longjmp` (the tool's keyword reading); it warns
that *"programs that break these rules are likely to fail in unexpected and unpredictable ways."*, and of `#cgo
nocallback` that *"the program will panic"* if the C function calls back into Go (https://pkg.go.dev/cmd/cgo, read
2026-11-26). `verified`.

**`os/signal`**: *"If the non-Go code installs any signal handlers, it must use the SA_ONSTACK flag with sigaction."*
*"Failing to do so is likely to cause the program to crash if the signal is received."* *"Go programs routinely run
with a limited stack, and therefore set up an alternate signal stack."*; for synchronous signals non-Go code *"should
record the existing Go signal handler."* *"Otherwise some Go run-time panics will not occur as expected."*; *"The non-Go
code should not change the signal mask on any threads created by the Go runtime."* *"If the non-Go code starts new
threads itself, those threads may set the signal mask as they please."*; and, as the tool summarised a sentence too long
to quote: if the non-Go code changes no handlers or masks, the behaviour is the same as for a pure Go program
(https://pkg.go.dev/os/signal, read 2026-11-26). **(scope)** a property (*changes no signal handlers or masks*) for the
promise, and a flag (`SA_ONSTACK`) for the warning. **(word)** *"likely to cause the program to crash"*. `verified`.

**golang/go 62440**, *"runtime: "fatal: morestack on g0" on amd64 after upgrade to Go 1.21"*, opened 2023-09-04,
closed 2024-05-16 by commit 1ffc296, milestone Go1.23; the tool's summary: a Fluent Bit plugin whose C coroutine
library switches stacks crashed under 1.21, bisected to CL 495855, which cached the stack bounds of the first callback.
prattmic: *"as the reason this case is suddenly causing problems, even though this use case was already unsupported."*
cherrymui: *"For cgo in general, we probably never really thought about what we support or not if C code switches
stacks."* *"If there is no Go frame at all when the C code switches stacks, it is probably fine."*
(https://github.com/golang/go/issues/62440, read 2026-11-26). The commit, *"runtime: always update stack bounds on
cgocallback"*: *"callbackUpdateSystemStack contains a fast path to exit early without update if SP is already within
the g0.stack bounds."* *"This is not safe, as a subsequent call may have new stack bounds that only partially overlap
the old stack bounds."* *"Fixes #62440."*; in the diff's comment: *"It is not safe for the C call to change the stack
out from under us."* *"we need to update the stack bounds even if SP appears to already be in bounds."*
(https://github.com/golang/go/commit/1ffc296, read 2026-11-26). Backports to release-branch.go1.21 and go1.22 on
2024-06-24, per the tool's reading of the issue page; the point releases carrying them: `unverified`. **What Go
promises a callback after an unseen switch: nothing written** (the two pages read); the runtime was changed to survive
it. `verified`.

## 6. The signal stack trick: Engelschall's paper, GNU Pth, libcoro, QEMU

**The paper is real.** Ralf S. Engelschall, *"Portable Multithreading: The Signal Stack Trick For User-Space Thread
Creation"*, 2000 USENIX Annual Technical Conference, San Diego, June 18-23, 2000, pp. 239-250
(https://www.usenix.org/legacy/event/usenix2000/general/full_papers/engelschall/engelschall_html/index.html, read
2026-11-26). **Its algorithm, `mctx_create`, step by step as the paper numbers it** (opening words verbatim, then the
paper's words where the tool quoted them):

1. *"Preserve the current signal mask and block..."* an arbitrary worker signal (the paper's code uses `SIGUSR1`).
2. *"Preserve a possibly existing signal action..."*, and *"configure a trampoline function as the new temporary
   signal action."*
3. *"Preserve a possibly active alternate signal stack..."*, and *"configure the memory chunk starting at sk_addr as
   the new temporary alternate signal stack."*
4. *"Save parameters for the trampoline step..."*; then *"send the current process the worker signal, temporarily
   unblock it"* so the handler runs on the new stack.
5. *"After the trampoline function asynchronously entered..."*: *"save its machine context in the mctx structure and
   immediately return from it to terminate the signal handler scope."* The code: `if (mctx_save(mctx_creat) == 0) {
   mctx_called = TRUE; return; }`. **The handler returns.**
6. *"Restore the preserved alternate signal stack, preserved signal action and preserved signal mask for worker
   signal."* The code disables the temporary stack (`ss.ss_flags = SS_DISABLE`) and restores the old one only where it
   was active: `if (!(oss.ss_flags & SS_DISABLE)) sigaltstack(&oss, NULL);`, then `sigaction(SIGUSR1, &osa, NULL)` and
   `sigprocmask(SIG_SETMASK, &osigs, NULL)`.
7. *"Save the current machine context of mctx_create..."*
8. *"Restore the previously saved machine context of the trampoline function..."*, *"to again transfer execution control
   onto the alternate stack."* (a `longjmp` into the save made inside the handler's frame, which has returned).
9. *"After reaching the trampoline function..."*: *"immediately bootstrap into a clean stack frame by just calling a
   second function."*
10. *"Set the new signal mask to be the same as..."* the original.
11. *"Load the passed startup information..."* (`sf_addr`, `sf_arg`) into local variables.
12. *"Save the current machine context for later restoring..."*
13. *"Restore the previously saved machine context of mctx_create..."*, back to the creator.
14. *"Return to the calling application."*; later *"The thread ``magically'' starts..."* when that context is restored.

**Why it returns rather than jumps**: on some kernels, the tool's reading, the kernel would *"refuse to readjust the
signal stack, and we are stuck"*, not believing the code left signal-handling context *"until the signal handler has
returned"*, so *"it is better to get rid of the signal handler scope with the straight-forward"* return. **On calling
code from the left frame**: *"longjmp(3) may not be called after the function which called the setjmp(3) returned"*;
*"the stack frame situation is not guaranteed to be in a clean and consistent state"*; *"one can reach a safe stack frame
again by just calling a function"* (step 9). **Its own caveat**: *"our approach may theoretically still not work,
because longjmp(3) does not cooperate."*; it relies on `sigaltstack` (*"conforms to the Single Unix Specification,
Version 2"*) and `sigsetjmp`/`siglongjmp` (*"conform to IEEE Std1003.1-1988"*); the paper does not use the word
*undefined* (same URL, read 2026-11-26). `verified`.

**GNU Pth ships it.** `pth_mctx.c` names *"VARIANT 1: THE STANDARDIZED SVR4/SUSv2 APPROACH"* (`#if PTH_MCTX_MTH(mcsc)`,
`makecontext`/`swapcontext`), *"VARIANT 2: THE SIGNAL STACK TRICK"* (`#elif PTH_MCTX_MTH(sjlj)` and not the three
`jmp_buf` fiddling dispatches), and variants 3 to 5 and X (*"LINUX SPECIFIC JMP_BUF FIDDLING"*, ...); in variant 2 the
handler returns (*"have do a normal "return" here"*, `mctx_called = TRUE; return;`) and the old alternate stack is
restored by `if (!(oss.ss_flags & SS_DISABLE))` `sigaltstack(&oss, NULL);`; *"Copyright (c) 1999-2004 Ralf S.
Engelschall"*, Debian package *"pth 2.0.1-2"* (https://sources.debian.org/src/pth/2.0.1-2/pth_mctx.c, read 2026-11-26).
Which variant Pth's `configure` picks on a given system, and whether GNU Pth is maintained today: `unverified`
(gnu.org's page answered HTTP 429).

**libcoro's `CORO_SJLJ` is a second library on the trick** (the critic's recollection, now sourced). Tarantool's
bundled `coro.c`, *"Copyright (c) 2001-2011 Marc Alexander Lehmann"*: the handler is `static void trampoline (int sig)
{ if (coro_setjmp (new_coro->env)) coro_init (); /* start it */ else trampoline_done = 1; }`. `coro_create` installs
the new stack with `sigaltstack (&nstk, &ostk)`, the handler with `nsa.sa_flags = SA_ONSTACK;` on `SIGUSR2`, sends
`kill (getpid (), SIGUSR2);`, waits `while (!trampoline_done)` `sigsuspend (&nsig);`, then disables the new stack
(`nstk.ss_flags = SS_DISABLE;`) and restores the old one only where it was active, `if (~ostk.ss_flags & SS_DISABLE)`
`sigaltstack (&ostk, 0);`, the old action (`sigaction (SIGUSR2, &osa, 0);`) and mask
(https://sources.debian.org/src/tarantool/2.6.0-1.4/third_party/coro/coro.c, read 2026-11-26; a downstream copy, the
upstream file not opened). **Reading of that code**: the handler returns on its first entry, and each coroutine later
starts by a jump into the save made inside that returned handler's frame, `coro_init()` then called from it. libcoro's
own page: it works if the system implements ucontext or *"the older sigaltstack interfaces"*; it does not name Perl's
Coro (https://software.schmorp.de/pkg/libcoro.html, read 2026-11-26). **Perl's Coro picks it by default on Linux off
x86**: its `Coro/Makefile.PL` offers *"s  If the ucontext functions are not working or you don't want"*
*"setjmp/longjmp/sigaltstack (also standard unix functions)."*, setting `-DCORO_SJLJ`, and on Linux runs `$iface =
$iface_asm || "s";`, `$iface_asm` being `"a"` only where the architecture name matches `i[3456]86`, `amd64` or `x86_64`
(the tool's reading of the code); it warns *"You cannot use this implementation if some other code uses SIGUSR2 or you
plan to create"* (https://sources.debian.org/src/libcoro-perl/6.540-1/Coro/Makefile.PL/, read 2026-11-26). **Reading**:
so on Linux arm64 a default build of Perl's Coro takes the signal stack trick; that a given distribution's package was
built so is `unverified`.

**QEMU ships it, today.** `util/coroutine-sigaltstack.c` on master: *"This file is partly based on pth_mctx.c, from the
GNU Portable Threads,"*; the trampoline `if (!sigsetjmp(coTS->tr_reenter, 0)) {` ... `return;`, its comment *"We have
to return, given that this is a signal handler and the sigmask and some other things are changed."*, then on the
caller's `siglongjmp(coTS->tr_reenter, 1);` it calls `coroutine_bootstrap(self, co);` (*"We have to jump into another
function here to get a new stack context for the auto variables."*); `qemu_coroutine_new` installs the stack with
`sigaltstack(&ss, &oss)`, signals itself with `pthread_kill(pthread_self(), SIGUSR2);` and `sigsuspend(&sigs);`, then
`ss.ss_flags = SS_DISABLE;` and `if (!(oss.ss_flags & SS_DISABLE)) {` `sigaltstack(&oss, NULL);`
(https://qemu.googlesource.com/qemu/+/refs/heads/master/util/coroutine-sigaltstack.c, read 2026-11-26). QEMU's
`meson_options.txt`: *"option('coroutine_backend', type: 'combo',"* *"choices: ['ucontext', 'sigaltstack', 'windows',
'wasm', 'auto'],"* *"value: 'auto', description: 'coroutine backend to use')"*
(https://qemu.googlesource.com/qemu/+/refs/heads/master/meson_options.txt, read 2026-11-26). Which systems `auto`
resolves to `sigaltstack` on: `unverified`.

**For the ffi-pragmatist, the variant the code states** (three implementations read, the same shape): **the handler
returns** after its save (none of the three leaves it by a jump); **the library restores the previous alternate stack
only if one was active, else leaves the alternate stack disabled**, and restores the old action and mask; then **the
creator jumps (`longjmp`; `siglongjmp` in QEMU) into the save taken inside the returned handler's frame**, on the memory
that was the alternate stack, and **calls a second function** from there; by a reading of the code, every coroutine of
the library then runs in frames above that returned handler's. Pth and libcoro signal the **process**
(`kill(getpid(), ...)`; the paper's *"send the current process"*), QEMU the **thread** (`pthread_kill(pthread_self(),
...)`).

## 7. The tradition the spec is written in: the Algol 60 report and the Oberon report

**Algol 60, the Revised Report** (*"[originally] Edited by Peter Naur"*; the page's HTML is a 1999 translation of a 1990
LaTeX file): section 1's footnote, *"Whenever the precision of arithmetic is stated as being in general not specified,
or the outcome"* *" of a certain process is left undefined or said to be undefined, this is to be interpreted in the
sense"* *" that a program only fully defines a computational process if the accompanying information specifies"* *" the
precision assumed, the kind of arithmetic assumed, and the course of action to be taken in all"* *" such cases as may
occur during the execution of the computation."*; uses of *undefined*: *"...a switch designator whose value is
undefined."* (4.3.5), *"...the value of the controlled variable is undefined after the exit."* (4.6.5), *"The effect of
a go to statement, outside a for statement, which refers to a label within the for statement, is undefined."* (4.6.6),
and in 3.3.4.3 *"if a=0:  undefined."*; **one *may* for an implementation's choice**, 3.3.6: *"No exact arithmetic will
be specified, however, and it is indeed understood that different hardware"* *" representations may evaluate
arithmetic expressions differently."*; none of the *undefined* sentences uses *may*
(https://www.masswerk.at/algol60/report.htm, read 2026-11-26). **(word)** *undefined*, which the footnote defines as
*left to the accompanying information*; **reading**: C's *implementation-defined* in all but name. `verified`.

**Oberon, *The Programming Language Oberon*, Niklaus Wirth, *"Revision 1.10.2013 / 3.5.2016"***: the introduction,
*"What remains unsaid is mostly left so intentionally, either because it is derivable from stated rules of the
language, or because it would unnecessarily restrict the freedom of implementors."*; 8.1, *"The typeguard v(T0) asserts
that v is of type T0, i.e. it aborts program execution, if it is not of type T0."*; 8.2.4, *"The value of NIL IS T is
undefined."*; 9.5, the CASE statement, says nothing of a value no label holds (pages 1, 6, 8 and 10 of
https://people.inf.ethz.ch/wirth/Oberon/Oberon07.Report.pdf, read 2026-11-26 as page images). **(word)** *aborts*, its
condition stated after *if*; *undefined* left undefined; silence as a permission to implementors, said once at the top.
**No *may* for a run-time outcome** on the pages read (pages 2 to 5, 11 onward: unread). `verified` for the pages read.

## 8. How a standard defines its *may*

**ISO/IEC Directives, Part 2**, *"Edition 9.0 2021-05 REDLINE VERSION"* (a copy hosted by the Consumer Technology
Association, https://www.cta.tech/media/1ebl2xah/iso-iec-directives-part-2.pdf, read 2026-11-26 as page images; iso.org
answered HTTP 403, so whether a later edition is current is `unverified`). 3.3.5 **permission**: *"expression, in the
content of a document (3.1.1), that conveys consent or liberty (or opportunity) to do something"*; 3.3.6
**possibility**: *"expression, in the content of a document (3.1.1), that conveys expected or conceivable material,
physical or causal outcome"*. Table 5, *Permission*: *may*, *"is permitted"*, *"is allowed"*, *"is permissible"*;
beneath it: *"Do not use "possible" or "impossible" in this context."* *"Do not use "can" instead of "may" in this
context."* *"Do not use "might" instead of "may" in this context."* *""May" signifies a permission expressed by the
document, whereas "can" refers to the ability of a user of the document or to a possibility open to him/her."* and, new
in this edition (red in the redline), *"Negative permissions are ambiguous and should not be used."* Table 6,
*Possibility and capability*: *can*, *"be able to"*, *"there is a possibility of"*, *"it is possible to"*; EXAMPLE 1
*"Use of this connector in corrosive atmospheric conditions can lead to failure of the locking mechanism."*; beneath it
*"Do not use "may" instead of "can" in this context."* (pages 15, 27 and 28 of the document). `verified`.

**RFC 2119**, S. Bradner, March 1997, 5. MAY: *"This word, or the adjective "OPTIONAL", mean that an item is truly
optional."* *"One vendor may choose to include the item because a particular marketplace requires it"* ... *"An
implementation which does not include a particular option MUST be prepared to interoperate"* *"with another
implementation which does include the option, though perhaps with reduced functionality."* *"In the same vein an
implementation which does include a particular option MUST be prepared to interoperate"* *"with another implementation
which does not include the option (except, of course, for the feature the option provides.)"*
(https://www.rfc-editor.org/rfc/rfc2119, read 2026-11-26). **RFC 8174**, B. Leiba, May 2017, updating it: the old
wording *"has caused confusion about how to interpret non-capitalized words such as "must" and "should"."*; *"The words
have the meanings specified herein only when they are in all capitals."* *"When these words are not capitalized, they
have their normal English meanings and are not affected by this document."*
(https://www.rfc-editor.org/rfc/rfc8174, read 2026-11-26). `verified`.

**What each fixes** (reading, from the words above): ISO makes *may* a **permission the document grants** and requires
*can* for a **possibility**; so by ISO's rule *"a callback ... may abort"* reads as the document permitting an abort,
and the possibility reading would be written *can abort*. RFC 2119 makes *MAY* **the implementation's option** and
obliges **the other party to be prepared for both outcomes**. **Neither warns, in the pages read, that a permission is
read as a promise**; the nearest warnings are ISO's *"Do not use"* lines and its *"Negative permissions are ambiguous"*,
and RFC 8174's record of readers confusing the defined words with plain English.

## 9. The precedents the repair added

**JNI** (Java SE 21, *Invocation API*): *"The JNI interface pointer (`JNIEnv`) is valid only in the current thread."*
*"Should another thread need to access the Java VM, it must first call `AttachCurrentThread()` to attach itself to the
VM and obtain a JNI interface pointer."*; `GetEnv`: *"If the current thread is not attached to the VM, sets `*env` to
`NULL`, and returns `JNI_EDETACHED`."*; *"A native thread attached to the VM must call `DetachCurrentThread()` to detach
itself before terminating."*; what follows a call from an unattached thread: **not stated**; nothing on signal
handlers (https://docs.oracle.com/en/java/javase/21/docs/specs/jni/invocation.html, read 2026-11-26). `verified`.

**HotSpot signal chaining** (Java SE 21, *Signal Chaining*): *"Signal chaining enables you to write applications that
need to install their own signal handlers. This facility is available on Linux and macOS."* *"When the HotSpot VM is
created, the signal handlers for signals that are used by the HotSpot VM are saved."* *"The new signal handlers are
chained behind the HotSpot VM signal handlers for the signals."* `libjsig` *"ensures that calls such as `signal()`,
`sigset()`, and `sigaction()` are intercepted"* ... *"Instead, these calls save the new signal handlers."*; enabled by
linking `-ljsig` or `LD_PRELOAD` (Linux), `DYLD_INSERT_LIBRARIES` (macOS); what happens without it: **not stated**
(https://docs.oracle.com/en/java/javase/21/vm/signal-chaining.html, read 2026-11-26). `verified`.

**Lua 5.4**, 4.5 *Handling Yields in C*: *"Internally, Lua uses the C `longjmp` facility to yield a coroutine."* *"Lua
cannot return to `foo` any more, because the `longjmp` removes its frame from the C stack."* *"To avoid this kind of
problem, Lua raises an error whenever it tries to yield across an API call, except for three functions"* (which take a
continuation function); `lua_yieldk`: *"This function can raise an error if it is called from a thread with a pending C
call with no continuation function"* *"(what is called a _C-call boundary_)"* (https://www.lua.org/manual/5.4/manual.html,
read 2026-11-26). The error's words, `ldo.c`: `luaG_runerror(L, "attempt to yield across a C-call boundary");` under
`if (l_unlikely(!yieldable(L)))` (https://www.lua.org/source/5.4/ldo.c.html, read 2026-11-26). **(word)** *raises an
error*, definite, **at the switch**, with its exception (*"except for three functions"*) stated. `verified`.

**Python `signal`**: *"A Python signal handler does not get executed inside the low-level (C) signal handler."*
*"Instead, the low-level signal handler sets a flag which tells the virtual machine to execute"* *"the corresponding
Python signal handler at a later point (for example, at the next bytecode instruction)."*; *"It makes little sense to
catch synchronous errors like SIGFPE or SIGSEGV"* *"that are caused by an invalid operation in C code."*; *"Python
signal handlers are always executed in the main Python thread of the main interpreter,"* *"even if the signal was
received in another thread."* (https://docs.python.org/3/library/signal.html, read 2026-11-26). `verified`.

**libcurl's watchdog.** `CURLOPT_NOSIGNAL`: *"If onoff is 1, libcurl uses no functions that install signal handlers or
any functions that cause signals"* *"to be sent to the process."* *"This option is here to allow multi-threaded Unix
applications to still set/use all timeout options etc,"* *"without risking getting signals."* *"If this option is set
and libcurl has been built with the standard name resolver, timeouts cannot occur"* *"while the name resolve takes
place."*; DEFAULT *"0"*; *"Added in curl 7.10"*; the page names neither `SIGALRM` nor `siglongjmp`
(https://curl.se/libcurl/c/CURLOPT_NOSIGNAL.html, read 2026-11-26). The source, `lib/hostip.c` (Fuchsia's mirror of
curl's main branch): the handler `alarmfunc` is `(void)sig;` `siglongjmp(curl_jmpenv, 1);`, and the comment above it:
*"This signal handler jumps back into the main libcurl code and continues"* *"execution. This effectively causes the
remainder of the application to run"* *"within a signal handler which is nonportable and could lead to problems."*;
above `curl_jmpenv`: *"Beware this is a global and unique instance."* ... *"This is not thread-safe stuff."*; in
`Curl_resolv_timeout`: *"will generate a signal and we will siglongjmp() from that here."* *"This technique has problems
(see alarmfunc)."* (https://fuchsia.googlesource.com/third_party/curl/+/main/lib/hostip.c, read 2026-11-26). That the
interrupted call is `getaddrinfo` itself: `unverified` (the comments say *the name resolver*). **CVE-2023-28320**,
*"siglongjmp race condition"*: *"If it is built to use the synchronous resolver, it allows name resolves to time-out slow
operations"* *"using `alarm()` and `siglongjmp()`."* *"When doing this, libcurl used a global buffer that was not mutex
protected and a multi-threaded application"* *"might therefore crash or otherwise misbehave."*; *"The fix is to only
support this timeout ability if curl has and can properly mutex protect the buffer."*; affected 7.9.8 to 8.0.1, fixed
in 8.1.0, published 2023-05-17 (the tool's summary of the page for the versions and date)
(https://curl.se/docs/CVE-2023-28320.html, read 2026-11-26). `verified`.

## 10. A conditional abort worded with *unless*

**None of the pages read words one with *unless*.** The shapes they use instead: **a promise, then its exception**
(POSIX Issue 6: *"shall execute correctly in contexts of interrupts, signals, and any of their associated functions."*
*"However, if longjmp() is invoked from a nested signal handler"* ... *"the behavior is undefined."*; WG14's 1997
answer: *"shall execute correctly when called from a non-nested signal handler"* *"invoked through calls to the raise
or abort functions;"* *"if longjmp is called from a signal handler invoked by other means, or from a nested signal
handler,"* *"the behavior is undefined."*); **a definite error with a stated exception** (Lua: *"raises an error
whenever it tries to yield across an API call, except for three functions"*); **a definite abort with its condition**
(Oberon: *"it aborts program execution, if it is not of type T0"*; Rust 1.81: *"will now abort on uncaught unwinds"*);
**a permitted act, then undefined consequences keyed to a property** (POSIX 2024: *"Although longjmp() is an
async-signal-safe function, if it is invoked from a signal handler which interrupted a"* ...); **a likelihood** (Go:
*"likely to cause the program to crash"*; curl: *"might therefore crash or otherwise misbehave"*; Linux: *"will lead to
unpredictable results"*). Only the pages named in this report were read for it.

## The table

| precedent | what a callback or handler may do | how the class is bounded | the word for the permitted outcome | defined in the document |
|---|---|---|---|---|
| POSIX 2024, XSH 2.4.3 | call async-signal-safe functions; others undefined when the handler interrupted an unsafe one | a list of safe calls + a property of the interrupted code | *undefined* | not read where |
| POSIX 2024, `longjmp()` | jump out (safe); every later unsafe call undefined if the handler interrupted unsafe code | a property, applied after the jump | *undefined* | not read where |
| POSIX Issue 6 (until TC2 of 2008, 2012) | `longjmp` *"shall execute correctly"*, except from a nested handler | a depth (one level) | *undefined* | not read where |
| POSIX Issue 6, `getcontext` | leaving by `longjmp`: context restoration *unspecified*; contexts for alternate stacks *"not defined"* | a mechanism | *unspecified*, *not defined* | not read where |
| C90 + DR 152 (1997) | `longjmp` correct from a non-nested handler invoked by `raise` or `abort` only | a list of origins + a depth | *undefined* | C90's definitions not read (C11's: 3.4.3) |
| C11 (N1570) 7.14.1.1 | call `abort`, `_Exit`, `quick_exit`, `signal(sig)`; touch `volatile sig_atomic_t` and lock-free atomics | a list of safe calls; async vs `raise`/`abort` | *undefined* | yes, 3.4.3 |
| Rust 2945 + 1.81 | an unwind out of `extern "C"` aborts; forced unwinds and `longjmp` over non-POFs UB | a property of frames (POF) and an ABI | *UB*; *abort* | *"“UB” stands for undefined behavior"*; list *"not exhaustive"* |
| Go `os/signal` | C's handlers need `SA_ONSTACK`; with no handler or mask change, behaviour as pure Go | a property (changes handlers or masks) + a flag | *"likely to cause the program to crash"* | no |
| Go 62440 | (nothing written) runtime refreshes stack bounds at every callback | none written | *"already unsupported"* (a comment) | no |
| Signal stack trick (paper, Pth, libcoro, QEMU) | handler saves and returns; coroutines start by a jump into its returned frame, then call a function | a mechanism (a step list) | *"may theoretically still not work"* | no |
| Lua 5.4 | no yield across a C call without continuation | a property (pending C call) + 3 named exceptions | *raises an error* | the error's words in the source |
| Python `signal` | the Python handler runs later, in the main thread, never in the C handler | a thread + a deferral | (no permitted outcome; a rule) | n/a |
| JNI | a native thread must attach before using the VM | a thread | not stated | no |
| HotSpot `libjsig` | C's handlers are chained behind the VM's when the library is loaded | platforms (Linux, macOS) + interposition | not stated | no |
| libcurl | `alarm` + `siglongjmp` runs *"the remainder of the application"* inside a handler | a build option + `CURLOPT_NOSIGNAL` | *"could lead to problems"*, *"might ... crash"* | no |
| Algol 60 | (no handlers) outcomes *undefined* = left to *"the accompanying information"* | per construct | *undefined*; one *may* (arithmetic) | yes, section 1's footnote |
| Oberon 2016 | (no handlers) a failed type guard aborts | a stated condition | *aborts*; *undefined* | no; silence declared intentional |
| ISO Directives Part 2 (2021) | (drafting rule) *may* = permission by the document, *can* = possibility | n/a | *may* / *can* | yes, 3.3.5, 3.3.6, Tables 5 and 6 |
| RFC 2119 / 8174 | (drafting rule) *MAY* = truly optional; the counterpart must be prepared for both | n/a | *MAY* | yes; lowercase plain English since 8174 |

## Which precedent each candidate of `00-shared.md` § 4 rests on

- **(a) two sentences by kind**: POSIX keeps signals (XSH 2.4.3) and contexts (the `makecontext` pages) apart and states
  their meeting only as *unspecified* (`getcontext`; glibc's `setcontext`); Lua states the **switch** rule in a sentence
  of its own, definite and with its exceptions. Linux's `SS_AUTODISARM` and glibc's manual are the sources read that
  name the two together (*"switch away from the signal handler with swapcontext"*; *"context switching from the signal
  handler"*).
- **(b) the property**: POSIX's move by defect 516 (2012) from a depth to a property of the interrupted code; Go's
  promise keyed to *"does not change any signal handlers or masks"*; Rust's POF.
- **(c) the sentence plus a clause**: POSIX Issue 6 and DR 152 are the list-plus-exception shape; the **thread** clause
  has JNI (*"valid only in the current thread"*), POSIX's `SIGEV_THREAD` (*"a newly created thread"*) and Python
  (*"always executed in the main Python thread"*) beside it.
- **(d1) no list**: the Rust Reference, *"The following list is not exhaustive; it may grow or shrink."*
- **(d2) per platform**: HotSpot (*"available on Linux and macOS"*), Go's Linux-only `SIGCANCEL` case.
- **(d3) change the runtime**: Go 62440 (the runtime made to survive what the docs never promised), Rust 1.81 (an
  unwind out of a non-unwind ABI made a definite abort), Linux's `SS_AUTODISARM` (a kernel flag so a switch away is
  safe), curl 8.1.0 (the feature kept only where it can be made safe).
- **(d4) a told end**: JNI's `AttachCurrentThread`/`DetachCurrentThread` is a told protocol for **threads**; none found
  for a handler's end.
- **(d5) the words alone**: Oberon's *"What remains unsaid is mostly left so intentionally"*.
- **(d8) abort at the exit**: Lua raises at the yield, Rust aborts at the boundary.
- **(d9) a told route**: Algol 60's *"accompanying information"* and C's *implementation-defined* (*"each implementation
  documents how the choice is made"*).

## What the history says went wrong where a document stated a permitted outcome

1. **Promises keyed to a mechanism or a depth did not last.** C89's *"shall execute correctly in contexts of
   interrupts, signals"* was read by WG14 in 1997 as covering only handlers invoked by `raise` or `abort` and not nested
   (DR 152), then left out of C99 in what N1318 (2008) calls *"an undocumented quiet change"*. POSIX's `setcontext` on a
   handler's context promised *"program execution continues with the program instruction following the instruction
   interrupted by the signal"*; Issue 5 removed the sentence, and Linux's manual now says *"the present verdict is "the
   result is unspecified""*.
2. **The mechanism family itself left the standard.** `makecontext`, `swapcontext` and `getcontext` were obsoleted in
   2004 and removed in 2008, the standard saying their users *"are almost always using them to implement co-routines"*
   and that *"Use of contexts to create alternate stacks is not defined"*.
3. **The promise that lasted is keyed to a property.** POSIX's defect 516 (2012) dropped the depth bound, made the jump
   itself async-signal-safe and put *undefined* on what follows only where the handler interrupted unsafe code.
4. **Implementations fixed what the documents left open**: Linux 4.7's `SS_AUTODISARM` for a `swapcontext` away from an
   alternate-stack handler; Go's runtime for a case its team called *"already unsupported"*; Rust's 1.81 abort, closing
   what its announcement calls *"the longstanding soundness problem"*, five years after RFC 2945's start date
   (2019-04-03 to 2024-09-05).
5. **A library that ran its callers inside a handler said so in its source and shipped it** (curl's *"the remainder of
   the application to run"* *"within a signal handler"*), and CVE-2023-28320 came from that path's global buffer.
6. **Defined permission words were misread**: RFC 8174 exists because readers confused RFC 2119's defined words with
   plain English; ISO forbids *may* for possibility. No precedent read records what readers do with a lowercase *may*
   in a sentence about a program's outcome.

## verdict

**approve** (advisory), for a text of (b)'s shape, the class bounded by a property as POSIX's has been since 2012, with
(c)'s *"on its thread"*, whose permitted outcome names its two outcomes (C's *unspecified* shape) or says it is the
runtime's permission (ISO's *may*), never a bare *may* a reader can take as possibility; and **against** re-offering
`x1`'s positive clause keyed to mechanisms as written.

## argument

The run-promises keyed to a mechanism or a depth did not last: C89's *"shall execute correctly in contexts of
interrupts, signals"* was narrowed by DR 152 and silently dropped from C99; POSIX's `setcontext` continuation after a
handler went in Issue 5; the context functions left POSIX in 2008. The promise that lasted is POSIX's since 2012: the
jump is safe, and what follows is bounded by a property of what the handler interrupted. Where documents stayed silent,
runtimes changed (Go 62440, Rust 1.81, `SS_AUTODISARM`). ISO makes *may* the document's permission and *can* a
possibility; RFC 8174 records readers confusing defined words with plain ones. Lua states its switch rule definitely,
at the switch.

## condition

My reading changes if a precedent is found of a language or C library document that **promised** callers a run after
a handler left by `longjmp` or a context switch, keyed to mechanisms, and kept it unrevised for a decade; or of a
standard that uses lowercase *may* for an abort and records readers getting it right; or Zig's or Rust's own documents
ruling on signal handlers (not reached here). The signal stack trick's weight changes if a build shows QEMU's `auto` or
Pth's `configure` choosing the `sigaltstack` variant on a current Linux or Darwin, which would make it a live shape
rather than a fallback (Perl's Coro off x86 already is one by its own code, unbuilt here).

## search

How it was done: web search to find a page, then the page itself fetched and quoted; nothing stands on a search
engine's excerpt alone. **Queries**: `POSIX.1-2008 removed makecontext swapcontext getcontext obsolescent "Issue 7"
removed functions`; `"longjmp function shall execute correctly in contexts of interrupts, signals" "nested signal
handler" C89 removed C99`; `Engelschall "Portable Multithreading" "signal stack trick" USENIX 2000 paper`; `libcoro
CORO_SJLJ sigaltstack Marc Lehmann coro.h "signal stack"`; `Perl Coro libcoro "CORO_SJLJ" Makefile.PL "sigaltstack"
metacpan`; `ziglang github issue setjmp longjmp support "setjmp" zig language`; `Oracle Java troubleshooting guide
"signal chaining" libjsig "Handle Signals and Exceptions" HotSpot`; `curl hostip.c alarmfunc siglongjmp curl_jmpenv
USE_ALARM_TIMEOUT`; `ISO/IEC Directives Part 2 "7.4" permission "may" "7.5" possibility "can" "Do not use" verbal
forms`. **Pages opened** (each cited above with its URL): the Open Group's Issue 8 XSH chapter 2, `longjmp`,
`sigaltstack`; Issue 7 (2018) `longjmp`; Issue 6 `longjmp`, `makecontext`, `getcontext`; Austin Group bug 516; Linux
`makecontext(3)`, `getcontext(3)`, `sigaltstack(2)`; NetBSD `swapcontext(3)`; glibc's *System V contexts*; N1570 and
N1256 (clauses 3.4, 4, 7.13, 7.14); WG14 N1318 and C90 issue 0152; RFC 2945, the Rust 1.81 announcement, the Rust
Reference's undefined-behaviour page; `cmd/cgo`, `os/signal`, golang/go 62440 and commit 1ffc296; Engelschall's paper
(HTML); `pth_mctx.c` (Debian pth 2.0.1-2), libcoro's `coro.c` (Tarantool's copy), libcoro's home page, Perl Coro's
`Coro/Makefile.PL` (Debian 6.540-1), QEMU's `coroutine-sigaltstack.c` and `meson_options.txt`; the Algol 60 Revised
Report (masswerk.at), the Oberon report (2016 revision, pages 1, 6 to 10); ISO/IEC Directives Part 2 (2021 redline,
pages 1, 3, 4, 17, 18, 28 to 30 of the PDF); RFC 2119, RFC 8174; JNI's Invocation API (Java 21), HotSpot's *Signal
Chaining* (Java 21); the Lua 5.4 manual and `ldo.c`; Python's `signal`; curl's `CURLOPT_NOSIGNAL`, CVE-2023-28320 and
`hostip.c` (Fuchsia mirror). **Failed or refused**: `https://open-std.org/jtc1/sc22/wg14/www/docs/dr_152.htm` (404;
the issue page was read instead), `https://raw.githubusercontent.com/curl/curl/master/lib/hostip.c` (404; the mirror was
read), `https://www.gnu.org/software/pth/` (429), `https://www.iso.org/sites/directives/current/part2/index.xhtml` and
the ISO house style page (403), the HTML of `blog.rust-lang.org/.../Rust-1.81.0.html` (a redirect; the slash form was
read). **Not searched**: Zig's language reference and `std.posix.sigaction`, the Rust standard library's or
`signal-hook`'s docs on handlers, C17 and C23, XBD's definition of *undefined*, the Open Group's Issue 7 text of the
2008 removal, Engelschall's PDF (the HTML was read), the upstream libcoro file, Windows's documents (no seat asked for
them). So every *not found* above means *not on the pages named here*.

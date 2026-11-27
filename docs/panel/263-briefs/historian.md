# Panel 263, the historian's brief

Added 2026-11-27 at about 10:35, when the coordinator widened the sitting; **repaired the same day from about 11:00 by
the coordinator's repair, on the critic's first reading** (`00-shared.md`'s head; the critic's item 14 and its § 11).
Read `00-shared.md` first, whole: § 1 the question, § 2 the defect as run (§ 2.2, the defect's own program returning at
exit 0 with megabytes written below its stack and `--sanitize` blind to it; § 2.3, where the recursion writes), § 3.9
the secondary question (step 199j's stop), § 4 the routes, now (a) to (q) with the critic's (a4), (n), (o), (p) and (q),
§ 7 which of them move a sentence of the spec; then the critic's first reading (`docs/panel/263-reports/critic-briefs.md`, beside these briefs under
`.claude/worktrees/panel263/`). **Your agent file** is `.claude/agents/historian.md`: every factual claim carries a
source, verified by a web search or a page you read, and **anything you could not confirm is written `unverified`**. You
have no veto. Your report is `.claude/worktrees/panel263/docs/panel/263-reports/historian.md`, written as you go. You
read pages; **you start no paid run, download nothing to disk, print no credential, and save no file but your report.**

**The other seats build at the same time and have no web.** Where a precedent describes a mechanism, say exactly what the
page says the code does, step by step (which stack a callback runs on, where the guard lies and who places it, what is
checked at a function's entry and against what, what happens when the check fails, what a release changed and why), so a
seat can name which of its routes is the precedent's. **Each line says what the page itself states, never what Heroes
should do about it** (`.claude/rules/records.md` § And the record says whose idea it was: of 90 lines four researchers
once proposed, three re-readers found 33 saying more than their page did). A precedent the sitting's resolution rests
on becomes the decision issue's `## Prior art` only as you mark it `verified`, its page quoted and its URL and the day
you read it given.

## The question you answer

**Who has run code of a language's own on a stack another party allocated, or kept that code from running past the
bottom of a stack it does not own, and what did each learn: which stack a callback from C runs on, what bounds it (a
guard page, a check at each entry, a counter, a segmented or growing stack), what it cost, and what went wrong?** Every
precedent below is what the clerk or the coordinator recalls, **none verified by this sitting**: each is a question for
you. For each, say: **(where it runs)** the stack the code runs on and who allocated it; **(the bound)** the mechanism
that stops a recursion, quoted; **(the cost)** any number the page gives, with its unit; **(history)** when it entered,
what broke, any revision, with dates and versions.

1. **cgo: Go called from C.** What the `cmd/cgo` documentation and the Go runtime's source comments say about a call
   from C into Go: on which stack the Go function runs (the goroutine's, or the system stack, `g0`, as recalled) and how
   the switch is made (`cgocallback`, `cgocallbackg`, `needm` for a thread Go did not create, as recalled); what bounds
   the Go code there. And **golang/go issue 62440**: panel 259's synthesis records *"Go 1.21's cgo callbacks broke on a
   coroutine library's stack manipulation, mended by refreshing the bounds at every callback (golang/go issue 62440, the
   historian, verified)"* (`docs/panel/259-*.md:39-40`), carried, to be re-read: whose bounds were refreshed (`g0`'s, as
   recalled), in which release, and what the issue says broke. This is the precedent `00-shared.md`'s route (b) is said to
   be (*"the route cgo takes"*, the defect's draft). **The critic reads it, as recalled, as routes (b) and (c)
   together** (the critic's item 14, § 11): the callback's Go code on a stack the runtime owns **and** a check at each Go
   function's entry against `stackguard0` that grows the stack by copying. Say, step by step and from the pages, which
   of the two cgo is, or both, and where the thunk's own work (`needm`, the bounds' refresh) runs: on C's stack or Go's.
2. **Go's move from segmented to contiguous stacks**: the release (1.3, as recalled), the *"hot split"* problem it names,
   the design document's words on copying stacks and on what a stack check at each function's entry compares
   (`stackguard`, as recalled); and what Go does at a function's entry today. The precedent for route (c)'s check and for
   (j)'s refusal.
3. **Rust's stack probes and guard pages**: the standard library's stack overflow handling on Unix (its guard page, its
   message *"has overflowed its stack"*, as recalled, and what it does on a thread it did not start), `__rust_probestack`
   and inline stack probes (`probe-stack`, as recalled), and any change to the main thread's guard page on Linux (as
   recalled, Rust stopped placing its own there, relying on the kernel's gap: verify the release and the reason). And
   **Rust's removal of segmented stacks** (2013, as recalled) and the reason given.
4. **`-fsplit-stack`**: GCC's split stacks (Ian Lance Taylor's design, gccgo, as recalled), what the prologue compares
   and calls (`__morestack`), which targets support it, and LLVM's support. **The clerk measured** both arm64 clangs
   accept the flag and emit no `__morestack` call (`00-shared.md` § 3.8): find what the compilers' documentation says of
   arm64, so the measurement is read against it.
5. **GCC's `-fstack-check` and `-fstack-clash-protection`**: what each probes, the Stack Clash advisory that led to the
   second (Qualys, 2017, as recalled) and its words on a stack that grows into a neighbouring mapping with no guard
   between, the shape of `00-shared.md` § 2.2; and clang's support of the second on Darwin and Linux arm64.
6. **Others the clerk recalls, each a question**: **Rust's `stacker` crate** (`maybe_grow`: checks the remaining stack
   and runs a closure on a newly allocated stack, as recalled: routes (e) and (g) in one); **Boost.Context's stack
   allocators** (`protected_fixedsize_stack` with a guard page against `fixedsize_stack` without, as recalled; the
   documentation's warning, if any); **Lua's C stack limit** (`LUAI_MAXCCALLS`, *"C stack overflow"*, a counter, as
   recalled: route (k)'s precedent); **CPython's recursion limits** and any move from a counter to a check of the C stack
   pointer against the thread's bounds (3.12 to 3.14, as recalled); **HotSpot's stack banging and its guard zones**
   (yellow, red, reserved, as recalled) and **JNI's `AttachCurrentThread`** for native threads; **OCaml 5's fibres**
   (effects on heap-allocated stacks, and which stack a call to C and a callback from C run on, as recalled); **GHC's
   `foreign export`** (Haskell code on Haskell's own heap stacks, as recalled); **Windows fibres and
   `SetThreadStackGuarantee`, `_resetstkoflw`**; **AddressSanitizer's fiber annotations**
   (`__sanitizer_start_switch_fiber`). Add the ones you find; drop the ones you cannot source.
7. **Added by the repair, for the routes the critic listed** (`00-shared.md` § 4): **a stack allocator handed through a
   library's door** (route (o)): OpenSSL's `ASYNC_set_mem_functions` documentation (its stack size and its default
   allocator, by release: the tree's reading is of 3.5.9, this Mac holds 4.1.0, `00-shared.md` § 2.7), Boost.Context's
   `stack_allocator` concept, any coroutine library that takes a caller's stack allocator; **a static bound on a call
   tree's stack** (route (n)): GCC's and clang's `-fstack-usage` and any system that refuses or checks a callback by a
   computed bound (embedded toolchains, as recalled); **a hardware watchpoint as a guard** (route (p)): Linux's
   `perf_event_open` breakpoints delivered as signals, and whether Darwin offers a user-space write watchpoint, as
   recalled none; **copy-stack coroutines** (`00-shared.md` § 5 C): greenlet and libco's shared stack, as recalled.
8. **For the secondary question, step 199j's stop** (`00-shared.md` § 3.9): what POSIX says of calling `swapcontext` or
   `setcontext` from a signal handler (async-signal-safety; the functions' removal from the standard, as recalled, in the
   2008 edition), and how a library that schedules green threads from `SIGALRM` (GNU Pth, as recalled) words what a
   handler may switch to while the interrupted code was itself switching.

## What you write

One section per precedent, each ending with a line **`verified`** (a page read, quoted, its URL and the day you read it)
or **`unverified`**; then a table: precedent · the stack foreign or callback code runs on · what bounds it · the cost the
page gives · what went wrong; then **which route of `00-shared.md` § 4 each precedent is**, (a) to (q) or none, and
**which route no precedent you found has taken**, written with what you searched (CLAUDE.md § RUN IT: a negative rests on
the searcher's vocabulary); and your `search` section as your agent file asks, the queries and the sources swept.

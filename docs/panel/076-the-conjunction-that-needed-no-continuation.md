# Panel 076 — the conjunction that needed no continuation

**Convened** 2026-08-16, M-selfhost-port, on `docs/debrief/DECIDE.md:338` and `:341`
**together**, because :341 says so in its own text. Full lane, five seats. Two of
them held **live vetoes with stated exit conditions** on this exact question, and
both re-measured the numbers those vetoes rest on.

## What was asked

A deep value still SEGFAULTs on `==` and `hash` — exit 139, no output, no
diagnostic — and the sitting had also to rule on catching **every** stack
overflow and printing a message.

## The measurements that decided it

**The cliff is not a property of the program**, and three seats narrowed it:

| | coordinator | compiler-engineer | ffi-pragmatist |
|---|---|---|---|
| `-O0` | 52 043 ok · 60 000 crash | **52 200 ok · 52 500 crash** | 52 043 ok · **56 000 crash** |
| `-O2` | 200 000 crash | **125 000 ok · 150 000 crash** | 200 000 crash |

`DECIDE.md:338`'s recorded **52043** is stale — it exits 0 today. And the ratio is
**~2.6×**, not the brief's *"more than 3×"*.

**Then the measurement that killed the depth bound outright.** Same binary, same
program, depth 52 200, `-O0`, reproduced by the coordinator:

```
plain environment                 exit 0
one 900 KB environment variable   exit 139
empty environment                 exit 0
ulimit -s 4096                    exit 139
```

**A program that passes is crashed by an environment variable**, because the
environment is copied onto the stack at process start. No number written in a
specification can survive that, and CLAUDE.md §11 names the class: a narrowing
that asks the world instead of the value expires in silence.

## The finding: `==` is a conjunction, so nobody needed a continuation

**Two seats prototyped the same repair independently, without seeing each other's
work, and measured the same result.** The compiler-engineer's veto — *"the
continuation lands in `emit/structural.rs` and the Heroes port must reproduce
it"* — turned out to be about a shape nobody has to build:

> `true` is the identity of `&&`. A nested `hero_array_eq` may push its
> elementwise work, **return `true` provisionally**, and let the outermost call
> drain the queue and AND the real answer in. The generated `_eq` never learns it
> was deferred.

And the recursion cannot escape the runtime, because the checker forbids it:
`record Node { child: Node }`, `child: Node?` and a self-referencing variant are
all `error[no_size]`, measured. **Every unbounded descent goes through `[T]` or
`{K: V}`, and both are runtime C.**

| | compiler-engineer | ffi-pragmatist |
|---|---|---|
| where | `runtime/parts/eq.c` (new) + `array.c` + `runtime.c` | `runtime/parts/array.c` alone |
| size | +130 (40 code) | **+84** |
| lines in `crates/` | **0** | **0** |
| `emit/structural.rs` | untouched (285) | untouched |
| depth 1 000 000, `-O0` | **exit 0** | **exit 0**, 2.1 s |
| control, same file unpatched | exit 139 | exit 139 |
| `cargo test` | 569 | 569 |
| `HERO_RUNTIME_ABI` | 14 | 14 |

**`hash` takes the opposite fix, and that is the compiler-engineer's own
contribution**: `eq` must be exact, while `hash` may lose information — its only
contract is that equal values hash equal. So `hero_hash_array` gets a **depth
cap**, and that number is a fact about *the value* (how deep a hash bothers to
look), not about the machine. Being wrong costs collisions, never a crash.
Measured: a map key at depth 1 000 000 exits 0, and two keys differing only past
the cap collide in `hash` and are still told apart by `eq`.

## Verdict table

| judge | verdict | the finding that decides it |
|---|---|---|
| **compiler-engineer** | object 1 · **approve 2 (runtime shape), veto 2 (emitter shape)** · object 3 | **Corrected its own number in the record**: the closure list's release depth is **7, not 3** — *"I put that number into a panel brief from memory and CLAUDE.md §1 says that is exactly what must not happen."* Zero cycles over 45 declarations, so depth is a **static property of the type graph**, margin ~7 400×. Its exit condition (>1000) is unmet, **so the veto stands and is simply not engaged** by a fix with zero emitter lines |
| **ffi-pragmatist** | object 1 · **approve 2 (runtime-only)** · object 3 · **veto 4 stands** | Compiled the handler and then demolished it as an answer. **(I)** A bound C library that installs its own `SIGSEGV` handler — SDL, Boehm GC, any JIT — **un-installs the guarantee with no compile error**, measured exit 139 in silence; §1.11 makes that the normal case. **(F)** With the handler in, ASan's **262-line** report becomes **2 lines**: it blinds the instrument. **(G)** A second fault inside the handler **hangs at 100 % CPU** — worse than exit 139. **(B)** The Darwin/Linux inversion compiles clean under all 13 flags, installs, runs, and is **indistinguishable from doing nothing** |
| **spec-warden** | **approve 3 runtime-only at +0** · **veto 1** · object 2, 4 | Ruled the sitting's frame: `spec:76` invites `children: [Node]` and `:144` promises `==` is recursive **with no bound** — *neither sentence is false*, so under §12 this is a **compiler bug under a true spec** and §1.6 has no bill. Priced the alternative honestly: the arena rewrite a depth bound teaches costs **+210 program tokens (130 → 340)** and **still exits 139**, because the reader's own walk recurses. And the spec carries **seven** abort sentences naming **zero** abort messages, so a runtime message owes nothing |
| **llm-ergonomist** (spec-only, blind, contamination disclosed) | **veto 4** · approve 2 · approve 3 with the message rewritten · 1 splits | *"The spec recruits the program."* `children: [Node]` is a tree and `==` is recursive are **two adjacent sentences**, and the crashing program is their composition — *"the word `recursively` is the promise that segfaults."* Counted **eleven named aborts** in the document: a reader closes it believing failure modes are enumerable and named. And: **"stack" means nothing to a spec-only reader** — the word never appears, nor "memory", nor "recursion". A message must name the program's own act with both numbers |
| **historian** (advisory) | approve 3 · **object to 3 as the answer to item A** | Fifteen systems. **OCaml ran this experiment for twenty years and abandoned it**: OCaml 4 had exactly this shape, its author told users to *"never rely on it — treat it as a debugging aid"*, and **OCaml 5 replaced it with a check the compiler emits into the function prologue**. Erlang's `eq()` is iterative **by construction** (`DECLARE_WSTACK`). *"Rust's `Drop` has overflowed since 2019, unfixed; Heroes already beat Rust there at panel 070. Stopping short on `==` gives that back."* **Two corrections to the brief**: `rust-lang/rust#69533` is **closed** (fixed 2020-03-19), and `runtime/heroes_runtime.h:19` says *"HERO_RUNTIME_ABI is 3"* eleven bumps after it became 14 |

## The resolution — `ratified 2026-08-24` (author instruction, batch over 069-079; § Author's verdict below)

1. **Candidate 1 (a depth bound) is refused**, on the warden's veto and a
   measurement rather than an argument: an environment variable moves the cliff,
   `ulimit` moves it, `-O` moves it 2.6×, and the rewrite it teaches costs +210
   program tokens and crashes anyway.
2. **The `==` worklist lands, in the runtime, and the emitter is not touched.**
   Two independent prototypes, both at depth 1 000 000 exit 0, both 569 green,
   both ABI 14. The compiler-engineer's veto on the **emitter** shape stands and
   is untouched by this; the port reproduces nothing.
3. **`hash` gets a depth cap**, because it is allowed to lose information and a
   cap on it is a fact about the value.
4. **Candidate 3 (the signal handler) is refused as this sitting's answer, and
   kept alive with its conditions written down.** It is defeasible by any C
   library that installs a `SIGSEGV` handler, it blinds ASan unless guarded by
   `__has_feature(address_sanitizer)`, its message **lies** when the fault is
   inside C, `_SC_SIGSTKSZ` and `SIGSTKSZ` compile on disjoint platforms, and
   Windows is a second implementation. It returns as an **unconditional
   reporter** — claiming a memory fault, never a cause — when the three-leg CI
   matrix proves the classifier on Linux and Windows.
5. **Candidate 4's veto stands, with a named route**: `.github/workflows/ci.yml`
   already runs three legs on `workflow_dispatch`, so the exit condition is one
   dispatch away — and it must assert **the printed line**, not exit 139, which
   the inverted build also produces.
6. **`--sanitize` does not discharge §1.12.** *"A Heroes program must not
   segfault — a program, not a sanitized build of one."*
7. **One behaviour change is adopted with it and stated here rather than
   discovered**: a generated `_eq` that panics (panel 061's `partial` refusal) can
   now fire on a subtree the old short-circuit would have skipped.

**What a veto at ratification would compel**: the change is one runtime file;
reverting is a revert, and no golden was regenerated.

## Predictions to score

| judge | prediction | at |
|---|---|---|
| compiler-engineer | `emit/structural.rs` is **≤ 300 lines** and contains no worklist, queue or resume state (285 today). If the runtime-only shape was wrong, a per-field continuation lands there and blows past it | M-selfhost-fixpoint |
| ffi-pragmatist | the corpus scores the loudness guarantee at **100 %** while it stays defeasible in one `sigaction` call: `examples/curl` and `examples/sqlite` both keep printing, because neither library touches `SIGSEGV`. **Zero tests will fail when a bound library resets the disposition** | the milestone that lands item B |
| spec-warden | this sitting adds **0** tokens, and at fixpoint the closure list's deepest compare chain is **≤ 12** with **zero** by-value cycles (7 and 0 today) | M-selfhost-fixpoint |
| llm-ergonomist | documentation alone moves belief and not outcomes: under a spec sentence with no implementation change, message-free failures stay at exit 139 while ≥60 % of readers now state a limit exists | M-program-corpus |
| historian | **the loud message will not name a `.hero` line** — symbolisation inside an async-signal-safe handler is what Rust, OCaml and Zig all decline. Scored by reading that milestone's golden `.expected`: a `path:line:col` in it falsifies me | the milestone that lands item B |

## Conditions on the record

- **compiler-engineer**: the emitter veto lifts at closure-list release depth
  > 1000 (7 today), or on a program whose `==` recurses unboundedly **without**
  passing through `[T]`/`{K: V}` — which would mean `no_size` has a hole.
- **ffi-pragmatist**: candidate 3 becomes `approve` as an unconditional reporter,
  ASan-guarded, declining via `SIG_DFL` + **return** (never `raise()`), with
  `fflush(stdout)` treated as a known hang risk. **`-D_GNU_SOURCE` must never
  reach `FLAGS`** — measured, it silently re-types `strerror_r` and that reaches
  every author's `extern` header.
- **spec-warden**: a spec sentence becomes admissible the day a depth number can
  be honoured across `-O0`/`-O2` and across `ulimit`.
- **historian**: reverses toward the handler alone on a compiler that emitted
  recursive structural equality, hit this in production, and **chose** the
  handler — *"Rust's #58068 is the nearest and it is an unfixed bug, not a
  decision. Find me a decision."*

## Author's verdict

**2026-08-24: ratified** (author instruction, *"ratifica anche quelle 11"* — a batch yes over panels 069-079, given after being told plainly that every one had shipped as a provisional default and that the tree had been green over all of them for eight days. The author was offered the alternative of reading each first and chose the batch.)

What the yes settles: **the `==` worklist lands in the RUNTIME and the emitter is not touched**, and a depth bound is **refused on the warden's veto plus a measurement** — an environment variable moves the cliff, `ulimit` moves it, `-O` moves it 2.6×, and the rewrite such a bound would teach costs +210 program tokens and crashes anyway. A limit that three unrelated knobs can move is not a limit a document can state.

**On the batch.** This is the fourth blanket ratification in this project's record and the largest. It closes a gap of eight days in which eleven sittings sat queued with nobody asking — the same shape panels 085-087 sat in for four. What the batch does NOT do is re-open anything each sitting left explicitly open: every reserved veto, queued follow-up and unmet condition inside this file stands exactly as written, and a yes over the resolution is not a yes over the questions the resolution deferred.

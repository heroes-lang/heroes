# Panel 070 — the stack nobody was counting

**Convened** 2026-08-16, M-selfhost-port, from panel 069's filed residue: a deep
value SEGFAULTS, silently, on today's compiler. Lane: **compiler-engineer,
ffi-pragmatist, historian**. The llm-ergonomist and spec-warden were not seated —
nothing here reaches the spec or the surface, and both seats' inputs would have
been empty. That is a deviation from the full lane and it is recorded rather than
assumed: if the resolution ever grows a spec sentence, it needs a warden.

## What was asked

Nine lines of Heroes build a `record Node { label: i64, children: [Node] }`
chain. Deep enough, and the program dies with **exit 139 and no output at all** —
no diagnostic, no `hero_panic`, nothing. `--sanitize` names it as a stack
overflow. lldb puts it at `runtime/parts/array.c:68`, `hero_array_decref`,
recursing through `a->elem->drop` into the generated `Node_drop` and back, one C
frame per level. **No `==` is involved**: construction and scope exit are enough.

design.md §1.12 says a Heroes program must not segfault. This is the clearest
case of that rule anyone has brought to a panel.

Shapes: **(a)** a depth counter that aborts loudly · **(b)** an explicit worklist
replacing the recursion · **(c)** probe the remaining stack and abort before the
guard page · **(d)** document a maximum depth.

## The convener briefed a false premise, for the fourth sitting in five

The brief's headline was *"it depends on the optimisation level"*, with a table
showing `-O2` at exit 0. **The compiler-engineer refuted it and the coordinator
confirmed the refutation**: `-O2` segfaults too, at depth 200000 where `-O0` goes
at 80000. Measured by that seat: the recursion depth is **identical at both
levels** — 20001 at depth 20000 — and clang eliminates **zero** calls. Only the
frame shrinks, 112 bytes to 48, as two generated functions inline. Assembly on
both arm64 and x86-64 shows a `call` at both levels.

And it is structural rather than lucky: `array.c:74`'s `a->elem->drop(...)` sits
**inside a `for` loop and is followed by `hero_release_block(a)`**. A call
followed by work is not in tail position, so this is not an optimisation clang
happens to make — it is one **no compiler is permitted to make**. `-O2` was never
a fix and could never have been one.

**Why the coordinator got it wrong is a defect of its own**, and the seat found
that too. `crates/heroes-cli/src/commands/run.rs:59` returns a hardcoded
`Exit::Program(134)` for **any** signal death, under a comment claiming it
follows the shell's `128 + signal` convention — which it does not: 134 is
SIGABRT's code and a SIGSEGV is 139. So `heroes run` cannot tell a clean
`hero_panic` from a segfault, and the brief's own table read `134` as "aborted
cleanly" when it was the crash wearing the wrong number. **One line, and it is
repaired regardless of which shape wins.**

## Verdict table

| judge | verdict | the finding that decides it |
|---|---|---|
| **compiler-engineer** | approve **(c)** · **veto (b) complete** · reject **(a)** · **(d)** refuted | **It wrote (a) the natural way and (a) broke a correct program**: two **3-element** `[i64]`s compared 20000 times abort with *"value nested too deep"*, because `array.c:142` returns from inside the loop and the counter never unwinds — **13 such early-return paths** across five entry points. Built **(c)**: 64 lines in a new `runtime/parts/stack.c` plus 5 call sites, **0 lines under `crates/`**, all four shapes × both levels at depth 400000 → **exit 134 with a message, 8 of 8**, 566 tests green, cost not measurable. Vetoes **(b) complete** on §1.7's ceiling: covering `==` needs `_eq`/`_hash` to become **resumable** — `h_Node_eq` compares a field then recurses, so a deferred sub-answer resumes mid-function, which is a continuation — landing in `emit/structural.rs`, rewriting 6 `emit/*.expected`, **and the Heroes port must reproduce it**. For a closure list it measured at depth **3** (381 probe reports across all 20 `selfhost/` modules; margin ~17000×) |
| **ffi-pragmatist** | **veto (c)** and **(d)** · object **(a)** · approve **(b)** | **§1.11 makes the depth the LIBRARY's choice, and it compiled the proof.** A binding-shaped C library whose nesting comes from a *file* — libxml2's, cJSON's, expat's shape — mirrored into Heroes with **one `while` loop and no recursion**: the `.hero` contains no number, and the same binary is exit 0 on a shallow file and **exit 139, silent, on a deep one**. So (a) and (d) ask the author to bound what they do not control. **Vetoes (c) on portability, with a compiled disqualifier**: Darwin's `pthread_get_stackaddr_np` returns the stack's **HIGH** end and Linux's `pthread_attr_getstack` returns the **LOW** end — same idea, opposite meaning, no diagnostic, and treating one as the other yields **−6805** remaining bytes. It could not compile the Linux or Windows arms and **reports them unverified rather than working**. **(b)'s storage hazard dissolves**: `refcount` is `HeroArrayHeader`'s first member and is dead at zero, so the worklist threads through it — intrusive, **zero allocation**, layout byte-identical. Measured: **depth 10,000,000, exit 0, no leak**; 1,000,000 on a 512 KB pthread; ASan+UBSan clean; 566 tests green |
| **historian** (advisory) | object to the framing — **a fifth shape is missing** | **Rust turns this exact crash into a printed message, in a native binary with no scheduler.** `sigaltstack` + a SIGSEGV/SIGBUS handler with a guard-page-range check → *"thread 'main' has overflowed its stack"* → abort; unix **and** Windows, [verified in `library/std/src/sys/pal/unix/stack_overflow.rs`](https://raw.githubusercontent.com/rust-lang/rust/master/library/std/src/sys/pal/unix/stack_overflow.rs). The JVM, OCaml and ASan do the same. **Zero fast-path cost — and it reports rather than prevents.** Everyone who wanted prevention converged: CPython's `_Py_Dealloc` now checks the **stack margin** and defers to a **worklist threaded through the dying object's own header** ([PR #132280](https://github.com/python/cpython/pull/132280), 2025-04-30) — that is (c) as trigger and (b) as action, in the release path, measured at Linux x86 **1.5% slower** to Mac ARM **3.8% faster**. Its 25-year depth counter was **replaced**. And **Rust and Swift both chose (d)** — [#58068](https://github.com/rust-lang/rust/issues/58068) open since 2019, [swift#47721](https://github.com/swiftlang/swift/issues/47721) since 2017 — but *their* (d) fails **loudly**, and Heroes' would fail silently |

## Where the seats disagree — and it is a real clash, not a smoothing

**The two seats that compiled prototypes vetoed each other's shape.** Both
measured; neither is arguing from principle.

- The **compiler-engineer** approves **(c)** because it covers **all five walks**
  — drop, `==`, `hash` — for 69 runtime lines and nothing under `crates/`, and it
  is complete *by construction*: `types/counted.rs` and `error[no_size]` prove
  `[T]` and `{K: V}` are the only unbounded indirections, so five guards cover
  every walk including ones not yet written.
- The **ffi-pragmatist** vetoes **(c)** because it could not verify it on two of
  three CI legs, and produced a compiled reason to expect it to be wrong there:
  the high-end/low-end inversion passes `-fsyntax-only` and returns a negative
  number at run time. *"I will not approve on reasoning."*

**They may be less far apart than the verdicts read**, and the synthesis says so
rather than pretending: the pragmatist's (b) prototype covers the **drop path
only**, and the engineer's veto is of **(b) complete** — the `_eq`/`_hash` half
that needs a continuation. Neither vetoed drop-only (b). The engineer's objection
to it is that it repairs the *highest* threshold (74642) and leaves the *lowest*
live (`==` at **52043**), which is true and is not a veto.

## Corrections the seats made to the brief

1. **`-O2` is not a fix** (compiler-engineer, confirmed by the coordinator). See
   above. The brief's headline was wrong and so was the recap built on it.
2. **musl is not a CI leg** (ffi-pragmatist). The brief asked about it; the three
   legs are `ubuntu-latest` (glibc), `macos-14`, `windows-latest`, verified at
   `.github/workflows/ci.yml:96`. A musl-only wrong answer would ship undetected —
   which is an argument *for* the pragmatist's caution about (c), not against it.
3. **The golden harness already runs both levels** (coordinator, correcting the
   historian's inference from the ROADMAP). `golden.rs:597` executes every `run/`
   case at `-O0` **and** `-O2`, deliberately, because an `-O0`/`-O2` divergence is
   a known class for a C emitter. The instrument is not blind — **the case was
   simply never written**, and writing one turns this red today.

## The resolution — `ratified 2026-08-24` (author instruction, batch over 069-079; § Author's verdict below)

**The panel rule's conservative default, applied to a genuine clash: adopt what
both compiling seats measured and neither vetoed, and hold what one of them
vetoed.**

1. **(d) is refused** by both compiling seats and the coordinator's own
   measurement. There is no number to document: the same source dies at **74642**
   (-O0) and **174003** (-O2) on one machine in one hour, and at **4750** on a
   default pthread the day `hero_spawn` lands — a **15.6×** swing on a thread
   nobody has created yet.
2. **(a) is refused on a demonstrated defect**, not on taste: written the natural
   way it aborted two 3-element arrays whose nesting depth is 1.
3. **(b) for the DROP path lands**, in the ffi-pragmatist's intrusive form —
   threaded through the dead `refcount`, zero allocation, layout byte-identical,
   `HERO_RUNTIME_ABI` unchanged at 14. Both seats measured it working; neither
   vetoed it. It closes the route that needs no `==` and the least program.
4. **`run.rs:59` is repaired in the same commit**: a signal death must report
   `128 + signal`, as its own comment already claims. It is one line, it is
   uncontested, and it is what hid this defect from the convener.
5. **(c) is held, not refused**, with the ffi-pragmatist's condition as its exit:
   it returns the day the probe is **compiled and run** on `ubuntu-latest` and
   `windows-latest` showing the remaining-bytes figure correct on both — *not* a
   syntax check, because the inversion passes one. Until then `==` and `hash`
   remain unbounded, which is **recorded as a known live hole** rather than
   closed by assumption.
6. **(f), the historian's missing shape, is queued as its own question.** It is a
   backstop rather than a fix — it reports and does not prevent — but it is the
   only shape that catches deep *user* recursion too, which this language will
   also have, and four independent systems ship it at no fast-path cost. Its
   three potholes are on the record: glibc 2.34 made `SIGSTKSZ` non-constant, a
   signal stack has no guard page of its own, and ASan installs the same handler.

**What a veto at ratification would compel**: (3) is one file and revertible;
(4) stands regardless, because the comment and the code disagree today.

## Predictions to score

| judge | prediction | at |
|---|---|---|
| compiler-engineer | the landed repair touches **0 lines under `crates/heroes/src/emit/`**, `HERO_RUNTIME_ABI` is still **14**, and the probe fires **0 times** across the 63 `run/` goldens and all 20 `selfhost/` modules | M-selfhost-fixpoint |
| ffi-pragmatist | a libxml2 binding mirrored into a Heroes `record` needs **no shim and no depth argument** under (b), at any nesting `xmlReadFile` accepts; under (a) or (d) the same binding needs a maximum libxml2 does not expose | M-ffi-ladder · `hero_spawn` |
| ffi-pragmatist | the unpatched runtime crashes that binding at **4750 ± 100** levels on a default pthread; the patched one survives 1,000,000 | when `hero_spawn` lands |
| historian | (b) with CPython's design costs **under 2%** over `heroes mutate`'s corpus in all three configurations; above 5% the CPython precedent does not transfer and this reopens | M-selfhost-port |
| historian | shape (c) computes bounds from the OS, and **CPython 3.14 broke on programs that switch stacks**; if M-isolated-threads chooses stack switching, a bounds probe installed now becomes silently wrong there | M-isolated-threads |

**Scored in this sitting**: the coordinator's own headline. *"It depends on the
optimisation level"* is **falsified** — `-O2` crashes at 200000, the recursion
depth is identical at both levels, and the tail call clang was supposed to be
making is one the C standard does not permit it to make.

## Conditions on the record

- **(b)'s two file-statics must become `_Thread_local` before `hero_spawn`**
  (ffi-pragmatist). Two threads dropping concurrently would splice each other's
  lists and free the wrong blocks — the §1.12 class this repair exists to close.
  Compiled clean on Darwin arm64; **the Windows leg must compile it, not be
  reasoned about**.
- **The closed-drop-chain fact is a premise about the world** and CLAUDE.md §11
  says it expires in silence. The pragmatist enumerated it by hand — no `extern`
  name is reachable from any `_release` or `_desc_drop`, so no binding can
  observe the release order — and it owes the test whose failure names it:
  `no_extern_is_reachable_from_a_drop`, over `heroes mutate`'s corpus.
- **`HERO_STACK_MARGIN` must carry its justification in the file** if (c) ever
  lands, and each guard owes a `fixedbugs` case that makes it fire (§9).
- **One reading was not done**: Munch-Maccagnoni & Douence's typed pointer
  reversal ([hal-04406342](https://inria.hal.science/hal-04406342), JFLA 2024),
  which Swift's Joe Groff named as the intended fix in 2022 and never shipped. It
  would give constant stack **and** constant heap with no worklist at all, and
  Heroes has full type information at drop-emission time. Recorded so the next
  sitting does not re-derive it.

## Author's verdict

**2026-08-24: ratified** (author instruction, *"ratifica anche quelle 11"* — a batch yes over panels 069-079, given after being told plainly that every one had shipped as a provisional default and that the tree had been green over all of them for eight days. The author was offered the alternative of reading each first and chose the batch.)

What the yes settles: the panel rule's **conservative default applied to a genuine clash between two compiling seats** — adopt what both measured and neither vetoed, hold what one of them vetoed. **(d) is refused** by both compiling seats and by the coordinator's own measurement. The yes ratifies that disposition, not a preference between the seats.

**On the batch.** This is the fourth blanket ratification in this project's record and the largest. It closes a gap of eight days in which eleven sittings sat queued with nobody asking — the same shape panels 085-087 sat in for four. What the batch does NOT do is re-open anything each sitting left explicitly open: every reserved veto, queued follow-up and unmet condition inside this file stands exactly as written, and a yes over the resolution is not a yes over the questions the resolution deferred.

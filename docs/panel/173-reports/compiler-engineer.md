# Panel 173 — compiler-engineer

**verdict: VETO**, on soundness, and it is cheap to answer: the mechanism's
shape is right and its *sentence* is false. Everything below was run in
`<scratch>/compiler-engineer-173` on 2026-09-21, Darwin 25.6.0 arm64, with the
compiler rebuilt from the seed (`rm -rf build && clang -I runtime seed/heroes.c
runtime/runtime.c -o heroes`, `real 3.43`, zero warnings).

**section**: design.md §1.12, its own falsifier clause (design.md:605):

> **What would make this wrong** (§12, panel 039's standard applied to a
> principle): a measured case where a defensive check *hides* a defect rather
> than surfacing it — an abort that stops a program at a place that tells the
> reader nothing, where the crash would have named the cause.

I have the inverse and it is worse: a defensive report that **misnames** the
cause. Secondarily design.md §4.17 (design.md:2036), the `certain | guess`
doctrine: this ships a guess in the grammar of a fact.

---

## What is my predecessor's and what is mine

**My predecessor's**, verified by me and not changed: the `hero_abort()` funnel
in `runtime/parts/panic.c`, the fifteen call sites converted in five files, the
compile-time yield to ASan, the chain-then-restore-then-reraise, the frame walk
that names the Heroes function, and the five `tests/golden/run/` cases. I
rebuilt it, re-ran every number, and the diff is byte-identical to the state
I received (`git diff --numstat runtime/` matches before and after my work).

**Mine**: three adversarial programs in the scratchpad copy's brief directory
(`cabort_lease.hero`, `cassert_lease.hero`, `nullwrite_lease.hero`, with
`beside2.h` and `beside3.h`); a throwaway `siginfo_t` probe compiled into a
`heroes-probe` binary and then removed; the four suite runs; the
baseline-versus-patched attribution of every red line. Two of those three
programs are the veto.

---

## The veto, in one table

`CPATH=docs/panel/173-briefs ./heroes build docs/panel/173-briefs/<x>.hero -o <out>`,
then N runs of `<out>`, recording `$?` and `wc -c` of stderr.

| program | what C actually did | exit | stderr | the report |
|---|---|---|---|---|
| `cabort_lease` | `abort()` for C's own reason, **freed nothing** | 134, 5/5 | 279 B | `panic: a C function freed bytes this program still leases — 1 lease(s) were live when the allocator refused the free, called from cabortlease.main` |
| `cassert_lease` | a C `assert(1 == 2)`, **freed nothing** | 134, 3/3 | 350 B | C's true line, then the same false one underneath |

`cabort_lease.hero` is nine lines; its C side is
`static inline void boom(void) { abort(); }`. Its whole stderr is a sentence
that names a `.lease()`, an `end_lease` and a freeing callee, none of which
happened. A reader given that line goes to the FFI boundary; the bug is inside
`boom`.

`cassert_lease` is the exact two-line shape panel 172's critic filed under
`panic_lease.hero` (`docs/panel/172-reports/completeness-critic.md` § 2). The
funnel closed the Heroes half of that class and left the **C** half wide open,
which is CLAUDE.md § RUN IT's rule about attacking the shapes beside the one
that provoked the repair (CL-061, CL-078).

The premise the handler rests on is
`SIGABRT ∧ hero_live_held > 0 ∧ !hero_runtime_spoke ⟹ a C function freed leased bytes`.
That is not valid. Every SIGABRT C can raise for any other reason is a false
positive: a library's `abort()`, a failed C `assert`, `__stack_chk_fail`, a
`raise(SIGABRT)` from a handler somebody else installed, an allocator refusing
a free that had nothing to do with a lease.

### And `siginfo_t` cannot save it — measured, not argued

Brief question 1 option (b) is dead on Darwin. I inserted a probe printing
`signum`, `si_code`, `si_pid`, `getpid()` and the funnel flag at the top of
`hero_lease_crash`, rebuilt, and ran each program twelve times:

| program | signum=5 (SIGTRAP) | signum=6 (SIGABRT) | si_code | si_pid |
|---|---|---|---|---|
| `lease070` (true positive) | 10/12 | **2/12** | 0 | `== getpid()` |
| `cabort_lease` (false) | 0/12 | 12/12 | 0 | `== getpid()` |
| `cassert_lease` (false) | 0/12 | 12/12 | 0 | `== getpid()` |
| `panic_lease` (runtime's own) | 0/12 | 12/12, `spoke=1` | 0 | `== getpid()` |

**On the SIGABRT rows the true case and the false cases are byte-identical in
`siginfo_t`.** There is no field to branch on. A SIGTRAP-only report would be
sound on this Mac but would drop the SIGABRT path of the real defect, which is
not rare: 200 runs of `lease070` gave **152 at 133 and 48 at 134**, a 24% miss
rate. Whether a SIGTRAP-only report says anything at all on glibc, where
`free(): invalid pointer` calls `abort()`, is **UNRUN**: I did not enter the
Linux container.

### What lifts the veto, and it costs zero lines

Say what was **observed** instead of asserting the cause. The runtime knows
exactly two facts at that instant: the process is dying, and N leases are live.
It does not know a free happened. A sentence in §4.17's `guess` register, for
example *"panic: the process aborted with 1 lease(s) still live; if a C
function freed the bytes of a `.lease()`, that is what the allocator refused"*,
is true in all four rows above. It is a **string change** in
`runtime/parts/os.c` and it closes the class. I am not prescribing the wording;
I am refusing the assertion.

---

## implementation_cost

**Zero in the compiler.** `git status --short selfhost/` is empty; `cmp
seed/heroes.c /Users/joseph/Temp/heroes/heroes-lang/seed/heroes.c` is silent, so
brief question 6 is answered: **a runtime edit reaches the seed not at all.**
No lexer, no checker, no descriptor, no ownership pass, no emitter, no spec
token, no diagnostic the checker can produce. This is not core and it is not
sugar; design.md Part 5 does not reach it, and I say that rather than invent a
row for it.

**In the runtime**, `git diff --numstat runtime/`:

```
5	5	runtime/parts/alloc.c
3	3	runtime/parts/failure.c
102	0	runtime/parts/os.c
20	1	runtime/parts/panic.c
6	6	runtime/parts/stack.c
```

136 lines touched, **+121 net**, of which **79 are code** and 50 are comment and
7 blank (`git diff -U0 runtime/ | grep '^+' | grep -v '^+++'`, classified by
leading `/*`, `*`, `*/`). `wc -l` before (`git show HEAD:...`) and after:

| file | before | after |
|---|---|---|
| `runtime/parts/os.c` | 347 | **449** |
| `runtime/parts/panic.c` | 58 | **77** |
| `runtime/parts/alloc.c` | 610 | 610 |
| `runtime/parts/failure.c` | 147 | 147 |
| `runtime/parts/stack.c` | 718 | 718 |

`grep -n 'runtime' tests/harness/suite_layout.hero` matches **nothing**, so the
~300-line ceiling instrument does not judge `runtime/parts/`, and `os.c` at 449
is unwatched. I note it; I do not object on it. `.claude/rules/module-shape.md:4`
does list `runtime/**` in its `paths:`.

**Two `sigaction` calls at process start**, `runtime/parts/os.c:218-219`, inside
a `static int done` guard, called once from `hero_args_set`. Stated as what they
are and not timed, per the brief.

### The cost my predecessor did not count: two suites go red

Both measured with the rebuilt compiler, both attributed against a baseline.

**1. `./heroes run tests/harness/main.hero -- ./heroes runtime`**

```
  runtime: 6 passed, 2 failed
FAIL runtime/threads
    runtime/parts/os.c:172: static struct sigaction hero_lease_prev_trap;
    runtime/parts/os.c:173: static struct sigaction hero_lease_prev_abrt;
    runtime/parts/panic.c:32: static volatile sig_atomic_t hero_runtime_spoke = 0;
FAIL runtime/threads
  objects that survive between calls: 49 against a floor of 39, which the tree has outgrown by more than 25%.
```

Baseline: I copied the five patched files aside, `git checkout HEAD --` them,
re-ran the same suite with the same binary, got **8 passed, 0 failed**, and
restored. So both failures are this change's, exactly. The landing owes three
`SHARED_BY_DECISION` entries in `tests/harness/suite_runtime.hero` (751 lines,
the list begins at :188) and `LEAST_SHARED_STATE` at **:217** raised from 39 to
at least 49. That is a real obligation and it is prose the suite grades in both
directions.

**2. `./heroes run tests/harness/main.hero -- ./heroes run`**

```
FAIL run/abort-lease-freed-by-c-callback
FAIL run/abort-lease-freed-by-c-later
FAIL run/abort-lease-freed-by-c
FAIL run/ffi-out-pointer-is-c-to-free
harness: 133 passed, 4 failed
```

All four say `tripped a sanitiser`. `tests/harness/suite_run.hero:135-148` runs
**every** case three times, at `-O0`, at `-O2` and under `--sanitize`, and fails
the case if stderr contains `AddressSanitizer`. Three of the five new goldens
are bad-frees by construction and one is a double-free, so they cannot live in
`tests/golden/run/` as written. I grepped `tests/harness/cases.hero` (101 lines)
and `suite_run.hero` (224 lines) for an opt-out: the only skip is
`shell.machine_lacks_the_library`, a missing pkg-config library. **There is no
per-case sanitizer opt-out**, so the landing needs a harness change, which
`.claude/rules/verification.md` then judges with the net's own tests.

Baseline: with the fifteen new golden files moved aside, the same suite and the
same patched runtime read **132 passed, 0 failed**. So the patched runtime
breaks nothing that exists; the four reds are the goldens' shape.

**3. `./heroes test selfhost/main.hero` → `675 tests, all passed`** with the
rebuilt compiler.

---

## The tables the brief asked for

### Stock versus patched, ten runs each, `-O0`

Stock numbers are the coordinator's of 2026-09-21, cited and not re-derived.
Patched numbers are mine: ten runs of each binary, `$?` and `wc -c` of stderr.

| program | stock | patched, ten runs | patched stderr | first line |
|---|---|---|---|---|
| `lease070` | 133, **0 B** | 133 ×9, 134 ×1 | **276 B** | `panic: a C function freed bytes this program still leases — 1 lease(s) were live when the allocator refused the free, called from lease070.main` |
| `b_callback` | 133, **0 B** | 133 ×9, 134 ×1 | 277 B | same, `called from bcallback.main` |
| `b_later` | 133, **0 B** | 133 ×6, 134 ×4 | 274 B | same, `called from blater.main` |
| `b_out` | 133, 0 B | 133 ×10 (and 133 ×200) | **0 B** | (silent, correctly) |
| `panic_lease` | 134, 32 B | 134 ×10 | **32 B** | `panic: array index out of range` |
| `control` | 134, 111 B | 134 ×10 | 111 B | `panic: 1 lease(s) never ended — …` |

Two more of mine, both shapes the brief named:

| program | patched | stderr | first line |
|---|---|---|---|
| `assert_lease` (a failed Heroes `assert`, lease live) | 134 ×10 | 50 B | `assert failed: x.len() == 3` |
| `so_lease` (infinite recursion, lease live) | 134 ×10 | **39 B** | `panic: stack exhausted in solease.down` |

The three `panic_lease` / `assert_lease` / `so_lease` rows are **one line each,
the true one**. That is the funnel working, and it is the part of my
predecessor's design I most want kept.

### Under `--sanitize`

`CPATH=docs/panel/173-briefs ./heroes build … --sanitize -o s_<x>`, three runs each.

| program | exit | stderr | our lease line present? |
|---|---|---|---|
| `lease070` | 134 ×3 | 1072 B | **0** — `SUMMARY: AddressSanitizer: bad-free giveaway.h:3 in eat` |
| `b_callback` | 134 ×3 | 1125 B | 0 |
| `b_later` | 134 ×3 | 1076 B | 0 |
| `b_out` | 134 ×3 | 1246 B | 0 — `double-free beside.h:16 in free_out` |
| `panic_lease` | 134 ×3 | 32 B | 0 |
| `control` | 134 ×3 | 111 B | 0 |
| `so_lease` | 134 ×3 | 15003, 15003, 379 B | 0 — `AddressSanitizer:DEADLYSIGNAL` |

The compile-time yield is **right and I would not change it**. ASan names the C
file and line (`giveaway.h:3 in eat`); our handler can only name the Heroes
caller. Yielding to the better report is the correct order. The guard is
`#if defined(HERO_STACK_GUARD_YIELDS_TO_ASAN) || defined(_WIN32)` at
`runtime/parts/os.c:169`, reusing `runtime/parts/stack.c:77-84`. It has to be
compile-time and not runtime: under ASan `stack.c` compiles only three empty
doors (`stack.c:137-145`), so `hero_stack_say`, `hero_stack_regs` and
`hero_stack_blame` do not exist to call.

---

## Question 2 — composition with `stack.c`

**Does `hero_stack_handler` re-raise or abort?** Both, by arm. Three arms end in
`hero_abort()` (`stack.c:406`, `:441`, `:511`, converted from `abort()` by this
change); the fall-through is `hero_stack_pass_on(signum, si, ctx)` at `:513`,
which chains to the saved disposition or restores `SIG_DFL` and returns so the
fault re-executes (`stack.c:362-376`).

**Does either installer overwrite the other's disposition?** No, and it is not a
matter of order: `hero_stack_guard_install` registers **SIGSEGV and SIGBUS**
(`stack.c:593-594`), `hero_lease_crash_install` registers **SIGTRAP and
SIGABRT** (`os.c:218-219`). Four distinct signals. The order at `os.c:239-242`
still matters for a different reason and my predecessor got it right: the
alternate stack is mapped inside `hero_stack_guard_enter()`
(`stack.c:529-554`, 256 KiB plus a guard page), so the lease handler's
`SA_ONSTACK` is only meaningful after the guard has run.

**Is the alternate stack big enough for both handlers' frames?** Measured rather
than reasoned: `so_lease` exhausts the stack with a lease live, which nests
`hero_stack_handler` (frame walk) then `hero_abort()` then SIGABRT then
`hero_lease_crash` (second frame walk) on the same 256 KiB alternate stack. Ten
runs: exit 134, **39 bytes, one true line**, no truncation, no second fault. Per
thread, as `stack.c:578-581` says: the disposition is the process's and the
stack the thread's, so a C library's own thread reaches neither installer. That
hole is `stack.c:55-59`'s and this change neither widens nor narrows it.

**A SIGSEGV inside C while a lease is live.** `nullwrite_lease.hero` (mine): a C
`*(volatile int *)0 = 1` with a lease live. Five runs, exit 134, **161 bytes**,
one line: `panic: a null pointer was read through — a handle or `ptr` holding
`nullptr` reached C where C dereferences it, at offset 0x0, called from
nullwritelease.main`. No false lease line. The composition is clean because the
SIGSEGV arm ends in `hero_abort()`, which sets the flag before the SIGABRT that
reaches the lease handler.

**What it owes the disposition it saved.** My predecessor replaced the
prototype's drop with chain-then-restore-then-reraise (`os.c:196-213`), which is
`hero_stack_pass_on`'s own shape. I judge that correct and I verified the code
path by reading both. What restore-and-reraise **loses** is nothing on this
machine, because nothing else had SIGTRAP or SIGABRT: I did not construct a
program that installs its own SIGABRT handler from C before `main`, so the
chaining branch at `os.c:197-201` is **UNRUN**. Under `--sanitize` the question
does not arise: the whole block is compiled out, so ASan's SIGABRT handling is
untouched, and the 1072-byte ASan report above is the proof.

---

## Question 3 — what the report can name

**It names the Heroes function, and that works.** `hero_stack_blame(pc, fp, lr)`
(`stack.c:331`) walks the interrupted context, which is intact because the
handler runs on the alternate stack. Measured: `lease070.main`,
`bcallback.main`, `blater.main`. The line it gains is `, called from
<module>.<function>`, and the cost is six lines in the handler
(`os.c:186-190`) because the walk already existed for the stack guard. That is
the cheapest thing in this diff and the most valuable.

**It cannot name the C callee.** The frame walk returns the first *Heroes*
symbol; `hero_stack_is_heroes` filters for the mangled prefix. ASan names
`giveaway.h:3 in eat` and we cannot.

**It cannot name WHICH lease.** `hero_live_held` is an `_Atomic int64_t`
counter, `runtime/parts/alloc.c:98`, bumped at `hero_alloc_held`
(`alloc.c:361-364`) and dropped at `hero_release_held` (`alloc.c:367-369`). No
pointer, no name, no table. Naming one would cost a table of live leases written
at those two sites plus a name per `.lease()` carried from the call site into
the runtime, which is a pointer per lease and an emitter change: it stops being
a runtime-only change at that point. Priced here, not built, and I would refuse
it on §1.1's ceiling if it were proposed.

---

## Question 5 — the goldens, and two defects in them

`tests/harness/expectation.hero:89-96`: `!panic: <m>` is judged by
`strings.contains(text: err, needle: m)` plus a non-zero exit. So the 133-versus-134
nondeterminism is genuinely free, as the coordinator said, and my predecessor
was right to spell the three reporting cases with `!panic:`.

Two things that containment then costs:

**`abort-panic-with-a-lease-live.expected` does not guard the regression it
names.** Its whole content is `!panic: array index out of range`. Containment is
satisfied by a stderr that *also* carries the false second line, which is
precisely the 282-byte two-line output this case was written to forbid. The case
would be green on the prototype it exists to refuse. It needs the exit-code or a
byte-count assertion the run form does not have, or the run form needs an
exact-stderr option. I did not build the prototype to confirm the green; the
claim rests on reading `expectation.hero:90`, and I mark the *would have been
green* half **UNRUN**.

**`ffi-out-pointer-is-c-to-free.expected` is `!exit: 133`, and `!exit:` is
exact** (`expectation.hero:84-88`). On this Mac it held for **200 out of 200**
runs, so it is not flaky here. But 133 is SIGTRAP, a Darwin allocator fact;
glibc's double-free path calls `abort()`. `.github/workflows/ci.yml:290` runs
the net on Linux x86-64, Linux arm64, Darwin arm64 and Windows. I did **not**
enter the Linux container, so *this case fails on Linux* is a question and not a
measurement. What is measured is that the case is currently red on Darwin too,
for the sanitiser reason above.

---

## needed_for_self_hosting

**No.** Principle 0 (CLAUDE.md § 2): the compiler does not lease anything to C
that C frees. This enters on the thesis side, as a design.md Part 11 effect, and
it has to earn that on the honesty of the sentence it prints, which is exactly
what the veto is about.

---

## prediction

Falsifiable, named instrument, checkable at the commit that closes defect 070:

**If the landing diff does not touch `tests/harness/suite_runtime.hero`, then
`./heroes run tests/harness/main.hero -- ./heroes runtime` at that commit
reports `runtime: 6 passed, 2 failed`, naming `runtime/parts/os.c:172`,
`:173` and `runtime/parts/panic.c:32` as unnamed shared objects and `49 against
a floor of 39`.** To make it green the file needs three `SHARED_BY_DECISION`
entries and `LEAST_SHARED_STATE` at line 217 moved from `39` to at least `49`.
I measured both numbers today and the baseline is 8 passed, 0 failed.

Second, tied to a line count: **the sound repair of the veto costs zero net
lines.** If the landing reworks the wording instead of adding a discriminator,
`git diff --numstat runtime/parts/os.c` at that commit shows an added count
within 5 of today's 102. If it exceeds 130, a discriminator was built, and then
I want the Linux measurement with it.

---

## condition — what lifts the veto

Any one of these, measured:

1. **The sentence stops asserting the cause.** Reword to what the runtime
   observed (the process is aborting, N leases are live) with the C free named
   as the likely reason rather than as a fact, in §4.17's `guess` register, and
   add `cabort_lease` and `cassert_lease` as goldens so the boundary is written
   down the way `b_out` writes down the other one. Zero new lines. This is the
   route I recommend.
2. **A discriminator that separates the two, run on Darwin and on Linux.** I
   measured `siginfo_t` dead for the SIGABRT half on Darwin (table above). A
   SIGTRAP-only report is sound here and loses 24% of the true cases (152/48 of
   200), and is UNRUN on glibc. If somebody finds a field or an allocator hook
   that works on both, the assertion becomes true and the veto goes.
3. **A measurement that my two programs are not a class.** They are ten and nine
   lines of Heroes over four lines of C; I do not expect this one to be found,
   but §12 holds a refusal to the same standard as a feature and that is the
   fact that would make mine wrong.

None of the three touches `selfhost/`, so the ceiling is not what this sitting
turns on. **What I would keep unchanged if the veto is answered**: the
`hero_abort()` funnel, the ASan yield, the chaining, and the frame-walk name.
Those four are my predecessor's and they are right.

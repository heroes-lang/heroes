# 115 — The floor is the thread that ran `main`

Date: 2026-09-06 · **full panel, five judges** · convened by the coordinator
during M-thread-stacks, after step 1 made the stack guard per thread and the
measurement that followed found a second question behind the first.

Status: **settled 2026-09-06 by author instruction**, which delegated the choice
rather than reading the sitting. See § Author's verdict, which says so in those
words and says what a no would still cost.

Lane: **full**, and the lane paid for itself in the two seats that do not
compile. The historian found that the shape proposed is settled practice in four
runtimes and that the number proposed carries a Windows defect the closest
precedent still ships. The ergonomist wrote the program twice, **withdrew its own
wording**, and produced the sentence this sitting turns on. A soundness lane
would have adopted a number nobody could defend and no spec sentence at all.

## The proposal, as put

> `runtime/parts/spawn.c`'s `hero_thread_spawn` creates every thread with DEFAULT
> attributes — `pthread_create(&t, NULL, ...)` on POSIX, `CreateThread(NULL, 0,
> ...)` on Windows — so the stack a Heroes worker gets is whatever the platform
> hands it. Proposal: pass an explicit size (`pthread_attr_setstacksize`,
> `dwStackSize`) so a thread THIS RUNTIME starts gets the same stack on every
> platform. The number proposed is 8 MiB, POSIX main-thread parity.

## Why it was asked

M-thread-stacks step 1 made the guard's bounds and alternate stack per thread, so
a worker that runs out of stack now says which function did it, at exit 134, on
all three platforms. That is what panel 107 adopted. The measurement taken
straight afterwards found the other half of the same subject: the stack a worker
gets is 536,576 bytes on macOS, 8,388,608 on Linux and 67,108,864 on Windows,
which is **125:1 across platforms for a thread this runtime creates itself**, and
**15.6:1 within one machine** — the author's own Mac gives a worker one sixteenth
of what it gives `main`.

Panel 107 refused three mechanisms and concluded *"the number cannot be made
uniform on any of these platforms"*. Every one of those refusals is about the
**process's main stack** and about **link flags**. A thread the runtime creates is
a different object: the creator passes the size as an argument, on all three
platforms, with no linker and no operator override. The ffi seat put it exactly:
`pthread_attr_setstacksize` on a runtime-created thread is a fourth mechanism and
is not on 107's list, **because on 2026-09-04 the runtime created no threads**.

## The verdicts

| seat | verdict | section | cost / delta | condition to flip |
|---|---|---|---|---|
| compiler-engineer | **object** (not veto: neither Part 5 nor §1.7 is engaged, and it says so rather than stretching them) | design.md §1.12 `:539-576`, its Principle 0 clause `:558-563`; §1.0 `:112-124` | `spawn.c` 218 → ~265, under §11's ~300; **zero** in lexer, checker, emitter; `HERO_RUNTIME_ABI` unmoved, 209 emission goldens untouched, and it **withdraws the `doctor` row and the `hero_os.h` cost it charged at panel 107** — a link flag can be inert on a platform and owed a row, an argument to `pthread_create` cannot | the number stated as a **floor** (`max(platform default, N)`), Windows untouched; the refused-size case **panicking** rather than falling back; option (d) refused by name |
| ffi-pragmatist | **object**, with a **veto scoped to two things**: the Windows arm at 8 MiB, and the three-parameter spelling | design.md §1.11 `:435`, §1.12 `:545`, §4.19 `:2002, :2088-2095`, Part 7.13 `:2553`; `selfhost/cli/flags.hero:54-105, :137` | ABI stays **21**; emitted C **md5-identical** across default and 8 MiB runtimes for six programs; sqlite, curl, sdl, ledger and threads all exit 0, `--sanitize` included. The widened spelling is `error[ffi_call_shape]` at `examples/threads/main.hero:37:5` and breaks every program that binds the door | Windows left alone or the number read off `/STACK`; the size a **constant inside `parts/spawn.c`**; the delivered size **verified after the fact**, never by the return code |
| historian | **approve (advisory)**, with two departures flagged | — | — | — |
| spec-warden | Q1 **object** · Q2 **veto** on every wording that says the abort names the function, **object** on the rest | design.md §1.6 `:284-295`, `:297-306`, `:347-349`; §1.2 `:190-204`; §1.4 `:223-235`; CLAUDE.md §12 | spec **3830**, headroom **266**; nine wordings measured; **no budget veto is available on either question and it said so before arguing** | Q1: the number argued from measured frames rather than a platform default, **and** recorded as never reaching the spec. Q2: wording A (+4) or H (+7) paid by a measured removal, once a blind seat re-runs 107's task against the loud compiler |
| llm-ergonomist | **approve H** first, **approve A** second, **and withdraws its own Version Two** | spec only, as the role requires | — | — |

### What the ffi seat measured, and it corrects the coordinator

The brief told this seat that a C library's own thread calling back into Heroes
now stops by name, and invited it to falsify that. It confirmed the observation
and then **built the counterfactual the coordinator had not**: it took the emitted
C, deleted the single line `hero_thread_guard("foreign.worker");`, and recompiled
against the same M-thread-stacks runtime.

    exit 132, empty stderr

**So step 1 does nothing whatever for a foreign thread**, and the coordinator's
sentence — *"the milestone that closed two days later built the door"* — is true
only for threads this runtime starts. `hero_stack_guard_enter()` has exactly two
callers, `hero_args_set` and `hero_spawn_enter`, and a library's thread reaches
neither. What saves the program is panel 111 R9's `hero_thread_guard`, from
M-isolated-threads, and the sitting records that rather than letting the tidier
story stand. The seat also tried the evasion — passing the callback as a bare
`ptr`, the SQLite shape — and got `error[type_mismatch]` at exit 1, so the
guard's over-approximation is sound.

### What the ffi seat found that nobody had asked for, and it is the trap

`pthread_attr_setstacksize`'s return code **is not a portable check**, measured on
both platforms rather than read:

| ask | macOS arm64 | Linux x86-64 glibc |
|---|---|---|
| `PTHREAD_STACK_MIN` / page | 16384 / 16384 | 16384 / 4096 |
| 4096, below the minimum | returns **0**, clamps to 28,672 | **EINVAL**, attr keeps the default |
| 8 MiB + 1, not page-aligned | **EINVAL**, attr keeps the default → **536,576** | returns **0**, rounds to 8,388,608 |
| 1 TiB | **ok**, and the thread is created | **ok**, and the thread is created |

The two platforms refuse **opposite** things and neither refuses an absurd size.
Demonstrated end to end: 8 MiB + 1 with the return discarded, recursion to 20,000,
gives `panic: stack exhausted in depth.down` — **blaming the author's recursion
for a runtime that silently got 536,576 bytes**. An implementation that drops the
return value restores the exact defect it was written to fix.

The seat built the robust spelling and ran it: ask the OS what it **gave**, in
`hero_spawn_enter`, and panic if it is short. It must live in `spawn.c` and not in
`stack.c`, because `stack.c`'s ASan arm makes `hero_stack_guard_enter` empty and
every program that declares an `extern` runs its Linux leg under `--sanitize`
(CLAUDE.md § Commands).

### What the compiler seat measured

`probe.c` against the real runtime, frame 26,624 bytes, macOS arm64:

| | worker stack | worker depth | main depth |
|---|---|---|---|
| (a) defaults | 536,576 | **19** | 312 |
| (b) 8 MiB | 8,400,896 | **313** | 312 |
| (c) 16 MiB | 16,789,504 | **628** | 312 |

**19 against 312 on one machine, one program, one runtime** is the number this
sitting would put to the author, and it is sharper than the cross-platform 125:1
the brief opened with.

RSS with all 256 slots alive, each thread having touched 64 KiB: 22,528 KB at
(a), 22,528 KB at (b), 22,544 KB at (c) — **+16,384 bytes, 0.07%, at the full
bound**. So *"the reserve is free until touched"* is verified on this platform and
the cost is address space alone: +1.87 GiB at 8 MiB, +3.87 GiB at 16 MiB. No
thread creation failed at 256 × 16 MiB.

Two facts recorded because nobody would have guessed them. **Darwin returns 12,288
bytes more than asked** (8,388,608 → 8,400,896), absorbed by `stack.c`'s 1 MiB
window. And **the alternate stack is 256 KiB, which at today's macOS default is
48.9% of the worker stack it guards**.

### What the historian found, and it decides the Windows arm

The shape is settled practice, verified with sources: **Rust** documents it in one
sentence (2 MiB on all Tier-1 platforms, *"the stack size of the main thread is
**not** determined by Rust"*), **libuv** shipped it in 1.45.0 (8 MB, *"consistent
across platforms and architectures"*), **Zig** pins 16 MiB in one constant,
**Nim** passes 2 MiB − 4 KiB on both POSIX and Windows, and **LLVM** and **Swift**
both did it specifically to escape Darwin's 512 KiB. Nobody reverted, and the seat
names that it searched for a revert and did not find one.

Two departures it flagged, and both survived into the resolution:

1. **On Windows 8 MiB is a reduction of 8x, not a normalisation.** Microsoft's own
   page says the header's reserve is the default *"for all threads and fibers"*,
   so this project's `/STACK:67108864` already gives created threads 64 MiB. The
   third row of the brief's table is not the platform's answer; it is this
   project's answer, already given.
2. **The nearest precedent carries a Windows defect.** Mozilla bug 958796:
   without `STACK_SIZE_PARAM_IS_A_RESERVATION`, the size parameter sets the
   **commit**, not the reserve. libuv's `src/win/thread.c` calls `_beginthreadex`
   without it to this day. Measured in this repository by the coordinator at the
   seat's request: `runtime/parts/spawn.c:168` is
   `CreateThread(NULL, 0, hero_spawn_trampoline, ..., 0, NULL)` and
   `STACK_SIZE_PARAM_IS_A_RESERVATION` appears nowhere in `runtime/`.

And a third finding the seat volunteered: **HotSpot picks its own number and
doubles it on aarch64** relative to x64 on the same OS — so the most-deployed
managed runtime concluded the axis that matters is the architecture, not the
operating system, and this project's primary machine is macOS arm64.

Q5 confirms panel 107 for threads specifically: **no specification states a thread
stack size.** JVMS §2.5.2 permits fixed or growing stacks and states the failure
instead. SRFI 18's `make-thread` takes a thunk and a name. The Go spec does not
contain the word *stack*. **Ada is the extension that sharpens the finding**: it
is the one specification that names the knob (`pragma Storage_Size`) and it still
refuses the number — *"the value of the Storage_Size attribute is unspecified"*.

### What the warden measured, and the veto is a command that was run

Nine wordings, `heroes measure` on copies of the spec, not estimated:

| | wording | max | delta |
|---|---|---|---|
| **A** | `…; integer division by zero and recursion too deep abort.` | 3834 | **+4** |
| **H** | `…; integer division by zero aborts; so does recursion too deep.` | 3837 | **+7** |
| C | panel 107's full property shape, standalone | 3852 | +22 |

Panel 107's own +16–23 estimate is confirmed at **+22**, and **the same property is
available at +4**, eighteen tokens under that sitting's cheapest guess.

**The veto is one command.** `runtime/parts/stack.c`'s handler names the function
only `if (who != NULL)`, and the name comes from `dladdr`:

    $ strip deep && ./deep
    panic: stack exhausted
    exit=134

**No name.** Shipping stripped is ordinary, so every wording promising a name —
including panel 107's own tabled shape and the ergonomist's first amendment —
would have been **false on the day it landed**. That is refusal #3's exact ground
at 107, and CLAUDE.md §12 would have turned every stripped-binary abort into an
unfixable compiler bug.

**The payment was found and verified by running it.** `spec:105`'s
`` `Point(3, 4)` does not exist. `` is **−12**, and the seat checked the compiler
rather than assuming: `error[missing_label]: `Point`'s fields are always named —
this one is `x``, one per field, the whole rule in the message, fix `certain` and
therefore machine-applicable under §8. **H + the removal is 3825, net −5: the
document shrinks.**

And the seat ruled panel 107's queued payment **lapsed** under §1.6 `:297-306`: it
named M-corpus-depth close, which passed on 2026-09-04, and a headroom of ≤ 323
that panels 110 and 111 have since overtaken. A prediction is re-decided, never
renewed.

### What the ergonomist found, and it is the sitting's centre

Asked to write the eight-thread program from the document alone, it wrote it twice
and **the silence produced the more dangerous program**. Under the document as it
stands, the careful reader builds a depth budget: a `MAX_DEPTH` constant, a
`depth` parameter, an `i64?` return — and then hits the callback boundary, where
`hero_thread_spawn`'s body is `(function(i64) -> i64)` and an error cannot cross.
The only ways to collapse it are `.must()` and `.default(v)`, and the program's
answer is a number being summed, so a failed thread contributes `0`. **A wrong sum
at exit 0.** Eleven lines against six. The incautious reader writes the naive
program by accident and is fine.

Handed A and H after the warden's veto, it **withdrew its own wording** and gave a
better reason than the token count: *this document never defines "abort"* — it
teaches the word by **nine instances**, so a tenth instance in the same list
inherits the whole meaning free, while a property stated beside the list asks the
reader to trust a new **kind** of claim. Its own wording also named a **resource**
(*bounded by the stack*), and naming a resource is what invites the number it had
itself reported wanting. A and H name none. And it noticed that its own sentence
would have made stack exhaustion **the only abort in the document whose output is
specified**, in a section whose authority comes from uniformity.

Its verdict on the strip finding is worth quoting for the record: losing *naming
the function* costs **zero lines**, because the name matters when a reader is
looking at a terminal, not when they are choosing a return type.

**And the sentence the sitting is named for:**

> A loud implementation plus a silent document is a trap that hides its own
> evidence.

The seat wrote its guarded program **while the compiler was already loud** — step 1
had landed hours earlier — because the implementation's loudness cannot reach the
author of a first draft. The document is the entire causal channel. Worse: the
guards that silence produces **convert the loud failure back into a quiet one**,
so the compiler team measures *does stack exhaustion abort*, gets green on three
platforms, and concludes the hole is shut while every generated program has
already routed around the instrument.

That is also the warden's flip condition met: a blind seat re-ran 107's task
against the loud compiler and showed the difference.

## Where the seats disagreed

**The compiler seat and the ffi seat wanted opposite things about Windows and
converge on the same act.** The compiler seat asked for a floor with Windows
untouched **or** the reservation flag; the ffi seat vetoed the Windows arm
outright. A floor leaves Windows alone, so the flag is never needed and the
defect the historian found cannot be imported. Both are satisfied by doing less.

**The warden and the ergonomist disagreed about the wording and the ergonomist
lost on a measurement**, which is the disagreement working. The warden's veto came
from `strip`; the ergonomist accepted it and then found two further reasons of its
own to prefer the shorter form. Neither seat argued the other's ground.

**The warden objected to 8 MiB and the compiler seat objected to 8 MiB for
different reasons** — a foreign default versus an equalizer that cuts the widest
platform. The floor answers both, and it answers them by removing the number
rather than by choosing a better one.

**Nobody defended option (d).** The compiler seat priced it at
`HERO_RUNTIME_ABI` 21→22, `selfhost/emit/decls.hero:79` and `:335`, 209 goldens,
two sites in `seed/heroes.c` and ten hand-written `extern` groups in `examples/`;
the ffi seat compiled it and got `error[ffi_call_shape]` on the author's line.
It is refused by name below.

## The resolution — provisional, and the robust one rather than the cheapest

**The conservative resolution was (a), do nothing**, and CLAUDE.md §4 requires
this file to say what that leaves standing: a worker thread on the author's own
machine reaches **19** levels of a recursion that `main` takes to **312**, from one
line of Heroes, with nothing in any document saying so. That is a §1.12 surprise
inside one program on one machine, and it is not a platform fact.

**Adopted, and it is smaller than the proposal rather than larger: the floor is
the thread that ran `main`.**

1. **POSIX only.** `hero_spawn_enter`'s creator asks for `max(the platform's own
   default, the size of the thread that ran main)`, rounded up to a page, through
   `pthread_attr_setstacksize`. The main thread's size is taken once at
   `hero_args_set`, where `hero_stack_bounds()` already measures it.
2. **Windows is not touched.** `CreateThread(NULL, 0, ...)` stays, so the
   executable's own reserve continues to apply to every thread, the 64 MiB from
   `/STACK` is neither cut nor re-stated, and
   `STACK_SIZE_PARAM_IS_A_RESERVATION` is never needed — which is how the defect
   the historian found in libuv's shipping code is avoided rather than guarded
   against.
3. **The delivered size is verified after the fact**, in `hero_spawn_enter`, by
   asking the OS what this thread actually got. The return code is not consulted,
   because the ffi seat measured that the two platforms refuse opposite things and
   neither refuses a terabyte. Short delivery is a panic naming what was asked for.
4. **No number enters this project.** The floor is a fact about the machine in
   hand, so there is nothing to argue, nothing to age and nothing to state in the
   spec. On macOS the worker rises from 536,576 to `main`'s own 8,372,224; on
   glibc the floor is inert at 8,388,608; on Windows nothing happens at all. **The
   change is one platform, and it lowers none.**
5. **Option (d) is refused by name**, with the price both compiling seats
   measured. It returns only if a program is shown that must choose its own stack;
   none exists in `examples/` or `selfhost/`.

**Why this and not (b) or (c), which is the whole point of the sitting.** The
proposal asked for one number everywhere and every seat that looked found the
number indefensible: 8 MiB is Linux's default imported (warden), it cuts Windows
8x (compiler seat, ffi seat, historian), and the mechanism that would deliver it
on Windows carries a defect its best-known implementation still ships
(historian). What the seats actually wanted was **the surprise removed**, and the
surprise is not that platforms differ — it is that one machine gives its own two
threads a 15.6:1 gap. Removing that needs no number, and Chromium and LLVM both
reached the same answer from the same Darwin measurement, which the historian
verified.

**And the document gets the sentence, at wording H.** `spec:159` gains
`; so does recursion too deep.` (+7) and `spec:105` loses `` `Point(3, 4)` does
not exist. `` (−12), for a measured **net −5**. The removal is §1.4 redundancy
paid back by a compiler that is loud in both directions, which is panel 089's
shape and the only condition under which redundancy is repayable. **The sentence
carries no promise about a message**, because a stripped binary prints none, and
**no word about threads**, on the ergonomist's veto: a thread clause in a document
with no threads is a promise about a construct the reader cannot find, and the
abort is a property of the language that a thread inherits without being named.

**What a veto compelled.** Had the sitting adopted the proposal as put, the
Windows arm would have set the commit size rather than the reserve, on the
platform whose small stack killed this compiler through four rounds of CI; and
the spec would have promised a function name that `strip` deletes. Both were
caught by seats that ran a command.

## What this sitting owes, and it is not optional

**A witness.** The ffi seat measured that **no program in `examples/` reaches the
worker ceiling** — `examples/threads/main.hero`'s `band` is iterative — so the
defect this repair fixes has zero witnesses in the corpus. A corpus program whose
worker recurses is owed with the repair, or the repair is untested by the net.

**A correction.** `runtime/parts/thread.c`'s closing paragraph says a recursing
callback *"still dies at exit 132 with an empty stderr. That is `M-thread-stacks`'
half by name (panel 107)."* Measured both ways by the ffi seat: the shipped
compiler gives exit 134 with a name, and **M-thread-stacks is not what closed
it** — the thread guard is. A reader who later narrows `callback_guard.hero`'s set
would reopen a hole the comment says belongs to someone else.

## Predictions to score

| origin | prediction | instrument | scored at |
|---|---|---|---|
| compiler-engineer | with a floor and Windows left alone, `wc -l runtime/parts/spawn.c` is **≤ 265** and `grep -lc "HERO_RUNTIME_ABI == 21" tests/emission/*.c \| wc -l` is still **209**; either moving falsifies zero compiler-side cost | two commands | M-thread-stacks close |
| ffi-pragmatist | on the Windows box, the proposal's literal spelling raises committed memory by ~8 MiB per live thread, past 2 GiB at 256 — **not run, and named as a question rather than a premise** | `Get-Process` commit size on `docs/environment/windows/WINDOWS-MACHINE.md` | whenever a Windows arm is proposed again; **the floor makes it moot** |
| ffi-pragmatist | no corpus program reaches the 4,500-deep worker ceiling today, so a recursive worker added to `examples/` is exit 134 on macOS and exit 0 on Linux on the current runtime | one corpus commit | M-thread-stacks close |
| spec-warden | with H landed and the removal taken, `heroes measure spec/heroes-spec.md` reads **≤ 3825**, `SPEC_TOKENS` repins and `LEDGER_ROWS` +1; and the sentence produces **zero** `.expected` diffs and **zero** corpus program changes — if it is inert on the corpus it bought nothing measurable | `heroes measure` + the net | M-thread-stacks close |
| llm-ergonomist | sentinel-collapse rate — a self-recursive `T?` whose error is discarded with `.default(` at a fixed-arity boundary — falls at least 10 absolute points from the current document to H: today ≥15%, H ≤5%. Secondary: first-try compile rate moves 0 ±2, hand-rolled depth budgets fall from ≥20% to ≤8% | a generated corpus, grep, no compiler needed | M-thesis-harness |
| historian | no runtime that pinned a uniform thread stack has reverted it — **stated as a failed search with the queries named**, not as an absence | a later search | any sitting that reopens the number |

## Author's verdict

**SETTLED 2026-09-06 by author instruction, and the instruction was a delegation
rather than a reading.** It was given in one line while this milestone was
closing: *carry on until the step is complete, fix every defect, and settle every
open decision with the most robust and safest solution*. That is CLAUDE.md §4's
own rule handed back — the synthesis above already took the robust resolution
over the conservative one and recorded what the conservative one would have left
standing, so the delegation lands on a resolution that was chosen by that
standard rather than on one that has to be re-judged against it.

**What now stands.** The floor is the thread that ran `main`: POSIX only, Windows
untouched, no number entering the project, and the delivered size verified after
the fact rather than by the return code.

**What is refused, permanently and by name**, each with the measurement that
produced it and a return condition in this file that is its only amendment path:
8 MiB on all three platforms; a Windows arm passing `dwStackSize` while
`STACK_SIZE_PARAM_IS_A_RESERVATION` is absent; and a stack-size parameter on
`hero_thread_spawn`.

**And this is recorded as a delegation, not as the author's own reading of the
sitting**, because the two are different objects and a record that blurs them is
false about a person rather than about a number (CLAUDE.md §14). The author's own
yes is still available and would cost nothing to withhold: the floor is 21 lines
of C the compiler never reads, `HERO_RUNTIME_ABI` did not move, and 209 emission
goldens are untouched, so a no is one revert. What a no would also take out is the
spec sentence at `spec:159`, and `spec:105`'s removal would come back with it.

## What the lane gave up

Nothing this sitting can see, and the full lane is why. The two seats that do not
compile produced the resolution's two load-bearing parts: the historian's
Windows-is-already-64-MiB correction, which turned the proposal's third row from a
platform fact into this project's own decision, and the ergonomist's withdrawal,
which replaced a 22-token property sentence with a 7-token list item and gave the
sitting its argument. The soundness lane would have adopted 8 MiB on all three
platforms with a Windows spelling that sets the wrong quantity.

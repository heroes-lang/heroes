# Panel 091 — the saving the architecture already makes

**Convened** 2026-08-25, opening M-separate-compilation step 2, by author
instruction (*"panel ok"*). **Trigger** a diagnostic *class* — whether an
`extern` declaration nobody calls is still verified — plus a sentence of
`spec/**` behind it (CLAUDE.md §4). **Lane** full panel: five seats,
differentiated inputs. **Status** `provisional — author ratification pending`.

**Nothing changes in the emitter. +0 spec tokens, +0 compiler lines.** That is
the sitting's result and it was not the proposal's plan.

## The proposal, verbatim

> Should an `extern` group's `_Static_assert`s and probes be **pruned by
> reachability**?
>
> Measured 2026-08-25: a `print(1)` program emits **166 lines** of C, of which
> **27** exist only for the library's `extern "hero_os.h"` group it never
> touches, plus a **17-line** `HERO_RET_*` macro block supporting them. Under one
> `.c` per module, **138 of selfhost's 155 modules** name no hero_os-backed
> builtin and would carry the group for nothing.
>
> The cost, measured: today an `extern "math.h" function pow(...) -> str` that is
> **never called** is `error[ffi_return_type]` exit 1 on the author's own line.
>
> - **A** nothing · **B** prune the library's group only · **C** prune every
>   unreached declaration · **D** prune per group · **E** something the sitting
>   finds.

Panel 087 queued this as *"half of its own unresolved disagreement"* — whether a
new `extern constant` should cost +2 lines in every program in the language.

## The resolution

**A. Nothing is pruned. The duplication that provoked the sitting is dissolved by
the milestone's own architecture at +0 lines.**

Three measured facts compose: the library is **one module**
(`selfhost/source.hero:109-110`, `LIBRARY_MODULE = "library"`); its externs are
called only from inside it; the milestone delivers **one `.c` per module**
(`design.md:786`). With acceptance row 1 landed — an `extern` is never callable
across a module boundary, panel 030 R2 / panel 033 — every group is emitted in
exactly one translation unit. The 138-of-155 multiplier dies with **no compiler
line written**.

Option **B** would land now, cost +40 lines and 146 re-blessings, and be dead
code at the milestone's close.

## The verdict table

| judge | verdict | section | its own finding |
|---|---|---|---|
| ffi-pragmatist | **veto on B, C and D alike** · approves F-restricted | §4.19, §1.11, §1.12 | **pruning makes a binding UNSAFE, not merely unverified** — seven compiled experiments |
| compiler-engineer | **veto on D** · objects to B and C · approves A | §4.19 (the parser flattens the group), §1.1 | **B is +40 lines and 146 of 146 blessed programs move**; and the brief's "the mechanism already exists" is **false** |
| spec-warden | **veto on C and D** · approves B · approves A as fallback | §1.6, §1.2, §1.0 | C and D prune **zero** author declarations from this compiler: 3 groups, 4 declarations, all reached |
| llm-ergonomist | **veto** | the thesis, locality | *"uses"* has **two readings** and the document picks neither; under one, a line's status depends on another file |
| historian (advisory) | **object to the mechanism, not the goal** | precedent | **nobody prunes by use**; the two systems that verify against real headers both emit **per declaration, at the declare site** |

## What the ffi-pragmatist compiled, and why it ended the ballot

Seven experiments, Apple clang 21.0.0, arm64. Four of them are the sitting.

**1. Pruning the `#include` deletes the type.** A group whose `record` is used and
whose functions are not: **12 errors**, `variable has incomplete type 'struct
timespec'`. A group record's C type *is* the header's type. The compiler-engineer
reproduced the same class independently on a `record` + `constant` group:
`unknown type name 'Widget'`, `use of undeclared identifier 'WIDGET_MAX'`, exit 2
on a legal program. The reason was already written down at
`selfhost/emit_externs.hero:16-21` (panel 061): *"a record lowers to no function,
so no walk of the emitted program can see one."*

**2. An unused group holds up a used one.** `<jpeglib.h>` alone is 8 errors
(`unknown type name 'size_t'`); `<stdio.h>` first, then clean. A two-group program
whose first group is uncalled, pruned: `unknown type name 'FILE'` — **exit 2,
`internal error: compiling the generated C failed`**, a correct program turned
into a compiler bug. The author's only lever on include order is group order, and
pruning silently removes elements from that list.

**3. Pruning takes the `-l` with it.** `emit_externs.libraries` at
`selfhost/emit_externs.hero:110` walks **the same `p.functions` list** `headers()`
does. A library with an `__attribute__((constructor))` — the shape of jemalloc, of
an ASan runtime, of a GTK module — bound by a group whose one function is never
called: today it links and the constructor runs; relinked without `-lhookme`,
**exit 0, no diagnostic, the constructor never runs**. §4.19 says a `link` library
is *declared, not called*; pruning makes it *called, or nothing*. The
compiler-engineer measured the diagnostic that dies with it: an unused
`link "nonexistent_library_xyz"` is `error[ffi_missing_library]` at **exit 1**
today.

**4. §1.12 — the probe is a memory-safety instrument.** `getline` with its
`size_t` out-parameter declared `i32`, **never called**. Today the assert and the
probe fire (verified independently by the coordinator: 4 warnings, exit 0). That
conversion, executed: `canary before = 0x54534554` → `canary after = 0x00000000`.
**Four bytes of an adjacent object destroyed, ASan and UBSan silent, exit 0.** The
instrument that sees it is the probe — precisely what pruning deletes. And a
pruned layout assert is worse than an unknown name: without the `#include`, C
synthesises an **incomplete** `struct timespec` from the first pointer it meets,
compiles with zero relevant diagnostics under `-Weverything`, and reads 16 bytes
from an 8-byte object.

**5. Option B is not free either.** The seat swapped `hero_file_write`'s two
parameters in the runtime header *and* its `.c`, left `HERO_RUNTIME_ABI` at **15**,
and built `print(1)` — a program that never writes a file. Caught **by the
library's 19 lines and by nothing else**. This is panel 037's measured hole
(*"two runtimes with changed behaviour kept the number at 10"*) closed by exactly
what B proposes to delete, and `HEROES_RUNTIME=<dir>` makes the skew reachable by
anybody.

## The historian's finding, and the seat that falsified its reasoning

**Nobody prunes FFI verification by use.** Rust never checks at all (RFC 3484:
*"The Rust compiler cannot check the correctness of the signatures"*); Haskell's
`ccall` never checks; Go's cgo has no declaration block, so *declared-but-unused
does not exist as a state*; Zig, C++ templates and the JVM make laziness
deliberate — JVMS §5.4 **requires** use-point errors even from eager
implementations. The two systems that *do* verify against real headers — GHC's
`CApiFFI` and Python cffi's API mode — emit **per declaration, in the declaring
unit, compiled once**. Neither chose reachability.

**And the sharpest precedent is a withdrawal that does not transfer.** GHC 6.8.3
and earlier `#include`d the header in the generated C so the C compiler checked
the FFI call at the right type — *Heroes' exact mechanism* — and dropped it at
6.10. The two stated reasons are native-backend compatibility and an FFI spec
forbidding macro expansion. Neither applies: Heroes has no native backend, and
§4.19 *wants* the preprocessor (CLAUDE.md §7's `extern constant` accessor exists
so a macro can be reached). The departure survives the historical check.

The cost of the other road is documented and open: Zig needed
`std.testing.refAllDecls` as an escape hatch and it is **broken across four open
issues**; Nim's DCE produced `nim-lang/Nim#9884`, **open since 2018**. CLAUDE.md
§6 says copy Nim's FFI surface — precedent says do not copy this part of its
codegen.

**Its reasoning rested on an inference it flagged as one** (CLAUDE.md §1: *"the
connective is 'so'"*) — *"a calling TU already `#include`s the header, so clang
type-checks the call site for free"*. **Two seats compiled it and it is false.**
A calling TU does not get the header; on clang 21 that is a hard error, `call to
undeclared function`. So **F cannot move the `#include`**. Flagging it is what
made it cheap to refute, which is the behaviour this project wants from a seat.

## Where the seats disagreed

**On whether F is a new option at all.** The ffi-pragmatist proposed
**F-restricted** — the `#include` and the `-l` stay in every unit that needs them,
unconditional; only the assert/probe block moves to the declaring module's TU,
emitted whole for every declaration, reached or not. The compiler-engineer
answered that **F is not a sixth option**: it is what acceptance row 1 already
decided, and today, with one translation unit, declare-site emission *is* current
behaviour. Cost in this sitting: **+0**. Both seats are describing the same
destination; they disagree about whether anything must be built to reach it, and
the engineer's reading is adopted because it is the one that writes no code.

**On B.** The spec-warden approved B (free, true, +0 tokens, and the FFI
paragraph's own six words — *"Anything beyond **this document**"* — scope it to
what is *not* in the spec, so the library group is not what that sentence
governs). The ffi-pragmatist vetoed B on the runtime-skew measurement. The
compiler-engineer objected on order. **The veto stands and A absorbs it**: A gives
up nothing B gives up, and costs nothing B costs.

**On the ergonomist's third wording.** It proposed spending ~6 tokens to say *"in
the group — declared, not just called"*. Under A the sentence is already true and
no token is spent. Recorded rather than adopted: if a future sitting reopens
pruning, that wording is the one to price first.

## What a veto would compel

The ffi-pragmatist's veto binds any reachability option. To lift it, someone must
show that a runtime signature skew under an unchanged `HERO_RUNTIME_ABI` is caught
by something other than the library's assert block. The seat searched
`cli_flags.hero`, the cache-key path and the stamp, and reports the positive form
rather than a failed search: the stamp is a **number**, and panel 037 already
measured that it does not see changed behaviour.

The compiler-engineer's veto binds **D** specifically, on `design.md §4.19`: *"the
parser flattens it … no later pass ever learns the word 'group'."* D requires the
IR to carry a group id — a design.md amendment, not a cost question.

## The coordinator's own errors, recorded

**1. The brief asserted a mechanism that does not exist.** It said *"THE MECHANISM
ALREADY EXISTS: `emit_builtins.reachable(p, s)` … the FFI half does not consult
it"*, implying that consulting it would suffice. **False, and the
compiler-engineer found it first.** `emit_builtins.hero:171-187` `calls_of`
discards `.extern_fn` in **both** arms, so `reachable` cannot see an extern at
all; B needs a second worklist, which is **30 of its 40 lines**. This is CLAUDE.md
§1's named shape — an inference from reading one call site, written into a brief
as a measurement — committed by the seat whose job is to police it. Verified
after the fact by the coordinator.

**2. A token delta quoted without its wrapping.** The brief priced option C's
rewording at +4 and +5. The spec-warden measured **+2 and +3** and gave the
reason: the delta moves with the line breaks. Both runs are correct and the brief
was not reproducible. **A quoted spec delta states its wrapping or it is not a
measurement.**

**3. A line count that two seats counted differently, and both were right.** The
brief said the library group costs **27** lines; the ffi-pragmatist measured
**19**. Reconciled by the coordinator: 19 is asserts (13) + probe functions (6);
27 adds the 7 `#line` directives and the `#include`. Neither number is wrong and
neither was stated with its unit. The engineer separately measured the macro
block's departure exactly: `examples/tree/main` 2217 → 2173 = **44** lines.

**4. A count that missed a duplicate.** The brief and the warden both reported 3
author groups over 3 headers. `cli_toolchain.hero:34` and `emit_literal.hero:40`
**both name `stdlib.h`** — two groups over one header, which matters to any
group-keyed option.

## What this sitting found that it was not convened for

Four items, all measured, none of them this sitting's to spend. They go to the
queue.

- **`spec:204` commands what the compiler does not check.** It says *"a
  **parameter** and a **field** are declared at the header's own width and sign"*.
  Measured: `pow(x: f32, y: f64)` against `double pow(double, double)` is **exit
  0**, prints `1024.0`; `memset(..., n: i32)` against `size_t` is **exit 0**;
  `memset(s: i64, ...)` against `void *` is **exit 1**, `ffi_parameter_type`. The
  check sees **kind**, not width or sign. §12 says the compiler has the bug. Found
  by the llm-ergonomist from the spec alone, flagged as an inference, measured by
  the coordinator.
- **A bindings module is invisible to `heroes check`.** `heroes check badbind.hero`
  is exit 0 with zero output — `check` never runs clang — so `build` is the only
  thing that verifies a group. Found by the ffi-pragmatist, unasked.
- **§4.19 owes one sentence about include order.** A group's `#include` may be
  load-bearing for a *later* group's header (the libjpeg finding). §4.19 does not
  say that group order is the author's include order and is never reordered or
  thinned.
- **Acceptance row 1 breaks the compiler's own source in seven places.**
  `cli_toolchain.system(command: ...)` is called qualified from `cli_libraries`,
  `cli_doctor`, `cli_mutate` and `cli_verbs` — 7 sites, 4 modules — and compiles
  today. Row 1 makes that a type error; the repair is row 1's own prescription, a
  Heroes wrapper in the declaring module.

## Predictions to score

Scored at **M-separate-compilation close** unless stated otherwise.

| seat | prediction | falsifier |
|---|---|---|
| compiler-engineer | with row 1 landed and no prune, a `print(1)` program's own `main.c` carries **0** `heroes-ffi-return` assertions and **0** `hero_ffi_probe_` functions, while `grep -rc heroes-ffi-return` over the build directory is **≥ 13** | `main.c` still carries the library's 13 — then the duplication survived the architecture and B was needed |
| compiler-engineer | if B lands, `grep -c reachable_externs selfhost/*.hero` at close is **0 or 1** — deleted, or present and called by nothing | it is called by something that needed it |
| ffi-pragmatist | `examples/sqlite/main.hero` split into a bindings module plus a caller compiles, links and runs **with no shim**, and `build` on the caller reports `ffi_return_type` **on the bindings module's own line** if any declaration is wrong | any bindings module whose group is `#include`d in two TUs producing a duplicate symbol at link |
| ffi-pragmatist | a two-group program with an uncalled `stdio.h` group before a called `jpeglib.h` group is **exit 2** under B/C/D and **exit 0** under A | the day a pruner lands |
| spec-warden | B removes **43 of 167** emitted lines per module and saves **≤ 0.5 s** of clang time across 155 units (measured 3.45–3.59 s → 3.20–3.23 s) | a larger saving, which would make the §1.0 argument the sitting says does not exist |
| spec-warden | C and D remove **0** additional lines from `selfhost/` | either prunes even one author declaration from this compiler |
| historian | if reachability ships without an eager mode, within 6 months an `extern` will be found whose signature was wrong the whole time it sat unreached | force full verification at the 6-month mark and find zero failures |
| llm-ergonomist | model-generated bindings declare more than they call (median ≈ 6:2) in **≥60%** of samples, and **≥30%** carry a wrong return type among the *unused* declarations | measure the **binding**, not the run: re-compile each sample with every declaration called once |

**The ergonomist's trap, recorded because it would invalidate the measurement:**
a pruning option scores *better* on a naive "does the program run and print the
right thing" first-try rate, because the wrong declarations are not on the path.
A harness that measures the run would reward the option this sitting refused.

## Process notes

**The freeze held.** The working tree was untouched from the first brief until
this file was written. Two addenda were sent mid-sitting — the seven qualified
cross-module `system` calls, and option F — following panel 086's precedent; both
carried measurements rather than instructions, and both told the seats their own
numbers win.

**Four of five seats corrected something the coordinator wrote.** That is the
sitting working, and it is also the argument for differentiated inputs: the false
mechanism was caught by the seat that compiles, the unreproducible token delta by
the seat that counts, the ambiguous wording by the seat that only reads the spec,
and the wrong shape of the whole question by the seat that reads other languages.

**The llm-ergonomist's blind A/B failed** and it said so unprompted: `spec:203-204`
reads variant α verbatim, so the seat identified which was current on sight. A
label-stripped comparison against a document the seat is given cannot be blind.
Recorded for the next sitting that tries one.

## Author's verdict

*Pending.* Queued in `docs/debrief/DECIDE.md`.

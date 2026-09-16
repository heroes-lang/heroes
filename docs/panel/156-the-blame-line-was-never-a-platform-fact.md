# Panel 156 — the blame line was never a platform fact

2026-09-16, at M-check-completeness, **convened by the author** after the
coordinator diagnosed a red CI and found the repair's direction contested. Five
seats. **Resolution provisional — author ratification pending**, and ratified the
same day by the delegation recorded at the foot of this file.

The sitting was called to ask which of three platforms is right about a crash
message. It found that the question had the wrong axis, that the green leg was
asserting a self-contradiction, and that every repair it needed was buildable and
small — and three of the four were built and measured before the synthesis was
written.

## The proposal, verbatim as put to the seats

> A Heroes program that dies because C read through a null pointer says three
> different things on the three platforms. Which one is correct, and what is
> repaired?

R1 which blame line is correct · R2 is the arm64 walk's failure a defect · R3
must an aborting program keep what it printed · R4 is Windows' missing arm a
defect · R5 what to do about the red `main` now.

## What ships today, measured on all three

`tests/golden/surface-fixtures/nullread/main.hero` at `-O0`. macOS on this Mac;
Linux in the container from `docs/ref/environment/linux/Dockerfile`; Windows over
`ssh win`, tree sent by `tar`, built with `seed/README.md`'s own line.

| | macOS arm64 | Linux x86-64 | Windows x86-64 |
|---|---|---|---|
| exit | 134 | 134 | **139** |
| stdout | `7` | empty | empty |
| stderr | `panic: … called from node_value` | `panic: … called from main.main` | **empty**, `Segmentation fault` |

Windows' row is word for word what this fixture's own comment describes as the
state **before** defect 045's repair — *"It died at exit 139 with both streams
empty, which design.md §1.12 forbids by name."* So that repair never reached one
of the three platforms, and the condition §1.12 names by name was live.

## The verdict table

| seat | R1 | R2 | R3 | R4 | R5 | veto |
|---|---|---|---|---|---|---|
| **compiler-engineer** | the Heroes caller | **defect, repaired +7 lines** | keep it, by buffering not flushing | defect, arm writable | red stands; narrow the row | not cast |
| **ffi-pragmatist** | **object — do not pin a symbol** | no objection, one caution | object to any `fflush`; refuse `_IONBF` | **approve, built and measured** | land R4 first | not cast |
| **llm-ergonomist** | the Heroes caller, if only one | — | **the surviving output is what located the defect** | silence is not acceptable | — | not cast |
| **spec-warden** | document does not bear on it | **defect, worse than briefed** | document silent; refuse the sentence | defect, zero tokens | **red stands** | **cast**, narrowly, against buying a spec sentence |
| **historian** | Go names its own frame, deliberately | — | **POSIX excludes `fflush`; glibc removed it** | `[1]` is documented | — | advisory |

## THE HINGE, SETTLED: it is an ISA fact, not a platform fact

The compiler seat instrumented the walk rather than reasoning about it:

```
DBG pc=0x1045427a8  fp=0x16b8c6520  lo=0x16b0cc000  hi=0x16b8c8000
DBG pc_sname=node_value
DBG iter=0  fp=0x16b8c6520  next_fp=0x16b8c6550  ret=0x1045427e4  ret_sname=main
DBG iter=1  fp=0x16b8c6550  next_fp=0x16b8c6bb0  ret=0x1802004e4  ret_sname=start
DBG iter=2  next_fp=0x0  ret=0x0  break: ret==0
```

Registers right, bounds right, `dladdr` answering, chain complete. **The Heroes
frame is not in it.** `otool -tvV -p _node_value` shows `sub sp / str / ldr /
ldr x0,[x8]` and **no `stp x29, x30`**: on AArch64 a **leaf function saves no
frame record**, so at the fault `x29` still points at `h_main_main`'s own frame,
whose saved return address is C's `main`. The walk asks *who called
`h_main_main`* and never *who is `h_main_main`*. On x86-64 the `call`
instruction pushes a return address unconditionally, so the same walk lands one
frame lower and finds it.

**x86-64 was right by accident of its instruction set, and Linux on arm64 would
fail identically.** The missing datum is the link register, `x30`.

## And the green leg was asserting a self-contradiction

The spec-warden found what the brief had not: `runtime/parts/stack.c:450-457`
says *"Nothing is printed when the walk finds no Heroes name"*, and that is
**false of the code beneath it**. `hero_stack_blame` returns `first` — the
`dladdr` symbol at the faulting PC — so `who != NULL` and a non-Heroes name is
printed. macOS's `called from node_value` is that path. **The suite has been
pinning the runtime's contradiction with its own comment as the required
answer.**

## AND THE AXIS IS WRONG: the blame name is an optimisation-level fact

The ffi seat's objection, measured on one machine:

| level | what the same program on the same Mac says |
|---|---|
| `-O0` | `called from node_value` |
| `-O2` | `called from main` |

because `h_main_main` is inlined away. Real headers ship `static inline` bodies
that vanish the same way, so at `-O2` **there is often no faulting C function
left to name**. Pinning either symbol pins the golden to clang's inliner. The
sitting was convened to choose between two platforms and the answer is that
neither name is a stable contract.

The llm-ergonomist reached the same place from the opposite end, reading only the
specification. Scaling the fixture to the shape any real program has — one
wrapper — it got `node_value` from A and `main.value_of` from B and reported that
**neither located the defect**, because both are function-granular and the defect
is the caller's *argument*. What located it was A's surviving `7`, which said
print #1 had completed.

## What the standards settle, and it is R3

The historian found that two standards decide this outright and not in the
obvious direction. C **§7.22.4.1** makes flush-on-`abort` **implementation-defined**.
POSIX Issue 8 **downgraded its own text** from *"shall include the effect of
`fclose()`"* to *"may include an attempt to"*, and its RATIONALE says why: the
standard requires `abort()` to be async-signal-safe. POSIX **§2.4.3**'s
async-signal-safe list includes `write()` and **excludes `fflush()`**. glibc
shipped flush-on-abort for decades and **removed it at 2.27**, citing *"deadlocks
and data corruption"*; FreeBSD keeps it under a source comment reading
`XXX ISO C requires that abort() be async-signal-safe`.

So the macOS/Linux divergence is **two legal answers**, not one bug, and flushing
from the handler is undefined behaviour by the letter — which is what the
compiler seat independently declined to do on deadlock grounds and the ffi seat
independently objected to.

## The resolution adopted

**R1. No symbol is pinned. The contract is the sentence and the offset;
`called from …` is best effort.** Adopted on the ffi seat's measurement: a name
that changes with `-O` is not a contract, and the historian found no language
that names only the foreign frame by design — Go switches goroutines
*specifically* to blame its own caller of the cgo call and gives the foreign side
as hex, and C symbols there need an explicit `SetCgoTraceback`.

**R2. The walk is repaired, and it is a two-platform repair.** `+7 net lines` in
`runtime/parts/stack.c`: `hero_stack_regs` gains an `lr` out-parameter
(`__ss.__lr` on Darwin arm64, `regs[30]` on Linux arm64, 0 elsewhere) and
`hero_stack_blame` probes `dladdr(lr - 1)` once before the loop. Built and run:
macOS now says `called from main.main`, exit 134, stdout `7`. **Zero lines under
`selfhost/`.** The *"nothing is printed"* comment is corrected to what the code
does, under its own date.

**R3. The output is kept by BUFFERING, never by flushing.** No stdio call enters
the handler. `print` ends every line with a newline (`runtime/parts/panic.c:34`),
so line buffering loses nothing; measured over 200 000 prints, default 0.01 s and
`_IOLBF` 0.23 s against `_IONBF`'s 0.43 s, output byte-identical. **The ffi
seat's refusal of `_IONBF` is adopted and its reason extends to any global
unbuffering: it takes a decision on behalf of a linked C library that shares the
stream.** `_IOLBF` is the weaker act — it changes when a line leaves, never
whether it arrives — and it is what a terminal-attached program already gets.
**And the fixture stops asserting pre-fault stdout either way**, because that is
a libc's choice and not this language's promise.

**R4. Windows gains the third arm, built and measured on the box.** Testing
`ExceptionCode == EXCEPTION_ACCESS_VIOLATION && NumberParameters >= 2 &&
ExceptionInformation[1] < HERO_NULL_WINDOW`, with `HERO_NULL_WINDOW` hoisted
above the platform split — it was inside `#elif !defined(_WIN32)` and invisible
to Windows, which is why the arm could not simply be added. Measured there: 139
with both streams empty became **127 with the panic sentence**, at `-O0` and at
`-O2`. Neighbours unregressed: stack exhaustion still `panic: stack exhausted`,
a null **write** now caught, and a 1 MiB field offset still 139-silent — the
documented gap inherited exactly. **And the file's self-declared inference is now
a measurement**: `GetSystemInfo().lpMinimumApplicationAddress` reads `0x10000` =
65536, which is `HERO_NULL_WINDOW` exactly.

**R5. The red stands and is not bought off.** `suite_surface.hero:283` is
narrowed to the contract — a non-zero exit and the panic sentence — which
**deletes a false assertion** rather than loosening a true one. There is no
`.expected` file for this fixture; both the warden and the compiler seat
corrected the coordinator's brief on that, and `tests/harness/**` is not on
`.claude/rules/records.md`'s append-only list, so editing the row is ordinary
work and `UPDATE_GOLDEN=1` is not engaged.

**R6, which no question asked. No spec sentence is bought.** The spec-warden's
narrow veto is adopted: `spec § 6`'s *"An abort ends the program at once, saying
why"* already covers this, on three measured grounds — the definition of abort is
§ 6's alone and panel 087 vetoed a closed list, so reading *"an abort"* as *only
the named ones* revives the vetoed list; the runtime already raises this one as an
abort; and §1.12 settles it without § 6 at all, since *a Heroes program must not
segfault* and Windows at 139 with two empty streams is that sentence's own
counter-example. The repair therefore costs **0 spec tokens**, measured, and a
purchase priced at +7 to +14 vendored fails Principle 0 for want of a named
removal.

**What the llm-ergonomist wanted and does not get**, recorded because it is the
one place a seat is overruled: two merges, *"an abort never loses it"* onto § 11
and a null-handle clause onto § 13. They are refused on the warden's veto and on
the ffi seat's finding that the second would promise a name that `-O2` deletes.
**What the ergonomist wins instead** is R3's substance without its sentence: the
output survives, and the fixture stops pretending a libc's buffering choice is
the language's promise.

## What a veto would compel

The spec-warden drops its veto on a registered prediction naming a live
instrument that shows models write materially different programs with the
sentence present, or on a fourth stopping behaviour neither §1.12 nor § 6
reaches. The ffi seat casts a veto if any resolution refuses `nullptr` at the
call site — `sqlite3_close(NULL)` and this repository's own shipped
`getaddrinfo(hints: nullptr)` golden are correct C — and objects if
`HERO_NULL_WINDOW` is widened past 65536, now that 65536 is measured as the
platform's own floor. The compiler seat would veto a walk where the `lr` probe
names a Heroes function that is not an ancestor of the fault.

## Predictions to score

| seat | prediction | scored by |
|---|---|---|
| compiler-engineer | R1+R2+R3 show **zero** lines under `selfhost/` and fewer than 20 under `runtime/`; `surface` green on macOS and Linux | `git diff --stat` at the milestone close |
| ffi-pragmatist | with the arm alone, Windows matches the panic line and still fails on the missing `7` and the absent `called from` | the Windows leg |
| ffi-pragmatist | SQLite step 3 of §4.19 needs no shim under this rule | `corpus` |
| spec-warden | the repair moves the spec by exactly **0**: `real 7974`, `cl100k_base 5989`, exit 0, digest unchanged | `heroes measure` at the close |
| spec-warden | the narrowed row is Linux green, macOS green, Windows red at 139 before R4 lands | the three legs |
| llm-ergonomist | guard rate on `== nullptr` for a C-returned handle is at most 20% now, at least 50% with a § 13 clause | M-thesis-harness; no instrument exists today |
| historian | an `fflush` in the handler deadlocks or double-faults on glibc when the fault is inside a stdio call | a fixture nobody has written |

## A defect found unasked, and it is filed

The ffi seat, working at its own boundary, found that **`heroes build --emit-c`
emits C that does not compile** for any `record … tag <name>` binding: it writes
the pre-probe spelling `node *` where the header says `struct node`, 15 clang
errors on a program that builds fine. The compiler is correct —
`selfhost/cli/pointee.hero` probes and re-emits — and only `--emit-c` hands out
the wrong round. It is **defect 048**.

## Process, recorded against this sitting

**Every seat delivered, and the watchdog killed none.** Three of five died at
panel 155 and four of five at panel 087. What changed is written down so it can
be repeated: every brief ordered *write your report file first, then improve it*,
and forbade any command over ~60 seconds; and the stale *~20 minutes to rebuild
from `selfhost/`* in the panel skill was corrected to the measured **59 s**,
which is what had kept seats from measuring at all.

**Two more stale numbers in the coordinator's briefs**, after four at panel 155,
and both were caught by seats rather than by the coordinator: this fixture has no
`.expected` file, and the Windows row was written as unrun hours after the author
had powered the box. The pattern is the same one CL-017 names — a brief assembled
from documents rather than from the tree.

**The seats were independent this time.** No resumed seat was handed another
seat's findings, which is the process rule panel 155 paid for. The agreement
between the compiler seat's ISA finding and the ffi seat's `-O` finding is
therefore two readings and not one.

## Author's verdict

**RATIFIED 2026-09-16, as adopted — BY DELEGATION AND NOT BY READING.** The
author's instruction, in full: *continue until the step is finished with zero
defects and zero open decisions — ratify the panels for me — and at the end push
everything and check that CI is green on all three systems.* The yes is therefore
the assistant's judgement under an authority the author handed over, on a sitting
the author has not read. Recording it as *the author ratified* would credit them
with a reading that did not happen, and nobody would check it
(`.claude/rules/records.md`).

**What the yes settles.** That no symbol is a contract in a crash line. That the
arm64 walk is repaired rather than enshrined, and the runtime's comment corrected
to what its code does. That no stdio call enters a signal handler, on the
strength of two standards and one libc's reversal. That Windows gets its third
arm. That the red is narrowed honestly and not bought. That the specification
buys nothing.

**What it does not settle.** Whether `print` should be line-buffered in general
is adopted here at `_IOLBF` on a measured 15x microbenchmark cost, and the ffi
seat's caution about a shared stream is recorded rather than dismissed — a
program that prints at a firehose and links a C library that also writes to
stdout is the shape that would reopen it. The 1 MiB-offset gap on Windows is
inherited unchanged and stays open. And the ergonomist's two spec merges are
refused rather than answered; if M-thesis-harness ever scores its prediction, the
warden's veto has its falsifier.

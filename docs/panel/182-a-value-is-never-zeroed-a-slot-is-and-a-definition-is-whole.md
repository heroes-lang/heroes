# Panel 182: a value is never zeroed, a slot is, and every definition is written whole

2026-09-28, M-agreed-retention, at the trunk `0fc98107`, which the seats copied.
**The soundness lane** (`.claude/skills/panel/SKILL.md` § Two lanes): the
question changes no surface, no diagnostic and no spec token, only what the
emitter and the ownership pass produce, so the two seats that compile sat, the
compiler-engineer and the ffi-pragmatist, with the completeness critic after
them. Briefs: `docs/panel/182-briefs/`. Reports: `docs/panel/182-reports/`; the
ffi-pragmatist's and the critic's were written out by the coordinator from their
final messages, verbatim, with a header, the harness refusing a subagent's
report file. Panel 181 sat on the same machine at the same time; nothing either
sitting read was changed while the other ran.

**What the lane gave up, and whether it held.** A reader's view. The critic
names four reader-facing edges: a compiler release bug in a program with an
`extern` panics naming the program's C marks first (the ffi-pragmatist's
finding); design.md Part 5's rule 5 is a public sentence that is false today;
under the resolution a lowering bug reaches the author as `internal error:` and
clang's words, where today it is a silent zero; and `--emit-c` and a debugger
show uninitialised temporaries rather than zeros. None changes a program's
meaning or a diagnostic class, so the lane holds, and the four are carried
below.

**Four errors in the shared brief, all found by the critic.**
- *10,902 owned slots*: the brief's pattern had no room for a pointer type;
  1,995 owned slots are `HeroArrayHeader *` or `HeroMapHeader *`, and the count
  is 12,897.
- *182 declaration lines* before the first `goto`: 181.
- *The verifier checks that a value's definition dominates its uses*: between
  blocks only; within a block it checked nothing (`selfhost/ir/values.hero:84-115`).
- *ASan in `run` catches an uninitialised read*: it is blind to that class;
  MemorySanitizer sees it, and only on the Linux leg.

## The proposal

Defect 114 (`docs/work/DEFECTS.md`): the emitted C zeroes refcounted
temporaries and slots at a function's entry, so `h_keywords_keyword`, 21 string
comparisons, zeroes 139 locals on every call; the defect's note of 2026-09-28
read the zeroing as the exit release's precondition and the cost as the
lowering's shape. The sitting decides which zeroing a release or a read needs,
whether a `match`'s arms keep one owned slot each, and what each route risks.

## The verdict table

| seat | verdict | section | cost / delta | prediction | condition |
|---|---|---|---|---|---|
| compiler-engineer | **object** to the defect's framing; **approve (a-min)** now and **(f)** as the architecture step; refuse **(e)**'s analysed slot zeroing; **veto (c)** | §1.7, §1.12, Part 2, panel 021 R3 | (a-min) +75 -7 in `emit/body.hero` and `ir/values.hero`; (f) +398 -23 in five files; `fmt` 0.78 of the trunk's user time for (a-min) and 0.70 for (f) at `-O0` | at the close, if (a-min), (d) or (f) landed: `grep -c '= {0};' seed/heroes.c` at most 22,000; clang's three uninitialised warnings on the seed 0; `fmt` on sixteen copies of `walk.hero` at most 0.85 of the `0fc98107` seed's user time | objects to (a-min) on a program whose C with values unzeroed clang refuses, or that crashes under `-ftrivial-auto-var-init=pattern`, or on which the order check fires |
| ffi-pragmatist | **approve** (a) on values, (b) and (d); **object** to (c) and to (a) on slots without an IR check; no veto | §1.11, §4.19, §1.12, §4.20 | 22 million calls of each route's `h_keywords_keyword`: today 414 ns at `-O0` and 68 at `-O2`, (a) 200 and 35, (d) 86 and 25; 0 diagnostics and 0 sanitizer reports on every route | at the close, whichever of (a), (b), (d) landed: `heroes_runtime.h` changes no declaration and the ABI reads 26; none of the 119 FFI `run` goldens' `.expected` and nothing under `examples/sqlite/` changes; `run` green on all 119 | vetoes a route whose emission makes an extern argument, an address handed to C or a thunk local depend on the prologue's zeroing |

## What the sitting measured

**Four fifths of the zeroing is dead, and zeroing it cost robustness.** The
compiler-engineer classified every `= {0}` of the seed by two instruments, the
critic re-derived it by a third, and all three agree class for class:

| class | zeroed | needed | dead |
|---|---:|---:|---:|
| values (`t<N>`) | 81,680 | 0 | 81,680 |
| synthetic slots (rule 5's, one per owning temporary) | 12,897 | 12,897 | 0 |
| named slots | 8,413 | 7,780 | 633, every one an `@` parameter written from its pointer before `goto bb0` |

Every slot is owed for a reason wider than the defect's note gave: every store
into a refcounted slot is emitted as load old, store, release old
(`selfhost/ir/own.hero:133-159`), so the old-value load reads a slot before its
first write, arms or none. No value is read or released before its one
definition: clang with every zeroing stripped (20,928 warnings, 0 on values),
the compiler-engineer's definite-assignment dataflow, the critic's analyser over
the C text (which counts a read through `&` as a read), and a compiler built
with `-ftrivial-auto-var-init=pattern` compiling itself to identical bytes. And
zeroing a value is the special case panel 021 R3 never licensed: the emitter's
own header reads *"The ONE exception (panel 021 R3): a refcounted slot"*, while
two lines of its body zero refcounted values too, carried from the Rust port
with no comment. **Zeroing them takes locals away from `-Werror=uninitialized`**,
the compile error that header says the prologue relies on; the critic counts
about 71,602 values returned to clang by not zeroing them (10,078 more are read
only through an address, where clang does not look, and are guarded by the
verifier alone).

**The critic's three findings that move the resolution.**
- **A value the IR shows as written whole is written in parts in the C.** The
  map lookup's found path (`selfhost/emit/container.hero:265-296`) writes
  `t3.tag = INT64_C(0);` and then copies the payload into `t3.as.ok`: the other
  union member and the padding keep what the prologue left. 132 such sites in
  the seed and 80 in the corpus. Nothing reads those bytes today (release, eq
  and hash branch on the tag first), and no instrument would see it if
  something did.
- **Route (f)'s failure is a leak, and the compiler never crosses the leak
  gate.** Every path out of the compiler's `main` ends in `exit(code:)`, which
  skips the gate by design (`runtime/hero_os.h:105`). A fault-injected (f)
  compiler, every whole-slot store called an initialisation, reached a
  byte-identical fixpoint at exit 0 with 1,334,909 live blocks at exit and 3.1
  GB of resident memory; `run` caught it on 20 of 210 goldens. And (f) takes
  3,914 slots out of clang's sight, where the trunk and (d) have clang behind
  the zeroing, for 2 to 5% over (d).
- **The unit cache key omits the C compiler and its flags** (filed as **defect
  122**; reproduced by the coordinator: a program printing `__clang_major__`
  built warm under Homebrew clang 22.1.8 printed 21, and back under Apple clang
  21 printed 22).

**The time**, the compiler-engineer's, user seconds, interleaved, load 2.7 to
4.3, ratio 1.00 to 1.05, the same inputs for every compiler:

| compiler | `fmt` 36,688 lines, `-O0` seed | `-O2` seed | `check selfhost/main.hero` | `--emit-c` |
|---|---|---|---|---|
| trunk | 3.11 | 0.72 | 4.26 | 35.5 |
| (a-min) | 2.43 (0.78) | 0.67 (0.93) | 3.32 (0.78) | 32.5 (0.92) |
| (d) = (a-min) + (b) | 2.23 (0.72) | 0.63 (0.87) | 3.02 (0.71) | 29.6 (0.83) |
| (f) = (d) + initialising stores | 2.17 (0.70) | 0.60 (0.83) | 2.95 (0.69) | 28.0 (0.79) |

At `-O2` the zeroing is inlined, not removed: the trunk's `h_keywords_keyword`
has 0 `memset` calls and 243 zero stores before its first call, (a-min)'s 66
(the critic, disassembled; both seats were right about what each counted). The
ffi-pragmatist's microbenchmark of the one function: today 414 ns, (a) 200, (d)
86 at `-O0`.

**Nothing crosses the C boundary.** Across 119 FFI goldens, 436 extern call
sites, 420 local arguments and 25 addresses handed to C, no zeroed local reaches
C; the `owned` out-cell C may leave unwritten is nulled by an IR store; no route
touches `runtime/heroes_runtime.h` (ffi-pragmatist). The ffi-pragmatist's one
unattributed hang did not reproduce in 400 runs (critic). The same golden's exit
status varies, 133 in 77 runs of 100 and 134 in 23 (the coordinator, the
trunk's build): Darwin's allocator's choice of signal, already on record in
`runtime/parts/os.c:137-152` with panel 173's 152 of 200, and not a defect.

## Disagreements, unsmoothed

1. **(f) against (a-min).** The compiler-engineer approves (f) as the
   architecture step: 0.70 against 0.78 at `-O0`, its new analysis failing only
   as a leak. The critic measured what that leak is invisible to: the compiler
   itself, the fixpoint, `emission`, `determinism`, `ir`, `emit` and
   `canonical`, and 190 of 210 `run` goldens under the maximal fault; and it
   costs clang's sight of 3,914 slots.
2. **(b)'s failure mode is argued, not measured.** The critic's fault
   injection for (b), a temporary used twice called consumed, changed 0 of 264
   programs and the compiler's own emission, so the instrument that would catch
   a double release there was never shown to fire.
3. **The 273 slots touched only through an address** (ffi-pragmatist) are 0 in
   the trunk's seed and 3,914 in (f)'s (critic): true of (f)-shaped emission,
   not of today's.

## The resolution: `provisional, author ratification pending`

**The most robust route priced is (a-min), and it is also the one that adds
robustness**; the faster routes buy 6 to 9 points more by removing checks the
compiler cannot yet do without, so they wait for the instruments that would
make them safe. Where robustness is at stake the author's instruction of
2026-09-27 does not ask for the fastest.

1. **A value is never zeroed; an `@` parameter's slot is never zeroed; every
   other refcounted slot is.** `emit/body.hero` prints `= {0}` on a refcounted
   slot unless it is a parameter, and never on a value, and its header says
   why each class is what it is: rule 3's old-value load reads every slot
   before its first store, and a value's one definition precedes every read on
   every path, which the verifier proves.
2. **The verifier proves it within a block too**: `ir/values.hero`'s `in_order`,
   the check it lacked, refusing a read above its definition in the same block,
   with a test that makes it fire.
3. **Every definition is written whole in the C.** At the 132 sites where the
   emitter writes a tag and then copies a payload into an aggregate, the
   aggregate is written whole first (`t = (T){.tag = ...};` then the copy), so
   no byte of a value is left as the prologue found it; an instrument counts
   partial writes over the seed and the corpus and pins zero.
4. **Defect 122 closes in the same lane**: the unit and runtime cache keys
   (`selfhost/cli/units.hero:82-84`, `selfhost/cli/toolchain.hero:154`) take the
   C compiler's identity (`clang --version`, which `clang_floor.hero` already
   writes on every build) and the flag list, so a changed compiler or flag
   rebuilds; a golden or unit test builds warm under one compiler and flag set
   and shows the object rebuilt under the other.
5. **`unread.slots_read` counts the exit sweep as a read**
   (`selfhost/emit/unread.hero:268`), as `ir/uses.hero:130-135` and
   `emit/body.hero:168-169` already say it is; latent under (a-min), a false
   `__attribute__((unused))` on 5,468 slots under any route that removes the
   old-value load.
6. **The records.** design.md Part 5's rule 5 (`docs/design/design.md:2614-2620`,
   *"decrefed at the end of its defining block"*) states the draft
   `own.hero:29-31` records as refuted within the hour; it is amended to rule 5
   as `own.hero` states it, with a dated note. `emit/body.hero:5`'s *"goto may
   not jump over an initialisation"* is C++'s rule and not C's (the critic
   compiled the counterexample under the project's flags), so the header says
   what is true: every local is declared at the top as one place to read them,
   not because C forbids the jump.
7. **The gate**: the compiler's tests and the net's own; `run`, `emission`
   blessed by its own procedure and re-run, `determinism`, `ir`, `emit` (by hand,
   diff read), `canonical`, `warnings`, `lines`, `corpus`, `layout`, `order`,
   `records`; the seed regenerated and the fixpoint by `cmp`; the three
   uninitialised warnings on the seed at 0; Linux arm64 and the Windows box
   before the merge; and the timing of `fmt`, `check` and `--emit-c` before and
   after on a still machine.

**Deferred, not refused, and the condition that brings each back**, carried in
this sitting's `docs/work/DECIDE.md` item for the author:
- **(b), the consuming store, and (f)'s initialising stores**: back when the
  compiler itself crosses a leak gate (its runs report live blocks at exit, the
  critic's differential probe or a runtime mode that makes `hero_exit` check),
  and (b)'s fault injection is shown to fire. Their gain over (a-min) is 6 to 9
  points of user time; their failure modes, a double release and a leak, have no
  instrument that sees them in the one program with 12,897 synthetic slots.
- **MemorySanitizer on the Linux leg**, measured working by the critic, as an
  instrument leg once defect 122's repair lets it build its own objects.
- **Coalescing the synthetic slots of mutually exclusive arms** (the critic's
  unlisted route), whose premise is unrun.

**What conservative would have been** (CL-040): (a-min) alone, without the whole
definitions, the verifier's order check or defect 122. Not taken: the partial
writes are the one path on which (a-min)'s unzeroed values could hold garbage no
instrument sees, and the order check is what makes *the verifier proves it*
true.

**Refused.** (c), an arm's slot released at its join: the compiler-engineer's
veto (it needs the liveness pass panel 021 R3 refused and a verifier that shares
the pass's answer) and the ffi-pragmatist's measurement (a `match` in a loop,
each arm's value lent to C, a heap use after free on the fourth iteration with 0
clang diagnostics). (e), slot zeroing decided by analysis: a wrong answer is an
uninitialised release through `&` that clang, ASan and the verifier do not see,
and it buys nothing measurable over (f) (§1.12).

**What the conditions compel.** The compiler-engineer's objection to the
defect's framing is the resolution; his conditions on (a-min) (a program clang
refuses, a crash under the pattern, the order check firing) are the gate's. The
ffi-pragmatist's objection to (a) on slots is honoured: no slot zeroing is
removed but the `@` parameters', whose copy-in the prologue writes before
`goto bb0`. Its veto condition is checked by its own `work/boundary.py` on the
landed emission.

## Predictions to score

| seat | prediction | checkable at |
|---|---|---|
| compiler-engineer | with (a-min), (d) or (f) landed: `grep -c '= {0};' seed/heroes.c` at most 22,000; clang's `-Wuninitialized -Wsometimes-uninitialized -Wconditional-uninitialized` on the seed 0; `fmt` on sixteen copies of `walk.hero` at most 0.85 of the `0fc98107` seed's user time, interleaved | the landing |
| ffi-pragmatist | with (a), (b) or (d) landed: no declaration of `heroes_runtime.h` changes and the ABI reads 26; no `.expected` of the 119 FFI `run` goldens and nothing under `examples/sqlite/` changes; `run` green on all 119 | the landing |

## Author's verdict

*Pending: `docs/work/DECIDE.md` carries this sitting as `panel 182`. Work
proceeds on the provisional resolution: one lane lands items 1 to 7 and closes
defects 114 and 122.*

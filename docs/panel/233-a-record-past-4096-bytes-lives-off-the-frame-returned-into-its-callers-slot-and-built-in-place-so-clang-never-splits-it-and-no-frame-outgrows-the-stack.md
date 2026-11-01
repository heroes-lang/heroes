# Panel 233 — a record past 4,096 bytes kept off the frame, returned into its caller's slot and built in place, so clang never splits it whole and no frame outgrows the stack

**Lane: soundness**, the compiler-engineer and the ffi-pragmatist, with a completeness critic before
and after the seats. Convened 2026-11-01 by the author's answer to decision 2283, *(A)*
(https://github.com/heroes-lang/heroes/issues/2283), the milestone M-latency-ceiling's one panel gate,
on defect 2278 (https://github.com/heroes-lang/heroes/issues/2278), which with defects 2275 and 2277
held the tag `m-latency-ceiling`; those two were repaired the same day (`b80265fe6`). The tree was
frozen at `0ee14c9be` from the briefs to this synthesis. Briefs: `233-briefs/`; reports:
`233-reports/`; evidence: `233-reports/evidence.tar.xz` and `evidence.md`.

**Status: provisional — author ratification pending**, asked by decision 2285
(https://github.com/heroes-lang/heroes/issues/2285).

## The proposal, as the seats received it

A correct program whose records hold records by copy, some megabytes deep, makes clang at `-O2` spend
minutes, and past ten minutes at 128 MiB, so `heroes run` (which builds at `-O2`) does not start
(defect 2278, four shapes, none committed by its filer). The questions (`233-briefs/00-shared.md`):
whether that cost is compiler-need (CLAUDE.md § 13, CL-006) or a refusal to name; the most robust
route that lets such a program run at `-O2`, every other program's C unchanged where possible; and
the threshold's unit. Routes named before the seats: R1 opaque copies keyed on size, R2 `optnone`, R3
heap boxes copied on write, R4 a refusal, R5 a flag, and the critic's R6 `noinline`, R7 construction in
place and return through the caller's slot, a bigger stack, a byte array in C. R3 and R4 were outside
this lane (spec § 3, a new diagnostic).

## What was measured

Every figure is clang's instructions retired and peak memory on the program's unit at `-O2`, with the
build's own flags, `/usr/bin/time -l clang -fintegrated-cc1`, each compile alone, read by the seat named
(its report gives the command); a reading reproduces to about 1 to 3 %.

| program | today, frozen compiler | the route as adopted (`sources 2141412850b671c8`) |
|---|---|---|
| depth 16, 1 MiB (compiler-engineer) | 66.18 billion, 2.78 GB; at `-O0` it aborts `stack exhausted` | 0.380 billion, about 11 MB; runs at `-O2` and `-O0` |
| depth 18, 4 MiB | 314.40 billion, 12.31 GB; at `-O0` it aborts | 0.398 billion, about 12 MB; runs at both |
| the filing's three other shapes at 2 MiB (E6) | one function 40.65 billion, 1.21 GB; by value past the 600 s bound; eight calls a level 1,908.19 billion, 10.41 GB | 3.67, 3.66 and 4.99 billion, 63 to 67 MB |
| the same at 128 MiB (E8) | not run (memory) | at most 5.01 billion and 67 MB; every one runs at both levels |
| a 16 MiB record through a function value (critic, `viaval7`) | aborts at both levels | 4.31 billion, 65 MB; runs at both |
| a program record holding defect 2253's 128 MiB `partial` record (critic, `grpwrap`) | counted 16 bytes, on the frame, aborts at `-O0` | runs at both, `-O0` frames of 112 and 80 bytes |

The cost was where the coordinator's time report put it: `SROAPass` 96.3 % of the depth-18 unit at `-O2`.
Clang splits every struct a frame holds into one value per scalar field, and every copy into one store
per field; inlining or one large body hands it millions. Today's `-O2` runs print the right answer
only because clang folds the whole record into the one field the program reads, which is also why
`-O0`, which keeps those frames, aborts (the critic's first pass). Measured beside the route on depth 18
(compiler-engineer § 6): R6, `noinline` on every builder, 1.88 billion and then `stack exhausted in
bigframe18.make18` at `-O2`; R2, `optnone`, 2.02 billion and then an abort in `main`; the room without
the other parts, 0.56 billion and an abort in `make14`. The ffi-pragmatist measured the cost to follow
bytes, not the fields a record names: a chain of array members costs as much as named ones or more
(1 MiB: named 65.7 billion, array 446.5 billion), and an escape of the result multiplies it (442.8 billion
and 6.96 GB against 65.7 and 2.78). Run cost of the route, which builds what today's `-O2` folds away:
about 3,700 instructions per construction of an 8 KiB record and 27,000 of a 32 KiB one (median of five);
depth 16 run 17.85 to 24.44 million (compiler-engineer § 9).

## The verdict table

| seat | verdict | section | cost / delta | prediction | condition |
|---|---|---|---|---|---|
| compiler-engineer | approve the route of large values off the frame (§§ 5 and 9 of its report), with the critic's five repairs built; object to R6 alone, R2 and the room without the other parts, each measured to turn today's correct `-O2` run of depth 18 into `stack exhausted`; object to R4, since the programs can compile and run; compiler-need under CLAUDE.md § 13, not Principle 0 | design.md §1.12; §1.7 and Part 5 (no construct added) | `selfhost/emit/large_value.hero` 385 lines, `runtime/parts/frame.c` 156, 19 files +276/−41; `emit/ctype.hero`'s decided ceiling 400 to 404; the whole patch 21 files, +817/−41 (`route-233.patch` in the evidence) | the four shapes at 128 MiB and depth 18, committed as `run` cases, each compile at `-O2` under 8 billion instructions and 100 MB on Apple clang 21, print `before`, `1` at both levels on all four CI legs, and the emission re-bless moves each blessed file only at its key line | wrong if a needed program gets a value past the line on a hot path and pays past the latency ledger's floor of effect; if a CI leg's clang prices the C otherwise; if the full panel reads spec § 3's *by value* as fixing where a local's bytes live; if a shape below the line still grows with its record (4,096 one-byte fields, the known gap) |
| ffi-pragmatist | object to the routes as briefed: veto (a) R7 written so a thunk or a function value takes the out-parameter convention, or without a per-thunk compile-time assertion; veto (b) R4 if its threshold counts a group record; approve R7 in its R7b shape with the assertion, paired with R6, the threshold in bytes from `sizeof`, applied to group records too | design.md §1.11, §1.12, §4.19 | every extern's ABI unchanged under R1, R2, R5, R6 (compiled and run against C at both levels); R7a refuses correct programs, R7c exits 134 silently, R7b keeps the ABI | an R7 lane following today's thunk rule builds `ffi/bind/bigabi.hero` with clang silent and exits 134 at `big_apply_ret`; with the assertion the build is refused; in R7b's shape `abi.sh` prints today's lines and raylib's unit is unchanged by any threshold of 4 KiB or more | veto (a) lifts if one C signature serves a function, its value and its thunk, or each thunk is derived from the type the probe checks; the approval becomes a veto if `abi.sh` shows an extern moving on x86-64 or Windows (unrun); veto (b) lifts if R4 exempts group records |

## Predictions to score

| seat | prediction | scored |
|---|---|---|
| compiler-engineer | the four shapes at 128 MiB and depth 18 as `run` cases: each unit under 8 billion instructions and 100 MB at `-O2` (Apple clang 21), `before`, `1` at both levels on the four CI legs; `viaval7` and `grpwrap` print their answers at both levels on the four legs; the counted record under 8 billion and 120 MB; the `emission` re-bless moving the key lines plus exactly the 26 programs repair 2 changes | at the repair's landing and the CI legs after its push |
| ffi-pragmatist | today's thunk rule plus R7: `bigabi.hero` builds silent and exits 134; with the assertion, refused; R7b: `abi.sh` unchanged, raylib's unit unchanged at a 4 KiB threshold | at the repair's landing |

## Disagreements, stated plainly

**Group records.** The ffi-pragmatist approved R7 only with the threshold in bytes from `sizeof`,
applied to group records too; the compiler-engineer's first route left them out. The critic measured
both sides: `sizeof` makes the emitted C depend on the target (`struct { long a[600]; }` is past 4,096
bytes on Darwin and both Linux targets and under it on Windows), and it cannot remove the frame copy C's
ABI makes for a large struct passed by value to a C function (16,777,248 bytes of frame for a 16 MiB
struct); leaving them out kept a program record holding a 128 MiB `partial` record on the frame. The
resolution takes the critic's target-independent rule, built and measured by the compiler-engineer: a
program type holding a `partial` group record by value counts as large, and a group record that is or
holds one takes its storage off the frame while its C convention stays C's.

**The ffi-pragmatist's approval had three conditions, and the resolution meets none of them as written**,
said so the author can weigh it: (i) R6 beside R7, waived, since the route's compile cost is already
low without it and R6 alone was measured to abort; (ii) a per-thunk `_Static_assert`, met by exclusion
instead: the checker refuses a program record inside a callback type, large or small, so no function
the route converts can be thunked, and that rule is now held by a test in `large_value.hero`, while a
function value's wrapper signature mismatch is refused by clang (the critic's `valsplit-route-mismatch.c`);
(iii) group records counted by `sizeof`, replaced by the rule above. Its veto (a) is not triggered:
one C signature serves a converted function and its value, and none of its thunks exist. Its veto (b)
concerns R4, which is not adopted.

**Repair 2's price.** Counting `partial`-holding types as large changes 26 blessed emissions beyond
their key line (programs over `passwd`, `statvfs`, `regex_t`, SHA-256's state, libuv, zlib and `ctime`,
mostly small structs whose size the rule cannot know), moves one `lldb` session
(`formatter-steps-aside`, a binding now off the frame), and makes every wrapper of a `partial` header
struct pay the room's traffic at run time. The compiler-engineer recommends paying it, since keeping a
correct program running ranks above the debugger's view; the conservative alternative below does not.

## The resolution adopted, provisionally

**R1. Large values live off the frame.** A type is large when its named fields, summed with no padding
(a variant's widest case plus its tag), pass **4,096 bytes**, or when it holds a header's `partial`
record by value; counted once per build over the checker's containment order, never by recursion down a
chain. A large local or temporary takes its room from the runtime's per-thread last-in-first-out room
(`runtime/parts/frame.c`), its name defined to that room for the body.

**R2. A large result is written into its caller's slot** (`void f(T *hero_out, ...)`), **a large
plain parameter is passed by `const T *`** (plain parameters are borrowed, never written, `ir/own.hero`
rule 1), and **a construction of a large record is written field by field into its destination**.

**R3. Function types that name a large program record take the same convention**, with no wrapper; C
never sees such a type, because the checker refuses a program record inside a callback type, a rule now
held by a test. Every `extern` call and every callback keeps C's ABI, byte for byte (the ffi-pragmatist,
compiled and run against C at both levels).

**R4. A group record that is or holds a `partial` record takes its storage off the frame**, its C
convention unchanged and its zeroing written by `memset` alone.

**R5. The room is bounded at 1 GiB for the whole process**, counted atomically, and running out of it
aborts with the guard's own opening words, then the truth: `panic: stack exhausted in
<module.function>: the room large values take off the stack is full, 1 GiB for the whole program`, told
by one thread only. Spec § 9's *running out of stack aborts* is kept.

**R6. Programs with no large value get byte-identical C.** No program in the tree passes the line; the
largest record any emission-corpus program declares is about 98 bytes.

**What the landing owes**, before the tag `m-latency-ceiling`: the route as the evidence's
`route-233.patch` holds it, rebuilt on the trunk; `run` cases for depth 18, the filing's three other
shapes, `viaval7`, `grpwrap` and the counted record (sized so the suite stays within its bounds, the
128 MiB readings kept as the sitting's); the `emission` re-bless, every key line and the 26 programs
read one by one; the `lldb` session re-pinned; the full net, gate A's fixpoint and the census; the CI's
four legs after the push, Linux's `-fstack-clash-protection` and the Windows target's
`__attribute__((cleanup))` among them, both unrun here.

**What it does not close**, filed apart: chains of group records nested by copy (the ffi-pragmatist's
group chain at 1 MiB, 446.9 billion instructions, unchanged by the route) and a Heroes function
returning a `partial` group record by value (`grpret`, which still aborts at both levels, as spec § 9
says a frame larger than the stack does), since a function type naming a group record may be a callback
and keeps C's convention.

## What the conservative resolution would have been, so the author can choose it

The route without repair 2: a type counted by its named fields alone, no `partial` rule and no group
record off the frame. It changes no blessed emission beyond its key line and no `lldb` session, and
leaves `grpwrap`'s abort at `-O0` standing, named. Every other part of the resolution stands with it.

## Prior art

The soundness lane sat no historian, so no precedent was sourced, and the sitting's decision issue
carries no `## Prior art` section (`.claude/rules/records.md` § And the record says whose idea it was).

## What the seats could not run

Linux's and Windows' clangs on the route (the CI's legs after the repair's push; the arm64 image has
neither `/usr/bin/time` nor `perf`, the critic's first pass); the x86-64 and Windows calling
conventions of the R7b shape; the Windows MSVC target's handling of the route's
`__attribute__((cleanup))`; a record of 4,096 one-byte fields under the line.

## Found beside the sitting

- **Defect 2284**, filed `adjacent` by the coordinator from the ffi-pragmatist's § 3c: a group record
  bound `partial` whose header names many members at depth keeps `heroes build` past ten minutes, in
  the compiler's own lock screen (`selfhost/cli/locks.hero`), not in clang
  (https://github.com/heroes-lang/heroes/issues/2284).

## Process, recorded against this sitting

- The coordinator's reproducer reproduced one of the defect's four shapes and not its filed reading
  (it finished, about 300 billion instructions); the seats wrote and measured the other three.
- The coordinator's first hypothesis, that defect 2253's `__optnone__` `main` made the cost, was
  measured false before the briefs (the batch's base compiler costs the same), and is in the brief as
  false.
- The instrument: `time -l` counts the clang driver alone unless `-fintegrated-cc1` (89 million
  against 63 billion instructions at depth 16), caught by the coordinator before the briefs; the
  build's own flags (`-gline-tables-only`) and the run at `-O0` were the critic's catches.
- Both seats were stopped twice by the account's five-hour limit and resumed from their own reports
  by message; the compiler-engineer's measurement of one shape was left running as an orphan and
  stopped by the coordinator, and taken again.
- The critic kept helper files in the session scratchpad, outside the repository, for part of its
  first pass, moved them into its worktree and recorded it.
- The critic's second pass rebuilt the compiler-engineer's route from its files (`sources
  588572956d7a94d0`) and found five things it had to answer; the seat built all five before this
  synthesis (§ 9 of its report, `sources 2141412850b671c8`), as the panel skill's *a route that does not
  build is not adopted* asks.
- The evidence's first packing kept 36 files of 2,237: `file` pads its columns, and the coordinator's
  filter read the padded type as no type; repaired, counted per folder, read back, no key found.

## Author's verdict

Pending, asked by decision 2285 (https://github.com/heroes-lang/heroes/issues/2285): ratify R1 to R6, and inside it
repair 2 or the conservative resolution. A yes settles defect 2278's route; it does not settle the
shapes filed apart, chains of group records and a `partial` record returned by value.

**RATIFIED, 2026-11-01**, R1 to R6 with repair 2, as the landing extended them. The author
answered `Approve` in conversation that evening, on the coordinator's report of the tag, which
put the two points the round's gate found and the sitting had not: R2 widened past a record to a
variant case and to a `T?` (`case_in_place` and `option_in_place`), and every construction of a
large value opening by writing the whole value zero, which `wholes` had refused ten of the
route's own emissions for lacking. So **the question to the author is answered: R1 to R6 stand,
with repair 2**, which keeps the `partial` rule and a group record off the frame; the
conservative resolution of § What the conservative resolution would have been is not taken, and
`grpwrap`'s abort at `-O0` does not stand. Recorded as a reading (CLAUDE.md § 4). The author may
overturn it, and the shapes filed apart, defects 2284, 2286, 2287 and 2288, are not settled by
it.

## Scored at the landing, 2026-11-01

The route landed on the trunk the day after the sitting. Two of the sitting's
own resolutions were found short by the round's gate, by instruments the
sitting did not run, and both are written here rather than only in the commit
because the second changes what R1 and R2 mean together.

**R2 was narrower than R1, and a correct program died of it.** R1 counts a
variant among the large types, *the widest case plus its tag*, and puts every
large local and temporary in the room; R2 wrote in place only *a construction
of a large **record***. So a variant's temporary lived in the room while the
compound literal that filled it was built on the frame, which put the whole
value back exactly where R1 had moved it from, and the same held of a `T?`,
large exactly when its payload is. Measured on this Mac with a 2 MiB payload,
`clang -O2 -Wframe-larger-than=65536`: `main`'s frame **4,194,624 bytes**
before, **no frame over 64 KiB** after. And one recursion run twice at the same
depth over the same bytes, differing only in whether the value sits in a case:
**`15`, exit 0 as a record; `panic: stack exhausted`, exit 134 as a case.** The
landing added `case_in_place` and `option_in_place` beside `in_place`
(`emit/large_value.hero`), which is R1 reaching the shapes R1 itself names, and
the two cases
`run/fixedbugs-2278-a-two-mib-variant-case-recurses-without-exhausting-the-stack`
and `run/fixedbugs-2278-a-large-option-holds-its-payload-off-the-frame` hold
it. **What the sitting would have had to run to see this**: not a reading of
the route but `-Wframe-larger-than` on each of the shapes R1 names, which no
seat was asked for, the predictions table having been written about records.

**A construction in place wrote no value whole, which panel 182's rule
refuses.** `wholes` read ten of the route's own emissions and refused them:
field-by-field stores with nothing writing the value whole, so the bytes no
field names kept what the room last held, and the room is given back
last-taken-first, so *what it last held* is another value's bytes, not merely
indeterminate ones. The store that follows a construction copies `sizeof`
bytes and carries them into the caller, and panel 215's R1 promises those
bytes are zero where the record is lent to C. Every construction now opens
with `t = *(T *)__builtin_memset(&t, 0, sizeof t);`, the form `emit/zeroed.hero`
already uses at its own in-room arm and for its reason: a `(T){0}` of a 128 MiB
record would be a 128 MiB object of the frame.

**And two rules of the runtime's own**, which `suite_runtime` told at the same
gate: `parts/frame.c` called `free` where design.md Part 7.13 puts every such
call in `parts/alloc.c` (`hero_free_raw` now joins `hero_malloc_raw` there, the
pair neither leak counter weighs), and `hero_frame_told` is spelled `_Atomic
int`.

**What held exactly as adopted**: R3 (a function type naming a large program
record takes the direct call's convention, no wrapper, and the checker's
refusal keeps such a type away from C), R4, R5 and R6: every program with no
large value emitted byte-identical C beyond its provenance key, which the
Linux arm64 bless confirmed on the two cases this Mac cannot judge.

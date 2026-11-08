# Panel 236: a large group record is written into its caller's slot by a call by name, while a function value keeps C's convention through a twin, each thunk asserted

**provisional — author ratification pending.** Soundness lane, convened 2026-11-08 on defect 2286
(https://github.com/heroes-lang/heroes/issues/2286), classed `systemic` by the author's decision of
2026-11-07 (route A, recorded in the defect's Class line), which named the proposal and sent it to
this sitting after panel 235. Seats: `compiler-engineer` and `ffi-pragmatist`; the completeness
critic read the briefs before the seats (`236-reports/critic-briefs.md`, on which the briefs were
repaired) and the reports after them (`236-reports/critic-reports.md`). Briefs in `236-briefs/`,
reports in `236-reports/`, evidence in `236-reports/evidence.tar.xz`. Base: `main` at `5bd4cf3e4`,
compiler `sources 2fa3d81374af3684`.

## The proposal, as the seats received it

A record an `extern` group binds from a C header (a group record) that the emitted C keeps off the
frame, today a `partial` one or one holding a `partial` one by value (panel 233's R4), and any other
the sitting rules large, takes panel 233's `hero_out` convention **between Heroes functions**: a
Heroes function returning one writes it into its caller's slot, a plain parameter of one is handed
as a pointer, and an indirect call through a function value does the same. **Where such a function
is handed to C as a callback, the emitter generates an adapter that keeps C's convention** and calls
the Heroes function through `hero_out`, so C's view of every function and every `extern` is byte for
byte what it is today. Defect 2286's two parts are the sitting's measure: a chain of group records
nested by copy costs clang at `-O2` 12.4 and 10.1 billion instructions at 64 KiB, and a Heroes
function returning defect 2253's 128 MiB `partial` record aborts at both levels (`grpnoext`;
`grpret` aborts at its `extern` call whatever the route, as defect 2253's cases expect).

## What was measured

On the base, by the coordinator and the critic (`236-briefs/00-shared.md`): `grpnoext`, `grpgen`
and `grpret` abort at both levels, `go`'s frame 134,217,792 bytes and more; `grpopt`, already
through `hero_out`, aborts on the room's 1 GiB, its prologue taking 9 regions of at least 128 MiB;
`grpwrap` runs. The chains at 64 KiB, clang `-O2` on the unit alone: `gn12` 12,427,643,548
instructions and `ga12` 10,115,706,929.

On the routes, each built in a seat's worktree and rebuilt by the critic from the compiler-engineer's
patches (keys `1699bb340bef9aeb` for R7b with rule B, `17a3862c60d14c24` for the proposal as
written):

| shape | base | R7b, rule B | the proposal as written |
|---|---|---|---|
| `grpnoext`, `grpgen`, `grpfix2` (a 128 MiB result read in Heroes) | abort, both levels | run, both levels | run |
| `grpdisc`, `grpbyval`, defect 2334's `big8k` | abort at `-O0` | run, both levels | run |
| `grpval` / `xvalret` (a 128 MiB result through a function value, the same shape) | abort, both levels | abort, both levels | run |
| `grpvalparam` (a 128 MiB parameter through a function value) | abort at `-O0` | abort at `-O0` | run |
| `threemod` (an `extern` and a function in one function type, three modules) | runs | runs | **refused at build**, exit 1, its message blaming the program falsely |
| `ga16`, the 1 MiB array chain, clang `-O2` | 460,606,411,427 instructions, 7.0 GB | 831,827,701, 16.4 MB | |
| `gn16`, the 1 MiB named chain, clang `-O2` | not run (about 17 GB by `gn14`'s growth) | 51,025,955,600, 632 MB, 92.5 % in clang's front end | |
| C's view of the four callback probes | | byte for byte the base's | byte for byte only in the ffi seat's spelling |

Under R7b with rule B every witness the compiler-engineer names runs at both levels (the critic's
re-run, `critic-reports.md` § 2), five sampled under `--sanitize` clean; on the compiler-engineer's
final build `emission` 2,298, `determinism` 697, `wholes` and `descriptors` 754 each, `emit` 13,
`lldb` 17 and `layout` 6 passed, 0 failed, the compiler's own tests 1,800, the compiler's own build
+0.18 % instructions warm. **The run-time price no seat measured** (the critic, § 7): 1,000 calls of
defect 2334's `total` at `-O2` read 76,929,897 instructions on the base and 1,775,060,970 on the
route, about 23 times, peak 1.4 MB against 11.1 MB, the room hiding from clang the copies it deleted
on the frame.

## The verdict table

| seat | verdict | section | cost | prediction | condition |
|---|---|---|---|---|---|
| compiler-engineer | **object** to the clause *an indirect call through a function value does the same*; **adopt with conditions** the rest in R7b's shape with rule B | design.md §1.7, §1.1, §1.12 | +279 lines, 16 files, all `selfhost/emit/`; R-P +491 before repairs, its adapter linkage unbuilt | `emission` reads exactly 16 failed before the landing's re-bless; `ga16` under 1.0 billion instructions and 20 MB | a veto if the landing needs a checker rule, a diagnostic or spec text, or lands with `layout` red; R-P approved instead on one adapter program-wide with `threemod` printing `6`, `6`, and a program the project needs that calls a value returning a group record past the stack |
| ffi-pragmatist | **adopt with conditions**; **veto** on the proposal as written if its crossing is `hero_callback_of`'s untyped table, lifted by a typed crossing **or by R7b** | design.md §4.19, §1.11, §1.12 | P: two typedefs per converted type, an adapter per `extern` used as a value, a typed crossing per type | `ga16` under 1 billion and 20 MB; `gn16` past 40 billion, at least 85 % in `-fsyntax-only`, whatever the route; on Windows the R7c model right in both directions, so only the assertion can see a wrong thunk there | the adopt turns to a veto if a CI leg moves an `extern`'s lowered declaration or a thunk's |
| critic | no verdict | | | | |

## Predictions to score

| seat | prediction | checked at |
|---|---|---|
| compiler-engineer | the landing's compiler on this base's corpus reads `emission` exactly 16 failed before its re-bless, the 16 files of its report's § 9 | the landing's gate |
| compiler-engineer and ffi-pragmatist | clang `-O2` on `f2/ga16` under 1.0 billion instructions and 20 MB (`/usr/bin/time -l clang -fintegrated-cc1`, `flags()`'s words) | the landing's gate (the critic read 831,827,701 and 16.4 MB on the route) |
| ffi-pragmatist | `gn16` past 40 billion, at least 85 % of it in `-fsyntax-only`, whatever the route | the critic read 51.0 billion and 92.5 % on the route: holds as registered |
| ffi-pragmatist | on the Windows box, the R7c model right for both directions under Microsoft x64 at both levels | the landing's Windows leg (the box timed out from 03:20; unrun) |

## Disagreements, stated plainly

**The route for a function value.** The ffi-pragmatist adopts the proposal with one typed crossing per
converted function type over R7b, because only the proposal runs `xvalret`, a 128 MiB result called
through a value, which robustness's rank decides. The compiler-engineer objects to the proposal's
value clause and recommends R7b, because the proposal as built refused `threemod` and its
one-adapter-per-program linkage was unbuilt. **The critic settled what a command could** (§ 1): the
typed crossing does not close `threemod`; one external adapter for the whole program does, in four
units edited by hand (`6|6`, exit 0); no seat built that adapter in the compiler, and the ffi seat's
route exists as hand-written C of one-unit programs. So frame 5 decides: **a route that does not build
is not adopted.** R7b's price is written down in R3 below, and the route that would lift it is named
for a later sitting.

**Two claims the critic did not find true as built**: the ffi seat's *both routes remove the thunk's
copy of a by-value parameter* holds for its hand-edited C, not for the built R7b, whose thunk is still
262,208 bytes at `-O0`; and its per-thunk assertion names the typedef, which `library.c` does not
write, so only the compiler-engineer's form, naming the twin's type beside the thunk, compiles in
every unit.

## The resolution adopted, provisionally

**R1. The set, rule B.** A group record is kept off the frame and takes the route when it is or holds
a `partial` record by value (panel 233's R4, unchanged), **or when its named fields, counted as panel
233's R1 counts a program record, pass `LARGE`, 4,096 bytes**. A count by named fields is the same on
every target (the ffi seat, § 3, defect 2334's record 8,192 bytes by count and by `sizeof` on all four);
rules A and B move the same 16 programs of the emission corpus, and rule B alone reaches the chains,
defect 2334 and `cbnamed` (the compiler-engineer, § 9). The union's double count (the critic's first
pass) can only move a record off the frame sooner, which costs room traffic and never correctness;
recorded, not changed here.

**R2. A call by name.** A Heroes function returning such a record writes it through `hero_out`, a plain
parameter of one is a pointer, a discarded result is written nowhere; a group record is constructed in
place, as panel 233's route constructs a program record (`large_construct.hero`); and a fixed-array
field of large elements is written one element at a time (the ffi seat's condition 4: the array literal
put 65,712 bytes back on `make12`'s frame).

**R3. A function value keeps C's convention, through a twin (R7b).** A function value, its typedef and
its thunk keep the function's C signature, through a twin, a small C-convention wrapper calling the
function in place; C's view of every function type, probe and thunk stays byte for byte the base's.
**Its price, written down**: a call through a value pays the by-value convention, so `grpval` and
`xvalret` (a 128 MiB result through a value, both levels) and `grpvalparam` (a 128 MiB parameter
through a value, at `-O0`) still abort with spec § 9's true words. **The route that would lift it**,
unbuilt and named for a later sitting: one adapter per `extern` used as a value, defined once for the
whole program, with the ffi seat's typed crossing (the critic's hand-built `threemod` ran), or the
compiler-engineer's third route, `hero_out` for function types no `extern` names and C's convention
for the rest.

**R4. One assertion per converted thunk**, in the compiler-engineer's form, naming the twin's type beside
the thunk, so it compiles in every unit: a thunk whose signature parts from its value is refused at
compile time. Both seats measured it both ways: without it a wrong thunk passes clang in silence and the
program exits 134; with it the build refuses.

**R5. Panel 233's route repaired where this sitting found it short**, in the same landing: defect 2337 (a
function taking or returning a large program record, its address taken beside a callback, gets no thunk
and no table row) and defect 2338 (a lent place rooted in a parameter received in place spelled through
the parameter's pointer cast to the record's own type), both correct programs refused today with exit 2.

**R6. Defect 2334 closes whole, not only off the stack.** Rule B moves its temporaries into the room, so it
runs at both levels; the critic measured the price at `-O2`, about 23 times the base's instructions. **The
landing also reads a fixed-array field's element in place**, the cure the compiler-engineer names in the
lowering (`ir/in_place.hero` never reads a fixed-array field in place today), so neither the stack nor
the time pays for a copy nobody wrote; measured at both levels before it lands.

**R7. Part (a) closes for chains of arrays, not of named halves.** `ga16` falls from 460.6 billion
instructions and 7.0 GB to 0.83 billion and 16.4 MB. `gn16` reads 51.0 billion and 632 MB on the route,
92.5 % of it clang's front end reading panel 215's padding table: another cause than the copies this
sitting removes. **It is filed at the landing as a defect of its own, measured on the landed compiler**,
and defect 2286 closes at that landing. Its run time is a contradiction to settle then: the critic read
637,378,953,468 instructions for the base's `-O0` binary before it aborts in `chain.make14`, and the
coordinator's run of the same program, built by main's compiler, aborted there after 17,759,412.

**R8. No diagnostic, no refusal, no spec text.** An `extern` taking or returning such a record by value
keeps C's ABI and spec § 9's abort (`grpextonly`, defect 2253's own cases): a build-time message would fire
on 2253's cases (the compiler-engineer, § 8) and a refusal would read `sizeof` against a stack the build
does not know, a thread's, `ulimit -s` or Windows' 64 MiB link (the ffi seat, § 5).

**R9. What the landing owes**: the compiler-engineer's thirteen cases (`run/fixedbugs-2286-*`,
`run/fixedbugs-2334-*`, `emit/fixedbugs-2286-the-thunk-of-a-converted-function-is-asserted-against-its-value`),
a case for `grpthread` (a converted call on a spawned thread, which aborts on the base and runs on the
route) and the three R3 shapes pinned as the aborts they are; the `emission` re-bless, its moved files read
one by one; `--sanitize` at both levels on every shape; `wholes`, `determinism`, `descriptors`, `emit`,
`lldb` and `layout` green; panel 215's R1 held (the padding zero where C reads a record that crossed
through `hero_out` or a pointer, the compiler-engineer's `padhero` and `padwrap`); and on the CI's four
legs the four callback shapes over a C library that calls back, at `-O0`, `-O2` and `--sanitize`, the
padding probe with its control, and the assertion's refusal of a wrong thunk.

## What the conservative resolution would have been, so the author can choose it

R7b with **rule A**, today's set alone: it closes `grpnoext`, `grpgen` and `grpfix2` and leaves the
chains, defect 2334 and `cbnamed` where they are (the compiler-engineer, § 9), and with them R6's cure.

## What the lane gave up

No spec text, diagnostic or surface moves (R8), so no seat reads the specification as a model or prices
its tokens; the historian did not sit, so this sitting's decision carries no prior art.

## Found beside the sitting, each filed

- Defect 2337 (blocking): a function taking or returning a large program record, its address taken beside
  a callback, thunked for C, `heroes build` exit 2 (the compiler-engineer, § 2).
- Defect 2338 (blocking): a group record's field of a program record received in place, lent to C, its
  padding zeroed through the `const` pointer, `heroes build` exit 2 (both seats).
- Defect 2339 (adjacent): at `-O2` an `extern`'s large by-value result inlined into `main`, the program's
  earlier output lost and the panic naming `main` (the ffi seat, § 5).
- Defect 2334 (blocking), filed during the sitting from batch 24's lane b24-misc and passed to both seats.
- The named chain's cost in panel 215's padding table (the critic, § 3): filed at the landing, measured on
  the landed compiler, its run-time contradiction settled first (R7).

## What the seats could not run

The CI's Linux and Windows legs (the Windows box answered at 01:01 and timed out from 03:20); the seed's
fixpoint under the route; the latency `compare`, the machine never still beside batch 24's lanes; the base's
`gn16` compile (about 17 GB); the single program-wide adapter of R3's lifting route.

## Process, recorded against this sitting

The seats worked in worktrees under `.claude/worktrees/236-<seat>/` rather than the scratchpad the skill
names, as panels 234 and 235 did, because CLAUDE.md forbids a file outside the repository's root. The
critic's first pass repaired the briefs on five counts (panel 233's veto (a) missing, `grpret` the wrong
witness, the room's 1 GiB, part (a) outside today's set, the union's count) and removed two steerings.
Every seat and lane stopped on the 5-hour usage limit from 01:10 to 03:17 and was resumed from where it
stood. The ffi seat wrote one file to `/tmp` and removed it within the minute (its report's § 9); the
critic's tool wrote one background output under the system's temporary folder, left in place. No paid
run, no credential printed, no write to GitHub by a seat.

## Author's verdict

Pending, asked by decision 2340 (https://github.com/heroes-lang/heroes/issues/2340). A yes settles R1 to R9 as adopted and lets the landing close defects 2286, 2334, 2337 and 2338; it does not settle R3's lifting route, which a later sitting builds before it is adopted.

# Panel 206: a constant's written body that cannot be computed is a compile error, and arithmetic in a function still aborts where it runs

Convened 2026-10-10 by the coordinator on the author's instruction, *as soon
as you have CPU, put the last open ones in together*, their answer through
the question widget, *yes, 3.69 USD* for the blind seat, and their message
raising it, *spend up to 30 dollars* for the night's paid runs, all between
04:37 and 04:49 by the clocks read before and after; for defect 577, which
lane b18-infer filed beside its repair of 564. **A full panel** (what `check`
refuses): the compiler-engineer, the ffi-pragmatist (added on the critic's
first pass), the spec-warden, the historian, the blind seat in ten fresh
`claude -p` sessions outside the repository (2.3717 USD), and the
completeness critic before the seats and after them. The tree frozen at
**`e0aeb991`**, worktree `lane-panel-206`, batch 18's round. Briefs written
from 04:49; the critic's first pass from 04:50:49 to 05:00:33, read at 05:04,
its repairs applied before any seat started (`rep` reverses a ratified
sitting's witness; panel 203's R3 already ratified an abort for `m2`; the
class is older than the round; a refusal would refuse code that runs today;
two semantics, per step or final value; floats out of reach; defect 578
found and filed); seats from 05:04; the account's session limit stopped every
seat at about 05:0x, resumed at 09:16 on the author's new login; the
ffi-pragmatist's reply copied at 09:21, the historian's and the
spec-warden's at 09:22; the blind seat's sessions from 09:23:20 to 09:24:39;
the compiler-engineer's reply copied at 09:30; the critic's second pass from
09:30:50 to 09:43:08, copied at 09:44; this synthesis from 09:44, every time
read from `date`. Briefs in `206-briefs/`, reports in `206-reports/`.

## The verdict table

| seat | verdict | on what |
|---|---|---|
| compiler-engineer | **object** to a refusal over function bodies (Principle 0 unmet, the compiler's own source never reaches it); `rep`'s abort ratified as panel 054's computed-count path; no veto | `proto2` and `proto3` built in its copy (+1.0% and +0.93% on `check selfhost/main.hero`), the census of 3,299 files: `proto2` refuses the repository's own `false && 1 / 0 == 0` |
| ffi-pragmatist | **approve, with conditions**: a refusal over literals and the program's own constants; a group's constant out of `check`, a probe at build its only route; **veto** any route letting 564's position typing narrow a group's constant | `__int128` and C11 probes on this Mac and Linux arm64, 16 FFI cases; `PATH_MAX * 600000` exits 0 on this Mac and 134 on Linux |
| spec-warden | **approve, provisional**, the round's abort at 0 tokens (D0); **object** to every refusal draft as things stand; F7e the one sentence it would let land with a refusal | `heroes measure` on thirteen drafts, the base after 204 and 205 at 7426 / 7561 vendored, 291 spendable real in the frozen tree |
| historian (advisory) | **approve** a refusal defined by a class of syntax (Go, C#, Ada, C), stopping at a call or a variable; precedent does not decide `rep` | Go's spec and #20108, Rust's `arithmetic_overflow` and #117949, Swift's SIL folding and SR-5964, Zig, C#'s CS0220, Ada RM 4.9, C11 6.6p4, GCC and clang's warnings, Java, Kotlin, Nim |
| blind seat | today's spec: 2 of 2 predict the round's per-step abort on four programs and the refusal of `300 - 100`, and **2 of 2 expect a constant's overflowing body refused when the program is compiled**, where the round aborts at each read; F7e: 2 of 2 predict its refusals; writing: **0 of 6** write literal arithmetic that cannot fit when asked for every bit set | `llm-ergonomist-scoring.md` |
| critic, second pass | route 1, refusing only a constant's written body whose step aborts (read or not), built: 0 of 3,299 files move, +0.05% on `check`, the compiler's own tests pass, the emitted C unchanged; a constant's body already cannot contain a call (panel 039's `constant_body`), so the class is syntactic; it matches the blind readers on all six programs; panel 054 ruled the count's type, the `0 - 1` witness was its landing's side effect | `route1a` and `route1b` built from the frozen tree, 23 shapes of its own, the census, `cmp` of the emitted C |

## What the sitting measured

- **In a function body**, literal arithmetic that cannot fit aborts at the
  step that overflows, as spec § 7 says (*Overflow aborts at every width*):
  `y: u8 = 200 + 100`, `x: u8 = 255 + 1 - 1`, `x: u8 = 2 - 3 + 5`, all 134 on
  the round; the trunk refused them only because the operands took no
  context, refusing `repeat("-", 3 - 1)` and `y: u8 = 2 + 3` with them. A
  refusal over function bodies refuses code that never runs: `proto2` the
  repository's `false && 1 / 0 == 0`
  (`tests/golden/run/adversarial-short-circuit.hero`), `proto3` a function
  never called, a branch not taken, a `test` block and `if false && y == 200
  + 100` (the critic); sparing them needs reachability from `main`, which a
  `test` block is not. The existing literal rule already ignores
  reachability (`y: u8 = 300` in a function nothing calls is refused). 0 of
  6 blind readers wrote such arithmetic when asked for every bit set.
- **In a constant's written body**, the round emits a C function computing it
  at every read and aborting there (`h_const_BIG`, `__builtin_add_overflow`);
  2 of 2 blind readers of today's spec expect the program refused when it is
  compiled, reading § 4's *a written body computes over literals and other
  constants* as the compiler's computation. A constant's body can already
  hold no call (panel 039's `selfhost/resolve/constant_body.hero`: *may only
  read literals and other constants*), so a refusal of its steps is defined
  by syntax, the shape the historian's precedents keep. The critic's route 1
  (`route1b`): every operator step of a constant's written body computed at
  the width the checker recorded, a written constant read through its own
  walk, memoised and guarded against cycles, a group constant giving no
  value; overflow, division or remainder by zero and a shift count outside
  0..63 refused, read or not; 0 of 3,299 tracked files move, +0.05% on
  `check selfhost/main.hero`, 1,555 own tests, the emitted C byte-identical.
  As built it misses an array element (`[200 + 100]`) and an `if` branch, and
  refuses a step inside `while false` in a constant body.
- **`rep`**: panel 054 (ratified 2026-08-14, its lines 103 to 110) ruled *a
  negative count cannot be written ... a computed count ... aborts ... at the
  subtraction that went negative rather than inside `repeat`*; its landing
  commit `4cafbb05` wrote the witness `repeat("-", 0 - 1) #~ type_mismatch`,
  which held because the operands took no context. On the round `-1` is still
  refused `int_out_of_range`, `0 - 1` aborts at the subtraction (054's
  computed path, told `integer overflow`), a `u64` count is the only one
  accepted (R11a true).
- **A group's constant** (`INT64_MAX + 1`, `PATH_MAX * 600000`) has a value
  only clang knows, platform by platform; 564 never narrows one (`f03`,
  `f07`, `f13` refused `type_mismatch`); a build probe of `__int128`
  `_Static_assert`s refuses at the `.hero` line on the platform where the run
  aborts (the ffi-pragmatist, two units by hand, Windows unrun).

## Disagreements, stated plainly

- **Refuse or abort.** The historian approves a refusal defined by syntax; the
  compiler-engineer and the spec-warden object over function bodies on
  Principle 0 and on code that never runs. The critic's route 1 confines the
  refusal to a constant's body, where the class is already syntactic, no
  call can stand and the blind readers expect it: the resolution takes it
  there, and keeps the abort in function bodies.
- **A sentence.** The spec-warden objects to a refusal landing with no
  sentence (§ 12); the readers already expect route 1 at 0 tokens, 2 of 2.
  The resolution takes the critic's C4b in § 4, the most explicit true
  sentence, at +13/+13 vendored, since the spec's tokens yield to robustness
  and truth (the author's instruction of 2026-09-28).

## The resolution, ratified by the author (below)

The most robust and complete route at every question (CLAUDE.md § 4,
CL-040); what conservative would have been is below the list.

1. **R1, a constant's written body: a step that cannot be computed is a
   compile error, wherever it is written in the body and whether or not the
   constant is read.** The critic's `route1b` lands, completed: every
   operator step of the body, inside an array literal, both branches of an
   `if`, a `match`'s arms and a loop's body alike (a syntactic class, the
   `while false` case refused as `y: u8 = 300` is in dead code), computed at
   the width the checker recorded, a written constant read through its own
   walk, a group constant giving no value; overflow, division or remainder by
   zero and a shift count outside the width refused, `int_out_of_range` and
   `division_by_zero` with the step quoted and its value said. Spec § 4 gains
   C4b, *... over literals and other constants, and a step of it that aborts
   is a compile error.*, +13/+13 on the vendored tables, the real count by one
   `--refresh` at the landing on the author's yes. Measured at the landing
   on the census, the critic's shapes and the cost (+0.05% built). Defect 577
   closes with it: its constant half repaired, its function half ruled.
2. **R2, a function body: literal arithmetic aborts where it runs**, as § 7
   says; no refusal there (it would refuse code that never runs, and 0 of 6
   readers wrote the mistake). Panel 203's R3 stands.
3. **R3, `rep`: the round's abort is panel 054's computed-count path,
   ratified.** `tests/golden/check/repeat-count-is-unsigned.hero`'s header
   corrected by a dated line beneath (a negative *literal* does not compile;
   a count computed negative aborts at its subtraction). R11a not adopted
   (panel 120 refused built-in signatures as a class), the count's type
   recorded as R11a's measurement.
4. **R4, a group's constant in arithmetic** stays an abort here; the
   ffi-pragmatist's build probe is filed as an `improvement` for a sitting of
   its own as defect 579 (platform-dependent values, an emitter change,
   Windows unrun).
5. **R5, refused**: a refusal over function bodies (`proto2`, `proto3`);
   F2a to F7h (false on the routes or ambiguous); C4a (*each time it is
   read*, true today, false once R1 lands for every refusable body); a
   final-value semantics (it would make § 7 describe a step the program does
   not run).

**The conservative alternative, the author's to choose instead**: D0, no
refusal and no sentence, 577 closed on its measurement, the constant's abort
at each read kept and C4a's sentence (+6) adopted to say so.

## Process notes

- **The compiler-engineer created an empty file in `/tmp` at 09:17** by a
  stray command, removed it at once and said so; the coordinator found no such
  file at 09:30 (CLAUDE.md § Hard stops). The ffi-pragmatist ran a second
  container for about ten seconds against its brief's *one at a time*.
- The account's session limit stopped every seat at about 05:0x; all were
  resumed at 09:16.

## Predictions to score

| seat | prediction | checkable at |
|---|---|---|
| compiler-engineer | if any refusal of literal steps lands, its module is at most 170 code lines, `check selfhost/main.hero` at least +0.5%, the census moves only `repeat-count-is-unsigned` and `adversarial-short-circuit` (for R1's constant route the critic measured +0.05% and 0 files, so the prediction is scored against a function-body route, not taken) | not taken |
| ffi-pragmatist | under a build probe, `x: i32 = PATH_MAX * 600000` builds on macOS and is refused at its line on Linux arm64 | the probe's sitting |
| spec-warden | P1 (for F7e, not taken), re-read for C4b: its real delta at the landing's `--refresh` | the landing |
| critic | R1 completed moves 0 tracked files and costs under +0.1% on `check` | the landing |

## Author's verdict

**RATIFIED, 2026-10-10**, R1 to R5 as written above, the author answering
through the question widget between 09:44 and 10:22 by the clocks read
before the question and after the answer, choosing *Ratifica R1-R5* over
*Conservativo (D0 + C4a)* and *I want to read it first*, on the
coordinator's summary of each route; in the same widget *yes, one* to the
`--refresh` R1's sentence needs at the landing. Recorded as a reading
(CLAUDE.md § 4). The author may overturn it.

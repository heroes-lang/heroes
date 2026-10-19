# Panel 225: a value only the probe sees is told beside a refusal, read from the program's own unit preprocessed whole, then cut at marks no header can write

**Convened the evening of 2026-10-18, synthesised 2026-10-19**, on defect 2137
(https://github.com/heroes-lang/heroes/issues/2137), with defect 2136
(https://github.com/heroes-lang/heroes/issues/2136) beside it, by the author's answer that evening,
meant as *yes, now*, with `m-witnessed-seed`'s tag waiting on both: they were filed at 05:01 UTC on
2026-10-18 and `m-readonly-lend` was placed at 05:07:29 UTC, so the next tag is their second and
`records/tagged` counts each `blocking` there. **Lane: soundness** (compiler-engineer,
ffi-pragmatist, a completeness critic before the seats and after them). **Resolution: provisional,
author ratification pending.** Briefs: `docs/panel/225-briefs/`, repaired in five places after the
critic's first pass (`docs/panel/225-reports/critic-briefs.md`), and a third step's brief to the
compiler-engineer (`compiler-engineer-union.md`) for the route this file adopts, built before it was
written (`/panel` § 3c). Reports: `docs/panel/225-reports/compiler-engineer.md`,
`ffi-pragmatist.md`, `critic.md`, `compiler-engineer-union.md`, and the coordinator's measurements,
`coordinator.md`. Every number below is a seat's, the critic's or the coordinator's, with its
command in the report that holds it.

## The proposal, verbatim (the shared brief's question)

> How is a value only the probe sees told in the same build as a refusal of its own module's unit
> at a line no constant's accessor holds?

Beside it, defect 2136, the same class one step earlier: a refusal made before any unit is written
(`probing.check_all`'s pointee check). Routes A to D were the coordinator's (A, the refused unit
asked as it is under the probe's words; B, the refused binding written inert; C, a unit of the
constants alone; D, left as it is); E (every refused unit) and F (the probe asked before a refusal
made before any unit) the critic's first pass; C-prime the compiler-engineer's and P the
ffi-pragmatist's, one route found twice (the program's own unit, marked, preprocessed whole, cut
after); G (a mark no header can write), H (the cut confined to the copy), I (the copy's fidelity
checked token for token) and the union the critic's second pass.

## What was measured

- **The defect, in both seats' copies** (each its own stage 2 of `7b97b9438`): `d2137.hero`, a
  parameter wider than the header's `int` and `K_DIV` (`INT_MIN / -1`) in one module, tells
  `narrow` alone at exit 1, and `K_DIV` on the next build.
- **Route A reaches only a warning made an error.** Asked as it is under the probe's `-w`, the
  refused unit compiles for 5 of the 10 refusal classes the ffi-pragmatist ran (§ 3), the five
  being decided by which warnings clang makes errors by default; the critic's U1, a union built
  from two members, is
  refused by a `_Static_assert` that stays an error under `-w`; and where its copy cannot compile,
  A pays 2 clang runs on every warm build for no answer (the compiler-engineer, U1: 18/9 against
  16/7).
- **A copy cut before the preprocessor reads another program.** The compiler-engineer's `ctr` (a
  bound function behind a macro that counts) makes routes B and C tell `K_CTR` undefined where the
  program's own unit reads `5 < 3` and 0; the ffi-pragmatist's `att` makes C tell `K_CNT` so. **The
  landed repair of defect 2128 does it today**: its copy writes a refused constant's accessor inert
  before preprocessing, so the compiler-engineer's `inert` and the ffi-pragmatist's `cnt` tell a
  value C defines undefined, and each corrected program builds and prints it (`1048576`,
  `-1073741821`). Filed as defect 2145 (https://github.com/heroes-lang/heroes/issues/2145),
  `blocking`.
- **C-prime and P, built, read the program as it is written.** Both read `d2137.hero` in one build
  (`narrow` and `K_DIV`, exit 1); the compiler-engineer's reaches `d2136.hero` (F), U1, L1 (a copied
  lock) and the second unit of S1 (E); the ffi-pragmatist's ten refusal classes of ten, SQLite's
  real header with a planted refusal (`ffi_return_type` and `K_DIV`, `SQLITE_OK` not told), and
  2109's fifteen defined constants read defined beside a refusal at `-O0` and `-O2`. On each: the
  critic's 17 probe cases 17 of 17; `unsupported` 353 and 0 and `full` 40 and 0 (the
  ffi-pragmatist's own run, the critic's on `heroes-g`); the compiler's own tests 1732 and 1731
  passed; no build's C moves (the compiler-engineer: six programs' `--emit-c` by `cmp` and their 13
  units by SHA-256; the ffi-pragmatist: four units and two `--emit-c` by `cmp`); a build that is not
  refused pays nothing (the same clang runs; the compiler building itself warm in 34.97 to 34.98 s
  before and 34.87 to 34.95 s after, three runs each); a refused build binding a header value pays 5
  clang runs and about 0.6 s cold, 1 run and 0.04 s warm, per unit asked.
- **Where they differ, the critic's § 2**: `tdef` (a refusal in a typedef) is answered by P alone,
  which cuts the typedefs; `twin` (a refused constant whose own C type is refused) by C-prime alone,
  since P compiles an inert twin that spells that type; `first` (a copy that cannot compile) by
  C-prime alone, which asks one unit at a time; F by C-prime alone.
- **Both cuts are defeated by a header that writes the mark** (the critic's § 3.1): a `pack(1)`,
  then the mark's text as a line or through `_Pragma`, then `K_SZ`, whose value reads `sizeof`.
  Each prototype tells `K_SZ` undefined beside `narrow`, where the program as written reads
  `INT_MIN / 2` and its correction prints `-1073741821`, meeting both seats' own falsifiers; and
  since C-prime cuts every probe's text, a correct program whose header holds its mark (`forge1`)
  is refused at exit 2. Without a forged mark the critic found no false word: a pragma's state
  across a cut (§ 3.2), `#pragma once` and include guards (§ 3.3), F beside the lock refusal (§ 4).
- **On an MSVC target `clang -E` expands macros inside an unknown pragma**, measured by the
  coordinator on the Windows box (clang 23.1.1, `x86_64-pc-windows-msvc`) and on this Mac under
  `-fms-extensions`: with `cut`, `keep`, `heroes_probe` and `hero_probe_cut` defined,
  `#pragma heroes_probe cut` printed `#pragma 3 1` and `#pragma hero_probe_cut on` printed
  `#pragma on_by_macro on`; `#pragma hero_probe_cut_0123abcd`, one identifier, printed itself on
  every target, and under `-fno-ms-extensions` and `--target=x86_64-pc-linux-gnu` all three did. So
  both prototypes' marks can be rewritten on Windows by any header defining one of their words.
- **The probe's own preprocess adds one word the program's does not**, found by the coordinator
  while writing this file: `-D main=hero_probe_hidden_main`. A header asking `#ifdef main` is read
  one way by the program and the other by every probe: a value the program defines refused as
  undefined at exit 1, and its mirror built at exit 0 printing `6125380652`. Filed as defect 2146
  (https://github.com/heroes-lang/heroes/issues/2146), `blocking`, beside the sitting rather than
  inside it: it is panel 216's probe, a copy's and a compiled unit's alike.
- **A probe's kept verdict does not see its header change**, found by the compiler-engineer
  building the union (§ 6 of its third report) and reproduced by the coordinator with the lane's
  stage 2 and on the box: `K_DIV`, `7` in a cold build, rewritten in its header to `INT_MIN / -1`,
  builds warm at exit 0 and the program prints what the machine held (`6104441900`, `6102508604`
  and `6097871916` over three runs here, `2242234981312` there), the key of a compiled unit's
  verdict holding the unit's text, which names the header and does not hold it. Filed as defect
  2147 (https://github.com/heroes-lang/heroes/issues/2147), `blocking`, panel 216's probe and
  panel 218's C2 rather than the copy, whose key covers every header's digest.
- **The union, built by the compiler-engineer's third step** (`compiler-engineer-union.md`, its diff
  11 files, 546 insertions and 113 deletions): 43 programs, each built in a fresh directory by
  stage 2, `heroes-g` and the union. It tells every value either prototype told (`tdef`, `twin`,
  `first`, `mixed`, d2137, d2136, U1, L1, S1); no `forge*` program tells `K_SZ`; `ctr` tells no
  `K_CTR`, and `inert` and `cnt` tell `K_BAD` alone; every program stage 2 builds it builds, with
  the same output (`forge1` prints `5`, every `forge*-next` `-1073741821`). The critic's 17 probe
  cases 17 of 17; `unsupported` 353 and 0; `full` 40 and 0; `layout` 6 and 0 (`rounds.hero` 293,
  `decls.hero` 299, `writer.hero` 299 by its rule); the compiler's own tests 1733; `emit` 13,
  `order` 3, `canonical` 2, each 0 failed; six `--emit-c` by `cmp` and 13 units by SHA-256 the same
  as stage 2's, and none of the 80 units the table compiled holds a mark; the cost run for run as
  `heroes-g`'s, a warm d2137 paying 8 clang runs where a cold one pays 18, since the copy's key is
  the same on every build and its verdict is read back.
- **The key alone does not hold; the count does** (the compiler-engineer's § 5). The key is the
  digest of the copy and of every file the build's probes list its headers reading, so a header
  holding it holds a digest of itself; but the digest is no cryptographic hash, and defect 2146
  gave a header a file the probe reads and no listing does (`fk`, a header including a file only
  `#ifdef main`). With the key learnt and written there, the union was silent, five lines carrying
  the key against the four written; a model built without the count told `K_SZ`, false, the
  corrected program printing `-1073741821`.
- **On the Windows box, under the union** (the coordinator, the lane with the union built there in
  two stages, `sources 2fe5f3ac647a2fae` as on this Mac): every program of the compiler-engineer's
  § 9 reads as this Mac's column, d2137, d2136, U1, `tdef`, `twin`, `first`, `mixed`, `ctr`,
  `inert`, `cnt`, the `forge*` programs and 2109's case at `-O0` and `-O2`; **`fk2`**, a header
  defining the keep mark's keyed name as a macro, had two of its four marks written `#pragma 3` by
  the MSVC target's `-E`, and its copy kept no verdict while the build told `narrow` alone, the
  compiler-engineer's prediction, scored true; the copy's preprocessed text there ends each of its
  1,402 lines with a carriage return, and the cut read it; and the compiler's own tests there, 1737,
  all passed. The commands and their output are `docs/panel/225-reports/coordinator.md`.

## Verdicts

| seat | verdict | condition |
|---|---|---|
| compiler-engineer | **adopt C-prime with E and F** (built); **veto B and C** (soundness: a measured false message, design.md §4.17); object A as the resolution and D; approve E; approve F at `check_all`, `lock_told`, `two_ways`/`defined_twice`, object at `bound_macro` and `pragma_ask` (reasoned, unrun). **Third step: adopt the union** (built, `heroes-u`) | turns to object if a program is shown whose asked constant the cut copy judges otherwise than the program's own unit as written, or if `unsupported`, `run` or `emission` moves anything but the new cases; withdraws the veto if B or C preprocess the whole unit first, which is C-prime, or if the author rules that a value told beside a refusal speaks of the corrected program. Third step: turns to object if the union's copy tells an asked constant otherwise than the probe of the same program's compiled unit (defect 2146's word aside), if the box's `fk2` copy answers or any `forge*` or `fk*` program tells `K_SZ` there, or if `run`, `emission` or `determinism` moves anything but new cases |
| ffi-pragmatist | **approve P with E** (built); **veto C** as a cut made before preprocessing; object A, B and D; approve F's C side on condition | turns to object if a CI leg's `clang -E` does not pass the marks as whole lines, or if a program is found where P's copy tells a value the corrected program's own build accepts |
| critic | no verdict | both cuts defeated by a header writing the mark; the seats hold the resolution to two different programs (§ 5.1); G, H, I and the union listed by neither seat; two questions the sitting did not ask (§ 8) |

## Resolution (provisional: author ratification pending)

**R1, the union, built.** Beside a refusal, a value only the probe sees is read from a copy of the
refused unit, and before a refusal made before any unit from a copy of every module's unit:

1. **the copy is the program's own unit**, written by the emitter with marks, preprocessed whole
   under the unit's own words, and cut after (C-prime and P), so every expansion a kept accessor
   makes reads as the program's unit reads it (`__COUNTER__`, `__LINE__`, `__FILE__`,
   `__OPTIMIZE__`, a `#define`), once the one word the probe's preprocess adds, defect 2146's, is
   gone in the same landing;
2. **the copy keeps the opening and the asked accessors** (P's extent): the typedefs, the
   string-literal table, the assertions, the functions and the refused accessors stand in stretches
   the cut removes, so no refusal outside an asked accessor can stop it (`tdef`);
3. **a refused accessor is written as the program writes it, inside a cut stretch, and the probe
   never asks it** (C-prime's; `twin`); `emit/constant_accessor.inert` is retired, which repairs
   defect 2145;
4. **the cut is applied to a copy alone** (P's, H): the probe of a unit that compiled reads that
   unit's text whole, as panel 216's R1 reads it (`forge1`);
5. **every refused unit is asked, one at a time** (E; `first`), a copy that does not compile silent
   alone; this is defect 2143's probe half, and the second unit's own clang refusal, its other half,
   stays open;
6. **F at `probing.check_all`, `lock_told`, `two_ways` and `defined_twice`** (defect 2136 and its
   family), not at `bound_macro` and `pragma_ask`, each of which refuses because the unit's reading
   of a name cannot be trusted (the compiler-engineer's reason, reasoned and unrun);
7. **a mark no header can write** (G, both forms): each mark one identifier carrying the copy's own
   key, the digest of the copy and of every file the build's probes list its headers reading, so no
   macro an unrelated header defines can rewrite it on any target; and **the count, which carries
   the soundness**: the cut finds exactly the marks the writer wrote, in their order, or the copy
   is silent, never a verdict (a header that learnt the key, `fk` and `fk2`, made it silent and no
   more, on this Mac and on the box).

**R2, the standard of truth is the program as written** (design.md §4.17, *every message is true of
the program as written*), the critic's first question (§ 8): a value told beside a refusal is a
statement about the unit the build tried to compile, read by the copy as that unit reads it. The
ffi-pragmatist's falsifier, the corrected program's next build, names no one program: a value read
through `__LINE__` beside a refusal is told by the trunk today, and one correction makes it defined
while another does not (the critic's § 5.1). A reading that moves with the author's correction is a
property of telling anything beside a refusal, not a defect. A ruling for the corrected program
would write §4.17 and convene the full panel.

**R3, a header does not steer how the compiler reads its own copy**, the critic's second question:
the compiler reads back only lines carrying the copy's key, and only when their count is the count
it wrote; whatever else a header writes is the header's, preprocessed and kept or cut with the
stretch it stands in; a count that differs makes the copy silent. A header that learns the key can
make the copy silent and nothing more, the cost of telling the value a build late, as before this
sitting.

**R4, refused**: B and C (vetoed by both seats as cuts made before preprocessing, a measured false
message); A as the resolution (its reading is kept whole in R1, which answers wherever A does); D
(2137 and 2136 count `blocking` at the next tag, and the form of D that would let one be placed
writes §4.17); I as a separate check (unbuilt): it would compare the copy with a second preprocess
of the same text under the same words, so it could catch only what differs between two runs of the
preprocessor, the moment and the place it reads (`__TIME__`, `__TIMESTAMP__`, a quoted include or
`__has_include` searched beside the main file, whose directory a probe's and a unit's differ in),
which the program's own compile, earlier, shares with neither; whether a header's value can read
them so is a question, unrun, of panel 216's probe whole rather than of the copy, and the forged
mark, which I would not catch, is R1's seventh part.
**What conservative would have been**: P with E as the ffi-pragmatist built it, the smallest diff
that answers 2137 (158 insertions and 69 deletions, and a new module of 75 lines), its marks
unkeyed and its inert twin kept, no F: 2136 told a build late and counting `blocking` at the next
tag, `twin` and `first` unanswered, and a header writing the mark, or on Windows defining one of
its words, able to move the cut. The author can choose it.

## Predictions to score

| seat | prediction | instrument | when |
|---|---|---|---|
| compiler-engineer | landed, `emission` moves 0 blessed files and `emit` 0 goldens, and each new `unsupported` case tells its value in the same build as its refusal, `ctr` telling no `K_CTR` and `inert` `K_BAD` alone | the landing lane's gate: `emission`, `emit`, `unsupported` | the batch gate, before `m-witnessed-seed` is placed |
| ffi-pragmatist | the cases landed from `d2137.hero` (both diagnostics, exit 1) and `cnt` (`K_BAD` alone) read the same on all four CI legs, every leg's `clang -E` passing the marks as whole lines | the `unsupported` form on the CI's legs | the landing push's first CI run |
| compiler-engineer, third step | on the Windows box, under the union, `fk2`'s copy keeps no verdict while its build tells `narrow` alone at exit 1, and every row of its § 9's item 3 reads as this Mac's column | the coordinator's run of § 9 on the box | **scored true, 2026-10-19**, before this synthesis adopted the union (§ What was measured) |

## Process notes

- The critic's first pass, over the briefs before any seat was launched, found the family of 2136
  wider than the pointee check, a second refused unit never asked (filed as defect 2143), and a
  refusal `-w` does not lift, and repaired the brief in five places. Its second pass ran both
  prototypes against each other, found the forged mark that defeats both, measured the
  compiler-engineer's whole forms where its report said unrun, and named the union.
- Both seats found, beside the question, the false message the landed repair of 2128 tells (2145):
  two seats, two reproducers, one mechanism.
- The union the critic named was built by the compiler-engineer in a third step, on a brief written
  to disk first (`/panel` § 3c: a route that does not build is not adopted), and run on the box by
  the coordinator before this file adopted it. Two `blocking` defects of panel 216's probe were
  found on the way, 2146 by the coordinator and 2147 by the compiler-engineer; each is the probe of
  every build rather than the copy, and each is repaired in the landing's batch, apart.
- The critic recorded two slips of its own (`critic-briefs.md` § 8): a listing redirected into
  `/tmp`, read back and removed; and two `timeout 60 heroes run` of builds that were refused, so no
  program ran.
- Copies inside the repository's root, `.claude/worktrees/225-<seat>/`, each with its own stage 2;
  the coordinator repaired defects 2142 and 2144 in the lane while the seats worked in copies of
  `7b97b9438` that never saw it, and measured the Windows box's `clang -E` for the third step's
  brief.
- No historian sat; no prior art is claimed.

## Scored, 2026-10-19

- **The compiler-engineer's first prediction: true**, at the batch's gate
  (`17aa3ec38`). `emission` moved no blessed file, its two failures being the
  two new `run/` cases with nothing blessed, blessed apart; `emit` read 13 and
  0; and each of the thirteen new `unsupported/` cases tells its value in the
  same build as its refusal,
  `fixedbugs-2137-a-call-behind-a-counting-macro-moves-no-value` (the seat's
  `ctr`) telling no `K_CTR` and
  `fixedbugs-2145-a-value-defined-beside-a-constant-the-region-refuses-is-not-told`
  (its `inert`) telling `K_BAD` alone.
- **The ffi-pragmatist's: true**, at the landing push's first CI run (39234988708,
  at `9efed1a96`): the run read `unsupported` at 0 failed on all four legs, Darwin
  arm64 367 passed, Linux x86-64 and Linux arm64 366, Windows x86-64 351, every
  leg's passed and skipped summing to the same 369 and neither case skipped on
  any of them; Windows skipped one case of the landing alone,
  `fixedbugs-2136-a-value-only-the-probe-sees-is-told-beside-a-copied-lock`,
  whose header includes `pthread.h`.
- **The compiler-engineer's third step's: true**, before this file adopted the
  union (§ What was measured).

## Author's verdict

Pending. The decision issue is https://github.com/heroes-lang/heroes/issues/2148.

**RATIFIED, 2026-10-19**, R1 to R4 as written above, its three predictions scored true, the
author answering in conversation that evening, meant as *ratify*, on the coordinator's
recommendation to ratify: the union of the two prototypes as landed at `c03e81984`, the copy
preprocessed whole and cut after, a copy's alone, its marks keyed and counted; every refused unit
asked, one at a time, and the probe asked beside the four refusals made before any unit; the
program as written the standard of truth; and what conservative would have been recorded and not
taken. Recorded as a reading (CLAUDE.md § 4). The author may overturn it.

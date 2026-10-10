# Panel 207: a constant's body refuses at least the integer steps whose operands are known, and the spec says exactly that

Convened 2026-10-10 by the coordinator under the author's goal of about
02:30, *at most five open, all improvement*, for defect 580, which lane
b18-close filed in its landing of panel 206 (ratified by 10:22). **Soundness
with the spec-warden, the historian and the ffi-pragmatist, no blind seat**
(no new form; panel 206's readers had measured the expectation of a constant
computed when the program is compiled), and the completeness critic before
the seats and after them. The tree frozen at **`259de604`**, worktree
`lane-panel-207`, batch 18's round. Briefs written from 11:35; the critic's
first pass from 11:36:14 to about 11:47, read at 11:46, its repairs applied
before any seat started (C4b false on straight-line bodies too; design.md
`:3165-3170` forbids a quota; R1 refuses dead steps; the forms a body admits;
the census); the account's session limit stopped every seat at about 11:5x,
resumed at 12:02 on the author's new login; the historian's reply copied at
12:10, the spec-warden's at 12:14, the ffi-pragmatist's at 12:15, the
compiler-engineer's at 12:21; the critic's second pass from 12:21:17 to
12:28, copied at 12:29; this synthesis from 12:29, every time read from
`date`. Briefs in `207-briefs/`, reports in `207-reports/`.

## The verdict table

| seat | verdict | on what |
|---|---|---|
| compiler-engineer | **veto** an exact evaluator with a step bound; **approve** narrowing C4b to the walk's class; **object** to route A (refusing `while`) as 580's repair | route A built twice (in the resolver it hid the checker's 32 diagnostics; in the checker within noise on `check selfhost/main.hero`, the compiler's own tests 1,565); a string doubled in a `for` grows 2^n bytes, a byte quota owed too |
| spec-warden | **object** to C4b as landed (false on 22 of the 46 judged shapes); **veto** the evaluator and probe routes (quotas); **approve** D1r provisionally; N1 preferred if the spec must state compile-time refusals exactly | `heroes measure` on sixteen drafts, 54 shapes, route A's binary on `check` |
| historian (advisory) | **object** to a step bound in the language; **approve** structural routes or a reworded C4b | Rust's `const_eval_limit` (#67260, removed by #103877, Rust 1.72's reason), Zig's quota (#21324, #22410, #16983 accepted), C++ (N3652, clang #143785 refused, #160440), D, Nim; Ada RM 4.9 and 6.8, Go, Starlark, Dhall, Agda and Idris; autoconf, CMake, `go generate` |
| ffi-pragmatist | **object** to route C (run the emitted accessors at build time) | the probe built by hand on this Mac and Linux arm64: exact at the `.hero` line, a group constant's platform value right (`PATH_MAX * 600000` refused on Linux, built on macOS), 0.42 s on the compiler's 403 constants; but its bound is a wall clock that refused one binary once and accepted it five times, macOS gives no memory bound, and a cross build reads the host's header |
| critic, second pass | N1 exact on all 46 shapes, the 206 shapes and eleven edges, but frozen to the walk if read as *if and only if*; D1r right on 0 of 6 reader predictions, its *may* weakening a ratified *is a compile error*; G1 (N1 with *at least*) exact and open above; route W (forget at a loop only the cells it writes) sound and unbuilt; the diagnostic note restates C4b | its copy of the frozen compiler and route A's, `fn/` analogs, `heroes measure` |

## What the sitting measured

- **C4b is false on 22 of the 46 shapes the walk judges** (the spec-warden,
  the critic): a constant body builds and aborts at every read on an index
  out of range, a string index, a failing `assert`, a nan in `<`, a branch
  the walk cannot decide, a cell read in a loop that does not write it, a
  constant computed by a loop, and a group's constant; the walk refuses an
  integer operator whose operands it knows (a literal, an operator on known
  values, a `=` name or written constant holding one, a cell since its last
  write with no loop or branch between that writes it, a loop body knowing
  only the cells it wrote itself), read or not, in dead code too (R1).
- **No route makes C4b true without a quota.** An exact evaluator needs a step
  bound for `while` and a byte bound for a value that doubles (`s @ s + s`
  in a `for` of three, eight bytes; 2^n for n lines); design.md `:3165-3170`
  returns compile-time evaluation only with *a structural termination
  argument ..., never a quota*, and panel 039's agreement 3 says the same;
  every language that shipped a step bound softened it (the historian).
  Route A (no `while`) leaves 15 shapes false and refuses the ratified
  `LOOPED`; route C's bound is a wall clock whose answer did not repeat.
- **The diagnostic note restates C4b**:
  `selfhost/check/const_values.hero:199`, *a step of a constant's body that
  aborts is a compile error, whether or not the step or `V` is ever reached
  (spec § 4)*, pinned 8 times in the 577 `full` golden, false on the same
  shapes (the critic, an unlisted message).
- **The drafts**, vendored rows against C4b (7430 / 7565), lower bounds: D1r
  -1/-1 (*may be refused*: right on 0 of 6 reader predictions, it predicts
  not even `200 + 100`); N1 and G1 +60/+59 (exact on every shape; G1's *at
  least* keeps it true as the walk learns more); G3 +47/+46 (G1 without the
  loop clause, true only under route W).

## Disagreements, stated plainly

- **D1r or the exact class.** The spec-warden approves D1r and prefers N1
  only if the spec must state compile-time refusals exactly; the author's
  instruction of 2026-09-28 says of the spec's tokens *always the most robust
  and solid route, at their cost too*, and D1r's *may* weakens the clause the
  author ratified at panel 206 (*is a compile error*). The resolution takes
  the exact class, in G1's open form.
- **Route W.** Unbuilt, it would make the walk better (a loop forgets only the
  cells it writes, sound since an unwritten cell keeps its value every turn)
  and the sentence shorter (G3). The resolution adopts it on condition it
  builds clean; G1 lands otherwise.

## The resolution, ratified by the author (below)

The most robust and complete route at every question (CLAUDE.md § 4,
CL-040); what conservative would have been is below the list.

1. **R1, spec § 4's sentence states the walk's class exactly, open above.**
   With route W built and measured clean (the walk forgets at a loop's entry
   only the cells the loop writes; the census, the 577 goldens, the cost on
   `check`), § 4 reads G3 made open, *a written body computes over literals
   and other constants. An integer operator in it that would abort, run or
   not, is a compile error at least where its operands are known: a literal,
   an operator on known values, a `=` name or written constant holding one,
   or a cell since its last write with no loop or branch between that writes
   it.* (about +49 vendored, priced at the landing); without route W, G1
   whole (+60/+59). The real count by one `--refresh` at the landing on the
   author's yes.
2. **R2, the diagnostic note says the same class**: `const_values.hero:199`
   and the 577 `full` golden's eight notes reworded by hand, the golden
   re-annotated; `const_steps.hero`'s module doc (`:1`, `:29`) made true.
3. **R3, what the class leaves aborts where it runs**, as § 7 says: an index,
   a string index, an `assert`, a nan, a branch the walk cannot decide, a
   group's constant, a hang; one `run` witness per kind (`loop`, s02, s06,
   s07, s15, and s31 unless route W refuses it), each pinning the abort.
   Defect 580 closes with R1, R2 and the witnesses.
4. **R4, the record**: a dated line beneath panel 206's R1 (*its heading reads
   wider than its body: the class is the integer operator whose operands the
   walk knows*) and beneath its R5 (C4a priced again here, not adopted).
5. **R5, refused**: an exact evaluator with a step or byte bound (two vetoes;
   design.md `:3165-3170`); route A as 580's repair; route C (a wall-clock
   quota, wrong under a cross build); D1r (predicts nothing, weakens a
   ratified clause); N3, G2, G4 and C4a (false or reopened).

**The conservative alternative, the author's to choose instead**: D1r, *a
step of it that would abort may be refused* (-1 token), true on every shape
and predicting nothing, the note reworded to match.

## Predictions to score

| seat | prediction | checkable at |
|---|---|---|
| spec-warden | P2: if N1 (here G1) lands, +70 to +85 real at the `--refresh`; P3: at the next milestone tag, 0 written constants with a `while` outside `tests/golden/` | the landing; the tag |
| compiler-engineer | any route making C4b true on s06, s07, s08 and s15 lands at least 100 new code lines and one new module (not taken) | not taken |
| ffi-pragmatist | under route C with a 2 s wall bound, s16b gets two verdicts on this Mac (not taken) | not taken |
| critic | route W moves s31 and t8 to refused and neither `LOOPED` nor `UNTOUCHED` | the landing |

## Author's verdict

**RATIFIED, 2026-10-10**, R1 to R5 as written above, the author answering
through the question widget between 12:29 and 12:33 by the clocks read
before the question and after the answer, choosing *Ratifica R1-R5* over
*Conservativo (D1r)* and *I want to read it first*, on the coordinator's
summary of each route; in the same widget *yes, one* to the `--refresh` R1's
sentence needs at the landing. Recorded as a reading (CLAUDE.md § 4). The
author may overturn it.

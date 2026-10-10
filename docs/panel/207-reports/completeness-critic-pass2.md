# Panel 207, completeness critic, second pass

Copied by the coordinator at 12:29 on 2026-10-10 (`date`) from the seat's final reply (its transcript), verbatim below the rule; the seat's running notes are in its folder under `.claude/worktrees/scratch-b15/` (ignored by git).

---

# Panel 207, completeness critic, second pass

I worked from 12:21:17 to 12:28 (`date`) in `.claude/worktrees/scratch-b15/critic-207b/`.
- `tree/` is an rsync of `lane-panel-207` at 259de604, without `.git`, `.claude/worktrees` or `build`. I ran no git inside it.
- The compilers were copied in: the frozen tree's `heroes` from `critic-207/tree` and the compiler-engineer's route-A binary (`heroes-ceA`).
- My edge probes are in `edges/`, the function analogs in `fn/`, and the critic-206b shapes are in `s206/` with results in `s206-check.tsv`. The drafts are in `drafts/list.txt`.
- The only git I ran was `git show main:` on the trunk, to read defects 580 and 581.
- `pgrep` found nothing of mine left running.

## Findings, each from a command I ran

**1. The set the sentences are judged on.** The spec-warden's 54 files (3 from p207, 36 critic shapes, 15 probes) split as follows:
- 8 are refused for other reasons: s01, s04, s09, s17, s26, s32, s34, s35.
- 46 are judged by the walk:
  - 14 are refused: flat, s12, s25, s30, t1 to t7, t9, u1, u2.
  - 22 build and abort.
  - 9 exit 0.
  - 1 hangs (s24).

  This matches the warden's count.
- Of critic-206b's 94 files, 30 have a constant. `check` refuses 25: 21 with `int_out_of_range`, 3 with `division_by_zero` (c06, c13, v03), c04 with `constant_body` and t09 with `constant_cycle`. It accepts 5: c03, c11, c12, c23, t12, t14 minus the one that does not belong, see below. Precisely: c03, c11 and c23 are constant bodies with nothing to refuse, while c12, t12 and t14 put the step in a function, where neither sentence applies.

**2. N1 holds as an exact class on all 46, on the 206b shapes and on 11 edge probes of mine.**
- Every one of the 14 refusals plus the 21 `int_out_of_range` and 3 `division_by_zero` refusals in 206b is an integer operator whose operands are known in N1's sense:
  - literals;
  - `-M` and `lim.A` (c05, c18);
  - a `=` name (t1, and my e02 `k = v`);
  - a cell under N1's loop rule: t5, t6, u1, e01, e03, e08, e09, e13 are all refused.
- Every build-and-abort case falls outside the class:
  - an index (`xs[5]`, `XS[0] + 100` in my e15);
  - `assert`;
  - a nan, which is not an integer operator;
  - a cell read in a loop the cell was not written in: s31, t8, and my e16, where the write is in the outer loop and the read in the inner one;
  - a for-loop's name (e11);
  - a constant computed by a loop (s13, u4);
  - a group's constant (s29, t11, c03).

  Two conditions hold this up:
  - It rests on index not being "an operator". § 7's table and its precedence line list no index, so t10 and e15 stay outside the class. A reader who counts indexing as an operator finds N1 false on t10 and e15.
  - N1 as drafted reads *"a cell written one earlier in the text"*. That is ungrammatical, and a landing must repair it.

**3. N1 does not stay true if the walk learns more, by the spec-warden's own standard of reading.**
- The warden scores N3 as false on 9 files (t1, t2, t4, t5, t6, u1, u2, s25, s30) only because the walk refuses *more* than N3 lists. That is reading *"is a compile error where"* as *if and only if*.
- Read the same way, N1 becomes false the day the walk refuses s31. CLAUDE.md § 12 would then name the improved walk as the bug, so N1 freezes the walk's quirk into the language. The quirk is the clause *"in the loop body it stands in if any"*.
- Read as a sufficient condition only, N1 survives. But then N3 was true on its 9, and the warden's table is inconsistent.

**4. D1r is true on 46 of 46 only under a counterfactual reading of *"would abort"*.** D1r dropped D1's *"if run"*.
- The function analogs in `fn/dead.hero` (`if false`, `if 1 > 2`, `while false`, `false && 1 / 0 == 0`) build and print `200`, `200`, `0` and `false`, exit 0. So the steps in s30, t4, s12 and c06 never run.
- Under the reading *"would abort when the program runs"*, D1r does not license 4 of the 46 refusals (s12, s30, t4, t7). It also leaves out 5 of the 30 constant shapes in 206b (c06, c19, c22, t05, t08).
- Those are exactly the dead-code and unread refusals of R1's ratified clause.
- Its *may* predicts nothing: not even `200 + 100` (flat), the case panel 206's readers expected.

**5. What a reader would predict for each shape, against the walk's actual result:**

| shape | walk | N1 | D1r | C4b |
|---|---|---|---|---|
| s02 | 134 | abort ✓ | either | refused ✗ |
| s06 | 134 | abort ✓ | either | refused ✗ |
| s11 | 134 | abort ✓ | either | refused ✗ |
| t4 | refused | refused ✓ | either, or *not licensed* if read literally | builds ✗ (the step never aborts) |
| t7 | refused | refused ✓ | same as t4 | builds ✗ |
| loop | 134 | abort ✓ | either | refused ✗ |

N1 is right on 6 of 6, D1r on 0 of 6, C4b on 0 of 6.

**6. The diagnostic note restates C4b, and nobody listed it.**
- `selfhost/check/const_values.hero:199` emits *"a step of a constant's body that aborts is a compile error, whether or not the step or `V` is ever reached (spec § 4)"*. That is a message (design.md §4.17), false on the same 22 shapes, and it stays false under any rewording unless it is reworded too.
- It is pinned 8 times in `tests/golden/full/fixedbugs-577-a-constant-s-step-that-aborts-says-its-value-and-why.expected`.
- C4b's words also stand in:
  - `const_steps.hero:1`;
  - the header of `tests/golden/check/fixedbugs-577-...-refused-where-it-is-written.hero` (lines 5 to 12). That is an append-only record, so it needs a dated line beneath, not an edit;
  - `docs/measurements/010-spec-budget-ledger.md`, also a record.
- `const_steps.hero:29` says *"nothing is refused that could run"*. The analogs in finding 4 make that false on s30 and t4 under its plain reading.

**7. R1's own body names the walk's class; its heading does not.**
- The heading reads *"a step that cannot be computed is a compile error"*.
- The body (206 synthesis, lines 102 to 116) reads *"every operator step ... overflow, division or remainder by zero and a shift count outside the width refused"*, with *"a group constant giving no value"*.
- C4b took the heading.
- The correction owed beneath R1: the heading reads wider than its body. Index, string index, `assert`, nan, a branch the walk cannot decide, and a cell read in a loop it was not written in all cannot be computed, and they abort where they run.
- Also beneath R5: R5 refused C4a (*"true today, false once R1 lands"*), yet the 207 drafts price C4a again.

**8. Prices: my copy reproduces the warden's, vendored rows, no `--refresh`.** Against C4b at 7430 legacy / 7565 cl100k:
- D1r: −1/−1.
- N1: +60/+59.

## Drafts (Question 2)

**G1 is N1's class, exact, and open above.** It costs +60/+59, the same as N1.

> *a written body computes over literals and other constants. An integer operator in it that would abort, run or not, is a compile error at least where its operands are known: a literal, an operator on known values, a `=` name or written constant holding one, or a cell since its last write with no loop or branch between that writes it, a loop body knowing only the cells it wrote itself.*

- Applying it by hand, its floor is exactly the walk's refused set on the 46, on 206b and on my edges.
- *"At least"* keeps it true when the walk learns more, so it does not freeze the walk.
- It promises nothing about s02, s06, s11 or loop. A reader falls back on § 7's abort, which is right today.

**G3 drops the loop clause.** It costs +47/+46.

> *... where its operands are known: a literal, an operator on known values, a `=` name or written constant holding one, or a cell since its last write with no loop or branch between that writes it.*

- It is false today on s31 and t8: it claims they are refused, and they build and abort.
- It becomes exact under a route nobody listed (route W): change the walk to forget at a loop's entry only the cells the loop writes. That is sound, because a cell the loop never writes keeps its value on every turn. It would move s31 and t8 to refused.
- Route W is not built. Its census and its effect on the goldens are unrun; my reading of LOOPED and UNTOUCHED says neither moves, which is an inference.

**Two drafts I priced and found false:**
- **G2**, *"where its operands are known before the program runs; another step that would abort may be"*, costs +23/+23. Every constant body is closed, so everything in it is "known before the program runs" in principle. Read naturally, it is C4b again.
- **G4**, *"over literals and written constants alone"*, costs +20/+20. It is false on s13 and u4: `A + 100`, where `A` is computed by a loop, is not refused.

## Golden expectations under each sentence (Question 3)

- No sentence changes what the compiler refuses. So no `check` golden (one line each, run as `--brief`) and no `run` golden moves.
- The one expectation that moves is the `full` golden's 8 notes, and only if the note is reworded to agree, which every sentence except C4b owes.
- Route W would move compiler behaviour (s31 and t8). That is unrun.

## Defect 580 (Question 4)

The rewording alone does not close 580. The note in finding 6 is a second false statement of the same class, a message, so 580 closes only with the spec sentence and the note reworded together. The witnesses are `run` goldens. The form pins an abort, for example `fixedbugs-577-a-group-s-constant...expected` reads `!panic: integer overflow`. One per kind:
- `loop.hero`;
- s02, the branch;
- s06 or s27, the index;
- s07, the assert;
- s15, the nan;
- s31, the unwritten cell in a loop;

plus the `full` golden's note, re-blessed. 580's title (*"only on a loop's later turn"*) names one of at least six kinds.

## Questions not asked

1. Does D1r's *may* weaken a clause the author ratified (*"is a compile error"*, chosen over *"Conservativo"*)? If it does, that is the author's to decide, not the synthesis's.
2. Is index an "operator" for the spec's purposes? N1's truth on t10 and e15 rests on § 7's silence.
3. Route W: should the walk be fixed so the spec sentence can be simple, rather than the spec describing the walk's quirk?
4. Who owns the note's wording? It is a message, so § 8's discipline applies, and no seat priced or read it.
5. s24 (the hang) is under no draft's words. Is that deliberate?

## What I did not run

- No `--refresh`, so every real figure but 9944 is unmeasured.
- No blind reader, so the reader predictions in finding 5 are my own application of each sentence.
- Route W is not built. No census, no suite, not even `spec`.
- The dead-code analogs are function bodies standing in for constant bodies. That is a proxy, not the refused programs themselves.
- No `-O2`, Linux or Windows.

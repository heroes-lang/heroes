# Panel 207, completeness critic, first pass

Copied by the coordinator at 11:46 on 2026-10-10 (`date`) from the seat's final reply (its transcript), verbatim below the rule; the seat's running notes are in its folder under `.claude/worktrees/scratch-b15/` (ignored by git).

---

# Panel 207, completeness critic, first pass

Clock: 11:36:14 to about 11:47 (`date`). I worked in `.claude/worktrees/scratch-b15/critic-207/`. `tree/` is an rsync of `lane-panel-207` at 259de604, without `.claude/worktrees` or `.git`, and I ran no git inside it. The compiler there was built from the seed and then from `selfhost/`. The cases were copied to `cases/` and my shapes are in `shapes/`. `probe.sh` runs `check`, then `build`, then the binary under `timeout 10` with output cut at 400 bytes. Results are in `shapes-round.txt`, `shapes-round2.txt`, `shapes-round3.txt` and `constants.tsv`. No binary was left running (`pgrep`).

## Repairs to the briefs, each from a command I ran

1. **The three p207 cases reproduce as the table says.** `loop.hero` builds and aborts with 134 *panic: integer overflow*. `loop2.hero` prints 200. `flat.hero` is refused `int_out_of_range` with the quoted message.

2. **C4b is false on bodies with no loop and no branch, so the sitting's framing (and defect 580's title) names one gap of at least four.** These are all straight-line bodies, all exit 0 at `check` and `build`, all abort with 134 when run:

   | shape | body | run |
   |---|---|---|
   | s06 | `xs = [1, 2, 3]` then `xs[5]` | *array index out of range* |
   | s22 | the same with `k = 1 + 4`, `xs[k]` | same |
   | s27 | `xs[0 - 1]` | same |
   | s18 | `xs[7] @ 4` (a write) | same |
   | s08 | `s = "ab"` then `s[5]` | *string index out of range* |
   | s07 | `assert 1 == 2` | *assert failed* |
   | s15 | `n = 0.0 / 0.0` then `if n < 1.0` | *a nan in `<`* |
   | s36 | `s = f"{0.1 + 0.2}"` then `s[19]` | index out of range |

   The cause is in `const_steps.hero`:
   - `.index` walks its base and index and judges nothing (lines 181-183).
   - `.assert_stmt` walks its value only (line 114).
   - A float gives no value (line 205).

   The other gaps:
   - **Computed branches.** s02 (`if 1 < 2` writes `v @ 200`, then `v + 100`) and s03 (the same through a `match` on `k = 1 + 1`) both abort with 134.
   - **Group constants.** See repair 4.
   - **A `match` on a computed value** goes the same way as s03.

3. **"Forgotten after any branch or loop that writes it" is wrong inside a loop.** Inside a loop's condition and body every cell is forgotten, whether the loop writes it or not (`forgotten`, `const_steps.hero:25-26`, `119`, `126`). s31 shows it: `a: u8 @ 200` is never written by the loop, `t @ a + 100` sits inside it, and the program builds and aborts with 134.

4. **"Its computation is closed: no input, no call" is false.** A written body may read a group's constant, whose value comes from C. s29 (`INT64_MAX + 1` from `stdint.h`) builds and aborts with 134 (`const_steps.hero:26-27`, defect 579). An exact evaluator stops there too. A `while` whose bound is a group constant has a trip count the checker cannot know.

5. **design.md does not "say the same of features"; it rules on this route.** design.md:3165-3170 says three conditions bring compile-time evaluation back, *jointly*. One of them is *"a *structural* termination argument of `mono.rs:24-33`'s standard, never a quota"*. A step bound is a quota.
   - Panel 039's agreement 3 (lines 81-90) has all five seats on it: *"A quota is the wrong answer to termination."*
   - The same paragraph (line 3164) also refuses a pass that *"would duplicate 1,754 lines of runtime semantics"*. That figure is carried from 039.
   - The brief's lead route is the one design.md names as inadmissible, and the seats must be told.

6. **The C1 citation (line 324) is right but incomplete.** It sits in § Appended 2026-09-04, which is the retired note 003's table moved into the sitting, and it frames folding as an optimisation. A body with loops is a zero-argument program, so the row it may fall under is C2 (line 325), *"run a function at compile time"*, refused by Principle 0. The brief should cite C1, C2 and lines 81-90 together.

7. **R1, ratified, refuses steps in dead code.** Its text (206 synthesis, lines 106-108) reads *"the `while false` case refused as `y: u8 = 300` is in dead code"*. The goldens C19 and C22 pin it (`tests/golden/check/fixedbugs-577-a-constant-s-step-that-aborts-is-refused-where-it-is-written.hero:81`, `:89`). My build refuses s12 (`while false`) and s30 (`if false`), and both bodies never run.
   - An exact evaluator that **replaces** the walk accepts them. That reverses a ratified clause, which is the author's call.
   - One that runs **beside** the walk (a union) keeps them refused.
   - The question does not distinguish the two.

8. **The compiler-engineer brief's list of admitted forms is wrong.**
   - "Records" is false: record construction is a call (spec § 9), and s09 (`Point(x: 200, y: 0)`) is refused `constant_body`.
   - Admitted but missing from the list: variant cases with payloads (s21 builds and aborts with 134 through `.num(v: v)`), `assert`, `return` (s25), `break` and `continue`, `f"..."` holes (s36), floats, chars, bools, `nullptr`, index and field reads.
   - `constant_body.hero`'s `bodies` refuses only `.call`, `.method`, `.try_expr` and `.hole`. It refuses no statement kind.

9. **Unstated fact: arrays cannot grow in a constant body.** s34 (`xs + [2]`) is refused `bad_operand` and s35 (`.push`) is refused `constant_body`.
   - So every `for` walks an array whose length the source fixes, and only `while` is unbounded.
   - `while true` with no `break` is already refused `no_value` (s04, s17).
   - A computed endless loop is not refused: s24 (`while i < 1` with `i @ i + 0`) builds and runs until timeout (124).

10. **The census frame, measured.** There are 3,348 tracked `.hero` files at 259de604 and 1,063 top-level written constants (awk, `count.awk`; 1,068 lines start `constant `).
    - `selfhost/` has 333 of them, 0 with a loop or a branch (34 multi-line bodies, not opened).
    - `examples/` has 188, 0 with a loop or a branch.
    - `tests/` has 27 with one (9 with a loop, 18 with a branch), every one a golden about constants: fixedbugs-139, 148, 149, 174, 175, 363, 382 and 577.
    - So the compiler needs nothing past the walk (Principle 0), and the corpus gives no evidence for a value of N.
    - The awk is a keyword match with strings and comments stripped, so it is approximate.

11. **The lane named is neither of SKILL.md's.** SKILL.md (lines 18-29) has the soundness lane (compiler-engineer and ffi-pragmatist only, for no surface, no diagnostic and no spec token) and the full panel.
    - The question includes a spec sentence and, on the bound-refusal route, a new refusal message, so *"no new reader-facing form is proposed"* is false of the brief's own route.
    - Panel 206's readers were measured on arithmetic only. Whether readers expect `xs[5]` or `assert` in a constant to be refused is unmeasured.
    - Leaving out the ffi-pragmatist is a choice where group constants (s29) are the open edge. The brief should say so.

12. **Defect 580 is not in the frozen tree.** `f9da9c95` is not an ancestor of 259de604, and the brief gives no path to it. The trunk path is `git show main:issues/2026-10/10/2026-10-10-1125-defect-580-...md`. Its sentence *"deciding it needs an exact evaluator ... with a step bound"* is a claim about the option set (CLAUDE.md § RUN IT). The routes below show it is one of several.

13. **Facts I verified:**
    - `heroes measure spec/heroes-spec.md` reads real 9944, cl100k 7565, claude-legacy 7430. Headroom is 296 against 10240, of which the FFI floor mortgages 60, so 236 are spendable. The spec-warden brief should carry this.
    - `eb5fef1b` changes `spec/heroes-spec.md`.
    - Panel 206 was ratified by 10:22 (its synthesis, lines 160-162).
    - The critic-206b `shapes/` folder holds 96 files, and the fixedbugs-577 goldens exist.
    - The round has moved on to `be2da1be` (lane b18-ffi merged). That touches no file under `selfhost/check`, `selfhost/resolve` or `spec`; the design.md change is an unrelated paragraph. The frozen tree stands, but the brief should say it is also not the round's head.

14. **Floats.**
    - On this Mac, `X * X - 0.01` with `X` 0.1 prints `1.734723475976807e-18`, the unfused value (Python gives 9.02e-19 for the fused one). So the emitted C does not contract here. `-O2` and the other platforms are unrun.
    - s36 against s37 (`s[19]` aborts, `s[18]` prints 52): whether a body aborts can turn on the runtime's float-to-text. An exact evaluator therefore needs a second copy of that printer, which is design.md's duplication objection measured on one shape.

## Routes nobody listed

- **A. Refuse `while` in a constant body, keep `for`.** This would be syntactic, the way calls are refused. For what it would need to be true, see repairs 9 and 10.
  - It is design.md's structural-termination condition.
  - It needs a byte bound too: `s @ s + s` inside a `for` doubles. I wrote that shape and did not run it.
  - Its step count is a product of literal array lengths: finite, not small.
  - It changes 9 test goldens (LOOPED, UNTOUCHED and C19 among them), a spec sentence and a diagnostic.
- **B. A union:** an exact evaluator beside the walk, so R1's dead-code clause stands (repair 7).
- **C. Run the emitted accessors at build time.** Build the constants into a probe binary and run it, bounded in time and bytes (`.claude/rules/verification.md` § A run that may not end), refusing at the `.hero` line.
  - It is exact by construction, with no second copy of the semantics: float rendering, index checks and group constants (with their platform value) all come for free.
  - It would have to be true that a refusal at `build` and not at `check` counts as C4b's "compile error".
  - It pays one probe per build. It is the shape of the ffi-pragmatist's 206 probe.
- **D. Extend the walk on the straight path, with no bound.** Index read and write, string index, `assert` and nan ordering (repair 2). That leaves only loops and computed branches for any bound to cover.
- **E. Compute each constant once, at start or at first read.** It refuses nothing new, but it turns an abort at every read into one, and a hang (s24) into one hang. It is an emitter change.
- **F. A warning at the bound, with the abort kept.** C4b would then need words for it.
- **G. Emit the evaluator's value as the constant.** The evaluator would become the semantics, which means two copies.
- **H. Interval analysis through loops.** It refuses on "may abort", which refuses correct programs.

## Questions the sitting does not ask

1. **Which aborts can a call-free body reach, and does C4b mean every one?** Eight are measured above, and a hang is not an abort. `.claude/rules/spec-shape.md` records that panel 087 refused a closed list of aborts. A narrowed C4b that enumerates which aborts are refused risks becoming that list.
2. **Does design.md:3165-3170 bind an evaluator used only for a diagnostic?** If it does, the bound route owes the three joint conditions or a panel on that paragraph.
3. **Is C1 or C2 the row for a body with loops?**
4. **At the bound, a refusal refuses a correct program.** s16 (10^7 iterations) prints 10000000 at run time, and `.claude/rules/verification.md` classes "a correct program refused" as `blocking`. What is N? Is it stated in the spec or as a flag (CLAUDE.md § 10)? Is it part of the language across compiler versions?
5. **Is a hang (s24) refused, and under which sentence?**
6. **Is a byte bound owed as well as a step bound?**
7. **Does the evaluator replace R1's walk or join it?**
8. **Are the straight-line kinds 580's, or a defect of their own?** The cause differs (the walk judges arithmetic only, rather than forgetting cells in loops), and the class is `blocking` by CLAUDE.md § 12.
9. **What does `check` cost?** The worst case is N per constant per check. The compiler's own 333 constants pay nothing, but a user's program can.
10. **What happens to a loop whose trip count is a group constant?**

## What I did not run

- No evaluator, no suite, no census of any route, no `--refresh`, no `-O2`, no Linux or Windows.
- s28 (string doubling, unbounded bytes) was written and deleted unrun.
- The 200k-constant chain went through `check` only (10.28 s real, exit 0, linear against the 20k chain's 1.01 s). It was not built or run.
- I did not open the 34 multi-line `selfhost/` bodies.
- Carried and not re-run: the "2 of 2" readers, the author's 02:30 goal, and whether the b18 lanes' agents are running (their worktrees exist).

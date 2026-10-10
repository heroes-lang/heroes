# Panel 206, compiler-engineer

Copied by the coordinator at 09:30 on 2026-10-10 (`date`) from the seat's final reply (its transcript), verbatim below the rule; the seat's running notes are in its folder under `.claude/worktrees/scratch-b15/` (ignored by git).

---

# Panel 206, compiler-engineer

**Clock:** 05:04:58 to about 05:08, then stopped by the session limit. Resumed at 09:16:56 and finished at 09:29:48 (`date`). One slip: a stray `cat > /tmp/x` created an empty file outside the repository at 09:17. I confirmed it was empty and removed it at once; it is in `notes.txt`.

**Compilers:** I built my copy's seed, then `bin/round` from the round's `selfhost/` (05:07). Three prototypes were built from it:
- **`bin/proto2`** (full scope): it refuses three things when every operand is a literal or a literal expression — overflow, `/` or `%` by zero, and a shift count outside 0..63.
- **`bin/proto3`** (overflow only).
- **`bin/proto1`** is proto2 without a cheap kind check up front; it is kept only to show the cost of that check.

---

- **verdict**: **object**. No veto: this adds no core construct and stays under the ceiling.

- **section**: design.md §1.7 (it is frontend-only, so no veto ground), §1.0 as §1.12 restates it at `design.md:594` (*"it would be safer" is not an entry ticket*), and Part 6's paragraph on compile-time evaluation (`design.md:3106-3130`).
  - Whether a fold used only to produce a diagnostic counts as panel 039's C1 is not covered by the document. C1 is filed as *an optimisation*, and this pass changes no emitted code.

- **implementation_cost** (all of it in the checker; nothing in the lowering, the IR or the emitter):
  - **New module** `selfhost/check/literal_steps.hero`:
    - 166 code lines for proto2, 197 by `wc`;
    - 158 for proto3, about 154 once its now-unused `aborting` function is deleted.
    - Counted with an awk copy of `suite_layout.hero:955`'s `code_lines`. The `layout` suite itself was not run.
  - **Driver** `selfhost/checker.hero`: +4 lines (frozen file 160 code lines).
  - **Untouched**: `check/walk.hero` (2432 by `wc`, already past its decided ceiling).
  - **How it works**: one forward pass over the expression arena. Each node is computed at the width the checker recorded on it, which is the width the C is emitted at. It stops at any operand that is not a literal and never reads another declaration, so it always terminates.
  - **What already existed**: the checker had no evaluator of a constant's body. `contextual.hero:140` `literal_value` decodes only a literal and its minus sign. A constant is emitted as a C function called at each read (`uint8_t h_const_BIG(void)`, seen in `runs/const.c`).
  - **Instructions retired** on `check selfhost/main.hero` (`/usr/bin/time -l`):

    | compiler | runs | change against the round |
    |---|---|---|
    | round | 79.42 to 79.59e9 | baseline |
    | proto2 | 80.22 to 80.27e9 | **+1.0%** |
    | proto3 | 80.32 to 80.34e9 | **+0.93%** |
    | proto1 | 81.48 to 81.51e9 | +2.5% |

    The wall-clock time difference was within noise.
  - **Compiler's own tests** with proto2: 1555 passed.
  - **Census**, `check` over 3299 tracked `.hero` files, proto2 against the round. Two files move:
    - `tests/golden/check/repeat-count-is-unsigned.hero` now refuses `0 - 1` (`int_out_of_range`). This brings panel 054's witness back.
    - `tests/golden/run/adversarial-short-circuit.hero` goes from exit 0 to 1, refusing `false && 1 / 0 == 0`. **That is a correct program refused**: it is the repository's own proof that `&&` short-circuits.
    - proto3 moves only the first of the two.

- **needed_for_self_hosting**: **no**. proto2 checks `selfhost/main.hero` at exit 0, so the compiler's own source contains no such step.

- **argument**:
  - This only adds work to the checker, and it fits easily: about 160 lines in one module.
  - It still fails Principle 0. The compiler's own source never hits it, and nobody has measured an effect on the thesis.
  - It refuses only operations whose operands are all literals. Every plausible mistake built from a name stays a run-time abort, in both compilers: `MAX + 1`, `x + 1` on a known binding, a constant read through another, `n - 6` for a `repeat` count (`t03`, `s11`, `t12`, `c01`).
  - It refuses steps that never run: proto3 still refuses the critic's `t05`, `t06`, `t07` and `t11`, and the census shows a real file refused for this.
  - The operand typing the round added (defect 564) is the specification, and the trunk's refusals were false messages. So the round's abort should land as it is.
  - For `rep`, the abort happens at the subtraction, which is exactly panel 054's §1.12 reason. Ratify that change here. If 054's *cannot be written* must hold to the letter, proto3 is the cheapest route I built and ran.

- **prediction**: if any refusal of literal steps lands in batch 18 or 19, three things will hold at that batch's gate:
  - its module is no more than 170 code lines in `layout`'s unit;
  - `heroes check selfhost/main.hero` retires at least 0.5% more instructions than the round;
  - the census moves no tracked file other than the two named above. Any other mover falsifies this.

- **condition**: I would approve proto3's scope given either of these:
  - a measured effect on the thesis: a harness or blind run in which at least 1 in 20 generated programs holds an operation on literals alone that aborts at run time today;
  - a ruling by this sitting that an unreached step may be refused, as `y: u8 = 300` already is in dead code, together with the pass moved into the operand walk at `walk.hero:584` and costing under 0.3%.

**What I did not run**:
- Any trunk compiler: the trunk-side behaviour is the critic's and the brief's, so it is carried. `c01` was not run on the trunk.
- The `layout`, `check`, `run`, `emission`, `determinism` and `annotations` suites with the prototypes.
- `-O2`.
- Tests or goldens for the new module.
- That the IR is unchanged: I only read it. The pass writes nothing except diagnostics.
- A cap on the quoted span: a 300-term chain prints the whole expression in its message, which is a finding for the spec-warden's wording.
- Moving the pass into the operand walk.
- Any precedent behaviour, such as Rust's lint on dead code.
- `measure`, and any paid run.

All of this is in `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/scratch-b15/206-compiler-engineer/`:
- `notes.txt`
- `tree/selfhost/check/literal_steps.hero` (proto3's scope)
- `literal_steps.full.hero` (proto2's scope)
- `census.tsv`, `census-moved.tsv`, `census3-moved.tsv`
- `cases-compare.txt`, `cases-proto3.txt`
- `runs/summary.txt`
- `instr2.txt`
- `bin/`

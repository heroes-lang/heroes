# Panel 206, completeness critic, second pass

Copied by the coordinator at 09:44 on 2026-10-10 (`date`) from the seat's final reply (its transcript), verbatim below the rule; the seat's running notes are in its folder under `.claude/worktrees/scratch-b15/` (ignored by git).

---

# Panel 206, completeness critic, second pass

Clock: 09:30:50 to 09:43:08 (`date`). I worked in `.claude/worktrees/scratch-b15/critic-206b/`. `tree/` is an rsync of `lane-panel-206` (e0aeb991) without `.claude/worktrees`, `.git` or `build`, and I ran no git inside it. Five compilers:
- `round`, `proto2` and `proto3` are copies of the compiler-engineer's binaries. `round` has the same sha1 as its `bin/round`.
- `trunk` is the 00:26 trunk binary (sha1 3852e789, the one every seat used).
- `route1a` and `route1b` are mine, built from `tree/` by `round`.

The probe for each case is `check`, then `build`, then the binary under `timeout 10` with output cut at 400 bytes (`probe.sh`). Every result below is in a `.txt` file in my folder.

## Findings, each from a command I ran

**1. Route 1 is built and run: it refuses a constant's written body whose step aborts, read or not.**

- **What I built.** `tree/selfhost/check/const_steps.hero` is 282 lines by `wc`, or 215 that are neither blank nor comment by awk. That is not `layout`'s unit, and `layout` was not run. `checker.hero` gains 3 lines. The module walks every valued constant's statements. Each operator step is computed at the width the checker recorded on it. A name or a qualified name that is a written constant is read through that constant's own walk, memoised per declaration and guarded against cycles. A group constant (no body), a local, an `if` or a `match` gives no value. The right operand of `&&` and `||` is not judged.
  - `route1a` refuses a step that cannot fit.
  - `route1b` also refuses division or remainder by zero and a shift count outside 0..63.
- **The named shapes:**

  | shape | route1a | route1b | round |
  |---|---|---|---|
  | p206 `const` | refused | refused | 134 |
  | `s11` | refused | refused | 134 |
  | `t04` | refused | refused | 134 |
  | `t05` (unread) | refused | refused | exit 0 |
  | `t08` (unread) | refused | refused | exit 0 |
  | `v03` (`7 % 0`) | exit 0 | refused | exit 0 |
  | `t09` (cycle) | `constant_cycle`, no hang | `constant_cycle`, no hang | `constant_cycle` |

  The refusals are `int_out_of_range` (*`200 + 100` computes 300, which does not fit a `u8`*), except `v03` on route1b, which is `division_by_zero`.
- **Neighbour shapes I wrote** (`c01` to `c23`, in `route1a-c1.txt` and `route1-c2.txt`):
  - Refused by both routes: per step `2 - 3 + 5` (`c01`) and `255 + 1 - 1` (`c02`), `-M` at `i8` (`c05`), `-128 / -1` (`c07`), `0 - 1` at `u64` (`c08`), a step in a local of the body (`c09`), the `i64` overflow past every width (`c15`), `-128 % -1` (`c16`), a forward reference (`c17`), and a qualified `lim.A + 100` (`c18`).
  - Refused by route1b only: `7 / Z` (`c13`) and `1 << 64` (`c14`).
  - A chain that fits throughout, `B - A` (`c11`), prints 55.
  - A constant read in a function, `A + 100` in `main` (`c12`), and the brief's `t12` and `t14` stay aborts. Route 1 never touches function bodies.
  - A group constant in a constant body, `INT64_MAX + 1` (`c03`), builds and aborts 134 on route1a, the round and the trunk.
- **Census.** `check --brief` over all 3299 `git ls-files '*.hero'` of the frozen tree, `-P 3`, compared with the round's column in the compiler-engineer's `census.tsv` (the same binary):
  - **0 files move under route1a, and 0 under route1b.**
  - Both join 3299 rows, and 1583 files are non-zero on every column.
  - `repeat-count-is-unsigned.hero` and `adversarial-short-circuit.hero` do not move.
- **Cost.** Instructions retired on `check selfhost/main.hero` (`/usr/bin/time -l`), run sequentially at load average 7, so wall time is not reported:

  | compiler | runs (instructions) | against round |
  |---|---|---|
  | round | 79.571e9, 79.572e9 | baseline |
  | route1b | 79.611e9, 79.611e9 | **+0.05%** |
  | route1a | 79.589e9, 79.613e9 | +0.02% to +0.05% |

  For comparison, `proto2` was +1.0% and `proto3` +0.93% (carried from the compiler-engineer's report).
- **Own tests** with route1b: **1555 tests, all passed**, exit 0.
- **Emitted C** is byte-identical between round and route1b for `c23` and `c11` (`cmp`).
- **A fact that changes the scope: a constant's body already cannot contain a call.**
  - `c04` (`constant N: u8` of `id(a: 200) + 100`) is refused `constant_body` on the trunk, the round and route 1: *may only read literals and other constants*.
  - That is `selfhost/resolve/constant_body.hero` (panel 039). It refuses `.call`, `.method`, `.try_expr` and `.hole` only.
  - So route 1's "no call" is a rule the language already has, not a choice the route makes. Panel 203 R3's `m2` (a call) cannot occur in a constant body.
  - The class route 1 refuses is therefore already a syntactic class. That is the shape the historian asked for (Go: defined by syntax, not by what a fold sees).
- **Against the blind seat.** On the blind seat's six programs (`route1a-p206.txt`), route1a gives:
  - `sum`, `rep`, `s22` and `s23` (the blind seat's p1, p2, p4, p5) build and abort 134;
  - `const` (p3) and `t02` (p6) are refused.

  That is n1's prediction, 2 of 2 readers on each, on all six. The 2-of-2 figures are carried from the coordinator's scoring.
- **Where my prototype is wrong**, so it is a lower and upper bound, not the route:
  - **It misses** an array element, `[200 + 100]` (`c10`, still aborts 134), and an `if` branch (`c22`, accepted; `proto3` refuses it).
  - **It refuses a step that never runs**: `x @ 200 + 100` inside `while false` in a constant body (`c19`). The round runs that program to exit 0. Loops are legal in a constant body, because `constant_body` does not refuse `while`.
- **F7e is false on route 1** for `sum`, `i8`, `rep`, `arg`, `s22` and `s23`, because they stay aborts. Route 1 needs a § 4 sentence instead. I priced two with `round measure` from `tree/`, vendored tables, no `--refresh`. My base reproduces the spec-warden's 7426 / 7561.
  - **C4b**, *…over literals and other constants, and a step of it that aborts is a compile error.*: **+13/+13**.
  - **C4c**, *…, each time it is read; a step of it that aborts is a compile error.*: **+18/+18**.
  - Both are lower bounds.

**2. `adversarial-short-circuit.hero` (`false && 1 / 0 == 0`).**
- `proto2` refuses it, `division_by_zero` at 10:17. `round`, `proto3`, `route1a` and `route1b` exit 0.
- `proto3`'s clean census is an accident of what the corpus holds, not something its scope guarantees. `c20`, `if false && y == 200 + 100` with `y: u8`, is refused `int_out_of_range` by `proto3`. The round runs it and prints 0.
- `proto3` also refuses `t06` (a function never called), `t07` (a branch not taken), `t11` (a test block) and `c22`. All four run on the round.
- The existing literal rule already ignores reachability. `y: u8 = 300` in a function nothing calls (`c21`) is refused `int_out_of_range` by the trunk, the round and `proto3`.
- **Can any scope over function bodies avoid refusing unreached steps?** That is an inference, not a measurement. Skipping the right of `&&`/`||` and every branch would still refuse `t06` and `t11`. Sparing those needs reachability from `main`, and a `test` block is reached only under `heroes test`. I built no such scope.
- The constant route avoids it differently. A constant body has no inputs and no calls, so the only "unreached" body is an unread constant (`t05`, `t08`), which route 1 refuses by definition. The exception is the loop-body case in finding 1.

**3. `rep`: panel 054 ruled on the count's type; the `0 - 1` witness was a side effect.**
- **What 054 says, in its own words** (`docs/panel/054-a-cost-with-no-exit.md:103-110`): *"A negative count cannot be written, so the class does not arise; `repeat("-", 40)` still reads as one line, because a literal takes the type its context asks for; and a* computed *count is `repeat(" ", col.to_u64().must())`, which aborts with `does_not_fit` … at the subtraction that went negative rather than inside `repeat`."*
- `grep "0 - 1"` finds nothing in the sitting's file.
- The witness line `repeat("-", 0 - 1) #~ type_mismatch` was written by the landing commit `4cafbb05` (2026-08-14), not by the ruling.
- The trunk refuses `repeat("-", 3 - 1)` (`r01`) with the same message it gives `0 - 1`: *expected `u64`, found `i64`*. So it refused a correct program, and the witness rested on the operands not taking the count's type.
- **On the round:**
  - `-1` is still refused `int_out_of_range` on all three compilers (`r02`).
  - `0 - 1` aborts 134 *integer overflow* at the subtraction. That is 054's "computed count" path, but with `integer overflow` rather than `does_not_fit`.
  - `w - c` on `u64` values already aborted on the trunk (`r06`).
- The golden's header line 1 still says *a negative one does not compile*. That is now true of `-1` only.

**4. R11a and C4a.**
- **R11a is true on the round and on route 1.** A `u64` count builds (`r04`). An `i64` count and a `u32` count are refused *expected `u64`* (`r03`, `r05`), on the trunk too. So *`n` a `u64`* is exact; "an unsigned count" would be false.
- **C4a is true on the round.** `c23` reads `BIG` twice. The emitted C calls `h_c23constreadtwice_BIG()` at lines 113 and 118, and its body recomputes with `__builtin_add_overflow` on each call.
- **On route 1 C4a is still true** (same emitted C, `cmp`). But the only reads that can still abort are group-constant operands (`c03`) and my prototype's gaps (`c10`).
- **The reader expectation the coordinator flagged** (2 of 2 expecting the constant refused at compile time) is met by route 1 at zero tokens. Whether a sentence is owed is the spec-warden's § 12 point.

**5. The ffi-pragmatist's probe for group constants is outside route 1.**
- Under route 1 a group constant stays an abort (`c03`), by construction: § 13 says *a group's `constant` has no body*, and route 1 judges bodies.
- I found no recorded objection to the probe itself. I grepped both seats' `notes.txt` for probe, static_assert, group, extern and clang. The only hit is the spec-warden's use of "probes" for its own runs.
- The spec-warden's objection is to wording: F7a and F7c say "constants" and are false on `v02`. The compiler-engineer's is its route stopping at every non-literal operand. Neither contradicts the probe.
- **In scope?** The probe is not needed for C4b or C4c to be true, since they say "written body". Its own report shows three costs: it is platform-dependent (`f02`/`f05` exit 0 on the Mac and 134 on Linux, carried); it is an emitter change; and it is untested on Windows (carried). It reads as a sitting of its own rather than a condition of this one.

## Routes nobody built

- **Evaluate a constant's body exactly.**
  - A body with no calls has no inputs, so an interpreter decides every step exactly: loops (`c19`), branches (`c22`) and arrays (`c10`). It would give neither false refusals nor misses.
  - It needs a step bound, because a `while` in a body can run for 10^18 iterations.
  - It meets panel 039's C1 row ("an evaluator"), and nobody has asked whether C1 covers a fold used only for a diagnostic.
- **Route 1 with a § 4 sentence, plus the round's abort for function bodies.** This is the only combination I ran that matches what the blind readers predicted on all six programs.
- **Emit a constant as a C constant expression** (a `static const` or an `enum`) so that clang's constant folding refuses it. Not built. It would make the refusal `build`-only.

## Questions the sitting did not ask

- Does § 4's existing sentence, *a written body computes over literals and other constants*, already promise route 1? 2 of 2 readers read it that way (carried). If it does, the round's `const` abort is a spec/compiler disagreement under CLAUDE.md § 12, a *wrong one accepted*, so `blocking`, not `improvement`.
- Is a constant meant to be computed on every read (C4a), or once?
- Should a refusal of steps reuse `int_out_of_range`, as both prototypes do, or get a new code? A new code is a diagnostic class, which triggers a panel.
- Should `repeat-count-is-unsigned.hero`'s header (*a negative one does not compile*) be corrected now that `0 - 1` compiles?
- Should a constant body's `while` be judged at compile time at all, given termination?

## What I did not run

- The `layout`, `check`, `run`, `emission`, `annotations` and `canonical` suites with either route, or tests and goldens for the module.
- Census and probes with the trunk beyond the shapes named above. The round's census column is the compiler-engineer's (same binary). The `selfhost/` files of the census were checked in my copy, which holds my module; its column was checked in its copy, which holds `literal_steps.hero`.
- A route that does not descend into loop bodies, or that does descend into arrays and branches.
- Linux, Windows, the ffi probe, `measure --refresh`, any paid run, `-O2`.
- `tree/` is left in route1b's state (`DIVISION` true).

Files are in `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/scratch-b15/critic-206b/`:
- `tree/selfhost/check/const_steps.hero`
- `bin/route1a`, `bin/route1b`
- `probe.sh`, `census-one.sh`
- `census-1a.tsv`, `census-1b.tsv`, `census-1b-moved.tsv` (empty)
- `instr.txt`, `selftest-1b.txt`
- `route1a-named.txt`, `route1a-c1.txt`, `route1-c2.txt`, `route1a-p206.txt`, `unreached.txt`, `rep.txt`
- `drafts/C4b.md`, `drafts/C4c.md`
- `shapes/`

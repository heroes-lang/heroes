# Panel 206, completeness critic, first pass

Copied by the coordinator at 05:04 on 2026-10-10 (`date`) from the seat's final reply (its transcript), verbatim below the rule; the seat's running notes are in its folder under `.claude/worktrees/scratch-b15/` (ignored by git).

---

# Panel 206, completeness critic, first pass

Clock: 04:50:49 to 05:00:33 (`date`). Two compilers were used. **trunk** is a copy of `/Users/joseph/Temp/heroes/heroes-lang/heroes`, built at 00:26 from the seed of `65b78f2e`; no seed, `selfhost/` or `runtime/` commit lands between that and `ba4fa89d`. **round** is built in my copy of `e0aeb991`: first the seed, then `heroes build selfhost/main.hero` (real 126.54 s). The probe for each case is `check`, then `build`, then the binary under `timeout 10` piped into `head -c 600`. All 58 shape files are in `.claude/worktrees/scratch-b15/critic-206/shapes/` and the copied cases are in `critic-206/cases/`.

## Repairs to the brief, each from a command I ran

1. **The six-case table is true as written.** Both compilers reproduce every row, the run exit is 134 each time, and `lit` gives `int_out_of_range` on both. One gap: `p206/` holds no trunk-side file for `lit` (`lit.t.txt` is missing), so that cell rested on nothing on disk until this run.

2. **"The trunk refused (for another reason)" is false for `rep`.** On the trunk, `tests/golden/check/repeat-count-is-unsigned.hero:22` is `print(repeat("-", 0 - 1))    #~ type_mismatch`. Line 1 of that file says *a negative one does not compile (panel 054)*. Panel 054, ratified 2026-08-14, ruled: *a negative count cannot be written, so the class does not arise*. `ce9c980b` deleted that annotation and wrote a correction beneath it. So the round reverses a ratified sitting's pinned witness. The brief should say this, and the second question ("a regression or a repair") should be asked of `rep` separately from the rest.

3. **Panel 203 already ratified a run-time abort of literal arithmetic at a narrow width.** R3 (ratified 00:24) says *with both landed `m2` becomes the overflow abort … a `run` witness pinning it*. `m2` is `y: u8 = first(a: 200, b: 2) + 100`, and `tests/golden/run/fixedbugs-564-a-sum-past-its-width-aborts` pins `!panic: integer overflow`. Panel 203's synthesis, line 67, also accepts `y: u8 = first(a: 1, b: 2)` aborting when it overflows. The brief cites R3 only as the ruling behind 564. Any refusal route has to leave `m2` an abort (it has a call) or reopen R3.

4. **"Panel 042's ruling 3" uses the wrong vocabulary.** Panel 042's own "ruling 3" (its lines 13-16 and 254) is *conversions returning `T?`*. Full adoption is item 3 of its provisional resolution (line 286). The ruling that bears on this sitting is panel 042's **ruling 4**, *overflow aborting at every width*, which was fixed going in and which the judges "were told to judge the consequences of, never the rulings". 564's issue and commit carry the same slip.

5. **The class is older than the round.** Both compilers accept each of these and abort 134:
   - a constant read through another constant: `constant B: u8` = `A + 100` with `A` = 200 (`s11`);
   - a binding of a literal: `x: u8 = 255; y: u8 = x + 1` (`t03`);
   - the `i64` default: `9223372036854775807 + 1` in an expression (`s17`) and in a constant (`t04`);
   - unary minus on a constant: `-M` with `M: i8` = -128 (`t12`);
   - division and remainder by a literal zero at `i64` (`s01`, `s02`) and by a zero constant at `u8` (`t14`);
   - a shift count past 63 (`s05`, `s33`);
   - `(0 - 9223372036854775807 - 1) / (0 - 1)` (`s27`);
   - an extern constant: `INT64_MAX + 1` (`v02`).

   What the round adds is narrow widths where every operand is a literal. The brief's question list should say this.

6. **The round also accepts these, which the trunk refused, each aborting 134:**
   - `%` and `/` by a literal zero at `u8` (`s03`, `s04`);
   - `x: u8 @ 0 - 1` (`s10`);
   - an `if` value (`s12`) and a `match` value (`s29`);
   - `repeat("-", 2 - 3)` (`s16`);
   - `x: u64 = 0 - 1` (`s18`);
   - a return (`s19`), an array element (`s20`), a record field (`s21`);
   - a character or hex operand: `0x61 + 200`, `200 + 'a'` (`s24`, `s25`);
   - `x: i8 = -128 / -1` (`s26`);
   - `x: u16 = 256 * 256` (`s28`);
   - a mutation, `b @ 255 + 1` (`s30`);
   - `-(2 - 1)` at `u8` (`s32`);
   - `id(a: 200) + 100` (`t10`).

7. **Programs the round accepts and runs to exit 0 would be refused.** The compiler-engineer's brief asks about exactly this, and here are four:
   - an unread `constant BIG: u8` of `200 + 100` (`t05`);
   - a function never called that returns `200 + 100` as `u8` (`t06`);
   - a branch not taken (`t07`);
   - a `test` block (`t11`).

   Two more pass on both compilers at exit 0: an unread `i64` constant overflowing (`t08`), and `constant N: i64` of `7 % 0` (`v03`).

8. **The value fits, the run aborts.** `x: u8 = 2 - 3 + 5` is mathematically 4 but aborts on the round (`s23`). `255 + 1 - 1` does the same (`s22`). Meanwhile `y: u8 = 300 - 100` is refused `int_out_of_range` even though its value fits (`t02`). So "whose value it can compute … and which cannot fit" means one of two things: the final value, or each step at the width as the emitted C does. The brief must say which. They differ on `s22` and `s23`.

9. **Floats are outside the class by spec § 11.** It says *Nothing fails to fit a float: too large is `inf`*. `x: f32 = 3e38 * 10.0` was refused on the trunk and runs on the round, printing `inf` at exit 0 (`t16`). `f64` gives `inf` on both (`t15`). The question should say the refusal is for integers only.

10. **I found no evaluator of a constant's body**, so the brief's phrase "where the checker already evaluates … a constant's body" is unsupported. The round emits a constant as a C function, `uint8_t h_const_BIG(void)`, holding `__builtin_add_overflow`, and calls it at each read. Grepping `selfhost/` for evaluator-shaped names found only the literal's sign and magnitude (`check/contextual.hero:140` `literal_value`), the lowering's `fold_negative` (`ir/emissions.hero:222`) and `resolve/cycles.hero` (`constant_cycle`). This is a negative claim from my vocabulary, so treat it as a question to the compiler-engineer.

11. **Spec line citations.** § 7's *Overflow aborts at every width* is `:199` in the frozen tree and `:198` on the trunk. `.claude/rules/spec-shape.md` says to cite `spec § 7`, never by line.

12. **No blind-seat brief exists.** `docs/panel/206-briefs/` holds only `00-shared.md` plus the compiler-engineer, historian and spec-warden briefs. The shared brief names the blind seat and *3.69 USD*, but the skill requires a written brief giving the run's size and budget.

13. **The full lane leaves out the ffi-pragmatist, and nothing says why.** The FFI shapes matter here. `INT64_MAX + 1` (`v02`) has a value only clang knows. `UINT8_MAX` declared `u8` passes `check` and fails `build` with `ffi_constant_type` (`v01`), which shows a refusal that only `build` makes already exists.

14. **The tree moves under the sitting.** Lane b18-infer's notes at 04:57 record 576 repaired in `grammar_expr.hero` (uncommitted; `lane-b18-infer` is at `8baae87b`), with three goldens' columns moved. The `` `-(-128` `` and `` `-(1` `` excerpts (`s08`, `s31`, `u05`, defect 576) will change in the round.

15. **The spec-warden needs the base, not only the deltas.** Panel 203's sentence has already landed in the frozen tree: `measure` reads 9889 real, with headroom 351 against 10240, or 291 net of the 60-token FFI floor. The brief's +38/+40, +33/+34, +9/+9 and +42 real all match the sitting files and the ledger.

## Routes nobody listed, and what would have to be true for each

- **Extend `int_out_of_range` to a literal-only operator tree.** It would reuse `literal_value` and `widths.int_fits`, with no new code and possibly no new sentence. True if § 2's *a literal must fit its type* reads as covering `200 + 100`, and if the sitting chooses per-step or final-value semantics (repair 8).
- **Fold in the IR rather than the checker.** Every operand has its width there, and it would reach `t03`, `t12` and `s11`. True if `heroes check` lowers, and if an IR diagnostic can keep design.md §4.17's locality.
- **Let clang evaluate.** Design.md's own answer to compile-time evaluation is that *the preprocessor evaluates*. Emitting a `_Static_assert` over `__builtin_*_overflow_p` would make `build` refuse. True if a refusal by `build` alone is acceptable outside the FFI, as `ffi_constant_type` already is.
- **Restore panel 054's refusal for `repeat` alone.** True if `rep` is judged on its own ruling, separately from 577.
- **Keep the abort and make it say where.** Today it prints only `panic: integer overflow`, even under `heroes run`. True if a source position can reach the panic at no cost.
- **State the abort in the spec.** True if the abort is what is adopted and a reader cannot already derive `s23` and `t02` from §§ 2 and 7.

## Questions the sitting should ask and does not

- **Panel 039's C1 row.** It records *fold a constant expression: no evaluator; refused as an optimisation with no measured need*. Design.md's compile-time evaluation paragraph adds three conditions for returning it to the table. Does a diagnostic-only fold fall under that row, or outside it?
- **577's class.** It is `improvement` ("the program aborts cleanly"). Is `rep` instead *a wrong one accepted* against a ratified ruling, which would make it `blocking`?
- **Warning versus error.** Rust's `arithmetic_overflow` is a deny-by-default lint that can be allowed. `grep -i 'severity|warning' selfhost/diag.hero` finds nothing, so I found no warning class in Heroes, which makes the precedent stricter here than it is in Rust.
- **Missing precedents for the historian.** Kotlin, whose literal rule panel 042 says Heroes follows exactly. C#, where an overflowing constant expression is refused at compile time (CS0220). Java, where constant overflow wraps silently. Nim. These are unverified, offered for the historian to check.
- **Narrow widths only?** Is a refusal there coherent while `9223372036854775807 + 1` stays accepted at `i64`?
- **A possible defect beside 541 and 564, unfiled as far as I found.** The round refuses `y: u8 = id(a: 2 + 3)` (`u03`), `first(a: 2 + 3, b: 1)` (`t01`) and `first(a: 1, b: 2 + 3)` (`u02`), each with *expected `u8`, found `i64`*, while it accepts `first(a: 2, b: 1)` (`u01`). The round's own § 9 text, *else from the type the context asks for*, would have a reader accept those three. My grep of `issues/` found it only in 564's issue.
- **Shift overflow.** `3 << 63` prints `-9223372036854775808` at exit 0 on both compilers (`s07`). The emitted C shifts through `uint64_t`, so it is not undefined behaviour. Does *cannot fit* reach a shift's value, or only its count?

## What I did not run

- The census over tracked `.hero` files: trunk against round, and which accepted files a refusal would move.
- Any prototype of a refusal.
- `measure --refresh`, and any paid run.
- Web sources for the precedents above.
- The `-O2` versions of the cases.

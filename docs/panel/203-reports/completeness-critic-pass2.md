# Panel 203, completeness critic, second pass

Copied by the coordinator at 00:12 on 2026-10-10 (`date`) from the seat's final reply (its transcript), verbatim below the rule; the seat's running notes are in its folder under `.claude/worktrees/scratch-b15/` (ignored by git).

---

# Panel 203, completeness critic, second pass

I worked from 00:02:15 to 00:11:26 on 2026-10-10 (`date`). Everything I ran is in `.claude/worktrees/scratch-b15/203-critic2/`. I used copies of the engineer's binaries, ran them with `HEROES_RUNTIME` set to a copy of the frozen tree's ABI-30 runtime, and measured against an rsync of the tracked files. I wrote nothing in the frozen tree or in any seat's folder, and paid for no run.

## Findings

1. **v3 is not order-free in its verdict. This falsifies a claim the engineer and the historian both relied on.** All three prototypes (v1 `heroes-q1-v1`, v2 `heroes-q1`, v3 `heroes-q1c`) behave the same way:
   - `first(a: [1], b: [])` is accepted, but `first(a: [], b: [1])` is refused `cannot_infer`.
   - `{"k": 1}` with `{}`, and `[[1]]` with `[[]]`, behave the same: accepted in one order, refused in the other (`w/e1a`–`e3b`).
   - The engineer's eight pairs all had one argument with a type of its own, or two literals. None had two arguments that both wait, so this case was never tested.
   - The likely cause, from reading the fallback (not instrumented): forms that refuse themselves (`[]`, `{}`) are tried by position within their class, not after the forms that can settle a type.
   - So no sentence can be true of v3 as built. The order has to be repaired first (the historian's condition 1).

2. **v3 changes the meaning of no tracked program.** I ran `build --dump-ir` with base and v3 on all 1,610 files both accept: 0 of 1,610 differ (`irdiff.txt`, shasum of each output, sample checked to be real IR). Programs v3 newly accepts do get narrow widths, and overflow there aborts at run time (exit 134 on both):
   - `ovf`: `y: u8 = first(a: 200, b: 100)`, then `y + y`.
   - `retctx`: `-> u8` returning `first(a: 250, b: 1)`, then `+ 10`.

   Literals that cannot fit are still refused, now as `int_out_of_range` instead of `type_mismatch`: 300 into u8 (`fit300`, `nest300`, `sib300`), -1 into u8 (`neg`). Inside a generic body, a literal or a `[]` checked against `T` is refused correctly (`genT`, `genT2`, `genT4`). There is one regression in message count: `genT2`'s single mistake is now reported twice (one message per literal), where base reported it once. That is class `adjacent`.

3. **What line 25 measured is a gap in spec § 6, not N1f.**
   - Today `x = ok(1)` is refused everywhere, on base and on v3 (`p/okalone`), because panel 002 makes `ok(…)` take its type from the context only (design.md:1229-1236).
   - Spec § 6 (line 159) does not say this. The only context rule in the spec is § 2's, for literals (grep of "context": lines 43 and 278 only).
   - That is why 4 of 4 readers, in both arms, accepted line 25. Neither N1f nor G3a can fix it.
   - The line-21 disagreement is N1f's own: 2 of 2 readers took "a generic function's parameter" to mean a parameter of generic type. v3 accepts line 21. If v3 does not land, N1f has to be reworded anyway.

4. **A sentence true of v3 (with finding 1 repaired), priced on the vendored tables without `--refresh`.** Base is 7,348 legacy / 7,479 cl100k. The § 6 clause I priced reads: `ok(v)` and `fail` each take their type from the context.

   | draft | legacy | cl100k | true of v3? |
   |---|---|---|---|
   | G0 (N1f removed) | −11 | −12 | no; leaves order and literals to the reader |
   | G3a | +4 | +5 | **no**: false on `apply_u8` and on line 25 |
   | V3T: "…from the arguments that have one of their own, else from the type the context asks for, else from a generic call or a literal among them (section 2); `ok(…)`, `[]` and a case name give none…" | +30 | +31 | yes, once the order is repaired |
   | the § 6 clause alone | +8 | +9 | closes line 25's gap |
   | V3T plus the § 6 clause | +38 | +40 | yes |

   The real count of each is unmeasured. Roughly +50 real, inferred from the 9,847/7,479 ratio, against a headroom of 333 net. N1f's tokens do not come back; the true sentence costs about twice what N1f did. Drafts are in `drafts/`, measured from `tree/drafts203/`.

5. **The 59 lines over walk.hero's ceiling look like a recorded raise, not a blocker, and a seam may exist after all.**
   - Precedent: `tests/harness/suite_layout.hero:100-135` records walk.hero raised four times (1583→1700→1708→1842→1870), each "raised with room", for a ratified form or a defect repair. My recount agrees with the engineer: 1,868 → 1,929 code lines.
   - "No seam" holds only for the new code. Seven existing functions in walk.hero call no function of walk.hero (grep of direct calls), about 115 code lines.
   - The largest is `field_of_function_type`, 46 code lines, which `may_end.hero:150` and `self_call.hero:215` already reach as `walk.field_of_function_type`. Moved together with `synth_name` (21 lines), that is 67 lines, enough to cover the 59.
   - Unbuilt. Whether they reference anything private to walk.hero is unchecked.

6. **v3's cost is linear in nesting depth, measured.** The historian's javac condition was unmeasured until now. In instructions retired, chains of `ident(...)` and `first(a: first(...), b: 2)`, in a `u8` context and without one, cost:

   | depth | base (millions) | v3 (millions) |
   |---|---|---|
   | 5 | 81.3 | 83.0 |
   | 80 | 160.9 | 168.1 |

   The step from depth 40 to 80 is +43.7M on base and +46.7M on v3 (`nest/`). There is no blow-up.

7. **The census gain is 5 distinct programs, and no user program.** The 6 `r1-*` files are one shasum in six copies: they are panel 201's input to its blind readers, not anything a reader wrote. The other 4 are goldens written to pin today's refusals. Nothing in `examples/`, `selfhost/` or `tests/golden/run/` moves. Nobody ran the critic's "Q1 write" task, so Principle 0's thesis case for Q1 rests only on spec truth (§ 12).

8. **Q2: the locality objection already has a precedent in today's checker, ratified by panel 199 R4.**
   - In `xm/`, `step` calls `m.helper` (another module) and then itself. When `helper` can `exit`, `step` is accepted; when `helper` only computes, it is refused `endless_recursion`. A refusal therefore already depends on another module's body.
   - `may_end.hero`'s header (lines 1-20) already judges "a call through the function's OWN parameter … at each call site … by what is handed in", as a least fixpoint over the whole program.
   - So the engineer's `xmod` point and repair 19 are not new to the summary route. And the route may be able to reuse `may_end`'s machinery for less than 205 lines; that is unbuilt.

9. **Q2: the run-time answer's message is weaker at `heroes run`'s default level than what the blind seat was shown.**
   - `param_value` at `-O0` prints "…in paramvalue.go, inside the recursion of paramvalue.go and paramvalue.step".
   - At `-O2` it prints only "stack exhausted in paramvalue.step", and `viamap` likewise loses the `map` frame.
   - All six runs (three shapes, both levels) abort with 134, so the abort itself holds.
   - The f-m reader was given the `-O0` text.

10. **Q2: closing 547 on its measurement is supported by an existing mechanism.**
    - 547 and 541 are both class `improvement`, not `blocking`.
    - `tests/golden/run/fixedbugs-520-a-self-call-through-a-value-with-a-way-out-it-never-takes-aborts.*` pins `!panic: stack exhausted` with a header citing panel 199 R2. 508's witness was replaced the same way once `check` began refusing.
    - So the record would read: 547 ticked, with *The repair* naming the ruling and the measurement (205 lines, +1.3%, 5 shapes, 0 tracked files), plus a `run` witness for each of `param_value`, `viafield` and `viamap`.
    - I found no closed defect worded "closed by ruling" (I searched for withdrawn, not a defect, closed by ruling, ruled that). So whether a defect can close with no change to the compiler is a question for the author, not a precedent.

11. **Q3 is a defect against a ratified ruling. File it `blocking`, area `check`.**
    - Panel 042 (ratified 2026-08-12), line 251: "Residual named: `b @ 1 + 2` still fails, because `Binary` is not a ⇐ form; another ~15 lines. **Total ~30**." That ~30 is what ruling 3 ("full adoption", line 283) costs, so the residual was priced inside the ruling, and it never landed.
    - Measured on both base and v3 (`p/plus.hero`): `y: u8 = 2 + 3`, `show(v: 2 + 3)`, `e: u8 = 2 * (3 + 4)` and the float form `d: f32 = 0.5 * 2.0` are all refused. `1 + b` and `(2)` are accepted.
    - No spec change is needed, so no sitting.
    - Its repair together with v3 turns m2 into the overflow abort that repair 17 predicted.

## Routes nobody built, and questions not asked

- **Q1, the order repair:** try forms that refuse themselves after every form that can settle a type. The order-swapped census pairs, including two-waiting-argument pairs, become the landing's witnesses.
- **Q1:** should § 6 say "`ok(v)` takes its type from the context" in the same change (+8/+9), or should a separate defect be filed against the spec for panel 002's unwritten rule?
- **Q1:** if the 59-line debt is paid by moving `field_of_function_type` and its neighbours, not by a raise, is it still a decision for the author?
- **Q1:** the `int_out_of_range` and `type_mismatch` message changes in 4 goldens, and the 1 owed own test, are edits by hand (CLAUDE.md forbids `UPDATE_GOLDEN`). Who annotates them?
- **Q2:** a summary route built on `may_end`'s existing composition through parameters, priced against the engineer's 205 lines.
- **Q2:** keep the "inside the recursion of" note at `-O2`. This is the critic's "sharpen the run-time message", now with a measured gap.
- **Q2:** one reading per arm cannot meet the engineer's condition. Nobody priced more readings: 4 sessions per arm at about 0.17 USD each, with 2.05 USD left of the 6.
- **Q3:** the neighbouring shapes before filing: unary `-`, `%`, shifts, comparison where neither side has a type, and an `if`/`match` value in an annotated position.

# Panel 201, completeness critic, second pass

Copied by the coordinator at 20:54 on 2026-10-09 (`date`) from the seat's final reply (its transcript), verbatim below the rule; the seat's running notes are in its folder under `.claude/worktrees/scratch-b15/` (ignored by git).

---

I found six things the seats missed, all from commands I ran. The most serious: Q3's message as built is false on a reachable shape. Q3 also costs +5.2% instructions on `check selfhost/main.hero` and grows quadratically on call chains. Q1's built route still refuses two correct shapes, and route I is not 0 spec tokens. All work is in `.claude/worktrees/scratch-b15/201-critic2/` (`notes.txt`, `p/`, `swp/`). I wrote nothing in the frozen tree or the seats' folders.

## Findings

1. **The spec-warden's veto is not a budget veto and not a soundness veto.**
   - The budget is not breached: 349 tokens are free by its own count.
   - The veto rests on `.claude/agents/spec-warden.md:43-44`, which grants a veto when "Principle 0's burden of proof is unmet". CLAUDE.md § 4 says "Seats veto on soundness". The two disagree, and the synthesis has to say which one governs.
   - Route I adds no form (no syntax). Under § 4 it would read as an objection.
   - CLAUDE.md § 12, "Spec beats compiler", weighs on the compiler's side: 2 of 2 readers read the spec as accepting.
   - N1f carries the same Principle 0 burden. Its reader effect is unrun (its own P3). The blind seat's "2 of 2 refused" was measured on the critic's +42-token sentence, which the spec-warden itself shows is false (an annotation and `push` also give the type). So no landing candidate has a measured reader effect.

2. **Route I is not 0 spec tokens.** I ran the spec-warden's own 14 refused probes with a q1 compiler I built myself:
   - q1 now accepts 6: app, map, fold, filter, `helper.ident`, and concrete3 (`g(x: 1, f: ident)`).
   - 8 stay refused: the two `ok(…)` shapes, the result-only call, the literal at `u8`, the concrete `u8` parameter, `concrete2`, ufcs, and `[]`.
   - So after route I, § 9's sentence is still false on 8 shapes, and route I owes a sentence too. The spec-warden already priced it, unknowingly: draft C ("asks one only of a function value") is exactly the critic's route that was built, at +15 / +16 (lower bound).
   - And N1f becomes false under q1, since a generic function's parameter now does type a generic value.
   - The honest comparison is N1f at about +12 against route I plus C at about +16. It is not +12 against 0.

3. **The built Q1 route refuses correct programs that its own description covers, and its message there is false.**
   - `xss.map(len_of)` with `len_of<T>(xs: [T]) -> i64` (`p/o7.hero`) is still `cannot_infer`.
   - `first(a: double, b: ident)` with `first<A>(a: A, b: A)` (`p/o3.hero:13`) is too, although `A` is settled by a non-generic argument.
   - The cause is in `value_argument.hero`'s `meet`: it never replaces a callee letter that is already bound before descending. It only binds when the value's side is a bare letter.
   - Both shapes print "this position asks for none". Under q1 that is false: the position asks.
   - The repair looks like a few lines in `meet` (unbuilt). It cannot go into `walk.hero`, which already sits at 1870 of 1870.

4. **Q1 picks a binding by the order of the parameter type's parts, and it duplicates a message.**
   - `p/o13.hero` (`mix<A, B>(f: (function(B, A) -> A), …)` with `second<X>`, `x: 1`, `y: "s"`) prints the same `type_mismatch: expected i64, found str` twice at one span.
   - `p/o12.hero`, with `(A, B)` order, prints it once.
   - The concrete analog (`p/k13.hero`) prints one message on base, blaming `y:`.
   - The cause: `meet` lets the first part it meets bind the value's letter.
   - Accept or refuse never changed with order in my probes (o1/o2, o12/o13, o3 in both orders, o6 context against arguments). So the compiler-engineer's own objection condition is not triggered. This is a second message for one mistake (`adjacent`).
   - Shapes that held: the nested case `app2(h: app, g: ident, x: 3)` is accepted and prints 3. `app(f: app, x: ident)`, `compose` with no `x`, and an `if`-expression value stay refused, which is correct. `app(f: ident, x: nope)` gives one message, so no cascade.
   - Not covered: a generic name inside an array-literal argument (`apply_all(fs: [ident, double], x: 3)`) stays refused. Rust accepts that shape (the historian's `identity` example).

5. **Q3's headline is a false message on a reachable shape.**
   - In `p/c1.hero`, `pong` calls `ping` on one path and `pang` on the other. In `p/c8.hero`, `ping` calls `pong` or `pang`.
   - Both are refused (correctly), but the message says "every path through `pong` calls `ping`" and never names `pang`.
   - Under verification.md § Bounded discovery, a false message is `blocking`.
   - The seat named this only as an incompleteness of `selfloop_first`. A true headline needs each member's set of targets ("calls `ping` or `pang`").
   - What held: `||` (c2), `for` (c6) and an `if` expression (c9) count as ways out and pass. A known-value cycle (c3), a generic cycle (c4), a UFCS cycle (c5) and two disjoint cycles (c7, two messages) are all refused. I found no correct program refused.

6. **The Q3 `check` cost is resolved, and it is above noise.**
   - The seat compared `heroes-base` built from the seed with plain clang (11,289,144 bytes, the same as my seed build) against `heroes build` binaries (about 15 MB). Instruction counts show the two builds are equivalent (72.68 G each), so that difference is harmless.
   - Instructions retired for `check selfhost/main.hero`, `/usr/bin/time -l`, 3 interleaved runs, all built the same way:

     | compiler | runs (G) | change |
     |---|---|---|
     | base | 72.674 / 72.681 / 72.681 | |
     | q1 | 72.656 / 72.700 / 72.727 | +0.0% |
     | q3 | 76.423 / 76.428 / 76.603 | **+5.2%** |

   - The run-to-run spread is at most 0.25%.
   - **It is quadratic.** A chain of n functions, each returning `f(i+1)(n) + 1`, declared callers first:

     | n | q3 extra instructions | `check` user time |
     |---|---|---|
     | 500 | 3.33 G | |
     | 1000 | 13.27 G | |
     | 2000 | 52.96 G | 0.20 s on base, 4.09 s on q3 |

   - The same 2000 functions declared callees first cost +0.34 G. So the cost depends on declaration order: the `while changed` loop re-walks every member and drops one per pass.
   - This meets the compiler-engineer's own objection condition ("a measured `check` slowdown … clearly above noise").
   - The fix is a worklist that re-walks only the callers of a dropped function. It is unbuilt.

7. **Q2: no measured reason to move. One unasked lead:** `build` of a module alone tells the reader to "add `function main()`, or use `--emit-c`", with the caret on its last token (`geom.hero:2:17`). It never mentions building the file that uses the module. Whether measurement 040's sessions ran `build` and followed that note cannot be checked, because their scratchpad is gone.

## Routes and questions nobody covered

- **Q1, the generalised route:** the q1 mechanism (wait, settle, then check against the substituted parameter) applied to every argument that needs context, not only generic names: `ok(…)`, `[]`, a literal at a narrower width, a result-only call, a concrete parameter. That route alone makes § 9's sentence true as written on all 14 probes, which would be a real 0 tokens, and it matches the author's "most robust, never the cheapest". Unbuilt and uncosted. The spec-warden's N5 already names it as the wording to use if the compiler gives context through a generic function's concrete parameters.
- **Q1:** whether N1f is true for `fs.push(ident)` (accepted) depends on whether a reader counts the built-in `push` as "a generic function". P2 assumes not. Unrun.
- **Q1:** base prints `expected '?', found (function(i64) -> i64)` on `first(a: ident, b: double)` (`p/o3.hero:12`), a placeholder leaking into a message. I did not search `issues/` for this one.
- **Q3:** the worklist fixpoint and the target-set headline, both prerequisites before landing. The seats' two witness designs also differ: the spec-warden's is a helper that may exit; the compiler-engineer's is a syntactic `return` never taken. The synthesis has to pick one.
- **Q3:** whether +5.2% is accepted as a reported price under rank 5. It is not a robustness gain, because R2 already aborts.
- **Q2:** the `no_entry_point` note naming "the file that uses this one" is a diagnostic-text route with no spec cost. Also unasked: one file gets two verdicts. A used module's `main(n: i64) -> i64` is accepted through `use` and refused when checked alone.
- **Unrun for both built routes:** `run`, `determinism`, the full net, and the seed and its fixpoint.

# Panel 201, compiler-engineer

Copied by the coordinator at 20:41 on 2026-10-09 (`date`) from the seat's final reply (its transcript), verbatim below the rule; the seat's running notes are in its folder under `.claude/worktrees/scratch-b15/` (ignored by git).

---

# Panel 201: compiler-engineer's report (the ceiling seat)

I built Q1 and Q3 and ran them on their cases. Q2 needed no build. Nothing was written outside my folder, no run was paid for, and the frozen tree was only read.

- **Folder:** `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/scratch-b15/201-compiler-engineer/`
  - `tree/`: the Q1 route
  - `tree-q3/`: the Q3 route
  - `census/`: an untouched copy, used for the census
  - `cases/`: the test programs
  - `notes.txt`: running notes
- **Compilers:** `heroes-base` (built from the seed at 36be56d0), `heroes-q1b` and `heroes-q3`.
- **Line counts** are in `layout`'s unit: non-blank lines outside `test` blocks. I mirrored the suite's `code_lines` (`tests/harness/suite_layout.hero:955`), and the suite itself passed both copies.

---

## Q1 (488): a generic function value handed to a generic callee

- **verdict:** approve, on one condition: design.md §4.12's "Flat only" sentence is amended in the same landing. No veto, because this adds no core construct.
- **section:** design.md §4.12 (`docs/design.md:1805-1807`), §4.5, §1.7.
- **implementation_cost:**
  - New file `selfhost/check/value_argument.hero`: 185 code lines (223 by `wc -l`), 2 unit tests.
  - `selfhost/check/walk.hero` (`user_call`): +7 lines, from 1863 to **1870, exactly its DECIDED ceiling** (`tests/harness/suite_layout.hero:479`). That leaves no room for the next change.
  - Lexer, parser, IR, monomorphisation, emitter and runtime: 0 lines. The checker records the instance at the name, the same way `check/function_value.hero:173` already does, and the existing passes take it unchanged.
  - Total about +192 lines on 548 modules and 147,561 lines (`wc -l`), about +0.13%.
- **needed_for_self_hosting:** no. The census shows `selfhost/main.hero` checks the same before and after.
- **argument:** "The spec read plainly" is not settled fact. The compiler's narrowing is the ratified §4.12 text from panel 105: *a generic callee's arguments are synthesised, so nothing is bound through another generic's parameter*. The spec's own example (`xs.map(double)`) uses a non-generic value. The route I built is the critic's unlisted one:
  - a generic name waits until the call's other arguments, then its context, have **settled** the callee's letters;
  - it then takes only the settled parts of its parameter's type;
  - it never binds through an unbound letter, so there are no unification variables, Go's nested case stays refused, and a mismatch is reported at the argument.
  - The strongest evidence for it: in `fixedbugs-a-message-never-shows-a-table-index`, the narrowing had hidden the program's real mistake behind two `cannot_infer`. With the route, the real mistake is reported: `` `join` takes `[str]` and `str`, found `[i64]` ``.
- **prediction:** at the landing gate, the census of `check` over the tracked `.hero` files moves exactly 2 files, the two goldens below, and `layout` reads `walk.hero` at 1870 of 1870.
- **condition:** I would change to object if any of these were shown:
  - a program whose result depends on argument order (a value's letter bound through an unsettled callee letter);
  - a third census file moving;
  - the §4.12 sentence left unamended (the compiler would then contradict ratified design).

**Cases** (`heroes run` with q1b; the instances listed are those the emitted C contains):

| shape | base | route |
|---|---|---|
| `app(f: ident, x: 20)` / `x: "hi"` | cannot_infer | 20, hi · `ident<i64>`, `ident<str>` |
| `ns.map(ident)`, `ss.map(ident)` | cannot_infer | 3, b · `map<i64, i64>`, `map<str, str>` |
| `ns.fold(19, second)` | cannot_infer | 3 · `second<i64>` |
| `ns.fold("start", konst<X, Y>)` | cannot_infer | start · `konst<str, i64>` |
| `h: (function(i64) -> i64) = twice(ident)` (context) | cannot_infer | 3 · `twice<i64>` |
| inside `outer<T>`: `app(f: ident, x: x)` | cannot_infer | 7, seven · `ident<i64>` and `ident<str>` via `outer`'s two instances |
| `compose(f: ident, g: ident, x: 3)` | cannot_infer | 3 |
| letter only in the result (`zero<A>(n) -> A?`) | cannot_infer | 9 · `zero<i64>` |
| `ns.map(helper.ident)` (defect 421's qualified form) | cannot_infer | 5 |
| record field `Box(f: ident)`; `return ident` under a declared type | exit 0 | exit 0, unchanged |
| `g = ident` · `h = twice(ident)` · `idv(ident)` (says neither) | cannot_infer | **still cannot_infer** |
| `ns.fold("", second)` (wrong program) | cannot_infer | `type_mismatch: expected i64, found str`, at the argument |
| generic with an `@` parameter | refused | refused once |

**Suites** (q1b):
- `check`: 636 passed, 2 failed. The two failures are the intended moves: `fixedbugs-402…:53` (`xs.map(ident)`) is now accepted, and the table-index golden now reports its real mistake.
- `fixes` 916/0, `permissive` 16/0, `full` 28/0, `emission` 1076/0, `corpus` 55/0, `layout check/` 4/0.
- The compiler's own tests: 1533, all passed.
- Census over 3121 files: exactly those 2 files move.

**Owed at the landing:**
- The 402 golden's header and annotation cite "panel 105's flat rule" and need an appended correction.
- The `cannot_infer` fix text (`function_value.hero:228`, pinned at `:260`) still says "a non-generic function's parameter", which is now narrower than the rule.

**Residual (not fixed):** passing a two-letter generic to a one-parameter function type (`app(f: first, …)`) says `cannot_infer` where `type_mismatch` is the true message. Base says the same.

**Unrun:** `run`, `determinism` and the full net with q1b; the seed and its fixpoint; any token price for a spec sentence (I think none is required, but that is the spec-warden's call).

---

## Q3 (520): a cycle of named functions, and a self-call through a known value

- **verdict:** approve, on condition that the replacement witnesses land with it and the residuals are filed. No veto: this is checker-only.
- **section:** design.md §4.7 and §4.17 (panel 199's R1 read through R4); §4.5 (one mistake, one message).
- **implementation_cost:**
  - New file `selfhost/check/endless_cycle.hero`: 191 code lines (239 by `wc -l`).
  - `self_walk.hero`: 104 → 113 (a `members` field and `among`).
  - `self_call.hero`: 227 → 252 (`known_callee`, the call arm).
  - `endless.hero`: 36 → 55.
  - Total about +244 code lines; 0 in IR, emitter and runtime.
- **needed_for_self_hosting:** no.
- **argument:** The rule finds the largest set of functions in which every path through each one calls a member before any way out, dropping functions until none escapes. The reason it is safe: any function left in that set can never return, whatever the set is. Two more things:
  - **Cycles:** each cycle of two or more functions, found by following each function's first such call, is reported once.
  - **Known values:** an immutable binding to a top-level function (`f = go`) counts as a call to `go`.
- **prediction:** at the landing gate the census moves exactly 3 files:
  - the 457 golden gains 2 lines;
  - the two 508 `run` goldens become refusals.

  Over the next corpus milestone, no program in `examples/` is refused by the cycle rule.
- **condition:** I would change my verdict if either of these appeared:
  - a correct program the rule refuses (one with a way out in any member);
  - a measured `check` slowdown on the compiler that rises clearly above noise.

**The message, as the prototype prints it** (for the blind seat):
```
error[endless_recursion]: `ping` and `pong` call each other for ever: every path through `ping` calls `pong` and every path through `pong` calls `ping`, before it can return, so no call to either returns
  at pingpong.hero:2:12
    |
  2 |     return pong(n)
    |            ^^^^^^^
  note: `pong` calls `ping` at line 5: `ping(n)`
  note: a call to either gives nothing back until the call it makes does, and that one makes another: give one of them a path that returns before its call, or ends on `exit(code:)`, `assert false` or a `while true` with no `break` of its own; an overflow or an index out of bounds stops the program and is no such path
```
A cycle of three prints as `` `a`, `b` and `c` call each other for ever `` with one note per later function. `f = go; f(n)` gets R1's existing message with the caret on `f(n)`.

**Cases** (q3):

| program | result |
|---|---|
| pingpong | refused |
| `f = go` | refused |
| three-function cycle | refused once |
| a caller of the cycle (`start` calling `ping`) | only the cycle is reported |
| `is_even`/`is_odd`; `exit` in one function; `return` on `n == 0` in one function; a base case through a value | **pass** |

- `--permissive` drops the new refusal, since it uses the same thesis code.

**Suites** (q3):
- `check` 637/1: the 457 golden's pinned "left to the run" shapes (lines 256 and 259) are now refused, so its comment needs an appended correction.
- `run 508`: 2 failed, as intended.
- `emission` 1074/4: the two 508 goldens, plus the two new witnesses, which have no blessed emission yet.
- `corpus` 55/0, `layout check/` 4/0, the compiler's own tests 1532, all passed.

**Replacement witnesses**, written in `tree-q3/tests/golden/run/fixedbugs-520-*`: a cycle and a value-call that each have a syntactic way out they never take. The `run` form passed both, 2/0, on all four of its runs (`-O0`, `-O2`, `--sanitize`, `--sanitize -O0`, per `suite_run.hero:8-35`). Each aborts 134: `panic: stack exhausted … inside the recursion of ping and pong`.

**Cost on `check` of the compiler** (user seconds, 5 runs each, load average 7 to 39):

| compiler | min | median |
|---|---|---|
| base | 6.15 | 6.35 |
| q1b | 5.64 | 6.85 |
| q3 | 5.88 | 7.33 |

This is **unresolved within noise.** The structural cost is one more collecting walk of every function not already refused alone, on every program. The census wall time, run 6 at a time on a loaded machine, read 41 s base, 43 s Q1 and 45 s Q3; those are rough figures.

**Residuals, sound but incomplete:**
- `cases/q3/selfloop_first.hero` (f calls f or g, g calls f) passes. Following first calls lands on a one-function loop. A true message would have to name each function's set of targets.
- A self-call through a parameter (`go(n: n, f: step)` inside `step`) passes.
- R1's message does not yet say that `f` holds `go`.

**Unrun:** the full net; the seed and its fixpoint; clang's `-Winfinite-recursion` interplay (I did not re-check R3).

---

## Q2 (467): a `main` in a module that is used

- **verdict:** approve the sentence as it stands, with no spec change and no compiler change.
- **section:** design.md §4.1 ("Entry point: `function main()`"), §1.6, and Principle 0.
- **implementation_cost:** 0 for the recommended route. The critic's route (a warning) is a reading, unbuilt:
  - `selfhost/check/decls.hero:89` already computes `is_main` with `source.is_root`;
  - a warning there would be about 4 lines, plus about 12 for a builder in `selfhost/value_errors.hero` (next to `main_returns` at `:165` and `main_parameters` at `:180`), plus a golden pair.
- **needed_for_self_hosting:** no.
- **What I ran** (`cases/q2/a`):
  - `check` of a program whose used module has a `main`: exit 0. `check` and `run` of the module alone: exit 0, and it prints its own result (8). `build` of the program runs the program's own `main`.
  - `m.main()` called from the program is accepted and runs.
  - A used module's `main(n: i64) -> i64` is accepted through `use`, but `check` of that file alone refuses it with `main_parameters` and `main_returns`. One file gets two verdicts, because `decls.hero:89` applies the `main` rules only to the file being compiled.
  - A file with no `main`: `check` exits 0; `build` and `run` exit 1 with `no_entry_point`, whose caret sits on the file's last token (`2:17`).
- **argument:** The extra `main` is dead code unless something calls it. It is exactly what makes `heroes run m.hero` work, which is a plausible reading of measurement 040's task ("make it compile"). The evidence is confounded and shows no mistake with a consequence, so a reworded sentence or a warning buys nothing measured.
- **prediction:** in a blind measurement on module files whose task does not ask for compiling, at most 1 session in 8 or more adds a `main`.
- **condition:** a measured session where a module's `main` was written on the belief that it runs when the module is used.
- **Unrun:** the blind sessions (the coordinator runs those).

**Adjacent, for the coordinator to file if wanted:** `needs_parameter_names` on a declaration's `(function(A, A) -> ())` prints `a type parameter` instead of `A` (`cases/q1/marked.hero`).

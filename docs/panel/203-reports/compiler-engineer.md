# Panel 203, compiler-engineer

Copied by the coordinator at 00:00 on 2026-10-10 (`date`) from the seat's final reply (its transcript), verbatim below the rule; the seat's running notes are in its folder under `.claude/worktrees/scratch-b15/` (ignored by git).

---

I resumed at 23:46 and the report is in by 00:00. Q1: I built the general route in three versions; the recommended one (v3) accepts all fourteen probes, `literal2` and `m1`/`m3`. It costs about 305 code lines, all inside the type checker. Q2: I built the per-function summary route; it refuses `param_value.hero` and leaves the three correct shapes alone, but I object to it on cost and partial coverage.

**How I built the compilers.** The base compiler was built in three stages from a clean copy (`base-tree/`), as the brief asked. Every compiler measured here was built by that stage-2 compiler, `heroes1b`. The exception is `heroes-q1-v1`, built by an earlier `heroes1` from my first build. I edited `walk.hero` at 23:11:19 while that build was running (it started 23:07:32 and ended 23:11:41), so I killed its stage 3. All runs use the copy's runtime (ABI 30) through `HEROES_RUNTIME`. Nothing was written in the frozen tree, the trunk, a lane or another seat's folder.

## Q1, defect 541: the general route

**What I built.** A new module, `tree/selfhost/check/waiting.hero` (288 lines, 244 code lines). Plus `settle_waiting` and `synth_argument` in `tree/selfhost/check/walk.hero` (81 lines added, 10 removed). An argument that has no type of its own waits while the other arguments settle the callee's type parameters. Such an argument is a number literal, a generic function's name, `ok`/`fail`, a generic call, `[]`/`{}`, `.case`, or an array or map holding one of these. A waiting argument is then checked against its parameter with the settled types filled in. When nothing settles it, the fallback order is:
1. the type the context asks for;
2. a generic call, synthesised;
3. a literal at its default `i64`/`f64`;
4. the other forms, synthesised (that synthesis is their own refusal);
5. a generic function's name, settled from whatever is bound so far;
6. anything left is refused.

**Three versions.** The spec-warden tested v1 and v2:
- **v1** (`heroes-q1-v1`) settles a generic function's name before falling back to a literal's default.
- **v2** (`heroes-q1`) falls back to the literal first.
- **v3** (`heroes-q1c`) is v2, but a generic call goes before a literal and an array literal goes after it.
- **q1sc** is v3 with literals never waiting. It is the "a literal settles at once" side of repair 17.

**Probes** (`q1c-probes.txt`, `pc-*.txt`):

| What | v3 result |
|---|---|
| The fourteen | 14 of 14 accepted and run: app 20, map 3, fold 19, filter 3, qualified 3, nestedok 1, nestedok2 5, resultonly 1, literal 3, concrete 1, concrete2 1, concrete3 1, ufcs 7, emptyarr 1 |
| `literal2` | Accepted, prints 255 |
| `nestedcall` | Still refused, one `cannot_infer` |
| `m1` | Accepted, prints 1 |
| `m3` | Accepted, prints 255 |
| `m2` | Still refused: the operands of `+` get no context, as the spec-warden found |
| f1 | Accepted, prints 6 |
| f3 | Refused `type_mismatch` (v1 accepts it, prints 0) |
| `r1` | Refused, on every version |
| `o3`, `o4` | Accepted |
| `o8` | Accepted only on v3 (refused on v1 and v2) |
| `o12`, `o13` | Refused, the message now pointing at `second` |
| `xss.map(len_of)` | Its call to `map` now checks; what remains is `print` refusing a `[i64]` |

**Order of arguments.** I rewrote seven probes with their parameters in the opposite order, plus a mixed-literal pair. On v3 the verdict is the same in both orders for all eight pairs (app, map, emptyarr, nestedok2/ok_rev, o8/apply_rev, f3/fold_rev, literal/literal2, mixlit). The message's position is not order-free: `first(a: 1, b: 2.5)` is told at `2.5`, and the swapped call at `1`.

**Each argument typed once.** True by construction from reading the code (one check, one synthesis, or one settling per argument). I did not instrument it.

**One gap, measured.** `apply_all(fs: [ident, half], x: 200)`, with `half(x: u8) -> u8`, is refused on v3: the literal picks `i64` before the array's `half` can say `u8`. Spec-warden's sentence G3a would predict this accepted, so on v3 G3a is false on this shape.

**Census.** `check --brief` over all 3,163 tracked `.hero` files. Base: 1,610 accepted, 1,553 refused. v2, v3 and q1sc all move the same 10 files, every one toward acceptance or a truer message:
- 6 files under `docs/panel/201-briefs/blind/r1-*` go from refused to accepted;
- golden 402 loses one `cannot_infer`;
- golden 544 goes from 7 `cannot_infer` to 1 `type_mismatch`, and that one is a real mistake in the program: `fail(code: 1, …)`;
- golden 546 loses both `cannot_infer`;
- the *a-message-never-shows-a-table-index* golden now gets a `bad_operand` on `join` over `[i64]`, the program's actual mistake.

No file goes from accepted to refused, and no check exits 2.

**The compiler's own tests.** 1,542 run, 1 fails, and it is expected: a test in `walk.hero` near line 2305 (`n: str? = same(1)`). The context now settles the type before the literal does, so the program's one diagnostic moves from the call to the literal.

**Cost** (`cost.txt`). `check selfhost/main.hero`, three interleaved runs, instructions retired. Base: 76.11, 76.13, 76.26 billion. v3: 76.19, 76.19, 76.20 billion. That is +0.08%, inside the base's own spread of 0.2%.

**Verdict on Q1**
- `verdict`: approve, v3
- `section`: design.md §1.7 and §4.12
- `implementation_cost`:
  - ~305 code lines, all in `selfhost/check/`: `waiting.hero` +244, `walk.hero` 1,868 → 1,929 code lines, which is 59 over its decided ceiling of 1,870 (`tests/harness/suite_layout.hero:479`).
  - The new code in `walk.hero` cannot move out: it calls `check` and `synth`, and those live in that file.
  - Nothing changes in the lexer, parser, IR, type descriptors or emitter. Monomorphisation reads the instances already recorded.
  - Also owed: the message text in `generic_argument.hero` and `function_value.hero` that quotes N1f, 4 golden `.expected` files with their annotations, and 1 test.
- `needed_for_self_hosting`: no
- `argument`: It is not a new construct. It changes the order in which the checker reads a call's arguments, and nothing after the checker learns anything new. It accepts 25 probes that are refused today. It refuses no tracked file it accepted before, and it costs nothing measurable in instructions. N1f becomes false under it, and so does "Flat only" in §4.12, which panel 105 ratified. The real price is the ceiling: 59 lines in a file nobody can split here, which needs a raised decided number or a new seam.
- `prediction`: if v3 lands, the landing census moves exactly these 10 files, and `check selfhost/main.hero` stays within +0.5% of base in instructions retired.
- `condition`: if `walk.hero` cannot take +59 lines through a ratified raise or a seam, or if `apply_u8` must be accepted for the spec sentence to hold, my verdict becomes an objection.

## Q2, defect 547: a self-call through a parameter

**What I built.** A per-function summary, the route the critic listed (`tree-q2/`):
- a new module, `check/param_calls.hero` (122 lines, 102 code lines);
- in `self_call.hero`: `handed`, `handed_arguments`, `param_position`, `forget`;
- two new fields in `self_walk.hero`, and the summary threaded through `endless.hero` and `endless_cycle.hero`.

A summary records, for each function and each parameter, whether every path through the function calls that parameter before any way out. The summaries are recomputed until none changes, starting from "no", so a "yes" is always proven. A call that hands a function to such a parameter then counts as a call to the function handed.

**Probes** (`q2b-probes.txt`):

| Refused (newly) | Kept (correct programs, run to exit 0) | Not seen (still abort 134 at run time) |
|---|---|---|
| `param_value` | `wayout` | `viafield` (a record field) |
| `chain` (`go` hands `f` to `h`) | `wayout2` | `viamap` (`map`'s `for` may run no turn, so it has a way out) |
| `pair` (a cycle through a parameter) | `exitfirst` | a local `g = step`, then handed |
| `ufcs` | | the helper shapes |
| `xmod`: `m.go` lives in another module, so the refusal at `step` depends on another file's body | | |

**Census and tests.** The census is identical to base on all 3,163 files: nothing gains or loses a refusal. The compiler's own tests: 1,542, all pass. Panel 201's cycle shapes c1 to c9 are unchanged.

**Cost.** 77.09, 77.10, 77.15 billion instructions against base 76.11 to 76.26: +1.3%, outside the spread.

**The message `param_value.hero` gets** (exit 1), for the blind seat:
```
error[endless_recursion]: every path through `step` calls `step` again before it returns, so no call to it can return
  at param_value.hero:5:12
    |
  5 |     return go(n: n, f: step)
    |            ^^^^^^^^^^^^^^^^^
  note: `go` calls its parameter `f` on every path before it returns, and this call hands it `step`, so the call is `step` calling itself; a path in `go` that returns before it calls `f` is a way out too
  note: a call to `step` gives nothing back until the call it makes does, and that one makes another: give `step` a path that returns before the call, or ends on `exit(code:)`, `assert false` or a `while true` with no `break` of its own; an overflow or an index out of bounds stops the program and is no such path
```
The first note is printed only at a plain call. `n.go(step)` and `m.go(…)` get the bare first line, which shows them no self-call at all: that gap is unbuilt and would cost about 15 more lines.

**Verdict on Q2**
- `verdict`: object
- `section`: design.md §1.1 (simplicity sets the ceiling); §4.7 and the 199/201 rulings for the run-time abort. The document does not cover a check that closes only part of a class.
- `implementation_cost`:
  - ~205 code lines, checker only: `param_calls.hero` +102; `self_call.hero` 259 → 336 code lines, past §11's 300, so a split is owed; `self_walk.hero` +8; `endless.hero` +16; `endless_cycle.hero` +2.
  - +1.3% instructions on every `check`.
- `needed_for_self_hosting`: no
- `argument`: It refuses 5 shapes and none of the 3,163 tracked files. It misses the record field, `map`, and a local alias, so the run-time abort stays necessary for the same class anyway. It makes a refusal depend on another module's body (`xmod`). It costs 205 lines and a measurable 1.3% on every check. The abort already stops the program cleanly with exit 134, so robustness is not at stake. No veto: it adds no construct and breaches no ceiling.
- `prediction`: if it lands, the landing census shows 0 changed files and `check selfhost/main.hero` rises at least 1% in instructions retired.
- `condition`: if the blind seat's Q2 repair arm shows readers fixing `go`, or producing this shape, measurably more often under the message than under the abort, I move to approve.

## What I could not run
- The harness forms: `check`, `run`, `emission`, `fixes`, `full`, `permissive`.
- The `layout` suite itself; I counted with a mirror of its rule.
- The seed regeneration and fixpoint, `-O2` and the other platforms.
- The critic's narrower route as its own build. q1sc keeps literals at their default and still refuses `concrete` and `ufcs`.
- The first note at method call sites (`n.go(step)`, `m.go(…)`).

Everything is in `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/scratch-b15/203-compiler-engineer/`:
- `notes.txt`
- `cost.txt`
- `census_base.txt`, `census_q1.txt`, `census_q1c.txt`, `census_q1s.txt`, `census_q2.txt`, `census_q2b.txt`
- `q1c-probes.txt`, `pc-*.txt`, `q2b-probes.txt`
- `selftests-q1c-full.txt`
- `tree/selfhost/check/waiting.hero`, `tree/selfhost/check/walk.hero`
- `tree-q2/selfhost/check/param_calls.hero`, `tree-q2/selfhost/check/self_call.hero`

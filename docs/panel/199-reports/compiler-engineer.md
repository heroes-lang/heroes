# Panel 199, compiler-engineer

Copied by the coordinator on 2026-10-09 from the seat's reply, verbatim apart
from this header: a subagent's Write of a report file is refused, so the
seat's running notes are `.claude/worktrees/scratch-b15/199-compiler-engineer/notes.txt`
(ignored by git) and its reply is the record. The seat ran twice: from about
08:09 to 08:25 on 2026-10-08 in a copy under the session scratchpad, which a
reboot at about 11:18 emptied with its prototype; resumed at 23:0x, it rebuilt
the prototype from its own transcript in the repository's ignored
`.claude/worktrees/scratch-b15/199-compiler-engineer/` (the author's rule of
that day: no file outside the repository's root), and finished there. Its
three B transcripts, which the blind seat read, are byte-identical to the
rebuilt prototype's output (`cmp`, its item 6).

---

I'm objecting to route (A) as it's written, and standing on the same rule under a third reading (R3), plus routes (F) and (G). No veto: the rule adds no core construct and stays well under the ceiling.

### Verdict and why

**`verdict`: object.** Neither reading of question 2 is safe:
- **Reading R1** (only `exit(code:)`, `assert false` and a `while true` with no `break` count as ways out) refuses three correct programs. `calleeexit`, `callback` and `fieldcb` print 0 1 2 and exit 0 at `-O0` and `-O2`; they stop because a callee, a callback or a field's function calls `exit`. A correct program refused is `blocking`.
- **Reading R2** (an abort nobody wrote, such as an overflow, also ends a path) lets p2, `so_lease` and `q5_overflow` through, and its list of aborting operations never closes.

**The route I stand on (R3):** R1, plus "a call to any other function or function value is a way out, since it may `exit`". It keeps every refusal and drops all three false ones. Built, cased and tested below.

**`section`:** design.md §1.1 (simplicity sets the ceiling), §1.7 and Part 5 (core versus sugar), §4.7 and spec `:238-240` (what ends a path), §4.17 (the diagnostic), and `design.md:3725` (no warning level, so (C) and (K) must be errors). design.md has no section on a diagnostic for a function that never returns.

**`needed_for_self_hosting`:** no. The prototype refuses nothing in the compiler (136,290 lines, 500 modules) or the harness.

**`argument`:** Under R1 the rule refused three correct programs, which is `blocking`. Under R2 it misses p2, the blind seat's discriminating task, and needs an open list of aborts. (B) answers question 2 by construction only in the shared brief: the IR holds `sub!` and `index!` as single instructions with no edge, and `exit` as a plain call, so (B) needs the same `exit` special case and `check` would not see it. R3 keeps every refusal and drops the false ones. It adds no core construct and no line to lowering, descriptors, ownership or the emitter: about 335 checker lines.

### `implementation_cost`: route (A) under R3, all measured

**Lines** (in the `layout` suite's own count; lexer, parser, resolver, IR, emitter and runtime all 0):
- `selfhost/check/self_call.hero`: new, 299 code lines, under the 300 ceiling.
- `selfhost/flow_errors.hero`: 240 to 261. The diagnostic sits beside `missing_return`.
- `selfhost/checker.hero`: 146 to 160 code lines (+33 raw, two test blocks included).
- `selfhost/diag.hero`: +1, the code joins the thesis-rule list.
- `selfhost/ir/mono.hero`: one test program given a base case. Its bare `grow([x])` is now refused by `check` first.
- `tests/harness/suite_golden.hero`: the `permissive` floor 8 to 11, which the suite itself asked for.
- About 335 code lines in total, 0.25% of the compiler.

**One reading of §4.11:** the rule asks `walk.field_of_function_type` and `walk.ufcs_hides_the_marker`, the checker's own predicates, rather than a second copy of them.

**Cost of `check`** (instructions retired, medians of 3, same input for both compilers, load under 6):

| input | frozen | prototype | change |
|---|---|---|---|
| p1 | 72.33M | 72.70M | +0.5% |
| harness root (32k lines) | 13,547M | 13,709M | +1.2% |
| compiler root (136k lines) | 67,384M | 67,938M | +0.8% |

Building the prototype took 104.59 s of user time.

**Gates:**

| run | result |
|---|---|
| the compiler's own tests | 1,432, all passed |
| `check` whole | 613 / 0 |
| `run` whole | 384 / 0 |
| `emission` | 1,002 / 0, every blessed emission byte-identical |
| `determinism` | 422 / 0 |
| `corpus` | 55 / 0 |
| `full` | 22 / 0 |
| `layout` | 5 / 0 |
| `permissive` (after the floor) | 12 / 0 |
| `annotations` (narrowed to my cases) | 2 / 0 |
| `fixes` (narrowed to my cases) | 1 / 0 |
| the net's own tests | 308 / 1 |

The one failure among the net's own tests (`suite_records.hero:6104`) gives the identical result with the frozen compiler on the same copy. It belongs to the copy, which has no `.git`, not to the change.

**Census:** `check` and `build --emit-c` on both compilers over all 2,980 tracked files, none unread.
- 2 verdicts moved, both 0 to 1: `docs/panel/173-briefs/q5_overflow.hero` and `so_lease.hero`.
- The emitted C moved in 0 of the 1,151 files that compile, and the same two files are newly refused.
- Neither moved file is a correct program: both are panel 173's deliberate stack-exhaustion probes. `q5_overflow` runs to `panic: stack exhausted in q5overflow.down` (exit 134) at both levels. `so_lease` doesn't build on this Mac (`giveaway.h` is missing).

### The other routes' costs
- **(A) under R1:** about 10 fewer lines; refuses the three correct programs above.
- **(A) under R2:** about 35 more lines for an abort list that is never complete; misses p2, `so_lease` and `q5_overflow`.
- **(B) on the IR:** unbuilt. `check` stays exit 0 while `build` refuses, as with `polymorphic_recursion`. My estimate is 120 to 150 lines plus a stage in `cli/compile.hero`.
- **(C) and (K):** clang 21.0.0 and 22.1.8 agree. The warning fires on `forever`, p1, p3, `gen`, and falsely on `serve`; it is silent on p2, `pingpong` and the function value. (K) is one line in `cli/flags.hero`, but the text is clang's and its exit code is unrun.
- **(F):** `_Noreturn` alone on `exit` trades `serve`'s warning for `-Winvalid-noreturn`. With the body also ending in `hero_unreachable()`, `serve` builds with no warning on both clangs. About 10 lines at the one site in `emit/signature.hero` (estimated), and it moves 17 of 483 blessed emissions plus the seed.
- **(G):** one flag. With it, `forever` and p3 abort 134 at `-O2` instead of running for ever, on both clangs. The cost: a correct tail recursion 10,000,000 deep prints `50000005000000` at `-O2` today and would abort 134. It already aborts at `-O0` today, so `build` and `run` disagree on it now; 100,000 deep still passes.
- **(H), (I), (M):** unbuilt. (J), the status quo, costs 0 lines, but at `-O2` `forever`, p3, `gen`, `pingpong`, the function value, `mixed` and `yes` all hang until `timeout` stops them (exit 124).

### Answers to the brief's items
- **Where the rule lives:** in `check`. `polymorphic_recursion` only lives after `check` because it needs instantiation; this rule doesn't.
- **Hidden aborts:** they are not ways out. Calls to other functions are, under R3.
- **Shapes:** 13 annotated refusals in `tests/golden/check/fixedbugs-457-a-function-whose-every-path-calls-itself-is-refused.hero`, marks equal to the lines `check` tells.
- **Misses:** R3 still lets through `mixed` (`climbs(helper(n))`, which hangs at `-O2`), mutual recursion, and a self-call through a function value.
- **Class:** a thesis rule, dropped by `check --permissive` and pinned by the `permissive` case. No fix, neither `certain` nor `guess`.

### Item 6: what the blind seat read
The rebuilt prototype prints p1, p2 and p3 byte-identical (`cmp`) to `blind/p1-B.txt`, `p2-B.txt` and `p3-B.txt`; p2 is refused. The frozen compiler's transcript of p2 is byte-identical to `p2-B-second-reading.txt`, so reading R2 changes nothing for p2.

### `prediction`
At the gate of the batch that lands this route, before the next milestone tag, three things hold, and any one false refutes me:
- The census moves only the two `173-briefs` probes plus the new cases, and none of the moved programs runs to exit 0.
- `self_call.hero` stays at or under 300 code lines by `layout`'s count.
- `check` on `tests/harness/main.hero` costs at most 2% more instructions than the trunk's compiler.

### `condition`
- A program that runs to exit 0 and that R3 refuses would move me to (H) or to a narrower rule.
- A measured rate at which models write the shapes R3 misses would raise (I).
- A correct program in `examples/` or on the closure list that relies on `-O2` tail calls deeper than 100,000 would make me prefer the spec naming the level over (G).

### Found beside the work
- The shared brief's premise that (B) answers question 2 by construction is false for this IR: the dump of p2 shows `sub!` as one instruction, with no branch to an abort.
- A deep tail recursion behaves differently under `build` (aborts) and `run` (prints), so spec `:280` is true at `-O0` and false at `-O2`. That is defect 508's other face.
- `mixed` dies with `panic: stack exhausted in mixed.helper`: the message names the frame that ran out, not the recursion. I read it as `adjacent` (a true message less exact than it could be), route (M)'s territory.

### What I could not run
- Clang 18.1.3 and 23.1.1, and Linux and Windows. The Ubuntu container on this Mac belongs to another session, so I left it alone.
- Routes (B), (C), (H), (I) and (M) were not built. (F), (G) and (K) were judged by hand on the emitted C, not through `heroes build`.
- The full net, the seed and its fixpoint.
- A census of R1 and R2 in this session: those two were run only in the first copy, both moving the same 2 files.
- What `yes` does at `-O2` after 10 seconds: it had printed 12.9 MB when `timeout` stopped it (exit 124), and I deleted that output from my scratch.
- `report.md` was refused for a subagent, so the running notes are in `notes.txt`. No paid run, and nothing of mine is still running.

Everything is in `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/scratch-b15/199-compiler-engineer/`:
- `notes.txt`
- `B-p1.txt`, `B-p2.txt`, `B-p3.txt`, `frozen-p2.txt`
- `census/check-moved.txt`, `census/emit-moved.txt`, `census/census.log`
- `form-*.txt`, `probes.sh`
- `tree/selfhost/check/self_call.hero`
- `tree/tests/golden/check/fixedbugs-457-a-function-whose-every-path-calls-itself-is-refused.hero` and `.expected`
- `tree/tests/golden/permissive/fixedbugs-457-the-control-arm-drops-the-endless-recursion-and-keeps-a-type-error.hero` and `.expected`

---

## Second reply, 2026-10-09: route R4 built beside R3

Copied by the coordinator from the seat's second reply, verbatim. Asked at
00:3x on 2026-10-09, after the completeness critic's second pass found that R3
lets plausible endless functions through and named an unlisted route, R4, that
nobody had built.

Between R3 and R4, I recommend R4. It refuses all four endless programs the critic found that R3 lets through. It refuses no correct program that R3 doesn't. Its census is identical to R3's. It costs about 219 more lines and about 0.15 points more `check` time. No veto: neither reading adds a core construct or reaches the ceiling.

### R4 beside R3

Both compilers were built and run in my copy under the repository's root. The machine was quiet (load under 6), and costs are medians of 3 runs on the same input as the frozen compiler.

| | R3 | R4 |
|---|---|---|
| Code lines, `layout`'s count | 335 | 554 |
| `check` instructions, p1 | +0.8% | +0.65% (both inside p1's noise, ±0.4%) |
| `check`, harness root (13,529M frozen) | 13,690M, +1.19% | 13,710M, +1.34% |
| `check`, compiler root (67,352M frozen) | 67,890M, +0.83% | 68,006M, +0.97% |
| Compiler's own tests | 1,432, all passed | 1,432, all passed |
| `check` / `run` | 613/0 / 384/0 | 613/0 / 384/0 |
| `emission` / `determinism` / `corpus` | 1,002/0 / 422/0 / 55/0 | 1,002/0 / 422/0 / 55/0 |
| `full` / `layout` / `permissive` | 22/0 / 5/0 / 12/0 | 22/0 / 5/0 / 12/0 |
| `annotations` / `fixes`, my cases | green | green |
| Census, 2,980 files, `check` and `--emit-c` | 2 verdicts moved | the same 2 moved |
| Emitted C moved | 0 of 1,151 | 0 of 1,151 |
| `filtercount`, `mapsum`, `factpred`, `mixed` | all four pass `check` | all four refused |
| `calleeexit`, `callback`, `fieldcb`, `serve`, `hidden2` | pass | pass |
| `deadfn` | refused | refused |
| p1, p2, p3 output | identical to the blind seat's B files | identical (`cmp`) |

- **Lines, R3:** `self_call.hero` 299, `flow_errors.hero` +21, `checker.hero` +14, `diag.hero` +1.
- **Lines, R4:** `self_call.hero` 289, a new `check/may_end.hero` 175, a new `check/endless.hero` 36, `flow_errors.hero` +39, `checker.hero` +14, `diag.hero` +1. Every file is at or under 300.
- **Run one `run` case is skipped** under both readings: it needs Linux's `sys/prctl.h`.
- **The two census moves** are `docs/panel/173-briefs/q5_overflow.hero` and `so_lease.hero`, both 0 to 1, deliberate stack-exhaustion probes. `q5_overflow` aborts 134 at both levels on the frozen compiler; `so_lease` doesn't build here (`giveaway.h` is missing). No moved program runs to exit 0, under either reading.
- **The four programs R3 misses** fail on the frozen compiler: `filtercount`, `mapsum` and `factpred` abort 134 at both levels, and `mixed` aborts 134 at `-O0` and hangs at `-O2` until `timeout` stops it (exit 124).
- **The five correct programs** each exit 0 at both levels.

### How R4 decides
A call before the self-call is a way out only if the callee may end the program. "May end" means the callee contains an `extern` call, `assert false` or any `while true`, calls through a field, an expression or a function value that is not its own parameter, or names a function that may end. A call through its own parameter is judged at each call site by the function handed in, which is how `map`, `filter` and `fold` are followed. The answer is a least fixpoint over the program.

Two things to know about it:
- **It errs towards sparing.** Breaks inside a `while true` are not looked for, and every `extern` counts as possibly ending.
- **The table is built only when needed:** for a function that would otherwise be refused and that names some function on a path before its self-call. For the compiler's own source it is never built.

Two fixes came out of building it:
- **A quadratic copy.** My first R4 grew a list through a record field for every call in every function, and spec § 10 says that copies the whole list at each push. The first walk now keeps only a yes/no, and a second walk collects the list only for a function the first would refuse.
- **The module split.** The driver now lives in `check/endless.hero`, cut from the walk the way `moved.hero` is cut from `moved_walk.hero`, so every file fits the ceiling.

What R4 still lets through: mutual recursion, a self-call through a function value, and a self-call preceded by a call that may end, such as any `extern`, any `while true` or a helper that can `exit`.

### `deadfn`
R3 and R4 both refuse `never`, a function nobody calls that only calls itself; the program prints 1 and exits 0. I agree with the critic that this is right per function, so I've worded my condition per function.

### Verdict and the rest of the record
- **`verdict`:** object to (A) as written under R1 (it refuses three correct programs) and R2 (it misses p2). Between R3 and R4, R4. (F) and (G) unchanged from my last reply.
- **`prediction`:** at the gate of the batch that lands (A) under R4, before the next milestone tag:
  - the census moves only the two `173-briefs` probes plus the new cases;
  - no moved function, if called, returns or ends the program;
  - every new or changed module is at or under 300 code lines by `layout`'s count;
  - `check` on `tests/harness/main.hero` costs at most +2% instructions against the trunk's compiler.

  Any one false refutes me.
- **`condition`:** a function that, when called, returns a value or ends the program, and that R4 refuses, would change my verdict; a function nobody calls is judged as if it were called. So would a measured rate of models writing what R4 misses, which would raise route (I).

**Could not run:** clang 18.1.3 and 23.1.1, Linux and Windows, the full net, and the seed with its fixpoint.

Everything is in `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/scratch-b15/199-compiler-engineer/`:
- `notes.txt`
- `r4src/` (the five modules as they would land)
- `self_call.R3.hero`, `heroes-r3`, `tree/heroes-r4`
- `census/` (R3) and `census4/` (R4)
- `r4-owntests.txt`, `r4-form-*.txt`
- `B4-p1.txt`, `B4-p2.txt`, `B4-p3.txt`
- `probes2.sh`
- `tree/tests/golden/check/fixedbugs-457-a-function-whose-every-path-calls-itself-is-refused.hero` and `.expected`: 17 marks for R4; red under R3, which misses four of them.

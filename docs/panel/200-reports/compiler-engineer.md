# Panel 200, compiler-engineer

Copied by the coordinator at 20:37 on 2026-10-09 (`date`) from the seat's final reply (its transcript), verbatim below the rule; the seat's running notes are in its folder under `.claude/worktrees/scratch-b15/` (ignored by git).

---

# Panel 200: compiler-engineer's report

Clock: 19:55:29 to 20:36:09 by `date`. Work was done only in `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/scratch-b15/200-compiler-engineer/`; running notes are in `notes.txt` there. The copy is of `36be56d0`; my first compiler was built from the seed. Load ran between 3 and 88 during the session, so only counts are reported, never durations.

Instructions come from `/usr/bin/time -l` and memory is "peak memory footprint" (the shared brief named maximum resident set; footprint is what `q3/m-*.txt` record, with the resident set beside it). clang is run with `-fintegrated-cc1`, because without it the count covers only clang's driver: 0.127e9 both for a 10 KB file and for the 46 MB seed. Four compilers were built in the copy:
- **heroes-b**: route B.
- **heroes-c**: B + C.
- **heroes-d**: B + C + D.

The baseline is the seed build.

**Breach of my own discipline (CL-025):** I edited `runtime/` while the `emission` suite was running on heroes-b. For one window the header said ABI 30, and that caused 5 of the 6 failures in that run (below).

---

## Q1 (453): `--emit-c` writes C that no clang run reads

- **verdict:** **approve route B** (the critic's "compile the whole-program rendering as the one unit"). **Object** to route A (a `-fsyntax-only` pass before writing). No veto.
- **section:** design.md §3.1 (`:715` determinism, `:734` "stop at the C and read it"); §1.12; `.claude/rules/cli-surface.md` exit-code contract.
- **implementation_cost:**
  - **Route B, built:** `selfhost/cli/assemble.hero` 324 → 329 lines. One code line changes, `if site.is_tests` becomes `if site.is_tests || !site.link`, so `round` compiles the fused unit for `--emit-c`; the other lines are comment. `layout` passes it.
  - Owed at landing:
    - rewrite the comment at `artifact.hero:102-117` (it becomes true: "the bytes written are the bytes clang read");
    - amend `fused`'s doc comment.
  - **Route A, measured by hand only** (build's words at `-O0 -g`):
    - small program (`run/arithmetic.hero`): 0.204e9 instructions, 28.5 MB, against 0.653e9 for its warm `--emit-c`. That is **+31%** on every run, uncached.
    - the compiler (46,097,621-byte artifact): 26.64e9 instructions, 379 MB, against 442.56e9 warm. That is **+6.0%**.
- **needed_for_self_hosting:** no.
- **argument:**
  - Route B removes the second pass instead of adding one, and it is cheaper:
    - The artifact of `selfhost/main.hero` is byte-identical to today's route on the same source (`cmp`).
    - Warm `--emit-c` of the compiler drops from **442.29e9 to 254.88e9** instructions; a small program goes from 657.1M to 650.2M.
    - Cold, the compiler's fused compile peaks at 1.30 GB of resident set, the same unit `heroes test selfhost/main.hero` already compiles.
  - What clang refuses goes through `whose.of_refusal`, the existing classes:
    - a header clang refuses on its own is told at exit 1;
    - an extern that disagrees with its header is told at exit 1;
    - anything else is `internal error: compiling the generated C failed:` with clang's words, then `error: clang refused the generated C`, exit 2.
  - The contract allows exit 1 here.
- **cases:**
  - `emission` with heroes-b read 1064 passed and 6 failed: 5 from my ABI-30 window and 1 `fixedbugs-218` timeout (exit 124, load near 70). I re-ran those 6 by hand (`--emit-c` then `cmp` against `tests/emission/`): all 6 exit 0 and byte-identical. That was a hand diff, not the gate's command.
  - **Red-before-repair case:** the ffi-pragmatist's two-header program (`q1/fp/`: `a.h` and `b.h`, each defining `static inline twice` with a different type):
    - `heroes run`: prints 6 and 8, exit 0.
    - Old `--emit-c`: exit 0, and the C it writes is refused by clang `-fsyntax-only`.
    - Route B: exit 1, `error[ffi_header_refused]` on `b.h`.
    - Old `heroes test`: already exit 1 with the same message.
  - **Finding beside it:** that message says `b.h` "does not compile", but `b.h` compiles alone; the conflict is between the two headers in one unit. This is a pre-existing false message in `heroes test`, which route B now shares. It needs filing, probably `blocking` (a false message).
- **prediction:** at landing, `emission` reads 0 failed; the seed artifact is unchanged by `cmp`; warm `--emit-c` of `selfhost/main.hero` is ≤ 260e9 instructions on this Mac.
- **condition:** a program that `heroes build` compiles module by module and that route B refuses, which is not the two-headers class above. Or a platform whose fused compile of the compiler dies (Windows: unrun).

## Q2 (465): ending the child on macOS (the ffi-pragmatist's numbers, read from their notes)

- **verdict:** approve the **sentinel per runner** (their route A); **object** to the kqueue watcher inside every program (their route B).
- **section:** §1.12; design.md `:1665` (reference counts stay non-atomic only while no counted value crosses a thread).
- **implementation_cost:** `runtime/parts/run.c` only. There is no emitter change and nothing is added per program.
- **needed_for_self_hosting:** no.
- **argument:**
  - Sentinel: ended the child 10 of 10. It costs about +0.95M instructions once per runner and nothing measurable per launch. It also covers children that are not Heroes programs: clang under `heroes build`, and the harness's own children.
  - Watcher: ended only **4 of 10** at 0.5 s under load, because the program runs unguarded between `exec` and its runtime's start. It adds 32 KB of resident memory and a thread to every program, against `:1665` and against the stack guard's signal handlers.
- **prediction:** the sentinel's 10-of-10 holds at 0.5 s with load above 30.
- **condition:** a measured case the sentinel misses and the watcher catches. **Unrun by me.**

## Q3 (470): where clang's memory goes in a one-exit function of many returns

- **verdict:** **approve route C, extended to record slots, as the repair. Do not close 470 as a known cost.** Coalescing is not the route (reason below). No veto.
- **section:** §1.12 (clang dying for memory on a correct program); §3.1.
- **where the memory goes** (an inference from two controls; the per-pass split with `opt`/`llc` from `llvm@22` is unrun):
  - The one exit releases every counted slot by value. At `-O2` every slot is promoted to a register value, so the exit merges one value per slot per way out: slots × returns.
  - Control X1, the same C with the exit's releases taking the slot's address: 800 returns go from **2,482.8e9 / 3.58 GB to 37.5e9 / 98 MB**.
  - Control X2, a `volatile` read of the slot instead: 101.5e9 at 200, unchanged from base.
  - So it is the promotion, not the size of the C text.
  - Coalescing has nothing to merge on this shape: every slot is a binding of the function's top scope and live at the one exit. That is an inference; coalescing is unbuilt.
- **implementation_cost:**
  - Built in heroes-c: `selfhost/emit/inst.hero` 473 → 499 lines.
    - New `release_slot`: a string, array or map slot is released through `hero_{str,array,map}_release_at(&slot)`.
    - `runtime/heroes_runtime.h` +6 lines; `str.c`, `array.c`, `map.c` +2 each. ABI left at 29 (the additions only add).
  - **`layout` fails**: `inst.hero` counts 365 lines against its 350. The two functions owe a module of their own (unbuilt).
  - The compiler's own tests: **1531 tests, 1 failed**, "the list holds every word the runtime's headers write". The three new names are owed in `guarded_names`.
- **numbers** (heroes-c at `-O2`; instructions, then peak footprint):

| returns | before | after |
|---|---|---|
| 200 | 101.5e9, 257 MB | 7.38e9, 37 MB |
| 400 | 505.9e9, 934 MB | 17.88e9, 56 MB |
| 800 | 2,482.8e9, 3.58 GB | 37.61e9, 113 MB |

  - Growth per doubling falls from ×3.6–3.8 to ×2.1–2.4.
  - At `-O0`: 1.29 / 2.62 / 5.99e9 before, 1.22 / 2.42 / 5.29e9 after.
  - The C's line counts are unchanged, and the 800-return program prints `3:333` both ways.
  - Run-time cost (`q3/bench.hero`, a three-return string function called 2e6 times at `-O2`): 5.035e9 → 5.146–5.195e9, **+2.2 to +3.2%**.
  - `emission` with heroes-c: **789 passed, 287 failed**, every failure a moved emission (no "emits nothing"). The grep prediction was 288. The seed moves too.
- **shape beside, not covered:** record slots. `T_release(&slot)` is local to the unit and gets inlined:
  - heroes-c output: 47.9e9 / 148 MB at 200 returns, 263.9e9 / 476 MB at 400.
  - The same C with that release marked `noinline` by hand: 9.5e9 / 42 MB at 200, 22.5e9 / 59 MB at 400.
- **needed_for_self_hosting:** no.
- **argument:** the author's rule was: if it holds and costs little, it is the repair. It holds, the growth goes linear, at a cost of 2–3% at run time on code heavy with releases. Robustness ranks above speed (§ Precedence).
- **prediction:** with C landed and extended to records, the 800-return string shape at `-O2` peaks ≤ 0.15 GB and ≤ 50e9 instructions on this Mac, and `bench.hero` moves ≤ +4%.
- **condition:** a real program at `-O2` slowed by more than 5%, or a platform where the extra call level costs more.

## Q4 (472): `step` into a function lands on the generated C

- **verdict:** **approve** that the **prologue alone** carries the function's line, with design.md §3.1's sentence amended. Cleanup and the exit stay under the generated file, which is defect 335's ruling. No veto.
- **section:** §3.1 `:682-688`.
- **implementation_cost:** built in heroes-d: `selfhost/emit/body.hero` 399 → 402 lines. One code line: `writer.at_generated(@w)` becomes `writer.at_span(@w, s, decls[f.decl].span.start)`. Every emission moves, because every prologue's `#line` changes (`emission` with heroes-d unrun). The `lines` suite with heroes-d: **422 passed, 0 failed**.
- **cases** (lldb on this Mac, `-O0 -g`, `q4/dbg.hero`):

| case | before | after |
|---|---|---|
| `step` into `greet` | `dbg.c:19:13` | `dbg.hero:1:13`, `h0_n=3` |
| breakpoint on the function | `dbg.c:19:13` | `dbg.hero:1:13` |
| `step`, `next`, `next` | `dbg.c:19`, `:20`, `:21` (one stop per prologue line) | `dbg.hero:1`, `dbg.hero:2:10`, `dbg.c:70:5` |

  - The last stop after the change is a release inside line 2, mapped to the generated file. That stop already exists today and is a wart beside this defect, worth filing.
  - Rejected route: `#line 0` rows. `step` stopped before the parameter was stored, showing `h0_n=6171910744`.
  - ASan, on a use-after-free I injected into the prologue (not a golden case): the frame was `dbg.c:98` before and `dbg.hero:2` after. My injection had no `#line` of its own; the emitter's version would name line 1.
- **needed_for_self_hosting:** no.
- **argument:** the `function` line is the function's own header, not a statement the author wrote that runs. The rule's purpose holds for cleanup, which stays generated.
- **prediction:** at landing, `lines` reads 0 failed, and every blessed emission and the seed move by prologue `#line` lines only.
- **condition:** a sanitizer report in the prologue that would send the author to the wrong place. **Unrun:** a `tests/golden/run/` case with a frame in housekeeping, and `-O2`.

## Q5 (523): an uninitialised deep struct and clang's walk

- **verdict:** **object to reversing panel 182.** Close 523 as a known cost with these numbers, and file the finding below. No veto.
- **section:** §3.1 `:676-678`; panel 182.
- **cases** (my C model, `q5/gen.py`, 2,000 locals of the top struct of a nested chain, `-O0 -g`, billions of instructions at depth 1,000 / 2,000 / 4,000):

| shape | depth 1,000 | depth 2,000 | depth 4,000 |
|---|---|---|---|
| uninitialised, then `t = make()` (today) | 12.53 | 35.10 | 118.18 |
| same, `-Wno-uninitialized` and its two siblings | 10.29 | 30.59 | 108.59 |
| union with a byte view | 11.31 | 32.53 | 112.53 |
| declaration at first definition | 3.90 | 6.38 | 12.75 |
| `= {0}` | clang exit 139 | clang exit 139 | clang exit 139 |

  - At depth 1,000 under `-fsyntax-only`:
    - today's shape: 9.74e9;
    - with every warning off: 7.48e9;
    - uninitialised but defined through an out-parameter: 3.08e9.
  - So most of the cost is the **call-result assignment** (defect 522's `hasConstFields` walk), not the missing initialiser. The check panel 182 kept costs about 2.2e9 of 12.5e9.
- **depth in real programs** (my regex over emitted typedefs, `q5/depth.py`):
  - the seed's deepest chain is 8 (`h_ast_Decl`);
  - in `tests/emission`, only 4 stress cases reach 32 or more (1,000; 250; 100; 64);
  - the next deepest is 16, and the examples reach 5.
- **needed_for_self_hosting:** no.
- **argument:** no real program comes near the depth where this cost shows. Declaring each temporary at its first definition is cheaper and zeroes nothing, but it is unbuilt: it needs a dominance rule and an amendment to §3.1's parenthesis on `goto` (C11 forbids a jump only into a variable-length array's scope).
- **finding beside it (CL-078):** a 1,000-deep record whose bottom holds a `str` fails `heroes build` with exit 2, "clang died (Segmentation fault: 11)", plus a note blaming nesting as the C compiler's limit.
  - The same emitted C with `= {0}` replaced by `__builtin_memset(&slot, 0, sizeof slot)` compiles (14.97e9 instructions, 135 MB), links and prints `x`.
  - So the emitter's `{0}` spelling is the limit, not clang's stack. A slot is still zeroed, as panel 182 rules.
  - Not filed by me. My search of `issues/` and `docs/panel/` for it came back empty, so it is a question, not a premise.
- **prediction:** with zeroing by `memset`, `fixedbugs-140`-style records holding a `str` build at depth 1,000 on this Mac.
- **condition:** a real program past depth 32.

## Not run

- the fixpoint of B, C or D;
- `wholes`, `determinism`, `emit`, the full net;
- any platform other than this Mac;
- route C for records inside the compiler, and its module split for `layout`;
- `emission` with heroes-d;
- the per-pass memory split;
- a golden case with an ASan frame in housekeeping;
- Q2's routes (their numbers, not mine).

**Reader-facing half that needs the absent seats:** Q4 (what the debugger shows) and Q1 (`--emit-c` starting to refuse at exit 1).

## Files

All under `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/scratch-b15/200-compiler-engineer/`:
- `notes.txt`
- `tree/selfhost/cli/assemble.hero`
- `tree/selfhost/emit/inst.hero`
- `tree/selfhost/emit/body.hero`
- `tree/runtime/heroes_runtime.h`
- `tree/runtime/parts/str.c`, `tree/runtime/parts/array.c`, `tree/runtime/parts/map.c`
- `q1/`, `q3/` (`gen.sh`, `genr.sh`, `cc.sh`, `m-*.txt`, `bench.hero`, `emission-c.log`, `selftest-c.log`, `layout.log`)
- `q4/` (`dbg.hero`, `step.lldb`, `next.lldb`, `lines-d.log`)
- `q5/` (`gen.py`, `depth.py`, `deepstr.hero`, `deepstr-memset.c`)

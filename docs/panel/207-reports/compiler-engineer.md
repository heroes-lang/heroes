# Panel 207, compiler-engineer

Copied by the coordinator at 12:21 on 2026-10-10 (`date`) from the seat's final reply (its transcript), verbatim below the rule; the seat's running notes are in its folder under `.claude/worktrees/scratch-b15/` (ignored by git).

---

# Panel 207, compiler-engineer

I worked from 12:02 to 12:20 on 2026-10-10 (`date`). My folder is `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/scratch-b15/207-compiler-engineer/`. `tree/` is an rsync of the frozen tree with its `.git` removed, and I ran no git inside it. The compiler there was built from the seed and then from `selfhost/`. Three variants of route A were built from it: `routeA/`, `routeA2/` and `routeA3/`. My notes are in `notes.txt`. The one git command I ran was `git -C lane-panel-207 ls-files`, a read of the frozen tree, to list the tracked files. None of my processes is still running (`pgrep`).

- **verdict**: **veto** of the brief's lead route, an exact evaluator with a step bound. I approve rewording C4b so it says what the walk decides. I object to route A as a repair for 580: it is cheap but repairs nothing in C4b.

- **section**: design.md Part 6, the compile-time evaluation paragraph (`docs/design.md:3159-3170`). It allows compile-time evaluation back only on three joint conditions, one of which is *"a structural termination argument ..., never a quota"*. My other grounds are design.md §1.7 (core plus elaboration) and panel 039's agreement 3 (`docs/panel/039-comptime-and-part-6.md:81-90`).

- **implementation_cost** (code lines in `layout`'s unit, `code_lines`, against `CEILING 300` in `tests/harness/suite_layout.hero:45`):
  - **Today's integer-only walk** is 522 lines: `selfhost/check/const_steps.hero` 294 and `selfhost/check/const_values.hero` 228. Its value type, `Slot` (`const_values.hero:41-43`), holds a sign and a 64-bit magnitude, nothing else.
  - **An exact evaluator** would need new value kinds: bool, floats, char, str, arrays, maps, payload cases and match patterns. Each operator on each kind would have to agree with the code the backend emits. const_steps is already 6 lines under its ceiling, so the evaluator needs at least one new module. **Not built**, so this is an estimate.
  - **Route A built in the resolver** (`routeA/`): `resolve/constant_body.hero` goes from 156 to 174 lines. But on the 577 golden it hid all 32 of the checker's diagnostics behind 3 `constant_body` errors. **So it is the wrong place for the refusal.**
  - **Route A built in the checker** (`routeA3/`): const_steps 295 (+1), const_values 241 (+13).
    - `check selfhost/main.hero` exits 0. Its instructions retired: base 80.999 / 81.057 / 81.081 G, route A 81.009 / 81.018 / 81.124 G. That is inside run-to-run noise, at load 4 to 5 from other sessions.
    - `heroes test selfhost/main.hero`: 1565 passed, exit 0.

- **needed_for_self_hosting**: **no**. `check selfhost/main.hero` with route A exits 0, so no constant the compiler reaches has a `while`. Route A's census moves no file under `selfhost/` or `examples/`.

- **argument**: Exactness needs a quota even without a step bound. With route A, `mine/double3.hero` (a `for` over three elements doing `s @ s + s`) builds and prints 8 bytes. `mine/straight3.hero` doubles a string on four straight lines and checks 0. A body that copies the value it evaluates therefore grows to 2^n bytes for n lines of source, so it needs a byte quota, which design.md refuses as it refuses a step quota. Route A leaves C4b false: s11, a `for` over a constant array, still builds and aborts with 134. Route A also refuses LOOPED, a correct ratified constant that prints 200. The forms admitted today cannot be evaluated exactly without a quota. So C4b must narrow to the walk's class: integer steps whose operands are known.

- **prediction**: Any route that makes C4b true on s06, s07, s08 and s15 (the straight-line index, assert, string index and nan shapes) will land at least 100 new code lines under `selfhost/check/` (layout's unit) and at least one new module, because `const_steps.hero` stands at 294 of 300. This is checkable at the landing commit of whichever defect takes those shapes.

- **condition**: I would withdraw the veto on three conditions, the three design.md names, met together:
  1. A measured Part 11 effect, meaning a blind seat that writes the index, assert or loop mistake in constant bodies, which panel 206 never measured.
  2. Termination bounded in **bytes** as well as steps, by structure: for example, string `+` refused in a constant body, or lengths tracked without materialising the strings.
  3. A named deletion.

  For route A: a tracked program that is not a golden and writes `while` in a constant.

**What I measured (route A in the checker, `routeA3/`, against the frozen tree's compiler):**

| what | result |
|---|---|
| p207 cases and critic-207 shapes | 16 move to `constant_body`: `loop`, `loop2`, s04, s05, s10, s12, s13, s14, s16, s17, s19, s20, s21, s24 (the endless one), s31, s32. 15 still build, among them s02, s03, s06, s07, s08, s11, s15, s18, s22, s27, s29 and s36. |
| R1's dead-code clause | kept: s12's `int_out_of_range` is still reported beside the new error |
| critic-206b shapes (94 `.hero` files) | 1 moves: `c19_const_while_false` |
| 577 goldens, harness `check` form filtered to 577 | 2 passed, 1 failed: exactly 3 diagnostics added (C19, LOOPED, UNTOUCHED), none removed |
| 577 goldens, harness `run` form | 2 passed, 1 failed: the run golden is refused |
| census: the 353 tracked files with a top-level constant (of 3,348 tracked `.hero` files) | 5 move: fixedbugs-139, 149 and 174, the 577 check golden, and `tests/golden/run/fixedbugs-577-a-constant-whose-every-step-fits-runs.hero` (exit 0 to 1) |

**What I did not run:**
- Route D, the exact evaluator, route C and the union with R1's walk: none was built.
- The string doubling at a size that matters: memory on a shared machine. The 2^n figure is arithmetic.
- The census over the 2,995 tracked files with no top-level constant.
- The `layout` suite itself (I counted lines with an awk copy of `code_lines`), the full net, `-O2`, Linux and Windows, `--refresh`.
- I did not open the 34 multi-line bodies in `selfhost/`.
- My claim that the compiler would render floats with the same runtime code as the program, so no second copy of the printer, is an inference.

Files are in `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/scratch-b15/207-compiler-engineer/`:
- `notes.txt`
- `shapes.tsv`, `shapes-a3.tsv`, `shapes206.tsv`
- `census-base.tsv`, `census-a3.tsv`
- `harnessA3-check577.txt`, `harnessA3-run577.txt`, `harnessA-check577.txt`
- `t3-*.txt` (timings)
- `mine/` (the two doubling shapes)
- `routeA3/selfhost/check/const_steps.hero`, `routeA3/selfhost/check/const_values.hero`

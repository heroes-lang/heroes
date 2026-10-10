# Panel 207, spec-warden

Copied by the coordinator at 12:14 on 2026-10-10 (`date`) from the seat's final reply (its transcript), verbatim below the rule; the seat's running notes are in its folder under `.claude/worktrees/scratch-b15/` (ignored by git).

---

# Panel 207, spec-warden

Clock: 12:03:05 to 12:14 (`date`), resumed after the session limit. Folder: `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/scratch-b15/207-spec-warden/`. `tree/` is an rsync of `lane-panel-207` (259de604) without `.claude/worktrees`. I removed its `.git` at once and ran no git in it. The compiler was built from the seed and then from `selfhost/` (exit 0). Running notes are in `notes.txt`.

## Base, measured

`heroes measure spec/heroes-spec.md` in my copy reads **7430 claude-legacy, 7565 cl100k and 9944 real**. That leaves 296 of headroom and **236 spendable** after the FFI floor. design.md §1.6, grepped today, still says 10240.

## What I ran

All of it is in `shapes-round.txt` and `probes/round.txt`.
- The 3 p207 cases and the critic's 36 shapes, copied (the originals stay read only). My results match the critic's table.
- 15 probes of my own (t1 to t11, u1 to u4). They locate the walk's boundary:
  - **The walk refuses:**
    - a `=` name read inside a loop (t1)
    - a straight-line cell (t2)
    - a chain of constants (t3)
    - a cell read after a loop that does not write it (t5)
    - a cell written earlier in the same loop body (t6, u1)
    - a branch that does not write the cell (u2)
  - **It refuses steps that never run:** the dead branch `if 1 > 2` (t4) and an unread constant (t7). So C4b is wrong in both directions: it also refuses steps that never abort, and it does not say so.
  - **It does not refuse, and the program aborts with 134:**
    - a cell read inside a `for` loop that never writes it (t8)
    - arithmetic on an array element (t10)
    - a `=` name bound to a loop-dependent value (u3)
  - **It does not refuse, and the program exits 0:**
    - `INT8_MAX + 1` on a group's constant prints 128 (t11)
    - `B = A + 100`, with `A` computed by a loop and never read, prints 1 (u4)
- **The compiler-engineer's route A**, which refuses `while` in a constant body. I copied its binary to `tree/heroes-ceA` and ran `check` only (`routeA-check.tsv`).
  - Every `while` body is refused as `constant_body`, including the correct `loop2` and `s16`.
  - C4b is still false on 13 shapes that build: s02, s03, s06, s07, s08, s11, s15, s18, s22, s27, s36, t8 and t10. On s29 too.
  - Its census of 5 moved files is **carried** from its `census-a.tsv`. I did not run it.

Of the 54 files, 46 are judged by the walk; the other 8 are refused for other reasons. On the walk as landed:
- 14 are refused
- 22 build and abort with 134
- 9 run and exit 0
- 1 hangs (s24, exit 124)

## The verdict

- `verdict`:
  - **object** to C4b as landed: false on 22 of the 46 judged files.
  - **veto** the evaluator routes (K1, K2) and the probe route (K3).
  - **approve D1r, provisional**: every price is a vendored lower bound.
- `section`:
  - design.md §1.0, Principle 0 (the veto)
  - design.md :3165-3170: compile-time evaluation returns only with *a structural termination argument ..., never a quota*. A step bound or a ten-second bound is a quota. This paragraph covers an evaluator kept only for a diagnostic, because the termination and duplication reasons are identical.
  - design.md §1.2 and §1.6, the payment rule
  - CLAUDE.md § 12
  - `.claude/rules/spec-shape.md`, panel 087
- `spec_token_delta`: vendored lower bounds against C4b (7430/7565), no `--refresh`. Real figures are inferred from the document's ratio of 1.31.

  | draft | delta (legacy / cl100k) |
  |---|---|
  | D0 | -13/-13 |
  | C4a | -7/-7 |
  | **D1r** | **-1/-1** |
  | D1s | 0/0 |
  | D1 | +3/+3 |
  | D1Ar | +4/+4 |
  | D1A | +8/+8 |
  | K3 | +14/+14 |
  | N3 | +17/+17 |
  | D1c | +18/+18 |
  | K4 | +18/+18 |
  | K2 | +31/+31 |
  | K1 | +34/+34 |
  | N1 | +60/+59 (about +78 real, inferred) |
  | N1A | +65/+64 |
  | Rfwd (a removal) | -7/-7 |

- `removal`:
  - D1r is a net -1 against C4b and owes nothing.
  - For N1: **nothing, and that is a problem**. The only redundancy I found in § 4 is Rfwd at -7, about an eighth of N1's price.
- `needed_for_self_hosting`: **no**. The compiler builds itself with the walk as it is. The critic's 0 of 333 `selfhost/` constants with a loop or a branch is carried, not re-run.
- `argument`: No route without a quota makes "every aborting step is refused" true. A `while` can abort at any depth, and the evaluator or probe routes either bound it, which design.md forbids, or leave it false. Route A still leaves 13 shapes false, and doubling a string inside a `for` needs a byte bound, which is again a quota. The walk's real set, N1, costs 59 tokens, describes the compiler's internals, and becomes false the moment the walk improves. The class occurs in no constant outside the tests. D1r is true on all 46 files under every route, costs one token less than C4b, and lists no aborts.
- `prediction`:
  - **P1**: D1r reads between -3 and +1 real at a landing `--refresh`.
  - **P2**: if N1 lands instead, it reads +70 to +85 real.
  - **P3**: at the next milestone tag, the census finds 0 written constants with a `while` outside `tests/golden/`.
- `condition`:
  - I would approve keeping C4b only if two things hold:
    - a route makes it true on all 46 judged files with a structural termination argument and no quota;
    - a Part 11 measurement shows the class in real first tries.
  - I would prefer N1 to D1r if either holds:
    - a blind run shows the "may" in D1r costs a rewrite that N1 prevents (at least 1 of 4 readers);
    - the author rules that the spec must state compile-time refusals exactly, at their cost (CLAUDE.md, 2026-09-28).

## Whether each draft is true

| draft | on the walk as landed (46 files) | on route A (`check` exits) |
|---|---|---|
| C4b | false on 22 that build and abort, and either on s20 and u4 or on s12, s30, t4 and t7, depending on how "aborts" is read | false on 13, plus s29 |
| D0, R1 kept as an error | false on the 14 refused: § 7 predicts an abort, or exit 0 for t7, and § 12 then makes R1's refusal the compiler's defect | |
| C4a, R1 kept as an error | false on the same 14 | |
| C4a, R1 demoted to a warning | **unrun**. Grepping for `warning` and `severity` found no Heroes warning severity, so a warning would be a new diagnostic class, which needs a panel | |
| **D1, D1r, D1s** | **true on 46 of 46** | false: they say nothing about `while`, and route A refuses correct bodies |
| **D1A, D1Ar** | | **true on 46 of 46** |
| D1c | "can compute" cannot be checked, and a reader of s06 would predict a refusal | |
| **N1** | **true on 46 of 46**, by my own application of the sentence; no blind reader has tried it | |
| N1A | | true on 46 of 46 |
| N3 | false on 9 (t1, t2, t4, t5, t6, u1, u2, s25, s30). It needs a narrower walk that nobody has built | |
| K1 to K4 | false on the walk; their own routes are not built, so unrun | K4 false on 13 |

On panel 087: N1 names one class of refusal and does not declare the list of aborts closed, so it is not 087's S4. It does carry the same weakness in the refusal direction: a running program can falsify it.

The `spec` suite in my copy: D1r and N1 each read 19 passed and 4 failed. The 4 are `budget`, `spendable`, `real` and `ledger`, the pinned count owed at a landing. The frozen spec reads 23 and 0. My copy was restored and checked with `cmp`.

R1's ratified wording, *a step that cannot be computed is a compile error, wherever it is written*, is false in the same way. The record owes a dated correction beneath it.

## The drafts, whole

Each replaces § 4's text after *Constants use SCREAMING_CASE, and*:
- **C4b**: *a written body computes over literals and other constants, and a step of it that aborts is a compile error.*
- **D0**: *a written body computes over literals and other constants.*
- **C4a**: *... other constants, each time it is read.*
- **D1**: *... other constants, and a step of it that would abort if run may be a compile error.*
- **D1r**: *a written body computes over literals and other constants, and a step of it that would abort may be refused.*
- **D1s**: *... other constants, and the compiler may refuse a step of it that would abort.*
- **D1c**: *... other constants, and a step of it that would abort, run or not, is a compile error where the compiler can compute it, else an abort at each read.*
- **D1A**: *... other constants, with no `while`, and a step of it that would abort if run may be a compile error.*
- **D1Ar**: *... other constants, with no `while`, and a step of it that would abort may be refused.*
- **K1**: *a written body is computed when the program is compiled, over literals and other constants: a step of it that would abort, run or not, is a compile error, as is a body past a million steps, and a step on a group's constant aborts where it runs.*
- **K2**: *a written body is computed when the program is compiled, over literals and other constants: a step of it that would abort, run or not, is a compile error within its first million steps, and past them or on a group's constant aborts where it runs.*
- **K3**: *a written body is computed when the program is built: a step of it that would abort, run or not, stops the build, as does a body still running after ten seconds.*
- **K4**: *... other constants, with no `while`, and a step of it that would abort, run or not, is a compile error, unless it reads a group's constant.*
- **N1**: *a written body computes over literals and other constants. An integer operator in it that would abort, run or not, is a compile error where its operands are known: a literal, an operator on known values, a `=` name or written constant holding one, and a cell written one earlier in the text, in the loop body it stands in if any, with no branch or loop between that writes it.*
- **N1A**: N1 with *, with no `while`* after *other constants*.
- **N3**: *... other constants, and integer arithmetic in it on literals alone, or on constants of one such line, that would abort, run or not, is a compile error.*
- **Rfwd** (a removal): § 4's *Declaration order never matters but a group's (section 13); mutual recursion needs no forward declarations.* becomes *Declaration order never matters but a group's (section 13).*

## What I did not run

- No `--refresh`, so every real figure but 9944 is an inference. No paid run and no blind readers.
- No evaluator, no probe route, no warning route, and no N3 walk.
- Route A was run with `check` only, using the compiler-engineer's binary; I did not repeat its runs.
- No census of my own. The compiler-engineer's census and the critic's 1,063 and 333 figures are carried.
- No suite other than `spec`. No `-O2`, Linux or Windows.
- No process of mine was left running (`pgrep`).

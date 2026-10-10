# Panels 202 and 203, completeness critic, first pass

Copied by the coordinator at 23:06 on 2026-10-09 (`date`) from the seat's final reply (its transcript), verbatim below the rule; the seat's running notes are in its folder under `.claude/worktrees/scratch-b15/` (ignored by git).

---

Pass 1 found **20 repairs** to the briefs. Panel 202's framing is too narrow: "groups bind one C name" covers only some of the header-conflict shapes. And I found four failures nobody has filed (repair 3): two give a wrong answer with no error, and two blame the compiler at exit 2.

I built a compiler in the three stages the briefs prescribe, in `.claude/worktrees/scratch-b15/critic-202-203/tree/` (the seed against `b17-rt29/runtime`, then selfhost twice). By mistake, a second stage-2 build ran at the same time as the first, sharing one build cache. Stage 3 built and printed `heroes 0.2.0`, but no check was rerun on a clean rebuild. My cases are in `critic-202-203/p202/<name>/` and `p203/`. Nothing was written in the frozen trees.

## Repairs to the briefs

**202, `00-shared.md`**

1. **These hold, checked by command:**
   - `assemble.hero:99` (`if site.is_tests`).
   - `headers_together.hero` is 65 lines.
   - `cli-surface.md:41` says "exit 1 the input has diagnostics".
   - `c-boundary.md:47` has six members.
   - The seed asserts ABI 29 (`seed/heroes.c:14`). The runtime says 30. `b17-rt29/runtime.c` is identical to `541d9595`'s.
   - The fp and fp2 numbers you carried hold on my build: `check` 0, `build` 0, prints 6 and 8, `test` 1. The new message is word for word the same in both orders.
2. **550's reproducer is C only.** `b17-emit/inc/` holds u1.c, u2.c and three headers, with no `.hero` file. Point the seats to `p202/inc1` and `p202/inc2` instead.
   - `inc1` (`use left` then `use right`) gets the old false message: "`b.h`, the header this group names, does not compile … previous definition … `a_impl.h`".
   - `inc2` (swapped) gets the new message.
   - Both build at exit 0 and print 6 and 8.
3. **Widen the question from "groups bind one C name with different types" to "headers two groups name that cannot share one unit".** Measured on the case folders:

   | case | check | build | test | `--emit-c` | what it shows |
   |---|---|---|---|---|---|
   | `one` (538's own shape: one module, two groups) | 0 | 1 | 1 | 1 | `check` already disagrees with `build` here |
   | `nobind` (`helper` defined differently in both headers, bound by neither group) | 0 | 0, prints 6 12 | 1 | 0 | The fix note "bind a C name they share" doesn't fit |
   | `macro` (a.h `#define twice(x)`, b.h `static inline int twice`) | 0 | 0 | 1 | 0 | `test` gives the old false message "`b.h` … does not compile: expected identifier or '('". A second false-message shape beside 550, unfiled |
   | `macro2` (same, `use` lines swapped) | 0 | 0 | 0 | 0 | The verdict itself depends on `use` order, so under route (b) `build` would too |
   | `onedef` (one header defining a non-static function, named by two modules) | 0 | 2 | 0 | 0 | The reverse disagreement. Route (b) gains this program |
   | `extdef` / `extsame` (two headers each defining a non-static `twice`) | 0 | 2 | 1 | 0 | `build` blames the compiler |
   | `clash` (a.h declares `long twice(long)`, b.h `double twice(double)`, a third module defines it) | 0 | 0 | 1 | not run | **Wrong value at exit 0**: `right.right(x: 4.0)` printed `4.0` |
   | `unguard` (one header with no include guard, named by two modules) | 0 | 0 | 0 | 0 | The fused unit includes each header once (`#include <c.h>` count is 1). This worry is settled |

   - The exit 2 cases say "internal error: linking failed: duplicate symbol '_twice'".
   - `grep -i 'duplicate symbol'` in `issues/` finds only the 2026-08-11 linkage-rule items about Heroes' own functions, so `onedef`/`extdef` are unfiled. They are `blocking` class: an exit 2 where the author could be told.
   - `clash` is one C symbol with two prototypes (the case Rust's `clashing_extern_declarations` lint covers). Route (c) would accept it in all four tools.
   - The `macro` false message is also unfiled.
4. **`check` asks clang nothing today.** `extern "nosuch.h"` passes `check` at exit 0 (`p202/nohdr`). An `i64` parameter against the header's `int` passes `check` at 0; `build` refuses it with `ffi_parameter_type` at exit 1 (`p202/width`). So route (a) would be the first clang question `check` ever asks, since all six classes are told at build. The brief should say so.
5. **Is 538's message a seventh class?** The sixth class at `c-boundary.md` reads "a header … that clang refuses on its own". 538 told headers that each compile alone under that code, and its card says "No new class". Ask the spec-warden and the compiler-engineer whether that is within the six or a seventh member that landed without a sitting.
6. **Name `heroes run`** (`build` at `-O2`) beside the four verbs.
7. **Stale clauses inherited from panel 200's rules.** "Through panel 200's rules" brings in the Windows box "shared with lane b17-fix", "lane-b17-fix works beside you" and the 21:00 time box. Say which still bind. `lane-b17-fix` still exists.

**202, `compiler-engineer.md`**

8. **Count per program, not per file.** 1,357 of the 3,163 tracked `.hero` files have an `extern`, and the route moves programs (a root and its modules). `heroes test <root>` already compiles route (b)'s one unit, so half the census is `build` against `test` per root, with no prototype needed.
9. **Add two costs for route (b) on `build`:**
   - `-O2` (`run`'s level) with peak memory (defect 470's class).
   - A warm rebuild after editing one module: the per-module cache recompiles one object, a fused unit recompiles everything.
10. **Route (c) for `--emit-c` reverses panel 200 R1, which is ratified.** The artifact is one file on stdout, and R1's point was that the file written is the one clang read. Say so.

**202, `ffi-pragmatist.md`**

11. The link question is partly measured:
   - Two `static inline` definitions never clash at link (fp builds).
   - Non-static definitions do clash (`extdef`, `onedef`).
   - Ask for a census of headers that define non-static functions (header-only libraries without `static`).
   - Ask what plain C99 `inline` (non-static, non-extern) does per module at `-O0`. Unrun.

**202, `historian.md`**

12. Add:
   - C11 6.2.7p2 and 6.9p5: two incompatible declarations of one external name across translation units are undefined behaviour with no diagnostic required. That is the `clash` case.
   - GCC's `-Wlto-type-mismatch`, a whole-program view catching that case. Clang's equivalent is unrun.

**202, `spec-warden.md`**

13. **The negative grep holds but misses the nearest precedent.** It matches only `:374`. Its vocabulary misses § 13 `:394-395`, "two records may not name one tag but `void`", which is the spec's one sentence about two declarations sharing a C name. The checker enforces it across the whole program (`check/acquiring.hero:19`, `one_tag_one_type`).

**203, `00-shared.md`**

14. **These hold, checked by command:**
   - N1f's text is at `spec:276-279`.
   - +16 real tokens (ledger row 7479: 9,831 to 9,847).
   - "Flat only" is at `design.md:1805-1806`.
   - `912b2df3` (520) and `61624d0f` (538) are in the tree.
   - `param_value.hero` passes `check` at 0 and runs to **134**, "stack exhausted in paramvalue.go, inside the recursion of paramvalue.go and paramvalue.step".
15. **Name the fourteen by file.** The folder holds 23 `q1_*.hero` files plus `q1_qualified/`.
   - Today the compiler refuses 15 of those files plus `qualified`. The two turbofish files are refused `unknown_name` by design.
   - The fourteen: app, map, fold, filter, qualified, nestedok, nestedok2, resultonly, literal, concrete, concrete2, concrete3, ufcs, emptyarr.
   - Add two refused files outside the fourteen:
     - `literal2`, `first(a: 255, b: b)`, where the literal comes before the argument that settles the type. This tests whether order matters.
     - `nestedcall`, `x = first(a: ok(1), b: ok(2))` with no context, which must stay refused.
   - Four of the fourteen are told `type_mismatch`, not `cannot_infer` (concrete, literal, literal2, ufcs).
16. **Under the general route N1f is false, not optional.** Panel 201's second critic pass said "N1f becomes false under q1". `design.md` §4.12 "Flat only" (panel 105, ratified) falls with it. The spec-warden's brief names only § 9: add §4.12, and say that panel 105 is partly reversed.
17. **Unasked: does a literal argument settle the type, or wait for the context?** Today a literal settles the type parameter as `i64`:
   - `y: u8 = first(a: 1, b: 2)` is refused "expected `u8`, found `i64`".
   - `show(v: first(a: 255, b: 2))` with `show(v: u8)` is refused the same way.
   - If literals wait for the context, both become accepted, and `y: u8 = first(a: 200, b: 2) + 100` changes from refused to an overflow abort.

   This is spec § 2 meeting § 9, and the census sees only accept or refuse, not a literal changing width (`p203/mean/m1-m3`).
18. **Q2's neighbouring shapes, measured:**
   - Through a built-in: `[n].map(step)` inside `step` passes `check` and aborts 134 at run time, "inside the recursion of library.map_37f8817a and viamap.step".
   - Through a record field (`Box(f: step)`, then `b.f(n)`): passes `check`, aborts 134.
   - Correct programs: a way out in `go`, or in `step`, both pass `check` and run to exit 0.
   - The built-ins and a used module's higher-order function cross module boundaries without a module cycle. Panel 201's historian leaned on `module_cycle`.
   - `map`, `fold` and `filter` call `f` only on a non-empty array, so they contain a way out.
19. **Q2 stands on existing rulings, and following a parameter is non-local.**
   - Spec `:281` already says "Recursion too deep aborts", and panel 199 R2 ruled that "unbounded recursion is an abort" at every level.
   - Panel 199 R1 says "a function nobody calls is judged as if it were called". Following a parameter makes the verdict at `step` depend on the body of `go`.
   - The llm-ergonomist (the veto on non-local constructs) is not seated. Say who scores locality.
20. **Process, both sittings.** Neither brief asks for the blind seat's prediction file before the runs, as panel 201's `llm-ergonomist-prediction.md` did.

## Routes nobody listed, and questions not asked

**202**
- **(e) A declaration comparison in the checker, no clang.** § 13 lets a result be wider than C's, so two correct groups can declare one C function with different Heroes types. A fair comparison needs the C types, which only clang knows. It would also miss `nobind` and `macro`.
- **(f) Each group's header in its own unit, behind thin wrappers.** It accepts fp, nobind and macro in both orders. It breaks a group record passed by value and `constant`, and costs one call per binding without LTO. Price it.
- **(g) Get 550's include graph from clang's `-H` output in the same run**, not from a second clang run.
- **(h) LTO or a link-time type check** for `clash`.
- **Questions:**
  - Is a program whose headers conflict a correct program? (`static inline` helpers yes; `clash` is undefined behaviour in C; `onedef` is wrong C, but the author never chose the units.)
  - May a verdict depend on `use` order (`macro`)?
  - What does `run` say?
  - What replaces the fix note for `nobind`?

**203**
- **Q1, a narrower route:** context only through a generic function's parameters whose type has no type parameter in it (concrete, concrete2, concrete3, ufcs). It has no settling-order question, and the spec-warden's draft N5 (+18) already words it.
- **Q2, a per-function summary instead of walking callers:** "calls its parameter `f` on every path", composed the way `may_end` is, with summaries for the built-ins. A non-empty array literal has no way out; a variable array may be empty, so it does.
- **Q2, sharpen the run-time message instead:** the field shape's panic names only `viafield.step`.

## Blind tasks (about 0.25 USD a session, 12 sessions per sitting, about 3 USD each)

**202**
- **B1, predict (4 sessions).** Give the fp files and ask for each of `check`, `build` and `test`: the exit code, and the output if it runs. Two sessions get today's spec, two get the leading route's sentence. Add one `clash` variant: "what does `right.right(x: 4.0)` print?" Predicting 8.0 shows the wrong value is invisible to a reader.
- **B2, repair (4).** Give 538's message on fp, and on nobind. Task: "make `heroes test` pass and keep the output". Score whether readers follow the note into editing the extern groups (wrong for nobind) or give a group a header of its own. Arms: today's message against the route's message.
- **B3 (2).** The `macro` program in the refusing order, with today's false message. Score edits to `b.h`, which compiles alone. This prices 550's class of false message.
- **B4 (2).** `onedef` with today's exit 2 linker message. Score whether readers blame the compiler or make the function `static inline`.

**203**
- **Q1 predict (6).** One program with six lines: `ns.map(ident)`, `first(a: b, b: 255)` with `b: u8`, `g(x: 1, n: ok(2))`, `first(a: xs, b: [])`, `y: u8 = first(a: 1, b: 2)`, `x = first(a: ok(1), b: ok(2))`. Ask accepted or refused, and the type. Three arms of two sessions:
  - Spec with N1f (today).
  - N1f removed.
  - The general route's rewording.

  Today all six are refused. Under the general route the first four are accepted and the sixth refused; line five depends on repair 17.
- **Q1 write (2).** "Write `lengths(xss: [[i64]]) -> [i64]` using `map` and a generic `len_of<T>`." Count how often readers write `xss.map(len_of)`.
- **Q2 repair (4).** Give `param_value.hero`. Two sessions get today's build 0 and run 134 with its panic; two get a prototype refusal naming `go`'s parameter `f`. Task: "make it build and do what it evidently means." Score whether the base case goes in `step` or in `go`.

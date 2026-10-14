# Panel 214, the compiler-engineer's report

Written 2026-10-14 from 01:04 to about 03:00 in my copy `.claude/worktrees/scratch-panel-214/compiler-engineer`
(detached at `8c8783ac7`, compiler built from the seed: `clang -I runtime seed/heroes.c
runtime/runtime.c -o heroes`, exit 0). Every number names the command that produced it. Nothing was
timed (the release's net ran beside me). The copy is removed at the end; the prototype's whole diff
is the appendix, so nothing here depends on a folder that no longer exists.

## The verdict, in the seat's structure

- `verdict`: **veto**, on two routes: **A** (keep adding one reader per shape), standing on §1.1,
  and **H** (read the runtime after the groups' headers), standing on §1.12. I **approve O + C**, a
  route that combines B's idea and C's, built and measured below. A veto compels a written answer
  from the author. It blocks nothing.
- `section`: design.md §1.1 (*"Implementation simplicity is a constraint ... It sets the
  ceiling"*), §1.7 (the size test), §1.12 (robustness) for H, §4.17 for the messages.
  `.claude/rules/c-boundary.md:133` (*"the narrowing is `declaration()`, not whose text it is"*):
  O+C follows it, because blame is decided by a clang run and a name's form, never by which file
  clang prints. design.md says nothing about who is blamed when a header fails to compile (`grep -n
  "ffi_header_refused\|internal error\|compiler bug" docs/design.md`: one hit, line 2715, about
  leases). So my objection to A rests on §1.1's ceiling, not on a rule written for this question.
- `implementation_cost`: **O + C as built on this tree: +296/-11 lines in four files**
  (`git diff --stat 8c8783ac7 -- selfhost`): `cli/whose.hero` +90, `cli/header_refused.hero` +110,
  `cli/declared_after.hero` +62/-6 (one test restated), `cli/clang_first.hero` +34/-5
  (`git diff --numstat`). In code
  lines (`code_of`): +186. **The same route O alone, built on the tree before the night
  (`08230cbfa`): +168/-9** (`clang_first` +31/-6, `header_refused` +75/-3, `whose` +62). The night
  after `08230cbfa` added **+1,506/-24 lines to `selfhost/cli/`** (`git diff --numstat 08230cbfa
  8c8783ac7 -- selfhost/cli`). Nothing lands in the lexer, the checker, the descriptors, ownership
  or the emitter. Every line is on the failure path of `cli/`.
- `needed_for_self_hosting`: **no**. The compiler builds itself without it. Robustness is the
  case for it: today, 8 author mistakes measured below end at exit 2 and a false *internal error*.
- `argument`: on this tree, 8 header mistakes I wrote in an hour are still told *internal error*.
  Two of them sit right beside readers that landed hours ago. Each shape costs A a new text reader
  of 150 to 420 lines, and the night spent 1,506 lines. Route O asks clang one question: is the
  unit's opening refused with the groups' headers and accepted without them? On the pre-night
  compiler that one question tells 18 of the night's 23 golden cases as the header's, plus 3 that
  today's tree still misses. Route C, a prefix test on the program's own C, tells 6 more. Census:
  878 programs, 0 moved. H refuses 18 tracked headers that compile today.
- `prediction`: **if A stays the rule, the 8 shapes listed in W4, each filed as a defect and
  repaired one by one, add at least 400 net lines to `selfhost/cli/`**. The instrument is
  `git diff --numstat <base> <close> -- selfhost/cli/`, read at the close of the milestone that
  repairs them. After `08230cbfa` the night spent +1,506 net lines in `selfhost/cli/` on six
  commits (2044 +270/-17, 2045 +396/-20, 2049 +601/-66, 2052 +99/-16, 2051 +247/-12, 2053 +1/-1,
  each `git show --numstat`), about 300 per substantive defect. **If O+C lands, those 8 shapes
  close with the appendix's 296 lines, and each later shape adds 0 lines to make its exit code
  right.** Checked the same way, at that milestone.
- `condition`: I lift the veto on A if someone shows a compiler defect that today's readers tell
  at exit 2 and O+C tells at exit 1. That would be a soundness loss. I looked for one and found
  none: the two simulated defects in W6 are told the same way by both compilers. I also lift it if
  the census, run on Linux or Windows, moves a program. I lift the veto on H if the 18 headers in
  W8 are shown to be outside the language's promise.

## W1. What the night cost, by the instrument

`git diff --numstat 2190ff8a3^ 8c8783ac7 -- selfhost/`. These are net lines added to the readers,
counted from just before defect 2034 up to this tree, with comments and tests included:

| file | lines |
|---|---|
| `runtime_name` | +418 |
| `header_refused` | +311/-36 |
| `declared_after` | +292 |
| `builtin_name` | +213 |
| `standard_name` | +181 |
| `clang_first` | +180/-16 |
| `group_file` | +59/-13 |
| `header_order` | +49/-1 |
| `order_trial` | +19/-1 |
| `runtime_key` | +14 |

Total: **1,669 net lines, in eleven commits over 21 hours** (dates from `git log --format='%h
%ad'`). On a separate axis, `emit/header_region.hero` grew +128/-6. That axis decides which warnings
are errors inside a header's region. Pascal-P4 is about 4,000 lines and sets the scale of the
ceiling, so one night spent 42% of a P4 on who is told for a single diagnostic class.

Code lines alone, measured with the `code_of` awk of `.claude/rules/verification.md` (comments,
blank lines and `test` blocks left out). The total line counts reproduce the brief's (`wc -l`).

| module | code lines |
|---|---|
| `clang_first` | 183 |
| `header_refused` | 181 |
| `standard_name` | 74 |
| `runtime_name` | 115 |
| `declared_after` | 136 |
| `builtin_name` | 69 |
| `group_file` | 69 |
| `header_order` | 227 |
| `header_alone` | 31 |
| `headers_together` | 65 |
| `runtime_key` | 41 (only `made_of`, 9 lines, is defect 2035's) |
| `whose` | 128 |
| `order_trial` | 95 |
| `emit/header_region` | 39 |

## W2. What each reader actually applies

Line numbers are at `8c8783ac7`. "O+C" in the last cell of each row is the route I adopt.

| module | the rule it applies | under O+C |
|---|---|---|
| `clang_first.reading` (70-165) | Takes the first error (`first_error` 232-249). Gives up on a missing file (74-75). Walks the include stack and takes the innermost group's header, never a compiler file (83-96, defect 2035). **Refuses when any compiler file stands after that group in the stack (97-102). Refuses when a note stands in a compiler file, unless `explaining` (182-214) recognises it (119-129).** Passes over macro-expansion notes (146-147, defect 2040). A note in another group's header means two headers (145-160). | The two bolded refusals stop deciding blame once the opening probe has spoken: `read_as(involved: true)`, +34 lines. `explaining` stays, as a writer of notes. |
| `clang_first.explaining` (182-214) | Tries four readings in order: `standard_name.declared`, `runtime_name.declared`, `.called`, `.suggested`. If none matches, the refusal is the compiler's. | Kept for its wording. Its verdict no longer matters. |
| `standard_name` (35-68) | A runtime line of the form `typedef __X_TYPE__ x_t;`, matching C11 7.20.1's names, is phrased as C's own name (defect 2044). | Kept for its wording. Without it the note reads "the Heroes runtime's `heroes_runtime.h` line 78", which is worse (W7). |
| `runtime_name.declared` (46-54) | The identifier under clang's caret equals the identifier at the note's column in the runtime's line (defect 2045). | Kept for its wording and its rename fix. |
| `runtime_name.called` (62-71) | The name before the first `(` of the runtime's line equals the callee around the caret (defect 2049). **It reads only the line clang quoted, so a call split over two lines escapes it** (x3, W4). | Kept for its wording. O tells x3 correctly. |
| `runtime_name.suggested` (83-92) | Looks for `did you mean '<name>'?` in the message or in a note at the error's place (defects 2049, 2052). | Kept for its wording. |
| `declared_after.told` (64-94) | The first error lies outside the headers, in one of two places: in a C header that the runtime's `#include <x.h>` line brings in (`included`, 154-169), or in the compiled file at a mangled program function (`emitted`, 177-188). Either way, the first note that is not an expansion must stand in a group's header at the same caret name (127-147). | The library branch is kept for its wording, and O decides its blame. **`emitted` is widened to `kept`**: any name in the compiler's own forms that no group binds (+62 lines, built). |
| `builtin_name` (53-102) | An error in a header the compiled file includes, whose only notes are clang's "included multiple times" at the include line and "unguarded header", means a clang builtin (defect 2051). | Kept for its wording. O decides its blame. |
| `group_file`, `runtime_key.made_of` | Which group a path belongs to, and which files are the compiler's (defects 2035, 2040). | Kept. Every route needs them. |
| `header_order`, `header_alone`, `headers_together` | Panel 204 R3's search for a move, defect 557's "compiles alone", and two headers refused together. None of them decides blame. | Kept and untouched. |
| `emit/header_region` | The region's pragmas: `QUIETED` 2, `KEPT` 5, `FORMAT_DEFINED` 5, `EXTENDED` 1. These decide what is accepted or refused, not who is blamed. Defects 2034, 2036, `5a92815d3`, 2052's flag and 2053 all belong to this axis. | Untouched. No route here reaches it. |

## W3. The route I adopt: O + C

- **O, the opening decides.** It is asked last, in `cli/whose.hero`, after every existing reader
  and every `ffi.explain` class has declined, just before `internal error` would be printed. The
  compiler compiles the unit's opening, `order_trial.opening(theirs(unit_headers))`, which is the
  unit up to and including `heroes_standard.h` without any C written for the program, under the
  unit's own flags. Then it compiles the same text **with only the group include lines removed**
  (`without_headers`). If the opening is refused with the headers and compiles without them, the
  headers are what clang refused, and the refusal is told at exit 1 on the group's string. The
  group is found from the stack, otherwise from a note, otherwise as the first header whose opening
  is refused when compiled alone (`alone_refused`). Every existing class keeps its wording because
  O runs only where today prints *internal error*.
- **C, a name of the compiler's own forms.** A first error in the program's C, at a name of the
  forms `h_`, `hero_`, `Hero`, `HERO_`, `HEROES_` or `main` that no group binds, whose first note
  stands in a group's header at that same name, is the header's (`declared_after.kept`). Any other
  name stays exit 2, so a defect that makes the emitter write a name outside its forms is never
  blamed on the header. The prefix list must equal the sentence the spec-warden prices.
  Measurement: of the 192 file-scope names the runtime declares (a JSON AST dump of `heroes_unit.h`
  plus `hero_os.h`, deduplicated by name), only ten fall outside these forms: the ten `<stdint.h>`
  typedefs, which are C's own names. Every macro beyond clang's own is `HERO_*` or `HEROES_*`,
  apart from `<stdbool.h>`'s and `<stddef.h>`'s (`clang -E -dM`, compared with an empty file's).
  Every top-level name in the 605 emissions is in these forms, plus bound names (a crude
  regular-expression grep of `tests/emission/*.c`; the one other name it printed, `struct pair`,
  is a bound record's tag).
- **Owed but not built: a test that holds the runtime's names to those forms.** It closes the one
  residual that every route shares (W6, NC1). My estimate is about 40 lines, reusing a reader of
  clang's AST dump such as `cli/renamed.hero`'s. Unrun.

## W4. The cases, run with the prototype (`build/heroes-o3`, the appendix)

- `unsupported` form: **279 passed, 0 failed**. Two Windows cases of defect 2053 were skipped
  (`platform_skips.hero`). These 279 include 52 of the 54 `#~ ffi_header_refused` cases; the other
  2 are those skipped Windows cases, unrun.
- `fixedbugs-563`: exit 1 with both compilers, message byte-identical.
- `run`, filtered to 2036, 2044 and 569: 4 + 1 + 2 passed, 0 failed. Defect 2052 has no `run` case:
  the harness says *no case of run matches fixedbugs-2052*, so the brief's "2052's run cases" do
  not exist.
- `full`: 38 passed. The compiler's own tests: **1637 passed**, after one test was restated. Two
  assertions in `declared_after`'s 2049 test said that a header declaring `h_femit_eight` or
  `h_other_main` is the compiler's. Under C it is the header's, because it declared a name in the
  compiler's forms. **This is the one place where O+C tells a different party than today does, on
  purpose.** The author should read it as such.
- **The six reproducers** (`hs`, `i64`, `e_fabs`, `imp`, `e_std`, `j_typo`): exit 1 and messages
  byte-identical to today's (`cmp`).
- **47 header shapes**: the 4 reproducers, the 16 of `scratch-typed-inspection/rep/2045-shapes`,
  and 27 I wrote. **39 give byte-identical messages; 8 move from exit 2 to exit 1**, and on each of
  those 8 today's compiler prints *internal error: compiling the generated C failed*:

| shape | the header's line | why today's readers miss it |
|---|---|---|
| x1 | `int main(void);` | the error is in the program's C, the note in the header |
| x3 | a call to `hero_str_len(` split over two lines | `runtime_name.called` reads only one quoted line |
| x6 | `void hero_extern_refs(int);` | an emitted name that `emitted` does not know |
| x10 | `HERO_STR_LIT(8)` misused | an expansion note in the runtime |
| x15 | `int main;` | as x1 |
| x16 | `#define HERO_TU_LOCAL static` | the emitter's own macro, redefined |
| y15 | `#pragma GCC poison int64_t` | the error is in `<stdint.h>`, **with no note naming any header** |
| y18 | `static int main(void) {...}` | as x1 |

y15 is the shape that only a clang run can settle. No text in clang's output names the header, and
the opening probe names it anyway.

## W5. The census

`git grep -l -E '^[[:space:]]*extern ' 8c8783ac7 -- '*.hero' ':!docs'` lists **878** programs
(1,583 with `docs/`). I built every one with `./heroes` and with the prototype, each run bounded by
`timeout 180` on the binary, and compared the exit code and the whole message. Before comparing I
replaced the unit's digest directory, the `-o` path and the trial-unit digest with fixed strings.

- **Census 3, the final prototype: 878 compared, 0 moved.** 333 build at exit 0 with both, 545 are
  refused at exit 1 with both, and none hits exit 2 with either compiler.
- **Census 2 caught my own regression, so it was not wasted.** The second prototype asked O inside
  `told_header`, ahead of `ffi.explain`. There it took **18 missing headers** (`'x.h' file not
  found`, which today is `ffi_missing_header`; examples include run cases 437 and 589 and sixteen
  `surface-fixtures`) and told them as `ffi_header_refused`. The repair was to ask O last. That
  ordering is the rule: **O must never preempt a class that already exists.**
- The census never reaches the units that probe a header on their own (`two_ways_ask`,
  `label_dump`, `defined_twice`, `reads`, `header_alone`). O+C leaves them untouched, because it is
  stated only on the refused program unit that `whose.of_refusal` reads. Their own paths were not
  re-measured (unrun).

## W6. False blame in both directions, run

I put negative controls through compilers with simulated defects, each built from a copy of
`selfhost` (today's tree, and today's tree with O+C) carrying one injected defect.

- **NC2, an emitter defect in the program's C whose note names the header.** `emit/extern_refs`
  was made to write `long long <bound>(int);` for every bound function. Today's compiler and O+C
  both answer **exit 2, internal error**, on a correct header and on `examples/sqlite/main.hero`.
  C does not take the name, because it is bound.
- **NC3, a defect in the region's policy.** `"-Wunused-function"` was added to `KEPT`. A correct
  header with an unused `static` function is told **`ffi_header_refused` at exit 1 by both**, with
  the `-Werror` note. This residual is shared by A, B, O, C, E and H: any refusal the compiler's
  own pragmas cause inside a header's region is told as the header's. The `-Werror` note names the
  policy. Defect 2036's known costs are this residual, accepted by the author's ruling.
- **NC1, a runtime defect.** A copy of the runtime (`HEROES_RUNTIME`) declares `int abs(long x);`
  in units only, and a correct header includes `<stdlib.h>`. **Today's compiler tells this at exit
  1 as the header's, with `fix (guess): rename 'abs' in '<SDK>/_stdlib.h', a name the Heroes
  runtime declares first in every unit`**, a false message written by `runtime_name.declared`. O+C
  prints the identical message, because A's reader answers before O is asked. If the readers were
  deleted, O would also say the header's. Every route shares this residual, and only the namespace
  test in W3 closes it. There is no case today: the 192-name measurement in W3 shows the runtime
  declares only reserved forms and C's own ten typedefs.
- **The other direction**, author mistakes told as the compiler's: today there are 8 (W4), and
  under O+C there are 0 of the 47 shapes.

| route | x1/x6/x15/x16/y18 (program's C) | x3/x10 (header's code, note in the runtime) | y15 (no note) | NC1 | NC2 | NC3 |
|---|---|---|---|---|---|---|
| A, today | compiler's ✗ | compiler's ✗ | compiler's ✗ | header's ✗ | compiler's ✓ | header's ✗ |
| B, error location | compiler's ✗, unless C | header's ✓ | compiler's ✗ | header's ✗ | compiler's ✓ | header's ✗ |
| O + C | header's ✓ | header's ✓ | header's ✓ | header's ✗ | compiler's ✓ | header's ✗ |

## W7. The counterfactual: route O on the compiler as it stood before the night

I patched O alone, without C and without the night's readers, into `08230cbfa`'s `selfhost`
(+168/-9) and built it with this tree's seed. `git diff --stat 08230cbfa 8c8783ac7 -- runtime/`
prints nothing. I compared it with `08230cbfa`'s own seed:

- **47 shapes: 30 at exit 2 before, 6 at exit 1, 11 at exit 0; O alone moves 23 of the 30 to exit 1**, the 17 others unchanged. That covers every shape
  in the opening, including all of those the night's four reader modules were written for (`hs`,
  `i64`, `e_fabs`, `e_std`, `j_typo`, `i_call`, `k_outer`...), plus x3, x10 and y15, which today's
  tree still misses. O alone leaves 7 shapes at exit 2. `imp` is defect 2052's link failure, fixed
  by the 16-line region flag. The other six are the program-C collisions (`f_emit`, x1, x6, x15,
  x16, y18), and C fixes them.
- **The night's 23 golden cases** (2044 5, 2045 5, 2049 8, 2051 2, 2052 3): the pre-night compiler
  answers 22 internal errors and 1 build at exit 0. **With O, 18 become `ffi_header_refused` at
  exit 1.** Two stay exit 2: 2049's function's C name and `main`'s C name, which are C's to fix.
  The three left over are 2052's: two link failures and one build at exit 0, fixed by the region
  flag. **So about 168 + 60 + 16 lines would have reached the exit codes that the night reached
  with 1,506.**
- **What the other ~1,260 lines bought is wording, and the wording is better.** O's generic notes
  read "the Heroes runtime's `heroes_runtime.h` line 78" where 2044's says "C's own `int64_t`".
  For `e_fabs` they repeat clang's "included multiple times" twice, with a path, where 2051 says
  "clang itself declares first". For `e_std`, my crude `unplaced` prints the SDK's absolute path
  (an issue-2047 shape) where 2049 says `<math.h>` and cites C11 7.1.3. **So the readers stay, as
  writers of wording. Deleting them would make messages worse, and the brief forbids that.** The
  change O makes to the ceiling is forward-looking. A shape found after O lands is already told at
  exit 1, so it is an `improvement` (a message that could be more exact) and not a `blocking`
  defect (an exit 2 where the author can be told, by `.claude/rules/verification.md` § Bounded
  discovery). Its wording waits in the backlog instead of jumping the queue.

## W8. The routes priced, A to K

"Built" means it was built and run in my copy. "Estimated" means it was not.

- **A**: 1,669 lines so far (W1). Each shape cost 181 to 418 lines with its tests. The 8 shapes in
  W4 are open, and NC1 is a false message it writes today. **Vetoed on §1.1.**
- **B, the first error's location decides**, estimated −10 to −20 code lines. It is O without the
  probe: the two compiler-file refusals in `clang_first` lifted, plus generic notes for compiler
  files. It misses y15, since the location is `<stdint.h>` and no note names a header, and the
  program-C shapes unless C is added. It shares NC1 and NC3. **O dominates B on every shape I ran**,
  for about 40 more lines and a probe that runs only on the failure path.
- **C, reserved names**: the prefix test plus `kept` (built, inside the +296) and a spec sentence
  (the spec-warden prices it; its real token count needs a paid `--refresh`, which I could not
  run). On its own C decides only collisions, not x3, x10 or y15. **Adopted with O.**
- **D, isolation through a shim per group**, estimated 800 to 1,500 lines. The work: a second kind
  of unit in `cli/units`, wrappers generated in `emit/` for every bound function, constant and
  record by value, the cache key, the link, 308 emissions with a group re-blessed (the critic's count), and the seed. A
  header's `static inline` function stops inlining into the program's unit. It removes collisions
  and nothing else. **Refused on cost.**
- **E, a bare-header probe**: estimated 40 to 60 lines. Both outcomes are the header's, so it
  shapes the message and decides no blame. It would separate "C refuses it" from "this compiler's
  region refuses it" (NC3) as a message, not a verdict. Not adopted.
- **F, renaming into C's reserved space**, measured scope: 4,591 `hero_`/`Hero`/`HERO_` tokens in
  `runtime/`, 820 string literals in 148 `selfhost` files, 605 emissions and a 57 MB seed. C11
  7.1.3 reserves `__x` and `_X` to the C implementation, which Heroes is not, so the compiler would
  become the trespasser. **Refused.**
- **G, structured diagnostics**: estimated 300 to 500 lines for a reader of LLVM's bitstream
  format, since `--serialize-diagnostics` is what nothing in the compiler reads. The critic found
  SARIF unstable on Apple clang 21; I did not run it again. G reads the same facts in another
  format and decides no blame. **Refused, orthogonal.**
- **H, the runtime after the headers.** I compiled each of the 220 tracked `run` and `examples`
  headers after today's prefix and before it (`#define HERO_STANDARD_AFTER_GROUPS 1`, guard, header,
  guard close, `heroes_unit.h`, `hero_os.h`, `heroes_standard.h`, syntax only, no region pragmas).
  **18 compile today and fail under H**: 6 on `bool` (`144`, `353`, `361` twice, the fixed-array
  case, `handle-success`), 7 on the runtime's `HeroStr`, `HeroMapHeader` or `HeroArrayHeader`
  (`245`, `314` three times, `462` three times), 1 on `expected identifier` (`245`), and 4 on
  defect 590's `__FLT_EVAL_METHOD__`. These are `run` programs that build and run today. **Vetoed
  on §1.12: correct programs refused.** The re-ordering would also move every emission with a
  group.
- **I, the unit without the header**: estimated 300 to 600 lines in `emit/`. Stand-ins cannot
  exist for a record held by value or a constant whose value only the header knows. Its question,
  whether the program's C fails without the header, is what `declaration()` and C already answer
  (NC2 stays exit 2). **Refused on cost.**
- **J, scanning a header's names**: 30 lines of Python did it for the runtime (W3). Its right home
  is the namespace test, not a step of every build. **Adopted as that test.**
- **K, a message without blame**: folded into O's wording ("the unit's opening compiles without it
  and is refused with it"). The exit code stays 1, as `.claude/rules/cli-surface.md` requires.

## W9. Windows, issues 2047 and 2054, the spec

- **Windows: unrun.** O's verdict is clang's exit code on the opening, so I infer that it reads the
  same on all three platforms. That is an inference. Defect 2053's words then shape the message and
  no longer decide blame. `order_trial.compiled` is the same function panel 204's search already calls,
  so no new kind of clang run is added; whether the Windows leg exercises it is unrun. The two 2053 cases were skipped here.
- **Issue 2047**: O names the compiler's files by their name and never by path. My `outside` and
  `unplaced` still print a C header's path the way clang gives it, which is 2047's shape in new
  code. A landing should use `declared_after.included` (`<math.h>`), as `unplaced` on this tree
  already does. **2054**: on the region's axis, so O+C does not touch it.
- **The spec** owes one sentence for C, which the spec-warden prices: the forms, `main`, and "a
  header that declares one is refused as its own". It owes none for O, which changes no verdict
  that the spec states.

## What I could not run

- The Windows box and Linux arm64 for the prototype: the 2053 cases and both platforms' census.
- The real token cost of the spec sentence, which needs a paid `--refresh`.
- The namespace test (estimated only).
- The probe units' own refusal paths.
- Timing of O's two clang runs on the failure path. Nothing was timed, by the brief's rule.

## Appendix: the prototype, `git diff 8c8783ac7 -- selfhost` in my copy

```diff
diff --git a/selfhost/cli/clang_first.hero b/selfhost/cli/clang_first.hero
index b8ad85ea8..6c3bed4a8 100644
--- a/selfhost/cli/clang_first.hero
+++ b/selfhost/cli/clang_first.hero
@@ -68,6 +68,14 @@ record Reading
     note: Attached
 
 function reading(stderr: str, s: source.Source, decls: [ast.Decl], runtime: str) -> Reading?
+    return read_as(stderr: stderr, s: s, decls: decls, runtime: runtime, involved: false)
+
+# Panel 214's route O, a prototype: where the unit's opening is refused with
+# its groups' headers and compiles without them (`cli/whose.hero`), the
+# headers are what clang refused, so a file of the compiler's own after the
+# group in the stack, or a note in one, no longer makes it the compiler's:
+# the note is kept and told as the compiler's file it stands in.
+function read_as(stderr: str, s: source.Source, decls: [ast.Decl], runtime: str, involved: bool) -> Reading?
     rows = ffi_lookup.lines(text: stderr)
     first = first_error(rows)?
 
@@ -97,7 +105,7 @@ function reading(stderr: str, s: source.Source, decls: [ast.Decl], runtime: str)
     hop: i64 @= at
 
     while hop < steps.len()
-        if group_file.compilers(path: steps[hop].path, compiled: compiled, runtime: own)
+        if !involved && group_file.compilers(path: steps[hop].path, compiled: compiled, runtime: own)
             return fail(code: "none", msg: "a file of the compiler's own")
         hop @ hop + 1
 
@@ -110,7 +118,8 @@ function reading(stderr: str, s: source.Source, decls: [ast.Decl], runtime: str)
         standard: no_name,
         runtime: no_name,
         called: no_name,
-        suggested: no_name
+        suggested: no_name,
+        own: no_name
     )
     notes: [Attached] @= []
     under = attached(rows, after: first.row)
@@ -123,8 +132,13 @@ function reading(stderr: str, s: source.Source, decls: [ast.Decl], runtime: str)
             explained = explaining(rows: rows, first: first, said: said, note: note, runtime: own)
 
             if explained.is_err()
-                return fail(code: "none", msg: "a note in a file of the compiler's own")
-            notes @ notes.push(explained.must())
+                if !involved
+                    return fail(code: "none", msg: "a note in a file of the compiler's own")
+                kept: Attached @= note
+                kept.own @ ok(own_file(path: note.path, compiled: compiled))
+                notes @ notes.push(kept)
+            else
+                notes @ notes.push(explained.must())
         else
             notes @ notes.push(note)
 
@@ -278,6 +292,21 @@ record Attached
     runtime: str?
     called: str?
     suggested: str?
+    own: str?
+
+# A file of the compiler's own as it is told, never by its path (issue
+# 2047): the C this compiler wrote for the program, or a file of the
+# runtime's by its name (panel 214's route O, a prototype).
+function own_file(path: str, compiled: str) -> str
+    if group_file.settled(path) == group_file.settled(compiled)
+        return "the C this compiler writes for this program"
+    p = clang_place.slashed(path)
+    cut: i64 @= p.len()
+
+    while cut > 0 && p[cut - 1] != '/'
+        cut @ cut - 1
+
+    return "the Heroes runtime's `" + p.slice(from: cut, to: p.len()) + "`"
 
 # The notes clang wrote under the error, up to the next diagnostic that is
 # not a note or clang's count of what it reported. A note's own includes
@@ -299,7 +328,7 @@ function attached(rows: [str], after: i64) -> [Attached]
             if !place.is_err()
                 at = clang_place.offset_of(text: line, needle: NOTE).must()
                 out @ out.push(
-                    Attached(path: place.must().path, row: place.must().row, text: line.slice(from: at + NOTE.len(), to: line.len()), index: index, standard: no_name, runtime: no_name, called: no_name, suggested: no_name)
+                    Attached(path: place.must().path, row: place.must().row, text: line.slice(from: at + NOTE.len(), to: line.len()), index: index, standard: no_name, runtime: no_name, called: no_name, suggested: no_name, own: no_name)
                 )
         index @ index + 1
 
diff --git a/selfhost/cli/declared_after.hero b/selfhost/cli/declared_after.hero
index ebc3296d2..7e6d66433 100644
--- a/selfhost/cli/declared_after.hero
+++ b/selfhost/cli/declared_after.hero
@@ -80,19 +80,68 @@ function told(stderr: str, s: source.Source, decls: [ast.Decl], runtime: str) ->
         )
         d.fixes @ [renamed(s, line: line, why: "a name of C's own library, which `" + library.must() + "` declares")]
         return ok(d)
-    decl = emitted(steps, name: line.name, s: s, decls: decls)?
-    called = "this program's function `" + source.span_text(s, decl.name) + "`"
+    decl = emitted(steps, name: line.name, s: s, decls: decls)
+
+    if decl.is_err()
+        return kept(steps, line: line, s: s, decls: decls, message: first.message)
+    called = "this program's function `" + source.span_text(s, decl.must().name) + "`"
     d: diag.Diagnostic @= opened(
         line: line,
         by: "the C this compiler writes for this program",
         message: first.message
     )
     d.notes @ d.notes.push(
-        EMITTED + line.name + "` is the name the C it writes gives " + called + ", at " + source.elsewhere(s, here: line.group.span.start, there: decl.name.start)
+        EMITTED + line.name + "` is the name the C it writes gives " + called + ", at " + source.elsewhere(s, here: line.group.span.start, there: decl.must().name.start)
     )
     d.fixes @ [renamed(s, line: line, why: "the name this compiler gives " + called)]
     return ok(d)
 
+# Panel 214's route C, a prototype: a name of the forms this compiler keeps to
+# itself, `h_*`, `hero_*`, `Hero*`, `HERO_*`, `HEROES_*` and `main`, which the
+# C it writes for the program declares and a group's header declares too,
+# where no group binds it: the header's, told as the reserved name it is. A
+# name of no such form stays the compiler's, at exit 2, so an emitter that
+# wrote a name outside its own is never told as the header's.
+function kept(steps: [clang_place.Place], line: Line, s: source.Source, decls: [ast.Decl], message: str) -> diag.Diagnostic?
+    if steps.len() != 1 || !group_file.group_of(s, decls, path: steps[0].path).is_err()
+        return fail(code: "none", msg: "the error lies in no file clang compiled")
+
+    if !reserved(line.name) || bound(name: line.name, s: s, decls: decls)
+        return fail(code: "none", msg: "no name this compiler keeps to itself")
+    d: diag.Diagnostic @= opened(line: line, by: "the C this compiler writes for this program", message: message)
+    d.notes @ d.notes.push(
+        EMITTED + line.name + "` is a name the C it writes for every program declares, of the forms it keeps to itself: `h_`, `hero_`, `Hero`, `HERO_`, `HEROES_` and `main`"
+    )
+    d.fixes @ [renamed(s, line: line, why: "a name this compiler keeps to itself")]
+    return ok(d)
+
+function reserved(name: str) -> bool
+    if name == "main"
+        return true
+
+    for head in ["h_", "hero_", "Hero", "HERO_", "HEROES_"]
+        if name.len() > head.len() && standard_name.starts(text: name, head: head)
+            return true
+
+    return false
+
+# Whether a group of the program binds `name`, which then keeps its C name.
+function bound(name: str, s: source.Source, decls: [ast.Decl]) -> bool
+    for decl in decls
+        match decl.kind
+            .function_decl fd =>
+                if !fd.value.header.is_err() && source.span_text(s, decl.name) == name
+                    return true
+            .constant_decl k =>
+                if !k.header.is_err() && source.span_text(s, decl.name) == name
+                    return true
+            .record_decl rec =>
+                if !rec.header.is_err() && source.span_text(s, decl.name) == name
+                    return true
+            .variant_decl | .test_decl => continue
+
+    return false
+
 # The headline, on the group's string, and the header's line as clang quoted
 # it.
 function opened(line: Line, by: str, message: str) -> diag.Diagnostic
@@ -270,9 +319,16 @@ test "a header declaring this compiler's name for a function of the program's is
     assert said.fixes[0].title == "rename `h_femit_main` in `f_emit.h`, the name this compiler gives this program's function `main`"
     assert taken(said).must().whose == "which this compiler gives one of the program's own functions"
 
-    # The compiler's: a name no function of the program's own is given, an
-    # error under an include, and a group's own name.
-    for wrong in ["h_femit_eight", "h_other_main", "eight"]
+    # Panel 214's route C, a prototype: a name of the compiler's own forms no
+    # function of the program's own is given is the header's too, a reserved
+    # name; a group's own name, and a name of no such form, stay the
+    # compiler's, as does an error under an include.
+    for reserved_name in ["h_femit_eight", "h_other_main"]
+        line = "femit.c:95:6: error: conflicting types for '" + reserved_name + "'\n   95 | void " + reserved_name + "(void);\n      |      ^\n"
+        written = "In file included from build/tu-1/femit.c:20:\n./f_emit.h:1:5: note: previous declaration is here\n    1 | int " + reserved_name + "(int x);\n      |     ^\n"
+        assert !told(stderr: line + written, s: s, decls: decls, runtime: "runtime").is_err()
+
+    for wrong in ["eight", "femit_main"]
         line = "femit.c:95:6: error: conflicting types for '" + wrong + "'\n   95 | void " + wrong + "(void);\n      |      ^\n"
         written = "In file included from build/tu-1/femit.c:20:\n./f_emit.h:1:5: note: previous declaration is here\n    1 | int " + wrong + "(int x);\n      |     ^\n"
         assert told(stderr: line + written, s: s, decls: decls, runtime: "runtime").is_err()
diff --git a/selfhost/cli/header_refused.hero b/selfhost/cli/header_refused.hero
index f6be9b6e3..6d8ea052f 100644
--- a/selfhost/cli/header_refused.hero
+++ b/selfhost/cli/header_refused.hero
@@ -73,6 +73,7 @@ use cli/clang_excerpt
 use cli/declared_after
 use cli/builtin_name
 use emit/ffi_build
+use cli/runtime_key
 use parse
 use resolve
 use checker
@@ -125,6 +126,112 @@ function refused(stderr: str, s: source.Source, decls: [ast.Decl], runtime: str)
         ]
     return [told(s, rows: one.rows, first: one.first, steps: one.steps, at: one.at, group: one.group, notes: one.notes)]
 
+# Panel 214's route O, a prototype: the unit's opening was refused with its
+# groups' headers and compiled without them (`cli/whose.hero` ran the two), so
+# the headers are what clang refused, whatever file of the compiler's own its
+# first error's stack or its notes reach. Told on the group whose header the
+# error lies in, as `refused` tells it; or, where clang's first error lies in
+# a file every unit reads before or after the groups, on the group whose
+# header one of its notes stands in (`outside`).
+function involved(stderr: str, s: source.Source, decls: [ast.Decl], runtime: str) -> [diag.Diagnostic]
+    read = clang_first.read_as(stderr: stderr, s: s, decls: decls, runtime: runtime, involved: true)
+
+    if read.is_err()
+        return outside(stderr: stderr, s: s, decls: decls, runtime: runtime)
+    one = read.must()
+    two = sides(one)
+
+    if !two.is_err()
+        return [
+            headers_together.told(s, one: two.must().one, other: two.must().other, message: one.first.message, added: two.must().added)
+        ]
+    return [told(s, rows: one.rows, first: one.first, steps: one.steps, at: one.at, group: one.group, notes: one.notes)]
+
+# clang's first error in a file of the compiler's own, or of C's own read
+# through one, with a note in a group's header: told on that group.
+function outside(stderr: str, s: source.Source, decls: [ast.Decl], runtime: str) -> [diag.Diagnostic]
+    none: [diag.Diagnostic] = []
+    rows = ffi_lookup.lines(text: stderr)
+    first = clang_first.first_error(rows)
+
+    if first.is_err()
+        return none
+    own = runtime_key.made_of(runtime)
+
+    if own.is_err()
+        return none
+    steps = clang_place.stack(stderr: stderr, line: rows[first.must().row])
+    compiled = steps[0].path
+
+    for note in clang_first.attached(rows, after: first.must().row)
+        if clang_first.expansion(note.text)
+            continue
+        reached = group_file.arriving(
+            stderr: stderr,
+            row: rows[note.index],
+            s: s,
+            decls: decls,
+            compiled: compiled,
+            runtime: own.must()
+        )
+
+        if reached.is_err()
+            continue
+        group = reached.must().group
+        place: str @= ""
+
+        if group_file.compilers(path: first.must().place.path, compiled: compiled, runtime: own.must())
+            place @ clang_first.own_file(path: first.must().place.path, compiled: compiled)
+        else
+            place @ "`" + clang_place.shown(path: first.must().place.path, directory: "") + "`, a header of C's own every unit reads after its groups' headers,"
+        d: diag.Diagnostic @= diag.new_diagnostic(
+            code: "ffi_header_refused",
+            message: "`" + group.header + "`, the header this group names, does not compile in this program's unit: clang refuses " + place + " at line " + first.must().place.row.to_str() + ", after it: " + first.must().message,
+            span: group.span
+        )
+        quoted = clang_excerpt.of(rows, after: note.index)
+
+        if !quoted.is_err()
+            d.notes @ d.notes.push(
+                "`" + group.header + "` line " + note.row.to_str() + " reads `" + quoted.must() + "`, where clang adds: " + note.text
+            )
+        d.notes @ d.notes.push(REPAIR)
+        return [d]
+
+    return none
+
+# A header the unit's opening is refused with, alone, where clang's words name
+# no group's header (`cli/whose.hero`'s `alone_refused`): told on its group,
+# with clang's first error and the file of the compiler's own, or of C's own
+# read through one, it lies in.
+function unplaced(stderr: str, s: source.Source, decls: [ast.Decl], runtime: str, group: group_file.Group) -> [diag.Diagnostic]
+    none: [diag.Diagnostic] = []
+    rows = ffi_lookup.lines(text: stderr)
+    first = clang_first.first_error(rows)
+
+    if first.is_err()
+        return none
+    own = runtime_key.made_of(runtime)
+
+    if own.is_err()
+        return none
+    steps = clang_place.stack(stderr: stderr, line: rows[first.must().row])
+    compiled = steps[0].path
+    place: str @= "`" + clang_place.shown(path: first.must().place.path, directory: "") + "`"
+    library = declared_after.included(steps, s: s, decls: decls, runtime: own.must())
+
+    if !library.is_err()
+        place @ "`" + library.must() + "`, a header of C's own every unit reads after its groups' headers,"
+    else if group_file.compilers(path: first.must().place.path, compiled: compiled, runtime: own.must())
+        place @ clang_first.own_file(path: first.must().place.path, compiled: compiled)
+    d: diag.Diagnostic @= diag.new_diagnostic(
+        code: "ffi_header_refused",
+        message: "`" + group.header + "`, the header this group names, does not compile in this program's unit: the unit's opening compiles without it and is refused with it, where clang refuses " + place + " at line " + first.must().place.row.to_str() + ": " + first.must().message,
+        span: group.span
+    )
+    d.notes @ d.notes.push(REPAIR)
+    return [d]
+
 # The group whose header clang refused on its own, where it refused one group's
 # header and no note names another's: the header `cli/header_alone.hero` asks
 # whether it compiles alone (defect 557).
@@ -273,6 +380,9 @@ function sides(read: clang_first.Reading) -> Sides?
 # and so at a function of the runtime's the refused line calls, or a name of
 # the runtime's clang suggests in it (defect 2049).
 function adds(note: clang_first.Attached, directory: str) -> str
+    if !note.own.is_err()
+        return "clang adds, at " + note.own.must() + " line " + note.row.to_str() + ", which every unit of this program reads: " + note.text
+
     if !note.standard.is_err()
         return "clang adds, at C's own `" + note.standard.must() + "`, which every unit of this program declares as `<stdint.h>` does, before any group's header: " + note.text
 
diff --git a/selfhost/cli/whose.hero b/selfhost/cli/whose.hero
index c6092e56d..083677724 100644
--- a/selfhost/cli/whose.hero
+++ b/selfhost/cli/whose.hero
@@ -25,6 +25,9 @@ use cli/name_order
 use cli/inline_only
 use cli/renamed
 use cli/group_file
+use cli/order_trial
+use emit/c_text
+use emit/ffi_lookup
 use diag
 use cli/deep_types
 use cli/link
@@ -113,6 +116,18 @@ function of_refusal(failed: ()?, art: compile.Artifact, s: source.Source, t: too
             _ = compile.stage(lacked, s).is_err()
             return fail(code: "diagnostics", msg: "a package names a library this machine does not have")
 
+    # Panel 214's route O, a prototype, asked last so that every class above
+    # keeps its word: what no reader of clang's words named is the headers'
+    # where the unit's opening is refused with them and compiles without
+    # them, whatever file of the compiler's own clang's first error or its
+    # notes reach.
+    if code != link.REFUSED
+        late = opening_told(message, art, s, t, under: words, unit_headers: unit_headers)
+
+        if late.len() > 0
+            _ = compile.stage(late, s).is_err()
+            return fail(code: "diagnostics", msg: "a header this program binds does not compile")
+
     # A unit clang refused carries its own path inside the message
     # (`units.tu_object`), the fused test unit's included.
     io.eprint(text: "internal error: " + message)
@@ -208,6 +223,81 @@ function told_header(message: str, art: compile.Artifact, s: source.Source, t: t
     _ = compile.stage(told, s).is_err()
     return true
 
+# The headers' refusal by route O, or none: the unit's opening refused with
+# them and compiled without them, told on the group clang's words name, or on
+# the first header whose opening alone is refused.
+function opening_told(message: str, art: compile.Artifact, s: source.Source, t: toolchain.Toolchain, under: compiling.Compiling, unit_headers: [str]) -> [diag.Diagnostic]
+    none: [diag.Diagnostic] = []
+
+    if !opening_involves(t, under: under, unit_headers: unit_headers)
+        return none
+    refusal: [diag.Diagnostic] @= header_refused.involved(stderr: message, s: s, decls: art.decls, runtime: t.runtime)
+
+    if refusal.len() == 0
+        refusal @ alone_refused(message, art, s, t, under: under, unit_headers: unit_headers)
+    return refusal
+
+# Whether the unit's opening (`cli/order_trial.hero`), its groups' headers
+# between the compiler's own and C's, is refused under the unit's words, and
+# the same opening with none of those headers compiles: then the headers are
+# what clang refused, and never the C this compiler wrote for the program.
+function opening_involves(t: toolchain.Toolchain, under: compiling.Compiling, unit_headers: [str]) -> bool
+    theirs = order_trial.theirs(unit_headers)
+
+    if theirs.len() == 0
+        return false
+    words = compiling.unit_words(under)
+    text = order_trial.opening(theirs, question: "")
+    with = order_trial.compiled(t, words: words, text: text)
+
+    if !with.asked || with.compiles
+        return false
+    without = order_trial.compiled(t, words: words, text: without_headers(text: text, headers: theirs))
+    return without.asked && without.compiles
+
+# The opening's text with its groups' include lines taken out and nothing
+# else: the region, the guard and the checks raised after it are kept, so the
+# one difference clang is asked about is the headers.
+function without_headers(text: str, headers: [str]) -> str
+    taken = c_text.includes(headers)
+    kept: [str] @= []
+
+    for line in ffi_lookup.lines(text: text)
+        out: bool @= false
+
+        for one in taken
+            if line == one
+                out @ true
+
+        if !out
+            kept @ kept.push(line)
+
+    return kept.join("\n") + "\n"
+
+# Where no file clang names is a group's header and no note stands in one,
+# `#pragma GCC poison int64_t` read before `<stdint.h>`: the first of the
+# unit's own headers whose opening, alone, is refused.
+function alone_refused(message: str, art: compile.Artifact, s: source.Source, t: toolchain.Toolchain, under: compiling.Compiling, unit_headers: [str]) -> [diag.Diagnostic]
+    none: [diag.Diagnostic] = []
+    words = compiling.unit_words(under)
+
+    for header in order_trial.theirs(unit_headers)
+        ran = order_trial.compiled(t, words: words, text: order_trial.opening([header], question: ""))
+
+        if ran.asked && !ran.compiles
+            group = group_file.group_of(s, art.decls, path: header)
+
+            if !group.is_err()
+                return header_refused.unplaced(
+                    stderr: message,
+                    s: s,
+                    decls: art.decls,
+                    runtime: t.runtime,
+                    group: group.must()
+                )
+
+    return none
+
 # The message of a failed `()?`.
 function failure_message(failed: ()?) -> str
     match failed
```

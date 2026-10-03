# Panel 188, the compiler-engineer's brief

Read `00-shared.md` and `00-facts.md` in this directory first; they bind you.
You judge the implementation cost and the core-or-sugar question of each
route (your charter, `.claude/agents/compiler-engineer.md`), and you BUILD.

## Where to look (at `826ddc2f`, by `grep -n` and `wc -l`)

- `selfhost/parse/group_head.hero` (164 lines): `header` at line 30 takes the
  string and calls `machine_locked` (comment from line 60, function at line
  72), panel 055's refusal of an absolute path, which reads the unquoted value
  through `module_text.unquote` and reports `machine_locked_path` with no fix.
  The same function judges a `link` string (`what` is `"link"` there).
- `selfhost/emit/c_text.hero`: `includes` at line 99, the `#include <...>`
  lines, with defect 207's line splice for a trigraph.
- `ffi_missing_header`: `selfhost/emit/ffi_build.hero:205` and
  `selfhost/emit/header_reach.hero:96`.
- The parser's budget: `tests/harness/suite_layout.hero` line 490,
  `"selfhost/parse/ 8696"`; the mirror of its count says 8,689 used (F3).

## What to build, in your copy `<scratchpad>/188-compiler-engineer/`

1. Reproduce F1 and F6 on your compiler (all thirteen names).
2. Build the route you judge the most robust and complete of Q1 (by the
   coordinator's reading, (1b) or (1c); judge that yourself): a refusal at
   `check`, on the `extern`'s own line, of a header name holding what the
   route forbids, with its message, its code and its fix's certainty (Q2).
   Where C's own spelling `<name>` is carried into the string (F6), say
   whether the fix that drops the brackets is `certain` by the rule of
   `.claude/rules/diagnostics-and-goldens.md` (*a `certain` fix repairs the
   defect the diagnostic names*).
3. Rebuild, then run: every F1 and F6 name; the `check` golden form whole
   (`./heroes run tests/harness/main.hero -- ./heroes check`); `annotations`
   and `fixes` narrowed to your new cases; the compiler's own tests; `layout`
   WHOLE (its `budget` and `concat` checks are asked only when it runs whole).
   Write your cases as `tests/golden/check/` files in your copy, each
   diagnostic annotated in its source (`#~ <code>`).
4. The budget: where your lines go, how many land in `selfhost/parse/`
   against the 7 of room, and whether the route must move lines out, live
   elsewhere, or raise the row with its reason (`layout/budget`'s message).
5. Q5 on your compiler: a `link` and a `package` string holding each of F1's
   characters, what `check` and `build` do.

Report per route: built or not, its lines (by the suite's unit), what it
refuses that a real program needs (if any), the cases run and their counts,
and the condition that would change your verdict.

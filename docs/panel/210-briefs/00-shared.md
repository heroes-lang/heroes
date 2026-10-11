# Panel 210, shared brief: an `@` parameter the callee never writes

Convened 2026-10-11 at about 02:10 by the coordinator (session heroes-lang-98)
on the author's choice at about 02:07 (the question widget, *Seduta 210
stanotte*): a lean sitting, the compiler-engineer, the spec-warden and the
historian, one completeness critic pass after the reports, no blind seat and
no paid run; the resolution adopted provisionally and lands tonight, the
author ratifying in the morning. What the lean sitting gives up: a
measurement of what a reader of the spec expects.

## The question

The open decision `issues/2026-10/10/2026-10-10-1737-an-at-parameter-the-callee-never-writes-is-accepted.md`
(panel 209's completeness critic, second pass, § 3 of
`docs/panel/209-reports/completeness-critic-pass2.md`): should a function's
`@` parameter that its body never writes (by `@` to the name, a field or an
element, or through an `@` argument) be a compile error with a fix removing
the `@` at the parameter and at every call site, as panel 209's
`never_rebound` refuses a cell nothing re-binds? Its default while open:
accepted, as today. Its recommendation: refuse it under the same rule; the
fix `certain` only where every caller is in the compiled file set, `guess`
otherwise; a parameter written only by a C call through an extern group must
count as written. The critic's probe, `at_param_never_written.hero`
(`function f(@n: i64)` whose body never writes `n`), was accepted at exit 0
and printed 2 (its § 3 table, line 24).

## The tree

Frozen at **`1dd890751`**, batch 20's round (`lane-round-b20`: the trunk at
`42656199`, M-inferred-cell landed, plus batch 20's four lanes), each seat in
its own copy made by `git archive`, under
`.claude/worktrees/scratch-b15/210-<seat>/tree/`. Build your compiler there:
`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes-seed && ./heroes-seed build selfhost/main.hero -o heroes`.
Never read, build or run in another seat's directory, the trunk or a lane. No
file outside `/Users/joseph/Temp/heroes/heroes-lang` (TMPDIR inside your
directory).

## What the spec says now (read by the coordinator at 02:09)

`spec/heroes-spec.md:137-138`: *An unused binding or parameter is a compile
error, and so is a cell nothing re-binds, an `@` argument counting; a read is
a use and a write is not, except through an `@` parameter.* § 9's `@`
parameters are `:271-274` (*Mutable parameters are marked `@` in the
signature and at the call site*). The cell rule's code is panel 209's
`never_rebound` (`docs/panel/209-a-cell-is-declared-by-its-own-symbol-and-typed-by-its-value.md`),
and batch 20's defect 608 extended it to `_ @= e` (lane b20-check).

## Questions every seat answers from its ground

1. Is the class real and how large: the sites in `selfhost/`, `examples/`
   and `tests/` (outside the golden cases that pin it) where an `@`
   parameter is never written, counted by a built rule, not by grep.
2. What counts as a write: `@` to the name, to a field or an element, an
   `@` argument to another call, a lend to C through an `extern` whose
   parameter is a pointer, a `for` over it, a method with `@` self. Name
   every shape and its verdict.
3. The fix: removing `@` at the parameter and at every call site; when is it
   `certain` (every caller in the compiled set; a public function of a
   library module?), when `guess`.
4. The spec: does § 5's sentence already cover it (an unused parameter is an
   error, and a parameter only read through `@` is used), or does it need a
   word, and at what token cost (vendored, `heroes measure`; no
   `--refresh`, it is a paid run).

## Rules every seat follows

- Every number you write is produced by a command you ran, named beside it;
  a negative sentence goes out as a question naming what you searched.
- Write your report as you go to `docs/panel/210-reports/<seat>.md`.
- No paid run, no `pkill` by pattern, no commit, no push. Long commands under
  `caffeinate -i`. Times from `date`. English. No em dashes.
- Verdict per route: approve, object or veto (veto on your seat's ground),
  your recommendation, and one prediction someone can score.
- Aim to finish by 03:15 by `date`.

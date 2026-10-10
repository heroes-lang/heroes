# Panel 203, the shared brief: the inference panel 201 filed apart, and a self-call through a parameter (541, 547)

Written by the coordinator on 2026-10-09 from 22:54 (`date`), on the tree
frozen at `46b80b82` (worktree `lane-panel-203`: batch 17's round, with panel
201's sentence N1f in spec § 9 and its cycle rule landed). Convened on the
author's yes of about 22:47 (*both sittings now, 6 USD between them*), under
the author's instruction of about 19:35, *every defect closed*. Every fact
names its command or file; a number marked **carried** is a report's, a
question for the seat.

**Lane: full**: both questions change what `check` refuses or accepts. The
compiler-engineer, the spec-warden, the historian, the blind seat run by the
coordinator as fresh `claude -p` sessions outside the repository (6 USD shared
with panel 202), the critic before the seats and after them.

## Q1, defect 541: an argument that needs its type from the context

Panel 201 (`docs/panel/201-the-spec-says-a-generic-function-s-parameter-types-no-value-and-a-cycle-that-cannot-end-is-refused.md`,
ratified) put N1f in § 9: *... else from the type the context asks for, and a
generic function's parameter asks for none* (+16 real tokens, defect 488's
landing), and filed this: panel 105's rule (design.md §4.12, *Flat only*)
refuses fourteen probes (the spec-warden's,
`.claude/worktrees/scratch-b15/201-spec-warden/probes/`, read only), a generic
function value, `ok(...)`, `[]`, a literal at a narrower width, a result-only
call, a concrete parameter of a generic function among them. Panel 201's
critic (carried): route I as built (`.claude/worktrees/scratch-b15/201-compiler-engineer/tree/`,
read only) accepts 6 of the 14 and refuses `xss.map(len_of)` and `first(a:
double, b: ident)` it covers; the **general route**, settling the callee's
letters first and checking every argument that needs context against the
substituted parameter, would make § 9 true without N1f, unbuilt and uncosted.
Panel 201's historian: Go dropped the same narrowing in 1.21, shape by shape.
**Should the checker give context to every argument that needs it, at what
cost, with which correct programs gained and which wrong ones still refused,
and does N1f then leave the spec** (its tokens back)?

## Q2, defect 547: a self-call through a parameter

Panel 201's R3 landed (defect 520): a cycle of named functions and a
self-call through a known value (`f = go`) are refused `endless_recursion`.
Still passing (defect 547, narrowed at batch 17): a self-call through a
parameter, `go(n: n, f: step)` inside `step`, aborting 134 at run time. **Can
`check` see it without refusing a correct program** (a parameter whose
callers pass a function with a way out), at what cost, or is the run-time
abort (defect 508's, at every level) the right answer?

## The rules every seat works under

As panel 200's shared brief states them (`docs/panel/200-briefs/00-shared.md`,
§ The rules every seat works under), with your folder
`.claude/worktrees/scratch-b15/203-<seat>/`, your copy rsynced from
`.claude/worktrees/lane-panel-203/`, and **two more**: build your compiler in
three stages, the seed against `541d9595`'s runtime
(`.claude/worktrees/scratch-b15/b17-rt29/runtime`, read only), then `selfhost`
with `HEROES_RUNTIME` pointing there, then `selfhost` again against the copy's
runtime (the seed is ABI 29, the runtime 30); and lane b18-close and panel 202
work beside you: never touch them. The spec-warden prices on the vendored row
written as a lower bound, the pinned real count as the base, no `--refresh`.
**Time box**: report within 60 minutes of starting, the unmeasured said
plainly.

## Corrections and additions from the critic's first pass, binding

Read by 23:06 (`date`): `docs/panel/203-reports/completeness-critic-pass1.md`,
every one from a command it ran, applied before any seat starts and binding
over the text above where they disagree. Read it whole: its repairs 1 to 13
are panel 202's, 14 to 19 panel 203's, 20 both; its unlisted routes and
questions are part of your brief. In short:
- The fourteen probes by file, and two more (`literal2`, `nestedcall`):
  repair 15. Under the general route N1f becomes false and design.md §4.12
  *Flat only* (panel 105) falls with it (repair 16). **Unasked and central**:
  does a literal argument settle a type parameter as `i64` or wait for the
  context (repair 17, `p203/mean/m1-m3`): a literal changing width changes
  meaning, not only accept or refuse.
- Q2's neighbours, measured (repair 18): a self-call through a built-in
  (`[n].map(step)`) and through a record field pass `check` and abort 134; the
  cycle rule leaned on `module_cycle`, and built-ins and a used module's
  higher-order function cross modules. Following a parameter makes the
  verdict at `step` depend on `go`'s body, a locality question (repair 19):
  the blind seat scores it.

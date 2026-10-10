# Panel 203, llm-ergonomist (blind seat): the coordinator's prediction before the readings

Written by the coordinator at 23:49 on 2026-10-09 (`date`), before any of the
sessions starts. Folders `<scratchpad>/readings-203/<label>`, outside the
repository (the author's exception of 2026-10-09; 6 USD shared with panel
202, of which 2.4286 were spent by panel 202). The program is six lines of
generic calls (`ns.map(ident)`, `first(a: b, b: 255)` with `b: u8`,
`g(x: 1, n: ok(2))`, `first(a: xs, b: [])`, `y: u8 = first(a: 1, b: 2)`,
`x = first(a: ok(1), b: ok(2))`); today's checker refuses all six (run by the
coordinator before the sessions). The labels' mapping, never in a folder:

- **e1-a, e1-b**: the trunk's spec, N1f in § 9 (`5b78bba8b56fd363`).
- **e2-a, e2-b**: § 9 with the spec-warden's G3a in N1f's place
  (`5312e78629b9fa93`): *... from the arguments that carry one, else from the
  type the context asks for, else from a literal argument (section 2) ...*

**What I expect.** e1: both readers refuse lines 19, 21, 22 and 25 quoting
N1f, and at least one accepts line 20 or 23 with `u8` by § 2's literal rule.
e2: both accept lines 19, 20 and 22 and refuse line 25; on line 23 at least
one accepts `u8`. **What it would falsify**: an e1 reader accepting line 19;
an e2 reader refusing line 19.

## Q2, added at 00:00 (`date`) before its two sessions start

- **f-n**: `param_value` (`step` hands itself to `go`, which calls its
  parameter on every path), the task *make the program build and do what it
  evidently means*, given the compiler-engineer's prototype refusal (its two
  notes, the first naming `go`'s parameter `f`); **f-m**: the same given
  today's tools, a build at 0 and a run at 134, *stack exhausted in main.step,
  inside the recursion of main.go and main.step*. The trunk's spec in both.
- **What I expect**: both readers put a base case in `step` (a path
  returning before the call); neither edits `go`. **Falsified by** a reader
  who gives `go` the way out, or removes the call.

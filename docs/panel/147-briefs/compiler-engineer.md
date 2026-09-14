# Panel 147 — compiler-engineer

Read `00-shared.md` first. It carries the question, the three routes and the
census. This brief adds what only you are asked for.

**You judge the ceiling** (design.md §1.1, §1.7, Part 5): implementation cost,
and core-versus-sugar. You have a veto on soundness.

## Where to look — and where NOT to

Live compiler, all Heroes:

| what | where |
|---|---|
| the exit sweep, and its six rules | `selfhost/ir/own.hero` (rule 5 is the one that matters here) |
| what a handle IS, the single home of the predicate | `selfhost/handles.hero` |
| generated eq/hash, and where a handle's satellites would go | `selfhost/emit/structural.hero` |
| retain/release generation and the descriptor table | `selfhost/emit/descriptors.hero`, `selfhost/emit/perfn.hero` |
| the slot table and phases | `selfhost/ir/phases.hero`, `selfhost/ir/owning.hero` |
| `consumes`, the nearest landed neighbour, 114 lines | `selfhost/check/consuming.hero` |
| `lease`/`end_lease`, the other landed neighbour | `selfhost/check/leasing.hero` |
| the `owned` freer, which frees at the call | `selfhost/check/freer.hero`, `selfhost/emit/ops.hero` |
| extern member parsing, where a `released` word would land | `selfhost/parse/members.hero` |
| the formatter, which owes every new surface form (CL-036) | `selfhost/print/fmt.hero` |

**Never `crates/`.** That tree is `archive/bootstrap-rs/`, nothing builds it,
and a seat sent there measures a compiler that no longer ships.

**File-length ceilings are live and several of these files are at theirs.**
`tests/harness/suite_layout.hero`'s `DECIDED` table is the list; `emit/ctype.hero`
and `emit/structural.hero` both had their ceilings raised this morning and are
at 388 and 323 against 395 and 335. A route whose cost lands in a file with no
room is more expensive than the same line count elsewhere, and the sitting wants
that said out loud.

## What to measure, not argue

1. **Route A's real cost.** A handle names its releaser; the compiler calls it
   at scope exit on every path. The sweep already exists — so the question is
   whether a handle can be given `_release` and entered in the slot table, or
   whether that machinery assumes refcounting the handle cannot satisfy. Read
   `ir/own.hero` rule 3 and rule 5 before answering: a store increfs before
   decrefing, and a handle has no count. **Say whether the sweep is reusable or
   whether Route A needs a second, parallel mechanism** — that is the hinge of
   this whole sitting and nobody else can answer it.
2. **Route B's real cost**: a `cleanup` statement is a new statement kind, so it
   is a new arm in every exhaustive `match` over the statement enum. **Count
   them**, the way panel 132 counted 68 for `ExprKind`. A count is a
   measurement; an estimate is not.
3. **§1.7's subtraction test** for each route: what, if anything, is REMOVED.
4. **Core or sugar.** If a route is sugar, name what it desugars TO and show it
   compiles. If it is core, say which invariant it changes.
5. **The interaction that will decide correctness**: a handle released at scope
   exit that was also RETURNED, or stored in a record, or copied. `consumes`
   and wart 20 are next door. Compile the shapes rather than reasoning about
   them.

## Prototype discipline

**In a copy.** Panel 054's engineer prototyped in the repository and a language
feature landed before its own panel had ruled. Prototype far enough that the
number is real, then throw it away and report the number.

You do not have to prototype all three routes. Prototype the one whose cost you
are least sure of, and say which you did.

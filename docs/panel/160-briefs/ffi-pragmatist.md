# Panel 160 — brief for the ffi-pragmatist

Read `00-shared.md` first. Your seat is design.md §1.11 (there is no standard
library; everything comes from C) and §4.19. You have a veto on ABI breakage.

## What panel 158 already settled, so you need not

**A fallible has no C ABI at any depth.** You measured it there:
`error[ffi_type]` refuses `T?` as an extern parameter, a result and an extern
record field. So no option on this ballot touches the boundary DIRECTLY, and
you may say so in one line.

## What is yours here, and nobody has run it

The `acquires` / `consumes` ledger tracks a handle through a container — you
proved that at panel 159 with `@dbs[i]` and `dbs[0] @ d`. The question this
sitting adds: **a handle inside a nested fallible.**

```
record Db tag sqlite3 … acquires sqlite3_close
dbs: [Db?] @ …                     # some opens failed
hit = find(dbs, is_open)           # a Db??
if hit.is_err()                    # asks: was anything found — NOT: did that open fail
    …
```

Build it against real `sqlite3.h`, in a copy. Measure, exit codes direct:

1. When `find` returns the outer failure (nothing matched), what does the
   ledger say at `main`'s return? Nothing acquired, so presumably clean —
   confirm.
2. When `find` returns `ok(fail(…))` — a slot that holds a FAILED open — and the
   program tests `.is_err()` on the outer level and takes the "found" branch:
   does it then try to close a handle that was never opened? What exit code, and
   what does `hero_runtime_check_leaks` print?
3. The mirror: a slot holds a SUCCESSFUL open, the program reads `.is_err()` as
   "did the open fail", sees `false`, and never closes it. **Is that a leak the
   ledger reports at 134**, or does the counting miss it? Panel 148's balance
   clause says a handle consumed twice hides one never consumed; say whether this
   shape reaches that.

Run the Linux leg under `--sanitize` if you can reach the container; a leak in a
binding is invisible on the Mac (`.claude/rules/platforms.md`). If you cannot,
UNRUN and name the command.

## The question your seat exists to ask

Of options A through E, which one leaves a binding author with the fewest ways
to release a handle at the wrong level? A refusal that forces `match` puts both
levels on the line. Does a real SQLite pool — `[Statement?]` filled through
out-parameters — ever NEED to test only the outer level, and what would it write
under A?

## Report

`docs/panel/160-reports/ffi-pragmatist.md`. Verdict · section · the programs and
their exit codes · prediction · condition.

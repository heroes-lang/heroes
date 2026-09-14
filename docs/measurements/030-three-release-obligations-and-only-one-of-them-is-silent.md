# 030 — Three release obligations, and only one of them is silent

Date: 2026-09-14 · M-cleanup-verdict step 1 · **the count this milestone may not
argue without, and it falsified two of the three things the milestone said about
its own subject.**

## Why this file exists

`docs/work/milestones/M-cleanup-verdict.md` opens with an item whose whole text
is a refusal to argue: *"What is owed at the opening, and it does not exist
yet: how many acquire-and-release pairs stand in `selfhost/` and under
`examples/`, and how many early returns and `?` operators sit between an acquire
and its release. That count is the argument in both directions, and neither
direction may be argued without it."*

This is that count. It is taken before the sitting, so the seats are handed a
figure rather than an impression.

**It also settles what the sitting is ABOUT**, and that is the finding rather
than a preliminary. The milestone file names two obligations, `owned` and
`lease`, and says both put *a release obligation on every path*. One of the two
puts none, the other one is loud, and the obligation that actually leaks in
silence is a third the file names only in its second item.

## What was run

Every number below was measured in the session that writes it (CLAUDE.md §1),
on this Mac, macOS 15.6 / Darwin 25.6.0, arm64, sqlite 3.51.0, against the
compiler at `2c7831ac`. Where a count came from a command, the command is
beside it; where it came from reading twelve function bodies, that is said.

**One instrument correction is recorded rather than quietly fixed.** The first
enumeration of handle types used `[A-Za-z_]+` for a C tag and reported **3**
handles in the tree. The class excludes digits, so `tag sqlite3` and
`tag sqlite3_stmt` did not match: the real figure is **9**. A pattern is a
premise too, and this one was wrong by two thirds.

## 1. The three obligations, and what a missed one does

| obligation | landed | obligation on a path? | a miss is | sites in `examples/` |
|---|---|---|---|---|
| `owned <freer>` | 2026-09-07 | **none** | — | 1 declaration |
| `x: cstr @ s.lease()` | 2026-09-09 | on every path | **LOUD** — abort at `main`'s return | 1 |
| a **C handle** | 2026-09-14 | on every path | **SILENT** — exit 0, no diagnostic | 5 types, 11 pairs |

**`owned` puts no obligation on any path, and the milestone file says it does.**
The compiler frees the C string **at the call**, inside a null guard: read in
the emitted C —

```
bb1:
    t10 = (void *)(char *)t9;
    (void)free(t10);
```

— on the source line of the call, and confirmed by running a program that takes
an early return between the acquire and the end of its function. There is no
path for a programmer to miss, because there is no programmer step.

**`lease` puts one and it is loud.** A lease that escapes on an early return:

```
panic: 1 lease(s) never ended — every `.lease()` owes one `end_lease`,
and this program is missing that many
```

exit **134**, and it says how many. `spec § 13` promises exactly this.

**The handle puts one and nothing says a word.** Measured twice, in the two
shapes the corpus actually contains. Asking SQLite itself rather than a leak
checker, because LeakSanitizer does not exist on Darwin arm64:

| the escape | rows seen | statements still open | `sqlite3_close` says BUSY | exit |
|---|---|---|---|---|
| `break` out of the stepping loop | 2 | **1** | **true** | **0** |
| a `?` firing inside it | — | **1** | **true** | **0** |

The second is the one the shipped corpus has. The same program written in C
answers `1` and `true` too, so the leak is the program's and not this
language's.

## 2. The pairs, and what sits between each acquire and its release

Twelve pairs read by hand, eleven under `examples/` and one in `selfhost/`.
Counts of `?` and of `return`/`break`/`continue` are taken **strictly between**
the acquire line and the release line.

| where | pair | `?` | jumps | would an escape leak? |
|---|---|---|---|---|
| `examples/sqlite/main.hero` `first_int` | prepare → finalize | 0 | 1 | no — prepare failed, the handle is NULL |
| `examples/sqlite/main.hero` `main` | open → close | 0 | 1 | **YES** |
| `examples/ledger/db/sqlite.hero` `opened` | open → close | 0 | 0 | no — this is the repaired shape |
| `examples/ledger/db/sqlite.hero` `prepared` | prepare → caller | 0 | 1 | no — NULL on failure |
| `examples/ledger/main.hero` `build` #1 | prepared → finalized | **4** | 0 | **YES ×4** |
| `examples/ledger/main.hero` `build` #2 | prepared → finalized | **6** | 0 | **YES ×6** |
| `examples/ledger/main.hero` `queried_balances` | prepared → finalized | **7** | 0 | **YES ×7** |
| `examples/ledger/main.hero` `queried_int` | prepared → finalized | **3** | 0 | **YES ×3** |
| `examples/ledger/main.hero` `queried_float` | prepared → finalized | **2** | 0 | **YES ×2** |
| `examples/ledger/main.hero` test at :280 | prepared → finalized | 0 | 0 | no |
| `examples/curl/main.hero` `main` | init → cleanup | 0 | 0 | no |
| `selfhost/cli/process.hero` `files_under` | scan → release | 0 | 1 | no — the scan failed, nothing was acquired |

**23 escaping paths would leak a handle**, and they sit in **two files**: 22 in
`examples/ledger/main.hero` and 1 in `examples/sqlite/main.hero`. Every one of
the 22 is a `?`.

**The one in `examples/sqlite/main.hero`'s `main` is a real defect of that
program, and its neighbour already knew.** `examples/ledger/db/sqlite.hero`'s
`opened` carries a seventeen-line comment explaining that **a failed open still
hands back a connection** — measured on two platforms, sqlite 3.51.0 here and
3.46.1 in the Linux image — and closes the handle on its own failure path.
`examples/sqlite/main.hero`, the program a reader meets FIRST, returned without
closing. **Repaired in this same step**, because a corpus that teaches the wrong
thing beside a sibling teaching the right one is worse than either. It is not a
`DEFECTS.md` number: that list holds failures of the **compiler** on a program,
and this is a failure of a program.

## 3. The threads are the counter-example, and they are unanimous

Ten examples spawn and join: `wordbands`, `dotproduct`, `mandelbrot`,
`montecarlo`, `threads`, `collatz`, `matmul`, `firsthit`, `nqueens`,
`histogram`. Every one has the identical shape — a push loop, then a join loop,
**six lines apart** — and **zero** escaping paths between them, `?` and jumps
both.

That unanimity is worth more to the sitting than the ten instances. A
scope-bound release buys nothing at all here, and these are ten of the
twenty-one FFI programs in the corpus.

## 4. What the sitting is handed

- The obligation that needs a form is the **handle**, not `owned` and not
  `lease`. Two thirds of the premise the milestone was scheduled on is gone.
- The exposure is **23 paths in 2 files**, all but one of them a `?`, and the
  `?` is the operator that makes an early return **invisible** — which is why
  the count is concentrated in the program that uses `?` most.
- **10 of 21 FFI programs have no acquire-and-release pair at all**, and 10
  more have one whose release cannot be skipped.
- **The language already owns a scope-bound release, and a handle is outside
  it — measured, not argued.** The exit sweep over the slot table
  (`selfhost/ir/own.hero`, rule 5: *ownership lives in slots, and cleanup is a
  walk over a table*) releases on every path, `?` included, and rule 5's own
  comment says why it had to: *`.must()`, `?`, `&&` and if-as-value all open a
  block in the MIDDLE of an expression*. Read in the emitted C of
  `examples/curl/main.hero`: the three `str?` slots each get a `_retain` and a
  `_release` and are swept; the `Curl` handle gets **`_eq` and `_hash` and
  nothing else — zero `_retain`, zero `_release`**. So the sweep does not skip
  handles by policy; it has nothing to call. **The question a form would answer
  is therefore whether a C handle can be given a release function and join the
  table the language already walks** — narrower and cheaper than *should Heroes
  have `defer`*, and landing inside machinery that is already load-bearing
  rather than beside it. That is the question this census puts.
- One pair in `selfhost/` is protected, and by an accident that does not
  generalise: `hero_dir_release` is the **runtime's** resource, so the runtime's
  own leak gate panics at exit when it is missed. A `sqlite3_stmt *` has no such
  gate and can have none, because the runtime never saw it allocated.

## What this measurement does NOT say

It does not price a form, name a spelling, or say whether one should enter.
That is the sitting's, and the milestone file forbids deciding a spelling before
deciding whether a form enters at all.

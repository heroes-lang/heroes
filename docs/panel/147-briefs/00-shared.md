# Panel 147 — shared brief

**The sitting**: M-cleanup-verdict. Convened 2026-09-14 on the author's
authorisation, full five seats, because a form that enters has surface.

**Working tree is FROZEN** from now until the synthesis is written. Build in a
copy: `cp -r` the tree to your own scratch directory and `rm -rf target build`
after the copy. A copied build directory leaves paths pointing back at the real
repository, which has silently produced wrong numbers in this project before.

**The cheap build route**: `clang -I runtime seed/heroes.c runtime/runtime.c -o
heroes` takes about 3.4 s. Rebuilding from `selfhost/` takes ~20 minutes and
will kill you on the watchdog. Use the seed unless you are measuring a change to
the compiler's own source.

Repository root: `/Users/joseph/Temp/heroes-lang`, at commit `7e6e71a8`.

---

## The question

**Can a C handle be given a release function so it joins the exit sweep the
language ALREADY walks on every path — or does a scope-bound release form
enter — or is this refused to design.md Part 6 with a named falsifier?**

Three routes are on the table. A fourth that nobody has listed is exactly what
this sitting wants to hear about.

- **Route A — the handle names its releaser.** `record Stmt tag sqlite3_stmt
  released sqlite3_finalize`. The compiler calls it once on every path out of
  the scope that acquired the handle, `?` and early return included, and
  refuses your own call of it. Measured spec cost: **+80 vendored tokens**.
- **Route B — a scope-bound statement.** `cleanup sqlite3_finalize(statement)`,
  running when the enclosing block is left however it is left. Measured spec
  cost: **+108 vendored tokens**.
- **Route C — refuse to design.md Part 6**, with a falsifier that can expire in
  its turn. Spec cost **0**.

**The sitting may NOT**: take M-deferral-ledger's Part 7 items, which belong to
that milestone; or decide a SPELLING before it has decided whether a form enters
at all. The spellings above exist so the token cost could be measured; treat
them as placeholders, and say so if you think the spelling changes the verdict.

---

## The census — every number below was measured 2026-09-14, this Mac, arm64, sqlite 3.51.0

Full document: `docs/measurements/030-three-release-obligations-and-only-one-of-them-is-silent.md`.

### 1. Of three release obligations, only one is silent

| obligation | obligation on a path? | a miss is | sites in `examples/` |
|---|---|---|---|
| `owned <freer>` | **none** — the compiler frees at the call | — | 1 declaration |
| `x: cstr @ s.lease()` | on every path | **LOUD**: exit 134, *"1 lease(s) never ended"*, saying how many | 1 |
| a **C handle** | on every path | **SILENT**: exit 0, no diagnostic | 5 types, 11 pairs |

The handle leak was measured by asking SQLite itself — `sqlite3_next_stmt` —
rather than a leak checker, because LeakSanitizer does not exist on Darwin
arm64. Both escape shapes give the same answer: **1 statement still open**,
**`sqlite3_close` says BUSY**, **exit 0**. The identical program in C answers
the same, so the leak is the program's and not this language's.

**The milestone was scheduled believing `owned` and `lease` both left an
obligation on every path. Two thirds of that is false.**

### 2. Exposure: 23 escaping paths, in two files

Twelve acquire-and-release pairs read by hand across `examples/` and
`selfhost/`. **23 paths would leak a handle**: 22 in `examples/ledger/main.hero`
— every one of them a `?` — and 1 in `examples/sqlite/main.hero`, repaired this
session.

Against that:

- the **ten** threaded examples are unanimous — spawn loop, join loop, six
  lines apart, **zero** escapes in all ten;
- **10 of 21** FFI programs have no acquire-and-release pair at all;
- the one pair in `selfhost/` (`cli/process.hero`'s `files_under`) is safe, and
  protected by an accident that does not generalise: `hero_dir_release` is the
  **runtime's** resource, so the runtime's own leak gate panics at exit when it
  is missed. A `sqlite3_stmt *` has no such gate and can have none.

### 3. The narrowing — the machinery exists and a handle is outside it

`selfhost/ir/own.hero` rule 5: *ownership lives in slots, and cleanup is a walk
over a table*. The exit sweep releases on every path **including `?`**, and rule
5's own comment says it had to be built that way because *`.must()`, `?`, `&&`
and if-as-value all open a block in the MIDDLE of an expression*.

A handle is outside that sweep. Read in `examples/curl/main.hero`'s emitted C:
the three `str?` slots each get a `_retain` and a `_release`; the `Curl` handle
gets **`_eq` and `_hash` and nothing else — zero retain, zero release**. The
sweep does not skip handles by policy. It has nothing to call.

---

## Context every seat needs

- **There are no destructors, by design** (`selfhost/emit/types.hero:6`).
- **`consumes` already exists** (panel 145, defect 031, 2026-09-13): a call may
  be marked as ending a value's life, and handing it one the caller borrowed is
  a compile error. **114 code lines**, refusing **2 of 17** functions in the
  shipped SQLite binding.
- **design.md Part 6's borrow-checker row moved from soundness to COST on
  2026-09-13**, by its own words, when `consumes` landed. Its stated falsifier
  was *an annotation a binding author writes once per function, separating a
  call that invalidates a handle from one that reads it, with a refusal rate on
  `examples/` below 13 of 17*. That fired at 2 of 17.
- **design.md Part 8 wart 20** states the affine-handle class in the present
  tense: one copy of a value holding a `ptr` can free what every other copy
  holds, at exit 0.
- **Defect 032 was closed this session**: `==` on a handle answered a constant
  `true`, so until today a program could not even ASK whether a handle was null
  — which is the guard any release form would need. It is fixed; do not price
  around it.
- **The vocabulary is silent**, searched 2026-09-10 over design.md, `spec/` and
  all sittings: `defer`, `RAII`, `scope guard`, `scope-bound`, `destructor`,
  `finally`, `cleanup on` return **nothing about a construct**.

## What your report must contain

A **verdict**, the **design.md section** it rests on, a **cost or delta you
measured** (not estimated — and if you could only estimate, say the word
*estimate*), a **falsifiable prediction**, and any **condition** you attach.
A negative claim names what you searched for. If you could not run something,
write *unrun* rather than reasoning past it.

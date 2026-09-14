# 031 — The census counted from the tree, and it has two levels

Date: 2026-09-14 · M-marked-acquisition step 1 · **the re-count the milestone's
own file demanded before anything is argued from the old one.**

## Why this file exists

`docs/measurements/030` was taken by reading twelve function bodies and was
**short by at least seven pairs**, found by panel 147's completeness critic.
CL-057 is the rule it broke: *the list is a measurement too*, and a list read
is not a list counted. `M-marked-acquisition`'s third item says so in terms —
**re-run it rather than citing it, count from the tree, and name the command.**

This is that count. Every number below comes from a command, and the commands
are here.

## The instrument

```sh
# every handle type, and every extern function whose signature mentions one
for f in $(grep -rlE "^ +record [A-Za-z0-9_]+ tag [A-Za-z0-9_]+ *$" \
             --include='*.hero' examples/ tests/ selfhost/); do
  for h in $(grep -oE "^ +record [A-Za-z0-9_]+ tag [A-Za-z0-9_]+ *$" "$f" \
               | awk '{print $2}'); do
    grep -nE "^ +function .*[^A-Za-z0-9_]${h}[^A-Za-z0-9_]|^ +function .*-> *${h} *$" "$f"
  done
done
```

**The character class matters and it is why 030 was wrong about handles too.**
`[A-Za-z_]+` excludes digits, so `tag sqlite3` did not match and the first
enumeration answered **3** handle types where the tree holds **9**.

## 1. The handle types, and what each library function does with one

**9 handle types**: `CDb` and `CStmt` (`examples/ledger/db/sqlite.hero`), `Db`
and `Stmt` (`examples/sqlite/main.hero`), `Curl` (`examples/curl/main.hero`),
`Db` and `Stmt` (`tests/golden/check/ffi-handle-refusals.hero`), `Chunk` and
`Thing` (two `tests/golden/run/` cases).

**And here is the fact that decides how big a mark's blast radius is.** Of every
`extern function` in the tree whose signature mentions a handle type, exactly
**three produce one**:

| producer | how | releaser | marked today? |
|---|---|---|---|
| `sqlite3_open` | `@out: CDb` | `sqlite3_close` | no |
| `sqlite3_prepare_v2` | `@statement: CStmt` | `sqlite3_finalize` | no |
| `curl_easy_init` | `-> Curl` | `curl_easy_cleanup` | **`consumes`**, since panel 145 |

Every other handle-typed parameter in the tree is a **use** — `sqlite3_step`,
`sqlite3_bind_*`, `sqlite3_column_*`, `sqlite3_errmsg`, `curl_easy_setopt`.

**So the producing/borrowing ambiguity that killed a type-keyed releaser at
panel 147 does not appear in this tree at all.** `sqlite3_db_handle` and
`sqlite3_next_stmt` — the two the sitting compiled to refuse Route A — are bound
by **no** binding here; they exist in the probes and in `sqlite3.h`. That is a
fact about **this corpus on this date**, not about C, and it is exactly the shape
that expires in silence: the first binding that declares one brings the
ambiguity back. It is recorded as a premise, not as a reassurance.

## 2. Two levels, and 030 flattened them into one

**Extern level — 7 acquire sites and 7 release sites**, counted from the tree,
excluding comments and programs embedded in test strings:

| | acquire | release |
|---|---|---|
| `examples/ledger/db/sqlite.hero` | `:215` open · `:275` prepare | `:230` close · `:235` close · `:280` finalize |
| `examples/sqlite/main.hero` | `:71` prepare · `:91` open | `:85` finalize · `:101` close · `:107` close |
| `examples/curl/main.hero` | `:57` init | `:82` cleanup |
| `tests/golden/fixedbugs/ffi-out-parameter-guard.hero` | `:41` open · `:46` prepare | — |

**Wrapper level — the one 030 missed.** `examples/ledger/db/sqlite.hero` wraps
all four operations in Heroes functions, because §4.19 refuses an `extern` call
across a module boundary, and the rest of the program calls only those:

| wrapper | what it is | call sites |
|---|---|---|
| `opened() -> Db?` | `:212`, acquires and RETURNS | **7** |
| `prepared(db, sql) -> Statement?` | `:271`, acquires and RETURNS | **6** |
| `closed(@db) -> i64` | `:234`, releases | **9** |
| `finalized(@statement) -> i64` | `:279`, releases | **6** |

**This is the structure that refused Route A**, and it is also the structure any
mark must survive: the acquiring `extern` call is one module away from every
place a handle is actually held, so a mark on the declaration **propagates**
through a Heroes wrapper the way `consumes` did — panel 145 measured that at 24
lines across `examples/ledger/` for two marks.

## 3. The critic's fourth correction, confirmed and made precise

**Every failure path in `examples/ledger/main.hero` closes the database and
calls `exit(1)`** — `:36-38`, `:43-45`, `:49-51` — so the **connection** is
released on every path and only the **statements** prepared inside `build` and
`show` leak, and the process leaves immediately afterwards.

So 030's headline *exit 0, in silence* stands for the reduction it was measured
on and not for this corpus: **no shipped program leaks a handle and then
continues.** The class is real and unobserved. The first program written here
that recovers from a database error instead of exiting will observe it.

## 4. A third instrument, free, already returned, and thrown away 21 times

**Every release call site in the corpus discards what the library says about
it.** Counted: **21 sites**, of which **19** are `_ = <release>(…)` and the
other **2** are the wrappers' own `return`, to callers that then discard it.

`sqlite3_close` answers **`SQLITE_BUSY`** exactly when a statement was left
open — it is the value this milestone's whole subject would be detected by, it
is already computed, already crosses the boundary, and is dropped at every site.
design.md:1869 names the shape: *an ignored C return code is C's own classic
silent bug*.

**Stated at its real width**: this is not a general instrument. `free` and
`curl_easy_cleanup` return `void`, so it detects nothing for them, and it
reports at close time rather than at the leak. It belongs on the ladder as a
third row, not at the top of it.

## What this measurement does not do

It does not price either instrument — that is this milestone's first item and
it must be **compiled**, which is exactly what panel 147 could not do. It does
not choose between a compile error and a loud exit. And it does not re-open
panel 147's R1.

# Panel 160 — ffi-pragmatist report

Seat: ffi-pragmatist. Mandate: design.md §1.11, §4.19. Veto on ABI breakage.
Written 2026-09-17, skeleton first and improved in place. Every number below was
produced by a command run in this session on the copy at
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-lang/bf49271c-9c48-4701-9a34-ec92d4cc09af/scratchpad/ffi160`
(seed built there: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`,
exit 0; `heroes doctor` every row ok, clang 21). Real header:
`/opt/homebrew/Cellar/sqlite/3.53.4/include/sqlite3.h`, linked `-lsqlite3`.
Linux: the `heroes-linux` container, `x86_64`, sqlite 3.46.1, seed built inside
at exit 0, every probe `heroes build … --sanitize`. The repository working tree
was not modified except for this file.

- `verdict`: **approve** — option **A**, the refusal of all four readers on a
  value whose payload is itself fallible. No veto: nothing on the ballot
  reaches the C ABI. Two corrections to the brief and one to the specification
  are recorded below, and a narrower route nobody listed is priced.
- `section`: design.md **§1.11** (*FFI ergonomics rank alongside comprehension*)
  and **§4.19**; spec **§ 13** for the sentence this sitting falsified.
  **design.md does not cover a handle inside a nested fallible**: grepped, it
  names the `T??` type at `:1184` and `:2836-2842` (the parser's refusal, and
  the checker building one from a container's element) and says nothing about
  what the four readers do on it or about a handle held in one. I say so
  explicitly as the brief requires.

## One line on the boundary itself

Panel 158 measured `error[ffi_type]` refusing `T?` as an extern parameter,
result and record field, so a `Db??` cannot be declared on either side of a C
call whatever A to E decides. No option changes layout, ownership, `str`
termination or a refcount's visibility. No ABI ground for a veto exists here.

## The experiment

Common binding in every probe, the spec § 13 example plus `SQLITE_OK`:

```
record Db tag sqlite3
function sqlite3_open(filename: cstr, @out: Db acquires sqlite3_close) -> i64
function sqlite3_close(db: Db consumes) -> i64
```

`open_db(path) -> Db?` closes the connection a failed open still hands back and
returns `fail(code: "open", …)`; `is_open(d: Db?) -> bool` is `!d.is_err()`.
An unused predicate parameter is `error[unused_binding]` (measured), so every
predicate reads its argument. `hit = find(dbs, is_open)` is a `Db??`. clang
accepted every program the checker passed; the two the checker refused (P2a,
and everything under the instrument) never reached clang.

### The Mac, `./heroes` built from the seed

| probe | shape | `build` | run | what it printed |
|---|---|---|---|---|
| P0 | `match` naming both levels, pool closed by a loop | 0 | **0** | `found an open db`, `skip a slot that failed: open` |
| P1 | two failed opens; `find` → outer `not_found`; `if hit.is_err() return` | 0 | **0** | `program reads this as: the open failed` — wrong reading, no handle consequence |
| P2a | slot holds a failed open; found branch; `sqlite3_close(hit.must())` | **1** | — | `error[type_mismatch]: expected Db, found Db?` … `fix (guess): .must()` |
| P2b | same, `sqlite3_close(hit.must().must())` | 0 | **134** | `panic: .must() on an error: open: cannot open …` — the pool's live db is stranded and the abort preempts the ledger |
| P2c | same, `sqlite3_close(hit.must().default(nullptr))`, pool never cleaned | 0 | **134** | `close answered 0` then `panic: 1 C handle(s) never given back` |
| P2d | P2c plus the cleanup loop | 0 | **0** | `found a slot, closing it` / `close answered 0` — **a close reported for a connection that never existed, exit 0, no diagnostic** |
| P3 | one successful open; `if hit.is_err() return`; falls off the end | 0 | **134** | `panic: 1 C handle(s) never given back … The first is at 0x…` — the ledger sees through `ok(ok(db))` inside `[Db?]` |
| P3b | two successful opens; `dbs[0]` closed TWICE through two copies (`dbs[0].must()` and `find(…).must().must()`, `same address? true`); `dbs[1]` never | 0 | **134** | `second close answered 21` (SQLITE_MISUSE) then `panic: 1 C handle(s) given back that were never taken … double release` |
| P4 | `[Stmt?]` pool through `@statement` out-parameters, option-A style: `any(stmts, is_prepared)` for the outer question, nested `match` for both levels | 0 | **0** | `any usable statement? true` / `answer: 42` / `skip: prepare` |
| P5 | `{str: Db?}` named pool, `match` on `pool[name]` naming both levels | 0 | **0** | `main: open, closing` / `archive: the open failed: open` / `missing: missing_key` |
| P5b | same pool, `if pool[name].is_err()` read as *did the open fail*, else `sqlite3_close(pool[name].must().default(nullptr))` | 0 | **0** | `archive: program reads this as: open succeeded, closing` — **defect 050 on a binding, exit 0** |
| P6 | `function first_open(dbs: [Db?]) -> Db?` with `inner = find(dbs, is_open)?` — a CORRECT use of `?` on a nested | 0 | **0** | `first open db is null? false` |
| P6a | P6 rewritten as `return match find(…)` `.ok inner => inner` `.err e => fail(…)`, what A forces | 0 | **0** | identical output; `first_open` is one line longer |

### The Linux leg, `--sanitize`

Nine of nine probes of the first round, and both P5 probes in a second round,
reproduce the Mac exit codes and print the same lines to the message; only the
addresses differ. ASan and LSan printed nothing on any of them: the leak probes
abort inside `hero_runtime_check_leaks` before LSan runs, P2d and P5b leak
nothing because closing `nullptr` is a no-op on both sides, and P3b's second
`sqlite3_close` touches memory freed inside system `libsqlite3`, which the
sanitizer does not instrument (panel 147 saw the same). P6 and P6a were written
after the container runs and are **UNRUN** there; the command is the same
`docker run` line with `p6_try_nested p6a_match_instead` in the list.

### The option-A instrument, `heroes-next`

In the copy only, `selfhost/check/builtins.hero`'s three arms and
`selfhost/check/access.hero`'s `try_type` were made to refuse when
`table.nested_fallible(types, id)` holds (a 17-arm helper added to
`check/table.hero`; the first draft used `_` and was refused by
`wildcard_on_variant`, which is the compiler doing its job). Seed check of the
patched compiler: exit 0. `heroes build selfhost/main.hero -o heroes-next`:
exit 0, `real 69.76` beside a running Docker job, so **not a timing claim**
(CL-025) and over the brief's ~60 s, run in the background.

| what `heroes-next check` read | files | exit 0 | nested-reader sites |
|---|---|---|---|
| `selfhost/main.hero` (**R3, Principle 0**) | 1 | 1 | **0** |
| `tests/harness/main.hero` | 1 | 1 | **0** |
| `examples/*/main.hero` | 54 | **54** (control under the seed: 54) | **0** |
| `tests/golden/run` | 124 | not counted (goldens) | **0** |
| `tests/golden/check` | 124 | not counted (refusals by design) | **0** |
| `tests/golden/emit` · `ir` · `unsupported` | 6 · 23 · 12 | not counted | **0** · **0** · **0** |
| the thirteen probes | 13 | P0, P4, P5, P6a | P1 1 · P2a 2 · P2b 2 · P2c 2 · P2d 2 · P3 1 · P3b 1 · P5b 2 · **P6 1** |

The message printed by the instrument: `` error[bad_operand]: `is_err` takes … a
single-level fallible, found `Db??` `` — so **R2: `state.show` prints `Db??`
today**, a spelling the parser refuses (158's R1 unlanded). The `?` arm borrowed
`not_fallible`'s text and inherited `fix (certain): remove the ?`, which on a
nested fallible is a **wrong certain fix**; a real rule must carry its own
message and no certain fix, or the repair the compiler applies is the defect.

## Answers to the brief's three questions

1. **Outer failure**: clean at `main`'s return, exit 0 (P1). The failed opens
   were consumed inside `open_db`; `not_found` carries no handle.
2. **`ok(fail(…))` and the found branch**: a handle cannot be released at the
   wrong level in ONE reader. `sqlite3_close(hit.must())` is `type_mismatch`
   (P2a), because a `Db?` does not reach a `consumes` parameter — the
   boundary's own type stops it, not any rule about levels. **And the
   compiler's `fix (guess)` is the second `.must()`**, which is P2b: exit 134
   on the found-but-failed slot, the other live connection stranded. The
   silent route is `.default(nullptr)`: SQLite answers 0 on a NULL close, the
   ledger discharges nothing on NULL by design (`runtime/parts/alloc.c:413`),
   and with the pool otherwise cleaned the program reports a close of a
   connection that never existed at **exit 0** (P2d, P5b). Nothing is
   corrupted; the answer is wrong. That is defect 050 wearing a handle.
3. **Successful open, `.is_err()` misread, never closed**: **the ledger reports
   it at 134** (P3). The set is keyed on the address, so the copy inside
   `ok(ok(db))` and the copy in `dbs[0]` are one obligation. **The balance clause
   does not reach this shape, and it is false as written.** P3b consumes one
   handle twice and another never; the runtime prints `1 C handle(s) given back
   that were never taken`, the stray reported before the leak
   (`alloc.c:269-300`), exit 134 on both platforms. spec § 13's *"The owing is
   counted, so a handle consumed twice hides one never consumed"* entered on
   2026-09-14 (`7e3bb986`); the counter became a set on 2026-09-15
   (`2e7d221c`, *"the counter became a set"*), which did not touch the spec;
   `tests/golden/run/abort-handle-given-back-twice.expected` asserts the set's
   message. The brief carried the clause as panel 148's fact: **that is its
   tenth corrected claim.** The leak of `dbs[1]` is indeed not printed, because
   the stray aborts first — but the program is not silent, and silence was the
   sentence's whole warning.

## The question my seat exists to ask

**Of A to E, A leaves a binding author the fewest ways to release a handle at
the wrong level, and it is measured rather than argued.** Today the type system
already closes the one-reader path (P2a). What remains open are the two-reader
paths, and they are the readers on the OUTER level: `.is_err()` misread (P1,
P3, P5b), `.must()` chained (P2b, P3b), `.must().default(nullptr)` (P2c, P2d,
P5b). Under **E** only the `.is_err()` sites close; P2d still compiles and
still exits 0. Under **A** all eight mistake probes stop compiling and the four
correct ones pass, including the `[Stmt?]` pool that is §4.19's ladder step 3
written honestly (P4) and the `{str: Db?}` named pool (P5).

**Does a real pool ever NEED only the outer level?** Yes — *is there any usable
statement* — and under A it writes `any(stmts, is_prepared)`, a `bool` that
touches no nested value. `any<A>` is `selfhost/library_source.hero:114`, already
shipped. P4 is that program at exit 0 under both compilers.

**What A costs, measured.** One correct shape, P6: `find(dbs, is_open)?` inside
a `-> Db?` function, refused with one site. Its repair is P6a, a three-line
`match` in place of two lines, output identical, exit 0 under both compilers.
Not a shim, not a wrapper, not a marshalling step — one line. **R3 is zero**:
the compiler, the harness, 54 examples and 289 golden programs apply no reader
to a nested fallible, so A costs the bootstrap nothing and the corpus nothing.

**A route nobody listed** (CL-057): **A minus `?`** — refuse `.is_err()`,
`.must()` and `.default()` on a nested payload and leave `?`, whose result type
still carries the second level and which has no silent form (a discarded
fallible is already refused). It stops every one of the eight mistake probes
and refuses none of the five correct ones. That is what *conservative* would
be; I approve **A** as the more robust and record this so the author can
choose (CL-040).

**On B**: as written it refuses P5 and P5b alike — a correct program and a
wrong one — while leaving `find` and every `[Db?]` untouched, and a wrapper
record per slot reopens the door. It protects nothing A does not, and takes
the named pool away. **If B were widened to `[V?]`, I veto it**: `[Db?]` and
`[Stmt?]` are how a pool of fallible opens is spelled (P0, P4, P6a), and the
replacement is parallel arrays or a record per slot — categorically harder.

## R4: what the specification owes, and what pays

The nesting sentence is owed (spec has no sentence about peeling; the shared
brief measured zero and I did not re-measure that). **The payment is in § 13
already**: the false balance clause. Vendored, lower bounds, `--refresh`
unavailable here (`ANTHROPIC_API_KEY` unset, exit 2 by
`.claude/rules/spec-shape.md`):

| text | `claude-legacy` | `cl100k_base` |
|---|---|---|
| the two § 13 sentences today (lease abort + balance clause) | 64 | 63 |
| merged: *"…and a lease nobody ends, like a handle nobody consumes **or one consumed twice**, aborts when `main` returns, saying how many."* | 53 | 52 |
| **delta** | **−11** | **−11** |
| panel 158's nesting sentence, the warden's merge | +9 | +9 |

A false sentence corrected is a named removal, and it leaves two vendored
tokens over after paying for 158's R3. The real instrument must price it
before it lands (author instruction 2026-09-16, *always measure with the real*).

## `argument` (≤120 words)

A `Db??` never crosses the boundary, so no option touches the ABI. What crosses
is the HANDLE inside it, and the readers on the outer level are the only way to
release it at the wrong level: the type system already stops one peel (P2a),
so the damage is in the second — `.must().must()` aborts at 134 stranding the
pool, `.must().default(nullptr)` closes nothing and exits 0 (P2d, P5b). A closes
all of it; E leaves P2d open. A costs the bootstrap zero sites (R3 measured over
selfhost, 54 examples, 289 goldens) and one correct shape a line's repair.
The outer question a pool needs is `any`, already shipped. And the brief's
balance clause is a spec sentence the runtime falsified two days ago.

## Programs and exit codes

| program | mac `check`/`build` | mac run | linux `--sanitize` build | linux run | `heroes-next` (option A) `check` |
|---|---|---|---|---|---|
| P0 `p0_match` | 0 | 0 | 0 | 0 | 0 |
| P1 `p1_outer` | 0 | 0 | 0 | 0 | 1 (1 site) |
| P2a `p2a_must_once` | 1 | — | 1 | — | 1 (2 sites) |
| P2b `p2b_must_must` | 0 | 134 | 0 | 134 | 1 (2 sites) |
| P2c `p2c_default_null` | 0 | 134 | 0 | 134 | 1 (2 sites) |
| P2d `p2d_default_null_cleanup` | 0 | **0** | 0 | **0** | 1 (2 sites) |
| P3 `p3_leak` | 0 | 134 | 0 | 134 | 1 (1 site) |
| P3b `p3b_double_close` | 0 | 134 | 0 | 134 | 1 (1 site) |
| P4 `p4_stmt_pool` | 0 | 0 | 0 | 0 | 0 |
| P5 `p5_named_pool` | 0 | 0 | 0 | 0 | 0 |
| P5b `p5b_named_pool_is_err` | 0 | **0** | 0 | **0** | 1 (2 sites) |
| P6 `p6_try_nested` (correct) | 0 | 0 | UNRUN | UNRUN | 1 (1 site) |
| P6a `p6a_match_instead` | 0 | 0 | UNRUN | UNRUN | 0 |
| `selfhost/main.hero` | 0 (seed, patched tree) | — | — | — | 0 (0 sites) |
| `tests/harness/main.hero` | — | — | — | — | 0 (0 sites) |
| `examples/*/main.hero` (54) | 54 of 54 exit 0 (seed) | — | — | — | 54 of 54 exit 0, 0 sites |
| `tests/golden/{run,check,emit,ir,unsupported}` (124·124·6·23·12) | — | — | — | — | 0 sites in every directory |

## `prediction` (falsifiable, named bindings)

Under option A landed as refusing all four readers on a nested payload:
**`examples/sqlite/main.hero`, `examples/ledger/main.hero` and
`examples/curl/main.hero` check at exit 0 with no edit, and §4.19's ladder step
3 written as a `[Stmt?]` pool needs no shim, no wrapper and no reader on the
`Stmt??`** — falsified by any nested-reader diagnostic on those three files,
or by P4 failing to check. Second: **`p3b_double_close` exits 134 with `given
back that were never taken` on both platforms, never 0**, so the spec's
balance clause is false until edited — falsified by a run of that program
exiting 0 or printing the leak sentence first.

## `condition`

- I move to **veto** if any resolution widens B to arrays, refusing `[V?]` at
  the declaration: `[Db?]` and `[Stmt?]` are the pool shape a binding author
  has (P0, P4, P6a), and parallel arrays or a record per slot is categorically
  harder. I also veto any repair that lets a fallible cross the FFI at any
  depth (panel 158's condition, unchanged).
- I move to **object** if A lands carrying a `certain` fix inherited from
  `not_fallible` (`remove the ?`), or with a message that does not name the
  repair — `match` naming both levels, `any` for the outer question — because
  §4.17 says a diagnostic carries what is needed to fix the program.
- I move to **approve E** only if a measurement shows P2d
  (`.must().default(nullptr)`) refused some other way at `check`.
- I keep **approve** if the sitting takes *A minus `?`* instead of A: it
  refuses the same eight mistakes and no correct program, and the record
  should say which was chosen and why.

## UNRUN

- P6 and P6a on Linux under `--sanitize` (written after the container runs).
- The real-tokeniser price of the § 13 merge: `heroes measure
  spec/heroes-spec.md --refresh` needs `ANTHROPIC_API_KEY`, unset here.
- Windows: no probe reached the box; only the author starts it.
- The full net, forbidden by the brief. Nothing here says whether the tree is
  green; the patched compiler lives only in the scratch copy.
- When the `heroes-next` build's `real 69.76` would read on a still machine.

## Files

Probes: `…/scratchpad/ffi160/p/*.hero` and `.bin`; measurements
`…/scratchpad/ffi160/m/*.md`; R3 output `…/scratchpad/ffi160/r3_selfhost.txt`,
`r3_harness.txt`; the patched checker is `…/scratchpad/ffi160/selfhost/check/
{builtins,access,table}.hero` and nothing under `/Users/joseph/Temp/heroes-lang`
was touched but this report.

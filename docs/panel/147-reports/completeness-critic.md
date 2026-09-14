# Panel 147 — completeness critic

Not a verdict. What is MISSING from the six briefs and the five reports.

Everything below was run 2026-09-14 on this Mac, arm64, against the frozen tree
at `7e6e71a8`. `git status --porcelain` was empty before and after every command
in this report; the only writes were into
`.../scratchpad/critic/`, where I built the seed myself (`clang -I runtime
seed/heroes.c runtime/runtime.c -o heroes`, `real 3.05`) so that no number here
depends on another seat's binary. Where I could not run a thing I write
**unrun** and name what I searched for.

**One frame before anything else, and no seat stated it plainly.** Route A
cannot be compiled. I wrote the mark and asked the shipped compiler:

```
$ ./heroes build relmark.hero -o /dev/null
error[expected_extern_signature]: expected a `function`, a `constant` or a
`record`, found a name (`released`) — an `extern` group holds what the header
declares, one per line
  at relmark.hero:2:35
```

So **every Route A claim in this sitting is either hand-inserted C or a reading
of a four-line draft.** The compiler-engineer's veto is hand-inserted C (I
reproduced it, § 2). The ffi-pragmatist's veto is hand-inserted C (I reproduced
it, § 2). The spec-warden's veto and the ergonomist's approval are readings of
the same four lines, and they read them oppositely. The synthesis must say this
in its own voice, because a reader will otherwise take "veto, measured" and
"approve, measured" as two measurements of one object.

---

## 1. A route nobody listed

### 1a. The three "fourth routes" are one family with three instruments, and the family is the real finding

| seat | its fourth route | when the miss is caught | who writes the release |
|---|---|---|---|
| historian (Route D) | linear types, Austral-style | compile time | the author |
| compiler-engineer | a fourth runtime counter beside `hero_live_held` | first run, at exit | the author |
| ffi-pragmatist | Route B + a `consumes`-style mark on the ACQUIRING call | compile time | the author, in a `cleanup` |

They differ on the instrument and agree on the thing that matters: **in none of
the three does the compiler choose a release call and insert it.** That is the
axis Routes A and B are on opposite ends of, and all three fourth routes are on
the same side of it. Three instruments, one route. Saying "three seats proposed
three fourth routes" hides the unanimity: **three of five seats, reasoning from
three different mandates, independently declined to let the compiler pick the
call.**

**None dominates**, and the ordering is a cost ladder with a measured rung at
each step (`code_lines`, the instrument `tests/harness/suite_layout.hero:484`
uses; I ran the coordinator's own `codelines.py` over the tree):

- engineer's counter: nearest landed neighbour `selfhost/check/leasing.hero`
  **195**, plus ~20 lines in `runtime/parts/alloc.c`. Cheapest, and the only one
  with a shipped precedent in this repository.
- ffi's mark-plus-`cleanup`: `selfhost/check/consuming.hero` **114** for the
  mark, plus all of Route B, which the compiler-engineer counted at 24
  `ast.StmtKind` arm sites (I reproduce 24 exactly, 12 files, same per-file
  breakdown) and no block-exit mechanism in the IR (`grep -rn 'scope'
  selfhost/ir/` returns **0**, reproduced). Most expensive.
- linear types: design.md `:3519` and `:3537` both call this the affine handle
  and both say **"core by §1.7"**. Unpriced anywhere.

**And the engineer's counter has a defect its own veto found and it did not turn
back on itself.** The report says *"Give a handle with a declared releaser a
fourth counter"* — a releaser declared on the handle is Route A's mark, i.e. on
the **type**. `hero_live_held` is driven by two calls
(`runtime/parts/alloc.c:180,185`), so a handle counter must be driven by calls
too. Key it on the type and `sqlite3_db_handle`'s borrowed `sqlite3 *`
increments and never decrements, and the program panics at exit for a leak it
does not have. I re-ran the seat's own probe to check the borrow is writable:

```
$ ./heroes run e6_borrow.hero
borrowed == owned: true
count: 2
```

So the engineer's fourth route only works with the mark on the **acquiring
call** — which is the ffi-pragmatist's fourth route minus the `cleanup`. Two of
the three fourth routes converge on one sentence nobody wrote: **the obligation
is created by a call, not by a type.** That sentence, not the choice of
instrument, is what this sitting discovered.

### 1b. A fifth route, and it is the one that dissolves the engineer's own condition

**Route E — refuse the escape by POSITION, the way `cstr` and `lease` already
do.** Three seats treat escape refusal as a door that is closed:

- compiler-engineer, condition 2: *"'prove' there is the dataflow
  `check/leasing.hero:29` says does not exist, so that condition is a request
  for a measurement, not a suggestion."*
- ffi-pragmatist §4a: *"The only way out is escape analysis, which §4.19 records
  as refused by name."*
- compiler-engineer §2: *"That is escape analysis... A different proposal, owed
  its own sitting."*

The door is open and the project walked through it twice. Run:

```
$ grep -rn 'escapes' selfhost/check/*.hero
selfhost/check/lending.hero:242:  state.push_diagnostic(@c, ffi_errors.cstr_escapes(...))
selfhost/check/leasing.hero:109:  state.push_diagnostic(@c, ffi_errors.lease_escapes(...))
selfhost/check/leasing.hero:238:  test "a lease copied by = or into a second cell escapes"
```

`docs/panel/122-the-lend-was-two-defects.md:25-29` put three candidates for the
`cstr` lend, and its `:56` records what landed: **"word every clause as a rule
on the POSITION of the `.cstr()` expression, never on the type"** — with the
result *"blast radius 0 files of 638"*. `selfhost/check/lending.hero:12-18`
states the three clauses verbatim: *a lend stands only as an argument of a call
(R2), no function outside an `extern` group answers `cstr` (R3), and no record
outside a group holds one (R4)*.

**Those are, term for term, the escape refusal Route A needs**, and design.md
`:2308` does not forbid them. Read it in full:

> the release is written by the author and **never inferred**, since an inferred
> release is the escape analysis panel 122 refused

What panel 122 refused is an **inferred release**. It shipped a positional
escape refusal in the same sitting. The ffi-pragmatist's *"refused by name"* is
a paraphrase that reverses the ruling, and the engineer's *"the dataflow does
not exist"* rests on `check/leasing.hero:25-30`, which says something narrower —
*"IT ASKS THE VALUE AND NOT THE WORLD... No flow analysis"* — and is the comment
on a module that ships an escape refusal without flow analysis.

**Route E's cost is therefore knowable today, and neither half was produced.**
Size, measured:

| shipped positional / mark rule | `code_lines` |
|---|---|
| `selfhost/check/ffi_sweep.hero` | **93** |
| `selfhost/check/consuming.hero` | **114** |
| `selfhost/check/leasing.hero` | **195** |
| `selfhost/check/freer.hero` | **221** |
| `selfhost/check/lending.hero` | **241** |

The compiler-engineer's bar for lifting its veto was *"under ~120 code lines in
one new `selfhost/check/` module"*. Two of the five nearest neighbours are under
it. Blast radius, measured (`grep -rn 'handle: C\?\(Db\|Stmt\|Curl\|Thing\)'`
plus the return positions):

- **3** handle-typed fields of records outside an `extern` group in the whole
  tree: `examples/ledger/db/sqlite.hero:204`, `:207`,
  `tests/golden/check/ffi-handle-refusals.hero:27`;
- **2** Heroes functions answering a wrapper of a handle:
  `examples/ledger/db/sqlite.hero:212` (`opened() -> Db?`) and `:271`
  (`prepared(...) -> Statement?`).

Five sites. That is the number the engineer's condition asked for and nobody
measured, and it is small enough that the sitting could have decided Route A's
soundness limb rather than deferring it to "a different proposal, owed its own
sitting".

**A cost of Route E nobody named either**, and it is the reason it is not free:
`tests/golden/run/fixedbugs-a-handle-in-a-record-field.hero` pins *a handle as a
record field* as a **fixed defect of 2026-09-13**, found by panel 146's own
completeness critic. R4 refuses that golden. A route that un-fixes yesterday's
defect is a real price and no seat weighed it, because no brief mentioned that
file.

### 1c. A sixth route: Route C plus an instrument that exists

Route C as drafted is a bare refusal. The spec-warden's §4 says the prediction
branch is the only payment open and that it *"has collected nothing in six
attempts"* — so a bare refusal pays nothing either. Nobody proposed the cheap
middle: **refuse the form, and ship the counter-example as a suite.** The
exposure is 22 paths in one file (§ 4c); `selfhost/check/ffi_sweep.hero` is a
**93-line** whole-file pass and is the shape a "`?` between an acquire and its
release" check would take. Zero spec tokens, §1.7 subtraction still zero, and
§1.6's payment satisfied by *an instrument that exists* rather than by a promise.
Whether it is right is a verdict; that it was never on the table is a gap.

---

## 2. Claims asserted and not measured

### compiler-engineer

| claim | status | the command that settles it |
|---|---|---|
| the ceiling table (17 rows), 24 `StmtKind` arm sites, 48 arm lines across 20 files, `grep 'scope' selfhost/ir/` = 0, `own.hero:216`, `inst.hero:165`, `owning.hero:40`, `alloc.c` leak gate, zero handle records in `selfhost/` | **all reproduced exactly** by me | `codelines.py`; `grep -rn '\.expr_stmt' selfhost/ \| grep '=>'` → 24; `grep -rn '\.decref_slot' \| grep '=>'` → 48 / 20 files |
| Route A's veto: `rc = 0` → `rc = 21` under one inserted line | **reproduced** — I ran the seat's own `v2`/`v2A` and diffed them: the whole delta is `sqlite3_close(h0_db); /* ROUTE A sweep */` | `./v2; ./v2A; diff v2.c v2A.c` |
| *"'prove' there is the dataflow `check/leasing.hero:29` says does not exist"* | **false as a "cannot"** — two positional escape refusals ship (§ 1b) | `grep -rn 'escapes' selfhost/check/*.hero` |
| the fourth route *"cannot corrupt memory because it never calls anything"* | **unchecked against its own veto case**: keyed on the type it miscounts a borrow (§ 1a) | a `released`-typed borrow probe; the seat already had one in `e6_borrow.hero` |
| *"24 exhaustive match sites... the one non-arm is `grammar_expr.hero:924`"* | self-contradicting: 24 lines, 23 arms | as above |
| Route A's line estimates; the Linux `--sanitize` leg | **self-labelled estimate / unrun** ✓ | — |

### spec-warden

| claim | status | the command that settles it |
|---|---|---|
| 5863 / +80 / +108 vendored | **re-measured by me**, identical: `route-a.md` 5943, `route-b.md` 5971, and `route-a.md` is byte-identical to `variant-3.md`, `route-b.md` to `variant-1.md` | `heroes measure <draft>`; `diff` |
| 7806 real, 386 headroom, 60 FFI floor | **correct** — `heroes measure spec/heroes-spec.md` prints them, dated 2026-09-13 in the tool's own row. Recalled rather than re-measured (the API row cannot be refreshed here), which the seat did not say | `heroes measure spec/heroes-spec.md` |
| `selfhost/` is 60,360 lines, zero handle records, one nullary pair | **all three reproduced** (`find selfhost -name '*.hero' \| wc -l` = 207, `cat ... \| wc -l` = 60360; `process.hero:52` is `function hero_dir_release()`) | as shown |
| **"Route A closes 0 of the 23"**, headed *"Measured from the tree at 7e6e71a8, not argued"* | **half measured, half reasoned.** The corpus half I verified line by line (`:204`, `:207`, `:212`, `:271-277`). The other half — that the draft's sentence refuses them — **cannot be compiled** (the `relmark.hero` error above), so it is a reading of four lines, not a measurement | nothing settles it until Route A is implemented; the seat's own prediction 1 names `heroes build`, but `heroes build` cannot parse `released` |
| *"§1.6 records six ledger rows bought that way and none ever collected"* | **unverified by me.** I searched `docs/measurements/010-spec-budget-ledger.md` for `prediction` (48 hits) and `metric 2` (3 rows) and could not separate six specific uncollected rows in the time. It is load-bearing for the Route B condition and should be checked before the synthesis leans on it | a row-by-row read of the ledger |

### ffi-pragmatist

| claim | status | the command that settles it |
|---|---|---|
| the seven uncompilable shipped sites | **reproduced exactly**: `:230, :235, :280` in the ledger binding, `:85, :101, :107` in `examples/sqlite/main.hero`, `:82` in curl | `grep -rn 'sqlite3_close\|sqlite3_finalize\|curl_easy_cleanup' examples/` |
| `e7` (borrow double-close, `21`) and `e3` (`value = 0` where the table holds 3) | **re-ran both, identical output** | `./e7_double_close; ./e3_routeA_borrow` |
| raylib: 0 of 21 obligations have Route A's shape | **holds, and understates.** 30 `Unload*`/`Close*` functions in `/opt/homebrew/include/raylib.h`; the by-value ones are there (`UnloadTexture(Texture2D)` :1446, `UnloadFont(Font)` :1498, `UnloadModel(Model)` :1594) and so are the nullary ones (`CloseWindow(void)` :987, `CloseAudioDevice(void)` :1663) | `grep -nE 'RLAPI void (Unload\|Close)' raylib.h` |
| the 21-library survey (13 596 functions, 1 073 releasers, 262 types, 212/29/21) | arithmetic closes in every row; scripts and `handles.json` (16 libraries) present; **self-corrected once** by hand. Not re-derived by me | rerun `handles.py` |
| *"the only way out is escape analysis, which §4.19 records as refused by name"* | **misreads the ruling** (§ 1b): §4.19 refuses an *inferred release* | `sed -n '2305,2310p' docs/design/design.md` |
| *"Route C's falsifier already fired — E1 is the program"* | **E1 is hand-written C, not a Heroes program.** CLAUDE.md §12 asks a Part 6 row to name the **program or compiler fact** that would make it wrong; a C file proves C can do it, which was never in doubt. The seat also vetoes the Heroes form E1 demonstrates, so its own §6 and its verdict row for C pull opposite ways | write E1 in Heroes — which is the `relmark.hero` error above |
| *"`check/freer.hero` ... (275 lines)"* | that is `wc -l`. `code_lines`, the instrument the ceiling table uses, says **221** — the same number the compiler-engineer's table carries. Two seats, two instruments, one file, and neither says which | `wc -l` vs `codelines.py` |
| *"how many functions a binding would have to mark"* | **self-labelled unrun** ✓ | — |

### llm-ergonomist

| claim | status | the command that settles it |
|---|---|---|
| its guess that variant 2 is the current language | **correct** — `variant-2.md` is byte-identical to `spec/heroes-spec.md`, and `VARIANT-KEY-do-not-give-to-ergonomist.txt` agrees | `diff spec/heroes-spec.md variants/variant-2.md` |
| the 6 / 3 / 0 release-site count | a count of its own three programs; accepted | write the program |
| **"Variant 3 is the only one of the three in which a double release is impossible"** — its own *"strongest single thing on this page"* | **false**, and falsified by a program that compiles today (§ 3a) | `./heroes run e6_borrow.hero` |
| *"lifetime discipline is a property of the type and not of the call site"* | **an inference, and it is the exact proposition the ffi survey measured false** (`sqlite3_db_handle`, `sqlite3_next_stmt`, `sqlite3_column_value`, `cairo_get_target`, `hb_font_get_face`). The seat could not have known — see § 4a | `ffi/returners.py` |

### historian

| claim | status | source |
|---|---|---|
| Vala's `.vapi`: `free_function = "sqlite3_finalize"` on `cname = "sqlite3_stmt"`, and `sqlite3_close` / `sqlite3_mutex_free` / `sqlite3_backup_finish` | **I fetched it myself and all four are there, verbatim as quoted** | `raw.githubusercontent.com/GNOME/vala/master/vapi/sqlite3.vapi`, fetched 2026-09-14 |
| Gustedt, *Defer available in gcc and clang*, 2026-02-15; TS 25755 edited by JeanHeyd Meneide; clang-22 under `-fdefer-ts` | **confirmed** by search; `gustedt.wordpress.com/2026/02/15/defer-available-in-gcc-and-clang/` exists with that date | web search, 2026-09-14 |
| *"C ... abandoned it in 2026... Refusing now refuses what C adopted"* | **overstated.** TS 25755 is a Technical Specification, optional, behind a clang flag, with GCC native support still in development branches. "C adopted" is not what a TS means, and this is the seat's single strongest fact against Route C | the same blog post |
| Vala emits the free on early-return paths | **still unrun**, and it is the load-bearing half of the Route A precedent. `which valac` → not found on this Mac. The seat flagged it; nobody lifted it | install valac, or read Vala's `codegen` |
| its falsifiable prediction (*"at least one of the five handle types has a producer returning a borrow"* — marked **unrun**) | **already scored inside this sitting, and it HELD.** The ffi-pragmatist's §3 names `sqlite3_db_handle` and `sqlite3_context_db_handle` for `sqlite3 *` and `sqlite3_next_stmt` for `sqlite3_stmt *`. It should be recorded as collected, not queued | the seat's own `returners.py` |
| Route C's precedent is "expiring" | the sitting's Route C is *a refusal with a named falsifier*, not a permanent one. The seat argued against a route nobody put | — |

---

## 3. Contradictions, and which side is checkable

### 3a. The big one: it is NOT two questions, at three specific points

The framing "the ergonomist wrote new programs, the others measured the old
corpus" is true and is not the whole truth. Three places where the ergonomist's
approval and the three refusals touch the same object, and in each the other
side is the checkable one.

**(i) "Double release is impossible" is false on a program that compiles today.**
The ergonomist's strongest claim rests on *"refuses your own call of it"*. The
compiler's own call is the one that doubles. `sqlite3_db_handle` returns a
borrowed `sqlite3 *`; declare it and you have two slots of the same `released`
type in one scope. I ran it:

```
$ ./heroes run e6_borrow.hero
borrowed == owned: true
```

and the C the sweep would emit, re-run from the ffi seat's artifact:

```
$ ./e7_double_close
borrowed == owned : true
count             : 2
sweep closes borrowed -> 0
sweep closes db       -> 21          # SQLITE_MISUSE
exit=0
```

Checkable side: the ffi-pragmatist's. **Route A does not make double release
impossible; it makes it automatic.**

**(ii) The ergonomist's own workaround breaks on its own reasoning applied
twice.** It concedes *"you cannot factor out an opener for a released type"* and
recommends *"mark `Stmt`, leave `Db` unmarked"*. But the 22 leaking paths are
all `Stmt`, and `Stmt` is produced by an opener:
`examples/ledger/db/sqlite.hero:271-277`, `function prepared(...) -> Statement?`
… `return ok(Statement(handle: statement))`, into
`record Statement { handle: CStmt }` at `:207`. Marking `Stmt` refuses exactly
the function that creates the exposure. Checkable side: the spec-warden's; I
verified both lines.

**(iii) The ergonomist could not see (i) or (ii), and that is the coordinator's
doing, not the seat's** — see § 4a.

**What the synthesis must tell a reader**, in one paragraph and not a footnote:
the ergonomist measured that **writing a new, flat, single-scope program is
strictly easier under Route A**, 0 release sites against 6, and that result
stands and nothing in the other four reports touches it. What it did not and
could not measure is that **the corpus is not made of flat single-scope
programs** — it is made of `opened()` and `prepared()`, the two shapes Route A
refuses — and that **Route A's one robustness claim is false**. Reporting "one
seat approved, three refused" without those two sentences will read as a split
panel. It is not a split panel: it is one seat answering a question the brief
asked and four seats answering the question the corpus asks.

### 3b. Two seats found the same Route B bug from opposite ends and neither knows

- llm-ergonomist, from the spec text alone: *"`cleanup sqlite3_close(db)` ...
  reads `db` HERE: nullptr"*, and *"The spec sets the trap in one section and
  disarms it in another"*, predicting ≥ 1 in 5 attempts spring it.
- spec-warden, from the corpus: *"All 22 target sites need `cleanup
  sqlite.finalized(@statement)` — an `@` argument ... copy-in/copy-out at a
  deferred point is unspecified."*

This is the **same defect**: the draft's sentence *"arguments are read where it
is written, not where it runs"* is wrong for a cell that is filled afterwards.
It is currently split across two reports and cited by neither. It is the
strongest single finding about Route B in this sitting and the synthesis will
lose it if it summarises seat by seat.

### 3c. The census says a handle *can have no* runtime gate; a seat proposes one

`docs/measurements/030-...md` § 4, handed to every seat as fact: *"A
`sqlite3_stmt *` has no such gate **and can have none**, because the runtime
never saw it allocated."* The compiler-engineer's fourth route is that gate, and
the runtime's own counter is driven by two ordinary calls
(`runtime/parts/alloc.c:180,185`), not by the allocator. A "cannot" in the
briefing document, falsified by a seat in the same sitting, needs a correction
line underneath it (records are append-only) rather than silence.

### 3d. Smaller ones

- **File sizes**: `check/freer.hero` is 275 (`wc -l`, ffi seat) and 221
  (`code_lines`, engineer's table). Both true, different instruments; a reader
  comparing them across reports is misled. I ran both.
- **`selfhost/` size**: the briefs say **60,312**; the spec-warden re-measured
  **60,360** and did not flag the correction. I get 60,360 (207 files).
- **Route B soundness**: the engineer says *"sound if built"*, the ffi seat
  **approves**, the warden says the **draft is unsound**. Compatible only if you
  notice the first two judge the mechanism and the third judges the four lines.
  Say which.

---

## 4. Framing facts a seat was handed and did not check

### 4a. The ergonomist's binding had no borrow in it

The variants' shared FFI group (`variant-*.md` and the seat's own experiment)
declares `sqlite3_open`, `sqlite3_prepare_v2`, `sqlite3_step`,
`sqlite3_column_text`, `sqlite3_finalize`, `sqlite3_close` — **every producer
hands back something you own.** Not one function in the sample returns a handle
the caller must not release. The one fact that decides Route A was absent from
the only seat that judged Route A on the spec text, and the seat's brief also
never mentions borrowing. Its approval is therefore not evidence about Route A;
it is evidence about Route A over a binding with no borrows, which is a binding
the ffi survey measured does not exist for sqlite, cairo, harfbuzz or libxml2.

### 4b. Route A's draft never defines "acquire", and each seat supplied its own

The whole of Route A, from `diff variants/variant-2.md variants/variant-3.md`:

> A handle may name the function that releases it ... the compiler calls it once
> on every path out of **the scope that acquired the handle**, an early return
> and `?` included, and refuses your own call of it. A released handle may not be
> returned or stored.

Nothing says which call acquires. Three readings were used:

- **ergonomist**: the scope that declares the cell — `db: Db @ nullptr` filled
  by an `@out` parameter counts as acquiring.
- **compiler-engineer**: any slot of that type in the function, because the
  sweep is a walk over the slot table (`ir/own.hero:216`) — so a borrow is
  swept, which is its veto.
- **spec-warden**: the scope containing the acquiring call — so `prepared`'s
  body, which is why it counts 0 of 22.

Each verdict follows from its reading. A sitting that decides *whether a form
enters* over a draft whose central term is undefined is deciding three questions
and reporting one answer.

### 4c. The census, audited — four findings

Every seat was handed
`docs/measurements/030-three-release-obligations-and-only-one-of-them-is-silent.md`
as fact. Its arithmetic is sound and its instrument note is exemplary. Four
things are wrong or missing.

**(1) The 22 `?` reproduce, and so does the repair — so the live exposure is 22,
in one file, not 23 in two.** Per-function `?` counts between acquire and
release reproduce exactly (4, 6, 7, 3, 2 = 22), and
`examples/sqlite/main.hero:102` now closes on the failed-open branch. The census
says both "23 escaping paths" and "repaired in this same step" and never
subtracts. The spec-warden caught this and made it prediction 4; it is not a
prediction, it is an arithmetic correction available today.

**(2) The pair enumeration is short by at least 7, and the list is a measurement
(CL-057).** The census says *"Twelve pairs read by hand, eleven under
`examples/`"*. `examples/ledger/main.hero` alone holds **13 acquire sites and 15
release sites**. Unlisted: `main`'s own `db` pair (`:39` → `:52`) and the `db`
pair in each of the **six** test blocks (`:242→:247`, `:250→:254`, `:257→:268`,
`:271→:275`, `:278→:286`, `:289→:297`). At least **19** pairs, not 12. None of
the seven adds an escaping path under the census's own convention — but the
census never states that convention, which is that `.must()` is not counted as a
path out even though `ir/own.hero` rule 5's comment names `.must()` as one of the
four constructs that open a block mid-expression.

**(3) "10 of 21 FFI programs have no acquire-and-release pair at all" is 8, and
the partition does not close.** 21 files carry an `extern` group (reproduced).
Eight have no releaser declared at all — `ctime`, `gallery/08-ffi`,
`gallery/13-lease`, `nbody`, `raylib`, `sdl`, `spectral`, `tally` (I grepped each
for a release-shaped declaration: nothing). Ten are the threaded ones (`tally`'s
`spawn` is in a comment, so the census's list of ten is right and my grep's
eleventh is noise). Three have real pairs: `examples/sqlite/main.hero`,
`examples/ledger/db/sqlite.hero`, `examples/curl/main.hero`. 8 + 10 + 3 = 21. The
census's 10 + 10 leaves one slot for three programs.

**(4) The one that matters most: the census never asked what happens next on the
22 paths, and the answer dissolves its headline.** Every caller of the five
leaking functions is one of two things (`grep -rn 'build(\|show(\|queried_'
examples/ledger/main.hero`):

- `main` at `:40` and `:46` — on `is_err()` it prints, calls
  `_ = sqlite.closed(@db)`, and `exit(1)` (`:42-45`, `:48-51`);
- the six `test` blocks — `.must()`, which aborts.

So **all 22 escaping paths terminate the process within a few statements.** The
census's headline row — *a miss is **SILENT**: exit 0, no diagnostic* — was
measured on a synthetic reduction, not on any shipped path; there is no program
in this corpus that leaks a handle and goes on running. That is a different
class from `lease`'s (which aborts a program that was going to keep going) and a
different class from wart 20's (a silent wrong answer at exit 0). The
ffi-pragmatist's `e3` is the real silent-wrong-answer case, and it is a case
**Route A manufactures**, not one the corpus has.

### 4d. The shared brief's other framing facts

| brief statement | check |
|---|---|
| the 1.331 vendored→real ratio | **wrong to use**, as the spec-warden found: `010-spec-budget-ledger.md:172` *"No row is convertible"*. The ledger's two real conversions are re-measurements (+118→+151, +99→+126), not applications of a factor |
| `selfhost/` is 60,312 lines | **60,360** (§ 3d) |
| "5 types, 11 pairs" / "twelve pairs" | consistent with each other; both short (§ 4c-2). The census's own instrument note says the tree holds **9** handle records and never reconciles 9 with 5 |
| `consumes` is 114 code lines, refusing 2 of 17 | **verified**: `codelines.py selfhost/check/consuming.hero` = 114; design.md `:3516` and `:3534` carry the 2 of 17 |
| the Part 6 borrow row moved soundness → cost on 2026-09-13 | **verified**, design.md `:3534-3536` |
| Part 8 wart 20 in the present tense; no destructors at `emit/types.hero:6`; defect 032 closed | **all three verified** |
| *"The vocabulary is silent ... nothing about a construct"* | **true as worded** — 30 hits for `defer`/`RAII`/`scope guard` across `docs/panel/`, all of them the verdict word "defer", and zero in `spec/`. But it is not silent about this sitting: `docs/panel/137-...:193` already recorded the same SQLite measurement and named *"M-cleanup-verdict, whose sitting is the ruling on a scope-bound release"*. The brief presents silence where there is a docket entry |
| ceilings raised this morning: `emit/ctype.hero` 388/395, `emit/structural.hero` 323/335 | **verified** against `suite_layout.hero`'s `DECIDED` |
| build discipline (`rm -rf target build` in the copy) | **both seats complied** — `work/tree/build` and `tree/build` were cleared and rebuilt in place |

**And one fact no brief carried**: `tests/golden/run/fixedbugs-a-handle-in-a-record-field.hero`,
landed 2026-09-13 by panel 146's completeness critic, pins *a handle as a record
field* as a fixed defect. Route A's *"may not be returned or stored"* refuses the
shape that golden exists to protect. Nobody weighed it because nobody was shown
it.

---

## 5. The question the sitting should have asked and did not

> **Does the obligation belong to the type, to the call, or to the value — and
> which of those three does this language already know how to talk about?**

The sitting asked *where does the release run* (scope exit, block exit, or
nowhere) and got three answers that all turn out to depend on a question it never
put. Every finding above collapses onto it:

- Route A puts the obligation on the **type**, and the ffi survey measured that a
  type does not determine ownership: `sqlite3_db_handle` and `sqlite3_next_stmt`
  hand back the same types borrowed. That single fact, not the sweep, is what
  produced two vetoes.
- Route B puts it on the **value at a site**, and both the ergonomist and the
  warden found independently that an `@` cell breaks it, because the value at the
  registration site is not the value at the release site.
- All three fourth routes put it on the **call**, and two of them converge there
  without noticing each other.
- The compiler already answers this question twice for other resources, and both
  times it chose the call or the position and never the type: `owned <freer>` on
  a declaration's **result** (`check/freer.hero`), `x: cstr @ s.lease()`
  recognised by *the declaration's INITIALISER being the lease call*
  (`check/leasing.hero:25-27`, and the comment says why in capitals: **"IT ASKS
  THE VALUE AND NOT THE WORLD"**), `consumes` on a **parameter position**, the
  `cstr` lend on the **position of the expression** (panel 122). Four rules, four
  times the same answer, and Route A is the first proposal in this project to
  put an FFI lifetime fact on a type.

A sitting that had asked it would not have needed the vetoes: CLAUDE.md § 11's
own rule — *a narrowing asks the value, never the world* — decides Route A
against, in one line, before any C is compiled.

**The second question, cheaper and also unasked**: *on each of the 22 paths,
what does the program do next?* It exits (§ 4c-4). The sitting priced three
forms against an exposure whose measured consequence in every shipped program is
a process that was about to terminate anyway — while the handle hazard that does
produce a wrong answer at exit 0 is wart 20's affine copy, which none of the
three routes touches and which Route A makes reachable by a second path.

---

## What the synthesis must not lose

1. **Route A cannot be compiled**; four of five Route A findings are readings or
   hand-inserted C, and the two vetoes are the hand-inserted-C ones. Both
   reproduce (I ran them).
2. **The ergonomist's approval and the three refusals are not a split.** Its 0
   release sites are real for a flat new program; its one robustness claim is
   false; its recommended workaround refuses `prepared()`. Say all three.
3. **The three "fourth routes" are one route with three instruments**, and its
   sentence is *the obligation is created by a call, not by a type*.
4. **Route E exists and was called impossible by three seats.** The positional
   escape refusal ships twice, at 241 and 195 code lines, and its blast radius on
   this tree is **five sites**. The engineer's veto condition is answerable this
   week, not "owed its own sitting".
5. **The census needs three corrections underneath it** (append-only): 22 not 23,
   8 not 10 pairless programs, at least 19 pairs not 12 — and a fourth line
   saying that all 22 paths end in `exit(1)` or `abort`.
6. **The historian is auditable this time and it holds** on both load-bearing
   sources (I fetched the Vala `.vapi` and confirmed the Gustedt post). Its one
   overstatement is *"C adopted"* for a Technical Specification behind a flag,
   and its own prediction was collected inside this sitting by another seat.

# Panel 210, spec-warden's report

Seat: spec-warden (design.md §1.2, §1.6, Principle 0). Tree `1dd890751`, my copy
at `.claude/worktrees/scratch-b15/210-spec-warden/tree/`, compiler built there
from the seed and then from `selfhost/` (02:11 to 02:14 by `date`). Probes in
`../probes/`, drafts in `../drafts/`, scratch in `../tmp/`, all inside my
directory. No paid run, no git command in the copy. Written as I went, from
02:15.

## 0. The ceiling, read today

`grep -n "1\.6" docs/design.md` in the frozen tree: §1.6 at line 253 says
**10240 tokens, measured by `claude-opus-5` through `POST
/v1/messages/count_tokens`**, the author's decision of 2026-09-14, and the
payment rule unconditional (panel 012 as amended by panel 046): an addition
owes a named removal or a registered prediction naming an instrument that
exists today.

## 1. The spec before

`./heroes measure spec/heroes-spec.md`, 02:14:

```
  claude-legacy      7502   (64995 ranks)
  cl100k_base        7636   (100256 ranks)
  real              10024   claude-opus-5, 2026-10-10 — the binding number
Headroom: 216 against the 10240 ceiling — but the FFI floor mortgages 60 of it
(panel 030 R3), so what is measured against the ceiling is 10084
```

The `real` row is the pinned count of panel 209's step 4 (ledger
`docs/measurements/010-spec-budget-ledger.md`, last row, `--refresh` at 23:14
on 2026-10-10); `measure` printed no STALE, so it is this file's. **156 real
tokens are free.** Every draft below is priced on the vendored tables only:
no `--refresh` tonight, so **no draft has a real price**, and the vendored
delta is not a bound on the real one (the ledger's last six rows read real
over vendored deltas from 0.67, panel 209 step 2, +12 to +8, to 1.9, panel
205's H2f, +9 to +17).

## 2. Q4: does § 5 already say it? No, and it says the opposite

§ 5, `:137-138`: *An unused binding or parameter is a compile error, and so is
a cell nothing re-binds, an `@` argument counting; a read is a use and a write
is not, except through an `@` parameter.*

- An `@` parameter only READ is **used** by this sentence (*a read is a use*),
  so the text **derives acceptance** for the class. It is not silence: spec and
  compiler agree today, exit 0.
- *A cell* is what `@=` declares (§ 5 `:134`); a parameter is declared by
  `Param = [ "@" ] ident ":" Type` (§ 4). `grep -n cell spec/heroes-spec.md`:
  seven lines (115, 131, 134, 138, 415, 417, 423), none calling an `@`
  parameter a cell. So *a cell nothing re-binds* does not reach it on a
  literal reading.
- Measured on today's compiler (`../probes/`): `@n` read only, `p1` exit 0;
  neither read nor written, `p7` `unused_binding` (already refused, the
  sentence's first clause); written only, `p6` exit 0 (the `except` clause).

**So the rule cannot land without a word**: a refusal the document derives as
legal is the compiler departing from the spec (CLAUDE.md § 12, *spec beats
compiler*), and every writer who reads § 5 and writes a read-only `@` pays a
round trip (§1.2, 500 to 2000 tokens each).

## 3. Q1: the class, counted by a built rule

A probe rule, unlanded, in my copy's `selfhost/resolve/unused_sweep.hero`
(`written_nowhere`: a local of kind `.param`, `mutable`, `reads > 0`,
`writes == 0`, outside a hole's module; the same `writes` counter
`rebound_nowhere` reads), built as `heroes-probe` (02:17 to 02:18). It is the
rule **as the issue words it**: writes by `@` to the name, a field or an
element, or an `@` argument.

- `./heroes-probe check selfhost/main.hero` (10.06 s real, 9.34 user): **3**
  sites, `parse/signature.hero:85` (`@a`, copied into `scratch` and never
  written back), `check/lending.hero:184` (`@c`), `check/path_end.hero:69`
  (`@c`, `return c.broke`). Denominator, by grep and so a denominator only:
  1306 `@` parameters on `^function` lines of `selfhost/`. **0.23%.**
- `heroes-probe check` over the 2034 `.hero` files of `examples/` and `tests/`
  (recovery corpus excluded, `xargs -P 4`, 26 s): **30** unique sites (34
  lines, a library module reported once per main). 3 `examples/` (all
  `examples/ledger/db/sqlite.hero`), 18 `tests/golden/check`, 8
  `tests/golden/run`, 1 `tests/golden/surface-fixtures`. A case refused before
  the sweep runs is not counted, so `tests/golden/check` is a lower bound.

**What the 33 are, read site by site:**

| shape | sites | `@` needed? |
|---|---|---|
| the parameter, or a field of it, handed to a `consumes` parameter (`closed(@db)` → `db_close(db: db.handle)`, `finish(@n)` → `node_free(n: n)`) | 12: `examples` 2, `run` 8, `check` 1, `surface-fixtures` 1 | **yes**: § 13 `:425-426`, *mark the parameter `@`* |
| handed to a `transfers` parameter (`ffi-transfers-a-borrowed-handle.hero:22`) | 1 | **yes**, same clause |
| read only: scratch copies, a flag read, `sqlite.hero:322` `stepped(@statement)` (`sqlite3_step` takes the handle by value; § 3 says a function without `@` may still change what C holds), the `check` goldens' fixtures (`fixedbugs-135`, `-260`) | 20: `selfhost` 3, `examples` 1, `check` 16 | no |

**13 of 33 (39%) are correct programs the rule as worded refuses.** Measured,
not read: `handle-a-helper-that-ends-its-parameter-is-the-runtime-s.hero` with
its `@` removed at the parameter, the call and the cell is refused
`consumed_borrowed_handle`, whose note says *Mark the parameter `@n` and the
call site `@n` too*. The two diagnostics' repairs undo each other.

And a shape the tree does not hold, built here (`../probes/lend/`): a helper
`fill_it(@s: Sl)` whose only write is `sl_fill(p: s.name.ptr(), n: ...)`, a
lend C writes through. Today exit 0 and runs, `16` then `36` (C's write came
back). The probe flags it. Without the `@` today's compiler refuses it (*a C
write is a mutation. Declare `s` with `@` — a `@` cell, or a `@` parameter*).
The landed cell rule already counts that lend: the same lend on a cell in
`main` (`lend_cell.hero`) is not `never_rebound`, exit 0, `16` then `36`. The
parameter's counter does not, which is the compiler-engineer's ground.

## 4. Q2: what counts as a write, every shape I could build

| shape | probe | verdict the rule owes |
|---|---|---|
| `n @ e` (`p2`), `p.x @ e` (`p3`), `xs[0] @ e` (`p9`), only on one branch (`p8`) | not flagged | written |
| `g(@n)`, an `@` argument (`p4`), an extern's out-parameter or `end_lease(@x)` by the same token | not flagged | written (the cell clause's *an `@` argument counting*) |
| `for x in xs` over it (`p5`), read in a generic (`p11`) | flagged | read only: refused |
| a copy mutated and never written back, `m @= l; m.pos @ m.pos + 1` (`p12`) | flagged | refused; **today exit 0, prints 0**, the caller meant 1 |
| handed, or a field of it, to `consumes` or `transfers` | flagged (13 sites) | **must count as written** (§ 13 requires the `@`) |
| a field lent to a C write, `s.f.ptr()` | flagged (`lend_at`) | **must count as written** (§ 13 `:411`) |
| a hole in the module (`p10`) | not flagged | spared, as the cell rule |
| a method with `@` self | none exists: § 9, UFCS does not apply when the first parameter is `@` | n/a |
| a function with an `@` parameter as a value (`p13`) | refused today, `mutable_parameter_as_value` | n/a: no function type carries `@`, so no interface forces one |
| an extern's `@` parameter | no body | out of the rule; C writes it |
| a cell only consumed (`cellconsume/cc.hero`) | today `never_rebound`, fix `=` | right: an `=` binding may be consumed, a non-`@` parameter may not. So **"ends" counts for a parameter and not for a cell**, and the spec's clause must attach it to the parameter alone |
| `retains`, `borrows` on the parameter | **unrun** | a question for the engineer |

## 5. Q3: the fix is a `guess`, always

Applied by hand to `p12` (`../probes/p12_fixed_by_removal.hero`: `@` removed at
the parameter and the call, `k @=` to `k =`, as a chain of `certain` fixes
would write): `check` exit 0, `run` prints **0**. The removal compiles and
keeps the bug: the defect a forgotten write-back is, not the one the message
names. `.claude/rules/diagnostics-and-goldens.md`: *a fix that leaves the
defect standing is a `guess`, however well it compiles*, and panel 071's
`_ = xs.push(4)` is the same shape. The compiler cannot tell a needless `@`
from a missing write, so **both repairs are offered and both are `guess`**,
whatever the file set; the issue's *`certain` where every caller is in the
compiled set* is refused on this ground. Beside it,
`fixedbugs-135-a-dropped-value-of-a-place-s-own-type.hero:32` (`b.items.push(4)`
dropped in `through_a_field(@b: Bag)`) shows the rule firing beside a
`discarded_value` whose own repair is the write: two messages for one mistake
(§4.17), unless the rule is silent where a dropped value of the parameter's
own type rooted at it was told.

## 6. The drafts and their prices

Each a whole copy of the spec in `../drafts/`, `./heroes measure <draft>`,
vendored only (legacy, cl100k; base 7502, 7636):

| draft | text (§ 5 `:138` unless said) | legacy | cl100k | note |
|---|---|---|---|---|
| D1 | *a cell nothing re-binds or an `@` parameter nothing writes, an `@` argument counting* | +8 | +8 | **contradicts § 13 `:425-426`**: a consuming helper's `@` is required and written nowhere |
| D2 | *... or an `@` parameter nothing writes or ends, an `@` argument counting* | +10 | +10 | "ends" is § 13's word (*the call ends that value's life*, *has ended as if given back*) |
| D2c | D2 with *(section 13)* after *ends* | +13 | +14 | the pointer spec-shape.md allows, priced as words |
| D2b | D2 plus *an `@` argument or a lend C writes through counting* | +16 | +16 | names the lend for the cell clause too; derivable from § 13 `:411` without it |
| D3 | in § 9: *`advance(@l)`, and one the body never writes or ends is an error* | +13 | +14 | the wrong home: use and non-use are § 5's (spec-shape.md) |
| D4 | *..., an `@` argument counting, or an `@` parameter nothing writes* | +9 | +9 | D1's defect |
| D5 | *a cell or an `@` parameter nothing re-binds* | +6 | +6 | cheapest; D1's defect, and adding "or ends" would wrongly spare a cell only consumed |
| D6 | *a read is a use and a write is not, and for an `@` parameter the reverse, ending it counting* | +6 | +6 | merges into the use rule; "the reverse" is an inversion a reader must do, unmeasured on a reader tonight |

Budget: every draft is far inside the 156 free real tokens, at any ratio the
ledger has seen. **No veto on §1.6.**

## 7. Payment (§1.6)

**Named removal: nothing, and that is a problem.** I read § 5, § 9 and § 13 for
a clause the rule makes redundant: *except through an `@` parameter* still
spares a written-never-read `@` parameter, and § 13's *mark the parameter `@`*
is the reason "ends" is needed, not a duplicate of it. D6 is the one draft that
merges rather than appends, and it is the one a reader would most likely misread.

**So the payment is registered predictions on instruments that exist today**
(a compile, a diagnostic transcript, the census, `heroes measure`):

- P1, the census: the corrected rule (consumes, transfers and a lend C writes
  through counted) refuses **exactly 20** sites at `1dd890751`, the 3 in
  `selfhost/`, `sqlite.hero:322` and 16 in `tests/golden/check`, and **0** of the
  13 FFI sites; scored at the landing's gate by `check` over the same files.
- P2, the thesis: `p12_shadow_copy.hero` is refused with **exactly one**
  diagnostic, at the parameter, carrying no `certain` fix; `lend_at.hero` and
  the consuming helper exit 0; scored by a compile at the landing.
- P3, the price: D2c's real delta on the landing's `--refresh` is **+9 to +20**.

## 8. Principle 0

Not needed for self-hosting: the compiler builds today holding 3 such
parameters. Thesis: one measured instance of the shape, `p12`, a silent wrong
value today (prints 0) that the rule turns into a compile error. Its
frequency is **unmeasured**: 0 of the 33 sites in the tree is a forgotten
write-back (a tree whose bugs were found and fixed would show none), no row of
`docs/metrics/operators.md` plants it (its table is lines 11 to 27, every row
read, and `grep -niE "@ ?param|copy.out|write.?back|drop.*write"` over the file
is empty), and the lean sitting has no blind reader.
The panel can accept it as a measured argument (CLAUDE.md § 2) on the
condition that two operator rows land with the rule so it gets scored:
`drop-param-write` (delete the one write of an `@` parameter the body also
reads) and `param-as-inout` (add `@` to a read-only parameter and its calls,
`bind-as-cell` one level up).

## 9. Verdicts

| route | verdict | why |
|---|---|---|
| R0, accepted as today, 0 tokens | approve | spec and compiler agree; costs nothing; leaves `p12` silent |
| R1 as the issue words it (writes by `@` and `@` arguments only), text D1, D4 or D5 | **veto** (Principle 0, measured) | 13 of its 33 hits in the tree are correct programs § 13 requires; its fix and `consumed_borrowed_handle`'s undo each other; D1 makes § 5 contradict § 13 |
| R1 with a `certain` removal where every caller is compiled | object | `p12_fixed_by_removal`: compiles, prints 0, the bug kept |
| R1 corrected: consumes, transfers and a C-write lend count as writes, both repairs `guess`, silent beside a told dropped value, text **D2c** in § 5 | **approve, provisional** (no real price) | sound on every shape built; +13/+14 vendored inside 156 free; paid by P1 to P3 |
| the same with D2 (no pointer) | approve, provisional | the conservative text, 3 to 4 tokens cheaper; record it as the alternative |
| D3 (the rule in § 9) | object | the wrong home (spec-shape.md: use and non-use live in § 5) |
| D6 (the reverse) | object | cheapest merge, highest misreading risk, no reader measured |

## 10. Checked after writing (02:25 by `date`)

- Each of the 13 FFI sites' callee read from its own `extern` block: 12
  `consumes` (`sqlite3_close`, `sqlite3_finalize`, `free`, `db_close` four
  times, `f_close`, `node_free` twice, `obj_delete`, `probe_close`), 1
  `transfers h_close` (`h_put`); `sqlite3_step`, the one `examples/` site left
  out of them, is declared with neither.
- Exit codes re-read with `$?` (the first reading used zsh's empty
  `PIPESTATUS`): `lend_at`, `lend_cell` and `p12` exit 0 today; `lend_at` exits
  1 under the probe.
- The landing's golden cost from the 16 `check` sites: three cases,
  `fixedbugs-135-a-dropped-value-of-a-place-s-own-type` (8),
  `fixedbugs-135-a-label-the-order-written-may-not-mean` (2) and
  `fixedbugs-260-each-at-argument-against-the-earlier-ones-at-once` (6), whose
  `@` parameters are fixtures of other rules; each needs bodies that write
  them or the new code annotated, never `UPDATE_GOLDEN`.

- My copy's `selfhost/resolve/unused_sweep.hero` was restored from
  `../unused_sweep.hero.orig` at 02:26 (`cmp` equal); the probe rule survives
  only in the built `../tree/heroes-probe`, and the patch is the 7-line
  `written_nowhere` described in section 3.

Finished 02:26 by `date`.

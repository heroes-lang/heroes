# Panel 147 — compiler-engineer

Measured 2026-09-14, this Mac (arm64), commit `7e6e71a8`, in a frozen copy at
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-lang/d2c6e340-fa30-4088-9630-1b1df8597855/scratchpad/panel-147/work/tree`.
The real tree was read and never written: `git status --porcelain` is empty.
Seed build `real 3.08`.

---

## Verdicts

| route | verdict | section |
|---|---|---|
| **A** — the handle names its releaser, the sweep calls it | **veto** (soundness) | design.md §1.12; CLAUDE.md § Precedence rank 3 |
| **B** — a scope-bound `cleanup` statement | **object** (core, not sugar) | design.md §1.7, Part 5 |
| **C** — refuse to Part 6 | **approve, conditional** | design.md Part 6 + §12 |

`needed_for_self_hosting`: **no**, for all three. `grep -rn "^ *record [A-Za-z_]* tag " selfhost/`
returns **0** — the compiler declares no handle at all. Its four `extern` groups are
`cli/process.hero:37,49`, `cli/io.hero:34`, `emit/literal.hero:41`. Principle 0 does
not carry any of these three.

---

## 1. The hinge: is the exit sweep reusable? Half of it

**Answer: the sweep's PLACEMENT is reusable. Its SEMANTICS is not. Route A needs a
second, parallel mechanism.** Measured from `--dump-ir`, not read.

### Placement is free (the one thing that argues FOR Route A)

`selfhost/ir/own.hero:216` gates the sweep list on `owed && layout.is_refcounted(c, slot.ty)`.
A `?` probe compiled in the copy shows both returning blocks carrying the identical
seven-line sweep:

```
bb1  ?: ok    preds bb0            bb2  ?: propagate    preds bb0
    ...                                ...
    decref_slot t                      decref_slot t
    decref_slot $f0                    decref_slot $f0
    ... (5 more) ...                   ... (5 more) ...
    return $t25                        return $t18
```

The slot `f: F` (a handle) is in the slot list and is skipped only because
`is_refcounted(F)` is false. One gate flip puts a release into **both** blocks. That
closes the census's 22 `?` leaks in `examples/ledger/main.hero` for free. This is the
strongest fact in Route A's favour and I want it on the record.

### Semantics is not reusable, and the reason is that every safety rule is an incref

Two functions, same shape, dumped side by side:

```
function opened() -> Holder          # a HANDLE escapes into a record
    slots  f: F
        $t5: F = call extern fopen($t2, $t4)
        store f <- $t5
        $t7: Holder = construct Holder($t6)
        return $t7                   # ZERO increfs, zero $own slots, no sweep

function boxed(a: str, b: str) -> Boxed    # a str escapes into a record
    slots  t: str · $own3: str · $own4: Boxed
        incref $t4                   # rule 6, at the constructor
        $t5: Boxed = construct Boxed($t4)
        incref $t5                   # rule 4, before the sweep
        decref_slot t · decref_slot $own3 · decref_slot $own4
        return $t5
```

Rules 3, 4 and 6 (`selfhost/ir/own.hero:16-39`) are **all increfs**, and all three are
what make the sweep safe when a value escapes. A handle has no count, so none of the
three has a counterpart. Worse, `selfhost/ir/owning.hero:40` answers
`allocates(.call) => true` **on the op kind alone** — it cannot tell an acquiring C
call from one that hands back a borrowed handle.

`selfhost/emit/inst.hero:165` turns `.decref_slot` into `reference_named(..., keep: false)`,
i.e. "consult the count". For a handle there is no count, so that arm needs a second
behaviour keyed on the slot type: a parallel mechanism, in a file at **350/350**.

---

## 2. The veto: Route A as spelled produces a silent wrong answer, measured

I reduced `examples/ledger/db/sqlite.hero:203-232` (the shipped escape shape), emitted
its C with the seed, inserted **the one line Route A's sweep adds** — `h0_db` is a
`local_slot`, so it joins the sweep list and is released at every returning block —
and ran both under `-fsanitize=address`:

```
--- today ---            create table rc = 0     exit 0
--- route A sweep ---    create table rc = 21    exit 0      (21 = SQLITE_MISUSE)
```

**Neither run printed anything from the sanitizer.** A control in the same session
(`malloc`, `free`, write) fired `heap-use-after-free`, exit 134 — so ASan works and
simply does not cover this class: the object is freed inside the system libsqlite3.
A raw-C check of the same shape answered `exec-after-close rc=21` with zero ASan output.

So Route A trades a **leak** the census could measure by asking `sqlite3_next_stmt`
for a **connection closed before its first use**, at exit 0, with no diagnostic and no
sanitizer report on the author's machine. Whether the Linux leg under `--sanitize`
(CL-055) sees it is **unrun** — I have no Linux here.

design.md §1.12 makes "must not corrupt memory" a goal of the language; CLAUDE.md
§ Precedence ranks it 3, above compiler size and Principle 0. Converting a silent leak
into a silent use-after-free is a soundness refusal, not a price. **Veto.**

This veto is on Route A **as spelled**. The sound version must refuse the escape:
`examples/ledger/db/sqlite.hero:204` (`handle: CDb`) and `:207` (`handle: CStmt`) —
the only two handle-typed record fields in the tree — plus `opened() -> Db?` (:212)
and `prepared(...) -> Statement?` (:271). That is escape analysis, and design.md
**:3519 and :3537 both call it the affine handle and both say "core by §1.7"**. A
different proposal, owed its own sitting.

---

## 3. Route B: core, and the mechanism it needs does not exist

**Core or sugar: core.** It desugars to no row of Part 5's table. The checker must
type the call at a point where no expression stands; the lowering must materialise it
at five edge kinds; the backend must emit it at each.

**`grep -rn "scope" selfhost/ir/` returns zero matches.** The IR has no lexical scope.
`containers.hero:54` `Block` is a basic block (`preds`, `insts`, `term`, `note`), and
the sweep fires **only** at `.return_term` (`selfhost/ir/own.hero:233-239`) — function
exit. `?`, `break` and `continue` reach it by jumping to a returning block. `own.hero:29-32`
records that block-scoped release was tried and refuted within the hour, because
`.must()`, `?`, `&&` and if-as-value all open a block in the middle of an expression.
So "when the enclosing block is left, however it is left" is a mechanism the compiler
does not have; its only precedent is `build.LoopEdges` (`flatten.hero:113,117,232,294`),
two jump targets and no ordered list.

**Count, the way panel 132 counted 68 for `ExprKind`: 24 exhaustive `match` sites over
`ast.StmtKind`**, across 12 files (lines carrying `.expr_stmt` and `=>`; the one non-arm
is `grammar_expr.hero:924`). Per file: `grammar_expr` 5, `print/fmt` 4, `mutate/edits` 4,
`ast` 3, and one each in `resolve/walk`, `print/bodies`, `mutate/typo`, `mutate/handles`,
`ir/questions`, `ir/flatten`, `check/walk`, `check/leasing`. Plus the CL-036 walk:
formatter round-trip, the dump printers, `heroes mutate`, the two highlighters
(`editors/vscode/syntaxes/heroes.tmLanguage.json`, `site/src/lib/highlight.ts`),
`heroes measure`.

Not a veto: Route B is sound if built. It is large, and it is core.

---

## 4. Where it lands, against ceilings that are live

`tests/harness/suite_layout.hero`'s `code_lines` (:484), `DECIDED` (:366-387),
`CEILING` 300 (:43). The table is a ratchet — ":46 *a stop on growth rather than a
licence*" — so a raise is an act, not a free edit.

| file | now | ceiling | room |
|---|---|---|---|
| `selfhost/ir.hero` | 310 | 310 | **0** |
| `selfhost/ir/verify.hero` | 300 | 300 | **0** |
| `selfhost/emit/inst.hero` | 350 | 350 | **0** |
| `selfhost/ast.hero` | 503 | 505 | **2** |
| `selfhost/print/fmt.hero` | 1153 | 1156 | **3** |
| `selfhost/ir/print.hero` | 466 | 470 | 4 |
| `selfhost/emit/gate.hero` | 360 | 365 | 5 |
| `selfhost/emit/ctype.hero` | 388 | 395 | 7 |
| `selfhost/check/walk.hero` | 1861 | 1870 | 9 |
| `selfhost/mutate/typo.hero` | 291 | 300 | 9 |
| `selfhost/ir/flatten.hero` | 1139 | 1150 | 11 |
| `selfhost/emit/structural.hero` | 323 | 335 | 12 |
| `selfhost/ir/own.hero` | 273 | 300 | 27 |
| `selfhost/mutate/edits.hero` | 271 | 300 | 29 |
| `selfhost/parse/members.hero` | 230 | 300 | 70 |
| `selfhost/check/freer.hero` | 221 | 300 | 79 |
| `selfhost/handles.hero` | 120 | 300 | 180 |

**Both routes must touch `ast.hero` (2 lines of room) and `fmt.hero` (3).** Those are
the same two files design.md:3518 already named "at ceiling" when `consumes` was priced.

**A new IR op costs 48 match-arm lines across 20 files** (measured: lines carrying
`.decref_slot` and `=>`), landing in three files with **zero** room.

Route A's own cost, if it were sound: `parse/members.hero` ~15 (by analogy with
`consumes`, "four lines and a two-line comment", suite_layout.hero:343),
`handles.hero` ~20, `ir/own.hero` ~10 for the gate, `check/freer.hero` ~40 for the
refusal of your own call, plus the `emit/inst.hero` arm. Those are **estimates**; I
measured the gate-flip's consequence, not a finished implementation.

---

## 5. §1.7's subtraction test

- **Route A**: removes nothing. Adds a mark, a sweep predicate, an emitter arm, and
  (to be sound) an escape refusal.
- **Route B**: removes nothing. Adds a construct to the core.
- **Route C**: removes nothing, adds nothing. The one route that passes §1.7's own
  working criterion ("does it move something from the core to the sugar, or remove a
  special case").

---

## 6. The fourth route nobody listed

The runtime already carries a **third leak counter whose panic accuses the program
rather than the compiler** — `runtime/parts/alloc.c:126-137`, `hero_live_held`,
`panic: %lld lease(s) never ended`, with the comment *"it accuses the program rather
than this compiler"*. That is the seam.

**Give a handle with a declared releaser a fourth counter of the same shape**:
increment at the marked acquire, decrement at the marked release, panic at exit naming
the type. It turns the census's **SILENT** row into its **LOUD** row, which is what the
census's own table says the difference is. It adds **no core construct, no IR op, no
ownership rule, no statement kind**, and it cannot corrupt memory because it never
calls anything — the veto above does not reach it. `consumes` already supplies the
release half (`examples/curl/main.hero:53`).

Cost, **estimate** (I did not build it): ~20 lines in `runtime/parts/alloc.c`, a mark
in `parse/members.hero` (230/300) and `handles.hero` (120/300), an emitter wrap in
`emit/ops.hero` (243/300). Nearest landed neighbour by shape is `lease`,
`selfhost/check/leasing.hero`, **195 code lines**.

What it does not do: release anything. It makes the miss fail the first run loudly,
which is exactly what `lease` chose. A **compile-time** answer needs flow analysis, and
`check/leasing.hero:29` and `check/consuming.hero:22` both state the checker has none.

---

## 7. Prediction (falsifiable, named milestone)

**At M-cleanup-verdict's close**: if Route A lands as spelled — a releaser on the
handle, called by the exit sweep, with no escape refusal — then
`./heroes run tests/harness/main.hero -- ./heroes corpus` goes **red on
`examples/ledger/`**, because `opened()`'s connection is closed before the caller's
first use and every statement answers `SQLITE_MISUSE`. My reduced reproduction of that
exact function moved from `rc = 0` to `rc = 21` under the one inserted line.
`examples/sqlite/main.hero` will **not** move, because its two handle slots never
escape their acquiring function.

Falsified if Route A lands as spelled and `corpus` stays green with
`examples/ledger/db/sqlite.hero:204` and `:207` unchanged.

Second, on size: **any sound Route A will exceed `consumes`'s 114 code lines by more
than 2x**, because the escape refusal has to reach record fields, array and map
elements, return types and Heroes call arguments, where `consumes` reaches one
argument position. Checkable by `code_lines` on whatever new `selfhost/check/` module
carries it.

---

## 8. Conditions

- **The veto on Route A lifts** if somebody shows a rule, costing under ~120 code
  lines in one new `selfhost/check/` module, that refuses every path by which a handle
  in a swept slot outlives its function — and shows `examples/ledger/` still compiling
  and `corpus` green with it. Then Route A is a cost argument and I withdraw to
  `object`.
- **It also lifts** if the sweep is applied only to a handle the compiler can prove
  never escapes — but "prove" there is the dataflow `check/leasing.hero:29` says does
  not exist, so that condition is a request for a measurement, not a suggestion.
- **My objection to Route B falls** if the cleanup list can be shown to lower without
  any new block-exit mechanism — e.g. compiled down to the existing function-exit sweep
  by hoisting every `cleanup` to the function's slot table. If that works, Route B
  becomes sugar and I approve it; I could not make it work on paper, because a
  `cleanup` inside a loop body must fire per iteration and the function-exit sweep
  fires once.
- **My approval of Route C is conditional** on §12: the Part 6 row must name what
  would make it wrong. Today's borrow-checker row names an annotation-based falsifier
  that **already fired** (`consumes`, 2 of 17). A refusal whose falsifier has fired is
  owed a new one. I would have it name the fourth route above: *a mark that makes a
  missed handle release loud at exit, under 250 code lines, with `corpus` green* — so
  the row can expire in its turn.
- **The spelling does not change any of these verdicts.** The veto is about when the
  release runs, not what the word is.

## What I did not run

- I did **not** build the compiler from `selfhost/` (~20 min, watchdog). Every Route A
  number above comes from the seed's own emitted C with one line inserted by hand, which
  is the line the sweep would insert — not from a modified compiler.
- The Linux leg under `--sanitize` is **unrun**.
- I prototyped **Route A**, the one whose cost I was least sure of. Routes B and C were
  counted, not prototyped.

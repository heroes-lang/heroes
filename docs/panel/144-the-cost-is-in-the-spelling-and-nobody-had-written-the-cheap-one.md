# Panel 144 — the cost is in the spelling, and nobody had written the cheap one

**Sat** 2026-09-13 · **milestone** M-deferral-ledger, step 9, its last · **status**
`provisional — author ratification pending`

**Lane: five seats plus a completeness critic.** The ledger's second item —
Part 8 warts **5**, **8** and **11**, plus **coverage**, the four entries with no
home at all. Warts 5 and 11 already carried rulings (panel 034, panel 137); this
sitting owed 8 and coverage, and found that the two it thought were settled are
not.

## The items, as design.md holds them

**Wart 8**: *"String concatenation is O(n²) under value semantics — and so is the
`[str]` you build to hand `join` (panel 037, measured: 1 000 000 pushes 806 s).
The remedy that works today is chunked accumulation, not a language change …
Needs `join`/`Builder` for compiler-scale output."*

**Coverage**: not in design.md at all. The word occurs **once** in the whole
document, at :132, where it means *spec* coverage, and **zero** times in
`spec/heroes-spec.md`. A production reader finds a silence.

## What was measured

### The array half is false, then true, then false again — and the variable is the spelling

Three shapes, one machine, `/usr/bin/time -p`, `user` read and `real` discarded
where the ratio said the run was waiting. Every figure below is the **median of
three runs**, a discipline the critic imposed after its own first run of one
program read `real 1.59` and its next three read `0.02`.

| how the push is written | n | user (median of 3) |
|---|---|---|
| `parts @ parts.push("x")`, a bare local | 1 000 000 | **0.02 s** |
| `w.parts @ w.parts.push("x")`, through a field | 50 000 | **11.51 s** |
| `put(@w.parts, "x")`, the field lent to an `@` parameter | 1 000 000 | **0.02 s** |
| `acc @ w.parts` hoisted, pushed, stored back once | 200 000 | **0.00 s** |

**Twenty times the work in five hundred times less time, between two spellings of
the same operation.** The field is not the cost. The spelling is.

**The mechanical witness.** `--emit-c`, then `grep -c hero_array_push_owned`:
**1** for the lent spelling, **1** for the hoisted spelling, **0** for the
in-field spelling. `selfhost/ir/place_store.hero:27-31` says why in its own
words — *"the path is empty — uniqueness comes from the place, and a path step
would put a container between the slot and the header, which is the exact
corruption panel 037 measured"* — and `@w.parts` resolves the path **at the call**,
so inside the callee the place is bare and the pass fires.

**And the repository had already found this route and forgotten it.** The place
store landed in `aacd2dbd`, 2026-08-24, *"the place store lands: p @ p.push(v)
grows in place — 944.76 s to 188.51 s (5.01x)"*. Two days later
`docs/records/log/2026-08-26-0001-…` reached the compiler's own arena by changing
the **call shape** to `push_expr(@exprs: [Expr], node)` — the lent-field spelling,
letter for letter — and `heroes check` went **191 s to 88 s**. The route nobody
in this sitting listed is the route this compiler already walks.

### Concatenation: unchanged and true

`s @ s + "x"`, user seconds: 100k 0.12 · 200k 0.40 · 400k 1.43 · 800k 5.39 —
ratios 3.3, 3.6, 3.8. Quadratic, confirmed on a four-point ladder rather than one
ratio. At the FFI boundary the same curve reads 0.18 s at 32 766 and 2.78 s at
131 064.

### The reachability count nobody had taken

`grep -rn quadratic selfhost/` returns **6 hits, all comments** —
`text_lines.hero:14`, `state.hero:84`, `resolve/state.hero:159`,
`measure/bpe.hero:53`, `emit/writer.hero:35` and `:62`. Six places where this
compiler's own authors hit the cliff and worked around it by hand. (The critic
reported five; measured again here it is six, and the correction is recorded
rather than smoothed.)

### Coverage, instrumented rather than argued

The FFI seat built a real SQLite binding, compiled the generated unit with
`clang -fprofile-instr-generate -fcoverage-mapping`, linked real `-lsqlite3`,
ran it, and read `llvm-cov report`:

- **`sqlite3` appears 0 times in the report.** Eight SQLite entry points called,
  a 65 KB statement prepared and stepped, 65 532 values bound — the instrument
  sees none of it.
- **All 8 `hero_ffi_probe_*` functions report 0.00%**, and they are 0% *by
  construction*: they exist so clang type-checks the argument list and are never
  called. Of 19 functions in the unit, 10 were missed; 8 of those 10 are the
  FFI's own verification machinery.

`heroes mutate` over the two pure bindings: 15 operators, **79 mutants, 66
killed**, and **not one survivor at a C call site** — all six `swap-args`
survivors are `print(…)` argument swaps. **44 of 44 one-character edits to
`extern` names die at compile time.** The two constructs mutation cannot reach,
the `cstr` lend and the `owned` freer, are compile errors by construction
(`error[type_mismatch]`, `error[unknown_freer]`).

### Two neighbours in the same ledger item, both stale

**Wart 5** says `typo-code` catches *"0 of 25"*. Run this sitting:
`./heroes mutate examples --operator typo-code` → corpus **120 programs**,
**59 mutants, 0 killed, 0%**. The rate survives exactly; the magnitude is
**2.4× stale**, and `docs/measurements/004` measured 17 programs.

**Warts 11 and 15 contradict each other**, both dated 2026-09-13, both written by
this milestone. design.md:3239 says *"all 22 files holding an `extern`"*;
design.md:3296 says *"the 24 files that declare an `extern`"*. Measured today in
`examples/`: **24** files mention `extern`, **21** open an `extern` group at line
start. **Neither method reproduces 22.** This is CL-017 in the milestone's own
work: a count written without the command that produced it.

## Verdicts

| seat | wart 8, array half | the `Builder` promise | coverage |
|---|---|---|---|
| `compiler-engineer` | **object** — stays, rewritten; the field path is the cost | **veto** on a language-level `Builder` | **object** — refuse as a Part 6 row; the item's falsifier does not fire |
| `spec-warden` | **object** — split, do not strike; and the **spec** owes a correction, +23 vendored measured | — | **approve** the row, **object** to the ledger's falsifier as drafted |
| `ffi-pragmatist` | **approve** striking the array half outright | strike it: `[str]`+`join` fixed the one real case at 3 lines and 14× | **object** — the row is right, its reason is not the strongest |
| `llm-ergonomist` | **object** — the cost is warned three sections from where the mistake is made | — | **refuse**; the silence reads as a refusal and is consistently applied |
| `historian` | **object** (advisory) — `Builder` is the answer *reference-counted* languages reached for | — | — |

## Where they disagree, unsmoothed

**Strike versus keep, and both sides were wrong in the same direction.** The FFI
seat: *"the array half is a dead sentence and must be struck, not softened."* The
engineer and the warden: it stays, because a field place is quadratic. Checked
rather than voted: **every** FFI bench pushes onto a bare local
(`parts @ parts.push(…)`) and the seat measured no field shape at all, so its
strike rests on one of the three spellings; and `put(@w.parts, …)` at 1 000 000
is 0.02 s, so *"a field place is quadratic"* is false as stated. **Neither side
ran the other's shape**, and the sitting's answer belongs to the critic, which
ran both.

**The engineer's generalisation, falsified.** Its report concludes *"The `@`
parameter is not the cost. The field path is."* It measured `put(@w, …)` — the
**whole record** lent, with the path step still inside the callee — and got
quadratic, correctly. `put(@w.parts, …)` lends the **field** and is linear. The
measurement stands; the sentence drawn from it does not.

**The warden's citation is a subtraction wearing quotation marks.** It cites
*"103 s of `heroes check`"* against `docs/records/log/2026-08-26-0001-…`;
`grep -c 103` on that file returns **0**. The file says 191 s to 88 s, whose
difference is 103. The substance is right and the citation is not.

**Two documents answered as one.** The ergonomist refuses coverage because the
spec *"names no command anywhere"* — true, and it names no tool at all — while
the warden and the FFI seat say a reader finds a silence that must be written.
Those are answers about **different texts**, and the resolution below says which
text takes which.

**Part 6 has never had a tool row.** The warden argued precedent exists because
the literate-source row *"points at `heroes doc`"*. Read the table: its column is
headed **`Feature`**, `heroes doc` appears only in the *reason* column, and all
**31** subjects are language or source forms. A line-coverage row would be the
first tool subject there. The critic is right, and the resolution pays for it
rather than stepping around it.

## The critic's answer to the asymmetry question

Beyond the route above, four more:

- **Nobody priced a diagnostic.** Four seats answered a 500× cliff with prose and
  none costed a checker rule, in a project whose thesis is that every plausible
  mistake is a compile error. What makes prose look inevitable is unstated in
  every report: design.md:3291 — *"a diagnostic is exit 1 or nothing"*. There is
  no warning level, so the option set is **exit 1 or silence**, and nobody said so.
- **The project already ships two things called coverage**: §132's spec coverage,
  and **M-corpus-coverage**, closed 2026-09-02, whose 20 programs were ordered by
  a measured coverage gap. A row refusing *"coverage"* unqualified refuses two
  practices this project performs.
- **`writer.hero`'s chunking is load-bearing for an unmeasured reason.** `w.chunk`
  is capped at `CHUNK_LINES = 128`, so its quadratic is bounded; the unbounded
  field accumulator is `w.chunks`, ~5 970 pushes for 764k lines, and nobody timed
  it. The unasked question: could `push_line` take `@chunk: [str]` the way
  `push_expr` was changed on 2026-08-26, making `CHUNK_LINES` unnecessary rather
  than load-bearing?
- **Every table in this sitting was a single run** until the medians above.

## Provisional resolution

**1. Wart 8 STAYS A WART, rewritten, and what it names changes.** Not *the array
you build is quadratic* — that is false for two of three spellings — but **a push
reached through a path copies the array, and the same push written through `@` on
the array itself grows in place**. The entry carries the three medians, the
`grep -c hero_array_push_owned` witness, commit `aacd2dbd` and the 2026-08-26
call-shape precedent, so the two halves of design.md cannot drift apart again.
**Return condition**: `ir_place_store` firing on `x.f @ x.f.push(v)` directly, or
100 000 in-field pushes under 0.1 s user.

**2. The `Builder` promise is STRUCK**, on the engineer's veto. It has no subject:
`join` shipped and is Tier 1; the largest Heroes program accumulates through a
267-line `Writer` record of ordinary Heroes; a builtin type is core by §1.7's own
test and cannot be erased in the frontend, at 500-800 lines across six modules to
replace a record a user can write. Principle 0: the compiler does not need it.

**3. The specification owes a correction, and "nothing" was the wrong answer —
but it CANNOT BE APPLIED IN THIS SESSION, and the reason is a hard stop.**
§10's *"`xs @ xs.push(4)` grows in place while nothing else holds `xs`"* is **true
and insufficient**, which is worse than silence: nothing else holds `w.parts`, and
it is 500× slower. Two seats reached that sentence from disjoint inputs — the
warden from the measurements, the ergonomist from the prose alone, which named it
as the one hesitation whose wrong guess produces *a silently different program*.

The drafted replacement, written and **measured** rather than estimated:

> `xs @ xs.push(4)` grows in place when `xs` is a plain name; reached through a
> field or an index it copies the whole array, so lend the array itself to an `@`
> parameter or hoist it into a name.

**+35 vendored, measured** on the draft: cl100k 5655 → 5690, legacy 5532 → 5567.
The `real` row cannot be measured for an off-path file and is **estimated** at
~+47 by the 1.332 ratio, landing near 7578 against an effective 7591 — which is
inside the ceiling and is an **estimate, not a measurement**, and the difference
matters here.

**Why it does not land today.** `tests/harness/suite_spec.hero:158` pins
`SPEC_DIGEST`, content-addressed, and `measure` **refuses to give a verdict when
it disagrees** — exit 1, `cannot_measure`, the verdict WITHHELD. The only command
that clears it is `heroes measure spec/heroes-spec.md --refresh`, which reaches
the network, and a network call is asked for every time. So editing the spec in
this session would leave the `spec` suite red with no legal way to make it green.
**The correction is adopted and queued, not applied.**

**And it shares its key with the open defects.** Defects 030 and 031 both name
`spec/heroes-spec.md` as the document whose sentence is false — § 3's *"No
aliasing exists anywhere"* and § 13's *"Unmarked pointers are never freed"*. All
three corrections are blocked behind the same single authorised command. **One
yes clears the spec half of all three**; what it does not clear is 029 and 031's
compiler work, which is `M-handle-verdict` and a milestone of its own.

**4. Line coverage is REFUSED, as a Part 6 row**, and the column head is widened
to `Feature or tool` in the same edit, so the first tool subject does not silently
contradict the table it sits in. The reason takes the FFI seat's argument, which
is the strongest and is one no other language can make: in a language whose
founding constraint is that everything real comes from C, a coverage percentage
**measures the glue and reports it as the program**. Both falsifiers are carried,
the FFI seat's first.

**5. Wart 5 is refreshed and closed**, keeping panel 034's ruling and adding its
date: 59 mutants over 120 programs, 0 killed, 0%.

**6. Warts 11 and 15's disagreeing counts are corrected underneath**, with both
methods and today's numbers, because a milestone that finds stale counts in other
people's entries owes the same instrument to its own.

**What conservative would have been**, recorded so it can be chosen: strike the
array half outright, as the FFI seat asked. It is refused because
`w.parts @ w.parts.push("x")` at 50 000 is 11.51 s and a struck sentence would be
a false one.

## Author's verdict

**Pending** — queued in `docs/work/DECIDE.md` as `panel 144`.

## Predictions to score

1. **Engineer, at the next milestone touching `selfhost/ir/place_store.hero`**:
   `w.chunk @ w.chunk.push(x)` at 100 000 still exceeds 20 s user, and
   `selfhost/emit/writer.hero` still chunks with `CHUNK_LINES`. Either false and
   the wart closes.
2. **Engineer, at M+1**: no `selfhost/` module declares a `Builder`,
   `writer.hero` stays under 300 lines, self-build wall time unchanged within 5%.
3. **Engineer, at the milestone adding a relational-substitution operator**:
   `ops.hero` asserts `operators().len() == 16` and the gallery kill rate under
   `check` is **below 50%**. Above 90% falsifies its reading of `twin_of` and
   reopens coverage.
4. **Warden, next spec-only metric-2 run**: zero generated programs accumulate
   through `x.f @ x.f.push(v)`; the amended spec measures ≤ 7570 real.
5. **FFI, at this milestone's close**: step 3 of §4.19's ladder needs no shim and
   no `Builder` — the binding compiles and runs today with a 65 KB generated
   statement at 0.06 s user.
6. **FFI, at this milestone's close**: any line-coverage instrument pointed at
   `examples/ledger/` reports ≥ 85% while attributing **zero** regions to
   `sqlite3.h`, and marks its 17 `hero_ffi_probe_*` functions uncovered.
7. **Ergonomist**: on ten string-building tasks the current spec produces a
   `+`-on-`str` inside a loop body in ≥ 30% of first-try programs; with a §7
   pointer it drops to ≤ 10%. **Unrunnable today** — metric 2 is API-only over 20
   frozen tasks with no `heroes` subcommand behind it, and the seat's own
   conditions say to measure the baseline before writing the line. Recorded as
   *unscorable with the instruments that exist*, which is a finding about the
   harness.

## What seats could not run

- **The historian has no file write**, so its report reached the coordinator
  through the run's journal and never the sitting's directory — and **the critic
  could not audit it**, exactly as panel 143 found one sitting earlier. The same
  door, still open. It also read only line 1 of design.md and worked from the
  brief's quotation of the wart rather than a fresh read, and said so.
- **The ergonomist's isolation is not enforced by the harness.** It reported that
  `.claude/rules/spec-shape.md` and `.claude/rules/verification.md` were loaded
  into its context without its asking, refused them as evidence, and rested no
  sentence on either. The seat whose entire value is reading **only** the
  specification cannot be guaranteed that by the machinery that convenes it.
- **The warden's `real` token delta is estimated**, not measured: `--refresh`
  reaches the network and is a hard stop without the author's word. The vendored
  delta is measured.
- **The FFI seat's `llvm-cov` result is true by construction** and the critic said
  so: instrumentation was applied to the generated unit only, with libsqlite3
  linked prebuilt. Rebuilding the amalgamation instrumented would settle whether
  the honesty argument is instead a packaging argument. **Unrun.**
- **The engineer did not rebuild from `selfhost/`**, forbidden for that seat, so
  its timings are the seed binary's and the 796,427-line figure is inherited.
- **`heroes mutate` takes a corpus directory** while `help` says `mutate [file]`.
  Whether that wording is a defect is a **question**, not a finding: nobody
  compared the seed's CLI text to the live compiler's.

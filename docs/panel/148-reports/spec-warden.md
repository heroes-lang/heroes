# Panel 148 — spec-warden

- **verdict**: **object** on A as drafted · **veto** on C · `provisional` on the
  real instrument (the after-count for a draft is **unrun**: `--refresh` is
  refused for a file no ceiling judges, panel 123 R5, and no conversion is
  admissible — `docs/measurements/010`)
- **section**: design.md §1.6 (payment, unconditional at every level), §1.0
  (burden of proof), **Part 6** row *"A releaser named on a HANDLE TYPE"*
  (design.md:2623) via CLAUDE.md §13, with §1.1, §1.3, §1.5 and §1.7 below
- **spec_token_delta**: measured on the vendored instrument, by me, this
  sitting, with `./heroes measure`:

  | draft | cl100k | delta | grammar derives its own example? |
  |---|---|---|---|
  | live `spec/heroes-spec.md` | **5863** | — | — |
  | C, as briefed | **5919** | **+56** | n/a (adds no surface) |
  | A, as briefed | **5927** | +64 | **no** |
  | **A, grammar repaired** | **5933** | **+70** | yes |
  | **A, merged + repaired** (new) | **5921** | **+58** | yes |

  Real: **7806** (claude-opus-5, 2026-09-13), and it is of *today's* bytes —
  the digest gate did not fire, so the pin matches. 7806 + 60 (FFI floor) =
  7866 against 8192: **326 free**. After-real for every draft: **unrun**.
- **removal**: **nothing — and that is a problem.** No removal is named and no
  prediction is registered in either brief. The merge below saves 12 vendored
  tokens against the repaired A, but a saving against a draft is not a removal
  from the document: the spec still grows. Payment is owed before this lands.
- **needed_for_self_hosting**: **no**, for both, and measured: `selfhost/` has
  four `extern` groups (`cli/io.hero`, `cli/process.hero` x2,
  `emit/literal.hero`), **zero handle records**, and its one acquire/release
  pair is `hero_dir_scan`/`hero_dir_release` — a global scan buffer, not a
  value, so it can carry neither mark. Not on §1.0's closure list. Both options
  therefore ride the second limb alone.
- **argument**: A is not on §1.0's closure list, so both options ride the
  thesis limb and neither is paid for. C fails that limb outright: it keys the
  obligation on the result TYPE plus a sibling declaration in the group, which
  is the axis design.md Part 6 refused yesterday — *ownership is a property of
  the CALL and not of the type* — and the falsifier that row demands, a
  header-derivable give/lend signal, does not exist (`sqlite3_db_handle`,
  SDK `sqlite3.h:7052`, verified on this box). The eight-token gap is not a
  reason to prefer it: I measured it away. A merged into the sentence that
  already carries the abort, with its grammar completed, is **+58** against
  C's +56.
- **prediction**: (i) the merged draft at
  `…/scratchpad/warden/spec-a-merged.md` reads **5921 vendored**, and its
  **real** count on `--refresh` at the landing commit lands in **7864..7896**
  (delta +58..+90), leaving **>= 236 free** after the FFI floor. Falsified by
  any refreshed reading above 7896 or below 7846. Instrument:
  `heroes measure spec/heroes-spec.md --refresh`, which exists. Scored at the
  commit that lands the text, in M-marked-acquisition. (ii) the shipped tree
  needs **exactly three** `acquires` marks and no fourth — the three producers
  of `docs/measurements/031`. Instrument: `grep -c` plus a compile of
  `examples/`. Falsified by a fourth site or by any producer the compiler
  cannot mark.
- **condition**: I lift the object on A when (a) the landing draft is the
  merged one or one measured cheaper, (b) its grammar amends **both**
  positions, and (c) the commit body carries a named removal or prediction (i)
  above. I lift the veto on C only if somebody produces the signal Part 6's
  falsifier names — a header-derivable separation of *gives* from *lends* — or
  a marked escape inside C's inference, **and** prices C's checker, which is
  unpriced today.

---

## 1. The eight-token gap: the budget cannot distinguish them, and the gap is not eight

**It cannot.** Eight vendored tokens is 0.14% of the document, taken on an
instrument design.md §1.6 says is *not the reader's*: the spread between the two
offline tables on this same file is **126-127**, and the gap between two real
Claude generations was **967**. Nothing licenses carrying the sign of an
8-token vendored difference across to `claude-opus-5`, and
`docs/measurements/010` forbids the conversion that would try. Against **326**
free real tokens, both options are noise. **A seat that ranks A against C on
the budget is manufacturing a distinction, and this seat declines to.**

**And the eight was wrong anyway.** As drafted the gap is **14** (A repaired,
+70, vs C, +56), and with the merge it is **2** (+58 vs +56). The one honest
reading is that the budget is indifferent, so §1.1 applies as written:
*tokens break ties only when comprehension is indifferent.* It is not.

## 2. Item 5, the premise audit: six findings, one of them a defect in A itself

Reproduced and confirmed: the three vendored counts to the token
(5863/5919/5927); `variant-3.md` byte-identical to the live spec; `spec-a.md`
== `variant-2.md`, `spec-c.md` == `variant-1.md`; `FFI_FLOOR` 60, `CEILING`
8192, headroom arithmetic; `REGISTRY_TOKENS` 916 current, and contextual marks
do not appear in `spec/reserved-words.md`, so A adds **no** registry cost.

**Finding 1 — A's grammar does not derive A's own example.** The draft amends
`CParam` only. The live `Member` production is

    Member = "function" ident "(" [ CParam { "," CParam } ] ")"
               [ "->" Type [ "owned" ident ] ] NEWLINE

so `function curl_easy_init() -> Curl acquires` — A's second worked example in
the shared brief, and **one of the three producers in the tree** — is
underivable. The prose says *"after a handle result or `@` out-parameter"* and
the grammar admits only the parameter. This is panel 133's
`return match s` defect, in the same document, three days later. The fix is
`[ "->" Type [ "owned" ident ] [ "acquires" ] ]`, and the document's own
precedent is on that very line: `owned` is already in both positions.
**Measured cost of the repair: +6 vendored (5927 -> 5933).** The briefed +64
underprices A.

**Finding 2 — the drafted A writes the same clause twice.** *"aborts when
`main` returns, saying how many"* appears exactly once in the spec today
(line 368, the lease). A's sentence adds a second copy six lines away.
`.claude/rules/spec-shape.md` (panel 122) says merging beats appending, and the
Report rule is one rule, one home. Merging recovers **12 vendored tokens**
against the repaired A and keeps every word of the meaning:

    cell, and a lease nobody ends, like a handle nobody consumes, aborts when
    `main` returns, saying how many.
    ...
    and the value does not survive the call. `acquires` after a handle result or
    `@` out-parameter says the call begins that handle's life, and the program
    owes it one `consumes` call.

Full draft: `…/scratchpad/warden/spec-a-merged.md`. **5921 vendored, +58**, both
productions amended, no heading moved, ASCII only, max line width unchanged at
134. **Unrun**: `suite_spec`'s `shape`/`named`/`rejected`/`inventory` checks on
this draft, and the ledger row. That is the sitting's third option.

**Finding 3 — C is the axis Part 6 refused yesterday, running backwards.**
design.md:2623 refuses a releaser keyed on the type *"because ownership is a
property of the CALL and not of the type, so the mark cannot know whether this
handle was given or lent"*, and says in terms that it refuses **the AXIS and
not the spelling**. C reads the obligation off *the result type plus a sibling
declaration in the group*. It therefore cannot know either: `sqlite3_open`
gives and `sqlite3_db_handle` lends, and both hand back a `CDb`. Verified on
this box, not recalled — `/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/sqlite3.h:7052`
`sqlite3 *sqlite3_db_handle(sqlite3_stmt*)` and `:7187` `sqlite3_next_stmt`.
`docs/measurements/031` records the same premise honestly: those two are bound
by no binding **in this tree, on this date**, and *"the first binding that
declares one brings the ambiguity back"*. When it does, C has no way to say
*this one lends*: the program either closes a borrowed handle — the compiler
seat measured that exact failure at panel 147, `rc = 21`, `SQLITE_MISUSE`, exit
0, sanitizer silent — or aborts at `main` for a leak that never happened.
§1.12 and CLAUDE.md § Precedence rank 3 make that a refusal rather than a
price, and CLAUDE.md §13 says Part 6 is where not to go. **That is the veto.**
A has no such hole: an unmarked producer is a borrow, stated where it is
created, which is the one arrangement this document has used four times
(`owned` on a result, `lease` on an initialiser, `consumes` on a parameter
position, the `cstr` lend on an expression's position).

**Finding 4 — C's compiler cost is unpriced, and the brief compares priced to
unpriced.** `docs/measurements/032` § 5 prices the built half of **A** (+25
code lines, 7 files) and grounds *"the checker needs no flow analysis"* on the
fact that **both ends are written by the binding author**. C removes one of
those ends. Nothing in the tree prices the group-wide inference C needs — a
type-to-consumer map per group, then a re-scan of every member's result — and
no measurement names it. CL-057: the list of costs is a measurement too, and
C's list has a hole where its largest item would be. Ranking C above A on +2
to +14 vendored tokens while its compiler side is unrun is the shape §1.1
forbids.

**Finding 5 — both options exceed `DELTA_GATE` in one commit.** `DELTA_GATE`
is **50** vendored (`tests/harness/suite_spec.hero:169`,
`selfhost/measure/judged.hero:148`). C is +56, A is +58 merged and +70
repaired. CLAUDE.md says *"A commit moving either judged document past
`DELTA_GATE` says in its body what paid for it"*. Neither brief mentions it.
Whichever lands, the body owes the payment sentence, and splitting the change
across two commits to slip under the width would be gaming an attribution
gate.

**Finding 6 — and that gate has an instrument for one of its two documents.**
Searched `DELTA_GATE` across `selfhost/` and `tests/harness/`: every live use
is inside `if path == budgets.CONTRACT` (`selfhost/cli/measure.hero:136`). The
spec path gets the digest staleness gate and `SPEC_TOKENS` equality, neither of
which is a width. So on `spec/heroes-spec.md` the delta gate is prose with no
enforcer, and this sitting is the first change large enough to notice. Filed as
an observation, not as a blocker for this sitting.

## 3. §1.7's subtraction, per option

Neither subtracts. §1.7's working criterion is *does it move something from the
core to the sugar, or remove a special case from the compiler* — and both add
one. **A adds the cheaper one**: one more contextual marker beside `consumes`,
reusing `parse/members.hero:93`'s `consumes_marker` shape, one arm wide.
**C adds a new kind of rule**: the first FFI fact in this language whose
meaning is not on the line that carries it, which is §1.3's named failure
(*constructs whose meaning lives elsewhere*) and gives up §1.5's asymmetry for
free — the mark is paid at **three declaration sites in the whole tree**
(`docs/measurements/031`), which is exactly where §1.5 says verbosity is cheap.
So the spec-token ranking and the compiler-complexity ranking point in opposite
directions, and §1.1 says the tokens lose.

## 4. What I could not run

- The **real** count of any draft. `--refresh` needs the network and a judged
  path; no reading was taken and none is inferred. Every after-figure above is
  vendored, and the verdict is `provisional` on that axis. It is not
  provisional on breach: a breach needs the real delta to exceed **326**, i.e.
  more than five times the vendored delta on three lines of prose, against a
  document-level relation near 1.33 that §1.6 forbids using as a factor anyway.
- `suite_spec` and `suite_special` on the merged draft (the tree is frozen and
  the suites would go red on `SPEC_TOKENS` by design).
- The runtime counter half of either instrument, which
  `docs/measurements/032` § 5 already marks unbuilt and unrun.

## 5. One line on what is not mine

Panel 147's ruling is right and I do not re-open it. I note only that it makes
C harder to defend, not easier: *the obligation is marked where it is created,
on the acquiring call, never on the type* is A's sentence, and C does not mark
it anywhere.

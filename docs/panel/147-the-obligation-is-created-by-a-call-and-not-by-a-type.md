# Panel 147 — the obligation is created by a call, and not by a type

**Sat 2026-09-14**, M-cleanup-verdict, full five seats plus the completeness
critic. Convened on the author's authorisation, who also chose the **narrowed**
question over the milestone file's original.

**Status: `provisional — author ratification pending`.**

---

## The proposal, verbatim as the seats received it

> Can a C handle be given a release function so it joins the exit sweep the
> language ALREADY walks on every path — or does a scope-bound release form
> enter — or is this refused to design.md Part 6 with a named falsifier?

Three routes were drafted so their spec cost could be measured. The seats were
told in writing to treat the spellings as placeholders.

- **Route A** — the handle names its releaser:
  `record Stmt tag sqlite3_stmt released sqlite3_finalize`. The compiler calls
  it once on every path out of the acquiring scope, `?` included, and refuses
  your own call. **+80 vendored tokens**, measured.
- **Route B** — a scope-bound statement: `cleanup sqlite3_finalize(statement)`.
  **+108 vendored**, measured.
- **Route C** — refuse to design.md Part 6 with a falsifier. **+0**.

---

## The verdict table

| seat | Route A | Route B | Route C | its veto scope |
|---|---|---|---|---|
| **compiler-engineer** | **VETO**, soundness | object — core, not sugar | approve, conditional | unqualified; **this is the sitting's one formal veto** |
| **ffi-pragmatist** | refuse, as specified | **approve** | its falsifier has already fired | veto is on **ABI breakage**, and it reports ABI untouched — so this refusal is an objection at full force, not a veto |
| **spec-warden** | refuse, Principle 0 | object — draft unsound for `@` | approve **with** a falsifier | veto is on **budget breach**, and it reports no route breaches — so this refusal is also not a formal veto |
| **llm-ergonomist** | approve, conditional | object | object | veto is on non-local constructs; **not cast** |
| **historian** *(advisory)* | approve — precedent is Vala | ancestry broad, originator shipped the bug | precedent expiring | none |

**Three seats refuse Route A on independently measured grounds, and exactly one
of the three is a formal veto within its seat's stated scope.** That distinction
is recorded rather than smoothed, because counting "three vetoes" would be a
number this sitting did not earn.

---

## What each seat actually found

**compiler-engineer — the veto, and it is a measurement.** It reduced
`examples/ledger/db/sqlite.hero`'s `opened()`, emitted its C with the seed, and
inserted **the one line Route A's sweep adds**. The next statement answered
**`rc = 21`** — `SQLITE_MISUSE` — where it answers 0 today: the connection is
closed before its first use. **Exit 0, and ASan silent**, because the object is
freed inside the system libsqlite3; a control in the same session fired
`heap-use-after-free` at exit 134, so the sanitizer works and simply does not
cover this class. Route A trades a leak the census could measure for a silent
wrong answer. design.md §1.12 and CLAUDE.md § Precedence rank 3 make that a
refusal rather than a price. It also reports the half of the hinge that favours
Route A and wanted it on the record: **placement is reusable** — `ir/own.hero:216`
gates the sweep on `is_refcounted`, the handle slot is already in the slot list,
and one gate flip would put a release into both the `ok` and the `propagate`
block. What is not reusable is the semantics: rules 3, 4 and 6 are all increfs,
and a handle has no count.

**ffi-pragmatist — the sharpest sentence in the sitting, compiled.**
**Ownership is a property of the call, not of the type.** `sqlite3_db_handle`,
`sqlite3_next_stmt`, `cairo_get_target` and `hb_font_get_face` all hand back the
very type Route A would have marked, *borrowed*. It compiled that binding:
`borrowed == owned: true`, and with the sweep in place the program printed **0
rows where the table holds 3**, exit 0, ASan silent. It also surveyed **21
libraries, 13 596 declared functions**: of **262** opaque types carrying a
release obligation, **212 (80.9%)** have exactly one single-argument releaser —
so Route A's *shape* fits four types in five — while **raylib is 0 of 21** and
libpng 0, both on design.md §1.11's own table. **11 of 11** releasers it tested
survived a NULL argument. ABI is untouched by either route, and it named the
thing that would break it so nobody proposes it later: a released bit inside the
value, which it compiled to `error: passing 'HandleWithFlag' to parameter of
incompatible type`.

**spec-warden — Principle 0, and a measurement nobody else took.** **22 of the
23 escaping paths acquire through `sqlite.prepared(...)`**, a *Heroes* function
in another module that returns the handle wrapped in a record. The scope that
acquires is not the scope that leaks, and Route A's own *may not be returned or
stored* would refuse the reference binding at `:232` and `:277`. Write the mark
and the corpus stops compiling; omit it and nothing is checked. **Measured yield
on the corpus: zero paths closed, at +80 tokens.** The closure-list limb is
structurally empty: `selfhost/` is 60,360 lines with four `extern` groups,
**zero handle records**, and one acquire-and-release pair that is nullary and
cannot carry the mark at all. §1.7's subtraction is **zero** for both forms, and
no named removal exists.

**llm-ergonomist — the only approval, and it was not wrong about what it
measured.** Reading three label-stripped specs and nothing else, it wrote the
same two-resource program three times and counted the sites where a writer must
remember to release: **6 with the language as it stands, 3 under Route B, 0
under Route A**. It found Route B's trap unaided — every habit says register on
the declaration line, but the handle is filled by the call *after*, so the
natural program registers the release of a null pointer, **and compiles**. It
guessed which variant was current and the tell is itself a finding: `cleanup`
appears in **no code block** anywhere, while every other construct in the
document is demonstrated at least once.

**historian — the precedent, and one prediction collected inside the sitting.**
Route A is not exotic: **12 systems** name a foreign function as a releaser, but
only **two** fire at scope exit and only **one** keys it on the type — **Vala**,
which compiles to C and whose shipped GNOME binding reads
`free_function = "sqlite3_finalize"` on `sqlite3_stmt`, the literal proposal,
alive twenty years. Route B's ancestry is broader but its originator shipped the
loop leak and **five** successors corrected it to block scope; Zig then needed
`errdefer` as well. And **Route C's long holder abandoned it this year**: C's
`defer` is TS 25755, in Clang 22 since 2026-02-15. **Its prediction — that a
per-type mark would prove insufficient because some producing function returns a
borrowed instance, naming `sqlite3_next_stmt`, which this milestone's own census
had used — was collected by the ffi seat an hour later. It held.**

---

## The completeness critic, which changed the resolution

**1. Route A cannot be compiled, and no brief said so.** `released` does not
parse: `error[expected_extern_signature]`. So every Route A finding here is
either hand-inserted C or a reading of four draft lines. *Veto, measured* and
*approve, measured* were **not two measurements of one object**, and a reader
of this file must not take them as such.

**2. The three "fourth routes" are one route with three instruments.** The
historian's linear types, the engineer's runtime counter and the ffi seat's mark
on the acquiring call differ on instrument and agree on the axis: **in none of
them does the compiler pick the release call.** No dominance — a cost ladder.
**And the engineer's counter carries its own veto's defect**: keyed on the type,
`sqlite3_db_handle`'s borrow increments and never decrements.

**3. A fifth route nobody listed.** Escape refusal already ships in this tree —
`cstr_escapes` (`check/lending.hero:242`) and `lease_escapes`
(`check/leasing.hero:109`). **Panel 122 refused an inferred release, not escape
refusal**, and its three clauses are term-for-term Route A's missing half.
Nearest shipped rules by code lines: `ffi_sweep` 93, `consuming` 114, `leasing`
195, `freer` 221, `lending` 241. **Blast radius on this tree: five sites.** A
sixth route — Route C plus a 93-line whole-file check — was never on the table.

**4. Two "cannot" claims are false**: the engineer's *the dataflow does not
exist* and the ffi seat's *escape analysis, refused by name*.

**5. The ergonomist's strongest claim is false.** *Double release is impossible*
under Route A — `e7_double_close` prints `sweep closes borrowed -> 0` and
`sweep closes db -> 21`. It could not have seen it: its binding contained no
borrowing function. **And the ergonomist and the warden found the same Route B
`@`-cell bug from opposite ends without citing each other** — that is the
strongest Route B finding in the sitting, and a seat-by-seat reading loses it.

**6. Route A's draft never defines *acquire*, and the three refusing seats each
supplied a different definition.** Each verdict follows from its own reading.

---

## The census that convened this sitting is corrected, and it was the
## coordinator's

`docs/measurements/030` was written this morning and handed to five seats as
fact. The critic audited it, which nobody had. **Four corrections**, carried
into that file underneath its original text as a record requires:

- **Live exposure is 22 paths in ONE file, not 23 in two.** The 23rd was
  repaired earlier the same session; the headline should have said so.
- **The pair list is short by at least seven** — `main`'s own db pair and six
  test blocks — so it is **≥19 pairs, not 12**. CL-057: the list is a
  measurement too, and this one was taken by reading rather than by enumerating.
- **"10 of 21 pairless" is 8.** The partition 10 + 10 leaves one slot for three
  programs; it is an arithmetic error in the coordinator's own summary.
- **And the one that matters: all 22 paths end in `exit(1)` or `abort`.** `main`
  closes and exits on `is_err()`; the six tests use `.must()`. **No shipped
  program leaks a handle and then goes on running.** The census's headline
  *SILENT, exit 0* was measured on a synthetic reduction, which is true of the
  reduction and overstates the corpus.

The briefs carried one further error of the coordinator's: they converted
vendored token counts to real ones at a ratio of 1.331, and
`docs/measurements/010` says in terms that **no row is convertible**. The warden
caught it and gave a range instead.

---

## The question the sitting should have asked, and did not

**Does the obligation belong to the type, the call, or the value?**

The compiler has already answered it four times and **never once on a type**:
`owned` rides a *result*, `lease` a *declaration's initialiser*, `consumes` a
*parameter position*, and the `cstr` lend an *expression's position*. Route A is
the first proposal in this project to put an FFI lifetime fact on a type — and
CLAUDE.md §11's **a narrowing asks the value, never the world** settles it in one
line, before any C is compiled. Three seats reached that refusal the expensive
way, each from its own mandate. The rule was already written.

---

## Resolution — `provisional — author ratification pending`

**R1. Route A is REFUSED**, on the axis rather than on the spelling. A releaser
keyed on the TYPE cannot be right, because the obligation is created by a
**call**: the same handle type is handed back owned by one C function and
borrowed by another, compiled at this sitting. Refusing it is not a cost
judgement and no spelling repairs it.

**R2. Route B does not enter AS DRAFTED.** Two seats found the same `@`-cell
defect from opposite ends: the deferred call's arguments are read where the
statement is written, so the natural program registers the release of a value
the acquiring call has not yet filled, and it compiles. If a scope-bound
statement ever enters it is **block-scoped, not function-scoped** — five
languages corrected Go's one choice and the record is unanimous — and it owes
the `errdefer` question up front.

**R3. Route C is NOT taken.** Its ground has moved twice under it: C itself
adopted `defer` this year (TS 25755, Clang 22), and the Part 6 row that would
host the refusal names a falsifier that **already fired** when `consumes`
landed at 2 of 17 on 2026-09-13. A refusal whose falsifier has fired is owed a
new one, and writing a fresh refusal on this evidence would be writing a promise
the next milestone breaks.

**R4. A form enters, and the sitting names its axis rather than its spelling:
the obligation is marked where it is CREATED — on the call — and never on the
type.** It takes **its own milestone**, whose first act is to price the two
instruments against each other rather than pick one here:

- a `consumes`-shaped mark on the **acquiring** call, whose nearest shipped
  neighbour is `check/consuming.hero` at **114 code lines**;
- **escape refusal**, whose machinery ships twice already — `cstr_escapes` and
  `lease_escapes` — measured at 93 to 241 code lines in its five shipped
  relatives, with a **blast radius of five sites** on this tree.

Whichever is priced cheaper, **the compiler never picks the release call**. That
is the one clause every refusing seat and all three proposed fourth routes
agree on.

**R5. What conservative would have been, recorded so the author can choose it**
(CLAUDE.md § 4, CL-040). Conservative is **Route C with a redrafted falsifier**:
refuse a scope-bound release, and write the Part 6 row on the census as
corrected — *no shipped program leaks a handle and goes on running; all 22 paths
end in `exit(1)` or `abort`* — with the falsifier *a corpus or closure-list
program that leaks a handle and continues, or acquires and escapes in the same
scope*. The sitting did not take it, because § 4 requires the most robust
resolution where robust and conservative disagree, and because the corrected
census weakens the urgency without touching the class: a program that handles an
error instead of exiting leaks, and this corpus simply has not written one yet.

**R6. `docs/measurements/030` is corrected in place**, underneath its original
text, with the four findings above and today's date.

---

## Predictions to score

| # | whose | prediction | checkable at |
|---|---|---|---|
| 1 | historian | a per-type mark is insufficient; some producing function returns a borrowed instance of the marked type | **ALREADY SCORED — HELD.** Collected by the ffi seat inside this sitting: `sqlite3_db_handle`, `borrowed == owned: true` |
| 2 | compiler-engineer | if Route A lands as spelled, `corpus` goes red on `examples/ledger/`; `examples/sqlite/` does not move | M-cleanup-verdict close, if ever built |
| 3 | compiler-engineer | any sound Route A exceeds `consumes`'s 114 code lines by more than 2× | the successor milestone |
| 4 | ffi-pragmatist | under Route A, `examples/ledger/db/sqlite.hero` cannot compile without deleting `opened()`'s close at `:230` | the successor milestone |
| 5 | llm-ergonomist | release-call sites 6 / 3 / 0 for status quo / B / A; and Route B's leak rate is **not** under half the status quo's | a Part 11 run |
| 6 | historian | if Route B enters, `errdefer`'s split is demanded within two milestones | two milestones after B, if ever |
| 7 | spec-warden | a re-count reads 22 in one file, not 23 in two | **ALREADY SCORED — HELD.** The critic's audit found exactly that |
| 8 | critic | escape refusal prices under the engineer's 120-line bar, on the evidence of `ffi_sweep` 93 and `consuming` 114 | the successor milestone |

---

## The briefs and the reports are in the record, and that is new

`docs/panel/147-briefs/` holds the six briefs, written to disk before any seat
started; `docs/panel/147-reports/` holds all six reports, the critic's included.
The historian's is there because the coordinator wrote it out: that seat has no
write tool by its own definition, which is why panels 143 and 144 both recorded
that it could not be audited.

**No sitting before this one kept either**, and the gap is worth stating because
it is the one panel 140's critic was convened over. That sitting's finding —
*"the briefs are prompts; they vanish when the workflow ends"* — was ratified on
2026-09-13, and its recommended home was `docs/panel/NNN-briefs/<seat>.md`.
What `/panel` step 2 implements instead is the session scratchpad, which is
**also** session-specific and vanishes with it, so panels 145 and 146 wrote
briefs that no later reader can open. `find docs/panel -maxdepth 1 -type d`
returns one directory, and it is from 2026-08. The question of which home the
rule meant is filed in `docs/work/DECIDE.md`; this sitting used both.

## Author's verdict

**Pending.** The item is open in `docs/work/DECIDE.md` as `panel 147`.

**What a yes settles**: R1, that a releaser keyed on the TYPE is refused on its
axis and no spelling repairs it; R2, that a scope-bound statement does not enter
as drafted and would be block-scoped if it ever did; R3, that Route C is not
taken, because C adopted `defer` this year and the Part 6 row's falsifier has
already fired; and **R4**, that a form enters marked on the acquiring CALL, in
its own milestone, whose first act is to price a `consumes`-shaped mark against
escape refusal — with the clause that **the compiler never picks the release
call**.

**What a yes does not settle**: the spelling, which the milestone file forbade
this sitting from choosing; which of the two instruments wins, which is the
successor milestone's to measure; and the name of that milestone.

**The conservative resolution is written at R5** and the author may take it
instead: Route C, refuse, with a falsifier redrafted on the corrected census —
*no shipped program leaks a handle and goes on running; all 22 paths end in
`exit(1)` or `abort`*. The sitting did not adopt it because CLAUDE.md § 4 takes
robust where the two disagree, and the correction weakens the urgency without
touching the class.

## What this sitting did not do

It did not decide a spelling, which the milestone file forbade before deciding
whether a form enters. It did not touch M-deferral-ledger's Part 7 items. And it
did not compile Route A, **because Route A does not parse** — every finding
about it rests on hand-inserted C or on reading four draft lines, and that limit
is written here rather than left for a reader to discover.

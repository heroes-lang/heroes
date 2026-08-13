# Panel 046 — what a prediction buys, and for how long

**Convened 2026-08-13**, from `docs/debrief/DECIDE.md`'s open item (panel 043
§ "The finding that outlives both questions"). Three judges, not five, and not
the soundness lane either — see § The lane, below.

---

## The proposal, verbatim

> design.md §1.6 (lines 273-278 and 289-296) lets an addition between the soft
> line 2000 and the ceiling pay with EITHER a named removal OR a pre-registered
> falsifiable prediction. Measured today: the named-removal branch has paid three
> times in eleven amendments; the prediction branch has 26 rows across panels
> 031-042 parked on a harness that has never run, `docs/measurements/` holds five
> files and none is metric-2, and metric 2 is scheduled for M-selfhost-fixpoint by
> author decision 2026-08-12. So a prediction-funded clause is protected until v1
> by an instrument nobody will run before v1. Decide:
>
> - **(a)** an unscored prediction protects its clause only until a NAMED
>   milestone, after which the clause is re-argued or removed;
> - **(b)** the branch closes to new registrations until one metric-2 run exists;
> - **(c)** a prediction is admissible as payment only if it names an instrument
>   that EXISTS on the day of registration, and the 26 parked rows get a named
>   expiry.
>
> Also on the table: design.md:314 asserts the named-removal clause "has produced
> zero removals in seven amendments", which is a dated measurement now false —
> CLAUDE.md §11's expired-premise class.

## The lane, and what it gave up

**Three judges: `spec-warden`, `historian`, `compiler-engineer`.** Neither
published lane fits this question, and the skill's rule — take the full panel
when in doubt — was not the conservative choice here.

The soundness lane is `compiler-engineer` + `ffi-pragmatist`, the two judges who
compile, and it is for a change with no surface, no diagnostic and no spec token.
This proposal *is* such a change. But the lane's argument is that a question with
no reader-facing half has nothing for the other three to be differentiated
*about* — and that is exactly backwards here: this question is the `spec-warden`'s
own brief (§1.2, §1.6), and it has no compiled half at all. Running the soundness
lane would have put the two judges who cannot see the budget in charge of it.

**What the lane gave up, stated so the author can price it**: no
`llm-ergonomist` and no `ffi-pragmatist`. The ergonomist reads only the spec and
one task, and this proposal changes no spec token, so it would have been given
nothing to be differentiated by — the correlated-opinion failure the panel's
whole design exists to avoid. The ffi-pragmatist compiles the C a binding would
need, and there is no C. Both would have voted on the proposal's *prose*, which
is what a panel is for avoiding.

**One consequence of that choice is real and is not hidden**: no judge in this
sitting read the spec blind. If the author believes a governance rule can change
what a reader of the spec does, this sitting cannot see it.

## The finding the sitting corrected before it ruled

**Two numbers in the proposal were wrong, both found by the judge with the tree.**

1. The 372-line file is **`emit/gate.rs`**, not `measure/gate.rs`. The delta
   gate lives in `crates/heroes/src/measure/gate.rs`, which is **121 lines** and
   is in breach of nothing. The §11 sweep is not coming for it.
2. **The gate reads no commit.** Its only assertion is
   `assert_eq!(measured.max(), SPEC_TOKENS)`, inside `#[cfg(test)]`. There is no
   git invocation in `measure/`, no CI directory, and no commit-msg hook — the
   module doc says it outright: *"the gate does not enforce the rule: it makes
   the rule's question unavoidable."* **It cannot tell a removal from a
   prediction, and cannot tell either from silence.** So all three options are,
   mechanically, edits to prose — which is what makes the choice between them
   about discipline rather than about enforcement.

And the proposal's own headline figure was loose. Re-measured for this file:

| | count |
|---|---|
| ledger rows from 2231 to 2974 (`measure/gate.rs`) | **14** |
| of those, rows paid with a **named removal** | **4** |
| of those, rows paid with a **registered prediction** | **6** |
| of those six, **scored** | **0** |
| Σ delta 2231 → 2974 over 13 transitions | **+743**, mean **+57.2** |

The "26" of the proposal counts judges' registered predictions across sittings,
which is a different population and includes several already scored. The number
that carries this ruling is the one in bold: **six ledger rows bought with a
promise, none of it ever collected.**

## Verdict table

| judge | verdict | section | cost / delta | prediction | condition |
|---|---|---|---|---|---|
| `spec-warden` | **adopt-with-condition**: (c) adopted, (a) as its enforcement half, **object** to (b) | design.md:273-278, 289-296, 314; §1.2 at 186-200 | spec **2974 → 2974, delta 0**. design.md is **51,122 tokens, 17× the spec, budgeted by nothing** — the asymmetry is the finding | over the next 8 `SPEC_TOKENS` rows, ≥5 name a removal or a prediction scorable by an existing instrument, and the mean delta falls from **+57.2** to **≤+35**. Falsified above +45 | flips to **veto** on (c) if the first three rows after adoption name an instrument in name only (a corpus, a milestone, a future harness renamed) |
| `historian` | **approve (c)**; **object to (a) as written**; (b) defensible but **unprecedented** | — (advisory, no veto) | — | if (a) lands as a bare named milestone, **≥80% of outstanding predictions will be renewed with text differing only in its date or milestone name**. Instrument: diff each renewal against its registration | reverses on (a) given one documented single-principal regime where a calendar deadline produced retirement over renewal above ~50%. Found none |
| `compiler-engineer` | **object** (not veto — no language construct proposed); adopt (c) with (a)'s bookkeeping in `/step` | CLAUDE.md §10's stopping rule; `measure/gate.rs:10-13` | (a) ~1 line per ledger row, **0 compiler lines**; (b) ~4 lines; (c) ~6 lines of prose, or **~10 lines** as a `const` ratchet mirroring `RESERVED_WORDS_TOKENS`. The `enum Payment` ledger is ~60 lines and **objected to** | at M-program-corpus close, if no `/step` bullet lands, `SCHEDULED.md`'s "Score at M-ffi-ladder close" row is **still open two milestones past its scoring point**; and `wc -l measure/gate.rs` ≤ 140 with zero `std::process::Command` | **veto** the moment any option requires the gate to invoke git or read a commit message — a new capability under §10, which the file's own doc already refused |

## Where they disagree, unsmoothed

**On (a) they do not agree, and the disagreement is the sitting's content.**

The `spec-warden` wants (a) as (c)'s enforcement half, conditioned only on the
named milestone not being M-selfhost-fixpoint — *"otherwise the expiry is the
tenancy."* The `historian` says a bare named milestone is the one shape the
record shows failing, and brings four regimes:

- **IETF Internet-Drafts** expire after six months, and RFC 2026 §2.2 lets any
  new version restart the clock. The community re-posts. *The draft proposing to
  abolish expiry — `draft-thomson-gendispatch-no-expiry` — itself expired*
  (datatracker, 2024-07-23).
- **Sunset clauses**: *J. Regulatory Economics* 68:85-123 (2025), a state-year
  panel 1963-2022, finds no efficiency effect — agencies *"seem to be renewed
  from the political inertia that they were meant to overcome."*
- **FDAAA 801** is the closest analogue and the strongest evidence: results due
  in 12 months, fines up to $10,000/day, statute from **2007**, first Notice of
  Noncompliance **2021**, **zero civil money penalties** ever assessed, and
  compliance at **40.9%** (DeVito, Bacon & Goldacre, *Lancet*, 2020). A missed
  deadline **never deletes the registration**.
- **Java preview features (JEP 12)** are the one regime where expiry bites, and
  *it is not a clock*: nothing previews whose *"design, specification, and
  implementation are all complete"* is not already true, and a release boundary
  invalidates the artifact and demands a new JEP. String Templates were
  genuinely dropped (JDK 23) by that mechanism.

The `compiler-engineer` sides with the historian by evidence rather than by
argument: **this project has already run experiment (a) and failed it.**
`docs/debrief/SCHEDULED.md`'s panel-036 item says *"Score at M-ffi-ladder
close"* and is still `- [ ]` today, with M-ffi-ladder closed 2026-08-12. The
ledger's 2675 row says *"scored at the next FFI rung"*, unscored. So a named
milestone with nobody obliged to look is the shape that has already lapsed twice
in this repository.

**On (b) all three converge, from three different directions** — and it is worth
naming, because (b) is the option that *sounds* most disciplined. The warden:
metric 2 waits for v1, removals were unavailable at three consecutive sittings,
so (b) freezes the spec while §1.0 still owes tokens — *"a rule that can only be
satisfied by an act three sittings failed to perform is a freeze wearing a
discipline's clothes."* The engineer: it converts design pressure into one
blocking dependency, which is Rust's accepted-but-unimplemented backlog. The
historian: no surveyed regime ever imposed a moratorium on registration pending
construction of the instrument.

**Where the analogy breaks, in the historian's own words, and it cuts against
its own evidence**: every regime surveyed polices *strangers with divergent
interests*, and deters **defection** — a sponsor who benefits from burying a
null result. This project has one author, one assistant and no adversary. The
failure mode here is **drift**, not defection, and fines, naming and deletion
are all anti-defection instruments. Importing the sanction without the adversary
is cargo cult. What survives that objection is only the *shape*: put the burden
at entry, where it is checkable, not at a back end where enforcement must be
summoned later and empirically never is.

## Resolution — provisional, author ratification pending

**R1. (c) is adopted.** A registered prediction is admissible **as payment**
only if it names (i) the instrument that will score it and (ii) the milestone at
which it is scored, **and that instrument exists on the day of registration** —
metric 3, `heroes mutate`, `heroes measure`, a line count, a compile, a
diagnostic transcript. A prediction naming an instrument nobody has built is
still **registered as an observation**; it simply pays nothing. This is not a
new burden: panel 040 #2 (`heroes mutate --survivors`), panel 041 #1 (a line
count) and panel 043 #3/#4 already qualify, and **every prediction this project
has ever scored named an instrument that existed when it was written.**

**R2. (a) is adopted in the JEP 12 shape, not as a clock.** An outstanding
prediction is **re-decided, never renewed**. At the milestone it names it is
scored — or it is marked `lapsed` in the ledger row that spent it, and the
clause it bought is re-argued **on its merits, under the removal branch**. A
renewal that differs from the registration only in its date or milestone name is
not a re-decision, and the historian's prediction is the instrument that catches
one. **Nothing is ever deleted from the record**: no surveyed regime de-registers,
and CLAUDE.md §14 forbids it here independently.

**R3. (b) is refused**, on all three judges' grounds. It is recorded rather than
argued away: if the author disagrees, the cost of taking it is that the spec is
frozen at 2974 until M-selfhost-fixpoint, including for §1.0 compiler-need items.

**R4. The bookkeeping is one bullet in `/step`'s milestone-close checklist, and
no compiler line.** The `const` ratchet the engineer priced at ~10 lines is
**not taken**: it would count unscorable payments without naming them, and the
ledger row already carries the name. Any form that makes the gate read git is
vetoed under §10, by the file's own recorded refusal.

**R5. Two expired premises in design.md are repaired in the same commit**, both
CLAUDE.md §11's class — a premise that dies silently while the sentence goes on
reading as correct:

- **:314** *"the 'named removal' clause has produced zero removals in seven
  amendments"* — false; four of fourteen ledger rows carry one.
- **:289-291** *"the observed mean of +30 tokens per amendment"*, concluding
  4096 binds *"after ~61 more amendments … a threshold that fires after five
  dozen panels is not an instrument."* The measured mean is **+57.2**, so the
  ceiling binds in **~20**, not 61. **This was not in the proposal**; the warden
  found it while checking the other one, and it is the more load-bearing of the
  two — the sentence that calls the ceiling toothless is itself false, and the
  headroom is roughly half what the document claims.

Both are repaired **by pointing at the ledger rather than by restating a
number**: a figure that exists in one place cannot expire silently, which is
CLAUDE.md's one-place rule doing the work a test would otherwise have to do.

**What a veto would compel.** The engineer's veto fires only on git-reading
mechanisation, which R4 does not take. The warden's fires if the first three
rows after adoption name an instrument in name only — in which case (b) returns
as the fallback, with the freeze cost stated in R3. The historian holds no veto.

## Predictions to score

Each names its instrument, and **every instrument below exists today** — which is
R1 applied to this sitting's own output.

| # | judge | prediction | instrument | scored at |
|---|---|---|---|---|
| 1 | `spec-warden` | over the next **8** `SPEC_TOKENS` rows, ≥5 name a removal or a prediction scorable by an existing instrument, and the mean per-amendment delta falls from **+57.2** to **≤+35**; falsified above +45 | the ledger in `crates/heroes/src/measure/gate.rs` + `heroes measure` | M-program-corpus close, or the 8th row, whichever is first |
| 2 | `spec-warden` | `SPEC_TOKENS` reaches **≥3000** on the next amendment (needs +26; median delta 39) | `heroes measure` | the next amendment |
| 3 | `historian` | were (a) adopted as a bare named milestone, **≥80%** of outstanding predictions would be renewed with text differing only in date or milestone name | `git diff` of each renewal against its registration | M-program-corpus close — **and it is scoreable only because R2 refused that shape**, so a score of 0 is the adopted rule working, not the prediction failing |
| 4 | `compiler-engineer` | if no `/step` bullet lands, `SCHEDULED.md`'s *"Score at M-ffi-ladder close"* row is still open two milestones past its scoring point | `grep` | M-program-corpus close |
| 5 | `compiler-engineer` | `wc -l crates/heroes/src/measure/gate.rs` ≤ **140** and `grep -c "std::process::Command"` = **0** — any commit-body mechanisation breaks both | `wc`, `grep` | M-program-corpus close |

## Watch list

- **design.md is 51,122 tokens and budgeted by nothing** (warden, measured). The
  spec has a ceiling, a soft line, a delta gate and a ledger; the document that
  governs it has none of them. Not this sitting's question, and it is the larger
  one.
- **Vendor o200k before the next amendment** — `SCHEDULED.md` (panel 019) says any
  verdict landing within 10 tokens of a ceiling needs it, and at 2974 the next
  row lands within 10 tokens of the old 3000 line.
- **The FDAAA counter-reading the historian could not settle**: if what moved
  compliance was the TrialsTracker's *public naming* rather than the deadline,
  then the cheapest fix is neither (a), (b) nor (c) but a visible ledger of
  unscored predictions — which this repository nearly has already.

---

## Ratification — 2026-08-13, by author decision in `/decide`

**Ratified as it stands: (a).** R1 through R5 are adopted unchanged, and (b) is
refused with the cost the sitting priced for it — a spec frozen at 2974 until
M-selfhost-fixpoint, §1.0 compiler-need items included — recorded rather than
argued away, so a later reader can see what was declined and at what price.

Three things the ratification settles that the provisional default left implicit.

**The rule is not retroactive, and the six rows are not invalidated.** R1 governs
what is admissible *as payment on the day of registration*; it cannot reach back
and un-buy a clause that is already in the spec. What R2 does reach is the
future of those six rows: each one now carries the milestone at which it is
**re-decided**, written into the ledger entry that spent it, so `/step`'s close
bullet finds it by the same `grep` it already runs. That is the whole of what
"the parked rows get a named expiry" means here — a date to *look* at the row,
never a date on which the clause dies.

**The expiry is not M-selfhost-fixpoint for anything that can be re-argued
sooner.** The spec-warden's condition was that the named milestone not be the
one metric 2 waits for, *"otherwise the expiry is the tenancy"*, and the author's
yes carries that condition. Five of the six rows named metric 2 and are
therefore re-decided at **M-program-corpus**, which is the next milestone: not
because metric 2 will exist there, but because that is when the clause is
re-argued under the removal branch if it still cannot be scored. The sixth,
`2675`, named *"the next FFI rung"* — a milestone that closed on 2026-08-12
before the row was three days old, which is the compiler-engineer's evidence
happening a third time — and it is re-decided at M-program-corpus with the rest.

**Prediction 5 bit within the hour, and on the wrong thing.** Writing R2's expiry
into `measure/gate.rs` took the file from 133 lines to 152, past the
compiler-engineer's `≤ 140`. The prediction's stated target is *"any commit-body
mechanisation"*, and what tripped it was **eight lines of prose** — the
instrument counts lines and cannot tell documentation from a `Command`. It was
honoured rather than reworded: the note was cut to four lines and the file sits
at **140** with `grep -c "std::process::Command"` still **0**. R2 forbids
renewing a prediction, and rewriting one's threshold the day it binds is the same
act under another name. What it cost is one paragraph of explanation moved from
the ledger into this section, which is where an argument belongs anyway. What it
shows is worth keeping: a line count is a **proxy**, and this project has now
watched one fire on the thing it was not aimed at — the same defect class as an
instrument that does not exist, arriving from the opposite side.

**What is now watched, and by what.** The warden's veto condition stands live:
it fires if the first three `SPEC_TOKENS` rows after this ratification name an
instrument in name only — a corpus, a milestone, or a future harness under a new
name. If it fires, (b) returns as the fallback with R3's freeze cost. Nothing
about that check is mechanised, and R4 keeps it that way: the gate reads no
commit, calls no git, and the engineer's veto fires on any form that makes it.

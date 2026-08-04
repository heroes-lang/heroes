# Panel 018 — the declaration and block-header shape

Date: 2026-08-04. Trigger: author proposal while reading the gallery — the
top level has three shapes, not one (`name = kind[: type]`, `test "…"`,
`extern name = function: sig`), and the author asked for one. The axis —
which side of `=` the keyword sits on, and what a header ends with — had
never been examined: §4.2's historical rejection (design.md:642-651)
litigated what *follows* the keyword only. Convened in an isolated worktree;
no language change lands with this file. Five judges, differentiated inputs
(panel 000 rules): the ergonomist judged label-stripped spec variants blind,
the warden re-measured every number, the ffi case was compiled against real
headers, the historian's surveys are sourced. Numbered 018 — 016/017 were
ratified the same day. The sitting ran against spec v0 (then the frozen
baseline); **spec v1 landed mid-session** after the metric-3 baseline run
(DESIGN-LOG 2026-08-04), still in the A shape — the subject is unchanged and
the measured deltas below are shape-attributable and transfer to v1's base.

## The candidate amendment (distilled)

Keyword-first declarations, no `=`, the parameter list attached to the name,
and the condition loop split out as `while`:

```
constant MAX_DEPTH: int
    64

function dist2(a: Point, b: Point) -> int
    dx = a.x - b.x
    return dx*dx + dy*dy

record Point
    x: int
    y: int

variant Token
    num
        v: int
    plus

test "3-4-5 triangle"
    assert dist2(Point(x: 0, y: 0), Point(x: 3, y: 4)) == 25

extern function sqrt(x: f64) -> f64

while cond          # the condition loop — `for cond` dies
for x in xs         # `for` keeps iteration only
```

shown here in its **F** trim (bare headers). The one open sub-decision is
**E vs F**: whether every block header additionally ends in a Python-style
`:` (`record Point:` · `function dist2(…) -> int:` · `while cond:` · a
payload case `num:`; body-less `extern` never takes one). Statement-level
bindings (`x = 5`, `v: int @ 0`, `v @ v + 1`, §4.4) are untouched; `=` keeps
one meaning and one direction everywhere.

Measured (`heroes measure`, both vendored tokenisers, maximum binds, spread
59-60; every variant edits only the nine declaration/header example sites of
spec v0, so the deltas isolate the shape):

| Variant | legacy / cl100k | Δ vs v0 |
|---|---|---|
| A — status quo on v0 (the panel-011 baseline) | 1989 / 2048 | 0 |
| B — `kind = subject` (the original proposal) | 1990 / 2049 | +1 |
| D — keyword-first, colon after the name | 1981 / 2040 | −8 |
| **E** — attached parens, trailing `:`, `while` | 1994 / 2053 | +5 |
| **F** — attached parens, bare headers, `while` | 1984 / 2044 | −4 |

Spec v1 as it stands measures 2077 / 2136 in the A shape — above the soft
2000, where panel 012's governance binds: an addition needs a named removal
or a pre-registered prediction. The candidate core is itself a removal; E's
colon rides on the ergonomist's pre-registered prediction below.

## Verdicts

The formal sitting judged A-D; two refinement rounds (attached parens +
trailing colon; the `while` split) went back to the same seats with the same
discipline. Condensed per seat; predictions in full below.

| Judge | Outcome |
|---|---|
| compiler-engineer | **Approve F** (net ≈ −45/−60 lines vs today: kills `at_line_start` recover.rs:110-123, `expected_entity` decl.rs:77-83, top-level `expected_eq` :63-71, `expected_signature` :124-131, and `loop_stmt`'s lookahead stmt.rs:161; top-level recovery gains a closed-keyword anchor; `layout.rs` untouched — every F header already ends in a line-ender). **Object E, no veto**: +60-90 lines over F for ~12 `expect(Colon)` sites (3 conditional), the colon re-plants a terminator F gets free, and it legalises at `constant` the adjacency `types.rs:177-192` repairs as a Certain mistake inside function types. **Reject B** (an `=` that inverts against `x = 5`, one failure path per decl for zero discriminating work) and **reject C** (a resolver rule for an unreferenceable name — the §1.7 criterion inverted). `while` split: +10 lines, −1 lookahead, a §1.7 special case leaves; `for x > 0` reaches a failed `expect(KwIn)` where a **Certain** `for`→`while` fix is attachable. LL(1) holds everywhere; no backtracking born; AST/checker/backend byte-identical throughout. |
| llm-ergonomist (blind, label-stripped variants) | **No silent wrong form is constructible under any variant** — every slip is a parse error; the decision is first-try rate. Final ranking **E > F > D > A >> B**. E rides the Python suite-colon motor program (task-1 whole-file pre-registered: E 62%, F 56%, D 55%, A 45%, B 20%); F's error class inverts from omission (0.5-1.5%/header) to insertion (12-18% of files, all loud) but F dissolves E's one wart (`constant MAX: int:`, ~+15pp error on constant-writing code) and keeps `:` strictly one-meaning. Attached parens convert ~50% of D's predicted error mass (the omitted name-colon was "the one prior-hostile atom"). `while` split: clear win, no trade — the old `for cond` would draw `while` emissions at 25-40%, the split predicts <2%. B objected-strong: direction inversion ≥30%, `=` polysemy against the spec's own "binds once, forever". |
| spec-warden | **No budget veto anywhere** (worst case well under the hard 3000). B's own "less spec" claim measured false (+1). F is itself a removal — panel 012 governance satisfied above the soft line; E is an addition there and rides on the ergonomist's pre-registered prediction. The sitting's frozen-baseline conditions are now moot in the right way: the metric-3 baseline run happened and v1 landed, so an adoption amends design.md **and** the spec (v1→v2) with the spec-compiler consistency tests updated in the same commit — the regime DESIGN-LOG 2026-08-04 established when the freeze lifted. |
| ffi-pragmatist | ABI byte-identical under every option, **verified by compiling** against real `math.h`/`sqlite3.h`. **Reject B**: it deletes the symbol-kind word (clang: "different *kind* of symbol"); `sqlite3_temp_directory` becomes unspellable. E/F's `extern function sqrt(x: f64) -> f64` keeps the kind slot extensible; the extern-*global* spelling must be committed at the FFI milestone. |
| historian (advisory) | **B unprecedented-bad**: 0 of 17 shipped languages put the keyword left of `=` (TOML states the `key = value` reading verbatim; R's `->`, the lone name-on-the-right form, is shunned by its own manual). Keyword-first is the Pascal→Go→Nim→Carbon mainline; Carbon documents the introducer rationale; no language ever flipped declaration shape after shipping. **For E**: the trailing colon is the rare syntax with a user study behind it (ABC added it after testing; Python FAQ preserves the rationale); Scala 3 *added* `:`+indent 16 years post-release; none found that removed one; Python has carried the `x: int`/suite-colon polysemy since 2006 with one documented cost (lambda annotations) inapplicable to Heroes. Named tests fight every first-class test syntax found (Zig/D/Pyret use strings; D-the-language refused names when asked). |

## Rejected along the way (each with its one-line cause)

- **B, `kind = subject`** — the original proposal. Rejected by every seat:
  measured +1 against its own "less spec" claim; inverts `=`'s universal
  direction, and only at top level; deletes the extern kind word; 0/17
  precedent.
- **C, named tests** — a resolver rule for a name nothing may reference
  (tests are strippable by design; resolve/top.rs:43-47). Its entire tooling
  benefit is string selection (`heroes test --only <substring>` + a
  test-string uniqueness diagnostic), precedented by Jest/pytest/go test.
- **Name-everything / entities-with-properties** — externs already bind
  names; in-language test references couple program code to stripped code;
  an attribute/reflection system is new semantics with no compiler need
  (Principle 0) adjacent to forsworn Part 6 territory.
- **A `variable` entity** — answered verbatim by design.md §4.2:673 ("There
  is no `variable` entity. Mutable globals are forbidden (locality)") and
  Part 6 (:1707); the statement-position reading reopens panel 003's
  ratified zero-keyword rule against §4.4's once-marked design.
- **`constant`→`global`** — token-identical measured; names the scope where
  the keyword set names the nature (all four entities are equally global,
  design.md:676-677); imports Python's *mutable* `global` prior against
  §4.2:673; Ada spells it `constant` verbatim.
- **`variant`→`union`/`sum`** — `union` already rejected at design.md:660-661
  (C's union is untagged; the keywords.rs trap would flip); `sum` is used
  by the spec's own example (`.sum s => sum_of`) and reserved as a future
  builtin (design.md:1215). The product/sum symmetry is already the stated
  reason the `record`/`variant` pair was chosen (:653-654).

## Resolution — **A stands**, provisional — author ratification pending

Nothing lands with this file (most conservative). Recorded as the candidate
amendment, every component carrying an accepted §1-derived argument:

1. **Keyword-first, no `=`** (unanimous among expressed verdicts): removes
   three compiler special cases, upgrades top-level recovery to a
   closed-keyword anchor, measures negative, wins blind, precedent mainline.
2. **Parameter list attached to the name** (`function dist2(a: Point, b:
   Point) -> int`): converts the largest predicted error class, −8 further
   compiler lines, LL(1) preserved (declaration `function name(` vs function
   type `function(` split by token 2).
3. **`while cond` / `for x in xs`** — two keywords, two meanings, replacing
   one keyword with two grammars: a §1.7 special case leaves the compiler,
   the `while` trap reverses into a parser-position **Certain** fix, and the
   ergonomist predicts the 25-40% `while`-emission class collapses to <2%.
4. **E vs F (the trailing `:`) stays open, pre-registered on both sides**,
   the panel's one unresolved split (ergonomist + historian for E; engineer
   for F; tokens within spread). Designated tiebreaker: the first metric-2
   run measures colon-insertion under F against colon-omission under E —
   flip conditions already stated by both judges (F wins if insertion <8%
   file-level; E wins if it beats F by ≥4pp on whole files). Conservative
   default if forced before measurement: **F** (a pure removal, no new
   diagnostic class, and every deletion the core promises survives).

Adoption mechanics under the post-freeze regime: design.md and the spec
(v1→v2) amend in the same commit, with the spec-compiler consistency tests
updated alongside; the sweep (~70 files at the sitting; the engineer's
prediction says >100 past M4 — **M3 closed at tag `m3`, so the window is
now**) lands in one step — `fmt.rs`, gallery, hand-edited goldens
(CLAUDE.md §9), design.md's appendix — with determinism and both acceptance
tests green. The extern-global spelling is committed at the FFI milestone; a
golden pins body-level `name = function: (…)` as a loud error;
`heroes test --only <substring>` + test-string uniqueness may land under any
resolution.

## Predictions to score

1. **engineer** (M4 close): core adopted before M4 → `decl.rs` + `data.rs` +
   `recover.rs` (436 at the sitting) ≤ 400, `at_line_start` deleted,
   `cursor.rs` peek down to one user; adoption missing M4 → the sweep
   exceeds 100 files. Under E specifically: the five syntax files gain ≥50
   lines over the F counterfactual and ≥3 goldens carry a
   missing-header-colon diagnostic.
2. **ergonomist** (first metric-2 run, n≥40/arm, all pre-registered):
   task-1 whole-file first-try E 62% / F 56% / D 55% / A 45% / B 20%;
   F colon-insertion 12-18% of files (all loud) vs E omission 5-8%;
   E's constant wart ~+15pp on constant-writing tasks (F: dissolved);
   old `for cond` draws `while` at 25-40%, the split <2%; silent-error
   delta ≤2pp everywhere (metric 3, now live, can partially check the
   silent side early).
3. **warden** (first metric-2 run): <10% of first-try failures attributable
   to declaration shape under A; the candidate earns its re-hearing at ≥10%
   keyword-first slips.
4. **ffi** (M7): the SQLite binding needs an extern-global line — one line
   under the candidate, unspellable under rejected B.
5. **historian** (metric-2): A-vs-D statistically indistinguishable (in
   deliberate tension with prediction 2 — scoring one scores the other);
   spurious trailing-colon insertion measurable under any colon-less syntax.

## Watch list

- `spec/reserved-words.md` §1.6 scope (open since 013): its Certain fixes
  quote the declaration shape verbatim and move with any adoption; the
  `while` row deletes and a `for`→`while` parser fix is born.
- The extern-global spelling (`extern constant name: type` or equivalent) —
  at the FFI milestone at the latest.
- CLU is absent from design.md's historical appendix and is the named
  ancestor of the current shape A (sourced) — one-line addition candidate.
- `printer/dump.rs:1-11`'s "two deliberate differences" claim needs its `=`
  clause re-read if the core lands (dump and surface nearly coincide).
- Under E only: `language-configuration.json:25` becomes Python's
  `.*:\s*$`; under F the pattern must be keyword-anchored (today's already
  misses function headers).

## Author's verdict — RATIFIED 2026-08-04

Option 2, by author instruction ("vai 2 e implementa tutto"): **the candidate
core is adopted with the F default** — keyword-first declarations without
`=`, the parameter list attached to the name, bare headers, and the
`while`/`for` split. The E-vs-F colon question stays open exactly as
pre-registered: the first metric-2 run measures colon-insertion under F
against the recorded E predictions, and the flip conditions above govern.
The sweep lands in this worktree branch (author: "continua finché tutti i
file del progetto hanno recepito la nuova sintassi F").

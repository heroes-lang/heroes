# Panel 142 — symmetric variant syntax: there is no asymmetry to restore, because there is no destructuring anywhere

**Sat** 2026-09-13 · **milestone** M-deferral-ledger, step 7 · **status**
`provisional — author ratification pending`

**Lane: full five seats plus a completeness critic.** The last Part 7 item in the
ledger. The gate CLAUDE.md § 4 asks once per milestone was given at step 1.

## The item, and why it needed a sitting at all

The language builds a case `.case(field: value)` and matches it `.case name`.
Panel 035 chose, in 2026-08-12, to **restore the symmetry in v2**, over an EBNF
appendix. Panel 133 already corrected half of it on 2026-09-12: the stated cost —
*"construction stays unspecified, so a program written from the spec alone cannot
build a variant"* — stopped being true on 2026-09-11, and the refused appendix
landed inline instead.

**What panel 133 left standing was: *"the asymmetry itself, and the choice to
restore symmetry in v2."*** That is a promise with no date and no return
condition, which is the exact shape this milestone was scheduled to end. **"v2"
is not one of the three verdicts**, and this sitting was convened to give it one.

## The reframing, found by the critic and verified by the coordinator

**There is no asymmetry to restore, because there is no destructuring anywhere in
Heroes.** A record pattern is refused by name, compiled this session:

```
error[expected_pattern]: expected a pattern, found a name (`Point`)
  — `.case`, `.case name`, a literal, or `_` (on `i64`/`str` only)
```

**Build by label, read by dot** holds for records exactly as it holds for
variants, and nobody calls *that* an asymmetry. So the item names one half of a
uniform rule and proposes to break it: **a symmetric variant pattern would be the
first destructuring form in the language**, and would immediately owe
`Point(x: a, y: b)` in patterns too — or leave a new asymmetry one level up, where
variants destructure and records do not. **No seat noticed that the proposal
creates the thing it removes.**

## What the seats measured

**The item's remaining claim is false, and the reader settled it by writing the
program.** Given only the specification, it wrote all six acts — building three
cases and matching three — first try. Its longest hesitation was **forty seconds**,
on the two-field *pattern*, and the inline production settled it; the longest on
any *construction* was twenty. *"Plainly: no. The item claim is already false."*
And it recorded a point for M-stated-grammar in passing: *"without those lines I
would have written `.rect w h`."*

**The asymmetry is load-bearing, and the price of removing it is a rule the
language already has.** `.circle(r: r)` — what every cited language writes — asks
to bind the field's own name. **Heroes forbids shadowing**, and the coordinator
compiled the collision: `error[shadowed_binding]: `r` is already in scope, bound
at line 5 — shadowing is an error here`. So the proposal has a dilemma with no
third branch: keep the rule, and a reader invents a second name at every pattern
while **the same arm compiles or not depending on what the function above it
binds** — a non-local, intermittent error; or exempt patterns from the rule, as
the precedents do, and **an arm silently captures an outer name**, a new
silent-error class in a language whose purpose is that plausible mistakes are
compile errors.

And the reader's sharpest sentence: *"It does not merge two rules; it makes two
rules **look identical and mean different things**."*

**Symmetry is a cost, not a saving** (warden). The most favourable draft — factor
out a production and reuse it — measures **+12**; the honest one, carrying the
sentence a reader needs, **+67**. The claim *one spelling states one rule* is false
on its own terms: the specification already states construction in one clause and
matching in one clause.

**There is nothing to deduplicate** (engineer). The two spellings are **two
algorithms that share a token shape**: construction is 20 lines of parse and an
arg list already shared with calls and records; matching is 22 lines, one optional
binder, and 64 lines of checking that exist only for exhaustiveness, which
construction has no analogue of. §1.7's subtraction is **zero**.

**And the four cited ancestors are falsified for named fields** (historian, each
with a source). Haskell's omission means **⊥ when building and wildcard when
matching** — the same written omission, two meanings. Rust's `E::V { x }` means
**opposite things** in the two positions, a *use* in an expression and a *binding*
in a pattern, and its two grammars overlap rather than coincide. OCaml patterns may
omit fields where expressions may not. **ML has no named-field data constructors at
all**, so that citation is unverified as stated.

**The decisive precedent is Swift, and it went the other way deliberately.**
SE-0155 proposed labels required in patterns; the core team accepted it *with a
modification* on 2017-04-20 because *"requiring associated value labels would be
unduly onerous"*, and settled on labels **mandatory at construction and omissible
in patterns**. That is Heroes' asymmetry, **reached by a committee that had the
symmetric option written down in front of it and rejected it**. Where symmetry was
kept — Erlang's records, Python's class patterns — the price is documented: the
same omission silently means two different things, and a bare name in a Python
pattern is *always* a capture, so constants must be dotted. **And languages move
towards asymmetry on purpose**: GHC's pattern synonyms exist in three flavours
precisely because building and matching are different functions.

## Verdicts

| seat | verdict |
|---|---|
| `compiler-engineer` | **veto** against ENTERS; recommends **REFUSED** — *"'v2' is not available and neither is 'enters'"* |
| `llm-ergonomist` | **refuse**; explicitly not a veto, having gone looking for one: the `=>` keeps the meaning determinable from the line, so locality survives |
| `spec-warden` | against: symmetry costs tokens in both directions, and the item's last live cost is dead |
| `historian` | **object** to the remaining sentence — *"it rests on a claim about four languages that does not survive checking, and on a migration promise no language in the record has kept"* — and supports a Part 6 refusal |
| `ffi-pragmatist` | measured the boundary; its clang experiment prices a direction every seat agrees is already repealed |

## The resolution adopted, provisionally

**Item 16 is REFUSED to Part 6, and "v2" is struck as a verdict the ledger cannot
carry.**

1. **The Part 6 row's falsifier**: *a measured Part 11 first-try rate at which
   readers get variant construction wrong above 15%* — the reader's own threshold,
   which it says would revive the item and reopen the row — **or** a decision to
   admit destructuring anywhere else in the language, at which point building by
   label and reading by dot stops being uniform and the case for one spelling
   returns with it.
2. **The item's text is struck beneath it**, as this ledger has now done five
   times: the four cited ancestors are falsified for named fields; Swift is the
   decisive precedent and went the other way deliberately; and the premise that
   there *is* an asymmetry is false one level up, since nothing in the language
   destructures.
3. **The form that returns is not a syntax change but a sentence** (the critic's
   R1, priced at **+35**): the specification states construction and matching in
   separate clauses and **never states the contrast** — a grep for it returns
   nothing. One clause converts a structural silence into a stated rule at zero
   productions, zero compiler lines and zero migration. It is recorded and not
   adopted, because §1.6's payment is unpaid and this sitting is a verdict rather
   than an amendment.
4. **What this sitting does not decide**: whether the language should ever
   destructure — that is the question one level up, and it belongs to whichever
   sitting proposes a record pattern, not to this one.

**What conservative would have been**: defer with the reader's 15% threshold as
the return condition rather than refusing. The sitting refuses because a veto is
engaged, no seat supports the change, the premise is false above the item, and a
Part 6 row carries a falsifier where a promise of "v2" carries nothing.

## Three corrections to the coordinator, recorded rather than quietly fixed

**The brief's construction count is wrong and the critic could not reproduce it.**
It said 1457; three independent attempts got 678 strict, 1350 loose and **626 from
the compiler's own AST dump**. The coordinator found the cause: the regex counted
**every dotted call with a labelled first argument** — `p.slice(from: 0)` among
them — and not variant constructions at all. The match-arm count, **994**, is exact
and reproducible from the AST. **A regex that looks like a measurement is the
shape CL-057 names**, and the warden restated 1457 as fact inside a Principle 0
argument.

**And two of the seats had their input polluted.** The reader reports that
`CLAUDE.md` and two `.claude/rules/` files *"were pushed into my context by the
harness, unbidden"* — its seat exists to see only the specification. It ignored
them and said so, which is the correct handling; but the panel's differentiation
claim is weaker than it reads while the harness injects the contract into a seat
that must not have it.

## Author's verdict

Pending — queued in `docs/work/DECIDE.md` as `panel 142`.

## Predictions to score

| prediction | instrument | scored at |
|---|---|---|
| reader: under the current text, ≤ 5% of first-try programs get a case constructor wrong at any arity; above 15% revives the item | metric 2's runner | M-thesis-harness |
| reader: ≥ 20% write `.rect w h` in a pattern, and **100% of those are parse errors, 0% silently different** | metric 2's runner | M-thesis-harness |
| critic: the exact construction count from the AST stays near 626 and nothing reproduces 1457 | the AST dump command | any later sitting citing it |
| the falsifier's own clock: a first-try construction error rate above 15%, or a destructuring form proposed anywhere | a sitting that proposes one | every later sitting of this ledger |

## What the seats could not source or could not run

No seat built either direction; the engineer's 150–200 lines is an estimate with
no branch behind it. The warden's symmetric drafts are vendored with the `real`
row declared unrun and scaled, which it says. The reader's four percentage
predictions all name a harness whose task corpus is empty. The FFI seat's clang
experiment measures a direction already repealed by design.md's mandatory field
names. The historian could not verify ML for named-field constructors, SML having
none, and marks the citation unverified as stated.

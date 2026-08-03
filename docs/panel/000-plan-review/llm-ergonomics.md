# Panel 000 — LLM ergonomics review (verdict: OBJECT)

## Top objections (all adopted in revision 2)

**1. The thesis metric could not test the thesis.** "Passes the type checker"
≠ "working program" (design.md §4.18 says so), and there was no comparison
arm: fewer tokens than *what*? **Fix:** grade on `heroes test` passing from
M6; add the within-subject control — `heroes check --permissive`, the same
compiler with the thesis-bearing checks disabled. Same model, same spec size,
same unfamiliarity, opposite design choices: the only comparison that isolates
*design* from training-data familiarity.

**2. Harness at M3, spec at end of M3 — but design.md L77 says build the
harness early.** The pre-amendment baseline would have been unrecoverable
forever, and spec-warden's veto threshold ("budget passes 1500") was a number
that did not exist. **Fix:** spec v0 + baseline in M0, before panels 002/003
amend anything.

**3. Panel verdicts carried no information.** An agent told to find the
strongest objection objects at ~100% base rate; five agents reading the same
document with different hats produce correlated verdicts; numeric fields get
confabulated; session 001 shipped with pre-written verdicts for a decision
already taken. **Fix (adopted wholesale):** differentiate *inputs* — the
llm-ergonomist never reads design.md (spec + tasks only; its verdict is an
experiment); compiler-engineer must cite files and line counts; spec-warden
gets measured counts; historian gets web access or is cut; every verdict
carries a falsifiable prediction scored against the next harness run; blind
A/B where applicable; retro-records marked as such.

**4. "Every diagnostic carries a Fix" was actively dangerous.** Mandatory
fixes force guesses, and a model *applies* them. design.md's own example
("change save's signature to accept a str id") is usually the wrong repair.
**Fix:** `Fix` tagged `certain | guess`; only `certain` machine-applicable;
`x.fixed` goldens where CI asserts the applied fix compiles.

**5. `???` collides with the unused-variable rule** — §4.16's own example
binds `p` used only inside the hole; §4.4 makes that an error. And "nearby
functions" was unbounded (context flooding, layout-dependent). **Fix:** holes
suppress unused-binding errors file-wide; suggestions type-ranked, capped at
5, deterministic.

## Measurement flaws (adopted)

n=20×5 samples with Wilson intervals (n=10 single-sample detects nothing under
~30pp); frozen, hashed prompt templates; API-only, single-turn, spec-only
context (the measured model co-designed the language — never hand it the
repo); one non-Anthropic model as robustness check; provenance stamps (spec
sha, compiler sha, model id, prompt sha, suite sha) with refuse-to-diff on
mismatch; mutation operators as data with per-operator kill rates;
turns-to-green capped at 5; report billed input_tokens from real runs, not
just the spec count.

## CLAUDE.md gaps (adopted as rules 1, 3, 9, 12)

Re-read protocol (spec in full every session; design.md by grep, never from a
remembered summary; uncitable rule = guess); session bookends; the
UPDATE_GOLDEN prohibition; precedence when artifacts disagree (spec beats
compiler; measurement beats opinion). Cyclone-rule ambiguities closed by
tooling (clippy disallowed-types) and the iterator-closure clarification.

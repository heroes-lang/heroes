# Panel 011 — measuring without an API key

Date: 2026-08-04. Trigger: author decision — no API key, and 100 hand-run
chat sessions will not happen. Two judges convened (spec-warden,
llm-ergonomist); the measurement apparatus is their joint mandate.

The session was called to decide how much of Part 11 survives. It ended by
discovering that the premise was wrong twice over.

## The proposal put to the judges

- **A.** Redefine the spec budget against a vendored offline tokeniser.
- **B.** Promote metric 3 (silent-error rate — mechanical, free) to primary
  evidence.
- **C.** Record metrics 2 and 4 as not runnable.

## Verdicts

| Judge | Verdict | Cost/delta | Prediction | Condition |
|---|---|---|---|---|
| spec-warden | approve-with-changes | **measured the spec, offline, in twenty minutes**: v0 = 1989 (Anthropic legacy) / 2048 (cl100k) / 2050 (o200k). The word heuristic said 1496 — low by **33–37%**, not panel 008's guessed 14%. Instrument spread is 59 tokens (3%), not the ~350 design.md feared | an independent Rust `heroes measure` reproduces 1989 ± 3; the closure-complete spec lands ≥2100 cl100k, breaching 2000 with zero discretionary additions | keep the unit as the reader's tokens and publish the spread per run; name the port debt; C is too fatalistic |
| llm-ergonomist | approve-with-changes | the *generative* half of the thesis goes unmeasured by name: metric 3 measures the detector conditional on an error drawn from a distribution the designers chose, and never observes what a model writes. "Measured claim" → "measured mechanism, unproven claim" | per-operator, no pooling: `typo-ident` and `mutate-undeclared` are caught 100% by **both** arms and contribute zero to the delta; `swap same-typed arguments` is the operator that decides the thesis | three-arm reporting (strict / permissive / conventional baseline), a meaning-preserving counter-arm, sealed and externally-cited operators, and metric 2 *attempted* before v1 |

## The finding that reframes everything

**The spec has never been over 1500. It has never been under 2000 either.**

Verified independently before adoption (the warden's 60-line reference
decoder validated against published vectors, then re-run):

| instrument | spec v0 as frozen | without the 5-line header |
|---|---|---|
| Anthropic legacy `claude.json` (65k) | **1989** | 1900 |
| cl100k_base | 2048 | 1952 |
| o200k_base | 2050 | 1955 |

Consequences, stated plainly:

1. **Every budget verdict in this repo rested on a number that was 33% low.**
   Panels 002, 003, 006, 007 and 008 all priced amendments against ~1496.
   The spec was 1989 the whole time. M0's journal line — "the budget is AT
   the edge" — was accidentally right about the situation and wrong about
   the number.
2. **The 1500 budget was breached on the day it was written.** It was never
   a constraint that held; it was a constraint nobody could measure.
3. **The 2000 budget, raised hours before this session, is met by one
   instrument and breached by two.** Under panel 009 rule 1 (the pessimistic
   bound binds), spec v0 is **already over the hard ceiling** at 2050.
4. **Part 11's stated reason for demanding an API is unsourced and
   contradicted.** It claims tiktoken "undercounts Claude tokens 15–20%";
   measured here, cl100k counts 3% *above* Anthropic's own legacy tokeniser
   on this document. The offline instrument's error is ~60 tokens, not ~350.

**"Needs a model in the loop" is not "needs an API key"** (ergonomist). A
local open-weights model runs 20×5 unattended, with no key and no cost, and
for a *comparative* metric a weaker model is arguably the better instrument
because the ceiling effect is smaller. Metric 2 was never dead; it was
mis-scoped.

## Resolution — adopted, `provisional — author ratification pending`

1. **Metric 1 is recovered, offline.** `heroes measure` counts with
   **two vendored tokenisers** (Anthropic legacy + cl100k, pinned by
   sha256), zero dependencies, ~180 lines of Rust. One instrument cannot
   detect its own drift; two disagreeing by 3% *is* the error bar. The
   **maximum over instruments binds**, and the spread is published per run.
   The unit stays the reader's tokens — the vendored encoders are
   instruments, not a redefinition.
2. **The budget is a hard measured ceiling of 2000**, reconciling §1.6's
   "~2000" with panel 009 rule 2. As of today the repo is **in breach at
   2050**, and that is written down rather than rounded away. The next spec
   amendment must be net-negative; the header cut (−89 measured) buys back
   the first amendment and nothing more.
3. **Metric 3 is primary but never pooled.** Per-operator kill rates, in
   **three arms** — `heroes check`, `heroes check --permissive`, and a
   one-time transliteration of the golden corpus to C (`-Wall -Werror`) and
   Python (`mypy --strict`) — plus a **meaning-preserving counter-arm**
   (consistent rename, reordering independent statements, redundant
   binding), because a language that rejects everything maximises catch
   rate. Operators are sealed before running, each cites an occurrence
   outside this project, and none is ever retired — a miss is fixed in the
   compiler, not deleted from the set.
4. **`forget the @ marker` is excluded from any headline.** Its catch rate
   is 100% by construction — the marker exists so that forgetting it errors
   — so it carries zero information. Report it, score it, never lead with
   it.
5. **Metric 2 is not "not runnable".** Minimum viable protocol, in order of
   preference: (a) a local open-weights model, Heroes vs C, same model;
   (b) paced sampling — 2 pre-registered tasks per milestone, ~16 samples
   by v1, inside ceremony the project already performs. n=20×1 gives ±20pp
   Wilson intervals, which is a number where there was none.
6. **The claim is restated honestly.** Part 11 keeps "what separates this
   project from an opinion" for the *mechanism*, and concedes in writing
   that §1.1's stated objective — first-try rate — is unmeasured until
   metric 2 runs. §1.2's rewrite-rate factor becomes a design rule that is
   no longer audited; one sentence says so.
7. **Port debt named.** Rule 10 puts `measure` in the one binary, so at M8c
   a 65k-rank BPE table and a byte-level merge loop must exist *in Heroes*,
   needing file I/O and maps — both already mortgaged closure items. Marked
   now rather than discovered at the port.

## Predictions to score

- warden: Rust `heroes measure` reproduces 1989 ± 3 (legacy) and 2048 ± 3
  (cl100k) on frozen v0 — **when it lands**; the closure-complete spec
  breaches 2000 with zero discretionary additions — **at the M6 audit**.
- ergonomist: `typo-ident` and `mutate-undeclared` kill ≥95% in *both* arms;
  `swap same-typed arguments` is the operator that decides the thesis; a C
  transliteration catches ≥65% of the same set — **at the first metric-3
  run**.
- ergonomist: if a meaning-preserving arm is added, `heroes check` rejects
  at least one of {consistent rename, reordered independent statements,
  redundant binding} — **same run**.

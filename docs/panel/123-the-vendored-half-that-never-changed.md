# Panel 123 — the vendored half was the half that never changed

Sitting of 2026-09-09, out of `docs/measurements/023-the-instrument-was-not-the-readers.md`.
**Four seats, not five**, chosen by CL-023's rule that only the seats whose
input differs are convened: the llm-ergonomist reads `spec/heroes-spec.md` and
nothing else, and the spec is not amended here, so it had nothing to be
differentiated about.

**The question.** `heroes measure` answers from two VENDORED tokenisers and
takes the larger; the larger, `cl100k_base`, is **OpenAI's**. Against
`POST /v1/messages/count_tokens` the spec is **5094** real tokens and the
document was **998 over** a ceiling it was told it was 40 under. The author has
decided the ceiling — 6144, 2026-09-09 — so this sitting is asked the
MECHANISM, and the hard part is one line: **a budget check that needs the
network is red in CI and on a plane.**

## Two more errors in the measurement, and the method is the same one

**The request offset is model-dependent and `023` states it as one constant.**
The spec-warden probed it twice per model over six ids: **6** on Opus 5, Sonnet
5 and Opus 4.8; **7** on Opus 4.6 and Haiku 4.5; **11** on Opus 4.7. So the spec
is **4127** on the older pair, not 4128 — which panel 122 had already caught
once — and the inter-generation gap is **967** rather than 966. And Opus 4.7
counts the same *content* as Opus 5 while wrapping it differently, so **"newest"
and "largest" are independent axes**, which kills one of the candidate rules
below on its own.

**A literal `6` in the instrument would be a premise about a request shape
Anthropic owns**, so the offset is measured in the same invocation that measures
the document, not hardcoded. The compiler seat priced that at ~15 of the ~120
lines.

## The floor is refused, and the coordinator had already told the author it was a fact

`023` recorded Anthropic's minimum cacheable prefix — 512 on the 5-generation,
1024 on Opus 4.8 and Sonnet 5, 2048 on Opus 4.7, **4096 on Opus 4.6, Opus 4.5
and Haiku 4.5** — and the coordinator built a *band* out of it and put it to the
author in those words: the spec lives between 4096 and 6144, one end sourced and
one decided. **The spec-warden refused it on four grounds and each one is
enough:**

1. **It is a property of a request PREFIX, not of a file.** `CLAUDE.md` alone
   measures **5547** on Haiku 4.5, clearing 4096 by **1451** before one spec
   token is loaded — and CLAUDE.md §1 has the spec read *inside* a session that
   has already loaded the contract. On the only workload this project runs, the
   threshold is cleared before the spec is opened.
2. **It inverts §1.6 and §1.2.** A floor makes a REMOVAL a violation and turns a
   forcing function into a band to centre. Padding a document to buy a cache
   discount pays in the factor §1.2 says dominates by orders of magnitude.
3. **`.claude/rules/module-shape.md` refuses it in this shape.** A vendor price
   list for three legacy models is a premise about the world, and this one
   expires silently *and upward-invisibly*: "keep the spec above 4096" goes on
   reading as correct long after the reason dies.
4. **The margin is inside the old error bar.** 4127 against 4096 is **31**
   content tokens, less than the 78-token spread this project was calling its
   error bar. A claim indistinguishable from noise at the project's own
   precision is not stated as a fact.

**And the seat could not verify the minimums, because this session's own guard
stopped it**: `/v1/models` was refused with *"the Anthropic key is for
`/v1/messages/count_tokens` and nothing else"*, the fence added hours earlier by
author instruction. So the four numbers are vendor-sourced and unmeasured here,
and they enter the record as a question rather than a premise (CLAUDE.md § RUN
IT). The fence worked, against the coordinator's own panel.

**What the record may claim**: the minimum exists, is model-dependent and
non-monotonic, is 4096 on three named legacy ids, is vendor-sourced and
unmeasured here, applies to a cached prefix and not to a file, and is already
cleared by `CLAUDE.md` alone. **What it may not claim**: that the spec lives in
a band, that 4096 is a floor, that a removal below it costs cacheability, or
that both ends are measured.

## Nothing guarantees a tokeniser, and the vendoring pinned the wrong half

The historian's central finding, and it is the one that reframes the whole
instrument.

**No stability guarantee exists, and the page that fails to give one is
sourced.** OpenAI's cookbook: *"Consider the counts from the function below an
estimate, not a timeless guarantee"*, and the `tiktoken` README carries no
versioning scheme and no immutability claim for a named encoding.

**And the surface that decides tokenization was edited twice under the fixed
name `cl100k_base`.** `tiktoken_ext/openai_public.py`'s `pat_str` — the
pre-tokenizer regex that decides where text splits — changed in PR #234
(2024-02-09, *"Optimize regular expressions used for splitting by ~20%"*) and PR
#258 (2024-10-03, possessive quantifiers against catastrophic backtracking),
merged on a round-trip test with **no proof of output equivalence**, and #258's
own author left a Unicode-whitespace case open. Hash verification of the
downloaded rank files was only added in 2024-01-30.

**So this project vendored the half that never changed.** The ranks are
content-addressed by `expected_hash`; `pat_str` lives in *code*, at the library
version. Hashing the ranks does not pin the tokeniser, and "we vendor it, so it
is reproducible" was never true of the part that moved.

**Anthropic's own documentation endorses the mechanism this sitting adopts**:
*"Recount prompts against the model you plan to use rather than reusing counts
measured against earlier models"*, and 4.7-and-later *"produce approximately 30
percent more tokens than on earlier models"*. **A correction the historian
insisted on**: that 30% is Claude-pre-4.7 against Claude-4.7+, while the
coordinator's 27.5% is cl100k against Claude. Two different comparisons of
similar magnitude, and writing them as corroborating each other is how a false
citation is born three documents later. The figure that belongs beside the 30%
is the **967**-token inter-generation gap.

## Two holes in the coordinator's enumeration, and one is the root cause

The compiler seat found both.

**`design.md:3080-3088`, Part 11 metric 1, was not on the list, and it carries
the old instrument as a RULE**: *"With two vendored BPE tokenisers … pinned by
sha256 and read offline — no API key, no network."* Worse, `:3085` says the
15-20% undercount claim *"is unsourced and was contradicted by measurement"*.
**That sentence is the root cause and not a symptom**: the document overrode a
correct vendor claim by comparing two instruments neither of which is a Claude
tokeniser. Its offline commitment is right and survives; its undercount clause
is corrected underneath, dated.

**`design.md:3101` already licenses the model id**: *"Every measurement records
provenance: spec sha, compiler sha, model id, prompt sha, suite sha."* So "the
model beside the number" is Part 11 applied, not new design, and it is cited
rather than invented.

**And the ceiling is live in 25 places across 8 files**, including **four inside
the panel seats' own prompts** — `.claude/agents/spec-warden.md` three times and
`.claude/agents/llm-ergonomist.md` once. The warden's words: *"my own brief is
one of the stale carriers; that is panel 069's failure written into the agent
definitions."*

## What precedent says, and it is unanimous on the mechanism

**Go writes the instrument's name beside the pinned data, for the stated
reason.** `go.mod`'s `toolchain` directive: *"For reproducibility, the `go`
command writes its own toolchain name in a `toolchain` line any time it is
updating the `go` version"*, with `go.sum` for the data and `GOPROXY=off` for
the offline switch. Pinned data, instrument identity, an offline mode. **A
recorded token count carrying `claude-opus-5 @ 2026-09-09` is that line.**

**`size-limit` is the failure this project lived**: absolute thresholds
(`"limit": "35 kB"`) with the compressor selected but its version never
recorded, so a brotli upgrade moves the metric silently.

**Chromium gates on a DELTA, not a level**, with a human override in a commit
footer (`Binary-Size: $ANY_TEXT_HERE`), and pre-*normalises* the measurement
rather than trusting it raw.

**Bazel, pip and Nix converge on the same policy**: the network is admissible
only where the answer is pre-recorded. Bazel's `sha256` — *"It is a security
risk to omit the SHA-256 as remote files can change. At best omitting this field
will make your build non-hermetic"* — plus `requires-network` and
`block-network` test tags; pip's hash-checking mode, which names the plane
problem out loud, *"to protect against remote tampering and network issues"*;
Nix's fixed-output derivations, where *"impurities such as these are okay since
(the hash of) the output is known in advance"*. **`--refresh` is
`nix-prefetch`, `pip-compile --generate-hashes`, and Bazel's
before-you-ship-it sha256.**

**And `rustc-perf` is a mature project that has not solved this and says so**:
compare *"within a given configuration (not across different configurations)"*,
and *"none of our policies … currently consider what to do in case of
conflicting benchmarking results"*. Compare-only-within-configuration is the
same rule as never-compare-across-tokenisers.

## Verdicts

| seat | verdict | section | cost / delta | prediction | condition |
|---|---|---|---|---|---|
| compiler-engineer | **object**, veto **held** with its trigger named | design.md:3080-3088 (Part 11 metric 1), `.claude/rules/cli-surface.md`, §1.6 | **~195** new `.hero` lines, 0.35% of `selfhost/`; **zero** new externs, zero lexer/checker/descriptor/ownership/emitter cases; the seed regenerated | at the close, `grep -c "^extern "` still answers 4, the diff adds ≤160 lines, and `seed/README.md`'s clang line is unchanged | fires the veto the moment any check, suite, CI step or default invocation reaches the network, or `--refresh` is written as `link "curl"` |
| spec-warden | **object**, veto **withheld** with its trigger met | §1.6, §1.2, §1.1, module-shape, records | spec delta **0** — the spec is not amended here; §1.6 itself is **2399** real tokens, 47% of the artifact it budgets, so the rewrite is subtractive, **−150 to −250 estimated** | at M-selfhost-fixpoint close the spec reads **≤5400** real and the vendored ratio stays in **[1.26, 1.29]** | **vetoes any commit that adds a token to `spec/heroes-spec.md` before §1.6 states a ceiling with its pinned model id and date** — the +126 interpolation clause included |
| historian | **approve**, advisory | precedent | 30 sourced precedents; **11 claims explicitly unsourced and named** | there exists an input on which `tiktoken` 0.5.2 and 0.14.0 disagree for `cl100k_base`, and the class is whitespace | a published stability guarantee from either vendor would make the refresh optional; it searched four pages and found the opposite in each |
| ffi-pragmatist | **not convened** | — | the route needs no FFI: `selfhost/cli/process.hero` already runs a named program with an argv list and already reads `ANTHROPIC_API_KEY` | — | — |

**The ffi seat was dropped mid-sitting rather than at the start, and that is
recorded rather than tidied**: the brief was written expecting libcurl through
the FFI, and the compiler seat measured that no FFI is needed at all — `curl` is
run as a program by machinery that already exists, for **zero** new externs. A
seat with nothing to be differentiated about is a seat that should not sit.

## Disagreements, unsmoothed

**The two compiling-adjacent seats disagree about which number binds.** The
engineer's `REAL_TOKENS` is *the maximum over the model ids measured*, on the
ground that design.md:3082 already states that rule for the vendored pair, so a
fifth model can only raise it — the safe direction. The warden **rejects
max-over-set as the gate**: the max can only rise as the set grows, so the
document shrinks because a vendor shipped, which is the failure that just cost
998 tokens with the sign flipped. Its rule is **one pinned model id, moved only
by author decision**, with the spread over other ids published beside it and
gating nothing.

**The resolution takes the warden's**, and the deciding fact is the warden's own
measurement: Opus 4.7 counts the same content as Opus 5 and wraps it
differently, so the set has two axes and a maximum over it is not a property of
the document at all. What the engineer's rule buys — that a new model cannot
silently grant headroom — the pinned rule buys too, because nothing moves until
somebody decides.

**Both seats hold a veto and neither fires.** The engineer's trigger is the
network in a gate; the resolution forbids it, so the trigger is not met. The
warden's trigger IS met right now — 5094 measured against the 4096 §1.6 states
— and it declines to fire on panel 069's precedent, because it would veto the
untouched document with nothing on the ballot.

## Resolution — PROVISIONAL, author ratification pending

**R1. The gate never touches the network. Ever.** Not a check, not a suite, not
a CI step, not a default `heroes measure`. design.md:3081 forbids it verbatim;
CI has **zero** `secrets.` references; and the repository is public, so GitHub
passes no secrets to fork PRs — a key-gated check is structurally red on every
fork PR, forever. **And no "refresh if a key is present" branch**: a check whose
verdict depends on the environment is a premise about the world that expires
silently.

**R2. Two questions, two instruments, and only one gates.** *Did the spec move
and nobody say so* is already answered offline and exactly by `SPEC_TOKENS`, a
pinned equality on the vendored count — it was never the broken part, only its
interpretation was, and it stays, renamed to say what it is. *Is the spec under
6144 real tokens* is answered by a **record**, refreshed by a human act.

**R3. One pinned model id binds**: `claude-opus-5`, **5094**, 2026-09-09,
recorded with its own measured offset. The spread over other ids is published
beside it and gates nothing. A new model moves nothing until a sitting measures
it and the author decides — which is `023`'s own cure, *the ceiling carries its
tokeniser and its date*.

**R4. `--refresh` prints, and a human pastes.** The flag emits the constant
lines on stdout at exit 0, `--dump`-shaped, obeying
`.claude/rules/cli-surface.md`'s artifact-on-stdout contract. No
constant-rewriting mutator inside the compiler, and no libcurl: `curl` is run as
a program through `selfhost/cli/process.hero`, which already runs a named
program with an argv list and already reads the environment. **Zero new
externs.** The offset is measured in the same invocation, never hardcoded.

**R5. Staleness withholds the verdict rather than computing one.** The record
carries the spec's content digest (`selfhost/cli/digest.hero`, offline,
deterministic, already the cache key). On a mismatch the tool prints
`STALE: recorded at <a>, this file is <b> — run heroes measure --refresh` and
exits 1. **A headroom sentence against a stale count is the exact sentence that
cost 998 tokens**; printing it in a fainter voice repeats the defect. The hash
gates, red on stale — and the dilemma about a keyless contributor dissolves on a
measurement: an edit under `spec/` already needs a panel, a ledger row and a
DESIGN-LOG line, and is already red on `spec/budget` and `spec/ledger` before
`spec/real` is reached. The marginal cost is zero.

**R6. What `heroes measure` prints changes, and the `spread` label is the worst
of the three.** The vendored rows stay — design.md:3080 wants both published and
their determinism is their value — relabelled from *the binding number* to **a
lower bound, not the reader's tokeniser**. `spread 78 between the two vendored
tables` is joined immediately by `generation gap 967 between Opus 5 and Opus
4.6`, because today the tool calls 78 *"the error bar"* while the measured
disagreement is 967, and **that single label is what cost 998 tokens**. One new
row is the binding one, read from the record. The headroom sentence keeps
`spec/spendable`'s exact substring contract and is computed from the real
number, which makes the printed number and the checked number one number again.

**R7. The 2000 soft line is RETIRED, not rescaled**, and it is the removal this
change pays with. It was set on the vendored instrument when the spec was 2231;
on the real one the spec has been above it since v0, so it has never once been
slack, and §1.6:277-282 forbids rescaling it with the ceiling. The payment rule
becomes **unconditional**: every amendment owes a named removal or a registered
prediction, at every level. That deletes a drift surface and strengthens the
rule.

**R8. The 38 ledger rows are renamed and annotated, never restated.** SPEC's
Fair Use rule is the model — a non-conforming number may be published and must
be labelled, and implying it is compliant is the violation — and Wikimedia's
*Legacy Pageviews*, which renamed the old series and documented the
discontinuity, is the shape. So: the series is named for its instrument, the
boundary is dated, **the figures stay as they are**, and the note says to read
them as deltas and never as levels. Deltas survived re-measurement — 022's +118
and +99 became +151 and +126 and the cheaper spelling stayed cheaper — while
every level and every headroom figure derived from them is void. **No row is
convertible**: 1.275 is a property of one day's mix of prose and code, not a
factor.

**R9. Part 11 metric 1 carries its own correction.** `design.md:3080-3088` keeps
its offline commitment and gains, underneath and dated, the correction that its
undercount clause is now sourced and measured at +27.5% and +33.4%. It is the
root-cause sentence and it says so.

**R10. The landing order is one commit for the ceiling and a second for the
flag**, and the first is indivisible: `tests/harness/suite_spec.hero:354` builds
the exact substring `selfhost/cli/measure.hero:214` prints, so the two `CEILING`
constants cannot land apart without `spec/spendable` red in between. **Inside
it, three traps the engineer named**: `SPEC_TOKENS` stays **3995**, because it
is the vendored max and the spec text is not changing; `LEDGER_ROWS` stays
**57** and **the ceiling is not a ledger row**, because `ledger_figures()` reads
a row's first cell as the spec's figure and fires on `newest != SPEC_TOKENS`, so
a row reading 6144 turns the suite red; and `site/src/lib/claims.ts:62` reads
`/^constant CEILING: i64\n\s+(\d+)$/m`, so the number stays on the line under
the declaration.

## Author's verdict

**RATIFIED 2026-09-09** (author instruction, `1a 2a 3a 4a`, meaning all four
sittings as recommended), R1 to R10 in full, **including the retirement of the
2000-token soft line**, which was the item the recommendation flagged as a rule
of §1.6 going away.

**The ground for retiring it is a measurement rather than a preference**: on the
real instrument the spec has been above 2000 since v0, so that line was never
once slack and its branch was unreachable in practice, while §1.6 forbade
rescaling it with the ceiling. What replaces it is stricter: the payment rule is
now **unconditional**, so every amendment owes a named removal or a registered
prediction naming an instrument that exists today, at every level.

**The floor's withdrawal is ratified as a correction and not as a decision.**
Nothing had been decided on it, so nothing is undone; the author was told the
spec lives in a band with both ends measured, and one end was a vendor's price
list about a different kind of request.

**And the whole resolution had landed before the ratification, with one part
landing after the sitting's own commit.** `--refresh` shipped 2026-09-09 in
`757a14af` and, run against the endpoint, **reproduced the hand-measured record
exactly**: 5128, `claude-opus-5`, 2026-09-09, digest `6022a8bc2f16ca0e`. The
ordering veto in this sitting's own record is therefore satisfied, verified at
`design.md:256` and `:277`.

## Predictions to score

| seat | prediction | scored at |
|---|---|---|
| compiler-engineer | `grep -c "^extern " selfhost/**/*.hero` still answers 4, the diff adds ≤160 new `.hero` lines, and `seed/README.md`'s clang line is unchanged; falsified if libcurl was linked instead of `curl` being run | the milestone that lands `--refresh` |
| spec-warden | at M-selfhost-fixpoint close, `count_tokens` for `claude-opus-5` reads ≤5400 and its ratio to the vendored maximum stays in [1.26, 1.29]; the first failure returns max-over-set to the ballot, the second says the offline cross-check is not a staleness detector | M-selfhost-fixpoint |
| historian | an input exists on which `tiktoken` 0.5.2 and 0.14.0 return different counts for `cl100k_base`, in the whitespace class; settling it costs two venvs and a loop | any milestone; it is two commands |
| coordinator, from the sitting | the floor claim is withdrawn from every live document, and no document says the spec lives in a band | this sitting's landing commit |

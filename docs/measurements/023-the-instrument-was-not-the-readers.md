# 023 — The instrument was not the reader's

**The spec has never been inside its own ceiling on any real Claude tokeniser.**
Measured 2026-09-09 against `POST /v1/messages/count_tokens`, the endpoint that
answers with the tokeniser a model actually uses, reached with a key the author
put in the gitignored `.env` the same hour and fenced to that one endpoint in
`.claude/hooks/guard_bash.py`.

**Whose finding this is.** The author's, in full: the idea, the key and the
question that produced it, *"ho una api key pronta per misurare meglio i token
con le api"*. The coordinator had spent the evening computing ceilings to the
token on the number this measurement falsifies.

## The method, and its one offset measured rather than assumed

`heroes measure` reads two VENDORED instruments and takes the larger:
`claude-legacy` (64995 ranks) and `cl100k_base` (100256 ranks). The endpoint
takes a model id and a `messages` array; its `input_tokens` covers the whole
request, so the message structure is part of the number.

**The structure is 6 tokens**, measured with two probes rather than inferred
from one: a single-character content answers **7**, and a five-token content
(`x x x x x`) answers **11**. Both give 6. Every file figure below is the
endpoint's answer less 6.

## What the spec measures

| instrument | the spec | against the hard 4096 |
|---|---|---|
| `claude-legacy`, vendored | 3917 | inside |
| `cl100k_base`, vendored, **the binding number** | 3995 | inside, 40 free |
| Haiku 4.5 and Opus 4.6, real | **4128** | **over by 32** |
| Opus 5 and Sonnet 5, real | **5094** | **over by 998** |

Opus 5 and Sonnet 5 agree exactly (5100 raw); Haiku 4.5 and Opus 4.6 agree
exactly (4134 raw). So this is not noise between models, it is **two tokeniser
generations**, and the gap between them is **966 tokens**.

**The `spread` this project has been treating as its error bar is 78.** It is the
disagreement between the two vendored instruments. The disagreement that matters
is 12.4 times larger, and it was never in the number.

**And the cause is documented rather than mysterious.** `cl100k_base` is
**OpenAI's** tokeniser. Anthropic's own documentation says not to use it for
Claude and that it undercounts Claude tokens by 15-20% on typical text and by
much more on code. Measured here: **+27.5%** on the spec and **+33.4%** on
`CLAUDE.md`, both above the typical-text band, which is what a document dense
with code spans and identifiers predicts.

## The contract is over its pin too

| | vendored (`heroes measure`) | real | its pin |
|---|---|---|---|
| `CLAUDE.md` | 5389 | **7190** | `CONTRACT_CEILING` **5500** |

Over by **1690**, 30.7%.

## What this falsifies

- **The 40 free tokens never existed.** The spec was over its ceiling on the
  older generation and 998 over on the current one.
- **Every ceiling number computed on 2026-09-08 is void as arithmetic**: the
  4224 the author confirmed, the 4237 the coordinator recommended, and the 4253
  of `docs/panel/121-the-brace-was-already-taken.md` R7.
- **The deltas were understated by the same factor**, and re-measured on the
  real instrument the ranking survives: the `f"…"` clause is **+151** where
  `docs/measurements/022-the-narrow-rule-costs-more-than-the-wide-one.md` read
  +118, and the `\{e}` clause is **+126** where it read +99. The cheaper
  spelling is still the cheaper one, now by 25 tokens instead of 19.
- **`design.md` §1.6's own sentence was right and load-bearing.** It says the
  reader's real tokeniser is unpublished, so the spread applies to the ceiling
  too. It is published now, and the spread it warned about was an order of
  magnitude short.

## The byte alternative, priced because the author proposed it

*"un limite di 6 KB … così è più facile da dire anche sul sito come claim, e non
ci vincola più a scelte troppo economiche"* (author, 2026-09-09).

| | bytes |
|---|---|
| `spec/heroes-spec.md` today | **12,862** |
| `CLAUDE.md` today | 19,835 |

So **6 KB (6144) would be a cut of 6718 bytes**, more than half the document,
which is the opposite of the proposal's intent. Candidate ceilings against
today's document:

| ceiling | against 12,862 |
|---|---|
| 12 KB = 12,288 | a cut of 574 |
| 13 KB = 13,312 | 450 bytes, about 179 real tokens |
| 14 KB = 14,336 | 1,474 bytes, about 585 real tokens |
| **16 KB = 16,384** | **3,522 bytes, about 1,398 real tokens** |
| 20 KB = 20,480 | 7,618 bytes, about 3,023 real tokens |

**The interpolation clause in bytes**: **+345** for the `\{e}` spelling, **+420**
for `f"…"`. At a 16 KB ceiling that is 10% of the headroom and 2.1% of the
budget, against the 19-token argument it was on 2026-09-08.

**What a byte ceiling buys, and the first reason is what happened here.** The
project's number moved 966 tokens without a character of the document changing,
because the instrument changed and the instrument belongs to somebody else.
`wc -c` cannot do that: no key, no network, no vendored ranks, and the same
answer in ten years. It is verifiable by a reader in one command, which is what
makes it a claim rather than an assertion, and it removes the offline and CI
problem that any network-dependent gate has.

**What it costs.** §1.6's argument is about what the MODEL pays, and that is
tokens. And bytes are lenient exactly where tokens are strict: the spec is 2.52
bytes per real token and `CLAUDE.md` 2.76, so the denser the code spans, the
more tokens a byte buys, and a byte budget under-charges the prose the spec is
made of.

**The route was not taken, and the author is the one who closed it**: *"ho
confuso bytes con token"*. The premise was corrected rather than the number, and
the unit stayed. The byte figures are kept here because a priced route that was
considered and declined is worth more in the record than in nobody's memory, and
because the bytes-per-token ratios above are the standing check on any future
proposal to switch units.

## What was decided, and the floor nobody had looked for

**The ceiling is 6144 TOKENS** (author decision 2026-09-09). Against today's
real **5094** that is **1050** free, or **989** net of the 60-token FFI floor
and the excluded bound, and the interpolation clause at +126 real tokens is
12.7% of it. The cure for a number nobody could trust is to name the instrument,
not to change the unit, so **the ceiling carries its tokeniser and its date** or
this measurement happens again the next time a vendor ships a generation.

**And the budget has a FLOOR that comes from the world.** Anthropic's minimum
cacheable prefix is model-dependent and **not monotonic across generations**:

| model | minimum cacheable prefix |
|---|---:|
| Opus 5, Fable 5, Mythos 5, Fable 5.1, Mythos 5.1 | 512 |
| Opus 4.8, Sonnet 5, Sonnet 4.6, Sonnet 4.5, Opus 4.1, Opus 4, Sonnet 4 | 1024 |
| Opus 4.7, Mythos Preview, Haiku 3.5 | 2048 |
| **Opus 4.6, Opus 4.5, Haiku 4.5** | **4096** |

Below the minimum a prompt **silently does not cache** — no error, just
`cache_creation_input_tokens: 0` — and a cache read costs about a tenth of base
input price.

**§1.6 says the spec is not documentation, it is the prompt.** So on Opus 4.6,
Opus 4.5 and Haiku 4.5, 4096 is not a ceiling at all: it is the line under which
this project's own prompt stops being cacheable. The spec measures **4128** on
Haiku 4.5, **32 tokens above that floor**, so it caches there today — and had
the old ceiling been honoured on the real instrument, keeping the spec under
4096, it would have cost exactly that.

**So the spec lives in a band rather than under a number**: at or above 4096 to
stay cacheable where the floor is highest, at or below 6144 to stay
comprehensible. Both ends are measured or sourced. 5000 and 10000, considered,
are neither: they are round decimals, and the other numbers Anthropic states in
this area are far away — a 150K default compaction trigger and a 20,000-token
minimum task budget.

## What is unmeasured

- **How the 6144 is enforced.** The number is decided; the mechanism is not.
  design.md §1.6, `heroes measure`'s answer, `SPEC_TOKENS` and `CEILING` in
  `tests/harness/suite_spec.hero`, the compiler's own copy in
  `selfhost/cli/measure.hero` and therefore the seed, three sentences in
  `README.md`, both home pages and `site/src/lib/claims.ts` all carry the old
  number or the old unit. That is a panel's shape and the author's
  ratification, not this file's.
- **Every other Claude generation.** Four model ids were counted. A fifth could
  disagree with both pairs, which is the argument for recording the model id
  beside any number rather than trusting a family name.
- **`heroes measure` still answers from the vendored ranks**, and nothing in the
  compiler reads the endpoint. What it would take is a `heroes` flag that
  REFRESHES a recorded number, never a check that needs the network to go green
  (`.env.example` § Anthropic carries the shape).
- **What the site says.** `site/src/lib/claims.ts` reads `constant CEILING` out
  of `tests/harness/suite_spec.hero` and checks that both home pages state it,
  so a change of unit moves that machinery and two published sentences with it.

## Corrected 2026-09-09 by panel 123, two errors in this file's own method

**The request offset is model-dependent and this file states it as one
constant.** Probed twice per model over six ids by the spec-warden at that
sitting: **6** on `claude-opus-5`, `claude-sonnet-5` and `claude-opus-4-8`;
**7** on `claude-opus-4-6` and `claude-haiku-4-5`; **11** on
`claude-opus-4-7`. So the figure above for the older pair is **4127** and not
4128 — which panel 122 had already caught once — and the inter-generation gap
is **967** rather than 966. And Opus 4.7 counts the same CONTENT as Opus 5 while
wrapping it differently, so *newest* and *largest* are independent axes: a
maximum over a set of model ids is not a property of the document.

**THE FLOOR SECTION ABOVE IS WITHDRAWN.** This file recorded Anthropic's minimum
cacheable prefix and the coordinator built a *band* out of it — 4096 to 6144,
"both ends measured or sourced" — and put it to the author in those words.
Panel 123's spec-warden refused it on four grounds, and each is enough:

- it is a property of a request **prefix**, not of a file, and `CLAUDE.md` alone
  measures **5547** on Haiku 4.5, clearing 4096 by 1451 before one spec token is
  loaded — while CLAUDE.md §1 has the spec read inside a session that has
  already loaded the contract, so on the only workload this project runs the
  threshold is cleared before the spec is opened;
- a floor makes a **removal a violation** and turns §1.6's forcing function into
  a band to centre, paying in the factor §1.2 says dominates;
- a vendor price list for three legacy models is a premise about the world that
  expires silently **and upward-invisibly** (`.claude/rules/module-shape.md`);
- the margin is **31** content tokens, inside the 78-token spread this project
  was calling its error bar — indistinguishable from noise at its own precision.

**And the numbers were never measured here.** The seat that would have verified
them was refused by this session's own guard — `/v1/models` answered *"the
Anthropic key is for `/v1/messages/count_tokens` and nothing else"* — so they
are vendor-sourced and enter the record as a question rather than a premise
(CLAUDE.md § RUN IT). What may be claimed: the minimum exists, is
model-dependent and non-monotonic, is 4096 on three named legacy ids, applies to
a cached prefix and not to a file, and is already cleared by `CLAUDE.md` alone.
What may not: that the spec lives in a band, that 4096 is a floor, or that both
ends are measured.

**And the 27.5% is not corroborated by Anthropic's ~30%.** The vendor's figure
is Claude-pre-4.7 against Claude-4.7-and-later; this file's is `cl100k_base`
against Claude. Two different comparisons of similar magnitude, and the
historian at panel 123 insisted they not be written as agreeing with each other,
because that is how a false citation is born three documents later. The figure
that belongs beside the vendor's 30% is the **967**-token inter-generation gap.

**One thing this file said that panel 123 strengthened rather than corrected.**
Vendoring the rank files pinned *the half that never changed*: `expected_hash`
content-addresses the ranks, while `pat_str` — the pre-tokenizer regex that
decides where text splits — lives in library **code** and was edited twice under
the fixed name `cl100k_base`, in `tiktoken` PRs #234 and #258, merged on a
round-trip test with no proof of output equivalence. So *"we vendor it, so it is
reproducible"* was never true of the part that moved.

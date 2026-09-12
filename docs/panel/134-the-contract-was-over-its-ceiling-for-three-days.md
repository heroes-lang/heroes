# Panel 134 — the contract was 1806 tokens over its ceiling, and the instrument printed nine

**Sat** 2026-09-12 · **milestone** M-declared-thresholds · **status**
`provisional — author ratification pending`

**Lane: three seats, not five, and what that gave up.** `llm-ergonomist` receives
only `spec/heroes-spec.md` and this proposal changes not one token of it;
`ffi-pragmatist` compiles the C a proposal implies and this one implies none. The
seats whose input differs are `spec-warden` (§1.6's instrument and the budget),
`compiler-engineer` (the cost in `selfhost/`) and `historian` (precedent for
pinning a remotely-taken measurement). CLAUDE.md §4's *"choosing only the seats
whose input differs"* (CL-023). **What it gave up**: no seat read the contract as
a READER of it, which is the one question a fifth seat could have been given —
whether a 341-line contract is obeyed better than a 1247-line one. That question
is open and the historian's evidence below bears on it.

## The proposal, as it went out

CLAUDE.md is judged on the maximum over two VENDORED tokeniser tables, the larger
of which is `cl100k_base`, OpenAI's. `spec/heroes-spec.md` is judged on
`claude-opus-5` through `POST /v1/messages/count_tokens`. Move the contract onto
the same instrument, raise `CONTRACT_CEILING` to 8192 (author instruction
2026-09-12), and land with it the delta gate design.md §1.6 keeps queued.

## The measurement that decided the sitting

**`CLAUDE.md` is 7306 real tokens** (spec-warden, this session: 7312 raw less the
measured two-probe offset of 6). The method validated itself by reproducing the
spec's pin exactly — 7531, the number already in the tree.

**So the contract has been 1806 tokens over its stated ceiling since 2026-09-09,
while `heroes measure CLAUDE.md` printed `Headroom: 9`.** That is §1.6's own
history happening a second time, one document across: the spec was 998 over a
line it was told it was 40 under. The trace was already in the tree —
`docs/measurements/023` recorded CLAUDE.md at 5389 vendored / **7190 real** on
2026-09-09 — and nothing read it.

**The margin that was spent.** `CONTRACT_CEILING` was set at `5127 + 373` on
2026-09-07. Measured per commit: **+364 vendored in five days over 23 commits**,
97.6% of the margin, with three commits carrying +347 of it.

## Verdicts

| seat | verdict | rests on | the number it brought | prediction | condition |
|---|---|---|---|---|---|
| `spec-warden` | adopt-with-condition (Q1, Q2) · adopt (Q3) | CLAUDE.md § *This file's own cost is measured* (CL-069); §1.6 for the instrument principle only | CLAUDE.md **7306 real**; ratio 1.3306 measured, not inferred; +364 vendored in 5 days; spec delta **zero** | at M-deferral-ledger's close: CLAUDE.md ≤ 7700 real and ≤ 5790 vendored; the +50 gate fires on ≤ 2 of the next 20 contract commits; the spec still reads 7531/5655 | `--refresh` must work for the contract, a staleness detector must exist, the gate lands with the raise, and the stale *"322 lines"* is fixed. **Vetoes if the refresher or the gate is deferred**, or if the contract measures above ~7800 real at landing |
| `compiler-engineer` | adopt-with-condition · **veto on the digest** | design.md §1.6 does not reach the contract | ~170 lines over six files, none in checker/lowering/emitter; `measure.hero` **38 `code_lines` from the 300 gate** and not in `DECIDED`; CLAUDE.md **34 commits in 7 days** against the spec's 17 | at the landing milestone `code_lines(selfhost/cli/measure.hero) > 300`, forcing a split or a 19th `DECIDED` row, which turns `assert DECIDED.len() == 18` red. Instrument: `heroes run tests/harness/main.hero -- ./heroes layout` | measure the contract BEFORE setting the ceiling; the digest veto lifts only if someone shows a keyless offline route back to green; the layout prediction is pre-empted by splitting `measure.hero` at a named seam in the same commit |
| `historian` (advisory) | **object**, narrowly — against Q1 as stated and against Q2; **for** Q3 | sourced precedent only | Anthropic's own docs: `count_tokens` **"is an estimate"**, and Claude 4.7+ tokenisers produce **~30% more tokens** on the same text. Chromium runs a 16 KiB per-CL gate **and grows ~100 KB/week** | — (no veto) | one internally-owned, instruction-raisable size budget that held its level three years without a ratchet. It searched and found none |

## Where they disagree, unsmoothed

**The digest.** The warden makes a contract digest a condition of adopting;
the engineer vetoes it. The engineer measured why: CLAUDE.md takes **34 commits a
week** to the spec's 17, is amended by author instruction with no panel, and a
digest over it would produce ~34 withheld verdicts a week, each clearable only by
a network refresh — **and not clearable at all on a fork PR**, which is panel 123
R1's own refusal returning through the window.

**Resolved by taking what both are actually asking for.** The warden wants a
stale number to be LOUD; the engineer wants the detector to be keyless and
offline. `SPEC_TOKENS`'s vendored pin is both, and `suite_spec.hero:105-108`
already certifies it *"always answered correctly"* at the job of *did this
document move and nobody say so*. So: **one vendored pin, `CONTRACT_TOKENS`,
doing three jobs** — the staleness detector, the previous level the delta gate
subtracts from, and the movement detector the contract never had. The spec keeps
its digest, where it costs nothing.

**The historian's Q1 objection rests on a misreading, and it is recorded rather
than dropped.** It read the proposal as letting the network judge at check time
and answered with the go.sum pattern: *the network fetches, a deterministic local
function judges, the judgment is pinned in-repo*. That is **already what this
repository does** — `REAL_TOKENS` is pinned, refreshed by hand through
`heroes measure --refresh`, and panel 123 R1 forbids a key-gated gate precisely
because GitHub passes no secret to a fork's PR. Its objection therefore converts
to approval on the design as it stands. **What survives the misreading is the
part nobody had**, and it is the sitting's second real finding.

## The two facts the historian brought that changed the resolution

1. **Anthropic's own documentation calls `count_tokens` an estimate**, and states
   that Claude 4.7 and later tokenise the same text **~30% higher**. Two
   consequences. A ceiling with nine tokens of room is a coin toss against a
   non-deterministic oracle — which is what the contract has had. And 7306 × 1.3
   is **9498**, so a model bump would breach 8192 without a byte changing. The
   repository is already protected, and the protection is now load-bearing rather
   than incidental: **one pinned model id binds and moves only by author
   decision** (panel 123 R3). This resolution restates that as the reason the
   contract's ceiling is safe, not merely as a convention.
2. **No internally-owned, raisable size budget in the sourced record ever held
   its level.** Angular's answer to a breach became a vendor support page titled
   *How to Increase Budget Size*; webpack ships `maxAssetSize` as a warning by
   default; the kernel disabled `CONFIG_FRAME_WARN` under KASAN rather than
   defend it; Chromium has no absolute limit at all. The one mechanism that held
   is the **ratchet** — Notion's `eslint-seatbelt`, where the allowed count falls
   automatically and cannot rise in a green PR, because the human is removed from
   the raise. And on the gate itself, the honest framing: Chromium runs a 16 KiB
   per-CL gate with a mandatory `Binary-Size:` footer and grew ~100 KB/week
   anyway. **A delta gate bounds attribution, not level.**

## The resolution adopted, provisionally

The most robust and complete one, never the cheapest and never a compromise
(CLAUDE.md §4). Six parts, all landing together:

**R1. The contract moves to the real instrument, and this is an EXTENSION, not a
repair.** Both technical seats found it independently: §1.6 governs *"the entire
language specification"* and `grep -n "CLAUDE.md" docs/design/design.md` returns
fifteen hits, none a measurement rule. The contract's ceiling is CLAUDE.md
§ *This file's own cost is measured, not estimated*. The brief that convened this
sitting called it a repair of an inconsistency; there was no inconsistency, and
the change has to be argued on its own merits — which the 7306 does.

**R2. `CONTRACT_CEILING` becomes 8192, written as `7306 measured + 886 of room`,
never as a round figure.** The author's number, and the engineer's form, because
the engineer is right that CLAUDE.md forbids a round figure in its own words and
right that a number copied from another document invites a later tidy-up to
"unify" them. Two constants, never one shared, and the comment says the
coincidence with the spec's 8192 is a coincidence. **The room is stated with its
measured duration rather than implied**: 886 real tokens is about nine days at
the five-day rate and about twenty-three at the three-day real rate. That is
roughly one milestone, which is why R3 is not optional.

**R3. The delta gate lands in the same commit: both documents, +50, VENDORED.**
The warden's shape. Vendored because a key-gated check is structurally red on
every fork PR forever. One threshold for both documents, equal to the figure
§1.6 already queued, so no second number is born. Measured: +50 fires on 2 of the
last 22 contract commits, and those two carry +255 of the net +364. **It is
adopted knowing what it buys**, in the historian's words: every increase gets a
name and a written reason, and the total is not thereby bounded.

**R4. The digest is REFUSED for the contract**, and replaced by `CONTRACT_TOKENS`,
a vendored pin serving as staleness detector, gate baseline and movement
detector at once. The engineer's veto stands on its measurement.

**R5. `contract()` must read the `real` row, and a test must prove it.** The
engineer found the defect that would have made this whole change a no-op:
`maximum_of` accepts exit 0 and exit 1, and `measure` prints the `maximum` row
before the staleness return, so `contract()` would go on comparing a VENDORED
number to a REAL ceiling — green forever, on the scale this sitting exists to
abolish. A stub compiler printing only a `maximum` row must make `spec/contract`
**red**.

**R6. Two false sentences in the tree are corrected while they are open.** The
`CONTRACT_CEILING` comment says *"It is 322 lines now"*; measured today, **341**.
And it says *"Claude Code's own documentation asks for under 200 lines"* — the
historian fetched that page and **the figure is not there**; it is third-party
blog content. What the page does say, verbatim, is *"Bloated CLAUDE.md files
cause Claude to ignore your actual instructions"*, and the measured adjacent
evidence is Chroma's Context Rot (18 frontier models, reliability falling with
input length) and Liu et al.'s Lost in the Middle. The claim gets weaker and
true instead of stronger and unsourced.

**What conservative would have been, recorded so the author can choose it**
(CLAUDE.md §4): move the instrument, set the ceiling to `7306 + 373` — the same
margin 2026-09-07 chose — and land the delta gate. That is **7679**, it refuses
the author's 8192, and it would put the contract about four days from its wall at
the measured rate. It is recorded because the historian's Q2 finding argues for
it: no raisable budget in the record ever held. It is not adopted because the
author has instructed 8192 and because four days of room is a threshold that
cannot be obeyed, which is the defect this milestone exists to remove.

## Author's verdict

**Pending.** The item is `panel 134` in `docs/work/DECIDE.md`.

**What the author has already settled**, before the sitting ran and recorded here
rather than staged as an open question: the ceiling is **8192**, instructed
2026-09-12. What the sitting adds to that instruction is the measurement it was
given without — 7306 — and three things the author has not yet seen.

**What a yes would settle**: that 8192 is written as `7306 + 886` rather than as
a round figure copied from the spec; that the delta gate at +50 vendored lands in
the same commit rather than after it, which is the condition on which the
warden's adoption rests and without which it vetoes; and that the contract gets
no digest, the engineer's veto standing on 34 commits a week.

**What a yes would NOT settle**, and each is live work rather than a formality:
whether `measure.hero` splits at a named seam or takes a 19th `DECIDED` row when
it crosses 300; what the contract's real count is on the day the ceiling is
written, since 7306 is today's and the warden vetoes above ~7800; and whether the
named removal it costed — CLAUDE.md §5 and §6, ~238 real tokens — is actually
spent, which is the author's call because §5 is theirs.

**What a no would compel.** If the author refuses 8192 the conservative number is
already costed: `7306 + 373` is **7679**, about four days of room at the measured
rate. If the author refuses the move to the real instrument altogether, the
contract stays 1806 tokens over a ceiling nothing enforces, and the honest repair
is then a removal rather than a number — §5 and §6 return ~238 real, which is
about an eighth of the breach.

## Predictions to score

| prediction | instrument | scored at |
|---|---|---|
| CLAUDE.md ≤ 7700 real and ≤ 5790 vendored | `heroes measure CLAUDE.md` and `count_tokens` on `claude-opus-5` | M-deferral-ledger close |
| the +50 gate fires on ≤ 2 of the next 20 commits touching CLAUDE.md | the gate itself | M-deferral-ledger close |
| the spec still reads 7531 real / 5655 vendored at the landing commit | `heroes measure` | this milestone's landing commit |
| `code_lines(selfhost/cli/measure.hero) > 300`, forcing a split or a 19th `DECIDED` row | `heroes run tests/harness/main.hero -- ./heroes layout` | the landing commit |

## The payment

The raise is not a spec amendment and spends **zero spec tokens** — measured,
7531 → 7531 real and 5655 → 5655 vendored. It is nonetheless owed a removal by
the discipline it is extending, and the warden costed one: CLAUDE.md §5 and §6
are **179 vendored / ~238 real**, and §5 says in its own words that it
*"constrains no new code"*. Collapsed to two pointer lines citing CL-021 and
CL-015 that returns ~150 vendored / ~200 real, with no section number changed
(CL-069).

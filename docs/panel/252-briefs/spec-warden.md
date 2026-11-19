# Panel 252, spec-warden

Read `00-shared.md` first, whole: the frozen commit (`181555e9a32d0b18878f9f10e48d2eadd939c285`), the five
questions with their arms and counts, and the rules. You judge the indicator (design.md §1.6, `grep -n
'^### 1.6' docs/design.md`: line 253) and Principle 0's burden (CLAUDE.md § 2): whether the specification
must say anything for each question, where, at what price, and what pays for it. Veto on a budget breach,
or where Principle 0's burden is unmet (`.claude/agents/spec-warden.md`).

**Your folder** is `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/rebel-scratch/252-spec-warden/`,
made by you as `00-shared.md` § Where each seat works shows; binaries `sw-...`. **Your report** is
`.claude/worktrees/rebel-round/docs/panel/252-reports/spec-warden.md`, written as you go, in
`.claude/agents/spec-warden.md:62-70`'s structure (`verdict`, with `provisional` where a count is vendored,
`section`, `spec_token_delta`, `removal`, `needed_for_self_hosting`, `argument` of at most 120 words,
`prediction`, `condition`), **one block per question**. Read `.claude/rules/spec-shape.md` in your copy. No
paid run but the refreshes named below, no credential printed, no `heroes test`.

## Your predictions come first, and the runner waits for them

**Register your predictions in your report, under a heading that reads exactly `## Registered
predictions`, before anything else you run takes long**: the blind seat's runner
(`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/rebel-scratch/252-llm-ergonomist/run.sh`)
**refuses to spend until that heading stands in your report with at least one non-empty line under it**
(before the next `## ` heading). When it starts it **copies your report as it stands** into its area,
`warden-at-start.md`, and writes the **copy's** sha256 and the time into its `run.log` before its first paid
session (`tools/gate.py`, repaired after panel 253's critic's finding 6: a hash of a file you keep writing
proves nothing later). Panel 251's were written 13 seconds after its runner started, by its coordinator's
launch order, and admitted only as a departure (`docs/panel/251-reports/critic-reports.md:145-186`;
`docs/panel/251-*.md:253-258`). **Write the heading when the predictions are whole, and the predictions
under it**: what is scored is what the copy holds. Word each prediction
so the blind seat's counted rules score it (`llm-ergonomist.md` § The decision rules), and say which arm
each pays for.

## The count before and after, as measured

`spec/heroes-spec.md` did not move since panel 251's commit (`git diff --quiet 3a5ee2691 181555e9a --
spec/heroes-spec.md`, exit 0): 492 lines, 29,407 bytes, sha256
`1d532d514831aa70234ef0eb8ad77850ebd4c2e390234f22e973a363ed757001`. By the writer's stage 2 (`sources
23c7d2a626b29fd5`), run from `bw/tree/` (`bw/` is
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/rebel-scratch/252-briefwriter/`):
`./heroes-next measure spec/heroes-spec.md` prints `claude-legacy 8551`, `cl100k_base 8702`, `maximum
8702`, `real 11384` (*claude-opus-5, 2026-11-15*, the pin, **carried**: no refresh ran) and *"Headroom:
5000 against the 16384 ceiling — but the FFI floor mortgages 60 of it (panel 030 R3), so what is measured
against the ceiling is 11444"*. The arms, by `bw/tools/make_arms.py` into `bw/arms/` and `./heroes-next
measure ../arms/spec-<arm>.md` (`bw/measure-arms.txt`), are `00-shared.md`'s table, **repaired after the
critic's first reading**: `q1-c` +26, `q1-a` +54 (added), `q2-c` +14 (repaired), `q3-r` +63 (repaired),
`q4-r` +56, `q4-r2` +52 (added), `q4-c` +17, `q5-r` +41 (repaired), `q5-c` +35, `q5-p` +1, `q5-u` +14
(added), `proposal` (`q3-r`, `q4-r`, `q5-r`) +160, **vendored maxima, lower bounds**. Each arm's diff is
`bw/arms/<arm>.diff`; the first arms are kept in `bw/arms.v1/`.

**The real counts are yours to take: at most thirteen refreshes**, one per arm (`now` re-measured rather
than carried, then the twelve), **at 0 USD**: *"Token counting is **free to use** but subject to requests
per minute rate limits based on your usage tier"* (`https://platform.claude.com/docs/en/build-with-claude/token-counting.md`,
§ *Pricing and rate limits*, read 2026-11-19 by the writer; the coordinator's decision (d), after the
critic's finding 6 found the *"free"* carried from 2026-11-15). `--refresh` refuses
every path but `spec/heroes-spec.md` and `CLAUDE.md`, so each arm is copied over your copy's
`spec/heroes-spec.md`, refreshed, and reverted (`cmp` against `bw/arms/spec-now.md` after each). **Panel
251's spec-warden ran `--refresh` in a copy with no `.git`, three requests each** (two probes and the
document, `selfhost/cli/refresh.hero`; `docs/panel/251-reports/spec-warden.md:33-40`, carried). The key:
`set -a; . /Users/joseph/Temp/heroes/heroes-lang/.env; set +a` in the one shell that refreshes, then
`echo ${#ANTHROPIC_API_KEY}`, **its length and never its value**. Log each refresh in your report: the
time, the arm, the constants printed. **Nothing you print is pasted into the tree.** An arm you write of
your own is vendored, a lower bound, in those words.

## What you are asked

1. **Q1, the private module and its face.** The rule (`module_cycle`) is design.md §4.1's
   (`docs/design.md:942-943`) and stands in no sentence of the spec, by panel 031's R6, *"with no spec
   sentence"* (`docs/panel/031-modules-the-namespace.md:193`). Q1-R lifts it at **0 tokens**: is the spec's
   silence then simply true, and does design.md §4.1 owe its sentence's removal in the same change? Q1-C's
   sentence (+26, `bw/arms/q1-c.diff`): is a refusal a reader meets at the first private module worth its
   tokens, against R6's ruling that the diagnostic carries it (`00-shared.md` § Question 1: the message
   and its note, whose route ends in `private_module` for a face)? And Q1-R2 (a ring only through a face and
   its `internal/`): what sentence would it owe, beside § 1's `internal` sentence (lines 36-37)? **And Q1-A**
   (arm `bw/arms/q1-a.diff`, +54, the critic's finding 7): an alias in the face names the private module's
   type as the face's own; its record is panel 031's R4, *"Every module you name needs its own `use`" ...
   "There is no re-export"* (`docs/panel/031-modules-the-namespace.md:178-182`), and **panel 135**, which
   deferred the `alias` form again with a settled shape and three return conditions
   (`docs/panel/135-*.md:124-165`, ratified `:217-221`): does Q1 meet one of them, condition (b) being *"a
   commit in `selfhost/` whose diff changes one structural type spelling at **8 or more sites**"*? A form,
   so Principle 0's ticket; what pays for it? **Removing a
   diagnostic class** (`module_cycle`): what `.claude/rules/diagnostics-and-goldens.md` and open task 2266
   (*"The codes a diagnostic can carry, listed and held by the net"*, `gh issue view 2266`) ask of it.
2. **Q2, a literal among them.** Does § 9's *"else from a generic call or a literal among them (section 2);
   `ok(...)`, `[]` and a case name give none"* (lines 305-308) say that `"a"` in `{"a": []}` gives `K`? If
   it does, the compiler has the bug (CLAUDE.md § 12) and Q2-R's 0 tokens stand; if it does not, is Q2-C
   (+14, **repaired** to *"nor does a literal whose first element or value gives none"*, the compiler's
   behaviour on 20 programs, an order-dependence inside one literal; its first wording was false of the
   blind task's own line 6, the critic's finding 1) the sentence a specification should state, and why does § 9's list name `[]` and not `{}`, which refuses itself the same way
   (`bw/q2/k-emptymap-value.out`; § 10's *"an empty one needs an annotation"*, line 320)?
3. **Q3, a record with a type parameter.** Q3-R (+63, its example repaired to `record Pair<A, B>`, never the
   blind task's answer, the critic's finding 2) changes § 9's generics sentence (line 303), § 3's
   `Prefix` (106-107) and § 4's `Declaration` (138-141): **Principle 0's burden** (the compiler-engineer's
   compiler-need answer; the blind seat's task 3), the payment, and the instruments it touches: the
   `grammar` suite (productions against `heroes grammar`), the `named` and `rejected` checks on
   `Pair<i64, str>` and `record Pair<A, B>` in a code span (`tests/harness/suite_spec.hero`), the type table's
   first column (`site/src/lib/tables.ts`). design.md §4.12 calls `Stack<T>` *"perfectly comprehensible to
   an LLM"* (`docs/design.md:1972-1973`) and rules *"Generics on functions only, not on types"* (`:2020`):
   which binds a sitting that would amend §4.12?
4. **Q4, a byte becoming text.** Q4-R (+56) states one rule in two sections (§ 2's hole, § 11's `print`):
   is that one rule with two homes (`.claude/rules/spec-shape.md` § Where a rule lives), and is the home
   § 2, § 10 (where `s[i]` yields a `u8`, line 318) or § 11? Is a new diagnostic class owed? Q4-C (+17): does
   a sentence that describes a trap pay, where the thesis asks the trap be a compile error? And the shape
   beside, a character literal defaulting to `u8` (§ 2, line 68): a sentence, unpriced. **And Q4-R2** (arm
   `bw/arms/q4-r2.diff`, +52, the critic's finding 10): one sentence in § 2 refusing an index into a `str` in
   a hole or a `print`, where Q4-R refuses every `u8`. **The record the first brief missed**: panel 061
   measured *"`b: u8 @ 'a'` prints 97"* and ruled on the spec's false *"Character literals are `i64`"*, not
   on the printing (`docs/panel/061-the-fields-nobody-named.md:279-284`).
5. **Q5, an exponent printed and refused.** § 11's *"reads back as the same value"* (lines 350-351) names
   no reader, and the language's own reader refuses `1e-06` (`bw/q5/lit-e-minus.out`): **is the sentence
   false for a reader of the spec** (panel 035 said of an earlier sentence of this paragraph, *"A false
   sentence in a 2422-token prompt is the worst possible spend"*, `docs/panel/035-*.md:59-62`)? § 2 says
   nothing of a float literal's form (lines 63-79): a ruling (panel 035's R6 at 175-179) or an omission?
   Price and judge Q5-R (+41, **repaired**: its first wording, *"digits on both sides of its point"*, gave
   `1e-06`, which has no point, as a float, the critic's finding 3), Q5-C (+35), Q5-P (+1) and Q5-U (+14,
   added: a float takes `_` today, `0.000_000_001` built and printed `1e-09`, though § 2 line 66 grants it to
   an `i64` alone, the critic's finding 9), each against what it removes. **Q5-R's blind rule now has a
   count** (`llm-ergonomist.md` § The decision rules: `now`'s right answers once regraded exceed its right
   answers under the frozen stage 2 by at least 3): a prediction you register for Q5-R is scored on it.
6. **The payment** (design.md §1.6, `:324-338`): every addition owes a named removal or a registered
   prediction naming an instrument that exists the day it is registered. The blind seat's counted rules
   are such an instrument for this sitting. And the consolidation block's *"at most **1,500** real tokens
   before the thesis harness has run"* (`docs/design.md:290-293`): does it still bind, and what do this
   sitting's arms spend against it?

## What you are asked to say

Your definition's structure per question; the measured deltas, real where refreshed and vendored where
not, each with its command; the sentence you would adopt word for word with its home, or none; the
predictions you register, under `## Registered predictions`, and the counted rule that scores each; and
every premise of these briefs you found false.

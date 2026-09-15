# Panel 154 — report of the spec-warden

**Verdicts.** **A — object.** **B — adopt.** **C — refuse, with veto.** **D — refuse, with veto.** **E — refuse, with veto** (as the resolution of defect 045; object if added on top of B). **F (sixth route, mine) — adopt: route B merged rather than appended, paid by R4.**

## The ceiling, grepped this sitting

`docs/design/design.md:255` reads **10240 tokens, measured by `claude-opus-5` through `POST /v1/messages/count_tokens`**. Not taken from the brief.

Base, re-measured in my copy, not carried: `./heroes measure spec/heroes-spec.md --refresh` → **7974 real / 5989 vendored (cl100k), 5863 legacy, digest `21a9dc541cfa2fa8`, 2266 free, 8034 against the ceiling once §4.19's 60-token FFI floor is deducted.** The brief's base is confirmed.

## Every route priced as spec text, measured

Each draft written into `spec/heroes-spec.md` in the copy, measured, base restored between drafts (`git diff --stat` in the copy is empty now; `/Users/joseph/Temp/heroes-lang` was never written to).

| route | real | Δ real | vendored | Δ vendored |
|---|---|---|---|---|
| base | 7974 | — | 5989 | — |
| **A**, silent (guard, no sentence) | 7974 | **0** | 5989 | **0** |
| **A**, stated | 7997 | +23 | 6007 | +18 |
| **A**, tightest wording | 7985 | **+11** | 5999 | +10 |
| **B** `accepts_null`, first draft | 8032 | +58 | 6033 | +44 |
| **B**, merged into the `nullptr` sentence | 8010 | **+36** | 6016 | +27 |
| **B** merged **+ R4** | 8001 | **+27** | 6012 | +23 |
| **B** merged **+ R4 + R3** | 7986 | **+12** | 6003 | +14 |
| **C** `rejects_null` | 8031 | +57 | 6033 | +44 |
| **D** marks decide | 8022 | +48 | 6026 | +37 |
| **E** static refusal | 7998 | +24 | 6005 | +16 |
| **R4** alone | 7965 | **−9** | 5985 | **−4** |
| **R3** alone | 7959 | **−15** | 5980 | **−9** |

R4 and R3 reproduce panel 153's figures exactly (−9/−4 and −15 real), independently measured. **No route breaches 10240; I have no budget veto.** Every route also clears `DELTA_GATE` 50 vendored.

**Route A does not cost zero and I will not let it be sold as free.** The zero row is inadmissible, not cheap: `.claude/rules/spec-shape.md:57` — *each site that aborts says so beside its operation*. A new abort at every handle call with no sentence is a spec that lies by omission. A's honest floor is **+11 real**. **Route D's zero is also false** — it redefines what `consumes` means and a reader cannot guess it: **+48 real, the second most expensive route on the table.**

**The encoding is worth more than the removals.** Merging B into the existing `nullptr` sentence instead of appending a second one saved **22 real tokens** (58 → 36), against R4's 9 and R3's 15. Panel 122's *merging beats appending* is the cheapest instrument in this room and no seat priced it.

## Principle 0: the compiler needs none of this

Counted myself in `selfhost/`, all four `extern` groups (`selfhost/cli/process.hero:37,49`, `selfhost/cli/io.hero:34`, `selfhost/emit/literal.hero:41`):

- `record <X> tag <Y>` declarations at extern-member indent: **0**
- handle-typed extern parameters: **0**
- parameters marked `consumes` / `borrows` / `acquires`: **0**

Every parameter the compiler binds is `str`, `cstr`, `i64` or `f64`. **`needed_for_self_hosting`: no, for all five routes.** So each must pay through Principle 0's second branch. §1.12 robustness plus a measured wrong answer at exit 0 is that payment, and it is sufficient — for a route that actually closes the class. It is not sufficient for a route that closes a subset and says it closed the class.

## Why C, D and E fail the burden of proof

**C** — unsafe by default costs the *same* +57 real as B's first draft for strictly less safety. A route that is dominated on both axes has no argument left. §1.12, CLAUDE.md § Precedence rank 3.

**D** — measured against the corpus, the premise rests on **four** real-library functions: `free`, `freeaddrinfo`, `curl_easy_cleanup` (`consumes`, all three legal on NULL) and `curl_easy_setopt` (unmarked). Four rows is not an enumeration of C's conventions (CL-057), and D's error is **silent in the dangerous direction**: it exempts every `consumes` parameter from the guard, so a release that dereferences its argument keeps the exact `-O2` wrong-answer-at-exit-0 the defect is about. Whether such a release exists is a question I could not run here and name as one rather than assume — but D is the only route whose failure mode is the defect reopened.

**E** — measured on the whole `.hero` corpus: literal `nullptr` written directly as a handle argument, **0 occurrences**; handle variables bound to `nullptr` and then passed, **5**, one of them the defect's own witness (`tests/golden/surface-fixtures/nullread/main.hero:37-38`: `empty: Node @ nullptr` then `node_value(p: empty)`). A syntactic check on the argument fires on **0 of 5** sites. Reaching the other five needs the flow analysis the checker states twice it does not have — verified verbatim at `selfhost/check/leasing.hero:29` and `selfhost/check/consuming.hero:22-23`. E buys +24 real and closes nothing.

## What a route costs a READER, §1.2

`accepts_null` at one declaration site: **27 → 30 vendored tokens, +3**, measured on a two-file pair. **Zero at use sites** — it is exactly §1.5's shape, verbose where you declare. Across the entire corpus, 41 handle-typed extern parameters exist (21 `consumes`, 4 `acquires`, 16 unmarked, 0 `borrows`); only those legally taking NULL carry the word, ~3 real-library rows today, so **~9 vendored tokens in every `.hero` file this repository owns**.

Against that: one correction round-trip is 500–2000 tokens (§1.2). B's whole spec cost, paid by R4, is **+27 real — under one-twentieth of a single round-trip**. And the class it closes is not a round-trip at all: it is a wrong answer at exit 0, the one failure §1.2's formula cannot price, because the model is never told it was wrong and never rewrites. That is what makes B's tokens free in the only unit that matters, and it is why I do not take A's cheaper +11: A refuses `free(NULL)` and `freeaddrinfo(NULL)` at runtime, which is 3 of the 4 real-library handle functions in this corpus, and it charges that rewrite to every reader forever to save **16 real tokens once**. CLAUDE.md § Precedence: most robust and production-ready, never the cheapest in tokens.

## The warden's fields

- `verdict`: **adopt route B** (encoding F), object A, veto C/D/E
- `section`: design.md §1.6 (ceiling and the unconditional payment rule), §1.2 (cost formula), §1.5 (declaration/use asymmetry), §1.12 via CLAUDE.md § Precedence rank 3; `.claude/rules/spec-shape.md:57` and panel 122's merge rule
- `spec_token_delta`: **measured**, 7974 → 8001 real and 5989 → 6012 vendored for B merged with R4 spent (+27 real, +23 vendored); 2239 free of 10240
- `removal`: **R4**, `acquires sqlite3_finalize` → `acquires` (−9 real, −4 vendored, deletes no rule; the section declares no such function). **R3 stays unspent** — I agree with panel 153's warden, it deletes reader orientation for 15 tokens the budget does not need. If the synthesis prefers B's verbose draft over my merged one, R3 becomes owed too.
- `needed_for_self_hosting`: **no** — 0 handle parameters in `selfhost/`, measured
- `argument`: No route breaches 10240, so no budget veto exists and cost cannot decide this. It decides among routes. A's apparent zero is inadmissible under spec-shape.md and its real floor is +11, bought by refusing legal C forever. C is dominated: same price as B, less safety. D costs +48 for a premise resting on four measured functions, and fails silently in the direction that reopens the defect. E fires on 0 of 5 corpus sites, including the witness, and needs flow analysis the checker twice denies having. B closes the class safe-by-default at +27 real once R4 pays, +3 vendored per declaration and 0 per use.
- `condition`: I move B to *object* if the ffi-realist shows the `accepts_null` word is needed on more than 15 of the 41 corpus handle parameters, which would move the cost from declaration-rare to declaration-common and break the §1.5 argument. I move E to *adopt-as-addition* the day the checker has flow analysis. Nothing moves D: its exemption is unmeasured and its failure is silent.

## Prediction, with the command that scores it

**When route B lands in its merged encoding with R4 spent, `spec/heroes-spec.md` measures ≤ 8010 real and ≤ 6016 vendored — that is ≤ +36 real and ≤ +27 vendored on today's 7974/5989 — and no more than 5 extern handle parameters in the whole repository carry `accepts_null`, i.e. ≤ 15 vendored tokens of reader cost across every `.hero` file.** Scored by, in the commit that lands it:

```sh
set -a && . ./.env && set +a && ./heroes measure spec/heroes-spec.md --refresh
./heroes measure spec/heroes-spec.md
grep -rc 'accepts_null' --include='*.hero' . | grep -v ':0'
```

Registered under §1.6's payment rule; the instrument is `heroes measure`, which exists today, and the milestone is the one that closes defect 045. It pays nothing on its own — **R4 is the named removal** — it is the falsifiable half.

Files: `/Users/joseph/Temp/heroes-lang/docs/design/design.md` §1.6 (line 255), `/Users/joseph/Temp/heroes-lang/spec/heroes-spec.md` § 13 (lines 328-393, the `nullptr` sentence at 357-359 and `CParam` at 392-393), `/Users/joseph/Temp/heroes-lang/.claude/rules/spec-shape.md` (line 57, the abort rule; lines 67-69, the merge rule), `/Users/joseph/Temp/heroes-lang/tests/golden/surface-fixtures/nullread/main.hero` (the witness), `/Users/joseph/Temp/heroes-lang/selfhost/check/leasing.hero:29` and `/Users/joseph/Temp/heroes-lang/selfhost/check/consuming.hero:22`. Drafts and measurements: `/private/tmp/claude-501/-Users-joseph-Temp-heroes-lang/8d7432e4-9b44-486f-80db-fda4fdc56efe/scratchpad/p154-spec-warden` (restored to base) and `/private/tmp/claude-501/-Users-joseph-Temp-heroes-lang/8d7432e4-9b44-486f-80db-fda4fdc56efe/scratchpad/routes.py`, `tight.py`.
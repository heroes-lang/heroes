# Panel 261, the spec-warden's brief

Read `00-shared.md` first, whole (§ 1 the proposal, § 2 the questions, § 3.1 the sentences it moves, § 3.6 the counts,
§ 6.1 a first diff, which is the clerk's and not yours), then the critic's first reading
(`docs/panel/261-reports/critic-briefs.md`), **on which these briefs were repaired** before any seat started. **Your agent file is the base's**
(`.claude/agents/spec-warden.md` of your copy); design.md §1.2 and §1.6 are reached by `grep -n`. Under the author's
amendment of 2026-11-22 (CLAUDE.md § Precedence: *"the most robust solution now always wins, a new form included and
at the cost of the spec's size"*), **the budget and Principle 0 are costs to be measured and written, never a veto**;
your veto on soundness of the text (a sentence that says what the compiler does not do) stands.

**Your copy**: `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/261-spec-warden`, a detached worktree at the
base `df92f9281` (`00-shared.md` names it); build its compiler from the seed and stage 2 there (`00-shared.md` § 8). Your texts go beside your
report as `docs/panel/261-reports/spec-w*.txt`, each with its diff `spec-w*.diff` against the base's
`spec/heroes-spec.md`. **No paid run but the `--refresh` requests below where authorised, no credential printed, `gh`
reads only, nothing outside the repository's root, no `pkill`, no commit.**

## The counts

- **Before**, read by the repair at the base `df92f9281` (`00-shared.md` § 3.6, stage 2's `heroes measure
  spec/heroes-spec.md` from the tree's root, no `--refresh`, 09:53; stage 2 `sources 4dcf3a9f20257573`): claude-legacy
  **10,067**, cl100k_base **10,245**, **real 13,439** (`claude-opus-5`, the pin of 2026-11-26), headroom **2,945**
  against **16,384**, the FFI floor mortgaging 60. The file: 516 lines, 35,246 bytes, sha256
  `3df14254998f68eb4a95ffe6f4c52b6772e2b0298a88ed82eee4b436fd12d93f`. (At the clerk's first base `07e747bc6` it read
  9,966, 10,144, real 13,309, headroom 3,075, 34,828 bytes: panel 260's step 197d added 130 real tokens in § 13's first
  line and `Extern`'s production, no line this proposal moves.) **The seed's compiler prints `STALE`** against this
  spec (its pin, `4dced6767460e590`, predates steps 192c and 197d): read with your stage 2. **Re-read them yourself**
  and say if any differs.
- **A candidate's price**: run `measure` from your copy's root (it reads `vendor/tokenizers/` from the folder it runs
  in). For any file but the pinned spec it prints the two vendored rows and no `real` row, and **a vendored delta is
  not a price** (`.claude/rules/spec-shape.md` § How a change to the document is made). The real count needs
  `./heroes-next measure spec/heroes-spec.md --refresh`, one request to the count-tokens endpoint each, **only where
  the coordinator's prompt authorises it, and then at most four**: `cp spec/heroes-spec.md spec.orig` and its `shasum
  -a 256`, apply the text, `set -a; . /Users/joseph/Temp/heroes/heroes-lang/.env; set +a` (print
  `${#ANTHROPIC_API_KEY}` alone, never the value), `--refresh`, then restore with `cp spec.orig spec/heroes-spec.md`
  and compare the `shasum` before any other command. Without that authorisation every count is written **a lower
  bound, vendored only**, in those words.

## What you judge

1. **The sentences the form owes, written and measured** (none is drafted for you; § 6.1 of `00-shared.md` is a
   pointer to the places, not a candidate): the type table's row (line 109); `TypeArg` (line 128) and whether
   `Param`'s *"`@p: T lent`"* refusal (line 164, panel 217's R1) is stated for a type too; § 9 line 321; **what a call
   through a value owes** (lines 314 to 320: is *"marked `@` in the signature and at the call site"* already true of a
   value once the type carries the mark, or does the text owe a clause, and does *"UFCS does not apply when the first
   parameter is `@`"* need one for a value held in a field, `00-shared.md` § 3.4 `f4`?); `spawn` (line 383); a
   callback's modes (lines 427 and 428). Merge where you can (`.claude/rules/spec-shape.md` § Where a rule lives:
   *merging beats appending*); one home per rule. **Two rules the proposal stands on that the spec does not state**
   (the critic's § 2 items 3 and 5, `grep -n -i` re-run by the repair: nothing for either): **the field first at a
   dot** (`f4` prints 20 where § 9 line 310's UFCS sentence predicts 13; design.md §4.11 line 1949 and panel 079 ruled
   it and refused the collision clause, `00-shared.md` § 2 question 2), on which `app.handler(@app, r)` stands; and
   **the names' one-way fit** (an expected function type carrying no names accepts a value whose type has them,
   `selfhost/check/type_fit.hero:13-37`, feature 1133; `00-shared.md` § 1 item 1), the shape a `lent` to `@`
   conversion would take. Say whether each owes a sentence, and price it.
2. **The interaction with § 5 line 178**: *"an `@` parameter nothing writes or ends"* is a compile error
   (`never_rebound`, `00-shared.md` § 3.4 `u1`), so a read-only handler in a table of `@` handlers is refused today.
   Whatever route the sitting takes on question 5 (a conversion, a waiver, two tables), the text must let a reader
   predict it: write the sentence each route owes, for the no-state handler too (`@_: App` refused, `_: App lent`
   built, probes `c2`, `c3`). **`never_rebound` is one code with four messages** (the critic's § 4 item 1, re-read by
   the repair: `selfhost/resolve/errors.hero:340`, a cell nothing re-binds; `:357`, a cell declared on `_`;
   `selfhost/resolve/written_nowhere.hero:75`, `@_`; `:92`, an `@` parameter nothing writes), so a prediction of the
   blind run's D3 counts the `@` parameter's and the `@_` parameter's messages, never the code alone
   (`blind/registration.md` § The grades, as repaired); and a write that changes nothing, `app @ app`, passes it today
   (`c1`), which the registration now reports as a row of its own.
3. **What a reader must be able to predict** from the text alone: which function is a value of which type; that
   `(function(@App) -> ())` and `(function(App) -> ())` are two types (as the names are, lines 307 to 309); where an
   `@` stands at a call through a value and where none does (`lent`); which call is refused for aliasing; what a mode
   in a callback means to C; what `spawn` refuses.
4. **The record's sentences the change falsifies**: § 9 line 321, and the reason in design.md §4.13 lines 2072 to 2078
   (design.md is amended beside the spec, citing the sitting); the words *"no function type carries an `@` (§ 9)"* in
   three diagnostics (`00-shared.md` § 3.3; `selfhost/data_errors.hero:180` and `:194`, two messages, and `:220`,
   `marker_through_a_value`'s note: `grep -rn 'no function type carries' selfhost`, the repair): each is a sentence a landing must change in the same commit, and
   `tests/harness/spec_text.hero` and the `spec` suite's checks (`tests/harness/suite_spec.hero`) are what hold the
   text: say which check holds which new sentence, or that none does (design.md §1.6's payment rule: an addition owes a
   named removal or a registered prediction naming an instrument that exists).
5. **Rebel's word**: `web.hero:19-23`'s *"a function value takes no `@` parameter (spec § 9)"* and `http.hero:2339-2341`
   become false with the landing (`00-shared.md` § 5.1): name every library and example sentence that cites § 9 for
   this (`grep -rn 'takes no `@`\|no function type carries' rebel/`), each owed in the landing.

## Predictions registered before the blind run

**Your report must hold the heading `## Predictions registered before the blind run`**, with your predictions under it,
**before any blind session starts** (the runner copies your report, hashed, as it starts). The run is **authorised by
the author** (2026-11-26, *ok a 20 dollars*, a ceiling of 20.00 USD for 31 sessions) **and started by the coordinator after the synthesis**
(`llm-ergonomist.md`); you predict before the adopted text exists, so **predict for each of your own texts** (each
`spec-w*.txt`, named by its route): under it as `adopted`, and under `now`, each decision rule's count of
`blind/registration.md` (D1 to D4 there), a number and a range, and the spend. An arm is read against your text whose
diff is byte for byte the arm's; else against your text of the same route, every line the arm's diff has and yours
does not named in the scoring; else the arm is *unpredicted*, neither right nor wrong. Say which of your texts you
expect adopted.

End with the verdict structure of your agent file, then one line per numbered question: the text you would adopt (its
count, real or *a lower bound, vendored only*), and the one measurement that would change your mind.

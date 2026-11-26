# Panel 261, the spec-warden: a function type carries its parameters' modes

Written 2026-11-26 from 10:18 by the spec-warden seat, in the detached worktree
`.claude/worktrees/261-spec-warden` at `df92f9281a89c83252a076b0dffed2ee2b32dec9` (`git log -1`, 10:19), working
files in its `.scratch-warden/` (untracked). Every number below was produced by the command beside it, run by me in
this session at that base, or is marked *an inference*, *carried* or *unrun*.

**No `--refresh`, no API call, no paid run.** The warden's brief allows `--refresh` *"only where the coordinator's
prompt authorises it"*; the prompt authorises none and says *never a paid run*. So **every candidate's count is a
lower bound, vendored only**, and the real figures given beside them are an inference, marked so. The base's `real`
row is the pinned one, which stage 2 prints offline.

## 1. My compilers, and the counts before

- Seed: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, `real 22.58 user 19.78` (load 16 to 28 by
  `uptime`, so not a duration); `--version` `sources 05186f2b38e77a97`.
- Stage 2: `./heroes build selfhost/main.hero -o heroes-next`, `real 397.54 user 234.67 sys 57.99` (load 15 to 31, not
  a duration); **`sources 4dcf3a9f20257573`**, the repair's key (`00-shared.md` § 8).
- **The spec at the base**: `wc -l -c` 516 lines, 35,246 bytes; `shasum -a 256` `3df14254...d12d93f`;
  `./heroes-next measure spec/heroes-spec.md`: claude-legacy **10,067**, cl100k_base **10,245**, **real 13,439**
  (`claude-opus-5, 2026-11-26`), headroom 2,945, the FFI floor's 60. **Every one equals the brief's.**
- **The ground's probes**: `sh ground/probes.sh ./heroes-next <a copy of ground/probe> <out>`; each of the 32
  `*.repair.out` I wrote `diff`s empty against the repair's once the file paths are normalised
  (`.scratch-warden/probe-out/`). So `f4` prints 20, `c1` and `c8` pass, `c2` is `never_rebound`, `c3` prints 1, `p1`
  is `expected_type`, `p4` `expected_function_type_params_close`, `s1` and `s2` the two `*_as_value` codes, as the
  brief says. `f7 --emit-c` under my stage 2: line 57 `typedef int64_t (*h_0fn_32a9823f)(h_f7_App, int64_t);`, line
  113 `int64_t h_f7_bump(h_f7_App *ph0_a, int64_t h1_x);` (`.scratch-warden/mine/f7.c`).
- design.md §1.6, reached by `grep -n '^### 1\.'` (line 255): the ceiling is **16384**, `claude-opus-5` through
  `POST /v1/messages/count_tokens` (lines 257 and 258); the payment rule unconditional (lines 326 to 328, 331 to 336);
  §1.2 at line 192. Under CLAUDE.md § Precedence as amended 2026-11-22, the budget and Principle 0 are costs written
  here and never my veto; a sentence that says what the compiler does not do is.

## 2. A correction to the brief: the names' one-way fit runs the other way

`00-shared.md` § 1 item 1, my brief's item 1 and the critic's § 2 item 3 say the fit lets *"an expected type that
carries no run of names accept a value whose type has one"*, from feature 1133's body (`gh issue view 1133`, read
10:33: *"`table.fits` widens: an expected type carrying no names accepts a value whose type does"*). **At the base
it is the converse**, by probes of mine (`.scratch-warden/mine/`, each canonical by `fmt | cmp`, run by stage 2):

| probe | what it writes | answer |
|---|---|---|
| `w4` | `double(n: i64)` handed where `(function(x: i64) -> i64)` is asked | `check` exit 0, **prints 8** |
| `w5` | `g: (function(x: i64) -> i64) = double`, then `g` handed where `(function(i64) -> i64)` is asked | **`error[type_mismatch]`**: *"expected `(function(i64) -> i64)`, found `(function(x: i64) -> i64)`"*, exit 1 |
| `w2` | `(function(str, str) -> str)`, a pair with no names | `error[needs_parameter_names]`, exit 1 |

`selfhost/check/type_fit.hero:13-22` says what the probes do: *"a name that is not there fits whatever the expected
type calls that position"*, the blank on the VALUE's side (a `_` parameter, or *"every declared function with no two
parameters of one type"*). Feature 1133's sentence was written when *"a declared function used as a value carries its
parameter names ALWAYS"* (its body); which later change moved the names off the value is **unrun** (no `git log -S` was
made). **What it means here**: the mode fit W1 proposes runs in today's direction, the value claiming less (`lent`, it
reads) than the slot grants (`@`, it may write), as a value with no name claims less than a type naming the position.

## 3. The candidate texts

Each is the whole spec with the text applied, `docs/panel/261-reports/spec-w*.txt`, with its diff against the base,
`spec-w*.diff` (`diff -u --label a/spec/heroes-spec.md --label b/spec/heroes-spec.md`, so `patch -p1` applies it;
each checked by applying it to a copy of the base and `cmp` against its text, four of four equal). Built by
`.scratch-warden/make_texts.py` (every anchor asserted to match the base exactly once). Counted by
`./heroes-next measure <text>` from my copy's root, 10:42.

| text | route on question 5 | cl100k_base | claude-legacy | real, **an inference** |
|---|---|---|---|---|
| `spec-w1-fit` | `lent` fits where a type asks `@` | 10,394 (**+149**) | 10,211 (+144) | about 13,625 to 13,642 (+186 to +203) |
| `spec-w1c-fit-callback-place` | the same, a callback's mark admitted | 10,406 (+161) | 10,223 (+156) | about 13,640 to 13,658 (+201 to +219) |
| `spec-w2-identity` | the marks are the type's, nothing converts | 10,362 (+117) | 10,180 (+113) | about 13,585 to 13,598 (+146 to +159) |
| `spec-w3-waiver` | identity, and `never_rebound` waived for a function used as a value | 10,371 (+126) | 10,189 (+122) | about 13,597 to 13,610 (+158 to +171) |

`shasum -a 256` (10:42 and 10:43), text then diff:

- `spec-w1-fit.txt` `4fc894caa1bd2e8d99d634de36b857ea18c8859881ed2972e894c7273670331b` (35,724 bytes);
  `spec-w1-fit.diff` `15d0c16c854599ee1d31c48424c4ed3a94f1169d6aa13a1071c6824f2a7cca07` (12,263);
- `spec-w1c-fit-callback-place.txt` `e78f855455581537599ce1e3f5e59b4e288114b6f258e7b1d7088ebbadb6b4fe` (35,748);
  `.diff` `b07163fef5180c31871f3d0945905164743fa5077cfcdfcc743187872cfd6a78` (12,287);
- `spec-w2-identity.txt` `157546ea53a6ada368440cc567f0a7431e8f239fec84554b7c8864adde3dbfe2` (35,632);
  `.diff` `6b57f965a7e87b107cc29fe9c97b76bb015bb3026bad9af9784ca29838015df3` (12,170);
- `spec-w3-waiver.txt` `b34676fa7f505502fc2cb592f34a66d832998f0df4156c382c412d90f20953f7` (35,668);
  `.diff` `1fa1d26a386a6919e5cb364d53eae3b5bcb553da7b45824150985193524576e2` (13,013).

**All four counts are a lower bound, vendored only.** The real column multiplies the cl100k delta by 1.25 to 1.36,
panel 260's ledger ratio (`docs/panel/260-reports/spec-warden.md:346`); the one ratio measured on both instruments at
this base, panel 260's own move from `07e747bc6`, read +101 vendored for +130 real, 1.29 (the clerk's and the repair's
counts, `00-shared.md` § 3.6). **An inference, never a price** (`.claude/rules/spec-shape.md` § How a change to the
document is made). The command that prices one: `cp spec/heroes-spec.md spec.orig`, `cp <text> spec/heroes-spec.md`,
`./heroes-next measure spec/heroes-spec.md --refresh`, `cp spec.orig spec/heroes-spec.md`, `shasum` compared.

**The pieces, each measured alone against the base** (`.scratch-warden/pieces/`, the same command):

| piece | where | cl100k | legacy |
|---|---|---|---|
| `TypeArg  = "@" [ ident ":" ] Type \| [ ident ":" ] Type [ "lent" ] .` | § 3, line 128 | +13 | +14 |
| the field first at a dot (below) | § 9, line 310 | +19 | +18 |
| the values bullet, fit route (W1, W1c) | § 9, lines 321 and 322 | +100 | +95 |
| the values bullet, identity route (W2, W3) | the same | +68 | +64 |
| the waiver (W3) | § 5, line 178 | +9 | +9 |
| `spawn`: *"`f` taking `x` with neither mark"* | § 11, line 383 | +12 | +12 |
| a callback: *", with neither mark,"* (W1, W2, W3) | § 13, lines 427 and 428 | +5 | +5 |
| a callback: *", an `@` or `lent` one the place C hands it,"* (W1c) | the same | +17 | +17 |
| *in no text*: a self-write counts as none, *"an `@` argument counting and `x @ x` not"* | § 5, line 178 | +7 | +7 |
| *in no text*: the fit bullet without `peek`'s type and `g(l)` | § 9 | +72 | +70 |

The pieces sum to each text's delta within one token (tokens merge at an edit's edges).

### The sentences, as the texts write them

**§ 3** (every text): `TypeArg  = "@" [ ident ":" ] Type | [ ident ":" ] Type [ "lent" ] .` It is `Param`'s shape
(§ 4 line 164) with the name optional, so `@p: T lent` is not derived in a type as it is not in a signature: **panel
217's R1 carries by the production, with no prose**, as it does for `Param`. **The type table's row (line 109) does
not move** (0 tokens): the production beneath it and § 9's bullet are where a reader looks, and a third statement
would be a second home.

**§ 9 line 310** (every text): *"- `x.f(y)` calls `x`'s field `f` where it has one, and is otherwise sugar for
`f(x, y)` (UFCS), except a thread's ..."*.

**§ 9 lines 321 and 322**, W1 and W1c:

```
- Top-level functions are values, typed with the marks their signature writes,
  and a call through a value marks its arguments as its type does:
  `xs.fold(0, add)`; `advance` above is a `(function(@Lex) -> ())`, called
  `f(@l)`, and `peek` a `(function(Lex lent) -> ())`, called `g(l)`. The marks
  are part of the type, but `lent` fits where a type asks `@`: `peek`, and
  `skip(_: Lex lent)`, are values of `advance`'s type too.
```

W2 and W3 end the same bullet at *"The marks are part of the type, as the names are."* `advance` and `peek` are the
bullet above's own examples (lines 315 to 317, `peek` with no `->`), so the form is taught on functions the reader has
just met, and `peek` is a `(function(Lex lent) -> ())` exactly.

**§ 5 line 178**, W3 alone: *"... or an `@` parameter nothing writes or ends (section 13) in a function never used as
a value, an `@` argument counting; ..."*.

**§ 11 line 383** (every text): *"`spawn(f, x)` runs `f(x)` on a thread of its own, `f` taking `x` with neither mark,
and gives a `(thread T)?`"*; *"neither mark"* is § 9 line 316's own phrase.

**§ 13 lines 427 and 428**: W1, W2, W3 *"its parameters follow the same rule, with neither mark, and `()` is
`void`"*; W1c *"its parameters follow the same rule, an `@` or `lent` one the place C hands it, and `()` is `void`"*.

### Ran: the `spec` and `grammar` suites on each text applied to the real path

Each text copied over `spec/heroes-spec.md` in my copy, then `./heroes-next run tests/harness/main.hero --
./heroes-next spec` and `... grammar`, then the base restored by `cp` and its `shasum` compared
(`3df14254...d12d93f` each time, `git status --porcelain spec/` empty); outputs `.scratch-warden/suite-*.txt` and
`final-*.txt`. **`spec`: 32 passed, 4 failed, on every text**, the four being `budget` (*"the spec measures 10393 and
the record says 10245"* on W1's first draft), `spendable`, `real` and `ledger`: the checks that pin the count, its
digest and the ledger's newest row, red for any change until the landing refreshes them, as they were for panel
260's texts. `shape`, `anchors`, `names`, `rejected`, `inventory`, `colouring`, `offered`, `fences`,
`refusal-words` and `design-copy` pass. **`grammar`: 12 passed, 0 failed, on every text**, and `heroes grammar` prints
the new `TypeArg` (`.scratch-warden/grammar-w1.txt` line 53). First run on W1, W1c, W2 and W3 as first drafted
(10:32 to 10:42; `real 157.74 user 86.35` for W1's `spec`, at load 30 to 76); W1, W2 and W3 again after the one
comma that separates *"with neither mark"* in § 13 (W1c unchanged):

**the same, 32 and 4 on `spec` with the same four, 12 and 0 on `grammar`, for each of the three** (10:42 to 10:47,
`final-spec-*.txt`, `final-grammar-*.txt`), `budget` reading *"the spec measures 10394"* for W1, 10362 for W2 and
10371 for W3, the counts of § 3; the base restored, `3df14254...d12d93f`, `git status --porcelain spec/` empty.

## 4. What I judge (the brief's items 1 to 5)

### 4.1 The sentences the form owes, and the two rules the spec does not state

- **The values bullet is the one home.** Line 321 is where today's refusal stands and where a reader looks to learn
  whether a function is a value, so the marks' type, the call through a value, the identity and the fit merge there
  (`.claude/rules/spec-shape.md` § Where a rule lives: *merging beats appending*); its removal, *"but not one with an
  `@` or `lent` parameter"*, is netted in the piece's +100.
- **Lines 314 to 320 owe nothing more**: *"marked `@` in the signature and at the call site"* and the exclusivity
  sentence speak of every call, and the bullet's *"a call through a value marks its arguments as its type does"* puts
  the type where the signature stood. **Through the fit, the type's mark judges the call**: a `lent` function bound in
  an `@` slot is called `f(@l)` and judged as `@`, so an `@` nested in another argument of that call is accepted, as
  it is beside an `@` today. That is sound by the reason design.md §4.8 gives for the nested refusal (lines 1560 to
  1562: *"the unmarked argument reads as a value taken where it stands"*): through the `@` type the argument is marked,
  and the place is what the reader was told. Measured today: `both(@p, bump(@p))` prints 4 (`mine/w1.hero`, exit 0),
  design.md's figure of 2026-10-18 (lines 1562 to 1564).
- **UFCS, and the field first: owed, +19.** Line 310 says `x.f(y)` IS `f(x, y)`; the base prints 20 for `f4` where
  that sentence predicts 13, because design.md §4.11 line 1949 and panel 079 put the field first. **The base's text
  already says what the compiler does not do**, and the proposal's driving shape, `app.handler(@app, r)`, stands on it:
  without the sentence a reader reads it as `handler(app, @app, r)`. With it, *"UFCS does not apply when the first
  parameter is `@`"* needs no clause for a value in a field: the dot is a field call before it is UFCS.
- **The names' fit: no sentence** (0 tokens). Its silence is on the lenient side (`w4`): the text says the names are
  part of the type, so a reader predicts a refusal the compiler does not make, and no reader's program is refused by
  the silence. Under §1.2 a sentence that removes no rewrite buys nothing. The mode fit is stated because its silence
  would be on the other side: under identity alone the read-only handler is refused (`u1`).

### 4.2 § 5 line 178 under each route, the no-state handler included

- **W1, the fit**: § 5 does not move. A read-only member of an `@` table writes `lent` (`peek`), one that ignores the
  state `_: T lent` (`skip`), both in the bullet; `@_: T` stays `never_rebound` by § 5 as written (`c2`), and `_: T` by
  value is refused by the identity, since only `lent` fits. **Why the fit stops at `lent`** (measured, not a taste): a
  `lent` parameter and an `@` one are one C type, `h_w6_Lex *ph0_l` for both (`mine/w6.hero --emit-c`, lines 112 and
  113), so a `lent` value stands in an `@` slot with no adapter; a by-value parameter is the struct itself (`f7.c` line
  57, `(h_f7_App, int64_t)`), so a by-value VALUE in an `@` slot would need a thunk holding the pointer, which is a
  closure, design.md Part 6 line 3644's refusal. The text says no more than a value can do.
- **W2, identity**: no sentence makes the read-only member legal; the reader is refused (`u1`), writes `app @ app`
  (`c1`, which passes), answers the state by value, or splits the table. The text says none of this, and D3 is where it
  shows (§ Predictions).
- **W3, the waiver**: the clause sits on § 5's sentence, the rule it amends; it covers `@_` too (*"nothing writes"*).
  **I object to it**: whether `never_rebound` applies is decided by a use elsewhere in the program, which design.md §1.3
  refuses (lines 213 and 214: *"the meaning of a line can be determined from that line plus the signature of the
  enclosing function"*), and an `@` that writes nothing is again a signature that lies, the class `never_rebound`
  closed.
- **The self-write** (`c1`, `c8`, `c7`: `app @ app` passes): no text of mine holds it, since refusing it is a new rule
  no seat has built; if the sitting adopts it, *"an `@` argument counting and `x @ x` not"* is +7 vendored.

### 4.3 What a reader must be able to predict, read against W1

Which function is a value of which type: every top-level function, its type its signature with the marks. That
`(function(@App) -> ())` and `(function(App) -> ())` are two types: *"the marks are part of the type"*. Where an `@`
stands at a call through a value: where the type writes one (`f(@l)`), none for `lent` (`g(l)`). Which call is refused
for aliasing: § 9's exclusivity sentence, judged by the type's marks. What a mode in a callback means to C: W1 refuses
it (*"with neither mark"*), W1c says *"the place C hands it"*. What `spawn` refuses: an `f` with either mark. **What W1
does not let a reader predict**: whether `app.handler(@app, r)` is refused for reading the callee out of the place the
call lends. § 7 line 241 (*"left to right as written"*) and `q2a`/`q2b` (the callee read before a nested `@`) predict
it accepted; **if the landed compiler refuses it, every text here owes a clause**, unwritten because no seat has built
either answer.

### 4.4 The record's sentences the change falsifies, and what holds the new ones

Owed in the landing's commit (each by `grep -n`, 10:22 to 10:40):

- `selfhost/data_errors.hero:180` and `:194` (the two `*_as_value` messages, *"no function type carries ... (§ 9)"*)
  and `:220` (`marker_through_a_value`'s note); comments at `selfhost/resolve/built_marks.hero:101` and
  `selfhost/check/function_value.hero:7`;
- design.md §4.13 lines 2072 to 2078 (*"But not one with an `@` or a `lent` parameter"*), §4.8 line 1559 (*"a function
  with one used as a value (§4.13)"* among `lent`'s refusals), Part 8 item 19's *"fifth cause"* (lines 4606 to 4608,
  stale today by `o1`) and its last lines (4634 to 4640), whose *"qualifier field on the function type"* the landing
  makes a fact or contradicts;
- **What holds each new sentence: nothing in the `spec` suite holds its truth.** `grammar/productions` holds the
  production's consistency and `heroes grammar` prints it, but *"It does not compare a production to the parser"*
  (`.claude/rules/spec-shape.md`); `named`, `rejected`, `inventory` and `colouring` hold the words of the new code
  spans. So the landing owes one golden case per sentence: a `check` case parsing `(function(@App) -> ())` and refusing
  `(function(@a: App lent) -> ())`; a `run` case calling through an `@` value; a `run` case binding a `lent` function in
  an `@` slot by name, through a value and through a generic parameter (`bind`); a `check` case refusing an `@` function
  where an unmarked or `lent` type is asked; `f4` as a `run` case (20); `spawn(bump, @x)` and `spawn(get, c)` refused;
  a callback's mark refused (W1) or its C built (W1c). **The payment** (design.md §1.6): the named removal is inside the
  values piece; the rest is my registered predictions below, scored by the blind runner, which exists (panel 256's, run
  by panels 257 and 259). Every text crosses `DELTA_GATE` (50 vendored, `tests/harness/suite_spec.hero:216`), so the
  landing commit's body names that payment.

### 4.5 Rebel's word

`grep -rn 'takes no `@`\|no function type carries' rebel/` prints **5 lines**: `web.hero:21` (inside 20 to 24, *"Why
the program dispatches"*), `:585` (inside 582 to 585, `before`'s doc), `:1362` (`limit_rate`'s doc, *"a step `before`
calls takes no `@` value"*), and `interpreter/run/value.hero:11` (inside 7 to 13). **Each becomes false with the
landing.** Beside them, resting on `web`'s opening comment by name (*"`web`'s opening comment says why"*):
`notes/app/state.hero:1-5` and `registry/server/serving.hero:5-8`, whose *"no closure"* stays true and whose *"so the
router finds ... and the program calls"* stops being the only way. `http.hero:2339-2341` (*"read and never changed:
each call receives its own copy"*) stays true of `serve_with` as written, and becomes false only if `serve_with` takes
`@state`. `PACKAGES.md:9356` (*"since a function value holds none of its own (spec § 9)"*) is about closures and stays
true; `PACKAGES.md` is regenerated from `heroes doc`, so `before`'s and `limit_rate`'s docs move it.

## 5. What it displaces, and §1.2

Nothing comes out but line 321's refusal, **and that is a problem I state rather than solve**: W1 costs about +195
real (an inference) of the 2,945 of headroom. §1.2's side: a correction round trip's median is **3,619 tokens**
(design.md §1.6 lines 391 to 394, measurement 052), so W1's tokens, read once per session, pay if the form averts one
round trip in about eighteen sessions that read it; the dispatch chains it replaces are measured in `00-shared.md`
§ 5.3 (notes' 31 lines, auth's 21 arms, a `_` arm in every `match` there). **Whether it averts them is D1 and D3**,
registered below. Principle 0: the compiler's own need is panels 109 and 121's knot (question 12), not built by me;
the thesis side is the blind run.

## Predictions registered before the blind run

Registered 2026-11-26 before any blind session (none is drawn). **The text I expect adopted: `spec-w1-fit`**, or W1c,
which differs from it in § 13 alone, where no task reaches, so every prediction for W1 is W1c's. Counts are out of 5
per task, of the 10 sessions of t2 and t3 for D3, of the 15 of an arm for D4; *right* is compiles, prints-right and
bound together. The decision rules are `llm-ergonomist.md` § The decision rules.

**`now`** (the base, the same for every text): t1 right **3** (2 to 5), t2 right **3** (2 to 5), t3 right **4** (3
to 5); not prints-right t1 **2** (0 to 3), t2 **2** (0 to 3), t3 **1** (0 to 2); refused-for-mode across t2 and t3
**1** of 10 (0 to 3), t1 **1** (0 to 2); self-writes **0** (0 to 1); lost-updates **1** of 15 (0 to 2).

**`adopted` = `spec-w1-fit`** (and W1c):
- **D1 holds**: t1 right **4** (3 to 5), t2 right **4** (3 to 5); t3 right **4** (3 to 5).
- **D2 holds**: not prints-right t1 **1** (0 to 2), t2 **1** (0 to 3), t3 **1** (0 to 2).
- **D3 holds**: refused-for-mode across t2 and t3 **1** of 10 (0 to 3); at least 3 of 5 t2 readers write `lent` (or no
  mark, by the value route) on `GET`, `KEYS` and `COUNT`, and at least 3 of 5 t3 readers write `lent` on `locked` and
  `push`'s function, `_` or named; t1 reported **1** (0 to 2). The refusal I expect when one comes: a by-value
  `s: Store` in an `@` table, not `never_rebound`.
- **Self-writes 0** (0 to 1); **D4: lost-updates 0** of 15 (0 to 1), at or below `now`.

**`adopted` = `spec-w2-identity`**: **D1 fails** on t2: t1 right **4** (2 to 5), t2 right **2** (1 to 4); **D3
fails**: refused-for-mode **4** of 10 (1 to 7), `never_rebound`'s `@`-parameter message the commonest; self-writes
**2** (0 to 4); D2 on its boundary on t2 (not prints-right **3**, 1 to 5, against `now`'s 2 plus 1); lost-updates **0**
(0 to 2).

**`adopted` = `spec-w3-waiver`**: **D1 holds** (t1 **4**, t2 **4**, each 3 to 5); **D2 holds**; **D3 holds**:
refused-for-mode **0** of 10 (0 to 2); self-writes **0**; lost-updates **0** (0 to 1). W3 passing D3 is no argument
for it: the run cannot see §1.3's cost, which falls on a reader of the function, not of the table.

**The real count**: W1 at `--refresh` lands between **+186 and +203** real, 13,625 to 13,642.

**The spend**: **11 USD** for the 31 sessions (7 to 17), under the 20.00 ceiling, no session past its cap.

## Verdict

- `verdict`: **approve `spec-w1-fit`, provisional** (every count a lower bound, vendored only; the real count
  unauthorised and unrun), on the conditions below; **object** to `spec-w3-waiver` (§1.3) and to `spec-w2-identity`
  as the adopted text (it leaves the read-only handler refused, D3); W1c in place of W1 only where the ffi-pragmatist
  builds the C of a callback's mark.
- `section`: design.md §1.6 (lines 257 and 258, the ceiling and its instrument; 326 to 336, the payment), §1.2 (192 to
  206), §1.3 (213 and 214); CLAUDE.md § Precedence as amended 2026-11-22.
- `spec_token_delta`: cl100k_base 10,245 to **10,394 (+149)**, claude-legacy 10,067 to 10,211 (+144), **a lower bound,
  vendored only**; real 13,439 to about 13,625 to 13,642, **an inference** (ratio 1.25 to 1.36); W1c +161, W2 +117,
  W3 +126 on cl100k.
- `removal`: line 321's *"but not one with an `@` or `lent` parameter"*, netted. Nothing else, **and that is a
  problem**; the payment is the registered predictions, scored by the blind runner that exists.
- `needed_for_self_hosting`: **no**, by any build of mine; panels 109, 121 and 138 measured the compiler's knot, which
  no seat has yet built through a value (question 12).
- `argument`: Line 321 is the refusal's one home, so the form lands there: values typed with their marks, the call
  marking as its type does, the marks part of the type. Identity alone makes the commonest member of a handler table,
  one that only reads, a compile error (`u1`), and the reader's way out is a self-write that passes (`c1`). The fit
  `lent`-to-`@` closes that without a waiver and costs no adapter, both being one C pointer (`w6`). The field first at
  a dot is owed whatever is adopted: today's line 310 predicts 13 where the compiler prints 20.
- `prediction`: under `adopted` = W1, at most 1 of 10 t2 and t3 readers refused for a mode (D3), and at least 4 of 5
  right on t1 and on t2 (D1); W1's real delta between +186 and +203.
- `condition`: I withdraw approval of W1 if the landed compiler applies the fit at naming only and not in `fits` and
  `bind` (then *"values of `advance`'s type too"* says what it does not do for a value or a generic path); if
  `app.handler(@app, r)` is refused (W1 then owes a clause); if D3 fails under W1; or if `--refresh` reads W1 past +250
  real.

## One line per question of `00-shared.md` § 2

1. Grammar and identity: `TypeArg` as `Param`, the refusal by production; *"the marks are part of the type"*. In W1
   (+13, and inside the bullet's +100, vendored). Would change my mind: a `bind` case that hides a mode.
2. The callee read from the lent place: no clause (§ 7 line 241 predicts the copy); the field first owed, +19
   vendored. Would change my mind: the landed compiler refusing `app.handler(@app, r)`.
3. Arrays, maps, records of such values, `==`: no text (a type is a type; § 7 lines 235 and 236 compare an address).
   Would change my mind: one of them refused by the landed compiler.
4. Generics and `Router<S>`: no spec text; generic records stay refused (§ 9 line 323). Would change my mind: a sketch
   that builds only with a generic record.
5. `lent` in a function type, and the read-only member: W1's fit, inside the bullet. Would change my mind: D3 failing
   under W1.
6. A mode mismatch's diagnostic: the spec names no code; the three messages of `data_errors.hero` are owed. Would
   change my mind: none, on text.
7. The C boundary: W1 *", with neither mark,"* +5; W1c *"the place C hands it"* +17, vendored. Would change my mind:
   the ffi seat's thunk, built.
8. Threads: *"`f` taking `x` with neither mark"*, +12 vendored. Would change my mind: a sound `lent` through `spawn`,
   built.
9. The emitted C: no text; `w6` shows the `lent` and `@` slots one C type. Would change my mind: a typedef keyed
   without the mode.
10. The tools that re-print a program: no text; each owes the form. Would change my mind: none, on text.
11. The spec's real cost: W1 +149 vendored, a lower bound; real unrun. Would change my mind: `--refresh` past +250.
12. The record: panel 217's refusal was a ruling with its alternative written; W1 reverses its R2 for `@` slots alone.
    Would change my mind: a compiler-need built through a value (it would move `needed_for_self_hosting`).
13. The route nobody listed: (h) as W1's fit, not as a waiver; (m) only at naming, which W1 does not promise. Would
    change my mind: a by-value fit shown to need no closure.
14. What the landing owes: § 4.4 and § 4.5 above. Would change my mind: none, on text.

## What stays unrun

The real count of every text (`--refresh`, unauthorised); every behaviour of the landed compiler (none exists); which
commit moved the names off a declared function's value type (§ 2); the blind run (the coordinator's, after the
synthesis).

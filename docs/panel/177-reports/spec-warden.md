# Panel 177, spec-warden

Ceiling reached by grep, this sitting: design.md §1.6 line 255, **10240 tokens on `claude-opus-5`
through `POST /v1/messages/count_tokens`**, and the payment rule is unconditional (lines 305-314).
Every number below is **measured**. Real means `./heroes measure spec/heroes-spec.md --refresh`, run
in my copy (`<scratchpad>/177-spec-warden/`, compiler built from the seed at `521c5e02`, `.env`
sourced with `set -a`, key length 108, value never printed). Vendored means the maximum over the two
vendored tables. Each draft was applied alone to the copy's spec, measured, and reverted, and the
harness checks the pristine sha256 after every run. Nothing here is estimated unless it says so.

**Baseline, re-measured today:** vendored 6282, **real 8361**, digest `0159e9b26bd998a2`. That
leaves 1879 free, or 1819 after the 60-token FFI floor.
**Calibration:** a one-word edit reads **+1 real**, with a new digest (`2d2fe6a56a2bc478`), so the
refresh recounts rather than echoing the pin. Panel 176's C1 text, rebuilt byte for byte from
`176-sw-work/C1_final.md`, reads **8543, digest `074e144b0f5efd6d`**. That is yesterday's number
and yesterday's digest, exactly.

---

## The verdict in the seat's structure

- `verdict`: **no veto.**
  - **Approve** these at their measured price: the set, `transfers <releaser>`, `retains`, the
    agreement sentence (A2) and the lease clause, with the grammar written using one named
    production, `Life`.
  - **Object** to the success clause placed on the parameter (alpha).
  - **Approve, conditionally,** the success clause placed on the result (beta), in its form that
    also covers `retains`.
  - Question 1:
    - **Object** to D as the sentence that lands. **Approve D+** as the floor.
    - **Object** to the two literal sentences the ergonomist reads (II for M, III for R): each is
      false of its own route.
    - **Object** to R in any form, and to P until the `==` question is answered.
    - The true texts of M and S are admissible, at their prices.
  - Not provisional: every count is real.
- `section`: design.md §1.6 (budget and payment), §1.2 (rewrite rate), §1.0 and §1.12 (Principle 0
  and its limit: *"It does not suspend Principle 0"*, line 590), CLAUDE.md §12 (a refusal meets the
  standard a feature does, and the spec beats the compiler).
- `spec_token_delta`: the recommended landing (`Bx_Dplus`) goes from **8361 to 8675 real (+314)**
  and from 6282 to 6538 vendored (+256), digest `5d057c4e04f6aaa3`. The same text with the short
  success sentence (`Bxs_Dplus`) reads **8659 (+298)**, digest `165aa4a1105e1f39`. The dearest
  combination measured is the adopted alpha text with the inline grammar plus route P: **8726**.
- `removal`: **nothing that pays, and that is a problem.**
  - The only true removal is *like a handle nobody consumes* becoming *like a handle still owed*
    (−3, already inside every adopted draft).
  - The `Life` production is 29 cheaper than the inline grammar. That is a cheaper spelling of new
    text, not a removal from the document.
  - I priced *These five mark only a value* at **−26 real** and declined it. The paragraph names six
    marks, not five (`owned` is one of them), so the phrase is ambiguous, and §1.2 says a saving
    that makes a mistake more likely is a net loss.
  - So every token here is paid by the predictions below.
- `needed_for_self_hosting`: **no.** `selfhost/` has three `extern` groups (`cli/process.hero`,
  `cli/io.hero`, `emit/literal.hero`), and none of their members carries `consumes`, `acquires`,
  `borrows`, `transfers` or `retains` (grep, run today).
- `argument` (≤120 words): The adopted text costs +218 to +294 real, 64% to 73% more than the
  C1 that panel 176 priced, because the added checks must be written. The success clause is
  the weakest form under Principle 0. `examples/` holds no instance of it, and its evidence is an
  emulation. Nobody has priced its zero-token alternative, a header shim freeing the value on
  failure, which panel 176 used twice. On the parameter it misses the shape beside it:
  `X509_up_ref` returns 0 on failure (OpenSSL 3.6.4 man page), leaving the runtime counting a
  reference C never took. Covering that needs a second slot or a grammar that also admits
  `acquires … on`. On the result, one sentence covers both: +60 to +76.
- `prediction`: see § 5. The one that most directly pays under §1.2: reading the landed spec
  alone, the llm-ergonomist binds `json_object_object_add` with success `0` **and**
  `X509_up_ref` with success `1`, both correct on the first attempt.
- `condition`: see § 6.

---

## 1. Cost of every candidate, measured

Real deltas are against 8361, vendored against 6282. The texts are in
`work/variants/<name>.md`, the (old, new) pairs in `work/variants.py`, and the harness in
`work/price.py`.

### 1a. The adopted resolution, items 1 to 8 of panel 176's synthesis

| variant | what it is | vendored | **real** | Δ real | digest |
|---|---|---|---|---|---|
| C1 (panel 176) | calibration | 6429 | 8543 | +182 | `074e144b0f5efd6d` |
| A0 | items 1, 2, 4, 6 and the lease clause, **no success clause**, grammar inline | 6485 | **8608** | +247 | `6880c7071285d764` |
| A0 `Life` | the same, grammar with `Life = ( "acquires" \| "retains" ) ident { "\|" ident } .` | 6465 | **8579** | +218 | `45047d0f71361883` |
| A0 `Releasers` | the same, grammar with `Releasers = ident { "\|" ident } .` | 6487 | 8602 | +241 | `0b8b71415ea1213c` |
| **A-alpha** | A0 plus the clause on the parameter, `transfers json_object_put on 0`, inline | 6542 | **8679** | +318 | `6f938feb640c022e` |
| **A-beta** | A0 plus the clause on the result, `-> i32 when 0`, inline | 6539 | **8678** | +317 | `61f06a4e0ed5dfa8` |
| **A-alpha `Life`** | alpha, `Life` grammar | 6522 | **8650** | +289 | `4bfb409448a64cfa` |
| **A-beta `Life`** | beta, `Life` grammar | 6519 | **8649** | +288 | `4cb080fa65d7e1e6` |
| A-alpha without A2 | prices A2's sentence at +32, as panel 176 found | 6517 | 8647 | +286 | `036384cc1c81c9dc` |
| alpha-ext, inline | the clause also covers `retains` on a parameter (`"retains" … [ "on" … ]`) | 6547 | 8686 | +325 | `0013d9b814591ee9` |
| alpha-ext, `Releasers` | | 6549 | 8681 | +320 | `2513624717621cdc` |
| alpha-ext, `Life` | admits `acquires … on` too, which no sentence covers | 6527 | 8658 | +297 | `7daf3c561d9b3946` |
| alpha-ext short, `Life` | *With `on 0` after it … made only when the call returns 0*; admits `acquires … on` too | 6522 | 8651 | +290 | `4dd21ab853e3db49` |
| alpha-ext short, `Releasers` | | 6544 | 8674 | +313 | `57b8765b26f7359a` |
| beta-ext, inline | *…nothing is transferred or retained* | 6542 | 8684 | +323 | `479cba1876266958` |
| beta-ext, `Releasers` | | 6544 | 8679 | +318 | `7d9731ec0b8832c5` |
| **beta-ext, `Life`** | the complete result form | 6522 | **8655** | +294 | `5f4e787d65927de7` |
| **beta-ext short, `Life`** | *With `when 0` after its result type … transfers and retains only when it returns 0* | 6513 | **8639** | +278 | `619a467e3cacb2dc` |
| beta-ext short, `Releasers` | | 6535 | 8663 | +302 | `5e50e5ed2063f4e5` |
| beta-ext short `Life` + *These five* | the declined removal | 6494 | 8613 | +252 | `7764e3704c7dc9ae` |
| O8 | item 8's optional clause, *never a `const` one*, after `char **` | 6289 | 8368 | +7 | `35b4ea3e5c3a3249` |
| O7 | item 7's optional clause, *a mark cannot depend on another argument's value* | 6293 | 8377 | +16 | `6cf8fbfb3758909e` |

**What the success clause costs by itself**, as the difference between two measured texts that share
a grammar:
- Placed on the parameter (alpha): **+71** (both grammars).
- Placed on the result (beta): **+70**.
- Covering `retains` as well:
  - alpha: **+78 inline**, and +79 with `Life`, which then also admits `acquires … on`; +72 in the
    short form;
  - beta: **+76 long** and **+60 short**, both with `Life`.
- **From C1 to A0 is +65.** That is the price of four things together: the transfer naming its
  releaser, `retains` naming a set and keeping the live entry, the abort sentence naming a crossed
  transfer, and the grammar that carries all of it.

### 1b. Question 1's sentence per route

The first block replaces *"Giving one back twice aborts, unless C has since reused its address"* in
the pristine text. The second block replaces the adopted text's abort sentence, on top of A-alpha
(8679).

| route | against pristine | Δ real | digest | on top of A-alpha | Δ over A-alpha | digest |
|---|---|---|---|---|---|---|
| **D** (as it stands) | 8361 | 0 | `0159e9b26bd998a2` | 8679 | 0 | `6f938feb640c022e` |
| **D+** (the limit said) | 8381 | +20 | `5125ddf8600292b0` | 8699 | +20 | `32f7f09b5e13fbc2` |
| M, ergonomist's II | 8370 | +9 | `1995902749b388a4` | | | |
| **M, true form** | 8399 | +38 | `627fa051210d0cd6` | 8721 | +42 | `3c14e5a4090fbb90` |
| R, ergonomist's III | 8375 | +14 | `725b46443a109d3f` | 8701 | +22 | `3c01057c3a45cd3a` |
| R, true form | 8401 | +40 | `195842df5e0465c5` | | | |
| **P** | 8404 | +43 | `78e15e8809b0824b` | 8726 | +47 | `786b0fb4483d8108` |
| P, saying what `==` gives | 8411 | +50 | `24d025803db18c35` | | | |
| **S** | 8398 | +37 | `47bb702067e57734` | 8706 | **+27** | `2cf9fe970402fc3b` |

**S is the one route that is cheaper on top of the adopted text than alone.** Its sentence merges
into the abort sentence and drops *unless C has since reused its address* for every handle except
one read from a group record's field. The same rule the ledger records applies: merging wins when
the joined sentence loses a clause.

### 1c. The recommended landings

| variant | vendored | **real** | Δ real | digest | free, and net of the floor |
|---|---|---|---|---|---|
| `Bx_Dplus`: beta-ext `Life` + D+ | 6538 | **8675** | +314 | `5d057c4e04f6aaa3` | 1565, 1505 |
| `Bxs_Dplus`: beta-ext short `Life` + D+ | 6529 | **8659** | +298 | `165aa4a1105e1f39` | 1581, 1521 |
| `Axs_Dplus`: alpha-ext short `Life` + D+ (admits `acquires … on` too) | 6538 | 8671 | +310 | `1eac504e53563d66` | |
| `Ax_Dplus`: alpha-ext inline + D+ | 6563 | 8706 | +345 | `7d70d63f010e8812` | |

**Every adopted draft is 4 to 6 times `DELTA_GATE`'s 50 vendored** (+231 to +281). The landing
commit's body names what paid, and here that can only be predictions (§ 5).

## 2. Verdict per candidate text, and the strongest reason against each

- **The set, `transfers <releaser>`, `retains`, A2 and the lease clause (A0 `Life`, +218): approve.**
  - Each rests on a measured §1.12 or §12 argument that panel 176 accepted:
    - defect 075, a crossed release that runs silent, on three platforms;
    - `xfer_*` refused at 134 under route A;
    - defect 079;
    - `xmod-lent`, read from source.
  - The strongest reason against: **not one of them has an instance in `examples/`**. The census
    below is run today.
  - So none of them has a corpus effect a Part 11 metric could measure, and each is paid for only
    by reproducers written outside the corpus.
- **The `Life` production: approve.**
  - It saves 29 real against the inline grammar for the same words, because four sites now share
    one right-hand side. Panel 176 objected to a named production (S1p) when it had one site and
    cost +2.
  - I ran the `grammar` suite on `Bx_Dplus`: **7 passed, 0 failed**. `Life` is defined and it is
    reachable.
- **The success clause, alpha (`val: Json transfers json_object_put on 0`): object.**
  - *It misses the shape beside it.* `retains` on a parameter was adopted for OpenSSL's `_up_ref`
    family. The installed man page says *"X509_up_ref() returns 1 for success and 0 for failure"*
    (OpenSSL 3.6.4, read today). Panel 176's count of 27 such functions is carried, not re-run.
  - With an unconditional `retains`, a failed up-ref leaves the live set counting a reference C
    never took, so a correct failure path aborts at exit. That is an inference from the resolution
    and the man page, **unrun**.
  - Alpha reaches that case only with a second `on` slot after `retains` (+78 inline), or with a
    `Life [ "on" … ]` that also admits `acquires … on` (+72 to +79), a form no sentence gives a
    meaning to.
  - Alpha also lets two transfers on one call name different success values. The checker would
    have to refuse that, and the text says nothing about it.
- **The success clause, beta (`-> i32 when 0`), in its form covering `retains`: approve,
  conditionally.**
  - One slot, one sentence, and it covers both effects: +76 long or +60 short, with `Life`.
  - The strongest reason against: beta cannot say that one parameter transfers only on success
    while another transfers always, on the same call. Whether any real function does that is the
    ffi seat's to find. It is not measured.
- **The shape neither placement can write** (a question, unrun):
  - `BIO_new_fp` hands its stream into a result, and its man page says it returns *"a file BIO or
    NULL if an error occurred"* (read today). Success there means a non-null result, and no literal
    `on` or `when` value can say that.
  - The live set already skips a null acquisition (`runtime/parts/alloc.c:402-405`, read). So
    *a transfer into a result that comes back null is not made* may be the rule that is actually
    missing. That is for the compiler-engineer.
- **O7 and O8: object, without prejudice.**
  - Items 7 and 8 need no sentence in the spec. § 13 already says `char **`, and item 7's limit is
    recorded in the milestone.
  - No measured reader writes either mistake from the spec. If the ergonomist writes
    `sqlite3_bind_text` with `lent` under `SQLITE_STATIC`, O7's +16 is bought.
- **Q1 D: object, as the sentence that lands.**
  - Today § 13 says *"mark the parameter `@` and the value does not survive the call"*, and says
    nothing about a dead handle handed to a call that does not consume it. Defect 088's program does
    exactly that and runs at 0.
  - A reader can take *does not survive* as a promise, and CLAUDE.md §12 makes a promise the spec
    does not keep a defect of the document. Whether readers actually take it that way is scored by
    the ergonomist's B1, run under I (the sentence as it stands).
- **Q1 D+ (+20): approve, as the floor.** It is the one sentence true of HEAD's behaviour, checked
  against the shared brief's table: a dead handle at a consuming parameter is refused, and anywhere
  else it reaches C. It lands unless a built route lands with its own sentence.
- **Q1 M: object to II, admit the true form.**
  - II is false of M as the brief states it: a copy made before the call, a loop over a binding
    declared outside it, and `finish(@b)` in a helper all get past it. II also drops the
    double-release fact, which copies keep true.
  - The true form costs +38 (+29 over II). It is still local (§1.3), but it needs the flow analysis
    that `check/consuming.hero` says the checker does not have.
- **Q1 R: object in both forms.**
  - III is false of R as the brief states it. A `borrows` handle is never in the set, so R aborts
    correct programs, and III never says so.
  - The true form (+40) has to write that into the spec: *one a `borrows` call hands back is never
    live*. That sentence refutes the route.
  - R also contradicts adopted item 4, where `retains` on a handle the program does not hold
    begins a life: json-c's borrow-then-`json_object_get` idiom is exactly a non-live handle handed
    to a call.
- **Q1 P (+43): object, until `==` is answered.**
  - P overwrites a binding that § 5 says *"binds once, forever"*. And `u1_static` prints `a == b`,
    where § 13 says *"`==` compares the address"*.
  - The sentence is true only if every observation of a dead binding aborts. If `==` returns false
    instead, § 5 and § 13 each need a sentence that nobody has priced (the `==` clause alone is +50).
  - P is the dearest of the four routes.
- **Q1 S (+37 alone, +27 on top): admissible, and the cheapest true text on top of the adopted
  text.**
  - It carries two exceptions, both in its sentence: a handle read from a group record's field,
    and whatever `==` compares.
  - Its ABI cost is the ffi-pragmatist's veto to use, not mine.
- **A route nobody listed** (reasoned, not built).
  - A fact that a handle is dead can live in five places: the value (S), the binding (M, P), the set
    (R, G), the type (A), or the syntax of the call site.
  - The last place is **E**: the consuming call empties its argument's cell, as
    `end_lease(@x)` already does in § 13.
  - E collapses into P and collides with § 13's rule that an `@` parameter on an `extern` is a C
    out-parameter, so I did not price it.

## 3. What pays: the author's word and the ledger

- The author's word of 2026-09-23, *pay all the tokens, without economising*, is recorded in panel
  176's synthesis (lines 9-16, read). I found no author word on price for this sitting.
- design.md §1.6 makes the payment rule unconditional, so the word removes the race to the cheapest
  draft and not the payment. The ledger's newest row (6282, panel 175) paid with one removal and
  three predictions scored by a reader.
- Here there is no removal to spend (§ removal above), so every form is paid by a prediction in § 5
  that names an instrument that exists today and the milestone that scores it.
- Named in the same terms, what pays each form:
  - **The set**: panel 175's registered predictions, scored at this landing.
  - **`transfers <releaser>`**: P3.
  - **The success clause**: P2 and P3.
  - **`retains`**: P2 (the `X509_up_ref` half) and panel 176's `x509_upref` prediction.
  - **A2**: panel 176's `xmod-*` prediction.
  - **D+**: P4.
  - **Every token together**: P1.

## 4. Principle 0 for each form

- **The closure list needs none of them** (the `selfhost/` grep above).
- **`grep -rE "\b(transfers|retains)\b" examples` is empty** (exit 1, run today).
- **`examples/` has five `consumes` lines, and all five are plain releases**: `sqlite3_close` twice,
  `sqlite3_finalize` twice, `curl_easy_cleanup`.
- **No file in `examples/` names** `json_object_object_add`, `cJSON_AddItem`, `_add0`, `_push0`,
  `_set0`, `_up_ref` or `json_object_get`.
- **In `tests/golden/` the one hit** for `retains` is a comment in
  `unsupported/ffi-writable-parameter.hero:4`.
- **What that does to their burden.** No corpus program can move a Part 11 metric for any of these
  forms, so each one enters on *"a measured argument the panel accepts"* (CLAUDE.md §2), and §1.12
  cannot stand in for that argument (design.md line 590).
  - Ranked from strongest to weakest:
    1. **Q1**: defect 088 is a use-after-free at exit 0, which is §1.12 itself.
    2. **The set and `transfers <releaser>`**: defect 075, measured on three platforms.
    3. **`retains`**: defect 079 and the critic's runs.
    4. **A2**: `xmod-lent`, read from source and not run.
    5. **The success clause**: one emulation in which one emitted line was edited per program, with
       a zero-spec-token alternative, the header shim, that nobody has priced.
  - The success clause's burden is met only provisionally. It becomes corpus evidence the day the
    historian's prediction scores: the first transfer that `examples/` marks is one that happens only
    on success.

## 5. Predictions, each with an instrument that exists today

- **P1 (tokens).** If `Bx_Dplus` lands as written, `heroes measure --refresh` reads **8675 ± 8**,
  or 8659 ± 8 for `Bxs_Dplus`. If the grammar is inline instead of `Life`, it reads **at least +25
  above** those figures. Instrument: `measure`. Scored at the landing, in M-agreed-retention.
- **P2 (§1.2, the reader).** Given the landed spec alone, the llm-ergonomist seat does two things
  correctly on its first attempt:
  - it binds `json_object_object_add` with success value `0` and a failure path that releases `val`;
  - it binds `X509_up_ref` with `retains X509_free` and success value `1`.

  Instrument: the ergonomist seat. Scored at the landing's reader test, then at M-thesis-harness.
  Either binding written wrong, or written where the grammar refuses the clause, scores the clause
  as not having bought its tokens.
- **P3 (run).** With the clause built, panel 176's critic's programs behave as follows:

  | program | expected |
  |---|---|
  | `jsonc_failed_add_t`, the correct failure path | **0** |
  | `jsonc_failed_leak_t`, the leak | **134 at exit**, *"1 C handle(s) never given back"* |
  | `jsonc_ok_add_t` | 0 |
  | `xfer_jsonc_t` | 0 |

  All on the three POSIX legs. Instrument: `heroes run`. Scored at the landing.
- **P4 (D against D+, this sitting).** In the ergonomist's task B1, read under I, the reader expects
  defect 088's program to be **stopped by the language** (at check, or by an abort before C). If
  instead it predicts that the program reaches C, D is not misleading, D+ has bought nothing, and
  its +20 is unpaid. Instrument: this sitting's ergonomist report.

## 6. Conditions that would change my verdict

- **I move to alpha** if the ffi seat finds a real function that, on one call, transfers one
  parameter only on success and another always. I also move if the ergonomist writes beta wrong
  where alpha comes out right.
- **I object to beta as well**, in favour of the shim with no clause at all, if the compiler-engineer
  prices the clause above panel 176's `transfers` itself, or if no library the ffi seat binds has
  a success-only transfer that a program reads after it fails.
- **I approve R** only if the runtime records borrowed handles, at a price somebody has measured.
- **I approve P** once `==` on a dead binding is defined and § 5's *binds once* is shown to stay
  true.
- **I veto** only above 10180 real (10240 less the 60-token floor). The dearest text measured is
  8726.

## 7. Two findings for the record

- **Panel 176's warden wrote that any draft touching `Member` or `CParam` "turns the `grammar`
  suite red until `heroes grammar` prints the same productions" (its § 4 item 7). Measured false
  today.** `heroes grammar` reads the productions out of `spec/heroes-spec.md` at run time
  (`selfhost/cli/grammar.hero:105`). I placed `Bx_Dplus` in the copy's spec, and it printed `Life`
  and passed 7 of 7. The suite's own comment says *"Nothing in this repository compares a production
  to the function that reads it."* So **nothing stops the new productions landing ahead of the
  parser**. Keeping the spec and the parser in one commit at the landing is discipline, not an
  instrument.
- **`spec` on `Bx_Dplus`: 16 passed, 4 failed**, the four being `budget`, `spendable`, `real` and
  `ledger`, which are the pins that a moved count trips. Every structural check passed. The
  pristine copy reads 20 passed and 0 failed, in `real 30.58`, `user 28.06`.

## 8. Unrun, and named

- No compiler implements any candidate text. Every behaviour claimed above is what the sentence
  says, not what a build does.
- The retains-on-failure consequence, the collision of E with `@`, and the `==` question under P
  and S are all reasoned, not run.
- The 27 `_up_ref` functions, the critic's json-c runs and the success values are carried from panel
  176. I re-read only the man pages for `X509_up_ref` and `BIO_new_fp`, OpenSSL 3.6.4.
- The first-attempt rates belong to the ergonomist. A single attempt is not a rate.

---

## Appendix A: the candidate texts in full

### A.1 The recommended landing, `Bx_Dplus` (8675 real)

This is the diff from pristine. The short beta form `Bxs_Dplus` differs only in the success sentence
(A.3).

```
334c334,335
< and what a `ptr` points at. A C out-parameter is an `@` parameter, and what it
---
> and what a `ptr` points at. Every declaration of one C function in a program carries the same marks on the
> parameters and results they share at one type. A C out-parameter is an `@` parameter, and what it
376c377
< cell, and a lease nobody ends, like a handle nobody consumes, aborts when
---
> cell, and a lease nobody ends, like a handle still owed, aborts when
383,388c384,397
< parameter says the call ends that value's
< life, so passing one the function borrowed is an error: mark the parameter `@`
< and the value does not survive the call. `acquires sqlite3_finalize` after a result
< or `@` out-parameter reaching a handle says the call begins that handle's life and names
< the one that ends it, which the program owes it. Giving one back twice aborts,
< unless C has since reused its address. `borrows` says the call hands
---
> parameter says the call ends that value's life, and `transfers json_object_put` that
> it hands the life to another value, which ends it with that call; either way passing
> one the function borrowed is an error: mark the parameter `@` and the value does not
> survive the call. `acquires sqlite3_close | sqlite3_close_v2` after a result or `@`
> out-parameter reaching a handle says the call begins that handle's life and names the
> calls that may end it, one of which the program owes it unless it is transferred, and
> `retains json_object_put` after a result or a parameter that the call adds a reference
> to that handle, owing one more release: a reference to a live handle must share a
> releaser with its life, and one the program does not hold begins a life. A call that transfers or retains only when it succeeds names the
> result that means success after its type, `-> i32 when 0`, and on any other result
> nothing is transferred or retained. Ending a
> handle with a call its life does not name, transferring it towards one, or ending it
> more often than it was taken, aborts, unless C has since reused its address, and nothing checks one
> handed to any other call after its life ended. `borrows` says the call hands
390c399
< one back says which it is. `consumes`, `acquires` and `borrows` mark only a value
---
> one back says which it is. `consumes`, `transfers`, `acquires`, `retains` and `borrows` mark only a value
401c410
<                [ "->" Type [ "owned" ident ] [ "acquires" ident | "borrows" ] ] NEWLINE
---
>                [ "->" Type [ "when" ( integer | "true" | "false" ) ] [ "owned" ident ] [ Life | "borrows" ] ] NEWLINE
405c414,415
<              [ "owned" ident ] [ "consumes" | "acquires" ident | "borrows" ] .
---
>              [ "owned" ident ] [ "consumes" | "transfers" ident | Life | "borrows" ] .
>     Life   = ( "acquires" | "retains" ) ident { "|" ident } .
```

### A.2 Alpha as briefed (`A_alpha_Life`, 8650)

- Everything as A.1, without the beta sentence, and with this sentence after *survive the call.*:

  > A transfer made only when the call succeeds names the result that means success, `val: Json
  > transfers json_object_put on 0`, and on any other result it is not made.

- The grammar: `CParam` carries `"transfers" ident [ "on" ( integer | "true" | "false" ) ]`, and
  `Member` carries no `when`.
- The abort sentence ends at *reused its address.* (D).

### A.3 The success sentences, as priced

- **alpha**: *A transfer made only when the call succeeds names the result that means success,
  `val: Json transfers json_object_put on 0`, and on any other result it is not made.*
- **beta**: *A call that transfers only when it succeeds names the result that means success after
  its type, `-> i32 when 0`, and on any other result nothing is transferred.*
- **alpha-ext**: *A transfer or a reference made only when the call succeeds names the result that
  means success, `val: Json transfers json_object_put on 0`, and on any other result it is not
  made.*
- **beta-ext**: *A call that transfers or retains only when it succeeds names the result that means
  success after its type, `-> i32 when 0`, and on any other result nothing is transferred or
  retained.*
- **alpha-ext short**: *With `on 0` after it, `val: Json transfers json_object_put on 0`, a
  transfer or a reference is made only when the call returns 0.*
- **beta-ext short**: *With `when 0` after its result type, `-> i32 when 0`, a call transfers and
  retains only when it returns 0.*

### A.4 Question 1, the sentence per route

**Replacing D in the pristine text:**
- **D**: Giving one back twice aborts, unless C has since reused its address.
- **D+**: Giving one back twice aborts, unless C has since reused its address, and nothing checks
  one handed to any other call after its life ended.
- **M (II)**: A handle handed to `consumes` may not be used again in the function that handed it;
  using it is an error.
- **M (true)**: A binding handed to a consuming parameter may not be read again in that function
  until it is written again, though a copy made before the call may, and giving one back twice
  aborts, unless C has since reused its address.
- **R (III)**: Handing a handle to any call after the call that ended its life aborts before C runs,
  unless C has since reused its address.
- **R (true)**: A handle of a type any `extern` consumes, handed to any call when it is not live,
  aborts before C runs, unless C has since reused its address, and one a `borrows` call hands back
  is never live.
- **P**: A binding handed to a call that ends its life is dead, and handing it to any call aborts
  before C runs; a copy made before the call is not, and giving one back twice aborts, unless C has
  since reused its address.
- **P (with `==`)**: A binding handed to a call that ends its life is dead: handing it to any call
  aborts before C runs, and it equals no handle; a copy made before the call is not, and giving one
  back twice aborts, unless C has since reused its address.
- **S**: A handle carries the acquisition that began its life, so handing it to any call after that
  life ended aborts before C runs, even where C has reused its address; one read from a group
  record's field is not checked.

**On top of the adopted text, replacing its abort sentence** (*Ending a handle with a call its life
does not name, transferring it towards one, or ending it more often than it was taken, aborts,
unless C has since reused its address.*):
- **D+**: the same sentence, then *…, and nothing checks one handed to any other call after its life
  ended.*
- **M**: the same sentence, then *A binding handed to a call that ends or transfers its life may not
  be read again in that function until it is written again, though a copy made before the call
  may.*
- **R (III)**: the same sentence, then *Handing one to any call after its life ended aborts before C
  runs, too.*
- **P**: the same sentence, then *A binding handed to a call that ends or transfers its life is
  dead, and handing it to any call aborts before C runs; a copy made before the call is not.*
- **S**, replacing the whole sentence: *Ending a handle with a call its life does not name,
  transferring it towards one, ending it more often than it was taken, or handing it to any call
  after its life ended aborts before C runs, even where C has reused its address, unless it was read
  from a group record's field.*

## Appendix B: evidence files

- `<scratchpad>/177-spec-warden/work/results.tsv`: every measurement, in the order taken.
- `<scratchpad>/177-spec-warden/work/variants.py`: every draft, as exact (old, new) pairs.
- `<scratchpad>/177-spec-warden/work/price.py`: the harness. It restores the pristine text and
  asserts its sha256 after each run.
- `<scratchpad>/177-spec-warden/work/variants/*.md`: each candidate spec in full.
- `<scratchpad>/177-spec-warden/work/diffs.txt`: the diffs of the five recommended or briefed texts.
- One refresh failed on a network timeout (`curl` exit 28) and was re-run. The number above is from
  the re-run.

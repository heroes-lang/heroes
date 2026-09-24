# Panel 176, spec-warden

Written out by the coordinator on 2026-09-23 from the seat's final message; the
seat has no file-write tool. Verbatim except for two mechanical changes: the
session's absolute scratchpad path is shortened to `<scratchpad>`, and the
HTML entities the transport added (`&lt;`, `&gt;`) are written as the
characters they stand for.

The ceiling was reached by grep: design.md §1.6 (lines 253–314) says **10240 tokens on `claude-opus-5`** through `count_tokens`, and says the payment rule is **unconditional**. Every number here comes from `heroes measure spec/heroes-spec.md` (vendored) and `--refresh` (the real count), run in my copy `<scratchpad>/176-spec-warden/`, built from the seed at `a747e5a2`. Each draft was applied alone to that copy's spec, measured, and reverted. The pristine file, my copy and the trunk's spec hash identically at the end (`eda63482…`).

**Baseline, reproduced:** 6282 vendored, **8361 real**, digest `0159e9b26bd998a2`. That leaves 1879 free, or 1819 after the 60-token FFI floor (panel 030 R3). A calibration draft adding one word read +1 vendored and +2 real with a new digest, so `--refresh` recomputes rather than echoing the pinned figure.

---

- `verdict`: **object** to each draft as the brief words it. **Approve** the route-A base (S2a) and one merged combination, C1, under the condition below. **No veto.** Not provisional: every count is on the real instrument.
- `section`: design.md §1.6 and §1.2; §1.3 (against V2); §1.12 as the Principle 0 basis; CLAUDE.md §12 (a refusal is held to the same standard as a feature).
- `spec_token_delta`: C1, the combination I judge likeliest, goes from 8361 to **8543 real (+182)** and from 6282 to 6429 vendored (+147), digest `074e144b0f5efd6d`. The same text appended instead of merged reads +206.
- `removal`: one clause, *like a handle nobody consumes* rewritten to *like a handle still owed*. It is **−3 real** in all three pairs I measured, and it is the only wording true under both V1 and R1. Nothing else in §13 comes out without falsifying a rule the checker enforces. So almost nothing is removed, and that is a problem: the other ~185 tokens must be paid by predictions.
- `needed_for_self_hosting`: **no**. Three `selfhost/` files declare `extern` groups, and no member line carries `consumes`, `acquires` or `borrows` (grep).
- `argument`: Each draft as worded makes a sentence false or leaves a real shape unwritable. R1 covers results only, so none of OpenSSL 3's 29 `int *_up_ref(T *)` can be declared. Its *already live* excludes json-c's documented borrow-then-`json_object_get` idiom. A2 is false of the tree's one legal repeat. V3 cannot hand a life to a result (`BIO_new_fp`). V2 is cheapest in the spec (+27 real under V1 in the full combination) but costs +552 vendored in one full json-c binding, and a transfer missing from one creator's set shows up only as a run-time 134. The merged true combination costs 182. It pays under §1.2 only if a missing `transfers` is a compile error.
- `prediction`: at panel 176's landing in M-agreed-retention, with V1 adopted, `xfer_jsonc.hero`, `xfer_cj.hero` and `xfer_ssl.hero` as filed (the transfer written `consumes`) are refused by `heroes check` at **exit 1 with a diagnostic naming `transfers`**, not first by `run` at 134. The same three written with `transfers` run at 0. The instrument is a `check`/`run` transcript, which exists today. Second, on tokens: if C1 lands as priced, `measure --refresh` reads **8543 ± 8**; if it lands appended it reads **≥ +200**.
- `condition`: I approve C1 if the checker makes the missing transfer word a compile error (the prediction above), R1 lands in its true form, and A2 lands as *at one type* together with an answer to Question 3. I move to V2 if the llm-ergonomist measures V2 at equal or better first-try correctness on a binding with at least 2 creators and 2 adders, and the ffi seat shows real bindings rarely exceed two such pairs. The veto fires only above 10180 real; the dearest draft I measured is 8567.

---

## 1. Verdict per draft

Every count is real, from `--refresh`, against 8361; vendored against 6282.

| draft | what I applied | vendored | **real** | verdict | what pays it |
|---|---|---|---|---|---|
| **S1** | `"acquires" ident { "|" ident }` inline in `Member` and `CParam` | +8 | **+14** | part of S2a | reproduces the brief's +14 |
| S1p | the same as a named production `Acquires` | +9 | +16 | object: 2 dearer, and `grammar` would need `heroes grammar` to print a new production | none |
| **S2a** | S1, plus the prose example becomes the fence's own pair, *names the calls that may end it, one of which the program owes it; ending it with any other aborts* | +27 | **+36** | **approve**: route A's own text, needed under every route | panel 175's registered predictions, scored at this landing: ffi-pragmatist's `sqlite_v2.hero` (134 with one name, 0 with two) and compiler-engineer's `xacquires` 0/0 B and ≤30 `selfhost/` lines |
| S2b | S1 plus the same clause appended, keeping `sqlite3_finalize` | +36 | +53 | object: 17 dearer for the same facts | none |
| **V1** as written | ergonomist's text plus `"transfers"` in `CParam` | +59 | **+72** | **object**: cannot land alone. Its text says *one of the calls its `acquires` named* while its grammar still names one. It leaves the handle-only list and the lease sentence untrue | none |
| V1 + S2a + list | the smallest version that is true of its paragraph | +90 | +114 | V1's share is +78 over S2a | my prediction |
| **V2** as written | ergonomist's text plus S1 | +32 | **+47** | **object** (§1.2, §1.3): +11 over S2a in the spec, but transfers multiply in every binding (§ 3) | none |
| V2, keeping *owes* | the dropped *which the program owes it* put back | +35 | +50 | (the literal V2 loses a debt the document states nowhere else in the acquires sentence) | |
| **V3** as written | ergonomist's text plus `[ "into" ident ]` | +42 | **+56** | **object**: cannot hand a life to a result (§ 3); makes :383's *ends that value's life* an overgeneralisation | none |
| V3 + S2a | | +69 | +92 | | |
| **R1** as written | result mark `"retains" ident` | +54 | **+64** | **object**: results only, *already live*, and leaves :387 false (§ 3) | none |
| R1, literal made true | plus *more often than it was taken* and the list | +63 | +74 | still results only | |
| R1, named, on a result or parameter, *live or borrowed*, true, plus lease | the form I approve | +62 | **+74** | **approve** | −3 removal, plus a prediction: `refcount.hero` exits 0 and `getter_wrong_repaired.hero` stays 134 before C, at the landing |
| R1, bare word | *`retains` … already live* | +51 | +58 | **withdrawn by me**: it has no releaser to begin a life on a borrowed handle (json-c, § 3) | |
| **A2** as written | ergonomist's sentence, at :334 or :331 (+29 at both placements) | +22 | **+29** | **object**: false of `twoarity` (§ 3) | none |
| A2, *at one type* | *…on the parameters and results they share at one type* | +25 | **+32** | **approve**, only with a Question 3 answer | prediction: `xmod-lent` and `xmod-owned` are `check` exit 1 and `twoarity` stays `check` 0, at the landing |

## 2. The combinations, merged against appended

All six are measured, not summed. Additivity held exactly where I tested it: V1 plus S2a read 108 = 72 + 36, and V3 plus S2a read 92 = 56 + 36.

| combination | vendored | **real** | total | digest |
|---|---|---|---|---|
| C1 **appended**: S2a, V1 literal, R1 (bare), list, A2 *at one type* | +168 | **+206** | 8567 | `66d82f625b94836d` |
| C1 merged (R1 bare, before withdrawal) | +132 | +161 | 8522 | `3b8be2841d5c1ef6` |
| C1 merged + lease | +132 | +158 | 8519 | `2eaca7c047a525da` |
| **C1 final**: set, V1, R1 named on a result or parameter, A2 *at one type*, lease | **+147** | **+182** | **8543** (measured twice) | `074e144b0f5efd6d` |
| C1 final without A2 | +122 | +150 | 8511 | `35fe4cf50d335bf7` |
| **C2 final**: the V3 route, same R1, A2 and lease | +146 | **+184** | 8545 | `031698c300e20ef1` |
| **C3 final**: the V2 route, same R1, A2 and lease | +123 | **+155** | 8516 | `561c2b216889e224` |

- **Merging pays 22% here** (206 to 161 on the same content). That matches the rule the ledger records: merging wins when the joined sentence loses a clause. Here it loses V1's *the program owes nothing more for it, and no `acquires` names the call*, a rule the checker would otherwise have to enforce.
- **C1 final leaves 1637 free** net of the floor. It is **over `DELTA_GATE`'s 50 vendored by 97**, so the landing commit's body must name what paid.
- Line numbers below refer to `spec/heroes-spec.md` at `a747e5a2`. The applied C1 text is the diff of `pristine.md` against `C1_final.md` in `<scratchpad>/176-sw-work/`. The drafts are `variants*.py` there, and the harness is `price.py`.

## 3. The strongest reason each draft is wrong

**V2: cheapest in the spec, dearest in every binding (§1.2).** I wrote the full json-c surface under both routes, identical except for the marks: 12 creators and 5 adders, reproduced from `/opt/homebrew/include/json-c/json_object.h`.
- `heroes measure` reads **498 vendored under V1 and 1050 under V2, +552 in one binding.** These are lower bounds, because `--refresh` refuses every path but the spec.
- That is 9.2 vendored tokens per added name (552 / 60). V2's spec saving over V1 is 27 real, so V2 loses on tokens alone once a program binds about three creator/adder pairs. That break-even is an estimate, because the binding cannot be counted on the real instrument.
- The omission shapes differ too. Under V1, a missing `transfers` is a declaration-level fact the checker can refuse: a `consumes` that no `acquires` names. Under V2, a transfer listed on one creator and not another is visible only as a run-time 134 on the path that reaches it. The first half is an inference, which is why it is my prediction.
- §1.3 as well: under V2, `cJSON_AddItemToObject`'s own line is identical to `cJSON_Delete`'s. The locality veto is the ergonomist's to hold.

**V3: cannot hand a life to the result.** `BIO *BIO_new_fp(FILE *stream, int close_flag)` is at `openssl@3/include/openssl/bio.h:729`, and `BIO_new_fp(3ssl)` says *"Setting the BIO_CLOSE flag calls fclose() on the stream when the BIO is freed."* The receiver is the result, not a parameter `into` can name. The transfer is also chosen per call by a flag, which is Question 3's shape arriving inside Question 1, so any Q2 rule forbidding one module per mode reaches it too. That last point is unrun: no compiler implements V1 or A2. V3's best argument is its precedent, since `counted_by n` already names a sibling parameter.

**R1 as drafted: three gaps, all measured from installed headers.**
- **It covers results only.** OpenSSL 3's headers have 31 distinct `*_up_ref` names; my grep captured 29 declarations, and **all 29 return `int`** and add the reference to their parameter. `X509_up_ref(3ssl)` says *"increments the reference count of a."* None of them can carry a result mark.
- **Its *already live* excludes json-c's own idiom.** `json_object.h`, above `json_object_object_get`, says: *"do not do json_object_put unless you have done a json_object_get"* and *"make sure you have first gotten shared ownership through json_object_get."* A borrowed handle is not in the live set, so the true R1 names its releaser and drops *already live*. That is why I withdrew my bare-word form, although it was 6 cheaper.
- **It leaves :387's *Giving one back twice aborts* false** of `refcount.hero`, where two give-backs are correct.
- Open question for the compiler-engineer: what happens when `retains` names a releaser outside the live entry's set? It is Question 2's disagreement arriving inside one module.

**A2 as drafted is false of the one repeat that must stay legal.** `tests/golden/surface-fixtures/twoarity/` declares `printf(format: cstr lent, value: i64)` and `printf(format: cstr lent, value: cstr lent)`.
- That is **one arity, not two**, as the shared brief says. Both parameters are shared by name and position, and `value`'s marks differ.
- The difference is **forced**: `lent` on an `i64` is `error[lent_shape]`, exit 1 (measured). The fixture `check`s 0 today.
- *At one type* makes the sentence true of it, for +3.
- On Principle 0, A2 has zero instances in the tree besides this fixture. That census is the brief's and I did not re-run it. Its basis is §1.12 on `xmod-lent`, where `i2.h:5`'s `keep` stores its argument (`kept = s`), so that module's `lent` is false and a lend there leaves C a dangling pointer. That is read from the source, not run: nothing reads `kept`, so no sanitiser would see it.
- The milestone file records that an unmarked `text` plus a lease is true of both of `sqlite3_bind_text`'s pointer modes. So A2 would force the lease path there rather than make it unwritable. `BIO_new_fp`'s two modes have no such shared-true declaration: that is an inference, unrun.

## 4. What § 13 says today that these drafts would make false

Line numbers are at `a747e5a2`.
1. **:386–387** *names the one that ends it, which the program owes it*: false under every route once the set lands. S2a rewrites it.
2. **:387–388** *Giving one back twice aborts, unless C has since reused its address*: false under R1. It becomes *more often than it was taken*.
3. **:390–392** *`consumes`, `acquires` and `borrows` mark only a value that reaches a handle…*: panel 175 landed this an hour ago. Under V1 or R1 it no longer lists every handle mark, and a reader cannot tell whether `transfers` or `retains` may mark a non-handle. Naming them measured +6.
4. **:376** *a lease nobody ends, like a handle nobody consumes, aborts*: under V1 a transferred handle is one nobody `consumes`, and it must not abort. Under R1 a handle consumed once with a reference outstanding must abort. *Like a handle still owed* is true under both, and −3.
5. **:383–384** *`consumes` … says the call ends that value's life*: under V3, `consumes into` does not end it.
6. **:389–390** *where any `extern` consumes a handle type every call handing one back says which it is*: true under V1 and R1 only if `bindings_say_which` is extended to `transfers` and `retains`. That is a question for the compiler-engineer; unrun.
7. The fence at :341 stays true as a one-name set. Every draft that touches `Member` or `CParam` turns the `grammar` suite red until `heroes grammar` prints the same productions, so the spec and the compiler must move in one commit.

## 5. Principle 0 and the payment rule

- **The compiler needs none of these** (grep above). The set grammar and the consumer vocabulary rest on §1.12: defect 075 is a silent wrong release that route A refuses before C runs. They are the price of making that repair without refusing correct programs (xfer_cj, xfer_jsonc, xfer_ssl and `sqlite3_close_v2` all read 134 under route A), which is CLAUDE.md §12's standard. R1 rests on §1.12 and defect 079; panel 175's critic measured that the per-address alternative turns `getter_wrong_repaired` into a raw double free (133, 0 B). A2 rests narrowly on §1.12 via `xmod-lent`. **No draft has a measured Part 11 effect**; the ergonomist's run this sitting is the first reader measurement.
- **This sitting owes the payment rule regardless.** §1.6 (lines 305–314) makes it unconditional. Panel 175's own file (lines 12–16) says the author's word *"removes the race to the cheapest draft"* and not the rule. **No author word on price is recorded for 176**: I searched `docs/work/DECIDE.md`, the milestone file and the newest log entry, and the only such word is 175's.

## 6. Unrun, and named

- No compiler implements any draft, so every behavioural claim here is what the sentence says, not what a build does.
- The reader effect of any spelling is the llm-ergonomist's to measure.
- The json-c binding counts are vendored lower bounds.
- The 423-declaration census is the brief's, not re-run.
- **The brief's "+29 with a consequence clause" is not found**: searching panel 175's file and reports for "+14", "+29", "14 real" and "29 real" finds nothing. The +14 reproduces here as S1; +29 matches none of my drafts (S2a +36, S2b +53). Treat it as a question, not a premise.
- No other seat had reported when I priced, so C1 is my reading of the likeliest combination. The coordinator should price the combination actually adopted from the tables above; the deltas add exactly within one paragraph.

Files:
- `<scratchpad>/176-sw-work/price.py` (the harness)
- `<scratchpad>/176-sw-work/variants.py` to `variants5.py` (every draft, exact text)
- `<scratchpad>/176-sw-work/C1_final.md` (the C1 text, applied)
- `<scratchpad>/176-sw-work/jsonc_V1.hero` and `jsonc_V2.hero` (the binding comparison)
- `tests/golden/surface-fixtures/twoarity/`
- `spec/heroes-spec.md` (lines 334, 376, 383–392, 400–405)

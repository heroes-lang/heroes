# Panel 153 — report of the spec-warden

**verdict**: Q2 **adopt**, paid by the named removal R1 (+0 real). Q1 Route A **object**. Q1 Route B **adopt** in its B2 wording, paid by a registered prediction, in its own commit whose body names the payment because it crosses `DELTA_GATE`. The `hints` sentence **adopt** (H2), paid by R2. **No veto**: the worst package measured reads 8048 tokens real against the 10240 ceiling design.md §1.6 states today (grepped this sitting; `selfhost/measure/judged.hero` `CEILING` 10240). Nothing here is provisional: every figure below is `heroes measure` run in my copy.

- `section`: design.md §1.6 (payment rule, unconditional), §1.2 (cost formula), §4.19 (*a form this document SHOWS is the form a reader writes*), CLAUDE.md §2 Principle 0.
- `spec_token_delta`: base 7974 real / 5989 vendored (re-measured in the copy, digest `2e77c4e72ce69512`; the ledger row 5989's forward prediction *reads 7974 again* HELD). Recommended landing, three commits: Q2+R1 **7974 → 7974 real** (+1 vendored); H2+R2 **−3 real** (−4 vendored, measured as Q2+H2+R1+R2 = 7971); B2 **+77 real / +58 vendored**. All three landed: **8048 real**, 2132 free net of `FFI_FLOOR` 60.
- `removal`: R1 *Unmarked pointers are never freed.* (−13 real / −6 vendored) pays Q2 exactly; R2 *A group's `constant` has no body: the header holds the value.* (−19 real / −17 vendored) pays H2. **For Route B, nothing — and that is a problem**: after R1 and R2, the only removable text left in § 13 is R3, the four examples in its first sentence (−15 real), which I do not recommend spending, so B2 pays by prediction. Panel 150's filed D2 (releaser clause, +23 real, still unlanded) is then also without a removal; I say so rather than allocate one twice.
- `needed_for_self_hosting`: **no** for both routes (0 tagged records in `selfhost/`, measured). Q2 is spec text only.
- `argument`: the ergonomist's blind task read the current fence and omitted both marks in 8 of 10 readings, 6 silent; the corrected fence, 2 of 10. Under §1.2 one silent leak is a correction round-trip of 500–2000 tokens against +13 tokens of fence, and R1 is the very sentence two of twenty readings took as permission, so removing it lowers the rewrite rate while paying the bill. Route B keeps the two record kinds the document has and puts the pointer crossing on the line; Route A adds a third kind, a marker word, and still meets `duplicate_tag` the day `hints` needs a by-value record beside the handle, so it needs B's widening anyway (an inference from the measured refusal and the rule's text). The `hints` shape already compiles: the document is merely silent.
- `prediction`: if Q2+R1 lands alone as drafted, `set -a && . ./.env && set +a && ./heroes measure spec/heroes-spec.md --refresh` prints **7974 tokens** on `claude-opus-5` with digest `2ec6dad90753bed2`, and `./heroes measure spec/heroes-spec.md` prints a vendored maximum of **5990 tokens**; any other number means the landed text is not the text priced. Scored at the commit that lands defect 043.
- `condition`: I move to Route A if the compiler-engineer shows `read()` cannot make a field-read handle refusable at a `consumes` site while a `handle`-marked record can; I withdraw the object to Route A if it is shown to bind real `hints` without a second record on the tag. I would veto Route B if it were proposed without `read` entering `selfhost/inventory.hero` in the same commit, because `spec/offered` goes red (measured).

## 1. The table: every draft alone against the base, then the packages

Base: **7974 real / 5989 vendored**. Real is `--refresh` on `claude-opus-5`; vendored is the `maximum` row (cl100k_base). Drafts are in my copy's `spec/heroes-spec.md`, restored by `git checkout` between each (0 dirty lines after the loop).

| draft | what it is, and the § 13 line it joins | real | Δ real | vendored | Δ vend |
|---|---|---|---|---|---|
| Q2 | fence gains `acquires sqlite3_close` and `consumes` on its two `function` lines | 7987 | **+13** | 5996 | +7 |
| A | Route A: *With `handle` it may list fields, read through the pointer and never written: `ai.ai_family` is C's `ai->ai_family`, a field of its own type walks a list, and a read through `nullptr` aborts.* merged onto *One with a `tag` and no fields is a handle…*; `Member` gains `[ "handle" ]` after `"tag" ident` | 8053 | **+79** | 6051 | +62 |
| B | Route B: *two records may not name one tag* rewritten in place to *a tag names at most one handle and one fielded record. `h.read()` copies the struct a handle points at, as that record, and aborts on `nullptr`; a field holding a handle walks a list.*; § 11's sentence gains `· `read` (section 13)` before *— and, written in Heroes* | 8036 | +62 | 6037 | +48 |
| Bprose / Bname | the two halves of B | 8027 / 7983 | +53 / +9 | 6028 / 5998 | +39 / +9 |
| **B2** | B plus the ergonomist's soundness clause *and is borrowed, so no call consumes it* | 8051 | **+77** | 6047 | +58 |
| H | `hints`: *and so is a record C reads by pointer, `const struct timespec *`* merged onto *A C out-parameter is an `@` parameter* | 7996 | +22 | 6004 | +15 |
| **H2** | same without `const`/`struct` in a code span: *and so is a record C takes by pointer and only reads* | 7990 | **+16** | 6001 | +12 |
| R1 | remove *Unmarked pointers are never freed.* (after the `owned` sentence) | 7961 | **−13** | 5983 | −6 |
| R2 | remove *A group's `constant` has no body: the header holds the value.* | 7955 | **−19** | 5972 | −17 |
| R3 | drop *— sockets, maths, JSON, databases —* from § 13's first sentence | 7959 | −15 | 5980 | −9 |
| Q2+R1 | | 7974 | **+0** | 5990 | +1 |
| Q2+R2 | | 7968 | −6 | 5979 | −10 |
| Q2+R1+R2 | | 7955 | −19 | 5973 | −16 |
| Q2+H2+R1+R2 | | 7971 | **−3** | 5985 | −4 |
| B2+H2 | | 8067 | +93 | 6059 | +70 |
| A+H | | 8075 | +101 | 6066 | +77 |
| Q2+B2+H2+R1+R2 | the whole sitting | 8048 | **+74** | 6043 | +54 |
| Q2+A+H+R1+R2 | the Route A alternative | 8056 | +82 | 6050 | +61 |

Real/vendored ratio on the additions runs 1.3–1.9 (Q2 1.86, B2 1.33), so the vendored +7 in defect 043 was, as it said, not the reader's number.

## 2. What the instrument said about each draft, beyond the count

- The `spec` suite runs in the copy in 27.03 s real against 25.00 s user (not waiting) and passes **20 of 20** on the base. On every draft four checks go red — `budget`, `ledger`, `real`, `spendable` — and their text says why: the pin still reads 5989/7974 for digest `2e77c4e72ce69512`. That is the copy's unrefreshed record, not the draft.
- **Route A's `handle` word is harness-clean** (16 passed, only the pin family red).
- **Route B goes red on `spec/offered`** — `read` is not in `selfhost/inventory.hero`, which the check reads as text. With one line `Builtin(name: "read", tier: .heroes)` added to that file's TEXT in the harness copy (no rebuild), B and B2 pass everything but the pin family. So the spec commit and the compiler's inventory line are one commit or the suite is red.
- **H went red on `spec/rejected`**: *these appear in the spec's code and the lexer refuses them: struct const*. Neither word may sit in a code span; H2 avoids both and is clean.
- `special` on Q2's fence: **10 passed, 0 failed**, 18.97 s real — the corrected fence compiles and runs against SQLite under the harness's own `main`, whose bare `sqlite3_close(db)` stays legal beside `consumes`.
- Today's compiler refuses the 042 pair: `heroes check` on a handle plus a fielded record on `tag addrinfo` is `error[duplicate_tag]`, exit 1, and its note's repair (*give the two types two tags*) is unfollowable for a struct with one.
- **The `hints` shape already works**: `@req: Timespec` against `nanosleep`'s `const struct timespec *` compiles and runs, the emitted call is `nanosleep(&h0_req, &h1_rem)`, prints `0` then `1000`, exit 0. (My grep of the SDK's `time.h` for `int nanosleep` found nothing; clang's acceptance of the probe is the measurement.) So H2 documents shipped behaviour and moves no code.
- Unrun: no `read()` was compiled, no `handle` record was compiled — neither exists in any compiler. The 8/10 and 2/10 rates are the ergonomist's blind task, not mine.

## 3. Verdicts

**Q2 — adopt, +0 real.** The fence is compiled by `suite_special` as a program and design.md §4.19 says a shown form is the written form; a worked example legal only in the section's weaker reading is a reader briefed into a leak the runtime cannot see (the counter arms only where some `extern` consumes). The payment is R1, a named removal measured in the commit that spends it (§1.6): the sentence two of twenty blind readings took as permission. Its content survives — `owned` says what the compiler frees, `acquires`/`consumes` say what the program owes, and the abort at `main` return catches the handle nobody consumed. Note for the coordinator: design.md §4.19's own fence carries the identical bare pair; unbudgeted, but §4.19's own sentence about shown forms applies to it, so correct it in the same commit. Answer to the sitting's question: **no**, a section's one worked example may not be incomplete on the section's own rule — that is what `special/the spec's example` exists to prove, and it passed on the corrected text.

**Q1 Route A — object.** +79 real for a third kind of record, a marker word, an unspecified stack with `partial`, and it does not reach the third leg: real `hints` need a by-value `AddrInfo` beside the handle `AI`, which is the measured `duplicate_tag` pair, so A needs B's widening on top. Dominated on §1.2 and on Principle 0.

**Q1 Route B — adopt as B2**, the dearer wording, on CLAUDE.md § 4's rule (most robust, never cheapest): the borrowed-field clause closes the one line the ergonomist found that could compile and corrupt (`freeaddrinfo(@ai)` inside the walk), and robustness outranks tokens (CLAUDE.md § Precedence 3). Three things the landing commit owes: `read` in `inventory.hero` (measured red without it); the widening of `one_tag_one_type`, which the compiler's own comment records as the **author's decision of 2026-09-13** narrowing panel 145, so ratification is the author's and not the panel's; and `emit/ffi_tag.hero`'s `record_by_tag`, which answers with the FIRST record on a tag and must learn to tell the handle from the fielded record. Principle 0: not compiler-need (0 tagged records in `selfhost/`); thesis by measured argument — panel 112 measured pointer-to-header-record taking `sqlite3.h`'s bindable callbacks from 17/106 to 82/106, `netdb.h` declares 18 struct-pointer producers, and today's compiler refuses the commonest C shape by name.

**`hints` — adopt H2**, +16 real, paid by R2. The compiler accepts it today; the document is silent, and the ergonomist's hesitation 7 is a reader guessing at exactly this.

## 4. `DELTA_GATE` (50 vendored in one commit)

Under: Q2 (+7), Q2+R1 (+1), H2 (+12), H2+R2 (−4 as part of Q2+H2+R1+R2), B (+48, but B2 is the wording to land). **Over, so the commit body must name what paid**: A (+62), B2 (+58), B2+H2 (+70), A+H (+77), the whole sitting as one commit (+54). Recommended shape: three commits — Q2+R1, H2+R2, then B2 with its compiler changes — of which only the third crosses the gate, and its body carries the registered prediction.

## 5. Payment for Route B, registered

Named removal: none left that I would spend (R3 is the reader's orientation about what is not built in). Prediction, instrument existing today, scored at the milestone that lands defect 042: with B2 landed, `tests/golden/run/fixedbugs-getaddrinfo-is-bindable.hero` rewritten to `read()` the list head and print `ai_family` for `127.0.0.1` prints **`2` on one line** (AF_INET) under `./heroes run` on Darwin and under `--sanitize` on the Linux leg, exit 0; and `./heroes check` on the 042 pair prints **0 diagnostics** where today it prints 1. Falsified by any other line, a non-zero exit, or a sanitizer report.

## 6. The one prediction with its command

`set -a && . ./.env && set +a && ./heroes measure spec/heroes-spec.md --refresh` after Q2+R1 lands alone prints `SPEC_REAL_TOKENS` **7974** tokens on `claude-opus-5`, `SPEC_DIGEST` `2ec6dad90753bed2`, and `./heroes measure spec/heroes-spec.md` prints `maximum` **5990** tokens. Run in my copy on the draft this sitting, both numbers read exactly that.

Files read, none written in the project: `/Users/joseph/Temp/heroes-lang/spec/heroes-spec.md`, `/Users/joseph/Temp/heroes-lang/docs/design/design.md` §1.2 §1.6 §4.8 §4.19 Part 6, `/Users/joseph/Temp/heroes-lang/docs/measurements/010-spec-budget-ledger.md`, `/Users/joseph/Temp/heroes-lang/docs/work/DEFECTS.md` 042–043, `/Users/joseph/Temp/heroes-lang/tests/harness/suite_spec.hero`, `/Users/joseph/Temp/heroes-lang/tests/harness/spec_text.hero`, `/Users/joseph/Temp/heroes-lang/selfhost/check/decls.hero`, `/Users/joseph/Temp/heroes-lang/docs/panel/153-reports/llm-ergonomist.md`. Drafts and logs: `/private/tmp/claude-501/-Users-joseph-Temp-heroes-lang/8d7432e4-9b44-486f-80db-fda4fdc56efe/scratchpad/drafts/` and `.../scratchpad/logs/`.
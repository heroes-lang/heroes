# Panel 147 — spec-warden

**Ceiling reached by grep at the start of this sitting, not from the brief.**
`docs/design/design.md:253-256` § 1.6 reads **8192 tokens, measured by
`claude-opus-5` through `POST /v1/messages/count_tokens`**. The brief's number
is today's number. Checked because CLAUDE.md §1 says to and because four briefs
in this project have carried a stale one.

- `verdict`: **veto (Route A) · object (Route B) · approve (Route C)** — `provisional`
  on the real deltas only, and the provisionality cannot change the outcome (below)
- `section`: **§1.0** (burden of proof), standing on **§1.6** (payment), **§1.7**
  (subtraction), **§1.12** (boundary completeness). §1.2 is invoked by the brief and
  **cannot be**: §1.6 says so in its own words — *"until metric 2 runs the formula
  remains the design rule it always was and stops being an audited one"*.
- `spec_token_delta`: **measured, vendored**: 5863 → 5943 (**+80**) Route A;
  5863 → 5971 (**+108**) Route B; +0 Route C. Live spec **7806 real**
  (`claude-opus-5`, 2026-09-13), headroom 386, FFI floor 60, **326 free**.
  Real deltas for the drafts are **unrun** — `--refresh` refuses an unjudged file
  (panel 123 R5), verified by running it. **No route breaches 8192 on any
  plausible conversion**, so the budget limb casts no veto and I say so rather
  than manufacture one.
- `removal`: **nothing — and that is a problem.** §1.7's subtraction is **0** for
  Route A and **0** for Route B. Neither moves a form from core to sugar; neither
  deletes a compiler special case. Both ADD to `selfhost/ir/own.hero`'s sweep.
- `needed_for_self_hosting`: **no**, and measured rather than asserted.
- `argument`: below, ≤120 words.
- `prediction` / `condition`: below.

---

## 1. Three premises checked, and a third one falsified

The brief asked me to find a third false premise. I found two, and the first
kills Route A.

### (a) Route A closes **0 of the 23** escaping paths the census counted

Measured from the tree at `7e6e71a8`, not argued:

- **22 of the 23** are in `examples/ledger/main.hero`, and every one acquires
  through `sqlite.prepared(...)` — a **Heroes** function in `examples/ledger/db/sqlite.hero`
  — which returns the handle wrapped in a Heroes record:
  `examples/ledger/db/sqlite.hero:271-277`, `function prepared(...) -> Statement?`
  … `return ok(Statement(handle: statement))`. The `released` mark sits on the
  extern record `CStmt` (`:63`). **The scope that acquires the `CStmt` is
  `prepared`'s body, not the scope that leaks.** Route A's own last sentence —
  *"A released handle may not be returned or stored"* — makes `:232` and `:277`
  compile errors. So: write `released` and the corpus's reference binding stops
  compiling; omit it and **nothing is checked**. Either way, 0 of 22.
- **The 23rd**, `examples/sqlite/main.hero`'s `main`, was **repaired in step 1 of
  this milestone** (record 030 §2, lines 107-114). Live exposure there: zero.

So Route A's measured yield against the census that convened this sitting is
**zero paths closed**, at +80 vendored tokens. The exposure is real; Route A
cannot reach it.

### (b) The closure-list limb is not thin, it is **structurally empty**

The brief invited a count. Run today: `selfhost/` is **60,360** lines with
**four** `extern` groups (`cli/process.hero` ×2, `cli/io.hero`, `emit/literal.hero`),
**zero** handle records — no `record X tag Y` with no fields anywhere in
`selfhost/` — and **one** acquire-and-release pair,
`hero_dir_scan`/`hero_dir_release` (`cli/process.hero:50-52,151-162`).

That pair **cannot carry Route A's mark at all**: `hero_dir_release()` is
nullary and releases a runtime-global scan buffer, not a handle. `released`
names a function the compiler calls *with the handle*. So Route A would apply
to **zero declarations in `selfhost/` today and zero after it landed**. Route B
*could* express it (`cleanup hero_dir_release()`) — but that site is already
safe, so the compiler still does not need it.

### (c) The brief's own real-token conversion is a move this project has ruled against

The brief converts at 7806/5863 = 1.331. `docs/measurements/010-spec-budget-ledger.md`
says, in its own closing section: **"No row is convertible: 1.275 is a property
of one day's mix of prose and code spans, not a factor."** The only two measured
delta conversions on record are that file's: +118 → **+151** (1.280) and +99 →
**+126** (1.273). I therefore give a *range*, not a point, and flag the estimate
as an estimate — which is why the verdict is marked provisional.

## 2. Principle 0, limb by limb

| route | limb it must stand on | satisfied? |
|---|---|---|
| **A** | closure list (§1.0) | **no** — zero handle records, zero expressible sites in `selfhost/`, and the form stays inapplicable after landing |
| **A** | thesis (measured Part 11, or an accepted Part 1 argument) | **no** — metric 2 has never run (§1.6 says so), and the Part 1 argument is refuted by §1(a): 0 of 23 |
| **B** | closure list | **no** — one pair, already protected by the runtime leak gate |
| **B** | thesis | **not today** — the Part 1 argument is live (22 of 23, §1.4's silent-bug class) but the **drafting is unsound**, §3 below |
| **C** | n/a — a refusal, held to a feature's standard (CLAUDE.md §12) | admissible **only with** the falsifier drafted in §6 |

Neither limb is met by either form. §1.0: **it waits, regardless of elegance.**
§1.12 is explicit that this is not suspended by safety: *"'it would be safer' is
not an entry ticket any more than 'it would be elegant' is."*

## 3. Route B is the better form and its draft is still wrong

The token ordering and the correctness ordering run **opposite**, which the
panel should hear plainly.

- **§1.3** kills Route A independently of everything above. `released
  sqlite3_finalize` on a record in another file changes what every exit of every
  function does. §1.3's test: *"the meaning of a line can be determined from that
  line plus the signature of the enclosing function."* Route A fails it —
  *"constructs whose meaning lives elsewhere"* is the named category to reject.
  Route B passes it: the call is written in the block that runs it.
- **§1.1** forbids buying A's 28-token saving: *"tokens win only when
  comprehension is indifferent."* It is not indifferent here.
- **§1.12 completeness** is the other half: *"any C library must be bindable."*
  Route A's *may not be returned or stored* outlaws the resource-factory shape —
  `fopen`, `sqlite3_open`, `curl_easy_init`. The sitting's own probe
  (`scratchpad/panel-147/work/probe/escape.hero`) is exactly that shape and would
  stop compiling.
- **But Route B's draft is unsound as written.** All 22 target sites need
  `cleanup sqlite.finalized(@statement)` — an `@` argument
  (`examples/ledger/db/sqlite.hero:279`). The draft says *"The call's arguments
  are read where it is written, not where it runs"*, which for `@` is the wrong
  end: copy-in/copy-out at a deferred point is unspecified, and the call reaches
  the live statement **only because the handle is a copied ADDRESS** (spec §3).
  That is Part 8 wart 20 load-bearing under a new construct. A form that works by
  leaning on an open corruption class is not a §1.12 form.

## 4. The §1.6 payment

- **Named removal**: **none exists**, for either route. I searched for one:
  `owned` frees at the call and leaves no path obligation (record 030 §1), so
  nothing there is subsumed; `lease`/`end_lease` keeps a loud runtime gate a
  static form does not replace. **§1.7 subtraction: 0 and 0.**
- **Registered prediction**: available and admissible, and the instrument
  **exists** — §1.6 names *a compile* and *a diagnostic transcript* on its own
  list. The honest prediction is §5.1. What is **not** admissible is any
  prediction naming metric 2: §1.6 records six ledger rows bought that way and
  **none ever collected**.

## 5. Predictions (falsifiable, instruments that exist today)

1. **Route A yields nothing.** Add `released sqlite3_finalize` to `CStmt`
   (`examples/ledger/db/sqlite.hero:63`) and build. Prediction: a compile error at
   `:277` (`return ok(Statement(handle: statement))`), and **0** of record 030's
   23 paths refused. Instrument: `heroes build`, which exists. Scored: before any
   spec text lands.
2. **Route B yields 22 of 23** only if the draft is changed so the deferred call's
   `@` argument is read **where it runs**. As drafted, prediction: the emitted C
   finalizes through a stale record copy and passes only because the `ptr` is
   copied by address — visible in `--emit-c`. Instrument: `heroes build --emit-c`.
3. **Budget.** When whichever route lands is refreshed, the **real** delta will be
   **1.20×–1.45×** the vendored delta: Route A **96–116**, Route B **130–157**.
   Falsified if either falls outside. Instrument: `heroes measure spec/heroes-spec.md --refresh`.
   Even at 2.0× (A=160, B=216) the ceiling holds against 326 free — **the budget
   limb cannot produce a veto at this sitting, and I decline to pretend otherwise.**
4. **Re-measure before any future sitting reopens this.** Record 030's 23 is a
   figure about **two files**, one of which was repaired mid-census. Prediction: a
   re-count at the next milestone reads **22, in one file**. Instrument: a re-run
   of record 030's method.

## 6. Part 6 row, drafted, with its falsifier (CLAUDE.md §12)

> | A scope-bound release for a C handle — `released <fn>` on a handle record, or a `cleanup <call>` statement | **refused because the measured exposure is not reachable by the form that was priced, and the form that reaches it was not drafted soundly** (panel 147, 2026-09-14; record 030 is the census). **22 of the 23 escaping paths acquire through a Heroes function in another module** — `examples/ledger/db/sqlite.hero:271-277` returns the handle inside a wrapper record — so the scope that acquires is not the scope that leaks, and a mark on the extern record refuses `:232` and `:277` by its own *may not be returned or stored* rather than closing any of them; the 23rd was repaired on 2026-09-14, so the form's measured yield on the corpus is **zero paths**. §1.7's subtraction is **zero** for both spellings: neither moves a form from core to sugar, neither deletes a compiler special case, and both add to `selfhost/ir/own.hero`'s sweep rather than reusing it. The closure-list limb is **empty and stays empty**: `selfhost/` is 60,360 lines with **four** `extern` groups, **zero** handle records and **one** acquire-and-release pair, `hero_dir_scan`/`hero_dir_release`, which is nullary and cannot carry a `released` mark at all. Half the FFI corpus argues the same way: ten threaded examples, spawn loop and join loop six lines apart, **zero** escaping paths. §1.12 refuses the cheap spelling twice over — *any C library must be bindable*, and *may not be returned or stored* outlaws `fopen`, `sqlite3_open` and `curl_easy_init`. **The falsifier, three ways**: a corpus or §1.0 closure-list program that leaks a handle where the acquisition and the escape are in the **same** scope, so a declaration-level mark can see it — the tree holds one such site today and it is fixed; **or** a Part 11 metric-2 rate at which a model omits a release **above** the rate at which it mis-writes the scope-bound form; **or** a drafting in which the deferred call's `@` argument is read **where it runs**, so the construct reaches the live handle without depending on Part 8 wart 20's copied address. Produce any one and this row is a cost argument rather than a reachability one. |

## 7. `argument` (≤120 words)

Route A is refused on measurement, not taste. Twenty-two of the census's
twenty-three leaking paths acquire inside `sqlite.prepared()` and receive the
handle wrapped in a record; Route A's mark sits on the extern type and its own
sentence forbids returning or storing it, so writing `released` breaks the
corpus's reference binding and omitting it checks nothing — **zero paths
closed**. The twenty-third is already repaired. `selfhost/` has zero handle
records, so the closure-list limb is empty and stays empty; metric 2 has never
run, so the thesis limb has no instrument. §1.7 subtracts zero, no removal
exists, and §1.3 puts Route A in the category design.md names for rejection.
Route B is the better form, drafted unsoundly for `@`.

## 8. `condition`

- **Route A**: I would withdraw the veto only on a compiled demonstration that
  `released` closes ≥10 of record 030's paths **without** refusing
  `examples/ledger/db/sqlite.hero` — which requires naming the acquisition, which
  four lines of spec do not do.
- **Route B**: my objection becomes approval on three things together — the draft
  reads the deferred call's arguments **where it runs**; a prediction registered
  naming `heroes build` plus a diagnostic transcript, scored at a named milestone
  (not metric 2); and a real refresh showing ≤ +160. There is still **no named
  removal**, so the prediction branch is the only payment open, and §1.6 records
  that that branch has collected nothing in six attempts.
- **Route C**: approved only with §6's falsifier attached. A row without one is
  the cheapest way to make the most expensive commitment (§Part 6 preamble), and
  this project just spent M-deferral-ledger finding nine expired ones.

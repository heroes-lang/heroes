# Panel 170 — spec-warden

**Every number below was produced by a command run in this session, 2026-09-20,
Darwin arm64, in a COPY of the tree at `65e10d28` with a compiler built from the
seed (`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, 3.72 s).**
The copy's spec was restored to `a271f06a…` after the last measurement and
`git status` in the main tree shows only `docs/panel/170-briefs/` and
`docs/panel/170-reports/`.

Ceiling re-grepped rather than taken from the brief: `docs/design/design.md:255`
— **10240 tokens, measured by `claude-opus-5` through
`POST /v1/messages/count_tokens`**.

---

- **verdict**: **object** — not veto. The ceiling does not bite and I will not
  pretend it does.
- **section**: design.md §1.6 (payment rule, unconditional), §1.2 (real cost =
  tokens × (1 + rewrite rate)), CLAUDE.md § 2 (Principle 0) and § 12 / CL-005
  (a refusal is held to the same standard as a feature).
- **spec_token_delta**: **measured**, not estimated. Baseline **8201 real** /
  6159 cl100k / 6032 claude-legacy, digest `6c3a27eb1b8ac830`. Table below.
- **removal**: **available and priced** — `spec § 13`'s *"and two records may not
  name one tag"*, the clause defect 072 indicts: **−11 real / −10 vendored**.
- **needed_for_self_hosting**: **no**, measured today.
- **argument**: see below, 118 words.
- **prediction**: two, both falsifiable, instrument and milestone named.
- **condition**: see below.

---

## 1. The measured baseline, run twice

`./heroes measure spec/heroes-spec.md --refresh`

| | value |
|---|---|
| real (`claude-opus-5`, 2026-09-20) | **8201** |
| `cl100k_base` | 6159 |
| `claude-legacy` | 6032 |
| vendored spread | 127 |
| digest | `6c3a27eb1b8ac830` |
| ceiling (design.md:255, grepped) | 10240 |
| FFI floor mortgaged (panel 030 R3) | 60 |
| measured against the ceiling | 8261 → **1979 free** |

The brief is correct to the token. An accident corroborates panel 169: a stale
tree at step 10 in my scratchpad measured **8154**, and 8201 − 8154 = **47** —
the lifetime sentence's price, re-derived from a document nobody prepared for it.

## 2. Every candidate spelling, priced on the instrument that judges it

Each applied to `spec/heroes-spec.md` on the real path in the copy, measured with
`--refresh`, reverted. `DELTA_GATE` is 50 **vendored**.

| # | spelling | real | Δ real | cl100k | Δ vend | defects closed |
|---|---|---|---|---|---|---|
| — | baseline | 8201 | — | 6159 | — | — |
| V0a | the handle-only sentence **alone** | 8227 | **+26** | 6181 | **+22** | 0 (pays a debt) |
| V0 | `consumes` widened to a leased `cstr` (incl. V0a) | 8264 | +63 | 6211 | +52 | 070 |
| V0m | the same, **merged** into the mark paragraph | 8279 | +78 | 6216 | +57 | 070 |
| VD | `owned <fn>` on an **input** parameter (incl. V0a) | 8275 | +74 | 6216 | +57 | 070 + an internal error |
| VA | `keeps` bare, **inside** the alternation | 8280 | +79 | 6225 | +66 | **zero — see §4** |
| VA2 | `keeps` bare, **own slot** | 8283 | **+82** | 6226 | +67 | **066, 068, 070** |
| VB | `keeps end_fn`, inside the alternation | 8297 | +96 | 6238 | +79 | **zero — see §4** |
| VB2 | `keeps end_fn`, own slot | 8300 | **+99** | 6239 | +80 | 066, 068, 070 |
| REM | delete *"and two records may not name one tag"* | 8190 | **−11** | 6149 | −10 | — |

**No route breaches anything.** Worst case VB2: 8300 + 60 = 8360 against 10240,
**1880 free**. My veto does not fire and I say so plainly rather than inventing a
budget objection. Every full route crosses `DELTA_GATE` (50 vendored), so the
commit body owes a payment line; **only V0a (+22) clears it.**

**A measured counter-example to the brief's own premise.** The brief states
*"merging beats appending by 36%, measured at panel 122 and again today."* Here
it did not: V0m, the merged wording, cost **+78** against V0's **+63** — merging
was **24% worse**. Merging beats appending when it deletes a clause; mine added
qualifiers (*"instead of the handle"*, *"`consumes` only"*) to stay true. The
rule is not a law and should not be quoted as one in the next brief.

## 3. Question 3, and the answer is yes — twice

**(a) `consumes`, read strictly, already says what defect 070's mark would say.**
`spec § 13` line 379: *"`consumes` after a parameter says the call ends that
value's life."* A C function that frees what it is handed ends that value's life.
070's reproducer becomes `function eat(s: cstr consumes)` under the spec's own
words. **Run**: `./heroes check` on exactly that → **exit 1,
`error[unread_mark]`.** The compiler refuses what the document permits.

**(b) The missing sentence is the narrowing, not the mark.** `grep -n "handle"
spec/heroes-spec.md` returns five hits — 355 (the definition), 358 (`nullptr`),
375 (*"a handle nobody consumes"*), 383–384 (`acquires … reaching a handle`), 386
(`borrows … a handle type`). **Not one says the marks are handle-only.** That
rule has been shipping since panel 150 R3 / defect 037 (2026-09-15) as a comment
in `selfhost/check/marks.hero` and a diagnostic with **no spec sentence and no
design.md Part 6 row behind it**. CL-005: a refusal is held to the same standard
as a feature. **V0a, +26 real / +22 vendored, is owed whatever this sitting
decides about retention**, and it is the only item on the table under
`DELTA_GATE`.

**(c) A third, unasked-for finding: a grammar-admitted, prose-undefined form
reaching the backend.** `CParam` admits `[ "owned" ident ]` on **every**
parameter; the prose defines `owned` only *"after a `cstr` result or a `char **`
out-parameter"*. Run:

- `function my_keep(p: cstr owned my_drop)` with a `str?` argument → `check`
  **exit 0**. The declared `cstr` is silently rewritten to `str?`.
- `function atoi(s: cstr owned free) -> i64` (real header) → `check` **exit 0**,
  `build` **exit 2**, `internal error: compiling the generated C failed`, clang
  complaining `passing 'h_0opt_f87774a' … to parameter of incompatible type
  'const char *'`, **caret on the caller's line, not the declaration**.

This is defect-070-shaped and reachable today by an author who reads the shipped
grammar. It is filed here because no seat's brief covers it.

## 4. The objection that decides the sitting: VA and VB as drafted close ZERO

`selfhost/check/marks.hero:64–84` iterates every extern parameter and calls
`refuse_unread` when `p.consumes || !p.acquires.is_err() || p.borrows`;
`refuse_unread` fires `unread_mark` unless `acquiring.handle_behind` succeeds.
**066, 068 and 070 all live on `cstr` and `ptr` parameters that reach no handle.**
So a fourth word placed inside `[ "consumes" | "acquires" ident | "borrows" ]`
is refused at precisely the parameters the defects use. The shared brief's §
*"What is the mark?"* proposes the word without saying which slot, and the
cheapest-looking spellings (VA, VB) are the ones that buy nothing.

**The shipped precedent for the right slot is `counted_by`**, and it is run:
`memchr(p: ptr counted_by n, ch: i32, n: u64)` called with `s.name.ptr()` →
`check` **exit 0**, no `unread_mark`. A CParam mark read on a `ptr` with no
handle anywhere already exists. `keeps` belongs beside it. **The own slot costs
+3 real** (VA2 8283 vs VA 8280) and it is the cheapest correctness in the table.

## 5. Question 3 of the shared brief, judged rather than restated

The counter-argument on the table is that `owned`, `consumes` and `acquires` are
already the author's words and the compiler enforces what they say rather than
auditing their truth, so an unmarked retaining parameter is no worse. **Judged:
the symmetry is false at the one property that decides.**

For the three existing marks the **omission fails safe**. Omit `acquires` and you
lose a leak check; omit `consumes` and the handle stays live and the spec's own
sentence catches it — *"a lease nobody ends, like a handle nobody consumes,
aborts when `main` returns, saying how many."*

For a retention mark the **omission fails unsafe**. The default reading of an
unmarked parameter is *C does not retain*, and when that is wrong the measured
outcome is 070's **empty stderr at exit 133** and 066's **wrong answer at exit
0**. A word whose absence is the dangerous case is not the same kind of word as
one whose absence is the conservative case — and putting it in the same grammar
alternation asserts that it is. This is why panel 169's objection bites, and it
is a different sentence from that objection.

## 6. Principle 0, re-run

`grep -rn "^extern " selfhost/` → **four groups, sixteen functions**
(`cli/io.hero`, `cli/process.hero` ×2, `emit/literal.hero`). Zero `consumes`,
zero `acquires`, zero `borrows`, zero `owned`, **zero `ptr` parameters, zero
handles**. Every `cstr` parameter — `hero_fs_exists`, `getenv`, `atof`,
`hero_run_go` — is a lend used for the duration of the call. **Not one binding
retains.**

My panel 169 count stands on re-inspection: the 14 `.lease()` and 31 `.ptr()`
hits in `selfhost/` are **all** diagnostic message text and test-fixture string
literals, not call sites.

**So the mark enters on the thesis branch, not the closure list.** Its evidence is
070's empty stderr and 066's exit-0 wrong answer — design.md §1.12 robustness,
rank 3 in CLAUDE.md § Precedence, which outranks the ceiling at rank 6. That is
why I object rather than veto. But the burden of proof is met **only by a route
that reaches the defects**, and VA and VB do not.

## 7. The route nobody listed (CL-057: the list is a measurement too)

`clang` already has the C-side word for non-retention and **the compiler already
asks clang per parameter**. Run:

- `void peek(const char *s __attribute__((noescape)))` → `clang -fsyntax-only`
  exit 0; `-Xclang -ast-dump` prints **`NoEscapeAttr`** on the `ParmVarDecl` and
  carries it in the function's own type string.
- `selfhost/cli/pointee_ask.hero`'s `record Pointee` already carries `param` and
  `index` and `cli/header_types.hero` already builds from the dump — so reading
  one more attribute is an existing seam, **not new architecture, and zero spec
  tokens**.

**And the honest half, measured**: only **12 of 3120** SDK headers carry
`noescape` (0.38%), and neither `stdlib.h` nor `sqlite3.h` is among them. So
clang cannot be the primary route — but it can **contradict** a wrong mark for
free, and its rarity is itself the argument that the Heroes declaration must
carry the fact.

## 8. §1.2: does it pay?

Today defect 070 gives the author **nothing**: `check` 0, `build` 0, empty
stderr, exit 133 nine times in ten. There is no diagnostic to read, so the
rewrite loop is unbounded rather than one 500–2000-token round trip. A `check`
refusal with a named repair pays VA2's **+82** back on the **first** avoided
incident. Per defect closed: **VA2 27 real/defect**, V0 63, VD 74, VB2 33,
VA/VB **infinite**. VA2 is the best buy on the table and VB2's extra 17 real buys
a releaser name that `end_lease` already supplies.

**Nothing on the table closes 072.** Producer identity is a different fact from
retention: retention says *C keeps it*, 072 needs *which allocator made it*. Two
mechanisms, and the sitting should say so rather than let one word appear to
cover four defects.

## 9. What I recommend, and its price

1. **V0a now, +26 real / +22 vendored** — under `DELTA_GATE`, pays a debt
   outstanding since 2026-09-15, and the only item here that needs no new form.
2. **VA2, +82 real / +67 vendored**, own slot beside `counted_by`, and
   `check/marks.hero` amended so the word is **not** swept by `refuse_unread`.
3. **Take REM, −11 real**, as the named removal: defect 072 already indicts that
   clause, so the sitting spends a sentence it was going to rewrite anyway.
4. **Not VB2.** +17 real over VA2 for a releaser name `end_lease` already gives.
5. **Fix the `owned`-on-an-input-parameter internal error** regardless of the
   route chosen; it is a shipped crash, not a design question.

Net of 1+2+3: **8272 real**, +60 floor = **8332 against 10240, 1908 free**.

## 10. Predictions

**P1 (the one that decides the sitting).** If `keeps` lands **inside** the
`[ "consumes" | "acquires" ident | "borrows" ]` alternation, then all three of
066's, 068's and 070's reproducers written with `keeps` on their `cstr`/`ptr`
parameter will report **`error[unread_mark]` at `check` exit 1** — three of
three refused, **zero defects closed** — unless `selfhost/check/marks.hero` is
amended to exempt the word. Instrument: `./heroes check` on the three
reproducers. Scored at: the **M-declared-extents close**.

**P2 (tokens).** VA2 lands at **8283 ± 6 real**, and this sitting's whole
resolution stays **under 8360 real**, leaving **≥1880** against the 10240 ceiling
after the 60-token FFI floor. Instrument: `./heroes measure spec/heroes-spec.md
--refresh` in the commit that lands it. Scored at: that commit.

## 11. Condition

I withdraw the objection if the resolution (i) puts the retention mark in its own
grammar slot rather than the swept alternation, (ii) carries V0a's handle-only
sentence, and (iii) states in its own text that 072 is **not** closed by it. I
move to **veto** only if a route is adopted whose measured `--refresh` total
passes 10240 minus the 60-token FFI floor — nothing measured today comes within
1880 of that, so I expect not to.

I mark nothing provisional: every number here is measured, not estimated.

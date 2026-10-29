# Panel 232, the spec-warden's report

Written as it goes, 2026-10-29, in `.claude/worktrees/232-spec-warden/`,
detached at the frozen head `afe983265`.

## Predictions for the blind seat

Registered before any blind session ran, from the T text read whole
(`spec/heroes-spec.md` at `afe983265`), P's three sentences
(`blind/README.md` § The two specifications), the truths, and panel 230's
blind compliance (24 of 24 answers in shape on Haiku 5.5), and nothing else.
Each prediction is a point, and a miss is the band named after it.

**Restated out of 10, with task W added**, at the coordinator's change of
design before the first session (4 tasks × 2 specifications × 2 models × 5 =
80 sessions). O, S and X were first registered out of 14 with the same
shares: O P 14, T 13 of 14; S P 12, T 2 of 14; X P 13, T 0 of 14.

| task | P: right of 10 | T: right of 10 | why, one line each |
|---|---|---|---|
| **O** | **10** (a miss below 9) | **9, 90%** (a miss below 8) | P: the sentence says it outright. T: left to right as written is every reader's default for operands, arguments and fields alike, and T's silence pushes nobody to another order; a session lost is a hedge written inside `answer`. |
| **S** | **9** (a miss below 7) | **1, 10%** (a miss above 3) | P: *wraps* removes the abort, and a rotation misreading gives the same two values here (no set bit falls off), so a loss is Haiku's arithmetic on 3 × 2^62. T: *Overflow aborts at every width* beside a shift whose parenthesis names only the count reads as an abort at the first `print` (no line printed), some sessions reading § 4's known-operand rule as a refusal. |
| **X** | **9** (a miss below 8) | **0, 0%** (a miss above 1) | P: *a literal one is refused* names the case exactly; a loss is `refused` written with lines, which the scorer counts wrong. T: § 11 says the out-of-range code aborts and nothing else, so T predicts `before` then `ends: abort`; § 4's compile-time rule is a constant body's alone. |
| **W** | **9** (a miss below 8) | **4, 40%** (a miss above 6) | P: *indices, then the value, and checks the place as it writes* is the program's three steps in order. T: silence splits three ways, the index checked as the place is reached (`index`, abort; C#'s order), both sides before the write (the truth; Java's and Go's), and the value first (`value`, `index`, abort; Python's and Rust's), the first the likeliest reading of § 10's *an out-of-bounds index aborts*. |

**What the four predict for the sentences** (the measured effect, P minus T,
in sessions of 10): **O at most 1**, **S at least 5**, **X at least 7**,
**W at least 3**. O is a ceiling task: its shape is the order every reader
already assumes, so it cannot show item 1's sentence serving the thesis
whichever way it comes out; W carries item 1's measurable half, the write's
two phases (F4).

## The instrument and the baseline, run here

- **The ceiling, read by grep** (my brief's own rule): design.md § 1.6,
  line 255, *16384 tokens, measured by `claude-opus-5` through `POST
  /v1/messages/count_tokens`*; with it, the consolidation block
  (M-buildable-structs to M-compatibility-promise) spends at most 1,500 real
  before the thesis harness has run. The ledger stood at 10,048 real on the
  raise day and reads 10,343 at row 237, so the block has spent **+295 of
  1,500** (my arithmetic on the ledger's own rows, 238 at row 7846 and 251 at
  row 7856 confirming the reading).
- **The compiler**: built in my worktree as `00-shared.md` says;
  `heroes-next --version` printed `sources 3e3c95dc246b73b3`, the tree's key
  (the first generation `00577d2a5957da82`, as F3 measured).
- **The baseline**, `heroes-next measure spec/heroes-spec.md` offline:
  claude-legacy **7,748**, cl100k_base **7,890**, maximum **7,890**; then
  `--refresh` with the trunk's `.env` loaded (key length printed, never the
  key): **10,343 real** on `claude-opus-5`, digest `46d399486a3fec77`.
  `--refresh` prints the record and writes nothing. Headroom **5,981** once
  panel 030's 60-token FFI floor is counted (F19 holds).
- **How a draft is priced**: each applied by a script to my worktree's
  `spec/heroes-spec.md` from a pristine copy, every anchor asserted to match
  exactly once, measured offline and with `--refresh`, then the pristine copy
  restored and its sha256 (`494683a6...`) checked before the next draft.

## The truths my wordings rest on, run on my compiler

Each program built by `heroes-next build` and its binary run under `timeout
20`, output through `head -c 3000`
(`.claude/worktrees/232-spec-warden-scratch/probes/`, `run.sh`).

| shape | printed | reading |
|---|---|---|
| `ys[noisy(5)] @ noisy(1)` (task W) | `index`, `value`, abort at 7:5 | the write's two phases (F4) |
| `ys[noisy(5)] @ ys[noisy(7)]` | `index`, `read`, abort at **7:43** | the value's own abort comes before the place's check |
| `rs[noisy(0)].xs[noisy(5)] @ noisy(1)` | `outer`, `inner`, `value`, abort at 10:5 | a nested place is checked at the write |
| `rs[noisy(3)].xs[noisy(0)] @ noisy(1)` | `outer`, `inner`, `value`, abort at 10:5 | **the outer index too**: nothing of the place is checked before the value |
| `bump(n: @ys[noisy(5)], by: noisy(1), extra: noisy(2))` | `place`, `by`, `extra`, abort at 10:5, the call's column | an `@` argument's place waits for **every** later argument (F26's *one was tried*, now two) |
| the same in bounds | `place`, `by`, `extra`, `3` | left to right as written |
| `peek(n: ys[noisy(5)], by: noisy(1))`, `n: i64 lent` | `place`, abort at **10:19**, the argument's column; `value` never printed | **a `lent` argument's place is checked at once, like a read**: the late check is `@`'s alone |
| `m[noisy(1)] @ noisy(2)` on a map | `key`, `value`, `2` | no check to place |
| `map`, `any`, `all`, `filter`, `find`, `fold` over `[1, 2, 3]` | `map 1 2 3`; `any` 1, 2; `all` 1; `filter` 1, 2, 3; `find` 1, 2; `fold 1 2 3` | F5 holds |
| `3 << 62`, `1 << 63`, `-8 >> 1`, `-1 >> 63`, `5 << 62`, `-3 << 62`, `-1 << 1` | `-4611686018427387904`, `-9223372036854775808`, `-4`, `-1`, `4611686018427387904`, `4611686018427387904`, `-2` | the wrap and the sign copied (F6, F28) |
| `exit(code: -1)`, `exit(code: 256)` | refused `int_out_of_range` | F7 |
| `exit(code: 255 + 1)`, `exit(code: TOO_HIGH)` | `before`, then `panic: .must() on an error: does_not_fit: ...` | F7 |
| `bump(n: @rs[noisy(3)].xs[noisy(0)], by: noisy(1), extra: noisy(2))` | `outer`, `inner`, `by`, `extra`, abort at 13:5, the call's column | a nested place lent with `@` waits too, its outer index included |
| `bump(n: @ys[noisy(5)], by: zs[noisy(9)], extra: noisy(2))` | `place`, `read`, abort at **11:55**, the read's column | a by-value read after an `@` argument aborts first |
| `pair(second: ..., first: ...)` (the brief's `probe/order.hero`), `add2(b: "xy", a: 1)` with no same-type pair | both refused `wrong_label`; the second with two `type_mismatch` beside its two `wrong_label`, four diagnostics for one mistake | *as written* is the declared order (F5) |
| `exit(code: (256))`, `exit(code: 1_000)`, `exit(code: - -256)` | each refused `int_out_of_range` | *a literal one* is narrower than what is refused |

### A finding for item 4: one call whose frame passes the stack dies of SIGSEGV in silence

`k2_two_mb_frame.hero`, twenty lines, **no recursion and no chain**: six
records each holding eight of the one before by value (`R5` is 2 MiB), built
in one function `make` that `main` calls once. Printed `start`, then **exit
139, nothing on stderr**, under this Mac's default stack (`ulimit -s` 8176
KB); under `ulimit -s 65520` it printed `start`, `1`, exit 0, so it is the
stack. The 16 MiB `R6` (`k1_one_huge_frame.hero`) dies at 139 under both
limits. The runtime says why (`runtime/parts/stack.c:662-684`): a fault is
read as exhaustion only within `HERO_STACK_WINDOW`, 1 MiB below the stack,
and a probe only when it lands less than 1 MiB below `sp`; a frame larger
than that, met with more than 1 MiB of stack left, re-raises to `SIG_DFL`.
Defect 007 (closed, issues 823, 843) repaired the frame over a page and
inside the window; I searched the issues for `HERO_STACK_WINDOW` and for
*stack exhausted frame larger* and found no other, a question rather than a
premise. **This is design.md § 1.12's class (a segfault), so `blocking` by
`.claude/rules/verification.md`'s classes, the coordinator's to file**; and
it means every wording of item 4 below, the issue's draft included, is false
on this shape today, as *recursion too deep aborts* is silent on it.

## The prices, every draft measured on the instrument

Each row is `price.py <id>` (`.claude/worktrees/232-spec-warden-scratch/`):
the draft applied to my worktree's `spec/heroes-spec.md`, `heroes-next measure
spec/heroes-spec.md` offline, then `--refresh`, then the pristine copy
restored and checked. The deltas are against 10,343 real and 7,890 vendored.
All 43 rows are in `prices.tsv` beside the script, each draft's whole text in
`drafts/<id>.md`. **Every combination measured equals the sum of its parts to
the token** (six checked), so the prices compose.

| id | home | the words (new text in *italics*) | real | vendored max |
|---|---|---|---|---|
| E0 | § 7 | P's: *An expression's parts are evaluated left to right as written; a write evaluates its place's indices, then the value, and checks the place as it writes.* | **+45** | +34 |
| E1 | § 7 | *An expression's parts are evaluated left to right as written.* | **+18** | +13 |
| E2 | § 7 | *..., and a place written with `@` is checked last: after its value, or after the call's other arguments.* | +46 | +38 |
| E3 | § 7 | E0 and *, and an `@` argument's place after the call's other arguments.* | +64 | +50 |
| E4 | § 7 | *Parts are evaluated left to right as written; a place written with `@` is checked after its value, or after the call's other arguments.* | +40 | +32 |
| M10a | § 10 | *An out-of-bounds index or slice aborts, in a place written with `@` after its value or the call's other arguments, and so does a slice that splits a character.* | **+21** | +19 |
| M10b | § 10 | *..., and so does a slice that splits a character; a place written with `@` is checked after its value or the call's other arguments.* | +23 | +20 |
| B1 | § 11 prose | *Those written in Heroes call their function on the elements in order, `find`, `any` and `all` stopping once the answer is known.* | +41 | +32 |
| B2 | § 11 prose | *`map`, `filter`, `fold`, `find`, `any` and `all` call the function on each element in order, the last three stopping at the answer that decides.* | +50 | +40 |
| B3 | § 11 prose | *`find`, `any` and `all` stop calling at the first element that decides.* | +24 | +20 |
| B4 | § 11's sentence | *written in Heroes, calling the function in order:* and *`all` (with `find`, stopping at the first that decides)* | +23 | +18 |
| B4r | § 11's sentence | B4, and `fold` *(left, ...)* loses *left*, which *in order* now says | **+21** | +16 |
| L1 | § 9 | *a call through a value names each in its own position* becomes *a call names each in its own position* | **−3** | −3 |
| L2 | § 9 | *Labels keep the declared order.* after the construction bullet | +12 | +8 |
| S0 | § 7 | P's: *Overflow aborts at every width, but `<<` shifts the bits and wraps.* | +16 | +10 |
| S3 | § 7 | *Overflow aborts at every width; `<<` wraps and `>>` copies the sign.* | **+18** | +12 |
| S7 | § 7 | *..., but `<<` shifts the bits and wraps; `>>` copies the sign.* | +26 | +17 |
| S1 | § 7 | *..., but `<<` shifts the bits and wraps, and `>>` copies the sign.* | +27 | +18 |
| S6 | § 7 | *...; `<<` drops the bits it shifts out, and `>>` copies the sign.* | +26 | +18 |
| S2 | § 7's table | *(i64 only; shift count 0..63 or it aborts; `<<` wraps, `>>` copies the sign)* | +18 | +12 |
| S5 | § 7's table | *(...; `<<` wraps)* | +8 | +5 |
| S4 | § 7 | the lost-bit route: *Overflow aborts at every width, but `<<` only where a set bit falls off the top, and `>>` copies the sign.* | +29 | +23 |
| H1 | § 13 | *handing it to C again or `==` on it aborts, and so does handing C a copy made before, unless ...* | **0** | 0 |
| H2 | § 13 | *...; a copy made before compares its address, and handing it to C aborts, unless ...* | +8 | +6 |
| (a) | § 13 | the clause kept, the runtime made to abort | 0 | 0 |
| K0 | § 9 | the issue's draft: *a call chain deeper than the stack aborts* | +5 | +4 |
| K1 | § 9 | *running out of stack aborts* | **−1** | +1 |
| K2 | § 9 | *exhausting the stack aborts* | +1 | 0 |
| K3 | § 9 | *a call that finds no stack left aborts* | +3 | +4 |
| X0 | § 11 | P's: *(ends the program; a code outside 0 to 255 aborts, a literal one is refused).* | +8 | +6 |
| X4 | § 11 | *(ends the program; a code outside 0 to 255 aborts, one written as a number is refused).* | **+9** | +8 |
| X1 | § 11 | *(ends the program; a literal code outside 0 to 255 is refused, a computed one aborts).* | +10 | +7 |
| X2 | § 11 | *(...; a code outside 0 to 255 aborts, or is refused where written as a number).* | +10 | +9 |
| X3 | § 11 | a compiler change: *(...; a code outside 0 to 255 aborts, and is refused where known, as in section 4).* | +13 | +12 |
| G1 | a § 14 | the paragraph whole, `paragraph-draft.md`'s four paragraphs | **+335** | +270 |
| G2 | § 0's opening line | *What 1.0.0 compiles, every later 1.x compiles, and it does what this document says.* | +31 | +28 |

**Together**:

| set | real | vendored max |
|---|---|---|
| **P, the blind seat's document** (E0 + S0 + X0) | **10,412 (+69)** | 7,940 (+50) |
| my words for the same three sentences (E1 + M10a + S3 + X4) | 10,409 (+66) | 7,942 (+52) |
| item 1, one home (E1 + M10a + B4r + L1) | 10,400 (+57) | 7,935 (+45) |
| items 2 to 5 (S3 + H1 + K1 + X4) | 10,369 (+26) | 7,911 (+21) |
| **the recommended set, items 1 to 5** (all of the above) | **10,426 (+83)** | **7,956 (+66)** |
| the same with item 1 in one § 7 sentence (E2 + B4r + L1 + items 2 to 5) | 10,433 (+90) | 7,962 (+72) |

After the recommended set the headroom is **5,898** with the FFI floor, and
the consolidation block stands at **378 of 1,500**.

**The paragraph's other homes**, on the vendored tables only, since
`--refresh` refuses every path but the specification and the contract (its
real price there is **unrun**): the paragraph alone 269 (185 words);
`README.md` 4,540 → **4,810 (+270)**, inserted after the `0.x` promise at
lines 131 to 133; `docs/design.md` 102,113 → **102,394 (+281)**. No ceiling
judges either (panel 123 R5).

**The recommended set passes the specification's own instruments**: applied
to my worktree, `heroes-next run tests/harness/main.hero -- ./heroes-next
spec` read **27 passed, 4 failed**, the four being `budget`, `spendable`,
`real` and `ledger`, the record pins a landing rewrites; `offered` (the
built-ins clause inside § 11's sentence), `named`, `rejected`, `inventory`,
`shape` and `anchors` passed. `unseen` read 3 and 0. `grammar` was not run:
no production moves.

## The verdicts

### Item 1, the order of evaluation

- `verdict`: **object** to P's sentence as drafted; **approve** the one-home
  route: § 7 *An expression's parts are evaluated left to right as written*
  (E1), § 10's index abort taking the write's timing (M10a), § 11's sentence
  taking the built-ins' calls (B4r), § 9's *through a value* removed (L1).
- `section`: design.md § 1.6 and § 1.2; `.claude/rules/spec-shape.md` § Where
  a rule lives (one home, *the index and slice aborts* are § 10's, merging
  beats appending); CLAUDE.md § 2.
- `spec_token_delta`: P's E0 10,343 → 10,388 (**+45 real**, +34 vendored);
  the route 10,343 → 10,400 (**+57 real**, +45 vendored), its three clauses
  the blind P also states +39, six below P's.
- `removal`: § 9's *through a value* (−3) and `fold`'s *left* (−2, inside
  B4r): 5 against 57. The rest is paid by registered predictions below, and
  the § 7 clause by none if O comes in as predicted, which the ledger row
  should say.
- `needed_for_self_hosting`: no.
- `argument`: P's sentence is silent on a shape the compiler has: an `@`
  argument's place waits for every later argument (w5, w9), so a P reader
  predicts the abort at the argument and is wrong. Its write clause sits in
  § 7 though the index abort's home is § 10. E1 with M10a is true on all
  twelve shapes run, each half where its operation lives. The § 7 clause buys
  no reader effect (O at ceiling, predicted); its ground is the promise
  (decision 2217's *A*: an order-dependent program is promised nothing while
  the order is unwritten). Labels: L1 extends § 9's half to direct calls for
  −3; L2 (+12) waits, since no instrument counts the mistake (050's
  `wrong_label` 9 of 60 were `slice`'s names).
- `prediction`: W, P − T ≥ 3 of 10 (pays M10a); O, P − T ≤ 1; landed alone,
  item 1 reads **10,400 real**; a blind re-run of W with E1 + M10a in place of
  E0 reads within 1 of E0's P count, and a task printing inside `xs.any(f)`
  reads at most 6 of 10 under T and at least 9 under B4r, both by panel 232's
  launcher at this milestone's close (pays B4r; about 4.20 USD for 30
  sessions at 0.14, a paid run the coordinator asks for).
- `condition`: W's P − T ≤ 1 → I object to the write clause as well; a shape
  where E1 + M10a is false (a `lent` argument checked late on another
  platform, say) → reword before landing.

### Item 2, `<<` and `>>`

- `verdict`: **approve** S3, *Overflow aborts at every width; `<<` wraps and
  `>>` copies the sign*; **object** to the lost-bit route's sentence (S4).
- `section`: design.md § 4.14 (both rules, with reasons) and § 1.6;
  CLAUDE.md § 12.
- `spec_token_delta`: 10,343 → 10,361 (**+18 real**, +12 vendored); P's S0
  +16; S4 +29.
- `removal`: nothing, and none is owed in kind: the sentence it amends is
  false today (`3 << 62` overflows and wraps), so this is a correction, paid
  by S's prediction.
- `needed_for_self_hosting`: no.
- `argument`: T says overflow aborts at every width, and a reader predicts an
  abort the program never meets; design.md and the compiler agree on the wrap
  and the copied sign, and the specification is the one out. S3 states both
  rules for +2 over P's S0. P's *shifts the bits* guards against a rotation
  reading task S cannot detect (both readings give its two values, `5 << 62`
  would not), at +8 more (S7, +26): the sitting's choice, priced. S4 is +11
  dearer, amends § 4.14, and as worded aborts `-1 << 1`, an exact −2, since
  every left shift of a negative drops a set sign bit (arithmetic on the bit
  pattern, unrun: no compiler implements the route).
- `prediction`: S, P − T ≥ 5 of 10; S3 lands at **10,361 real**.
- `condition`: S's P − T ≤ 2 → the clause is a correction alone and S0 or S3
  is chosen on price; the lost-bit route adopted → S4's +29 is its price, and
  § 4.14's reason (a mask naming its top flag) is answered first.

### Item 3, `==` on a handle given back

- `verdict`: **approve** either route on the budget.
- `section`: design.md § 1.6 (nothing to pay); CLAUDE.md § 12 (the
  specification wins unless the sitting rules otherwise).
- `spec_token_delta`: the clause kept and the runtime made to abort, 0; H1,
  the clause narrowed to the crossing, 10,343 → 10,343 (**0 real**, 0
  vendored); H2, adding *a copy made before compares its address*, +8.
- `removal`: none owed.
- `needed_for_self_hosting`: no.
- `argument`: the budget has no say: either route is free. H2's extra clause
  buys nothing H1 does not, § 13 saying of a handle two sentences earlier that
  *`==` compares the address*: 8 tokens of restatement. The choice is the
  runtime's (a lock and a probe per handle `==`, the golden pinning `null:
  false`), the ffi-pragmatist's under § 12's default. I did not run the handle
  case; H1's truth rests on the critic's F9 and is that seat's to confirm.
- `prediction`: H1 lands at **10,343 real**.
- `condition`: none from the budget.

### Item 4, a stack exhausted without recursion

- `verdict`: **approve** K1, *running out of stack aborts*, over the issue's
  draft (K0); **object** to landing any wording before the large-frame defect
  above is filed, every wording being false on it today.
- `section`: design.md § 1.12 (CLAUDE.md § Precedence, rank 3) and § 1.6;
  CLAUDE.md § 12.
- `spec_token_delta`: K1 10,343 → 10,342 (**−1 real**, +1 vendored); K0 +5
  real.
- `removal`: K1 is itself one real token below today's words.
- `needed_for_self_hosting`: no.
- `argument`: the issue's draft names a chain, and the shape I ran that fails
  is one call: twenty lines, no recursion, a 2 MiB record built in one
  function, SIGSEGV with nothing on stderr (k2), because the guard reads a
  fault as exhaustion only within 1 MiB of the stack's end and of `sp`.
  *Running out of stack* covers recursion, a chain, one large frame and a
  thread's smaller stack, for a token less than today. The sentence is the
  rule and the runtime has the bug (§ 12); robustness ranks the repair first,
  and a `1.0.0` promise that a program *does what the specification says*
  over a silent segfault would be false on its first day.
- `prediction`: K1 lands at **10,342 real**; k2 exits 139, nothing on stderr,
  on Darwin arm64 at `afe983265` until the window is repaired (run here).
- `condition`: a shape that runs out of stack unreported which no repair can
  reach on some platform → the sentence names only what can be promised.

### Item 5, § 11's `exit`

- `verdict`: **approve** with one word changed: *one written as a number is
  refused* (X4) instead of P's *a literal one is refused* (X0).
- `section`: design.md § 1.6; § 7's grammar (`Unary = ( "-" | "!" | "~" )
  Unary | Postfix`); CLAUDE.md § RUN IT.
- `spec_token_delta`: X4 10,343 → 10,352 (**+9 real**, +8 vendored); X0 +8.
- `removal`: none; paid by X's prediction.
- `needed_for_self_hosting`: no.
- `argument`: today's sentence is false for a literal: it says a code outside
  aborts, and `exit(code: 256)` never runs (ledger row 237 already describes
  it as saying the refusal, F19). P's *a literal one* is false in the
  grammar's own reading: `-1` and `- -256` are a minus on a literal, `(256)` a
  parenthesis, and all three are refused (run). *Written as a number* is true
  of every shape run, the computed edges (`255 + 1`, a constant) included, for
  one token. Refusing § 4's known codes (X3, +13) is a compiler change nothing
  here needs.
- `prediction`: X, P − T ≥ 7 of 10; X4 lands at **10,352 real**.
- `condition`: X's P − T ≤ 2 → the clause is a correction alone.

### Item 6, the paragraph `1.0.0` rests on

- `verdict`: **veto** in the specification, whole (G1) or as one sentence
  (G2); **approve** the README as its home, where the budget has no
  jurisdiction.
- `section`: CLAUDE.md § 2 (Principle 0); design.md § 1.6 (the document is
  the prompt); `.claude/rules/spec-shape.md` (§ 6, the one definition of
  abort, F32).
- `spec_token_delta`: as a § 14, 10,343 → 10,678 (**+335 real**, +270
  vendored); as one sentence, +31 real. README 4,540 → 4,810 vendored (+270;
  real unrun); design.md 102,113 → 102,394 vendored (+281).
- `removal`: nothing, and in the specification that is the problem.
- `needed_for_self_hosting`: no.
- `argument`: none of its sentences changes what a program written from the
  specification does or what the compiler accepts, and no task measures it:
  Principle 0 is unmet in both halves. Its third paragraph restates § 6's
  abort, refused outside § 6; its second states an absence the document's
  silence already rules. At +335 it would take 28% of the block's remaining
  1,205. The promise has a home today: `README.md:131-133` holds `0.x`'s one
  sentence, which this replaces at `1.0.0`, under `checkReadme`,
  `site/CLAUDE.md`, both editions and a publish the author is asked for.
  Still in its words: *so* joins an absence to a guarantee, and *keeps an
  older program* is universal beside its own defect exclusion (F22).
- `prediction`: the README with the paragraph reads **4,810** on the vendored
  maximum.
- `condition`: a measured effect of the paragraph on programs written from
  the specification (a blind task) → re-priced in the specification.

### Items 7 and 8

No sentence of the specification is drafted for either, so **0 spec tokens**.
One caution for item 7: § 6's *Codes are stable snake_case strings* is about a
program's own `fail` codes (`missing_key`, `not_found`, `file_not_found` ...),
which the specification already promises by naming them; it names no compiler
diagnostic, so a promise of diagnostic codes lives with design.md § 4.17's
`"schema"`, not here.

## Overall

- `verdict`: **object**, no veto on items 1 to 5; **veto** on item 6 in the
  specification.
- `section`: design.md § 1.6; CLAUDE.md § 2.
- `spec_token_delta`: P as the blind seat reads it 10,343 → 10,412 (+69 real,
  +50 vendored); the recommended set for items 1 to 5, 10,343 → **10,426
  (+83 real)**, 7,890 → 7,956 (+66 vendored); the ceiling 16384, headroom
  after it 5,898 with the FFI floor, the consolidation block 378 of 1,500.
- `removal`: −6 real named (§ 9's *through a value*, `fold`'s *left*, K1's
  token); items 2, 4 and 5 correct false sentences; the rest is registered
  predictions on an instrument that exists, and the § 7 order clause (+18) is
  bought by the promise alone if O comes in as predicted.
- `needed_for_self_hosting`: no, for every item.
- `prediction`: the recommended set lands at **10,426 real** exactly if nothing
  else lands first; the blind effects O ≤ 1, S ≥ 5, X ≥ 7, W ≥ 3 of 10.
- `condition`: the large-frame defect filed (`blocking`) before item 4's
  sentence lands; W's effect measured before item 1's write clause is called
  paid.

## Commands

From `.claude/worktrees/232-spec-warden/` at `afe983265`: `clang -I runtime
seed/heroes.c runtime/runtime.c -o heroes-stage1`, `./heroes-stage1 build
selfhost/main.hero -o heroes`, `./heroes build selfhost/main.hero -o
heroes-next`; `env -u ANTHROPIC_API_KEY ./heroes-next measure
spec/heroes-spec.md`; `set -a; . /Users/joseph/Temp/heroes/heroes-lang/.env;
set +a; ./heroes-next measure spec/heroes-spec.md --refresh`. From the
scratch, `python3 -I price.py <id> ...` (the same two commands per draft) and
`probes/run.sh <program> ...`. 44 `--refresh` calls in all, the token-counting
endpoint and no other paid call; no clock started.

## The synthesis's wordings, priced (2026-10-29)

Added at the coordinator's request after the critic's second pass
(`.claude/worktrees/232-coordinator/pricing-request.md`;
`232-reports/critic-reports.md` R1, R4, R11, R15; the ffi-pragmatist's
finding 13 for P1). Measured as every row above: each wording applied alone
to my worktree's `spec/heroes-spec.md` from the pristine copy, `heroes-next
measure` offline and `--refresh`, the copy restored and its sha256
(`494683a6...`) checked; `python3 -I price.py <id>`, rows appended to
`prices.tsv`. Eleven `--refresh` calls, 55 in all this sitting; no other paid
call, no clock. **The wordings' truths were not re-run here**: they rest on
the critic's and the ffi-pragmatist's runs.

| id | home | the words (new text in *italics*) | real | vendored max |
|---|---|---|---|---|
| D1 | § 6's table | `.default(v)`'s row: *extract or fall back; `v` is evaluated only for an error* | **+13** | +10 |
| D2 | § 7, instead of E1 and D1 | *An expression's parts are evaluated left to right as written, but `.default(v)` evaluates `v` only for an error.* | **+39** | +29 |
| H3 | § 13, line 440's sentence | *Giving one back ends it: reading it where every path has ended it is a compile error; elsewhere handing it to C again or `==` on it aborts, and so does handing C a copy made before, unless ...* | **+21** | +14 |
| H4 | § 13, instead of H3 | *Giving one back ends it: a use every path reaches after the end is refused, and otherwise handing it to C again or `==` on it aborts, and so does handing C a copy made before, unless ...* | **+19** | +13 |
| P1 | § 13, after its first sentence | *A program's own C declares no name starting `h_`, `hero_`, `Hero`, `HERO_` or `HEROES_`: those are the compiler's, and a later one may add any.* | **+59** | +46 |
| N1 | § 11 | *... because the number may not fit, failing `does_not_fit`.* | **+11** | +8 |
| M10c | § 10, instead of M10a | *An out-of-bounds index or slice aborts, the index of a place written with `@` only after its value or the call's other arguments, and so does a slice that splits a character.* | **+24** | +22 |

Reused as they stand: E1 +18, B4r +21, L2 +12 (in place of L1), S6 +26, S3
+18, K1 −1, X4 +9.

**THE SET**, E1 + D1 + M10c + B4r + L2 + S6 + H3 + K1 + X4 + P1 + N1, one
document measured once, and each swap once:

| document | real | vendored max (legacy) | headroom with the FFI floor | the block's 1,500 |
|---|---|---|---|---|
| **THE SET** | **10,556 (+213)** | **8,054 (+164)** (7,911, +163) | **5,768** | **508** |
| D2 in place of E1 + D1 | 10,564 (+221) | 8,060 (+170) | 5,760 | 516 |
| S3 in place of S6 | 10,548 (+205) | 8,048 (+158) | 5,776 | 500 |
| H4 in place of H3 | 10,554 (+211) | 8,053 (+163) | 5,770 | 506 |

The set equals the sum of its eleven parts to the token, and each swap the
set less its part plus the other (+221 = 213 − 18 − 13 + 39; +205 = 213 −
26 + 18; +211 = 213 − 21 + 19), so a twelfth row of the arithmetic is no
measurement but holds on every one taken. The headroom is 16,384 − real −
60; the block is 295 before this sitting plus the set.

**The instruments on THE SET**, applied to my worktree and restored:
`heroes-next run tests/harness/main.hero -- ./heroes-next spec` read **27
passed, 4 failed**, the four the pins a landing rewrites:

- `spec/budget`: *the spec measures 8054 and the record says 7890, a delta of
  164*;
- `spec/spendable`: `measure` no longer says *is 10403 and the check goes red
  at 16384*;
- `spec/real`: `measure` no longer prints *10343 claude-opus-5, 2026-10-29*;
- `spec/ledger`: *the ledger's newest row says 7890 and the compiler measures
  8054*.

`offered`, `named` and `rejected` passed with P1's five prefixes and N1's
`does_not_fit` in code spans. `unseen` read **3 passed, 0 failed**.

Two prices the sitting may want beside the set, arithmetic on the rows
above: D2 says in § 7 what D1 says in § 6's table for +8 more than E1 and D1
together, and P1 at +59 is the set's largest single row, 28% of it.

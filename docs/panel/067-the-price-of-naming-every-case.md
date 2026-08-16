# Panel 067 — the price of naming every case, and who was actually charging it

**Lane: full** (compiler-engineer, llm-ergonomist, spec-warden, ffi-pragmatist,
historian). Convened and synthesised 2026-08-16.
**Provisional — author ratification pending.**

**Verdict: nothing enters the language. Five seats, five independent routes,
one answer** — and the sitting's real product is not the answer but three
things it found on the way: the coordinator's number was wrong four times over,
the file that motivated the complaint carries a **porting regression rather
than a language cost**, and the FFI seat found a **live exit-0-then-abort
defect** that has nothing to do with the question.

## What was asked

The author, reading a claim of the coordinator's, asked: *"forse è un prezzo
troppo alto, forse possiamo cambiare il linguaggio per includere qualcosa in
più"* — maybe the price of `_`-forbidden-on-variants (spec:117) is too high;
maybe the language should gain a form.

Candidates put to the seats: **A** nothing · **B** a `rest` arm that still
breaks when a case is added · **C** a lookup-table form with a default,
distinguished from control flow · **D** whatever a seat proposes.

## The number, corrected four times — and the correction is the finding

| claimed | by whom | what was wrong |
|---|---|---|
| "the port is *longer* than Rust, 1129 vs 1025" | coordinator | compared code+tests against code-only; **the port is 21% shorter** (812 vs 1025) |
| 1129 lines | coordinator | it is 1129; an earlier 872 was written from memory |
| 66 padding arms in `selfhost/`, 38 in the knot | coordinator | **61** and **37** (engineer); **40** and **23** under a stricter definition (warden); **28** literal-bodied (historian) |
| "4.7% of the file" | coordinator | **0.77%** of the tree, because `_ => 0` *is itself a line* and the price is the **difference**, not the arm count (engineer). In tokens — the unit §1.2's formula actually uses — **1.10% port-wide, 2.99% in the worst file** (warden) |

**The lesson the sitting hands back to its own coordinator**: every seat that
measured found a different number from the brief, and the brief's number was the
one that reached the author. A panel briefed on a line count against a
token-denominated formula is a panel asked the wrong question.

## Verdict table

| judge | verdict | the finding that decides it |
|---|---|---|
| **compiler-engineer** | **veto B and C** · **approve D** (a code change, not a language change) | **The worst site in the file is a porting regression.** Rust's `expr.rs:35` is ONE function returning operator *and* power (`Option<(BinaryOp, u8)>`); the port split it in two, so 50 non-operator tokens are enumerated **twice** and the second table must invent an answer — it invents `.add`. **Proved dangerous, not just ugly**: changing `.percent => .rem` to `.percent => .add` — the copy-paste those ten lines invite — leaves `heroes check` at exit 0 and **46/46 tests passing**, with `%` parsing as `+`. *"§4.7's ban did not cost those 20 lines; the split did."* Merging them back (option D): **811 → 793 lines**, 46/46 green, ten padding arms gone and the rest loud `fail(…)` instead of `.add`. And the classification asked for: of 42 recoverable lines, **32 are decisions where the default is the wrong answer for the case that will be added**; the honest complaint is **10 lines, 0.18%, all in test code** |
| **spec-warden** | **veto B and C** · approve **A** | **§1.2 kills it on arithmetic.** Total lifetime saving across the whole self-hosted compiler: **840 tokens** — between **0.4 and 1.7 correction round-trips**, against a form whose **+94 spec tokens are paid on every inference** because §1.6 says the spec *is* the prompt. And: *"the proposal's own evidence refutes it"* — the Rust parser uses **33 `_ =>` catch-alls**, the port forbids every one, pays the tax in full, and is still 21% shorter. **§1.3 is breached outright**: `else => .add` means *"and every case added later, forever"*, so its meaning *"lives elsewhere in **time**, which is the one direction no reader and no model can look."* B is self-defeating — a checked `rest(9)` is a **count in code**, and the cheapest repair a model takes is to change 9 to 10 |
| **llm-ergonomist** (spec-only, blind) | **veto `_`, `else`, and case-groups** · approve a **diagnostic** change | **The cost is transcription, not decision — so move the transcription to the compiler.** Writing the table needed recall of 47 exact spellings, and the four it would get wrong (`.bang_eq`, `.terminator`, `.hole`, `.error`) are *"exactly the kinds a `_ => 0` would hide forever"*. Its proposal costs no syntax: a non-exhaustive `match` is reported with **every missing case written as an arm whose body is `???`**, one per line — the program still cannot build (a hole produces no binary), the author still decides, the end state is byte-identical. **And it found a hole in the rule**: a `{TokenKind: i64}` map with `.default(0)`, built only from spec-shown syntax, *is* the forbidden catch-all — *"I would not experience it as cheating; I would experience it as 'a table belongs in a map'"* |
| **ffi-pragmatist** | **none-needed**, no veto | **The boundary does not pay this cost and cannot be made to.** Measured: 50 `extern` groups, **zero** variants in any FFI-touching file, and **none of the three doors** into a group admits a `variant` — so a C enum arrives as `constant`s and dispatch is on `i64`, where a catch-all is **already mandatory** (exhaustiveness over `i64` is impossible). Proved on a real version bump: SQLite returned `1555` (`SQLITE_CONSTRAINT_PRIMARYKEY`) to a binding that never named it, and the `_` arm caught it. **And C already exists through the map + `.default` route, silently absorbing a new case at exit 0** — which is why the seat does not veto it: it would name an existing hole, not open one |
| **historian** (advisory) | **none-needed** | **The remedy is already shipped, and Heroes shipped it first.** 26 of 28 literal-bodied arms already use `|`, carrying **136 variant names** — the arms are a ~5× compression, and a new form would save ~22 lines, not 38. **GHC Proposal #522 "Or Patterns" (shipped 9.12.1, 2024-12-16)** gives Heroes' exact motivation verbatim and answers it with **syntax rather than a ban or a default**. **Java asked this question and answered it the other way for exactly one reason** — novel run-time values from separately-compiled code (Goetz & Bierman, 2023-05-23) — *"a match-all clause risks sweeping exhaustiveness errors under the rug"*; that sole objection is void here. **D ships Heroes' rule as language law** (`final switch`) and has not retreated. Swift's `@unknown default` is **binary-resilience**, not ergonomics, and is a *warning*. **Nobody removed a wildcard; everybody who shipped one built a detector and then found it unusable by default** (OCaml's warning 4 off by default and its creator says keep it off; Rust's lint allow-by-default; Swift's a warning) |

## Where the seats disagree, stated plainly

**One disagreement, and it is not about A.** The llm-ergonomist approves a
**diagnostic** that writes the missing arms with `???` bodies, at an estimated
+30 spec tokens. The spec-warden priced only B and C and did **not** price that
form; under panel 012 those 30 tokens still owe a named removal or an
admissible prediction. **The conservative resolution therefore adopts A alone
and queues the diagnostic separately** rather than landing an unpriced sentence
on one seat's estimate — which is the same discipline that caught this
sitting's own brief.

The historian's or-pattern finding and the ergonomist's transcription finding
point the same way from opposite ends: the cost is already 5×-compressed by a
form the language has, and what remains is typing the compiler could do.

## The resolution — provisional, author ratification pending

1. **§4.7 and spec:117 are unchanged.** No catch-all, no `rest`, no
   lookup-with-default, no named case-groups. Five seats, no seat in favour.
2. **Option D lands as ordinary work** (no spec token, no panel): merge the
   port's two operator tables back into one `binary_op(kind) -> Operator?`,
   restoring the Rust factoring the port broke. Measured −18 lines, 46/46
   green, and it deletes the `.add` invention that a one-line edit proved
   dangerous.
3. **The port is swept once** for the same shape — two functions matching one
   variant to answer one question become one function returning a fallible
   record. Three files walk `TokenKind` for **three different** questions and
   stay separate.
4. **The ergonomist's `???`-arms diagnostic is queued, not landed** (see the
   disagreement above).
5. **The refusal is falsifiable** (CLAUDE.md §12). It returns on all three of
   the warden's conditions: a closure-list construct that *cannot* be written
   without a default; a measured metric-2 effect showing the form lowers
   rewrite rate; and a named removal of ≥94 spec tokens or an admissible
   prediction.

## What the sitting found that it was not convened for

**A live defect, and it is §1.12's class** (ffi-pragmatist F7). These six all
pass `heroes check` at **exit 0** and abort at **exit 134** — *"entered
unreachable code — this is a compiler bug"*:

    [ptr]   [cstr]   {i64: ptr}   {i64: cstr}   {ptr: i64}   {cstr: i64}

Cause: `emit/descriptors.rs`'s `_ => None`, whose doc says *"which `gate.rs`
has already refused"* — **a dead premise**: the gate refuses only `Ty::Unit`.
CLAUDE.md §11's class, and the **same shape** as the descriptor defect this
port fixed hours earlier: a narrowing resting on a claim about the world, the
comment still reading as correct. Panel 029 filed three types; the fix covered
one.

**And the case named after that defect never runs its live half** (F8):
`tests/golden/unsupported/fixedbugs-container-element-reached-clang.hero`
carries `[ptr]`, `{str: ptr}` and `[[ptr]]`, but its `[()]` line fires first and
stops the build, so the four lines after it are never reached. §9's `#~`
invariant proves where a diagnostic *points*; it cannot prove the lines after
the first firing one were executed.

Both are queued as defects with the pragmatist's conditions attached.

## Predictions to score

| judge | prediction | at |
|---|---|---|
| compiler-engineer | padding stays **under 1.0%** of a ≥9000-line port and does not scale; ≥70% of *new* padding is decision rather than assertion; the D-sweep deletes **more than 42** lines; **≥1 further two-table disagreement** is found | M-selfhost-fixpoint |
| spec-warden | a mutation that adds a variant case is caught in ~100% of exhaustive programs and **0%** under `else` — *"if the caught rate under `else` measures above 0%, the form is safer than this panel judged and C is re-argued"* | M-selfhost-fixpoint |
| llm-ergonomist | applying the `???`-arms fix **never** yields a building program; **≥1** variant-keyed map with `.default` exists in the self-hosted source (the hole taken in practice) | M-selfhost-fixpoint |
| ffi-pragmatist | **zero** exhaustiveness-only arms in `selfhost/` dispatch on an `extern` result; F7 stays live because the port trips no container of `ptr`/`cstr`; after the fix all six shapes behave | M-selfhost-port · the fixing milestone |
| historian | literal-bodied arms stay **≤45** carrying **>4×** that many names, in **≤5 files**; adding one `TokenKind` case breaks **≤8** sites, each a real decision; **zero** constructs suppressing exhaustiveness exist | M-selfhost-fixpoint |

## Process notes

All five judges worked in copies with `target`/`build` removed; the tree was
frozen from briefs to synthesis and `git status` is clean. **The coordinator
briefed a wrong number for the second sitting running** — panel 066 got a dead
premise, 067 got a line count offered against a token-denominated formula — and
in both cases the seats caught it. The instrument is working; the seat that
convenes it is the one that keeps needing correction, which is worth recording
in the process notes rather than in a postscript.

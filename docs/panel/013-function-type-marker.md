# Panel 013 — the function type's marker: `fn` was both forbidden and required

Date: 2026-08-04. Trigger: found while writing the M2 type parser (`/step`,
M2 step 1). Two spec artifacts contradicted each other, and the contradiction
was not theoretical — it made a specified construct unlexable:

```
$ heroes lex apply.hero
1:24 error fn
apply.hero:1:24: error[reserved_word]: `fn` is not a word in this language — use `function`: …
```

on the line `apply = function: (f: (fn(int) -> int), x: int) -> int`. **Every
function-typed parameter in the language was unwritable**, including the
spec's own `map`/`filter`/`fold` and the M6 acceptance program's
`values.fold(0, plus)`. Five judges convened; the marker's spelling touches
surface syntax, a diagnostic class and `spec/**`, so all five had standing.

## Proposal (as put to the judges)

1. `spec/heroes-spec.md` § "Functions and calls" and design.md §4.13:
   "Function type syntax: `(fn(A) -> B)` — the parentheses are mandatory."
2. `spec/reserved-words.md` line 15: `fn` is a compile error — "`fn` is not a
   word in this language — use `function`" — with a `Certain`,
   machine-applicable fix, implemented in the M1 lexer.

- **A** — `fn` leaves the foreign-word registry and lexes as an ordinary
  identifier; the parser accepts it only inside a parenthesised function
  type, and in declaration position re-emits the registry's message + fix.
- **B** — the type syntax drops `fn`: `(function(A) -> B)`. Registry and
  lexer untouched. Measured spec cost: 0 tokens.
- **D** — `fn` becomes a real keyword, legal *only* as the function-type
  marker; anywhere else the parser emits the registry's message + fix.
- C (keep the lexer error, have the parser tolerate the `Error` token) was
  rejected before convening: the diagnostic is already in the output vector
  when the parser runs, so it would have to be retracted.

Recommended provisional resolution as put to the judges: **D**. The evidence
overturned it; see *Resolution*.

## Verdicts

| Judge | Verdict | Cost/delta | Prediction | Condition |
|---|---|---|---|---|
| compiler-engineer | **approve B**, object D, object A. No veto (the function type is already core — design.md:1609, closure list at :138 — and none of the options adds a construct) | B: **0** compiler lines (`keywords.rs:48` stays true), 2 spec lines, 5 design.md lines, 0 in `editors/vscode/`; the shared type-parser arm (~6 lines) is paid identically by all three. D: +1 `keywords.rs`, +2 `token.rs`, +10–25 rejection sites in `syntax/`, a reworded registry row, and `fn` moves to `keyword` in `heroes.tmLanguage.json` — TextMate cannot express "keyword only in type position", so the M1 editor bonus *regresses* on the highest-frequency Rust prior. A: +15/−2 across four layers, plus an O(n) `slice` string compare in the self-hosted parser's hot path | at tag `m2`: under B, `crates/heroes/src/lexer/`, `spec/reserved-words.md` and `editors/` show **0 changed lines** and the whole function-type feature is ≤25 added lines under `syntax/`. Under D, either the registry message grows past its current 85 chars **or** a golden shows the compiler asserting "`fn` is not a word" about a word it just accepted | flips to approve D only on Part 11 metric-1 data where `(fn(A) -> B)` beats `(function(A) -> B)` beyond the instrument's 2% error bar; will not approve A on any evidence short of removing `fn` from design.md §4.17's list |
| llm-ergonomist | **approve variant 2 (= B)**, object variant 1; **veto** any formulation making `fn` a legal identifier (= A) | silent wrong programs over three tasks: **0 for both** spellings — the decision is first-try rate and rule consistency, not silent bugs, and a panel adopting B for the wrong reason will misapply the precedent. Tokens per annotation: 14 vs 20 | with n≥40 per arm: declaration-position `fn` (`name = fn:`) **20–35%** under variant 1 vs **5–10%** under variant 2 (the headline, 15–25pp for B); type-position spelling error **10–15%** vs **20–30%** (variant 1 wins this line — recorded against its own verdict, and resolved on frequency: declarations outnumber function-type annotations ~10:1); whole-file first-try +5–12pp for B; silent-error delta **0 ± 1**; arity error **≥25% in both arms** | B approved **only if the arity hole is fixed in the same amendment**: the spec shows one parameter while its own `fold` needs two, and no zero-parameter form is stated |
| spec-warden | **approve B**, object D, object A (strongest) | measured, both tokenisers: spec **now 1989/2048**; **B 1989/2048 — identical, 0 delta**; D needs an honesty sentence: +12 minimum, +24 for the true one, +37 with the declaration-position error named; `spec/reserved-words.md` 825/831 now, +37 under D, +40 under A. `spec/` as a package: 2879 now, **2879 under B**, **2940 under D** | at the M6 closure-list audit: if `reserved-words.md` is in §1.6's scope the closure-complete `spec/` breaches 3000 with zero new features (2879 + panel 012's ~129 mortgage = **3008**, and 3069 under D); if out of scope, `heroes-spec.md` lands 2170–2350 and 3000 holds through v1. On record for both. Also: **D produces ≥1 `check/` case where the applied `Certain` fix does not compile**; B produces zero | B: the five mechanical design.md edits land in the same commit, otherwise the source of truth stays contradictory. D: unliftable without ruling on §1.6's scope for `reserved-words.md`, taking the −96 header cut, and a golden pinning both `fn` positions |
| ffi-pragmatist | approve A, **approve D with a documentation condition**, **object B** (no veto: nothing here touches the ABI) | **null result, and it is the useful one**: no option changes the emitted C, the mangler, or `extern` passthrough. Compiled against three real headers (SDK `stdlib.h`, `sqlite3.h` + `-lsqlite3`, homebrew `raylib.h`): a *typed* Heroes callback is a hard clang error every time — Heroes `int` is `int64_t`, real callbacks take C `int` and `const` pointers — while `ptr` compiles, links and runs. `fn` occurs **0** times in declarator or field position across the macOS SDK and homebrew headers; `function` occurs **571** | at M7: SQLite (ladder step 3) binds with `cb: ptr`, no shim, no function type — already demonstrated. And the sharp one: the first `extern` written with a `(fn(…) -> …)` callback **fails against the real header** for `qsort`, `sqlite3_exec`, `sqlite3_busy_handler` and all six raylib callback typedefs — 9/9. The only expressible real callback is `sqlite3_bind_text`'s destructor `void (*)(void*)` | the resolution text must carry one sentence: **FFI callbacks are `ptr` until a C-width type vocabulary exists** (§4.19's deferred annotation vocabulary + Part 7 item 10). Vetoes at M7: presenting the function type as the FFI callback spelling; an emitter-inserted cast from `ptr` to a header's function-pointer type; `-pedantic-errors` before callbacks have a typed route |
| historian (advisory) | **approve B**, object D | precedent table below; nine languages checked, all sourced | if D lands, the registry's single `fn` row must split into ≥2 messages and the `Certain` fix must be **downgraded to `guess`** for at least one position, because inside a parenthesised type `function` is exactly what D forbids. If B lands, `fn`/`func`/`def` keep one message and one certain fix each | withdraws on a sourced language whose function-*type* keyword differs from its function-*declaration* keyword with the declaration word rejected in type position; searched Rust, Go, Zig, Nim, Odin, Swift, Hylo, Oberon-07, Erlang — none. Or on Part 11 data favouring `fn` |

## Findings that changed the proposal

**design.md contradicts itself, and the bug is upstream of `spec/`.** §4.17
(design.md:1444) names the autopilot mistakes the reserved-word errors exist
to catch: "`struct`/`enum`/**`fn`**/`let`/`var`/`function` used wrongly". The
same document spells the function type with `fn` at §4.12, §4.13, Part 7 and
twice in the appendix. §4.17 is the thesis section; §4.13 is a syntax note
whose only stated rationale is the mandatory parens, not the word. Resolve
against the syntax note.

**§1.9 had already settled it, and the instrument agrees.** design.md:305–309:
"An LLM struggles with unfamiliar semantics, not unfamiliar spelling… The
tokeniser, for its part, has no preference between `fn` and `::`."
Measurement: `fn` and `function` are each **one token** on both vendored
tables, so the two spec variants count identically. D's only benefit is
Rust-familiarity of the spelling — the benefit §1.9 refuses as a tie-breaker.

**Nobody uses two words.** Seven languages in Heroes' declared ancestry
spell a function type with the *same* word that declares a function — Rust
`fn`, Go `func`, Zig `fn`, Nim `proc`, Odin `proc`, Oberon-07 `PROCEDURE`,
Erlang `fun` (with Heroes' exact extra parens: `fun((T) -> T)`) — and two use
no keyword at all (Swift, Hylo). **Zero use a different word in type
position.** The mechanism D needs *does* have precedent — Rust's `dyn`
(contextual in 2015, strict keyword in 2018, RFC 2113), C++11's `auto`
(N2546 removed the storage-class meaning), C# 14's `field` (shipped with a
documented breaking change) — but in every case the one-position word is the
**only** spelling of its concept. A forbidden *synonym* that is also required
syntax is unattested.

**The registry's uniformity is an asset, and it was the thing being spent.**
Eighteen rows, one sentence shape ("X is not a word in this language"), no
exceptions. Under A or D, one row becomes positional and the `Certain` fix
`fn` → `function` becomes *wrong* in the position where `fn` is required —
a machine-applicable repair that produces a syntax error is the worst class
of diagnostic under §1.2, because the model trusts it, applies it, and errors
again. Under B the same fix becomes correct **everywhere**, at zero cost:
`f: fn(int) -> int` → `f: function(int) -> int` is one step from right.

**The function type is not an FFI feature** (null result, compiled). No
spelling reaches the emitter; `ptr` is the callback vocabulary until C widths
exist. What the boundary is missing is `c_int` and `const`, not a marker.

**The arity example was under-powered** (ergonomist, reading only the spec).
The spec shows `(fn(A) -> B)`: one parameter. Its own built-in list contains
`fold`, which needs two, and nothing states how a zero-parameter function
type is spelled. Both spellings force a guess there, and the effect is
larger than the spelling delta.

## Resolution — **B**, provisional, author ratification pending

Adopted: **the function type is `(function(A, B) -> C)`; `fn` keeps its loud
error, with its `Certain` fix, in every position.** One word, one meaning,
everywhere. The recommendation put to the judges (D) is overturned by their
evidence — recorded because that is what the panel is for.

Landing now (this commit):

- design.md — the five sites: §4.12 (:1177), §4.13 (:1227), Part 7 item 4
  (:1765), appendix `map` (:2009) and `fold` (:2016). §4.13 also gains the
  arity statement, and §4.19 the ffi-pragmatist's required sentence.
- `spec/heroes-spec.md` — **untouched: v0 stays frozen** (DESIGN-LOG
  2026-08-03, panel 000). The v1 amendment text is recorded below with its
  measured cost, as panels 002/003/006/007 do.
- `spec/reserved-words.md`, `crates/heroes/src/lexer/`, `editors/` — **0
  changed lines**, which is the whole point of B.
- `crates/heroes/src/syntax/types.rs` (M2 step 1) — `(` opens either `()` or
  `(function(…) -> T)`, one token of lookahead; plus a `Certain`-fixed
  diagnostic for `(function(x: int) -> int)`, the name copied in from a
  signature.

**What a veto would have compelled.** The ergonomist's veto is scoped to A
(`fn` as a legal identifier): had A been adopted, `fn = 5` would compile and
the registry's 100% catch rate for the commonest Rust prior would become
position-dependent. No judge vetoed B; the ffi-pragmatist's objection stands
on the record without one, and its condition is met verbatim in §4.19.

### The v1 amendment text (measured, not applied)

Spec v1 replaces the two `fn` sites and fixes the arity hole in the same
edit:

```
- Top-level functions are values: `xs.fold(0, add)`. Function type syntax:
  `(function(A, B) -> C)` — the parentheses are mandatory; no arguments is
  `(function() -> C)`.
…
  at the call site: `map = function<A, B>: (xs: [A], f: (function(A) -> B)) -> [B]`.
```

Measured with the project's own instrument (`heroes measure`, two vendored
tables, sha256-pinned):

| variant | claude-legacy | cl100k | binding max | Δ |
|---|---|---|---|---|
| v0 as frozen | 1989 | 2048 | **2048** | — |
| `fn` → `function` only | 1989 | 2048 | **2048** | **0** |
| + the arity sentence (the text above) | 2001 | 2062 | **2062** | **+14** |

The spelling change is free. The +14 is the arity fix, above the soft 2000,
so panel 012's rule applies: it is carried by the ergonomist's
pre-registered falsifiable prediction (arity error ≥25% in both arms, and the
fix is worth more first-try rate than the spelling choice). The named
removal remains available if the author prefers to stay under soft: the
spec's HTML provenance header, measured **−96**, which also still asserts the
"~1500 tokens" budget panel 012 falsified.

## Predictions to score

| # | Judge | Prediction | Checkable at |
|---|---|---|---|
| 1 | compiler-engineer | under B: 0 changed lines in `lexer/`, `spec/reserved-words.md`, `editors/`; function-type feature ≤25 added lines under `syntax/` | tag `m2` |
| 2 | llm-ergonomist | declaration-position `fn` 20–35% (v1) vs 5–10% (v2); type-position error 10–15% vs 20–30%; first-try +5–12pp for B; silent-error delta 0 ± 1 | first Part 11 run (metric 2/3) |
| 3 | llm-ergonomist | arity error ≥25% in **both** arms — the variant-neutral defect dominates the spelling | same run |
| 4 | spec-warden | scope of §1.6 decides a breach: `spec/` package 3008 (in scope, closure-complete) vs `heroes-spec.md` 2170–2350 (out of scope) | M6 closure-list audit |
| 5 | ffi-pragmatist | SQLite binds with `cb: ptr`, no shim, no function type; a typed `(function(…) -> …)` callback fails against the real header 9/9 on the ladder | M7 |
| 6 | historian | under B the registry keeps one message + one `certain` fix per foreign word; goldens stay green | tag `m2`, then M3d |

## Watch list added

- **§1.6's scope for `spec/reserved-words.md` is undecided** (spec-warden):
  831 measured tokens specifying which words are legal — semantics, and a
  diagnostic class that triggers `/panel` — currently uncounted. True
  headroom is 952 or 121 depending on the answer. Worth more than the
  largest number in this panel; queued.
- **`heroes measure` cannot reproduce panel 011's published o200k number**
  (2050): only two tables are vendored, so the reported maximum is 2048.
  Immaterial to every verdict here, recorded because a warden who lets an
  unreproducible number stand has no standing to demand measurement.
- **The foreign-word registry collides with C identifiers bindings must
  name** (ffi-pragmatist, pre-existing and option-independent): in SDK
  headers, declarator/field position, `function` occurs 571 times, `func`
  29, `assert` 11, `test` 5, `match` 3 — and §4.19 has no alias syntax, so
  those symbols are unreachable. Belongs to §4.19's deferred vocabulary at
  M7.
- **Closures at v1.5 break C-ABI compatibility of function values**
  (ffi-pragmatist): a capturing closure is a record plus a pointer, so the
  type system will have to distinguish capture-free at the boundary. An
  argument for keeping the function type narrow — and identical under either
  spelling.

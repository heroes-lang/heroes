# Panel 017 — the type system's four edges

Date: 2026-08-04. Trigger: M3b, the bidirectional checker (design.md Part 10
step 5). Four questions the checker cannot avoid answering and the spec does not
answer; each had been queued since an earlier panel found it, and all four came
due together because M3b is the pass that must have an opinion. Four judges (the
ffi-pragmatist has no standing: none of the four touches the ABI, `extern`, or
the runtime's contract — B's runtime obligation is design.md §4.20's already).

## Proposal (as put to the judges)

| # | question | options |
|---|---|---|
| **A** | How is a jump (`break`/`continue`/`return`) typed? | **A1** no type, skipped at a join · **A2** a bottom type (`Never`) · **A3** `()`, RFC 1216's recorded wrong turn |
| **B** | `+` on `str`? | **B1** concatenation, `str + str` only · **B2** a `concat` built-in · **B3** leave it unwritable |
| **C** | A `()` value in binding position (`x = print("a")`)? | **C1** an error · **C2** legal |
| **D** | A declaration as an inline arm body (`=> x = 5`)? | **D1** an error · **D2** legal |

## Verdicts

| Judge | A | B | C | D |
|---|---|---|---|---|
| compiler-engineer | **approve A1**, object to the wording; **A3 refuted by execution** | approve B1 (+23 lines) | approve C1, **object to "certain"** | approve D1, **in the parser** |
| llm-ergonomist | approve A1 + one required sentence | **approve B1 — and object to B as posed** | approve C1 on diagnostic grounds, low stakes | approve D1 weakly |
| spec-warden | approve at **0 spec tokens**; **VETO A2** | approve B1 ≤ +13; **B(d) mandatory at +8** | approve C1 as a compiler decision, 0 tokens | approve D1, 0 tokens — already derivable |
| historian (advisory) | **object**: A1's cost is per construct; recommends A2 *or* one shared rule | approve B1; drop Zig from the argument | approve C1 — **Nim ships it verbatim** | approve D1, in the parser |

Measured, by the judges given the instruments:

- **A, built three times** in scratch clones of the real tree: A3 **581 lines**
  and *rejects every function whose body ends in `return`* (7 probes, reproduced);
  **A1 608 (+27)**, all 7 probes correct; A2 613 (+32), byte-identical
  diagnostics to A1. `Option<TyId>` appears in **4 sites**, not threaded
  everywhere, because `Return`/`Break`/`Continue` are `StmtKind` — so `synth` is
  total and the compiler is never asked what `break` is worth.
- **spec cost**, measured on edited copies (binding max of two vendored
  tokenisers; baseline **2048**): A **0** · B1 **+13** · **B(d) +8** ·
  C1 +9 (+39 with panel 003's mirror) · D1 +5. Maximalist all-four: 2198, i.e.
  802 under the ceiling — so this was never a budget question.
- **the reserved-list defect**, measured: `resolve/builtins.rs` reserves `sort`
  and the int-to-string conversion; the spec lists neither, so `sort = function:`
  is *spec-legal and resolver-rejected*. And spec v0 **cannot render an int into
  a message by any route** — `+` is numeric-only, `join` needs a `[str]`, and
  there is no int→str at all.

## What changed the questions

**1. A3 was refuted by execution, not by argument.** The engineer built it: under
`()` every function whose body ends in `return` is a type error. RFC 1216 says
the same thing in the Rust compiler's own words ("some code in the compiler
assigns type `()` to diverging expressions because it doesn't have a sensible
type to assign to them"), and C++ pays for the same design with a bespoke clause
in `[expr.cond]` — which is *why* `throw` is usable in value position nowhere
else.

**2. The historian's objection to A1 was answered by building it differently.**
Its case: A1's cost is one rule *per branch-joining construct*, Heroes already
has two (`match` and `if`), and Rust's decade of `!` pain is specifically
**never-type fallback** — a global-inference problem a bidirectional checker with
explicit signatures does not have, so A2 is cheaper than it looks. Its stated
withdrawal condition was precise: *"I withdraw this prediction — and my A
objection with it — if A1 lands as a single shared join routine whose branch list
carries `diverges` as data, with no per-construct branch."* That is
`types/join.rs`: **one routine, every join**. Both conditions now hold at once —
no `Never` variant for the descriptor pass to meet at M5c, and no rule repeated
per construct.

**3. design.md contradicted itself, and the panel had to pick a side.** §4.7's
third bullet says a `match`'s type "comes from the arms that can [produce a
value]"; its fourth said "every arm must produce a value — the rule that makes
`tag = match t` with a `.eof => break` arm loud". Those are the same program,
called legal and illegal one bullet apart. Resolved for the third: the fourth now
reads "produce a value **or jump**", and its payoff moves to where the silent
program actually was — an arm whose body is `()`-typed, as in `_ => print(0)`,
inside a `match` used as a value. That case needs no arm-specific rule at all; it
is the ordinary ⇐-check, which is why the engineer measured its own panel-014
prediction ("≤25 lines") at **0**.

**4. B is worse than it was posed, and the ergonomist found it by writing
Heroes.** Asked to produce `"unknown_char at position 12"` from the spec alone,
it wrote `to_str(position)` from habit — a function that does not exist — and
then established that the string *is* producible using only what the spec lists:
a `constant: str` digit table, `slice`, `push` into a `[str]`, a reverse loop and
`join`. **28 lines, two convention guesses, and two silent failure modes** (digit
order, and whether `to:` is inclusive). So `str + str` alone fixes nothing: the
binding constraint is the missing conversion. Adopted together, and the
conversion is **`to_str`** rather than §4.20's `.str()` — `to_int`/`to_f64`
already establish the scheme so a model can derive the third, and `str` is the
name of a *type*, which panel 013's "one word, one meaning" rules out for a
function.

**5. C1's fix cannot be `Certain`, and the engineer showed why with the
type.** A `Fix` carries exactly one span, and the only programs that reach C1's
diagnostic are those where the name *is* read — so deleting the binding orphans
the read, and a two-site repair is not representable. It ships `Guess`, and no
`x.fixed` golden. The historian supplied the precedent that settles C itself:
**Nim** — the language CLAUDE.md §6 says Heroes copies the surface of — ships
"the `void` type is only valid for parameters and return types; other symbols
cannot have the type `void`" verbatim. C/C++ agree structurally. Rust, Kotlin and
Swift all allow it and all felt the need to warn; two of the three recorded false
positives.

**6. D1 belongs in the parser, and both judges that looked at the code agree.**
Today `1 => x = 5` is reported as `unused_binding: "remove the binding, or read
it"` — a repair the author cannot apply, since nothing can read it. Put the rule
in the checker and the resolver still speaks first, so the user gets the
misleading message *and* the real one: two diagnostics for one mistake, which
this project treats as a defect. Rust excludes `let` from expression position by
**grammar** (verified), and that is where it goes: `syntax/control.rs`, with a
guard for a recovery-`Error` value so a broken arm body still reports once.

**7. The spec is normatively wrong, and that is the warden's finding, not an
aside.** Panel 013 ratified `(function(A, B) -> C)`; spec v0 still says
`(fn(A) -> B)`, so a model briefed from the spec writes the *reserved-word error*
spelling — and CLAUDE.md §1 has the assistant read that spec at every session
start. The freeze exists to preserve the pre-amendment measurement, which panel
011 has already published (1989 / 2048 / 2050) and git preserves. The v1 package,
measured with the panel-015 removals spent: **1998 (−50 from v0)** — under the
soft 2000 line for the first time. Scheduled for M3d, where the baseline harness
run the freeze was waiting for lands.

## Resolution — RATIFIED 2026-08-04

Ratified as they land, by author instruction (`/goal`).

1. **A1**, with the two conditions met: **one shared join routine**
   (`types/join.rs`), and **no bottom type** in the table. design.md §4.7's
   contradiction is amended in the same commit. A `match` all of whose arms jump
   produces no value and is legal only in statement position — Kotlin's sentence
   for a non-exhaustive `when`, adopted.
2. **B1** (`str + str` is concatenation, no implicit conversion of anything else)
   **and B(d)**: `to_str` and `sort` enter the built-in inventory as a **defect
   fix**. design.md §1.11/§4.20 and the appendix are amended to `to_str`.
3. **C1**, with a `Guess` fix and no `.fixed` golden. Recorded bill: no generic
   over a `()`-returning function until `()` becomes a type — nothing on the
   closure list needs one.
4. **D1**, in the parser, with the `ExprKind::Error` guard and a golden pinning
   exactly one diagnostic.

## Predictions to score

| # | Judge | Prediction | Checkable at |
|---|---|---|---|
| 1 | compiler-engineer | `types/exprs.rs` breaches the ~300-line rule at M3c unless the operator rule and the join rule are split out at M3b | M3c close — **scored: correct**, and the split landed with it (`join.rs`, `ops.rs`, `expect.rs`) |
| 2 | compiler-engineer | if `Ty` ever needs a `Never` variant, A1's approval is withdrawn and A2 should have been taken at M3b — 5 lines then, a rewrite of every join site later | M4 |
| 3 | compiler-engineer | the ownership pass's owned-temporary classifier must key on the result *type*, not the node kind, and `--dump-ir` must show a `decref` on the temp of `a + b + c` | M5b |
| 4 | llm-ergonomist | first-try on "return a `str` containing an `int`": ≤8% (±6) unwritable today, ≤20% (±10) with `+` alone, **≥70% (±12)** with `+` and `to_str`; ≥55% of the failures call a nonexistent int→str name | first Part 11 run |
| 5 | llm-ergonomist | A1 beats A3 by +15 (±8) points, ≥70% of A3's rejections are correct-intent code, and A1's own inversion (`.plus => 1` where `return 1` was meant) appears in ≤3% (±2) of value matches | first Part 11 run |
| 6 | spec-warden | the v1 package measures **1998 ± 6** binding max (−50 from v0); and of the next 20 model-written programs annotating a function value from spec v0, **≥15 write `(fn(A) -> B)`** versus ≤2 after the amendment — 6,500–26,000 tokens of rewrite bought by refusing to spend 13 | M3d / v1 package |
| 7 | historian | under A1 the "skip the diverging branch" decision appears at ≥2 construct-specific sites by M3b close and ≥3 by M5 | M3b close — **scored: falsified by construction**, 1 site (`join.rs`), which is the judge's own withdrawal condition |

## Watch list

- **`print`'s reservation** still has PEP 3105 against it (a function *so that*
  one module can replace it, for output capture in tests). Heroes has `test`
  blocks and no capture mechanism. Carried from panel 015, still open.
- **Ordering on `str`** is not offered (`a < b` on strings is an error). A
  collation is a language decision; the rejection is relaxable.
- **`to_int` on a `str`** is not offered: parsing can fail and the spec does not
  say what it returns. Wants `str -> int?`, and that is a panel.
- **Argument labels may not reorder**: `f(b: 2, a: 1)` is an error, because the
  label is checked against the parameter at that *position*. Unspecified in the
  spec; the rejection is the conservative half.
- **`()` inside a container** (`[()]`, `{str: ()}`, `()?`) resolves clean today.
  The engineer's condition 4: ~10 lines to reject it, and the hole is open under
  C1 and C2 alike — C1 does not close it and is not credited with closing it.
- **A `match` over `bool`** is treated as countable (so `_` is banned) but its two
  cases are not nameable, since `true`/`false` are literals, not `.cases`. Today
  that makes an exhaustive `bool` match unwritable except with literal arms plus
  `_`… which the ban then rejects. Found while writing `patterns.rs`; queued.

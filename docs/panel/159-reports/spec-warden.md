# Panel 159 — spec-warden report

Written 2026-09-16. Every number, path and count below was produced by a command
run in this session (CL-017, CL-077). All drafts were built in the session
scratchpad; nothing in the tree was modified. Exit codes captured directly.

- `verdict`: **object (provisional)**. Provisional because every delta below is
  measured on the VENDORED table and design.md §1.6 binds on `claude-opus-5`
  through `POST /v1/messages/count_tokens`, which this machine cannot reach.
  **No veto is cast**: nothing on the ballot approaches the ceiling.
- `section`: design.md §1.6 (budget, instrument, unconditional payment), §1.2
  (tokens × (1 + rewrite rate)), §1.0/Principle 0;
  `.claude/rules/spec-shape.md` § Where a rule lives.
- `spec_token_delta`: base **5989** cl100k / **5863** legacy / **7974** real
  (`claude-opus-5`, taken 2026-09-15, CACHED not re-measured). Recommended
  package **5999** cl100k, **+10**. Real delta **UNRUN**.
- `removal`: § 3's `cstr` clause, a duplicate of § 13's — measured **−14**. It
  pays R1 in full. **R2 and R3 have no removal named: nothing — and that is a
  problem.**
- `needed_for_self_hosting`: **no**, for all three.

## The ceiling, grepped this session

`docs/design/design.md:255`: **10240 tokens, measured by `claude-opus-5` through
`POST /v1/messages/count_tokens`**. Payment rule unconditional at every level
(design.md:281-282 and 311-314). Not taken from any brief.

## The instrument gap in the briefs (the eighth correction the shared brief invites)

Both briefs ask each seat to price drafts with `./heroes measure <draft>`. For an
arbitrary path that command reads **only the two vendored tables**. `--refresh`,
the only route to the binding number, **refuses every path but
`spec/heroes-spec.md` and `CLAUDE.md`** (`selfhost/measure/pinned.hero:107-112`,
panel 123 R5), and here it exits **2** for want of `ANTHROPIC_API_KEY`. So the
sitting's own method yields a LOWER BOUND, which is the 2026-09-09 failure
(`docs/measurements/023-the-instrument-was-not-the-readers.md`) in miniature.
The gap between vendored and real on this document is **1985 tokens**, so no
verdict here may rest on a vendored delta alone. It does not change the outcome:
the worst draft measured leaves ~2200 tokens of real headroom.

## R1 — the direction is NOT stated, and "ascending" alone does not state it either

Every occurrence of "order" in the document (5 sites, grepped):

| line | text |
|---|---|
| 100 | `Declaration order never matters` |
| 180 | `a map's insertion order does not affect it` (about `==`) |
| 183 | `ordering one aborts, in < <= > >= and in sort` (about `nan`) |
| 248 | `the other order is another type` (named parameters) |
| 288-289 | `keys(m) ... gives the keys in no order, and for k in sort(keys(m)) walks them in order` |

The only candidate sentence is built as a **contrast with `no order`**, so "in
order" reads as *some definite order*. It picks no direction. **Not stated.**

**And the one-word repair the sitting priced does not close it.** Measured:

- `sort([3,1,2])` → `123`; `sort(["pear","apple","fig"])` → `applefigpear`;
  `sort([true,false])` → `falsetrue` (exit 0).
- `sort(["B","a","A","b"])` → **`A,B,a,b`** — byte order, uppercase first.
  `sort(["zz","z"])` → `z,zz`.
- `print("a" < "b")` → **exit 1**, `error[bad_operand]: '<' takes any integer or
  a float, found 'str'`.

So § 7 gives the reader **no ordering relation on `str` or `bool` at all**, and
the document's ONLY example of `sort` sorts `str` (`sort(keys(m))` over
`{str: i64}`). Telling that reader "ascending" names a relation the document
does not have. A reader who assumes case-insensitive order still **compiles and
prints a silently different answer** — the exact class the sitting convened for,
untouched by the +2 word.

## R2 — the grammar ALREADY derives `xs[i] @ v`. Derivation, by hand:

```
Statement -> Place "@" Expression NEWLINE                (§ 5)
Place     -> ident { "." ident | "[" Expression "]" }    (§ 5)
          -> ident "[" Expression "]"        one repetition, second alternative
          -> xs "[" i "]"                    Expression -> ... -> Primary -> ident
Statement -> xs [ i ] @ v NEWLINE                                              ∎
```

Four steps, no ambiguity. `heroes grammar` prints both (its lines 57-59, 64).
The compiler agrees: `xs[i] @ 99` prints `99`, exit 0; `p.x @ 7` and
`grid[0][1] @ 5` print `75`, exit 0.

**`Place` is ONE rule with THREE instances — name, field, element — stated once
in the notation the document's opening paragraph declares normative.** Prose for
the element instance alone restates a third of a rule and makes the other two
look excluded. That is a NEW ambiguity bought with tokens, and it is the
strongest reason to refuse the proposal as written.

## R3 — § 1 states the signature, not its exhaustiveness

`- One file is one module; the file you compile holds `function main()`.` gives
the exact signature; § 3's table makes `()` what a function with no `->`
returns. Missing is one word of force: *holds* is not *exactly*. The compiler
refuses `-> ()?` at exit 1 with `error[main_returns]` and a `note:` that already
carries the fix.

## The six drafts the brief asked for (cl100k_base; base 5989)

| # | draft | count | delta |
|---|---|---|---|
| 1 | R1 free-standing, `` `sort` is ascending.`` (§ 11) | 5996 | **+7** |
| 2 | R1 merged into § 11's `sort` parenthesis (`ascending;`) | 5991 | **+2** |
| 3 | R2 free-standing, `` `xs[i] @ v` replaces an element.`` (§ 10) | 6002 | **+13** |
| 4 | R2 merged into the `m[k] @ v` sentence (§ 10) | 5999 | **+10** |
| 5 | R3 free-standing bullet (§ 1) | 6001 | **+12** |
| 6 | R3 merged, `, that signature exactly` (§ 1) | 5993 | **+4** |
| — | all three merged | 6005 | +16 |
| — | all three free-standing | 6021 | +32 |

**Panel 122 holds a second time, on three fresh drafts: merging beat appending
every time, and the merged set costs half the free-standing set (+16 vs +32).**

## And the four drafts the brief did not ask for, which are the answer

| draft | count | delta |
|---|---|---|
| R1 merged into § 10 instead (`in ascending order`) | 5990 | +1 |
| **R1 COMPLETE, merged** — `sort` (ascending: a number by value, a `str` by bytes, `false` before `true`; never a type parameter) | 6000 | **+11** |
| R1 complete, free-standing (§ 11) | 6015 | +26 |
| **R2 GENERAL, merged into § 5** — `@` declares a mutable cell and re-binds it, **or a field or element inside one** | 5998 | **+9** |

Two findings the sitting's framing hides:

1. **The +1 variant is the worst buy on the table.** It puts a `sort` fact in
   § 10 (spec-shape.md § Where a rule lives) and states the direction only for
   the map-keys case, leaving `sort([i64])` unstated. One token for a third of a
   rule.
2. **For R2 the COMPLETE rule is CHEAPER than the partial one: +9 against +10.**
   Naming the general form costs less than naming one instance, and it carries
   no expressio-unius damage. If anything lands for R2, it is this.

## Removals, measured

| candidate | delta | my position |
|---|---|---|
| § 3's `and only a group's record may hold one` — § 13 already says `outside a group nothing answers cstr and no record holds one` | **−14** | **spend it.** A true duplicate under the one-home rule; § 13 is `cstr`'s home. § 3 keeps its own half (`A cstr copies an address too.`) |
| § 7's `so x != x asks whether it is one` | −12 | available, but it is content, not duplication |
| § 10's quadratic-concatenation sentence | −31 | available; I would rather not spend it |

## What I would land, measured end to end

| package | count | delta |
|---|---|---|
| R1 complete + the −14 removal | **5986** | **−3** — the silent-error class closes and the document SHRINKS |
| R1 complete + R2 general + R3 merged + the −14 removal | **5999** | **+10** |

## Instrument safety, run rather than assumed

Against `package.md`, `r1c-merge.md`, `r2-merge.md`, `r3-merge.md` and
`rm-cstr-dup.md`, using the harness's own `spec_text` module copied into the
scratchpad:

- `offered_builtins` reads **24** names on every draft, byte-identical join to
  the base (the § 11 merge sits inside a prose parenthesis, which
  `without_prose_parentheses` strips — its own unit tests say so and the run
  confirms it);
- `section_named(text:, name: "FFI")` resolves on the package (15845);
- non-ASCII inventory is exactly `· — … →` on every draft — the closed set;
- `## ` headings: 13 on every draft.

## Verdict per resolution

- **R1: object to the form on the ballot, approve the complete one.** +11
  merged, paid in full by the −14 removal, net −3. The +2 word is a sentence
  that LOOKS like the closure and is not; under §1.2 a construct that saves
  tokens while leaving the error probability where it was is a net loss, and
  this one names a relation (`<` on `str`) the document does not have.
- **R2: object.** The document already answers it; the compiler agrees; the cost
  is zero. If the panel insists on prose because a reader measurably missed it,
  it must be the GENERAL rule in § 5 (+9), never the element instance (+10/+13).
  **No removal is named for it, so under the unconditional rule it waits.**
- **R3: object at +4** on burden of proof — `error[main_returns]` teaches it at
  the first attempt, with a `note:`, and §1.2's second factor is what the
  sentence has to beat. **No removal named: it waits.**
- **R4: merged, not three** — measured twice over, on cheap drafts (+16 vs +32)
  and on complete ones.

## Prediction, with an instrument that exists today

1. If the package lands, `./heroes measure spec/heroes-spec.md` reads **5999**
   cl100k / **5875** claude-legacy, and `SPEC_TOKENS` = 5999 in
   `tests/harness/suite_spec.hero` with `budget`, `recorded` and `ledger` green
   in the same commit. Instrument: `heroes measure` and `suite_spec.hero`, both
   run this session. Scored at M-check-completeness close.
2. If only R1 complete + the removal lands: **5986**, a net −3, and the first
   amendment in this document's history to close a silent-error class while
   shrinking. Same instruments, same milestone.
3. §1.2: a re-run of panel 126's llm-ergonomist protocol (six programs, spec as
   the only input) against the amended document produces **zero** programs that
   assume a `str` order other than bytes. If one still does, R1's sentence
   failed and is re-argued under the removal branch (design.md §1.6, *an
   outstanding prediction is re-decided, never renewed*).

## Condition — what would change my verdict

- **To approve:** R1 lands in the complete form (+11) with the −14 removal, and
  R2/R3 either name their own removal or wait.
- **To veto:** the sitting lands R1 as the bare word `ascending` and calls R1
  closed. That is Principle 0's burden unmet — no compiler need, and no measured
  thesis effect for the `str` case, which is the case the document's own example
  uses.
- **To withdraw the `provisional`:** one run of
  `ANTHROPIC_API_KEY=... ./heroes measure spec/heroes-spec.md --refresh` on a
  worktree whose `spec/heroes-spec.md` IS the package draft.

## UNRUN

- **The real (`claude-opus-5`) delta of every draft above.** Command:
  `ANTHROPIC_API_KEY=... ./heroes measure spec/heroes-spec.md --refresh`, run in
  a worktree whose `spec/heroes-spec.md` is the draft — `--refresh` refuses any
  other path. Exits 2 here for want of the key; `.env` exists but carries no
  `ANTHROPIC_API_KEY=` assignment (checked).
- **The `spec` and `special` suites against the drafts.** Command:
  `./heroes run tests/harness/main.hero -- ./heroes spec` in such a worktree.
  What I ran instead is the `offered`/`section_named`/inventory/heading probe
  above, which covers the checks my edits could plausibly break; `budget`,
  `recorded` and `ledger` would fail by construction until `SPEC_TOKENS` moves.
- **Whether the `str` byte order holds above ASCII.** Not tested; only
  `A B a b` and `z zz` were run.

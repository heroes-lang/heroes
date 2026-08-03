# Panel 000 — language design review (verdict: OBJECT)

## Top objections

**1. Principle 0's second question is a destructive filter that contradicts
Part 1.** "Can the compiler be written without it? If yes → reject" deletes
the language's differentiators: `???` (§4.16), the same-typed-argument rule
(§4.9, "the most original construct"), rich errors (§4.17), the `_` ban,
unused-variable errors, `f64`, `%`, `.is_err()`. A compiler needs none of
them. §1.1 states LLM comprehension *is the objective*; Principle 0
substituted self-hosting minimality. **Fix (adopted):** self-hosting is
necessary, not sufficient; question 1 kept, question 2 dropped.

**2. The `T` → `T?` promotion rule reopens a Part 6 permanent rejection.**
Part 6 rejects implicit conversions outright ("`f(x)` works for an invisible
reason"); adding promotion to §4.6 while marking §4.3 and Part 6 unchanged
leaves the amended document self-contradictory in three places.
**Fix (adopted, panel 002):** the `ok(x)` constructor — §4.6 already names the
cases `ok`/`err`, and `.ok`/`.err` arms are already in the appendix — zero new
vocabulary, ~8 tokens, no coercion point in any checking rule.

**3. Promotion was not the root bug; `fail`'s type is unspecified and
uninferable.** `fail` returns `[Token]?` (L1780), `Expr?` (L1833), `int?`
(L1885): return-type-directed instantiation, which §4.12's "look at argument
types, deduce T" cannot do. Same panel session (002).

**4. Promotion is needed at six sites, not two** — L1838, L1852, L1864, L1865,
L1878, L1886 — and one is inside a generic, where uniform promotion makes
`T??` undecidable under monomorphisation with `A := Expr?`. design.md does not
cover nested optionals at all.

**5. The −5 token accounting was unfalsifiable and out of sequence.** The
budget applies to `spec/heroes-spec.md`, which revision 1 deferred to end of
M3 while landing amendments in M0. **Fix (adopted):** spec v0 in M0.

## Gaps found in the closure list (all adopted)

`if`/`else` was absent; map iteration (+ the unspecified iteration order —
a fixpoint requirement); `sort` (named in §4.12, listed nowhere);
`join`/`Builder` (wart 8: the C emitter is the O(n²) case); the string/map
helpers of bug 5; error accumulation via `@diags: [Diagnostic]`; `exit(code)`;
`()` as a type (used at L1971/L1437, absent from §4.3); `print`'s variadic
mixed-type contract (L1984 is the document's one implicit conversion);
argument evaluation order with nested `@` unspecified.

## Missed inconsistencies in design.md (adopted into the errata/panel queue)

- L578: third `&`-for-boolean instance (`c >= 48 & c <= 57`).
- L1984: `print` mixes `str` and `int`.
- L1886: `.must()` after `has` — the sentinel pattern §4.9 added `has` to remove.
- `err` payload type never named, though `e.code`/`e.msg` are used.
- Statement-vs-expression position undefined against "blocks are expressions" (L784).

## What to cut (adopted)

- **`discard`** — wrongly justified (ignored `T?` is near-nonexistent; `?`
  costs one character). The real class is `xs.push(4)` as a statement with
  `push` pure. Cheaper fix, zero keywords: *a non-`()` expression in statement
  position is an error* (panel 003).
- Bug 4 ("name check") is a to-do, not a bug.
- **Do not delete the "no debugger" non-goal** — `#line` gives lines, not
  values; reword instead.

## Verdict

**object** — resolved in revision 2 by adopting `ok(x)`, dropping Principle
0's second question, and folding every gap above into the closure list and the
panel queue.

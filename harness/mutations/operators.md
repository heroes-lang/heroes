# Mutation operators (metric 3 — silent-error rate)

Each operator is applied mechanically to every applicable site in the golden
corpus. For each mutant: does `heroes check` reject it (KILLED — the design
worked) or accept it (SURVIVED — a silent error the thesis says should not
exist)? Report per-operator kill rate. The operator list is data: extend it
here, never in code.

| id | Operator | Plausible-mistake class | Thesis mechanism that should kill it |
|---|---|---|---|
| swap-args | Swap two same-typed arguments at a call site | classic LLM inversion | mandatory named args (§4.9) |
| drop-case | Delete one variant case's arm from a `match` | forgotten case | exhaustiveness (§4.7) |
| forget-at-decl | `v: int @ 0` → `v = 0`, keep later `v @ ...` | mutability confusion | `@` on non-declared name (§4.4) |
| mutate-undeclared | Introduce `totl @ total + x` (typo'd target) | silent new variable in other langs | mandatory type on declaration (§4.4) |
| typo-ident | Typo one identifier at one use site | 1-char edit | no shadowing + unused + undefined (§4.4) |
| typo-code | Typo the error code in `fail("…", …)` or in the `e.code == "…"` that reads it back | 1-char edit, across the two ends of a contract | **nothing** — §4.6 makes the code a bare `str`, and this row is what measures that |
| wildcard-variant | Replace a variant arm with `_ =>` | lazy catch-all | `_` ban on variants (§4.7) |
| positional-named | Remove argument names where two params share a type | style transfer from Python | same-typed-argument rule (§4.9) |
| mix-int-float | `1 + 2.0` at one site | implicit-conversion prior | no implicit conversions (§4.3) |
| shadow | Redeclare an in-scope name | inner-scope habit | shadowing ban (§4.4) |
| drop-question | Remove one `?` from a fallible call in fallible context | forgotten propagation | type mismatch `T?` vs `T` (§4.6) |
| typo-digit | Move the last digit of a `constant`'s value by one | 1-digit slip in a number copied from a header or a table | **nothing** — a number in the file has no authority to be checked against, and this row measures the subset where one exists (a C constant, §4.19) |

Sites are found syntactically; one mutant per site; mutants that fail to parse
are excluded (they measure the lexer, not the thesis).

**An operator may name no killing mechanism.** Ten of these rows point at a rule
the design already installed, and measure whether it holds; `typo-code` and
`typo-digit` point at a gap, and measure how wide it is. Both directions belong
here — an instrument whose every row can only report success is not measuring, it
is confirming. When a rule closes the gap, the row gains its mechanism and the
number moves; that movement is the evidence the rule was worth its cost.

**A row may also lose its sites, and that is not the same as passing.** A rule
that removes the *form* the operator edits leaves the row with nothing to mutate,
and a harness reporting per-operator rates cannot tell a defence from an empty
denominator (`mod.rs`'s rule 3 and `tests.rs`'s opening note). Where that happens
the site count is the number to report, not the rate: `typo-digit` on a C constant
goes to zero sites under §4.19's header-valued form, because there is no longer a
digit in the file — and *that* is the claim, stated as a count.

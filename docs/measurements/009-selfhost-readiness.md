# Measurement 009 — self-hosting readiness: what the lexer port finds

**Status: in progress — M-selfhost-probe is open.** This document accumulates
the gap list as the port advances, one entry per finding, under the milestone's
own rule (docs/ROADMAP.md § M-selfhost-probe):

> Every form the probe proposes arrives with three things — the Heroes code
> that does the same job *without* it, that code's line count, and the form's
> own spec cost quoted from `heroes measure` — and a form whose workaround
> compiles is a Part 7 deferral by default, overturned only by §1.0
> compiler-need (nothing in Heroes expresses the case at all) or a measured
> Part 11 effect.

The ROADMAP names this file `004-selfhost-readiness.md`; 004 was taken by
`error-codes` before this milestone opened, so the number is 009 and the
ROADMAP's pointer is corrected with this milestone's close.

**The subject moved under the probe before it started.** The ROADMAP's premise
(983 non-test lines, `as_bytes()` at seven sites, "the closure list has no form
that replaces byte access") describes the lexer of 2026-08-12. Today the lexer
is **1,643 non-test lines** across 9 files (M-literal-bases and M-sized-integers
grew it), and the byte wall the premise feared **no longer exists**: `s[i]`
yields a `u8` (spec § Strings), a character literal is an integer that takes
its width from context, and `examples/calculator/lex.hero` already scans bytes
this way. The probe's findings so far are accordingly smaller than the wall
that was predicted — which is itself the measurement.

## Ported so far, tests green under `heroes test selfhost/<file>.hero`

| selfhost file | ports | Rust lines | Heroes lines | tests |
|---|---|---|---|---|
| `token.hero` | `lexer/token.rs` | 194 | 198 | 3 |
| `keywords.hero` | `lexer/keywords.rs` | 87 | 113 | 3 |
| `escape.hero` | `lexer/escape.rs` (pure half) | 100 of 168 | 128 | 4 |
| `digits.hero` | `lexer/digits.rs` (minus the two diagnostic builders) | 155 of 207 | 197 | 4 |

## Gap list — forms the port wanted

Every entry so far is **workaround-compiles**: nothing yet qualifies for the
closure list under §1.0. No proposal leaves this file before the milestone
closes.

1. **No `i128` — and no need for one.** `digits.rs::decode_wide` decodes into
   i128 because no 64-bit type spans u64's top and i8's bottom. The port
   decodes into `u64`, which holds every literal a program can write (literals
   are unsigned by construction; `-1` is unary minus applied to `1`), and the
   signed lower bound moves to the checker port, where negation lives.
   Workaround: 0 extra lines — a different type choice, not a detour.
   **Deferral by default.**

2. **Overflow aborts; a decoder wants it to fail politely.** Rust's
   `from_str_radix` returns `Err` on overflow; Heroes aborts on the wrapped
   multiply. Workaround: guard each accumulation step
   (`value > (U64_MAX - d) / radix`), +2 lines. The alternative form would be
   checked arithmetic (`mul_checked -> u64?`); nothing on the closure list
   needs it and the guard is idiomatic. **Deferral by default.**

3. **No `Option<T>`.** Every `Option` in the Rust lexer ports as `T?` with a
   named miss code (`not_a_keyword`, `not_foreign`, `no_base_marker`), and
   both callers match on the miss anyway. Workaround: 1 constant per code.
   The distinction Rust draws between `Option` and `Result` collapses into
   `T?` without loss *in this codebase*, because a miss always has one cause.
   **No form wanted at all.**

4. **No tuples.** `split_base` returns `(Base, &str)` in Rust; the port
   returns a two-field record (`Split { base, digits }`), +3 lines, self-
   documenting. **Deferral by default** (Part 6 already refuses tuples).

5. **No byte-to-str conversion.** `escape.rs` maps an escape to its byte
   (`Option<u8>`); the port's one table returns the decoded one-character
   *string* instead, because building a `str` from a computed `u8` has no
   spelling. Workaround: return `"\n"` instead of `10` — 0 extra lines here.
   The place this could bite for real is `scan.rs`'s unexpected-character
   path (slicing the offending char out of the source text serves there) and
   any future `to_str`-of-byte want. **Watch, not want, so far.**

6. **No ASCII case fold.** `canonical_int` is `to_ascii_lowercase` in Rust;
   the port maps the six hex digits by hand through a 7-arm `match` in a
   fold, +9 lines. A general `lower()` built-in is exactly what §1.11 says
   comes from C when a program needs it; the lexer needs six letters.
   **Deferral by default.**

## Idioms that carried over without friction (the positive result)

- A 61-case variant and a 61-arm exhaustive `match` as a return expression.
- Contextual case construction (`.kw_if`, `ok(.binary)`) through `ok()` and
  `push()`.
- `match` on `str` with `|` joins and `_`.
- Structural `==` on records and variants (the `ForeignWord` triple).
- Nested-record mutation through `@` (`u.span.start @ 0`) with value
  semantics keeping the original intact.
- `u8` byte scanning: `s[i]`, char literals as `u8` in context, `b - '0'`
  arithmetic, `to_u64(...)/to_i64(...)` with `.must()` at provably-safe sites.

## Still owed by this milestone

- The stateful heart: `number.rs`, `literals.rs`, `layout.rs`, `scan.rs`,
  `mod.rs` — LexState threading through `@` parameters, the diagnostics-and-
  Fix records, terminator insertion, the indent stack.
- The Rust lexer's own tests, translated (`lexer/tests/`, 843 lines): the
  acceptance is that the port passes them.
- The two diagnostic builders deferred from `digits.hero` (need a render-in-
  base helper — the first real string-building want; measure it when it is
  written, not before).
- The fifteenth closure-list row: `hero_spawn`'s measured cost and signature
  (author instruction 2026-08-12 — the ROADMAP's own text).
- If the byte wall stays filled: the second-file question (the lexer has zero
  `BTreeMap` and zero closures, so these findings are a lower bound — the
  ROADMAP already says a maps-and-generics file is owed before
  M-selfhost-port opens).

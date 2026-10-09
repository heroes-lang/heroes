# Panel 203, llm-ergonomist (blind seat): folder `e2-a`

Copied by the coordinator on 2026-10-09 from the session's `report.md`, unchanged below the rule. A fresh `claude -p` session outside the repository (the author's exception of 2026-10-09), `claude-opus-5-5`, `--max-budget-usd 0.6`; started 23:49:31 and ended 23:50:54 by `date`; `run.json`: cost USD and turns 0.3656 6. Its inputs in `docs/panel/203-briefs/blind/e2-a/`, the spec named in `llm-ergonomist-prediction.md`.

---

# Report

## lines

General rule used for every line (section 9): "Generics: on functions only, no constraints, always inferred, never written at the call site" and "A type parameter takes its type from the arguments that carry one, else from the type the context asks for, else from a literal argument (section 2); a call that says none of these is an error."

Bindings in `main` that the lines use: `ns = [1, 2, 3]` is `[i64]` (section 2: "A literal takes the type its context asks for ... otherwise `i64`."), `b: u8 = 7` is a `u8`, `xs = [4, 5]` is `[i64]`.

### Line 19: `print(ns.map(ident).len().to_str())`
- Accepted.
- `map`'s `A` = `i64`, `map`'s `B` = `i64`; `ident`'s `A` = `i64`.
- Prints `3`.
- Basis: "`x.f(y)` is sugar for `f(x, y)` (UFCS)." "Top-level functions are values: `xs.fold(0, add)`." `map` is declared as "`function map<A, B>(xs: [A], f: (function(A) -> B)) -> [B]`", and "`xs.map(double)` takes both from `double`'s signature." `A` comes from `ns`, the argument that carries one ("from the arguments that carry one"), and `ident`'s signature `(function(A) -> A)`, taken at `i64`, gives `B` = `i64`. The result is a `[i64]` of 3 elements; `len` gives 3 and `to_str` writes it. `print` "takes the types this language renders as text: a number, `str` or `bool`."
- This rests on a choice: passing a generic function as a value (see choice_points).

### Line 20: `print(first(a: b, b: 255).to_str())`
- Accepted.
- `first`'s `A` = `u8`.
- Prints `7`.
- Basis: `b` is a `u8`, so it is the argument that carries a type ("takes its type from the arguments that carry one"); `255` is a literal and "A literal takes the type its context asks for", so it is a `u8`, and "a literal must fit its type", which 255 does in `u8`. The two parameters share the type `A`, so names are required and given: "When two parameters in a signature share a type, named arguments are mandatory at the call site". The argument name `b:` names `first`'s parameter; it binds nothing in `main`, so it neither shadows nor conflicts with `main`'s `b`. `first` returns `pair[0]`, which is `a`, i.e. `main`'s `b`, whose value is 7.

### Line 21: `print(g(x: 1, n: ok(2)).to_str())`
- Accepted.
- `g`'s `T` = `i64`.
- Prints `1`.
- Basis: no argument carries a type for `T` (`1` is a literal), and `.to_str()` asks for no particular type, so `T` falls to the last rule, "else from a literal argument (section 2)", and section 2 gives "otherwise `i64`". `n` is declared `i64?`, so `ok(2)` is the `i64?` holding 2 ("`ok(v)` or `fail(code:, msg:)`"). Inside `g`, `n.is_err()` is allowed since `n`'s `T` is `i64`, not fallible ("`.is_err()` ... refused where `T` is itself fallible"). `x` and `n` have different types, so names are not required, but `Arg = [ ident ":" ] [ "@" ] Expression` permits them. `g` returns `x` = 1 on both paths.

### Line 22: `print(first(a: xs, b: []).len().to_str())`
- Accepted.
- `first`'s `A` = `[i64]`.
- Prints `2`.
- Basis: `xs` carries `[i64]` ("from the arguments that carry one"), so `A` = `[i64]` and the empty literal `[]` is given that type by its parameter. Section 10 says "an empty one needs an annotation: `xs: [i64] = []`"; I read the parameter type, fixed by `xs`, as supplying the type the empty literal needs (see choice_points). `first` returns `a` = `xs` = `[4, 5]`, whose `len` is 2.

### Lines 23 and 24: `y: u8 = first(a: 1, b: 2)` then `print(y.to_str())`
- Accepted.
- `first`'s `A` = `u8`.
- Prints `1`.
- Basis: neither argument carries a type (both literals), so `A` takes "the type the context asks for", which is `u8` from the declaration `y: u8 =` (`Binding = ( "=" | ":" Type ( "@" | "=" ) ) Expression NEWLINE`). Both literals are then `u8` ("`b: u8 @ 255`, and `b + 1` is a `u8`") and fit. `first` returns `a` = 1.

### Lines 25 and 26: `x = first(a: ok(1), b: ok(2))` then `print(x.is_err().to_str())`
- Accepted.
- `first`'s `A` = `i64?`.
- Prints `false`.
- Basis: there is no context (`x =` has no annotation: "immutable binding, type inferred"). The arguments `ok(1)` and `ok(2)` are each a `T?` whose `T` comes from a literal, so the literal falls to "otherwise `i64`" and `A` = `i64?` (see choice_points; this is the least certain line). `x` is `ok(1)`. "`.is_err()` | boolean test; refused where `T` is itself fallible": `x` is `i64?` and its `T` is `i64`, not fallible, so the test is allowed and gives `false` ("a `bool` prints `true` or `false`"). `x` is bound and then read, so nothing drops a `T?`.

Summary of output, in order: `3`, `7`, `1`, `2`, `1`, `false`.

## choice_points

1. Line 19, a generic function as a value. The specification says "Top-level functions are values" and "`xs.map(double)` takes both from `double`'s signature", but never says whether a generic function such as `ident` may be passed uncalled. I chose that it may, its own `A` fixed by the parameter type it is passed to (`(function(i64) -> B)`), giving `B` = `i64` and output `3`. The other reading (a generic function has no single type, so it is not a value) refuses line 19.

2. Line 22, the empty array `[]` as an argument. Section 10 says "an empty one needs an annotation: `xs: [i64] = []`". I read "annotation" as any place that fixes its type, here the parameter type `A` already fixed to `[i64]` by `xs`, consistent with section 2's "A literal takes the type its context asks for". Output `2`. The other reading (only a written `name: [T]` annotation will do) refuses line 22.

3. Line 25, `ok(1)` as an argument with no context. The rule lists "from the arguments that carry one, else from the type the context asks for, else from a literal argument". `ok(1)` is not itself a literal, and whether it "carries" a type is unstated. I chose that its literal takes the default `i64`, so it is an `i64?` and `A` = `i64?`, output `false`. The other reading (`ok(1)` neither carries a type nor is a literal argument, so "a call that says none of these is an error") refuses line 25, and line 26 with it.

4. Line 21, named arguments where none are required. `Arg = [ ident ":" ] [ "@" ] Expression` permits them; I treat them as allowed. Refusing them would refuse line 21, but nothing in the text forbids them.

## confidence

- Lines 20, 21, 23/24: high.
- Line 22: medium high (choice 2).
- Line 19: medium (choice 1).
- Lines 25/26: medium low (choice 3); refusal is a credible reading.

## context

Only `brief.md`, `spec.md` and `main.hero` from this directory were read. Beyond them, my context held the harness's system prompt and an automatically attached note with the user's account email address; neither bears on the answer. No other file was read and nothing was run.

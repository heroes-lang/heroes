# Panel 203, llm-ergonomist (blind seat): folder `e1-a`

Copied by the coordinator on 2026-10-09 from the session's `report.md`, unchanged below the rule. A fresh `claude -p` session outside the repository (the author's exception of 2026-10-09), `claude-opus-5-5`, `--max-budget-usd 0.6`; started 23:49:31 and ended 23:50:32 by `date`; `run.json`: cost USD and turns 0.2704 5. Its inputs in `docs/panel/203-briefs/blind/e1-a/`, the spec named in `llm-ergonomist-prediction.md`.

---

# report

## lines

Shared facts used below: `ns = [1, 2, 3]` and `xs = [4, 5]` are `[i64]`, because "A literal takes the type its context asks for ... otherwise `i64`." (section 2), and a plain `=` binding gives no context. `b: u8 = 7` makes `b` a `u8`.

The key rule throughout is section 9: "A type parameter takes its type from the arguments, else from the type the context asks for, and a generic function's parameter asks for none; a call that says neither is an error." I read "a generic function's parameter asks for none" to mean that an argument passed to a generic function gets no expected type from the parameter. So a literal there falls back to `i64`, and an empty `[]` has nothing to take its type from.

### Line 19: `print(ns.map(ident).len().to_str())`: REFUSED
- `map<A, B>(xs: [A], f: (function(A) -> B))`. From `ns`, `map`'s `A` is `i64`. `ident` is passed as a value, not called, so its own type parameter `A` gets no arguments. Its context is `map`'s parameter `f`, and "a generic function's parameter asks for none". So `ident`'s `A` gets a type from neither source, and that is "an error". `map`'s `B` can only come from `f`'s signature ("`xs.map(double)` takes both from `double`'s signature"), and that signature is unresolved, so `B` is unresolved too.
- Quoted: "A type parameter takes its type from the arguments, else from the type the context asks for, and a generic function's parameter asks for none; a call that says neither is an error." "Generics: on functions only, no constraints, always inferred, never written at the call site". "`xs.map(double)` takes both from `double`'s signature."
- Prints nothing (refused).

### Line 20: `print(first(a: b, b: 255).to_str())`: REFUSED
- The named arguments are required and given: "When two parameters in a signature share a type, named arguments are mandatory at the call site". `a: b` gives `A = u8`. The literal `255` gets no expected type from parameter `b` ("a generic function's parameter asks for none"), so it is `i64` ("otherwise `i64`"). That gives `A = i64`, which conflicts with `u8`. There is no conversion to settle it: "No implicit conversions, widths included".
- Prints nothing (refused).

### Line 21: `print(g(x: 1, n: ok(2)).to_str())`: ACCEPTED
- `T = i64`. The argument `1` gets no expected type from the generic parameter `x`, so it is `i64`, and `T` "takes its type from the arguments". Parameter `n` has the concrete type `i64?`, which is not a type parameter. So `ok(2)` is checked against `i64?`, and `2` is an `i64`. Naming the arguments is optional here because `T` and `i64?` are different types. The `Arg` production allows a name: "Arg = [ ident ":" ] [ "@" ] Expression ." Inside `g`, `n.is_err()` is allowed because `i64` is not fallible ("`.is_err()` | boolean test; refused where `T` is itself fallible").
- `g` returns `x` on both paths, so the value is `1`. `to_str` gives `"1"`.
- Prints: `1`

### Line 22: `print(first(a: xs, b: []).len().to_str())`: REFUSED
- `a: xs` gives `A = [i64]`. But `[]` is an empty container literal, and "an empty one needs an annotation: `xs: [i64] = []`". Its only possible context is parameter `b` of a generic function, which "asks for none". So `[]` has no type.
- Prints nothing (refused).

### Lines 23 and 24: `y: u8 = first(a: 1, b: 2)` then `print(y.to_str())`: REFUSED (line 23)
- `A` "takes its type from the arguments" first. Both arguments are literals in a generic parameter's position, which "asks for none", so both are `i64` ("otherwise `i64`") and `A = i64`. The rule reaches "else from the type the context asks for" only when the arguments say nothing, and here they say `i64`. So the `u8` annotation never reaches `A`. `first` then returns an `i64` into a `u8` binding, which is refused: "No implicit conversions, widths included".
- Prints nothing (refused). Line 24 is fine on its own, but it depends on `y`.

### Lines 25 and 26: `x = first(a: ok(1), b: ok(2))` then `print(x.is_err().to_str())`: ACCEPTED
- `ok(1)` and `ok(2)` are `i64?`: each literal is `i64` with no context, and `ok(v)` is the `T?` of `v` ("A `T?` is a `T` or an error: `ok(v)` or `fail(code:, msg:)`"). So `A = i64?`. A type parameter may be fallible: "a type parameter that arrived fallible is still dropped" shows the spec expects one. Inside `first`, `[a, b]` is `[i64?]` and `pair[0]` returns `ok(1)`. `x` is `i64?`, and `.is_err()` is allowed because the inner `T` is `i64`, not fallible ("refused where `T` is itself fallible"). `x` is `ok(1)`, so `is_err()` is `false`, and `to_str` gives `"false"`.
- Prints: `false`

Summary: 21 prints `1`, and 25 to 26 prints `false`. Lines 19, 20, 22 and 23 are refused, so the program as a whole does not compile.

## choice_points

1. **What "a generic function's parameter asks for none" covers.** I took it to mean that a generic function's parameter gives no expected type to the argument in its position. This applies to literals (lines 20, 21, 23, 25), to the empty `[]` (line 22) and to a generic function passed as a value (line 19). The other reading is that the parameter only gives no type through its own type parameter, so arguments can be unified with each other. Under that reading, line 19 is accepted with `map`'s `A = B = i64` and `ident`'s `A = i64`, and prints `3`. Line 20 is accepted with `A = u8`, and `255` fits, so it prints `255`. Line 22 is accepted with `A = [i64]` and prints `2`.
2. **Line 23: do literal arguments "say" a type?** I took the literals to say `i64` through the section 2 default, so the arguments decide `A` and the `u8` context is never consulted. The other reading treats a context-less literal as saying nothing, so `A` comes from the context `u8`. Then the line is accepted with `A = u8`, and line 24 prints `2`.
3. **Line 19: a generic top-level function as a value.** The spec never says outright whether a generic function can be passed uninstantiated. I applied the same type-parameter rule to `ident`'s `A`, which gives no arguments and no context, and so an error. If the checker instead instantiated it from `map`'s `A = i64`, the line would print `3` (see point 1).
4. **Line 21: literal `1` in a generic position.** I gave it the `i64` default, so `T = i64`. If a context-less literal were taken to say nothing (the other reading in point 2), `T` would have no source, and the line would be refused as "a call that says neither". Line 25 would be refused for the same reason.

## confidence

- Line 21 (accepted, `T = i64`, prints `1`) and lines 25 to 26 (accepted, `A = i64?`, prints `false`): high.
- Line 22 (refused): fairly high.
- Line 20 (refused) and line 23 (refused): medium. Both depend on how "asks for none" and the default to `i64` interact.
- Line 19 (refused): medium-low. The spec's `xs.map(double)` example shows functions as values supplying types, but says nothing about a generic one.

## context

Only this directory's files reached my context: `brief.md`, `spec.md` and `main.hero`. The harness also added the user's account email and environment details such as the working directory, platform and date. None of that was used in the analysis. I ran nothing and read no other files.

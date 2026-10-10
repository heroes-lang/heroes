# Panel 203, llm-ergonomist (blind seat): folder `e1-b`

Copied by the coordinator on 2026-10-09 from the session's `report.md`, unchanged below the rule. A fresh `claude -p` session outside the repository (the author's exception of 2026-10-09), `claude-opus-5-5`, `--max-budget-usd 0.6`; started 23:49:31 and ended 23:50:30 by `date`; `run.json`: cost USD and turns 0.2685 5. Its inputs in `docs/panel/203-briefs/blind/e1-b/`, the spec named in `llm-ergonomist-prediction.md`.

---

# report

Each line is judged on its own, as `main.hero`'s comment says: the other five lines are assumed absent (or accepted) when one is judged. The bindings `ns` (`[i64]`, its literals defaulting to `i64`), `b` (`u8`, typed immutable binding) and `xs` (`[i64]`) are all used, so none of them is refused for being unused.

The rule that decides most lines is section 9: "A type parameter takes its type from the arguments, else from the type the context asks for, and a generic function's parameter asks for none; a call that says neither is an error." Together with section 2: "A literal takes the type its context asks for [...] otherwise `i64`."

## lines

**Line 19, `print(ns.map(ident).len().to_str())`: refused.**
- `map`'s `A` would be `i64` from `ns`. Its `B` has to come from `ident`'s signature, but `ident` is itself generic (`ident<A>(x: A) -> A`) and is passed as a value, not called. So nothing tells `ident`'s own `A` what it is. It has no arguments, and the place it sits in is `map`'s parameter `f`, which "asks for none".
- Since `ident` cannot be instantiated, `map`'s `B` cannot be either, and the line is an error.
- Rests on: "A type parameter takes its type from the arguments, else from the type the context asks for, and a generic function's parameter asks for none; a call that says neither is an error." and "`xs.map(double)` takes both from `double`'s signature." (`double` is not generic, so its signature is concrete; `ident`'s is not.)
- If it were accepted (see choice_points): `A = i64`, `B = i64`, `ident`'s `A = i64`, and it prints `3`.

**Line 20, `print(first(a: b, b: 255).to_str())`: refused.**
- `a: b` gives `A = u8`. The literal `255` sits in a parameter of a generic function, which asks for no type, so it is an `i64`. `A` then gets `u8` from one argument and `i64` from the other, and section 3 forbids that mismatch.
- The named arguments are required here and are present: "When two parameters in a signature share a type, named arguments are mandatory at the call site".
- Rests on: "A literal takes the type its context asks for [...] otherwise `i64`.", "a generic function's parameter asks for none", and "No implicit conversions, widths included".
- If it were accepted: `A = u8`, and it prints `7`.

**Line 21, `print(g(x: 1, n: ok(2)).to_str())`: accepted.**
- `T = i64`. The literal `1` is in a generic parameter, which asks for no type, so it defaults to `i64`.
- `n: i64?` is a concrete parameter, so `ok(2)` is an `i64?`.
- Inside `g`, `n.is_err()` is allowed because `n`'s `T` is `i64`, which is not fallible. `g` returns `x` on both paths.
- Prints `1`.
- Rests on: "A type parameter takes its type from the arguments", "a generic function's parameter asks for none", "A literal takes the type its context asks for [...] otherwise `i64`.", "`ok(v)` or `fail(code:, msg:)`", and "`.is_err()` | boolean test; refused where `T` is itself fallible".

**Line 22, `print(first(a: xs, b: []).len().to_str())`: refused.**
- `[]` is an empty container literal, and it gets no type from its context, because a generic function's parameter asks for none. An empty literal needs an annotation, and this one has none.
- Rests on: "an empty one needs an annotation: `xs: [i64] = []`" and "a generic function's parameter asks for none".
- If it were accepted: `A = [i64]`, and it prints `2`.

**Lines 23 and 24, `y: u8 = first(a: 1, b: 2)` / `print(y.to_str())`: refused (line 23).**
- `A` takes its type from the arguments first. The literals `1` and `2` are in generic parameters, which ask for nothing, so each is an `i64`. That makes `A = i64`, and the call returns `i64`.
- The binding's `u8` is only a fallback ("else from the type the context asks for"), and it is not reached because the arguments already decide `A`. Binding an `i64` to `y: u8` is a width mismatch.
- Rests on: "A type parameter takes its type from the arguments, else from the type the context asks for", "A literal takes the type its context asks for [...] otherwise `i64`.", and "No implicit conversions, widths included".
- If it were accepted: `A = u8`, and line 24 prints `2`.

**Lines 25 and 26, `x = first(a: ok(1), b: ok(2))` / `print(x.is_err().to_str())`: accepted.**
- `ok(1)` and `ok(2)` are both `i64?`, with the inner literals defaulting to `i64`. So `A = i64?`, and `x` is an `i64?` bound by `=`. That is not a `_` drop, so the rule about dropping a `T?` does not apply.
- `x.is_err()` is on an `i64?` whose `T` (`i64`) is not fallible, so it is allowed. `x` is `ok(1)`, so the call gives `false`.
- Prints `false`.
- Rests on: "A type parameter takes its type from the arguments", "A literal takes the type its context asks for [...] otherwise `i64`.", "`.is_err()` | boolean test; refused where `T` is itself fallible", and "`to_str`, `to_f32` and `to_f64` cannot [fail], so they give a value". It also relies on the rule that `print` takes a `str`.

## choice_points

1. **Line 19: whether a generic function passed as a value can be instantiated from the parameter it is passed to.**
   - My choice: no. The spec says a generic function's parameter "asks for none", and `ident` has no arguments of its own, so the line is refused.
   - Other reading: unify `ident`'s signature against `(function(i64) -> B)`. That gives `ident`'s `A = i64` and `B = i64`, and the line prints `3`.
2. **Line 20: whether a literal argument takes its type from what another argument has already fixed for `A`.**
   - My choice: no. The literal's context is the generic parameter, which asks for nothing, so `255` is `i64`. That conflicts with `u8`, and the line is refused.
   - Other reading: `A = u8` from `a`, then `255` becomes `u8` (it fits), and the line prints `7`.
3. **Line 22: the same question for the empty `[]`.**
   - My choice: refused, because nothing annotates it.
   - Other reading: `A = [i64]` from `xs` types `[]`, and the line prints `2`.
4. **Line 23: whether bare literals count as arguments that "say" a type, or leave `A` open for the context's `u8`.**
   - My choice: they say `i64` (section 2's default), so `A = i64` and the `u8` binding is refused.
   - Other reading: the literals are flexible, `A = u8` from the binding, and line 24 prints `2`.

I applied the reading consistently: a generic parameter gives no context to the argument written in it, as the spec says in so many words.

## confidence

- Lines 21 and 25/26 (accepted, printing `1` and `false`): high.
- Lines 20, 22 and 23 (refused): medium-high. They follow directly from "a generic function's parameter asks for none", but a checker that unifies argument by argument could accept them.
- Line 19 (refused): medium. The spec does not say outright how a generic function used as a value gets instantiated.

## context

Only `brief.md`, `spec.md` and `main.hero` from this directory were read. The only other things in my context were the harness's environment details and an automatically attached user email address. Neither was used in the analysis.

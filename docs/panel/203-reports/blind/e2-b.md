# Panel 203, llm-ergonomist (blind seat): folder `e2-b`

Copied by the coordinator on 2026-10-09 from the session's `report.md`, unchanged below the rule. A fresh `claude -p` session outside the repository (the author's exception of 2026-10-09), `claude-opus-5-5`, `--max-budget-usd 0.6`; started 23:49:31 and ended 23:50:33 by `date`; `run.json`: cost USD and turns 0.2737 5. Its inputs in `docs/panel/203-briefs/blind/e2-b/`, the spec named in `llm-ergonomist-prediction.md`.

---

# Report on main.hero

Background shared by all lines. `ns = [1, 2, 3]` and `xs = [4, 5]` have no annotation, so their literals are `i64` ("A literal takes the type its context asks for ... otherwise `i64`."), which makes them `[i64]`. `b: u8 = 7` is a `u8`. Every binding in `main` and every parameter of the three generic functions is read, so no line is refused for an unused name. `first` has two parameters of type `A`, so named arguments are required, and every call of it names them ("When two parameters in a signature share a type, named arguments are mandatory at the call site"). Inference follows section 9: "A type parameter takes its type from the arguments that carry one, else from the type the context asks for, else from a literal argument (section 2); a call that says none of these is an error."

## lines

**Line 19, `print(ns.map(ident).len().to_str())`: accepted. Prints `3`.**
- `map<A, B>`: A = `i64` (from `ns`, which is `[i64]`), B = `i64`. `ident<A>`: A = `i64`.
- `ns.map(ident)` is `map(ns, ident)` by UFCS ("`x.f(y)` is sugar for `f(x, y)` (UFCS)."). A comes from the argument that carries a type, `ns`. `ident` is a top-level function used as a value ("Top-level functions are values: `xs.fold(0, add)`."). The parameter type `(function(i64) -> B)` instantiates it at `i64 -> i64`, which gives B = `i64` ("`xs.map(double)` takes both from `double`'s signature."). The result is a 3-element `[i64]`, `len` gives 3, and `to_str` gives `"3"`.

**Line 20, `print(first(a: b, b: 255).to_str())`: accepted. Prints `7`.**
- `first<A>`: A = `u8`.
- The argument `b` carries the type `u8` ("from the arguments that carry one"). The literal `255` then takes the type its context asks for, `u8` ("A literal takes the type its context asks for — `b: u8 @ 255`"). It fits, as "a literal must fit its type" requires. `first` returns `a`, which is 7.

**Line 21, `print(g(x: 1, n: ok(2)).to_str())`: accepted. Prints `1`.**
- `g<T>`: T = `i64`.
- No argument at `T`'s position carries a type, since `1` is a literal. `.to_str()` accepts any number, so the context asks for no particular type. T therefore falls to the literal argument and its default: "else from a literal argument (section 2)", and "otherwise `i64`". `n: ok(2)` takes `i64?` from the parameter, and `n.is_err()` is allowed because `i64` is not itself fallible ("refused where `T` is itself fallible"). Both paths return `x`, which is 1.

**Line 22, `print(first(a: xs, b: []).len().to_str())`: accepted. Prints `2`.**
- `first<A>`: A = `[i64]`.
- `xs` carries `[i64]`, which fixes A. The empty literal `[]` gets its type from the parameter it fills, `[i64]`. The rule "an empty one needs an annotation: `xs: [i64] = []`" exists so the element type is known, and here the parameter type supplies it (see choice_points). `first` returns `a`, which is `xs`, and its `len` is 2.

**Line 23, `y: u8 = first(a: 1, b: 2)` with its `print(y.to_str())` on 24: accepted. Prints `1`.**
- `first<A>`: A = `u8`.
- Neither argument carries a type, since both are literals. The context asks for `u8` through the annotated binding ("else from the type the context asks for"). The literals 1 and 2 become `u8` and fit. `first` returns `a`, so `y` is 1.

**Line 25, `x = first(a: ok(1), b: ok(2))` with its `print(x.is_err().to_str())` on 26: accepted. Prints `false`.**
- `first<A>`: A = `i64?`. (`ok`'s own value type is `i64`.)
- `ok(1)` is a `T?` ("`ok(v)` or `fail(code:, msg:)`"). Nothing gives its inner literal a type, so the literal defaults to `i64` ("otherwise `i64`"). That makes each argument an `i64?`, and A is `i64?`. A fallible type parameter is anticipated by the spec ("a type parameter that arrived fallible"). `x` is bound and read on line 26, so nothing drops a `T?`. `x.is_err()` is allowed because `x`'s `T` is `i64`, which is not fallible. `x` is `ok(1)`, so `is_err()` is `false` and `to_str` gives `"false"`.

## choice_points

1. **Line 19: passing a generic function as a value.** The spec says top-level functions are values and that `map` takes B from the function's signature. It never says what happens when that signature is itself generic. I chose to let `ident` be instantiated by the expected parameter type `(function(i64) -> B)`, giving `i64 -> i64`. Under the other reading, a generic function has no single signature and cannot be a value. Line 19 would then be refused because B is said by none of the three sources.
2. **Line 22: does `[]` need a written annotation?** I read "an empty one needs an annotation" as requiring that the element type be known, and the parameter type, already fixed at `[i64]` by `xs`, supplies it. Read literally, the annotation must be written (as in `xs: [i64] = []`). Line 22 would then be refused.
3. **Line 25: does `ok(1)` carry a type?** I let `ok(1)` settle its inner literal by the section 2 default (`i64`), so the argument carries `i64?` and A = `i64?`; line 26 prints `false`. The other reading takes the section 9 list strictly. `ok(1)` is not a literal argument, and its literal waits for context. No argument carries a type, the binding `x =` asks for none, and there is no bare literal argument. The call would then be "a call that says none of these", an error, and lines 25 and 26 would be refused.
4. **Line 21: whether `.to_str()` is a context that asks for a type.** I took it as asking for none, because it accepts any number, so T falls to the literal default `i64`. No other reading changes the output: any numeric type prints `1`.
5. **Independence of lines.** The program's header says "each line on its own", and the brief asks for each line separately. I judged each line on its own and did not treat a refusal of one as stopping the others. A real checker would refuse the whole file if any line were refused.

## confidence

- High: lines 20, 21 and 23. These follow directly from the stated inference order and the literal rules.
- Moderate: line 22, which depends on choice point 2.
- Moderate: line 19, which depends on choice point 1.
- Moderate to low: line 25, which depends on choice point 3. The strict reading (refused) is a real possibility.

## context

Nothing other than this directory's files reached my context for the task. I read only `brief.md`, `spec.md` and `main.hero`, and I wrote only `report.md`. The harness's automatic session context contained an account email address, which I did not use. The task relied on no outside knowledge of this language.

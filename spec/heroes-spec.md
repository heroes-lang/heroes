# The Heroes Language

Heroes is a small compiled language. This document is the whole language.

## Files and layout
- One file is one program. Entry point: `function main()`.
- `#` comments to end of line, markdown inside. A comment directly above a
  declaration documents it; `##` is a section heading.
- Indentation is significant and rigid: exactly 4 spaces per level; a tab is a
  compile error. No braces, no semicolons, no parentheses around conditions.
- Syntax is ASCII-only; strings and comments may contain any UTF-8.

## Top-level declarations
Every top-level line starts with its kind. A `constant`'s name takes `: type`;
a `function`'s parameter list attaches to its name; `record` and `variant`
declare types, so nothing follows their name.

```
constant MAX_DEPTH: int
    64

function dist2(a: Point, b: Point) -> int
    dx = a.x - b.x
    dy = a.y - b.y
    return dx*dx + dy*dy

record Point
    x: int
    y: int

variant Token
    num
        v: int
    plus
    lparen
```

Declaration order never matters; mutual recursion needs no forward
declarations. There are no mutable globals. Constants use SCREAMING_CASE.

## Types
| Type | Meaning |
|---|---|
| `int` | 64-bit signed integer (the only integer type) |
| `f64` | 64-bit float |
| `bool` | `true` / `false` |
| `str` | immutable UTF-8 string, indexed in bytes |
| `[T]` | dynamic array, indices from 0 |
| `{K: V}` | map |
| `T?` | fallible: a `T`, or an error |

- No implicit conversions: `1 + 2.0` is an error; write `to_f64(x)`, `to_int(x)`.
- No null. Absence is a different type (`T?`).
- Character literals are `int`: `'a'`, `'0'`, `' '`.
- Five escapes, and no others: `\n` `\t` `\\` `\"` in a string, `\'` instead of
  `\"` in a character literal. Any other escape is a compile error.
- `==` is structural equality on everything — ints, strings, records,
  variants, arrays, maps, recursively; a map's insertion order does not affect it.
- Every value behaves as an independent copy: after `b = a`, mutating `b`
  never changes `a`. No aliasing exists anywhere.
- A record or variant holds its fields **by value**, so it may contain itself only
  through `[T]` or `{K: V}`: `children: [Node]` is a tree, `child: Node` has no size.

## Bindings
```
x = 5              # immutable binding, type inferred
v: int @ 0         # mutable declaration — the type is REQUIRED
v @ v + 1          # mutation; only a declared @ name can be mutated
```
`=` binds once, forever. `@` declares a mutable cell and re-binds it. `m[k] @ v` inserts or replaces;
`keys(m) -> [K]` gives the keys, so `for k in sort(keys(m))` walks in order.
Signatures are always explicit; inference is local only. Empty container
literals need an annotation: `xs: [int] = []` · `m: {str: int} @ {}`.
All bindings are initialised. An unused binding or parameter is a compile
error; a read is a use and a write is not, except through an `@` parameter.
Shadowing is a compile error.

## Functions and calls
- Record construction is a call with field names, always mandatory:
  `Point(x: 3, y: 4)`. `Point(3, 4)` does not exist.
- When two parameters in a signature share a type, named arguments are
  mandatory at the call site: `copy(from: a, to: b)`.
- `x.f(y)` is sugar for `f(x, y)` (UFCS). There are no methods, no
  inheritance, no overloading, no default values, no user variadics.
- Mutable parameters are marked `@` in the signature and at the call site:
  `function advance(@l: Lex)` … `advance(@l)`. Semantics: copy in, copy
  out (copy-out always happens, including on early return and `?`). UFCS does
  not apply when the first parameter is `@`.
- Top-level functions are values: `xs.fold(0, add)`. Function type syntax:
  `(function(A) -> B)`, `(function(A, B) -> C)`, `(function() -> C)` — the
  parentheses are mandatory.
- Generics: on functions only, no constraints, always inferred, never written
  at the call site: `function map<A, B>(xs: [A], f: (function(A) -> B)) -> [B]`.

## Control flow
`match` is the only destructuring construct:
```
value = match e
    .num n  => n.v
    .sum s  => sum_of(s.children)
```
- Exhaustive or compile error. `_` as a catch-all arm is FORBIDDEN on
  variants (allowed on `int`/`str`, where exhaustiveness is impossible).
- `_` names anything you do not use: a payload (`.num _ => 0`), a parameter, a
binding (`_ = f(x)`). It binds nothing, so it is never unused and may repeat.
- `|` joins patterns: `.plus | .times => f()`.
- An arm's body is one statement, inline, or an indented block; a block's value
  is its last expression. A `match` may stand as a statement. A jump (`return`,
  `break`, `continue`) is a valid arm body: it yields no value and does not
  constrain the `match`'s type.

`if cond` / `else if` / `else` take only `bool` — there is no truthiness.
Loops: `while cond` and `for x in xs`; `break` and `continue` exist; ranges
are `range(a, b) -> [int]`.

## Failure: `T?`
A `T?` is a `T` or an error. Construct an error with `fail(code, msg)`; codes
are stable snake_case strings; read `e.code` and `e.msg`. No exceptions exist.

| Operation | Meaning |
|---|---|
| `match` on `.ok x` / `.err e` | destructure — the general way |
| `expr?` | propagate the error to the caller (whose return type must be fallible) |
| `.must()` | extract or abort |
| `.default(v)` | extract or fall back |
| `.is_err()` | boolean test |

`?` on a non-fallible value is a compile error. Map access `m[k]` returns
`V?` with code `missing_key`; `has(m, k) -> bool` tests membership. An out-of-bounds array index
aborts; integer overflow aborts; division by zero aborts. `/` and `%` truncate
toward zero, so `-7 / 3` is `-2` and `-7 % 3` is `-1`.

## Operators
```
arithmetic   + - * / %          (int with int, f64 with f64 — never mixed)
             +                  (str with str: concatenation)
comparison   == != < <= > >=
boolean      && || !            (bool only; && and || short-circuit)
```
Precedence, strongest first: call and `.` → unary `-` `!` → `* / %` → `+ -`
→ comparisons → `&&` → `||`. `& | ^ << >> ~` are reserved for future bitwise
use. There is no ternary; `if` is an expression, and so is `match`.

## Strings, arrays, maps
`s[i]` yields an `int` in 0..255 (a byte); iterate characters with
`s.chars()`, which yields single-character `str`. Multi-line literals
separate elements by newline; single-line by comma.

Built-ins: `print(...)` · `len` · `push` · `slice(from:, to:)` (`to` excluded) ·
`chars` · `keys` · `join` · `sort` · `to_int` (truncating; out of range aborts) ·
`to_f64` · `to_str` — and, written in
Heroes: `map` · `filter` · `fold` · `find` · `any` · `all` · `range`.
None of these names may be redeclared. `print` writes its values with no
separator and exactly one trailing newline. An `f64` always prints a point or
exponent: `1.0`, `0.1`, `1e-06`, `1e+23`.

## Tests and holes
```
test "3-4-5 triangle"
    assert dist2(Point(x: 0, y: 0), Point(x: 3, y: 4)) == 25
```
`test` blocks run only when asked for; ordinary builds ignore them. An
`assert` failure shows the source expression and both sides.

`???` is a valid expression anywhere. It is not an error: the compiler
reports what belongs there (the expected type, what is in scope). A program
with holes type-checks everything else but produces no binary.

## FFI
Anything beyond this document — files, sockets, maths, JSON — comes from C
libraries:
```
extern function sqrt(x: f64) -> f64
```
`ptr` is an opaque pointer, `cstr` a C string.

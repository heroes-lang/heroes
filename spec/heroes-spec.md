<!-- heroes-spec v0 — THE PRE-AMENDMENT BASELINE.
     This document is not documentation: it is the prompt handed to a model
     (design.md §1.6, budget ~1500 tokens). v0 faithfully condenses design.md
     Part 4 AS IT STANDS, including its open questions — amendments land only
     with panel verdicts, so the M0 measurement baseline stays recoverable. -->

# The Heroes Language

Heroes is a small compiled language. This document is the whole language.

## Files and layout
- One file is one program. Entry point: `main = function: ()`.
- `#` comments to end of line; comment text is markdown. A comment directly
  above a declaration is its documentation. `##` is a section heading.
- Indentation is significant and rigid: exactly 4 spaces per level; a tab is a
  compile error. No braces, no semicolons, no parentheses around conditions.
- Syntax is ASCII-only; strings and comments may contain any UTF-8.

## Top-level declarations
Everything at top level has one shape. `constant` and `function` declare values
and take `: type`; `record` and `variant` declare types and do not.

```
MAX_DEPTH = constant: int
    64

dist2 = function: (a: Point, b: Point) -> int
    dx = a.x - b.x
    dy = a.y - b.y
    return dx*dx + dy*dy

Point = record
    x: int
    y: int

Token = variant
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
- `==` is structural equality on everything — ints, strings, records,
  variants, arrays, maps, recursively.
- Every value behaves as an independent copy: after `b = a`, mutating `b`
  never changes `a`. No aliasing exists anywhere.

## Bindings
```
x = 5              # immutable binding, type inferred
v: int @ 0         # mutable declaration — the type is REQUIRED
v @ v + 1          # mutation; only a declared @ name can be mutated
```
`=` binds once, forever. `@` declares a mutable cell and re-binds it.
Signatures are always explicit; inference is local only. Empty container
literals need an annotation: `xs: [int] = []` · `m: {str: int} @ {}`.
All bindings are initialised. An unused variable is a compile error.
Shadowing is a compile error.

## Functions and calls
- Record construction is a call with field names, always mandatory:
  `Point(x: 3, y: 4)`. `Point(3, 4)` does not exist.
- When two parameters in a signature share a type, named arguments are
  mandatory at the call site: `copy(from: a, to: b)`.
- `x.f(y)` is sugar for `f(x, y)` (UFCS). There are no methods, no
  inheritance, no overloading, no default values, no user variadics.
- Mutable parameters are marked `@` in the signature and at the call site:
  `advance = function: (@l: Lex)` … `advance(@l)`. Semantics: copy in, copy
  out (copy-out always happens, including on early return and `?`). UFCS does
  not apply when the first parameter is `@`.
- Top-level functions are values: `xs.fold(0, add)`. Function type syntax:
  `(fn(A) -> B)` — the parentheses are mandatory.
- Generics: on functions only, no constraints, always inferred, never written
  at the call site: `map = function<A, B>: (xs: [A], f: (fn(A) -> B)) -> [B]`.

## Control flow
`match` is the only destructuring construct:
```
value = match e
    .num n  => n.v
    .sum s  => sum_of(s.children)
```
- Exhaustive or compile error. `_` as a catch-all arm is FORBIDDEN on
  variants (allowed on `int`/`str`, where exhaustiveness is impossible).
- `_` as a payload name is allowed: `.num _ => 0`.
- `|` joins patterns: `.plus | .times => f()`.
- An arm is an expression or an indented block; a block's value is its last
  expression. `if` is an expression too.

`if cond` / `else if` / `else` take only `bool` — there is no truthiness.
Loops: `for cond` and `for x in xs`; `break` and `continue` exist; ranges are
`range(a, b) -> [int]`.

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
`V?`; `has(m, k) -> bool` tests membership. An out-of-bounds array index
aborts; integer overflow aborts; division by zero aborts (integer division
truncates).

## Operators
```
arithmetic   + - * / %          (int with int, f64 with f64 — never mixed)
comparison   == != < <= > >=
boolean      && || !            (bool only; && and || short-circuit)
```
Precedence, strongest first: call and `.` → unary `-` `!` → `* / %` → `+ -`
→ comparisons → `&&` → `||`. `& | ^ << >> ~` are reserved for future bitwise
use. There is no ternary; `if` is an expression.

## Strings, arrays, maps
`s[i]` yields an `int` in 0..255 (a byte); iterate characters with
`s.chars()`, which yields single-character `str`. Multi-line literals
separate elements by newline; single-line by comma.

Built-ins: `print(...)` · `len` · `push` · `slice(from:, to:)` · `chars` ·
`has` · `join` · `to_int` · `to_f64` · `range` — and, written in Heroes:
`map` · `filter` · `fold` · `find` · `any` · `all`.

## Tests and holes
```
test "3-4-5 triangle"
    assert dist2(Point(x: 0, y: 0), Point(x: 3, y: 4)) == 25
```
`heroes test file.hero` compiles and runs `test` blocks; ordinary builds
ignore them. `assert` failures show the source expression and both sides.

`???` is a valid expression anywhere. It is not an error: the compiler
reports what belongs there (the expected type, what is in scope). A program
with holes type-checks everything else but produces no binary.

## FFI
Anything beyond this document — files, sockets, maths, JSON — comes from C
libraries:
```
extern sqrt = function: (x: f64) -> f64
```
`ptr` is an opaque pointer, `cstr` a C string. (Header binding syntax lands
with the FFI milestone.)

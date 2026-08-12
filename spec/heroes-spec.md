# The Heroes Language

Heroes is a small compiled language. This document is the whole language.

## Files and layout
- One file is one module; the file you compile holds `function main()`.
- `use geom` binds `geom` to `geom.hero`'s declarations, beside this file,
  written qualified: `geom.dist2(a: p, b: q)`, `p: geom.Point`. Every
  module you name needs its own `use`. No aliases, no wildcard.
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
constant MAX_DEPTH: i64
    64

function dist2(a: Point, b: Point) -> i64
    dx = a.x - b.x
    dy = a.y - b.y
    return dx*dx + dy*dy

record Point
    x: i64
    y: i64

variant Token
    num
        v: i64
    plus
    lparen
```

Declaration order never matters; mutual recursion needs no forward
declarations. There are no mutable globals. Constants use SCREAMING_CASE.

## Types
| Type | Meaning |
|---|---|
| `i8` `i16` `i32` `i64` | signed integers, of that many bits |
| `u8` `u16` `u32` `u64` | unsigned integers |
| `f64` | 64-bit float |
| `bool` | `true` / `false` |
| `str` | immutable UTF-8 string, indexed and measured in bytes |
| `[T]` | dynamic array, indices from 0 |
| `{K: V}` | map |
| `T?` | fallible: a `T`, or an error |

- No implicit conversions, widths included: `a + b` needs both the same type, and
  `1 + 2.0` is an error. `to_f64(x)` and `to_i64(x)` convert between kinds of
  number and abort out of range; `fit_i8(x)` … `fit_u64(x)` convert between
  widths and return `T?` — or `T`, where the value cannot fail to fit.
- A literal takes the type its context asks for — `b: u8 @ 255`, and `b + 1` is a
  `u8` — otherwise `i64`. Overflow aborts at every width.
- Character literals are `i64`: `'a'`, `'0'`, `' '`.
- One `i64` is `0x1f` `0o37` `0b11111` or `31`, with `_` between any two digits.
  A leading zero is an error, never octal. Every base writes a value, so
  `0xffffffffffffffff` does not fit and is refused.
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
v: i64 @ 0         # mutable declaration — the type is REQUIRED
v @ v + 1          # mutation; only a declared @ name can be mutated
```
`=` binds once, forever. `@` declares a mutable cell and re-binds it. `m[k] @ v` inserts or replaces;
`keys(m) -> [K]` gives the keys, so `for k in sort(keys(m))` walks in order.
Signatures are always explicit; inference is local only. Empty container
literals need an annotation: `xs: [i64] = []` · `m: {str: i64} @ {}`.
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
- Top-level functions are values: `xs.fold(0, add)`, folding left with the
  accumulator first. Function type syntax:
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
  variants (allowed on `i64`/`str`, where exhaustiveness is impossible).
- `_` names anything you do not use: a payload (`.num _ => 0`), a parameter, a
binding (`_ = f(x)`). It binds nothing, so it is never unused and may repeat.
- `|` joins patterns: `.plus | .times => f()`.
- An arm's body is one statement, inline, or an indented block; a block's value
  is its last expression. A `match` may stand as a statement. A jump (`return`,
  `break`, `continue`) is a valid arm body: it yields no value and does not
  constrain the `match`'s type.

`if cond` / `else if` / `else` take only `bool` — there is no truthiness.
Loops: `while cond` and `for x in xs`; `break` and `continue` exist; ranges
are `range(from: a, to: b) -> [i64]`, `to` excluded.

## Failure: `T?`
A `T?` is a `T` or an error: `ok(v)` or `fail(code, msg)`. Codes are stable
snake_case strings; read `e.code` and `e.msg`. No exceptions exist.

| Operation | Meaning |
|---|---|
| `match` on `.ok x` / `.err e` | destructure — the general way |
| `expr?` | propagate the error to the caller (whose return type must be fallible) |
| `.must()` | extract or abort |
| `.default(v)` | extract or fall back |
| `.is_err()` | boolean test |

Map access `m[k]` returns
`V?` with code `missing_key`. An out-of-bounds index or slice
aborts, and so does a slice that splits a character; integer overflow aborts; integer division by zero aborts. `/` and `%` truncate
toward zero, so `-7 / 3` is `-2` and `-7 % 3` is `-1`.

## Operators
```
arithmetic   + - * / %          (i64 with i64, f64 with f64 — never mixed)
             +                  (str with str: concatenation)
comparison   == != < <= > >=
boolean      && || !            (bool only; && and || short-circuit)
bitwise      & | ^ ~ << >>      (i64 only; shift count 0..63 or it aborts)
```
Precedence, strongest first: call and `.` → unary `-` `!` `~` → `* / %` → `+ -`
→ `<<` `>>` → `&` → `^` → `|` → comparisons → `&&` → `||`. There is no ternary; `if` is an expression, and so is `match`.

## Strings, arrays, maps
`s[i]` yields a `u8`; iterate characters with
`s.chars()`, which yields single-character `str`. Multi-line literals
separate elements by newline; single-line by comma.

Value semantics has a price: `+` on `str` copies both sides and `push` copies the
array, so accumulating either in a loop is quadratic. `join` is linear.

Built-ins: `print(...)` · `len` · `push` · `slice(from:, to:)` (`to` excluded) ·
`chars` · `keys` · `join(xs, sep)` · `sort` · `to_i64` (truncating; out of range aborts) ·
`to_f64` · `to_str` · `fit_i8` `fit_i16` `fit_i32` `fit_i64` `fit_u8` `fit_u16`
`fit_u32` `fit_u64` — and, written in
Heroes: `map` · `filter` · `fold` · `find` · `any` · `all` · `range`.
None of these names may be redeclared. `print` writes its values with no
separator and exactly one trailing newline. An `f64` prints a point or
exponent (`1.0`, `1e-06`), or `inf`, `-inf`, `nan`.
Files and the process, also provided: `read_file(path: str) -> str?` ·
`write_file(path: str, text: str) -> ()?` · `args() -> [str]` (the arguments
after the program name) · `exit(code: i64)` (ends the program).

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
Anything beyond this document — sockets, maths, JSON, databases — comes from C
libraries. A group names its header and its library, and clang checks every
signature and constant against that header, so a wrong FFI type is a compile error:
```
extern "sqlite3.h" link "sqlite3"
    constant SQLITE_OK: i64
    function sqlite3_open(path: cstr, @out: ptr) -> i64
    function sqlite3_close(db: ptr) -> i64
```
A group's `constant` has no body: the header holds the value. `ptr` is an opaque
pointer whose only literal is `nullptr`, `cstr` a C string, and `s.cstr()` passes a
`str` to C. A C out-parameter is an `@` parameter.

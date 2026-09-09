# The Heroes Language

Heroes is a small compiled language. This document is the whole language.

## Files and layout
- One file is one module; the file you compile holds `function main()`.
- `use geom` binds `geom` to `geom.hero`'s declarations, written
  qualified: `geom.dist2(a: p, b: q)`, `p: geom.Point`. Every
  module you name needs its own `use`. No wildcard.
- A `use` may be a path: `use syntax/decl` binds `decl`, the last part, and
  `use syntax/decl as sd` binds `sd` instead. A file may not bind one name twice;
  two files may name the same module differently. Every `use` starts at the
  directory of the file you compile.
- `#` comments to end of line, markdown inside. A comment directly above a
  declaration documents it; `##` is a section heading.
- Indentation is significant and rigid: exactly 4 spaces per level; a tab is a
  compile error. No braces, no semicolons, no parentheses around conditions.
- Syntax is ASCII-only; comments may contain any UTF-8, strings any but a raw
  carriage return.

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
declarations. There are no mutable globals. Constants use SCREAMING_CASE, and
a written body computes over literals and other constants.

## Types
| Type | Meaning |
|---|---|
| `i8` `i16` `i32` `i64` | signed integers, of that many bits |
| `u8` `u16` `u32` `u64` | unsigned integers |
| `f32` `f64` | floats, of that many bits — `f32` is C's `float` |
| `bool` | `true` / `false` |
| `str` | immutable UTF-8 string, indexed and measured in bytes |
| `[T]` | dynamic array, indices from 0 |
| `{K: V}` | map |
| `T?` | fallible: a `T`, or an error |
| `()` | nothing: what a function with no `->` returns; `ok()` is the `()?` |
| `ptr` `cstr` | an opaque pointer and a C string; `nullptr` is the null of both |

- No implicit conversions, widths included: `a + b` needs both the same type, and
  `1 + 2.0` is an error. Convert with `to_<type>`, and the name says whether it
  can fail: `to_str`, `to_f32` and `to_f64` cannot, so they give a value; `to_i8`
  … `to_u64` give a `T?`, because the number may not fit. `to_i64` takes a float
  too, truncating toward zero. Nothing fails to fit a float: too large is `inf`,
  and `to_f32` rounds.
- A literal takes the type its context asks for — `b: u8 @ 255`, and `b + 1` is a
  `u8` — otherwise `i64`. Overflow aborts at every width.
- A character literal is an integer: `'a'`, `'0'`, `' '`.
- One `i64` is `0x1f` `0o37` `0b11111` or `31`, with `_` between any two digits.
  A leading zero is an error, never octal. Every base writes a value, so a literal must fit its type.
- Six escapes, and no others: `\n` `\t` `\r` `\\` `\"` in a string, `\'` instead
  of `\"` in a character literal. Any other escape is a compile error.
- `==` is structural equality on any two values of one type, recursively; a map's
  insertion order does not affect it. A `ptr`, a `cstr` and a function value compare
  as an address, and `nan` equals nothing, itself included, so `x != x` asks whether it is one.
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
`keys(m) -> [K]` gives the keys in no order; `for k in sort(keys(m))` walks
them in order.
Signatures are always explicit; inference is local only. Empty container
literals need an annotation: `xs: [i64] = []` · `m: {str: i64} @ {}`.
All bindings are initialised. An unused binding or parameter is a compile
error; a read is a use and a write is not, except through an `@` parameter.
A line that computes a value must use it: bind it, or discard it on purpose with
`_ = f(x)`, which a `()` line refuses: it stands alone.
A `_` never drops a `T?`: not `_ = e`, and not a `_` parameter written one.
Answer the error. Both read the OUTERMOST type, so a `[T?]`, a record holding
one, or a type parameter that arrived fallible is still dropped.
Shadowing is a compile error: a `use` binds its name for the whole file, so nothing else in the file may take it.

## Functions and calls
- Record construction is a call with field names, always mandatory:
  `Point(x: 3, y: 4)`.
- When two parameters in a signature share a type, named arguments are
  mandatory at the call site: `copy(from: a, to: b)`.
- `x.f(y)` is sugar for `f(x, y)` (UFCS). There are no methods, no
  inheritance, no overloading, no default values, no user variadics, no
  anonymous functions.
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
  A type parameter takes its type from the arguments, else from the type the
  context asks for; a call that says neither is an error.

## Control flow
`match` is the only destructuring construct:
```
value = match e
    .num n  => n.v
    .sum s  => sum_of(s.children)
```
- Exhaustive or compile error. `_` as a catch-all arm is FORBIDDEN on
  variants (allowed on `i64`/`str`, where exhaustiveness is impossible).
- `_` names anything you do not use: a payload (`.num _ => 0`), a parameter. It
binds nothing, so it is never unused and may repeat.
- `|` joins patterns: `.plus | .times => f()`.
- An arm's body is one statement, inline, or an indented block; a block's value
  is its last expression. A `match` may stand as a statement. A jump (`return`,
  `break`, `continue`) is a valid arm body: it yields no value and does not
  constrain the `match`'s type. An arm that does nothing is a block holding
  `_ = 0` — `continue` is not one.

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
`V?` with code `missing_key`, and `find` fails `not_found`. An out-of-bounds index or slice
aborts, and so does a slice that splits a character; integer division by zero aborts; so does recursion too deep. `/` and `%` truncate
toward zero, so `-7 / 3` is `-2` and `-7 % 3` is `-1`.

## Operators
```
arithmetic   + - * / %          (both sides one numeric type — never mixed)
             +                  (str with str: concatenation)
comparison   == != < <= > >=    (`< <= > >=`: a number only, and a `nan` aborts)
boolean      && || !            (bool only; && and || short-circuit)
bitwise      & | ^ ~ << >>      (i64 only; shift count 0..63 or it aborts)
```
Precedence, strongest first: call and `.` → unary `-` `!` `~` → `* / %` → `+ -`
→ `<<` `>>` → `&` → `^` → `|` → comparisons → `&&` → `||`. There is no ternary; `if` is an expression, and so is `match`.

## Strings, arrays, maps
`s[i]` yields a `u8`; iterate characters with
`s.chars()`, which yields single-character `str`. Multi-line literals
separate elements by newline; single-line by comma.

`+` on `str` copies both sides — a concatenation loop is quadratic; `join` and
`repeat` build in one pass. `xs @ xs.push(4)` grows in place while nothing else
holds `xs`.

Built-ins: `print(...)` · `len` · `push` · `slice(from:, to:)` (`to` excluded) ·
`chars` · `keys` · `join(xs, sep)` · `repeat(s, n)` · `sort` (a number, `str` or `bool`, never a type parameter; a `nan` aborts) ·
`to_f32` · `to_f64` · `to_str` · `to_i8` `to_i16` `to_i32` `to_i64` `to_u8` `to_u16`
`to_u32` `to_u64` — and, written in Heroes: `map` · `filter`, keeping what
the function accepts · `fold` · `find`, the first it accepts, or an error ·
`any` · `all` · `range`.
`print` writes its values with no
separator and exactly one trailing newline, and takes the types this language
renders as text: a number, `str` or `bool`. A float prints a point or
exponent (`1.0`, `1e-06`), or `inf`, `-inf`, `nan`; a `bool` prints `true` or `false`.
Files and the process, also provided: `read_file(path: str) -> str?` ·
`write_file(path: str, text: str) -> ()?` · `args() -> [str]` (the arguments
after the program name; one that is not UTF-8 aborts) · `exit(code: i64)` (ends
the program) · `validated(c: cstr) -> str?` (text from C) ·
`args_checked() -> [str?]` (which does not).

## Tests and holes
```
test "3-4-5 triangle"
    assert dist2(a: Point(x: 0, y: 0), b: Point(x: 3, y: 4)) == 25
```
`test` blocks run only when asked for; ordinary builds ignore them. An
`assert` failure shows the source expression, and both sides where `print`
takes them; where a side is an aggregate it shows the expression alone, until
this language renders one.

`???` is a valid expression anywhere. It is not an error: the compiler
reports what belongs there (the expected type, what is in scope). A program
with holes type-checks everything else but produces no binary.

## FFI
Anything beyond this document — sockets, maths, JSON, databases — comes from C
libraries. A group names its header, and `link` a library when the symbols need one. clang
checks every result type, constant and record field against that header, and a result may be
wider than C's. A **parameter** and a **field** are declared at the header's own
width and sign — `i32` where C says int, `u64` where it says `size_t` — and one that
disagrees is refused, except a parameter C converts exactly (`i16` against int)
and what a `ptr` points at. A C out-parameter is an `@` parameter, and what it
points at is held to the same width and sign — `@n: u64` where it says
`size_t *`:
```
extern "sqlite3.h" link "sqlite3"
    constant SQLITE_OK: i64
    function sqlite3_open(path: cstr, @out: ptr) -> i64
    function sqlite3_close(db: ptr) -> i64
```
A callback is a **parameter**, never a result; its parameters follow the same rule
and `()` is `void`: `atexit(f: (function() -> ()))`.
`owned sqlite3_free` after a `cstr` result or a `char **` out-parameter: the
compiler frees that string with that function, hands it over as a `str?` (the
`@` cell is only written), and refuses your own call of it. Unmarked pointers
are never freed.
Where a library lives is the machine's answer, not the program's, so a group may
name a **package** instead of a library: `extern "raylib.h" package "raylib"`
asks the system where its headers and libraries are and what else it needs. A
package answering with anything this compiler does not pass on is refused,
naming what it said.

A group's `record` is the header's struct: all its fields, and the same name
unless the header writes it after the word struct, which `tag` gives:
`record FileStat tag stat partial`. A
field is a number, `bool`, `ptr`, `cstr`, another record of the group, or a fixed
array of one: `i32[4]`, never a `[T]`; build one with `[a, b, c, d]`.
`record Font partial` names only some, and then comparing it and using it as a
map key are compile errors — for it and for any value holding it. Its size stays
C's, not the field list's.

A group's `constant` has no body: the header holds the value. `s.cstr()` lends a
`str` to C, and the lend may stand only as an argument of a call; outside a
group nothing answers `cstr` and no record holds one. `c.validated()` copies one
back as a `str?`.

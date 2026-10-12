# Reserved words and their prescribed errors

design.md §4.17: the likeliest mistake from a model writing on autopilot is a
keyword imported from another language. Each one is a **loud failure with the
solution pre-written** — the lexer recognises these words and emits the exact
text below; where the word stands in a place its own repair does not fit, such
as `struct` inside a body or `int(` called, the message says instead what is
right there (`selfhost/foreign_at.hero`). This file is the registry; the golden
tests in `check/` pin the text, and the `spec` suite holds each row to
`selfhost/keywords.hero`'s `declared_word`, or runs it.

| Foreign word | Error text |
|---|---|
| `int` | error: `int` is not a word in this language — an integer says its width: `i64`, and also `i8` `i16` `i32` `u8` `u16` `u32` `u64` |
| `struct` | error: `struct` is not a word in this language — use `record`: `record Point` |
| `enum` | error: `enum` is not a word in this language — use `variant`: `variant Token` |
| `union` | error: `union` is not a word in this language — use `variant`: `variant Token` |
| `class` | error: `class` is not a word in this language — use `record` (there is no inheritance) |
| `fn` | error: `fn` is not a word in this language — use `function`: `function f(x: i64) -> i64` |
| `func` | error: `func` is not a word in this language — use `function` |
| `def` | error: `def` is not a word in this language — use `function` |
| `let` | error: `let` is not a word in this language — bind with `=`: `x = 5` |
| `var` | error: `var` is not a word in this language — declare a mutable cell with `@=`: `v @= 0` |
| `const` | error: `const` is not a word in this language — use `constant`: `constant MAX: i64` |
| `elif` | error: `elif` is not a word in this language — write `else if` |
| `switch` | error: `switch` is not a word in this language — use `match` |
| `case` | error: `case` is not a word in this language — a `match` arm is `.name => expr` |
| `null` / `nil` / `None` | error: there is no null in this language — absence is a fallible type: `i64?`, and a C pointer's zero is `nullptr` |
| `try` / `catch` / `throw` / `raise` | error: there are no exceptions in this language — errors are values: `fail(code, msg)`, propagate with `?` |
| `return` used as `return;` | error: `;` is not part of the language's syntax — a line's end ends what the line holds, a statement, a field or a case, and no `;` ends one |
| `return` with no value, in a function with `-> i64` | error: this function returns `i64`, so `return` needs a value |
| `import` / `include` | error: `import` and `include` are not words in this language — a module is named with `use`: `use geom` |

Also prescribed (design.md §4.17): `@name` in **prefix statement position**
(Python/Ruby decorator prior) is always a syntax error at the `@`, whose message
names the legal form the line reads as: `v: T @= e` declaring a cell, `v @ e`
mutating one, or `f(@v)`, an argument lent to a mutable parameter.

Two parser-position prescriptions joined them with panel 018, worded in the
parser because their words are legal elsewhere: `for` without `in` (the Go
condition-loop prior) fails with the fix `use \`while\`` — `Certain` when no
loop variable can be present, `Guess` after one; and a trailing `:` on any
block header (the Python suite colon, the shape's top pre-registered slip)
fails with a `Certain` fix that deletes it.

Keywords of Heroes itself (cannot be identifiers): `constant` `function`
`record` `variant` `match` `if` `else` `for` `while` `in` `break` `continue` `return`
`test` `assert` `extern` `nullptr` `use` `true` `false` `fail` — plus the two-character forms
`=>` `->` `@=` and the sigil `@`.

# Reserved words and their prescribed errors

design.md §4.17: the likeliest mistake from a model writing on autopilot is a
keyword imported from another language. Each one is a **loud failure with the
solution pre-written** — the lexer recognises these words and emits the exact
text below (~5 lines of code each). This file is the registry; the diagnostic
renderer reads from it conceptually, the golden tests in `check/` pin it.

| Foreign word | Error text |
|---|---|
| `struct` | error: `struct` is not a word in this language — use `record`:  `record Point` |
| `enum` | error: `enum` is not a word in this language — use `variant`:  `variant Token` |
| `union` | error: `union` is not a word in this language — use `variant`:  `variant Token` |
| `class` | error: `class` is not a word in this language — use `record` (there is no inheritance) |
| `fn` | error: `fn` is not a word in this language — use `function`:  `function f(x: int) -> int` |
| `func` | error: `func` is not a word in this language — use `function` |
| `def` | error: `def` is not a word in this language — use `function` |
| `let` | error: `let` is not a word in this language — bind with `=`:  `x = 5` |
| `var` | error: `var` is not a word in this language — declare a mutable with `@`:  `v: int @ 0` |
| `const` | error: `const` is not a word in this language — use `constant`:  `constant MAX: int` |
| `elif` | error: `elif` is not a word in this language — write `else if` |
| `switch` | error: `switch` is not a word in this language — use `match` |
| `case` | error: `case` is not a word in this language — a `match` arm is `.name => expr` |
| `null` / `nil` / `None` | error: there is no null in this language — absence is a fallible type:  `int?` |
| `try` / `catch` / `throw` / `raise` | error: there are no exceptions in this language — errors are values:  `fail(code, msg)`, propagate with `?` |
| `return` used as `return;` | error: `return` with no value only in functions returning `()` |
| `import` / `include` | error: `import` and `include` are not words in this language — a module is named with `use`:  `use geom` |

Also prescribed (design.md §4.17): `@name` in **prefix statement position**
(Python/Ruby decorator prior) is always a syntax error with a note showing the
two legal `@` forms (`v: int @ 0` declaration, `v @ expr` mutation).

Two parser-position prescriptions joined them with panel 018, worded in the
parser because their words are legal elsewhere: `for` without `in` (the Go
condition-loop prior) fails with the fix `use \`while\`` — `Certain` when no
loop variable can be present, `Guess` after one; and a trailing `:` on any
block header (the Python suite colon, the shape's top pre-registered slip)
fails with a `Certain` fix that deletes it.

Keywords of Heroes itself (cannot be identifiers): `constant` `function`
`record` `variant` `match` `if` `else` `for` `while` `in` `break` `continue` `return`
`test` `assert` `extern` `use` `true` `false` `fail` — plus the two-character forms
`=>` `->` and the sigil `@`.

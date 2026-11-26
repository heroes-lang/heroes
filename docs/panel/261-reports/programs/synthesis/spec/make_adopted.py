"""Panel 261's synthesis: the adopted text, applied to the base's spec.

Usage: python3 -I make_adopted.py <base spec> <out text>
Every anchor must match the base exactly once; the script stops otherwise.
"""
import sys

base_path, out_path = sys.argv[1], sys.argv[2]
text = open(base_path, encoding="utf-8").read()

REPLACEMENTS = [
    # § 3: the marks in a function type, `Param`'s shape with the name optional.
    (
        '    TypeArg  = Type | ident ":" Type .\n',
        '    TypeArg  = "@" [ ident ":" ] Type | [ ident ":" ] Type [ "lent" ] .\n',
    ),
    # § 5: a write of a place to itself is no write.
    (
        "an `@` parameter nothing writes or ends (section 13), an `@` argument counting; a read",
        "an `@` parameter nothing writes or ends (section 13), an `@` argument counting and `x @ x` not; a read",
    ),
    # § 9: the field first at a dot (panel 079, design.md §4.11).
    (
        "- `x.f(y)` is sugar for `f(x, y)` (UFCS), except a thread's `wait` and a mailbox's `send`,\n",
        "- `x.f(y)` calls `x`'s field `f` where it has one, and is otherwise sugar for\n"
        "  `f(x, y)` (UFCS), except a thread's `wait` and a mailbox's `send`,\n",
    ),
    # § 9: functions as values, with their marks; the fits; the join; the extern.
    (
        "- Top-level functions are values, but not one with an `@` or `lent` parameter:\n"
        "  `xs.fold(0, add)`.\n",
        "- Top-level functions are values, typed with the marks their signature writes,\n"
        "  and a call through a value marks its arguments as its type does:\n"
        "  `xs.fold(0, add)`; `advance` above is a `(function(@Lex) -> ())`, called\n"
        "  `f(@l)`, and `peek` a `(function(Lex lent) -> ())`, called `g(l)`. The marks\n"
        "  are part of the type, but `lent` fits where a type asks `@`: `peek`, and\n"
        "  `skip(_: Lex lent)`, are values of `advance`'s type too; and a function whose\n"
        "  parameters share no type names none, so it fits a type naming them. Where the\n"
        "  elements of a list or a map, the branches of an `if` or the arms of a `match`\n"
        "  are such values, they take the type each of them fits, in any order. A\n"
        "  function of an `extern` group is no value where a parameter is `@`, or a\n"
        "  record `lent`: those marks are C's (section 13).\n",
    ),
    # § 11: `spawn` takes no marked function.
    (
        "`spawn(f, x)` runs `f(x)` on a thread of its own and gives a `(thread T)?`",
        "`spawn(f, x)` runs `f(x)` on a thread of its own, `f` taking `x` with neither mark, and gives a `(thread T)?`",
    ),
    # § 13: a callback's marks, as C meets them.
    (
        "A callback is a **parameter**, never a result or a signal handler, nor called by one; its parameters follow the same rule\n"
        "and `()` is `void`: ",
        "A callback is a **parameter**, never a result or a signal handler, nor called by one; its parameters follow the same rule,\n"
        "an `@` one the place C hands it and a `lent` one that place through a pointer to const, a number's too, and a null place,\n"
        "or two that overlap where one is `@`, aborts, naming the callback; `()` is `void`: ",
    ),
]

for old, new in REPLACEMENTS:
    count = text.count(old)
    if count != 1:
        sys.exit(f"anchor matched {count} times, not once: {old[:70]!r}")
    text = text.replace(old, new)

open(out_path, "w", encoding="utf-8").write(text)

# 003 — `heroes fmt` deletes a control form used inside an expression

Date: 2026-08-26, during the first repo-wide `heroes fmt` sweep (the sweep was
reverted; nothing shipped). **Found by the sweep itself**, because formatting
file 50 of 165 broke every one of the 107 files after it: `fmt` refused them
with a diagnostic pointing into the file it had just written.

**Status: fixed 2026-08-26**, two repairs in one commit — see § The repair.

Severity: **★★★** — `fmt` rewrote a compiler source file into a **different
program that does not parse**, at **exit 0**, with the author's file replaced.
This is the worst shape a formatter's bug can take, and it is the third instance
of the same class (panel 014, panel 060/061, this).

## The symptom

`heroes fmt --in-place selfhost/digits.hero` exited 0 and wrote this:

```
function canonical_int(text: str) -> str
    out: str @ ""
    for c in text.chars()
        out @ out + match c
    return out
```

Every arm was gone. What the file said before:

```
    for c in text.chars()
        out @ out + match c
            "A" => "a"
            "B" => "b"
            "C" => "c"
            "D" => "d"
            "E" => "e"
            "F" => "f"
            _ => c
```

Seven arms deleted, `31` `=>` in the file down to `24`. The next `heroes fmt` on
any file that `use`s `digits` then reported

```
selfhost/digits.hero:238:1: error[missing_match_arms]: a `match` needs its arms
indented one level below it, found the end of the block
error: refusing to format a file with diagnostics
```

which is how the sweep noticed. Had `digits.hero` been the last file formatted,
nothing would have noticed.

## The cause

`print_fmt.hero`'s `render()` returns a control form's **header alone**. Its own
comment says so, and names the contract that makes it safe:

```
        # Headers only: valued() prints the blocks under them.
        .if_expr ie =>
            ...
        .match_expr me => return "match " + render(tree, s, id: me.scrutinee)
```

`valued()` does print the blocks — when the value **is** a control form. It
matches on `tree.exprs[value].kind` and has arms for `.if_expr` and
`.match_expr`. A value that *contains* a control form has some other kind:
`out + match c` is a `.binary`, which fell into

```
        .name | .hole | .unary | .binary | .field | .index => plain_valued(...)
```

and `plain_valued` calls `render()`. Header printed, body dropped.

**The contract was true of one shape and was written as though it were true of
the type.** That is the whole defect, and it is CLAUDE.md §11's premise rule at
the level of an invariant between two functions: `render`'s comment states the
obligation it is owed and nothing checks that the obligation is met.

## The class, measured — nine shapes, not one

The provoking case is a witness, not the class (CLAUDE.md §1). Measured with
probes before the repair:

| shape | before |
|---|---|
| `out @ out + match c` | **all arms deleted** |
| `out @ out + p + match c` | **all arms deleted** |
| `return "x" + match c` | **all arms deleted** |
| `match` in an arm of a `match` at a binary's edge | **all four deleted** |
| `out @ out + if c == "a" … else …` | **the `else` branch deleted** |
| `return match c … + "x"` (control form on the **left**) | **all arms deleted** |
| `out @ match c` | fine |
| `return match c` | fine |
| `v = match c` | fine |

So it is not about `match`: it is **any control form that is an operand of a
binary operator**. `if` behaves identically because `render` treats both the
same way.

## The repair

**Two repairs, and the second is the one that matters.**

1. `print_fmt.hero` — `valued()` grows a `.binary` arm that asks
   `binary_control_tail()` whether the expression's right edge is a control
   form. If it is, everything left of it is rendered by `binary_prefix()` and
   handed to `valued()` **as the head**, which is the path that already knows
   how to print a block under a head. A control form runs to the end of its line
   and carries its block indented below, so it can only ever be the rightmost
   operand — that is what makes the prefix printable as a head.

2. `cli_syntax_cmds.hero` — **`fmt` now reads back what it wrote.**
   `refuse_output_the_formatter_broke()` re-parses the formatted text and
   re-formats it, and refuses at **exit 2** — the compiler's own error class
   (§7) — if the output does not parse or is not a fixpoint. The author's file is
   left untouched.

The second repair is worth more than the first. Repair 1 fixes the shapes
somebody thought of; repair 2 turns **every shape nobody thought of** from
"silently writes a different program" into "stops and says it is a compiler
bug". One shape in the table above is still unspellable by the printer — the
control form on the **left**, with the expression continuing after its block,
which `heroes check` accepts as legal Heroes — and it now refuses loudly instead
of destroying the file. `tests/harness` has no case for it because the guard's
test lives with the guard, in `cli_syntax_cmds.hero`, and it fires on exactly
that shape.

## Why this is the third instance, and what the instrument owes

- **Panel 014**: an arm whose body was a control form. *"`heroes fmt` silently
  deletes code."* The post-mortem then: *"`heroes fmt` had no test with a control
  form in an arm body."*
- **Panels 060 / 061**: `fmt` hoisted a `record` out of its `extern` group, and
  deleted the word `partial` — *"two blind spots cancel into a green test"*,
  because `print_dump` dropped the same word and `assert_canonical` compared the
  two dumps.
- **This**: a control form as a binary operand.

Every one was found by a person. The guard that was supposed to find them,
`assert_canonical` — `dump(text) == dump(fmt(text))` — **exists nowhere in the
selfhost compiler.** It was in the Rust bootstrap
(`archive/bootstrap-rs/heroes/src/printer/tests/mod.rs:45-51`) and was not
ported; `print_dump.hero:12` still describes it as though it were live.

Repair 2 is not that guard and does not replace it. It catches output that does
not **parse**; `assert_canonical` catches output that parses and **means
something else**, which is the panel 061 case. Restoring it is still owed, and
the obstacle is real: the input here is parsed with the whole module graph while
the output is one file, so the two dumps are not comparable without splitting the
parse. That is written down in the guard's own comment so the next reader does
not mistake one property for the other.

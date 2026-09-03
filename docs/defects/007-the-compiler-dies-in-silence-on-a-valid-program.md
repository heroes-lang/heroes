# 007 — `heroes check` exits 139 without a word on a legal program

Date: 2026-09-03, M-corpus-depth step 5. **Found by writing a program**, and
then by asking the obvious next question: the interpreter being written has a
recursive-descent parser, and so does the compiler.

**Status: OPEN.** Nothing is fixed here. The program that found it works around
its own half by nesting 60 deep instead of 500, with every ceiling measured in
its comment (`examples/interpreter/main.hero`).

Severity: **★★★★** — design.md §1.12 says a Heroes program must not segfault,
and the compiler is a Heroes program. `heroes check` on a **valid** file exits
**139** (SIGSEGV) having printed **nothing at all**: no diagnostic, no panic, no
`internal error`. The runtime's stack guard (panel 104) exists to turn exactly
this into a named panic, and in the neighbouring stage it does.

## The program, and it is legal Heroes

```
function main()
    print(((((… 440 parentheses … 1 …)))))
```

Generated, not typed: `'function main()\n    print(' + '('*n + '1' + ')'*n + ')'`.

## What each verb says, measured 2026-09-03 on the author's Mac with the
compiler built from `seed/heroes.c`

| n | `heroes lex` | `heroes parse` | `heroes check` |
|---|---|---|---|
| 420 | 0 | 0 | **0** |
| 440 | 0 | 0 | **139, silent** |
| 500 | 0 | **134** — `panic: stack exhausted in grammarexpr.postfix` | **139, silent** |
| 1000 | 0 | 134 | 139, silent |

Three things in that table matter more than the ceiling itself.

**The lexer never fails**, at any depth, because it is a loop. So the failure is
recursion and nothing else.

**`parse` and `check` disagree about the same file.** At n = 500 the parse stage
overflows and the guard names the function it was in; the check stage overflows
and the process dies with no output. Whatever the guard is reading, it reads it
in one of these stages and not in the other. **The guard is not broken in
general** — a small-frame recursion is caught at both `-O0` and `-O2`:

```
function down(n: i64) -> i64
    if n <= 0
        return 0

    return 1 + down(n - 1)
```

`down(1000000)` is `panic: stack exhausted in down1000000.down` at exit 134,
built either way. So this is a coverage gap in one shape, not an absent guard.

**And the ceiling is low.** 440 parentheses is not an adversarial input; it is
the kind of expression a generated program or a machine-written test can hold,
and the corpus's own new interpreter meets the same wall at 200 in its `-O0`
configuration.

## The measured cause of the low ceiling, which is a separate finding

`heroes build --emit-c` over `examples/interpreter/`'s parser shows the frames:
**247 hoisted locals in the prologue of `syn/expr.hero`'s `compared`**, 151 in
`primary`. CLAUDE.md §7 puts every local in the prologue, so a recursive
function's frame is proportional to its whole **body** rather than to what is
live at any one point, and one nesting level of a seven-level precedence
grammar passes through all seven of those frames plus `grouped`. Under
`--sanitize` the same chain measured **11,728 bytes between `bp` and `sp` for a
single frame** (ASan's own report, which pads stack variables, so that number is
an upper bound rather than the plain frame size).

Two ceilings of the same program, same machine, same day:

```
examples/interpreter/, nesting depth      --sanitize 120 ok / 160 over
                                          -O0        190 ok / 200 over (139, silent)
                                          -O2        340 ok / 360 over (134, named)
```

**A hypothesis, stated as one because it has not been run**: a frame this large
moves the stack pointer past the guard region in a single step, so the fault
lands where the handler declines to call it a stack overflow, and the signal is
re-raised. Confirming it means reading `runtime/parts/stack.c`'s two witnesses
(the faulting address and the stack pointer) against these numbers — which is
the fix's job, not this file's.

## What is owed

1. **`heroes check` must not exit 139 in silence.** Whatever the ceiling turns
   out to be, crossing it has to name itself, as `parse` already does. This is
   §1.12 at its plainest.
2. **The guard's coverage needs a case per shape**, not per language: a
   `fixedbugs` program with a wide-frame mutual recursion, run in all three
   configurations, asserting exit 134 and a message — the shape that is missed
   today.
3. **The hoisted-frame question is architecture** (CLAUDE.md §4): a temporary
   that is live inside one basic block does not need a slot for the whole
   function, and every recursive program in this language pays for the ones
   that do. That is a panel path and it is filed in `docs/work/SCHEDULED.md`
   rather than decided here.

## Why this was not found before

Nothing in the corpus recursed deeply through a wide grammar. `examples/calculator/`
parses arithmetic with the same shape but is only ever given short expressions;
the compiler's own tests parse the compiler's own source, whose deepest
expression nests nowhere near 400. It took a program written to nest on purpose,
which is what M-corpus-depth exists to add.

# 007 — `heroes check` exits 139 without a word on a legal program

Date: 2026-09-03, M-corpus-depth step 5. **Found by writing a program**, and
then by asking the obvious next question: the interpreter being written has a
recursive-descent parser, and so does the compiler.

**Status: fixed 2026-09-03**, the same evening — `runtime/parts/stack.c`, one
witness given a second shape; see § The repair at the end. The hypothesis below
was right in its conclusion and wrong in its mechanism, and the difference is
the interesting part. The program that found it still nests 60 deep instead of
500, with every ceiling measured in its comment (`examples/interpreter/main.hero`):
the ceiling is unchanged, and what changed is that crossing it has a name.

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

## The repair — 2026-09-03, the same evening

**The hypothesis above had the right conclusion and the wrong mechanism.** It
said a wide frame "moves the stack pointer past the guard region in a single
step". Under `lldb`, on the 440-parenthesis file, the faulting instruction is
not a store in the compiler at all: it is `ldur x11, [x11, #-0x8]` inside
**`___chkstk_darwin`**, libsystem's stack probe, which clang's prologue calls
for any frame larger than a page. The probe walks DOWN through the pages of the
frame it is about to claim, one load per page, **while `sp` still stands where
the caller left it**. Measured registers at the fault:

```
pc  = ___chkstk_darwin + 60        fault address = 0x16f603ff8
sp  = 0x16f605940                  sp - fault    = 6,472 bytes
x9  = 0x2180                       the frame: 8,576 bytes (grammarexpr.primary)
fp chain: primary ← postfix ← unary ← binary ← parse_expr ← group ← primary …
```

So the first witness held (the address is in the guard) and the second did
not: `sp < lo + 4096` fails by 2,368 bytes, the handler concludes the fault is
not an overflow, restores `SIG_DFL`, and the kernel kills the process at 139.
**Why `parse` named it and `check` did not** is nothing but which function's
frame happened to straddle the guard: `postfix`'s frame is under a page, so it
is touched after `sp` moves and `sp` is low; `primary`'s is 8.5 KiB, probed
first. Same file, same recursion, two verbs, two stack depths at entry, two
different frames on the boundary.

**The fix is one line and its comment**: the second witness has a second shape.
A probe lands *below* `sp` by less than one frame, and nothing legitimate is
ever written below `sp` except a frame being set up — so `addr < sp && sp -
addr < HERO_STACK_WINDOW` is the witness for the probing shape, beside the
original `sp_low` for the stored shape. Panel 104's falsifier — a wild store
8 KiB under the stack from a shallow frame — is megabytes below `sp`, and was
**re-run against both runtimes**: exit 139 before and after, on the Mac and in
the Linux container; a store to `NULL` likewise.

Measured after the fix, on this Mac, from the seed-built compiler:

| n | `heroes check` before | after |
|---|---|---|
| 420 | 0 | 0 |
| 440 | 139, silent | **134, `panic: stack exhausted in grammarexpr.postfix`** |
| 500 | 139, silent | 134, named |
| 2000 | 139, silent | 134, named |

**And the shape has a fixture of its own**,
`tests/golden/surface-fixtures/wideframe/main.hero`: a mutual recursion whose
`descend` holds 24 locals of a 32-word record — a frame of **32 KiB at `-O0`**
(`sub sp, sp, #0x8, lsl #12`), and `otool` shows the prologue calling
`___chkstk_darwin`. Against the runtime at `756b3920` it is exit **139 with
nothing on stderr**, which is what makes it a test of this defect rather than
of the guard in general; against the repaired one it is `panic: stack exhausted
in main.bounce` at `-O0` and `in main.descend` at `-O2`, exit 134 both. Under
`--sanitize` the guard yields to ASan by design (panel 104) and ASan reports
`stack-overflow`, so the two rows live in `tests/harness/suite_surface.hero`
(`verb_probes` 42 → 44) and not in `run/`, whose judge refuses ASan's words on
sight. On **Linux x86-64** (the container of
`docs/environment/linux/LINUX-MACHINE.md`, clang 22) the fixture panicked with a
name **before the fix as well**: clang emits no probe there (`sub $0x65f0,%rsp`
and a first store with `sp` already low), so the defect was Darwin's alone and
the repair is neutral on Linux — measured at `-O0`, `-O2` and `--sanitize`.
Windows is untouched by construction: the change is inside the POSIX branch, and
the vectored handler there keys on `EXCEPTION_STACK_OVERFLOW` and never reads
`sp`. The box was off, so that last sentence is a reading of the code and not a
run.

**What is deliberately not done here.** The ceiling itself — 440 parentheses in
the compiler, 190 nesting levels in the interpreter at `-O0` — is set by the
hoisted-frame rule (CLAUDE.md §7), and lowering it is architecture, filed for
the next sitting that touches the emitter. clang's own default is 256 bracket
levels; the ceiling is not the defect, the silence was.

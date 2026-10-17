# Panel 218, the shared brief: a header constant whose undefined value clang folds in an initialiser

Written 2026-10-17 from 03:00 by the coordinator, on the trunk at `cbf724983` (`git log -1`), the
lane `lane-land215` at `51a433cad` holding the same tree plus defect 2108's repair. Convened on
defect 2109 (https://github.com/heroes-lang/heroes/issues/2109), class `systemic`, by the author's
answer of that night accepting the coordinator's recommendation, *a sitting*. **Lane: soundness**
(compiler-engineer and ffi-pragmatist, then a completeness critic), the seats of panel 216, whose
file you read first: `docs/panel/216-a-header-constant-c-leaves-undefined-is-read-by-running-the-programs-own-unit-under-a-trap-and-a-statement-expression-is-asserted-in-its-accessor.md`,
ratified 2026-10-17. Its R1 (Ux-trap-S: the module's own unit preprocessed under the program's
words, compiled with four checks in trap mode at the program's level, linked with the runtime
alone, its accessors run by a probe that resumes after each trap) is being built in the lane; its
R4 named this class outside every route it measured and filed it. A route that needs surface, a
new diagnostic class or a spec token re-convenes the sitting full; say so.

## The question

A group's integer `constant` that names an enumerator, a `static const` object, or a brace-list
record, whose header computes its value in an initialiser that C leaves undefined, builds at exit 0
and prints a stable value, where the same expression spelt as a `#define` is refused today. What
does the compiler do with it?

## What was measured while this brief was written (every number is a command's)

The trunk's compiler built from its seed at `9e1176d87` (no change since to this path), this Mac,
Apple clang 21.0.0, `heroes build` (`-O0`), each binary run three times. Header `f.h`:

```c
enum { E_SHIFT = 1 << 31 };
static const int S_DIV = -2147483647 - 1 / -1 * 0 + (-2147483647 - 1) / -1;
#define D_SHIFT (1 << 31)
```

one program per name, `extern "f.h"` with `constant <NAME>: i32` and `print(<NAME>)`:

| name | build | prints |
|---|---|---|
| `E_SHIFT` | 0 | `-2147483648 -2147483648 -2147483648` |
| `S_DIV` | 0 | `1 1 1` |
| `D_SHIFT` | 1, `ffi_constant_type` | — |

The compiler-engineer and the critic of panel 216 measured the same class outside Ux (Ux-whole
flags neither enumerator nor `static const`, the value being folded in the front end before any
check is emitted), and named a brace-list record constant as its third shape, unrun here.

**Unrun, and questions rather than premises**: what clang says of each initialiser under
`-Wall -Wextra` and under the region's own warnings, on clang 18, 21, 22 and Windows' 23; whether
C11 makes any of them a constraint violation that requires a diagnostic (§ 6.6 p4, § 6.7.2.2 p2);
how many enumerators and `static const` objects the corpus and the real headers bind, and how many
of those a route would refuse; what an enumerator's value is that C *defines* and a route must keep
(`enum { E = -1u }`, `UINT_MAX`, a value from another enumerator, a `sizeof`).

## The routes the coordinator listed, and where the list came from

From defect 2109's text and panel 216's file alone; a route a seat finds that is not here is the
most valuable thing it can bring.

- **A, nothing**: recorded as a known cost, the value stable and the class named in § 13's words.
- **B, ask clang's own diagnostics at the initialiser**: an enumerator's or a constant object's
  initialiser re-asked under the warnings that name an undefined value, in a unit of questions.
- **C, the initialiser's expression re-evaluated under Ux**: for an enumerator or object whose
  definition the header gives, its initialiser's text read from clang's dump and compiled as an
  accessor's `return`, so R1's probe judges it like a macro.

## What each seat is asked

Rule on the question and say which route, built far enough that its numbers are real: which
programs it refuses and which it keeps, over the shapes above and over the corpus and the real
headers; what it costs a cold build; what it does on clang 18 and on the Windows box's clang 23
(the box may be off: say *unrun*); and the message it tells, if any. CLAUDE.md § Precedence ranks
robustness above everything but the hard stops and the rule of running, and the author read ranks
3 and 4 together on 2026-10-16: robustness decides THAT a measured class of wrong programs closes,
Principle 0 HOW.

## The rules every seat keeps

- **Build in your own copy**, a detached worktree under
  `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/218-<seat>` made with
  `git -C /Users/joseph/Temp/heroes/heroes-lang worktree add --detach <path> cbf724983`, or under
  your scratchpad folder `<scratchpad>/218-<seat>/`; then `rm -rf build`, then your compiler from
  the seed, `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`. Never the trunk's
  `./heroes`, never the trunk's tracked files, never the lane `lane-land215`. Remove a worktree you
  added when you finish.
- **Write your report as you go**, to `docs/panel/218-reports/<seat>.md` in the trunk's tree, the
  one file you write there. Every number names the command that produced it; what you could not
  run says *unrun*.
- **No paid run** (no `claude -p`, no API call, no cloud run).
- **No credential is printed**: `gh auth status`, never `gh auth token`.
- **A program that may not end** is built and its binary run under `timeout`, output bounded.
- **Nothing outside the repository's root** but your scratchpad copy; the Linux containers
  (`heroes-linux` is x86-64 under Rosetta, run without `--platform`; `heroes-linux-arm64` native)
  and their own `/tmp` are yours to use.

# 003 — The M6 closure-list audit

**Principle 0's checkpoint** (design.md §1.0), run at M6 close under panel 005's
three riders: **mechanical**, **spec-coverage with named cuts**, and a **runtime
tier** for file I/O, `args()` and `exit(code)`.

Status: `provisional — author ratification pending`, like every panel resolution.
Nothing here changes the language; what it does is say which forms have earned
their place, which are mortgaged, and which are not admissible yet.

## Rider 1 — mechanical

The audit is a program, not an argument: `tests/golden/run/closure-list.hero`
exercises every entry design.md §1.0 lists, and it runs, is ASan- and
leak-counter-clean, and its `test` block passes.

| closure-list entry | status |
|---|---|
| `record` · `variant` + exhaustive `match` | ✅ M5c |
| `[T]` · `{K: V}` · `str` · `int` · `bool` · `()` | ✅ M5b–M5d |
| `T?` with `?` / `.must()` / `.default()` | ✅ M5d, M6 step 1 |
| `=` / `@` bindings · `@` parameters | ✅ M4–M5c |
| `if` / `else if` / `else` | ✅ M5a |
| `while` · `for x in xs` · over a map's keys · `break` / `continue` | ✅ (a map is `for k in sort(keys(m))`, panel 026) |
| `return` · UFCS | ✅ |
| **function values** | ✅ **M6 step 5** — a bare C function pointer |
| **generics on functions** | ✅ **M6 step 6** — monomorphisation, one copy per type tuple |
| **`test` + `assert`** | ✅ **M6 step 7** — `heroes test`, one process per test |
| file I/O · `args()` · `exit(code)` · modules | ❌ **M8a**, as design.md already assigns them |
| library: `print` `len` `push` `slice` `chars` `keys` `sort` `join` `to_int` `to_f64` `to_str` | ✅ M6 step 3 (Tier 1) |
| library: `map` `filter` `fold` `find` `any` `all` `range` | ✅ **M6 steps 4 and 6b** — Heroes source, in `library/source.hero` |
| `panic` | ✅ in the runtime, not user-callable — and deliberately (`resolve/builtins.rs`) |
| `Builder` | ❌ **not built, and not needed**: `join` allocates once by summing the lengths first, which is what design.md:1318 asked `Builder` for |

**Nine of the fourteen language rows and every library row are done.** The four
that are not are the four design.md already schedules for M8a, and they are one
milestone rather than four: modules is the hard one, and file I/O, `args()` and
`exit` are `<stdio.h>` plus three declarations once `extern` verifies against a
real header (M7).

**The audit found two live defects**, which is the argument for running it as a
program:

- a built-in used as a value — `map(xs, to_str)` — checked clean and produced
  invalid C at exit 2. `types/exprs.rs` answered `error_ty()` with **no
  diagnostic**, and its comment said the case was unreachable. Fixed and named
  (`check/fixedbugs-builtin-as-value.hero`).
- earlier in the same milestone, `[()]`, `[ptr]` and `{str: ptr}` built binaries
  that aborted saying "this is a compiler bug"
  (`unsupported/fixedbugs-container-element-reached-clang.hero`).

Both were reachable for three milestones and neither had a test, because nobody
had written the program that meets them. That is what "mechanical" buys.

## Rider 2 — spec coverage, with named cuts

The spec is **2363 tokens** of a 4096 ceiling, 1733 of headroom. Against the
closure list, four things are on it and **not in the spec**, and they are named
here rather than discovered later:

| mortgaged | why it is not in the spec yet |
|---|---|
| **modules** | M8a. It is the one closure-list row that changes the surface — a module is a declared name rather than a file stem — and pricing it before its shape exists would be pricing a guess. |
| **file I/O** | M7's `extern` makes it three declarations. Whether the spec needs a sentence at all is the question M7 answers, not this audit. |
| **`args()`** | Same, and smaller. |
| **`exit(code)`** | Same, and it interacts with §10's exit-code contract, which is the compiler's rather than the language's. |

**The runtime tier for the three (rider 3): plain `extern`s, not Tier 1.** The
reasoning is the one §1.11 already gives and this milestone confirmed twice.
`join` is Tier 1 because a `Builder` cannot be written in Heroes without exposing
the allocator; `chars` is Tier 1 because UTF-8 decoding needs byte access the
language does not offer. File I/O, `args()` and `exit` need **none** of that:
they are C functions with C signatures, and §4.19's whole claim is that such a
thing needs no tier at all — it needs an `#include`. Making them Tier 1 would
spend runtime surface on the exact case the FFI exists to serve.

The consequence is a **spec cut, named**: if they arrive as ordinary `extern`s,
the spec gains **nothing** for them, and design.md's closure list should stop
listing them beside language features. They are the first three programs of §4.19's
ladder, not rows of the language.

## Rider 3 — and the one this audit adds: `outline` and `explain`

The ROADMAP lists both under M6. **Neither is admissible**, and the rule that
says so is this project's own.

CLAUDE.md §10's stopping rule (panel 016): *"a capability enters the surface only
if the fixpoint invocation, the golden harness or the Part 11 harness must type
it, or it has a measured Part 11 effect."* Applied:

- the **fixpoint invocation** is `build … -o A`, `--emit-c -o B.c`, `--emit-c -o
  C.c`, `diff`. Neither verb appears.
- the **golden harness** runs `lex`, `parse`, `check`, `build`, `run`, `fmt`,
  `test`. Neither verb appears.
- the **Part 11 harness** has not run since these were proposed, so there is **no
  measured effect** to cite.

So the warrant would have to be the fourth branch, and the fourth branch is a
measurement nobody has taken. **They wait for the harness run, and the ROADMAP
line moves rather than the rule.** `heroes test` passed the same test and
entered: design.md's verification list has named it since M0, and the acceptance
criterion for this milestone is written in terms of it.

This is worth stating plainly because it is the first time the stopping rule has
refused something the ROADMAP itself scheduled. Panel 016 wrote it for exactly
that case — a rule that only ever agrees with the plan is not a rule.

## What the milestone actually settled, in one line each

- **`{K: V}` stays** (panel 026, the author's call, funded at +43 and carrying a
  −17 removal). The alternative was −57 and the deletion of a type.
- **The descriptor ABI has five members and gains no sixth** (panel 027), on
  compiled evidence: C11 zero-fills a short initialiser list, so the veto is
  about what the language cannot see rather than about elegance.
- **Generics are monomorphisation, before the ownership pass** (panel 029),
  because that pass had already decided wrongly rather than declined to decide.
- **Polymorphic recursion is refused**, and MLton is why that is free.
- **The library is Heroes source, compiled with the program** (panel 028), and
  `types/builtins.rs` lost 92 lines when the six higher-order built-ins became
  declarations — §4.12's claim, measured.

## What the audit leaves open, deliberately

- **`sort` on a non-scalar element type** is refused by the gate, not the
  checker, and the question of what order a `[Point]` has belongs to whoever
  lands `sort_by`. Panel 027 R1 named it; nothing since has needed it.
- **A user generic instantiated in two modules** has no named home (panel 029
  R5b). M6 has one translation unit; M8a decides, with both measurements in the
  record.
- **The tier phrase** in the spec is pre-measured at **−6** and available. Panel
  028 R6 kept it on the ergonomist's evidence; if a harness run shows it buys
  nothing, the removal is one edit.
- **`heroes mutate` is at 97% / 81%** over 839 mutants. The gap between the two
  is the thesis's own measurement and it has not been analysed since M5b.

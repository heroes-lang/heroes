---
paths:
  - "selfhost/emit/**"
  - "selfhost/ir/**"
  - "runtime/**"
  - "seed/**"
  - "tests/emission/**"
---

# Generated C

Home of CLAUDE.md § 7 since 2026-09-07. What each rule cost to learn is in
`docs/contract/case-law.md`; the entries are cited as `CL-NNN`.

## The shape of the file

C11. `int64_t`, `double`, `bool`. `#include "heroes_runtime.h"`, so clang
type-checks every runtime call, and one `_Static_assert` on
`HERO_RUNTIME_ABI`.

**What that stamp catches is a header from another compiler, and not a decoy**
(CL-007). A decoy `runtime/` that copies the number passes; what protects a
build against one is the **cache key**, which covers the whole runtime's
contents. The stamp's job is version skew, and it is **not** extended to cover
behaviour.

`#line` when an instruction's line differs from the **current effective line**
(`#line N` anchors the *next* line), restored to the generated file around
synthetic code, and the restore carries the printer's own output line count.
Emitter debugging is the test helper's job: `--no-line` is refused by the
stopping rule in `.claude/rules/cli-surface.md` (panels 016, 020).

One `goto` and label per basic block, explicit entry `goto bb0`, a label **only
where an edge targets it**, all locals hoisted to the prologue, and a
unit-typed temporary never declared at all (`void t0;` is a hard error). An `@`
parameter is a pointer parameter, so design.md §4.8's copy-out is `*p_l = l;`.
`hero_unreachable()` at every type-system-proven-unreachable point. The emitted
C never mentions the output path.

## Arithmetic and memory

Arithmetic aborts via `__builtin_*_overflow`, never C undefined behaviour, and
`%` is guarded like `/`, because `INT64_MIN % -1` does not trap on arm64.
`INT64_C(n)` for every `int` literal.

**A refcounted slot is the one exception to the no-initialisation rule**:
zero-initialised so cleanup is unconditional, with `ptr == NULL` as the
non-value every runtime entry point rejects (panel 021).

A generated `eq` or `hash` walks **fields, never bytes**, because padding makes
two equal records hash differently and silently. `hash` is never null. Copy on
write is **one unshare per step** of a mutated place: one at the primitive lets
a nested store alias, measured, with every instrument in this project reporting
success (panel 022).

## Names

Every name goes through the mangler, `h_<module>_<name>[_<typehash>]`, and that
includes fields, variant cases and labels. The module component is sanitised to
`[A-Za-z0-9]` so the first `_` ends it. `extern` FFI names pass through
unmangled by design.

**One exception, and it is the mirror of the rule** (panel 038, ratified
2026-08-12): an `extern constant`'s accessor **is** mangled, because unmangled
`int64_t SQLITE_OK(void) { return SQLITE_OK; }` has the macro eat its own
definition. A linker name must survive the mangler; a preprocessor name must
never appear outside the accessor's body.

## Flags, and the verdict on a clang failure

**The compile-flag list is `selfhost/cli/flags.hero`, in `flags()`, and this
file states no count and no copy of it** (CL-030: that one sentence has been
wrong three times, twice by pointing at a tree nobody builds and once by
carrying a count in prose). `-std=gnu11` is named and not inherited (CL-013).

A clang failure is exit 2 and says the **compiler** is wrong, with one named
class of exception that belongs to the author's own `extern`:
`.claude/rules/c-boundary.md` has it.

## The instrument

**The double-emit determinism test stays green at all times.** It is the
`determinism` suite of the net, and a byte of difference between two emissions
of the same input is a failure whatever else passes.

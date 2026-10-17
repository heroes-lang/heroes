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
`docs/records/contract/case-law.md`; the entries are cited as `CL-NNN`.

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
**A re-anchoring line joins its anchor, and a name is written where it changes**
(panel 219's R1, ratified 2026-10-17; design.md §3.1): a restore that would
re-anchor the very line the previous C line sits on is not written, its C line
joined onto that one (`emit/line_join.hero`), never onto a directive, a
preprocessor line, a blank line, a line holding a newline or a `//`; and a
`#line` names its file only where the file clang presumes changes, `#line N`
alone elsewhere (`emit/line_name.hero`). The line table is the same at every
address. What an author sees move: clang's C column and its quoted excerpt on
the exit-2 path, the whole Heroes line's C, and C-line coverage by hand.

One `goto` and label per basic block, explicit entry `goto bb0`, a label **only
where an edge targets it**, all locals hoisted to the prologue, and a
unit-typed temporary never declared at all (`void t0;` is a hard error). An `@`
parameter is a pointer parameter and the body reaches the caller's place through
it, `(*p_l)`, so §4.8's copy-out writes no C (design.md §3.1, panel 196's R1).
Every temporary and every slot the lowering invented is declared `HERO_NODEBUG`
(`nodebug`, defined in the unit's own preamble, so the runtime's ABI does not
move), and so is every C local the emitter declares inside a body: a stopped
frame lists the author's bindings alone, and a hidden slot has no debug entry
to show a wrong value with (panel 213's R1).
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
two equal records hash differently and silently.

**And C never reads a group record's padding as it was left** (panel 215's
R1, ratified 2026-10-16): before each `extern` call, the bytes no C member
holds in every group record the call lends through `@`, a whole cell, a
field, an element, an `@` parameter handed on and the elements of an array's
buffer, are written zero in place, never a byte a member holds and never
inside a union (`emit/padding.hero`). Every span is clang's `offsetof` and
`sizeof` of a member, in a table each unit writes once, a member's own name
guarded from the header's macros; the gaps are found once, before `main`, and a
lend only writes them. Measured before it on Linux x86-64 at
`-O0`: two records `==` called equal reached C with seven bytes of a dirtied
stack each (`run/ffi-the-padding-c-reads-is-zero`). What it does not
promise: the bits a bit-field's byte holds beside it (defect 2099), and five
shapes read and not yet run (issue 2100). `hash` is never null.

**And the bits beside a named bit-field, since panel 223** (2026-10-17, provisional, defect 2099):
at both of R1's sites, before each lend and after each store of a whole group record (defect
2112), in each byte a named bit-field shares with bits no named bit-field holds, those bits are
cleared by `__atomic_fetch_and` through `volatile unsigned char *` (`hero_pad_clear`), the masks
found once before `main` by `m.F--` on a zeroed object for each named bit-field, its name guarded
(`hero_pad_masks`), so no byte order is assumed; a byte a `const` bit-field holds is left as it
was. What stays, said: an atomic `and` beside C's own plain write to the same byte is a data race
by C11's letter, measured to lose nothing on three legs where a plain read-modify-write lost
thousands; R1's zeroing write already lands inside a bit-field's access unit on the MSVC target
and, for a byte only an unnamed bit-field holds, on all four legs, changing no member; and a
library version that adds a bit-field where the binding's header had padding has those bits
cleared. Copy on
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

**The debug word is not in that list: it is the level's**, `flags.debug_words`,
`-g` at `-O0` and `-gline-tables-only` at `-O2`, and `flags.level_words` hands
the list, the level, its debug word and the sanitizers alike to a unit's
compile and its probes (`compiling.unit_words`), the runtime's object, the
identity probe and the link, so no word reaches one of them and not the others;
`toolchain.build_words` names the debug word in the runtime's key (panel 197,
ratified 2026-10-07).

A clang failure is exit 2 and says the **compiler** is wrong, with one named
class of exception that belongs to the author's own `extern`:
`.claude/rules/c-boundary.md` has it.

## The instrument

**The double-emit determinism test stays green at all times.** It is the
`determinism` suite of the net, and a byte of difference between two emissions
of the same input is a failure whatever else passes.

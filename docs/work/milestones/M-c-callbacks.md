# M-c-callbacks — a Heroes function reaches a C callback parameter *(closed 2026-09-05)*


**CLOSED 2026-09-05**, the day it opened, tag `m-c-callbacks`, six steps. The
milestone's own record is [033](journal/033-c-callbacks.md); what stays here is
the reasoning a later milestone has to honour, and it is four things.

**The split is settled and is not re-argued.** The permission is bought entirely
by §1.11 and CLAUDE.md §12's FFI-completeness instruction and needs no thread at
all — three seats of `docs/panel/111` asked for the split independently, and
bundled into M-isolated-threads it would have let the weaker half ride the
stronger's Principle 0 ticket.

**Where a callback may stand is a question about position, and never about
vocabulary.** C spells a function pointer everywhere it spells a type, so *can a
header declare this* cannot be the rule. A function value crosses as a
**parameter** and in no other position; one argument refuses all three of the
remaining shapes — an `extern`'s result, an `@` out-parameter and an `extern
constant` — and it is the permission's own mirror: an address passed **out** is
one this compiler emitted, so C calls a body this compiler type-checked; an
address handed **back** is a body nothing here has seen. `spec:224-225` states
it.

**The `const` hole is declared rather than open**, and its shape is measured
rather than assumed. `docs/panel/112` refused a `const` spelling on four
independent grounds and wrote design.md **Part 8 wart 19** at zero spec tokens,
in the order the counting found: over the real `sqlite3.h`'s 106 callback
signatures, a pointer to a struct the header declares is **66.7%** of what cannot
be spelled and a const pointee is **2.6%**. Anyone reopening it starts from R6:
`quals` on the existing `function_ty` case is the **only** admissible route — 2
edits in 1 file, no new `Ty` variant, so panel 083's veto does not fire — and a
new `Ty` case is 173 `non_exhaustive` errors across 49 files.

**R8's condition was met by the guard and NOT by atomics, so M-isolated-threads
still owes them.** Panel 111 R8 said the refcount work lands with the permission
or before it, on the ground that the FFI refusal was by accident the only thing
between a Heroes program and `hero_str_incref`'s race. What landed instead is
R9's guard, emitted into the callback: a foreign thread stops by name at the
entry of every function whose address the program takes, so the corruption is
**unreachable rather than repaired**. `_Atomic` still appears nowhere in
`runtime/`, and `cow.c`'s `if (a->refcount == 1)` is a test-and-mutate that an
atomic would not fix in any case.

*******************************************************************************
**OPEN: 0**

*******************************************************************************

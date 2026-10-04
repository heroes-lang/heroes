---
kind: decision
area: none
milestone: none
filed: 2026-09-28
commit: 64003eddbe9d3c25ffc48ccc77993a1b6ae32da2
github: none
---

# A value is never zeroed, a slot is, and every definition is written whole

2026-09-28 | the emitted C never zeroes a value or an `@` parameter's slot and
zeroes every other refcounted slot; the verifier checks a value's definition
precedes its reads within a block as well as across blocks; every aggregate
the emitter writes by tag and payload is written whole first; the unit and
runtime cache keys take the C compiler's identity and its flags (defect 122);
the exit sweep counts as a read in `unread.slots_read`; design.md Part 5's rule
5 and the emitter's header made true; the consuming store and the initialising
stores deferred until the compiler itself crosses a leak gate | 81,680 of the
seed's 102,990 zeroings were values no path reads or releases before their one
definition, by four instruments that agree, and zeroing them took locals from
`-Werror=uninitialized`, the check the prologue relies on; every slot is owed,
since every store loads the old value first; the faster routes remove checks
the compiler cannot do without yet, the critic having shown a leak-only failure
reaches a byte-identical fixpoint with 1.3 million live blocks unseen |
design.md §1.12, §1.7, Part 5 | **panel 182**, the soundness lane, two seats and
a critic, provisional

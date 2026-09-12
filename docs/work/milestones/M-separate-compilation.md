# M-separate-compilation — one `.c` per module *(closed 2026-08-26)*


**The record is [journal 025](journal/025-separate-compilation.md).** What stays
here is only what a later milestone has to honour.

**The four acceptance rows, and what closed each** (panel 030 R2 — they are what
lifted the ffi-pragmatist's veto):

| row | what it demands | closed by |
|---|---|---|
| 1 | the header travels with the `extern` into every calling TU, or an `extern` is never callable across a module boundary | step 5, via panel 033 R5's third answer: a qualified mention is a type error and the route is a Heroes function in the declaring module |
| 2 | headers and link flags enter the cache key, `runtime_text()` kept | step 7, by clang's own dependency listing rather than by naming headers |
| 3 | every dependency's emitted interface enters the key | by construction — a TU's text carries the prototypes and tag enums of everything it reaches, so a dependency's signature change moves the caller's key |
| 4 | a two-module FFI case with a wrong `extern` signature | step 10, in panel 093 R5's re-reading: the row as tabled ("exit 1 in both TUs") is unsatisfiable, because the caller is correct |

**Three things a later sitting must not re-derive.**

**The `extern` block is not pruned, and the architecture is why.** Panel 091
vetoed every pruning option with compiled evidence: pruning the `#include`
deletes a group record's C type, an uncalled group holds up a used one, the prune
takes the `-l` with the declaration and a constructor library stops running at
exit 0, and the probe is a **memory-safety instrument** — the only thing that
sees a `size_t` out-parameter declared `i32`, which when executed wrote four bytes
into an adjacent object with ASan and UBSan silent. One `.c` per module plus row 1
puts every group in exactly one TU, which is what makes the duplication go away
for free.

**A cache key can be blind to its own input by construction**, and this is the
shape to check first in anything that caches: an `extern constant` emits as
`return CONF_LIMIT;` whatever the header holds, so the emitted C is
byte-identical across a header edit. What stands between that and a wrong answer
at exit 0 is asking clang which files it actually opened. `tests/harness/suite_cache.hero`
is the instrument, and `suite_units.hero` holds what one translation unit must
contain.

#### What it does not deliver, and what the next sitting argues from

**The per-module build still does not beat the fused one.** Re-measured at the
close, one machine, one tree: per-module **cold 57.9 s**, **warm 45.2 s**; fused
**emit 40.4 s + one clang line 3.7 s = 44.1 s**. The gap is about a second, where
it was 8.7 before step 9's frontend repair and where this file once carried 127.

**And the reason is architectural rather than a defect**: panel 093 R4 puts the
emitted text **in** the cache key, so a warm build must emit all 157 translation
units to learn that it may reuse their objects. **A cache cannot skip the work
that computes its own key.** The frontend is now ~18% of a build where the open
item measured 83%, so an incremental frontend is the smaller half; the emission
is the larger one. Both numbers are in `docs/work/SCHEDULED.md (retired 2026-09-12)` with their dates.

*******************************************************************************
**OPEN: 0**

*******************************************************************************

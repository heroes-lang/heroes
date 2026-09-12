# M-declared-freer — the string C hands you, freed by name *(closed 2026-09-07)*


**What happened is [journal 036](journal/036-declared-freer.md).** Panels 109
and 116, both ratified; five steps; `owned <C function>` in both positions, with
the release built in the lowering. What stays here is only what a later
milestone is bound by.

**`ptr owned` as a counted value is REFUSED under a standing veto**, and the
veto did not lapse with this milestone. Its return conditions are measured and
written in `docs/panel/109`; the program they point at is
`examples/ledger/main.hero`'s five leaking error paths. The area is deliberately
still free: this id names the deliverable and not the topic (CLAUDE.md §14), so
a milestone that delivers the `ptr` half takes a new id.

**The third case — C keeping a lent buffer — is queued and not refused.** It
needs a lifetime across two calls that a language without references cannot
state, and the C-side spelling works today (`constant SQLITE_TRANSIENT: ptr`).
A sitting of its own, when a program needs it.

**Two facts a later sitting on the FFI must not re-derive.** A freer is verified
by the probe, and **a probe exists per `extern`** — so a probe can never check a
freer no `extern` declares, nor an arity the release contradicts, and those two
refusals live in the checker (`selfhost/check/freer.hero`) rather than in the
emitter. And **the probe's spelling is the question it asks**: a marked cell is
`char **` in the probe and `(char **)&cell` at the call site, and the two must
agree or clang is being asked two different things.

**The freer's own module is the boundary.** The release is a C call emitted in
the marked declaration's unit, and that unit includes only its own groups'
headers, so a freer declared in another module cannot be called. Anything that
later widens where an `extern` is reachable from touches this.

*******************************************************************************
**OPEN: 0**

*******************************************************************************

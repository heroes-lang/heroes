# Panel 150 — brief for the historian

Read `docs/panel/150-briefs/00-shared.md` first, including its three required
sittings. Advisory, no veto. **Verify every date and claim by web search and
fetch**; unsourced precedent is inadmissible and a claim with no link you
actually fetched is struck.

**Your last finding reframed panel 149** and it is the starting point here: the
split is not how deep a rule looks, it is WHERE THE WORD IS WRITTEN. Every system
marking the FUNCTION stops at its signature; every system reaching a field marks
the FIELD or the TYPE. **Heroes has ratified that it marks the CALL** (panel
147), so the exit the industry took is closed to it. Do not re-propose it.

## Q1 — an untyped pointer with ownership

The question is narrower than last time and it is this: **who has made ownership
annotations mandatory on an UNTYPED pointer, where the type system cannot tell
one pointer from another?**

1. **Clang's `ownership_returns` / `ownership_takes` / `ownership_holds`.** These
   apply to `void *`-shaped APIs and take a MODULE string precisely so that two
   unrelated allocators do not collide. Fetch the documentation and quote how
   that module argument works. **That is the mechanism this sitting may need**,
   and if it exists it is the answer to *every `ptr` is one type*.
2. **SAL's `__drv_allocatesMem(Mem)`** takes a kind argument for what looks like
   the same reason. Confirm or refute.
3. **Anyone who made such an annotation MANDATORY rather than optional**, and
   what the false-positive rate did to them. You found Swift retracting exactly
   such a rule to a flag inside one release cycle, for noise; look for a second
   instance, and for anyone who kept it.

## Q2 — a container handed over whose elements are not

4. **GObject-Introspection's `(transfer container)` is this question in another
   vocabulary** and you found it last time. Now go further: does anything let a
   caller release the ELEMENTS of a transferred container, or is the container's
   own destructor always the only sanctioned route?
5. **Anyone who tried per-element ownership inside an aggregate and abandoned
   it.** That is the most valuable thing you can bring, as it was last time.

## Predict something falsifiable

One prediction with what would settle it.

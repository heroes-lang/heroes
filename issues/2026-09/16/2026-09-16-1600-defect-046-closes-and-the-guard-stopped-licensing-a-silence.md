- [x] **046 — both static walks abandon past a nesting bound, so `check` accepts what its own rule refuses** | a record chain 16 deep ending in an `f64` map key, or in a `partial` group record compared with `==`, was `check` 0 where 15 deep was `check` 1 — and no generic was involved | **CLOSED 2026-09-16** | `selfhost/check/map_keys.hero` · `selfhost/check/partial.hero` · `tests/golden/check/fixedbugs-a-nesting-bound-was-a-silence.hero`

    **Origin:** panel 155's ffi-pragmatist, attacking the shapes beside the one
    that sitting was convened about, and re-measured at the synthesis with one
    construction for both rules.

    **The repair is the shape the compiler already had in a third place.** Both
    walks terminated on `if depth > 16`, which terminates by GIVING UP. Both now
    carry a set of the type ids already walked — `@seen: {i64: bool}` — which is
    what `check/reaches.hero` has used since panel 149 repaired **defect 035**,
    the identical line in the handle walk. It is total without abandoning
    anything: the type arena is finite and every id is entered once, and a type
    walked before returned absence then and returns it now, so the memo is
    correct as well as terminating. **The compiler now answers this question one
    way in three places instead of two ways in three places.**

    **Measured after the repair**, `heroes check` on a chain of records ending in
    the offending type, both rules at each depth:

    | depth | 1 | 15 | 16 | 17 | 24 | 40 |
    |---|---|---|---|---|---|---|
    | `float_map_key` | 1 | 1 | **1** | **1** | **1** | **1** |
    | `ffi_partial_operation` | 1 | 1 | **1** | **1** | **1** | **1** |

    Before it, 16 and past were `check` 0 with no diagnostic, then `build` 0, then
    a named abort at 134.

    **The argument for the bound was TRUE and not enough, which is the part worth
    keeping.** `map_keys.hero` held that absence at the bound was safe because the
    runtime guard sits behind the rule. It does, and it fires: a `nan` key at
    depth 16 still aborts 134, measured, because the guard is a structural
    self-comparison that nesting cannot evade. What the argument missed is that
    **a guard licensing a compile-time silence is a guard the compiler leans on to
    be wrong quietly** — and `spec § 13` promises its refusal *"for it and for any
    value holding it"*, which is a promise about a VALUE and not to a depth. The
    old comment is corrected under its own date rather than deleted, in both
    files.

    **It also settles panel 155's R4.** The spec-warden offered *only the float
    rule has a guard leaning on it* as one of three grounds for splitting that
    sitting's question, and the ground was false: the two walks stated the same
    give-up, licensed by the same kind of guard, in the same words. The
    completeness critic's reading that the float twin abandoned one level
    shallower was an artefact of two differently-built nests; measured with one
    construction they were identical.

    **The golden** is `tests/golden/check/fixedbugs-a-nesting-bound-was-a-silence.hero`:
    both chains at depth 17 in one file, because one walk's repair must not be
    able to land without the other's, **plus two controls at depth 1** — the only
    depths that ever worked. If either control stops firing, the repair has traded
    one hole for another and the case says so.

    **Verified**: `check` 122 to **123**, `annotations` 159 to **160**, `canonical`
    2, `fixes` 10, the compiler's own tests, and the full net. **The seed was
    regenerated in the same commit** and the fixpoint verified byte-identical,
    820,349 to 820,861 lines.

    **No panel of its own, and the reason is named**: panel 155 R4 already adopted
    the direction — *a total walk, or a bound with a diagnostic behind it rather
    than a silence* — and panel 149 ratified this exact shape for the twin. The
    diagnostic class is unchanged; only its reach is repaired to match what it had
    always claimed.

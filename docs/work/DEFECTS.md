# DEFECTS — the compiler defects that are still open

Read by whoever attacks a defect. Every item is a **measured** failure of the
compiler on a program — a crash, a wrong answer at exit 0, a silence where a
message is owed — carrying its reproducer, its cause where known, and what is
owed. The file exists by author instruction 2026-09-03: one file inside
`docs/work/`, so that everything is tidy.

**Only open defects live here.** The moment one is repaired its entry is ticked,
gains a *The repair* section with the measurements that prove it, and moves to
`docs/work/DONE.md`, the record (CLAUDE.md §3). A repair is owed at the class
and not at the witness, with a `tests/golden/fixedbugs/` case per shape.

**The shape.** One line per item, then the body indented four spaces — a
reproducer, a cause and a measurement are the entry, not decoration. Nothing
lives outside the two banners, and this file is why `records/lists` exists: it
had grown 2753 bytes of prose about five already-repaired defects, every one of
them already in the record.

**Numbers are never reused, and 014 was issued twice** — `docs/work/DONE.md`
carries a Windows-diagnostic defect and an FFI-boundary defect both numbered
014, filed a day apart. A record is not rewritten (CLAUDE.md §14), so the
collision stands there; the next number to issue is **025** (022 and 023 were issued on 2026-09-08, 022 was SPLIT on 2026-09-09 by panel 122 into 022 and 024, and all three closed the same day, 024 last, at M-held-bytes) — 017 to 021 were
all issued on 2026-09-08 and all closed on 2026-09-08, and all five are in the
record.

Format: `- [ ] **NNN — <title>** | <what it does, in one line> | <where to look>`

*******************************************************************************
**OPEN: 1**

- [ ] **026 — a function type names no parameters, so a call through one takes its arguments positionally** | two same-typed arguments can be swapped in silence through a function value and through a C callback. **NARROWED 2026-09-11 by panel 128**: the invented label is refused now, and the document says the call is positional; what remains open is the inversion itself, which the instrument prices at 50% unchanged | `spec § 9 Functions and calls` · `spec § 13 FFI` · `selfhost/check/walk.hero` · `docs/panel/127-the-rule-that-did-not-reach-its-own-library.md`

    **Origin:** panel 127's ffi seat, 2026-09-11, which named it as two holes;
    it is filed as one because the cause is one, and both witnesses were re-run
    in the landing session rather than taken from the seat's report.

    **Witness one, a call through a function value.** `report(code: str, msg:
    str)` is refused positionally at a direct call, as § 9 requires. Through a
    value of its own type it is not:

        f: (function(str, str) -> str) @ report
        print(f("msg_first", "code_second"))     # prints msg_first/code_second
        print(f(nonsense: "a", rubbish: "b"))    # prints a/b

    Both compile at exit 0. The second is the sharper half: a label that names
    nothing is accepted and ignored, so the label is not even a claim.

    **Witness two, a C callback's own parameter order.** With a header of one
    line, `int64_t hero_cb_apply(int64_t (*f)(int64_t, int64_t), int64_t, int64_t)`,
    and `function subtract(minuend: i64, subtrahend: i64)` passed as `f`, the
    program prints **7**. Rename the two parameters to `(subtrahend, minuend)`
    and leave the body alone and it prints **-7**, with no diagnostic: the names
    are a promise and C hands the arguments by position.

    **The cause, read at the line.** `selfhost/check/walk.hero:1541` gets the
    label to expect from `fd.value.params[position].name`, a span into a
    **declaration**. A function TYPE, `(function(A, B) -> C)`, has no parameter
    names to read, by the language's own grammar (`spec § 2 Types`), so there is
    nothing for the rule to compare against. Panel 127 closed the same shape for
    the two built-ins by giving them names the compiler carries as literals; the
    same trick has nowhere to live here, because the type is written at every
    site and names nothing at any of them.

    **What it is not.** Not the built-in hole panel 127 repaired, and the repair
    cannot be escaped through this door: a built-in may not be taken as a value
    at all, `error[builtin_as_value]`, measured. Not an ABI question either: the
    widths and signs of a callback's parameters ARE checked (`spec § 13`), only
    their order is not.

    **WHAT PANEL 128 DID AND DID NOT CLOSE, 2026-09-11.** Closed: a label at
    such a call is `error[label_on_function_value]` with a certain fix, so a
    label that named nothing is no longer accepted and ignored; and `spec § 3`'s
    function-type row now says the call is positional, a sentence that was
    vetoed as false at the ballot and became true when the refusal landed.
    **Still open, and this entry is now only about this**: the two values may be
    handed over the wrong way round. `heroes mutate --operator swap-args` reads
    **50%** on the shape, before and after, measured. At the C boundary the ffi
    seat measured the same swap reaching memory three times, once at **exit 0**
    with a heap-use-after-free under the sanitizer.

    **And the sitting found where it actually ships**, which this entry did not
    know when it was filed: inside `fold`. A program hands `fold` a function and
    the library calls it positionally with no name in the chain; `fold` at
    `A := B` with the callback's roles inverted prints `cba` instead of `abc`,
    exit 0. `fold`'s type is `(function(B, A) -> B)`, two distinct letters, so
    **no route the sitting considered reaches it** — not refusing same-typed
    types, not mandatory names on a same-typed pair. Whoever repairs this owes
    that shape an answer of its own.

    **The repair is scheduled with the measurement that lifts its veto**, at
    M-check-completeness in `docs/work/SCHEDULED.md`.

    **What is owed.** A ruling, because three routes exist and all three change
    the language: a function type may name its parameters, which makes the names
    part of the type and every signature longer; a function type whose
    parameters share a type is refused, which is the strictest and costs the
    corpus its comparators; or the document says what the compiler does, which
    is what panel 127 refused for the built-ins on CLAUDE.md §12. The
    measurement that should open that sitting exists: `heroes mutate --operator
    swap-args --survivors` reads **75** survivors now, and this class is what
    remains in them.



*******************************************************************************

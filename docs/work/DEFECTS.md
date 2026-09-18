# DEFECTS — the compiler defects that are still open

Every item is a **measured** failure of the compiler on a program — a crash, a
wrong answer at exit 0, a silence where a message is owed — carrying its
reproducer, its cause where known, and what is owed. **Only open defects live
here**: a repaired one is ticked, gains a *The repair* section with the
measurements that prove it, and moves to `docs/records/done/`. A repair is owed
at the class and not at the witness, with a `tests/golden/fixedbugs/` case per
shape.

**The shape** is `.claude/rules/records.md` § The lists, and § A live list is a
preamble, a count and its items is why this preamble is fifteen lines. **The
next number is READ, never remembered** — `records/numbering` takes one above
the highest issued across this file and `docs/records/done/`. Who issued which
number since 2026-09-08, and why 014 exists twice, is
`docs/records/log/2026-09-16-2200-the-defect-register-leaves-the-list.md`.

Format: `- [ ] **NNN — <title>** | <what it does, in one line> | <where to look>`

*******************************************************************************
**OPEN: 2**

- [ ] **061 — a fixed-array field cannot be passed to C at all** | `strlen(u.sysname)` against `function strlen(s: cstr) -> u64` is `type_mismatch`, because Heroes gives a fixed array no array-to-pointer decay, so a bound `char[N]` field only LOOKS bound | `selfhost/check/` · `spec § 13`

    **Origin:** panel 162's ffi-pragmatist, 2026-09-18, measured while answering a question about reading.

    **Measured, verbatim**, with a full 256-element literal in place so nothing
    else could error:

    ```
    function strlen(s: cstr) -> u64
    strlen(u.sysname)   ->   error[type_mismatch]: expected cstr, found i8[256]
    ```

    **No Heroes-side shim can route around it.** In C, `char[N]` decays to
    `char *` at every call; in Heroes the field has no spelling that reaches a
    `cstr` or a `ptr` parameter. So a program cannot hand the field to the very
    C function that would read it, and the seat that measured it calls this the
    completeness failure `.claude/rules/c-boundary.md` names in its own words:
    *a library Heroes cannot bind is a library the author must leave C code
    around for*.

    **It is filed apart from panel 162's resolution rather than absorbed.** That
    sitting widens `slice`, `validated` and `repeat` so a field can be READ; this
    is the other direction, handing a field TO C, and no route in that sitting
    was priced against it.

    **The shapes beside it are unrun** (CL-061): a fixed array passed to a `ptr`
    parameter, to an `@` out-parameter, and as a struct member of a value
    crossing by value — the last of which works today, since the whole record
    crosses.

- [ ] **062 — `ffi_incomplete_record` names a field that is present** | a single `cstr` field in an `extern record` reports *does not name `nodename`* with `nodename` declared on the line below | `selfhost/emit/ffi_record.hero:38-60`

    **Origin:** panel 162's compiler-engineer, 2026-09-18, found beside the
    question; reproduced by the completeness critic in the same sitting.

    **Cause, named by the seat that found it**: `incomplete_record` forwards
    clang's *missing field* wording verbatim, and clang is complaining about a
    positional probe rather than about the field the author left out. So the
    diagnostic is right that something is wrong and wrong about what.

    **What it costs** is the thing design.md §4.17 exists to prevent: an error
    that names a token the author has already written sends them to correct
    something that is correct.

*******************************************************************************

# M-reflection-verdict — the ruling on reflection


**Scheduled by author instruction 2026-09-03**, the author's own example of a
feature never considered (*"reflection, say, just as an example"*), and a
**decision**, not a feature.

**The record has no row for it, and that is the finding.** Measured 2026-09-03
with `grep -rIn -i reflect` over design.md, DESIGN-LOG, the panels and the
reasoning notes: three mentions and no ruling. Panel 018 (`docs/panel/018:99`)
calls an attribute/reflection system *"new semantics with no compiler need
(Principle 0) adjacent to forsworn Part 6 territory"*; panel 039's C4 row
(`docs/panel/039-comptime-and-part-6.md` § Appended 2026-09-04) files
*reflection over types* under Part 6's **Ruby** row rather than its Macros row;
and the packages session noted that FastAPI's decorators and type-driven
validation *"need reflection Heroes has not got"* (`docs/work/DONE.md`, its
2026-09-04 entry — the reasoning note that held both was retired that day and
the C4 table moved into the sitting that had cited it). None is a Part 6 row with a
falsifier, so today the refusal is uncitable (CLAUDE.md §1) and a sitting could be
convened on it as an open question — the shape §1 warns against.

**Two questions, and they are not one.** *Run-time* reflection — a value that
carries its type and can be asked for its fields — is the Ruby row's territory
(`design.md:2395`: *"you cannot tell from the source what is callable … it
requires runtime dispatch, i.e. an interpreter"*), and the expected verdict is a
Part 6 row naming the program that would make it wrong. *Compile-time
derivation* — a `to_str`, a JSON rendering or a `hash` generated over a record's
fields, the way the emitter **already** generates `eq` and `hash` by walking
fields (CLAUDE.md §7) — is refused by nothing in the record, and it is where the
pressure is measured: `examples/json/` is **671** lines that render and parse a
`variant` by hand, and **20** functions named `render`, `show`, `shown`,
`rendered` or `to_str_of` are hand-written across `examples/`
(`grep -rhoE '^function (to_str|render|to_json|from_json|show|format)_?[a-z]*' examples/`,
2026-09-03). The comptime paragraph under Part 6 (`design.md:2406-2428`) is the
nearest ruling, and its three return conditions bind here too.

**Why here.** M-core-packages step 2 is `encoding/json`, and Go's is built on
reflection. Whatever the ruling, that package is written after it. Full five
seats; a refusal lands as a Part 6 row with its falsifier (CLAUDE.md §12).

**A third question, and it is the author's** (2026-09-06: *"The other possibility
is adding annotations like Java's or Python's, though not meant as decorators:
simply tags one can put on some parts of the code, which the compiler can use —
so system tags, or other tags that could be defined by the user to do something.
If this does not complicate things. Because I see some keywords that look exactly
like decorators, such as `owned fclose`."*). **(iii) A general annotation
mechanism, Java's shape: user-defined tags on declarations.** The sitting rules
on it with a Part 6 row and its falsifier (CLAUDE.md §12), because its only
refusal today is the panel 018 bullet quoted above
(`docs/panel/018-top-level-declaration-shape.md:97-100`), rejected along the way
and named by no DESIGN-LOG row. **The author's observation is what split the
question, and it is correct**: `owned fclose` IS an annotation. The language has
six — `owned`, `tag`, `partial`, `link`, `package`, `as` — every one a contextual
word matched by its text in `selfhost/parse/` and none in
`selfhost/keywords.hero`'s table of 21, each read in one position after the thing
it modifies, each carrying a check, so none is inert; measured 2026-09-07 from the
parser, not from memory. Those six are §4.19's, so their shape rule is
M-core-packages' question (vii) and not this sitting's. What this sitting is
handed for (iii): panel 114 R7's clause that a tag set is *"closed and enumerated
in the spec, so an unlisted tag is a compile error rather than a dead branch"*
(`docs/panel/114-the-question-was-not-which-platform.md:244-252`) with its
witness at `:150-156` — Rust's `#[cfg(feature = "widnows")]`, compiling clean and
deleting a function in silence — which a user-DEFINED set cannot satisfy by
construction; §1.3's *"constructs whose meaning lives elsewhere"*
(`design.md:217`), since a tag the compiler cannot check is read by something the
line does not show, and in Java that something is run-time reflection, this
sitting's own first question; `design.md:1953`, which makes `@name` in prefix
position a syntax error on purpose; **and the honest argument for it**, that one
general syntax pays CLAUDE.md §9's tooling bill once rather than once per word —
true, and the cheap half, because the check each word carries in the checker is
the expensive half and a general syntax does not pay it. A decorator in Python's
sense, a function that takes a function and gives back another, needs a closure
to build and is M-closures-verdict's, named here and not judged.
Measured 2026-09-07: `decorator` appears three times across design.md, `spec/`,
`docs/panel/`, `DESIGN-LOG.md`, `docs/work/`, this file and CLAUDE.md — the prior's
refusal in `spec/reserved-words.md:30` and FastAPI twice — and zero times in any
sitting; spec headroom **225**.

*******************************************************************************
**OPEN: 1**

- [ ] **M-reflection-verdict** | shape B-prime, the generated per-field walk with handlers as parameters, which is the only route to the ROT — scheduled with what it owes, and NOT adopted | `docs/panel/132-two-seats-invented-the-same-thing-and-two-others-invented-the-other.md` · `selfhost/emit/structural.hero` · `selfhost/ir/mono.hero`

    **Origin:** panel 132, 2026-09-11, where it was reached twice independently.
    The ffi-pragmatist found that a record of handlers cannot work because **a
    record cannot be generic**, so its accumulator is fixed at one type and every
    byte must buffer in a Heroes string before reaching C — design.md §1.11's
    level-3 glue inverted; it moved the handlers into parameters, freeing the
    accumulator, and compiled one walk driving both a Heroes string and a live
    `FILE *`, plus one builder consuming both a `Json` and a live SQLite row.
    Independently the historian, reading Ada's `T'Write` (ISO 1995) and
    `T'Put_Image` (ISO 2022), wrote that the precedent transfers *"only if the
    per-scalar writers are passed as ordinary function arguments"*.

    **Why it is scheduled and not adopted, and this is the distinction that cost
    panel 117 four days ago.** What was compiled is a **hand-written simulation of
    what the compiler would generate**, 78 lines of it, not the generation. The
    boundary half is measured and sound — `HeroDesc` still **5** members, the
    header's struct by value and unredeclared, `_Static_assert`s intact,
    monomorphised `HERO_TU_LOCAL` so dead-code elimination survives, **nothing in
    the value ever** — and the compiler side is unrun. Field-name literals go from
    **18 to 0**, which is the whole buy and it is real.

    **Three refusals its own proposing seat calls blocking, each compiled.** A
    walk over a `partial` record builds at exit 0 reporting 12 bytes of a measured
    `sizeof(struct stat) == 144`, and **consuming one is not merely an abort but
    impossible**, there being no honest construction call. `sort(keys(m))` on
    `{Point: i64}` is `error[unordered_element]`, and brute force found **four**
    pairs of `==`-equal maps marshalling to different bytes. And the ergonomic
    cost is a 13-argument call whose generated signature is a **577-character line
    the formatter does not wrap**.

    **Two predictions must be run before it lands**, both the historian's: that it
    **fails on a record whose field is a list of records**, because the nested walk
    must itself receive the writer set and the instantiation cannot be inferred at
    the inner call site; and that derivation is **linear** in field count at 8, 64
    and 512 fields, which scores that seat's own panel 131 warning — if the 512
    case exceeds 8x the 64 case, the quadratic disease it sourced for Haskell's
    `Generic` is in this shape too.

    **And Ada's own documented mistake is the third thing owed**: GNAT says the
    default derived format is *"deliberately not documented and subject to
    change"* and tells users not to depend on field order. A Heroes walk feeding a
    JSON package must fix field order in the document, or a golden will pin what
    the compiler never promised.

*******************************************************************************

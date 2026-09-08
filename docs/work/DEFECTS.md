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
collision stands there; the next number to issue is **017**.

Format: `- [ ] **NNN — <title>** | <what it does, in one line> | <where to look>`

*******************************************************************************
**OPEN: 1**

- [ ] **017 — `heroes measure` overstates the spec's usable room by 60 tokens** | the number a session consults before amending the spec is 60 larger than the number the net goes red at, and both are printed by this repository | `selfhost/cli/measure.hero:157` · `tests/harness/suite_spec.hero:203`

    **Origin:** the spec-warden seat, panel 118, 2026-09-08, which said "the warden's own instrument overstates usable room by 60 tokens — file it"; verified by the coordinator by reading both sites, 2026-09-08.

    **Reproducer**, run 2026-09-08 from the seed-built compiler:

        $ ./heroes measure spec/heroes-spec.md
          maximum            3903   the binding number
        Above the soft 2000: an addition needs a named removal or a
        pre-registered falsifiable prediction (panel 012). Headroom: 193.

    **The wrong answer at exit 0.** `selfhost/cli/measure.hero:157` prints
    `CEILING - highest`, which is 4096 - 3903 = 193. The net's own check is
    `SPEC_TOKENS + FFI_FLOOR >= CEILING` at `tests/harness/suite_spec.hero:203`,
    with `FFI_FLOOR` 60, so the largest green `SPEC_TOKENS` is 4035 and the real
    room is **132**. A session that trusts the tool it is told to trust can spend
    193 and find out from a red suite, which is the failure the tool exists to
    prevent.

    **Cause.** `measure` is a general instrument and knows nothing of a
    spec-specific mortgage; the mortgage lives only in the harness. So the
    number is right for any other file and wrong for the one document whose
    budget it was built to settle.

    **What is owed.** Not obviously the tool: making `measure` subtract a
    harness constant couples the compiler to the test suite. The cheaper repair
    is for `measure` to name the mortgage where it knows the file, or for the
    printed line to stop calling itself headroom. Either way it is a **class**
    and not a witness: any second claim on the same ceiling has the same shape.
    A `tests/harness/` case that asserts the two numbers agree is the instrument.

*******************************************************************************

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

- [ ] **016 — `assert` drops both sides for every aggregate** | the spec promises both sides and the compiler prints neither | `selfhost/emit/abort.hero` · `spec/heroes-spec.md:202`

    **Origin:** 2026-09-06, found while measuring what a debugger would have to
    show in a stopped frame. Re-run 2026-09-07 at `f1ebe1fc`.
    **Status:** open, and the repair is gated on a scheduled ruling.
    **Severity:** a silence where a message is owed, and the message is the one
    an author reads at the moment a test fails.

    **The reproducer**, three `test` blocks in one file, run with `heroes test`:

    ```
    test "scalars"
        assert 3 + 4 == 8

    test "arrays"
        assert [1, 2] == [1, 3]
    ```

    What it prints:

    ```
    assert failed: 3 + 4 == 8
      left:  7
      right: 8
    assert failed: [1, 2] == [1, 3]
    ```

    The scalar case shows both sides. The array case shows **nothing** — no
    `left:`, no `right:`, no note that they were withheld. A record comparison
    behaves the same way, and so does every `{K: V}`, every `T?` and every variant.

    **What the spec promises.** `spec/heroes-spec.md:202`: *"An `assert` failure
    shows the source expression and both sides."* It states no exception, and
    CLAUDE.md §12 is the precedence rule: the spec beats the compiler, so this is
    the compiler's defect and not a loose sentence.

    **The cause, and it is one function.** `selfhost/emit/abort.hero` asks
    `builtins.to_str_entry` for a rendering of each side and emits
    `hero_panic_assert_sides` only `if rendered.len() == 2`; otherwise it falls
    through to `hero_panic_assert`, which takes the source text alone.
    `to_str_entry` answers for the integers, the floats, `bool` and `str`, and
    fails for `.array`, `.map`, `.named`, `.fallible`, `.case_ty` and the rest — the
    same table that makes `print(p)` on a record `error[bad_operand]`. So the
    failure is not in `assert`: it is that **this language cannot render an
    aggregate to text at all**, and `assert` is the one place where that silence
    reaches an author who is not asking for it.

    **What is owed, and why it is not repaired here.** The repair is a rendering
    for an aggregate, which is a `to_str` derived over a record's fields — the
    question M-reflection-verdict is convened for, in that milestone's own words.
    Filing it here rather than repairing it is deliberate: a defect entry is where
    a measured failure lives while it is open, and taking the design decision now
    would settle a scheduled sitting from inside a defect report. Two things are
    owed the day it is repaired: the sides for every aggregate shape, and a
    `fixedbugs` case per shape, because the provoking case is a witness and not the
    class. One thing is owed **before** then, and it costs nothing: if the sitting
    refuses derivation, `spec:202` is the sentence that must change, and the
    refusal pays for it out of the spec's headroom rather than leaving a promise
    the compiler will never keep.

    **Found by**: measuring what `M-typed-inspection` would have to show in a
    stopped frame. The debugger question and this one meet at the same table, which
    is why the ROADMAP row for that milestone names this entry.

*******************************************************************************

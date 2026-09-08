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
collision stands there; the next number to issue is **020** — 017 was issued
and closed the same day, 2026-09-08, and is in the record, and 018 and 019 were
issued the same day and are open below.

Format: `- [ ] **NNN — <title>** | <what it does, in one line> | <where to look>`

*******************************************************************************
**OPEN: 2**

- [ ] **018 — `<` names two of the ten types it accepts** | a comparison refused between two values of one non-ordered type says "takes `i64` or `f64`", where the checker accepts every integer and float width | `selfhost/check/ops.hero:81-90` · `:131` · `:168` · `:270`

    **Origin:** found 2026-09-08 grounding the llm-ergonomist's finding at panel
    119, which predicted that the modal first-try failure in every arm of its
    experiment is `a < b` on two `str`. The message that greets that failure is
    the wrong one.

    Reproducer, run on this Mac 2026-09-08 — `a = "apple"`, `b = "banana"`,
    `print(a < b)`:
    ```
    error[bad_operand]: `<` takes `i64` or `f64`, found `str`
    ```
    **The checker is right and the text is wrong.** `ops.hero:81-82` accepts
    `.int_ty | .float_ty` — all eight integer widths and both floats — and
    `heroes run` confirms it: `u8 < u8`, `i32 < i32` and `f32 < f32` each print
    `true`, and unary `-` on an `i32` prints `-3`. The literal reaches
    `value_errors.bad_operand` at three sites: `:90` the comparison, `:131` a
    `str` under an arithmetic operator, `:168` unary `-`.

    **The right vocabulary already exists in the sibling message**, which is
    what makes this a defect rather than a wording preference:
    `mixed_arithmetic` says *"`<` takes two of one type — two integers of the
    SAME width, or two floats of the same width"*, and it fires whenever the two
    sides differ. The wrong text fires only when both sides are the same
    non-ordered type, which is the shape a reader writes first.

    **The repair's whole surface, measured 2026-09-08:** zero goldens pin the
    text, and outside `selfhost/check/ops.hero` the string appears nowhere in
    `selfhost/`. So it is three producer sites plus that file's own assert at
    `:270`, and a `tests/golden/fixedbugs/` case per shape — two `str`, two
    `bool`, and the unary site.

    **Why it matters:** a reader holding a `u8` is told the language compares
    only `i64` and `f64`, and converts a value that needed no conversion.

- [ ] **019 — the compiler's own fix names a family the language removed** | `mixed_arithmetic`'s `guess` fix offers `fit_<width>(x)`, and nothing named `fit_i32` is in scope | `selfhost/value_errors.hero:32` · `selfhost/check/builtins.hero:399` · `design.md:923` · `docs/panel/043`

    **Origin:** found 2026-09-08 following defect 018's reproducer one step
    further — reading the fix the compiler offers, and then running it.

    Reproducer, run on this Mac 2026-09-08:
    ```
    $ heroes check mixed.hero        # a: u8 @ 3 · b: i32 @ 4 · print(a < b)
      fix (guess): convert one side: `to_f64(x)`, `to_i64(x)`, or `fit_<width>(x)` between widths
    $ heroes check fit.hero          # b = fit_i32(a)
    error[unknown_name]: nothing named `fit_i32` is in scope
    ```
    **And the family was not merely never built: it was built and withdrawn.**
    `fit_<w>` shipped at M-sized-integers step 7 (`docs/panel/043` § Q1), and
    `design.md:923` records why it went — panel 017's naming rule, *"the three
    conversions share one scheme and a model can derive the third from the two
    the spec already lists"*, and *"a family called `fit_` broke exactly that"*.
    So the compiler recommends, in a diagnostic, the name this project decided
    against.

    The correct repair is the spec's own `to_<width>`, which gives a `T?`
    because the number may not fit (`spec:64-69`). Verified 2026-09-08:
    `to_i32(a).must() < b` runs and prints `true`.

    **Second site, same name:** `selfhost/check/builtins.hero:399` tells a
    reader *"this is the shape a `fit_<width>` that widens hands back, because a
    widening cannot fail"* — describing a function that does not exist, while
    explaining a different error.

    **The repair's surface:** two strings, and zero goldens pin either, measured
    2026-09-08. `guess` rather than `certain` is the one thing limiting the
    damage: `heroes check --apply` will not write it for you.

    **Why it matters:** a diagnostic exists to let a reader fix the program
    without opening another file (design.md §4.17). This one sends them to a
    name that produces a second error.

*******************************************************************************

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
collision stands there; the next number to issue is **022** — 017 was issued
and closed the same day, 2026-09-08, and is in the record, and 018 to 021 were
issued the same day and are open below.

Format: `- [ ] **NNN — <title>** | <what it does, in one line> | <where to look>`

*******************************************************************************
**OPEN: 4**

- [ ] **020 — the library's `extern` constants are readable from every program** | `print(HERO_OS_OK)` runs and prints `0` in a plain user program, while the library's `extern` FUNCTIONS are properly walled off | `selfhost/resolve/names.hero:60` · `:53` · `:105` · `selfhost/library_source.hero:135-137` · `docs/panel/033` R5

    **Origin:** the compiler-engineer's seat, panel 120, 2026-09-08, found while
    enumerating the embedded library to answer whether the spec's list of seven
    is complete.

    Reproducer, run on this Mac 2026-09-08:
    ```
    $ heroes run ext3.hero          # print(HERO_OS_OK) · print(HERO_OS_NOT_FOUND)
    0
    1
    ```
    **The wall exists and has a hole in exactly one match arm.** A user program
    writing `function filter` gets `error[builtin_name_taken]`, and one calling
    `hero_args_count()` gets `error[extern_across_modules]` with a §4.19 note. But
    `selfhost/resolve/names.hero:60` reads
    `.constant_decl | .record_decl | .test_decl => state.record_use(…)` with **no
    `is_extern` guard**, while `.function_decl` carries one at `:53` and again at
    `:105`. Panel 033 R5's rule was applied to one kind of extern and not the
    other.

    **It is `.claude/rules/module-shape.md`'s narrowing-on-a-premise**: the arm
    groups `constant_decl` with kinds that cannot be extern, and `constant_decl`
    can — `library_source.hero:135-137` and `:148` declare four of them. The
    discriminator already exists: `ast.hero:425-429` gives `constant_decl` a
    `header: token.Span?`, set at `parse/tails.hero:114`. The seat priced the
    repair at **~7 lines, one arm**, plus one golden with its `#~` annotation.

    **Why it matters:** four names a program never declared are in its scope, and
    a program that reads one is relying on the compiler's own plumbing.

- [ ] **021 — a diagnostic shows a reader `#0`, which is not a type anyone can write** | a six-line program with an ordinary generic function is told `found [#0]`, where the file that renders it states in writing that this cannot happen to a user | `selfhost/check/render.hero:19-23` · `:51-53` · `selfhost/check/state.hero:198` · `selfhost/check/walk.hero:1539` · `:1617`

    **Origin:** the compiler-engineer's seat, panel 120, 2026-09-08, and the
    coordinator's own reproducer is shorter than the seat's.

    Reproducer, run on this Mac 2026-09-08:
    ```
    $ heroes check z.hero
    error[bad_operand]: `join` takes `[str]` and `str`, found `[#0]`
    ```
    from six lines: `function id<T>(x: T) -> T` returning `x`, and
    `print(join(xs.map(id).map(id), ", "))`.

    **The premise is stated and it is dead.** `render.hero:19-23` says an empty
    generic-name list *"is legal and prints `#0`, which only happens where a
    caller outside the checker has no function in hand"*, and `:51` says *"A type
    parameter prints as it was declared: `A`, not `#0`"*. Both are false of a
    user program. Cause: `check/state.hero:198` renders with `c.generic_names` —
    the **caller's** letters — so a callee's type parameter has no name inside
    `main()`.

    **The machinery to fix it already exists**: `check/walk.hero:1539` and `:1617`
    already pass `callee_generics`/`callee_letters` for two other messages. The
    seat priced it at 3-4 lines for a loud fallback or ~20 to thread the callee's
    letters. **No golden asserts `#0`** — the one hit,
    `tests/golden/run/fixedbugs-a-context-bound-parameter-through-a-generic.hero:4`,
    is a comment.

    **Why it matters:** design.md §4.17 asks a diagnostic to carry everything
    needed to fix the program without opening another file, and `#0` is a
    placeholder from the checker's own tables. The seat's note is the sharpest
    part: this fires on the exact misreading of `fold`'s argument order that
    panel 120's new sentence exists to prevent.

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

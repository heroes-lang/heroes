- [x] **060 — a header record with a `const` member makes the compiler blame itself** | `internal error: compiling the generated C failed` at exit 2 for an `extern` the author wrote, where `.claude/rules/c-boundary.md` asks for exit 1 on the `.hero` line | `selfhost/emit/ffi_field.hero` · `selfhost/emit/ffi.hero` · `tests/golden/unsupported/ffi-const-member.hero`

    **Origin:** panel 161's ffi-pragmatist found it beside the `char` question,
    2026-09-17; reproduced by the coordinator on Darwin arm64 before filing.

    **The repair.** A twenty-first reader in `emit/ffi.hero`'s `explain()`:
    `emit/ffi_field.const_member`. It recovers the member name from clang's
    `cannot assign to variable '…' with const-qualified data member '<m>'` and
    asks which of THIS program's group records declares a field of that name —
    CLAUDE.md §7's `declaration()` narrowing unchanged, so a `const` member in
    anybody else's struct still stays exit 2 and the compiler's fault.

    **The location was right all along and only the verdict was wrong.** clang's
    error already lands on a `#line` pointing at the author's file; `explain()`
    walked twenty readers, none recognised the wording, so it returned nothing,
    `blamed()` answered no, and the failure fell through to `internal error`.

    **Before and after, measured on the reproducer:**

    ```
    before   internal error: compiling the generated C failed        exit 2
             …then clang's raw wording about a variable `t3`
             the author never wrote

    after    error[ffi_const_field]: `cm.h` declares `Locked.id` as   exit 1
             `const`, so C refuses to assign a `Locked` once it
             exists — and building one here is that assignment
               at cm.hero:3:9, caret on `id`
             note: …take it FROM the library rather than make it
    ```

    **It reads the ERROR line alone and never the warnings above it.** clang
    names the struct's TYPE only in the warning, and holding state across
    clang's output is a premise about clang's layout that panel 038 already paid
    for once — `emit/ffi_record.hero`'s own module doc says so. The member name
    is therefore the whole key.

    **Why no sitting, written down so it can be disagreed with.** The repair
    does not move the line between accepted and refused: the program was refused
    before and is refused after, and what changed is who is blamed and what the
    message says. It is a fifth member of `.claude/rules/c-boundary.md`'s named
    exception — *a clang failure the author's own extern caused*, panel 036,
    widened by panel 048 — and that class's own narrowing rule admits it
    unchanged. **Measured before deciding it**: a clang JSON AST walk over the
    six headers this project binds (`time.h`, `stdio.h`, `sqlite3.h`,
    `curl/curl.h`, `dirent.h`, `pwd.h`) finds **zero** `const` non-pointer
    struct members, so the shape is real in C and absent from this corpus.

    **The verification:** `unsupported` 13 → **14 passed, 0 failed**;
    `annotations` 163 → **164 passed, 0 failed**; `canonical` 2 passed;
    the compiler's own tests 654 → **655, all passed**; the seed regenerated and
    the fixpoint verified byte-identical; the full net re-run whole.

    **The golden case is `tests/golden/unsupported/ffi-const-member.hero`**, with
    its own local header, because no header this project binds has the shape. It
    carries the `#~ ffi_const_field` annotation as well as the `.expected`
    snapshot: a regenerator can rewrite a snapshot and cannot invent an
    annotation.

    **Two shapes beside it stay unrun and are named rather than implied**
    (CL-061): a `const` member in a `partial` record, and a nested record
    holding one. A third is measured — panel 161's ffi-pragmatist found
    `const char[N]` refused at every spelling, and that **it is the `const` and
    not the `char`**, since `const int32_t n[4]` is refused too — but whether
    those refusals carry a good message or this same internal error was not
    measured, and this repair does not claim them.

- [x] **037 — a handle over a struct C names with two words cannot be spelled, so the author falls back to `ptr` and leaks at exit 0** | where a group consumes a `ptr`, nothing demands a mark on the call that hands one back, so the program leaks with zero diagnostics on every platform | **repaired 2026-09-15**, both halves, and the route is panel 152 R1 obeyed and panel 150 R3 landed | `selfhost/check/acquiring.hero`'s `consumed_types` · `selfhost/handles.hero`'s `is_handle` · `runtime/parts/alloc.c`

    **Origin:** measured by panel 149's **ffi-pragmatist**, which called it *"the
    more serious"* of the two holes that sitting looked at, and re-measured by
    the coordinator on 2026-09-15 before this entry was written.

    **The reproducer, and it is three lines of binding.**

    ```
    extern "pool.h"
        function opaque_open(n: i64) -> ptr
        function opaque_close(p: ptr consumes)
    ```

    A program that calls `opaque_open` and never `opaque_close`: `heroes check`
    exits **0**, the built program exits **0**, stderr is empty, and the pointer
    is never released. The identical binding written with a handle type aborts at
    134 and, since panel 149, is refused at compile time.

    **The cause is that the rule is handle-gated and a `ptr` is not a handle.**
    `consumed_types` asks `reaches_handle` of each `consumes` parameter, and
    `handles.is_handle` answers only for a group `record` with a `tag` and no
    fields. A `ptr` has no pointee in the type system at all.

    **The runtime half already works, which is what makes this a frontend gap
    rather than a design one.** Measured by the ffi seat: `acquires` and
    `consumes` on a `ptr` DO count, and a `ptr` version with the release skipped
    panics *"1 C handle(s) never given back"*, byte-identical to the handle case.
    Only the static demand is missing.

    **It outranks what panel 149 repaired, and the sitting said so.** That
    repair's hole was LOUD — the program aborted at 134 naming its own cause.
    This one is SILENT: exit 0, no diagnostic, a real leak. design.md §1.12 and
    CLAUDE.md § Precedence rank 3 put a silent corruption class above a loud one.

    **THE OPTION SET WAS SHORT BY TWO, measured 2026-09-15 after panel 150.**
    The sitting adopted *a `tag` names a C type name* and its ffi-pragmatist said
    in as many words that it was not recommending a spelling. Two routes exist
    and neither was priced:

    **Route A, a surface change.** `tag struct addrinfo` is `error[reserved_word]`
    today, raised in the LEXER at `selfhost/scan.hero:128`, so the parser never
    sees a usable identifier — `struct`, `union` and `enum` are refused as text
    and the refusal cannot know it is in a tag position. Freeing the position
    means either moving that refusal to where position is known, which weakens a
    diagnostic defect 015 paid for, or a second spelling such as
    `tag "struct addrinfo"`, which is consistent with `extern "netdb.h"` putting
    C text in quotes two words earlier and is two forms where the project
    prefers one. **Either is surface syntax and owes a sitting.**

    **Route B, and it needs NO surface change at all.** The compiler already asks
    clang what a header's type text is: `cli/pointee.hero` declares
    `extern __typeof__(f) hero_ty_f;` per extern and reads the `qualType` out of
    `-ast-dump=json`, which for `freeaddrinfo(ai: AI consumes)` is the header's
    own `struct addrinfo *`. So the correct spelling is already reachable and the
    author would not have to say it twice. `handles.c_spelling` writes
    `tag_text + " *"` from the author's word alone, and that is the one line the
    answer would replace. This is the direction panel 150's ffi-pragmatist
    praised — the only route measured to INCREASE what clang checks — arrived at
    from the other side.

    **What was re-measured here, so a later session need not.** `netdb.h` on this
    Mac declares **19** struct-returning entry points and **zero** `typedef` for
    `addrinfo`, so this is not one header's quirk. `tag struct addrinfo` is
    `error[reserved_word]`. And `record Blob tag void` works but is capped at
    ONE per program by `duplicate_tag`, which is why it is not the answer.

    **HALF OF THIS CLOSED 2026-09-15, and the half that closed is the one a
    program author meets.** Until then `record AI tag addrinfo` against `netdb.h`
    emitted `addrinfo *`, clang refused it, and the author got **`internal error:
    compiling the generated C failed`, exit 2, twenty-two raw clang errors, zero
    Heroes diagnostics**, and a path into a generated file — the failure §4.19's
    own guarantee exists to prevent.

    **The answer was already spoken and one line was throwing it away.** clang
    says *"must use 'struct' tag to refer to type 'addrinfo'"*;
    `emit/ffi_tag.hero` has read that exact string since panel 074; and
    `emit/ffi_lookup.hero` discarded it by comparing the declaration's Heroes
    NAME where clang's message carries C's. One comparison, repaired: a record is
    now found by its TAG as well as by its name.

    **Measured after**: `error[ffi_tag_needs_struct]` with the caret on the
    author's own line, saying the tag is RIGHT and that this compiler cannot
    write it. A misspelled tag stays `error[ffi_unknown_name]`. A record with no
    tag keeps the old `certain` fix, unchanged.

    **The diagnostic offers no fix, and that is deliberate.** Its sibling offers
    a `certain` one because a record with no tag can be given one; here the tag is
    already correct and the repair does not exist, so a fix would be the shape
    `.claude/rules/diagnostics-and-goldens.md` refuses — one that leaves the
    defect standing. The case is `tests/golden/fixedbugs/fixedbugs-a-tag-that-needs-struct.hero`,
    and its blessed emission **pins the wrong C in bytes**: `addrinfo *`, five
    times, so the day the capability lands the file moves and a suite says so.

    **WHAT REMAINS OPEN is the capability.** The author now knows exactly what is
    wrong and still cannot write the binding. Panel 152 adopted the route and it
    is queued for ratification: supply the qualifier **from clang's own refusal**,
    which is the message this diagnostic already reads, and never from a generated
    probe — because `struct nosuchtype *p;` compiles at exit 0, so one bit cannot
    tell a missing qualifier from a misspelled tag.

    **PANEL 151 REFUSED ROUTE B AND COMPILED THE NARROWING**, 2026-09-15, both
    seats with a veto. A probe generated from the header cannot check the binding
    against the header: measured, `record Db tag sqlite3_stmt` gives two
    `error[ffi_parameter_type]` today and zero under Route B. **Route A stands
    and Route B is struck.**

    **What replaces it, compiled by the ffi-pragmatist**: ask clang ONE BIT per
    tag — *does this word need `struct` in front of it?* — and keep the author's
    word as the identity, so every refusal survives. `addrinfo *hero_tagprobe;`
    fails to compile, `CURL *hero_tagprobe;` succeeds, and that single bit is the
    whole answer. 0.02 s per tag, cacheable under the existing key. **It is owed
    a FULL sitting**, queued as `panel 151` in `docs/work/DECIDE.md`, because it
    amends design.md:2192 and §4.19's *"no external tool, no libclang"*.

    **What is owed, and why it is not done here.** Demanding a mark on a `ptr`
    producer widens what a diagnostic refuses, which is a diagnostic CLASS, so
    CLAUDE.md § 4 sends it to the panel. The question that sitting has to answer
    is not *should a `ptr` be marked* but **which `ptr`**: every C function
    returning a pointer would be caught by the naive rule, and this compiler's
    own source calls several. `struct addrinfo` is the case to brief it on,
    because the ffi seat measured that `getaddrinfo` cannot be written any other
    way: `netdb.h` on this Mac has no `typedef` for `addrinfo`, so the handle
    form does not compile and `ptr` is the only spelling that works.

    **THE REPAIR, 2026-09-15, both halves in one step.**

    **The capability: `struct` is supplied from clang's own refusal and never
    guessed.** The emitter goes on writing the author's word — `record AI tag
    addrinfo` is `addrinfo *` — and `cli/assemble.hero` compiles one ROUND: the
    pointee check, the fused unit or one translation unit per module, the link.
    When a round fails, `cli/produce.hero` hands every refusal it met to
    `emit/ffi_tag.hero`'s `needs_struct`, which reads *must use 'struct' tag to
    refer to type 'X'* for a word some handle of this program carries as its
    tag, and the round runs again with `struct X *` where `X *` was
    (`handles.spelled`, the one place both spellings are written). A round that
    learns nothing new ends the loop, so it is bounded by the tagged handles in
    the program, and a program whose tags need nothing runs one round as
    before. `--emit-c`, the seed and every blessed emission write the author's
    word, because the first emission is always his. **Measured**: the
    one-module case builds cold in **0.47 s** and warm in **0.30 s**, the second
    round's object being cached under its own text; `getaddrinfo` against
    `netdb.h` builds and prints `0` on this Mac plain and under `--sanitize`,
    and in the Linux container under `--sanitize`; two struct-only tags in one
    header are learned from one refusal; a handle declared in one module and
    used in another is spelled `struct probe *` in both translation units.
    **The shape next to the repair stays refused**: `tag addrinfoo` is
    `error[ffi_unknown_name]` before and after, because clang says *unknown type
    name* for it and never *must use 'struct'*, which is the whole reason panel
    152 refused a one-bit probe.

    **The silent half: a mark nothing reads is refused** (panel 150 R3,
    `check/marks.hero`, `error[unread_mark]`). `acquires`, `borrows` and
    `consumes` on a result or parameter whose type reaches no handle — a `ptr`,
    a `cstr`, an `i32` — are refused with the handle spelling named as the
    repair; on a handle they are read as before. Every mark in `examples/` and
    `tests/golden/` already sat on a handle, measured before the rule landed,
    so it refused no program; the one golden written over `ptr consumes`
    became `record Block tag void`, and its diagnostic reads as it did.

    **What paid, and what was scored.** The spec is **+1 vendored and +0
    real** (7974 to 7974, digest `2e77c4e72ce69512`): the −3 phrase *what the
    header leaves opaque* and panel 150's +7 correction cancelled on the
    reader's instrument. Panel 152 R3's **7978** is MISSED — the landed text is
    not the text it priced, by the warden's own clause — and panel 150's
    warden's **7981** is MISSED for the same reason, with D1 not landing alone.
    design.md:2192 stops saying the tag is written *verbatim* (panel 152 R4),
    and Part 6 gains the row for the refused mark with its falsifier: two
    opaque `void *` families in one program, which `one_tag_one_type` caps at
    one `tag void`.

    **Gated by**: the compiler's own tests **649** (three new), and every suite
    of the net one at a time, all green after the day's first full net came
    back 1855 passed, 6 failed — four of them the binary carrying the spec's old
    pinned digest because the pins moved while the seed was emitted, two of
    them real and repaired: `emit/ctype.hero` at 399 lines against a decided
    395, and the two run emissions unblessed. The seed is regenerated at
    **818,752** lines and re-emitted byte for byte by the compiler built from
    it. Cases: `tests/golden/run/fixedbugs-a-tag-that-needs-struct.hero`,
    `tests/golden/run/fixedbugs-getaddrinfo-is-bindable.hero`,
    `tests/golden/surface-fixtures/structtag/`,
    `tests/golden/check/unread-mark.hero`,
    `tests/golden/fixedbugs/fixedbugs-a-misspelled-handle-tag-stays-refused.hero`.

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
collision stands there; the next number to issue is **034** (022 and 023 were issued on 2026-09-08, 022 was SPLIT on 2026-09-09 by panel 122 into 022 and 024, and all three closed the same day, 024 last, at M-held-bytes; **025** and **026** were issued and closed on 2026-09-11 at M-labelled-builtins and M-named-callbacks, **027** was issued 2026-09-11 beside panel 131 and **028** beside panel 132, and **029** was issued 2026-09-13 beside panel 135 and **030** the same day beside panel 137, **031** beside panel 139; **032** was issued and closed on 2026-09-14 at M-cleanup-verdict step 1, and it is the first since 025 that no sitting produced — the census that opens this milestone found it, and the compiler simply disagreed with `spec § 13`; **033** was issued on 2026-09-14 at the M-marked-acquisition close, by the full net before the push, and it is the first here found by attacking the shapes NEXT to a repair rather than the repair itself; **034** was issued on 2026-09-14 by panel 149's spec-warden while it was pricing 033's repair, and it is the first here that a SITTING CONVENED ON ANOTHER DEFECT found — the seat went looking for what the specification already said and found the compiler disagreeing with it somewhere nobody had asked about; **035** and **036** were issued the same day by the same sitting, 035 by its compiler-engineer and 036 by its completeness critic, so **panel 149 produced three defects while ruling on a fourth** and the next number to issue is **037**) — 017 to 021 were
all issued on 2026-09-08 and all closed on 2026-09-08, and all five are in the
record.

Format: `- [ ] **NNN — <title>** | <what it does, in one line> | <where to look>`

*******************************************************************************
**OPEN: 4**

- [ ] **033 — `unmarked_handle_producer` does not look inside a returned record** | an `extern` handing a handle back in a struct FIELD carries no mark, the checker says nothing, and the imbalance is caught at exit instead of at compile time | `selfhost/check/acquiring.hero` · `tests/golden/run/abort-handle-given-back-unmarked.hero` · `runtime/parts/alloc.c`'s fourth exit check

    **Origin:** found 2026-09-14 at the M-marked-acquisition close, by the full
    net before the push and then by attacking the shapes next to the one that
    provoked it (CLAUDE.md § RUN IT). Filed after the tag, so the tag's own
    commit stands over a clean list.

    **The reproducer runs and it is in the tree.** `pair_make(n: i64) -> Pair`
    where `record Pair` holds `s: Slot` and `Slot` is a handle: the program
    compiles with no diagnostic, prints 7, and aborts at exit 134 saying the
    count went to -1. R5 reads RESULTS and `@` out-parameters and asks
    `handle_behind` of each; a record type whose FIELD is a handle answers no,
    so `pair_make` is not a producer as far as the rule is concerned.

    **It is a real C shape and not a contrivance.** `struct addrinfo` carries
    pointers the caller must free, and a struct-returning constructor handing
    back an owned member is ordinary in C. The three shipped bindings here do
    not use it, which is why nothing fired.

    **What is owed, and why it is not done here.** Widening what a diagnostic
    refuses is a diagnostic CLASS, so CLAUDE.md § 4 sends it to the panel; the
    milestone was closed and tagged when this was found. The question for that
    sitting is not *should the rule see fields* but **how deep**: a handle
    behind two records, behind an optional, behind a list. The cheap answer
    stops at one level and would be a premise about the world in the sense
    `.claude/rules/module-shape.md` refuses.

    **What holds meanwhile is written down rather than assumed**: the program
    does not corrupt anything silently. The counter catches it at exit, and
    since 2026-09-14 its message names this shape explicitly as one of the
    three that reach it. Robustness is preserved (CLAUDE.md § Precedence 3);
    what is missing is the earlier catch.

- [ ] **034 — the handle counter counts CALLS and not MARKS, so it kills the correct program and blesses the leaking one** | one `extern` carrying two `acquires` marks emits ONE increment per call, so releasing both handles aborts at 134 and releasing one exits 0 | `selfhost/emit/ops.hero:162` · `selfhost/handles.hero`'s `hands_a_handle_over` · `runtime/parts/alloc.c`'s fourth exit check

    **Origin:** found 2026-09-14 by **panel 149's spec-warden**, which was
    convened on defect 033 and went looking for what `spec § 13` already said
    rather than for what 033 described. Reproduced independently in the same
    sitting by the **compiler-engineer** seat, with two parameters of ONE handle
    type, and re-run by the coordinator with two DISTINCT handle types before
    this entry was written (CLAUDE.md § RUN IT: a number is measured in the
    session that writes it).

    **This is worse than 033 and it is filed second only because numbers are
    never reused.** 033 is a missing refusal: the program is wrong and nothing
    says so until exit. This one is an INVERTED instrument. Measured, with
    `mixed_out(n: i64, @a: Slot acquires slot_close, @b: Conn acquires
    conn_close)`:

    | the program | what it does | exit |
    |---|---|---|
    | closes both handles — **correct** | discharges what it was given | **abort 134** |
    | closes one, leaks the other — **wrong** | leaves a handle live | **exit 0** |

    The message the correct program dies with is *"1 more handle(s) given back
    than were taken"*, and it names three causes, none of them the one that
    happened.

    **The cause is a boolean where a count belongs.** `selfhost/emit/ops.hero:162`
    asks `handles.hands_a_handle_over(decls[e.decl])`, which is a
    per-DECLARATION yes-or-no (`selfhost/handles.hero`'s `any_acquires` returns
    `bool`), and the emitter writes **one** `hero_handle_acquired();` per call
    however many marks the call carries. The emitted C is the proof: one call,
    one increment, two `hero_handle_consumed();`.

    **The specification already rules the other way, so this is a compiler bug
    and not a design question** (CLAUDE.md § 12): `spec § 13` says *"which the
    program owes it. The owing is counted"* — one owing per handle, not per
    call. It costs no spec token to settle.

    **The price is known and it is small**, priced by panel 149's
    compiler-engineer as a written, line-counted, uncompiled patch:
    `selfhost/handles.hero` 27 → 36 lines and `selfhost/emit/ops.hero:158-166`
    9 → 13, so **+13 lines across two files and exactly two call sites in the
    whole tree**. **Golden-neutral**: no declaration under `examples/`, `tests/`
    or `selfhost/` carries two `acquires` or two `consumes` today, which is why
    nothing was red.

    **It is not conditional on the sitting's resolution.** The compiler-engineer
    seat said so in those words, and it is why this is filed separately rather
    than folded into 033: whatever the panel rules about how deep a rule looks,
    a correct program must not abort.

- [ ] **035 — `reaches_handle` gives up at a depth bound and the giving up is silent** | past a nesting depth the walk returns absent, so a handle used as a map key is accepted with no diagnostic, and the same bound will be inherited by every rule that reuses the walk | `selfhost/check/map_keys.hero:103-105` · its twin's justification at `:161-163`

    **Origin:** found 2026-09-14 by **panel 149's compiler-engineer** while
    measuring the walk's termination for defect 033's repair, widened by the
    sitting's **completeness critic**, and re-measured by the coordinator before
    this entry was written.

    **Three constructions, three different boundaries, and that IS the finding.**
    The coordinator's chain — `record R0` holding the handle, then `R1` … `Rn`
    each holding the one below, with `{Rn: i64}` as the map — refuses at **15**
    and is **accepted in silence at 16, 17 and 18**. The compiler-engineer's
    construction refused through 16 and accepted from 17. The critic's refused 15
    and 16 and accepted 17 and 18. The bound is `depth > 16` in one place, and
    where it lands depends on how the program is shaped, which is exactly why a
    depth bound cannot be reasoned about from the source.

    **It has no group gate.** The critic measured that it reproduces for ordinary
    Heroes records as well as group records, so nothing about the FFI bounds it.

    **The premise was left behind on the original.** `reaches_float` at
    `map_keys.hero:161-163` carries the sentence that licenses the bound — *"the
    runtime guard still sits behind this rule, so a missed refusal costs a named
    abort rather than a wrong answer"* — and it is TRUE there. `reaches_handle`
    was written beside it with **no justification for its own bound at all**, and
    the sentence is false for it: behind the handle rule there is no runtime
    guard, only an address. `selfhost/handles.hero`'s `handle_map_key` calls the
    refusal it guards *"a hole nothing can close"*, and C recycles a closed
    handle's address, so a key stored under one is found again by a handle that
    has nothing to do with it.

    **What is owed**: a visited set keyed on the declaration index, replacing the
    bound. It is owed here and not only at 033 because this rule is wrong today,
    independently of whether the walk ever acquires a second caller.

- [ ] **036 — `owned <freer>` after a non-`cstr` result checks clean and makes clang blame the compiler** | a freer named after an integer result silently retypes it as `str?`, emits `h_library_validated` on the integer, and exits 2 saying the compiler failed | `selfhost/check/freer.hero` · `.claude/rules/c-boundary.md`'s four exit-1 classes

    **Origin:** found 2026-09-14 by **panel 149's completeness critic**, outside
    the sitting's question. **Reproduced by the coordinator, and the first
    attempt FAILED**, which is what the entry below records rather than hides.

    **The reproducer, and the condition the first run was missing.** Written
    without declaring the freer, `function labs(x: i64) -> i64 owned free` is
    refused correctly: `error[unknown_freer]` at exit 1, at check and at build.
    **The defect needs the freer DECLARED**:

    ```
    extern "stdlib.h"
        function free(p: ptr)
        function labs(x: i64) -> i64 owned free
    ```

    `heroes check` exits **0**. `heroes build` exits **2**:
    `internal error: compiling the generated C failed`, with clang reporting
    *"incompatible integer to pointer conversion passing 'int64_t' … to parameter
    of type 'const char *'"* at `h_library_validated(t5)`.

    **So the guard exists and watches the wrong half.** `unknown_freer` asks
    whether the NAME resolves and never whether the RESULT is a type `owned` can
    apply to, and `spec § 13` gives `owned` only after *"a `cstr` result or a
    `char **` out-parameter"*. The compiler disagrees with the specification
    (CLAUDE.md § 12), so this costs no spec token.

    **It is a fifth exit-1 class.** `.claude/rules/c-boundary.md` names four
    classes where a clang failure is the author's `extern` and exits 1. This one
    exits 2, the compiler blaming itself for a `.hero` file's mistake, which is
    the failure §4.19's guarantee exists to prevent. Panel 149's ffi-pragmatist
    filed a sibling of it independently: a `tag` naming a C type that needs the
    `struct` keyword also exits 2.

*******************************************************************************

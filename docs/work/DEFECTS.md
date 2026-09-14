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
collision stands there; the next number to issue is **037** (022 and 023 were issued on 2026-09-08, 022 was SPLIT on 2026-09-09 by panel 122 into 022 and 024, and all three closed the same day, 024 last, at M-held-bytes; **025** and **026** were issued and closed on 2026-09-11 at M-labelled-builtins and M-named-callbacks, **027** was issued 2026-09-11 beside panel 131 and **028** beside panel 132, and **029** was issued 2026-09-13 beside panel 135 and **030** the same day beside panel 137, **031** beside panel 139; **032** was issued and closed on 2026-09-14 at M-cleanup-verdict step 1, and it is the first since 025 that no sitting produced — the census that opens this milestone found it, and the compiler simply disagreed with `spec § 13`; **033** was issued on 2026-09-14 at the M-marked-acquisition close, by the full net before the push, and it is the first here found by attacking the shapes NEXT to a repair rather than the repair itself; **034** was issued on 2026-09-14 by panel 149's spec-warden while it was pricing 033's repair, and it is the first here that a SITTING CONVENED ON ANOTHER DEFECT found — the seat went looking for what the specification already said and found the compiler disagreeing with it somewhere nobody had asked about; **035** and **036** were issued the same day by the same sitting, 035 by its compiler-engineer and 036 by its completeness critic, so **panel 149 produced three defects while ruling on a fourth** and the next number to issue is **039** — **037** and **038** were both issued on 2026-09-15 at panel 149's ratification, out of what that sitting had NAMED and not filed: 037 is the `ptr` blind spot its ffi-pragmatist called the more serious of the two holes it looked at, and 038 is the per-element release its completeness critic found no seat had been briefed on) — 017 to 021 were
all issued on 2026-09-08 and all closed on 2026-09-08, and all five are in the
record.

Format: `- [ ] **NNN — <title>** | <what it does, in one line> | <where to look>`

*******************************************************************************
**OPEN: 2**

- [ ] **037 — a `ptr` producer leaks at exit 0, and the whole handle machinery cannot see it** | where a group consumes a `ptr`, nothing demands a mark on the call that hands one back, so the program leaks with zero diagnostics on every platform | `selfhost/check/acquiring.hero`'s `consumed_types` · `selfhost/handles.hero`'s `is_handle` · `runtime/parts/alloc.c`

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

    **What is owed, and why it is not done here.** Demanding a mark on a `ptr`
    producer widens what a diagnostic refuses, which is a diagnostic CLASS, so
    CLAUDE.md § 4 sends it to the panel. The question that sitting has to answer
    is not *should a `ptr` be marked* but **which `ptr`**: every C function
    returning a pointer would be caught by the naive rule, and this compiler's
    own source calls several. `struct addrinfo` is the case to brief it on,
    because the ffi seat measured that `getaddrinfo` cannot be written any other
    way: `netdb.h` on this Mac has no `typedef` for `addrinfo`, so the handle
    form does not compile and `ptr` is the only spelling that works.

- [ ] **038 — a mark is one obligation and a fixed array of handles can be released one element at a time** | `Slot[4]` behind one `acquires` gives one increment, so a program releasing all four aborts at 134, and the runtime message names no cause that fits | `selfhost/check/reaches.hero` · `runtime/parts/alloc.c`'s fourth exit check

    **Origin:** panel 149 adopted *one mark is one obligation on the whole
    value*, which is what C ships, and left this shape open rather than guessing.
    Predicted by that sitting's **historian** before it was run, on its own
    number, and confirmed twice: once on receipt of the report and once by the
    coordinator on 2026-09-15 against the repaired compiler.

    **The reproducer.** `record Four` holding `a: Slot[4]`, producer marked
    `four_make(n: i64) -> Four acquires slot_close`, the program releasing each
    of the four elements with its own `slot_close`: exit **134**, *"3 more
    handle(s) given back than were taken"*.

    **The message names two causes and neither is this one.** Since 2026-09-15
    it reads *a double release* or *a lying `borrows`*, and this is a third
    thing: a whole-value mark met by per-element releases. It was not covered by
    the old three-cause wording either — that one's third cause was an UNMARKED
    producer, and this producer is marked — so nothing was lost when the clause
    was struck, and nothing has ever named this.

    **Why no mark can carry the number, which is the part that makes it a
    question rather than a bug.** The count is C's choice at run time, and both
    real headers say so in their own text: `jpeglib.h` has
    `JQUANT_TBL *quant_tbl_ptrs[NUM_QUANT_TBLS];` commented *"or NULL if not
    defined"*, and `<net/route.h>` has `struct sockaddr *rti_info[RTAX_MAX]`
    where which slots are filled is a sibling bitmask field. Panel 149's
    compiler-engineer measured the same from two headers filling four slots and
    two, against byte-identical Heroes programs.

    **The route nobody listed, and it is why this is filed rather than resolved.**
    All three candidates that sitting weighed act on the PRODUCER's declaration,
    and the two programs that disagree differ on the CONSUMER's: the ffi seat's
    shim releases the COMPOSITE with one call and exits 0, the compiler seat's
    releases the ELEMENTS and aborts. Its completeness critic named the choice —
    refuse the element release, or let the mark say WHOLE versus PER-ELEMENT —
    and no seat was briefed on it.

    **What holds meanwhile, written down rather than assumed**: the program does
    not corrupt anything. The counter catches it at exit and the abort is loud.
    What is wrong is the message, which offers two repairs and neither applies.

*******************************************************************************

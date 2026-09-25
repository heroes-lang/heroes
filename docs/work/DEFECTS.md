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
**OPEN: 7**

- [ ] **077 — a handle given back after C has handed its address out again is `check` 0 and `run` 0, and the release lands on the new handle** | the live set keys on the address, so a stale handle that equals a live one is accepted as the live one, and the one correct release that follows is the call that aborts | `runtime/parts/alloc.c:436` · `spec § 13`

    **Origin:** panel 175's spec-warden, 2026-09-23 (its F1), reproduced by the
    coordinator the same day before filing.

    **The reproducer**, over `r1.h`'s `box_open`/`box_close` (both `noinline`,
    `malloc` and `free`):

        extern "r1.h"
            record Box tag box
            function box_open() -> Box acquires box_close
            function box_close(b: Box consumes)

        function main()
            b = box_open()
            box_close(b: b)
            c = box_open()
            print("same address: ", b == c)
            box_close(b: b)
            print("after the second give-back of b")

    `check` 0; `run` 0, zero bytes of stderr, three of three on Darwin arm64,
    printing `same address: true`. The seat measured five of five at `-O0` and
    `-O2`, and with `box_close(b: c)` added at the end the program aborts at that
    call, the one correct release in it (134, 396 bytes, three of three).

    **That reproducer depends on the allocator, and this one does not** —
    panel 175's completeness critic, 2026-09-23: two binaries from
    byte-identical C, differing only in their UUID and code signature, read 0
    and 134, because the signature decides whether libmalloc hands the freed
    block straight back. The critic's `u1_static.hero`, over a C `f_open` that
    returns one static cell every time and an `f_close` that frees nothing,
    is the one to repair against: `f_open`, `f_close(x: a)`, `b = f_open()`,
    `print(a == b)`, `f_close(x: a)` reads `true` and exits 0 with zero bytes,
    three of three, rerun by the coordinator.

    **Why it is a defect.** Spec § 13 says *the live handles are a set, so
    giving one back twice aborts on its own*, and here a handle given back twice
    does not. `hero_handle_consumed` compares the address (`alloc.c:436`) and the
    comment at `:411-414` already says the set holds one entry per address. What
    it does not say is that the second release of a stale handle then frees a
    live one in silence.

    **Linux, run by the coordinator:** `run` 0, three of three, `same address:
    true`, on arm64 and x86-64 alike. **Under `--sanitize` it is caught**, 134,
    *given back that were never taken*, on both, and that is the sanitizer
    HIDING the defect rather than finding it: ASan's quarantine does not hand a
    freed address straight back, so `c` gets a different one and the stale `b`
    is a stray. **Unrun:** Windows.

    **Added 2026-09-24, with defect 088's correction:** the stale `b` is the
    copy design.md Part 8 wart 20 says outlives a consuming call, and what this
    defect adds to the wart's class is the reuse — the copy is not merely
    dangling, it names a live handle, so no check on the address alone can tell
    them apart. The spec sentence `a747e5a2` landed, *unless C has since reused
    its address*, states that limit; §1.12 is why it cannot be the answer.

- [ ] **088 — a handle handed to a call after the call that ended its life is `check` 0 and `run` 0, and C is handed freed memory** | § 13 says a `consumes` call ends the value's life, and nothing reads the value as dead afterwards: the checker does not, and the live set is asked only by a consuming call | `spec § 13` · `selfhost/emit/handle_traffic.hero` · `runtime/parts/alloc.c`

    **Origin:** panel 176's completeness critic, 2026-09-23 (its § 7), which
    searched this file for `use.after`, `survive` and `after consum` and found
    nothing. Reproduced by the coordinator on 2026-09-24 before filing, from the
    critic's own files copied out of its directory.

    **The reproducer**, over a three-function cJSON in miniature (`CreateObject`,
    `AddItemToObject` that links `item` under `object`, `Delete` that frees a
    node and its children), declared with `acquires cJSON_Delete` on the
    creator, `item: Json consumes` on the adder and `item: Json consumes` on
    `Delete`:

        b = cJSON_CreateObject()
        cJSON_Delete(item: b)
        c = cJSON_CreateObject()
        _ = cJSON_AddItemToObject(object: b, string: "c".cstr(), item: c)
        print("wrote into a deleted object")

    `check` 0; **run 0, three of three, on Darwin arm64, Linux x86-64 and Linux
    arm64**, printing its line after C wrote into freed memory; with
    `--sanitize`, `heap-use-after-free` on all three (134 on Darwin, 1 on
    Linux). Windows unrun: the box was off.

    **The shapes beside it, and why they are not this defect.** The same dead
    handle given to a CONSUMING parameter is stopped before C on all three
    legs, 134 with the runtime's *given back that were never taken* — a true
    line, since the address left the set at the first release. And through a
    helper, `finish(@b)` releasing it and the caller reusing `b` as `item`, the
    same 134. So the silence is exactly where the handle reaches a parameter
    that does not consume: no check reads it there.

    **Why it is a defect.** design.md §1.12: a Heroes program must not corrupt
    memory, and this one does at exit 0 with no C in it but the library's own.
    It belongs with defect 077, the other shape where the live set cannot tell
    a handle that is over from one that is not, which is why panel 177 takes
    both.

    **Corrected 2026-09-24, before panel 177 sat: this shape is inside a class
    design.md already states, and the filing missed it.** Part 8 wart 20 — *one
    copy of a value holding a `ptr` can free what every other copy holds, at
    exit 0* — says the half `consumes` does not reach is *a copy made BEFORE
    the call still holds the freed address*, and that the class is the affine
    handle, which Part 6's borrow-checker row refuses on COST since 2026-09-13.
    `b` above is that copy. The critic searched this file and the coordinator
    searched nothing else; `grep -n "wart 20\|affine" docs/design/design.md`
    finds it, and panel 153 had declined to file a shape of the same class on
    that ground and said so. **It stays filed, and this is why**: the wart
    admits the class *only while the cheapest guard is being built rather than
    argued about*, and the live set that panel 150 added since gives this
    shape a guard the wart could not name — the dead handle is not in the set
    when it reaches a parameter that does not consume. Whether that guard is
    the cheapest, and what it does to a `borrows` handle that was never in the
    set, is panel 177's question. The author can disagree and close it on the
    wart, as panel 153 put it.

- [ ] **096 — inside an `extern` group, a remark followed by a blank line before the next member makes `fmt` refuse the file** | the group printer never emits the blank line for a continuing member, so the remark becomes the next member's doc, the self-check sees a different tree and `fmt` exits 2 on a program `check` accepts | `selfhost/print/fmt.hero` (the group's member walk, the `continues` branch)

    **Origin:** the skeptic seat over defect 095's repair, 2026-09-25, attacking
    the repair at the shapes beside it; measured on the seed compiler of
    `0a8fd346` and on the repaired one alike, so it is older than both.

    **The reproducer.** A group whose first member is followed by a remark, a
    blank line, and `record Ob tag ob`:

        extern "x.h"
            function ob_put(o: Ob consumes)
            # a remark about what follows

            record Ob tag ob

    `check` exit 0; `fmt` exit 2, *`fmt` changed the TREE … a DIFFERENT
    PROGRAM*, and the file is not touched. With no line-broken signature
    anywhere, so it is not 095's shape: the blank line is what is lost.

    **Why it is a defect.** A correct program is refused by a tool that must
    be idempotent on every program the parser accepts (design.md §4.15); the
    exit-2 guard stops the silent form, which would attach the remark to the
    record as its doc. The repair keeps the blank line between a remark and
    the member it does not document, inside a group as outside one.

- [ ] **098 — two threads that acquire and release handles from one C allocator abort a correct program** | the live set records an end after C returns, so between C's release and the runtime's record the other thread can be handed the same address, the runtime overwrites the live entry with the new life, and the first thread's record then deletes the new one: the second thread's correct release is a stray | `runtime/parts/alloc.c` (`hero_handle_acquired` on a live entry with an end pending, `hero_handle_ended`)

    **Origin:** the skeptic seat over the landing of panel 177's items 1 and 2
    (the poison and the dead set), 2026-09-25, which measured it on the
    runtime before that landing too; reproduced by the coordinator the same day
    on the trunk's compiler at `9e17d471`, so it is older than the landing.
    Panel 177's completeness critic had listed threads as unmeasured for both
    of its routes.

    **The reproducer.** Two threads spawned with `hero_thread_spawn`, each
    200,000 times `n = sn_new()`, `sn_value(n: n)`, `sn_free(n: n)`, over ONE
    C pool with a free list under a `pthread_mutex_t`, which is what every
    real allocator is; `sn_new() -> S acquires sn_free`, `sn_free(n: S
    consumes)`. `check` 0; `run` **134, ten of ten, on Darwin arm64**, *1 C
    handle(s) given back that were never taken*; and ten of ten on Linux x86-64 and on Linux arm64, the same line. Windows unrun.

    **Why it is a defect.** A correct program is refused, and the class is
    every program that hands handles to C from more than one thread over one
    allocator, which is the ordinary case for a server. The runtime's own
    pending count (defect 084) already knows an end is in flight when the
    second thread's acquisition arrives: an acquisition of a live address with
    an end pending is C having released it inside that call and handed it out
    again, so it is one more reference, not a replacement, and the end in
    flight then leaves the new life standing. The repair makes the set say
    that, and pins it with the reproducer as a golden over the shared pool.

- [ ] **099 — a comment after the last item of a block leaves the block when `fmt` runs** | a `#` remark written at the block's indent after the last member of a group, the last statement of a body or the last field of a record is printed by the NEXT declaration's walk, at column 0, with a blank line added under it: exit 0, the tree the same, and the comment now sits at file level above the next function | `selfhost/print/fmt.hero` (`format_file`'s walk, `comments_before` at the next item's indent)

    **Origin:** the parser seat that repaired defect 096, 2026-09-25, measuring
    the shapes beside its reproducer; reproduced by the coordinator the same
    day on the trunk's compiler at `0029567d`, so it is older than that repair.
    Searched `docs/work/`, `docs/records/done/` and `docs/learn/` for "last
    statement", "end of the body", "after the last member", "after the last
    field", "trailing remark": not filed.

    **The reproducer**, three shapes, each `fmt` exit 0 and each output a
    fixpoint:

        function f() -> i64
            x = 1
            return x
            # a remark after the last statement of the body

        function main()
            print(f())

    comes back with the remark at column 0 under `return x`'s block, above
    `function main()`, a blank line under it; the same for a remark after the
    last member of an `extern` group and after the last field of a record.

    **Why it is a defect.** `fmt` moves a comment out of the block the author
    wrote it in, silently: the tree does not change, so the self-check that
    stops defect 095's and 096's shapes cannot see it, and a remark about the
    end of a body now reads as a remark about the function below it. Defect
    095's record calls the silent move the worse form of the two. The repair
    prints a comment indented inside a block, after the block's last item and
    before the dedent, at the block's own indent.

    **And without the blank line it is a refusal, measured 2026-09-25** by the
    skeptic seat over defect 096's repair and re-run by the coordinator on the
    trunk's compiler: the same remark written directly above the next
    declaration, no blank line between (a body's last statement then
    `function main()`, a group's last member, a record's last field inside a
    group), is `check` 0 and `fmt` **2**, *changed the TREE*, because the
    remark printed at the next declaration's column becomes its doc. One root,
    two outcomes: with the blank line the move is silent, without it the
    self-check catches it.

- [ ] **100 — inside an `extern` group, a comment at another column than the member below it, or directly under a second head of the same group, becomes that member's doc when `fmt` runs** | `fmt` re-indents a column-0 comment between two members to the members' indent, and merges two `extern` heads with the same header into one group while keeping a remark that sat directly under the second head, so in both the comment ends up at the member's column, directly above it, and the re-parse takes it as the member's doc: `check` 0, `fmt` exit 2 | `selfhost/print/fmt.hero` (the group walk, `continues`, `extern_head_once`)

    **Origin:** the skeptic seat over defect 096's repair, 2026-09-25, beside
    its reproducer; re-run by the coordinator the same day on the trunk's
    compiler at `5c3a6e39`, so older than that repair.

    **The reproducers.** A group `extern "x.h"` with `record Ob tag ob`, then
    a comment at column 0, then `    record Pool tag pool`: `fmt` 2. And two
    groups `extern "x.h"` one under the other, the second holding
    `    # remark under a second head` above `    record Pool tag pool`:
    `fmt` 2. `take_docs` in the parser takes a comment as a doc only at the
    member's own column and directly above it, which the source did not have
    and the output does.

    **Why it is a defect.** A correct program is refused by a tool that must
    be tree-preserving on every program the parser accepts (design.md §4.15).
    Beside it, the skeptic found one shape the repair of 096 in its first form
    broke: a doc above the second of two same-head groups, `fmt` 0 on the
    trunk, lost at the merge in the repair's first form; the repair of this
    defect and of 096 land together.

- [ ] **101 — `fmt` moves a comment silently at three more places: off a block-opening line, out of a bracket at a body's end, and away from a blank line it adds between match arms** | a trailing comment on the line that opens a block (`if x > 0  # note`) is printed on the block's first line; a comment trailing the last element of a bracketed value at the end of a body leaves the function and becomes a column-0 remark above the next declaration; and a comment between two `match` arms gains a blank line under it the source does not have. Each at `fmt` exit 0 with the tree the same, so the self-check cannot see it | `selfhost/print/fmt.hero` (the block openers, the bracketed-value printer, the match-arm walk) · `selfhost/print/page.hero` once defect 099's repair lands

    **Origin:** the skeptic seat over the repair of defects 096, 099 and 100,
    2026-09-25, beside its reproducers; reproduced by the coordinator the same
    day on the trunk's compiler at `3b40c60d`, so it is older than that
    repair.

    **The reproducers.** `if x > 0  # only the positive case` over a block:
    `fmt` 0, the comment printed as the block's first line. `return [` then
    `1,` then `2  # the last element` then `]` as a body's last statement:
    `fmt` 0, the elements printed one per line and the comment at column 0
    under the function, above `function main()`, a blank line under it. A
    `# the other values` between the two arms of `return match x`: `fmt` 0, a
    blank line added under the comment.

    **Why it is a defect.** The first two move a comment away from the code
    the author wrote it beside, silently, which defect 095's record calls the
    worse form of a formatter's mistake; the third breaks the rule defect
    096's repair wrote down, that a blank line is printed only where the
    source has one. None changes the tree, so no instrument sees any of them.
    The repair keeps a trailing comment on the line it trails, a comment
    inside a bracket inside the bracket, and prints a blank line between arms
    only where the source has one.

*******************************************************************************

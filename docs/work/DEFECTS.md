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

- [ ] **091 — writing one element of a fixed-array field through a cell is `check` 0 and dies at run time saying it is a compiler bug** | a store whose place ends in an index is always sent to the copy-on-write element writer, which serves `[T]` and fails for `T[N]`, and the emitter writes `hero_unreachable()` where the store should be | `selfhost/emit/inst.hero:133` · `selfhost/emit/container.hero:234` · `spec § 5`

    **Origin:** the coordinator of panel 178, 2026-09-24, measuring today's
    route for `sockaddr_un.sun_path` for that sitting's shared brief;
    number agreed with the session holding this list that night.

    **The reproducer** (`docs/panel/178-briefs/elem_min.hero` + `elem_min.h`):

        extern "elem_min.h"
            record Slot tag slot partial
                name: i8[4]

        function main()
            s: Slot @ Slot(name: [0, 0, 0, 0])
            s.name[1] @ 72
            print(s.name[1])

    `check` **0**, run **134**, `panic: entered unreachable code — this is a
    compiler bug, please report it`: three of three on Darwin arm64 and on
    Linux x86-64, and on Linux arm64 by `heroes run` and by the built binary.
    The emitted C (`--emit-c`) carries
    `hero_unreachable(); /* not an element write */` where the store belongs.

    **Why it is a defect, and why the repair is a lowering.** It is defect
    052's sibling: that was `s[0] @ 65` on a `str`, closed 2026-09-16 by a
    REFUSAL, because spec § 3 calls `str` immutable. Here the document has
    ruled the other way: spec § 5 says *`@` declares a mutable cell and
    re-binds it, or a field or element inside one*, and § 13 gives a group
    record a fixed array field. So the spec admits the write, the compiler has
    the bug (CLAUDE.md § 12), and the repair is to emit the store, index
    checked as the read already is.

    **What it cost before it was found.** It is the only route today for the
    bytes a program writes into a fixed field, `sun_path` being the case that
    found it: `sunpath_*_ascii.hero` dies the same way on all three legs, so
    such a field has no working route except a literal that spells every byte
    as a number. Panel 178 weighs a form for it; the repair is owed whatever
    that sitting decides.

- [ ] **092 — a whole group record lent to a `void *` parameter is handed to C with no bound on the count C is told, and C writes past it** | a record behind `@` against a `void *` passes every pointee check, so `read(fd, buf: @h, n: 4096)` into a 48-byte record is `check` 0 and a stack overflow at run time | `selfhost/emit/extern_probe.hero` · `selfhost/cli/pointee.hero` · `tests/golden/fixedbugs/ffi-pointee-void.hero` · `spec § 13`

    **Origin:** panel 178's ffi-pragmatist, 2026-09-24 (its § 8 F1), found
    while binding the census structs; reproduced by the coordinator the same
    day on all three legs before filing.

    **The reproducer** (`docs/panel/178-reports/ffi-pragmatist-work/rec_overwrite.hero`):

        extern "netdb.h"
            record Hints tag addrinfo partial
                ai_family: i32
                ai_socktype: i32

        extern "unistd.h"
            function read(fd: i32, @buf: Hints, n: u64) -> i64

        function main()
            h: Hints @ Hints(ai_family: 0, ai_socktype: 1)
            n = read(fd: 0, buf: @h, n: 4096)
            print(n)
            print(h.ai_family)

    With 4096 bytes on standard input: `check` **0**; run **138** on Darwin
    arm64, **135** (Bus error) on Linux arm64, **139** (segmentation fault) on
    Linux x86-64, three of three on each, after printing `4096`.
    `--sanitize`: `AddressSanitizer: stack-buffer-overflow`, *WRITE of size
    4096*, on all three. The seat measured that the same shape through `write`
    hands C's reader the stack beyond the record (`rec_overread.hero`).

    **Why it is a defect.** design.md §1.12: a Heroes program must not
    segfault and must not corrupt memory, and this one does both at `check` 0.
    It is defect 010's sibling on another shape: `@value: i32` against
    `void *` is refused (`tests/golden/fixedbugs/ffi-pointee-void.hero`), and a
    whole record in the same position is not. The count that bounds the write
    is a C argument the program supplies, which is the shape `counted_by`
    relates for a lent field; nothing relates it for a record.

- [ ] **093 — a `str` holding a zero byte reaches C through `.cstr()` cut at that byte, at exit 0** | `read_file` makes a `str` from any bytes, a zero among them, and `.cstr()` hands C the same bytes zero-copy, so C reads a shorter string than the program holds and nothing says so | `selfhost/emit/` `.cstr()` · `runtime/` `hero_str_cstr` · `spec § 13` · `docs/records/log/2026-08-04-0001-escape-sequences-five-split-by-context-go-s-rule.md`

    **Origin:** panel 178's ffi-pragmatist, 2026-09-24 (its § 8 F3), found
    while pricing route T's refusal of an interior zero; reproduced by the
    coordinator the same day on all three legs before filing.

    **The reproducer** (`docs/panel/178-reports/ffi-pragmatist-work/nul.hero`),
    with `p/nul.bin` holding the five bytes `a b \0 c d`:

        extern "string.h"
            function strlen(s: cstr lent) -> u64

        function main()
            s = read_file("p/nul.bin").must()
            print(s.len())
            print(strlen(s.cstr()))

    `check` **0**, run **0**, prints `5` then `2`: three of three on Darwin
    arm64, and on Linux arm64 and x86-64.

    **Why it is a defect.** A wrong answer at exit 0. The language already
    ruled that an interior zero must not reach C: panel 008 froze the escape
    set with `\0` out because *interior NUL voids §4.20's free `.cstr()`* (the
    log entry above). The escape was closed and `read_file` is a second door to
    the same byte, which nobody closed. Whether the repair refuses at `.cstr()`
    (a `T?`, or an abort) or at `read_file` is the repair's question.

- [ ] **094 — a group `constant` whose header value is a struct initialiser passes `check` and stops the build with `internal error`** | the emitter writes `return PT_INIT;` and a file-scope `__typeof__(PT_INIT)` probe, and `{1, 2}` is neither an expression nor a type, so clang fails and the compiler reports itself broken | `selfhost/emit/` constant emission · `.claude/rules/c-boundary.md` · `spec § 13`

    **Origin:** panel 178's compiler-engineer, 2026-09-24 (its *Found while
    measuring* 1), looking for a route by which C itself says which value is
    valid; reproduced by the coordinator the same day on all three legs before
    filing.

    **The reproducer** (`docs/panel/178-reports/compiler-engineer-work/w/cinit/`):
    `cinit.h` is `struct pt { int x; int y; };` and `#define PT_INIT {1, 2}`;

        extern "cinit.h"
            record Pt tag pt
                x: i32
                y: i32
            constant PT_INIT: Pt

        function main()
            p = PT_INIT
            print(p.y)

    `check` **0**, `run` **2**, `internal error: compiling the generated C
    failed`, on Darwin arm64, Linux arm64 and Linux x86-64.

    **Why it is a defect.** CLAUDE.md § 7's exception: the compiler blames
    itself for a binding it accepted. And it is the one route to *which value of
    a struct is valid* where C says it rather than the binding's author: the
    same program over Darwin's `PTHREAD_MUTEX_INITIALIZER` fails the same way
    (the seat's `w/mutex_init.hero`). A compound literal, `(struct pt)PT_INIT`,
    is valid C.

- [ ] **095 — `fmt` refuses a group member whose parameters are written one per line with a comment between two of them** | the signature printer joins the parameters onto one line and has no place for the comment, which is expelled after the declaration and becomes the next member's doc, so the self-check sees a different tree and `fmt` exits 2 on a program `check` accepts | `selfhost/print/fmt.hero` (`signature`) · `selfhost/parse/members.hero`

    **Origin:** the landing review of M-agreed-retention step 11, its surface
    finder, 2026-09-24; reproduced by the parser seat against the seed compiler
    of the same day (`fmt` exit 2 before the lane and after it), so it predates
    the landing.

    **The reproducer.** An `extern` member written

        function ob_get(
            o: Ob,
            # a comment between
            n: i64
        ) -> Ob retains ob_put

    followed by another member: `check` exit 0, `fmt` exit 2 with *`fmt`
    changed the TREE of … the output parses and is a fixpoint, and it is a
    DIFFERENT PROGRAM*, and the file is not touched. With no member following,
    the comment lands after the group as a file-level comment and `fmt` exits 0
    having moved it. Measured with `parse --dump-ast`: the one line that
    differs is `doc # a comment between`, attached to the following member.

    **Why it is a defect.** A correct program is refused by a tool that must
    be idempotent on every program the parser accepts (design.md §4.15, CLAUDE.md
    §9); the exit-2 guard is what stops the silent form, which is worse. The
    repair is either a place for a comment inside a printed parameter list
    (the printer keeps the author's line breaks when a comment sits between
    parameters) or a refusal at parse of a comment inside a signature, and
    the choice is a formatter-surface question for the sitting that owns
    `fmt`'s canonical form.

*******************************************************************************

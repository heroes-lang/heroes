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
**OPEN: 5**

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

- [ ] **079 — a reference-counted C handle aborts a correct program whichever way its extra reference is declared** | the live set holds one life per address, so a second reference to one object has nowhere to live, and its correct release is reported as a release of something never taken | `runtime/parts/alloc.c:411` · `spec § 13`

    **Origin:** panel 175's ffi-pragmatist, 2026-09-23, attacking route A at the
    shape beside it; reproduced by the coordinator the same day before filing.

    **The reproducer**, the shape of `CFRetain`/`CFRelease`,
    `g_object_ref`/`g_object_unref`, `X509_up_ref`/`X509_free`: a C object whose
    `obj_unref` frees it when its count reaches zero.

        extern "rc.h"
            record Obj tag obj
            function obj_new() -> Obj acquires obj_unref
            function obj_ref(o: Obj) -> Obj acquires obj_unref
            function obj_unref(o: Obj consumes)

        function main()
            a = obj_new()
            b = obj_ref(o: a)
            obj_unref(o: a)
            obj_unref(o: b)
            print("both references given back")

    `check` 0; `run` **134**, 396 bytes, *1 C handle(s) given back that were
    never taken*, three of three on Darwin arm64, and the same under
    `--sanitize`. With `obj_ref(o: Obj) -> Obj borrows` it prints its line, C
    frees the object at count zero, and it still exits 134 with *2 C handle(s)
    given back that were never taken*. **There is no spelling that runs.**

    **Why it is a defect.** The program is correct C and correct Heroes; the
    runtime aborts it. The set's own comment at `alloc.c:411-414` says a second
    acquisition of a live address *is lost here*, which is right for C handing
    an address out again after a leak and wrong for a second reference. The
    seat's count per slot (`<scratchpad>/175-ffi-pragmatist/runtimeA2/`) runs
    the program at 0 and still stops `handle.hero`'s double release, measured by
    the seat and not yet by the coordinator.

    **Unrun by the coordinator:** Linux and Windows.

- [ ] **084 — one handle given to two consuming parameters of one call aborts a correct program, and the message calls it a double release** | the live set takes the handle back once per marked parameter, so `SSL_set_bio(s, b, b)`, OpenSSL's socket-BIO idiom, is refused although its C is correct | `selfhost/emit/handle_traffic.hero:101` · `runtime/parts/alloc.c:422`

    **Origin:** panel 176's ffi-pragmatist, 2026-09-23 (its § 5 item 1);
    reproduced by the coordinator the same day before filing.

    **The reproducer**, against Homebrew's OpenSSL 3 (`--include
    /opt/homebrew/opt/openssl@3/include --library /opt/homebrew/opt/openssl@3/lib`):
    `SSL_new`, then `b = BIO_new(type: BIO_s_mem())`, then
    `SSL_set_bio(s: s, rbio: b, wbio: b)` with both BIO parameters `consumes`,
    then `SSL_free(ssl: s)`. `build` 0; **run 134, three of three**, *1 C
    handle(s) given back that were never taken*. OpenSSL's own page says *"If
    the rbio and wbio parameters are the same … then one reference is
    consumed"*, and the seat measured the same C clean under ASan with 0 leaks.

    **Why it is a defect.** A correct program is refused and the message names a
    double release that did not happen: defect 079's class, over the most common
    OpenSSL call there is. The seat measured every route panel 176 weighs
    leaving it at 134, so the repair is owed whatever vocabulary lands.

- [ ] **085 — `unread_releaser` answers 1 for a module checked alone and 0 for the same module checked inside its program** | the rule's message says *no `extern` of this module declares it* and its lookup resolves over the whole program, so the verdict depends on which file is handed to `check` | `selfhost/check/acquiring.hero:269`

    **Origin:** panel 176's ffi-pragmatist, 2026-09-23 (its § 5 item 4);
    reproduced by the coordinator the same day before filing.

    **The reproducer.** `bio.hero` declares `h_open() -> H acquires h_close2`
    and only `main.hero` declares `h_close2`; `main.hero` uses `bio`.
    `heroes check bio.hero`: **exit 1**, `error[unread_releaser]`.
    `heroes check main.hero`: **exit 0**, and the seat measured `build` and `run`
    at 0 too.

    **Why it is a defect.** One program, two verdicts, and the message states
    the rule the lookup does not apply. Which of the two is right is the
    repair's question: the per-module reading is what the message and
    `check/acquiring.hero`'s own note say, and the program-wide one is what lets
    a libcrypto BIO mark name libssl's calls today (the seat's § 2c).

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

*******************************************************************************

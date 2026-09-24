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
**OPEN: 14**

- [ ] **075 — `acquires` names the call that ends a handle's life, and a program that ends it with another is `check` 0 and `run` 0** | the named releaser is read for existence and never at the call that gives the handle back, and the live set keeps an address and nothing else | `selfhost/check/acquiring.hero` · `runtime/heroes_runtime.h:205` · `spec § 13`

    **Origin:** 2026-09-23, M-agreed-retention step 1, measuring the milestone's
    second item at the shapes beside it. The contradiction it names across two
    modules turned out to be admitted inside ONE, which is where the defect is.

    **The reproducer**, against the platform's real `stdio.h`:

        extern "stdio.h"
            record File tag __sFILE
            function popen(command: cstr lent, mode: cstr lent) -> File acquires pclose
            function pclose(stream: File consumes) -> i32
            function fclose(stream: File consumes) -> i32

        function main()
            f = popen(command: "true".cstr(), mode: "r".cstr())
            rc = fclose(stream: f)
            print("fclose on a popen stream: ", rc)

    `check` 0, and `run` 0 five times out of five on **all four platforms**:
    Darwin arm64 as written, both Linux legs with `tag _IO_FILE`, Windows x86-64
    with `_popen`, `_pclose` and `tag _iobuf`. Under `--sanitize` on Darwin it
    builds, runs at 0 and writes zero bytes. What `fclose` on a `popen` stream
    does inside each C library is UNRUN here; what is measured is that nothing
    in Heroes objected, on any platform. The same shape through an
    `@out` parameter (`h_open_out(@out: H acquires h_close)`, then
    `h_close2(x: h)`) and across two modules is `check` 0 and `run` 0 as well.

    **The cause, read rather than inferred.** `unread_releaser` asks whether the
    name after `acquires` is an `extern` of this module taking the handle
    `consumes`, and nothing asks it again. `hero_handle_acquired` and
    `hero_handle_consumed` take `const void *` and nothing else, so the set
    balances whichever releaser runs.

    **What is owed.** Spec § 13 says the call *names the one that ends it, which
    the program owes it*, and panel 148 adopted the named form over the bare word
    because the bare word *leaves the mismatched-deallocator class open* (its
    `What conservative would have been`). Measured today, the named form leaves
    it open too.

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

- [ ] **078 — `owned` on an out-parameter the header spells `const char **` stops the build with `internal error`, exit 2, instead of an `ffi_` diagnostic** | the author's binding disagrees with the header on one qualifier, and the compiler reports itself as broken | `selfhost/emit/` · `.claude/rules/c-boundary.md`

    **Origin:** panel 175's spec-warden, 2026-09-23 (its F4), reproduced by the
    coordinator the same day before filing.

    **The reproducer**, over `r1.h`'s `fill_out(const char **out)` and
    `free_out(const char *p)`:

        extern "r1.h"
            function fill_out(@out: cstr owned free_out)
            function free_out(p: cstr)

        function main()
            s: str? @ fail(code: "none", msg: "nothing yet")
            fill_out(out: @s)
            print("after")

    `check` 0; `build -O0` exit **2**, `internal error: compiling the generated
    C failed`, and inside it clang's own sentence, *passing 'char **' to
    parameter of type 'const char **' discards qualifiers in nested pointer
    types*, pointing at `hero_ffi_probe_h_s4cstrowned_fill_out`. The same cell
    without `owned` builds (panel 172's `b_out`).

    **Why it is a defect.** `.claude/rules/c-boundary.md` names the one class of
    clang failure that is the author's and not the compiler's, the author's own
    `extern`, and this is that class reported as the other. design.md §4.17 asks
    for a diagnostic that says what to change without opening another file.

    **Linux, run by the coordinator:** the same `internal error`, exit 2, on
    arm64 and x86-64. **Unrun:** Windows; whether a `const char **` cell can be
    `owned` at all, which is the question the diagnostic has to answer.

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

- [ ] **080 — the crash handler's lease line names the Heroes function that called the caller, not the one that called C** | the frame walk skips the direct caller when the C function that died was a noreturn call in an optimised library, so the line says `in e.main` for a death inside `e.run` | `runtime/parts/os.c:249` · `runtime/parts/stack.c`

    **Origin:** panel 175's ffi-pragmatist, 2026-09-23, prototyping route E;
    reproduced by the coordinator the same day before filing.

    **The reproducer.** A shared C library built `-O2 -fomit-frame-pointer`
    whose `lib_assert(x)` fails `assert(x == 42)`, called with a lease live from
    a Heroes function `run` that `main` calls:

        function run(what: str)
            if what == "assert_lease"
                y = "payload"
                d: cstr @ y.lease()
                lib_assert(x: 7)
                end_lease(@d)

    `run` 134, three of three, and the line reads *the process died with 1
    lease(s) still live, **in e.main***: on Darwin arm64 and on Linux arm64. The
    lease and the call are in `e.run`. On Linux x86-64 the walk finds no frame
    and the line names none, which is not false.

    **Why it is a defect.** Panel 173 R1: no path prints a sentence measured
    false. *In* claims the frame that called C, and a noreturn call does not save
    the return address the walk reads. The seat measured *under* true in every
    case it ran, because the named function IS on the stack.

- [ ] **081 — on Linux x86-64 a C trap with a lease live dies at 132 with nothing on stderr** | the lease handler installs SIGTRAP and SIGABRT, and `__builtin_trap` on x86-64 is `ud2`, which is SIGILL | `runtime/parts/os.c:289`

    **Origin:** panel 175's ffi-pragmatist, 2026-09-23; reproduced by the
    coordinator the same day before filing.

    **The reproducer** is 080's library and program, case `trap_lease`: a lease
    live, then `lib_trap()`, which is `__builtin_trap()`. **Linux x86-64: 132,
    zero bytes, three of three.** Linux arm64, the same program: 133 and the
    runtime's lease line, 264 bytes. Darwin, measured by the seat: 133 and the
    line.

    **Why it is a defect.** Defect 070 closed on the report that names the live
    leases when C kills the process; on one of the four platforms, for one of
    the ways C kills it, the report is not there. The seat measured the line
    printing with SIGILL added, 265 bytes. Darwin x86-64 is UNRUN, since there is
    no Intel Mac here, and libmalloc's own trap would be `ud2` there too.

- [ ] **082 — the lease line says the process died, and then the handler it found lets the process live and exit 0** | the line is written before the previous disposition is called, so a C library that recovers from its own signal leaves a false sentence above a successful run | `runtime/parts/os.c:249` · `:264`

    **Origin:** panel 175's compiler-engineer, 2026-09-23, prototyping route E,
    who reported it and did not file it; reproduced by the coordinator the same
    day before filing.

    **The reproducer.** A header whose constructor installs, before `main`, a
    SIGABRT handler that `siglongjmp`s back, and a function that `abort()`s
    inside a `sigsetjmp`:

        extern "e.h"
            function e_try_abort() -> i32

        function main()
            x = "payload"
            c: cstr @ x.lease()
            print("recovered: ", e_try_abort())
            end_lease(@c)

    With `E_CTOR=1`: **exit 0**, stdout `recovered: 1`, and 277 bytes on stderr
    beginning *panic: the process died with 1 lease(s) still live*, three of
    three on Darwin arm64, Linux arm64 and Linux x86-64.

    **Why it is a defect.** Panel 173 R1, *no path prints a sentence measured
    false*, and panel 173 R2 kept the chaining that makes this path reachable.
    The seat measured its E2 ordering — say the line only after the previous
    disposition has been called and has not returned control — at 0 bytes here
    and unchanged everywhere else.

- [ ] **083 — an `@` cell holding a pointer is accepted against a header parameter that takes `void *`, and C is handed the address of the program's own cell** | C converts `void **` to `void *` in silence, and the width check an `@` cell gets exists for numbers only | `selfhost/emit/extern_probe.hero:172` · `selfhost/cli/pointee.hero`

    **Origin:** panel 176's llm-ergonomist, 2026-09-23, reading nothing but the
    specification: *"mark the parameter `@`" reads as the extern's own
    parameter; on a `void *` handle that becomes `void **`, which C converts
    silently*. Reproduced by the coordinator the same day before filing.

    **The reproducers**, over `void eat(void *p)` that prints what it got and
    frees it, and `void wipe(void *p)` that writes eight zero bytes through it:

        extern "v.h"
            record Mem tag void
            function make() -> Mem acquires eat
            function eat(@p: Mem borrows)

        function main()
            m: Mem @ make()
            eat(@m)

    `check` 0, `build` 0; C prints `made 0x1056b9d70` then `eat got
    0x16af42478`, a stack address, and frees it: 134, three of three, Darwin
    arm64. The same over `record Box tag box` is `error[ffi_parameter_type]` at
    exit 1, because `box **` against `box *` is a type clang refuses. And
    `wipe(@p: ptr)` against `wipe(void *)`: `check` 0, `build` 0, **run 0**, C
    handed the cell and writing into it; `wipe(@n: i64)` against the same header
    is refused, `ffi_parameter_type`.

    **The shape it came from.** The checker asks `@p: Mem consumes` for a
    producer mark (`unmarked_handle_producer`, whose note offers `borrows`), and
    the grammar allows one of `consumes`, `acquires`, `borrows`, so following the
    note writes the shape above.

    **Why it is a defect.** design.md §4.19: a wrong binding is a compile error.
    The probe casts an opaque or numeric `@` argument to `void *` on purpose and
    says what the cast gives up is checked in `cli/pointee.hero` by width and
    sign, which exists for numeric cells; an opaque cell, a `ptr` or a `tag void`
    handle, has nothing that checks the header takes a pointer to a pointer.
    **Unrun:** Linux and Windows; a C function writing more than the cell's eight
    bytes, which would be stack corruption at exit 0.

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

- [ ] **086 — a handle given back twice before the program has acquired any dies at 133 with nothing on stderr** | while the live set is still empty, `hero_handle_consumed` counts the stray and returns, so the double release reaches C, where after one acquisition the same program is stopped before C with the runtime's line | `runtime/parts/alloc.c:428`

    **Origin:** panel 176's compiler-engineer, 2026-09-23 (its § 9); reproduced
    by the coordinator the same day before filing.

    **The reproducer.** A producer marked `borrows`, so nothing is ever
    acquired, and its handle given to a consuming `g_close` twice:
    `a = g_open()`, `g_close(x: a)`, `g_close(x: a)`. **133, zero bytes, three
    of three**, Darwin arm64. The same double release after one acquisition has
    allocated the set (`cap1.hero`): **134, 396 bytes, three of three**, before C.

    **Why it is a defect.** Defect 071 moved the stray report before the C call
    so that a double release is stopped before it happens; that promise now
    depends on the program's history. The seat measured the one-line repair —
    report instead of return at `alloc.c:432` — at 134 and 396 bytes for both.

- [ ] **087 — `heroes grammar` says the language has six contextual words, and the parser reads eleven** | the sentence lists `as`, `link`, `package`, `tag`, `partial`, `owned`, and `parse/members.hero` and `parse/tails.hero` also read `acquires`, `borrows`, `consumes`, `counted_by` and `lent` | `selfhost/cli/grammar.hero` · `selfhost/parse/members.hero:115`

    **Origin:** panel 176's compiler-engineer, 2026-09-23 (its § 9); reproduced
    by the coordinator the same day: `./heroes grammar` prints *"the six
    contextual words — `as`, `link`, `package`, `tag`, `partial`, `owned` —
    which are ordinary identifiers everywhere except the one position each is
    read in"*, and `grep` over the two parser files finds the other five read
    as markers.

    **Why it is a defect.** A tool that prints the language's own grammar states
    a count that is false by five, and panel 176's resolution would make it
    false by seven. A literal list of the words the parser owns is a premise
    that expires in silence; the seat priced deriving it from one table at
    about twelve lines.

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

- [ ] **089 — a C library that ignores SIGABRT and then raises it is killed by the runtime, under a false lease line** | panel 173's lease handler takes SIGTRAP, SIGABRT and SIGILL whatever disposition it finds, so a signal the program set to be ignored reaches the handler, which speaks and re-raises it at its default | `runtime/parts/os.c` (`hero_lease_crash_install`)

    **Origin:** the coordinator, 2026-09-23, at panel 175's landing, attacking
    the Windows arm being written at the shape beside the recovering handler of
    defect 082: a handler that recovers was covered, a disposition that ignores
    was not.

    **The reproducer.** A C constructor sets SIGABRT and SIGILL to `SIG_IGN`; a
    C function raises both and returns 1; the program holds a lease across the
    call and prints what it returned. The right answer is `ignored: 1` and exit
    0. **On the trunk at `a747e5a2`: 134, three of three, on Darwin arm64, Linux
    x86-64 and Linux arm64**, with *the process died with 1 lease(s) still
    live* (300 bytes on Darwin and Linux arm64, 254 on Linux x86-64). Windows
    unrun on the trunk, where no SIGABRT handler is installed, so the shape is a
    question there rather than a premise.

    **SIGTRAP is not in the reproducer, and why.** Under Rosetta, the Linux
    x86-64 leg on this Mac, a C program with no Heroes in it that ignores
    SIGTRAP and raises it dies at 133, where the same program exits 0 on Linux
    arm64 (measured 2026-09-24); SIGABRT and SIGILL are honoured there. A case
    raising SIGTRAP would be red on that leg for the emulator's reason.

    **Why it is a defect.** The handler's own comment says it never changes what
    the process does, and here it turns a program that goes on into a death, and
    names a cause for it. The repair is in route E's lane: a signal found
    ignored is put back and not taken, on the POSIX arm and on Windows' `signal`.

*******************************************************************************

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

*******************************************************************************

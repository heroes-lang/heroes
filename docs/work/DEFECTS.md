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
**OPEN: 2**

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

- [ ] **076 — a runtime panic blames a fabricated `str` for a heap a C function corrupted** | the magic check on a string block names ONE cause, and on a C double free that cause is false while the sentence reads as certain | `runtime/parts/str.c:64` · `docs/panel/173-the-runtime-may-say-what-it-saw-and-not-why.md`

    **Origin:** 2026-09-23, M-agreed-retention step 1, measuring the milestone's
    first item at the shape beside panel 172's `b_out`: a result mark that says
    the program frees what C has already freed.

    **The reproducer.** `fill` writes a `strdup` into the cell and frees it
    before returning, so the `owned free` below is a false claim about C:

        extern "stdlib.h"
            function free(p: ptr)

        extern "r1.h"
            function fill(@out: cstr owned free)

        function main()
            s: str? @ fail(code: "none", msg: "nothing yet")
            fill(out: @s)
            print(s.default("<nothing>"))

    `check` 0. Run, `-O0` and `-O2`, the C side `noinline` so that clang cannot
    delete the allocation: **Darwin arm64** 8 of 10 exit 134 printing
    `panic: not a Heroes string block — a str was fabricated from a foreign
    pointer; use hero_str_from_bytes`, 2 of 10 exit 133 with zero bytes; **Linux
    x86-64** 5 of 5 the same sentence; **Linux arm64** 5 of 5 glibc's own
    `free(): double free detected in tcache 2`; **Windows x86-64** 5 of 5
    `0xC0000374`, STATUS_HEAP_CORRUPTION, zero bytes. Under `--sanitize` on
    Darwin the truth is `heap-use-after-free`.

    **Why it is a defect and not a limit of the FFI.** The false `owned` is the
    program's; the false SENTENCE is the runtime's. Nothing was fabricated: the
    check saw a string block whose header was not a string block's, and a C
    function writing through a pointer it had already freed is a second way to
    get there. Panel 173 held the signal handler to *no path prints a sentence
    this sitting measured false*, and `hero_held_release`'s own comment in the
    same file, `str.c:447`, records the rule this breaks: *freed memory owes
    nobody its contents*. `hero_str_from_bytes` is also a runtime function no Heroes
    program can call, so the advice sends the reader nowhere (design.md §4.17).

    **The shape beside it, UNRUN.** `str.c:462`'s held-block check says *this is
    a compiler bug* on a bad magic, which is the same claim of one cause; no
    probe corrupting a lease header has been written.

*******************************************************************************

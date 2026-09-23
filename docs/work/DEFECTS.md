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
**OPEN: 1**

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

*******************************************************************************

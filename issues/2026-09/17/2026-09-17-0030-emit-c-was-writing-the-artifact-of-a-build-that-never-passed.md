- [x] **049 — `--emit-c` writes the pre-probe tag spelling, and the obvious repair makes the artifact platform-dependent** | a program binding `record <Name> tag <name>` got C that clang refuses, and the probe that fixed it made the SAME program emit two different files on two machines | closed 2026-09-17, M-check-completeness, inside panel 157 R1 and without a new sitting

    **Origin:** the tag half of defect 048, withdrawn 2026-09-16 within the hour
    of landing, by CI's Windows leg.

    **The entry said the choice belonged to a sitting, between two evils**: an
    artifact that is deterministic and does not compile, or programs judged by a
    predicate instead of by bytes (panel 157 R4, adopted and unbuilt, filed at
    M-package-manager). **The measurement found a third thing, and it is not a
    choice.** `heroes build` on a program whose header is not on this machine
    exits **1**, `error[ffi_missing_header]`. `heroes build --emit-c` on the SAME
    program exited **0** and wrote 10468 bytes (measured 2026-09-17 on a
    fabricated `nosuchheader_160.h`). So `--emit-c` was writing the artifact of
    a build that did NOT pass — before any probe, and against the rule panel 157
    R1 gave it. The withdrawn probe did not open that hole. It fell through it:
    on Windows, no `netdb.h`, the probe *learned nothing* and emitted the bare
    word at exit 0 where `build` on that machine says `ffi_missing_header` at
    exit 1.

    **The cause, in one line.** `cli/produce.hero` called `artifact.write_c`
    at `:194`, BEFORE the rounds at `:229-247` that learn a tag's spelling from
    clang's *must use 'struct' tag* and compile again. The artifact was the
    first draft; the object clang linked was the second.

    **The repair is to make R1 literally true.** The `emit_c` block moves below
    the rounds and runs only when they passed; `write_c` gains `struct_tags` and
    `version` and re-renders through `emit.emit_maybe` — the same call
    `assemble.fused` uses for the object it compiles — so the bytes written are
    the bytes clang accepted. A build the machine cannot do is refused above, by
    the same `blamed` that judges a `build`. `produce.hero` stays at exactly 300
    lines; `artifact.hero` 108 to 117.

    **Why the artifact then stops depending on the machine.** Two machines that
    both have the header learn the same spelling and write the same bytes. A
    machine without it writes nothing and says why, and `emission` already skips
    `machine_lacks_the_library`. **Conditional on the machine, never dependent
    on it** — the distinction the withdrawn probe's own comment reached for and
    did not apply.

    **Measured on the Mac, compiler built from the lane's source:**

        run-fixedbugs-getaddrinfo-is-bindable  --emit-c   exit 0   bare addrinfo 0, struct addrinfo 10   -fsyntax-only 0
                                               (before:   bare 7, struct 0, -fsyntax-only 1 "use of undeclared identifier 'addrinfo'")
        nosuchheader_160.h                     --emit-c   exit 1   error[ffi_missing_header], no artifact written
                                               (before:   exit 0, 10468 bytes)
        examples/sqlite (tag is a typedef)     --emit-c   exit 0   byte-identical to the blessed file
        run/builtins (no extern)               --emit-c   exit 0   byte-identical to the blessed file

    **Linux x86-64, in the container**: the seed built, the lane's compiler
    built, and `getaddrinfo`'s artifact came out **byte-identical to the Mac's**
    — `cmp` silent — with the same 10 `struct addrinfo *` and 0 bare. The
    fabricated missing header is exit 1 there too. That is the whole claim the
    withdrawn probe could not make.

    **Windows:** the box was offline. The arm it would exercise — a missing
    header → refusal — is the fabricated-header row above, run on the Mac. CI's
    Windows leg is the judge of the real `netdb.h` case.

    **What the suite said, and every row was an expected consequence.** First
    run: **43 red**. Eighteen were `tests/golden/ir/` fixtures — modules with no
    `main` — dying at exit **2**, *Undefined symbols: _main*, because the round
    LINKS and `--emit-c` has no binary to link. That is the repair's own second
    half: `Site` gains `link: bool`, false for `--emit-c`, and both round paths
    stop after the compile, which is where a tag's spelling is learned anyway.
    Second run: **25 red**, and the split is exactly the design — **22 to
    retire** (programs the build refuses: 20 wrong FFI bindings, plus
    `ir/owned-cell` binding a name `err.h` does not declare and
    `ir/regression-extern-parameter-types` declaring `abs`'s parameter wider than
    the header's `int`) and **3 whose bytes legitimately changed**.

    **Fifteen of the 22 retired files were blessing C that does not compile**,
    measured with `-fsyntax-only`. That is the class `docs/work/milestones/M-package-manager.md`
    counted at 28 of 240 and could not see, and retiring them is what records the
    decision — the suite's own rule since defect 048.

    The 3 re-blessed were diffed before `UPDATE_EMISSION=1` ran: **every changed
    line names one of the program's own tags and nothing else** — 48 lines all
    naming `gauge` or `probe`, 20 and 20 all naming `addrinfo` — and all three
    now pass `-fsyntax-only` where the blessed files did not. `emission` **478 to
    456 passed, 0 failed**, the difference being exactly the 22.

    **The wall clock that item asked whoever built this to measure first**:
    `emission` goes **56.45 s to 76.30 s**, +35%, `/usr/bin/time -p` on a still
    machine, `real`/(`user`+`sys`) 1.13 and 1.12. The cost is that `--emit-c` now
    compiles every TU instead of rendering a string. It stays in `emission`: the
    suite is one of twenty and the full net runs once before a push.

    **What this does NOT close.** Panel 157 R4 — build both ways, compare exit
    code and stdout, and LINK — stays adopted and unbuilt at M-package-manager.
    It is the instrument that would have FOUND 048 and 049; this is the repair
    of what it would have found. `emission` still blesses bytes and a byte
    comparison still cannot see that bytes do not compile; what changed is that
    the bytes it blesses for a tagged binding are now the ones that did.

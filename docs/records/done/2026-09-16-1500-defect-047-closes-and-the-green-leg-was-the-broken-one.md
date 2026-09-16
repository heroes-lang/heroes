- [x] **047 — defect 045's blame line is measured on one platform and fails on the other two** | `tests/golden/surface-fixtures/nullread/` was green on Darwin, named the wrong caller on Linux, and produced no message at all on Windows | **CLOSED 2026-09-16**, repaired on all three and measured on all three | `docs/panel/156-the-blame-line-was-never-a-platform-fact.md` · `runtime/parts/stack.c` · `runtime/parts/os.c` · `tests/harness/suite_surface.hero`

    **Origin:** the author, 2026-09-16, who asked why CI was broken. Panel 156 was
    convened on it by the author when the repair's direction turned out to be
    contested, and every seat delivered.

    **It was three defects wearing one test, and the third was found by the
    repair itself.**

    | | before | after |
    |---|---|---|
    | macOS arm64 | 134, `7`, `called from node_value` | 134, `7`, `at offset 0x0`, `called from main.main` |
    | Linux x86-64 | 134, **stdout empty**, `called from main.main` | 134, `7`, `at offset 0x0`, `called from main.main` |
    | Windows x86-64 | **139, both streams empty** | 127, `7`, the sentence and `at offset 0x0` |

    **The repair, and none of it is in `selfhost/`.**

    **1. The frame walk, and the GREEN leg was the broken one.** `node_value` is
    a C symbol with no `h_` prefix, so it was never an answer: it is `first`, the
    fallback `hero_stack_blame` returns when the walk finds no Heroes name. The
    suite had been pinning that failure as the required output. The cause is an
    **ISA fact and not a platform one**, instrumented by panel 156's
    compiler-engineer: on AArch64 a leaf function saves no frame record —
    `node_value` compiles to `sub sp / str / ldr / ldr x0,[x8]` with no
    `stp x29, x30` — so at the fault x29 still points at `h_main_main`'s own
    frame and the walk asks *who called `h_main_main`*. x86-64's `call` pushes a
    return address unconditionally, so the identical walk lands one frame lower.
    **Linux was right by accident of its instruction set, and Linux arm64 would
    have failed the same way.** The repair is the link register: `hero_stack_regs`
    gains an `lr` out-parameter (`__ss.__lr` on Darwin arm64, `regs[30]` on Linux
    arm64, 0 elsewhere) and `hero_stack_blame` probes `dladdr(lr - 1)` once before
    the chain.

    **2. The lost output, and it is kept by BUFFERING and never by flushing.**
    POSIX.1-2024 § 2.4.3's async-signal-safe list contains `write()` and does not
    contain `fflush()`, and C § 7.22.4.1 leaves flushing on `abort` implementation-
    defined — POSIX downgraded its own *"shall include the effect of `fclose()`"*
    to *"may"* for exactly this reason, and glibc removed flush-on-abort at 2.27
    citing *"deadlocks and data corruption"*. So nothing stdio enters the handler;
    `hero_err_unbuffered` becomes `hero_streams_survive_abort` and stdout joins
    stderr.

    **3. Windows had no arm for this fault at all.** The vectored handler tested
    `ExceptionAddress == 0`, which is the PC — a null pointer being CALLED,
    defect 013. A null pointer READ THROUGH has a valid PC and the touched address
    in `ExceptionInformation[1]`, which no arm read, so the exception fell to
    `EXCEPTION_CONTINUE_SEARCH`. **And `HERO_NULL_WINDOW` lived inside
    `#elif !defined(_WIN32)`**, invisible to that platform, so the arm could not
    have been written without hoisting it. Panel 156's ffi-pragmatist built and
    measured the arm on the box; the constant is hoisted above the platform split.

    **THE THIRD DEFECT, FOUND BY THE REPAIR AND ONLY ON THE THIRD MACHINE.** The
    first draft wrote `setvbuf(stdout, NULL, _IOLBF, 0)` for every platform —
    correct on POSIX, green on two machines of three. On Windows **it kills the
    process**: Microsoft's CRT hands a size of 0 to its invalid-parameter handler,
    which terminates at once and silently. Measured with three probes, one per
    call so no abort could hide another: `_IOLBF, 0` is **exit 127 with nothing on
    either stream**, while `_IOLBF, 4096` and `_IONBF, 0` both return 0 and print.
    The compiler built from that file did not survive `heroes doctor`. And passing
    a size repairs nothing, because that CRT implements `_IOLBF` as FULL
    buffering — so the line would still be in the buffer when the process died.
    **`_IONBF` is the only mode on that platform that keeps the promise**, which
    is why the call is a split and not one line. `.claude/rules/platforms.md`'s
    rule paid for itself inside one hour.

    **What the test now asserts, and one half of it is STRONGER.**
    `suite_surface.hero`'s row drops `called from node_value` — the name is an
    **optimisation-level** fact, not a platform one: the same program on the same
    Mac says `node_value` at `-O0` and `main` at `-O2`, because `h_main_main` is
    inlined away, and real headers ship `static inline` bodies that vanish the
    same way. It gains `at offset 0x0`, which is the other half of the contract
    panel 156 R1 set, and **keeps `out_is("7\n")`**, which was true on one platform
    when it was written and is now measured true on all three.

    **Verified**: the full net **1892 passed, 0 failed** across 24 suites; the
    compiler's own tests **654**; `surface` 109 on macOS; and the fixture run by
    hand on all three machines with the numbers in the table above. **The seed was
    regenerated in the same commit** (`seed/README.md`: if the diff touches
    `selfhost/`, it is), 819,718 to 820,349 lines, and the fixpoint verified —
    the compiler built from the new seed re-emits it byte-identical.

    **What is deliberately NOT closed.** Windows still has no frame walk, so no
    `called from` there: `SymFromAddr` needs dbghelp initialised before the fault
    and a PDB beside the binary, and panel 156 R1 makes the name best-effort
    rather than contract. And a field offset of 1 MiB is still 139-silent on
    Windows, the gap inherited from defect 045 unchanged — no better and no worse.

- [x] **045 — a null handle handed to a C function that reads through it SEGFAULTS, which §1.12 forbids outright** | `cstr` arguments are guarded on their way out and handles are not, so a null handle reaching a header that dereferences it built clean and died at exit 139 with nothing on either stream | **repaired 2026-09-15**, panel 154 R1 and R2, ratification pending | `selfhost/cli/flags.hero` · `runtime/parts/stack.c`'s `HERO_NULL_WINDOW` and its handler

    **Origin:** measured 2026-09-15 by **panel 153's completeness critic**, as
    the one cost of a read route five seats had missed, and it got worse twice
    while it was being repaired — once by me, attacking the shape beside my own
    first repair, and once by panel 154's completeness critic, which falsified
    three sentences of the brief I wrote for it.

    **THE CLASS WAS NEVER ARGUMENT-SHAPED, and that is the whole finding.** The
    sitting offered five routes and every one legislated the ARGUMENT at a Heroes
    call site: guard every handle argument, guard with a word to opt out, guard
    with a word to opt in, infer from the marks already there, or refuse a
    literal `nullptr` at compile time. Four of five seats converged on the
    second. **The class is a DEREFERENCE**, and it reaches three places an
    argument rule cannot: a null `ptr` argument, which no route could refuse
    without refusing a shipped golden; a null reached THROUGH a non-null handle,
    which is defect 042's own list-walk shape; and a field past the runtime's
    window.

    **WHAT REPAIRED IT IS ONE STRING IN THE FLAG LIST.**
    `-fno-delete-null-pointer-checks`. The standard lets clang assume a pointer
    it dereferences is non-null and delete every check downstream, so the fault
    was folded away entirely — and with it the program's honesty. Measured at
    `-O2`, before and after:

    | witness | before | after |
    |---|---|---|
    | one call through a null handle | **exit 0, prints `8372224`**, five runs of five identical | exit 134, named |
    | two calls, the fixture | exit 133, both streams empty, the correct `7` lost too | exit 134, stdout `7`, named |
    | a null behind a non-null handle | exit 133, both streams empty | exit 134, named |
    | a null **`ptr`** argument | **exit 0, prints `8372224`** | exit 134, named |

    **It guards the dereference and not the argument, so it refuses nothing.**
    `free(NULL)`, `sqlite3_close(NULL)` and the shipped
    `getaddrinfo(hints: nullptr)` golden all stay legal. A deterministic wrong
    answer at exit 0 is the shape a golden would have recorded and passed on
    forever, and two of the four rows above were exactly that.

    **What it costs, measured with the machine still**: twenty `--emit-c` runs of
    `examples/ledger/`, median `real` 2.87 s with the flag and 2.87 s without;
    10^9 iterations of a chain walk carrying three redundant null checks the
    optimiser would otherwise delete, `real` 1.04 s both; the seed +16,528 bytes,
    0.38%; the emitted C **byte-identical**; the compiler's own tests green
    either way.

    **AND THE WINDOW MY FIRST REPAIR SHIPPED RESTED ON A PREMISE ABOUT THE
    WORLD.** It read *"a field's offset is smaller than a page in every struct a
    header can lay out"*, and `.claude/rules/module-shape.md` forbids resting a
    narrowing on one. Three lines of header falsified it — `struct bignode { char
    pad[1048576]; int64_t value; }` faults at offset 1 MiB and died at 139 in
    silence, at `-O0` — and the critic then laid out 275 records from 21 headers
    of this SDK and found exactly one over a page, `_opaque_pthread_t` at 8192
    bytes: the premise was false and the corpus that would have caught it does
    not exist. `HERO_NULL_WINDOW` is now the platform's own floor, **Darwin's
    measured `__PAGEZERO` of 4 GiB** here and 64 KiB elsewhere, and the message
    prints the faulting offset so a reader can tell a null plus a field from a
    small wild pointer. Measured after: the 1 MiB field says `at offset
    0x100000, called from big_value` at both levels. **Panel 104's wild store
    still re-raises**, because the stack lives near the top of the address space.

    **What was adopted and NOT landed, with the reason.** Route A's guard — a
    null check on every handle argument, the symmetric twin of `guard_arguments`
    for `cstr` — is adopted and waits on its own clock, because the flag closes
    the class and the guard buys one thing the flag does not: the BLAME LINE.
    Measured: at `-O0` the panic names `node_value`, and under the flag at `-O2`
    the same program says `main`, because inlining flattened the frame. Route B's
    fourth parameter word is deferred on the evidence rule the compiler-engineer
    stated and the sitting was held to — *two is a class and one is a witness* —
    since exactly one shipped program would be refused today.

    **Two corrections this repair owes and pays.** `guard_cstr_arguments` does
    not exist: the function is `guard_arguments`, `selfhost/emit/ops.hero:244`.
    The stale name was in `.claude/rules/c-boundary.md`, and I copied it from
    there into this defect's own entry and into both of panel 154's briefs, so a
    citation nobody greps propagated into a sitting's framing. And the `-O2` row
    of this entry was false of the fixture it cited: the shape decides, and the
    five-row table in the sitting is what settles it.

    **What stays open and is named rather than closed by silence.** A
    hand-written struct whose touched field sits past the floor on Linux or
    Windows, where 64 KiB is an inference and not a measurement
    (`.claude/rules/platforms.md`: a platform fact is run on a platform). No
    record in this SDK is past it. The falsifier is a program that reads one and
    dies at 139 in silence; what closes it is each floor read on its own machine.

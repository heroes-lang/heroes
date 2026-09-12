- [x] **013 — a callback declared `ptr` and handed `nullptr` compiles and segfaults** | 2026-09-05, panel 112's spec-warden; the price of the proposed repair measured by the coordinator the same hour | **repaired 2026-09-05 on POSIX; the Windows twin is scheduled** | panel 111's llm-ergonomist registered it as a prediction at the sitting that opened the permission, and it came true six hours later | **§1.12**: *a Heroes program must not segfault*, which design.md calls a goal of the language rather than a quality of its implementation

    **THE REPRODUCER**, three lines, exit **139**:

    ```
    extern "stdlib.h"
        function atexit(f: ptr) -> i32

    function main()
        print("before")
        _ = atexit(nullptr)
    ```

    Prints `before`, then dies with no message. `qsort(… compar: ptr)` handed
    `nullptr` is the same shape.

    **THE CAUSE.** Panel 111 R4 made the *right* spelling legal —
    `f: (function() -> ())`, which refuses `nullptr` at the call site with
    `type_mismatch` — and left the *wrong* one legal beside it. Declared `ptr`, a
    callback parameter accepts any `ptr`, and `nullptr` is one. C then calls
    address zero. That is half a repair, and the half that shipped is the half
    that adds a route rather than the half that closes one.

    **WHY THE OBVIOUS REPAIR IS NOT FREE, and this is the measurement that keeps
    the entry open.** The warden proposed `-Wpedantic` as a fifth entry in
    `selfhost/emit/extern_probe.hero`'s four-entry `PROBE_ERRORS`, inside the
    pragma block the probes already carry. It is exactly right in direction:
    clang says *"converts between void pointer and function pointer"* on the
    author's own line, and design.md §4.19's own sentence says the pedantic flag
    *"waits until callbacks have a typed route"* — a condition R4 met. The seat
    measured zero false fires on `curl`, `ctime`, `nbody` and `spectral`.

    **The coordinator ran it over the rest of the corpus and it fires twice on
    `examples/sqlite/main.hero`**, which declares `sqlite3_exec`'s callback as
    `ptr` and passes `nullptr` at `:56` — and that program is **correct**. SQLite
    documents a null callback as *do not call back*. So the flag does not
    separate the segfault from the deliberate NULL, because at the declaration
    they are the same program.

    **The two routes out, and neither is free:**

    1. **Land the flag and rewrite the example.** Measured: it cannot be
       rewritten at the typed spelling and keep what it demonstrates, because
       `nullptr` does not inhabit a function type — `atexit(nullptr)` against
       `f: (function() -> ())` is `type_mismatch`, verified. The example would
       have to grow a real Heroes callback it does not need.
    2. **Let `nullptr` inhabit a function type.** A spec change (`spec:62` says
       it is *"the null of both"*), a panel path, and it does **not** close this
       defect: `atexit(nullptr)` would compile again and segfault again. The
       difference between the two programs is whether the C function tolerates
       NULL, and no compiler on either side of the boundary knows that.

    **What is owed**: a decision between those two, or a third nobody has found.
    It is the author's, and it is put to them with panel 112 rather than
    defaulted.

    **THE COORDINATOR'S RECOMMENDATION, and it is to repair NEITHER yet.** Route 1
    is worse than "rewrite an example": measured, `examples/sqlite/main.hero`
    **cannot be written at the typed spelling at all**, because `sqlite3_exec`'s
    callback takes `char **` and that is Part 8 wart 19's pointer-to-pointer cause
    — 18 occurrences in that header, no Heroes spelling. So landing the flag today
    does not cost one edit, it makes a **correct shipped program inexpressible with
    no route back**. Route 2 is a spec change that leaves the defect standing by
    its own analysis. And the two together still leave it standing, because
    `atexit(NULL)` segfaults in C as well: whether NULL is legal is a property of
    the C function, not of the type, and no vocabulary on either side of the
    boundary carries it.

    **So the repair is BLOCKED ON WART 19's pointer-to-pointer cause, not merely
    undecided**, and that is the finding rather than a delay. The day `char **`
    has a spelling, `examples/sqlite` can be typed, the probe stops converting a
    void pointer to a function pointer, `-Wpedantic` lands with no correct program
    to break, and what is left is the deliberate NULL — which is a program bug C
    shares and which should then be a **note on the diagnostic**, not a refusal.
    Shipping a flag that breaks a correct program with no route back would be the
    cheapest resolution available and the least robust, which is the trade
    CLAUDE.md §4 exists to refuse.

    **What is NOT owed**: the cast. Two seats measured a mismatched
    function-pointer cast under this project's own fourteen flags at zero
    diagnostics, exit 0, and an arbitrary permutation. design.md §4.19 forbids it
    by name and panel 112 R5 refuses it permanently.

    **THE REPAIR, 2026-09-05, and it is not the one this entry was filed
    expecting.** Both routes the entry priced were refusals, and both were
    blocked. The repair that landed changes the question from *how do I prevent
    it* to *how does the program say what happened* — which is exactly what
    panel 104 did for the stack, in the same handler, on the line above.

    **The witness is the faulting PC, not the faulting address.** A program that
    calls through a null function pointer does not touch a bad address: it goes
    to EXECUTE at address zero. Measured on this Mac, one probe, four shapes:

    ```
    a wild store to address 0                si_addr 0        pc NONZERO
    a wild read from address 0               si_addr 0        pc NONZERO
    a call through a NULL function pointer   si_addr 0        pc ZERO
    a call through a garbage pointer         si_addr 0x1234   pc NONZERO
    ```

    Three of the four give `si_addr == 0` and exactly one gives `pc == 0`.
    `si_addr` is the witness panel 104 already rejected once, when a wild store
    was reported as a stack overflow; this is the same lesson, in the same file,
    checked before rather than after.

    **What it costs: nothing anybody pays.** Zero spec tokens, zero compiler
    lines, no ABI move (`HERO_RUNTIME_ABI` stays 21 — no declaration changed),
    and **`sqlite3_exec(callback: nullptr)` is untouched**, because a correct
    program never faults and so never reaches the handler. That is what neither
    blocked route could offer.

    **Measured firing, two platforms, one witness program** —
    `qsort(base: buf, n: 4, size: 8, compar: nullptr)`, which glibc does not
    check the way it checks `atexit`:

    ```
    macOS   panic: a null function pointer was called …   exit 134
    glibc   panic: a null function pointer was called …   exit 134
    ```

    **And the adjacent shapes are untouched**, verified rather than assumed on
    both: a stack overflow still says `panic: stack exhausted in deep.down`, an
    out-of-range index still says `panic: array index out of range`, and the
    `abort-null-cstr-into-c` and `ffi-callback-c-calls-back` goldens still exit
    134 and 0.

    **What the three platforms taught, and it is this entry's real finding.**
    The originally filed program, `atexit(f: ptr)` handed `nullptr`, does three
    different things: **segfault on macOS**, **`Fatal glibc error: … assertion
    failed: func != NULL` on Linux**, and **exit 0 on Windows**, where the CRT
    checks. So NULL-tolerance is not a property of the type, nor even of the C
    function — it is a property of the C **library on that machine**. No
    compiler on either side of the boundary can carry that, which is why a
    compile-time refusal was never available. And on Linux `--sanitize` already
    named it on the author's own line before this repair existed.

    **The Windows twin was withheld for two hours and then landed**, when the
    author turned the box back on. It was held out because § Commands says an
    unrun platform fact is an inference, and shipping it unrun would have put a
    possible compile error on the Windows leg at the next TAG. Measured there:
    it compiles (seed from C alone, exit 0, zero errors) and it fires. **But not
    against either witness that works elsewhere** — the Windows CRT validates
    BOTH `qsort` and `atexit` and terminates the process before anything reaches
    address zero, which is a fourth platform behaviour on top of the three above.
    What reaches it is a callback that does not go through the CRT at all,
    `static inline void run_callback(void (*f)(void)) { f(); }` in a header the
    group names — which is what any third-party library looks like. The adjacent
    shapes are untouched on Windows too.

    **And one thing was measured rather than assumed before the branch went in**:
    `runtime/runtime.c` compiled on this Mac before and after is **byte-identical**,
    `md5` equal, because the branch is preprocessed away here. That is what let
    the net running at the time still answer for the tree.

    **The case**: `tests/golden/fixedbugs/ffi-a-null-function-pointer-says-so.hero`,
    with no `.expected` and no `run/` twin on purpose — the program dies on two
    platforms and exits 0 on the third, so a run golden would be red on one leg
    by design.

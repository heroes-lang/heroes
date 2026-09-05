# DEFECTS — the compiler defects that are still open

Read by whoever attacks a defect. Every item here is a **measured** failure of
the compiler on a program — a crash, a wrong answer at exit 0, a silence where
a message is owed — with its reproducer, its cause where known, and what is
owed. It exists because the author said so on 2026-09-03: *"I do not like the
defect directory … if anything is still open in defect at the end, make one
single file called DEFECTS.md inside work, so that everything is tidy"*.
Seven files under the old defect directory became this list and six entries in
`docs/work/DONE.md` that evening; the directory is gone.

**Only open defects live here.** The moment one is repaired its entry is ticked,
gains a *The repair* section with the measurements that prove it, and moves to
`docs/work/DONE.md` — the record — as every other list in this directory does
(CLAUDE.md §3). A defect that stays here after its fix is the shape §3 was
amended to prevent.

**One notation: `- [ ]`.** A finding written as a bare bullet is invisible to
every count in this project. The body of an entry is indented under its line;
it may be long, because a defect's reproducer, cause and measurements are the
entry, not decoration.

Format: `- [ ] **NNN — <title>** | <date, found by> | <status> | <where it came from> | <severity>`, then the body.

**One defect is open as of 2026-09-05**, filed by panel 112 and put to the author with it, because both of its candidate repairs cost something a measurement cannot choose between. The paragraph below was written when the list was empty and is kept: it is the record of what closed before it.

**Nothing was open here earlier on 2026-09-05.** Defect 011 (two `@` arguments of one
place compiling and running) and defect 012 (`function f(_: ())` reaching clang
as `void h0_`, exit 2) were both repaired that night and are in
`docs/work/DONE.md`. This line is a date and not a boast: an empty list is worth
exactly as much as the last search that filled it, and the searches that found
these two were a record audit and a panel seat measuring something else.

- [ ] **013 — a callback declared `ptr` and handed `nullptr` compiles and segfaults** | 2026-09-05, panel 112's spec-warden; the price of the proposed repair measured by the coordinator the same hour | open | panel 111's llm-ergonomist registered it as a prediction at the sitting that opened the permission, and it came true six hours later | **§1.12**: *a Heroes program must not segfault*, which design.md calls a goal of the language rather than a quality of its implementation

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

# Panel 195, ffi-pragmatist

Read `00-shared.md` first, then defect 382's issue file whole, then
`runtime/heroes_runtime.h` (the layout assert and `HERO_STR_STATIC`),
`runtime/parts/str.c:120-150`, `runtime/parts/array.c:55-100` and
`runtime/parts/drop.c:15-110` in your copy.

Your seat judges the C boundary and the platforms (design.md §1.11, §1.12,
§4.19) and holds a veto on ABI breakage.

1. **Write and compile the C every route on the table needs**, by hand, on
   this Mac with the compile flags the compiler uses (`selfhost/cli/flags.hero`,
   `flags()`), under `-fsanitize=address,undefined` and once under `-fsanitize=thread` with the constant read from two threads at once
   (through `hero_thread_spawn`, as `00-shared.md` says); and in
   the `heroes-linux-arm64` Docker image (`docker run --rm -v <your dir>:/w
   heroes-linux-arm64 ...`). Report what compiles, what each sanitizer says,
   and instructions retired for the reproducer.
2. **Read-only memory**: a `static const` block whose count an atomic load
   reads. Say on which platforms the block lands in a read-only section
   (`otool -l`/`objdump -h`, the section of the symbol), and what happens when
   today's `hero_array_decref` (no guard) is handed it: run it and report.
3. **The Windows box** (`ssh win`, Git Bash, clang 23.1.1; your folder
   `/c/w/p195-ffi-<pid>`, `df -k /c` showing 10 GB free before you copy, never
   remove anything): the same C under clang there, at least compile and run
   the reproducer; a box that does not answer is reported unrun. **A Windows
   leg of batch 12 runs on the box at the same time in `/c/w/b12-*`: do not
   touch it, and keep your load light (the box has 2 cores).** Hand-written C
   and `runtime/runtime.c` only, one clang at a time; never build the 37 MB
   seed there (the critic's reading of the box's load).
4. **The ABI and a binding's C**: does a header a program binds (defect 361's
   guard) see any new word? Does a program built against runtime 27 and
   linked with the new one, or the reverse, fail loudly (the `_Static_assert`
   on `HERO_RUNTIME_ABI`), and does the route need that stamp to move?

Verdict: approve, object or veto per route, with the section, the cost, a
falsifiable prediction and its condition. Your report:
`<scratchpad>/p195/reports/ffi-pragmatist.md`, written as you go.

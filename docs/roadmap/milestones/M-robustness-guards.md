# M-robustness-guards — the guards that shut the holes *(closed 2026-09-03)*


**Done 2026-09-03**, tag `m-robustness-guards`; the record is
[journal 031](journal/031-robustness-guards.md). What a later milestone has to
honour:

- **Robust over cheap, on every platform, and never a `-Wno-` flag** (author
  instruction 2026-09-03, four sentences, `docs/roadmap/scheduling.md`). A landing is
  measured on the Mac, the Linux image and the Windows box BEFORE its commit;
  a flag that hides what clang saw is not one of the options a sitting may choose.
  **Excepted once, by the author, 2026-10-09**: `-Wno-infinite-recursion`,
  panel 199's R3, put to the author after the ratification because the sitting
  had not seen this rule. The checker's `endless_recursion` is its witness:
  clang would warn only on a function that passed that check, one whose path
  holds a call that may end the program, where its reading of the C cannot be
  true or false by itself; and since defect 508's repair an endless recursion
  aborts at every level instead of hanging. No other `-Wno-` flag.
- **The clang floor is 18** (`selfhost/cli/clang_floor.hero`; panel 103, author
  leave): the CI's Ubuntu leg, and the oldest clang whose `-ast-dump=json` shape
  the pointee check was measured on. Raising it owes a measurement of what the
  next-oldest CI leg builds with; lowering it promises a dump shape nobody read.
- **`HERO_RUNTIME_ABI` is 19** and the bump is a two-phase edit — emitter, then
  header, then the seed regenerated in the same commit (`seed/README.md`).
- **The stack guard is process-wide and names the function on POSIX only**
  (`runtime/parts/stack.c`): a C library's own thread that overflows is the hole
  M-isolated-threads owns, and Windows names the failure and not the function
  until dbghelp and a PDB are measured on the box.
- **Compiling a leaf below a nested program's root re-bases its `use` paths** —
  a stated rule with a fixture (`tests/golden/surface-fixtures/nested/`), not a
  defect; the historian's standing prediction on it (Zig #13970's shape) is
  scored the day a defect of that shape is filed.

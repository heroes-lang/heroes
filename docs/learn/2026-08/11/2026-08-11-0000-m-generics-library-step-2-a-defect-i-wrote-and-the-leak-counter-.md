- [ ] **M-generics-library step 2** | **A defect I wrote and the leak counter's magic word caught.** `hero_map_grown` first did `memcpy` of the live entries plus `free` of the old block *without* dropping — the reasoning being that the references pass to the new block unchanged, so no descriptor should run. Every printed answer was correct and the program panicked at exit with `not a Heroes string block`. Task: say what that reasoning got right, and name the second place it put refcount arithmetic

    **Where to look:** runtime/runtime.c (hero_map_grown) · DESIGN-LOG 2026-08-11
    **Why it matters:** the fix is shorter than the bug and does no arithmetic at all

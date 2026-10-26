# Panel 230, the ffi-pragmatist's brief

Read `00-shared.md` beside this file first; its last section says what the critic's first pass
changed. You judge the founding constraint (design.md §1.11, §4.19, §4.20): everything comes from C,
and the C a route implies is written and compiled, or shown not to build. You hold a veto on ABI
breakage.

**Where you work**: `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/230-ffi-pragmatist`,
detached at `595d92d88`; its compiler built there (`clang -I runtime seed/heroes.c runtime/runtime.c
-o heroes`, then `./heroes build selfhost/main.hero -o heroes-next`). Scratch:
`.claude/worktrees/230-ffi-pragmatist-scratch/`. Your report is
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/panic-location/docs/panel/230-reports/ffi-pragmatist.md`,
**written as you go, from your first finding** (a seat stopped by a rate limit keeps only what is on
disk). No file outside the repository's root, no credential printed, no paid run, **no clock**: a
size or a time to measure is prepared and handed to the coordinator, who holds the machine still with
the other two sessions.

**What you are asked, each with the C written and compiled, or the command that settles it:**

1. **The place record in C**: its declaration in `runtime/heroes_runtime.h` (the field types,
   `const`, where its strings live), the emitted `static const` beside a function, and the entry
   points' new signatures, `_Noreturn` and cold; compiled under `selfhost/cli/flags.hero`'s `flags()`
   **as written** (`-Wall`, nine `-Werror=` words, `-Wno-infinite-recursion`; no blanket `-Werror`,
   the critic's finding 31) **and again with `-Werror` added**, at `-O0` and `-O2`. What
   `HERO_RUNTIME_ABI` 30 → 31 breaks, and that a runtime of 30 against a unit of 31 stops at the
   `_Static_assert` rather than linking.
2. **Kind 3, the aborts no program call stands behind** (`00-shared.md`): the stack guard
   (`stack.c:374`, `hero_stack_names` at `:544-561`), the lease handlers (`os.c:211-426`), the
   past-the-buffer report (`lend.c:651-699`), the exit check (`alloc.c:272-404`). What of a place
   each can print async-signal-safely (`write`, `_write`, no `fprintf`, no allocation): L4's names,
   L5's image-relative address beside the sources key (`hero_program_sources`, panel 222), and
   whether L2's line table can be read there at all.
3. **The line table on each platform**: on Darwin a built binary carries none (the critic, finding
   5: the DWARF stays in `build/`'s objects, reached through the debug map; no `dsymutil` in
   `selfhost/`), so what L2 and L5 cost there (`build/` kept, or a `dsymutil` step and its time); on
   Linux (ELF, DWARF in the binary?); on Windows (a PDB, `SymFromAddr`, `SymGetLineFromAddr64`;
   `stack.c:850-857`: unmeasured on the box). A platform you cannot run is written *unrun*.
4. **The C boundary for S1**: a group record's fields (a `cstr`, a `ptr`, a handle, a union member, a
   fixed array of bytes), a `cstr` that may dangle: write the C a renderer would be for a group
   record and show it reads no foreign memory, under `--sanitize` and, on Darwin, Guard Malloc. A
   `partial` record is refused by `==` (spec § 13), so never a side.
5. **The statuses**: what `abort()` exits with on Darwin (134, measured here) and Linux (no run in
   this sitting: cite one or write it unrun); Windows' `0xC0000409` (panel 174's brief, 2026-09-21,
   by `__fastfail`, which `os.c:241-247` keeps; a re-run is the author's box); what `exit(code:)`
   gives past 0 to 255 on each (defect 2224: 256 → 0 on Darwin; Windows' statuses are 32 bits wide,
   unrun); whether a C caller (`system`, `posix_spawn`, `WEXITSTATUS`) can tell a `main` that failed
   from an abort; and what M1's `exit 1` does with § 13's exit check (a lease live when `main`
   returns a `fail`).
6. **M5's route as C**: a program binding `hero_write_err` from `hero_os.h` today (the critic wrote
   one, `error: no_key: port`, exit 1); whether that binding is a supported route or an accident the
   sitting should close or name, on the three platforms.
7. **Every route's binary**: the text and read-only data of a program with many sites before and
   after the place records (`size`, `otool -l` or `llvm-size`), prepared for the coordinator.

**Your output**: `verdict` (approve | object | veto) per route, the section you stand on, the C you
compiled with its command and what it printed, `prediction`, `condition`.

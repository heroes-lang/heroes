# Panel 197, compiler-engineer

Read `00-shared.md` beside this file first, whole, and the critic's first pass
`docs/panel/197-reports/completeness-critic-pass1.md`. Your directory is
`<scratchpad>/197-compiler-engineer/`.

You judge the ceiling and the cost of every route (A) to (L), and of any route
you find that the list missed. Counts only, with the instrument the shared
brief names.

1. **Clang's memory on today's emission of the 400- and 800-return shapes**
   (the shape: *a function of N strings, each followed by an `if` that returns
   it*, `docs/panel/190-briefs/00-shared.md:38-40`; write the generator, emit
   with `--emit-c`, then compile the emitted unit alone): at `-O0` and `-O2`,
   under `-g`, `-gline-tables-only`, `-g -fno-standalone-debug` and the other
   words a route names, the peak footprint (read twice), instructions retired
   and object size. Say how far today's C is from the card's history number.
   800 runs on this Mac only, alone (the shared brief's room rule).
2. **Defect 219's depth on the real emission**: `run/fixedbugs-170-variants-32-deep-build`,
   `run/fixedbugs-170-variants-and-options-100-deep-build` and the
   `run/fixedbugs-140-*` cases show the shape; generate it at 2,000, 3,000,
   5,000 and 10,000 and, per route, say which depth compiles, which dies,
   **which phase dies** (`-emit-llvm -S` against `-c`) and **on which
   declaration** (the critic's hand shape died in the backend through a
   file-scope variable of the top type). Try (E) on the real emission: emit
   bottom-up declarations for the debug-info writer the way `typeorder.hero`'s
   `in_order` does for the type conversion, prototyped in your copy's
   `selfhost/emit/`, and measure it at 3,000, 5,000 and 10,000 on this Mac and
   in Docker. For (A), say what each platform allows (the shared brief's (A)).
3. **The compiler's own build under each route**: the units of
   `./heroes build selfhost/main.hero` replayed alone (their command lines
   from the build), summed instructions and peak footprint of the largest
   unit, and the objects' total size under `build/tu-*/` (Darwin's binary
   carries no DWARF).
4. **lldb on this Mac**, on a small program of three functions and a loop:
   `b <file>.hero:<line>`, `run`, `next`, `step`, `finish`, `bt`, `p <name>`,
   `frame variable`, under `-g`, `-gline-tables-only`, and (E) where it differs,
   at `-O0` (`lldb --batch -o ...`). Record what each prints. Say whether line
   stepping is identical, whether `b file.hero:N` still resolves, and what the
   column under (K) means. **Defect 335 is open**: stepping out of a `return`
   lands on the `function` line under every route.
5. **The cache key and the tests**: under each route, where the debug word
   enters `compiler_key` and `build_words` (`toolchain.hero:120`, `:134-135`)
   so no object built under one word is reused under another, and which
   `flags.hero` and `compiling.hero` tests change.
6. **Build the route you adopt** in your copy and run the shared brief's cases
   (`run/fixedbugs-170-*` and `-140-*`, the `lines` suite, the compiler's own
   tests narrowed to `flags` and `compiling`, the `run` suite's
   `!sanitizer:` cases, the hand lldb run). A route that does not build is not
   adopted.

Your verdict names the route you would adopt, its cost in the units above, and
what each other route would cost, including what it does to M-typed-inspection
and M-vscode-extension. Veto on soundness only.

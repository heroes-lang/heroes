# Panel 197, ffi-pragmatist

Read `00-shared.md` beside this file first, whole, and the critic's first pass
`docs/panel/197-reports/completeness-critic-pass1.md`. Your directory is
`<scratchpad>/197-ffi-pragmatist/`.

You judge the C boundary under each route (A) to (L). Write the C a real
binding produces: a Heroes program with an `extern` group over a header of
your own holding a struct and a `static inline` function, built by `heroes
build` (so the header's code is compiled under the unit's own flags), and the
same built under each route's debug word. Counts only, with the instrument the
shared brief names.

1. **Stepping into the header's `static inline` code** in lldb on this Mac
   under `-g`, `-gline-tables-only` and (E): does `step` enter it, does `bt`
   name it with file and line, does `p` read its locals.
2. **The sanitizers' reports through the compiler's own path** (objects kept
   under `build/`, no `.dSYM`, a debug map): a heap use-after-free and a
   signed overflow planted **in the header's `static inline`**, not in a
   separately built library, under `--sanitize` (`-fsanitize=address,undefined`,
   `flags.hero:200`, on compile and link, `link.hero:136`): does each report
   keep its file, line and function under each route, on this Mac and in Docker
   `heroes-linux-arm64`. The critic's one-step hand run (with a `.dSYM`) found
   UBSan's line independent of the debug word and ASan's needing line tables;
   say whether the compiler's path agrees. Note that the net's 21
   `!sanitizer:` needles name no location.
3. **The runtime's abort paths**: `hero_panic` prints `panic: <msg>` with no
   location (`runtime/parts/panic.c:39-43`); the stack guard names a function
   by `dladdr` on POSIX (`runtime/parts/stack.c:371-395`) and nothing on
   Windows (`:725-735`); **`flags.hero:174` says Darwin names the symbol
   because of `-g`**. Run the guard's message (a recursion past the stack) on
   this Mac and in Docker under each route, and say whether `dladdr`'s name
   depends on the debug word.
4. **The Windows box**: on one emitted file of a small program with an
   `extern` group, built there by hand with the compiler's compile and link
   words under `-g` and `-gline-tables-only`: the object's and the `.pdb`'s
   sizes, `llvm-pdbutil dump -l -symbols` on each (lines and locals present or
   not), and `lldb.exe` (installed there; the CI once recorded it exiting
   `0xC0000135`) setting a breakpoint by file and line and printing a local.
   Whatever lldb does, the `.pdb` reading stands.
5. **The 400-return shape on Linux arm64** in Docker under `-g` and
   `-gline-tables-only` at `-O2`, read with `/sys/fs/cgroup/memory.peak` (one
   clang per `docker run`), twice each, so the Mac's number has a second
   platform.

Your verdict names the route you would adopt from the boundary's side, and
any route that loses a binding's debugging or a sanitizer's location. Veto on
ABI breakage or a lost report.

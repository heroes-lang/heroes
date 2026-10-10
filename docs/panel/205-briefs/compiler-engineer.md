# Panel 205, compiler-engineer's brief

Read `00-shared.md` first. Build in your copy the routes you judge worth it
for both questions and run them on `p205/sw`, `p205/gnu` (Linux, in the
container), `p205/gmpmac`, panel 204's Expect with libjpeg case
(`.claude/worktrees/scratch-b15/critic-204b/ex/`, read only) and over every
tracked `.hero` file with an `extern` (which program moves, at which exit
code, and whether `selfhost/main.hero`'s `--emit-c` stays byte-identical):
the cost in files and lines (`layout`'s unit) and in instructions on `build`.
Say what the runtime's own C needs from the prefix (could the groups' headers
come first, or a program's own header before the prefix, without breaking
`heroes_runtime.h`?), and which `-Werror` a package's own code must still
meet for the compiler's promises to hold (`ffi_parameter_type` and its
siblings ask clang about a header; does a system directory change their
answers?). A route you recommend is built and run before you report.

# Panel 243, historian

Read `00-shared.md` first, whole. You have no worktree; web search and fetch are your
instruments, and every precedent you write is marked `verified` with its URL and the day you
read it, or left out.

- Q1: how do other languages' C foreign-function interfaces hold an argument passed to a C
  variadic: Rust (its rule on passing `f32`, `i8`, `i16`, `bool` to a variadic, and the error
  that refuses them), Zig (`@cVaArg`, its variadic calls), Go's cgo (variadic C functions),
  Swift (`CVarArg`, `withVaList`), Nim (`varargs` pragma), D, Python ctypes; and what C
  compilers' `-Wformat` checks and does not (widths against types). For each: what is refused,
  what is converted, what is left to the programmer.
- Q2: how do build systems and compilers tell a library's own include folder (from `pkg-config`,
  Homebrew keg-only formulae and their caveats) from the SDK's: Homebrew's documentation on
  keg-only, clang's and GCC's search order for `-I` against `-isystem`, and any language
  toolchain that refuses a header shadowing a system one.
- Q3: the C standard's text and clang's documentation on where a quoted `#include` is searched
  first (the includer's directory), and whether `../` paths are read the same way.

Report: one row per precedent, `verified` with URL and day, what the page itself says.
